/////////////////////////////////////////////////////////////////////////////
// Install Settings Dialog
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Construct.h"
#include "Install Settings Dlg.h"

// CInstallSettingsDlg dialog
IMPLEMENT_DYNAMIC(CInstallSettingsDlg, CDialog)

CInstallSettingsDlg::CInstallSettingsDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CInstallSettingsDlg::IDD, pParent)
{
	m_OSSetting = 0;
}

CInstallSettingsDlg::~CInstallSettingsDlg()
{
}

void CInstallSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_LICENSE, m_License);
	DDX_Control(pDX, IDC_OS, m_OS);
	DDX_Control(pDX, IDC_FULLSCREEN, m_Fullscreen);
	DDX_Control(pDX, IDC_PATH, m_InstallPath);
}

BEGIN_MESSAGE_MAP(CInstallSettingsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInstallSettingsDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CInstallSettingsDlg message handlers
BOOL CInstallSettingsDlg::OnInitDialog() 
{
	// Startup
	CDialog::OnInitDialog();

	m_OS.SetCurSel(m_OSSetting);
	m_License.SetWindowText(m_LicenseText);
	m_InstallPath.SetWindowText(m_Path);
	if (m_bFullscreen) m_Fullscreen.SetCheck(BST_CHECKED);

	SubclassChildControls();

	return TRUE;
}
void CInstallSettingsDlg::OnBnClickedOk()
{
	m_License.GetWindowText(m_LicenseText);
	m_InstallPath.GetWindowText(m_Path);
	m_OSSetting = m_OS.GetCurSel();
	if (m_Fullscreen.GetCheck() == BST_CHECKED) m_bFullscreen = true;
	else m_bFullscreen = false;

	OnOK();
}
