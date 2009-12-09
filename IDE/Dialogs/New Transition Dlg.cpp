// NewTransitionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "NewTransitionDlg.h"

// CNewTransitionDlg dialog
IMPLEMENT_DYNAMIC(CNewTransitionDlg, CDialog)

CNewTransitionDlg::CNewTransitionDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CNewTransitionDlg::IDD, pParent)
{
}

CNewTransitionDlg::~CNewTransitionDlg()
{
}

void CNewTransitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Transitions);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
}

BEGIN_MESSAGE_MAP(CNewTransitionDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CNewTransitionDlg::OnNMDblclkList1)
END_MESSAGE_MAP()

void CNewTransitionDlg::OnOK()
{
	// Get text
	POSITION Position  = m_Transitions.GetFirstSelectedItemPosition();
	int sel            = m_Transitions.GetNextSelectedItem(Position);

	if (sel < 0) {
		MessageBox("Please pick a transition to add.", "Add transition", MB_OK | MB_ICONEXCLAMATION);
	}
	else {
		m_Transition.m_Name = m_Transitions.GetItemText(sel, 0);
		if (m_Transition.m_Name == "") return CDialog::OnCancel();

		m_Transition.m_FileName = m_Transition.m_Name + ".fx";
		
		CDialog::OnOK();
	}
}

void CNewTransitionDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	dlgMan.Save();
}

// CNewTransitionDlg message handlers
BOOL CNewTransitionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Transitions.InsertColumn(0, "Name", 0, 110, NULL);
	m_Transitions.InsertColumn(1, "Author", 0, 100, NULL);
	m_Transitions.SetExtendedStyle(LVS_EX_FULLROWSELECT);
		
	//Why god why... why code deliberate memory leaks...
	/*
	char* curDir;
	curDir = new char[MAX_PATH+9];
	*/
	char curDir[MAX_PATH + 9];
	GetModuleFileName(NULL, curDir, MAX_PATH+9);
	PathRemoveFileSpec(curDir);
	CString Want = curDir;
	Want += "\\Transitions\\";
	strcat(curDir, "\\Transitions\\*");
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char DirSpec[MAX_PATH];  // directory specification

	strncpy (DirSpec, curDir, strlen(curDir)+1);
	strncat (DirSpec, "\\*", 3);
	hFind = FindFirstFile(curDir, &FindFileData);

	//::MessageBox(NULL, curDir, NULL, NULL);

	DWORD id = 0;

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		CErrorDlg error;
		error.Error("Transitions directory missing", "The transitions directory has been moved.\nConstruct cannot initialise your transitions.");     
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
				int item = m_Transitions.InsertItem(m_Transitions.GetItemCount(), file.Left(file.GetLength() - 3), 0);

				CString Want2 = Want;
				Want2 += file;
				CStdioFile File(Want2, CFile::modeRead);
				CString String;
				int Count = 0;

				CString Author, Version;

				while(File.ReadString(String))
				{
					if (Count == 1)
						Author = String;
					if (Count == 2)
					{
						Version = String;
						break;
					}
					Count++;
				}

				File.Close();

				Author = Author.Right(Author.GetLength() - 3);
				m_Transitions.SetItemText(item, 1, Author);
			}
		}
	}	

	// Resizing
	dlgMan.Load(this->m_hWnd, "Software\\Construct\\NewTransitionDlg");
    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDC_LIST1, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(IDOK, ANCHOR_RIGHT | ANCHOR_TOP);
	dlgAnchor.Add(IDCANCEL, ANCHOR_RIGHT | ANCHOR_TOP);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewTransitionDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dlgAnchor.OnSize();

	Invalidate();
}

void CNewTransitionDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	OnOK();
}
