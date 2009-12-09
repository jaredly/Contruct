#if !defined(AFX_TEMPLATEDIALOG_H__954523CF_A5FC_4979_ADD1_F021222DB65C__INCLUDED_)
#define AFX_TEMPLATEDIALOG_H__954523CF_A5FC_4979_ADD1_F021222DB65C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// TemplateDialog dialog

class TemplateDialog : public CExtNCW<CExtResizableDialog>
{
// Construction
public:
	TemplateDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TemplateDialog)
	enum { IDD = IDD_NTEMPLATE };
	CListCtrl	m_Templates;
	CEdit		m_Name;
	CExtButton	m_Create;
	CExtButton	m_Cancel;
	CEdit		m_Author;
	//}}AFX_DATA

	CImageList imageList;
	void OnDblClkTemplates(NMHDR *pNMHDR, LRESULT *pResult);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TemplateDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TemplateDialog)
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCreate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEMPLATEDIALOG_H__954523CF_A5FC_4979_ADD1_F021222DB65C__INCLUDED_)
