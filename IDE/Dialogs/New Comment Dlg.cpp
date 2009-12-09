/////////////////////////////////////////////////////////////////////////////
// Expanded Comment Dialog
//
// Last revision: 1.0
// September 12th 2008
//
/////////////////////////////////////////////////////////////////////////////

// File includes
#include "stdafx.h"
#include "..\Construct.h"
#include "AddCommentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddCommentDlg dialog

CAddCommentDlg::CAddCommentDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CAddCommentDlg::IDD, pParent)
{
	memset(&lf, 0, sizeof(LOGFONT));
	
	colour = RGB(255, 255, 200);
	lf.lfHeight = 14;
	strcpy(lf.lfFaceName, "Arial");
}

void CAddCommentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddCommentDlg)
	DDX_Control(pDX, IDC_FONT, m_Font);
	DDX_Control(pDX, IDC_COMMENT, m_Comment);
	DDX_Control(pDX, IDC_COLOR, m_Color);
	DDX_Control(pDX, IDC_CLOSE, m_Close);
	DDX_Control(pDX, IDC_ADD, m_Add);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddCommentDlg, CExtResizableDialog)
	//{{AFX_MSG_MAP(CAddCommentDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_FONT, OnFont)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddCommentDlg message handlers

BOOL CAddCommentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Add.SetWindowText("OK");
	m_Close.SetWindowText(CANCEL);
	m_Color.SetWindowText(C_COLOUR);
	m_Font.SetWindowText(C_FONT);

	m_Comment.SetWindowTextA(theComment);

	// Languages
	CString title;
	title.Format("Construct : %s", EE_EDITCOMMENT);
	SetWindowText(title);

	// Add resizing
	dlgMan.Load(this->m_hWnd, "Software\\Construct\\CommentDlg");
    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDC_COLOR, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_FONT, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_ADD, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_CLOSE, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_COMMENT, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddCommentDlg::OnAdd() 
{
	// Get comment
	m_Comment.GetWindowText(theComment);

	CDialog::OnOK();	
}

void CAddCommentDlg::OnClose() 
{
	EndDialog(0);
}

void CAddCommentDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dlgAnchor.OnSize();	

	Invalidate();
}

void CAddCommentDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	dlgMan.Save();
}

void CAddCommentDlg::OnFont() 
{
	CFontDialog fontDlg(&lf);
	fontDlg.DoModal();
	fontcolour = fontDlg.GetColor();	
}

void CAddCommentDlg::OnColor() 
{
	CColorDialog dlg;
	dlg.DoModal();
	colour = dlg.GetColor();	
}
