#include "stdafx.h"
#include "..\..\Construct.h"
#include "Remove Family Variable Dlg.h"

// CRemoveFamilyVarDlg dialog
IMPLEMENT_DYNAMIC(CRemoveFamilyVarDlg, CDialog)

CRemoveFamilyVarDlg::CRemoveFamilyVarDlg(CWnd* pParent /*=NULL*/)
	: CExtWS<CDialog>(CRemoveFamilyVarDlg::IDD, pParent)
{

}

CRemoveFamilyVarDlg::~CRemoveFamilyVarDlg()
{
}

void CRemoveFamilyVarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_Objects);
}

BEGIN_MESSAGE_MAP(CRemoveFamilyVarDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CRemoveFamilyVarDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CRemoveFamilyVarDlg message handlers
BOOL CRemoveFamilyVarDlg::OnInitDialog()
{
	BOOL ret = CExtWS<CDialog>::OnInitDialog();

	SetWindowText(CString("Construct : Remove Family Variable: ") + varName);

	m_Objects.InsertColumn(0, "Remove", 0, 100);
	m_Objects.InsertColumn(1, "Object name", 0, 260);
	m_Objects.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	vector<CObjType*>::iterator i = objectsToList.begin();

	for ( ; i != objectsToList.end(); i++) {
		int nItem = m_Objects.GetItemCount();

		m_Objects.InsertItem(nItem, "");
		m_Objects.SetCheckbox(nItem, 0, true);
		m_Objects.SetItemText(nItem, 1, (*i)->GetName());
	}

	SetDlgItemText(IDC_VARNAMEEDIT, varName);

	m_Objects.SetHeaderCheckedState(0, 2);

	SubclassChildControls();

	return ret;
}

void CRemoveFamilyVarDlg::OnBnClickedOk()
{
	if (m_Objects.CountCheckedItems(0) == 0) {
		MessageBox("No items were checked.  The private variable will not be removed.", "Remove private variable");
		CExtWS<CDialog>::OnOK();
		return;
	}

	else {
		CString msg;
		msg.Format("This action will remove all events relating to the family private variable '%s', as well as events referring to the variable of that name in the %d selected objects.  "
			"Do you want to continue?", varName, m_Objects.CountCheckedItems(0));
		
		if (MessageBox(msg, "Remove family variable", MB_ICONEXCLAMATION | MB_YESNO) == IDNO)
			return;
	}

	int count = m_Objects.GetItemCount();

	for (int i = 0; i < count; i++) {
		if (m_Objects.GetCheckbox(i, 0)) {
			
			// Delete from the object
			CObjType* pType = objectsToList[i];

			int index = find_index(pType->m_PrivateValues.begin(), pType->m_PrivateValues.end(), varName);
			
			pType->m_PrivateValues.erase(pType->m_PrivateValues.begin() + index);

			PrivateVariableInstanceDeleter updater(index);
			ForEachObjectInstance(pType, m_app, updater);

		}
	}

	CExtWS<CDialog>::OnOK();
}
