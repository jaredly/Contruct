
#include "StdAfx.h"

// Preview runtime only code
#ifdef CONSTRUCT_PREVIEW

////////////////////////////
// Unpack plugins to 0.csx, 1.csx etc.
bool CRuntime::UnpackPlugins(int startresource)
{
	// Get number of plugins
	char strbuf[256];

	if (LoadString(GetModuleHandle(NULL), IDS_numPlugins, strbuf, 256) == 0)
		throw runtime_error("Error unpacking resources (1)");

	numPlugins = atoi(strbuf);

	// If over 1000 plugins or negative, probably undefined string - show error
	if (numPlugins < 0 || numPlugins > 1000)
		throw runtime_error("Error unpacking resources (2)");

	// Loop all plugin resources
	for (int i = 0; i < numPlugins; i++) 
	{
		// Get number of plugins
		char tempstr[256];

		int curnum = 1000;
		curnum += i;
		curnum *= 16;
		curnum -= 15;

		//CString cur;
		//cur.Format("%d", curnum);

		LoadString(GetModuleHandle(NULL), curnum, tempstr, 256);

		char currentDirectory[MAX_PATH];
		GetModuleFileName(NULL, currentDirectory, MAX_PATH);

		char pathDrive[MAX_PATH];
		char pathDir[MAX_PATH];
		_splitpath(currentDirectory, pathDrive, pathDir, NULL, NULL);

		CString myPath = pathDrive;
		myPath += pathDir;
		myPath += "Construct.ini";
	
		char installPath[MAX_PATH];

		GetPrivateProfileString("Path", "Install", "", installPath, MAX_PATH, myPath);

		CString curFile;
		curFile.Format("%sPlugins\\Runtime\\%s", installPath, tempstr);

		// Load up this module
		PluginModule plugin;
		plugin.module = LoadLibrary(CSTR(curFile));
		plugin.previewName = tempstr;

		int e = GetLastError();

		if (plugin.module == NULL) {
			throw runtime_error((const char*)(CString("Failed to load plugin ") + tempstr));
		}

		// Get proc addresses
		plugin.RTCreateObject = (LPRTCREATEOBJECT)GetProcAddress(plugin.module, "RTCreateObject");
		plugin.RTDestroyObject = (LPRTDESTROYOBJECT)GetProcAddress(plugin.module, "RTDestroyObject");
		plugin.RTWindowProc = (PLUGINWNDPROC)GetProcAddress(plugin.module, "RTWindowProc");

		// Initialise
		LPRTDLLLOAD RTDllLoad = (LPRTDLLLOAD)GetProcAddress(plugin.module, "RTDllLoad");

		// Handle vectors
		PluginModuleVectors emv;
		pluginvecs.push_back(emv);
		PluginModuleVectors* pv = &(pluginvecs.back());

		MicroAceTool mat(&(pv->CndRoutines), &(pv->ActRoutines), &(pv->ExpRoutines), &(pv->ExpNames));
	
		// Load vectors and get IDE flags
		plugin.ideFlags = RTDllLoad(&mat);

		// Get the plugin routine tables
		plugin.pvCndRoutines = &(pv->CndRoutines);
		plugin.pvActRoutines = &(pv->ActRoutines);
		plugin.pvExpRoutines = &(pv->ExpRoutines);
		plugin.pvExpNames = &(pv->ExpNames);

		// Make all expression names lowercase
		vector<CString>::iterator j;
		for (j = plugin.pvExpNames->begin(); j != plugin.pvExpNames->end(); j++)
			j->MakeLower();
		
		// Add to plugin list
		plugins.push_back(plugin);
	}

	return true;
}

#endif // CONSTRUCT_PREVIEW