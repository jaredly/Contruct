// Dialogs\EditGridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "EditGridDlg.h"

// CEditGridDlg dialog
IMPLEMENT_DYNAMIC(CEditGridDlg, CDialog)

CEditGridDlg::CEditGridDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CEditGridDlg::IDD, pParent)
{

}

CEditGridDlg::~CEditGridDlg()
{
}

void CEditGridDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_WIDTH, m_Width);
	DDX_Control(pDX, IDC_HEIGHT, m_Height);
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEditGridDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CEditGridDlg::OnOK)
END_MESSAGE_MAP()

// CEditGridDlg message handlers
BOOL CEditGridDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Width.SetWindowText(m_OldWidth);
	m_Height.SetWindowText(m_OldHeight);

	if (m_OldSnapTo)
		m_SnapTo.SetCheck(BST_CHECKED);

	SubclassChildControls();

	return TRUE;
}

void CEditGridDlg::OnOK()
{
	m_Width.GetWindowTextA(m_OldWidth);
	m_Height.GetWindowTextA(m_OldHeight);

	if (m_SnapTo.GetCheck() == BST_CHECKED)
		m_OldSnapTo = true;
	else
		m_OldSnapTo = false;
	
	CDialog::OnOK();
}
