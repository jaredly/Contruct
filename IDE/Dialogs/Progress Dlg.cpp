// Dialogs\ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "ProgressDlg.h"


// CProgressDlg dialog

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

CProgressDlg::CProgressDlg(CString status, CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CProgressDlg::IDD, pParent)
{
	m_StatusText = status;
	progress = 0;	
}

void CProgressDlg::Start(CString status)
{
	if (status != "")
		m_StatusText = status;

	Create(IDD_PROGRESSDLG);
	SetProgress(0);
}

BOOL CProgressDlg::OnInitDialog()
{
	BOOL ret = CDialog::OnInitDialog();

	CenterWindow();
	ShowWindow(SW_SHOW);

	CString status;
	status.Format("%s... %d%%", m_StatusText, progress);
	m_Status.SetWindowText(status);
	m_Status.Invalidate();
	m_Progress.SetRange(0, 100);
	m_Progress.SetPos(0);
	m_Progress.Invalidate();

	SubclassChildControls();

	return ret;
}

void CProgressDlg::SetProgress(int prog)
{
	CString status;
	status.Format("%s... %d%%", m_StatusText, prog);
	m_Status.SetWindowText(status);
	m_Status.Invalidate();
	m_Progress.SetPos(prog);
}

void CProgressDlg::Finish()
{
	DestroyWindow();
}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATUS, m_Status);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
END_MESSAGE_MAP()


// CProgressDlg message handlers
