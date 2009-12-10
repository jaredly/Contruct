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

#if (!defined __EXT_TABWND_H)
#define __EXT_TABWND_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_HOOK_H)
	#include "../Src/ExtHook.h"
#endif

#if (!defined __EXT_MOUSECAPTURESINK_H)
	#include <../Src/ExtMouseCaptureSink.h>
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXTEMPL_H__
	#include <AfxTempl.h>
#endif

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtTabWnd window

// tab item styles
#define __ETWI_INVISIBLE					0x00000001L
#define __ETWI_GROUP_START					0x00000002L
#define __ETWI_SELECTED						0x00000004L
#define __ETWI_IN_GROUP_ACTIVE				0x00000008L
#define __ETWI_DYNTAB_TEMPORARY				0x00000010L // internal, for dynamic tab control bars
#define __ETWI_DISABLED						0x00000020L
#define __ETWI_CENTERED_TEXT				0x00000040L

#define __ETWI_USER_STYLE_24				0x00000080L
#define __ETWI_USER_STYLE_23				0x00000100L
#define __ETWI_USER_STYLE_22				0x00000200L
#define __ETWI_USER_STYLE_21				0x00000400L
#define __ETWI_USER_STYLE_20				0x00000800L
#define __ETWI_USER_STYLE_19				0x00001000L
#define __ETWI_USER_STYLE_18				0x00002000L
#define __ETWI_USER_STYLE_17				0x00004000L
#define __ETWI_USER_STYLE_16				0x00008000L
#define __ETWI_USER_STYLE_15				0x00010000L
#define __ETWI_USER_STYLE_14				0x00020000L
#define __ETWI_USER_STYLE_13				0x00040000L
#define __ETWI_USER_STYLE_12				0x00080000L
#define __ETWI_USER_STYLE_11				0x00100000L
#define __ETWI_USER_STYLE_10				0x00200000L
#define __ETWI_USER_STYLE_09				0x00400000L
#define __ETWI_USER_STYLE_08				0x00800000L
#define __ETWI_USER_STYLE_07				0x01000000L
#define __ETWI_USER_STYLE_06				0x02000000L
#define __ETWI_USER_STYLE_05				0x04000000L
#define __ETWI_USER_STYLE_04				0x08000000L
#define __ETWI_USER_STYLE_03				0x10000000L
#define __ETWI_USER_STYLE_02				0x20000000L
#define __ETWI_USER_STYLE_01				0x40000000L
#define __ETWI_USER_STYLE_00				0x80000000L

// extended tab item styles
#define __ETWI_EX_NO_CLOSE_ON_TAB			0x00000001L

#define __ETWI_EX_USER_STYLE_30				0x00000002L
#define __ETWI_EX_USER_STYLE_29				0x00000004L
#define __ETWI_EX_USER_STYLE_28				0x00000008L
#define __ETWI_EX_USER_STYLE_27				0x00000010L
#define __ETWI_EX_USER_STYLE_26				0x00000020L
#define __ETWI_EX_USER_STYLE_25				0x00000040L
#define __ETWI_EX_USER_STYLE_24				0x00000080L
#define __ETWI_EX_USER_STYLE_23				0x00000100L
#define __ETWI_EX_USER_STYLE_22				0x00000200L
#define __ETWI_EX_USER_STYLE_21				0x00000400L
#define __ETWI_EX_USER_STYLE_20				0x00000800L
#define __ETWI_EX_USER_STYLE_19				0x00001000L
#define __ETWI_EX_USER_STYLE_18				0x00002000L
#define __ETWI_EX_USER_STYLE_17				0x00004000L
#define __ETWI_EX_USER_STYLE_16				0x00008000L
#define __ETWI_EX_USER_STYLE_15				0x00010000L
#define __ETWI_EX_USER_STYLE_14				0x00020000L
#define __ETWI_EX_USER_STYLE_13				0x00040000L
#define __ETWI_EX_USER_STYLE_12				0x00080000L
#define __ETWI_EX_USER_STYLE_11				0x00100000L
#define __ETWI_EX_USER_STYLE_10				0x00200000L
#define __ETWI_EX_USER_STYLE_09				0x00400000L
#define __ETWI_EX_USER_STYLE_08				0x00800000L
#define __ETWI_EX_USER_STYLE_07				0x01000000L
#define __ETWI_EX_USER_STYLE_06				0x02000000L
#define __ETWI_EX_USER_STYLE_05				0x04000000L
#define __ETWI_EX_USER_STYLE_04				0x08000000L
#define __ETWI_EX_USER_STYLE_03				0x10000000L
#define __ETWI_EX_USER_STYLE_02				0x20000000L
#define __ETWI_EX_USER_STYLE_01				0x40000000L
#define __ETWI_EX_USER_STYLE_00				0x80000000L

// tab window styles
#define __ETWS_ORIENT_TOP					0x00000000L
#define __ETWS_ORIENT_BOTTOM				0x00000001L
#define __ETWS_ORIENT_LEFT					0x00000002L
#define __ETWS_ORIENT_RIGHT					0x00000003L
#define __ETWS_ORIENT_MASK					0x00000003L

#define __ETWS_EQUAL_WIDTHS					0x00000004L
#define __ETWS_SHOW_BTN_HELP				0x00000008L
#define __ETWS_SHOW_BTN_CLOSE				0x00000010L
#define __ETWS_INVERT_VERT_FONT				0x00000020L
#define __ETWS_BOLD_SELECTION				0x00000040L
#define __ETWS_GROUPED						0x00000080L

#define __ETWS_ENABLED_BTN_HELP				0x00000100L
#define __ETWS_ENABLED_BTN_CLOSE			0x00000200L
#define __ETWS_ENABLED_BTN_UP				0x00000400L
#define __ETWS_ENABLED_BTN_DOWN				0x00000800L

#define __ETWS_CENTERED_TEXT				0x00001000L
#define __ETWS_FULL_WIDTH					0x00002000L
#define __ETWS_AUTOHIDE_SCROLL				0x00004000L
#define __ETWS_HOVER_FOCUS					0x00008000L

#define __ETWS_SCROLL_BY_PAGE				0x00010000L
#define __ETWS_FT_NO_ITEMS_INCLINE_BEFORE	0x00020000L
#define __ETWS_FT_NO_ITEMS_INCLINE_AFTER	0x00040000L
#define __ETWS_SHOW_BTN_SCROLL_HOME			0x00080000L
#define __ETWS_SHOW_BTN_SCROLL_END			0x00100000L
#define __ETWS_SHOW_BTN_TAB_LIST			0x00200000L

#define __ETWS_ENABLED_BTN_TAB_LIST			0x00400000L
#define __ETWS_GROUPED_EXPAND_ITEMS			0x00800000L
#define __ETWS_ITEM_DRAGGING				0x01000000L
#define __ETWS_HIDE_ICONS					0x02000000L

#define __ETWS_ENABLED_BTN_SCROLL_HOME		0x04000000L
#define __ETWS_ENABLED_BTN_SCROLL_END		0x08000000L
#define __ETWS_MULTI_ROW_COLUMN				0x10000000L
#define __ETWS_ALIGN_FONT_DIFF				0x20000000L

#define __ETWS_USER_STYLE_01				0x40000000L
#define __ETWS_USER_STYLE_00				0x80000000L

// extended tab window styles
#define __ETWS_EX_CLOSE_ON_TABS				0x00000001L
#define __ETWS_EX_CLOSE_ON_SELECTED_ONLY	0x00000002L

#define __ETWS_EX_USER_STYLE_29				0x00000004L
#define __ETWS_EX_USER_STYLE_28				0x00000008L
#define __ETWS_EX_USER_STYLE_27				0x00000010L
#define __ETWS_EX_USER_STYLE_26				0x00000020L
#define __ETWS_EX_USER_STYLE_25				0x00000040L
#define __ETWS_EX_USER_STYLE_24				0x00000080L
#define __ETWS_EX_USER_STYLE_23				0x00000100L
#define __ETWS_EX_USER_STYLE_22				0x00000200L
#define __ETWS_EX_USER_STYLE_21				0x00000400L
#define __ETWS_EX_USER_STYLE_20				0x00000800L
#define __ETWS_EX_USER_STYLE_19				0x00001000L
#define __ETWS_EX_USER_STYLE_18				0x00002000L
#define __ETWS_EX_USER_STYLE_17				0x00004000L
#define __ETWS_EX_USER_STYLE_16				0x00008000L
#define __ETWS_EX_USER_STYLE_15				0x00010000L
#define __ETWS_EX_USER_STYLE_14				0x00020000L
#define __ETWS_EX_USER_STYLE_13				0x00040000L
#define __ETWS_EX_USER_STYLE_12				0x00080000L
#define __ETWS_EX_USER_STYLE_11				0x00100000L
#define __ETWS_EX_USER_STYLE_10				0x00200000L
#define __ETWS_EX_USER_STYLE_09				0x00400000L
#define __ETWS_EX_USER_STYLE_08				0x00800000L
#define __ETWS_EX_USER_STYLE_07				0x01000000L
#define __ETWS_EX_USER_STYLE_06				0x02000000L
#define __ETWS_EX_USER_STYLE_05				0x04000000L
#define __ETWS_EX_USER_STYLE_04				0x08000000L
#define __ETWS_EX_USER_STYLE_03				0x10000000L
#define __ETWS_EX_USER_STYLE_02				0x20000000L
#define __ETWS_EX_USER_STYLE_01				0x40000000L
#define __ETWS_EX_USER_STYLE_00				0x80000000L

// default set of styles for normal tab window
#define __ETWS_DEFAULT \
	( __ETWS_ORIENT_TOP \
	| __ETWS_ENABLED_BTN_TAB_LIST \
	| __ETWS_ENABLED_BTN_HELP \
	| __ETWS_ENABLED_BTN_CLOSE \
	| __ETWS_AUTOHIDE_SCROLL \
	)

// hit test values
#define __ETWH_NOWHERE						(-1L)
#define __ETWH_BORDER_AREA					(-2L)
#define __ETWH_ITEMS_AREA					(-3L)
#define __ETWH_BUTTON_LEFTUP				(-4L)
#define __ETWH_BUTTON_RIGHTDOWN				(-5L)
#define __ETWH_BUTTON_HELP					(-6L)
#define __ETWH_BUTTON_CLOSE					(-7L)
#define __ETWH_BUTTON_TAB_LIST				(-8L)
#define __ETWH_BUTTON_SCROLL_HOME			(-9L)
#define __ETWH_BUTTON_SCROLL_END			(-10L)
#define __ETWH_TAB_FIRST					(0L)

#if (!defined __EXT_MFC_NO_TAB_CTRL)

class CExtPopupMenuTipWnd;

class __PROF_UIS_API CExtTabWnd
	: public CWnd
	, public CExtMouseCaptureSink
	, public CExtPmBridge
{
	CRuntimeClass * m_pHelperLastPaintManagerRTC;

protected:
	CToolTipCtrl m_wndToolTip;
// Construction
public:

	DECLARE_DYNCREATE( CExtTabWnd );
	DECLARE_CExtPmBridge_MEMBERS_GENERIC( CExtTabWnd );

	CExtTabWnd();

	virtual bool _IsMdiTabCtrl() const;
	virtual bool _IsCustomLayoutTabWnd() const;


	class __PROF_UIS_API TAB_ITEM_INFO : public CObject
	{
	protected:
		CExtTabWnd * m_pWndTab;
		DWORD m_dwItemStyle, m_dwItemStyleEx;
		CExtSafeString m_sText;
		CExtSafeString m_sTooltipText;
		CExtCmdIcon m_icon;
		CSize m_sizeLastMeasuredItem, m_sizeLastMeasuredText, m_sizeLastMeasuredIcon;
		CRect m_rcItem, m_rcCloseButton;
		LPARAM m_lParam;
		mutable CObject * m_pEventProvider;
		bool m_bHelperToolTipAvail:1, m_bMultiRowWrap:1;

		TAB_ITEM_INFO * m_pPrev, * m_pNext;

		void _AssignFromOther(
			const TAB_ITEM_INFO & other
			);

	public:
		TAB_ITEM_INFO(
			CExtTabWnd * pWndTab = NULL,
			TAB_ITEM_INFO * pPrev = NULL,
			TAB_ITEM_INFO * pNext = NULL,
			__EXT_MFC_SAFE_LPCTSTR sText = NULL,
			const CExtCmdIcon * pIcon = NULL, // CHANGED 2.53
			DWORD dwStyle = 0, // visible & non-group
			LPARAM lParam = 0,
			__EXT_MFC_SAFE_LPCTSTR sTooltipText = NULL			
			);
		TAB_ITEM_INFO( const TAB_ITEM_INFO & other );
		~TAB_ITEM_INFO();
		TAB_ITEM_INFO & operator = (
			const TAB_ITEM_INFO & other
			)
		{
			_AssignFromOther( other );
			return *this;
		}

		CExtTabWnd * GetTabWnd();
		const CExtTabWnd * GetTabWnd() const;
		operator CExtTabWnd * ()
		{
			return GetTabWnd();
		}
		operator const CExtTabWnd * () const
		{
			return GetTabWnd();
		}

		__EXT_MFC_SAFE_LPCTSTR TextGet() const;
		operator __EXT_MFC_SAFE_LPCTSTR () const
		{
			return TextGet();
		}
		void TextSet(
			__EXT_MFC_SAFE_LPCTSTR sText = NULL
			);
		TAB_ITEM_INFO & operator = (
			__EXT_MFC_SAFE_LPCTSTR sText
			)
		{
			ASSERT_VALID( this );
			TextSet( sText );
			return *this;
		}
		__EXT_MFC_SAFE_LPCTSTR TooltipTextGet() const;
		void TooltipTextSet(
			__EXT_MFC_SAFE_LPCTSTR sText = NULL
			);

		CSize IconGetSize() const;
		CExtCmdIcon * IconGetPtr() const;
		void IconSet(
			const CExtCmdIcon & _icon
			);
		void IconSet(
			HICON hIcon = NULL,
			bool bCopyIcon = true
			);
// REMOVED 2.53
// 		TAB_ITEM_INFO & operator = (
// 			HICON hIcon
// 			);

		DWORD GetItemStyle() const;
		DWORD ModifyItemStyle(
			DWORD dwRemove,
			DWORD dwAdd = 0
			);

		DWORD GetItemStyleEx() const;
		DWORD ModifyItemStyleEx(
			DWORD dwRemove,
			DWORD dwAdd = 0
			);

		CSize Measure(
			CDC * pDcMeasure = NULL
			);

		const CSize & GetLastMeasuredItemSize() const;
		const CSize & GetLastMeasuredTextSize() const;
		const CSize & GetLastMeasuredIconSize() const;
		const CRect & ItemRectGet() const;
		void ItemRectSet( const RECT & rcItem );
		const CRect & CloseButtonRectGet() const;
		void CloseButtonRectSet( const RECT & rcCloseButton );

		TAB_ITEM_INFO * GetPrev();
		const TAB_ITEM_INFO * GetPrev() const;
		TAB_ITEM_INFO * GetNext();
		const TAB_ITEM_INFO * GetNext() const;

		bool IsFirst() const;
		bool IsLast() const;

		bool VisibleGet() const;
		bool VisibleSet( bool bVisible = true );

		bool EnabledGet() const;
		bool EnabledSet( bool bEnable = true );
		
		bool SelectedGet() const;
		bool SelectedSet( bool bSelected = true );

		bool GroupStartGet() const;
		bool GroupStartSet( bool bGroupStart = true );

		bool InGroupActiveGet() const;
		bool InGroupActiveSet( bool bInGroupActive = true );

		const TAB_ITEM_INFO * GetInGroupFirst() const;
		TAB_ITEM_INFO * GetInGroupFirst();
		const TAB_ITEM_INFO * GetInGroupLast() const;
		TAB_ITEM_INFO * GetInGroupLast();
		LONG GetIndexOf() const;

		bool HitTest(
			const POINT & ptClient
			) const;

		LPARAM LParamGet() const;
		LPARAM LParamSet(
			LPARAM lParam = 0L
			);

		CObject * EventProviderGet() const;
		void EventProviderSet(
			CObject * pEventProvider = NULL
			);

		bool IsToolTipAvailByExtent() const;
		
		bool MultiRowWrapGet() const;
		void MultiRowWrapSet(
			bool bMultiRowWrap = true
			);

#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
#endif
	
		friend class CExtTabWnd;
	}; // class TAB_ITEM_INFO

	virtual void _OnTabItemHook_GetItemStyle(
		const CExtTabWnd::TAB_ITEM_INFO & _TII,
		DWORD & dwItemStyle
		) const;
	virtual void _OnTabItemHook_ModifyItemStyle(
		CExtTabWnd::TAB_ITEM_INFO & _TII,
		DWORD dwItemStyle,
		DWORD & dwRemove,
		DWORD & dwAdd
		);

	virtual void _OnTabItemHook_GetItemStyleEx(
		const CExtTabWnd::TAB_ITEM_INFO & _TII,
		DWORD & dwItemStyleEx
		) const;
	virtual void _OnTabItemHook_ModifyItemStyleEx(
		CExtTabWnd::TAB_ITEM_INFO & _TII,
		DWORD dwItemStyleEx,
		DWORD & dwRemove,
		DWORD & dwAdd
		);

// Attributes
public:
	static bool g_bEnableOnIdleCalls;
	bool m_bReflectQueryRepositionCalcEffect:1;

protected:
	HFONT m_hFont;
	DWORD m_dwTabWndStyle, m_dwTabWndStyleEx;
	LONG m_nSelIndex,
		m_nIndexVisFirst, m_nIndexVisLast,
		m_nDelayedSelIndex,
		m_nHoverTrackedIndex;
	LONG m_nVisibleItemCount;
	LONG m_nItemsExtent;
	LONG m_nScrollPos, m_nScrollMaxPos;
	LONG m_nScrollDirectionRest; 

	CRect
		m_rcTabItemsArea,
		m_rcTabNearBorderArea,
		m_rcBtnUp,
		m_rcBtnDown,
		m_rcBtnScrollHome,
		m_rcBtnScrollEnd,
		m_rcBtnHelp,
		m_rcBtnClose,
		m_rcBtnTabList;

	typedef
		CArray < TAB_ITEM_INFO *, TAB_ITEM_INFO *& >
		TabItemsArr_t;

	TabItemsArr_t m_arrItems;
	virtual void _RemoveAllItemsImpl();

	virtual bool _IsScrollAvail() const
	{
		return (m_nScrollMaxPos > 0) ? true : false;
	}
	virtual bool _IsScrolling() const
	{
		return (m_nScrollDirectionRest != 0) ? true : false;
	}
	virtual HFONT OnQueryFont() const;
	virtual void _GetTabWndFont(
		CFont * pFont,
		bool bSelected,
		DWORD dwOrientation = DWORD(-1) // default orientation
		) const;
public:
	CRect GetRectTabItemsArea() const
	{
		ASSERT_VALID( this );
		return m_rcTabItemsArea;
	}
	CRect GetRectTabNearBorderArea() const
	{
		ASSERT_VALID( this );
		return m_rcTabNearBorderArea;
	}
	CRect GetRectBtnUp() const
	{
		ASSERT_VALID( this );
		return m_rcBtnUp;
	}
	CRect GetRectBtnDown() const
	{
		ASSERT_VALID( this );
		return m_rcBtnDown;
	}
	CRect GetRectBtnScrollHome() const
	{
		ASSERT_VALID( this );
		return m_rcBtnScrollHome;
	}
	CRect GetRectBtnScrollEnd() const
	{
		ASSERT_VALID( this );
		return m_rcBtnScrollEnd;
	}
	CRect GetRectBtnHelp() const
	{
		ASSERT_VALID( this );
		return m_rcBtnHelp;
	}
	CRect GetRectBtnClose() const
	{
		ASSERT_VALID( this );
		return m_rcBtnClose;
	}
	CRect GetRectBtnTabList() const
	{
		ASSERT_VALID( this );
		return m_rcBtnTabList;
	}
	
	virtual DWORD GetTabWndStyle() const;
	virtual DWORD ModifyTabWndStyle(
		DWORD dwRemove,
		DWORD dwAdd = 0,
		bool bUpdateTabWnd = false
		);

	virtual DWORD GetTabWndStyleEx() const;
	virtual DWORD ModifyTabWndStyleEx(
		DWORD dwRemove,
		DWORD dwAdd = 0,
		bool bUpdateTabWnd = false
		);

	virtual DWORD OrientationGet() const;
	virtual DWORD OrientationSet(
		DWORD dwOrientation,
		bool bUpdateTabWnd = false
			);
	bool OrientationIsHorizontal() const;
	bool OrientationIsVertical() const;
	bool OrientationIsTopLeft() const;

	virtual HWND PmBridge_GetSafeHwnd() const;
	virtual void PmBridge_OnPaintManagerChanged(
		CExtPaintManager * pGlobalPM
		);

	LONG ItemGetCount() const;
	LONG ItemGetVisibleCount() const;
	
	TAB_ITEM_INFO * ItemGet( LONG nIndex );
	const TAB_ITEM_INFO * ItemGet( LONG nIndex ) const;

	bool ItemEnabledGet( LONG nIndex ) const;
	void ItemEnabledSet(
		LONG nIndex,
		bool bEnabled = true,
		bool bUpdateTabWnd = false
		);

	__EXT_MFC_SAFE_LPCTSTR ItemTextGet( LONG nIndex ) const;
	void ItemTextSet(
		LONG nIndex,
		__EXT_MFC_SAFE_LPCTSTR sText = NULL,
		bool bUpdateTabWnd = false
		);

	__EXT_MFC_SAFE_LPCTSTR ItemTooltipTextGet( LONG nIndex ) const;
	void ItemTooltipTextSet(
		LONG nIndex,
		__EXT_MFC_SAFE_LPCTSTR sTooltipText = NULL,
		bool bUpdateTabWnd = false
		);

	LPARAM ItemLParamGet( LONG nIndex ) const;
	void ItemLParamSet(
		LONG nIndex,
		LPARAM lParam = 0
		);
	
//	HICON ItemIconGet( LONG nIndex ) const; // REMOVED 2.53
	CExtCmdIcon & ItemIconGet( LONG nIndex ); // ADDED 2.53
	const CExtCmdIcon & ItemIconGet( LONG nIndex ) const; // ADDED 2.53
	void ItemIconSet( // ADDED 2.53
		LONG nIndex,
		const CExtCmdIcon & _icon,
		bool bUpdateTabWnd = false
		);
	void ItemIconSet(
		LONG nIndex,
		HICON hIcon = NULL,
		bool bCopyIcon = true,
		bool bUpdateTabWnd = false
		);

	DWORD ItemStyleGet( LONG nIndex );
	DWORD ItemStyleModify(
		LONG nIndex,
		DWORD dwRemove,
		DWORD dwAdd = 0,
		bool bUpdateTabWnd = false
		);

	LONG ItemGetIndexOf( const TAB_ITEM_INFO * pTii ) const;

	LONG SelectionGet() const;
	const TAB_ITEM_INFO * SelectionGetPtr() const;
	TAB_ITEM_INFO * SelectionGetPtr();
	LONG SelectionSet(
		LONG nSelIndex,
		bool bEnsureVisible = false,
		bool bUpdateTabWnd = false
		);
	LONG SelectionDelayedGet() const;
	bool SelectionDelay(
		LONG nSelIndex = -1, // -1 - to cancel
		DWORD dwMilliseconds = 0 // should be > 0 if nSelIndex >= 0
		);

	bool SelectionBoldGet() const;
	void SelectionBoldSet(
		bool bBold = true,
		bool bUpdateTabWnd = false
			);

	bool ItemMove( // move item into the new position
		LONG nIndex,
		LONG nIndexNew,
		bool bUpdateTabWnd = false
		);

	void ItemGetGroupRange(
		LONG nIndex,
		LONG & nGroupStart,
		LONG & nGroupEnd,
		TAB_ITEM_INFO ** ppTiiStart = NULL,
		TAB_ITEM_INFO ** ppTiiEnd = NULL
		);
	void ItemGetGroupRange(
		LONG nIndex,
		LONG & nGroupStart,
		LONG & nGroupEnd,
		const TAB_ITEM_INFO ** ppTiiStart = NULL,
		const TAB_ITEM_INFO ** ppTiiEnd = NULL
		) const;

	TAB_ITEM_INFO * ItemInsert(
		__EXT_MFC_SAFE_LPCTSTR sText = NULL,
		HICON hIcon = NULL,
		bool bCopyIcon = true,
		DWORD dwItemStyle = 0, // visible & non-group
		LONG nIndex = -1, // default - append
		LPARAM lParam = 0,
		bool bUpdateTabWnd = false
		);
	TAB_ITEM_INFO * ItemInsert( // ADDED 2.53
		__EXT_MFC_SAFE_LPCTSTR sText,
		const CExtCmdIcon & _icon,
		DWORD dwItemStyle = 0, // visible & non-group
		LONG nIndex = -1, // default - append
		LPARAM lParam = 0,
		bool bUpdateTabWnd = false
		);

	LONG ItemRemove( // returns count of removed items
		LONG nIndex,
		LONG nCountToRemove = 1,
		bool bUpdateTabWnd = false
		);
	void ItemRemoveGroup(
		LONG nIndex,
		LONG * p_nGroupStart = NULL,
		LONG * p_nCountInGroup = NULL,
		bool bUpdateTabWnd = false
		);
	void ItemRemoveAll(
		bool bUpdateTabWnd = false
		);

	LONG ItemHitTest(
		const POINT & ptClient
		) const;

	bool ItemEnsureVisible(
		INT nItemIndex,
		bool bUpdateTabWnd = false
		);

	LONG ItemFindByLParam(
		LPARAM lParam,
		LONG nIndexStartSearch = -1,
		bool bIncludeVisible = true,
		bool bIncludeInvisible = false
		) const;
	LONG ItemFindByStyle(
		DWORD dwItemStyleInclude,
		DWORD dwItemStyleExclude = __ETWI_INVISIBLE,
		LONG nIndexStartSearch = -1
		) const;
	virtual INT ItemFindByAccessChar(
		__EXT_MFC_SAFE_TCHAR chrAccess,
		INT nStartIdx = -1,
		BOOL bRestartAt0 = TRUE
		) const;
// Operations
public:

	bool m_bReflectParentSizing:1;
	static HCURSOR g_hCursor;
	mutable CExtCmdIcon m_iconTabItemCloseButton;

	virtual BOOL Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( __EXT_MFC_IDC_STATIC ),
		DWORD dwWindowStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		DWORD dwTabStyle = __ETWS_DEFAULT,
		CCreateContext * pContext = NULL
		);

	void UpdateTabWnd(
		bool bImmediate = true
		);

	static bool RegisterTabWndClass();

protected:
	
	static bool g_bTabWndClassRegistered;
	bool m_bDirectCreateCall:1;
	bool m_bDelayRecalcLayout:1;
	bool m_bTrackingButtonPushed:1;
	bool m_bTrackingButtonHover:1;
	bool m_bEnableTrackToolTips:1;
	bool m_bPushedUp:1;
	bool m_bPushedDown:1;
	bool m_bPushedScrollHome:1;
	bool m_bPushedScrollEnd:1;
	bool m_bPushedHelp:1;
	bool m_bPushedClose:1;
	bool m_bPushedTabList:1;
	LONG m_nPushedTrackingButton; // MK_... values or (-1L)
	LONG m_nPushedTrackingHitTest;
	bool m_bPushedTrackingCloseButton:1;
	LONG m_nHoverTrackingHitTest;
	CRect m_rcRecalcLayout;
	CPoint m_ptStartDrag;
	CRect m_rcLastMovedItemRect;
	bool m_bDragging:1;

	virtual void DoPaint( 
		CDC * pDC,
		CRect & rcClient
		);

	virtual bool _ItemMoveImpl( // move item into the new position
		LONG nIndex,
		LONG nIndexNew
		);

	virtual bool _CreateHelper();
	virtual bool _RecalcLayoutImpl();
	virtual void _RecalcLayout_GetClientRect(
		CRect & rcClient
		);

	virtual bool _ProcessMouseClick(
		CPoint point,
		bool bButtonPressed,
		INT nMouseButton, // MK_... values
		UINT nMouseEventFlags
		);

	virtual void _ProcessMouseMove(
		UINT nFlags, 
		CPoint point
		);

	static void _CbPaintCombinedTabListBtnContent(
		LPVOID pCookie,
		CDC & dc,
		const CWnd & refWndMenu,
		const CRect & rcExcludeArea, // in screen coords
		int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
		);

	virtual int OnTabWndQueryMultiRowColumnDistance() const;
	virtual bool OnTabWndQueryAlignFontBasedDifference() const;
	virtual bool OnTabWndQueryMultiRowColumnLayout() const;
	virtual void OnTabWndSyncVisibility();

	virtual bool OnTabWndMouseTrackingPushedStart(
		INT nMouseButton, // MK_... values
		LONG nHitTest
		);
	virtual void OnTabWndMouseTrackingPushedStop(
		bool bEnableReleaseCapture = true
		);
	virtual bool OnTabWndMouseTrackingHoverStart(
		LONG nHitTest
		);
	virtual void OnTabWndMouseTrackingHoverStop(
		bool bEnableReleaseCapture = true
		);
	virtual bool OnTabWndQueryHoverChangingRedraw() const;
	virtual bool OnTabWndStartDrag( 
		LONG nIndex 
		);
	virtual bool OnTabWndProcessDrag( 
		LONG nIndex 
		);
	virtual bool OnTabWndClickedButton(
		LONG nHitTest,
		bool bButtonPressed,
		INT nMouseButton, // MK_... values
		UINT nMouseEventFlags
		);
	virtual void OnTabWndClickedItemCloseButton(
		LONG nItemIndex
		);
	virtual bool OnTabWndClickedItem(
		LONG nItemIndex,
		bool bButtonPressed,
		INT nMouseButton, // MK_... values
		UINT nMouseEventFlags
		);

	virtual bool OnTabWndSelectionChange(
		LONG nOldItemIndex,
		LONG nNewItemIndex,
		bool bPreSelectionTest
		);

	virtual CExtCmdIcon * OnTabWndQueryItemIcon(
		const TAB_ITEM_INFO * pTii
		) const;
	virtual __EXT_MFC_SAFE_LPCTSTR OnTabWndQueryItemText(
		const TAB_ITEM_INFO * pTii
		) const;
	virtual __EXT_MFC_SAFE_LPCTSTR OnTabWndQueryItemTooltipText(
		const TAB_ITEM_INFO * pTii
		) const;

public:
	virtual CSize OnTabWndQueryItemCloseButtonSize(
		const TAB_ITEM_INFO * pTii
		) const;
	virtual CExtCmdIcon * OnTabWndQueryItemCloseButtonShape(
		const TAB_ITEM_INFO * pTii
		) const;
	virtual INT OnTabWndQueryItemCloseButtonPaintState(
		const TAB_ITEM_INFO * pTii
		) const;

protected:
	virtual void OnTabWndItemInsert(
		LONG nItemIndex,
		TAB_ITEM_INFO * pTii
		);
	virtual void OnTabWndRemoveItem(
		LONG nItemIndex,
		LONG nCount,
		bool bPreRemove
		);
	virtual void OnTabWndRemoveAllItems(
		bool bPreRemove
		);

	virtual void OnTabWndMeasureItemAreaMargins(
		LONG & nSpaceBefore,
		LONG & nSpaceAfter,
		LONG & nSpaceOver
		);
	virtual void _DrawItem(
		CDC & dc,
		LONG nItemIndex
		);
	virtual void OnTabWndDrawEntire(
		CDC & dc,
		CRect & rcClient
		);
	virtual void OnTabWndEraseClientArea(
		CDC & dc,
		CRect & rcClient,
		CRect & rcTabItemsArea,
		CRect & rcTabNearBorderArea,
		DWORD dwOrientation,
		bool bGroupedMode
		);
	virtual void OnTabWndUpdateItemMeasure(
		TAB_ITEM_INFO * pTii,
		CDC & dcMeasure,
		CSize & sizePreCalc
		);
	virtual void OnTabWndDrawItem(
		CDC & dc,
		CRect & rcTabItemsArea,
		LONG nItemIndex,
		TAB_ITEM_INFO * pTii,
		bool bTopLeft,
		bool bHorz,
		bool bSelected,
		bool bCenteredText,
		bool bGroupedMode,
		bool bInGroupActive,
		bool bInvertedVerticalMode,
		const CRect & rcEntireItem,
		CSize sizeTextMeasured,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon
		);
	
	virtual void OnTabWndDrawButton(
		CDC & dc,
		CRect & rcButton,
		LONG nHitTest,
		bool bTopLeft,
		bool bHorz,
		bool bEnabled,
		bool bHover,
		bool bPushed,
		bool bGroupedMode
		);

	virtual CSize OnTabWndCalcButtonSize(
		CDC & dcMeasure,
		LONG nTabAreaMetric // vertical max width or horizontal max heights of all tabs
		);

	virtual void OnTabWndDoScroll(
		LONG nStep,
		bool bSmoothScroll = true
		);

	virtual bool OnTabWndToolTipQueryEnabled() const;

	virtual bool OnTabWndItemPosChanging(
		LONG nItemIndex,
		LONG nItemNewIndex
		);
	virtual void OnTabWndItemPosChanged(
		LONG nItemIndex,
		LONG nItemNewIndex
		);

	virtual INT OnTabWndGetParentSizingMargin(
		DWORD dwOrientation
		) const;

	virtual INT OnTabWndButtonsCalcWidth(
		INT nButtonExtent,
		INT nBetweenButtonExtent
		);
	virtual void OnTabWndButtonsRecalcLayout(
		CRect &rcButtonsArea,
		INT nButtonExtent,
		INT nBetweenButtonExtent
		);

	virtual CExtPopupMenuTipWnd * OnAdvancedPopupMenuTipWndGet() const;
	virtual void OnAdvancedPopupMenuTipWndDisplay(
		CExtPopupMenuTipWnd & _ATTW,
		const RECT & rcExcludeArea,
		__EXT_MFC_SAFE_LPCTSTR strTipText
		) const;

public:

	virtual LONG GetHoverTrackingItem() const;
	virtual LONG GetPushedTrackingItem() const;

	virtual CRect CalcPreviewLayout( const CRect & rcAvail );
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTabWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtTabWnd();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtTabWnd)
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnCancelMode();
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnSetFont( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetFont( WPARAM, LPARAM );
	//}}AFX_MSG
	afx_msg LRESULT OnSizeParent( WPARAM wParam, LPARAM lParam );
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThemeChanged( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT _OnQueryRepositionCalcEffect( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

	friend class TAB_ITEM_INFO;
}; // class CExtTabWnd

__PROF_UIS_API void AFXAPI __EXT_MFC_DDX_TabIndex( CDataExchange * pDX, INT nIDC, LONG & index );

#if (!defined __EXT_MFC_NO_TABMDI_CTRL )

/////////////////////////////////////////////////////////////////////////////
// CExtTMWI

#if (!defined __EXT_CONTROLBAR_H)
	#include <ExtControlBar.h>
#endif

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

#define __EXTTAB_MDI_UPDATE_TIMER_ID			3
#define __EXTTAB_MDI_UPDATE_TIMER_PERIOD		10
//#define __EXTTAB_MDI_UPDATE_TIMER_ID_2			4
//#define __EXTTAB_MDI_UPDATE_TIMER_PERIOD_2		10
//#define __EXTTAB_MDI_UPDATE_TIMER_ID_3			5
//#define __EXTTAB_MDI_UPDATE_TIMER_PERIOD_3		10

#define __EXTTAB_MDI_NC_AREA_GAP_DX_L			2
#define __EXTTAB_MDI_NC_AREA_GAP_DX_R			2
#define __EXTTAB_MDI_NC_AREA_GAP_DY_T			2
#define __EXTTAB_MDI_NC_AREA_GAP_DY_B			2

// default set of styles for MDI tab window
#define __ETWS_MDI_DEFAULT \
	( __ETWS_DEFAULT \
	| __ETWS_CENTERED_TEXT \
	| __ETWS_SHOW_BTN_TAB_LIST \
	| __ETWS_SHOW_BTN_CLOSE \
	)

template < class _BT >
class CExtTMWI
	: public _BT
	, public CExtHookSink
{
	mutable CExtCmdIcon m_iconMdiChildCache;
protected:
	bool m_bInSyncLayout:1;
	INT m_nSyncCounter, m_nSyncWaitCount;
public:
	CRect m_rcReservedOuterSpace, m_rcReservedBorderSpace;
	CExtTMWI()
		: m_bInSyncLayout( false )
		, m_rcReservedOuterSpace( 0, 0, 0, 0 )
		, m_rcReservedBorderSpace( 0, 0, 0, 0 )
		, m_nSyncCounter( 0 )
		, m_nSyncWaitCount( 2 )
	{
		_BT::m_bReflectQueryRepositionCalcEffect = true;
	}
	virtual ~CExtTMWI()
	{
	}
	virtual bool _IsMdiTabCtrl() const
	{
		return true;
	}
protected:
	virtual HWND _GetHwndMainFrame()
	{
		ASSERT_VALID( this );
		CWnd * pParentWnd = GetParent();
		ASSERT( pParentWnd != NULL );
		HWND hWndParent = pParentWnd->GetSafeHwnd();
		ASSERT( hWndParent != NULL );
		ASSERT( ::IsWindow(hWndParent) );
		// FrameFeatures does not use CMDIFrameWnd
		// ASSERT_KINDOF( CMDIFrameWnd, pParentWnd );
		return hWndParent;
	}
	virtual HWND _GetHwndMdiArea()
	{
		ASSERT_VALID( this );
		HWND hWndMdiFrame = _GetHwndMainFrame();
		ASSERT( hWndMdiFrame != NULL );
		if( hWndMdiFrame == NULL )
			return NULL;
		CFrameWnd * pFrame =
			STATIC_DOWNCAST(
				CFrameWnd,
				CWnd::FromHandlePermanent( hWndMdiFrame )
				);
		ASSERT_VALID( pFrame );
		ASSERT( ::IsWindow(pFrame->GetSafeHwnd()) );
		CMDIFrameWnd * pMdiFrame =
			DYNAMIC_DOWNCAST(
				CMDIFrameWnd,
				pFrame
				);
		if( pMdiFrame == NULL )
		{
			// FrameFeatures does not use CMDIFrameWnd
			for(	HWND hWnd = ::GetWindow( pFrame->GetSafeHwnd(), GW_CHILD );
					hWnd != NULL;
					hWnd = ::GetWindow( hWnd, GW_HWNDNEXT )
				)
			{
				ASSERT( ::IsWindow( hWnd ) );
				TCHAR strClassName[ 512 ];
				::memset( strClassName, 0, sizeof(strClassName) );
				::GetClassName( hWnd, strClassName, 512 );
				if( _tcslen( strClassName ) == 0 )
					continue;
				__EXT_MFC_STRLWR( strClassName, 512 );
				//if( _tcscmp( strClassName, _T("mdiclient") ) == 0 )
				if( _tcsstr( strClassName, _T("mdiclient") ) != NULL )
					return hWnd;
			}
			return NULL;
		} // if( pMdiFrame == NULL )
		//ASSERT_VALID( pMdiFrame );
		ASSERT( pMdiFrame->m_hWndMDIClient != NULL );
		ASSERT( ::IsWindow( pMdiFrame->m_hWndMDIClient ) );
		return pMdiFrame->m_hWndMDIClient;
	}
	virtual bool _SyncAllItems()
	{
		if( m_nSyncCounter < m_nSyncWaitCount )
		{
			m_nSyncCounter++;
			return false;
		}
		if( m_bInSyncLayout || m_bDragging )
			return true;
		m_bInSyncLayout = true;
		// FrameFeatures does not use CMDIFrameWnd
		//CMDIFrameWnd * pMdiFrame =
		//		STATIC_DOWNCAST(
		//			CMDIFrameWnd,
		//			CWnd::FromHandlePermanent( _GetHwndMainFrame() )
		//			);
		//CWnd * pWndActiveMdiChild = pMdiFrame->MDIGetActive(NULL);
		CWnd * pWndActiveMdiChild = NULL;
		HWND hWndMainFrame = _GetHwndMainFrame();
		HWND hWndMdiArea = _GetHwndMdiArea();
		if(		hWndMainFrame == NULL
			||	hWndMdiArea == NULL
			)
		{
			m_bInSyncLayout = false;
			return true;
		}
		CMDIFrameWnd * pMdiFrame =
			DYNAMIC_DOWNCAST(
				CMDIFrameWnd,
				CWnd::FromHandlePermanent(hWndMainFrame)
				);
		if( pMdiFrame != NULL )
		{
			pWndActiveMdiChild = pMdiFrame->MDIGetActive(NULL);
		} // if( pMdiFrame != NULL )
		else
		{
			HWND hWnd = (HWND)
				::SendMessage(
					hWndMdiArea,
					WM_MDIGETACTIVE,
					0,
					0
					);
			if( hWnd == NULL )
			{
				m_bInSyncLayout = false;
				return true;
			}
			pWndActiveMdiChild = CWnd::FromHandle( hWnd );
		} // else from if( pMdiFrame != NULL )
		if( pWndActiveMdiChild == NULL )
		{
			m_bInSyncLayout = false;
			return true;
		}
		bool bCloseEnabled = false;
		HWND hWndMdiChild = NULL;
		if( pWndActiveMdiChild != NULL )
		{
			hWndMdiChild =
				pWndActiveMdiChild->
					GetWindow( GW_HWNDFIRST )->
						GetSafeHwnd();
			CMenu * pSysMenu = pWndActiveMdiChild->GetSystemMenu( FALSE );
			if( pSysMenu != NULL )
			{
				MENUITEMINFO mii;
				::memset(&mii,0,sizeof(MENUITEMINFO));
				mii.cbSize = sizeof(MENUITEMINFO);
				mii.fMask = MIIM_STATE;
				if( pSysMenu->GetMenuItemInfo(
						SC_CLOSE,
						&mii,
						FALSE
						)
					)
				{
					if( ( mii.fState & (MFS_DISABLED|MFS_GRAYED) ) == 0 )
						bCloseEnabled = true;
				}
			}
		}
		ModifyTabWndStyle(
			bCloseEnabled ? 0 : __ETWS_ENABLED_BTN_CLOSE,
			bCloseEnabled ? __ETWS_ENABLED_BTN_CLOSE : 0,
			false
			);
		INT nVisibleCount = 0;
		for( ; hWndMdiChild != NULL; hWndMdiChild = ::GetWindow( hWndMdiChild, GW_HWNDNEXT ) )
		{
			if( ( ::__EXT_MFC_GetWindowLong( hWndMdiChild, GWL_STYLE ) & WS_VISIBLE ) != 0 )
				nVisibleCount ++;
			LONG nIndexExist = ItemFindByHWND( hWndMdiChild, -1, true, true );
			if( nIndexExist >= 0 )
			{
				if( hWndMdiChild == pWndActiveMdiChild->GetSafeHwnd() )
					SelectionSet( nIndexExist, true, false );
				continue;
			}
			ItemInsert(
				NULL,
				NULL, false,
				0,
				-1,
				LPARAM( hWndMdiChild ),
				false
				);
			if( hWndMdiChild == pWndActiveMdiChild->GetSafeHwnd() )
				SelectionSet( ItemGetCount()-1, true, false );
		} // for( ; hWndMdiChild != NULL; hWndMdiChild = ::GetWindow( hWndMdiChild, GW_HWNDNEXT ) )
		m_nVisibleItemCount = nVisibleCount;

		for( INT nIndex = 0; nIndex < ItemGetCount(); )
		{
			TAB_ITEM_INFO * pTii = ItemGet( nIndex );
			ASSERT_VALID( pTii );
			HWND hWnd = (HWND)pTii->LParamGet();
			ASSERT( hWnd );
			if( ::IsWindow( hWnd ) )
			{
				if(		hWnd == pWndActiveMdiChild->m_hWnd 
					&&	(!bCloseEnabled)
					)
					pTii->ModifyItemStyleEx( 0, __ETWI_EX_NO_CLOSE_ON_TAB );

				bool bVisible = ::IsWindowVisible( hWnd ) ? true : false;
				pTii->VisibleSet( bVisible );
				nIndex++;
			}
			else
				ItemRemove( nIndex );
		}
		m_bInSyncLayout = false;
		bool bLayoutUpdated = _RecalcFrameLayout();
		return bLayoutUpdated;
	}

	virtual bool _RecalcFrameLayout()
	{
		bool bWasVisible = (GetStyle() & WS_VISIBLE) ? true : false;
		OnTabWndSyncVisibility();
		bool bNowVisible = (GetStyle() & WS_VISIBLE) ? true : false;
		if( bWasVisible != bNowVisible )
		{
			// FrameFeatures does not use CMDIFrameWnd
			CFrameWnd * pFrame =
				STATIC_DOWNCAST(
					CFrameWnd,
					CWnd::FromHandlePermanent( _GetHwndMainFrame() )
					);
			pFrame->DelayRecalcLayout();
		} // if( bWasVisible != bNowVisible )
		_BT::UpdateTabWnd( false );
		bool bLayoutUpdated = _RecalcLayoutImpl();
		if( bLayoutUpdated )
		{
			Invalidate();
			UpdateWindow();
		}
		return bLayoutUpdated;
	}
public:

	virtual BOOL Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( __EXT_MFC_IDC_STATIC ),
		DWORD dwWindowStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		DWORD dwTabStyle = __ETWS_MDI_DEFAULT,
		CCreateContext * pContext = NULL
		)
	{
		ASSERT( pParentWnd != NULL );
		ASSERT( pParentWnd->GetSafeHwnd() != NULL );
		ASSERT( ::IsWindow(pParentWnd->GetSafeHwnd()) );
		m_nSyncCounter = 0;
		// FrameFeatures does not use CMDIFrameWnd
		// ASSERT_KINDOF( CMDIFrameWnd, pParentWnd );
		if(	! _BT::Create(
				pParentWnd,
				rcWnd,
				nDlgCtrlID,
				dwWindowStyle,
				dwTabStyle,
				pContext
				)
			)
		{
			ASSERT( FALSE );
			return FALSE;
		}
		// FrameFeatures does not use CMDIFrameWnd
		HWND hWndMdiArea = _GetHwndMdiArea();
		//ASSERT( hWndMdiArea != NULL );
		if( hWndMdiArea == NULL )
		{
			//return FALSE;
			return TRUE;
		} // if( hWndMdiArea == NULL )
		if( IsHookedWindow( hWndMdiArea ) )
			return TRUE;
		if( ! SetupHookWndSink( hWndMdiArea, false, true ) )
		{
			ASSERT( FALSE );
			return FALSE;
		}
		// setup MDI update timer
		SetTimer(
			__EXTTAB_MDI_UPDATE_TIMER_ID,
			__EXTTAB_MDI_UPDATE_TIMER_PERIOD,
			NULL
			);
		return TRUE;
	}
	virtual LONG ItemFindByHWND(
		HWND hWnd,
		LONG nIndexStartSearch = -1,
		bool bIncludeVisible = true,
		bool bIncludeInvisible = false
		) const
	{
		return
			_BT::ItemFindByLParam(
				(LPARAM)hWnd,
				nIndexStartSearch,
				bIncludeVisible,
				bIncludeInvisible
				);
	}
	void UpdateTabWnd(
		bool bImmediate = true
		)
	{
		_SyncAllItems();
		_BT::UpdateTabWnd( bImmediate );
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
		__EXT_MFC_LONG_PTR dwWndStyle = ::__EXT_MFC_GetWindowLong( hWnd, GWL_STYLE );
		if( (dwWndStyle&WS_VISIBLE) != 0 )
			dwItemStyle &= ~__ETWI_INVISIBLE;
		else
			dwItemStyle |= __ETWI_INVISIBLE;
		if( (dwWndStyle&WS_DISABLED) != 0 )
			dwItemStyle |= __ETWI_DISABLED;
		else
			dwItemStyle &= ~__ETWI_DISABLED;
	}
protected:
	virtual void OnMdiTabImplAdjustBorderSpaces()
	{
		ASSERT_VALID( this );
	}
	virtual void OnMdiTabImplDrawOuterBorder(
		CDC & dc,
		const CRect & rcOuterBorder,
		const CRect & rcMdiAreaClient,
		const CRect & rcMdiAreaWnd
		)
	{
		ASSERT_VALID( this );
		ASSERT( dc.GetSafeHdc() != NULL );
		dc;
		rcOuterBorder;
		rcMdiAreaClient;
		rcMdiAreaWnd;
	}
	virtual bool OnHookWndMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		UINT nMessage,
		WPARAM & wParam,
		LPARAM & lParam
		)
	{
		__PROF_UIS_MANAGE_STATE;
		if( CWnd::FromHandlePermanent(GetSafeHwnd()) == NULL )
			return
				CExtHookSink::OnHookWndMsg(
					lResult,
					hWndHooked,
					nMessage,
					wParam,
					lParam
					);
		if( hWndHooked == _GetHwndMdiArea() )
		{
			bool bDelayUpdateMdiTabCtrl = false;
			switch( nMessage )
			{
			case WM_WINDOWPOSCHANGING:
			{
				LPWINDOWPOS lpWindowPos = 
					reinterpret_cast < LPWINDOWPOS > (lParam);
				ASSERT( lpWindowPos != NULL );
				lpWindowPos->flags |= SWP_FRAMECHANGED;
				break;
			}
			break;
			case WM_NCCALCSIZE:
			{
				OnMdiTabImplAdjustBorderSpaces();
				lResult = 0;
				NCCALCSIZE_PARAMS * pNCCSP =
					reinterpret_cast < NCCALCSIZE_PARAMS * > ( lParam );
				ASSERT( pNCCSP != NULL );
				DWORD dwWndStyle = GetStyle();
				bool bVisibleTab =
					( (dwWndStyle & WS_VISIBLE) != 0 ) ? true : false;
				CRect rcMdiTabWnd( pNCCSP->rgrc[0] );
				DWORD dwOrientation = OrientationGet();
				rcMdiTabWnd.DeflateRect(
					(bVisibleTab && dwOrientation == __ETWS_ORIENT_LEFT) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DX_L,
					(bVisibleTab && dwOrientation == __ETWS_ORIENT_TOP) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DY_T,
					(bVisibleTab && dwOrientation == __ETWS_ORIENT_RIGHT) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DX_R,
					(bVisibleTab && dwOrientation == __ETWS_ORIENT_BOTTOM) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DY_B
					);
				rcMdiTabWnd.DeflateRect(
					m_rcReservedOuterSpace.left + m_rcReservedBorderSpace.left,
					m_rcReservedOuterSpace.top + m_rcReservedBorderSpace.top,
					m_rcReservedOuterSpace.right + m_rcReservedBorderSpace.right,
					m_rcReservedOuterSpace.bottom + m_rcReservedBorderSpace.bottom
					);
				::CopyRect( &(pNCCSP->rgrc[0]), rcMdiTabWnd );
				return true;
			}
			break;
			case WM_NCPAINT:
			{
				lResult = 0;
				CWnd * pWndMdiArea = CWnd::FromHandle( _GetHwndMdiArea() );
				CRect rcMdiAreaWnd, rcMdiAreaClient;
				pWndMdiArea->GetWindowRect( &rcMdiAreaWnd );
				pWndMdiArea->GetClientRect( &rcMdiAreaClient );
				pWndMdiArea->ClientToScreen( &rcMdiAreaClient );
				if( rcMdiAreaWnd == rcMdiAreaClient )
					return true;
				CPoint ptDevOffset = -rcMdiAreaWnd.TopLeft();
				rcMdiAreaWnd.OffsetRect( ptDevOffset );
				rcMdiAreaClient.OffsetRect( ptDevOffset );
				rcMdiAreaWnd.DeflateRect(
					m_rcReservedOuterSpace.left,
					m_rcReservedOuterSpace.top,
					m_rcReservedOuterSpace.right,
					m_rcReservedOuterSpace.bottom
					);
				CWindowDC dcWindow( pWndMdiArea );
				ASSERT( dcWindow.GetSafeHdc() != NULL );
				CRect rcOuterBorder = rcMdiAreaWnd;
				rcMdiAreaWnd.DeflateRect(
					m_rcReservedBorderSpace.left,
					m_rcReservedBorderSpace.top,
					m_rcReservedBorderSpace.right,
					m_rcReservedBorderSpace.bottom
					);
				dcWindow.ExcludeClipRect( &rcMdiAreaClient );
				CExtMemoryDC dc( &dcWindow );
				dc.FillSolidRect(
					&rcOuterBorder,
					PmBridge_GetPM()->GetColor( COLOR_3DFACE, this )
					);
				if( rcOuterBorder != rcMdiAreaClient )
					OnMdiTabImplDrawOuterBorder(
						dc,
						rcOuterBorder,
						rcMdiAreaClient,
						rcMdiAreaWnd
						);
				DWORD dwWndStyle = GetStyle();
				bool bInvisibleTab =
					( (dwWndStyle & WS_VISIBLE) == 0 ) ? true : false;
				DWORD dwOrientation = OrientationGet();
				rcOuterBorder.InflateRect(
					(bInvisibleTab || dwOrientation != __ETWS_ORIENT_LEFT) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DX_L,
					(bInvisibleTab || dwOrientation != __ETWS_ORIENT_TOP) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DY_T,
					(bInvisibleTab || dwOrientation != __ETWS_ORIENT_RIGHT) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DX_R,
					(bInvisibleTab || dwOrientation != __ETWS_ORIENT_BOTTOM) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DY_B
					);
				PmBridge_GetPM()->PaintTabNcAreaRect(
					dc,
					rcOuterBorder,
					this
					);
				return true;
			}
			case WM_MDIDESTROY:
			{
				HWND hWndMdiChild = (HWND)wParam;
				if( hWndMdiChild != NULL )
				{
					LONG nIndex = ItemFindByHWND( hWndMdiChild, -1, true, true );
					if( nIndex >= 0 )
					{
						m_bInSyncLayout = true;
						ItemRemove( nIndex, 1, true );
						m_bInSyncLayout = false;
					}
				}
				bDelayUpdateMdiTabCtrl = true;
			} // if( nMessage == WM_MDIDESTROY )
			break;
			case WM_MDIACTIVATE:
			{
				HWND hWndMdiChild = (HWND)wParam;
				if( hWndMdiChild != NULL )
				{
					LONG nIndex = ItemFindByHWND( hWndMdiChild, -1, true, true );
					if( nIndex >= 0 )
					{
						m_bInSyncLayout = true;
						SelectionSet( nIndex, true, true );
						m_bInSyncLayout = false;
					}
				}
				bDelayUpdateMdiTabCtrl = true;
			} // else if( nMessage == WM_MDIACTIVATE )
			break;
			case WM_MDIREFRESHMENU:
			case WM_MDICREATE:
				bDelayUpdateMdiTabCtrl = true;
			break;
			} // switch( nMessage )
			if( bDelayUpdateMdiTabCtrl )
			{
				// setup MDI update timer
				PostMessage(
					WM_TIMER,
					__EXTTAB_MDI_UPDATE_TIMER_ID,
					0
					);
			}
		} // if( hWndHooked == _GetHwndMdiArea() )
		return
			CExtHookSink::OnHookWndMsg(
				lResult,
				hWndHooked,
				nMessage,
				wParam,
				lParam
				);
	}
	virtual void OnTabWndClickedItemCloseButton(
		LONG nItemIndex
		)
	{
		nItemIndex;
		OnTabWndClickedButton(
			__ETWH_BUTTON_CLOSE,
			false,
			MK_LBUTTON,
			0
			);
	}
	virtual bool OnTabWndClickedButton(
		LONG nHitTest,
		bool bButtonPressed,
		INT nMouseButton, // MK_... values
		UINT nMouseEventFlags
		)
	{
		if( nMouseButton != MK_LBUTTON )
			return false;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		CExtCustomizeSite * pSite =
			CExtCustomizeSite::GetCustomizeSite( m_hWnd );
		if(		pSite != NULL
			&&	pSite->IsCustomizeMode()
			)
			return false;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
		bool bRetVal =
			_BT::OnTabWndClickedButton(
				nHitTest,
				bButtonPressed,
				nMouseButton, // MK_... values
				nMouseEventFlags
				);
		DWORD dwStyle = GetTabWndStyle();
		if(		(!bButtonPressed)
			&&	(	( nHitTest == __ETWH_BUTTON_CLOSE && (dwStyle&__ETWS_ENABLED_BTN_CLOSE) != 0 )
				||	( nHitTest == __ETWH_BUTTON_HELP && (dwStyle&__ETWS_ENABLED_BTN_HELP) != 0 )
				)
			&&	SelectionGet() >= 0
			)
		{ // if "X" or "?" pressed
			TAB_ITEM_INFO * pTii = SelectionGetPtr();
			ASSERT_VALID( pTii );
			HWND hWndMdiChild = (HWND)pTii->LParamGet();	
			if( hWndMdiChild != NULL && ::IsWindow(hWndMdiChild) )
			{
				PostMessage( WM_CANCELMODE );
				if( nHitTest == __ETWH_BUTTON_CLOSE )
				{ // if "X" pressed
					POINT point = { 0, 0 };
					::GetCursorPos( &point );
					::PostMessage(
						hWndMdiChild,
						WM_SYSCOMMAND,
						SC_CLOSE,
						MAKELONG( point.x, point.y )
						);
// 					PostMessage(
// 						WM_TIMER,
// 						__EXTTAB_MDI_UPDATE_TIMER_ID_2,
// 						0
// 						);
				} // if "X" pressed
				else
				{ // if "?" pressed
					CPoint ptCursor( 0, 0 );
					::GetCursorPos( &ptCursor );
					VERIFY( ::ClientToScreen( hWndMdiChild, &ptCursor ) );
					::PostMessage(
						hWndMdiChild,
						WM_SYSCOMMAND,
						SC_CONTEXTHELP,
						MAKELONG( ptCursor.x, ptCursor.y )
						);
				} // if "?" pressed
			} // if( hWndMdiChild != NULL && ::IsWindow(hWndMdiChild) )
		} // if "X" or "?" pressed
		return bRetVal;
	}
	virtual bool OnTabWndClickedItem(
		LONG nItemIndex,
		bool bButtonPressed,
		INT nMouseButton, // MK_... values
		UINT nMouseEventFlags
		)
	{
		if( nMouseButton != MK_LBUTTON )
			return false;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		CExtCustomizeSite * pSite =
			CExtCustomizeSite::GetCustomizeSite( m_hWnd );
		if(		pSite != NULL
			&&	pSite->IsCustomizeMode()
			)
			return false;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
		return
			_BT::OnTabWndClickedItem(
				nItemIndex,
				bButtonPressed,
				nMouseButton, // MK_... values
				nMouseEventFlags
				);
	}
	virtual bool OnTabWndSelectionChange(
		LONG nOldItemIndex,
		LONG nNewItemIndex,
		bool bPreSelectionTest
		)
	{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		CExtCustomizeSite * pSite =
			CExtCustomizeSite::GetCustomizeSite( m_hWnd );
		if(		pSite != NULL
			&&	pSite->IsCustomizeMode()
			)
			return false;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
		bool bRetVal =
			_BT::OnTabWndSelectionChange( nOldItemIndex, nNewItemIndex, bPreSelectionTest );
		if( (!m_bInSyncLayout) && (!bPreSelectionTest) && (!m_bDragging))
		{
			ASSERT( bRetVal );
			HWND hWnd = (HWND)ItemLParamGet( nNewItemIndex );
			if( hWnd != NULL && ::IsWindow(hWnd) )
			{
				// FrameFeatures does not use CMDIFrameWnd
				HWND hWndMdiArea = _GetHwndMdiArea();
				if( hWndMdiArea != NULL )
				{
					HWND hWndAlreadyActive = (HWND)
						::SendMessage( hWndMdiArea, WM_MDIGETACTIVE, 0, 0 );
					if( hWndAlreadyActive != hWnd )
						::SendMessage( hWndMdiArea, WM_MDIACTIVATE, (WPARAM)hWnd, 0 );
					::SetWindowPos(
						hWndMdiArea, NULL, 0, 0, 0, 0,
						SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER
							|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_FRAMECHANGED
						);
				} // if( hWndMdiArea != NULL )
			} // if( hWnd != NULL && ::IsWindow(hWnd) )
		} // if( (!m_bInSyncLayout) && (!bPreSelectionTest) )
		else
		{
			HWND hWndMdiArea = _GetHwndMdiArea();
			if( hWndMdiArea != NULL )
				::SetWindowPos(
					hWndMdiArea, NULL, 0, 0, 0, 0,
					SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER
						|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_FRAMECHANGED
					);
		} // else from if( (!m_bInSyncLayout) && (!bPreSelectionTest) )
		return bRetVal;
	}
	virtual bool OnTabWndMouseTrackingPushedStart(
		INT nMouseButton, // MK_... values
		LONG nHitTest
		)
	{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		CExtCustomizeSite * pSite =
			CExtCustomizeSite::GetCustomizeSite( m_hWnd );
		if(		pSite != NULL
			&&	pSite->IsCustomizeMode()
			)
			return false;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
		return _BT::OnTabWndMouseTrackingPushedStart( nMouseButton, nHitTest );
	}
	virtual bool OnTabWndMouseTrackingHoverStart(
		LONG nHitTest
		)
	{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		CExtCustomizeSite * pSite =
			CExtCustomizeSite::GetCustomizeSite( m_hWnd );
		if(		pSite != NULL
			&&	pSite->IsCustomizeMode()
			)
			return false;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
		return _BT::OnTabWndMouseTrackingHoverStart( nHitTest );
	}
	virtual CExtCmdIcon * OnTabWndQueryItemIcon(
		const CExtTabWnd::TAB_ITEM_INFO * pTii
		) const
	{
		ASSERT_VALID( pTii );
		HWND hWndMdiChild = (HWND)pTii->LParamGet();	
		if(		hWndMdiChild == NULL
			||	(! ::IsWindow( hWndMdiChild ) )
			)
			return NULL;
		HICON hIcon = (HICON)
			::SendMessage(
				hWndMdiChild,
				WM_GETICON,
				WPARAM(ICON_SMALL),
				0
				);
		if( hIcon == NULL )
		{
			hIcon = (HICON)
				::SendMessage(
					hWndMdiChild,
					WM_GETICON,
					WPARAM(ICON_BIG),
					0
					);
			if( hIcon == NULL )
			{
#if _MFC_VER >= 0x700
	// disable warning C4312: type cast' : conversion from 'DWORD' to 'HICON' of greater size
	#pragma warning( push )
	#pragma warning( disable : 4312 )
#endif  // _MFC_VER >= 0x700
				hIcon = (HICON)
						::__EXT_MFC_GetClassLong(
							hWndMdiChild,
							__EXT_MFC_GCL_HICONSM
							);
				if( hIcon == NULL )
				{
					hIcon = (HICON)
							::__EXT_MFC_GetClassLong(
								hWndMdiChild,
								__EXT_MFC_GCL_HICON
								);
					if( hIcon == NULL )
						return NULL;
				} // if( hIcon == NULL )
#if _MFC_VER >= 0x700
	// rollback warning C4312: type cast' : conversion from 'DWORD' to 'HICON' of greater size
	#pragma warning( pop )
#endif  // _MFC_VER >= 0x700
			} // if( hIcon == NULL )
		} // if( hIcon == NULL )
		ASSERT( hIcon != NULL );
		m_iconMdiChildCache.Empty();
		CExtCmdIcon _iconBig;
		_iconBig.AssignFromHICON( hIcon, true );
		VERIFY(
			m_iconMdiChildCache.
				CreateScaledCopy(
					_iconBig,
					CSize( 16, 16 )
					)
			);
		if( ! m_iconMdiChildCache.IsEmpty() )
			return (&m_iconMdiChildCache);
		return NULL;
	}
	virtual __EXT_MFC_SAFE_LPCTSTR OnTabWndQueryItemText(
		const CExtTabWnd::TAB_ITEM_INFO * pTii
		) const
	{
		ASSERT_VALID( pTii );
		HWND hWndMdiChild = (HWND)pTii->LParamGet();	
		if( hWndMdiChild != NULL
			&& ::IsWindow(hWndMdiChild)
			)
		{
			CWnd * pWnd = CWnd::FromHandle( hWndMdiChild );
			ASSERT( pWnd != NULL );
			static CString sText;
			sText = _T("");
			if( pWnd->IsKindOf( RUNTIME_CLASS(CFrameWnd) ) )
			{
				CDocument * pActiveDoc =
					((CFrameWnd*)pWnd)->GetActiveDocument();
				if( pActiveDoc != NULL )
					sText = pActiveDoc->GetTitle();
				if( sText.IsEmpty() )
					sText = ((CFrameWnd *)pWnd)->GetTitle();
				if( ! sText.IsEmpty() )
				{
					int nWindow = ((CFrameWnd*)pWnd)->m_nWindow;
					if( nWindow > 0 )
					{
						sText += _T(":");
						CExtSafeString sNo;
						sNo.Format( _T("%d"), nWindow );
						sText += sNo;
					} // if( nWindow > 0 )
				} // if( ! sText.IsEmpty() )
			} // if( pWnd->IsKindOf( RUNTIME_CLASS(CFrameWnd) ) )
			else
				sText = _T("");

			if( sText.IsEmpty() )
			{
				int nTextLen = pWnd->GetWindowTextLength();
				if( nTextLen > 0 )
				{
					pWnd->GetWindowText( sText.GetBuffer( nTextLen+2 ), nTextLen+1 );
					sText.ReleaseBuffer();
				} // if( nTextLen > 0 )
			} // if( sText.IsEmpty() )
			return (sText.IsEmpty()) ? _T("") : ((LPCTSTR)sText);
			
		}
		return NULL;
	}
	virtual void OnTabWndItemInsert(
		LONG nItemIndex,
		CExtTabWnd::TAB_ITEM_INFO * pTii
		)
	{
		_BT::OnTabWndItemInsert(
			nItemIndex,
			pTii
			);
		if( m_bInSyncLayout || m_bDragging )
			return;
		bool bDoRecalcLayout =
			(ItemGetCount() == 1)
				? true
				: false
				;
		if( bDoRecalcLayout )
			_RecalcFrameLayout();
	}
	virtual void OnTabWndRemoveItem(
		LONG nItemIndex,
		LONG nCount,
		bool bPreRemove
		)
	{
		_BT::OnTabWndRemoveItem(
			nItemIndex,
			nCount,
			bPreRemove
			);
		if( !bPreRemove || m_bInSyncLayout || m_bDragging )
			return;
		bool bDoRecalcLayout =
			(ItemGetCount() == 0)
				? true
				: false
				;
		if( bDoRecalcLayout )
			_RecalcFrameLayout();
	}
	virtual void OnTabWndRemoveAllItems(
		bool bPreRemove
		)
	{
		_BT::OnTabWndRemoveAllItems(
			bPreRemove
			);
		if( (!bPreRemove) || m_bInSyncLayout || m_bDragging)
			return;
		_RecalcFrameLayout();
	}
	virtual void OnTabWndSyncVisibility()
	{
		LONG nItemCount = ItemGetCount();
		LONG nItemVisibleCount = ItemGetVisibleCount();
		DWORD dwWndStyle = GetStyle();
		if(		nItemCount > 0 
			&&	nItemVisibleCount > 0
			&&	(!(		CExtControlBar::FindPrintPreviewMode(
							STATIC_DOWNCAST( CFrameWnd, GetParent() )
							)
					||	CExtControlBar::IsOleIpObjActive(
							STATIC_DOWNCAST( CFrameWnd, GetParent() )
							)
				) )
			)
		{
			if( (dwWndStyle & WS_VISIBLE) == 0 )
			{
				::SetWindowPos(
					m_hWnd,
					NULL, 0, 0, 0, 0,
					SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOOWNERZORDER
						|SWP_FRAMECHANGED
						|SWP_SHOWWINDOW
					);
				HWND hWndMdiArea = _GetHwndMdiArea();
				if( hWndMdiArea != NULL )
					::SetWindowPos(
						hWndMdiArea,
						NULL, 0, 0, 0, 0,
						SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOOWNERZORDER
							|SWP_FRAMECHANGED
						);
			}
		} // if( nItemCount > 0 ...
		else
		{
//			SetTimer(
//				WM_TIMER,
//				__EXTTAB_MDI_UPDATE_TIMER_ID_3,
//				__EXTTAB_MDI_UPDATE_TIMER_PERIOD_3,
//				NULL
//				);
			if( (dwWndStyle & WS_VISIBLE) != 0 )
				::SetWindowPos(
					m_hWnd,
					NULL, 0, 0, 0, 0,
					SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOOWNERZORDER
						|SWP_NOSENDCHANGING
						|SWP_HIDEWINDOW
					);
		} // else from if( nItemCount > 0 ...
		dwWndStyle = GetStyle();
		if( (dwWndStyle & WS_VISIBLE) == 0 )
			m_nIndexVisFirst = m_nIndexVisLast = -1;
	}
	virtual void PostNcDestroy()
	{
		m_nSyncCounter = 0;
		_BT::PostNcDestroy();
	}
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
	{
		switch( message )
		{
		case WM_WINDOWPOSCHANGING:
		{
			LPWINDOWPOS lpWindowPos = 
				reinterpret_cast < LPWINDOWPOS > (lParam);
			ASSERT( lpWindowPos != NULL );
			lpWindowPos->flags |= SWP_FRAMECHANGED;
			break;
		}
		break;
		case WM_NCCALCSIZE:
		{
			NCCALCSIZE_PARAMS * pNCCSP =
				reinterpret_cast < NCCALCSIZE_PARAMS * > ( lParam );
			ASSERT( pNCCSP != NULL );
			
			DWORD dwWndStyle = GetStyle();
			if( (dwWndStyle & WS_VISIBLE) != 0 )
			{
				CRect rcMdiTabWnd( pNCCSP->rgrc[0] );
				DWORD dwOrientation = OrientationGet();
				rcMdiTabWnd.DeflateRect(
					(dwOrientation == __ETWS_ORIENT_RIGHT) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DX_L,
					(dwOrientation == __ETWS_ORIENT_BOTTOM) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DY_T,
					(dwOrientation == __ETWS_ORIENT_LEFT) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DX_R,
					(dwOrientation == __ETWS_ORIENT_TOP) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DY_B
					);
				::CopyRect( &(pNCCSP->rgrc[0]), rcMdiTabWnd );
			}
			return 0;
		}
		break;
		case WM_NCPAINT:
		{
			CRect rcMdiTabWnd, rcMdiTabClient;
			GetWindowRect( &rcMdiTabWnd );
			GetClientRect( &rcMdiTabClient );
			ClientToScreen( &rcMdiTabClient );
			if( rcMdiTabWnd == rcMdiTabClient )
				return 0;
			CPoint ptDevOffset = -rcMdiTabWnd.TopLeft();
			rcMdiTabWnd.OffsetRect( ptDevOffset );
			rcMdiTabClient.OffsetRect( ptDevOffset );
			CWindowDC dcWindow( this );
			ASSERT( dcWindow.GetSafeHdc() != NULL );
			dcWindow.ExcludeClipRect( &rcMdiTabClient );
			CExtMemoryDC dc( &dcWindow );
			dc.FillSolidRect(
				&rcMdiTabWnd,
				PmBridge_GetPM()->GetColor( COLOR_3DFACE, this )
				);
			DWORD dwOrientation = OrientationGet();
			rcMdiTabWnd.InflateRect(
				(dwOrientation != __ETWS_ORIENT_RIGHT) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DX_L,
				(dwOrientation != __ETWS_ORIENT_BOTTOM) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DY_T,
				(dwOrientation != __ETWS_ORIENT_LEFT) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DX_R,
				(dwOrientation != __ETWS_ORIENT_TOP) ? 0: __EXTTAB_MDI_NC_AREA_GAP_DY_B
				);
			PmBridge_GetPM()->PaintTabNcAreaRect(
				dc,
				rcMdiTabWnd,
				this
				);
			return 0;
		}
		break;
		case WM_TIMER:
			if( wParam == __EXTTAB_MDI_UPDATE_TIMER_ID )
			{
				bool bKillTimer = true;
				if( ! m_bInSyncLayout )
					bKillTimer = _SyncAllItems();
				if( bKillTimer )
					KillTimer( __EXTTAB_MDI_UPDATE_TIMER_ID );
				return 0;
			} // if( wParam == __EXTTAB_MDI_UPDATE_TIMER_ID )
// 			if( wParam == __EXTTAB_MDI_UPDATE_TIMER_ID_2 )
// 			{
// 				KillTimer( __EXTTAB_MDI_UPDATE_TIMER_ID_2 );
// 				HWND hWndMainFrame = _GetHwndMainFrame();
// 				if( hWndMainFrame == NULL || (! ::IsWindow( hWndMainFrame ) ) )
// 					return 0;
// 				HWND hWndMdiArea = _GetHwndMdiArea();
// 				if( hWndMdiArea == NULL || (! ::IsWindow( hWndMdiArea ) ) )
// 					return 0;
// 				bool bActiveAbleChildPresent = false;
// 				HWND hWnd = ::GetWindow( hWndMdiArea, GW_CHILD );
// 				for( ; hWnd != NULL; hWnd = ::GetWindow( hWnd, GW_HWNDNEXT ) )
// 				{
// 					DWORD dwStyle = ::__EXT_MFC_GetWindowLong( hWnd, GWL_STYLE );
// 					if( (dwStyle&WS_VISIBLE) == 0 )
// 						continue;
// 					bActiveAbleChildPresent = true;
// 					break;
// 				}
// 				if( ! bActiveAbleChildPresent )
// 				{
// 					::SetForegroundWindow( hWndMainFrame );
// 					::SetActiveWindow( hWndMainFrame );
// 					::SetFocus( hWndMdiArea );
// 				}
// 				return 0;
// 			} // if( wParam == __EXTTAB_MDI_UPDATE_TIMER_ID_2 )
//			if( wParam == __EXTTAB_MDI_UPDATE_TIMER_ID_3 )
//			{
//				KillTimer( __EXTTAB_MDI_UPDATE_TIMER_ID_3 );
//				DWORD dwWndStyle = GetStyle();
//				if( (dwWndStyle & WS_VISIBLE) != 0 )
//					SetWindowPos(
//						NULL, 0, 0, 0, 0,
//						SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOOWNERZORDER
//							|SWP_HIDEWINDOW
//						);
//				return 0;
//			} // if( wParam == __EXTTAB_MDI_UPDATE_TIMER_ID )
		break;
		case WM_CONTEXTMENU:
			{
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
				HWND hWndTrack = _GetHwndMainFrame();
				ASSERT( hWndTrack != NULL && ::IsWindow(hWndTrack) );
				CExtPopupMenuWnd * popup =
					CExtPopupMenuWnd::InstantiatePopupMenu(
						GetSafeHwnd(),
						RUNTIME_CLASS(CExtPopupMenuWnd),
						this
						);
				VERIFY( popup->CreatePopupMenu(hWndTrack) );
				CExtControlBar::POPUP_MENU_EVENT_DATA _pmed(
					CExtControlBar::POPUP_MENU_EVENT_DATA::__PMED_MDITABS_CTX,
					popup,
					this,
					point,
					WM_CONTEXTMENU,
					NULL
					);
				if( !_pmed.NotifyTarget( false ) )
				{
					delete popup;
					return 0;
				} // if( !_pmed.NotifyTarget( false ) )
				_pmed.NotifyTarget( true );
				if( popup->ItemGetCount() == 0 )
				{
					delete popup;
					return 0;
				}
				::SetFocus( hWndTrack );
				HWND hWndOwn = m_hWnd;
				MSG _msg;
				while( ::PeekMessage( &_msg, NULL, WM_WINDOWPOSCHANGING, WM_WINDOWPOSCHANGED, PM_REMOVE ) )
					::DispatchMessage( &_msg );
				while( ::PeekMessage( &_msg, NULL, WM_SIZE, WM_SIZE, PM_REMOVE ) )
					::DispatchMessage( &_msg );
				if( ! ::IsWindow(hWndOwn) )
					return 0;
				if(	! popup->TrackPopupMenu(
						TPMX_OWNERDRAW_FIXED|TPMX_DO_MESSAGE_LOOP,
						point.x,
						point.y
						)
					)
				{
					delete popup;
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
}; // class CExtTMWI

/////////////////////////////////////////////////////////////////////////////
// CExtTabMdiWnd window

class __PROF_UIS_API CExtTabMdiWnd : public CExtTMWI < CExtTabWnd >
{
public:
	DECLARE_DYNCREATE( CExtTabMdiWnd );
	CExtTabMdiWnd();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTabMdiWnd)
	protected:
	//}}AFX_VIRTUAL
	// Implementation
public:
	virtual ~CExtTabMdiWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
protected:
	//{{AFX_MSG(CExtTabMdiWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtTabMdiWnd

#if (!defined __EXT_MFC_NO_DYNAMIC_BAR_SITE)

template < class _BC >
class __PROF_UIS_API CExtTMDBS
	: public _BC
{
public:
	virtual void OnTabWndDrawItem(
		CDC & dc,
		CRect & rcTabItemsArea,
		LONG nItemIndex,
		CExtTabWnd::TAB_ITEM_INFO * pTii,
		bool bTopLeft,
		bool bHorz,
		bool bSelected,
		bool bCenteredText,
		bool bGroupedMode,
		bool bInGroupActive,
		bool bInvertedVerticalMode,
		const CRect & rcEntireItem,
		CSize sizeTextMeasured,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon
		)
	{
		dc;
		rcTabItemsArea;
		nItemIndex;
		pTii;
		bTopLeft;
		bHorz;
		bSelected;
		bCenteredText;
		bGroupedMode;
		bInGroupActive;
		bInvertedVerticalMode;
		rcEntireItem;
		sizeTextMeasured;
		pFont;
		sText;
		pIcon;
		ASSERT( FALSE );
		// The un-specialized version of the CExtTMDBS class should not be used.
	}
}; // class CExtTMDBS

template < >
class __PROF_UIS_API CExtTMDBS < CExtTabMdiWnd > : public CExtTabMdiWnd
{
public:
	virtual void OnTabWndDrawItem(
		CDC & dc,
		CRect & rcTabItemsArea,
		LONG nItemIndex,
		CExtTabWnd::TAB_ITEM_INFO * pTii,
		bool bTopLeft,
		bool bHorz,
		bool bSelected,
		bool bCenteredText,
		bool bGroupedMode,
		bool bInGroupActive,
		bool bInvertedVerticalMode,
		const CRect & rcEntireItem,
		CSize sizeTextMeasured,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon
		);
}; // class CExtTMDBS < CExtTabMdiWnd >

//#if _MFC_VER >= 0x700
//	#ifndef __EXT_PROFUIS_STATIC_LINK
//		template class __PROF_UIS_API CExtTMDBS < CExtTabMdiWnd >;
//	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
//#endif // _MFC_VER >= 0x700

#endif // (!defined __EXT_MFC_NO_DYNAMIC_BAR_SITE )

#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL )

// tab window class name
#define __EXT_TAB_WND_CLASS_NAME _T("ProfUIS-TabWindow")

// helper values
#define __EXTTAB_MARGIN_BORDER_HX				2
#define __EXTTAB_MARGIN_BORDER_HY				2
#define __EXTTAB_MARGIN_BORDER_VX				2
#define __EXTTAB_MARGIN_BORDER_VY				2

#define __EXTTAB_MARGIN_ICON2TEXT_X				3
#define __EXTTAB_MARGIN_ICON2TEXT_Y				3

#define __EXTTAB_MIN_HORZ_WIDTH					24 // 6
#define __EXTTAB_MIN_HORZ_HEIGHT				20

#define __EXTTAB_MIN_VERT_WIDTH					20
#define __EXTTAB_MIN_VERT_HEIGHT				24 // 6

#define __EXTTAB_SEPARATOR_AREA_X				3
#define __EXTTAB_SEPARATOR_AREA_Y				3

#define __EXTTAB_SEPARATOR_DX					1
#define __EXTTAB_SEPARATOR_DY					1
#define __EXTTAB_SEPARATOR_GAP_HORZ				2
#define __EXTTAB_SEPARATOR_GAP_VERT				2

#define __EXTTAB_ADD_END_SCROLL_SPACE			6

#define __EXTTAB_BTN_MIN_DX						12
#define __EXTTAB_BTN_MIN_DY						12
#define __EXTTAB_BTN_GAP_DX						2
#define __EXTTAB_BTN_GAP_DY						2
#define __EXTTAB_BETWEEN_BTN_GAP_DX				2
#define __EXTTAB_BETWEEN_BTN_GAP_DY				2

#define __EXTTAB_SCROLL_TIMER_ID				1
#define __EXTTAB_SCROLL_TIMER_PERIOD			20
#define __EXTTAB_SCROLL_STEP					10

#define __EXTTAB_BETWEEN_GROUP_GAP_DX			20
#define __EXTTAB_BETWEEN_GROUP_GAP_DY			20

#define __EXTTAB_SELECTION_DELAY_TIMER_ID		2
#define __EXTTAB_SELECTION_DELAY_TIMER_PERIOD	200

#define __EXTTAB_FLOATING_BAR_TIMER_ID			3
#define __EXTTAB_FLOATING_BAR_TIMER_PERIOD		50

#define __EXTTAB_IN_FRAME_GAP_LEFT_DX			6
#define __EXTTAB_IN_FRAME_GAP_RIGHT_DX			6
#define __EXTTAB_IN_FRAME_GAP_TOP_DY			6
#define __EXTTAB_IN_FRAME_GAP_BOTTOM_DY			6

#define __EXTTAB_DRAG_START_DX					5
#define __EXTTAB_DRAG_START_DY					5

/////////////////////////////////////////////////////////////////////////////
// CExtTabOneNoteWnd window

#ifndef __EXT_MFC_NO_TAB_ONENOTE_CTRL

// default set of styles for One Note tab window
#define __ETWS_ONENOTE_DEFAULT \
	( __ETWS_DEFAULT \
	| __ETWS_CENTERED_TEXT \
	| __ETWS_BOLD_SELECTION \
	)

class __PROF_UIS_API CExtTabOneNoteWnd : public CExtTabWnd
{
public:
	CExtTabOneNoteWnd();
	virtual ~CExtTabOneNoteWnd();

	DECLARE_DYNCREATE( CExtTabOneNoteWnd );

	virtual bool _IsCustomLayoutTabWnd() const;

	class __PROF_UIS_API TAB_ITEM_INFO_ONENOTE : public CExtTabWnd::TAB_ITEM_INFO
	{
	protected:
		COLORREF m_clrBkLight;
		COLORREF m_clrBkDark;
		void _AssignFromOther(
			const TAB_ITEM_INFO_ONENOTE & other
			)
		{
			ASSERT_VALID( (&other) );
			m_pWndTab = other.m_pWndTab;
			m_dwItemStyle = other.m_dwItemStyle;
			m_dwItemStyleEx = other.m_dwItemStyleEx;
			m_sText = other.m_sText;
			m_sTooltipText = other.m_sTooltipText;
			m_icon = other.m_icon;
			m_pPrev = other.m_pPrev;
			m_pNext = other.m_pNext;
			m_sizeLastMeasuredItem = other.m_sizeLastMeasuredItem;
			m_sizeLastMeasuredText = other.m_sizeLastMeasuredText;
			m_sizeLastMeasuredIcon = other.m_sizeLastMeasuredIcon;
			m_rcItem = other.m_rcItem;
			m_rcCloseButton = other.m_rcCloseButton;
			m_lParam = other.m_lParam;
			m_pEventProvider = other.m_pEventProvider;
			m_bHelperToolTipAvail = other.m_bHelperToolTipAvail;
			m_bMultiRowWrap = other.m_bMultiRowWrap;
			m_clrBkLight = other.m_clrBkLight;
			m_clrBkDark = other.m_clrBkDark;
			ASSERT_VALID( this );
		}

	public:
		TAB_ITEM_INFO_ONENOTE(
			CExtTabWnd * pWndTab = NULL,
			TAB_ITEM_INFO * pPrev = NULL,
			TAB_ITEM_INFO * pNext = NULL,
			__EXT_MFC_SAFE_LPCTSTR sText = NULL,
			const CExtCmdIcon * pIcon = NULL, // CHANGED 2.53
			DWORD dwStyle = 0, // visible & non-group
			LPARAM lParam = 0,
			COLORREF clrBkLight = (COLORREF)(-1L),
			COLORREF clrBkDark = (COLORREF)(-1L)
		)
		: TAB_ITEM_INFO(
			pWndTab,
			pPrev,
			pNext,
			sText,
			pIcon,
			dwStyle,
			lParam
			)
			, m_clrBkLight( clrBkLight )
			, m_clrBkDark( clrBkDark )
		{
		}
		TAB_ITEM_INFO_ONENOTE( const TAB_ITEM_INFO_ONENOTE & other )
			: TAB_ITEM_INFO( other )
		{
			_AssignFromOther( other );
		}
		~TAB_ITEM_INFO_ONENOTE()
		{
		}
		TAB_ITEM_INFO & operator = (
			const TAB_ITEM_INFO_ONENOTE & other
			)
		{
			_AssignFromOther( other );
			return *this;
		}

		COLORREF GetColorBkLight() const
		{
			ASSERT_VALID( this );
			return m_clrBkLight;
		}

		COLORREF GetColorBkDark() const
		{
			ASSERT_VALID( this );
			return m_clrBkDark;
		}

		void SetColorBkLight( COLORREF clrBkLight )
		{
			ASSERT_VALID( this );
			m_clrBkLight = clrBkLight;
			GetTabWnd()->UpdateTabWnd( true );
		}

		void SetColorBkDark( COLORREF clrBkDark )
		{
			ASSERT_VALID( this );
			m_clrBkDark = clrBkDark;
			GetTabWnd()->UpdateTabWnd( true );
		}

	}; // class TAB_ITEM_INFO_ONENOTE

	virtual BOOL Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( __EXT_MFC_IDC_STATIC ),
		DWORD dwWindowStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		DWORD dwTabStyle = __ETWS_ONENOTE_DEFAULT,
		CCreateContext * pContext = NULL
		);

	TAB_ITEM_INFO_ONENOTE * ItemInsert(
		__EXT_MFC_SAFE_LPCTSTR sText = NULL,
		HICON hIcon = NULL,
		bool bCopyIcon = true,
		DWORD dwItemStyle = 0, // visible & non-group
		LONG nIndex = -1, // default - append
		LPARAM lParam = 0,
		COLORREF clrBkLight = (COLORREF)(-1L),
		COLORREF clrBkDark = (COLORREF)(-1L),
		bool bUpdateTabWnd = false
		);
	TAB_ITEM_INFO_ONENOTE * ItemInsert( // ADDED 2.53
		__EXT_MFC_SAFE_LPCTSTR sText,
		const CExtCmdIcon & _icon,
		DWORD dwItemStyle = 0, // visible & non-group
		LONG nIndex = -1, // default - append
		LPARAM lParam = 0,
		COLORREF clrBkLight = (COLORREF)(-1L),
		COLORREF clrBkDark = (COLORREF)(-1L),
		bool bUpdateTabWnd = false
		);

	TAB_ITEM_INFO_ONENOTE * ItemGet( LONG nIndex );
	const TAB_ITEM_INFO_ONENOTE * ItemGet( LONG nIndex ) const;
	
	const TAB_ITEM_INFO_ONENOTE * SelectionGetPtr() const;
	TAB_ITEM_INFO_ONENOTE * SelectionGetPtr();

	virtual void OnTabWndQueryItemColors(
		LONG nItemIndex,
		bool bSelected,
		bool bHover,
		bool bEnabled,
		COLORREF * pclrBorderLight = NULL,
		COLORREF * pclrBorderDark = NULL,
		COLORREF * pclrBkLight = NULL,
		COLORREF * pclrBkDark = NULL,
		COLORREF * pclrText = NULL
		);

	void GenerateItemBkColors(
		LONG nIndex,
		COLORREF * pclrBkLight = NULL,
		COLORREF * pclrBkDark = NULL
	);

protected:
	LONG m_nNextColorIndex;

	virtual bool _ItemMoveImpl( // move item into the new position
		LONG nIndex,
		LONG nIndexNew
		);

	virtual bool OnTabWndQueryHoverChangingRedraw() const;

	virtual void OnTabWndMeasureItemAreaMargins(
		LONG & nSpaceBefore,
		LONG & nSpaceAfter,
		LONG & nSpaceOver
		);

	virtual void OnTabWndUpdateItemMeasure(
		TAB_ITEM_INFO * pTii,
		CDC & dcMeasure,
		CSize & sizePreCalc
		);

	virtual void OnTabWndEraseClientArea(
		CDC & dc,
		CRect & rcClient,
		CRect & rcTabItemsArea,
		CRect & rcTabNearBorderArea,
		DWORD dwOrientation,
		bool bGroupedMode
		);
	
	virtual void OnTabWndDrawItem(
		CDC & dc,
		CRect & rcTabItemsArea,
		LONG nItemIndex,
		TAB_ITEM_INFO * pTii,
		bool bTopLeft,
		bool bHorz,
		bool bSelected,
		bool bCenteredText,
		bool bGroupedMode,
		bool bInGroupActive,
		bool bInvertedVerticalMode,
		const CRect & rcEntireItem,
		CSize sizeTextMeasured,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon
		);
	static void stat_OnTabWndDrawItemOneNoteImpl(
		CExtTabOneNoteWnd * pWndTabs,
		CDC & dc,
		CRect & rcTabItemsArea,
		LONG nItemIndex,
		TAB_ITEM_INFO * pTii,
		bool bTopLeft,
		bool bHorz,
		bool bSelected,
		bool bCenteredText,
		bool bGroupedMode,
		bool bInGroupActive,
		bool bInvertedVerticalMode,
		const CRect & rcEntireItem,
		CSize sizeTextMeasured,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon,
		COLORREF clrForceTabText = COLORREF(-1L),
		COLORREF clrForceTabBk1 = COLORREF(-1L),
		COLORREF clrForceTabBk2 = COLORREF(-1L)
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTabOneNoteWnd)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

}; // class CExtTabOneNoteWnd

#if (!defined __EXT_MFC_NO_TABMDI_CTRL )

/////////////////////////////////////////////////////////////////////////////
// CExtTabMdiOneNoteWnd window

class __PROF_UIS_API CExtTabMdiOneNoteWnd : public CExtTMWI < CExtTabOneNoteWnd >
{
public:
	DECLARE_DYNCREATE( CExtTabMdiOneNoteWnd );
	CExtTabMdiOneNoteWnd();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTabMdiOneNoteWnd)
	protected:
	//}}AFX_VIRTUAL
	// Implementation
protected:
	void OnMdiTabImplAdjustBorderSpaces();
	virtual void OnMdiTabImplDrawOuterBorder(
		CDC & dc,
		const CRect & rcOuterBorder,
		const CRect & rcMdiAreaClient,
		const CRect & rcMdiAreaWnd
		);
public:
	virtual ~CExtTabMdiOneNoteWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
protected:
	//{{AFX_MSG(CExtTabMdiOneNoteWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtTabMdiOneNoteWnd

#if (!defined __EXT_MFC_NO_DYNAMIC_BAR_SITE)

template < >
class __PROF_UIS_API CExtTMDBS < CExtTabMdiOneNoteWnd >
	: public CExtTabMdiOneNoteWnd
{
public:
	virtual void OnTabWndDrawItem(
		CDC & dc,
		CRect & rcTabItemsArea,
		LONG nItemIndex,
		CExtTabWnd::TAB_ITEM_INFO * pTii,
		bool bTopLeft,
		bool bHorz,
		bool bSelected,
		bool bCenteredText,
		bool bGroupedMode,
		bool bInGroupActive,
		bool bInvertedVerticalMode,
		const CRect & rcEntireItem,
		CSize sizeTextMeasured,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon
		);
}; // class CExtTMDBS < CExtTabMdiOneNoteWnd >

//#if _MFC_VER >= 0x700
//	#ifndef __EXT_PROFUIS_STATIC_LINK
//		template class __PROF_UIS_API CExtTMDBS < CExtTabMdiOneNoteWnd >;
//	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
//#endif // _MFC_VER >= 0x700

#endif // (!defined __EXT_MFC_NO_DYNAMIC_BAR_SITE )

#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL )

#endif // __EXT_MFC_NO_TAB_ONENOTE_CTRL


/////////////////////////////////////////////////////////////////////////////
// CExtTabWhidbeyWnd window

#ifndef __EXT_MFC_NO_TAB_WHIDBEY_CTRL

// default set of styles
#define __ETWS_WHIDBEY_DEFAULT \
	( __ETWS_CENTERED_TEXT \
	| __ETWS_BOLD_SELECTION \
	| __ETWS_ORIENT_TOP \
	)

class __PROF_UIS_API CExtTabWhidbeyWnd : public CExtTabWnd
{
public:
	CExtTabWhidbeyWnd();
	virtual ~CExtTabWhidbeyWnd();

	DECLARE_DYNCREATE( CExtTabWhidbeyWnd );

	virtual bool _IsCustomLayoutTabWnd() const;

	virtual BOOL Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( __EXT_MFC_IDC_STATIC ),
		DWORD dwWindowStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		DWORD dwTabStyle = __ETWS_WHIDBEY_DEFAULT,
		CCreateContext * pContext = NULL
		);

	virtual void OnTabWndQueryItemColors(
		LONG nItemIndex,
		bool bSelected,
		bool bHover,
		bool bEnabled,
		COLORREF * pclrBorder = NULL,
		COLORREF * pclrBkLight = NULL,
		COLORREF * pclrBkDark = NULL,
		COLORREF * pclrText = NULL
		);

protected:

	virtual void OnTabWndMeasureItemAreaMargins(
		LONG & nSpaceBefore,
		LONG & nSpaceAfter,
		LONG & nSpaceOver
		);

	virtual void OnTabWndUpdateItemMeasure(
		TAB_ITEM_INFO * pTii,
		CDC & dcMeasure,
		CSize & sizePreCalc
		);

	virtual void OnTabWndEraseClientArea(
		CDC & dc,
		CRect & rcClient,
		CRect & rcTabItemsArea,
		CRect & rcTabNearBorderArea,
		DWORD dwOrientation,
		bool bGroupedMode
		);
	
	virtual void OnTabWndDrawItem(
		CDC & dc,
		CRect & rcTabItemsArea,
		LONG nItemIndex,
		TAB_ITEM_INFO * pTii,
		bool bTopLeft,
		bool bHorz,
		bool bSelected,
		bool bCenteredText,
		bool bGroupedMode,
		bool bInGroupActive,
		bool bInvertedVerticalMode,
		const CRect & rcEntireItem,
		CSize sizeTextMeasured,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon
		);
	static void stat_OnTabWndDrawItemWhidbeyImpl(
		CExtTabWhidbeyWnd * pWndTabs,
		CDC & dc,
		CRect & rcTabItemsArea,
		LONG nItemIndex,
		TAB_ITEM_INFO * pTii,
		bool bTopLeft,
		bool bHorz,
		bool bSelected,
		bool bCenteredText,
		bool bGroupedMode,
		bool bInGroupActive,
		bool bInvertedVerticalMode,
		const CRect & rcEntireItem,
		CSize sizeTextMeasured,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon,
		COLORREF clrForceTabText = COLORREF(-1L),
		COLORREF clrForceTabBk1 = COLORREF(-1L),
		COLORREF clrForceTabBk2 = COLORREF(-1L)
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTabWhidbeyWnd)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

}; // class CExtTabWhidbeyWnd

#if (!defined __EXT_MFC_NO_TABMDI_CTRL )

/////////////////////////////////////////////////////////////////////////////
// CExtTabMdiWhidbeyWnd window

class __PROF_UIS_API CExtTabMdiWhidbeyWnd : public CExtTMWI < CExtTabWhidbeyWnd >
{
public:
	DECLARE_DYNCREATE( CExtTabMdiWhidbeyWnd );
	CExtTabMdiWhidbeyWnd();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTabMdiWhidbeyWnd)
	protected:
	//}}AFX_VIRTUAL
	// Implementation
public:
	virtual ~CExtTabMdiWhidbeyWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
protected:
	//{{AFX_MSG(CExtTabMdiWhidbeyWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtTabMdiWhidbeyWnd

#if (!defined __EXT_MFC_NO_DYNAMIC_BAR_SITE)

template < >
class __PROF_UIS_API CExtTMDBS < CExtTabMdiWhidbeyWnd >
	: public CExtTabMdiWhidbeyWnd
{
public:
	virtual void OnTabWndDrawItem(
		CDC & dc,
		CRect & rcTabItemsArea,
		LONG nItemIndex,
		CExtTabWnd::TAB_ITEM_INFO * pTii,
		bool bTopLeft,
		bool bHorz,
		bool bSelected,
		bool bCenteredText,
		bool bGroupedMode,
		bool bInGroupActive,
		bool bInvertedVerticalMode,
		const CRect & rcEntireItem,
		CSize sizeTextMeasured,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon
		);
}; // class CExtTMDBS < CExtTabMdiWhidbeyWnd >

//#if _MFC_VER >= 0x700
//	#ifndef __EXT_PROFUIS_STATIC_LINK
//		template class __PROF_UIS_API CExtTMDBS < CExtTabMdiWhidbeyWnd >;
//	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
//#endif // _MFC_VER >= 0x700

#endif // (!defined __EXT_MFC_NO_DYNAMIC_BAR_SITE )

#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL )

#endif // __EXT_MFC_NO_TAB_WHIDBEY_CTRL


#endif // (!defined __EXT_MFC_NO_TAB_CTRL )

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __EXT_TABWND_H

