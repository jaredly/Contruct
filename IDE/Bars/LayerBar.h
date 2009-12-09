#pragma once

#include "..\stdafx.h"
#include "..\Utilities\ddmgr.h"

#include "..\resource.h"
class CLayoutEditor;
class CLayout;

#define CLR_BG RGB(255,255,255)
#define CLR_SEL RGB(0,0,128)

/////////////////////////////////////////////////////////////////////////////
// CLayerListBox window

class CLayerListBox : public CDragListBox
{
// Construction
public:
	CLayerListBox();
	~CLayerListBox();

// Attributes
public:
	CLayoutEditor* layout_editor;
	CLayout* layout;
	CFont m_Font;
	CImageList imglist;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayerListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL BeginDrag(CPoint pt);
	virtual void Dropped(int nSrcIndex, CPoint pt);
	virtual UINT Dragging(CPoint pt);
	//}}AFX_VIRTUAL
	// Generated message map functions
protected:
	//{{AFX_MSG(CLayerListBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP dropInfo);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CLayerDlg dialog

class CLayerDlg : public CExtResizableDialog
{
// Construction
public:
	CLayerDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_BARDIALOG };

	CLayoutEditor* layout_editor;
	void RefreshLayers();

	CExtToolControlBar m_Toolbar;
	CLayerListBox m_layerListBox;
	CToolTipCtrlEx toolTip;

	// Images for preview
	map<CLayer*, PreviewImage> images;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLayerAdd();
	afx_msg void OnLayerAddDown();
	afx_msg void OnLayerUp();
	afx_msg void OnLayerDown();
	afx_msg void OnLayerDel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchange();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnLockLayer();
	afx_msg void OnHideLayer();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CLayerBar window

class CLayerBar : public CExtControlBar
{
// Construction
public:
	CLayerBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayerBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLayerBar();
	CLayerDlg m_layerDlg;

	// Generated message map functions
protected:
	//{{AFX_MSG(CLayerBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
