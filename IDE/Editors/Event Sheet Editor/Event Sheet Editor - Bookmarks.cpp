/////////////////////////////////////////////////////////////////////////////
// Event sheet editor - bookmarks
/////////////////////////////////////////////////////////////////////////////
 
// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "..\..\Construct Doc.h"
#include "Event Sheet Editor.h"

void EventSheetEditor::OnBookmark()
{
	CreateUndo();

	// Toggle bookmark status for all events
	SelectedEventVector SelectedEvents;
	CreateEventSelectionVector(SelectedEvents, (*m_pEventList));

	for(int a = 0; a < SelectedEvents.size(); a ++)
	{
		SelectedEvents[a]->m_bBookmarked = !SelectedEvents[a]->m_bBookmarked;
		InvalidateRect(SelectedEvents[a]->m_rect(this));
	}
}

void EventSheetEditor::RemoveBookmarks()
{
	CreateUndo();

	for (int i = 0; i < m_pEventList->size(); i++)
	{
		m_pEventList->at(i)->m_bBookmarked = false;
		InvalidateRect(m_pEventList->at(i)->m_rect(this));
	}
}

void EventSheetEditor::NextBookmark()
{
	bool found = false;
	bool found_first = false;
	bool found_vis = false;
	bool down = true;
	int y = 0;
	bool closed = false;
	for(int a = 0; a < m_pEventList->size(); a++)
	{
		FindBookmarkEvent(m_pEventList->at(a), found, found_first, found_vis, down, y, closed);
		if(found)
			a = m_pEventList->size();
	}
	if(!found)
		y = GetScrollBarCtrl(1)->GetScrollLimit();
	if(y > GetScrollBarCtrl(1)->GetScrollLimit())
		y = GetScrollBarCtrl(1)->GetScrollLimit();
	SetScrollPos(1,y, true);
	Invalidate();
}

void EventSheetEditor::PreviousBookmark()
{
	bool found = false;
	bool found_first = false;
	bool found_vis = false;
	bool down = false;
	int y = 0;
	bool closed = false;
	for(int a = m_pEventList->size()-1; a >= 0 ; a--)
	{
		FindBookmarkEvent(m_pEventList->at(a), found, found_first, found_vis, down, y, closed);
		if(found)
			a = 0;
	}
	if(!found)
		y = 0;
	if(y > GetScrollBarCtrl(1)->GetScrollLimit())
		y = GetScrollBarCtrl(1)->GetScrollLimit();
	SetScrollPos(1,y, true);
	Invalidate();
}

void EventSheetEditor::FindBookmarkEvent(CEditorEvent* pParent, bool& found, bool& found_first, bool& found_vis, bool& down, int& y, bool& closed)
{
	bool local_closed = closed;
	if(!local_closed)
	{
		local_closed = pParent->m_open(this);
		if(local_closed)
		{
			y = pParent->m_rect(this).top-31 + this->ScrollPos.y;
		}
	}

	if(found)
		return;

	for(int a = 0; a < pParent->m_EventList.size(); a++)
	{
		int e = a;
		if(!down)
			e = pParent->m_EventList.size()-1-a;
		CEditorEvent* child = pParent->m_EventList.at(e);
		

	
		FindBookmarkEvent(child, found, found_first, found_vis, down, y, local_closed);
		if(found)
		{
			closed = local_closed;
			return;

		}
	}
	
	// If we find a visible event, then we are ready to start looking for bookmarks...
	if(pParent->m_visible(this))
	{
		found_vis = true;
	}


	if(pParent->m_bBookmarked)
	{
		if(!closed)
			y = pParent->m_rect(this).top-31+ this->ScrollPos.y;
		
		
		if(found_vis)
		{
			if(pParent->m_rect(this).top != 31 || !pParent->m_visible(this)) // this will be 31 if u have just bookmarked to that spot
				found_first = true;
			if(found_first)
			{
				found = true;
				if(!down)
				{
					if(pParent->m_visible(this))
						found = false;
				}
				if(found)
					return;	
			}
			else
			{
				found_first = true;
			}	
		}
	}


}