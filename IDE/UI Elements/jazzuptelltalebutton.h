#if !defined(AFX_JAZZUPTELLTALEBUTTON_H__C8C95B2E_2FD5_481E_891E_80333E7C65B9__INCLUDED_)
#define AFX_JAZZUPTELLTALEBUTTON_H__C8C95B2E_2FD5_481E_891E_80333E7C65B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JazzUpTellTaleButton.h : header file
//

#define		WM_RUBRIC_WND_CLICKED_ON		( WM_APP + 04100 )

/////////////////////////////////////////////////////////////////////////////
// CJazzUpTellTaleButton window

class CJazzUpTellTaleButton : public CExtButton
{
// Construction
public:
	CJazzUpTellTaleButton();

// Attributes
public:

// Operations
public:
	void		SetIcon		(HICON nID );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJazzUpTellTaleButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CJazzUpTellTaleButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CJazzUpTellTaleButton)
	afx_msg void OnClicked();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JAZZUPTELLTALEBUTTON_H__C8C95B2E_2FD5_481E_891E_80333E7C65B9__INCLUDED_)
