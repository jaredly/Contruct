#pragma once

class CLayoutEditor;
class EventSheet;

class CLayout : public VEditTime
{
public:
    // Constructors/Destructors
    CLayout();
    //CLayout(long w, long h);
	~CLayout();

    // Attributes
    void				Set(long w, long h);
    void				SetName(CString);
    void				SetColor(COLORREF clr);
    void				SetOpacity(float opacity);
    long				GetW() const;
    long				GetH() const;
	CString				GetName();
	COLORREF			GetColor();
	float				GetOpacity();
    virtual bool		Serialize(CArchive& ar);
	CObj*				GetObj(long instanceID);
	long				GetUnusedLayerID();
	void				SetChanged(bool bChanged);
	bool				GetChanged();
	virtual const char*	GetName(int iObjectIdentifier);

	// Draw functions
	void				Fill(RECT rc, D3DCOLOR color);
	void				FillGradient(const RECT& _rc, D3DCOLOR color1, D3DCOLOR color2, Direction direction);
	void				Line(int x1, int y1, int x2, int y2, D3DCOLOR color);
	void				Line(int x1, int y1, int x2, int y2, D3DCOLOR color1, D3DCOLOR color2);
	void				Point(int x, int y, D3DCOLOR color);
	void				Box(RECT rc, D3DCOLOR color);
	void				SetTexture(int index);
	int					AddDefaultTexture();
	int					AddTextureFromResource(HMODULE hModule, LPCTSTR pResource);
	int					AddTextureFromFile(const char* file);
	void				SerializeImage(int& image, bin& ar);
	void				SerializeAnimation(int& anim, bin& ar);
	void				DeleteTexture(int handle);
	void				GetTextureSize(int handle, SIZE* pSize);
	void				Blitrc(float x, float y, float w, float h, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, const RECTF* uv = NULL);
	void				Blitrc(float x, float y, float w, float h, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4,  const RECTF* uv = NULL);
	void				BlitrcSkew(float x, float y, float w, float h, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, const RECTF* uv = NULL, float skewX = 0.0f, float skewY = 0.0f);

	int					CreateFont(const char* fontface, unsigned int pxSize, bool italics, bool bold);
	void				DrawText(int iFont, const char* text, RECT& rc, D3DCOLOR color, DWORD format);
	void				DestroyFont(int iFont);
	HWND				GetFrameHwnd();

	// Images
	int					AddImage(int iWidth, int iHeight);
	int					AddImageFromResource(HMODULE hModule, LPCTSTR pResource);

	bool				EditImage(int iHandle);
	bool				EditImageInAnimation(int aHandle, int index);
	int					AddImageAsTexture(int iInternalHandle);
	bool				TextureExists(int iTexture);
	void				GenerateIcon(int iObjectHandle, bool bSmallImage = false, int iHandle = -1);

	// Animations
	int					AddAnimation(int iWidth, int iHeight, int iFrameCount, LPCSTR szName, int iParentHandle = -1);
	void				RemoveAnimation(int iHandle);
	int					QueryAnimationFrameCount(int iHandle);
	void				InsertFrameIntoAnimation(int iHandle, int iPosition, int iCount = 1);
	void				RemoveFrameFromAnimation(int iHandle, int iPosition, int iCount = 1);
	bool				EditAnimation(int iHandle);
	int					GetAnimationFrame(int iHandle, int index);
	void				SetAnimationModificationSettings(int iHandle,	bool AllowRemove, bool AllowSubanimations,
	bool				AllowAddRemoveFrame, bool AllowRenameFrame, bool AutoGenerateButton);
	void				SetImageName(int iHandle, LPCSTR szName);

	virtual int			GetSubAnimation(int iHandle, int index);
	virtual int			GetSubAnimationCount(int iHandle);

	POINT				GetImageCenterPoint(int handle);
	POINT				GetImagePoint(int handle, CString name);
	SIZE				GetImageSize(int handle);

	void*				GetDisplayDevice();
	void				EndBatch();
	void				Restore2DMode();
	void				TranslateFrameToScreenCoords(float& x, float& y);
	int					AddAngle(int iWidth, int iHeight, int iFrameCount, float angle, int iAnimation);
	editInfo*			GetObjectAt(POINT pos);
	editInfo*			GetObjectByID(int InstanceID);
	editInfo*			FindObjectByAnID(int InstanceID, int ObjectID);

	// Fonts
	list<CD3DDisplayFont> Fonts;

	// To save which properties are expanded
	map<CString, bool> expandSave;

	void FindContainerObjects(CObj* o, vector<CObj*>& object_list);

	// information
	CApplication* application;
	int identifier;
	EventSheet* event_sheet;
	int temporary_event_sheet_id;

	int m_unboundedScrolling;
	bool application_background;
	HTREEITEM tree;
	CObjMap objects;
	CLayerList layers;
	CLayer *current_layer;

	// Get a list of the object types on this layout, and include global objects.
	void GetLocalObjectTypes(vector<CObjType*>& out);

	// Layout editor
	CLayoutEditor* m_LayoutEditor;

	CObj* FindObjFromNumber(int fixed);

	// Grid enabled
	bool	m_Grid, m_SnapMovements, m_SnapResize;
	int		m_GridWidth, m_GridHeight;

	CString m_WndName;

	int m_ObjectFrameIsLockedTo;
	int m_ParentObjectFrameIsLockedTo;

	virtual void	CaptureFrameEditor(int iObjectIdentifier);
	virtual void	ReleaseFrameEditor(int iObjectIdentifier);

	virtual float	GetZoom();
	virtual editInfo* GetObjectBelongingToThisBehavior();
	virtual POINT GetCursor();

	void Message(int message);

	OINFO* m_oTemporyBehaviorInfo;	// used inbetwen creation of tempory movements for modifying properties - I use it so if an edittime function is called, we can work out from what object it is if the identifier is -1
	editInfo* m_pTempMovExt;	// used inbetwen creation of tempory movements for modifying properties - I use it so if an edittime function is called, we can work out from what object it is if the identifier is -1

	OINFO* m_oControlBehaviorInfo;	// If you tell a movement to be in control of the frame editor this is used
	editInfo* m_pControlBehaviorExt;// If you tell a movement to be in control of the frame editor this is used
	BYTE** m_pControlBehaviorData;	// If you tell a movement to be in control of the frame editor, this is used to serialize to once unlocked.
	int* m_pControlBehaviorDataSize;
	CPoint m_mouse; // frame editor sets this member, no need to serialize

	virtual void RefreshProperties();
	virtual void RefreshObjectList();

	CPoint scroll_position;
	CPoint m_EventEditorScroll;

	unsigned int GetFrameCount();

	// Key system
	void			SetGlobalKey(const char* name, void* param, bool save = true);
	void*			GetGlobalKey(const char* name);
	bool			GlobalKeyExists(const char* name);

	void			SetLayoutKey(const char* name, void* param, bool save = true);
	void*			GetLayoutKey(const char* name);
	bool			LayoutKeyExists(const char* name);

	KeyCollection	layoutKeys;		// Edittime layout specific keys

	HDC				BeginHDCDraw();
	void			EndHDCDraw(RECT drawzone);

	void			SetLayoutKeyString(const char* name, const char* string, bool save = true);
	const char*		GetLayoutKeyString(const char* name);

	void			SetGlobalKeyString(const char* name, const char* string, bool save = true);
	const char*		GetGlobalKeyString(const char* name);

	void			ApplyThemingToDialog(HWND hWnd);

	int				GetControlCount();
	const char*		GetControlName(int index);
	int				GetControlPlayer(int index);

	const char*		GetAnimationName(int iHandle);

	int hdcTexture;
	int hdcOldTexture;
	IDirect3DSurface9* pHdcSurface;
	HDC hdc;
	int hdcOldTarget;

	enum LayoutTextureLoading {
		tl_use_app_setting,
		tl_load_on_app_start,
		tl_load_on_layout_start
	} texture_loading;

protected:
    long m_w;
    long m_h;
	CString m_Name;
	COLORREF m_clr;
	bool m_bChanged;
};