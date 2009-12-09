// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

// ExtObject constructor:
// Called when Construct creates an instance of your object.  Use your editTime data
// to initialise the object.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
{
	pRuntime = pVRuntime;
	info.editObject = editObject;
}

// This is called just after the constructor, when Construct has added details to your
// object such as siblings and type pointer.  If you need this data on startup, it is
// valid to read here.
void ExtObject::OnCreate()
{
	// Load settings
	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	int Version = 0;
	ar >> Version;

	ar >> maxspeed >> acc >> dec >> rotateMode >> directionsMode;

	if (Version >= 2)
		ar >> player;
	else
		player = 0;

	ar.detach();

	speed = dx = dy = 0.0f;
	ignoringInput = false;
	activated = true;

	// Get controls
	//vk_left = pRuntime->GetControlVK("Move Left", player);
	//vk_right = pRuntime->GetControlVK("Move Right", player);
	//vk_up = pRuntime->GetControlVK("Move Up", player);
	//vk_down = pRuntime->GetControlVK("Move Down", player);

	// Check for defaults
	//if (vk_left == -1) vk_left = VK_LEFT;
	//if (vk_right == -1) vk_right = VK_RIGHT;
	//if (vk_up == -1) vk_up = VK_UP;
	//if (vk_down == -1) vk_down = VK_DOWN;

	// Get solid trait
	pSolid = pRuntime->GetTraitFromName("Solid");
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
	//
}

// Trigger 'on collision' for objects I hit and are kept out of
bool ExtObject::IsOverlappingAndRegister(CRunObjType* pSolid)
{
	int count;
	CRunObject** instances;

	pRuntime->GetTypeInstances(pSolid, instances, count);

	if (count == 0) return false;

	CRunObject** i = instances;
	CRunObject** end = instances + count;

	bool ret = false;

	for ( ; i != end; ++i) {
		if (pRuntime->QueryCollision(pLink, *i)) {
			ret = true;
			pRuntime->RegisterCollision(pLink, *i);
		}
	}

	return ret;
}

// Called every frame for you to update your object if necessary
// Return values: 1 (do not call RTOnFrame again), 0 (continue calling)

// Change actual co-ordinates and cause rotation in OnFrame since these must both be
// overridable in the following events
BOOL ExtObject::OnFrame()
{
	float left = 0;
	float right = 0;
	float up = 0;
	float down = 0;

	if (!ignoringInput) {

		if (directionsMode != DIRMODE_UPDOWN) {
			left = pRuntime->GetControlState("Move Left", player);
			right = pRuntime->GetControlState("Move Right", player);
		}
		if (directionsMode != DIRMODE_LEFTRIGHT) {
			up = pRuntime->GetControlState("Move Up", player);
			down = pRuntime->GetControlState("Move Down", player);
		}

		if (directionsMode == DIRMODE_4) {
			if (up>0.5 && left>0.5)
				left = 0;
			if (up>0.5 && right>0.5)
				right = 0;
			if (down>0.5 && left>0.5)
				left = 0;
			if (down>0.5 && right>0.5)
				right = 0;
		}
	}

	// Prevent moonwalking
	if (up>0.5 && down>0.5)
		up = down = 0;
	if (left>0.5 && right>0.5)
		left = right = 0;

	if (GetFocus() != pRuntime->GetFrameHwnd(pLink->pLayout)) {
		up = left = down = right = 0;
	}

	float timedelta = pRuntime->GetTimeDelta();
	
	// Decelerate components (before X/Y changed to prevent sticking)
	if (!(left>0.5) && !(right>0.5)) {
		if (dec < 0)	// Negative deceleration is instant
			dx = 0;
		else {
			if (dx < 0) {
				dx += dec * timedelta;
				if (dx > 0) dx = 0;
			}
			else if (dx > 0) {
				dx -= dec * timedelta;
				if (dx < 0) dx = 0;
			}
		}
	}

	if (!(up>0.5) && !(down>0.5)) {
		if (dec < 0)	// Negative deceleration is instant
			dy = 0;
		else {
			if (dy < 0) {
				dy += dec * timedelta;
				if (dy > 0) dy = 0;
			}
			else if (dy > 0) {
				dy -= dec * timedelta;
				if (dy < 0) dy = 0;
			}
		}
	}

	bool xMotion = true;
	bool yMotion = true;

	// Modify the object position
	if (IsOverlappingAndRegister(pSolid)) {
		// Try moving out in the positive and negative x and y directions.
		// Stop at whichever is not colliding first.
		float offset = 1.0f;
		int dir = 0;
		float origX = pLink->info.x;
		float origY = pLink->info.y;
		//xMotion = yMotion = false;

		do {
			switch (dir) {
			case 0:
				pLink->info.x = origX - offset;
				pLink->info.y = origY;
				break;
			case 1:
				pLink->info.x = origX + offset;
				pLink->info.y = origY;
				break;
			case 2:
				pLink->info.x = origX;
				pLink->info.y = origY - offset;
				break;
			case 3:
				pLink->info.x = origX;
				pLink->info.y = origY + offset;
				break;
			}

			dir++;
			if (dir >= 4) {
				dir = 0;
				offset++;
			}

			pRuntime->UpdateBoundingBox(pLink);
		}
		while (pRuntime->QueryCollision(pLink, pSolid));
	}
	else {
		float oldX = pLink->info.x;
		float oldY = pLink->info.y;

		// Try moving across the X axis
		pLink->info.x += dx * timedelta;
		pRuntime->UpdateBoundingBox(pLink);

		// Check for solids
		if (IsOverlappingAndRegister(pSolid)) {
			pLink->info.x = oldX;
			pRuntime->UpdateBoundingBox(pLink);
			dx = 0.0f;
			xMotion = false;
		}

		// Try moving across the Y axis
		pLink->info.y += dy * timedelta;
		pRuntime->UpdateBoundingBox(pLink);

		// Check for solids
		if (IsOverlappingAndRegister(pSolid)) {
			pLink->info.y = oldY;
			pRuntime->UpdateBoundingBox(pLink);
			dy = 0.0f;
			yMotion = false;
		}
	}

	if (dx != 0 || dy != 0) {

		// Rotation mode 0: does not alter angle
		float oldAngle = pLink->info.angle;

		// Rotation mode 1: 8-direction angle
		if (rotateMode == 1) {
			
			// Right
			if (right) {
				pLink->info.angle = 0.0f;

				// Right + up
				if (up>0.5)
					pLink->info.angle = -45.0f;
				// Right + down
				if (down>0.5)
					pLink->info.angle = 45.0f;
			}
			// Left
			else if (left) {
				pLink->info.angle = 180.0f;

				// Left + up
				if (up>0.5)
					pLink->info.angle = -135.0f;
				// Left + down
				if (down>0.5)
					pLink->info.angle = 135.0f;
			}
			// Neither right or left are down.  Handle up & down
			else if (up>0.5)
				pLink->info.angle = -90.0f;
			else if (down>0.5)
				pLink->info.angle = 90.0f;
		}
		// 360 degree rotation mode
		else if (rotateMode == 2) {
			pLink->info.angle = DEGREES(atan2(dy,dx));
		}

		pRuntime->UpdateBoundingBox(pLink);

		// Check for collision with solid
		if (IsOverlappingAndRegister(pSolid)) {
			pLink->info.angle = oldAngle;
			pRuntime->UpdateBoundingBox(pLink);
		}
	}

	// Update the object box
	pRuntime->UpdateBoundingBox(pLink);

	// Apply acceleration to components
	if (xMotion) {
		if (left>0.5) {
			if (acc < 0)
				dx = -maxspeed;
			else
				dx -= acc * timedelta;
		}
		if (right>0.5) {
			if (acc < 0)
				dx = maxspeed;
			else
				dx += acc * timedelta;
		}
	}
	if (yMotion) {
		if (up) {
			if (acc < 0)
				dy = -maxspeed;
			else
				dy -= acc * timedelta;
		}
		if (down) {
			if (acc < 0)
				dy = maxspeed;
			else
				dy += acc * timedelta;
		}
	}

	// Now apply the max speed cap
	speed = GetSpeed();

	if (speed > maxspeed)
		SetSpeed(maxspeed);

	return !activated;
}

BOOL ExtObject::OnFrame2()
{
	return 1;
}

// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your extension doesn't draw anything.
void ExtObject::Draw()
{
	//
}

// WindowProc:  called when a window message, or WM_COMMAND specifying your window,
// is sent to Construct.  You must call RegisterWindow() before Construct will send
// you messages for the given window.
LRESULT ExtObject::WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	return -2;
}

BOOL ExtObject::PreTranslateMessage(MSG* msg)
{
	return 0;
}

// For global objects
void ExtObject::OnFrameChange(int oldFrame, int newFrame)
{
	// Do anything your global object needs when the frame changes
	// oldFrame is -1 on start of app, newFrame is -1 on end of app
}

// Debugging
void ExtObject::DebuggerUpdateDisplay(ExpStore*& pPrivateVars)
{
	pRuntime->AddDebuggerItem("Speed", speed, false);
	pRuntime->AddDebuggerItem("Max Speed", maxspeed, false);
	pRuntime->AddDebuggerItem("Acceleration", acc, false);
	pRuntime->AddDebuggerItem("Deceleration", dec, false); 
	pRuntime->AddDebuggerItem("Angle moving", DEGREES(atan2(dy,dx)), true);
}

void ExtObject::OnDebuggerValueChanged(const char* name, const char* value)
{
	// Convenience values
	CString label = name;
	CString text = value;
	double fValue = atof(text);
	__int64 iValue = _atoi64(text);

	if (label == "Speed") {
		Clamp(fValue, 0.0, (double)maxspeed);
		SetSpeed(fValue);
	}
	else if (label == "Max Speed") {
		Clamp(fValue, 0.0, 1000.0);
		maxspeed = fValue;
	}
	else if (label == "Acceleration") {
		Clamp(fValue, 0.0, 1000.0);
		acc = fValue;
	}
	else if (label == "Deceleration") {
		Clamp(fValue, 0.0, 1000.0);
		dec = fValue;
	}
}

void ExtObject::Serialize(bin& ar)
{
	if (ar.loading) {
		ar >> dx >> dy >> speed >> acc >> dec >> maxspeed >> rotateMode >> directionsMode
			>> ignoringInput >> activated;

		if (activated)
			pRuntime->CallOnFrame(this);
	}
	else {
		ar << dx << dy << speed << acc << dec << maxspeed << rotateMode << directionsMode
			<< ignoringInput << activated;
	}
}

long ExtObject::GetData(int id, void* param)
{
	switch (id) {
	// If your extension uses a window, you can return it here
	case GET_WINDOW:
		break;
	}

	return 0;
}

long ExtObject::CallFunction(int id, void* param)
{
	switch (id) {

		// Write speed to param
	case CF_GETSPEED:
		*(float*)param = speed;
		break;
	}

	return 0;
}

// Return the Default Value.  This is the value to be returned if your object name
// is used without an expression name, eg. in "Counter + 1".
// Parameters can be passed, eg. MyArray(3,7).
long ExtObject::ReturnDefaultValue(LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

// Called for undefined expression names, if your extension allows it.
// Eg: MyObject.UndefinedExpression(3,4,5) calls this routine with
// "UndefinedExpression" as the expression name.
long ExtObject::ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

#else //ifdef RUN_ONLY

CRunObject* WINAPI RTCreateObject(bin& ar, VRuntime* pRuntime) 
{
	return NULL;
}

void WINAPI RTDestroyObject(ExtObject* object)
{
	//
}

#endif