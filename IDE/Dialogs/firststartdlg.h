#if !defined(AFX_FIRSTSTARTDLG_H__A1230339_3B23_4025_8C20_265A5D6BE323__INCLUDED_)
#define AFX_FIRSTSTARTDLG_H__A1230339_3B23_4025_8C20_265A5D6BE323__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FirstStartDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFirstStartDlg dialog

class CFirstStartDlg : public CDialog
{
// Construction
public:
	CFirstStartDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFirstStartDlg)
	enum { IDD = IDD_FIRSTSTART };
	CStatic	m_Information;
	CExtButton	m_Help;
	CExtButton	m_Done;
	CExtComboBox	m_Language;
	//}}AFX_DATA

	CString chosenLanguage;
	CString showMaximized;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFirstStartDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFirstStartDlg)
	afx_msg void OnDone();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeLanguage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIRSTSTARTDLG_H__A1230339_3B23_4025_8C20_265A5D6BE323__INCLUDED_)
