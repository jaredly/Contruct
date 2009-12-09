#pragma once

#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"

/////////////////////////////////////////////////////////////////////////////
// CNewBehaviorDlg dialog

class CNewBehaviorDlg : public CExtNCW<CExtResizableDialog>
{
// Construction
public:
	CNewBehaviorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewBehaviorDlg)
	enum { IDD = IDD_NEWMOVEMENT };
	CListCtrl	m_List;
	CExtButton	m_Cancel;
	CExtButton	m_OK;
	CExtEdit	m_Desc;

	vector<int> behaviors;

	//}}AFX_DATA

	CImageList  m_Images;

	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewBehaviorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CString Text;
	int     ID;

// Implementation
protected:

	afx_msg void OnSize(UINT nType, int cx, int cy);

	// Generated message map functions
	//{{AFX_MSG(CNewBehaviorDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelChange(NMHDR* pNM, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};