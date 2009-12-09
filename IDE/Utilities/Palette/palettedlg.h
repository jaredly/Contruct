#if !defined(AFX_PALETTEDLG_H__D38209FA_C483_4512_9442_F26658C5AD6D__INCLUDED_)
#define AFX_PALETTEDLG_H__D38209FA_C483_4512_9442_F26658C5AD6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaletteDlg.h : header file
//
#include "..\..\Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPaletteDlg dialog
class CImageEditor;

class CPaletteDlg : public CDialog
{
// Construction
public:
	COLORREF* pColor1;
	COLORREF* pColor2;
	CImageEditor* m_pImageView;
	bool m_right;
	bool m_left;
	bool m_shiftheld;
	int color1;
	int color2;
	CPaletteDlg(CWnd* pParent = NULL);   // standard constructor
	COLORREF Palette[256];
// Dialog Data
	//{{AFX_DATA(CPaletteDlg)
	enum { IDD = IDD_PALETTE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	friend class CImageEditor;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaletteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPaletteDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALETTEDLG_H__D38209FA_C483_4512_9442_F26658C5AD6D__INCLUDED_)
