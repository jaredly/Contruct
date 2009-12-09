// Dialogs\UpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "UpdateDlg.h"
#include "..\MainFrm.h"

extern class CMainFrame* g_MainFrame;

void DownloadFile(void* Data)
{
	CUpdateDlg* pDlg = (CUpdateDlg*)Data;

	pDlg->hr = URLDownloadToFile(NULL, pDlg->m_RemoteURL, pDlg->m_LocalFile, NULL, NULL);   

	return;
}

// CUpdateDlg dialog
IMPLEMENT_DYNAMIC(CUpdateDlg, CDialog)

CUpdateDlg::CUpdateDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CUpdateDlg::IDD, pParent)
{

}

CUpdateDlg::~CUpdateDlg()
{
}

void CUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_FILES, m_Files);
	DDX_Control(pDX, IDC_DOWNLOAD, m_Download);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
}

BEGIN_MESSAGE_MAP(CUpdateDlg, CDialog)
	ON_BN_CLICKED(IDC_DOWNLOAD, &CUpdateDlg::OnBnClickedDownload)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CUpdateDlg message handlers
BOOL CUpdateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Files.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_Files.InsertColumn(0, "Name", 0, 120);
	m_Files.InsertColumn(1, "Description", 0, 280);

	// Resizing
	dlgMan.Load(this->m_hWnd, "Software\\Construct\\UpdateDlg");
    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDC_FILES, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(IDC_PROGRESS, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_DOWNLOAD, ANCHOR_RIGHT | ANCHOR_TOP);
	dlgAnchor.Add(IDCANCEL, ANCHOR_TOP | ANCHOR_RIGHT);

	// Set some update params
	m_UpdateObj.SetLocalDirectory("", true);
	CString updateFileURL;
	m_UpdateObj.SetUpdateFileURL("http://www.scirra.com/update.txt");
	m_UpdateObj.SetRemoteURL("http://construct.cvs.sourceforge.net/*checkout*/construct/IDE/");

	CString ToAdd;

	// Do the update
	if (m_UpdateObj.DoUpdateCheck())
	{
		int Index = 0;

		// Missing files
		for (int i = 0; i < m_UpdateObj.GetNumberMissing(); i++)
		{                 
			ToAdd = m_UpdateObj.GetMissingAt(i);

			CString URL = "http://construct.cvs.sourceforge.net/*checkout*/construct/IDE/";
			URL += ToAdd;
			m_Local.Add(ToAdd);
			URL.Replace("\\", "/");
			m_URLs.Add(URL);
	
			// Snip the path away if there is one
			if (ToAdd.Find("\\") != -1)
				ToAdd = ToAdd.Right(ToAdd.GetLength() - ToAdd.Find("\\") - 1);

			m_Files.InsertItem(Index, ToAdd);
			m_Files.SetItemText(Index, 1, m_UpdateObj.missingInfo.GetAt(i));
			m_Files.SetCheck(Index, 1);

			Index++;
		}

		// Different files
		for (int i = 0; i < m_UpdateObj.GetNumberDifferent(); i++)
		{
			ToAdd = m_UpdateObj.GetDifferentAt(i);

			CString URL = "http://construct.cvs.sourceforge.net/*checkout*/construct/IDE/";
			URL += ToAdd;
			m_Local.Add(ToAdd);
			URL.Replace("\\", "/");
			m_URLs.Add(URL);

			// Snip the path away if there is one
			if (ToAdd.Find("\\") != -1)
				ToAdd = ToAdd.Right(ToAdd.GetLength() - ToAdd.Find("\\") - 1);

			m_Files.InsertItem(Index, ToAdd);
			m_Files.SetItemText(Index, 1, m_UpdateObj.differentInfo.GetAt(i));
			m_Files.SetCheck(Index, 1);

			Index++;
		}
	}	

	if (m_Files.GetItemCount() == 0)
	{
		ToAdd.Format("No updates found");
		m_Files.InsertItem(0, ToAdd);
		m_Files.EnableWindow(0);
		OnOK();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CUpdateDlg::OnBnClickedDownload()
{
	// Get the items we're going to download
	CStringArray ToDownload;
	CStringArray FileName;
	hr = 0;

	CPath Path;
	Path.SetToCurrentDirectory();

	for (int i = 0; i < m_Files.GetItemCount(); i++)
		if (m_Files.GetCheck(i))
		{
			ToDownload.Add(m_URLs.GetAt(i));
			FileName.Add(m_Local.GetAt(i));
		}

	// Set progress bar
	m_Progress.SetRange(0, ToDownload.GetSize());
	m_Progress.SetStep(1);

	bool bNeedsRestart = false;
	bool bNeedsSelfDestruct = false;

	CString OldConstruct;
	CString NewConstruct;

	bool allOK = true;

	m_Download.EnableWindow(FALSE);

	// Download
	for (int i = 0; i < ToDownload.GetSize(); i++)
	{
		// Be aware that some items may be in memory and thus 
		// need to wait, or be swapped with a .bat file
		CString Item = ToDownload.GetAt(i);

		// Build a local path
		CString Local = Path.GetFullPath();
		Local += FileName.GetAt(i);

		// Check for special cases
		if (Local.Find("Construct.exe") != -1)
		{
			// We're going to have to self destruct this file and copy in the new
			bNeedsSelfDestruct = true;
			bNeedsRestart = true;

			OldConstruct = Local;
	
			Local = Local.Left(Local.GetLength() - 13);
			Local += "Data\\Construct.exe";

			NewConstruct = Local;
		}

		if (Local.Find("Plugins\\") != -1 && Local.Find("Runtime") == -1)
		{
			// Edit time plugin needs to be unloaded, get file name
			CPath Path;
			Path.SetPath(Local);
			CString Name = Path.GetFileName();

			// Find it and unload
			OINFO* pInfo = GetOINFOByFile(Name);
			if (pInfo)
			{
				FreeLibrary(pInfo->hInstLib);
			}

			bNeedsRestart = true;
		}

		// Download file
		m_RemoteURL = Item;
		m_LocalFile = Local;

		HANDLE hHandle = (HANDLE)_beginthread(DownloadFile, 0, (void*)this);
		AtlWaitWithMessageLoop(hHandle);

		if (FAILED(hr))
			allOK = false;

		m_Progress.StepIt();
	}

	// All went smoothly
	if (allOK) {
		if (bNeedsRestart)
			MessageBox("One or more updates applied require Construct to close.", "Restart needed");

		else
			MessageBox("All updates successfully applied.", "Updates completed");
	}

	// Some downloads failed
	else 
	{
		CErrorDlg Dlg;
		Dlg.Error("Updating error", "One or more errors occured while updating. Please try again.");
	}

	// If we need a self destruct create the .bat
	if (bNeedsSelfDestruct)
	{
		Path.SetToAppDataDirectory("Scirra");
		CString TempPath;
		TempPath.Format("%sExecute.bat", Path.GetFullPath());
		HANDLE hFile = CreateFile(TempPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		static char Temp[] = 
		":Repeat\r\n"
		"del \"%s\"\r\n"
		"if exist \"%s\" goto Repeat\r\n"
		"move \"%s\" \"%s\"";

		// Make .bat
		CString Execute = Temp;

		// Variables we want are (in order)
		// Construct.exe with path
		// Construct.exe with path
		// New Construct.exe with path, Construct.exe with path
		Execute.Format(Temp, OldConstruct, OldConstruct, NewConstruct, OldConstruct);

		DWORD Len;
		WriteFile(hFile, (const char*)Execute, Execute.GetLength(), &Len, NULL);
		CloseHandle(hFile);

		ShellExecute(NULL, "open", TempPath, NULL, NULL, SW_HIDE);
	}

	m_Download.EnableWindow(TRUE);

	if (bNeedsRestart)
		g_MainFrame->OnClose();
	else
		OnOK();
}

void CUpdateDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dlgAnchor.OnSize();

	Invalidate();
}

void CUpdateDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	dlgMan.Save();
}