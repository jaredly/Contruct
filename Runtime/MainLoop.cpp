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

////////////////////////////////
// Run app - the main loop
void CRuntime::Run()
{
	firstFrame = true;

	lastTimer = 0;
	lastFrameCount = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&timerFreq);

	// For time delta
	QueryPerformanceCounter((LARGE_INTEGER*)&oldTime);

#ifdef CONSTRUCT_DIRECTX9

	POINT mouse;
	GetCursorPos(&mouse);

	if (!fullscreen)
		ScreenToClient(hWnds.front(), &mouse);

	// For interpolating the mouse coords over subframes
	float mx = mouse.x, my = mouse.y, oldmx = mouse.x, oldmy = mouse.y;
	linearMouseX = mx;
	linearMouseY = my;

	// Motion blur enabled:
	while (bRunning) {

		if (motionBlur)
		{
			renderer.BeginScene();

			// Temporally anti-alias 'blurNumSteps' frames.
			renderer.SetRenderTarget(blurFloatAccumulate);
			renderer.ClearRenderTarget();

			cr::color accumFilter = cr::color(1.0 / (float)blurNumSteps, 1.0, 1.0, 1.0).premultiplied();

			// Handle the timedelta here
			QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
			timedelta = (double)(startTime - oldTime) / (double)timerFreq;
			oldTime = startTime;

			// Split the timedelta for temporal anti-aliasing
			timedelta /= (double)blurNumSteps;

			// Apply timescaling (done in GetTimeDelta now)
			//timedelta *= timeScale;

			// Linearly interpolate the mouse position over these steps
			GetCursorPos(&mouse);

			if (!fullscreen)
				ScreenToClient(hWnds.front(), &mouse);

			oldmx = mx;
			oldmy = my;
			mx = mouse.x;
			my = mouse.y;

			for (int i = 0; i < blurNumSteps; i++) {

				// Calculate the clock hacks to make the runtime think more time is passing than really is
				//clockTemporalOffset = (int)(timedelta * 1000.0) * i;
				clockTemporalOffset = 0;
				//timer += (timedelta * timeScale);

				// Kahan summation to minimise rounding errors in timer
				timerY = (timedelta * timeScale) - timerC;
				timerT = timer + timerY;
				timerC = (timerT - timer) - timerY;
				timer = timerT;

				// Linearly interpolate the mouse over the frames from the old to the new mouse coordinate.
				linearMouseX = oldmx + ((mx - oldmx) * (float)(i+1)) / (float)blurNumSteps;
				linearMouseY = oldmy + ((my - oldmy) * (float)(i+1)) / (float)blurNumSteps;
			
				// Draw frame to 32-bit integer texture. Guaranteed to be opaque.
				renderer.SetRenderTarget(tempDisplayTarget);

				// Process a frame
				if (Frame(false)) {		// Do not present - leave scene on tempDisplayTarget
					bRunning = false;
					break;
				}

				// Accumulate additively
				renderer.SetAdditiveBlending();

				if (blurFloatAccumulate == unallocated_texture)
					InitMotionBlur();

				renderer.SetRenderTarget(blurFloatAccumulate);
				renderer.SetTexture(tempDisplayTarget);

				renderer.Quad(winWidthOffset, winHeightOffset, 0.0, cr::origin, accumFilter);

				renderer.SetAlphaBlending();

			}

			if (!bRunning)
				break;

			// Finally copy the accumulated buffer to the display
			if (renderer.GetMultiMonitorMode() == cr::multimonitor_singlescreen) {

				renderer.SetRenderTarget(NULL);
				renderer.SetTexture(blurFloatAccumulate);
				renderer.Quad(winWidthOffset, winHeightOffset);
			}
			// Clone multi-screen mode draws same surface to all displays
			else if (renderer.GetMultiMonitorMode() == cr::multimonitor_clone) {
				BackBufferIterator b = backBuffers.begin();

				for ( ; b != backBuffers.end(); b++) {
					renderer.SetBackBuffer(*b);
					renderer.SetRenderTarget(NULL);
					renderer.SetTexture(blurFloatAccumulate);
					renderer.Quad(0.0, 0.0);
				}
			}

			renderer.EndScene();
			batchesPerPresent = renderer.GetBatchesPerPresent();

			__int64 startPresent, endPresent;
			QueryPerformanceCounter((LARGE_INTEGER*)&startPresent);

			renderer.Present();

			QueryPerformanceCounter((LARGE_INTEGER*)&endPresent);
			presentTime += (double)(endPresent - startPresent) / (double)timerFreq;

			turnOnMotionBlur = false;

			// Only change the state of 'motionBlur' flag here, to allow smooth enabling and disabling
			// at runtime.
			if (turnOffMotionBlur) {
				turnOffMotionBlur = false;
				motionBlur = false;
			}
		}
		else {

			// Motion blur off: ordinary render
			if (Frame())
				break;

			turnOffMotionBlur = false;

			if (turnOnMotionBlur) {
				turnOnMotionBlur = false;
				motionBlur = true;
			}

		}
	}

#else
	while (bRunning)
	{
		
		// Frame() returns true if the application is to exit
		if (Frame())
			break;

	}
#endif

	// About to exit
	GenerateEvent(-1, SYSTEM_ENDOFAPP, NULL);
}

// Perform one iteration of the main loop in CRuntime::Run()
bool CRuntime::Frame(bool present)
{
	// Calculate time delta since old frame, in seconds
	if (present) {

		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
		timedelta = (double)(startTime - oldTime) / (double)timerFreq;
		oldTime = startTime;

		// Apply time scaling (now done in GetTimeDelta)
		//timedelta *= timeScale;

		// Kahan summation to minimise rounding errors in timer
		timerY = (timedelta * timeScale) - timerC;
		timerT = timer + timerY;
		timerC = (timerT - timer) - timerY;
		timer = timerT;

		// Prevent jumping for modal windows etc.
		if (GetTimeDelta() >= 1.0)
			timedelta = 0.0;
	}

#ifndef CONSTRUCT_SDL

	//Handle all messages
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// Check for plugin wanting a call to PreTranslateMessage
		if (!pluginPreMsgHooks.empty()) {
			list<CRunObject*>::iterator i = pluginPreMsgHooks.begin();
			const list<CRunObject*>::const_iterator hooks_end = pluginPreMsgHooks.end();

			for ( ; i != hooks_end; ++i) {

				_D_ENTER(DCS_PLUGIN, *i);

				// Return 0 from PreTranslateMessage indicates message should be translated/dispatched
				if ((*i)->PreTranslateMessage(&msg) == 0) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				_D_RUNTIME();
				// else, the return was nonzero, indicating the message was translated and should not be dispatched.
			}
		}
		else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

#ifdef CONSTRUCT_DIRECTX9
	// Custom asynchronous window dragging: update if dragging
	if (dragging) {

		// End drag when left button up
		if (!AsyncKeyDown(VK_LBUTTON))
			dragging = false;
		// Still in drag - keep window positioned
		else {
			POINT pt;
			GetCursorPos(&pt);

			expectedResize = true;
			SetWindowPos(hWnds.front(), NULL, pt.x - mouseXOffset, pt.y - mouseYOffset, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			expectedResize = false;
		}
	}
#endif

	// Perform the drawing
	PreFrame(present);

#else

	SDL_Event Event;

    //Handle all messages
	while (SDL_PollEvent(&Event)) 
	{
		if (Event.type == SDL_QUIT)
			bRunning = false;
    }

#endif
	// Quit
	if (!bRunning) return true;

	// Is suspended: simply loop round the message pump
#ifdef CONSTRUCT_DEBUGGER
	if (isSuspended) {
		Sleep(10);
		return false;
	}
#endif

	// Run the frame render to window
	PostFrame(present);

	// Show windows only after first frame has been rendered
	if (firstFrame) {

		WindowIterator w = hWnds.begin();

		for ( ; w != hWnds.end(); w++)
			ShowWindow(*w, SW_SHOW);

		firstFrame = false;
	}

	frameCount++;

	int clocktimer = clock() - clockOffset;

	// If set FPS, sleep for any spare time till next frame
	if (fpsMode == cr::framerate_fixed) {

		static __int64 fixed_framecount = 0;
		__int64 userFps64 = userFps;

		//__int64 tick_time = (__int64)1000 / userFps64;

		// If more than 2 frames behind scheduled, skip
		__int64 time_behind_schedule = ((__int64)clock() - clockOffset) - (((__int64)1000 * fixed_framecount) / userFps64);

		if (time_behind_schedule >= (2 * (__int64)1000) / userFps64)
			fixed_framecount += (time_behind_schedule * userFps64) / (__int64)1000;

		// Sleep up to the current scheduled frame, should iron out inaccuracies
		// with Sleep()'s timing
		while ((__int64)clock() - clockOffset < ((__int64)1000 * fixed_framecount) / userFps64)
			Sleep(1);			

		fixed_framecount++;
	}
	else if (fpsMode == cr::framerate_vsynced) {
		// When a vsynced app is hidden, Present() takes no time and it seems to run uncapped.
		// Cap at 200fps (5ms gaps)
		__int64 nowTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&nowTime);

		while (((nowTime - startTime) * 1000) / timerFreq <= 4) {
			Sleep(1);
			QueryPerformanceCounter((LARGE_INTEGER*)&nowTime);
		}
	}


	// New second: update fps counter
	if (clocktimer >= lastTimer + 1000) {
		fps = frameCount - lastFrameCount;

		lastSecondPresentTime = presentTime;
		presentTime = 0;

#ifdef CONSTRUCT_DIRECTX9
		if (motionBlur && blurNumSteps != 0)
			fps /= blurNumSteps;
#endif

		lastFrameCount = frameCount;
		lastTimer = clocktimer;

		// Show FPS in caption if specified
		
#if defined(CONSTRUCT_PREVIEW) && !defined(APPRUNTIME)
		if (fpsInCaption && !fullscreen) {
			CString caption;

			char strbuf[256];
			GetWindowText(hWnds.front(), strbuf, 256);
			caption = strbuf;

#ifdef CONSTRUCT_DIRECTX9
			if (caption.Find("(DX9 runtime") != -1) 
			{
				caption.Delete(caption.Find("(DX9 runtime"), caption.GetLength());

				//if (!motionBlur)
				//	batchesPerPresent = renderer.GetBatchesPerPresent();

				CString fpsText;
				fpsText.Format("(DX9 runtime, %d FPS, %.2f mb VRAM)", fps, (float)renderer.GetTotalTextureMemoryUsage() / 1048576.0f);
				caption += fpsText;

				SetWindowText(hWnds.front(), caption);
			}
		}
#endif
#ifdef CONSTRUCT_SDL
			if (caption.Find("(SDL runtime") != -1) {
				caption.Delete(caption.Find("(SDL runtime"), caption.GetLength());

				CString fpsText;
				fpsText.Format("(SDL runtime, %d FPS)", fps);
				caption += fpsText;

				SDL_WM_SetCaption(caption, NULL);
			}
		}
#endif
#endif
	}

	return false;
}

void CRuntime::PreFrame(bool present)
{
#ifdef CONSTRUCT_DEBUGGER
	if (isSuspended)
		return;
#endif

	list<CRunLayout*>::iterator f;
	list<CRunLayout*>::iterator frames_end;

#ifndef APPRUNTIME

	////////////////////////
	// DRAW CURRENT FRAME //
	////////////////////////
#ifdef CONSTRUCT_DIRECTX9

	// No point drawing anything if the device is lost
	if (deviceAlreadyLost)
		return;

	// Update the window width/height offsets incase the window is bigger than the layout
	CRunLayout* pSysLayout = GetSystemFrame();
	winWidthOffset = max((winWidth / 2) - (pSysLayout->width / 2), 0);
	winHeightOffset = max((winHeight / 2) - (pSysLayout->height / 2), 0);

	RECT clientSize;
	GetClientRect(hWnds.front(), &clientSize);
	realWinWidth = clientSize.right - clientSize.left;
	realWinHeight = clientSize.bottom - clientSize.top;

	if (present) {

		renderer.BeginScene();

		// Get the back buffers for each multihead (or just the one if single screen)
		GetBackBuffers();

		// Use the first monitor backbuffer
		renderer.SetBackBuffer(backBuffers.front());
		renderer.SetTexture(NULL);
		renderer.SetRenderTarget(NULL);

		// Create temp texture if layout smaller than window
		if (pSysLayout->width < winWidth || pSysLayout->height < winHeight) {

			renderer.ClearRenderTarget();

			if (tempDisplayTarget == unallocated_texture)
				tempDisplayTarget = CreateDisplayTargetTexture();
		}
	}

	// Set primary display target.
	// When no effects, this can go straight to backbuffer (target -1).
	// However, when effects, this needs to go to the display texture since backbuffer is read-only,
	// then the display texture copied to BB.
	if (tempDisplayTarget == unallocated_texture)
		renderer.SetRenderTarget(NULL);
	else {
		if (multisamples > 0) {

			if (multisampleTargets[1] == unallocated_texture)
				multisampleTargets[1] = renderer.CreateRenderTargetTexture(winWidth, winHeight, cr::texture_format_a8r8g8b8, true);

			renderer.SetRenderTarget(multisampleTargets[1]);
		}
		// Multisampling off
		else
			renderer.SetRenderTarget(tempDisplayTarget);
	}
#endif

	// Render all running frames.
	f = runningFrames.begin();
	frames_end = runningFrames.end();

	backBufferRendered = false;
	systemFrameBlocked = false;

	for ( ; f != frames_end; f++) {

		// Set the system object current frame
		system.pLayout = *f;

		// Draw this frame
		if ((*f)->systemDrawn)
			(*f)->Draw();
		// Check if a non system modal frame is blocking system
		else if ((*f)->modal)
			systemFrameBlocked = true;
	}

#ifdef CONSTRUCT_DIRECTX9
	// Draw transition
	if (runningTransition && runningTransition->toFrame != NULL) {
		runningTransition->progress = (double)(GetAppClock() - runningTransition->msStart) / (double)runningTransition->msDuration;

		runningTransition->transition->pEffect->SetFloat(runningTransition->progressHandle, runningTransition->progress);

		// Draw the second frame to the transition target
		system.pLayout = runningTransition->toFrame;
		renderer.SetRenderTarget(tempTransitionTarget);
		runningTransition->toFrame->Draw();

		renderer.SetTexture(NULL);

		// Draw the first frame to the standard temp target, where it will be background
		system.pLayout = runningTransition->fromFrame;
		renderer.SetRenderTarget(tempDisplayTarget);
		runningTransition->fromFrame->Draw();

		renderer.SetTexture(NULL);

		// Now draw transition effect to backbuffer
		renderer.SetRenderTarget(NULL);
		backBufferRendered = true;

		// TODO: batch transitions
		renderer.EndBatch();

		unsigned int nPasses;
		ID3DXEffect* curFx = runningTransition->transition->pEffect;
		CRunEffect* f = runningTransition->transition;

		if (f->vForegroundTexture)	curFx->SetTexture(f->vForegroundTexture, tempTransitionTarget->texture_ptr);
		if (f->vBackgroundTexture)	curFx->SetTexture(f->vBackgroundTexture, tempDisplayTarget->texture_ptr);
		if (f->vPixelWidth)			curFx->SetFloat(f->vPixelWidth, 1.0f / (float)winWidth);
		if (f->vPixelHeight)		curFx->SetFloat(f->vPixelHeight, 1.0f / (float)winHeight);

		// Transitions are always rendering on fullscreen textures (0,0) to (1,1)
		if (f->vBgStart) {
			float zeroes[2] = {0.0f, 0.0f};
			curFx->SetFloatArray(f->vBgStart, zeroes, 2);
		}
		if (f->vBgEnd) {
			float ones[2] = {1.0f, 1.0f};
			curFx->SetFloatArray(f->vBgEnd, ones, 2);
		}

		curFx->Begin(&nPasses, 0);

		for (int i = 0; i < nPasses; i++) {

			curFx->BeginPass(i);

			// Use the last render texture as the draw texture.  May be SourceTexture.
			renderer.SetTexture(tempTransitionTarget);
			renderer.Quad(winWidthOffset, winHeightOffset);
			renderer.EndBatch();

			curFx->EndPass();

		}//for each pass

		curFx->End();
		renderer.EndBatch();

		// End of transition
		if (runningTransition->progress >= 1.0f) {
			DestroyRunningFrame(runningTransition->fromFrame);
			runningTransition->toFrame->systemDrawn = true;
			runningTransition = NULL;
		}
	}

	// Copy to BB if using effects
	if (present) {
		if (tempDisplayTarget != unallocated_texture && !backBufferRendered ) {

			renderer.UpdateTransform(false);

			// Copy multisample texture if used
			if (multisamples > 0 && renderer.GetRenderTarget() == multisampleTargets[1])
				renderer.CopyMultisampleSurfaceToTexture(multisampleTargets[1], tempDisplayTarget);

			// Single screen mode: ordinary draw to backbuffer
			if (renderer.GetMultiMonitorMode() == cr::multimonitor_singlescreen) {

				renderer.SetTexture(tempDisplayTarget);
				renderer.SetRenderTarget(NULL);
				renderer.Quad(winWidthOffset, winHeightOffset, winWidth, winHeight);
				DrawBorderForSmallLayouts();
			}
			// Clone mode: blit same surface to all screens
			else if (renderer.GetMultiMonitorMode() == cr::multimonitor_clone) {

				BackBufferIterator b = backBuffers.begin();

				for ( ; b != backBuffers.end(); b++) {

					renderer.EndBatch();
					renderer.SetTexture(NULL);
					renderer.SetBackBuffer(*b);
					renderer.SetRenderTarget(NULL);

					renderer.SetTexture(tempDisplayTarget);
					renderer.Quad(winWidthOffset, winHeightOffset, winWidth, winHeight);
					DrawBorderForSmallLayouts();

					runningFrames.front()->Draw(false);

					renderer.EndBatch();
				}

			}
		}

		renderer.EndScene();
	}

	// Render any frames that need to present to a different window
	vector<CRunLayout*>::iterator w = wndFramesToDraw.begin();

	for ( ; w != wndFramesToDraw.end(); w++) {

		if ((*w)->hWnd == NULL) continue;	// Got deleted since draw request

		LPDIRECT3DSURFACE9 pBackBuffer;
		(*w)->pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);

		// Render this window to our swap backbuffer
		renderer.SetBackBuffer(pBackBuffer);

		if (tempDisplayTarget == unallocated_texture)
			tempDisplayTarget = CreateDisplayTargetTexture();

		renderer.BeginScene();
		renderer.SetRenderTarget(tempDisplayTarget);
		
		system.pLayout = *w;
		(*w)->Draw();

		if (tempDisplayTarget != unallocated_texture) {
			renderer.SetTexture(tempDisplayTarget);
			renderer.SetRenderTarget(NULL);
			renderer.Quad(0,0);
		}

		renderer.EndScene();
		
		// Present to the target window - done at same time as main present
		//(*w)->pSwapChain->Present(NULL, NULL, (*w)->hWnd, NULL, 0);

		pBackBuffer->Release();
	}
#endif
#endif //not APPRUNTIME


}

#ifdef CONSTRUCT_DIRECTX9

// Draw a black border around the bottom and right edges if the layout is smaller than the window
void CRuntime::DrawBorderForSmallLayouts()
{
	if (winWidthOffset > 0) {
		renderer.SetTexture(NULL);
		
		rect rc;
		rc.left = winWidth - winWidthOffset;
		rc.right = winWidth;
		rc.top = 0;
		rc.bottom = winHeight;
		renderer.Quad(rc, 0.0, cr::origin, cr::opaque_black);
	}
	if (winHeightOffset > 0) {
		renderer.SetTexture(NULL);
		
		rect rc;
		rc.left = 0;
		rc.right = winWidth;
		rc.top = winHeight - winHeightOffset;
		rc.bottom = winHeight;
		renderer.Quad(rc, 0.0, cr::origin, cr::opaque_black);
	}
}
#endif

////////////////////////////////
// Do an application frame
void CRuntime::PostFrame(bool present)
{
	// Debugger will not manually DoFrame during window drags and sizes etc
#ifdef CONSTRUCT_DEBUGGER
	if (isSuspended)
		return;
#endif

	list<CRunLayout*>::iterator f;
	list<CRunLayout*>::iterator frames_end;

	// Update input state
	UpdateInputState();

	// Update the layout clocks
	f = runningFrames.begin();
	frames_end = runningFrames.end();

#ifndef CONSTRUCT_DIRECTX9
	const clock_t clockTemporalOffset = 0;
#endif

	for ( ; f != frames_end; f++)
		(*f)->currentClock = (timer * 1000.0) - (*f)->startClock;	

	// Process the built in keyboard control states
	ProcessControlState();


	///////////////////
	// ONFRAME CALLS //
	///////////////////
	callingOnFrames = true;
	ObjIterator i = onFrameCallees.begin();

	for ( ; i != onFrameCallees.end(); ) {

		// Don't call OnFrame for objects on a blocked system frame
		if (systemFrameBlocked && (*i)->pLayout->systemDrawn) {
			i++;
			continue;
		}

		_D_ENTER(DCS_PLUGIN, *i);

		solIndex = (*i)->info.iid;

		// If nonzero return, fast erase from the OnFrame callees list
		if ((*i)->OnFrame()) {

			_D_RUNTIME();

			int index = i - onFrameCallees.begin();
			*i = onFrameCallees.back();
			onFrameCallees.pop_back();
			i = onFrameCallees.begin() + index;

			// If last, stop loop
			if (onFrameCallees.empty())
				break;
		}
		else
			i++;

		_D_RUNTIME();
	}

	callingOnFrames = false;

	if (!calleeBuffer.empty()) {
		onFrameCallees.insert(onFrameCallees.end(), calleeBuffer.begin(), calleeBuffer.end());
		calleeBuffer.resize(0);
	}

	////////////////////
	// RUN EVENT LIST //
	////////////////////
	// This is done just before Present so whilst the event list processes, the GPU can also
	// get on with its processing and have some extra time to finish its work.
	// Game may appear 1 frame behind but not a problem at high framerates, and a lot
	// more efficient.
	RunEventList();

	// FLUSH DELAYED OBJECTS
	if (delayedObjectsExist)
		FlushDelayedObjects();

	////////////////////
	// ONFRAME2 CALLS //
	////////////////////
	callingOnFrame2s = true;
	i = onFrame2Callees.begin();

	for ( ; i != onFrame2Callees.end(); ) {

		// Don't call OnFrame for objects on a blocked system frame
		if (systemFrameBlocked && (*i)->pLayout->systemDrawn) {
			i++;
			continue;
		}

		_D_ENTER(DCS_PLUGIN, *i);

		solIndex = (*i)->info.iid;

		// If nonzero return, fast erase from the OnFrame callees list
		if ((*i)->OnFrame2()) {

			_D_RUNTIME();

			int index = i - onFrame2Callees.begin();
			*i = onFrame2Callees.back();
			onFrame2Callees.pop_back();
			i = onFrame2Callees.begin() + index;

			// If last, stop loop
			if (onFrame2Callees.empty())
				break;
		}
		else
			i++;

		_D_RUNTIME();
	}

	callingOnFrame2s = false;

	if (!calleeBuffer.empty()) {
		onFrame2Callees.insert(onFrame2Callees.end(), calleeBuffer.begin(), calleeBuffer.end());
		calleeBuffer.resize(0);
	}

	// Handle attributes like "Center view on me"
	HandleBuiltinAttributes();

	if (!(FrameDeathRow.empty())) {

		vector<CRunLayout*>::iterator f = FrameDeathRow.begin();
		vector<CRunLayout*>::const_iterator end = FrameDeathRow.end();

		for ( ; f != end; f++) {

			CRunLayout* pLayout = *f;

			CRunLayout* pOldFrame = system.pLayout;
			system.pLayout = pLayout;
			GenerateEvent(-1, SYSTEM_ENDOFFRAME, NULL);
			system.pLayout = pOldFrame;

			pLayout->DestroyNonGlobalObjects();

#ifndef APPRUNTIME
			if (pLayout->pSwapChain) {
				pLayout->pSwapChain->Release();
				pLayout->pSwapChain = NULL;
			}

			vector<CRunLayout*>::iterator del = find(wndFramesToDraw.begin(), wndFramesToDraw.end(), pLayout);
			if (del != wndFramesToDraw.end())
				wndFramesToDraw.erase(del);
#endif
		}

		FrameDeathRow.resize(0);
	}

	// Update the object inspector
#ifdef CONSTRUCT_DEBUGGER
	pObjectPage->RefreshInspection();
	pWatchPage->RefreshWatch();
	pObjectPage->UpdateObjectTree();
#endif

	FlushDeathRow();

#ifndef APPRUNTIME
#ifdef CONSTRUCT_DIRECTX9
	// Present the screen(s)
	//Display.Present();
	
	if (present) {

		HRESULT hr;
		
		renderer.EndBatch();
		
		/*
		SwapChainIterator s = swapChains.begin();

		for ( ; s != swapChains.end(); s++)
			hr = (*s)->Present(0, 0, NULL, NULL, 0);
			*/
			
		__int64 startPresent, endPresent;
		QueryPerformanceCounter((LARGE_INTEGER*)&startPresent);

		hr = renderer.Present();

		QueryPerformanceCounter((LARGE_INTEGER*)&endPresent);
		presentTime += (double)(endPresent - startPresent) / (double)timerFreq;

		// Lost device?
		if (hr == D3DERR_DEVICELOST) {

			if (!deviceAlreadyLost) {
				OnLostDevice();
				deviceAlreadyLost = true;
			}

			hr = renderer.GetDevice()->TestCooperativeLevel();

			if (hr == D3DERR_DEVICENOTRESET) {

				deviceAlreadyLost = false;

				//ReleaseSwapChains();
				ReleaseBackBuffers();

				// OK to reset now
				renderer.Reset();
				OnResetDevice();

				//GetSwapChains();
				//GetBackBuffers();

				// Trigger on device reset
				GenerateEvent(-1, SYSTEM_ONDEVICERESET, NULL);
			}
		}
		else if (FAILED(hr))
			throw cr::CDX9Error(_T("Failed to present the screen"), hr);
		else
			ReleaseBackBuffers();

		vector<CRunLayout*>::iterator w;
		for (w = wndFramesToDraw.begin(); w != wndFramesToDraw.end(); w++)
			(*w)->pSwapChain->Present(NULL, NULL, (*w)->hWnd, NULL, 0);

		wndFramesToDraw.resize(0);
	}
#endif
#else

	// App runtime does not vsync; simply run ~100fps
	Sleep(10);

#endif

	// Check if a file is waiting to be loaded or saved
	if (system.saving) {
		system.saving = false;

		if (system.loadPath == "<quicksave>") {
			CRuntime::Save(system.quicksave);
		}
		else {
			bin ar(g_allocator);
			CRuntime::Save(ar);
			ar.save(system.loadPath);
		}

	}
	if (system.loading) {
		system.loading = false;

		if (system.loadPath == "<quicksave>") {
			CRuntime::Load(system.quicksave);
		}
		else {
			bin ar(g_allocator);
			ar.load(system.loadPath);
			CRuntime::Load(ar);
		}

		GenerateEvent(-1, SYSTEM_ONLOAD, NULL);
	}

#ifdef CONSTRUCT_DIRECTX9
	// Check if the resolution is set to be changed
	if (system.changeresolution) {
		system.changeresolution = false;

		bool was_fullscreen = fullscreen;

		// Check display resolution supported for fullscreen mode
		if (fullscreen || system.changeResFullscreen == 1) {

			BOOL bRetVal;
			DEVMODE devMode;
			int iMode = 0;

			int wantedWidth = system.changeResWidth;
			int wantedHeight = system.changeResHeight;
			int bestWidth = 100000;
			int bestHeight = 100000;

			do
			{
				bRetVal = ::EnumDisplaySettings(NULL, iMode, &devMode);
				iMode++;

				if (bRetVal)
				{
					int curWidth = devMode.dmPelsWidth;
					int curHeight = devMode.dmPelsHeight;

					// This display mode is big enough to fit the display in, but is smaller than the last best size
					if ((curWidth >= wantedWidth && curHeight >= wantedHeight)
						&& (curWidth < bestWidth && curHeight < bestHeight)) {
						bestWidth = curWidth;
						bestHeight = curHeight;
					}
				}
			}
			while (bRetVal);

			// Still 100000x100000: no resolution found that supports this
			if (bestWidth != 100000 && bestHeight != 100000)
			{
				system.changeResWidth = bestWidth;
				system.changeResHeight = bestHeight;
			}
		}

		// Change fullscreen mode
		if (system.changeResFullscreen > -1) {

			fullscreen = (system.changeResFullscreen != 0);
			renderer.GetFirstPresent().Windowed = !fullscreen;

			// Modify window styles for windowed mode
			if (fullscreen) {
				expectedResize = true;
				CWnd w;
				w.Attach(hWnds.front());
				w.ModifyStyle(WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, WS_POPUP);

				CRect clientRec;
				clientRec.SetRect(0, 0, winWidth, winHeight);
				w.CalcWindowRect(&clientRec);
				w.SetWindowPos(NULL, 0, 0, clientRec.Width(), clientRec.Height(), SWP_NOMOVE | SWP_NOZORDER);

				w.Detach();
				expectedResize = false;
			}
		}

		vector<CRunLayout*>::iterator f = Frames.begin();

		/*
		for ( ; f != Frames.end(); f++) {
			if ((*f)->width < system.changeResWidth || (*f)->height < system.changeResHeight) {

				AddDebugLogMessage("Attempted to change resolution larger than a layout - no action taken");

				return;		// Not allowed to change res if a layout is smaller than new resolution size
			}
		}
		*/

		winWidth = system.changeResWidth;
		winHeight = system.changeResHeight;

		// Update winWidth/winHeight for all layouts
		f = Frames.begin();

		for ( ; f != Frames.end(); f++) {
			(*f)->winWidth = winWidth;
			(*f)->winHeight = winHeight;
		}

		// Change window size to new size
		expectedResize = true;
		CWnd w;
		w.Attach(hWnds.front());
		CRect clientRec;
		clientRec.SetRect(0, 0, winWidth, winHeight);
		w.CalcWindowRect(&clientRec);
		w.SetWindowPos(NULL, 0, 0, clientRec.Width(), clientRec.Height(), SWP_NOMOVE | SWP_NOZORDER);
		w.Detach();
		expectedResize = false;

		RECT clientSize;
		GetClientRect(hWnds.front(), &clientSize);
		realWinWidth = clientSize.right - clientSize.left;
		realWinHeight = clientSize.bottom - clientSize.top;

		// Windowed
		if (renderer.GetFirstPresent().Windowed) {

			RECT rWindow;

			//Get window bounds
			GetClientRect(hWnds.front(), &rWindow);

			//Setup backbuffer
			renderer.GetFirstPresent().BackBufferWidth = rWindow.right - rWindow.left;
			renderer.GetFirstPresent().BackBufferHeight = rWindow.bottom - rWindow.top;
		}
		// Fullscreen mode
		else {
			renderer.GetFirstPresent().BackBufferWidth = winWidth;
			renderer.GetFirstPresent().BackBufferHeight = winHeight;
		}

		// Changing multisampling mode?
		if (system.changeMultisampling > -1) {
			renderer.GetFirstPresent().MultiSampleType = cr::CDX9Renderer::get_d3d9_multisample(system.changeMultisampling + 1);
		}

		renderer.GetFirstPresent().BackBufferCount = 1;
		renderer.GetFirstPresent().BackBufferFormat = D3DFMT_X8R8G8B8;

		// Save or free all DirectX resources that are about to bomb out
		//OnLostDevice();

		//ReleaseSwapChains();

		// Runtime lost device handler
		OnLostDevice();

		// Do the device reset
		renderer.Reset();

		// Runtime reset device handler
		OnResetDevice();
		
		// Update bounding boxes for everything
		ObjTypeIterator t = objects.begin();

		for ( ; t != objects.end(); t++) {
			ObjIterator i = (*t)->instances.begin();

			for ( ; i != (*t)->instances.end(); i++) {
				UpdateBoundingBox(*i);
			}
		}

		// Add window styles for windowed mode
		if (!fullscreen) {

			expectedResize = true;

			// Modify window styles for windowed mode
			CWnd w;
			w.Attach(hWnds.front());

			if (caption)
				w.ModifyStyle(WS_POPUP, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION);

			CRect clientRec;
			clientRec.SetRect(0, 0, winWidth, winHeight);
			w.CalcWindowRect(&clientRec);

			// Centre the window when changing back from fullscreen
			if (was_fullscreen) {
				DEVMODE DevMode;
				EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode);
				int windowX = (DevMode.dmPelsWidth / 2) - (clientRec.Width() / 2);
				int windowY = (DevMode.dmPelsHeight / 2) - (clientRec.Height() / 2);
				//w.SetWindowPos(NULL, windowX, windowY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				w.SetWindowPos(NULL, windowX, windowY, clientRec.Width(), clientRec.Height(), SWP_NOZORDER);
			}

			w.Detach();

			expectedResize = false;
		}

		system.changeResFullscreen = -1;
		system.changeMultisampling = -1;

		// Trigger on device reset
		GenerateEvent(-1, SYSTEM_ONDEVICERESET, NULL);
	}
#endif // DX9 runtime
}