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

#if (!defined __EXT_GRIDWND_H)
#define __EXT_GRIDWND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_SCROLLWND_H)
	#include <ExtScrollWnd.h>
#endif 

#if !defined( _OLEAUTO_H_ )
	#include <OleAuto.h>
#endif

#if (!defined __EXT_EXTCONTENTEXPANDWND_H)
	#include <ExtContentExpandWnd.h>
#endif

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

#if (!defined __EXT_DURATIONWND_H)
	#include <ExtDurationWnd.h>
#endif

#if (!defined __EXT_EDIT_H)
	#include <ExtEdit.h>
#endif

#if (!defined __EXT_SLIDER_H)
	#include <ExtSliderWnd.h>
#endif

#if (!defined __EXT_MFC_NO_GRIDBASEWND)

/////////////////////////////////////////////////////////////////////////////
// CExtGridHitTestInfo

// hit-testing area detection
// (CExtGridHitTestInfo::m_dwAreaFlags flags)
#define __EGBWA_NOWHERE							0x00000000L
#define __EGBWA_OUTER_TOP						0x00000001L
#define __EGBWA_OUTER_BOTTOM					0x00000002L
#define __EGBWA_OUTER_V							(__EGBWA_OUTER_TOP|__EGBWA_OUTER_BOTTOM)
#define __EGBWA_OUTER_LEFT						0x00000004L
#define __EGBWA_OUTER_RIGHT						0x00000008L
#define __EGBWA_OUTER_H							(__EGBWA_OUTER_LEFT|__EGBWA_OUTER_RIGHT)
#define __EGBWA_OUTER_SIDE_MASK					(__EGBWA_OUTER_H|__EGBWA_OUTER_V)
#define __EGBWA_NEAR_CELL_BORDER_TOP			0x00000010L
#define __EGBWA_NEAR_CELL_BORDER_BOTTOM			0x00000020L
#define __EGBWA_NEAR_CELL_BORDER_V				(__EGBWA_NEAR_CELL_BORDER_TOP|__EGBWA_NEAR_CELL_BORDER_BOTTOM)
#define __EGBWA_NEAR_CELL_BORDER_LEFT			0x00000040L
#define __EGBWA_NEAR_CELL_BORDER_RIGHT			0x00000080L
#define __EGBWA_NEAR_CELL_BORDER_H				(__EGBWA_NEAR_CELL_BORDER_LEFT|__EGBWA_NEAR_CELL_BORDER_RIGHT)
#define __EGBWA_NEAR_CELL_BORDER_SIDE_MASK		(__EGBWA_NEAR_CELL_BORDER_H|__EGBWA_NEAR_CELL_BORDER_V)
#define __EGBWA_OUTER_CELLS						0x00000100L
#define __EGBWA_INNER_CELLS						0x00000200L
#define __EGBWA_FROZEN_AREA_TOP					0x00000400L
#define __EGBWA_FROZEN_AREA_BOTTOM				0x00000800L
#define __EGBWA_FROZEN_AREA_LEFT				0x00001000L
#define __EGBWA_FROZEN_AREA_RIGHT				0x00002000L
#define __EGBWA_FROZEN_AREA_ANY					(__EGBWA_FROZEN_AREA_TOP|__EGBWA_FROZEN_AREA_BOTTOM|__EGBWA_FROZEN_AREA_LEFT|__EGBWA_FROZEN_AREA_RIGHT)
// the CExtGridCell::OnHitTestInfoAdjust() method may fill
// m_nButtonType, m_rcPart and add following flags:
#define __EGBWA_CELL_CHECKBOX					0x00004000L
#define __EGBWA_CELL_IMAGE						0x00008000L
#define __EGBWA_CELL_BUTTON						0x00010000L
#define __EGBWA_CELL_TEXT						0x00020000L
#define __EGBWA_CELL_TEXT_AREA					0x00040000L
#define __EGBWA_CELL_SORT_ARROW					0x00080000L
#define __EGBWA_CELL_FOCUS_ARROW				0x00100000L
#define __EGBWA_CELL_MASK \
	(__EGBWA_CELL_CHECKBOX \
	|__EGBWA_CELL_IMAGE \
	|__EGBWA_CELL_BUTTON \
	|__EGBWA_CELL_TEXT \
	|__EGBWA_CELL_TEXT_AREA \
	|__EGBWA_CELL_SORT_ARROW \
	|__EGBWA_CELL_FOCUS_ARROW \
	)

// drop-after state
#define __EGBWA_OUTER_DROP_AFTER				0x00200000L

// advanced
#define __EGBWA_TREE_BOX						0x00400000L
#define __EGBWA_TREE_BOX_IS_EXPANDED			0x00800000L
#define __EGBWA_TREE_OUTLINE_AREA				0x01000000L

class __PROF_UIS_API CExtGridHitTestInfo
{
public:
	CPoint m_ptClient;
	LONG m_nRowNo, m_nColNo, m_nVisibleRowNo, m_nVisibleColNo;
	DWORD m_dwAreaFlags;
	CRect m_rcItem, m_rcExtra, m_rcPart;
	INT m_nButtonType;
	CExtGridHitTestInfo(
		CPoint ptClient = CPoint( -1, -1 )
		);
	CExtGridHitTestInfo(
		const CExtGridHitTestInfo & htInfo
		);
	CExtGridHitTestInfo & operator = (
		const CExtGridHitTestInfo & htInfo
		);
	void Empty();
	bool IsHoverEmpty() const;
	bool IsHoverInner() const;
	bool IsHoverOuter(
		bool * p_bOuterCellAtLeftRight = NULL
		) const;
	static bool IsHoverEqual(
		const CExtGridHitTestInfo & htInfoLeft,
		const CExtGridHitTestInfo & htInfoRight,
		bool bCompareRects = true,
		bool bValidtateRects = true,
		bool bCompareOuterDropAfterState = true
		);
	bool IsHoverEqual(
		const CExtGridHitTestInfo & htInfo,
		bool bCompareRects = true,
		bool bValidtateRects = true,
		bool bCompareOuterDropAfterState = true
		) const;
	bool IsValidRect(
		bool bEmptyIsInvalid = true
		) const;
	INT GetInnerOuterTypeOfColumn() const;
	INT GetInnerOuterTypeOfRow() const;
	static INT GetInnerOuterTypeOfColumn( DWORD dwAreaFlags );
	static INT GetInnerOuterTypeOfRow( DWORD dwAreaFlags );
	static DWORD CellTypesToAreaFlags(
		INT nColType,
		INT nRowType
		);
protected:
	void _AssignFromOther(
		const CExtGridHitTestInfo & htInfo
		);
}; // class CExtGridHitTestInfo

/////////////////////////////////////////////////////////////////////////////
// CExtGridBaseWnd

// extended/behavior styles provided with
// CExtGridBaseWnd::SiwGetStyleEx()
// ( these styles are not intersected with
//   CExtScrollItemWnd::SiwGetStyle() style values )

// allow inner/outer cell hover processing on WM_MOUSEMOVE
#define __EGBS_EX_HVI_EVENT_CELLS				0x00000001L
#define __EGBS_EX_HVO_EVENT_CELLS				0x00000002L
// highlight inner/outer cells in the hovered row
// ( __EGBS_EX_HVI_HIGHLIGHT_ROWS requires __EGBS_EX_HVI_EVENT_CELLS|__EGBS_EX_HVI_HIGHLIGHT_CELL,
//   __EGBS_EX_HVO_HIGHLIGHT_ROWS requires __EGBS_EX_HVO_EVENT_CELLS|__EGBS_EX_HVO_HIGHLIGHT_CELL )
#define __EGBS_EX_HVI_HIGHLIGHT_ROWS			0x00000004L
#define __EGBS_EX_HVO_HIGHLIGHT_ROWS			0x00000008L
// highlight inner/outer cells in the hovered column
// ( __EGBS_EX_HVI_HIGHLIGHT_COLUMNS requires __EGBS_EX_HVI_EVENT_CELLS|__EGBS_EX_HVI_HIGHLIGHT_CELL,
//   __EGBS_EX_HVO_HIGHLIGHT_COLUMNS requires __EGBS_EX_HVO_EVENT_CELLS|__EGBS_EX_HVO_HIGHLIGHT_CELL )
#define __EGBS_EX_HVI_HIGHLIGHT_COLUMNS			0x00000010L
#define __EGBS_EX_HVO_HIGHLIGHT_COLUMNS			0x00000020L
// highlight cell under hover
// ( __EGBS_EX_HVI_HIGHLIGHT_CELL requires __EGBS_EX_HVI_EVENT_CELLS,
//   __EGBS_EX_HVO_HIGHLIGHT_CELL requires __EGBS_EX_HVO_EVENT_CELLS )
#define __EGBS_EX_HVI_HIGHLIGHT_CELL			0x00000040L
#define __EGBS_EX_HVO_HIGHLIGHT_CELL			0x00000080L

// highlight inner/outer cells matching with the selection
// in the same row/column
#define __EGBS_EX_SI_HIGHLIGHT_COLUMNS			0x00000100L
#define __EGBS_EX_SO_HIGHLIGHT_COLUMNS			0x00000200L
#define __EGBS_EX_SI_HIGHLIGHT_ROWS				0x00000400L
#define __EGBS_EX_SO_HIGHLIGHT_ROWS				0x00000800L

// highlight inner/outer cells matching with the focus
// in the same row/column
#define __EGBS_EX_FI_HIGHLIGHT_COLUMNS			0x00001000L
#define __EGBS_EX_FO_HIGHLIGHT_COLUMNS			0x00002000L
#define __EGBS_EX_FI_HIGHLIGHT_ROWS				0x00004000L
#define __EGBS_EX_FO_HIGHLIGHT_ROWS				0x00008000L

// tooltip/expanding for inner/outer cells
// (if expanding is enabled - toolips are not used)
#define __EGBS_EX_CELL_TOOLTIPS_OUTER_L			0x00010000L
#define __EGBS_EX_CELL_TOOLTIPS_OUTER_R			0x00020000L
#define __EGBS_EX_CELL_TOOLTIPS_OUTER_T			0x00040000L
#define __EGBS_EX_CELL_TOOLTIPS_OUTER_B			0x00080000L
#define __EGBS_EX_CELL_TOOLTIPS_OUTER_HORZ		(__EGBS_EX_CELL_TOOLTIPS_OUTER_T|__EGBS_EX_CELL_TOOLTIPS_OUTER_B)
#define __EGBS_EX_CELL_TOOLTIPS_OUTER_VERT		(__EGBS_EX_CELL_TOOLTIPS_OUTER_L|__EGBS_EX_CELL_TOOLTIPS_OUTER_R)
#define __EGBS_EX_CELL_TOOLTIPS_OUTER			(__EGBS_EX_CELL_TOOLTIPS_OUTER_HORZ|__EGBS_EX_CELL_TOOLTIPS_OUTER_VERT)
#define __EGBS_EX_CELL_EXPANDING_OUTER_L		0x00100000L
#define __EGBS_EX_CELL_EXPANDING_OUTER_R		0x00200000L
#define __EGBS_EX_CELL_EXPANDING_OUTER_T		0x00400000L
#define __EGBS_EX_CELL_EXPANDING_OUTER_B		0x00800000L
#define __EGBS_EX_CELL_EXPANDING_OUTER_HORZ		(__EGBS_EX_CELL_EXPANDING_OUTER_T|__EGBS_EX_CELL_EXPANDING_OUTER_B)
#define __EGBS_EX_CELL_EXPANDING_OUTER_VERT		(__EGBS_EX_CELL_EXPANDING_OUTER_L|__EGBS_EX_CELL_EXPANDING_OUTER_R)
#define __EGBS_EX_CELL_EXPANDING_OUTER			(__EGBS_EX_CELL_EXPANDING_OUTER_HORZ|__EGBS_EX_CELL_EXPANDING_OUTER_VERT)
#define __EGBS_EX_CELL_TOOLTIPS_INNER			0x01000000L
#define __EGBS_EX_CELL_EXPANDING_INNER			0x02000000L
#define __EGBS_EX_CELL_TOOLTIPS					(__EGBS_EX_CELL_TOOLTIPS_INNER|__EGBS_EX_CELL_TOOLTIPS_OUTER)
#define __EGBS_EX_CELL_EXPANDING				(__EGBS_EX_CELL_EXPANDING_INNER|__EGBS_EX_CELL_EXPANDING_OUTER)
#define __EGBS_EX_CELL_TE_MASK					(__EGBS_EX_CELL_TOOLTIPS|__EGBS_EX_CELL_EXPANDING)

#define __EGBS_EX_CORNER_AREAS_3D				0x04000000L
#define __EGBS_EX_CORNER_AREAS_CURVE			0x08000000L

#if (!defined __EXT_MFC_NO_GRIDWND)

// use external data from data provider for
// determining grid dimensions etc.
#define __EGWS_EX_EXTERNAL_DATA					0x10000000L
#define __EGWS_EX_USE_THEME_API					0x20000000L
#define __EGWS_EX_USER_STYLE_00					0x40000000L

#if (!defined __EGWS_EX_PM_COLORS)
	#define __EGWS_EX_PM_COLORS					0x80000000L
#endif

#endif // (!defined __EXT_MFC_NO_GRIDWND)

class CExtPopupMenuTipWnd;

class __PROF_UIS_API CExtGridBaseWnd : public CExtScrollItemWnd
{
// Construction
public:
	DECLARE_DYNCREATE( CExtGridBaseWnd );
	CExtGridBaseWnd();

// Attributes
public:
	int m_nMouseActivateCode;
	DWORD m_dwDefaultScrollTypeH, m_dwDefaultScrollTypeV;
	CRect m_rcDefaultCellBorderHtSpaces;
	HCURSOR m_hCursorDefault,
		m_hCursorResizingH1, m_hCursorResizingV1,
		m_hCursorResizingH2, m_hCursorResizingV2,
		m_hCursorOuterSelectionTop, m_hCursorOuterSelectionBottom,
		m_hCursorOuterSelectionLeft, m_hCursorOuterSelectionRight,
		m_hCursorInnerCell, m_hCursorCheckBox, m_hCursorBtnUp,
		m_hCursorBtnDown, m_hCursorBtnDropdown, m_hCursorBtnEllipsis,
		m_hCursorFocusArrow, m_hCursorSortArrow, m_hCursorIcon;

protected:
	volatile CRect m_rcFrozenRange;
	volatile LONG m_nCountOfRows, m_nCountOfColumns,
		m_nCountOfOuterRowsT, m_nCountOfOuterColumnsL,
		m_nCountOfOuterRowsB, m_nCountOfOuterColumnsR;
	CSize m_sizeCellDefault;
	CRect m_rcOuterT,m_rcOuterB,m_rcOuterL,m_rcOuterR,
		m_rcOuterLT,m_rcOuterRT,m_rcOuterLB,m_rcOuterRB;
	CRect m_rcFrozenT,m_rcFrozenB,m_rcFrozenL,m_rcFrozenR,
		m_rcFrozenLT,m_rcFrozenRT,m_rcFrozenLB,m_rcFrozenRB;
	CRect m_rcOuterFrozenTL, m_rcOuterFrozenTR, m_rcOuterFrozenBL, m_rcOuterFrozenBR,
		m_rcOuterFrozenLT, m_rcOuterFrozenLB, m_rcOuterFrozenRT, m_rcOuterFrozenRB;
	mutable CRect m_rcClient;
	CPoint m_ptFocusCell, m_ptTrackOffset;
	bool m_bProcessingCancelMode:1,
		m_bLockVirtualCacheUpdateH:1,
		m_bLockVirtualCacheUpdateV:1;
	CArray < CRect, CRect & > m_arrSelAreas;
	CExtGridHitTestInfo m_htInfoHover, m_htInfoResizing,
		m_htInfoExpand, m_htInfoCellPressing, m_htInfoDataDnd;
	UINT m_nTimerIdPopupMenuWaiting, m_nTimerIdDelayedExpand,
		m_nTimerIdExpandWait, m_nTimerIdPressedButton;
	typedef
		CMap < LONG, LONG, INT, INT >
		index2extent_map_t;
	index2extent_map_t
		m_mapExtentsLeft,
		m_mapExtentsRight,
		m_mapExtentsTop,
		m_mapExtentsBottom;
	enum e_MouseTrackType_t
	{
		__EMTT_NOTHING = 0,
		__EMTT_SEL_OUTER_ROWS = 1,
		__EMTT_SEL_OUTER_COLUMNS = 2,
		__EMTT_SEL_INNER_ROWS = 3,
		__EMTT_SEL_INNER_COLUMNS = 4,
		__EMTT_SEL_VAR_ROWS = 5,
		__EMTT_SEL_VAR_COLUMNS = 6,
		__EMTT_SEL_VAR_RANGE = 7,
		__EMTT_RESIZING_H = 8,
		__EMTT_RESIZING_V = 9,
		__EMTT_DATA_DND_STARTING = 10,
	};
	e_MouseTrackType_t m_eMTT;
	mutable INT m_nTrackExtentMin, m_nTrackExtentMax, m_nTrackExtentCurrent;
	HWND m_hWndInplaceControl;
	CPoint m_ptLastME;

// Operations
public:
	virtual DWORD SiwModifyStyle(
		DWORD dwStyleAdd,
		DWORD dwStyleRemove = 0L,
		bool bUpdateWnd = true
		);
	HWND GetSafeInplaceActiveHwnd();
	DWORD SelectionTypeGet() const;
	DWORD SelectionTypeSet(
		DWORD dwNewSelectionType
		);
	virtual CExtGridHitTestInfo & HitTest(
		CExtGridHitTestInfo & htInfo,
		bool bReAlignCellResizing,
		bool bSupressZeroTopCellResizing,
		bool bComputeOuterDropAfterState = false
		) const;
	virtual LONG OuterColumnCountLeftGet() const;
	virtual LONG OuterColumnCountRightGet() const;
	virtual LONG OuterColumnCountLeftSet(
		LONG nCount = 0L,
		bool bRedraw = true
		);
	virtual LONG OuterColumnCountRightSet(
		LONG nCount = 0L,
		bool bRedraw = true
		);
	virtual LONG OuterRowCountTopGet() const;
	virtual LONG OuterRowCountBottomGet() const;
	virtual LONG OuterRowCountTopSet(
		LONG nCount = 0L,
		bool bRedraw = true
		);
	virtual LONG OuterRowCountBottomSet(
		LONG nCount = 0L,
		bool bRedraw = true
		);
	virtual INT OuterColumnWidthGet(
		bool bLeft,
		LONG nColNo,
		bool bZeroIfNoRows = true
		) const;
	virtual INT OuterRowHeightGet(
		bool bTop,
		LONG nRowNo,
		bool bZeroIfNoColumns = true
		) const;
	virtual void OuterColumnWidthSet(
		bool bLeft,
		LONG nColNo,
		INT nWidth
		);
	virtual void OuterRowHeightSet(
		bool bTop,
		LONG nRowNo,
		INT nHeight
		);
	virtual void OuterExtentsClearAll(
		bool bLeft = true,
		bool bRight = true,
		bool bTop = true,
		bool bBottom = true
		);
	virtual LONG ColumnCountGet() const;
	virtual LONG ColumnCountSet(
		LONG nCount = -1L,
		bool bRedraw = true
		);
	virtual bool ColumnInsert(
		LONG nColNo, // -1 or greater than count - append
		LONG nColInsertCount = 1L,
		bool bRedraw = true
		);
	bool ColumnAdd(
		LONG nColInsertCount = 1L,
		bool bRedraw = true
		)
	{
		return
			ColumnInsert(
				-1,
				nColInsertCount,
				bRedraw
				);
	}
	virtual LONG ColumnRemove(
		LONG nColNo,
		LONG nColRemoveCount = 1L, // -1 - remove up to end (truncate)
		bool bRedraw = true
		);
	LONG ColumnTruncate(
		LONG nColNo,
		bool bRedraw = true
		)
	{
		return ColumnRemove( nColNo, -1L, bRedraw );
	}
	LONG ColumnRemoveAll(
		bool bRedraw = true
		)
	{
		return ColumnTruncate( 0L, bRedraw );
	}
	virtual LONG RowCountGet() const;
	virtual LONG RowCountSet(
		LONG nCount = -1L,
		bool bRedraw = true
		);
	virtual bool RowInsert(
		LONG nRowNo, // -1 or greater than count - append
		LONG nRowInsertCount = 1L,
		bool bRedraw = true
		);
	bool RowAdd(
		LONG nRowInsertCount = 1L,
		bool bRedraw = true
		)
	{
		return
			RowInsert(
				-1,
				nRowInsertCount,
				bRedraw
				);
	}
	virtual LONG RowRemove(
		LONG nRowNo,
		LONG nRowRemoveCount = 1L, // -1 - remove up to end (truncate)
		bool bRedraw = true
		);
	LONG RowTruncate(
		LONG nRowNo,
		bool bRedraw = true
		)
	{
		return RowRemove( nRowNo, -1L, bRedraw );
	}
	LONG RowRemoveAll(
		bool bRedraw = true
		)
	{
		return RowTruncate( 0L, bRedraw );
	}
	virtual INT DefaultColumnWidthGet() const;
	virtual INT DefaultColumnWidthSet(
		INT nColumnWidth,
		bool bRedraw = true
		);
	virtual INT DefaultRowHeightGet() const;
	virtual INT DefaultRowHeightSet(
		INT nRowHeight,
		bool bRedraw = true
		);
	bool FixedSizeColumnsGet() const;
	bool FixedSizeColumnsSet(
		bool bFixedSizeColumns,
		bool bRedraw = true
		);
	bool FixedSizeRowsGet() const;
	bool FixedSizeRowsSet(
		bool bFixedSizeRows,
		bool bRedraw = true
		);
	virtual bool EnsureVisibleColumn(
		LONG nColNo,
		bool bRedraw = true
		);
	virtual bool EnsureVisibleRow(
		LONG nRowNo,
		bool bRedraw = true
		);
protected:
	LONG m_nEnsureVisibleColumnLockCount, m_nEnsureVisibleRowLockCount;
public:
	virtual void EnsureVisibleColumnLock();
	virtual void EnsureVisibleColumnUnlock();
	virtual bool EnsureVisibleColumnIsLocked() const;
	virtual void EnsureVisibleRowLock();
	virtual void EnsureVisibleRowUnlock();
	virtual bool EnsureVisibleRowIsLocked() const;
	void EnsureVisibleLock();
	void EnsureVisibleUnlock();
	virtual INT SelectionGetHitCount(
		LONG nColNo,
		LONG nRowNo
		) const;
	virtual bool SelectionGetForCellPainting(
		LONG nColNo,
		LONG nRowNo
		) const;
	virtual bool SelectionGetForCell(
		LONG nColNo,
		LONG nRowNo
		) const;
	virtual INT SelectionGetAreaCount() const;
	virtual CRect SelectionGet(
		bool bNormalized,
		INT nAreaNumber // -1 - get normalized union of all areas
		) const;
	virtual bool SelectionRemoveAt(
		INT nAreaNumber,
		bool bRedraw = true
		);
	virtual bool SelectionSetAt(
		INT nAreaNumber,
		const RECT & rcNewSelection,
		bool bRedraw = true
		);
	virtual bool SelectionInsertAt(
		INT nAreaNumber,
		const RECT & rcNewSelection,
		bool bRedraw = true
		);
	virtual bool SelectionSet(
		const RECT & rcNewSelection,
		bool bReplaceOldAreas = true,
		bool bReplaceLastArea = false,
		bool bRedraw = true
		);
	bool SelectionSet(
		const POINT & ptNewSelection,
		bool bReplaceOldAreas = true,
		bool bReplaceLastArea = false,
		bool bRedraw = true
		);
	bool SelectionIsEmpty() const;
	virtual bool SelectionUnset(
		bool bSetToFocus = true,
		bool bRedraw = true
		);
	virtual LONG SelectionGetForCellNextPrev(
		bool bSearchHorz,
		bool bSearchNext,
		LONG nColNo,
		LONG nRowNo
		) const;
	LONG SelectionGetNextRowInColumn(
		LONG nColNo,
		LONG nRowNo
		) const;
	LONG SelectionGetPrevRowInColumn(
		LONG nColNo,
		LONG nRowNo
		) const;
	LONG SelectionGetNextColumnInRow(
		LONG nColNo,
		LONG nRowNo
		) const;
	LONG SelectionGetPrevColumnInRow(
		LONG nColNo,
		LONG nRowNo
		) const;
	LONG SelectionGetFirstRowInColumn(
		LONG nColNo
		) const;
	LONG SelectionGetLastRowInColumn(
		LONG nColNo
		) const;
	LONG SelectionGetFirstColumnInRow(
		LONG nRowNo
		) const;
	LONG SelectionGetLastColumnInRow(
		LONG nRowNo
		) const;
	virtual CSize OnGbwCellJoinQueryInfo(
		LONG nColNo,
		LONG nRowNo,
		INT nColType = 0,
		INT nRowType = 0
		) const;
	virtual bool OnGbwCellJoinAdjustCoordinates(
		LONG & nColNo,
		LONG & nRowNo,
		INT nColType = 0,
		INT nRowType = 0
		) const;
	virtual CPoint FocusGet() const;
	virtual CPoint FocusSet(
		const POINT & ptNewFocus,
		bool bEnsureVisibleColumn = true,
		bool bEnsureVisibleRow = true,
		bool bResetSelectionToFocus = true,
		bool bRedraw = true,
		bool * p_bCanceled = NULL
		);
	CPoint FocusUnset(
		bool bRedraw = true
		);
	bool AutoFocusBottomRightGet() const;
	bool AutoFocusBottomRightSet(
		bool bAutoFocusBottomRight
		);
	bool MultiAreaSelectionGet() const;
	bool MultiAreaSelectionSet(
		bool bMultiAreaSelection
		);
	bool NoHideSelectionGet() const;
	bool NoHideSelectionSet(
		bool bNoHideSelection,
		bool bRedraw = true
		);
	bool GridLinesHorzGet() const;
	bool GridLinesHorzSet(
		bool bGridLines,
		bool bRedraw = true
		);
	bool GridLinesVertGet() const;
	bool GridLinesVertSet(
		bool bGridLines,
		bool bRedraw = true
		);
	bool LbExtSelectionGet() const;
	bool LbExtSelectionSet( bool bLbExtSelection );
	bool SubtractSelectionAreasGet() const;
	bool SubtractSelectionAreasSet(
		bool bSubtractSelectionAreas,
		bool bRedraw = true
		);
	virtual void SelectionSplitAllAreas(
		bool bSplitByColumns,
		bool bSplitByRows
		);
	bool HoverEventsInnerGet() const;
	bool HoverEventsOuterGet() const;
	void HoverEventsSet(
		bool bHoverInnerCells = true,
		bool bHoverOuterCells = true
		);
	bool HoverHighlightCellInnerGet() const;
	bool HoverHighlightCellOuterGet() const;
	bool HoverHighlightColumnInnerGet() const;
	bool HoverHighlightColumnOuterGet() const;
	bool HoverHighlightRowInnerGet() const;
	bool HoverHighlightRowOuterGet() const;
	void HoverHighlightSet(
		bool bHighlightSingleCellsInner = true,
		bool bHighlightSingleCellsOuter = true,
		bool bHighlightColumnCellsInner = true,
		bool bHighlightColumnCellsOuter = true,
		bool bHighlightRowCellsInner = true,
		bool bHighlightRowCellsOuter = true
		);
	CExtGridHitTestInfo & HoverInfoGet();
	const CExtGridHitTestInfo & HoverInfoGet() const;

	bool SelectionHighlightColumnInnerGet() const;
	bool SelectionHighlightColumnOuterGet() const;
	bool SelectionHighlightRowInnerGet() const;
	bool SelectionHighlightRowOuterGet() const;
	void SelectionHighlightSet(
		bool bHighlightColumnCellsInner = true,
		bool bHighlightColumnCellsOuter = true,
		bool bHighlightRowCellsInner = true,
		bool bHighlightRowCellsOuter = true
		);

	bool FocusHighlightColumnInnerGet() const;
	bool FocusHighlightColumnOuterGet() const;
	bool FocusHighlightRowInnerGet() const;
	bool FocusHighlightRowOuterGet() const;
	void FocusHighlightSet(
		bool bHighlightColumnCellsInner = true,
		bool bHighlightColumnCellsOuter = true,
		bool bHighlightRowCellsInner = true,
		bool bHighlightRowCellsOuter = true
		);
	bool EditCell( // default parameters will start to edit focused cell
		LONG nColNo = -1L,
		LONG nRowNo = -1L,
		INT nColType = 0,
		INT nRowType = 0,
		bool bContinueMsgLoop = true,
		__EXT_MFC_SAFE_LPCTSTR strStartEditText = NULL
		);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtGridBaseWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtGridBaseWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

	virtual bool OnGbwCalcVisibleItemRect(
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		RECT & rcCellExtra,
		RECT & rcCell,
		LONG * p_nVisibleColNo = NULL,
		LONG * p_nVisibleRowNo = NULL
		) const;
	virtual void OnGbwWalkVisibleAreas(
		CDC & dc,
		bool bFocusedControl,
		CExtGridHitTestInfo * pHT = NULL
		) const;

	virtual void OnSiwPaintBackground(
		CDC & dc,
		bool bFocusedControl
		) const;
	virtual void OnSiwPaintForeground(
		CDC & dc,
		bool bFocusedControl
		) const;
	virtual void OnGbwPaintCell(
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual COLORREF OnGbwQueryBackColor(
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual COLORREF OnGbwQueryGridLinesColor() const;
	virtual bool OnGbwQueryCellGridLines(
		bool bHorz,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnGbwQueryEmptyAreaLinesMetrics(
		bool & bDrawEmptyAreaLinesH_Bottom,
		bool & bDrawEmptyAreaLinesV_Bottom,
		bool & bDrawEmptyAreaLinesH_Right,
		bool & bDrawEmptyAreaLinesV_Right,
		bool & bDrawEmptyAreaLinesH_Corner,
		bool & bDrawEmptyAreaLinesV_Corner,
		INT & nEmptyColumnWidth,
		INT & nEmptyRowHeight
		) const;
	virtual void OnGbwAdjustRects(
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		RECT & rcCellExtraA,
		RECT & rcCellA
		) const;
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
	virtual void OnGbwPaintExtraSpaceH(
		bool bPostPaint,
		CDC & dc,
		const RECT & rcRowExtra,
		const RECT & rcRow,
		LONG nVisibleRowNo,
		LONG nRowNo,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		bool bFocusedControl
		) const;
	virtual void OnGbwPaintExtraSpaceV(
		bool bPostPaint,
		CDC & dc,
		const RECT & rcColExtra,
		const RECT & rcCol,
		LONG nVisibleColNo,
		LONG nColNo,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		bool bFocusedControl
		) const;
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
	virtual LONG FrozenColumnCountLeftGet() const;
	virtual LONG FrozenColumnCountRightGet() const;
	virtual LONG FrozenRowCountTopGet() const;
	virtual LONG FrozenRowCountBottomGet() const;
	virtual void FrozenColumnCountLeftSet(
		LONG nFrozenCount = 0L,
		bool bRedraw = true
		);
	virtual void FrozenColumnCountRightSet(
		LONG nFrozenCount = 0L,
		bool bRedraw = true
		);
	virtual void FrozenRowCountTopSet(
		LONG nFrozenCount = 0L,
		bool bRedraw = true
		);
	virtual void FrozenRowCountBottomSet(
		LONG nFrozenCount = 0L,
		bool bRedraw = true
		);
	virtual CRect OnSiwGetFrozenRange() const;
	virtual CSize OnSiwCalcItemSize() const;
	virtual CSize OnSwGetTotalSize() const;
	virtual bool OnSwDoScrollBy(
		CSize sizeScroll,
		bool bDoScroll = true
		);
	virtual CSize OnSiwCalcPageMetrics( int nDirection ) const;
	virtual void OnGbwQueryVisibleColumnRange(
		LONG & nColFirst,
		LONG & nColLast,
		LONG & nColFirstOffset
		) const;
	virtual void OnGbwQueryVisibleRowRange(
		LONG & nRowFirst,
		LONG & nRowLast,
		LONG & nRowFirstOffset
		) const;
	virtual bool OnSwDoMouseWheel(
		UINT fFlags,
		short zDelta,
		CPoint point
		);
	virtual CRect OnSwRecalcLayout(
		bool bDoLayout,
		LPCRECT pRectClientSrc = NULL
		);
	virtual void OnGbwEraseArea(
		CDC & dc,
		const RECT & rcArea,
		DWORD dwAreaFlags
		) const;
	virtual CRect OnGbwQueryCellBorderHtSpaces(
		CExtGridHitTestInfo * pHT = NULL // if NULL - get default resizing border spaces
		) const;
	virtual LONG OnGbwQueryPixelColumnOffset(
		LONG nColNo
		);
	virtual LONG OnGbwQueryPixelRowOffset(
		LONG nRowNo
		);
	virtual bool OnGbwSetCursor(
		const CExtGridHitTestInfo & htInfo
		);
	virtual bool OnGbwCanResizeColumn(
		LONG nColNo,
		INT * p_nExtentMin = NULL,
		INT * p_nExtentMax = NULL
		);
	virtual bool OnGbwCanResizeRow(
		LONG nRowNo,
		INT * p_nExtentMin = NULL,
		INT * p_nExtentMax = NULL
		);
	virtual bool OnGbwQueryTrackingCellMenu() const;
	virtual void OnGbwHoverRemove(
		bool bAllowReleaseCapture = true
		);
	virtual bool OnGbwHoverRecalc(
		const POINT * pPtClient = NULL
		);
	virtual void OnGbwHoverCell(
		const CExtGridHitTestInfo & htInfoPrevious,
		const CExtGridHitTestInfo & htInfoCurrent,
		bool bHoverEnter,
		bool bHoverLeave
		);
	virtual bool OnGbwAnalyzeCellMouseClickEvent(
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nRepCnt, // 0 - button up, 1 - single click, 2 - double click, 3 - post single click & begin editing
		UINT nFlags, // mouse event flags
		CPoint point // mouse pointer in client coordinates
		);
	virtual bool OnGbwAnalyzeCellSysKeyEvent(
		bool bKeyDownEvent, // true - sys-key-down event, false - key-up event
		UINT nChar, // key code
		UINT nRepCnt, // sys-key-down/sys-key-up press count
		UINT nFlags // sys-key-down/sys-key-up event flags
		);
	virtual bool OnGbwAnalyzeCellKeyEvent(
		bool bKeyDownEvent, // true - key-down event, false - key-up event
		UINT nChar, // virtual key code
		UINT nRepCnt, // key-down/key-up press count
		UINT nFlags // key-down/key-up event flags
		);
	virtual bool OnGbwBeginEdit(
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		bool bContinueMsgLoop = true,
		__EXT_MFC_SAFE_LPCTSTR strStartEditText = NULL
		);
	virtual bool OnGbwSelectionCanModify();
	virtual void OnGbwSelectionChanged();
	virtual bool OnGbwFocusChanging(
		const POINT & ptOldFocus,
		const POINT & ptNewFocus,
		bool & bEnsureVisibleColumn,
		bool & bEnsureVisibleRow,
		bool & bResetSelectionToFocus,
		bool & bRedraw
		);
	virtual void OnGbwFocusChanged(
		const POINT & ptOldFocus,
		const POINT & ptNewFocus
		);
	virtual void OnGbwResizingAdjustComputedTrackExtent(
		INT & nTrackExtentToAdjust,
		INT nTrackExtentMin,
		INT nTrackExtentMax,
		const CExtGridHitTestInfo & htInfo,
		bool bHorz
		) const;
	virtual void OnGbwResizingStateUpdate(
		bool bInitial,
		bool bFinal,
		const CPoint * p_ptClient = NULL
		);
	virtual void OnGbwResizingAdjustHtInfo(
		CExtGridHitTestInfo & htInfo
		);
	virtual void OnGbwResizingStateApply(
		bool bHorz,
		LONG nItemNo,
		INT nItemExtent
		);
	virtual CRect OnSwGetClientRect() const;
	virtual void OnSwUpdateScrollBars();

	virtual CRect & _SelectionAreaConvert( CRect & rcArea ) const;
protected:
	virtual void _AddToSelArrayImpl( CRect & rcApplyNew );
	virtual void _InvalidateHoveredArea(
		const CExtGridHitTestInfo & htInfo
		);
	bool _DoSetCursor(
		const POINT & ptClient
		);
	bool _DoSetCursor();
	void _AnalyzeVirtualFocus(
		bool bRedraw = true
		);
	CPoint _AnalyzeVirtualFocus(
		const POINT & ptFocusOld
		);
	CRect _ResizingTrackerRect( INT nExtentCalc );
	void _ResizingTrackerDraw( const CRect & rcTracker );

public:
	void EnableTooltips(
		bool bTooltipsOuterTop = true,
		bool bTooltipsOuterBottom = true,
		bool bTooltipsOuterLeft = true,
		bool bTooltipsOuterRight = true,
		bool bTooltipsInner = false
		);
	void EnableExpanding(
		bool bExpandingOuterTop = false,
		bool bExpandingOuterBottom = false,
		bool bExpandingOuterLeft = false,
		bool bExpandingOuterRight = false,
		bool bExpandingInner = true
		);
protected:
	CToolTipCtrl m_wndToolTip;
	mutable CExtContentExpandWnd m_wndContentExpand;
	INT m_nSizeOfExpandShadow;

public:
	CToolTipCtrl & GetToolTipCtrl();
	const CToolTipCtrl & GetToolTipCtrl() const;
	CExtContentExpandWnd & GetContentExpandWnd();
	const CExtContentExpandWnd & GetContentExpandWnd() const;
	INT ExpandShadowSizeGet() const;
	INT ExpandShadowSizeSet( INT nSizeOfNewShadow );

	virtual bool _CreateHelper();

	INT m_nAdvancedTipStyle;
	virtual CExtPopupMenuTipWnd * OnAdvancedPopupMenuTipWndGet() const;
	virtual void OnAdvancedPopupMenuTipWndDisplay(
		CExtPopupMenuTipWnd & _ATTW,
		const RECT & rcExcludeArea
		) const;
	virtual bool OnGbwTooltip(
		const CExtGridHitTestInfo & htInfo
		);
	virtual bool OnGbwExpand(
		const CExtGridHitTestInfo & htInfo,
		UINT nDelayMilliseconds
		);
	virtual bool OnGbwPaintExpandedItemContent(
		CExtContentExpandWnd::PAINT_ITEM_CONTENT_DATA & picd
		) const;

protected:
	virtual bool OnGbwProcessVkBeginEdit( UINT nChar );
	
	virtual bool OnGbwDataDndIsAllowed() const;
	virtual CPoint OnGbwDataDndGetStartOffset() const;
	virtual bool OnGbwDataDndCanStart(
		const CExtGridHitTestInfo & htInfo
		);
	virtual void OnGbwDataDndDo(
		const CExtGridHitTestInfo & htInfo
		);

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtGridBaseWnd)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSysKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnSysKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
#if _MFC_VER < 0x700
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#else
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
#endif
	afx_msg LRESULT _OnPaintExpandedItemContent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
}; // class CExtGridBaseWnd

#if (!defined __EXT_MFC_NO_GRIDWND)

//////////////////////////////////////////////////////////////////////////
// CExtArray

template < class TYPE > class CExtArray
	: public CArray < TYPE, TYPE & >
{
protected:
	bool m_bSortedStateFlag:1;
	bool m_bSortedAscending:1;
	static int stat_CompareItems(
		const TYPE & element1,
		const TYPE & element2
		)
	{
		if( element1 < element2 )
			return -1;
		if( element2 < element1 )
			return 1;
		return 0;
	}
	static void stat_SwapItems(
		TYPE & element1,
		TYPE & element2
		)
	{
		TYPE temp = element1;
		element1 = element2;
		element2 = temp;
	}
public:
	virtual int OnCompareItems(
		const TYPE & element1,
		const TYPE & element2
		) const
	{
		return stat_CompareItems( element1, element2 );
	}
	virtual void OnSwapItems(
		TYPE & element1,
		TYPE & element2
		)
	{
		stat_SwapItems( element1, element2 );
	}
protected:
	void QuickSortRecursive(
		int d,
		int h,
		bool bAscending
		)
	{
		int i, j;
		int iTmpIndex;
		i = h;
		j = d;
		iTmpIndex = int( (d + h) / 2 );
		do
		{
			if( bAscending )
			{
				while( OnCompareItems( GetAt(j), GetAt(iTmpIndex) ) < 0 )
					j++;
				while( OnCompareItems( GetAt(i), GetAt(iTmpIndex) ) > 0 )
					i--;
			} // if( bAscending )
			else
			{
				while( OnCompareItems( GetAt(j), GetAt(iTmpIndex) ) > 0 )
					j++;
				while( OnCompareItems( GetAt(i), GetAt(iTmpIndex) ) < 0 )
					i--;
			} // else from if( bAscending )
			if( i >= j )
			{
				if( i != j )
					OnSwapItems( ElementAt(i), ElementAt(j) );
				i--;
				j++;
			} // if( i >= j )
		}
		while( j <= i );
		if( d < i )
			QuickSortRecursive( d, i, bAscending );
		if( j < h )
			QuickSortRecursive( j, h, bAscending );
	}
public:
	CExtArray(
		bool bSortItems = false,
		bool bAscending = true
		)
		: m_bSortedStateFlag ( bSortItems )
		, m_bSortedAscending( bAscending )
	{
	}
	CExtArray( const CExtArray < TYPE > & other )
		: m_bSortedStateFlag ( false )
		, m_bSortedAscending( true )
	{
		VERIFY( AssignFromOther( other ) );
	}
	void Clean()
	{
		RemoveAll();
		FreeExtra();
	}
	const TYPE & operator [] ( int i_index ) const
	{
		ASSERT( 0 <= i_index && i_index < GetSize() );
		return ( GetData() [ i_index ] );
	}
	const TYPE & GetAt( int i_index ) const
	{
		ASSERT( 0 <= i_index && i_index < GetSize() );
		return ( GetData() [i_index] );
	}
	TYPE & operator [] ( int i_index )
	{
		ASSERT( 0 <= i_index && i_index < GetSize() );
		return ( GetData() [i_index] );
	}
	bool IsSorted() const
	{
		return m_bSortedStateFlag;
	}
	bool IsSortedAscending() const
	{
		ASSERT( m_bSortedStateFlag );
		return m_bSortedAscending;
	}
	bool QuickSort( bool bAscending = true )
	{
		m_bSortedStateFlag = false;
		m_bSortedAscending = bAscending;
		bool rc = true;
		if( GetSize() == 0 )
		{
			m_bSortedStateFlag = true;
			return true;
		}
		try
		{
			int	iLow = 0;
			int iHigh = (int)(GetSize()-1);
			QuickSortRecursive( iLow, iHigh, bAscending );
			m_bSortedStateFlag = true;
		}
		catch( ... )
		{
			::SetLastError( ERROR_INVALID_PARAMETER );
			rc = false;
		}
		return rc;
	}
	int BinaryInsertPosFind( const TYPE & _key ) const
	{
		ASSERT( m_bSortedStateFlag );
		int nCount = int( GetSize() );
		if( nCount == 0 )
			return 0;
		if( nCount == 1 )
		{
			const TYPE & _element =
				( ( CExtArray < TYPE > & ) (*this) )
					.ElementAt( 0 );
			return ( _key < _element ) ? 0 : 1;
		} // if( nCount == 1 )

		int nIdx = nCount-1, nIdx2, nMargine1 = 0, nMargine2 = nCount-1;
		for( ; true; )
		{
			const TYPE & _element =
				( ( CExtArray < TYPE > & ) (*this) )
					.ElementAt( nIdx );
			int nCompareResult = OnCompareItems( _key, _element );
			if( nCompareResult == 0 )
				return nIdx;
			if( m_bSortedAscending )
			{
				if( nCompareResult > 0 )
					nMargine1 = nIdx;
				else
					nMargine2 = nIdx;
			} // if( m_bSortedAscending )
			else
			{
				if( nCompareResult > 0 )
					nMargine2 = nIdx;
				else
					nMargine1 = nIdx;
			} // else from if( m_bSortedAscending )
			nIdx2 = (nMargine2 - nMargine1)/2 + nMargine1;
			if( nIdx2 == nIdx )
				break;
			nIdx = nIdx2;
		}
		const TYPE & _element =
			( ( CExtArray < TYPE > & ) (*this) )
				.ElementAt( nIdx );
		int nCompareResult = OnCompareItems( _key, _element );
		return ( nCompareResult < 0 ) ? nIdx : (nIdx+1);
	}
	int BinarySearch( const TYPE & _key ) const
	{
		ASSERT( m_bSortedStateFlag );

		int nCount = (int)GetSize();
		if( nCount == 0 )
			return -1;

		int nIdx, nIdx2, nMargine1, nMargine2;

		nMargine1 = 0;
		nMargine2 = nCount - 1;
		nIdx = nMargine2;
		for( ; true; )
		{
			const TYPE & element = GetAt( nIdx );
			int compare_result = OnCompareItems( _key, element );
			if( compare_result == 0 )
				return nIdx;
			if( m_bSortedAscending )
			{
				if( compare_result > 0 )
					nMargine1 = nIdx;
				else
					nMargine2 = nIdx;
			} // if( m_bSortedAscending )
			else
			{
				if( compare_result > 0 )
					nMargine2 = nIdx;
				else
					nMargine1 = nIdx;
			} // else from if( m_bSortedAscending )
			nIdx2 = ( nMargine2 - nMargine1 ) / 2 + nMargine1;
			if( nIdx2 == nIdx )
				break;
			nIdx = nIdx2;
		} // for( ; true; )
		return -1;
	}
	bool AppendFromOther(
		const CExtArray < TYPE > & other,
		bool bResort = false
		)
	{
		bool retVal = true;
		try
		{
			int len = (int)other.GetSize();
			for( int i = 0; i < len; i++ )
			{
				const TYPE & element = other.GetAt(i);
				Add( (TYPE &) element );
			} // for( int i = 0; i < len; i++ )
			if( bResort )
				retVal =
					QuickSort( m_bSortedAscending );
		}
		catch( ... )
		{
			retVal = false;
		}
		return retVal;
	}
	bool AssignFromOther(
		const CExtArray < TYPE > & other,
		bool bCopyItems = true,
		bool bCopyParms = true,
		bool bResort = false
		)
	{
		bool retVal = true;
		try
		{
			if( bCopyItems )
			{
				Clean();
				retVal = AppendFromOther( other, bResort );
				ASSERT( GetSize() == other.GetSize() );
			} // if( bCopyItems )
			if( bCopyParms )
			{
				m_bSortedStateFlag = other.m_bSortedStateFlag;
				m_bSortedAscending = other.m_bSortedAscending;
			} // if( bCopyParms )
		}
		catch( ... )
		{
			retVal = false;
		}
		return retVal;
	}
	bool EvalDifference(
		CExtArray < TYPE > & other,
		CExtArray < TYPE > & result,
		bool bResort = false
		) const
	{ // compute all own items which are not present in other array
		ASSERT( m_bSortedStateFlag );
		ASSERT( other.m_bSortedStateFlag );
		result.Clean();
		int len = int( other.GetSize() );
		if( len == 0 )
			return result.AssignFromOther( *this, true, true, bResort );
		bool retVal = true;
		try
		{
			len = int( GetSize() );
			for( int i = 0; i < len; i++ )
			{
				const TYPE & element = GetAt(i);
				int nIdx = other.BinarySearch( element );
				if(nIdx == -1)
					result.Add( (TYPE &) element );
			} // for( int i = 0; i < len; i++ )
			if( bResort )
				retVal = ( const_cast < CExtArray < TYPE > * > ( this ) ) -> QuickSort( m_bSortedAscending );
		}
		catch( ... )
		{
			retVal = false;
		}
		return retVal;
	}
	bool EvalIntersection(
		CExtArray < TYPE > & other,
		CExtArray < TYPE > & result,
		bool bResort = false
		) const
	{ // computer all own items which are present in other array
		ASSERT( m_bSortedStateFlag );
		ASSERT( other.m_bSortedStateFlag );
		result.Clean();
		int len = int( GetSize() );
		if( len == 0 )
			return true;
		bool retVal = true;
		try
		{
			for( int i = 0; i < len; i++ )
			{
				const TYPE & element = GetAt(i);
				int nIdx = other.BinarySearch( element );
				if( nIdx != -1 )
					result.Add( const_cast < TYPE & > ( element ) );
			} // for( int i = 0; i < len; i++ )
			if( bResort )
				retVal = ( const_cast < CExtArray < TYPE > * > ( this ) ) -> QuickSort( m_bSortedAscending );
		}
		catch( ... )
		{
			retVal = false;
		}
		return retVal;
	}
	CExtArray < TYPE > & operator = ( const CExtArray < TYPE > & other )
	{
		Clean();
		VERIFY( AssignFromOther( other ) );
		return (*this);
	}
	void operator += ( const CExtArray < TYPE > & other )
	{
		VERIFY( AppendFromOther( other, m_bSortedStateFlag ) );
	}
	int InsertUnique( const TYPE & element )
	{ // returns element index
		int nCount = int( GetSize() );
		if( nCount == 0 )
		{
			m_bSortedStateFlag = true;
			Add( const_cast < TYPE & > ( element ) );
			return 0;
		} // if( nCount == 0 )
		ASSERT( m_bSortedStateFlag );
		int nIdx = BinaryInsertPosFind( element );
		if( nIdx == nCount )
		{
			if( ElementAt( nIdx-1 ) == element )
				return nIdx;
			Add( const_cast < TYPE & > ( element ) );
			return nCount;
		} // if( nCount == 0 )
		if( ElementAt( nIdx ) == element )
			return nIdx;
		InsertAt( nIdx, const_cast < TYPE & > ( element ), 1 );
		return nIdx;
	}
	int operator += ( const TYPE & element )
	{
		if( m_bSortedStateFlag || GetSize() == 0 )
			return InsertUnique( element );
		Add( * ( ( TYPE * ) ( &element ) ) );
		return int( GetSize() - 1 );
	}
	TYPE & ElementAt( int nIndex )
	{
		return CArray < TYPE, TYPE & > :: ElementAt( nIndex );
	}
	const TYPE & ElementAt( int nIndex ) const
	{
		return ( ( CArray < TYPE, TYPE & > * ) this ) -> ElementAt( nIndex );
	}
	int CompareWithOther(
		const CExtArray < TYPE > & other
		) const
	{
		int nCount = (int)GetSize();
		int nCountOther = (int)other.GetSize();
		if( nCount < nCountOther )
			return -1;
		if( nCount > nCountOther )
			return 1;
		int nIndex, nCmpResult;
		for( nIndex = 0; nIndex < nCount; nIndex ++ )
		{
			const TYPE & element1 = ElementAt( nIndex );
			const TYPE & element2 = other.ElementAt( nIndex );
			nCmpResult = OnCompareItems( element1, element2 );
			if( nCmpResult != 0 )
				return nCmpResult;
		}
		return 0;
	}
	bool operator == ( const CExtArray < TYPE > & other ) const
	{
		return ( CompareWithOther( other ) == 0 ) ? true : false;
	}
	bool operator != ( const CExtArray < TYPE > & other ) const
	{
		return ( CompareWithOther( other ) != 0 ) ? true : false;
	}
	bool operator <= ( const CExtArray < TYPE > & other ) const
	{
		return ( CompareWithOther( other ) <= 0 ) ? true : false;
	}
	bool operator >= ( const CExtArray < TYPE > & other ) const
	{
		return ( CompareWithOther( other ) >= 0 ) ? true : false;
	}
	bool operator <  ( const CExtArray < TYPE > & other ) const
	{
		return ( CompareWithOther( other ) <  0 ) ? true : false;
	}
	bool operator >  ( const CExtArray < TYPE > & other ) const
	{
		return ( CompareWithOther( other ) >  0 ) ? true : false;
	}
}; // template class CExtArray

/////////////////////////////////////////////////////////////////////////////
// CExtAlloc

#ifndef __EXT_ALLOC_MAX_REASONABLE_COUNT_IN_CHUNK
	#define __EXT_ALLOC_MAX_REASONABLE_COUNT_IN_CHUNK (10L*1000L)
#endif

#ifdef _DEBUG
	#define __EXT_DEBUG_ALLOC_TRACK_NUMBERS
	//#define __EXT_DEBUG_ALLOC_TRACE_NORMAL_EVENTS
#endif

#ifdef __EXT_DEBUG_ALLOC_TRACK_NUMBERS
	#define __EXT_DEBUG_ALLOC_TRACE0(__x__) TRACE0(__x__)
	#define __EXT_DEBUG_ALLOC_TRACE1(__x__,__y__) TRACE1(__x__,__y__)
	#define __EXT_DEBUG_ALLOC_TRACE2(__x__,__y__,__z__) TRACE2(__x__,__y__,__z__)
	#define __EXT_DEBUG_ALLOC_TRACE3(__x__,__y__,__z__,__w__) TRACE3(__x__,__y__,__z__,__w__)
#else // __EXT_DEBUG_ALLOC_TRACK_NUMBERS
	#define __EXT_DEBUG_ALLOC_TRACE0(__x__)
	#define __EXT_DEBUG_ALLOC_TRACE1(__x__,__y__)
	#define __EXT_DEBUG_ALLOC_TRACE2(__x__,__y__,__z__) 
	#define __EXT_DEBUG_ALLOC_TRACE3(__x__,__y__,__z__,__w__)
#endif // !__EXT_DEBUG_ALLOC_TRACK_NUMBERS

class __PROF_UIS_API CExtAlloc : public IMalloc
{
protected:
	CExtAlloc(
		__EXT_MFC_ULONG_SIZE_T nCountInChunk,
		bool bHeapMinimizeOnTheFly,
		IUnknown * pUnkOuter,
		bool bZeroNewMemory
		);
	virtual ~CExtAlloc();

	IUnknown * m_pUnkOuter;
	ULONG m_nRefCount;
	__EXT_MFC_ULONG_SIZE_T m_nCountInChunk, m_nSizeFreeMap;
#ifdef __EXT_DEBUG_ALLOC_TRACK_NUMBERS
	__EXT_MFC_ULONG_SIZE_T m_nDebugHighLevelAllocNo,
		m_nDebugHighLevelBreakAllocNo,
		m_nDebugHighLevelAllocCount;
#endif // __EXT_DEBUG_ALLOC_TRACK_NUMBERS
#ifdef _DEBUG
	bool m_bDebugInDesructor:1;
#endif // _DEBUG
	bool m_bHeapMinimizeOnTheFly:1;
	bool m_bZeroNewMemory:1;

	struct AFX_NOVTABLE _1_header_chunk_t
	{
		__EXT_MFC_ULONG_SIZE_T m_nBlockSize;
		IMalloc * m_pMalloc;
#ifdef __EXT_DEBUG_ALLOC_TRACK_NUMBERS
		__EXT_MFC_ULONG_SIZE_T m_nDebugHighLevelAllocNo;
#endif // __EXT_DEBUG_ALLOC_TRACK_NUMBERS
	}; // struct _1_header_chunk_t
	struct AFX_NOVTABLE _N_header_chunk_t
	{
		__EXT_MFC_ULONG_SIZE_T m_nHdrOffset; // positive
#ifdef __EXT_DEBUG_ALLOC_TRACK_NUMBERS
		__EXT_MFC_ULONG_SIZE_T m_nDebugHighLevelAllocNo;
#endif // __EXT_DEBUG_ALLOC_TRACK_NUMBERS
	}; // struct _N_header_chunk_t
	struct AFX_NOVTABLE _N_header_block_t
	{
		IMalloc * m_pMalloc;
		_N_header_block_t * m_pNext, * m_pPrev;
		__EXT_MFC_ULONG_SIZE_T m_nItemSize;
#ifdef __EXT_DEBUG_ALLOC_TRACK_NUMBERS
		__EXT_MFC_ULONG_SIZE_T m_nDebugHighLevelAllocCount;
#endif // __EXT_DEBUG_ALLOC_TRACK_NUMBERS
	}; // struct _N_header_block_t
	typedef
		CMap < __EXT_MFC_ULONG_SIZE_T, __EXT_MFC_ULONG_SIZE_T, _N_header_block_t *, _N_header_block_t * >
		HighLevelChunksMap_t;
	HighLevelChunksMap_t m_mapHighLevelChunks;
	bool _HighLevel_N_BlockIsFullyFree( _N_header_block_t * pHdr );
	bool _HighLevel_N_BlockHasFI( _N_header_block_t * pHdr );
	_N_header_block_t * _HighLevel_N_BlockGetWithFI( __EXT_MFC_ULONG_SIZE_T nSizeItem );
	__EXT_MFC_ULONG_SIZE_T _HighLevel_N_BlockGetTotlalSize( __EXT_MFC_ULONG_SIZE_T nSizeItem );
	void * _HighLevel_N_BlockGetFirstFI( _N_header_block_t * pHdr, bool bMarkAllocated );
	void _HighLevel_N_BlockOptimizeEmpty( _N_header_block_t * pHdr );
	void _HighLevel_N_BlockFree( _N_header_block_t * pHdr );
	void * _HighLevel_1Alloc(
		__EXT_MFC_ULONG_SIZE_T cb
		);
	void * _HighLevel_NAlloc(
		__EXT_MFC_ULONG_SIZE_T cb
		);
	void * _HighLevel_1Realloc( 
		void * pv,
		__EXT_MFC_ULONG_SIZE_T cb
		);
	void * _HighLevel_NRealloc( 
		void * pv,
		__EXT_MFC_ULONG_SIZE_T cb
		);
	void _HighLevel_1Free( 
		void * pv
		);
	void _HighLevel_NFree( 
		void * pv
		);
	__EXT_MFC_ULONG_SIZE_T _HighLevel_1GetSize( 
		void * pv
		);
	__EXT_MFC_ULONG_SIZE_T _HighLevel_NGetSize( 
		void * pv
		);
	int _HighLevel_1DidAlloc( 
		void * pv
		);
	int _HighLevel_NDidAlloc( 
		void * pv
		);
	void _HighLevel_1HeapMinimize();
	void _HighLevel_NHeapMinimize();
	virtual void * _LowLevel_alloc( __EXT_MFC_ULONG_SIZE_T nSize );
	virtual void _LowLevel_free( void * pMemory );
	virtual void * _LowLevel_realloc( void * pMemory, __EXT_MFC_ULONG_SIZE_T nSize );
#ifdef __EXT_DEBUG_ALLOC_TRACK_NUMBERS
	void _HighLevel_trace_leaks();
	typedef
		CMap < LPVOID, LPVOID, __EXT_MFC_ULONG_SIZE_T, __EXT_MFC_ULONG_SIZE_T >
		CrtLibTraceMap_t;
	CrtLibTraceMap_t m_mapLowLevelAllocationsTrace;
	virtual void _LowLevel_trace_leaks();
	struct AFX_NOVTABLE _CRT_header_chunk_t
	{
		__EXT_MFC_ULONG_SIZE_T m_nBlockSize;
		IMalloc * m_pMalloc;
		__EXT_MFC_ULONG_SIZE_T m_nDebugLowLevelAllocNo;
	}; // struct _CRT_header_chunk_t
	__EXT_MFC_ULONG_SIZE_T m_nDebugLowLevelAllocNo, m_nDebugLowLevelBreakAllocNo;
#endif // __EXT_DEBUG_ALLOC_TRACK_NUMBERS

	static bool _LowLevel_IsValidPointer(
		const CExtAlloc * pAlloc,
		bool bCheckReadAccess = true,
		bool bCheckWriteAccess = true
		);
	static bool _LowLevel_IsValidPointer(
		LPCVOID pMemory,
		__EXT_MFC_ULONG_SIZE_T nByteCount,
		bool bCheckReadAccess = true,
		bool bCheckWriteAccess = true
		);

public:
	static IMalloc * CreateObject(
		__EXT_MFC_ULONG_SIZE_T nCountInChunk = 256,
		bool bHeapMinimizeOnTheFly = false,
		IUnknown * pUnkOuter = NULL,
		bool bZeroNewMemory = true
		);

	/////////////////////// IUnknown /////////////////////////
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        REFIID riid,
        void ** ppvObject
		);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);

	/////////////////////// IMalloc /////////////////////////
	virtual void * STDMETHODCALLTYPE Alloc(
		__EXT_MFC_ULONG_SIZE_T cb
		);
	virtual void * STDMETHODCALLTYPE Realloc( 
		void * pv,
		__EXT_MFC_ULONG_SIZE_T cb
		);
	virtual void STDMETHODCALLTYPE Free( 
		void * pv
		);
	virtual __EXT_MFC_ULONG_SIZE_T STDMETHODCALLTYPE GetSize( 
		void * pv
		);
	virtual int STDMETHODCALLTYPE DidAlloc( 
		void * pv
		);
	virtual void STDMETHODCALLTYPE HeapMinimize(void);
}; // class __PROF_UIS_API CExtAlloc

/////////////////////////////////////////////////////////////////////////////
// CExtArchiveIStream

class __PROF_UIS_API CExtArchiveIStream : public IStream
{
	ULONG m_nRefCount;
	CArchive * m_pAr;
	bool m_bAutoDestroyArchive:1;
public:
	CExtArchiveIStream(
		CArchive * pAr = NULL,
		bool bAutoDestroyArchive = false
		);
	CExtArchiveIStream(
		CArchive & ar,
		bool bAutoDestroyArchive = false
		);
	virtual ~CExtArchiveIStream();
	CArchive * ArchiveAttach(
		CArchive * pAr = NULL,
		bool bAutoDestroyArchive = false
		);
	CArchive * ArchiveAttach(
		CArchive & ar,
		bool bAutoDestroyArchive = false
		);
	CArchive * ArchiveDetach();
	operator IStream * ()
	{
		return this;
	}
public:

	/////////////////////// IUnknown /////////////////////////
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        REFIID riid,
        void ** ppvObject
		);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);
	/////////////////////// ISequentialStream /////////////////////////
    virtual HRESULT STDMETHODCALLTYPE Read( 
        void * pv,
        ULONG cb,
        ULONG * pcbRead
		);
    virtual HRESULT STDMETHODCALLTYPE Write( 
        const void * pv,
        ULONG cb,
        ULONG * pcbWritten
		);
	
	/////////////////////// IStream /////////////////////////
    virtual HRESULT STDMETHODCALLTYPE Seek( 
        LARGE_INTEGER dlibMove,
        DWORD dwOrigin,
        ULARGE_INTEGER * plibNewPosition
		);
    virtual HRESULT STDMETHODCALLTYPE SetSize( 
        ULARGE_INTEGER libNewSize
		);
    virtual HRESULT STDMETHODCALLTYPE CopyTo( 
        IStream * pstm,
        ULARGE_INTEGER cb,
        ULARGE_INTEGER * pcbRead,
        ULARGE_INTEGER * pcbWritten
		);
    virtual HRESULT STDMETHODCALLTYPE Commit( 
        DWORD grfCommitFlags
		);
    virtual HRESULT STDMETHODCALLTYPE Revert(void);
    virtual HRESULT STDMETHODCALLTYPE LockRegion( 
        ULARGE_INTEGER libOffset,
        ULARGE_INTEGER cb,
        DWORD dwLockType
		);
    virtual HRESULT STDMETHODCALLTYPE UnlockRegion( 
        ULARGE_INTEGER libOffset,
        ULARGE_INTEGER cb,
        DWORD dwLockType
		);
    virtual HRESULT STDMETHODCALLTYPE Stat( 
        STATSTG * pstatstg,
        DWORD grfStatFlag
		);
    virtual HRESULT STDMETHODCALLTYPE Clone( 
        IStream * * ppstm
		);
}; // class CExtArchiveIStream

/////////////////////////////////////////////////////////////////////////////
// styles for CExtGridWnd

// grid window behavior/sort/edit/drag-n-drop styles provided with
// CExtGridWnd::BseGetStyle() and CExtGridWnd::BseModifyStyle()

// allow editing
#define __EGWS_BSE_EDIT_CELLS_OUTER_T			0x00000001L
#define __EGWS_BSE_EDIT_CELLS_OUTER_B			0x00000002L
#define __EGWS_BSE_EDIT_CELLS_OUTER_HORZ		(__EGWS_BSE_EDIT_CELLS_OUTER_T|__EGWS_BSE_EDIT_CELLS_OUTER_B)
#define __EGWS_BSE_EDIT_CELLS_OUTER_L			0x00000004L
#define __EGWS_BSE_EDIT_CELLS_OUTER_R			0x00000008L
#define __EGWS_BSE_EDIT_CELLS_OUTER_VERT		(__EGWS_BSE_EDIT_CELLS_OUTER_L|__EGWS_BSE_EDIT_CELLS_OUTER_R)
#define __EGWS_BSE_EDIT_CELLS_OUTER				(__EGWS_BSE_EDIT_CELLS_OUTER_HORZ|__EGWS_BSE_EDIT_CELLS_OUTER_VERT)
#define __EGWS_BSE_EDIT_CELLS_INNER				0x00000010L
// allow "walking" editors for inner cells
#define __EGWS_BSE_WALK_HORZ					0x00000020L
#define __EGWS_BSE_WALK_VERT					0x00000040L
#define __EGWS_BSE_WALK							(__EGWS_BSE_WALK_HORZ|__EGWS_BSE_WALK_VERT)

// where cell ellipsis/dropdown/updown buttons should appear?
// in any cell which have buttons
#define __EGWS_BSE_BUTTONS_PERSISTENT			0x00000080L
// in the focused cell only
#define __EGWS_BSE_BUTTONS_IN_FOCUSED_CELL		0x00000100L
// any cell in the focused row
#define __EGWS_BSE_BUTTONS_IN_FOCUSED_ROW		0x00000200L
// any cell in the focused column
#define __EGWS_BSE_BUTTONS_IN_FOCUSED_COLUMN	0x00000400L
// in the hovered cell
#define __EGWS_BSE_BUTTONS_IN_HOVERED_CELL		0x00000800L
// any cell in the hovered row
#define __EGWS_BSE_BUTTONS_IN_HOVERED_ROW		0x00001000L
// any cell in the hovered column
#define __EGWS_BSE_BUTTONS_IN_HOVERED_COLUMN	0x00002000L
// in any selected cell
#define __EGWS_BSE_BUTTONS_IN_SELECTED_CELLS	0x00004000L
// any cell with selection in the same row
#define __EGWS_BSE_BUTTONS_IN_SELECTED_ROW		0x00008000L
// any cell with selection in the same column
#define __EGWS_BSE_BUTTONS_IN_SELECTED_COLUMN	0x00010000L
#define __EGWS_BSE_MASK \
	(__EGWS_BSE_BUTTONS_PERSISTENT \
	|__EGWS_BSE_BUTTONS_IN_FOCUSED_CELL \
	|__EGWS_BSE_BUTTONS_IN_FOCUSED_ROW \
	|__EGWS_BSE_BUTTONS_IN_FOCUSED_COLUMN \
	|__EGWS_BSE_BUTTONS_IN_HOVERED_CELL \
	|__EGWS_BSE_BUTTONS_IN_HOVERED_ROW \
	|__EGWS_BSE_BUTTONS_IN_HOVERED_COLUMN \
	|__EGWS_BSE_BUTTONS_IN_SELECTED_CELLS \
	|__EGWS_BSE_BUTTONS_IN_SELECTED_ROW \
	|__EGWS_BSE_BUTTONS_IN_SELECTED_COLUMN \
	)

// activate inplace editor on single lbutton click
#define __EGWS_BSE_EDIT_SINGLE_LCLICK			0x00020000L
#define __EGWS_BSE_EDIT_SINGLE_FOCUSED_ONLY		0x00040000L
// activate inplace editor on double lbutton click
#define __EGWS_BSE_EDIT_DOUBLE_LCLICK			0x00080000L
// activate inplace editor on VK_RETURN key-down
#define __EGWS_BSE_EDIT_RETURN_CLICK			0x00100000L

// allow sorting by multiple rows/columns
#define __EGWS_BSE_SORT_COLUMNS_ALLOW_MULTIPLE	0x00200000L
#define __EGWS_BSE_SORT_ROWS_ALLOW_MULTIPLE		0x00400000L
// row/column sorting (click on outer cells)
#define __EGWS_BSE_SORT_COLUMNS_T				0x00800000L
#define __EGWS_BSE_SORT_COLUMNS_B				0x01000000L
#define __EGWS_BSE_SORT_COLUMNS					(__EGWS_BSE_SORT_COLUMNS_T|__EGWS_BSE_SORT_COLUMNS_B)
#define __EGWS_BSE_SORT_MULTIPLE_COLUMNS		(__EGWS_BSE_SORT_COLUMNS|__EGWS_BSE_SORT_COLUMNS_ALLOW_MULTIPLE)
#define __EGWS_BSE_SORT_ROWS_L					0x02000000L
#define __EGWS_BSE_SORT_ROWS_R					0x04000000L
#define __EGWS_BSE_SORT_ROWS					(__EGWS_BSE_SORT_ROWS_L|__EGWS_BSE_SORT_ROWS_R)
#define __EGWS_BSE_SORT_MULTIPLE_ROWS			(__EGWS_BSE_SORT_ROWS|__EGWS_BSE_SORT_ROWS_ALLOW_MULTIPLE)
#define __EGWS_BSE_SORT_ANY_MASK				(__EGWS_BSE_SORT_COLUMNS|__EGWS_BSE_SORT_ROWS)

// activate inplace editor automatically when starting to type any text
#define __EGWS_BSE_EDIT_AUTO					0x08000000L

// default bse styles
#define __EGWS_BSE_DEFAULT \
	(__EGWS_BSE_BUTTONS_PERSISTENT \
	|__EGWS_BSE_EDIT_SINGLE_LCLICK \
	|__EGWS_BSE_EDIT_SINGLE_FOCUSED_ONLY \
	|__EGWS_BSE_EDIT_DOUBLE_LCLICK \
	|__EGWS_BSE_EDIT_RETURN_CLICK \
	|__EGWS_BSE_EDIT_CELLS_INNER \
	|__EGWS_BSE_EDIT_AUTO \
	)

// grid window extended behavior/sort/edit/drag-n-drop styles provided with
// CExtGridWnd::BseGetStyleEx() and CExtGridWnd::BseModifyStyleEx()

// inner/outer row/column highlighting during sort/drag starting
#define __EGBS_BSE_EX_HIGHLIGHT_PRESSING_COLUMNS_INNER	0x00000001L
#define __EGBS_BSE_EX_HIGHLIGHT_PRESSING_COLUMNS_OUTER	0x00000002L
#define __EGBS_BSE_EX_HIGHLIGHT_PRESSING_COLUMNS		(__EGBS_BSE_EX_HIGHLIGHT_PRESSING_COLUMNS_INNER|__EGBS_BSE_EX_HIGHLIGHT_PRESSING_COLUMNS_OUTER)
#define __EGBS_BSE_EX_HIGHLIGHT_PRESSING_ROWS_INNER		0x00000004L
#define __EGBS_BSE_EX_HIGHLIGHT_PRESSING_ROWS_OUTER		0x00000008L
#define __EGBS_BSE_EX_HIGHLIGHT_PRESSING_ROWS			(__EGBS_BSE_EX_HIGHLIGHT_PRESSING_ROWS_INNER|__EGBS_BSE_EX_HIGHLIGHT_PRESSING_ROWS_OUTER)
#define __EGBS_BSE_EX_HIGHLIGHT_PRESSING_ALL				(__EGBS_BSE_EX_HIGHLIGHT_PRESSING_COLUMNS|__EGBS_BSE_EX_HIGHLIGHT_PRESSING_ROWS)
#define __EGBS_BSE_EX_HIGHLIGHT_PRESSING_STAY_COLUMNS	0x00000010L
#define __EGBS_BSE_EX_HIGHLIGHT_PRESSING_STAY_ROWS		0x00000020L
#define __EGBS_BSE_EX_HIGHLIGHT_PRESSING_STAY			(__EGBS_BSE_EX_HIGHLIGHT_PRESSING_STAY_COLUMNS|__EGBS_BSE_EX_HIGHLIGHT_PRESSING_STAY_ROWS)
// enable drag-n-drop starting events (for outer rows/columns),
// perform swapping on drag-n-drop
#define __EGBS_BSE_EX_DRAG_START_COLUMNS				0x00000040L
#define __EGBS_BSE_EX_DRAG_SWAP_COLUMNS					0x00000080L
#define __EGBS_BSE_EX_DRAG_START_ROWS					0x00000100L
#define __EGBS_BSE_EX_DRAG_SWAP_ROWS					0x00000200L
#define __EGBS_BSE_EX_DRAG_COLUMNS						(__EGBS_BSE_EX_DRAG_START_COLUMNS|__EGBS_BSE_EX_DRAG_SWAP_COLUMNS)
#define __EGBS_BSE_EX_DRAG_ROWS							(__EGBS_BSE_EX_DRAG_START_ROWS|__EGBS_BSE_EX_DRAG_SWAP_ROWS)
#define __EGBS_BSE_EX_DRAG_START_ANY_MASK				(__EGBS_BSE_EX_DRAG_START_COLUMNS|__EGBS_BSE_EX_DRAG_START_ROWS)
// perform removing on drag-n-drop
#define __EGBS_BSE_EX_DRAG_REMOVE_COLUMNS				0x00000400L
#define __EGBS_BSE_EX_DRAG_REMOVE_ROWS					0x00000800L
// use proportional resizing when "none" scrolling strategy is applied
#define __EGBS_BSE_EX_PROPORTIONAL_COLUMN_WIDTHS		0x00001000L
#define __EGBS_BSE_EX_PROPORTIONAL_ROW_HEIGHTS			0x00002000L
#define __EGBS_BSE_EX_PROPORTIONAL_XY					(__EGBS_BSE_EX_PROPORTIONAL_COLUMN_WIDTHS|__EGBS_BSE_EX_PROPORTIONAL_ROW_HEIGHTS)
// do best fit sizing on double clicks over divider
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_COLUMN_OUTER			0x00004000L
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_COLUMN_INNER			0x00008000L
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_ROW_OUTER				0x00010000L
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_ROW_INNER				0x00020000L
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_VISIBLE_ROWS			0x00040000L // walk the visible rows only when the column divider is clicked
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_VISIBLE_COLUMNS		0x00080000L // walk the visible columns only when the row divider is clicked
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_COLUMN_MEASURE_OUTER	0x00100000L
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_COLUMN_MEASURE_INNER	0x00200000L
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_ROW_MEASURE_OUTER		0x00400000L
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_ROW_MEASURE_INNER		0x00800000L
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_COLUMN_MEASURE_ALL	(__EGBS_BSE_EX_DBLCLK_BEST_FIT_COLUMN_MEASURE_OUTER|__EGBS_BSE_EX_DBLCLK_BEST_FIT_COLUMN_MEASURE_INNER)
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_ROW_MEASURE_ALL		(__EGBS_BSE_EX_DBLCLK_BEST_FIT_ROW_MEASURE_OUTER|__EGBS_BSE_EX_DBLCLK_BEST_FIT_ROW_MEASURE_INNER)
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_COLUMN				(__EGBS_BSE_EX_DBLCLK_BEST_FIT_COLUMN_OUTER|__EGBS_BSE_EX_DBLCLK_BEST_FIT_COLUMN_MEASURE_ALL)
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_ROW					(__EGBS_BSE_EX_DBLCLK_BEST_FIT_ROW_OUTER|__EGBS_BSE_EX_DBLCLK_BEST_FIT_ROW_MEASURE_ALL)
#define __EGBS_BSE_EX_DBLCLK_BEST_FIT_AVAILABLE				(__EGBS_BSE_EX_DBLCLK_BEST_FIT_COLUMN_OUTER|__EGBS_BSE_EX_DBLCLK_BEST_FIT_COLUMN_INNER|__EGBS_BSE_EX_DBLCLK_BEST_FIT_ROW_OUTER|__EGBS_BSE_EX_DBLCLK_BEST_FIT_ROW_INNER)

// render grid lines in the data cell area free of any cells
#define __EGBS_BSE_EX_EAL_BOTTOM_H							0x01000000L
#define __EGBS_BSE_EX_EAL_BOTTOM_V							0x02000000L
#define __EGBS_BSE_EX_EAL_RIGHT_H							0x04000000L
#define __EGBS_BSE_EX_EAL_RIGHT_V							0x08000000L
#define __EGBS_BSE_EX_EAL_CORNER_H							0x10000000L
#define __EGBS_BSE_EX_EAL_CORNER_V							0x20000000L

#define __EGWS_BSE_EX_DEFAULT 0

/////////////////////////////////////////////////////////////////////////////
// CExtGridCell

class CExtGridHitTestInfo;
class CExtGridWnd;
class CExtGridDataProvider;

// values for dwHelperPaintFlags parameter of:
//    CExtGridBaseWnd::OnGbwPaintCell()
//    CExtGridCell::OnPaintBackground()
//    CExtGridCell::OnPaintForeground()

// column of the cell is hovered
#define __EGCPF_HOVER_BY_COLUMN						0x00000001L
// row of the cell is hovered
#define __EGCPF_HOVER_BY_ROW						0x00000002L
// column of the cell contains at least one selected cell
#define __EGCPF_HIGHLIGHTED_BY_SELECTED_COLUMN		0x00000004L
// row of the cell contains at least one selected cell
#define __EGCPF_HIGHLIGHTED_BY_SELECTED_ROW			0x00000008L
// column of the cell contains focused cell
#define __EGCPF_HIGHLIGHTED_BY_FOCUSED_COLUMN		0x00000010L
// row of the cell contains focused cell
#define __EGCPF_HIGHLIGHTED_BY_FOCUSED_ROW			0x00000020L
// grid control or any kind of its child window is focused
#define __EGCPF_FOCUSED_CONTROL						0x00000040L
// cell is highlighted by pressed row/column
#define __EGCPF_HIGHLIGHTED_BY_PRESSED_COLUMN		0x00000080L
#define __EGCPF_HIGHLIGHTED_BY_PRESSED_ROW			0x00000100L
// painting dragged outer cell content
#define __EGCPF_OUTER_DND							0x00000200L
// printer/print-preview output rendering (with or without meta file)
#define __EGCPF_PRINTER								0x00000400L
#define __EGCPF_PRINT_PREVIEW						0x00000800L
#define __EGCPF_PRINTING_TARGET_MASK				(__EGCPF_PRINTER|__EGCPF_PRINT_PREVIEW)
// rendering DC is metafile based
#define __EGCPF_METAFILE							0x00001000L
#define __EGCPF_SIMPLIFIED_RENDERING_TARGET			(__EGCPF_PRINTING_TARGET_MASK|__EGCPF_METAFILE)
// do not draw grid cell parts for printouts
#define __EGCPF_NO_BUTTONS							0x00002000L
#define __EGCPF_NO_SORT_ARROWS						0x00004000L
#define __EGCPF_NO_FOCUS_ARROWS						0x00008000L
#define __EGCPF_NO_BUTTON_ELLIPSIS_DOTS				0x00010000L
#define __EGCPF_NO_BUTTON_DROPDOWN_ARROW			0x00020000L
// painting report group area item
#define __EGCPF_REPORT_GROUP_AREA					0x00040000L
// painting expanded content
#define __EGCPF_EXPANDED_CONTENT					0x00080000L

#define __EGCPF_USER_STYLE_11						0x00100000L
#define __EGCPF_USER_STYLE_10						0x00200000L
#define __EGCPF_USER_STYLE_09						0x00400000L
#define __EGCPF_USER_STYLE_08						0x00800000L
#define __EGCPF_USER_STYLE_07						0x01000000L
#define __EGCPF_USER_STYLE_06						0x02000000L
#define __EGCPF_USER_STYLE_05						0x04000000L
#define __EGCPF_USER_STYLE_04						0x08000000L
#define __EGCPF_USER_STYLE_03						0x10000000L
#define __EGCPF_USER_STYLE_02						0x20000000L
#define __EGCPF_USER_STYLE_01						0x40000000L
#define __EGCPF_USER_STYLE_00						0x80000000L

// values for CExtGridCell::m_dwStyle

// horizontal text align
#define __EGCS_TA_HORZ_BY_TYPE						0x00000000L
#define __EGCS_TA_HORZ_LEFT							0x00000001L
#define __EGCS_TA_HORZ_RIGHT						0x00000002L
#define __EGCS_TA_HORZ_CENTER						0x00000003L
#define __EGCS_TA_HORZ_MASK							0x00000003L
// vertical text align
#define __EGCS_TA_VERT_BY_TYPE						0x00000000L
#define __EGCS_TA_VERT_TOP							0x00000010L
#define __EGCS_TA_VERT_BOTTOM						0x00000020L
#define __EGCS_TA_VERT_CENTER						0x00000030L
#define __EGCS_TA_VERT_MASK							0x00000030L
// horizontal icon/check align ( __EGCS_ICA_HORZ_BY_TYPE by default
// is the same as __EGCS_ICA_HORZ_CENTER, __EGCS_ICA_HORZ_CENTER
// is used only when text is empty)
#define __EGCS_ICA_HORZ_BY_TYPE						0x00000000L
#define __EGCS_ICA_HORZ_LEFT						0x00000100L
#define __EGCS_ICA_HORZ_RIGHT						0x00000200L
#define __EGCS_ICA_HORZ_CENTER						0x00000300L
#define __EGCS_ICA_HORZ_MASK						0x00000300L
// vertical icon/check align ( __EGCS_ICA_VERT_BY_TYPE by default
// is the same as __EGCS_ICA_VERT_CENTER)
#define __EGCS_ICA_VERT_BY_TYPE						0x00000000L
#define __EGCS_ICA_VERT_TOP							0x00001000L
#define __EGCS_ICA_VERT_BOTTOM						0x00002000L
#define __EGCS_ICA_VERT_CENTER						0x00003000L
#define __EGCS_ICA_VERT_MASK						0x00003000L
// draw "..." if not enough room for cell's text
#define __EGCS_TEXT_ELLIPSIS						0x00000008L
// draw row/column number in header cell
#define __EGCS_HDR_ROW_COLUMN_NUMBER				0x00000080L
// draw focus arrow in header cell
#define __EGCS_HDR_FOCUS_ARROW_RESERVE_SPACE		0x00000800L
// draw focus arrow in header cell
#define __EGCS_HDR_FOCUS_ARROW_DISPLAY				0x00008000L
// checkmark type
#define __EGCS_CHK_NONE								0x00000000L
#define __EGCS_CHK_CHECK							0x00010000L
#define __EGCS_CHK_RADIO							0x00020000L
#define __EGCS_CHK_INDETERMINATE					0x00030000L
#define __EGCS_CHK_MASK								0x00030000L
// checkmark state
#define __EGCS_CHECKED								0x00080000L
// have sort-arrow
#define __EGCS_SORT_ARROW							0x00100000L
// have sort-arrow is descending
#define __EGCS_SORT_DESCENDING						0x00200000L
// have up-down button
#define __EGCS_BUTTON_UPDOWN						0x00400000L
#define __EGCS_PRESSED_UP							0x00800000L
#define __EGCS_PRESSED_DOWN							0x01000000L
// have drop-down button
#define __EGCS_BUTTON_DROPDOWN						0x02000000L
#define __EGCS_PRESSED_DROPDOWN						0x04000000L

#define __EGCS_PRESSED \
	(	__EGCS_PRESSED_DOWN \
	|	__EGCS_PRESSED_UP \
	|	__EGCS_PRESSED_DOWN \
	|	__EGCS_PRESSED_ELLIPSIS \
	)

// have ellipsis button
#define __EGCS_BUTTON_ELLIPSIS						0x08000000L
#define __EGCS_PRESSED_ELLIPSIS						0x10000000L
// read-only cell mode
#define __EGCS_READ_ONLY							0x20000000L
// display dropdown listbox-menu when dropdown button clicked
#define __EGCS_TRACK_DROPDOWN_MENU					0x40000000L
// force disable inplace-control
#define __EGCS_NO_INPLACE_CONTROL					0x80000000L
// default style set for CExtGridCell::m_dwStyle
#define __EGCS_DEFAULT_STYLES						(__EGCS_TRACK_DROPDOWN_MENU)
// default value for dwStyleMask parameter
// of CExtGridCell::Compare()
#define __EGCS_COMPARE_MASK							(__EGCS_CHK_MASK|__EGCS_CHECKED)

// values for CExtGridCell::m_dwStyleEx

// disable column/row sorting on this cell click
#define __EGCS_EX_DISABLE_SORT						0x00000001L
// disable column/row dragging
#define __EGCS_EX_DISABLE_START_DRAG				0x00000002L
// disable inplace-control border painting
#define __EGCS_EX_NO_INPLACE_CONTROL_BORDER			0x00000004L
// undefined role flag (cell is intersection of other cells with different values)
#define __EGCS_EX_UNDEFINED_ROLE					0x00000008L
// wraps text into multiple lines in a cell. The number of wrapped lines is 
// dependent on the width of the column and the length of the cell contents. 
#define __EGCS_EX_WRAP_TEXT							0x00000010L
// do not paint sort arrow
#define __EGCS_EX_DO_NOT_PAINT_SORT_ARROW			0x00000020L
// disable auto editing for particular cell
#define __EGCS_EX_NO_AUTO_EDIT						0x00000040L
// empty flag (cell is empty)
#define __EGCS_EX_EMPTY								0x00000080L
// vertical layout (if supported)
#define __EGCS_EX_VERTICAL_LAYOUT					0x00000100L
// uses rest free area
#define __EGCS_EX_REST_AREA							0x00000200L
// tree grid related: tree node is hidden
#define __EGCS_EX_HIDDEN_TREE_NODE					0x00000400L
// resize in-place editor window automatically during text typing
#define __EGCS_EX_AUTO_SIZE_INPLACE_CONTROL			0x00000800L
// invisible row/column in printing and print preview
#define __EGCS_EX_PPVW_HIDDEN						0x00001000L
// keep header cell active in report grid when it's drag-n-dropped
// from header area into group area
#define __EGCS_EX_KEEP_ACTIVE_ON_GROUPING			0x00002000L
// do not select entire text in the in-place cell editor
#define __EGCS_EX_NO_INPLACE_CONTROL_SELECTION		0x00004000L
// do not parse text when starting automatic editing
#define __EGCS_EX_NO_VALIDATE_AUTO_EDIT_TEXT		0x00008000L
// disable ellipsis button
#define __EGCS_EX_BUTTON_ELLIPSIS_DISABLED			0x00010000L
// disable dropdown button
#define __EGCS_EX_BUTTON_DROPDOWN_DISABLED			0x00020000L
// disable up button
#define __EGCS_EX_BUTTON_UPDOWN_UP_DISABLED			0x00040000L
// disable down button
#define __EGCS_EX_BUTTON_UPDOWN_DOWN_DISABLED		0x00080000L
// invisible cell
#define __EGCS_EX_INVISIBLE							0x00100000L

#define __EGCS_EX_DO_NOT_NOTIFY_LB_SEL_FOR_EQUAL_TEXT	0x00200000L

#define __EGCS_EX_USER_STYLE_09						0x00400000L
#define __EGCS_EX_USER_STYLE_08						0x00800000L
#define __EGCS_EX_USER_STYLE_07						0x01000000L
#define __EGCS_EX_USER_STYLE_06						0x02000000L
#define __EGCS_EX_USER_STYLE_05						0x04000000L
#define __EGCS_EX_USER_STYLE_04						0x08000000L
#define __EGCS_EX_USER_STYLE_03						0x10000000L
#define __EGCS_EX_USER_STYLE_02						0x20000000L
#define __EGCS_EX_USER_STYLE_01						0x40000000L
#define __EGCS_EX_USER_STYLE_00						0x80000000L

// default style set for CExtGridCell::m_dwStyleEx
#define __EGCS_EX_DEFAULT_STYLES					__EGCS_EX_EMPTY

// default value for dwStyleExMask parameter
// of CExtGridCell::Compare()
#define __EGCS_EX_COMPARE_MASK						0x00000000L

class CExtPopupInplaceListBox;
class CExtPopupListBoxMenuWnd;

class __PROF_UIS_API CExtGridCell
	: public CObject
{
public:
	DECLARE_SERIAL( CExtGridCell );
protected:
	CExtGridDataProvider * m_pDataProvider;
	DWORD m_dwStyle, m_dwStyleEx;
	INT m_nIconIndex;
	INT m_nFontIndex;
	LPARAM m_lParam;
public:
	CExtGridCell(
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCell( const CExtGridCell & other );
	virtual ~CExtGridCell();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG
	virtual LPARAM LParamGet() const;
	virtual LPARAM LParamSet( LPARAM lParamNew = 0L );
	virtual DWORD GetStyle() const;
	virtual DWORD ModifyStyle(
		DWORD dwStyleAdd,
		DWORD dwStyleRemove = 0L
		);
	virtual DWORD GetStyleEx() const;
	virtual DWORD ModifyStyleEx(
		DWORD dwStyleExAdd,
		DWORD dwStyleExRemove = 0L
		);
protected:
	void _AssignM( const CExtGridCell & other );
	void _InitM();
	static CExtGridCell * g_pCellMenuTracking;
public:
	static CExtGridCell * GetMenuTrackingCell();
	virtual CExtGridDataProvider * DataProviderGet();
	const CExtGridDataProvider * DataProviderGet() const;
	virtual CExtGridDataProvider * DataProviderSet(
		CExtGridDataProvider * pDataProvider
		);
	virtual void Serialize( CArchive & ar );
	virtual bool IsUndefined() const;
	virtual bool IsInvisible() const;
	virtual bool IsEmpty() const;
	virtual void Empty();
	virtual void Reset();
	virtual void Assign( const CExtGridCell & other );
	CExtGridCell & operator = (  const CExtGridCell & other );
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;
	virtual int CompareStyleOnly(
		const CExtGridCell & other,
		DWORD dwStyleMask,
		DWORD dwStyleExMask
		) const;
	virtual void TextGet( CExtSafeString & strCopy ) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	virtual void GetVariant( VARIANT & varCopy ) const;
	bool operator == ( const CExtGridCell & other ) const;
	bool operator != ( const CExtGridCell & other ) const;
	bool operator <  ( const CExtGridCell & other ) const;
	bool operator >  ( const CExtGridCell & other ) const;
	bool operator <= ( const CExtGridCell & other ) const;
	bool operator >= ( const CExtGridCell & other ) const;
	static HRESULT stat_VariantClear( LPVARIANT pVariant );
	static HRESULT stat_VariantCopy( VARIANTARG * pvargDest, VARIANTARG * pvargSrc );
	static HRESULT stat_VariantSerializeData(
		LPVARIANT pVariant,
		IStream * pStream,
		bool bStore,
		VARTYPE * pVarTypeSerialized
		);
	static HRESULT stat_VariantSerialize(
		LPVARIANT pVariant,
		IStream * pStream,
		bool bStore
		);
	static HRESULT stat_VariantSerialize(
		LPVARIANT pVariant,
		CArchive & ar
		);
	static int stat_VariantCompare(
		LPVARIANT pVariantLeft,
		LPVARIANT pVariantRight,
		LCID lcid = LOCALE_USER_DEFAULT,
		ULONG dwFlags = 0L
		);
	static HRESULT stat_VariantCompareHR(
		LPVARIANT pVariantLeft,
		LPVARIANT pVariantRight,
		LCID lcid = LOCALE_USER_DEFAULT,
		ULONG dwFlags = 0L,
		bool bReturnOnlyValidResult = true // return only VARCMP_EQ or VARCMP_GT or VARCMP_LT values
		);
	virtual __EXT_MFC_SAFE_LPCTSTR GetTextBuffer() const;
	virtual CExtGridCell * Clone(
		IMalloc * pMalloc = NULL
		) const;
	void * operator new (
		size_t nSize
		)
	{
		return CObject :: operator new ( nSize );
	}
	void operator delete (
		void * pMemory
		)
	{
		CObject :: operator delete ( pMemory );
	}
	void * operator new (
		size_t nSize,
		IMalloc * pMalloc,
		bool bThrowException
		);
	void operator delete (
		void * pMemory,
		IMalloc * pMalloc,
		bool bThrowException
		);
#ifdef _DEBUG
	void * operator new [] ( // must never be used!
		size_t nSize
		);
	void operator delete [] ( // must never be used!
		void * pMemory
		);
	void * operator new [] ( // must never be used!
		size_t nSize,
		IMalloc * pMalloc,
		bool bThrowException
		);
	void operator delete [] ( // must never be used!
		void * pMemory,
		IMalloc * pMalloc,
		bool bThrowException
		);
#endif // _DEBUG
	static CExtGridCell * InstanceCreate(
		CRuntimeClass * pRTC,
		IMalloc * pMalloc,
		bool bThrowException = false
		);
	static CExtGridCell * InstanceCreate(
		const CExtGridCell & cell,
		bool bCopyData,
		IMalloc * pMalloc,
		bool bThrowException = false
		);
	static void InstanceDestroy(
		CExtGridCell * pCell,
		IMalloc * pMalloc,
		bool bThrowException = false
		);
	static CExtGridCell * InstanceLoad(
		CArchive & ar,
		IMalloc * pMalloc,
		bool bThrowException = false
		);
	static bool InstanceSave(
		CExtGridCell & cell,
		CArchive & ar,
		bool bThrowException = false
		);

	virtual bool OnPaintBackground(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnPaintForeground(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnPaintText(
		const RECT & rcCellText,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnPaintRestArea(
		const RECT & rcCellRest,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual HFONT OnQueryCellFont(
		const CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		bool & bFontMustBeDestroyed,
		DWORD dwHelperPaintFlags = 0
		) const;
	virtual CSize OnMeasureTextSize(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcCellText,
		DWORD dwAreaFlags
		) const;
	virtual void OnPaintSortArrow(
		const RECT & rcSortArrow,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	enum e_button_type_t
	{
		__EBTT_ELLIPSIS = 0,
		__EBTT_DROPDOWN = 1,
		__EBTT_UPDOWN_UP = 2,
		__EBTT_UPDOWN_DOWN = 3,
		__EBTT_FILTER_APPLIED = 4,
		__EBTT_FILTER_UNAPPLIED = 5,
	};
	bool IsButtonEnabled(
		INT nButtonType, // e_button_type_t
		CExtGridWnd * pWndGrid = NULL,
		LONG nColNo = 0,
		LONG nRowNo = 0,
		INT nColType = 0,
		INT nRowType = 0
		) const;
	bool IsButtonPressed(
		INT nButtonType, // e_button_type_t
		CExtGridWnd * pWndGrid = NULL,
		LONG nColNo = 0,
		LONG nRowNo = 0,
		INT nColType = 0,
		INT nRowType = 0
		) const;
	bool IsButtonStayPressed(
		INT nButtonType, // e_button_type_t
		CExtGridWnd * pWndGrid = NULL,
		LONG nColNo = 0,
		LONG nRowNo = 0,
		INT nColType = 0,
		INT nRowType = 0
		) const;
	virtual void OnPaintButton(
		const RECT & rcButton,
		INT nButtonType, // e_button_type_t
		bool bPressed,
		bool bHovered,
		bool bFocused,
		bool bEnabled,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnPaintIcon(
		const CExtCmdIcon * pIcon,
		const RECT & rcIcon,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnPaintCheck(
		const RECT & rcCheck,
		bool bPressed,
		bool bHovered,
		bool bEnabled,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual CSize OnCalcCheckSize(
		bool bPressed,
		bool bHovered,
		bool bEnabled,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnCalcLayout(
		DWORD dwCellStyle,
		DWORD dwCellStyleEx,
		CRect & rcCellRest,
		CRect & rcCellText,
		CRect & rcFocusArrow,
		CRect & rcIcon,
		CRect & rcCheck,
		CRect & rcSortArrow,
		CRect & rcUpdownButtonTop,
		CRect & rcUpdownButtonBottom,
		CRect & rcDropdownButton,
		CRect & rcEllipsisButton,
		CSize sizeFocusArrow,
		CSize sizeIcon,
		CSize sizeCheck,
		CSize sizeSortArrow,
		INT nExtentUpdownButton,
		INT nExtentDropdownButton,
		INT nExtentEllipsisButton,
		INT & nFocusGlyphType,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual CRect OnQueryRestAreaMargins(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual CRect OnQueryTextAreaMargins(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual CSize OnCalcSortArrowSize(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual CSize OnCalcFocusArrowSize(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual INT OnCalcButtonExtent(
		INT nButtonType, // e_button_type_t
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnFormatHeaderNumberString(
		CExtSafeString & strText,
		const CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags
		) const;
	virtual bool OnQueryVerticalLayout() const;
	virtual COLORREF OnQueryTextColor(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual COLORREF OnQueryBackColor(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual UINT OnQueryDrawTextFlags(
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags,
		bool bIncludeHorizontalFlags = true,
		bool bIncludeVerticalFlags = true,
		bool bIncludeOtherFlags = true
		) const;
	virtual void OnHitTestInfoAdjust(
		const CExtGridWnd & wndGrid,
		CExtGridHitTestInfo & htInfo,
		bool bComputeOuterDropAfterState
		) const;
	virtual void OnHover(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		bool bHoverEnter,
		bool bHoverLeave
		);
	virtual bool OnClick(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nRepCnt, // 0 - button up, 1 - single click, 2 - double click, 3 - post single click & begin editing
		UINT nFlags // mouse event flags
		);
	virtual void OnSetCheck(
		INT nCheck,
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell
		);
	virtual bool OnQueryButtonInfo(
		INT nButtonType,
		CExtGridWnd * pWndGrid = NULL,
		LONG nColNo = 0,
		LONG nRowNo = 0,
		INT nColType = 0,
		INT nRowType = 0,
		bool * p_bEnabled = NULL,
		bool * p_bPressed = NULL,
		bool * p_bStayPressed = NULL,
		UINT * p_nTimerElapseValue = NULL
		) const;
	virtual void OnButtonPressing(
		CExtGridWnd & wndGrid,
		INT nButtonType,
		bool bMouseInsideButton,
		ULONG nPressedTimerCounter,
		const RECT & rcButton,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual void OnButtonPressed(
		CExtGridWnd & wndGrid,
		INT nButtonType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual void OnChangeButtonPressedState(
		INT nButtonType,
		bool bPressed
		);
	virtual bool OnQueryEnabledIncrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnQueryEnabledDecrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnValueIncrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnValueDecrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnSysKey(
		CExtGridWnd & wndGrid,
		bool bKeyDownEvent, // true - sys-key-down event, false - sys-key-up event
		UINT nChar, // key code
		UINT nRepCnt, // sys-key-down/sys-key-up press count
		UINT nFlags // sys-key-down/sys-key-up event flags
		);
	virtual bool OnKey(
		CExtGridWnd & wndGrid,
		bool bKeyDownEvent, // true - key-down event, false - key-up event
		UINT nChar, // virtual key code
		UINT nRepCnt, // key-down/key-up press count
		UINT nFlags // key-down/key-up event flags
		);
	virtual bool OnSetCursor(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo
		);
	virtual bool OnQueryExtentPercent(
		double & lfExtentPercent,
		bool bGet
		);
	bool ExtentPercentGet(
		double & lfExtentPercent
		) const;
	bool ExtentPercentSet(
		double lfExtentPercent
		);
	virtual bool OnQueryAutoSizeInplaceControl() const;
	bool AutoSizeInplaceControlGet() const;
	void AutoSizeInplaceControlSet(
		bool bAutoSizeInplaceControl = true
		);
	virtual bool OnQueryExtent(
		INT & nItemExtent,
		bool bGet,
		INT nExtentType = 0 // -1 - min, 0 - curr, 1 - max
		);
	bool ExtentGet(
		INT & nItemExtent,
		INT nExtentType = 0 // -1 - min, 0 - curr, 1 - max
		) const;
	bool ExtentSet(
		INT nItemExtent,
		INT nExtentType = 0 // -1 - min, 0 - curr, 1 - max
		);
	virtual bool OnQueryExtraSpace(
		INT & nItemExtraSpace,
		bool bGet,
		bool bAfter // true - extra space after, false - after before
		);
	bool ExtraSpaceGet(
		INT & nItemExtraSpace,
		bool bAfter // true - extra space after, false - after before
		) const;
	bool ExtraSpaceSet(
		INT nItemExtraSpace,
		bool bAfter // true - extra space after, false - after before
		);
	virtual void OnGetToolTipText(
		const CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		CExtSafeString & strToolTipText
		) const;
	virtual bool OnInitAdvancedTip(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		CExtPopupMenuTipWnd & _ATTW,
		RECT & rcArea
		);
	virtual bool OnInitToolTip(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		CToolTipCtrl & wndToolTip,
		UINT nToolNo,
		const RECT & rcTool
		);
	virtual bool OnInitExpandWnd(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		CExtContentExpandWnd & wndContentExpand,
		const RECT & rcExpand,
		INT nSizeOfExpandShadow
		);
	virtual bool OnPaintExpandedContent(
		CExtContentExpandWnd::PAINT_ITEM_CONTENT_DATA & picd,
		const CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		CExtContentExpandWnd & wndContentExpand,
		INT nSizeOfExpandShadow
		) const;
	virtual INT IconIndexGet() const;
	virtual INT IconIndexSet(
		INT nIconIndex = -1 // -1 - remove icon
		);
	virtual INT FontIndexGet() const;
	virtual INT FontIndexSet(
		INT nFontIndex = -1 // -1 - remove font
		);
	virtual CExtCmdIcon * IconGet();
	const CExtCmdIcon * IconGet() const;
	virtual CSize IconGetSize() const;
	virtual HFONT FontGet();
	HFONT FontGet() const;

	virtual void OnInplaceControlAdjustCreationRect(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		RECT & rcInplaceControl, // RECT to adjust
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual bool OnInplaceControlWindowProc(
		LRESULT & lResult,
		UINT nMessage,
		WPARAM wParam,
		LPARAM lParam,
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl
		);
	virtual bool OnInplaceControlPreTranslateMessage(
		MSG * pMsg,
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl
		);
	virtual bool OnInplaceControlIdle(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nIdleCounter
		);
	virtual bool OnInplaceControlTextInputVerify(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		__EXT_MFC_SAFE_LPCTSTR sTextInitial,
		__EXT_MFC_SAFE_LPCTSTR sTextPrevious,
		CExtSafeString & sTextNew,
		bool bEndEdit
		) const;
	virtual bool OnInplaceControlDateTimeInputVerify(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		COleDateTime dtInitial,
		COleDateTime dtPrevious,
		COleDateTime & dtNew,
		bool bEndEdit
		) const;
	virtual bool OnInplaceControlDurationInputVerify(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		COleDateTimeSpan dtSpanInitial,
		COleDateTimeSpan dtSpanPrevious,
		COleDateTimeSpan & dtSpanNew,
		bool bEndEdit
		) const;
	virtual bool OnInplaceControlHotKeyInputVerify(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwHotKeyInitial,
		DWORD dwHotKeyPrevious,
		DWORD & dwHotKeyNew,
		bool bEndEdit
		) const;
	virtual bool OnInplaceControlSliderInputVerify(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		INT nPosInitial,
		INT nPosPrevious,
		INT & nPosNew,
		bool bEndEdit
		) const;
	virtual bool OnInplaceControlIPAddressInputVerify(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwIPAddressInitial,
		DWORD dwIPAddressPrevious,
		DWORD & dwIPAddressNew,
		bool bEndEdit
		) const;
	virtual void OnInplaceControlTextInputComplete(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		__EXT_MFC_SAFE_LPCTSTR sTextNew,
		bool bSaveChanges
		);
	virtual void OnInplaceControlDateTimeInputComplete(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		COleDateTime dtNew,
		bool bSaveChanges
		);
	virtual void OnInplaceControlDurationInputComplete(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		COleDateTimeSpan dtSpanNew,
		bool bSaveChanges
		);
	virtual void OnInplaceControlHotKeyInputComplete(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwHotKeyNew,
		bool bSaveChanges
		);
	virtual void OnInplaceControlSliderInputComplete(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		INT nPosNew,
		bool bSaveChanges
		);
	virtual void OnInplaceControlIPAddressInputComplete(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwIPAddressNew,
		bool bSaveChanges
		);
	virtual void OnInplaceControlQueryStyle(
		CExtGridWnd & wndGrid,
		CWnd * pWndInplaceControlToCreate,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		bool & bReadOnly,
		bool & bPassword,
		COLORREF & clrBack,
		COLORREF & clrText
		);
	virtual void OnQueryTextForInplaceControl( 
		CExtSafeString & strCopy 
		) const;
	
	virtual __EXT_MFC_SAFE_TCHAR OnQueryPasswordChar(
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnParseAutoEditStartText(
		CExtSafeString & sTextAutoEditStart
		) const;
	virtual HRESULT OnParseText(
		__EXT_MFC_SAFE_LPCTSTR sText
		) const;
	struct __PROF_UIS_API TrackCellStateInfo_t
	{
		CExtGridCell & m_cell;
		CExtGridWnd & m_wndGrid;
		INT m_nButtonType;
		CRect m_rcCellExtra, m_rcCell;
		LONG m_nVisibleColNo, m_nVisibleRowNo;
		LONG m_nColNo, m_nRowNo;
		INT m_nColType, m_nRowType;
		DWORD m_dwListBoxStyles;
		bool m_bHelperSelChanged:1, m_bAdjustListBoxWidth:1, m_bAdjustListBoxHeight:1,
			m_bAlingListBoxWidthToCellWidthMin:1, m_bAlingListBoxWidthToCellWidthMax:1;
		LONG m_nItemNoSelEndOK, m_nItemNoSelChange, m_nItemNoSelCancel;
		TrackCellStateInfo_t(
			CExtGridCell & cell,
			CExtGridWnd & wndGrid,
			INT nButtonType,
			const RECT & rcCellExtra,
			const RECT & rcCell,
			LONG nVisibleColNo,
			LONG nVisibleRowNo,
			LONG nColNo,
			LONG nRowNo,
			INT nColType,
			INT nRowType,
			DWORD dwListBoxStyles = WS_CHILD|WS_VISIBLE|WS_VSCROLL|LBS_NOINTEGRALHEIGHT|LBS_HASSTRINGS|LBS_OWNERDRAWVARIABLE
			)
			: m_cell( cell )
			, m_wndGrid( wndGrid )
			, m_nButtonType( nButtonType )
			, m_rcCellExtra( rcCellExtra )
			, m_rcCell( rcCell )
			, m_nVisibleColNo( nVisibleColNo )
			, m_nVisibleRowNo( nVisibleRowNo )
			, m_nColNo( nColNo )
			, m_nRowNo( nRowNo )
			, m_nColType( nColType )
			, m_nRowType( nRowType )
			, m_dwListBoxStyles( dwListBoxStyles )
			, m_bHelperSelChanged( false )
			, m_bAdjustListBoxWidth( true )
			, m_bAdjustListBoxHeight( true )
			, m_bAlingListBoxWidthToCellWidthMin( true )
			, m_bAlingListBoxWidthToCellWidthMax( false )
			, m_nItemNoSelEndOK( -1L )
			, m_nItemNoSelChange( -1L )
			, m_nItemNoSelCancel( -1L )
		{
		}
		~TrackCellStateInfo_t()
		{
		}
	}; // struct TrackCellStateInfo_t
	virtual bool OnButtonPopupMenuTrack(
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnButtonPopupMenuComplete(
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	static bool stat_CbInitListBoxContent(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	static CSize stat_CbAdjustListBoxSize(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	static bool stat_CbListBoxSelection(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		int eSAT // CExtPopupInplaceListBox::e_sel_action_t
		);
	static bool stat_CbListBoxItemClick(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	static bool stat_CbListBoxItemDraw(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LPDRAWITEMSTRUCT pDIS
		);
	static bool stat_CbListBoxItemMeasure(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LPMEASUREITEMSTRUCT pMIS
		);
	virtual CSize OnPopupListBoxCalcItemExtraSizes(
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LONG nItemNo
		) const;
	virtual CSize OnPopupListBoxMeasureTrackSize(
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		CExtPopupListBoxMenuWnd * pPopup
		);
	virtual CSize OnPopupListBoxQueryMaxSize(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual CSize OnPopupListBoxAdjustSize(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnPopupListBoxInitContent(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnPopupListBoxSelChange(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnPopupListBoxSelEndOK(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnPopupListBoxSelEndCancel(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnPopupListBoxClose(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnPopupListBoxItemClick(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnPopupListBoxItemDraw(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LPDRAWITEMSTRUCT pDIS
		);
	virtual bool OnPopupListBoxItemMeasure(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LPMEASUREITEMSTRUCT pMIS
		);
	virtual bool OnCellPressingEnter(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nFlags // mouse event flags
		);
	virtual void OnCellPressingStateChanged(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nFlags, // mouse event flags
		bool bMouseInsideCell
		);
	virtual bool OnCellPressingLeave(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nFlags, // mouse event flags
		bool bPressedEvent,
		bool bStartDragEvent
		);
	virtual bool OnOuterDndTrack(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nFlags // mouse event flags
		);
	virtual bool OnOuterDndComplete(
		CExtGridWnd & wndGrid,
		bool bDraggedCell, // true - dragged cell, false - drop-target cell
		const CExtGridHitTestInfo & htInfoDrag,
		const CExtGridHitTestInfo & htInfoDrop
		);
	virtual bool OnOuterDndOut(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo
		);

	virtual bool HiddenInPrintPreviewGet() const;
	virtual void HiddenInPrintPreviewSet( bool bHiddenInPrintPreview );

	// NOTE: To use user-defined colors you should apply 
	// the CExtGCC template for the cell class.
	// CExtGCC is stores the colors data.
	enum e_cell_state_t
	{
		__ECS_ALL						= 0, // color in any state
		__ECS_NORMAL					= 1, // normal, not hovered, not selected, not highlighted
		__ECS_SELECTED					= 2, // selected
		__ECS_HOVERED					= 3, // hovered
		// next added in 2.83
		__ECS_READ_ONLY					= 4, // normal, but read-only
		__ECS_HIGHLIGHT_PRESSING		= 5, // pressed
		__ECS_HIGHLIGHT_BY_FOCUS		= 6, // in the same row or column with focused
		__ECS_HIGHLIGHT_BY_SELECTION	= 7, // in the same row or column with selected
		__ECS_HIGHLIGHT_BY_HOVER		= 8, // in the same row or column with hovered
		// total count of supported colors
		__ECS_COUNT						= 9,
	};
	virtual COLORREF TextColorSet(
		e_cell_state_t eCellState,
		COLORREF clr = COLORREF( -1L )
		);
	virtual COLORREF TextColorGet(
		e_cell_state_t eCellState
		) const;
	virtual COLORREF BackColorSet(
		e_cell_state_t eCellState,
		COLORREF clr = COLORREF( -1L )
		);
	virtual COLORREF BackColorGet(
		e_cell_state_t eCellState
		) const;

	virtual void ResetColors(
		bool bResetTextColors = true,
		bool bResetBackgroundColors = true
		);
	virtual void AssignColors(
		const CExtGridCell & _other,
		bool bAssignTextColors = true,
		bool bAssignBackgroundColors = true
		);
	virtual INT CompareColors(
		const CExtGridCell & _other,
		bool bCompareTextColors = true,
		bool bCompareBackgroundColors = true
		) const;
	virtual void SerializeColors(
		CArchive & ar,
		bool bSerializeTextColors = true,
		bool bSerializeBackgroundColors = true
		);

	virtual CSize JoinSet( CSize sizeJoin );
	virtual CSize JoinGet() const;
	virtual void ResetJoin();
	virtual void AssignJoin( const CExtGridCell & _other );
	virtual INT CompareJoin( const CExtGridCell & _other ) const;
	virtual void SerializeJoin( CArchive & ar );
	virtual CSize OnJoinQueryInfo(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType = 0,
		INT nRowType = 0
		) const;

	// font parameters
	virtual void FontHeightSet(
		LONG nHeight
		);
	virtual bool FontHeightGet(
		LONG & nHeight
		) const;
	virtual void FontHeightSetDefault();

	virtual void FontWidthSet(
		LONG nWidth
		);
	virtual bool FontWidthGet(
		LONG & nWidth
		) const;
	virtual void FontWidthSetDefault();

	virtual void FontWeightSet(
		LONG nWeight
		);
	virtual bool FontWeightGet(
		LONG & nWeight
		) const;
	virtual void FontWeightSetDefault();
  
	virtual void FontItalicSet(
		bool bItalic = true
		);
	virtual bool FontItalicGet(
		bool & bItalic
		) const;
	virtual void FontItalicSetDefault();
  
	virtual void FontUnderlineSet(
		bool bUnderline = true
		);
	virtual bool FontUnderlineGet(
		bool & bUnderline
		) const;
 	virtual void FontUnderlineSetDefault();
  
	virtual void FontStrikeOutSet(
		bool bStrikeOut = true
		);
	virtual bool FontStrikeOutGet(
		bool & bStrikeOut
		) const;
	virtual void FontStrikeOutSetDefault();

	virtual void FontCharsetSet(
		BYTE nCharSet
		);
	virtual bool FontCharsetGet(
		BYTE & nCharSet
		) const;
	virtual void FontCharsetSetDefault();

	virtual void FontQualitySet(
		BYTE nQuality
		);
	virtual bool FontQualityGet(
		BYTE & nQuality
		) const;
	virtual void FontQualitySetDefault();

	virtual void FontFaceNameSet(
		__EXT_MFC_SAFE_LPCTSTR sFaceName
		);
	virtual bool FontFaceNameGet(
		CExtSafeString & sFaceName
		) const;
	virtual void FontFaceNameSetDefault();

	virtual CSize MeasureCell(
		CExtGridWnd * pWndGrid, // can be NULL
		CDC & dcMeasure,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual CSize MeasureCell(
		CExtGridWnd * pWndGrid, // can be NULL
		CDC & dcMeasure
		) const;
	CSize MeasureCell(
		CExtGridWnd * pWndGrid // can be NULL
		) const;
	virtual INT OnMeasureBestFitExtent(
		CExtGridWnd * pWndGrid, // can be NULL
		CDC & dcMeasure,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		bool bMeasuringColumnExtent
		) const;


protected:
	virtual CSize _CalcMaxCheckBoxSize(
		CDC * pDCMeasure,
		const CExtGridWnd & wndGrid
		) const;
}; // class CExtGridCell

#define IMPLEMENT_ExtGridCell_Clone( __derived__ , __base__ ) \
	virtual CExtGridCell * Clone( \
		IMalloc * pMalloc = NULL \
		) const \
	{ \
		ASSERT_VALID( this ); \
		if( pMalloc == NULL ) \
			return __base__ :: Clone( pMalloc ); \
		CExtGridCell * pOther = \
				new ( pMalloc, false ) __derived__ \
				(	(	const_cast \
						< __derived__ * > \
						( this ) \
					) \
					-> DataProviderGet() \
				); \
		ASSERT( pOther != NULL ); \
		if( pOther != NULL ) \
		{ \
			ASSERT_VALID( pOther ); \
			pOther->Assign( *this ); \
			ASSERT( pOther->DataProviderGet() == DataProviderGet() ); \
		} \
		return pOther; \
	}

/////////////////////////////////////////////////////////////////////////////
// CExtGCC
// template for adding coloring support to other cell types

template < class _BC >
class CExtGCC : public _BC
{
protected:
	COLORREF m_arrColorsBackground[CExtGridCell::__ECS_COUNT], m_arrColorsText[CExtGridCell::__ECS_COUNT];
public:
	CExtGCC(
		CExtGridDataProvider * pDataProvider = NULL
		)
		: _BC( pDataProvider )
	{
		ASSERT_VALID( this );
		ResetColors();
	}

	CExtGCC( const CExtGridCell & other )
	{
		ResetColors();
		Assign( other );
	}

#ifdef _DEBUG
	virtual void AssertValid() const
	{
		ASSERT( this != NULL );
		ASSERT( IsKindOf(RUNTIME_CLASS(CExtGridCell)) );
		_BC::AssertValid();
	}
#endif // _DEBUG

	virtual void Assign( const CExtGridCell & other )
	{
		ASSERT_VALID( this );
		_BC::Assign( other );
		AssignColors( other );
	}
	
	virtual CExtGridCell * Clone(
		IMalloc * pMalloc = NULL
		) const
	{
		ASSERT_VALID( this );
		if( pMalloc == NULL )
		{
			try
			{
				CExtGridCell * pOther =
					new CExtGCC < _BC >
						(	(	const_cast
								< CExtGCC < _BC > * >
								( this )
							)
							-> DataProviderGet()
						);
				ASSERT( pOther != NULL );
				if( pOther != NULL )
				{
					ASSERT_VALID( pOther );
					pOther->Assign( *this );
					ASSERT( pOther->DataProviderGet() == DataProviderGet() );
				} // if( pOther != NULL )
				return pOther;
			} // try
			catch( CException * pException )
			{
				ASSERT( FALSE );
				pException->Delete();
			} // catch( CException * pException )
			catch( ... )
			{
				ASSERT( FALSE );
			} // catch( ... )
			return NULL;
		} // if( pMalloc == NULL )
		CExtGridCell * pOther =
			new (pMalloc, false) CExtGCC < _BC >
				(	(	const_cast
						< CExtGCC < _BC > * >
						( this )
					)
					-> DataProviderGet()
				);
		ASSERT( pOther != NULL );
		if( pOther != NULL )
		{
			ASSERT_VALID( pOther );
			pOther->Assign( *this );
			ASSERT( pOther->DataProviderGet() == DataProviderGet() );
		} // if( pOther != NULL )
		return pOther;
	}

	virtual void Serialize( CArchive & ar )
	{
		ASSERT_VALID( this );
		_BC::Serialize( ar );
		SerializeColors( ar );
	}

	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const
	{
		ASSERT_VALID( this );
		INT nRetVal = _BC::Compare( other, dwStyleMask, dwStyleExMask );
		if( nRetVal != 0 )
			return nRetVal;
		nRetVal = CompareColors( other );
		return nRetVal;
	}
	
	virtual COLORREF TextColorSet(
		CExtGridCell::e_cell_state_t eCellState,
		COLORREF clr = COLORREF( -1L )
		)
	{
		ASSERT_VALID( this );
		COLORREF clrOld = COLORREF( -1L );
		if( 0 <= INT(eCellState) && INT(eCellState) < INT(__ECS_COUNT) )
		{
			clrOld = m_arrColorsText[ INT(eCellState) ];
			m_arrColorsText[ INT(eCellState) ] = clr;
		}
#ifdef _DEBUG
		else
		{
			ASSERT( FALSE );
		}
#endif // _DEBUG
		return clrOld;
	}

	virtual COLORREF TextColorGet(
		CExtGridCell::e_cell_state_t eCellState
		) const
	{
		ASSERT_VALID( this );
		if( 0 <= INT(eCellState) && INT(eCellState) < INT(__ECS_COUNT) )
			return m_arrColorsText[ INT(eCellState) ];
		ASSERT( FALSE );
		return COLORREF( -1L );
	}

	virtual COLORREF BackColorSet(
		CExtGridCell::e_cell_state_t eCellState,
		COLORREF clr = COLORREF( -1L )
		)
	{
		ASSERT_VALID( this );
		COLORREF clrOld = COLORREF( -1L );
		if( 0 <= INT(eCellState) && INT(eCellState) < INT(__ECS_COUNT) )
		{
			clrOld = m_arrColorsBackground[ INT(eCellState) ];
			m_arrColorsBackground[ INT(eCellState) ] = clr;
		}
#ifdef _DEBUG
		else
		{
			ASSERT( FALSE );
		}
#endif // _DEBUG
		return clrOld;
	}

	virtual COLORREF BackColorGet(
		CExtGridCell::e_cell_state_t eCellState
		) const
	{
		ASSERT_VALID( this );
		if( 0 <= INT(eCellState) && INT(eCellState) < INT(__ECS_COUNT) )
			return m_arrColorsBackground[ INT(eCellState) ];
		ASSERT( FALSE );
		return COLORREF( -1L );
	}
}; // class CExtGCC

/////////////////////////////////////////////////////////////////////////////
// CExtGCJ
// template for adding joining support to other cell types

template < class _BC >
class CExtGCJ : public _BC
{
	CSize m_sizeJoin;
public:
	CExtGCJ( CExtGridDataProvider * pDataProvider = NULL )
		: _BC( pDataProvider )
	{
		ASSERT_VALID( this );
		ResetJoin();
	}
	CExtGCJ( const CExtGridCell & other )
	{
		ResetJoin();
		Assign( other );
	}
#ifdef _DEBUG
	virtual void AssertValid() const
	{
		ASSERT( this != NULL );
		ASSERT( IsKindOf( RUNTIME_CLASS(CExtGridCell) ) );
		_BC::AssertValid();
	}
#endif // _DEBUG
	virtual void Assign( const CExtGridCell & other )
	{
		ASSERT_VALID( this );
		_BC::Assign( other );
		AssignJoin( other );
	}
	virtual CExtGridCell * Clone(
		IMalloc * pMalloc = NULL
		) const
	{
		ASSERT_VALID( this );
		if( pMalloc == NULL )
		{
			try
			{
				CExtGridCell * pOther =
					new CExtGCJ < _BC >
						(	(	const_cast
								< CExtGCJ < _BC > * >
								( this )
							)
							-> DataProviderGet()
						);
				ASSERT( pOther != NULL );
				if( pOther != NULL )
				{
					ASSERT_VALID( pOther );
					pOther->Assign( *this );
					ASSERT( pOther->DataProviderGet() == DataProviderGet() );
				} // if( pOther != NULL )
				return pOther;
			} // try
			catch( CException * pException )
			{
				ASSERT( FALSE );
				pException->Delete();
			} // catch( CException * pException )
			catch( ... )
			{
				ASSERT( FALSE );
			} // catch( ... )
			return NULL;
		} // if( pMalloc == NULL )
		CExtGridCell * pOther =
			new (pMalloc, false) CExtGCJ < _BC >
				(	(	const_cast
						< CExtGCJ < _BC > * >
						( this )
					)
					-> DataProviderGet()
				);
		ASSERT( pOther != NULL );
		if( pOther != NULL )
		{
			ASSERT_VALID( pOther );
			pOther->Assign( *this );
			ASSERT( pOther->DataProviderGet() == DataProviderGet() );
		} // if( pOther != NULL )
		return pOther;
	}
	virtual void Serialize( CArchive & ar )
	{
		ASSERT_VALID( this );
		_BC::Serialize( ar );
		SerializeJoin( ar );
	}
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const
	{
		ASSERT_VALID( this );
		INT nRetVal = _BC::Compare( other, dwStyleMask, dwStyleExMask );
// no reason to compare join
//		if( nRetVal != 0 )
//			return nRetVal;
//		nRetVal = CompareJoin( other );
		return nRetVal;
	}
	virtual CSize JoinSet( CSize sizeJoin )
	{
		ASSERT_VALID( this );
		CSize sizeOldJoin = m_sizeJoin;
		m_sizeJoin = sizeJoin;
		return sizeOldJoin;
	}
	virtual CSize JoinGet() const
	{
		ASSERT_VALID( this );
		return m_sizeJoin;
	}
}; // class CExtGCJ

/////////////////////////////////////////////////////////////////////////////
// CExtGCF
// template for adding custom font support to other cell types

template < class _BC >
class CExtGCF : public _BC
{
public:
	CExtGCF(
		CExtGridDataProvider * pDataProvider = NULL
		)
		: _BC( pDataProvider )
		, m_nHeight( -1L )
		, m_bHeightDefault( true )
		, m_nWidth( -1L )
		, m_bWidthDefault( true )
		, m_nWeight( -1L )
		, m_bWeightDefault( true )
		, m_nItalic( -1 )
		, m_nUnderline( -1 )
		, m_nStrikeOut( -1 )
		, m_nCharSet( -1 )
		, m_nQuality( -1 )
		, m_sFaceName( _T("") )
		, m_bFaceNameDefault( true )
	{
		ASSERT_VALID( this );
	}

	CExtGCF( const CExtGridCell & other )
	{
		Assign( other );
	}

#ifdef _DEBUG
	virtual void AssertValid() const
	{
		ASSERT( this != NULL );
		ASSERT( IsKindOf(RUNTIME_CLASS(CExtGridCell)) );
		_BC::AssertValid();
	}
#endif // _DEBUG
	
	virtual void Assign( const CExtGridCell & other )
	{
		ASSERT_VALID( this );
		_BC::Assign( other );

		FontHeightSetDefault();
		FontWidthSetDefault();
		FontWeightSetDefault();
		FontItalicSetDefault();
		FontUnderlineSetDefault();
		FontStrikeOutSetDefault();
		FontCharsetSetDefault();
		FontQualitySetDefault();
		FontFaceNameSetDefault();

		LONG nHeight = -1L;
		if( other.FontHeightGet( nHeight ) )
			FontHeightSet( nHeight );
		
		LONG nWidth = -1L;
		if( other.FontWidthGet( nWidth ) )
			FontWidthSet( nWidth );
		
		LONG nWeight = -1L;
		if( other.FontWeightGet( nWeight ) )
			FontWeightSet( nWeight );
		
		bool bItalic = false;
		if( other.FontItalicGet( bItalic ) )
			FontItalicSet( bItalic );
		
		bool bUnderline = false;
		if( other.FontUnderlineGet( bUnderline ) )
			FontUnderlineSet( bUnderline );
		
		bool bStrikeOut = false;
		if( other.FontStrikeOutGet( bStrikeOut ) )
			FontStrikeOutSet( bStrikeOut );
		
		BYTE nCharSet = 0;
		if( other.FontCharsetGet( nCharSet ) )
			FontCharsetSet( nCharSet );
		
		BYTE nQuality = 0;
		if( other.FontQualityGet( nQuality ) )
			FontQualitySet( nQuality );
		
		CExtSafeString sFaceName;
		if( other.FontFaceNameGet( sFaceName ) )
			FontFaceNameSet( sFaceName );
	}

	virtual CExtGridCell * Clone(
		IMalloc * pMalloc = NULL
		) const
	{
		ASSERT_VALID( this );
		if( pMalloc == NULL )
		{
			try
			{
				CExtGridCell * pOther =
					new CExtGCF < _BC >
						(	(	const_cast
								< CExtGCF < _BC > * >
								( this )
							)
							-> DataProviderGet()
						);
				ASSERT( pOther != NULL );
				if( pOther != NULL )
				{
					ASSERT_VALID( pOther );
					pOther->Assign( *this );
					ASSERT( pOther->DataProviderGet() == DataProviderGet() );
				} // if( pOther != NULL )
				return pOther;
			} // try
			catch( CException * pException )
			{
				ASSERT( FALSE );
				pException->Delete();
			} // catch( CException * pException )
			catch( ... )
			{
				ASSERT( FALSE );
			} // catch( ... )
			return NULL;
		} // if( pMalloc == NULL )
		CExtGridCell * pOther =
			new (pMalloc, false) CExtGCF < _BC >
				(	(	const_cast
						< CExtGCF < _BC > * >
						( this )
					)
					-> DataProviderGet()
				);
		ASSERT( pOther != NULL );
		if( pOther != NULL )
		{
			ASSERT_VALID( pOther );
			pOther->Assign( *this );
			ASSERT( pOther->DataProviderGet() == DataProviderGet() );
		} // if( pOther != NULL )
		return pOther;
	}
	
	virtual void Serialize( CArchive & ar )
	{
		ASSERT_VALID( this );
		_BC::Serialize( ar );
		if( ar.IsStoring() )
		{
			ar << BYTE(m_bHeightDefault);
			if( !m_bHeightDefault )
				ar << DWORD(m_nHeight);
			ar << BYTE(m_bWidthDefault);
			if( !m_bWidthDefault )
				ar << DWORD(m_nWidth);
			ar << BYTE(m_bWeightDefault);
			if( !m_bWeightDefault )
				ar << DWORD(m_nWeight);
			ar << m_nItalic;
			ar << m_nUnderline;
			ar << m_nStrikeOut;
			ar << m_nCharSet;
			ar << m_nQuality;
			ar << BYTE(m_bFaceNameDefault);
			if( !m_bFaceNameDefault )
				ar << m_sFaceName;
		} // if( ar.IsStoring() )
		else
		{
			FontHeightSetDefault();
			FontWidthSetDefault();
			FontWeightSetDefault();
			FontItalicSetDefault();
			FontUnderlineSetDefault();
			FontStrikeOutSetDefault();
			FontCharsetSetDefault();
			FontQualitySetDefault();
			FontFaceNameSetDefault();

			BYTE bTmp;
			DWORD dwTmp;
			ar >> bTmp;
			m_bHeightDefault = (bTmp > 0) ? true : false;
			if( !m_bHeightDefault )
			{
				ar >> dwTmp;
				m_nHeight = LONG(dwTmp);
			}
			ar >> bTmp;
			m_bWidthDefault = (bTmp > 0) ? true : false;
			if( !m_bWidthDefault )
			{
				ar >> dwTmp;
				m_nWidth = LONG(dwTmp);
			}
			ar >> bTmp;
			m_bWeightDefault = (bTmp > 0) ? true : false;
			if( !m_bWeightDefault )
			{
				ar >> dwTmp;
				m_nWeight = LONG(dwTmp);
			}
			ar >> m_nItalic;
			ar >> m_nUnderline;
			ar >> m_nStrikeOut;
			ar >> m_nCharSet;
			ar >> m_nQuality;
			ar >> bTmp;
			m_bFaceNameDefault = (bTmp > 0) ? true : false;
			if( !m_bFaceNameDefault )
				ar >> m_sFaceName;
		} // else from if( ar.IsStoring() )
	}

	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const
	{
		ASSERT_VALID( this );

		LONG nHeight1 = 0L, nHeight2 = 0L;
		FontHeightGet( nHeight1 );
		other.FontHeightGet( nHeight2 );
		if( nHeight1 < nHeight2 )
			return -1;
		if( nHeight1 > nHeight2 )
			return +1;
		
		LONG nWidth1 = 0L, nWidth2 = 0L;
		FontWidthGet( nWidth1 );
		other.FontWidthGet( nWidth2 );
		if( nWidth1 < nWidth2 )
			return -1;
		if( nWidth1 > nWidth2 )
			return +1;
		
		LONG nWeight1 = 0L, nWeight2 = 0L;
		FontWeightGet( nWeight1 );
		other.FontWeightGet( nWeight2 );
		if( nWeight1 < nWeight2 )
			return -1;
		if( nWeight1 > nWeight2 )
			return +1;
		
		bool bItalic1 = false, bItalic2 = false;
		FontItalicGet( bItalic1 );
		other.FontItalicGet( bItalic2 );
		if( (!bItalic1) && bItalic2 )
			return -1;
		if( bItalic1 && (!bItalic2) )
			return +1;
		
		bool bUnderline1 = false, bUnderline2 = false;
		FontUnderlineGet( bUnderline1 );
		other.FontUnderlineGet( bUnderline2 );
		if( (!bUnderline1) && bUnderline2 )
			return -1;
		if( bUnderline1 && (!bUnderline2) )
			return +1;
		
		bool bStrikeOut1 = false, bStrikeOut2 = false;
		FontStrikeOutGet( bStrikeOut1 );
		other.FontStrikeOutGet( bStrikeOut2 );
		if( (!bStrikeOut1) && bStrikeOut2 )
			return -1;
		if( bStrikeOut1 && (!bStrikeOut2) )
			return +1;
		
		BYTE nCharSet1 = 0, nCharSet2 = 0;
		FontCharsetGet( nCharSet1 );
		other.FontCharsetGet( nCharSet2 );
		if( nCharSet1 < nCharSet2 )
			return -1;
		if( nCharSet1 > nCharSet2 )
			return +1;
		
		BYTE nQuality1 = 0, nQuality2 = 0;
		FontQualityGet( nQuality1 );
		other.FontQualityGet( nQuality2 );
		if( nQuality1 < nQuality2 )
			return -1;
		if( nQuality1 > nQuality2 )
			return +1;
		
		CExtSafeString sFaceName1, sFaceName2;
		FontFaceNameGet( sFaceName1 );
		other.FontFaceNameGet( sFaceName2 );
		INT nRet = sFaceName1.Compare( sFaceName2 );
		if( nRet < 0 )
			return -1;
		if( nRet > 0 )
			return +1;
		
		return 
			_BC::Compare(
				other,
				dwStyleMask,
				dwStyleExMask
				);
	}
		
	HFONT OnQueryCellFont(
		const CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		bool & bFontMustBeDestroyed,
		DWORD dwHelperPaintFlags // = 0
		) const
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&wndGrid) );
		nVisibleColNo;
		nVisibleRowNo;
		wndGrid;
		HFONT hCellFont = 
			_BC::OnQueryCellFont(
				wndGrid,
				nVisibleColNo,
				nVisibleRowNo,
				nColNo,
				nRowNo,
				nColType,
				nRowType,
				dwAreaFlags,
				bFontMustBeDestroyed,
				dwHelperPaintFlags
				);

		LONG nHeight = 0L;
		LONG nWidth = 0L;
		LONG nWeight = 0L;
		bool bItalic = false;
		bool bUnderline = false;
		bool bStrikeOut = false;
		BYTE nCharSet = 0;
		BYTE nQuality = 0;
		CExtSafeString sFaceName;

		bool bHeightSpecified = FontHeightGet( nHeight );
		bool bWidthSpecified = FontWidthGet( nWidth );
		bool bWeightSpecified = FontWeightGet( nWeight );
		bool bItalicSpecified = FontItalicGet( bItalic );
		bool bUnderlineSpecified = FontUnderlineGet( bUnderline );
		bool bStrikeOutSpecified = FontStrikeOutGet( bStrikeOut );
		bool bCharSetSpecified = FontCharsetGet( nCharSet );
		bool bQualitySpecified = FontQualityGet( nQuality );
		bool bFaceNameSpecified = FontFaceNameGet( sFaceName );

		if(		(!bHeightSpecified)
			&&	(!bWidthSpecified)
			&&	(!bWeightSpecified)
			&&	(!bItalicSpecified)
			&&	(!bUnderlineSpecified)
			&&	(!bStrikeOutSpecified)
			&&	(!bCharSetSpecified)
			&&	(!bQualitySpecified)
			&&	(!bFaceNameSpecified)
			)
			return hCellFont;

		LOGFONT _lf;
		::memset( &_lf, 0, sizeof(LOGFONT) );

		if( hCellFont != NULL )
			::GetObject( 
				(HGDIOBJ) hCellFont, 
				sizeof( LOGFONT ), 
				(LPVOID) &_lf 
				);
		else
			wndGrid.OnSiwGetDefaultFont().GetLogFont( &_lf );

		if(		bFontMustBeDestroyed 
			&&	hCellFont != NULL 
			)
			::DeleteObject( hCellFont );

		if( bHeightSpecified )
			_lf.lfHeight = nHeight;
		if( bWidthSpecified )
			_lf.lfWidth = nWidth;
		if( bWeightSpecified )
			_lf.lfWeight = nWeight;
		if( bItalicSpecified )
			_lf.lfItalic = bItalic ? BYTE(1): BYTE(0);
		if( bUnderlineSpecified )
			_lf.lfUnderline = bUnderline ? BYTE(1) : BYTE(0);
		if( bStrikeOutSpecified )
			_lf.lfStrikeOut = bStrikeOut ? BYTE(1) : BYTE(0);
		_lf.lfCharSet = 
			(bCharSetSpecified && nCharSet >= 0) ? BYTE(nCharSet) : BYTE(DEFAULT_CHARSET);
		if( bQualitySpecified )
			_lf.lfQuality = nQuality;
		if( bFaceNameSpecified )
			__EXT_MFC_STRCPY( _lf.lfFaceName, LF_FACESIZE, sFaceName );

		HFONT hFont  = ::CreateFontIndirect( &_lf );
		ASSERT( hFont != NULL );

		bFontMustBeDestroyed = true;
		return hFont;
	}
		
	// font parameters
	virtual void FontHeightSet(
		LONG nHeight
		)
	{
		ASSERT_VALID( this );
		m_nHeight = nHeight;
		m_bHeightDefault = false;
	}
	virtual bool FontHeightGet(
		LONG & nHeight
		) const
	{
		ASSERT_VALID( this );
		if( m_bHeightDefault )
			return false;
		nHeight = m_nHeight;
		return true;
	}
	virtual void FontHeightSetDefault()
	{
		ASSERT_VALID( this );
		m_nHeight = -1L;
		m_bHeightDefault = true;
	}

	virtual void FontWidthSet(
		LONG nWidth
		)
	{
		ASSERT_VALID( this );
		m_nWidth = nWidth;
		m_bWidthDefault = false;
	}
	virtual bool FontWidthGet(
		LONG & nWidth
		) const
	{
		ASSERT_VALID( this );
		if( m_bWidthDefault )
			return false;
		nWidth = m_nWidth;
		return true;
	}
	virtual void FontWidthSetDefault()
	{
		ASSERT_VALID( this );
		m_nWidth = -1L;
		m_bWidthDefault = true;
	}

	virtual void FontWeightSet(
		LONG nWeight
		)
	{
		ASSERT_VALID( this );
		m_nWeight = nWeight;
		m_bWeightDefault = false;
	}
	virtual bool FontWeightGet(
		LONG & nWeight
		) const
	{
		ASSERT_VALID( this );
		if( m_bWeightDefault )
			return false;
		nWeight = m_nWeight;
		return true;
	}
	virtual void FontWeightSetDefault()
	{
		ASSERT_VALID( this );
		m_nWeight = -1L;
		m_bWeightDefault = true;
	}
  
	virtual void FontItalicSet(
		bool bItalic = true
		)
	{
		ASSERT_VALID( this );
		m_nItalic = bItalic ? SHORT(1) : SHORT(0);
	}
	virtual bool FontItalicGet(
		bool & bItalic
		) const
	{
		ASSERT_VALID( this );
		if( m_nItalic < 0 )
			return false;
		bItalic = ( m_nItalic != 0 ) ? true : false;
		return true;
	}
	virtual void FontItalicSetDefault()
	{
		ASSERT_VALID( this );
		m_nItalic = -1;
	}
  
	virtual void FontUnderlineSet(
		bool bUnderline = true
		)
	{
		ASSERT_VALID( this );
		m_nUnderline = bUnderline ? SHORT(1) : SHORT(0);
	}
	virtual bool FontUnderlineGet(
		bool & bUnderline
		) const
	{
		ASSERT_VALID( this );
		if( m_nUnderline < 0 )
			return false;
		bUnderline = ( m_nUnderline != 0 ) ? true : false;
		return true;
	}
	virtual void FontUnderlineSetDefault()
	{
		ASSERT_VALID( this );
		m_nUnderline = -1;
	}

	virtual void FontStrikeOutSet(
		bool bStrikeOut = true
		)
	{
		ASSERT_VALID( this );
		m_nStrikeOut = bStrikeOut ? SHORT(1) : SHORT(0);
	}
	virtual bool FontStrikeOutGet(
		bool & bStrikeOut
		) const
	{
		ASSERT_VALID( this );
		if( m_nStrikeOut < 0 )
			return false;
		bStrikeOut = ( m_nStrikeOut != 0 ) ? true : false;
		return true;
	}
	virtual void FontStrikeOutSetDefault()
	{
		ASSERT_VALID( this );
		m_nStrikeOut = -1;
	}

	virtual void FontCharsetSet(
		BYTE nCharSet
		)
	{
		ASSERT_VALID( this );
		m_nCharSet = nCharSet;
	}
	virtual bool FontCharsetGet(
		BYTE & nCharSet
		) const
	{
		ASSERT_VALID( this );
		if( m_nCharSet < 0 )
			return false;
		nCharSet = BYTE(m_nCharSet);
		return true;
	}
	virtual void FontCharsetSetDefault()
	{
		ASSERT_VALID( this );
		m_nCharSet = -1;
	}

	virtual void FontQualitySet(
		BYTE nQuality
		)
	{
		ASSERT_VALID( this );
		m_nQuality = nQuality;
	}
	virtual bool FontQualityGet(
		BYTE & nQuality
		) const
	{
		ASSERT_VALID( this );
		if( m_nQuality < 0 )
			return false;
		nQuality = BYTE(m_nQuality);
		return true;
	}
	virtual void FontQualitySetDefault()
	{
		ASSERT_VALID( this );
		m_nQuality = -1;
	}

	virtual void FontFaceNameSet(
		__EXT_MFC_SAFE_LPCTSTR sFaceName
		)
	{
		ASSERT_VALID( this );
		m_sFaceName = sFaceName;
		m_bFaceNameDefault = false;
	}
	virtual bool FontFaceNameGet(
		CExtSafeString & sFaceName
		) const
	{
		ASSERT_VALID( this );
		if( m_bFaceNameDefault )
			return false;
		sFaceName = m_sFaceName;
		return true;
	}
	virtual void FontFaceNameSetDefault()
	{
		ASSERT_VALID( this );
		m_sFaceName.Empty();
		m_bFaceNameDefault = true;
	}
	
protected:
    LONG m_nHeight;
    bool m_bHeightDefault:1;
    LONG m_nWidth;
	bool m_bWidthDefault:1;
	LONG m_nWeight;
	bool m_bWeightDefault:1;
    SHORT m_nItalic;
    SHORT m_nUnderline;
    SHORT m_nStrikeOut;
    SHORT m_nQuality;
    SHORT m_nCharSet;
    CExtSafeString m_sFaceName;
	bool m_bFaceNameDefault:1;

}; // class CExtGCF

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellEx

class __PROF_UIS_API CExtGridCellEx
	: public CExtGCJ < CExtGCF < CExtGCC < CExtGridCell > > >
{
public:
	DECLARE_SERIAL( CExtGridCellEx );
	//IMPLEMENT_ExtGridCell_Clone( CExtGridCellEx, CExtGCJ < CExtGCF < CExtGCC < CExtGridCell > > > );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellEx, CExtGridCell );
	CExtGridCellEx(
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellEx( const CExtGridCell & other );
	virtual ~CExtGridCellEx();

	void LimitTextSet( 
		LONG nMaxChars = -1L 
		);
	LONG LimitTextGet() const;

	bool SetCursor( INT nCursorID = -1 );
	bool SetCursor( HCURSOR hCursor = NULL );
	HCURSOR GetCursor() const;

	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	// virtual methods
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;
	virtual bool OnSetCursor(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo
		);
	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	
protected:
	LONG m_nMaxChars;
	HCURSOR m_hCursor;
}; // class CExtGridCellEx

/////////////////////////////////////////////////////////////////////////////
// CExtGIE

template < class _BIE >
class CExtGIE : public _BIE
{
public:
	CExtGIE(
		CExtGridWnd & wndGrid,
		CExtGridCell & cell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		)
		: _BIE()
		, m_wndGrid( wndGrid )
		, m_cell( cell )
		, m_nVisibleColNo( nVisibleColNo )
		, m_nVisibleRowNo( nVisibleRowNo )
		, m_nColNo( nColNo )
		, m_nRowNo( nRowNo )
		, m_nColType( nColType )
		, m_nRowType( nRowType )
		, m_rcCellExtra( rcCellExtra )
		, m_rcCell( rcCell )
		, m_rcInplaceControl( rcInplaceControl )
		, m_nLastEditedColNo( nLastEditedColNo )
		, m_nLastEditedRowNo( nLastEditedRowNo )
		, m_bInConstruction( false )
		, m_bHelperHasBorder( true )
		, m_bInEndEdit( false )
	{
		DWORD dwCellStyleEx = m_cell.GetStyleEx();
		m_bHelperHasBorder =
			( (dwCellStyleEx&__EGCS_EX_NO_INPLACE_CONTROL_BORDER) == 0 )
				? true : false ;

		m_rcNonClientMargins.SetRect( 3, 2, 2, 2 );
		m_rcInplaceControl.InflateRect( 1, 1, 1, 1 );
		m_rcInplaceControlInitial = m_rcInplaceControl;

		if( m_rcNonClientMargins.left < 0 )
			m_rcNonClientMargins.left = 0;
		if( m_rcNonClientMargins.right < 0 )
			m_rcNonClientMargins.right = 0;
		if( m_rcNonClientMargins.top < 0 )
			m_rcNonClientMargins.top = 0;
		if( m_rcNonClientMargins.bottom < 0 )
			m_rcNonClientMargins.bottom = 0;
	}

public:
	CExtGridWnd & m_wndGrid;
	CExtGridCell & m_cell;
	LONG m_nVisibleColNo, m_nVisibleRowNo,
		m_nColNo, m_nRowNo,
		m_nColType, m_nRowType;
protected:
	bool m_bInEndEdit:1;
	LONG m_nLastEditedColNo, m_nLastEditedRowNo;
	CRect m_rcCellExtra, m_rcCell, m_rcInplaceControl, m_rcInplaceControlInitial, m_rcNonClientMargins;
	CFont m_font;
	bool m_bHelperHasBorder:1, m_bInConstruction:1;

public:
	virtual BOOL PreTranslateMessage( MSG * pMsg )
	{
		ASSERT_VALID( this );
		if( m_bInEndEdit )
			return FALSE;
		if( m_cell.OnInplaceControlPreTranslateMessage(
				pMsg,
				m_hWnd,
				m_wndGrid,
				m_nVisibleColNo,
				m_nVisibleRowNo,
				m_nColNo,
				m_nRowNo,
				m_nColType,
				m_nRowType,
				m_rcCellExtra,
				m_rcCell,
				m_rcInplaceControl
				)
			)
			return TRUE;
		return _BIE::PreTranslateMessage( pMsg );
	}
	virtual void PostNcDestroy()
	{
		delete this;
	}

	virtual COLORREF GetBkColor() const = 0;

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if( message == WM_NCCALCSIZE )
		{
			NCCALCSIZE_PARAMS * pNCCSP =
				reinterpret_cast < NCCALCSIZE_PARAMS * > ( lParam );
			ASSERT( pNCCSP != NULL );
			CRect rcInBarWnd( pNCCSP->rgrc[0] );
			rcInBarWnd.DeflateRect(
				m_rcNonClientMargins.left,
				m_rcNonClientMargins.top,
				m_rcNonClientMargins.right,
				m_rcNonClientMargins.bottom
				);
			::CopyRect( &(pNCCSP->rgrc[0]), rcInBarWnd );
			return 0;
		} // if( message == WM_NCCALCSIZE )
		
		if( message == WM_NCPAINT )
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
			CWindowDC dc( this );
			ASSERT( dc.GetSafeHdc() != NULL );
			dc.ExcludeClipRect( &rcInBarClient );

			COLORREF clrBk = GetBkColor();
			COLORREF clrSysBk =
				m_wndGrid.OnSiwGetSysColor( COLOR_WINDOW );
			if( clrBk == COLORREF(-1L) )
				clrBk = clrSysBk;

			dc.FillSolidRect( &rcInBarWnd, clrBk );
			if( m_bHelperHasBorder )
			{
				COLORREF clrBorder =
					m_wndGrid.OnSiwGetSysColor(
						COLOR_3DDKSHADOW
						);
				dc.Draw3dRect( &rcInBarWnd, clrBorder, clrBorder );
			} // if( m_bHelperHasBorder )
			return 0;
		} // if( message == WM_NCPAINT )

		if( message == WM_CONTEXTMENU )
		{ // fix for grid's in-place editor
			CWnd * pWnd = NULL;
			if( wParam != 0 )
				pWnd = CWnd::FromHandle( (HWND)wParam );
			CPoint pos( short(LOWORD(lParam)), short(HIWORD(lParam)) );
			OnContextMenu( pWnd, pos );
			return 0;
		} // fix for grid's in-place editor

		return _BIE::WindowProc(message, wParam, lParam);
	}
}; // class CExtGIE

/////////////////////////////////////////////////////////////////////////////
// CExtGIED (for CExtGridInplaceDateTime and CExtGridInplaceDuration classes)

template < class _BIED >
class CExtGIED : public _BIED
{
public:
	CExtGIED(
		CExtGridWnd & wndGrid,
		CExtGridCell & cell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		)
		: _BIED( 
			wndGrid,
			cell,
			nVisibleColNo,
			nVisibleRowNo,
			nColNo,
			nRowNo,
			nColType,
			nRowType,
			rcCellExtra,
			rcCell,
			rcInplaceControl,
			nLastEditedColNo,
			nLastEditedRowNo
			)
	{
	}

	virtual COLORREF GetBkColor() const
	{ 
		ASSERT_VALID( this );
		return CExtDurationWnd::GetBkColor(); 
	}

	virtual void _DoEndEdit( bool bSaveChanges ) = 0;

public:
	virtual bool OnShowDropDownMenu()
	{
		ASSERT_VALID( this );
		return false;
	}
	virtual bool IsSpinButtonVisible() const
	{
		ASSERT_VALID( this );
		return false;
	}
	virtual bool IsDropDownButtonVisible() const
	{
		ASSERT_VALID( this );
		return false;
	}

protected:
	virtual CSize OnQueryBorderSize() const
	{
		ASSERT_VALID( this );
		return CSize( 0, 0 );
	}
	virtual CRect OnQueryItemMargins( const CExtDurationWnd::ITEM_INFO * pII ) const
	{
		ASSERT_VALID( this );
		ASSERT( pII != NULL );
		pII;
		return CRect( 0, 0, 0, 0 );
	}
	virtual HFONT OnQueryFont() const
	{
		ASSERT_VALID( this );
		HFONT hFont = (HFONT)m_font.GetSafeHandle();
		if( hFont != NULL )
			return hFont;
		return _BIED::OnQueryFont();
	}

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lResult = 0L;
		if( m_cell.OnInplaceControlWindowProc(
				lResult,
				message,
				wParam,
				lParam,
				m_hWnd,
				m_wndGrid,
				m_nVisibleColNo,
				m_nVisibleRowNo,
				m_nColNo,
				m_nRowNo,
				m_nColType,
				m_nRowType,
				m_rcCellExtra,
				m_rcCell,
				m_rcInplaceControl
				)
			)
			return lResult;

		if( message == WM_GETDLGCODE )
			return DLGC_WANTALLKEYS|DLGC_WANTCHARS|DLGC_WANTTAB;

		if( message == WM_KILLFOCUS )
		{
			_DoEndEdit( true );
			return 0L;
		}

		if( message == WM_ACTIVATEAPP )
		{
			if( !wParam )
				_DoEndEdit( false );
			return 0L;
		}

		if( message == WM_CANCELMODE || message == WM_ACTIVATEAPP )
		{
			_DoEndEdit( false );
			return 0L;
		}

		if( message == WM_KEYDOWN )
		{
			switch( int(wParam) )
			{
			case VK_RETURN:
				_DoEndEdit( true );
				return 0L;
			case VK_ESCAPE:
				_DoEndEdit( false );
				return 0L;
			case VK_TAB:
				{
					HWND hWndParent = GetParent()->GetSafeHwnd();
					if( hWndParent != NULL )
					{
						if( ( ::__EXT_MFC_GetWindowLong(hWndParent,GWL_STYLE) & WS_POPUP ) == 0 )
						{
							hWndParent = ::GetParent(hWndParent);
							if( hWndParent != NULL )
							{
								::SendMessage(
									hWndParent,
									WM_NEXTDLGCTL,
									CExtPopupMenuWnd::IsKeyPressed(VK_SHIFT),
									0
									);
							}
						}
					}
				} // case VK_TAB
				return 0L;
			case VK_UP:
			case VK_DOWN:
			case VK_LEFT:
			case VK_RIGHT:
				{
					if(		m_nColType != 0
						||	m_nRowType != 0
						||	CExtPopupMenuWnd::IsKeyPressed(VK_CONTROL)
						||	CExtPopupMenuWnd::IsKeyPressed(VK_SHIFT)
						)
						break;

					bool bTranslate = true;
					DWORD dwBseStyle = m_wndGrid.BseGetStyle();

					if( (dwBseStyle&__EGWS_BSE_WALK) == 0 )
						bTranslate = false;
					else if(	INT(wParam) == VK_UP
							||	INT(wParam) == VK_DOWN
						)
					{
						if(		(dwBseStyle&__EGWS_BSE_WALK_VERT) == 0
							||	m_wndGrid.SiwScrollTypeVGet() == __ESIW_ST_VIRTUAL
							)
							bTranslate = false;
					}
					else if(	INT(wParam) == VK_LEFT
							||	INT(wParam) == VK_RIGHT
						)
					{
						if(		(dwBseStyle&__EGWS_BSE_WALK_HORZ) == 0
							||	m_wndGrid.SiwScrollTypeHGet() == __ESIW_ST_VIRTUAL
							)
							bTranslate = false;
						else
						{
							ITEM_INFO * pII = SelectionGet();
							if( INT(wParam) == VK_LEFT )
							{
								if(		pII != NULL 
									&&	!IsFirstVisibleItem( pII )
									)
									bTranslate = false;
								
								else if( m_nColNo == 0L )
									bTranslate = false;
							}
							else if( INT(wParam) == VK_RIGHT )
							{
								if(		pII != NULL 
									&&	!IsLastVisibleItem( pII )
									)
									bTranslate = false;
								else
								{
									LONG nColCount =
										(m_nColType == 0)
											? m_wndGrid.ColumnCountGet()
											: (	(m_nColType < 0)
												? m_wndGrid.OuterColumnCountLeftGet()
												: m_wndGrid.OuterColumnCountRightGet()
												)
											;
									ASSERT( m_nColNo < nColCount );
									if( m_nColNo == (nColCount - 1) )
										bTranslate = false;
								}
							}
						}
					}
					if( bTranslate )
					{
						LRESULT lResult = 0L;
						HWND hWndParent = GetParent()->GetSafeHwnd();
						if( hWndParent != NULL )
						{
							HWND hWndOwn = m_hWnd;
							HWND hWndGrid = m_wndGrid.m_hWnd;
							CExtGridWnd * pWndGrid = &m_wndGrid;
							_DoEndEdit( true );
							lResult =
								::SendMessage(hWndParent,WM_KEYDOWN,wParam,lParam);
							if(		::IsWindow(hWndGrid)
								&&	( ! ::IsWindow(hWndOwn) )
								&&	CWnd::FromHandlePermanent(hWndGrid) == pWndGrid
								)
							{
								ASSERT_VALID( pWndGrid );
								pWndGrid->EditCell();
							}
						}
						return lResult;
					}
				} // case VK_...
			} // switch( INT(wParam) )
		} // if( message == WM_KEYDOWN )

		return _BIED::WindowProc(message, wParam, lParam);		
	}
}; // class CExtGIED

/////////////////////////////////////////////////////////////////////////////
// CExtGridInplaceEdit

#if _MFC_VER >= 0x700
	#ifndef __EXT_PROFUIS_STATIC_LINK
		template class __PROF_UIS_API CExtGIE < CExtEditBase >;
	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
#endif // _MFC_VER >= 0x700

class __PROF_UIS_API CExtGridInplaceEdit : public CExtGIE < CExtEditBase >
{
protected:
	bool m_bInValidation:1;
	bool m_bForceDisableInitSelText:1;
	CExtSafeString m_strTextPrevious, m_strTextInitial;
public:
	DECLARE_DYNAMIC( CExtGridInplaceEdit );
	CExtGridInplaceEdit(
		CExtGridWnd & wndGrid,
		CExtGridCell & cell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual ~CExtGridInplaceEdit();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtGridInplaceEdit)
	public:
	virtual bool Create();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

    //{{AFX_MSG(CExtGridInplaceEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	void _DoEndEdit( bool bSaveChanges );
	bool _DoVerifyTextInput( bool bEndEdit );

public:
	virtual COLORREF GetBkColor() const;

	virtual void DoEndEdit( bool bSaveChanges );

	virtual bool DoVerifyTextInput(
		__EXT_MFC_SAFE_LPCTSTR sTextInitial,
		__EXT_MFC_SAFE_LPCTSTR sTextPrevious,
		CExtSafeString & sTextNew,
		bool bEndEdit
		);

	void TextGet( 
		CExtSafeString & strTextPrevious, 
		CExtSafeString & strTextInitial 
		) const;

}; // class CExtGridInplaceEdit

/////////////////////////////////////////////////////////////////////////////
// CExtGridInplaceHotKey

#if _MFC_VER >= 0x700
	#ifndef __EXT_PROFUIS_STATIC_LINK
		template class __PROF_UIS_API CExtGIE < CExtEditHotKeyBase >;
	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
#endif // _MFC_VER >= 0x700

class __PROF_UIS_API CExtGridInplaceHotKey : public CExtGIE < CExtEditHotKeyBase >
{
protected:
	bool m_bInValidation:1;
	CExtSafeString m_strTextPrevious, m_strTextInitial;
public:
	DECLARE_DYNAMIC( CExtGridInplaceHotKey );
	CExtGridInplaceHotKey(
		CExtGridWnd & wndGrid,
		CExtGridCell & cell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual ~CExtGridInplaceHotKey();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtGridInplaceHotKey)
	public:
	virtual bool Create();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

    //{{AFX_MSG(CExtGridInplaceHotKey)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	void _DoEndEdit( bool bSaveChanges );
	bool _DoVerifyInput( bool bEndEdit );

public:
	virtual COLORREF GetBkColor() const
	{ 
		ASSERT_VALID( this );
		return CExtEditHotKeyBase::GetBkColor(); 
	}

	virtual void DoEndEdit( bool bSaveChanges );
	virtual bool DoVerifyInput(
		DWORD dwHotKeyInitial,
		DWORD dwHotKeyPrevious,
		DWORD & dwHotKeyNew,
		bool bEndEdit
		);

	DWORD m_dwHotKeyPrevious;
	DWORD m_dwHotKeyInitial;

}; // class CExtGridInplaceHotKey

/////////////////////////////////////////////////////////////////////////////
// CExtGridInplaceDateTime

#if _MFC_VER >= 0x700
	#ifndef __EXT_PROFUIS_STATIC_LINK
		template class __PROF_UIS_API CExtGIE < CExtDateTimeWnd >;
	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
#endif // _MFC_VER >= 0x700

#if _MFC_VER >= 0x700
	#ifndef __EXT_PROFUIS_STATIC_LINK
		template class __PROF_UIS_API CExtGIED < CExtGIE < CExtDateTimeWnd > >;
	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
#endif // _MFC_VER >= 0x700

class __PROF_UIS_API CExtGridInplaceDateTime : public CExtGIED < CExtGIE < CExtDateTimeWnd > >
{
public:
	DECLARE_DYNAMIC( CExtGridInplaceDateTime );
	CExtGridInplaceDateTime(
		CExtGridWnd & wndGrid,
		CExtGridCell & cell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual ~CExtGridInplaceDateTime();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

public:
	virtual void DoEndEdit( bool bSaveChanges );
	virtual bool DoVerifyTextInput(
		COleDateTime dtInitial,
		COleDateTime dtPrevious,
		COleDateTime & dtNew,
		bool bEndEdit
		);
	
protected:
	COleDateTime m_dtPrevious, m_dtInitial;

protected:
	void _DoEndEdit( bool bSaveChanges );
	bool _DoVerifyInput( bool bEndEdit );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtGridInplaceDateTime)
	public:
	virtual bool Create();
	//}}AFX_VIRTUAL

    //{{AFX_MSG(CExtGridInplaceDateTime)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

}; // class CExtGridInplaceDateTime

/////////////////////////////////////////////////////////////////////////////
// CExtGridInplaceDuration

#if _MFC_VER >= 0x700
	#ifndef __EXT_PROFUIS_STATIC_LINK
		template class __PROF_UIS_API CExtGIE < CExtDurationWnd >;
	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
#endif // _MFC_VER >= 0x700

#if _MFC_VER >= 0x700
	#ifndef __EXT_PROFUIS_STATIC_LINK
		template class __PROF_UIS_API CExtGIED < CExtGIE < CExtDurationWnd > >;
	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
#endif // _MFC_VER >= 0x700

class __PROF_UIS_API CExtGridInplaceDuration : public CExtGIED < CExtGIE < CExtDurationWnd > >
{
public:
	DECLARE_DYNAMIC( CExtGridInplaceDuration );
	CExtGridInplaceDuration(
		CExtGridWnd & wndGrid,
		CExtGridCell & cell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual ~CExtGridInplaceDuration();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

	virtual void DoEndEdit( bool bSaveChanges );
	virtual bool DoVerifyInput(
		COleDateTimeSpan dtSpanInitial,
		COleDateTimeSpan dtSpanPrevious,
		COleDateTimeSpan & dtSpanNew,
		bool bEndEdit
		);
	
	COleDateTimeSpan m_dtSpanPrevious, m_dtSpanInitial;

protected:
	virtual void _DoEndEdit( bool bSaveChanges );
	bool _DoVerifyInput( bool bEndEdit );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtGridInplaceDuration)
	public:
	virtual bool Create();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExtGridInplaceDuration)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
}; // class CExtGridInplaceDuration

/////////////////////////////////////////////////////////////////////////////
// CExtGridInplaceEditIPAddress

class __PROF_UIS_API CExtGridInplaceEditIPAddress : public CExtGIE < CIPAddressCtrl >
{
public:
	DECLARE_DYNAMIC( CExtGridInplaceEditIPAddress );
		
	CExtGridInplaceEditIPAddress(
		CExtGridWnd & wndGrid,
		CExtGridCell & cell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);

public:
	virtual COLORREF GetBkColor() const
	{ 
		ASSERT_VALID( this );
		return COLORREF( -1L ); 
	}

	virtual void DoEndEdit( bool bSaveChanges );
	virtual bool DoVerifyTextInput(
		DWORD dwIPAddressInitial,
		DWORD dwIPAddressPrevious,
		DWORD & dwIPAddressNew,
		bool bEndEdit
		);
	
protected:
	DWORD m_dwIPAddressPrevious, m_dwIPAddressInitial;
	
	WNDPROC m_pWNDPROC[4];
	HWND m_hWndHooked[4];
	static CExtGridInplaceEditIPAddress * g_pHookTarget;

protected:
	void _DoEndEdit( bool bSaveChanges );
	bool _DoVerifyInput( bool bEndEdit );

	static LRESULT CALLBACK g_HookWndProc(
		HWND hWnd,
		UINT nMessage,
		WPARAM wParam,
		LPARAM lParam
		);

public:
	virtual bool Create();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtGridInplaceEditIPAddress)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

    //{{AFX_MSG(CExtGridInplaceEditIPAddress)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

}; // class CExtGridInplaceEditIPAddress

/////////////////////////////////////////////////////////////////////////////
// CExtGridInplaceSlider

#if (!defined __EXT_MFC_NO_SLIDER)
	#define __BASEOF_CExtGridInplaceSlider__ CExtGIE < CExtSliderWnd >
#else
	#define __BASEOF_CExtGridInplaceSlider__ CExtGIE < CSliderCtrl >
#endif

#if _MFC_VER >= 0x700
	#ifndef __EXT_PROFUIS_STATIC_LINK
		template class __PROF_UIS_API __BASEOF_CExtGridInplaceSlider__;
	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
#endif // _MFC_VER >= 0x700

class __PROF_UIS_API CExtGridInplaceSlider : public __BASEOF_CExtGridInplaceSlider__
{
public:
	DECLARE_DYNAMIC( CExtGridInplaceSlider );
	CExtGridInplaceSlider(
		CExtGridWnd & wndGrid,
		CExtGridCell & cell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual ~CExtGridInplaceSlider();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

public:
	virtual COLORREF GetBkColor() const
	{ 
		ASSERT_VALID( this );
		return CExtSliderWnd::GetBkColor(); 
	}

	virtual void DoEndEdit( bool bSaveChanges );
	virtual bool DoVerifyTextInput(
		INT nPosInitial,
		INT nPosPrevious,
		INT & nPosNew,
		bool bEndEdit
		);
	
protected:
	INT m_nPosPrevious, m_nPosInitial;

protected:
	void _DoEndEdit( bool bSaveChanges );
	bool _DoVerifyInput( bool bEndEdit );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtGridInplaceSlider)
	public:
	virtual bool Create();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

    //{{AFX_MSG(CExtGridInplaceSlider)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
}; // class CExtGridInplaceSlider

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellIPAddress

class __PROF_UIS_API CExtGridCellIPAddress : public CExtGridCellEx
{
public:
	DECLARE_SERIAL( CExtGridCellIPAddress );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellIPAddress, CExtGridCellEx );
	CExtGridCellIPAddress(
		CExtGridDataProvider * pDataProvider = NULL
		);
	
	void GetAddress(
		BYTE & nField0,
		BYTE & nField1,
		BYTE & nField2,
		BYTE & nField3 
		);
	void GetAddress(
		DWORD & dwAddress 
		);
	
	void SetAddress(
		BYTE nField0,
		BYTE nField1,
		BYTE nField2,
		BYTE nField3 
		);
	void SetAddress(
		DWORD dwAddress 
		);
	
	virtual void Empty();
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	static bool stat_ParseIPAddress( 
		CExtSafeString sText,
		DWORD & dwIPAddress
		);

	// virtual methods
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;
	virtual void TextGet( CExtSafeString & strCopy ) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	virtual void GetVariant( VARIANT & varCopy ) const;

	virtual bool OnParseAutoEditStartText(
		CExtSafeString & sTextAutoEditStart
		) const;
	virtual HRESULT OnParseText(
		__EXT_MFC_SAFE_LPCTSTR sText
		) const;

	virtual void OnFormatCellText(
		CExtSafeString & strText,
		DWORD dwAddress
		) const;

	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);

	virtual void OnInplaceControlIPAddressInputComplete(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwIPAddressNew,
		bool bSaveChanges
		);

protected:
	DWORD m_dwIPAddress;

}; // class CExtGridCellIPAddress

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellString

class __PROF_UIS_API CExtGridCellString : public CExtGridCellEx
{
public:
	DECLARE_SERIAL( CExtGridCellString );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellString, CExtGridCellEx );
	CExtGridCellString(
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellString( const CExtGridCellString & other );
	CExtGridCellString( const CExtGridCell & other );
	virtual ~CExtGridCellString();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG
	virtual void Serialize( CArchive & ar );
	virtual void Empty();
	virtual void Assign( const CExtGridCell & other );
	virtual void TextGet( CExtSafeString & strCopy ) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	virtual void GetVariant( VARIANT & varCopy ) const;
	virtual __EXT_MFC_SAFE_LPCTSTR GetTextBuffer() const;

	// virtual methods
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;

protected:
	CExtSafeString m_str;

}; // class CExtGridCellString

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellStringDM

class __PROF_UIS_API CExtGridCellStringDM : public CExtGridCell
{
	void _StringBufferFree();
	void _StringBufferSet( __EXT_MFC_SAFE_LPCTSTR strBuf );
public:
	DECLARE_SERIAL( CExtGridCellStringDM );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellStringDM, CExtGridCell );
	__EXT_MFC_SAFE_LPTSTR m_strData;
	CExtGridCellStringDM(
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellStringDM( const CExtGridCellStringDM & other );
	CExtGridCellStringDM( const CExtGridCell & other );
	virtual ~CExtGridCellStringDM();

	virtual void Serialize( CArchive & ar );
	virtual void Empty();
	virtual void Assign( const CExtGridCell & other );
	virtual void TextGet( CExtSafeString & strCopy ) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	virtual void GetVariant( VARIANT & varCopy ) const;
	virtual __EXT_MFC_SAFE_LPCTSTR GetTextBuffer() const;
}; // class CExtGridCellStringDM

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellStringTI
// template for internal static text buffer cell type
// nCTBS - cell text buffer size (must be > 1)

template < UINT nCTBS = 64 >
class CExtGridCellStringTI : public CExtGridCell
{
public:
	BYTE m_strData[ sizeof(TCHAR)*nCTBS ];
	CExtGridCellStringTI(
		CExtGridDataProvider * pDataProvider = NULL
		)
		: CExtGridCell( pDataProvider )
	{
		ASSERT( nCTBS > 1 );
		::memset( m_strData, 0, sizeof(TCHAR)*nCTBS );
	}
	CExtGridCellStringTI( const CExtGridCellStringTI & other )
	{
		ASSERT( nCTBS > 1 );
		(*this) = other;
	}
private:
	__EXT_MFC_SAFE_LPCTSTR _GetBufferPtr() const
	{
		ASSERT( nCTBS > 1 );
		return __EXT_MFC_SAFE_LPCTSTR( (LPCTSTR)(LPVOID)m_strData );
	}
	__EXT_MFC_SAFE_LPTSTR _GetBufferPtr()
	{
		ASSERT( nCTBS > 1 );
		return __EXT_MFC_SAFE_LPTSTR( (LPTSTR)(LPVOID)m_strData );
	}
	LONG _GetBufferSize()
	{
		ASSERT( nCTBS > 1 );
		return nCTBS;
	}
public:
	virtual void Serialize( CArchive & ar )
	{
		ASSERT_VALID( this );
		ASSERT( nCTBS > 1 );
		CExtGridCell::Serialize( ar );
		if( ar.IsStoring() )
		{
			CExtSafeString sTmp( _GetBufferPtr() );
			ar << sTmp;
		} // if( ar.IsStoring() )
		else
		{
			::memset( m_strData, 0, sizeof(TCHAR)*nCTBS );
			CExtSafeString sTmp;
			ar >> sTmp;
			if( !sTmp.IsEmpty() )
				__EXT_MFC_STRNCPY(
					_GetBufferPtr(),
					_GetBufferSize(),
					sTmp,
					nCTBS - 1
					);
		} // else from if( ar.IsStoring() )
	}
	virtual void Empty()
	{
		ASSERT_VALID( this );
		ASSERT( nCTBS > 1 );
		::memset( m_strData, 0, sizeof(TCHAR)*nCTBS );
		CExtGridCell::Empty();
	}
	virtual void Assign( const CExtGridCell & other )
	{
		ASSERT_VALID( this );
		ASSERT( nCTBS > 1 );
		::memset( m_strData, 0, sizeof(TCHAR)*nCTBS );
		CExtSafeString sTmp;
		other.TextGet( sTmp );
		__EXT_MFC_STRNCPY(
			_GetBufferPtr(),
			_GetBufferSize(),
			sTmp,
			nCTBS - 1
			);
		ModifyStyleEx( 0, __EGCS_EX_EMPTY );
	}
	virtual void TextGet( CExtSafeString & strCopy ) const
	{
		ASSERT_VALID( this );
		ASSERT( nCTBS > 1 );
		if(		IsUndefined() 
			||	IsEmpty()
			)
		{
			strCopy = _T("");
			return;
		}
		strCopy = _GetBufferPtr();
	}
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		)
	{
		ASSERT_VALID( this );
		ASSERT( nCTBS > 1 );
		LPCTSTR strNew = LPCTSTR(str);
		LPCTSTR strOld = _GetBufferPtr();
		if( strNew == strOld )
			return;
		::memset( m_strData, 0, sizeof(TCHAR)*nCTBS );
		if( strNew[0] == _T('\0') )
			return;
		__EXT_MFC_STRNCPY(
			_GetBufferPtr(),
			_GetBufferSize(),
			strNew,
			nCTBS - 1
			);
		ModifyStyleEx( 0, __EGCS_EX_EMPTY );
	}
	virtual void GetVariant( VARIANT & varCopy ) const
	{
		ASSERT_VALID( this );
		ASSERT( nCTBS > 1 );
		stat_VariantClear( &varCopy );
		USES_CONVERSION;
		BSTR bstrCopy = ::SysAllocString( T2OLE( _GetBufferPtr() ) );
		if( bstrCopy == NULL )
			return;
		varCopy.vt = VT_BSTR;
		varCopy.bstrVal = bstrCopy;
	}
	virtual __EXT_MFC_SAFE_LPCTSTR GetTextBuffer() const
	{
		ASSERT_VALID( this );
		ASSERT( nCTBS > 1 );
		return _GetBufferPtr();
	}
	virtual CExtGridCell * Clone(
		IMalloc * pMalloc = NULL
		) const
	{
		ASSERT_VALID( this );
		ASSERT( nCTBS > 1 );
		if( pMalloc == NULL )
		{
			try
			{
				CExtGridCell * pOther =
					new CExtGridCellStringTI < nCTBS >
						(	(	const_cast
								< CExtGridCellStringTI < nCTBS > * >
								( this )
							)
							-> DataProviderGet()
							);
				ASSERT( pOther != NULL );
				if( pOther != NULL )
				{
					ASSERT_VALID( pOther );
					pOther->Assign( *this );
					ASSERT( pOther->DataProviderGet() == DataProviderGet() );
				} // if( pOther != NULL )
				return pOther;
			} // try
			catch( CException * pException )
			{
				ASSERT( FALSE );
				pException->Delete();
			} // catch( CException * pException )
			catch( ... )
			{
				ASSERT( FALSE );
			} // catch( ... )
			return NULL;
		} // if( pMalloc == NULL )
		CExtGridCell * pOther =
			new (pMalloc, false) CExtGridCellStringTI < nCTBS >
				(	(	const_cast
						< CExtGridCellStringTI < nCTBS > * >
						( this )
					)
					-> DataProviderGet()
				);
		ASSERT( pOther != NULL );
		if( pOther != NULL )
		{
			ASSERT_VALID( pOther );
			pOther->Assign( *this );
			ASSERT( pOther->DataProviderGet() == DataProviderGet() );
		} // if( pOther != NULL )
		return pOther;
	}
}; // class CExtGridCellStringTI

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellStringTE
// template for external static text buffer cell type
// p_CTBuff - pointer to external text buffer (must be not NULL)
// nCTBS - cell text buffer size (must be > 1)

template < LPVOID p_CTBuff, UINT nCTBS = 64 >
class CExtGridCellStringTE : public CExtGridCell
{
public:
	CExtGridCellStringTE(
		CExtGridDataProvider * pDataProvider = NULL
		)
		: CExtGridCell( pDataProvider )
	{
		ASSERT( p_CTBuff != NULL );
		ASSERT( nCTBS > 1 );
		::memset( p_CTBuff, 0, sizeof(TCHAR)*nCTBS );
	}
	CExtGridCellStringTE( const CExtGridCellStringTE & other )
	{
		ASSERT( p_CTBuff != NULL );
		ASSERT( nCTBS > 1 );
		(*this) = other;
	}
private:
	__EXT_MFC_SAFE_LPCTSTR _GetBufferPtr() const
	{
		ASSERT( p_CTBuff != NULL );
		ASSERT( nCTBS > 1 );
		return __EXT_MFC_SAFE_LPCTSTR( (LPCTSTR)p_CTBuff );
	}
	__EXT_MFC_SAFE_LPTSTR _GetBufferPtr()
	{
		ASSERT( p_CTBuff != NULL );
		ASSERT( nCTBS > 1 );
		return __EXT_MFC_SAFE_LPTSTR( (LPTSTR)p_CTBuff );
	}
public:
	virtual void Serialize( CArchive & ar )
	{
		ASSERT_VALID( this );
		ASSERT( p_CTBuff != NULL );
		ASSERT( nCTBS > 1 );
		CExtGridCell::Serialize( ar );
		if( ar.IsStoring() )
		{
			CExtSafeString sTmp( _GetBufferPtr() );
			ar << sTmp;
		} // if( ar.IsStoring() )
		else
		{
			::memset( p_CTBuff, 0, sizeof(TCHAR)*nCTBS );
			CExtSafeString sTmp;
			ar >> sTmp;
			if( !sTmp.IsEmpty() )
				__EXT_MFC_STRNCPY(
					_GetBufferPtr(),
					_GetBufferSize(),
					sTmp,
					nCTBS - 1
					);
		} // else from if( ar.IsStoring() )
	}
	virtual void Empty()
	{
		ASSERT_VALID( this );
		ASSERT( p_CTBuff != NULL );
		ASSERT( nCTBS > 1 );
		::memset( p_CTBuff, 0, sizeof(TCHAR)*nCTBS );
		CExtGridCell::Empty();
	}
	virtual void Assign( const CExtGridCell & other )
	{
		ASSERT_VALID( this );
		ASSERT( p_CTBuff != NULL );
		ASSERT( nCTBS > 1 );
		::memset( p_CTBuff, 0, sizeof(TCHAR)*nCTBS );
		CExtSafeString sTmp;
		other.TextGet( sTmp );
		__EXT_MFC_STRNCPY(
			_GetBufferPtr(),
			_GetBufferSize(),
			sTmp,
			nCTBS - 1
			);
		ModifyStyleEx( 0, __EGCS_EX_EMPTY );
	}
	virtual void TextGet( CExtSafeString & strCopy ) const
	{
		ASSERT_VALID( this );
		ASSERT( p_CTBuff != NULL );
		ASSERT( nCTBS > 1 );
		if(		IsUndefined() 
			||	IsEmpty()
			)
		{
			strCopy = _T("");
			return;
		}
		strCopy = _GetBufferPtr();
	}
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		)
	{
		ASSERT_VALID( this );
		ASSERT( p_CTBuff != NULL );
		ASSERT( nCTBS > 1 );
		LPCTSTR strNew = LPCTSTR(str);
		LPCTSTR strOld = _GetBufferPtr();
		if( strNew == strOld )
			return;
		::memset( p_CTBuff, 0, sizeof(TCHAR)*nCTBS );
		if( strNew[0] == _T('\0') )
			return;
		__EXT_MFC_STRNCPY(
			_GetBufferPtr(),
			_GetBufferSize(),
			strNew,
			nCTBS - 1
			);
		ModifyStyleEx( 0, __EGCS_EX_EMPTY );
	}
	virtual void GetVariant( VARIANT & varCopy ) const
	{
		ASSERT_VALID( this );
		ASSERT( p_CTBuff != NULL );
		ASSERT( nCTBS > 1 );
		stat_VariantClear( &varCopy );
		USES_CONVERSION;
		BSTR bstrCopy = ::SysAllocString( T2OLE( _GetBufferPtr() ) );
		if( bstrCopy == NULL )
			return;
		varCopy.vt = VT_BSTR;
		varCopy.bstrVal = bstrCopy;
	}
	virtual __EXT_MFC_SAFE_LPCTSTR GetTextBuffer() const
	{
		ASSERT_VALID( this );
		ASSERT( p_CTBuff != NULL );
		ASSERT( nCTBS > 1 );
		return _GetBufferPtr();
	}
	virtual CExtGridCell * Clone(
		IMalloc * pMalloc = NULL
		) const
	{
		ASSERT_VALID( this );
		ASSERT( nCTBS > 1 );
		if( pMalloc == NULL )
		{
			try
			{
				CExtGridCell * pOther =
					new CExtGridCellStringTE < p_CTBuff, nCTBS >
						(	(	const_cast
								< CExtGridCellStringTE < p_CTBuff, nCTBS > * >
								( this )
							)
							-> DataProviderGet()
						);
				ASSERT( pOther != NULL );
				if( pOther != NULL )
				{
					ASSERT_VALID( pOther );
					pOther->Assign( *this );
					ASSERT( pOther->DataProviderGet() == DataProviderGet() );
				} // if( pOther != NULL )
				return pOther;
			} // try
			catch( CException * pException )
			{
				ASSERT( FALSE );
				pException->Delete();
			} // catch( CException * pException )
			catch( ... )
			{
				ASSERT( FALSE );
			} // catch( ... )
			return NULL;
		} // if( pMalloc == NULL )
		CExtGridCell * pOther =
			new (pMalloc, false) CExtGridCellStringTE < p_CTBuff, nCTBS >
				( ( ( const_cast < CExtGridCellStringTE < p_CTBuff, nCTBS > * > ( this ) ) -> DataProviderGet() ) );
		ASSERT( pOther != NULL );
		if( pOther != NULL )
		{
			ASSERT_VALID( pOther );
			pOther->Assign( *this );
		} // if( pOther != NULL )
		return pOther;
	}
}; // class CExtGridCellStringTE

/////////////////////////////////////////////////////////////////////////////
// CExtGCE
// template for adding row/column extent and extra-space
// information to other cell types (used for header cells)

template < class _BC >
class CExtGCE : public _BC
{
	INT m_nItemExtentCurrent, m_nItemExtentMin, m_nItemExtentMax,
		m_nExtraSpaceAfter, m_nExtraSpaceBefore;
public:
	CExtGCE(
		CExtGridDataProvider * pDataProvider = NULL,
		INT nItemExtentCurrent = GetSystemMetrics(SM_CYHSCROLL)+1,
		INT nItemExtentMin = 0,
		INT nItemExtentMax = 32767,
		INT nExtraSpaceAfter = 0,
		INT nExtraSpaceBefore = 0
		)
		: _BC( pDataProvider )
		, m_nItemExtentCurrent( nItemExtentCurrent )
		, m_nItemExtentMin( nItemExtentMin )
		, m_nItemExtentMax( nItemExtentMax )
		, m_nExtraSpaceAfter( nExtraSpaceAfter )
		, m_nExtraSpaceBefore( nExtraSpaceBefore )
	{
		ASSERT_VALID( this );
	}
	CExtGCE( const CExtGridCell & other )
		: m_nItemExtentCurrent( 16 )
		, m_nItemExtentMin( 0 )
		, m_nItemExtentMax( 32767 )
		, m_nExtraSpaceAfter( 0 )
		, m_nExtraSpaceBefore( 0 )
	{
		Assign( other );
	}
#ifdef _DEBUG
	virtual void AssertValid() const
	{
		ASSERT( this != NULL );
		ASSERT( IsKindOf(RUNTIME_CLASS(CExtGridCell)) );
		_BC::AssertValid();
//		ASSERT(
//				m_nItemExtentCurrent >= 0
//			&&	m_nItemExtentMin >= 0
//			&&	m_nItemExtentMax >= 0
//			&&	m_nItemExtentMin <= m_nItemExtentMax
//			&&	m_nItemExtentMin <= m_nItemExtentCurrent
//			&&	m_nItemExtentCurrent <= m_nItemExtentMax
//			&&	m_nExtraSpaceAfter >= 0
//			&&	m_nExtraSpaceBefore >= 0
//			);
	}
#endif // _DEBUG
	virtual void Assign( const CExtGridCell & other )
	{
		ASSERT_VALID( this );
		_BC::Assign( other );
		INT nItemExtentCurrent, nItemExtentMin, nItemExtentMax,
			nExtraSpaceAfter, nExtraSpaceBefore;
		if(		other.ExtentGet( nItemExtentMin, -1 )
			&&	other.ExtentGet( nItemExtentCurrent, 0 )
			&&	other.ExtentGet( nItemExtentMax, 1 )
			)
		{
			ASSERT(
					nItemExtentCurrent >= 0
				&&	nItemExtentMin >= 0
				&&	nItemExtentMax >= 0
				&&	nItemExtentMin <= nItemExtentMax
				&&	nItemExtentMin <= nItemExtentCurrent
				&&	nItemExtentCurrent <= nItemExtentMax
				);
			VERIFY( ExtentSet( nItemExtentMin, -1 ) );
			VERIFY( ExtentSet( nItemExtentCurrent, 0 ) );
			VERIFY( ExtentSet( nItemExtentMax, 1 ) );
		}
		if( other.ExtraSpaceGet( nExtraSpaceAfter, true ) )
			ExtraSpaceSet( nExtraSpaceAfter, true );
		if( other.ExtraSpaceGet( nExtraSpaceBefore, false ) )
			ExtraSpaceSet( nExtraSpaceBefore, false );
	}
	virtual CExtGridCell * Clone(
		IMalloc * pMalloc = NULL
		) const
	{
		ASSERT_VALID( this );
		if( pMalloc == NULL )
		{
			try
			{
				CExtGridCell * pOther =
					new CExtGCE < _BC >
						(	(	const_cast
								< CExtGCE < _BC > * >
								( this )
							)
							-> DataProviderGet()
						);
				ASSERT( pOther != NULL );
				if( pOther != NULL )
				{
					ASSERT_VALID( pOther );
					pOther->Assign( *this );
					ASSERT( pOther->DataProviderGet() == DataProviderGet() );
				} // if( pOther != NULL )
				return pOther;
			} // try
			catch( CException * pException )
			{
				ASSERT( FALSE );
				pException->Delete();
			} // catch( CException * pException )
			catch( ... )
			{
				ASSERT( FALSE );
			} // catch( ... )
			return NULL;
		} // if( pMalloc == NULL )
		CExtGridCell * pOther =
			new (pMalloc, false) CExtGCE < _BC >
				(	(	const_cast
						< CExtGCE < _BC > * >
						( this )
					)
					-> DataProviderGet()
				);
		ASSERT( pOther != NULL );
		if( pOther != NULL )
		{
			ASSERT_VALID( pOther );
			pOther->Assign( *this );
			ASSERT( pOther->DataProviderGet() == DataProviderGet() );
		} // if( pOther != NULL )
		return pOther;
	}
	virtual void Serialize( CArchive & ar )
	{
		ASSERT_VALID( this );
		_BC::Serialize( ar );
		INT nItemExtentCurrent, nItemExtentMin, nItemExtentMax,
			nExtraSpaceAfter, nExtraSpaceBefore;
		if( ar.IsStoring() )
		{
			VERIFY( ExtentGet( nItemExtentMin, -1 ) );
			VERIFY( ExtentGet( nItemExtentCurrent, 0 ) );
			VERIFY( ExtentGet( nItemExtentMax, 1 ) );
			VERIFY( ExtraSpaceGet( nExtraSpaceAfter, true ) );
			VERIFY( ExtraSpaceGet( nExtraSpaceBefore, false ) );
			ASSERT(
					nItemExtentCurrent >= 0
				&&	nItemExtentMin >= 0
				&&	nItemExtentMax >= 0
				&&	nItemExtentMin <= nItemExtentMax
				&&	nItemExtentMin <= nItemExtentCurrent
				&&	nItemExtentCurrent <= nItemExtentMax
				&&	nExtraSpaceAfter >= 0
				&&	nExtraSpaceBefore >= 0
				);
			ar << DWORD(nItemExtentCurrent);
			ar << DWORD(nItemExtentMin);
			ar << DWORD(nItemExtentMax);
			ar << DWORD(nExtraSpaceAfter);
			ar << DWORD(nExtraSpaceBefore);
		} // if( ar.IsStoring() )
		else
		{
			DWORD dwTmp;
			ar >> dwTmp;
			nItemExtentCurrent = INT(dwTmp);
			ar >> dwTmp;
			nItemExtentMin = INT(dwTmp);
			ar >> dwTmp;
			nItemExtentMax = INT(dwTmp);
			ar >> dwTmp;
			nExtraSpaceAfter = INT(dwTmp);
			ar >> dwTmp;
			nExtraSpaceBefore = INT(dwTmp);
			ASSERT(
					nItemExtentCurrent >= 0
				&&	nItemExtentMin >= 0
				&&	nItemExtentMax >= 0
				&&	nItemExtentMin <= nItemExtentMax
				&&	nItemExtentMin <= nItemExtentCurrent
				&&	nItemExtentCurrent <= nItemExtentMax
				&&	nExtraSpaceAfter >= 0
				&&	nExtraSpaceBefore >= 0
				);
			VERIFY( ExtentSet( nItemExtentMin, -1 ) );
			VERIFY( ExtentSet( nItemExtentCurrent, 0 ) );
			VERIFY( ExtentSet( nItemExtentMax, 1 ) );
			VERIFY( ExtraSpaceSet( nExtraSpaceAfter, true ) );
			VERIFY( ExtraSpaceSet( nExtraSpaceBefore, false ) );
		} // else from if( ar.IsStoring() )
	}
	virtual bool OnQueryExtent(
		INT & nItemExtent,
		bool bGet,
		INT nExtentType = 0 // -1 - min, 0 - curr, 1 - max
		)
	{
		ASSERT( this != NULL );
		//ASSERT( IsKindOf(RUNTIME_CLASS(CExtGridCell)) );
		if( bGet )
		{
			if( nExtentType < 0 )
				nItemExtent = m_nItemExtentMin;
			else if( nExtentType > 0 )
				nItemExtent = m_nItemExtentMax;
			else
				nItemExtent = m_nItemExtentCurrent;
		} // if( bGet )
		else
		{
			if( nExtentType < 0 )
				m_nItemExtentMin = nItemExtent;
			else if( nExtentType > 0 )
				m_nItemExtentMax = nItemExtent;
			else
				m_nItemExtentCurrent = nItemExtent;
		} // else from if( bGet )
		return true;
	}
	virtual bool OnQueryExtraSpace(
		INT & nItemExtraSpace,
		bool bGet,
		bool bAfter // true - extra space after, false - after before
		)
	{
		ASSERT( this != NULL );
		ASSERT( IsKindOf(RUNTIME_CLASS(CExtGridCell)) );
		if( bGet )
		{
			if( bAfter )
				nItemExtraSpace = m_nExtraSpaceAfter;
			else
				nItemExtraSpace = m_nExtraSpaceBefore;
		} // if( bGet )
		else
		{
			if( bAfter )
				m_nExtraSpaceAfter = nItemExtraSpace;
			else
				m_nExtraSpaceBefore = nItemExtraSpace;
		} // else from if( bGet )
		return true;
	}
}; // class CExtGCE

/////////////////////////////////////////////////////////////////////////////
// CExtGCP
// template for adding row/column relative percent extent
// (used for header cells)

template < class _BC >
class CExtGCP : public _BC
{
	double m_lfExtentPercent;
public:
	CExtGCP(
		CExtGridDataProvider * pDataProvider = NULL,
		double lfExtentPercent = 0.5
		)
		: _BC( pDataProvider )
		, m_lfExtentPercent( lfExtentPercent )
	{
		ASSERT_VALID( this );
	}
	CExtGCP( const CExtGridCell & other )
		: m_lfExtentPercent( 0.5 )
	{
		Assign( other );
	}
#ifdef _DEBUG
	virtual void AssertValid() const
	{
		ASSERT( this != NULL );
		ASSERT( IsKindOf(RUNTIME_CLASS(CExtGridCell)) );
		_BC::AssertValid();
//		ASSERT( m_lfExtentPercent >= 0.0 );
	}
#endif // _DEBUG
	virtual void Assign( const CExtGridCell & other )
	{
		ASSERT_VALID( this );
		_BC::Assign( other );
		double lfExtentPercent = 0.0;
		if( other.ExtentPercentGet( lfExtentPercent ) )
		{
			ASSERT( lfExtentPercent >= 0.0 );
			VERIFY( ExtentPercentSet( lfExtentPercent ) );
		}
	}
	virtual CExtGridCell * Clone(
		IMalloc * pMalloc = NULL
		) const
	{
		ASSERT_VALID( this );
		if( pMalloc == NULL )
		{
			try
			{
				CExtGridCell * pOther =
					new CExtGCP < _BC >
						(	(	const_cast
								< CExtGCP < _BC > * >
								( this )
							)
							-> DataProviderGet()
						);
				ASSERT( pOther != NULL );
				if( pOther != NULL )
				{
					ASSERT_VALID( pOther );
					pOther->Assign( *this );
					ASSERT( pOther->DataProviderGet() == DataProviderGet() );
				} // if( pOther != NULL )
				return pOther;
			} // try
			catch( CException * pException )
			{
				ASSERT( FALSE );
				pException->Delete();
			} // catch( CException * pException )
			catch( ... )
			{
				ASSERT( FALSE );
			} // catch( ... )
			return NULL;
		} // if( pMalloc == NULL )
		CExtGridCell * pOther =
			new (pMalloc, false) CExtGCP < _BC >
				(	(	const_cast
						< CExtGCP < _BC > * >
						( this )
					)
					-> DataProviderGet()
				);
		ASSERT( pOther != NULL );
		if( pOther != NULL )
		{
			ASSERT_VALID( pOther );
			pOther->Assign( *this );
			ASSERT( pOther->DataProviderGet() == DataProviderGet() );
		} // if( pOther != NULL )
		return pOther;
	}
	virtual void Serialize( CArchive & ar )
	{
		ASSERT_VALID( this );
		_BC::Serialize( ar );
		double lfExtentPercent = 0.0;
		if( ar.IsStoring() )
		{
			ExtentPercentGet( lfExtentPercent );
			ASSERT( lfExtentPercent >= 0.0 );
			ar << lfExtentPercent;
		} // if( ar.IsStoring() )
		else
		{
			ar >> lfExtentPercent;
			ASSERT( lfExtentPercent >= 0.0 );
			VERIFY( ExtentPercentSet( lfExtentPercent ) );
		} // else from if( ar.IsStoring() )
	}
	virtual bool OnQueryExtentPercent(
		double & lfExtentPercent,
		bool bGet
		)
	{
		ASSERT( this != NULL );
		//ASSERT( IsKindOf(RUNTIME_CLASS(CExtGridCell)) );
		if( bGet )
			lfExtentPercent = m_lfExtentPercent;
		else
			m_lfExtentPercent = lfExtentPercent;
		return true;
	}
}; // class CExtGCP

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellHeader

class __PROF_UIS_API CExtGridCellHeader
	: public CExtGCJ < CExtGCF < CExtGCC < CExtGCP < CExtGCE < CExtGridCellStringDM > > > > >
{
public:
	DECLARE_SERIAL( CExtGridCellHeader );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellHeader, CExtGCJ < CExtGCF < CExtGCC < CExtGCP < CExtGCE < CExtGridCellStringDM > > > > > );
	CExtGridCellHeader(
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellHeader( const CExtGridCell & other );

	virtual bool IsEmpty() const;
	
}; // class CExtGridCellHeader

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellHeaderFilter

class __PROF_UIS_API CExtGridCellHeaderFilter : public CExtGridCellHeader
{
public:
	DECLARE_SERIAL( CExtGridCellHeaderFilter );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellHeaderFilter, CExtGridCellHeader );
	CExtSafeString m_strTextFilterEquals;              // ID_EXT_HEADER_FILTER_MENU_TF_EQUALS
	CExtSafeString m_strTextFilterDoesNotEqual;        // ID_EXT_HEADER_FILTER_MENU_TF_DOES_NOT_EQUAL
	CExtSafeString m_strTextFilterBeginsWith;          // ID_EXT_HEADER_FILTER_MENU_TF_BEGINS_WITH
	CExtSafeString m_strTextFilterDoesNotBeginWith;    // ID_EXT_HEADER_FILTER_MENU_TF_DOES_NOT_BEGIN_WITH
	CExtSafeString m_strTextFilterContains;            // ID_EXT_HEADER_FILTER_MENU_TF_CONTAINS
	CExtSafeString m_strTextFilterDoesNotContain;      // ID_EXT_HEADER_FILTER_MENU_TF_DOES_NOT_CONTAIN
	CExtSafeString m_strTextFilterEndsWidth;           // ID_EXT_HEADER_FILTER_MENU_TF_ENDS_WITH
	CExtSafeString m_strTextFilterDoesNotEndWith;      // ID_EXT_HEADER_FILTER_MENU_TF_DOES_NOT_END_WITH
	CExtArray < CExtSafeString > m_arrUniqueStringValues; // value filter
	CExtArray < CExtSafeString > m_arrStaticStringValues; // value filter (! m_bDynamicValueFilters )
	bool m_bDynamicValueFilters:1;
	bool m_bValueFilterShowAllState:1;
	bool m_bSortingCommandsEnabled:1;
	bool m_bTextFiltersEnabled:1;
	bool m_bValueFiltersEnabled:1;
	INT m_nInplaceEditWidth;
	static CExtGridCell::TrackCellStateInfo_t * g_pHelperTCSI;
	CExtGridCellHeaderFilter(
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellHeaderFilter( const CExtGridCell & other );
	virtual ~CExtGridCellHeaderFilter();
	virtual void OnCalcLayout(
		DWORD dwCellStyle,
		DWORD dwCellStyleEx,
		CRect & rcCellRest,
		CRect & rcCellText,
		CRect & rcFocusArrow,
		CRect & rcIcon,
		CRect & rcCheck,
		CRect & rcSortArrow,
		CRect & rcUpdownButtonTop,
		CRect & rcUpdownButtonBottom,
		CRect & rcDropdownButton,
		CRect & rcEllipsisButton,
		CSize sizeFocusArrow,
		CSize sizeIcon,
		CSize sizeCheck,
		CSize sizeSortArrow,
		INT nExtentUpdownButton,
		INT nExtentDropdownButton,
		INT nExtentEllipsisButton,
		INT & nFocusGlyphType,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual INT OnCalcButtonExtent(
		INT nButtonType, // e_button_type_t
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual bool OnQueryCellFilteredState(
		CExtGridCell * pCell
		) const;
	virtual bool OnQueryTextValueFilteredState(
		CExtSafeString & str
		) const;
	virtual bool OnQueryTextValueFilteredStateTF(
		CExtSafeString & str
		) const;
	virtual bool OnQueryTextValueFilteredStateVF(
		CExtSafeString & str
		) const;
	virtual bool IsAnyFilterApplied() const;
	virtual void FilterConditionsClear();
	virtual bool IsMenuAvailable() const;
	virtual bool IsEmpty() const;
	virtual void Empty();
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );
	virtual void SerializeFilterData( CArchive & ar );
	virtual void OnPaintButton(
		const RECT & rcButton,
		INT nButtonType, // e_button_type_t
		bool bPressed,
		bool bHovered,
		bool bFocused,
		bool bEnabled,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual bool OnButtonPopupMenuTrack(
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	static void stat_CalcPlainUniqueValuesForColumn(
		CExtArray < CExtSafeString > & arrUniqueStringValues,
		const CExtGridWnd & wndGrid,
		LONG nColNo
		);
	static void stat_CalcPlainUniqueValuesForRow(
		CExtArray < CExtSafeString > & arrUniqueStringValues,
		const CExtGridWnd & wndGrid,
		LONG nRowNo
		);
	virtual bool OnPopupListBoxInitContent(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual CSize OnPopupListBoxMeasureTrackSize(
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		CExtPopupListBoxMenuWnd * pPopup
		);
	virtual bool OnPopupListBoxItemDraw(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LPDRAWITEMSTRUCT pDIS
		);
	virtual bool OnPopupListBoxItemMeasure(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LPMEASUREITEMSTRUCT pMIS
		);
	virtual bool OnPopupListBoxItemClick(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
protected:
	static bool stat_CbInplaceEditWndProcTF(
		LRESULT & lResult,
		UINT message,
		WPARAM wParam,
		LPARAM lParam,
		CExtPopupMenuWnd::CInPlaceEditWnd & wndEdit,
		CExtGridCellHeaderFilter * pHeaderFilterCell
		);
	static bool stat_CbVerifyTextInputTF(
		CExtPopupMenuWnd::CInPlaceEditWnd & wndEdit,
		CExtGridCellHeaderFilter * pHeaderFilterCell,
		__EXT_MFC_SAFE_LPCTSTR sTextOld,
		__EXT_MFC_SAFE_LPCTSTR sTextNew
		);
	static void stat_CbPutTextInputResultTF(
		CExtPopupMenuWnd::CInPlaceEditWnd & wndEdit,
		CExtGridCellHeaderFilter * pHeaderFilterCell,
		__EXT_MFC_SAFE_LPCTSTR sTextNew
		);
public:
	virtual bool OnTextFilterInplaceEditWndProc(
		LRESULT & lResult,
		UINT message,
		WPARAM wParam,
		LPARAM lParam,
		CExtPopupMenuWnd::CInPlaceEditWnd & wndEdit
		);
	virtual bool OnTextFilterVerifyTextInput(
		CExtPopupMenuWnd::CInPlaceEditWnd & wndEdit,
		__EXT_MFC_SAFE_LPCTSTR sTextOld,
		__EXT_MFC_SAFE_LPCTSTR sTextNew
		);
	virtual void OnTextFilterPutTextInputResult(
		CExtPopupMenuWnd::CInPlaceEditWnd & wndEdit,
		__EXT_MFC_SAFE_LPCTSTR sTextNew,
		bool bFinal
		);
	virtual bool OnPopupFilterListBoxItemCheckGet(
		INT nItemIndex,
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual void OnPopupFilterListBoxItemCheckSet(
		INT nItemIndex,
		bool bCheck,
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnInitializeFilterPopupMenu(
		CExtPopupMenuWnd * pPopup,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
}; // class CExtGridCellHeaderFilter

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellColor

class __PROF_UIS_API CExtGridCellColor : public CExtGridCellEx
{
public:
	class __PROF_UIS_API CPackedColor
	{
	public:
		BYTE m_arrColorBytes[5];
		enum e_PackedFlags_t
		{
			__EPFT_DEFAULT_FLAGS = BYTE(0x00),
			__EPFT_SYSCOLOR = BYTE(0x01),
			__EPFT_EMPTY = BYTE(0x02),
		};

		CPackedColor();
		CPackedColor( 
			COLORREF clr,
			bool bSetAlpha = true
			);
		CPackedColor( 
			INT nSysColorIdx 
			);
		CPackedColor( 
			const CPackedColor & other 
			);
		~CPackedColor();
		
		CPackedColor & operator =( const CPackedColor & other );
		bool operator == ( const CPackedColor & other ) const;
		bool operator != ( const CPackedColor & other ) const;
		bool operator <  ( const CPackedColor & other ) const;
		bool operator >  ( const CPackedColor & other ) const;
		bool operator <= ( const CPackedColor & other ) const;
		bool operator >= ( const CPackedColor & other ) const;

		void Clear();
		bool IsEmpty() const;
		bool IsSysColor() const;
		COLORREF GetRefColor() const;
		INT GetSysColorIdx() const;
		COLORREF GetColor() const;

		__EXT_MFC_SAFE_LPCTSTR GetColorName( 
			COLORREF clr 
			) const;
		COLORREF GetColorRGB( 
			__EXT_MFC_SAFE_LPCTSTR lpszColorName 
			) const;

		void SetColor( 
			COLORREF clr,
			bool bSetAlpha = true
			);
		void SetColor( 
			INT nSysColorIdx 
			);
		void Serialize( 
			CArchive & ar 
			);
		
		static COLORREF stat_GenRandomizedColor(
			DWORD nRedMin = DWORD(0),
			DWORD nRedMax = DWORD(255),
			DWORD nGreenMin = DWORD(0),
			DWORD nGreenMax = DWORD(255),
			DWORD nBlueMin = DWORD(0),
			DWORD nBlueMax = DWORD(255)
			);

		void SetRandomizedColor(
			DWORD nRedMin = DWORD(0),
			DWORD nRedMax = DWORD(255),
			DWORD nGreenMin = DWORD(0),
			DWORD nGreenMax = DWORD(255),
			DWORD nBlueMin = DWORD(0),
			DWORD nBlueMax = DWORD(255)
			);

	protected:
		void InitColorMap();

		typedef
			CMap <
				COLORREF,
				COLORREF,
				__EXT_MFC_SAFE_LPCTSTR,
				__EXT_MFC_SAFE_LPCTSTR
				>
			ColorNamesMap_t;
		ColorNamesMap_t m_mapColorNames;
	
		typedef
			CMapStringToPtr
			ColorRGBMap_t;
		ColorRGBMap_t m_mapColorRGB;

	}; // class CPackedColor

	enum e_Mode_t
	{
		eNone		= 0,
		eRGB		= 1,
		eRGBName	= 2,
		eRGBHex		= 3,
		eRGBA		= 4,
		eRGBAName	= 5,
		eRGBAHex	= 6,
		eHSL		= 7,
		eCMY		= 8,
	};

	bool m_bEnableButtonRandomColor:1;
	bool m_bEnableButtonCustomColor:1;
	bool m_bEnableButtonDefaultColor:1;

	CExtSafeString m_sBtnTextColorRandom;
	CExtSafeString m_sBtnTextColorCustom;
	CExtSafeString m_sBtnTextColorDefault;

	COLORREF m_clrDefault;
 
	DECLARE_SERIAL( CExtGridCellColor );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellColor, CExtGridCellEx );
	CExtGridCellColor(
		CExtGridDataProvider * pDataProvider = NULL,
		COLORREF clr = RGB(0,0,0),
		CSize sizeIcon = CSize( 20, 13 ),
		bool bEnableButtonRandomColor = false,
		bool bEnableButtonCustomColor = true,
		bool bEnableButtonDefaultColor = false
		);
	CExtGridCellColor(
		CExtGridDataProvider * pDataProvider,
		INT nSysColorIdx,
		CSize sizeIcon = CSize( 20, 13 ),
		bool bEnableButtonRandomColor = false,
		bool bEnableButtonCustomColor = true,
		bool bEnableButtonDefaultColor = false
		);
	CExtGridCellColor(
		CExtGridDataProvider * pDataProvider,
		CPackedColor & PackedColor,
		CSize sizeIcon = CSize( 20, 13 ),
		bool bEnableButtonRandomColor = false,
		bool bEnableButtonCustomColor = true,
		bool bEnableButtonDefaultColor = false
		);
	CExtGridCellColor( const CExtGridCell & other );

#ifdef _DEBUG
	virtual void AssertValid() const;
#endif // _DEBUG

	virtual e_Mode_t GetMode() const;
	virtual e_Mode_t SetMode( e_Mode_t eMode );

	virtual void Clear();
	virtual bool IsSysColor() const;
	virtual INT GetSysColorIdx() const;
	virtual COLORREF GetColor() const;
	virtual __EXT_MFC_SAFE_LPCTSTR GetColorName( 
		COLORREF clr 
		) const;
	virtual COLORREF GetColorRGB( 
		__EXT_MFC_SAFE_LPCTSTR lpszColorName 
		) const;
	virtual void SetColor( 
		COLORREF clr,
		bool bSetAlpha = true
		);
	virtual void SetColor( 
		INT nSysColorIdx 
		);
	virtual void SetRandomizedColor(
		DWORD nRedMin = DWORD(0),
		DWORD nRedMax = DWORD(255),
		DWORD nGreenMin = DWORD(0),
		DWORD nGreenMax = DWORD(255),
		DWORD nBlueMin = DWORD(0),
		DWORD nBlueMax = DWORD(255)
		);

	// virtual methods
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;
	virtual void TextGet( CExtSafeString & strCopy ) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	virtual void GetVariant( VARIANT & varCopy ) const;
	virtual bool IsEmpty() const;
	virtual void Empty();
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	virtual CSize IconGetSize() const;
	CSize IconSetSize( CSize size );

	virtual void OnPaintIcon(
		const CExtCmdIcon * pIcon,
		const RECT & rcIcon,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnButtonPressed(
		CExtGridWnd & wndGrid,
		INT nButtonType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnButtonPopupMenuTrack(
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual CRect OnQueryRestAreaMargins(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual bool OnInplaceControlPreTranslateMessage(
		MSG * pMsg,
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl
		);
	virtual HRESULT OnParseText(
		__EXT_MFC_SAFE_LPCTSTR sText
		) const;
	virtual bool OnParseColor( 
		__EXT_MFC_SAFE_LPCTSTR sText,
		COLORREF & clr
		) const;
	virtual void OnFormatCellText(
		CExtSafeString & strText,
		COLORREF clr
		) const;
	virtual void OnQueryListSeparator(
		CExtSafeString & str
		) const;

protected:
	CPackedColor m_PackedColor;
	e_Mode_t m_eMode;
	CSize m_sizeIcon;

}; // class CExtGridCellColor

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellVariant

class __PROF_UIS_API CExtGridCellVariant
	: public CExtGridCellEx
	, public VARIANT
{
public:
	DECLARE_SERIAL( CExtGridCellVariant );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellVariant, CExtGridCellEx );
	signed __int64 & _GetInt64Ref();
	unsigned __int64 & _GetUInt64Ref();
	signed __int64 *& _GetInt64PtrRef();
	unsigned __int64 *& _GetUInt64PtrRef();
protected:
	void _VariantInit();
public:
	bool _VariantClear(
		bool bConvertToErrorIfFailed = true
		);
	bool _VariantAssign(
		const VARIANT & other,
		bool bConvertToErrorIfFailed = true
		);
	void _VariantAssign(
		IDispatch * pInterface,
		bool bAllowNullInterface = true
		);
	void _VariantAssign(
		IUnknown * pInterface,
		bool bAllowNullInterface = true
		);
	void _VariantAssign(
		IDispatch ** ppInterface,
		bool bAllowNullInterface = true
		);
	void _VariantAssign(
		IUnknown ** ppInterface,
		bool bAllowNullInterface = true
		);
	void _VariantAssign(
		const SAFEARRAY * pSA,
		bool bAllowConvertToError = true
		);
	void _VariantAssign( const SAFEARRAY ** ppSA );
	void _VariantAssign( const DECIMAL & decimalVal );
	void _VariantAssign( const DECIMAL ** decimalVal );
//	bool _VariantAssign( BSTR bstrOther );
//	bool _VariantAssign( BSTR * bstrOther );
	bool _VariantAssign( const char * strOther );
	bool _VariantAssign( const unsigned short * strOther );
#if _MFC_VER >= 0x700
#if (defined _UNICODE) && (defined _WCHAR_T_DEFINED)
	bool _VariantAssign( const __wchar_t * strOther );
#endif // (defined _UNICODE) && (defined _WCHAR_T_DEFINED)
#endif // _MFC_VER >= 0x700
	void _VariantAssign( bool bSrc );
	void _VariantAssign( signed char nSrc );
	void _VariantAssign( signed char * nSrc );
	void _VariantAssign( unsigned char nSrc );
	void _VariantAssign( unsigned char * nSrc );
	void _VariantAssign( signed short nSrc );
	void _VariantAssign( signed short * nSrc );
	void _VariantAssign( unsigned short nSrc );
	void _VariantAssign(
		unsigned short * nSrc,
		VARTYPE nVariantType = VT_UI2 // VT_UI2 or VT_BSTR only (VT_BYREF for VT_UI2 will be added automatically)
		);
#if _MFC_VER >= 0x700
#if (defined _UNICODE) && (defined _WCHAR_T_DEFINED)
	void _VariantAssign(
		__wchar_t * nSrc,
		VARTYPE nVariantType = VT_UI2 // VT_UI2 or VT_BSTR only (VT_BYREF for VT_UI2 will be added automatically)
		);
#endif // (defined _UNICODE) && (defined _WCHAR_T_DEFINED)
#endif // _MFC_VER >= 0x700
	void _VariantAssign(
		signed int nSrc,
		VARTYPE nVariantType = VT_I4 // VT_I4 or VT_INT only
		);
	void _VariantAssign(
		signed int * nSrc,
		VARTYPE nVariantType = VT_I4 // VT_I4 or VT_INT only (VT_BYREF will be added automatically)
		);
	void _VariantAssign(
		unsigned int nSrc,
		VARTYPE nVariantType = VT_UI4 // VT_UI4 or VT_UINT only
		);
	void _VariantAssign(
		unsigned int * nSrc,
		VARTYPE nVariantType = VT_UI4 // VT_UI4 or VT_UINT only (VT_BYREF will be added automatically)
		);
	void _VariantAssign(
		signed long nSrc,
		VARTYPE nVariantType = VT_I4 // VT_I4 or VT_ERROR only
		);
	void _VariantAssign(
		signed long * nSrc,
		VARTYPE nVariantType = VT_I4 // VT_I4 or VT_ERROR only (VT_BYREF will be added automatically)
		);
	void _VariantAssign( unsigned long nSrc );
	void _VariantAssign( unsigned long * nSrc );
	void _VariantAssign( signed __int64 nSrc );
	void _VariantAssign( signed __int64 * nSrc );
	void _VariantAssign( unsigned __int64 nSrc );
	void _VariantAssign( unsigned __int64 * nSrc );
	void _VariantAssign( float nSrc );
	void _VariantAssign( float * nSrc );
	void _VariantAssign(
		double nSrc,
		VARTYPE nVariantType = VT_R8 // VT_R8 or VT_DATE only
		);
	void _VariantAssign(
		double * nSrc,
		VARTYPE nVariantType = VT_R8 // VT_R8 or VT_DATE only (VT_BYREF will be added automatically)
		);
	void _VariantAssign( CY cySrc );
	void _VariantAssign( CY * cySrc );
	bool _VariantIsEqual(
		const VARIANT & other,
		LCID _lcid_ = LOCALE_USER_DEFAULT,
		ULONG nVarCmpFlagsFlags = 0L
		) const;
	bool _VariantIsInequal(
		const VARIANT & other,
		LCID _lcid_ = LOCALE_USER_DEFAULT,
		ULONG nVarCmpFlagsFlags = 0L
		) const;
	bool _VariantIsLess(
		const VARIANT & other,
		LCID _lcid_ = LOCALE_USER_DEFAULT,
		ULONG nVarCmpFlagsFlags = 0L
		) const;
	bool _VariantIsGreater(
		const VARIANT & other,
		LCID _lcid_ = LOCALE_USER_DEFAULT,
		ULONG nVarCmpFlagsFlags = 0L
		) const;
	bool _VariantIsLessOrEqual(
		const VARIANT & other,
		LCID _lcid_ = LOCALE_USER_DEFAULT,
		ULONG nVarCmpFlagsFlags = 0L
		) const;
	bool _VariantIsGreaterOrEqual(
		const VARIANT & other,
		LCID _lcid_ = LOCALE_USER_DEFAULT,
		ULONG nVarCmpFlagsFlags = 0L
		) const;
	int _VariantCompare(
		const VARIANT & other,
		LCID _lcid_ = LOCALE_USER_DEFAULT,
		ULONG nVarCmpFlagsFlags = 0L
		) const;
	bool _VariantChangeType(
		VARTYPE vtNewType,
		const VARIANT * pVariantOther = NULL,
		bool bConvertToErrorIfFailed = true
		);

protected:
	void _AssignA( const CExtGridCellVariant & other );
public:
	CExtGridCellVariant(
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant( const CExtGridCellVariant & other );
	CExtGridCellVariant( const CExtGridCell & other );
	CExtGridCellVariant(
		const VARIANT & other,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		IDispatch * pInterface,
		bool bAllowNullInterface = true,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		IUnknown * pInterface,
		bool bAllowNullInterface = true,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		IDispatch ** ppInterface,
		bool bAllowNullInterface = true,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		IUnknown ** ppInterface,
		bool bAllowNullInterface = true,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		const SAFEARRAY * pSA,
		bool bAllowConvertToError = true,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		const SAFEARRAY ** ppSA,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		const DECIMAL & decimalVal,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		const DECIMAL ** decimalVal,
		CExtGridDataProvider * pDataProvider = NULL
		);
//	CExtGridCellVariant( BSTR bstrOther, CExtGridDataProvider * pDataProvider = NULL );
//	CExtGridCellVariant( BSTR * bstrOther, CExtGridDataProvider * pDataProvider = NULL );
	CExtGridCellVariant(
		const char * strOther,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		const unsigned short * strOther,
		CExtGridDataProvider * pDataProvider = NULL
		);
#if _MFC_VER >= 0x700
#if (defined _UNICODE) && (defined _WCHAR_T_DEFINED)
	CExtGridCellVariant(
		const __wchar_t * strOther,
		CExtGridDataProvider * pDataProvider = NULL
		);
#endif // (defined _UNICODE) && (defined _WCHAR_T_DEFINED)
#endif // _MFC_VER >= 0x700
	CExtGridCellVariant(
		bool bSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		signed char nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		signed char * nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		unsigned char nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		unsigned char * nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		signed short nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		signed short * nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		unsigned short nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		unsigned short * nSrc,
		VARTYPE nVariantType = VT_UI2, // VT_UI2 or VT_BSTR only (VT_BYREF for VT_UI2 will be added automatically)
		CExtGridDataProvider * pDataProvider = NULL
		);
#if _MFC_VER >= 0x700
#if (defined _UNICODE) && (defined _WCHAR_T_DEFINED)
	CExtGridCellVariant(
		__wchar_t * nSrc,
		VARTYPE nVariantType = VT_UI2, // VT_UI2 or VT_BSTR only (VT_BYREF for VT_UI2 will be added automatically)
		CExtGridDataProvider * pDataProvider = NULL
		);
#endif // (defined _UNICODE) && (defined _WCHAR_T_DEFINED)
#endif // _MFC_VER >= 0x700
	CExtGridCellVariant(
		signed int nSrc,
		VARTYPE nVariantType = VT_I4, // VT_I4 or VT_INT only
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		signed int * nSrc,
		VARTYPE nVariantType = VT_I4, // VT_I4 or VT_INT only (VT_BYREF will be added automatically)
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		unsigned int nSrc,
		VARTYPE nVariantType = VT_UI4, // VT_UI4 or VT_UINT only
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		unsigned int * nSrc,
		VARTYPE nVariantType = VT_UI4, // VT_UI4 or VT_UINT only (VT_BYREF will be added automatically)
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		signed long nSrc,
		VARTYPE nVariantType = VT_I4, // VT_I4 or VT_ERROR only
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		signed long * nSrc,
		VARTYPE nVariantType = VT_I4, // VT_I4 or VT_ERROR only (VT_BYREF will be added automatically)
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		unsigned long nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		unsigned long * nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		signed __int64 nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		signed __int64 * nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		unsigned __int64 nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		unsigned __int64 * nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		float nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		float * nSrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		double nSrc,
		VARTYPE nVariantType = VT_R8, // VT_R8 or VT_DATE only
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		double * nSrc,
		VARTYPE nVariantType = VT_R8, // VT_R8 or VT_DATE only (VT_BYREF will be added automatically)
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		CY cySrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellVariant(
		CY * cySrc,
		CExtGridDataProvider * pDataProvider = NULL
		);
	virtual ~CExtGridCellVariant();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

	virtual void Serialize( CArchive & ar );
	virtual void Empty();
	virtual void Reset();
	virtual void Assign( const CExtGridCell & other );
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;
	virtual void TextGet( CExtSafeString & strCopy ) const;
	virtual void TextGet( 
		CExtSafeString & strCopy, 
		LPCVARIANT pvarValue 
		) const;
	virtual void OnFormatCellText( 
		CExtSafeString & strCopy, 
		LPCVARIANT pvarValue 
		) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	virtual UINT OnQueryDrawTextFlags(
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags,
		bool bIncludeHorizontalFlags = true,
		bool bIncludeVerticalFlags = true,
		bool bIncludeOtherFlags = true
		) const;
	virtual void GetVariant( VARIANT & varCopy ) const;
	CExtGridCellVariant & operator = (  const CExtGridCell & other );
	CExtGridCellVariant & operator = (  const VARIANT & other );
	CExtGridCellVariant & operator = ( IDispatch * pInterface );
	CExtGridCellVariant & operator = ( IUnknown * pInterface );
	CExtGridCellVariant & operator = ( IDispatch ** ppInterface );
	CExtGridCellVariant & operator = ( IUnknown ** ppInterface );
	CExtGridCellVariant & operator = ( const SAFEARRAY * pSA );
	CExtGridCellVariant & operator = ( const SAFEARRAY ** ppSA );
	CExtGridCellVariant & operator = ( const DECIMAL & decimalVal );
	CExtGridCellVariant & operator = ( const DECIMAL ** decimalVal );
	bool operator == ( const CExtGridCellVariant & other ) const;
	bool operator == ( const VARIANT & other ) const;
	bool operator != ( const CExtGridCellVariant & other ) const;
	bool operator != ( const VARIANT & other ) const;
	bool operator < ( const CExtGridCellVariant & other ) const;
	bool operator < ( const VARIANT & other ) const;
	bool operator > ( const CExtGridCellVariant & other ) const;
	bool operator > ( const VARIANT & other ) const;
	bool operator <= ( const CExtGridCellVariant & other ) const;
	bool operator <= ( const VARIANT & other ) const;
	bool operator >= ( const CExtGridCellVariant & other ) const;
	bool operator >= ( const VARIANT & other ) const;
//	CExtGridCellVariant & operator = ( BSTR bstrOther );
//	CExtGridCellVariant & operator = ( BSTR * bstrOther );
	CExtGridCellVariant & operator = ( const char * strOther );
	CExtGridCellVariant & operator = ( const unsigned short * strOther );
#if _MFC_VER >= 0x700
#if (defined _UNICODE) && (defined _WCHAR_T_DEFINED)
	CExtGridCellVariant & operator = ( const __wchar_t * strOther );
#endif // (defined _UNICODE) && (defined _WCHAR_T_DEFINED)
#endif // _MFC_VER >= 0x700
	CExtGridCellVariant & operator = ( bool bSrc );
	CExtGridCellVariant & operator = ( signed char nSrc );
	CExtGridCellVariant & operator = ( signed char * nSrc );
	CExtGridCellVariant & operator = ( unsigned char nSrc );
	CExtGridCellVariant & operator = ( unsigned char * nSrc );
	CExtGridCellVariant & operator = ( signed short nSrc );
	CExtGridCellVariant & operator = ( signed short * nSrc );
	CExtGridCellVariant & operator = ( unsigned short nSrc );
	CExtGridCellVariant & operator = ( unsigned short * nSrc );
#if _MFC_VER >= 0x700
#if (defined _UNICODE) && (defined _WCHAR_T_DEFINED)
	CExtGridCellVariant & operator = ( __wchar_t * nSrc );
#endif // (defined _UNICODE) && (defined _WCHAR_T_DEFINED)
#endif // _MFC_VER >= 0x700
	CExtGridCellVariant & operator = ( signed int nSrc );
	CExtGridCellVariant & operator = ( signed int * nSrc );
	CExtGridCellVariant & operator = ( unsigned int nSrc );
	CExtGridCellVariant & operator = ( unsigned int * nSrc );
	CExtGridCellVariant & operator = ( signed long nSrc );
	CExtGridCellVariant & operator = ( signed long * nSrc );
	CExtGridCellVariant & operator = ( unsigned long nSrc );
	CExtGridCellVariant & operator = ( unsigned long * nSrc );
	CExtGridCellVariant & operator = ( signed __int64 nSrc );
	CExtGridCellVariant & operator = ( signed __int64 * nSrc );
	CExtGridCellVariant & operator = ( unsigned __int64 nSrc );
	CExtGridCellVariant & operator = ( unsigned __int64 * nSrc );
	CExtGridCellVariant & operator = ( float nSrc );
	CExtGridCellVariant & operator = ( float * nSrc );
	CExtGridCellVariant & operator = ( double nSrc );
	CExtGridCellVariant & operator = ( double * nSrc );
	CExtGridCellVariant & operator = ( CY cySrc );
	CExtGridCellVariant & operator = ( CY * cySrc );
	virtual HRESULT OnParseText(
		__EXT_MFC_SAFE_LPCTSTR sText
		) const;
	virtual bool OnQueryEnabledIncrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnQueryEnabledDecrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnQueryEnabledIncrement(
		LPCVARIANT pvarValue,
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnQueryEnabledDecrement(
		LPCVARIANT pvarValue,
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnValueIncrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnValueDecrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnValueIncrement(
		LPVARIANT pvarValue,
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnValueDecrement(
		LPVARIANT pvarValue,
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual bool OnInplaceControlPreTranslateMessage(
		MSG * pMsg,
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl
		);

protected:
	void _DoValueIterate( 
		HWND hWndInplaceControl,
		bool bNext, 
		bool bSetSel = false 
		);

}; // class CExtGridCellVariant

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellBool

class __PROF_UIS_API CExtGridCellBool : public CExtGridCellEx
{
public:
	static CExtSafeString g_strTextTrue;
	static CExtSafeString g_strTextFalse;
	static CExtSafeString g_strTextIndeterminate;
	DECLARE_SERIAL( CExtGridCellBool );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellBool, CExtGridCellEx );
	CExtGridCellBool(
		CExtGridDataProvider * pDataProvider = NULL
		);

	virtual bool DataGet() const;
	virtual bool DataSet( bool bData );

	virtual __EXT_MFC_SAFE_LPCTSTR LabelTextGet( bool bData ) const;
	virtual void LabelTextSet( bool bData, __EXT_MFC_SAFE_LPCTSTR str );

	virtual void Empty();
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;
	virtual void TextGet( CExtSafeString & strCopy ) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	virtual void GetVariant( VARIANT & varCopy ) const;

	virtual HRESULT OnParseText(
		__EXT_MFC_SAFE_LPCTSTR sText
		) const;

	virtual void OnInplaceControlQueryStyle(
		CExtGridWnd & wndGrid,
		CWnd * pWndInplaceControlToCreate,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		bool & bReadOnly,
		bool & bPassword,
		COLORREF & clrBack,
		COLORREF & clrText
		);
	virtual CSize OnPopupListBoxCalcItemExtraSizes(
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LONG nItemNo
		) const;
	virtual bool OnPopupListBoxInitContent(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnPopupListBoxSelEndOK(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual bool OnInplaceControlPreTranslateMessage(
		MSG * pMsg,
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl
		);
	virtual bool OnQueryEnabledIncrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnQueryEnabledDecrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnValueIncrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnValueDecrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);

protected:
    CExtSafeString	m_sLabelTrue;
    CExtSafeString	m_sLabelFalse;
}; // class CExtGridCellBool

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellComboBox

class __PROF_UIS_API CExtGridCellComboBox : public CExtGridCellString
{
public:
	DECLARE_SERIAL( CExtGridCellComboBox );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellComboBox, CExtGridCellString );
	CExtGridCellComboBox(
		CExtGridDataProvider * pDataProvider = NULL
		);
	virtual ~CExtGridCellComboBox();

	virtual void Empty();
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	virtual HRESULT OnParseText(
		__EXT_MFC_SAFE_LPCTSTR sText
		) const;

	class __PROF_UIS_API ITEM_INFO
	{
	public:
		ITEM_INFO()
			: m_sString(_T(""))
			, m_dwItemData(0)
			, m_nImage( -1 )
			, m_nSelectedImage( -1 )
			, m_nOverlayImage( -1 )
			, m_nIndent( 0 )
			, m_nCheck( 0 )
			, m_bEnabled( true )
			, m_clrText( COLORREF( -1L ) )
			, m_clrBack( COLORREF( -1L ) )
		{
		}
		ITEM_INFO(
			__EXT_MFC_SAFE_LPCTSTR lpszString,
			DWORD dwItemData = 0,
			INT nImage = -1,
			INT nSelectedImage = -1,
			INT nOverlayImage = -1,
			INT nIndent = 0,
			INT nCheck = 0,
			bool bEnabled = true,
			COLORREF clrText = COLORREF( -1L ),
			COLORREF clrBack = COLORREF( -1L )
			)
			: m_sString( lpszString )
			, m_dwItemData( dwItemData )
			, m_nImage( nImage )
			, m_nSelectedImage( nSelectedImage )
			, m_nOverlayImage( nOverlayImage )
			, m_nIndent( nIndent )
			, m_nCheck( nCheck )
			, m_bEnabled( bEnabled )
			, m_clrText( clrText )
			, m_clrBack( clrBack )
		{
		}
		ITEM_INFO( const ITEM_INFO & other )
			: m_sString( other.m_sString )
			, m_dwItemData( other.m_dwItemData )
			, m_nImage( other.m_nImage )
			, m_nSelectedImage( other.m_nSelectedImage )
			, m_nOverlayImage( other.m_nOverlayImage )
			, m_nIndent( other.m_nIndent )
			, m_nCheck( other.m_nCheck )
			, m_bEnabled( other.m_bEnabled )
			, m_clrText( other.m_clrText )
			, m_clrBack( other.m_clrBack )
		{
		}
		virtual ~ITEM_INFO()
		{
		}
		ITEM_INFO & operator = ( const ITEM_INFO & other )
		{
			m_sString = other.m_sString;
			m_dwItemData = other.m_dwItemData;
			m_nImage = other.m_nImage;
			m_nSelectedImage = other.m_nSelectedImage;
			m_nOverlayImage = other.m_nOverlayImage;
			m_nIndent = other.m_nIndent;
			m_nCheck = other.m_nCheck;
			m_bEnabled = other.m_bEnabled;
			m_clrText = other.m_clrText;
			m_clrBack = other.m_clrBack;
			return (*this);
		}
		void Serialize( CArchive & ar )
		{
			if( ar.IsStoring() )
			{
				ar << m_sString;
				ar << m_dwItemData;
				ar << DWORD(m_nImage);
				ar << DWORD(m_nSelectedImage);
				ar << DWORD(m_nOverlayImage);
				ar << DWORD(m_nIndent);
				ar << DWORD(m_nCheck);
				BYTE bEnabled = m_bEnabled ? BYTE(1) : BYTE(0);
				ar << bEnabled;
				ar << DWORD(m_clrText);
				ar << DWORD(m_clrBack);
			} // if( ar.IsStoring() )
			else
			{
				ar >> m_sString;
				ar >> m_dwItemData;
				DWORD dwTmp;
				ar >> dwTmp;
				m_nImage = INT(dwTmp);
				ar >> dwTmp;
				m_nSelectedImage = INT(dwTmp);
				ar >> dwTmp;
				m_nOverlayImage = INT(dwTmp);
				ar >> dwTmp;
				m_nIndent = INT(dwTmp);
				ar >> dwTmp;
				m_nCheck = INT(dwTmp);
				BYTE bEnabled = 0;
				ar >> bEnabled;
				m_bEnabled = ( bEnabled == BYTE(1) ) ? true : false;
				ar >> dwTmp;
				m_clrText = COLORREF(dwTmp);
				ar >> dwTmp;
				m_clrBack = COLORREF(dwTmp);
			} // else from if( ar.IsStoring() )
		}

		CExtSafeString m_sString;
		DWORD m_dwItemData;

		// Zero-based index of an image within the image list. 
		// The specified image will be displayed for the item when it is not selected.
		INT m_nImage; 

		// Zero-based index of an image within the image list. 
		// The specified image will be displayed for the item when it is selected.
		INT m_nSelectedImage;
		
		// Zero-based index of an overlay image within the image list. 
		INT m_nOverlayImage;

		// Number of indent spaces to display for the item. 
		// Each indentation equals 10 pixels by default. 
		INT m_nIndent;

		// Checked/unchecked state (used in CExtGridCellCheckListComboBox)
		INT m_nCheck;

		// Enabled/Disabled state
		bool m_bEnabled:1;

		// Text color
		COLORREF m_clrText;

		// Background color
		COLORREF m_clrBack;

	}; // struct ITEM_INFO

	void SetImageList(
		CImageList * pImageList 
		);

	CImageList * GetImageList() const;

	LONG AddString( 
		__EXT_MFC_SAFE_LPCTSTR lpszString,
		DWORD dwItemData = 0
		);

	LONG InsertString(
		__EXT_MFC_SAFE_LPCTSTR lpszString,
		DWORD dwItemData = 0,
		LONG nIndex = -1 // default - append
		);

	LONG AddItem( 
		ITEM_INFO * pItemInfo 
		);

	LONG InsertItem(
		ITEM_INFO * pItemInfo,
		LONG nIndex = -1 // default - append
		);

	__EXT_MFC_SAFE_LPCTSTR GetItemString(
		LONG nIndex 
		) const;

	bool SetItemString(
		LONG nIndex,
		__EXT_MFC_SAFE_LPCTSTR lpszString
		);

	bool SetItemData(
		LONG nIndex,
		DWORD dwItemData 
		);

	DWORD GetItemData(
		LONG nIndex 
		) const;

	bool SetItemImages(
		LONG nIndex,
		INT nImage,
		INT nSelectedImage,
		INT nOverlayImage
		);

	bool GetItemImages(
		LONG nIndex,
		INT * pnImage,
		INT * pnSelectedImage = NULL,
		INT * pnOverlayImage = NULL
		);

	bool SetItemIndent(
		LONG nIndex,
		INT nIndent 
		);

	INT GetItemIndent(
		LONG nIndex 
		) const;

	ITEM_INFO * GetItem(
		LONG nIndex 
		) const;

	bool SetItem(
		LONG nIndex,
		ITEM_INFO * pItemInfo
		) const;

	bool DeleteItem(
		LONG nIndex 
		);

	void ResetContent();

	LONG GetCount() const;

	bool SetCurSel(
		LONG nIndex,
		bool bChangeCellText = true
		);	

	LONG GetCurSel() const;

	LONG FindString(
		DWORD dwItemData,
		LONG nIndexStart = -1
		) const;

	LONG FindString(
		__EXT_MFC_SAFE_LPCTSTR lpszString,
		LONG nIndexStart = -1,
		bool bCaseSensitive = true
		) const;

	LONG FindStringExact(
		__EXT_MFC_SAFE_LPCTSTR lpszString,
		LONG nIndexStart = -1,
		bool bCaseSensitive = false
		) const;

	LONG SelectString(
	   __EXT_MFC_SAFE_LPCTSTR lpszString 
		);

	bool SetEnumMode( 
		bool bEnumMode = true 
		);
	bool GetEnumMode() const;	

	bool IsItemEnabled(
		LONG nIndex
		) const;
	void EnableItem(
		LONG nIndex,
		bool bEnable = true
		);

	void SetItemBackColor( 
		LONG nIndex,
		COLORREF clrBack = COLORREF(-1L)
		);
	COLORREF GetItemBackColor( 
		LONG nIndex
		) const;

	void SetItemTextColor( 
		LONG nIndex,
		COLORREF clrText = COLORREF(-1L)
		);
	COLORREF GetItemTextColor(
		LONG nIndex
		) const;

	// virtual methods
	virtual CSize OnPopupListBoxCalcItemExtraSizes(
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LONG nItemNo
		) const;
	virtual bool OnPopupListBoxInitContent(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnPopupListBoxSelEndOK(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnPopupListBoxItemDraw(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LPDRAWITEMSTRUCT pDIS
		);
	virtual bool OnPopupListBoxItemMeasure(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LPMEASUREITEMSTRUCT pMIS
		);
	virtual bool OnInplaceControlPreTranslateMessage(
		MSG * pMsg,
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl
		);
	virtual void OnInplaceControlTextInputComplete(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		__EXT_MFC_SAFE_LPCTSTR sTextNew,
		bool bSaveChanges
		);
	virtual bool OnQueryEnabledIncrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnQueryEnabledDecrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnValueIncrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnValueDecrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);

	virtual bool OnKey(
		CExtGridWnd & wndGrid,
		bool bKeyDownEvent, // true - key-down event, false - key-up event
		UINT nChar, // virtual key code
		UINT nRepCnt, // key-down/key-up press count
		UINT nFlags // key-down/key-up event flags
		);

	virtual HFONT OnCreateListBoxItemFont( 
		const CExtGridWnd & wndGrid,
		LONG nItem 
		);
	
	virtual void OnPopupListBoxQueryItemIcon(
		LONG nItem, 
		CExtCmdIcon & _iconNormal,
		CExtCmdIcon & _iconOverlay,
		CExtCmdIcon & _iconSelected
		) const;
	virtual INT OnPopupListBoxQueryItemIndent( LONG nItem ) const;
	virtual INT OnPopupListBoxQueryItemIndentWidth() const;

	virtual CSize IconGetSize() const;
	virtual void OnPaintIcon(
		const CExtCmdIcon * pIcon,
		const RECT & rcIcon,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual bool OnQueryButtonInfo(
		INT nButtonType,
		CExtGridWnd * pWndGrid = NULL,
		LONG nColNo = 0,
		LONG nRowNo = 0,
		INT nColType = 0,
		INT nRowType = 0,
		bool * p_bEnabled = NULL,
		bool * p_bPressed = NULL,
		bool * p_bStayPressed = NULL,
		UINT * p_nTimerElapseValue = NULL
		) const;
	virtual COLORREF OnQueryTextColor(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual COLORREF OnQueryBackColor(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	
protected:
	bool m_bEnumMode:1;
	CImageList * m_pImageList;

	void _DoItemsIterate( 
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		HWND hWndInplaceControl,
		bool bNext, 
		bool bSetSel = false 
		);
	BOOL _IsItemIndexValid( 
		LONG nIndex 
		) const;
	LONG _GetIndexOf( 
		const ITEM_INFO * pItemInfo 
		) const;
	ITEM_INFO * m_pCurSel;
	
public:
	virtual INT OnCompareItems(
		const ITEM_INFO * element1,
		const ITEM_INFO * element2
		);
	virtual void SortItems(
		bool bAscending = true
		);

protected:
	class __PROF_UIS_API CComboItemsArray : public CExtArray < ITEM_INFO * >
	{
	public:
		CExtGridCellComboBox * m_pGridCellComboBox;
		CComboItemsArray()
			: m_pGridCellComboBox( NULL )
		{
		}
		virtual int OnCompareItems(
			ITEM_INFO * const & element1,
			ITEM_INFO * const & element2
			) const
		{
			ASSERT_VALID( m_pGridCellComboBox );
			return m_pGridCellComboBox->OnCompareItems( element1, element2 );
		}
	}; // class CComboItemsArray

	CComboItemsArray m_arrItems;
}; // class CExtGridCellComboBox

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellDropListComboBox

class __PROF_UIS_API CExtGridCellDropListComboBox : public CExtGridCellComboBox
{
public:
	DECLARE_SERIAL( CExtGridCellDropListComboBox );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellDropListComboBox, CExtGridCellComboBox );
	CExtGridCellDropListComboBox(
		CExtGridDataProvider * pDataProvider = NULL
		);
	virtual bool OnClick(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nRepCnt, // 0 - button up, 1 - single click, 2 - double click, 3 - post single click & begin editing
		UINT nFlags // mouse event flags
		);
}; // class CExtGridCellDropListComboBox

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellCheckListComboBox

class __PROF_UIS_API CExtGridCellCheckListComboBox : public CExtGridCellComboBox
{
public:
	DECLARE_SERIAL( CExtGridCellCheckListComboBox );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellCheckListComboBox, CExtGridCellComboBox );
	CExtGridCellCheckListComboBox(
		CExtGridDataProvider * pDataProvider = NULL
		);

	void ItemCheckAll(
		INT nCheck
		);
	bool ItemCheckSet(
		LONG nIndex,
		INT nCheck
		);
	INT ItemCheckGet(
		LONG nIndex 
		) const;

	// virtual methods
	virtual void Empty();
	virtual void TextGet( CExtSafeString & strCopy ) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	virtual bool OnPopupListBoxInitContent(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnPopupListBoxItemDraw(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LPDRAWITEMSTRUCT pDIS
		);
	virtual bool OnPopupListBoxItemMeasure(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LPMEASUREITEMSTRUCT pMIS
		);
	virtual bool OnPopupListBoxClose(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnPopupListBoxItemClick(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnQueryEnabledIncrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnQueryEnabledDecrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnValueIncrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnValueDecrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual CSize IconGetSize() const;
	virtual void OnPaintIcon(
		const CExtCmdIcon * pIcon,
		const RECT & rcIcon,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;

	virtual void OnQueryListSeparator(
		CExtSafeString & str
		) const;

}; // class CExtGridCellCheckListComboBox

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellPassword

class __PROF_UIS_API CExtGridCellPassword : public CExtGridCellString
{
public:
	DECLARE_SERIAL( CExtGridCellPassword );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellPassword, CExtGridCellString );
	CExtGridCellPassword(
		CExtGridDataProvider * pDataProvider = NULL
		);

	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;

	// virtual methods
	virtual void OnInplaceControlQueryStyle(
		CExtGridWnd & wndGrid,
		CWnd * pWndInplaceControlToCreate,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		bool & bReadOnly,
		bool & bPassword,
		COLORREF & clrBack,
		COLORREF & clrText
		);
	virtual void OnPaintText(
		const RECT & rcCellText,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;

}; // class CExtGridCellPassword

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellFont

class __PROF_UIS_API CExtGridCellFont : public CExtGridCellEx
{
public:
	DECLARE_SERIAL( CExtGridCellFont );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellFont, CExtGridCellEx );
	CExtGridCellFont(
		CExtGridDataProvider * pDataProvider = NULL
		);

	LOGFONT DataGet() const;
	LOGFONT DataSet( const LOGFONT & lfData );

	COLORREF FontColorGet() const;
	COLORREF FontColorSet( COLORREF clrFont );

	virtual void Empty();
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	// virtual methods
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;
	virtual void TextGet( CExtSafeString & strCopy ) const;
	virtual void GetVariant( VARIANT & varCopy ) const;

	virtual void OnButtonPressed(
		CExtGridWnd & wndGrid,
		INT nButtonType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual void OnInplaceControlQueryStyle(
		CExtGridWnd & wndGrid,
		CWnd * pWndInplaceControlToCreate,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		bool & bReadOnly,
		bool & bPassword,
		COLORREF & clrBack,
		COLORREF & clrText
		);
	virtual bool OnInplaceControlPreTranslateMessage(
		MSG * pMsg,
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl
		);
	virtual COLORREF OnQueryTextColor(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnFormatCellText(
		CExtSafeString & strText,
		LOGFONT & lfData
		) const;
	
protected:
    LOGFONT m_lfData;
    COLORREF m_clrFont;

}; // class CExtGridCellFont

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellFolder

class __PROF_UIS_API CExtGridCellFolder : public CExtGridCellString
{
public:
	DECLARE_SERIAL( CExtGridCellFolder );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellFolder, CExtGridCellString );
	CExtGridCellFolder(
		CExtGridDataProvider * pDataProvider = NULL
		);

	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	UINT FolderDialogStyleSet( UINT ulFlags );
	UINT FolderDialogStyleGet() const;

	void FolderDialogTitleSet( __EXT_MFC_SAFE_LPCTSTR lpszTitle );
	__EXT_MFC_SAFE_LPCTSTR FolderDialogTitleGet() const;

	void FolderDialogRootSet( __EXT_MFC_SAFE_LPCTSTR lpszRoot );
	__EXT_MFC_SAFE_LPCTSTR FolderDialogRootGet() const;

	// virtual methods
	virtual void OnButtonPressed(
		CExtGridWnd & wndGrid,
		INT nButtonType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnInplaceControlPreTranslateMessage(
		MSG * pMsg,
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl
		);

protected:
#if ( defined __EXT_MFC_NO_SHELL_DIALOG_BFF )
	static int CALLBACK stat_BrowseCallbackProc(
		HWND hWnd, 
		UINT uMsg, 
		LPARAM lParam, 
		LPARAM lpData
		);
#endif // ( defined __EXT_MFC_NO_SHELL_DIALOG_BFF )
	virtual bool GetFolder(
		__EXT_MFC_SAFE_LPCTSTR lpszTitle,
		__EXT_MFC_SAFE_LPTSTR lpszPath,
		__EXT_MFC_SAFE_LPCTSTR lpszRoot = NULL,
		HWND hWndOwner = NULL
		);
	UINT m_ulFolderDialogFlags;
	CExtSafeString m_sTitle;
	CExtSafeString m_sRoot;
}; // class CExtGridCellFolder

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellFile

#define __GRID_CELL_FILE_DEFAULT_FILTER		_T("All Files (*.*)|*.*||")
#define __GRID_CELL_FILE_DEFAULT_FLAGS		( OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER )

class __PROF_UIS_API CExtGridCellFile : public CExtGridCellString
{
public:
	DECLARE_SERIAL( CExtGridCellFile );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellFile, CExtGridCellString );
	CExtGridCellFile(
		CExtGridDataProvider * pDataProvider = NULL
		);

	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	DWORD FileDialogStyleSet( DWORD dwFlags );
	DWORD FileDialogStyleGet() const;

	void FileDialogDefExtSet( __EXT_MFC_SAFE_LPCTSTR lpszDefExt );
	__EXT_MFC_SAFE_LPCTSTR FileDialogDefExtGet() const;

	void FileDialogFilterSet( __EXT_MFC_SAFE_LPCTSTR lpszFilter );
	__EXT_MFC_SAFE_LPCTSTR FileDialogFilterGet() const;

	BOOL m_bOpenFileDialog;

	// virtual methods
	virtual void OnButtonPressed(
		CExtGridWnd & wndGrid,
		INT nButtonType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnInplaceControlPreTranslateMessage(
		MSG * pMsg,
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl
		);

protected:
	virtual bool GetFile(
		bool bOpen, // true for Open, false for SaveAs
		__EXT_MFC_SAFE_LPTSTR lpszFileName,
		__EXT_MFC_SAFE_LPCTSTR lpszDefExt = NULL,
		DWORD dwFlags = __GRID_CELL_FILE_DEFAULT_FLAGS,
		__EXT_MFC_SAFE_LPCTSTR lpszFilter = NULL,
		CWnd * pWndParent = NULL
		);

protected:
	DWORD m_dwFileDialogFlags;
	CExtSafeString m_sDefExt;
	CExtSafeString m_sFilter;

}; // class CExtGridCellFile

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellFileImage

class __PROF_UIS_API CExtGridCellFileImage : public CExtGridCellFile
{
public:
	DECLARE_SERIAL( CExtGridCellFileImage );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellFileImage, CExtGridCellFile );
	CExtGridCellFileImage(
		CExtGridDataProvider * pDataProvider = NULL
		);

	// virtual methods
	virtual CSize IconGetSize() const;
	virtual void OnPaintIcon(
		const CExtCmdIcon * pIcon,
		const RECT & rcIcon,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual CRect OnQueryRestAreaMargins(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;

}; // class CExtGridCellFileImage

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellHyperLink

class __PROF_UIS_API CExtGridCellHyperLink : public CExtGridCellString
{
public:
	DECLARE_SERIAL( CExtGridCellHyperLink );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellHyperLink, CExtGridCellString );
	CExtGridCellHyperLink(
		CExtGridDataProvider * pDataProvider = NULL
		);

	void SetTextColorVisited( COLORREF clrText );
	COLORREF GetTextColorVisited() const;
	
	void SetTextHoverUnderline( BOOL bSet );
	BOOL GetTextHoverUnderline() const;
	
	void SetAllowVisitedState( BOOL bSet );
	BOOL GetAllowVisitedState() const;

	void SetURL( __EXT_MFC_SAFE_LPCTSTR lpszURL );
	__EXT_MFC_SAFE_LPCTSTR GetURL() const;

	enum e_OpenMode_t
	{
		__EOM_OPEN = 0,
		__EOM_EDIT = 1,
		__EOM_PRINT = 2,
	};

	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	// virtual methods
	virtual HFONT OnQueryCellFont(
		const CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		bool & bFontMustBeDestroyed,
		DWORD dwHelperPaintFlags = 0
		) const;
	virtual COLORREF OnQueryTextColor(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual bool OnClick(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nRepCnt, // 0 - button up, 1 - single click, 2 - double click, 3 - post single click & begin editing
		UINT nFlags // mouse event flags
		);
	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual bool OnKey(
		CExtGridWnd & wndGrid,
		bool bKeyDownEvent, // true - key-down event, false - key-up event
		UINT nChar, // virtual key code
		UINT nRepCnt, // key-down/key-up press count
		UINT nFlags // key-down/key-up event flags
		);
	virtual bool OnSetCursor(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo
		);
	
	virtual bool OnHyperLinkOpen(
		__EXT_MFC_SAFE_LPCTSTR strURL,
		e_OpenMode_t eOM = __EOM_OPEN,
		bool bAllowErrorAlert = false
		) const;
	virtual bool OnHyperLinkOpenViaCOM(
		__EXT_MFC_SAFE_LPCTSTR strURL,
		e_OpenMode_t eOM = __EOM_OPEN,
		bool bAllowErrorAlert = false
		) const;
	virtual bool OnHyperLinkOpenViaShell(
		__EXT_MFC_SAFE_LPCTSTR strURL,
		e_OpenMode_t eOM = __EOM_OPEN,
		bool bAllowErrorAlert = false
		) const;
	virtual bool OnHyperLinkOpenViaRegInfo(
		__EXT_MFC_SAFE_LPCTSTR strURL,
		e_OpenMode_t eOM = __EOM_OPEN,
		bool bAllowErrorAlert = false
		) const;

protected:
	COLORREF m_clrTextVisited;
	BOOL m_bHoverTextUnderline;
	// target URL
	CExtSafeString m_strURL;
	// allow visited state highlighting
	BOOL m_bAllowVisitedState;
	mutable bool m_bVisited:1;

}; // class CExtGridCellHyperLink

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellCheckBox

class __PROF_UIS_API CExtGridCellCheckBox : public CExtGridCellString
{
public:
	DECLARE_SERIAL( CExtGridCellCheckBox );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellCheckBox, CExtGridCellString );
	CExtGridCellCheckBox(
		CExtGridDataProvider * pDataProvider = NULL
		);

	INT SetCheck( 
		INT nCheck
		);
	INT GetCheck() const;

	bool Set3StateMode( 
		bool b3State = true
		);
	bool Get3StateMode() const;

	bool SetAutoTextMode( 
		bool bAutoTextMode = true
		);
	bool GetAutoTextMode() const;

	// virtual methods
	virtual bool OnKey(
		CExtGridWnd & wndGrid,
		bool bKeyDownEvent, // true - key-down event, false - key-up event
		UINT nChar, // virtual key code
		UINT nRepCnt, // key-down/key-up press count
		UINT nFlags // key-down/key-up event flags
		);
	virtual bool OnClick(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nRepCnt, // 0 - button up, 1 - single click, 2 - double click, 3 - post single click & begin editing
		UINT nFlags // mouse event flags
		);
	virtual HRESULT OnParseText(
		__EXT_MFC_SAFE_LPCTSTR sText
		) const;
	virtual void GetVariant( VARIANT & varCopy ) const;
	virtual void TextGet( CExtSafeString & strCopy ) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);

	virtual __EXT_MFC_SAFE_LPCTSTR LabelTextGet( INT nCheck ) const;
	virtual void LabelTextSet( INT nCheck, __EXT_MFC_SAFE_LPCTSTR str );

	virtual void Empty();
	virtual void Assign( const CExtGridCell & other );
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;
	virtual void Serialize( CArchive & ar );

	virtual void OnInplaceControlQueryStyle(
		CExtGridWnd & wndGrid,
		CWnd * pWndInplaceControlToCreate,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		bool & bReadOnly,
		bool & bPassword,
		COLORREF & clrBack,
		COLORREF & clrText
		);

protected:
	bool m_b3StateMode:1, m_bAutoTextMode:1;
    CExtSafeString	m_sLabelTrue;
    CExtSafeString	m_sLabelFalse;
    CExtSafeString	m_sLabelIndeterminate;
}; // class CExtGridCellCheckBox

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellRadioButton

class __PROF_UIS_API CExtGridCellRadioButton : public CExtGridCellString
{
public:
	DECLARE_SERIAL( CExtGridCellRadioButton );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellRadioButton, CExtGridCellString );
	CExtGridCellRadioButton(
		CExtGridDataProvider * pDataProvider = NULL
		);

	INT SetCheck( 
		INT nCheck
		);
	INT GetCheck() const;

	// virtual methods
	virtual void GetVariant( VARIANT & varCopy ) const;

}; // class CExtGridCellRadioButton

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellDateTime

class __PROF_UIS_API CExtGridCellDateTime : public CExtGridCellEx
{
public:
	DECLARE_SERIAL( CExtGridCellDateTime );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellDateTime, CExtGridCellEx );
	CExtGridCellDateTime(
		CExtGridDataProvider * pDataProvider = NULL
		);

	enum eMode_t
	{
		all = 0,
		time = 1,
		date = 2,
	};
	enum eTimeFormat_t
	{
		h12 = 0,
		h24 = 1,
		automatic = 2,
	};
	enum eItem_t
	{
		year = 0,
		month = 1,
		day = 2,
		hour = 3,
		minute = 4,
		second = 5,
		designator = 6
	};

	eMode_t SetMode( eMode_t eMode );
	eMode_t GetMode() const;

	eTimeFormat_t SetTimeFormat( eTimeFormat_t eTimeFormat );
	eTimeFormat_t GetTimeFormat() const;

	void SetDateTime( const COleDateTime & dateSrc );
	void SetDateTime( const VARIANT & varSrc );
	void SetDateTime( DATE dtSrc );
	void SetDateTime( time_t timeSrc );
	void SetDateTime( const SYSTEMTIME & systimeSrc );
	void SetDateTime( const FILETIME & filetimeSrc );
	INT SetDateTime(
		INT nYear, INT nMonth, INT nDay,
		INT nHour, INT nMin, INT nSec
		);
	INT SetDate(INT nYear, INT nMonth, INT nDay);
	INT SetTime(INT nHour, INT nMin, INT nSec);

	COleDateTime GetDateTime() const;
	BOOL GetAsSystemTime( SYSTEMTIME & sysTime ) const;

	INT GetYear() const;
	INT GetMonth() const;       // month of year (1 = Jan)
	INT GetDay() const;         // day of month (0-31)
	INT GetHour() const;        // hour in day (0-23)
	INT GetMinute() const;      // minute in hour (0-59)
	INT GetSecond() const;      // second in minute (0-59)
	INT GetDayOfWeek() const;   // 1=Sun, 2=Mon, ..., 7=Sat
	INT GetDayOfYear() const;   // days since start of year, Jan 1 = 1

	const CExtGridCellDateTime & operator = ( const COleDateTime & dateSrc );
	const CExtGridCellDateTime & operator = ( const VARIANT & varSrc );
	const CExtGridCellDateTime & operator = ( DATE dtSrc );

	const CExtGridCellDateTime & operator = ( time_t timeSrc );
	const CExtGridCellDateTime & operator = ( const SYSTEMTIME & systimeSrc );
	const CExtGridCellDateTime & operator = ( const FILETIME & filetimeSrc );
	
	virtual void Empty();
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	// virtual methods
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;
	virtual void GetVariant( VARIANT & varCopy ) const;
	virtual void TextGet( CExtSafeString & strCopy ) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	
	virtual bool OnQueryEnabledIncrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnQueryEnabledDecrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnValueIncrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnValueDecrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);

	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual bool OnInplaceControlPreTranslateMessage(
		MSG * pMsg,
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl
		);
	virtual void OnInplaceControlDateTimeInputComplete(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		COleDateTime dtNew,
		bool bSaveChanges
		);
	virtual bool OnButtonPopupMenuTrack(
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool IsItemVisible( 
		eItem_t eItem
		) const;

protected:
	void _RemoveDateTimeField(
		CExtSafeString & sFormatStr,
		TCHAR chField,
		__EXT_MFC_SAFE_LPCTSTR sSeparator
		) const;

	static bool stat_CbInitDatePickerContent(
		CExtDatePickerWnd & wndDatePicker,
		CExtGridCellDateTime * pCell
		);
	static bool stat_CbDatePickerSelection(
		LPVOID pSelectionNotification,
		CExtGridCellDateTime * pCell
		);
	virtual bool OnPopupDatePickerInitContent(
		CExtDatePickerWnd & wndDatePicker
		);
	virtual bool OnPopupDatePickerSelChange(
		LPVOID pSelectionNotification
		);

	virtual DWORD OnQueryDatePickerStyle() const;
	
	virtual bool OnValueChanging(
		const COleDateTime & dtDateTimeOld,
		const COleDateTime & dtDateTimeNew
		) const;
	virtual bool OnValueChanged(
		const COleDateTime & dtDateTimeOld,
		const COleDateTime & dtDateTimeNew
		) const;

protected:
	eMode_t m_eMode;		
	eTimeFormat_t m_eTimeFormat;		
	COleDateTime m_dtDate;

}; // class CExtGridCellDateTime

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellDuration

class __PROF_UIS_API CExtGridCellDuration : public CExtGridCellEx
{
public:
	DECLARE_SERIAL( CExtGridCellDuration );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellDuration, CExtGridCellEx );
	CExtGridCellDuration(
		CExtGridDataProvider * pDataProvider = NULL
		);

	enum eItem_t
	{
		day = 1,
		hour = 2,
		minute = 3,
		second = 4,
	};

	void DurationSet( const COleDateTimeSpan & dtSpan );
	void DurationSet( double dSpan );
	void DurationSet( LONG nDays, INT nHours, INT nMins, INT nSecs );
	void DurationSet( LONG nTotalSeconds );

	COleDateTimeSpan DurationGet() const;
 
	const CExtGridCellDuration & operator = ( const COleDateTimeSpan & dtSpanSrc );
	const CExtGridCellDuration & operator = ( double dSpan );
	const CExtGridCellDuration & operator = ( LONG nTotalSeconds );

	bool ContinuousScrollingSet( 
		bool bSet = true 
		);
	bool ContinuousScrollingGet() const;

	void DurationMaxSet( LONG nMaxDuration );
	LONG DurationMaxGet() const;

	virtual void Empty();
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	// virtual methods
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;
	virtual void GetVariant( VARIANT & varCopy ) const;
	virtual void TextGet( CExtSafeString & strCopy ) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	
	LONG OnQueryMaxDuration() const;

	virtual bool OnQueryEnabledIncrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnQueryEnabledDecrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnValueIncrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnValueDecrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);

	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual void OnInplaceControlDurationInputComplete(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		COleDateTimeSpan dtSpanNew,
		bool bSaveChanges
		);
	virtual bool IsItemVisible( 
		eItem_t eItem
		) const;

protected:
	virtual bool OnValueChanging(
		const COleDateTimeSpan & dtSpanOld,
		const COleDateTimeSpan & dtSpanNew
		) const;
	virtual bool OnValueChanged(
		const COleDateTimeSpan & dtSpanOld,
		const COleDateTimeSpan & dtSpanNew
		) const;

protected:
	COleDateTimeSpan m_dtSpan;
	bool m_bContinuousScrolling:1;
	LONG m_nMaxDuration;

}; // class CExtGridCellDuration

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellFontFaceName

class __PROF_UIS_API CExtGridCellFontFaceName : public CExtGridCellComboBox
{
protected:
	static int CALLBACK stat_FontEnumProc(
		ENUMLOGFONTEX * lpelfe,
		NEWTEXTMETRICEX * lpntme, 
		int FontType,
		CExtGridCellFontFaceName * pCellComboFontFaceName
		);
public:
	static CExtCmdIcon g_IconFontTrueType;
	static CExtCmdIcon g_IconFontRaster;
	DECLARE_SERIAL( CExtGridCellFontFaceName );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellFontFaceName, CExtGridCellComboBox );
	CExtGridCellFontFaceName(
		CExtGridDataProvider * pDataProvider = NULL
		);
	virtual void SynchronizeFontList(
		BYTE aCharSet = DEFAULT_CHARSET
		);
	virtual int OnFontEnum(
		ENUMLOGFONTEX * lpelfe,
		NEWTEXTMETRICEX * lpntme, 
		int FontType
		);
	virtual CSize OnPopupListBoxCalcItemExtraSizes(
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LONG nItemNo
		) const;
	virtual CExtCmdIcon * OnQueryFontItemIcon( LONG nItem );
	virtual HFONT OnCreateListBoxItemFont( 
		const CExtGridWnd & wndGrid,
		LONG nItem 
		);
	virtual bool OnPopupListBoxItemDraw(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LPDRAWITEMSTRUCT pDIS
		);
	virtual CSize IconGetSize() const;
	virtual void OnPaintIcon(
		const CExtCmdIcon * pIcon,
		const RECT & rcIcon,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
}; // class CExtGridCellFontFaceName

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellProgress

class __PROF_UIS_API CExtGridCellProgress : public CExtGridCellString
{
public:
	DECLARE_SERIAL( CExtGridCellProgress );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellProgress, CExtGridCellString );
	CExtGridCellProgress(
		CExtGridDataProvider * pDataProvider = NULL
		);
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

	enum e_TextMode_t
	{
		eNone = 0,
		eText = 1,
		ePercent = 2,
		ePosition = 3,
		eTextAndPercent = 4,
		eTextAndPosition = 5,
	};
	e_TextMode_t TextModeSet(
		e_TextMode_t eTextMode
		);
	e_TextMode_t TextModeGet() const;

	void SetRange(
		INT nLower, 
		INT nUpper
		);
	void GetRange(
		INT & nLower, 
		INT & nUpper
		) const;

	INT GetPos() const;
	INT SetPos( INT nPos );

	INT OffsetPos( INT nPos );
	INT SetStep( INT nStep );
	INT StepIt();

	void BarColorsSet(
		COLORREF clrBarBegin = COLORREF( -1L ),
		COLORREF clrBarEnd = COLORREF( -1L )
		);
	void BarColorsGet(
		COLORREF & clrBarBegin,
		COLORREF & clrBarEnd
		) const;

	void TextColorsSet(
		COLORREF clrText = COLORREF( -1L ),
		COLORREF clrFilledText = COLORREF( -1L )
		);
	void TextColorsGet(
		COLORREF & clrText,
		COLORREF & clrFilledText
		) const;

	bool InvertBarGradientSet( 
		bool bSet = true 
		);
	bool InvertBarGradientGet() const;

	bool VerticalSet( 
		bool bSet = true 
		);
	bool VerticalGet() const;

	bool ReversedSet( 
		bool bSet = true 
		);
	bool ReversedGet() const;

	bool TiedTextSet( 
		bool bSet = true 
		);
	bool TiedTextGet() const;

protected:
	bool m_bVertical:1;
	bool m_bReversed:1;

	INT m_nRangeLower;
	INT m_nRangeUpper;
	INT m_nPos;
	INT m_nStep;

	COLORREF m_clrBarBegin;
	COLORREF m_clrBarEnd;
	COLORREF m_clrText;
	COLORREF m_clrFilledText;

	bool m_bInvertBarGradient;
	bool m_bTiedText:1;

	e_TextMode_t m_eTextMode;

public:

	// virtual methods
	virtual void Empty();
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);

	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;

	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual void OnPaintText(
		const RECT & rcCellText,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;

}; // class CExtGridCellProgress

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellNumberBase

class __PROF_UIS_API CExtGridCellNumberBase : public CExtGridCellVariant
{
public:
	DECLARE_SERIAL( CExtGridCellNumberBase );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellNumberBase, CExtGridCellVariant );
	CExtGridCellNumberBase(
		CExtGridDataProvider * pDataProvider = NULL
		);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

	virtual void OnFormatCellText( 
		CExtSafeString & strCopy, 
		LPCVARIANT pvarValue 
		) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	virtual HRESULT OnParseText(
		__EXT_MFC_SAFE_LPCTSTR sText
		) const;

	virtual void OnQueryTextForInplaceControl( 
		CExtSafeString & strCopy 
		) const;

public:
	virtual bool OnQueryNumberText( 
		LPCVARIANT pvarValue,
		CExtSafeString & sText 
		) const;
	virtual bool OnTextGetPrepareVariant( 
		LPVARIANT pvarValue 
		) const; 
	virtual UINT OnQueryNumDigits( 
		LPCVARIANT pvarValue = NULL 
		) const;
	virtual UINT OnQueryLeadingZero() const;
	virtual bool OnQueryTrailingZeroes() const;
	virtual void OnQueryGrouping( CExtSafeString & sVal ) const;
	virtual void OnQueryDecimalSeparator( CExtSafeString & sVal ) const;
	virtual void OnQueryThousandSeparator( CExtSafeString & sVal ) const;
	virtual UINT OnQueryNegativeOrder() const;
	
protected:
	virtual void OnValidateNumberText( CExtSafeString & sText ) const;

}; // class CExtGridCellNumberBase

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellNumber

class __PROF_UIS_API CExtGridCellNumber : public CExtGridCellNumberBase
{
public:
	DECLARE_SERIAL( CExtGridCellNumber );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellNumber, CExtGridCellNumberBase );
	CExtGridCellNumber(
		CExtGridDataProvider * pDataProvider = NULL
		);

	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

public:
	virtual UINT OnQueryNumDigits( 
		LPCVARIANT pvarValue = NULL
		) const;
	virtual UINT OnQueryLeadingZero() const;
	virtual bool OnQueryTrailingZeroes() const;
	virtual void OnQueryGrouping( CExtSafeString & sVal ) const;
	virtual void OnQueryDecimalSeparator( CExtSafeString & sVal ) const;
	virtual void OnQueryThousandSeparator( CExtSafeString & sVal ) const;
	virtual UINT OnQueryNegativeOrder() const;
	
	bool SetAllowTrailingZeroes( 
		bool bAllowTrailingZeroes = true 
		);
	bool GetAllowTrailingZeroes() const;	
	
	bool SetAutoNumDigits( 
		bool bAutoNumDigits = true 
		);
	bool GetAutoNumDigits() const;	

	INT SetNumDigits( 
		INT nNumDigits = -1
		);
	INT GetNumDigits() const;

	INT SetLeadingZero( 
		INT nLeadingZero = -1
		);
	INT GetLeadingZero() const;

	INT SetNegativeOrder( 
		INT nNegativeOrder = -1
		);
	INT GetNegativeOrder() const;

	void SetGrouping( 
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR( NULL ) 
		);
	void GetGrouping( 
		CExtSafeString & strCopy 
		) const;

	void SetDecimalSeparator( 
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR( NULL ) 
		);
	void GetDecimalSeparator( 
		CExtSafeString & strCopy 
		) const;

	void SetThousandSeparator( 
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR( NULL ) 
		);
	void GetThousandSeparator( 
		CExtSafeString & strCopy 
		) const;

protected:
	// Remove trailing zeroes
	bool m_bAllowTrailingZeroes:1;

	// Automatically calculates number of fractional digits
	bool m_bAutoNumDigits:1;

	// Specifies number of fractional digits. 
	// The default value is -1. In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_IDIGITS. 
	INT m_nNumDigits;

	// Specifies whether to use leading zeroes in decimal fields. 
	// The default value is -1. In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_ILZERO. 
	//
	// The specifier can be one of the following values:
	// 0 - No leading zeros 
	// 1 - Leading zeros 
	INT m_nLeadingZero;

	// Specifies format for a negative number. 
	// The default value is -1. In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_INEGNUMBER. 
	//
	// The mode can be one of the following values:
	// 0 - Left parenthesis, number, right parenthesis. Example: (1.1)  
	// 1 - Negative sign, number. Example: -1.1 
	// 2 - Negative sign, space, number. Example: - 1.1 
	// 3 - Number, negative sign. Example: 1.1- 
	// 4 - Number, space, negative sign. Example: 1.1 - 
	INT m_nNegativeOrder;

	// Specifies size of each group of digits to the left of the decimal. 
	// The default value is "" (empty). In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_SGROUPING. 
	//
	// An explicit size is needed for each group, and sizes are separated by semicolons. 
	// If the last value is zero, the preceding value is repeated. 
	// For example, to group thousands, specify 3;0. 
	// Indic locales group the first thousand and then group by hundreds 
	// for example 12,34,56,789, which is represented by 3;2;0. 
	CExtSafeString m_sGrouping;

	// Specifies character(s) used as the decimal separator. 
	// The default value is "" (empty). In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_SDECIMAL. 
	// The maximum number of characters allowed for this string is 4 (including a null terminator). 
	CExtSafeString m_sDecimalSeparator;

	// Specifies character(s) used to separate groups of digits to the left of the decimal. 
	// The default value is "" (empty). In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_STHOUSAND. 
	// The maximum number of characters allowed for this string is 4 (including a null terminator). 
	CExtSafeString m_sThousandSeparator;

}; // class CExtGridCellNumber

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellUpDown

class __PROF_UIS_API CExtGridCellUpDown : public CExtGridCellNumber
{
public:
	DECLARE_SERIAL( CExtGridCellUpDown );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellUpDown, CExtGridCellNumber );
	CExtGridCellUpDown(
		CExtGridDataProvider * pDataProvider = NULL
		);

}; // class CExtGridCellUpDown

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellUpDownFontWeight

class __PROF_UIS_API CExtGridCellUpDownFontWeight : public CExtGridCellUpDown
{
public:
	struct __PROF_UIS_API FontWeightDefinion
	{
		INT m_nWeightValue;
		LPCVOID m_pWeightName;
	};
	static const FontWeightDefinion g_arrDefaultFontWeights[9];
	DECLARE_SERIAL( CExtGridCellUpDownFontWeight );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellUpDownFontWeight, CExtGridCellUpDown );
	CExtGridCellUpDownFontWeight(
		CExtGridDataProvider * pDataProvider = NULL
		);
	virtual void OnQueryListBoxValues(
		CExtSafeStringArray & arrValueNames,
		CArray < INT, INT > & arrValueWeights
		) const;
	virtual bool OnPopupListBoxInitContent(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnQueryEnabledIncrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnQueryEnabledDecrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual HRESULT OnParseText(
		__EXT_MFC_SAFE_LPCTSTR sText
		) const;
	virtual bool OnPopupListBoxSelEndOK(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
}; // class CExtGridCellUpDownFontWeight

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellUpDownRectPart

class __PROF_UIS_API CExtGridCellUpDownRectPart : public CExtGridCellUpDown
{
public:
	enum e_cell_part_type_t
	{
		__ECPT_UNKNOWN = 0,
		__ECPT_X       = 1,
		__ECPT_Y       = 2,
		__ECPT_WIDTH   = 3,
		__ECPT_HEIGHT  = 4,
	};
	e_cell_part_type_t m_eCPT;
	DECLARE_SERIAL( CExtGridCellUpDownRectPart );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellUpDownRectPart, CExtGridCellUpDown );
	CExtGridCellUpDownRectPart(
		CExtGridDataProvider * pDataProvider = NULL
		);
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );
	virtual CExtCmdIcon * IconGet();
}; // class CExtGridCellUpDownRectPart

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellUpDownColorPart

class __PROF_UIS_API CExtGridCellUpDownColorPart : public CExtGridCellUpDown
{
public:
	enum e_cell_part_type_t
	{
		__ECPT_UNKNOWN = 0,
		__ECPT_RED     = 1,
		__ECPT_GREEN   = 2,
		__ECPT_BLUE    = 3,
	};
	e_cell_part_type_t m_eCPT;
	CSize m_sizeIcon;
	DECLARE_SERIAL( CExtGridCellUpDownColorPart );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellUpDownColorPart, CExtGridCellUpDown );
	CExtGridCellUpDownColorPart(
		CExtGridDataProvider * pDataProvider = NULL
		);
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );
	virtual CSize IconGetSize() const;
	virtual void OnPaintIcon(
		const CExtCmdIcon * pIcon,
		const RECT & rcIcon,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnQueryListBoxValues(
		CArray < INT, INT > & arrValues
		) const;
	virtual bool OnPopupListBoxInitContent(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual bool OnPopupListBoxItemDraw(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LPDRAWITEMSTRUCT pDIS
		);
	virtual bool OnPopupListBoxItemMeasure(
		CExtPopupInplaceListBox & wndListBox,
		CExtGridCell::TrackCellStateInfo_t & _tcsi,
		LPMEASUREITEMSTRUCT pMIS
		);
	virtual bool OnQueryEnabledIncrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnQueryEnabledDecrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual HRESULT OnParseText(
		__EXT_MFC_SAFE_LPCTSTR sText
		) const;
}; // class CExtGridCellUpDownColorPart

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellCurrency

class __PROF_UIS_API CExtGridCellCurrency : public CExtGridCellNumberBase
{
public:
	DECLARE_SERIAL( CExtGridCellCurrency );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellCurrency, CExtGridCellNumberBase );
	CExtGridCellCurrency(
		CExtGridDataProvider * pDataProvider = NULL
		);

	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	virtual void GetVariant( VARIANT & varCopy ) const;
	virtual void OnFormatCellText( 
		CExtSafeString & strCopy, 
		LPCVARIANT pvarValue 
		) const;
public:
	virtual UINT OnQueryNumDigits( 
		LPCVARIANT pvarValue = NULL
		) const;
	virtual UINT OnQueryLeadingZero() const;
	virtual bool OnQueryTrailingZeroes() const;
	virtual void OnQueryGrouping( CExtSafeString & sVal ) const;
	virtual void OnQueryDecimalSeparator( CExtSafeString & sVal ) const;
	virtual void OnQueryThousandSeparator( CExtSafeString & sVal ) const;
	virtual void OnQueryCurrencySymbol( CExtSafeString & sVal ) const;
	virtual UINT OnQueryNegativeOrder() const;
	virtual UINT OnQueryPositiveOrder() const;
	
	bool SetAllowTrailingZeroes( 
		bool bAllowTrailingZeroes = true 
		);
	bool GetAllowTrailingZeroes() const;	

	bool SetAutoNumDigits( 
		bool bAutoNumDigits = true 
		);
	bool GetAutoNumDigits() const;	
	
	INT SetNumDigits( 
		INT nNumDigits = -1
		);
	INT GetNumDigits() const;

	INT SetLeadingZero( 
		INT nLeadingZero = -1
		);
	INT GetLeadingZero() const;

	INT SetNegativeOrder( 
		INT nNegativeOrder = -1
		);
	INT GetNegativeOrder() const;

	INT SetPositiveOrder( 
		INT nPositiveOrder = -1
		);
	INT GetPositiveOrder() const;

	void SetGrouping( 
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR( NULL ) 
		);
	void GetGrouping( 
		CExtSafeString & strCopy 
		) const;

	void SetDecimalSeparator( 
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR( NULL ) 
		);
	void GetDecimalSeparator( 
		CExtSafeString & strCopy 
		) const;

	void SetThousandSeparator( 
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR( NULL ) 
		);
	void GetThousandSeparator( 
		CExtSafeString & strCopy 
		) const;

	void SetCurrencySymbol( 
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR( NULL ) 
		);
	void GetCurrencySymbol( 
		CExtSafeString & strCopy 
		) const;

protected:
	// Remove trailing zeroes
	bool m_bAllowTrailingZeroes:1;

	// Automatically calculates number of fractional digits
	bool m_bAutoNumDigits:1;

	// Specifies number of fractional digits for the monetary format. 
	// The default value is -1. In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_ICURRDIGITS. 
	INT m_nNumDigits;

	// Specifies whether to use leading zeroes in decimal fields. 
	// The default value is -1. In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_ILZERO. 
	//
	// The specifier can be one of the following values:
	// 0 - No leading zeros 
	// 1 - Leading zeros 
	INT m_nLeadingZero;

	// Specifies negative currency mode. 
	// The default value is -1. In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_INEGCURR. 
	//
	// The mode can be one of the following values:
	// 0 - Left parenthesis, monetary symbol, number, right parenthesis. Example: ($1.1)
	// 1 - Negative sign, monetary symbol, number. Example: -$1.1 
	// 2 - Monetary symbol, negative sign, number. Example: $-1.1 
	// 3 - Monetary symbol, number, negative sign. Example: $1.1- 
	// 4 - Left parenthesis, number, monetary symbol, right parenthesis. Example: (1.1$) 
	// 5 - Negative sign, number, monetary symbol. Example: -1.1$ 
	// 6 - Number, negative sign, monetary symbol. Example: 1.1-$ 
	// 7 - Number, monetary symbol, negative sign. Example: 1.1$- 
	// 8 - Negative sign, number, space, monetary symbol (like #5, but with a space before the monetary symbol). Example: -1.1 $ 
	// 9 - Negative sign, monetary symbol, space, number (like #1, but with a space after the monetary symbol). Example: -$ 1.1 
	// 10 - Number, space, monetary symbol, negative sign (like #7, but with a space before the monetary symbol). Example: 1.1 $- 
	// 11 - Monetary symbol, space, number, negative sign (like #3, but with a space after the monetary symbol). Example: $ 1.1- 
	// 12 - Monetary symbol, space, negative sign, number (like #2, but with a space after the monetary symbol). Example: $ -1.1 
	// 13 - Number, negative sign, space, monetary symbol (like #6, but with a space before the monetary symbol). Example: 1.1- $ 
	// 14 - Left parenthesis, monetary symbol, space, number, right parenthesis (like #0, but with a space after the monetary symbol). Example: ($ 1.1)
	// 15 - Left parenthesis, number, space, monetary symbol, right parenthesis (like #4, but with a space before the monetary symbol). Example: (1.1 $) 
	INT m_nNegativeOrder;

	// Specifies position of the monetary symbol in the positive currency mode. 
	// The default value is -1. In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_ICURRENCY. 
	//
	// The mode can be one of the following values:
	// 0 - Prefix, no separation, for example $1.1 
	// 1 - Suffix, no separation, for example 1.1$ 
	// 2 - Prefix, 1-character separation, for example $ 1.1 
	// 3 - Suffix, 1-character separation, for example 1.1 $ 
	INT m_nPositiveOrder;

	// Specifies size of each group of monetary digits to the left of the decimal. 
	// The default value is "" (empty). In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_SMONGROUPING. 
	//
	// An explicit size is needed for each group, and sizes are separated by semicolons. 
	// If the last value is zero, the preceding value is repeated. 
	// For example, to group thousands, specify 3;0. 
	// Indic locales group the first thousand and then group by hundreds 
	// for example 12,34,56,789, which is represented by 3;2;0. 
	CExtSafeString m_sGrouping;

	// Specifies character(s) used as the monetary decimal separator. 
	// The default value is "" (empty). In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_SMONDECIMALSEP. 
	// The maximum number of characters allowed for this string is 4 (including a null terminator). 
	CExtSafeString m_sDecimalSeparator;

	// Specifies character(s) used to separate groups of digits to the left of the monetary decimal. 
	// The default value is "" (empty). In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_SMONTHOUSANDSEP. 
	// The maximum number of characters allowed for this string is 4 (including a null terminator). 
	CExtSafeString m_sThousandSeparator;

	// Specifies string used as the local monetary symbol. 
	// The default value is "" (empty). In this case we are using information specified 
	// by the LCTYPE constant value LOCALE_SCURRENCY. 
	// The maximum number of characters allowed for this string is 6 (including a null terminator). 
	CExtSafeString m_sCurrencySymbol;

}; // class CExtGridCellCurrency

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellPercentage

class __PROF_UIS_API CExtGridCellPercentage : public CExtGridCellNumber
{
public:
	DECLARE_SERIAL( CExtGridCellPercentage );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellPercentage, CExtGridCellNumber );
	CExtGridCellPercentage(
		CExtGridDataProvider * pDataProvider = NULL
		);

	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	virtual void OnFormatCellText( 
		CExtSafeString & strCopy, 
		LPCVARIANT pvarValue 
		) const;

public:
	INT SetPercentSignPos( 
		INT nPercentOrder = -1
		);
	INT GetPercentSignPos() const;

protected:
	virtual UINT OnQueryNumDigits( 
		LPCVARIANT pvarValue = NULL
		) const;
	virtual bool OnTextGetPrepareVariant( 
		LPVARIANT pvarValue 
		) const;

	virtual UINT OnQueryPercentSignPos() const;

	// Specifies position for a percent sign. 
	// The default value is -1 which is corresponds the #2 mode.
	// The mode can be one of the following values:
	// 0 - Percent sign, number. Example: %1.1 
	// 1 - Percent sign, space, number. Example: % 1.1 
	// 2 - Number, percent sign. Example: 1.1% 
	// 3 - Number, space, percent sign. Example: 1.1 % 
	INT m_nPercentSignPos;

}; // class CExtGridCellPercentage

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellFraction
 
class __PROF_UIS_API CExtGridCellFraction : public CExtGridCellNumberBase
{
public:
	DECLARE_SERIAL( CExtGridCellFraction );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellFraction, CExtGridCellNumberBase );
	CExtGridCellFraction(
		CExtGridDataProvider * pDataProvider = NULL
		);

	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	virtual void OnFormatCellText( 
		CExtSafeString & strCopy, 
		LPCVARIANT pvarValue 
		) const;

public:
	enum e_FractionType_t
	{
		eNone				= 0,
		eUpToOneDigit		= 1,
		eUpToTwoDigits		= 2,
		eUpToThreeDigits	= 3,
		eUpToFourDigits		= 4,
		eAsHalves			= 5,
		eAsQuarters			= 6,
		eAsEighths			= 7,
		eAsSixteenths		= 8,
		eAsTenths			= 9,
		eAsHundredths		= 10,
	};

	e_FractionType_t FractionTypeSet(
		e_FractionType_t eFractionType
		);
	e_FractionType_t FractionTypeGet() const;

	virtual __EXT_MFC_SAFE_LPCTSTR OnFormatFraction(
		DOUBLE dVal, 
		__EXT_MFC_SAFE_LPTSTR lpszBuffer, 
		INT nBufSize
		) const;

protected:
	e_FractionType_t m_eFractionType;

}; // class CExtGridCellFraction

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellScientific

class __PROF_UIS_API CExtGridCellScientific : public CExtGridCellNumberBase
{
public:
	DECLARE_SERIAL( CExtGridCellScientific );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellScientific, CExtGridCellNumberBase );
	CExtGridCellScientific(
		CExtGridDataProvider * pDataProvider = NULL
		);

	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	virtual void OnFormatCellText( 
		CExtSafeString & strCopy, 
		LPCVARIANT pvarValue 
		) const;
	
	virtual void OnQueryTextForInplaceControl( 
		CExtSafeString & strCopy 
		) const;

public:
	INT SetDecimalPlaces(
		INT nDecimalPlaces
		);
	INT GetDecimalPlaces() const;

protected:
	INT m_nDecimalPlaces;

}; // class CExtGridCellScientific

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellSlider

class __PROF_UIS_API CExtGridCellSlider : public CExtGridCellVariant
{
public:
	DECLARE_SERIAL( CExtGridCellSlider );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellSlider, CExtGridCellVariant );
	CExtGridCellSlider(
		CExtGridDataProvider * pDataProvider = NULL
		);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual void OnInplaceControlSliderInputComplete(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		INT nPosNew,
		bool bSaveChanges
		);

public:
	INT GetLineSize() const;
	INT SetLineSize( INT nSize );

	INT GetPageSize() const;
	INT SetPageSize( INT nSize );
	
	INT GetRangeMax() const;
	INT GetRangeMin() const;
	
	void GetRange( INT & nMin, INT & nMax ) const;
	void SetRange( INT nMin, INT nMax );
	INT SetRangeMin( INT nMin );
	INT SetRangeMax( INT nMax );
	
	INT GetPos() const;
	INT SetPos( INT nPos );

protected:
	INT m_nLineSize;
	INT m_nPageSize;
	INT m_nRangeMin;
	INT m_nRangeMax;

}; // class CExtGridCellSlider

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellPictureBase

class __PROF_UIS_API CExtGridCellPictureBase : public CExtGridCellEx
{
public:
	DECLARE_SERIAL( CExtGridCellPictureBase );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellPictureBase, CExtGridCellEx );
	CExtGridCellPictureBase(
		CExtGridDataProvider * pDataProvider = NULL
		);

	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );
	virtual bool IsEmpty() const;
	
	// virtual methods
	virtual void GetVariant( VARIANT & varCopy ) const;
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;
	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		); 
	virtual CSize MeasureCell(
		CExtGridWnd * pWndGrid, // can be NULL
		CDC & dcMeasure,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual CSize OnMeasureTextSize(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcCellText,
		DWORD dwAreaFlags
		) const;
	virtual void OnPaintText(
		const RECT & rcCellText,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;

public:
	enum e_ImageMode_t
	{
		eAlign = 0,		// The image is aligned according to the text aligning styles (the __EGCS_TA_HORZ_xxx and __EGCS_TA_VERT_xxx styles)
		eTile = 1,		// The image is repeated until the entire available area is filled. 
		eStretch = 2,	// The image is stretched to fit all the available area. 
	};

	e_ImageMode_t ImageModeSet(
		e_ImageMode_t eImageMode
		);
	e_ImageMode_t ImageModeGet() const;

	const CExtBitmap * BitmapGet() const;
	CExtBitmap * BitmapGet();
	bool BitmapSet( 
		CExtBitmap * pBmp 
		);
	
	virtual bool OnQueryEnabledBitmapBufferAssignment() const;
	virtual CExtBitmap * BitmapGetBuffer();
	const CExtBitmap * BitmapGetBuffer() const;

protected:
	e_ImageMode_t m_eImageMode;
}; // class CExtGridCellPictureBase

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellPicture

class __PROF_UIS_API CExtGridCellPicture : public CExtGridCellPictureBase
{
public:
	DECLARE_SERIAL( CExtGridCellPicture );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellPicture, CExtGridCellPictureBase );
	CExtGridCellPicture(
		CExtGridDataProvider * pDataProvider = NULL
		);
	virtual ~CExtGridCellPicture();

	virtual void Empty();
	virtual bool OnQueryEnabledBitmapBufferAssignment() const;
	virtual CExtBitmap * BitmapGetBuffer();

	virtual CSize MeasureCell(
		CExtGridWnd * pWndGrid, // can be NULL
		CDC & dcMeasure,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual CSize OnMeasureTextSize(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcCellText,
		DWORD dwAreaFlags
		) const;
	virtual void OnPaintText(
		const RECT & rcCellText,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	
protected:
	class __PROF_UIS_API CExtBitmapInternal : public CExtBitmap
	{
	public:
		CExtBitmapInternal()
			: m_pCell( NULL )
		{
		}
		CExtBitmapInternal(
			const CExtBitmap & other
			)
			: CExtBitmap( other )
			, m_pCell( NULL )
		{
		}
		virtual void OnContentChanged()
		{
			CExtBitmap::OnContentChanged();
			if( m_pCell != NULL )
			{
				ASSERT_VALID( m_pCell );
				m_pCell->ModifyStyleEx( 0, __EGCS_EX_EMPTY );
			}
		}
		CExtGridCell * m_pCell;
	}; // class CExtBitmapInternal

	CExtBitmapInternal m_bmp;

}; // class CExtGridCellPicture

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellPictureRef

class __PROF_UIS_API CExtGridCellPictureRef : public CExtGridCellPictureBase
{
public:
	DECLARE_SERIAL( CExtGridCellPictureRef );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellPictureRef, CExtGridCellPictureBase );
	CExtGridCellPictureRef(
		CExtGridDataProvider * pDataProvider = NULL,
		CExtBitmap * pExternalBitmapBuffer = NULL
		);
	virtual ~CExtGridCellPictureRef();
	virtual bool OnQueryEnabledBitmapBufferAssignment() const;
	virtual CExtBitmap * BitmapGetBuffer();
	virtual void BitmapSetBuffer(
		CExtBitmap * pExternalBitmapBuffer = NULL
		);
	virtual void Assign( const CExtGridCell & other )
	{
		ASSERT_VALID( this );
		CExtGridCellPictureBase::Assign( other );
		CExtGridCellPictureRef * pCell =
				DYNAMIC_DOWNCAST(
					CExtGridCellPictureRef,
					( const_cast < CExtGridCell * > ( &other ) )
					);
		if( pCell != NULL )
			BitmapSetBuffer( pCell->BitmapGetBuffer() ); // assumes external buffer can be assigned safely
	}
protected:
	CExtBitmap * m_pExternalBitmapBuffer;
}; // class CExtGridCellPictureRef

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellInplaceSlider

class __PROF_UIS_API CExtGridCellInplaceSlider : public CExtGridCellVariant
{
public:
	DECLARE_SERIAL( CExtGridCellInplaceSlider );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellInplaceSlider, CExtGridCellVariant );
	CExtGridCellInplaceSlider(
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtGridCellInplaceSlider( const CExtGridCellInplaceSlider & other );
	CExtGridCellInplaceSlider( const CExtGridCell & other );

	virtual ~CExtGridCellInplaceSlider();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

	virtual void Serialize( CArchive & ar );
	virtual void Empty();
	virtual void Assign( const CExtGridCell & other );
	virtual void OnFormatCellText( 
		CExtSafeString & strCopy, 
		LPCVARIANT pvarValue 
		) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);

	virtual bool OnQueryEnabledIncrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnQueryEnabledDecrement(
		const CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		) const;
	virtual bool OnValueIncrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnValueDecrement(
		CExtGridWnd & wndGrid,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);

public:
	enum e_SliderArea_t
	{
		__ESA_ICON_AND_TEXT = 0, // both icon and text
		__ESA_SLIDER = 1, // all slider
		__ESA_SCROLLABLE = 2, // scrollable part of slider
		__ESA_PAGE_UP = 3,
		__ESA_PAGE_DOWN = 4,
		__ESA_LINE_UP = 5,
		__ESA_LINE_DOWN = 6,
		__ESA_THUMB = 7,
		__ESA_COUNT = 8,
		__ESA_UNKNOWN_OR_NOWHERE = -1,
		__ESA_PAINT_RANGE_FIRST = __ESA_SLIDER,
		__ESA_PAINT_RANGE_LAST = __ESA_THUMB,
		__ESA_CLICKABLE_RANGE_FIRST = __ESA_PAGE_UP,
		__ESA_CLICKABLE_RANGE_LAST = __ESA_THUMB,
	};

protected:
	ULONG m_nScrollTotalRange;
	ULONG m_nScrollPageSize;
	
	INT m_nScrollButtonExtentH;
	INT m_nScrollButtonExtentV;
	INT m_nScrollControlExtentH;
	INT m_nScrollControlExtentV;

	CExtSafeString m_sLabel;
	static e_SliderArea_t g_eAreaTracked;
	static e_SliderArea_t g_eAreaPressed;

	static CExtGridCellInplaceSlider * g_pTrackedGridCellInplaceSlider;

public:
	ULONG ScrollLimitGet() const;

	virtual ULONG ScrollTotalRangeGet() const;
	virtual ULONG ScrollTotalRangeSet( ULONG nScrollTotalRange );

	virtual ULONG ScrollPosGet() const;
	virtual ULONG ScrollPosSet( ULONG nScrollPos );
	virtual ULONG OnScrollPosSet(
		ULONG nScrollPos,
		CExtGridWnd * pWndGrid = NULL,
		LONG nColNo = 0L,
		LONG nRowNo = 0L,
		INT nColType = 0,
		INT nRowType = 0
		);

	virtual ULONG ScrollPageSizeGet() const;
	virtual ULONG ScrollPageSizeSet( ULONG nScrollPageSize );

	virtual INT ScrollButtonExtentHorzGet() const;
	virtual INT ScrollButtonExtentHorzSet( INT nScrollButtonExtent );

	virtual INT ScrollButtonExtentVertGet() const;
	virtual INT ScrollButtonExtentVertSet( INT nScrollButtonExtent );

	virtual INT ScrollControlExtentHorzGet() const;
	virtual INT ScrollControlExtentHorzSet( INT nScrollControlExtent );

	virtual INT ScrollControlExtentVertGet() const;
	virtual INT ScrollControlExtentVertSet( INT nScrollControlExtent );

public:
	virtual void OnCalcSliderLayout(
		CRect rcAll,
		CRect * arrAreaRects // count is exactly equal to __ESA_COUNT
		) const;
	virtual void OnPaintScrollArea(
		CDC & dc,
		e_SliderArea_t eSA,
		CRect rcArea,
		bool bEnabled,
		bool bHovered,
		bool bPressed,
		const CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnPaintRestArea(
		const RECT & rcCellRest,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual CRect OnQueryRestAreaMargins(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual bool OnClick(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nRepCnt, // 0 - button up, 1 - single click, 2 - double click, 3 - post single click & begin editing
		UINT nFlags // mouse event flags
		);
	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual void OnQueryTextForInplaceControl( 
		CExtSafeString & strCopy 
		) const;
	virtual void OnInplaceControlTextInputComplete(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		__EXT_MFC_SAFE_LPCTSTR sTextNew,
		bool bSaveChanges
		);
	virtual bool OnSysKey(
		CExtGridWnd & wndGrid,
		bool bKeyDownEvent, // true - sys-key-down event, false - sys-key-up event
		UINT nChar, // key code
		UINT nRepCnt, // sys-key-down/sys-key-up press count
		UINT nFlags // sys-key-down/sys-key-up event flags
		);
}; // class CExtGridCellInplaceSlider

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellHotKey

class __PROF_UIS_API CExtGridCellHotKey : public CExtGridCellEx
{
public:
	DECLARE_SERIAL( CExtGridCellHotKey );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellHotKey, CExtGridCellEx);
	CExtGridCellHotKey(
		CExtGridDataProvider * pDataProvider = NULL
		);

 	void SetHotKey( 
		const ACCEL & _dataAccel 
		);
 	void SetHotKey( 
		DWORD dwHotKey 
		);
 	void SetHotKey( 
		WORD wKey,
		BYTE fVirt
		);
  	void GetHotKey( 
		ACCEL & _dataAccel
		) const;
 	DWORD GetHotKey() const;
  	void GetHotKey( 
		WORD & wKey,
		BYTE & fVirt
		) const;
	const CExtGridCellHotKey & operator = ( DWORD dwHotKey );

	virtual void Empty();
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	// virtual methods
	virtual int Compare(
		const CExtGridCell & other,
		DWORD dwStyleMask = __EGCS_COMPARE_MASK,
		DWORD dwStyleExMask = __EGCS_EX_COMPARE_MASK
		) const;
	virtual void GetVariant( VARIANT & varCopy ) const;
	virtual void TextGet( CExtSafeString & strCopy ) const;
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	
	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual void OnInplaceControlHotKeyInputComplete(
		HWND hWndInplaceControl,
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwHotKeyNew,
		bool bSaveChanges
		);

protected:
	DWORD m_dwHotKey;

}; // class CExtGridCellHotKey

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellButton

class __PROF_UIS_API CExtGridCellButton : public CExtGridCellString
{
public:
	DECLARE_SERIAL( CExtGridCellButton );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellButton, CExtGridCellString);
	CExtGridCellButton(
		CExtGridDataProvider * pDataProvider = NULL
		);

	// virtual methods
	virtual HWND OnInplaceControlCreate(
		CExtGridWnd & wndGrid,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		); 
	virtual void OnCalcLayout(
		DWORD dwCellStyle,
		DWORD dwCellStyleEx,
		CRect & rcCellRest,
		CRect & rcCellText,
		CRect & rcFocusArrow,
		CRect & rcIcon,
		CRect & rcCheck,
		CRect & rcSortArrow,
		CRect & rcUpdownButtonTop,
		CRect & rcUpdownButtonBottom,
		CRect & rcDropdownButton,
		CRect & rcEllipsisButton,
		CSize sizeFocusArrow,
		CSize sizeIcon,
		CSize sizeCheck,
		CSize sizeSortArrow,
		INT nExtentUpdownButton,
		INT nExtentDropdownButton,
		INT nExtentEllipsisButton,
		INT & nFocusGlyphType,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual COLORREF OnQueryTextColor(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnPaintButton(
		const RECT & rcButton,
		INT nButtonType, // e_button_type_t
		bool bPressed,
		bool bHovered,
		bool bFocused,
		bool bEnabled,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual CRect OnQueryTextAreaMargins(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnPaintForeground(
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	
}; // class CExtGridCellButton

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellObjectSize

class __PROF_UIS_API CExtGridCellObjectSize : public CExtGridCellNumber
{
public:
	DECLARE_SERIAL( CExtGridCellObjectSize );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellObjectSize, CExtGridCellNumber );
	CExtGridCellObjectSize(
		CExtGridDataProvider * pDataProvider = NULL
		);

	enum e_Mode_t
	{
		eAuto		= 0,
		eBytes		= 1,
		eKilobytes	= 2, // 1 kilobyte  (KB) = 2^10 bytes 
		eMegabytes	= 3, // 1 megabyte  (MB) = 2^20 bytes 
		eGigabytes	= 4, // 1 gigabyte  (GB) = 2^30 bytes 
		eTerabytes	= 5, // 1 terabyte  (TB) = 2^40 bytes 
		ePetabytes	= 6, // 1 petabyte  (PB) = 2^50 bytes
		eExabytes	= 7, // 1 exabyte   (EB) = 2^60 bytes
	};

	e_Mode_t GetMode() const;
	e_Mode_t SetMode( e_Mode_t eMode );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );

	virtual void OnFormatCellText( 
		CExtSafeString & strCopy, 
		LPCVARIANT pvarValue 
		) const;
	
	virtual UINT OnQueryNumDigits( 
		LPCVARIANT pvarValue = NULL
		) const;
 	virtual void OnQuerySizeAbbreviation(
		e_Mode_t eMode,
		CExtSafeString & strText
		) const;

protected:
	e_Mode_t m_eMode;

}; // class CExtGridCellObjectSize

/////////////////////////////////////////////////////////////////////////////
// CExtGridDataSortOrder

class __PROF_UIS_API CExtGridDataSortOrder : public CObject
{
	void _AssignFromOther(
		const CExtGridDataSortOrder & other
		);
public:
	DECLARE_SERIAL( CExtGridDataSortOrder );
	struct __PROF_UIS_API ITEM_INFO
	{
		LONG m_nRowColNo;
		bool m_bAscending:1;
		ITEM_INFO(
			LONG nRowColNo = -1L,
			bool bAscending = true
			)
			: m_nRowColNo( nRowColNo )
			, m_bAscending( bAscending )
		{
		}
		ITEM_INFO( const ITEM_INFO & other )
			: m_nRowColNo( other.m_nRowColNo )
			, m_bAscending( other.m_bAscending )
		{
		}
		ITEM_INFO & operator = ( const ITEM_INFO & other )
		{
			m_nRowColNo = other.m_nRowColNo;
			m_bAscending = other.m_bAscending;
			return (*this);
		}
		void Serialize( CArchive & ar )
		{
			BYTE nFlags = 0;
			if( ar.IsStoring() )
			{
				if( m_bAscending )
					nFlags |= 0x01;
				ar.Write( &m_nRowColNo, sizeof(m_nRowColNo) );
				ar.Write( &nFlags, sizeof(nFlags) );
			} // if( ar.IsStoring() )
			else
			{
				ar.Read( &m_nRowColNo, sizeof(m_nRowColNo) );
				ar.Read( &nFlags, sizeof(nFlags) );
				if( (nFlags&0x01) != 0 )
					m_bAscending = true;
				else
					m_bAscending = false;
			} // else from if( ar.IsStoring() )
		}
		int Cmp( const ITEM_INFO & other ) const
		{
			if( m_nRowColNo < other.m_nRowColNo )
				return -1;
			if( m_nRowColNo > other.m_nRowColNo )
				return 1;
			if( m_bAscending )
			{
				if( ! other.m_bAscending )
					return 1;
			} // if( m_bAscending )
			else
			{
				if( other.m_bAscending )
					return -1;
			} // else from if( m_bAscending )
			return 0;
		}
		bool operator == ( const ITEM_INFO & other ) const
		{
			return ( (Cmp(other) == 0) ? true : false );
		}
		bool operator != ( const ITEM_INFO & other ) const
		{
			return ( (Cmp(other) != 0) ? true : false );
		}
		bool operator <= ( const ITEM_INFO & other ) const
		{
			return ( (Cmp(other) <= 0) ? true : false );
		}
		bool operator >= ( const ITEM_INFO & other ) const
		{
			return ( (Cmp(other) >= 0) ? true : false );
		}
		bool operator < ( const ITEM_INFO & other ) const
		{
			return ( (Cmp(other) < 0) ? true : false );
		}
		bool operator > ( const ITEM_INFO & other ) const
		{
			return ( (Cmp(other) > 0) ? true : false );
		}
	}; // struct ITEM_INFO
	CArray < ITEM_INFO, ITEM_INFO & > m_arrItems;
	CExtGridDataSortOrder();
	CExtGridDataSortOrder(
		const CExtGridDataSortOrder & other
		);
	virtual ~CExtGridDataSortOrder();
	CExtGridDataSortOrder & operator =(
		const CExtGridDataSortOrder & other
		);
	bool operator == (
		const CExtGridDataSortOrder & other
		) const;
	bool operator != (
		const CExtGridDataSortOrder & other
		) const;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG
	virtual void Serialize( CArchive & ar );
	bool ItemsUnique() const;
	void MakeItemsUnique();
	bool IsEmpty() const;
	void Empty();
	LONG GetRowColPos( LONG nRowColNo ) const;
	void SetupOrder(
		const CExtGridDataSortOrder & _gdsoUpdate,
		bool bInvertIntersectionSortOrder
		);
	void UpdateIndices(
		LONG nRowColNo,
		LONG nInsertRemoveCount,
		bool bInsert
		);
	void SwapDroppedSeries(
		LONG nRowColNoSrc,
		LONG nRowColNoDropBefore
		);
}; // class CExtGridDataSortOrder

/////////////////////////////////////////////////////////////////////////////
// CExtGridDataProvider
// (abstract data provider for any grid window)

struct __PROF_UIS_API CExtMDP_MappingAPI
{
	virtual bool MappingIsEnabledX() const
	{
		return false;
	}
	virtual bool MappingIsEnabledY() const
	{
		return false;
	}
	virtual bool MappingEnableX( bool bEnable )
	{
		bEnable;
		return false;
	}
	virtual bool MappingEnableY( bool bEnable )
	{
		bEnable;
		return false;
	}
	virtual ULONG MapV2RX( ULONG nX ) const
	{
		nX;
		return false;
	}
	virtual ULONG MapV2RY( ULONG nY ) const
	{
		nY;
		return false;
	}
	virtual ULONG MapR2VX( ULONG nX ) const
	{
		nX;
		return false;
	}
	virtual ULONG MapR2VY( ULONG nY ) const
	{
		nY;
		return false;
	}
	virtual bool ColumnHide(
		ULONG nColNo,
		ULONG nHideCount = 1
		)
	{
		nColNo;
		nHideCount;
		return false;
	}
	virtual bool RowHide(
		ULONG nRowNo,
		ULONG nHideCount = 1
		)
	{
		nRowNo;
		nHideCount;
		return false;
	}
	bool MapSortOrderR2V(
		CExtGridDataSortOrder & _gdso,
		bool bColumns // true = sort order for columns, false - for rows
		) const
	{
		INT nIdx, nCount = INT( _gdso.m_arrItems.GetSize() );
		for( nIdx = 0; nIdx < nCount; nIdx ++ )
		{
			CExtGridDataSortOrder::ITEM_INFO & _sii = _gdso.m_arrItems.ElementAt( nIdx );
			_sii.m_nRowColNo = LONG( bColumns ? MapR2VY( ULONG(_sii.m_nRowColNo) ) : MapR2VX( ULONG(_sii.m_nRowColNo) ) );
			if( _sii.m_nRowColNo < 0 )
				return false;
		}
		return true;
	}
	bool MapSortOrderV2R(
		CExtGridDataSortOrder & _gdso,
		bool bColumns // true = sort order for columns, false - for rows
		) const
	{
		INT nIdx, nCount = INT( _gdso.m_arrItems.GetSize() );
		for( nIdx = 0; nIdx < nCount; nIdx ++ )
		{
			CExtGridDataSortOrder::ITEM_INFO & _sii = _gdso.m_arrItems.ElementAt( nIdx );
			_sii.m_nRowColNo = LONG( bColumns ? MapV2RY( ULONG(_sii.m_nRowColNo) ) : MapV2RX( ULONG(_sii.m_nRowColNo) ) );
			if( _sii.m_nRowColNo < 0 )
				return false;
		}
		return true;
	}
}; // struct CExtMDP_MappingAPI

class __PROF_UIS_API CExtGridDataProvider : public CObject, public CExtMDP_MappingAPI
{
public:
	DECLARE_DYNAMIC( CExtGridDataProvider );

	struct __PROF_UIS_API IDataProviderEvents
	{
		virtual void OnDataProviderSortEnter(
			bool bColumns // true = sorting/swapping columns, false - rows
			) = 0;
		virtual void OnDataProviderSortLeave(
			bool bColumns // true = sorting/swapping columns, false - rows
			) = 0;
		virtual void OnDataProviderSwapSeries(
			bool bColumns, // true = sorting/swapping columns, false - rows
			LONG nRowColNo1,
			LONG nRowColNo2,
			LONG nSwapCounter
			) = 0;
		virtual void OnDataProviderSwapDroppedSeries(
			bool bColumns, // true = swapping columns, false - rows
			LONG nRowColNoSrc,
			LONG nRowColNoDropBefore
			) = 0;
	}; // struct IDataProviderEvents

	mutable CExtGridDataProvider * m_pOuterDataProvider;

	CExtGridDataProvider();
	virtual ~CExtGridDataProvider();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG
	virtual __EXT_MFC_SAFE_LPTSTR StringAlloc(
		INT nCharCountIncZT
		) = 0;
	__EXT_MFC_SAFE_LPTSTR StringAlloc(
		const CExtSafeString & strSrc
		);
	__EXT_MFC_SAFE_LPTSTR StringAlloc(
		__EXT_MFC_SAFE_LPCTSTR strSrc
		);
	virtual void StringFree(
		__EXT_MFC_SAFE_LPTSTR strToFree
		) = 0;
	virtual bool ColumnInsert(
		ULONG nColNo,
		ULONG nInsertCount = 1
		) = 0;
	virtual bool RowInsert(
		ULONG nRowNo,
		ULONG nInsertCount = 1
		) = 0;
	virtual ULONG ColumnCountGet() const = 0;
	virtual ULONG RowCountGet() const = 0;
	virtual bool ColumnRemove(
		ULONG nColNo,
		ULONG nRemoveCount = 1
		) = 0;
	virtual bool RowRemove(
		ULONG nRowNo,
		ULONG nRemoveCount = 1
		) = 0;
	virtual void MinimizeMemoryUsage() = 0;
	virtual bool RowDefaultValueBind(
		ULONG nRowNo = (ULONG(-1L)), // if (ULONG(-1L)) - default value for all rows
		CExtGridCell * pCell = NULL // if NULL - remove default value
		) = 0;
	virtual bool ColumnDefaultValueBind(
		ULONG nColNo = (ULONG(-1L)), // if (ULONG(-1L)) - default value for all columns
		CExtGridCell * pCell = NULL // if NULL - remove default value
		) = 0;
	bool RowDefaultValueUnbind(
		ULONG nRowNo = (ULONG(-1L)) // if (ULONG(-1L)) - default value for all rows
		);
	bool ColumnDefaultValueUnbind(
		ULONG nColNo = (ULONG(-1L)) // if (ULONG(-1L)) - default value for all columns
		);
	virtual void RowDefaultValueUnbindAll() = 0;
	virtual void ColumnDefaultValueUnbindAll() = 0;
	virtual CExtGridCell * RowDefaultValueGet(
		ULONG nRowNo
		) = 0;
	virtual CExtGridCell * ColumnDefaultValueGet(
		ULONG nColNo
		) = 0;
	const CExtGridCell * RowDefaultValueGet(
		ULONG nRowNo
		) const;
	const CExtGridCell * ColumnDefaultValueGet(
		ULONG nColNo
		) const;
	virtual CExtGridCell * CellGet(
		ULONG nColNo,
		ULONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) = 0;
	const CExtGridCell * CellGet(
		ULONG nColNo,
		ULONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	virtual bool CellRangeSet(
		ULONG nColNo,
		ULONG nRowNo,
		ULONG nColCount = 1L,
		ULONG nRowCount = 1L,
		const CExtGridCell * pCellNewValue = NULL, // if NULL - empty existing cell values
		bool bReplace = false, // false - assign to existing cell instances or column/row type created cells, true - create new cloned copies of pCellNewValue
		CRuntimeClass * pInitRTC = NULL, // runtime class for new cell instance (used if bReplace=false)
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true, // false - use row default value (only when bAutoFindValue=true)
		ULONG * p_nUpdatedCellCount = NULL // returns count of really updated cells (zero also may be treated as success)
		) = 0;
	bool CellRangeEmpty(
		ULONG nColNo,
		ULONG nRowNo,
		ULONG nColCount = 1L,
		ULONG nRowCount = 1L,
		ULONG * p_nUpdatedCellCount = NULL // returns count of really erased cells (zero also may be treated as success)
		);
	bool CellRangeEmptyAll(
		ULONG * p_nUpdatedCellCount = NULL // returns count of really erased cells (zero also may be treated as success)
		);
	virtual bool CacheReserveForOuterCells(
		ULONG nColCount,
		ULONG nRowCount
		) = 0;
	virtual void CacheReservedCountsGet(
		ULONG * p_nColCount,
		ULONG * p_nRowCount
		) const = 0;
	virtual bool CacheData(
		const CExtScrollItemCacheInfo & _sciNew,
		const CExtScrollItemCacheInfo & _sciOld
		) = 0;
	virtual bool CacheIsVisibleFirstRecord( bool bHorz ) = 0;
	virtual bool CacheIsVisibleLastRecord( bool bHorz ) = 0;
	virtual bool CacheIsVisibleFirstRecord( bool bHorz ) const;
	virtual bool CacheIsVisibleLastRecord( bool bHorz ) const;
	virtual ULONG CacheColumnCountGet() = 0;
	virtual ULONG CacheRowCountGet() = 0;
	ULONG CacheColumnCountGet() const;
	ULONG CacheRowCountGet() const;
	virtual INT IconGetCount() = 0;
	INT IconGetCount() const;
	virtual CExtCmdIcon * IconGetAt( INT nIdx ) = 0;
	const CExtCmdIcon * IconGetAt( INT nIdx ) const;
	virtual INT IconInsert( // returns index or -1
		CExtCmdIcon * pIcon,
		INT nIdx = -1, // append
		bool bCopyIcon = true
		) = 0;
	INT IconInsert( // returns index or -1
		const CExtCmdIcon * pIcon,
		INT nIdx = -1, // append
		bool bCopyIcon = true
		);
	virtual INT IconRemove(
		INT nIdx = 0,
		INT nCountToRemove = -1 // all
		) = 0;
	virtual INT FontGetCount() = 0;
	INT FontGetCount() const;
	virtual HFONT FontGetAt( INT nIdx ) = 0;
	HFONT FontGetAt( INT nIdx ) const;
	virtual INT FontInsert( // returns index or -1
		HFONT hFont,
		INT nIdx = -1, // append
		bool bCopyFont = true
		) = 0;
	virtual INT FontRemove(
		INT nIdx = 0,
		INT nCountToRemove = -1 // all
		) = 0;
	virtual bool SortOrderUpdate(
		bool bColumns, // true = sort order for columns, false - for rows
		IDataProviderEvents * pDPE = NULL
		) = 0;
	virtual bool SortOrderSet(
		const CExtGridDataSortOrder & _gdso,
		bool bColumns, // true = sort order for columns, false - for rows
		IDataProviderEvents * pDPE = NULL
		) = 0;
	virtual bool SortOrderGet(
		CExtGridDataSortOrder & _gdso,
		bool bColumns // true = sort order for columns, false - for rows
		) const = 0;
	virtual bool SwapDroppedSeries(
		bool bColumns, // true = swap columns, false - rows
		ULONG nRowColNoSrc,
		ULONG nRowColNoDropBefore,
		IDataProviderEvents * pDPE = NULL
		) = 0;

	virtual bool _SortSwapSeries(
		LONG nRowColNo1,
		LONG nRowColNo2,
		LONG & nSwapCounter,
		CExtGridDataProvider::IDataProviderEvents * pDPE,
		bool bColumns
		)
	{
		nRowColNo1;
		nRowColNo2;
		nSwapCounter;
		pDPE;
		bColumns;
		return false;
	}
}; // class CExtGridDataProvider

/////////////////////////////////////////////////////////////////////////////
// CExtGDPI
// template for adding icon store to any data provider

template < class _BDP >
class CExtGDPI : public _BDP
{
	CTypedPtrArray < CPtrArray, CExtCmdIcon * > m_arrIcons;
	INT _IconRemoveImpl(
		INT nIdx = 0,
		INT nCountToRemove = -1 // all
		)
	{
		if( nIdx < 0 )
		{
			ASSERT( FALSE );
			return 0;
		} // if( nIdx < 0 )
		if( nCountToRemove == 0 )
			return 0;
		INT nCount = (INT)m_arrIcons.GetSize();
		if( nCount == 0 )
			return 0;
		if( nIdx >= nCount )
			return 0;
		if( nCountToRemove < 0 )
			nCountToRemove = nCount;
		if( (nIdx+nCountToRemove) > nCount )
			nCountToRemove = nCount-nIdx;
		ASSERT( nCountToRemove >= 0 );
		if( nCountToRemove == 0 )
			return 0;
		INT nPosCurrent = nIdx;
		INT nPosLast = nIdx + nCountToRemove - 1;
		ASSERT( nPosLast >= nPosCurrent );
		for( ; nPosCurrent <= nPosLast; nPosCurrent++ )
		{
			CExtCmdIcon * pIcon = m_arrIcons[ nPosCurrent ];
			ASSERT( pIcon != NULL );
			if( pIcon != NULL )
				delete pIcon;
		}
		m_arrIcons.RemoveAt( nIdx, nCountToRemove );
		return nCountToRemove;
	}
public:
	CExtGDPI()
	{
	}
	virtual ~CExtGDPI()
	{
		_IconRemoveImpl();
		ASSERT( m_arrIcons.GetSize() == 0 );
	}
	virtual INT IconGetCount()
	{
		ASSERT_VALID( this );
		INT nCount = (INT)m_arrIcons.GetSize();
		return nCount;
	}
	virtual CExtCmdIcon * IconGetAt( INT nIdx )
	{
		ASSERT_VALID( this );
		INT nCount = (INT)m_arrIcons.GetSize();
		if( nIdx < 0 || nIdx >= nCount )
		{
			ASSERT( FALSE );
			return NULL;
		} // if( nIdx < 0 || nIdx >= nCount )
		CExtCmdIcon * pIcon = m_arrIcons[ nIdx ];
		ASSERT( pIcon != NULL );
		return pIcon;
	}
	virtual INT IconInsert( // returns index or -1
		CExtCmdIcon * pIcon,
		INT nIdx = -1, // append
		bool bCopyIcon = true
		)
	{
		ASSERT_VALID( this );
		if( pIcon == NULL )
		{
			ASSERT( FALSE );
			return -1;
		} // if( pIcon == NULL )
		INT nCount = (INT)m_arrIcons.GetSize();
		if( nIdx < 0 || nIdx > nCount )
			nIdx = nCount;
		if( bCopyIcon )
			pIcon = new CExtCmdIcon( *pIcon );
		m_arrIcons.InsertAt( nIdx, pIcon, 1 );
		return nIdx;
	}
	virtual INT IconRemove(
		INT nIdx = 0,
		INT nCountToRemove = -1 // all
		)
	{
		ASSERT_VALID( this );
		return _IconRemoveImpl( nIdx, nCountToRemove );
	}
}; // class CExtGDPI

/////////////////////////////////////////////////////////////////////////////
// CExtGDPF
// template for adding font store to any data provider

template < class _BDP >
class CExtGDPF : public _BDP
{
	class CExtGDPFFont
	{
	public:
		CExtGDPFFont( HFONT hFont, bool bAutoDestroy )
			: m_hFont( hFont )
			, m_bAutoDestroy( bAutoDestroy )
		{
			ASSERT( m_hFont != NULL );
		}
		virtual ~CExtGDPFFont()
		{
			if( m_bAutoDestroy )
			{
				::DeleteObject( m_hFont );
				m_hFont = NULL;
			}
		}
		HFONT GetFont() const
		{
			return m_hFont;
		}
	protected:
		HFONT m_hFont;
		bool m_bAutoDestroy:1;
	};

	CTypedPtrArray < CPtrArray, CExtGDPFFont * > m_arrFonts;
	INT _FontRemoveImpl(
		INT nIdx = 0,
		INT nCountToRemove = -1 // all
		)
	{
		if( nIdx < 0 )
		{
			ASSERT( FALSE );
			return 0;
		}
		if( nCountToRemove == 0 )
			return 0;
		INT nCount = (INT)m_arrFonts.GetSize();
		if( nCount == 0 )
			return 0;
		if( nIdx >= nCount )
			return 0;
		if( nCountToRemove < 0 )
			nCountToRemove = nCount;
		if( (nIdx+nCountToRemove) > nCount )
			nCountToRemove = nCount-nIdx;
		ASSERT( nCountToRemove >= 0 );
		if( nCountToRemove == 0 )
			return 0;
		INT nPosCurrent = nIdx;
		INT nPosLast = nIdx + nCountToRemove - 1;
		ASSERT( nPosLast >= nPosCurrent );
		for( ; nPosCurrent <= nPosLast; nPosCurrent++ )
		{
			CExtGDPFFont * pFont = m_arrFonts[ nPosCurrent ];
			ASSERT( pFont != NULL );
			if( pFont != NULL )
			{
				delete pFont;
				pFont = NULL;
			}
		}
		m_arrFonts.RemoveAt( nIdx, nCountToRemove );
		return nCountToRemove;
	}
public:
	CExtGDPF()
	{
	}
	virtual ~CExtGDPF()
	{
		_FontRemoveImpl();
		ASSERT( m_arrFonts.GetSize() == 0 );
	}
	virtual INT FontGetCount()
	{
		ASSERT_VALID( this );
		INT nCount = (INT)m_arrFonts.GetSize();
		return nCount;
	}
	virtual HFONT FontGetAt( INT nIdx )
	{
		ASSERT_VALID( this );
		INT nCount = (INT)m_arrFonts.GetSize();
		if( nIdx < 0 || nIdx >= nCount )
		{
			ASSERT( FALSE );
			return NULL;
		}
		CExtGDPFFont * pFont = m_arrFonts[ nIdx ];
		ASSERT( pFont != NULL );
		HFONT hFont = NULL;
		if( pFont != NULL )
			hFont = pFont->GetFont();
		return hFont;
	}
	virtual INT FontInsert( // returns index or -1
		HFONT hFont,
		INT nIdx = -1, // append
		bool bCopyFont = true
		)
	{
		ASSERT_VALID( this );
		if( hFont == NULL )
		{
			ASSERT( FALSE );
			return -1;
		}
		INT nCount = (INT)m_arrFonts.GetSize();
		if( nIdx < 0 || nIdx > nCount )
			nIdx = nCount;
		CExtGDPFFont * pFont = NULL;
		if( bCopyFont )
		{
			LOGFONT lf;
			::memset( &lf, 0, sizeof(LOGFONT) );
			::GetObject( 
				hFont, 
				sizeof(LOGFONT), 
				&lf 
				);
			HFONT hFontCopy = ::CreateFontIndirect( &lf );
			ASSERT( hFontCopy != NULL );
			pFont = new CExtGDPFFont( hFontCopy, true );
		}
		else
			pFont = new CExtGDPFFont( hFont, false );
		ASSERT( pFont != NULL );
		if( pFont != NULL )
			m_arrFonts.InsertAt( nIdx, pFont, 1 );
		return nIdx;
	}
	virtual INT FontRemove(
		INT nIdx = 0,
		INT nCountToRemove = -1 // all
		)
	{
		ASSERT_VALID( this );
		return _FontRemoveImpl( nIdx, nCountToRemove );
	}
}; // class CExtGDPF

/////////////////////////////////////////////////////////////////////////////
// CExtGridDataProviderBL
// (basic layer data provider)

#if _MFC_VER >= 0x700
	#ifndef __EXT_PROFUIS_STATIC_LINK
		template class __PROF_UIS_API CExtGDPI < CExtGridDataProvider >;
		template class __PROF_UIS_API CExtGDPF < CExtGDPI < CExtGridDataProvider > >;
	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
#endif // _MFC_VER >= 0x700

class __PROF_UIS_API CExtGridDataProviderBL
	: public CExtGDPF < CExtGDPI < CExtGridDataProvider > >
{
protected:
	ULONG m_nColCountReserved, m_nRowCountReserved;
	typedef
		CMap <
			ULONG,
			ULONG,
			CExtGridCell *,
			CExtGridCell *
			>
		DefValueMap_t;
	DefValueMap_t m_mapDVX, m_mapDVY;
	void _DVM_RemoveAll(
		CExtGridDataProviderBL::DefValueMap_t & _map
		);
	bool _DVM_Setup(
		CExtGridDataProviderBL::DefValueMap_t & _map,
		ULONG nIdx,
		CExtGridCell * pCell
		);
	CExtGridCell * _DVM_Lookup(
		CExtGridDataProviderBL::DefValueMap_t & _map,
		ULONG nIdx
		);
	virtual IMalloc * _DVM_GetCellAllocator() = 0;
public:
	DECLARE_DYNAMIC( CExtGridDataProviderBL );

	CExtGridDataProviderBL();
	virtual ~CExtGridDataProviderBL();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

	virtual bool RowDefaultValueBind(
		ULONG nRowNo = (ULONG(-1L)), // if (ULONG(-1L)) - default value for all rows
		CExtGridCell * pCell = NULL // if NULL - remove default value
		);
	virtual bool ColumnDefaultValueBind(
		ULONG nColNo = (ULONG(-1L)), // if (ULONG(-1L)) - default value for all columns
		CExtGridCell * pCell = NULL // if NULL - remove default value
		);
	virtual void RowDefaultValueUnbindAll();
	virtual void ColumnDefaultValueUnbindAll();
	virtual CExtGridCell * RowDefaultValueGet(
		ULONG nRowNo
		);
	virtual CExtGridCell * ColumnDefaultValueGet(
		ULONG nColNo
		);
	virtual bool CacheReserveForOuterCells(
		ULONG nColCount,
		ULONG nRowCount
		);
	virtual void CacheReservedCountsGet(
		ULONG * p_nColCount,
		ULONG * p_nRowCount
		) const;
	virtual bool CacheData(
		const CExtScrollItemCacheInfo & _sciNew,
		const CExtScrollItemCacheInfo & _sciOld
		);
	virtual bool CacheIsVisibleFirstRecord( bool bHorz );
	virtual bool CacheIsVisibleLastRecord( bool bHorz );
	virtual ULONG CacheColumnCountGet();
	virtual ULONG CacheRowCountGet();
}; // class CExtGridDataProviderBL

/////////////////////////////////////////////////////////////////////////////
// CExtGridDataProviderRT
// (data provider which retranslates method calls to other
// agregated data provider object with/without X/Y flipping)

class __PROF_UIS_API CExtGridDataProviderRT : public CExtGridDataProvider
{
public:
	DECLARE_DYNCREATE( CExtGridDataProviderRT );
	struct __PROF_UIS_API IDataProviderEventsFlip
		: public CExtGridDataProvider::IDataProviderEvents
	{
		IDataProviderEvents * m_pOther;
		IDataProviderEventsFlip(
			IDataProviderEvents * pOther
			)
			: m_pOther( pOther )
		{
			ASSERT( m_pOther != NULL );
		}
		virtual void OnDataProviderSortEnter(
			bool bColumns // true = sorting/swapping columns, false - rows
			)
		{
			ASSERT( m_pOther != NULL );
			m_pOther->OnDataProviderSortEnter( ! bColumns );
		}
		virtual void OnDataProviderSortLeave(
			bool bColumns // true = sorting/swapping columns, false - rows
			)
		{
			ASSERT( m_pOther != NULL );
			m_pOther->OnDataProviderSortLeave( ! bColumns );
		}
		virtual void OnDataProviderSwapSeries(
			bool bColumns, // true = sorting/swapping columns, false - rows
			LONG nRowColNo1,
			LONG nRowColNo2,
			LONG nSwapCounter
			)
		{
			ASSERT( m_pOther != NULL );
			m_pOther->OnDataProviderSwapSeries(
				! bColumns,
				nRowColNo1,
				nRowColNo2,
				nSwapCounter
				);
		}
		virtual void OnDataProviderSwapDroppedSeries(
			bool bColumns, // true = swapping columns, false - rows
			LONG nRowColNoSrc,
			LONG nRowColNoDropBefore
			)
		{
			m_pOther->OnDataProviderSwapDroppedSeries(
				! bColumns,
				nRowColNoSrc,
				nRowColNoDropBefore
				);
		}
	}; // struct IDataProviderEventsFlip
	CExtGridDataProvider * m_pOther;
	bool m_bAutoDeleteOther:1;
	bool m_bFlipHV:1;
	CExtGridDataProviderRT(
		CExtGridDataProvider * pOther = NULL,
		bool bAutoDeleteOther = false,
		bool bFlipHV = false
		);
	virtual ~CExtGridDataProviderRT();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG
	virtual __EXT_MFC_SAFE_LPTSTR StringAlloc(
		INT nCharCountIncZT
		);
	virtual void StringFree(
		__EXT_MFC_SAFE_LPTSTR strToFree
		);
	virtual bool ColumnInsert(
		ULONG nColNo,
		ULONG nInsertCount = 1
		);
	virtual bool RowInsert(
		ULONG nRowNo,
		ULONG nInsertCount = 1
		);
	virtual ULONG ColumnCountGet() const;
	virtual ULONG RowCountGet() const;
	virtual bool ColumnRemove(
		ULONG nColNo,
		ULONG nRemoveCount = 1
		);
	virtual bool RowRemove(
		ULONG nRowNo,
		ULONG nRemoveCount = 1
		);
	virtual bool ColumnHide(
		ULONG nColNo,
		ULONG nHideCount = 1
		);
	virtual bool RowHide(
		ULONG nRowNo,
		ULONG nHideCount = 1
		);
	virtual void MinimizeMemoryUsage();
	virtual bool RowDefaultValueBind(
		ULONG nRowNo = (ULONG(-1L)), // if (ULONG(-1L)) - default value for all rows
		CExtGridCell * pCell = NULL // if NULL - remove default value
		);
	virtual bool ColumnDefaultValueBind(
		ULONG nColNo = (ULONG(-1L)), // if (ULONG(-1L)) - default value for all columns
		CExtGridCell * pCell = NULL // if NULL - remove default value
		);
	virtual void RowDefaultValueUnbindAll();
	virtual void ColumnDefaultValueUnbindAll();
	virtual CExtGridCell * RowDefaultValueGet(
		ULONG nRowNo
		);
	virtual CExtGridCell * ColumnDefaultValueGet(
		ULONG nColNo
		);
	virtual CExtGridCell * CellGet(
		ULONG nColNo,
		ULONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	virtual bool CellRangeSet(
		ULONG nColNo,
		ULONG nRowNo,
		ULONG nColCount = 1L,
		ULONG nRowCount = 1L,
		const CExtGridCell * pCellNewValue = NULL, // if NULL - empty existing cell values
		bool bReplace = false, // false - assign to existing cell instances or column/row type created cells, true - create new cloned copies of pCellNewValue
		CRuntimeClass * pInitRTC = NULL, // runtime class for new cell instance (used if bReplace=false)
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true, // false - use row default value (only when bAutoFindValue=true)
		ULONG * p_nUpdatedCellCount = NULL // returns count of really updated cells (zero also may be treated as success)
		);
	virtual bool CacheReserveForOuterCells(
		ULONG nColCount,
		ULONG nRowCount
		);
	virtual void CacheReservedCountsGet(
		ULONG * p_nColCount,
		ULONG * p_nRowCount
		) const;
	virtual bool CacheData(
		const CExtScrollItemCacheInfo & _sciNew,
		const CExtScrollItemCacheInfo & _sciOld
		);
	virtual bool CacheIsVisibleFirstRecord( bool bHorz );
	virtual bool CacheIsVisibleLastRecord( bool bHorz );
	virtual ULONG CacheColumnCountGet();
	virtual ULONG CacheRowCountGet();
	virtual INT IconGetCount();
	virtual CExtCmdIcon * IconGetAt( INT nIdx );
	virtual INT IconInsert( // returns index or -1
		CExtCmdIcon * pIcon,
		INT nIdx = -1, // append
		bool bCopyIcon = true
		);
	virtual INT IconRemove(
		INT nIdx = 0,
		INT nCountToRemove = -1 // all
		);
	virtual INT FontGetCount();
	virtual HFONT FontGetAt( INT nIdx );
	virtual INT FontInsert( // returns index or -1
		HFONT hFont,
		INT nIdx = -1, // append
		bool bCopyFont = true
		);
	virtual INT FontRemove(
		INT nIdx = 0,
		INT nCountToRemove = -1 // all
		);
	virtual bool SortOrderUpdate(
		bool bColumns, // true = sort order for columns, false - for rows
		IDataProviderEvents * pDPE = NULL
		);
	virtual bool SortOrderSet(
		const CExtGridDataSortOrder & _gdso,
		bool bColumns, // true = sort order for columns, false - for rows
		IDataProviderEvents * pDPE = NULL
		);
	virtual bool SortOrderGet(
		CExtGridDataSortOrder & _gdso,
		bool bColumns // true = sort order for columns, false - for rows
		) const;
	virtual bool SwapDroppedSeries(
		bool bColumns, // true = swap columns, false - rows
		ULONG nRowColNoSrc,
		ULONG nRowColNoDropBefore,
		IDataProviderEvents * pDPE = NULL
		);
}; // class CExtGridDataProviderRT

/////////////////////////////////////////////////////////////////////////////
// CExtGridDataProviderRecordset
// (cached data provider for vertical static/virtual modes)

class __PROF_UIS_API CExtGridDataProviderRecordset : public CExtGridDataProviderRT
{
protected:
	CExtScrollItemCacheInfo m_sci;
	ULONG m_nRowNoHeader;
	bool m_bHeaderInitialized:1,
		m_bVisibleFirstRecord:1, m_bVisibleLastRecord:1;
	LONG m_nVirtualPageSize, m_nVirtualRsOffset;
	// virtual cusor shifting
	void _VCS_ToPageStart();
	void _VCS_FromCurrent( LONG nPosChangingV );
public:
	DECLARE_DYNCREATE( CExtGridDataProviderRecordset );
	CExtGridDataProviderRecordset();
	virtual ~CExtGridDataProviderRecordset();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif // _DEBUG
	void ClearRecorsetCache();
	virtual bool CacheReserveForOuterCells(
		ULONG nColCount,
		ULONG nRowCount
		);
	virtual bool CacheDataStatic(
		const CExtScrollItemCacheInfo & _sciNew,
		const CExtScrollItemCacheInfo & _sciOld
		);
	virtual bool CacheDataVirtual(
		const CExtScrollItemCacheInfo & _sciNew,
		const CExtScrollItemCacheInfo & _sciOld
		);
	virtual bool CacheData(
		const CExtScrollItemCacheInfo & _sciNew,
		const CExtScrollItemCacheInfo & _sciOld
		);
	virtual bool CacheIsVisibleFirstRecord( bool bHorz );
	virtual bool CacheIsVisibleLastRecord( bool bHorz );
	virtual ULONG ColumnCountGet() const;
	virtual ULONG RowCountGet() const;
	virtual ULONG CacheColumnCountGet();
	virtual ULONG CacheRowCountGet();
	virtual CExtGridCell * CellGet(
		ULONG nColNo,
		ULONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	virtual void RsSeekToFirst() = 0L;
	virtual void RsSeekToLast() = 0L;
	virtual void RsSeekToNext() = 0L;
	virtual void RsSeekToPrevious() = 0L;
	virtual void RsSeekFromStart( LONG nPos ) = 0L;
	virtual void RsSeekFromEnd( LONG nPos ) = 0L;
	virtual void RsSeekFromCurrent( LONG nPos ) = 0L;
	virtual bool RsIsBOF() = 0L;
	virtual bool RsIsEOF() = 0L;
	virtual bool RsIsVirtualMode() = 0L;
	virtual LONG RsFieldsCount() = 0;
	virtual LONG RsRecordsCount() = 0;
	virtual void RsCacheRow(
		ULONG nCacheColOffset,
		ULONG nCacheRowNo,
		bool bHeader
		) = 0;
	bool RsIsBOF() const;
	bool RsIsEOF() const;
	bool RsIsVirtualMode() const;
	LONG RsFieldsCount() const;
	LONG RsRecordsCount() const;
	virtual bool SortOrderUpdate(
		bool bColumns, // true = sort order for columns, false - for rows
		IDataProviderEvents * pDPE = NULL
		);
	virtual bool SortOrderSet(
		const CExtGridDataSortOrder & _gdso,
		bool bColumns, // true = sort order for columns, false - for rows
		IDataProviderEvents * pDPE = NULL
		);
	virtual bool SortOrderGet(
		CExtGridDataSortOrder & _gdso,
		bool bColumns // true = sort order for columns, false - for rows
		) const;
	virtual bool SwapDroppedSeries(
		bool bColumns, // true = swap columns, false - rows
		ULONG nRowColNoSrc,
		ULONG nRowColNoDropBefore,
		IDataProviderEvents * pDPE = NULL
		);
}; // class CExtGridDataProviderRecordset

/////////////////////////////////////////////////////////////////////////////
// CExtGridDataProviderMemory
// (dynamic memory data provider based on different kinds of
// IMalloc objects, used as default data provider)

class __PROF_UIS_API CExtGridDataProviderMemory : public CExtGridDataProviderBL
{
protected:
	IMalloc * m_pMallocV, * m_pMallocF, * m_pMallocD;
	ULONG m_nStringAllocationAlign;
	ULONG m_nDX, m_nDY;
	CExtGridCell *** m_pSections;
	CExtGridDataSortOrder m_gdsoColumns, m_gdsoRows;
public:
	bool m_bEnableSortOrderUpdatingColumns:1;
	bool m_bEnableSortOrderUpdatingRows:1;
	virtual bool _SortOrderCheck(
		const CExtGridDataSortOrder & _gdso,
		bool bColumns
		);
	virtual int _SortCmpCells(
		CExtGridCell * pCell_1,
		CExtGridCell * pCell_2
		);
	virtual int _SortCmpSeries(
		LONG nRowColNo1,
		LONG nRowColNo2,
		bool bColumns,
		const CExtGridDataSortOrder & _gdso
		);
	virtual bool _SortSwapSeries(
		LONG nRowColNo1,
		LONG nRowColNo2,
		LONG & nSwapCounter,
		CExtGridDataProvider::IDataProviderEvents * pDPE,
		bool bColumns
		);
	virtual bool _SortStep(
		bool bColumns,
		CExtGridDataProvider::IDataProviderEvents * pDPE,
		CExtGridDataSortOrder & _gdso,
		LONG & nSwapCounter,
		LONG nLow,
		LONG nHigh
		);
	CExtGridCell ** _SectionGetAt(
		ULONG nX,
		bool bAllowDelayedAllocation
		);
	void _SectionFree( CExtGridCell ** pSection );
	void _SectionFreeAll( bool bReleaseSectionStack );
	ULONG _SectionInsertAt(
		ULONG nX,
		ULONG nSectionCount,
		bool bForceDelayedRangeAllocation
		);
	bool _SectionEdject(
		ULONG nX,
		ULONG nDX,
		bool bDeallocateRest
		);
	bool _SectionRealloc(
		ULONG nX,
		ULONG nDY
		);
	bool _SectionReallocAll(
		ULONG nDY
		);
	static void _SectionRangeDestroy(
		IMalloc * pMalloc,
		CExtGridCell ** pSection,
		ULONG nY,
		ULONG nDY
		);
	bool _SectionRangeInjectAll(
		ULONG nY,
		ULONG nDY
		);
	static void _SectionRangeBottomToBottom(
		CExtGridCell ** pSection,
		ULONG nY,
		ULONG nDY,
		ULONG nSectionSize
		);
	void _SectionRangeBottomToBottomAll(
		ULONG nY,
		ULONG nDY
		);
	static void _SectionRangeEdject(
		IMalloc * pMalloc,
		CExtGridCell ** pSection,
		ULONG nY,
		ULONG nDY,
		ULONG nSectionSize
		);
	void _SectionRangeEdjectAll(
		ULONG nY,
		ULONG nDY,
		bool bDeallocateRest
		);

	virtual IMalloc * _DVM_GetCellAllocator();
	DECLARE_DYNCREATE( CExtGridDataProviderMemory );
	CExtGridDataProviderMemory(
		IMalloc * pMallocV = NULL,
		IMalloc * pMallocF = NULL,
		IMalloc * pMallocD = NULL,
		ULONG nStringAllocationAlign = 32
		);
	virtual ~CExtGridDataProviderMemory();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG
	virtual __EXT_MFC_SAFE_LPTSTR StringAlloc(
		INT nCharCountIncZT
		);
	virtual void StringFree(
		__EXT_MFC_SAFE_LPTSTR strToFree
		);
	virtual bool ColumnInsert(
		ULONG nColNo,
		ULONG nInsertCount = 1
		);
	virtual bool RowInsert(
		ULONG nRowNo,
		ULONG nInsertCount = 1
		);
	virtual ULONG ColumnCountGet() const;
	virtual ULONG RowCountGet() const;
	virtual bool ColumnRemove(
		ULONG nColNo,
		ULONG nRemoveCount = 1
		);
	virtual bool RowRemove(
		ULONG nRowNo,
		ULONG nRemoveCount = 1
		);
public:
	virtual void MinimizeMemoryUsage();
	virtual CExtGridCell * CellGet(
		ULONG nColNo,
		ULONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	virtual bool CellRangeSet(
		ULONG nColNo,
		ULONG nRowNo,
		ULONG nColCount = 1L,
		ULONG nRowCount = 1L,
		const CExtGridCell * pCellNewValue = NULL, // if NULL - empty existing cell values
		bool bReplace = false, // false - assign to existing cell instances or column/row type created cells, true - create new cloned copies of pCellNewValue
		CRuntimeClass * pInitRTC = NULL, // runtime class for new cell instance (used if bReplace=false)
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true, // false - use row default value (only when bAutoFindValue=true)
		ULONG * p_nUpdatedCellCount = NULL // returns count of really updated cells (zero also may be treated as success)
		);
	virtual bool SortOrderUpdate(
		bool bColumns, // true = sort order for columns, false - for rows
		IDataProviderEvents * pDPE = NULL
		);
	virtual bool SortOrderSet(
		const CExtGridDataSortOrder & _gdso,
		bool bColumns, // true = sort order for columns, false - for rows
		IDataProviderEvents * pDPE = NULL
		);
	virtual bool SortOrderGet(
		CExtGridDataSortOrder & _gdso,
		bool bColumns // true = sort order for columns, false - for rows
		) const;
	virtual bool SwapDroppedSeries(
		bool bColumns, // true = swap columns, false - rows
		ULONG nRowColNoSrc,
		ULONG nRowColNoDropBefore,
		IDataProviderEvents * pDPE = NULL
		);
}; // class CExtGridDataProviderMemory

/////////////////////////////////////////////////////////////////////////////
// CExtMDP template class
// Adds ability to hide rows/columns

template < class _BDP >
class CExtMDP : public _BDP
{
public:
	struct IDataProviderEventsMapped : public CExtGridDataProvider::IDataProviderEvents
	{
		CExtMDP_MappingAPI * m_pMappingAPI;
		CExtGridDataProvider::IDataProviderEvents * m_pOther;
		IDataProviderEventsMapped(
			CExtMDP_MappingAPI * pMappingAPI,
			CExtGridDataProvider::IDataProviderEvents * pOther
			)
			: m_pMappingAPI( pMappingAPI )
			, m_pOther( pOther )
		{
			ASSERT( m_pMappingAPI != NULL );
			ASSERT( m_pOther != NULL );
		}
		virtual void OnDataProviderSortEnter(
			bool bColumns // true = sorting/swapping columns, false - rows
			)
		{
			ASSERT( m_pMappingAPI != NULL );
			ASSERT( m_pOther != NULL );
			m_pOther->OnDataProviderSortEnter( bColumns );
		}
		virtual void OnDataProviderSortLeave(
			bool bColumns // true = sorting/swapping columns, false - rows
			)
		{
			ASSERT( m_pMappingAPI != NULL );
			ASSERT( m_pOther != NULL );
			m_pOther->OnDataProviderSortLeave( bColumns );
		}
		virtual void OnDataProviderSwapSeries(
			bool bColumns, // true = sorting/swapping columns, false - rows
			LONG nRowColNo1,
			LONG nRowColNo2,
			LONG nSwapCounter
			)
		{
			ASSERT( m_pMappingAPI != NULL );
			ASSERT( m_pOther != NULL );
			LONG nRowColNo1Mapped = LONG( bColumns ? m_pMappingAPI->MapR2VX( ULONG(nRowColNo1) ) : m_pMappingAPI->MapR2VY( ULONG(nRowColNo1) ) );
			LONG nRowColNo2Mapped = LONG( bColumns ? m_pMappingAPI->MapR2VX( ULONG(nRowColNo2) ) : m_pMappingAPI->MapR2VY( ULONG(nRowColNo2) ) );
			m_pOther->OnDataProviderSwapSeries(
				bColumns,
				nRowColNo1Mapped,
				nRowColNo2Mapped,
				nSwapCounter
				);
		}
		virtual void OnDataProviderSwapDroppedSeries(
			bool bColumns, // true = swapping columns, false - rows
			LONG nRowColNoSrc,
			LONG nRowColNoDropBefore
			)
		{
			ASSERT( m_pMappingAPI != NULL );
			ASSERT( m_pOther != NULL );
			LONG nRowColNoSrcMapped = LONG( bColumns ? m_pMappingAPI->MapR2VX( ULONG(nRowColNoSrc) ) : m_pMappingAPI->MapR2VY( ULONG(nRowColNoSrc) ) );
			LONG nRowColNoDropBeforeMapped = LONG( bColumns ? m_pMappingAPI->MapR2VX( ULONG(nRowColNoDropBefore) ) : m_pMappingAPI->MapR2VY( ULONG(nRowColNoDropBefore) ) );
			m_pOther->OnDataProviderSwapDroppedSeries(
				bColumns,
				nRowColNoSrcMapped,
				nRowColNoDropBeforeMapped
				);
		}
	}; // struct IDataProviderEventsMapped
	CArray < ULONG, ULONG >
		  m_arrMapV2RX
		, m_arrMapV2RY
		, m_arrMapR2VX
		, m_arrMapR2VY
		;
	bool m_bMappingEnabledX:1, m_bMappingEnabledY:1, m_bAutoDisableMappingX:1, m_bAutoDisableMappingY:1;
	CExtMDP()
		: m_bMappingEnabledX( false )
		, m_bMappingEnabledY( false )
		, m_bAutoDisableMappingX( true )
		, m_bAutoDisableMappingY( true )
	{
	}
	~CExtMDP()
	{
	}
	virtual bool MappingIsEnabledX() const
	{
		ASSERT_VALID( this );
		return m_bMappingEnabledX;
	}
	virtual bool MappingIsEnabledY() const
	{
		ASSERT_VALID( this );
		return m_bMappingEnabledY;
	}
	virtual bool MappingEnableX( bool bEnable )
	{
		ASSERT_VALID( this );
		if( ! bEnable )
		{
			if( ! m_bMappingEnabledX )
				return true;
			m_bMappingEnabledX = false;
			m_arrMapV2RX.RemoveAll();
			m_arrMapR2VX.RemoveAll();
			return true;
		}
		if( m_bMappingEnabledX )
			return true;
		ULONG nIndex, nCount = _BDP::ColumnCountGet();
		m_arrMapV2RX.SetSize( nCount );
		m_arrMapR2VX.SetSize( nCount );
		for( nIndex = 0; nIndex < nCount; nIndex++ )
		{
			m_arrMapV2RX.SetAt( nIndex, nIndex );
			m_arrMapR2VX.SetAt( nIndex, nIndex );
		}
		m_bMappingEnabledX = true;
		return true;
	}
	virtual bool MappingEnableY( bool bEnable )
	{
		ASSERT_VALID( this );
		if( ! bEnable )
		{
			if( ! m_bMappingEnabledY )
				return true;
			m_bMappingEnabledY = false;
			m_arrMapV2RY.RemoveAll();
			m_arrMapR2VY.RemoveAll();
			return true;
		}
		if( m_bMappingEnabledY )
			return true;
		ULONG nIndex, nCount = _BDP::RowCountGet();
		m_arrMapV2RY.SetSize( nCount );
		m_arrMapR2VY.SetSize( nCount );
		for( nIndex = 0; nIndex < nCount; nIndex++ )
		{
			m_arrMapV2RY.SetAt( nIndex, nIndex );
			m_arrMapR2VY.SetAt( nIndex, nIndex );
		}
		m_bMappingEnabledY = true;
		return true;
	}
	virtual ULONG MapV2RX( ULONG nX ) const
	{
		ASSERT_VALID( this );
		if( ! MappingIsEnabledX() )
			return nX;
		ULONG nCount = ULONG( m_arrMapV2RX.GetSize() );
		if( nX > nCount )
		{
			ASSERT( FALSE );
			return ULONG(-1L);
		}
		if( nX == nCount )
		{
			if( nX == 0 )
				return nX;
			ULONG nMappedX = m_arrMapV2RX[ nX - 1 ];
			ASSERT( nMappedX != ULONG(-1L) );
			nMappedX ++;
			return nMappedX;
		}
		ULONG nMappedX = m_arrMapV2RX[ nX ];
		ASSERT( nMappedX != ULONG(-1L) );
		return nMappedX;
	}
	virtual ULONG MapV2RY( ULONG nY ) const
	{
		ASSERT_VALID( this );
		if( ! MappingIsEnabledY() )
			return nY;
		ULONG nCount = ULONG( m_arrMapV2RY.GetSize() );
		if( nY > nCount )
		{
			ASSERT( FALSE );
			return ULONG(-1L);
		}
		if( nY == nCount )
		{
			if( nY == 0 )
				return nY;
			ULONG nMappedY = m_arrMapV2RY[ nY - 1 ];
			ASSERT( nMappedY != ULONG(-1L) );
			nMappedY ++;
			return nMappedY;
		}
		ULONG nMappedY = m_arrMapV2RY[ nY ];
		ASSERT( nMappedY != ULONG(-1L) );
		return nMappedY;
	}
	virtual ULONG MapR2VX( ULONG nX ) const
	{
		ASSERT_VALID( this );
		if( ! MappingIsEnabledX() )
			return nX;
		ULONG nCount = ULONG( m_arrMapR2VX.GetSize() );
		if( nX > nCount )
		{
			ASSERT( FALSE );
			return ULONG(-1L);
		}
		if( nX == nCount )
		{
			ULONG nMappedX = ULONG( m_arrMapV2RX.GetSize() );
			return nMappedX;
		}
		ULONG nMappedX = m_arrMapR2VX[ nX ];
		return nMappedX;
	}
	virtual ULONG MapR2VY( ULONG nY ) const
	{
		ASSERT_VALID( this );
		if( ! MappingIsEnabledY() )
			return nY;
		ULONG nCount = ULONG( m_arrMapR2VY.GetSize() );
		if( nY > nCount )
		{
			ASSERT( FALSE );
			return ULONG(-1L);
		}
		if( nY == nCount )
		{
			ULONG nMappedY = ULONG( m_arrMapV2RY.GetSize() );
			return nMappedY;
		}
		ULONG nMappedY = m_arrMapR2VY[ nY ];
		return nMappedY;
	}
	virtual ULONG ColumnCountGet() const
	{
		ASSERT_VALID( this );
		if( ! MappingIsEnabledX() )
		{
			ULONG nCount = _BDP::ColumnCountGet();
			return nCount;
		}
		ULONG nCount = ULONG( m_arrMapV2RX.GetSize() );
		return nCount;
	}
	virtual ULONG RowCountGet() const
	{
		ASSERT_VALID( this );
		if( ! MappingIsEnabledY() )
		{
			ULONG nCount = _BDP::RowCountGet();
			return nCount;
		}
		ULONG nCount = ULONG( m_arrMapV2RY.GetSize() );
		return nCount;
	}
	virtual bool ColumnRemove(
		ULONG nColNo,
		ULONG nRemoveCount = 1
		)
	{
		ASSERT_VALID( this );
		if( ! MappingIsEnabledX() )
			return _BDP::ColumnRemove( nColNo, nRemoveCount );
		bool bRetVal = true;
		ULONG nColNoRef = nColNo + nRemoveCount - 1L, nCounter = 0L;
		for( ; nCounter < nRemoveCount; nCounter ++, nColNoRef -- )
		{
			ULONG nColNoMapped = MapV2RX( nColNoRef );
			if( nColNoMapped == ULONG(-1L) )
				continue;
			if( ! _BDP::ColumnRemove( nColNoMapped, 1L ) )
			{
				bRetVal = false;
				bRetVal;
			}
			m_arrMapR2VX.RemoveAt( nColNoRef, 1 );
			m_arrMapV2RX.RemoveAt( nColNoMapped, 1 );
			ULONG nEnd = ULONG( m_arrMapV2RX.GetSize() );
			for( ; nColNoMapped < nEnd; nColNoMapped ++ )
			{
				ULONG nAdjustedNo = m_arrMapV2RX[ nColNoMapped ];
				nAdjustedNo --;
				m_arrMapV2RX.SetAt( nColNoMapped, nAdjustedNo );
			}
			nEnd = ULONG( m_arrMapR2VX.GetSize() );
			for( ; nColNoRef < nEnd; nColNoRef ++ )
			{
				ULONG nAdjustedNo = m_arrMapR2VX[ nColNoMapped ];
				nAdjustedNo --;
				m_arrMapR2VX.SetAt( nColNoRef, nAdjustedNo );
			}
		}
		if( m_bAutoDisableMappingX )
		{
			ULONG nCountCurrent = ColumnCountGet(), nCountReserved = 0L;
			CacheReservedCountsGet( &nCountReserved, NULL );
			if( nCountCurrent == nCountReserved )
				MappingEnableX( false );
		}
		return bRetVal;
	}
	virtual bool RowRemove(
		ULONG nRowNo,
		ULONG nRemoveCount = 1
		)
	{
		ASSERT_VALID( this );
		if( ! MappingIsEnabledY() )
			return _BDP::RowRemove( nRowNo, nRemoveCount );
		bool bRetVal = true;
		ULONG nRowNoRef = nRowNo + nRemoveCount - 1L, nCounter = 0L;
		for( ; nCounter < nRemoveCount; nCounter ++, nRowNoRef -- )
		{
			ULONG nRowNoMapped = MapV2RY( nRowNoRef );
			if( nRowNoMapped == ULONG(-1L) )
				continue;
			if( ! _BDP::RowRemove( nRowNoMapped, 1L ) )
			{
				bRetVal = false;
				bRetVal;
			}
			m_arrMapR2VY.RemoveAt( nRowNoRef, 1 );
			m_arrMapV2RY.RemoveAt( nRowNoMapped, 1 );
			ULONG nEnd = ULONG( m_arrMapV2RY.GetSize() );
			for( ; nRowNoMapped < nEnd; nRowNoMapped ++ )
			{
				ULONG nAdjustedNo = m_arrMapV2RY[ nRowNoMapped ];
				nAdjustedNo --;
				m_arrMapV2RY.SetAt( nRowNoMapped, nAdjustedNo );
			}
			nEnd = ULONG( m_arrMapR2VY.GetSize() );
			for( ; nRowNoRef < nEnd; nRowNoRef ++ )
			{
				ULONG nAdjustedNo = m_arrMapR2VY[ nRowNoMapped ];
				nAdjustedNo --;
				m_arrMapR2VY.SetAt( nRowNoRef, nAdjustedNo );
			}
		}
		if( m_bAutoDisableMappingY )
		{
			ULONG nCountCurrent = RowCountGet(), nCountReserved = 0L;
			CacheReservedCountsGet( NULL, &nCountReserved );
			if( nCountCurrent == nCountReserved )
				MappingEnableY( false );
		}
		return bRetVal;
	}
	virtual bool ColumnInsert(
		ULONG nColNo,
		ULONG nInsertCount = 1
		)
	{
		ASSERT_VALID( this );
		if( ! MappingIsEnabledX() )
			return _BDP::ColumnInsert( nColNo, nInsertCount );
		ULONG nColNoMapped = MapV2RX( nColNo );
		if( nColNoMapped == ULONG(-1L) )
			return false;
		if( ! _BDP::ColumnInsert( nColNoMapped, nInsertCount ) )
			return false;
		m_arrMapV2RX.InsertAt( nColNo, ULONG(-1L), nInsertCount );
		m_arrMapR2VX.InsertAt( nColNoMapped, ULONG(-1L), nInsertCount );
		ULONG nStep, nIdx, nEnd = nInsertCount;
		for( nStep = 0; nStep < nEnd; nStep++ )
		{
			m_arrMapV2RX.SetAt( nColNo + nStep, nColNoMapped + nStep );
			m_arrMapV2RX.SetAt( nColNoMapped + nStep, nColNo + nStep );
		}
		nEnd = ULONG( m_arrMapV2RX.GetSize() );
		for( nIdx = nColNoMapped + nInsertCount; nIdx < nEnd; nIdx ++ )
		{
			ULONG nAdjustedNo = m_arrMapV2RX[ nIdx ];
			nAdjustedNo += nInsertCount;
			m_arrMapV2RX.SetAt( nIdx, nAdjustedNo );
		}
		nEnd = ULONG( m_arrMapR2VX.GetSize() );
		for( nIdx = nColNo + nInsertCount; nIdx < nEnd; nIdx ++ )
		{
			ULONG nAdjustedNo = m_arrMapR2VX[ nIdx ];
			nAdjustedNo += nInsertCount;
			m_arrMapR2VX.SetAt( nIdx, nAdjustedNo );
		}
		return true;
	}
	virtual bool RowInsert(
		ULONG nRowNo,
		ULONG nInsertCount = 1
		)
	{
		ASSERT_VALID( this );
		if( ! MappingIsEnabledY() )
			return _BDP::RowInsert( nRowNo, nInsertCount );
		ULONG nRowNoMapped = MapV2RY( nRowNo );
		if( nRowNoMapped == ULONG(-1L) )
			return false;
		if( ! _BDP::RowInsert( nRowNoMapped, nInsertCount ) )
			return false;
		m_arrMapV2RY.InsertAt( nRowNo, ULONG(-1L), nInsertCount );
		m_arrMapR2VY.InsertAt( nRowNoMapped, ULONG(-1L), nInsertCount );
		ULONG nStep, nIdx, nEnd = nInsertCount;
		for( nStep = 0; nStep < nEnd; nStep++ )
		{
			m_arrMapV2RY.SetAt( nRowNo + nStep, nRowNoMapped + nStep );
			m_arrMapR2VY.SetAt( nRowNoMapped + nStep, nRowNo + nStep );
		}
		nEnd = ULONG( m_arrMapV2RY.GetSize() );
		for( nIdx = nRowNoMapped + nInsertCount; nIdx < nEnd; nIdx ++ )
		{
			ULONG nAdjustedNo = m_arrMapV2RY[ nIdx ];
			nAdjustedNo += nInsertCount;
			m_arrMapV2RY.SetAt( nIdx, nAdjustedNo );
		}
		nEnd = ULONG( m_arrMapR2VY.GetSize() );
		for( nIdx = nRowNo + nInsertCount; nIdx < nEnd; nIdx ++ )
		{
			ULONG nAdjustedNo = m_arrMapR2VY[ nIdx ];
			nAdjustedNo += nInsertCount;
			m_arrMapR2VY.SetAt( nIdx, nAdjustedNo );
		}
		return true;
	}
	virtual bool ColumnHide(
		ULONG nColNo,
		ULONG nHideCount = 1
		)
	{
		ASSERT_VALID( this );
		ULONG nEnd = nColNo + nHideCount, nColCount = ColumnCountGet();
		if( nEnd > nColCount )
		{
			ASSERT( FALSE );
			return false;
		}
		if( nHideCount == 0 )
			return true;
		if( ! MappingEnableX( true ) )
			return false;
		ULONG nColRef = nColNo + nHideCount - 1, nColMapped, nCounter, nIdx;
		for( nCounter = 0; nCounter < nHideCount; nCounter ++, nColRef -- )
		{
			nColMapped = MapV2RX( nColRef );
			ASSERT( nColMapped != ULONG(-1L) );
			m_arrMapV2RX.RemoveAt( nColRef, 1 );
			m_arrMapR2VX.SetAt( nColMapped, ULONG(-1L) );
//			nEnd = ULONG( m_arrMapV2RX.GetSize() );
//			for( nIdx = nColRef; nIdx < nEnd; nIdx ++ )
//			{
//				ULONG nAdjustedNo = m_arrMapV2RX[ nIdx ];
//				nAdjustedNo --;
//				m_arrMapV2RX.SetAt( nIdx, nAdjustedNo );
//			}
			nEnd = ULONG( m_arrMapR2VX.GetSize() );
			for( nIdx = nColMapped; nIdx < nEnd; nIdx ++ )
			{
				ULONG nAdjustedNo = m_arrMapR2VX[ nIdx ];
				nAdjustedNo --;
				m_arrMapR2VX.SetAt( nIdx, nAdjustedNo );
			}
		}
		return true;
	}
	virtual bool RowHide(
		ULONG nRowNo,
		ULONG nHideCount = 1
		)
	{
		ASSERT_VALID( this );
		ULONG nEnd = nRowNo + nHideCount, nRowCount = RowCountGet();
		if( nEnd > nRowCount )
		{
			ASSERT( FALSE );
			return false;
		}
		if( nHideCount == 0 )
			return true;
		if( ! MappingEnableY( true ) )
			return false;
		ULONG nRowRef = nRowNo + nHideCount - 1, nRowMapped, nCounter, nIdx;
		for( nCounter = 0; nCounter < nHideCount; nCounter ++, nRowRef -- )
		{
			nRowMapped = MapV2RY( nRowRef );
			ASSERT( nRowMapped != ULONG(-1L) );
			m_arrMapV2RY.RemoveAt( nRowRef, 1 );
			m_arrMapR2VY.SetAt( nRowMapped, ULONG(-1L) );
//			nEnd = ULONG( m_arrMapV2RY.GetSize() );
//			for( nIdx = nRowRef; nIdx < nEnd; nIdx ++ )
//			{
//				ULONG nAdjustedNo = m_arrMapV2RY[ nIdx ];
//				nAdjustedNo --;
//				m_arrMapV2RY.SetAt( nIdx, nAdjustedNo );
//			}
			nEnd = ULONG( m_arrMapR2VY.GetSize() );
			for( nIdx = nRowMapped; nIdx < nEnd; nIdx ++ )
			{
				ULONG nAdjustedNo = m_arrMapR2VY[ nIdx ];
				nAdjustedNo --;
				m_arrMapR2VY.SetAt( nIdx, nAdjustedNo );
			}
		}
		return true;
	}
	virtual bool ColumnDefaultValueBind(
		ULONG nColNo = (ULONG(-1L)), // if (ULONG(-1L)) - default value for all columns
		CExtGridCell * pCell = NULL // if NULL - remove default value
		)
	{
		ASSERT_VALID( this );
		if( ! MappingIsEnabledX() )
			return _BDP::ColumnDefaultValueBind( nColNo, pCell );
		if( nColNo == ULONG(-1L) )
		{
			ULONG nColNoMapped = MapV2RY( nColNo );
			ASSERT( nColNoMapped != ULONG(-1L) );
			bool bRetVal = _BDP::ColumnDefaultValueBind( nColNoMapped, pCell );
			return bRetVal;
		}
		ULONG nColCount = ColumnCountGet();
		for( nColNo = 0L; nColNo < nColCount; nColNo++ )
		{
			ULONG nColNoMapped = MapV2RY( nColNo );
			ASSERT( nColNoMapped != ULONG(-1L) );
			if( ! _BDP::ColumnDefaultValueBind( nColNoMapped, pCell ) )
				return false;
		}
		return true;
	}
	virtual bool RowDefaultValueBind(
		ULONG nRowNo = (ULONG(-1L)), // if (ULONG(-1L)) - default value for all rows
		CExtGridCell * pCell = NULL // if NULL - remove default value
		)
	{
		ASSERT_VALID( this );
		if( ! MappingIsEnabledY() )
			return _BDP::RowDefaultValueBind( nRowNo, pCell );
		if( nRowNo == ULONG(-1L) )
		{
			ULONG nRowNoMapped = MapV2RY( nRowNo );
			ASSERT( nRowNoMapped != ULONG(-1L) );
			bool bRetVal = _BDP::RowDefaultValueBind( nRowNoMapped, pCell );
			return bRetVal;
		}
		ULONG nRowCount = RowCountGet();
		for( nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
		{
			ULONG nRowNoMapped = MapV2RY( nRowNo );
			ASSERT( nRowNoMapped != ULONG(-1L) );
			if( ! _BDP::RowDefaultValueBind( nRowNoMapped, pCell ) )
				return false;
		}
		return true;
	}
	virtual CExtGridCell * RowDefaultValueGet(
		ULONG nRowNo
		)
	{
		ASSERT_VALID( this );
		ULONG nRowNoMapped = MapV2RY( nRowNo );
		ASSERT( nRowNoMapped != ULONG(-1L) );
		CExtGridCell * pCell = _BDP::RowDefaultValueGet( nRowNoMapped );
		return pCell;
	}
	virtual CExtGridCell * ColumnDefaultValueGet(
		ULONG nColNo
		)
	{
		ASSERT_VALID( this );
		ULONG nColNoMapped = MapV2RY( nColNo );
		ASSERT( nColNoMapped != ULONG(-1L) );
		CExtGridCell * pCell = _BDP::ColumnDefaultValueGet( nColNoMapped );
		return pCell;
	}
	virtual CExtGridCell * CellGet(
		ULONG nColNo,
		ULONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		)
	{
		ASSERT_VALID( this );
		ULONG nColNoMapped = MapV2RX( nColNo );
		ASSERT( nColNoMapped != ULONG(-1L) );
		ULONG nRowNoMapped = MapV2RY( nRowNo );
		ASSERT( nRowNoMapped != ULONG(-1L) );
		CExtGridCell * pCell = _BDP::CellGet( nColNoMapped, nRowNoMapped, pInitRTC, bAutoFindValue, bUseColumnDefaultValue );
		return pCell;
	}
	virtual bool CellRangeSet(
		ULONG nColNo,
		ULONG nRowNo,
		ULONG nColCount = 1L,
		ULONG nRowCount = 1L,
		const CExtGridCell * pCellNewValue = NULL, // if NULL - empty existing cell values
		bool bReplace = false, // false - assign to existing cell instances or column/row type created cells, true - create new cloned copies of pCellNewValue
		CRuntimeClass * pInitRTC = NULL, // runtime class for new cell instance (used if bReplace=false)
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true, // false - use row default value (only when bAutoFindValue=true)
		ULONG * p_nUpdatedCellCount = NULL // returns count of really updated cells (zero also may be treated as success)
		)
	{
		ASSERT_VALID( this );
		if( ! ( MappingIsEnabledX() || MappingIsEnabledY() ) )
			return _BDP::CellRangeSet( nColNo, nRowNo, nColCount, nRowCount, pCellNewValue, bReplace, pInitRTC, bAutoFindValue, bUseColumnDefaultValue, p_nUpdatedCellCount );
		ULONG nWalkX = 0, nWalkY = 0;
		for( ; nWalkY < nRowCount; nWalkY ++ )
		{
			ULONG nRowNoReal = nRowNo + nWalkY;
			ULONG nRowNoMapped = MapV2RY( nRowNoReal );
			if( nRowNoMapped == ULONG(-1L) )
				continue;
			for( ; nWalkX < nColCount; nWalkX ++ )
			{
				ULONG nColNoReal = nColNo + nWalkX;
				ULONG nColNoMapped = MapV2RX( nColNoReal );
				if( nColNoMapped == ULONG(-1L) )
					continue;
				ULONG nUpdatedCellCount = 0L;
				if( ! _BDP::CellRangeSet(
						nColNoMapped,
						nRowNoMapped,
						1L,
						1L,
						pCellNewValue,
						bReplace,
						pInitRTC,
						bAutoFindValue,
						bUseColumnDefaultValue,
						&nUpdatedCellCount
						)
					)
					return false;
				if( p_nUpdatedCellCount != NULL )
					(*p_nUpdatedCellCount) += nUpdatedCellCount;
			}
		}
		return true;
	}
	virtual bool CacheReserveForOuterCells(
		ULONG nColCount,
		ULONG nRowCount
		)
	{
		ASSERT_VALID( this );
		bool bRetVal = _BDP::CacheReserveForOuterCells( nColCount, nRowCount );
		return bRetVal;
	}
	virtual void CacheReservedCountsGet(
		ULONG * p_nColCount,
		ULONG * p_nRowCount
		) const
	{
		ASSERT_VALID( this );
		_BDP::CacheReservedCountsGet( p_nColCount, p_nRowCount );
	}
	virtual bool SortOrderSet(
		const CExtGridDataSortOrder & _gdso,
		bool bColumns, // true = sort order for columns, false - for rows
		CExtGridDataProvider::IDataProviderEvents * pDPE = NULL
		)
	{
		ASSERT_VALID( this );
		CExtGridDataSortOrder _gdsoMapped = _gdso;
		//INT nCount = INT( _gdsoMapped.m_arrItems.GetSize() );
		VERIFY( MapSortOrderV2R( _gdsoMapped, bColumns ) );
		IDataProviderEventsMapped _dpeMapped( this, pDPE );
		bool bRetVal = _BDP::SortOrderSet( _gdsoMapped, bColumns, &_dpeMapped );
		return bRetVal;
	}
	virtual bool SortOrderGet(
		CExtGridDataSortOrder & _gdso,
		bool bColumns // true = sort order for columns, false - for rows
		) const
	{
		ASSERT_VALID( this );
		_gdso.Empty();
		CExtGridDataSortOrder _gdsoMapped;
		if( ! _BDP::SortOrderGet( _gdsoMapped, bColumns ) )
			return false;
		if( ! MapSortOrderR2V( _gdsoMapped, bColumns ) )
			return false;
		_gdso = _gdsoMapped;
		return true;
	}
	virtual bool SwapDroppedSeries(
		bool bColumns, // true = swap columns, false - rows
		ULONG nRowColNoSrc,
		ULONG nRowColNoDropBefore,
		CExtGridDataProvider::IDataProviderEvents * pDPE = NULL
		)
	{
		ASSERT_VALID( this );
		ULONG nRowColNoSrcMapped =  bColumns ? MapV2RX( ULONG(nRowColNoSrc) ) : MapV2RY( ULONG(nRowColNoSrc) );
		ASSERT( nRowColNoSrcMapped != ULONG(-1L) );
		ULONG nRowColNoDropBeforeMapped =  bColumns ? MapV2RX( ULONG(nRowColNoDropBefore) ) : MapV2RY( ULONG(nRowColNoDropBefore) );
		ASSERT( nRowColNoDropBeforeMapped != ULONG(-1L) );
		IDataProviderEventsMapped _dpeMapped( this, pDPE );
		bool bRetVal = _BDP::SwapDroppedSeries( bColumns, nRowColNoSrcMapped, nRowColNoDropBeforeMapped, &_dpeMapped );
		return bRetVal;
	}
	virtual bool _SortSwapSeries(
		LONG nRowColNo1,
		LONG nRowColNo2,
		LONG & nSwapCounter,
		CExtGridDataProvider::IDataProviderEvents * pDPE,
		bool bColumns
		)
	{
		ASSERT_VALID( this );
		LONG nRowColNo1Mapped = LONG( bColumns ? MapV2RX( ULONG(nRowColNo1) ) : MapV2RY( ULONG(nRowColNo1) ) );
		LONG nRowColNo2Mapped = LONG( bColumns ? MapV2RX( ULONG(nRowColNo2) ) : MapV2RY( ULONG(nRowColNo2) ) );
		bool bRetVal = _BDP::_SortSwapSeries( nRowColNo1Mapped, nRowColNo2Mapped, nSwapCounter, pDPE, bColumns );
		return bRetVal;
	}
}; // class CExtMDP

/////////////////////////////////////////////////////////////////////////////
// CExtGridWnd window

class __PROF_UIS_API CExtGridWnd
	: public CExtGridBaseWnd
	, public CExtGridDataProvider::IDataProviderEvents
{
// Construction
public:
	DECLARE_DYNCREATE( CExtGridWnd );
	CExtGridWnd();

// Attributes
public:
	static bool g_bEnableOnIdleCalls;
	HCURSOR m_hCursorOuterDragOK, m_hCursorOuterDragCancel;
protected:
	CExtGridDataProvider * m_pDataProvider;
	DWORD m_dwBseStyle, m_dwBseStyleEx;
#ifdef _DEBUG
	bool m_bDebugModifyingColumns:1, m_bDebugModifyingRows:1;
#endif // _DEBUG
	LONG m_nHelperLastEditedColNo, m_nHelperLastEditedRowNo;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtGridWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtGridWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

	virtual DWORD BseGetStyle() const;
	virtual DWORD BseModifyStyle(
		DWORD dwStyleAdd,
		DWORD dwStyleRemove = 0L,
		bool bRedraw = true
		);
	virtual DWORD BseGetStyleEx() const;
	virtual DWORD BseModifyStyleEx(
		DWORD dwStyleExAdd,
		DWORD dwStyleExRemove = 0L,
		bool bRedraw = true
		);

	// just test/set __EGWS_EX_EXTERNAL_DATA ex-style
	bool ExternalDataGet() const;
	void ExternalDataSet( bool bExternalData );

	virtual CExtGridDataProvider & OnGridQueryDataProvider();
	const CExtGridDataProvider & OnGridQueryDataProvider() const;

	virtual LONG OuterColumnCountLeftSet(
		LONG nCount = 0L,
		bool bRedraw = true
		);
	virtual LONG OuterColumnCountRightSet(
		LONG nCount = 0L,
		bool bRedraw = true
		);
	virtual LONG OuterRowCountTopSet(
		LONG nCount = 0L,
		bool bRedraw = true
		);
	virtual LONG OuterRowCountBottomSet(
		LONG nCount = 0L,
		bool bRedraw = true
		);
	virtual bool ColumnInsert(
		LONG nColNo, // -1 or greater than count - append
		LONG nColInsertCount = 1L,
		bool bRedraw = true
		);
	virtual bool RowInsert(
		LONG nRowNo, // -1 or greater than count - append
		LONG nRowInsertCount = 1L,
		bool bRedraw = true
		);
	virtual LONG ColumnRemove(
		LONG nColNo,
		LONG nColRemoveCount = 1L, // -1 - remove up to end (truncate)
		bool bRedraw = true
		);
	virtual LONG RowRemove(
		LONG nRowNo,
		LONG nRowRemoveCount = 1L, // -1 - remove up to end (truncate)
		bool bRedraw = true
		);
#ifdef _DEBUG
	virtual LONG ColumnCountSet(
		LONG nCount = -1L,
		bool bRedraw = true
		);
	virtual LONG RowCountSet(
		LONG nCount = -1L,
		bool bRedraw = true
		);
#endif // _DEBUG
	virtual LONG ColumnCountGet() const;
	virtual LONG RowCountGet() const;

	bool GridColumnDefaultValueBind(
		LONG nColNo = -1L, // if -1L - default value for all columns
		INT nColType = 0,
		CExtGridCell * pCell = NULL // if NULL - remove default value
		);
	bool GridRowDefaultValueBind(
		LONG nRowNo = -1L, // if -1L - default value for all rows
		INT nRowType = 0,
		CExtGridCell * pCell = NULL // if NULL - remove default value
		);
	bool GridColumnDefaultValueUnbind(
		LONG nColNo = -1L, // if -1L - default value for all columns
		INT nColType = 0
		);
	bool GridRowDefaultValueUnbind(
		LONG nRowNo = -1L, // if -1L - default value for all rows
		INT nRowType = 0
		);
	void GridColumnDefaultValueUnbindAll();
	void GridRowDefaultValueUnbindAll();
	CExtGridCell * GridColumnDefaultValueGet(
		LONG nColNo,
		INT nColType = 0
		);
	const CExtGridCell * GridColumnDefaultValueGet(
		LONG nColNo,
		INT nColType = 0
		) const;
	CExtGridCell * GridRowDefaultValueGet(
		LONG nRowNo,
		INT nRowType = 0
		);
	const CExtGridCell * GridRowDefaultValueGet(
		LONG nRowNo,
		INT nRowType = 0
		) const;
	
	CExtGridCell * GridCellGet(
		const CExtGridHitTestInfo & htInfo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	const CExtGridCell * GridCellGet(
		const CExtGridHitTestInfo & htInfo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	virtual CExtGridCell * GridCellGet(
		LONG nColNo,
		LONG nRowNo,
		INT nColType = 0, // -1 - nColNo is fixed column at left, 1 - at right, 0 - data cell
		INT nRowType = 0, // -1 - nRowNo is fixed column at top, 1 - at bottom, 0 - data cell
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	virtual bool GridCellSet(
		LONG nColNo,
		LONG nRowNo,
		const CExtGridCell * pCell = NULL, // NULL - clear (make empty)
		LONG nColCount = 1L,
		LONG nRowCount = 1L,
		INT nColType = 0, // -1 - nColNo is fixed column at left, 1 - at right, 0 - data cell
		INT nRowType = 0, // -1 - nRowNo is fixed column at top, 1 - at bottom, 0 - data cell
		bool bRedraw = true
		);
	const CExtGridCell * GridCellGet(
		LONG nColNo,
		LONG nRowNo,
		INT nColType = 0, // -1 - nColNo is fixed column at left, 1 - at right, 0 - data cell
		INT nRowType = 0, // -1 - nRowNo is fixed column at top, 1 - at bottom, 0 - data cell
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	CExtGridCell * GridCellGetOuterAtTop(
		LONG nColNo,
		LONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	const CExtGridCell * GridCellGetOuterAtTop(
		LONG nColNo,
		LONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	CExtGridCell * GridCellGetOuterAtBottom(
		LONG nColNo,
		LONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	const CExtGridCell * GridCellGetOuterAtBottom(
		LONG nColNo,
		LONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	CExtGridCell * GridCellGetOuterAtLeft(
		LONG nColNo,
		LONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	const CExtGridCell * GridCellGetOuterAtLeft(
		LONG nColNo,
		LONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	CExtGridCell * GridCellGetOuterAtRight(
		LONG nColNo,
		LONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	const CExtGridCell * GridCellGetOuterAtRight(
		LONG nColNo,
		LONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	bool GridCellSetOuterAtTop(
		LONG nColNo,
		LONG nRowNo,
		const CExtGridCell * pCell = NULL, // NULL - clear (make empty)
		LONG nColCount = 1L,
		LONG nRowCount = 1L,
		bool bRedraw = true
		);
	bool GridCellSetOuterAtBottom(
		LONG nColNo,
		LONG nRowNo,
		const CExtGridCell * pCell = NULL, // NULL - clear (make empty)
		LONG nColCount = 1L,
		LONG nRowCount = 1L,
		bool bRedraw = true
		);
	bool GridCellSetOuterAtLeft(
		LONG nColNo,
		LONG nRowNo,
		const CExtGridCell * pCell = NULL, // NULL - clear (make empty)
		LONG nColCount = 1L,
		LONG nRowCount = 1L,
		bool bRedraw = true
		);
	bool GridCellSetOuterAtRight(
		LONG nColNo,
		LONG nRowNo,
		const CExtGridCell * pCell = NULL, // NULL - clear (make empty)
		LONG nColCount = 1L,
		LONG nRowCount = 1L,
		bool bRedraw = true
		);
	virtual bool GridCellRectsGet(
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		RECT * pRectCell,
		RECT * pRectCellExtra = NULL,
		RECT * pRectText = NULL,
		RECT * pRectTextArea = NULL,
		RECT * pRectIcon = NULL,
		RECT * pRectCheck = NULL,
		RECT * pRectButtonEllipsis = NULL,
		RECT * pRectButtonDropdown = NULL,
		RECT * pRectButtonUp = NULL,
		RECT * pRectButtonDown = NULL,
		RECT * pRectFocusArrow = NULL,
		RECT * pRectSortArrow = NULL
		) const;
protected:
	virtual void OnDataProviderSortEnter(
		bool bColumns // true = sorting/swapping columns, false - rows
		);
	virtual void OnDataProviderSortLeave(
		bool bColumns // true = sorting/swapping columns, false - rows
		);
	virtual void OnDataProviderSwapSeries(
		bool bColumns, // true = sorting/swapping columns, false - rows
		LONG nRowColNo1,
		LONG nRowColNo2,
		LONG nSwapCounter
		);
	virtual void OnDataProviderSwapDroppedSeries(
		bool bColumns, // true = swapping columns, false - rows
		LONG nRowColNoSrc,
		LONG nRowColNoDropBefore
		);
	virtual void OnGridSyncCellSortArrows(
		bool bSortColumns,
		const CExtGridDataSortOrder & _gdso,
		bool bSetup
		);
public:
	virtual bool GridSortOrderGet(
		bool bSortColumns,
		CExtGridDataSortOrder & _gdsoCurrent
		) const;
	virtual bool GridSortOrderSetup(
		bool bSortColumns,
		const CExtGridDataSortOrder & _gdsoUpdate,
		bool bUpdateExistingSortOrder = false,
		bool bInvertIntersectionSortOrder = true,
		bool bRedraw = true
		);
	virtual void OnGbwPaintCell(
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual bool OnGridPaintCellTextHook(
		bool bPostNotification,
		const CExtGridCell & _cell,
		const RECT & rcCellText,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnGbwHoverCell(
		const CExtGridHitTestInfo & htInfoPrevious,
		const CExtGridHitTestInfo & htInfoCurrent,
		bool bHoverEnter,
		bool bHoverLeave
		);
	virtual CExtGridHitTestInfo & HitTest(
		CExtGridHitTestInfo & htInfo,
		bool bReAlignCellResizing,
		bool bSupressZeroTopCellResizing,
		bool bComputeOuterDropAfterState = false
		) const;
	virtual bool OnGbwAnalyzeCellMouseClickEvent(
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nRepCnt, // 0 - button up, 1 - single click, 2 - double click, 3 - post single click & begin editing
		UINT nFlags, // mouse event flags
		CPoint point // mouse pointer in client coordinates
		);
	virtual bool OnGbwAnalyzeCellSysKeyEvent(
		bool bKeyDownEvent, // true - sys-key-down event, false - key-up event
		UINT nChar, // key code
		UINT nRepCnt, // sys-key-down/sys-key-up press count
		UINT nFlags // sys-key-down/sys-key-up event flags
		);
	virtual bool OnGbwAnalyzeCellKeyEvent(
		bool bKeyDownEvent, // true - key-down event, false - key-up event
		UINT nChar, // virtual key code
		UINT nRepCnt, // key-down/key-up press count
		UINT nFlags // key-down/key-up event flags
		);
	virtual bool OnGbwBeginEdit(
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		bool bContinueMsgLoop = true,
		__EXT_MFC_SAFE_LPCTSTR strStartEditText = NULL
		);
	virtual bool OnGbwSetCursor(
		const CExtGridHitTestInfo & htInfo
		);

	virtual bool OnSiwCacheChanged(
		const CExtScrollItemCacheInfo & _sciNew,
		const CExtScrollItemCacheInfo & _sciOld
		);
	virtual bool OnSiwQueryVirtualBOF( bool bHorz ) const;
	virtual bool OnSiwQueryVirtualEOF( bool bHorz ) const;
	virtual bool OnSiwVirtualEndTestH( LONG nColNo, LPARAM lParam ) const;
	virtual bool OnSiwVirtualEndTestV( LONG nRowNo, LPARAM lParam ) const;
	virtual void OnGbwResizingStateApply(
		bool bHorz,
		LONG nItemNo,
		INT nItemExtent
		);
	virtual void OnGbwResizingAdjustComputedTrackExtent(
		INT & nTrackExtentToAdjust,
		INT nTrackExtentMin,
		INT nTrackExtentMax,
		const CExtGridHitTestInfo & htInfo,
		bool bHorz
		) const;
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
	virtual bool OnGbwCanResizeColumn(
		LONG nColNo,
		INT * p_nExtentMin = NULL,
		INT * p_nExtentMax = NULL
		);
	virtual bool OnGbwCanResizeRow(
		LONG nRowNo,
		INT * p_nExtentMin = NULL,
		INT * p_nExtentMax = NULL
		);

	virtual bool OnGbwTooltip(
		const CExtGridHitTestInfo & htInfo
		);
	virtual bool OnGbwExpand(
		const CExtGridHitTestInfo & htInfo,
		UINT nDelayMilliseconds
		);
	virtual bool OnGbwPaintExpandedItemContent(
		CExtContentExpandWnd::PAINT_ITEM_CONTENT_DATA & picd
		) const;
	virtual bool OnGridTrackCellButton(
		CExtGridCell * pCell,
		const CExtGridHitTestInfo & htInfo
		);
	virtual CSize OnGridQueryStartDragOffset(
		const CExtGridHitTestInfo * pHtInfo = NULL
		) const;
	virtual const CExtGridHitTestInfo & OnGridQueryPressingCellInfo() const;
	virtual void OnGridTrackCellMousePressing(
		CExtGridCell * pCell,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nFlags, // mouse event flags
		bool bAllowStartDragEvent = true,
		CExtGridDataSortOrder * p_gdsoUpdate = NULL,
		bool bUpdateMultipleSort = false,
		bool bSortColumns = false
		);
	virtual LONG OnGridHookOuterCellDND(
		bool bFinal,
		LONG nExternalHT,
		const POINT & point,
		CExtGridCell * pCell,
		const CExtGridHitTestInfo & htInfo,
		CExtContentExpandWnd & wndArrows,
		CExtContentExpandWnd & wndDND
		);
	virtual void OnGridTrackOuterCellDND(
		CExtGridCell * pCell,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nFlags // mouse event flags
		);
	virtual void OnGridOuterDragComplete(
		const CExtGridHitTestInfo & htInfoDrag,
		const CExtGridHitTestInfo & htInfoDrop
		);
	virtual void OnGridOuterDragOut(
		const CExtGridHitTestInfo & htInfo
		);
	virtual void OnGridCalcOuterDropTarget(
		const CExtGridHitTestInfo & htInfoDrag,
		CExtGridHitTestInfo & htInfoDrop,
		CPoint point
		);
	virtual void OnGridPaintDraggedCell(
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		const RECT & rcCell,
		DWORD dwAreaFlags
		) const;
	virtual bool OnGridQueryCenteredDndAlignment() const;
	virtual COLORREF OnGbwQueryBackColor(
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnGbwQueryEmptyAreaLinesMetrics(
		bool & bDrawEmptyAreaLinesH_Bottom,
		bool & bDrawEmptyAreaLinesV_Bottom,
		bool & bDrawEmptyAreaLinesH_Right,
		bool & bDrawEmptyAreaLinesV_Right,
		bool & bDrawEmptyAreaLinesH_Corner,
		bool & bDrawEmptyAreaLinesV_Corner,
		INT & nEmptyColumnWidth,
		INT & nEmptyRowHeight
		) const;

protected:
	virtual bool OnGbwProcessVkBeginEdit( UINT nChar );

public:
	virtual void PmBridge_OnPaintManagerChanged(
		CExtPaintManager * pGlobalPM
		);
	INT GridIconGetCount() const;
	CExtCmdIcon * GridIconGetAt( INT nIdx );
	const CExtCmdIcon * GridIconGetAt( INT nIdx ) const;
	INT GridIconInsert( // returns index or -1
		CExtCmdIcon * pIcon,
		INT nIdx = -1, // append
		bool bCopyIcon = true
		);
	INT GridIconInsert( // returns index or -1
		const CExtCmdIcon * pIcon,
		INT nIdx = -1, // append
		bool bCopyIcon = true
		);
	INT GridIconRemove(
		INT nIdx = 0,
		INT nCountToRemove = -1 // all
		);

	INT GridFontGetCount() const;
	HFONT GridFontGetAt( INT nIdx );
	HFONT GridFontGetAt( INT nIdx ) const;
	INT GridFontInsert( // returns index or -1
		HFONT hFont,
		INT nIdx = -1, // append
		bool bCopyFont = true
		);
	INT GridFontRemove(
		INT nIdx = 0,
		INT nCountToRemove = -1 // all
		);

	bool GridEmptyAreaLinesHorzBottomGet() const;
	bool GridEmptyAreaLinesHorzBottomSet(
		bool bGridLines,
		bool bRedraw = true
		);
	bool GridEmptyAreaLinesVertBottomGet() const;
	bool GridEmptyAreaLinesVertBottomSet(
		bool bGridLines,
		bool bRedraw = true
		);
	bool GridEmptyAreaLinesHorzRightGet() const;
	bool GridEmptyAreaLinesHorzRightSet(
		bool bGridLines,
		bool bRedraw = true
		);
	bool GridEmptyAreaLinesVertRightGet() const;
	bool GridEmptyAreaLinesVertRightSet(
		bool bGridLines,
		bool bRedraw = true
		);
	bool GridEmptyAreaLinesHorzCornerGet() const;
	bool GridEmptyAreaLinesHorzCornerSet(
		bool bGridLines,
		bool bRedraw = true
		);
	bool GridEmptyAreaLinesVertCornerGet() const;
	bool GridEmptyAreaLinesVertCornerSet(
		bool bGridLines,
		bool bRedraw = true
		);

	virtual bool ColumnHide(
		LONG nColNo,
		LONG nHideCount = 1,
		bool bRedraw = true
		);
	virtual bool RowHide(
		LONG nRowNo,
		LONG nHideCount = 1,
		bool bRedraw = true
		);
	virtual bool ColumnUnHideAll(
		bool bRedraw = true
		);
	virtual bool RowUnHideAll(
		bool bRedraw = true
		);

	virtual void FilterStateClearAll(
		bool bClearAtTop = true,
		bool bClearAtBottom = true,
		bool bClearAtLeft = true,
		bool bClearAtRight = true
		);
	virtual bool FilterStateForOuterColumnSerialize(
		CArchive & ar,
		bool bLeft = true,
		LONG nColNo = 0L,
		bool bRedrawAfterLoading = true,
		bool bEnableThrowExceptions = false
		);
	virtual bool FilterStateForOuterRowSerialize(
		CArchive & ar,
		bool bTop = true,
		LONG nRowNo = 0L,
		bool bRedrawAfterLoading = true,
		bool bEnableThrowExceptions = false
		);
	bool FilterStateForOuterColumnSave(
		__EXT_MFC_SAFE_LPCTSTR sGridID,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%
		HKEY hKeyRoot = HKEY_CURRENT_USER,
		bool bLeft = true,
		LONG nColNo = 0L,
		bool bEnableThrowExceptions = false
		);
	bool FilterStateForOuterRowSave(
		__EXT_MFC_SAFE_LPCTSTR sGridID,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%
		HKEY hKeyRoot = HKEY_CURRENT_USER,
		bool bTop = true,
		LONG nRowNo = 0L,
		bool bEnableThrowExceptions = false
		);
	bool FilterStateForOuterColumnLoad(
		__EXT_MFC_SAFE_LPCTSTR sGridID,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%
		HKEY hKeyRoot = HKEY_CURRENT_USER,
		bool bLeft = true,
		LONG nColNo = 0L,
		bool bRedrawAfterLoading = true,
		bool bEnableThrowExceptions = false
		);
	bool FilterStateForOuterRowLoad(
		__EXT_MFC_SAFE_LPCTSTR sGridID,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%
		HKEY hKeyRoot = HKEY_CURRENT_USER,
		bool bTop = true,
		LONG nRowNo = 0L,
		bool bRedrawAfterLoading = true,
		bool bEnableThrowExceptions = false
		);

	virtual bool OnGridFilterQueryFilteredState(
		bool bFilteringRows,
		LONG nRowColNo = -1, // if -1 - ignore nRowColNo, check all ranges at both outer sides and fill p_nRowColNoFound & p_bTopLeftFound
		bool bTopLeft = true,
		LONG * p_nRowColNoFound = NULL,
		bool * p_bTopLeftFound = NULL
		);
	virtual void OnGridFilterChanged(
		CExtGridCellHeaderFilter * pHeaderFilterCell,
		CExtGridCell::TrackCellStateInfo_t & _tcsi
		);
	virtual void OnGridFilterCalcRowIndicesForFilteringColumns(
		LONG nScanColNo,
		bool bLeft,
		CList < LONG, LONG & > & _listIndices
		);
	virtual void OnGridFilterCalcColumnIndicesForFilteringRows(
		LONG nScanRowNo,
		bool bTop,
		CList < LONG, LONG & > & _listIndices
		);
	virtual void OnGridFilterUpdateForColumns(
		LONG nScanColNo,
		LONG nEventSrcRowNo,
		bool bLeft,
		CExtGridCellHeaderFilter * pHeaderFilterCell,
		bool bRedraw = true
		);
	virtual void OnGridFilterUpdateForRows(
		LONG nScanRowNo,
		LONG nEventSrcColNo,
		bool bTop,
		CExtGridCellHeaderFilter * pHeaderFilterCell,
		bool bRedraw = true
		);
	virtual bool OnGridFilterCalcPlainUniqueValuesForColumn(
		CExtArray < CExtSafeString > & arrUniqueStringValues,
		LONG nColNo
		) const;
	virtual bool OnGridFilterCalcPlainUniqueValuesForRow(
		CExtArray < CExtSafeString > & arrUniqueStringValues,
		LONG nRowNo
		) const;
	virtual bool OnGridFilterSortCommand(
		LONG nRowColNo,
		bool bSortColumns,
		bool bAscending,
		bool bUpdateExistingSortOrder
		);
	virtual void UpdateStaticFilterValueForOuterColumn(
		LONG nRowNo,
		bool bLeft = true,
		LONG nColNo = 0L
		);
	virtual void UpdateStaticFilterValueForOuterRow(
		LONG nColNo,
		bool bTop = true,
		LONG nRowNo = 0L
		);

	virtual bool OnGridQueryPpvwVisibilityForColumn(
		LONG nColNo,
		INT nColType
		) const;
	virtual bool OnGridQueryPpvwVisibilityForRow(
		LONG nRowNo,
		INT nRowType
		) const;

	virtual void OnGridCellInputComplete(
		CExtGridCell & _cell,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		HWND hWndInputControl = NULL
		);
	virtual void OnGridCellInplaceControlTextInputComplete(
		CExtGridCell & _cell,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		__EXT_MFC_SAFE_LPCTSTR sTextNew,
		bool bSaveChanges
		);
	virtual void OnGridCellInplaceControlDateTimeInputComplete(
		CExtGridCell & _cell,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		COleDateTime dtNew,
		bool bSaveChanges
		);
	virtual void OnGridCellInplaceControlDurationInputComplete(
		CExtGridCell & _cell,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		COleDateTimeSpan dtSpanNew,
		bool bSaveChanges
		);
	virtual void OnGridCellInplaceControlHotKeyInputComplete(
		CExtGridCell & _cell,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwHotKeyNew,
		bool bSaveChanges
		);
	virtual void OnGridCellInplaceControlSliderInputComplete(
		CExtGridCell & _cell,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		INT nPosNew,
		bool bSaveChanges
		);
	virtual void OnGridCellInplaceControlIPAddressInputComplete(
		CExtGridCell & _cell,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwIPAddressNew,
		bool bSaveChanges
		);
	virtual void OnGridCellInplaceControlAdjustCreationRect(
		CExtGridCell & _cell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		RECT & rcInplaceControl, // RECT to adjust
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual HWND OnGridCellInplaceControlCreate(
		CExtGridCell & _cell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl,
		LONG nLastEditedColNo,
		LONG nLastEditedRowNo
		);
	virtual bool OnGridCellInplaceControlPreTranslateMessage(
		CExtGridCell & _cell,
		MSG * pMsg,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl
		);
	virtual bool OnGridCellInplaceControlTextInputVerify(
		const CExtGridCell & _cell,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		__EXT_MFC_SAFE_LPCTSTR sTextInitial,
		__EXT_MFC_SAFE_LPCTSTR sTextPrevious,
		CExtSafeString & sTextNew,
		bool bEndEdit
		);
	virtual bool OnGridCellInplaceControlDateTimeInputVerify(
		const CExtGridCell & _cell,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		COleDateTime dtInitial,
		COleDateTime dtPrevious,
		COleDateTime & dtNew,
		bool bEndEdit
		);
	virtual bool OnGridCellInplaceControlDurationInputVerify(
		const CExtGridCell & _cell,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		COleDateTimeSpan dtSpanInitial,
		COleDateTimeSpan dtSpanPrevious,
		COleDateTimeSpan & dtSpanNew,
		bool bEndEdit
		);
	virtual bool OnGridCellInplaceControlHotKeyInputVerify(
		const CExtGridCell & _cell,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwHotKeyInitial,
		DWORD dwHotKeyPrevious,
		DWORD & dwHotKeyNew,
		bool bEndEdit
		);
	virtual bool OnGridCellInplaceControlSliderInputVerify(
		const CExtGridCell & _cell,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		INT nPosInitial,
		INT nPosPrevious,
		INT & nPosNew,
		bool bEndEdit
		);
	virtual bool OnGridCellInplaceControlIPAddressInputVerify(
		const CExtGridCell & _cell,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwIPAddressInitial,
		DWORD dwIPAddressPrevious,
		DWORD & dwIPAddressNew,
		bool bEndEdit
		);
	virtual bool OnGridCellInplaceControlWindowProc(
		CExtGridCell & _cell,
		LRESULT & lResult,
		UINT nMessage,
		WPARAM wParam,
		LPARAM lParam,
		HWND hWndInplaceControl,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcInplaceControl
		);
	virtual void OnGridCellInplaceControlQueryStyle(
		const CExtGridCell & _cell,
		CWnd * pWndInplaceControlToCreate,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		bool & bReadOnly,
		bool & bPassword,
		COLORREF & clrBack,
		COLORREF & clrText
		);
	virtual void OnGridCellSetCheck(
		CExtGridCell & _cell,
		INT nCheck,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell
		);
	virtual HFONT OnGridCellQueryFont(
		const CExtGridCell & _cell,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		bool & bFontMustBeDestroyed,
		DWORD dwHelperPaintFlags = 0
		) const;
	virtual COLORREF OnGridCellQueryTextColor(
		const CExtGridCell & _cell,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual COLORREF OnGridCellQueryBackColor(
		const CExtGridCell & _cell,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual bool OnGridCellButtonPressing(
		CExtGridCell & _cell,
		INT nButtonType,
		bool bMouseInsideButton,
		ULONG nPressedTimerCounter,
		const RECT & rcButton,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnGridCellButtonPressed(
		CExtGridCell & _cell,
		INT nButtonType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnGridCellGetToolTipText(
		const CExtGridCell & _cell,
		const CExtGridHitTestInfo & htInfo,
		CExtSafeString & strToolTipText
		) const;
	virtual CRect OnSwRecalcLayout(
		bool bDoLayout,
		LPCRECT pRectClientSrc = NULL
		);
	virtual void OnGridDoProportionalResizing(
		INT nClientAreaExtent,
		bool bResizeColumns, // true - columns, false - rows
		LONG nFirstItemNumber
		);

	virtual bool OnQueryBestFitActionEnabled(
		bool bBestFitByColumnWidth,
		const CExtGridHitTestInfo & htInfoBestFit
		) const;
	virtual bool BestFitColumn(
		LONG nColNo,
		INT nColType = 0,
		bool bDoBestFitByOuterCells = true,
		bool bDoBestFitByInnerCells = true,
		bool bDoBestFitByVisibleRowRangeOnly = false,
		bool bRedraw = true
		);
	virtual bool BestFitRow(
		LONG nRowNo,
		INT nRowType = 0,
		bool bDoBestFitByOuterCells = true,
		bool bDoBestFitByInnerCells = true,
		bool bDoBestFitByVisibleColumnRangeOnly = false,
		bool bRedraw = true
		);
	virtual void OnBestFitAdjustColumn(
		INT & nBFE,
		const CExtGridCell * pCell,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual void OnBestFitAdjustRow(
		INT & nBFE,
		const CExtGridCell * pCell,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType
		);
	virtual bool OnGridHookCellPaintBackground(
		const CExtGridCell & _cell,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual bool OnGridHookCellPaintForeground(
		const CExtGridCell & _cell,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;

	virtual CSize GridCellJoinGet(
		LONG nColNo,
		LONG nRowNo,
		INT nColType = 0,
		INT nRowType = 0
		) const;
	virtual bool GridCellJoinTest(
		CSize sizeJoin,
		LONG nColNo,
		LONG nRowNo,
		INT nColType = 0,
		INT nRowType = 0,
		bool bCheckCellsPresentInJoinedArea = false,
		bool bCheckFrozenAreaIntersection = true
		) const;
	virtual bool GridCellJoinSet(
		CSize sizeJoin,
		LONG nColNo,
		LONG nRowNo,
		INT nColType = 0,
		INT nRowType = 0,
		bool bValidateJoin = false,
		bool bCheckCellsPresentInJoinedArea = false,
		bool bCheckFrozenAreaIntersection = true
		);
	virtual CSize OnGbwCellJoinQueryInfo(
		LONG nColNo,
		LONG nRowNo,
		INT nColType, // = 0
		INT nRowType // = 0
		) const;
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

	virtual void OnGbwResizingAdjustHtInfo(
		CExtGridHitTestInfo & htInfo
		);

	virtual bool OnGridHookCellQueryEnabledIncrement(
		LONG nColNo,
		LONG nRowNo,
		LONG nColType,
		LONG nRowType,
		const CExtGridCell & _cell,
		bool & bEnabled
		) const;
	virtual bool OnGridHookCellQueryEnabledDecrement(
		LONG nColNo,
		LONG nRowNo,
		LONG nColType,
		LONG nRowType,
		const CExtGridCell & _cell,
		bool & bEnabled
		) const;
	virtual bool OnGridHookCellValueIncrement(
		LONG nColNo,
		LONG nRowNo,
		LONG nColType,
		LONG nRowType,
		CExtGridCell & _cell,
		bool & bResult
		);
	virtual bool OnGridHookCellValueDecrement(
		LONG nColNo,
		LONG nRowNo,
		LONG nColType,
		LONG nRowType,
		const CExtGridCell & _cell,
		bool & bResult
		);
	virtual bool OnGridHookCellQueryButtonInfo(
		INT nButtonType,
		const CExtGridCell & _cell,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		bool * p_bEnabled,
		bool * p_bPressed,
		bool * p_bStayPressed,
		UINT * p_nTimerElapseValue,
		bool & bRetVal
		) const;

	virtual CRect OnSiwGetFrozenRange() const;

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtGridWnd)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtGridWnd

#endif // (!defined __EXT_MFC_NO_GRIDWND)

#endif // (!defined __EXT_MFC_NO_GRIDBASEWND)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined __EXT_GRIDWND_H

