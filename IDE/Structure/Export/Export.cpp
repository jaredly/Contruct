// Export.cpp : Exporting functions
//

// File includes
#include "StdAfx.h"
#include "Export.h"
#include "..\..\Utilities\Ini.h"
#include "..\..\Dialogs\Export Wizard Dialog\Export Wizard Dlg.h"

// Helper function for adding an int to binary
void AddBinaryInt(CString& s, int value)
{
	s += ((char*)&value)[0];
	s += ((char*)&value)[1];
	s += ((char*)&value)[2];
	s += ((char*)&value)[3];
}

// Helper function for adding binary to string
void AddBinaryToString(CString& s, void* data, int size)
{
	for (int i = 0; i < size; i++)
		s += ((char*)data)[i];
}

// Helper function for padding
void PadString(CString& s, int count)
{
	for (int i = 0; i < count; i++)
		s += '\0';
}

// Helper function for allocating a WCHAR
wchar_t* AllocStringResourceFormat(const char* ascii, int resID, int* buflen)
{
	int size = (strlen(ascii) + 1 + resID) * 2;

	// Rule 1:  size must be even
	if (size % 2 == 1) size++;
	*buflen = size;

	wchar_t* theString = new wchar_t[size];

	// Null out last char
	theString[size - 1] = 0;

	int i;

	// Rule 2:  resID number of null wchar_t's must prepend string
	for (i = 0; i < resID; i++)
		theString[i] = 0;

	// Rule 3:  follow with number of characters
	theString[i] = strlen(ascii);

	i++;

	// Now add the actual unicode characters
	MultiByteToWideChar(CP_ACP, 0, ascii, -1, theString + i, size / 2);

	// Done
	return theString;
}

//TODO: someone comment and identify this function!
// I made it :P - DAVO
// its a conversion that is necassary. Without it, runtime crashes
// Revised 5 Nov 2006 by DAVO
int CExport::ObjTypeIDCnvt(int obj)
{
	if(obj == -1)
		return -1;
	if(m_ExportTypesNumbers.find(obj) == m_ExportTypesNumbers.end())
	{
		// could be a family
		int index = m_ExportTypesNumbers.size() + BehaviorCount + TraitsCount;
		for(vector<FamilyLookup>::iterator i = Families.begin();
			i != Families.end();
			i++)
		{
			if(i->m_Family->identifier == obj)
			{
				return index;
			}
			index++;
		}
		return -2;
	}
	else
		return m_ExportTypesNumbers[obj];
}

int CExport::ObjIDCnvt(int obj)
{
	return obj;
}

// Generate application data
void CExport::GenerateApplicationData(int layoutid)
{
	appBlock << application->file_information.name;

	appBlock << application->window_information.window_width;
	appBlock << application->window_information.window_height;
	appBlock << application->window_information.eye_distance;

	appBlock << application->window_information.show_menu;
	appBlock << m_bScreensaver;

	appBlock << (BYTE)application->game_information.fps_mode;
	appBlock << application->game_information.fps;

	bool Old = application->game_information.fullscreen;

	// Fullscreen
	if (m_bScreensaver)
		application->game_information.fullscreen = true;

	appBlock << (application->game_information.fullscreen ? true : false);

	application->game_information.fullscreen = Old;

	// Sampler state
	appBlock << (int)application->game_information.sampler;

	// Global vars
	appBlock << application->global_variables.size();

	list<CApplication::GlobalVariable>::iterator i =  application->global_variables.begin();
	list<CApplication::GlobalVariable>::iterator globals_end =  application->global_variables.end();

	for ( ; i != globals_end; i++) {
		appBlock << i->name << i->type << i->value;
	}

	// Export controls
	appBlock << application->m_Controls.size();

	vector<BehaviorControl>::iterator c = application->m_Controls.begin();

	for ( ; c != application->m_Controls.end(); c++) {
		appBlock << c->name << c->vk << c->player;
	}

	appBlock << application->window_information.disable_windows_key;

	// Export global keys
	appBlock << application->globalKeys.GetSavedKeyCount();

	map<CString, KeyData>::iterator k = application->globalKeys.keys.begin();
	for ( ; k != application->globalKeys.keys.end(); k++)
	{
		// Save if necessary
		if (k->second.bSave)
		{
			appBlock << k->first;
			appBlock << k->second.iType;

			if (k->second.iType == 0)
				appBlock << k->second.pData;
			else
				appBlock << k->second.String;
		}
	}

	// Export simulated shader
	appBlock << (int)application->game_information.simulate_shaders;

	// Export source .cap name
	appBlock << application->file_information.file_path;
	appBlock << application->game_information.fps_in_caption;
	appBlock << application->game_information.use_motion_blur;
	appBlock << application->game_information.motion_blur_steps;

	appBlock << (int)application->game_information.text_rendering_mode;

	appBlock << application->game_information.override_timedelta;
	appBlock << application->game_information.time_delta_override;

	appBlock << application->window_information.caption;
	appBlock << application->window_information.minimize_box;
	appBlock << (int)application->window_information.resizable;
	appBlock << application->window_information.maximize_box;

	appBlock << application->game_information.minimum_fps;

	// Previewing specific layout?
	if (layoutid == -1)
		appBlock << -1;			// Preview entire application
	else {
		// Find index from id
		POSITION Position = application->layouts.GetHeadPosition();

		for (int i = 0; i < application->layouts.GetCount(); i++) 
		{
			CLayout* curLayout = application->layouts.GetNext(Position);

			if (curLayout->identifier == layoutid) {
				appBlock << i;
				break;
			}
		}//for
	}//else

	appBlock << application->game_information.multisamples;

	appBlock << (int)application->game_information.texture_loading;
}

void CExport::Preview(CApplication* pApplication, int layoutid)
{
	ProgressDlg.Start("Loading preview");
	m_bScreensaver = false;

	application = pApplication;

	LayoutBlock.allocator = &g_allocator;
	eventBlock.allocator = &g_allocator;
	appBlock.allocator = &g_allocator;
	imageBlock.allocator = &g_allocator;
	hlslBlock.allocator = &g_allocator;
	menuBlock.allocator = &g_allocator;

	CPath path;
	path.SetToAppDataDirectory("Scirra");
	CString OutPath;
	OutPath.Format("%sTemp.exe", path.GetFullPath());

	CString iniPath;
	iniPath.Format("%sConstruct.ini", path.GetFullPath());

	CIni ini;
	ini.SetPathName(iniPath);
	path.SetToCurrentDirectory();
	BOOL ret = ini.WriteString("Path", "Install", path.GetFullPath());

	CString DataEXE;

	if (application->runtime == CApplication::rt_application)
		DataEXE.Format("%sData\\App_p", path.GetFullPath());

	else if (application->runtime == CApplication::rt_directx)
		DataEXE.Format("%sData\\DX9_p", path.GetFullPath());

	if (application->game_information.use_python)
		DataEXE += "s";		// script enabled

	DataEXE += ".exe";

	// Copy runtime.exe to the target file
	if (DeleteFile(OutPath) == 0)
	{
		if (FileExists(OutPath))
		{
			CErrorDlg Dlg;
			Dlg.Error("Preview failed", "A preview is already running. Please close this before previewing again.");

			return;
		}
	}

	ret = CopyFile(DataEXE, OutPath, false);

	// File copied, assume 10% progress
	ProgressDlg.SetProgress(10);

	DoExport(true, OutPath, false, layoutid);

	// Execute temp
	ret = (BOOL)ShellExecute(NULL, "open", OutPath, NULL, NULL, SW_SHOW);

	// Done
	ProgressDlg.Finish();
}

void CExport::PreviewDebug(CApplication* pApplication, int layoutid)
{
	ProgressDlg.Start("Loading debug");
	m_bScreensaver = false;

	application = pApplication;

	LayoutBlock.allocator = &g_allocator;
	eventBlock.allocator = &g_allocator;
	appBlock.allocator = &g_allocator;
	imageBlock.allocator = &g_allocator;
	hlslBlock.allocator = &g_allocator;
	menuBlock.allocator = &g_allocator;

	CPath path;
	path.SetToAppDataDirectory("Scirra");
	CString OutPath;
	OutPath.Format("%sTemp.exe", path.GetFullPath());

	CString iniPath;
	iniPath.Format("%sConstruct.ini", path.GetFullPath());
	CIni ini;
	ini.SetPathName(iniPath);
	path.SetToCurrentDirectory();
	ini.WriteString("Path", "Install", path.GetFullPath());

	CString DataEXE;

	if (application->runtime == CApplication::rt_application)
		DataEXE.Format("%sData\\App_pd", path.GetFullPath());

	else if (application->runtime == CApplication::rt_directx)
		DataEXE.Format("%sData\\DX9_pd", path.GetFullPath());

	if (application->game_information.use_python)
		DataEXE += "s";		// script enabled

	DataEXE += ".exe";

	// Copy runtime.exe to the target file
	CopyFile(DataEXE, OutPath, false);

	// File copied, assume 10%
	ProgressDlg.SetProgress(10);

	DoExport(true, OutPath, false, layoutid);

	// Execute temp
	ShellExecute(NULL, "open", OutPath, NULL, NULL, SW_SHOW);

	// Done
	ProgressDlg.Finish();
}

void CExport::ExportToEXE(CApplication* pApplication)
{
	application = pApplication;

	// Prepare allocators
	LayoutBlock.allocator = &g_allocator;
	eventBlock.allocator = &g_allocator;
	appBlock.allocator = &g_allocator;
	imageBlock.allocator = &g_allocator;
	hlslBlock.allocator = &g_allocator;
	menuBlock.allocator = &g_allocator;

	// Open export wizard
	CExportWizardDlg Dlg;
	Dlg.application = pApplication;
	if (Dlg.DoModal() != IDOK) return;

	// Transfer settings
	m_bInstaller	= Dlg.m_bInstaller;
	m_License		= Dlg.m_License;
	m_InstallPath	= Dlg.m_InstallPath;
	m_OS			= Dlg.m_OS;
	m_bScreensaver	= Dlg.m_bScreensaver;

	// Make paths
	CPath Path;
	Path.SetToCurrentDirectory();
	CString OutPath;

	// Application
	if (application->runtime == CApplication::rt_application)
		OutPath.Format("%sData\\App", Path.GetFullPath());

	// Direct-X
	else if (application->runtime == CApplication::rt_directx)
		OutPath.Format("%sData\\DX9", Path.GetFullPath());

	// Append _s for scripting (python)
	if (application->game_information.use_python)
		OutPath += "_s";		// script enabled

	OutPath += ".exe";

	// Copy runtime.exe to the target file
	CopyFile(OutPath, Dlg.m_OutputPath, false);

	// File copied, assume 10% progress
	ProgressDlg.Start("Exporting");
	ProgressDlg.SetProgress(10);

	// Transfer path
	m_Out = Dlg.m_OutputPath;

	// Screensaver
	//if (m_bScreensaver)
	//	Dlg.m_OutputPath.Replace(".exe", ".scr");

	DoExport(false, Dlg.m_OutputPath, false);

	ProgressDlg.Finish();

	// Python reminder
	if (application->game_information.use_python) {
		MessageBox(NULL, "Your application has scripting enabled and requires Python26.dll to run.  This file has been "
			"copied to the same directory as your EXE - remember to include it when sending or distributing your game!",
			"Export EXE reminder", MB_ICONINFORMATION | MB_OK);
	}
}

void CExport::DoExport(bool previewMode, const CString& pathName, bool bApp, int layoutid)
{
	// Progress dialog is going from 10% to 100% in this function

	// Make the DLL list
	CList<CString, CString&> dllList;

	CPath path;
	path.SetToCurrentDirectory();

	// Allocate some initial space for the binary blocks
	imageBlock.allocate(20000);			// 20kb initial image buffer
	eventBlock.allocate(5000);			// 5kb initial event buffer
	LayoutBlock.allocate(5000);			// 5kb initial frame buffer
	appBlock.allocate(1000);			// 1kb initial app buffer
	hlslBlock.allocate(5000);			// 5kb initial hlsl buffer
	menuBlock.allocate(1000);

	// Generate
	GenerateLayout(dllList);	// also loads DLLs to dllList
	ProgressDlg.SetProgress(15);
	GenerateEvents();
	ProgressDlg.SetProgress(20);
	GenerateApplicationData(layoutid);
	ProgressDlg.SetProgress(25);
	GenerateImages();				// Longest job, use 25-80%
	ProgressDlg.SetProgress(80);

	// Open the file for addition of file data in resource
	m_UpdateHandle = BeginUpdateResource(pathName, FALSE);

	// Do python
	GeneratePython();

	// HLSL
	UpdateResource(m_UpdateHandle, "HLSL", MAKEINTRESOURCE(994), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					hlslBlock.ptr(), hlslBlock.size());

	ProgressDlg.SetProgress(81);

	// Images
	UpdateResource(m_UpdateHandle, "IMAGEBLOCK", MAKEINTRESOURCE(995), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					imageBlock.ptr(), imageBlock.size());

	ProgressDlg.SetProgress(82);

	// Application settings
	UpdateResource(m_UpdateHandle, "APPBLOCK", MAKEINTRESOURCE(997), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					appBlock.ptr(), appBlock.size());

	ProgressDlg.SetProgress(83);

	// Layouts
	UpdateResource(m_UpdateHandle, "LEVELBLOCK", MAKEINTRESOURCE(998), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					LayoutBlock.ptr(), LayoutBlock.size());

	ProgressDlg.SetProgress(84);

	// Events
	UpdateResource(m_UpdateHandle, "EVENTBLOCK", MAKEINTRESOURCE(999), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					eventBlock.ptr(), eventBlock.size());

	ProgressDlg.SetProgress(85);

	// Number of DLLs
	long c = dllList.GetCount();
	CString ibuf;
	long te = c;
	te += MvtLookup.size();
	ibuf.Format("%d", te);
	int ilen = 0;

	// Make string of DLL count in to resource string format
	wchar_t* resString = AllocStringResourceFormat(ibuf, 1, &ilen);

	// String of DLL count is a string at resource 1
	UpdateResource(m_UpdateHandle, RT_STRING, MAKEINTRESOURCE(1), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					resString, ilen);

	// AllocStringResourceFormat makes a new string
	delete [] resString;

	CString dll;
	POSITION pos4 = dllList.GetHeadPosition();

	int d = 0;
		
	for (d = 1000; d < (1000 + c); d++)
	{
		// In preview mode, we only put the string of the plugin name in the resources
		if (previewMode) {
			dll = dllList.GetNext(pos4);

			ilen = 0;

			// Make string of DLL count in to resource string format
			wchar_t* resString = AllocStringResourceFormat(dll, 1, &ilen);

			UpdateResource(m_UpdateHandle, RT_STRING, MAKEINTRESOURCE(d), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
								resString, ilen);

			delete [] resString;
		}
		// In export EXE mode, the actual DLL file must be dumped in resources
		else {

			CString dllPath;
			dllPath.Format("%sPlugins\\Runtime\\%s", path.GetFullPath(), dllList.GetNext(pos4));

			// Get the DLL file binary
			FILE* f = fopen(dllPath, "rb");
			fseek(f, 0, SEEK_END);
			int fsize = ftell(f);
			fseek(f, 0, SEEK_SET);

			BYTE* fdata = new BYTE[fsize];
			fread(fdata, 1, fsize, f);
			fclose(f);
	
			UpdateResource(m_UpdateHandle, "DLLBLOCK", MAKEINTRESOURCE(d), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
							 fdata, fsize);

			delete [] fdata;
		}

	}//for

	ProgressDlg.SetProgress(88);

	// Behavior DLLs are added afterwards
	int iter = 0;
	for (int Mvt = d; Mvt < (d + MvtLookup.size()); Mvt++)
	{
		if (previewMode) {
			dll = MvtLookup[iter].Name;
			ilen = 0;

			// Make string of DLL count in to resource string format
			wchar_t* resString = AllocStringResourceFormat(dll, 1, &ilen);

			UpdateResource(m_UpdateHandle, RT_STRING, MAKEINTRESOURCE(Mvt), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
								resString, ilen);

			delete [] resString;
			iter++;
		}
		// In EXE mode export actual DLL file
		else {

			CString dllPath;
			dllPath.Format("%sPlugins\\Runtime\\%s", path.GetFullPath(), MvtLookup[iter].Name);

			// Get the DLL file binary
			FILE* f = fopen(dllPath, "rb");
			fseek(f, 0, SEEK_END);
			int fsize = ftell(f);
			fseek(f, 0, SEEK_SET);

			BYTE* fdata = new BYTE[fsize];
			fread(fdata, 1, fsize, f);
			fclose(f);
	
			UpdateResource(m_UpdateHandle, "DLLBLOCK", MAKEINTRESOURCE(Mvt), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
							 fdata, fsize);

			delete [] fdata;

			iter++;

		}
	}//for

	ProgressDlg.SetProgress(90);

	ExportResources(previewMode);

	ProgressDlg.SetProgress(99);

	UpdateResource(m_UpdateHandle, "MENUBLOCK", MAKEINTRESOURCE(993), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					menuBlock.ptr(), menuBlock.size());

	// Write DLL segment
	EndUpdateResource(m_UpdateHandle, FALSE);

	// Free the bin data
	imageBlock.free();
	eventBlock.free();
	appBlock.free();
	LayoutBlock.free();
	hlslBlock.free();
	menuBlock.free();

	if (m_bInstaller)
	{
		// Now create an installer if necessary
		CreateInstaller();

		// Delete the old file
		DeleteFile(m_Out);

		// Transfer installer to path
		CPath Transfer;
		Transfer.SetPath(m_Out);

		CopyFile(m_InstallerOut, m_Out, FALSE);
	}

	// Now we're done
	ProgressDlg.SetProgress(100);
}