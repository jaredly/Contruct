/////////////////////////////////////////////////////////////////////////////
//
// Disclaimer Notice
// ------------------
// The Author cannot guarantee this software is error free.
// The author also disclaims all responsibility for damages or 
// loss of profits caused by the failure of this software
//. Your usage of these modifications indicates your willingness to 
// accept complete responsibility.
//
//
// Developed by: Norm Almond for www.codeproject.com 
//
// Please use this software freely, if you fix or make any modifications to
// the software you can let me know so I can improve future versions.
//
// Version 1.00 - First Released 03 March 2001
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PALETTEWND_H__71DAEF19_5ED9_4537_9008_92F1F70D45CE__INCLUDED_)
#define AFX_PALETTEWND_H__71DAEF19_5ED9_4537_9008_92F1F70D45CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaletteWnd.h : header file
//


#define		CCN_COLOR_CHANGED		WM_APP + 0x1001
#define		CCN_CANCEL				WM_APP + 0x1002


/////////////////////////////////////////////////////////////////////////////
// CPaletteWnd window

class CPaletteWnd : public CWnd
{
// Construction
public:
	CPaletteWnd();

// Attributes
public:

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaletteWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create(CPoint pt, CWnd* pParent, UINT nID);
	virtual ~CPaletteWnd();
	void SetCustomColor(UINT nIndex, COLORREF crCustom);
	void  SetCurrentColor(COLORREF crCurrent);


protected:
	bool			m_bDrag;
	int				m_nIndex;
	BOOL			SelectColor(CPoint& pt);
	CRect			m_rc;
	CToolTipCtrl	m_wndToolTip;
	void			CreateToolTips();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPaletteWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_PALETTEWND_H__71DAEF19_5ED9_4537_9008_92F1F70D45CE__INCLUDED_)
