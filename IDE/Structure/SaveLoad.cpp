// SaveLoad.cpp

#include "StdAfx.h"
#include "..\MainFrm.h"
extern CLayerBar* g_pLayerBar;

#include "GUIPersistence.h"

void CMainFrame::OnFileNewApplication() 
{
	CApplication* pApplication = new CApplication();
	pApplication->Instantiate();
	m_apps.AddTail(pApplication);

	newAppNumber++;
	CString appName = APPLICATION;

	CString Name;
	Name.Format(" %d", newAppNumber);
	appName += Name;

	pApplication->ChangeName(Name);
	pApplication->file_information.creator = "Creator";
	pApplication->runtime = CApplication::rt_application;

	project_bar.AddApplication(pApplication);

	// Add a layout
	pApplication->AddLayout();

	project_bar.tree.Select(pApplication->tree_items.application, TVGN_CARET);
	project_bar.tree.Expand(pApplication->tree_items.application, TVE_EXPAND);

	project_bar.tree.Select(project_bar.tree.GetChildItem(pApplication->tree_items.layouts), TVGN_CARET);
	project_bar.OnDoubleClick(NULL, NULL);	

	pApplication->ChangeModifiedStatus(false);
	m_pPApp = pApplication;
}

void CMainFrame::OnFileNew() 
{
	CApplication* pApplication = new CApplication();
	pApplication->Instantiate();
	m_apps.AddTail(pApplication);

	newAppNumber++;
	CString appName = APPLICATION;

	CString str;
	str.Format(" %d", newAppNumber);
	appName += str;

	pApplication->ChangeName(appName);
	pApplication->file_information.creator = "Creator";

	project_bar.AddApplication(pApplication);

	// Add a layout
	pApplication->AddLayout();

	project_bar.tree.Select(pApplication->tree_items.application, TVGN_CARET);
	project_bar.tree.Expand(pApplication->tree_items.application, TVE_EXPAND);

	project_bar.tree.Select(project_bar.tree.GetChildItem(pApplication->tree_items.layouts), TVGN_CARET);
	project_bar.OnDoubleClick(NULL, NULL);		

	// By default not changed
	pApplication->ChangeModifiedStatus(false);

	// Default preview application is this one
	m_pPApp = pApplication;
}


void CMainFrame::OnFileOpen() 
{
	char openFilters[]=
	"Construct Application Files (*.cap)|*.cap|All Files (*.*)|*.*||";

	// Load selector
	CFileDialog pdlg (TRUE, "Construct Application Files (*.cap)", "*.cap", OFN_FILEMUSTEXIST, openFilters);
	if (pdlg.DoModal() == IDOK)
	{
		DoLoad(pdlg.GetPathName());
	}
}

CApplication* CMainFrame::DoLoad(LPCSTR File)
{
	// Verify this file exists
	if (!FileExists(File)) return NULL;

	// Open file selected
	CFile file;
	file.Open(File, CFile::modeRead, NULL);

	// Check it's a Construct file
	char loadBuffer[CNA_FORMAT_LENGTH + 1];
	file.Read(loadBuffer, CNA_FORMAT_LENGTH);
	CString check = loadBuffer;

	if (check != CNA_FORMAT)
	{
		// Is it a Construct file at all?
		if (check.Find("Construct") == -1)
		{
			// Error, odd file, probably not a Construct file at all
			::MessageBox(NULL, "This file is incompatible with Construct.", "Incompatible file", MB_ICONEXCLAMATION);
			return NULL;
		}

		else
		{
			// Error, old version
			::MessageBox(NULL, "This file has been created with a previous version of Construct and is incompatible.", "Old Construct file", MB_ICONEXCLAMATION);
			return NULL;
		}
	}

	CArchive ar(&file, CArchive::load, NULL, NULL);

	// Create new application to load to
	CApplication *pApplication = new CApplication();

	try {
		if (!pApplication->Serialize(ar))
			return NULL;
	}
	catch (runtime_error e) {
		::MessageBox(NULL, CString("Failed to load application:  ") + e.what(), "Error loading application", MB_ICONEXCLAMATION);
		return NULL;
	}

	m_apps.AddTail(pApplication);

	pApplication->file_information.file_path = File;
	pApplication->m_appID = ++newAppNumber;

	ar.Close();
	file.Close();

	// Update bars
	project_bar.AddApplication(pApplication);
	project_bar.UpdateApplication(pApplication);

	// persist GUI
	CPath out;
	out.SetPath(pApplication->file_information.file_path);
	
	std::string directory = (LPCSTR)out.GetFullPath();
	directory += (LPCSTR)out.GetFileTitle();
	directory += ".persist";
	 
	GUIPersistence persist(this, pApplication);
	
	if (!persist.LoadPersistence(directory))
	{
		// no persistence, open layout 1
		project_bar.tree.Select(project_bar.tree.GetChildItem(pApplication->tree_items.layouts), TVGN_CARET);
		project_bar.OnDoubleClick(NULL, NULL);		
	}

	// Set current application
	m_pPApp = pApplication;

	// Add to MRU
	AddToMRU(pApplication);

	return pApplication;
}

bool CMainFrame::OnFileSaveAs(CApplication* pApplication) 
{
	char openFilters[]=
    "Construct Application Files (*.cap)|*.cap|All Files (*.*)|*.*||";

	// Load selector
	CFileDialog dlg(FALSE, "Construct Application Files (*.cap)", "*.cap", OFN_OVERWRITEPROMPT, openFilters);
	if (dlg.DoModal() == IDOK)
	{
		// Save file selected
		CString Name = dlg.GetPathName();
		Name += ".tmp";
		pApplication->file_information.file_path = dlg.GetPathName();

		return DoSave(Name, pApplication, false);
	}

	else
		return false;
}

bool CMainFrame::DoSave(LPCSTR Name, CApplication* pApplication, bool isAutosave, bool omit_error)
{
	// Check version of old file
	if (pApplication->file_information.created_with < CONSTRUCT_VERSION_ID && !omit_error)
	{
		if (MessageBox("This application was saved in an older version of Construct.  If you overwrite the file, you will no longer be able to open it in the older version."
			"  Are you sure you want to overwrite the file?  If you choose 'No', you will be prompted to save as a different filename.",
			"Save", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		{
			pApplication->file_information.created_with = CONSTRUCT_VERSION_ID;
			OnFileSaveAs(pApplication);
			return false;
		}

		pApplication->file_information.created_with = CONSTRUCT_VERSION_ID;
	}

	// check read only
	DWORD attributes = GetFileAttributes(pApplication->file_information.file_path);

	if ((attributes & FILE_ATTRIBUTE_READONLY) && FileExists(pApplication->file_information.file_path))
	{
		pApplication->file_information.file_path = "";
		CExtMsgBox::DoMsgBox(m_hWnd, "This file cannot be overwritten, as it is set to read only.", "Cannot overwrite file", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	CFile File;
	File.Open(Name, CFile::modeCreate | CFile::modeReadWrite, NULL);
	
	// Write identifiable version string
	File.Write(CNA_FORMAT, CNA_FORMAT_LENGTH);

	CArchive ar(&File, CArchive::store, NULL, NULL);

	try
	{
		pApplication->Serialize(ar);
		ar.Close();

		File.SeekToBegin();

		// Verify
		try {
			char strbuf[14];

			CArchive ar_verify(&File, CArchive::load, NULL, NULL);
			ar_verify.Read(strbuf, CNA_FORMAT_LENGTH);

			CApplication verifyApp;
			verifyApp.verifying = true;
			verifyApp.Serialize(ar_verify);

			ar_verify.Close();
		}
		catch (runtime_error e) {
			CString msg;
			msg.Format("Failed to save application (verify failure).  Your changes have NOT been saved!  Please report this bug to Scirra along with any actions you may have "
				"performed in Construct prior to this error appearing.  Reported error:\n\n%s", e.what());

			MessageBox(msg, "Save", MB_OK | MB_ICONHAND);

			return false;
		}
		catch (...) {
			MessageBox("Failed to save application (verify failure).  Your changes have NOT been saved!  Please report this bug to Scirra along with any actions you may have "
				"performed in Construct prior to this error appearing.  Reported error:\n\nUnhandled exception", "Save", MB_OK | MB_ICONHAND);

			return false;
		}


		File.Close();
	}
	catch (runtime_error e) {

		::MessageBox(NULL, CString("Failed to save application (") + e.what() + ") - please report this bug to Scirra!", "Error saving application", MB_ICONEXCLAMATION);

		File.Close();
		ar.Close();

		return false;
	}

	catch (...)
	{
		::MessageBox(NULL, "There was a serious error saving. Please report this to Scirra with your .cap file (.tmp), and restart Construct.", "Error saving application", MB_ICONEXCLAMATION);

		File.Close();
		ar.Close();

		return false;
	}

	// Run the backups
	bool backingUpSaves = (m_INI.GetInt("AutoBackup", "On", 0) == 1);
	bool backingUpAutoSaves = (m_INI.GetInt("AutoBackup", "AutoSaves", 0) == 1);

	if ((backingUpSaves && !isAutosave) || (backingUpAutoSaves && isAutosave)) {

		int numBackups = m_INI.GetInt("AutoBackup", "NumBackups", 0);

		CString appFileName = pApplication->file_information.file_path;
		if (appFileName.Right(4).MakeLower() == ".cap")
			appFileName = appFileName.Left(appFileName.GetLength() - 4);

		// Backup N is out of date - delete it
		CString backupFile;
		backupFile.Format("%s.bak%d.cap", appFileName, numBackups);
		DeleteFile(backupFile);

		// Loop from backup N to backup 1
		for (int i = numBackups - 1; i > 0; i--) {

			CString curBackup;
			CString nextBackup;

			// Rename backup N to backup N + 1
			curBackup.Format("%s.bak%d.cap", appFileName, i);
			nextBackup.Format("%s.bak%d.cap", appFileName, i + 1);
			rename(curBackup, nextBackup);
		}

		// Copy the old version of the .cap file to backup 1 - it has not yet been replaced with the latest save data
		backupFile.Format("%s.bak1.cap", appFileName);
		CopyFile(pApplication->file_information.file_path, backupFile, FALSE);
	}

	// Now copy to intended location
	DeleteFile(pApplication->file_information.file_path);
	rename(Name, pApplication->file_information.file_path);

	// Finish up
	pApplication->ChangeModifiedStatus(false);

	if (!isAutosave)
		AddToMRU(pApplication);

	return true;
}

bool CMainFrame::OnFileSave(CApplication* pApplication, bool isAutosave) 
{
	if (pApplication->file_information.file_path == "") return OnFileSaveAs(pApplication);

	// Open file selected
	CString Name = pApplication->file_information.file_path;
	Name += ".tmp";

	return DoSave(Name, pApplication, isAutosave);
}

void CMainFrame::AddToMRU(CApplication* pApplication)
{
	CStringArray MRU;
	for (int i = 1; i <= 10; i++)
	{
		CString Item;
		Item.Format("%d", i);
		MRU.Add(m_INI.GetString("Recent", Item));
	}

	m_INI.WriteString("Recent", "1", pApplication->file_information.file_path);

	for (int mru_index = 0, ini_index = 2; mru_index < 10; mru_index++) {

		if (MRU.GetAt(mru_index) != pApplication->file_information.file_path) {

			CString iniNum;
			iniNum.Format("%d", ini_index);

			m_INI.WriteString("Recent", iniNum, MRU.GetAt(mru_index));
			ini_index++;
		}
	}

}