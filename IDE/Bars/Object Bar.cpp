// ObjectBar.cpp
// Implementation of object bar, new to 0.96.3

#include "stdafx.h"
#include "Object Bar.h"
#include "..\MainFrm.h"
#include "..\ChildFrame.h"
#include "..\Construct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern PropertiesBar*	g_PropertyBar;
extern CMainFrame*		pMainWnd;
extern CLayerBar*		g_pLayerBar;

static int CALLBACK 
ObjectCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	CString    strItem1 = (const char*)lParam1;
	CString    strItem2 = (const char*)lParam2;

	strItem1.MakeLower();
	strItem2.MakeLower();

	return strcmp(strItem1, strItem2);
}

static int CALLBACK 
ObjectCompareZA(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	CString    strItem1 = (const char*)lParam1;
	CString    strItem2 = (const char*)lParam2;

	strItem1.MakeLower();
	strItem2.MakeLower();

	return strcmp(strItem2, strItem1);
}

/////////////////////////////////////////////////////////////////////////////
// ObjectBarDialog

ObjectBarDialog::ObjectBarDialog(CChildFrame& parent_)
	: CExtResizableDialog(IDD_BARDIALOG, NULL),
	parent(parent_),
	view(ob_large_icons),
	sorting(ob_sort_zorder),
	show_only_selected_layer(false),
	show_nonlayout_objects(true)
{
}

void ObjectBarDialog::DoDataExchange(CDataExchange* pDX)
{
	CExtResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_MENUBAR, toolbar);
}

BEGIN_MESSAGE_MAP(ObjectBarDialog, CExtResizableDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_NOTIFY(NM_CLICK, 100, OnClickObject)
	ON_NOTIFY(NM_DBLCLK, 100, OnDblClickObject)
	ON_NOTIFY(NM_RCLICK, 100, OnRClickObject)
	ON_NOTIFY(LVN_KEYDOWN, 100, OnKeyDown)
	ON_NOTIFY(LVN_BEGINDRAG, 100, OnBeginDrag)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Object bar dialog functions
BOOL ObjectBarDialog::OnInitDialog() 
{
	CExtResizableDialog::OnInitDialog();

	CRect size;
	GetWindowRect(&size);

	objects.Create(WS_VISIBLE | WS_CHILD | LVS_AUTOARRANGE | LVS_ICON | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOCOLUMNHEADER, CRect(0, 0, 100, 100), this, 100);
	//object_tree.Create(WS_CHILD | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_EDITLABELS, CRect(0, 0, 10, 10), this, 101);

	large_images.Create(32, 32, ILC_COLOR24, 3, 3);
	small_images.Create(16, 16, ILC_COLOR24, 3, 3);

	objects.SetImageList(&large_images, LVSIL_NORMAL);
	objects.SetImageList(&small_images, LVSIL_SMALL);

	objects.ShowScrollBar(SB_VERT); 
	objects.EnableScrollBar(SB_VERT, ESB_DISABLE_BOTH);

	objects.InsertColumn(0, "", 0, 80);

	toolbar.DestroyWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ObjectBarDialog::OnSize(UINT nType, int cx, int cy) 
{
	CExtResizableDialog::OnSize(nType, cx, cy);

	if (IsWindow(objects.m_hWnd)) 
	{
		CRect list_rect;

		list_rect.top = 0;
		list_rect.left = 0;
		list_rect.right = cx;
		list_rect.bottom = cy;

		objects.MoveWindow(list_rect);

		objects.SetColumnWidth(0, cx - 5);
	}
}

void ObjectBarDialog::SetLayout(CLayout* layout_, CApplication* application_)
{
	application = application_;
	layout = layout_;

	Refresh();
}

void ObjectBarDialog::Refresh(bool layer_changed)
{
	// check for unnecessary refresh
	if (layer_changed && !show_only_selected_layer)
		return;

	// clear all lists
	objects.DeleteAllItems();

	for (int i = 0; i < large_images.GetImageCount(); i++)
		large_images.Remove(0);

	for (int i = 0; i < small_images.GetImageCount(); i++)
		small_images.Remove(0);

	CObj*			pObject;
	CObjType*		pObjectType;
	CStringArray	List; // Object list

	if (layout)
	{
		POSITION LayerPos = layout->layers.GetHeadPosition();

		// For each layer
		while(LayerPos)
		{
			CLayer* pLayer = layout->layers.GetNext(LayerPos);

			if (show_only_selected_layer && pLayer != layout->current_layer)
				continue;

			if (!show_nonlayout_objects && pLayer->m_layerType == LAYER_NONFRAME)
				continue;

			// Loop all objects
			CObjList Objects;
			pLayer->GetEveryObject(Objects, layout);

			POSITION ObjectPos = Objects.GetHeadPosition();

			for (int i = 0; i < Objects.GetCount(); i++) 
			{
				long ID = Objects.GetNext(ObjectPos);
				layout->objects.Lookup(ID, pObject);

				if (pObject->GetGlobalID() != -1) 
				{
					pObjectType = pObject->GetObjectType(application);
					
					// Failed/invalid object type, for some reason
					if(!pObjectType)
						continue;

					bool bAdd = true;

					for (int j = 0; j < List.GetSize(); j++)
						if (List.GetAt(j) == pObjectType->GetName())
							bAdd = false;

					if (bAdd)
					{
						HBITMAP large_image = pObjectType->m_Image.MakeBitmap();
						HBITMAP small_image = pObjectType->small_image.MakeBitmap();

						int ImageID = ImageList_Add(large_images.m_hImageList, large_image, NULL);
						ImageList_Add(small_images.m_hImageList, small_image, NULL);

						DeleteObject(large_image);
						DeleteObject(small_image);

						int Item = objects.InsertItem(objects.GetItemCount(), pObjectType->GetName(), ImageID);

						objects.SetItemData(Item, (LPARAM)((const char*)pObjectType->GetName()));

						List.Add(pObjectType->GetName());
					}
				} // Iterate each object
			} // Iterate each layer
		}
	}

	else
	{
		POSITION Pos = application->object_types.GetStartPosition();
		CObjType* oT;
		long nKey = 0;

		while (Pos != NULL) 
		{	
			application->object_types.GetNextAssoc(Pos, nKey, oT);
			if(oT && !oT->m_bIsGroupType)
			{
				int ImageID = ImageList_Add(large_images.m_hImageList, oT->m_Image.MakeBitmap(), NULL);
				int Item = objects.InsertItem(objects.GetItemCount(), oT->GetName(), ImageID);

				objects.SetItemData(Item, (DWORD)((LPCSTR)oT->GetName()));

				List.Add(oT->GetName());
			}
		}
	}

	objects.ShowScrollBar(SB_VERT); 

	// Work out if there's a vertical scrollbar. If there is, we'll resize a bit.
	int iListCount = objects.GetItemCount();
	int iListSize = objects.GetCountPerPage();

	if(iListCount < iListSize)
	{
		// Disable the arrows on the vertical scrollbar
		objects.EnableScrollBar(SB_VERT, ESB_DISABLE_BOTH);
		objects.SetScrollRange(SB_VERT, 0, 2);
	}

	else
	{
		// Enable the vertical scrollbar
		objects.EnableScrollBar(SB_VERT, ESB_ENABLE_BOTH);
	}

	if (sorting == ob_sort_az)
		objects.SortItems(ObjectCompare, (LPARAM)&objects);
	if (sorting == ob_sort_za)
		objects.SortItems(ObjectCompareZA, (LPARAM)&objects);

	// now replace the ids
	for (int i = 0; i < objects.GetItemCount(); i++)
	{
		CObjType* type = GetTypeFromName(application, objects.GetItemText(i, 0));
		objects.SetItemData(i, type->ObjectIdentifier);
	}
}

void ObjectBarDialog::OnClickObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION Pos = objects.GetFirstSelectedItemPosition();
	int Item = objects.GetNextSelectedItem(Pos);

	if (!layout) return;

	if (layout->m_ObjectFrameIsLockedTo != 0) // InputLocked()
		return;

	// Select in layout editor, if it's open
	if (parent.m_tabs.SelectionGet() == 0 && parent.m_tabs.ItemGetCount() == 2)
	{
		parent.layout_editor[0][0]->m_sel.RemoveAll();

		// This is intentionally here; clear selection if clicked on whitespace
		if(Item == -1)
		{
			parent.layout_editor[0][0]->Invalidate();
			g_PropertyBar->Update(parent.layout_editor[0][0], TYPE_LAYOUT, NULL, layout, NULL, application); 

			return;
		}

		// Now we have to wangle in the selected object
		int ObjectIdentifier = objects.GetItemData(Item);

		CObj*			pObject;
		CObjType*		pObjectType;

		POSITION LayerPos = layout->layers.GetHeadPosition();

		// So we have to find all the CObj's with this CObjType in the layout, and add them
		// For each layer
		while(LayerPos)
		{
			CLayer* pLayer = layout->layers.GetNext(LayerPos);

			// Loop all objects
			CObjList Objects;
			pLayer->GetEveryObject(Objects, layout);

			POSITION ObjectPos = Objects.GetHeadPosition();

			for (int i = 0; i < Objects.GetCount(); i++) 
			{
				CObj* pTestObject;
				long ID = Objects.GetNext(ObjectPos);
				layout->objects.Lookup(ID, pTestObject);

				if (pTestObject->GetGlobalID() != -1) 
				{
					CObjType* pTestType = pTestObject->GetObjectType(application);

					if (pTestType->ObjectIdentifier == ObjectIdentifier)
					{
						pObjectType = pTestType;
						pObject = pTestObject;

						long nKey = pObject->GetInstanceID();
						parent.layout_editor[0][0]->m_sel.AddTail(nKey);
					}
				}
			}
		}

		g_PropertyBar->Update(parent.layout_editor[0][0], TYPE_OBJECT, &parent.layout_editor[0][0]->m_sel, layout, &layout->objects, application); // Show object properties

		parent.layout_editor[0][0]->Invalidate();

		// While we're here, show animations for object
		// Future note: .. to be continued

		int iRoot = -1;
		OINFO* oInfo = GetOINFO(pObjectType->DLLIndex);
		if (oInfo->ETGetAnimationHandle)
		{
			oInfo->ETGetAnimationHandle(pObject->editObject, iRoot);
			pMainWnd->animator.UpdateAnimations(application, layout, pObjectType, iRoot);
		}
	}
}

void ObjectBarDialog::OnDblClickObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION Pos = objects.GetFirstSelectedItemPosition();
	int Item = objects.GetNextSelectedItem(Pos);

	if (Item == -1) return;

	CObjType* pType;
	long ID = objects.GetItemData(Item);
	application->object_types.Lookup(ID, pType);

	if (!pType) return;

	// If layout editor open..
	if (parent.m_tabs.SelectionGet() == 0 && parent.m_tabs.ItemGetCount() == 2)
	{
		POSITION ObjectPos = layout->objects.GetStartPosition();
		CObj* pObject;
		long ID = 0;

		for (int i = 0; i < layout->objects.GetCount(); i++) 
		{
			layout->objects.GetNextAssoc(ObjectPos, ID, pObject);

			CObjType* type = pObject->GetObjectType(application);

			if (type->ObjectIdentifier == pType->ObjectIdentifier)
			{
				OINFO* info = GetOINFO(pType->DLLIndex);
				if(info->ETOnNotify)
					info->ETOnNotify(pObject->editObject, 1);

				return;
			}
		}
	}

	// If event sheet editor, see if it has conditions. If so, open Event Wizard
	if ((parent.m_tabs.SelectionGet() == 1) && (parent.m_tabs.ItemGetCount() == 2) || parent.m_tabs.ItemGetCount() == 1)
	{
		bool bUse = false;

		for (int i = 0; i < pType->GetTableCount(CONDITION); i++)
		{
			ACESEntry2* pAce = pType->GetACESEntry(CONDITION, i);
			if(pAce == NULL || pAce->aceListName == "")
				continue; // Ignore null entries

			bUse = true;
		}

		if (bUse)
		{
			parent.m_pEventView[0][0]->m_InitialStage = 2;
			parent.m_pEventView[0][0]->m_InitialSelect = ID;
			parent.m_pEventView[0][0]->m_OldName = pType->GetName();
			parent.m_pEventView[0][0]->AddCondition(true);
		}
	}
}

void ObjectBarDialog::OnRClickObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION Pos = objects.GetFirstSelectedItemPosition();
	int Item = objects.GetNextSelectedItem(Pos);

	// Check if we're right clicking a blank space
	if (Item == -1)
	{
		POINT MousePosition;
		GetCursorPos(&MousePosition);

		CExtPopupMenuWnd * popup = new CExtPopupMenuWnd;
		popup->LoadMenu(m_hWnd, IDR_BLANK, true, false);
		popup->ItemRemove(0);

		UINT item = 0;

		if (parent.m_tabs.SelectionGet() == 0 && parent.m_tabs.ItemGetCount() == 2)
		{
			popup->ItemInsertCommand(1, -1, "Insert an object", NULL, NULL);
			popup->ItemInsert();
		}

		if (view == ob_large_icons)
		{
			popup->ItemInsertCommand(2, -1, "Show large icons", NULL, NULL, false, 3);
			popup->ItemInsertCommand(3, -1, "Show small icons", NULL, NULL, false, 0);
			popup->ItemInsertCommand(4, -1, "Show small icons (horizontal)", NULL, NULL, false, 0);
			popup->ItemInsertCommand(5, -1, "Show object tree", NULL, NULL, false, 0);
		}

		else if (view == ob_small_icons)
		{
			popup->ItemInsertCommand(2, -1, "Show large icons", NULL, NULL, false, 0);
			popup->ItemInsertCommand(3, -1, "Show small icons", NULL, NULL, false, 3);
			popup->ItemInsertCommand(4, -1, "Show small icons (horizontal)", NULL, NULL, false, 0);
			popup->ItemInsertCommand(5, -1, "Show object tree", NULL, NULL, false, 0);
		}

		else if (view == ob_small_icons_horizontal)
		{
			popup->ItemInsertCommand(2, -1, "Show large icons", NULL, NULL, false, 0);
			popup->ItemInsertCommand(3, -1, "Show small icons", NULL, NULL, false, 0);
			popup->ItemInsertCommand(4, -1, "Show small icons (horizontal)", NULL, NULL, false, 3);
			popup->ItemInsertCommand(5, -1, "Show object tree", NULL, NULL, false, 0);
		}

		else
		{
			popup->ItemInsertCommand(2, -1, "Show large icons", NULL, NULL, false, 0);
			popup->ItemInsertCommand(3, -1, "Show small icons", NULL, NULL, false, 0);
			popup->ItemInsertCommand(4, -1, "Show small icons (horizontal)", NULL, NULL, false, 0);
		//	popup->ItemInsertCommand(5, -1, "Show object tree", NULL, NULL, false, 3);
		}

		popup->ItemInsert();

		if (sorting == ob_sort_az)
		{
			popup->ItemInsertCommand(6, -1, "Sort A-Z", NULL, NULL, false, 3);
			popup->ItemInsertCommand(7, -1, "Sort Z-A", NULL, NULL, false, 0);
			popup->ItemInsertCommand(8, -1, "Sort by Z-Order", NULL, NULL, false, 0);
		}

		else if (sorting == ob_sort_za)
		{
			popup->ItemInsertCommand(6, -1, "Sort A-Z", NULL, NULL, false, 0);
			popup->ItemInsertCommand(7, -1, "Sort Z-A", NULL, NULL, false, 3);
			popup->ItemInsertCommand(8, -1, "Sort by Z-Order", NULL, NULL, false, 0);
		}

		else
		{
			popup->ItemInsertCommand(6, -1, "Sort A-Z", NULL, NULL, false, 0);
			popup->ItemInsertCommand(7, -1, "Sort Z-A", NULL, NULL, false, 0);
			popup->ItemInsertCommand(8, -1, "Sort by Z-Order", NULL, NULL, false, 3);
		}

		popup->ItemInsert();

		if (show_only_selected_layer)
			popup->ItemInsertCommand(9, -1, "Show only objects on the selected layer", NULL, NULL, false, 1);
		else
			popup->ItemInsertCommand(9, -1, "Show only objects on the selected layer", NULL, NULL, false, 0);

		if (show_nonlayout_objects)
			popup->ItemInsertCommand(10, -1, "Show non-layout objects", NULL, NULL, false, 1);
		else
			popup->ItemInsertCommand(10, -1, "Show non-layout objects", NULL, NULL, false, 0);

		// show menu
		popup->TrackPopupMenu(TPMX_DO_MESSAGE_LOOP | TPMX_NO_WM_COMMAND | TPMX_NO_CMD_UI, MousePosition.x, MousePosition.y, NULL, NULL, NULL, &item);
			
		if (item == 1)
			parent.layout_editor[0][0]->OnFrameAction(IDFR_INSERT);

		else if (item == 2)
		{
			view = ob_large_icons;
			ListView_SetView(objects.m_hWnd, LV_VIEW_ICON);		
			
			objects.ShowWindow(SW_SHOW);
			object_tree.ShowWindow(SW_HIDE);
		}

		else if (item == 3)
		{
			view = ob_small_icons;
			ListView_SetView(objects.m_hWnd, LV_VIEW_DETAILS);		
			
			objects.ShowWindow(SW_SHOW);
			object_tree.ShowWindow(SW_HIDE);
		}

		else if (item == 4)
		{
			view = ob_small_icons_horizontal;
			ListView_SetView(objects.m_hWnd, LV_VIEW_LIST);		

			objects.ShowWindow(SW_SHOW);
			object_tree.ShowWindow(SW_HIDE);
		}

		else if (item == 5)
		{
			view = ob_object_tree;
			
			objects.ShowWindow(SW_HIDE);
			object_tree.ShowWindow(SW_SHOW);
		}

		else if (item == 6)
		{
			sorting = ob_sort_az;
			Refresh();
		}

		else if (item == 7)
		{
			sorting = ob_sort_za;
			Refresh();
		}

		else if (item == 8)
		{
			sorting = ob_sort_zorder;
			Refresh();
		}

		else if (item == 9)
		{
			show_only_selected_layer = !show_only_selected_layer;
			Refresh();
		}

		else if (item == 10)
		{
			show_nonlayout_objects = !show_nonlayout_objects;
			Refresh();
		}
	}

	else
	{
		if (parent.m_tabs.SelectionGet() == 0 && parent.m_tabs.ItemGetCount() == 2)
		{
			parent.layout_editor[0][0]->m_sel.RemoveAll();

			CObjType* pType;
			long ID = objects.GetItemData(Item);
			application->object_types.Lookup(ID, pType);

			CLayout* pLayout = parent.layout_editor[0][0]->layout;

			// Iterate each instance
			POSITION instancePos = pLayout->objects.GetStartPosition();
			long unused = 0;

			while (instancePos)
			{
				CObj* pObj;
				pLayout->objects.GetNextAssoc(instancePos, unused, pObj);

				// Add 
				if (pObj->editObject->ObjectIdentifier == pType->ObjectIdentifier)
					parent.layout_editor[0][0]->m_sel.AddTail(unused);
			}

			parent.layout_editor[0][0]->OnRButtonUp(0, CPoint(0,0));
		}
	}
}

void ObjectBarDialog::OnKeyDown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	LV_KEYDOWN* pKeyDown = (LV_KEYDOWN*)pNMHDR;

	// delete, delete
	if (pKeyDown->wVKey == VK_DELETE)
	{
		// only if in layout editor
		if (parent.m_tabs.SelectionGet() == 0 && parent.m_tabs.ItemGetCount() == 2)
			parent.layout_editor[0][0]->OnKeyDown(VK_DELETE, 0, 0);
	}
}

void ObjectBarDialog::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	POSITION Pos = objects.GetFirstSelectedItemPosition();
	int Item = objects.GetNextSelectedItem(Pos);

	if (Item == -1) return; // Safety first

	// Select in layout editor, if it's open
	if (parent.m_tabs.SelectionGet() == 0 && parent.m_tabs.ItemGetCount() == 2)
	{
		// Find a CObj for the CObjType
		CObjType* pType;
		long ID = objects.GetItemData(Item);
		application->object_types.Lookup(ID, pType);

		// Find out if this is a nonframe object
		CPlugin plugin = GetPlugin(pType->DLLIndex);

		if (plugin.m_Flags & OF_NODRAW)
			return;

		CLayout* pLayout = parent.layout_editor[0][0]->layout;

		// We're creating a duplicate
		g_bDuplicate = TRUE;

		parent.layout_editor[0][0]->m_sel.RemoveAll();

		// Iterate each instance
		POSITION InstancePos = pLayout->objects.GetStartPosition();
		long unused = 0;
		CObj* pObj;

		while (InstancePos)
		{
			pLayout->objects.GetNextAssoc(InstancePos, unused, pObj);

			// Add 
			if (pObj->editObject->ObjectIdentifier == pType->ObjectIdentifier)
				break;
		}

		long OID = pObj->GetInstanceID();

		parent.layout_editor[0][0]->m_sel.AddTail(OID);
		CPoint pt = pObj->GetObjectRect(parent.layout_editor[0][0]).GetBoundingRect().CenterPoint();

		parent.layout_editor[0][0]->m_oldPt = pt;
		pt.Offset(3,3);

		parent.layout_editor[0][0]->InitializeMove(pt);
	}
}