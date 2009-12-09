/////////////////////////////////////////////////////////////////////////////
// Event Wizard Dialog
/////////////////////////////////////////////////////////////////////////////

// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "EventWizardDlg.h"
#include "..\Object Info Dlg.h"
#include "..\ParametersDlg.h"

void CEventWizardDlg::OnBnClickedNext()
{
	if (!PreChangeStep())
	{
		return;
	}

	// Step 1, to step 2
	if (m_iStep == 1)
	{			
		m_iStep = 2;
		ChangeStep();
	}

	// Step 2, to step 3
	else if (m_iStep == 2)
	{
		CString Step;
		m_Next.GetWindowText(Step);
		
		if (Step == "Next")
		{
			m_iStep = 3;
			ChangeStep();
		}

		else
		{
			param_strings.RemoveAll();
			param_types.clear();
			EndDialog(IDOK);
			return;
		}
	}

	// Finish
	else if (m_iStep == 3)
	{
		EndDialog(IDOK);
		return;
	}

	UpdateButtons();
}

void CEventWizardDlg::OnBnClickedBack()
{
	// Step 2, to step 1
	if (m_iStep == 2)
	{
		PreChangeStep();

		m_iStep = 1;
		ChangeStep();
	}

	// Step 3, to step 2
	else if (m_iStep == 3)
	{
		m_iStep = 2;

		m_Params->DestroyWindow();
		delete m_Params;

		ChangeStep();
	}

	UpdateButtons();
}

bool CEventWizardDlg::PreChangeStep()
{
	if (m_iStep == 1)
	{
		POSITION Pos = m_List.GetFirstSelectedItemPosition();
		int Sel = m_List.GetNextSelectedItem(Pos);

		if (Sel == -1)
		{
			return false;
		}

		else
		{
			m_pSelectedObject = m_List.GetItemData(Sel);
			return true;
		}
	}
	if(m_iStep == 2)
	{
		int sel = m_ACEList.GetSelectedItem();
		m_aceIndex = m_ACEList.GetItemData(sel);

		if (sel == -1)
		{
			return false;
		}

		CObjType* pType = application->FindObjTypeFromNumber(m_pSelectedObject);
		if(pType)
		{
			int sel = m_Tabs.SelectionGet();
			if(sel == 0) // object
			{
				m_childid = -1;		
				pAce = pType->GetACESEntry(m_aceType, m_aceIndex);
			}

			else if(sel <= pType->behaviors.size())
			{
				list<CBehavior>::iterator i = pType->behaviors.begin();
				advance(i, sel-1);
				m_childid = i->id;
				CObjType tmp;
				tmp.CreateFromBehavior(&*i);

				pAce = tmp.GetACESEntry(m_aceType, m_aceIndex);
			}
			else
			{
				sel -= pType->behaviors.size() + 1;
				list<CEffect>::iterator i = pType->effects.begin();
				advance(i, sel);
				m_childid = -i->m_effectID;
			
				CObjType tmp;
				tmp.CreateFromEffect(&*i);

				pAce = tmp.GetACESEntry(m_aceType, m_aceIndex);
			}
		}
	}

	if(m_iStep == 3)
	{
		bool valid = m_Params->OnFinish();

		if (valid) {
			param_strings.RemoveAll();

			for(int a = 0; a < m_Params->strings.GetCount(); a++)
			{
				CString test = m_Params->strings.GetAt(a);
				param_strings.Add(m_Params->strings.GetAt(a));
			}
			
			param_types = m_Params->types;
			m_Params->DestroyWindow();
			delete m_Params;
		}
		else
			return false;
	}

	return true;
}
void CEventWizardDlg::ChangeStep()
{
	m_locked = true;
	if (m_iStep == 1)
	{
		// Show object window, nothing else
		m_List.ShowWindow(SW_SHOW);
		m_ACEList.ShowWindow(SW_HIDE);
		m_CategoryTabs.ShowWindow(SW_HIDE);
		m_Tabs.ShowWindow(SW_HIDE);

		m_Find.SetWindowText("");
		m_Filter = "";

		int Sel = m_List.GetSelectedItem();
		CString Name = m_List.GetItemText(Sel, NULL); // Previously selected name

		// Clear list
		m_List.DeleteAllItems();

		// New - call UpdateDisplay to account for filter
		UpdateDisplay(Name);

		m_Find.SetFocus();
	}

	if (m_iStep == 2)
	{
		m_List.ShowWindow(SW_HIDE);
		m_ACEList.ShowWindow(SW_SHOW);
		m_CategoryTabs.ShowWindow(SW_SHOW);
		m_Tabs.ShowWindow(SW_SHOW);

		m_Find.SetWindowText("");
		m_Filter = "";

		int curSel = m_ACEList.GetSelectedItem();
		CString name = m_ACEList.GetItemText(curSel,NULL);

		// Clear list
		m_ACEList.DeleteAllItems();
		m_ACEList.RemoveAllGroups();
	
		// Remember the previous strings
		//SelectedTabIndex = m_Tabs.SelectionGet();
		SelectedTabIndex = tabSelectionIndex;

		int SelectedCategoryIndex = m_CategoryTabs.SelectionGet();
		
		if(SelectedCategoryIndex != -1)
			SelectedCategory = m_CategoryTabs.ItemTextGet(SelectedCategoryIndex);
		if(SelectedCategory == "")
			SelectedCategory = "All"; // force it to display all if theres no category

		if(SelectedTabIndex == -1)
			SelectedTabIndex = 0;
		
		if(SelectedCategoryIndex == -1)
			SelectedCategoryIndex = 0;

		SelectedCategory.Replace("&&", "&");

		m_Tabs.ItemRemoveAll();
		m_CategoryTabs.ItemRemoveAll();

		pMov = NULL;
		pEff = NULL;

		CObjType* pType = application->FindObjTypeFromNumber(m_pSelectedObject);
		step2_type = pType;

		if(pType)
		{
			categories.clear();
			list<CString> tabs;
			
			// Get all tabs
			tabs.push_back(pType->GetName());
			int tab_index = 0;

			// Add movements to top
			for(list<CBehavior>::iterator i = pType->behaviors.begin(); i != pType->behaviors.end(); i++)
			{
				if(tabs.size() == SelectedTabIndex)
				{
					pMov = &*i;
					tab_index = tabs.size();
				}

				tabs.push_back(i->Text);
			}

			// Add effects to top, but not if this is a condition
			if (m_aceType != CONDITION)
			{
				for(list<CEffect>::iterator i = pType->effects.begin(); i != pType->effects.end(); i++)
				{
					if(tabs.size() == SelectedTabIndex)
					{
						pMov = NULL;
						pEff = &*i;
						tab_index = tabs.size();
					}

					tabs.push_back(i->Text);
				}
			}

			for(list<CString>::iterator i = tabs.begin(); i!= tabs.end(); i++)
				m_Tabs.ItemInsert(*i,0,true,0,m_Tabs.ItemGetCount());

			// Select previous selection
			if (SelectedTabIndex < tabs.size())
				m_Tabs.SelectionSet(SelectedTabIndex);
			else
				m_Tabs.SelectionSet(0);
	
			if(m_Tabs.SelectionGet() == -1)
				m_Tabs.SelectionSet(0);

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
		
			for (int i = 0; i < pType->GetTableCount(m_aceType); i++)
			{
				ACESEntry2* pAce = pType->GetACESEntry(m_aceType, i);
				if(pAce == NULL || pAce->aceListName == "")
					continue; // Ignore null entries

				CString category = pAce->aceCategory;
				if(categories.find(category) == categories.end())
					categories[category] = categories.size();
			}

			
			
			UpdateDisplay(name);

			
			m_ACEList.EnableGroups();
			m_ACEList.SetFocus();
			m_Tabs.Invalidate();
			m_CategoryTabs.Invalidate();
		}

		m_Find.SetFocus();
	}

	if (m_iStep == 3)
	{
		m_Find.SetWindowText("");
		m_Filter = "";

		m_List.ShowWindow(SW_HIDE);
		m_ACEList.ShowWindow(SW_HIDE);
		m_CategoryTabs.ShowWindow(SW_HIDE);
		m_Tabs.ShowWindow(SW_HIDE);

		m_Params = new CParametersDlg;
		CParametersDlg& parametersDialog = *m_Params;

		parametersDialog.application = application;
		parametersDialog.layout = layout;
		parametersDialog.m_pEventEditor = (CEventEditor*)this->GetParent();
		parametersDialog.objectMap = &application->object_types;

		parametersDialog.oid = m_pSelectedObject;

		// Moved to a permanent scope - sometimes referenced outside of this function -Ash
		// see CEventWizardDlg::tmp
		CObjType* pType = application->FindObjTypeFromNumber(m_pSelectedObject);
		parametersDialog.m_selfType = pType;	// Use the base type for 'Self' - not a movement or effect

		parametersDialog.mid = -1;

		// Check if we're editing
		if(m_childid < -1)
		{
			tmp.CreateFromEffect(pType->GetEffect(-m_childid));
			pType = &tmp; 
		}

		if(m_childid > -1)
		{
			tmp.CreateFromBehavior(pType->GetBehaviorByUniqueID(m_childid));
			pType = &tmp;
			parametersDialog.mid = m_childid;
		}

		parametersDialog.aceID = m_aceIndex;//m_editAceIndex == -1 ? m_aceIndex : m_editAceIndex;

		ACESEntry2* pAce = 	parametersDialog.m_pACEEntry = pType->GetACESEntry(m_aceType, parametersDialog.aceID);
	
		parametersDialog.m_ID = pType;

		for(int a = 0; a < min(pAce->params.size(),param_strings.GetCount()); a++)
		{
			CString paramText = param_strings.GetAt(a);
			int paramType = param_types.at(a);

			//If we have changed the ace, we need to check the params match to the types
			PARAM& Param = pAce->params.at(a);	
			if(param_types.at(a) != Param.type  && param_types.at(a) != EDITORPARAM_USERPARAM)
			{
				
				CString Default = Param.initStr;
				if(Param.type == 9)
					Default = "0";
				if (Default == "")
				{	
					switch (Param.type)
					{
						case 1: Default = "0"; break;
						case 2: Default = "\"\""; break;
						case 11: Default = "\"\""; break;
						default: Default = "0"; break;
					}
				}
				paramText = Default;
				paramType = Param.type;
			}
			m_Params->strings.Add(paramText);
			m_Params->types.push_back(paramType);
		}

		CRect Rect;
		GetClientRect(&Rect);
		Rect.top = 47;
		Rect.bottom -= 42;
	
		parametersDialog.Create(IDD_PARAMETERS, this);
		parametersDialog.MoveWindow(Rect);

		dlgAnchor.Add(parametersDialog.GetSafeHwnd(),  ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);	
	}

	m_locked = false;

	Invalidate();
}
