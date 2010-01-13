#include "stdafx.h"
#include "ChronoEventEditor\ChronoEventEditor.h"
#include "Event Sheet Editor\Event Sheet Editor.h"

#include "..\MainFrm.h"

extern CMainFrame* g_MainFrame;

void CEditorEvent::Init()
{
	m_Anim.m_bDestroyed = false;
	m_select_default  = false;
	m_open_default = true;
	m_rect_default = CRect(0,0,32,32);
	m_type = 0;
	m_bEnabled = true;

	m_displayActionType_default = 3;
	m_displayConditionType_default  = 3;
	m_displayEventType_default  = 1;

	m_gridRec_default =
		m_handle_default =
		m_CondArea_default =
		CRect(0,0,0,0); //


}

void EventSheet::SetChanged(bool changed_)
{
	changed = changed;

	g_MainFrame->tabs.Invalidate();
}

CEditorEvent* CEditorEvent::EventAt(CPoint pt, EventSheetEditor* pEd, CEditorEvent** pRecieveParent)
{
	if(m_Anim.m_bDestroyed) 
		return 0;
	if(m_handle(pEd).PtInRect(pt) && m_visible(pEd))
		return this;
	
	CEditorEvent* pTemp = 0;
	if(m_rect(pEd).PtInRect(pt))
	{
		for(int a = 0; a < m_EventList.size() ; a ++ )
		{
			int n = 0;
			pTemp = m_EventList[a]->EventAt(pt, pEd, pRecieveParent);
			if(pTemp && !pTemp->m_Anim.m_bDestroyed && pTemp->m_visible(pEd))
			{
				if(pTemp->m_visible(pEd))
				{
				if(m_EventList[a] == pTemp) // the child returned itself
					if(pRecieveParent)
						*pRecieveParent = this;
				
					return pTemp;
				}
			}
		}
		if(m_visible(pEd))
			return this;
	}
	return 0;
}

CEditorCondition* CEditorEvent::conditionAt(CPoint pt, EventSheetEditor* pEd)
{
	if(this->canAddConditions())
	for(int a = 0; a < m_Conditions.size() ; a ++ )
		if( m_Conditions[a]->m_rect(pEd).PtInRect(pt) && 
			m_Conditions[a]->m_visible &&
			!m_Conditions[a]->m_Anim.m_bDestroyed
			)
			return m_Conditions[a];
	
	return 0;
}

CEditorAction* CEditorEvent::actionAt(CPoint pt, EventSheetEditor* pEd)
{
	if(this->canAddActions())
	for(int a = 0; a < m_Actions.size() ; a ++ )
		if( m_Actions[a]->m_rect(pEd).PtInRect(pt) && 
			m_Actions[a]->m_visible &&
			!m_Actions[a]->m_Anim.m_bDestroyed)
			return m_Actions[a];
	
	return 0;
}

CEditorAction* CEditorEvent::actionAt(CPoint pt,SelectedActionVector& Actions, EventSheetEditor* pEd)
{
	if(!this->canAddActions())
		return 0;
	for(int a = 0; a < m_Actions.size() ; a ++ )
	{

		if( m_Actions[a]->m_rect(pEd).PtInRect(pt) && 
			m_Actions[a]->m_visible &&
			!m_Actions[a]->m_Anim.m_bDestroyed)
			Actions.push_back(m_Actions[a]);
	}
	if(Actions.size() != 0 )
		return (Actions[Actions.size()-1]);
	else
		return 0;
}



bool CEditorEvent::OnToggleBookmarks(CEditorEvent& Event, EventSheetEditor& Ed)
{
	if (m_select(&Ed))
		Event.m_bBookmarked = !Event.m_bBookmarked;

	for(int a = 0; a < Event.m_EventList.size(); a++)
	{
//
	}

	return false;
}

CRect& CEditorEvent::m_rect(EventSheetEditor* Ed){
	return GetMapItemFunction_CRect(& m_ED_rect, m_rect_default, Ed); 
}
CRect& CEditorEvent::m_gridRec(EventSheetEditor* Ed){
	return GetMapItemFunction_CRect(& m_ED_gridRec, m_gridRec_default, Ed);   	
}
CRect& CEditorEvent::m_handle(EventSheetEditor* Ed){
	return GetMapItemFunction_CRect( &m_ED_handle, m_handle_default, Ed);   	
}
CRect& CEditorEvent::m_CondArea(EventSheetEditor* Ed){
	return GetMapItemFunction_CRect( &m_ED_CondArea, m_CondArea_default, Ed);   	
}
BOOL& CEditorEvent::m_visible(EventSheetEditor* Ed)
{		return GetMapItemFunction_bool(& m_ED_visible, m_visible_default, Ed);   	}

BOOL& CEditorEvent::m_select(EventSheetEditor* Ed)
{		return GetMapItemFunction_bool(& m_ED_select, m_select_default, Ed);   	}

int& CEditorEvent::m_displayActionType(EventSheetEditor* Ed)
{		return GetMapItemFunction_int( &m_ED_displayActionType, m_displayActionType_default, Ed);   	}

int&  CEditorEvent::m_displayConditionType(EventSheetEditor* Ed)
{		return GetMapItemFunction_int( &m_ED_displayConditionType, m_displayConditionType_default, Ed);   	}


int&  CEditorEvent::m_displayEventType(EventSheetEditor* Ed)
{		return GetMapItemFunction_int( &m_ED_displayEventType, m_displayEventType_default, Ed);   	}

BOOL&  CEditorEvent::m_open(EventSheetEditor* Ed)
{		return GetMapItemFunction_bool( &m_ED_open, m_open_default, Ed);   	}

CEditorEvent::~CEditorEvent()
{
	if (freeMemory) {
		for(int a = 0; a < m_Actions.size(); a++)
			delete m_Actions[a];
		for(int c = 0; c < m_Conditions.size(); c++)
			delete m_Conditions[c];
		for(int e = 0; e < m_EventList.size(); e++)
		{
			delete m_EventList[e];
		}
	}
}

bool CEditorEvent::Serialize(CArchive &ar, bool Save, EventSheetEditor* Ed)
{
	CString ExpectedName = "CEditorEvent";
	int     Version      = 2;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return false;

	if( ar.IsLoading())
	{
		ar >> m_type;
		ar >> m_handle_default;
		ar >> m_displayActionType_default;
		ar >> m_displayConditionType_default;
		ar >> m_displayEventType_default;
		ar >> m_select_default;
		ar >> m_Anim.Space;
		this->m_Anim.m_bDestroyed= false;

		// Bookmarks & enabled
		ar >> m_bBookmarked;

		if (Version >= 2)
			ar >> m_bEnabled;
		else
			m_bEnabled = true;

		unsigned int csize;
		unsigned int asize;
		unsigned int esize;
	
		switch (m_type)
		{

		case EVENT_TYPE: // Events
			
			ar >> m_open_default;
			ar >> m_BreakPoint;

			// Serialize the conditions
			ar >> csize;
			for(int c = 0; c < csize; c ++ )
			{
				CEditorCondition* pCon = new CEditorCondition;
				
				if (!pCon->Serialize(ar, Ed))
					return false;

				m_Conditions.push_back(pCon);
			}

			// Serialize the actions
			ar >> asize;
			for(int a = 0; a < asize; a ++ )
			{
				CEditorAction* pAct = new CEditorAction;
				
				if (!pAct->Serialize(ar, Ed))
					return false;

				m_Actions.push_back(pAct);
			}

			// Serialize child events
			ar >> esize;
			for(int e = 0; e < esize; e ++ )
			{
				CEditorEvent* Evt = new CEditorEvent;
				Evt->Init();
				if (!Evt->Serialize(ar,Save, Ed))
					return false;

				if(Evt->m_select_default || Save)
					m_EventList.push_back(Evt);

				else
					for(int a = 0; a < Evt->m_EventList.size(); a ++)
						m_EventList.push_back(Evt->m_EventList[a]);
			}

			if(Ed)
				((EventSheetEditor*)Ed)->ReformatEvent(this);
			
			break;

		case COMMENT_TYPE: // Comments

			ar >> m_cText;
			ar >> m_cTextBG;
			ar >> m_cTextColour;
			ar >> m_cFontName;
			ar >> m_cFontSize;
			ar >> m_cbFontBold;
			ar >> m_cbFontItalic;
			ar >> m_cbFontStrike;
			ar >> m_cbFontUnderline;

			break;
		case GROUP_TYPE: // Groups

			ar >> m_open_default;
			ar >> m_sActive;
			ar >> m_sDescription;
			ar >> m_sTitle;

			ar >> esize;
			for(int e = 0; e < esize; e ++ )
			{
				CEditorEvent* Evt = new CEditorEvent;
				Evt->Init();			
				if (!Evt->Serialize(ar,Save, Ed))
					return false;

				if(Evt->m_select_default || Save)
					m_EventList.push_back(Evt);
				else
					for(int a = 0; a < Evt->m_EventList.size(); a ++)
						m_EventList.push_back(Evt->m_EventList[a]);
			}

			break;

		case INCLUDE_TYPE:
			ar >> event_sheet_include;
			break;

		case SCRIPT_TYPE: // Comments

			ar >> m_cText;
			ar >> m_cTextBG;
			ar >> m_cTextColour;
			ar >> m_cFontName;
			ar >> m_cFontSize;
			ar >> m_cbFontBold;
			ar >> m_cbFontItalic;
			ar >> m_cbFontStrike;
			ar >> m_cbFontUnderline;

			break;
		}
		// Done Loading
	}

	else
	{
		ar << m_type;
		ar << m_handle(Ed);

		ar << m_displayActionType(Ed);
		ar << m_displayConditionType(Ed);
		ar << m_displayEventType(Ed);

		ar << m_select(Ed);
		ar << m_Anim.Space;

		// Bookmarks & enabled
		ar << m_bBookmarked;
		ar << m_bEnabled;

		switch (m_type)
		{
		case EVENT_TYPE:	//Events
		{
			ar << m_open(Ed);
			ar << m_BreakPoint;
		
			// Serialize the conditions
			int ConditionsSize = 0;
			for(int c = 0; c < m_Conditions.size(); c ++ )
				if(m_Conditions[c]->ValidToDisplay(NULL))
					ConditionsSize ++;
			ar << ConditionsSize;
			for(int c = 0; c < m_Conditions.size(); c ++ )
				if(m_Conditions[c]->ValidToDisplay(NULL))
					m_Conditions[c]->Serialize(ar, Ed);

			// Serialize the actions
			int ActionsSize = 0;
			for(int a = 0; a < m_Actions.size(); a ++ )
				if(m_Actions[a]->ValidToDisplay(NULL))
					ActionsSize ++;
			ar << ActionsSize;
			for(int a = 0; a < m_Actions.size(); a ++ )
				if(m_Actions[a]->ValidToDisplay(NULL))
					m_Actions[a]->Serialize(ar, Ed);

			// Serialize child events
			ar << m_EventList.size();
			for(int e = 0; e < m_EventList.size(); e ++ )
				m_EventList[e]->Serialize(ar);
			}
			break;
			
		
		case COMMENT_TYPE: // Comments

			ar << m_cText;
			ar << m_cTextBG;
			ar << m_cTextColour;
			ar << m_cFontName;
			ar << m_cFontSize;
			ar << m_cbFontBold;
			ar << m_cbFontItalic;
			ar << m_cbFontStrike;
			ar << m_cbFontUnderline;
			break;
		case GROUP_TYPE:	// Groups
			ar << m_open(Ed);
			ar << m_sActive;
			ar << m_sDescription;
			ar << m_sTitle;

			// Davo... groups have subevents...
			ar << m_EventList.size();
			for(int e = 0; e < m_EventList.size(); e ++ )
				m_EventList[e]->Serialize(ar,Save, Ed);

			break;
	
		case SCRIPT_TYPE:	// Groups
			ar << m_cText;
			ar << m_cTextBG;
			ar << m_cTextColour;
			ar << m_cFontName;
			ar << m_cFontSize;
			ar << m_cbFontBold;
			ar << m_cbFontItalic;
			ar << m_cbFontStrike;
			ar << m_cbFontUnderline;

			break;
		case INCLUDE_TYPE:
			ar << event_sheet_include;
		}
		// Done

	}

	return true;
}

bool CEditorEvent::PtInOnOffButton(CPoint pt, EventSheetEditor* pEd)
{
	CRect OnOffButton;
	OnOffButton = CRect(0,0,11,11);

	int yoffset = m_CondArea(pEd).bottom;

	//revised 12-2-07
	//  we need it to be inline with the bottom condition
	/*for(int a = 0; a < m_Conditions.size(); a ++)
	{
		yoffset += m_Conditions[a]->m_rect(pEd).Height()+1;


	}*/

	OnOffButton.OffsetRect(m_handle(pEd).left+EVENT_TAB_BUTTONX-1,yoffset-EVENT_TAB_BUTTONY-1);
	return (OnOffButton.PtInRect(pt) && (m_EventList.size()>0 || m_type == GROUP_TYPE));
}

void CEditorEvent::Select(bool select, EventSheetEditor* pEd)
{
	m_select(pEd) = select;
	for(int a = 0; a < m_EventList.size(); a ++)
	{
		m_EventList[a]->Select(select, pEd);
	}
}

bool CEditorEvent::DeleteSelectedEvents(EventVector& ParentEventList, int& parentindex, EventSheetEditor& Ed, int* subtractfromanim)
{
	int addindex = 0;

	for( int a = 0; a < m_EventList.size(); a ++ )
	{
		if(!m_EventList[a]->m_Anim.m_bDestroyed)
		{
			int subtract_from_anim = 0;
			if(m_EventList[a]->DeleteSelectedEvents(m_EventList,a,Ed, &subtract_from_anim))
			{
				m_EventList[a]->Remove(true, &Ed);
				m_EventList[a]->m_Anim.StartSpace -= subtract_from_anim;
				a--;
			}

			else if( m_select(&Ed))
			{
				//basically we are removing a parent event but not its child event!
				// so we need to take the child event out of the event
				// and stick it in the same list as its parent
				CEditorEvent* child = m_EventList[a];
				m_EventList.erase(m_EventList.begin() + a);	// remove but dont deallocate the event	
				addindex ++;
				ParentEventList.insert(ParentEventList.begin()+parentindex+addindex, child);
				
				if(subtractfromanim)
					*subtractfromanim += child->m_rect(&Ed).Height();
				a--;
			}
		}
	}

	return m_select(&Ed) ? true : false;
}

bool CEditorEvent::InstantDeleteSelectedEvents(EventVector& ParentEventList, int& parentindex, EventSheetEditor* pEd)
{
	for( int a = 0; a < m_EventList.size(); a ++ )
	{
		if(!m_EventList[a]->m_Anim.m_bDestroyed)
		{
			if(m_EventList[a]->InstantDeleteSelectedEvents(m_EventList,a,pEd))
			{
				m_EventList[a]->Remove(false, pEd);
				a--;
			}

			else if( m_select(pEd))
			{
				//basically we are removing a parent event but not its child event!
				CEditorEvent* child = m_EventList[a];

				m_EventList.erase(m_EventList.begin() + a);	// remove but dont deallocate the event
		
				//PORT: insert() returns an iterator to the element inserted
				ParentEventList.insert(ParentEventList.begin()+parentindex+1, child);
				a--;
			}
		}
	}

	return m_select(pEd) ? true : false;

}

void CEditorEvent::Remove(bool Animate, EventSheetEditor* pEd)
{
	if(Animate)
		m_Anim.Start(pEd->m_Ellapsed);
	else
	{
		m_Anim.Space = 0;
		m_Anim.StartSpace=0;
		m_Anim.m_bDestroyed = true;
		m_select(pEd) = false;
	}
}

void CEditorEvent::ForceTriggersToTop()
{
	// Step 1. Check if we actually need to...
	bool beenNonTriggers = false;
	bool needsUpdating = false;
	for(ConditionVector::iterator i = m_Conditions.begin(); i != m_Conditions.end(); i++)
	{
		if( (*i)->IsTrigger(NULL) )
		{
			if(beenNonTriggers)
				needsUpdating = true;
		}
		else
			beenNonTriggers = true;
	}

	// We found some triggers in the wrong place...we need to force them to the top
	if(needsUpdating)
	{
		ConditionVector newConditions;
		for(ConditionVector::iterator i = m_Conditions.begin(); i != m_Conditions.end(); i++)
		{
			if( (*i)->IsTrigger(NULL) )
				newConditions.push_back(*i);
		}
		for(ConditionVector::iterator i = m_Conditions.begin(); i != m_Conditions.end(); i++)
		{
			if( !(*i)->IsTrigger(NULL) )
				newConditions.push_back(*i);
		}

		// Copy sorted list
		m_Conditions = newConditions;
	}
}

