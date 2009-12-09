// Miscellaneous functions (miscfunc.cpp)


// Includes
#include "stdafx.h"
#include "miscfunc.h"

bool GetListOfFiles(vector<CString>& paths, CString path, CString path_name)
{
	CString Folder;
	Folder = path + "*";

	WIN32_FIND_DATA FindFileData;

	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(Folder, &FindFileData);

	DWORD id = 0;

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return false;
	} 
	else
	{
		while (FindNextFile(hFind, &FindFileData) != 0)
		{
			CString name = FindFileData.cFileName;
			if(name == "..")
				continue;
			CString _path, _path_name;
			_path.Format("%s%s\\", path, name);
			_path_name.Format("%s%s\\", path_name, name);
			if(!GetListOfFiles(paths, _path, _path_name))
			{
				_path_name.Format("%s%s", path_name, name);
				paths.push_back(_path_name);
			}
			
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////
// CFolderDlg class ///////////////////////////////////////////
///////////////////////////////////////////////////////////////

// Get title
CString CFolderDlg::GetTitle()
{
	return m_Title;
}

// Get initial directory
CString CFolderDlg::GetInitialDirectory()
{
	return m_InitialDirectory;
}

// Get dialog UI setting
bool CFolderDlg::GetUseNewDialog()
{
	return m_UseNewDialog;
}

// Set title
void CFolderDlg::SetTitle(LPCSTR browseTitle)
{
	m_Title = browseTitle;
}

// Set initial directory
void CFolderDlg::SetInitialDirectory(LPCSTR directoryPath)
{
	m_InitialDirectory = directoryPath;
}

// Set dialog UI setting
void CFolderDlg::SetUseNewDialog(bool useNewDialog)
{
	m_UseNewDialog = useNewDialog;
}

// Get returned path
CString CFolderDlg::GetPathName()
{
	return m_ReturnPath;
}

// Callback to set initial path
INT CALLBACK CFolderDlg::BrowseDlgCallback(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	// If the dialog's just starting
	if (uMsg == BFFM_INITIALIZED)
	{
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)pData);
	}

	return 0;
}

// Show the browse for folder dialog
int	CFolderDlg::DoModal()
{
	// Empty directory
	m_ReturnPath.Empty();

	// CoInitialize for COM functions
	CoInitialize(NULL);

	LPMALLOC allocBuffer;
	::SHGetMalloc(&allocBuffer);

	// Setup BROWSEINFO structure
	BROWSEINFO browseInfo;
	::ZeroMemory(&browseInfo, sizeof(BROWSEINFO));
	browseInfo.pidlRoot = 0;
	browseInfo.lpszTitle = m_Title;
	browseInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
	browseInfo.lpfn = BrowseDlgCallback;
	browseInfo.lParam = (long)(LPCSTR)m_InitialDirectory;

	//if (!m_UseNewDialog) browseInfo.ulFlags |= BIF_NEWDIALOGSTYLE;

	// Retrieve folder
	char folderBuffer[MAX_PATH];
	LPITEMIDLIST pidlFolder;

	// Open dialog
	pidlFolder = ::SHBrowseForFolder(&browseInfo);

	if (pidlFolder == 0) return IDCANCEL;

	// Retrieve path
	if(::SHGetPathFromIDList(pidlFolder, folderBuffer))
	{
		m_ReturnPath = folderBuffer;
	}

	allocBuffer->Free(pidlFolder);
	allocBuffer->Release();   

	// Uninitialize COM
	CoUninitialize();

	return IDOK;
}

///////////////////////////////////////////////////////////////
// CPath class ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// Set path (splits path)
void CPath::SetPath(LPCSTR setPath)
{
	char pathDrive[_MAX_DRIVE], pathDirectory[_MAX_DIR];
	char pathinputFileName[_MAX_FNAME], pathExtension[_MAX_EXT];

	_splitpath(setPath, pathDrive, pathDirectory, pathinputFileName, pathExtension);

	m_Drive = pathDrive;
	m_Path = pathDirectory;
	m_FileName.Format("%s%s", pathinputFileName, pathExtension);
	m_FileTitle = pathinputFileName;
	m_FileExtension = pathExtension;
	m_FullPath.Format("%s%s", pathDrive, pathDirectory);

	if (m_FullPath.Right(1) != "\\")
		m_FullPath += "\\";
}

// Set path from current application
void CPath::SetToCurrentDirectory()
{	
	// Path name
	char currentDirectory[MAX_PATH];
	GetModuleFileName(NULL, currentDirectory, MAX_PATH);	
	CString setPath = currentDirectory;

	SetPath(setPath);
}

void CPath::SetToAppDataDirectory(CString appName)
{
	TCHAR szPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szPath);
	CString thePath(szPath);
	if (thePath.Right(1) != "\\")
		thePath += "\\";
	thePath += appName;

	// Ensure directory is created
	CreateDirectory(thePath, NULL);

	// Add trailing slash ("..\Application Data\Scirra\")
	thePath += "\\";

	SetPath(thePath);
}

// Get drive
CString CPath::GetDrive()
{ 
	return m_Drive;
}

// Get path
CString CPath::GetPath()
{
	return m_Path;
}

// Get inputFile extension
CString CPath::GetFullPath()
{
	return m_FullPath;
}

// Get inputFilename
CString CPath::GetFileName()
{
	return m_FileName;
}

// Get inputFile title
CString CPath::GetFileTitle()
{
	return m_FileTitle;
}

// Get inputFile extension
CString CPath::GetFileExtension()
{
	return m_FileExtension;
}

///////////////////////////////////////////////////////////////
// CDownload class ////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// Download progress class
class CDownloadProgress : public IBindStatusCallback  
{
public:
    CDownloadProgress(void* theClass) 
	{
		// Copy our class data over
		currentSession = (CDownload*)theClass;
	}

	~CDownloadProgress() {}

	STDMETHOD(OnProgress)(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR wszStatusText)
	{
		// Transfer the useful information
		currentSession->m_TotalBytes = ulProgressMax;
		currentSession->m_TransferredBytes = ulProgress;

		// Call DownloadProgress function
		currentSession->DownloadProgress();

		if (currentSession->m_DoCancel)
			return E_ABORT;
		else
			return S_OK;
	}

	// Don't worry about the rest

    STDMETHOD(OnStartBinding)(DWORD dwReserved, IBinding __RPC_FAR *pib)
    { return E_NOTIMPL; }

    STDMETHOD(GetPriority)(LONG __RPC_FAR *pnPriority)
    { return E_NOTIMPL; }

    STDMETHOD(OnLowResource)(DWORD reserved)
    { return E_NOTIMPL; }

    STDMETHOD(OnStopBinding)(HRESULT hresult, LPCWSTR szError)
    { return E_NOTIMPL; }

    STDMETHOD(GetBindInfo)(DWORD __RPC_FAR *grfBINDF, BINDINFO __RPC_FAR *pbindinfo)
    { return E_NOTIMPL; }

    STDMETHOD(OnDataAvailable)(DWORD grfBSCF, DWORD dwSize, FORMATETC __RPC_FAR *pformatetc, STGMEDIUM __RPC_FAR *pstgmed)
    { return E_NOTIMPL; }

    STDMETHOD(OnObjectAvailable)(REFIID riid, IUnknown __RPC_FAR *punk)
    { return E_NOTIMPL; }

    STDMETHOD_(ULONG,AddRef)()
    { return 0; }

    STDMETHOD_(ULONG,Release)()
    { return 0; }

    STDMETHOD(QueryInterface)(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
    { return E_NOTIMPL; }

protected:
	CDownload* currentSession;
};

// Main download thread
void DownloadinputFile(void* theData)  
{	
	// Copy class data over
	CDownload* currentSession = (CDownload*)theData;

	// Declare a callback class
    CDownloadProgress callbackData(currentSession);

	// Begin the actual download
    HRESULT didDownload = URLDownloadToFile(NULL, currentSession->m_RemoteURL, currentSession->m_LocalPath, 0, &callbackData);

	// What happened?
	if (SUCCEEDED(didDownload))
		// Download complete, call function
		currentSession->DownloadComplete();
	else
		// Download failed, call function
		currentSession->DownloadFailed();

    return;  
}

// Begin the download
void CDownload::BeginDownload(LPCSTR remoteinputFile, LPCSTR localinputFile)
{
	// Check if we've got the necessary parameters
	if ((remoteinputFile != NULL) && (localinputFile != NULL))
	{
		// Store the information
		m_RemoteURL = remoteinputFile;
		m_LocalPath = localinputFile;

		// Begin the download
		_beginthread(DownloadinputFile, 0, (void*)this);
	}

	// Else we don't, throw an error
	else
	{
		DownloadError();
	}
}

// Cancel the current download
void CDownload::CancelDownload()
{
	m_DoCancel = true;
}

// Get the local inputFile
CString CDownload::GetLocalFile()
{
	return m_LocalPath;
}

// Get the remote URL
CString CDownload::GetRemoteURL()
{
	return m_RemoteURL;
}

// Get the total bytes of the download
int CDownload::GetTotalBytes()
{
	return m_TotalBytes;
}

// Get the number of bytes downloaded
int CDownload::GetTransferredBytes()
{
	return m_TransferredBytes;
}

///////////////////////////////////////////////////////////////
// CClipboard class ///////////////////////////////////////////
///////////////////////////////////////////////////////////////

HBITMAP CClipboard::GetClipboardBitmap()
{
	HBITMAP returnBitmap;

	// Open the clipboard
	if (OpenClipboard(NULL))
	{
		returnBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);

		// Close clipboard
		CloseClipboard();
	}

	return returnBitmap;
}

CString CClipboard::GetClipboardText()
{
	char* textBuffer = NULL;
	CString returnString;

	// Open the clipboard
	if (OpenClipboard(NULL)) 
	{
		// Lock the data on the clipboard into a buffer
		HANDLE clipboardData = GetClipboardData(CF_TEXT);
		char* textBuffer = (char*)GlobalLock(clipboardData);

		// Transfer the buffer into a CString
		returnString = textBuffer;

		// Unlock the data
		GlobalUnlock(clipboardData);

		// Close clipboard
		CloseClipboard();
	}

	return returnString;
}

void CClipboard::SetClipboardBitmap(HBITMAP toPut)
{
	// Open the clipboard
	if (OpenClipboard(NULL))
	{
		// Empty the clipboard
		EmptyClipboard();

		// Put image on clipboard
		SetClipboardData(CF_BITMAP, toPut);

		// Close clipboard
		CloseClipboard();
	}
}

void CClipboard::SetClipboardText(LPCSTR toPut)
{
	// Open the clipboard
	if(OpenClipboard(NULL))
	{
		// Get a global buffer
		HGLOBAL clipboardBuffer;

		// Textual buffer
		char* textBuffer;

		// Empty the clipboard
		EmptyClipboard();

		// Allocate enough space for the text
		clipboardBuffer = GlobalAlloc(GMEM_DDESHARE, strlen(toPut) + 1);
		textBuffer = (char*)GlobalLock(clipboardBuffer);
		strcpy(textBuffer, toPut);

		// Unlock buffer
		GlobalUnlock(clipboardBuffer);

		// Put text on clipboard
		SetClipboardData(CF_TEXT, clipboardBuffer);

		// Close clipboard
		CloseClipboard();
	}
}

///////////////////////////////////////////////////////////////
// CinputFileTime class ////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// Set the current inputFile (retrieves times)
void CFileTimeN::SetFile(LPCSTR inFile)
{
	// Buffers for times
	FILETIME inputFileModified, inputFileCreated, inputFileAccessed;
	SYSTEMTIME systemFileModified, systemFileCreated, systemFileAccessed;

	// Misc buffers
	OFSTRUCT openFileStruct;

	// Retrieve time
	HFILE inputFile = OpenFile(inFile, &openFileStruct, OF_READ);
	GetFileTime((HANDLE)inputFile, &inputFileCreated, &inputFileAccessed, &inputFileModified);
	_lclose(inputFile);

	// Convert times
	FileTimeToSystemTime(&inputFileCreated, &systemFileCreated);
	FileTimeToSystemTime(&inputFileAccessed, &systemFileAccessed);
	FileTimeToSystemTime(&inputFileModified, &systemFileModified);

	CTime temporaryTimeM(systemFileModified);
	m_ModifiedTime = temporaryTimeM;

	CTime temporaryTimeC(systemFileCreated);
	m_CreationTime = temporaryTimeC;

	CTime temporaryTimeA(systemFileAccessed);
	m_AccessTime = temporaryTimeA;
}

// Retrieve access time
CTime CFileTimeN::GetAccessTime()
{
	return m_AccessTime;
}

// Retrieve creation time
CTime CFileTimeN::GetCreationTime()
{
	return m_CreationTime;
}

// Retrieve modified time
CTime CFileTimeN::GetModifiedTime()
{
	return m_ModifiedTime;
}

///////////////////////////////////////////////////////////////
// CCRC32 class ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// Static CRC table
DWORD CCRC32::crcTable[256] =
{
	0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
	0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
	0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
	0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
	0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
	0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
	0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
	0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
	0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
	0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
	0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
	0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
	0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
	0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
	0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
	0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,

	0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
	0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
	0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
	0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
	0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
	0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
	0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
	0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
	0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
	0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
	0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
	0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
	0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
	0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
	0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
	0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,

	0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
	0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
	0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
	0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
	0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
	0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
	0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
	0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
	0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
	0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
	0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
	0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
	0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
	0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
	0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
	0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,

	0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
	0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
	0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
	0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
	0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
	0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
	0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
	0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
	0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
	0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
	0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
	0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
	0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
	0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
	0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
	0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
};

// Calculate CRC32 of a byte. Inline for speed.
inline void CCRC32::CalculateCRC32(const BYTE byte, DWORD &inCRC)
{
	inCRC = ((inCRC) >> 8) ^ crcTable[(byte) ^ ((inCRC) & 0x000000FF)];
}

// Turn the DWORD into a CString
CString CCRC32::FormatCRC32(DWORD inCRC)
{
	CString returnString;
	returnString.Format("%08x", inCRC);
	return returnString;
}

// Hash a string
DWORD CCRC32::HashString(LPCTSTR inString)
{
	DWORD outCRC = 0xFFFFFFFF;

	// Loop the string and calculate the value of each byte
	while(*inString != _T('\0'))
	{
		CalculateCRC32((BYTE)*inString, outCRC);
		inString++;
	}

	outCRC = ~outCRC;

	return outCRC;
}

// Hash a inputFile
DWORD CCRC32::HashFile(LPCTSTR inFile)
{
	// Input inputFile
	DWORD outCRC = 0xFFFFFFFF;

/*	ifstream inputFile;

	DWORD outCRC = 0xFFFFFFFF;

	// Open the inputFile
	inputFile.open(inFile, ios::in | ios::nocreate | ios::binary, filebuf::sh_read);

	// If inputFile's open, loop through chunk by chunk and CRC it
	if(inputFile.is_open())
	{
		// Input buffer
		char inputBuffer[4096];

		// Loop and read counts
		int currentLoop, readCount;
		readCount = inputFile.read(inputBuffer, sizeof(inputBuffer)).gcount();

		while(readCount)
		{
			for(currentLoop = 0; currentLoop < readCount; currentLoop++)
				CalculateCRC32(inputBuffer[currentLoop], outCRC);

			readCount = inputFile.read(inputBuffer, sizeof(inputBuffer)).gcount();
		}

		// Close the inputFile
		inputFile.close();
	}

	outCRC = ~outCRC;*/

	return outCRC;
}

///////////////////////////////////////////////////////////////
// CListCtrlX class ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// Get selected item
int CListCtrlX::GetSelectedItem()
{
	POSITION itemPosition = GetFirstSelectedItemPosition();
	return GetNextSelectedItem(itemPosition);
}