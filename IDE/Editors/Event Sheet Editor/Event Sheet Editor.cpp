/////////////////////////////////////////////////////////////////////////////
// Event sheet editor - main file
/////////////////////////////////////////////////////////////////////////////
 
// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "..\..\Construct Doc.h"
#include "Event Sheet Editor.h"
#include <assert.h>
/////////////////////////////////////////////////////////////////////////////
// EventSheetEditor

IMPLEMENT_DYNCREATE(EventSheetEditor, CChronoEventEditor)

EventSheetEditor::EventSheetEditor()
{
	m_pEventList= NULL;
	m_CtrlDown = false;
	m_ShiftDown = false;
	scrolllock = 0;
	ScrollPos = CPoint(0,0);
	m_pHeader = 0;
	m_pEventList = 0;

	layout = NULL;
	application = NULL;
	tooltip = false;
	m_undo = 0;
	m_LastRibbon = 4;
	m_pEventList = 0;

}

EventSheetEditor::~EventSheetEditor()
{
	DeleteIconBitmaps();
}

BEGIN_MESSAGE_MAP(EventSheetEditor, CChronoEventEditor)
	//{{AFX_MSG_MAP(EventSheetEditor)
	ON_WM_CREATE()
	ON_COMMAND(ID_INSERTC, OnAddComment)
	ON_COMMAND(ID_EXPAND, OnExpandComment)
	ON_COMMAND(ID_COMMENTCOLOUR, OnCommentColour)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_NOTIFY(SCN_CHARADDED, 10000, OnChar)

	// Bookmarks
	ON_COMMAND(ID_BOOKMARK, OnBookmark)
	ON_COMMAND(ID_REMOVEBOOKMARKS, RemoveBookmarks)
	ON_COMMAND(ID_NEXTBOOKMARK, NextBookmark)
	ON_COMMAND(ID_PREVIOUSBOOKMARK, PreviousBookmark)

	// Clipboard
	ON_COMMAND(ID_EDIT_CUT, OnToolbarCut)
	ON_COMMAND(ID_EDIT_COPY, OnToolbarCopy)
	ON_COMMAND(IDFR_COPY, OnToolbarCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnToolbarPaste)
	ON_COMMAND(ID_EDIT_DELETE, OnToolbarDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)

	// Structure, Ribbon
	ON_COMMAND(ID_RIBBON_ADDEVENT, OnAddEvent)
	ON_COMMAND(ID_RIBBON_ADDCONDITION, OnAddCondition)
	ON_COMMAND(ID_RIBBON_ADDSUBEVENT, OnAddSubEvent)
	ON_COMMAND(ID_RIBBON_ADDACTION, OnAddAction)
	ON_COMMAND(ID_RIBBON_ADDGROUP, OnAddGroup)
	ON_COMMAND(ID_RIBBON_TOGGLEEVENT, OnToggleEvent)
	ON_COMMAND(ID_RIBBON_FINDREPLACE, OnFindReplace)

	// Mouse & keyboard
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()

	ON_COMMAND(ID_EDIT_FINDANDREPLACE2, OnFindReplace)
	ON_WM_TIMER()
	ON_COMMAND(ID_CHANGEALLTO_CHRONOLOGICAL, OnAllChronological)
	ON_COMMAND(ID_CHANGEALLTO_LIST, OnAllList)
	ON_COMMAND(ID_CONDITIONS_ADDCONDITION, OnAddCondition)
	ON_COMMAND(ID_EDITOR_INSERTCOMMENT, OnExpandComment)
	ON_COMMAND(ID_EDIT_UNDO, OnToolbarUndo)
	ON_COMMAND(ID_EDIT_REDO, OnToolbarRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EventSheetEditor diagnostics


void EventSheetEditor::OnRibbonAdd()
{
	AddGroup();
}

void EventSheetEditor::AssertValid() const
{
	//CView::AssertValid();
}

extern CMainFrame* pMainWnd;
extern CMultiDocTemplate* pDocTemplate;

void EventSheetEditor::Dump(CDumpContext& dc) const
{
	//CView::Dump(dc);
}

CConstructDoc* EventSheetEditor::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CConstructDoc)));
	return (CConstructDoc*)m_pDocument;
}

CEditorImage* EventSheetEditor::ImageAt(CPoint pt, CEditorAction* Action)
{
	for(int a = 0; a < Action->m_imgs.size(); a++)
		if(Action->m_imgs[a].m_Rect.PtInRect(pt))
			return &Action->m_imgs[a];

	return 0;
}

CEditorImage* EventSheetEditor::ImageAt(CPoint pt, CEditorCondition* Condition)
{
	for(int a = 0; a < Condition->m_imgs.size(); a++)
		if(Condition->m_imgs[a].m_Rect.PtInRect(pt))
			return &Condition->m_imgs[a];

	return 0;
}

void EventSheetEditor::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CEditorEvent* pEvent = EventAt(point);
	if (pEvent)
	{
		if(pEvent->m_type == SCRIPT_TYPE)
			EditScript(pEvent);
		else if(pEvent->m_type == COMMENT_TYPE)
			EditComment(pEvent);
		else if(pEvent->m_type == INCLUDE_TYPE)
		{
			EventSheet* pEventSheet = application->FindEventSheetFromNumber(pEvent->event_sheet_include);
			OpenEventSheet(pEventSheet);
		}
		else
		{
			CEditorCondition* pCondition = pEvent->conditionAt(point, this);
			CEditorAction* pAction = pEvent->actionAt(point, this);
			if(pCondition)
			{
				if(CEditorImage* image = ImageAt(point, pCondition))
					EditImage(pCondition, image);
				else
				{
					CRect icon = pCondition->m_rect(this);
					icon.right = icon.left + 16;
					icon.bottom = icon.top + 16;

					EditCondition(pCondition);
				}
			}

			else if(pAction)
			{
				if(CEditorImage* image = ImageAt(point, pAction))
					EditImage(pAction, image);
				else
				{
					CRect icon = pAction->m_rect(this);
					icon.right = icon.left + 16;
					icon.bottom = icon.top + 16;
					EditAction(pAction);
				}
			}
			
			else 
			{
				// expand / close
				pEvent->m_open(this) = 1- pEvent->m_open(this);
				Invalidate();
			}
		}
	}

	else
		AddCondition();

	Invalidate();
}

void EventSheetEditor::OnAddAction()
{
	if(this->m_FocusEvent)
	{
		int index = -1;
		if(m_FocusAction)
			for(int i = 0; i < m_FocusEvent->m_Actions.size(); i++)
				if(m_FocusAction == m_FocusEvent->m_Actions.at(i))
					index = i;

		DeselectConditions();
		AddAction(m_FocusEvent, index);
		InvalidateRect(m_FocusEvent->m_rect(this));
	}

}

void EventSheetEditor::OnAddCondition()
{
		// Insert a condition into an existing row
	if(this->m_FocusEvent)
	{
		int index = -1;
		if(m_FocusCondition)
			for(int i = 0; i < m_FocusEvent->m_Conditions.size(); i++)
				if(m_FocusCondition == m_FocusEvent->m_Conditions.at(i))
					index = i;

		DeselectConditions();
		AddCondition(false, NULL, false, m_FocusEvent, false, index);
		InvalidateRect(m_FocusEvent->m_rect(this));
	}


	/*SelectedEventVector SelectedEvents;
	CreateEventSelectionVector(SelectedEvents, (*m_pEventList));

	if (SelectedEvents.size() == 1)
	{
		AddCondition(NULL, false, SelectedEvents[0]);
		InvalidateRect(SelectedEvents[0]->m_rect(this));
	}*/
}

void EventSheetEditor::OnAddSubEvent()
{
	if (m_FocusEvent)
	{
		if(m_FocusEvent->canAddEvents())
		{
			AddCondition(false, NULL, true, m_FocusEvent);
			InvalidateRect(m_FocusEvent->m_rect(this));
		}
	}
}

void EventSheetEditor::OnAddEvent()
{
	DeselectEvents();
	DeselectActions();
	DeselectConditions();

	if(m_FocusEvent)
	{
		CEditorEvent* pParent = FindEventParent(m_FocusEvent);
		if(AddCondition(false, NULL, true, pParent))
		{
			// The above adds the event to the end of the event list, we need to move it
			EventVector* pEvents = m_pEventList; 
			if(pParent)
				pEvents = &pParent->m_EventList;

			int index = -1;
			for(int i = 0; i < pEvents->size(); i++)
				if(m_FocusEvent == pEvents->at(i))
					index = i;
			if(index != -1)
			{
				CEditorEvent* newEvent = pEvents->back();
				pEvents->pop_back();
				pEvents->insert(pEvents->begin() + index, newEvent);
			}
		}

	}
	else
		AddCondition();
}

void EventSheetEditor::OnAddGroup()
{
	DeselectEvents();
	DeselectActions();
	DeselectConditions();
	if(m_FocusEvent)
	{
		AddGroup(false, m_FocusEvent);
	}
	else
		AddGroup();
}

void EventSheetEditor::OnToggleEvent()
{
	SelectedEventVector SelectedEvents;
	CreateEventSelectionVector(SelectedEvents, (*m_pEventList));

	for(int a = 0; a < SelectedEvents.size(); a ++)
	{
		SelectedEvents[a]->m_bEnabled = !SelectedEvents[a]->m_bEnabled;
		InvalidateRect(SelectedEvents[a]->m_rect(this));
	}
}

/////////////////////////////////////////////////////////////////////////////
// EventSheetEditor message handlers

// Icons
HBITMAP EventSheetEditor::GetIcon(int oid)
{
	if(oid < 0) 
		return 0;
	if(oid >= m_hbitmaps.size()) 
		return 0;
	else
		return m_hbitmaps.at(oid);
}

void EventSheetEditor::OnSwitchToLayoutEditor()
{
	KillFocusOnInlineEditor();
}

void EventSheetEditor::OnInitialUpdate() 
{
	if (!application) 
	{
		if (((CChildFrame*)GetParentFrame())->application != NULL)
			application = ((CChildFrame*)GetParentFrame())->application;
		else
			return;
	}

	m_inline_param = 0;
    CChronoEventEditor::OnInitialUpdate();
	CreateIconBitmaps();

	EventSheet* sheet = ((CChildFrame*)GetParentFrame())->event_sheet;

	// 
	if(sheet)
	{
		m_pEventList = &sheet->m_EventList;
		m_pHeader = &sheet->m_Header;
	}

	if(!m_pEventList)
		return;

	CMainFrame* pMainFrame = ((CMainFrame*)(EventSheetEditor::GetParentOwner()));
	pMainFrame->m_Ribbon.Ribbon_PageSelectionSet(m_LastRibbon);

	CObjTypeMap* objTypeMap = &application->object_types;
	CObjType* oT;

	POSITION pos = objTypeMap->GetStartPosition();
	long mapKey;
	CList<CObjType*, CObjType*&> otList;

	// Loop through and add the objects
	while (pos != NULL) 
	{
		objTypeMap->GetNextAssoc(pos, mapKey, oT);

		CObj *o;
		if(layout)
		{
		POSITION pos = layout->objects.GetStartPosition();
	
		
			for (int i = 0; i < layout->objects.GetCount(); i++) 
			{
				long ID;
				layout->objects.GetNextAssoc(pos, ID, o);
				if (o->GetGlobalID() == oT->ObjectIdentifier || oT->m_bGlobal)
					if (oT->GetTableCount(0) != 0)
						otList.AddTail(oT);
			}
		}
		else
		{
			otList.AddTail(oT);
		}
	}

	CObjType* system = &application->m_System;
	otList.AddTail(system);



	m_tmpScrollCount = 0;

	// Update scroll bars
	UpdateScrollbars();


	CRect wnd;
	GetWindowRect(wnd);
	MoveWindow(wnd);

	//Refresh text
	Reformat();

	return;
}

int EventSheetEditor::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	int retVal = CChronoEventEditor::OnCreate(lpCreateStruct);
	if (retVal != 0)
		return retVal;

	CMainFrame* pMainFrame = ((CMainFrame*)(EventSheetEditor::GetParentOwner()));
	pMainFrame->m_Ribbon.Ribbon_PageSelectionSet(m_LastRibbon);

	// Set comment colours
	m_CommentColour = RGB(255,255,215);
	m_CommentDlg.colour = RGB(255, 255, 215);

	ModifyStyle(0,WS_CLIPCHILDREN);

	// Search all the other event editors open and see if we can find
	// one that is editing the same event list. We need
	// to get its pointer to CUndo
	CMainFrame* pMainWnd = (CMainFrame*)(this->GetParent()->GetParent()->GetParent()->GetParent());
	HWND clientWindow;
	clientWindow = ::GetWindow(pMainWnd->m_hWndMDIClient, GW_CHILD);

	EventSheetEditor* owner;

	while (clientWindow != NULL)
	{
		CChildFrame* child = (CChildFrame*)CWnd::FromHandle(clientWindow);

		for(int a = 0; a < 4; a ++)
		{
			owner = child->m_pEventView[a%2][a/2];
			if(owner)
			{
				if(owner->m_pEventList == this->m_pEventList)
				{
					if(owner->m_undo)
					{	
							m_undo = owner->m_undo;
							return 0;
					}
				}

			}
		}
	
		clientWindow = :: GetWindow(clientWindow, GW_HWNDNEXT);
	}
	CChildFrame* child = (CChildFrame*)this->GetParent()->GetParent();
	if(child->m_pEventView[0][0] == NULL)
	{
		child->m_pEventView[0][0] = this;
	}

	//no undo...create one :/

	m_undo = new CUndo(this);

	if(layout)
	{
		SetScrollPos(0, layout->m_EventEditorScroll.x);
		SetScrollPos(1, layout->m_EventEditorScroll.y);
	}

	OnInitialUpdate();
	UpdateScrollbars();

	return 0;
}

void EventSheetEditor::CreateUndo()
{
	((CChildFrame*)GetParentFrame())->event_sheet->SetChanged(true);

	// START UNDO INFO //
	CAction *action = new CAction();
	action->m_type = ACTION_EVENTEDITOR;
	action->m_bGroup = true;
	CArchive *ar = action->Prepare1();
	*ar << (int)(m_pEventList->size());
	for(int a = 0; a < m_pEventList->size(); a++)
		(*m_pEventList)[a]->Serialize(*ar, false,this);
	
	action->Prepare2();
	m_undo->AddAction(action);
	// END UNDO INFO //
}

void EventSheetEditor::OnToolbarUndo()
{
	if(m_pInlineScintilla.GetSafeHwnd())
	{
		m_pInlineScintilla.Undo();
		return;
	}
	m_undo->Undo();
	Invalidate();
}

void EventSheetEditor::OnToolbarRedo()
{
	if(m_pInlineScintilla.GetSafeHwnd())
	{
		m_pInlineScintilla.Redo();
		return;
	}
	m_undo->Redo();
	Invalidate();
}

void EventSheetEditor::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	if(m_pInlineScintilla.GetSafeHwnd())
	{
		pCmdUI->Enable(m_pInlineScintilla.CanUndo());
		return;
	}
	pCmdUI->Enable(m_undo->CanUndo());
}

void EventSheetEditor::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	if(m_pInlineScintilla.GetSafeHwnd())
	{
		pCmdUI->Enable(m_pInlineScintilla.CanRedo());
		return;
	}
	pCmdUI->Enable(m_undo->CanRedo());
}

CAction* EventSheetEditor::PerformAction(CAction *action, BOOL bRepeat)
{

// START UNDO INFO //
	CAction *actionNew = new CAction();
	actionNew->m_type = ACTION_EVENTEDITOR;
	actionNew->m_bGroup = true;
	CArchive *ar2 = actionNew->Prepare1();
	*ar2 << (int)(m_pEventList->size());
	for(int a = 0; a < m_pEventList->size(); a++)
		(*m_pEventList)[a]->Serialize(*ar2, false,this);
	
	actionNew->Prepare2();

	// END UNDO INFO //

	//Remove every event
	for(int e = 0; e < (*m_pEventList).size() ; e ++ )
	{
		//if((*m_pEventList)[e]->DeleteSelectedEvents((*m_pEventList),e,*this))
		{
			(*m_pEventList)[e]->Remove(false, this);
		}
	}

	// Now serialize to create the new ones
	CArchive *ar = action->Unprepare1();

	int size = 0;
	*ar >> size;

	for(int a = 0; a < size; a ++)
	{
		CEditorEvent* e = new CEditorEvent;
		e->Init();
		e->Serialize(*ar, true, this);	//the save bool prevents it ignoring unselected items etc
		m_pEventList->push_back(e);
	}

	Invalidate();

	action->Unprepare2();
	return actionNew;
}

// Add a comment
void EventSheetEditor::OnAddComment()
{	
	DeselectEvents();
	DeselectActions();
	DeselectConditions();

	CreateUndo();

	CEditorEvent* newEvent = new CEditorEvent;
	newEvent->Init();
	newEvent->m_type = 1;
	newEvent->m_cText = "";
	newEvent->m_cTextBG = m_CommentColour;
	newEvent->m_cTextColour = 0;
	newEvent->m_cFontName = "Arial";
	newEvent->m_cFontSize = 14;
	newEvent->m_cbFontBold = false;
	newEvent->m_cbFontItalic = false;
	newEvent->m_cbFontStrike = false;
	newEvent->m_cbFontUnderline = false;

	m_pEventList->push_back(newEvent);
	Invalidate();
}

// Open colour dialog
void EventSheetEditor::OnCommentColour()
{
	CColorDialog dlg;
	dlg.SetCurrentColor(m_CommentColour);
	dlg.DoModal();
	m_CommentColour = dlg.GetColor();
}

// Expand comment dialog
void EventSheetEditor::OnExpandComment()
{	
	m_CommentDlg.theComment = "";
	if (m_CommentDlg.DoModal() == IDOK)
	{
		CreateUndo();
		if (m_CommentDlg.theComment == "") m_CommentDlg.theComment = " ";

		CEditorEvent* pNewComment = new CEditorEvent;
		pNewComment->Init();
		pNewComment->m_type = 1;
		pNewComment->m_cText = m_CommentDlg.theComment;
		pNewComment->m_cTextBG = m_CommentDlg.colour;
		pNewComment->m_cTextColour = m_CommentDlg.fontcolour;
		pNewComment->m_cFontName = m_CommentDlg.lf.lfFaceName;
		pNewComment->m_cFontSize =m_CommentDlg.lf.lfHeight;
		pNewComment->m_cbFontBold = m_CommentDlg.lf.lfWeight == FW_BOLD? true : false;
		pNewComment->m_cbFontItalic = m_CommentDlg.lf.lfItalic;
		pNewComment->m_cbFontStrike = m_CommentDlg.lf.lfStrikeOut;
		pNewComment->m_cbFontUnderline = m_CommentDlg.lf.lfUnderline;

		SelectedEventVector SelectedEvents;
		CreateEventSelectionVector(SelectedEvents, (*m_pEventList));

		if (SelectedEvents.size() >= 1)
		{
			// Move it
			EventVector* pEventList = FindEventListParent(SelectedEvents[0], m_pEventList); 
			if(!pEventList)
				pEventList = m_pEventList;
			if (find(pEventList->begin(), pEventList->end(), SelectedEvents[0]) != pEventList->end())
				pEventList->insert(find(pEventList->begin(), pEventList->end(), SelectedEvents[0]), pNewComment);
			else
				pEventList->push_back(pNewComment);
		}

		else
			m_pEventList->push_back(pNewComment);

		Invalidate();
	}
}

void EventSheetEditor::AddGroup(CEditorEvent* Parent, CEditorEvent* pAfter)
{
	CEditEventGroup dlg;
	if(dlg.DoModal() == IDOK)
	{
		CreateUndo();
		CEditorEvent* pEvent = new CEditorEvent();
		pEvent->Init();
		pEvent->m_select(this) = true;

		pEvent->m_type = 2; // group
		pEvent->m_sActive = dlg.m_Active;
		pEvent->m_sDescription = dlg.m_Description;
		pEvent->m_sTitle = dlg.m_Title;

		if(Parent)
		{
			Parent->m_EventList.push_back(pEvent);
		}
		else
		{
			if(pAfter){
				CEditorEvent* Parent = FindEventParent(pAfter);
				EventVector* pEventList = m_pEventList;
				if(Parent)
					pEventList = &Parent->m_EventList;
				
				int index = 0;
				for(int i = 0; i < pEventList->size(); i++)
				{
					if((*pEventList)[i] == pAfter)
					{
						index = i + 1;
						break;
					}
				}

				pEventList->insert(pEventList->begin() + index, pEvent);
			}
			else
			{
				m_pEventList->push_back(pEvent);
			}
		}
		Invalidate();
	}
}

void EventSheetEditor::AddAction(CEditorEvent* pEvent, int index)
{
	if(index == -1)
		index = pEvent->m_Actions.size();

	CEventWizardDlg EventWizard;
	EventWizard.m_aceType = ACTION;
	EventWizard.layout = layout;
	EventWizard.application = application;
	EventWizard.SetMap(&application->object_types);

	if(EventWizard.DoModal() != IDOK)
		return;

	CreateUndo();

	CEditorAction* pAction = new CEditorAction;
	pAction->Init();

	CObjType* pType = application->FindObjTypeFromNumber(EventWizard.m_pSelectedObject);
	if(!pType)
		return;
	pAction->oid = pType->ObjectIdentifier;
	pAction->m_Family = 0; // need to remove this bool later
	pAction->m_Object = pType->GetName(); // is this set in reformat ??
	pAction->DLLIndex = pType->DLLIndex;

	pAction->actID = EventWizard.m_aceIndex;
	pAction->mid = EventWizard.m_childid;

	CParametersDlg lazy;
	lazy.application = application;
	lazy.m_ID = pType;
	lazy.m_selfType = pType;
	AppendParameters(EventWizard.pAce, EventWizard.param_strings, EventWizard.param_types, pAction->params, pAction->actID, lazy);

	vector<CEditorAction*>::iterator i = pEvent->m_Actions.begin();
	advance(i, index);

	if(i!= pEvent->m_Actions.end())
		pEvent->m_Actions.insert(i, pAction);
	else
		pEvent->m_Actions.push_back(pAction);

	m_FocusAction = pAction;

	ReformatAction(pAction);
}

// Add a condition
bool EventSheetEditor::AddCondition(bool bPreset, CEditorCondition* Parent, bool SubEvent, CEditorEvent* Event, bool IgnoreTrigger, int index)
{
	CEditorCondition* pNewCondition = 0;

	CEventWizardDlg EventWizard;
	EventWizard.layout = layout;
	EventWizard.application = application;
	EventWizard.SetMap(&application->object_types);

	// No presets, just add a condition
	if (!bPreset)
	{
		EventWizard.m_aceType = CONDITION;
	}

	// There's presets (added 0.96.3) so use them
	else
	{
		EventWizard.SetStage(m_InitialStage, m_InitialSelect, m_OldName);
		EventWizard.m_aceType = CONDITION;
	}

	// Cancel clicked - quit
	if(EventWizard.DoModal() != IDOK)
		return false;

	pNewCondition = new CEditorCondition;
	pNewCondition->Init();

	CObjType* pType = application->FindObjTypeFromNumber(EventWizard.m_pSelectedObject);
	if(!pType)
		return false;
	pNewCondition->oid = pType->ObjectIdentifier;
	pNewCondition->m_Family = 0; // need to remove this bool later
	pNewCondition->m_Object = pType->GetName(); // is this set in reformat ??
	pNewCondition->DLLIndex = pType->DLLIndex;

	pNewCondition->cndID = EventWizard.m_aceIndex;
	pNewCondition->mid = EventWizard.m_childid;

	CParametersDlg lazy;
	lazy.application = application;
	lazy.m_ID = pType;
	lazy.m_selfType = pType;
	AppendParameters(EventWizard.pAce, EventWizard.param_strings, EventWizard.param_types, pNewCondition->params, pNewCondition->cndID, lazy);

	ACESEntry2* acesEntry = EventWizard.pAce;

	CreateUndo();
	ReformatCondition(pNewCondition);
	
	// No parent or event, so insert new event
	if ((Parent == NULL) && (Event == NULL) && (!SubEvent))
	{
		CEditorEvent* newEvent = new CEditorEvent;
		newEvent->Init();
		newEvent->m_select(this) = true;
		newEvent->m_Conditions.push_back(pNewCondition);
		m_pEventList->push_back(newEvent);
		
		m_FocusCondition = pNewCondition;
		m_FocusEvent = newEvent;
	}
	
	// Parent, but not subevent, insert into existing
	if ((Event != NULL) && (!SubEvent))
	{
		if (acesEntry->retrn & 0x0004 || acesEntry->retrn & 0x0008)
		{
			if (!IgnoreTrigger)
			{
				// Check for existing trigger
				for (int i = 0; i < Event->m_Conditions.size(); i++)
				{
					if (Event->m_Conditions[i]->IsTrigger(application))
					{
						CErrorDlg Error;
						Error.Error("Multiple triggers", "One event line cannot contain more than one triggered condition. Please remove any existing trigger (marked with a green arrow), or create a new event.");
						return false;
					}
				}
			}
		}

		vector<CEditorCondition*>::iterator i = Event->m_Conditions.begin();
		pNewCondition->m_select(this) = true;
		if(index == -1)
			index = Event->m_Conditions.size();
		advance(i, index);
		if(i!= Event->m_Conditions.end())
			Event->m_Conditions.insert(i, pNewCondition);
		else
			Event->m_Conditions.push_back(pNewCondition);
		m_FocusCondition = pNewCondition;
	}

	// Subevent, insert below parent
	if (SubEvent)
	{
		CEditorEvent* newEvent = new CEditorEvent;
		newEvent->Init();
		newEvent->m_select(this) = true;
		newEvent->m_Conditions.push_back(pNewCondition);
		if(Event)
		{
			if(index >= 0 && index <= Event->m_EventList.size())
				Event->m_EventList.insert(Event->m_EventList.begin() + index, newEvent);
			else
				Event->m_EventList.push_back(newEvent);	
		}
		else
		{
			if(index >= 0 && index <= m_pEventList->size())
				m_pEventList->insert(m_pEventList->begin() + index, newEvent);
			else
				m_pEventList->push_back(newEvent);	
		}
		m_FocusCondition = pNewCondition;
		m_FocusEvent = newEvent;
	}

	Invalidate();
	return true;
} 


void EventSheetEditor::AppendParameters(ACESEntry2* ace, CStringArray& strings, vector<int> types,  ParamVector& params, int number,  CParametersDlg& dlg)
{
	CApplication* application = dlg.application;

	for( int p = 0; p < params.size(); p++)
			delete params[p];

	params.resize(0);

	for (int p = 0; p < strings.GetSize(); p++)
	{
		CEditorParam* param = new CEditorParam;
		param->valid = true;
		param->oid = dlg.m_ID->ObjectIdentifier;
		PARAM& src = ace->params[p];


		CString reserved = "";

		int type = types.at(p);

		if(type == 9)	//combo box...we need to work out what the number is as a selection
		{
			CStringArray strarray;
			CString Def = src.initStr;

			// Parse items and original value
			char  TokenCharacters[] = "|";
			char* CurrentToken;	

 			CurrentToken = strtok(Def.GetBuffer(Def.GetLength()), TokenCharacters);

			while(CurrentToken != NULL)
			{
				strarray.Add(CurrentToken);

				CurrentToken = strtok(NULL, TokenCharacters);
			}
	
			int line = atoi(strings.GetAt(p));
			
			if(line < 0)
			{
				reserved = "(BUG!!type is 9)";
			}
				//throw ("Error in AppendParameters function: line is lower than 0. Please warn scirra of this bug");
			else if(line >= strarray.GetSize())
			{
				reserved = "(BUG!!type is 9)";

			}
				//throw ("Error in AppendParameters function: line is greater than max. Please warn scirra of this bug");
			else
				reserved = strarray.GetAt(line);

		}

		// Menu res
		if(type == 15)
		{
			int line = atoi(strings.GetAt(p));
			list<MenuResource>::iterator i = application->resources.menu_resources.begin();
			advance(i, line);
			if(i != application->resources.menu_resources.end())
				reserved = i->GetName();
		}

		// Binary resource
		if(type == 16)
		{
			int line = atoi(strings.GetAt(p));
		}


		param->CreateFromString(strings.GetAt(p), application, types.at(p), reserved);
		params.push_back(param);
	}
}

// Form string
CString EventSheetEditor::FormString(CStringArray& strings, CString string, int objid, LPCSTR ObjectName)
{
	CStringArray strings2;
	for(int a = 0; a < strings.GetSize(); a ++)
	{
		CString str;
		str.Format("<numparam>%d</numparam><tagparam>%s</tagparam>", a, strings.GetAt(a));
		strings2.Add(str);
	}
	CString ret = (LPCSTR)string;

	// remove the <img> tags that someone seems to be using :S
	//ret.Replace("<img>", "");
	//ret.Replace("</img>", "");
	//TODO: loop backwards so u replace 10 before 1 and 0
	for (int i = 0; i < strings.GetSize(); i++)
	{
		switch (i)
		{
			case 0:
				ret.Replace("%0", strings2.GetAt(0)); break;
			case 1:
				ret.Replace("%1", strings2.GetAt(1)); break;
			case 2:
				ret.Replace("%2", strings2.GetAt(2)); break;
			case 3:
				ret.Replace("%3", strings2.GetAt(3)); break;
			case 4:
				ret.Replace("%4", strings2.GetAt(4)); break;
			case 5:
				ret.Replace("%5", strings2.GetAt(5)); break;
			case 6:
				ret.Replace("%6", strings2.GetAt(6)); break;
			case 7:
				ret.Replace("%7", strings2.GetAt(7)); break;
			case 8:
				ret.Replace("%8", strings2.GetAt(8)); break;
			case 9:
				ret.Replace("%9", strings2.GetAt(9)); break;
		}
	}

	CString icon;

		int img = 0;
		
		if(!m_familyhbitmaps.Lookup(objid, img))
			m_objecthbitmaps.Lookup(objid, img);



	icon.Format("<img>%d</img>", img);

	// Get img n counts
	int Count =    ret.Replace("</imgn>", "</imgn>");
	int Position = 0;

	for (int i = 0; i < Count; i++)
	{
		Position = ret.Find("</imgn>", Position);
		CString ID = ret.Mid(Position - 1, 1);

		CString Name = " <i>";

		CObjType* oT;
		POSITION pos = application->object_types.GetStartPosition();
		long mapKey;

		while (pos != NULL) 
		{
			application->object_types.GetNextAssoc(pos, mapKey, oT);
			if (oT->ObjectIdentifier == atoi(ID))
			{
				Name += oT->GetName();
				Name += "</img>";
				ret.Insert(Position + 7, Name);
				break;
			}
		}
	}

	// Now remove old tags
	ret.Replace("<imgn>", "<img>");
	ret.Replace("</imgn>", "</img>");

	// Other tags //ret.Replace("%o", CString(ObjectName) + " " + icon);
	ret.Replace("%o", icon);
	ret.Replace("%n", ObjectName);

	return ret;
}

CString EventSheetEditor::StripHTML(LPCSTR String)
{
	CString str = String;

	// First replace the negate icon
	str.Replace("<img>2</img>", "[negated] ");

	// Remove all <img>...</img>
	int tagIndex;

	while (1) {
		tagIndex = str.Find("<img>");

		if (tagIndex == -1)
			break;
		else {
			int closeTag = str.Find("</img>");

			// erase entire tag
			str.Delete(tagIndex, (closeTag - tagIndex) + 6);
		}
	}

	// remove all <special_image>...</special_image>
	while (1) {
		tagIndex = str.Find("<special_image>");

		if (tagIndex == -1)
			break;
		else {
			int closeTag = str.Find("</special_image>");

			// erase entire tag
			str.Delete(tagIndex, (closeTag - tagIndex) + 16);
		}
	}

	CString Result = str;
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

	// Collapse spaces in Work
	while (Work.Replace("  ", " ") != 0);

	// Collapse " ." to "."
	Work.Replace(" .", ".");

	// Replace &amp; with &
	Work.Replace("&amp;", "&");

	return Work;
}

void EventSheetEditor::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CChronoEventEditor::OnVScroll(nSBCode, nPos, pScrollBar);
	//layout->m_EventEditorScroll.y = nPos;
}

void EventSheetEditor::OnSize(UINT nType, int cx, int cy) 
{
	CChronoEventEditor::OnSize(nType, cx, cy);
}



void EventSheetEditor::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_ActionTooltip.Hide();
	m_InsertAfter.Hide();
	m_InsertBefore.Hide();
	m_InsertSub.Hide();
	KillFocusOnInlineEditor();

	int place = -1;

	for (int x = 0; x < m_pEventList->size(); x++)
	{
		if (m_pEventList->at(x)->m_rect(this).PtInRect(point))
			place = x;
	}

	CEditorEvent* pEvent = EventAt(point);

	if (pEvent)
	{
		if(!pEvent->m_select(this))
		DeselectEvents();
		pEvent->m_select(this) = true;
		m_FocusEvent = pEvent;

		Invalidate();
		if (pEvent->m_type == 1)	//comment
		{
			// Cursor position
			POINT cursorPosition;
			GetCursorPos(&cursorPosition);

			// Load popup context menu
			CExtPopupMenuWnd* Popup = new CExtPopupMenuWnd;
			Popup->CreatePopupMenu(m_hWnd);

			// Append new items
			Popup->ItemInsertCommand(1, -1, RIBBON_CUT, NULL, NULL);
			Popup->ItemInsertCommand(2, -1, RIBBON_COPY, NULL, NULL);
			Popup->ItemInsertCommand(3, -1, RIBBON_PASTE, NULL, NULL);
			Popup->ItemInsertCommand(4, -1, EE_DELETECOMMENT, NULL, NULL);
			Popup->ItemInsertCommand(5, -1, EE_EDITCOMMENT, NULL, NULL);

			// Display it
			UINT ChosenItem = 0;
			Popup->TrackPopupMenu(TPMX_DO_MESSAGE_LOOP | TPMX_NO_WM_COMMAND, cursorPosition.x, cursorPosition.y, NULL, NULL, NULL, &ChosenItem);

			if (ChosenItem == 1)
				OnToolbarCut();

			// Copy
			if (ChosenItem == 2)
				OnToolbarCopy();

			// Paste
			if (ChosenItem == 3)
				OnToolbarPaste();

			// Delete comment chosen
			if (ChosenItem == 4)
			{
				CreateUndo();

				// Delete comment
				CEditorEvent* ev = EventAt(point);
				ev->Remove(true, this);
			}

			// Edit comment chosen
			if (ChosenItem == 5)
			{
				CEditorEvent* pComment = EventAt(point);
				EditComment(pComment);
			}

			return;
		}

		if(pEvent->m_type == INCLUDE_TYPE)
		{
			// Cursor position
			POINT cursorPosition;
			GetCursorPos(&cursorPosition);

			// Load popup context menu
			CExtPopupMenuWnd* Popup = new CExtPopupMenuWnd;
			Popup->CreatePopupMenu(m_hWnd);

			// Append new items
			Popup->ItemInsertCommand(1, -1, EE_DELETEINCLUDE, NULL, NULL);
			Popup->ItemInsertCommand(2, -1, EE_GOTOINCLUDE, NULL, NULL);

			Popup->ItemInsertCommand();

			int index  = 100;
			for(list<EventSheet*>::iterator i = ((EventSheetEditor*)this)->application->event_sheets.begin();
				i != ((EventSheetEditor*)this)->application->event_sheets.end();
				i++)
			{
				Popup->ItemInsertCommand(index, -1, (*i)->GetName(), NULL, NULL);
				index ++;
			}

			// Display it
			UINT ChosenItem = 0;
			Popup->TrackPopupMenu(TPMX_DO_MESSAGE_LOOP | TPMX_NO_WM_COMMAND, cursorPosition.x, cursorPosition.y, NULL, NULL, NULL, &ChosenItem);

			// Delete include
			if (ChosenItem == 1)
			{
				CreateUndo();
				CEditorEvent* ev = EventAt(point);
				ev->Remove(true, this);
			}

			// Go-to include
			if (ChosenItem == 2)
			{
				CEditorEvent* pInclude = EventAt(point);

				if (!pInclude) return;

				EventSheet* pEventSheet = application->FindEventSheetFromNumber(pInclude->event_sheet_include);

				
				OpenEventSheet(pEventSheet);
			}

			// Edit include
			else if (ChosenItem >= 100)
			{
				CreateUndo();
				CEditorEvent* pInclude = EventAt(point);
				list<EventSheet*>::iterator i = ((EventSheetEditor*)this)->application->event_sheets.begin();
				advance(i, ChosenItem - 100);
				pInclude->event_sheet_include = (*i)->identifier;
				
				Invalidate();	
			}

			return;
		}

		// Condition column, handle ( so conditions or the event itself)
		if (point.x < m_pHeader->m_Split)
		{
			HandleConditionColumnRightClick(point);
		}

		// Action column, handle
		else
		{
			if (pEvent->m_gridRec(this).PtInRect(point))
				HandleGridRightClick(point);

			else
			{
				// Get action at point
				CEditorAction* Action = pEvent->actionAt(point, this);
				if (Action == NULL)
				{
					DeselectActions();
					return;
				}

				DeselectEvents();
				DeselectConditions();
				if(!Action->m_select(this))
					DeselectActions();
				Action->m_select(this) = true;

				m_FocusAction = Action;	
				m_FocusEvent = pEvent;

				Invalidate();
				
				// Cursor position
				POINT cursorPosition;
				GetCursorPos(&cursorPosition);

				// Load popup context menu
				CExtPopupMenuWnd* popup = new CExtPopupMenuWnd;
				popup->CreatePopupMenu(m_hWnd);

				// Append new items
				popup->ItemInsertCommand(1, -1, RIBBON_CUT, NULL, NULL);
				popup->ItemInsertCommand(2, -1, RIBBON_COPY, NULL, NULL);
				popup->ItemInsertCommand(3, -1, RIBBON_PASTE, NULL, NULL);
				popup->ItemInsert();

				popup->ItemInsertCommand(5, -1, EE_EDITACTION, NULL, NULL);	
				popup->ItemInsertCommand(4, -1, EE_DELETEACTION, NULL, NULL);
				popup->ItemInsertCommand(6, -1, EE_TOGGLEACTION, NULL, NULL);

				CObjType* objectType = application->FindObjTypeFromNumber(Action->oid);
				ACEMenuMap map;

				if (Action->params.size() == 0)
					popup->ItemEnabledSet(popup->ItemFindPosForCmdID(4), false);

				// Display it
				UINT ChosenItem = 0;
				popup->TrackPopupMenu(TPMX_DO_MESSAGE_LOOP | TPMX_NO_WM_COMMAND, cursorPosition.x, cursorPosition.y, NULL, NULL, NULL, &ChosenItem);

				if (ChosenItem == 1)
					OnToolbarCut();

				// Copy
				if (ChosenItem == 2)
					OnToolbarCopy();

				// Paste
				if (ChosenItem == 3)
					OnToolbarPaste();

				if (ChosenItem == 4)
				{
					CreateUndo();
					Action->Remove(true, this);
					Invalidate();
				}

				if (ChosenItem == 5)
				{
					EditAction(Action);
					Invalidate();
				}

				// Toggle Action
				if (ChosenItem == 6)
				{
					CreateUndo();
					if(Action)
					{
						Action->m_bEnabled = !Action->m_bEnabled;
						InvalidateRect(Action->m_rect(this));
					}
				}

				if (ChosenItem > 20) //We are changing the action
				{
					CreateUndo();
					MenuMapItem item;
					map.Lookup(ChosenItem, item);
					int i;

					ACESEntry2* entry = objectType->GetACESEntry(ACTION, item, i);
				
					Action->actID = i;

					while(entry->params.size() < Action->params.size())
					{
						delete Action->params.back();
						Action->params.pop_back();
					}
					while(entry->params.size() > Action->params.size())
					{
						CEditorParam* param = new CEditorParam;
						param->oid = Action->oid;
						Action->params.push_back(param);
						Action->params.back()->m_type = entry->params.at(Action->params.size()-1).type;
					}
					for(int a = 0; a < entry->params.size(); a++)
					{
						CEditorParam* EdParam = Action->params.at(a);
						PARAM& Param = entry->params.at(a);

						if(EdParam->m_type != Param.type)
						{
							CString Default = Param.initStr;
							if(Param.type == 9)
								Default = "0";
							if (Default == "")
							{
								
								switch (Param.type)
								{
								case 1: Default = "0"; break;
								case 2: Default = "\"\""; break;
								case 11: Default = "\"\""; break;
								default: Default = "0"; break;
								}
							}
							EdParam->CreateFromString(Default, application, Param.type, "");
						}
					}
					
					switch (item.type)
					{
					case 0: // normal
						Action->mid = -1;
						break;
					case 1: // movement
						Action->mid = item.child;
						break;
					case 2:
						Action->mid = item.child * -1;
						break;
					}
					ReformatAction(Action, true);
					Invalidate();
				}
			}
		}
	}
	else		// We aren't clicking on an event
	{
		// Cursor position
		POINT cursorPosition;
		GetCursorPos(&cursorPosition);

		// Load popup context menu
		CExtPopupMenuWnd* Popup = new CExtPopupMenuWnd;
		Popup->CreatePopupMenu(m_hWnd);

		// If we can paste, allow that
		CString csF = m_pDDMgr->AvailableDataType(NULL);
		if (m_pDDMgr->OkToPaste())
		{
			Popup->ItemInsertCommand(6, -1, RIBBON_PASTE, NULL, NULL);
			Popup->ItemInsert();
		}

		// Append new items
		Popup->ItemInsertCommand(1, -1, EE_INSERTEVENT, NULL, NULL);
		Popup->ItemInsertCommand(5, -1, EE_INSERTSCRIPT, NULL, NULL);
		Popup->ItemInsertCommand(2, -1, EE_INSERTCOMMENT, NULL, NULL);
		Popup->ItemInsertCommand(3, -1, EE_INSERTGROUP, NULL, NULL);
		Popup->ItemInsertCommand(4, -1, EE_INCLUDEEVENTSHEET, NULL, NULL);

		// Add icons
		CExtCmdIcon icon;

		icon.AssignFromHBITMAP(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_RIBBON_ADDEVENT)), RGB(255, 255, 255));
		Popup->ItemSetPopupIcon(Popup->ItemFindPosForCmdID(1), icon);

		icon.AssignFromHBITMAP(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_COMMENT_NEW_LARGE)), RGB(255, 255, 255));
		SIZE s;
		s.cx = 16;
		s.cy = 16;
		icon.Scale(s);
		Popup->ItemSetPopupIcon(Popup->ItemFindPosForCmdID(2), icon);

		icon.AssignFromHBITMAP(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_RIBBON_ADDGROUP)), RGB(255, 255, 255));
		icon.Scale(s);
		Popup->ItemSetPopupIcon(Popup->ItemFindPosForCmdID(3), icon);

		// Display it
		UINT ChosenItem = 0;
		Popup->TrackPopupMenu(TPMX_DO_MESSAGE_LOOP | TPMX_NO_WM_COMMAND, cursorPosition.x, cursorPosition.y, NULL, NULL, NULL, &ChosenItem);

		switch(ChosenItem)
		{
		case 1:	// Insert Event
			AddCondition();
			break;

		case 2:	// Insert Comment
			OnExpandComment();
			
			break;

		case 3: // Insert Group
			AddGroup();
			break;

		case 4:
			AddEventSheet();
			break;

		case 5:
			AddScript();
			break;

		case 6:
			OnToolbarPaste();
			break;
		}
		return;
	}
		
	CChronoEventEditor::OnRButtonUp(nFlags, point);
}

void EventSheetEditor::EditComment(CEditorEvent* pComment)
{
	m_CommentDlg.theComment = pComment->m_cText;
	m_CommentDlg.colour = pComment->m_cTextBG;
	m_CommentDlg.fontcolour = pComment->m_cTextColour;
	strcpy(m_CommentDlg.lf.lfFaceName, pComment->m_cFontName);
	
	m_CommentDlg.lf.lfHeight = pComment->m_cFontSize;
	if (pComment->m_cbFontBold) m_CommentDlg.lf.lfWeight = FW_BOLD;
	m_CommentDlg.lf.lfItalic = pComment->m_cbFontItalic;
	m_CommentDlg.lf.lfStrikeOut = pComment->m_cbFontStrike;
	m_CommentDlg.lf.lfUnderline = pComment->m_cbFontUnderline;

	if (m_CommentDlg.DoModal() == IDOK)
	{
		CreateUndo();
		if (m_CommentDlg.theComment == "") m_CommentDlg.theComment = " ";

		pComment->m_cText = m_CommentDlg.theComment;
		pComment->m_cTextBG = m_CommentDlg.colour;
		pComment->m_cTextColour = m_CommentDlg.fontcolour;
		pComment->m_cFontName = m_CommentDlg.lf.lfFaceName;
		pComment->m_cFontSize =m_CommentDlg.lf.lfHeight;
		pComment->m_cbFontBold = m_CommentDlg.lf.lfWeight == FW_BOLD? true : false;
		pComment->m_cbFontItalic = m_CommentDlg.lf.lfItalic;
		pComment->m_cbFontStrike = m_CommentDlg.lf.lfStrikeOut;
		pComment->m_cbFontUnderline = m_CommentDlg.lf.lfUnderline;

		Invalidate();
	}
}

void EventSheetEditor::AddEventSheet()
{
	CreateUndo();
	CEditorEvent* evnt = new CEditorEvent();
	evnt->Init();
	evnt->m_type = INCLUDE_TYPE;
	evnt->event_sheet_include = -1;
	evnt->m_select(this) = true;
	m_pEventList->push_back(evnt);
	Invalidate();

}
void EventSheetEditor::EditImage(CEditorAction*	Action, CEditorImage* Image)
{
	if(EditImage(Image))
		ReformatAction(Action);

}
void EventSheetEditor::EditImage(CEditorCondition*  Condition, CEditorImage* Image)
{
	if(EditImage(Image))
		ReformatCondition(Condition);
}
bool EventSheetEditor::EditImage(CEditorImage* Image)
{


	int oid = 0;
	if( Image->tok )
		oid = Image->tok->oidOwner;
//	if( Image->id)
//		oid = *Image->id;

	CString old;
	old.Format("%d", oid);

	ObjectSelectorDialog ObjectSelector(application, layout, old, oid);

	if( ObjectSelector.DoModal() == IDOK) 
	{
		CreateUndo();
		if( Image->tok )
		{
			if (ObjectSelector.selected.is_family)
			{
				Image->tok->oidOwner = ObjectSelector.selected.family->identifier;
				Image->tok->str.Format("%d", ObjectSelector.selected.family->identifier);
			}

			else
			{
				Image->tok->oidOwner = ObjectSelector.selected.type->ObjectIdentifier;
				Image->tok->str.Format("%d", ObjectSelector.selected.type->ObjectIdentifier);
			}

			Image->tok->length = Image->tok->str.GetLength();
		}
	}

	return true;
}



void EventSheetEditor::EditAction(CEditorAction* Action, int step)
{
	CObjType* objectType = application->FindObjTypeFromNumber(Action->oid);
	if(!objectType)
		return;

	CEventWizardDlg EventWizard;
	EventWizard.m_aceType = ACTION;
	EventWizard.layout = layout;
	EventWizard.application = application;
	EventWizard.SetMap(&application->object_types);
	EventWizard.m_childid = Action->mid;

	EventWizard.m_pSelectedObject = Action->oid;
	EventWizard.m_aceIndex = Action->actID;
	EventWizard.m_iStep = step; // most commonly people will want to edit the params
	if(step == 3 && Action->params.size() == 0)
		EventWizard.m_iStep = 2; // If we have no params...the user probably wants to select another action

	// Add parameters
	for (int o = 0; o < Action->params.size(); o++)
	{
		EventWizard.param_strings.Add(Action->params.at(o)->CreateParamString(this));
		EventWizard.param_types.push_back(Action->params.at(o)->m_type);
	}

	if(EventWizard.DoModal() != IDOK)
		return;

	objectType = application->FindObjTypeFromNumber(EventWizard.m_pSelectedObject);
	if(!objectType)
		return;
		//UNDO INFO
	CreateUndo();

	Action->actID = EventWizard.m_aceIndex;
	Action->mid = EventWizard.m_childid;
	Action->oid = objectType->ObjectIdentifier;
	Action->m_Object = objectType->GetName();
	Action->DLLIndex = objectType->DLLIndex;

	// Some time I'll make AppendParameters not require CParametersDlg as a parameter to the function
	CParametersDlg lazy;
	lazy.application = application;
	lazy.m_ID = objectType;
	lazy.m_selfType = objectType;
	AppendParameters(EventWizard.pAce, EventWizard.param_strings, EventWizard.param_types, Action->params, Action->actID, lazy);


	this->ReformatAction(Action);
	Invalidate();
	
}

// Edit a condition
void EventSheetEditor::EditCondition(CEditorCondition* Condition, int step)
{
	CObjType* objectType = application->FindObjTypeFromNumber(Condition->oid);
	if(!objectType)
		return;

	CEventWizardDlg EventWizard;
	EventWizard.m_aceType = CONDITION;
	EventWizard.layout = layout;
	EventWizard.application = application;
	EventWizard.SetMap(&application->object_types);
	EventWizard.m_childid = Condition->mid;

	EventWizard.m_pSelectedObject = Condition->oid;
	EventWizard.m_aceIndex = Condition->cndID;
	EventWizard.m_iStep = step; // default to 3
	if(step == 3 && Condition->params.size() == 0)
		EventWizard.m_iStep = 2; // If we have no params...the user probably wants to select another action

	// Add parameters
	for (int o = 0; o < Condition->params.size(); o++)
	{
		EventWizard.param_strings.Add(Condition->params.at(o)->CreateParamString(this));
		EventWizard.param_types.push_back(Condition->params.at(o)->m_type);
	}

	if(EventWizard.DoModal() != IDOK)
		return;

	objectType = application->FindObjTypeFromNumber(EventWizard.m_pSelectedObject);
	if(!objectType)
		return;

	CreateUndo();

	Condition->cndID = EventWizard.m_aceIndex;
	Condition->mid = EventWizard.m_childid;
	Condition->oid = objectType->ObjectIdentifier;
	Condition->m_Object = objectType->GetName();
	Condition->DLLIndex = objectType->DLLIndex;

	// Some time I'll make AppendParameters not require CParametersDlg as a parameter to the function
	CParametersDlg lazy;
	lazy.application = application;
	lazy.m_ID = objectType;
	lazy.m_selfType = objectType;
	AppendParameters(EventWizard.pAce, EventWizard.param_strings, EventWizard.param_types, Condition->params, Condition->cndID, lazy);

	this->ReformatCondition(Condition);
	Invalidate();
}

void EventSheetEditor::HandleGridRightClick(CPoint point)
{

}


void EventSheetEditor::ReformatAction(CEditorAction*	Action, bool ReworkParams)
{
	//TODO: Error checking
	// That's reassuring

	// We find our action entry
	
	long id = Action->oid;
	
	CObjType* objectType = application->FindObjTypeFromNumber(id);
	if(!objectType)
		return;
	ACESEntry2* acesEntry = 0;

	if(Action->mid == -1)		// ace or custom ace
		 acesEntry = objectType->GetACESEntry(ACTION, Action->actID);
	else if(Action->mid > -1)					// movement			
	{
		CBehavior* mi = objectType->GetBehaviorByUniqueID(Action->mid);
		if(!mi)
			return;
		acesEntry = GetOINFO(mi->BehaviorDLLIndex)->GetACEEntry(ACTION,Action->actID);					
	}
	else if(Action->mid < -1)					// effect			
	{
		int effectnumber = 0 - Action->mid;
		EffectIterator ei = objectType->effects.begin();

		for(EffectIterator ei = objectType->effects.begin(); ei!= objectType->effects.end(); ei++)
		{
			if(ei->m_effectID == effectnumber)
			{
				acesEntry = ei->GetACESEntry(ACTION,Action->actID);	
			}
		}				
	}
	if(!acesEntry)
		return;
	// Work out all the parameters
	CStringArray strings;
	vector<int> types;
	Action->m_imgs.clear();
	if(ReworkParams)
	{
		CString param;
		for(int p = 0; p < Action->params.size(); p++)
		{
			param = Action->params[p]->CreateParamString(this);
			strings.Add(param);
			types.push_back(Action->params[p]->m_type);
			if(!Action->params[p]->valid)
				return;	// we dont reformat the entire action if a param is invalid because as far as we care its dead
		}
		// This is a hack. Basically the AppendParameters function uses the member variables in the ParameterDlg, but
		// doesn't require the dialog to actually be made
		CParametersDlg lazy;
		lazy.application = this->application;
		lazy.m_ID = objectType;
		lazy.m_selfType = objectType;
		AppendParameters(acesEntry, strings, types, Action->params, Action->oid, lazy);
		strings.RemoveAll();
		types.clear();
	}
	
	
	for(int p = 0; p <  Action->params.size(); p++)
		strings.Add( Action->params[p]->CreateFormattedParamString(this, Action, NULL ));

	Action->m_Text = FormString(strings, acesEntry->aceDisplayText, Action->oid, objectType->GetName());
	Action->m_Object =  objectType->GetName();
	

	// We have all the info to start reformatting
	// Really? Where does the reformatting occur, there was merely 10 blank lines here
}

void EventSheetEditor::ReformatCondition(CEditorCondition*	Condition, bool ReworkParams)
{
	//TODO: Error checking
	// That's kind of important

	// We find our action entry
	long id = Condition->oid;
	
	CObjType* objectType = application->FindObjTypeFromNumber(id);
	if(!objectType)
		return;
	ACESEntry2* acesEntry;

	if(Condition->mid == -1) // ace or custom ace
		acesEntry = objectType->GetACESEntry(CONDITION, Condition->cndID);
	else // movement			
	{
		CBehavior* mi = objectType->GetBehaviorByUniqueID(Condition->mid);
		if(!mi)
			return;
		acesEntry = GetOINFO(mi->BehaviorDLLIndex)->GetACEEntry(CONDITION,Condition->cndID);
	}

	if(!acesEntry)
		return;

	// Work out all the parameters
	CStringArray strings;
	vector<int> types;

	Condition->m_imgs.clear();

	if(ReworkParams)
	{
		for(int p = 0; p < Condition->params.size(); p++)
		{
			strings.Add( Condition->params[p]->CreateParamString(this) );
			types.push_back(Condition->params[p]->m_type);
			if(!Condition->params[p]->valid)
				return;	// we dont reformat the entire condition if a param is invalid because as far as we care its dead
		}
		// This is a hack. Basically the AppendParameters function uses the member variables in the ParameterDlg, but
		// doesn't require the dialog to actually be made

		CParametersDlg lazy;
		lazy.application = this->application;
		lazy.m_ID = objectType;
		lazy.m_selfType = objectType;
		AppendParameters(acesEntry, strings, types, Condition->params, Condition->oid, lazy);
		strings.RemoveAll();
		types.clear();
	}

	for(int p = 0; p <  Condition->params.size(); p++)
		strings.Add( Condition->params[p]->CreateFormattedParamString(this, NULL, Condition));

	Condition->m_Text = FormString(strings, acesEntry->aceDisplayText, Condition->oid, objectType->GetName());
	Condition->m_Object =  objectType->GetName();

	if (acesEntry->retrn & 0x0004 || acesEntry->retrn & 0x0008)
	{
		CString Add = "<img>1</img>";
		Add += Condition->m_Text;
		Condition->m_Text = Add;
	}

	if(Condition->m_Negate)
	{
		CString Add = "<img>2</img>";
		Add += Condition->m_Text;
		Condition->m_Text = Add;
	}
}

void EventSheetEditor::ReformatEvent(CEditorEvent*	Event)
{
	for(int a = 0; a < Event->m_Actions.size(); a++ )
		ReformatAction(Event->m_Actions[a], true);
	
	for(int a = 0; a < Event->m_Conditions.size(); a++ )
		ReformatCondition(Event->m_Conditions[a], true);

	for(int e = 0; e < Event->m_EventList.size(); e ++)
		ReformatEvent(Event->m_EventList[e]);

}
void EventSheetEditor::Reformat()
{
	if(m_pEventList) {
		for(int e = 0; e < m_pEventList->size(); e ++)
			ReformatEvent(((*m_pEventList)[e]));
	}

}
		


void EventSheetEditor::OnDestroy() 
{
	CMainFrame* pMainFrame = ((CMainFrame*)(EventSheetEditor::GetParentOwner()));
	m_LastRibbon = pMainFrame->m_Ribbon.Ribbon_PageSelectionGet();

	CChronoEventEditor::OnDestroy();
}

void EventSheetEditor::CreateIconBitmaps()
{
	DeleteIconBitmaps();

	// System icon
	m_hbitmaps.push_back(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SYSTEMC)));

	// Trigger icon
	m_hbitmaps.push_back(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_TRIGGERC)));

	// Negate icon
	m_hbitmaps.push_back(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_NOTEQUAL)));

	// Loop icon
	m_hbitmaps.push_back(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_LOOPC)));


	CObjTypeMap* objTypeMap = &application->object_types;
	CObjType* oT;

	POSITION pos = objTypeMap->GetStartPosition();
	long mapKey;

	// Loop through and add the objects
	while (pos != NULL) 
	{
		objTypeMap->GetNextAssoc(pos, mapKey, oT);
		m_objecthbitmaps.SetAt(oT->ObjectIdentifier, m_hbitmaps.size());
		m_hbitmaps.push_back(oT->small_image.MakeBitmap());	
	}

	// now loop all the families

	for(list<Family>::iterator i = application->families.begin(); i != application->families.end(); i++)
	{
		if(i->is_in_use)
		{
			m_objecthbitmaps.SetAt(i->identifier, m_hbitmaps.size());
			m_familyhbitmaps.SetAt(i->identifier, m_hbitmaps.size());
			m_hbitmaps.push_back(i->small_image.MakeBitmap());
		}
	}
}

// Find and replace functionality
void EventSheetEditor::OnFindReplace() 
{
	if (m_FindReplaceDlg.m_hWnd)
		m_FindReplaceDlg.DestroyWindow();

	m_FindReplaceDlg.m_pEventList = m_pEventList;
	m_FindReplaceDlg.pChrono = this;

	m_FindReplaceDlg.Create(IDD_FINDREPLACE, this);
	m_FindReplaceDlg.ShowWindow(SW_SHOW);
}

void EventSheetEditor::RecurseEdit(CEditorEvent* pEvent, int iType)
{
	for (int i = 0; i < pEvent->m_EventList.size(); i++)
	{
		pEvent->m_EventList[i]->m_displayActionType(this) = iType;
		RecurseEdit(pEvent->m_EventList[i], iType);
	}
}

CEditorEvent* EventSheetEditor::FindEventParent(CEditorEvent* pEvent, CEditorEvent* pEventParent)
{
	EventVector* eventlist;
	if(pEventParent)
		eventlist = &pEventParent->m_EventList;
	else
		eventlist = m_pEventList;

	for(int i = 0; i < eventlist->size(); i++)
	{
		if( (*eventlist)[i] == pEvent)
			return pEventParent; 
		CEditorEvent* ret = FindEventParent(pEvent, (*eventlist)[i]);
		if(ret)
			return ret;
	}
	return NULL;
}

EventVector* EventSheetEditor::FindEventListParent(CEditorEvent* pEvent, EventVector* eventlist)
{
	for(int i = 0; i < eventlist->size(); i++)
	{
		if( (*eventlist)[i] == pEvent)
			return eventlist; 
		EventVector* ret = FindEventListParent(pEvent, &(*eventlist)[i]->m_EventList);
		if(ret)
			return ret;
	}
	return NULL;
}

// Change all to chronological
void EventSheetEditor::OnAllChronological() 
{
	CreateUndo();
	for (int i = 0; i < m_pEventList->size(); i++)
	{
		m_pEventList->at(i)->m_displayActionType(this) = 1;
		RecurseEdit(m_pEventList->at(i), 1);
	}

	Invalidate();
}

// Change all to list
void EventSheetEditor::OnAllList() 
{
	CreateUndo();
	for (int i = 0; i < m_pEventList->size(); i++)
	{
		m_pEventList->at(i)->m_displayActionType(this) = 3;
		RecurseEdit(m_pEventList->at(i), 3);
	}

	Invalidate();
}

void EventSheetEditor::DeleteIconBitmaps()
{
	for(int a=0; a < m_hbitmaps.size(); a ++)
		DeleteObject(m_hbitmaps.at(a));

	m_hbitmaps.clear();
	m_objecthbitmaps.RemoveAll();
	m_familyhbitmaps.RemoveAll();
}


bool EventSheetEditor::InlineEditParam(ParamVector& params, CPoint pt)
{
	for(int p = 0; p < params.size(); p++)
	{
		CEditorParam& param = *params[p];
		for(int d = 0; d < param.displayrects.size(); d++)
		{
			if(param.displayrects[d].PtInRect(pt))
			{
				m_inline_param = &param;

				if((param.m_type != 1) && (param.m_type != 2) && (param.m_type != 6) && (param.m_type != 0))	// only allow editing of stringish stuff
					continue;

				// We dont want these annoying things on the screen when inline editing!
				m_InsertAfter.Hide();
				m_InsertBefore.Hide();
				m_InsertSub.Hide();

				// Edit with scintilla
				if (param.m_type == 1 || param.m_type == 2)
				{
					CScintillaWnd* pContent = &m_pInlineScintilla;
					CWnd* pParent = this;

					if(pContent->GetSafeHwnd())
						pContent->DestroyWindow();

					CRect Pos = param.displayrects[0];
					Pos.top = Pos.bottom - 14;
					Pos.bottom = Pos.top + 18;
					Pos.right = Pos.left + 300;

					Pos.OffsetRect(-16,-2);

					pContent->Create("Scintilla", WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL|WS_BORDER, Pos, this,10000);
					pContent->ID = 0;
					pContent->ModifyStyleEx(0,WS_EX_WINDOWEDGE,0);

					pContent->SetDisplayLinenumbers(false);
					pContent->SetDisplaySelection(0);

					pContent->SetDisplayFolding(TRUE);

					pContent->SendMessage(SCI_SETKEYWORDS, 0, 
						(long)_T("System and and_eq asm auto bitand bitor bool break case catch char class compl const const_cast continue default delete do double dynamic_cast else enum m_TypeCheckerlicit m_TypeCheckerort extern false float for friend goto if inline int long mutable namespace new not not_eq operator or or_eq private protected public register reinterpret_cast return short signed sizeof static static_cast struct switch template this throw true try typedef typeid typename union unsigned using virtual void volatile wchar_t while xor xor_eq"));

					pContent->SendMessage(SCI_SETVSCROLLBAR, NULL, false);
					pContent->SendMessage(SCI_SETHSCROLLBAR, NULL, false);
					
					m_Original = param.CreateParamString(this);
					pContent->SetWindowText(param.CreateParamString(this));

					pContent->SetFocus();
					pContent->SelectAll();
				}

				// Object parameter
				else
				{
					//
				}

				return true;
			}
		}
	}

	return false;
}

void EventSheetEditor::InlineEditCondition(CEditorCondition& condition, CPoint pt)
{
	// First check if we are clicking on any of the images
	for(int i = 0; i < condition.m_imgs.size(); i ++)
	{
		if(condition.m_imgs.at(0).m_Rect.PtInRect(pt))
		{
			return;
		}
	}
	//find what param we are editing.
	InlineEditParam(condition.params, pt);
}
void EventSheetEditor::InlineEditAction(CEditorAction& action, CPoint pt)
{
	//First check if we are clicking on any of the images
	for(int i = 0; i < action.m_imgs.size(); i ++)
	{
		if(action.m_imgs.at(0).m_Rect.PtInRect(pt))
		{
			return;
		}
	}

	InlineEditParam(action.params, pt);
}

void CreateEventSelectionVector(SelectedEventVector & pSelectedEventList,EventVector& EventList, CChronoEventEditor* pEd);
void CreateConditionSelectionVector(SelectedConditionVector & pSelectedConditionList,EventVector& EventList, CChronoEventEditor* pEd);
void CreateActionSelectionVector(SelectedActionVector & pSelectedActionList,EventVector& EventList, CChronoEventEditor* pEd);

bool EventSheetEditor::KillFocusOnInlineEditor(void)
{
	if(m_pInlineScintilla.GetSafeHwnd())
	{
		CString ret;
		m_pInlineScintilla.GetText(ret);

		CTypeChecker typeChecker;
		typeChecker.pApp = application;
		typeChecker.objMap = &(application->object_types);

		if (m_inline_param == NULL)
			return false;

		typeChecker.pCallerType = application->FindObjTypeFromNumber(m_inline_param->oid);

		int chPos;

		// Validate expression
		try {
			typeChecker.ParseUserString(ret, &chPos);
		}
		catch (runtime_error e) {
			CErrorDlg Dlg;
			Dlg.Error("Error in expression", e.what());

			m_pInlineScintilla.DestroyWindow();
			return false;
		}
		catch (...) {
			CErrorDlg Dlg;
			Dlg.Error("Error in expression", "Unparsable expression.  Please check your expression is correctly written.");

			m_pInlineScintilla.DestroyWindow();
			return false;
		}

		try {
			typeChecker.Validate(chPos);
		}
		catch (runtime_error e) {
			CErrorDlg Dlg;
			Dlg.Error("Error in expression", e.what());

			m_pInlineScintilla.DestroyWindow();		
			return false;
		}
		catch (...) {
			CErrorDlg Dlg;
			Dlg.Error("Error in expression", "Unvalidatable expression.  Please check your expression is correctly written.");

			m_pInlineScintilla.DestroyWindow();
			return false;
		}

		ExpType type;

		try {
			type = typeChecker.TypeCheck();
		}
		catch (runtime_error e) {
			CErrorDlg Dlg;
			Dlg.Error("Error in expression", e.what());

			m_pInlineScintilla.DestroyWindow();
			return false;
		}
		catch (...) {
			CErrorDlg Dlg;
			Dlg.Error("Error in expression", "Uncheckable expression.  Please check your expression is correctly written.");

			m_pInlineScintilla.DestroyWindow();
			return false;
		}

		CreateUndo();

		// Expression OK
		this->m_inline_param->CreateFromString(ret, application, this->m_inline_param->m_type, "");

		m_pInlineScintilla.DestroyWindow();

		SelectedConditionVector SelectedConditions;
		CreateConditionSelectionVector(SelectedConditions,(*m_pEventList));

		for(int c = 0; c < SelectedConditions.size(); c++)
			ReformatCondition(SelectedConditions[c]);

		SelectedActionVector SelectedActions;
		CreateActionSelectionVector(SelectedActions,(*m_pEventList));

		for(int a = 0; a < SelectedActions.size(); a++)
		{
			ReformatAction(SelectedActions[a]);
			InvalidateRect(SelectedActions[a]->m_rect(this));
		}

		SelectedEventVector SelectedEvents;
		CreateEventSelectionVector(SelectedEvents,(*m_pEventList));

		for(int e = 0; e < SelectedEvents.size(); e++)
			ReformatEvent(SelectedEvents[e]);

		Invalidate();

		return true;
	}
	return false;
}

void EventSheetEditor::OnTimer(UINT nIDEvent) 
{	
	if (nIDEvent == 1)
	{
		KillTimer(1);

		KillFocusOnInlineEditor();
	}

	else
		CChronoEventEditor::OnTimer(nIDEvent);
}

void EventSheetEditor::OnChar(NMHDR* pNMHDR, LRESULT* pResult)
{
	SCNotification *scn = (SCNotification*)pNMHDR;

	// Check the key isn't enter
	if (scn->ch == 13)
	{
		CString Old;
		m_pInlineScintilla.GetWindowText(Old);

		Old.Replace("\r\n", "");

		if (Old != "")
			m_pInlineScintilla.SetWindowTextA(Old);
		else
			m_pInlineScintilla.SetWindowTextA(m_Original);

		m_pInlineScintilla.EnableWindow(FALSE);

		SetTimer(1, 50, NULL);

		return;
	}

	CParamScintilla::OnChar(pNMHDR, pResult,m_pInlineScintilla,& this->application->object_types,
		 m_Tooltip,
		 m_ParameterTooltipIsVisible,
		 m_QuotesOpen, application, true);
}

void EventSheetEditor::Insert_ClickAfter()
{
	if(m_InsertAfter.m_InsertType == ACTION)
		AddAction(m_InsertAfter.m_pInsertEventSpot, m_InsertAfter.m_InsertIndex+1);
	if(m_InsertAfter.m_InsertType == CONDITION)
		AddCondition(false, NULL, false, m_InsertAfter.m_pInsertEventSpot, false, m_InsertAfter.m_InsertIndex+1);
	if(m_InsertAfter.m_InsertType == EVENT)
		AddCondition(false, NULL, true, m_InsertAfter.m_pInsertEventSpot, false, m_InsertAfter.m_InsertIndex+1);
}

void EventSheetEditor::Insert_ClickBefore()
{
	if(m_InsertBefore.m_InsertType == ACTION)
		AddAction(m_InsertBefore.m_pInsertEventSpot, m_InsertBefore.m_InsertIndex);
	if(m_InsertBefore.m_InsertType == CONDITION)
		AddCondition(false, NULL, false, m_InsertBefore.m_pInsertEventSpot, false, m_InsertBefore.m_InsertIndex);
	if(m_InsertBefore.m_InsertType == EVENT)
		AddCondition(false, NULL, true, m_InsertBefore.m_pInsertEventSpot, false, m_InsertBefore.m_InsertIndex);
}

void EventSheetEditor::Insert_ClickSub()
{
	if(m_InsertAfter.m_InsertType == EVENT)
		AddCondition(false, NULL, true, m_InsertSub.m_pInsertEventSpot, false, m_InsertSub.m_InsertIndex+1);
}


void EventSheetEditor::AddScript()
{
	// Check if python scripting is enabled and warn if not
	if (!application->game_information.use_python) {
		int ret = MessageBox(EE_PYTHON, EE_PYTHONTITLE, MB_ICONQUESTION | MB_OKCANCEL);

		if (ret == IDOK)
			application->game_information.use_python = true;
		else
			return;			
	}

	CScriptDlg dlg;
	dlg.pApp = application;
	dlg.DoModal();

	if (dlg.m_script == "")
		return;

	CEditorEvent* python = new CEditorEvent;
	python->Init();
	python->m_type = SCRIPT_TYPE;
	python->m_cText = dlg.m_script;
	python->m_cTextBG = RGB(255,255,255);
	python->m_cTextColour = RGB(0,0,0);
	python->m_cFontName = "Arial";
	python->m_cFontSize = 14;
	python->m_cbFontBold = false;
	python->m_cbFontItalic = false;
	python->m_cbFontStrike = false;
	python->m_cbFontUnderline = false;

	m_pEventList->push_back(python);
	Invalidate();
}

void EventSheetEditor::EditScript(CEditorEvent* pEvent)
{

	CScriptDlg dlg;
	dlg.pApp = application;
	dlg.m_script = pEvent->m_cText;
	dlg.DoModal();

	// If comment has no entry, force a blank space
	if (dlg.m_script == "")
		return;

	CEditorEvent* newEvent = pEvent;
	newEvent->m_type = SCRIPT_TYPE;
	newEvent->m_cText = dlg.m_script;
	newEvent->m_cTextBG = RGB(255,255,255);
	newEvent->m_cTextColour = RGB(0,0,0);
	newEvent->m_cFontName = "Arial";
	newEvent->m_cFontSize = 14;
	newEvent->m_cbFontBold = false;
	newEvent->m_cbFontItalic = false;
	newEvent->m_cbFontStrike = false;
	newEvent->m_cbFontUnderline = false;


}

void EventSheetEditor::RegisterObjectID(int& oid, CArchive& ar)
{
	if(ar.IsLoading())
	{
		object_id_translator.translate_id_load(oid);
	}
	else
	{
		object_id_translator.translate_id_save(oid);
	}
}

void EventSheetEditor::ObjectIDTranslator::translate_id_load(int &oid)
{
	if(!use) 
		return;

	map<int, int>::iterator found = translate_to_table.find(oid);
	if(found == translate_to_table.end())
	{
		// Somethings gone wrong in the process because the id should be in the table.
		assert(0);
		
	}
	else
		oid = found->second; // translated
}
void EventSheetEditor::ObjectIDTranslator::translate_id_save(int &oid)
{
	if(!use)
		return;

	// We simply tell the used_items about the oid
	used_items.insert(oid);

}

CArchive& operator >> (CArchive& ar, ObjectTypeDetail& detail)
{
	CString ExpectedName = "ObjectTypeDetail";
	int     Version      = 1;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return ar;
	//list<NameDllInfo> behaviors;
	//list<CString> effects;
	//list<NameDllInfo> private_variables;

	CString name;
	DWORD dll;

	ar >> detail.info.name;
	ar >> detail.info.dll;
	ar >> detail.id;

	int count;
	int i;

	ar >> count;

	for (i = 0; i < count; i++)
	{
		ar >> name;
		ar >> dll;

		detail.behaviors.push_back(ObjectTypeDetail::NameDllInfo(name, dll));		
	}

	ar >> count;

	for (i = 0; i < count; i++)
	{
		ar >> name;

		detail.effects.push_back(name);		
	}

	ar >> count;

	for (i = 0; i < count; i++)
	{
		ar >> name;
		ar >> dll;

		detail.private_variables.push_back(ObjectTypeDetail::NameDllInfo(name, dll));		
	}

	return ar;
}

CArchive& operator << (CArchive& ar, ObjectTypeDetail& detail)
{
	CString ExpectedName = "ObjectTypeDetail";
	int     Version      = 1;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return ar;
	//list<NameDllInfo> behaviors;
	//list<CString> effects;
	//list<NameDllInfo> private_variables;

	ar << detail.info.name;
	ar << detail.info.dll;
	ar << detail.id;

	ar << (int)detail.behaviors.size();

	list<ObjectTypeDetail::NameDllInfo>::iterator b = detail.behaviors.begin();
	for (; b != detail.behaviors.end(); b++)
	{
		ar << b->name;
		ar << b->dll;	
	}

	ar << (int)detail.effects.size();
	list<CString>::iterator e = detail.effects.begin();
	for (; e != detail.effects.end(); e++)
	{
		ar << *e;
	}

	ar << (int)detail.private_variables.size();
	list<ObjectTypeDetail::NameDllInfo>::iterator pv = detail.private_variables.begin();
	for (; pv != detail.private_variables.end(); pv++)
	{
		ar << pv->name;
		ar << pv->dll;	
	}

	return ar;
}

void EventSheetEditor::ObjectIDTranslator::serialize_objects(CArchive& ar, CApplication* pApp)
{
	CString ExpectedName = "serialize_objects";
	int     Version      = 1;
	SerializeClassHeader(ExpectedName, Version, ar);

	map<int, CObjType*> object_types;

	// Make a map of all our object types
	POSITION pos = pApp->object_types.GetStartPosition();
	long nKey;
	CObjType* oT;

	while (pos != NULL) {
		pApp->object_types.GetNextAssoc(pos, nKey, oT);

		if (!oT->m_bIsGroupType)
			object_types[oT->ObjectIdentifier] = oT;
	}

	// Add families too
	list<Family>::iterator f = pApp->families.begin();
	for(;f != pApp->families.end(); f++)
	{
		if(f->is_in_use)
		{
			oT = pApp->FindObjTypeFromNumber(f->identifier);
			if(oT)
			{
				object_types[oT->ObjectIdentifier] = oT;
			}
		}
	}
	// ...And system
	object_types[-1] = &pApp->m_System;
	

	if(ar.IsLoading())
	{
		int count;
		ar >> count;

		for(int i = 0; i < count; i++)
		{
			ObjectTypeDetail ot_detail;
			ar >> ot_detail;

			// Find what object best matches the detail
			map<int, CObjType*>::iterator ot = object_types.begin();

			for(; ot != object_types.end(); ot++)
			{
				if(ot_detail.Match(ot->second))
					translate_to_table[ot_detail.id] = ot->second->ObjectIdentifier;
			}
		}
	}
	else
	{
		list<ObjectTypeDetail> detail;

		set<int>::iterator s = used_items.begin();

		for(; s != used_items.end(); s++)
		{
			CObjType* pType = object_types[*s];
		
			ObjectTypeDetail ot_detail;
			ot_detail.CopyDetails(pType);

			detail.push_back(ot_detail);
		}


		ar << (int) detail.size();
		list<ObjectTypeDetail>::iterator d = detail.begin();
		for(; d!= detail.end(); d++)
		{
			ar << *d;
		}
	}
}

void EventSheetEditor::ObjectIDTranslator::begin()
{
	use = true;
}
void EventSheetEditor::ObjectIDTranslator::end()
{
	use = false;
	used_items.clear();
	translate_to_table.clear();
}

void EventSheetEditor::OpenEventSheet(EventSheet* pEventSheet)
{
	if (!pEventSheet) 
		return;


	// Check if this event sheet is already open
	HWND ClientWindow;
	ClientWindow = ::GetWindow(pMainWnd->m_hWndMDIClient, GW_CHILD);

	while (ClientWindow != NULL)
	{
		CChildFrame* pWnd = (CChildFrame*)CWnd::FromHandle(ClientWindow);
		if(pWnd->event_sheet == pEventSheet)
		{
			if (pWnd->m_tabs.ItemGetCount() != 1)
				if (pWnd->m_tabs.SelectionGet() != 1)
					pWnd->m_tabs.SelectionSet(1, true, true);

			::BringWindowToTop(ClientWindow);
			return;
		}

		ClientWindow = :: GetWindow(ClientWindow, GW_HWNDNEXT);
	}

	// Otherwise create a new window
	CChildFrame* pChildFrame = new CChildFrame(application, NULL, pEventSheet);

	CCreateContext context;
	context.m_pCurrentFrame = pChildFrame;
	context.m_pNewViewClass = RUNTIME_CLASS(EventSheetEditor);
	context.m_pNewDocTemplate = pDocTemplate;
	context.m_pCurrentDoc = pDocTemplate->CreateNewDocument();
	CRect rc(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);

	CString EventSheetTitle;
	EventSheetTitle.Format("Event Sheet: %s", pEventSheet->GetName());
	BOOL ret = pChildFrame->Create(NULL, 
		EventSheetTitle, WS_CHILD | WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
					 rc, pMainWnd, &context);

	// Maximize
	pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);

	CWnd* view = pChildFrame->splitter.GetWindow(GW_CHILD);
	if(view)
	for(int a = 0; a < 10; a ++)
	{
		if(!dynamic_cast<EventSheetEditor*>(view))
			view = view->GetWindow(GW_HWNDNEXT);
			if(!view)
				a = 10;
	}
	
	((EventSheetEditor*)view)->m_pEventList = &pEventSheet->m_EventList;
	((EventSheetEditor*)view)->m_pHeader = &pEventSheet->m_Header;
	((EventSheetEditor*)view)->OnInitialUpdate();

	// Now remove the tab
	CWnd* pTabs = pChildFrame->splitter.GetWindow(GW_CHILD);
	for(int a = 0; a < 10; a ++)
	{
		if(!dynamic_cast<CExtTabWnd*>(pTabs))
			pTabs = pTabs->GetWindow(GW_HWNDNEXT);
			if(!pTabs)
				break;
	}

	if(pTabs)
		((CExtTabWnd*)pTabs)->ItemRemove(0, 1, true);

	pChildFrame->RecalcLayout();
	pChildFrame->splitter.RecalcLayout();
}