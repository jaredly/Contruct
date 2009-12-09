#include "stdafx.h"
#include "..\Construct.h"
#include "New Behavior Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool BehaviorSortFunction(const OINFO* a, const OINFO* b)
{
	CString l = a->extName;
	CString r = b->extName;

	l.MakeLower();
	r.MakeLower();

	return l < r;
}

/////////////////////////////////////////////////////////////////////////////
// CNewBehaviorDlg dialog

CNewBehaviorDlg::CNewBehaviorDlg(CWnd* pParent /*=NULL*/)
		: CExtNCW<CExtResizableDialog>(CNewBehaviorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewBehaviorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CNewBehaviorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewBehaviorDlg)
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDC_INFO, m_Desc);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewBehaviorDlg, CDialog)
	//{{AFX_MSG_MAP(CNewBehaviorDlg)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CNewBehaviorDlg::OnNMDblclkList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CNewBehaviorDlg::OnSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewBehaviorDlg message handlers

BOOL CNewBehaviorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Add movements to dialog
	POSITION pos = g_dlls->GetStartPosition();
	DWORD    nKey;
	OINFO*   oInfo;

	m_Images.Create(32, 32, ILC_COLOR24, 3, 3);
	ListView_SetImageList(m_List.m_hWnd, m_Images.m_hImageList, LVSIL_NORMAL);

	vector<OINFO*> sorted;

	while (pos != NULL)
	{ 
		// locate the object DLL to use
		g_dlls->GetNextAssoc(pos, nKey, oInfo);

		if (oInfo->ideFlags & OF_MOVEMENTPLUGIN) 
		{
			sorted.push_back(oInfo);
		}
	}

	sort(sorted.begin(), sorted.end(), BehaviorSortFunction);

	for (int i = 0; i < sorted.size(); i++)
	{
		int icon = ImageList_Add(m_Images.m_hImageList, sorted[i]->hLargeIcon, NULL);

		int item = m_List.InsertItem(m_List.GetItemCount(), sorted[i]->extName, icon);
		behaviors.push_back(sorted[i]->oID);
	}

	// Resizing
	dlgMan.Load(this->m_hWnd, "Software\\Construct\\NewBehaviorDialog");
    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDC_LIST, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(IDOK, ANCHOR_RIGHT | ANCHOR_TOP);
	dlgAnchor.Add(IDCANCEL, ANCHOR_RIGHT | ANCHOR_TOP);
	dlgAnchor.Add(IDC_INFO, ANCHOR_BOTTOM | ANCHOR_LEFT);

	// Change title
	CString Title;
	Title.Format("Construct : %s", PROPERTIES_NEWMOVEMENT);
	SetWindowText(Title);

	m_Cancel.SetWindowText(CANCEL);
	m_OK.SetWindowText(INSERT);
	m_OK.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewBehaviorDlg::OnOK() 
{
	// Get text
	POSITION Position = m_List.GetFirstSelectedItemPosition();
	int sel           = m_List.GetNextSelectedItem(Position);

	if (sel == -1) return;

	Text = m_List.GetItemText(sel, 0);
	Text.Replace(" ", "");
	Text.Replace("&", "");

	ID = behaviors[sel];
	
	CDialog::OnOK();
}

void CNewBehaviorDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	dlgMan.Save();
}

void CNewBehaviorDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	OnOK();
}

void CNewBehaviorDlg::OnSelChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	// Get text
	POSITION Position = m_List.GetFirstSelectedItemPosition();
	int sel           = m_List.GetNextSelectedItem(Position);

	if (sel == -1) 
	{
		m_OK.EnableWindow(FALSE);
		return;
	}

	CPlugin plugin = GetPlugin(behaviors[sel]);

	m_OK.EnableWindow(TRUE);

	// build string to show
	CString Text;
	Text.Format("%s: %s\r\n%s: %s\r\n%s: %s", IO_AUTHOR, plugin.m_Author, IO_VERSION, plugin.m_Version, IO_DESCRIPTION, plugin.m_Description);
	m_Desc.SetWindowText(Text);
}

void CNewBehaviorDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dlgAnchor.OnSize();

	Invalidate();
}