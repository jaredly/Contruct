#if !defined(AFX_CAPTIONEDITXP_H__A88559DD_B3AC_4A50_B055_CC9AB2BB9B87__INCLUDED_)
#define AFX_CAPTIONEDITXP_H__A88559DD_B3AC_4A50_B055_CC9AB2BB9B87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// captioneditxp.h : header file
//

#include "CColourBox.h"
#include "..\Editors\CPictureEditor.h"
#include "EditSpinSlider.h"

/////////////////////////////////////////////////////////////////////////////
// CCaptionEditXP window

class COutlineFillOption:	public CDialog
{
public:
	CRect m_Outline;
	CRect m_Fill;
	CRect m_OutlineFill;
	int result;
	CPictureEditor* pPicEd;

// Operations
public:

	BOOL Create(DWORD dwStyle,CRect WndSize, CWnd* pParentWnd, UINT nID)
	{
		result = 0;
		return CDialog::CreateEx(0,0, NULL, dwStyle,WndSize, pParentWnd, nID);
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaptionEditXP)

	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnPaint();
};




class CCaptionEditXP : public CDialog
{
// Construction
public:
	CCaptionEditXP();

// Attributes
public:

// Operations
public:

	BOOL Create(DWORD dwStyle,CRect WndSize, CWnd* pParentWnd, UINT nID)
	{//AfxRegisterWndClass(CS_VREDRAW)

/*		CDialog::Create(IDD_PALETTE,pParentWnd);
		ModifyStyle(GetStyle(),dwStyle);
		//this->MoveWindow(WndSize,true);
		return false;*/
	
		return CDialog::CreateEx(0,0, NULL, dwStyle,WndSize, pParentWnd, nID);
	//	return CEdit::Create(dwStyle,WndSize,pParentWnd,nID);
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaptionEditXP)

	//}}AFX_VIRTUAL

// Implementation
public:
	CPictureEditor* pPicEd;
//	CPictureEditor* pPicEd;
#define ID_BRUSHSIZE 0
#define ID_BRUSHHARDNESS 1
#define ID_BRUSHTHICKNESS 2
#define ID_BRUSHANGLE 3
#define ID_BRUSHSTEP 4
#define ID_LINETHICKNESS 5
#define ID_OPACITY 6
#define ID_FLOW 7
#define ID_ALPHA1 8
#define ID_ALPHA2 9

	int m_type;
	CEditSpinSlider m_Edit;
	
	CFont m_EditFont;
	CFont m_CaptionFont;
	CString m_CaptionString;



	virtual ~CCaptionEditXP();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCaptionEditXP)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseOut();
	afx_msg	void OnChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAPTIONEDITXP_H__A88559DD_B3AC_4A50_B055_CC9AB2BB9B87__INCLUDED_)
