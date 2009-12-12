//////////////////////////////////////
// VIRTUAL EDIT TIME
// Functions to control Construct during the edit time environment.
#ifndef _VEDITTIME_H_
#define _VEDITTIME_H_

#define USEMYRESOURCES() AFX_MANAGE_STATE(AfxGetStaticModuleState())
class bin;
class editInfo;
class CAnimSettings;
#ifndef DECLSPEC_NOVTABLE
#define DECLSPEC_NOVTABLE
#endif
class DECLSPEC_NOVTABLE VEditTime {
public:

	// Drawing operations
	virtual void		Fill(RECT rc, D3DCOLOR color) = 0;
	virtual void		FillGradient(const RECT& rc, D3DCOLOR color1, D3DCOLOR color2, Direction direction) = 0;
	virtual void		Line(int x1, int y1, int x2, int y2, D3DCOLOR color) = 0;
	virtual void		Line(int x1, int y1, int x2, int y2, D3DCOLOR color1, D3DCOLOR color2) = 0;
	virtual void		Box(RECT rc, D3DCOLOR color) = 0;

	// Texture operations
	virtual void		SetTexture(int index) = 0;
	virtual int			AddDefaultTexture() = 0;
	virtual int			AddTextureFromResource(HMODULE hModule, LPCTSTR pResource) = 0;
	virtual int			AddTextureFromFile(const char* file) = 0;
	virtual void		DeleteTexture(int handle) = 0;
	virtual void		GetTextureSize(int handle, SIZE* pSize) = 0;
	virtual void		Blitrc(float x, float y, float w, float h, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, const RECTF* uv = NULL) = 0;

	// Fonts
	virtual int			CreateFont(const char* fontface, unsigned int pxSize, bool italics, bool bold) = 0;
	virtual void		DrawText(int iFont, const char* text, RECT& rc, D3DCOLOR color, DWORD format) = 0;
	virtual void		DestroyFont(int iFont) = 0;

	// General
	virtual HWND		GetFrameHwnd() = 0;

	// Images	
	virtual int			AddImage(int iWidth, int iHeight) = 0;


	virtual bool		EditImage(int iHandle) = 0;
	virtual int			AddImageAsTexture(int iHandle) = 0;
	virtual void		GenerateIcon(int iObjectIdentifier, bool bSmallIcon = false, int iHandle = -1) = 0;

	// Animations
	virtual int		AddAnimation(int iWidth, int iHeight, int iFrameCount, LPCSTR szName, int iParentHandle) = 0;
	virtual void		RemoveAnimation(int iHandle) = 0;
	virtual int		QueryAnimationFrameCount(int iHandle) = 0;
	virtual void		InsertFrameIntoAnimation(int iHandle, int iPosition, int iCount = 1) = 0;
	virtual void		RemoveFrameFromAnimation(int iHandle, int iPosition, int iCount = 1) = 0;
	virtual bool		EditAnimation(int iHandle) = 0;

	virtual int		AddImageFromResource(HMODULE hModule, LPCTSTR pResource) = 0;
	virtual void		SerializeImage(int& image, bin& ar) = 0;
	virtual POINT		GetImageCenterPoint(int handle) = 0;
	virtual POINT		GetImagePoint(int handle, CString szNumber) = 0;
	virtual SIZE		GetImageSize(int handle) = 0;

	virtual int		GetAnimationFrame(int iHandle, int index) = 0;
	virtual void		SerializeAnimation(int& anim, bin& ar) = 0;	

	virtual void		Point(int x, int y, D3DCOLOR color) = 0;
	virtual const char*	GetName(int iObjectIdentifier) = 0;

	virtual void		CaptureFrameEditor(int iObjectIdentifier) = 0;
	virtual void		ReleaseFrameEditor(int iObjectIdentifier) = 0;
	virtual float		GetZoom() = 0;
	virtual editInfo*	GetObjectBelongingToThisBehavior() = 0;
	virtual POINT		GetCursor() = 0;
	virtual void		RefreshProperties() = 0;

	virtual void		SetAnimationModificationSettings(int iHandle,	bool AllowRemove, bool AllowSubanimations, bool AllowAddRemoveFrame, bool AllowRenameFrame, bool AutoGenerateButton) = 0;
	virtual void		SetImageName(int iHandle, LPCSTR szName) = 0;

	virtual int		GetSubAnimation(int iHandle, int index) = 0;
	virtual int		GetSubAnimationCount(int iHandle) = 0;

	virtual void*		GetDisplayDevice() = 0;
	virtual void		EndBatch() = 0;
	virtual void		Restore2DMode() = 0;
	virtual void		TranslateFrameToScreenCoords(float& x, float& y) = 0;
	virtual int			AddAngle(int iWidth, int iHeight, int iFrameCount, float angle, int iAnimation) = 0;
	virtual unsigned int GetFrameCount() = 0;

	// Key access
	virtual void		SetGlobalKey(const char* name, void* param, bool save = true) = 0;
	virtual void*		GetGlobalKey(const char* name) = 0;
	virtual bool		GlobalKeyExists(const char* name) = 0;

	virtual void		SetLayoutKey(const char* name, void* param, bool save = true) = 0;
	virtual void*		GetLayoutKey(const char* name) = 0;
	virtual bool		LayoutKeyExists(const char* name) = 0;

	virtual void		RefreshObjectList() = 0;

	virtual void		Blitrc(float x, float y, float w, float h, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4,  const RECTF* uv = NULL) = 0;

	// HDC drawing
	virtual HDC		BeginHDCDraw() = 0;
	virtual void		EndHDCDraw(RECT drawzone) = 0;

	virtual editInfo*	GetObjectAt(POINT pos) = 0;
	virtual editInfo*	GetObjectByID(int InstanceID) = 0;

	// New string overloads for keys
	virtual void		SetLayoutKeyString(const char* name, const char* string, bool save = true) = 0;
	virtual const char*	GetLayoutKeyString(const char* name) = 0;
	virtual void		SetGlobalKeyString(const char* name, const char* string, bool save = true) = 0;
	virtual const char*	GetGlobalKeyString(const char* name) = 0;

	virtual void		BlitrcSkew(float x, float y, float w, float h, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, const RECTF* uv = NULL, float skewX = 0.0f, float skewY = 0.0f) = 0;

	virtual editInfo*	FindObjectByAnID(int InstanceID, int ObjectID) = 0;

	virtual void		ApplyThemingToDialog(HWND hWnd) = 0;
	virtual bool		TextureExists(int iTexture) = 0;

	virtual int			GetControlCount() = 0;
	virtual const char* GetControlName(int index) = 0;
	virtual int			GetControlPlayer(int index) = 0;

	virtual const char*	GetAnimationName(int iHandle) = 0;

	virtual float		GetEyeDistance3d() = 0;
};

// Handy define for loading resources from this ext
#define AddTextureFromResource2(p) AddTextureFromResource(AfxGetResourceHandle(), (p))
#define AddImageFromResource2(p) pEditTime->AddImageFromResource(AfxGetResourceHandle(), (p))

#endif //_VEDITTIME_H_