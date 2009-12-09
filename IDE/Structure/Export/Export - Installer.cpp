// File includes
#include "StdAfx.h"
#include "Export.h"
#include "..\..\Utilities\zip.h"

void CExport::CreateInstaller()
{
	if (m_bInstaller)
	{
		CPath DataPath;
		DataPath.SetToCurrentDirectory();

		CPath AppDataPath;
		AppDataPath.SetToAppDataDirectory("Scirra");

		// Installer.exe location
		CString InstallerOut = DataPath.GetFullPath();
		InstallerOut += "\\Data\\Installer.exe";

		// Make ZIP file
		CString ZIPFile;
		ZIPFile.Format("%sData.zip", AppDataPath.GetFullPath());

		// Delete file
		DeleteFile(ZIPFile);

		// Create ZIP
		HZIP hZIP = CreateZip(ZIPFile, 0);

		CPath OutName;
		OutName.SetPath(m_Out);

		// Write data file
		CFile File;
		CString Name;
		Name.Format("%sdata.bin", AppDataPath.GetFullPath());
		File.Open(Name, CFile::modeCreate | CFile::modeWrite, NULL);
		CArchive ar(&File, CArchive::store, NULL, NULL);

		CString OS;

		// Write settings
		switch (m_OS)
		{
		case 0:
		case 1:
			OS = "NT";
			break;
		case 2:
			OS = "XP";
			break;
		case 3:
			OS = "Vista";
			break;
		}

		ar << application->file_information.name;
		ar << application->file_information.creator; 
		ar << m_License;					// License
		ar << m_InstallPath;				// Install path
		ar << OutName.GetFileName();		// Run file
		ar << (int)0 << (int)0 << (int)0;	// Fullscreen, gradient start, gradient end
		ar << OS;							// Minimum OS. NT, XP, Vista
		ar << (int)0;						// Needs DX (1 = true)

		// Close
		ar.Close();
		File.Close();
		
		// Add files to ZIP
		ZipAdd(hZIP, "data.bin", Name);
		CString Watermark;
		Watermark.Format("%s\\Data\\watermark.bmp", DataPath.GetFullPath());
		ZipAdd(hZIP, "watermark.bmp", Watermark);
		ZipAdd(hZIP, OutName.GetFileName(), m_Out);

		// Close
		CloseZip(hZIP);

		// Copy installer.exe
		m_InstallerOut.Format("%sInstaller.exe", AppDataPath.GetFullPath());
		CopyFile(InstallerOut, m_InstallerOut, FALSE);

		// UpdateResource it
		HANDLE UpdateHandle = BeginUpdateResource(m_InstallerOut, FALSE);
		FILE* f = fopen(ZIPFile, "rb");

		fseek(f, 0, SEEK_END);	// Go to end
		int Size = ftell(f);
		fseek(f, 0, SEEK_SET);	// Go back to beginning

		BYTE* pBuffer = new BYTE[Size];
		fread(pBuffer, 1, Size, f);
		fclose(f);

		UpdateResource(UpdateHandle, "RUNTIME", MAKEINTRESOURCE(131), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					   pBuffer, Size);

		delete[] pBuffer;

		// Done
		EndUpdateResource(UpdateHandle, FALSE);
	}
}