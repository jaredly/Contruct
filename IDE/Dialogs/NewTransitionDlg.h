#pragma once

// CNewTransitionDlg dialog

#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"

class CNewTransitionDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CNewTransitionDlg)

public:
	// Transition
	CTransition m_Transition;

	CNewTransitionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewTransitionDlg();
	virtual BOOL OnInitDialog();
	CListCtrl m_Transitions;
	void OnOK();

	CExtButton m_OK, m_Cancel;

	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;

// Dialog Data
	enum { IDD = IDD_NEWTRANSITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
