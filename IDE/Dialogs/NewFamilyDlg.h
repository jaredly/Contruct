#pragma once

// CNewFamilyDlg dialog

#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"

class CNewFamilyDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CNewFamilyDlg)

public:
	CNewFamilyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewFamilyDlg();

	CListCtrl m_List;
	CExtButton m_OK, m_Cancel, m_Manage;
	CImageList m_Images;
	CApplication* application;

	// Name and icon of chosen family
	CString m_Name;
	CxImage small_image;
	CxImage m_Image;

	CString m_Path;

	// Resizable
	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;   

// Dialog Data
	enum { IDD = IDD_NEWFAMILY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void OnOK();
	BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	void OnRefresh();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedManage();
public:
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};
