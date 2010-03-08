//////////////////////////////////////////////
// CONSTRUCT VIRTUAL RUNTIME
// Functions for controlling Construct at run time.
#ifndef _VRUNTIME_H_
#define _VRUNTIME_H_

#include "Renderers\RendererCommon.h"

class CFrame;
typedef int AnimHandle;

// Imports from cr namespace

// SDK defines these in stdafx.h
#if !defined(CONSTRUCT_SDK) && !defined(APPRUNTIME)
	typedef cr::TextureHandle		TextureHandle;
	typedef cr::FontHandle			FontHandle;
#endif //CONSTRUCT_SDK

typedef cr::color				cr_color;
typedef cr::point				point;
typedef cr::rect				rect;
typedef cr::quad				quad;
typedef cr::cr_float			cr_float;
typedef cr::texture_format		texture_format;
typedef cr::renderstate			renderstate;
typedef cr::renderstate_value	renderstate_value;
typedef cr::samplerstate		samplerstate;
typedef cr::samplerstate_value	samplerstate_value;
typedef cr::gradient_direction	gradient_direction;

const point origin(cr::origin);
const cr_color transparent_black(cr::transparent_black);
const cr_color opaque_white(cr::opaque_white);

// Deprecated typedefs
#ifndef CONSTRUCT_SDK
typedef int CD3DBATCH;
typedef int Direction;
#endif

enum TickAnimRet {
	TA_NORMAL,			// Nothing happened (just counter increment)
	TA_FINISHED,		// Animation finished
	TA_LOOPED,			// Animation finished then restarted
	TA_PONGED,			// Animation pinged or ponged
	TA_FRAMECHANGED,	// Displaying frame changed
	TA_FORCE_DWORD = 0x7FFFFFFF,
};

///////////////////////////
// RENDERING FUNCTIONS
class DECLSPEC_NOVTABLE IRenderer {
public:
	virtual TextureHandle	CreateRenderTargetTexture(UINT width, UINT height, texture_format format = cr::texture_format_a8r8g8b8) = 0;
	virtual void			ResizeRenderTargetTexture(TextureHandle th, UINT new_width, UINT new_height) = 0;
	virtual TextureHandle	CreateTextureFromFile(PCTSTR filename, texture_format format = cr::texture_format_a8r8g8b8) = 0;
	virtual TextureHandle	CreateTextureFromFileInMemory(LPCVOID pData, UINT len, texture_format format = cr::texture_format_a8r8g8b8) = 0;
	virtual TextureHandle	CreateTextureFromResource(HMODULE hModule, LPCTSTR resource, texture_format format = cr::texture_format_a8r8g8b8) = 0;

	virtual TextureHandle	CreateTextureFromHandle(ImageHandleInfo* ihi) = 0;

	virtual void			AddTextureReference(TextureHandle th) = 0;	// Increment references to a texture
	virtual void			ReleaseTexture(TextureHandle th) = 0;		// Decrement references and deallocate if zero
	virtual void			DeallocateTexture(TextureHandle th) = 0;	// Immediately release from VRAM
	virtual bool			TextureExists(TextureHandle th) const = 0;	// Test if a texture exists
	virtual void			ReplaceTexture(TextureHandle oldTex, TextureHandle newTex) = 0;

	virtual void			SaveTextureToFileInMemory(void** lpBuffer, int* pBufferSize, TextureHandle th, int iImageFormat) = 0;
	virtual void*			GetTextureFileBuffer(void** lpBuffer) = 0;
	virtual void			FreeTextureFileMemory(void** lpBuffer) = 0;

	// Batching control - avoid EndBatch() if possible, as it flushes the GPU command queue
	virtual void			EndBatch() = 0;
	virtual void			BeginBatchQuads(UINT vertices, UINT indices) = 0;
	virtual void			AddIndex(unsigned short index) = 0;
	virtual void			AddVertex(cr::point3d xyz, point uv, const cr_color& c) = 0;

	// State control
	virtual void			SetRenderState(renderstate rs, renderstate_value rsv) = 0;
	virtual void			SetSamplerState(samplerstate ss, samplerstate_value ssv) = 0;

	virtual renderstate_value	GetRenderState(renderstate rs) const = 0;
	virtual samplerstate_value	GetSamplerState(samplerstate ss) const = 0;

	virtual void			SetAdditiveBlending() = 0;
	virtual void			SetAlphaBlending() = 0;

	// Drawing
	virtual void			ClearRenderTarget(const cr_color& c = transparent_black, const rect* area = NULL) = 0;

	virtual void			SetTexture(TextureHandle th) = 0;
	virtual void			SetRenderTarget(TextureHandle th) = 0;
	virtual void			RestoreRenderTarget() = 0;
	virtual TextureHandle	GetTexture() const = 0;
	virtual TextureHandle	GetRenderTarget() const = 0;

	// Overloads don't seem to work in Construct<->SDK interfaces, so they are named differently here
	virtual void			Quad_xy(cr_float x, cr_float y, cr_float angle = 0.0, point hotspot = origin, const cr_color& filter = opaque_white, const rect* _uv = NULL, cr_float z_elevation = 0.0) = 0;
	virtual void			Quad_xywh(cr_float x, cr_float y, cr_float width, cr_float height, cr_float angle = 0.0, point hotspot = origin, const cr_color& filter = opaque_white, const rect* _uv = NULL, cr_float x_skew = 0.0, cr_float y_skew = 0.0, const cr_color* vertexcolors = NULL, cr_float z_elevation = 0.0) = 0;
	
	virtual void			Point(point p, const cr_color& c) = 0;

	// Aliased lines
	virtual void			Line(point p1, point p2, const cr_color& c) = 0;
	virtual void			Line2(point p1, point p2, const cr_color& c1, const cr_color& c2) = 0;

	// Antialiased ID3DXLines
	virtual void			SmoothLine(point p1, point p2, const cr_color& c, cr_float width = 1.0) = 0;
	virtual void			Box(const rect& r, const cr_color& c, cr_float angle = 0.0, point hotspot = origin) = 0;
	virtual void			Fill(const rect& r, const cr_color& c, cr_float angle = 0.0, point hotspot = origin) = 0;
	virtual void			FillGradient(const rect& r, const cr_color& c1, const cr_color& c2, gradient_direction dir, cr_float angle = 0.0, point hotspot = origin) = 0;

	// Text rendering.  Names involve CR to prevent conflict with WINAPI names
	virtual FontHandle		CreateCRFont(PCTSTR font_face, INT height, UINT width = 0, UINT weight = FW_NORMAL, BOOL italic = false, DWORD quality = ANTIALIASED_QUALITY, BYTE charset = DEFAULT_CHARSET, BYTE pitch_and_family = DEFAULT_PITCH|FF_DONTCARE) = 0;
	virtual bool			FontExists(FontHandle fh) const = 0;
	virtual void			DrawTextCR(FontHandle fh, PCTSTR text, const rect& r, DWORD format, const cr_color& c) = 0;
	virtual void			ReleaseCRFont(FontHandle fh) = 0;

	virtual void*			GetDevice() const = 0;		// Avoid using if possible, non-portable engine specific device pointer!

	virtual void			SetScreenTranslation(bool enable = true) = 0;

	virtual void			SetPointSize(cr_float point_size) = 0;
	virtual cr_float		GetPointSize() const = 0;

	// Projection math
	virtual point			Project(cr::point3d pt) const;
	virtual void			Project(const cr::point3d* pts, cr::point3d* outs, UINT count) const;
	virtual cr::point3d		Unproject(cr::point3d pt) const;
	virtual void			Unproject(const cr::point3d* pts, cr::point3d* outs, UINT count) const;

	virtual void			SmoothLine3D(cr::point3d p1, cr::point3d p2, const cr_color& c, cr_float width = 1.0) = 0;

	virtual void			GetTextSize(FontHandle fh, PCTSTR text, rect& r) = 0;
};

class DECLSPEC_NOVTABLE VRuntime {
#ifndef APPRUNTIME
protected:
	VRuntime(IRenderer* _renderer)
		: pRenderer(_renderer)
	{}
#endif
public:

	///////////////////////////
	// DISPLAY ENGINE FUNCTIONS
	// Textures
	virtual Texture	deprecated_AddTextureRT(int w, int h) = 0;
	virtual void	deprecated_ResizeTextureRT(Texture* tex, int w, int h) = 0;
	virtual Texture	deprecated_AddTextureFromResource(HMODULE hModule, LPCTSTR pResource) = 0;
	virtual Texture	deprecated_AddTextureFromFile(const char* filename) = 0;
	virtual Texture	deprecated_AddTextureFromFileInMemory(LPCVOID pData, UINT len) = 0;

	// Gives a texture reference from an image bank handle
	virtual Texture	deprecated_AddTextureFromHandle(BankImage handle) = 0;
	virtual void	deprecated_DeleteTexture(Texture tex) = 0;	// Frees texture completely and invalidates handle
	virtual void	deprecated_Blit(float x, float y, float xf = 1.0f, float yf = 1.0f, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, RECTF* uv = NULL) = 0;
	virtual void	deprecated_Blitrc(float x, float y, float w, float h, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, RECTF* uv = NULL) = 0;
	virtual void	deprecated_Blit(float x, float y, float xf, float yf, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4,  RECTF* uv = NULL) = 0;
	virtual void	deprecated_Blitrc(float x, float y, float w, float h, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4,  RECTF* uv = NULL) = 0;

	virtual void	deprecated_SetTexture(Texture tex) = 0;
	virtual void	deprecated_SetRenderTarget(Texture tex) = 0;
	virtual void	deprecated_RestoreRenderTarget() = 0;
	virtual void	deprecated_GetTextureSize(Texture tex, SIZE* imgSize, SIZE* texSize) = 0;
	virtual void	deprecated_ClearRT(D3DCOLOR color = 0x00000000) = 0;
	virtual void	deprecated_ReplaceTexture(Texture oldTexture, Texture newTexture) = 0;

	// For app runtime access to imagelist
	virtual HBITMAP	GetImageHBITMAP(BankImage imgHandle) = 0;
	
	// Display states (DirectX)
	virtual void			deprecated_SetRenderState(D3DRENDERSTATETYPE rstype, UINT value) = 0;
	virtual unsigned long	deprecated_GetRenderState(D3DRENDERSTATETYPE rstype) = 0;
	virtual void			deprecated_SetSamplerState(D3DSAMPLERSTATETYPE sstype, UINT value) = 0;
	virtual void			deprecated_SetGlobalFilter(D3DCOLOR color) = 0;
	virtual D3DCOLOR		deprecated_GetGlobalFilter() = 0;

	// Primitives (DirectX)
	virtual void	deprecated_Fill(const RECTF& rc, D3DCOLOR color) = 0;
	virtual void	deprecated_Point(float x, float y, D3DCOLOR color) = 0;
	virtual void	deprecated_Line(float x1, float y1, float x2, float y2, D3DCOLOR color) = 0;
	virtual void	deprecated_Box(const RECTF& rc, D3DCOLOR color) = 0;
	virtual void	deprecated_FillGradient(const RECTF& rc, D3DCOLOR c1, D3DCOLOR c2, Direction dir) = 0;
//	virtual void	GradientLine(int x1, int y1, int x2, int y2, D3DCOLOR start, D3DCOLOR end) = 0;
//	virtual void	GradientBox(const RECT rc, D3DCOLOR tl, D3DCOLOR tr, D3DCOLOR bl, D3DCOLOR br) = 0;

	// Set a render target to receive copies of what the frame looks like
	virtual void	SetCopyFrameContent(CRunObject* obj, TextureHandle rt, bool postrender = true, bool bSet = true) = 0;

	// Batching (DirectX)
	virtual void	deprecated_BeginBatch(int type, int num) = 0;
	virtual void	deprecated_EndBatch() = 0;

	// Fonts & texts (DirectX)
	virtual DXFont	deprecated_CreateFont(const char* fontface, unsigned int pxSize, bool italics, bool bold) = 0;
	virtual void	deprecated_DrawText(DXFont iFont, const char* text, RECTF& rc, D3DCOLOR color, DWORD format) = 0;
	virtual void	deprecated_DestroyFont(DXFont iFont) = 0;

	// General purpose display functions (DirectX)
	virtual void*	deprecated_GetDisplayDevice() const = 0;

	//////////////////////////////
	// CONSTRUCT RUNTIME FUNCTIONS
	// Get data
	virtual HWND GetAppHwnd() const = 0;				// HWND to main window
	virtual HINSTANCE GetAppHInstance() const = 0;		// HINSTANCE of application
	virtual int GetRuntimeType() const = 0;				// See defines below

	// Event, action, condition and expression handling
	virtual void GenerateEvent(int oid, int cndID, CRunObject* pCaller) = 0;
	virtual void GenerateEventFast(FASTTRIGGER ft) = 0;
	virtual void GenerateEventInline(int cndID, CRunObject* pCaller) = 0;
	virtual FASTTRIGGER GetFastTriggerHandle(int oid, int cndID, ExpReturn* cndParamList) = 0;
	virtual void GetCurrentEventPtrs(EventParametersInfo2& epi) const = 0;
	virtual void GetCurrentEventSolInfo(EventSolInfo& esi) const = 0;
	virtual bool RunConditions(CNDS_PTR pCnds) = 0;
	virtual void RunActions(ACTS_PTR pActions) = 0;
	virtual void RunSubEvents(SUBEVENTS_PTR pSubEvents) = 0;
	virtual void ReEvaluateCndParameter(int index, ExpReturn* ret) = 0;
	virtual CEventLine2* GetCurrentEventPtr() const = 0;
	virtual void* GetCurrentConditionPtr() const = 0;
	virtual bool CndsBefore(CNDS_PTR pCnds) const = 0;
	virtual bool CndsAfter(CNDS_PTR pCnds) const = 0;

	// Objects & object lists
	virtual void DestroyObject(CRunObject* pObject) = 0;
	virtual CRunObject* CreateObject(CRunObjType* pType, int layer, CRunLayout* pLayout) = 0;
	virtual CRunObjType* GetTypeFromName(const char* name) const = 0;
	virtual CRunObjType* GetTraitFromName(const char* name) = 0;
	virtual CRunObjType* GetTypeFromOID(int oid) const = 0;
	virtual void NewSOL() = 0;
	virtual void NewSOLCopy() = 0;
	virtual void NewSOLCopy(ObjTypeList* modifiers) = 0;
	virtual void RestoreSOL() = 0;
	virtual void RestoreSOL(ObjTypeList* modifiers) = 0;
	virtual void ToFront(CRunObject* obj) = 0;
	virtual void ToBack(CRunObject* obj) = 0;
	virtual void InFront(CRunObject* obj, CRunObject* other) = 0;
	virtual void ToBehind(CRunObject* obj, CRunObject* other) = 0;
	virtual void ToLayer(CRunObject* obj, int layer) = 0;
	virtual int GetNumLayers(CRunLayout* pLayout) const = 0;

	// Private variables
	virtual int GetPrivateVariableCount(CRunObject* obj) = 0;
	virtual void GetPrivateVariableInfo(CRunObject* obj, int index, int& type, const char*& str) = 0;

	// Collisions & selection
	virtual bool QuerySelectCollision(CRunObject* a, CRunObject* b) = 0;
	virtual bool QuerySelectCollision(CRunObject* obj, CRunObjType* type) = 0;
	virtual bool QueryCollision(CRunObject* a, CRunObject* b) = 0;
	virtual bool QueryCollision(CRunObject* obj, CRunObjType* type) = 0;
	virtual bool QueryPointCollision(int x, int y, CRunObject* obj) = 0;
	virtual bool QueryPointCollision(int x, int y, CRunObjType* type) = 0;
	virtual bool QuerySelectPointCollision(int x, int y, CRunObject* obj) = 0;
	virtual bool QuerySelectPointCollision(int x, int y, CRunObjType* type) = 0;
	virtual void Select(CRunObject* obj) = 0;
	virtual bool IsSelected(CRunObject* obj) = 0;
	virtual void Unselect(CRunObject* obj) = 0;
	virtual void UnselectAll(CRunObject* obj) = 0;
	virtual void UnselectAll(CRunObjType* type) = 0;
	virtual void SelectAll(CRunObjType* type) = 0;
	virtual void UpdateBoundingBox(CRunObject* obj) = 0;

	// Plugin notifications & windowing
	virtual void RegisterWindow(HWND hWnd, CRunObject* pObject) = 0;
	virtual void UnRegisterWindow(HWND hWnd, CRunObject* pObject) = 0;
	virtual void CallPreTranslateMessage(CRunObject* pObject, bool bCall = true) = 0;
	virtual LRESULT WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) = 0;

	// Animations
	virtual CRunAnimation*	GetAnimationPtr(Animation anim) = 0;
	virtual TickAnimRet		TickAnimation(CRunObject* obj) = 0;
	virtual Animation		GetAnimationByName(Animation parent, const char* name) = 0;
	virtual Animation		GetAnimationByNearestAngle(Animation parent, float angle) = 0;
	virtual CRunAnimationFrame* GetCurAnimationFramePtr(AnimInfo* info) = 0;
	virtual CRunAnimation*	SetAnimation(Animation anim, CRunObject* obj) = 0;
	virtual void			UpdateAnimationFrame(CRunObject* obj) = 0;
	virtual CRunAnimationFrame* GetAnimationFrame(CRunAnimation* pAnim, int frameIndex) = 0;
	virtual POINT			GetImageCenterPoint(BankImage handle) = 0;
	virtual bool			GetImagePoint(CRunObject* obj, int number, POINTF& pt) = 0;
	virtual bool			GetImagePoint(CRunObject* obj, const char* pointName, POINTF& pt) = 0;
	virtual int				GetAnimationFrameCount(CRunAnimation* pAnim) = 0;
	virtual CRunAnimation*	GetSubAnimation(CRunAnimation* pAnim, int index) = 0;
	virtual int				GetSubAnimationCount(CRunAnimation* pAnim) = 0;
	virtual CRunAnimation*	GetSubAngleAnimation(CRunAnimation* pAnim, int index) = 0;
	virtual int				GetSubAngleAnimationCount(CRunAnimation* pAnim) = 0;

	// Wrappers and extended calls
	virtual long		CallEx(int id, long param1, long param2) = 0;
	virtual void		GetObjectSiblings(CRunObject* obj, CRunObject**& siblingList, int& count) const = 0;
	virtual void		GetLayers(CRunLayout* pLayout, CRunLayer**& layerList, int& count) const = 0;
	virtual void		GetTypes(CRunObjType**& typesList, int& count) = 0;
	virtual void		GetTypeInstances(CRunObjType* pType,  CRunObject**& instancesList, int& count) const = 0;
	virtual void		GetTypeSelectedInstances(CRunObjType* pType, CRunObject**& instancesList, int& count) const = 0;
	virtual CRunObject* GetFirstInstance(CRunObjType* pType) const = 0;
	virtual CRunObject* GetPairedObject(CRunObjType* pType, CRunObject* obj) const = 0;
	virtual int			GetInstanceIndex(CRunObject* obj) = 0;
	virtual CRunLayer*	GetObjectLayer(CRunObject* obj) const = 0;


	virtual void	GetMousePos(int& x, int& y) = 0;
	virtual bool	ObjectOffscreen(CRunObject* obj) const = 0;
	virtual bool	ObjectOutsideFrame(CRunObject* obj) const = 0;
	virtual void	CallOnFrame(CRunObject* obj) = 0;
	virtual void	CallOnFrame2(CRunObject* obj) = 0;

	// Give a random number [0, 1) using runtime's Mersenne Twister
	virtual double	Random() = 0;

	// Workaround for non-ABI conformant compilers returning CStrings
	virtual void	ReturnStringWorkaround(const char* str) = 0;
	virtual void	AssignCString(void* mfc_cstr, const char* str) = 0;

	// Get input state pointer
	virtual const SimpleInputState* GetInputStatePtr() = 0;

	// Throw a runtime error.  In most cases terminates the runtime with the error message.
	virtual void Throw(const char* msg) = 0;

	// Frame functions
	virtual CRunLayout*		CreateRunningFrame(int frameNum, HWND hWnd = NULL) = 0;		// Create a new running frame
	virtual void			DestroyRunningFrame(CRunLayout* pLayout) = 0;
	virtual void			DrawRunningFrame(CRunLayout* pLayout) = 0;
	virtual HWND			GetFrameHwnd(CRunLayout* pLayout) = 0;
	virtual SIZE			GetWindowSize() const = 0;
	virtual void			DoFrame() = 0;		// Force the runtime to run another tick
	virtual LRESULT			SendMessageToPlugins(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) = 0;

	// For ExpReturn::GetVariableIndex
	virtual int		TranslatePrivateVariableIndex(CRunObjType* pCallerType, CRunObjType* pOriginalType, int index) const = 0;

	// Debugger functions
	virtual void	AddDebuggerItem(const char* label, const char* value, bool readOnly) = 0;
	virtual void	AddDebuggerItem(const char* label, int value, bool readOnly) = 0;
	virtual void	AddDebuggerItem(const char* label, __int64 value, bool readOnly) = 0;
	virtual void	AddDebuggerItem(const char* label, float value, bool readOnly) = 0;
	virtual void	AddDebuggerItem(const char* label, double value, bool readOnly) = 0;
	virtual void	AddDebuggerCombo(const char* label, const char* items, int sel) = 0;
	virtual void	AddDebugLogMessage(const char* msg) = 0;
	virtual int		GetVariableIndexFromName(const char* name, CRunObject* obj) = 0;

	// Control systems
	virtual int		GetControlVK(const char* controlName, int player) = 0;
	virtual int		GetControlDIK(const char* controlName, int player) = 0;

	virtual void SpawnSelect(CRunObject* parent, CRunObject* sibling) = 0;
	virtual void deprecated_Restore2DMode() = 0;

	// Key access
	virtual void SetGlobalKey(const char* name, void* param) = 0;
	virtual void* GetGlobalKey(const char* name) = 0;
	virtual bool GlobalKeyExists(const char* name) = 0;

	virtual void SetLayoutKey(CRunLayout* pLayout, const char* name, void* param) = 0;
	virtual void* GetLayoutKey(CRunLayout* pLayout, const char* name) = 0;
	virtual bool LayoutKeyExists(CRunLayout* pLayout, const char* name) = 0;

	virtual void ScreenToLayout(const CRunLayout* pLayout, float& x, float& y) const = 0;
	virtual void ScreenToLayer(const CRunLayout* pLayout, int layer, float& x, float& y) const = 0;

	virtual double GetTimeDelta() const = 0;
	virtual clock_t GetAppClock() = 0;
	virtual clock_t GetLayoutClock(CRunLayout* pLayout) = 0;

	virtual	HMENU	GetMenuResource(int id) = 0;

	virtual CRunLayer* GetLayer(CRunLayout* pLayout, int number) = 0;
	virtual CRunLayer* GetLayer(CRunLayout* pLayout, const char* name) = 0;

	virtual Texture deprecated_GetRenderTarget() = 0;
	virtual unsigned long deprecated_GetSamplerState(D3DSAMPLERSTATETYPE sstype) = 0;

	virtual void SerializeObjectPtr(CRunObject*& obj, bin& ar) = 0;
	virtual void SerializeObjectType(CRunObjType*& pType, bin& ar) = 0;

	virtual void SelectF(CRunObject* obj, CRunObjType* pType) = 0;
	virtual bool IsSelectedF(CRunObject* obj, CRunObjType* pType) = 0;
	virtual void UnselectF(CRunObject* obj, CRunObjType* pType) = 0;

	virtual CollisionMask* CreateCollisionMask(int width, int height) = 0;
	virtual void ReleaseCollisionMask(CollisionMask* mask) = 0;
	virtual CollisionMask* GetCollisionMaskFromImage(int image) = 0;
	virtual void AddMaskToMask(CollisionMask* src, CollisionMask* dest, int x, int y) = 0;
	virtual void ExportCollisionToFile(CollisionMask* mask, int index) = 0;
	virtual void SetAnimationFrame(CRunObject* obj, int frameIndex) = 0;

	virtual void DrawObject(CRunObject* obj) = 0;

	virtual void deprecated_SaveTextureToFileInMemory(void** lpBuffer, int* iBufferSize, int iTextureHandle, int iImageFormat) = 0;
	virtual void deprecated_FreeTextureFileMemory(void** lpBuffer) = 0;
	virtual void* deprecated_GetTextureFileBuffer(void** lpBuffer) = 0;

	// Using x, y in screen coordinates and translating layer coords
	virtual bool QueryScreenPointCollision(int x, int y, CRunObjType* type) = 0;
	virtual bool QuerySelectScreenPointCollision(int x, int y, CRunObjType* type) = 0;

	virtual bool deprecated_TextureExists(Texture iTexture) = 0;

	// New String keys
	virtual const char* GetGlobalKeyString(const char* name) = 0;
	virtual const char* GetLayoutKeyString(CRunLayout* pLayout, const char* name) = 0;

	virtual void	deprecated_BlitrcSkew(float x, float y, float w, float h, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4,  RECTF* uv = NULL, float skewX = 0.0f, float skewY = 0.0f) = 0;
	virtual void	GenerateCollisionMaskFromTexture(CRunObject* pObj, TextureHandle th) = 0;

	// Custom rendering functions
	virtual void	deprecated_AddVertex(D3DCOLOR color, float x, float y, float u, float v) = 0;
	virtual void	deprecated_AddIndex(unsigned int i) = 0;

	// Bounce functions.  Note angles here are in radians.
	virtual float	CalculateBounceAngle(CRunObject* _This, float input_angle, CRunObjType* bounceOff) = 0;

	virtual void	deprecated_ReleaseTexture(Texture tex) = 0;	// Decrease reference count on a texture, frees if reference reaches 0

	virtual void	SetTimeScale(double ts) = 0;
	virtual double	GetTimeScale() const = 0;

	virtual int GetZIndex(CRunObject* obj) = 0;

	// Custom 3d rendering functions
	virtual void	deprecated_AddVertex3d(D3DCOLOR color, float x, float y, float z, float u, float v) = 0;

#ifdef APPRUNTIME
	// Do not call this function in the application runtime
	virtual void    SetTransform(int, int) = 0;
#else
	virtual void    deprecated_SetTransform(D3DTRANSFORMSTATETYPE State, D3DXMATRIX* pMatrix) = 0;
#endif

	// Enable and all draw commands operate in screen space - disable to restore scroll rendering
	virtual void	SetScreenTranslation(bool enable) = 0;
	virtual void	deprecated_Setup2DMode() = 0;

	// For collision workarounds
	virtual void	RegisterCollision(CRunObject* a, CRunObject* b) = 0;
	virtual bool	QueryRegisterCollision(CRunObject* a, CRunObjType* b) = 0;
	virtual bool	SelectRegisteredCollisions(CRunObject* obj, CRunObjType* pType) = 0;

	// Get family types - if type is a family, gets its member types - otherwise gets the list of families type belongs to
	virtual void	GetFamilyTypes(CRunObjType* pType, CRunObjType**& pTypesList, int& count) = 0;

	virtual void	ScreenToScreen(POINT& p) = 0;

	// Smooth line drawing
	virtual void	deprecated_SmoothLineBegin() = 0;
	virtual void	deprecated_SmoothLine(float x1, float y1, float x2, float y2, D3DCOLOR color) = 0;
	virtual void	deprecated_SmoothLineEnd() = 0;

	// Preset animations
	virtual CRunAnimation* GetPresetAnimation(CRunAnimation* pRoot, int tagID) = 0;

	virtual ImageHandleInfo* LoadSerializedImageHandle(bin& ar) = 0;

	virtual bool QueryScreenPointInstanceCollision(int x, int y, CRunObject* obj) = 0;

	virtual bool ConditionIsNegated() const = 0;

	// Animation load/release
	virtual void LoadAnimation(CRunAnimation* pRootAnim) = 0;
	virtual void ReleaseAnimation(CRunAnimation* pRootAnim) = 0;

	virtual void SuppressElseEvent() = 0;

	// Other control functions
	virtual float GetControlState(const char* controlName, int player) = 0;
	virtual bool SetControlState(const char* controlName, int player, float val) = 0;

	// private variable by name
	virtual int		GetPVIndexFromName(const CString& name, CRunObjType* pType)= 0;

	//////////////////////////////
	// PLUGIN ACCESSIBLE DATA - for internal use only!

	// Temporary strings for string returning expressions
	CString tempStrings[32];
	int curtempstr;

#ifndef APPRUNTIME
	// Renderer class
	IRenderer* const pRenderer;
#endif
};

inline void CRunObject::UpdateBoundingBox()
{
	pRuntime->UpdateBoundingBox(this);
}

inline void CRunObject::TriggerEvent(int cndIndex)
{
	pRuntime->GenerateEvent(info.oid, cndIndex, this);
}

// Values returned by GetRuntimeType().
#define	APPLICATION_RUNTIME		1
#define	DIRECTX9_RUNTIME		2
#define	SDL_RUNTIME				3

// RegisterWindow shortcut
#define RegisterWindow2(a) RegisterWindow((a), this)
#define GenerateEvent2(a) GenerateEvent(info.oid, (a), this)

#endif // _VRUNTIME_H_