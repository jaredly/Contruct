// Miscfunc.h: Definitions for miscellaneous functions.

#ifndef MISCFUNC_H
#define MISCFUNC_H

bool GetListOfFiles(vector<CString>& paths, CString path, CString path_name);

// For threads
#include <process.h>

// COM interfaces
#include <urlmon.h>

#if _MSC_VER < 1500
typedef __int64 QWORD, *LPQWORD;
#endif

#define MAKEQWORD(a, b)	\
	((QWORD)( ((QWORD) ((DWORD) (a))) << 32 | ((DWORD) (b))))

// Wrap SHBrowseForFolder
class CFolderDlg
{
public:
	// Set properties
	void SetTitle(LPCSTR browseTitle);
	void SetInitialDirectory(LPCSTR directoryPath);
	void SetUseNewDialog(bool useNewDialog);

	// Get properties
	CString GetTitle();
	CString GetInitialDirectory();
	bool GetUseNewDialog();

	// Get path name
	CString GetPathName();

	// Do modal
	int DoModal();

protected:
	// Properties
	CString m_Title,
		    m_InitialDirectory,
	        m_ReturnPath;

	bool m_UseNewDialog;

	static INT CALLBACK BrowseDlgCallback(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData);
};

// Wrap various path functions
class CPath
{
public:
	// Set path
	void SetPath(LPCSTR setPath);

	// Set path from this application
	void SetToCurrentDirectory();
	void SetToAppDataDirectory(CString appName);

	// Get attributes
	CString GetDrive();
	CString GetPath();
	CString GetFullPath();
	CString GetFileName();
	CString GetFileTitle();
	CString GetFileExtension();

protected:
	// Properties
	CString m_Drive,
		    m_Path,
			m_FullPath,
			m_FileName,
			m_FileTitle,
			m_FileExtension;
};

// For downloading files
class CDownload 
{
public:
	// Begin a download
	void BeginDownload(LPCSTR remoteFile, LPCSTR localFile);

	// Cancel the download
	void CancelDownload();

	// Get attributes
	CString GetLocalFile();
	CString GetRemoteURL();
	int GetTotalBytes();
	int GetTransferredBytes();

	// Must be public
	int     m_TotalBytes,
	        m_TransferredBytes;
	bool    m_DoCancel;

	// Callback functions, derive a class and override these if necessary
	virtual void DownloadComplete() { /* */ }
	virtual void DownloadError() { /* */ }
	virtual void DownloadFailed() { /* */ }
	virtual void DownloadProgress() { /* */ }

	// Properties
	CString m_RemoteURL,
		    m_LocalPath;
};

// Basic clipboard class
class CClipboard
{
public:
	// Retrieve data
	HBITMAP GetClipboardBitmap();
	CString GetClipboardText();

	// Put data
	void SetClipboardBitmap(HBITMAP toPut);
	void SetClipboardText(LPCSTR toPut);
};

// Deals with file times

//PORT: this is an ambiguous symbol with ATL::CFileTime - using CFileTimeN
class CFileTimeN
{
public:
	// Set file
	void SetFile(LPCSTR newFile);

	// Get attributes
	CTime GetCreationTime();
	CTime GetAccessTime();
	CTime GetModifiedTime();

protected:
	// Attributes
	CTime m_CreationTime,
		  m_AccessTime,
		  m_ModifiedTime;
};

// Creates CRC32 hashes
class CCRC32
{
public:
	// Hashing functions
	DWORD HashString(LPCTSTR szString);
	DWORD HashFile(LPCTSTR szFilename);

	// Format a CRC32 value into a string
	CString FormatCRC32(DWORD inCRC);

protected:
	// Get file size
	inline void CalculateCRC32(const BYTE byte, DWORD &dwCrc32);

	// CRC32 table
	static DWORD crcTable[256];
};

// Lots of extra enhancements and stuff to make CListCtrl easier
class CListCtrlX : public CListCtrl
{
public:
	// Get selected item
	int GetSelectedItem();
};

// Easily list files
class CListFiles
{
public:
	// List all files in a directory
	void ListFiles(LPCSTR targetDirectory, bool searchSubDirectories);

	// Get number of results
	int GetNumberOfResults();

	// Get result at
	CString GetResultAt(int resultPosition);
	
protected:
	// Files
	CStringArray m_FoundFiles;
};

#endif // Only include once