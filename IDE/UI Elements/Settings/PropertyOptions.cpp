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
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoSave)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAutoSave, CPropertyPage)
	//{{AFX_MSG_MAP(CAutoSave)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoSave message handlers

void CAutoSave::OnClear() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CAutoSave::OnSetActive() 
{
	SetDlgItemInt(IDC_TIME, pINI->GetInt("AutoSave", "Time", 0), true);
	if (pINI->GetInt("AutoSave", "On", 0) == 1) m_AutoSave.SetCheck(BST_CHECKED);

	return CPropertyPage::OnSetActive();
}

BOOL CAutoSave::OnKillActive() 
{
	if(IsDlgButtonChecked(IDC_AUTOSAVE))
		pINI->WriteInt("AutoSave", "On", 1);
	else
		pINI->WriteInt("AutoSave", "On", 0);

	pINI->WriteInt("AutoSave", "Time", GetDlgItemInt(IDC_TIME, false, true));

	return CPropertyPage::OnKillActive();
}