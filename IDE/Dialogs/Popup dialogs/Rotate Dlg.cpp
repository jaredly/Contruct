#include "stdafx.h"
#include "..\..\Construct.h"
#include "Rotate Dlg.h"
#include "..\..\Editors\ImageEditor.h"

// CRotateDlg dialog
IMPLEMENT_DYNAMIC(CRotateDlg, CDialog)

CRotateDlg::CRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRotateDlg::IDD, pParent)
{
}

CRotateDlg::~CRotateDlg()
{
}

void CRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_ROTATE, m_Rotate);
	DDX_Control(pDX, IDC_ANGLE, m_Angle);
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRotateDlg, CDialog)
	ON_BN_CLICKED(IDC_ROTATE, &CRotateDlg::OnBnClickedRotate)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CRotateDlg message handlers
BOOL CRotateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_AngleIcons.Create(16, 16, ILC_COLOR24 | ILC_MASK, 3, 3);

	HBITMAP icon = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_ANGLEICONS));

	ImageList_Add(m_AngleIcons, icon, NULL);

	DeleteObject(icon);

	m_Angle.SetImageList(&m_AngleIcons);
	m_Angle.SetWindowText("0");

	for (int i = 0; i < 8; i++)
	{
		COMBOBOXEXITEM insItem;
		insItem.mask = CBEIF_IMAGE | CBEIF_TEXT | CBEIF_SELECTEDIMAGE; 
		insItem.iItem = i;
		insItem.iImage = i;
		insItem.iSelectedImage = i;

		CString Text;
		Text.Format("%d", i * 45);
		insItem.pszText = Text.GetBuffer(0);
		m_Angle.InsertItem(&insItem);
	}

	m_Angle.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRotateDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
		
	CRect rect;
	GetClientRect(&rect);

	dc.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(0, 0, 0));
	dc.FillSolidRect(1, 1, rect.Width() - 2, rect.Height() - 2, RGB(255, 255, 255));
}

void CRotateDlg::OnBnClickedRotate()
{
	CString text;
	m_Angle.GetWindowText(text);

	m_pParent->m_PicEd.Rotate(atoi(text));

	EndDialog(0);
}