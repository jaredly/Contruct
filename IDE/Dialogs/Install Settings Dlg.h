#pragma once
#include "afxwin.h"

class CInstallSettingsDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CInstallSettingsDlg)

public:
	CInstallSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInstallSettingsDlg();
	BOOL OnInitDialog();

	enum { IDD = IDD_INSTALLER };

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk();

	// Controls
	CExtButton		m_OK;
	CExtButton		m_Cancel;
	CEdit			m_License;
	CComboBox		m_OS;
	CButton			m_Fullscreen;
	CEdit			m_InstallPath;

	// Output
	CString		m_LicenseText;
	CString		m_Path;
	int			m_OSSetting;
	bool		m_bFullscreen;

	DECLARE_MESSAGE_MAP()
};
