// File includes
#include "StdAfx.h"
#include "Export.h"

#pragma pack(push, 2)

typedef struct {
	WORD Reserved1;       // reserved, must be 0
	WORD ResourceType;    // type is 1 for icons
	WORD ImageCount;      // number of icons in structure (1)
	BYTE Width;           // icon width (32)
	BYTE Height;          // icon height (32)
	BYTE Colors;          // colors (0 means more than 8 bits per pixel)
	BYTE Reserved2;       // reserved, must be 0
	WORD Planes;          // color planes
	WORD BitsPerPixel;    // bit depth
	DWORD ImageSize;      // size of structure
	WORD ResourceID;      // resource ID
} GROUPICON;

#pragma pack(pop)

bool FileExists(CString path)
{
	return GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES;
}

void GetFileModifiedTime(CString path, FILETIME& ft)
{
	HANDLE f = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	GetFileTime(f, NULL, NULL, &ft);	// Last written time
	CloseHandle(f);
}

void CopyFileIfDifferent(CString from, CString to)
{
	if (FileExists(to)) {

		// Check modified-by dates
		FILETIME fromDate, toDate;
		GetFileModifiedTime(from, fromDate);
		GetFileModifiedTime(to, toDate);

		// Target file is not the same modified date - copy the source file over it
		if (fromDate.dwHighDateTime != toDate.dwHighDateTime || fromDate.dwLowDateTime != toDate.dwLowDateTime)
			CopyFile(from, to, FALSE);

	}
	// Dest file does not exist
	else
		CopyFile(from, to, FALSE);		// Copy anyway
}

void CExport::ExportResources(bool previewing)
{
	// Export dependencies files
	for (int i = 0; i < m_Dependencies.GetCount(); i++)
	{
		CPath Path;
		Path.SetToCurrentDirectory();
		CString File = Path.GetFullPath();
		File += "Plugins\\Runtime\\";
		File += m_Dependencies.GetAt(i);

		// Add to resources only when exporting EXEs - otherwise just copy the file to appdata
		if (previewing) {
			CPath appDir;
			appDir.SetToAppDataDirectory("Scirra");
			CString appDataFile = appDir.GetFullPath();
			appDataFile += m_Dependencies.GetAt(i);

			CopyFileIfDifferent(File, appDataFile);
		}
		// Exporting EXE
		else {
			// Get the file binary
			FILE* pFile = fopen(File, "rb");
			if (pFile == NULL) {
				CString msg;
				CErrorDlg Dlg;
				msg.Format("Warning: cannot attach dependency %s: please ensure the file exists.", File);
				Dlg.Error("Export error", msg);

				continue;
			}

			fseek(pFile, 0, SEEK_END);
			int Size = ftell(pFile);
			fseek(pFile, 0, SEEK_SET);

			BYTE* pData = new BYTE[Size];
			fread(pData, 1, Size, pFile);
			fclose(pFile);
		
			// Dumb WinAPI requires uppercase name.
			CString upperName = m_Dependencies.GetAt(i);
			upperName.MakeUpper();

			UpdateResource(m_UpdateHandle, "DEPENDENCIES", upperName, 
						   MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), pData, Size);
		}
	}

	// Export binary files
	int index = 1;

	bool no_to_all = false;

	for(list<BinaryResource>::iterator i = application->resources.file_resources.begin(); i != application->resources.file_resources.end(); i++) 
	{
		// check if file exists
		if (!FileExists(i->path))
		{
			if (no_to_all) continue;

			// offer a chance to find a new copy
			CString message;
			message.Format("Resource file: '%s' could not be found; do you want to search for it?", i->GetName());

			int result = CExtMsgBox::DoMsgBox(NULL, message, "Relink resource", __EXT_MB_YESNO | __EXT_MB_NO_TO_ALL);

			if (result == IDYES)
			{
				CFileDialog dialog(TRUE);

				if (dialog.DoModal() == IDOK)
					i->path = dialog.GetPathName();
			}

			else if (result == ID_EXT_MBB_NO_TO_ALL)
			{
				no_to_all = true;
				continue;
			}

			else
				continue;
		}

		// Get the DLL file binary
		FILE* pFile = fopen(i->path, "rb");

		fseek(pFile, 0, SEEK_END);
		int Size = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);

		BYTE* pData = new BYTE[Size];
		fread(pData, 1, Size, pFile);
		fclose(pFile);
	
		UpdateResource(m_UpdateHandle, "FILES", MAKEINTRESOURCE(index), 
					   MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), pData, Size);
 
		index++;

		// Progress 90-99
		ProgressDlg.SetProgress(90 + (9 * index) / application->resources.file_resources.size());
	}

	// Export icons
	BYTE* tempBuffer = NULL;
	long theSize;
	application->resources.icon_resources.front().icon.Encode(tempBuffer, theSize, CXIMAGE_FORMAT_ICO);

	UpdateResource(m_UpdateHandle, RT_ICON, MAKEINTRESOURCE(1), MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), (tempBuffer+22), theSize - 22);

	GROUPICON grData;
	grData.Reserved1 = 0;     // reserved, must be 0
	grData.ResourceType = 1;  // type is 1 for icons
	grData.ImageCount = 1;    // number of icons in structure (1)

	// This is the directory entry
	grData.Width        = 32;                 // icon width (32)
	grData.Height       = 32;                 // icon height (32)
	grData.Colors       = 0;                  // colors (256)
	grData.Reserved2    = 0;                  // reserved, must be 0
	grData.Planes       = 2;                  // color planes
	grData.BitsPerPixel = 32;                 // bit depth
	grData.ImageSize    = theSize - 22;    // size of image
	grData.ResourceID   = 1;                  // resource ID is 1

	UpdateResource(m_UpdateHandle, RT_GROUP_ICON, "MAINICON",
				   MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), &grData, sizeof(GROUPICON));

	application->resources.icon_resources.front().icon.FreeMemory(tempBuffer);

	// Export menus
	menuBlock.clear();
	//menuBlock << (int)application->m_MenuResources.size();
	menuBlock << 0;

/*	for (list<MenuResource>::iterator i = application->m_MenuResources.begin(); i != application->m_MenuResources.end(); i++)
	{
		menuBlock << i->m_Name;

		menuBlock << (int)i->m_Items.size();

		for (int x = 0; x < i->m_Items.size(); x++)
			ExportMenuItems(i->m_Items[x]); 
	}*/
}

void CExport::ExportMenuItems(MenuResourceItem item)
{
	menuBlock << item.text;
	menuBlock << item.id;

	menuBlock << (int)item.items.size();

	for (int i = 0; i < item.items.size(); i++)
		ExportMenuItems(item.items[i]);
}