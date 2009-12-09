#if !defined(AFX_VIEWTEXTDLG_H__79697C3C_C1C6_421F_87E7_A1D9F1547910__INCLUDED_)
#define AFX_VIEWTEXTDLG_H__79697C3C_C1C6_421F_87E7_A1D9F1547910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewTextDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewTextDlg dialog

class CViewTextDlg : public CDialog
{
// Construction
public:
	CViewTextDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL OnInitDialog();

	EditExt* pExt;

// Dialog Data
	//{{AFX_DATA(CViewTextDlg)
	enum { IDD = IDD_VIEWTEXT };
	CEdit	m_Edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewTextDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewTextDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWTEXTDLG_H__79697C3C_C1C6_421F_87E7_A1D9F1547910__INCLUDED_)
