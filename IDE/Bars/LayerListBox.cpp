#include "StdAfx.h"
#include "LayerBar.h"

#include "..\Bars\PropertiesBar.h"
extern PropertiesBar* g_PropertyBar;

/////////////////////////////////////////////////////////////////////////////
// CLayerListBox

CLayerListBox::CLayerListBox()
{
	m_Font.CreateFont(14, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "Arial");
}

CLayerListBox::~CLayerListBox()
{
}

BEGIN_MESSAGE_MAP(CLayerListBox, CDragListBox)
	//{{AFX_MSG_MAP(CLayerListBox)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayerListBox message handlers

int CLayerListBox::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CDragListBox::OnCreate(lpcs) == -1)
		return -1;

	DragAcceptFiles(TRUE); // allow dropping objects onto the layers
	imglist.Create(IDR_LAYERICONS, 12, 0, RGB(255, 0, 255));

	return 0;
};

void CLayerListBox::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDragListBox::PreSubclassWindow();
}

void CLayerListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	// draw the layer list item
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	// Work out the z number of the layer
	CString text;
	text.Format("%d", (this->GetCount() - lpDIS->itemID) - 1);

	// init some stuff
	pDC->SelectObject(&m_Font);
	CRect rcFull(lpDIS->rcItem);
	CSize szThumbnail(64, 48);
	CRect rcThumbnail(rcFull.TopLeft() + CPoint(16 + 4, 4), szThumbnail);
	CRect rcText(rcThumbnail.right + 6, rcFull.top + 4, rcFull.right - 4, rcFull.bottom - 4);	
	CLayer* pLayer = (CLayer*)(GetItemDataPtr(lpDIS->itemID));
	pDC->SetBkMode(TRANSPARENT);
	CLayerDlg* parent = (CLayerDlg*)GetParent();

	int drawCommand = 0;
	if ((lpDIS->itemAction & ODA_DRAWENTIRE) || ((!(lpDIS->itemState & ODS_SELECTED) && (lpDIS->itemAction & ODA_SELECT))))
		drawCommand = 1;	
	if ((lpDIS->itemState & ODS_SELECTED) && (lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
		drawCommand = 2;

	if (drawCommand > 0) {
		// item has been selected or draw all item 
		// fill background for setted background color 
		CRect rect(rcFull);

		if (pLayer->m_layerType == LAYER_NONFRAME) // text position for nonframe layer
			rcText.left = rcFull.left + 26 + 6;

		if (drawCommand == 1) { // draw the item, non selected
			pDC->FillSolidRect(rect, CLR_BG);
			pDC->DrawEdge(rect,EDGE_SUNKEN,BF_BOTTOM);
			pDC->SetTextColor(RGB(0,0,0));			// unselected text color
			rect.CopyRect(rcText);
			pDC->DrawText(pLayer->m_name,lstrlen(pLayer->m_name), rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	
			if (pLayer->m_layerType != LAYER_NONFRAME)
			{
				CRect pos;
				pos.left  = (rcFull.TopLeft() + CPoint(4, 22+16)).x;
				pos.top   = (rcFull.TopLeft() + CPoint(4, 22+16)).y;

				pos.right = pos.left + 13;
				pos.bottom = pos.top + 13;

				pDC->DrawText(text,lstrlen(text),pos, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_VCENTER);
			}	
		}

		if (drawCommand == 2) { // draw the item, selected
			pDC->FillRect(rect,&CBrush(CLR_SEL));	// fill item frame
			pDC->DrawEdge(rect,EDGE_SUNKEN,BF_BOTTOM);
			pDC->SetTextColor(CLR_BG);			// selected text color
			rect.CopyRect(rcText);
			pDC->DrawText(pLayer->m_name,lstrlen(pLayer->m_name), rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

			if (pLayer->m_layerType != LAYER_NONFRAME)
			{
				CRect pos;
				pos.left  = (rcFull.TopLeft() + CPoint(4, 22+16)).x;
				pos.top   = (rcFull.TopLeft() + CPoint(4, 22+16)).y;

				pos.right = pos.left + 13;
				pos.bottom = pos.top + 13;

				pDC->DrawText(text,lstrlen(text),pos, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_VCENTER);
			}
		}

		if (pLayer->m_layerType != LAYER_NONFRAME) { // images
			// draw thumbnail of layer
			CxImage* pImage = parent->images[pLayer].bitmap;	//TODO: use real number
			
			rcThumbnail.InflateRect(1, 1);
			pDC->Draw3dRect(rcThumbnail, 0,0);
			rcThumbnail.DeflateRect(1, 1);

			//draw
			HDC hdc = pDC->GetSafeHdc();
			if(pImage)
				pImage->Draw2(hdc, rcThumbnail);

			// draw the visible/hidden/locked icon
			imglist.Draw(pDC, (!ISLAYERVISIBLE(pLayer->m_state)), rcFull.TopLeft() + CPoint(4, 22-16), ILD_TRANSPARENT);
			imglist.Draw(pDC, (ISLAYERLOCKED(pLayer->m_state))+2, rcFull.TopLeft() + CPoint(4, 22), ILD_TRANSPARENT);

		}
	}
}

void CLayerListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS) 
{
	// find the height of a layer list item. this should be the stored text
	CRect rcClient;
	GetClientRect(rcClient);
	lpMIS->itemWidth = max(rcClient.Width(), 128);
	CString sz;
	GetText(lpMIS->itemID, sz);
	lpMIS->itemHeight = atol((LPCTSTR)sz);
}

BOOL CLayerListBox::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDragListBox::OnNotify(wParam, lParam, pResult);
}

extern CDragObjects *g_dragInfo2;

void CLayerListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point);
}

void CLayerListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// toggle through the visible/locked status of layers
	CRect rc;
	CPoint ptt;
	CLayer *layer;

	if(layout_editor->layout->m_ObjectFrameIsLockedTo != 0)
		return;

	::GetCursorPos(&ptt);
	int item = ItemFromPt(ptt);

	if (item == -1) return;

	layer = (CLayer*)GetItemDataPtr(item);

	if (item >= 0) 
	{
 		GetItemRect(item, &rc);
 		CPoint pt = point - rc.TopLeft();
 		CRect rc2(CPoint(4, 22-16), CSize(12, 12));
 		if (PtInRect(&rc2, pt))	// Visibility
 		{
 			layer->m_state = (layer->m_state & 0x02) + (ISLAYERVISIBLE(layer->m_state));
 			Invalidate();
 			layout_editor->m_bUpdateFrame = true;
 			layout_editor->Invalidate();
 		}
 		rc2.OffsetRect(0,16);
 		if (PtInRect(&rc2, pt))	// lockability
 		{
 			layer->m_state = (layer->m_state & 0x01) + ((!ISLAYERLOCKED(layer->m_state)) <<1);
 			Invalidate();
 			layout_editor->m_bUpdateFrame = true;
 			layout_editor->Invalidate();
 		}
 	}



	CDragListBox::OnLButtonDown(nFlags, point);

	g_PropertyBar->Update(layout_editor, TYPE_LAYER, NULL, NULL,NULL, layout_editor->application, 0, layer); // update the property editor


	if(GetKeyState(VK_CONTROL) >> 4)
	{
		// Get every item in the layer, and add to the selection
		layout_editor->m_sel.RemoveAll();
		POSITION pos = layer->m_zOrder.GetHeadPosition();
		while(pos)
			layout_editor->m_sel.AddTail(layer->m_zOrder.GetNext(pos));

		layout_editor->Invalidate();
	}



	SetCurSel(item);
}

void CLayerListBox::OnDropFiles(HDROP hdrop)
{
	// this function gets called when objects are dropped from the layout editor onto a layer
	CPoint pt;
	DragQueryPoint(hdrop, &pt);
    DragFinish ( hdrop ); // Free up memory

	CLayer *newLyr = NULL;

	// find the layer the objects were dropped onto
	BOOL bOutside = FALSE;
	UINT item = ItemFromPoint(pt, bOutside);
	if (bOutside)
		return;
	newLyr = (CLayer*)GetItemDataPtr(item); // the layer to move the objects to!
	if (!newLyr)
		return;
	if (newLyr->m_layerType != LAYER_NORMAL)
		return;
	
	// START UNDO INFO //
	CAction *action = new CAction();
	CLayer *lyr;
	action->m_type = ACTION_CHANGEZORDER;
	CArchive *ar = action->Prepare1();
	layout_editor->SerializeAllZLists(ar);
	action->Prepare2();
	layout_editor->m_undo->AddAction(action);
	// END UNDO INFO //
	
	// begin moving objects to the correct layer
	POSITION pos = layout_editor->layout->layers.GetHeadPosition();
	long value;
	for (int j=0; j < layout_editor->layout->layers.GetCount(); j++) { // loop through the layers
		lyr = layout_editor->layout->layers.GetNext(pos);
		if (lyr == newLyr) // this is the layer being moved to, we can skip it
			continue;
		POSITION nextItem = lyr->m_zOrder.GetHeadPosition();
		POSITION curItem = lyr->m_zOrder.GetTailPosition();
		long c = lyr->m_zOrder.GetCount();
		for (int i=0; i < c; i++) { // loop through the objects in this layer
									// curItem = item to be moved, nextItem = next item to be looked at
			curItem = nextItem;
			value = lyr->m_zOrder.GetNext(nextItem);
			if (layout_editor->m_sel.Find(value)) { // object is in the selection dropped on the layer
				lyr->m_zOrder.RemoveAt(curItem);
				newLyr->m_zOrder.AddTail(value);
			}
		}
	}

	// update layout
	layout_editor->m_bUpdateFrame = true;
	layout_editor->Invalidate();
}

BOOL CLayerListBox::BeginDrag(CPoint pt)
{
	// a layer has just started being dragged
	int item = ItemFromPt(pt);
	return CDragListBox::BeginDrag(pt);
}

UINT CLayerListBox::Dragging(CPoint pt)
{
	// a layer is being dragged
	int item = ItemFromPt(pt);
	CRect rc;
	GetWindowRect(&rc);
	if (item == -1 && PtInRect(rc, pt)) { // hack to allow dragging to end of list
		DrawInsert(-1);
		DrawInsert(GetCount());
		return DL_MOVECURSOR;
	}
	else
		return CDragListBox::Dragging(pt);
}

void CLayerListBox::Dropped(int nSrcIndex, CPoint pt)
{
	if (nSrcIndex == GetCount() - 1) return;

	// a layer has been dropped, figure out reordering
	int item = ItemFromPt(pt);
	CRect rc;
	GetWindowRect(&rc);
	CDragListBox::Dropped(nSrcIndex, pt);

	// get the layerlist, remove all items
	CLayerList *layerList = &layout_editor->layout->layers;
	while (layerList->GetCount() > 0)
		layerList->RemoveHead();

	// add the layers back into the layerlist, in the order they are in the listbox
	CLayer *j;
	for (int i = 0; i < GetCount(); i++) {
		j = (CLayer*)GetItemDataPtr(i);
		layerList->AddHead(j);
	}

	// update the layout
	layout_editor->layout->SetChanged(true);
	layout_editor->m_bUpdateFrame = true;
	layout_editor->Invalidate();
}