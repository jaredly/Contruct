// RUNTIME STRUCTS & DATA
#ifndef _RUNDEFS_H_
#define _RUNDEFS_H_

#include <map>
#include "..\Common\Renderers\RendererCommon.h"

typedef struct RunObjectHeaderInfo OBJHEADER;
typedef OBJHEADER* LPOHEADER;

// General typedefs
typedef vector<class CRunObject*>							ObjList;
typedef vector<class CRunObject*>::iterator					ObjIterator;
typedef vector<class CRunObject*>::const_iterator			ObjReadIterator;
typedef const vector<class CRunObject*>::const_iterator		ObjConstIterator;
typedef vector<class CRunObjType*>							ObjTypeList;
typedef vector<class CRunObjType*>::iterator				ObjTypeIterator;
typedef vector<class CRunObjType*>::const_iterator			ObjTypeReadIterator;
typedef const vector<class CRunObjType*>::const_iterator	ObjTypeConstIterator;
/*
typedef vector<struct action>								ActionList;
typedef vector<struct action>::iterator						ActionIterator;
typedef vector<struct action>::const_iterator				ActionReadIterator;
typedef const vector<struct action>::const_iterator			ActionConstIterator;
*/
typedef vector<struct condition>							ConditionList;
typedef vector<struct condition>::iterator					ConditionIterator;
typedef vector<struct condition>::const_iterator			ConditionReadIterator;
typedef const vector<struct condition>::const_iterator		ConditionConstIterator;
typedef vector<class CEventLine*>							EventList;
typedef vector<class CEventLine*>::iterator					EventIterator;
typedef vector<class CEventLine*>::const_iterator			EventReadIterator;
typedef const vector<class CEventLine*>::const_iterator		EventConstIterator;
typedef vector<ObjList>										ObjectLinksList;

typedef int Texture;
typedef int Animation;
typedef int BankImage;
typedef int DXFont;

// For making read-only elements from SDK
#ifdef CONSTRUCT_SDK
#define SDK_CONST
#else
#define SDK_CONST
#endif

#ifdef PYTHONIMPORT
struct PyFunct
{
	CString name;
	int paramCount;
};
#endif

// Effects are not usually accessible by the SDK due to DirectX usage
#ifdef CONSTRUCT_DIRECTX9

struct CRunEffectParameter {
	int type;
	CString name;
	CString varname;
	CString value;
	float fValue;
	D3DXHANDLE handle;
};

class CRunEffect {
public:
	ID3DXEffect*	pEffect;
	CString			name;

	vector<CRunEffectParameter> params;

	// Look for all standard script parameters eg ForegroundTexture
	void GetParameters();
	// Setup and initialise default values and handles for script defined parameters
	void SetupCustomParameters();
	// Clone an effect and initialise parameters & defaults
	void InitialiseFrom(class CRuntime* pRuntime, CRunEffect* pEffect);

	// For effects such as Lens which may sample pixels it is simultaneously writing to.
	bool			crossSampling;
 
	// For effects such as Warp which commonly need to sample pixels outside the bounding box.
	bool			borderMode;

	// For effects which may need the entire foreground buffer clear.  Unusual; slow performance
	bool			fullScreenMode;

	// True if anything in the chain references SourceTexture.
	bool			usesSourceTexture;

	int id;			// Effect ID, usually 0 and incrementing, but runWithoutPs may remove some entries.

	int majorVer;
	int minorVer;

	// Handles
	D3DXHANDLE vBackgroundTexture;
	D3DXHANDLE vForegroundTexture;
	D3DXHANDLE vSourceTexture;
	D3DXHANDLE vPreviousTexture;
	D3DXHANDLE vFrameCounter;
	D3DXHANDLE vBoxLeft, vBoxTop, vBoxRight, vBoxBottom;
	D3DXHANDLE vBoxWidth, vBoxHeight;
	D3DXHANDLE vHotspotX, vHotspotY;
	D3DXHANDLE vPixelWidth, vPixelHeight;
	D3DXHANDLE vBgStart, vBgEnd;

	bool renderWithRenderStates;
	DWORD SrcBlend;
	DWORD DestBlend;
};

struct InstanceEffect {
	CRunEffect* fx;
	bool enabled;

	vector<CRunEffectParameter> params;
};

typedef vector<InstanceEffect>								EffectList;
typedef vector<InstanceEffect>::iterator					EffectIterator;
typedef vector<InstanceEffect>::const_iterator				EffectReadIterator;
typedef const vector<InstanceEffect>::const_iterator		EffectConstIterator;

#endif // CONSTRUCT_DIRECTX9

#ifdef APPRUNTIME
typedef vector<void*> EffectList;
#endif

// Bitmask for holding a collision map
struct CollisionMask {
	int pitch;		// bytes
	float width;		// pixels
	float height;		// pixels
	int boundingbox; // nonzero if no masks and using bounding box colls (solid rect object). 'bits' will be junk data

	// ALIGNED data - use _aligned_malloc / _aligned_free
	BYTE* bits[8];

	unsigned int mem_size;	// size of buffers at bits[n]
};

#ifndef CONSTRUCT_SDK
class ImagePoint : public NameComparableCIS {
public:
	int x;
	int y;
};
#endif

// Image handle map struct
class ImageHandleInfo {
public:
	TextureHandle th;		// DirectX texture (unallocated_texture if not yet loaded from pngFile)
	CollisionMask cm;	// Collision bit data
	POINT hotspot;

	BYTE* pngFile;		// Pointer to system memory PNG file of image
	int pngFileSize;

#ifndef CONSTRUCT_SDK
	vector<ImagePoint> points;

	ImageHandleInfo() : th(unallocated_texture),
						pngFile(NULL),
						pngFileSize(-1)
	{
	}

	~ImageHandleInfo()
	{
		if (pngFile) {
			delete[] pngFile;
			pngFile = NULL;
		}
	}
  #ifdef CONSTRUCT_DIRECTX9
	// Move this image from SRAM to VRAM
	void TransferToVRAM(Renderer& renderer);
	void ReleaseVRAM(Renderer& renderer);
	bool IsInVRAM(Renderer& renderer);
  #endif
#endif
};

typedef std::vector<ImageHandleInfo*>				ImageHandleList;
typedef std::vector<ImageHandleInfo*>::iterator		ImageHandleIterator;

// Dynamically generated
struct ObjCollisionMask {
	CollisionMask* curMask;

	// Box and angle this collision map is generated for
	float w;
	float h;
	float angle;
};

// Collision algorithms
enum CollisionMode {
	COLLISIONMODE_NONE,			// No collisions will be detected
	COLLISIONMODE_POINT,		// Only overlaps on hotspot detected
	COLLISIONMODE_BOX,			// Bounding box overlaps
	COLLISIONMODE_FINE,			// Using bitmasks
	COLLISIONMODE_FORCE_DWORD = 0x7fffffff
};

// Translation modes
enum DisplayTranslateMode {
	TRANSLATE_SCROLL,			// Translate frame co-ords to screen co-ords
	TRANSLATE_SCREEN,			// Use frame co-ords as screen co-ords (scrolling has no effect on display position)
	TRANSLATE_ZERO,				// Always draws at (0,0)

	// (Force 32 bit size)
	TRANSLATE_FORCE_DWORD = 0x7fffffff
};

// Special typedefs
typedef map<int, ImageHandleInfo*>							ImageMap;
typedef map<int, ImageHandleInfo*>::iterator				ImageIterator;
typedef list<CRunObject*>									LayerObjList;
typedef list<CRunObject*>::iterator							LayerObjIterator;
typedef list<CRunObject*>::const_iterator					LayerObjReadIterator;
typedef const list<CRunObject*>::const_iterator				LayerObjConstIterator;


class AnimInfo
{
public:
	AnimInfo()
	{
		playing = true;
		angle = 0;
		counter = 0;
		curFrame = -1;
		repeat = -1;
		speed = 0;
		repeat_to = 0;
		pingpong = 0;
		updated = false;
		pAnim = NULL;

		autoFlip = false;
		autoMirror = false;
		autoRotations = -1;

	}
	Animation	handle;			// Animation identifier
	bool		playing;		// True if we want to increment counter
	float		angle;			// Animation angle last used
	float		counter;		// Animation play counter (counts from 0..speed)
	int			curFrame;		// Current frame

	bool		autoFlip;
	bool		autoMirror;
	int			autoRotations;
	// overwrites
	float		speed;
	int			repeat;
	int			repeat_to;
	int			pingpong;	// 0= not ping pong, 1 = forward and pingpong 2 = backward and pingpong
	bool		updated;
	class CRunAnimation* pAnim;		// Animation ptr
};

#ifndef CONSTRUCT_SDK
struct DebuggerInspectionLine {
	CString item;
	CString value;

	CRunObject* pOwner;

	bool readOnly;
	bool bold;
	bool combo;
	int comboSel;

	bool operator==(const CString& r) const {
		return item == r;
	}
};


typedef vector<DebuggerInspectionLine> ExtDebugInfo;
typedef vector<DebuggerInspectionLine>::iterator ExtDebugIterator;
typedef vector<DebuggerInspectionLine>::const_iterator ExtDebugReadIterator;
#endif

struct RunObjectInfoPlus{
	CollisionMask* collisionMaskHandle;
	CollisionMask* generatedMask;		// Created by GenerateCollisionMaskFromTexture
	float skewX;
	float skewY;

	// New filter
	cr::color filter;

	// Z co-ordinate (depth)
	float z_elevation;

#ifdef PYTHON
	PyObject* pDictionary;
#else
	void* pDictionary;
#endif
};

struct RunObjectHeaderInfo {
	// (Optional) The current DirectX texture being used by your object.
	TextureHandle		curTexture;

	// Image bank entry for this object.
	ImageHandleInfo*	imgHandle;

	// Position and dimension.  Call pRuntime->UpdateBoundingBox() on the object changed after modifying these
	float			x;				// X co-ordinate (top left point)
	float			y;				// Y co-ordinate (top left point)
	float			w;				// Width
	float			h;				// Height
	float			angle;			// Rotation angle
	float			displayangle;	// Actual displayed angle
	bool			isMirrored;
	bool			isFlipped;
	RECTF SDK_CONST	box;			// Bounding box.  Read-only from SDK, used by runtime, call pRuntime->UpdateBoundingBox

	// IDs, read only from SDK
	int	SDK_CONST	uid;			// Unique ID
	int	SDK_CONST	iid;			// Instance ID
	int				visible;		// Is visible
	bool			global;			// Survives frame changes

	AnimInfo*		curAnim;		// Current animation id

	float			HotSpotX;		// Offset from the top left point
	float			HotSpotY;		// Offset from the top left point
	float			HotSpotAngle;	// Hotspot in polar form
	float			HotSpotDist;

	RECTF*			pDisplayBoxOverride;	// Override the display bounding box
	RunObjectInfoPlus*	pInfo;

	unsigned long	filter_deprecated;			// Display filter; deprecated, use pInfo->filter
	int	SDK_CONST	z;				// Z order - use VRuntime functions to modify Z order/layer
	int	SDK_CONST	layer;			// Layer number
	int				destroying;		// If true, will be cleaned up and deleted at end of next frame

	// Points to iterator to current position in instances list of current layer
	LayerObjIterator* pLayerIterator;
	DisplayTranslateMode deprecated_translation;
	CollisionMode	  collMode;		// Collision mode
	ObjCollisionMask  dynMask;		// Collision map, if this object is using one
									// pCollMask is handled automatically by the runtime.  Only modify collisionMaskHandle

	int SDK_CONST	oid;			// Object ID

	class initialObject* editObject;	// Pointer to Initial Object this object was created from
	void (*UpdateAngle)(CRunObject* object);	// Called if non 0
	float last_angle; // Used to determine if the angle has changed
};

class CRunObject {
public:
	// Standard object data
	OBJHEADER info;

	// Pointer to the Runtime
	class VRuntime* pRuntime;

	// Pointer to the frame I am on
	class CRunLayout* pLayout;

	// Pointer to the object Type
	class CRunObjType* pType;

	// Movement SDK link between the moving and the moved
	class CRunObject* pLink;

	// Reserved for Construct's own data.
#ifdef CONSTRUCT_SDK
	char _reserved_construct[40];
#else	// CONSTRUCT_SDK
	ObjList siblings;

	// List of currently active effects
#ifdef CONSTRUCT_DIRECTX9
	EffectList effects;
#endif

	// Align for debug mode
	#ifndef CONSTRUCT_RUNTIME_DEBUG
		char _unused_padding[8];
	#endif
#endif	// CONSTRUCT_SDK

	// Pure virtuals
	virtual BOOL	OnFrame() = 0;
	virtual BOOL	OnFrame2() = 0;
	virtual void	Draw() = 0;
	virtual LRESULT	WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) = 0;
	virtual BOOL	PreTranslateMessage(MSG* pMsg) = 0;
	virtual long	ReturnDefaultValue(const class ExpReturn* theParams, class ExpReturn& ret) = 0;
	virtual long	ReturnUndefinedExpression(CString& expName, const class ExpReturn* theParams, class ExpReturn& ret) = 0;
	virtual void	OnCreate() = 0;

	// plugin communication functions
	virtual long	GetData(int id, void* param) = 0;
	virtual long	CallFunction(int id, void* param) = 0;

	// For global objects
	virtual void	OnFrameChange(int oldFrame, int newFrame) = 0;

	// For runtime save/load
	virtual void	Serialize(class bin& ar) = 0;

	// Debugger functions
	virtual void	DebuggerUpdateDisplay(class ExpStore*& pPrivateVars) = 0;
	virtual void	OnDebuggerValueChanged(const char* name, const char* value) = 0;

	// Helper function
	void UpdateBoundingBox();
	void TriggerEvent(int cndIndex);
};

typedef long (CRunObject::*ACROUTINE)(const class ExpReturn* params);
typedef long (CRunObject::*EXPROUTINE)(const class ExpReturn* params, class ExpReturn& ret);

// Tables: actions, conditions, expressions
enum TABLE
{
	CONDITION = 0,
	ACTION,
	EXPRESSION
};

// ConditionRoutine: describes a condition routine (with flags)
class AceRoutine
{
public:
	AceRoutine() {}
	AceRoutine(ACROUTINE f, short r) { routine = f; flags = r; }

	ACROUTINE routine;
	short flags;
};


class CRunLayer {
public:

#ifdef CONSTRUCT_SDK
	CRunLayer(VRuntime* _pRuntime);
	VRuntime* pRuntime;
#else
	CRunLayer(class CRuntime* _pRuntime);
	class CRuntime* pRuntime;
#endif

	bool HasEffects();

	int z;			// layer number
	int type;

	// Clear background settings
	DWORD deprecated_backcolor;

	// Polar co-ords to layer centre
	float hsDist;
	float hsAngle;

	//Layer transformations
	float angle;
	float scrollXf;
	float scrollYf;
	float zoomXf;
	float zoomYf;

	float scrollXoffset;
	float scrollYoffset;
	float zoomXoffset;
	float zoomYoffset;

	DWORD deprecated_filter;
	bool visible;

	float mouseX;
	float mouseY;

	int number;		// 0-based layer index

	CString name;

	bool forceOwnTexture;

	double opacity;
	DWORD deprecated_colorfilter;

	float currentAngle;		// angle, but 0 if the display does not have room to rotate

	cr::color backColor;
	cr::color colourfilter;
	cr::color filter;

	enum LayerSampler {
		sampler_default,		// Use application setting
		sampler_point,
		sampler_linear
	} sampler;

	bool	enable3d;			// Enable Z buffer for 3D layering

	// Not available to SDK
#ifndef CONSTRUCT_SDK

	// Objects on this layer - Z sorted
	list<CRunObject*> instances;

	void ReIndexInstanceZValues();
	bool zIndicesSorted;		// true if all objects in 'instances' are known to be sorted by info.z

	void UpdateFilter();

#ifdef CONSTRUCT_DIRECTX9
	// List of currently active effects
	EffectList effects;
#endif

#endif
};

struct ScreenCopyingObject {
	CRunObject* obj;
	TextureHandle rt;

	bool operator==(const ScreenCopyingObject& r) const {
		return obj == r.obj && rt == r.rt;
	}
};

struct D3DRECTF {
	float x1;
	float y1;
	float x2;
	float y2;
};


// CRunLayout class
// Represents a level at runtime
class CRunLayout {
public:
#ifdef CONSTRUCT_SDK
private:
	CRunLayout();
public:
	VRuntime* pRuntime;
#else
	CRunLayout(CRuntime* pRun);
	CRuntime* pRuntime;
#endif

	~CRunLayout();

	friend class CCapReader;

	void	Load();
	void	Unload();
	void	CreateInitialObjects();
	void	DestroyNonGlobalObjects();

#ifdef CONSTRUCT_DIRECTX9
	// Render from a source texture through to display stepping effects
	void	RenderEffectChain(EffectList& effects, TextureHandle sourceTexture, TextureHandle destTexture, D3DRECT& clearZone, D3DRECTF& drawZone, RECTF& tbox, float hsX, float hsY);

	// Render an OF_SINGLEDRAWOP object to display, using direct drawing to target (faster)
	//void	RenderSimpleEffect(InstanceEffect& effect, CRunObject* o, int destTexture, const RECTF& tbox, const D3DRECTF& clearZone);
#endif

#if !defined(APPRUNTIME) && !defined(CONSTRUCT_SDK)
	// Render frame
	void	Draw(bool incrementFrameCounter = true);
	void DrawRunObjectOnScreen(class CRunObject* q, const RECTF& obox, EffectList* effects = NULL);

	void	CopyScreenToObject(ScreenCopyingObject& sco);
#endif


	int number;
	int width;
	int height;
	int winWidth;
	int winHeight;
	int frameXoffset;	// X co-ordinate in window
	int frameYoffset;	// Y co-ordinate in window
	bool unboundedScrolling;

	// Background color, both values pertain to the same 
	DWORD deprecated_bg_color;
	COLORREF bg_colorref;

	// The current scroll values, adjusted for layer scroll coefficients.
	double scrollX;
	double scrollY;
	double zoomX;
	double zoomY;
	double display_angle;

	// In frame coordinates
	double mouseX, mouseY;

	double frameCounter;

	bool systemDrawn;		// Drawn by CRuntime.  CreateRunningFrame uses FALSE

	HWND hWnd;

	// Current scroll values, not adjusted for the layer's scroll coefficient.
	double originalScrollX;
	double originalScrollY;

	CString name;

	__int64 frameCounter64;

	bool modal;			// true if modal and should block the system frame
	bool selfdestruct;	// for "End modal frame" action to work with frameview

	float zoomedWindowWidth;
	float zoomedWindowHeight;
	float curXscale;
	float curYscale;
	double curAngle;

	double startClock;
	double currentClock;

	cr::color bg_color;

#ifndef CONSTRUCT_SDK

#ifdef APPRUNTIME
	void* pSwapChain;
#else
	IDirect3DSwapChain9* pSwapChain;
#endif

	vector<CRunLayer*> layers;

	RECTF viewport;
	inline const RECTF& GetObjectBox(CRunObject* obj) const;
	void CalculateZElevatedBoundingBox(CRunObject* obj) const;
	inline bool ObjectBoxOnScreen(const RECTF& obox);

	// Object type references on this frame
	ObjTypeList typeReferences;

	// List of initalObjects
	list<initialObject> initObjects;

	// List of objects wanting to copy screen
	vector<ScreenCopyingObject>		prerender_screenCopiers;		// Copy before object rendered
	vector<ScreenCopyingObject>		postrender_screenCopiers;		// Copy after object rendered

	/////////////////////////////
	// The Events for this frame
	EventList				Events;
	TriggerMap				Triggers;

	// Key data
	map<CString, void*> frameKeys;
	map<CString, CString> frameKeysStrings;

	// Current layer iterator - points to layer being drawn
	vector<CRunLayer*>::iterator y;

	void ApplyScrollBounding();

	double scrollCentreX, scrollCentreY;

	int framedWidth;	// min(winWidth, width)
	int framedHeight;

	// Provides bounding
	void SetScrollX(double sx);
	void SetScrollY(double sy);

	EffectList effectsToRun;

	ImageHandleList used_image_handles;		// Image handles referenced on this layout

	enum TextureLoading {
		tl_use_app_setting,
		tl_load_on_app_start,
		tl_load_per_layout
	} texture_loading;

	bool has_loaded_textures;				// Layout has references to its textures
	void LoadLayoutTextures();
	void FreeLayoutTextures(bool force_free = false);

#endif

};

#if !defined(CONSTRUCT_SDK) && !defined(APPRUNTIME) && defined(CONSTRUCT_DIRECTX9)

class CRunTransition {
public:
	// Frames to transition between
	CRunLayout* fromFrame;
	CRunLayout* toFrame;

	CString name;

	// Progress, 0.0 to 1.0
	float progress;
	D3DXHANDLE progressHandle;

	// The transition effect
	CRunEffect* transition;

	// Transition timeout
	int msDuration;
	int msStart;		// time of start, clock()

	CRuntime* pRuntime;
};

#endif

/////////////////////////////
// ANIMFRAME / ANIMATION
class CRunAnimationFrame {
public:
	class ImageHandleInfo*	imgHandle;		// Pointer to image bank entry
	TextureHandle th;		// Reference to display texture

	float		timeFactor;		// Time multiplier, eg. 2 means frame shows twice as long as a standard frame
	float		pivotX;			// Pivot point cartesian coords
	float		pivotY;
	float		pivotAngle;		// Pivot point polar coords
	float		pivotDist;

	// Image dimensions
	float		width;
	float		height;
};

class CRunAnimation;

typedef vector<CRunAnimationFrame> AnimationFrames;

typedef list<CRunAnimation> Animations;

class CRunAnimation {
public:
	Animation	handle;			// Animation identifier
	CRunAnimation*	parent;
	float		speed;			// Animation play speed

	int		repeat;			// Start the animation again when it finishes
	bool	pingpong;
	int		repeat_to;			// index to repeat to
	float	angle;				// Angle that this animation represents (right = 0, down = 90...)
	bool    is_angle;

	CString		name;			// Name of animation
	AnimationFrames frames;	// Animation frames
#ifndef CONSTRUCT_SDK
	Animations subAnims;		// Sub-animations
	Animations Angles;
	int tagID;				// Used for behaviors to know which animations are for stopped, jumping etc

	typedef std::vector<ImageHandleInfo*> ImageHandleList;
	ImageHandleList referenced_images;

	void GetReferencedImages(ImageHandleList& il);
	void Load(Renderer& renderer);
	void Unload(Renderer& renderer);
	void UpdateFrameInfo();
#endif
};

typedef list<CRunAnimation>::iterator AnimationIterator;

// For abstracting STL usage in ACE code
class MicroAceTime {
public:
	virtual void RunAce(ACROUTINE f, const char* scriptName, int type, int flags, int index = -1) = 0;
	virtual void Param(int type, const char* name, const char* desc, const char* def) = 0;
	virtual void EditAce(const char* aceName, const char* aceCategory, const char* aceDisplay, const char* scriptName, short flags, int type, int index = -1, int reserved = 0) = 0;
};

class MicroAceTool : public MicroAceTime {
public:
	MicroAceTool(vector<AceRoutine>* pcr, vector<AceRoutine>* par, vector<EXPROUTINE>* per, vector<CString>* pn);
	
	void RunAce(ACROUTINE f, const char* scriptName, int type, int flags, int index = -1);
	void Param(int type, const char* name, const char* desc, const char* def);
	void EditAce(const char* aceName, const char* aceCategory, const char* aceDisplay, const char* scriptName, short flags, int type, int index = -1, int reserved = 0);

	vector<AceRoutine>*	pCndRoutines;
	vector<AceRoutine>*			pActRoutines;
	vector<EXPROUTINE>*			pExpRoutines;
	vector<CString>*			pExpNames;
};

// Plugin runtime routine pointer typedefs
typedef class CRunObject* (WINAPI * LPRTCREATEOBJECT)(class initialObject* editObject, class VRuntime* pRuntime);
typedef void (WINAPI * LPRTDESTROYOBJECT)(CRunObject* pObject);
//typedef vector<ACESEntry>* (WINAPI * LPGETTABLE)(TABLE table);
//typedef long (WINAPI * LPGETROUTINES)(TABLE table);
typedef int (WINAPI * LPRTDLLLOAD)(MicroAceTime* at);
typedef long FASTTRIGGER;
typedef LRESULT (CALLBACK* PLUGINWNDPROC)(HWND, UINT, WPARAM, LPARAM, CRunObject* object);

// PluginModule: Represents a .csx file loaded
struct PluginModule {
	HMODULE module;							// HMODULE to the .csx (dll) file

	LPRTCREATEOBJECT	RTCreateObject;
	LPRTDESTROYOBJECT	RTDestroyObject;

	PLUGINWNDPROC			RTWindowProc;

	vector<AceRoutine>* pvCndRoutines;// Condition routines
	vector<AceRoutine>* pvActRoutines;		// Action routines
	vector<EXPROUTINE>* pvExpRoutines;		// Expression routines
	vector<CString>*	pvExpNames;			// Expression names

	CString				previewName;		// CSX filename if in preview mode

	int ideFlags;

#ifdef PYTHONIMPORT
	vector<PyFunct> py_actions;
	vector<PyFunct> py_conditions;
	vector<PyFunct> py_expressions;
#endif


};

class SelObjList {
public:
	ObjList sol;		// Selected object list
	int selectAll;		// Nonzero if all-selected flag on

	SelObjList() : selectAll(1)
	{
		sol.reserve(50);	// All SOLs reserve some memory to prevent reallocation errors.
	}
};

typedef list<class SelObjList>				SOLList;
typedef list<class SelObjList>::iterator	SOLIterator;

struct PrivateVar {
	CString name;
	int type;			// 0 = numeric, 1 = string

	PrivateVar() {}
	PrivateVar(const CString& _name, int _type) : name(_name), type(_type) {}

	inline bool operator==(const CString& other) const {
		CString l = name;
		CString r = other;
		l.MakeLower();
		r.MakeLower();
		return l == r;
	}
	inline bool operator==(const PrivateVar& other) const {
		CString l = name;
		CString r = other.name;
		l.MakeLower();
		r.MakeLower();
		return l == r;
	}
};

enum DisableShaderWhen {
	SVA_NOSETTING,
	SVA_PS20UNAVAILABLE,
	SVA_PS20AVAILABLE,
	SVA_PS14UNAVAILABLE,
	SVA_PS14AVAILABLE,
	SVA_PS11UNAVAILABLE,
	SVA_PS11AVAILABLE,
};

// CRunObjType defines an object type
class CRunObjType {
public:

	// Object ID
	int oid;

	// Pointer to plugin module
	struct PluginModule* pPlugin;
	int pluginID;	// dll index

	// -1 if ordinary type, >=0 if a team (virtual) object type
	int teamID;
	int contained;				// Nonzero if inside a container
	
	// True if a movement type
	bool movement;

	bool global;

	// Runtime pointer
	VRuntime* pRuntime;

	// Object unique name
	CString	Name;

	// Additional name used by movement types.  Unique amongst sisters.
	CString AuxName;

	// Parameters & edata to create objects of this type with
	initialObject* createParams;

	DisableShaderWhen destroyWhen;
	bool disabled;		// Destroying due to shader version

	// For linking movements to their associated type
	CRunObjType* pLinkType;

	// The Selected Object Lists (one for each depth of recursion)
	//int solDepth;				// Depth of current SOL
	SelObjList*			pSol;	// Pointer to current SOL (sols[solDepth])
	SOLList				sols;
	SOLIterator			curSol;	// Iterator to current SOL

	inline bool IsFamily() { return teamID > -1; }

	// List of effects this object type has available
#ifdef CONSTRUCT_SDK
	vector<void*>		effects;
#else

#ifdef CONSTRUCT_DIRECTX9
	EffectList			effects;
#endif

	// List of object instances
	ObjList				instances;
	ObjList				delayed;		// Delayed-add instances (to fix spawning self)

	// Container
	ObjTypeList			sisterTypes;	// Other types inside the container type

	// List of team types that this object belongs to, or in a family, member types of this family
	ObjTypeList			teams;

	// List of private variable names this object uses
	vector<PrivateVar>	privateVars;

	// Family private variables mapper
	map<CRunObjType*, vector<int> > teamPvMap;

	// The family movement 'container'
	ObjTypeList			familyMovs;

	// Delete all this type's instances
	void DeleteInstances();
	void MarkAllDestroying();

	// Don't save or load instances in this type
	bool noSerialize;

	// Flush delayed objects
	void FlushDelayed();

#ifdef PYTHONIMPORT
	map<CString, ACROUTINE> PySetAttrib;
	map<CString, EXPROUTINE> PyGetAttrib;
#endif

#ifdef PYTHON
	PyObject* pDictionary;
#endif


#endif
};

typedef long CNDS_PTR;
typedef long ACTS_PTR;
typedef long SUBEVENTS_PTR;

// For event generating conditions
struct EventParametersInfo2 {
	CNDS_PTR pCnds;
	ACTS_PTR pActs;
	SUBEVENTS_PTR pSubEvents;
};


// For self triggering conditions
struct EventSolInfo {
	bool modifiesSol;
	ObjTypeList* modifiers;
};

// For manually executing an event (!)
class CEventLine2 {
public:
	virtual void Run() = 0;		// Run the eventline
};

// Simple input struct:  maintained by the runtime, allowing plugins
// a simple way to access the input state.
// See VRuntime::GetInputStatePtr().
struct SimpleInputState {
	// Window-message based keyboard state, key down if true at VK_ index
	UCHAR		keybState[256];
	// Either window-message or DirectInput based mouse button state depending on runtime
	UCHAR		mouseState[3];		// 0 = left, 1 = middle, 2 = right
	// Keyboard state if DirectInput is being used.  Key down if bit 0x80 is set at DIK_ index
	UCHAR		directState[256];
	bool		isDirectInput;		// True if direct input is enabled and directState is being updated
};

// Default GetData IDs
#define GET_WINDOW			-1
#define GET_ANIMATION		-2
#define GET_ANIMFRAME		-3
#define GET_IMAGE    		-4
#define CF_GETSPEED			-5

// Default CallFunction IDs
#define CALL_REFRESH		-1

#endif