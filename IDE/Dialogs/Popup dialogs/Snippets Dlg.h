#pragma once

class CScriptDlg;

class CSnippetsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSnippetsDlg)

public:
	CSnippetsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSnippetsDlg();

	CListCtrl	m_Snippets;
	CExtButton	m_Insert;
	CScriptDlg* m_pParent;

	vector<CString> m_SnippetTexts;

// Dialog Data
	enum { IDD = IDD_SNIPPETS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnPaint() ;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedInsert();
	afx_msg void OnNMDblclkResults(NMHDR *pNMHDR, LRESULT *pResult);
};


#include "..\ScriptDlg.h"