// NewHLSLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "NewHLSLDlg.h"
#include <shlwapi.h>

// CNewHLSLDlg dialog

//IMPLEMENT_DYNAMIC(CNewHLSLDlg, CDialog)

CNewHLSLDlg::CNewHLSLDlg(CWnd* pParent /*=NULL*/) : CExtNCW<CExtResizableDialog>(CNewHLSLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewMovementDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CNewHLSLDlg::~CNewHLSLDlg()
{
}

void CNewHLSLDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDC_EDITOR, m_Editor);
	DDX_Control(pDX, IDC_DESCRIPTION, m_Description);
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNewHLSLDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CNewHLSLDlg::OnNMDblclkList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CNewHLSLDlg::OnLvnItemchangedList)
END_MESSAGE_MAP()

void CNewHLSLDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	dlgMan.Save();
}

void CNewHLSLDlg::OnOK() 
{
	// Get text
	POSITION Position = m_List.GetFirstSelectedItemPosition();
	int sel           = m_List.GetNextSelectedItem(Position);

	if (sel < 0) {
		CErrorDlg Dlg;
		Dlg.Error("Error adding effect", "Please select an effect to add.");
	}
	else {
		m_Text     = m_List.GetItemText(sel, 0);
		if (m_Text == "") return CDialog::OnCancel();
		m_Filename = m_Text + ".fx";

		m_Version = atof(m_List.GetItemText(sel, 1));
		
		CDialog::OnOK();
	}
}

void CNewHLSLDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dlgAnchor.OnSize();

	Invalidate();
}

void CNewHLSLDlg::OnChangeCategory()
{
	m_List.DeleteAllItems();

	int Tab = m_Tabs.GetCurSel();

	for (int i = 0; i < m_Effects.size(); i++)
	{
		// Get tab specifier
		bool bAdd = false;
		if (Tab == 0) bAdd = true;
		if ((Tab == 1) && (m_Effects[i].Version == 2)) bAdd = true;
		if ((Tab == 2) && (m_Effects[i].Version == 1.4)) bAdd = true;
		if ((Tab == 3) && (m_Effects[i].Version == 1.1)) bAdd = true;
		if ((Tab == 4) && (m_Effects[i].Version == 0)) bAdd = true;

		if (bAdd)
		{
			int Item = m_List.InsertItem(m_List.GetItemCount(), m_Effects[i].Name);
			m_List.SetItemText(Item, 2, m_Effects[i].Author);
			m_List.SetItemText(Item, 3, m_Effects[i].Description);

			CString verStr;
			if (m_Effects[i].Version == 0.0)
				verStr = "None";
			else
				verStr.Format("%.1f", m_Effects[i].Version);

			m_List.SetItemText(Item, 1, verStr);
		}
	}
}

// CNewHLSLDlg message handlers
BOOL CNewHLSLDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_List.InsertColumn(0, "Name", 0, 130, NULL);
	m_List.InsertColumn(1, "Shader", 0, 70, NULL);
	m_List.InsertColumn(2, "Author", 0, 120, NULL);

	char* curDir;
	curDir = new char[MAX_PATH+9];
	GetModuleFileName(NULL, curDir, MAX_PATH+9);
	PathRemoveFileSpec(curDir);
	CString Want = curDir;
	Want += "\\Effects\\";
	strcat(curDir, "\\Effects\\*");
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char DirSpec[MAX_PATH];  // directory specification

	strncpy (DirSpec, curDir, strlen(curDir)+1);
	strncat (DirSpec, "\\*", 3);
	hFind = FindFirstFile(curDir, &FindFileData);

	DWORD id = 0;

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		CErrorDlg error;
		error.Error("Effects directory missing", "The effects directory has been moved.\nConstruct cannot initialise your effects.");     
		EndDialog(0);
	} 

	else
	{
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			CString file = FindFileData.cFileName;
			CString ext = file.Right(3);
			ext.MakeLower();
			if (ext == ".fx")
			{
				int item = m_List.InsertItem(m_List.GetItemCount(), file.Left(file.GetLength() - 3), 0);

				CString Want2 = Want;
				Want2 += file;
				CStdioFile File(Want2, CFile::modeRead);
				CString String;
				int Count = 0;

				CString Author, Version, Description;

				while(File.ReadString(String))
				{
					if (Count == 1)
						Author = String;
					if (Count == 2)
						Version = String;
					if (Count == 3)
					{
						Description = String;
						Description = Description.Right(Description.GetLength() - 2);
						Description.Trim();
						break;
					}
					Count++;
				}

				File.Close();

				Author = Author.Right(Author.GetLength() - 3);
				m_List.SetItemText(item, 2, Author);
				m_List.SetItemText(item, 3, Description);

				CAddEffect Ef;
				Ef.Name = file.Left(file.GetLength() - 3);
				Ef.Author = Author;
				Ef.Description = Description;

				if (Version.Find("0.0") != -1)
					Ef.Version = 0;
				if (Version.Find("1.1") != -1)
					Ef.Version = 1.1;
				if (Version.Find("1.4") != -1)
					Ef.Version = 1.4;
				if (Version.Find("2.0") != -1)
					Ef.Version = 2;

				CString verStr;
				verStr.Format("%.1f", Ef.Version);
				m_List.SetItemText(item, 1, verStr);

				m_Effects.push_back(Ef);
			}
		}
	}	

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
		
	// Resizing
	dlgMan.Load(this->m_hWnd, "Software\\Construct\\NewHLSLDlg2");
    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDC_LIST, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(IDOK, ANCHOR_RIGHT | ANCHOR_TOP);
	dlgAnchor.Add(IDCANCEL, ANCHOR_RIGHT | ANCHOR_TOP);
	dlgAnchor.Add(IDC_CANCEL, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_DESCRIPTION, ANCHOR_LEFT|ANCHOR_BOTTOM|ANCHOR_RIGHT);
	dlgAnchor.Add(IDC_EDITOR, ANCHOR_BOTTOM|ANCHOR_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewHLSLDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	OnOK();
}

void CNewHLSLDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	*pResult = 0;

	CString fxName = m_List.GetItemText(pNMLV->iItem, 0);
	CString fxDesc;

	if (m_Effects.size() > pNMLV->iItem)
		fxDesc = m_Effects[pNMLV->iItem].Description;

	SetDlgItemText(IDC_DESCRIPTION, fxDesc);
}
