/*
	Construct - DirectX 9 Game and Application creation
    Copyright (C) 2007 Scirra

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

/*
	Prof-UIS professional user interface library

	Used with kind permission of FOSS Software, Inc.

	www.prof-uis.com

	The Prof-UIS library in this source code is only to be used with this project.
	You MAY NOT use it for any other purpose.  If you wish to do so, please purchase
	an appropriate license from the above address.
*/

// File includes
#include "StdAfx.h"
#include "Construct.h"
#include "Structure\Export\Export.h"
#include "Dialogs\ExchangeDlg.h"
#include "MainFrm.h"
#include "ChildFrame.h"
#include "Utilities\CGFilTyp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CColorizedThemeOffice2003, CExtPaintManagerOffice2003, VERSIONABLE_SCHEMA|1 );

// About dialog
#include "Dialogs\AboutDlg.h"

// message_bar
#include "Dialogs\Error Dlg.h"

// First run
#include "Dialogs\FirstStartDlg.h"

// Language
CString g_Language;
CMainFrame* g_MainFrame;
CLayoutEditor *g_pLayoutView;

#include "UI Elements\Settings\SettingsDlg.h"
#include "UI Elements\Settings\AutoSaveOptions.h"

// Python Scripting
#include "Dialogs\ScriptDlg.h"

CIni* pINI;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_NEW_TEMPLATE, OnFileTemplate)
	ON_COMMAND(ID_VIEW_LAYERBAR, OnLayerBar)
	ON_COMMAND(ID_VIEW_SIDEBAR, OnProjectBar)
	ON_COMMAND(ID_VIEW_PROPERTIESBAR, OnPropertiesBar)
	ON_COMMAND(ID_VIEW_ANIMBAR, OnAnimationBar)
	ON_COMMAND(ID_PREFERENCES, OnPreferences)
	ON_COMMAND(ID_APP_ABOUT, OnAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_VIEWVRAM, OnViewVRAM)
	ON_COMMAND(ID_FILE_NEWAPP, OnFileNewApplication)
	ON_COMMAND(ID_HELP, OnContextHelp)
	ON_COMMAND(ID_APPEARANCE_XP, OnXP)
	ON_COMMAND(ID_APPEARANCE_2003, On2003)
	ON_COMMAND(ID_APPEARANCE_2005, On2005)
	ON_COMMAND(ID_ISCLOSING, OnClosing)
	ON_COMMAND(ID_EXCHANGE, OnExchange)
	ON_COMMAND(ID_WEB_SCIRRACOM, OnWebScirra)
	ON_COMMAND(ID_WEB_FORUMS, OnWebForums)
	ON_COMMAND(ID_WEB_REPORTBUG, OnWebReportBug)
	ON_COMMAND(ID_WEB_HELP, OnWebHelp)
	ON_COMMAND(ID_WEB_UPDATE, OnWebUpdate)
	ON_REGISTERED_MESSAGE( CExtControlBar::g_nMsgConstructPopupMenu, OnConstructPopupMenuCB )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CDragObjects *g_dragInfo;

// The allocator
edittime_allocator g_allocator;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

PropertiesBar* g_PropertyBar;

CString gInsert;
int gInsertAuto = 0;
CLayerBar* g_pLayerBar;

WIN32_FIND_DATA wfd;
HANDLE hFind;

LRESULT CMainFrame::OnConstructPopupMenuCB(WPARAM wParam, LPARAM lParam)
{
	return 1;
}

void CMainFrame::ObjectTypesChanged()
{
	InitialUpdateAllEditors();
}

void CMainFrame::OnClosing()
{
	OnClose();
}

void CMainFrame::OnExchange()
{
	CExchangeDlg Dlg;
	Dlg.DoModal();
}

void CMainFrame::OnWebScirra()
{
	ShellExecute(NULL, "open", "http://www.scirra.com/", NULL, NULL, 0);
}

void CMainFrame::OnWebForums()
{
	ShellExecute(NULL, "open", "http://scirra.com/phpBB2/", NULL, NULL, 0);
}

void CMainFrame::OnWebReportBug()
{
	ShellExecute(NULL, "open", "http://scirra.com/bugs.php", NULL, NULL, 0);
}

void CMainFrame::OnWebHelp()
{
	ShellExecute(NULL, "open", "http://scirra.com/learn.php", NULL, NULL, 0);
}

void CMainFrame::OnWebUpdate()
{
	DoUpdateCheck(false);
}

void CMainFrame::InitialUpdateAllEditors()
{
	CWnd* clientWindow = GetActiveFrame();//GetWindow(GW_CHILD);
	while (clientWindow != NULL)
	{
		CChildFrame* frame = dynamic_cast<CChildFrame*>(clientWindow);
		if(frame)
			frame->InitialUpdateAllEditors();
		clientWindow = clientWindow->GetWindow(GW_HWNDNEXT);
	}
}

bool FileExists(LPCSTR Name)
{
	return GetFileAttributes(Name) != 0xFFFFFFFF;
}

//////////////////////////////////////////////////////
// PROF-UIS CODE
//////////////////////////////////////////////////////
CMainFrame::CMainFrame()
{
	BOOL toSend = true;

	bIsMinimized = false;

	CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	if( ! g_PaintManager.PaintManagerStateLoad(
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			)
		)

	g_PaintManager.InstallPaintManager(new CExtPaintManagerStudio2008);

	CExtPopupMenuWnd::g_bMenuExpanding = false;
	CExtPopupMenuWnd::g_bMenuHighlightRarely = false;

	// Window placement persistence
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;

	g_PropertyBar = &m_PropertiesBar;
	g_pLayerBar = &m_LayerBar;
	g_dragInfo = new CDragObjects();

	newAppNumber = 0;
	m_pPApp = NULL;
	has_been_closed = false;
}

CMainFrame::~CMainFrame()
{
	CApplication *app;
	POSITION pos = m_apps.GetHeadPosition();
	delete g_dragInfo;

	while (pos != NULL) 
	{
		// iterate through objects to choose one to select
		app = m_apps.GetNext(pos);
		delete app;
	}
}

void CMainFrame::RecalcLayout(BOOL bNotify)
{
	CExtNCW < CMDIFrameWnd > :: RecalcLayout( bNotify );
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CExtNCW < CMDIFrameWnd > :: PreCreateWindow( cs ) )
		return FALSE;

	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_LAYOUT_RTL);
	cs.lpszClass =
		::AfxRegisterWndClass(
			CS_BYTEALIGNCLIENT,
			::LoadCursor( NULL, IDC_ARROW ),
			NULL,
			NULL
			);
	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;

	return TRUE;
}

#define ID_RIBBON_MINIMIZE_RESTORE      5000

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if( nID == ID_RIBBON_MINIMIZE_RESTORE )
	{
		if( nCode == CN_UPDATE_COMMAND_UI )
		{
			((CCmdUI *)pExtra)->Enable();
			((CCmdUI *)pExtra)->SetCheck( m_Ribbon.RibbonPage_ExpandedModeGet() );
		}
		else if( pExtra == NULL )
		{
			m_Ribbon.RibbonPage_ExpandedModeSet( ! m_Ribbon.RibbonPage_ExpandedModeGet(), true );
		}
		return TRUE;
	}
	if( nID == ID_RTL )
	{
		if( nCode == CN_UPDATE_COMMAND_UI )
		{
			((CCmdUI *)pExtra)->Enable( g_PaintManager.m_bIsWinNT );
			if( g_PaintManager.m_bIsWinNT )
				((CCmdUI *)pExtra)->SetCheck(
					( (GetExStyle()&WS_EX_LAYOUT_RTL) != 0 ) ? 1 : 0
					);
		}
		else if( pExtra == NULL && g_PaintManager.m_bIsWinNT )
		{
			CExtNcFrameImpl::m_bNcFrameImpl_IsEnabled = false;
			if( (GetExStyle()&WS_EX_LAYOUT_RTL) != 0 )
				ModifyStyleEx( WS_EX_LAYOUT_RTL, 0, SWP_FRAMECHANGED );
			else
				ModifyStyleEx( 0, WS_EX_LAYOUT_RTL, SWP_FRAMECHANGED );
			CExtNcFrameImpl::m_bNcFrameImpl_IsEnabled = true;
			_AdjustRTL();
		}
		return TRUE;
	}

	CExtNCW < CMDIFrameWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
	return TRUE;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( m_Ribbon.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	return CExtNCW < CMDIFrameWnd > :: PreTranslateMessage(pMsg);
}

BOOL CMainFrame::DestroyWindow() 
{
	CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	m_Ribbon.CustomizeStateSave(pApp->m_pszRegistryKey,	pApp->m_pszProfileName,	pApp->m_pszProfileName);
	CExtControlBar::ProfileBarStateSave(this, pApp->m_pszRegistryKey, pApp->m_pszProfileName, pApp->m_pszProfileName);
	g_PaintManager.PaintManagerStateSave(pApp->m_pszRegistryKey, pApp->m_pszProfileName, pApp->m_pszProfileName);
	g_CmdManager->ProfileWndRemove(GetSafeHwnd());
	
	return CExtNCW < CMDIFrameWnd > :: DestroyWindow();
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	bool bDwmMode = NcFrameImpl_IsDwmCaptionReplacement();
	// window placement persistence
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		if( bDwmMode )
			NcFrameImpl_NcLock( true );
		SetWindowPlacement( &m_dataFrameWP );
		CExtNCW < CMDIFrameWnd > :: ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		if( bDwmMode )
		{
			NcFrameImpl_NcLock( false );
			NcFrameImpl_RecalcNcFrame();
		}
		return;
	}
	CExtNCW < CMDIFrameWnd > :: ActivateFrame( nCmdShow );
	if( bDwmMode )
		NcFrameImpl_RecalcNcFrame();
}

void CMainFrame::_AdjustRTL()
{
	ASSERT_VALID( this );
	if( (GetExStyle()&WS_EX_LAYOUT_RTL) != 0 )
	{
		m_Ribbon.ModifyStyleEx( 0, WS_EX_LAYOUTRTL, 0 );

		g_ResourceManager->SetLangLayout( LAYOUT_RTL );
	}
	else
	{
		m_Ribbon.ModifyStyleEx( WS_EX_LAYOUTRTL, 0, 0 );

		g_ResourceManager->SetLangLayout( LAYOUT_LTR );
	}
	m_Ribbon.Ribbon_UpdateLayout( true );
	CExtNcFrameImpl::NcFrameImpl_RecalcNcFrame();
	CExtNcFrameImpl::NcFrameImpl_SetupRgn();
	if( IsWindowVisible() )
		RedrawWindow( NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN );
}


//////////////////////////////////////////////////////
// END PROF-UIS CODE
//////////////////////////////////////////////////////

void CMainFrame::OnContextHelp()
{
	CPath path;
	path.SetToCurrentDirectory();
	CString Help = path.GetFullPath();

	Help += "Construct.chm";

	ShellExecute(NULL, "open", Help, NULL, path.GetFullPath(), SW_SHOWMAXIMIZED);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// Did the MDI Client create successfully?
	if (CExtNCW < CMDIFrameWnd >::OnCreate(lpCreateStruct) == -1)
		return -1;	

	CPath path;
	CWinApp * pApp = ::AfxGetApp();
	g_CmdManager->ProfileSetup(pApp->m_pszProfileName, GetSafeHwnd());

	LoadLanguage("English (UK)");

	gInsert = "";

	path.SetToCurrentDirectory();	

	CGCFileTypeAccess FileType;
	CString csTempText;

	FileType.SetExtension("cap");

	csTempText = path.GetFullPath();
	csTempText += "Construct.exe";
	csTempText += " \"%1\"";
	FileType.SetShellOpenCommand(csTempText);
	FileType.SetDocumentShellOpenCommand(csTempText);

	FileType.SetDocumentClassName("Construct.Application");

	csTempText = path.GetFullPath();
	csTempText += "capico.ico";
	csTempText += ",0";
	FileType.SetDocumentDefaultIcon(csTempText);

	// set the necessary registry entries	
	FileType.RegSetAllInfo();

	// INI
	TCHAR szPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szPath);
	CString IP;
	IP.Format("%s\\Scirra\\", szPath);
	CreateDirectory(IP, NULL);
	IP += "Construct.ini";
	m_INIPath=IP;
	m_INI.SetPathName(m_INIPath);

	// First run wizard?
	if (m_INI.GetInt("General", "Run", 0) == 0)
	{
		CString language = "English (UK)";

		WritePrivateProfileString("General", "Run", "1", m_INIPath);
		WritePrivateProfileString("General", "Language", language, m_INIPath);
		WritePrivateProfileString("General", "ShowMaximized", "Y", m_INIPath);
	}

	if (!m_INI.IsSectionExist("RecentObjects")) {
		m_INI.WriteInt("RecentObjects", "MouseKeyb.csx", 2);
		m_INI.WriteInt("RecentObjects", "Sprite.csx", 2);
		m_INI.WriteInt("RecentObjects", "TileBackground.csx", 2);
		m_INI.WriteInt("RecentObjects", "Box.csx", 2);
		m_INI.WriteInt("RecentObjects", "DirectSound.csx", 2);
		m_INI.WriteInt("RecentObjects", "FrameView.csx", 2);
		m_INI.WriteInt("RecentObjects", "Image.csx", 2);
		m_INI.WriteInt("RecentObjects", "Text.csx", 2);
	}
	
	if (!m_INI.IsSectionExist("Recent")) {
		m_INI.WriteString("Recent", "1", "None");
		m_INI.WriteString("Recent", "2", "None");
		m_INI.WriteString("Recent", "3", "None");
		m_INI.WriteString("Recent", "4", "None");
		m_INI.WriteString("Recent", "5", "None");
		m_INI.WriteString("Recent", "6", "None");
		m_INI.WriteString("Recent", "7", "None");
		m_INI.WriteString("Recent", "8", "None");
		m_INI.WriteString("Recent", "9", "None");
		m_INI.WriteString("Recent", "10", "None");
	}

	if (!m_INI.IsSectionExist("RecentObjectsApp")) {
		m_INI.WriteInt("RecentObjectsApp", "Button.csx", 2);
		m_INI.WriteInt("RecentObjectsApp", "ComboBox.csx", 2);
		m_INI.WriteInt("RecentObjectsApp", "Edit.csx", 2);
		m_INI.WriteInt("RecentObjectsApp", "ListBox.csx", 2);
		m_INI.WriteInt("RecentObjectsApp", "Array.csx", 2);
		m_INI.WriteInt("RecentObjectsApp", "FrameView.csx", 2);
		m_INI.WriteInt("RecentObjectsApp", "Image.csx", 2);
		m_INI.WriteInt("RecentObjectsApp", "File.csx", 2);
	}

	if (!m_INI.IsSectionExist("ImageEditor")) 
	{
		m_INI.WriteInt("ImageEditor", "Tool", ID_BRUSH);

		m_INI.WriteDouble("ImageEditor", "Hardness", 1);
		m_INI.WriteDouble("ImageEditor", "Angle", 0);
		m_INI.WriteDouble("ImageEditor", "Thickness", 100);
		m_INI.WriteDouble("ImageEditor", "Size",1);
		m_INI.WriteDouble("ImageEditor", "Step",1);
		m_INI.WriteDouble("ImageEditor", "LineThickness",1);
		m_INI.WriteBool("ImageEditor", "Smooth", 0);
		m_INI.WriteDouble("ImageEditor", "Opacity", 255);

		m_INI.WriteInt("ImageEditor", "Colour1", RGB(255,0,0));
		m_INI.WriteInt("ImageEditor", "Colour2", RGB(0,0,255));
		m_INI.WriteInt("ImageEditor", "Colour1Alpha", 255);
		m_INI.WriteInt("ImageEditor", "Colour2Alpha", 255);
	}

	g_Language = m_INI.GetString("General", "Language", "English (UK)");
	g_Language += ".txt";

	m_Ribbon.Create(NULL, this);
	m_Ribbon.Init();
	m_Ribbon.Ribbon_PageSelectionSet(0, true);

	CProfStudioPropertyGridCtrl& m_PGC = m_PropertiesBar.m_Grid;
	m_PropertiesBar.m_pStore = &(m_PGC.m_PS);
	m_PropertiesBar.m_pGrid = &m_PGC;

	// MDI tabs
	tabs.Create(this, CRect(0,0,0,0), UINT(IDC_STATIC), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, __ETWS_MDI_DEFAULT | __ETWS_HIDE_ICONS | __ETWS_ITEM_DRAGGING);
	tabs.ModifyTabWndStyle( 0, __ETWS_SHOW_BTN_TAB_LIST|__ETWS_ENABLED_BTN_TAB_LIST );

	// Bars
	project_bar.Create(BAR_PROJECT, this, 123);
	m_PropertiesBar.Create(BAR_PROPERTIES, this, 124);
	m_LayerBar.Create(BAR_LAYERS, this, 125);


	animator_parent.Create(BAR_ANIMATOR, this, 127);
	animator.Create(IDD_ANIMATORBAR, &animator_parent);
	animator.ShowWindow(SW_SHOW);

	UpdateTheme();

	///////////////////
	// SETUP DOCKING //
	///////////////////
	project_bar.EnableDocking(CBRS_ALIGN_ANY);
	m_PropertiesBar.EnableDocking(CBRS_ALIGN_ANY);
	m_LayerBar.EnableDocking(CBRS_ALIGN_ANY);
	animator_parent.EnableDocking(CBRS_ALIGN_ANY);

	g_MainFrame = this;

	CExtControlBar::FrameEnableDocking(this);       // Allow docking
	CExtControlBar::FrameInjectAutoHideAreas(this);	// Allow pinning

	m_wndStatusBar.Create(this);
	m_wndStatusBar.SetIndicators(indicators,  sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.AddPane(1,1);
	m_wndStatusBar.SetPaneWidth(1, 50);
	m_wndStatusBar.AddPane(2,2);
	m_wndStatusBar.SetPaneWidth(2, 100);

	// Load settings
	m_Ribbon.CustomizeStateLoad(pApp->m_pszRegistryKey,	pApp->m_pszProfileName,	pApp->m_pszProfileName);

	if(!CExtControlBar::ProfileBarStateLoad(this,pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName,
			&m_dataFrameWP))	
	{
		m_PropertiesBar.DockControlBar(AFX_IDW_DOCKBAR_LEFT, true);
		m_PropertiesBar.SetInitDesiredSizeVertical(CSize(210, 400));

		// Bar sizes
		m_LayerBar.SetInitDesiredSizeVertical(CSize(210, 400));
		project_bar.SetInitDesiredSizeVertical(CSize(210, 400));
		animator_parent.SetInitDesiredSizeVertical(CSize(210, 400));

		// Initial docking settings
		project_bar.DockControlBarInnerOuter(AFX_IDW_DOCKBAR_RIGHT, true);
		project_bar.DockControlBarIntoTabbedContainer(&m_LayerBar, 1, NULL, true);
		project_bar.DockControlBarIntoTabbedContainer(&animator_parent, 1, NULL, true);
		project_bar.SetInitDesiredSizeVertical(CSize(210, 200));	
	}

	// Docking markers
	CExtControlBar::g_eResizablePanelDockingType = CExtControlBar::__RESIZABLE_DOCKING_TYPE_STUDIO_2005;

	// Hide the property grid combo
	CExtPropertyGridComboBoxBar* pWnd = STATIC_DOWNCAST(CExtPropertyGridComboBoxBar, m_PGC.GetChildByRTC(RUNTIME_CLASS(CExtPropertyGridComboBoxBar)));
	pWnd->ShowWindow(SW_HIDE); 

	// Set tip bar height
	CExtPropertyGridTipBar* pTipBar = STATIC_DOWNCAST(CExtPropertyGridTipBar, m_PGC.GetChildByRTC(RUNTIME_CLASS(CExtPropertyGridTipBar)));
	if(pTipBar != NULL)
		pTipBar->HeightMinSet(55);

	m_PGC.Invalidate();
	m_PropertiesBar.RedrawWindow();

	SetTimer(500, 500, NULL);
	ActivateFrame(0);

	// Auto save
	if (m_INI.GetInt("AutoSave", "On", 0) == 1)
	{
		m_AutoSaveTime = m_INI.GetInt("AutoSave", "Time", 0);
		m_bAutoSave = true;

		if (m_INI.GetInt("AutoSave", "On", 0) == 1)
			m_bAutoBackup = true;
		else
			m_bAutoBackup = false;

		KillTimer(999);
		SetTimer(999, (m_AutoSaveTime * 60) * 1000, NULL);
	}

	else 
	{
		m_bAutoSave = false;
		m_bAutoBackup = false;
		m_AutoSaveTime = 0;
		KillTimer(999);
	}

	// Disable menu fading
	CExtPopupMenuWnd::g_nDefaultFadeOutAnimationStepCount = 0;
	CExtPopupMenuWnd::g_nDefaultFadeOutAnimationEllapse = 0;


	/*
	CXAudio2SetupParameters setup;

	setup.cacheMode = CXAudio2::CACHE_OFF;

	// initialise audio
	audio.Init(setup);
	*/


	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

extern CString g_LanguageID;

void CMainFrame::OnPreferences() 
{
	pINI = &m_INI;

	CSettingsDialog Dlg;

	CAutoSave* pAutoSave = (CAutoSave*)Dlg.AddPage(RUNTIME_CLASS(CAutoSave),
                _T("Auto save"), IDD_PROPERTY_PAGE_2, 
                _T("Auto save settings"));

	pAutoSave->m_AutoSaveTime = m_INI.GetInt("AutoSave", "Time", 0);
	
	Dlg.SetTitle("Construct Preferences");
	Dlg.SetLogoText("Construct");    

	int oldTheme = m_INI.GetInt("General", "Theme", 4);

	if (Dlg.DoModal() == IDOK)
	{
		if (m_INI.GetInt("General", "Theme", 4) != oldTheme)
			UpdateTheme();

		// Auto save preferences
		if (m_INI.GetInt("AutoSave", "On", 0) == 1) 
		{
			m_bAutoSave = true;

			if (m_INI.GetInt("AutoBackup", "On", 0) == 1) 
				m_bAutoBackup = true;
			else
				m_bAutoBackup = false;

			SetTimer(999, (m_AutoSaveTime * 60) * 1000, NULL);
		}

		else
		{
			m_bAutoSave = false;
			m_bAutoBackup = false;

			KillTimer(999);
		}
	}
}

// Show about dialog
void CMainFrame::OnAbout() 
{
	CAboutDlg about;
	about.DoModal();
}

// SHOW/HIDE LAYER BAR
void CMainFrame::OnLayerBar() 
{
	BOOL bShow = m_LayerBar.IsVisible();
	ShowControlBar(&m_LayerBar, !bShow, FALSE);
}

// SHOW/HIDE PROJECT BAR
void CMainFrame::OnProjectBar() 
{
	BOOL bShow = project_bar.IsVisible();
	ShowControlBar(&project_bar, !bShow, FALSE);
}

// SHOW/HIDE PROPERTIES BAR
void CMainFrame::OnPropertiesBar() 
{
	BOOL bShow = m_PropertiesBar.IsVisible();
	ShowControlBar(&m_PropertiesBar, !bShow, FALSE);
}

// SHOW/HIDE ANIMATION BAR
void CMainFrame::OnAnimationBar() 
{
	BOOL bShow = animator_parent.IsVisible();
	ShowControlBar(&animator_parent, !bShow, FALSE);
}

void CMainFrame::OnFileTemplate() 
{
	TemplateDialog dlg;
	dlg.DoModal();
}

void CMainFrame::OnViewVRAM()
{
	if (m_pPApp == NULL)
		return;

	const float megabyte = 1024 * 1024;
	float targetMb = (float)CD3DDisplay::vramtargetbytes / megabyte;
	float texMb = (float)CD3DDisplay::vramtexturebytes / megabyte;

	CString msg;
	msg.Format("VRAM usage by Construct:\n\n%.2fmb textures\n%.2fmb targets\n%.2fmb total\n\nApprox %d MB texture memory remaining", 
		texMb, targetMb, targetMb + texMb, m_pPApp->m_display->GetD3DDevice()->GetAvailableTextureMem() / (1024*1024));
	::MessageBox(NULL, msg, "VRAM usage", MB_OK | MB_ICONINFORMATION);
}

#include "Dialogs\UpdateDlg.h"
#include "Utilities\WebGrab.h"

void UpdateCheck(void* mainframe_)
{
	CMainFrame* mainframe = (CMainFrame*)mainframe_;

	mainframe->DoUpdateCheck(true);
}

void CMainFrame::DoUpdateCheck(bool silent)
{
	CWebGrab WG;
	WG.SetTimeOut(2000);

	CString Ver;
	WG.GetFile("http://www.scirra.com/update.php", Ver);
	
	CString VerNumber;
	CString VerMessage;
	AfxExtractSubString(VerNumber, Ver, 0, '|');
	AfxExtractSubString(VerMessage, Ver, 1, '|');

	if (atoi(VerNumber) > CONSTRUCT_VERSION_ID)
	{
		CErrorDlg Dlg;
		CString msg = "There is a new version of Construct available. Visit http://www.scirra.com to download the latest update.\r\n\r\n";
		msg += VerMessage;

		if (Dlg.UpdateMessage("New version available", msg) == IDOK)
			ShellExecute(NULL, "open", "http://www.scirra.com/download.php", NULL, NULL, 0);

		return;
	}
	else if (!silent) {
		::MessageBox(NULL, "Your version of Construct (" CONSTRUCT_VERSION_STRING ") appears to be up to date.", "Check for updates", MB_OK | MB_ICONINFORMATION);
	}

	if (silent) {
		CUpdateDlg Dlg;
		Dlg.DoModal();
	}
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{	
	if (nIDEvent == 999)
	{
		// Auto save
		// Save all open applications
		CApplication *app;
		POSITION pos = m_apps.GetHeadPosition();

		bool bClose = true;
		while (pos != NULL) 
		{
			// iterate through objects to choose one to select
			app = m_apps.GetNext(pos);
			
			// Check save
			if (app->GetModifiedStatus())
			{
				if (app->file_information.file_path == "") continue;

				OnFileSave(app, true);	// Autosave
			}
		}
	}
	
	if (nIDEvent == 500)
	{
		//if(m_Maximize)
			ShowWindow(SW_SHOWMAXIMIZED);
		KillTimer(500);

		m_PropertiesBar.RedrawWindow();

	#ifndef DEBUGBUILD

		if (m_INI.GetInt("General", "UpdateCheck", -1) == -1) {

			int ret = MessageBox("Do you want Construct to automatically check for updates on startup?  This does not send any information"
				" about you or your computer to Scirra.  If you select Yes, you may be alerted by your firewall.", "Check for updates online", MB_ICONQUESTION | MB_YESNO);

			m_INI.WriteInt("General", "UpdateCheck", ret == IDYES);
		}

		if (m_INI.GetInt("General", "UpdateCheck", 0) == 1)
			_beginthread(UpdateCheck, 0, (void*)this);

	#endif
	}
	
	CExtNCW <CMDIFrameWnd >::OnTimer(nIDEvent);
}

void CMainFrame::OnClose() 
{
	// Loop apps check saved
	CApplication *app;
	POSITION pos = m_apps.GetHeadPosition();

	bool bClose = true;
	while (pos != NULL) 
	{
		// iterate through objects to choose one to select
		POSITION oldPos = pos;
		app = m_apps.GetNext(pos);
		
		// Check save
		if (app->CloseSafely()) {
			delete app;
			m_apps.RemoveAt(oldPos);
		}
		else
			bClose = false;
	}

	if (!bClose) return;

	has_been_closed = true;	// stop layout editors crashing trying to access apps deleted above
	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CExtNCW < CMDIFrameWnd >::OnSize(nType, cx, cy);
}

void CMainFrame::UpdateTheme()
{
	// Load theme
	switch (m_INI.GetInt("General", "Theme", 4)) {
		case 0:
			OnXP();
			break;
		case 1:
			OnOfficeXP();
			break;
		case 2:
			On2003();
			break;
		case 3:
			On2005();
			break;
		case 4:
			On2008();
			break;
		case 5:
			OnLunaBlue();
			break;
		case 6:
			OnLunaSilver();
			break;
		case 7:
			OnLunaBlack();
			break;
		default:
			On2008();
			break;
	}
}

void CMainFrame::OnXP() 
{
	g_PaintManager.InstallPaintManager(
	  new CExtPaintManagerXP
	);

	// Write theme to INI
	m_INI.WriteInt("General", "Theme", 0);
}

void CMainFrame::OnOfficeXP() 
{
	g_PaintManager.InstallPaintManager(
	  new CExtPaintManagerNativeXP
	);

	// Write theme to INI
	m_INI.WriteInt("General", "Theme", 1);
}

void CMainFrame::On2003() 
{  
	g_PaintManager.InstallPaintManager(new CExtPaintManagerOffice2003NoThemes);

	// Write theme to INI
	m_INI.WriteInt("General", "Theme", 2);
}

void CMainFrame::On2005() 
{   
	g_PaintManager.InstallPaintManager(new CExtPaintManagerStudio2005);

	// Write theme to INI
	m_INI.WriteInt("General", "Theme", 3);
}

void CMainFrame::On2008() 
{   
	g_PaintManager.InstallPaintManager(new CExtPaintManagerStudio2008);

	// Write theme to INI
	m_INI.WriteInt("General", "Theme", 4);
}

void CMainFrame::OnLunaBlue() 
{  
	g_PaintManager.InstallPaintManager(
      RUNTIME_CLASS( CExtPaintManagerOffice2007_Blue )
	);
	// Write theme to INI
	m_INI.WriteInt("General", "Theme", 5);
}

void CMainFrame::OnLunaSilver() 
{   
	g_PaintManager.InstallPaintManager(
	  RUNTIME_CLASS( CExtPaintManagerOffice2007_Silver )
	);

	// Write theme to INI
	m_INI.WriteInt("General", "Theme", 6);
}

void CMainFrame::OnLunaBlack() 
{  
	g_PaintManager.InstallPaintManager(
	  RUNTIME_CLASS( CExtPaintManagerOffice2007_Black )
	);
	// Write theme to INI
	m_INI.WriteInt("General", "Theme", 7);
}

void CMainFrame::OnPreview()
{
	CExport Export;
	Export.main_frame = this;
	Export.Preview(m_pPApp);
}

void CMainFrame::OnPreviewLayout(CLayout* layout)
{
	//CHECKME - why would layout be NULL here?
	// Parameter now ignored, uses same code as OnDebugLayout
	//if (layout == NULL) return;

	CExport Export;
	Export.main_frame = this;
	//Export.Preview(m_pPApp, layout->identifier);

	CChildFrame* pChildFrame = (CChildFrame*)CWnd::FromHandle(::GetWindow(m_hWndMDIClient, GW_CHILD));

	if (pChildFrame == NULL || pChildFrame->layout == NULL)
		MessageBox("There is no active layout to preview - make sure a layout is open!", "Preview layout");
	else
		Export.Preview(m_pPApp, pChildFrame->layout->identifier);
}

void CMainFrame::OnDebug()
{
	CExport Export;
	Export.main_frame = this;
	Export.PreviewDebug(m_pPApp);
}

void CMainFrame::OnDebugLayout()
{
	CExport Export;
	Export.main_frame = this;
	CChildFrame* pChildFrame = (CChildFrame*)CWnd::FromHandle(::GetWindow(m_hWndMDIClient, GW_CHILD));

	if (pChildFrame == NULL || pChildFrame->layout == NULL)
		MessageBox("There is no active layout to debug - make sure a layout is open!", "Preview layout");
	else
		Export.PreviewDebug(m_pPApp, pChildFrame->layout->identifier);
}

void CMainFrame::OnExportEXE()
{
	CExport Export;
	Export.main_frame = this;
	Export.ExportToEXE(m_pPApp);
}

void CMainFrame::CloseChildWindows(CLayout* pLayout)
{
	HWND ClientWindow;
	ClientWindow = ::GetWindow(m_hWndMDIClient, GW_CHILD);

	// Now find any layouts within this application
	while (ClientWindow != NULL)
	{
		CChildFrame* pWnd = (CChildFrame*)CWnd::FromHandle(ClientWindow);
		if(pWnd->layout == pLayout)
			pWnd->DestroyWindow();

		ClientWindow = :: GetWindow(ClientWindow, GW_HWNDNEXT);
	}

	// Update windows displaying this potentially
	tabs.RedrawWindow();
}

void CMainFrame::CloseChildWindows(EventSheet* pEventSheet)
{
	HWND ClientWindow;
	ClientWindow = ::GetWindow(m_hWndMDIClient, GW_CHILD);

	// Now find any layouts within this application
	while (ClientWindow != NULL)
	{
		CChildFrame* pWnd = (CChildFrame*)CWnd::FromHandle(ClientWindow);
		if(pWnd->event_sheet == pEventSheet)
			pWnd->DestroyWindow();

		ClientWindow = :: GetWindow(ClientWindow, GW_HWNDNEXT);
	}

	// Update windows displaying this potentially
	tabs.RedrawWindow();
}