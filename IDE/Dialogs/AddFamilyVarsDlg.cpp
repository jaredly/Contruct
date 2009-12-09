// Dialogs\AddFamilyVarsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "AddFamilyVarsDlg.h"


// CAddFamilyVarsDlg dialog

IMPLEMENT_DYNAMIC(CAddFamilyVarsDlg, CDialog)

CAddFamilyVarsDlg::CAddFamilyVarsDlg(CWnd* pParent /*=NULL*/)
	: CExtWS<CDialog>(CAddFamilyVarsDlg::IDD, pParent)
{

}

BOOL CAddFamilyVarsDlg::OnInitDialog()
{
	BOOL ret = CExtWS<CDialog>::OnInitDialog();

	m_ObjName.SetWindowText(pType->GetName());
	m_FamilyName.SetWindowText(pFamily->name);

	m_VarsList.InsertColumn(0, "Variable name", 0, 200);
	m_VarsList.InsertColumn(1, "Add?", 0, 150);
	m_VarsList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	pFamily->UpdateFamilyData();
	totalCheckboxes = 0;

	vector<FamilyVariable>::iterator i = pFamily->variables.begin();

	for ( ; i != pFamily->variables.end(); i++) {
		if (!i->valid)
			continue;

		int nItem = m_VarsList.GetItemCount();
		m_VarsList.InsertItem(nItem, i->name);

		// This family variable already exists in the object
		if (pType->PrivateVariableExists(i->name))
			m_VarsList.SetItemText(nItem, 1, "Already exists");
		else {
			m_VarsList.SetCheckbox(nItem, 1, true);
			totalCheckboxes++;
		}
	}

	m_VarsList.SetHeaderCheckedState(1, 2);
	SubclassChildControls();

	return ret;
}

CAddFamilyVarsDlg::~CAddFamilyVarsDlg()
{
}

void CAddFamilyVarsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VARSLIST, m_VarsList);
	DDX_Control(pDX, IDC_OBJNAME, m_ObjName);
	DDX_Control(pDX, IDC_FAMILYNAME, m_FamilyName);
}


BEGIN_MESSAGE_MAP(CAddFamilyVarsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAddFamilyVarsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddFamilyVarsDlg message handlers

void CAddFamilyVarsDlg::OnBnClickedOk()
{
	varsToAdd.resize(0);

	for (int i = 0; i < m_VarsList.GetItemCount(); i++) {
		if (m_VarsList.GetCheckbox(i, 1) == 1) {	// getcheckbox returns -1 when one does not exist
			varsToAdd.push_back(m_VarsList.GetItemText(i, 0));
		}
	}

	if (varsToAdd.empty()) {
		if (MessageBox("All events referring to the unselected family variables will be removed.",
			"Family variables", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK)
		{
			CDialog::OnOK();
		}
		return;
	}
	else if (totalCheckboxes - varsToAdd.size() > 0) {
		CString msg;
		msg.Format("All events referring to the %d unchecked family variable(s) will be removed.", totalCheckboxes - varsToAdd.size());

		if (MessageBox(msg, "Family variables", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK)
			CDialog::OnOK();

		return;
	}
	else
		CDialog::OnOK();
}
