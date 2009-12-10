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

#if (!defined __EXTSHORTCUTLISTWND_H)
#define __EXTSHORTCUTLISTWND_H

#if (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXTEMPL_H__
	#include <AfxTempl.h>
#endif

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_EXTCONTENTEXPANDWND_H)
	#include <ExtContentExpandWnd.h>
#endif

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

#if (!defined __EXT_MOUSECAPTURESINK_H)
	#include <../Src/ExtMouseCaptureSink.h>
#endif

// shortcut list window styles

#define __ESLW_HORIZONTAL				0x00000001L
#define __ESLW_BIG_ICON_VIEW			0x00000002L
#define __ESLW_ITEM_TOOLTIP				0x00000004L
#define __ESLW_ITEM_EXPANDING			0x00000008L
#define __ESLW_INV_VFONT				0x00000010L
#define __ESLW_SMOOTH_SCROLL			0x00000020L
#define __ESLW_EDIT_ITEMS				0x00000040L
#define __ESLW_START_DRAG				0x00000080L
#define __ESLW_USER_DEFINED_23			0x00000100L
#define __ESLW_USER_DEFINED_22			0x00000200L
#define __ESLW_USER_DEFINED_21			0x00000400L
#define __ESLW_USER_DEFINED_20			0x00000800L
#define __ESLW_USER_DEFINED_19			0x00001000L
#define __ESLW_USER_DEFINED_18			0x00002000L
#define __ESLW_USER_DEFINED_17			0x00004000L
#define __ESLW_USER_DEFINED_16			0x00008000L
#define __ESLW_USER_DEFINED_15			0x00010000L
#define __ESLW_USER_DEFINED_14			0x00020000L
#define __ESLW_USER_DEFINED_13			0x00040000L
#define __ESLW_USER_DEFINED_12			0x00080000L
#define __ESLW_USER_DEFINED_11			0x00100000L
#define __ESLW_USER_DEFINED_10			0x00200000L
#define __ESLW_USER_DEFINED_09			0x00400000L
#define __ESLW_USER_DEFINED_08			0x00800000L
#define __ESLW_USER_DEFINED_07			0x01000000L
#define __ESLW_USER_DEFINED_06			0x02000000L
#define __ESLW_USER_DEFINED_05			0x04000000L
#define __ESLW_USER_DEFINED_04			0x08000000L
#define __ESLW_USER_DEFINED_03			0x10000000L
#define __ESLW_USER_DEFINED_02			0x20000000L
#define __ESLW_USER_DEFINED_01			0x40000000L
#define __ESLW_USER_DEFINED_00			0x80000000L

#define __ESLW_DEF_STYLES \
	(	__ESLW_BIG_ICON_VIEW \
	|	__ESLW_SMOOTH_SCROLL \
	|	__ESLW_ITEM_EXPANDING \
	)

//	|	__ESLW_ITEM_TOOLTIP \
//	|	__ESLW_EDIT_ITEMS \
//	|	__ESLW_START_DRAG \
//	|	__ESLW_HORIZONTAL|__ESLW_INV_VFONT \

// shortcut list hit-test codes
// (if greater 0 - item index )

#define __ESLW_HT_BUTTON_UP		(-1L)
#define __ESLW_HT_BUTTON_DOWN	(-2L)
#define __ESLW_HT_NOWHERE		(-3L)

/////////////////////////////////////////////////////////////////////////////
// CExtShortcutListWnd window

class __PROF_UIS_API CExtShortcutListWnd
	: public CWnd
	, public CExtMouseCaptureSink
	, public CExtPmBridge
{
// Construction
public:
	CExtShortcutListWnd();

	DECLARE_DYNCREATE( CExtShortcutListWnd );
	DECLARE_CExtPmBridge_MEMBERS( CExtShortcutListWnd );

	class __PROF_UIS_API SHORTCUT_ITEM_INFO
	{
		void _AssignFromOther(
			const SHORTCUT_ITEM_INFO & other
			)
		{
			m_iconBig = other.m_iconBig;
			m_iconSmall = other.m_iconSmall;
			m_lParam = other.m_lParam;
			m_rcBoundsAll = other.m_rcBoundsAll;
			m_rcBoundsIcon = other.m_rcBoundsIcon;
			m_rcBoundsText = other.m_rcBoundsText;
			m_sText = other.m_sText;
			m_bHover = other.m_bHover;
			m_bPressed = other.m_bPressed;
			m_bVisible = other.m_bVisible;
			m_bChecked = other.m_bChecked;
		}
	public:
		CExtCmdIcon m_iconBig, m_iconSmall;
		LPARAM m_lParam;
		CRect m_rcBoundsAll, m_rcBoundsIcon, m_rcBoundsText;
		CExtSafeString m_sText;
		bool m_bHover:1, m_bPressed:1, m_bVisible:1,
			m_bChecked:1;

	public:
		void Clear()
		{
			m_iconBig.Empty();
			m_iconSmall.Empty();
			m_lParam = 0L;
			m_rcBoundsAll.SetRect( 0, 0, 0, 0 );
			m_rcBoundsIcon.SetRect( 0, 0, 0, 0 );
			m_rcBoundsText.SetRect( 0, 0, 0, 0 );
			m_sText = _T("");
			m_bHover = m_bPressed = m_bChecked = false;
			m_bVisible = true;
		}
		
		SHORTCUT_ITEM_INFO()
		{
			Clear();
		}
		SHORTCUT_ITEM_INFO(
			const SHORTCUT_ITEM_INFO & other
			)
		{
			_AssignFromOther( other );
		}
		~SHORTCUT_ITEM_INFO()
		{
		}
		SHORTCUT_ITEM_INFO & operator = (
			const SHORTCUT_ITEM_INFO & other
			)
		{
			_AssignFromOther( other );
			return *this;
		}
	}; // class SHORTCUT_ITEM_INFO

	class __PROF_UIS_API CItemTextEditWnd : public CEdit
	{
		SHORTCUT_ITEM_INFO * m_pSII;
		bool m_bInEndEdit:1;
	public:

		CItemTextEditWnd(
			SHORTCUT_ITEM_INFO * pSII
			)
			: m_pSII( pSII )
			, m_bInEndEdit( false )
		{
			ASSERT( m_pSII != NULL );
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

protected:
	HFONT m_hFont;

	typedef
		CArray <
			SHORTCUT_ITEM_INFO *,
			SHORTCUT_ITEM_INFO *
			>
		vShortcutItems_t;

	vShortcutItems_t m_vItems;

	bool m_bDirectCreateCall:1;
	bool m_bDelayRecalcLayout:1;
	bool m_bScrolling:1;
	bool m_bCancelingActionsMode:1;
	bool m_bInLayoutRecalcProc:1;
	bool m_bPressedTracking:1;
	LONG m_nTrackingHT;
	DWORD m_dwShortcutListStyle;
	CExtContentExpandWnd m_wndContentExpand;
	CToolTipCtrl m_wndToolTip;
	HWND m_hWndEditor;
	LONG m_nScrollPos, m_nScrollSize, m_nScrollStep;
	LONG m_nIndexVisibleFirst, m_nIndexVisibleLast;
	UINT m_nScrollPeriod;
	CRect m_rcButtonUp, m_rcButtonDown;
	CPoint m_ptStartLeftBtnTrack;

// Operations
public:
	BOOL Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( __EXT_MFC_IDC_STATIC ),
		DWORD dwWindowStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		DWORD dwShortcutListStyle = __ESLW_DEF_STYLES,
		CCreateContext * pContext = NULL
		);

	void UpdateShortcutListWnd( bool bUpdateNow );

	DWORD GetShortcutListStyle() const
	{
		ASSERT_VALID( this );
		return m_dwShortcutListStyle;
	}
	DWORD ModifyShortcutListStyle(
		DWORD dwAdd,
		DWORD dwRemove = 0L,
		bool bUpdateNow = false
		)
	{
		ASSERT_VALID( this );
		DWORD dwShortcutListStyleOld = m_dwShortcutListStyle;
		m_dwShortcutListStyle &= ~dwRemove;
		m_dwShortcutListStyle |= dwAdd;
		UpdateShortcutListWnd( bUpdateNow );
		return dwShortcutListStyleOld;
	}
	
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
			( const_cast < CExtShortcutListWnd * > ( this ) )
				-> m_hWndEditor;
	}

	bool IsHorizontalLayout() const
	{
		ASSERT_VALID( this );
		bool bBigIconView =
			( m_dwShortcutListStyle & __ESLW_HORIZONTAL )
				? true
				: false
				;
		return bBigIconView;
	}

	bool IsBigIconView() const
	{
		ASSERT_VALID( this );
		bool bBigIconView =
			( m_dwShortcutListStyle & __ESLW_BIG_ICON_VIEW )
				? true
				: false
				;
		return bBigIconView;
	}
	void SetBigIconView(
		bool bBigIconView = true,
		bool bUpdateNow = false
		)
	{
		ASSERT_VALID( this );
		bool bBigIconViewOld = IsBigIconView();
		if(		( bBigIconViewOld && bBigIconView )
			||	( (!bBigIconViewOld) && (!bBigIconView) )
			)
		{
			UpdateShortcutListWnd( bUpdateNow );
			return;
		}
		ModifyShortcutListStyle(
			bBigIconView ? __ESLW_BIG_ICON_VIEW : 0,
			bBigIconView ? 0 : __ESLW_BIG_ICON_VIEW,
			bUpdateNow
			);
	}

	void GetVisibleRange(
		LONG & nIndexVisibleFirst,
		LONG & nIndexVisibleLast
		) const
	{
		ASSERT_VALID( this );
		nIndexVisibleFirst = m_nIndexVisibleFirst;
		nIndexVisibleLast = m_nIndexVisibleLast;
	}

	LONG ScrollSizeGet() const;
	LONG ScrollPosGet() const;
	void ScrollPosSet(
		LONG nScrollPos,
		bool bUpdateNow = false
		);

	LONG ItemGetCount() const;
	const CExtShortcutListWnd::SHORTCUT_ITEM_INFO * ItemGetInfo(
		LONG nIndex
		) const;
	CExtShortcutListWnd::SHORTCUT_ITEM_INFO * ItemGetInfo(
		LONG nIndex
		);
	CExtShortcutListWnd::SHORTCUT_ITEM_INFO & ItemInsert(
		LONG nIndexInsertBefore = -1L, // append
		__EXT_MFC_SAFE_LPCTSTR sText = NULL,
		HICON hIconBig = NULL,
		bool bCopyIconBig = true,
		HICON hIconSmall = NULL,
		bool bCopyIconSmall = true,
		bool bUpdateNow = false
		);
	CExtShortcutListWnd::SHORTCUT_ITEM_INFO & ItemInsert(
		const CExtCmdIcon & iconBig,
		const CExtCmdIcon & iconSmall,
		LONG nIndexInsertBefore = -1L, // append
		__EXT_MFC_SAFE_LPCTSTR sText = NULL,
		bool bUpdateNow = false
		);
	LONG ItemRemove(
		LONG nIndex,
		LONG nCount = 1L,
		bool bUpdateNow = false
		);
	LONG ItemRemoveAll(
		bool bUpdateNow = false
		);
	LONG ItemGetIndexOf(
		const CExtShortcutListWnd::SHORTCUT_ITEM_INFO * pSII
		) const;
	LONG ItemGetIndexOf(
		CExtShortcutListWnd::SHORTCUT_ITEM_INFO * pSII
		)
	{
		const CExtShortcutListWnd * pConstThis = this;
		return pConstThis->ItemGetIndexOf( pSII );
	}
	LONG ItemGetIndexOf(
		const CExtShortcutListWnd::SHORTCUT_ITEM_INFO & _sii
		) const
	{
		return ItemGetIndexOf( &_sii );
	}
	LONG ItemGetIndexOf(
		CExtShortcutListWnd::SHORTCUT_ITEM_INFO & _sii
		)
	{
		return ItemGetIndexOf( &_sii );
	}

	LONG ItemHitTest(
		const POINT & ptClient,
		bool bTestIconArea = true,
		bool bTestTextArea = true
		) const;
	void ItemGetRects(
		LONG nIndex,
		RECT * pRectBoundsIcon = NULL,
		RECT * pRectBoundsText = NULL,
		RECT * pRectBoundsAll = NULL
		) const;
	bool ItemEnsureVisible(
		LONG nIndex,
		bool bUpdateNow = false
		);
	bool ItemIsPartiallyVisible(
		LONG nIndex,
		bool bTestVerticalVisibility = true,
		bool bTestHorizontalVisibility = true,
		bool bTestIconVisibility = true,
		bool bTestTextVisibility = true
		) const;
	bool ItemBeginEdit( LONG nIndex );

protected:

	virtual void OnShortcutListItemMeasure(
		CDC & dc,
		const CRect & rcClient,
		CExtShortcutListWnd::SHORTCUT_ITEM_INFO & _sii
		);
	virtual CFont * OnShortcutListItemGetFont(
		bool bHorzLayout,
		CExtShortcutListWnd::SHORTCUT_ITEM_INFO & _sii
		);

	virtual void OnShortcutListItemInserted(
		LONG nIndex,
		CExtShortcutListWnd::SHORTCUT_ITEM_INFO & _sii
		);
	virtual bool OnShortcutListItemRemoving(
		LONG nIndex,
		CExtShortcutListWnd::SHORTCUT_ITEM_INFO & _sii
		);
	virtual void OnShortcutListItemRemoved(
		LONG nIndex,
		CExtShortcutListWnd::SHORTCUT_ITEM_INFO & _sii
		);

	virtual void OnShortcutListEraseEntire(
		CDC & dc,
		const RECT & rcClient
		);
	virtual void OnShortcutListDrawEntire(
		CDC & dc,
		const RECT & rcClient
		);
	virtual void OnShortcutListEraseItem(
		CDC & dc,
		CExtShortcutListWnd::SHORTCUT_ITEM_INFO & _sii,
		CRect rcClient,
		CRect rcBoundsIcon,
		CRect rcBoundsText,
		CRect rcBoundsAll,
		bool bExpandMode
		);
	virtual void OnShortcutListDrawItem(
		CDC & dc,
		CExtShortcutListWnd::SHORTCUT_ITEM_INFO & _sii,
		CRect rcClient,
		CRect rcBoundsIcon,
		CRect rcBoundsText,
		CRect rcBoundsAll,
		bool bExpandMode
		);
	virtual void OnShortcutListDrawButton(
		CDC & dc,
		CRect rcButton,
		bool bUpBtn,
		bool bHover,
		bool bPressed
		);

	virtual HWND OnShortcutListStartItemEditor(
		LONG nIndex
		);
	virtual void OnShortcutListCancelItemEditor(
		HWND hWndEditor
		);

	virtual void OnShortcutListCalcButtonsPos(
		CRect & rcButtonUp,
		CRect & rcButtonDown,
		const CRect & rcClient,
		bool bHorzLayout
		);

	virtual bool OnShortcutCanDoHover() const;
	virtual bool OnShortcutListItemHoverStart(
		LONG nIndex,
		bool bPreHoverTest
		);
	virtual void OnShortcutListItemHoverStop(
		LONG nIndex
		);
	virtual bool OnShortcutListItemPressedStart(
		LONG nIndex,
		bool bPrePressedTest
		);
	virtual void OnShortcutListItemPressedStop(
		LONG nIndex,
		bool bOnItem
		);

	virtual void OnShortcutListMeasureScrollParms(
		LONG & nScrollStep,
		UINT & nScrollPeriod,
		bool bSmoothScroll
		);

	virtual bool OnShortcutListCanStartDrag(
		LONG nIndex,
		CPoint ptClientTrackStart,
		CPoint ptClientTrackCurrent
		);
	virtual void OnShortcutListStartItemDrag(
		LONG nIndex,
		CPoint ptClientTrackStart,
		CPoint ptClientTrackCurrent
		);

public:
	virtual bool OnShortcutListDeactivateEditor(
		CExtShortcutListWnd::SHORTCUT_ITEM_INFO * pSII,
		bool bUpdateDeactivation
		);

protected:
	static bool g_bShortcutListWndClassRegistered;
	static bool RegisterShortcutListWndClass();
	bool _CreateHelper();
	void _CancelActions();
	void _RecalcLayoutImpl();
	void _AnalyzeHover( CPoint point );
	bool _ProcessMouseWheel( short zDelta, bool bDirectMsg );
	
	virtual void DoPaint( 
		CDC * pDC,
		CRect & rcClient
		);

public:
	virtual void PmBridge_OnPaintManagerChanged(
		CExtPaintManager * pGlobalPM
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtShortcutListWnd)
	public:
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtShortcutListWnd();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CExtShortcutListWnd)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCancelMode();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg LRESULT OnSetFont( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetFont( WPARAM, LPARAM );
	//}}AFX_MSG

	afx_msg LRESULT _OnPaintExpanedItemContent(WPARAM wParam, LPARAM lParam);

#if _MFC_VER < 0x700
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#else
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
#endif


	DECLARE_MESSAGE_MAP()
}; // class CExtShortcutListWnd

/////////////////////////////////////////////////////////////////////////////

#define __EXT_SHORTCUTLISTWND_CLASS_NAME _T("ProfUIS-ShortcutListWnd")

#define __EXT_SHORTCUTLISTWND_ICON2TEXT_DX					4
#define __EXT_SHORTCUTLISTWND_ICON2TEXT_DY					4
#define __EXT_SHORTCUTLISTWND_ITEM_DX_BEFORE				4
#define __EXT_SHORTCUTLISTWND_ITEM_DY_BEFORE				4
#define __EXT_SHORTCUTLISTWND_ITEM_DX_AFTER					4
#define __EXT_SHORTCUTLISTWND_ITEM_DY_AFTER					4
#define __EXT_SHORTCUTLISTWND_BTN2BORDER_DX					5
#define __EXT_SHORTCUTLISTWND_BTN2BORDER_DY					5

#define __EXT_SHORTCUTLISTWND_DELAY_UPDATE_TIMER_ID			875
#define __EXT_SHORTCUTLISTWND_DELAY_UPDATE_TIMER_PERIOD		1
#define __EXT_SHORTCUTLISTWND_HOVER_TIMER_ID				876
#define __EXT_SHORTCUTLISTWND_HOVER_TIMER_PERIOD			20
#define __EXT_SHORTCUTLISTWND_PRESSED_TIMER_ID				877
#define __EXT_SHORTCUTLISTWND_PRESSED_TIMER_PERIOD_SMOOTH	10
#define __EXT_SHORTCUTLISTWND_PRESSED_TIMER_PERIOD_NORMAL	250

#define __EXT_SHORTCUTLISTWND_SCROLL_STEP_SIZE_SMOOTH		3L

#define __EXT_SHORTCUTLISTWND_ICON_BORDER_GAP_X				2
#define __EXT_SHORTCUTLISTWND_ICON_BORDER_GAP_Y				2

#define __EXT_SHORTCUTLISTWND_ITEM_GAP_X					__EXT_SHORTCUTLISTWND_ICON_BORDER_GAP_X
#define __EXT_SHORTCUTLISTWND_ITEM_GAP_Y					__EXT_SHORTCUTLISTWND_ICON_BORDER_GAP_Y

#define __EXT_SHORTCUTLISTWND_EXPANDED_TEXT_GAP_X			2
#define __EXT_SHORTCUTLISTWND_EXPANDED_TEXT_GAP_Y			2

#define __EXT_SHORTCUTLISTWND_EXPANDED_TOTAL_GAP_H_X		(-2)
#define __EXT_SHORTCUTLISTWND_EXPANDED_TOTAL_GAP_H_Y		0
#define __EXT_SHORTCUTLISTWND_EXPANDED_TOTAL_GAP_V_X		0
#define __EXT_SHORTCUTLISTWND_EXPANDED_TOTAL_GAP_V_Y		(-2)

#define __EXT_SHORTCUTLISTWND_MIN_EDITOR_DX					10
#define __EXT_SHORTCUTLISTWND_MIN_EDITOR_DY					10

#endif // (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // __EXTSHORTCUTLISTWND_H
