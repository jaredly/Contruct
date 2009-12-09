// Construct.cpp : Defines the class behaviors for the application.
//

// File includes
#include "StdAfx.h"
#include "Construct.h"

#include "MainFrm.h"
#include "ChildFrame.h"
#include "Construct Doc.h"

// Insert New Object
#include "Dialogs\Error Dlg.h"

// Editors
#include "Editors\LayoutEditor.h"
#include "Editors\Event Sheet Editor\Event Sheet Editor.h"

#include "Structure.h"

#include <shlwapi.h>

#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString gInsert;

CMainFrame* pMainWnd;
CMultiDocTemplate* pDocTemplate;
CDLLMap *g_dlls;

// New plugin map
CPluginMap g_Plugins;

Construct *gApp = NULL;
HINSTANCE ghInst;
LPCTSTR g_lpszChildWndClass;
vector<CString> g_Traits;
vector<CString> g_PythonLib;

/////////////////////////////////////////////////////////////////////////////
// Construct

BEGIN_MESSAGE_MAP(Construct, CWinApp)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Construct construction

Construct::Construct()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only Construct object

Construct theApp;

/////////////////////////////////////////////////////////////////////////////
// Construct initialization

BOOL Construct::InitInstance()
{
	CWinApp::InitInstance();

	InitCommonControls();

	ghInst = m_hInstance;
	gApp = this;

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Construct"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register document templates

	pDocTemplate = new CMultiDocTemplate(
		130,
		RUNTIME_CLASS(CConstructDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CLayoutEditor));
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(
		131,
		RUNTIME_CLASS(CConstructDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(EventSheetEditor));
	AddDocTemplate(pDocTemplate);

	// Load plugins
	LoadDLLs();

	// Create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	m_pMainWnd = pMainFrame;

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	pMainWnd = pMainFrame;

	pMainFrame->SetIcon(LoadIcon(MAKEINTRESOURCE(IDR_MAINWNDICON)), true);
	pMainFrame->SetIcon(LoadIcon(MAKEINTRESOURCE(IDR_MAINWNDICON)), false);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo); 

	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing; // Dispatch commands specified on the command line 
	if (!ProcessShellCommand(cmdInfo)) 
		return FALSE; 

	// Load traits
	// Note:  the text files are now unused -Ash
	g_Traits.clear();

	CPath path;
	path.SetToCurrentDirectory();

	CString traits = path.GetFullPath();
	traits += "Traits\\*";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile(traits, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			CString file = FindFileData.cFileName;
			CString ext = file.Right(4);
			ext.MakeLower();

			if (ext == ".txt")
			{
				CString traitName = file.Left(file.GetLength() - 4);
				g_Traits.push_back(traitName);
			}
		}
	}

	////////////////////
	// Create a list of available python libs

	CPath Path;
	Path.SetToCurrentDirectory();

	CString Base;
	Base.Format("%sdata\\Python\\", Path.GetFullPath());
	
	g_PythonLib.clear();
	GetListOfFiles(g_PythonLib, Base, "");

	/////////////////////

	AfxOleInit();

	if(cmdInfo.m_strFileName != "")
		pMainFrame->DoLoad(cmdInfo.m_strFileName);


	m_hDll = NULL;
	m_hDll = ::LoadLibrary(_T("SciLexer.dll"));


	return TRUE;
}

int Construct::ExitInstance ()
{
	UnloadDLLs();

	::FreeLibrary(m_hDll);

    return CWinApp::ExitInstance();
}

// MicroPropertyVector imps
MicroPropertyVector::MicroPropertyVector()
{
	pVec = NULL;
}

MicroPropertyVector::MicroPropertyVector(vector<CPropItem>* p, CPropItem* i)
{
	pVec = p;
	pItem = i;
}

// For properties vector push backs
void MicroPropertyVector::Proc(void* x)
{
	CVirtualPropItem* pCurItem = (CVirtualPropItem*)x;

	// The int wasnt written directly
	pItem->Type = pCurItem->Type;

	pVec->push_back(*pItem);
}

void MicroPropertyVector::Assign(void* s, const char* p)
{
	*(CString*)s = p;
}

// For obtaining ACE data
MicroAceTool::MicroAceTool(ACEList* c, ACEList* a, ACEList* e)
{
	cnds = c;
	acts = a;
	exps = e;
}

void MicroAceTool::EditAce(const char* aceName, const char* aceCategory, const char* aceDisplay, const char* scriptName, short flags, int type, int index, int reserved)
{
	//ace.resourceID = resID;
	ace.aceListName = aceName;
	ace.aceCategory = aceCategory;
	ace.aceDisplayText = aceDisplay;
	ace.scriptName = scriptName;
	ace.retrn = flags;

	// Reserved 255 for common aces
	if(acts->size() == 0)
	{
		ACESEntry2 blank;
		blank.resourceID = 0;
		blank.retrn = 0;
		blank.aceDisplayText = "";
		blank.aceName = "";
		
		acts->assign(255, blank);
		cnds->assign(255, blank);
		exps->assign(255, blank);	
	}

	// CONDITION
	if (type == 0)
	{
		if(index < 0 || index >= cnds->size())
			cnds->push_back(ace);
		else
			(*cnds)[index] = ace;
	}

	// ACTION
	if (type == 1)
	{
		if(index < 0 || index >= acts->size())
			acts->push_back(ace);
		else
			(*acts)[index] = ace;
	}

	// EXPRESSION
	if (type == 2)
	{
		if(index < 0 || index >= exps->size())
			exps->push_back(ace);
		else
			(*exps)[index] = ace;
	}

	ace.params.resize(0);
}

void MicroAceTool::Param(int type, const char* name, const char* desc, const char* init)
{
	PARAM p;
	p.type = type;
	p.name = name;
	p.desc = desc;
	p.initStr = init;
	ace.params.push_back(p);
}

void MicroAceTool::RunAce(ACROUTINE f, const char* p, int type, int flags)
{
}

void Construct::SortCategories(vector<ACESEntry2>& aces, vector<AceCategory>& cats)
{
	vector<ACESEntry2>::iterator i = aces.begin();
	vector<ACESEntry2>::iterator aces_end = aces.end();
	int index = 0;

	// For each ACE
	for ( ; i != aces_end; i++, index++) 
	{
		// Blank ACE
		if (i->aceCategory == "" && i->aceListName == "")
			continue;

		AceCategory aceCat(i->aceCategory);
		vector<AceCategory>::iterator c = push_back_unique_find(cats, aceCat);

		AceListEntry ale;
		ale.listname = i->aceListName;
		ale.aceID = index;
		ale.pAce = &(*i);

		c->aces.push_back(ale);
	}

	// Iterate categories and sort ACEs
	vector<AceCategory>::iterator c = cats.begin();
	vector<AceCategory>::iterator cats_end = cats.end();

	for ( ; c != cats_end; c++) {
		sort(c->aces.begin(), c->aces.end());
	}
}

void Construct::LoadDLLs()
{
	char* curDir;
	curDir = new char[MAX_PATH+9];
	GetModuleFileName(NULL, curDir, MAX_PATH+9);
	PathRemoveFileSpec(curDir);
	strcat(curDir, "\\Plugins\\*");
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char DirSpec[MAX_PATH];  // directory specification
	DWORD dwError;
	strncpy (DirSpec, curDir, strlen(curDir)+1);
	strncat (DirSpec, "\\*", 3);
	hFind = FindFirstFile(curDir, &FindFileData);

	DWORD id = 0;

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		CErrorDlg error;
		error.Error("Plugins directory missing", "The plugins directory has been moved.\nConstruct cannot initialise your plugins.");     
	} 
	else
	{
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			CString file = FindFileData.cFileName;
			CString ext = file.Right(4);
			ext.MakeLower();
			if (!strcmp(ext, ".csx")) 
			{
				CString totalfile = curDir;
				totalfile = totalfile.Left(totalfile.GetLength()-1);
				totalfile += file;
				HINSTANCE hInstDll = ::LoadLibrary(totalfile);

				if (hInstDll == NULL) {
					CString msg;
					msg.Format("Error loading %s (%d) - this plugin may not be available!", totalfile, GetLastError());
					
					CErrorDlg Dlg;
					Dlg.Error("Object failed to load", msg);
				}

				else
				{
					OINFO *oInfo = new OINFO;
					oInfo->hInstLib = hInstDll;
					strcpy(oInfo->extFileName, FindFileData.cFileName);
					LoadString(hInstDll, OBJ_NAME, oInfo->extName, 256);

					if (CString(oInfo->extName) == "") continue;

					// Micro ace tool to obtain ACE data
					MicroAceTool mat(&(oInfo->conditions), &(oInfo->actions), &(oInfo->expressions));

					// Call ETDllLoad
					ETDLLLOAD ETDllLoad = (ETDLLLOAD)GetProcAddress(hInstDll, "ETDllLoad");

					if (ETDllLoad == NULL) {
						CString msg;
						msg.Format("Error loading %s (%d) - this plugin may not be available!", totalfile, GetLastError());
					
						CErrorDlg Dlg;
						Dlg.Error("Object failed to load", msg);

						continue;
					}

					// Load the ACE vectors
					ETDllLoad(&mat);

					// Now collate in to the Category vectors
					SortCategories(oInfo->conditions, oInfo->cndCategories);
					SortCategories(oInfo->actions, oInfo->actCategories);
					SortCategories(oInfo->expressions, oInfo->expCategories);

					// Get ETDrawObject routine
					oInfo->ETDrawObject = (ETDRAWOBJECT)GetProcAddress(hInstDll, "ETDrawObject");

					// Get ETInitializeObject routine
					oInfo->ETInitializeObject = (ETINITIALIZEOBJECT)GetProcAddress(hInstDll, "ETInitializeObject");

					// Get ETCreateObject routine
					oInfo->ETCreateObject = (ETCREATEOBJECT)GetProcAddress(hInstDll, "ETCreateObject");

					// Get ETPropertyChanged routine
					oInfo->ETOnPropertyChanged = (ETONPROPERTYCHANGED)GetProcAddress(hInstDll, "ETOnPropertyChanged");

					// Get EDestroyObject routine
					oInfo->ETDestroyObject = (ETDESTROYOBJECT)GetProcAddress(hInstDll, "ETDestroyObject");

					// Get ETPutObject routine
					oInfo->ETPutObject = (ETPUTOBJECT)GetProcAddress(hInstDll, "ETPutObject");

					// Get ETRemoveObject routine
					oInfo->ETRemoveObject = (ETREMOVEOBJECT)GetProcAddress(hInstDll, "ETRemoveObject");

					// Get ETSerialize routine
					oInfo->ETSerialize = (ETSERIALIZE)GetProcAddress(hInstDll, "ETSerialize");
					
					// Get ETAddDefaultEvent routine
					oInfo->ETAddDefaultEvent = (ETADDDEFAULTEVENT)GetProcAddress(hInstDll, "ETAddDefaultEvent");

					// Get ETOnMessage routine
					oInfo->ETOnMessage = (ETONMESSAGE)GetProcAddress(hInstDll, "ETOnMessage");
	
					// Get ETOnDoubleClick routine
					oInfo->ETOnNotify = (ETONNOTIFY)GetProcAddress(hInstDll, "ETOnNotify");

					// Get ETGetAnimationHandle
					oInfo->ETGetAnimationHandle = (ETGETANIMATION)GetProcAddress(hInstDll, "ETGetAnimationHandle");

					oInfo->hLargeIcon = LoadBitmap(oInfo->hInstLib, MAKEINTRESOURCE(OBJ_ICON));
					oInfo->hSmallIcon = LoadBitmap(oInfo->hInstLib, MAKEINTRESOURCE(OBJ_SICON));

					GetInfo* pfunc;
					oInfo->ideFlags = 0;
					oInfo->MinimumVersion = 1;

					pfunc = reinterpret_cast<GetInfo*>(GetProcAddress(hInstDll,"GetInfo"));
					if (pfunc) 
					{
						(*pfunc)(oInfo);

						// Check version requirements, currently 1
						if (oInfo->MinimumVersion > 1) continue;

						CPropItem item;

						// We use a Virtual Property Item to abstract writing to CStrings
						// by taking their addresses.
						CVirtualPropItem vitem;
						vitem.Type = 0;
						vitem.Description = &(item.Description);
						vitem.Label = &(item.Label);
						vitem.Text = &(item.Text);

						// Micro vector tool abstracts the process of adding an element to a vector from the SDK
						MicroPropertyVector mpvt(&(oInfo->Properties), &item);

						// Call - davo : why is this called on loadup ?
						//      - ash  : mode 0 obtains the property table definition.  modes 1 and 2 access properties
						oInfo->ETOnPropertyChanged(NULL, NULL, 0, 
												   &vitem, NULL, &mpvt);

						oInfo->oID = id;
						m_dlls[oInfo->oID] = oInfo;

						// Set data for CPlugin
						CPlugin Plugin;

						// Icons
						Plugin.m_LargeIcon = oInfo->hLargeIcon;
						Plugin.m_SmallIcon = oInfo->hSmallIcon;
						Plugin.m_FullPath = totalfile;
						Plugin.m_FileName = file;
						Plugin.m_Flags = oInfo->ideFlags;
						if ((oInfo->ideFlags & OF_MOVEMENTPLUGIN)) Plugin.m_bMovement = true;
						Plugin.m_pInfo = oInfo; // Tweak to allow more

						// Texts
						char LoadText[1024];
						LoadString(oInfo->hInstLib, OBJ_NAME, LoadText, 1023);
						Plugin.m_Name.Format("%s", LoadText);

						LoadString(oInfo->hInstLib, OBJ_AUTHOR, LoadText, 1023);
						Plugin.m_Author.Format("%s", LoadText);

						LoadString(oInfo->hInstLib, OBJ_VERSION, LoadText, 1023);
						Plugin.m_Version.Format("%s", LoadText);

						LoadString(oInfo->hInstLib, OBJ_DESCRIPTION, LoadText, 1023);
						Plugin.m_Description.Format("%s", LoadText);

						LoadString(oInfo->hInstLib, OBJ_CATEGORY, LoadText, 1023);
						Plugin.m_Category.Format("%s", LoadText);

						LoadString(oInfo->hInstLib, OBJ_WEB, LoadText, 1023);
						Plugin.m_WebLink.Format("%s", LoadText);

						g_Plugins[id] = Plugin;

						// Advance id
						id++;
					}
				}
			}

			ext.ReleaseBuffer();
		}    

		dwError = GetLastError();
		FindClose(hFind);
	}

	delete [] curDir;
	g_dlls = &m_dlls;
}

void Construct::UnloadDLLs()
{
	OINFO *oInfo;
	POSITION pos = m_dlls.GetStartPosition();
	DWORD nKey;
	while (pos != NULL) {
		m_dlls.GetNextAssoc( pos, nKey, oInfo );
		BOOL freed = ::FreeLibrary(oInfo->hInstLib);
		m_dlls.RemoveKey(oInfo->oID);
		delete oInfo;
		oInfo = NULL;
	}
}

// Messagebox wrapper (AG)
DWORD ExtMessageBox(HWND hWnd, CString msg, CString caption, DWORD flags)
{
	CExtMsgBox mb;
	mb.MessageSet(msg);
	mb.CaptionSet(caption);

	// Convert Win32 flags to Prof-UIS flags
	UINT mbStyle = 0;
	if (flags & MB_OK)					mbStyle |= __EXT_MB_OK;
	if (flags & MB_OKCANCEL)			mbStyle |= __EXT_MB_OKCANCEL;
	if (flags & MB_ABORTRETRYIGNORE)	mbStyle |= __EXT_MB_ABORTRETRYIGNORE;
	if (flags & MB_YESNOCANCEL)			mbStyle |= __EXT_MB_YESNOCANCEL;
	if (flags & MB_YESNO)				mbStyle |= __EXT_MB_YESNO;
	if (flags & MB_RETRYCANCEL)			mbStyle |= __EXT_MB_RETRYCANCEL;
	if (flags & MB_CANCELTRYCONTINUE)	mbStyle |= __EXT_MB_CANCELTRYCONTINUE;

	if (flags & MB_ICONHAND)			mbStyle |= __EXT_MB_ICONHAND;
	if (flags & MB_ICONQUESTION)		mbStyle |= __EXT_MB_ICONQUESTION;
	if (flags & MB_ICONEXCLAMATION)		mbStyle |= __EXT_MB_ICONEXCLAMATION;
	if (flags & MB_ICONASTERISK)		mbStyle |= __EXT_MB_ICONASTERISK;

	int ret = mb.DoMsgBox(hWnd, msg, caption, mbStyle);

	return ret;
}