#pragma once
#include "afxcmn.h"

#include "..\UI Elements\XListCtrl\XListCtrl.h"
#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"

// CControlsDlg dialog

class CControlsDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CControlsDlg)

	CApplication* application;
	CStringArray controls;
	CStringArray players;

	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;

public:
	CControlsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CControlsDlg();

	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnComboSelection(WPARAM, LPARAM);

	void RefreshControls();
	void DeleteCurSel();
	CExtButton m_Add, m_Remove, m_Edit, m_Close;

// Dialog Data
	enum { IDD = IDD_MANAGECONTROLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	CXListCtrl m_Ctrls;

	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnLvnKeydownControlslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRename();
	afx_msg void OnDestroy();
};
