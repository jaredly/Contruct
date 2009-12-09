// Object Selector Dlg.cpp
#include "stdafx.h"
#include "..\Construct.h"
#include "ObjectSelector.h"

// ObjectSelectorDialog dialog
IMPLEMENT_DYNAMIC(ObjectSelectorDialog, CDialog)

bool ObjectTypeSortFunction(const CObjType* typeA, const CObjType* typeB)
{
	CString l = typeA->GetName();
	CString r = typeB->GetName();

	l.MakeLower();
	r.MakeLower();

	return l < r;
}

ObjectSelectorDialog::ObjectSelectorDialog(CApplication* application_, CLayout* layout_, CString previous_, int dll_filter_, bool show_families_, bool show_attributes_, int exclude_)
	: CExtNCW<CExtResizableDialog>(ObjectSelectorDialog::IDD, NULL),
	application(application_),
	layout(layout_),
	show_families(show_families_),
	show_attributes(show_attributes_),
	previous(previous_),
	exclude(exclude_)
{
	if (dll_filter_)
		dll_filter = application->FindObjTypeFromNumber(dll_filter_)->DLLIndex;
}

void ObjectSelectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJECTLIST, objects);
	DDX_Control(pDX, IDOK, ok);
	DDX_Control(pDX, IDCANCEL, cancel);
}

BEGIN_MESSAGE_MAP(ObjectSelectorDialog, CExtNCW<CExtResizableDialog>)
	ON_BN_CLICKED(IDOK, &ObjectSelectorDialog::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_OBJECTLIST, &ObjectSelectorDialog::OnNMDblclkList)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// ObjectSelectorDialog message handlers
BOOL ObjectSelectorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	images.DeleteImageList();
	images.Create(16, 16, ILC_COLOR24, 3, 3);
	objects.SetImageList(&images, LVSIL_NORMAL);

	// Resizing anchors
	dialog_manager.Load(this->m_hWnd, "Software\\Construct\\ObjectSelectorDialog");
    dialog_anchor.Init(this->m_hWnd);

	dialog_anchor.Add(IDC_OBJECTLIST, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dialog_anchor.Add(IDOK, ANCHOR_BOTTOM | ANCHOR_RIGHT);
	dialog_anchor.Add(IDCANCEL, ANCHOR_BOTTOM | ANCHOR_RIGHT);

	CObjTypeMap* object_map = &application->object_types;
	POSITION position = object_map->GetStartPosition();

	long key;

	CObjType* object;

	vector<CObjType*> type_list;

	while (position != NULL) 
	{
		object_map->GetNextAssoc(position, key, object);

		if (layout)
		{
			if (object->ObjectIdentifier == exclude) continue;

			if (object->m_bGlobal)
			{
				type_list.push_back(object);

				continue;
			}

			POSITION position = layout->objects.GetStartPosition();

			for (int i = 0; i < layout->objects.GetCount(); i++) 
			{
				long ID;
				CObj* o;

				layout->objects.GetNextAssoc(position, ID, o);
					
				if (o->GetGlobalID() == object->ObjectIdentifier)
				{
					if(object->m_bShowInEventSheetEditor)
						type_list.push_back(object);

					break;
				}
			}
		}

		else
			type_list.push_back(object);
	}

	sort(type_list.begin(), type_list.end(), ObjectTypeSortFunction);

	// add items to list
	vector<CObjType*>::iterator o = type_list.begin();

	objects.AddGroup(0, "Objects");

	int iter = -1;

	for ( ; o != type_list.end(); o++)
	{
		iter++;

		if (dll_filter != -1 && dll_filter != (*o)->DLLIndex)
		{
			iter--;
			continue;
		}

		HBITMAP bitmap = (*o)->small_image.MakeBitmap();
		int icon_index = ImageList_Add(images, bitmap, bitmap);
		DeleteObject(bitmap);

		objects.InsertItem(iter, (*o)->GetName(), icon_index, 0);
		objects.SetItemData(iter, (DWORD)(*o));

		if ((*o)->ObjectIdentifier == atoi(previous))
			objects.SetItemState(iter, LVIS_SELECTED, LVIS_SELECTED);
	}

	// add families
	if (show_families)
	{
		objects.AddGroup(1, "Families");

		list<Family>::iterator f = application->families.begin();

		for ( ; f != application->families.end(); f++)
		{
			if(f->is_in_use)
			{
				HBITMAP bitmap = f->small_image.MakeBitmap();
				int icon_index = ImageList_Add(images, bitmap, bitmap);
				DeleteObject(bitmap);

				objects.InsertItem(objects.GetItemCount(), f->name, icon_index, 1);

				int pos = objects.GetItemCount() - 1;

				Family* family = &(*f);

				objects.SetItemData(pos, (DWORD)family);

				if (f->identifier == atoi(previous))
					objects.SetItemState(pos, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
	}

	if (show_attributes)
	{
		objects.AddGroup(2, "Attributes");

		POSITION position = application->object_types.GetStartPosition();
		
		CStringArray list;

		while (position != NULL) 
		{
			application->object_types.GetNextAssoc(position, key, object);
			
			for (int i = 0; i < object->traits.size(); i++)
			{
				CString trait = object->traits[i];
				
				bool found = false;

				for (int j = 0; j < list.GetSize(); j++)
					if (list.GetAt(j) == trait)
						found = true;

				if (!found)
					list.Add(trait);
			}
		}

		for (int i = 0; i < list.GetSize(); i++)
		{
			objects.InsertItem(objects.GetItemCount(), list.GetAt(i), -1, 2);
			int pos = objects.GetItemCount() - 1;
			objects.SetItemData(pos, -1);

			

			CString item = list.GetAt(i);
			if (item == previous)
				objects.SetItemState(pos, LVIS_SELECTED, LVIS_SELECTED);
		}
	}

	objects.EnableGroups(true);
	objects.SetFocus();

	return TRUE;

}

void ObjectSelectorDialog::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	OnBnClickedOk();
}

void ObjectSelectorDialog::OnBnClickedOk()
{
	int item = objects.GetSelectionMark();
	
	if (item == -1)
		OnCancel();
	else
	{
		if (objects.groups[item] == 0)
		{
			CObjType* object = ((CObjType*)objects.GetItemData(item));
			selected.type = object;
		}

		else if (objects.groups[item] == 1)
		{
			Family* family = ((Family*)objects.GetItemData(item));
			selected.family = family;
			selected.is_family = true;
		}

		else
		{
			selected.is_trait = true;
			selected.trait = objects.GetItemText(item, 0);
		}

		OnOK();
	}
}

void ObjectSelectorDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dialog_anchor.OnSize();	
}

void ObjectSelectorDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	dialog_manager.Save();	
}