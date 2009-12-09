// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Structure.h"

#include "Construct Doc.h"

#include "Bars\ProjectBar.h"
#include "Bars\PropertiesBar.h"
#include "Bars\LayerBar.h"
#include "Bars\Animator Bar.h"
#include "Bars\Object Bar.h"
#include "Utilities\Ini.h"
#include "Structure\Ribbon.h"
#include "ChildFrame.h"
#include "Dialogs\TemplateDialog.h"
#include "ChildFrame.h"
#include "UI Elements\MDITabWnd.h"
#include "Utilities\CXAudio2.h"
#include "Utilities\MP3.h"

// Class
class CMainFrame : public CExtNCW<CMDIFrameWnd>
{
protected:
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();

	void InitialUpdateAllEditors();

	void ObjectTypesChanged();
	
	CList<CApplication*, CApplication*&> m_apps;

	// window placement persistence
	WINDOWPLACEMENT m_dataFrameWP;

	void _AdjustRTL();

	CExtRibbonNode * m_pRibbonNode;
	void _InitRibbonBar();

	// audio
	CXAudio2 audio;
	Mp3 mp3;

	// INI path
	CIni    m_INI;
	CString m_INIPath;

	// Store window statuses..
	bool bWasOpenProject;
	bool bWasOpenResources;
	bool bWasOpenLayers;
	bool bWasOpenProperties;
	bool bIsMinimized;

	CStringArray list;

	bool m_Maximize;

	long newAppNumber;
	CApplication* m_pPApp;

	CConstructMDITabs tabs;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual void OnContextHelp();
	virtual void OnClosing();
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	//}}AFX_VIRTUAL

	bool m_bAutoSave;
	bool m_bAutoBackup;
	int	 m_AutoSaveTime;

	bool has_been_closed;

	virtual ~CMainFrame();

protected:
	BOOL VerifyBarState(LPCTSTR lpszProfileName);

public:  // control bar embedded members

	// Ribbon bar
	CXRibbonBar			m_Ribbon;

	// Menu & status bars
	CExtStatusControlBar m_wndStatusBar;

	// Bars
	ProjectBar		project_bar;
	PropertiesBar	m_PropertiesBar;
	CLayerBar		m_LayerBar;
	CExtControlBar	animator_parent;
	AnimatorBar		animator;

	// Delay loading
	CString			m_Load;

	void nothing(){}
	// Internal
	CApplication* DoLoad(LPCSTR File);
	bool		  DoSave(LPCSTR File, CApplication* pApplication, bool isAutosave = false, bool omit_error = false);
	void		  AddToMRU(CApplication* pApplication);

	int m_iMaximized;
	void DoUpdateCheck(bool silent = true);

	void CloseChildWindows(EventSheet* pEventSheet);
	void CloseChildWindows(CLayout* pLayout);

	void UpdateTheme();

// Generated message map functions
public:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnExportEXE();
	afx_msg void OnEditMenu();
	afx_msg void OnPreferences();
	afx_msg void OnAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileNewApplication();
	afx_msg void OnFileTemplate();
	afx_msg void OnPreview();
	afx_msg void OnPreviewLayout(CLayout*);
	afx_msg void OnDebug();
	afx_msg void OnDebugLayout();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnExit();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg bool OnFileSave(CApplication* pApplication, bool isAutosave = false);
	afx_msg bool OnFileSaveAs(CApplication* pApplication);
	afx_msg void OnFileOpen();
	afx_msg void OnLayerBar();
	afx_msg void OnProjectBar();
	afx_msg void OnResourceBar();
	afx_msg void OnAnimationBar();
	afx_msg void OnPropertiesBar();
	afx_msg void OnObjectBar();
	afx_msg void OnViewVRAM();
	afx_msg void OnXP();
	afx_msg void OnOfficeXP();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void On2003();
	afx_msg void On2005();
	afx_msg void On2008();
	afx_msg void OnLunaBlue();
	afx_msg void OnLunaSilver();
	afx_msg void OnLunaBlack();
	afx_msg void OnExchange();
	afx_msg void OnWebScirra();
	afx_msg void OnWebForums();
	afx_msg void OnWebReportBug();
	afx_msg void OnWebHelp();
	afx_msg void OnWebUpdate();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	virtual LRESULT OnConstructPopupMenuCB(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



class CColorizedThemeOffice2003 : public CExtPaintManagerOffice2003
{
public:
	DECLARE_SERIAL( CColorizedThemeOffice2003 );

	void SetThemeColors(
		COLORREF clrFillHint,
		COLORREF clrAccentHint
		)
	{
		if(		m_clrFillHint == clrFillHint
			&&	m_clrAccentHint == clrAccentHint
			)
			return;

		m_clrFillHint = clrFillHint;
		m_clrAccentHint = clrAccentHint;
		InitTranslatedColors();
		InitHelperBrushes();

		CMainFrame* pMainFrame = (STATIC_DOWNCAST(CMainFrame,AfxGetMainWnd()));
		ASSERT( pMainFrame != NULL );
		ASSERT_VALID( pMainFrame );
		pMainFrame->RedrawWindow(
			NULL,
			NULL,
			RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
			| RDW_FRAME | RDW_ALLCHILDREN
			);
		CExtControlBar::stat_RedrawFloatingFrames( pMainFrame );
		CExtControlBar::stat_RecalcBarMetrics( pMainFrame );
	}

	COLORREF m_clrFillHint;
	COLORREF m_clrAccentHint;

	CColorizedThemeOffice2003()
		: m_clrFillHint( RGB(196,196,196) )
		, m_clrAccentHint( RGB(160, 180, 225) )
	{
	}

protected:
	virtual bool OnQueryThemeColors(
		COLORREF *pclrFillHint,
		COLORREF *pclrAccentHint = NULL
		)
	{
		if( pclrFillHint != NULL )
			*pclrFillHint = m_clrFillHint;
		if( pclrAccentHint != NULL )
			*pclrAccentHint = m_clrAccentHint;
		return true;
	}

	virtual bool OnQueryUseThemeColors() const
	{
		return true;
	}

	virtual e_system_theme_t OnQuerySystemTheme() const
	{
		return ThemeUnknown;
	}

	virtual bool OnStateSerialize(
		CArchive & ar,
		bool bEnableThrowExceptions = false
		)
	{
		CExtPaintManagerOffice2003::OnStateSerialize( ar, bEnableThrowExceptions );
		if( ar.IsStoring() )
		{
			ar << DWORD(m_clrFillHint);
			ar << DWORD(m_clrAccentHint);
		}
		else
		{
			DWORD dwTmp;
			ar >> dwTmp;
			m_clrFillHint = COLORREF(dwTmp);
			ar >> dwTmp;
			m_clrAccentHint = COLORREF(dwTmp);
		}
		return true;
	}
};