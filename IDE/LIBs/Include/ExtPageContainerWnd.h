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

#if (!defined __EXT_PAGECONTAINERWND_H)
#define __EXT_PAGECONTAINERWND_H

#if (!defined __EXT_MFC_NO_PAGECONTAINER_CTRL)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#ifndef __AFXTEMPL_H__
	#include <AfxTempl.h>
#endif

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

#if (!defined __EXT_EXTCONTENTEXPANDWND_H)
	#include <ExtContentExpandWnd.h>
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXT_HOOK_H)
	#include "../Src/ExtHook.h"
#endif

#if (!defined __EXT_MOUSECAPTURESINK_H)
	#include <../Src/ExtMouseCaptureSink.h>
#endif

// page container control styles

#define __EPCWS_HORIZONTAL			0x00000001L // horizontal orientation
#define __EPCWS_SINGLE_EXPANDED		0x00000002L // allow only one page be expanded
#define __EPCWS_SINGLE_FULL_SIZE	0x00000004L // make single-expanded page max sized (only with __EPCWS_SINGLE_EXPANDED style)
#define __EPCWS_NOT_ALL_CONTRACTED	0x00000008L // always at least 1 expanded
#define __EPCWS_SCROLL_POS_AREA		0x00000010L // show scroll pos area at right/bottom
#define __EPCWS_PAGE_BORDERS		0x00000020L // enable 3DSMAX(R) like borders around pages and different style for captions
#define __EPCWS_NO_SCROLL			0x00000040L // do not scroll by drag operation
#define __EPCWS_CAPTION_DRAG		0x00000080L // allow start drag-n-drop captions
#define __EPCWS_CAPTION_HOVER		0x00000100L // change caption style on hover
#define __EPCWS_CAPTION_FLAT		0x00000200L // flat captions
#define __EPCWS_CAPTION_EXPAND		0x00000400L // expand page captions
#define __EPCWS_CAPTION_TOOLTIPS	0x00000800L // show tooltips on captions (only without __EPCWS_CAPTION_EXPAND)
#define __EPCWS_CAPTION_EXPBTN		0x00001000L // "+" or "-" sign on captions
#define __EPCWS_CAPTION_CENTER_TEXT	0x00002000L // center caption text
#define __EPCWS_CAPTION_NO_CLICKS	0x00004000L // do not expand/contract pages by user actions
#define __EPCWS_CAPTION_FOCUS		0x00008000L // set focus to page after caption clicked
#define __EPCWS_CAPTION_INV_VFONT	0x00010000L // invert vertical font (with __EPCWS_HORIZONTAL layout)
#define __EPCWS_CAPTION_EDIT		0x00020000L // edit captions (only without __EPCWS_HORIZONTAL)
#define __EPCWS_NO_EXPAND_ANIMATION	0x00040000L // do not perform expand animation in MS-Outlook(R) like mode (__EPCWS_SINGLE_EXPANDED|__EPCWS_SINGLE_FULL_SIZE|__EPCWS_NOT_ALL_CONTRACTED)
#define __EPCWS_NO_FOCUS_ON_CLICK	0x00080000L // do not request control focus (when clicking client area)
#define __EPCWS_USER_DEFINED_11		0x00100000L
#define __EPCWS_USER_DEFINED_10		0x00200000L
#define __EPCWS_USER_DEFINED_09		0x00400000L
#define __EPCWS_USER_DEFINED_08		0x00800000L
#define __EPCWS_USER_DEFINED_07		0x01000000L
#define __EPCWS_USER_DEFINED_06		0x02000000L
#define __EPCWS_USER_DEFINED_05		0x04000000L
#define __EPCWS_USER_DEFINED_04		0x08000000L
#define __EPCWS_USER_DEFINED_03		0x10000000L
#define __EPCWS_USER_DEFINED_02		0x20000000L
#define __EPCWS_USER_DEFINED_01		0x40000000L
#define __EPCWS_USER_DEFINED_00		0x80000000L

// outlook bar like
#define __EPCWS_STYLES_OUTLOOKBAR \
	(	__EPCWS_CAPTION_HOVER \
	|	__EPCWS_CAPTION_CENTER_TEXT \
	|	__EPCWS_SINGLE_EXPANDED \
	|	__EPCWS_SINGLE_FULL_SIZE \
	|	__EPCWS_NOT_ALL_CONTRACTED \
	/*|	__EPCWS_CAPTION_TOOLTIPS*/ \
	|	__EPCWS_NO_FOCUS_ON_CLICK\
	)

// outlook bar like + expanded captions
#define __EPCWS_STYLES_OUTLOOKBAR_BETTER \
	(	__EPCWS_STYLES_OUTLOOKBAR \
	|	__EPCWS_CAPTION_EXPAND \
	)

// 3d studio max bar like
#define __EPCWS_STYLES_3DSMAXBAR \
	(	__EPCWS_PAGE_BORDERS \
	|	__EPCWS_CAPTION_HOVER \
	|	__EPCWS_SCROLL_POS_AREA \
	|	__EPCWS_CAPTION_FOCUS \
	/*|	__EPCWS_CAPTION_TOOLTIPS*/ \
	)

// 3d studio max bar like + expanded captions + "+"/"-" glyph on captions
#define __EPCWS_STYLES_3DSMAXBAR_BETTER \
	(	__EPCWS_STYLES_3DSMAXBAR \
	|	__EPCWS_CAPTION_EXPBTN \
	|	__EPCWS_CAPTION_EXPAND \
	)

#define __EPCWS_STYLES_DEFAULT __EPCWS_STYLES_OUTLOOKBAR_BETTER|__EPCWS_CAPTION_EDIT
//#define __EPCWS_STYLES_DEFAULT \
//	(	0 \
//	/*|	__EPCWS_PAGE_BORDERS*/ \
//	/*|	__EPCWS_CAPTION_FLAT*/ \
//	/*|	__EPCWS_CAPTION_EXPBTN*/ \
//	|	__EPCWS_CAPTION_HOVER \
//	|	__EPCWS_SCROLL_POS_AREA \
//	/*|	__EPCWS_CAPTION_FOCUS*/ \
//	/*|	__EPCWS_CAPTION_DRAG*/ \
//	|	__EPCWS_CAPTION_EXPAND \
//	/*|	__EPCWS_CAPTION_TOOLTIPS*/ \
//	|	__EPCWS_SINGLE_EXPANDED \
//	|	__EPCWS_SINGLE_FULL_SIZE \
//	|	__EPCWS_NOT_ALL_CONTRACTED \
//	/*|	__EPCWS_NO_EXPAND_ANIMATION*/ \
//	)

// hit test codes

#define __EPCHT_NOWHERE				0x00000000L // nowhere
#define __EPCHT_ON_SCROLL_POS_AREA	0x00000001L // on scroll pos area
#define __EPCHT_ON_PAGE_CAPT_TEXT	0x00000002L // near text on caption
#define __EPCHT_ON_PAGE_CAPT_ICON	0x00000004L // on caption icon
#define __EPCHT_ON_PAGE_CAPT_GLYPH	0x00000008L // on caption "+" / "-" sign
#define __EPCHT_ON_PAGE_BORDER_L	0x00000010L // on page left border
#define __EPCHT_ON_PAGE_BORDER_R	0x00000020L // on page right border
#define __EPCHT_ON_PAGE_BORDER_T	0x00000040L // on page top border
#define __EPCHT_ON_PAGE_BORDER_B	0x00000080L // on page bottom border
#define __EPCHT_ON_PAGE_WND			0x00000100L // on page window area

#define __EPCHT_ON_PAGE_CAPT_ANY \
	(	__EPCHT_ON_PAGE_CAPT_TEXT \
	|	__EPCHT_ON_PAGE_CAPT_ICON \
	|	__EPCHT_ON_PAGE_CAPT_GLYPH \
	)

#define __EPCHT_ON_PAGE_BORDER_ANY \
	(	__EPCHT_ON_PAGE_BORDER_L \
	|	__EPCHT_ON_PAGE_BORDER_R \
	|	__EPCHT_ON_PAGE_BORDER_T \
	|	__EPCHT_ON_PAGE_BORDER_B \
	)

#define __EPCHT_ON_PAGE_ANY \
	(	__EPCHT_ON_PAGE_CAPT_ANY \
	|	__EPCHT_ON_PAGE_BORDER_ANY \
	|	__EPCHT_ON_PAGE_WND \
	)

/////////////////////////////////////////////////////////////////////////////
// CExtPageContainerWnd window

class __PROF_UIS_API CExtPageContainerWnd
	: public CWnd
	, public CExtHookSink
	, public CExtMouseCaptureSink
	, public CExtPmBridge
{
// Construction
public:
	CExtPageContainerWnd();

	DECLARE_DYNCREATE( CExtPageContainerWnd );
	DECLARE_CExtPmBridge_MEMBERS( CExtPageContainerWnd );

	class __PROF_UIS_API PAGE_ITEM_INFO
	{
	protected:
		HWND m_hWndPage;
		CExtCmdIcon m_icon;
		CExtSafeString m_sText;
		LPARAM m_lParam;
		bool m_bExpanded:1, m_bVisible:1,
			m_bCaptionHover:1, m_bCaptionPressed:1;
		CSize m_sizeCaptionLastMeasured, m_sizeTextLastMeasured;
		CRect m_rcCaption, m_rcPage, m_rcWnd, m_rcWndInitial;
	public:

		PAGE_ITEM_INFO(
			HWND hWndPage,
			__EXT_MFC_SAFE_LPCTSTR sText = NULL,
			LPARAM lParam = 0L,
			bool bExpanded = false
			);
		~PAGE_ITEM_INFO();

		CExtPageContainerWnd * GetPageContainerWnd();
		CExtPageContainerWnd * GetPageContainerWnd() const
		{
			return
				( const_cast < PAGE_ITEM_INFO * > ( this ) )
					-> GetPageContainerWnd();
		}

		CExtCmdIcon & IconGet();
		const CExtCmdIcon & IconGet() const;
		void IconSet( const CExtCmdIcon & _icon );
		void IconSet( HICON hIcon, bool bCopyIcon );

		__EXT_MFC_SAFE_LPCTSTR TextGet() const;
		void TextSet( __EXT_MFC_SAFE_LPCTSTR sText );

		CFont * GetCaptionFont(
			bool bHorzLayout
			);

		bool IsExpanded() const
		{
			return m_bExpanded;
		}
		bool IsVisible() const
		{
			return m_bVisible;
		}
		bool IsCaptionHover() const
		{
			return m_bCaptionHover;
		}
		bool IsCaptionPressed() const
		{
			return m_bCaptionPressed;
		}

		CSize GetLastMeasuredCaptionSize() const
		{
			return m_sizeCaptionLastMeasured;
		}
		CSize GetLastMeasuredTextSize() const
		{
			return m_sizeTextLastMeasured;
		}

		const CRect & GetRectCaption() const
		{
			return m_rcCaption;
		}
		const CRect & GetRectPage() const
		{
			return m_rcPage;
		}
		const CRect & GetRectWnd() const
		{
			return m_rcWnd;
		}
		const CRect & GetRectInitial() const
		{
			return m_rcWndInitial;
		}
		void SetRectInitial( const RECT & rcWndInitial )
		{
			m_rcWndInitial = rcWndInitial;
		}

		LPARAM LPARAM_Get() const
		{
			return m_lParam;
		}
		LPARAM LPARAM_Set( LPARAM lParam )
		{
			LPARAM lParamOld = m_lParam;
			m_lParam = lParam;
			return lParamOld;
		}

		HWND GetSafePageHwnd() const
		{
			HWND hWnd = (HWND)m_hWndPage;
			if( hWnd != NULL
				&& ::IsWindow( hWnd )
				)
				return hWnd;
			return NULL;
		}

	protected:
		CSize MeasureCaption(
			CDC & dcCalcLayout,
			bool bHorzLayout,
			PAGE_ITEM_INFO * pPageInfoPrev
			);
		
		friend class CExtPageContainerWnd;
	}; // class PAGE_ITEM_INFO

	class __PROF_UIS_API HIT_TEST_INFO
	{
	public:
		CPoint m_ptClient;
		LONG m_nItemIndex;
		PAGE_ITEM_INFO * m_pPageInfo;
		DWORD m_dwHitTestCode;
	protected:
		void _AssignFromOther(
			const HIT_TEST_INFO & other
			)
		{
			m_ptClient = other.m_ptClient;
			m_nItemIndex = other.m_nItemIndex;
			m_pPageInfo = other.m_pPageInfo;
			m_dwHitTestCode = other.m_dwHitTestCode;
		}
		bool _CompareWithOther(
			const HIT_TEST_INFO & other
			) const
		{
			if( m_dwHitTestCode != other.m_dwHitTestCode )
				return false;
			if( (m_dwHitTestCode & __EPCHT_ON_PAGE_ANY) == 0 )
				return true;
			if( m_pPageInfo == other.m_pPageInfo )
				return true;
			return false;
		}
	public:
		void Clear( bool bClearPoint )
		{
			if( bClearPoint )
				m_ptClient.x = m_ptClient.y = 0;
			m_nItemIndex = -1L;
			m_pPageInfo = NULL;
			m_dwHitTestCode = __EPCHT_NOWHERE;
		}
		HIT_TEST_INFO(
			int x = 0,
			int y = 0
			)
		{
			Clear( true );
			m_ptClient.x = x;
			m_ptClient.y = y;
		}
		HIT_TEST_INFO(
			const POINT & ptClient
			)
		{
			Clear( true );
			m_ptClient.x = ptClient.x;
			m_ptClient.y = ptClient.y;
		}
		HIT_TEST_INFO(
			const HIT_TEST_INFO & other
			)
		{
			_AssignFromOther( other );
		}
		~HIT_TEST_INFO()
		{
		}
		HIT_TEST_INFO & operator = (
			HIT_TEST_INFO & other
			)
		{
			_AssignFromOther( other );
			return * this;
		}
		bool operator == (
			const HIT_TEST_INFO & other
			) const
		{
			return _CompareWithOther( other );
		}
		bool operator != (
			const HIT_TEST_INFO & other
			) const
		{
			return !_CompareWithOther( other );
		}
	}; // class HIT_TEST_INFO

	class __PROF_UIS_API CItemTextEditWnd : public CEdit
	{
		PAGE_ITEM_INFO * m_pPageInfo;
		bool m_bInEndEdit:1;
	public:

		CItemTextEditWnd(
			PAGE_ITEM_INFO * pPageInfo
			)
			: m_pPageInfo( pPageInfo )
			, m_bInEndEdit( false )
		{
			ASSERT( m_pPageInfo != NULL );
		}
		
		// CEdit syntax of Create() method
		BOOL Create(
			DWORD dwStyle,
			const RECT & rect,
			CWnd * pParentWnd,
			UINT nID
			);
		virtual void PostNcDestroy();

	protected:

		void DoEndEdit( bool bUpdateItemText );
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	}; // class CItemTextEditWnd

// Attributes
public:
	static UINT g_nMsgQueryCancelResetDragCursor;

	static bool g_bAnimationAccelerated;
	static LONG g_nAnimationStepCount;
	static LONG g_ctAnimationStepClocks;

protected:
	HFONT m_hFont;
	static HCURSOR g_hCursorDragScrollOnH;
	static HCURSOR g_hCursorDragScrollOnV;
	static HCURSOR g_hCursorDragScrollOff;
	static HCURSOR g_hCursorCaption;
	static bool g_bPageContainerWndClassRegistered;
	CToolTipCtrl m_wndToolTip;
	CExtContentExpandWnd m_wndContentExpand;
	DWORD m_dwPageContainerStyle;
	bool m_bDirectCreateCall:1;
	bool m_bDelayRecalcLayout:1;
	bool m_bInLayoutRecalcProc:1;
	bool m_bCancelingActionsMode:1;
	bool m_bDragScrolling:1;
	bool m_bInExpandingAnimationSF:1;
	LONG m_nScrollPos, m_nScrollSize, m_nScrollDragStart;
	CRect m_rcPageBorderSizes;
	CSize m_sizeBorderBeforeAfterSpaces;
	LONG m_nSpaceCaption2Wnd;
	CRect m_rcScrollPosArea;
	CExtPageContainerWnd::PAGE_ITEM_INFO
		* m_pPageInfoHover,
		* m_pPageInfoPressed,
		* m_pPageInfoEnsureVisible;
	CPoint m_ptStartLeftBtnTrack;
	HWND m_hWndEditor;

	typedef
		CArray <
			PAGE_ITEM_INFO *,
			PAGE_ITEM_INFO *
			>
		pages_vector_t;

	pages_vector_t m_vPages;

// Operations
public:

	bool RegisterPageContainerWndClass();

	BOOL Create(
		CWnd * pWndParent,
		UINT nDlgCtrlID = (UINT)__EXT_MFC_IDC_STATIC,
		CRect rect = CRect( 0, 0, 0, 0 ),
		DWORD dwPageContainerStyle = __EPCWS_STYLES_DEFAULT,
		DWORD dwWindowStyle = WS_CHILD|WS_VISIBLE,
		CCreateContext * pContext = NULL
		);

	virtual void UpdatePageContainerWnd(
		bool bUpdateLayoutNow
		);

protected:
	bool _CreateHelper();
	virtual void _RecalcLayoutImpl();
	void _CancelActions();
	void _AdjustWithPageContainerStyle();
	static bool _SetCursorDragScroll(
		bool bDragScrollingNow,
		bool bHorzLayout
		);
	static bool _SetCursorCaption();
	bool _ProcessMouseWheel( short zDelta, bool bDirectMsg );

	virtual void DoPaint( 
		CDC * pDC,
		CRect & rcClient
		);

protected:
	virtual CExtCmdIcon & OnPageContainerQueryItemIcon(
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo
		);
	virtual __EXT_MFC_SAFE_LPCTSTR OnPageContainerQueryItemText(
		const CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo
		);
	virtual void OnPageContainerDoItemTextChanging(
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo,
		CExtSafeString & sItemText,
		__EXT_MFC_SAFE_LPCTSTR sNewText
		);
	virtual CFont * OnPageContainerQueryItemFont(
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo,
		bool bHorzLayout
		);
	virtual void OnPageContainerItemInserted(
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo
		);
	virtual bool OnPageContainerItemRemoving(
		LONG nIndex,
		bool bDestroyPageWindow
		);
	virtual void OnPageContainerItemRemoved(
		LONG nIndex,
		bool bDestroyPageWindow,
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo
		);
	virtual bool OnPageContainerItemExpanding(
		LONG nIndex,
		bool bExpand
		);
	virtual void OnPageContainerItemExpanded(
		LONG nIndex,
		bool bExpand
		);
	virtual void OnPageContainerMeasurePageBorderInfo(
		CDC & dcCalcLayout,
		CRect & rcPageBorderSizes,
		CSize & sizeBorderBeforeAfterSpaces,
		LONG & nSpaceCaption2Wnd,
		LONG & nScrollPosAreaMetric,
		bool bHorzLayout
		);
	virtual void OnPageContainerMeasureCaptionSize(
		CDC & dcCalcLayout,
		bool bHorzLayout,
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo,
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfoPrev,
		CSize & sizeCaption,
		CSize & sizeText
		);
	virtual void OnPageContainerEraseEntire(
		CDC & dc,
		CRect rcClient
		);
	virtual void OnPageContainerDrawEntire(
		CDC & dc,
		CRect rcClient
		);
	virtual void OnPageContainerEraseItem(
		CDC & dc,
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo,
		CRect rcPage,
		CRect rcCaption,
		CRect rcWnd
		);
	virtual void OnPageContainerDrawItem(
		CDC & dc,
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo,
		CRect rcPage,
		CRect rcCaption,
		CRect rcWnd
		);
	virtual void OnPageContainerEraseCaption(
		CDC & dc,
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo,
		CRect rcCaption
		);
	virtual void OnPageContainerDrawCaption(
		CDC & dc,
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo,
		CRect rcCaption
		);
	virtual void OnPageContainerEraseScrollPosArea(
		CDC & dc,
		CRect rcScrollPosArea,
		LONG nScrollPos,
		LONG nScrollSize
		);
	virtual void OnPageContainerDrawScrollPosArea(
		CDC & dc,
		CRect rcScrollPosArea,
		LONG nScrollPos,
		LONG nScrollSize
		);
	virtual void OnPageContainerHitTest(
		CExtPageContainerWnd::HIT_TEST_INFO & _pht
		) const;

	virtual bool OnPageContainerCanDoHoverActions();
	virtual void OnPageContainerCaptionHoverStart(
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo
		);
	virtual void OnPageContainerCaptionHoverStop(
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo
		);

	virtual void OnPageContainerCaptionPressedStart(
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo
		);
	virtual void OnPageContainerCaptionPressedStop(
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo
		);

	virtual void OnPageContainerAdjustMouseWheelParms(
		LONG & zDelta
		);
	virtual bool OnPageContainerIsDragablePosition(
		const POINT & ptScreen
		) const;

	virtual bool OnPageContainerCanDoCaptionDrag(
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo,
		CPoint ptStartLeftBtnTrack,
		CPoint ptCurrentLeftBtn,
		UINT nMouseMoveFlags
		);
	virtual void OnPageContainerStartCaptionDrag(
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo,
		CPoint ptStartLeftBtnTrack,
		CPoint ptCurrentLeftBtn,
		UINT nMouseMoveFlags
		);
	virtual void OnPageContainerActivateContentExpandWnd(
		CExtContentExpandWnd & wndContentExpand,
		const RECT & rcItemLess, // in client
		const RECT & rcItemGreater, // in client
		DWORD dwCookieCmd, // values [0..1000] are reserved
		LPARAM lParam
		);

	virtual bool OnPageContainerSetCursorOnDragableArea(
		bool bDragScrollingNow
		);
	virtual bool OnPageContainerSetCursorOnCaption(
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo
		);

	virtual bool OnPageContainerHookPageWndMsg(
		LRESULT & lResult,
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo,
		UINT nMessage,
		WPARAM & wParam,
		LPARAM & lParam
		);
public:
	virtual CExtPaintManager::glyph_t &
		OnPageContainerQueryItemGlyph(
			const CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo,
			bool bExpandedGlyph
			) const;
protected:
	virtual bool OnPageContainerQueryCancelExpandAnimation();
	virtual HWND OnPageContainerStartItemEditor(
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo
		);
	void OnPageContainerCancelItemEditor(
		HWND hWndEditor
		);
public:
	virtual bool OnPageContainerDeactivateEditor(
		CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo,
		bool bUpdateItemText
		);

protected:
	virtual bool OnHookWndMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		UINT nMessage,
		WPARAM & wParam,
		LPARAM & lParam
		);

public:

	LONG PageInsert(
		HWND hWndPage,
		LONG nIndexInsertBefore = -1L, // append
		__EXT_MFC_SAFE_LPCTSTR sPageCaptionText = NULL,
		HICON hCaptionIcon = NULL,
		bool bCopyCaptionIcon = false,
		LPARAM lParam = 0L,
		bool bExpanded = true,
		bool bEnsureVisible = false,
		bool bUpdateLayoutNow = false
		);
	LONG PageInsert(
		HWND hWndPage,
		const CExtCmdIcon & iconCaption,
		LONG nIndexInsertBefore = -1L, // append
		__EXT_MFC_SAFE_LPCTSTR sPageCaptionText = NULL,
		LPARAM lParam = 0L,
		bool bExpanded = true,
		bool bEnsureVisible = false,
		bool bUpdateLayoutNow = false
		);
	LONG PageRemove(
		LONG nIndex,
		LONG nCount = 1L,
		bool bDestroyPageWindow = true,
		bool bUpdateLayoutNow = false
		);
	void PageRemoveAll(
		bool bDestroyPageWindow = true,
		bool bUpdateLayoutNow = false
		);
	LONG PageGetCount() const;
	CExtPageContainerWnd::PAGE_ITEM_INFO * PageGetInfo(
		LONG nIndex
		);
	const CExtPageContainerWnd::PAGE_ITEM_INFO * PageGetInfo(
		LONG nIndex
		) const;
	LONG PageGetIndexOf(
		const CExtPageContainerWnd::PAGE_ITEM_INFO * pPageInfo
		) const;
	LONG PageFind( HWND hWnd ) const;
	void PageGetLayoutRectInfo(
		LONG nIndex,
		RECT * pRectPage,
		RECT * pRectCaption = NULL,
		RECT * pRectWnd = NULL
		) const;

	bool PageIsVisible( LONG nIndex ) const;
	bool PageShow(
		LONG nIndex,
		bool bShow = true,
		bool bEnsureVisible = false,
		bool bUpdateLayoutNow = false
		);
	bool PageIsExpanded( LONG nIndex ) const;
	bool PageExpand(
		LONG nIndex,
		bool bExpand = true,
		bool bEnsureVisible = false,
		bool bUpdateLayoutNow = false
		);
	bool PageEnsureVisible(
		LONG nIndex,
		bool bUpdateLayoutNow = false
		);
	CExtPageContainerWnd::HIT_TEST_INFO &
		PageHitTest(
			CExtPageContainerWnd::HIT_TEST_INFO & _pht
			) const;

	LONG PageGetNext(
		LONG nStartIndex = -1L,
		bool bOnlyExpanded = false,
		bool bOnlyVisible = false,
		bool bNext = true,
		bool bOnlyEnabled = false
		) const;

	LONG ScrollPosGet() const
	{
		return m_nScrollPos;
	}
	LONG ScrollSizeGet() const
	{
		return m_nScrollSize;
	}
	virtual void ScrollPosAdjust(
		LONG zDelta,
		bool bUpdateLayoutNow = true
		);
	
	bool IsInExpandingAnimation() const
	{
		return m_bInExpandingAnimationSF;
	}

	bool IsDragScrolling() const
	{
		return m_bDragScrolling;
	}

	bool CanDoLayoutMouseScrolling() const
	{
		ASSERT_VALID( this );
		if( (m_dwPageContainerStyle
				& (__EPCWS_SINGLE_EXPANDED|__EPCWS_SINGLE_FULL_SIZE)
				)
			==	  (__EPCWS_SINGLE_EXPANDED|__EPCWS_SINGLE_FULL_SIZE)
			)
			return false;
		return true;
	}
	
	bool HaveScrollPosArea() const
	{
		ASSERT_VALID( this );
		if( !CanDoLayoutMouseScrolling() )
			return false;
		if( m_dwPageContainerStyle & __EPCWS_SCROLL_POS_AREA )
			return true;
		return false;
	}

	bool IsHorizontalLayout() const
	{
		ASSERT_VALID( this );
		if( m_dwPageContainerStyle & __EPCWS_HORIZONTAL )
			return true;
		return false;
	}

	DWORD GetPageContainerStyle() const;
	DWORD ModifyPageContainerStyle(
		DWORD dwAdd,
		DWORD dwRemove = 0L,
		bool bUpdateLayoutNow = false
		);

	CToolTipCtrl & GetToolTipCtrl()
	{
		return m_wndToolTip;
	}
	const CToolTipCtrl & GetToolTipCtrl() const
	{
		return m_wndToolTip;
	}
	CExtContentExpandWnd & GetContentExpandWnd()
	{
		return m_wndContentExpand;
	}
	const CExtContentExpandWnd & GetContentExpandWnd() const
	{
		return m_wndContentExpand;
	}
	
	HWND GetEditorHWND() const
	{
		return
			( const_cast < CExtPageContainerWnd * > ( this ) )
				-> m_hWndEditor;
	}


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtPageContainerWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtPageContainerWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtPageContainerWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnSetFont( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetFont( WPARAM, LPARAM );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class PAGE_ITEM_INFO;
	
	afx_msg LRESULT _OnPaintExpanedItemContent(WPARAM wParam, LPARAM lParam);

#if _MFC_VER < 0x700
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#else
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
#endif
}; // class CExtPageContainerWnd

/////////////////////////////////////////////////////////////////////////////

#define __EXT_PAGECONTAINERWND_CLASS_NAME _T("ProfUIS-PageContainerWnd")

#define __EXT_PAGECONTAINERWND_DEFPGB_LEFT			6
#define __EXT_PAGECONTAINERWND_DEFPGB_TOP			6
#define __EXT_PAGECONTAINERWND_DEFPGB_RIGHT			6
#define __EXT_PAGECONTAINERWND_DEFPGB_BOTTOM		6
#define __EXT_PAGECONTAINERWND_MINPAGEEXT_DX		2
#define __EXT_PAGECONTAINERWND_MINPAGEEXT_DY		2

#define __EXT_PAGECONTAINERWND_DEFCAPT_DX_BEFORE	4
#define __EXT_PAGECONTAINERWND_DEFCAPT_DX_AFTER		4
#define __EXT_PAGECONTAINERWND_DEFCAPT_DY_BEFORE	4
#define __EXT_PAGECONTAINERWND_DEFCAPT_DY_AFTER		4

#define __EXT_PAGECONTAINERWND_CAPT_BEFORE_DX		2
#define __EXT_PAGECONTAINERWND_CAPT_BEFORE_DY		2
#define __EXT_PAGECONTAINERWND_CAPT_AFTER_DX		2
#define __EXT_PAGECONTAINERWND_CAPT_AFTER_DY		2
#define __EXT_PAGECONTAINERWND_CAPT_ICON2TEXT_DX	2
#define __EXT_PAGECONTAINERWND_CAPT_ICON2TEXT_DY	2
#define __EXT_PAGECONTAINERWND_CAPT_AFTER_GLYPH_DX	2
#define __EXT_PAGECONTAINERWND_CAPT_AFTER_GLYPH_DY	2

#define __EXT_PAGECONTAINERWND_CAPT_INNER_MIN_DX	15
#define __EXT_PAGECONTAINERWND_CAPT_INNER_MIN_DY	15
#define __EXT_PAGECONTAINERWND_CAPT_HBORDER_DX		2
#define __EXT_PAGECONTAINERWND_CAPT_HBORDER_DY		2
#define __EXT_PAGECONTAINERWND_CAPT_VBORDER_DX		2
#define __EXT_PAGECONTAINERWND_CAPT_VBORDER_DY		2

#define __EXT_PAGECONTAINERWND_CAPT2WND_DX			1
#define __EXT_PAGECONTAINERWND_CAPT2WND_DY			1

#define __EXT_PAGECONTAINERWND_SCROLLPOSAREA_DX		6
#define __EXT_PAGECONTAINERWND_SCROLLPOSAREA_DY		6

#define __EXT_PAGECONTAINERWND_HOVER_TIMER_ID				666
#define __EXT_PAGECONTAINERWND_HOVER_TIMER_PERIOD			20
#define __EXT_PAGECONTAINERWND_DELAY_UPDATE_TIMER_ID		667
#define __EXT_PAGECONTAINERWND_DELAY_UPDATE_TIMER_PERIOD	1

#define __EXT_PAGECONTAINERWND_MIN_EDITOR_DX		10
#define __EXT_PAGECONTAINERWND_MIN_EDITOR_DY		10

#endif //  (!defined __EXT_MFC_NO_PAGECONTAINER_CTRL)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __EXT_PAGECONTAINERWND_H


