// Dialogs\ManageVariableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "ManageVariableDlg.h"
#include "AddVariableDlg.h"

// CManageVariableDlg dialog
IMPLEMENT_DYNAMIC(CManageVariableDlg, CDialog)

CManageVariableDlg::CManageVariableDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CManageVariableDlg::IDD, pParent)
{
	pType = NULL;
	layout = NULL;
	application = NULL;
}

CManageVariableDlg::~CManageVariableDlg()
{
}

void CManageVariableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Control(pDX, IDC_RENAME, m_Edit);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	DDX_Control(pDX, IDOK, m_Close);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDC_MOVEUP, m_MoveUp);
	DDX_Control(pDX, IDC_MOVEDOWN, m_MoveDown);
}

BEGIN_MESSAGE_MAP(CManageVariableDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD, &CManageVariableDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, &CManageVariableDlg::OnBnClickedRemove)
	ON_BN_CLICKED(IDC_RENAME, &CManageVariableDlg::OnBnClickedRename)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST, &CManageVariableDlg::OnLvnKeydown)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST, &CManageVariableDlg::OnLvnEndlabelEdit)
	ON_BN_CLICKED(IDOK, &CManageVariableDlg::OnBnClickedOk)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MOVEUP, &CManageVariableDlg::OnBnClickedMoveup)
	ON_BN_CLICKED(IDC_MOVEDOWN, &CManageVariableDlg::OnBnClickedMovedown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CManageVariableDlg::OnDblClick)
END_MESSAGE_MAP()

void CManageVariableDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	dlgMan.Save();
}

void CManageVariableDlg::OnBnClickedAdd()
{
	CAddVariableDlg Dlg;

	Dlg.Name.Format("Variable %d", m_List.GetItemCount() + 1);

	if (Dlg.DoModal() == IDOK)
	{
		if (VariableNameExists(Dlg.Name))
			MessageBox(CString("The private variable name '") + Dlg.Name + "' already exists.  Please choose a unique name.", "Private variables", MB_OK | MB_ICONEXCLAMATION);

		else 
		{
			m_List.InsertItem(m_List.GetItemCount(), Dlg.Name, 0);
			m_List.SetItemText(m_List.GetItemCount() - 1, 1, GetTypeName(Dlg.Type));
			m_List.SetItemText(m_List.GetItemCount() - 1, 2, Dlg.Value);

			// Add the new variable to the object type
			int id = pType->VariableID++;
			pType->m_PrivateValues.push_back(PrivateValue(Dlg.Name, Dlg.Type, id));

			// Add the new variable to the corresponding instance values
			PrivateVariableInstanceAdder updater(id, Dlg.Value);
			ForEachObjectInstance(pType, layout->application, updater);
		}
	}
}

class MultiPrivateVariableTester : public ObjectFunctor 
{
public:

	int index;
	bool doneFirst;
	CString val;
	bool multi;

	MultiPrivateVariableTester() : 
		doneFirst(false),
		multi(false) 
	{
	}

	MultiPrivateVariableTester(int _index) : 
		index(_index), 
		doneFirst(false), 
		multi(false)
	{
	}

	void operator()(CObj* obj)
	{
		if (!doneFirst)
		{
			val = obj->m_values[index].value;
			doneFirst = true;
		}

		else if (obj->m_values[index].value != val)
			multi = true;
	}
};

bool CManageVariableDlg::VariableHasMultipleValues(CString name, CString& val)
{
	int index = find_index(pType->m_PrivateValues.begin(), pType->m_PrivateValues.end(), name);

	MultiPrivateVariableTester tester(index);
	ForEachObjectInstance(pType, layout->application, tester);

	if (tester.multi) 
	{
		val = "";
		return true;
	}

	else 
	{
		val = tester.val;
		return false;
	}
}

bool CManageVariableDlg::VariableNameExists(CString name)
{
	name.MakeLower();

	for (int i = 0; i < m_List.GetItemCount(); i++) {
		CString curName = m_List.GetItemText(i, 0);
		curName.MakeLower();
		if (curName == name)
			return true;
	}

	return false;
}

CString CManageVariableDlg::GetTypeName(int type)
{
	switch (type) {
		case 0:
			return "Number";
		case 1:
			return "Text";
		default:
			return "???";
	}
}

int CManageVariableDlg::GetNameType(CString type)
{
	if (type == "Number")
		return 0;
	else if (type == "Text")
		return 1;
	else
		return 0;
}

BOOL CManageVariableDlg::OnInitDialog()
{
	CExtNCW<CExtResizableDialog>::OnInitDialog();

	CExtBitmap Bitmap;
	Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_ICONADD));
	m_Add.SetIcon(Bitmap.CreateHICON());
	m_Add.SetDrawBorder(FALSE);
	m_Add.SetFlat();

	Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_ICONDELETE));
	m_Remove.SetIcon(Bitmap.CreateHICON());
	m_Remove.SetDrawBorder(FALSE);
	m_Remove.SetFlat();

	Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_ICONEDIT));
	m_Edit.SetIcon(Bitmap.CreateHICON());
	m_Edit.SetDrawBorder(FALSE);
	m_Edit.SetFlat();

	Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_UPARROW));
	m_MoveUp.SetIcon(Bitmap.CreateHICON());
	m_MoveUp.SetDrawBorder(FALSE);
	m_MoveUp.SetFlat();

	Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_DOWNARROW));
	m_MoveDown.SetIcon(Bitmap.CreateHICON());
	m_MoveDown.SetDrawBorder(FALSE);
	m_MoveDown.SetFlat();

	m_List.InsertColumn(0, "Name", LVCFMT_LEFT, 150);
	m_List.InsertColumn(1, "Type", LVCFMT_LEFT, 50);
	m_List.InsertColumn(2, "Initial Value", LVCFMT_LEFT, 150);

	SetWindowText("Construct : Manage Private Variables");
	
	RefreshVariablesList();

	// Resizing
	dlgMan.Load(this->m_hWnd, "Software\\Construct\\ManageVariableDlg");
    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDC_LIST, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(IDOK, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_ADD, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_REMOVE, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_RENAME, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_MOVEUP, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_MOVEDOWN, ANCHOR_LEFT | ANCHOR_BOTTOM);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return true;
}

void CManageVariableDlg::RefreshVariablesList()
{
	m_List.DeleteAllItems();

	vector<PrivateValue>::iterator i = pType->m_PrivateValues.begin();

	for ( ; i != pType->m_PrivateValues.end(); i++) 
	{
		m_List.InsertItem(m_List.GetItemCount(), i->name);
		m_List.SetItemText(m_List.GetItemCount() - 1, 1, GetTypeName(i->type));
		
		CString value;
		VariableHasMultipleValues(i->name, value);
		m_List.SetItemText(m_List.GetItemCount() - 1, 2, value);
	}
}

void CManageVariableDlg::OnBnClickedRemove()
{
	ConfirmDeleteSelectedItems();
}

// Updates an object's private variable to a new value.
class PrivateVariableValueUpdater : public ObjectFunctor {
public:
	int pvIndex;
	CString newValue;

	PrivateVariableValueUpdater() {}
	PrivateVariableValueUpdater(int _index, CString _value) : pvIndex(_index), newValue(_value) {}

	void operator()(CObj* pObj)
	{
		// Simply poke in the new value to the specified private variable
		pObj->m_values[pvIndex].value = newValue;
	}
};

// Used to move an object's private variable up or down.
class PrivateVariableMover : public ObjectFunctor {
public:
	int pvIndex;
	bool up;		// false = down

	PrivateVariableMover() {}
	PrivateVariableMover(int _pvIndex, bool _up) : pvIndex(_pvIndex), up(_up) {}

	void operator()(CObj* pObj)
	{
		InitValue temp = pObj->m_values[pvIndex];
		pObj->m_values.erase(pObj->m_values.begin() + pvIndex);
		pObj->m_values.insert(pObj->m_values.begin() + (pvIndex + (up ? -1 : 1)), temp);
	}
};

void CManageVariableDlg::OnBnClickedRename()
{
	int nItem = m_List.GetSelectionMark();
	if (nItem == -1)
		return;

	CAddVariableDlg Dlg;
	CString oldName = Dlg.Name = m_List.GetItemText(nItem, 0);
	Dlg.Type = GetNameType(m_List.GetItemText(nItem, 1));
	Dlg.Value = m_List.GetItemText(nItem, 2);

	Dlg.Caption = "Construct : Edit private variable";

	if (Dlg.DoModal() == IDOK) {

		if (Dlg.Name != oldName && VariableNameExists(Dlg.Name))
			MessageBox(CString("The private variable name '") + Dlg.Name + "' already exists.  Please choose a unique name.", "Private variables", MB_OK | MB_ICONEXCLAMATION);

		else
		{
			m_List.SetItemText(nItem, 0, Dlg.Name);
			m_List.SetItemText(nItem, 1, GetTypeName(Dlg.Type));
			m_List.SetItemText(nItem, 2, Dlg.Value);

			vector<PrivateValue>::iterator v = find(pType->m_PrivateValues.begin(), pType->m_PrivateValues.end(), oldName);

			// If renamed, update
			if (Dlg.Name != oldName)
				(*v).name = Dlg.Name;
			(*v).type = Dlg.Type;

			// Value changed - update across all instances
			if (Dlg.Value != "") 
			{
				// Iterate and update all objects
				int index = find_index(pType->m_PrivateValues.begin(), pType->m_PrivateValues.end(), Dlg.Name);

				PrivateVariableValueUpdater updater(index, Dlg.Value);
				ForEachObjectInstance(pType, layout->application, updater);
			}
		}
	}
}

void CManageVariableDlg::ConfirmDeleteSelectedItems()
{
	int selCount = m_List.GetSelectedCount();

	if (selCount == 1) 
	{
		CString name = m_List.GetItemText(m_List.GetSelectionMark(), 0);

		// Determine if this variable belongs to any families
		vector<Family*>::iterator f = pType->families.begin();

		CString familiesList;

		for ( ; f != pType->families.end(); f++) {
			(*f)->UpdateFamilyData();

			if ((*f)->VariableNameExists(name))
				familiesList += (*f)->name + "\n";				
		}

		bool okay = false;

		if (familiesList != "") {
			CString msg = "This private variable is also used by the following families:\n\n";
			msg += familiesList;
			msg += "\nIf you delete this variable, it will also be removed from the above families.  All events referring to this variable and the variable in the above families will be removed.  Do you want to continue?";

			if (MessageBox(msg, "Remove private variable", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
				okay = true;
		}
		else if (MessageBox(CString("This will remove all actions, conditions and expressions relating to the variable.  Are you sure you want to delete the variable '") + name + "'?", "Delete Variable", MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			okay = true;
		}

		if (okay) {
			m_List.DeleteItem(m_List.GetSelectionMark());

			// Iterate and update all objects
			int index = find_index(pType->m_PrivateValues.begin(), pType->m_PrivateValues.end(), name);

			pType->m_PrivateValues.erase(pType->m_PrivateValues.begin() + index);

			PrivateVariableInstanceDeleter updater(index);
			ForEachObjectInstance(pType, layout->application, updater);
		}
	}

	else if (selCount > 1)
	{
		CString msg;
		msg.Format("Are you sure you want to delete these %d private variables?", m_List.GetSelectedCount());
	
		if (MessageBox(msg, "Delete Variables", MB_ICONQUESTION | MB_YESNO) == IDNO)
			return;

		// Iterate selected items and delete
		POSITION selpos = m_List.GetFirstSelectedItemPosition();

		while (selpos) 
		{
			int nItem = m_List.GetNextSelectedItem(selpos);
			CString name = m_List.GetItemText(nItem, 0);

			// Iterate and update all objects
			int index = find_index(pType->m_PrivateValues.begin(), pType->m_PrivateValues.end(), name);

			ErasePrivateVariableReferences(application, pType, pType->m_PrivateValues[index].identifier);

			pType->m_PrivateValues.erase(pType->m_PrivateValues.begin() + index);

			PrivateVariableInstanceDeleter updater(index);
			ForEachObjectInstance(pType, layout->application, updater);
		}

		RefreshVariablesList();
	}
}

void CManageVariableDlg::OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pLVKeyDow->wVKey == VK_DELETE)
		ConfirmDeleteSelectedItems();
}

void CManageVariableDlg::OnLvnEndlabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here

	CString Name = pDispInfo->item.pszText;
	CString oldName = m_List.GetItemText(pDispInfo->item.iItem,0);

	if (Name == "") 
	{
		*pResult = 0;
		return;
	}

	if (Name != oldName && VariableNameExists(Name))
		*pResult = 0;

	else 
	{
		// Do rename
		*pResult = 1;

		// Update name
		(*find(pType->m_PrivateValues.begin(), pType->m_PrivateValues.end(), oldName)).name = Name;
	}
}

void CManageVariableDlg::OnBnClickedOk()
{
	// Save variables info

	CDialog::OnOK();
}

void CManageVariableDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dlgAnchor.OnSize();

	Invalidate();
}

void CManageVariableDlg::OnBnClickedMoveup()
{
	POSITION listpos = m_List.GetFirstSelectedItemPosition();
	int sel = m_List.GetNextSelectedItem(listpos);

	if (sel < 1) return;

	PrivateValue temp = pType->m_PrivateValues[sel];
	pType->m_PrivateValues.erase(pType->m_PrivateValues.begin() + sel);
	pType->m_PrivateValues.insert(pType->m_PrivateValues.begin() + (sel - 1), temp);

	PrivateVariableMover mover(sel, true); // up
	ForEachObjectInstance(pType, application, mover);

	RefreshVariablesList();

	if (sel > 0) sel--;

	m_List.SetItemState(sel, LVIS_SELECTED, LVIS_SELECTED);
}

void CManageVariableDlg::OnBnClickedMovedown()
{
	POSITION listpos = m_List.GetFirstSelectedItemPosition();
	int sel = m_List.GetNextSelectedItem(listpos);

	if (sel == -1 || sel >= m_List.GetItemCount() - 1) return;

	PrivateValue temp = pType->m_PrivateValues[sel];
	pType->m_PrivateValues.erase(pType->m_PrivateValues.begin() + sel);
	pType->m_PrivateValues.insert(pType->m_PrivateValues.begin() + (sel + 1), temp);

	PrivateVariableMover mover(sel, false);	// down
	ForEachObjectInstance(pType, application, mover);
	
	RefreshVariablesList();

	if (sel < m_List.GetItemCount() - 1) sel++;

	m_List.SetItemState(sel, LVIS_SELECTED, LVIS_SELECTED);
}

void CManageVariableDlg::OnDblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	OnBnClickedRename();
}
