// AutoSave.cpp : implementation file
//

#include "StdAfx.h"
#include "..\..\Construct.h"
#include "AutoSaveOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\..\Utilities\Ini.h"
extern CIni* pINI;

/////////////////////////////////////////////////////////////////////////////
// CAutoSave property page

IMPLEMENT_DYNCREATE(CAutoSave, CPropertyPage)

CAutoSave::CAutoSave() : CPropertyPage(CAutoSave::IDD)
{
	//{{AFX_DATA_INIT(CAutoSave)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAutoSave::~CAutoSave()
{
}

void CAutoSave::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_AUTOSAVE, m_AutoSave);
	DDX_Control(pDX, IDC_AUTOBACKUP, m_AutoBackup);
	DDX_Control(pDX, IDC_AUTOBACKUPAUTOSAVE, m_AutoBackupAutoSaves);
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoSave)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_THEME, m_ThemeCombo);
}

BEGIN_MESSAGE_MAP(CAutoSave, CPropertyPage)
	//{{AFX_MSG_MAP(CAutoSave)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_THEME, &CAutoSave::OnCbnSelchangeTheme)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoSave message handlers

void CAutoSave::OnClear() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CAutoSave::OnSetActive() 
{
	if (!pINI->IsKeyExist("AutoSave", "Time"))
		pINI->WriteInt("AutoSave", "Time", 10);	// 10 mins default autosave time

	if (!pINI->IsKeyExist("AutoBackup", "NumBackups"))
		pINI->WriteInt("AutoBackup", "NumBackups", 5);

	SetDlgItemInt(IDC_TIME, pINI->GetInt("AutoSave", "Time", 0), true);
	SetDlgItemInt(IDC_NUMBACKUPS, pINI->GetInt("AutoBackup", "NumBackups", 0), true);
	if (pINI->GetInt("AutoSave", "On", 0) == 1) m_AutoSave.SetCheck(BST_CHECKED);
	if (pINI->GetInt("AutoBackup", "On", 0) == 1) m_AutoBackup.SetCheck(BST_CHECKED);
	if (pINI->GetInt("AutoBackup", "AutoSaves", 0) == 1) m_AutoBackupAutoSaves.SetCheck(BST_CHECKED);

	m_ThemeCombo.ResetContent();
	m_ThemeCombo.AddString("XP");
	m_ThemeCombo.AddString("Native XP");
	m_ThemeCombo.AddString("Office 2003");
	m_ThemeCombo.AddString("Visual Studio 2005");
	m_ThemeCombo.AddString("Visual Studio 2008");
	m_ThemeCombo.AddString("Office 2007 Blue (unfinished)");
	m_ThemeCombo.AddString("Office 2007 Silver (unfinished)");
	m_ThemeCombo.AddString("Office 2007 Black (unfinished)");
	m_ThemeCombo.SetCurSel(pINI->GetInt("General", "Theme", 4));

	return CPropertyPage::OnSetActive();
}

BOOL CAutoSave::OnKillActive() 
{
	if(IsDlgButtonChecked(IDC_AUTOSAVE))
		pINI->WriteInt("AutoSave", "On", 1);
	else
		pINI->WriteInt("AutoSave", "On", 0);

	if(IsDlgButtonChecked(IDC_AUTOBACKUP))
		pINI->WriteInt("AutoBackup", "On", 1);
	else
		pINI->WriteInt("AutoBackup", "On", 0);

	if(IsDlgButtonChecked(IDC_AUTOBACKUPAUTOSAVE))
		pINI->WriteInt("AutoBackup", "AutoSaves", 1);
	else
		pINI->WriteInt("AutoBackup", "AutoSaves", 0);

	pINI->WriteInt("AutoSave", "Time", GetDlgItemInt(IDC_TIME, false, true));
	pINI->WriteInt("AutoBackup", "NumBackups", GetDlgItemInt(IDC_NUMBACKUPS, false, true));

	return CPropertyPage::OnKillActive();
}

void CAutoSave::OnCbnSelchangeTheme()
{
	pINI->WriteInt("General", "Theme", m_ThemeCombo.GetCurSel());
}
