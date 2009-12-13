/////////////////////////////////////////////////////////////////////////////
// Event sheet editor - lookup
/////////////////////////////////////////////////////////////////////////////
 
// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "..\..\Construct Doc.h"
#include "Event Sheet Editor.h"

// Get ready folks, this is one big ol' function
void EventSheetEditor::HandleConditionColumnRightClick(CPoint point)
{
	CEditorEvent* pParentEvent;
	CEditorEvent* pEvent = EventAt(point, &pParentEvent);

	if(pEvent)
	{
		CEditorCondition* pCondition = EventAt(point)->conditionAt(point, this);

		// Cursor position
		POINT CursorPosition;
		GetCursorPos(&CursorPosition);

		// Load popup context menu
		CExtPopupMenuWnd* popup = new CExtPopupMenuWnd;
		popup->CreatePopupMenu(m_hWnd);
		
		if (pEvent)
		{
			///////////////////////////////////////
			// CUT
			// COPY
			// PASTE
			// ----------
			///////////////////////////////////////
			popup->ItemInsertCommand(6, -1, EE_DELETEEVENT, NULL, NULL);
			popup->ItemInsertCommand(1, -1, RIBBON_CUT, NULL, NULL);
			popup->ItemInsertCommand(2, -1, RIBBON_COPY, NULL, NULL);
			popup->ItemInsertCommand(3, -1, RIBBON_PASTE, NULL, NULL);
			popup->ItemInsert(); 

			///////////////////////////////////////
			// BOOKMARK/UNBOOKMARK
			// COPY AS TEXT
			// ----------
			///////////////////////////////////////
			if (!pCondition)
			{
				if (pEvent->m_type == 0)
				{
					if (pEvent->m_bBookmarked)
						popup->ItemInsertCommand(4, -1, EE_UNBOOKMARKEVENT, NULL, NULL);
					else
						popup->ItemInsertCommand(4, -1, EE_BOOKMARKEVENT, NULL, NULL);

					popup->ItemInsertCommand(5, -1, EE_COPYASTEXT, NULL, NULL);
	
					popup->ItemInsertCommand(17, -1, RIBBON_TOGGLEEVENT, NULL, NULL);

					popup->ItemInsert();
				}
			}

			if(pCondition)
			{
				DeselectEvents();
				DeselectActions();
				if(!pCondition->m_select(this))
					DeselectConditions();
				pCondition->m_select(this) = true;
				m_FocusCondition = pCondition;	
				m_FocusEvent = pEvent;
				Invalidate();
			}
			else
				DeselectConditions();

			///////////////////////////////////////
			// DELETE CONDITION
			// EDIT CONDITION
			// INVERT CONDITION
			// REPLACE CONDITION
			// ----------
			///////////////////////////////////////
			if(pCondition)
			{
				popup->ItemInsertCommand(7, -1, EE_DELETECONDITION, NULL, NULL);
				if(pCondition->params.size())
					popup->ItemInsertCommand(8, -1, EE_EDITCONDITION, NULL, NULL);

				popup->ItemInsertCommand(9, -1, EE_INVERTCONDITION, NULL, NULL);

				popup->ItemInsertCommand(10, -1, EE_REPLACECONDITION, NULL, NULL);

				popup->ItemInsertCommand(28, -1, EE_TOGGLECONDITION, NULL, NULL);

				popup->ItemInsert();
			}

			///////////////////////////////////////
			// INSERT NEW CONDITION
			///////////////////////////////////////
			if(pEvent->canAddConditions())
				popup->ItemInsertCommand(11, -1, EE_INSERTNEWCONDITION, NULL, NULL);

			if(pEvent->canAddEvents())
			{
				// Group
				///////////////////////////////////////
				// INSERT NEW COMMENT
				// INSERT NEW EVENT
				// EDIT GROUP
				// ----------
				///////////////////////////////////////
				if (pEvent->m_type == 2)
				{
					popup->ItemInsertCommand(12, -1, EE_INSERTNEWEVENT, NULL, NULL);
					popup->ItemInsertCommand(13, -1, EE_EDITGROUP, NULL, NULL);

					popup->ItemInsert();

					popup->ItemInsertCommand(26, -1, EE_EXPANDALL, NULL, NULL);
					popup->ItemInsertCommand(27, -1, EE_COLLAPSEALL, NULL, NULL);
				}

				// Event
				///////////////////////////////////////
				// INSERT SUBEVENT
				// ----------
				///////////////////////////////////////
				else
				{
					popup->ItemInsertCommand(24, -1, EE_INSERTCOMMENT, NULL, NULL);
					popup->ItemInsertCommand(25, -1, EE_INSERTGROUP, NULL, NULL);
					if(pEvent->canAddEvents()){
						popup->ItemInsertCommand(14, -1, EE_INSERTSUBEVENT, NULL, NULL);
					}
					popup->ItemInsert();
				}
			}
		}

		// Switch views
		if (pEvent->m_type != 2)
		{
			popup->ItemInsertCommand(15, -1, EE_CHRONOLOGICALVIEW, NULL, NULL);
			popup->ItemInsertCommand(16, -1, EE_LISTVIEW, NULL, NULL);
		}

		ACEMenuMap map;
		CObjType* objectType = NULL;

		if(pCondition)
			objectType = application->FindObjTypeFromNumber(pCondition->oid);

		// Display it
		UINT ChosenItem = 0;

		// Add icons
		CExtCmdIcon icon;
	
		// Invert condition
		icon.AssignFromHBITMAP(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_NOTEQUAL)), RGB(255, 255, 255));
		popup->ItemSetPopupIcon(popup->ItemFindPosForCmdID(9), icon);

		// Insert new condition
		icon.AssignFromHBITMAP(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_RIBBON_ADDCONDITION)), RGB(255, 255, 255));
		popup->ItemSetPopupIcon(popup->ItemFindPosForCmdID(11), icon);

		// Insert new event
		icon.AssignFromHBITMAP(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_RIBBON_ADDEVENT)), RGB(255, 255, 255));
		popup->ItemSetPopupIcon(popup->ItemFindPosForCmdID(12), icon);

		// Insert sub-event
		icon.AssignFromHBITMAP(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_RIBBON_ADDSUBEVENT)), RGB(255, 255, 255));
		popup->ItemSetPopupIcon(popup->ItemFindPosForCmdID(14), icon);

		// Insert comment
		icon.AssignFromHBITMAP(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_COMMENT_NEW_LARGE)), RGB(255, 255, 255));
		SIZE s;
		s.cx = 16;
		s.cy = 16;
		icon.Scale(s);
		popup->ItemSetPopupIcon(popup->ItemFindPosForCmdID(24), icon);

		// Insert group
		icon.AssignFromHBITMAP(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_RIBBON_ADDGROUP)), RGB(255, 255, 255));
		icon.Scale(s);
		popup->ItemSetPopupIcon(popup->ItemFindPosForCmdID(25), icon);

		// i dont know why, but no wm and no cmd actually ment - post cmd and ui while the popup is open :S
		popup->TrackPopupMenu(TPMX_DO_MESSAGE_LOOP | TPMX_NO_WM_COMMAND, CursorPosition.x, CursorPosition.y, NULL, NULL, NULL, &ChosenItem);

		EventVector* pEventList = m_pEventList;

		//if(pParentEvent)
		//	*pEventList = pParentEvent->m_EventList;

		int index = NULL;
		int i = 0;

		pEventList = FindEventListEventExistsIn(pEvent, m_pEventList, index);

		// Cut
		if (ChosenItem == 1)
			OnToolbarCut();

		// Copy
		if (ChosenItem == 2)
			OnToolbarCopy();

		// Paste
		if (ChosenItem == 3)
			OnToolbarPaste();

		// Bookmark/unbookmark event
		if (ChosenItem == 4)
		{
			CreateUndo();
			if(pEvent)
				pEvent->m_bBookmarked = !pEvent->m_bBookmarked;

			Invalidate();
		}

		// Copy as text
		if (ChosenItem == 5)
		{
			if(pEvent)
			{
				// Make the string
				CString Event;
				for (int i = 0; i < pEvent->m_Conditions.size(); i++)
				{
					Event += "+ ";

					CString nameStr;

					int ID = pEvent->m_Conditions[i]->oid;
					if (ID == -1)
						nameStr = "System";
					else
						nameStr = application->FindObjTypeFromNumber(ID)->GetName();

					nameStr += ": ";
					Event += nameStr;

					CString conditionText = StripHTML(pEvent->m_Conditions[i]->m_Text);
					conditionText.Trim();

					if (conditionText.Left(nameStr.GetLength()) == nameStr)
						conditionText.Delete(0, nameStr.GetLength());

					conditionText.Trim();

					Event += conditionText;
					Event += "\r\n";
				}

				for (int i = 0; i < pEvent->m_Actions.size(); i++)
				{
					Event += "-> ";

					int ID = pEvent->m_Actions[i]->oid;
					if (ID == -1)
						Event += "System";
					else
						Event += application->FindObjTypeFromNumber(ID)->GetName();

					Event += ": ";
					Event += StripHTML(pEvent->m_Actions[i]->m_Text);
					Event += "\r\n";
				}

				Event = Event.Left(Event.GetLength() - 1);

				OpenClipboard();

				EmptyClipboard();
				HGLOBAL hClipboardData;
				hClipboardData = GlobalAlloc(GMEM_DDESHARE, Event.GetLength() +1 );

				char* pchData;
				pchData = (char*)GlobalLock(hClipboardData);		  
				strcpy(pchData, LPCSTR(Event));

				GlobalUnlock(hClipboardData);

				SetClipboardData(CF_TEXT, hClipboardData);		  
				CloseClipboard();
			}

			Invalidate();
		}

		// Delete event
		if (ChosenItem == 6)
		{
			CreateUndo();

			SelectedEventVector SelectedEvents;
			CreateEventSelectionVector(SelectedEvents, (*m_pEventList));

			for(int a = 0; a < SelectedEvents.size(); a ++)
				pEvent->Remove(true, this);

			Invalidate();
		}

		// Toggle event
		if (ChosenItem == 17)
		{
			CreateUndo();
			SelectedEventVector SelectedEvents;
			CreateEventSelectionVector(SelectedEvents, (*m_pEventList));

			for(int a = 0; a < SelectedEvents.size(); a ++)
			{
				SelectedEvents[a]->m_bEnabled = !SelectedEvents[a]->m_bEnabled;
				InvalidateRect(SelectedEvents[a]->m_rect(this));
			}
		}

		// Delete condition
		if (ChosenItem == 7)
		{
			CreateUndo();

			SelectedConditionVector SelectedConditions;
			CreateConditionSelectionVector(SelectedConditions, (*m_pEventList));

			for(int a = 0; a < SelectedConditions.size(); a ++)
				SelectedConditions[a]->Remove(true, this);
		}

		// Edit a condition
		else if (ChosenItem == 8)
		{
			if(pEvent)
				EditCondition(pCondition);
		}

		// Invert current condition
		if (ChosenItem == 9)
		{
			CreateUndo();

			pCondition->m_Negate = !pCondition->m_Negate;
			ReformatCondition(pCondition);

			Invalidate();

			return;
		}

		// Replace a condition
		else if (ChosenItem == 10)
		{
			int Count = pEvent->m_Conditions.size();

			if(pEvent)
				AddCondition(false, NULL, false, pEvent, true);

			if (Count != pEvent->m_Conditions.size())
			{
				if(pCondition)
					pCondition->Remove(false, this); 
			}
		}

		// Insert a condition into an existing row
		if (ChosenItem == 11)
		{
			int index = -1;
			if(m_FocusCondition)
			for(int i = 0; i < m_FocusEvent->m_Conditions.size(); i++)
				if(m_FocusCondition == m_FocusEvent->m_Conditions.at(i))
					index = i;

			if(pEvent)
			{
				DeselectConditions();
				AddCondition(false, NULL, false, pEvent, false, index);
			}
		}
		// Toggle Condition
		if (ChosenItem == 28)
		{
			CreateUndo();
			if(m_FocusCondition)
			{
				m_FocusCondition->m_bEnabled = !m_FocusCondition->m_bEnabled;
				InvalidateRect(m_FocusCondition->m_rect(this));
			}
		}
	
		// Add an event
		else if (ChosenItem == 12)
		{
			if(pEvent)
				AddCondition(false, NULL, true, pEvent);
		}

		// Edit group
		if (ChosenItem == 13)
		{
			CEditEventGroup Dlg;
			Dlg.m_Description = pEvent->m_sDescription;
			Dlg.m_Title = pEvent->m_sTitle;
			Dlg.m_Active = pEvent->m_sActive;

			if (Dlg.DoModal() == IDOK)
			{
				CreateUndo();
				// Update group
				pEvent->m_sTitle = Dlg.m_Title;
				pEvent->m_sActive = Dlg.m_Active;
				pEvent->m_sDescription = Dlg.m_Description;
			}

			Invalidate();
		}

		// Add a subevent
		else if (ChosenItem == 14)
		{
			if(pEvent)
				AddCondition(false, NULL, true, pEvent);
		}

		if (ChosenItem == 15)
		{
			CreateUndo();
			if(pEvent)
				pEvent->m_displayActionType(this) = 1;
			Invalidate();
		}

		if (ChosenItem == 16)
		{
			CreateUndo();
			if(pEvent)
				pEvent->m_displayActionType(this) = 3;
			Invalidate();
		}

		if(ChosenItem == 20)
		{
			int size = m_pEventList->size();
			AddCondition();
			if(size != m_pEventList->size())
			{
				CreateUndo();
				CEditorEvent* pNewEvent = m_pEventList->back();
				m_pEventList->pop_back();

				pEventList->insert(pEventList->begin() + index , pNewEvent);
				Invalidate();
			}
		}

		if(ChosenItem == 21)
		{
			int size = pEventList->size();

			OnExpandComment();

			if(size != pEventList->size())
			{
				CEditorEvent* pNewEvent = pEventList->back();
				pEventList->pop_back();

				pEventList->insert( pEventList->begin() + index , pNewEvent);
				Invalidate();
			}
		}

		if(ChosenItem == 22)
		{
			int size = pEventList->size();

			AddGroup();

			if(size != pEventList->size())
			{
				CEditorEvent* pNewEvent = pEventList->back();
				pEventList->pop_back();

				pEventList->insert( pEventList->begin() + index , pNewEvent);
				Invalidate();
			}
		}

		if(ChosenItem == 23)
		{
			int size = pEventList->size();

			AddEventSheet();

			if(size != pEventList->size())
			{
				CEditorEvent* pNewEvent = pEventList->back();
				pEventList->pop_back();

				pEventList->insert( pEventList->begin() + index , pNewEvent);

				Invalidate();
			}
		}

		// Insert new comment
		if (ChosenItem == 24)
		{
			OnExpandComment();

			Invalidate();
		}

		// Insert new group
		if (ChosenItem == 25)
		{
			if(m_FocusEvent)
			{
				AddGroup(false, m_FocusEvent);
			}
			else
				AddGroup();

			Invalidate();
		}
		if (ChosenItem == 26)
		{
			ExpandAllGroups(true);
			Invalidate();
		}
		if (ChosenItem == 27)
		{
			ExpandAllGroups(false);
			Invalidate();
		}

		if(ChosenItem > 30 && objectType != NULL)
		{
			CreateUndo();
			MenuMapItem item;
			map.Lookup(ChosenItem, item);
			int i;

			ACESEntry2* entry = objectType->GetACESEntry(CONDITION, item, i);
		
			pCondition->cndID = i;

			while(entry->params.size() < pCondition->params.size())
			{
				delete pCondition->params.back();
				pCondition->params.pop_back();
			}

			while(entry->params.size() > pCondition->params.size())
			{
				CEditorParam* param = new CEditorParam;
				param->oid = pCondition->oid;
				pCondition->params.push_back(param);
			}

			for(int a = 0; a < entry->params.size(); a++)
			{
				CEditorParam* EdParam = pCondition->params.at(a);
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
				pCondition->mid = -1;
				break;
			case 1: // movement
				pCondition->mid = item.child;
				break;
			case 2:
				pCondition->mid = item.child * -1;
				break;
			}
			ReformatCondition(pCondition, true);
			Invalidate();
				

		}
	}
}