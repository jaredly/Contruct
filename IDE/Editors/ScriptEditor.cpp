// ScriptEditor.cpp : implementation file
//

// File includes
#include "StdAfx.h"
#include "..\Construct.h"
#include "..\ChildFrame.h"

// Application structure for Object map
#include "..\Structure.h"

// CScriptEditor structure
#include "ScriptEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptEditor

IMPLEMENT_DYNCREATE(CScriptEditor, CView)

CScriptEditor::CScriptEditor()
{
	m_bIsCreated = FALSE;

	HMODULE m_hDll = NULL;
	m_hDll = ::LoadLibrary(_T("SciLexer.dll"));
}

CScriptEditor::~CScriptEditor()
{
	AfxFreeLibrary(m_hDll);
}


BEGIN_MESSAGE_MAP(CScriptEditor, CView)
	//{{AFX_MSG_MAP(CScriptEditor)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_DEBUG_START, OnGenerate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptEditor drawing

void CScriptEditor::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CScriptEditor diagnostics

#ifdef _DEBUG
void CScriptEditor::AssertValid() const
{
	CView::AssertValid();
}

void CScriptEditor::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

#include "Scintilla\ScintillaWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CScriptEditor message handlers

void CScriptEditor::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	if (!m_bIsCreated) {
		m_hDll = NULL;
		m_hDll = ::LoadLibrary(_T("SciLexer.dll"));

		scintWin.Create(_T("Title"), WS_CHILD | WS_VISIBLE, 
						CRect(-1, -1, 0, 0), this, 10000);

		scintWin.Init();
		scintWin.SendMessage(SCI_SETKEYWORDS, 0, (long)"event\nconditions\nactions\nsub");
		m_bIsCreated = TRUE;

		scintWin.SendMessage(SCI_SETHSCROLLBAR, FALSE, 0);
		scintWin.SendMessage(SCI_SETVSCROLLBAR, FALSE, 0);
		UpdateScrollBars();

		if (!m_pLayout) 
		{
			if (((CChildFrame*)GetParentFrame())->m_pLayout != NULL)
				m_pLayout = ((CChildFrame*)GetParentFrame())->m_pLayout;
			else
				return;
		}
	}
}

void CScriptEditor::UpdateScrollBars()
{
	// In this function we get the scroll range / pos from scintilla and put it in the main view
	SCROLLINFO scrollInfo;

	// Copy vertical scrollbar info
	scintWin.GetScrollInfo(SB_VERT, &scrollInfo);
	SetScrollInfo(SB_VERT, &scrollInfo, TRUE);
	// Scrollbars don't disable automatically(!)
	if (scrollInfo.nMin == scrollInfo.nMax)
		EnableScrollBarCtrl(SB_VERT, FALSE);
	else
		EnableScrollBarCtrl(SB_VERT, TRUE);
}

void CScriptEditor::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//CTreeListView::OnVScroll(nSBCode, nPos, pScrollBar);
	// Pass the message onto the treecontrol
	if (nSBCode == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION)
	{
		scintWin.SendMessage(WM_VSCROLL, MAKELONG(nSBCode, nPos));
	}
	else
	{
		scintWin.SendMessage(WM_VSCROLL, nSBCode, 0);
	}
	
	UpdateScrollBars();
}

void CScriptEditor::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//CTreeListView::OnVScroll(nSBCode, nPos, pScrollBar);
	// Pass the message onto the treecontrol
	if (nSBCode == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION)
	{
		scintWin.SendMessage(WM_HSCROLL, MAKELONG(nSBCode, nPos));
	}
	else
	{
		scintWin.SendMessage(WM_HSCROLL, nSBCode, 0);
	}
	
	UpdateScrollBars();
}

void CScriptEditor::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	if (!m_bIsCreated) {
		m_hDll = NULL;
		m_hDll = ::LoadLibrary(_T("SciLexer.dll"));

		scintWin.Create(_T("Title"), WS_CHILD | WS_VISIBLE, 
						CRect(0, 0, 0, 0), this, 10000);

		scintWin.Init();
		scintWin.SendMessage(SCI_SETKEYWORDS, 0, (long)"event\nconditions\nactions\nsub");
		m_bIsCreated = TRUE;

		scintWin.SendMessage(SCI_SETHSCROLLBAR, FALSE, 0);
		scintWin.SendMessage(SCI_SETVSCROLLBAR, FALSE, 0);
		UpdateScrollBars();
	}

	scintWin.MoveWindow(-1, -1, cx, cy);
	
	UpdateScrollBars();
}

void CScriptEditor::OnGenerate()
{
	CString Script;

	CEventSheet* sheet = m_app->FindEventSheetFromNumber(m_pLayout->m_EventListID);
	for (int i = 0; i < sheet->m_EventList.size(); i++)
	{
		CEditorEvent Event = *sheet->m_EventList.at(i);	// i cant believe this works...and we should do it via pointer!

		Script += "event\r\n{\r\n	conditions{\r\n";

		// Conditions
		//PORT: for loop scoping
		int j;

		for (j = 0; j < Event.m_Conditions.size(); j++)
		{
			CEditorCondition Condition = *Event.m_Conditions.at(j);

			Script += "		";
			Script += Condition.m_Object;
			Script += ".";
			Script += Condition.m_Script;
			Script += "(";

			//PORT: for loop scoping
			int k;

			for (k = 0; k < Condition.params.size(); k++)
			{
				Script += Condition.params.at(k)->CreateParamString(this);
				Script += ", ";
			}

			if (k != 0)
				Script = Script.Left(Script.GetLength() - 2);

			Script += ")";
			Script += "\r\n";
		}

		Script += "	}\r\n\r\n	actions{\r\n";

		// Actions
		for (j = 0; j < Event.m_Actions.size(); j++)
		{
			CEditorAction* Action = Event.m_Actions.at(j);

			Script += "		";
			Script += Action->m_Object;
			Script += ".";
			Script += Action->m_Script;
			Script += "(";

			//PORT: for loop scoping
			int k;

			for (k = 0; k < Action->params.size(); k++)
			{
				Script += Action->params.at(k)->CreateParamString(this);
				Script += ", ";
			}

			if (k != 0)
				Script = Script.Left(Script.GetLength() - 2);

			Script += ")";
			Script += "\r\n";
		}

		Script += "	}\r\n}\r\n\r\n";
	}

	scintWin.SetText(Script);
}