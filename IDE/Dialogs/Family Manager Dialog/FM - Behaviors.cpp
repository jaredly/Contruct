#include "Stdafx.h"
#include "Family Manager Dlg.h"
#include "FM - Effects.h"
#include "..\NewHLSLDlg.h"

IMPLEMENT_DYNAMIC(CFMBehaviors, CPropertySheet)

CFMBehaviors::CFMBehaviors(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CFMBehaviors::IDD, 0)
{
	m_pFamily = NULL;
}

void CFMBehaviors::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_LIST, m_Items);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFMBehaviors, CPropertyPage)
	ON_BN_CLICKED(IDC_REMOVE, &CFMBehaviors::OnRemove)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CFMBehaviors::OnItemChanged)
END_MESSAGE_MAP()

BOOL CFMBehaviors::OnSetActive()
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
		ShowFamily(m_pFamily);

	return TRUE;
}

void CFMBehaviors::ShowFamily(Family* pFamily)
{
	m_pFamily = pFamily;

	m_Items.DeleteAllItems();

	if (!m_pFamily)
		return;

	if (!IsWindow(m_hWnd))
		return;

	m_Remove.EnableWindow(FALSE);

	vector<FamilyBehavior>::iterator v = pFamily->behaviors.begin();

	int Count = 0;

	for ( ; v != pFamily->behaviors.end(); v++) 
	{
		if (v->valid)
		{
			int item = m_Items.InsertItem(Count, v->name, 0);
			m_Items.SetItemData(item, (DWORD)(&pFamily->behaviors.at(Count)));
		}

		Count++;
	}
}

void CFMBehaviors::OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
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

class RemoveBehavior
{
public:
	CObjType* type;
	list<CBehavior>::iterator position;
};

void CFMBehaviors::OnRemove()
{
	POSITION Pos = m_Items.GetFirstSelectedItemPosition();
	int Item = m_Items.GetNextSelectedItem(Pos);

	if (Item == -1) return;

	vector<RemoveBehavior> remove;

	// Remove this effect from each object in the family
	FamilyBehavior* pFamilyBehavior = (FamilyBehavior*)m_Items.GetItemData(Item);

	for(int r = 0; r < pFamilyBehavior->reference_ids.size(); r++)
	{ 	
		FamilyAssociate* assoc = &pFamilyBehavior->reference_ids.at(r);
		CObjType* pType = m_pParent->application->FindObjTypeFromNumber(assoc->object);

		list<CBehavior>::iterator ToDelete;

		for(list<CBehavior>::iterator i = pType->behaviors.begin(); i!= pType->behaviors.end(); i++)
		{
			if(i->id == assoc->item_id)
			{
				RemoveBehavior r;
				r.type = pType;
				r.position = i;
				remove.push_back(r);
			}
		}
	}

	for (int i = 0; i < remove.size(); i++)
		remove[i].type->behaviors.erase(remove[i].position);

	m_pFamily->UpdateFamilyData();

	ShowFamily(m_pFamily);
}