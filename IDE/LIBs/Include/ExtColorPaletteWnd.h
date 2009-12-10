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

#if (!defined __EXT_COLORPALETTE_WND_H)
#define __EXT_COLORPALETTE_WND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined __EXT_MFC_NO_COLORPALETTE)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_SCROLLWND_H)
	#include <ExtScrollWnd.h>
#endif 

/////////////////////////////////////////////////////////////////////////////
// CExtColorPaletteWnd window

// show active color area
#define __ECPWS_SHOW_ACTIVE_COLOR_AREA	0x00000001L
// show two colors in the active color area 
#define __ECPWS_SHOW_TWO_COLORS			0x00000002L
// ensure all colors are unique
#define __ECPWS_UNIQUE_COLORS			0x00000004L
// no scrolling
#define __ECPWS_NO_VSCROLLING			0x00000008L
// show tooltips
#define __ECPWS_TOOLTIPS				0x00000010L
// request focus on mouse clicks
#define __ECPWS_REQUEST_FOCUS			0x00000020L
// center palette cells horizontally
#define __ECPWS_HCENTER					0x00000040L

#define __ECPWS_USER_STYLE_24			0x00000080L
#define __ECPWS_USER_STYLE_23			0x00000100L
#define __ECPWS_USER_STYLE_22			0x00000200L
#define __ECPWS_USER_STYLE_21			0x00000400L
#define __ECPWS_USER_STYLE_20			0x00000800L
#define __ECPWS_USER_STYLE_19			0x00001000L
#define __ECPWS_USER_STYLE_18			0x00002000L
#define __ECPWS_USER_STYLE_17			0x00004000L
#define __ECPWS_USER_STYLE_16			0x00008000L
#define __ECPWS_USER_STYLE_15			0x00010000L
#define __ECPWS_USER_STYLE_14			0x00020000L
#define __ECPWS_USER_STYLE_13			0x00040000L
#define __ECPWS_USER_STYLE_12			0x00080000L
#define __ECPWS_USER_STYLE_11			0x00100000L
#define __ECPWS_USER_STYLE_10			0x00200000L
#define __ECPWS_USER_STYLE_09			0x00400000L
#define __ECPWS_USER_STYLE_08			0x00800000L
#define __ECPWS_USER_STYLE_07			0x01000000L
#define __ECPWS_USER_STYLE_06			0x02000000L
#define __ECPWS_USER_STYLE_05			0x04000000L
#define __ECPWS_USER_STYLE_04			0x08000000L
#define __ECPWS_USER_STYLE_03			0x10000000L
#define __ECPWS_USER_STYLE_02			0x20000000L
#define __ECPWS_USER_STYLE_01			0x40000000L
#define __ECPWS_USER_STYLE_00			0x80000000L

#define __ECPWS_DEFAULT_STYLES \
	( __ECPWS_UNIQUE_COLORS \
	| __ECPWS_SHOW_ACTIVE_COLOR_AREA \
	| __ECPWS_SHOW_TWO_COLORS \
	| __ECPWS_TOOLTIPS \
	| __ECPWS_HCENTER \
	)

#define __ECPW_CELL_INDEX_INVALID (-1)
#define __ECPW_CELL_INDEX_FOREGROUND (-2)
#define __ECPW_CELL_INDEX_BACKGROUND (-3)

class __PROF_UIS_API CExtColorPaletteWnd : public CExtScrollWnd
{
// Construction
public:
	DECLARE_DYNCREATE( CExtColorPaletteWnd );
	CExtColorPaletteWnd();

// Attributes
public:
	typedef
		CArray < COLORREF, COLORREF >
		CColorArray;

protected:
	static bool g_bColorPaletteWndClassRegistered;
	DWORD m_dwColorPaletteWndStyle;
	INT m_nColumnCountMin, m_nColumnCountMax;
	INT m_nTrackItem;
	INT m_nTrackMouseFlags;
	COLORREF m_clrForeground, m_clrBackground;

	CColorArray m_arrColors;
	CToolTipCtrl m_wndToolTip;

	bool m_bPresubclassDialogMode:1;

// Operations
public:
	DWORD CpwGetStyle() const;
	DWORD CpwModifyStyle(
		DWORD dwStyleRemove,
		DWORD dwStyleAdd,
		bool bRepaint = true
		);

	COLORREF CpwForegroundColorGet() const;
	COLORREF CpwForegroundColorSet(
		COLORREF clr,
		bool bRepaint = true
		);
	COLORREF CpwBackgroundColorGet() const;
	COLORREF CpwBackgroundColorSet(
		COLORREF clr,
		bool bRepaint = true
		);

	CColorArray & CpwColorArrayGet();
	const CColorArray & CpwColorArrayGet() const;
	INT CpwColorArrayGetCount() const;
	COLORREF CpwColorArrayGetAt( INT nPos ) const;
	INT CpwColorGetIndexOf( COLORREF clr ) const;
	INT CpwHitTest(
		POINT ptClient,
		LPRECT pRcItem = NULL,
		COLORREF * pColor = NULL
		) const;
	void CpwColorArraySet(
		const CColorArray & arrColors,
		bool bReplaceExisting = true, // false - append to existing
		bool bRedraw = true
		);
	COLORREF CpwColorArraySetAt(
		INT nPos,
		COLORREF clrNew,
		bool bRedraw = true
		);
	
	void CpwEnsureUniqueColorValues(
		bool bRedraw = true
		);

	void CpwRedrawWnd();

	INT CpwColumnCountMinGet() const;
	INT CpwColumnCountMinSet(
		INT nCount = 1,
		bool bRedraw = true
		);
	INT CpwColumnCountMaxGet() const;
	INT CpwColumnCountMaxSet(
		INT nCount = 32767,
		bool bRedraw = true
		);

// Overrides
public:
	virtual void OnCpwMeasureSizes(
		CSize & _sizeItem,
		CSize & _sizeBetweenItems,
		CSize & _sizeOuterDistance
		) const;
	virtual void OnCpwCalcItemRect(
		INT nPos,
		CRect & rcItem,
		INT nColumnCount,
		const CSize & _sizeItem,
		const CSize & _sizeBetweenItems,
		const CSize & _sizeOuterDistance
		) const;
	virtual void OnCpwCalcActiveColorRects(
		CRect & rcAreaAll,
		CRect & rcItemForeground,
		CRect & rcItemBackground
		) const;
	virtual INT OnCpwCalcColumnCount(
		INT nMin,
		INT nMax
		) const;
	virtual INT OnCpwCalcTotalHeight() const;
	virtual bool OnCpwCalcVisibleRowRange(
		INT & nStartRow,
		INT & nEndRow,
		bool & bActiveColorAreaVisible
		) const;
	virtual bool OnCpwCalcRowRange(
		INT nRow,
		INT & nStartItem,
		INT & nEndItem
		) const;

	virtual CExtSafeString OnCpwGetTooltipText(
		INT nItem,
		CPoint ptClient,
		const CRect & rcItem,
		COLORREF clr
		) const;

	virtual bool OnCpwItemClicked(
		INT nItem,
		UINT nMouseFlags,
		bool bMouseButtonPressed,
		CPoint point,
		const CRect & rcItem,
		COLORREF clr
		);
	virtual bool OnCpwActiveColorChange(
		bool bForeground, // true - foreground color, false - background color
		COLORREF clrOld,
		COLORREF clrNew,
		bool bRepaint = true
		);
	
	virtual void OnCpwPaintItem(
		CDC & dc,
		INT nItemID,
		const CRect & rcItem,
		COLORREF clrItem,
		COLORREF clrBorder
		);

	virtual int OnQueryMaxTipWidth( 
		__EXT_MFC_SAFE_LPCTSTR lpszText 
		);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtColorPaletteWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtColorPaletteWnd();

	bool Create(
		CWnd * pWndParent,
		DWORD dwColorPaletteWndStyle = __ECPWS_DEFAULT_STYLES,
		DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,
		DWORD dwExStyle = 0L,
		UINT nDlgCtrlID = ((UINT)(__EXT_MFC_IDC_STATIC)),
		__EXT_MFC_SAFE_LPCTSTR strWindowText = NULL,
		CRect rc = CRect( 0, 0, 0, 0 )
		);

	static bool RegisterColorPaletteWndClass();
	static HCURSOR g_hCursor;
	CExtScrollBar m_wndScrollBarV;

protected:
	bool m_bDirectCreateCall:1;
	virtual bool _CreateHelper();
	virtual void _RequestFocus();
	virtual void _RedrawWnd();
	virtual void _CancelActions();

public:
	virtual CSize OnSwGetTotalSize() const;
	virtual CSize OnSwGetPageSize( int nDirection ) const;
	virtual CSize OnSwGetLineSize( int nDirection ) const;
	virtual void OnSwPaint( CDC & dc );
	virtual bool OnSwDoMouseWheel(
		UINT fFlags,
		short zDelta,
		CPoint point
		);

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtColorPaletteWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtColorPaletteWnd

#define __EXT_COLOR_PALETTE_WND_CLASS_NAME _T("ProfUIS-ColorPaletteWindow")

#endif // (!defined __EXT_MFC_NO_COLORPALETTE)

/////////////////////////////////////////////////////////////////////////////


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __EXT_COLORPALETTE_WND_H
