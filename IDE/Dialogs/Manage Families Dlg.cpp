// dialogs\ManageFamiliesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "ManageFamiliesDlg.h"
#include "ImageEditorDlg.h"
// CManageFamiliesDlg dialog
//IMPLEMENT_DYNAMIC(CManageFamiliesDlg, CDialog)

CManageFamiliesDlg::CManageFamiliesDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CManageFamiliesDlg::IDD, pParent)
{

}

CManageFamiliesDlg::~CManageFamiliesDlg()
{
}

void CManageFamiliesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	DDX_Control(pDX, IDOK, m_Close);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDC_NAME, m_Name);
}

BEGIN_MESSAGE_MAP(CManageFamiliesDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD, &CManageFamiliesDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, &CManageFamiliesDlg::OnBnClickedRemove)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST, &CManageFamiliesDlg::OnLvnKeydown)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CManageFamiliesDlg message handlers
BOOL CManageFamiliesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CExtBitmap Bitmap;
	Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_ICONADD));
	m_Add.SetIcon(Bitmap.CreateHICON());

	Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_ICONDELETE));
	m_Remove.SetIcon(Bitmap.CreateHICON());

	// Default column needed
	m_List.InsertColumn(0, "Name", LVCFMT_LEFT, 200);

	RefreshFamilies();

	// Resizing
	dlgMan.Load(this->m_hWnd, "Software\\Construct\\FamiliesMgr2");
    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDC_LIST, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(IDC_NAME, ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_TEXT, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_INFOICON, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_ADD, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_REMOVE, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDOK, ANCHOR_RIGHT | ANCHOR_BOTTOM);

	return true;
}

void CManageFamiliesDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	dlgMan.Save();
}

void CManageFamiliesDlg::RefreshFamilies()
{
	m_List.DeleteAllItems();

	CPath Path;
	Path.SetToCurrentDirectory();
	
	CString FamiliesDirectory = Path.GetFullPath();
	FamiliesDirectory += "Families\\*";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile(FamiliesDirectory, &FindFileData);

	m_FamilyImages.DeleteImageList();
	m_FamilyImages.Create(16, 16, ILC_COLOR24, 3, 3);
	ListView_SetImageList(m_List.m_hWnd, m_FamilyImages.m_hImageList, LVSIL_SMALL);

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		CErrorDlg error;
		error.Error("Families directory missing", "The families directory has been moved.\nConstruct cannot initialise your families.");     
		EndDialog(0);
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
				CString FamilyPath = FamiliesDirectory;
				FamilyPath = FamilyPath.Left(FamilyPath.GetLength() - 1);
				FamilyPath += file;
	
				HBITMAP Bitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), FamilyPath, IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
				int icon = ImageList_Add(m_FamilyImages.m_hImageList, Bitmap, NULL);
				DeleteObject(Bitmap);

				m_List.InsertItem(m_List.GetItemCount(), file.Left(file.GetLength() - 4), icon);
			}
		}
	}	

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

bool CManageFamiliesDlg::FamilyNameExists(CString name)
{
	name.MakeLower();

	for (int i = 0; i < m_List.GetItemCount(); i++) {
		CString curName = m_List.GetItemText(i, 0);
		curName.MakeLower();
		if (curName == name)
			return true;
	}

	return false;
}

void CManageFamiliesDlg::OnBnClickedRemove()
{
	ConfirmDeleteSelectedItems();
}

void CManageFamiliesDlg::ConfirmDeleteSelectedItems()
{
	int selCount = m_List.GetSelectedCount();

	if (selCount == 1) {
		CString name = m_List.GetItemText(m_List.GetSelectionMark(), 0);

		if (MessageBox(CString("Are you sure you want to delete the family '") + name + "'?", "Delete Family", MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			CPath Path;
			Path.SetToCurrentDirectory();
			
			CString Family = Path.GetFullPath();
			Family += "Families\\";
			Family += name;
			Family += ".bmp";
			DeleteFile(Family);

			RefreshFamilies(); 
		}
	}
	else if (selCount > 1) {
		CString msg;
		msg.Format("Are you sure you want to delete these %d families?", m_List.GetSelectedCount());

		if (MessageBox(msg, "Delete Families", MB_ICONQUESTION | MB_YESNO) == IDNO)
			return;

		// Iterate selected items and delete
		POSITION selpos = m_List.GetFirstSelectedItemPosition();

		while (selpos) 
		{
			int nItem = m_List.GetNextSelectedItem(selpos);
			CString name = m_List.GetItemText(nItem, 0);

			CPath Path;
			Path.SetToCurrentDirectory();
			
			CString Family = Path.GetFullPath();
			Family += "Families\\";
			Family += name;
			Family += ".bmp";
			DeleteFile(Family);
		}

		RefreshFamilies(); 
	}
}

void CManageFamiliesDlg::OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pLVKeyDow->wVKey == VK_DELETE)
		ConfirmDeleteSelectedItems();
}

void CManageFamiliesDlg::OnBnClickedAdd()
{
	CString Name;
	m_Name.GetWindowText(Name);

	if (Name == "") {
		MessageBox("Please enter the name of the family to add.", "Add new family", MB_ICONEXCLAMATION);
		return;
	}

	if (FamilyNameExists(Name)) {
		MessageBox(CString("The family name '") + Name + "' is already being used.  Please choose a unique name.", "Add new attribute", MB_ICONEXCLAMATION);
		return;
	}

	// Open image editor..
	CImageResource Image;
	Image.bitmap.Create(16, 16, 32);
	Image.bitmap.Clear(0);
	Image.bitmap.AlphaCreate();
	Image.bitmap.AlphaInvert();

	try
	{
		CImageEditorDlg Dlg;

		Dlg.m_Animation = false;
		Dlg.application = application;
		Dlg.m_sourceImages.push_back(&Image);
		Dlg.m_Icon = true;
		
		Dlg.CopyNeededDataFromImages();

		if(Dlg.EditImage(&Image.bitmap, true))
		{
			Dlg.CopyNeededDataToImages();

			// Save to disk
			CxImage Save;
			Save.CreateFromHBITMAP(Image.GetHBitmap(true, false));
			
			CPath Path;
			Path.SetToCurrentDirectory();
			
			CString Family = Path.GetFullPath();
			Family += "Families\\";
			Family += Name;
			Family += ".bmp";
			Save.Save(Family, CXIMAGE_FORMAT_BMP);
		}
	}

	catch (CD3DError e) 
	{
		CErrorDlg err;
		err.Error("Error editing family icon", e.GetString());
	}
	
	RefreshFamilies();

	m_Name.SetWindowText("");
	m_Name.SetFocus();
}

void CManageFamiliesDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dlgAnchor.OnSize();

	Invalidate();
}