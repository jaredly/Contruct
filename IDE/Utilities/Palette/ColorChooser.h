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


#if !defined(AFX_COLORCHOOSER_H__D0F786BD_F34A_4798_88BB_B2E3DD1DC78C__INCLUDED_)
#define AFX_COLORCHOOSER_H__D0F786BD_F34A_4798_88BB_B2E3DD1DC78C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorChooser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorChooser window

#include "PaletteWnd.h"

// Mask use for detect system color - sets COLORREF hi order bit
#define		SYSTEM_COLOR_MASK		0x80000000
#define		WEB_COLOR_MASK			0x40000000


#define		COLOR_CHANGED			0x0001


typedef struct tagCOLOR_NOTIFY 
{
    NMHDR   hdr;
    COLORREF color;
} COLOR_NOTIFY;



class CColorChooser : public CWnd
{
// Construction
public:
	CColorChooser();
	CColorChooser(CPoint pt, CWnd *pWnd, COLORREF crCurrent);

protected:

	CPaletteWnd		m_wndPalette;
	CWnd*			m_pWndParent;
	CWnd*			m_pWndSelect;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorChooser)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create(CPoint pt, CWnd* pWnd, COLORREF crCurrent);

	virtual ~CColorChooser();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorChooser)
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaletteColorChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSystemColorChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnWebColorChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORCHOOSER_H__D0F786BD_F34A_4798_88BB_B2E3DD1DC78C__INCLUDED_)
