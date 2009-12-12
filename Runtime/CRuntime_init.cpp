
#include "StdAfx.h"

/////////////////////////////
// Constructor
CRuntime::CRuntime(bool& success)
: mtRand53(time(0))
#ifndef APPRUNTIME
	,
  render_layer(this),
  VRuntime(reinterpret_cast<IRenderer*>(&render_layer))
#endif
{
	// Zero out vars
	nullPtr = 0;
	hInstance = NULL;
	disableWindowsKey = bRunning = completedInitialisation = false;
	curtempstr = frameCount = fps = curUid = totalObjects = 0;
	bkColor = RGB(255,255,255);
	pCurrentEvent = NULL;
	bRunning = true;
	translationMode = TRANSLATE_SCROLL;

#ifdef CONSTRUCT_DEBUGGER
	dcs = DCS_RUNTIME;
	g_actionIndex = -1;
	isSuspended = false;
#endif

	// The 5 rendering buffers
#ifdef CONSTRUCT_DIRECTX9
	tempLayerTarget = unallocated_texture;
	tempDisplayTarget = unallocated_texture;
	tempRenderTarget[0] = unallocated_texture;
	tempRenderTarget[1] = unallocated_texture;
	tempSourceTarget = unallocated_texture;
	tempTransitionTarget = unallocated_texture;
	multisampleTargets[0] = unallocated_texture;
	multisampleTargets[1] = unallocated_texture;
	dinput = NULL;

	motionBlur = false;
	turnOffMotionBlur = false;
	turnOnMotionBlur = false;
	blurNumSteps = 0;
	clockTemporalOffset = 0;
	timer = 0;
	timerC = 0;
	timerY = 0;
	timerT = 0;

	blurFloatAccumulate = unallocated_texture;
	presentTime = 0;
	lastSecondPresentTime = 0;
#endif

	solIndex = 0;

	// Zero out input state
	memset(&inputState, 0, sizeof(inputState));
	changeFrame = -1;

	CapReader.pRuntime = this;

	// Initialise system object
	system.pType = NULL;
	system.pRuntime = this;
	system.pCRuntime = this;
	system.info.destroying = false;

	systemFrameBlocked = false;
	
	disableAlt = false;
	callingOnFrames = false;
	callingOnFrame2s = false;
	timedelta = 0;
	fpsInCaption = 0;
	timeScale = 1.0;

	overrideTimeDelta = false;
	overrideTimeDeltaFPS = 60;

	delayedObjectsExist = false;
	isTopLevelTrigger = true;
	expectedResize = false;

	dragging = false;
	mouseXOffset = mouseYOffset = 0;

	suppress_else_event = false;

	// Check for D3DX DLL if in DirectX9 mode
#ifdef CONSTRUCT_DIRECTX9
	//nextFontID = 0;
	runningTransition = NULL;
	deviceAlreadyLost = false;

	// Try to find needed DLL (eg "d3dx9_39.dll")
	// Changed to August 2008 D3DX by AG for XAudio2
	CString d3dxDll;
	d3dxDll.Format("d3dx9_%d.dll", D3DX_SDK_VERSION);
	DWORD dw = SearchPath(NULL, d3dxDll, NULL, 0, NULL, NULL);

	// Not found
	if (dw == 0) {
		CString message = "You need the August 2008 DirectX update in order to run this application.  Click 'OK' to open the DirectX download page, where you can install the August 2008 DirectX End-User Runtime."
			"  Press 'Cancel' to exit the application.";

		int click = MessageBox(NULL, message, "DirectX update required", MB_OKCANCEL | MB_ICONHAND);

		if (click == IDOK)
			ShellExecute(NULL, NULL, "http://www.microsoft.com/downloads/Browse.aspx?displaylang=en&categoryid=2", NULL, NULL, SW_SHOW);

		// Terminate
		success = false;
	}

#endif
}

// Destructor
CRuntime::~CRuntime()
{
	// Close runtime
	Close();
}

void CRuntime::CreateTempDirectory()
{
	// Get the temporary directory
	char temppath[MAX_PATH+1];
	GetTempPath(512, temppath);
	GetLongPathName(temppath, temppath, MAX_PATH+1);

	char tempfolder[MAX_PATH+1];
	GetTempFileName(temppath, "cap", 0, tempfolder);

	tempDir = tempfolder;

	// Delete the temp file
	DeleteFile(tempDir);

	pythonDir = tempDir;

	tempDir += "\\";
	pythonDir = tempDir + "Python";

	pythonZipPath = tempDir + "PythonLibs.zip";

	// Make the directory
	if (CreateDirectory(tempfolder, NULL) == 0) {
		CString msg;
		msg.Format("Error creating temporary folder at %s (%d)!", tempfolder, GetLastError());
		MessageBox(NULL, msg, "Construct Runtime", MB_ICONHAND | MB_OK);
	}
	
	if (CreateDirectory(pythonDir, NULL) == 0) {
		CString msg;
		msg.Format("Error creating temporary folder at %s (%d)!", pythonDir, GetLastError());
		MessageBox(NULL, msg, "Construct Runtime", MB_ICONHAND | MB_OK);
	}

}

// Helper function to get a pointer and size of a resource's data
HGLOBAL OpenResourceBinary(int id, const char* type, BYTE*& pData, int& size)
{
	HRSRC theResource = FindResource(NULL, MAKEINTRESOURCE(id), type);

	int e = GetLastError();
	if (theResource == NULL) {
		CString msg;
		msg.Format("Error loading \"%s\" (%d)", type, e);
		throw runtime_error((const char*)msg);
	}

	HGLOBAL theData = LoadResource(NULL, theResource);
	pData = (BYTE*)LockResource(theData);
	size = SizeofResource(NULL, theResource);
	return theData;	// Must call FreeResource() on this
}

#ifdef CONSTRUCT_DIRECTX9
void CRuntime::InitMotionBlur()
{
	try {
		if (blurFloatAccumulate == unallocated_texture)
			blurFloatAccumulate = renderer.CreateRenderTargetTexture(winWidth, winHeight, cr::texture_format_a16b16g16r16f);

		if (tempDisplayTarget == unallocated_texture)
			tempDisplayTarget = CreateDisplayTargetTexture();
	}
	catch (cr::CDX9Error e) {
#ifdef CONSTRUCT_PREVIEW
		CString msg;
		msg.Format("Could not enable motion blur for previewing this application:\n\n"
					"%s\n\n"
					"Motion blur has been disabled.  Your hardware may not support it.\n\n"
					"This message does not appear in exported EXEs, but motion blur will silently disable where it is not supported.",
					e.GetMsg());
		MessageBox(NULL, msg, "Motion blur effect", MB_ICONEXCLAMATION | MB_OK);
#endif
		motionBlur = false;
	}
}
#endif

HHOOK g_hKeyboardHook = NULL;

// Initialise runtime
bool CRuntime::Initialise(CRuntimeSetup* crSetup)
{
	// Start by getting and saving the CPU capabilities.
	DWORD cpuFeatures = GetCPUCaps();
	supportMMX = cpuFeatures & CPU_FEATURE_MMX;
	supportSSE = cpuFeatures & CPU_FEATURE_SSE;
	supportSSE2 = cpuFeatures & CPU_FEATURE_SSE2;

	// Construct requires MMX for collisions
#ifndef APPRUNTIME
	if (!supportMMX)
		throw runtime_error("Your CPU does not support MMX technology (it must be pretty old!).  Please buy a newer PC then try again!");

#endif

	// Save hInstance
	hInstance = crSetup->hInstance;

	CapReader.pRuntime = this;

	CreateTempDirectory();

	// Get app properties
	BYTE* pData;
	int dataSize;
	HGLOBAL hData = OpenResourceBinary(997, "APPBLOCK", pData, dataSize);
	CapReader.ReadAppProperties(pData, dataSize, props);
	FreeResource(hData);

	// Cannot use both multisampling and motion blur
#ifndef APPRUNTIME
	if (multisamples > 0 && motionBlur)
		throw runtime_error("Cannot enable both multisamples and motion blur.  Please change one of these settings.");
#endif

#ifdef PYTHON
	// Get python
	hData = OpenResourceBinary(992, "PYTHONLIBS", pData, dataSize);
	CapReader.ReadPythonResources(pData, dataSize);
	FreeResource(hData);
#endif

	// Get menu resources
	hData = OpenResourceBinary(993, "MENUBLOCK", pData, dataSize);
	CapReader.ReadMenuResources(pData, dataSize, menus);
	FreeResource(hData);

	crSetup->winWidth = props.winWidth;
	crSetup->winHeight = props.winHeight;
	crSetup->eyeDistance = props.eyeDistance;
	crSetup->screensaver = props.screensaver;
	fpsMode = props.fpsMode;
	userFps = props.fps;

	//if (disableWindowsKey)
	//	g_hKeyboardHook = SetWindowsHookEx( WH_KEYBOARD_LL,  DisableWinKeyKeyboardProc, hInstance, 0);

#ifdef CONSTRUCT_DIRECTX9
	crSetup->display_params.fps_mode = props.fpsMode;
	crSetup->display_params.fullscreen = fullscreen = props.fullscreen;
	crSetup->display_params.backbuffer_width = props.winWidth;
	crSetup->display_params.backbuffer_height = props.winHeight;

	switch (multisamples) {
	case 0:	// Off
		crSetup->display_params.multisamples = 0;
		break;
	case 1: // 2x
		crSetup->display_params.multisamples = 2;
		break;
	case 2:	// 4x
		crSetup->display_params.multisamples = 4;
		break;
	case 3: // 8x
		crSetup->display_params.multisamples = 8;
		break;
	default:
		crSetup->display_params.multisamples = 0;
		break;
	}

	// PreInit gets the D3D caps and allows MakeWindows to create the correct number of windows to pass to Init()
	//Display.SetMultiMonitorMode(MMM_CLONE);
	//Display.PreInit();
	renderer.LoadD3D();
#endif

#ifndef CONSTRUCT_SDL
	// Create a window for the runtime
	if (!MakeWindows(crSetup))
		throw runtime_error("Cannot create window");
#else
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
		exit(1);

	// Register SDL_Quit to be called at exit; makes sure things are cleaned up when we quit.
	atexit(SDL_Quit);
    
	// Attempt to create window with 32bit pixels in hardware
	Display.screen = SDL_SetVideoMode(props.winWidth, props.winHeight, 32, SDL_HWSURFACE);
	SDL_SetAlpha(Display.screen, SDL_SRCALPHA, 255); // Set alpha to normal
#endif

	InitCommonControls();

	// The preview runtimes show the runtime in the title bar.
#ifdef CONSTRUCT_PREVIEW
	#ifdef CONSTRUCT_DIRECTX9
	props.appTitle += " (DX9 runtime)";
	#endif
	#ifdef APPRUNTIME
	props.appTitle += " (App runtime)";
	#endif
	#ifdef CONSTRUCT_SDL
	props.appTitle += " (SDL runtime)";
	#endif
#endif

#ifndef CONSTRUCT_SDL
	SetWindowText(hWnds.front(), props.appTitle);
#else
	SDL_WM_SetCaption(props.appTitle, NULL);
#endif

	// Restore mouse cursor from hourglass
	if (!props.screensaver)
		SetCursor(LoadCursor(NULL, IDC_ARROW));

	// Load a menu
	if (props.UseMenu)
		if (menus.size() != 0)
			SetMenu(hWnds.front(), menus[0]);

#ifndef APPRUNTIME
#ifndef CONSTRUCT_SDL
	// Direct-X setup
	crSetup->display_params.hWnds = hWnds;
	crSetup->display_params.hFocusWnd = hWnds.front();

	winWidthOffset = 0;
	winHeightOffset = 0;

	// Retrieve all display modes: can't set an invalid display mode size
	if (crSetup->display_params.fullscreen) {
		BOOL bRetVal;
		DEVMODE devMode;
		int iMode = 0;

		int wantedWidth = crSetup->display_params.backbuffer_width;
		int wantedHeight = crSetup->display_params.backbuffer_height;
		int bestWidth = 100000;
		int bestHeight = 100000;

		bool found = false;

		do
		{
			bRetVal = ::EnumDisplaySettings(NULL, iMode, &devMode);
			iMode++;

			if (bRetVal)
			{
				int curWidth = devMode.dmPelsWidth;
				int curHeight = devMode.dmPelsHeight;

				// Display mode found!
				if (curWidth == wantedWidth && curHeight == wantedHeight)
					found = true;

				// This display mode is big enough to fit the display in, but is smaller than the last best size
				if ((curWidth >= wantedWidth && curHeight >= wantedHeight)
					&& (curWidth < bestWidth && curHeight < bestHeight)) {
					bestWidth = curWidth;
					bestHeight = curHeight;
				}
			}
		}
		while (bRetVal);

		// Identical display mode not found: use next best that can fit it all on
		if (!found) {

			// Still 100000x100000: no resolution found that supports this
			if (bestWidth == 100000 || bestHeight == 100000)
				throw runtime_error("The display resolution required by this application is not supported.");

	#ifdef CONSTRUCT_PREVIEW
			CString msg;
			msg.Format("Switching to Fullscreen mode: Display mode %d x %d not supported, switching to next best %d x %d.\n\n"
				"The 'Window Width' and 'Window Height' values in Application Properties define the fullscreen resolution.",
				crSetup->display_params.backbuffer_width, crSetup->display_params.backbuffer_height, bestWidth, bestHeight);
			MessageBox(NULL, msg, "Fullscreen preview", MB_OK | MB_ICONEXCLAMATION);
	#endif

			crSetup->display_params.backbuffer_width = wantedWidth = bestWidth;
			crSetup->display_params.backbuffer_height = wantedHeight = bestHeight;
		}
	}

	// Set the eye distance before we initialize
	eyeDistance = crSetup->eyeDistance;
	renderer.SetEyeDistance(eyeDistance);

	// Start up the display engine
	//Display.Init(&(crSetup->d3dDisplaySetup));
	renderer.CreateDevice(crSetup->display_params);

	/*
	// No identical match for display mode
	if (crSetup->d3dDisplaySetup.resWidth != actualDisplayWidth || crSetup->d3dDisplaySetup.resHeight != actualDisplayHeight)
		if (tempDisplayTarget == unallocated_texture)
			tempDisplayTarget = CreateDisplayTargetTexture();

		winWidthOffset = (actualDisplayWidth - crSetup->d3dDisplaySetup.resWidth) / 2;
		winHeightOffset = (actualDisplayHeight - crSetup->d3dDisplaySetup.resHeight) / 2;
	}
	*/

	GetSwapChains();

	// Multi-monitor settings require the temp display target

	if (renderer.GetMultiMonitorMode() != cr::multimonitor_singlescreen) {
		if (tempDisplayTarget == unallocated_texture)
				tempDisplayTarget = CreateDisplayTargetTexture();
	}

	// Linear resizers
	if (props.sampler == 0) {
		renderer.SetSamplerState(cr::ss_magfilter, cr::ssv_point);
		renderer.SetSamplerState(cr::ss_minfilter, cr::ssv_point);
	}
	if (props.sampler == 1) {
		renderer.SetSamplerState(cr::ss_magfilter, cr::ssv_linear);
		renderer.SetSamplerState(cr::ss_minfilter, cr::ssv_linear);
	}

	// Premultiplied alpha mode
	renderer.SetAlphaBlending();

	// Runtime uses clamp-sampling
	renderer.SetSamplerState(cr::ss_addressu, cr::ssv_clamp);
	renderer.SetSamplerState(cr::ss_addressv, cr::ssv_clamp);

	// Create the multisampling target if one required
	//if (multisamples > 0)		// 0 is off
	//	multisampleTargets[0] = renderer.CreateRenderTargetTexture(crSetup->winWidth, crSetup->winHeight, cr::texture_format_a8r8g8b8, true);

#if defined(CONSTRUCT_DIRECTX9) && defined(CONSTRUCT_PREVIEW)
	// Handle shader simulation
	if (simShader != SS_NOSIM) {

		UINT ps_major = D3DSHADER_VERSION_MAJOR(renderer.GetCaps().PixelShaderVersion);
		UINT ps_minor = D3DSHADER_VERSION_MINOR(renderer.GetCaps().PixelShaderVersion);

		CString hardwarePS;
		hardwarePS.Format("%d.%d", ps_major, ps_minor);

		CString simulatedPS;

		switch (simShader) {
		case SS_PS14:
			simulatedPS = "1.4";
			break;
		case SS_PS11:
			simulatedPS = "1.1";
			break;
		case SS_PS00:
			simulatedPS = "0.0";
			break;
		}

		float ps_version = atof(hardwarePS);
		float sim_version = atof(simulatedPS);

		// If fullscreen MessageBox()'s won't work
		if (!fullscreen) {
		
			if (sim_version > ps_version) {
				CString msg;
				msg.Format("You have chosen to simulate a pixel shader version (PS %s) higher than your hardware supports (PS %s).  "
					"You can only simulate lower hardware capabilities.  The application will continue to use PS %s.",
					simulatedPS, hardwarePS, simulatedPS);
				MessageBox(NULL, msg, "Simulate shader", MB_OK | MB_ICONEXCLAMATION);
			}
			else if (sim_version == ps_version) {
				CString msg;
				msg.Format("You have chosen to simulate the same pixel shader version as your hardware supports (PS %s).  "
					"The application will continue normally.", hardwarePS);
				MessageBox(NULL, msg, "Simulate shader", MB_OK | MB_ICONINFORMATION);
			}
			else {
				CString msg;
				msg.Format("You are simulating pixel shader %s capabilites.  Your hardware supports pixel shader %s.",
					simulatedPS, hardwarePS);
				MessageBox(NULL, msg, "Simulate shader", MB_OK | MB_ICONEXCLAMATION);
			}

		}
	}
#endif // shader sims
#endif

	// Load the PNG image list
	hData = OpenResourceBinary(995, "IMAGEBLOCK", pData, dataSize);
	CapReader.ReadImageData(pData, dataSize, imagehandle_to_address);
	FreeResource(hData);

#ifdef CONSTRUCT_DIRECTX9
	// Initialise DirectInput
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
								IID_IDirectInput8, (void**)&dinput, NULL)))
		throw runtime_error("Failed to initialise DirectInput.  Please ensure you have DirectX 8 or above installed!");

	// Initialize the keyboard
	if (FAILED(dinput->CreateDevice(GUID_SysKeyboard, &d_keyboard, NULL)))
		throw runtime_error("Failed to initialise DirectInput.");
	if (FAILED(d_keyboard->SetDataFormat(&c_dfDIKeyboard)))
		throw runtime_error("Failed to initialise DirectInput.");

	if (FAILED(d_keyboard->SetCooperativeLevel(hWnds.front(), DISCL_BACKGROUND |
											 DISCL_NONEXCLUSIVE)))
		throw runtime_error("Failed to initialise DirectInput.");

	if (FAILED(d_keyboard->Acquire()))
		throw runtime_error("Failed to initialise DirectInput.");

	// initialize the mouse
	if (FAILED(dinput->CreateDevice(GUID_SysMouse, &d_mouse, NULL)))
		throw runtime_error("Failed to initialise DirectInput.");
	if (FAILED(d_mouse->SetCooperativeLevel(hWnds.front(), DISCL_BACKGROUND |
										  DISCL_NONEXCLUSIVE)))
		throw runtime_error("Failed to initialise DirectInput.");

	if (FAILED(d_mouse->SetDataFormat(&c_dfDIMouse)))
		throw runtime_error("Failed to initialise DirectInput.");

	if (FAILED(d_mouse->Acquire()))
		throw runtime_error("Failed to initialise DirectInput.");

	inputState.isDirectInput = true;

#endif
#endif //#ifndef APPRUNTIME

	// Save window dimensions
	winWidth = crSetup->winWidth;
	winHeight = crSetup->winHeight;
	RECT clientSize;
	GetClientRect(hWnds.front(), &clientSize);
	realWinWidth = clientSize.right - clientSize.left;
	realWinHeight = clientSize.bottom - clientSize.top;

	// Unpack dependencies before loading plugins
	UnpackDependencies();

	// Unpack the resource plugins to the temp dir and load them
	UnpackPlugins(1000);

	// Read the frame data
	hData = OpenResourceBinary(998, "LEVELBLOCK", pData, dataSize);
	CapReader.ReadFrameData(pData, dataSize);
	FreeResource(hData);

#ifdef CONSTRUCT_DEBUGGER
	// Create debugger once object types are known but before parsing event block (which may send logs)
	// Create invisible initially
	pDebug->Create();
	pDebug->ShowWindow(SW_HIDE);
#endif

	// Read the CAP event tree
	hData = OpenResourceBinary(999, "EVENTBLOCK", pData, dataSize);
	CapReader.ReadEventList(pData, dataSize);
	FreeResource(hData);

	// Iterate all events determining their modifiers
	vector<CRunLayout*>::iterator f = Frames.begin();
	const vector<CRunLayout*>::const_iterator Frames_end = Frames.end();

	for ( ; f != Frames_end; f++) {
		EventIterator e = (*f)->Events.begin();
		EventConstIterator Events_end = (*f)->Events.end();

		for ( ; e != Events_end; ++e) {

			// Recurse down tree finding SOL modifiers
			(*e)->GetSolModifiers(*f);

			// If this event line is a group, it can be marked as top level for optimisation
			if ((*e)->GetType() == EVENT_GROUP)
				((CEventGroup*)*e)->isTopLevel = true;
		}
	}

	// Initialise effects
#ifdef CONSTRUCT_DIRECTX9
	hData = OpenResourceBinary(994, "HLSL", pData, dataSize);
	CapReader.ReadHLSLData(pData, dataSize);
	FreeResource(hData);

	// If motionblur is required, set up the textures
	if (motionBlur) {

		InitMotionBlur();

	}

#endif //APPRUNTIME

	// Clock offset (timers relative to Run())
	clockOffset = clock();
	curFrame = 0;

	// Mark objects which are not to serialize
	CRunObjType* pNoSerialize = GetTypeFromName("No serialize");

	if (pNoSerialize) {
		if (!pNoSerialize->teams.empty()) {
			ObjTypeIterator t = pNoSerialize->teams.begin();
			ObjTypeIterator end = pNoSerialize->teams.end();

			for ( ; t != end; t++)
				(*t)->noSerialize = true;
		}		
	}

	// Set current frame
	CRunLayout* pFirstFrame = Frames.front();
	runningFrames.push_back(pFirstFrame);
	system.pLayout = pFirstFrame;
	pFirstFrame->systemDrawn = true;
	pFirstFrame->Load();

	// Load any other layouts which want their textures loaded on startup
	vector<CRunLayout*>::iterator ly = Frames.begin();
	ly++;	// already loaded 1st layout

	for ( ; ly != Frames.end(); ++ly) {
		if ((*ly)->texture_loading == CRunLayout::tl_load_on_app_start)
			(*ly)->LoadLayoutTextures();
		else if ((*ly)->texture_loading == CRunLayout::tl_use_app_setting && texture_loading == tl_load_on_app_start)
			(*ly)->LoadLayoutTextures();
	}

	// Directories exist etc. by now.
	completedInitialisation = true;

	// Initial frame mouse coord
	POINT mouse;
	GetCursorPos(&mouse);

	if (!fullscreen)
		ScreenToClient(hWnds.front(), &mouse);

	pFirstFrame->mouseX = mouse.x;
	pFirstFrame->mouseY = mouse.y;

	// Create initial frame objects (generatevent works)
	pFirstFrame->CreateInitialObjects();

	FlushDelayedObjects();

	system.changeResWidth = winWidth;
	system.changeResHeight = winHeight;

	// Start of Layout triggered only if not previewing another layout
	if (previewLayout <= 0)
		GenerateEvent(-1, SYSTEM_STARTOFFRAME, NULL);

#ifdef CONSTRUCT_DEBUGGER
	pDebug->ShowWindow(SW_SHOW);
	pDebug->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	pDebug->ModifyStyleEx(0, WS_EX_TOPMOST);
#endif

	// Previewing single layout?
	if (previewLayout > 0)		// if == 0, no need to jump anywhere, already on layout 1
		system.DoFrameChange(previewLayout, "none", 0);

	return true;
}

// Shut down runtime
void CRuntime::Close()
{
	int i;

	// Free all objects
	ObjTypeIterator t = objects.begin();
	ObjTypeConstIterator objects_end = objects.end();

	for ( ; t != objects_end; t++) {

		// Virtual types do not need freeing, they don't 'own' their instances
		if ((*t)->teamID > -1) {
			delete (*t);
			continue;
		}

		(*t)->DeleteInstances();

		// CreateParams only deletable in movements
		// (normal objects point it to a vector initialObject)
		if ((*t)->movement && (*t)->createParams != NULL) {
			(*t)->createParams->Free();
			delete (*t)->createParams;
			(*t)->createParams = NULL;
		}

		delete (*t);
	}

	objects.clear();

#ifdef CONSTRUCT_DIRECTX9

	// Clear DirectX fonts
	/*
	map<int, CD3DDisplayFont>::iterator f = Fonts.begin();

	for ( ; f != Fonts.end(); f++)
		f->second.Destroy();

	Fonts.clear();
	*/

	// Direct-x cleanup only

	// Clear effects
	for (i = 0; i < effects.size(); i++) {
		effects[i]->pEffect->Release();
		delete effects[i];
	}
	effects.clear();

	// Free DInput
	if (dinput) {
		d_mouse->Unacquire();
		d_mouse->Release();
		d_mouse = NULL;

		d_keyboard->Unacquire();
		d_keyboard->Release();
		d_keyboard = NULL;

		dinput->Release();
		dinput = NULL;
	}
#endif

	// Free bitmaps
	map<int, HBITMAP>::iterator bm = imageBmps.begin();
	map<int, HBITMAP>::const_iterator imageBmps_end = imageBmps.end();

	for ( ; bm != imageBmps_end; bm++)
		DeleteObject(bm->second);

	// Free frames
	for (i = 0; i < Frames.size(); i++) {

		for (int j = 0; j < Frames[i]->layers.size(); j++)
			delete Frames[i]->layers[j];
		
		Frames[i]->FreeLayoutTextures(true);	// force free, application terminating
		delete Frames[i];
	}

	Frames.clear();

#ifndef APPRUNTIME

	// Release any textures left: application may still hold references to some textures
	ImageIterator imh = imagehandle_to_address.begin();
	ImageIterator end = imagehandle_to_address.end();

	for ( ; imh != end; ++imh) {
		imh->second->ReleaseVRAM(renderer);
	}

	// Free up allocated collision masks
	imh = imagehandle_to_address.begin();
	end = imagehandle_to_address.end();

	for ( ; imh != end; imh++) {
		for (i = 0; i < 8; i++)
			FreeMask(imh->second->cm);

		delete imh->second;
	}
		
#endif

	// Free plugins
	for (i = 0; i < plugins.size(); i++)
		FreeLibrary(plugins[i].module);

	// Clear all files from the temp directory
	if (completedInitialisation) {
		SHFILEOPSTRUCT delOp;
		TCHAR* fromBuf = new TCHAR[tempDir.GetLength() + 3];
		strcpy(fromBuf, CSTR(tempDir));
		fromBuf[tempDir.GetLength()] = '*';
		fromBuf[tempDir.GetLength() + 1] = '\0';
		fromBuf[tempDir.GetLength() + 2] = '\0';

		delOp.hwnd = NULL;
		delOp.wFunc = FO_DELETE;
		delOp.pFrom = fromBuf;
		delOp.pTo = NULL;
		delOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
		delOp.fAnyOperationsAborted = FALSE;
		delOp.lpszProgressTitle     = NULL;
		delOp.hNameMappings         = NULL;
		SHFileOperation(&delOp);
		_rmdir(CSTR(tempDir));

		delete[] fromBuf;
	}

#ifdef CONSTRUCT_DIRECTX9

	ReleaseSwapChains();

	// Free any runtime allocated textures
	SafeFreeTexture(tempLayerTarget);
	SafeFreeTexture(tempDisplayTarget);
	SafeFreeTexture(tempSourceTarget);
	SafeFreeTexture(tempRenderTarget[0]);
	SafeFreeTexture(tempRenderTarget[1]);
	SafeFreeTexture(tempTransitionTarget);
	SafeFreeTexture(blurFloatAccumulate);

	if (multisampleTargets[0] != unallocated_texture)
		renderer.DeallocateTexture(multisampleTargets[0]);
	if (multisampleTargets[1] != unallocated_texture)
		renderer.DeallocateTexture(multisampleTargets[1]);

	// Shut down the display engine
	renderer.Close();
#endif

	// Free our windows
	for (WindowIterator w = hWnds.begin(); w != hWnds.end(); w++)
		DestroyWindow(*w);

	hWnds.clear();

}

// For detecting CPU features
DWORD GetCPUCaps()
{
	SYSTEM_INFO		si;
    DWORD			dwFeatures;

	// Assume no features are present.
	dwFeatures = 0;

	// Get the system information.
    GetSystemInfo(&si);

	// If this is at least a pentium or compatibel ...
    if ( si.dwProcessorType != PROCESSOR_INTEL_386 && 
		 si.dwProcessorType != PROCESSOR_INTEL_486)
    {
		//
		// ... get the features.
		//
        __asm
        {
			//
			// Save registers.
			//
            push	eax
            push	ebx
            push	ecx
            push	edx

			//
			// Get the features.
			//
            mov		eax, 1
			cpuid
            mov		dwFeatures, edx

			//
			// Restore registers.
			//
            pop		edx
            pop		ecx
            pop		ebx
            pop		eax
        }
    }

	// Return the features.
	return dwFeatures;
}


// Create window for runtime
bool CRuntime::MakeWindows(CRuntimeSetup* crSetup)
{
	hWnds.resize(0);

    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

    wc.lpfnWndProc = CRuntime_WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(long);
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;

	if (crSetup->screensaver)
		wc.lpszClassName = "WindowsScreenSaverClass"; 
	else
	    wc.lpszClassName = "ConstructRt";

    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(5));  //IDI_MAINICON == 5

    //Register class
    if(!RegisterClassEx(&wc))
        return false;

	DWORD style = WS_CLIPCHILDREN;
	DWORD exStyle = NULL;

	if (minimize_box)						style |= WS_MINIMIZEBOX;
	if (maximize_box)						style |= WS_MAXIMIZEBOX;
	if (resizable != resize_disabled)		style |= WS_THICKFRAME;

#ifdef CONSTRUCT_DIRECTX9
	// Fullscreen apps have to be popup to display correctly (no caption etc)
	if (fullscreen || !caption)
		style |= WS_POPUP;
	else
		style |= WS_OVERLAPPED | WS_SYSMENU;

	int windowCount = renderer.GetMultiHeadCount();

#else
	style |= WS_OVERLAPPED | WS_SYSMENU;

	int windowCount = 1;
	bool fullscreen = false;
#endif

    // Create windows
	for (int i = 0; i < windowCount; i++) {

		HWND curHwnd;
		HWND hWndParent = NULL;
		if (i > 0) hWndParent = hWnds.front();

		if (crSetup->screensaver) {
			curHwnd = CreateWindowEx (exStyle, "WindowsScreenSaverClass", "",
								style, CW_USEDEFAULT, CW_USEDEFAULT, crSetup->winWidth, crSetup->winHeight,
								hWndParent, NULL, hInstance, this);

			SetWindowLong(curHwnd, GWL_USERDATA, (long)this);
			hWnds.push_back(curHwnd);
		}
		else {
			curHwnd = CreateWindowEx (exStyle, "ConstructRt", "",
								style, CW_USEDEFAULT, CW_USEDEFAULT, crSetup->winWidth, crSetup->winHeight,
								hWndParent, NULL, hInstance, this);

			SetWindowLong(curHwnd, GWL_USERDATA, (long)this);
			hWnds.push_back(curHwnd);

		}
	}

#ifdef CONSTRUCT_DEBUGGER
//	m_Debugger.DoModal();
//	m_Debugger.ShowWindow(SW_SHOW);
#endif

	// Set the main window client area to the desired size and center it
	if (!fullscreen) {
		expectedResize = true;	// Stop device reset, SetWindowPos() calls WM_SIZE

		CWnd w;
		w.Attach(hWnds.front());
		CRect clientRec;
		clientRec.SetRect(0, 0, crSetup->winWidth, crSetup->winHeight);
		w.CalcWindowRect(&clientRec);
		w.SetWindowPos(NULL, 0, 0, clientRec.Width(), clientRec.Height(), SWP_NOMOVE | SWP_NOZORDER);

		// Now centre the window
		DEVMODE DevMode;
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode);
		int windowX = (DevMode.dmPelsWidth / 2) - (clientRec.Width() / 2);
		int windowY = (DevMode.dmPelsHeight / 2) - (clientRec.Height() / 2);
		w.SetWindowPos(NULL, windowX, windowY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		w.Detach();

		expectedResize = false;
	}

    //Success
    return true;
}

MicroAceTool::MicroAceTool(vector<AceRoutine>* pcr, vector<AceRoutine>* par, vector<EXPROUTINE>* per, vector<CString>* pn)
{
	pCndRoutines = pcr;
	pActRoutines = par;
	pExpRoutines = per;
	pExpNames = pn;
}

void MicroAceTool::RunAce(ACROUTINE f, const char* scriptName, int type, int flags, int index)
{


	// todo: move this somewhere else...i've just got it here to ensure the size is right!
	if(pActRoutines->size() == 0)
	{
		AceRoutine cr(NULL, NULL); 
		pActRoutines->assign(255, cr);
		pCndRoutines->assign(255, cr);

		pExpRoutines->assign(255, reinterpret_cast<EXPROUTINE>(NULL));
		pExpNames->assign(255, "");
	}


	// Condition
	if (type == 3) {
		AceRoutine cr(f, flags);
		if(index < 0 || index >= pCndRoutines->size())
			pCndRoutines->push_back(cr);
		else
			(*pCndRoutines)[index] = cr;
			
		return;
	}
	// Action
	if (type == 4) {
		AceRoutine ar(f, flags);
		if(index < 0 || index >= pActRoutines->size())
			pActRoutines->push_back(ar);
		else
			(*pActRoutines)[index] = ar;

		return;
	}
	// Expression
	if (type == 5) {

		AceRoutine ar(f, flags);

		if(index < 0 || index >= pExpRoutines->size())
		{
			pExpRoutines->push_back(reinterpret_cast<EXPROUTINE>(f));
			pExpNames->push_back(scriptName);
		}
		else
		{
			(*pExpRoutines)[index] = reinterpret_cast<EXPROUTINE>(f);	
			(*pExpNames)[index] = scriptName;
		}



		return;
	}
}

void MicroAceTool::EditAce(const char* aceName, const char* aceCategory, const char* aceDisplay, const char* scriptName, short flags, int type, int index, int reserved)
{
}

void MicroAceTool::Param(int type, const char* name, const char* desc, const char* init)
{
}



// EXE runtime-only code
#ifdef CONSTRUCT_RUNTIME

void CRuntime::ExtractDependency(LPCTSTR lpszType, LPCTSTR lpszName, WORD langID)
{
	CString msg;
	/*
	msg.Format("Type: %s\nName: %s", lpszType, lpszName);
	MessageBox(NULL, msg, "Dependency", NULL);
	*/

	// Create the file for writing
	FILE* f = fopen(tempDir + lpszName, "wb");

	if (f == NULL) {
		msg.Format("Error unpacking dependencies (1): could not create file %s (%d)", tempDir + lpszName, errno);
		throw runtime_error((const char*)msg);
	}

	// Get the resource data
	HRSRC resLoad = FindResource(NULL, lpszName, lpszType);

	if (resLoad == NULL) {
		int e = GetLastError();
		msg.Format("Error unpacking dependencies (2): %d", e);

		throw runtime_error((const char*)msg);
	}

	HGLOBAL resData = LoadResource(NULL, resLoad);
	LPCSTR depData = (LPCSTR)LockResource(resData);

	DWORD resSize = SizeofResource(NULL, resLoad);

	// Write the resource data to disk
	fwrite(depData, 1, resSize, f);
	fclose(f);
	FreeResource(resData);
}

/*
BOOL CALLBACK EnumResLangProc(HMODULE hModule, LPCTSTR lpszType, LPCTSTR lpszName, WORD wIDLanguage, LONG_PTR lParam)
{
	CRuntime* pRuntime = (CRuntime*)lParam;
	pRuntime->ExtractDependency(lpszType, lpszName, wIDLanguage);
	return TRUE;
}
*/

BOOL CALLBACK EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam)
{
	CRuntime* pRuntime = (CRuntime*)lParam;
	pRuntime->ExtractDependency(lpszType, lpszName, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
	return TRUE;
}

void CRuntime::UnpackDependencies()
{
	EnumResourceNames(NULL, "DEPENDENCIES", EnumResNameProc, (LONG_PTR)this);
}

////////////////////////////
// Unpack plugins to 0.csx, 1.csx etc.
bool CRuntime::UnpackPlugins(int startresource)
{
	// Store the old current directory, but look for DLLs for plugin loading etc in the temp directory
	char oldCurrentDirectory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, oldCurrentDirectory);
	SetCurrentDirectory(tempDir);

	// Get number of plugins
	char strbuf[256];

	if (LoadString(GetModuleHandle(NULL), IDS_numPlugins, strbuf, 256) == 0)
		throw runtime_error("Error unpacking resources (1)");

	numPlugins = atoi(strbuf);

	// If over 1000 plugins or negative, probably undefined string - show error
	if (numPlugins < 0 || numPlugins > 1000)
		throw runtime_error("Error unpacking resources (2)");

	// Loop all plugin resources
	for (int i = 0; i < numPlugins; i++) 
	{
		CString curFile;
		curFile.Format("%s%d.csx", tempDir, i);

		// Create the file for writing
		FILE* f = fopen(CSTR(curFile), "wb");

		if (f == NULL) {
			CString msg;
			msg.Format("Error unpacking resources (3): could not create file %s (%d)", curFile, errno);
			throw runtime_error((const char*)msg);
		}

		// Get the resource data
		HRSRC resLoad = FindResource(NULL, MAKEINTRESOURCE(startresource + i), "DLLBLOCK");

		int e = GetLastError();
		CString s; s.Format("Error unpacking resources (4): %d", e);

		if (resLoad == NULL) throw runtime_error((const char*)s);

		HGLOBAL resData = LoadResource(NULL, resLoad);
		LPCSTR dllData = (LPCSTR)LockResource(resData);

		// Write the resource data to disk
		fwrite(dllData, 1, SizeofResource(NULL, resLoad), f);
		fclose(f);
		FreeResource(resData);

		// Load up this module
		PluginModule plugin;
		plugin.module = LoadLibrary(CSTR(curFile));

		if (plugin.module == NULL) {
			CString msg;
			msg.Format("Failed to load plugin %s (%d)", curFile, GetLastError());
			throw runtime_error((const char*)msg);
		}

		//DWORD e = GetLastError();

		// Get proc addresses
		plugin.RTCreateObject = (LPRTCREATEOBJECT)GetProcAddress(plugin.module, "RTCreateObject");
		plugin.RTDestroyObject = (LPRTDESTROYOBJECT)GetProcAddress(plugin.module, "RTDestroyObject");
		plugin.RTWindowProc = (PLUGINWNDPROC)GetProcAddress(plugin.module, "RTWindowProc");

		// Initialise
		LPRTDLLLOAD RTDllLoad = (LPRTDLLLOAD)GetProcAddress(plugin.module, "RTDllLoad");

		// Handle vectors
		PluginModuleVectors emv;
		pluginvecs.push_back(emv);
		PluginModuleVectors* pv = &(pluginvecs.back());

		MicroAceTool mat(&(pv->CndRoutines), &(pv->ActRoutines), &(pv->ExpRoutines), &(pv->ExpNames));
	
		// Load vectors
		plugin.ideFlags = RTDllLoad(&mat);

		// Get the plugin routine tables
		plugin.pvCndRoutines = &(pv->CndRoutines);
		plugin.pvActRoutines = &(pv->ActRoutines);
		plugin.pvExpRoutines = &(pv->ExpRoutines);
		plugin.pvExpNames = &(pv->ExpNames);

		// Make all expression names lowercase
		vector<CString>::iterator j;
		for (j = plugin.pvExpNames->begin(); j != plugin.pvExpNames->end(); j++)
			j->MakeLower();
		
		// Add to plugins list
		plugins.push_back(plugin);
	}

	// Restore current directory
	SetCurrentDirectory(oldCurrentDirectory);

	return true;
}

#else

// Nothing done in preview runtime
void CRuntime::UnpackDependencies()
{
}

#endif // CONSTRUCT_RUNTIME