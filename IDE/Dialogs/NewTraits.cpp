// Dialogs/NewTraits.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "NewTraits.h"


// CNewTraitDlg dialog

IMPLEMENT_DYNAMIC(CNewTraitDlg,CExtNCW<CExtResizableDialog>)

CNewTraitDlg::CNewTraitDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CNewTraitDlg::IDD, pParent)
{

}

CNewTraitDlg::~CNewTraitDlg()
{
}

void CNewTraitDlg::DoDataExchange(CDataExchange* pDX)
{
	CExtNCW<CExtResizableDialog>::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_OK);

}


void CNewTraitDlg::OnOK() 
{
	ID = m_List.GetCurSel();
	
	m_List.GetText(ID, Text);


	
	CExtNCW<CExtResizableDialog>::OnOK();
}

BOOL CNewTraitDlg::OnInitDialog() 
{
	CExtNCW<CExtResizableDialog>::OnInitDialog();

    m_HeaderCtrl.SetTitleText("New movement");
    m_HeaderCtrl.SetDescText("Select a movement");
    m_HeaderCtrl.SetIconHandle(AfxGetApp()->LoadIcon(IDR_MAINFRAME));

    // Create the header ctrl
    m_HeaderCtrl.Init(this);

    // Move all dialog ctrls down to accommodate header area
    m_HeaderCtrl.MoveCtrls(this);


	// time to fill our traits list

	char CurDir[MAX_PATH+9];
	GetModuleFileName(NULL, CurDir, MAX_PATH);
	PathRemoveFileSpec(CurDir);
	strcat(CurDir, "\\Traits\\*");
	
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char DirSpec[MAX_PATH];  // directory specification

	strncpy (DirSpec, CurDir, strlen(CurDir)+1);
	strncat (DirSpec, "\\*", 3);
	hFind = FindFirstFile(CurDir, &FindFileData);

	

				
	int index = 1;
	while (FindNextFile(hFind, &FindFileData) != 0) 
	{
		CString file = FindFileData.cFileName;
		CString ext = file.Right(4);
		ext.MakeLower();
		
		if (ext == ".txt")
		{
			CString name = file.Left(file.GetLength() - 4);
			m_List.AddString(name);			
		}
	}

/*
	// Add movements to dialog
	POSITION pos = g_dlls->GetStartPosition();
	DWORD    nKey;
	OINFO*   oInfo;

	m_Images.Create(16, 16, ILC_COLOR24, 3, 3);
	ListView_SetImageList(m_List.m_hWnd, m_Images.m_hImageList, LVSIL_SMALL);

	m_List.InsertColumn(0, "Name", 0, 110, NULL);
	m_List.InsertColumn(0, "License", 0, 100, NULL);

	while (pos != NULL)
	{ 
		// locate the object DLL to use
		g_dlls->GetNextAssoc(pos, nKey, oInfo);

		if (oInfo->ideFlags & OF_MOVEMENTEXTENSION) 
		{
			int icon = ImageList_Add(m_Images.m_hImageList, oInfo->smallIcon, NULL);

			int item = m_List.InsertItem(0, oInfo->extName, icon);
			m_List.SetItemData(item, oInfo->oID);

			char *strText;
			strText = new char[1024];
			LoadString(oInfo->hInstLib, 10, strText, 1024);

			m_List.SetItemText(item, 1, strText);
		}
	}

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
*/		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CNewTraitDlg, CExtNCW<CExtResizableDialog>)
END_MESSAGE_MAP()


// CNewTraitDlg message handlers
