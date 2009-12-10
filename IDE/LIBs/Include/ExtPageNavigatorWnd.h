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

#if (!defined __EXT_PAGE_NAVIGATOR_H)
#define __EXT_PAGE_NAVIGATOR_H

#if (!defined __EXT_MFC_NO_PAGE_NAVIGATOR)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

#if (!defined __EXT_MOUSECAPTURESINK_H)
	#include <../Src/ExtMouseCaptureSink.h>
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// page navigator wnd class name
#define __EXT_PAGE_NAVIGATOR_CLASS_NAME		_T("ProfUIS-PageNavigator")

// general hit-testing values
#define __EPNWH_NOWHERE						(0L)
#define __EPNWH_ITEM_PAGE					(1L)
#define __EPNWH_ITEM_CAPTION				(2L)
#define __EPNWH_COLLAPSED_ITEMS_AREA		(3L)
#define __EPNWH_EXPANDED_ITEMS_AREA			(4L)
#define __EPNWH_SPLITTER					(5L)

class CExtPageNavigatorOptionsDlg;

//////////////////////////////////////////////////////////////////////////
// CExtPageNavigatorWnd window

class __PROF_UIS_API CExtPageNavigatorWnd
	: public CWnd
	, public CExtMouseCaptureSink
	, public CExtPmBridge
{
// Construction
public:
	DECLARE_DYNCREATE( CExtPageNavigatorWnd );
	DECLARE_CExtPmBridge_MEMBERS( CExtPageNavigatorWnd );

	CExtPageNavigatorWnd();
 
// Attributes
public:
	class PAGE_ITEM_INFO;

	// class ITEM_PANE_INFO
	class __PROF_UIS_API ITEM_PANE_INFO : public CObject
	{
	protected:
		HWND m_hWnd;
		CExtSafeString m_sText;
		bool m_bExpanded:1;
		bool m_bExpandable:1;
		CRect m_rcRect;
		CRect m_rcCaption;
		INT m_nHeight;
		LPARAM m_lParam;

		bool m_bHover:1;
		bool m_bPressed:1;

		CExtPageNavigatorWnd *m_pPageNavigatorWnd;

	public:
		ITEM_PANE_INFO( 
			HWND hWnd,
			__EXT_MFC_SAFE_LPCTSTR sText = NULL,
			INT nHeight = -1,
			CExtPageNavigatorWnd * pPageNavigatorWnd = NULL
			)
			: m_hWnd( hWnd )
			, m_sText( ( sText == NULL ) ? _T("") : sText )
			, m_bExpanded( true )
			, m_bExpandable( true )
			, m_nHeight( nHeight )
			, m_rcRect( 0, 0, 0, 0 )
			, m_rcCaption( 0, 0, 0, 0 )
			, m_lParam( 0 )
			, m_bHover( false )
			, m_bPressed( false )
			, m_pPageNavigatorWnd( pPageNavigatorWnd )
		{
		}

#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
#endif
		virtual void Serialize( CArchive & ar );

		HWND GetSafeHwnd();

		__EXT_MFC_SAFE_LPCTSTR TextGet() const;
		void TextSet( 
			__EXT_MFC_SAFE_LPCTSTR sText 
			);
		const CExtSafeString & TextGetRef() const;
		CExtSafeString & TextGetRef();

		enum e_ExpandType_t
		{
			__ET_COLLAPSE = 0,
			__ET_EXPAND = 1,
			__ET_TOGGLE = 2
		};

		bool Expand( 
			e_ExpandType_t eExpandType,
			bool bUpdate = false
			);
		bool IsExpanded() const;

		bool ExpandableGet() const;
		void ExpandableSet( 
			bool bSet = true
			);

		CRect ItemRectGet() const;
		void ItemRectSet( 
			const RECT & rc 
			);
		void ItemRectSet( 
			int x1, 
			int y1, 
			int x2, 
			int y2 
			);
		void ItemRectSetEmpty();

		CRect CaptionRectGet() const;
		void CaptionRectSet( 
			const RECT & rc 
			);
		void CaptionRectSet( 
			int x1, 
			int y1, 
			int x2, 
			int y2 
			);
		void CaptionRectSetEmpty();

		INT HeightGet() const;
		void HeightSet( 
			INT nHeight 
			);

		LPARAM LParamGet() const;
		void LParamSet( 
			LPARAM lParam = 0 
			);

		friend class PAGE_ITEM_INFO;
		friend class CExtPageNavigatorWnd;

	}; // class ITEM_PANE_INFO

	class __PROF_UIS_API PAGE_ITEM_INFO : public CObject
	{
	public:

#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
#endif
		virtual void Serialize( 
			CArchive & ar,
			bool bPersistent = true
			);

		__EXT_MFC_SAFE_LPCTSTR TextGet() const;
		void TextSet( 
			__EXT_MFC_SAFE_LPCTSTR sText = NULL 
			);

		LPARAM LParamGet() const;
		void LParamSet( 
			LPARAM lParam = 0 
			);

		__EXT_MFC_SAFE_LPCTSTR TooltipGet() const;
		void TooltipSet( 
			__EXT_MFC_SAFE_LPCTSTR sTooltip = NULL 
			);

		CSize IconGetSize( 
			bool bExpanded 
			) const;

		CExtCmdIcon * IconGetPtr( 
			bool bExpanded 
			) const;

		void IconSet(
			HICON hIcon,
			bool bExpanded
			);

		void Show();
		void Hide();

		bool EnabledGet() const;
		bool EnabledSet( 
			bool bEnabled = true,
			bool bPersistent = true
			);

		friend class CExtPageNavigatorWnd;

	protected:
		PAGE_ITEM_INFO(
			__EXT_MFC_SAFE_LPCTSTR sText = NULL,
			const CExtCmdIcon * pIconExpanded = NULL,
			const CExtCmdIcon * pIconCollapsed = NULL,
			DWORD dwData = 0,
			CExtPageNavigatorWnd * pPageNavigatorWnd = NULL
			);
		PAGE_ITEM_INFO( 
			const PAGE_ITEM_INFO & other 
			);
		PAGE_ITEM_INFO( 
			CExtPageNavigatorWnd * pPageNavigatorWnd 
			);
		~PAGE_ITEM_INFO();

		PAGE_ITEM_INFO & operator=(
			const PAGE_ITEM_INFO & other
			)
		{
			_AssignFromOther( other );
			return *this;
		}

		CExtSafeString m_sText;
		CExtSafeString m_sTooltip;
		CExtCmdIcon m_IconExpanded;
		CExtCmdIcon m_IconCollapsed;
		LPARAM m_lParam;

		bool m_bEnabledOriginal:1;
		LONG m_nIndexOriginal;

		bool m_bEnabled:1;
		bool m_bVisible:1;
		bool m_bExpanded:1;

		bool m_bHover:1;
		bool m_bSelected:1;
		bool m_bPressed:1;

		bool m_bConfigButton:1;
		CExtPageNavigatorWnd *m_pPageNavigatorWnd;

		CRect m_rcRect;
		CRect m_rcActivePage;

		void _ClearPressedFlag();
		void _ClearHoverFlag();

		void _AssignFromOther(
			const PAGE_ITEM_INFO & other
			);

		void ActivePageRectSet( 
			LPCRECT rcActivePage 
			);

		void Resize();

	protected:
		
		typedef
			CArray <
				ITEM_PANE_INFO *,
				ITEM_PANE_INFO *
				>
			PageItemPanesArr_t;

		PageItemPanesArr_t m_arrItems;

		LONG _PaneRemoveAllImpl();

		virtual void OnPageItemDrawEntire(
			CDC & dc
			) const;

	public:
		ITEM_PANE_INFO * HitTestPane( 
			const POINT & ptClient 
			) const;

		ITEM_PANE_INFO * PaneInsert(
			HWND hWnd,
			LONG nIndex = -1, // default - append
			__EXT_MFC_SAFE_LPCTSTR sText = NULL,
			INT nHeight = -1,
			bool bUpdate = false
			);

		ITEM_PANE_INFO * PaneGetInfo(
			LONG nIndex
			);
		const ITEM_PANE_INFO * PaneGetInfo(
			LONG nIndex
			) const;

		LONG PaneGetCount() const
		{
			ASSERT_VALID( this );
			return LONG( m_arrItems.GetSize() );
		}

		bool PaneRemove( 
			LONG nIndex,
			bool bUpdate = false 
			);

		LONG PaneRemoveAll(
			bool bUpdate = false
			);

		bool PaneExpand( 
			LONG nIndex,
			ITEM_PANE_INFO::e_ExpandType_t eExpandType,
			bool bUpdate = false
			);
		bool PaneIsExpanded( 
			LONG nIndex 
			) const;

		bool PaneExpandableGet( 
			LONG nIndex 
			) const;
		void PaneExpandableSet( 
			LONG nIndex,
			bool bSet = true
			);

		friend class CExtPageNavigatorOptionsDlg;

	}; // class PAGE_ITEM_INFO

	typedef
		CArray <
			PAGE_ITEM_INFO *,
			PAGE_ITEM_INFO *
			>
		PageItemsArr_t;

	friend class PAGE_ITEM_INFO;

	class __PROF_UIS_API SELECTION_NOTIFICATION
	{
	public:
		SELECTION_NOTIFICATION(
			bool bChangedFinally,
			LONG nPageOld,
			LONG nPageNew,
			LPARAM lParamCookie = NULL
			);
		bool m_bChangedFinally;
		LPARAM m_lParamCookie;
		LONG m_nPageOld;
		LONG m_nPageNew;
		operator WPARAM() const;
		static const SELECTION_NOTIFICATION * FromWPARAM( WPARAM wParam );
		friend class CExtPageNavigatorWnd;
	private:
		LRESULT Notify( HWND hWndNotify ) const;
	}; // class SELECTION_NOTIFICATION

protected:
	HFONT m_hFont;

// Operations
public:
	LPARAM m_lParamCookie;
	static const UINT g_nMsgSelectionNotification;
	HWND m_hWndNotificationReceiver;

	static HCURSOR g_hCursor;

	static bool RegisterWndClass();

	virtual bool Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( __EXT_MFC_IDC_STATIC ),
		DWORD dwWindowStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CCreateContext * pContext = NULL
		);
	
	void UpdatePageNavigatorWnd(
		bool bUpdate = false
		);

	bool ShowSplitter( 
		bool bShow = true,
		bool bUpdate = false
		);
	bool IsSplitterVisible() const;

	bool EnableSplitter( 
		bool bEnable = true,
		bool bUpdate = false
		);
	bool IsSplitterEnabled() const;

	bool ShowConfigButton( 
		bool bShow = true,
		bool bUpdate = false
		);
	bool IsConfigButtonVisible() const;

	virtual LONG HitTest( 
		const POINT & ptClient 
		) const;
	virtual PAGE_ITEM_INFO * HitTestItem( 
		const POINT & ptClient 
		) const;

	PAGE_ITEM_INFO * ItemInsert(
		LONG nIndex = -1, // default - append
		__EXT_MFC_SAFE_LPCTSTR sText = NULL,
		HICON hIconExpanded = NULL,
		HICON hIconCollapsed = NULL,
		DWORD dwData = 0,
		bool bUpdate = false
		);
	PAGE_ITEM_INFO * ItemInsert(
		LONG nIndex = -1, // default - append
		__EXT_MFC_SAFE_LPCTSTR sText = NULL,
		const CExtCmdIcon * pIconExpanded = NULL,
		const CExtCmdIcon * pIconCollapsed = NULL,
		DWORD dwData = 0,
		bool bUpdate = false
		);

	PAGE_ITEM_INFO * ItemGetInfo(
		LONG nIndex
		);
	const PAGE_ITEM_INFO * ItemGetInfo(
		LONG nIndex
		) const;
	bool ItemSetInfo(
		LONG nIndex,
		PAGE_ITEM_INFO * pPII
		);

	LONG ItemGetIndexOf(
		const PAGE_ITEM_INFO * pPII
		) const;

	LONG ItemRemoveAll( 
		bool bUpdate = false 
		);

	bool ItemRemove( 
		LONG nIndex,
		bool bUpdate = false 
		);

	LONG ItemGetCount() const;

	INT ItemGetExpandedCount() const;
	INT ItemGetCollapsedCount() const;
	
	INT ItemGetVisibleCount() const;
	INT ItemGetEnabledCount() const;
	
	void ItemSetExpandedCount( 
		INT nCount 
		);

	bool ItemEnabledGet( 
		LONG nIndex
		) const;
	bool ItemEnabledSet( 
		LONG nIndex,
		bool bEnabled = true,
		bool bPersistent = true,
		bool bUpdate = false 
		);

	void SelectionSet( 
		LONG nIndex 
		);
	void SelectionSet( 
		PAGE_ITEM_INFO * pPII 
		);
	LONG SelectionGet() const;

	PAGE_ITEM_INFO * ItemGetPressed() const;
	ITEM_PANE_INFO * ItemPaneGetPressed() const;
	
	INT MinPageAreaHeightGet() const;
	void MinPageAreaHeightSet( 
		INT nHeight 
		);

	bool ItemMove( 
		LONG nIndex1,
		LONG nIndex2,
		bool bPersistent = true,
		bool bUpdate = false 
		);

	bool ItemsSwap( 
		LONG nIndex1,
		LONG nIndex2,
		bool bPersistent = true,
		bool bUpdate = false 
		);
	
	LONG ItemFind(
		LPARAM lParam,
		LONG nIndexStart = -1
		) const;
	LONG ItemFind(
		__EXT_MFC_SAFE_LPCTSTR sText,
		LONG nIndexStart = -1
		) const;

	virtual bool StateLoad(
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile,
		bool bPersistent = true,
		HKEY hKeyRoot = HKEY_CURRENT_USER
		);
	virtual bool StateSave(
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile,
		HKEY hKeyRoot = HKEY_CURRENT_USER
		);
	virtual void StateReset(
		bool bPersistent = true
		);
	
	virtual void OnPageItemPaneDrawCaption(
		CDC & dc,
		const CRect & rcCaption,
		__EXT_MFC_SAFE_LPCTSTR strCaption,
		bool bHover,
		bool bPressed,
		bool bExpandable,
		bool bExpanded,
		bool bDrawTopLine = true
		) const;

	virtual INT OnQueryItemPageCaptionHeight() const;
	virtual INT OnQueryItemCaptionHeight() const;
	virtual INT OnQuerySplitterHeight() const;
	virtual INT OnQueryItemHeight() const;
	virtual INT OnQueryCollapsedAreaHeight() const;
	virtual CSize OnQueryCollapsedItemSize() const;

	virtual HWND OnPageNavigatorGetNotificationReceiver() const;

public:
	virtual void PmBridge_OnPaintManagerChanged(
		CExtPaintManager * pGlobalPM
		);

protected:
	static bool g_bWndClassRegistered;
	bool m_bDirectCreateCall:1;
	bool m_bInitialized:1;
	bool m_bUpdatingLayout:1;

	bool m_bItemsMouseHover:1;
	bool m_bSplitterMouseTracking:1;
	bool m_bSplitterMouseHover:1;
	bool m_bCanceling:1;

	CRect m_rcPageNavigatorClient;
	CRect m_rcPageNavigatorInnerArea;

	CRect m_rcItemCaption;
	CRect m_rcCollapsedItemsArea;
	CRect m_rcExpandedItemsArea;
	CRect m_rcSplitter;

	HCURSOR m_hSizeCursor;
	HCURSOR m_hHandCursor;

	static BYTE g_arrBmpDataShowMoreButtons[];
	static BYTE g_arrBmpDataShowFewerButtons[];

	CExtSafeString m_sCommandProfile;

	INT m_nMinPageAreaHeight;
	INT m_nExpandedItemsDesired;
	
	bool m_bSplitterVisible:1;
	bool m_bSplitterEnabled:1;

	PAGE_ITEM_INFO * m_pCurrentItem;

	PageItemsArr_t m_arrItems;

	CToolTipCtrl m_wndToolTip;

	virtual void _RecalcLayout();
	virtual bool _CreateHelper();
	bool _InitToolTip( 
		bool bActivate = false 
		);
	PAGE_ITEM_INFO * _GetConfigButton() const;

	void _ClearExpandedFlag();
	void _ClearPressedFlag();
	void _ClearHoverFlag();
	void _ClearVisibleFlag();
	void _ClearPanesPressedFlag();
	void _ClearPanesHoverFlag();

	INT _CalcMaxExpandedItemsCount();

	virtual PAGE_ITEM_INFO * _ItemGetInfoImpl(
		LONG nIndex
		);

	virtual bool _ItemRemoveImpl(
		LONG nIndex
		);

	virtual LONG _SelectionGetImpl() const;

	virtual LONG _ItemRemoveAllImpl( 
		bool bRemoveExpandButton = false 
		);

	virtual void OnPageNavigatorDrawEntire(
		CDC & dc,
		const CRect & rcClient
		) const;

	virtual void OnPageNavigatorEraseClientArea(
		CDC & dc,
		const CRect & rcClient
		) const;

	virtual void OnPageNavigatorDrawBorder(
		CDC & dc,
		const CRect & rcClient
		) const;

	virtual void OnPageNavigatorDrawItemCaptionArea(
		CDC & dc,
		const CRect & rcItemCaptionArea,
		__EXT_MFC_SAFE_LPCTSTR sText
		) const;

	virtual void OnPageNavigatorDrawCollapsedItemsArea(
		CDC & dc,
		const CRect & rcCollapsedItemsArea
		) const;

	virtual void OnPageNavigatorDrawExpandedItemsArea(
		CDC & dc,
		const CRect & rcExpandedItemsArea
		) const;

	virtual void OnPageNavigatorDrawSplitter(
		CDC & dc,
		const CRect & rcSplitter,
		bool bDrawDots = true
		) const;

	virtual void _ActivateOnClick();
	virtual bool _ProcessMouseMove( CPoint point );

	virtual bool _ProcessMouseClick(
		CPoint point,
		bool bButtonPressed,
		INT nMouseButton // MK_... values
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtPageNavigatorWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtPageNavigatorWnd();

	bool m_bAutoDeleteWindow:1;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtPageNavigatorWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSetFont( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetFont( WPARAM, LPARAM );
	//}}AFX_MSG
#if _MFC_VER < 0x700
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#else
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
#endif
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThemeChanged( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

}; // class CExtPageNavigatorWnd

__PROF_UIS_API void AFXAPI __EXT_MFC_DDX_PageNavigatorIndex( CDataExchange * pDX, INT nIDC, LONG & index );

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // (!defined __EXT_MFC_NO_PAGE_NAVIGATOR)

#endif // (!defined __EXT_PAGE_NAVIGATOR_H)
