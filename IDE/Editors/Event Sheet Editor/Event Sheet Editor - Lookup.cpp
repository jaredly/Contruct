/////////////////////////////////////////////////////////////////////////////
// Event sheet editor - lookup
/////////////////////////////////////////////////////////////////////////////
 
// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "..\..\Construct Doc.h"
#include "Event Sheet Editor.h"

CEditorEvent* EventSheetEditor::EventAt(CPoint pt, CEditorEvent** pRecieveParent)
{
	if(pRecieveParent)
		*pRecieveParent = 0;
	CEditorEvent* ptempEvent;
 	for(int a = 0; a < (*m_pEventList).size() ; a ++ )
	{
		int n = 0;
		ptempEvent = (*m_pEventList)[a]->EventAt(pt, this, pRecieveParent);
		if(ptempEvent)
		{
			return ptempEvent;
		}
	}

	return 0;
}

int EventSheetEditor::EventNumberAt(CPoint pt)
{
 	for(int a = 0; a < (*m_pEventList).size() ; a ++ )
	{
		if ((*m_pEventList)[a]->m_rect(this).PtInRect(pt)) return a;
	}

	return -1;
}

void EventSheetEditor::RecursiveSelect(
		CEditorAction* action1,
		CEditorCondition* condition1,
		CEditorEvent* event1, 
		CEditorAction* action2,
		CEditorCondition* condition2,
		CEditorEvent* event2,
		bool& inside_action, bool& inside_condition ,bool& inside_event, EventVector* m_pEventList)
{
	for( EventVector::iterator e = m_pEventList->begin(); e!= m_pEventList->end(); e++)
	{
		if(event1 == *e)
		{
			inside_event = 1 - inside_event;
			(*e)->m_select(this) = true;
			InvalidateRect((*e)->m_rect(this));
		}
		if(event2 == *e)
		{
			inside_event = 1 - inside_event;
			(*e)->m_select(this) = true;
			InvalidateRect((*e)->m_rect(this));
		}
		if(inside_event)
		{
			(*e)->m_select(this) = true;
			InvalidateRect((*e)->m_rect(this));
		}
		// loop actions
		for( ActionVector::iterator a  = (*e)->m_Actions.begin(); a!= (*e)->m_Actions.end(); a++)
		{
			if(action1 == *a)
			{
				inside_action = 1-inside_action;
				(*a)->m_select(this) = true;
				InvalidateRect((*a)->m_rect(this));
			}
			if(action2 == *a)
			{
				inside_action = 1-inside_action;
				(*a)->m_select(this) = true;
				InvalidateRect((*a)->m_rect(this));
			}
			if(inside_action)
			{
				(*a)->m_select(this) = true;
				InvalidateRect((*a)->m_rect(this));
			}
		}
		// loop conditions
		for( ConditionVector::iterator c  = (*e)->m_Conditions.begin(); c!= (*e)->m_Conditions.end(); c++)
		{
			if(condition1 == *c)
			{
				inside_condition = 1-inside_condition;
				(*c)->m_select(this) = true;
				InvalidateRect((*c)->m_rect(this));
			}
			if(condition2 == *c)
			{
				inside_condition = 1-inside_condition;
				(*c)->m_select(this) = true;
				InvalidateRect((*c)->m_rect(this));
			}
			if(inside_condition)
			{
				(*c)->m_select(this) = true;
				InvalidateRect((*c)->m_rect(this));
			}
		}

		RecursiveSelect(action1, condition1, event1, action2,condition2, event2,inside_action,inside_condition, inside_event, &(*e)->m_EventList);


	}

}