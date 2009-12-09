/////////////////////////////////////////////////////////////////////////////
// Event sheet editor - functions
/////////////////////////////////////////////////////////////////////////////
 
// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "..\..\Construct Doc.h"
#include "Event Sheet Editor.h"

void EventSheetEditor::CreateEventSelectionVectorLimited(SelectedEventVector& pSelectedEventList, EventVector& EventList)
{
	for(int a = 0 ; a < EventList.size() ; a ++)
	{
		if(EventList[a]->m_select(this))
			pSelectedEventList.push_back(EventList[a]);
		else 
			CreateEventSelectionVectorLimited(pSelectedEventList, EventList[a]->m_EventList);
	}
}

void EventSheetEditor::CreateEventSelectionVector(SelectedEventVector& pSelectedEventList,EventVector& EventList)
{
	for(int a = 0 ; a < EventList.size() ; a ++)
	{
		if(EventList[a]->m_select(this))
			pSelectedEventList.push_back(EventList[a]);
	
		CreateEventSelectionVector(pSelectedEventList, EventList[a]->m_EventList);
	}
}

void EventSheetEditor::CreateConditionSelectionVector(SelectedConditionVector& pSelectedConditionList, EventVector& EventList)
{
	for(int a = 0 ; a < EventList.size() ; a ++)
	{
		for(int c = 0; c < EventList[a]->m_Conditions.size(); c ++ )
			if(	EventList[a]->m_Conditions[c]->m_select(this) 
				&& !EventList[a]->m_Conditions[c]->m_Anim.m_bDestroyed)
				pSelectedConditionList.push_back(EventList[a]->m_Conditions[c]);

		CreateConditionSelectionVector(pSelectedConditionList,EventList[a]->m_EventList);
	}
}

void EventSheetEditor::CreateActionSelectionVector(SelectedActionVector& pSelectedActionList, EventVector& EventList)
{
	for(int a = 0 ; a < EventList.size() ; a ++)
	{
		for(int c = 0; c < EventList[a]->m_Actions.size(); c ++ )
			if(	EventList[a]->m_Actions[c]->m_select(this))
				pSelectedActionList.push_back(EventList[a]->m_Actions[c]);

		CreateActionSelectionVector(pSelectedActionList,EventList[a]->m_EventList);
	}
}

void EventSheetEditor::OnToolbarCopy()
{
	if(m_pInlineScintilla.GetSafeHwnd())
		m_pInlineScintilla.Copy();
	else
	{
		switch (m_Drag.dragtype)
		{
			case EVENT:
			{
				SelectedEventVector  m_SelectedEvents;
				CreateEventSelectionVectorLimited(m_SelectedEvents,(*m_pEventList));
				CEditorDragEvents* Events = new CEditorDragEvents;
				Events->pEd = this;
				Events->m_pSelectedEvents = &m_SelectedEvents;

				DROPEFFECT de = DROPEFFECT_NONE;
				m_pDDMgr->PrepareDrop(DO_CLIPBOARD,"Construct Events",Events,&de);				
					
				// wait till the drop is done...then....

				Invalidate();
					
			}
			break;

			case CONDITION:
			{
				SelectedConditionVector m_SelectedConditions;
				CreateConditionSelectionVector(m_SelectedConditions, (*m_pEventList));
				CEditorDragConditions* Conditions = new CEditorDragConditions;
				Conditions->pEd = this;
				Conditions->m_pSelectedConditions = &m_SelectedConditions;

				DROPEFFECT de = DROPEFFECT_NONE;
				m_pDDMgr->PrepareDrop(DO_CLIPBOARD,"Construct Conditions",Conditions,&de);				
			
				// wait till the drop is done..then...
				m_Drag.dragtype = -1;
				m_Drag.IsDragging = false;
				Invalidate();
				m_SelectedConditions.clear();
			}
			break;

			case ACTION:
			{
				SelectedActionVector m_SelectedActions;
				CreateActionSelectionVector(m_SelectedActions,(*m_pEventList));
				CEditorDragActions* Actions = new CEditorDragActions;

				Actions->pEd = this;
				Actions->m_pSelectedActions = &m_SelectedActions;
				
				DROPEFFECT de = DROPEFFECT_NONE;
				m_pDDMgr->PrepareDrop(DO_CLIPBOARD,"Construct Actions",Actions,&de);				
			
				// wait till the drop is done..then...
				m_Drag.dragtype = -1;
				m_Drag.IsDragging = false;
				Invalidate();
				m_SelectedActions.clear();
			
			}
			break;

			default:
			break;
		}
	}
}

void EventSheetEditor::OnToolbarPaste()
{
	if(m_pInlineScintilla.GetSafeHwnd())
	{
		m_pInlineScintilla.Paste();
		return;
	}

	CString csF = m_pDDMgr->AvailableDataType(NULL);

	if (m_pDDMgr->OkToPaste())
	{
		CreateUndo();
		if (csF == "Construct Events") m_Drag.dragtype = EVENT;
		if (csF == "Construct Conditions") m_Drag.dragtype = CONDITION;
		if (csF == "Construct Actions") m_Drag.dragtype = ACTION;
	
		switch (m_Drag.dragtype)
		{
			case EVENT:
			{
				m_Drag.dragtype = -1;

				CEditorEvent* tmpEvent = m_FocusEvent;
				
				
				EventVector* tempList = m_pEventList;
				CEditorDragEvents Events;
				int index = -1;
				if(tmpEvent)
				{
					tempList = FindEventListEventExistsIn(tmpEvent,&(*m_pEventList),index);
				}
				if(tempList == 0 ) 
					return ; // eeppp...the event doesn't have a parent!?!?!?!?
				
				Events.m_pEventListPart		= tempList;
				Events.index				= index;
				Events.GoingUp				= false;
				Events.pEd = (EventSheetEditor*)this;

				DeselectActions();
				DeselectConditions();
				DeselectEvents();
		
				m_pDDMgr->DoDrop(&Events,
							  NULL,
							  "Construct Events");
			}
			break;

			case CONDITION:
			{		
				m_Drag.dragtype = -1;

				// Get selected events and add condition to them
				SelectedEventVector SelectedEvents;
				CreateEventSelectionVector(SelectedEvents, (*m_pEventList));

				if(SelectedEvents.size() == 0)
				{
					CEditorCondition* tmpCondition = m_FocusCondition;
					CEditorEvent* tmpEvent = 0;
					int index = 0;
					tmpEvent = FindEventConditionExistsIn(tmpCondition,m_pEventList,index);
					if(!tmpEvent)
					{
						tmpEvent = m_FocusEvent;
					}

					CEditorDragConditions Conditions;
					Conditions.m_pConditions		= &tmpEvent->m_Conditions;
					Conditions.index				= index;
					Conditions.GoingUp				= false;
					Conditions.pEd = (EventSheetEditor*)this;

					DeselectActions();
					DeselectConditions();
					DeselectEvents();

					if(tmpEvent)
					{
						m_pDDMgr->DoDrop(&Conditions,
									  DROPEFFECT_NONE,
									  "Construct Conditions");
					}
				}
				else
				{
					for(int a = 0; a < SelectedEvents.size(); a ++)
					{
						CEditorDragConditions Conditions;
						Conditions.m_pConditions		= &SelectedEvents[a]->m_Conditions;
						Conditions.index				= 0;
						Conditions.GoingUp				= false;
						Conditions.pEd = (EventSheetEditor*)this;

						m_pDDMgr->DoDrop(&Conditions,
									  DROPEFFECT_NONE,
									  "Construct Conditions");

						InvalidateRect(SelectedEvents[a]->m_rect(this));
					}
				}
			}
			break;	

			case ACTION:
			{		
				m_Drag.dragtype = -1;

				// Get selected events and add condition to them
				SelectedEventVector SelectedEvents;
				CreateEventSelectionVector(SelectedEvents,(*m_pEventList));

				if(SelectedEvents.size() == 0)
				{
					CEditorAction* tmpAction = m_FocusAction;
					CEditorEvent* tmpEvent = 0;
					int index = 0;
					tmpEvent = FindEventActionExistsIn(tmpAction,m_pEventList,index);

					if(!tmpEvent)
					{
						tmpEvent = m_FocusEvent;
					}

					CEditorDragActions Actions;
					Actions.m_pActions		= &tmpEvent->m_Actions;
					Actions.index				= index;
					Actions.GoingUp				= false;
					Actions.pEd = (EventSheetEditor*)this;

					DeselectActions();
					DeselectConditions();
					DeselectEvents();		
					if(tmpEvent)
					{
						m_pDDMgr->DoDrop(&Actions,
									  DROPEFFECT_NONE,
									  "Construct Actions");
					}
				}
				else
				{
					for(int a = 0; a < SelectedEvents.size(); a ++)
					{
						CEditorDragActions Actions;
						Actions.m_pActions			= &SelectedEvents[a]->m_Actions;
						Actions.index				= 0;
						Actions.GoingUp				= false;
						Actions.pEd = (EventSheetEditor*)this;

						m_pDDMgr->DoDrop(&Actions,
									  DROPEFFECT_NONE,
									  "Construct Actions");

						InvalidateRect(SelectedEvents[a]->m_rect(this));
					}
				}
			}
			break;	
		}		
	}

	Invalidate();
}

BOOL EventSheetEditor::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	m_Drag.IsDragging = false;
	m_ClickedSelectedAction = false;
	m_ClickedSelectedCondition = false;
	m_ClickedSelectedEvent = false;

	// DRAG INFO
	m_Drag.dragtype = -1;
	m_Drag.IsDragging = false;
	CString csF = m_pDDMgr->AvailableDataType(pDataObject);
	if (csF == "Construct Events") m_Drag.dragtype = EVENT;
	if (csF == "Construct Conditions") m_Drag.dragtype = CONDITION;
	if (csF == "Construct Actions") m_Drag.dragtype = ACTION;
	if (csF == "Construct Object") m_Drag.dragtype = OBJECT;
	if(m_Drag.dragtype != -1)
		m_Drag.IsDragging = true;
	// DRAG INFO END

	switch (m_Drag.dragtype)
	{
	case EVENT:
		{
			CreateUndo();
			//First if we are moving...delete the old stuff	
			if(dropEffect == DROPEFFECT_MOVE)
			{
				//We need to delete the old ones....
				for(int a = 0; a < (*m_pEventList).size() ; a ++ )
				{
					if((*m_pEventList)[a]->InstantDeleteSelectedEvents((*m_pEventList),a, this))
					{
						(*m_pEventList)[a]->Remove(false, this);
					}
				}
				
			}
			// At this point we have a new event list...but remember we 
			// haven't rendered so even though the events have moved there
			// rectangles are old...meaning we can still 'select' our new
			// rectangle
			////

			m_Drag.dragtype = -1;

			CEditorEvent* tmpEvent = EventAt(m_Mouse);
			if(tmpEvent == 0 ) return false; // eep we lost our selected events! Might want to do something here!
			
			EventVector* tempList;
			CEditorDragEvents Events;
			int index;
			tempList = FindEventListEventExistsIn(tmpEvent,&(*m_pEventList),index);
			if(tempList == 0 ) return false; // eeppp...the event doesn't have a parent...
			
			Events.m_pEventListPart		= tempList;
			Events.index				= index;
			Events.GoingUp				= m_Drag.GoingUp;

			if(point.x > tmpEvent->m_handle(this).left +32 && tmpEvent->canAddEvents())
			{
				Events.m_pEventListPart = &tmpEvent->m_EventList;
				Events.index = 0;
				Events.GoingUp = true;

			}
			
			Events.pEd					= (EventSheetEditor*)this;

			DeselectEvents();
	
			m_pDDMgr->DoDrop(&Events,
                          pDataObject,
                          "Construct Events");
		}
		break;

	case CONDITION:
		{		
			CreateUndo();

			if(dropEffect == DROPEFFECT_MOVE)
			{
				SelectedConditionVector m_SelectedConditions;
				CreateConditionSelectionVector(m_SelectedConditions, (*m_pEventList));
				for(int c = 0; c < m_SelectedConditions.size(); c++)
					m_SelectedConditions[c]->Remove(false, this);

				m_SelectedConditions.clear();
			}

			m_Drag.dragtype = -1;

			CEditorEvent* tmpEvent = EventAt(m_Mouse);

			if(tmpEvent == 0 ) 
				return false; 
			if(!tmpEvent->canAddConditions())
				return false;

			CEditorCondition* tmpCondition = tmpEvent->conditionAt(m_Mouse, this);
			
			int index = 0;
			if(tmpCondition != 0)
			{
				for(int c = 0; c < tmpEvent->m_Conditions.size() ; c ++ )
				{
					if (tmpEvent->m_Conditions[c] == tmpCondition)
						index = c;
				}
			}

			DeselectConditions();

			CEditorDragConditions Conditions;
			Conditions.m_pConditions		= &tmpEvent->m_Conditions;
			Conditions.index				= index;
			Conditions.GoingUp				= m_Drag.GoingUp;
			Conditions.pEd					= (EventSheetEditor*)this;
			m_pDDMgr->DoDrop(&Conditions,
                          pDataObject,
                          "Construct Conditions");
		}
		break;	
		
	case ACTION:
		{	
			CreateUndo();

			//First if we are moving...delete the old stuff	
			if(dropEffect == DROPEFFECT_MOVE)
			{
				SelectedActionVector m_SelectedActions;
				CreateActionSelectionVector(m_SelectedActions,(*m_pEventList));
				for(int a=0; a < m_SelectedActions.size(); a ++)
					m_SelectedActions[a]->Remove(false, this);

				m_SelectedActions.clear();
			}


			m_Drag.dragtype = -1;

			CEditorEvent* tmpEvent = EventAt(m_Mouse);

			if(tmpEvent == 0 ) 
				return false; // eep we lost our event that held the condition our mouse is over!!!
			if(!tmpEvent->canAddActions())
				return false; // type doesn't accept droppin of actions
			CEditorAction* tmpAction = tmpEvent->actionAt(m_Mouse, this);
			
			int index = 0;
			if(tmpAction != 0)
			{			
				for(int a = 0; a < tmpEvent->m_Actions.size() ; a ++ )
				{
					if (tmpEvent->m_Actions[a] == tmpAction)
						index = a;
				}
			}

			DeselectActions();

			CEditorDragActions Actions;
			Actions.m_pActions		= &tmpEvent->m_Actions;
			Actions.index				= index;
			Actions.GoingUp				= m_Drag.GoingUp;
			Actions.pEd					= (EventSheetEditor*)this;
			m_pDDMgr->DoDrop(&Actions, pDataObject, "Construct Actions");
		}
		break;
	case OBJECT:
		{
			CEditorDragObject Object;
			Object.pEd = (EventSheetEditor*)this;
			m_pDDMgr->DoDrop(&Object, pDataObject, "Construct Object");
			int obj = Object.ObjectIdentifier;

			int newindex = -1;
			int oldindex = -1;

			for(int a = 0; a < this->m_pHeader->m_Objects.size(); a++)
			{
				if(m_pHeader->m_Objects.at(a).oid == Object.ObjectIdentifier)
					oldindex = a;
				
				if(m_pHeader->m_Objects.at(a).m_rect.PtInRect(point))
				{
					newindex = a;				
				}
			}

			if(oldindex == -1)
				return false;
			if(newindex == -1)
				return false;

			CEditorObject o = m_pHeader->m_Objects.at(oldindex);
			m_pHeader->m_Objects.erase( m_pHeader->m_Objects.begin() + oldindex);

			if(newindex >= m_pHeader->m_Objects.size())
				m_pHeader->m_Objects.push_back(o);
			else
				m_pHeader->m_Objects.insert(m_pHeader->m_Objects.begin() + newindex, o);

			Invalidate();
		}

		break;	
	}

	OnTimer(0); // update the time

    return true;
}

void EventSheetEditor::DeselectEvents()
{
	SelectedEventVector  SelectedEvents;
	CreateEventSelectionVector(SelectedEvents,(*m_pEventList));

	for(int a=0; a < SelectedEvents.size(); a ++)
	{
		SelectedEvents[a]->m_select(this) = false;
		InvalidateRect(SelectedEvents[a]->m_rect(this));
	}
}

DROPEFFECT EventSheetEditor::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	/// Update Key States
	m_CtrlDown = false;
	m_ShiftDown = false;

	if(dwKeyState & MK_CONTROL)
		m_CtrlDown = true;

	if(dwKeyState & MK_SHIFT)
		m_ShiftDown = true;

	m_Drag.CtrlDown = m_CtrlDown;

	DROPEFFECT de = m_pDDMgr->OnDragOver(pDataObject, dwKeyState, point);

	// DRAG INFO
	m_Drag.dragtype = -1;
	m_Drag.IsDragging = false;
	CString csF = m_pDDMgr->AvailableDataType(pDataObject);
	if (csF == "Construct Events") m_Drag.dragtype = EVENT;
	if (csF == "Construct Conditions") m_Drag.dragtype = CONDITION;
	if (csF == "Construct Actions") m_Drag.dragtype = ACTION;
	if (csF == "Construct Object") m_Drag.dragtype = OBJECT;
	if(m_Drag.dragtype != -1)
		m_Drag.IsDragging = true;
	// DRAG INFO END

	if(m_Mouse.y != point.y)
		m_Drag.GoingUp = point.y < m_Mouse.y ? true : false;

	m_Mouse = point;
	Invalidate();
	
	switch (m_Drag.dragtype)
	{
	case EVENT:
		{
			// Mouse is over an event
			CEditorEvent* tmpEvent = EventAt(m_Mouse);

			if(tmpEvent == 0 ) 
				return false;

			// If we arn't holding control, dont let them hover over something selected!
			if(!m_CtrlDown)
				if(tmpEvent->m_select(this))
					return false;

			//CRect Rect = tmpEvent->m_handle(this);
			//Rect.left = tmpEvent->m_rect(this).left;
			//Rect.bottom = tmpEvent->m_rect(this).bottom;
			//if(!Rect.PtInRect(point))
			//	return false;
		}
		break;

	case CONDITION:
		{
			CEditorEvent* tmpEvent = EventAt(m_Mouse);
			if(tmpEvent == 0 )
				return false;
			if(tmpEvent->m_type == 1 )
				return false; // Type doesn't accept draggin
			
			CEditorCondition* tmpCondition = tmpEvent->conditionAt(m_Mouse, this);
			
			// If we arn't holding control, dont let them hover over something selected!
			if(!m_CtrlDown)
				if(tmpCondition)
					if(tmpCondition->m_select(this))
						return false;

	/*		CRect Rect = tmpEvent->m_handle;
			Rect.bottom = tmpEvent->m_rect.bottom;
			if(tmpCondition)
			{
				Rect = tmpCondition->m_rect;
				if(!Rect.PtInRect(point))
					return false;
			}*/

		}
		break;

	case ACTION:
		{
			CEditorEvent* tmpEvent = EventAt(m_Mouse);
			if(tmpEvent == 0 )
				return false;
			if(tmpEvent->m_type == 1 )
				return false; // Type doesn't accept draggin
			
			CEditorAction* tmpAction = tmpEvent->actionAt(m_Mouse, this);
			
			// If we arn't holding control, dont let them hover over something selected!
			if(!m_CtrlDown)
				if(tmpAction)
					if(tmpAction->m_select(this))
						return false;

			CRect Rect = tmpEvent->m_handle(this);
			Rect.bottom = tmpEvent->m_rect(this).bottom;
		}
		break;
	case OBJECT:
		{
			if(!m_pHeader->m_rect.PtInRect(m_Mouse))
			{
				return false;
			}
		}
		break;
	}

	// Auto scroll

	POINT pt;
	GetCursorPos( &pt );
	RECT rect;
	GetClientRect( &rect );
	ScreenToClient( &pt );


	if ((GetTickCount() - 25) > m_lastTick) {
			POINT pt;
			GetCursorPos( &pt );
			RECT rect;
			GetClientRect( &rect );
			ClientToScreen( &rect );

			SetRedraw(0);
			if( pt.y < rect.top + 10)
				ScrollToPosition(GetScrollPosition()+CPoint(0,-10));
			else if( pt.y > rect.bottom - 10)
				ScrollToPosition(GetScrollPosition()+CPoint(0,10));
			if( pt.x < rect.left + 10)
				ScrollToPosition(GetScrollPosition()+CPoint(-10,0));
			else if( pt.x > rect.right - 10)
				ScrollToPosition(GetScrollPosition()+CPoint(10,0));
			SetRedraw();

		m_lastTick = GetTickCount();
	}
	if (m_lastTick > GetTickCount())
		m_lastTick = GetTickCount();	

	// end auto scroll

    return de;
}

void EventSheetEditor::DeselectActions()
{
	SelectedActionVector SelectedActions;
	CreateActionSelectionVector(SelectedActions,(*m_pEventList));

	for(int a = 0; a < SelectedActions.size(); a++)
	{
		SelectedActions[a]->m_select(this) = false;
		InvalidateRect(SelectedActions[a]->m_rect(this));
	}
}

void EventSheetEditor::DeselectConditions()
{
	SelectedConditionVector SelectedConditions;
	CreateConditionSelectionVector(SelectedConditions,(*m_pEventList));

	for(int a=0; a < 	SelectedConditions.size(); a ++)
	{
		SelectedConditions[a]->m_select(this) = false;
		InvalidateRect(SelectedConditions[a]->m_rect(this));
	}
}

void EventSheetEditor::OnToolbarDelete()
{
	if(m_pInlineScintilla.GetSafeHwnd())
	{
		m_pInlineScintilla.Clear();
		return;
	}

	CreateUndo();

	// Delete Events
	for(int e = 0; e < (*m_pEventList).size() ; e ++ )
	{
		int subtractspace = 0;
		if((*m_pEventList)[e]->DeleteSelectedEvents((*m_pEventList),e,*this, &subtractspace))
		{
			(*m_pEventList)[e]->Remove(true, this);
			(*m_pEventList)[e]->m_Anim.StartSpace -= subtractspace;

		}
	}

	// Delete Conditions
	SelectedConditionVector m_SelectedConditions;
	CreateConditionSelectionVector(m_SelectedConditions,(*m_pEventList));
	for(int c=0; c < m_SelectedConditions.size(); c ++)
		m_SelectedConditions[c]->Remove(true, this);

	m_SelectedConditions.clear();

	// Delete Actions
	SelectedActionVector m_SelectedActions;
	CreateActionSelectionVector(m_SelectedActions,(*m_pEventList));
	for(int a = 0; a < m_SelectedActions.size(); a++)
		m_SelectedActions[a]->Remove(true, this);

	m_FocusEvent = NULL;
	m_FocusAction = NULL;
	m_FocusCondition = NULL;

	// Refresh the screen
	Invalidate();
}

void EventSheetEditor::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	if(m_pInlineScintilla.GetSafeHwnd())
	{
		pCmdUI->Enable(TRUE);
		return;
	}

	SelectedConditionVector m_SelectedConditions;
	CreateConditionSelectionVector(m_SelectedConditions,(*m_pEventList));

	SelectedEventVector m_SelectedEvents;
	CreateEventSelectionVector(m_SelectedEvents,(*m_pEventList));

	SelectedActionVector m_SelectedActions;
	CreateActionSelectionVector(m_SelectedActions,(*m_pEventList));

	if ((m_SelectedEvents.size() == 0) && (m_SelectedConditions.size() == 0) && (m_SelectedActions.size() == 0))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void EventSheetEditor::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	if(m_pInlineScintilla.GetSafeHwnd())
	{
		pCmdUI->Enable(TRUE);
		return;
	}

	SelectedConditionVector m_SelectedConditions;
	CreateConditionSelectionVector(m_SelectedConditions, (*m_pEventList));

	SelectedEventVector m_SelectedEvents;
	CreateEventSelectionVector(m_SelectedEvents, (*m_pEventList));

	SelectedActionVector m_SelectedActions;
	CreateActionSelectionVector(m_SelectedActions, (*m_pEventList));

	if ((m_SelectedEvents.size() == 0) && (m_SelectedConditions.size() == 0) && (m_SelectedActions.size() == 0))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void EventSheetEditor::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	if(m_pInlineScintilla.GetSafeHwnd())
	{
		if(m_pInlineScintilla.CanPaste())
			pCmdUI->Enable(TRUE);
		else
			pCmdUI->Enable(FALSE);

		return;
	}

	pCmdUI->Enable(m_pDDMgr->OkToPaste());
}

void EventSheetEditor::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	if(m_pInlineScintilla.GetSafeHwnd())
	{
		pCmdUI->Enable(TRUE);
		return;
	}

	if(m_pInlineScintilla.GetSafeHwnd())
	{
		pCmdUI->Enable(TRUE);
		return;
	}

	SelectedConditionVector m_SelectedConditions;
	CreateConditionSelectionVector(m_SelectedConditions,(*m_pEventList));

	SelectedEventVector m_SelectedEvents;
	CreateEventSelectionVector(m_SelectedEvents,(*m_pEventList));

	SelectedActionVector m_SelectedActions;
	CreateActionSelectionVector(m_SelectedActions,(*m_pEventList));

	if ((m_SelectedEvents.size() == 0) && (m_SelectedConditions.size() == 0) && (m_SelectedActions.size() == 0))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void EventSheetEditor::OnToolbarCut()
{
	OnToolbarCopy();
	OnToolbarDelete();
}

void EventSheetEditor::ExpandAllGroups(bool open, CEditorEvent* pParent)
{
	EventVector* pEvents = this->m_pEventList;
	if(pParent)
	{
		if(pParent->m_type == GROUP_TYPE)
		{
			pParent->m_open(this) = open;
		}
		pEvents = &pParent->m_EventList;
	}
	EventVector::iterator e = pEvents->begin();
	for(;e!= pEvents->end(); e++)
	{
		ExpandAllGroups(open, *e);
	}
}