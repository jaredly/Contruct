#if !defined(AFX_IMAGEEDITOR_H__C16E09CB_9680_496F_BE18_2EEBD92813DF__INCLUDED_)
#define AFX_IMAGEEDITOR_H__C16E09CB_9680_496F_BE18_2EEBD92813DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageEditor.h : header file
//

// Includes needed
#include "..\LIBs\Include\ExtToolControlBar.h"

#include "..\Utilities\Palette\ToolSettings.h"
#include "..\Utilities\Palette\ColorMixer.h"
#include "..\Utilities\Palette\PaletteDlg.h"
#include "CPictureEditor.h"
#include "..\Dialogs\Popup dialogs\Rotate Dlg.h"

/////////////////////////////////////////////////////////////////////////////
// CImageEditor view

class CPaletteBar : public CExtPanelControlBar
{

public:
	CPaletteBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaletteBar();

public:

	CPaletteDlg	m_wndChild;


	// Generated message map functions
protected:
	//{{AFX_MSG(CMyBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



class CImageEditor : public CScrollView
{
	friend class CImageEditorDlg;

public:
	CImageEditor();          // protected constructor used by dynamic creation
		
	void UpdateCursor();
	DECLARE_DYNCREATE(CImageEditor)

// Attributes
public:
	CLayout*			m_pLayout;
	CApplication*		m_app;
	CImageEditorDlg*	m_pImgEdDlg;

	CRotateDlg			m_RotateDlg;

// Operations
public:
	void OnFileOpen(CString path);
	void OnEditPaste();
	void OnEditSelectAll();
	void EditImage(CxImage* MyImage, CxImage* MyCollision = NULL);
	int m_MouseY;
	int m_MouseX;
	void UpdateScrollbars();
	void UpdateTool();
	void ChangeTool();
	CD3DDisplay *m_display;
	CPictureEditor m_PicEd;
	int m_zoom_val;
	CRect OldWindowRect;
	bool m_Shiftheld;
	bool m_Controlheld;


	int m_linesize;
	int m_type;
	CPoint m_MouseOrigin;
	CPoint m_MouseLast;
	int m_oldtool;
	bool m_mousedown;
	int m_tool;
	int m_SelShift;

	CExtToolControlBar* m_pMain;
	CExtToolControlBar* m_pTools;
	CToolSettings* m_pTool_Settings;
	CColorMixer* m_pColor_Mixer;

	CPaletteBar* m_pPaletteBar;

	bool m_usermoved;
	bool drawingDisabled;	// for exceptions


	friend class CImageEditor;
// Overrides

	//{{AFX_VIRTUAL(CImageEditor)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CImageEditor();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageEditor)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSelectFree();
	afx_msg void OnSelectPolygon();	
	afx_msg void OnSelectRectangle();	
	afx_msg void OnSelectEllipse();
	afx_msg void OnSelectWand();
	afx_msg void OnSelectColor();
	afx_msg void OnZoom();	
	afx_msg void OnPick();
	afx_msg void OnPen();
	afx_msg void OnBrush();
	afx_msg void OnCustomBrush();
	afx_msg void OnTube();
	afx_msg void OnErasor();
	afx_msg void OnText();
	afx_msg void OnFill();
	afx_msg void OnArrow();
	afx_msg void OnLine();
	afx_msg void OnRectangle();
	afx_msg void OnPolygon();
	afx_msg void OnEllipse();
	afx_msg void OnHotspot();
	afx_msg void OnRotate();
	afx_msg void OnActionpoint();
	afx_msg void OnBackground();

	afx_msg void OnResize();
	afx_msg void OnUndo();
	afx_msg void OnRedo();
	afx_msg void OnMirror();
	afx_msg void OnFlip();
	afx_msg void OnWrap();
	afx_msg void OnMask();
	afx_msg void OnGrid();

	afx_msg void OnNew();
	afx_msg void OnCut();
	afx_msg void OnCopy();
	afx_msg void OnPaste();
	afx_msg void OnCrop();






	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnIdleUpdateCmdUI();
	
	//}}AFX_MSG

	void OnEnterIdle(UINT nWhy, CWnd* pWho) ;








	DECLARE_MESSAGE_MAP()







};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEEDITOR_H__C16E09CB_9680_496F_BE18_2EEBD92813DF__INCLUDED_)
