#pragma once

// CFindReplaceDlg dialog
#include "..\resource.h"
#include "afxwin.h"
#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"

class CFindReplaceDlg : public CExtWS<CDialog>
{
	DECLARE_DYNAMIC(CFindReplaceDlg)

public:
	CFindReplaceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFindReplaceDlg();

// Dialog Data
	enum { IDD = IDD_FINDREPLACE };
	EventVector* m_pEventList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExtButton	m_Search;
	CExtButton	m_Bookmark;
	CExtButton	m_Replace;
	CExtButton	m_ReplaceAll;

	CButton		m_Actions;
	CButton		m_Conditions;

	CExtEdit	m_Find;
	CExtEdit	m_ReplaceWith;
	CListCtrl	m_List;
	EventSheetEditor* pChrono;

	// Search stuff
	CString m_Text;
	void DoSearch(CEditorEvent* pEvent, int& Number);

	// Handlers
	afx_msg void OnBnClickedSearch();
	afx_msg BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	CString StripHTML(LPCSTR String);
	afx_msg void OnNMDblclkResults(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBookmark();
	afx_msg void OnReplace();
	afx_msg void OnReplaceAll();
};
