/////////////////////////////////////////////////////////////////////////////
// Insert object Dialog
//
// Last revision: 1.1
// October 2008
//
// Changes:
// - Rewrote dialog
//
/////////////////////////////////////////////////////////////////////////////

// File includes
#include "StdAfx.h"
#include "..\Construct.h"
#include "..\Utilities\QArray.h"
#include "Insert Object Dlg.h"
#include <shlwapi.h>
#include "ExchangeDlg.h"
#include <commctrl.h>

// Object information
#include "Object Info Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\MainFrm.h"

extern CPluginMap g_Plugins;

static int CALLBACK 
MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   // lParamSort contains a pointer to the list view control.
   CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
   CString    strItem1 = pListCtrl->GetItemText(lParam1, 0);
   CString    strItem2 = pListCtrl->GetItemText(lParam2, 0);

   return strcmp(strItem2, strItem1);
}

/////////////////////////////////////////////////////////////////////////////
// InsertObjectDialog dialog

InsertObjectDialog::InsertObjectDialog(CApplication& application_)
	: CExtNCW<CExtResizableDialog>(InsertObjectDialog::IDD, NULL),
	application(application_)
{
	m_curSel = -1;
	onclicklisttimeractivated = false;
}

void InsertObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_GETMORE, exchange);
	DDX_Control(pDX, IDC_OBJECTS, objects);
	DDX_Control(pDX, IDOK, ok);
	DDX_Control(pDX, IDCANCEL, cancel);
	DDX_Control(pDX, IDC_LISTOBJECTS, list_objects);
	DDX_Control(pDX, IDC_INFO, info);
	DDX_Control(pDX, IDC_FILTER, filter);
	DDX_Control(pDX, IDC_NAME, name);
	DDX_Control(pDX, IDC_FOLDER, folders);
}

BEGIN_MESSAGE_MAP(InsertObjectDialog, CExtResizableDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OBJECTS, OnClickList1)
	ON_BN_CLICKED(IDC_LISTOBJECTS, OnChangeObjectView)
	ON_BN_CLICKED(IDC_GETMORE, OnExchange)
	ON_NOTIFY(NM_DBLCLK, IDC_OBJECTS, OnDblclkList1)
	ON_NOTIFY(WM_LBUTTONUP, IDC_OBJECTS, OnSelected)

	ON_WM_SIZE()
	ON_WM_DESTROY()

	ON_NOTIFY(NM_RCLICK, IDC_OBJECTS, OnRClickList)
	ON_LBN_DBLCLK(IDC_CATEGORIES, OnChangeCategory)
	ON_LBN_SELCHANGE(IDC_CATEGORIES, OnSelchangeCategories)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_FILTER, &InsertObjectDialog::OnBnClickedFilter)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// InsertObjectDialog message handlers

extern CString gInsert;
extern int   gInsertAuto;
extern CString insert_name;
extern CString insert_folder;

BOOL InsertObjectDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Add resizing
	BOOL First = dlgMan.Load(m_hWnd, "Software\\Construct\\InsertObjectDialog099");
	if (!First)
	{
		first_run.Attach(this);
		first_run.SetHighlightOnMouseOver();
		first_run.SetResize();
		first_run.SetText(_T(IO_FIRSTRUN));
	}

    dlgAnchor.Init(m_hWnd);

	dlgAnchor.Add(IDOK, ANCHOR_RIGHT | ANCHOR_TOP);
	dlgAnchor.Add(IDCANCEL, ANCHOR_RIGHT | ANCHOR_TOP);
	dlgAnchor.Add(IDC_LISTOBJECTS, ANCHOR_RIGHT | ANCHOR_TOP);
	dlgAnchor.Add(IDC_GETMORE, ANCHOR_RIGHT | ANCHOR_TOP);
	dlgAnchor.Add(IDC_FILTER, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDTOPTEXT, ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_TOP);

	dlgAnchor.Add(IDC_NAME, ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_RIGHT);
	dlgAnchor.Add(IDC_FOLDER, ANCHOR_TOP | ANCHOR_RIGHT | ANCHOR_BOTTOM);

	dlgAnchor.Add(IDC_OBJECTS, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(IDC_INFO, ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_TOOLBOX, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(10, ANCHOR_LEFT | ANCHOR_BOTTOM | ANCHOR_RIGHT);

	CRect Rect;
	objects.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	Rect.top = Rect.bottom - 1;
	Rect.bottom = Rect.top + 16;
	
	tabs.Create(this, Rect, 1000, WS_CHILD | WS_VISIBLE, __ETWS_ORIENT_BOTTOM);

	dlgAnchor.Add(tabs, ANCHOR_LEFT | ANCHOR_BOTTOM);
	
	tabs.ItemInsert(OT_ALL, 0, true, 0, 1);
	tabs.ItemInsert(OT_AUDIO, 0, true,0, 2);
	tabs.ItemInsert(OT_CONTROLS, 0, true,0, 3);
	tabs.ItemInsert(OT_DATAFILES, 0, true, 0, 4);
	tabs.ItemInsert(OT_GAME, 0, true, 0, 5);
	tabs.ItemInsert(OT_GRAPHICS, 0, true, 0, 6);
	tabs.ItemInsert(OT_INPUT, 0, true,0, 7);
	tabs.ItemInsert(OT_INTERNET, 0, true, 0, 8);
	tabs.ItemInsert(OT_OTHER, 0, true, 0, 9);
	tabs.ItemInsert(OT_SYSTEM, 0, true, 0, 10);
	tabs.SelectionSet(0);

	// Load objects
	LoadObjects();
	ListView_SetImageList(objects.m_hWnd, m_LargeImages, LVSIL_NORMAL);		
	ListView_SetImageList(objects.m_hWnd, small_images, LVSIL_SMALL);	

	// Name of dialog
	CString dialogTitle;
	dialogTitle.Format("Construct : %s", IO_TITLE);
	SetWindowText(dialogTitle);

	// Set texts
	cancel.SetWindowText(CANCEL);
	ok.SetWindowText(INSERT);
	ok.EnableWindow(FALSE);
	exchange.SetWindowText(IO_EXCHANGE);
	info.SetWindowText(IO_CLICKFORINFO);
	list_objects.SetWindowText(IO_LIST_OBJECTS);
	filter.SetWindowText(IO_SEARCH);

	OnChangeCategory();

	if (g_MainFrame->m_INI.GetInt("InsertObject", "List", 0) == 1)
	{
		list_objects.SetCheck(BST_CHECKED);
		OnChangeObjectView();
	}

	objects.SetScrollPos(SB_VERT, 0);
	objects.SetScrollPos(1, 0);
	objects.Invalidate();

	// fill object folders
	vector<CApplication::ObjectFolder>::iterator folder = application.object_folders.begin();

	for ( ; folder != application.object_folders.end(); folder++)
		folders.AddString(folder->name);

	// select default
	folders.SetCurSel(0);

	// Refresh
	SetTimer(10, 200, NULL);

	return TRUE; 
}

void InsertObjectDialog::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 10)
	{
		KillTimer(10);

		objects.SetScrollPos(SB_VERT, 0);
		objects.SetScrollPos(1, 0);
		objects.Invalidate();
	}

	else
	{
		onclicklisttimeractivated = false;
		KillTimer(500);
		CString give;
		name.GetWindowText(give);
		name.SetSel(0, give.GetLength());
		name.SetFocus();
	}
}

void InsertObjectDialog::OnExchange()
{
	CExchangeDlg Dlg;
	Dlg.DoModal();
}

void InsertObjectDialog::OnOK() 
{
	OnDblclkList1(0,0);

	CDialog::OnOK();
}

void InsertObjectDialog::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (ok.IsWindowEnabled()) 
	{
		gInsert = "";
		POSITION pos = objects.GetFirstSelectedItemPosition();

		if (pos != NULL)
		{
			while (pos) 
			{
				int nItem = objects.GetNextSelectedItem(pos);

				LVITEM lvi;
				lvi.mask = LVIF_PARAM;
				lvi.iItem = nItem;
				lvi.iSubItem = 0;

				if (ListView_GetItem(objects.m_hWnd, &lvi)) 
				{
					CPlugin Plugin = GetPluginByFile((const char*)lvi.lParam);
					gInsert = Plugin.m_FileName;

					name.GetWindowText(insert_name);
					folders.GetLBText(folders.GetCurSel(), insert_folder);

					bool bGame = false;
					if (application.runtime != CApplication::rt_application) bGame = true;

					// common objects
					long count = g_MainFrame->m_INI.GetInt(bGame ? "RecentObjects" : "RecentObjectsApp", Plugin.m_FileName, 0);
					g_MainFrame->m_INI.WriteInt(bGame ? "RecentObjects" : "RecentObjectsApp", Plugin.m_FileName, count + 1);

					if (Plugin.m_Flags & OF_NODRAW)
						gInsertAuto = 1;				
				}
			}
		}

		EndDialog(0);
	}
}

void InsertObjectDialog::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = objects.GetFirstSelectedItemPosition();

	if (pos == NULL) 
	{
		if (m_curSel != -1) 
		{
			m_curSel = -1;
			info.SetWindowText(IO_CLICKFORINFO);
			
			ok.EnableWindow(FALSE);
		}
	}

	else 
	{
		while (pos) 
		{
			int nItem = objects.GetNextSelectedItem(pos);
			if (nItem != m_curSel) 
			{
				m_curSel = nItem;
				LVITEM lvi;
				lvi.mask = LVIF_PARAM;
				lvi.iItem = nItem;
				lvi.iSubItem = 0;
				if (ListView_GetItem(objects.m_hWnd, &lvi)) 
				{
					// New, use CPlugin
					CPlugin Plugin = GetPluginByFile((const char*)lvi.lParam);

					ok.EnableWindow(TRUE);
				
					CString Text;
					Text.Format("%s: %s\r\n%s: %s\r\n%s: %s", IO_AUTHOR, Plugin.m_Author, IO_VERSION, Plugin.m_Version, IO_DESCRIPTION, Plugin.m_Description);
					info.SetWindowText(Text);

					CString give = Plugin.m_Name;

					give = application.GetUniqueObjectTypeName(give);

					name.SetWindowText(give);

					SetTimer(500, 1000, NULL);
				}

				else 
					ok.EnableWindow(FALSE);
			}

			break;
		}
	}

	*pResult = 0;
}

void InsertObjectDialog::OnSelected(NMHDR* pNMHDR, LRESULT* pResult)
{
	CString give;
	name.GetWindowText(give);
	name.SetSel(0, give.GetLength());
	name.SetFocus();
}

void InsertObjectDialog::OnChangeObjectView() 
{
	if (list_objects.GetCheck())
	{
		::SetWindowLong(objects.GetSafeHwnd(), GWL_STYLE,
		::GetWindowLong(objects.GetSafeHwnd(), GWL_STYLE) | LVS_SMALLICON);
	}

	else
	{
		::SetWindowLong(objects.GetSafeHwnd(), GWL_STYLE,
		::GetWindowLong(objects.GetSafeHwnd(), GWL_STYLE) & ~(LVS_SMALLICON));
	}
}

void InsertObjectDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dlgAnchor.OnSize();	

	Invalidate();
}

void InsertObjectDialog::OnDestroy() 
{
	first_run.Detach();

	ListView_SetImageList(objects.m_hWnd, NULL, LVSIL_SMALL);
	ListView_SetImageList(objects.m_hWnd, NULL, LVSIL_NORMAL);

	if (list_objects.GetCheck() == BST_CHECKED)
		g_MainFrame->m_INI.WriteInt("InsertObject", "List", 1);
	else
		g_MainFrame->m_INI.WriteInt("InsertObject", "List", 0);

	CDialog::OnDestroy();
	
	dlgMan.Save();
}

void InsertObjectDialog::OnRClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION listpos = objects.GetFirstSelectedItemPosition();
	int item = objects.GetNextSelectedItem(listpos);
	CString itemtext = objects.GetItemText(item, 0);

	if (itemtext == "") return;

	// Mouse position
	POINT cursorPosition;
	GetCursorPos(&cursorPosition);

	CExtPopupMenuWnd * popup =  new CExtPopupMenuWnd;
	popup->LoadMenu(m_hWnd, IDR_OBJECT, true, false);

	UINT ChosenItem = 0;
	popup->TrackPopupMenu(TPMX_DO_MESSAGE_LOOP|TPMX_NO_WM_COMMAND|TPMX_NO_CMD_UI, cursorPosition.x, cursorPosition.y, NULL, NULL, NULL, &ChosenItem);

	// Open info dialog
	if (ChosenItem == ID_OBJECTINFO)
	{
		CObjectInfoDlg objInfo;
		objInfo.objname = itemtext;
		objInfo.DoModal();
	}	

	// Open website address
	else if (ChosenItem == ID_MENU_VISITURL)
	{
		CPlugin Plugin = GetPluginByFile((const char*)objects.GetItemData(item));
		ShellExecute(NULL, "open", Plugin.m_WebLink, NULL, NULL, NULL);
	}

	*pResult = 0;
}

void InsertObjectDialog::LoadObjects()
{
	// Create imagelists
	m_LargeImages.Create(32, 32, ILC_COLOR24, 0, 10);
	small_images.Create(16, 16, ILC_COLOR24, 0, 10);

	map<int, CPlugin>::iterator i = g_Plugins.begin();

	for ( ; i != g_Plugins.end(); i++) 
	{
		CString lowerName = i->second.m_Name;
		lowerName.MakeLower();

		if (!i->second.m_bMovement && lowerName != "tag")
		{
			bool bGame = false;
			if (application.runtime != CApplication::rt_application) bGame = true;

			if (!bGame && i->second.m_Category == OT_GAME)
				continue;

			// Extract the icon
			ImageList_Add(m_LargeImages, i->second.m_LargeIcon, NULL);
			ImageList_Add(small_images, i->second.m_SmallIcon, NULL);
		}
	}
}

void InsertObjectDialog::OnChangeCategory() 
{
	bool bGame = false;
	if (application.runtime != CApplication::rt_application) bGame = true;

	int CategoryID = tabs.SelectionGet();
	CString SelectedCategory;

	// Find category to use
	if ((CategoryID != 0) && (CategoryID != -1))
	{
		SelectedCategory = tabs.ItemGet(CategoryID)->TextGet();
		SelectedCategory.Replace("&&", "&");
		objects.RemoveAllGroups();
	}

	// Otherwise show all
	else
	{
		objects.AddGroup(0, OT_COMMON);
		objects.AddGroup(1, OT_AUDIO);
		objects.AddGroup(2, OT_CONTROLS);
		objects.AddGroup(3, "Data & Files"); // Why doesn't it work!
		if (bGame) objects.AddGroup(4, OT_GAME);
		objects.AddGroup(5, OT_GRAPHICS);
		objects.AddGroup(6, OT_INPUT);
		objects.AddGroup(7, OT_INTERNET);
		objects.AddGroup(8, OT_OTHER);
		objects.AddGroup(9, OT_PROFESSIONAL);
		objects.AddGroup(10, OT_SYSTEM);
	}

	SelectedCategory.MakeLower();
	objects.DeleteAllItems();

	bool bAddPlugin = false;
	if (CategoryID == 0 || CategoryID == -1) bAddPlugin = true;

	// Prepare common object selection
	CQArray <intstr, intstr&> intstrArr;
	CStringArray oArray;
	g_MainFrame->m_INI.GetKeyNames(bGame ? "RecentObjects" : "RecentObjectsApp", &oArray);
	for (int i = 0; i < oArray.GetCount(); i++)
	{
		intstr obj;
		obj.variable = oArray.GetAt(i);
		obj.id = g_MainFrame->m_INI.GetInt(bGame ? "RecentObjects" : "RecentObjectsApp", obj.variable, 0);
		intstrArr.Add(obj);
	}

	intstrArr.QuickSort(FALSE); 

	map<int, CPlugin>::iterator i = g_Plugins.begin();

	// Image index
	int Index = 0;
	int Image = 0;

	// Loop all objects
	for ( ; i != g_Plugins.end(); i++) 
	{
		CString lowerName = i->second.m_Name;
		lowerName.MakeLower();

		if (i->second.m_bMovement || lowerName == "tag") 
		{
			Index++;
			continue;
		}

		CString Category = i->second.m_Category;
		Category.MakeLower();

		// We want this object
		if (Category.Find(SelectedCategory, 0) != -1 || bAddPlugin)
		{
			// Add this plugin
			LVITEM InsertPlugin;

			if (CategoryID == 0)
			{
				InsertPlugin.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE | LVIF_GROUPID;
				InsertPlugin.state = 0;
				InsertPlugin.stateMask = 0;
				InsertPlugin.iItem = ListView_GetItemCount(objects.m_hWnd) - 1;
				InsertPlugin.iImage = Image;
				InsertPlugin.iSubItem = 0;
				InsertPlugin.lParam = (LPARAM)((const char*)i->second.m_FileName);
				InsertPlugin.pszText = i->second.m_Name.GetBuffer(0);

				// Now find a group
				InsertPlugin.iGroupId = -1;

				// Audio
				if (Category.Find(OT_AUDIO.MakeLower(), 0) != -1) 
					InsertPlugin.iGroupId = 1;

				// Controls
				if (Category.Find(OT_CONTROLS.MakeLower(), 0) != -1) 
					InsertPlugin.iGroupId = 2;

				// Data and files
				if (Category.Find("data & files", 0) != -1) 
					InsertPlugin.iGroupId = 3;

				// Game
				if (Category.Find(OT_GAME.MakeLower(), 0) != -1)
					if (!bGame)
					{
						Index++;
						continue;
					}
					else
						InsertPlugin.iGroupId = 4;

				// Graphics
				if (Category.Find(OT_GRAPHICS.MakeLower(), 0) != -1) 
					InsertPlugin.iGroupId = 5;

				// Input
				if (Category.Find(OT_INPUT.MakeLower(), 0) != -1) 
					InsertPlugin.iGroupId = 6;

				// Internet
				if (Category.Find(OT_INTERNET.MakeLower(), 0) != -1) 
					InsertPlugin.iGroupId = 7;

				// Other
				if (Category.Find(OT_OTHER.MakeLower(), 0) != -1) 
					InsertPlugin.iGroupId = 8;

				// Professional
				if (Category.Find(OT_PROFESSIONAL.MakeLower(), 0) != -1) 
					InsertPlugin.iGroupId = 9;

				// System
				if (Category.Find(OT_SYSTEM.MakeLower(), 0) != -1) 
					InsertPlugin.iGroupId = 10;

				// Is it a common object?
				for (int j = 0; j < min(intstrArr.GetCount(), 8); j++) 
				{
					if (!_stricmp(i->second.m_FileName, intstrArr[j].variable))
					{ 
						// This is a commonly used object
						if (InsertPlugin.iGroupId != -1) {

							if (lowerName != "directsound")
								ListView_InsertItem(objects.m_hWnd, &InsertPlugin);
						}

						InsertPlugin.iGroupId = 0;
					}
				}

				// Do not insert Directsound - it still exists for loading old .caps, but can't be inserted
				if (lowerName != "directsound")
					ListView_InsertItem(objects.m_hWnd, &InsertPlugin);
			}

			else
			{
				InsertPlugin.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
				InsertPlugin.state = 0;
				InsertPlugin.stateMask = 0;
				InsertPlugin.iItem = ListView_GetItemCount(objects.m_hWnd);
				InsertPlugin.iImage = Image;
				InsertPlugin.iSubItem = 0;
				InsertPlugin.lParam = (LPARAM)((const char*)i->second.m_FileName);
				InsertPlugin.pszText = i->second.m_Name.GetBuffer(0);
				ListView_InsertItem(objects.m_hWnd, &InsertPlugin);
			}
		}

		Index++;

		if (!bGame && Category != OT_GAME.MakeLower())
			Image++;

		if (bGame)
			Image++;
	}

	if (CategoryID == 0 || CategoryID == -1)
		objects.EnableGroups();

	// Sort the list view items
	objects.SortItems(MyCompareProc, (LPARAM)&objects);

	objects.SetScrollPos(0, 0);
	objects.SetScrollPos(SB_VERT, 0);
}

void InsertObjectDialog::OnSelchangeCategories() 
{

	OnChangeCategory();
}

BOOL InsertObjectDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) 
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			OnOK();
			return TRUE;
		default:
			if(onclicklisttimeractivated)
			{
				KillTimer(500);
				SetTimer(500, 1000, NULL);

			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void InsertObjectDialog::OnBnClickedFilter()
{
	bool bGame = false;
	if (application.runtime != CApplication::rt_application) bGame = true;

	if (filter_dialog.m_hWnd)
		filter_dialog.DestroyWindow();

	CPoint Pt;
	::GetCursorPos(&Pt);

	filter_dialog.m_bGame = bGame;
	filter_dialog.Create(IDD_FILTER, this);
	filter_dialog.SetWindowPos(NULL, Pt.x, Pt.y, -1, -1, SWP_NOSIZE);
	filter_dialog.ShowWindow(SW_SHOW);
}

void InsertObjectDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (filter_dialog.m_hWnd)
		filter_dialog.DestroyWindow();

	CExtNCW<CExtResizableDialog>::OnLButtonUp(nFlags, point);
}
