// ChronoEventEditor.cpp : implementation file
//

#include "stdafx.h"
#include "ChronoEventEditor.h"
#include "..\Event Sheet Editor\Event Sheet Editor.h"

#include "..\..\Structure\Export\Export.h"
CHTMLDrawer g_HTMLDraw;

#include"..\..\LIBS\Include\Resources\Resource.h"
#include "..\..\Utilities\Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
extern HINSTANCE ghInst;

/////////////////////////////////////////////////////////////////////////////
// CChronoEventEditor

IMPLEMENT_DYNCREATE(CChronoEventEditor, CScrollView)

CChronoEventEditor::CChronoEventEditor()
{

}

CChronoEventEditor::~CChronoEventEditor()
{
}

BEGIN_MESSAGE_MAP(CChronoEventEditor, CScrollView)
	//{{AFX_MSG_MAP(CChronoEventEditor)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()

	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFontMgr gMyFontManager;

/////////////////////////////////////////////////////////////////////////////
// CChronoEventEditor message handlers

//void CChronoEventEditor::OnDraw( CDC* pDC )

void CChronoEventEditor::NumerateEventList(EventVector* m_pEventList, int& num)
{
	for(EventVector::iterator i = m_pEventList->begin(); i!= m_pEventList->end(); i++)
	{
		if(!(*i)->m_Anim.m_bDestroyed)
		{
			if((*i)->m_type == 0 || (*i)->m_type == 4 )
			{
				(*i)->m_number = num ++;
			}
			NumerateEventList(&(*i)->m_EventList, num);
		}
	}
}



int CChronoEventEditor::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_leftDown = false;
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;


	m_FocusCondition = 0;
	m_ClickedSelectedCondition = false;
	m_FocusAction = 0;
	m_ClickedSelectedAction = false;
	m_FocusEvent = 0;
	m_ClickedSelectedEvent = false;

	
	m_Drag.IsDragging = false;
	m_Drag.dragtype = -1;


	m_CtrlDown = false;
	m_ShiftDown = false;

	//LEAK HERE!  THIS IS NOT DELETED!
	m_pDDMgr = new CDragDropMgr;
	m_pDDMgr->MakeDropTarget((CScrollView*)this);	
	
	m_pDDMgr->AddFormat("Construct Events");
	m_pDDMgr->AddFormat("Construct Conditions");
	m_pDDMgr->AddFormat("Construct Actions");
	m_pDDMgr->AddFormat("Construct Object");

	MyFont.CreateFont(14,0,0,0,0,0,0,0,0,0,0,0,0,"Arial");
	SetTimer(0,20,0);
	m_Ellapsed = 0;
	
	//m_pHeader->m_Split = 200;
	//m_pHeader->m_isDrag = false;


	return 0;
}



BOOL CChronoEventEditor::OnEraseBkgnd(CDC* pDC) 
{
	return 0;
}

bool CChronoEventEditor::Animate(CEditorEvent& Event)
{
	// Animate the conditions
	for(int c = 0; c < Event.m_Conditions.size(); c ++)
	{
		if(Event.m_Conditions[c]->m_Anim.m_bDestroyed)
		{
			
			Event.m_Conditions[c]->m_Anim.Animate(m_Ellapsed);
			Invalidate();
			if(Event.m_Conditions[c]->m_Anim.Space <= 0)
			{
				delete Event.m_Conditions[c];
				Event.m_Conditions.erase(Event.m_Conditions.begin() + c);
				c --;
			}
		}

	}

	// Animate the actions
	for(int a = 0; a < Event.m_Actions.size(); a ++)
	{
		if(Event.m_Actions[a]->m_Anim.m_bDestroyed)
		{
						
			Event.m_Actions[a]->m_Anim.Animate(m_Ellapsed);
			Invalidate();
			if(Event.m_Actions[a]->m_Anim.Space <= 0)
			{
				delete Event.m_Actions[a];
				Event.m_Actions.erase(Event.m_Actions.begin() + a);
				a --;
			}
		}

	}	

	// Animate the sub events
	for(int e = 0; e < Event.m_EventList.size(); e ++)
	{
		if(Animate(*Event.m_EventList[e]))
		{
			Invalidate();
			delete Event.m_EventList[e];
			Event.m_EventList.erase(Event.m_EventList.begin() + e);
			e --;

		}
	}


	// Now check the current event!
	if(Event.m_Anim.m_bDestroyed)
	{
		Invalidate();
		Event.m_Anim.Animate(m_Ellapsed);
		if(Event.m_Anim.Space <= 0)
			return true;
	}
	return false;	
}

void CChronoEventEditor::OnNextBookmark()
{

}

void CChronoEventEditor::OnPreviousBookmark()
{

}



////////////////
/*
 Task:
We need to find the first visible event




 */

void CChronoEventEditor::OnTimer(UINT nIDEvent) 
{
	if (m_pEventList == NULL)
		return;

	// TODO: Add your message handler code here and/or call default
	m_Ellapsed = clock();

	// If an animation is playing, we invalidate the screen
	for(int e = 0; e < (*m_pEventList).size(); e ++)
	{
		if(Animate(*(*m_pEventList)[e]))
		{
			delete (*m_pEventList)[e];
			m_pEventList->erase(m_pEventList->begin() + e);
			Invalidate();
			e --;
		}
	}	


	CScrollView::OnTimer(nIDEvent);
}


void CEditorHeader::Render(CRect &Pos, CDC& dc, CChronoEventEditor& Ed)
{
	//return; 

	// no longer render the header 

	COLORREF ObjectTabOutline = OBJECT_TAB_OUTLINE;
	COLORREF ObjectTabBG = OBJECT_TAB_BG;
	COLORREF ObjectOutline = OBJECT_OUTLINE;
	COLORREF ObjectBG = OBJECT_BG;

	dc.FillSolidRect(Pos,HEADERBG);
	dc.FillSolidRect(Pos.left,Pos.bottom-3,Pos.Width(),3,HEADEROUTLINE);
	dc.FillSolidRect(Pos.left,Pos.bottom-2,Pos.Width(),1,HEADERBG2);

	CRect Region(Ed.m_pHeader->m_Split+3+2 + Pos.left,Pos.bottom - 25,Ed.m_pHeader->m_Split+3+2 + Pos.left,Pos.bottom - 3);

	
	//dc.SetViewportOrg(0-Ed.m_ScrollX,0);
	
	CRgn Rgn;
	CMemDC* dc2 = (CMemDC*)&dc;
	Rgn.CreateRectRgn(Ed.m_pHeader->m_Split - dc2->m_rect.left,0,dc2->m_rect.right,dc2->m_rect.bottom);
	dc.SelectClipRgn(&Rgn);
	
	for(int a = 0; a < this->m_Objects.size() ; a ++ )
	{
		// Draw the tab bits for the objects
		for(int b = 0; b < 7; b ++ )
			dc.FillSolidRect(Region.left-5,Region.top+6+b*2,3,1,g_PaintManager->GetColor( COLOR_BTNSHADOW ));
	
		Region.right = Region.left + m_Objects[a].width;
		dc.FillSolidRect(Region.left+2,Region.top,Region.Width()-4,1,ObjectTabOutline);
		dc.FillSolidRect(Region.left+1,Region.top+1,Region.Width()-2,2,ObjectTabBG);
		dc.SetPixel(Region.left+1,Region.top+1,ObjectTabOutline);
		dc.SetPixel(Region.left+Region.Width() -2,Region.top+1,ObjectTabOutline);
		dc.SetPixel(Region.left,Region.top+2,ObjectTabOutline);
		dc.SetPixel(Region.left+Region.Width()-1,Region.top+2,ObjectTabOutline);
		dc.FillSolidRect(Region.left,Region.top+3,Region.Width(),Region.Height()-3,ObjectOutline);
		dc.FillSolidRect(Region.left + 1, Region.top + 3, Region.Width() - 2,Region.Height()-2,ObjectBG);

		// Draw icon
		m_Objects[a].m_rect = Region;
		//m_Objects[a].m_rect.OffsetRect(-Ed.m_ScrollX,0);

		// Work out mid-point
		int midPoint = Region.left + (Region.Width() / 2) - 8;

		HDC hdcScreenCompat = CreateCompatibleDC(dc.m_hDC);
		int iconid = 0;
		if(m_Objects[a].family)
		{
			((EventSheetEditor*)&Ed)->m_familyhbitmaps.Lookup(m_Objects[a].oid, iconid);
		}
		else
		{
			((EventSheetEditor*)&Ed)->m_objecthbitmaps.Lookup(m_Objects[a].oid, iconid);
		}
		HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcScreenCompat, Ed.GetIcon(iconid));
		BitBlt(dc.m_hDC, midPoint, Region.top + 4, 16, 16, hdcScreenCompat, 0, 0, SRCCOPY);
		SelectObject(hdcScreenCompat, hbmpOld);
		DeleteDC(hdcScreenCompat);
		Region.OffsetRect(m_Objects[a].width,0);
		Region.OffsetRect(7,0);

	}
	for(int b = 0; b < 7; b ++ )
		dc.FillSolidRect(Region.left-5,Region.top+6+b*2,3,1,g_PaintManager->GetColor(COLOR_BTNSHADOW));
	dc.SetViewportOrg(0,0);	

	Rgn.DeleteObject();
	Rgn.CreateRectRgn(0,0,dc2->m_rect.Width(),dc2->m_rect.Height());
	dc.SelectClipRgn(&Rgn);

	m_rect = Pos;
}

void CChronoEventEditor::ResetScrollBar()
{
	
    // Flag to avoid a call from OnSize while resetting the scrollbar
/*	m_bScrollBarMessage = TRUE;

	CRect rFrame;

	GetClientRect( rFrame );

 
	SCROLLINFO    si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_RANGE;
	si.nPage = rFrame.Height();
	si.nMin = 0 ;

	SetScrollInfo(SB_VERT, &si);
        
	EnableScrollBarCtrl( SB_VERT, TRUE );
    

	m_bScrollBarMessage = FALSE;*/
}


void CChronoEventEditor::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
//
//    CRect rFrame;
//
//    GetClientRect( rFrame );
//
//    switch( nSBCode )
//    {
//   /*     case SB_LINEUP:
//            iScrollBarPos = max( iScrollBarPos - 1, 0 );
//        break;
//
//        case SB_LINEDOWN:
//            iScrollBarPos = min( iScrollBarPos + 1, 
//                                 GetScrollLimit( SB_VERT ) );
//        break;
//
//        case SB_PAGEUP:
//            iScrollBarPos = max( iScrollBarPos - rFrame.Height(), 0 );
//        break;
//
//        case SB_PAGEDOWN:
//            iScrollBarPos = min( iScrollBarPos + rFrame.Height(), 
//                                 GetScrollLimit( SB_VERT ) );
//        break;
//*/
//        case SB_THUMBTRACK:
//        case SB_THUMBPOSITION:
//        {
//            SCROLLINFO si;
//
//            ZeroMemory( &si, sizeof(SCROLLINFO) );
//
//            si.cbSize = sizeof(SCROLLINFO);
//            si.fMask  = SIF_TRACKPOS;
//
//            if( GetScrollInfo( SB_HORZ, &si, SIF_TRACKPOS ) )
//                iScrollBarPos = si.nTrackPos;
//            else
//                iScrollBarPos = (UINT)nPos;
//            break;
//        }
//    }        
//
//    SetScrollPos( SB_HORZ, iScrollBarPos );
//
//	m_ScrollX = iScrollBarPos;

	SetRedraw(false);
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
	ScrollPos.x = CScrollView::GetScrollPos(0);
	SetRedraw(true);

    Invalidate();





}


BOOL CChronoEventEditor::OnScroll(UINT nScrollCode, UINT nPos, BOOL
                                bDoScroll)
{
      SCROLLINFO info;
      info.cbSize = sizeof(SCROLLINFO);
      info.fMask = SIF_TRACKPOS;

      if (LOBYTE(nScrollCode) == SB_THUMBTRACK)
      {
         GetScrollInfo(SB_HORZ, &info);
         nPos = info.nTrackPos;
      }

      if (HIBYTE(nScrollCode) == SB_THUMBTRACK)
      {
         GetScrollInfo(SB_VERT, &info);
         nPos = info.nTrackPos;
      }

	  bool ret = CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);

	  ScrollPos = CScrollView::GetScrollPosition();
	  Invalidate();

      return ret;
}


void CChronoEventEditor::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{




  //    return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);












 /*   int iScrollBarPos = GetScrollPos( SB_VERT );

    CRect rFrame;

    GetClientRect( rFrame );

    switch( nSBCode )
    {
        case SB_LINEUP:
            iScrollBarPos = max( iScrollBarPos - 1, 0 );
        break;

        case SB_LINEDOWN:
            iScrollBarPos = min( iScrollBarPos + 1, 
                                 GetScrollLimit( SB_VERT ) );
        break;

        case SB_PAGEUP:
            iScrollBarPos = max( iScrollBarPos - rFrame.Height(), 0 );
        break;

        case SB_PAGEDOWN:
            iScrollBarPos = min( iScrollBarPos + rFrame.Height(), 
                                 GetScrollLimit( SB_VERT ) );
        break;

        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
        {
            SCROLLINFO si;

            ZeroMemory( &si, sizeof(SCROLLINFO) );

            si.cbSize = sizeof(SCROLLINFO);
            si.fMask  = SIF_TRACKPOS;

            if( GetScrollInfo( SB_VERT, &si, SIF_TRACKPOS ) )
                iScrollBarPos = si.nTrackPos;
            else
                iScrollBarPos = (UINT)nPos;
            break;
        }
    }        

    SetScrollPos( SB_VERT, iScrollBarPos );

	m_ScrollY = iScrollBarPos;

    Invalidate();*/


	SetRedraw(false);
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
	ScrollPos.y = CScrollView::GetScrollPos(1);
	SetRedraw(true);
	Invalidate();
}
BOOL CChronoEventEditor::OnMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
	bool ret = CScrollView::OnMouseWheel(fFlags, zDelta, point);
	int newy = CScrollView::GetScrollPos(1);
	//if(newy == ScrollPos.y)
	//	return FALSE;
	//else
		ScrollPos.y = newy;
	Invalidate();
	return ret;
}

void CChronoEventEditor::OnSize(UINT nType, int cx, int cy) 
{
	UpdateScrollbars(true);
	CScrollView::OnSize(nType, cx, cy);
}

DROPEFFECT CChronoEventEditor::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	m_InsertAfter.Hide();
	m_InsertBefore.Hide();
	m_InsertSub.Hide();
	m_ActionTooltip.Hide();
    m_pDDMgr->OnDragEnter(pDataObject);
	m_Drag.IsDragging = true;
    return this->OnDragOver(pDataObject,dwKeyState,point);
}

void CChronoEventEditor::OnDragLeave() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Drag.IsDragging = false;
	m_Drag.dragtype = -1;
	m_ClickedSelectedAction = false;
	m_ClickedSelectedCondition = false;
	m_ClickedSelectedEvent = false;
}




EventVector* FindEventListEventExistsIn(CEditorEvent* Event, EventVector* pEventList,int& index)
{
	EventVector* tempList;
	for(int a = 0; a < pEventList->size(); a++)
	{
		if((*pEventList)[a] == Event) 
		{
			index = a;
			return pEventList;
		}
		tempList = FindEventListEventExistsIn(Event, &(*pEventList)[a]->m_EventList,index);
		if(tempList!= 0)
			return tempList;

	}
	
	return 0;


}




CEditorEvent* FindEventConditionExistsIn(CEditorCondition* Condition, EventVector* pEventList,int& index)
{
	CEditorEvent* tempEvent;
	for(int a = 0; a < pEventList->size(); a++)
	{
		for(int c = 0; c < (*pEventList)[a]->m_Conditions.size(); c ++ )
		{
			
			if(((*pEventList)[a]->m_Conditions[c]) == Condition) 
			{
				index = c;
				return ((*pEventList)[a]);
			}
		}
		tempEvent = FindEventConditionExistsIn(Condition, &(*pEventList)[a]->m_EventList,index);
		if(tempEvent!= 0)
			return tempEvent;
	}
	return 0;
}

CEditorEvent* FindEventActionExistsIn(CEditorAction* Action, EventVector* pEventList,int& index)
{
	CEditorEvent* tempEvent;
	for(int a = 0; a < pEventList->size(); a++)
	{
		for(int c = 0; c < (*pEventList)[a]->m_Actions.size(); c ++ )
		{
			
			if(((*pEventList)[a]->m_Actions[c]) == Action) 
			{
				index = c;
				return ((*pEventList)[a]);
			}
		}
		tempEvent = FindEventActionExistsIn(Action, &(*pEventList)[a]->m_EventList,index);
		if(tempEvent!= 0)
			return tempEvent;
	}
	return 0;
}






void CEditorHeader::Serialize(CArchive& ar)
{
	if(ar.IsLoading())
	{
		ar >> m_Split;
		int size = m_Objects.size();
		ar >> size;
		for(int a = 0; a < size; a ++)
		{
			CEditorObject obj;
			obj.Serialize(ar);
			m_Objects.push_back(obj);
		}


	}
	else
	{
		ar << m_Split;
		int size = m_Objects.size();
		ar << size;
		for(int a = 0; a < size; a ++)
		{
			m_Objects.at(a).Serialize(ar);
			//m_Objects.push_back(obj);
		}



	}

}

void CEditorHeader::OnLeftClick(CPoint pt)
{
	OnMouseMove(pt);

	CPoint pt2 = pt;
	pt2.y = m_rect.top;

	if(	CRect(m_Split-2,m_rect.top,m_Split+5,m_rect.bottom).PtInRect(pt2))
	{
		m_isDrag = true;
		m_DragNumber = 0;
	}

	int pos = m_Split;

	for(int a = 0; a < m_Objects.size(); a ++ )
	{
		pos += m_Objects[a].width + 7;
		if(	CRect(m_rect.left+pos-2,m_rect.top,m_rect.left+pos+5,m_rect.bottom).PtInRect(pt))
		{
			m_isDrag = true;
			m_DragNumber = a+1;
		}
	}


// we need to see if we are trying to resize a column
}

void CEditorHeader::OnLeftUp(CPoint pt)
{
	OnMouseMove(pt);
	if(m_isDrag)
		m_isDrag = false;
}

void CEditorHeader::OnMouseMove(CPoint pt)
{
	
	int pos = m_Split;


	bool cursorsplit = false;

	if(m_isDrag)
	{
		cursorsplit = true; 

		if(m_DragNumber == 0)
		{
			m_Split = max(100,pt.x - m_rect.left-1);
		}

		else
		{
			m_Objects[m_DragNumber-1].width = max(24,pt.x - m_Objects[m_DragNumber-1].m_rect.left-3);
		}
	}

	else
	{
		CPoint p1 = pt;
		p1.y = m_rect.top;
		if(	CRect(m_Split-2,m_rect.top,m_Split+5,m_rect.bottom).PtInRect(p1))
		{
			cursorsplit = true;
		}

		for(int a = 0; a < m_Objects.size(); a ++ )
		{
			pos += m_Objects[a].width + 7;
			if(	CRect(m_rect.left+pos-2,m_rect.top,m_rect.left+pos+5,m_rect.bottom).PtInRect(pt))
			{
				// resize handle
				cursorsplit = true;
				
			}
		}
	}

	if(cursorsplit)
		SetCursor(LoadCursor(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_EXT_RESIZE_H2)));
	else
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));


}

void CChronoEventEditor::LockScrollUpdate()
{
	scrolllock ++;
	if(GetScrollBarCtrl(0))
	GetScrollBarCtrl(0)->SetRedraw(false);
	if(GetScrollBarCtrl(1))
	GetScrollBarCtrl(1)->SetRedraw(false);
}
void CChronoEventEditor::UnlockScrollUpdate()
{
	scrolllock --;
	if(scrolllock == 0)
	{
		this->EnableScrollBarCtrl(0,false);
		if(GetScrollBarCtrl(0))
		GetScrollBarCtrl(0)->SetRedraw(true);
			if(GetScrollBarCtrl(1))
		GetScrollBarCtrl(1)->SetRedraw(true);
	//	GetScrollBarCtrl(0)->Invalidate();
		//GetScrollBarCtrl(1)->Invalidate();
			if(GetScrollBarCtrl(0))
		GetScrollBarCtrl(0)->RedrawWindow();
				if(GetScrollBarCtrl(1))
		GetScrollBarCtrl(1)->RedrawWindow();

	}
}




// TODO
CEditorEvent* CChronoEventEditor::FindNextVisibleEvent(CEditorEvent *pEvent)
{
	return 0;
}

CEditorEvent* FindVisibleEventBelowEvent(CEditorEvent* pEvent, bool found, SelectedEventVector SelectedEvents)
{
	return 0;
}

CEditorObject* CEditorHeader::GetObjectAtX(long x)
{
	for(int c = 0; c < m_Objects.size(); c++)
	{
		if(m_Objects[c].m_rect.left <= x &&
			m_Objects[c].m_rect.right >= x	) 
		{
			return &m_Objects[c];
		}
	}

	return NULL;
}


///////////////////////////////////////
//		Drag Events



 void CEditorDragEvents::Serialize( CArchive& ar ) 
{
	CString ExpectedName = "CEditorDragEvents";
	int     Version      = 1;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return;
	if( ar.IsLoading() )
	{

		pEd->object_id_translator.begin();
		pEd->object_id_translator.serialize_objects(ar, pEd->application);
					
		// Serialize child events
		unsigned int esize;
		ar >> esize;

		for(int e = 0; e < esize; e ++ )
		{
			CEditorEvent* Evt = new CEditorEvent;
			Evt->Init();
			Evt->Serialize(ar, false, pEd);
			if(m_pEventListPart->size() == 0)
				m_pEventListPart->push_back(Evt);
			else
				m_pEventListPart->insert(index+m_pEventListPart->begin()+ (GoingUp?0:1)+e ,Evt);
			
		}
		pEd->object_id_translator.end();
	}
	else
	{
		// I was unable to find a way to serialize a mem file so I ended up serializing twice. Once the the memfile, and then to the CArchieve
		
		// Step 1. Find out what objects we use by serializing
		pEd->object_id_translator.begin();

		CMemFile mf;
		CArchive delayed(&mf, CArchive::store);

		// Serialize child events with our mem file
		for(int e = 0; e < m_pSelectedEvents->size(); e ++ )
			(*m_pSelectedEvents)[e]->Serialize(delayed, false, pEd);

		delayed.Close();

		pEd->object_id_translator.serialize_objects(ar, pEd->application);

		// Serialize child events
		ar << m_pSelectedEvents->size();
		for(int e = 0; e < m_pSelectedEvents->size(); e ++ )
			(*m_pSelectedEvents)[e]->Serialize(ar, false, pEd);

		// Step 2. 
		pEd->object_id_translator.end();
	}
}


///////////////////////////////////////
//		Drag Conditions

 void CEditorDragConditions::Serialize( CArchive& ar ) 
{
	CString ExpectedName = "CEditorDragConditions";
	int     Version      = 1;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return;
		if( ar.IsLoading() )
		{
			
			// Serialize child events
			unsigned int esize;
			ar >> esize;
			bool dopush = (m_pConditions->size() == 0);
			for(int e = 0; e < esize; e ++ )
			{
				CEditorCondition* Cnd = new CEditorCondition;
				Cnd->Init();
				Cnd->Serialize(ar, pEd);
				if(dopush)
					m_pConditions->push_back(Cnd);
				else
					m_pConditions->insert(index+m_pConditions->begin()+ (GoingUp?0:1)+e ,Cnd);
			}
		}
		else
		{
			// Serialize child events
			ar << m_pSelectedConditions->size();
			for(int e = 0; e < m_pSelectedConditions->size(); e ++ )
				(*m_pSelectedConditions)[e]->Serialize(ar, pEd);
		}
}

///////////////////////////////////////
//		Drag Actions


 void CEditorDragActions::Serialize( CArchive& ar ) 
{
	CString ExpectedName = "CEditorDragActions";
	int     Version      = 1;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return;
		if( ar.IsLoading() )
		{
			
			// Serialize child events
			unsigned int esize;
			ar >> esize;
			bool do_push_back = m_pActions->size() == 0;
			for(int e = 0; e < esize; e ++ )
			{
				CEditorAction* Act = new CEditorAction;
				Act->Init();
				Act->Serialize(ar, pEd);
				if(do_push_back)
					m_pActions->push_back(Act);
				else
					m_pActions->insert(index+m_pActions->begin()+ (GoingUp?0:1)+e ,Act);
			}
		}
		else
		{
			// Serialize child events
			ar << m_pSelectedActions->size();
			for(int e = 0; e < m_pSelectedActions->size(); e ++ )
				(*m_pSelectedActions)[e]->Serialize(ar, pEd);
		}
}

///////////////////
//  Drag Objects

void CEditorDragObject::Serialize( CArchive& ar ) 
{
	CString ExpectedName = "CEditorDragObject";
	int     Version      = 1;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return;
		if( ar.IsLoading() )
		{
			ar >> ObjectIdentifier;
		}
		else
		{
			ar << ObjectIdentifier;
		}
 }


 
BOOL& GetMapItemFunction_bool(  ChronoBoolMap *map, BOOL& def, CChronoEventEditor* ed)
{
		if( ed == NULL)														
			return def;										
		else																
		{																	
			ChronoBoolMap::iterator i = map->find(ed);
													
			if( i != map->end())
			{
				def = *(*i).second;
				return *(*i).second;	
			}
			else															
			{			
				BOOL* item = new BOOL;
				*item = def;
				(*map)[ed] = item;														
				return *item;													
			}																		
		}				
}
int& GetMapItemFunction_int( ChronoIntMap *map, int& def, CChronoEventEditor* ed)
{
	if( ed == NULL)														
			return def;										
		else																
		{																	
			ChronoIntMap::iterator i = map->find(ed);
													
			if( i != map->end())	
			{
				def = *(*i).second;
				return *(*i).second;
			}
			else															
			{			
				int* item = new int;
				*item = def;
				(*map)[ed] = item;														
				return *item;													
			}																		
		}					
}
CRect& GetMapItemFunction_CRect( ChronoRectMap *map, CRect& def, CChronoEventEditor* ed)
{
	if( ed == NULL)														
			return def;										
		else																
		{																	
			ChronoRectMap::iterator i = map->find(ed);
													
			if( i != map->end())
			{
				def = *(*i).second;
				return *(*i).second;
			}
			else															
			{			
				CRect* item = new CRect;
				*item = def;
				(*map)[ed] = item;														
				return *item;													
			}																		
		}					
}



void CChronoEventEditor::InlineEditCondition(CEditorCondition& condition,CPoint pt)
{

}
void CChronoEventEditor::InlineEditAction(CEditorAction& action, CPoint pt)
{

}
bool CChronoEventEditor::KillFocusOnInlineEditor(void)
{
return 0;
}






void CChronoEventEditor::UpdateScrollbars(bool forced)
{
	// Scrollbar
	if(!m_pHeader)
		return;

	//if (m_pHeader->m_Objects.empty())
	//	return;

	int left = 0;//m_pHeader->m_Objects.front().m_rect.left;
	int right = 0;//m_pHeader->m_Objects.back().m_rect.right;

	//int min,  max;
	bool changesize = false;
	{
		CRect rec;
		GetClientRect(rec);

		CSize Desired(CSize(right - left + m_pHeader->m_Split+8, m_tmpScrollCount+rec.Height()));

		CRect client;
		GetClientRect(client);
		CSize Range = Desired ;//- client.BottomRight();
		if(Range.cx < 0) Range.cx = 0;
		if(Range.cy < 0) Range.cy = 0;

		CSize actual;
		
		actual = GetTotalSize();
		//actual = actual - client.BottomRight();
		if(actual.cx < 0) actual.cx = 0;
		if(actual.cy < 0) actual.cy = 0;


		if(forced || (actual.cx != Range.cx || actual.cy != Range.cy))
		{
			LockScrollUpdate();
			SetScrollSizes(MM_TEXT, Desired);
			CScrollView::SetScrollPos(0,ScrollPos.x);
			CScrollView::SetScrollPos(1,ScrollPos.y);


			UnlockScrollUpdate();

		}
	}

}


void CChronoEventEditor::OnMButtonDown(UINT nFlags, CPoint point)
{

}

void CChronoEventEditor::Insert_ClickAfter()
{

}
void CChronoEventEditor::Insert_ClickBefore()
{

}