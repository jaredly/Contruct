#pragma once

#include "..\Structure.h"

#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"

// CManageFamiliesDlg dialog

class CManageFamiliesDlg : public CExtNCW<CExtResizableDialog>
{
	//DECLARE_DYNAMIC(CManageFamiliesDlg)

public:
	CManageFamiliesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManageFamiliesDlg();

// Dialog Data
	enum { IDD = IDD_MANAGEFAMILIES };

	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;
	CApplication* application;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CExtButton m_Add, m_Remove, m_Edit, m_Close, m_Cancel;

	bool FamilyNameExists(CString name);

	BOOL OnInitDialog();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedRename();
	CListCtrl m_List;

	afx_msg void OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
	void ConfirmDeleteSelectedItems();

	CEdit m_Name;

	void RefreshFamilies();
	CImageList m_FamilyImages;

	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};
