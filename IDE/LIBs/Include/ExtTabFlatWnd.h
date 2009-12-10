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

#if (! defined __EXT_TAB_FLAT_WND_H)
#define __EXT_TAB_FLAT_WND_H

#if (!defined __EXT_MFC_NO_TABFLAT_CTRL)

#if (!defined __EXT_TABWND_H)
	#include <ExtTabWnd.h>
#endif

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CExtTabFlatWnd window

class __PROF_UIS_API CExtTabFlatWnd : public CExtTabWnd
{
// Construction
public:
	DECLARE_DYNCREATE( CExtTabFlatWnd );
	CExtTabFlatWnd();
	virtual bool _IsCustomLayoutTabWnd() const;

// Attributes
public:

	bool ItemsHasInclineGet( bool bBefore ) const;
	bool ItemsHasInclineBeforeGet() const;
	bool ItemsHasInclineAfterGet() const;
	void ItemsHasInclineSet( bool bBefore, bool bSet = true );
	void ItemsHasInclineBeforeSet( bool bSet = true );
	void ItemsHasInclineAfterSet( bool bSet = true );

	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTabFlatWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtTabFlatWnd();

protected:
	virtual int _CalcRgnShift( bool bHorz, const CRect & rc );
	
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
	static void stat_OnTabWndDrawItemFlatImpl(
		CExtTabFlatWnd * pWndTabs,
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
		COLORREF clrForceTabBk = COLORREF(-1L)
		);
	
	void OnTabWndMeasureItemAreaMargins(
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
	virtual void OnTabWndDrawEntire(
		CDC & dc,
		CRect & rcClient
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
	
public:
	virtual void OnFlatTabWndGetItemColors(
		LONG nItemIndex,
		bool bSelected,
		bool bHover,
		bool bEnabled,
		COLORREF &clrLight,
		COLORREF &clrShadow,
		COLORREF &clrDkShadow, 
		COLORREF &clrTabBk, 
		COLORREF &clrText
		);
	virtual void OnFlatTabWndGetMarginColors( COLORREF & clrMargin );

protected:
	virtual void OnFlatTabWndQueryItemInclines(
		LONG nItemIndex,
		bool bSelected,
		bool * p_bItemHasInclineBefore,
		bool * p_bItemHasInclineAfter
		);

	virtual int OnFlatTabWndGetSize( bool bHorz );
	
	// Generated message map functions
	//{{AFX_MSG(CExtTabFlatWnd)
	//}}AFX_MSG
	afx_msg LRESULT OnSizeParent( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
}; // class CExtTabFlatWnd

#define __EXTTAB_FLAT_GAP_X 2
#define __EXTTAB_FLAT_GAP_Y 2

#if (!defined __EXT_MFC_NO_TABMDI_CTRL )

/////////////////////////////////////////////////////////////////////////////
// CExtTabMdiFlatWnd window

class __PROF_UIS_API CExtTabMdiFlatWnd : public CExtTMWI < CExtTabFlatWnd >
{
public:
	DECLARE_DYNCREATE( CExtTabMdiFlatWnd );
	CExtTabMdiFlatWnd();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTabMdiFlatWnd)
	protected:
	//}}AFX_VIRTUAL
	// Implementation
public:
	virtual ~CExtTabMdiFlatWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
protected:
	//{{AFX_MSG(CExtTabMdiFlatWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtTabMdiFlatWnd

#if (!defined __EXT_MFC_NO_DYNAMIC_BAR_SITE)

template < >
class __PROF_UIS_API CExtTMDBS < CExtTabMdiFlatWnd >
	: public CExtTabMdiFlatWnd
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
}; // class CExtTMDBS < CExtTabMdiFlatWnd >

//#if _MFC_VER >= 0x700
//	#ifndef __EXT_PROFUIS_STATIC_LINK
//		template class __PROF_UIS_API CExtTMDBS < CExtTabMdiFlatWnd >;
//	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
//#endif // _MFC_VER >= 0x700

#endif // (!defined __EXT_MFC_NO_DYNAMIC_BAR_SITE )

#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL )

#if (!defined __EXT_MFC_NO_TABBUTTONS_CTRL)

class __PROF_UIS_API CExtTabButtonsWnd : public CExtTabFlatWnd
{
// Construction
public:
	DECLARE_DYNCREATE( CExtTabButtonsWnd );
	CExtTabButtonsWnd();

// Attributes
public:

	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTabButtonsWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtTabButtonsWnd();

protected:
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
	static void stat_OnTabWndDrawItemButtonsImpl(
		CExtTabButtonsWnd * pWndTabs,
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
		COLORREF clrForceTabBk = COLORREF(-1L)
		);

	void OnTabWndMeasureItemAreaMargins(
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
	virtual void OnTabWndDrawEntire(
		CDC & dc,
		CRect & rcClient
		);
	virtual INT OnTabWndGetParentSizingMargin(
		DWORD dwOrientation
		) const;
	virtual void OnFlatTabWndQueryItemInclines(
		LONG nItemIndex,
		bool bSelected,
		bool * p_bItemHasInclineBefore,
		bool * p_bItemHasInclineAfter
		);
	
	// Generated message map functions
	//{{AFX_MSG(CExtTabButtonsWnd)
	//}}AFX_MSG
	afx_msg LRESULT OnSizeParent( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
}; // class CExtTabButtonsWnd

#if (!defined __EXT_MFC_NO_TABMDI_CTRL )

/////////////////////////////////////////////////////////////////////////////
// CExtTabMdiButtonsWnd window

class __PROF_UIS_API CExtTabMdiButtonsWnd : public CExtTMWI < CExtTabButtonsWnd >
{
public:
	DECLARE_DYNCREATE( CExtTabMdiButtonsWnd );
	CExtTabMdiButtonsWnd();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTabMdiButtonsWnd)
	protected:
	//}}AFX_VIRTUAL
	// Implementation
public:
	virtual ~CExtTabMdiButtonsWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
protected:
	//{{AFX_MSG(CExtTabMdiButtonsWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtTabMdiButtonsWnd

#if (!defined __EXT_MFC_NO_DYNAMIC_BAR_SITE)

template < >
class __PROF_UIS_API CExtTMDBS < CExtTabMdiButtonsWnd >
	: public CExtTabMdiButtonsWnd
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
}; // class CExtTMDBS < CExtTabMdiButtonsWnd >

//#if _MFC_VER >= 0x700
//	#ifndef __EXT_PROFUIS_STATIC_LINK
//		template class __PROF_UIS_API CExtTMDBS < CExtTabMdiButtonsWnd >;
//	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
//#endif // _MFC_VER >= 0x700

#endif // (!defined __EXT_MFC_NO_DYNAMIC_BAR_SITE )

#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL )

#endif // (!defined __EXT_MFC_NO_TABBUTTONS_CTRL)

#endif // (!defined __EXT_MFC_NO_TABFLAT_CTRL)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__EXT_TAB_FLAT_WND_H)
