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

#if (!defined __EXT_TABBEDTOOLCONTROLBAR_H)
#define __EXT_TABBEDTOOLCONTROLBAR_H

#if (!defined __EXT_MFC_NO_TABBED_TOOLBAR)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_TOOLCONTROLBAR_H)
	#include <ExtToolControlBar.h>
#endif

#if (! defined __EXT_TAB_PAGE_CONTAINER_WND_H)
	#include <ExtTabPageContainerWnd.h>
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CExtTabbedToolControlBar window

class __PROF_UIS_API CExtTabbedToolControlBar : public CExtToolControlBar
{
public:
	class __PROF_UIS_API LocalNoReflectedToolBar : public CExtToolControlBar
	{
		bool m_bFakeMultirowLayout:1;
	public:
		LocalNoReflectedToolBar();
		CExtTabPageContainerWnd * GetContainer();
		CExtTabbedToolControlBar * GetContainerBar();
		virtual CSize CalcDynamicLayout(
			int nLength,
			DWORD dwMode
			);
		virtual CSize CalcFixedLayout(
			BOOL bStretch,
			BOOL bHorz
			);
	protected:
		virtual CSize _CalcLayout(
			DWORD dwMode,
			int nLength = -1
			);
		virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
		virtual bool OnQueryMultiRowLayout() const;
		virtual void _RecalcPositionsImpl();
		virtual void _RecalcLayoutImpl();
		virtual void DoPaint( CDC * pDC );
		virtual void DoEraseBk( CDC * pDC );
	}; // class LocalNoReflectedToolBar
	class LocalTabWnd;
	class __PROF_UIS_API LocalTabPageContainerWnd : public CExtTabPageContainerWnd
	{
	public:
		virtual CExtTabWnd * OnTabWndGetTabImpl();
	}; // class LocalTabPageContainerWnd 
protected:
	CExtTabPageContainerWnd * m_pWndTabPageContainer;
	bool m_bChangingDockState:1;
	DWORD
		m_dwTabsOrientationAtLeft,
		m_dwTabsOrientationAtRight,
		m_dwTabsOrientationAtTop,
		m_dwTabsOrientationAtBottom,
		m_dwTabsOrientationFloating;
public:
	INT m_nFloatingWidth; // width in floating state
	DECLARE_DYNCREATE( CExtTabbedToolControlBar );
	CExtTabbedToolControlBar();
	virtual ~CExtTabbedToolControlBar();
	virtual CExtTabPageContainerWnd * GetTabPageContainer();
	const CExtTabPageContainerWnd * GetTabPageContainer() const;
	DWORD TabsOrientationAtLeftGet();
	void TabsOrientationAtLeftSet( DWORD dwOrientation );
	DWORD TabsOrientationAtRightGet();
	void TabsOrientationAtRightSet( DWORD dwOrientation );
	DWORD TabsOrientationAtTopGet();
	void TabsOrientationAtTopSet( DWORD dwOrientation );
	DWORD TabsOrientationAtBottomGet();
	void TabsOrientationAtBottomSet( DWORD dwOrientation );
	DWORD TabsOrientationFloatingGet();
	void TabsOrientationFloatingSet( DWORD dwOrientation );
	void TabsOrientationSetDockedOuter();
	void TabsOrientationSetDockedInner();
	void TabsOrientationSetDockedLeftTop();
	void TabsOrientationSetDockedRightBottom();
	CExtToolControlBar * BarInsert(
		__EXT_MFC_SAFE_LPCTSTR strCaption,
		CWnd * pToolBarOwnerWnd = NULL,
		INT nTabIndex = -1,
		HICON hIcon = NULL,
		bool bCopyIcon = true,
		bool bSelect = false
		);
	CExtToolControlBar * BarInsert(
		__EXT_MFC_SAFE_LPCTSTR strCaption,
		const CExtCmdIcon & icon,
		CWnd * pToolBarOwnerWnd = NULL,
		INT nTabIndex = -1,
		bool bSelect = false
		);
	INT BarGetCount() const;
	CExtToolControlBar * BarGetAt( INT nTabIndex );
	const CExtToolControlBar * BarGetAt( INT nTabIndex ) const;
	INT BarGetIndexOf(
		const CExtToolControlBar * pToolBar
		) const;
	INT BarRemove(
		INT nTabIndex,
		INT nCountToRemove = 1,
		bool bRecalcLayout = true
		);
	INT BarRemoveAll(
		bool bRecalcLayout = true
		);
protected:
	virtual void OnBarInserted( INT nTabIndex );
	virtual bool OnBarRemoving( INT nTabIndex );
	virtual void OnBarRemoved( INT nTabIndex );
public:
	virtual bool OnBarEraseBk(
		CDC & dc,
		CExtToolControlBar * pToolBar
		);
protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	virtual CExtTabPageContainerWnd * OnTabPageContainerCreateObject();
	virtual CExtTabPageContainerWnd * OnTabPageContainerCreate();
	virtual CExtToolControlBar * OnBarCreateObject();
	virtual CExtToolControlBar * OnBarCreate(
		__EXT_MFC_SAFE_LPCTSTR strCaption,
		CWnd * pToolBarOwnerWnd
		);
public:
	virtual CRect OnGetTabPageContainerAreaMargins() const;
	virtual bool _GetFullRowMode() const;
	virtual bool _AffixmentIsAlignedHW() const;
	virtual bool OnQueryMultiRowLayout() const;
	virtual CSize CalcDynamicLayout(
		int nLength,
		DWORD dwMode
		);
	virtual CSize CalcFixedLayout(
		BOOL bStretch,
		BOOL bHorz
		);
	virtual CSize GetButtonAdjustment(
		CExtToolControlBar * pBar
		) const;
protected:
    virtual void _DraggingUpdateState(
		const CPoint & point,
		bool bForceFloatMode
		);
	virtual void DoEraseBk( CDC * pDC );
	virtual void OnPaintBackground(
		CDC & dc,
		const CRect & rcClient,
		const CRect & rcTabPageContainer
		);
	virtual DWORD OnQueryTabOrientation(
		UINT nDockBarDlgCtrlID
		);
	virtual void OnAdjustTabbedLayout(
		UINT nDockBarDlgCtrlID
		);
	virtual int OnCalcMaxToolBarWidth();
	virtual int OnCalcMaxToolBarHeight();
	virtual CSize OnCalcTabPageContainerSize(
		UINT nDockBarDlgCtrlID
		);
	virtual DWORD OnGetDockBarDlgCtrlID();
	virtual CSize _CalcLayout(
		DWORD dwMode,
		int nLength = -1
		);
public:
	virtual void OnUpdateCmdUI(
		CFrameWnd * pTarget,
		BOOL bDisableIfNoHndler
		);
}; // class CExtTabbedToolControlBar

#if (!defined __EXT_MFC_NO_TABBED_TOOLBAR_FLAT)

/////////////////////////////////////////////////////////////////////////////
// CExtTabbedToolControlBarFlat window

class __PROF_UIS_API CExtTabbedToolControlBarFlat : public CExtTabbedToolControlBar
{
public:
	class __PROF_UIS_API LocalToolBar : public CExtTabbedToolControlBar::LocalNoReflectedToolBar
	{
	public:
		virtual COLORREF OnQueryCustomAccentEffectForIcon(
			CDC & dc,
			CExtBarButton * pTBB
			);
	}; // class LocalToolBar
	class LocalTabWnd;
	class __PROF_UIS_API LocalTabPageContainerWnd : public CExtTabPageContainerFlatWnd
	{
	public:
		virtual CExtTabWnd * OnTabWndGetTabImpl();
	}; // class LocalTabPageContainerWnd
public:
	DECLARE_DYNCREATE( CExtTabbedToolControlBarFlat );
	CExtTabbedToolControlBarFlat();
	virtual ~CExtTabbedToolControlBarFlat();
	virtual CExtTabPageContainerWnd * OnTabPageContainerCreateObject();
	CExtTabPageContainerFlatWnd * GetFlatTabPageContainer();
	const CExtTabPageContainerFlatWnd * GetFlatTabPageContainer() const;
	virtual bool OnBarEraseBk(
		CDC & dc,
		CExtToolControlBar * pToolBar
		);
	virtual CSize GetButtonAdjustment(
		CExtToolControlBar * pBar
		) const;
protected:
	virtual CExtToolControlBar * OnBarCreateObject();
	virtual void OnPaintBackground(
		CDC & dc,
		const CRect & rcClient,
		const CRect & rcTabPageContainer
		);
}; // class CExtTabbedToolControlBarFlat

#endif // __EXT_MFC_NO_TABBED_TOOLBAR_FLAT

#if (!defined __EXT_MFC_NO_TABBED_TOOLBAR_BUTTONS)

/////////////////////////////////////////////////////////////////////////////
// CExtTabbedToolControlBarButtons window

class __PROF_UIS_API CExtTabbedToolControlBarButtons : public CExtTabbedToolControlBar
{
public:
	class __PROF_UIS_API LocalToolBar : public CExtTabbedToolControlBar::LocalNoReflectedToolBar
	{
	public:
		virtual COLORREF OnQueryCustomAccentEffectForIcon(
			CDC & dc,
			CExtBarButton * pTBB
			);
	}; // class LocalToolBar
	class LocalTabWnd;
	class __PROF_UIS_API LocalTabPageContainerWnd : public CExtTabPageContainerButtonsWnd
	{
	public:
		virtual CExtTabWnd * OnTabWndGetTabImpl();
	}; // class LocalTabPageContainerWnd
public:
	DECLARE_DYNCREATE( CExtTabbedToolControlBarButtons );
	CExtTabbedToolControlBarButtons();
	virtual ~CExtTabbedToolControlBarButtons();
	virtual CExtTabPageContainerWnd * OnTabPageContainerCreateObject();
	CExtTabPageContainerButtonsWnd * GetButtonsTabPageContainer();
	const CExtTabPageContainerButtonsWnd * GetButtonsTabPageContainer() const;
	virtual bool OnBarEraseBk(
		CDC & dc,
		CExtToolControlBar * pToolBar
		);
	virtual CSize GetButtonAdjustment(
		CExtToolControlBar * pBar
		) const;
protected:
	virtual CExtToolControlBar * OnBarCreateObject();
	virtual void OnPaintBackground(
		CDC & dc,
		const CRect & rcClient,
		const CRect & rcTabPageContainer
		);
}; // class CExtTabbedToolControlBarButtons

#endif // __EXT_MFC_NO_TABBED_TOOLBAR_BUTTONS

#if (!defined __EXT_MFC_NO_TABBED_TOOLBAR_WHIDBEY)

/////////////////////////////////////////////////////////////////////////////
// CExtTabbedToolControlBarWhidbey window

class __PROF_UIS_API CExtTabbedToolControlBarWhidbey : public CExtTabbedToolControlBar
{
public:
	class __PROF_UIS_API LocalToolBar : public CExtTabbedToolControlBar::LocalNoReflectedToolBar
	{
	public:
		virtual COLORREF OnQueryCustomAccentEffectForIcon(
			CDC & dc,
			CExtBarButton * pTBB
			);
	}; // class LocalToolBar
	class LocalTabWnd;
	class __PROF_UIS_API LocalTabPageContainerWnd : public CExtTabPageContainerWhidbeyWnd
	{
	public:
		virtual CExtTabWnd * OnTabWndGetTabImpl();
	}; // class LocalTabPageContainerWnd
public:
	DECLARE_DYNCREATE( CExtTabbedToolControlBarWhidbey );
	CExtTabbedToolControlBarWhidbey();
	virtual ~CExtTabbedToolControlBarWhidbey();
	virtual CExtTabPageContainerWnd * OnTabPageContainerCreateObject();
	CExtTabPageContainerWhidbeyWnd * GetWhidbeyTabPageContainer();
	const CExtTabPageContainerWhidbeyWnd * GetWhidbeyTabPageContainer() const;
	virtual bool OnBarEraseBk(
		CDC & dc,
		CExtToolControlBar * pToolBar
		);
	virtual CSize GetButtonAdjustment(
		CExtToolControlBar * pBar
		) const;
protected:
	virtual CExtToolControlBar * OnBarCreateObject();
	virtual void OnPaintBackground(
		CDC & dc,
		const CRect & rcClient,
		const CRect & rcTabPageContainer
		);
}; // class CExtTabbedToolControlBarWhidbey

#endif // __EXT_MFC_NO_TABBED_TOOLBAR_WHIDBEY

#if (!defined __EXT_MFC_NO_TABBED_TOOLBAR_ONENOTE)

/////////////////////////////////////////////////////////////////////////////
// CExtTabbedToolControlBarOneNote window

class __PROF_UIS_API CExtTabbedToolControlBarOneNote : public CExtTabbedToolControlBar
{
public:
	class __PROF_UIS_API LocalToolBar : public CExtTabbedToolControlBar::LocalNoReflectedToolBar
	{
	public:
		virtual COLORREF OnQueryCustomAccentEffectForIcon(
			CDC & dc,
			CExtBarButton * pTBB
			);
	}; // class LocalToolBar
	class LocalTabWnd;
	class __PROF_UIS_API LocalTabPageContainerWnd : public CExtTabPageContainerOneNoteWnd
	{
	public:
		virtual CExtTabWnd * OnTabWndGetTabImpl();
	}; // class LocalTabPageContainerWnd
public:
	DECLARE_DYNCREATE( CExtTabbedToolControlBarOneNote );
	CExtTabbedToolControlBarOneNote();
	virtual ~CExtTabbedToolControlBarOneNote();
	virtual CExtTabPageContainerWnd * OnTabPageContainerCreateObject();
	CExtTabPageContainerOneNoteWnd * GetOneNoteTabPageContainer();
	const CExtTabPageContainerOneNoteWnd * GetOneNoteTabPageContainer() const;
	virtual bool OnBarEraseBk(
		CDC & dc,
		CExtToolControlBar * pToolBar
		);
	virtual CSize GetButtonAdjustment(
		CExtToolControlBar * pBar
		) const;
protected:
	virtual CExtToolControlBar * OnBarCreateObject();
	virtual void OnPaintBackground(
		CDC & dc,
		const CRect & rcClient,
		const CRect & rcTabPageContainer
		);
}; // class CExtTabbedToolControlBarOneNote

#endif // __EXT_MFC_NO_TABBED_TOOLBAR_ONENOTE

#endif // __EXT_MFC_NO_TABBED_TOOLBAR

#endif /// __EXT_TABBEDTOOLCONTROLBAR_H

