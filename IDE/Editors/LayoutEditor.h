#pragma once

class CArrayPasteDlg;
#include "..\Construct Doc.h"

void AllocateDisplaySurface(int& iTexture, CD3DDisplay& display);
void AllocateDisplaySurface(int* iTexture, CD3DDisplay* display);
void AllocateDisplaySurface(int* iTexture, CD3DDisplay** display);

extern ControlTableEntry controlTable[];

class CLayoutEditor;
class CLayout;

struct PropertyMgrValue {
	CString text;
	int i;
	double f;
};

class CPropValueMgr {
public:
	CExtPropertyItem* pItem;
	CExtGridCellEx* pCell;
	PropertyMgrValue value;
	int type;

	virtual int OnChange() = 0;

	void GetPropertyValue();
};

// For modifying app properties
class CAppPropertyValueMgr : public CPropValueMgr {
public:
	CApplication*& application;
	CString propName;

	CAppPropertyValueMgr(CApplication*& _pApp, const CString& _propName) : application(_pApp), propName(_propName) {}

	int OnChange();
};

// For modifying resource properties
class ApplicationResourcePropertyValueMgr : public CPropValueMgr {
public:
	ApplicationResource* resource;
	list<ApplicationResource>* stored;
	CString propName;
	CApplication* application;

	ApplicationResourcePropertyValueMgr(ApplicationResource* _resource, const CString& _propName, list<ApplicationResource>* stored_, CApplication* application_) : 
		resource(_resource), 
		propName(_propName),
		stored(stored_),
		application(application_)
	{
	}

	int OnChange();
};

// For modifying controls
class CControlPropertyValueMgr : public CPropValueMgr {
public:
	CApplication*& application;
	CString propName;
	int player;

	CControlPropertyValueMgr(CApplication*& _pApp, const CString& _propName, int _player)
		: application(_pApp), propName(_propName), player(_player) {}

	int OnChange();
};

// For modifying layout properties
class CLayoutPropertyValueMgr : public CPropValueMgr {
public:
	CLayout* layout;
	CString propName;

	CLayoutPropertyValueMgr(CLayout* _layout, const CString& _propName) : layout(_layout), propName(_propName) {}
	
	int OnChange();
};

// For modifying layer properties
class CLayerPropertyValueMgr : public CPropValueMgr {
public:
	CLayer* pLayer;
	CString propName;
	CLayoutEditor* layout;

	CLayerPropertyValueMgr(CLayer* _pLayer, CLayoutEditor* _layout, const CString& _propName)
		: pLayer(_pLayer), layout(_layout), propName(_propName) {}	
	int OnChange();
};

// For modifying layer properties
class CAnimationPropertyValueMgr : public CPropValueMgr {
public:
	CString propName;
	CObj* pObj;
	CAnimation* pAnim;
	int frameIndex;

	CAnimationPropertyValueMgr(CAnimation* _pAnim, int _frameIndex, const CString& _propName)
		: propName(_propName), frameIndex(_frameIndex), pAnim(_pAnim) {}

	int OnChange();
};

// For modifying standard plugin properties eg. X/Y
class CPluginStdPropertyValueMgr : public CPropValueMgr {
public:
	CObj* o;
	CObjType* pType;
	CString propName;
	CLayoutEditor* frame;

	CPluginStdPropertyValueMgr(CObj* _obj, CObjType* _type, CLayoutEditor* _frame, const CString& _propName)
		: o(_obj), pType(_type), frame(_frame), propName(_propName) {}

	int OnChange();
};

class FamilyPropertyMgr : public CPropValueMgr {
public:
	CObjType* pType;
	CString name;
	CLayoutEditor* frame;

	FamilyPropertyMgr(CObjType* _pType, CString _name, CLayoutEditor* _frame)
		: pType(_pType), name(_name), frame(_frame) {}

	int OnChange();
};

class CContainerPropertyMgr : public CPropValueMgr {
public:
	CContainer* pContainer;
	long oid;
	CLayoutEditor* frame;

	CContainerPropertyMgr(CContainer* _pCtnr, long _oid, CLayoutEditor* _frame)
		: pContainer(_pCtnr), oid(_oid), frame(_frame) {}

	int OnChange();
};

class CTransitionPropertyMgr : public CPropValueMgr {
public:

	CTransition* m_pTransition;
	CApplication* application;
	CString propName;

	CTransitionPropertyMgr(CTransition* pTransition, CApplication* application_, CString _propName)
		: m_pTransition(pTransition), application(application_), propName(_propName) {}

	int OnChange();
};

class EventSheetPropertyMgr : public CPropValueMgr {
public:

	EventSheet* m_pEventSheet;
	CApplication* application;
	CString propName;

	EventSheetPropertyMgr(EventSheet* pEventSheet, CApplication* application, CString _propName)
		: m_pEventSheet(pEventSheet), application(application), propName(_propName) {}

	int OnChange();
};

class CGlobalPropertyMgr : public CPropValueMgr {
public:
	CApplication* application;
	CString name;

	CGlobalPropertyMgr(CApplication* _pApp, CString _name) : application(_pApp), name(_name) {}

	int OnChange();
};

// For modifying alterable value properties
class CPluginPrivateVarMgr : public CPropValueMgr {
public:
	CObj* o;
	CObjType* pType;
	int valueIndex;
	CLayoutEditor* frame;

	CPluginPrivateVarMgr(CObj* _obj, CObjType* _type, int _valueIndex, CLayoutEditor* _frame)
		: o(_obj), pType(_type), valueIndex(_valueIndex), frame(_frame) {}

	int OnChange();
};

// For modifying plugin defined properties
class CPluginPropertyValueMgr : public CPropValueMgr {
public:
	CObj* obj;
	editInfo* data;
	CObjType* type;
	OINFO* oInfo;
	CString propName;
	CLayoutEditor* frame;
	int propIndex;

	CPluginPropertyValueMgr(CObj* _obj, editInfo* data, CObjType* _type, OINFO* _oInfo,
		CLayoutEditor* _frame, const CString& _propName);

	int OnChange();
};

// For modifying movement properties
class BehaviorPropertyValueMgr : public CPropValueMgr {
public:
	CObjType* pOwner;	// Pointer to type which contains this CBehavior
	CBehavior* pMov;	// Pointer to CBehavior to modify
	CLayout* level;
	CLayoutEditor* frame;
	CString propName;
	int propIndex;

	BehaviorPropertyValueMgr(CBehavior* _pMov, CObjType* _pOwner, CLayout* _level,
							CLayoutEditor* _frame, const CString& _propName);

	int OnChange();
};

// For modifying trait properties....not that there really are properties but i'm copying the movement system
class CTraitPropertyValueMgr : public CPropValueMgr {
public:
	CObjType* pOwner;	// Pointer to type which contains this CBehavior
	CString Trait;	// Pointer to CBehavior to modify
	CLayoutEditor* frame;
	CString propName;
	int propIndex;

	CTraitPropertyValueMgr(CString Trait, CObjType* _pOwner, CLayoutEditor* _frame, const CString& _propName);

	int OnChange();
};

// For modifying fx properties
class CEffectPropertyValueMgr : public CPropValueMgr {
public:
	list<CEffect>& effects;	// Pointer to effects list to use
	CEffect* pEffect;	// Pointer to CEffect to modify
	CObjType *pType;
	CLayer *pLayer;
	CLayoutEditor *frame;
	CString propName;
	int propIndex;

	CEffectPropertyValueMgr(CEffect* _pEffect, list<CEffect>& _effects, CObjType* _pType,
		CLayer* _pLayer, CLayoutEditor *frame, const CString& _propName);

	int OnChange();
};

#include "Utilities\ddmgr.h"
#include "..\Bars\LayerBar.h"
#include "..\Utilities\Undo.h"

#include "Structure.h"

#include "..\UI Elements\Controls\Richtooltipctrl.h"

void CreatePremultipliedThumbnail(CxImage* src, CxImage* dest, int width, int height, COLORREF background, bool checkers);
void CreatePremultipliedThumbnail(CxImage* src, CxImage* dest, int width, int height, RGBQUAD background, bool checkers);

class CLayoutEditor : public CScrollView
{
public: // create from serialization only
	CLayoutEditor();
	DECLARE_DYNCREATE(CLayoutEditor)
	void UpdateStatus();

	float RoundGridX(float value, bool isResizing = false /* when true, is a positioning instead */);
	float RoundGridY(float value, bool isResizing = false /* when true, is a positioning instead */);
	float GridWidth()  { return layout->m_GridWidth; }
	float GridHeight() { return layout->m_GridHeight; }
	bool InputLocked();

	void OnTimer(UINT nIDEvent) ;

	int scrolllock;
	void LockScrollUpdate();

	void UnlockScrollUpdate();
	
	// for scrolling while drag/select/resize
	UINT    m_nTimerID;
	DWORD	m_lastTick;
	
// Attributes
public:
	CConstructDoc*	GetDocument();
	CLayout*		layout;
	CApplication*	application;
	CDragDropMgr	ddm;
	CUndo*			m_undo;

	CString			m_AddedObject;

	void SerializeAllZLists(CArchive* ar);
	void GetListOfAllGroups(CObjList* dest,CObjList* src);
	void DeleteObjectRecursive(long object);
	int m_LastRibbon;
	CD3DDisplay**	m_display;
	bool m_bPanel;
	
	map<int, int>* m_image_to_texture;

	// Texture for layer rendering
	int* iLayerTexture;

	void InitializeObject(CObj* o);

	// Effect textures
	int* iOriginalTexture;
	int* iEffectTargets;
	int* iDisplayTexture;
	int* iPicture;
	//int* m_display_life;	//aaagh -AG
	int* iThumbnail;

	//CD3DDisplayFont m_defaultFont;
	int				scrollX, scrollY;

	bool			HasCreated;
	bool			doMove;

	bool			lastEnterPressed;

	CObjList		m_sel;
	void ToggleGrid();
	void EditGrid();
	void OnSnapMovement();
	void OnSnapResize();

	bool			m_UpdatePreview;	// Redraw layer thumbs?

	CPoint m_mouseDragStartPos;

// Operations
	void UpdateRulersInfo(int nMessage, CPoint ScrollPos, CPoint Pos = NULL);
	void DeleteSelection(bool omit_prompt = false);
	void DeleteObject(long object, BOOL bGroupUndo = FALSE);
	CString GetUnusedName(CString name);
	long GetUnusedID();
	long GetUnusedOID();
	CObj* CreateObject(const char* objectFile, CPoint point, CObj* fObj = NULL, long fOID = -1, BOOL bNewType = FALSE);
	CObj* CreateClonedObject(const char* objectFile, CPoint point, CObj* fObj, long fOID);
	CObj* CreateNewClonedObject(const char* objectFile, CPoint point, CObj *fObj);
	BOOL CreateAndInitialize(OINFO *oInfo, editInfo *editObject, CLayout *m_pLayout);
	CObj* GetObject(long instanceID);
	void SelectItemsInBox(UINT nFlags);
	void ResizeObject();
	void InitializeMove(CPoint point);
	void InitializeSelect(CPoint point);
	void CheckSizingHandles(CPoint point);
	void ArrayPaste(CArrayPasteDlg *apDlg);
	CRect GetSelRect();
	void Paste(CPoint point, BOOL bClone = FALSE);
	CLayer* GetObjectLayer(long object);
	CObjectRectangle GetTempChangedRect(CObj *o);
	CAction* PerformAction(CAction *action, BOOL bRepeat);
	void RotateSelection(long amt);
	void DrawObject(CObj *o, CPoint pOffset, bool unmoved, bool bOutline);
	long CheckGroup(CObj *o2, CPoint point);
	long GroupSel();
	void Ungroup(CObj* o);
	void RemoveFromGroup(CObj *o2);
	void AddToGroup(CObj *obj, CObj *group);
	void UpdateSelection(); // called when the selection of objects on the frame is changed
	void PasteDraggedObjects();

	// Layer boxes
	bool m_bLayerBoxes;
	void OnShowBoxes();

	void UpdateScrollbars();
	// Can auto increment ID when creating object
	bool m_AutoIncrement;
	int  m_LoopIteration;
	OINFO* m_Clone;

	vector<CString> Traits;

	// Tooltips
	CRichToolTipCtrl m_ToolTip;

public:
	virtual void OnFrameAction(UINT idCommand);
	virtual void OnInitialUpdate();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayoutEditor)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnClose();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnNewTemplate();
	virtual void OnToolbarDelete();
	virtual void OnToolbarCut();
	virtual void OnToolbarCopy();
	virtual void OnToolbarPaste();
	virtual void OnToolbarSelectAll();
	virtual void OnToolbarUnselect();
	virtual void OnToolbarInvertSelection();
	virtual void OnToolbarUndo();
	virtual void OnToolbarRedo();
	virtual void OnDestroy();
	virtual void AddLayout();
	virtual void RemoveLayout();
	virtual void OnAddEvent(UINT idCommand);
	virtual LRESULT OnExtMenuPrepareOneLevel(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	CSize CalcActualViewSize( CDC * pDC = NULL );
	CRect CalcActualViewRect( CDC * pDC = NULL );
	afx_msg LRESULT OnDrawRuler(WPARAM wParam, LPARAM lParam);
	
public:
	CRect m_rcTrackingCreation;
	bool m_bTrackingCreation:1;
	CMap < HWND, HWND, int, int > m_mapSelection;
		CSize m_sizeGridStep;

	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);

////////////////////
// CACHE
public:
	CString m_PriorObjectFile;
	OINFO*  m_PriorOINFO;

	int     m_PriorClonedObject;
	OINFO*  m_PriorClonedOINFO;

////////////////////
// BLOCK - Special Per View Scrolling code
public:
	CPoint ScrollPos;
	
	// if one of the views is activated we update the scrollbars
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView,
					CView* pDeactiveView)
	{

		if(bActivate)
		{
			this->SetRedraw(false);
			UpdateScrollbars();
			CScrollView::SetScrollPos(0,ScrollPos.x);
			CScrollView::SetScrollPos(1,ScrollPos.y);
			this->SetRedraw(true);
			RedrawWindow();
			UpdateScrollbars();
		}

/*		if(pActivateView)
		{
			if (pActivateView->GetScrollBarCtrl(0))
				pActivateView->GetScrollBarCtrl(0)->SetScrollPos(ScrollPos.x);
			if (pActivateView->GetScrollBarCtrl(1))
				pActivateView->GetScrollBarCtrl(1)->SetScrollPos(ScrollPos.y);
			UpdateScrollbars();
		}*/
		CScrollView::OnActivateView( bActivate,  pActivateView,
					 pDeactiveView);
		if(bActivate)
			Invalidate();//RedrawWindow();
		GetParent()->Invalidate();

	}
	CPoint GetScrollPosition()
	{
		//return CScrollView::GetScrollPosition();
		return ScrollPos;
	}
	int SetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE)
	{
		BOOL ret;
		ret = CScrollView::SetScrollPos(nBar,  nPos,  bRedraw);
		if(nBar ==0)
			ScrollPos.x = CScrollView::GetScrollPos(0);
		if(nBar ==1)
			ScrollPos.y = CScrollView::GetScrollPos(1);

		layout->scroll_position = ScrollPos;

		return ret;

	}
private:
	void CenterOnPoint(CPoint ptCenter)
	{
		CScrollView::CenterOnPoint( ptCenter);
		ScrollPos = CScrollView::GetScrollPosition();
		layout->scroll_position = ScrollPos;
	}
public:
	void ScrollToPosition(POINT pt)
	{
		CScrollView::ScrollToPosition( pt);
		ScrollPos = CScrollView::GetScrollPosition();
		if(!CScrollView::GetScrollBarCtrl(0)->IsWindowEnabled())
		{
			ScrollPos.x = 0;
			GetScrollBarCtrl(0)->SetScrollPos(0);
		}
		if(!CScrollView::GetScrollBarCtrl(1)->IsWindowEnabled())
		{
			ScrollPos.y = 0;
			GetScrollBarCtrl(1)->SetScrollPos(0);
		}
		layout->scroll_position = ScrollPos;

	}
//// END BLOCK


// Implementation
public:
	virtual ~CLayoutEditor();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	BOOL		m_bMouseDown;
	BOOL		m_bMovingObject;
	BOOL		m_bSizingObject;
	BOOL		m_bLayersAfterMove;
	long		m_objToSize;
	BOOL		m_bDraggingRect;
	int			m_sizeType;
	LPCTSTR		m_sizeCur;
	CPoint		m_oldPt;
	double		m_oldRatio;
	CPoint		m_curPt;
	BOOL		m_bRedrawBG;
	long		m_clickedOn;
	CPoint		m_centerPt;
	BOOL		m_bSetLayerList;
public:
	BOOL		m_bUpdateFrame;
private:
	bool		m_mdragging;
	bool		m_usermoved;
	POINT		m_mOld;
	DROPEFFECT	m_de;
	BOOL		m_bSuspendDrawing;
	BOOL		m_bCachedID;
	long		m_cachedID;
	int			m_last_rendered_layers_clock;

public:


	bool RenderEffectChain(list<CEffect*>& effects, int sourceTexture, int destTexture, D3DRECT& clearZone, D3DRECT& drawZone, RECTF& tbox, float hsX, float hsY);

	// For lost devices
	void CreateDirectXFrame();

	// Thumbnail texture

public:
	float m_Zoom;
	int m_zoom_val;
#define LYR_THUMB_W 66
#define LYR_THUMB_H 50

public:
	BOOL m_bInsert;

	// Frame editor coordinate adjusted draw functions
	void FrameFill(RECT rc, D3DCOLOR filter);
	void FrameBox(RECT rc, D3DCOLOR filter);
	void FrameBox(int width,RECT rc, D3DCOLOR filter);
	void FrameLine(int x1, int y1, int x2, int y2, D3DCOLOR color);

	unsigned int m_drawCount;
public:
	void SwitchEditor(void);
// Generated message map functions
public:
	//{{AFX_MSG(CLayoutEditor)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	protected:

	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGroups(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNewMovement(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNewEffect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRemoveMovements(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRemoveEffects(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSnapMovement(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSnapResize(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version
inline CConstructDoc* CLayoutEditor::GetDocument()
   { return (CConstructDoc*)m_pDocument; }
#endif
