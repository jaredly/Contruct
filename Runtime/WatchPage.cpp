// MyPropertyPages.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "WatchPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CWatchPage, CResizablePage)

/////////////////////////////////////////////////////////////////////////////
// CWatchPage property page

CWatchPage::CWatchPage() : CResizablePage(CWatchPage::IDD)
{
	//{{AFX_DATA_INIT(CWatchPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	active = false;
	pausedUpdating = false;
}

CWatchPage::~CWatchPage()
{
}

void CWatchPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWatchPage)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_WATCH, m_List);
}

BEGIN_MESSAGE_MAP(CWatchPage, CResizablePage)
	//{{AFX_MSG_MAP(CWatchPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_KEYDOWN, IDC_WATCH, &CWatchPage::OnWatchKeydown)
	ON_NOTIFY(NM_RCLICK, IDC_WATCH, &CWatchPage::OnRclickWatch)
	ON_NOTIFY(NM_CLICK, IDC_WATCH, OnClick)
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_EDIT_END, OnEditEnd)
END_MESSAGE_MAP()

BOOL CWatchPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_List.InsertColumn(0, "Source", 0, 80);
	m_List.InsertColumn(1, "Item", 0, 100);
	m_List.InsertColumn(2, "Value", 0, 100);

	AddAnchor(IDC_WATCH, TOP_LEFT, BOTTOM_RIGHT);

	return TRUE;
}

BOOL CWatchPage::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_NEXT);
	active = true;
	
	return CResizablePage::OnSetActive();
}

BOOL CWatchPage::OnKillActive()
{
	active = false;
	return CResizablePage::OnKillActive();
}

void CWatchPage::RefreshWatch()
{
	if (!active || pausedUpdating) return;

	vector<WatchLine> curDisplay;
	ExtDebugInfo curDebug;
	ExpStore* unused;

	// Iterate watch objects
	Watch::iterator i = watch.begin();
	Watch::iterator watch_end = watch.end();

	// Delete destroying objects
	for ( ; i != watch_end; ) {
		bool increment = true;
		switch (i->first.type) {
		case WATCH_INSTANCE:
		case WATCH_INSTANCEPRIVATEVAR:
			if (i->first.obj->info.destroying) {
				i = watch.erase(i);
				increment = false;
			}
		}

		if (increment)
			i++;
	}

	for (i = watch.begin(); i != watch_end; i++) {

		// Get a debug list from this object
		curDebug.resize(0);
		pRuntime->curDebugOutput = &curDebug;

		CString source;

		switch(i->first.type) {
		case WATCH_OBJTYPE:
			source = i->first.objType->Name + " *";
			pRuntime->AddDebuggerItem("Instances", (int)i->first.objType->instances.size(), true);
			break;
		case WATCH_INSTANCE:
			{
			CRunObject* curObj = i->first.obj;
			CRunObjType* curType = curObj->pType;

			if (curObj == &pRuntime->system)
				source = "System";
			else if (curType->movement) {
				source.Format("%s #%d", curType->AuxName, find_index(curType->instances.begin(), curType->instances.end(), curObj) + 1);
			}
			else
				source.Format("%s #%d", curType->Name, find_index(curType->instances.begin(), curType->instances.end(), curObj) + 1);
		
			pObjectsPage->AddCommonInspectItems(curObj);
			curObj->DebuggerUpdateDisplay(unused);
			}
			break;
		case WATCH_INSTANCEPRIVATEVAR:
			{
			CRunObject* curObj = i->first.obj;
			CRunObjType* curType = curObj->pType;

			source.Format("%s #%d", curType->Name, find_index(curType->instances.begin(), curType->instances.end(), curObj) + 1);
		
			pRuntime->AddDebuggerItem(CString("'") + curType->privateVars[i->first.pvIndex].name + "'",
								i->first.pPrivateVars[i->first.pvIndex].GetString(), false);
			}
			break;
		case WATCH_GLOBALVAR:
			{
			int globalIndex = i->first.pvIndex;

			source = "(System)";
			pRuntime->AddDebuggerItem(CString("'") + pRuntime->globalNames[globalIndex] + "'",
								pRuntime->globalVars[globalIndex].GetString(), false);
			}
			break;
		}
		
		// Iterate all keys this object has being watched
		KeyList::iterator k = i->second.begin();
		KeyList::iterator keys_end = i->second.end();

		for ( ; k != keys_end; k++) {

			// Generate a watch line and add it
			curDisplay.push_back(WatchLine());
			WatchLine& wl = curDisplay.back();
			wl.label = k->label;
			wl.objIdent = source;
			ExtDebugInfo::iterator d = find(curDebug.begin(), curDebug.end(), k->label);
			wl.value = d->value;
			wl.readOnly = d->readOnly;

			wl.pItem = k;
			wl.pKey = i;
		}
	}

	// Full refresh
	if (curDisplay.size() != lastDisplay.size()) {
		m_List.SetRedraw(false);
		m_List.DeleteAllItems();

		vector<WatchLine>::iterator w = curDisplay.begin();

		for ( ; w != curDisplay.end(); w++) {
			int curLine = m_List.GetItemCount();
			m_List.InsertItem(curLine, w->objIdent);
			m_List.SetItemText(curLine, 1, w->label);
			m_List.SetItemText(curLine, 2, w->value);

			if (w->readOnly)
				m_List.SetItemColors(curLine, 2, GetSysColor(COLOR_GRAYTEXT), GetSysColor(COLOR_WINDOW));
			else
				m_List.SetEdit(curLine, 2);
		}

		m_List.SetRedraw(true);
		m_List.UpdateWindow();
	}
	// Else item update
	else {
		m_List.SetRedraw(false);

		vector<WatchLine>::iterator w = curDisplay.begin();
		vector<WatchLine>::iterator x = lastDisplay.begin();
		int index = 0;

		for ( ; w != curDisplay.end(); w++, x++, index++) {
			if (w->label != x->label)
				m_List.SetItemText(index, 1, w->label);
			if (w->objIdent != x->objIdent)
				m_List.SetItemText(index, 0, w->objIdent);
			if (w->value != x->value)
				m_List.SetItemText(index, 2, w->value);
		}

		m_List.SetRedraw(true);
		m_List.UpdateWindow();
	}

	lastDisplay = curDisplay;
}

void CWatchPage::OnWatchKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	*pResult = 0;

	// Hide from watch
	if (pLVKeyDow->wVKey == VK_DELETE) {
		int selLine = m_List.GetSelectionMark();

		if (selLine >= 0 && selLine < m_List.GetItemCount()) {
			WatchLine& wl = lastDisplay[selLine];

			// Remove this watch item
			wl.pKey->second.erase(wl.pItem);

			// No more items: remove the object item
			if (wl.pKey->second.empty())
				watch.erase(wl.pKey);
		}
	}
	//else if (pLVKeyDow->wVKey == VK_RETURN)
		pausedUpdating = false;
}

void CWatchPage::OnRclickWatch(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	CMenu menu;
	menu.LoadMenu(MAKEINTRESOURCE(IDR_WATCHCONTEXT));

	int selLine = m_List.GetSelectionMark();
	if (selLine < 0 || selLine >= m_List.GetItemCount())
		menu.GetSubMenu(0)->EnableMenuItem(ID_REMOVE, MF_BYCOMMAND | MF_GRAYED);

	POINT p;
	GetCursorPos(&p);
	switch (menu.GetSubMenu(0)->TrackPopupMenuEx(TPM_RETURNCMD | TPM_LEFTALIGN, p.x, p.y, &m_List, NULL))
	{
	case ID_REMOVE:
		{
		WatchLine& wl = lastDisplay[selLine];

		// Remove this watch item
		wl.pKey->second.erase(wl.pItem);

		// No more items: remove the object item
		if (wl.pKey->second.empty())
			watch.erase(wl.pKey);

		break;
		}
	case ID_REMOVEALL:
		watch.clear();
		break;
	}
}

LRESULT CWatchPage::OnEditEnd(WPARAM nItem, LPARAM nSubItem)
{
	// Get item label
	CString label = m_List.GetItemText(nItem, 1);
	CString text = m_List.GetItemText(nItem, nSubItem);

	__int64 iValue = _atoi64(text);
	double fValue = atof(text);

	// Resume updating
	pausedUpdating = false;

	WatchLine& wl = lastDisplay[nItem];

	CRunObject* pMyObject = wl.pKey->first.obj;

	// Check for common labels
	if (wl.pKey->first.type == WATCH_INSTANCE || wl.pKey->first.type == WATCH_INSTANCEPRIVATEVAR && pMyObject != NULL) {

		if (label == "X")
			pMyObject->info.x = fValue;
		else if (label == "Y")
			pMyObject->info.y = fValue;
		else if (label == "Width")
			pMyObject->info.w = fValue;
		else if (label == "Height")
			pMyObject->info.h = fValue;
		else if (label == "Angle")
			pMyObject->info.angle = fValue;
		else if (label == "Opacity") {
			pMyObject->info.pInfo->filter.a = fValue / 100.0;
		}
		
		pMyObject->OnDebuggerValueChanged(label, text);

		pRuntime->UpdateBoundingBox(pMyObject);
	}

	// Check for system object
	if (pMyObject->pType == NULL) {
		pMyObject->OnDebuggerValueChanged(label, text);
	}

	// Save scroll position
	int nScroll = m_List.GetScrollPos(SB_VERT);

	// Force full refresh by clearing old display
	lastDisplay.resize(0);
	RefreshWatch();

	// Restore scroll position
	m_List.SetSelectionMark(nItem);
	m_List.SetCurSel(nItem, TRUE);
	m_List.SetScrollPos(SB_VERT, nScroll, TRUE);

	return 0;
}

void CWatchPage::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;
	}

	pausedUpdating = false;

	// Edit columns clicked
	if (nItem >= 0 && nSubItem == 2)
	{
		COLORREF text;
		COLORREF bg;
		m_List.GetItemColors(nItem, 2, text, bg);

		if (text != GetSysColor(COLOR_GRAYTEXT) && !lastDisplay[nItem].readOnly != NULL) {
			// User clicked an edit button.  Pause refreshing to prevent glitches.
			pausedUpdating = true;
		}
	}

	*pResult = 0;
}