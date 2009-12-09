#pragma once
#include "afxwin.h"

// CNewProjectDlg dialog
class CNewProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewProjectDlg)

public:
	CNewProjectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewProjectDlg();

// Dialog Data
	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	CComboBox m_Profile;
	CExtButton m_EditProfiles;
	CExtEdit m_Name;
	CExtButton m_OK;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedEditprofiles();
};
