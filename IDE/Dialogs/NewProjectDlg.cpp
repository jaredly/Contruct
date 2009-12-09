// NewProjectDlg.cpp : implementation file
#include "stdafx.h"
#include "..\Construct.h"
#include "NewProjectDlg.h"

// CNewProjectDlg dialog
IMPLEMENT_DYNAMIC(CNewProjectDlg, CDialog)

CNewProjectDlg::CNewProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjectDlg::IDD, pParent)
{

}

CNewProjectDlg::~CNewProjectDlg()
{
}

void CNewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROFILE, m_Profile);
	DDX_Control(pDX, IDC_EDITPROFILES, m_EditProfiles);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDOK, m_OK);
}

BEGIN_MESSAGE_MAP(CNewProjectDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CNewProjectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_EDITPROFILES, &CNewProjectDlg::OnBnClickedEditprofiles)
END_MESSAGE_MAP()

// CNewProjectDlg message handlers
BOOL CNewProjectDlg::OnInitDialog()
{
	BOOL ret = CDialog::OnInitDialog();

	//SubclassChildControls();

	return ret;
}

void CNewProjectDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CNewProjectDlg::OnBnClickedEditprofiles()
{
	// TODO: Add your control notification handler code here
}
