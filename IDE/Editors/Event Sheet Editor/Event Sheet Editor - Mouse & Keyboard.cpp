/////////////////////////////////////////////////////////////////////////////
// Event sheet editor - mouse
/////////////////////////////////////////////////////////////////////////////
 
// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "..\..\Construct Doc.h"
#include "Event Sheet Editor.h"

void EventSheetEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
		case VK_DELETE:
			OnToolbarDelete();
		break;

		// Bookmarking hotkeys
		case 0x42:
		{
			if (m_CtrlDown)
			{
				// Toggle bookmark status for all events
				SelectedEventVector SelectedEvents;
				CreateEventSelectionVector(SelectedEvents, (*m_pEventList));

				for(int a = 0; a < SelectedEvents.size(); a ++)
				{
					SelectedEvents[a]->m_bBookmarked = !SelectedEvents[a]->m_bBookmarked;
					InvalidateRect(SelectedEvents[a]->m_rect(this));
				}
			}
		}
		break;

		case VK_CONTROL:
			m_CtrlDown = true;
		break;

		case VK_SHIFT:
			m_ShiftDown = true;
		break;

		case VK_NEXT:
			OnNextBookmark();
		break;

		case VK_PRIOR:
			OnPreviousBookmark();
		break;

		case 'E':
			if (!m_CtrlDown)
				OnAddEvent();
		break;

		case 'S':
			if (!m_CtrlDown)
				OnAddSubEvent();
		break;

		case 'A':
			if (!m_CtrlDown)
				OnAddAction();
		break;

		case 'C':
			if (!m_CtrlDown)
				OnAddCondition();
		break;
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void EventSheetEditor::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
		case VK_CONTROL:
			m_CtrlDown = false;
		break;

		case VK_SHIFT:
			m_ShiftDown = false;
		break;
	}

	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void EventSheetEditor::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_pHeader)
		return;
	m_ObjectHeaderTooltip.ShowAt(point, *this);

	m_Mouse = point;
	m_pHeader->OnMouseMove(point);
	if(m_pHeader->m_isDrag)
		Invalidate();

	else
	{
		// Is is possible we might be dragging.
		if(!m_Drag.IsDragging && m_leftDown)
		{
			if(	( (point.x - m_Drag.StoredPos.x)*(point.x - m_Drag.StoredPos.x)
				+(point.y - m_Drag.StoredPos.y)*(point.y - m_Drag.StoredPos.y)) >  8)
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

						DROPEFFECT de = DROPEFFECT_COPY;
						m_pDDMgr->PrepareDrop(DO_DRAGDROP,"Construct Events",Events,&de);				
					
						// wait till the drop is done...then....
						m_Drag.dragtype = -1;
						m_Drag.IsDragging = false;
						Invalidate();
					}
					break;

					case CONDITION:
					{
						SelectedConditionVector m_SelectedConditions;
						CreateConditionSelectionVector(m_SelectedConditions,(*m_pEventList));
						CEditorDragConditions* Conditions = new CEditorDragConditions;

						Conditions->pEd = this;
						Conditions->m_pSelectedConditions = &m_SelectedConditions;

						DROPEFFECT de = DROPEFFECT_COPY;
						m_pDDMgr->PrepareDrop(DO_DRAGDROP,"Construct Conditions",Conditions,&de);				
					
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

						DROPEFFECT de = DROPEFFECT_COPY;
						m_pDDMgr->PrepareDrop(DO_DRAGDROP,"Construct Actions",Actions,&de);				
					
						// wait till the drop is done..then...
						m_Drag.dragtype = -1;
						m_Drag.IsDragging = false;
						Invalidate();
						m_SelectedActions.clear();
					}
					break;	

					case OBJECT:
					{
						CEditorDragObject* Objects = new CEditorDragObject;
						Objects->pEd = (EventSheetEditor*)this;
						Objects->ObjectIdentifier = m_pHeader->GetObjectAtX(m_Drag.StoredPos.x)->oid;

						DROPEFFECT de = DROPEFFECT_COPY;
						m_pDDMgr->PrepareDrop(DO_DRAGDROP,"Construct Object",Objects,&de);				
					
						// wait till the drop is done..then...
						m_Drag.dragtype = -1;
						m_Drag.IsDragging = false;
						Invalidate();
					}
					break;	

					default:
					break;
				}
			}
		}
		else
		{
			//Tooltips
			CEditorEvent* pMyEvent = EventAt(point);

			if(pMyEvent)
			{
				SelectedActionVector actions;
				CEditorAction* pAction = pMyEvent->actionAt(point, actions, this);
				CEditorCondition* pCondition = pMyEvent->conditionAt(point, this);
				if(pMyEvent->m_displayActionType(this) < 2)	//only grid and chrono should show the tooltip
				{	
					if(pAction)
					{
						
						CPoint pt = (actions[0])->m_rect(this).TopLeft();
						pt.x += 16;
						pt.y = (actions[0])->m_rect(this).bottom-4;
						if(!m_InsertBefore.IsWindowVisible())
							m_ActionTooltip.ShowAt(pt,actions,*this);		
					}
					else
					{
						m_ActionTooltip.Hide();
					}

				}
				else
				{
						


				}
			}
			else
				m_ActionTooltip.Hide();
			
		}
	}

	SelectedConditionVector SelectedConditions;
	CreateConditionSelectionVector(SelectedConditions,(*m_pEventList));

	for(int c = 0; c < SelectedConditions.size(); c++)
		InvalidateRect(SelectedConditions[c]->m_rect(this));

	SelectedActionVector SelectedActions;
	CreateActionSelectionVector(SelectedActions,(*m_pEventList));

	for(int a = 0; a < SelectedActions.size(); a++)
		InvalidateRect(SelectedActions[a]->m_rect(this));

	SelectedEventVector SelectedEvents;
	CreateEventSelectionVector(SelectedEvents,(*m_pEventList));

	for(int e = 0; e < SelectedEvents.size(); e++)
		InvalidateRect(SelectedEvents[e]->m_rect(this));

	////////////////////////
	// Change to a hand cursor if overlapping "new action"
	bool useHandCursor = false;
	static CEditorEvent* pLastEvent = NULL;

	if (pLastEvent)
	{
		pLastEvent->mouseOverNewAction = false;
		pLastEvent->mouseOverFooter = false;

	}

	if (!(point.x >= m_pHeader->m_Split - 2 && point.x <= m_pHeader->m_Split + 5))
	{
		CEditorEvent* pEvent = this->EventAt(point);
		if(pEvent)
		{
			pEvent->mouseOverNewAction = false;
			pEvent->mouseOverFooter = false;

			if(pEvent->canAddActions())
			{
				if(pEvent->m_gridRec(this).PtInRect(point))
				{
					useHandCursor = true;
					pEvent->mouseOverNewAction = true;
				}
			}
			if(pEvent->m_type == 2 && pEvent->m_open(this)) // Group
			{
				CRect Footer = pEvent->m_rect(this);
				Footer.top = Footer.bottom - 17;
				Footer.left += EVENT_INDENT * 2;


				if(Footer.PtInRect(point))
				{
					useHandCursor = true;
					pEvent->mouseOverFooter = true;
				}
			}


			pLastEvent = pEvent;

			this->Invalidate();
		}
	}
	////////////////////////
	// Change to a hand cursor if overlapping "new event to group"


	static HCURSOR handCursor = LoadCursor(NULL, IDC_HAND);
	static HCURSOR arrowCursor = LoadCursor(NULL, IDC_ARROW);

	if (useHandCursor)
		SetCursor(handCursor);
	//else
	//	SetCursor(arrowCursor);
	////////////////////////

	int objid = 0;
	CPoint pt = point;   // copy!	
	CRect myrect;
	GetWindowRect(&myrect);


	CRect rect;
	rect.top = pt.y;
	rect.left = pt.x;
	rect.right = pt.x + 20;
	rect.bottom = pt.y + 20;

	CScrollView::OnMouseMove(nFlags, point);
}

void EventSheetEditor::OnLButtonUp(UINT nFlags, CPoint pt) 
{
	if(KillFocusOnInlineEditor())
		return;
	m_Drag.IsDragging = false;
	m_leftDown = false;

	if(m_pHeader->m_isDrag)
	{
		ReleaseCapture();
		m_pHeader->OnLeftUp(pt);	
	}

	CEditorEvent* pMyEvent = EventAt(pt);

	if(pMyEvent)
	{
		if(!pMyEvent->PtInOnOffButton(pt, this))
		{
			CEditorCondition* pMyCondition = pMyEvent->conditionAt(pt, this);
			if( pMyCondition == m_FocusCondition && pMyCondition != 0)
			{
				if(m_ClickedSelectedCondition)
				{
					// User clicked on a selected *condition* in the 'on left mouse down'
					if(m_CtrlDown)
					{
						pMyCondition->m_select(this) = false;
						InvalidateRect(pMyCondition->m_rect(this));
					}
					else
					{
						//TODO: Edit the condition or the param or something like lol
						InlineEditCondition(*pMyCondition, pt);
					}
				}
			}
			else
			{
				CEditorAction* pMyAction = pMyEvent->actionAt(pt,this);
				if( pMyAction == m_FocusAction && pMyAction != 0)
				{
					if(m_ClickedSelectedAction)
					{
						// User clicked on a selected *action* in the 'on left mouse down'
						
						if(m_CtrlDown)
						{
							pMyAction->m_select(this) = false;	
							InvalidateRect(pMyAction->m_rect(this));
						}
						else
						{
						//TODO: Edit the condition or the param or something like lol
							InlineEditAction(*pMyAction, pt);
						}
					}
				}
				else
				{

					// User clicked on a selected *event* in the 'on left mouse down'
					if( pMyEvent == m_FocusEvent && pMyEvent != 0)
					{
						if(m_ClickedSelectedEvent)
						{
							pMyEvent->m_select(this) = false;
							if(!m_CtrlDown)
							{
								DeselectEvents();
							}
							InvalidateRect(pMyEvent->m_rect(this));
						}
					}
			
				}
			}
		}
	}

	else
	{
		if(m_pHeader->m_rect.PtInRect(pt))
			m_pHeader->OnLeftUp(pt);
	}

	m_ClickedSelectedAction = false;
	m_ClickedSelectedCondition = false;
	m_ClickedSelectedEvent = false;

	CScrollView::OnLButtonUp(nFlags, pt);
}

void EventSheetEditor::OnLButtonDown(UINT nFlags, CPoint pt) 
{
	if (!(pt.x >= m_pHeader->m_Split - 2 && pt.x <= m_pHeader->m_Split + 5))
	{
		CEditorEvent* pEvent = EventAt(pt);
		if(pEvent)
		{
			if(pEvent->canAddActions())
			{
				if(pEvent->mouseOverNewAction)
				{
					AddAction(pEvent, -1);
					return;
				}
				if(pEvent->mouseOverFooter)
				{
					AddCondition(false, NULL, true, pEvent);
					return;
				}		
			}
		}
	}

	if(KillFocusOnInlineEditor())
		return;
	m_ActionTooltip.Hide();

	CEditorEvent* pMyEventParent = NULL;
	CEditorEvent* pMyEvent = EventAt(pt, &pMyEventParent);
	m_leftDown = true;
	
	//if(m_pHeader->m_rect.PtInRect(pt))
	if(pt.x >= m_pHeader->m_Split - 2 && pt.x <= m_pHeader->m_Split + 5)
	{
		m_InsertAfter.Hide();
		m_InsertBefore.Hide();
		m_InsertSub.Hide();
		m_ActionTooltip.Hide();
		m_pHeader->OnLeftClick(pt);
		if(m_pHeader->m_isDrag)
		{
			SetCapture();
		}
		else // basically just if we arn't doing something interesting in the header...
		{
			if(!m_CtrlDown)
			{
				DeselectEvents();
				DeselectActions();
				DeselectConditions();
			}
		}



		for(int a = 0; a < m_pHeader->m_Objects.size(); a ++ )
		{
			if(m_pHeader->m_Objects.at(a).m_rect.PtInRect(pt))
			{
				m_Drag.dragtype = OBJECT;
				m_Drag.StoredPos = pt;
				m_Drag.CtrlDown = false;
			}
		}
	}
	else if(pMyEvent)
	{
		if(pMyEvent->PtInOnOffButton(pt, this))
		{
			// The user is clicked the + button
			pMyEvent->m_open(this) = pMyEvent->m_open(this) ? false:true;
			Invalidate();
			
		}

		else
		{
			///////////////
			// Select Condition
			
			CEditorCondition* pMyCondition = pMyEvent->conditionAt(pt, this);
			if(pMyCondition)
			{
				// find index
				int index = 0;
				for(int a = 0; a < pMyEvent->m_Conditions.size() ; a ++ )
					if(pMyEvent->m_Conditions[a] == pMyCondition)
						index = a;

				//Deselect selected events or actions
				DeselectEvents();
				DeselectActions();
				

				// Now the conditions
	
				if(pMyCondition->m_select(this)) // if we clicked on something already selected...wait till release
					m_ClickedSelectedCondition = true;
				else
				{
					if(!m_CtrlDown)
					{
						DeselectConditions();
					}
					pMyCondition->m_select(this) = true;
					InvalidateRect(pMyCondition->m_rect(this));
				}

				if( m_ShiftDown && m_FocusCondition)
				{
					bool b1 = false, b2 = false, b3 = false;
					RecursiveSelect(NULL, m_FocusCondition,NULL ,NULL ,pMyCondition ,NULL,b1,b2,b3, this->m_pEventList);
				}

				m_FocusCondition = pMyCondition;
				m_FocusEvent = pMyEvent;

				if(m_CtrlDown)
				{
					m_InsertAfter.Hide();
					m_InsertBefore.Hide();
					m_InsertSub.Hide();
				}
				else
				{	
					if(m_ClickedSelectedCondition)
					{
						CPoint point  = pMyCondition->m_rect(this).TopLeft();
						point.x = pt.x - 50;
						point.y -= 19;
						if(pMyEvent->m_displayConditionType(this) == 1)
						{
							point.y += 19;
							point.x = pMyCondition->m_rect(this).TopLeft().x - 100;
						}
						m_InsertBefore.ShowAt(point, pMyCondition, *this);
						m_InsertBefore.pEventEditor = (EventSheetEditor*)(this);


						point.y += 19 + pMyCondition->m_rect(this).Height();
						if(pMyEvent->m_displayConditionType(this) == 1)
						{
							point.y -= 19 + pMyCondition->m_rect(this).Height();
							point.x = pMyCondition->m_rect(this).right;
						}

						m_InsertAfter.ShowAt(point, pMyCondition, *this);
						m_InsertAfter.pEventEditor = (EventSheetEditor*)(this);

						m_InsertAfter.m_pInsertEventSpot = m_InsertBefore.m_pInsertEventSpot = pMyEvent;

						m_InsertAfter.m_InsertType       = m_InsertBefore.m_InsertType       = CONDITION;
						m_InsertAfter.m_InsertIndex      = m_InsertBefore.m_InsertIndex      = index;
						m_InsertAfter.m_IsAfter = true;
						m_InsertBefore.m_IsAfter = false;
						m_ActionTooltip.Hide();
					}
				}


				// We may be wanting to drag these conditions...
				m_Drag.dragtype = CONDITION;
				m_Drag.StoredPos = pt;
				m_Drag.IsDragging = false;
			}
			else
			{
				/////////////////////
				// Select Action

				SelectedActionVector actions;
				
				CEditorAction* pMyAction = pMyEvent->actionAt(pt, actions, this);

				if(pMyAction)
				{
					int index = 0;
					for(int a = 0; a < pMyEvent->m_Actions.size() ; a ++ )
						if (pMyEvent->m_Actions[a] == pMyAction)
							index = a;
			

					//Deselect selected events or conditions
					DeselectEvents();
					DeselectConditions();

					if(pMyAction->m_select(this)) // if we clicked on something already selected...wait till release
						m_ClickedSelectedAction = true;

					else
					{
						if(!m_CtrlDown)
						{
							DeselectActions();
						}
						pMyAction->m_select(this) = true;
						InvalidateRect(pMyAction->m_rect(this));						
					}
					if( m_ShiftDown && m_FocusAction)
					{
						bool b1 = false, b2 = false, b3 = false;
						RecursiveSelect(m_FocusAction,NULL ,NULL ,pMyAction , NULL ,NULL,b1,b2,b3, this->m_pEventList);
					}
					m_FocusAction = pMyAction;
					m_FocusEvent = pMyEvent;
					
					// We may be wanting to drag these conditions...
					m_Drag.dragtype = ACTION;
					m_Drag.StoredPos = pt;
					m_Drag.IsDragging = false;

		
					if(m_CtrlDown)
					{
						m_InsertAfter.Hide();
						m_InsertBefore.Hide();
						m_InsertSub.Hide();
					}
					else
					{

						if(m_ClickedSelectedAction)
						{
							CPoint point = pMyAction->m_rect(this).TopLeft();
							point.x = pt.x - 50;
							point.y -= 19;

							if(pMyEvent->m_displayActionType(this) == 1)
							{
								point.y += 19;
								point.x = pMyAction->m_rect(this).TopLeft().x - 100;
							}

							m_InsertBefore.ShowAt(point, pMyAction, *this);
							m_InsertBefore.pEventEditor = (EventSheetEditor*)(this);

							point.y += 19 + pMyAction->m_rect(this).Height();
						
							if(pMyEvent->m_displayActionType(this) == 1)
							{
								point.y -= 19 + pMyAction->m_rect(this).Height();
								point.x = pMyAction->m_rect(this).right;
							}

							m_InsertAfter.ShowAt(point, pMyCondition, *this);
							m_InsertAfter.pEventEditor = (EventSheetEditor*)(this);

							m_InsertAfter.m_pInsertEventSpot = m_InsertBefore.m_pInsertEventSpot = pMyEvent;
							m_InsertAfter.m_InsertType       = m_InsertBefore.m_InsertType       = ACTION;
							m_InsertAfter.m_InsertIndex      = m_InsertBefore.m_InsertIndex      = index;
							m_InsertAfter.m_IsAfter = true;
							m_InsertBefore.m_IsAfter = false;

							m_ActionTooltip.Hide();
						}


					}	
				}

				/////////////////////
				// Select Event

				else //if(!pMyEvent->m_gridRec(this).PtInRect(pt))  -edit: we arn't going to worry about left clicking in the grid, it may as well select the event 
				{
					//Deselect selected actions or conditions
					DeselectActions();
					DeselectConditions();

					m_InsertAfter.Hide();
					m_InsertBefore.Hide();
					m_InsertSub.Hide();
					m_ActionTooltip.Hide();


					// Now select the events
					
					if(pMyEvent->m_select(this)) // if we clicked on something already selected...wait till release
						m_ClickedSelectedEvent = true;
					else
					{
					// User is clicking on an event
						if(!m_CtrlDown)
						{
							DeselectEvents();
						}
						
						pMyEvent->Select(true, this);
						InvalidateRect(pMyEvent->m_rect(this));
						
					}
					if( m_ShiftDown && m_FocusEvent)
					{
						bool b1 = false, b2 = false, b3 = false;
						RecursiveSelect(NULL, NULL, m_FocusEvent, NULL,NULL, pMyEvent,b1,b2,b3, this->m_pEventList);
					}
					m_FocusEvent = pMyEvent;
					// We may be wanting to drag these events...
					m_Drag.dragtype = EVENT;
					m_Drag.StoredPos = pt;
					m_Drag.IsDragging = false;

					if(pt.x > this->m_pHeader->m_Split) //prevent dragging events unless the mouse is in condition column
						m_Drag.dragtype = -1;

					if(m_CtrlDown)
					{
						m_InsertAfter.Hide();
						m_InsertBefore.Hide();
						m_InsertSub.Hide();
					}
					else
					{	
						if(m_ClickedSelectedEvent)
						{
							// find index
							int index = 0;
							FindEventListEventExistsIn(pMyEvent, m_pEventList, index);
						
							CPoint point  = pMyEvent->m_rect(this).TopLeft();
							point.x += 24;
							point.y -= 20;

							m_InsertBefore.ShowAt(point, pMyEvent, *this);
							m_InsertBefore.pEventEditor = (EventSheetEditor*)(this);


							point.y += 20 + pMyEvent->m_rect(this).Height();
							
							m_InsertAfter.ShowAt(point, pMyEvent, *this);
							m_InsertAfter.pEventEditor = (EventSheetEditor*)(this);

							point.x += 101;
							
							if(pMyEvent->canAddEvents())
							{
								m_InsertSub.ShowAt(point, pMyEvent, *this);
								m_InsertSub.pEventEditor = (EventSheetEditor*)(this);
							}

							m_InsertAfter.m_pInsertEventSpot = m_InsertBefore.m_pInsertEventSpot = pMyEventParent;
							m_InsertSub.m_pInsertEventSpot = pMyEvent;
							m_InsertAfter.m_InsertType       = m_InsertBefore.m_InsertType       = m_InsertSub.m_InsertType       = EVENT;
							m_InsertAfter.m_InsertIndex      = m_InsertBefore.m_InsertIndex      = m_InsertSub.m_InsertIndex      = index;

							m_InsertAfter.m_IsAfter = true;
							m_InsertBefore.m_IsAfter = false;

							m_InsertSub.m_IsAfter = true;
							m_InsertSub.m_IsSub = true;


							m_ActionTooltip.Hide();
						}
					}
					
				}
				/*else
				{
					if(!m_CtrlDown)
					{
						DeselectEvents();
						DeselectActions();
						DeselectConditions();
					}


				}*/
			}
		}
	}
	else
	{
		if(!m_CtrlDown)
		{
			DeselectActions();
			DeselectConditions();
			DeselectEvents();

			m_InsertAfter.Hide();
			m_InsertBefore.Hide();
			m_InsertSub.Hide();
			m_ActionTooltip.Hide();
		}
	}
	CScrollView::OnLButtonDown(nFlags, pt);
}