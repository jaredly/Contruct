#if !defined(AFX_TELLTALEBUTTON_H__95D6E790_7385_4538_B21E_E88E4978575C__INCLUDED_)
#define AFX_TELLTALEBUTTON_H__95D6E790_7385_4538_B21E_E88E4978575C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TelltaleButton.h : header file
//

#define		WM_RUBRIC_WND_CLICKED_ON		( WM_APP + 04100 )

/////////////////////////////////////////////////////////////////////////////
// CTelltaleButton window

class CTelltaleButton : public CButton
{
// Construction
public:
	CTelltaleButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTelltaleButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTelltaleButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTelltaleButton)
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELLTALEBUTTON_H__95D6E790_7385_4538_B21E_E88E4978575C__INCLUDED_)
