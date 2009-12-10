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

#if (! defined __EXT_TAB_PAGE_CONTAINER_WND_H)
#define __EXT_TAB_PAGE_CONTAINER_WND_H

#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_HOOK_H)
	#include "../Src/ExtHook.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtTabCtrl.h : header file
//

#ifndef __AFXTEMPL_H__
	#include <AfxTempl.h>
#endif

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

#if (!defined __EXT_TABWND_H)
	#include <ExtTabWnd.h>
#endif

#if (! defined __EXT_TAB_FLAT_WND_H)
	#include <ExtTabFlatWnd.h>
#endif

// tab page container window class name
#define __EXT_TAB_PAGE_CONTAINER_CLASS_NAME _T("ProfUIS-TabPageContainer")

template < class _BT >
class CExtTWPC : public _BT
{
public:
	CExtTWPC()
	{
		m_bReflectParentSizing = true;
	}

	virtual ~CExtTWPC()
	{
	}
// 	virtual HFONT OnQueryFont() const
// 	{
// 		DWORD dwOrientation = OrientationGet();
// 		if(		dwOrientation == __ETWS_ORIENT_TOP
// 			||	dwOrientation == __ETWS_ORIENT_BOTTOM
// 			)
// 		{
// 			CExtTabPageContainerWnd * pTPC = 
// 				DYNAMIC_DOWNCAST( CExtTabPageContainerWnd, GetParent() );
// 			if( pTPC != NULL )
// 				return pTPC->OnQueryFont();
// 		}
// 		return _BT::OnQueryFont();
// 	}	
	virtual void OnTabWndSyncVisibility()
	{
		CExtTabPageContainerWnd * pTPC = 
			DYNAMIC_DOWNCAST( CExtTabPageContainerWnd, GetParent() );
		if( pTPC != NULL )
			pTPC->OnTabWndSyncVisibility();
	}
	virtual void OnTabWndItemPosChanged(
		LONG nItemIndex,
		LONG nItemNewIndex
		)
	{
		_BT::OnTabWndItemPosChanged(
			nItemIndex,
			nItemNewIndex
			);
		CExtTabPageContainerWnd * pTPC = 
			DYNAMIC_DOWNCAST( CExtTabPageContainerWnd, GetParent() );
		if( pTPC != NULL )
			pTPC->OnTabWndItemPosChanged(
				nItemIndex,
				nItemNewIndex
				);
	}
	virtual bool OnTabWndSelectionChange(
		LONG nOldItemIndex,
		LONG nNewItemIndex,
		bool bPreSelectionTest
		)
	{
		if( ! _BT::OnTabWndSelectionChange(
				nOldItemIndex,
				nNewItemIndex,
				bPreSelectionTest
				)
			)
			return false;
		CExtTabPageContainerWnd * pTPC = 
			DYNAMIC_DOWNCAST( CExtTabPageContainerWnd, GetParent() );
		if( pTPC == NULL )
			return false;
		return
			pTPC->OnTabWndSelectionChange(
				nOldItemIndex,
				nNewItemIndex,
				bPreSelectionTest
				);
	}

	virtual bool OnTabWndClickedButton(
		LONG nHitTest,
		bool bButtonPressed,
		INT nMouseButton, // MK_... values
		UINT nMouseEventFlags
		)
	{
		bool bRetVal =
			_BT::OnTabWndClickedButton(
			nHitTest,
			bButtonPressed,
			nMouseButton, // MK_... values
			nMouseEventFlags
			);
		CExtTabPageContainerWnd * pTPC = 
			DYNAMIC_DOWNCAST( CExtTabPageContainerWnd, GetParent() );
		if( pTPC != NULL )
			pTPC->OnTabWndClickedButton(
				nHitTest,
				bButtonPressed,
				nMouseButton, // MK_... values
				nMouseEventFlags
				);
		return bRetVal;
	}

	virtual void OnTabWndClickedItemCloseButton(
		LONG nItemIndex
		)
	{
		CExtTabPageContainerWnd * pTPC = 
			DYNAMIC_DOWNCAST( CExtTabPageContainerWnd, GetParent() );
		if( pTPC != NULL )
		pTPC->OnTabWndClickedItemCloseButton( nItemIndex );
	}
	
	virtual void PostNcDestroy()
	{
		delete this;
	}

	virtual HWND _GetHwndCtxMenuNotify()
	{
		ASSERT_VALID( this );
		if( GetSafeHwnd() == NULL )
			return NULL;
		CFrameWnd * pFrame = GetParentFrame();
		if( pFrame != NULL )
		{
			if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
				pFrame = pFrame->GetParentFrame();
			if( pFrame != NULL )
				return pFrame->m_hWnd;
		}
		HWND hWnd = ::GetParent( m_hWnd );
		ASSERT( hWnd != NULL && ::IsWindow(hWnd) );
		__EXT_MFC_ULONG_PTR dwStyle = ::__EXT_MFC_GetWindowLong( hWnd, GWL_STYLE );
		if( ( dwStyle & WS_CHILD ) == 0 )
			return hWnd; // notify popup page container
		hWnd = ::GetParent( hWnd );
		ASSERT( hWnd != NULL && ::IsWindow(hWnd) );
		return hWnd;
	}

	virtual void _OnTabItemHook_GetItemStyle(
		const CExtTabWnd::TAB_ITEM_INFO & _TII,
		DWORD & dwItemStyle
		) const
	{
		ASSERT_VALID( this );
		if( GetSafeHwnd() == NULL )
			return;
		HWND hWnd = (HWND) _TII.LParamGet();
		if( hWnd == NULL && (! ::IsWindow( hWnd ) ) )
			return;
		__EXT_MFC_ULONG_PTR dwWndStyle = ::__EXT_MFC_GetWindowLong( hWnd, GWL_STYLE );
		if( (dwWndStyle&WS_DISABLED) != 0 )
			dwItemStyle |= __ETWI_DISABLED;
		else
			dwItemStyle &= ~__ETWI_DISABLED;
	}

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch( message )
		{
		case WM_CONTEXTMENU:
			{
				HWND hWndTrack = _GetHwndCtxMenuNotify();
				if( hWndTrack == NULL )
					break;
				ASSERT( ::IsWindow(hWndTrack) );
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
				CExtCustomizeSite * pSite =
					CExtCustomizeSite::GetCustomizeSite( m_hWnd );
				if(		pSite != NULL
					&&	pSite->IsCustomizeMode()
					)
					return 0;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
				if( CExtControlBar::FindHelpMode(this) )
					return 0;
				CPoint point = DWORD(lParam);
				CPoint ptClient = point;
				ScreenToClient( &ptClient );
				LONG nHitTest = ItemHitTest( ptClient );
				if( nHitTest >= 0 )
					SelectionSet( nHitTest, true, true );
				CExtPopupMenuWnd * pPopup =
					CExtPopupMenuWnd::InstantiatePopupMenu(
						GetSafeHwnd(),
						RUNTIME_CLASS(CExtPopupMenuWnd),
						this
						);
				VERIFY( pPopup->CreatePopupMenu(hWndTrack) );
				CExtControlBar::POPUP_MENU_EVENT_DATA _pmed(
					CExtControlBar::POPUP_MENU_EVENT_DATA::__PMED_TAB_PAGE_CONTAINER_TABS_CTX,
					pPopup,
					this,
					point,
					WM_CONTEXTMENU,
					NULL
					);
				if( !_pmed.NotifyTarget( false ) )
				{
					delete pPopup;
					return 0;
				} // if( !_pmed.NotifyTarget( false ) )
				_pmed.NotifyTarget( true );
				if( pPopup->ItemGetCount() == 0 )
				{
					delete pPopup;
					return 0;
				}
				::SetFocus( hWndTrack );
				HWND hWndOwn = m_hWnd;
				if(	! pPopup->TrackPopupMenu(
						TPMX_OWNERDRAW_FIXED|TPMX_DO_MESSAGE_LOOP,
						point.x,
						point.y
						)
					)
				{
					delete pPopup;
				}
				if( ! ::IsWindow(hWndOwn) )
					return 0;
				Invalidate();
				UpdateWindow();
			}
		return 0;
		} // switch( message )
		return _BT::WindowProc(message, wParam, lParam);
	}
}; // template class CExtTWPC

/////////////////////////////////////////////////////////////////////////////
// CExtTabPageContainerWnd window

class __PROF_UIS_API CExtTabPageContainerWnd 
	: public CWnd
	, public CExtHookSpy
	, public CExtPmBridge
{
// Construction
public:
	DECLARE_DYNCREATE( CExtTabPageContainerWnd )
	DECLARE_CExtPmBridge_MEMBERS( CExtTabPageContainerWnd );

	CExtTabPageContainerWnd();

// Attributes
public:
	CExtTabWnd * GetSafeTabWindow();
	const CExtTabWnd * GetSafeTabWindow() const;

protected:
	HFONT m_hFont;
	CExtTabWnd *  m_pWndTab;
	bool m_bTabWndVisible:1;

// Operations
protected:		
	DWORD ModifyTabStyle( DWORD dwTabWndStyle, bool bSet );
	DWORD ModifyTabStyleEx( DWORD dwTabWndStyleEx, bool bSet );
	
public:
	static bool g_bHandlePriorNextKeyEvents;
	static bool g_bHandleTabKeyEvents;
	static bool g_bAllowAccelBasedPageSelection;
	bool m_bHandlePriorNextKeyEvents:1, m_bHandleTabKeyEvents:1, m_bAllowAccelBasedPageSelection:1;
	static HCURSOR g_hCursor;


	void ShowTabStrip( 
		bool bShow = true
		);
	bool IsTabStripVisible() const;

	DWORD CenterTextSet( 
		bool bSet = true 
		);
	bool CenterTextGet() const;
	
	DWORD AutoHideScrollSet( 
		bool bSet = true 
		);
	bool AutoHideScrollGet() const;
	
	DWORD ShowBtnScrollHomeSet( 
		bool bSet = true 
		);
	bool ShowBtnScrollHomeGet() const;
	
	DWORD ShowBtnScrollEndSet( 
		bool bSet = true 
		);
	bool ShowBtnScrollEndGet() const;
	
	DWORD ShowBtnCloseSet( 
		bool bSet = true 
		);
	bool ShowBtnCloseGet() const;
	
	DWORD EnabledBtnCloseSet( 
		bool bSet = true 
		);
	bool EnabledBtnCloseGet() const;
	
	DWORD ShowBtnHelpSet( 
		bool bSet = true 
		);
	bool ShowBtnHelpGet() const;
	
	DWORD ShowBtnTabListSet( 
		bool bSet = true 
		);
	bool ShowBtnTabListGet() const;

	DWORD EnabledBtnTabListSet( 
		bool bSet = true 
		);
	bool EnabledBtnTabListGet() const;

	DWORD EnabledBtnHelpSet( 
		bool bSet = true 
		);
	bool EnabledBtnHelpGet() const;
	
	DWORD EqualWidthsSet( 
		bool bSet = true 
		);
	bool EqualWidthsGet() const;
	
	DWORD FullWidthSet( 
		bool bSet = true 
		);
	bool FullWidthGet() const;
	
	DWORD HoverFocusSet( 
		bool bSet = true 
		);
	bool HoverFocusGet() const;

	DWORD ItemDraggingSet( 
		bool bSet = true 
		);
	bool ItemDraggingGet() const;
	
	DWORD InvertVertFontSet( 
		bool bSet = true 
		);
	bool InvertVertFontGet() const;

	DWORD OrientationGet() const;
	DWORD OrientationSet(DWORD dwOrientation);

	bool OrientationIsHorizontal() const;
	bool OrientationIsVertical() const;
	bool OrientationIsTopLeft() const;

	bool SelectionBoldGet() const;
	void SelectionBoldSet( bool bBold = true);

	DWORD CloseOnTabsSet( 
		bool bSet = true,
		bool bSelectedOnly = false
		);
	bool CloseOnTabsGet() const;
	bool CloseOnTabsSelectedOnlyGet() const;

	virtual BOOL Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( __EXT_MFC_IDC_STATIC ),
		DWORD dwWindowStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CCreateContext * pContext = NULL
		);

	LONG PageFindByHWND(
		HWND hWnd,
		LONG nIndexStartSearch = -1,
		bool bIncludeVisible = true,
		bool bIncludeInvisible = false
		) const;
	
	bool PageEnsureVisible(
		INT nItemIndex,
		bool bUpdateTabWnd = false
		);

	bool PageEnabledGet( 
		INT nItemIndex 
		) const;
	bool PageEnabledSet( 
		INT nItemIndex, 
		bool bEnable = true 
		);

	bool PageVisibleGet( 
		INT nItemIndex 
		) const;
	bool PageVisibleSet( 
		INT nItemIndex, 
		bool bVisible = true 
		);

	bool PageMove( // move page into the new position
		LONG nIndex,
		LONG nNewIndex,
		bool bUpdateTabWnd = false
		);

	BOOL PageInsert(
		HWND hWnd,
		__EXT_MFC_SAFE_LPCTSTR sItemText = NULL,
		HICON hIcon = NULL,
		bool bCopyIcon = true,
		LONG nPos = -1L, // append
		bool bSelect = false
		);
	BOOL PageInsert(
		CWnd * pWnd,
		__EXT_MFC_SAFE_LPCTSTR sItemText = NULL,
		HICON hIcon = NULL,
		bool bCopyIcon = true,
		LONG nPos = -1L, // append
		bool bSelect = false
		);
	BOOL PageInsert(
		HWND hWnd,
		const CExtCmdIcon & pageIcon,
		__EXT_MFC_SAFE_LPCTSTR sItemText = NULL,
		LONG nPos = -1L, // append
		bool bSelect = false
		);
	BOOL PageInsert(
		CWnd * pWnd,
		const CExtCmdIcon & pageIcon,
		__EXT_MFC_SAFE_LPCTSTR sItemText = NULL,
		LONG nPos = -1L, // append
		bool bSelect = false
		);

	LONG PageRemove( // returns count of removed items
		LONG nIndex,
		LONG nCountToRemove = 1,
		bool bDestroyPageWnd = true
		);
		
	LONG PageRemoveAll( bool bDestroyPageWnd = true ); // returns count of removed items

	LONG PageGetCount() const;

	LONG PageSelectionGet() const;
	void PageSelectionSet( LONG nPos );
	void PageSelectionSetByWnd( CWnd * pWnd );
	void PageSelectionSetByHWND( HWND hWnd );
	
	CExtCmdIcon & PageIconGet( 
		LONG nIndex 
		);
	const CExtCmdIcon & PageIconGet( 
		LONG nIndex 
		) const;
	void PageIconSet(
		LONG nIndex,
		const CExtCmdIcon & _icon,
		bool bUpdateTabWnd = false
		);
	void PageIconSet(
		LONG nIndex,
		HICON hIcon = NULL,
		bool bCopyIcon = true,
		bool bUpdateTabWnd = false
		);

	bool PageCenterTextGet(
		LONG nIndex
		) const;
	void PageCenterTextSet( 
		LONG nIndex,
		bool bSet = true, 
		bool bUpdateTabWnd = false
		);

	__EXT_MFC_SAFE_LPCTSTR PageTextGet( LONG nIndex ) const;
	void PageTextSet(
		LONG nIndex,
		__EXT_MFC_SAFE_LPCTSTR sText = NULL,
		bool bUpdateTabWnd = false
		);

	__EXT_MFC_SAFE_LPCTSTR PageTooltipTextGet( LONG nIndex ) const;

	void PageTooltipTextSet(
		LONG nIndex,
		__EXT_MFC_SAFE_LPCTSTR sTooltipText = NULL,
		bool bUpdateTabWnd = false
		);

	HWND PageHwndSet(
		LONG nIndex,
		HWND hWndNew
		);
	HWND PageHwndGet( LONG nIndex ) const;
	HWND PageHwndGetSafe( LONG nIndex ) const;
	const CWnd * PagePermanentWndGet( LONG nIndex ) const;
	CWnd * PagePermanentWndGet( LONG nIndex );
	
	BOOL PagesNavigate(BOOL bNext);

	static bool RegisterTabCtrlWndClass();
	
protected:
	static bool g_bTabCtrlWndClassRegistered;
	bool m_bDirectCreateCall:1;
	
	virtual void DoPaint( CDC * pDC );

	virtual bool _CreateHelper();
	virtual void _ResetAllPageIdentifiersImpl();
	virtual void _RepositionBarsImpl();
	virtual void _RealignAllImpl();
	
	virtual bool OnHookSpyKeyMsg(
		MSG * pMSG
		);

	virtual CExtTabWnd* OnTabWndGetTabImpl()
	{
		return new CExtTWPC < CExtTabWnd >;
	}

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTabPageContainerWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtTabPageContainerWnd();

	virtual HFONT OnQueryFont() const;

	virtual bool OnTabWndClickedButton(
		LONG nHitTest,
		bool bButtonPressed,
		INT nMouseButton, // MK_... values
		UINT nMouseEventFlags
		);
	virtual void OnTabWndClickedItemCloseButton(
		LONG nItemIndex
		);
	virtual void OnTabWndItemPosChanged(
		LONG nItemIndex,
		LONG nItemNewIndex
		);
	virtual bool OnTabWndSelectionChange(
		LONG nOldItemIndex,
		LONG nNewItemIndex,
		bool bPreSelectionTest
		);
	virtual void OnTabWndSyncVisibility();

// Generated message map functions
protected:
	//{{AFX_MSG(CExtTabPageContainerWnd)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnSetFont( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetFont( WPARAM, LPARAM );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtTabPageContainerWnd 

__PROF_UIS_API void AFXAPI __EXT_MFC_DDX_TabPageContainerIndex( CDataExchange * pDX, INT nIDC, LONG & index );

#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL)

/////////////////////////////////////////////////////////////////////////////
// CExtTabPageContainerFlatWnd window

class __PROF_UIS_API CExtTabPageContainerFlatWnd
	: public CExtTabPageContainerWnd
{
	DECLARE_DYNCREATE( CExtTabPageContainerFlatWnd )

// Construction
public:
	bool m_bRenderConsistentPageBackground:1;
	CExtTabPageContainerFlatWnd();

	bool ItemsHasInclineGet( bool bBefore ) const;
	void ItemsHasInclineSet( bool bBefore, bool bSet = true );
	
	bool ItemsHasInclineBeforeGet() const;
	void ItemsHasInclineBeforeSet( bool bSet = true );
	
	bool ItemsHasInclineAfterGet() const;
	void ItemsHasInclineAfterSet( bool bSet = true );

// Attributes
public:

// Operations
public:

	virtual CExtTabWnd* OnTabWndGetTabImpl()
	{
		return new CExtTWPC < CExtTabFlatWnd >;
	}

	virtual BOOL Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( __EXT_MFC_IDC_STATIC ),
		DWORD dwWindowStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CCreateContext * pContext = NULL
		);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTabPageContainerFlatWnd)
	protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtTabPageContainerFlatWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtTabPageContainerFlatWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtTabPageContainerFlatWnd

#endif // (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL)

#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_BUTTONS_CTRL)

/////////////////////////////////////////////////////////////////////////////
// CExtTabPageContainerButtonsWnd window

class __PROF_UIS_API CExtTabPageContainerButtonsWnd
	: public CExtTabPageContainerFlatWnd
{
	DECLARE_DYNCREATE( CExtTabPageContainerButtonsWnd )

// Construction
public:
	CExtTabPageContainerButtonsWnd();

// Attributes
public:

// Operations
public:

	virtual CExtTabWnd* OnTabWndGetTabImpl()
	{
		return new CExtTWPC < CExtTabButtonsWnd >;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTabPageContainerButtonsWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtTabPageContainerButtonsWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtTabPageContainerButtonsWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtTabPageContainerButtonsWnd

#endif // (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_BUTTONS_CTRL)

#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_ONENOTE_CTRL)

/////////////////////////////////////////////////////////////////////////////
// CExtTabPageContainerOneNoteWnd window

class __PROF_UIS_API CExtTabPageContainerOneNoteWnd
	: public CExtTabPageContainerWnd
{
	DECLARE_DYNCREATE( CExtTabPageContainerOneNoteWnd )

public:
	bool m_bRenderConsistentPageBackground:1, m_bRenderGradientInheritance:1;
	CExtTabPageContainerOneNoteWnd();
	virtual ~CExtTabPageContainerOneNoteWnd();

	virtual CExtTabWnd* OnTabWndGetTabImpl()
	{
		return new CExtTWPC < CExtTabOneNoteWnd >;
	}

	BOOL PageInsert(
		HWND hWnd,
		__EXT_MFC_SAFE_LPCTSTR sItemText = NULL,
		HICON hIcon = NULL,
		bool bCopyIcon = true,
		LONG nPos = -1L, // append
		bool bSelect = false,
		COLORREF clrBkLight = (COLORREF)(-1L),
		COLORREF clrBkDark= (COLORREF)(-1L)
		);
	BOOL PageInsert(
		CWnd * pWnd,
		__EXT_MFC_SAFE_LPCTSTR sItemText = NULL,
		HICON hIcon = NULL,
		bool bCopyIcon = true,
		LONG nPos = -1L, // append
		bool bSelect = false,
		COLORREF clrBkLight = (COLORREF)(-1L),
		COLORREF clrBkDark= (COLORREF)(-1L)
		);
	BOOL PageInsert(
		HWND hWnd,
		const CExtCmdIcon & pageIcon,
		__EXT_MFC_SAFE_LPCTSTR sItemText = NULL,
		LONG nPos = -1L, // append
		bool bSelect = false,
		COLORREF clrBkLight = (COLORREF)(-1L),
		COLORREF clrBkDark= (COLORREF)(-1L)
		);
	BOOL PageInsert(
		CWnd * pWnd,
		const CExtCmdIcon & pageIcon,
		__EXT_MFC_SAFE_LPCTSTR sItemText = NULL,
		LONG nPos = -1L, // append
		bool bSelect = false,
		COLORREF clrBkLight = (COLORREF)(-1L),
		COLORREF clrBkDark= (COLORREF)(-1L)
		);

	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
}; // class CExtTabPageContainerOneNoteWnd

#endif // (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_ONENOTE_CTRL)


#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_WHIDBEY_CTRL)

/////////////////////////////////////////////////////////////////////////////
// CExtTabPageContainerWhidbeyWnd window

class __PROF_UIS_API CExtTabPageContainerWhidbeyWnd
	: public CExtTabPageContainerWnd
{
	DECLARE_DYNCREATE( CExtTabPageContainerWhidbeyWnd )
public:
	bool m_bRenderConsistentPageBackground:1;
	CExtTabPageContainerWhidbeyWnd();
	virtual ~CExtTabPageContainerWhidbeyWnd();

	virtual CExtTabWnd* OnTabWndGetTabImpl()
	{
		return new CExtTWPC < CExtTabWhidbeyWnd >;
	}

	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
}; // class CExtTabPageContainerWhidbeyWnd

#endif // (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_WHIDBEY_CTRL)


#endif // (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__EXT_TAB_PAGE_CONTAINER_WND_H)
