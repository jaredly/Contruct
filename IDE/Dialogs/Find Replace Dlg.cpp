// Dialogs\FindReplaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "FindReplaceDlg.h"
//#include "atlrx.h"
#include "..\Editors\Event Sheet Editor\Event Sheet Editor.h"

// CFindReplaceDlg dialog
IMPLEMENT_DYNAMIC(CFindReplaceDlg, CDialog)

CFindReplaceDlg::CFindReplaceDlg(CWnd* pParent /*=NULL*/) 
: CExtWS<CDialog>(CFindReplaceDlg::IDD, pParent)
{

}

CFindReplaceDlg::~CFindReplaceDlg()
{
}

void CFindReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_SEARCH, m_Search);
	DDX_Control(pDX, IDC_BOOKMARK, m_Bookmark);
	DDX_Control(pDX, IDC_REPLACE, m_Replace);
	DDX_Control(pDX, IDC_REPLACEALL, m_ReplaceAll);

	DDX_Control(pDX, IDC_ACTIONS, m_Actions);
	DDX_Control(pDX, IDC_CONDITIONS, m_Conditions);

	DDX_Control(pDX, IDC_FIND, m_Find);
	DDX_Control(pDX, IDC_REPLACEWITH, m_ReplaceWith);

	DDX_Control(pDX, IDC_RESULTS, m_List);

	CExtWS<CDialog>::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFindReplaceDlg, CExtWS<CDialog>)
	ON_BN_CLICKED(IDC_SEARCH, CFindReplaceDlg::OnBnClickedSearch)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_RESULTS, &CFindReplaceDlg::OnNMDblclkResults)
	ON_BN_CLICKED(IDC_BOOKMARK, &CFindReplaceDlg::OnBnClickedBookmark)
	ON_BN_CLICKED(IDC_REPLACE, &CFindReplaceDlg::OnReplace)
	ON_BN_CLICKED(IDC_REPLACEALL, &CFindReplaceDlg::OnReplaceAll)
END_MESSAGE_MAP()

BOOL CFindReplaceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// List
	m_List.InsertColumn(0, "Line", 0, 40);
	m_List.InsertColumn(1, "Occurance", 0, 280);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// Check boxes
	m_Actions.SetCheck(BST_CHECKED);
	m_Conditions.SetCheck(BST_CHECKED);

	return TRUE;
}

CString CFindReplaceDlg::StripHTML(LPCSTR String)
{
	CString Result = String;
	CString Work;

	bool bInTag = false;
	bool bParamNum = false;
	bool bParamNum2 = false;

	for (int i = 0; i < Result.GetLength(); i++)
	{
		if (!bInTag && Result[i] == '<')
		{
			bInTag = true;
			
			// Parameter number check
			if (Result.Mid(i + 1, 8) == "numparam" && !bParamNum2)
			{
				bParamNum2 = false;
				bParamNum = true;
			}
		}

		if (bInTag && Result[i] == '>')
		{
			if (bParamNum2)
			{
				bParamNum = false;
				bParamNum2 = false;
			}

			if (bParamNum && !bParamNum2)
				bParamNum2 = true;

			if (!bParamNum)
				bInTag = false;
		}

		if (!bInTag && Result[i] != '>' && !bParamNum)
			Work += Result[i];
	}

	return Work;
	
}

// CFindReplaceDlg message handlers
void CFindReplaceDlg::OnBnClickedSearch()
{
	m_List.DeleteAllItems();

	// Get text
	CString Text;
	m_Find.GetWindowTextA(Text);
	Text.MakeLower();
	m_Text = Text;

	int Number = 0;

	if(pChrono)
		pChrono->CreateUndo();

	// Iterate event list
	for (int i = 0; i < m_pEventList->size(); i++)
	{
		CEditorEvent* pEvent = m_pEventList->at(i);
		DoSearch(pEvent, Number);

		// Comments etc dont increment number
		if (pEvent->m_type == 0 || pEvent->m_type == 4)
			Number++;
	}
}

void CFindReplaceDlg::DoSearch(CEditorEvent* pEvent, int& Number)
{
	int i, j;

	
	if (m_Conditions.GetCheck() == BST_CHECKED)
	{
		for (j = 0; j < pEvent->m_Conditions.size(); j++)
		{
			CString Match = pEvent->m_Conditions[j]->m_Text;
			Match.MakeLower();

			// Matches?
			if (Match.Find(m_Text) != -1)
			{
				CString ItemN;
				ItemN.Format("%d", Number + 1);
				int Item = m_List.InsertItem(m_List.GetItemCount(), ItemN);

				CString Text = pEvent->m_Conditions[j]->m_Text;
				Text = StripHTML(Text);
				CString Add = "Condition: ";
				Add += Text;

				m_List.SetItemText(Item, 1, Add);

				m_List.SetItemData(Item, (DWORD)pEvent);
			}
		}
	}

	if (m_Actions.GetCheck() == BST_CHECKED)
	{
		for (j = 0; j < pEvent->m_Actions.size(); j++)
		{
			CString Match = pEvent->m_Actions[j]->m_Text;
			Match.MakeLower();
		
			// Matches?
			if (Match.Find(m_Text) != -1)
			{
				CString ItemN;
				ItemN.Format("%d", Number + 1);
				int Item = m_List.InsertItem(m_List.GetItemCount(), ItemN);

				CString Text = pEvent->m_Actions[j]->m_Text;
				Text = StripHTML(Text);
				CString Add = "Action: ";
				Add += Text;

				m_List.SetItemText(Item, 1, Add);

				m_List.SetItemData(Item, (DWORD)pEvent);
			}
		}
	}

	// Loop sub events
	for (i = 0; i < pEvent->m_EventList.size(); i++)
	{
		CEditorEvent* pSubEvent = pEvent->m_EventList[i];

		// Comments etc dont increment number
		if (pSubEvent->m_type == 0 || pSubEvent->m_type == 4)
			Number++;

		DoSearch(pSubEvent, Number);
	}
}

void CFindReplaceDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
}

void CFindReplaceDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CFindReplaceDlg::OnNMDblclkResults(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	// Check result
	POSITION Pos = m_List.GetFirstSelectedItemPosition();
	int Item = m_List.GetNextSelectedItem(Pos);

	if (Item == -1) return; 

	CEditorEvent* pEvent = (CEditorEvent*)m_List.GetItemData(Item);

	int y = pEvent->m_rect(pChrono).top-31 + pChrono->ScrollPos.y;
	if(y > pChrono->GetScrollBarCtrl(1)->GetScrollLimit())
		y = pChrono->GetScrollBarCtrl(1)->GetScrollLimit();

	pChrono->SetScrollPos(1, y, true);
	pChrono->Invalidate();
}

void CFindReplaceDlg::OnBnClickedBookmark()
{
	// Check result
	POSITION Pos = m_List.GetFirstSelectedItemPosition();

	while (Pos)
	{
		int Item = m_List.GetNextSelectedItem(Pos);

		if (Item == -1) return; 

		CEditorEvent* pEvent = (CEditorEvent*)m_List.GetItemData(Item);
		
		pEvent->m_bBookmarked = true;
		pChrono->InvalidateRect(pEvent->m_rect(pChrono));
	}
}

void CFindReplaceDlg::OnReplace()
{
	// Check result
	POSITION Pos = m_List.GetFirstSelectedItemPosition();
	int Item = m_List.GetNextSelectedItem(Pos);

	if (Item == -1) return; 

	// Get the text we're replacing
	CString Old;
	CString New;

	m_Find.GetWindowTextA(Old);
	m_ReplaceWith.GetWindowTextA(New);
	
	CEditorEvent* pEvent = (CEditorEvent*)m_List.GetItemData(Item);

	if (m_Actions.GetCheck() == BST_CHECKED)
	{
		for (int i = 0; i < pEvent->m_Actions.size(); i++)
		{
			for (int j = 0; j < pEvent->m_Actions[i]->params.size(); j++)
			{
				CString Parameter = pEvent->m_Actions[i]->params[j]->CreateParamString(pChrono);
				int type = pEvent->m_Actions[i]->params[j]->m_type;
				Parameter.Replace(Old, New);
				pEvent->m_Actions[i]->params[j]->CreateFromString(Parameter, pChrono->application, type, "", false);
				pChrono->ReformatAction(pEvent->m_Actions[i]);
			}
		}
	}

	if (m_Conditions.GetCheck() == BST_CHECKED)
	{
		for (int i = 0; i < pEvent->m_Conditions.size(); i++)
		{
			for (int j = 0; j < pEvent->m_Conditions[i]->params.size(); j++)
			{
				CString Parameter = pEvent->m_Conditions[i]->params[j]->CreateParamString(pChrono);
				int type = pEvent->m_Actions[i]->params[j]->m_type;
				Parameter.Replace(Old, New);
				pEvent->m_Conditions[i]->params[j]->CreateFromString(Parameter, pChrono->application, type, "", false);
				pChrono->ReformatCondition(pEvent->m_Conditions[i]);
			}
		}
	}

	pChrono->InvalidateRect(pEvent->m_rect(pChrono));
	pChrono->Invalidate();
}

void CFindReplaceDlg::OnReplaceAll()
{
	OnBnClickedSearch();

	// Get the text we're replacing
	CString Old;
	CString New;

	m_Find.GetWindowTextA(Old);
	m_ReplaceWith.GetWindowTextA(New);

	for (int Line = 0; Line < m_List.GetItemCount(); Line++)
	{
		int Item = Line;
		
		CEditorEvent* pEvent = (CEditorEvent*)m_List.GetItemData(Item);

		if (m_Actions.GetCheck() == BST_CHECKED)
		{
			for (int i = 0; i < pEvent->m_Actions.size(); i++)
			{
				for (int j = 0; j < pEvent->m_Actions[i]->params.size(); j++)
				{
					CString Parameter = pEvent->m_Actions[i]->params[j]->CreateParamString(pChrono);
					Parameter.Replace(Old, New);
					pEvent->m_Actions[i]->params[j]->CreateFromString(Parameter, pChrono->application, 0, "", false);
					pChrono->ReformatAction(pEvent->m_Actions[i]);
				}
			}
		}

		if (m_Conditions.GetCheck() == BST_CHECKED)
		{
			for (int i = 0; i < pEvent->m_Conditions.size(); i++)
			{
				for (int j = 0; j < pEvent->m_Conditions[i]->params.size(); j++)
				{
					CString Parameter = pEvent->m_Conditions[i]->params[j]->CreateParamString(pChrono);
					Parameter.Replace(Old, New);
					pEvent->m_Conditions[i]->params[j]->CreateFromString(Parameter, pChrono->application, 0, "", false);
					pChrono->ReformatCondition(pEvent->m_Conditions[i]);
				}
			}
		}

		pChrono->InvalidateRect(pEvent->m_rect(pChrono));
		pChrono->Invalidate();
	}
}
