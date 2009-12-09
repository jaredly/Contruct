// PicEdResizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PicEdResizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPicEdResizeDlg dialog


CPicEdResizeDlg::CPicEdResizeDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CPicEdResizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPicEdResizeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPicEdResizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPicEdResizeDlg)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_Resize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPicEdResizeDlg, CDialog)
	//{{AFX_MSG_MAP(CPicEdResizeDlg)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_WIDTH,EditWidth)
	ON_EN_CHANGE(IDC_WIDTH_PERCENTAGE,EditWidthPercentage)
	ON_EN_CHANGE(IDC_HEIGHT,EditHeight)
	ON_EN_CHANGE(IDC_HEIGHT_PERCENTAGE,EditHeightPercentage)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicEdResizeDlg message handlers

int CPicEdResizeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CPicEdResizeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString MyString;
	MyString.Format("%d",m_pPicEd->m_Width);
    GetDlgItem(IDC_WIDTH)->SetWindowText(MyString);

	MyString.Format("%d",m_pPicEd->m_Height);
    GetDlgItem(IDC_HEIGHT)->SetWindowText(MyString);

	// Cosmetics
	m_Cancel.SetWindowText(CANCEL);
	m_Resize.SetWindowText(RESIZE);

	SubclassChildControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPicEdResizeDlg::EditWidth()
{
	CEdit* myEditWidth = (CEdit*)GetDlgItem(IDC_WIDTH);

	CString MyText;
	myEditWidth->GetWindowText(MyText);
	int width = atoi(MyText);
}

void CPicEdResizeDlg::EditHeight()
{

}

void CPicEdResizeDlg::EditWidthPercentage()
{

}

void CPicEdResizeDlg::EditHeightPercentage()
{

}

void CPicEdResizeDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CString MyString;	
	GetDlgItem(IDC_WIDTH)->GetWindowText(MyString);
	m_width = atoi(MyString);

	GetDlgItem(IDC_HEIGHT)->GetWindowText(MyString);
	m_height = atoi(MyString);
	

	CDialog::OnClose();
}

void CPicEdResizeDlg::OnDestroy() 
{

	
	CString MyString;	
	GetDlgItem(IDC_WIDTH)->GetWindowText(MyString);
	m_width = atoi(MyString);

	GetDlgItem(IDC_HEIGHT)->GetWindowText(MyString);
	m_height = atoi(MyString);
		
	
	
	
	
	
	
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
