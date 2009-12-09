#include "StdAfx.h"

/*
	Construct - DirectX 9 Game and Application creation
    Copyright (C) 2007 Scirra

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

// The bin allocator
runtime_allocator g_allocator;

#ifdef CONSTRUCT_DEBUGGER
void CRuntime::LogMsg(CString msg)
{
	// Try/catch in case current event is not applicable
	int ev;
	if (pCurrentEvent == NULL) {
		ev = -1;
	}
	else {
		try { ev = pCurrentEvent->number; } catch (...) { ev = -1; }
		/*
		try {
			if (pCurrentEvent->pCurCnd == NULL)
				cnd = -1;
			else
				cnd = (*(pCurrentEvent->pCurCnd))->number;
		} catch (...) { cnd = -1; }		
		*/
	}

	pLogPage->AddLogMessage(msg, ev, -1, g_actionIndex);
}

#define LOGMSG(m) LogMsg(m)
#else
#define LOGMSG(m)  
void CRuntime::AddDebuggerItem(const char* label, const char* value, bool readOnly)
{
}
void CRuntime::AddDebuggerCombo(const char* label, const char* items, int sel)
{
}
#endif

void CRuntime::ReallocDisplayTargetTexture(TextureHandle& tex)
{
#ifndef APPRUNTIME
	if (tex != unallocated_texture)
		tex = CreateDisplayTargetTexture();
#endif
}

void CRuntime::SafeFreeTexture(TextureHandle& tex)
{
#ifndef APPRUNTIME
	if (tex != unallocated_texture)
		renderer.DeallocateTexture(tex);
#endif
}

TextureHandle CRuntime::CreateDisplayTargetTexture()
{
#ifdef APPRUNTIME
	return (TextureHandle)-2;
#else
	return renderer.CreateRenderTargetTexture(winWidth, winHeight);
#endif
}

// Flush 'delayed' to 'instances' for this type.
void CRunObjType::FlushDelayed()
{
	if (delayed.empty())
		return;

	ObjIterator i = delayed.begin();
	ObjConstIterator delayed_end = delayed.end();

	for ( ; i != delayed_end; i++)
		instances.push_back(*i);

	delayed.resize(0);
}

// Flush 'Delayed' vector to the 'Instances' vector for all types.
void CRuntime::FlushDelayedObjects()
{
	// Add the delayed-add instances to the instances lists.  Fixes spawning self.
	ObjTypeIterator t = objects.begin();
	ObjTypeConstIterator objects_end = objects.end();

	for ( ; t != objects_end; t++) {
		if ((*t)->delayed.empty())
			continue;

		ObjIterator i = (*t)->delayed.begin();
		ObjConstIterator delayed_end = (*t)->delayed.end();

		for ( ; i != delayed_end; i++) {
			(*t)->instances.push_back(*i);

			if (system.loading)
				(*i)->info.destroying = true;
		}

		(*t)->delayed.resize(0);
	}

	delayedObjectsExist = false;
}

void CRuntime::FlushDeathRow()
{
	// Actually deallocate all objects sitting on death row
	if (!(DeathRow.empty())) {

		ObjIterator i = DeathRow.begin();
		ObjConstIterator DeathRow_end = DeathRow.end();

		for ( ; i != DeathRow_end; ++i)
			// Perform actual removal and deallocation
			DestroyRunObject(*i);

		DeathRow.resize(0);
	}//if
}

// Handle builtin attributes frame by frame, eg. Center view on me
void CRuntime::HandleBuiltinAttributes()
{
	// Get a list of objects with "Center view on me" attribute, and do centering.
	CRunObjType* pCenters = GetTypeFromName("Center view on me");
	if (pCenters != NULL) {
		if (!pCenters->instances.empty()) {
			
			float totalX = 0, totalY = 0;
			
			ObjIterator i = pCenters->instances.begin();
			ObjConstIterator instances_end = pCenters->instances.end();

			// Total up the X and Y coords
			for ( ; i != instances_end; i++) {
				totalX += (*i)->info.x;
				totalY += (*i)->info.y;
			}

			// Average out the center point
			totalX /= pCenters->instances.size();
			totalY /= pCenters->instances.size();

			CRunLayout* pLayout = pCenters->instances.front()->pLayout;

			pLayout->SetScrollX(totalX - (pLayout->winWidth / 2));
			pLayout->SetScrollY(totalY - (pLayout->winHeight / 2));
		}
	}

	// Check for "Destroy if far" objects
	CRunObjType* pDestroys = GetTypeFromName("Destroy if far");
	if (pDestroys != NULL) {
		if (!pDestroys->instances.empty()) {

			ObjIterator i = pDestroys->instances.begin();
			ObjConstIterator instances_end = pDestroys->instances.end();

			for ( ; i != instances_end; i++) {
				// Bounds check:  if a window size away, destroy the object.
				float x = (*i)->info.x;
				float y = (*i)->info.y;
				CRunLayout* pLayout = (*i)->pLayout;

				if (x < -winWidth || y < -winHeight || x > pLayout->width + winWidth || y > pLayout->height + winHeight)
					DestroyObject(*i);
			}
		}
	}
}

// Verify for debug purposes that the SOL is completely clean.
/*
void CRuntime::DebugTempFunction()
{
	ObjTypeIterator t = objects.begin();
	ObjTypeIterator end = objects.end();

	for ( ; t != end; t++) {
		if ((*t)->pSol->selectAll == false)
			__asm int 3;
	}
}
*/


////////////////////////////////
// Run the event list
void CRuntime::RunEventList()
{
	// Run events for each running frame.
	list<CRunLayout*>::iterator f = runningFrames.begin();
	list<CRunLayout*>::iterator frames_end = runningFrames.end();

	for ( ; f != frames_end; ) {

		// Skip events for a blocked system frame
		if (systemFrameBlocked && (*f)->systemDrawn) {
			f++;
			continue;
		}

		// Set the system object current frame
		system.pLayout = *f;

		EventIterator i = (*f)->Events.begin();
		EventConstIterator Events_end = (*f)->Events.end();

		isTopLevelTrigger = false;

		// Run each event in the root list
		// This will naturally recurse via any CEventParents
		for ( ; i != Events_end; ++i)
		{
			CEventLine* pEvent = *i;

			// Run event
			pEvent->Run();
			
			// If this event made any SOL modifications,
			// make sure they are rolled back for the next event.
			if (pEvent->modifiesSol)
				ClearSOL(pEvent->modifiers);

			// Flushing delayed objects is only done by top-level events - this is when it is safe to modify
			// the instance lists.  This means created objects only enter the instances list after the next top-level event
			if (delayedObjectsExist)
				FlushDelayedObjects();

			// Verify SOL for debugging purposes
#ifdef SOL_DEBUGGING
			ObjTypeIterator t = objects.begin();
			ObjTypeIterator end = objects.end();

			for ( ; t != end; ++t) {
				// Verify pSol points at front element of SOL
				ASSERT((*t)->pSol == &((*t)->sols.front()));

				// Verify SOL is empty
				ASSERT((*t)->pSol->selectAll);
			}
#endif
		}

		isTopLevelTrigger = true;

		// A frame change is requested
		if (changeFrame > -1) {

			// Check if frame is invalid
			if (changeFrame >= Frames.size()) {
				changeFrame = -1;
				f++;
				continue;
			}

			bool wasSystemDrawn = (*f)->systemDrawn;

			FlushDelayedObjects();

			GenerateEvent(-1, SYSTEM_ENDOFFRAME, NULL);
			(*f)->DestroyNonGlobalObjects();

#ifdef CONSTRUCT_DEBUGGER
			pObjectPage->RefreshInspection();
			pWatchPage->RefreshWatch();
			pObjectPage->UpdateObjectTree();
#endif

			FlushDeathRow();

			CRunLayout* pNewFrame = Frames[changeFrame];

			// Find all still-existing objects (like globals) and update their frame pointers where necessary
			ObjTypeIterator t = objects.begin();
			ObjTypeConstIterator objects_end = objects.end();

			if (pNewFrame != *f) for ( ; t != objects_end; t++) {
				ObjIterator i = (*t)->instances.begin();
				ObjConstIterator instances_end = (*t)->instances.end();

				for ( ; i != instances_end; i++) {
					// Holds old frame
					if ((*i)->pLayout == *f) {

						// Update frame pointer
						(*i)->pLayout = pNewFrame;

						int layer = (*i)->info.layer;

						if (layer >= 0 && layer < (*f)->layers.size()) {
							// Move the layer iterator across to new frame
							LayerObjList& oldZorder = (*f)->layers[layer]->instances;

							// Not enough layers - put on top layer
							if (layer >= pNewFrame->layers.size()) {
								layer = pNewFrame->layers.size() - 1;
								(*i)->info.layer = layer;
							}

							LayerObjList& newZorder = pNewFrame->layers[layer]->instances;
							LayerObjIterator& rIter = *((*i)->info.pLayerIterator);

							if (&rIter != NULL) {
								oldZorder.erase(rIter);
								newZorder.push_back(*i);
								rIter = newZorder.end();
								rIter--;

								// Move to same layer on the next frame
								ToLayer(*i, (*i)->info.layer);
							}
						}

						// On Frame Changed
						(*i)->OnFrameChange((*f)->number, pNewFrame->number);
					}
				}
			}

			// Kill all waiting objects to be destroyed
			FlushDeathRow();

			// Unload old frame
			(*f)->Unload();

			// Replace this running frame
			*f = pNewFrame;
			changeFrame = -1;

			(*f)->Load();
			(*f)->CreateInitialObjects();

			system.everyEvents.clear();
			system.expiredEqualities.clear();

			(*f)->systemDrawn = wasSystemDrawn;

			// Start of Frame
			system.pLayout = *f;

			FlushDelayedObjects();

			GenerateEvent(-1, SYSTEM_STARTOFFRAME, NULL);
		}
		else
			f++;
	}

	// Once all events have run we can reset the registered collisions state
	registeredCollisions.clear();
}

void CRuntime::ScreenToLayout(const CRunLayout* pLayout, float& x, float& y) const
{
	if (pLayout == NULL) return;

	double realWinWidthOffset = ((double)realWinWidth * (double)winWidthOffset) / (double)winWidth;
	double realWinHeightOffset = ((double)realWinHeight * (double)winHeightOffset) / (double)winHeight;
	
	// Normalise screen position
	double Xpercent = (double)(x - realWinWidthOffset) / (double)realWinWidth;
	double Ypercent = (double)(y - realWinHeightOffset) / (double)realWinHeight;
	Xpercent -= 0.5;
	Ypercent -= 0.5;

	// Apply scaling
	double Xscale = pLayout->zoomX;
	double Yscale = pLayout->zoomY;

	double zoomedWindowWidth = (double)winWidth / Xscale;
	double zoomedWindowHeight = (double)winHeight / Yscale;

	// Convert from normalised to layout coordinates
	double scrollCentreX = pLayout->scrollX + winWidth / 2;
	double scrollCentreY = pLayout->scrollY + winHeight / 2;
	x = Xpercent * zoomedWindowWidth;
	y = Ypercent * zoomedWindowHeight;

	if (pLayout->curAngle != 0.0)
		RotatePoint(x, y, RADIANS(0 - pLayout->curAngle));

	x += scrollCentreX;
	y += scrollCentreY;
}

void CRuntime::ScreenToLayer(const CRunLayout* pLayout, int layer, float& x, float& y) const
{
	if (pLayout == NULL) return;
	if (layer < 0 || layer >= pLayout->layers.size()) return;

	CRunLayer* pLayer = pLayout->layers[layer];

	double realWinWidthOffset = ((double)realWinWidth * (double)winWidthOffset) / (double)winWidth;
	double realWinHeightOffset = ((double)realWinHeight * (double)winHeightOffset) / (double)winHeight;

	// Normalise screen position
	double Xpercent = (double)(x - realWinWidthOffset) / (double)realWinWidth;
	double Ypercent = (double)(y - realWinHeightOffset) / (double)realWinHeight;
	Xpercent -= 0.5;
	Ypercent -= 0.5;

	// Apply scaling (old incorrect method)
	//double Xscale = ((pLayout->zoomX - 1.0) * pLayer->zoomXf) + 1.0;
	//double Yscale = ((pLayout->zoomY - 1.0) * pLayer->zoomYf) + 1.0;
	double Xscale;
	double Yscale;
	
	
	Xscale = pLayout->zoomX * pLayer->zoomXoffset;
	Yscale = pLayout->zoomY * pLayer->zoomYoffset;
	if(pLayer->zoomXf != 1)
		Xscale = pow(Xscale, (double)pLayer->zoomXf);
	if(pLayer->zoomYf != 1)
		Yscale = pow(Yscale, (double)pLayer->zoomYf);	

	double zoomedWindowWidth = (double)winWidth / Xscale;
	double zoomedWindowHeight = (double)winHeight / Yscale;

	// Convert from normalised to layout coordinates
	double scrollCentreX = pLayout->scrollX * pLayer->scrollXf + winWidth / 2;
	double scrollCentreY = pLayout->scrollY * pLayer->scrollYf + winHeight / 2;
	x = Xpercent * zoomedWindowWidth;
	y = Ypercent * zoomedWindowHeight;

	if (pLayer->currentAngle != 0.0)
		RotatePoint(x, y, RADIANS(0 - pLayer->currentAngle));

	x += scrollCentreX;
	y += scrollCentreY;
}

// Updating input state
void CRuntime::UpdateInputState()
{
//#ifdef APPRUNTIME
	
	// Render all running frames.
	list<CRunLayout*>::iterator f = runningFrames.begin();
	list<CRunLayout*>::const_iterator frames_end = runningFrames.end();

	POINT mouse;
	GetCursorPos(&mouse);

	if (!fullscreen)
		ScreenToClient(hWnds.front(), &mouse);

	for ( ; f != frames_end; f++) {

		float mx;
		float my;

#ifdef CONSTRUCT_DIRECTX9
		if (motionBlur) {
			mx = linearMouseX;
			my = linearMouseY;
		}
		else {
			mx = mouse.x;
			my = mouse.y;
		}
#else
		mx = mouse.x;
		my = mouse.y;
#endif

		ScreenToLayout(*f, mx, my);

		(*f)->mouseX = mx;
		(*f)->mouseY = my;

		vector<CRunLayer*>::iterator y = (*f)->layers.begin();
		vector<CRunLayer*>::iterator layers_end = (*f)->layers.end();

		for ( ; y != layers_end; y++) {
			
#ifdef CONSTRUCT_DIRECTX9
			if (motionBlur) {
				(*y)->mouseX = linearMouseX;
				(*y)->mouseY = linearMouseY;
			}
			else {
				(*y)->mouseX = mouse.x;
				(*y)->mouseY = mouse.y;
			}
#else
			(*y)->mouseX = mouse.x;
			(*y)->mouseY = mouse.y;
#endif

			ScreenToLayer((*f), y - (*f)->layers.begin(), (*y)->mouseX, (*y)->mouseY);
		}
	}

#ifdef CONSTRUCT_DIRECTX9 // DirectX runtime
	DIMOUSESTATE dmouseState;

	d_keyboard->GetDeviceState(sizeof(UCHAR[256]), (LPVOID)(inputState.directState));
	d_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&dmouseState);

	inputState.mouseState[0] = dmouseState.rgbButtons[0];
	inputState.mouseState[1] = dmouseState.rgbButtons[2];	// DInput uses 2 for middle, 1 for right
	inputState.mouseState[2] = dmouseState.rgbButtons[1];
#endif

}

//Window procedure
LRESULT CALLBACK CRuntime_WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	// Pointer to CRuntime
	static CRuntime* pRuntime = NULL;

	// Check if this hWnd doesn't belong to CRuntime: pass to relevant plugin
	if (pRuntime) {

		LRESULT ret = pRuntime->SendMessageToPlugins(hWnd, uMessage, wParam, lParam);

		if (ret != -2)
			return ret;

	}//if

	// Check for first create to obtain pRuntime
	if (uMessage == WM_CREATE && pRuntime == NULL)
		pRuntime = (CRuntime*)((CREATESTRUCT*)lParam)->lpCreateParams;

	// Else handle the message as mine
	if (pRuntime)
		return pRuntime->WindowProc(hWnd, uMessage, wParam, lParam);

	// Or whatever
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

//Window procedure
LRESULT CALLBACK CRuntime_SSProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	// Pointer to CRuntime
	static CRuntime* pRuntime = NULL;

	// Check if this hWnd doesn't belong to CRuntime: pass to relevant plugin
	if (pRuntime) {

		LRESULT ret = pRuntime->SendMessageToPlugins(hWnd, uMessage, wParam, lParam);

		if (ret != -2)
			return ret;

	}//if

	// Check for first create to obtain pRuntime
	if (uMessage == WM_CREATE && pRuntime == NULL)
		pRuntime = (CRuntime*)((CREATESTRUCT*)lParam)->lpCreateParams;

	// Else handle the message as mine
	if (pRuntime)
		return pRuntime->WindowProc(hWnd, uMessage, wParam, lParam);

	// Or whatever
	return DefWindowProc(hWnd,uMessage,wParam,lParam);
}

LRESULT CRuntime::SendMessageToPlugins(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	HWND msgHwnd = hWnd;

	// Command message: get HWND from lParam
	if (uMessage == WM_COMMAND)
		msgHwnd = (HWND)lParam;

	// Message not for me
	//if (msgHwnd != pRuntime->GetAppHwnd()) {

	// Find and call the right plugin window
	list<PluginWindowInfo>::iterator i;
	const list<PluginWindowInfo>::iterator pluginWnds_end = pluginWnds.end();

	for (i = pluginWnds.begin(); i != pluginWnds_end; i++) {

		// Message for this plugin's hWnd?
		if (i->hWnd == msgHwnd) {

			_D_ENTER(DCS_PLUGIN, i->pObject);

			// Call and return
			LRESULT ret = i->pObject->WindowProc(hWnd, uMessage, wParam, lParam);

			_D_RUNTIME();

			// -2 will allow another object or the runtime to handle the message
			if (ret != -2)
				return ret;

		}//if
		// If a NULL window registered, pass all messages but ignore return value
		else if (i->hWnd == NULL) {

			_D_ENTER(DCS_PLUGIN, i->pObject);

			i->pObject->WindowProc(hWnd, uMessage, wParam, lParam);

			_D_RUNTIME();
		}
	}

	return -2;		// Message still active
}

// Handle this message as definitely for the runtime itself
LRESULT CRuntime::WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
#ifdef APPRUNTIME
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;
#endif

	// Disabled alt presses to stop freezing
	if (uMessage == WM_SYSKEYDOWN) {
			
		// was Alt pressed?  Bit-29 of lParam is documented as:
		// "The value is 1 if the ALT key is down while the key is pressed"
		if (disableAlt && lParam & (1<<29)) {
			// yep, Alt was pressed so tell Windows to stop all further 
			// processing of this message.
			return 0;
		}
	}

    //Check message
    switch(uMessage)
    {
    case WM_DESTROY:

		// Close app
		bRunning = false;

        break;

	case WM_SIZE:

#ifdef CONSTRUCT_DIRECTX9

		// If window sized and not fullscreen, change the display resolution so backbuffer doesn't stretch
		if (!fullscreen) {

			// Windows sends a WM_SIZE on startup...
			if (firstFrame || expectedResize)
				break;

			if (resizable == resize_showmore && (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)) {
				
				system.changeResWidth = LOWORD(lParam);
				system.changeResHeight = HIWORD(lParam);
				system.changeResFullscreen = -1;	// no change

				// Only change if window size has actually changed
				if (system.changeResWidth != winWidth || system.changeResHeight != winHeight)
					system.changeresolution = true;
			}
			
		}
#endif
		break;
		/*
	case WM_MOVING:
	case WM_SIZING:

		// Keep screen refreshed
		Frame(true, false);	// Present, don't handle more window messages
		
		break;
		*/


#ifdef APPRUNTIME

	case WM_PAINT:

		hdc = BeginPaint(hWnds.front(), &ps);

		GetClientRect(hWnds.front(), &rt);

		SetBkColor(hdc, bkColor);

		ExtTextOut(hdc, 0, 0, ETO_OPAQUE, CRect(0, 0, winWidth, winHeight), NULL, 0, NULL);

		EndPaint(hWnds.front(), &ps);

		return 0;

	// Use the input messages to update mouse coords
	// ONLY APP RUNTIME:  DirectX uses DirectInput
	case WM_LBUTTONDOWN:
		inputState.mouseState[0] = 0x80;
		break;
	case WM_LBUTTONUP:
		inputState.mouseState[0] = 0;
		break;
	case WM_MBUTTONDOWN:
		inputState.mouseState[1] = 0x80;
		break;
	case WM_MBUTTONUP:
		inputState.mouseState[1] = 0;
		break;
	case WM_RBUTTONDOWN:
		inputState.mouseState[2] = 0x80;
		break;
	case WM_RBUTTONUP:
		inputState.mouseState[2] = 0;
		break;
	case WM_KEYUP:
		inputState.keybState[wParam] = 0;
		break;
#endif


	case WM_KEYDOWN:

#ifdef APPRUNTIME
		inputState.keybState[wParam] = 0x80;
#endif
#ifdef CONSTRUCT_DEBUGGER
		if (wParam == VK_F11) {
			// Toggle debugger visibility
			if (pDebug->IsWindowVisible())
				pDebug->ShowWindow(SW_HIDE);
			else
				pDebug->ShowWindow(SW_SHOW);
		}
#endif
		break;

#ifdef CONSTRUCT_DIRECTX9
		// DirectX runtime: manually handle window dragging to prevent pausing
	case WM_SYSCOMMAND:

		// Move
		if ((wParam & 0xFFF0) == SC_MOVE) {

			if (!dragging) {

				dragging = true;

				// Get start mouse cursor point
				POINT p;
				GetCursorPos(&p);

				RECT rc;
				GetWindowRect(hWnds.front(), &rc);

				mouseXOffset = p.x - rc.left;
				mouseYOffset = p.y - rc.top;
			}
			return 0;
		}

		break;

#endif

	}//switch

	// Screensaver handling
	if (props.screensaver)
	{
		switch(uMessage)
		{
			case WM_SYSCOMMAND:
				if(wParam == SC_SCREENSAVE || wParam == SC_CLOSE)
					return FALSE;
			break;

			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_MOUSEMOVE:
				Close();

			break;
		}
	}

    return DefWindowProc(hWnd,uMessage,wParam,lParam);
}

////////////////////////////////
// Create an instance of an object OID
CRunObject* CRuntime::CreateRunObject(initialObject* et, int layer, CRunLayout* pLayout, bool createSiblings, CRunObject* linkTo)
{
	// Check valid OID
	if (et->objectID < 0 || et->objectID >= objects.size())
		throw runtime_error("Error creating an object.  The application may be corrupt.");

	// Get the object type
	CRunObjType* pObjType = objects[et->objectID];

	// Not allowed to create this type any more
	if (pObjType->disabled)
		return NULL;

	CRunObject* pRunObject = NULL;

	// Create the object
	pRunObject = pObjType->pPlugin->RTCreateObject(et, this);

	// Placement new a siblings ObjList and other construct datazone structures.  See comments.
	new (&(pRunObject->siblings)) ObjList;

#ifdef CONSTRUCT_DIRECTX9
	new (&(pRunObject->effects)) EffectList;
#endif

	pRunObject->pType = pObjType;								// Pointer to type
	pRunObject->info.oid = pObjType->oid;						// OID
	pRunObject->info.destroying = false;						// Initialise destroy flag
	pRunObject->info.layer = layer;								// Store layer
	pRunObject->pLink = NULL;									// No link to any object
	pRunObject->info.uid = curUid++;							// Set UID then increment
	pRunObject->pLayout = pLayout;
	pRunObject->info.collMode = COLLISIONMODE_FINE;				// Fine collisions by default
	pRunObject->info.imgHandle = NULL;							// No specified default image
	pRunObject->info.pInfo = new RunObjectInfoPlus;
	pRunObject->info.pInfo->collisionMaskHandle = NULL;					// No specified default mask
	pRunObject->info.pInfo->generatedMask = NULL;
	pRunObject->info.pInfo->skewX = 0.0f;
	pRunObject->info.pInfo->skewY = 0.0f;
	pRunObject->info.pInfo->z_elevation = 0.0f;
	pRunObject->info.curAnim = 0;
	pRunObject->info.displayangle = 0;
	pRunObject->info.global = false;
	pRunObject->info.deprecated_translation = TRANSLATE_SCROLL;	// No longer has any effect
	pRunObject->info.dynMask.curMask = NULL;
	pRunObject->info.global = pObjType->global;
	pRunObject->info.visible = true;
	pRunObject->info.pDisplayBoxOverride = NULL;
	pRunObject->info.isFlipped = false;
	pRunObject->info.isMirrored = false;
	pRunObject->info.UpdateAngle = 0;
	pRunObject->info.last_angle = -1;
	pRunObject->info.HotSpotX = 0;
	pRunObject->info.HotSpotY = 0;
	pRunObject->info.HotSpotAngle = 0;
	pRunObject->info.HotSpotDist = 0;
	
	
#ifdef PYTHON
	pRunObject->info.pInfo->pDictionary = PyDict_New();
#endif

#ifdef CONSTRUCT_DIRECTX9
	// Copy any effects from the type to the instance
	for (int e = 0; e < pObjType->effects.size(); e++) {
		pRunObject->effects.push_back(pObjType->effects[e]);
		InstanceEffect& ie = pRunObject->effects.back();

		ie.enabled = true;
		//ie.fx->InitialiseFrom(this, pObjType->effects[e]);
	}
#endif

	// By default, register to call OnFrame and OnFrame2
	if (callingOnFrames)
		calleeBuffer.push_back(pRunObject);		// Buffer as to not invalidate iterators
	else
		onFrameCallees.push_back(pRunObject);

	if (callingOnFrame2s)
		calleeBuffer.push_back(pRunObject);
	else
		onFrame2Callees.push_back(pRunObject);

	// Add this instance to the instance list of any family/trait the object is in
	ObjTypeIterator t = pObjType->teams.begin();
	ObjTypeConstIterator teams_end = pObjType->teams.end();

	for ( ; t != teams_end; t++)
		(*t)->instances.push_back(pRunObject);

	// Add instance to target layer and make iterator point to itself
	// Not valid for movement exts or non-drawing exts.
	if (pObjType->movement || pObjType->pPlugin->ideFlags & OF_NODRAW)
		pRunObject->info.pLayerIterator = NULL;
	else {
		pRunObject->info.z = pLayout->layers[layer]->instances.size();

		pRunObject->info.pLayerIterator = new LayerObjIterator;		// Allocate iterator
		pLayout->layers[layer]->instances.push_back(pRunObject);
		*(pRunObject->info.pLayerIterator) = pLayout->layers[layer]->instances.end();
		(*(pRunObject->info.pLayerIterator))--;
	}

	// Set the instance ID for this object
	pRunObject->info.iid = pObjType->instances.size() + pObjType->delayed.size();

	pObjType->delayed.push_back(pRunObject);
	delayedObjectsExist = true;
	//pObjType->instances.push_back(pRunObject);

	// Increment total object counter
	totalObjects++;

	// Creating siblings? Instance the sister types
	if (createSiblings && !(pObjType->sisterTypes.empty())) {
		t = pObjType->sisterTypes.begin();
		ObjTypeConstIterator sisterTypes_end = pObjType->sisterTypes.end();

		// Create sibling instances
		for ( ; t != sisterTypes_end; ++t) {
						
			CRunObject* pNew = CreateRunObject((*t)->createParams, layer, pLayout, false, pRunObject);
			pRunObject->siblings.push_back(pNew);

		}

		// Now for each sibling, update their siblings list
		ObjIterator i = pRunObject->siblings.begin();
		ObjConstIterator siblings_end = pRunObject->siblings.end();

		for ( ; i != siblings_end; i++) {

			// Add me to their siblings
			(*i)->siblings.push_back(pRunObject);

			// Check for movement?
			if ((*i)->pType->movement) {

				// Sister types contains the target type
				CRunObjType* pLinkType = (*i)->pType->pLinkType;
				
				// Movement for this object?
				if (pLinkType == pRunObject->pType) {
					// Attach the movement instance to this object being created
					(*i)->pLink = pRunObject;
				}
				// Must be for a sibling
				else {
					// Loop siblings looking for this type
					ObjIterator j = pRunObject->siblings.begin();

					for ( ; j != siblings_end; j++) {
						// Found the sibling to link to
						if (pLinkType == (*j)->pType) {
							// Link this sibling to the found sibling
							(*i)->pLink = (*j);
							break;
						}
					}
				}//else
			}//movement

			// Add the other siblings if any
			ObjIterator j = pRunObject->siblings.begin();

			for ( ; j != siblings_end; j++) {

				// Siblings do not contain themselves in their list
				if ((*j) != (*i))
					(*i)->siblings.push_back(*j);

			}//for

		}//for
	}// creating siblings

	// For when creating movement instances
	if (linkTo != NULL)
		pRunObject->pLink = linkTo;

	// The call that created siblings handles the OnCreate calls in the correct order (for movements to create properly)
	if (createSiblings) {

		_D_ENTER(DCS_PLUGIN, pRunObject);

		// Call OnCreate since everything is done now
		pRunObject->OnCreate();

		_D_RUNTIME();

		// Subsequently call sibling OnCreates
		ObjIterator s = pRunObject->siblings.begin();
		ObjConstIterator siblings_end = pRunObject->siblings.end();

		for ( ; s != siblings_end; s++) {

			_D_ENTER(DCS_PLUGIN, *s);

			(*s)->OnCreate();

			_D_RUNTIME();
		}
	}

#ifndef APPRUNTIME
	ImageHandleInfo* ih = (ImageHandleInfo*)pRunObject->info.imgHandle;
	if (ih != NULL)
		pRunObject->info.pInfo->collisionMaskHandle = &ih->cm;
#endif

#ifdef CONSTRUCT_DEBUGGER
	creationRow.push_back(pRunObject);
#endif

	// Return pointer
	return pRunObject;
}

////////////////////////////////
// Get pointer to OID
CRunObjType* CRuntime::pOid(int oid)
{
	return objects[oid];
}

////////////////////////////////
// Destroy an object instance
void CRuntime::DestroyRunObject(class CRunObject* obj, bool destroySiblings)
{
	CRunObjType* pType = pOid(obj->info.oid);
	int iid = obj->info.iid;

	// Remove from the type's instance list using instance ID
	ObjList& instances = pType->instances;
	instances[iid] = instances.back();			// Move last object to this object's place
	instances[iid]->info.iid = iid;				// Update instance ID to new position in vector
	instances.pop_back();						// Remove duplicated element

	// Remove from onFrame/onFrame2 lists if present
	ObjIterator i = find(onFrameCallees.begin(), onFrameCallees.end(), obj);

	if (i != onFrameCallees.end()) {
		*i = onFrameCallees.back();
		onFrameCallees.pop_back();
	}

	i = find(onFrame2Callees.begin(), onFrame2Callees.end(), obj);

	if (i != onFrame2Callees.end()) {
		*i = onFrame2Callees.back();
		onFrame2Callees.pop_back();
	}

	// Delete entry if instance held a window
	list<PluginWindowInfo>::iterator e = pluginWnds.begin();
	const list<PluginWindowInfo>::const_iterator pluginWnds_end = pluginWnds.end();

	for ( ; e != pluginWnds_end; ++e) {
		// Match rundata addresses
		if (e->pObject == obj) {
			pluginWnds.erase(e);
			break;
		}
	}

	// Remove from instance lists of any of the families/traits this object was a part of
	ObjTypeIterator t = obj->pType->teams.begin();
	ObjTypeConstIterator teams_end = obj->pType->teams.end();

	for ( ; t != teams_end; t++) {
		ObjIterator i = find((*t)->instances.begin(), (*t)->instances.end(), obj);

		if (i != (*t)->instances.end())
			(*t)->instances.erase(i);
	}

	// Free any cloned effects (cloning appears to only need freeing once)
	//for (int e = 0; e < obj->effects.size(); e++)
	//	obj->effects[e]->pEffect->Release();

	// Free a generated mask if any
	if (obj->info.pInfo->generatedMask) {
		FreeMask(obj->info.pInfo->generatedMask);
		delete obj->info.pInfo->generatedMask;
	}

	// Remove from the layer it was on and delete the stored iterator (if not movement)
	if (obj->info.pLayerIterator != NULL) {

		// 3D layering enables stable_sort which invalidates iterators
		//obj->pLayout->layers[obj->info.layer]->instances.erase(*(obj->info.pLayerIterator));
		CRunLayer* pLayer = obj->pLayout->layers[obj->info.layer];

		pLayer->instances.erase(find(pLayer->instances.begin(), pLayer->instances.end(), obj));

		delete obj->info.pLayerIterator;
		obj->info.pLayerIterator = NULL;
		obj->pLayout->layers[obj->info.layer]->zIndicesSorted = false;
	}

	// Destroy siblings if any
	if (destroySiblings && !(obj->siblings.empty())) {
		i = obj->siblings.begin();
		ObjConstIterator siblings_end = obj->siblings.end();

		// Don't destroy siblings in this loop - we're doing that already
		for ( ; i != siblings_end; ++i)
			DestroyRunObject(*i, false);
	}

	// Free any mask the object may have owned
	if (obj->info.dynMask.curMask) {
		FreeMask(obj->info.dynMask.curMask);
		delete obj->info.dynMask.curMask;
	}

	// Free the extended runtime data
	delete obj->info.pInfo;

	// Manually destruct the construct data classes, because the SDK doesn't see/deallocate the vector there.
	// See comments around CRunObject::siblings.
	obj->siblings.~vector();

#ifdef CONSTRUCT_DIRECTX9
	obj->effects.~vector();
#endif

	// Finally, deallocate the object
	pType->pPlugin->RTDestroyObject(obj);

	totalObjects--;
}

void CRunObjType::DeleteInstances()
{
	while (instances.size() > 0) {

		((CRuntime*)pRuntime)->DestroyRunObject(instances.back());
		
	}

	instances.resize(0);
}

void CRunObjType::MarkAllDestroying()
{
	ObjIterator i = instances.begin();
	ObjIterator end = instances.end();

	for ( ; i != end; i++)
		(*i)->info.destroying = true;
}

// Add trigger to the trigger map
CEventLine** CRuntime::AddTriggerToMap(CRunLayout* pLayout, CEventLine* ev)
{
	EventTriggerInfo evinfo = ev->GetTriggerInfo();
	int oid = evinfo.oid;
	oid++;	// OFFSET OID BY +1

	TriggerMap::iterator tm;
	TriggerList::iterator tl;
	ParamIndexList::iterator i;

	int s = pLayout->Triggers.size();

	// If the OID does not yet have an index, create one
	while (s <= oid) {
		TriggerList trl;
		pLayout->Triggers.push_back(trl);
		s = pLayout->Triggers.size();
	}

	// Get the iterator for this trigger map entry
	tm = pLayout->Triggers.begin() + oid;

	// Get the trigger condition ID
	int cndID = evinfo.cndID;

	// If cndID does not yet have an index, create one
	while (tm->size() <= cndID) {
		ParamIndexList pl;
		tm->push_back(pl);
	}

	// Get iterator for this trigger list entry
	tl = tm->begin() + cndID;

	// Now search the parameters list for a matching list of parameters.
	// If none found create entry
	for (i = tl->begin(); i != tl->end(); i++) {

		// Does this trigger match this parameter type?
		if (i->front()->TriggerMatch(evinfo.pCnd)) {

			i->push_back(ev);	// Add to the triggers-by-parameter list
			return &(i->back());
		}
	}

	// No existing entry found; add one
	ParamList pl;
	pl.push_back(ev);
	tl->push_back(pl);

	return &(tl->back().back());
}

// Fast triggers: get handle
FASTTRIGGER CRuntime::GetFastTriggerHandle(int oid, int cndID, ExpReturn* cndParamList)
{
	// Davo's 255 reserved ACEs
	if (oid != -1)
		cndID += 255;

	oid++;	// OFFSET OID BY +1

	// Use the triggers from the current active frame.
	TriggerMap& Triggers = system.pLayout->Triggers;

	// OID doesn't exist
	if (Triggers.size() <= oid) return NULL;

	// cndID doesn't exist
	if (Triggers[oid].size() <= cndID) return NULL;

	ParamIndexList* p = &(Triggers[oid][cndID]);

	// No events to handle
	if (p->size() == 0) return NULL;

	// Search ParamIndexList for a matching cndParamList
	ParamIndexList::iterator i;

	ExpressionList* paramsPtr;
	ExpressionIterator e;

	int n;
	bool match;

	// Loop every param index
	for (i = p->begin(); i != p->end(); i++) {
		
		// Test the first entry to check for a const parameter match
		paramsPtr = i->front()->GetParamsPtr();

		n = 0;
		match = true;

		// Test each parameter
		for (e = paramsPtr->begin(); e != paramsPtr->end(); e++) {

			// If not constant, cannot be fast triggered
			if (!((*e)->isConstant))
				break;

			// Parameters dont match - quit
			if ((*e)->constantValue != cndParamList[n]) {
				match = false;
				break;
			}

			n++;
		}

		if (match)
			return (long)&(*i);

	}//for each param index

	// None found
	return NULL;
}

// Generate event from fast trigger handle
void CRuntime::GenerateEventFast(FASTTRIGGER ft)
{
	//if (ft == NULL) return;

	FastTrigger f = (FastTrigger)ft;

	ParamList::const_iterator i = f->begin();

	// Run fast event
	const ParamList::const_iterator e = f->end();

	for ( ; i != e; ++i) {
		(*i)->RunFast();

		// Clear SOLs if this event modified any
		if ((*i)->modifiesSol)
			ClearSOL((*i)->modifiers);
	}
}

void CRuntime::GenerateEvent(int oid, int cndID, CRunObject* pCaller)
{
	// Davo's 255 reserved ACEs
	if (oid != -1) {

		// We need to trigger family events if this OID is not a family.
		CRunObjType* pType = objects[oid];

		// Non-family type: trigger family condition
		if (!pType->IsFamily()) {

			//bool wasTopLevelTrigger = isTopLevelTrigger;
			//isTopLevelTrigger = false;

			// Movement type
			if (pType->movement) {

				if (!pType->familyMovs.empty()) {
					ObjTypeIterator t = pType->familyMovs.begin();
					ObjTypeIterator end = pType->familyMovs.end();

					for ( ; t != end; t++)
						GenerateEvent((*t)->oid, cndID, pCaller);
				}

			}
			// Ordinary type
			else if (!pType->teams.empty()) {
				ObjTypeIterator t = pType->teams.begin();
				ObjTypeIterator end = pType->teams.end();

				for ( ; t != end; t++)
					GenerateEvent((*t)->oid, cndID, pCaller);
			}

			//isTopLevelTrigger = wasTopLevelTrigger;
		}

		// OFFSET CNDID BY 255
		cndID += 255;
	}

	oid++;	// OFFSET OID BY +1

	// Use the triggers from the current active frame.
	TriggerMap& Triggers = (pCaller == NULL ? system.pLayout->Triggers : pCaller->pLayout->Triggers);

	ParamIndexList::const_iterator i;
	ParamList::const_iterator j;

	// Triggers map doesn't have an OID for this object
	if (Triggers.size() <= oid)
		return;

	// No cndID for this trigger
	if (Triggers[oid].size() <= cndID)
		return;

	ParamIndexList* p = &(Triggers[oid][cndID]);

	// No triggers here
	if (p->size() == 0)
		return;

	const ParamIndexList::const_iterator p_end = p->end();

	bool wasTopLevelTrigger = isTopLevelTrigger;
	isTopLevelTrigger = false;

	// Else run all triggers
	for (i = p->begin(); i != p_end; ++i) {
		for (j = i->begin(); j != i->end(); ++j) {

			(*j)->Run(oid-1, pCaller);

			// Clear SOLs if last event modified the SOL with other conditions
			if ((*j)->modifiesSol)
				ClearSOL((*j)->modifiers);

			if (delayedObjectsExist && wasTopLevelTrigger)
				FlushDelayedObjects();
		}
	}
	
	isTopLevelTrigger = wasTopLevelTrigger;
}

// Generate an event inline - event will not clean up SOLs after itself!
void CRuntime::GenerateEventInline(int cndID, CRunObject* pCaller)
{
	int oid = pCaller->info.oid;

	// Davo's 255 reserved ACEs
	if (oid != -1)
		cndID += 255;

	oid++;	// OFFSET OID BY +1

	// Use the triggers from the current active frame.
	TriggerMap& Triggers = (pCaller == NULL ? system.pLayout->Triggers : pCaller->pLayout->Triggers);

	ParamIndexList::const_iterator i;
	ParamList::const_iterator j;

	// Triggers map doesn't have an OID for this object
	if (Triggers.size() <= oid)
		return;

	// No cndID for this trigger
	if (Triggers[oid].size() <= cndID)
		return;

	ParamIndexList* p = &(Triggers[oid][cndID]);

	// No triggers here
	if (p->size() == 0)
		return;

	const ParamIndexList::const_iterator p_end = p->end();

	// Else run all triggers
	for (i = p->begin(); i != p_end; ++i) {
		for (j = i->begin(); j != i->end(); ++j) {

			(*j)->Run(oid-1, NULL);		// No selection of this object for inline calls!

			// Inline event: do not clean up SOLs
			//if ((*j)->modifiesSol)
			//	ClearSOL((*j)->modifiers);
		}
	}
}

// Registering a plugin window
void CRuntime::RegisterWindow(HWND hWnd, CRunObject* pObject)
{
	// Save the data
	PluginWindowInfo ew;

	ew.hWnd = hWnd;
	ew.pObject = pObject;

	pluginWnds.push_back(ew);
}

void CRuntime::UnRegisterWindow(HWND hWnd, CRunObject* pObject)
{
	// Remove window from pluginWnds
	PluginWindowInfo ew;
	ew.hWnd = hWnd;
	ew.pObject = pObject;
	pluginWnds.remove(ew);
}

void CRuntime::CallPreTranslateMessage(CRunObject* pObject, bool bCall)
{
	if (bCall)
		pluginPreMsgHooks.push_back(pObject);
	else
		pluginPreMsgHooks.remove(pObject);
}

// Re evaluate a parameter in a condition
void CRuntime::ReEvaluateCndParameter(int index, ExpReturn* ret)
{
	(*(pCurrentEvent->pCurCnd))->params[index]->Evaluate(ret);
}

// Re evaluate a parameter in a condition with instance (For Each Object Ordered)
void CRuntime::ReEvaluateCndParameterWithInstance(int index, ExpReturn* ret, CRunObject* o)
{
	CExpression* exp = (*(pCurrentEvent->pCurCnd))->params[index];
	exp->overrideCalledOID = true;	// Crowbar this instance selection in
	exp->Evaluate(ret, o, 0);
}

CRunObjType* CRuntime::GetTypeFromName(const char* name) const
{
	CString s = name;
	s.MakeLower();

	ObjTypeList::const_iterator i = objects.begin();
	ObjTypeConstIterator objects_end = objects.end();

	for ( ; i != objects_end; i++) {

		CString objName = (*i)->Name;
		objName.MakeLower();

		if (objName == s)
			return (*i);
	}

	return NULL;
}

// A special replacement SOL
void CRuntime::SpawnSelect(CRunObject* parent, CRunObject* sibling)
{
	/*
	// Same types; do selection replacement
	if(parent->pType == sibling->pType)
	{
		SelObjList& rSol = *(sibling->pType->pSol);

		if (rSol.selectAll) {
			// Just select sibling
			rSol.selectAll = false;
			rSol.sol.resize(0);
			rSol.sol.push_back(sibling);
		}
		else {
			// Find parent, and replace it.
			ObjIterator i = find(rSol.sol.begin(), rSol.sol.end(), parent);

			if(i != rSol.sol.end())
			{
				*i = sibling;
				return;
			}
		}
	}
	else
		// Types differ; safe to Select()
		Select(sibling);
		*/
}

void CRuntime::Select(CRunObject* obj, int oid)
{
	Select(obj, objects[oid]);
}

// Add object to its SOL
void CRuntime::Select(CRunObject* obj, CRunObjType* pType)
{
	bool family = pType->teamID > -1;

	SelObjList& rSol = *(pType->pSol);

	if (rSol.selectAll) {

		rSol.selectAll = false;
		rSol.sol.resize(0);
		rSol.sol.push_back(obj);

		// Pick family container
		if (family) {

			int index = find_index(pType->instances.begin(), pType->instances.end(), obj);

			ObjTypeIterator t = pType->familyMovs.begin();
			ObjTypeIterator end = pType->familyMovs.end();

			for ( ; t != end; t++) {
				SelObjList& rFSol = *((*t)->pSol);
				rFSol.selectAll = false;
				rFSol.sol.resize(0);
				rFSol.sol.push_back((*t)->instances[index]);
			}

		}

		// Pick container
		if (obj->pType->contained) {

			ObjIterator i = obj->siblings.begin();
			ObjConstIterator siblings_end = obj->siblings.end();

			for ( ; i != siblings_end; i++) {
				SelObjList& rSibSol = *((*i)->pType->pSol);
				rSibSol.selectAll = false;
				rSibSol.sol.resize(0);
				rSibSol.sol.push_back(*i);
			}
		}
	}
	else {
		rSol.sol.push_back(obj);

		// Pick family container
		if (family) {

			int index = find_index(pType->instances.begin(), pType->instances.end(), obj);

			ObjTypeIterator t = pType->familyMovs.begin();
			ObjTypeIterator end = pType->familyMovs.end();

			for ( ; t != end; t++) {
				(*t)->pSol->sol.push_back((*t)->instances[index]);
			}

		}
		// Pick container
		if (obj->pType->contained) {

			ObjIterator i = obj->siblings.begin();
			ObjConstIterator siblings_end = obj->siblings.end();

			for ( ; i != siblings_end; i++) {
				SelObjList& rSibSol = *((*i)->pType->pSol);
				rSibSol.sol.push_back(*i);
			}
		}
	}
}

//  Check if object exists in SOL *if SelectAll is false*
bool CRuntime::IsSelected(CRunObject* obj, CRunObjType* pType)
{
	SelObjList& rSol = *(pType->pSol);

	// This function won't tell you if an object is selected via selectAll
	if (rSol.selectAll) return false;

	// Search
	ObjIterator i = find(rSol.sol.begin(), rSol.sol.end(), obj);

	return i != rSol.sol.end();
}

// Unselect object
void CRuntime::Unselect(CRunObject *obj, CRunObjType* pType)
{
	SelObjList& rSol = *(pType->pSol);

	// This function won't work on SelectAll
	if (rSol.selectAll) return;

	// Search
	ObjIterator i = find(rSol.sol.begin(), rSol.sol.end(), obj);

	// Fast erase
	if (i != rSol.sol.end()) {
		*i = rSol.sol.back();
		rSol.sol.pop_back();

		// Containers?  erase siblings as well
		if (pType->contained) {
			ObjIterator s = obj->siblings.begin();
			ObjConstIterator siblings_end = obj->siblings.end();

			for ( ; s != siblings_end; s++) {
				SelObjList& rSibSol = *((*s)->pType->pSol);
				*(find(rSibSol.sol.begin(), rSibSol.sol.end(), *s)) = rSibSol.sol.back();
				rSibSol.sol.pop_back();
			}
		}
	}
}

// Unselect all instances (no actions referring to this type will run)
void CRuntime::UnselectAll(CRunObjType *type)
{
	type->pSol->selectAll = false;
	type->pSol->sol.resize(0);

	// Unselect all for contained and family movement types
	if (!type->sisterTypes.empty()) {
		ObjTypeIterator t = type->sisterTypes.begin();
		ObjTypeIterator end = type->sisterTypes.end();

		for ( ; t != end; t++) {
			(*t)->pSol->selectAll = false;
			(*t)->pSol->sol.resize(0);
		}
	}

	if (!type->familyMovs.empty()) {
		ObjTypeIterator t = type->familyMovs.begin();
		ObjTypeIterator end = type->familyMovs.end();

		for ( ; t != end; t++) {
			(*t)->pSol->selectAll = false;
			(*t)->pSol->sol.resize(0);
		}
	}
}

void CRuntime::UnselectAll(CRunObject *obj)
{
	UnselectAll(obj->pType);
}

// Select all instances using selectAll flag
void CRuntime::SelectAll(CRunObjType *type)
{
	type->pSol->selectAll = true;

	// Select all for contained and family movement types
	if (!type->sisterTypes.empty()) {
		ObjTypeIterator t = type->sisterTypes.begin();
		ObjTypeIterator end = type->sisterTypes.end();

		for ( ; t != end; t++)
			(*t)->pSol->selectAll = true;
	}

	if (!type->familyMovs.empty()) {
		ObjTypeIterator t = type->familyMovs.begin();
		ObjTypeIterator end = type->familyMovs.end();

		for ( ; t != end; t++)
			(*t)->pSol->selectAll = true;
	}
}

bool CRuntime::CndsBefore(CNDS_PTR pCnds) const
{
	ConditionList& cndList = *(ConditionList*)pCnds;

	// Conditions come before if not the first element
	return (*(pCurrentEvent->pCurCnd) != cndList.begin());
}

bool CRuntime::CndsAfter(CNDS_PTR pCnds) const
{
	ConditionList& cndList = *(ConditionList*)pCnds;

	// Conditions come after if not the last element
	return (*(pCurrentEvent->pCurCnd) != (cndList.end() - 1));
}

// Run conditions
bool CRuntime::RunConditions(CNDS_PTR pCnds)
{
/*	ConditionList& cndList = *(ConditionList*)pCnds;
	ConditionIterator c = *(pCurrentEvent->pCurCnd);

	// Skip this condition
	++c;

	// Now run the rest of the conditions (user better have done NewSOLCopy if needed)
	for ( ; c != cndList.end(); ++c) {
		if (!c->Do())
			return false;
	}

	return true;*/

	ConditionList& cndList = *(ConditionList*)pCnds;
	ConditionIterator oldcnd = *(pCurrentEvent->pCurCnd);
	ConditionIterator* c = pCurrentEvent->pCurCnd;

	// Skip this condition
	++(*c);

	// Now run the rest of the conditions (user better have done NewSOLCopy if needed)
	for ( ; *(c) != cndList.end(); ++(*c)) {
		if (!(*c)->Do())
		{
			*c = oldcnd;
			return false;
		}
	}
	*c = oldcnd;
	return true;

}

// Layer code
CRunLayer::CRunLayer(CRuntime* _pRuntime)
{
	pRuntime = _pRuntime;
	scrollXoffset = 0;
	scrollYoffset = 0;
	zoomXoffset = 0;
	zoomYoffset = 0;
	scrollXf = 1;
	scrollYf = 1;
	zoomXf = 1;
	zoomYf = 1;
	zoomXoffset = 1;
	zoomYoffset = 1;
	visible = true;
	currentAngle = 0;
	zIndicesSorted = true;
}

void CRuntime::NewSOL()
{
	::NewSOL(&objects);
}

void CRuntime::NewSOLCopy()
{
	::NewSOLCopy(&objects);
}

void CRuntime::NewSOLCopy(ObjTypeList* modifiers)
{
	::NewSOLCopy(modifiers);
}

void CRuntime::RestoreSOL()
{
	::RestoreSOL(&objects);
}

void CRuntime::RestoreSOL(ObjTypeList* modifiers)
{
	::RestoreSOL(modifiers);
}

HBITMAP CRuntime::GetImageHBITMAP(int imgHandle)
{
	// App runtime simply does a lookup in to the hbitmaps table
#ifdef APPRUNTIME
	return imageBmps[imgHandle];
	// Game runtime will have to create a temp bitmap from the texture and load that
#endif

#ifdef CONSTRUCT_SDL
	return NULL;
	//TODOSDL
#endif

#ifdef CONSTRUCT_DIRECTX9
	HBITMAP hBmpRet = imageBmps[imgHandle];

	if (hBmpRet != NULL)
		return hBmpRet;

	else {
		ImageHandleInfo* ih = (ImageHandleInfo*)imgHandle;
		CString bmpTempFile = tempDir + "temp.bmp";
		D3DXSaveTextureToFile(bmpTempFile, D3DXIFF_BMP, ih->th->texture_ptr, NULL);

		hBmpRet = (HBITMAP)LoadImage(NULL, bmpTempFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		imageBmps[imgHandle] = hBmpRet;

		DeleteFile(bmpTempFile);

		return hBmpRet;
	}
#endif
}

int CRuntime::GetPrivateVariableCount(CRunObject* obj)
{
	if (obj == NULL) return -1;

	return obj->pType->privateVars.size();
}

void CRuntime::GetPrivateVariableInfo(CRunObject* obj, int index, int& type, const char*& str)
{
	if (obj == NULL) return;

	type = obj->pType->privateVars[index].type;
	str = (const char*)(obj->info.editObject->initVars[index]);
}

CRunLayout* CRuntime::CreateRunningFrame(int frameNum, HWND hWnd)
{
	// Push back a new running frame - fail if already exists
	if (frameNum < 0 || frameNum >= Frames.size())
		return NULL;

#ifdef CONSTRUCT_DIRECTX9
	// CreateAdditionalSwapChain fails on multihead rendering
	if (renderer.GetMultiMonitorMode() != cr::multimonitor_singlescreen)
		return NULL;
#endif

	CRunLayout* pLayout = Frames[frameNum];
	list<CRunLayout*>::iterator i = find(runningFrames.begin(), runningFrames.end(), pLayout);
	
	// Doesn't exist - OK
	if (i == runningFrames.end()) {
		pLayout->hWnd = hWnd;

#ifdef CONSTRUCT_DIRECTX9
		if (hWnd) {
			// Make swap chain for this frame
			D3DPRESENT_PARAMETERS present;
			memset(&present, 0, sizeof(D3DPRESENT_PARAMETERS));

			present.BackBufferFormat = D3DFMT_A8R8G8B8;
			present.SwapEffect = D3DSWAPEFFECT_DISCARD;
			present.Windowed = true;
			present.hDeviceWindow = hWnd;
			present.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// Show additional frames instantly

			VERIFY(SUCCEEDED(renderer.GetDevice()->CreateAdditionalSwapChain(&present, &(pLayout->pSwapChain))));
		}
		else
			pLayout->pSwapChain = NULL;
#endif
		runningFrames.push_back(pLayout);
		pLayout->systemDrawn = false;
		pLayout->Load();
		pLayout->CreateInitialObjects();

		CRunLayout* pOldFrame = system.pLayout;
		system.pLayout = pLayout;
		FlushDelayedObjects();
		GenerateEvent(-1, SYSTEM_STARTOFFRAME, NULL);	// Start of Frame
		system.pLayout = pOldFrame;

		return pLayout;
	}
	else return NULL;
}

void CRuntime::DestroyRunningFrame(CRunLayout* pLayout)
{
	if (pLayout == NULL) return;

	pLayout->Unload();

	list<CRunLayout*>::iterator i = find(runningFrames.begin(), runningFrames.end(), pLayout);

	if (i != runningFrames.end()) {

		if (find(FrameDeathRow.begin(), FrameDeathRow.end(), *i) == FrameDeathRow.end())
			FrameDeathRow.push_back(*i);

		runningFrames.erase(i);		
	}
}

// Animations not supported in App runtime
#ifdef APPRUNTIME

CRunAnimation* CRuntime::GetAnimationPtr(Animation anim) { return NULL; }
TickAnimRet CRuntime::TickAnimation(CRunObject* obj) { return TA_NORMAL; }
Animation CRuntime::GetAnimationByName(Animation parent, const char* name) { return -1; }
Animation CRuntime::GetAnimationByNearestAngle(Animation parent, float angle) { return -1; }
CRunAnimationFrame* CRuntime::GetCurAnimationFramePtr(AnimInfo* anim) { return NULL; }
CRunAnimation* CRuntime::SetAnimation(Animation anim, CRunObject* obj) { return NULL; }
void CRuntime::UpdateAnimationFrame(CRunObject* obj) {}
void CRuntime::DrawRunningFrame(CRunLayout *pLayout) {}
void CRuntime::UpdateKnownAnimationFrame(CRunAnimation* pAnim, CRunObject* obj) {}

#else

void CRuntime::DrawRunningFrame(CRunLayout *pLayout)
{
	if (pLayout == NULL) return;

	CRunLayout* old = system.pLayout;
	system.pLayout = pLayout;

	// Sending to alternative HWND : present to given window
	if (pLayout->hWnd) {

		// We're mid-scene here; we're going to have to come back to drawing it later.
		wndFramesToDraw.push_back(pLayout);
	}
	else
		pLayout->Draw();

	system.pLayout = old;
}

void CRuntime::UpdateKnownAnimationFrame(CRunAnimation* pAnim, CRunObject* obj)
{
	// We presume if this function is called that the pAnim is correct and the curFrame is also correct.
	// Error checking is done in functions which call this.
	CRunAnimationFrame* pFrame;
	if(obj->info.curAnim->curFrame < pAnim->frames.size())
		pFrame = &(pAnim->frames[obj->info.curAnim->curFrame]);
	else
		pFrame = &(pAnim->frames[0]);

	// edit: so that we can prevent losing the animation frame number when u change to stopped and then to walk, we
	// dont set the current frame until the animation ticks

	obj->info.curTexture = pFrame->th;
	obj->info.imgHandle = pFrame->imgHandle;
	obj->info.pInfo->collisionMaskHandle = NULL;
	
	ImageHandleInfo* ih = (ImageHandleInfo*)obj->info.imgHandle;
	if (ih != NULL)
		obj->info.pInfo->collisionMaskHandle = &ih->cm;

	float oldHotSpotX = obj->info.HotSpotX;
	float oldHotSpotY = obj->info.HotSpotY;

	obj->info.HotSpotX = (pFrame->pivotX * obj->info.w) / pFrame->width;
	obj->info.HotSpotY = (pFrame->pivotY * obj->info.h) / pFrame->height;

	// No need to do the math for the hot spot angle/dist if it hasn't changed!
	if (oldHotSpotX != obj->info.HotSpotX || oldHotSpotY != obj->info.HotSpotY) {
		obj->info.HotSpotAngle = atan2f(obj->info.HotSpotY, obj->info.HotSpotX);
		obj->info.HotSpotDist = sqrtf(obj->info.HotSpotX * obj->info.HotSpotX + obj->info.HotSpotY * obj->info.HotSpotY);
	}
}

CRunAnimation* CRuntime::GetAnimationPtr(Animation anim)
{
	if (anim < 0) return NULL;

	return animationPtrs[anim];
}

bool CRuntime::UpdateObjectSizeForFrameChange(CRunObject* obj, int oFrame, Animation oAnim, int nFrame, Animation nAnim)
{
	if(oAnim == nAnim)
		if(oFrame == nFrame)
			return false;	// no animation change

	if(oAnim == -1 || nAnim == -1 || nFrame == -1 || oFrame == -1)
		return false;

	if (obj == NULL) return false;	// bad function call

	// Get pointer to current animation & frame
	CRunAnimation* p_oAnim = animationPtrs[oAnim];
	CRunAnimation* p_nAnim = animationPtrs[nAnim];

	int size = p_oAnim->frames.size();
	if(oFrame < 0 || oFrame >= size)
		oFrame = 0;
	size = p_nAnim->frames.size();
	if(nFrame < 0 || nFrame >= size)
		nFrame = 0;

	CRunAnimationFrame* p_oFrame = &(p_oAnim->frames[oFrame]);
	CRunAnimationFrame* p_nFrame = &(p_nAnim->frames[nFrame]);

	float xratio = obj->info.w / p_oFrame->width;
	float yratio = obj->info.h / p_oFrame->height;
	obj->info.w = p_nFrame->width * xratio;
	obj->info.h = p_nFrame->height * yratio;
	/*
	obj->info.w = p_nFrame->width;
	obj->info.h = p_nFrame->height;
	*/

	// Davo's a moron for forgetting this.
	UpdateBoundingBox(obj);

	return true;
}

// Plugins call this every frame to animate their animations.
TickAnimRet CRuntime::TickAnimation(CRunObject* obj)
{
	TickAnimRet status = TA_NORMAL;

	if (obj == NULL) return TA_NORMAL;
	if (!obj->info.curAnim) return TA_NORMAL;

	AnimInfo* info = obj->info.curAnim;

	// Get pointer to current animation & frame
	CRunAnimation* pAnim = info->pAnim;
	if(!pAnim)
		return TA_NORMAL;
	if(pAnim->frames.empty())
		return TA_NORMAL;

	if(info->curFrame >= pAnim->frames.size() || info->curFrame < 0)
	{
		// This should never happen...but if it does...it stops crashes
		info->curFrame = 0;
	}
		
	CRunAnimationFrame* pFrame = &(pAnim->frames[info->curFrame]);




	// Increment counter and check if next frame needs showing
	if (info->playing) {
		info->counter += GetTimeDelta();

		// Time in seconds this frame should display for
		// Eg. anim speed 100 = 100 frames per sec = 0.01 of a second per frame
		float timeForThisFrame = abs(pFrame->timeFactor / info->speed);
		
		// Prevent hanging if close to zero
		if (timeForThisFrame <= 0.001)
			timeForThisFrame = 0.001f;

		
		while (info->counter >= timeForThisFrame && info->playing) {
			int oldframe = info->curFrame;
			info->counter -= timeForThisFrame;
			
			if(info->speed >= 0){
				if(info->pingpong == 2)
					info->curFrame --;
				else
					info->curFrame ++;
			}
			else{
				if(info->pingpong == 2)
					info->curFrame ++;
				else
					info->curFrame --;
			}

			// No more frames left in this animation?
			if (info->curFrame < info->repeat_to)
			{
				if (info->repeat!=0)
				{
					if(info->pingpong == 2)
					{
						info->curFrame = info->repeat_to;
						info->repeat--;
							// Last repeat: finish
						if (info->repeat == 0) {
							info->playing = false;
							status = TA_FINISHED;
						}
						else {
							info->curFrame = info->repeat_to;
							info->pingpong = 1;
							status = TA_LOOPED;
						}
					}
					if(info->speed < 0)
					{
						info->curFrame = info->repeat_to;
						info->repeat--;
							// Last repeat: finish
						if (info->repeat == 0) {
							info->playing = false;
							status = TA_FINISHED;
						}
						else {
							info->curFrame = info->pAnim->frames.size() - 1;
							status = TA_LOOPED;
						}
					}
				}
				else // repeat != 0
				{
					info->playing = false;
					info->curFrame = info->repeat_to;
				}
			}
			if (info->curFrame >= pAnim->frames.size()) {

				// If looping and come to the end - simply go back
				if (info->repeat!=0)
				{
					if(info->pingpong)
					{
						info->curFrame -=2;
						info->pingpong = 2;

						status = TA_PONGED;
					}
					else {
						info->repeat--;

						// Last repeat: finish
						if (info->repeat == 0) {
							info->playing = false;
							info->curFrame = pAnim->frames.size() - 1;
							status = TA_FINISHED;
						}
						else {
							info->curFrame = info->repeat_to;
							status = TA_LOOPED;
						}
					}
				}
				else // repeat != 0
				{
					info->playing = false;
					info->curFrame = pAnim->frames.size() - 1;
					//status = TA_FINISHED;
				}

				// Not looping - stop
				//if(info->repeat > 0)
				//	info->repeat--;

			}// end of anim
			if (info->curFrame < 0)
				info->curFrame = 0;

			UpdateObjectSizeForFrameChange(obj, oldframe, info->handle, info->curFrame, info->handle);

			// If still playing - update to the next frame
			if (info->playing)
				UpdateKnownAnimationFrame(pAnim, obj);

			if (status == TA_NORMAL)
				status = TA_FRAMECHANGED;

			// Update time for this frame 
			timeForThisFrame = pAnim->frames[info->curFrame].timeFactor / info->speed;

			// Prevent infinite loops
			if (timeForThisFrame < 0.001f)
				timeForThisFrame = 0.001f;
		}//while

		// Force an update
		if (info->updated) {
			info->updated = false;

			// If still playing - update to the next frame
			if (info->playing)
				UpdateKnownAnimationFrame(pAnim, obj);
		}
	}//playing

	return status;
}

Animation CRuntime::GetAnimationByName(Animation parent, const char* name)
{


	if (parent < 0 || name == NULL)
		return -1;

	// Get ptr
	CRunAnimation* pAnim = animationPtrs[parent];

	if (pAnim == NULL) return -1;

	// Search and try to match for a name
	CString animName = name;
	animName.MakeLower();

	// If has no parent
	if (pAnim->name == animName) return pAnim->handle;

	Animations::iterator i = pAnim->subAnims.begin();
	Animations::const_iterator subAnims_end = pAnim->subAnims.end();

	for ( ; i != subAnims_end; i++)
		if (i->name == animName)
			return i->handle;
		

	// Not found
	return -1;
}

Animation CRuntime::GetAnimationByNearestAngle(Animation parent, float angle)
{

	if (parent < 0)
		return -1;

	while(angle > 360)
		angle -= 360;
	while(angle < 0)
		angle += 360;

	// Get ptr
	CRunAnimation* pAnim = animationPtrs[parent];

	if(pAnim->Angles.size() == 0)
		pAnim = pAnim->parent;
	float closest = 360.0f;

	Animations::iterator i = pAnim->Angles.begin();
	Animations::const_iterator subAnims_end = pAnim->Angles.end();

	// None by default
	int iHandle = -1;

	
	for ( ; i != subAnims_end; i++)
	{
		
		if (abs(angle - i->angle) < closest || 360-abs(angle - i->angle) < closest )
		{
			closest = min( abs(angle - i->angle), 360-abs(angle - i->angle));
			iHandle = i->handle;
		}

	}
	return iHandle;
}

CRunAnimationFrame* CRuntime::GetCurAnimationFramePtr(AnimInfo* info)
{
	if (info == 0) return NULL;

	CRunAnimation* pAnim = animationPtrs[info->handle];

	if (info->curFrame >= 0 && info->curFrame < pAnim->frames.size())
		return &(pAnim->frames[info->curFrame]);
	else
		return NULL;
}

CRunAnimation* CRuntime::SetAnimation(Animation anim, CRunObject* obj)
{
	if (anim==-1 || !obj) 
		return NULL;

	AnimInfo* info = obj->info.curAnim;
	
	int oldframe = info->curFrame;
	Animation oldAnim = obj->info.curAnim->handle;	// Set animation and set first frame

	CRunAnimation* pAnim = animationPtrs[anim];
	info->handle = anim;
	info->repeat = pAnim->repeat;
	info->repeat_to = pAnim->repeat_to;
	info->pingpong = pAnim->pingpong;
	info->speed = pAnim->speed;
	info->counter = 0;
	if(info->curFrame == -1)
		info->curFrame = 0;
	info->pAnim = pAnim;
	info->playing = true;
	if(oldAnim != anim)
	{
		UpdateObjectSizeForFrameChange(obj, oldframe, oldAnim, info->curFrame, info->handle);

		// Only attempt to set the frame if it is a valid curFrame number
		//if (info->curFrame >= 0 && info->curFrame < pAnim->frames.size())
			UpdateKnownAnimationFrame(pAnim, obj);
	}
	
	return pAnim;
}

void CRuntime::UpdateAnimationFrame(CRunObject* obj)
{
	if (obj == NULL) return;
	if (obj->info.curAnim < 0) return;

	AnimInfo* info = obj->info.curAnim;
	CRunAnimation* pAnim = animationPtrs[info->handle];

	// Only attempt to set the frame if it is a valid curFrame number
	if (info->curFrame >= 0 && info->curFrame < pAnim->frames.size())
		UpdateKnownAnimationFrame(pAnim, obj);
}

#endif // else / APPRUNTIME

void CRuntime::Restart()
{
	//
}

void CRuntime::Break(bool bResume)
{
	//
}

void CRuntime::Step()
{
	//
}

CRunLayout* CRuntime::GetSystemFrame() const
{
	list<CRunLayout*>::const_iterator f = runningFrames.begin();
	for ( ; f != runningFrames.end(); f++) {
		if ((*f)->systemDrawn)
			return *f;
	}
 	
	// None found - use latest frame (hacky)
	return runningFrames.back();
}

void CRuntime::Save(bin& ar)
{
	//bin ar(g_allocator);
	ar.loading = false;
	int i;

	//ar.allocate(100000);

	// "Construct Runtime 1.0" version
	ar << CString("CRT1.0");
	ar << props.appTitle << objects.size();
	ar << winWidth << winHeight << frameCount << userFps << totalObjects << bkColor << curUid << GetAppClock();
	ar << timeScale;

	// Global vars
	ar << globalVars.size();
	for (i = 0; i < globalVars.size(); i++)
		ar << globalVars[i];

	/*
	// Every event states
	// Problem - pointers work OK for quicksave/quickload, but not for separate executions of the runtime
	ar << system.everyEvents.size();
	map<void*, clock_t>::iterator e = system.everyEvents.begin();

	for ( ; e != system.everyEvents.end(); ++e)
		ar << (int)e->first << e->second;
		*/

	// Get system frame to save
	CRunLayout* psf = GetSystemFrame();
	
	ar << psf->number;
	ar << psf->frameCounter << psf->width << psf->height << psf->scrollX << psf->scrollY;
	ar << psf->unboundedScrolling << psf->bg_color << psf->zoomX << psf->zoomY << psf->frameCounter64;
	ar << psf->modal << psf->curXscale << psf->curYscale << psf->curAngle << GetLayoutClock(psf);

	for (i = 0; i < psf->layers.size(); i++) {
		CRunLayer* layer = psf->layers[i];
		ar << layer->angle << layer->backColor << layer->opacity << layer->colourfilter << layer->scrollXf << layer->scrollYf << layer->zoomXf << layer->zoomYf;
		ar << layer->visible;
		
		// Save layer Z order UIDs
		ar << layer->instances.size();

		list<CRunObject*>::iterator r = layer->instances.begin();
		
		for ( ; r != layer->instances.end(); r++)
			ar << (*r)->info.uid;
	}

	// For each type
	ObjTypeIterator t = objects.begin();
	ObjTypeConstIterator objects_end = objects.end();

	for ( ; t != objects_end; t++) {

		// Only real types are saved
		if ((*t)->movement || (*t)->teamID > -1 || (*t)->noSerialize)
			continue;

		// Number of instances
		ar << (*t)->instances.size();

		// Export each instance
		ObjIterator j = (*t)->instances.begin();
		ObjConstIterator instances_end = (*t)->instances.end();

		for ( ; j != instances_end; j++) {

			ar << (*j)->info.layer;
			ar << (*j)->info.uid;
			ar << (*j)->info.global;
			(*j)->Serialize(ar);

			// Look for this object's movements and serialize them
			ObjIterator s = (*j)->siblings.begin();
			ObjConstIterator siblings_end = (*j)->siblings.end();

			for (s = (*j)->siblings.begin(); s != siblings_end; s++) {

				if ((*s)->pType->movement && (*s)->pLink == *j) {
					(*s)->Serialize(ar);
				}

			}// for each sibling
		}// for each instance
	}

	//ar.save(fname);
}

CRunObject* CRuntime::GetObjectFromUID(int uid)
{
	ObjTypeIterator t = objects.begin();
	ObjTypeIterator end = objects.end();

	for ( ; t != end; t++) {
		ObjIterator i = (*t)->instances.begin();
		ObjIterator instances_end = (*t)->instances.end();

		for ( ; i != instances_end; i++) {

			if ((*i)->info.uid == uid)
				return *i;
		}
	}

	throw runtime_error("Invalid UID referenced");
}

void CRuntime::Load(bin& ar)
{
	//bin ar(g_allocator);
	//ar.load(fname);
	ar.loading = true;

	int count, i;

	if (!ar.valid())
		return;

	// Too small for 2 strings and object count
	if (ar.size() < 12)
		return;

	CString s;
	int x;

	ar >> s;
	if (s != "CRT1.0") return;			// Not a Construct Runtime file
	ar >> s;
	if (s != props.appTitle) return;	// Not a file saved by this application (basic check but will do for now)
	ar >> x;
	if (x != objects.size()) return;	// Same name but different object type count: also invalid

	// Destroy all objects ready to recreate
	ObjTypeIterator t = objects.begin();
	ObjTypeConstIterator objects_end = objects.end();

	for ( ; t != objects_end; t++) {

		// Virtual types do not need freeing, they don't 'own' their instances
		if ((*t)->teamID > -1)
			continue;

		// Don't do this for non serializers
		if ((*t)->noSerialize)
			continue;

		(*t)->DeleteInstances();
	}

	// Reset registered windows
	pluginWnds.clear();

	clock_t desiredAppClock;
	ar >> winWidth >> winHeight >> frameCount >> userFps >> totalObjects >> bkColor >> curUid >> desiredAppClock;
	clockOffset = GetAppClock() - desiredAppClock;

	ar >> timeScale;

	// Global vars
	ar >> count;
	for (i = 0; i < count; i++)
		ar >> globalVars[i];

	// Every event states
	system.everyEvents.clear();
	/*
	ar >> count;

	for (i = 0; i < count; i++) {
		int first;
		clock_t second;
		ar >> first >> second;
		system.everyEvents[(void*)first] = second;
	}
	*/

	// Load system frame
	CRunLayout* psf = GetSystemFrame();

	clock_t desiredLayoutClock;

	int layoutNum;
	ar >> layoutNum;

	bool changedLayout = false;

	// Check if we need to change the layout
	if (psf->number != layoutNum) {
		psf->Unload();
		runningFrames.resize(0);
		runningFrames.push_back(Frames[layoutNum]);
		psf = runningFrames.front();
		psf->systemDrawn = true;
		psf->Load();
		system.pLayout = psf;
		changedLayout = true;
	}

	ar >> psf->frameCounter >> psf->width >> psf->height >> psf->scrollX >> psf->scrollY;
	ar >> psf->unboundedScrolling >> psf->bg_color >> psf->zoomX >> psf->zoomY >> psf->frameCounter64;
	ar >> psf->modal >> psf->curXscale >> psf->curYscale >> psf->curAngle >> desiredLayoutClock;

	psf->startClock = GetAppClock() - desiredLayoutClock;

	vector<vector<int>> layerZOrders;

	for (i = 0; i < psf->layers.size(); i++) {
		CRunLayer* layer = psf->layers[i];
		ar >> layer->angle >> layer->backColor >> layer->opacity >> layer->colourfilter >> layer->scrollXf >> layer->scrollYf >> layer->zoomXf >> layer->zoomYf;
		ar >> layer->visible;

		vector<int> zOrder;
		int zOrderCount;

		ar >> zOrderCount; // number of instances on layer

		for (int j = 0; j < zOrderCount; j++) {
			int uid;
			ar >> uid;
			zOrder.push_back(uid);
		}

		layerZOrders.push_back(zOrder);

		layer->UpdateFilter();
	}

	ObjList created;

	// Load instances
	t = objects.begin();

	for ( ; t != objects_end; t++) {

		// Only real types are loaded
		if ((*t)->movement || (*t)->teamID > -1 || (*t)->noSerialize)
			continue;

		ar >> count;

		// Create and serialize this many objects
		for (i = 0; i < count; i++) {
			int layer, uid;
			ar >> layer >> uid;

			bool global;
			ar >> global;
			
			CRunObject* obj = CreateRunObject((*t)->createParams, layer, system.pLayout, false);
			obj->OnCreate();
			obj->Serialize(ar);
			obj->info.uid = uid;
			obj->info.global = global;
			UpdateBoundingBox(obj);
			created.push_back(obj);

			// Look for movements that this object has and load them
			ObjTypeIterator s = (*t)->sisterTypes.begin();
			ObjTypeIterator sisterTypes_end = (*t)->sisterTypes.end();

			for ( ; s != sisterTypes_end; s++) {
				if ((*s)->movement && (*s)->pLinkType == *t) {
					CRunObject* mObj = CreateRunObject((*s)->createParams, obj->info.layer, system.pLayout, false, obj);
					mObj->OnCreate();
					mObj->Serialize(ar);
					created.push_back(mObj);
				}
			}

			/*
			ObjIterator s = obj->siblings.begin();
			ObjConstIterator siblings_end = obj->siblings.end();
			
			for ( ; s != siblings_end; s++) {

				if ((*s)->pType->movement && (*s)->pLink == obj)
	
					(*s)->Serialize(ar);
			}
			*/
		}
	}

	LinkContainersTogether(created);

	// Check for delayed serialized objects
	vector<DelayedSerializeObjectPtr>::iterator ds = delayedSerializeObjects.begin();

	for ( ; ds != delayedSerializeObjects.end(); ds++) {

		*(ds->obj) = GetObjectFromUID(ds->uid);

	}

	delayedSerializeObjects.resize(0);

	// Now set the layer Z orders
	vector<vector<int>>::iterator y = layerZOrders.begin();

	for ( ; y != layerZOrders.end(); y++) {
		
		CRunLayer* pLayer = psf->layers[y - layerZOrders.begin()];
		pLayer->instances.resize(0);

		vector<int>::iterator i = y->begin();
		
		for ( ; i != y->end(); i++) {
			CRunObject* o = GetObjectFromUID(*i);
			o->info.z = pLayer->instances.size();
			pLayer->instances.push_back(o);

			// Make iterator reference last item as well
			LayerObjIterator& rIter = *(o->info.pLayerIterator);
			rIter = pLayer->instances.end();
			rIter--;
		}
	}

	// If we switched layout trigger Start of Layout
	if (changedLayout)
		GenerateEvent(-1, SYSTEM_STARTOFFRAME, NULL);
}

CRunAnimation* CRuntime::GetSubAnimation(CRunAnimation* pAnim, int index)
{
	if(!pAnim)
		return NULL;
	if(pAnim->subAnims.size() <= index)
		return NULL;
	Animations::iterator i = pAnim->subAnims.begin();
	advance(i, index);
	return &*i;
}
int	CRuntime::GetSubAnimationCount(CRunAnimation* pAnim)
{
	if(!pAnim)
		return 0;
	return pAnim->subAnims.size();
}

CRunAnimation*	CRuntime::GetSubAngleAnimation(CRunAnimation* pAnim, int index)
{
	if(!pAnim)
		return NULL;
	if(pAnim->Angles.size() <= index)
		return NULL;
	Animations::iterator i = pAnim->Angles.begin();
	advance(i, index);
	return &*i;
}

int	CRuntime::GetSubAngleAnimationCount(CRunAnimation* pAnim)
{
	if(!pAnim)
		return 0;
	return pAnim->Angles.size();
}

clock_t CRuntime::GetLayoutClock(CRunLayout* pLayout)
{
	if (pLayout == NULL) return -1;
	return pLayout->currentClock;
}

#ifndef APPRUNTIME
CRunAnimation* CRuntime::GetPresetAnimation(CRunAnimation* pRoot, int tagID)
{
	if (pRoot->tagID == tagID)
		return pRoot;

	AnimationIterator i = pRoot->subAnims.begin();
	AnimationIterator end = pRoot->subAnims.end();

	// Recurse through subanimations until the right one is found
	for ( ; i != end; i++) {

		CRunAnimation* ret = GetPresetAnimation(&(*i), tagID);

		if (ret) return ret;
	}

	// Not found; return NULL
	return NULL;
}
#endif

#ifdef APPRUNTIME

CollisionMask* CRuntime::CreateCollisionMask(int width, int height)
{		return NULL;		}

void CRuntime::ReleaseCollisionMask(CollisionMask* mask)
{							}

CollisionMask* CRuntime::GetCollisionMaskFromImage(int image)
{		return NULL;		}

void CRuntime::AddMaskToMask(CollisionMask *src, CollisionMask *dest, int x, int y)
{							}

void CRuntime::ExportCollisionToFile(CollisionMask *mask, int index)
{							}

void CRuntime::DrawObject(CRunObject* obj)
{							}

#else
CollisionMask* CRuntime::CreateCollisionMask(int width, int height)
{
	collisionmasks.push_back(CollisionMask());
	CollisionMask& cm = collisionmasks.back();

		// Determine size of our buffer.  All buffers are rounded up to 128 bit pitch, just in case SSE can be used.
	const int align_pitch_bits = 64;
	const int align_pitch_bytes = align_pitch_bits / 8;

	int pitch = width / 8;
	if (width % 8 != 0)
		pitch++;		// 11 pixel width needs 2 bytes not 1 rounded

	// Eg. a 20 byte pitch must round up to 32 (+12, 16 - 4)
	if (pitch % align_pitch_bytes != 0)
		pitch += align_pitch_bytes - (pitch % align_pitch_bytes);

	// If the pitch does not leave at least a 64 pixel gutter, increase it by 64 pixels.
	// This prevents false positives when a 64 pixel check from the far right edge can wrap around to the next line.
	if ((pitch * 8) - width < align_pitch_bits)
		pitch += align_pitch_bytes;

	//pitch += LEFTPADDING;

	cm.pitch = pitch;
	cm.width = width;
	cm.height = height;
	
	for(int i = 0; i < 8; i++)
	{
		cm.mem_size = cm.height * cm.pitch + 16;
		cm.bits[i] = (BYTE*)_aligned_malloc(cm.mem_size, 16);
		memset(cm.bits[i], 0, cm.mem_size);
	}

	return &cm;
}

void CRuntime::ReleaseCollisionMask(CollisionMask* mask)
{
	for(list<CollisionMask>::iterator i = collisionmasks.begin(); i != collisionmasks.end(); i++)
	{
		if(&*i == mask)
		{
			collisionmasks.erase(i);
			break;
		}
	}
	FreeMask(mask);
}

CollisionMask* CRuntime::GetCollisionMaskFromImage(int image)
{
	return &(((ImageHandleInfo*)image)->cm);
}

void CRuntime::DrawObject(CRunObject* obj)
{
	DisplayTranslateMode old = translationMode;
	translationMode = TRANSLATE_SCREEN;
	obj->pLayout->DrawRunObjectOnScreen(obj, obj->pLayout->GetObjectBox(obj));

	translationMode = old;
}
#endif

#ifdef CONSTRUCT_DIRECTX9
void CRuntime::OnLostDevice()
{
	// Prepare fonts
	/*
	map<int, CD3DDisplayFont>::iterator ft = Fonts.begin();

	for ( ; ft != Fonts.end(); ft++) {
		if (ft->second.d3dxfont)
			ft->second.d3dxfont->OnLostDevice();
	}
	*/

	// Prepare effects
	vector<CRunEffect*>::iterator ef = effects.begin();

	for ( ; ef != effects.end(); ef++) {
		if ((*ef)->pEffect)
			(*ef)->pEffect->OnLostDevice();
	}

	// Reallocate the rendertargets - they're about to change size
	SafeFreeTexture(tempLayerTarget);
	SafeFreeTexture(tempDisplayTarget);
	SafeFreeTexture(tempSourceTarget);
	SafeFreeTexture(tempRenderTarget[0]);
	SafeFreeTexture(tempRenderTarget[1]);
	SafeFreeTexture(tempTransitionTarget);
	SafeFreeTexture(blurFloatAccumulate);
	blurFloatAccumulate = unallocated_texture;

	if (multisampleTargets[0] != unallocated_texture)
		renderer.DeallocateTexture(multisampleTargets[0]);
	if (multisampleTargets[1] != unallocated_texture)
		renderer.DeallocateTexture(multisampleTargets[1]);

	//vector<CRunLayout*>::iterator f = Frames.begin();

	//ReleaseBackBuffers();
	ReleaseSwapChains();

	//renderer.OnLostDevice();
	renderer.SetDeviceLost(true);
}

// List all the swap chains in the 'swapChains' vector
void CRuntime::GetSwapChains()
{
	swapChains.resize(0);
	
	for (int i = 0; i < renderer.GetMultiHeadCount(); i++) {
		IDirect3DSwapChain9* pSwap;
		renderer.GetDevice()->GetSwapChain(i, &pSwap);
		swapChains.push_back(pSwap);
	}
}

// Get the backbuffer for each swapchain
void CRuntime::GetBackBuffers()
{
	backBuffers.resize(0);

	SwapChainIterator s = swapChains.begin();

	for ( ; s != swapChains.end(); s++) {
		IDirect3DSurface9* pBackBuffer;
		(*s)->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
		backBuffers.push_back(pBackBuffer);
	}
}

void CRuntime::ReleaseBackBuffers()
{
	BackBufferIterator b = backBuffers.begin();

	for ( ; b != backBuffers.end(); b++)
		(*b)->Release();

	backBuffers.resize(0);
}

void CRuntime::ReleaseSwapChains()
{
	SwapChainIterator s = swapChains.begin();

	for ( ; s != swapChains.end(); s++)
		(*s)->Release();

	swapChains.resize(0);
}

void CRuntime::OnResetDevice()
{
	renderer.SetDeviceLost(false);

	GetSwapChains();
	//GetBackBuffers();

	if (props.sampler == 0) {
		renderer.SetSamplerState(cr::ss_magfilter, cr::ssv_point);
		renderer.SetSamplerState(cr::ss_minfilter, cr::ssv_point);
	}
	if (props.sampler == 1) {
		renderer.SetSamplerState(cr::ss_magfilter, cr::ssv_linear);
		renderer.SetSamplerState(cr::ss_minfilter, cr::ssv_linear);
	}

	// Re-add any rendertarget textures
	ReallocDisplayTargetTexture(tempLayerTarget);
	ReallocDisplayTargetTexture(tempDisplayTarget);
	ReallocDisplayTargetTexture(tempSourceTarget);
	ReallocDisplayTargetTexture(tempRenderTarget[0]);
	ReallocDisplayTargetTexture(tempRenderTarget[1]);
	ReallocDisplayTargetTexture(tempTransitionTarget);

	if (multisampleTargets[0] != unallocated_texture)
		multisampleTargets[0] = renderer.CreateRenderTargetTexture(winWidth, winHeight, cr::texture_format_a8r8g8b8, true);

	if (multisampleTargets[1] != unallocated_texture)
		multisampleTargets[1] = renderer.CreateRenderTargetTexture(winWidth, winHeight, cr::texture_format_a8r8g8b8, true);

	// Restore fonts
	/*
	map<int, CD3DDisplayFont>::iterator ft = Fonts.begin();

	for ( ; ft != Fonts.end(); ft++) {
		if (ft->second.d3dxfont)
			ft->second.d3dxfont->OnResetDevice();
	}
	*/

	// Restore effects
	vector<CRunEffect*>::iterator ef = effects.begin();

	for ( ; ef != effects.end(); ef++) {
		if ((*ef)->pEffect)
			(*ef)->pEffect->OnResetDevice();
	}

	//renderer.OnResetDevice();

	if (motionBlur)
		InitMotionBlur();
}

#endif

void CRuntime::UpdateBoundingBox(CRunObject* obj)
{
	if (obj == NULL)
		return;

	OBJHEADER& info = obj->info;

#ifndef APPRUNTIME
	// This allows sprites and stuff to change their angle to the animation angle.
	if(obj->info.last_angle != obj->info.angle)
	{
		if(obj->info.UpdateAngle)
			obj->info.UpdateAngle(obj);
		else
			obj->info.displayangle = obj->info.angle;	
		obj->info.last_angle = obj->info.angle;
	}
#endif

	if (info.curAnim && info.curAnim->pAnim)
		UpdateKnownAnimationFrame(info.curAnim->pAnim, obj);	

	info.box.left = info.x - info.HotSpotX;
	info.box.top = info.y - info.HotSpotY;
	info.box.right = info.box.left + info.w;
	info.box.bottom = info.box.top + info.h;

	if (info.w < 0.0f)
		swap(info.box.left, info.box.right);
	if (info.h < 0.0f)
		swap(info.box.top, info.box.bottom);

#ifndef APPRUNTIME
	// This allows sprites and stuff to change their angle to the animation angle.
	if (info.displayangle != 0.0f) {

		RECTF& box = info.box;

		// Angle in radians
		float radians = cr::to_radians(info.displayangle);
		float sin_a, cos_a;
		cr::sincosf(radians, &sin_a, &cos_a);


		bool sin_positive = (sin_a >= 0.0f);
		bool cos_positive = (cos_a >= 0.0f);

		float left_x = box.left - info.x;
		float right_x = box.right - info.x;
		float top_y = box.top - info.y;
		float bottom_y = box.bottom - info.y;

		float left_x_cos = left_x * cos_a;
		float left_x_sin = left_x * sin_a;
		float right_x_cos = right_x * cos_a;
		float right_x_sin = right_x * sin_a;
		float top_y_cos = top_y * cos_a;
		float top_y_sin = top_y * sin_a;
		float bottom_y_cos = bottom_y * cos_a;
		float bottom_y_sin = bottom_y * sin_a;

		float left, right, top, bottom;

		if (sin_positive) {
			// First quadrant 0-90 degrees: point 3 (bottom left) will be leftmost
			if (cos_positive) {
				left   = info.x + left_x_cos - bottom_y_sin;
				right  = info.x + right_x_cos - top_y_sin;

				top    = info.y + top_y_cos + left_x_sin;
				bottom = info.y + bottom_y_cos + right_x_sin;
			}
			// cos is negative: second quadrant 90-180 degrees, point 4 (bottom right) will be leftmost
			else {
				left   = info.x + right_x_cos - bottom_y_sin;
				right  = info.x + left_x_cos - top_y_sin;

				top    = info.y + bottom_y_cos + left_x_sin;
				bottom = info.y + top_y_cos + right_x_sin;
			}

		}
		// sin is negative
		else {
			// cos is positive: fourth quadrant 270-360 degrees, point 1 (top left) will be leftmost
			if (cos_positive) {
				left   = info.x + left_x_cos - top_y_sin;
				right  = info.x + right_x_cos - bottom_y_sin;

				top    = info.y + top_y_cos + right_x_sin;
				bottom = info.y + bottom_y_cos + left_x_sin;

			}
			// cos is negative: third quadrant 180-270 degrees, point 2 (top right) will be leftmost
			else {
				left   = info.x + right_x_cos - top_y_sin;
				right  = info.x + left_x_cos - bottom_y_sin;

				top    = info.y + bottom_y_cos + right_x_sin;
				bottom = info.y + top_y_cos + left_x_sin;
			}

		}

		box.left = left;
		box.right = right;
		box.top = top;
		box.bottom = bottom;
	}
#endif

	const float& skewX = info.pInfo->skewX;
	const float& skewY = info.pInfo->skewY;

	if (skewX != 0.0f || skewY != 0.0f) {
		float skewAbs = max(fabs(skewX), fabs(skewY));

		// Enlarge entire box by this amount (ignoring angle, enough room to fully rotate)
		info.box.right += skewAbs;
		info.box.left -= skewAbs;
		info.box.top -= skewAbs;
		info.box.bottom += skewAbs;
	}

	/*
	if(info.box.left > info.box.right)
		swap(info.box.left, info.box.right);
	if(info.box.top > info.box.bottom)
		swap(info.box.top, info.box.bottom);
	*/
}

void CRuntime::ProcessControlState()
{
	ControlsMap::iterator k = keyControls.begin();
	ControlsMap::const_iterator k_end = keyControls.end();
	for(; k!=k_end; k++)
	{
		if(AsyncKeyDown(k->second.vk))
			k->second.state = 1;
		else
			k->second.state = 0;
	}
}