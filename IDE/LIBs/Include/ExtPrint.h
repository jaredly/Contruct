// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2008 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// mailto:support@prof-uis.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#if (!defined __EXT_PRINT_H)
#define __EXT_PRINT_H

#if (!defined __EXT_MFC_NO_PRINT)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_TOOLCONTROLBAR_H)
	#include <ExtToolControlBar.h>
#endif

#if (!defined __EXT_MFC_NO_REPORTGRIDWND)
	#if (!defined __EXT_REPORT_GRID_WND_H)
		#include <ExtReportGridWnd.h>
	#endif // (!defined __EXT_REPORT_GRID_WND_H)
#endif // (!defined __EXT_MFC_NO_REPORTGRIDWND)

#if (!defined __EXT_MFC_NO_TREEGRIDWND)
	#if (!defined __EXTTREEGRIDWND_H)
		#include <ExtTreeGridWnd.h>
	#endif
#endif // (!defined __EXT_MFC_NO_TREEGRIDWND)

#if (!defined __EXT_MFC_NO_GRIDBASEWND)
	#if (!defined __EXT_GRIDWND_H)
		#include <ExtGridWnd.h>
	#endif 
#endif // #if (!defined __EXT_MFC_NO_GRIDBASEWND)

#if (!defined __EXT_MFC_NO_PROPERTYGRIDWND)
	#if (!defined __EXTPROPERTYGRIDWND_H)
		#include <ExtPropertyGridWnd.h>
	#endif
#endif // (!defined __EXT_MFC_NO_PROPERTYGRIDWND)

#if (!defined __EXT_SCROLLWND_H)
	#include <ExtScrollWnd.h>
#endif 

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class _AFX_WIN_STATE;
class CPreviewDC;
class CExtPPVW_Printable;
class CExtPPVW_ToolBar;
class CExtPPVW_ContainerWnd;
class CExtPPVW_HostWnd;

/////////////////////////////////////////////////////////////////////////////
// CExtPPVW_MetafileCache

class __PROF_UIS_API CExtPPVW_MetafileCache
{
	bool m_bMetafileCache_DiskMode:1, m_bMetafileCache_Initializing:1;
	CArray < HANDLE, HANDLE & > m_arrMetafileCache_MemoryEmfPages;
	CExtSafeStringArray m_arrMetafileCache_DiskEmfNames;
	CMap < INT, INT, INT, INT & > m_mapMetafileCache_DiskC;
	CMap < INT, INT, HANDLE, HANDLE & > m_mapMetafileCache_DiskH;
public:
	bool m_bUseEnhancedMetafiles:1;
	CExtPPVW_MetafileCache();
	virtual ~CExtPPVW_MetafileCache();
	virtual bool MetafileCache_Initializing() const;
	virtual bool MetafileCache_DiskModeGet() const;
	virtual void MetafileCache_DiskModeSet( bool bDiskMode );
	virtual void MetafileCache_FreeAllPages();
	virtual INT MetafileCache_GetPageCount() const;
	virtual HANDLE MetafileCache_LockPage( INT nEmfIndex );
	virtual bool MetafileCache_UnockPage( INT nEmfIndex );
	virtual bool MetafileCache_Init( INT nEmfCount );
	virtual bool MetafileCache_InitAt(
		INT nEmfIndex,
		HANDLE hMetafile,
		__EXT_MFC_SAFE_LPCTSTR strMetafileName
		);
	virtual void MetafileCache_InitComplete( bool bOK );
	CExtSafeString MetafileCache_GetUniqueName( INT nEmfIndex );
	CExtSafeString MetafileCache_GetFolder() const;
};

/////////////////////////////////////////////////////////////////////////////
// CExtPPVW_Printable

class __PROF_UIS_API CExtPPVW_Printable : public CExtPPVW_MetafileCache
{
protected:
public:
	CExtSafeString m_strPpvCommandProfileName;
	CExtPPVW_ContainerWnd * m_pWndPrintPreviewContainer;
	CExtPPVW_HostWnd * m_pWndPP;
	bool m_bRestoreScrollBarVisibilityH:1, m_bRestoreScrollBarVisibilityV:1;
	DWORD m_dwRestoreScrollBarStyles, m_dwBasicPaintFlags;
public:
	bool m_bPrintPreviewEnabled:1, m_bPrintPreviewCheckMarkCmdStyle:1,
		m_bPrintPreviewShowPageMargins:1, m_bPpvwCreated:1,
		m_bUsePrintDialogFromOutside:1, m_bUsePrintDialogFromPreview:1,
	m_bSendingToPrinter:1;
	CExtPPVW_Printable();
	virtual ~CExtPPVW_Printable();
	virtual bool IsPrintPreviewMode() const;
	virtual bool IsPrinterAvailable() const;
	virtual bool IsPrintPreviewAvailable() const;
	virtual BOOL OnPreparePrinting(
		CPrintInfo * pInfo
		);
	virtual void OnPreparePrinting_Begin();
protected:
	virtual double _Internal_ComputerPreparePrintingProgress(
		LONG nPassNo,
		LONG nPassCount,
		LONG nPageNo,
		LONG nPageCount,
		LONG nContentWidthInPages,
		LONG nDocumentItemNo,
		LONG nDocumentItemCount,
		LONG nInPageDocumentItemFirstNo,
		LONG nInPageDocumentItemLastNo
		);
	virtual bool _Internal_Notify_OnPreparePrinting_Progress(
		LONG nPassNo,
		LONG nPassCount,
		LONG nPageNo,
		LONG nPageCount,
		LONG nContentWidthInPages,
		LONG nDocumentItemNo,
		LONG nDocumentItemCount,
		LONG nInPageDocumentItemFirstNo,
		LONG nInPageDocumentItemLastNo
		);
public:
	virtual bool OnPreparePrinting_Progress(
		LONG nPassNo,
		LONG nPassCount,
		LONG nPageNo,
		LONG nPageCount,
		LONG nContentWidthInPages,
		LONG nDocumentItemNo,
		LONG nDocumentItemCount,
		LONG nInPageDocumentItemFirstNo,
		LONG nInPageDocumentItemLastNo,
		double lfTotalJobPercent // 0.0 ... 100.0
		);
	virtual void OnPreparePrinting_End(
		bool bCanceled
		);
	virtual void OnBeginPrinting(
		CDC * pDC,
		CPrintInfo * pInfo
		);
	virtual void OnPrint(
		CDC * pDC,
		CPrintInfo * pInfo
		);
	virtual void OnEndPrinting(
		CDC * pDC,
		CPrintInfo * pInfo
		);
	virtual void OnBeginPrintPreview(
		bool bPreviewMode
		);
	virtual void OnEndPrintPreview(
		CDC * pDC,
		CPrintInfo * pInfo,
		POINT point,
		CExtPPVW_HostWnd * pWndPrintPreviewHost
		);
	virtual void OnPrepareDC(
		CDC * pDC,
		CPrintInfo * pInfo = NULL
		);
	virtual CWnd * OnGetPrintableWnd() = 0;
	virtual CWnd * OnGetWndToDisableWhilePrinting();
	virtual void OnGetPrintableDocTitle(
		CExtSafeString & strPrintableDocTitle
		);
	virtual void OnRecalcBars();
	class _PrintingDialog;
	static _AFX_WIN_STATE stat_winState;
	static BOOL CALLBACK stat_PrintingAbortProc( HDC, int );
	virtual BOOL DoPrintPreview(
		bool bPreviewMode,
		bool bDirectMode = false,
		CRuntimeClass * pPreviewViewClass = NULL,
		CPrintPreviewState * pState = NULL
		);
	virtual BOOL DoPrintDoc(
		BOOL bUsePrintDialog = FALSE
		);
	virtual bool DoReGenerateReport();
	virtual void OnInitializePrintPreviewToolBar();

	virtual INT AdjustExtentMeasuredX( INT nX ) const;
	virtual INT AdjustExtentMeasuredY( INT nY ) const;
	void AdjustExtentMeasured( SIZE & _sizeAdjust ) const
	{
		_sizeAdjust.cx = AdjustExtentMeasuredX( _sizeAdjust.cx );
		_sizeAdjust.cy = AdjustExtentMeasuredY( _sizeAdjust.cy );
	}
	virtual INT AdjustExtentRenderedX( INT nX ) const;
	virtual INT AdjustExtentRenderedY( INT nY ) const;
	void AdjustExtentRendered( SIZE & _sizeAdjust ) const
	{
		_sizeAdjust.cx = AdjustExtentRenderedX( _sizeAdjust.cx );
		_sizeAdjust.cy = AdjustExtentRenderedY( _sizeAdjust.cy );
	}

	struct friendly_app_t : public CWinApp
	{
		inline static bool _IsPrinterAvailable()
		{
			friendly_app_t * pApp = ( friendly_app_t * ) ::AfxGetApp();
			if( pApp->m_hDevNames == NULL )
			{
				PRINTDLG _pd;
				::memset( &_pd, 0, sizeof(PRINTDLG) );
				_pd.lStructSize = sizeof(PRINTDLG);
				if( pApp->GetPrinterDeviceDefaults( &_pd ) )
					return true;
				return false;
			}
			else
				return true;
		}
		inline static void _InvokeOnFilePrintSetup()
		{
			friendly_app_t * pApp = ( friendly_app_t * ) ::AfxGetApp();
			pApp->OnFilePrintSetup();
		}
	};
}; // class CExtPPVW_Printable

/////////////////////////////////////////////////////////////////////////////
// CExtPPVW_ToolBar

class __PROF_UIS_API CExtPPVW_ToolBar : public CExtToolControlBar
{
public:
	DECLARE_DYNCREATE( CExtPPVW_ToolBar );
	CExtPPVW_HostWnd * m_pWndPrintPreview;
	CExtZoomScrollBar m_wndZoomScrollBar;
	CExtPPVW_ToolBar();
	virtual ~CExtPPVW_ToolBar();
	virtual bool OnQueryMultiRowLayout() const;
	virtual CExtBarContentExpandButton * OnCreateBarRightBtn();
	bool InitZoomScrollBar();
protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
}; // class CExtPPVW_ToolBar

/////////////////////////////////////////////////////////////////////////////
// CExtPPVW_ContainerWnd

#define __EXT_MFC_PPV_CLASS_NAME_CONTAINER _T("ProfUIS-PrintPreviewContainer")

class __PROF_UIS_API CExtPPVW_ContainerWnd : public CExtScrollWnd
{
public:
	DECLARE_DYNCREATE( CExtPPVW_ContainerWnd );
	DECLARE_CExtPmBridge_MEMBERS( CExtPPVW_ContainerWnd );

	CExtPPVW_HostWnd * m_pWndPrintPreviewHost;
	CExtScrollBar m_wndScrollBarH, m_wndScrollBarV;
	bool m_bPpvwCreated:1;

	CExtPPVW_ContainerWnd();
	virtual ~CExtPPVW_ContainerWnd();

#if (defined _DEBUG)
	void AssertValid() const;
	void Dump( CDumpContext & dc ) const;
#endif // (defined _DEBUG)

// Implementation
public:
	virtual bool OnSwDoMouseWheel(
		UINT fFlags,
		short zDelta,
		CPoint point
		);
	virtual void OnSwUpdateScrollBars();
	virtual CRect OnSwRecalcLayout(
		bool bDoLayout,
		LPCRECT pRectClientSrc = NULL
		);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtPPVW_ContainerWnd)
	public:
	virtual BOOL PreTranslateMessage( MSG * pMsg );
	virtual BOOL OnCmdMsg( UINT nID, int nCode, void * pExtra, AFX_CMDHANDLERINFO * pHandlerInfo );
	virtual CScrollBar * GetScrollBarCtrl( int nBar ) const;
	protected:
	virtual BOOL PreCreateWindow( CREATESTRUCT & cs );
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExtPPVW_ContainerWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtPPVW_ContainerWnd

/////////////////////////////////////////////////////////////////////////////
// CExtPPVW_HostWnd

#define __EXT_MFC_PPV_CLASS_NAME_HOST			_T("ProfUIS-PrintPreview")

#define __EXT_MFC_PPV_PAGE_MODE_1				0
#define __EXT_MFC_PPV_PAGE_MODE_2				1
#define __EXT_MFC_PPV_PAGE_MODE_3				2
#define __EXT_MFC_PPV_PAGE_MODE_4				3
#define __EXT_MFC_PPV_PAGE_MODE_5				4
#define __EXT_MFC_PPV_PAGE_MODE_6				5
#define __EXT_MFC_PPV_PAGE_MODE_7				6
#define __EXT_MFC_PPV_PAGE_MODE_MIN				__EXT_MFC_PPV_PAGE_MODE_1
#define __EXT_MFC_PPV_PAGE_MODE_MAX				__EXT_MFC_PPV_PAGE_MODE_7
#define __EXT_MFC_PPV_PAGE_MODE_DEFAULT			__EXT_MFC_PPV_PAGE_MODE_1

#define __EXT_MFC_PPV_VIEW_MODE_FIT_WIDTH		0
#define __EXT_MFC_PPV_VIEW_MODE_FIT_HEIGHT		1
#define __EXT_MFC_PPV_VIEW_MODE_FIT_PAGES		2
#define __EXT_MFC_PPV_VIEW_MODE_MAGNIFY			3
#define __EXT_MFC_PPV_VIEW_MODE_MIN				__EXT_MFC_PPV_VIEW_MODE_FIT_WIDTH
#define __EXT_MFC_PPV_VIEW_MODE_MAX				__EXT_MFC_PPV_VIEW_MODE_MAGNIFY
#define __EXT_MFC_PPV_VIEW_MODE_DEFAULT			__EXT_MFC_PPV_VIEW_MODE_MAGNIFY

#define __EXT_MFC_MAGNIFY_LEVEL_0				0
#define __EXT_MFC_MAGNIFY_LEVEL_1				1
#define __EXT_MFC_MAGNIFY_LEVEL_2				2
#define __EXT_MFC_MAGNIFY_LEVEL_3				3
#define __EXT_MFC_MAGNIFY_LEVEL_4				4
#define __EXT_MFC_MAGNIFY_LEVEL_5				5
#define __EXT_MFC_MAGNIFY_LEVEL_6				6
#define __EXT_MFC_MAGNIFY_LEVEL_7				7
#define __EXT_MFC_MAGNIFY_LEVEL_8				8
#define __EXT_MFC_MAGNIFY_LEVEL_9				9
#define __EXT_MFC_MAGNIFY_LEVEL_10				10
#define __EXT_MFC_MAGNIFY_LEVEL_11				11
#define __EXT_MFC_MAGNIFY_LEVEL_12				12
#define __EXT_MFC_MAGNIFY_LEVEL_13				13
#define __EXT_MFC_MAGNIFY_LEVEL_14				14
#define __EXT_MFC_MAGNIFY_LEVEL_MIN				__EXT_MFC_MAGNIFY_LEVEL_0
#define __EXT_MFC_MAGNIFY_LEVEL_MAX				__EXT_MFC_MAGNIFY_LEVEL_14
#define __EXT_MFC_MAGNIFY_LEVEL_COUNT			( __EXT_MFC_MAGNIFY_LEVEL_MAX - __EXT_MFC_MAGNIFY_LEVEL_MIN + 1 )
#define __EXT_MFC_MAGNIFY_LEVEL_MIDDLE			( ( __EXT_MFC_MAGNIFY_LEVEL_MAX - __EXT_MFC_MAGNIFY_LEVEL_MIN ) / 2 )
#define __EXT_MFC_MAGNIFY_PX_PER_LEVEL			5
#define __EXT_MFC_MAGNIFY_PX_PER_BUTTON			24
#define	__EXT_MFC_MAGNIFY_PX_HORZ				( __EXT_MFC_MAGNIFY_LEVEL_COUNT * __EXT_MFC_MAGNIFY_PX_PER_LEVEL + __EXT_MFC_MAGNIFY_PX_PER_BUTTON * 2 )
#define __EXT_MFC_MAGNIFY_PX_VERT				__EXT_MFC_MAGNIFY_PX_PER_BUTTON
#define __EXT_MFC_MAGNIFY_LEVEL_SCROLL_MIN		__EXT_MFC_MAGNIFY_LEVEL_MIN
#define __EXT_MFC_MAGNIFY_LEVEL_SCROLL_MAX		__EXT_MFC_MAGNIFY_LEVEL_MAX
#define __EXT_MFC_MAGNIFY_LEVEL_SCROLL_MIDDLE	__EXT_MFC_MAGNIFY_LEVEL_MIDDLE
#define __EXT_MFC_MAGNIFY_LEVEL_SCROLL_PAGE		1

#define __EXT_MFC_PPV_PAGE_STACK_SIZE_DEFAULT 8

class __PROF_UIS_API CExtPPVW_HostWnd
	: public CScrollView
	, public CExtPmBridge
{
public:
	DECLARE_DYNCREATE( CExtPPVW_HostWnd );
	DECLARE_CExtPmBridge_MEMBERS( CExtPPVW_HostWnd );
	CExtPPVW_HostWnd();
	bool PpvSetPrintable( CExtPPVW_Printable * pPpvPrintable );
// Attributes
protected:
	CExtPPVW_Printable * m_pPpvPrintable;
	CDC m_dcPpvPrinter;
public:
	CPreviewDC * m_pPpvPreviewDC;
	CExtPPVW_ToolBar * m_pPpvWndToolBar;
	bool m_bPpvUseDocumentAreaBackground:1;
	bool m_bPpvUseZoomScrollBar:1;
	UINT m_nPpvPageMode, m_nPpvViewMode, m_nPpvMagnifyLevel;
	CPrintPreviewState * m_pPpvPrintPreviewState;
	CPrintInfo * m_pPpvPrintPreviewInfo;
public:
	UINT m_nPpvDisplayedPageCountDesired;
	UINT m_nPpvDisplayedPageCountMax;
	UINT m_nPpvDisplayedPageCountCurrent;
	UINT m_nPpvDisplayedPageIndex;
protected:
	CSize m_sizePpvDisplayedPagesDimesion;
	INT m_nPpvDistanceBetweenPages;
	HCURSOR m_hPpvMagnifyCursorIn, m_hPpvMagnifyCursorOut;
	CSize m_sizePpvPrinterPPI; // printer's pixels/per/inch

	struct PAGE_INFO
	{
		PAGE_INFO();
		CRect rectScreen; // screen rect (screen device units)
		CSize sizeUnscaled; // unscaled screen rect (screen device units)
		CSize sizeScaleRatio; // scale ratio (cx/cy)
		CSize sizeZoomOutRatio; // scale ratio when zoomed out (cx/cy)
	};
	PAGE_INFO * m_arrPpvPI;
	PAGE_INFO m_arrPpvPI_default[ __EXT_MFC_PPV_PAGE_STACK_SIZE_DEFAULT ];

// Operations
public:
	void PpvSetModes(
		UINT nPpvPageMode,
		UINT nPpvViewMode,
		UINT nPpvMagnifyLevel,
		UINT nPage,
		CPoint point
		);
protected:
	void _PpvScalePageMetrics( UINT nPage );
	void _PpvScalePageMetricsXY( UINT nPage, UINT _nPpvViewMode, bool bApply );
	CSize _PpvCalcPageScringMetrics();
public:
	void _PpvSetCurrentPage( UINT nPage, BOOL bClearRatios );
protected:
	BOOL _PpvFindPageRect( CPoint & point, UINT & nPage );
	virtual CSize CalcScaleRatio(CSize windowSize, CSize actualSize);
	virtual void PositionPage( UINT nPage );
	virtual void OnDisplayPageNumber( UINT nPage, UINT nPagesDisplayed );
	virtual void _PpvCleanUp();
public:
	virtual bool _PpvReGenerate();
	virtual void _PpvUpdateBars();
	virtual BOOL OnScrollBy(
		CSize sizeScroll,
		BOOL bDoScroll = TRUE
		);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtPPVW_HostWnd)
	public:
	virtual BOOL PreTranslateMessage( MSG * pMsg );
	virtual BOOL PreCreateWindow( CREATESTRUCT & cs );
	virtual CScrollBar * GetScrollBarCtrl( int nBar ) const;
	protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	virtual void OnDraw( CDC * pDC );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtPPVW_HostWnd();
	virtual void OnPrepareDC( CDC * pDC, CPrintInfo * pInfo = NULL );
///	IOleInPlaceUIWindow *m_pIPUIWindow;
#if (defined _DEBUG)
	void AssertValid() const;
	void Dump( CDumpContext & dc ) const;
#endif // (defined _DEBUG)

	//{{AFX_MSG(CExtPPVW_HostWnd)
protected:
	afx_msg void OnPreviewMarginsCmd();
	afx_msg void OnPreviewMarginsCmdUpdate( CCmdUI * pCmdUI );
public:
	afx_msg void OnPreviewClose();
protected:
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar * pScrollBar );
	afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar * pScrollBar );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
	afx_msg BOOL OnSetCursor( CWnd * pWnd, UINT nHitTest, UINT message );
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);

	afx_msg void OnPpv_PageNextCmd();
	afx_msg void OnPpv_PageNextUpdate( CCmdUI * pCmdUI );
	afx_msg void OnPpv_PagePrevCmd();
	afx_msg void OnPpv_PagePrevUpdate( CCmdUI * pCmdUI );

	afx_msg void OnPpv_ViewMode_Magnify_Cmd();
	afx_msg void OnPpv_ViewMode_Magnify_Update( CCmdUI * pCmdUI );
	afx_msg void OnPpv_ViewMode_FitWidth_Cmd();
	afx_msg void OnPpv_ViewMode_FitWidth_Update( CCmdUI * pCmdUI );
	afx_msg void OnPpv_ViewMode_FitHeight_Cmd();
	afx_msg void OnPpv_ViewMode_FitHeight_Update( CCmdUI * pCmdUI );
	afx_msg void OnPpv_ViewMode_FitPages_Cmd();
	afx_msg void OnPpv_ViewMode_FitPages_Update( CCmdUI * pCmdUI );

	afx_msg void OnPpv_Zoom_SB_Cmd();
	afx_msg void OnPpv_Zoom_SB_Update(CCmdUI* pCmdUI);

	afx_msg void OnPpv_Magnify_In_Cmd();
	afx_msg void OnPpv_Magnify_In_Update(CCmdUI* pCmdUI);
	afx_msg void OnPpv_Magnify_Out_Cmd();
	afx_msg void OnPpv_Magnify_Out_Update(CCmdUI* pCmdUI);

	afx_msg void OnPpv_PageMode_1_Cmd();
	afx_msg void OnPpv_PageMode_1_Update( CCmdUI * pCmdUI );
	afx_msg void OnPpv_PageMode_2_Cmd();
	afx_msg void OnPpv_PageMode_2_Update( CCmdUI * pCmdUI );
	afx_msg void OnPpv_PageMode_3_Cmd();
	afx_msg void OnPpv_PageMode_3_Update( CCmdUI * pCmdUI );
	afx_msg void OnPpv_PageMode_4_Cmd();
	afx_msg void OnPpv_PageMode_4_Update( CCmdUI * pCmdUI );
	afx_msg void OnPpv_PageMode_5_Cmd();
	afx_msg void OnPpv_PageMode_5_Update( CCmdUI * pCmdUI );
	afx_msg void OnPpv_PageMode_6_Cmd();
	afx_msg void OnPpv_PageMode_6_Update( CCmdUI * pCmdUI );
	afx_msg void OnPpv_PageMode_7_Cmd();
	afx_msg void OnPpv_PageMode_7_Update( CCmdUI * pCmdUI );

	afx_msg void OnPreviewPrint();
	afx_msg void OnPreviewSetup();

	//}}AFX_MSG

	afx_msg LRESULT _OnIdleUpdateCmdUI( WPARAM wParam, LPARAM lParam );
	afx_msg void _OnInitialUpdate();
	
	DECLARE_MESSAGE_MAP()
}; // class CExtPPVW_HostWnd

/////////////////////////////////////////////////////////////////////////////
// required member methods for  PPVW

#define __EXT_MFC_IMPLEMENT_REQUIRED_PPVW_METHODS( __BASE_CLASS__ ) \
public: \
	virtual CWnd * OnGetPrintableWnd() \
	{ \
		ASSERT_VALID( this ); \
		CWnd * pWndPrintable = this; \
		if( pWndPrintable->IsKindOf( RUNTIME_CLASS( CView ) ) ) \
			pWndPrintable = pWndPrintable->GetParentFrame(); \
		return pWndPrintable; \
	} \
	virtual BOOL __PPVW_Impl_PreTranslateMessage( MSG * pMsg ) \
	{ \
		if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL ) \
		{ \
			ASSERT_VALID( m_pWndPP ); \
			if( m_pWndPrintPreviewContainer->PreTranslateMessage( pMsg ) ) \
				return TRUE; \
		} \
		return __BASE_CLASS__ :: PreTranslateMessage( pMsg ); \
	} \
	virtual BOOL __PPVW_Impl_OnCmdMsg( UINT nID, int nCode, void * pExtra, AFX_CMDHANDLERINFO * pHandlerInfo ) \
	{ \
		ASSERT_VALID( this ); \
		switch( nID ) \
		{ \
		case ID_FILE_PRINT_SETUP: \
			switch( nCode ) \
			{ \
			case CN_COMMAND: \
				CExtPaintManager::stat_PassPaintMessages(); \
				if( IsPrintPreviewAvailable() ) \
				{ \
					if( IsPrintPreviewMode() && m_pWndPP->GetSafeHwnd() != NULL ) \
						m_pWndPP->SendMessage( WM_COMMAND, ID_FILE_PRINT_SETUP ); \
					else \
						friendly_app_t::_InvokeOnFilePrintSetup(); \
				} \
			break; \
			case CN_UPDATE_COMMAND_UI: \
				ASSERT( pExtra != NULL ); \
				((CCmdUI*)pExtra)->Enable( IsPrintPreviewAvailable() ? TRUE : FALSE ); \
			break; \
			} \
			return TRUE; \
		case ID_FILE_PRINT: \
		case ID_FILE_PRINT_DIRECT: \
			switch( nCode ) \
			{ \
			case CN_COMMAND: \
				CExtPaintManager::stat_PassPaintMessages(); \
				if( IsPrintPreviewAvailable() ) \
				{ \
					if( m_pWndPP->GetSafeHwnd() != NULL ) \
						m_pWndPP->OnPreviewClose(); \
					CExtPPVW_Printable::DoPrintPreview( false, ( nID == ID_FILE_PRINT_DIRECT ) ? true : false ); \
				} \
			break; \
			case CN_UPDATE_COMMAND_UI: \
				ASSERT( pExtra != NULL ); \
				((CCmdUI*)pExtra)->Enable( IsPrintPreviewAvailable() ? TRUE : FALSE ); \
			break; \
			} \
			return TRUE; \
		case ID_FILE_PRINT_PREVIEW: \
			switch( nCode ) \
			{ \
			case CN_COMMAND: \
				CExtPaintManager::stat_PassPaintMessages(); \
				if( IsPrintPreviewAvailable() ) \
				{ \
					if( m_bPrintPreviewCheckMarkCmdStyle && m_pWndPP->GetSafeHwnd() != NULL ) \
						m_pWndPP->OnPreviewClose(); \
					else \
						CExtPPVW_Printable::DoPrintPreview( true ); \
				} \
			break; \
			case CN_UPDATE_COMMAND_UI: \
				ASSERT( pExtra != NULL ); \
				if( ! IsPrintPreviewAvailable() ) \
				{ \
					((CCmdUI*)pExtra)->SetCheck( 0 ); \
					((CCmdUI*)pExtra)->Enable( FALSE ); \
				} \
				else \
				{ \
					((CCmdUI*)pExtra)->Enable( TRUE ); \
					if( m_bPrintPreviewCheckMarkCmdStyle ) \
						((CCmdUI*)pExtra)->SetCheck( IsPrintPreviewMode() ? 1 : 0 ); \
					else \
						((CCmdUI*)pExtra)->SetCheck( 0 ); \
				} \
			break; \
			} \
			return TRUE; \
		} \
		if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL ) \
		{ \
			ASSERT_VALID( m_pWndPrintPreviewContainer ); \
			return m_pWndPrintPreviewContainer->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ); \
		} \
		return __BASE_CLASS__ :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ); \
	} \
protected: \
	virtual LRESULT __PPVW_Impl_WindowProc( UINT message, WPARAM wParam, LPARAM lParam ) \
	{ \
		switch( message ) \
		{ \
		case 0x0363: /* WM_IDLEUPDATECMDUI */  \
			if( m_pWndPP->GetSafeHwnd() != NULL ) \
				return m_pWndPP->SendMessage( message, wParam, lParam ); \
			return 0; \
		case WM_SETFOCUS: \
			if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL ) \
			{ \
				ASSERT_VALID( m_pWndPP ); \
				m_pWndPrintPreviewContainer->SetFocus(); \
				return 0; \
			} \
		break; \
		case WM_NCCALCSIZE: \
			if( m_pWndPP != NULL && m_dwRestoreScrollBarStyles != 0 ) \
				return 0; \
		break; \
		case WM_SETCURSOR: \
			::SetCursor( ::LoadCursor( NULL, IDC_ARROW ) ); \
			return 1; \
		case WM_SIZE: \
			if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL ) \
			{ \
				CRect rcClient; \
				m_pWndPrintPreviewContainer->GetParent()->GetClientRect( &rcClient ); \
				m_pWndPrintPreviewContainer->MoveWindow( &rcClient ); \
				OnRecalcBars(); \
			} \
			break; \
		case WM_CONTEXTMENU: \
			if( IsPrintPreviewMode() ) \
				return 0; \
			break; \
		} \
		LRESULT lResult = __BASE_CLASS__ :: WindowProc( message, wParam, lParam ); \
		return lResult; \
	}

#define __EXT_MFC_IMPLEMENT_REQUIRED_PPVW_PreTranslateMessage( __BASE_CLASS__ ) \
public: \
  virtual BOOL PreTranslateMessage( MSG * pMsg ) \
  { \
	  return __PPVW_Impl_PreTranslateMessage( pMsg ); \
  }

#define __EXT_MFC_IMPLEMENT_REQUIRED_PPVW_OnCmdMsg( __BASE_CLASS__ ) \
public: \
	virtual BOOL OnCmdMsg( UINT nID, int nCode, void * pExtra, AFX_CMDHANDLERINFO * pHandlerInfo ) \
	{ \
		return __PPVW_Impl_OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );  \
	}

#define __EXT_MFC_IMPLEMENT_REQUIRED_PPVW_WindowProc( __BASE_CLASS__ ) \
protected: \
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam ) \
	{ \
		return __PPVW_Impl_WindowProc( message, wParam, lParam ); \
	}

/////////////////////////////////////////////////////////////////////////////
// template < class _BC > class CExtPPVW

template < class _BC >
class CExtPPVW
	: public _BC
	, public CExtPPVW_Printable
{
public:
	CExtPPVW()
	{
	}
	virtual ~CExtPPVW()
	{
	}

	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_METHODS( _BC )
	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_PreTranslateMessage( _BC )
	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_OnCmdMsg( _BC )
	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_WindowProc( _BC )

public:
	virtual void OnRecalcBars()
	{
		ASSERT_VALID( this );
		CExtPPVW_Printable::OnRecalcBars();
		if( m_pWndPP->GetSafeHwnd() != NULL )
			m_pWndPP->_PpvUpdateBars();
	}

}; // template < class _BC > class CExtPPVW

#if (!defined __EXT_MFC_NO_GRIDBASEWND)

/////////////////////////////////////////////////////////////////////////////
// template < > class CExtPPVW < CExtGridWnd >

template < > class __PROF_UIS_API CExtPPVW < CExtGridWnd >
	: public CExtGridWnd
	, public CExtPPVW_Printable
{
public:
	CExtPPVW()
	{
		m_dwBasicPaintFlags =
			  __EGCPF_NO_BUTTONS
			| __EGCPF_NO_SORT_ARROWS
			| __EGCPF_NO_FOCUS_ARROWS
			;
	}
	virtual ~CExtPPVW()
	{
	}

	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_METHODS( CExtGridWnd )
	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_PreTranslateMessage( CExtGridWnd )
	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_OnCmdMsg( CExtGridWnd )
	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_WindowProc( CExtGridWnd )

public:
	virtual void OnSwUpdateScrollBars()
	{
		ASSERT_VALID( this );
		if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
			return;
		CExtGridWnd::OnSwUpdateScrollBars();
	}
	virtual bool OnSwDoMouseWheel(
		UINT fFlags,
		short zDelta,
		CPoint point
		)
	{
		ASSERT_VALID( this );
		if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
			return false;
		return CExtGridWnd::OnSwDoMouseWheel( fFlags, zDelta, point );
	}
	virtual CRect OnSwRecalcLayout(
		bool bDoLayout,
		LPCRECT pRectClientSrc = NULL
		)
	{
		ASSERT_VALID( this );
		if( bDoLayout && m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
		{
			ASSERT_VALID( m_pWndPP );
			CRect rcClient;
			if( pRectClientSrc != NULL )
				rcClient = *pRectClientSrc;
			else
				CWnd::GetClientRect( &rcClient );
			if( m_pWndPP->GetSafeHwnd() != NULL )
				m_pWndPP->_PpvUpdateBars();
			m_pWndPrintPreviewContainer->SetWindowPos(
				&CWnd::wndTop, 0, 0, 0, 0,
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE
				);
			CWnd::RepositionBars(
				AFX_IDW_PANE_FIRST - 1,
				AFX_IDW_PANE_FIRST + 1,
				AFX_IDW_PANE_FIRST,
				bDoLayout ? CWnd::reposDefault : CWnd::reposQuery,
				&rcClient,
				&rcClient,
				TRUE
				);
			//return rcClient;
			bDoLayout = false;
		} // if( bDoLayout && m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
		CRect rcRetVal = CExtGridWnd::OnSwRecalcLayout( bDoLayout, pRectClientSrc );
		return rcRetVal;
	}
	virtual void OnRecalcBars()
	{
		ASSERT_VALID( this );
		CExtPPVW_Printable::OnRecalcBars();
 		if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
 			m_pWndPrintPreviewContainer->OnSwRecalcLayout( true );
	}
	virtual void OnEndPrintPreview(
		CDC * pDC,
		CPrintInfo * pInfo,
		POINT point,
		CExtPPVW_HostWnd * pWndPrintPreviewHost
		)
	{
		ASSERT_VALID( this );
		CExtPPVW_Printable::OnEndPrintPreview( pDC, pInfo, point, pWndPrintPreviewHost );
		if( GetSafeHwnd() != NULL )
		{
			OnSwUpdateScrollBars();
			OnSwRecalcLayout( true );
			OnSwDoRedraw();
		}
	}
	virtual void OnPrepareDC(
		CDC * pDC,
		CPrintInfo * pInfo = NULL
		);
	virtual bool IsPrintPreviewAvailable() const;
	virtual BOOL OnPreparePrinting(
		CPrintInfo * pInfo
		);
};  // template < > class CExtPPVW < CExtGridWnd >

#endif // #if (!defined __EXT_MFC_NO_GRIDBASEWND)

#if (!defined __EXT_MFC_NO_TREEGRIDWND)

/////////////////////////////////////////////////////////////////////////////
// template < > class CExtPPVW < CExtTreeGridWnd >

template < > class __PROF_UIS_API CExtPPVW < CExtTreeGridWnd >
	: public CExtTreeGridWnd
	, public CExtPPVW_Printable
{
public:
	CExtPPVW()
	{
		m_dwBasicPaintFlags =
			  __EGCPF_NO_BUTTONS
			| __EGCPF_NO_SORT_ARROWS
			| __EGCPF_NO_FOCUS_ARROWS
			;
	}
	virtual ~CExtPPVW()
	{
	}

	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_METHODS( CExtTreeGridWnd )
	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_PreTranslateMessage( CExtTreeGridWnd )
	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_OnCmdMsg( CExtTreeGridWnd )
	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_WindowProc( CExtTreeGridWnd )

public:
	virtual void OnSwUpdateScrollBars()
	{
		ASSERT_VALID( this );
		if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
			return;
		CExtTreeGridWnd::OnSwUpdateScrollBars();
	}
	virtual bool OnSwDoMouseWheel(
		UINT fFlags,
		short zDelta,
		CPoint point
		)
	{
		ASSERT_VALID( this );
		if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
			return false;
		return CExtTreeGridWnd::OnSwDoMouseWheel( fFlags, zDelta, point );
	}
	virtual CRect OnSwRecalcLayout(
		bool bDoLayout,
		LPCRECT pRectClientSrc = NULL
		)
	{
		ASSERT_VALID( this );
		if( bDoLayout && m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
		{
			ASSERT_VALID( m_pWndPP );
			CRect rcClient;
			if( pRectClientSrc != NULL )
				rcClient = *pRectClientSrc;
			else
				CWnd::GetClientRect( &rcClient );
			if( m_pWndPP->GetSafeHwnd() != NULL )
				m_pWndPP->_PpvUpdateBars();
			m_pWndPrintPreviewContainer->SetWindowPos(
				&CWnd::wndTop, 0, 0, 0, 0,
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE
				);
			CWnd::RepositionBars(
				AFX_IDW_PANE_FIRST - 1,
				AFX_IDW_PANE_FIRST + 1,
				AFX_IDW_PANE_FIRST,
				bDoLayout ? CWnd::reposDefault : CWnd::reposQuery,
				&rcClient,
				&rcClient,
				TRUE
				);
			//return rcClient;
			bDoLayout = false;
		} // if( bDoLayout && m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
		CRect rcRetVal = CExtTreeGridWnd::OnSwRecalcLayout( bDoLayout, pRectClientSrc );
		return rcRetVal;
	}
	virtual void OnRecalcBars()
	{
		ASSERT_VALID( this );
		CExtPPVW_Printable::OnRecalcBars();
 		if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
 			m_pWndPrintPreviewContainer->OnSwRecalcLayout( true );
	}
	virtual void OnEndPrintPreview(
		CDC * pDC,
		CPrintInfo * pInfo,
		POINT point,
		CExtPPVW_HostWnd * pWndPrintPreviewHost
		)
	{
		ASSERT_VALID( this );
		CExtPPVW_Printable::OnEndPrintPreview( pDC, pInfo, point, pWndPrintPreviewHost );
		if( GetSafeHwnd() != NULL )
		{
			OnSwUpdateScrollBars();
			OnSwRecalcLayout( true );
			OnSwDoRedraw();
		}
	}
	virtual void OnPrepareDC(
		CDC * pDC,
		CPrintInfo * pInfo = NULL
		);
	virtual bool OnCalcPrintableTreeItemArrayImpl(
		HTREEITEM htiParent,
		CList < HTREEITEM, HTREEITEM & > & listPrintableItems
		);
	virtual void OnCalcPrintableTreeItemArray(
		CArray < HTREEITEM, HTREEITEM & > & arrPrintableItems
		);
	virtual INT OnCalcPrintableTreeItemIndent(
		CDC & dcMeasure,
		HTREEITEM hti
		);
	virtual bool IsPrintPreviewAvailable() const;
	virtual BOOL OnPreparePrinting(
		CPrintInfo * pInfo
		);
};  // template < > class CExtPPVW < CExtTreeGridWnd >

#endif // (!defined __EXT_MFC_NO_TREEGRIDWND)

#if (!defined __EXT_MFC_NO_REPORTGRIDWND)

/////////////////////////////////////////////////////////////////////////////
// template < > class CExtPPVW < CExtReportGridWnd >

template < > class __PROF_UIS_API CExtPPVW < CExtReportGridWnd >
	: public CExtReportGridWnd
	, public CExtPPVW_Printable
{
public:
	bool m_bRestoreColumnChooserVisibility:1;
	CExtPPVW()
		: m_bRestoreColumnChooserVisibility( false )
	{
		m_dwBasicPaintFlags =
			  __EGCPF_NO_BUTTONS
			| __EGCPF_NO_SORT_ARROWS
			| __EGCPF_NO_FOCUS_ARROWS
			;
	}
	virtual ~CExtPPVW()
	{
	}

	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_METHODS( CExtReportGridWnd )
	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_PreTranslateMessage( CExtReportGridWnd )
	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_OnCmdMsg( CExtReportGridWnd )
	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_WindowProc( CExtReportGridWnd )

public:
	virtual void OnSwUpdateScrollBars()
	{
		ASSERT_VALID( this );
		if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
			return;
		CExtReportGridWnd::OnSwUpdateScrollBars();
	}
	virtual bool OnSwDoMouseWheel(
		UINT fFlags,
		short zDelta,
		CPoint point
		)
	{
		ASSERT_VALID( this );
		if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
			return false;
		return CExtReportGridWnd::OnSwDoMouseWheel( fFlags, zDelta, point );
	}
	virtual CRect OnSwRecalcLayout(
		bool bDoLayout,
		LPCRECT pRectClientSrc = NULL
		)
	{
		ASSERT_VALID( this );
		if( bDoLayout && m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
		{
			ASSERT_VALID( m_pWndPP );
			CRect rcClient;
			if( pRectClientSrc != NULL )
				rcClient = *pRectClientSrc;
			else
				CWnd::GetClientRect( &rcClient );
			if( m_pWndPP->GetSafeHwnd() != NULL )
				m_pWndPP->_PpvUpdateBars();
			m_pWndPrintPreviewContainer->SetWindowPos(
				&CWnd::wndTop, 0, 0, 0, 0,
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE
				);
			CWnd::RepositionBars(
				AFX_IDW_PANE_FIRST - 1,
				AFX_IDW_PANE_FIRST + 1,
				AFX_IDW_PANE_FIRST,
				bDoLayout ? CWnd::reposDefault : CWnd::reposQuery,
				&rcClient,
				&rcClient,
				TRUE
				);
			//return rcClient;
			bDoLayout = false;
		} // if( bDoLayout && m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
		CRect rcRetVal = CExtReportGridWnd::OnSwRecalcLayout( bDoLayout, pRectClientSrc );
		return rcRetVal;
	}
	virtual void OnRecalcBars()
	{
		ASSERT_VALID( this );
		CExtPPVW_Printable::OnRecalcBars();
 		if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
 			m_pWndPrintPreviewContainer->OnSwRecalcLayout( true );
	}
	virtual BOOL DoPrintPreview(
		bool bPreviewMode,
		bool bDirectMode = false,
		CRuntimeClass * pPreviewViewClass = NULL,
		CPrintPreviewState * pState = NULL
		)
	{
		ASSERT_VALID( this );
		m_bRestoreColumnChooserVisibility = false;
		BOOL bRetVal = CExtPPVW_Printable::DoPrintPreview( bPreviewMode, bDirectMode, pPreviewViewClass, pState );
		if(		bRetVal
			&&	bPreviewMode
			&&	ReportColumnChooserIsVisible()
			)
		{
			CExtReportGridColumnChooserWnd * pCCW =
				ReportColumnChooserGet();
			ASSERT_VALID( pCCW );
			ASSERT( pCCW->GetSafeHwnd() != NULL );
			if( pCCW->GetParent() != this )
			{
				m_bRestoreColumnChooserVisibility = true;
				ReportColumnChooserShow( false );
			}
		}
		return bRetVal;
	}
	virtual void OnEndPrintPreview(
		CDC * pDC,
		CPrintInfo * pInfo,
		POINT point,
		CExtPPVW_HostWnd * pWndPrintPreviewHost
		)
	{
		ASSERT_VALID( this );
		CExtPPVW_Printable::OnEndPrintPreview( pDC, pInfo, point, pWndPrintPreviewHost );
		if( GetSafeHwnd() != NULL )
		{
			if( m_bRestoreColumnChooserVisibility )
			{
				m_bRestoreColumnChooserVisibility = false;
				ReportColumnChooserShow();
			}
			OnSwUpdateScrollBars();
			OnSwRecalcLayout( true );
			OnSwDoRedraw();
		}
	}
	virtual void OnPrepareDC(
		CDC * pDC,
		CPrintInfo * pInfo = NULL
		);
	virtual bool OnCalcPrintableTreeItemArrayImpl(
		HTREEITEM htiParent,
		CList < HTREEITEM, HTREEITEM & > & listPrintableItems
		);
	virtual void OnCalcPrintableTreeItemArray(
		CArray < HTREEITEM, HTREEITEM & > & arrPrintableItems
		);
	virtual INT OnCalcPrintableTreeItemIndent(
		CDC & dcMeasure,
		HTREEITEM hti
		);
	virtual bool IsPrintPreviewAvailable() const;
	virtual BOOL OnPreparePrinting(
		CPrintInfo * pInfo
		);
};  // template < > class CExtPPVW < CExtReportGridWnd >

#endif // (!defined __EXT_MFC_NO_REPORTGRIDWND)

#if (!defined __EXT_MFC_NO_PROPERTYGRIDWND)

/////////////////////////////////////////////////////////////////////////////
// template < > class CExtPPVW < CExtPropertyGridCtrl >

template < > class __PROF_UIS_API CExtPPVW < CExtPropertyGridCtrl >
	: public CExtPropertyGridCtrl
	, public CExtPPVW_Printable
{
public:
	CExtPPVW()
	{
		m_dwBasicPaintFlags =
			  __EGCPF_NO_BUTTONS
			| __EGCPF_NO_SORT_ARROWS
			| __EGCPF_NO_FOCUS_ARROWS
			;
	}
	virtual ~CExtPPVW()
	{
	}

	__EXT_MFC_IMPLEMENT_REQUIRED_PPVW_METHODS( CExtPropertyGridCtrl )

public:
	virtual void RecalcLayout()
	{
		ASSERT_VALID( this );
		if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
			return;
		CExtPropertyGridCtrl::RecalcLayout();
	}
	virtual void OnRecalcBars()
	{
		ASSERT_VALID( this );
		CExtPPVW_Printable::OnRecalcBars();
 		if( m_pWndPrintPreviewContainer->GetSafeHwnd() != NULL )
 			m_pWndPrintPreviewContainer->OnSwRecalcLayout( true );
	}
	virtual void OnEndPrintPreview(
		CDC * pDC,
		CPrintInfo * pInfo,
		POINT point,
		CExtPPVW_HostWnd * pWndPrintPreviewHost
		)
	{
		ASSERT_VALID( this );
		CExtPPVW_Printable::OnEndPrintPreview( pDC, pInfo, point, pWndPrintPreviewHost );
		if( GetSafeHwnd() != NULL )
			RecalcLayout();
	}
	virtual void OnPrepareDC(
		CDC * pDC,
		CPrintInfo * pInfo = NULL
		);
	virtual void OnCalcPrintablePropertyItemArray(
		CArray < CExtPropertyItem *, CExtPropertyItem * & > & arrPrintableItems
		);
	virtual INT OnCalcPrintableTreeItemIndent(
		CDC & dcMeasure,
		CExtPropertyItem * pPropertyItem
		);
	virtual bool IsPrintPreviewAvailable() const;
	virtual BOOL OnPreparePrinting(
		CPrintInfo * pInfo
		);
	virtual BOOL OnCmdMsg(
		UINT nID,
		int nCode,
		void * pExtra,
		AFX_CMDHANDLERINFO * pHandlerInfo
		);
	virtual bool DoReGenerateReport();
	virtual bool OnPgcCreateBars();
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage( MSG * pMsg );
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
};  // template < > class CExtPPVW < CExtPropertyGridCtrl >

#endif // (!defined __EXT_MFC_NO_PROPERTYGRIDWND)

#endif // __EXT_MFC_NO_PRINT

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __EXT_PRINT_H
