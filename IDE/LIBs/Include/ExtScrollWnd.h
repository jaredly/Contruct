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

#if (!defined __EXT_SCROLLWND_H)
#define __EXT_SCROLLWND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __AFXTEMPL_H__)
	#include <AfxTempl.h>
#endif

#if (!defined __EXT_MOUSECAPTURESINK_H)
	#include <../Src/ExtMouseCaptureSink.h>
#endif

#if (!defined __EXT_CONTROLBAR_H)
	#include <ExtControlBar.h>
#endif

#if (!defined __EXT_HOOK_H)
	#include "../Src/ExtHook.h"
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#if (!defined __EXT_MFC_NO_SCROLLCONAINERWND)
	#if (!defined __EXT_LABEL_H)
		#include <ExtLabel.h>
	#endif
#endif // (!defined __EXT_MFC_NO_SCROLLCONAINERWND)

#define __EXT_SCROLL_NUMERIC_MAX INT_MAX

/////////////////////////////////////////////////////////////////////////////
// CExtScrollBar window

class __PROF_UIS_API CExtScrollBar
	: public CScrollBar
	, public CExtAnimationSingleton
	, public CExtHookSpy
{
public:
	DECLARE_DYNCREATE( CExtScrollBar );
	CExtScrollBar();

public:
	INT m_nStepSize;
	virtual INT GetStepSize() const;
	enum eScrollerOrientation_t
	{
		__ESO_TOP = 0,
		__ESO_BOTTOM = 1,
		__ESO_LEFT = 2,
		__ESO_RIGHT = 3,
		__ESO_AUTO = 4,
		__ESO_NONE = 5,
		__ESO_MIN_VALUE = 0,
		__ESO_MAX_VALUE = 5,
	};
	eScrollerOrientation_t m_eSO;
	bool m_bAutoDeleteOnPostNcDestroy:1, m_bReflectParentSizing:1,
		m_bDisableAutoReserveSpace:1, m_bHelperLightAccent:1,
		m_bAutoReserveSpaceBefore:1, m_bAutoReserveSpaceAfter:1,
		m_bCompleteRepaint:1, m_bSmoothPainting:1,
		m_bHideHwndBefore:1, m_bHideHwndAfter:1,
		m_bEnabledToolTips:1, m_bEnabledTrackingToolTips:1,
		m_bEnableHookSpy:1, m_bHookCapturedState:1,
		m_bSendActionNotifications:1, m_bPopupInactiveLightMode:1;
	HWND m_hWndBefore, m_hWndAfter;
	CExtSafeString
		m_strTipTextForUpButton, m_strTipTextForDownButton,
		m_strTipTextForPageUpArea, m_strTipTextForPageDownArea,
		m_strTipTextForThumbButton;
	int m_nMouseActivateCode;

	virtual void SyncReservedSpace( CExtScrollBar * pOtherBar );
	virtual eScrollerOrientation_t GetScrollerOrientation() const;

public:

	//{{AFX_VIRTUAL(CExtScrollBar)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	virtual bool OnHookSpyPreTranslateMessage(
		MSG * pMSG
		);
	virtual bool OnHookSpyMouseMoveMsg(
		MSG * pMSG
		);
	virtual bool OnHookSpyMouseClickMsg(
		MSG * pMSG
		);
	bool ScrollBar_OnMouseMoveMsg(
		MSG * pMSG
		);
	bool ScrollBar_OnMouseClickMsg(
		MSG * pMSG
		);
	virtual HWND ScrollBar_CaptureGet() const;
	virtual void ScrollBar_CaptureSet();
	virtual void ScrollBar_CaptureRelease();
	virtual void ScrollBar_TrackMouseLButtonDown(
		MSG * pMSG
		);

	virtual void OnSbPaint( CDC & dc );

	virtual bool _IsLockAbleMessage( UINT message ) const;
	virtual INT _GetScrollPos(
		bool bTrackPos = false
		) const;
	virtual void _SetScrollPos(
		INT nPos,
		bool bTrackPos = false,
		bool bRedraw = true,
		bool bSendScrollingNotification = true
		);
	virtual ~CExtScrollBar();
	virtual CExtPopupMenuTipWnd * OnAdvancedPopupMenuTipWndGet() const;
	virtual bool OnAdvancedPopupMenuTipWndGetInfo(
		CExtPopupMenuTipWnd & _ATTW,
		INT nSBMHT,
		bool bTrackingTip,
		CRect & rcExcludeArea,
		CExtSafeString & strTipText
		) const;
	virtual void OnAdvancedPopupMenuTipWndDisplay(
		CExtPopupMenuTipWnd & _ATTW,
		INT nSBMHT,
		bool bTrackingTip
		) const;
	virtual void OnAdvancedPopupMenuTipWndDisplay(
		CExtPopupMenuTipWnd & _ATTW,
		const RECT & rcExcludeArea,
		__EXT_MFC_SAFE_LPCTSTR strTipText,
		bool bTrackingTip
		) const;
	virtual void PostNcDestroy();
	virtual bool AnimationClient_EnabledGet() const;
	virtual bool AnimationSite_EnabledGet() const;
	virtual void AnimationSite_OnProgressShutdownTimer( UINT nTimerID );
	virtual HWND AnimationSite_GetSafeHWND() const;
	virtual const CExtAnimationParameters *
		AnimationClient_OnQueryAnimationParameters(
			INT eAPT // __EAPT_*** animation type
			) const;
	virtual bool AnimationClient_CacheNextState(
		CDC & dc,
		const RECT & rcAcAnimationTarget,
		bool bAnimate,
		INT eAPT // __EAPT_*** animation type
		);
	virtual void PmBridge_OnPaintManagerChanged(
		CExtPaintManager * pGlobalPM
		);
	INT m_nHorzBarHeight, m_nVertBarWidth,
		m_nHorzButtonWidth, m_nVertButtonHeight,
		m_nHorzThumbMinWidth, m_nVertThumbMinHeight;
protected:
	virtual void _ScanSysMertics();
	bool m_bProcessingClick:1, m_bProcessingOutClick:1,
		m_bProcessingHover:1, m_bProcessingCancelMode:1,
		m_bHelperHaveTrackPos:1;
	INT m_nSBMHT, m_nHelperTrackPos;
public:
	bool m_bAlwaysSendThumbButtonNotifications:1;

protected:
	//{{AFX_MSG(CExtScrollBar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
    afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg LRESULT OnSizeParent( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
}; // class CExtScrollBar

/////////////////////////////////////////////////////////////////////////////
// CExtNSB template
// Provides any CWnd with Prof-UIS CExtScrollBar nicer scroll bars.
// CWnd should use RepositionBars for docking Prof-UIS scroll bars to its borders.

template < class _BT >
class CExtNSB : public _BT
{
public:
	mutable CExtScrollBar m_wndScrollBarH, m_wndScrollBarV;
	virtual bool NSB_EnsureScrollBarsCreated() const
	{
		if(		m_wndScrollBarH.GetSafeHwnd() != NULL
			&&	m_wndScrollBarV.GetSafeHwnd() != NULL
			)
			return true;
		m_wndScrollBarH.m_eSO = CExtScrollBar::__ESO_BOTTOM;
		m_wndScrollBarV.m_eSO = CExtScrollBar::__ESO_RIGHT;
		if( m_wndScrollBarV.GetSafeHwnd() == NULL )
		{
			if( ! m_wndScrollBarV.Create(
					WS_CHILD|WS_VISIBLE|SBS_VERT|SBS_RIGHTALIGN,
					CRect(0,0,0,0),
					((CWnd*)this),
					1
					)
				)
			{
				ASSERT( FALSE );
				return false;
			}
		}
		if( m_wndScrollBarH.GetSafeHwnd() == NULL )
		{
			if( ! m_wndScrollBarH.Create(
					WS_CHILD|WS_VISIBLE|SBS_HORZ|SBS_BOTTOMALIGN,
					CRect(0,0,0,0),
					((CWnd*)this),
					2
					)
				)
			{
				ASSERT( FALSE );
				return false;
			}
		}
		m_wndScrollBarH.SyncReservedSpace( &m_wndScrollBarV );
		m_wndScrollBarV.SyncReservedSpace( &m_wndScrollBarH );
		return true;
	}
	virtual CScrollBar * GetScrollBarCtrl( int nBar ) const
	{
		ASSERT_VALID( this );
		switch( nBar )
		{
		case SB_HORZ:
		case SB_VERT:
			if( NSB_EnsureScrollBarsCreated() )
			{
				CExtScrollBar * pExtScrollBar = ( nBar == SB_HORZ ) ? (&m_wndScrollBarH) : (&m_wndScrollBarV);
				ASSERT_VALID( pExtScrollBar );
				ASSERT( pExtScrollBar->GetSafeHwnd() != NULL );
				//pExtScrollBar->m_bSendActionNotifications = false;
				return pExtScrollBar;
			}
		break;
		}
		return NULL;
	}
}; // class CExtNSB

/////////////////////////////////////////////////////////////////////////////
// CExtZoomScrollBar window

class __PROF_UIS_API CExtZoomScrollBar : public CExtScrollBar
{
// Construction
public:

	DECLARE_DYNCREATE( CExtZoomScrollBar );
	CExtZoomScrollBar();

public:

	//{{AFX_VIRTUAL(CExtZoomScrollBar)
	//}}AFX_VIRTUAL

public:
	virtual ~CExtZoomScrollBar();

protected:
	//{{AFX_MSG(CExtZoomScrollBar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtZoomScrollBar

/////////////////////////////////////////////////////////////////////////////
// class CExtNCSB_ScrollContainer

#define __EXT_NCSB_SCROLL_CONTAINER_CLASS_NAME _T("ProfUIS-NCSB_ScrollContainer")

class __PROF_UIS_API CExtNCSB_ScrollContainer : public CWnd
{
public:
	enum e_mode_t
	{
		__EM_MIDDLE_CONTAINER		= 0,
		__EM_CORNER_AREA			= 1,
		__EM_HORIZONTAL_SCROLL_BAR	= 2,
		__EM_VERTICAL_SCROLL_BAR	= 3,
		__EM_MIN_VALUE				= 0,
		__EM_MAX_VALUE				= 3,
	};
protected:
	mutable HWND m_hWndScrollingTaget;
	e_mode_t m_eMode;
	mutable CExtScrollBar * m_pWndScrollBar;
public:
	CExtNCSB_ScrollContainer(
		HWND hWndScrollingTaget,
		e_mode_t eMode
		);
	virtual ~CExtNCSB_ScrollContainer();
	HWND GetSafeScrollingTargetHWND() const;
	CExtScrollBar * GetScrollBarInContainer() const;
	e_mode_t GetMode() const;
	virtual CExtScrollBar * OnInstantiateAndCreateScrollBar( bool bChildMode );
	virtual bool Create( CWnd * pWndParent );
protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
}; /// class CExtNCSB_ScrollContainer

/////////////////////////////////////////////////////////////////////////////
// template class CExtNCSB_Impl
// skin scroll bars of any window with non-client area scroll bars

template < class _BTNCSBimpl >
class CExtNCSB_Impl : public _BTNCSBimpl
{
protected:
	mutable HWND m_hWndNcsbContainerHorizontal, m_hWndNcsbContainerVertical, m_hWndNcsbContainerCorner, m_hWndNcsbContainerMiddle;
	mutable CExtNCSB_ScrollContainer * m_pNcsbContainerHorizontal, * m_pNcsbContainerVertical, * m_pNcsbContainerCorner, * m_pNcsbContainerMiddle;
	bool m_bNcsbHelperDestructionMode:1, m_bNcsbFirstPaintMessagePassed:1;
public:
	CPoint m_ptNcsbProtectMouseMoveLast;
	bool m_bNcsbUse32BitScrollInfo:1, m_bNcsbForceMiddleContainerMode:1,
		m_bNcsbProtectMessageLoopFromMfcToolHitTesting:1, m_bNcsbDelayedInitialization:1,
		m_bNcsbForceRedrawOnMouseWheel:1, m_bNcsbCreatingContainers:1,
		m_bNcsbUpdateTargetWindowScrollInfoH:1, m_bNcsbUpdateTargetWindowScrollInfoV:1,
		m_bNcsbUpdateTargetWindowScrollPosH:1, m_bNcsbUpdateTargetWindowScrollPosV:1;
	CRect m_rcNcsbCustomNcAreaSizes;
	COLORREF m_clrNcsbCustomNcAreaFill;
	CExtNCSB_Impl(
			bool bNcsbDelayedInitialization = false,
			bool bNcsbForceMiddleContainerMode = false
			)
		: m_pNcsbContainerHorizontal( NULL )
		, m_pNcsbContainerVertical( NULL )
		, m_pNcsbContainerCorner( NULL )
		, m_pNcsbContainerMiddle( NULL )
		, m_hWndNcsbContainerHorizontal( NULL )
		, m_hWndNcsbContainerVertical( NULL )
		, m_hWndNcsbContainerCorner( NULL )
		, m_hWndNcsbContainerMiddle( NULL )
		, m_bNcsbUse32BitScrollInfo( true )
		, m_bNcsbHelperDestructionMode( false )
		, m_bNcsbDelayedInitialization( bNcsbDelayedInitialization )
		, m_bNcsbForceMiddleContainerMode( bNcsbForceMiddleContainerMode )
		, m_bNcsbProtectMessageLoopFromMfcToolHitTesting( true )
		, m_ptNcsbProtectMouseMoveLast( -32767, -32767 )
		, m_rcNcsbCustomNcAreaSizes( -1, -1, -1, -1 )
		, m_clrNcsbCustomNcAreaFill( COLORREF(-1L) )
		, m_bNcsbFirstPaintMessagePassed( false )
		, m_bNcsbForceRedrawOnMouseWheel( false )
		, m_bNcsbCreatingContainers( false )
		, m_bNcsbUpdateTargetWindowScrollInfoH( false )
		, m_bNcsbUpdateTargetWindowScrollInfoV( false )
		, m_bNcsbUpdateTargetWindowScrollPosH( false )
		, m_bNcsbUpdateTargetWindowScrollPosV( false )
	{
	}
	CExtNCSB_Impl(
		UINT nIDTemplate,
		CWnd * pParentWnd,
		bool bNcsbDelayedInitialization = false,
		bool bNcsbForceMiddleContainerMode = false
		)
		: _BTNCSBimpl( nIDTemplate, pParentWnd )
		, m_pNcsbContainerHorizontal( NULL )
		, m_pNcsbContainerVertical( NULL )
		, m_pNcsbContainerCorner( NULL )
		, m_pNcsbContainerMiddle( NULL )
		, m_hWndNcsbContainerHorizontal( NULL )
		, m_hWndNcsbContainerVertical( NULL )
		, m_hWndNcsbContainerCorner( NULL )
		, m_hWndNcsbContainerMiddle( NULL )
		, m_bNcsbUse32BitScrollInfo( true )
		, m_bNcsbHelperDestructionMode( false )
		, m_bNcsbDelayedInitialization( bNcsbDelayedInitialization )
		, m_bNcsbForceMiddleContainerMode( bNcsbForceMiddleContainerMode )
		, m_bNcsbProtectMessageLoopFromMfcToolHitTesting( true )
		, m_ptNcsbProtectMouseMoveLast( -32767, -32767 )
		, m_rcNcsbCustomNcAreaSizes( -1, -1, -1, -1 )
		, m_clrNcsbCustomNcAreaFill( COLORREF(-1L) )
		, m_bNcsbFirstPaintMessagePassed( false )
		, m_bNcsbForceRedrawOnMouseWheel( false )
		, m_bNcsbCreatingContainers( false )
	{
	}
	CExtNCSB_Impl(
		__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
		CWnd * pParentWnd,
		bool bNcsbDelayedInitialization = false,
		bool bNcsbForceMiddleContainerMode = false
		)
		: _BTNCSBimpl( lpszTemplateName, pParentWnd )
		, m_pNcsbContainerHorizontal( NULL )
		, m_pNcsbContainerVertical( NULL )
		, m_pNcsbContainerCorner( NULL )
		, m_pNcsbContainerMiddle( NULL )
		, m_hWndNcsbContainerHorizontal( NULL )
		, m_hWndNcsbContainerVertical( NULL )
		, m_hWndNcsbContainerCorner( NULL )
		, m_hWndNcsbContainerMiddle( NULL )
		, m_bNcsbUse32BitScrollInfo( true )
		, m_bNcsbHelperDestructionMode( false )
		, m_bNcsbDelayedInitialization( bNcsbDelayedInitialization )
		, m_bNcsbForceMiddleContainerMode( bNcsbForceMiddleContainerMode )
		, m_bNcsbProtectMessageLoopFromMfcToolHitTesting( true )
		, m_ptNcsbProtectMouseMoveLast( -32767, -32767 )
		, m_rcNcsbCustomNcAreaSizes( -1, -1, -1, -1 )
		, m_clrNcsbCustomNcAreaFill( COLORREF(-1L) )
		, m_bNcsbFirstPaintMessagePassed( false )
		, m_bNcsbForceRedrawOnMouseWheel( false )
		, m_bNcsbCreatingContainers( false )
	{
	}
	virtual ~CExtNCSB_Impl()
	{
		NCSB_EnsureContainersDestroyed();
	}
	virtual bool NCSB_IsDestructionMode() const
	{
		ASSERT_VALID( this );
		return m_bNcsbHelperDestructionMode;
	}
	virtual bool NCSB_IsMiddleContainerRequired() const
	{
		ASSERT_VALID( this );
		if( m_bNcsbForceMiddleContainerMode )
			return true;
		return false;
	}
	virtual CWnd * NCSB_GetParentWindow()
	{
		ASSERT_VALID( this );
		if( GetSafeHwnd() == NULL )
			return NULL;
		DWORD dwStyle = GetStyle();
		if( (dwStyle&WS_CHILD) == 0 )
			return this;
		CWnd * pWndParent = GetParent();
		if( pWndParent->GetSafeHwnd() == ::GetDesktopWindow() )
			return this;
		return pWndParent;
	}
	virtual bool NSCB_IsLeftScrollBarMode() const
	{
		ASSERT_VALID( this );
		DWORD dwStyleEx = GetExStyle();
		if( (dwStyleEx&WS_EX_LEFTSCROLLBAR) != 0 )
			return true;
		else
			return false;
	}
	virtual CExtNCSB_ScrollContainer * NCSB_GetContainer( CExtNCSB_ScrollContainer::e_mode_t eMode ) const
	{
		ASSERT_VALID( this );
		ASSERT( INT(CExtNCSB_ScrollContainer::__EM_MIN_VALUE) <= INT(eMode) && INT(eMode) <= INT(CExtNCSB_ScrollContainer::__EM_MAX_VALUE) );
		CExtNCSB_ScrollContainer * pWnd = NULL;
		HWND hWnd = NULL;
		switch( eMode )
		{
		case CExtNCSB_ScrollContainer::__EM_MIDDLE_CONTAINER:
			pWnd = m_pNcsbContainerMiddle;
			hWnd = m_hWndNcsbContainerMiddle;
		break;
		case CExtNCSB_ScrollContainer::__EM_CORNER_AREA:
			pWnd = m_pNcsbContainerCorner;
			hWnd = m_hWndNcsbContainerCorner;
		break;
		case CExtNCSB_ScrollContainer::__EM_HORIZONTAL_SCROLL_BAR:
			pWnd = m_pNcsbContainerHorizontal;
			hWnd = m_hWndNcsbContainerHorizontal;
		break;
		case CExtNCSB_ScrollContainer::__EM_VERTICAL_SCROLL_BAR:
			pWnd = m_pNcsbContainerVertical;
			hWnd = m_hWndNcsbContainerVertical;
		break;
		}
		if(		pWnd == NULL
			||	hWnd == NULL
			||	(! ::IsWindow( hWnd ) )
			||	LPVOID( CWnd::FromHandlePermanent( hWnd ) ) != LPVOID( pWnd )
			)
			return NULL;
		return pWnd;
	}
	virtual CExtNCSB_ScrollContainer * NCSB_InstantiateAndCreateContainer(
		CExtNCSB_ScrollContainer::e_mode_t eMode,
		CWnd * pWndParent
		)
	{
		ASSERT_VALID( this );
		if( NCSB_IsDestructionMode() || GetSafeHwnd() == NULL || pWndParent->GetSafeHwnd() == NULL )
			return NULL;
		ASSERT_VALID( pWndParent );
		CExtNCSB_ScrollContainer * pWnd =
			new CExtNCSB_ScrollContainer(
				m_hWnd,
				eMode
				);
		if( ! pWnd->Create( pWndParent ) )
			return NULL;
		return pWnd;
	}
	virtual bool NCSB_EnsureContainersCreated()
	{
		ASSERT_VALID( this );
		if( ! g_PaintManager.m_bAutoSkinScrollBars )
			return false;
		if( NCSB_IsDestructionMode() || GetSafeHwnd() == NULL )
			return false;
		if( m_pNcsbContainerHorizontal != NULL || m_pNcsbContainerVertical != NULL || m_pNcsbContainerCorner != NULL )
		{
			ASSERT( m_pNcsbContainerHorizontal != NULL && m_pNcsbContainerVertical != NULL && m_pNcsbContainerCorner != NULL );
			ASSERT( m_hWndNcsbContainerHorizontal != NULL && m_hWndNcsbContainerVertical != NULL && m_hWndNcsbContainerCorner != NULL );
			return true;
		}
		CWnd * pWndParent = NCSB_GetParentWindow();
		if( pWndParent == NULL )
			return false;
		ASSERT_VALID( pWndParent );
		ASSERT( pWndParent->GetSafeHwnd() != NULL );
		if( m_bNcsbCreatingContainers )
			return false;
		m_bNcsbCreatingContainers = true;
		if( NCSB_IsMiddleContainerRequired() )
		{
			ASSERT( (GetStyle()&WS_CHILD) != 0 );
			if( m_pNcsbContainerMiddle == NULL )
			{
				m_pNcsbContainerMiddle = NCSB_InstantiateAndCreateContainer( CExtNCSB_ScrollContainer::__EM_MIDDLE_CONTAINER, pWndParent );
				m_hWndNcsbContainerMiddle = m_pNcsbContainerMiddle->GetSafeHwnd();
				if( m_pNcsbContainerMiddle == NULL )
				{
					m_bNcsbCreatingContainers = false;
					return false;
				}
			}
			pWndParent = m_pNcsbContainerMiddle;
			ASSERT( LPVOID(pWndParent) == LPVOID(NCSB_GetParentWindow()) );
		}
		m_pNcsbContainerHorizontal = NCSB_InstantiateAndCreateContainer( CExtNCSB_ScrollContainer::__EM_HORIZONTAL_SCROLL_BAR, pWndParent );
		m_hWndNcsbContainerHorizontal = m_pNcsbContainerHorizontal->GetSafeHwnd();
		if( m_pNcsbContainerHorizontal == NULL )
		{
			NCSB_EnsureContainersDestroyed();
			m_pNcsbContainerHorizontal = m_pNcsbContainerVertical = m_pNcsbContainerCorner = m_pNcsbContainerMiddle = NULL;
			m_hWndNcsbContainerHorizontal = m_hWndNcsbContainerVertical = m_hWndNcsbContainerCorner = m_hWndNcsbContainerMiddle = NULL;
			m_bNcsbCreatingContainers = false;
			return false;
		}
		m_pNcsbContainerVertical = NCSB_InstantiateAndCreateContainer( CExtNCSB_ScrollContainer::__EM_VERTICAL_SCROLL_BAR, pWndParent );
		m_hWndNcsbContainerVertical = m_pNcsbContainerVertical->GetSafeHwnd();
		if( m_pNcsbContainerVertical == NULL )
		{
			NCSB_EnsureContainersDestroyed();
			m_pNcsbContainerHorizontal = m_pNcsbContainerVertical = m_pNcsbContainerCorner = m_pNcsbContainerMiddle = NULL;
			m_hWndNcsbContainerHorizontal = m_hWndNcsbContainerVertical = m_hWndNcsbContainerCorner = m_hWndNcsbContainerMiddle = NULL;
			m_bNcsbCreatingContainers = false;
			return false;
		}
		m_pNcsbContainerCorner = NCSB_InstantiateAndCreateContainer( CExtNCSB_ScrollContainer::__EM_CORNER_AREA, pWndParent );
		m_hWndNcsbContainerCorner = m_pNcsbContainerCorner->GetSafeHwnd();
		if( m_pNcsbContainerCorner == NULL )
		{
			NCSB_EnsureContainersDestroyed();
			m_pNcsbContainerHorizontal = m_pNcsbContainerVertical = m_pNcsbContainerCorner =  m_pNcsbContainerMiddle = NULL;
			m_hWndNcsbContainerHorizontal = m_hWndNcsbContainerVertical = m_hWndNcsbContainerCorner = m_hWndNcsbContainerMiddle = NULL;
			m_bNcsbCreatingContainers = false;
			return false;
		}
		m_bNcsbCreatingContainers = false;
		return true;
	}
	virtual void NCSB_EnsureContainersDestroyed()
	{
		ASSERT_VALID( this );
		CExtNCSB_ScrollContainer * pWnd;
		pWnd = NCSB_GetContainer( CExtNCSB_ScrollContainer::__EM_HORIZONTAL_SCROLL_BAR );
		if( pWnd->GetSafeHwnd() != NULL )
			pWnd->DestroyWindow();
		pWnd = NCSB_GetContainer( CExtNCSB_ScrollContainer::__EM_VERTICAL_SCROLL_BAR );
		if( pWnd->GetSafeHwnd() != NULL )
			pWnd->DestroyWindow();
		pWnd = NCSB_GetContainer( CExtNCSB_ScrollContainer::__EM_CORNER_AREA );
		if( pWnd->GetSafeHwnd() != NULL )
			pWnd->DestroyWindow();
		pWnd = NCSB_GetContainer( CExtNCSB_ScrollContainer::__EM_MIDDLE_CONTAINER );
		if( pWnd->GetSafeHwnd() != NULL )
			pWnd->PostMessage( WM_CLOSE );
	}
	virtual INT NCSB_GetScrollBarExtent( bool bHorizontal )
	{
		ASSERT_VALID( this );
		INT nExtent = (INT)::GetSystemMetrics( bHorizontal ? SM_CYHSCROLL : SM_CXVSCROLL );
		return nExtent;
	}
	virtual void NCSB_GetScrollInfoWindow( bool bHorizontal, SCROLLINFO & _scroll_info )
	{
		ASSERT_VALID( this );
		::memset( &_scroll_info, 0, sizeof(SCROLLINFO) );
		_scroll_info.cbSize = sizeof(SCROLLINFO);
		if( NCSB_IsDestructionMode() || GetSafeHwnd() == NULL )
			return;
		int nBar = bHorizontal ? SB_HORZ : SB_VERT;
		if( m_bNcsbUse32BitScrollInfo )
		{
			if(	((CWnd*)(this)) ->
					CWnd::GetScrollInfo(
						nBar,
						&_scroll_info,
						SIF_RANGE|SIF_PAGE|SIF_POS|SIF_TRACKPOS
						)
				)
				return;
		} // if( m_bNcsbUse32BitScrollInfo )
		INT nMin = 0, nMax = 0;
		CWnd::GetScrollRange( nBar, &nMin, &nMax );
		_scroll_info.nMin = nMin;
		_scroll_info.nMax = nMax;
		_scroll_info.nPos = _scroll_info.nTrackPos = CWnd::GetScrollPos( nBar );
	}
	virtual void NCSB_SetScrollInfoWindow( bool bHorizontal, SCROLLINFO & _scroll_info )
	{
		ASSERT_VALID( this );
		::memset( &_scroll_info, 0, sizeof(SCROLLINFO) );
		_scroll_info.cbSize = sizeof(SCROLLINFO);
		if( NCSB_IsDestructionMode() || GetSafeHwnd() == NULL )
			return;
		int nBar = bHorizontal ? SB_HORZ : SB_VERT;
		if( m_bNcsbUse32BitScrollInfo )
		{
			if(	((CWnd*)(this)) ->
					CWnd::SetScrollInfo(
						nBar,
						&_scroll_info
						)
				)
				return;
		} // if( m_bNcsbUse32BitScrollInfo )
		if( ( _scroll_info.fMask & SIF_RANGE ) != 0 )
			CWnd::SetScrollRange( nBar, _scroll_info.nMin, _scroll_info.nMax );
		if( ( _scroll_info.fMask & SIF_POS ) != 0 )
			CWnd::SetScrollPos( nBar, _scroll_info.nPos );
	}
	virtual void NCSB_GetScrollInfoBar( CExtScrollBar * pScrollBar, SCROLLINFO & _scroll_info )
	{
		ASSERT_VALID( this );
		::memset( &_scroll_info, 0, sizeof(SCROLLINFO) );
		_scroll_info.cbSize = sizeof(SCROLLINFO);
		if( NCSB_IsDestructionMode() || GetSafeHwnd() == NULL )
			return;
		ASSERT_VALID( pScrollBar );
		ASSERT( pScrollBar->GetSafeHwnd() != NULL );
		if( m_bNcsbUse32BitScrollInfo )
		{
			if(	pScrollBar->GetScrollInfo(
					&_scroll_info,
					SIF_RANGE|SIF_PAGE|SIF_POS|SIF_TRACKPOS
					)
				)
				return;
		} // if( m_bNcsbUse32BitScrollInfo )
		INT nMin = 0, nMax = 0;
		pScrollBar->GetScrollRange( &nMin, &nMax );
		_scroll_info.nMin = nMin;
		_scroll_info.nMax = nMax;
		_scroll_info.nPos = _scroll_info.nTrackPos = pScrollBar->GetScrollPos();
	}
	virtual void NCSB_UpdateChannelPos( bool bHorizontal, bool bTrackPos )
	{
		ASSERT_VALID( this );
		if( NCSB_IsDestructionMode() || GetSafeHwnd() == NULL )
			return;
		CExtNCSB_ScrollContainer * pWnd =
			NCSB_GetContainer(
				bHorizontal
					? CExtNCSB_ScrollContainer::__EM_HORIZONTAL_SCROLL_BAR
					: CExtNCSB_ScrollContainer::__EM_VERTICAL_SCROLL_BAR
				);
		if( pWnd == NULL )
			return;
		ASSERT( pWnd->GetSafeHwnd() != NULL );
		CExtScrollBar * pScrollBar = pWnd->GetScrollBarInContainer();
		if( pScrollBar == NULL )
			return;
		ASSERT( pScrollBar->GetSafeHwnd() != NULL );
		int nBar = bHorizontal ? SB_HORZ : SB_VERT;
		INT nPos = (INT)pScrollBar->_GetScrollPos( bTrackPos );
		CWnd::SetScrollPos( nBar, nPos );
	}
	virtual void NCSB_UpdateChannel( bool bHorizontal )
	{
		ASSERT_VALID( this );
		if( NCSB_IsDestructionMode() || GetSafeHwnd() == NULL )
			return;
		CExtNCSB_ScrollContainer * pWnd =
			NCSB_GetContainer(
				bHorizontal
					? CExtNCSB_ScrollContainer::__EM_HORIZONTAL_SCROLL_BAR
					: CExtNCSB_ScrollContainer::__EM_VERTICAL_SCROLL_BAR
				);
		if( pWnd == NULL )
			return;
		ASSERT( pWnd->GetSafeHwnd() != NULL );
		CExtScrollBar * pScrollBar = pWnd->GetScrollBarInContainer();
		if( pScrollBar == NULL )
			return;
		ASSERT( pScrollBar->GetSafeHwnd() != NULL );
		BOOL bWindowEnabled = IsWindowEnabled();
		pWnd->EnableWindow( bWindowEnabled );
		SCROLLINFO _scroll_info_window, _scroll_info_bar;
		::memset( &_scroll_info_window, 0, sizeof(SCROLLINFO) );
		::memset( &_scroll_info_bar, 0, sizeof(SCROLLINFO) );
		_scroll_info_window.cbSize = sizeof(SCROLLINFO);
		_scroll_info_bar.cbSize = sizeof(SCROLLINFO);
		if( bWindowEnabled )
		{
			NCSB_GetScrollInfoWindow( bHorizontal, _scroll_info_window );
			NCSB_GetScrollInfoBar( pScrollBar, _scroll_info_bar );
		}
		//_scroll_info_window.fMask = _scroll_info_bar.fMask = SIF_POS|SIF_TRACKPOS;
		//_scroll_info_window.nMin = _scroll_info_window.nMax = _scroll_info_window.nPage
		//	= _scroll_info_bar.nMin = _scroll_info_bar.nMax = _scroll_info_bar.nPage
		//	= 0;
		if( ::memcmp( &_scroll_info_window, &_scroll_info_bar, sizeof(SCROLLINFO) ) != 0 )
			NCSB_SetScrollInfoWindow( bHorizontal, _scroll_info_bar );
	}
	virtual void NCSB_RescanChannel( bool bHorizontal )
	{
		ASSERT_VALID( this );
		if( NCSB_IsDestructionMode() || GetSafeHwnd() == NULL )
			return;
		CExtNCSB_ScrollContainer * pWnd =
			NCSB_GetContainer(
				bHorizontal
					? CExtNCSB_ScrollContainer::__EM_HORIZONTAL_SCROLL_BAR
					: CExtNCSB_ScrollContainer::__EM_VERTICAL_SCROLL_BAR
				);
		if( pWnd == NULL )
			return;
		ASSERT( pWnd->GetSafeHwnd() != NULL );
		CExtScrollBar * pScrollBar = pWnd->GetScrollBarInContainer();
		if( pScrollBar == NULL )
			return;
		ASSERT( pScrollBar->GetSafeHwnd() != NULL );
		BOOL bWindowEnabled = IsWindowEnabled();
		pWnd->EnableWindow( bWindowEnabled );
		SCROLLINFO _scroll_info_window, _scroll_info_bar;
		::memset( &_scroll_info_window, 0, sizeof(SCROLLINFO) );
		::memset( &_scroll_info_bar, 0, sizeof(SCROLLINFO) );
		_scroll_info_window.cbSize = sizeof(SCROLLINFO);
		_scroll_info_bar.cbSize = sizeof(SCROLLINFO);
		if( bWindowEnabled )
		{
			NCSB_GetScrollInfoWindow( bHorizontal, _scroll_info_window );
			NCSB_GetScrollInfoBar( pScrollBar, _scroll_info_bar );
		}
		if( ::memcmp( &_scroll_info_window, &_scroll_info_bar, sizeof(SCROLLINFO) ) != 0 )
		{
			if( m_bNcsbUse32BitScrollInfo )
				pScrollBar->SetScrollInfo( &_scroll_info_window, FALSE );
			else
			{
				pScrollBar->SetScrollRange( _scroll_info_window.nMin, _scroll_info_window.nMax, FALSE );
				pScrollBar->SetScrollPos( _scroll_info_window.nPos, FALSE );
			}
		}
		BOOL bEnable =
			(	bWindowEnabled
			&&	( _scroll_info_window.nMin < _scroll_info_window.nMax )
			&&	(	_scroll_info_window.nPage == 0
				||	INT( _scroll_info_window.nMax - _scroll_info_window.nMin + 1 ) > INT(_scroll_info_window.nPage)
				)
			) ? TRUE : FALSE;
		pScrollBar->EnableWindow( bEnable );
		pScrollBar->Invalidate();
		pScrollBar->UpdateWindow();
	}
	virtual void NCSB_RescanContainers()
	{
		ASSERT_VALID( this );
		if( NCSB_IsDestructionMode() || GetSafeHwnd() == NULL )
			return;
		NCSB_RescanChannel( true );
		NCSB_RescanChannel( false );
		CExtNCSB_ScrollContainer * pWndC = NCSB_GetContainer( CExtNCSB_ScrollContainer::__EM_CORNER_AREA );
		if( pWndC != NULL )
		{
			ASSERT( pWndC->GetSafeHwnd() != NULL );
			pWndC->Invalidate();
			pWndC->UpdateWindow();
		}
	}
	virtual bool NCSB_RepositionContainers( bool bRescanScrollPositions = true )
	{
		ASSERT_VALID( this );
		if( NCSB_IsDestructionMode() || GetSafeHwnd() == NULL )
			return false;
		CWnd * pWndParent = NCSB_GetParentWindow();
		if( pWndParent == NULL )
		{
			NCSB_EnsureContainersDestroyed();
			m_pNcsbContainerHorizontal = m_pNcsbContainerVertical = m_pNcsbContainerCorner =  m_pNcsbContainerMiddle = NULL;
			m_hWndNcsbContainerHorizontal = m_hWndNcsbContainerVertical = m_hWndNcsbContainerCorner = m_hWndNcsbContainerMiddle = NULL;
			return false;
		}
		ASSERT( pWndParent->GetSafeHwnd() != NULL );
		if( ! NCSB_EnsureContainersCreated() )
			return false;
		CExtNCSB_ScrollContainer * pWndH = NCSB_GetContainer( CExtNCSB_ScrollContainer::__EM_HORIZONTAL_SCROLL_BAR );
		CExtNCSB_ScrollContainer * pWndV = NCSB_GetContainer( CExtNCSB_ScrollContainer::__EM_VERTICAL_SCROLL_BAR );
		CExtNCSB_ScrollContainer * pWndC = NCSB_GetContainer( CExtNCSB_ScrollContainer::__EM_CORNER_AREA );
		CRect rcClient, rcLarger;
		GetClientRect( &rcClient );
		rcLarger = rcClient;
		bool bShowH = false, bShowV = false;
		DWORD dwStyle = GetStyle();
		bool bLeftScrollBarMode = NSCB_IsLeftScrollBarMode();
		if( (dwStyle&WS_HSCROLL) != 0 )
		{
			if( (dwStyle&WS_VISIBLE) != 0 )
				bShowH = true;
			rcLarger.bottom += NCSB_GetScrollBarExtent( true );
		}
		if( (dwStyle&WS_VSCROLL) != 0 )
		{
			if( (dwStyle&WS_VISIBLE) != 0 )
				bShowV = true;
			if( bLeftScrollBarMode )
				rcLarger.left -= NCSB_GetScrollBarExtent( false );
			else
				rcLarger.right += NCSB_GetScrollBarExtent( false );
		}
		const CWnd * pWndInsertAfter = &CWnd::wndTop;
		bool bChildMode = ( (dwStyle&WS_CHILD) != 0 ) ? true : false;
		if( bChildMode && ::GetParent(m_hWnd) == ::GetDesktopWindow() )
			bChildMode = false;
		UINT nBasecSetWindowPosFlags = SWP_NOCOPYBITS|SWP_NOOWNERZORDER;
		if( ! bChildMode )
		{
			pWndInsertAfter = &wndTopMost; // pWndParent;
			nBasecSetWindowPosFlags |= SWP_NOACTIVATE;
		}
		bool bShowC = ( bShowH && bShowV ) ? true : false;
		bool bVisibleH = false, bVisibleV = false, bVisibleC = false;
		if( pWndH != NULL )
		{
			ASSERT( pWndH->GetSafeHwnd() != NULL );
			if( (pWndH->GetStyle()&WS_VISIBLE) != 0 )
				bVisibleH = true;
		}
		if( pWndV != NULL )
		{
			ASSERT( pWndV->GetSafeHwnd() != NULL );
			if( (pWndV->GetStyle()&WS_VISIBLE) != 0 )
				bVisibleV = true;
		}
		if( pWndC != NULL )
		{
			ASSERT( pWndC->GetSafeHwnd() != NULL );
			if( (pWndC->GetStyle()&WS_VISIBLE) != 0 )
				bVisibleC = true;
		}
		bool bAnyMoved = false;
		if( pWndH != NULL )
		{
			ASSERT( pWndH->GetSafeHwnd() != NULL );
			CRect rcReal, rc(
				rcClient.left,
				rcClient.bottom,
				rcClient.right,
				rcLarger.bottom
				);
			ClientToScreen( &rc );
			pWndH->GetWindowRect( &rcReal );
			if( bVisibleH != bShowH || rcReal != rc )
			{
				bAnyMoved = true;
				if( bChildMode )
					pWndParent->ScreenToClient( &rc );
				pWndH->SetWindowPos(
					pWndInsertAfter, rc.left, rc.top, rc.Width(), rc.Height(),
					nBasecSetWindowPosFlags | ( bShowH ? SWP_SHOWWINDOW : SWP_HIDEWINDOW )
					);
			} // if( rcReal != rc )
		}
		if( pWndV != NULL )
		{
			ASSERT( pWndV->GetSafeHwnd() != NULL );
			CRect rcReal, rc(
				bLeftScrollBarMode ? rcLarger.left : rcClient.right,
				rcClient.top,
				bLeftScrollBarMode ? rcClient.left : rcLarger.right,
				rcClient.bottom
				);
			ClientToScreen( &rc );
			pWndV->GetWindowRect( &rcReal );
			if( bVisibleV != bShowV || rcReal != rc )
			{
				bAnyMoved = true;
				if( bChildMode )
					pWndParent->ScreenToClient( &rc );
				pWndV->SetWindowPos(
					pWndInsertAfter, rc.left, rc.top, rc.Width(), rc.Height(),
					nBasecSetWindowPosFlags | ( bShowV ? SWP_SHOWWINDOW : SWP_HIDEWINDOW )
					);
			} // if( rcReal != rc )
		}
		if( pWndC != NULL )
		{
			ASSERT( pWndC->GetSafeHwnd() != NULL );
			CRect rcReal, rc(
				bLeftScrollBarMode ? rcLarger.left : rcClient.right,
				rcClient.bottom,
				bLeftScrollBarMode ? rcClient.left : rcLarger.right,
				rcLarger.bottom
				);
			ClientToScreen( &rc );
			pWndC->GetWindowRect( &rcReal );
			if( bVisibleC != bShowC || rcReal != rc )
			{
				bAnyMoved = true;
				if( bChildMode )
					pWndParent->ScreenToClient( &rc );
				pWndC->SetWindowPos(
					pWndInsertAfter, rc.left, rc.top, rc.Width(), rc.Height(),
					nBasecSetWindowPosFlags | ( bShowC ? SWP_SHOWWINDOW : SWP_HIDEWINDOW )
					);
			} // if( rcReal != rc )
		}
		if( bRescanScrollPositions )
			NCSB_RescanContainers();
		return bAnyMoved;
	}
	virtual bool NSCB_MessageCanAffectScrollBars( UINT message ) const
	{
		ASSERT_VALID( this );
		if(		( __EXT_MFC_WM_MOUSEFIRST <= message && message <= __EXT_MFC_WM_MOUSELAST )
			||	( WM_KEYFIRST <= message && message <= WM_KEYLAST )
			)
			return true;
		return false;
	}
protected:
	virtual void PreSubclassWindow()
	{
		m_bNcsbHelperDestructionMode = false;
		_BTNCSBimpl::PreSubclassWindow();
		TCHAR szCompare[512] = _T("");
		::GetClassName(
			m_hWnd,
			szCompare,
			sizeof( szCompare )/sizeof( szCompare[0] )
			);
		if( ! m_bNcsbForceRedrawOnMouseWheel )
		{
			if(		_tcsicmp( szCompare, _T("LISTBOX") ) == 0
				||	_tcsicmp( szCompare, _T("COMBOLBOX") ) == 0
				)
				m_bNcsbForceRedrawOnMouseWheel = true;
		}
		if(		g_PaintManager.m_bIsWinVistaOrLater
			&&	_tcsicmp( szCompare, _T("SysTreeView32") ) == 0
			)
			m_bNcsbUpdateTargetWindowScrollPosH = m_bNcsbUpdateTargetWindowScrollPosV = true;
		if( ( GetStyle() & (WS_CLIPCHILDREN|WS_CLIPSIBLINGS) ) != (WS_CLIPCHILDREN|WS_CLIPSIBLINGS) )
			ModifyStyle( 0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS );
		if( ! m_bNcsbDelayedInitialization )
			NCSB_RepositionContainers();
	}
	virtual void PostNcDestroy()
	{
		m_bNcsbHelperDestructionMode = true;
		m_bNcsbFirstPaintMessagePassed = true;
		//NCSB_EnsureContainersDestroyed();
		m_pNcsbContainerHorizontal = m_pNcsbContainerVertical = m_pNcsbContainerCorner = m_pNcsbContainerMiddle = NULL;
		m_hWndNcsbContainerHorizontal = m_hWndNcsbContainerVertical = m_hWndNcsbContainerCorner = m_hWndNcsbContainerMiddle = NULL;
		m_ptNcsbProtectMouseMoveLast.x = m_ptNcsbProtectMouseMoveLast.y = -32767;
		_BTNCSBimpl::PostNcDestroy();
	}
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
	{
		if(		( ! g_PaintManager.m_bAutoSkinScrollBars )
			||	(	(! m_bNcsbFirstPaintMessagePassed)
					&&	message != WM_ERASEBKGND
					&&	message != WM_PAINT
					&&	message != WM_NCPAINT
				)
			)
			return _BTNCSBimpl::WindowProc( message, wParam, lParam );;
		switch( message )
		{
		case WM_NCCALCSIZE:
			if(		m_rcNcsbCustomNcAreaSizes.left >= 0
				&&	m_rcNcsbCustomNcAreaSizes.top >= 0
				&&	m_rcNcsbCustomNcAreaSizes.right >= 0
				&&	m_rcNcsbCustomNcAreaSizes.bottom >= 0
				)
			{
				NCCALCSIZE_PARAMS * pNCCSP =
					reinterpret_cast < NCCALCSIZE_PARAMS * > ( lParam );
				ASSERT( pNCCSP != NULL );
				CRect rcCalcClient( pNCCSP->rgrc[0] );
				rcCalcClient.DeflateRect( &m_rcNcsbCustomNcAreaSizes );
				DWORD dwStyle = GetStyle();
				bool bLeftScrollBarMode = NSCB_IsLeftScrollBarMode();
				if( (dwStyle&WS_HSCROLL) != 0 )
					rcCalcClient.bottom -= NCSB_GetScrollBarExtent( true );
				if( (dwStyle&WS_VSCROLL) != 0 )
				{
					if( bLeftScrollBarMode )
						rcCalcClient.left += NCSB_GetScrollBarExtent( false );
					else
						rcCalcClient.right -= NCSB_GetScrollBarExtent( false );
				}
				::CopyRect( &(pNCCSP->rgrc[0]), rcCalcClient );
				return 0L;
			}
		break;
		case WM_NCPAINT:
			m_bNcsbFirstPaintMessagePassed = true;
			if(		m_rcNcsbCustomNcAreaSizes.left >= 0
				&&	m_rcNcsbCustomNcAreaSizes.top >= 0
				&&	m_rcNcsbCustomNcAreaSizes.right >= 0
				&&	m_rcNcsbCustomNcAreaSizes.bottom >= 0
				)
			{
				CRect rcInBarWnd, rcInBarClient;
				GetWindowRect( &rcInBarWnd );
				GetClientRect( &rcInBarClient );
				ClientToScreen( &rcInBarClient );
				if( rcInBarWnd == rcInBarClient )
					return 0;
				CPoint ptDevOffset = -rcInBarWnd.TopLeft();
				rcInBarWnd.OffsetRect( ptDevOffset );
				rcInBarClient.OffsetRect( ptDevOffset );
				CWindowDC dcWindow( this );
				dcWindow.ExcludeClipRect( &rcInBarClient );
				CExtMemoryDC dc( &dcWindow );
				if( m_clrNcsbCustomNcAreaFill != COLORREF(-1L) )
					dc.FillSolidRect( &rcInBarWnd, m_clrNcsbCustomNcAreaFill );
				else if( ! g_PaintManager->PaintDockerBkgnd( true, dc, this ) )
					dc.FillSolidRect( &rcInBarWnd, g_PaintManager->GetColor( COLOR_3DDKSHADOW ) );
				dc.__Flush();
				NCSB_RepositionContainers();
				return 0;
			}
			NCSB_RepositionContainers();
		break;
		case WM_MOUSEMOVE:
			if( m_bNcsbProtectMessageLoopFromMfcToolHitTesting )
			{
				CPoint point( short(LOWORD(DWORD(lParam))), short(HIWORD(DWORD(lParam))) );
				if( m_ptNcsbProtectMouseMoveLast == point )
					return 0;
				m_ptNcsbProtectMouseMoveLast = point;
			}
		break;
		case WM_WINDOWPOSCHANGED:
		case WM_SHOWWINDOW:
		case WM_SIZE:
		case WM_ENABLE:
			NCSB_RepositionContainers();
		break;
		case WM_DESTROY:
			m_bNcsbHelperDestructionMode = true;
			NCSB_EnsureContainersDestroyed();
			m_pNcsbContainerHorizontal = m_pNcsbContainerVertical = m_pNcsbContainerCorner = m_pNcsbContainerMiddle = NULL;
			m_hWndNcsbContainerHorizontal = m_hWndNcsbContainerVertical = m_hWndNcsbContainerCorner = m_hWndNcsbContainerMiddle = NULL;
		break;
		default:
			if( NSCB_MessageCanAffectScrollBars( message ) )
			{
				if( NCSB_RepositionContainers() )
					Invalidate();
			}
		break;
		case WM_PAINT:
		case WM_ERASEBKGND:
			m_bNcsbFirstPaintMessagePassed = true;
		break;
		} // switch( message )
		LRESULT lResult = _BTNCSBimpl::WindowProc( message, wParam, lParam );
		switch( message )
		{
		case WM_HSCROLL:
			NCSB_RescanContainers();
			if( m_bNcsbUpdateTargetWindowScrollInfoH )
				NCSB_UpdateChannel( true );
			else if( m_bNcsbUpdateTargetWindowScrollPosH )
				NCSB_UpdateChannelPos( true, true );
		break;
		case WM_VSCROLL:
			NCSB_RescanContainers();
			if( m_bNcsbUpdateTargetWindowScrollInfoV )
				NCSB_UpdateChannel( false );
			else if( m_bNcsbUpdateTargetWindowScrollPosV )
				NCSB_UpdateChannelPos( false, true );
		break;
		case WM_PAINT:
			NCSB_RescanContainers();
		break;
		case WM_MOUSEWHEEL:
			if( m_bNcsbForceRedrawOnMouseWheel )
			{
				Invalidate();
				UpdateWindow();
			}
		break;
		} // switch( message )
		return lResult;
	}
}; /// template class CExtNCSB_Impl

template < class _BTNCSB >
class CExtNCSB : public CExtNCSB_Impl < _BTNCSB >
{
public:
	CExtNCSB(
			bool bNcsbDelayedInitialization = false,
			bool bNcsbForceMiddleContainerMode = false
			)
			: CExtNCSB_Impl < _BTNCSB > (
				bNcsbDelayedInitialization,
				bNcsbForceMiddleContainerMode
				)
	{
	}
	CExtNCSB(
		UINT nIDTemplate,
		CWnd * pParentWnd,
		bool bNcsbDelayedInitialization = false,
		bool bNcsbForceMiddleContainerMode = false
		)
		: CExtNCSB_Impl < _BTNCSB > ( nIDTemplate, pParentWnd, bNcsbDelayedInitialization, bNcsbForceMiddleContainerMode )
	{
	}
	CExtNCSB(
		__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
		CWnd * pParentWnd,
		bool bNcsbDelayedInitialization = false,
		bool bNcsbForceMiddleContainerMode = false
		)
		: CExtNCSB_Impl < _BTNCSB > ( lpszTemplateName, pParentWnd, bNcsbDelayedInitialization, bNcsbForceMiddleContainerMode )
	{
	}
	virtual ~CExtNCSB()
	{
	}
}; /// template class CExtNCSB

template < > class CExtNCSB < CListCtrl > : public CExtNCSB_Impl < CListCtrl >
{
public:
	CExtNCSB(
		bool bNcsbDelayedInitialization = false,
		bool bNcsbForceMiddleContainerMode = false
		);
	virtual ~CExtNCSB();
protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
}; // template < > class CExtNCSB < CListCtrl >

#if (!defined __EXT_MFC_NO_SCROLLWND)

/////////////////////////////////////////////////////////////////////////////
// CExtScrollWnd window

// based on MFC source code in viewscrl.cpp
class __PROF_UIS_API CExtScrollWnd
	: public CWnd
	, public CExtMouseCaptureSink
	, public CExtPmBridge
{
public:
	DECLARE_DYNCREATE( CExtScrollWnd );
	DECLARE_CExtPmBridge_MEMBERS( CExtScrollWnd );

	CExtScrollWnd();

public:
	bool m_bScrollPhysical:1, m_bEatErasing:1,
		m_bClientCB:1, m_bBufferedPainting:1,
		m_bScrollInvalidate:1, m_bScrollErase:1,
		m_bScrollUpdateWindow:1, m_bRedrawUpdateWindow:1,
		m_bRedrawInvalidate:1, m_bRedrawErase:1,
		m_bUse32BitScrollInfo:1;

protected:
	HFONT m_hFont;
	int m_nUpdateScrollBars;

public:

	//{{AFX_VIRTUAL(CExtScrollWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	virtual ~CExtScrollWnd();

	virtual void PmBridge_OnPaintManagerChanged(
		CExtPaintManager * pGlobalPM
		);
	virtual CSize OnSwGetTotalSize() const;
	virtual CSize OnSwGetPageSize( int nDirection ) const;
	virtual CSize OnSwGetLineSize( int nDirection ) const;
	virtual CRect OnSwRecalcLayout(
		bool bDoLayout,
		LPCRECT pRectClientSrc = NULL
		);
	virtual CRect OnSwGetClientRect() const;
	
	virtual bool OnSwCanAutoHideScrollBar( bool bHorz ) const;
	virtual bool OnSwHasScrollBar( bool bHorz ) const;
	virtual void OnSwEnableScrollBarCtrl( int nBar, bool bEnable );
	virtual bool OnSwScrollInfoAdjust(
		int nBar,
		SCROLLINFO & si,
		bool bRedraw = true
		);
	virtual void OnSwSetScrollRange(
		int nBar,
		LONG nMinPos,
		LONG nMaxPos,
		bool bRedraw = true
		);
	virtual bool OnSwQueryThumbTrackEnabled( bool bHorz ) const;

	virtual LONG ScrollLimit32Get( int nBar ) const;
	virtual void ScrollInfo32Get(
		int nBar,
		LONG * p_nMinPos,
		LONG * p_nMaxPos,
		LONG * p_nPageSize,
		LONG * p_nCurrPos,
		LONG * p_nTrackPos
		) const;
	virtual LONG ScrollPos32Get(
		int nBar,
		bool bTrackPos = false
		) const;
	virtual void ScrollPos32Set(
		int nBar,
		LONG nPos,
		bool bRedraw = true
		);

	virtual CPoint OnSwGetScrollPos() const;
	virtual CPoint OnSwGetScrollPaintPos() const;
	virtual void OnSwSetScrollPos( POINT pt );

	virtual void OnSwGetScrollBarState(
		CSize sizeClient,
		CSize & sizeNeedSB,
		CSize & sizeRange,
		CPoint & ptMove,
		bool bInsideClient
		) const;
	virtual CSize OnSwGetScrollBarSizes() const;
	virtual bool OnSwCalcClientSizes(
		CSize & size,
		CSize & sizeSb
		);
	
	virtual UINT OnSwCalcMouseScrollLines(
		UINT fFlags,
		short zDelta,
		CPoint point
		);
	virtual bool OnSwDoMouseWheel(
		UINT fFlags,
		short zDelta,
		CPoint point
		);

	virtual bool OnSwDoScroll(
		UINT nScrollCode,
		UINT nPos,
		bool bDoScroll = true
		);
	virtual bool OnSwDoScrollBy(
		CSize sizeScroll,
		bool bDoScroll = true
		);
	virtual void OnSwUpdateScrollBars();

	virtual void OnSwDoScrollWindow(
		int xAmount,
		int yAmount,
		LPCRECT lpRect = NULL,
		LPCRECT lpClipRect = NULL
		);

	virtual void OnSwEnsurePointAvail( CPoint point );

	virtual void OnSwInvalidate( bool bErase );
	virtual void OnSwUpdateWindow();
	virtual void OnSwDoRedraw();
	virtual void OnSwPaint( CDC & dc );
	virtual void OnSwPaintAreaBetweenScrollBarWindows(
		CDC & dc,
		const RECT & rcAreaBetweenScrollBarWindows
		);

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtScrollWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnEnable( BOOL bEnable );
	afx_msg LRESULT OnSetFont( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetFont( WPARAM, LPARAM );
	//}}AFX_MSG
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThemeChanged( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
}; // class CExtScrollWnd

#if (!defined __EXT_MFC_NO_SCROLLITEMWND)

/////////////////////////////////////////////////////////////////////////////
// CExtScrollItemWnd window & CExtScrollItemCacheInfo helper

// item scrolling type
#define __ESIW_ST_NONE					0x00000000L
#define __ESIW_ST_PIXEL					0x00000001L
#define __ESIW_ST_ITEM					0x00000002L
#define __ESIW_ST_VIRTUAL				0x00000003L
#define __ESIW_ST_MASK					0x00000003L
#define __ESIW_ST_BIT_COUNT				2L

// item scroll window styles
#define __ESIS_STH_NONE					__ESIW_ST_NONE
#define __ESIS_STH_PIXEL				__ESIW_ST_PIXEL
#define __ESIS_STH_ITEM					__ESIW_ST_ITEM
#define __ESIS_STH_VIRTUAL				__ESIW_ST_VIRTUAL
#define __ESIS_STV_NONE					(__ESIW_ST_NONE << __ESIW_ST_BIT_COUNT)
#define __ESIS_STV_PIXEL				(__ESIW_ST_PIXEL << __ESIW_ST_BIT_COUNT)
#define __ESIS_STV_ITEM					(__ESIW_ST_ITEM << __ESIW_ST_BIT_COUNT)
#define __ESIS_STV_VIRTUAL				(__ESIW_ST_VIRTUAL << __ESIW_ST_BIT_COUNT)
#define __ESIS_STH_MASK					__ESIW_ST_MASK
#define __ESIS_STV_MASK					(__ESIW_ST_MASK << __ESIW_ST_BIT_COUNT)
#define __ESIS_ST_MASK					(__ESIS_STH_MASK|__ESIS_STV_MASK)
#define __ESIS_DISABLE_THUMBTRACK_H		0x00000010L
#define __ESIS_DISABLE_THUMBTRACK_V		0x00000020L
#define __ESIS_DISABLE_THUMBTRACK		(__ESIS_DISABLE_THUMBTRACK_H|__ESIS_DISABLE_THUMBTRACK_V)
#define __ESIS_DISABLE_AUTOHIDE_SB_H	0x00000040L
#define __ESIS_DISABLE_AUTOHIDE_SB_V	0x00000080L
#define __ESIS_DISABLE_AUTOHIDE_SB		(__ESIS_DISABLE_AUTOHIDE_SB_H|__ESIS_DISABLE_AUTOHIDE_SB_V)
#define __ESIS_PREFER_HORZ_WALK			0x00000100L

#if (!defined __EXT_MFC_NO_GRIDBASEWND)

// helper flags to optimize event processing when
// all rows/columns have equal sizes
#define __EGBS_FIXED_SIZE_COLUMNS		0x00000200L
#define __EGBS_FIXED_SIZE_ROWS			0x00000400L

// basic selection/focus type
#define __EGBS_SFB_NONE					0x00000000L
#define __EGBS_SFB_CELLS				0x00001000L
#define __EGBS_SFB_FULL_ROWS			0x00002000L
#define __EGBS_SFB_FULL_COLUMNS			0x00003000L
#define __EGBS_SFB_MASK					0x00003000L
// allow multiple row/column/cell selection
#define __EGBS_SFM_ROWS					0x00000800L
#define __EGBS_SFM_COLUMNS				0x00008000L
#define __EGBS_SFM_MASK					(__EGBS_SFM_ROWS|__EGBS_SFM_COLUMNS)
// enabled multiple selection types
#define __EGBS_SFM_CELLS_H				(__EGBS_SFB_CELLS|__EGBS_SFM_COLUMNS)
#define __EGBS_SFM_CELLS_V				(__EGBS_SFB_CELLS|__EGBS_SFM_ROWS)
#define __EGBS_SFM_CELLS_HV				(__EGBS_SFB_CELLS|__EGBS_SFM_COLUMNS|__EGBS_SFM_ROWS)
#define __EGBS_SFM_FULL_ROWS			(__EGBS_SFB_FULL_ROWS|__EGBS_SFM_ROWS)
#define __EGBS_SFM_FULL_COLUMNS			(__EGBS_SFB_FULL_COLUMNS|__EGBS_SFM_COLUMNS)
// any selection mask
#define __EGBS_SF_MASK					(__EGBS_SFB_MASK|__EGBS_SFM_MASK)
// allow selection via outer row/column clicks
#define __EGBS_SF_SELECT_OUTER_ROWS		0x00010000L
#define __EGBS_SF_SELECT_OUTER_COLUMNS	0x00020000L

// enable row/column resizing
#define __EGBS_RESIZING_CELLS_OUTER_H	0x00040000L
#define __EGBS_RESIZING_CELLS_OUTER_V	0x00100000L
#define __EGBS_RESIZING_CELLS_OUTER		(__EGBS_RESIZING_CELLS_OUTER_H|__EGBS_RESIZING_CELLS_OUTER_V)
#define __EGBS_RESIZING_CELLS_INNER_H	0x00200000L
#define __EGBS_RESIZING_CELLS_INNER_V	0x00400000L
#define __EGBS_RESIZING_CELLS_INNER		(__EGBS_RESIZING_CELLS_INNER_H|__EGBS_RESIZING_CELLS_INNER_V)
#define __EGBS_RESIZING_CELLS_MASK		(__EGBS_RESIZING_CELLS_OUTER|__EGBS_RESIZING_CELLS_INNER)

// auto move focused cell to bottom/right corner of selection
#define __EGBS_AUTO_FOCUS_BOTTOM_RIGHT	0x00800000L

// allow multiple cell ranges to be selected at a time
#define __EGBS_MULTI_AREA_SELECTION		0x01000000L

// no hide selection when not focused
#define __EGBS_NO_HIDE_SELECTION		0x02000000L

// draw grid lines
#define __EGBS_GRIDLINES_H				0x04000000L
#define __EGBS_GRIDLINES_V				0x08000000L
#define __EGBS_GRIDLINES				(__EGBS_GRIDLINES_H|__EGBS_GRIDLINES_V)

// mouse click + move selects cell range
#define __EGBS_LBEXT_SELECTION			0x10000000L

// subtract overlapped cell ranges in selection areas
#define __EGBS_SUBTRACT_SEL_AREAS		0x20000000L

// resize rows/columns on-the-fly (no half-tone tracker lines)
#define __EGBS_DYNAMIC_RESIZING_H		0x40000000L
#define __EGBS_DYNAMIC_RESIZING_V		0x80000000L
#define __EGBS_DYNAMIC_RESIZING			(__EGBS_DYNAMIC_RESIZING_H|__EGBS_DYNAMIC_RESIZING_V)

#endif // (!defined __EXT_MFC_NO_GRIDBASEWND)

// extended/behavior styles provided with
// CExtScrollItemWnd::SiwGetStyleEx()
// ( these styles are not intersected with
//   CExtScrollItemWnd::SiwGetStyle() style values )

#if (!defined __EGWS_EX_PM_COLORS)
	#define __EGWS_EX_PM_COLORS					0x80000000L
#endif


class __PROF_UIS_API CExtScrollItemCacheInfo
{
public:
	LONG m_nPosH;
	LONG m_nMaxH;
	LONG m_nPageSizeH;
	LONG m_nPosV;
	LONG m_nMaxV;
	LONG m_nPageSizeV;
	bool m_bBeginH:1;
	bool m_bEndH:1;
	bool m_bBeginV:1;
	bool m_bEndV:1;
	CExtScrollItemCacheInfo(
		LONG nPosH = 0L,
		LONG nMaxH = 0L,
		LONG nPageSizeH = 0L,
		LONG nPosV = 0L,
		LONG nMaxV = 0L,
		LONG nPageSizeV = 0L,
		bool bBeginH = false,
		bool bEndH = false,
		bool bBeginV = false,
		bool bEndV = false
		);
	CExtScrollItemCacheInfo(
		const CExtScrollItemCacheInfo & other
		);
	CExtScrollItemCacheInfo & operator = (
		const CExtScrollItemCacheInfo & other
		);
	bool operator == (
		const CExtScrollItemCacheInfo & other
		) const;
	bool operator != (
		const CExtScrollItemCacheInfo & other
		) const;
	void Empty(
		bool bEmptyH = true,
		bool bEmptyV = true
		);
	void EmptyH();
	void EmptyV();
	bool IsEmpty(
		bool bCheckEmptyH = true,
		bool bCheckEmptyV = true
		) const;
	bool IsEmptyH() const;
	bool IsEmptyV() const;
	bool IsValid() const;
	static bool IsEqual(
		const CExtScrollItemCacheInfo & left,
		const CExtScrollItemCacheInfo & right,
		bool bComparePosH = true,
		bool bComparePosV = true,
		bool bCompareMaxH = true,
		bool bCompareMaxV = true,
		bool bComparePageSizeH = true,
		bool bComparePageSizeV = true,
		bool bCompareBofH = false,
		bool bCompareBofV = false,
		bool bCompareEofH = false,
		bool bCompareEofV = false
		);
	bool IsEqual(
		const CExtScrollItemCacheInfo & other,
		bool bComparePosH = true,
		bool bComparePosV = true,
		bool bCompareMaxH = true,
		bool bCompareMaxV = true,
		bool bComparePageSizeH = true,
		bool bComparePageSizeV = true,
		bool bCompareBofH = false,
		bool bCompareBofV = false,
		bool bCompareEofH = false,
		bool bCompareEofV = false
		) const;
	LONG GetExtentH() const;
	LONG GetExtentV() const;
	LONG GetPosChangingH(
		const CExtScrollItemCacheInfo & _sciOld
		) const;
	LONG GetPosChangingV(
		const CExtScrollItemCacheInfo & _sciOld
		) const;
	LONG GetMaxChangingH(
		const CExtScrollItemCacheInfo & _sciOld
		) const;
	LONG GetMaxChangingV(
		const CExtScrollItemCacheInfo & _sciOld
		) const;
	LONG GetExtentChangingH(
		const CExtScrollItemCacheInfo & _sciOld
		) const;
	LONG GetExtentChangingV(
		const CExtScrollItemCacheInfo & _sciOld
		) const;
	LONG GetPageSizeChangingH(
		const CExtScrollItemCacheInfo & _sciOld
		) const;
	LONG GetPageSizeChangingV(
		const CExtScrollItemCacheInfo & _sciOld
		) const;
	bool IsShiftToBofH(
		const CExtScrollItemCacheInfo & _sciOld
		) const;
	bool IsShiftToBofV(
		const CExtScrollItemCacheInfo & _sciOld
		) const;
	bool IsShiftToEofH(
		const CExtScrollItemCacheInfo & _sciOld
		) const;
	bool IsShiftToEofV(
		const CExtScrollItemCacheInfo & _sciOld
		) const;
	void FlipHV();
	void Assign(
		const CExtScrollItemCacheInfo & other,
		bool bAssignH = true,
		bool bAssignV = true
		);
	void AssignH(
		const CExtScrollItemCacheInfo & other
		);
	void AssignV(
		const CExtScrollItemCacheInfo & other
		);
}; // class CExtScrollItemCacheInfo

class __PROF_UIS_API CExtScrollItemWnd : public CExtScrollWnd
{
public:
	DECLARE_DYNCREATE( CExtScrollItemWnd );
	CExtScrollItemWnd();

public:
	bool m_bExtractResourceStyles:1;

protected:
	static bool g_bScrollItemWndClassRegistered;
	bool m_bDirectCreateCall:1;
	DWORD m_dwScrollItemWndStyle, m_dwScrollItemWndStyleEx;

public:
	virtual DWORD SiwGetStyle() const;
	virtual DWORD SiwModifyStyle(
		DWORD dwStyleAdd,
		DWORD dwStyleRemove = 0L,
		bool bUpdateWnd = true
		);
	virtual DWORD SiwGetStyleEx() const;
	virtual DWORD SiwModifyStyleEx(
		DWORD dwStyleExAdd,
		DWORD dwStyleExRemove = 0L,
		bool bUpdateWnd = true
		);
	DWORD SiwScrollTypeHGet() const;
	DWORD SiwScrollTypeVGet() const;
	DWORD SiwScrollTypeHSet(
		DWORD dwScrollTypeNew,
		bool bUpdateWnd = true
		);
	DWORD SiwScrollTypeVSet(
		DWORD dwScrollTypeNew,
		bool bUpdateWnd = true
		);
	bool SiwPreferHorizontalWalkGet() const;
	bool SiwPreferHorizontalWalkSet(
		bool bPreferHorizontalWalk
		);
	bool SiwThumbTrackEnabledGet( bool bHorz ) const;
	bool SiwThumbTrackEnabledSet(
		bool bHorz,
		bool bEnabled = true
		);
	bool SiwAutohideScrollBarGet( bool bHorz ) const;
	bool SiwAutohideScrollBarSet(
		bool bHorz,
		bool bAutoHide = true
		);

	//{{AFX_VIRTUAL(CExtScrollItemWnd)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	virtual ~CExtScrollItemWnd();

	BOOL Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( __EXT_MFC_IDC_STATIC ),
		DWORD dwScrollItemWndStyle = 0L,
		DWORD dwWindowStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		CCreateContext * pContext = NULL
		);
	virtual bool _CreateHelper();
	static bool RegisterScrollItemWndClass();

	virtual void OnSwGetScrollBarState(
		CSize sizeClient,
		CSize & sizeNeedSB,
		CSize & sizeRange,
		CPoint & ptMove,
		bool bInsideClient
		) const;
	virtual CSize OnSwGetLineSize( int nDirection ) const;
	virtual void OnSwUpdateScrollBars();
	virtual CSize OnSwGetPageSize( int nDirection ) const;
	virtual CSize OnSwGetTotalSize() const;
	virtual bool OnSwDoScroll(
		UINT nScrollCode,
		UINT nPos,
		bool bDoScroll = true
		);
	virtual bool OnSwDoScrollBy(
		CSize sizeScroll,
		bool bDoScroll = true
		);
	virtual bool SiwFireCacheChanging(
		LONG nScrollPosNewX = -1L, // if <0L - use current
		LONG nScrollPosNewY = -1L, // if <0L - use current
		bool bUpdate = true
		);
	virtual CPoint OnSwGetScrollPaintPos() const;
	virtual void OnSwPaint( CDC & dc );
	virtual bool OnSwQueryThumbTrackEnabled( bool bHorz ) const;
	virtual bool OnSwCanAutoHideScrollBar( bool bHorz ) const;

	virtual CSize OnSiwCalcItemSize() const;
	virtual CSize OnSiwCalcPageMetrics( int nDirection ) const;
	virtual bool OnSiwQueryVirtualBOF( bool bHorz ) const;
	virtual bool OnSiwQueryVirtualEOF( bool bHorz ) const;
	virtual CRect OnSiwGetFrozenRange() const;
	virtual CRect OnSiwGetVisibleRange() const;
	virtual INT OnSiwQueryItemExtentH(
		LONG nColNo,
		INT * p_nExtraSpaceBefore = NULL,
		INT * p_nExtraSpaceAfter = NULL
		) const;
	virtual INT OnSiwQueryItemExtentV(
		LONG nRowNo,
		INT * p_nExtraSpaceBefore = NULL,
		INT * p_nExtraSpaceAfter = NULL
		) const;
	virtual bool OnSiwVirtualEndTestH( LONG nColNo, LPARAM lParam ) const;
	virtual bool OnSiwVirtualEndTestV( LONG nRowNo, LPARAM lParam ) const;
	virtual bool OnSiwWalkCell(
		CDC & dc,
		LPVOID pQueryData,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		bool & bVirtualRightReached,
		bool & bVirtualBottomReached,
		DWORD dwAreaFlags,
		bool bFocusedControl
		) const;
	virtual bool OnSiwWalkItemsH(
		CDC & dc,
		LPVOID pQueryData,
		const RECT & rcRowExtra,
		const RECT & rcRow,
		LONG nVisibleRowNo,
		LONG nRowNo,
		const RECT & rcVisibleRange,
		bool & bVirtualBottomReached,
		DWORD dwAreaFlags,
		bool bFocusedControl
		) const;
	virtual bool OnSiwWalkItemsV(
		CDC & dc,
		LPVOID pQueryData,
		const RECT & rcColExtra,
		const RECT & rcCol,
		LONG nVisibleColNo,
		LONG nColNo,
		const RECT & rcVisibleRange,
		bool & bVirtualRightReached,
		DWORD dwAreaFlags,
		bool bFocusedControl
		) const;
	virtual void OnSiwPaintBackground(
		CDC & dc,
		bool bFocusedControl
		) const;
	virtual void OnSiwPaintForeground(
		CDC & dc,
		bool bFocusedControl
		) const;
	virtual bool OnSiwQueryFocusedControlState() const;
	virtual void OnSiwDrawFocusRect(
		CDC &dc,
		LPCRECT pRect,
		CObject * pObjSrc = NULL,
		LPARAM lParam = 0L
		) const;
	virtual COLORREF OnSiwGetReadOnlyTextColor() const;
	virtual COLORREF OnSiwGetReadOnlyBackgroundColor() const;
	virtual COLORREF OnSiwGetSysColor( int nIndex ) const;
	// nLighterOrDarker is related to COLOR_3DFACE brush:
	// -3 - darkest, -2 - darker, -1 - dark,
	// 1 - light, 2 - lighter, 3 - lightest
	virtual CBrush & OnSiwGetLighterOrDarkerBrush( int nLighterOrDarker ) const;
	virtual CFont & OnSiwGetDefaultFont() const;

	virtual bool OnSiwCacheChanged(
		const CExtScrollItemCacheInfo & _sciNew,
		const CExtScrollItemCacheInfo & _sciOld
		);

protected:
	CExtScrollItemCacheInfo m_sciLast;
	virtual bool _NotifyCacheChanged(
		LONG nPosH,
		LONG nPosOldH,
		LONG nMaxH,
		LONG nPageSizeH,
		LONG nPosV,
		LONG nPosOldV,
		LONG nMaxV,
		LONG nPageSizeV,
		bool bBeginH,
		bool bEndH,
		bool bBeginV,
		bool bEndV
		);
public:

protected:
	//{{AFX_MSG(CExtScrollItemWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtScrollItemWnd

#define __EXT_SCROLLITEMWND_CLASS_NAME _T("ProfUIS-ScrollItemWindow")

#endif // (!defined __EXT_MFC_NO_SCROLLITEMWND)


#if (!defined __EXT_MFC_NO_SCROLLCONAINERWND)

/////////////////////////////////////////////////////////////////////////////
// CExtScrollContainerWnd window

class __PROF_UIS_API CExtScrollContainerWnd : public CExtScrollWnd
{
	mutable CSize m_sizeToScroll;
public:
	DECLARE_DYNCREATE( CExtScrollContainerWnd );
	CExtScrollContainerWnd();
	virtual ~CExtScrollContainerWnd();

	CExtScrollBar m_wndScrollBarH, m_wndScrollBarV;
	CExtLabel m_wndCorner;
	COLORREF m_clrBkColor;
	bool m_bAutoDelete:1, m_bRedirectPreTranslateMessage:1, m_bRedirectOnCmdMsg:1,
		m_bRedirectFocus:1, m_bSynchronizeEnabledState:1;
	UINT m_nScrollRedrawFlags;

	//{{AFX_VIRTUAL(CExtScrollContainerWnd)
	public:
	virtual CScrollBar * GetScrollBarCtrl(int nBar) const;
	virtual BOOL PreTranslateMessage( MSG * pMsg );
	virtual BOOL OnCmdMsg( UINT nID, int nCode, void * pExtra, AFX_CMDHANDLERINFO * pHandlerInfo );
	protected:
	virtual BOOL PreCreateWindow( CREATESTRUCT & cs );
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	virtual bool Create(
		CWnd * pWndParent,
		const RECT & rc,
		UINT nDlgCtrlID = AFX_IDW_PANE_FIRST
		);
	virtual CWnd * OnGetWndToScroll();
	virtual CPoint OnSwGetScrollPaintPos() const;
	virtual void OnSwPaint( CDC & dc );
	virtual void OnSwPaintAreaBetweenScrollBarWindows(
		CDC & dc,
		const RECT & rcAreaBetweenScrollBarWindows
		);
	virtual CSize OnSwGetTotalSize() const;
	virtual CSize OnSwGetPageSize( int nDirection ) const;
	virtual CSize OnSwGetLineSize( int nDirection ) const;
	virtual CRect OnSwRecalcLayout(
		bool bDoLayout,
		LPCRECT pRectClientSrc = NULL
		);
	virtual bool OnSwDoScrollBy(
		CSize sizeScroll,
		bool bDoScroll = true
	);

protected:
	//{{AFX_MSG(CExtScrollContainerWnd)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSetFocus( CWnd * pOldWnd );
	afx_msg void OnEnable( BOOL bEnable );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; /// class CExtScrollContainerWnd

#endif // (!defined __EXT_MFC_NO_SCROLLCONAINERWND)

#endif // (!defined __EXT_MFC_NO_SCROLLWND)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined __EXT_SCROLLWND_H
