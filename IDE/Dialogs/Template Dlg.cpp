// TemplateDialog.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "TemplateDialog.h"
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\MainFrm.h"

extern CMainFrame* g_MainFrame;

/////////////////////////////////////////////////////////////////////////////
// TemplateDialog dialog


TemplateDialog::TemplateDialog(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(TemplateDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(TemplateDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void TemplateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TemplateDialog)
	DDX_Control(pDX, IDC_TEMPLATES, m_Templates);
	//DDX_Control(pDX, IDC_PLATFORMS, m_Platforms);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_CREATE, m_Create);
	DDX_Control(pDX, IDC_CANCEL, m_Cancel);
	DDX_Control(pDX, IDC_AUTHOR, m_Author);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TemplateDialog, CDialog)
	//{{AFX_MSG_MAP(TemplateDialog)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_CREATE, OnCreate)
	ON_NOTIFY(NM_DBLCLK, IDC_TEMPLATES, &TemplateDialog::OnDblClkTemplates)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TemplateDialog message handlers

void TemplateDialog::OnDblClkTemplates(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	OnCreate();
}

void TemplateDialog::OnCancel() 
{
	EndDialog(0);
}

BOOL TemplateDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassChildControls();

	m_Cancel.SetWindowText(CLOSE);

	//m_Platforms.AddString("Palm");
	//m_Platforms.AddString("Windows (32-bit)");
	//m_Platforms.SetCheck(0, 1);
	//m_Platforms.AddString("Windows Mobile");

	SetWindowText("Construct : New Template");

	imageList.Create(16, 16, ILC_COLOR24, 3, 3);
	m_Templates.SetImageList(&imageList, LVSIL_SMALL);

	// Load all templates
	CString curDir, findFileDir;
	char getDir[1024];

	GetModuleFileName(NULL, getDir, MAX_PATH);
	PathRemoveFileSpec(getDir);
	
	// Perform misc ops
	curDir = getDir;
	curDir += "\\Templates\\";
	findFileDir = curDir + "*.*";

	// Initialise a search
	WIN32_FIND_DATA findPlugins;
	HANDLE doFind = INVALID_HANDLE_VALUE;

	doFind = FindFirstFile(findFileDir, &findPlugins);

	m_Templates.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 150);
	m_Templates.EnableToolTips(TRUE);

	// If we can't find files, give an error
	if (doFind == INVALID_HANDLE_VALUE) 
	{
		CErrorDlg error;
		error.Error("Templates directory missing", "The templates directory has been moved.\nConstruct cannot initialise your templates.");     

		EndDialog(0);
	} 

	else 
	{
		while (FindNextFile(doFind, &findPlugins) != 0) 
		{
			// Get the current plugin path
			CString tempName = curDir;
			tempName += findPlugins.cFileName;			

			if (tempName.Find(".cst") == -1) continue;

			CString Bitmap = tempName;
			Bitmap = Bitmap.Left(Bitmap.GetLength() - 4);
			Bitmap += ".bmp";

			// Load template information into CFile, and bitmap into HBITMAP
			HBITMAP theBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		                                     Bitmap, IMAGE_BITMAP, 0, 0,
                                             LR_LOADFROMFILE | LR_CREATEDIBSECTION);

			CString ToAdd = "";
			ToAdd += findPlugins.cFileName;
			ToAdd = ToAdd.Left(ToAdd.GetLength() - 4);

			int currentIcon = ImageList_Add(imageList, theBitmap, theBitmap);

			int item = m_Templates.InsertItem(currentIcon, ToAdd, currentIcon);
		}

		// Close find
		FindClose(doFind);
	}

	m_Name.SetWindowTextA("My application");

	return TRUE;
}

void TemplateDialog::OnCreate() 
{
	UpdateData();

	// Create subfolder
	CPath Get;
	Get.SetToCurrentDirectory();

	CString Name;
	m_Name.GetWindowText(Name);

	if (Name == "")
	{
		CErrorDlg dlg;
		dlg.Error("Invalid name", "Please enter a name for your project.");
		return;
	}

	CString Path;
	Path.Format("%s\\Projects\\", Get.GetFullPath());
	CreateDirectory(Path, NULL);
	Path.Format("%s\\Projects\\%s\\", Get.GetFullPath(), Name);
	CreateDirectory(Path, NULL);

	// Copy over the template
	POSITION thePos = m_Templates.GetFirstSelectedItemPosition();
	int theItem     = m_Templates.GetNextSelectedItem(thePos);

	CString Title   = m_Templates.GetItemText(theItem, 0);

	CString ToCopy;
	ToCopy.Format("%sTemplates\\%s.cst", Get.GetFullPath(), Title);
	CString CopyTo;
	CopyTo.Format("%sProjects\\%s\\", Get.GetFullPath(), Name);
	CopyTo += Name;
	CopyTo += ".cap";

	CopyFile(ToCopy, CopyTo, FALSE);

	CApplication* pApp = g_MainFrame->DoLoad(CopyTo);

	if (pApp) {
		m_Name.GetWindowText(pApp->file_information.name);
		m_Author.GetWindowTextA(pApp->file_information.creator);
	}

	g_MainFrame->project_bar.tree.Select(g_MainFrame->project_bar.tree.GetChildItem(pApp->tree_items.application), TVGN_CARET);
	//g_MainFrame->project_bar.OnLButtonDblClk(NULL, NULL);	

	EndDialog(0);
}
