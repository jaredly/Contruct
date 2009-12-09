/////////////////////////////////////////////////////////////////////////////
// Event Wizard Dialog
/////////////////////////////////////////////////////////////////////////////

// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "EventWizardDlg.h"

HWND listw;

static int CALLBACK SortACES(LPARAM i1, LPARAM i2, LPARAM lParamSort)
{
	TCHAR tsz1[200];
	TCHAR tsz2[200];
	ListView_GetItemText(listw, i1, 0, tsz1, 200);
	ListView_GetItemText(listw, i2, 0, tsz2, 200);

	return _tcscmp(tsz2, tsz1);
}

void CEventWizardDlg::OnFilterChange()
{
	m_bOK = false;

	m_Find.GetWindowText(m_Filter);
	m_Filter.MakeLower();

	if (m_iStep == 1)
	{
		int Sel = m_List.GetSelectedItem();
		CString Name = m_List.GetItemText(Sel, NULL); // Previously selected name

		// Clear list
		m_List.DeleteAllItems();
		
		UpdateDisplay(Name);

		if (m_List.GetItemCount() == 1)
		{
			m_List.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
			m_List.Invalidate();
			UpdateButtons();
		}
		else
		{
			// We work out which is the best possible one to select


		}
	}

	else if (m_iStep == 2)
	{
		int curSel = m_ACEList.GetSelectedItem();
		CString name = m_ACEList.GetItemText(curSel,NULL);

		// Clear list
		m_ACEList.DeleteAllItems();
		m_ACEList.RemoveAllGroups();
	
		UpdateDisplay(name, false);

		m_ACEList.EnableGroups();

		if (m_ACEList.GetItemCount() == 1)
		{
			m_ACEList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
			m_ACEList.Invalidate();
			UpdateButtons();
		}
	} 

	m_Find.SetFocus();
}

bool CEventWizardDlg::ApproveItem(CString Item)
{
	// Check against filter
	if (m_Filter == "") return true;

	CString Compare = Item;
	Compare.MakeLower();
	Compare.Remove('*');

	if (Compare.Find(m_Filter) != -1) return true;
	else return false;
}

class SortedACE
{
public:

	SortedACE(CString name_, int i_, int category_)
	:		name(name_),
			i(i_),
			category(category_)
	{
	}

	bool operator < (const SortedACE& b)
	{
		CString first = name;
		CString second = b.name;

		first.Remove('*');
		second.Remove('*');

		return first < second;
	}

	CString name;
	int i;
	int category;

};

void CEventWizardDlg::UpdateDisplay(CString Previous, bool addTabs)
{
	if (m_iStep == 1)
	{
		// Add objects in
		LVITEM InsertPlugin;

		// Mask: TEXT, IMAGE, LPARAM, STATE
		InsertPlugin.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE | LVIF_GROUPID;
		InsertPlugin.state = 0;
		InsertPlugin.stateMask = 0;
		InsertPlugin.iItem = 0;
		InsertPlugin.iSubItem = 0;
		InsertPlugin.iGroupId = 0;

		// Normal objects
		int index = 0;

		for(map<CString, int>::iterator i = object_folders.begin(); i!= object_folders.end(); i++)
		{
			m_List.AddGroup(i->second, i->first);
			index++;
		}
		int Icon = 1;

		// System object
		if (ApproveItem(SYSTEM))
		{
			InsertPlugin.iImage = 0;
			InsertPlugin.iGroupId = object_folders["Object"];
			InsertPlugin.pszText = new char[SYSTEM.GetLength() + 1];
			strcpy(InsertPlugin.pszText, SYSTEM);
			ListView_InsertItem(m_List.m_hWnd, &InsertPlugin);

			if(SYSTEM == Previous)
				m_List.SetItemState(0,LVIS_SELECTED, LVIS_SELECTED);

			m_List.SetItemData(0, application->m_System.ObjectIdentifier);	
		}

		// Now loop the otList backwards to display each object icon
		for(vector<CObjType*>::iterator i = m_otList.begin(); i != m_otList.end(); i++)
		{	
			CString Name = (*i)->GetName();

			if (ApproveItem(Name))
			{
				InsertPlugin.iItem = ListView_GetItemCount(m_List.m_hWnd);
				InsertPlugin.iImage = Icon;
				InsertPlugin.pszText = Name.GetBuffer(Name.GetLength());
				InsertPlugin.iGroupId = object_folders[(*i)->GetFolder()];
				ListView_InsertItem(m_List.m_hWnd, &InsertPlugin);

				m_List.SetItemData(m_List.GetItemCount() - 1, (*i)->ObjectIdentifier);	

				if((*i)->GetName() == Previous)
					m_List.SetItemState(m_List.GetItemCount() - 1, LVIS_SELECTED, LVIS_SELECTED);
			}

			// Increment icon number
			Icon++;
		}	

		// Add families
		m_List.AddGroup(object_folders.size(), "Families");

		bool bFamilies = false;
		list<Family>::iterator f = application->families.begin();

		for ( ; f != application->families.end(); f++)
		{
			if(f->is_in_use)
			{
				bFamilies = true;

				if (ApproveItem(f->name))
				{
					InsertPlugin.iItem = ListView_GetItemCount(m_List.m_hWnd);
					InsertPlugin.iImage = Icon;
					InsertPlugin.iGroupId = object_folders.size();
					InsertPlugin.pszText = f->name.GetBuffer(f->name.GetLength());
					ListView_InsertItem(m_List.m_hWnd, &InsertPlugin);

					m_List.SetItemData(m_List.GetItemCount() - 1, f->identifier);

					if(f->name == Previous)
						m_List.SetItemState(m_List.GetItemCount() - 1,LVIS_SELECTED, LVIS_SELECTED);
				}
			}
			Icon++;
		}

		if (!bFamilies && object_folders.size() <= 1) m_List.RemoveAllGroups();
		else m_List.EnableGroups();
	}

	else if (m_iStep == 2)
	{
		if (addTabs) {
			m_CategoryTabs.ItemInsert("All");

			m_bOK = true;
			m_CategoryTabs.SelectionSet(0); //default
			m_bOK = false;
		}

		int index = 1;
		for(map<CString, int>::iterator i = categories.begin(); i != categories.end(); i++)
		{
			CString tabText = i->first;
			tabText.Replace("&", "&&");

			if (addTabs)
				m_CategoryTabs.ItemInsert(tabText,0,0,0,index++);

			m_bOK = true;
			if(i->first == SelectedCategory && addTabs)
				m_CategoryTabs.SelectionSet(m_CategoryTabs.ItemGetCount()-1);
			m_bOK = false;

			m_ACEList.AddGroup(i->second, i->first);
		}

		m_ACEList.bold_items.clear();

		// BOOM!  The vector element pMov points to is nuked
		//CObjType* pType = application->FindObjTypeFromNumber(m_pSelectedObject);
		//pType = application->FindObjTypeFromNumber(m_pSelectedObject);
		CObjType* pType = step2_type;

		if(pType)
		{	
			// Get all categories
			CObjType temp;
			if(pMov)
			{
				temp.CreateFromBehavior(pMov);
				pType = &temp;
			}

			if(pEff)
			{
				temp.CreateFromEffect(pEff);
				pType = &temp;
			}	

			vector<SortedACE> aces;

			for (int i = 0; i < pType->GetTableCount(m_aceType); i++)
			{
				ACESEntry2* pAce = pType->GetACESEntry(m_aceType, i);
				if(pAce == NULL || pAce->aceListName == "")
					continue; // ignore null entries

				// Verify it meets the filter
				if (!ApproveItem(pAce->aceListName))
					continue;

				CString category = pAce->aceCategory;
				
				if(SelectedCategory == "Common")
				{
					if(i >= 255) 
						i = pType->GetTableCount(m_aceType); //skip anything about the first aces
				}
				else
					if(SelectedCategory != "All" && SelectedCategory != category)
						continue; // if its not the current category

				aces.push_back(SortedACE(pAce->aceListName, i, categories[category]));
			}

			// now sort
			sort(aces.begin(), aces.end());

			for (int j = 0; j < aces.size(); j++)
			{
				// If *, bold it and remove star
				//MessageBox("fast bullets 3");
				CString name = aces[j].name;
				if (aces[j].name.Left(1) == "*")
				{
					name = aces[j].name.Right(aces[j].name.GetLength() - 1);
					m_ACEList.InsertItem(j, name, NULL, aces[j].category);
					m_ACEList.bold_items[j] = true;
				}

				else
				{
					m_ACEList.InsertItem(j, name, NULL, aces[j].category);
					m_ACEList.bold_items[j] = false;
				}
				//MessageBox("fast bullets 4");

				

				m_ACEList.SetItemData(j, aces[j].i);
				if(name == Previous)
				{
					m_ACEList.SetItemState(j, LVIS_SELECTED, LVIS_SELECTED);
				}

				


				//MessageBox("fast bullets 5");
				
				//m_ACEList.SetItemState(j, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
	}
}