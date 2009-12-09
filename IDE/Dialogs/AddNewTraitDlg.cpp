// AddNewTraitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "AddNewTraitDlg.h"


// CAddNewTraitDlg dialog

IMPLEMENT_DYNAMIC(CAddNewTraitDlg, CDialog)

CAddNewTraitDlg::CAddNewTraitDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CAddNewTraitDlg::IDD, pParent)
{
	pApp = NULL;
}

CAddNewTraitDlg::~CAddNewTraitDlg()
{
}

void CAddNewTraitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Control(pDX, IDC_RENAME, m_Edit);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	DDX_Control(pDX, IDOK, m_Close);
}

BOOL CAddNewTraitDlg::OnInitDialog()
{
	CExtNCW<CExtResizableDialog>::OnInitDialog();

	m_Name.SetWindowText("My Attribute");

	ASSERT(pApp != NULL);

	m_List.InsertColumn(1, "Attribute name", 0, 197);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	RefreshTraitList();

	CExtBitmap Bitmap;
	Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_ICONADD));
	m_Add.SetIcon(Bitmap.CreateHICON());

	Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_ICONEDIT));
	m_Edit.SetIcon(Bitmap.CreateHICON());

	Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_ICONDELETE));
	m_Remove.SetIcon(Bitmap.CreateHICON());

	return true;
}

void CAddNewTraitDlg::RefreshTraitList()
{
	m_List.DeleteAllItems();

	vector<CString>::iterator i = pApp->m_Traits.begin();
	
	for ( ; i != pApp->m_Traits.end(); i++)
		m_List.InsertItem(m_List.GetItemCount(), *i);
}

BEGIN_MESSAGE_MAP(CAddNewTraitDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAddNewTraitDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAddNewTraitDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ADD, &CAddNewTraitDlg::OnBnClickedAdd)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST1, &CAddNewTraitDlg::OnEndLabelEdit)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, &CAddNewTraitDlg::OnListKeyDown)
	ON_BN_CLICKED(IDC_RENAME, &CAddNewTraitDlg::OnBnClickedRename)
	ON_BN_CLICKED(IDC_REMOVE, &CAddNewTraitDlg::OnBnClickedDelete)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST1, &CAddNewTraitDlg::OnLvnBeginlabeleditList1)
END_MESSAGE_MAP()


// CAddNewTraitDlg message handlers

void CAddNewTraitDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CAddNewTraitDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CAddNewTraitDlg::OnBnClickedAdd()
{
	CString Name;
	m_Name.GetWindowText(Name);

	if (Name == "") {
		MessageBox("Please enter the name of the attribute to add.", "Add new attribute", MB_ICONEXCLAMATION);
		return;
	}

	if (TraitNameExists(Name)) {
		MessageBox(CString("The attribute name '") + Name + "' is already being used.  Please choose a unique name.", "Add new attribute", MB_ICONEXCLAMATION);
		return;
	}

	pApp->m_Traits.push_back(Name);
	
	RefreshTraitList();

	m_Name.SetWindowText("");
	m_Name.SetFocus();
}

void CAddNewTraitDlg::OnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	
	// Accept the change if no element exists with the given name
	CString Name = pDispInfo->item.pszText;

	// Name is "" if the user did not make any change.
	if (Name == "") {
		*pResult = 0;
		return;
	}

	if (TraitNameExists(Name))
		*pResult = 0;
	else {
		*pResult = 1;

		CString oldName = m_List.GetItemText(pDispInfo->item.iItem,0);
		
		// Update this name in the traits list
		*(find(pApp->m_Traits.begin(), pApp->m_Traits.end(), oldName)) = Name;

		// Update any reference to this name in all CObjType
		CObjTypeMap* objTypeMap = &pApp->m_ObjectTypes;
		POSITION pos = objTypeMap->GetStartPosition();
		CObjType* pType;
		long nKey;

		// iterate through objects
		while (pos != NULL)
		{
			objTypeMap->GetNextAssoc(pos, nKey, pType);

			vector<CString>::iterator iter = find(pType->m_Traits.begin(), pType->m_Traits.end(), oldName);

			// Update name
			if (iter != pType->m_Traits.end())
				*iter = Name;
		}
	}
}

bool CAddNewTraitDlg::TraitNameExists(CString Name)
{
	Name.MakeLower();

	vector<CString>::iterator i = pApp->m_Traits.begin();

	for ( ; i != pApp->m_Traits.end(); i++) {
		CString curName = *i;
		curName.MakeLower();
		if (Name == curName)
			return true;
	}

	return false;
}
void CAddNewTraitDlg::OnListKeyDown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	// Check if delete pressed
	if (pLVKeyDow->wVKey == VK_DELETE)
		ConfirmDeleteSelectedItems();
}

void CAddNewTraitDlg::ConfirmDeleteSelectedItems()
{
	// TODO: remove any references to deleted traits in all CObjType*

	int delCount = m_List.GetSelectedCount();

	if (delCount == 1) {
		CString msg;
		CString Name = m_List.GetItemText(m_List.GetSelectionMark(), 0);
		
		if (TraitNameIsBuiltIn(Name))
			msg.Format("Are you sure you want to delete the built-in attribute '%s'?  This may prevent some built-in behaviour from working, such as a movement's response to attributes.  You can restore this functionality by adding another attribute with the same name.", Name);
		else
			msg.Format("Are you sure you want to delete the attribute '%s'?", Name);
		
		if (MessageBox(msg, "Delete attribute", MB_YESNO | MB_ICONQUESTION) == IDYES) {

			// Delete this selected trait			
			pApp->m_Traits.erase(find(pApp->m_Traits.begin(), pApp->m_Traits.end(), Name));

			// Remove any references in any CObjType
			CObjTypeMap* objTypeMap = &pApp->m_ObjectTypes;
			POSITION pos = objTypeMap->GetStartPosition();
			CObjType* pType;
			long nKey;

			// iterate through objects
			while (pos != NULL)
			{
				objTypeMap->GetNextAssoc(pos, nKey, pType);

				vector<CString>::iterator iter = find(pType->m_Traits.begin(), pType->m_Traits.end(), Name);

				// Remove name
				if (iter != pType->m_Traits.end())
					pType->m_Traits.erase(iter);
			}

			RefreshTraitList();
		}
	}
	else if (delCount > 1) {
		CString msg;
		msg.Format("Are you sure you want to delete these %d attributes?", m_List.GetSelectedCount());
		MessageBox(msg, "Delete attribute", MB_YESNO | MB_ICONQUESTION);

		// Iterate selected items and delete
		POSITION selpos = m_List.GetFirstSelectedItemPosition();

		while (selpos) {
			int nItem = m_List.GetNextSelectedItem(selpos);
			CString Name = m_List.GetItemText(nItem, 0);
			pApp->m_Traits.erase(find(pApp->m_Traits.begin(), pApp->m_Traits.end(), Name));

			// Remove any references in any CObjType
			CObjTypeMap* objTypeMap = &pApp->m_ObjectTypes;
			POSITION pos = objTypeMap->GetStartPosition();
			CObjType* pType;
			long nKey;

			// iterate through objects
			while (pos != NULL)
			{
				objTypeMap->GetNextAssoc(pos, nKey, pType);

				vector<CString>::iterator iter = find(pType->m_Traits.begin(), pType->m_Traits.end(), Name);

				// Remove name
				if (iter != pType->m_Traits.end())
					pType->m_Traits.erase(iter);
			}
		}

		RefreshTraitList();
	}
}

void CAddNewTraitDlg::OnBnClickedRename()
{
	if (m_List.GetSelectionMark() != -1) {
		CString Name = m_List.GetItemText(m_List.GetSelectionMark(), 0);

		if (TraitNameIsBuiltIn(Name))
			MessageBox("You cannot rename the built-in attributes.", "Attributes", MB_ICONINFORMATION);
		else {
			m_List.SetFocus();
			m_List.EditLabel(m_List.GetSelectionMark());
		}
	}
}

void CAddNewTraitDlg::OnBnClickedDelete()
{
	ConfirmDeleteSelectedItems();
}

void CAddNewTraitDlg::OnLvnBeginlabeleditList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	CString Name = m_List.GetItemText(pDispInfo->item.iItem, 0);
	
	*pResult = TraitNameIsBuiltIn(Name);
}

bool CAddNewTraitDlg::TraitNameIsBuiltIn(CString Name)
{
	Name.MakeLower();

	for (int i = 0; i < 7; i++) {
		CString cur = g_BuiltInTraits[i];
		cur.MakeLower();
		if (Name == cur)
			return true;
	}

	return false;
}