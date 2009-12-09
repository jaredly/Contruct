// Dialogs/OnFuncDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OnFuncDlg.h"
#include "..\Resource.h"

// COnFuncDlg dialog

IMPLEMENT_DYNAMIC(COnFuncDlg, CDialog)

COnFuncDlg::COnFuncDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COnFuncDlg::IDD, pParent)
{

	//m_params.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,100,100),this,0,0);
}

COnFuncDlg::~COnFuncDlg()
{
}

void COnFuncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_OBJECTS, m_Objects);
	DDX_Control(pDX, IDC_PARAMTREE, m_Params);
	DDX_Control(pDX, IDC_LIST2, m_Functions);
}


BEGIN_MESSAGE_MAP(COnFuncDlg, CDialog)
ON_BN_CLICKED(IDC_ADD, &COnFuncDlg::OnBnClickedAdd)
ON_BN_CLICKED(IDC_REMOVE, &COnFuncDlg::OnBnClickedRemove)
ON_BN_CLICKED(IDC_UP, &COnFuncDlg::OnBnClickedUp)
ON_BN_CLICKED(IDC_DOWN, &COnFuncDlg::OnBnClickedDown)
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &COnFuncDlg::OnTcnSelchangeTab1)
ON_BN_CLICKED(IDOK2, &COnFuncDlg::OnBnClickedOk2)
ON_BN_CLICKED(IDOK3, &COnFuncDlg::OnBnClickedOk3)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_OBJECTS, &COnFuncDlg::OnLvnItemchangedObjects)
ON_LBN_SELCHANGE(IDC_LIST2, &COnFuncDlg::OnLbnSelchangeList2)
END_MESSAGE_MAP()



BOOL COnFuncDlg::OnInitDialog()
{
	ready = false;
	CDialog::OnInitDialog();
	m_tab.InsertItem(0,"Action");
	m_tab.InsertItem(0,"Condition");
	m_tab.InsertItem(0,"Expression");


	// fill our listbox with all the objects including the system object

	int currentPosition = 1;
	// Create the image list and add the System icon
	imageList.Create(32, 32, ILC_COLOR24, 3, 3);
	m_Objects.SetImageList(&imageList, LVSIL_NORMAL);
	HBITMAP systemBitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SYSTEM));
	int currentIcon = ImageList_Add(imageList, systemBitmap, systemBitmap);


	// Add this plugin
	//LVITEM insertPlugin;

	// Mask: TEXT, IMAGE, LPARAM, STATE
/*	insertPlugin.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	insertPlugin.state = 0;
	insertPlugin.stateMask = 0;
	insertPlugin.iItem = 0;
	insertPlugin.iImage = currentIcon;
	insertPlugin.iSubItem = 0;
	insertPlugin.pszText = new char[GetStr("45").GetLength() + 1];
	strcpy(insertPlugin.pszText, SYSTEM);
	ListView_InsertItem(m_Objects.m_hWnd, &insertPlugin);
	m_Objects.SetItemData(0, -1);	// System is -1*/

	// Prepare to iterate all objects
	/*POSITION pos = objMap->GetStartPosition();
	long mapKey;
	CObjType* objectType;

	CList<CObjType*, CObjType*&> otList;

	// Loop through and add all objects to the otList
	while (pos != NULL) 
	{
		objMap->GetNextAssoc(pos, mapKey, objectType);
		otList.AddTail(objectType);
	}

	// Now loop the otList backwards to display each object icon
	pos = otList.GetTailPosition();
	for (int i = 0; i < otList.GetCount(); i++)
	{
		objectType = otList.GetPrev(pos);

		HBITMAP Bitmap = objectType->m_Image.MakeBitmap();
		currentIcon = ImageList_Add(imageList, Bitmap, Bitmap);

		// Add the icon to the listview control for this object
		// Mask: TEXT, IMAGE, LPARAM, STATE
		insertPlugin.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
		insertPlugin.state = LVIS_SELECTED;
		insertPlugin.stateMask = 0;
		insertPlugin.iItem = ListView_GetItemCount(m_Objects.m_hWnd);
		insertPlugin.iImage = currentIcon;
		insertPlugin.iSubItem = 0;
		insertPlugin.pszText = objectType->m_name.GetBuffer(objectType->m_name.GetLength());
		ListView_InsertItem(m_Objects.m_hWnd, &insertPlugin);

		//CAHNGE by AG
		// Item data now stores CObjType pointer
		m_Objects.SetItemData(currentPosition, (DWORD)objectType);

		currentPosition++;
	}	

	ready = true;
	RefreshFunctions();
*/


	return TRUE;
}

void COnFuncDlg::OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult)
{
//    LPNMOPTIONTREE pNMOptionTree = (LPNMOPTIONTREE)pNotifyStruct;
//	COptionTreeItem* pItem = pNMOptionTree->otiItem;
//	if(pItem->GetInfoText() == "Add a new parameter")
//	{
//		//COptionTreeItem* pRoot = pItem->GetParent();
//		//m_params.DeleteAllItems();//DeleteItem(pItem);
//		/////////////////////////////////
//	/*	COptionTreeItemStatic* pStatic
//			= (COptionTreeItemStatic*)m_params.InsertItem(new COptionTreeItemStatic(), pRoot);
//		pStatic->SetLabelText  ("Parameter 1");
//*/ 
//		/////////////////////////////////
//	/*	COptionTreeItemEdit* pEdit 
//			= (COptionTreeItemEdit*)m_params.InsertItem(new COptionTreeItemEdit(), pRoot);
//		pEdit->SetLabelText  ("Description");
//		pEdit->CreateEditItem(NULL);*/
//		//pEdit->SetEditInt(app->m_Height);
//
//		/////////////////////////////////
//	/*	COptionTreeItemButton* pButton;
//		pButton = (COptionTreeItemButton*)m_params.InsertItem(new COptionTreeItemButton(), pRoot);
//		pButton->SetLabelText  ("");
//		pButton->SetInfoText ("Add a new parameter");
//		pButton->CreateButton  (0, "Add Param");
//*/
//
//
//	}


}
void COnFuncDlg::OnBnClickedAdd()
{
		//COptionTreeItem* pRoot = pItem->GetParent();
		//m_params.DeleteAllItems();//DeleteItem(pItem);
		/////////////////////////////////

	//COptionTreeItem* pRoot;
	//pRoot = m_params.InsertItem(new COptionTreeItem());
	//pRoot->SetLabelText(_T("Parameter 1"));
	//pRoot->Expand();
	
	/*COptionTreeItemButton* pButton;
	pButton = (COptionTreeItemButton*)m_params.InsertItem(new COptionTreeItemButton(), pRoot);
		pButton->SetLabelText  ("");
		pButton->SetInfoText ("Add a new parameter");
		pButton->CreateButton  (0, "Add Param");*/


	/*	COptionTreeItemStatic* pStatic
			= (COptionTreeItemStatic*)m_params.InsertItem(new COptionTreeItemStatic(), pRoot);
		pStatic->SetLabelText  ("Name");*/

		/////////////////////////////////
		/*COptionTreeItemEdit* pEdit 
			= (COptionTreeItemEdit*)m_params.InsertItem(new COptionTreeItemEdit(), pRoot);
		pEdit->SetLabelText  ("Name");
		pEdit->CreateEditItem(NULL);

		COptionTreeItemComboBox* pCombo 
			= (COptionTreeItemComboBox*)m_params.InsertItem(new COptionTreeItemComboBox(),pRoot);
		pCombo->SetLabelText ("Type");
		pCombo->CreateComboItem();
		pCombo->AddString("Unknown");
		pCombo->AddString("Value");
		pCombo->AddString("String");
		pCombo->AddString("Colour");
		pCombo->SetCurSel(0);*/

		//pEdit->SetEditInt(app->m_Height);

		/////////////////////////////////
	/*	COptionTreeItemButton* pButton;
		pButton = (COptionTreeItemButton*)m_params.InsertItem(new COptionTreeItemButton(), pRoot);
		pButton->SetLabelText  ("");
		pButton->SetInfoText ("Add a new parameter");
		pButton->CreateButton  (0, "Add Param");
*/
	// TODO: Add your control notification handler code here
}

void COnFuncDlg::OnBnClickedRemove()
{
	// TODO: Add your control notification handler code here
}

void COnFuncDlg::OnBnClickedUp()
{
	// TODO: Add your control notification handler code here
}

void COnFuncDlg::OnBnClickedDown()
{
	// TODO: Add your control notification handler code here
}

void COnFuncDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	RefreshFunctions();
}

void COnFuncDlg::OnBnClickedOk2()	// Add function or update function...
{
	ACESEntry2 entry;

	// reading what we have in the properties we need to create an entry;
	entry.resourceID = 0;
	GetDlgItem(IDC_EDIT1)->GetWindowText(entry.aceName);
	GetDlgItem(IDC_EDIT1)->GetWindowText(entry.aceDisplayText);
	entry.retrn = 0;				// flags

	HTREEITEM item = m_Params.GetFirstVisibleItem();
	while(item)
	{
		PARAM param;
		param.name = m_Params.GetItemText(item);
		param.desc = "";
		param.initStr = "";
		param.type = 0;

		entry.params.push_back(param);

		item = m_Params.GetNextVisibleItem(item);

	}


	// okay our entry is filled up with the info we have

	// we should now check if there are other functions with the same name to perform an 'update'...
	//    TODO

	// the are no items to update...so we add it to the vector and force an update

	vector<ACESEntry2>* pfunctions = GetACEsVector();

	if(pfunctions)
		pfunctions->push_back(entry);
	
	RefreshFunctions();


	// TODO: Add your control notification handler code here
}

void COnFuncDlg::OnBnClickedOk3()	//Remove function
{
	// TODO: Add your control notification handler code here
}

void COnFuncDlg::OnLvnItemchangedObjects(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	RefreshFunctions();
	//We have changed objects, update the functions
}

void COnFuncDlg::RefreshFunctions()
{
	if(!ready)
		return ShowControls(false);	

	m_Functions.ResetContent();

	vector<ACESEntry2>* pfunctions = GetACEsVector();

	if(pfunctions)
	{
		ShowControls(true);
		//loop and add all the functions to the list
		for(int x = 0; x < pfunctions->size(); x ++)
		{
			m_Functions.AddString((*pfunctions)[x].aceName);
		}
	}
	else
	{
		ShowControls(false);
	}
}

bool COnFuncDlg::ItemSelected()
{
	POSITION pos = m_Objects.GetFirstSelectedItemPosition();
	if(pos)
		return true;
	else
		return false;
}

bool COnFuncDlg::SystemSelected()
{
	POSITION pos = m_Objects.GetFirstSelectedItemPosition();
	if(m_Objects.GetItemData(m_Objects.GetNextSelectedItem(pos)) == -1)
		return true;
	else
		return false;
}

CObjType* COnFuncDlg::GetNonSystemObject()
{
	POSITION pos = m_Objects.GetFirstSelectedItemPosition();
	return (CObjType*)m_Objects.GetItemData(m_Objects.GetNextSelectedItem(pos));
}
// worst code i've written rofl!
bool COnFuncDlg::ActionFunctions()
{
	if(m_tab.GetCurSel()==0)
		return true;
	return false;
}
bool COnFuncDlg::ConditionFunctions()
{
	if(m_tab.GetCurSel()==1)
		return true;
	return false;
}
bool COnFuncDlg::ExpressionFunctions()
{
	if(m_tab.GetCurSel()==2)
		return true;
	return false;
}


vector<ACESEntry2>* COnFuncDlg::GetACEsVector()
{
	if(ItemSelected())
	{
	/*	if(SystemSelected())
		{
			if(ActionFunctions())
				return &m_level->m_CustomActions;
			if(ConditionFunctions())
				return &m_level->m_CustomConditions;
			if(ExpressionFunctions())
				return &m_level->m_CustomExpressions;
		}
		else*/
		{
			CObjType* type = GetNonSystemObject();
			if(ActionFunctions())
				return &type->m_CustomActions;
			if(ConditionFunctions())
				return &type->m_CustomConditions;
			if(ExpressionFunctions())
				return &type->m_CustomExpressions;

		}
	}
	return 0;
}
void COnFuncDlg::OnLbnSelchangeList2()
{
	// TODO: Add your control notification handler code here
}

void COnFuncDlg::ShowControls(bool show)
{


		GetDlgItem(IDC_LIST2)->ShowWindow(show);
		GetDlgItem(IDC_STATICNAME)->ShowWindow(show);
		GetDlgItem(IDC_STATICPARAMS)->ShowWindow(show);
		GetDlgItem(IDC_EDIT1)->ShowWindow(show);
		GetDlgItem(IDC_ADD)->ShowWindow(show);
		GetDlgItem(IDC_REMOVE)->ShowWindow(show);
		GetDlgItem(IDC_UP)->ShowWindow(show);
		GetDlgItem(IDC_DOWN)->ShowWindow(show);
	
		GetDlgItem(IDC_PARAMTREE)->ShowWindow(show);
		GetDlgItem(IDOK2)->ShowWindow(show);
		GetDlgItem(IDOK3)->ShowWindow(show);



}