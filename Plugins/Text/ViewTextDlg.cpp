// ViewTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "constructsdk.h"
#include "ViewTextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTextDlg dialog


CViewTextDlg::CViewTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewTextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewTextDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CViewTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewTextDlg)
	DDX_Control(pDX, IDC_TEXT, m_Edit);
	//}}AFX_DATA_MAP
}

BOOL CViewTextDlg::OnInitDialog()
{
	BOOL ret = CDialog::OnInitDialog();

	// Load extension text
	m_Edit.SetWindowText(pExt->text);

	return ret;
}


BEGIN_MESSAGE_MAP(CViewTextDlg, CDialog)
	//{{AFX_MSG_MAP(CViewTextDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTextDlg message handlers

void CViewTextDlg::OnOK() 
{
	// Copy text to object
	m_Edit.GetWindowText(pExt->text);
	
	CDialog::OnOK();
}
