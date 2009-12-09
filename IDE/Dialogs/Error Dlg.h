#if !defined(AFX_ERRORDLG_H__3C03032F_EC4C_4D67_83F1_5FA2001EA085__INCLUDED_)
#define AFX_ERRORDLG_H__3C03032F_EC4C_4D67_83F1_5FA2001EA085__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ErrorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CErrorDlg dialog

class CErrorDlg : public CExtWS<CDialog>
{
// Construction
public:
	CErrorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CErrorDlg)
	enum { IDD = IDD_ERROR };
	CExtButton m_OK;
	CEdit m_Message;
	CExtButton m_Cancel;
	//}}AFX_DATA

	// Call error
	CString errorTitle;
	CString errorMessage;
	bool update;

	bool Error(LPCSTR title, LPCSTR message)
	{
		errorTitle = title;
		errorMessage = message;
		update = false;

		DoModal();

		return true;
	}
	INT_PTR UpdateMessage(LPCSTR title, LPCSTR message);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CErrorDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CErrorDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORDLG_H__3C03032F_EC4C_4D67_83F1_5FA2001EA085__INCLUDED_)
