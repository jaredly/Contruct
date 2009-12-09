// ControlsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "ControlsDlg.h"

// CControlsDlg dialog

IMPLEMENT_DYNAMIC(CControlsDlg, CDialog)

CControlsDlg::CControlsDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CControlsDlg::IDD, pParent)
{
	application = NULL;

	int i;
	for (i = 0; controlTable[i].vk != -2; i++)
		controls.Add(controlTable[i].name);

	// Last item
	controls.Add(controlTable[i].name);

	for (i = 0; i < 10; i++) {
		CString s;
		s.Format("Player %d", i+1);
		players.Add(s);
	}
}

BOOL CControlsDlg::OnInitDialog()
{
	CExtNCW<CExtResizableDialog>::OnInitDialog();

	m_Ctrls.InsertColumn(0, "Control name", 0, 150);
	m_Ctrls.InsertColumn(1, "Key",			0, 150);
	m_Ctrls.InsertColumn(2, "Player",		0, 100);

	RefreshControls();

	CExtBitmap Bitmap;
	Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_ICONADD));
	m_Add.SetIcon(Bitmap.CreateHICON());

	Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_ICONEDIT));
	m_Edit.SetIcon(Bitmap.CreateHICON());

	Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_ICONDELETE));
	m_Remove.SetIcon(Bitmap.CreateHICON());
	
	m_Ctrls.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// Resizing
	dlgMan.Load(this->m_hWnd, "Software\\Construct\\ControlsDlg");
    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDC_CONTROLSLIST, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(IDC_ADD, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_REMOVE, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_RENAME, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDOK, ANCHOR_RIGHT | ANCHOR_BOTTOM);

	return TRUE;
}

CControlsDlg::~CControlsDlg()
{
}

void CControlsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTROLSLIST, m_Ctrls);
	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Control(pDX, IDC_RENAME, m_Edit);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	DDX_Control(pDX, IDOK, m_Close);
}


BEGIN_MESSAGE_MAP(CControlsDlg, CDialog)
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_COMBO_SELECTION, OnComboSelection)
	ON_BN_CLICKED(IDC_ADD, &CControlsDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, &CControlsDlg::OnBnClickedDelete)
	ON_NOTIFY(LVN_KEYDOWN, IDC_CONTROLSLIST, &CControlsDlg::OnLvnKeydownControlslist)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_CONTROLSLIST, &CControlsDlg::OnLvnBeginlabeledit)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_CONTROLSLIST, &CControlsDlg::OnLvnEndlabeledit)
	ON_BN_CLICKED(IDC_RENAME, &CControlsDlg::OnBnClickedRename)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

LRESULT CControlsDlg::OnComboSelection(WPARAM nItem, LPARAM nSubItem)
{
	if (nItem >= 0)
	{
		// Control column
		if (nSubItem == 1) {

			// Find this combo key
			CString itemText = m_Ctrls.GetItemText(nItem, nSubItem);

			for (int i = 0; controlTable[i].vk != -1; i++) {
				if (itemText == controlTable[i].name) {
					application->m_Controls[nItem].comboIndex = i;
					application->m_Controls[nItem].vk = controlTable[i].vk;
					break;
				}
			}
		}
		// Player column
		else if (nSubItem == 2) {

			CString itemText = m_Ctrls.GetItemText(nItem, nSubItem);
			int newplayer = 0;

			for (int i = 0; i < 10; i++) {
				if (itemText == players[i]) {
					newplayer = i;
					break;
				}
			}

			// Check this item name doesnt exist already for this player
			CString controlName = m_Ctrls.GetItemText(nItem, 0);

			BehaviorControl bc;
			bc.name = controlName;
			bc.player = newplayer;

			if (application->m_Controls[nItem].player != newplayer && exists_element(application->m_Controls.begin(), application->m_Controls.end(), bc)) {
				//CString msg;
				//msg.Format("The control '%s' already exists for player %d.", controlName, newplayer+1);
				//MessageBox(msg, "Cannot change player", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}

			// Set the new player
			application->m_Controls[nItem].player = newplayer;

		}

	}

	return 0;
}

void CControlsDlg::RefreshControls()
{
	m_Ctrls.DeleteAllItems();

	vector<BehaviorControl>::iterator i = application->m_Controls.begin();

	for ( ; i != application->m_Controls.end(); i++) {
		int nItem = m_Ctrls.GetItemCount();
		m_Ctrls.InsertItem(nItem, i->name);
		m_Ctrls.SetComboBox(nItem, 1, TRUE, &controls, 20, i->comboIndex);
		m_Ctrls.SetComboBox(nItem, 2, TRUE, &players, 10, i->player);
	}
}

void CControlsDlg::OnBnClickedAdd()
{
	// Add a new control key
	int counter = 2;

	BehaviorControl mc;
	mc.name = "Control 1";
	mc.player = 0;

	while (exists_element(application->m_Controls.begin(), application->m_Controls.end(), mc))
		mc.name.Format("Control %d", counter++);

	// Add this control
	mc.comboIndex = 0;
	//mc.vk = controlTable[0].vk;
	application->m_Controls.push_back(mc);

	int nItem = m_Ctrls.GetItemCount();
	m_Ctrls.InsertItem(nItem, mc.name);
	m_Ctrls.SetComboBox(nItem, 1, TRUE, &controls, 20, 0);
	m_Ctrls.SetComboBox(nItem, 2, TRUE, &players, 10, 0);
}

void CControlsDlg::OnBnClickedDelete()
{
	DeleteCurSel();
}

void CControlsDlg::OnLvnKeydownControlslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pLVKeyDow->wVKey == VK_DELETE)
		DeleteCurSel();
}

void CControlsDlg::DeleteCurSel()
{
	int nItem = m_Ctrls.GetSelectionMark();
	if (nItem >= 0) {

		CString itemText = application->m_Controls[nItem].name;
		itemText.MakeLower();

		if (itemText == "move left" || itemText == "move right" || itemText == "move up" || itemText == "move down"
			|| itemText == "jump" || itemText == "steer left" || itemText == "steer right"
			|| itemText == "accelerate" || itemText == "brake")
		{
			CString msg;
			msg.Format("Are you sure you want to delete the default control '%s'?  Movements using it will revert to "
						"their default keys without it.  You can restore the key by adding a control with the name '%s'"
						" again.", application->m_Controls[nItem].name, application->m_Controls[nItem].name);

			if (MessageBox(msg, "Delete Control", MB_YESNO | MB_ICONQUESTION) == IDNO)
				return;
		}

		m_Ctrls.DeleteItem(nItem);
		application->m_Controls.erase(application->m_Controls.begin() + nItem);
	}
}
void CControlsDlg::OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CControlsDlg::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;

	// Check name exists
	BehaviorControl temp;
	temp.name = pDispInfo->item.pszText;

	CString playerText = m_Ctrls.GetItemText(pDispInfo->item.iItem, 2);

	for (int i = 0; i < 10; i++) {
		if (playerText == players[i]) {
			temp.player = i;
			break;
		}
	}
	
	if (exists_element(application->m_Controls.begin(), application->m_Controls.end(), temp) || temp.name == "")
		*pResult = 0;	// Don't allow rename
	else {
		*pResult = 1;	// Allow rename
		application->m_Controls[pDispInfo->item.iItem].name = temp.name;
	}
}

void CControlsDlg::OnBnClickedRename()
{
	m_Ctrls.SetFocus();
	m_Ctrls.EditLabel(m_Ctrls.GetSelectionMark());
}

void CControlsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dlgAnchor.OnSize();

	Invalidate();
}

void CControlsDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	dlgMan.Save();
}