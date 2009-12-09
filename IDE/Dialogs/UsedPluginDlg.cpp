// UsedPluginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Used Plugins Dlg.h"
#include "Object Info Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// UsedPluginDialog dialog

UsedPluginDialog::UsedPluginDialog(CApplication* application_) : 
	CExtNCW<CExtResizableDialog>(UsedPluginDialog::IDD, NULL),
	application(application_)
{
}

void UsedPluginDialog::DoDataExchange(CDataExchange* pDX)
{
	CExtNCW<CExtResizableDialog>::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFORMATION, information);
	DDX_Control(pDX, IDCLOSE, close);
	DDX_Control(pDX, IDC_OBJECTS, objects);
}

BEGIN_MESSAGE_MAP(UsedPluginDialog, CExtResizableDialog)
	ON_BN_CLICKED(IDCLOSE, OnClose)
	ON_BN_CLICKED(IDC_INFORMATION, OnInformation)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// UsedPluginDialog message handlers

BOOL UsedPluginDialog::OnInitDialog() 
{
	CExtNCW<CExtResizableDialog>::OnInitDialog();
	
	objects.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	objects.InsertColumn(0, "Name"   , LVCFMT_LEFT, 90);
	objects.InsertColumn(1, "Creator", LVCFMT_LEFT, 110);
	objects.InsertColumn(2, "License", LVCFMT_LEFT, 100);

	dialog_manager.Load(this->m_hWnd, "Software\\Construct\\UsedPlugins");
    dialog_anchor.Init(this->m_hWnd);

	dialog_anchor.Add(IDCLOSE, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dialog_anchor.Add(IDC_APPNAME, ANCHOR_TOP | ANCHOR_LEFT | ANCHOR_RIGHT);
	dialog_anchor.Add(IDC_BOX1, ANCHOR_BOTTOMRIGHT | ANCHOR_TOPLEFT);
	dialog_anchor.Add(IDC_OBJECTS, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dialog_anchor.Add(IDC_INFORMATION, ANCHOR_RIGHT | ANCHOR_TOP);
	dialog_anchor.Add(IDCLOSE, ANCHOR_RIGHT | ANCHOR_TOP);

	SetDlgItemText(IDC_APPNAME, application->file_information.name);

	CString Title;
	Title.Format("Construct : Used plugins");
	SetWindowText(Title);

	// Loop and find all plugins
	CStringArray IDList;

	for (int j = 0; j < application->object_types.GetCount(); j++)
	{
		// Check if we have the specified ID
		CObjType* oT;

		POSITION pos = application->object_types.GetStartPosition();
		long mapKey;

		// Loop through and add the objects
		while (pos != NULL) 
		{
			application->object_types.GetNextAssoc(pos, mapKey, oT);

			// Do we have this ID
			bool Add = true;

			CString ID;
			ID.Format("%d", oT->DLLIndex);

			for (int k = 0; k < IDList.GetSize(); k++)
			{
				if (IDList.GetAt(k) == ID)
					Add = false;
			}

			if (Add) IDList.Add(ID);
		}
	}

	for (int i = 0; i < IDList.GetSize(); i++)
	{
		OINFO* oInfo = GetOINFO(atoi(IDList.GetAt(i)));

		char strText[1024];
		LoadString(oInfo->hInstLib, OBJ_AUTHOR, strText, 1024);

		int item = objects.InsertItem(i, oInfo->extName, 0);
		objects.SetItemText(i, 1, strText);

		LoadString(oInfo->hInstLib, 10, strText, 1024);

		objects.SetItemText(i, 2, strText);
	}

	// Setup buttons
	close.SetWindowText(CLOSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void UsedPluginDialog::OnClose() 
{
	CDialog::OnCancel();
}

void UsedPluginDialog::OnInformation() 
{
	POSITION position = objects.GetFirstSelectedItemPosition();
	int item = objects.GetNextSelectedItem(position);

	if (item == -1) return;

	CString item_text = objects.GetItemText(item, 0);

	CObjectInfoDlg objInfo;
	objInfo.objname = item_text;
	objInfo.DoModal();	
}

void UsedPluginDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dialog_anchor.OnSize();		

	Invalidate();
}

void UsedPluginDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	dialog_manager.Save();	
}
