// Dialogs\PreferencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "PreferencesDlg.h"

// CPreferencesDlg dialog
IMPLEMENT_DYNAMIC(CPreferencesDlg, CDialog)

CPreferencesDlg::CPreferencesDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CPreferencesDlg::IDD, pParent)
{

}

CPreferencesDlg::~CPreferencesDlg()
{
}

void CPreferencesDlg::DoDataExchange(CDataExchange* pDX)
{
//	DDX_Control(pDX, IDC_PGC, m_PGC);
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPreferencesDlg, CDialog)
END_MESSAGE_MAP()

// CPreferencesDlg message handlers
BOOL CPreferencesDlg::OnInitDialog() 
{
	CExtNCW<CExtResizableDialog>::OnInitDialog();

	return TRUE;
}

void CPreferencesDlg::OnOK()
{
	CDialog::OnOK();
}