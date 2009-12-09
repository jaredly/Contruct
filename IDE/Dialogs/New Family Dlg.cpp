// Dialogs\NewFamilyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "NewFamilyDlg.h"
#include "ManageFamiliesDlg.h"

// CNewFamilyDlg dialog
IMPLEMENT_DYNAMIC(CNewFamilyDlg, CDialog)

CNewFamilyDlg::CNewFamilyDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CNewFamilyDlg::IDD, pParent)
{

}

CNewFamilyDlg::~CNewFamilyDlg()
{
}

void CNewFamilyDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_MANAGE, m_Manage);
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewFamilyDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MANAGE, &CNewFamilyDlg::OnBnClickedManage)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CNewFamilyDlg::OnNMDblclkList)
END_MESSAGE_MAP()

void CNewFamilyDlg::OnOK()
{
	// Get text
	POSITION Position = m_List.GetFirstSelectedItemPosition();
	int sel           = m_List.GetNextSelectedItem(Position);

	if (sel == -1) return;

	m_Name = m_List.GetItemText(sel, 0);

	m_Path += m_Name;
	m_Path += ".bmp";

	small_image.Load(m_Path);

	CDialog::OnOK();
}

void CNewFamilyDlg::OnRefresh()
{
	m_List.DeleteAllItems();
	m_Images.DeleteImageList();
	m_Images.Create(16, 16, ILC_COLOR24,11, 11);
	ListView_SetImageList(m_List.m_hWnd, m_Images.m_hImageList, LVSIL_SMALL);

	char curDir[MAX_PATH+9];
	GetModuleFileName(NULL, curDir, MAX_PATH+9);
	PathRemoveFileSpec(curDir);
	strcat(curDir, "\\Families\\*");

	m_Path = curDir;
	m_Path.Replace("*", "");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char DirSpec[MAX_PATH];  // directory specification

	strncpy (DirSpec, curDir, strlen(curDir)+1);
	strncat (DirSpec, "\\*", 3);
	hFind = FindFirstFile(curDir, &FindFileData);

	DWORD id = 0;

	CStringArray found;

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		CErrorDlg error;
		::ExtMessageBox(m_hWnd, "Families directory missing", "The families directory has been moved.\r\nConstruct cannot initialise your families.", NULL);    
		OnCancel();
	} 

	else
	{
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			CString file = FindFileData.cFileName;
			CString ext = file.Right(4);
			ext.MakeLower();
			if (ext == ".bmp")
			{
				CString Path = curDir;
				Path = Path.Left(Path.GetLength() - 1);
				Path += file;
	
				HBITMAP Bitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), Path, IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
				int icon = ImageList_Add(m_Images.m_hImageList, Bitmap, NULL);
				DeleteObject(Bitmap);
			
				m_List.InsertItem(m_List.GetItemCount(), file.Left(file.GetLength() - 4), icon);
				found.Add(file.Left(file.GetLength() - 4));
			}
		}
	}	

	// now add families we don't have that were in the .cap file
	for (list<Family>::iterator i = application->families.begin(); i != application->families.end(); i++)
	{
		bool exists = false;

		for (int j = 0; j < found.GetCount(); j++)
			if (found.GetAt(j) == i->name)
				exists = true;

		if (!exists)
		{
			HBITMAP bitmap = i->small_image.MakeBitmap();
			int icon = ImageList_Add(m_Images.m_hImageList, bitmap, NULL);
			DeleteObject(bitmap);

			m_List.InsertItem(m_List.GetItemCount(), i->name, icon);
			found.Add(i->name);
		}
	}
}

void CNewFamilyDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	dlgMan.Save();
}

BOOL CNewFamilyDlg::OnInitDialog() 
{
	CExtNCW<CExtResizableDialog>::OnInitDialog();

	m_List.InsertColumn(0, "Name", 0, 150);
	OnRefresh();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	dlgMan.Load(this->m_hWnd, "Software\\Construct\\NewFamilyDlgs");
    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDOK, ANCHOR_TOP | ANCHOR_RIGHT);
	dlgAnchor.Add(IDCANCEL, ANCHOR_TOP | ANCHOR_RIGHT);
	dlgAnchor.Add(IDC_LIST, ANCHOR_BOTTOMRIGHT | ANCHOR_TOPLEFT);

	dlgAnchor.Add(IDC_MANAGE, ANCHOR_BOTTOM | ANCHOR_RIGHT);

	return TRUE;
}

void CNewFamilyDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dlgAnchor.OnSize();	
}

void CNewFamilyDlg::OnBnClickedManage()
{
	CManageFamiliesDlg Dlg;
	Dlg.application = application;
	Dlg.DoModal();

	OnRefresh();
}

void CNewFamilyDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	OnOK();
}
