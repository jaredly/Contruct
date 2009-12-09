// ACE Filter Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\Construct.h"
#include "ACE Filter Dlg.h"
#include "..\Object Info Dlg.h"

extern CPluginMap g_Plugins;
 
// CACEFilterDlg dialog
IMPLEMENT_DYNAMIC(CACEFilterDlg, CDialog)

CACEFilterDlg::CACEFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CACEFilterDlg::IDD, pParent)
{
	m_bGame = true;
}

CACEFilterDlg::~CACEFilterDlg()
{
}

void CACEFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_SEARCH, m_Search);
	DDX_Control(pDX, IDC_RESULTS, m_Results);
	DDX_Control(pDX, IDC_FILTER, m_Filter);
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CACEFilterDlg, CDialog)
	ON_BN_CLICKED(IDC_FILTER, &CACEFilterDlg::OnBnClickedFilter)
	ON_WM_PAINT()
	ON_NOTIFY(NM_DBLCLK, IDC_RESULTS, &CACEFilterDlg::OnNMDblclkResults)
END_MESSAGE_MAP()

// CACEFilterDlg message handlers
BOOL CACEFilterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Filter.SetWindowText(IO_SEARCH);

	// Image list
	m_Images.Create(16, 16, ILC_COLOR24, 4, 4);

	map<int, CPlugin>::iterator i = g_Plugins.begin();

	for ( ; i != g_Plugins.end(); i++) 
	{
		if (!i->second.m_bMovement && i->second.m_Name != "Tag")
		{
			if (!m_bGame && i->second.m_Category == OT_GAME)
				continue;

			CBitmap Bitmap;
			Bitmap.Attach(i->second.m_SmallIcon);
			m_Images.Add(&Bitmap, &Bitmap);
			Bitmap.Detach();
		}
	}

	m_Results.SetImageList(&m_Images, LVSIL_SMALL);

	m_Results.InsertColumn(0, IO_NAME, LVCFMT_LEFT, 82);
	m_Results.InsertColumn(1, IO_RESULT, LVCFMT_LEFT, 152);
			
	m_Results.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CACEFilterDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
		
	CRect rect;
	GetClientRect(&rect);

	dc.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(0, 0, 0));
	dc.FillSolidRect(1, 1, rect.Width() - 2, rect.Height() - 2, RGB(255, 255, 255));
}

void CACEFilterDlg::OnBnClickedFilter()
{
	// Clear results
	m_Results.DeleteAllItems();

	// Search term
	CString Search;
	m_Search.GetWindowText(Search);
	Search.MakeLower();

	CString Found;

	map<int, CPlugin>::iterator i = g_Plugins.begin();

	int Count = 0;

	for ( ; i != g_Plugins.end(); i++) 
	{
		if (!i->second.m_bMovement && i->second.m_Name != "Tag")
		{
			if (!m_bGame && i->second.m_Category == OT_GAME)
				continue;

			// We meet again, Mr. Plugin
			// First check is the name..
			CString Test = i->second.m_Name;
			Test.MakeLower();

			if (Test.Find(Search) != -1)
			{
				int Item = m_Results.InsertItem(m_Results.GetItemCount(), i->second.m_Name, Count);
				m_Results.SetItemText(Item, 1, IO_NAME);
				m_Results.SetItemData(Item, i->second.m_ID);
			}

			// Check description
			Test = i->second.m_Description;
			Test.MakeLower();

			if (Test.Find(Search) != -1)
			{
				int Item = m_Results.InsertItem(m_Results.GetItemCount(), i->second.m_Name, Count);
				m_Results.SetItemText(Item, 1, IO_DESCRIPTION);
				m_Results.SetItemData(Item, i->second.m_ID);
			}

			// Check author
			Test = i->second.m_Author;
			Test.MakeLower();

			if (Test.Find(Search) != -1)
			{
				int Item = m_Results.InsertItem(m_Results.GetItemCount(), i->second.m_Name, Count);
				m_Results.SetItemText(Item, 1, IO_AUTHOR);
				m_Results.SetItemData(Item, i->second.m_ID);
			}

			// Now go through all the ACEs
			vector<ACESEntry2>* pConditions = i->second.m_pInfo->GetTable(1);
			for (int j = 0; j < pConditions->size(); j++)
			{
				bool bAdd = false;

				Test = pConditions->at(j).aceListName;
				Test.MakeLower();
				if (Test.Find(Search) != -1)
					bAdd = true;

				Test = pConditions->at(j).aceDisplayText;
				Test.MakeLower();
				if (Test.Find(Search) != -1)
					bAdd = true;

				Test = pConditions->at(j).scriptName;
				Test.MakeLower();
				if (Test.Find(Search) != -1)
					bAdd = true;

				if (bAdd)
				{
					CString Add;
					Add.Format("ACE: %s", pConditions->at(j).scriptName);
					int Item = m_Results.InsertItem(m_Results.GetItemCount(), i->second.m_Name, Count);
					m_Results.SetItemText(Item, 1, Add);
					m_Results.SetItemData(Item, i->second.m_ID);
				}
			}

			vector<ACESEntry2>* pActions = i->second.m_pInfo->GetTable(0);
			for (int j = 0; j < pActions->size(); j++)
			{
				bool bAdd = false;

				Test = pActions->at(j).aceListName;
				Test.MakeLower();
				if (Test.Find(Search) != -1)
					bAdd = true;

				Test = pActions->at(j).aceDisplayText;
				Test.MakeLower();
				if (Test.Find(Search) != -1)
					bAdd = true;

				Test = pActions->at(j).scriptName;
				Test.MakeLower();
				if (Test.Find(Search) != -1)
					bAdd = true;

				if (bAdd)
				{
					CString Add;
					Add.Format("ACE: %s", pActions->at(j).scriptName);
					int Item = m_Results.InsertItem(m_Results.GetItemCount(), i->second.m_Name, Count);
					m_Results.SetItemText(Item, 1, Add);
					m_Results.SetItemData(Item, i->second.m_ID);
				}
			}

			vector<ACESEntry2>* pExpressions = i->second.m_pInfo->GetTable(2);
			for (int j = 0; j < pExpressions->size(); j++)
			{
				bool bAdd = false;

				Test = pExpressions->at(j).aceListName;
				Test.MakeLower();
				if (Test.Find(Search) != -1)
					bAdd = true;

				Test = pExpressions->at(j).aceDisplayText;
				Test.MakeLower();
				if (Test.Find(Search) != -1)
					bAdd = true;

				Test = pExpressions->at(j).scriptName;
				Test.MakeLower();
				if (Test.Find(Search) != -1)
					bAdd = true;

				if (bAdd)
				{
					CString Add;
					Add.Format("ACE: %s", pExpressions->at(j).scriptName);
					int Item = m_Results.InsertItem(m_Results.GetItemCount(), i->second.m_Name, Count);
					m_Results.SetItemText(Item, 1, Add);
					m_Results.SetItemData(Item, i->second.m_ID);
				}
			}

			Count++;
		}
	}
}

void CACEFilterDlg::OnNMDblclkResults(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	POSITION Pos = m_Results.GetFirstSelectedItemPosition();
	int Item = m_Results.GetNextSelectedItem(Pos);
	if (Item == -1) return;

	// Open object information dialog
	CObjectInfoDlg Dlg;
	Dlg.objname = m_Results.GetItemText(Item, 0);
	Dlg.DoModal();
}
