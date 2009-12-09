/////////////////////////////////////////////////////////////////////////////
// Object Information Dialog
//
// Last revision: 1.0
// 21st March 2008
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "..\Construct.h"
#include "Object Info Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CEmptyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CEmptyListCtrl)
	ON_WM_PAINT()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CEmptyListCtrl::OnPaint() 
{
    Default();
    if (GetItemCount() <= 0)
    {
        COLORREF clrText = ::g_PaintManager->GetColor(COLOR_WINDOWTEXT);
        COLORREF clrTextBk = ::g_PaintManager->GetColor(COLOR_WINDOW);

        CDC* pDC = GetDC();
        // Save dc state
        int nSavedDC = pDC->SaveDC();

        CRect rc;
        GetWindowRect(&rc);
        ScreenToClient(&rc);

        CHeaderCtrl* pHC;
        pHC = GetHeaderCtrl();
        if (pHC != NULL)
        {
            CRect rcH;
            pHC->GetItemRect(0, &rcH);
            rc.top += rcH.bottom;
        }
        rc.top += 10;

        CString strText; // The message you want!
		strText = OI_NONE;

        pDC->SetTextColor(clrText);
        pDC->SetBkColor(clrTextBk);
        pDC->FillRect(rc, &CBrush(clrTextBk));
        pDC->SelectObject(&m_Font);
        pDC->DrawText(strText, -1, rc, 
                      DT_CENTER | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP);

        // Restore dc
        pDC->RestoreDC(nSavedDC);
        ReleaseDC(pDC);
    }
    // Do not call CListCtrl::OnPaint() for painting messages
}

/////////////////////////////////////////////////////////////////////////////
// CObjectInfoDlg dialog
CObjectInfoDlg::CObjectInfoDlg(CWnd* pParent /*=NULL*/) : CExtNCW<CExtResizableDialog>(CObjectInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CObjectInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectInfoDlg)
	DDX_Control(pDX, IDCLOSE, m_Close);
	DDX_Control(pDX, IDC_ACE, m_AceType);
	DDX_Control(pDX, IDC_ACELIST, m_AceList);
	DDX_Control(pDX, IDC_INFO, m_Info);
	DDX_Control(pDX, IDC_OICON, m_Icon);
	DDX_Control(pDX, IDC_NAME, m_Name);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CObjectInfoDlg, CExtResizableDialog)
	//{{AFX_MSG_MAP(CObjectInfoDlg)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_ACE, OnChangeACE)
	ON_BN_CLICKED(IDCLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectInfoDlg message handlers

BOOL CObjectInfoDlg::OnInitDialog() 
{
	CExtResizableDialog::OnInitDialog();

	// Resizing
	dlgMan.Load(this->m_hWnd, "Software\\Construct\\ObjectInformaitDlg");
    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDCLOSE, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_ACE, ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_TOP | ANCHOR_RIGHT);
	dlgAnchor.Add(IDC_ACELIST, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(IDCLOSE, ANCHOR_BOTTOM | ANCHOR_RIGHT);
	dlgAnchor.Add(IDC_INFO, ANCHOR_TOP | ANCHOR_RIGHT | ANCHOR_LEFT);
	dlgAnchor.Add(IDC_BOX2, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(IDC_OICON, ANCHOR_TOP | ANCHOR_LEFT);
	dlgAnchor.Add(IDC_NAME, ANCHOR_TOP | ANCHOR_LEFT | ANCHOR_RIGHT);

	// Languages
	CString title;
	title.Format("Construct : %s", OI_TITLE);
	SetWindowText(title);

	SetDlgItemText(IDC_BOX2, OI_ACEINFORMATION);

	m_AceType.AddString(OI_ACTIONS);
	m_AceType.AddString(OI_CONDITIONS);
	m_AceType.AddString(OI_EXPRESSIONS);

	m_AceList.InsertColumn(0, PROPERTIES_TITLE, LVCFMT_LEFT, 105, NULL);
	m_AceList.InsertColumn(1, OI_PARAMETERS, LVCFMT_LEFT, 70, NULL);
	m_AceList.InsertColumn(2, OI_DESCRIPTION, LVCFMT_LEFT, 155, NULL);
	m_AceList.SetExtendedStyle(LVS_EX_FULLROWSELECT	| LVS_EX_GRIDLINES);

	OINFO *oInfo;
	POSITION pos = g_dlls->GetStartPosition();
	DWORD nKey;

	// Find the DLL
	while (pos != NULL) 
	{
		g_dlls->GetNextAssoc(pos, nKey, oInfo);
	
		// Got it
		if (oInfo->extName == objname)
		{
			// New, use CPlugin
			CPlugin Plugin = GetPluginByFile(oInfo->extFileName);
		
			CString Text;
			Text.Format("Author: %s\r\nVersion: %s\r\nWeb link: %s\r\nDescription: %s", Plugin.m_Author, Plugin.m_Version, Plugin.m_WebLink, Plugin.m_Description);
			m_Info.SetWindowText(Text);

			// Set icon
			m_Icon.SetBitmap(Plugin.m_pInfo->hLargeIcon);
			m_Name.SetWindowText(Plugin.m_Name);

			break;
		}
	}

	m_AceType.SetCurSel(0);
	OnChangeACE();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CObjectInfoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dlgAnchor.OnSize();	
}

void CObjectInfoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	dlgMan.Save();	
}

void CObjectInfoDlg::OnChangeACE() 
{
	int acenum = m_AceType.GetCurSel();
	int acetype = 0;
	int table = 0;

	if (acenum == 0) table = 0;
	if (acenum == 1) table = 1;
	if (acenum == 2) table = 2;

	m_AceList.DeleteAllItems();

	int numaces;
	OINFO *oInfo;
	POSITION pos = g_dlls->GetStartPosition();
	DWORD nKey;

	// Find the DLL
	while (pos != NULL) 
	{
		g_dlls->GetNextAssoc(pos, nKey, oInfo);

		// Got it
		if (oInfo->extName == objname)
		{
			// Get the number of table entries
			numaces = oInfo->GetTable(table)->size();

			// Loop each ACE entry
			for (int i = 0; i < numaces; i++)
			{	
				// Get the pointer to current ACE
				ACESEntry2* pACE = oInfo->GetACEEntry(table, i);

				if(pACE == NULL || pACE->aceListName == "")
					continue; // Ignore null entries

				// Add name
				// Check for * (new)
				int item = -1;

				if (pACE->aceListName.Left(1) == "*")
					item = m_AceList.InsertItem(i, pACE->aceListName.Right(pACE->aceListName.GetLength() - 1), 0);
				else
					item = m_AceList.InsertItem(i, pACE->aceListName, 0);

				// Add number of parameters
				CString numberOfParameters;
				numberOfParameters.Format("%d", pACE->params.size());
				m_AceList.SetItemText(item, 1, numberOfParameters);

				// Add description
				m_AceList.SetItemText(item, 2, pACE->aceDisplayText);
			}

			break;
		}
	}
}

void CObjectInfoDlg::OnClose() 
{
	EndDialog(0);
}
