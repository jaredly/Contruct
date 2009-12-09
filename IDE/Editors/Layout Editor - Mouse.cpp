// Layout Editor - Mouse.cpp : implementation of the CLayoutEditor class
//

// File includes
#include "StdAfx.h"
#include "..\Construct.h"
#include "..\Construct Doc.h"
#include "..\Bars\LayerBar.h"
#include "LayoutEditor.h"
#include "..\ChildFrame.h"
#include "..\Dialogs\Insert Object Dlg.h"
#include "..\Dialogs\ParametersDlg.h"
#include "..\MainFrm.h"
#include "..\Structure.h"

extern CString gInsert;
extern int gInsertAuto;

extern CLayerBar* g_pLayerBar;
extern BOOL g_bPaste;
extern BOOL g_bPasteClone;
extern long g_addToGroup;   

extern PropertiesBar* g_PropertyBar;
extern CLayoutEditor* g_pLayoutView;
BOOL g_bIsDragging = FALSE;
extern CMainFrame* pMainWnd;
extern bool g_SuspendFrameEditor;

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Mouse Triggers ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////

BOOL CLayoutEditor::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	ScreenToClient(&pt);

	if (!(nFlags & MK_CONTROL)) return CScrollView::OnMouseWheel(nFlags, zDelta, pt);

	LockScrollUpdate();

	CRect Clientrect;
	GetClientRect(Clientrect);
	// TODO: Add your message handler code here and/or call default
	float oldzoom = m_Zoom;
	m_zoom_val += zDelta/4;

	if(m_zoom_val >= 0 )
		m_Zoom = (m_zoom_val/120.0)+1;
	if(m_zoom_val < 0 ) 
		m_Zoom = 1.0/(1-(m_zoom_val/120.0));

	CPoint centerScrollPosition = GetScrollPosition();
	CRect ClientRect(0,0,0,0);
	GetClientRect(&ClientRect);
	centerScrollPosition.x += ClientRect.right/2;
	centerScrollPosition.y += ClientRect.bottom/2;

	centerScrollPosition.x += (pt.x  -ClientRect.right/2)/2;
	centerScrollPosition.y += (pt.y  -ClientRect.bottom/2)/2;

	int newXScrollPosition = centerScrollPosition.x * m_Zoom / oldzoom;
	int newYScrollPosition = centerScrollPosition.y * m_Zoom / oldzoom;

	newXScrollPosition -= ClientRect.right/2;
	newYScrollPosition -= ClientRect.bottom/2;

	SetRedraw(false);

	UpdateScrollbars();
	ScrollToPosition(CPoint(newXScrollPosition,newYScrollPosition));
	UpdateScrollbars();
	SetRedraw(true);


	UnlockScrollUpdate();
	RedrawWindow();


	//	pos.x = CScrollView::GetScrollPosition().x;
	//	pos.y = CScrollView::GetScrollPosition().y;

	Invalidate();
	return TRUE;// CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CLayoutEditor::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!layout)
		return;

	// make sure layers shown are for this layout
	g_pLayerBar->m_layerDlg.layout_editor = this;
	g_pLayerBar->m_layerDlg.RefreshLayers();

	layout->m_LayoutEditor = this;	

	if(GetKeyState(VK_SPACE) >> 4)
		return;

	if(InputLocked())
	{
		layout->Message(WM_LBUTTONDOWN);
		return;
	}

	if (gInsert != "") 
	{ 
		// are we inserting an object?
		CreateObject(gInsert, point);
		g_SuspendFrameEditor = false;

		gInsert = "";

		CChildFrame* pCF = (CChildFrame*)GetParentFrame();
		pCF->object_bar.Refresh();

		g_PropertyBar->Update(this, TYPE_OBJECT, &m_sel, layout, &layout->objects, application); // show object props
		layout->SetChanged(true);

		return;
	}

	if (g_bPaste) 
	{
		Paste(point);
		g_bPaste = FALSE;
		g_PropertyBar->Update(this, TYPE_OBJECT, &m_sel, layout, &layout->objects, application); // show object props

		CChildFrame* pCF = (CChildFrame*)GetParentFrame();
		pCF->object_bar.Refresh();

		layout->SetChanged(true);

		return;
	}

	if (g_bPasteClone) 
	{
		Paste(point, TRUE);
		g_bPasteClone = FALSE;
		g_PropertyBar->Update(this, TYPE_OBJECT, &m_sel, layout, &layout->objects, application); // show object props

		CChildFrame* pCF = (CChildFrame*)GetParentFrame();
		pCF->object_bar.Refresh();

		layout->SetChanged(true);

		return;
	}

	point -= CPoint(FRAME_MARGIN_W*m_Zoom, FRAME_MARGIN_H*m_Zoom); // offset for frame border
	point += GetScrollPosition(); // offset for scroll position
	CObj* o;
	CObjMap* objMap = &layout->objects;
	POSITION pos, pos2;

	if (g_addToGroup >= 0)
	{
		long groupToAdd = -1;
		pos2 = layout->layers.GetHeadPosition();
		for (int j = 0; j < layout->layers.GetCount(); j++) 
		{
			CLayer *lyr = layout->layers.GetNext(pos2);
			if (lyr->m_layerType != LAYER_NONFRAME && ISLAYERVISIBLE(lyr->m_state) && !ISLAYERLOCKED(lyr->m_state)) 
			{
				pos = lyr->m_zOrder.GetTailPosition();
				for (int i = 0; i < lyr->m_zOrder.GetCount(); i++) 
				{
					long id = lyr->m_zOrder.GetPrev(pos);
					objMap->Lookup(id, o);

					if (o->GetVisible() && !o->GetLocked() && o->m_bIsGroup) 
					{
						if (o->GetObjectRect(this).PtInRect(point)) 
						{
							groupToAdd = o->GetInstanceID(); // we've got an object
																// (finally! - tigs)
							break;
						}
					}
				}
			}
		}

		if (groupToAdd >= 0) {
			pos = m_sel.GetHeadPosition();
			int j = 0;
			for (int i = 0; i < m_sel.GetCount(); i++) {
				o = GetObject(m_sel.GetNext(pos));
				if (o->m_bInGroup == FALSE) { // not in another group

					// START UNDO INFO //
					CAction *action = new CAction();
					action->m_type = ACTION_GROUPREMOVE;
					if (j > 0)
						action->m_bGroup = FALSE;
					CArchive *ar = action->Prepare1();
					*ar << groupToAdd;
					long id = o->GetInstanceID();
					*ar << id;
					action->Prepare2();
					m_undo->AddAction(action);
					// END UNDO INFO //

					j++;

					AddToGroup(o, GetObject(groupToAdd));
				}
			}

			m_sel.RemoveAll();
			m_sel.AddTail(groupToAdd);
		}

		g_addToGroup = -1;
		return;
	}

	long cursel = -1;
	m_oldPt = point;
	m_curPt = point;

	// Clicked on a sizing handle
	if (!m_sel.IsEmpty() && m_sizeType > 0) 
	{ 
		// Are any valid objects selected, are we over a sizing handle?
		if (objMap->Lookup(m_objToSize, o)) 
		{
			SetCursor(LoadCursor(NULL, m_sizeCur));
			m_bSizingObject = TRUE; // begin sizing the object
			m_sel.RemoveAll();
			m_sel.AddTail(m_objToSize);
			m_oldRatio = ((double)o->GetH()) / ((double)o->GetW());
			m_centerPt = CPoint(o->GetW()/2 + o->GetX(), o->GetH()/2 + o->GetY());
			m_centerPt.x *= m_Zoom;
			m_centerPt.y *= m_Zoom;

			SetCapture();
			UpdateSelection();
			return;
		}
	}

	int ClickedOnSelectedID = -1;
	POSITION selpos = m_sel.GetHeadPosition();
	while(selpos) 
	{
		long object = m_sel.GetNext(selpos);
		CObj* temp;
		if (layout->objects.Lookup(object, temp))
		{
			if (temp->GetObjectRect(this).PtInRect(point))
			{
				ClickedOnSelectedID = object;
			}
		}
	}

	// This is used to allow for shift clicks, where we select the object under the selected one
	bool OkayToSelect = true;
	if(GetKeyState(VK_RETURN)>>4 && ClickedOnSelectedID != -1)
		OkayToSelect = false;

	// Clicked on object or blank space
	pos2 = layout->layers.GetHeadPosition();
	for (int j = 0; j < layout->layers.GetCount(); j++) 
	{
		CLayer *lyr = layout->layers.GetNext(pos2);
		if (lyr->m_layerType != LAYER_NONFRAME && ISLAYERVISIBLE(lyr->m_state) && !ISLAYERLOCKED(lyr->m_state)) 
		{
			pos = lyr->m_zOrder.GetTailPosition();
			for (int i = 0; i < lyr->m_zOrder.GetCount(); i++) 
			{
				long id = lyr->m_zOrder.GetPrev(pos);
				objMap->Lookup(id, o);

				if (o->GetVisible() && !o->GetLocked()) 
				{
					if (o->GetObjectRect(this).PtInRect(point) && OkayToSelect)
					{
						if (o->m_bIsGroup && o->m_bGroupOpen)
							cursel = CheckGroup(o, point);
						else
							cursel = o->GetInstanceID(); // we've got an object
						break;
					}
				}
				if(id == ClickedOnSelectedID)
					OkayToSelect = true;
			}
		}
	}

	if (cursel != -1) 
	{
		if (!(nFlags & MK_SHIFT) && !m_sel.Find(cursel))
		{
			m_sel.RemoveAll();
			m_sel.AddTail(cursel);
		}

		else if ((nFlags & MK_SHIFT) && m_sel.Find(cursel)) 
		{
			m_sel.RemoveAt(m_sel.Find(cursel));
		}

		else if ((nFlags & MK_SHIFT) && !m_sel.Find(cursel)) 
		{
			m_sel.AddTail(cursel);
		}
		UpdateSelection();
	}

	else 
	{
		if (nFlags & MK_SHIFT) 
		{
			//
		}

		else 
		{
			m_sel.RemoveAll();
		}
		UpdateSelection();
	}

	// Update property window
	if (m_sel.IsEmpty()) 
	{
		g_PropertyBar->Update(this, TYPE_LAYOUT, NULL, layout, NULL, application); // nothing is selected, show frame props

		// Clear animations bar too
		pMainWnd->animator.UpdateAnimations(NULL, NULL, NULL, -1);
	}

	else 
	{
		g_PropertyBar->Update(this, TYPE_OBJECT, &m_sel, layout, &layout->objects, application); // show object props

		if (m_sel.GetCount() == 1)
		{
			// Show animations
			// First get the animation root
			int iRoot = -1;
			
			POSITION Pos = m_sel.GetHeadPosition();
			CObj*    pObj = GetObject(m_sel.GetNext(Pos));

			CObjType* pType = pObj->GetObjectType(application);
			
			if(pType)
			{
				OINFO* oInfo = GetOINFO(pType->DLLIndex);

				if (oInfo->ETGetAnimationHandle) 
				{
					oInfo->ETGetAnimationHandle(pObj->editObject, iRoot);

					pMainWnd->animator.UpdateAnimations(application, layout, pType, iRoot);
				}
			}
		}
	}

	m_bMouseDown = TRUE;
	m_bSizingObject = FALSE;
	m_bDraggingRect = FALSE;
	m_clickedOn = cursel;
	RedrawWindow();
	ReleaseCapture();
}

long CLayoutEditor::CheckGroup(CObj *o2, CPoint point)
{
	POSITION pos = o2->m_groupObjects.GetTailPosition();
	CObj *o;
	for (int i = 0; i < o2->m_groupObjects.GetCount(); i++) {
		o = GetObject(o2->m_groupObjects.GetPrev(pos));
		if (o->m_bIsGroup && o->m_bGroupOpen)
			return CheckGroup(o, point);
		else
		{
			if (o->GetObjectRect(this).PtInRect(point)) 
			{
				return o->GetInstanceID(); // we've got an object
			}
		}
	}

	return o2->GetInstanceID();
}

void CLayoutEditor::OnLButtonUp(UINT nFlags, CPoint point) 
{
	layout->m_LayoutEditor = this;
	if (!layout)
		return;

	m_UpdatePreview = true;

	if(InputLocked())
	{
		layout->Message(WM_LBUTTONUP);
		return;
	}

	

	if (m_bDraggingRect)
		SelectItemsInBox(nFlags);
	else if (m_bSizingObject)
		ResizeObject();

	KillTimer(m_nTimerID);
	m_bDraggingRect = FALSE;
	m_bSizingObject = FALSE;
	m_bMouseDown = FALSE;
	g_bDuplicate = FALSE;
	m_bMovingObject = FALSE;
	
	ReleaseCapture();
	Invalidate();
	RedrawWindow();
}

void CLayoutEditor::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	layout->m_LayoutEditor = this;

	point -= CPoint(FRAME_MARGIN_W * m_Zoom, FRAME_MARGIN_H * m_Zoom);
	point += GetScrollPosition();

	if(InputLocked())
	{
		layout->Message(WM_LBUTTONDBLCLK);
		return;
	}

	CObjMap* objMap = &layout->objects;
	CObj* o;

	long cursel = 0;

	// Clicked on object or blank space
	POSITION pos2 = layout->layers.GetHeadPosition();
	for (int j = 0; j < layout->layers.GetCount(); j++) 
	{
		CLayer *lyr = layout->layers.GetNext(pos2);
		if (lyr->m_layerType != LAYER_NONFRAME && ISLAYERVISIBLE(lyr->m_state) && !ISLAYERLOCKED(lyr->m_state)) 
		{
			POSITION pos = lyr->m_zOrder.GetTailPosition();
			for (int i = 0; i < lyr->m_zOrder.GetCount(); i++) 
			{
				long id = lyr->m_zOrder.GetPrev(pos);
				objMap->Lookup(id, o);

				if (o->GetVisible() && !o->GetLocked()) 
				{
					if (o->GetObjectRect(this).PtInRect(point)) 
					{
						if (o->m_bIsGroup && o->m_bGroupOpen)
							cursel = CheckGroup(o, point);
						else
							cursel = o->GetInstanceID(); // we've got an object
						break;
					}
				}
			}
		}
	}

	if(cursel)
	{
		CObj* clicked = GetObject(cursel);
		CObjType* pType = clicked->GetObjectType(application);

		if (pType != NULL) {
			OINFO* info = GetOINFO(pType->DLLIndex);
			if(info->ETOnNotify)
				info->ETOnNotify(clicked->editObject, 1);
		}
	}

	else
	{
		CObjTypeMap* objTypeMap = &application->object_types;
	
		InsertObjectDialog dialog(*application);
		dialog.DoModal();

		if (gInsert != "")
		{
			if (gInsertAuto == 1)
			{
				m_AddedObject = gInsert;
				CObj* pObj = CreateObject(gInsert, CPoint(-10000, -10000));
				g_SuspendFrameEditor = false;
				m_bPanel = true;
				SetTimer(500, 2000, NULL);

				RedrawWindow();
				gInsert = "";
				gInsertAuto = 0;

				CChildFrame* pCF = (CChildFrame*)GetParentFrame();
				pCF->object_bar.Refresh();

				// Update the prop grid to show properties
				CObjList fakeSelection;
				long mfc_sucks = pObj->GetInstanceID();
				fakeSelection.AddTail(mfc_sucks);
				g_PropertyBar->Update(this, TYPE_OBJECT, &fakeSelection, layout, &layout->objects, application);

				layout->SetChanged(true);
			}
		}
	}
}

void CLayoutEditor::OnRButtonDown(UINT nFlags, CPoint point) 
{
	layout->m_LayoutEditor = this;

	// Inserting object
	if (gInsert != "") 
		gInsert = "";

	if (g_bPaste)
		g_bPaste = FALSE;
	if (g_bPasteClone)
		g_bPasteClone = FALSE;

	if(InputLocked())
	{
		layout->Message(WM_RBUTTONDOWN);
		return;
	}

	m_bMouseDown = FALSE;
	m_bSizingObject = FALSE;
	m_bDraggingRect = FALSE;
	ReleaseCapture();
	RedrawWindow();
}

LRESULT CLayoutEditor::OnExtMenuPrepareOneLevel( WPARAM wParam, LPARAM lParam )
{
    lParam; // unused parameter
    
    CExtPopupMenuWnd::MsgPrepareMenuData_t * pData =
        reinterpret_cast < CExtPopupMenuWnd::MsgPrepareMenuData_t * > ( wParam );
    ASSERT( pData != NULL );

	CExtPopupMenuWnd * popup = pData->m_pPopup;
    ASSERT( popup != NULL );
    
	// Insert movement items
	POSITION pos = m_sel.GetHeadPosition();

	if (m_sel.GetCount() == 1)
	{
		CObj*      o = GetObject(m_sel.GetHead());
		if (!o->m_bIsGroup)
		{
			CObjType* oT = o->GetObjectType(application);

			// Insert triggers
			INT nReplacePos = popup->ItemFindPosForCmdID(8989);
			if(!(nReplacePos < 0))
			{
				bool bAdded = false;
				int index = 1;

				int Count = oT->GetTableCount(1);

				for (int i = 0; i < Count; i++)
				{
					// Check if trigger
					ACESEntry2* pACE = oT->GetACESEntry(1, i);
					if (pACE->retrn & CND_TRIGGERED)
					{
						bAdded = true;
						CString Name;
						Name = pACE->aceDescription;
						Name.Replace("%o", "");
						Name.Replace("%0", "..."); Name.Replace("<0>", "...");
						Name.Replace("%1", "..."); Name.Replace("<1>", "...");
						Name.Replace("%2", "..."); Name.Replace("<2>", "...");
						Name.Replace("%3", "..."); Name.Replace("<3>", "...");
						Name.Replace("%4", "..."); Name.Replace("<4>", "...");
						Name.Replace("%5", "..."); Name.Replace("<5>", "...");

						popup->ItemInsertCommand(8989 + i, nReplacePos + index, Name, 0);
					}

					index++;
				}

				popup->ItemRemove(nReplacePos);
			}
		}
	}

    return 1L;
}

// THIS WORKS. IT WORKS.
void CLayoutEditor::OnAddEvent(UINT id)
{
	POSITION pos = m_sel.GetHeadPosition();

	CStringArray Names;

	CObj*		obj = GetObject(m_sel.GetNext(pos));
	CObjType*	oT = obj->GetObjectType(application);

	bool Found = false;

	int Count = oT->GetTableCount(1);
	int ItemWanted = id - 8989;
	if (ItemWanted >= Count) return;

	ACESEntry2* pACE = oT->GetACESEntry(1, ItemWanted);
	
	CString chosen = pACE->aceDescription;
	CParametersDlg parametersDialog;
	parametersDialog.application = application;
	parametersDialog.layout = layout;

	parametersDialog.objectMap = &application->object_types;
	parametersDialog.oid = oT->ObjectIdentifier;
	parametersDialog.aceID = ItemWanted;
	parametersDialog.m_pACEEntry = pACE;
	parametersDialog.m_ID = oT;
	parametersDialog.m_selfType = oT;
	parametersDialog.mid = -1;

	if (pACE->params.size() != 0)
		if (parametersDialog.DoModal() != IDOK)
			return;

	CEditorCondition* newCondition = new CEditorCondition;

	newCondition->Init();

	newCondition->oid = parametersDialog.oid;
	newCondition->m_Family = false;
	newCondition->m_Object = oT->GetName();
	newCondition->DLLIndex = oT->DLLIndex;
	newCondition->cndID = parametersDialog.aceID;
	newCondition->mid = parametersDialog.mid;
	newCondition->m_Script = pACE->aceName;
	
	CEditorEvent* new_event = new CEditorEvent;
	new_event->Init();
	new_event->m_Conditions.push_back(newCondition);
	layout->event_sheet->m_EventList.push_back(new_event);

	CChildFrame *pCF = (CChildFrame*)GetParentFrame();
	pCF->m_tabs.SelectionSet(1);
	LRESULT End = 0;

	pCF->Invalidate();
	pCF->m_tabs.Invalidate();
	Invalidate();
}

// Right click menu
void CLayoutEditor::OnRButtonUp(UINT nFlags, CPoint point) 
{
	layout->m_LayoutEditor = this;

	CPoint original = point;
	ClientToScreen(&point);

	if(InputLocked())
	{
		layout->Message(WM_RBUTTONUP);

		return;
	}

	CObj* o;
	CObjMap* objMap = &layout->objects;
	POSITION pos, pos2;

	CPoint u = original;
	u -= CPoint(FRAME_MARGIN_W*m_Zoom, FRAME_MARGIN_H*m_Zoom); // offset for frame border
	u += GetScrollPosition(); // offset for scroll position

	// Clicked on object or blank space
	bool ClickedOnSelectedItem = false;
	POSITION selpos = m_sel.GetHeadPosition();
	while(selpos) 
	{
		long object = m_sel.GetNext(selpos);
		CObj* temp;
		if (layout->objects.Lookup(object, temp))
		{
			if (temp->GetObjectRect(this).PtInRect(u))
			{
				ClickedOnSelectedItem = true;
			}
		}
	}

	// If we haven't clicked on a selected item, select the item under the mouse (if there is one)
	if(!ClickedOnSelectedItem)
	{
		pos2 = layout->layers.GetHeadPosition();
		for (int j = 0; j < layout->layers.GetCount(); j++) 
		{
			CLayer *lyr = layout->layers.GetNext(pos2);
			if (lyr->m_layerType != LAYER_NONFRAME && ISLAYERVISIBLE(lyr->m_state) && !ISLAYERLOCKED(lyr->m_state)) 
			{
				pos = lyr->m_zOrder.GetHeadPosition();
				for (int i = 0; i < lyr->m_zOrder.GetCount(); i++) 
				{
					long id = lyr->m_zOrder.GetNext(pos);
					objMap->Lookup(id, o);

					if (o->GetVisible() && !o->GetLocked()) 
					{
						if (o->GetObjectRect(this).PtInRect(u)) 
						{
							bool bDo = true;
							POSITION pos = m_sel.GetHeadPosition();
							for (int i = 0; i < m_sel.GetCount(); i++)
							{
								CObj* oa;
								oa = GetObject(m_sel.GetNext(pos));
								if (oa->GetInstanceID() == o->GetInstanceID())
								{
									bDo = false;
								}
							}

							if (bDo)
							{
								OINFO* info = GetOINFO(o->GetObjectType(application)->DLLIndex);

								m_sel.RemoveAll();

								if (!(info->ideFlags & OF_NODRAW))
								{
									long id = o->GetInstanceID();
									m_sel.AddTail(id);
								}
							}
						}
					}
				}
			}
		}
	}


	UpdateSelection();
	RedrawWindow();

	// Load popup context menu
	CExtPopupMenuWnd * popup =  new CExtPopupMenuWnd;
	popup->LoadMenu(m_hWnd, IDR_LAYOUT, true, false);

	popup->ItemGetInfo(popup->ItemFindPosForCmdID(IDFR_INSERT)).SetText(LE_INSERTOBJECT);

	popup->ItemGetInfo(popup->ItemFindPosForCmdID(IDFR_PASTECLONE)).SetText(LE_PASTECLONE);
	popup->ItemGetInfo(popup->ItemFindPosForCmdID(IDFR_OBJECTINFORMATION)).SetText(LE_OBJECTINFORMATION);
	popup->ItemGetInfo(popup->ItemFindPosForCmdID(IDFR_ARRAYPASTE)).SetText(LE_ARRAYPASTE);

	popup->ItemGetInfo(popup->ItemFindByText("1")).SetText(LE_ALIGN);
	popup->ItemGetInfo(popup->ItemFindByText("2")).SetText(LE_DISTRIBUTE);
	popup->ItemGetInfo(popup->ItemFindByText("5")).SetText(LE_HIDELOCK);
	popup->ItemGetInfo(popup->ItemFindByText("7")).SetText(LE_ORDER);
	popup->ItemGetInfo(popup->ItemFindByText("8")).SetText(LE_SIZE);
	popup->ItemGetInfo(popup->ItemFindPosForCmdID(IDFR_CENTERFRAME)).SetText(LE_CENTERFRAME);
	popup->ItemGetInfo(popup->ItemFindPosForCmdID(IDFR_CENTERFRAMEOVERSELECTION)).SetText(LE_CENTERFRAMEOVERSELECTION);

	if (m_sel.GetCount() != 1)
	{
		popup->ItemRemove(popup->ItemFindByText("New event"));
	}

	else
	{
		CObj*      o = GetObject(m_sel.GetHead());
		if (!o->m_bIsGroup)
		{
			CObjType* oT = o->GetObjectType(application);
			int Count = oT->GetTableCount(1);

			bool bFound = false;
			for (int i = 0; i < Count; i++)
			{
				// Check if trigger
				ACESEntry2* pACE = oT->GetACESEntry(1, i);
				if (pACE->retrn & CND_TRIGGERED)
					bFound = true;
			}

			if (!bFound)
				popup->ItemRemove(popup->ItemFindByText("New event"));
		}
	}


	popup->ItemDefaultSet(popup->ItemFindPosForCmdID(IDFR_INSERT));

	if (m_sel.GetCount() == 0)
	{
		popup->ItemEnabledSet(popup->ItemFindPosForCmdID(IDFR_OBJECTINFORMATION), false);
		popup->ItemEnabledSet(popup->ItemFindPosForCmdID(IDFR_ARRAYPASTE), false);

		// Disable clipboard actions
		popup->ItemEnabledSet(popup->ItemFindPosForCmdID(IDFR_CUT), false);
		popup->ItemEnabledSet(popup->ItemFindPosForCmdID(IDFR_COPY), false);
		popup->ItemEnabledSet(popup->ItemFindPosForCmdID(IDFR_DELETE), false);
	}

	POSITION position = m_sel.GetHeadPosition();

	// Mouse position
	POINT curPos;
	GetCursorPos(&curPos);

	// Display it
	popup->TrackPopupMenu(TPMX_DO_MESSAGE_LOOP, curPos.x, curPos.y, NULL, NULL, NULL, NULL);

	m_UpdatePreview = true;

	CView::OnRButtonUp(nFlags, point);
}

void CLayoutEditor::OnMouseMove(UINT nFlags, CPoint point) 
{
	layout->m_LayoutEditor = this;

	if (!layout)
		return;

	if(g_bDuplicate)
		if(!(GetKeyState(VK_LBUTTON)>>4))
			g_bDuplicate = false;

	UpdateStatus();

	// MouseMove is not ready if hasnt drawn a frame yet - Ash
	if (!HasCreated) return;


	if ((GetTickCount() - 25) > m_lastTick) {
		if (m_bDraggingRect || m_bSizingObject) {
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
		}
		m_lastTick = GetTickCount();
	}
	if (m_lastTick > GetTickCount())
		m_lastTick = GetTickCount();


	// Middle click drag?
	if ((m_mdragging || ((GetKeyState(VK_SPACE)>>4) && (GetKeyState(VK_LBUTTON)>>4))) && m_usermoved) 
	{
		// Scroll this movement

		CPoint pt = point;

		SetRedraw(false);
		LockScrollUpdate();
		int dx = m_mOld.x - pt.x;
		int dy = m_mOld.y - pt.y;

		m_usermoved = false;
		ScrollToPosition(GetScrollPosition() + CPoint(dx,dy));

		SetRedraw(true);

		// Update old mouse position
		m_mOld = pt;

		UnlockScrollUpdate();
		Invalidate();
		RedrawWindow();

		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
	

		m_usermoved = true;
		return;
	}

	m_mOld = point;

	point += CPoint(-FRAME_MARGIN_W*m_Zoom, -FRAME_MARGIN_H*m_Zoom);


	point += GetScrollPosition();
	UpdateRulersInfo(RW_POSITION, GetScrollPosition(), point);
	if(InputLocked())
	{
		layout->m_LayoutEditor = this;
		layout->Message(WM_MOUSEMOVE);
		return;
	}

	// Update point and cursor
	if (gInsert != "" || g_bPaste || g_bPasteClone || g_addToGroup >= 0) 
	{ // are we inserting an object?
		SetCursor(LoadCursor(NULL, IDC_CROSS));
		return;
	}

	if (m_bDraggingRect) 
	{ // are we dragging a rectangle?
		m_curPt = point;
		RedrawWindow();
		return;
	}

	// Begin selection dragging
	if (m_bMouseDown && m_clickedOn == -1 && !m_bMovingObject && !m_bSizingObject)
		InitializeSelect(point);

	// Begin object moving
	else if (m_bMouseDown && m_clickedOn >= 0 && !m_bMovingObject && !m_bSizingObject)
		InitializeMove(point);


	// Force object sizing to regulations
	else if (m_bSizingObject) 
	{
		m_curPt = point;
		//if (nFlags & MK_SHIFT && (m_sizeType == 1 || m_sizeType == 9))
		//	m_curPt.y = m_oldPt.y + ((double)(m_curPt.x - m_oldPt.x)) * m_oldRatio;
		//if (nFlags & MK_SHIFT && (m_sizeType == 3 || m_sizeType == 7))
		//	m_curPt.y = m_oldPt.y - ((double)(m_curPt.x - m_oldPt.x)) * m_oldRatio;
		SetCursor(LoadCursor(NULL, m_sizeCur));
		RedrawWindow();
	}
 
	// Over sizing handles
	else
		CheckSizingHandles(point);

	
	static CPoint oldpoint;
	CString coord;

	CObj* o;
	CObjMap* objMap = &layout->objects;
	POSITION pos, pos2;

	if (point != oldpoint)
	{
		bool bFound = false;

		// See if an object lays here
		pos2 = layout->layers.GetHeadPosition();
		for (int j = 0; j < layout->layers.GetCount(); j++) 
		{
			CLayer *lyr = layout->layers.GetNext(pos2);
			if (lyr->m_layerType != LAYER_NONFRAME && lyr->m_state == LAYSTATE_VISIBLE) 
			{
				pos = lyr->m_zOrder.GetTailPosition();
				for (int i = 0; i < lyr->m_zOrder.GetCount(); i++) 
				{
					long id = lyr->m_zOrder.GetPrev(pos);
					objMap->Lookup(id, o);

					if (o->GetVisible() && !o->GetLocked()) 
					{
						if (o->GetObjectRect(this).PtInRect(point)) 
						{
							if (!(o->m_bIsGroup || o->m_bGroupOpen))
							{
								CString old;
								m_ToolTip.GetText(old, this);

								CObjType* pObjType = o->GetObjectType(application);
								CString strObjName = "???";

								if (pObjType)
								{
									strObjName = pObjType->GetName();				

									bFound = true;

									CString tooltip_text;
									tooltip_text.Format("Name: %s\r\nPosition: %.2f, %.2f\r\nSize: %.2f, %.2f\r\nAngle: %.2f\r\nLayer: %s", 
														pObjType->GetName(), o->editObject->objectX, o->editObject->objectY, o->editObject->objectWidth, o->editObject->objectHeight,
														o->editObject->objectAngle, lyr->m_name);
									
									if(tooltip_text != old)
									{	
										m_ToolTip.Pop();
										m_ToolTip.DelTool(this);
										m_ToolTip.AddTool(this, tooltip_text);
									}
								}
							
								POINT a;
								a.x = point.x;
								a.y = point.y;
								//Kill the Tool Tip when you move the mouse.
								//
							}
							else
							{
								bFound = false;
							//	m_ToolTip.DelTool(this);
							}
							break;
						}
					}
				}
			}
		}

		if (!bFound)
			m_ToolTip.DelTool(this);

		oldpoint = point;
	}
}

void CLayoutEditor::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_mdragging = true;
	m_mOld = point;
	SetCursor(LoadCursor(NULL, IDC_SIZEALL));
  
	SetCapture();
}

void CLayoutEditor::OnMButtonUp(UINT nFlags, CPoint point) 
{
	m_mdragging = false;
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	
	ReleaseCapture();
	m_UpdatePreview = true;
	Invalidate();

}

BOOL CLayoutEditor::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CLayoutEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Note: if you are looking for the code where hitting enter pastes an object, its actually in
	// on drag over, because the stupid drag manager wont call any of the layout editors message functions

	layout->m_LayoutEditor = this;

	if(InputLocked())
	{
		layout->Message(WM_KEYDOWN);
		return;
	}

	CString tempStr = "";
	
	int dist = 1;
	if(GetKeyState(VK_SHIFT)>>4)
	{
		dist = 16;
	}
	switch (nChar)
	{
		case VK_UP:
			{
				CObj* o;

				// Move current selection up 1
				if (m_sel.GetCount() >= 1) 
				{
					POSITION pos = m_sel.GetHeadPosition();

					for (int i = 0; i < m_sel.GetCount(); i++) 
					{
						o = GetObject(m_sel.GetNext(pos));
						
						o->SetPos(o->GetX(), o->GetY() - dist);

						layout->SetChanged(true);
					}

					g_MainFrame->m_PropertiesBar.Refresh();
				}

				Invalidate();
			} break;

		case VK_DOWN:
			{
				CObj* o;

				// Move current selection up 1
				if (m_sel.GetCount() >= 1) 
				{
					POSITION pos = m_sel.GetHeadPosition();

					for (int i = 0; i < m_sel.GetCount(); i++) 
					{
						o = GetObject(m_sel.GetNext(pos));
						o->SetPos(o->GetX(), o->GetY() + dist);

						layout->SetChanged(true);
					}

					g_MainFrame->m_PropertiesBar.Refresh();
				}

				Invalidate();
			} break;

		case VK_LEFT:
			{
				CObj* o;

				// Move current selection up 1
				if (m_sel.GetCount() >= 1) 
				{
					POSITION pos = m_sel.GetHeadPosition();

					for (int i = 0; i < m_sel.GetCount(); i++) 
					{
						o = GetObject(m_sel.GetNext(pos));
						o->SetPos(o->GetX() - dist, o->GetY());

						layout->SetChanged(true);
					}

					g_MainFrame->m_PropertiesBar.Refresh();
				}

				Invalidate();
			} break;

		case VK_RIGHT:
			{
				CObj* o;

				// Move current selection up 1
				if (m_sel.GetCount() >= 1) 
				{
					POSITION pos = m_sel.GetHeadPosition();

					for (int i = 0; i < m_sel.GetCount(); i++) 
					{
						o = GetObject(m_sel.GetNext(pos));
						o->SetPos(o->GetX() + dist, o->GetY());

						layout->SetChanged(true);
					}

					g_MainFrame->m_PropertiesBar.Refresh();
				}

				Invalidate();
			} break;

		case VK_DELETE:
			{
				DeleteSelection();
			} break;
	}

	Invalidate();
	
	m_UpdatePreview = true;
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////// Dragging and Scrolling ///////////////////////////
/////////////////////////////////////////////////////////////////////////////


BOOL CLayoutEditor::OnScroll(UINT nScrollCode, UINT nPos, BOOL
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

      return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}


void CLayoutEditor::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	this->SetRedraw(false);
	UpdateRulersInfo(RW_VSCROLL, GetScrollPosition());
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
	ScrollPos.y = CScrollView::GetScrollPos(1);

	m_UpdatePreview = true;

	layout->scroll_position = ScrollPos;
	this->SetRedraw(true);
	Invalidate();
}

void CLayoutEditor::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	this->SetRedraw(false);
	UpdateRulersInfo(RW_HSCROLL, GetScrollPosition());
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
	ScrollPos.x = CScrollView::GetScrollPos(0);

	m_UpdatePreview = true;

	this->SetRedraw(true);
	layout->scroll_position = ScrollPos;
	Invalidate();
}

CDragObjects *g_dragInfo2;

void CLayoutEditor::InitializeMove(CPoint point)
{
	layout->m_LayoutEditor = this;

	CRect rc(m_oldPt, CSize(1,1));
	rc.InflateRect(1,1);
	if (!rc.PtInRect(point))
	{
		DROPEFFECT drop = DROPEFFECT_NONE|DROPEFFECT_SCROLL;
		CDragObjects *dragInfo = new CDragObjects;
		dragInfo->layout_editor = this;
		dragInfo->layout = layout;
		dragInfo->m_pSel = &m_sel;
		dragInfo->application = application;
		dragInfo->m_curPt = m_oldPt;
		dragInfo->layers.clear();

		// keep layers when ctrl-dragging
		m_bLayersAfterMove = TRUE;
		POSITION pos = m_sel.GetHeadPosition();
		CObj *o;

		for (int i = 0; i < m_sel.GetCount(); i++) 
		{
			o = GetObject(m_sel.GetNext(pos));
			dragInfo->layers.push_back(GetObjectLayer(o->GetInstanceID())->m_layerID);
		}

		m_bMouseDown = FALSE;
		g_bIsDragging = TRUE;
		m_bMovingObject = TRUE;
		
		CStringList    lsDraggedFiles;
		//POSITION       pos;
		CString        sFile;
		UINT           uBuffSize = 0;

		// For every selected item in the list,
		// put the filename into lsDraggedFiles.
		// c_FileList is our dialog's CListCtrl.
		/*pos = c_FileList.GetFirstSelectedItemPosition();

		while ( NULL != pos )
        {
			nSelItem = c_FileList.GetNextSelectedItem ( pos );
			// put the filename in sFile
			sFile = c_FileList.GetItemText ( nSelItem, 0 );    

			lsDraggedFiles.AddTail ( sFile );

			// Calculate the # of chars required to hold this string.
			uBuffSize += lstrlen ( sFile ) + 1;
        }*/

		// Add 1 extra for the final null char,
		// and the size of the DROPFILES struct.
		uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (uBuffSize + 1);

		DROPFILES* pDrop;

		// Allocate memory from the heap for the DROPFILES struct.
		pDrop = (DROPFILES*)(new char[uBuffSize]);

		// Fill in the DROPFILES struct.
		pDrop->pFiles = sizeof(DROPFILES);

#ifdef _UNICODE
		// If we're compiling for Unicode, set the Unicode flag in the struct to
		// indicate it contains Unicode strings.
		pDrop->fWide = TRUE;
#endif

		TCHAR* pszBuff;

		// Copy all the filenames into memory after
		// the end of the DROPFILES struct.
//		pos = lsDraggedFiles.GetHeadPosition();
		pszBuff = (TCHAR*) (LPBYTE(pDrop) + sizeof(DROPFILES));
		char foo[1];
		foo[0] = 0;
		lstrcpy(pszBuff, (LPCTSTR)foo);

/*		while ( NULL != pos )
        {
			lstrcpy ( pszBuff, (LPCTSTR) lsDraggedFiles.GetNext ( pos ) );
			pszBuff = 1 + _tcschr ( pszBuff, '\0' );
        }*/

		g_dragInfo2 = dragInfo;

		g_pLayoutView = this;
		g_pFrame = layout;
		g_pSel = &m_sel;
		g_pApp = application;
		g_pPt = &point;
		g_pDrop = &drop;
		
		m_mouseDragStartPos = point;

		// Set up the timer for scrolling
		m_nTimerID = SetTimer(75, 25, NULL);

		ddm.PrepareDrop(FALSE, DRAGDROP_OBJECTS, dragInfo, &drop, "CF_HDROP", (CObject*)pDrop);
		
		layout->SetChanged(true);

		KillTimer(m_nTimerID);

		m_bMovingObject = FALSE;
		delete dragInfo;
		delete pDrop;
	}
	RedrawWindow();
}

DROPEFFECT CLayoutEditor::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
    ddm.OnDragEnter(pDataObject);
    return this->OnDragOver(pDataObject,dwKeyState,point);
}

void CLayoutEditor::OnDragLeave() 
{
	RedrawWindow();
}

void CLayoutEditor::PasteDraggedObjects()
{
	CPoint* original = g_pPt;
	DROPEFFECT* pOriginalDrop = g_pDrop;
	CDragObjects* originaldragInfo2 = g_dragInfo2;
	bool originalbIsDragging = g_bIsDragging;



	CPoint startdragpos = m_mouseDragStartPos;

	CObjList oldsel;
	POSITION pos3;
	pos3 = m_sel.GetHeadPosition();
	while(pos3)
		oldsel.AddTail( m_sel.GetNext(pos3));

	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	HGLOBAL data = NULL;

	CDragObjects* dragInfo = new CDragObjects;

	////////////////////////////
	// Save the selected objects

	CSharedFile	sf(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
	CArchive	ar(&sf, CArchive::store);

	DROPEFFECT drop = DROPEFFECT_NONE|DROPEFFECT_SCROLL;

	dragInfo->layout_editor = this;
	dragInfo->layout = layout;
	dragInfo->m_pSel = &m_sel;
	dragInfo->application = application;
	dragInfo->m_curPt = m_oldPt;
	dragInfo->layers.clear();

	// keep layers when ctrl-dragging
	m_bLayersAfterMove = TRUE;
	POSITION pos = m_sel.GetHeadPosition();
	CObj *o;

	for (int i = 0; i < m_sel.GetCount(); i++) 
	{
		o = GetObject(m_sel.GetNext(pos));
		dragInfo->layers.push_back(GetObjectLayer(o->GetInstanceID())->m_layerID);
	}

	m_bMouseDown = FALSE;
	g_bIsDragging = TRUE;
	m_bMovingObject = TRUE;
	
	CStringList    lsDraggedFiles;
	//POSITION       pos;
	CString        sFile;
	UINT           uBuffSize = 0;


	// Add 1 extra for the final null char,
	// and the size of the DROPFILES struct.
	uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (uBuffSize + 1);

	DROPFILES* pDrop;

	// Allocate memory from the heap for the DROPFILES struct.
	pDrop = (DROPFILES*)(new char[uBuffSize]);

	// Fill in the DROPFILES struct.
	pDrop->pFiles = sizeof(DROPFILES);

#ifdef _UNICODE
	// If we're compiling for Unicode, set the Unicode flag in the struct to
	// indicate it contains Unicode strings.
	pDrop->fWide = TRUE;
#endif

	TCHAR* pszBuff;

	// Copy all the filenames into memory after
	// the end of the DROPFILES struct.
//		pos = lsDraggedFiles.GetHeadPosition();
	pszBuff = (TCHAR*) (LPBYTE(pDrop) + sizeof(DROPFILES));
	char foo[1];
	foo[0] = 0;
	lstrcpy(pszBuff, (LPCTSTR)foo);

/*		while ( NULL != pos )
	{
		lstrcpy ( pszBuff, (LPCTSTR) lsDraggedFiles.GetNext ( pos ) );
		pszBuff = 1 + _tcschr ( pszBuff, '\0' );
	}*/

	g_dragInfo2 = dragInfo;

	g_pLayoutView = this;
	g_pFrame = layout;
	g_pSel = &m_sel;
	g_pApp = application;
	g_pPt = &startdragpos;
	g_pDrop = &drop;

	dragInfo->Serialize(ar);
	
	// Trans
	ar.Close();
	data = sf.Detach();
	

	// Paste
	CMemFile mf((BYTE*)::GlobalLock(data), ::GlobalSize(data));
	CArchive	ar2(&mf, CArchive::load);

	g_bIsDragging = FALSE;

	g_pSel = &m_sel;
	g_pFrame = layout;
	g_pApp = application;
	g_pFrame = layout;

	g_pLayoutView = this;

	GetCursorPos(&point);
	this->ScreenToClient(&point);
	g_pPt = &point;
	DROPEFFECT dropEffect = DROPEFFECT_COPY;
	g_pDrop = &dropEffect;
	g_bDuplicate = FALSE;

	

	CDragObjects dragInfo2;
	dragInfo2.Serialize(ar2);


	ar2.Close();

	delete dragInfo;

	g_pPt = original;	
	g_pDrop = pOriginalDrop;
	g_dragInfo2 = originaldragInfo2;
	g_bIsDragging = originalbIsDragging;


	m_sel.RemoveAll();
	POSITION pos2;
	pos2 = oldsel.GetHeadPosition();
	while(pos2)
		m_sel.AddTail( oldsel.GetNext(pos2));

}

DROPEFFECT CLayoutEditor::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
    m_de = ddm.OnDragOver(pDataObject,dwKeyState,point);
	RedrawWindow();
	m_curPt = point;
	if(g_bDuplicate)
		m_de |= DROPEFFECT_COPY;

	
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



	if(GetKeyState(VK_RETURN)>>4)
	{
		if(!lastEnterPressed)
		{
			PasteDraggedObjects();
		}
		this->lastEnterPressed = true;
	}
	else
	{
		this->lastEnterPressed = false;

	}

	return m_de;
}


	CObjList		*g_pSel;
	CApplication			*g_pApp;
	extern CLayoutEditor	*g_pLayoutView;
	CPoint			*g_pPt;
	DROPEFFECT		*g_pDrop;
	BOOL			g_bDuplicate = FALSE;

BOOL CLayoutEditor::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	layout->m_LayoutEditor = this;

   CString csF = ddm.AvailableDataType(pDataObject);

	if(layout->m_ObjectFrameIsLockedTo != 0)
		return FALSE;

    if (csF == DRAGDROP_OBJECTS) 
	{
		if (g_bIsDragging) 
		{
			g_bIsDragging = FALSE;

			g_pSel = &m_sel;
			g_pFrame = layout;
			g_pApp = application;
			g_pFrame = layout;

			g_pLayoutView = this;

			GetCursorPos(&point);
			this->ScreenToClient(&point);
			g_pPt = &point;
			g_pDrop = &dropEffect;
			g_bDuplicate = FALSE;

			CDragObjects *dragInfo = new CDragObjects;

			if (!ddm.DoDrop(dragInfo, pDataObject, DRAGDROP_OBJECTS)) 
			{
				CErrorDlg error;
				error.Error("Drag/drop error", "An error occured whilst dropping objects.");

				delete dragInfo;
				return FALSE;
			}

			if (dragInfo->layout_editor = this)
				g_PropertyBar->Update(this, TYPE_OBJECT, &m_sel, layout, &layout->objects, application); // show object props

			delete dragInfo;
			m_bMovingObject = FALSE;
			m_bMouseDown = FALSE;
			RedrawWindow();
			m_bUpdateFrame = TRUE;
			KillTimer(m_nTimerID);

			layout->SetChanged(true);

			return TRUE;
		}
	}

    return FALSE;
}

void CLayoutEditor::UpdateSelection()
{

}