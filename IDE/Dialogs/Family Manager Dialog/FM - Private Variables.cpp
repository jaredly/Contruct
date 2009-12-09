#include "Stdafx.h"
#include "Family Manager Dlg.h"
#include "FM - Private Variables.h"
#include "Remove Family Variable Dlg.h"

IMPLEMENT_DYNAMIC(CFMPrivateVariables, CPropertySheet)

CFMPrivateVariables::CFMPrivateVariables(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CFMPrivateVariables::IDD, 0)
{
	m_pFamily = NULL;
}

void CFMPrivateVariables::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_LIST, m_Items);
	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFMPrivateVariables, CPropertyPage)
	ON_BN_CLICKED(IDC_REMOVE, &CFMPrivateVariables::OnRemove)
	ON_BN_CLICKED(IDC_ADD, &CFMPrivateVariables::OnAdd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CFMPrivateVariables::OnItemChanged)
END_MESSAGE_MAP()

BOOL CFMPrivateVariables::OnSetActive()
{
	CPropertyPage::OnSetActive();
	CPropertyPage::OnInitDialog();

	CListCtrl* pItems = (CListCtrl*)GetDlgItem(IDC_LIST);

	// Set up list control
	pItems->DeleteColumn(0);
	pItems->DeleteColumn(0);
	pItems->InsertColumn(1, "Name", LVCFMT_LEFT, 200);
	pItems->SetExtendedStyle(LVS_EX_FULLROWSELECT);

	if (m_pFamily)
	{
		m_Add.EnableWindow();
		ShowFamily(m_pFamily);
	}

	return TRUE;
}

void CFMPrivateVariables::ShowFamily(Family* pFamily)
{
	m_pFamily = pFamily;

	m_Items.DeleteAllItems();
	m_Add.EnableWindow(FALSE);

	if (!m_pFamily)
		return;

	if (!IsWindow(m_hWnd))
		return;

	m_Add.EnableWindow();
	m_Remove.EnableWindow(FALSE);

	vector<FamilyVariable>::iterator v = pFamily->variables.begin();

	int Count = 0;

	for ( ; v != pFamily->variables.end(); v++) 
	{
		if (v->valid)
			m_Items.InsertItem(Count, v->name, 0);

		Count++;
	}
}

void CFMPrivateVariables::OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	POSITION Pos = m_Items.GetFirstSelectedItemPosition();
	int Item = m_Items.GetNextSelectedItem(Pos);

	if (Item != -1)
		m_Remove.EnableWindow();
	else
		m_Remove.EnableWindow(FALSE);

	*pResult = 0;
}

void CFMPrivateVariables::OnRemove()
{
	POSITION Pos = m_Items.GetFirstSelectedItemPosition();
	int Item = m_Items.GetNextSelectedItem(Pos);

	if (Item == -1) return;

	CRemoveFamilyVarDlg Dlg;
	CString Name = m_Items.GetItemText(Item, 0);
	Dlg.varName = Name;
	m_pFamily->GetObjectTypes(Dlg.objectsToList);
	Dlg.m_app = m_pParent->application;

	// OnOK() implements removal
	Dlg.DoModal();

	m_pFamily->UpdateFamilyData();

	ShowFamily(m_pFamily);
}

void CFMPrivateVariables::OnAdd()
{
	CAddVariableDlg Dlg;
	Dlg.Caption = "Construct : Add Family Variable";
	Dlg.Name.Format("Variable %d", m_Items.GetItemCount() + 1);

	if (Dlg.DoModal() == IDOK) 
	{
		// First verify this private variable doesnt already exist
		if (m_pFamily->VariableNameExists(Dlg.Name)) 
		{
			CString msg;
			msg.Format("The private variable '%s' already exists in this family.  Please choose a unique name.", Dlg.Name);
			MessageBox(msg, "Add family variable", MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		vector<CObjType*> objects;
		m_pFamily->GetObjectTypes(objects);

		vector<CObjType*>::iterator i = objects.begin();

		// For each object type in the family
		for ( ; i != objects.end(); i++) {

			CObjType* pType = (*i);

			// If the object type already has this variable then we don't need to do anything
			if (pType->PrivateVariableExists(Dlg.Name))
				continue;

			// Add the new variable to the object type
			int id = pType->VariableID++;
			pType->m_PrivateValues.push_back(PrivateValue(Dlg.Name, Dlg.Type, id));

			// Add the new variable to the corresponding instance values
			PrivateVariableInstanceAdder updater(id, Dlg.Value);
			ForEachObjectInstance(pType, m_pParent->application, updater);
		}
	}

	m_pFamily->UpdateFamilyData();

	ShowFamily(m_pFamily);
}