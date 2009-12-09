/////////////////////////////////////////////////////////////////////////////
// Export Wizard Dialog
/////////////////////////////////////////////////////////////////////////////

// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "Export Wizard Dlg.h"
#include "..\Install Settings Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern vector<CString> g_PythonLib;

////////////////////////////////////////////////////////////////////////////
// CExportWizardDlg dialog

CExportWizardDlg::CExportWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportWizardDlg::IDD, pParent)
{
	m_iStep = 1;
}

void CExportWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BACK, m_Back);
	DDX_Control(pDX, IDC_NEXT, m_Next);
	DDX_Control(pDX, IDC_CANCEL, m_Cancel);

	// Step 1
	DDX_Control(pDX, IDC_ENABLEPYTHON, m_EnablePython);
	DDX_Control(pDX, IDC_COPYDLL, m_CopyDLL);
	DDX_Control(pDX, IDC_S1T1, m_Text1);
	DDX_Control(pDX, IDC_MODULES, m_Modules);

	// Step 2
	DDX_Control(pDX, IDC_S2T1, m_Text2);
	DDX_Control(pDX, IDC_EXPORTPATH, m_Output);
	DDX_Control(pDX, IDC_BROWSE, m_Browse);
	DDX_Control(pDX, IDC_RUNAFTER, m_RunAfter);
	DDX_Control(pDX, IDC_INSTALLER, m_Installer);
	DDX_Control(pDX, IDC_INSTALLSETTINGS, m_InstallSettings);
	DDX_Control(pDX, IDC_SCREENSAVER, m_Screensaver);
}

BEGIN_MESSAGE_MAP(CExportWizardDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()

	ON_BN_CLICKED(IDC_NEXT, &CExportWizardDlg::OnBnClickedNext)
	ON_BN_CLICKED(IDC_BACK, &CExportWizardDlg::OnBnClickedBack)
	ON_BN_CLICKED(IDC_CANCEL, &CExportWizardDlg::OnBnClickedCancel)

	ON_BN_CLICKED(IDC_BROWSE, &CExportWizardDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_INSTALLSETTINGS, &CExportWizardDlg::OnBnClickedInstallSettings)

	ON_BN_CLICKED(IDC_ENABLEPYTHON, OnCheckUsePython)

	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportWizardDlg message handlers

BOOL CExportWizardDlg::OnInitDialog() 
{
	// Startup
	CDialog::OnInitDialog();

	//SubclassChildControls();

	// Resizing anchors
	m_DlgManager.Load(this->m_hWnd, "Software\\Construct\\aExportWizardDialogaa");
    m_DlgAnchor.Init(this->m_hWnd);

	m_DlgAnchor.Add(IDC_BACK, ANCHOR_BOTTOM | ANCHOR_RIGHT);
	m_DlgAnchor.Add(IDC_CANCEL, ANCHOR_BOTTOM | ANCHOR_LEFT);
	m_DlgAnchor.Add(IDC_NEXT, ANCHOR_BOTTOM | ANCHOR_RIGHT);
	
	m_Cancel.SetWindowText(CANCEL);

	m_iStep = 1;

	// New drawing code
	LOGFONT* m_pLF;
	m_pLF=(LOGFONT*)calloc(1,sizeof(LOGFONT));
	strncpy(m_pLF->lfFaceName,"Arial",6);
	m_pLF->lfHeight = 14;
	m_pLF->lfWeight = 600;
	m_pLF->lfItalic = 0;
	m_pLF->lfUnderline = 0;
	m_FontBold.CreateFontIndirect(m_pLF);

	// Window title
	SetWindowText("Construct : Export Wizard");

	// Default settings
	if (application->game_information.use_python) m_EnablePython.SetCheck(BST_CHECKED);
	m_InstallPath.Format("#PROGRAMFILES#%s\\%s\\", application->file_information.creator, application->file_information.name);
	m_OS = 0;

	// Can this be a screensaver?
	if (application->runtime == CApplication::rt_application)
		m_Screensaver.EnableWindow(FALSE);

	// Add python modules
	for(int i = 0; i < g_PythonLib.size(); i++)
	{
		CString File = g_PythonLib.at(i);
		int Index = m_Modules.AddString(File);
	
		if(application->m_PythonDistrib.find(File) != application->m_PythonDistrib.end())
			m_Modules.SetCheck(Index, TRUE);
	}

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExportWizardDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_DlgAnchor.OnSize();

	Invalidate();
}

// Save dialog settings
void CExportWizardDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	m_DlgManager.Save();	
}

void CExportWizardDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);

	dc.FillSolidRect(0, 0, rect.Width(), 45, RGB(255, 255, 255));
	dc.FillSolidRect(0, 46, rect.Width(), 1, RGB(0, 0, 0));

	dc.FillSolidRect(0, rect.bottom - 40, rect.Width(), 1, RGB(158, 158, 158));
	dc.FillSolidRect(0, rect.bottom - 39, rect.Width(), 1, RGB(255, 255, 255));

	CFont* pOldFont = NULL;

	if (m_iStep == 1)
	{
		pOldFont = dc.SelectObject(&m_FontBold);
		dc.DrawText("Python settings.", CRect(7, 7, 200, 21), 0);
	}

	if (m_iStep == 2)
	{
		pOldFont = dc.SelectObject(&m_FontBold);
		dc.DrawText("Export and install settings.", CRect(7, 7, 200, 21), 0);
	}

	if (pOldFont) dc.SelectObject(pOldFont);
}

void CExportWizardDlg::OnBnClickedNext()
{
	Invalidate();

	if (m_iStep == 2)
	{
		// Save variables and finish up
		m_Output.GetWindowText(m_OutputPath);

		if (m_OutputPath == "")
		{
			CErrorDlg Dlg;
			Dlg.Error("Error exporting", "You must enter a legitimate path to export to.");
			return;
		}

		// Installer
		if (m_Installer.GetCheck() == BST_CHECKED) m_bInstaller = true;
		else m_bInstaller = false;

		// Enable python
		if (m_EnablePython.GetCheck() == BST_CHECKED) application->game_information.use_python = true;
		else application->game_information.use_python = false;

		// Screensaver
		if (m_Screensaver.GetCheck() == BST_CHECKED) m_bScreensaver = true;
		else m_bScreensaver = false;

		// Save python modules
		application->m_PythonDistrib.clear();

		for(int i = 0; i < m_Modules.GetCount(); i++)
		{
			CString File;
			m_Modules.GetText(i, File);

			if(m_Modules.GetCheck(i))
				application->m_PythonDistrib[File] = 0;
		}

		OnOK();
	}

	if (m_iStep == 1)
		ChangeStep(2);
}

void CExportWizardDlg::OnBnClickedBack()
{
	Invalidate();

	if (m_iStep == 2)
		ChangeStep(1);
}

void CExportWizardDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CExportWizardDlg::ChangeStep(int iStep)
{
	if (m_iStep == 2 && iStep == 1)
	{
		m_iStep = 1;
		UpdateButtons();

		// Controls
		// Hide step 2
		m_Text2.ShowWindow(SW_HIDE);
		m_Output.ShowWindow(SW_HIDE);
		m_RunAfter.ShowWindow(SW_HIDE);
		m_Installer.ShowWindow(SW_HIDE);
		m_Browse.ShowWindow(SW_HIDE);
		m_InstallSettings.ShowWindow(SW_HIDE);
		m_Screensaver.ShowWindow(SW_HIDE);

		// Show step 1
		m_EnablePython.ShowWindow(SW_SHOW);
		m_CopyDLL.ShowWindow(SW_SHOW);
		m_Text1.ShowWindow(SW_SHOW);
		m_Modules.ShowWindow(SW_SHOW);
	}

	if (m_iStep == 1 && iStep == 2)
	{
		m_iStep = 2;
		UpdateButtons();

		// Controls
		// Hide step 1
		m_EnablePython.ShowWindow(SW_HIDE);
		m_CopyDLL.ShowWindow(SW_HIDE);
		m_Text1.ShowWindow(SW_HIDE);
		m_Modules.ShowWindow(SW_HIDE);

		// Show step 2
		m_Text2.ShowWindow(SW_SHOW);
		m_Output.ShowWindow(SW_SHOW);
		m_RunAfter.ShowWindow(SW_SHOW);
		m_Installer.ShowWindow(SW_SHOW);
		m_Browse.ShowWindow(SW_SHOW);
		m_InstallSettings.ShowWindow(SW_SHOW);
		m_Screensaver.ShowWindow(SW_SHOW);
	}
}

void CExportWizardDlg::UpdateButtons()
{
	if(m_iStep == 1)
	{
		m_Back.EnableWindow(FALSE);
		m_Next.SetWindowTextA("Next");
	}

	else if (m_iStep == 2)
	{
		m_Next.SetWindowText("Finish");
		m_Next.EnableWindow();
		m_Back.EnableWindow();
	}
}

void CExportWizardDlg::OnCheckUsePython()
{
	if (m_EnablePython.GetCheck() == BST_CHECKED)
	{
		m_CopyDLL.EnableWindow();
		m_Modules.EnableWindow();
	}

	else
	{
		m_CopyDLL.EnableWindow(FALSE);
		m_Modules.EnableWindow(FALSE);
	}
}

void CExportWizardDlg::OnBnClickedBrowse()
{
	char PopupFilters[] = "EXE Files (*.exe)|*.exe||";

	CFileDialog FileDlg(false, "Executable files", ".exe", OFN_OVERWRITEPROMPT, PopupFilters, NULL);
	if (FileDlg.DoModal() != IDOK) OnCancel();;

	// Get path
	m_OutputPath = FileDlg.GetPathName();
	m_Output.SetWindowText(FileDlg.GetPathName());
}

void CExportWizardDlg::OnBnClickedInstallSettings()
{
	CInstallSettingsDlg Dlg;
	Dlg.m_LicenseText	= m_License;
	Dlg.m_bFullscreen	= m_bFullscreen;
	Dlg.m_OSSetting		= m_OS;
	Dlg.m_Path			= m_InstallPath;

	if (Dlg.DoModal() == IDOK)
	{
		// Save settings
		m_License		= Dlg.m_LicenseText;
		m_OS			= Dlg.m_OSSetting;
		m_bFullscreen	= Dlg.m_bFullscreen;
		m_InstallPath	= Dlg.m_Path;
	}		
}