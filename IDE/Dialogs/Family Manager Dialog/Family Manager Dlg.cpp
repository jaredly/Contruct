// Dialogs\FamilyManagerDlg.cpp : implementation file
#include "stdafx.h"
#include "..\..\Construct.h"
#include "Family Manager Dlg.h"
#include "..\ObjectSelector.h"
#include "..\..\Utilities\Helper.h"

// FamilyManagerDialog dialog
IMPLEMENT_DYNAMIC(FamilyManagerDialog, CDialog)

FamilyManagerDialog::FamilyManagerDialog(CApplication* application_) :
	CExtWS<CDialog>(FamilyManagerDialog::IDD, (CWnd*)NULL),
	application(application_)
{

}

void FamilyManagerDialog::DoDataExchange(CDataExchange* pDX)
{
	CExtWS<CDialog>::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJECTS, objects);
	DDX_Control(pDX, IDC_LIST, families);
	DDX_Control(pDX, IDOK, ok);
	DDX_Control(pDX, IDC_ADD, add);
	DDX_Control(pDX, IDC_REMOVE, remove);
}

BEGIN_MESSAGE_MAP(FamilyManagerDialog, CExtWS<CDialog>)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &FamilyManagerDialog::OnItemChanged)
	ON_BN_CLICKED(IDC_ADD, &FamilyManagerDialog::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, &FamilyManagerDialog::OnBnClickedRemove)
END_MESSAGE_MAP()

// FamilyManagerDialog message handlers
BOOL FamilyManagerDialog::OnInitDialog() 
{
	CExtWS<CDialog>::OnInitDialog();

	list<Family>::iterator f = application->families.begin();

	// Set up family list
	family_images.Create(16, 16, ILC_COLOR24, 3, 3);
	families.SetImageList(&family_images, LVSIL_SMALL);
	families.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	for ( ; f != application->families.end(); f++)
	{
		if(f->is_in_use)
		{
			// Add icon
			HBITMAP bitmap = f->small_image.MakeBitmap();
			int Icon = ImageList_Add(family_images.m_hImageList, bitmap, NULL);
			DeleteObject(bitmap);

			int ID = families.InsertItem(Icon, f->name, Icon);
			families.SetItemData(ID, f->identifier);
		}
	}

	object_images.Create(32, 32, ILC_COLOR24, 3, 3);
	objects.SetImageList(&object_images, LVSIL_NORMAL);

	// Set parent for pages
	effects_page.m_pParent = this;
	private_variables_page.m_pParent = this;
	behaviors_page.m_pParent = this;

	property_sheet.AddPage(&private_variables_page);
	property_sheet.AddPage(&effects_page);
	property_sheet.AddPage(&behaviors_page);

	property_sheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	property_sheet.ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	property_sheet.ModifyStyle(0, WS_TABSTOP);

	CRect rcSheet;
	GetDlgItem(IDC_PROPSHEET)->GetWindowRect(&rcSheet);
	ScreenToClient(&rcSheet);
	property_sheet.SetWindowPos(NULL, rcSheet.left - 7, rcSheet.top - 7, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

	SubclassChildControls();
				
	return TRUE;
}

void FamilyManagerDialog::OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	POSITION Pos = families.GetFirstSelectedItemPosition();
	int Item = families.GetNextSelectedItem(Pos);

	// Clean up any old data
	objects.DeleteAllItems();

	for (int i = 0; i < object_images.GetImageCount(); i++)
		object_images.Remove(0);

	// Show objects for the selected family
	int ID = families.GetItemData(Item);

	Family* family = application->FindFamilyFromNumber(ID);

	if (family == NULL)
	{
		effects_page.ShowFamily(NULL);
		private_variables_page.ShowFamily(NULL);
		behaviors_page.ShowFamily(NULL);

		return;
	}

	family->UpdateFamilyData();

	vector<CObjType*> Objects;
	family->GetObjectTypes(Objects);

	for (int i = Objects.size() - 1; i >= 0; i--)
	{
		CObjType* pObjectType = Objects[i];
		HBITMAP bitmap = pObjectType->m_Image.MakeBitmap();
		int ImageID = ImageList_Add(object_images.m_hImageList, bitmap, NULL);
		DeleteObject(bitmap);
		int Item = objects.InsertItem(objects.GetItemCount(), pObjectType->GetName(), ImageID);
		objects.SetItemData(Item, pObjectType->ObjectIdentifier);
	}

	add.EnableWindow();
	remove.EnableWindow();

	// Now, tell the three property page children we have a family
	effects_page.ShowFamily(family);
	private_variables_page.ShowFamily(family);
	behaviors_page.ShowFamily(family);

	*pResult = 0;
}

void FamilyManagerDialog::OnBnClickedAdd()
{
	POSITION Pos = families.GetFirstSelectedItemPosition();
	int Item = families.GetNextSelectedItem(Pos);

	if (Item == -1) return;

	int ID = families.GetItemData(Item);
	Family* family = application->FindFamilyFromNumber(ID);

	vector<CObjType*> Objects;
	family->GetObjectTypes(Objects);

	ObjectSelectorDialog ObjectSelector(application, NULL, "", -1, false);

	if(ObjectSelector.DoModal() == IDOK) 
	{
		// Check it's not already in the family
		bool bAdd = true;

		for (int i = 0; i < Objects.size(); i++)
			if (Objects[i]->ObjectIdentifier == ObjectSelector.selected.type->ObjectIdentifier)
				bAdd = false;

		if (bAdd)
		{
			// First step is to get the new object
			CObjType* pNewObject = application->FindObjTypeFromNumber(ObjectSelector.selected.type->ObjectIdentifier);

			// Add to object
			//pNewObject->families.push_back(family);
			if (application->AddObjectTypeToFamily(pNewObject, family)) {

				// Now add it to the list
				HBITMAP bitmap = pNewObject->m_Image.MakeBitmap();
				int ImageID = ImageList_Add(object_images.m_hImageList, bitmap, NULL);
				DeleteObject(bitmap);
				int Item = objects.InsertItem(objects.GetItemCount(), pNewObject->GetName(), ImageID);
				objects.SetItemData(Item, pNewObject->ObjectIdentifier);
			}
		}
	}

	// Refresh variables
	long Result = 0;
	OnItemChanged(NULL, &Result);
}

void FamilyManagerDialog::OnBnClickedRemove()
{
	POSITION Pos = families.GetFirstSelectedItemPosition();
	int Item = families.GetNextSelectedItem(Pos);

	if (Item == -1) return;

	int ID = families.GetItemData(Item);
	Family* family = application->FindFamilyFromNumber(ID);

	// Now get the CObjType
	POSITION Position = objects.GetFirstSelectedItemPosition();
	int Obj = objects.GetNextSelectedItem(Position);

	// Fix crash; need to check an object is selected too..
	if (Obj == -1) return;

	int ObjID = objects.GetItemData(Obj);
	CObjType* pType = application->FindObjTypeFromNumber(ObjID);

	// Check if last in family
	CApplication::ObjTypeFamilyStatus status = application->PromptIfTypeLastInFamily(pType, family);

	switch (status)
	{
		// Delete family
	case CApplication::user_clicked_yes_delete_family:

		pType->families.erase(find(pType->families.begin(), pType->families.end(), family));
		application->DeleteFamily(family);

		// Clear the tabs
		private_variables_page.ShowFamily(NULL);
		effects_page.ShowFamily(NULL);
		behaviors_page.ShowFamily(NULL);

		break;

	case CApplication::user_clicked_no:

		return;	// user cancelled operation

	case CApplication::not_last_type_in_family:

		// Remove this type from the family
		pType->families.erase(find(pType->families.begin(), pType->families.end(), family));
		add.EnableWindow(false);
		remove.EnableWindow(false);

		// Clear the tabs
		private_variables_page.ShowFamily(NULL);
		effects_page.ShowFamily(NULL);
		behaviors_page.ShowFamily(NULL);

		break;
	}

	RefreshSelectedFamily(ID);
}

void FamilyManagerDialog::RefreshSelectedFamily(int ID)
{
	int oldID = ID;
	int toSelect = -1;

	// Now remove from list
	families.DeleteAllItems();
	objects.DeleteAllItems();

	list<Family>::iterator f = application->families.begin();

	for (int i = 0; i < family_images.GetImageCount(); i++)
		family_images.Remove(0);

	for ( ; f != application->families.end(); f++)
	{
		if(f->is_in_use)
		{
			// Add icon
			HBITMAP bitmap = f->small_image.MakeBitmap();
			int Icon = ImageList_Add(family_images.m_hImageList, bitmap, NULL);
			DeleteObject(bitmap);

			int ID = families.InsertItem(Icon, f->name, Icon);
			families.SetItemData(ID, f->identifier);
			
			if (f->identifier == oldID)
				toSelect = families.GetItemCount() - 1;
		}
	}

	if (toSelect >= 0 && families.GetItemCount() != 0) 
	{
		families.SetItemState(toSelect, LVIS_FOCUSED, LVIS_FOCUSED);
		families.SetItemState(toSelect, LVIS_SELECTED, LVIS_SELECTED);

		// Refresh variables
		long Result = 0;
		OnItemChanged(NULL, &Result);
	}
}