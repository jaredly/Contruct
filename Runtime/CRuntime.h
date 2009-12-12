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

#pragma once

/////////////////////////////
// DEFINITIONS AND PROTOTYPES
LRESULT CALLBACK CRuntime_WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

inline bool AsyncKeyDown(int vk)
{
	return (GetAsyncKeyState(vk) >> 15) != 0;
}

#define min4(a, b, c, d) (min(min((a), (b)), min((c), (d))))
#define max4(a, b, c, d) (max(max((a), (b)), max((c), (d))))

// Dont care about 255 char truncations
#pragma warning(disable: 4786)

typedef ParamIndexList* Trigger;
typedef ParamList* FastTrigger;

#ifdef CONSTRUCT_DIRECTX9
typedef vector<IDirect3DSurface9*> BackBufferList;
typedef vector<IDirect3DSurface9*> SurfaceList;
typedef vector<IDirect3DSurface9*>::iterator BackBufferIterator;
typedef vector<IDirect3DSurface9*>::iterator SurfaceIterator;
typedef vector<IDirect3DSwapChain9*> SwapChainList;
typedef vector<IDirect3DSwapChain9*>::iterator SwapChainIterator;
#endif

typedef vector<HWND>				WindowList;
typedef vector<HWND>::iterator		WindowIterator;

// Debugger defines
#ifdef CONSTRUCT_DEBUGGER

enum DebuggerCodeSection {
	DCS_ACTION,			// Inside CSX action routine
	DCS_CONDITION,		// Inside CSX condition routine
	DCS_EXPRESSION,		// Inside CSX expression routine
	DCS_PLUGIN,			// Any other CSX routine
	DCS_RUNTIME			// Any runtime routine
};

#define D_ENTER(sec, obj)				pRuntime->dcs = sec; pRuntime->curObject = obj
#define D_RUNTIME()						pRuntime->dcs = DCS_RUNTIME
#define _D_ENTER(sec, obj)				dcs = sec; curObject = obj
#define _D_RUNTIME()					dcs = DCS_RUNTIME

#define D_START_ACTIONS()				g_actionIndex = 1
#define D_NEXT_ACTION()					g_actionIndex++
#define D_END_ACTIONS()					g_actionIndex = -1

// Non-debugger defines: eliminate code
#else

#define D_ENTER(sec, obj)
#define D_RUNTIME()
#define _D_ENTER(sec, obj)
#define _D_RUNTIME()

#define D_START_ACTIONS()
#define D_NEXT_ACTION()
#define D_END_ACTIONS()

#endif

#define OF_NODRAW				0x0002		// Plugin is not drawn at edittime
#define OF_NOCOMMONDEBUG		0x0080		// Don't show 'Common' field (X/Y/W/H/Angle/Opacity) in debugger
#define OF_NODEBUG				0x0100		// Don't show at all in debugger
#define OF_SINGLEDRAWOP			0x0800		// Single draw operation

/////////////////////////////
// CRUNTIMESETUP
// Setup parameters for CRuntime initialisation.
struct CRuntimeSetup {
	HINSTANCE			hInstance;
	int					winWidth;
	int					winHeight;
	float				eyeDistance;
	bool				screensaver;

#ifdef CONSTRUCT_DIRECTX9
	cr::d3d9_init_params	display_params;
#endif
};

// CPU features
DWORD GetCPUCaps();
#define CPU_FEATURE_MMX		0x00800000
#define CPU_FEATURE_SSE		0x02000000
#define CPU_FEATURE_SSE2	0x04000000
#define CPU_FEATURE_3DNOW	0x80000000


HGLOBAL OpenResourceBinary(int id, const char* type, BYTE*& pData, int& size);

enum SimulatedShader {
	SS_NOSIM,
	SS_PS14,
	SS_PS11,
	SS_PS00,
};

// A key control
typedef pair<CString, int> RunControl;		// Name, player

// Class stored for each control
class RunControlInfo{
public:
	RunControlInfo(int key)
	{
		vk = key; 
		state = 0;
	} 
	RunControlInfo& operator = (const RunControlInfo& o)
	{	
		vk = o.vk; 
		state = o.state;
		return *this;
	}; 
	RunControlInfo(){
		vk = 0;
		state = 0;
	}
	int vk;
	float state;
};

typedef map<RunControl, RunControlInfo> ControlsMap;

//////////////////////////////////////////////
// PLUGIN DEFINITIONS

// Windowproc proto
LRESULT CALLBACK CRuntime_WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

// Plugin windowproc info
struct PluginWindowInfo {
	HWND hWnd;
	CRunObject* pObject;

	inline bool operator==(const PluginWindowInfo& ewi) {
		return hWnd == ewi.hWnd && pObject == ewi.pObject;
	}
};

struct PluginModuleVectors {
	vector<AceRoutine>			CndRoutines;		// Condition routines
	vector<AceRoutine>			ActRoutines;		// Action routines
	vector<EXPROUTINE>			ExpRoutines;		// Expression routines
	vector<CString>				ExpNames;			// Expression names
};

/////////////////////////////
// CRUNTIME CLASS
// The Runtime class
class CRuntime : public VRuntime {
public:
	CRuntime(bool& success);
	~CRuntime();

	/////////////////////////////
	// Runtime functions
	// Initialise runtime
	bool		Initialise(CRuntimeSetup* crSetup);

	// CAP file reader
	CCapReader	CapReader;

	// App properties
	AppProperties	props;

	// Menu resources
	vector<HMENU>	menus;

	// Unpack and load plugins (IDR_DLL0 etc)
	bool		UnpackPlugins(int startresource);
	void		UnpackDependencies();
	void		ExtractDependency(LPCTSTR lpszType, LPCTSTR lpszName, WORD langID);

	// Run the application
	void		Run();

	// Called by Run to render a frame, main body split in to two sub-functions
	bool		Frame(bool present = true);
	void			PreFrame(bool present);
	void			PostFrame(bool present);
	int			lastTimer, lastFrameCount;
	bool		firstFrame;
	bool		expectedResize;
	__int64		timerFreq;
	__int64		startTime;
	__int64		oldTime;
	double		presentTime;
	double		lastSecondPresentTime;
	MSG			msg;
	bool		backBufferRendered;

	int			previewLayout;			// If > -1, jump to that layout immediately (for IDE preview layout)
	UINT		multisamples;
	
	// Run the event list
	void		RunEventList();

	// End, free and shut down all CRuntime resources
	void		Close();

	// The System Object
	class SystemObject	system;

	// Debugger functions
	void		Restart();
	void		Break(bool bResume = false);
	void		Step();

	// Saving and loading runtime state
	void		Save(bin& ar);
	void		Load(bin& ar);
	void		LinkContainersTogether(ObjList& created);		// For after creating layouts or loading

	void		FlushDelayedObjects();
	void		FlushDeathRow();
	void		HandleBuiltinAttributes();

	/////////////////////////////
	// Runtime capabilities

	// Display engine (DX9)
#ifndef APPRUNTIME

	// Old display engine
	//CDisplay Display;

	// New Construct Renderer (CR) engine
	Renderer renderer;
	CRenderLayer render_layer;						// SDK interface to renderer

	ImageMap	imagehandle_to_address;				// For converting handles to ImageHandleInfos

	DWORD		textQuality;

#ifndef CONSTRUCT_SDL

	// Effects
	vector<CRunEffect*> effects;

	// Temporary render targets
	TextureHandle	tempLayerTarget;		// Layers with effects draw to here.
	TextureHandle	tempDisplayTarget;		// Display renders here for effects to read from.
	TextureHandle	tempSourceTarget;		// Objects Draw() to this so the original texture is accessible to HLSL
	TextureHandle	tempRenderTarget[2];	// HLSL effects blit between these two targets.
	TextureHandle	tempTransitionTarget;	// Transitions draw secondary frame to here

	TextureHandle	multisampleTargets[2];	// Targets for multisampling: 0 = for layers, 1 = for display
	//TextureHandle nonmultisample_displaytarget;

	// DirectInput
	LPDIRECTINPUT8         dinput;
	LPDIRECTINPUTDEVICE8   d_keyboard;
	LPDIRECTINPUTDEVICE8   d_mouse;

#endif
#endif

	// Animations list and the map for mapping animation handles to an index to the array
	Animations animationBank;
	//map<int, int> animationMap;
	map<int, CRunAnimation*> animationPtrs;

	void UpdateKnownAnimationFrame(CRunAnimation* pAnim, CRunObject* obj);

	// Collision functions (Collisions.cpp)
	void GenerateShiftedMasks(CollisionMask& cm);
	void GenerateCollisionMaskFromTexture(CRunObject* pObj, TextureHandle th);

	bool UpdateObjectSizeForFrameChange(CRunObject* obj, int oFrame, Animation oAnim, int nFrame, Animation nAnim);

	// Imagelist as BMPs.  This is the replacement for imageMap in the app runtime, but objects calling
	// GetImageHBITMAP in the game runtimes will fill in an entry here on a request by request basis.
	map<int, HBITMAP>			imageBmps;
	HBITMAP		GetImageHBITMAP(int imgHandle);

	// Coordinate translation
	DisplayTranslateMode translationMode;

	// Coordinate conversion
	template<class T>
	void		Translate(T& x, T& y);
	void		Translate(point& p);
	void		Translate(point3d& p);
	rect		TranslateRect(const rect& r);
	template<class T>
	void		TranslateBox(T& x1, T& y1, T& x2, T& y2);
 
	// Handle a message for CRuntime
	LRESULT		WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	// Window dragging stuff
	int mouseXOffset;
	int mouseYOffset;
	bool dragging;

	// Create an object
	CRunObject*	CreateRunObject(initialObject* et, int layer, CRunLayout* pLayout, bool createSiblings = true, CRunObject* linkTo = NULL);
	void		DestroyRunObject(class CRunObject* pRunObj, bool destroySiblings = true);

	// Generate an event
	void		GenerateEvent(int oid, int cndID, CRunObject* pCaller);
	void		GenerateEventInline(int cndID, CRunObject* pCaller);
	void		GenerateEventFast(FASTTRIGGER ft);
	FASTTRIGGER	GetFastTriggerHandle(int oid, int cndID, ExpReturn* cndParamList);

	// Convert oid to LPOBJTYPE
	CRunObjType*	pOid(int oid);

	CRunObjType*	GetTypeFromName(const char* name) const;

	void		ReEvaluateCndParameter(int index, ExpReturn* ret);
	void		ReEvaluateCndParameterWithInstance(int index, ExpReturn* ret, CRunObject* o);

	// COLLISION QUERIES
	bool		QuerySelectCollision(CRunObject* a, CRunObject* b, CRunObjType* b_type);
	bool		QuerySelectCollision(CRunObject* obj, CRunObjType* type);
	bool		QueryCollision(CRunObject* a, CRunObject* b);
	bool		QueryCollision(CRunObject* obj, CRunObjType* type);
	bool		QueryPointCollision(int x, int y, CRunObject* obj);
	bool		QueryPointCollision(int x, int y, CRunObjType* type);
	bool		QuerySelectPointCollision(int x, int y, CRunObject* obj, CRunObjType* pType);
	bool		QuerySelectPointCollision(int x, int y, CRunObjType* type);

	// Wrappers assuming type = 
	bool		QuerySelectCollision(CRunObject* a, CRunObject* b)
	{			return QuerySelectCollision(a, b, b->pType);						}

	bool		QuerySelectPointCollision(int x, int y, CRunObject* obj)
	{			return QuerySelectPointCollision(x, y, obj, obj->pType);					}

	bool QueryScreenPointCollision(int x, int y, CRunObjType* type);
	bool QuerySelectScreenPointCollision(int x, int y, CRunObjType* type);

	bool QueryScreenPointInstanceCollision(int x, int y, CRunObject* obj);

	// For collision workarounds
	void RegisterCollision(CRunObject* a, CRunObject* b);
	map<CRunObject*, ObjList> registeredCollisions;

	bool QueryRegisterCollision(CRunObject* obj, CRunObjType* type);
	bool SelectRegisteredCollisions(CRunObject* obj, CRunObjType* pType);

	// COLLISION ALGORITHMS
	CollisionMask&	GetActiveMask(CRunObject* obj);
	void		FreeMask(CollisionMask& m);
	void		FreeMask(CollisionMask* m);
	void		GenerateScaledMask(CRunObject* obj, CollisionMask* src);
	CollisionMask*	GetDefaultMask(CRunObject* obj);

	// Simple extraction of single bit from bitmask
	bool		IsOverlapPointInMask(int x, int y, CRunObject* obj);

	// Not supporting 32 bit collisions, Pentium 2 came out with MMX in 1997
	//bool		IsOverlapMaskInMask32(CRunObject* a, CRunObject* b);

	//DAVODAVODAVO{
	int			IsOverlapBoxInMask64MMX(CRunObject *a, CollisionMask& coll1, RECTF rect);
	//}s
	// MMX enabled 64 bit unscaled bitmask algorithm
	int			IsOverlapMaskInMask64MMX(CRunObject *a, CRunObject *b, CollisionMask& collA, CollisionMask& collB);

	// Fastest 128 bit SSE overlap function, but only works at offsets of 0,1,2,3,4,5,6,7 from 128 pixel aligns
	int			IsOverlapMaskInMask128SSE(CRunObject* a, CRunObject* b, CollisionMask& collA, CollisionMask& collB);

	float		CalculateBounceAngle(CRunObject* _This, float input_angle, CRunObjType* bounceOff);

	// SELECTION FUNCTIONS
	void		Select(CRunObject* obj, CRunObjType* pType);
	void		Select(CRunObject* obj, int oid);
	bool		IsSelected(CRunObject* obj, CRunObjType* pType);
	void		Unselect(CRunObject* obj, CRunObjType* pType);
	void		UnselectAll(CRunObject* obj);
	void		UnselectAll(CRunObjType* type);
	void		SelectAll(CRunObjType* type);
	void		SpawnSelect(CRunObject* parent, CRunObject* sibling);

	void		Select(CRunObject* obj)
	{			Select(obj, obj->pType);		}

	bool		IsSelected(CRunObject* obj)
	{			return IsSelected(obj, obj->pType);	}
	
	void		Unselect(CRunObject* obj)
	{			Unselect(obj, obj->pType);		}

	void		NewSOL();
	void		NewSOLCopy();
	void		NewSOLCopy(ObjTypeList* modifiers);
	void		RestoreSOL();
	void		RestoreSOL(ObjTypeList* modifiers);

	bool		RunConditions(CNDS_PTR pCnds);
	bool		CndsBefore(CNDS_PTR pCnds) const;
	bool		CndsAfter(CNDS_PTR pCnds) const;

	bool		IterValid(long a, long b) { return false; }
	void		IterIncrement(long a) {}

	void		ScreenToLayout(const CRunLayout* pLayout, float& x, float& y) const;
	void		ScreenToLayer(const CRunLayout* pLayout, int layer, float& x, float& y) const;

	int GetPrivateVariableCount(CRunObject* obj);
	void GetPrivateVariableInfo(CRunObject* obj, int index, int& type, const char*& str);

	CRunLayout* CreateRunningFrame(int frameNum, HWND hWnd = NULL);
	void DrawRunningFrame(CRunLayout* pLayout);
	void DestroyRunningFrame(CRunLayout* pLayout);
	LRESULT SendMessageToPlugins(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

	// For SerializeObjectPtr
	struct DelayedSerializeObjectPtr {
		CRunObject** obj;
		int uid;
	};

	vector<DelayedSerializeObjectPtr> delayedSerializeObjects;

	CollisionMask* CreateCollisionMask(int width, int height);
	void ReleaseCollisionMask(CollisionMask* mask);
	CollisionMask* GetCollisionMaskFromImage(int image);
	void AddMaskToMask(CollisionMask* src, CollisionMask* dest, int x, int y);
	void ExportCollisionToFile(CollisionMask* mask, int index);

	list<CollisionMask> collisionmasks; //used for SDK calls to CreateCollisionMask

	void DrawObject(CRunObject* obj);

	// VRuntime implementations
#include "VRun_imps.hpp"

	/////////////////////////////
	// Useful public variables

	// CPU features (nonzero if supported)
	int			supportMMX;
	int			supportSSE;
	int			supportSSE2;

	int			winWidth;			// Window width
	int			winHeight;			// Window height
	float		eyeDistance;
	int			winWidthOffset;		// For fullscreen
	int			winHeightOffset;
	int			realWinWidth;		// Window client width
	int			realWinHeight;		// Window client height

	bool		caption;			// Window caption
	bool		minimize_box;		// Minimize box
	bool		maximize_box;
	enum ResizeSetting {
		resize_disabled,
		resize_showmore,
		resize_stretch
	}			resizable;

	__int64		frameCount;			// Number of frames elapsed
	bool		bRunning;			// App running?  Set to false to end app
	int			fps;				// FPS rate
	int			userFps;			// Desired FPS rate if set
	cr::framerate_mode fpsMode;
	int			fpsInCaption;
	int			totalObjects;		// Total number of instances created
	int			batchesPerPresent;

	int			nullPtr;			// A zero value.  Referenced to by ExpParts with no L/R operands
	bool		disableAlt;

	COLORREF	bkColor;			// Window back colour (app runtime)

	SimpleInputState inputState;	// Maintained by runtime for exts to use
	void UpdateInputState();
	void CreateTempDirectory();		// Creates temp folder in local settings for csx/python data/etc

	MTRand53	 mtRand53;			// Mersenne Twister float [0, 1) generator

	// Current event ptr
	CEventLine* pCurrentEvent;

	// Names of all event sheets
	vector<CString> eventSheetNames;

	friend class SystemObject;
	friend class CCapReader;
	friend class CRunLayout;
	friend class CEvent;
	friend class CEventParent;
	friend class CEventGroup;
	friend class CExpression;
	friend class ExpDot;
	friend class ExpIdent;

	/////////////////////////////
	// Private vars
private:

	/////////////////////////////
	// The Object Types list (including qualifiers)
	vector<CRunObjType*>	objects;
	bool delayedObjectsExist;		// Any of the object types delayed object lists are not empty
	bool isTopLevelTrigger;			// True if a top-level trigger not called by any other eventd
									// (so we can tell if we should flush delayed objects after the trigger)

	/////////////////////////////
	// The Frames list
	vector<CRunLayout*>		Frames;
	list<CRunLayout*>		runningFrames;	// List of all running, parallel frames
#ifdef CONSTRUCT_DIRECTX9
	list<CRunTransition>	Transitions;	// Transitions
	CRunTransition* runningTransition;

	// Motion blur effect
	bool					motionBlur;
	bool					turnOffMotionBlur;		// To prevent glitching turning off mid-motion-blur-render
	bool					turnOnMotionBlur;
	int						blurNumSteps;
	TextureHandle			blurFloatAccumulate;	// Handle to accumulation float buffer
	void					InitMotionBlur();

	// For faking timers over temporal antialiasing
	clock_t					clockTemporalOffset;

	// Mouse coords interpolated linearly in screen space 
	float					linearMouseX;
	float					linearMouseY;

	// Device reset handling
	void					OnLostDevice();
	void					OnResetDevice();
	bool					deviceAlreadyLost;
	void					DrawBorderForSmallLayouts();
#endif

	vector<CRunLayout*>		wndFramesToDraw;	// List of frames that have a HWND and need to draw/present to it later

	/////////////////////////////
	// The Plugin Modules list
	list<PluginModuleVectors>	pluginvecs;
	vector<PluginModule>		plugins;

	/////////////////////////////
	// The Plugin Registered Windows list
public:	// Accessible via windowproc
	list<PluginWindowInfo>		pluginWnds;

	bool fullscreen;

private:
	// and plugins requesting PreTranslateMessage
	list<CRunObject*>		pluginPreMsgHooks;

	// Global values
	vector<ExpStore>		globalVars;
	vector<CString>			globalNames;	// strings of global var names

	// Objects that want calling for OnFrame and OnFrame2
	ObjList		onFrameCallees;
	ObjList		onFrame2Callees;

	// For protecting onFrameCallees vectors from being changed during iteration
	bool		callingOnFrames;
	bool		callingOnFrame2s;
	ObjList		calleeBuffer;		// To add when calls complete

	// Where objects go to die...
	ObjList DeathRow;
	vector<CRunLayout*>		FrameDeathRow;

	// Controls
	ControlsMap				keyControls;

	void ProcessControlState(); // Sets the control states to the key value

	/////////////////////////////
	// List of CD3D fonts, for DX9 runtime
#ifndef APPRUNTIME
	//map<int, CDisplayFont> Fonts;
	//int nextFontID;

#ifndef CONSTRUCT_SDL
	IDirect3DSwapChain9* pMainSwapChain;
	IDirect3DSurface9* pMainBackBuffer;

	// Todo: transfer above single-screen code to multi-monitor code using lists below;
	// use separate device windows to allow CreateDevice to succeed
	SwapChainList swapChains;
	BackBufferList backBuffers;

	void GetSwapChains();
	void GetBackBuffers();
	void ReleaseSwapChains();
	void ReleaseBackBuffers();
#endif
#endif

	// Link group names to a group ptr
	map<CString, vector<CEventGroup*> > groupMap;

	// Add a trigger to the Triggers map for optimised iteration
	CEventLine**		AddTriggerToMap(CRunLayout* pLayout, CEventLine* ev);

	// Current frame
	int			curFrame;
	int			changeFrame;	// > -1 if indicating a frame change is to happen at the end of event loop

	// General runtime vars
public:
	//HWND hWnd;
	WindowList	hWnds;
	HINSTANCE	hInstance;		// App hInstance
	bool		MakeWindows(CRuntimeSetup* crSetup);		// Create a window for runtime
	TextureHandle CreateDisplayTargetTexture();
	void		ReallocDisplayTargetTexture(TextureHandle& tex);
	void		SafeFreeTexture(TextureHandle& tex);

	clock_t		clockOffset;	// Timer offset at start of app (load time)
	int			curUid;			// current UID assignment
	int			numPlugins;		// Number of plugin extracted
	CString		tempDir;		// Temporary directory
	CString		pythonDir;		// Temporary directory for python
	CString		pythonZipPath;	// Location of PythonLibs.zip
	CString		capFile;		// CAP file for previews to give right AppPath

	CRunObject* GetObjectFromUID(int uid);

	// True when data extracted to temp directory (for cleanup)
	bool		completedInitialisation;

	bool		systemFrameBlocked;
	bool		disableWindowsKey;

	// Get system frame created by runtime
	CRunLayout* GetSystemFrame() const;

	CRunAnimation*	GetSubAnimation(CRunAnimation* pAnim, int index);
	int	GetSubAnimationCount(CRunAnimation* pAnim);

	virtual CRunAnimation*	GetSubAngleAnimation(CRunAnimation* pAnim, int index);
	virtual int	GetSubAngleAnimationCount(CRunAnimation* pAnim);

	// For plugin key access
	map<CString, void*> pluginKeys;
	map<CString, CString> pluginKeysStrings;

	// For time delta, for movements
	double timedelta;
	double timer;					// hi-res timer, cumulative timedelta
	double timerC, timerY, timerT;	// rounding error correction in timer
	SimulatedShader simShader;
	bool overrideTimeDelta;
	float overrideTimeDeltaFPS;
	double		timeScale;		// For slow-motion etc
	double		minimum_fps;

	// Texture loading policy
	enum TextureLoading {
		tl_load_on_app_start,
		tl_load_per_layout
	} texture_loading;

	//void DebugTempFunction();

	// Debugging runtime data
#ifdef CONSTRUCT_DEBUGGER
public:
	DebuggerCodeSection dcs;
	CRunObject* curObject;

	// Debugger pointer
	class CDebugger* pDebug;
	class CObjectsPage* pObjectPage;
	class CLogPage* pLogPage;
	class CWatchPage* pWatchPage;
	vector<DebuggerInspectionLine>* curDebugOutput;

	bool isSuspended;

	void SetSuspended(bool suspended = true) {
		isSuspended = suspended;
	}

	// Opposite of deathRow - list of objects created this tick.
	vector<CRunObject*> creationRow;

	friend class CDebugger;
	friend class CObjectsPage;
	friend class CWatchPage;

	void LogMsg(CString msg);
#endif

	int solIndex; //used by pairing

	bool suppress_else_event;

};