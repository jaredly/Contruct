#include "Stdafx.h"
#include "Family Manager Dlg.h"
#include "FM - Effects.h"
#include "..\NewHLSLDlg.h"

IMPLEMENT_DYNAMIC(CFMEffects, CPropertySheet)

CFMEffects::CFMEffects(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CFMEffects::IDD, 0)
{
	m_pFamily = NULL;
}

void CFMEffects::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_LIST, m_Items);
	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFMEffects, CPropertyPage)
	ON_BN_CLICKED(IDC_REMOVE, &CFMEffects::OnRemove)
	ON_BN_CLICKED(IDC_ADD, &CFMEffects::OnAdd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CFMEffects::OnItemChanged)
END_MESSAGE_MAP()

BOOL CFMEffects::OnSetActive()
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

void CFMEffects::ShowFamily(Family* pFamily)
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

	vector<FamilyEffect>::iterator v = pFamily->effects.begin();

	int Count = 0;

	for ( ; v != pFamily->effects.end(); v++) 
	{
		if (v->valid)
			m_Items.InsertItem(Count, v->name, 0);

		Count++;
	}
}

void CFMEffects::OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
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

void CFMEffects::OnRemove()
{
	POSITION Pos = m_Items.GetFirstSelectedItemPosition();
	int Item = m_Items.GetNextSelectedItem(Pos);

	if (Item == -1) return;

	// Remove this effect from each object in the family
	FamilyEffect* pFamilyEffect = &m_pFamily->effects.at(Item);
	for(int r = 0; r < pFamilyEffect->reference_ids.size(); r++)
	{ 	
		FamilyAssociate* assoc = &pFamilyEffect->reference_ids.at(r);
		CObjType* pType = m_pParent->application->FindObjTypeFromNumber(assoc->object);

		list<CEffect>::iterator ToDelete;

		for(list<CEffect>::iterator i = pType->effects.begin(); i!= pType->effects.end(); i++)
		{
			if(i->m_effectID == assoc->item_id)
				ToDelete = i;
		}

		pType->effects.erase(ToDelete);
	}

	m_pFamily->UpdateFamilyData();

	ShowFamily(m_pFamily);
}

void CFMEffects::OnAdd()
{
	CNewHLSLDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		vector<CObjType*> Objects;
		m_pFamily->GetObjectTypes(Objects);

		for (int i = 0; i < Objects.size(); i++)
		{
			CString fxName = Objects[i]->GetUniqueEffectName(dlg.m_Text);
			Objects[i]->effects.push_back(CEffect());
			CEffect& Effect = Objects[i]->effects.back();
			Effect.m_effectID = m_pParent->application->m_objID++;
			Effect.effectFile = dlg.m_Filename;
			Effect.Text = fxName;
			Effect.version = dlg.m_Version;	
			Effect.initialised = false;
		}
	}

	m_pFamily->UpdateFamilyData();

	ShowFamily(m_pFamily);
}