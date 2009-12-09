// FirstStartDlg.cpp : First start dialog
//

/////////////////////////////////////////////////////////////////////////////
// Insert object Dialog
//
// Last revision: 0.7
// 24th April 2006
//
// Changes:
// - Started
//
// Todo:
// - Save other properties
//
/////////////////////////////////////////////////////////////////////////////

// File includes
#include "stdafx.h"
#include "..\Construct.h"
#include "FirstStartDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFirstStartDlg dialog


CFirstStartDlg::CFirstStartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFirstStartDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFirstStartDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CFirstStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFirstStartDlg)
	DDX_Control(pDX, IDC_INFOTEXT, m_Information);
	DDX_Control(pDX, IDC_HHELP, m_Help);
	DDX_Control(pDX, IDC_DONE, m_Done);
	DDX_Control(pDX, IDC_LANGUAGE, m_Language);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFirstStartDlg, CDialog)
	//{{AFX_MSG_MAP(CFirstStartDlg)
	ON_BN_CLICKED(IDC_DONE, OnDone)
	ON_CBN_SELCHANGE(IDC_LANGUAGE, OnChangeLanguage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFirstStartDlg message handlers

void CFirstStartDlg::OnDone() 
{
	m_Language.GetLBText(m_Language.GetCurSel(), chosenLanguage);

	if (IsDlgButtonChecked(IDC_MAXIMIZE) != 0)
		showMaximized = "1";

	EndDialog(0);
}

BOOL CFirstStartDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Default settings
	showMaximized = "0";

	m_Language.SetCurSel(0);
	OnChangeLanguage();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFirstStartDlg::OnChangeLanguage() 
{
	// Retrieve language
	m_Language.GetLBText(m_Language.GetCurSel(), chosenLanguage);

	// Load INI texts
	// Title of dialog
	CString dialogTitle;
//	dialogTitle.Format("Construct : %s", GetStrFromFile(chosenLanguage, FIRST_START_WIZARD));
	SetWindowText(dialogTitle);

	// Information text
//	m_Information.SetWindowText(GetStrFromFile(chosenLanguage, FIRST_START_WIZARD_INFO));
}
