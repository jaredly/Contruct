// File includes
#include "StdAfx.h"
#include "Export.h"
#include "..\..\Utilities\zip.h"

void CExport::GeneratePython()
{
	if (application->game_information.use_python)
	{
		CPath Path;
		Path.SetToAppDataDirectory("Scirra");

		// Make ZIP file
		CString ZIPFile;
		ZIPFile.Format("%sPython.zip", Path.GetFullPath());

		// Delete file
		DeleteFile(ZIPFile);

		// Create ZIP
		HZIP hZIP = CreateZip(ZIPFile, 0);

		// Add relevant files
		Path.SetToCurrentDirectory();
		
		CString InfoPath;
		InfoPath.Format("%sdata\\Python\\", Path.GetFullPath());

		// Force StringIO to be included!
		application->m_PythonDistrib["StringIO.pyc"] = 0; 
		application->m_PythonDistrib["zipfile.pyc"] = 0; 
		application->m_PythonDistrib["struct.pyc"] = 0; 
		application->m_PythonDistrib["os.pyc"] = 0; 
		application->m_PythonDistrib["ntpath.pyc"] = 0; 
		application->m_PythonDistrib["stat.pyc"] = 0; 
		application->m_PythonDistrib["UserDict.pyc"] = 0; 
		application->m_PythonDistrib["copy_reg.pyc"] = 0; 
		application->m_PythonDistrib["types.pyc"] = 0; 

		for(map<CString, int>::iterator i = application->m_PythonDistrib.begin();
			i != application->m_PythonDistrib.end();
			i++)
		{
			CString File = InfoPath + i->first;

			ZipAdd(hZIP, i->first,  File);
		}
		
		CloseZip(hZIP);

		// Load file in to put in resource
		FILE* f = fopen(ZIPFile, "rb");

		fseek(f, 0, SEEK_END);	// Go to end
		int Size = ftell(f);
		fseek(f, 0, SEEK_SET);	// Go back to beginning

		BYTE* pBuffer = new BYTE[Size];
		fread(pBuffer, 1, Size, f);
		fclose(f);

		UpdateResource(m_UpdateHandle, "PYTHONLIBS", MAKEINTRESOURCE(992), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					   pBuffer, Size);

		delete[] pBuffer;		

		// Move DLL
		CString pyDllPath;
		pyDllPath.Format("%sPython26.dll", Path.GetFullPath());

		CPath appDir;
		appDir.SetToAppDataDirectory("Scirra");
		CString pyAppDataDllPath;
		pyAppDataDllPath.Format("%sPython26.dll", appDir.GetFullPath());

		CopyFile(pyDllPath, pyAppDataDllPath, TRUE); // Fail to copy if file already there
	}
}