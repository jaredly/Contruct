#if !defined(AFX_SETTINGSDLG_H__F73E1F0C_0A90_11D6_A046_0050BAAB2555__INCLUDED_)
#define AFX_SETTINGSDLG_H__F73E1F0C_0A90_11D6_A046_0050BAAB2555__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "PrefsStatic.h"

#define WM_SETTINGSDIALOG_CLOSE				WM_USER+5

typedef class _PageInfo		PAGE_INFO;

class _PageInfo
{
public:
	BOOL		bViewClass;			// View flag for runtime checking
	UINT		nID;				// Resource ID for the Page
	CWnd		*pWnd;				// Pointer to the page
	CWnd		*pWndParent;		// Pointer to the parent page if has
	CString		csCaption;			// Caption on the tree
	CString		csParentCaption;	// Caption of the parent on the tree
};

typedef CTypedPtrArray <CPtrArray, PAGE_INFO*>		PAGE_LIST;
typedef CMap<CWnd *, CWnd *, DWORD, DWORD&>			WNDTREE_MAP;

/////////////////////////////////////////////////////////////////////////////
// CSettingsDialog dialog

class CSettingsDialog : public CExtNCW<CExtResizableDialog>
{
// Construction
public:
	CSettingsDialog(CWnd* pParent = NULL);   // standard constructor
	~CSettingsDialog();

public:
	BOOL Create();
	void ExpandTree();
	BOOL DestroyPages();
	BOOL CreatePage( const PAGE_INFO *pInfo);
	void ShowPage(const PAGE_INFO *pInfo, UINT nShow = SW_SHOW);
	void SetLogoText(CString sText);
	void SetTitle(CString sTitle);
	HTREEITEM GetNextItemCOrS(HTREEITEM hItem);
	HTREEITEM FindItem(const CString &csCaption);
	HTREEITEM FindItem(CWnd *pWnd);
	CWnd* AddPage(CRuntimeClass *pWndClass, const char *pCaption, UINT nID,const char *pParentCaption);
	CWnd* AddPage(CRuntimeClass *pWndClass, const char *pCaption, UINT nID = 0,CWnd *pDlgParent = NULL);

// Dialog Data
	PAGE_LIST		m_pInfo;		// Containing page info
	CRect			m_FrameRect;	// Rectangle size of a setting page
	CString			m_csTitle;		// Title of selected page
	CString			m_csLogoText;	// Logo text
	WNDTREE_MAP		m_wndMap;		// MFC CMap class for internal page management
	CWnd*			m_pParent;		// Parent window to receive OK, Apply, Cancel message

// Dialog Data
	//{{AFX_DATA(CSettingsDialog)
	enum { IDD = IDD_SETTINGS_DLG };
	CStatic			m_PageFrame;
	CTreeCtrl		m_TreeCtrl;
	CPrefsStatic	m_CaptionBarCtrl;
	CExtButton m_OK;
	CExtButton m_Apply;
	CExtButton m_Cancel;
	//}}AFX_DATA


// Overrides
	virtual BOOL CreateWnd(CWnd *pWnd, CCreateContext *pContext = NULL);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingsDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ExpandBranch(HTREEITEM hti);
	void InitTreeCtrl();
	BOOL RefreshData();

	// Generated message map functions
	//{{AFX_MSG(CSettingsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetDispInfoTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnPreferenceHelp();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSDLG_H__F73E1F0C_0A90_11D6_A046_0050BAAB2555__INCLUDED_)
