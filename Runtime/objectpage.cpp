// MyPropertyPages.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "uxtheme.h"
#include "ObjectPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CObjectsPage, CResizablePage)

/////////////////////////////////////////////////////////////////////////////
// CObjectsPage property page

CObjectsPage::CObjectsPage() : CResizablePage(CObjectsPage::IDD)
{
	//{{AFX_DATA_INIT(CObjectsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	pMyObject = NULL;
	pMyType = NULL;
	inspectArea = INSPECT_PROPERTIES;
	pCurrentVars = NULL;
	pCurMovement = NULL;
	pCurLineOwner = NULL;
	pausedUpdating = false;
	suspended = false;
	invalidateNextRefresh = true;	// Initial invalidate to show + buttons
}

CObjectsPage::~CObjectsPage()
{
}

void CObjectsPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectsPage)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TREE, m_ItemTree);
	DDX_Control(pDX, IDC_PAUSE, m_PauseBtn);
	DDX_Control(pDX, IDC_STEP, m_Step);
	DDX_Control(pDX, IDC_FRAMES, m_FramesCombo);
}


BEGIN_MESSAGE_MAP(CObjectsPage, CResizablePage)
	//{{AFX_MSG_MAP(CObjectsPage)
	//ON_NOTIFY(NM_RCLICK, IDC_TREE, OnRButtonDown)
	ON_BN_CLICKED(IDC_RESTART, OnRestart)
	ON_BN_CLICKED(IDC_STEP, OnStep)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClick)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CObjectsPage::OnTvnSelChangedTree)
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_EDIT_END, OnEditEnd)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, &CObjectsPage::OnRClickList)
	ON_NOTIFY(NM_RCLICK, IDC_TREE, &CObjectsPage::OnRClickTree)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST, &CObjectsPage::OnListKeydown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CObjectsPage::OnDoubleClickList)
	ON_CBN_SELCHANGE(IDC_FRAMES, &CObjectsPage::OnSelchangeFrames)
END_MESSAGE_MAP()


BOOL CObjectsPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	m_List.SubclassDlgItem(IDC_LIST, this);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_List.InsertColumn(0, "Item", LVCFMT_LEFT, 100);
	m_List.InsertColumn(1, "Value", LVCFMT_LEFT, 100);

	// Create initial objects tree
	m_ItemTree.InsertItem("System");

	// Get all object types and display them
	ObjTypeIterator t = pRuntime->objects.begin();
	ObjTypeConstIterator objects_end = pRuntime->objects.end();

	vector<CString> movementNames;

	for ( ; t != objects_end; t++) {

		// Movement and group types are not shown in the tree, or if No Debug is set.
		if ((*t)->movement || (*t)->teamID > -1 || (*t)->pPlugin->ideFlags & OF_NODEBUG)
			continue;

		HTREEITEM hTypeParent = m_ItemTree.InsertItem((*t)->Name);
		typeToItemMap[(*t)->Name] = hTypeParent;
	}

	vector<CRunLayout*>::iterator f = pRuntime->Frames.begin();

	for ( ; f != pRuntime->Frames.end(); f++)
		m_FramesCombo.AddString((*f)->name);

	m_FramesCombo.SetCurSel(0);

	AddAnchor(IDC_PAUSE, BOTTOM_LEFT);
	AddAnchor(IDC_STEP, BOTTOM_LEFT);
	AddAnchor(IDC_RESTART, BOTTOM_LEFT);
	AddAnchor(IDC_FRAMES, BOTTOM_RIGHT);
	AddAnchor(IDC_LIST, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TREE, TOP_LEFT, BOTTOM_LEFT);
	
	return TRUE;
}

BOOL CObjectsPage::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_NEXT);
	
	return CResizablePage::OnSetActive();
}

void CObjectsPage::OnRButtonDown(NMHDR* pNMHDR, LRESULT* pResult)
{
}

void CObjectsPage::OnPause()
{
	if (suspended) {
		m_PauseBtn.SetWindowText("Pause");
		pRuntime->SetSuspended(false);
		suspended = false;
		m_Step.EnableWindow(false);
	}
	else {
		m_PauseBtn.SetWindowText("Resume");
		pRuntime->SetSuspended(true);
		suspended = true;
		m_Step.EnableWindow();
	}
}

void CObjectsPage::OnRestart()
{
	//TODO
}

void CObjectsPage::OnStep()
{
	// Force a single DoFrame
	if (suspended) {
		pRuntime->SetSuspended(false);
		pRuntime->Frame();
		pRuntime->SetSuspended(true);
	}
}

void CObjectsPage::OnTvnSelChangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;

	CString objName = m_ItemTree.GetItemText(hItem);
	
	// System object selected
	if (objName == "System") {
		SetObjectToInspect(&(pRuntime->system), INSPECT_PROPERTIES);
		return;
	}

	// Else check for object type name clicked
	CRunObjType* pType = pRuntime->GetTypeFromName(objName);

	// Name found: inspect type
	if (pType) {
		SetTypeToInspect(pType);
		return;
	}

	// Check if standard items (Common), (Properties) or (Private Variables) were picked
	ObjectInspectionArea area = INSPECT_NONE;
	CString movName;

	if (objName == "(Common)")
		area = INSPECT_COMMON;
	else if (objName == "(Properties)")
		area = INSPECT_PROPERTIES;
	else if (objName == "(Private Variables)")
		area = INSPECT_PRIVATEVARS;
	// Check if it was a movement
	else if (objName.Left(1) == "(") {
		// Strip out brackets
		movName = objName.Mid(1, objName.GetLength() - 2);
		area = INSPECT_MOVEMENT;
	}

	if (area != INSPECT_NONE) {
		HTREEITEM hParent = m_ItemTree.GetParentItem(hItem);
		HTREEITEM hParentParent = m_ItemTree.GetParentItem(hParent);

		if (hParentParent != NULL) {
			objName = m_ItemTree.GetItemText(hParentParent);
			CRunObjType* pType = pRuntime->GetTypeFromName(objName);

			if (pType != NULL) {
				// Get item index
				CString strIndex = m_ItemTree.GetItemText(hParent);
				strIndex.Delete(0);
				int index = atoi(strIndex) - 1;

				CRunObject* pInstance = pType->instances[index];

				// If a movement was picked, inspect that instance.
				if (area == INSPECT_MOVEMENT) {
					ObjIterator o = pInstance->siblings.begin();
					ObjConstIterator siblings_end = pInstance->siblings.end();

					for ( ; o != siblings_end; o++) {
						if ((*o)->pType->movement && (*o)->pType->AuxName == movName) {
							SetObjectToInspect(*o, INSPECT_PROPERTIES);
							break;
						}
					}
				}
				// Otherwise just inspect the selected object
				else
					SetObjectToInspect(pInstance, area);

				return;
			}
		}

	}

	// Name not found: check parent for type, then find instance
	HTREEITEM hParent = m_ItemTree.GetParentItem(hItem);

	if (hParent != NULL) {

		objName = m_ItemTree.GetItemText(hParent);
		CRunObjType* pType = pRuntime->GetTypeFromName(objName);

		if (pType != NULL) {
			// Get item index
			CString strIndex = m_ItemTree.GetItemText(hItem);
			strIndex.Delete(0);
			int index = atoi(strIndex) - 1;

			SetObjectToInspect(pType->instances[index], INSPECT_ALL);
			return;
		}

	}
}

void CObjectsPage::SetObjectToInspect(CRunObject* obj, ObjectInspectionArea area)
{
	pMyObject = obj;
	pMyType = NULL;
	inspectArea = area;
	pCurrentVars = NULL;

	objDebugDisplay.resize(0);

	RefreshInspection();
}

void CObjectsPage::SetTypeToInspect(CRunObjType* type)
{
	pMyObject = NULL;
	pMyType = type;

	objDebugDisplay.resize(0);

	RefreshInspection();
}

void CObjectsPage::AddListLine(CString item, CString value, bool readOnly, bool bold, bool combo, int comboSel)
{
	int line = m_List.GetItemCount();

	m_List.InsertItem(line, item);

	// Combo box item?
	if (combo) {
		CStringArray sarr;
		
		int numElems = value.Replace("|", "|") + 1;

		for (int i = 0; i < numElems; i++) {
			CString token;
			AfxExtractSubString(token, value, i, '|');
			sarr.Add(token);
		}

		m_List.SetComboBox(line, 1, true, &sarr, 200, comboSel);
	}
	else {
		m_List.SetItemText(line, 1, value);

		// The bold headers neither disable or create edits
		if (!bold) {
			if (readOnly) {

				// Disable last line
				//m_List.SetEnabled(line, false);
				m_List.SetItemColors(line, 1, GetSysColor(COLOR_GRAYTEXT), GetSysColor(COLOR_WINDOW));
			}
			// Modifiable?
			else {
				m_List.SetEdit(line, 1);
			}
		}
		// Bold?
		else
			m_List.SetBold(line, 0, true);
	}
}

void CObjectsPage::AddListLine(CString item, int value, bool readOnly, bool bold)
{
	CString strValue;
	strValue.Format("%d", value);
	AddListLine(item, strValue, readOnly);
}

void CObjectsPage::AddListHeader(CString text)
{
	objDebugDisplay.push_back(DebuggerInspectionLine());
	DebuggerInspectionLine& dil = objDebugDisplay.back();
	dil.item = text;
	dil.item.Replace("&", "&&");
	dil.value = "";
	dil.readOnly = true;
	dil.bold = true;
	dil.pOwner = NULL;
	dil.combo = false;
}

void CRuntime::AddDebuggerItem(const char* label, const char* value, bool readOnly)
{
	curDebugOutput->push_back(DebuggerInspectionLine());
	DebuggerInspectionLine& dil = curDebugOutput->back();
	dil.item = label;
	dil.value = value;
	dil.readOnly = readOnly;
	dil.bold = false;
	dil.pOwner = pObjectPage->pCurLineOwner;
	dil.combo = false;
}

void CRuntime::AddDebuggerCombo(const char* label, const char* items, int sel)
{
	curDebugOutput->push_back(DebuggerInspectionLine());
	DebuggerInspectionLine& dil = curDebugOutput->back();
	dil.item = label;
	dil.value = items;
	dil.readOnly = false;
	dil.bold = false;
	dil.pOwner = pObjectPage->pCurLineOwner;
	dil.combo = true;
	dil.comboSel = sel;
}

bool DebugDisplayLabelsHaveChanged(const ExtDebugInfo& l, const ExtDebugInfo& r)
{
	if (l.size() != r.size()) return true;

	// Check for matching labels
	ExtDebugReadIterator iter1 = l.begin();
	ExtDebugReadIterator iter2 = r.begin();

	for ( ; iter1 != l.end(); iter1++, iter2++) {
		// Check the items in the combo are still the same
		if (iter1->combo) {
			if (iter1->value != iter2->value)
				return true;			
		}
		else if (iter1->item != iter2->item)
			return true;
	}

	// They all matched!
	return false;
}

void CObjectsPage::AddCommonInspectItems(CRunObject* pObj)
{
	OBJHEADER& info = pObj->info;

	pRuntime->AddDebuggerItem("X", info.x, false);
	pRuntime->AddDebuggerItem("Y", info.y, false);
	pRuntime->AddDebuggerItem("Width", info.w, false);
	pRuntime->AddDebuggerItem("Height", info.h, false);

	// #define OF_ALLOWANGLES 0x0004
	if (pObj->pType != NULL && pObj->pType->pPlugin->ideFlags & 0x00000004)
		pRuntime->AddDebuggerItem("Angle", info.angle, false);

	//pRuntime->AddDebuggerCombo("Layer", "One|Two|Three", 0);

	pRuntime->AddDebuggerItem("Opacity", info.pInfo->filter.a * 100.0f, false);
}

void CObjectsPage::AddPrivateVariableInspectItems()
{
	// Inspecting private variables: if no pointer, get and update.  We only need to get the pointer once.
	if (pCurrentVars == NULL) {
		pMyObject->DebuggerUpdateDisplay(pCurrentVars);

		// Still null:  quit.
		if (pCurrentVars == NULL)
			return;
	}

	int numVars = pMyObject->pType->privateVars.size();

	ExpStore* i = pCurrentVars;
	ExpStore* end = i + numVars;
	int index = 0;

	for ( ; i != end; i++, index++) {
		CString s;
		s.Format("'%s'", pMyObject->pType->privateVars[index].name);

		if (i->Type() == EXPTYPE_STRING)
			pRuntime->AddDebuggerItem(s, (const char*)*(i->GetStringPtr()), false);
		else
			pRuntime->AddDebuggerItem(s, i->GetString(), false);
	}
}

void CObjectsPage::RefreshInspection()
{
	// This prevents the updating items glitching over the item edit boxes.
	if (pausedUpdating)
		return;

	ExtDebugInfo oldDisplay = objDebugDisplay;

	// Direct output to this vector
	pRuntime->curDebugOutput = &objDebugDisplay;

	// Inspecting object?
	if (pMyObject) {

		objDebugDisplay.resize(0);

		switch (inspectArea) {
		case INSPECT_COMMON:
			pCurLineOwner = pMyObject;
			AddCommonInspectItems(pMyObject);
			break;
		case INSPECT_PROPERTIES:
			pCurLineOwner = pMyObject;
			pMyObject->DebuggerUpdateDisplay(pCurrentVars);
			break;
		case INSPECT_PRIVATEVARS:
			pCurLineOwner = pMyObject;
			AddPrivateVariableInspectItems();
			break;
		case INSPECT_ALL:

			if (!(pMyObject->pType->pPlugin->ideFlags & OF_NOCOMMONDEBUG)) {
				AddListHeader("Common");

				pCurLineOwner = pMyObject;
				AddCommonInspectItems(pMyObject);

				AddListHeader("");
			}

			AddListHeader("Properties");

			pCurLineOwner = pMyObject;
			pMyObject->DebuggerUpdateDisplay(pCurrentVars);

			if (pCurrentVars) {
				AddListHeader("");
				AddListHeader("Private Variables");

				AddPrivateVariableInspectItems();
			}

			// Get movements and show their props
			if (!pMyObject->siblings.empty()) {
				ObjIterator s = pMyObject->siblings.begin();
				ObjConstIterator siblings_end = pMyObject->siblings.end();

				for ( ; s != siblings_end; s++) {
					if ((*s)->pType->movement && !((*s)->pType->pPlugin->ideFlags & OF_NODEBUG)) {

						// Movement header
						AddListHeader("");
						AddListHeader((*s)->pType->AuxName);

						// Associate these lines with the movement instance instead
						pCurLineOwner = *s;
						(*s)->DebuggerUpdateDisplay(pCurrentVars);
					}
				}
			}
			break;
		}

		// Is destroying?  Don't try to show next frame!
		if (pMyObject->info.destroying)
			pMyObject = NULL;

	}
	// Inspecting type?
	else if (pMyType) {

		objDebugDisplay.resize(0);

		pRuntime->AddDebuggerItem("Instances", (int)pMyType->instances.size(), true);
	}

	// Nothing got added?  Add blank line to trick "DebugDisplayLabelsHaveChanged"
	if (objDebugDisplay.empty())
		AddListHeader("");

	// Check for differences in size or labels: full refresh
	if (DebugDisplayLabelsHaveChanged(oldDisplay, objDebugDisplay)) {

		m_List.SetRedraw(false);
		m_List.DeleteAllItems();
		lineOwners.resize(0);

		ExtDebugIterator i = objDebugDisplay.begin();

		for ( ; i != objDebugDisplay.end(); i++) {
			AddListLine(i->item, i->value, i->readOnly, i->bold, i->combo, i->comboSel);
			lineOwners.push_back(i->pOwner);
		}

		m_List.SetRedraw(true);
		m_List.UpdateWindow();
	}
	// Same labels still: we can just update the item texts of those that have changed
	else {

		m_List.SetRedraw(false);

		ExtDebugIterator i1 = objDebugDisplay.begin();
		ExtDebugIterator i2 = oldDisplay.begin();
		int index = 0;

		for ( ; i1 != objDebugDisplay.end(); i1++, i2++, index++) {

			// Combo differs
			if (i1->combo) {
				//if (i1->comboSel != i2->comboSel)
				//	;//TODO: update combo sel
			}
			// Values differ
			else if (i1->value != i2->value) {

				// Simply update the item text with the new value, or change the combo sel
				m_List.SetItemText(index, 1, i1->value);

			}
		}//for

		m_List.SetRedraw(true);
		m_List.UpdateWindow();

	}//labels changed
}

void CObjectsPage::UpdateObjectTree()
{
	// Objects created?
	if (!pRuntime->creationRow.empty()) {

		m_ItemTree.SetRedraw(false);
		ObjIterator i = pRuntime->creationRow.begin();
		ObjConstIterator creationRow_end = pRuntime->creationRow.end();

		for ( ; i != creationRow_end; i++) {

			// Find the item count and insert a new item
			HTREEITEM hItem = typeToItemMap[(*i)->pType->Name];
			if (hItem == NULL)
				continue;

			HTREEITEM hChild = m_ItemTree.GetChildItem(hItem);

			int count = 0;

			if (hChild != NULL) {
				HTREEITEM hNext = hChild;
				count = 1;
				while (hNext = m_ItemTree.GetNextItem(hChild, TVGN_NEXT)) {
					hChild = hNext;
					count++;
				}
			}

			CString itemText;
			itemText.Format("#%d", count + 1);
			HTREEITEM hInstanceParent = m_ItemTree.InsertItem(itemText, hItem);

			// Add "(Common)", "(Properties)", "(Private Variables)" and movement subitems where relevant
			if (!((*i)->pType->pPlugin->ideFlags & OF_NOCOMMONDEBUG))
				m_ItemTree.InsertItem("(Common)", hInstanceParent);

			m_ItemTree.InsertItem("(Properties)", hInstanceParent);

			if (!(*i)->pType->privateVars.empty())
				m_ItemTree.InsertItem("(Private Variables)", hInstanceParent);

			if (!(*i)->pType->sisterTypes.empty()) {
				ObjTypeIterator t = (*i)->pType->sisterTypes.begin();
				ObjTypeConstIterator sisters_end = (*i)->pType->sisterTypes.end();

				for ( ; t != sisters_end; t++) {
					if ((*t)->movement && !((*t)->pPlugin->ideFlags & OF_NODEBUG)) {
						CString movName;
						movName.Format("(%s)", (*t)->AuxName);
						m_ItemTree.InsertItem(movName, hInstanceParent);
					}
				}
			}
			m_ItemTree.SetRedraw(true);
		}

		pRuntime->creationRow.resize(0);
	}
	// Objects destroyed?
	if (!pRuntime->DeathRow.empty()) {
		ObjIterator i = pRuntime->DeathRow.begin();
		ObjConstIterator DeathRow_end = pRuntime->DeathRow.end();

		for ( ; i != DeathRow_end; ++i) {

			HTREEITEM hItem = typeToItemMap[(*i)->pType->Name];

			if (hItem == NULL)
				continue;

			HTREEITEM hChild = m_ItemTree.GetChildItem(hItem);
			HTREEITEM hNext = hChild;
			while (hNext = m_ItemTree.GetNextItem(hChild, TVGN_NEXT))
				hChild = hNext;

			m_ItemTree.DeleteItem(hChild);

			// Destroy siblings
			ObjIterator s = (*i)->siblings.begin();
			ObjIterator s_end = (*i)->siblings.end();

			for ( ; s != s_end; ++s) {
				HTREEITEM hItem = typeToItemMap[(*s)->pType->Name];

				if (hItem == NULL)
					continue;

				HTREEITEM hChild = m_ItemTree.GetChildItem(hItem);
				HTREEITEM hNext = hChild;
				while (hNext = m_ItemTree.GetNextItem(hChild, TVGN_NEXT))
					hChild = hNext;

				m_ItemTree.DeleteItem(hChild);
			}
		}
	}

	// Seems to be necessary to make initial expand buttons appear
	if (invalidateNextRefresh) {
		invalidateNextRefresh = false;
		m_ItemTree.Invalidate();
	}
}

void CObjectsPage::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
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
	if (nItem >= 0 && nSubItem == 1)
	{
		COLORREF text;
		COLORREF bg;
		m_List.GetItemColors(nItem, 1, text, bg);

		if (text != GetSysColor(COLOR_GRAYTEXT) && lineOwners[nItem] != NULL) {
			// User clicked an edit button.  Pause refreshing to prevent glitches.
			pausedUpdating = true;
		}
		
	}

	*pResult = 0;
}

LRESULT CObjectsPage::OnEditEnd(WPARAM nItem, LPARAM nSubItem)
{
	// Get item label
	CString label = m_List.GetItemText(nItem, 0);
	CString text = m_List.GetItemText(nItem, nSubItem);

	__int64 iValue = _atoi64(text);
	double fValue = atof(text);

	// Resume updating
	pausedUpdating = false;

	// Check for common labels
	if (pMyObject) {

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
			pMyObject->info.pInfo->filter.a = fValue / 100.0f;
		}
		
		// Work out which object owned the edited line
		CRunObject* lineOwner = lineOwners[nItem];
		if (lineOwner)
			lineOwner->OnDebuggerValueChanged(label, text);

		// Update bounding box for non system objects
		if (pMyObject->pType != NULL)
			pRuntime->UpdateBoundingBox(pMyObject);
	}

	// Save scroll position
	int nScroll = m_List.GetScrollPos(SB_VERT);

	// Force full refresh by clearing old display
	objDebugDisplay.resize(0);
	RefreshInspection();

	// Restore scroll position
	m_List.SetSelectionMark(nItem);
	m_List.SetCurSel(nItem, TRUE);
	m_List.SetScrollPos(SB_VERT, nScroll, TRUE);

	return 0;
}

void CObjectsPage::OnRClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	CMenu menu;
	menu.LoadMenu(MAKEINTRESOURCE(IDR_LISTCONTEXT));

	// Don't show Add to Watch if not a valid line
	int selectedLine = m_List.GetSelectionMark();
	if (selectedLine >= 0 && selectedLine < lineOwners.size() && lineOwners[selectedLine] == NULL)
		return;

	POINT p;
	GetCursorPos(&p);
	switch (menu.GetSubMenu(0)->TrackPopupMenuEx(TPM_RETURNCMD | TPM_LEFTALIGN, p.x, p.y, &m_List, NULL))
	{
		// Nothing selected
	case 0:
		break;
	case ID_ADDTOWATCH:

		{
			// Add this watch item
			WatchKey key;
			key.label = m_List.GetItemText(selectedLine, 0);
			CString itemText = key.label;

			WatchItem item;
			if (pMyObject) {
				if (itemText.Left(1) == "'") {
					/*
					item.type = WATCH_INSTANCEPRIVATEVAR;
					itemText = itemText.Mid(1, itemText.GetLength() - 2);
					item.pvIndex = find_index(pMyObject->pType->privateVars.begin(), pMyObject->pType->privateVars.end(), itemText);
					item.pPrivateVars = pCurrentVars;
					*/
					itemText = itemText.Mid(1, itemText.GetLength() - 2);

					// System object i.e. a global variable
					if (pMyObject->pType == NULL) {
						item.type = WATCH_GLOBALVAR;
						item.pvIndex = find_index(pRuntime->globalNames.begin(), pRuntime->globalNames.end(), itemText);
						item.pPrivateVars = NULL;
					}
					// A normal object i.e. a private variable
					else {
						item.type = WATCH_INSTANCEPRIVATEVAR;
						item.pvIndex = find_index(pMyObject->pType->privateVars.begin(), pMyObject->pType->privateVars.end(), itemText);
						item.pPrivateVars = pCurrentVars;
					}
				}
				else {
					item.type = WATCH_INSTANCE;
					item.pvIndex = -1;
					item.pPrivateVars = NULL;
				}

				item.obj = lineOwners[selectedLine];
			}
			else if (pMyType) {
				item.type = WATCH_OBJTYPE;
				item.objType = pMyType;
			}

			vector<WatchKey>& watchKeys = pWatchPage->watch[item];
			if (!exists_element(watchKeys.begin(), watchKeys.end(), key))
				watchKeys.push_back(key);
			break;
		}
	}
}

void CObjectsPage::OnRClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	CMenu menu;

	POINT curPos;
	GetCursorPos(&curPos);
	m_ItemTree.ScreenToClient(&curPos);
	HTREEITEM hSelected = m_ItemTree.HitTest(curPos);

	CString itemText = m_ItemTree.GetItemText(hSelected);

	// Check if type clicked
	CRunObjType* pType = pRuntime->GetTypeFromName(itemText);

	if (pType || hSelected == NULL) {
		menu.LoadMenu(MAKEINTRESOURCE(IDR_MENU1));

		if (hSelected == NULL) {
			// Disable 'Hide', 'Destroy all'
			menu.GetSubMenu(0)->EnableMenuItem(ID_HIDE, MF_BYCOMMAND | MF_GRAYED);
			menu.GetSubMenu(0)->EnableMenuItem(ID_DESTROYALL, MF_BYCOMMAND | MF_GRAYED);
		}

		POINT p;
		GetCursorPos(&p);
		switch (menu.GetSubMenu(0)->TrackPopupMenuEx(TPM_RETURNCMD | TPM_LEFTALIGN, p.x, p.y, &m_ItemTree, NULL))
		{
			// Nothing selected
		case 0:
			break;
		case ID_HIDE:
			// Delete this type from the tree view
			m_ItemTree.DeleteItem(hSelected);
			typeToItemMap[pType->Name] = NULL;
			hiddenTypes.push_back(pType->Name);
			break;
		case ID_SHOWALL:
			{
			// Add back to the tree all hidden items
			vector<CString>::iterator i = hiddenTypes.begin();

			for ( ; i != hiddenTypes.end(); i++) {
				typeToItemMap[*i] = m_ItemTree.InsertItem(*i);

				// Re-add its instances
				CRunObjType* pType = pRuntime->GetTypeFromName(*i);
				pRuntime->creationRow.insert(pRuntime->creationRow.end(), pType->instances.begin(), pType->instances.end());
			}

			hiddenTypes.resize(0);
			invalidateNextRefresh = true;
			}
			break;
		case ID_EXPANDALL:
			{
				HTREEITEM rootItem = m_ItemTree.GetRootItem();
				HTREEITEM hNext = rootItem;

				while (hNext = m_ItemTree.GetNextItem(rootItem, TVGN_NEXT)) {
					rootItem = hNext;

					m_ItemTree.Expand(rootItem, TVE_EXPAND);
				}
			}
			break;
		case ID_COLLAPSEALL:
			{
				HTREEITEM rootItem = m_ItemTree.GetRootItem();
				HTREEITEM hNext = rootItem;

				while (hNext = m_ItemTree.GetNextItem(rootItem, TVGN_NEXT)) {
					rootItem = hNext;

					m_ItemTree.Expand(rootItem, TVE_COLLAPSE);
				}
			}
			break;	
		case ID_DESTROYALL:
			{
				ObjIterator i = pType->instances.begin();

				for ( ; i != pType->instances.end(); i++)
					pRuntime->DestroyObject(*i);
			}
			break;
		}
	}
	else {
		menu.LoadMenu(MAKEINTRESOURCE(IDR_TREEINSTANCECONTEXT));

		if (itemText == "System" || itemText == "")
			menu.GetSubMenu(0)->EnableMenuItem(ID_DESTROY, MF_BYCOMMAND | MF_GRAYED);
		else
			menu.GetSubMenu(0)->EnableMenuItem(ID_DESTROY, MF_BYCOMMAND | MF_ENABLED);

		POINT p;
		GetCursorPos(&p);
		switch (menu.GetSubMenu(0)->TrackPopupMenuEx(TPM_RETURNCMD | TPM_LEFTALIGN, p.x, p.y, &m_ItemTree, NULL))
		{
			// Nothing selected
		case 0:
			break;
		case ID_DESTROY:
			// Get instance number
			if (itemText.Left(1) == "#") {
				itemText = itemText.Mid(1);
				int index = atoi(itemText) - 1;
				CRunObjType* pType = pRuntime->GetTypeFromName(m_ItemTree.GetItemText(m_ItemTree.GetParentItem(hSelected)));

				if (pType != NULL)
					pRuntime->DestroyObject(pType->instances[index]);
			}
			break;

		}
	}
}

void CObjectsPage::OnListKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	*pResult = 0;

	// Toggle window visibility
	if (pLVKeyDow->wVKey == VK_F11) {
		if (IsWindowVisible())
			pRuntime->pDebug->ShowWindow(SW_HIDE);
		else
			pRuntime->pDebug->ShowWindow(SW_SHOW);
	}

	// Enable updates for enter-pressed item edits
	//else if (pLVKeyDow->wVKey == VK_RETURN)
		pausedUpdating = false;
}

void CObjectsPage::OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	int selectedLine = m_List.GetSelectionMark();
	CString itemText = m_List.GetItemText(selectedLine, 0);

	if (selectedLine < 0 || selectedLine >= m_List.GetItemCount())
		return;

	// Add to watch
	WatchKey key;
	key.label = m_List.GetItemText(m_List.GetSelectionMark(), 0);

	WatchItem item;
	if (pMyObject) {
		if (itemText.Left(1) == "'") {

			itemText = itemText.Mid(1, itemText.GetLength() - 2);

			// System object i.e. a global variable
			if (pMyObject->pType == NULL) {
				item.type = WATCH_GLOBALVAR;
				item.pvIndex = find_index(pRuntime->globalNames.begin(), pRuntime->globalNames.end(), itemText);
				item.pPrivateVars = NULL;
			}
			// A normal object i.e. a private variable
			else {
				item.type = WATCH_INSTANCEPRIVATEVAR;
				item.pvIndex = find_index(pMyObject->pType->privateVars.begin(), pMyObject->pType->privateVars.end(), itemText);
				item.pPrivateVars = pCurrentVars;
			}
		}
		else {
			item.type = WATCH_INSTANCE;
			item.pvIndex = -1;
			item.pPrivateVars = NULL;
		}

		item.obj = lineOwners[selectedLine];
	}
	else if (pMyType) {
		item.type = WATCH_OBJTYPE;
		item.objType = pMyType;
	}

	vector<WatchKey>& watchKeys = pWatchPage->watch[item];	

	/*
	// Operator[] is insisting on merging unrelated items.  Loop to explicitly use operator==
	Watch::iterator w = pWatchPage->watch.begin();
	Watch::iterator watch_end = pWatchPage->watch.end();

	for ( ; w != watch_end; w++) {
		if (w->first == item)
			break;
	}

	// Not found: insert new
	if (w == watch_end)
		watchKeys = &(pWatchPage->watch[item]);
	// Found: use existing
	else
		watchKeys = &(w->second);
		*/

	if (!exists_element(watchKeys.begin(), watchKeys.end(), key))
		watchKeys.push_back(key);
}

void CObjectsPage::OnSelchangeFrames()
{
	pRuntime->changeFrame = m_FramesCombo.GetCurSel();
}
