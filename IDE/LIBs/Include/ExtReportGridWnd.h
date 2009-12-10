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

#if (!defined __EXT_REPORT_GRID_WND_H)
#define __EXT_REPORT_GRID_WND_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXTTREEGRIDWND_H)
	#include <ExtTreeGridWnd.h>
#endif

#if (!defined __EXT_COMBO_BOX_H)
	#include <ExtComboBox.h>
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined __EXT_MFC_NO_REPORTGRIDWND)

class CExtReportGridCategoryComboBox;
class CExtReportGridColumnChooserMiniFrameWnd;
class CExtReportGridColumnChooserWnd;
class CExtReportGridGroupAreaWnd;
class CExtReportGridColumn;
class CExtReportGridItem;
class CExtReportGridWnd;
class CExtPopupMenuTipWnd;
class CExtPopupMenuWnd;

/////////////////////////////////////////////////////////////////////////////
// CExtReportGridCategoryComboBox window

class __PROF_UIS_API CExtReportGridCategoryComboBox : public CExtComboBoxBase
{
protected:
	CExtReportGridWnd * m_pRGW;
public:
	DECLARE_DYNCREATE( CExtReportGridCategoryComboBox );
	CExtReportGridCategoryComboBox(
		CExtReportGridWnd * pRGW = NULL
		);
	virtual ~CExtReportGridCategoryComboBox();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif

	virtual bool Create(
		CWnd * pWndParent,
		bool bVisible = true
		);

protected:
	virtual void OnSynchronizeCurSel();

public:
	// Overrides
	//{{AFX_VIRTUAL(CExtReportGridCategoryComboBox)
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	//{{AFX_MSG(CExtReportGridCategoryComboBox)
	//}}AFX_MSG
	afx_msg void OnSelEndOK();
	afx_msg LRESULT OnSizeParent( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

	friend class CExtReportGridWnd;
	friend class CExtReportGridColumnChooserWnd;
}; // class CExtReportGridCategoryComboBox

/////////////////////////////////////////////////////////////////////////////
// CExtReportGridColumnChooserMiniFrameWnd window

class __PROF_UIS_API CExtReportGridColumnChooserMiniFrameWnd : public CMiniFrameWnd
{
	CExtReportGridWnd & m_RGW;
public:
	DECLARE_DYNAMIC( CExtReportGridColumnChooserMiniFrameWnd );
	CExtReportGridColumnChooserMiniFrameWnd(
		CExtReportGridWnd & _RGW
		);
	virtual ~CExtReportGridColumnChooserMiniFrameWnd();
	CExtReportGridWnd & GetReportGridWnd();
	const CExtReportGridWnd & GetReportGridWnd() const;
	virtual bool Create();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtReportGridColumnChooserMiniFrameWnd)
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	virtual void RecalcLayout( BOOL bNotify = TRUE );
	//}}AFX_VIRTUAL
	afx_msg UINT OnNcHitTest(CPoint point);

protected:
	//{{AFX_MSG(CExtReportGridColumnChooserMiniFrameWnd)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtReportGridColumnChooserMiniFrameWnd

#define __EXT_COLUMN_CHOOSER_MINI_FRAME_CLASS_NAME _T("ProfUIS-ColumnChooser-MiniFrame")

/////////////////////////////////////////////////////////////////////////////
// CExtReportGridColumnChooserWnd window

class __PROF_UIS_API CExtReportGridColumnChooserWnd : public CExtGridWnd
{
	CExtReportGridWnd & m_RGW;
	CRect m_rcExtents;
	UINT m_nOrientation; // AFX_IDW_DOCKBAR_FLOAT(default), ..LEFT, ..RIGHT, ..TOP, ..BOTTOM
public:
	CExtScrollBar m_wndScrollBarV;
	DECLARE_DYNAMIC( CExtReportGridColumnChooserWnd );
	CExtReportGridColumnChooserWnd(
		CExtReportGridWnd & _RGW
		);
	virtual ~CExtReportGridColumnChooserWnd();
	CExtReportGridWnd & GetReportGridWnd();
	const CExtReportGridWnd & GetReportGridWnd() const;
	virtual CRect ExtentsGet() const;
	virtual void ExtentsSet(
		const RECT & rcExtents
		);
	virtual UINT OrientationGet() const;
	virtual void OrientationSet(
		UINT nOrientation
		);
	void SetContentFromCategory(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName = __EXT_MFC_SAFE_LPCTSTR(NULL) // NULL - empty content
		);
protected:
	virtual bool _CreateHelper();
public:
	virtual INT OnSiwQueryItemExtentH(
		LONG nColNo,
		INT * p_nExtraSpaceBefore = NULL,
		INT * p_nExtraSpaceAfter = NULL
		) const;
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
	virtual bool OnGbwDataDndIsAllowed() const;
	virtual CPoint OnGbwDataDndGetStartOffset() const;
	virtual bool OnGbwDataDndCanStart(
		const CExtGridHitTestInfo & htInfo
		);
	virtual void OnGbwDataDndDo(
		const CExtGridHitTestInfo & htInfo
		);
	virtual void OnSiwPaintForeground(
		CDC & dc,
		bool bFocusedControl
		) const;
	virtual void OnGbwEraseArea(
		CDC & dc,
		const RECT & rcArea,
		DWORD dwAreaFlags
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

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtReportGridColumnChooserWnd)
	public:
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExtReportGridColumnChooserWnd)
	//}}AFX_MSG
	afx_msg LRESULT OnSizeParent( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
}; // class CExtReportGridColumnChooserWnd

/////////////////////////////////////////////////////////////////////////////
// CExtReportGridSortOrder

class __PROF_UIS_API CExtReportGridSortOrder : public CObject
{
protected:
	CTypedPtrArray < CPtrArray, CExtReportGridColumn * > m_arrColumns;
	CArray < bool, bool > m_arrAscendingFlags;
	LONG m_nGroupCount;
public:
	DECLARE_DYNCREATE( CExtReportGridSortOrder );
	CExtReportGridSortOrder();
	CExtReportGridSortOrder( const CExtReportGridSortOrder & other );
	virtual ~CExtReportGridSortOrder();
	CExtReportGridSortOrder & operator = ( const CExtReportGridSortOrder & other );
	virtual void Assign( const CExtReportGridSortOrder & other );
	bool operator == ( const CExtReportGridSortOrder & other ) const;
	bool operator != ( const CExtReportGridSortOrder & other ) const;
	virtual bool IsEqual( const CExtReportGridSortOrder & other ) const;
	virtual void Empty();
	virtual bool IsEmpty() const;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif
protected:
	bool _VerifyOwner( const CExtReportGridWnd * pRGW ) const; // if not NULL - verify all column pointers are owned by report grid
	bool _VerifySortImpl() const; // verify all column pointers are marked as sort-enabled
	bool _VerifyUniqueImpl() const; // verify all column pointers are unique
public:
	virtual LONG ColumnCountGet() const;
	virtual LONG ColumnGroupCountGet() const;
	virtual bool ColumnGroupCountSet( LONG nGroupCount );
	virtual CExtReportGridColumn * ColumnGetAt(
		LONG nIndex,
		bool * p_bAscending = NULL
		);
	const CExtReportGridColumn * ColumnGetAt(
		LONG nIndex,
		bool * p_bAscending = NULL
		) const;
	virtual bool ColumnSetAt(
		CExtReportGridColumn * pRGC,
		LONG nIndex,
		bool bAscending
		);
	virtual bool ColumnInsert(
		CExtReportGridColumn * pRGC,
		LONG nIndex, // -1 - append
		bool bAscending
		);
	virtual bool ColumnRemove(
		LONG nIndex,
		LONG nCountToRemove = 1
		);
	bool ColumnRemove(
		const CExtReportGridColumn * pRGC,
		LONG nCountToRemove = 1
		);
	virtual LONG ColumnGetIndexOf(
		const CExtReportGridColumn * pRGC
		) const;
	virtual bool VerifyColumns(
		const CExtReportGridWnd * pRGW = NULL, // if not NULL - verify all column pointers are owned by report grid
		bool bVerifyUnique = true, // verify all column pointers are unique
		bool bVerifySort = true // verify all column pointers are marked as sort-enabled
		) const;
}; // class CExtReportGridSortOrder

/////////////////////////////////////////////////////////////////////////////
// CExtReportGridGroupAreaWnd window

class __PROF_UIS_API CExtReportGridGroupAreaWnd
	: public CWnd
	, public CExtMouseCaptureSink
{
	CExtReportGridWnd & m_RGW;
protected:
	CExtReportGridColumn * m_pHelperProcessedRGC;
	static bool g_bReportGroupAreaWndClassRegistered;
	bool m_bDirectCreateCall:1;
	INT m_nMinimalHeight, m_nMaxItemWidth;
	CRect m_rcLayoutIndent;
	virtual bool _CreateHelper();
public:
	DECLARE_DYNAMIC( CExtReportGridGroupAreaWnd );
	CExtReportGridGroupAreaWnd(
		CExtReportGridWnd & _RGW
		);
	virtual ~CExtReportGridGroupAreaWnd();
	CExtReportGridWnd & GetReportGridWnd();
	const CExtReportGridWnd & GetReportGridWnd() const;

	static bool RegisterReportGroupAreaWndClass();
	bool Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( __EXT_MFC_IDC_STATIC ),
		DWORD dwWindowStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS
		);
	virtual INT MinimalHeightGet() const;
	virtual void MinimalHeightSet( INT nMinimalHeight );
	virtual INT MaxItemWidthGet() const;
	virtual void MaxItemWidthSet( INT nMaxItemWidth );
	virtual CRect LayoutIndentGet() const;
	virtual void LayoutIndentSet( const RECT & rcLayoutIndent );
	virtual CExtReportGridSortOrder * OnQueryReportGridSortOrder();
	CExtReportGridSortOrder * GetReportGridSortOrder();
	const CExtReportGridSortOrder * GetReportGridSortOrder() const;
 	virtual CSize OnCalcLayoutSize() const;
 	virtual CSize OnCalcItemSize(
 		const CExtReportGridColumn * pRGC
 		) const;
	virtual CSize OnQueryItemOffset() const;
	virtual void OnQueryConnectorInfo(
		CPen * pPenToCreate,
		INT & nHorzDistanceFromRight,
		INT & nVertDistanceFromBottom
		) const;
	virtual LONG ItemHitTest(
		const POINT & ptClient,
		RECT * pRectItem = NULL
		) const;
	virtual LONG ItemDropHitTest( // returns index of item before which ptClient drops new item or -1
		const POINT & ptClient
		) const;
	virtual bool ItemDropMarkerGet(
		LONG nHT,
		POINT & ptTop,
		INT & nHeight
		) const;

	virtual void DoPaint( CDC & dc );

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtReportGridGroupAreaWnd)
protected:
	virtual BOOL PreCreateWindow( CREATESTRUCT & cs );
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExtReportGridGroupAreaWnd)
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg BOOL OnEraseBkgnd( CDC * pDC );
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnSizeParent( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT _OnPaintExpandedItemContent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
}; // class CExtReportGridGroupAreaWnd

// report group area wnd class name
#define __EXT_REPORT_GROUP_AREA_WND_CLASS_NAME _T("ProfUIS-ReportGroupArea")

/////////////////////////////////////////////////////////////////////////////
// CExtReportGridColumn cell

class __PROF_UIS_API CExtReportGridColumn : public CExtGridCellHeaderFilter
{
public:
	DECLARE_SERIAL( CExtReportGridColumn );
	IMPLEMENT_ExtGridCell_Clone( CExtReportGridColumn, CExtGridCellHeaderFilter );
	CExtReportGridColumn(
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtReportGridColumn( const CExtGridCell & other );
	virtual ~CExtReportGridColumn();
	mutable UINT m_nColumnListMenuID;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );
	virtual void TextSet(
		__EXT_MFC_SAFE_LPCTSTR str = __EXT_MFC_SAFE_LPCTSTR(NULL), // empty text
		bool bAllowChangeDataType = false
		);
	virtual __EXT_MFC_SAFE_LPCTSTR ColumnNameGet() const;
	virtual void ColumnNameSet(
		__EXT_MFC_SAFE_LPCTSTR strColumnName
		);
	virtual __EXT_MFC_SAFE_LPCTSTR CategoryNameGet() const;
	virtual void CategoryNameSet(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName
		);
	bool ColumnIsActive() const;
protected:
	virtual void InitNames(
		__EXT_MFC_SAFE_LPCTSTR strColumnName,
		__EXT_MFC_SAFE_LPCTSTR strCategoryName
		);
protected:
	bool m_bHelperIsActive:1, m_bGroupingEnabled:1, m_bSortingAscending:1,
		m_bCtxMenuVisible:1, m_bCtxMenuAlignmentCommandsEnabled:1;
	CExtSafeString m_strCategoryName;
	mutable CExtReportGridWnd * m_pRGW;
public:
	CExtReportGridWnd * GetReportGrid() const;
	virtual bool CtxMenuVisibleGet() const;
	virtual void CtxMenuVisibleSet(
		bool bSet = true
		);
	virtual bool CtxMenuAlignmentCommandsEnabledGet() const;
	virtual void CtxMenuAlignmentCommandsEnabledSet(
		bool bSet = true
		);
	virtual bool GroupingEnabledGet() const;
	virtual void GroupingEnabledSet(
		bool bGroupingEnabled = true
		);
	virtual bool SortingEnabledGet() const;
	virtual void SortingEnabledSet(
		bool bSortingEnabled = true
		);
	virtual bool DragDropEnabledGet() const;
	virtual void DragDropEnabledSet(
		bool bDragDropEnabled = true
		);
	virtual bool SortingAscendingGet() const;
	virtual void SortingAscendingSet(
		bool bSortingAscending = true
		);
	virtual bool OnCellPressingLeave(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nFlags, // mouse event flags
		bool bPressedEvent,
		bool bStartDragEvent
		);
	virtual CSize OnCalcSizeInGroupArea() const;
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
	friend class CExtReportGridWnd;
}; // class CExtReportGridColumn

/////////////////////////////////////////////////////////////////////////////
// CExtReportGridItem cell

class __PROF_UIS_API CExtReportGridItem : public CExtTreeGridCellNode
{
public:
	DECLARE_SERIAL( CExtReportGridItem );
	IMPLEMENT_ExtGridCell_Clone( CExtReportGridItem, CExtTreeGridCellNode );
	CExtReportGridItem(
		CExtGridDataProvider * pDataProvider = NULL
		);
//	CExtReportGridItem( const CExtGridCell & other );
	virtual ~CExtReportGridItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );
	virtual ULONG TreeNodeCalcOffset(
		bool bExpandedOnly
		) const;
	virtual bool OnQueryExtraSpace(
		INT & nItemExtraSpace,
		bool bGet,
		bool bAfter // true - extra space after, false - after before
		);
	virtual void PreviewAreaDestroy();
	virtual CExtGridCell * PreviewAreaGet(
		CRuntimeClass * pInitRTC = NULL
		);
protected:
	CExtGridCell * m_pCellPreviewArea;
	CExtReportGridWnd * m_pRGW;
	friend class CExtReportGridDataProvider;
	friend class CExtReportGridWnd;
}; // class CExtReportGridItem

/////////////////////////////////////////////////////////////////////////////
// CExtReportGridDataProvider

class __PROF_UIS_API CExtReportGridDataProvider : public CExtTreeGridDataProvider
{
	CExtReportGridSortOrder m_rgso;
public:
	DECLARE_DYNCREATE( CExtReportGridDataProvider );
	CExtReportGridDataProvider();
	virtual ~CExtReportGridDataProvider();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG
	// report sort order
	virtual CExtReportGridSortOrder & ReportSortOrderGet();
	virtual bool ReportSortOrderSet(
		const CExtReportGridSortOrder & _rgso,
		bool bUpdateSortOrder,
		CExtReportGridWnd * pRGW
		);
	virtual bool ReportSortOrderUpdate(
		CExtReportGridWnd * pRGW
		);
	virtual bool ReportSortOrderVerify(
		const CExtReportGridSortOrder & _rgso,
		CExtReportGridWnd * pMsgRGW
		) const;
	virtual void _RgsoHideGroups(
		LONG nColumnGroupCount,
		LONG nNestedIndex = 0L,
		CExtTreeGridCellNode * pNode = NULL
		);
	// generic sort order
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
	// column operations
	virtual void RemoveReportColumn( CExtReportGridColumn * pRGC );
	virtual ULONG InactiveColumnCountGet() const;
	virtual ULONG ActiveColumnCountGet() const;
	virtual bool ActivateColumn(
		ULONG nIndexInactive,
		ULONG nIndexActive = ULONG(-1L) // ULONG(-1L) - last position
		);
	virtual bool DeactivateColumn(
		ULONG nIndexActive,
		ULONG nIndexInactive = ULONG(-1L) // ULONG(-1L) - last position
		);
	virtual ULONG ActiveIndexGet(
		const CExtReportGridColumn * pRGC
		) const;
	virtual ULONG InactiveIndexGet(
		const CExtReportGridColumn * pRGC
		) const;
	virtual CExtReportGridColumn * ActiveColumnGet(
		ULONG nIndex
		);
	const CExtReportGridColumn * ActiveColumnGet(
		ULONG nIndex
		) const;
	virtual CExtReportGridColumn * InactiveColumnGet(
		ULONG nIndex
		);
	const CExtReportGridColumn * InactiveColumnGet(
		ULONG nIndex
		) const;
	virtual CExtGridCell * ReportItemGetOuterCell(
		LONG nPlainColNo,
		const CExtReportGridItem * pRGI,
		CRuntimeClass * pRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	const CExtGridCell * ReportItemGetOuterCell(
		LONG nPlainColNo,
		const CExtReportGridItem * pRGI,
		CRuntimeClass * pRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	virtual bool ReportItemSetOuterCell(
		LONG nPlainColNo,
		const CExtReportGridItem * pRGI,
		const CExtGridCell * pCellNewValue = NULL, // if NULL - empty existing cell values
		LONG nColCount = 1L,
		LONG nRowCount = 1L,
		bool bReplace = false, // false - assign to existing cell instances or column/row type created cells, true - create new cloned copies of pCellNewValue
		CRuntimeClass * pInitRTC = NULL // runtime class for new cell instance (used if bReplace=false)
		);
	virtual CExtGridCell * ReportItemGetCell(
		const CExtReportGridColumn * pRGC,
		const CExtReportGridItem * pRGI,
		CRuntimeClass * pRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	const CExtGridCell * ReportItemGetCell(
		const CExtReportGridColumn * pRGC,
		const CExtReportGridItem * pRGI,
		CRuntimeClass * pRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	virtual bool ReportItemSetCell(
		const CExtReportGridColumn * pRGC,
		const CExtReportGridItem * pRGI,
		const CExtGridCell * pCellNewValue = NULL, // if NULL - empty existing cell values
		bool bReplace = false, // false - assign to existing cell instances or column/row type created cells, true - create new cloned copies of pCellNewValue
		CRuntimeClass * pInitRTC = NULL // runtime class for new cell instance (used if bReplace=false)
		);
	friend class CExtReportGridWnd;
}; // class CExtReportGridDataProvider

/////////////////////////////////////////////////////////////////////////////
// CExtReportGridWnd window

// report grid's styles
// (CExtReportGridWnd::ReportGridGetStyle(),
// (used in the CExtReportGridWnd::ReportGridGetStyle() and
// CExtReportGridWnd::ReportGridModifyStyle() methods)

#define __ERGS_AUTO_PREVIEW									0x00000001
#define __ERGS_LAYOUT_AND_STYLE_2003						0x00000002
#define __ERGS_COLLAPSE_AFTER_REGROUPING					0x00000004
#define __ERGS_ENABLE_COLUMN_CTX_MENU						0x00000008
#define __ERGS_ENABLE_COLUMN_LIST_IN_CTX_MENU				0x00000010
#define __ERGS_BOLD_GROUPS									0x00000020
#define __ERGS_KEEP_COLUMNS_ACTIVE_ON_GROUPING				0x00000040
#define __ERGS_ENABLE_DEACTIVATION_OF_LAST_COLUMN			0x00000080
#define __ERGS_USER_DEFINED_23								0x00000100
#define __ERGS_USER_DEFINED_22								0x00000200
#define __ERGS_USER_DEFINED_21								0x00000400
#define __ERGS_USER_DEFINED_20								0x00000800
#define __ERGS_USER_DEFINED_19								0x00001000
#define __ERGS_USER_DEFINED_18								0x00002000
#define __ERGS_USER_DEFINED_17								0x00004000
#define __ERGS_USER_DEFINED_16								0x00008000
#define __ERGS_USER_DEFINED_15								0x00010000
#define __ERGS_USER_DEFINED_14								0x00020000
#define __ERGS_USER_DEFINED_13								0x00040000
#define __ERGS_USER_DEFINED_12								0x00080000
#define __ERGS_USER_DEFINED_11								0x00100000
#define __ERGS_USER_DEFINED_10								0x00200000
#define __ERGS_USER_DEFINED_09								0x00400000
#define __ERGS_USER_DEFINED_08								0x00800000
#define __ERGS_USER_DEFINED_07								0x01000000
#define __ERGS_USER_DEFINED_06								0x02000000
#define __ERGS_USER_DEFINED_05								0x04000000
#define __ERGS_USER_DEFINED_04								0x08000000
#define __ERGS_USER_DEFINED_03								0x10000000
#define __ERGS_USER_DEFINED_02								0x20000000
#define __ERGS_USER_DEFINED_01								0x40000000
#define __ERGS_USER_DEFINED_00								0x80000000

#define __ERGS_DEFAULT \
	( __ERGS_LAYOUT_AND_STYLE_2003 \
	| __ERGS_COLLAPSE_AFTER_REGROUPING \
	| __ERGS_ENABLE_COLUMN_CTX_MENU \
	| __ERGS_ENABLE_COLUMN_LIST_IN_CTX_MENU \
	)

// report grid's extended styles
// (used in the CExtReportGridWnd::ReportGridGetStyleEx() and
// CExtReportGridWnd::ReportGridModifyStyleEx() methods)

#define __ERGS_EX_USER_DEFINED_31							0x00000001
#define __ERGS_EX_USER_DEFINED_30							0x00000002
#define __ERGS_EX_USER_DEFINED_29							0x00000004
#define __ERGS_EX_USER_DEFINED_28							0x00000008
#define __ERGS_EX_USER_DEFINED_27							0x00000010
#define __ERGS_EX_USER_DEFINED_26							0x00000020
#define __ERGS_EX_USER_DEFINED_25							0x00000040
#define __ERGS_EX_USER_DEFINED_24							0x00000080
#define __ERGS_EX_USER_DEFINED_23							0x00000100
#define __ERGS_EX_USER_DEFINED_22							0x00000200
#define __ERGS_EX_USER_DEFINED_21							0x00000400
#define __ERGS_EX_USER_DEFINED_20							0x00000800
#define __ERGS_EX_USER_DEFINED_19							0x00001000
#define __ERGS_EX_USER_DEFINED_18							0x00002000
#define __ERGS_EX_USER_DEFINED_17							0x00004000
#define __ERGS_EX_USER_DEFINED_16							0x00008000
#define __ERGS_EX_USER_DEFINED_15							0x00010000
#define __ERGS_EX_USER_DEFINED_14							0x00020000
#define __ERGS_EX_USER_DEFINED_13							0x00040000
#define __ERGS_EX_USER_DEFINED_12							0x00080000
#define __ERGS_EX_USER_DEFINED_11							0x00100000
#define __ERGS_EX_USER_DEFINED_10							0x00200000
#define __ERGS_EX_USER_DEFINED_09							0x00400000
#define __ERGS_EX_USER_DEFINED_08							0x00800000
#define __ERGS_EX_USER_DEFINED_07							0x01000000
#define __ERGS_EX_USER_DEFINED_06							0x02000000
#define __ERGS_EX_USER_DEFINED_05							0x04000000
#define __ERGS_EX_USER_DEFINED_04							0x08000000
#define __ERGS_EX_USER_DEFINED_03							0x10000000
#define __ERGS_EX_USER_DEFINED_02							0x20000000
#define __ERGS_EX_USER_DEFINED_01							0x40000000
#define __ERGS_EX_USER_DEFINED_00							0x80000000

#define __ERGS_EX_DEFAULT							0

class __PROF_UIS_API CExtReportGridWnd : public CExtTreeGridWnd
{
protected:
	DWORD m_dwReportGridStyle, m_dwReportGridStyleEx;
public:
	bool m_bHelperRenderingGroupArea:1, m_bHelperRenderingOuterDND:1;
	static CExtBitmap g_bmpHeaderAreaBk;
	static RECT g_rcHeaderAreaPadding;
public:
	DECLARE_DYNCREATE( CExtReportGridWnd );
	CExtReportGridWnd();
	virtual ~CExtReportGridWnd();

	UINT m_nColumnListMenuStartID;

	virtual DWORD ReportGridGetStyle() const;
	virtual DWORD ReportGridModifyStyle(
		DWORD dwStyleAdd,
		DWORD dwStyleRemove = 0L,
		bool bRedraw = true
		);
	virtual DWORD ReportGridGetStyleEx() const;
	virtual DWORD ReportGridModifyStyleEx(
		DWORD dwStyleExAdd,
		DWORD dwStyleExRemove = 0L,
		bool bRedraw = true
		);

	virtual CExtGridDataProvider & OnGridQueryDataProvider();
	CExtReportGridDataProvider & GetReportData();
	const CExtReportGridDataProvider & GetReportData() const;

protected:
	CExtReportGridDataProvider & _GetReportData() const;
	mutable LONG m_nHelerLockSiwDrawFocusRect,
		m_nHelerLockOnSwUpdateScrollBars, m_nHelerLockDrawHL,
		m_nHelerLockEnsureVisibleColumn, m_nHelerLockEnsureVisibleRow;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtReportGridWnd)
	public:
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExtReportGridWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	// sort order
	virtual LONG ReportSortOrderMaxGroupColumnCountGet() const;
	virtual void ReportSortOrderMaxGroupColumnCountSet(
		LONG nMaxColCount,
//		bool bAdjustSortOrderIfExceeded = true,
		bool bRedrawIfAdjusted = true
		);
	virtual void OnReportMaxGroupColumnCountReached();
protected:
	LONG m_nMaxColCountForGrouping;
public:
	virtual CExtReportGridSortOrder & ReportSortOrderGet();
	const CExtReportGridSortOrder & ReportSortOrderGet() const;
	virtual bool ReportSortOrderSet(
		const CExtReportGridSortOrder & _rgso,
		bool bShowMsgBoxesIfFailed,
		bool bRedraw = true
		);
	virtual bool ReportSortOrderUpdate(
		bool bRedraw = true
		);
	virtual bool ReportSortOrderVerify(
		const CExtReportGridSortOrder & _rgso,
		bool bShowMsgBoxesIfFailed
		) const;
	// category combo
	virtual CExtReportGridCategoryComboBox * ReportCategoryComboBoxGet();
	const CExtReportGridCategoryComboBox * ReportCategoryComboBoxGet() const;
	virtual bool ReportCategoryComboBoxIsVisible() const;
	virtual void ReportCategoryComboBoxShow(
		bool bShow = true
		);
	// column chooser
	virtual CExtReportGridColumnChooserWnd * ReportColumnChooserGet();
	const CExtReportGridColumnChooserWnd * ReportColumnChooserGet() const;
	virtual bool ReportColumnChooserIsVisible() const;
	virtual void ReportColumnChooserShow(
		bool bShow = true
		);
	// group area
	virtual CExtReportGridGroupAreaWnd * ReportGroupAreaGet();
	const CExtReportGridGroupAreaWnd * ReportGroupAreaGet() const;
	virtual bool ReportGroupAreaIsVisible() const;
	virtual void ReportGroupAreaShow(
		bool bShow = true
		);
	// columns
	virtual CExtReportGridColumn * ReportColumnRegister(
		__EXT_MFC_SAFE_LPCTSTR strColumnName = NULL,
		__EXT_MFC_SAFE_LPCTSTR strCategoryName = NULL,
		bool bActivate = true,
		bool bRedraw = true,
		CRuntimeClass * pRTC = NULL // should be kind of CExtReportGridColumn
		);
	bool ReportColumnActivate(
		CExtReportGridColumn * pRGC,
		bool bActivate = true,
		LONG nColNo = -1L, // -1L - activate last
		bool bRedraw = true
		);
	virtual bool ReportColumnActivate( // default parameters will activate/deactivate all the columns
		__EXT_MFC_SAFE_LPCTSTR strColumnName = NULL,
		__EXT_MFC_SAFE_LPCTSTR strCategoryName = NULL,
		bool bActivate = true,
		LONG nColNo = -1L, // -1L - activate last
		bool bRedraw = true
		);
	bool ReportColumnIsActive(
		const CExtReportGridColumn * pRGC
		) const;
	virtual bool ReportColumnIsActive(
		__EXT_MFC_SAFE_LPCTSTR strColumnName,
		__EXT_MFC_SAFE_LPCTSTR strCategoryName
		) const;
	bool ReportColumnRename(
		__EXT_MFC_SAFE_LPCTSTR strColumnNameOld,
		__EXT_MFC_SAFE_LPCTSTR strCategoryNameOld,
		__EXT_MFC_SAFE_LPCTSTR strColumnNameNew = NULL,
		__EXT_MFC_SAFE_LPCTSTR strCategoryNameNew = NULL,
		bool bRedraw = true
		);
	const CExtReportGridColumn * ReportColumnGet(
		__EXT_MFC_SAFE_LPCTSTR strColumnName,
		__EXT_MFC_SAFE_LPCTSTR strCategoryName = NULL
		) const;
	virtual CExtReportGridColumn * ReportColumnGet(
		__EXT_MFC_SAFE_LPCTSTR strColumnName,
		__EXT_MFC_SAFE_LPCTSTR strCategoryName = NULL
		);
	virtual LONG ReportColumnGetCount(
		bool bIncludingActiveColumns = true,
		bool bIncludingInactiveColumns = true
		) const;
	virtual POSITION ReportColumnGetStartPosition(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName = __EXT_MFC_SAFE_LPCTSTR(NULL), // NULL - all categories
		bool bIncludeActive = true,
		bool bIncludeInactive = true
		) const;
	virtual CExtReportGridColumn * ReportColumnGetNext(
		POSITION & pos,
		__EXT_MFC_SAFE_LPCTSTR strCategoryName = __EXT_MFC_SAFE_LPCTSTR(NULL), // NULL - all categories
		bool bIncludeActive = true,
		bool bIncludeInactive = true
		);
	const CExtReportGridColumn * ReportColumnGetNext(
		POSITION & pos,
		__EXT_MFC_SAFE_LPCTSTR strCategoryName = __EXT_MFC_SAFE_LPCTSTR(NULL), // NULL - all categories
		bool bIncludeActive = true,
		bool bIncludeInactive = true
		) const;
	void ReportColumnGetContent(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName, // NULL - all categories
		CTypedPtrArray < CPtrArray, CExtReportGridColumn * > & arrCategoryColumns,
		bool bIncludeActive = true,
		bool bIncludeInactive = true
		);
	LONG ReportColumnUnRegisterCategory(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName,
		bool bRedraw = true
		);
	LONG ReportColumnUnRegister( // returns number of unregistered columns
		__EXT_MFC_SAFE_LPCTSTR strColumnName = NULL, // both NULL = unregister all
		__EXT_MFC_SAFE_LPCTSTR strCategoryName = NULL,
		bool bRedraw = true
		);
	virtual bool ReportColumnUnRegister(
		CExtReportGridColumn * pRGC,
		bool bRedraw = true
		);
	virtual bool ReportColumnIsRegistered(
		const CExtReportGridColumn * pRGC
		) const;
	void ReportColumnGetAvailableCategoryNames(
		CExtSafeStringArray & arrAvailableCategoryNames
		) const;
protected:
	virtual LONG _ReportColumnAdjustStylesByMask_Impl(
		CExtReportGridColumn * pRGC,
		HTREEITEM hTreeItem,
		DWORD dwStyleToSet,
		DWORD dwStyleMaskToRemove,
		bool bExtendedStyle
		);
public:
	virtual void ReportColumnAdjustStylesByMask(
		CExtReportGridColumn * pRGC,
		DWORD dwStyleToSet,
		DWORD dwStyleMaskToRemove,
		bool bExtendedStyle,
		bool bRedraw = true
		);
	virtual void ReportColumnAdjustIconAlignmentH(
		CExtReportGridColumn * pRGC,
		DWORD dwAlignment,
		bool bRedraw = true
		);
	virtual void ReportColumnAdjustIconAlignmentV(
		CExtReportGridColumn * pRGC,
		DWORD dwAlignment,
		bool bRedraw = true
		);
	virtual void ReportColumnAdjustTextAlignmentH(
		CExtReportGridColumn * pRGC,
		DWORD dwAlignment,
		bool bRedraw = true
		);
	virtual void ReportColumnAdjustTextAlignmentV(
		CExtReportGridColumn * pRGC,
		DWORD dwAlignment,
		bool bRedraw = true
		);
	// report column resizing and scrolling
	bool ReportColumnProportionalResizingGet() const;
	void ReportColumnProportionalResizingSet(
		bool bUseProportionalResizing,
		bool bRedraw = true
		);
	// auto preview mode
	bool ReportAutoPreviewModeGet() const;
	void ReportAutoPreviewModeSet(
		bool bAutoPreviewMode,
		bool bRedraw = true
		);
	// items
	virtual bool ReportItemRegister(
		CTypedPtrArray < CPtrArray, CExtReportGridItem * > & arrRegisteredItems,
		bool bUpdateSortOrder = false,
		bool bRedraw = false
		);
protected:
	virtual bool _ReportItemUnRegisterImpl(
		CExtReportGridItem * pRGI,
		bool bResetFocus,
		bool bRedraw
		);
	virtual LONG _SelectSubtreeForUnRegistering(
		HTREEITEM hTreeItem
		);
public:
	virtual bool ReportItemUnRegister(
		CExtReportGridItem * pRGI,
		bool bResetFocus = true,
		bool bRedraw = true
		);
	virtual LONG ReportItemUnRegisterAll(
		bool bRedraw = true
		);
	virtual bool ReportItemModifySelectionForUnRegistering(
		bool bRedraw = true,
		HTREEITEM * p_htiResetFocus = NULL
		);
	virtual bool ReportItemUnRegisterSelection(
		bool bRedraw = true
		);
	CExtReportGridItem * ReportItemRegister(
		bool bUpdateSortOrder = false,
		bool bRedraw = false
		);
	virtual bool ReportItemIsRegistered(
		const CExtReportGridItem * pRGI
		) const;
	virtual CExtGridCell * ReportItemGetCell(
		const CExtReportGridColumn * pRGC,
		const CExtReportGridItem * pRGI,
		CRuntimeClass * pRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	const CExtGridCell * ReportItemGetCell(
		const CExtReportGridColumn * pRGC,
		const CExtReportGridItem * pRGI,
		CRuntimeClass * pRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	CExtGridCell * ReportItemGetCell(
		__EXT_MFC_SAFE_LPCTSTR strColumnName,
		__EXT_MFC_SAFE_LPCTSTR strCategoryName,
		const CExtReportGridItem * pRGI,
		CRuntimeClass * pRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	const CExtGridCell * ReportItemGetCell(
		__EXT_MFC_SAFE_LPCTSTR strColumnName,
		__EXT_MFC_SAFE_LPCTSTR strCategoryName,
		const CExtReportGridItem * pRGI,
		CRuntimeClass * pRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	virtual bool ReportItemSetCell(
		const CExtReportGridColumn * pRGC,
		const CExtReportGridItem * pRGI,
		const CExtGridCell * pCellNewValue = NULL, // if NULL - empty existing cell values
		bool bReplace = false, // false - assign to existing cell instances or column/row type created cells, true - create new cloned copies of pCellNewValue
		CRuntimeClass * pInitRTC = NULL // runtime class for new cell instance (used if bReplace=false)
		);
	bool ReportItemSetCell(
		__EXT_MFC_SAFE_LPCTSTR strColumnName,
		__EXT_MFC_SAFE_LPCTSTR strCategoryName,
		const CExtReportGridItem * pRGI,
		const CExtGridCell * pCellNewValue = NULL, // if NULL - empty existing cell values
		bool bReplace = false, // false - assign to existing cell instances or column/row type created cells, true - create new cloned copies of pCellNewValue
		CRuntimeClass * pInitRTC = NULL // runtime class for new cell instance (used if bReplace=false)
		);
	CExtGridCell * ReportGridCellGetOuterAtTop(
		const CExtReportGridColumn * pRGC,
		LONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	const CExtGridCell * ReportGridCellGetOuterAtTop(
		const CExtReportGridColumn * pRGC,
		LONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	CExtGridCell * ReportGridCellGetOuterAtBottom(
		const CExtReportGridColumn * pRGC,
		LONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	const CExtGridCell * ReportGridCellGetOuterAtBottom(
		const CExtReportGridColumn * pRGC,
		LONG nRowNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	CExtGridCell * ReportGridCellGetOuterAtLeft(
		LONG nColNo,
		const CExtReportGridItem * pRGI,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	const CExtGridCell * ReportGridCellGetOuterAtLeft(
		LONG nColNo,
		const CExtReportGridItem * pRGI,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	CExtGridCell * ReportGridCellGetOuterAtRight(
		LONG nColNo,
		const CExtReportGridItem * pRGI,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		);
	const CExtGridCell * ReportGridCellGetOuterAtRight(
		LONG nColNo,
		const CExtReportGridItem * pRGI,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true, // auto find row/column default value (only when pInitRTC=NULL)
		bool bUseColumnDefaultValue = true // false - use row default value (only when bAutoFindValue=true)
		) const;
	bool ReportGridCellSetOuterAtTop(
		const CExtReportGridColumn * pRGC,
		LONG nRowNo,
		const CExtGridCell * pCell = NULL, // NULL - clear (make empty)
		LONG nColCount = 1L,
		LONG nRowCount = 1L,
		bool bRedraw = true
		);
	bool ReportGridCellSetOuterAtBottom(
		const CExtReportGridColumn * pRGC,
		LONG nRowNo,
		const CExtGridCell * pCell = NULL, // NULL - clear (make empty)
		LONG nColCount = 1L,
		LONG nRowCount = 1L,
		bool bRedraw = true
		);
	bool ReportGridCellSetOuterAtLeft(
		LONG nColNo,
		const CExtReportGridItem * pRGI,
		const CExtGridCell * pCell = NULL, // NULL - clear (make empty)
		LONG nColCount = 1L,
		LONG nRowCount = 1L,
		bool bRedraw = true
		);
	bool ReportGridCellSetOuterAtRight(
		LONG nColNo,
		const CExtReportGridItem * pRGI,
		const CExtGridCell * pCell = NULL, // NULL - clear (make empty)
		LONG nColCount = 1L,
		LONG nRowCount = 1L,
		bool bRedraw = true
		);
	virtual CExtReportGridItem * ReportItemFocusGet();
	const CExtReportGridItem * ReportItemFocusGet() const;
	virtual CExtReportGridItem * ReportItemFromTreeItem(
		HTREEITEM hTreeItem
		);
	const CExtReportGridItem * ReportItemFromTreeItem(
		HTREEITEM hTreeItem
		) const;

protected:
	CExtSafeMapStringToPtr m_mapColumnCategories;
	CMapPtrToPtr m_mapColumns;
	bool m_bCreateHelperInvoked:1;
	virtual bool _CreateHelper();
	CExtReportGridCategoryComboBox * m_pCAB;
	CExtReportGridColumnChooserWnd * m_pCCW;
	CExtReportGridGroupAreaWnd * m_pGAW;
public:
	CExtScrollBar m_wndScrollBarH, m_wndScrollBarV;
	virtual LONG OnGridHookOuterCellDND(
		bool bFinal,
		LONG nExternalHT,
		const POINT & point,
		CExtGridCell * pCell,
		const CExtGridHitTestInfo & htInfo,
		CExtContentExpandWnd & wndArrows,
		CExtContentExpandWnd & wndDND
		);
	virtual void OnGridOuterDragOut(
		const CExtGridHitTestInfo & htInfo
		);
	virtual bool OnReportGridQueryColumnKeepActiveOnGroupingBehavior(
		__EXT_MFC_SAFE_LPCTSTR strColumnName,
		__EXT_MFC_SAFE_LPCTSTR strCategoryName
		) const;
	virtual void OnReportGridColumnChooserAdjustLocation();
	virtual CExtReportGridColumnChooserWnd * OnReportGridCreateColumnChooser();
	virtual CExtReportGridCategoryComboBox * OnReportGridCreateCategoryComboBox();
	virtual CExtReportGridGroupAreaWnd * OnReportGridCreateGroupArea();
	virtual void OnReportGridColumnCategoryAdded(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName
		);
	virtual void OnReportGridColumnCategoryRemoved(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName
		);
	virtual LONG ColumnCountGet() const;
	virtual LONG RowCountGet() const;
protected:
	virtual bool GridSortOrderSetup(
		bool bSortColumns,
		const CExtGridDataSortOrder & _gdsoUpdate,
		bool bUpdateExistingSortOrder = false,
		bool bInvertIntersectionSortOrder = true,
		bool bRedraw = true
		);
	virtual bool OnTreeGridQueryDrawOutline(
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
public:
	virtual bool OnTreeGridQueryKbCollapseEnabled(
		HTREEITEM hTreeItem,
		LONG nColNo
		);
	virtual bool OnTreeGridQueryKbExpandEnabled(
		HTREEITEM hTreeItem,
		LONG nColNo
		);
	virtual bool OnTreeGridQueryKbJumpParentEnabled(
		HTREEITEM hTreeItem,
		LONG nColNo
		);
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
	virtual bool SelectionGetForCellPainting(
		LONG nColNo,
		LONG nRowNo
		) const;
	virtual CRect & _SelectionAreaConvert( CRect & rcArea ) const;
	virtual bool SelectionGetForCell(
		LONG nColNo,
		LONG nRowNo
		) const;
	virtual bool OnTreeGridQueryColumnOutline(
		LONG nColNo
		) const;
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
	virtual void OnGbwAdjustRects(
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		RECT & rcCellExtraA,
		RECT & rcCellA
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
	virtual void OnGridCalcOuterDropTarget(
		const CExtGridHitTestInfo & htInfoDrag,
		CExtGridHitTestInfo & htInfoDrop,
		CPoint point
		);
	virtual void OnGbwResizingStateUpdate(
		bool bInitial,
		bool bFinal,
		const CPoint * p_ptClient = NULL
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
	virtual void OnGridCellInputComplete(
		CExtGridCell & _cell,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		HWND hWndInputControl = NULL
		);
	virtual void OnSiwDrawFocusRect(
		CDC &dc,
		LPCRECT pRect,
		CObject * pObjSrc = NULL,
		LPARAM lParam = 0L
		) const;
	virtual void OnReportGridPaintGroupRow(
		CDC & dc,
		const CExtReportGridItem * pRGI,
		LONG nVisibleRowNo,
		LONG nRowNo,
		const RECT & rcRowExtra,
		const RECT & rcRow,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const;
	virtual void OnReportGridFormatGroupRowText(
		const CExtReportGridItem * pRGI,
		CExtSafeString & strRowText,
		LONG nGroupIndex
		) const;
	virtual void OnTreeGridPaintExpandButton(
		CDC & dc,
		HTREEITEM hTreeItem,
		bool bExpanded,
		const CRect & rcIndent
		) const;
	virtual void OnReportGridFormatSortByToolTipText(
		CExtSafeString & str,
		const CExtReportGridColumn * pRGC
		) const;
	virtual void OnReportGridColumnChooserNoFieldsAvailableLabel( CExtSafeString & str ) const;
	virtual void OnReportGridColumnChooserQueryCaption( CExtSafeString & str ) const;
	virtual void OnReportGridGroupAreaQueryEmptyText( CExtSafeString & str ) const;
	virtual void OnReportGridEmptyDataMessage( CExtSafeString & str ) const;
	virtual void OnReportGridSortOrderVerificationFailed(
		const CExtReportGridSortOrder & _rgso,
		LONG nFailureIndex,
		bool bGroupingFailed
		);
	virtual void OnSiwPaintForeground(
		CDC & dc,
		bool bFocusedControl
		) const;
	virtual void OnReportGridReportSortOrderUpdateComplete();
protected:
	mutable CExtReportGridItem * m_pProtectReportGridQueryItemExtraSpaceRGI;
public:
	virtual INT OnReportGridQueryItemExtraSpace(
		CExtReportGridItem * pRGI,
		bool bAfter
		) const;
	virtual bool OnTreeQueryPlainGridModeVeticalLayout() const;
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
	virtual void OnSwUpdateScrollBars();
	virtual bool EnsureVisibleColumn(
		LONG nColNo,
		bool bRedraw = true
		);
	virtual bool EnsureVisibleRow(
		LONG nRowNo,
		bool bRedraw = true
		);
	virtual void OnGbwEraseArea(
		CDC & dc,
		const RECT & rcArea,
		DWORD dwAreaFlags
		) const;
	virtual bool OnReportGridPaintHeaderRowBackground(
		CDC & dc,
		const RECT & rcArea,
		CObject * pHelperSrc = NULL,
		LPARAM lParam = 0L
		) const;
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
	virtual bool OnGridCellGetToolTipText(
		const CExtGridCell & _cell,
		const CExtGridHitTestInfo & htInfo,
		CExtSafeString & strToolTipText
		) const;
	virtual bool OnGbwAnalyzeCellMouseClickEvent(
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nRepCnt, // 0 - button up, 1 - single click, 2 - double click, 3 - post single click & begin editing
		UINT nFlags, // mouse event flags
		CPoint point // mouse pointer in client coordinates
		);
	virtual void OnGridTrackOuterCellDND(
		CExtGridCell * pCell,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nFlags // mouse event flags
		);
	// auto preview area overrides
	virtual void OnReportGridAutoPreviewAreaPaint(
		CDC & dc,
		const RECT & rcAutoPreview,
		const CExtReportGridItem * pRGI,
		const CExtGridCell * pCellPreviewArea
		) const;
	virtual COLORREF OnReportGridAutoPreviewAreaQueryTextColor(
		const CExtReportGridItem * pRGI,
		const CExtGridCell * pCellPreviewArea
		) const;
	// local command manager's profile management
protected:
	CExtSafeString m_strLocalCommandProfileName;
public:
	virtual __EXT_MFC_SAFE_LPCTSTR OnReportGridCommandProfileQueryName() const;
	virtual void OnReportGridCommandProfileInit();
	virtual void OnReportGridCommandProfileShutdown();
	// column context menu overrides
protected:
	CExtReportGridColumn * m_pCtxMenuRGC;
public:
	virtual CExtReportGridColumn * OnReportGridColumnCtxMenuGetCtxColumn();
	virtual bool OnReportGridColumnCtxMenuTrack(
		CWnd * pWndSrc,
		CExtReportGridColumn * pRGC,
		const POINT & ptScreen,
		const RECT & rcItemScreen,
		const RECT & rcItemExtraScreen,
		DWORD dwMenuTrackingFlags = 0
		);
	virtual bool OnReportGridColumnCtxMenuConstruct(
		CExtPopupMenuWnd * pPopup,
		CWnd * pWndSrc,
		CExtReportGridColumn * pRGC,
		DWORD & dwMenuTrackingFlags
		);
protected:
	static bool stat_cbItemClickForColumnVisibility(
		CExtPopupMenuWnd * pPopup,
		LPVOID pItemData
		);
protected:
	// column context menu command handlers
	void _OnRgColumnCtxCmdSortAscending();
	void _OnRgUpdateColumnCtxCmdSortAscending( CCmdUI * pCmdUI );
	void _OnRgColumnCtxCmdSortDescending();
	void _OnRgUpdateColumnCtxCmdSortDescending( CCmdUI * pCmdUI );
	void _OnRgColumnCtxCmdGroupByContextField();
	void _OnRgUpdateColumnCtxCmdGroupByContextField( CCmdUI * pCmdUI );
	void _OnRgColumnCtxCmdShowHideGroupByBox();
	void _OnRgUpdateColumnCtxCmdShowHideGroupByBox( CCmdUI * pCmdUI );
	void _OnRgColumnCtxCmdRemoveColumn();
	void _OnRgUpdateColumnCtxCmdRemoveColumn( CCmdUI * pCmdUI );
	void _OnRgColumnCtxCmdShowHideFieldChooser();
	void _OnRgUpdateColumnCtxCmdShowHideFieldChooser( CCmdUI * pCmdUI );
	void _OnRgColumnCtxCmdBestFitContextField();
	void _OnRgUpdateColumnCtxCmdBestFitContextField( CCmdUI * pCmdUI );
	void _OnRgColumnCtxCmdColumnAlignmentLeft();
	void _OnRgUpdateColumnCtxCmdColumnAlignmentLeft( CCmdUI * pCmdUI );
	void _OnRgColumnCtxCmdColumnAlignmentCenter();
	void _OnRgUpdateColumnCtxCmdColumnAlignmentCenter( CCmdUI * pCmdUI );
	void _OnRgColumnCtxCmdColumnAlignmentRight();
	void _OnRgUpdateColumnCtxCmdColumnAlignmentRight( CCmdUI * pCmdUI );
	void _OnRgColumnCtxCmdColumnAlignmentByType();
	void _OnRgUpdateColumnCtxCmdColumnAlignmentByType( CCmdUI * pCmdUI );

public:
	virtual bool OnGridQueryCenteredDndAlignment() const;

	// state serialization
	virtual bool ReportGridStateSerialize(
		CArchive & ar,
		bool bSerializeColumnAlignment = true,
		bool bSerializeSortOrder = true,
		bool bEnableThrowExceptions = false,
		bool * p_bFormatIncompatibilityDuringLoad = NULL
		);
	bool ReportGridStateLoad(
		__EXT_MFC_SAFE_LPCTSTR sReportGridID,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%
		HKEY hKeyRoot = HKEY_CURRENT_USER,
		bool bSerializeColumnAlignment = true,
		bool bSerializeSortOrder = true,
		bool bEnableThrowExceptions = false,
		bool * p_bFormatIncompatibilityDuringLoad = NULL
		);
	bool ReportGridStateSave(
		__EXT_MFC_SAFE_LPCTSTR sReportGridID,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%
		HKEY hKeyRoot = HKEY_CURRENT_USER,
		bool bSerializeColumnAlignment = true,
		bool bSerializeSortOrder = true,
		bool bEnableThrowExceptions = false,
		bool * p_bFormatIncompatibilityDuringLoad = NULL
		);

	virtual void OnReportGridItemInitializeOuterCells(
		CExtReportGridItem * pRGI
		);

	virtual bool BestFitRow(
		LONG nRowNo,
		INT nRowType = 0,
		bool bDoBestFitByOuterCells = true,
		bool bDoBestFitByInnerCells = true,
		bool bDoBestFitByVisibleColumnRangeOnly = false,
		bool bRedraw = true
		);

	virtual bool ColumnHide(
		LONG nColNo,
		LONG nHideCount = 1,
		bool bRedraw = true
		);
	virtual bool ColumnUnHideAll(
		bool bRedraw = true
		);

protected:
	virtual void OnAdjustTreeRowFilteredState(
		HTREEITEM hti,
		CList < LONG, LONG & > & _listIndices,
		LONG nScanRowNo,
		bool bTop,
		INT nIndentLevel,
		bool bLeavesOnly
		);
public:
	virtual bool FilterStateForOuterRowSerialize(
		CArchive & ar,
		bool bTop = true,
		LONG nRowNo = 0L,
		bool bRedrawAfterLoading = true,
		bool bEnableThrowExcetions = false
		);

	struct __PROF_UIS_API StringPair_t
	{
		CExtSafeString m_strLeft, m_strRight;
		void AssignFromOther( const StringPair_t & other );
		int CompareWithOther( const StringPair_t & other ) const;
		StringPair_t()
		{
		}
		StringPair_t( const StringPair_t & other )
		{
			AssignFromOther( other );
		}
		StringPair_t & operator = ( const StringPair_t & other )
		{
			AssignFromOther( other );
			return (*this);
		}
		bool operator == ( const StringPair_t & other ) const
		{
			return ( CompareWithOther( other ) == 0 ) ? true : false;
		}
		bool operator != ( const StringPair_t & other ) const
		{
			return ( CompareWithOther( other ) != 0 ) ? true : false;
		}
		bool operator <= ( const StringPair_t & other ) const
		{
			return ( CompareWithOther( other ) <= 0 ) ? true : false;
		}
		bool operator >= ( const StringPair_t & other ) const
		{
			return ( CompareWithOther( other ) >= 0 ) ? true : false;
		}
		bool operator <  ( const StringPair_t & other ) const
		{
			return ( CompareWithOther( other ) <  0 ) ? true : false;
		}
		bool operator >  ( const StringPair_t & other ) const
		{
			return ( CompareWithOther( other ) >  0 ) ? true : false;
		}
	}; // struct StringPair_t

	virtual bool ReportFilterStateSerialize(
		CArchive & ar,
		bool bRedrawAfterLoading = true,
		bool bEnableThrowExceptions = false
		);
	bool ReportFilterStateLoad(
		__EXT_MFC_SAFE_LPCTSTR sGridID,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%
		HKEY hKeyRoot = HKEY_CURRENT_USER,
		bool bRedrawAfterLoading = true,
		bool bEnableThrowExceptions = false
		);
	bool ReportFilterStateSave(
		__EXT_MFC_SAFE_LPCTSTR sGridID,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%
		HKEY hKeyRoot = HKEY_CURRENT_USER,
		bool bEnableThrowExceptions = false
		);

	virtual bool OnGridFilterSortCommand(
		LONG nRowColNo,
		bool bSortColumns,
		bool bAscending,
		bool bUpdateExistingSortOrder
		);

	virtual void OnTreeGridFilterCalcPlainUniqueValuesForColumn(
		HTREEITEM hti,
		CExtArray < CExtSafeString > & arrUniqueStringValues,
		LONG nColNo,
		bool bLeavesOnly,
		bool bIncludeHidden = true
		) const;

	virtual bool OnGridFilterQueryFilteredState(
		bool bFilteringRows,
		LONG nRowColNo = -1, // if -1 - ignore nRowColNo, check all ranges at both outer sides and fill p_nRowColNoFound & p_bTopLeftFound
		bool bTopLeft = true,
		LONG * p_nRowColNoFound = NULL,
		bool * p_bTopLeftFound = NULL
		);

	virtual void FilterStateClearAll(
		bool bClearAtTop = true,
		bool bClearAtBottom = true,
		bool bClearAtLeft = true,
		bool bClearAtRight = true
		);

	friend class CExtReportGridColumnChooserWnd;
}; // class CExtReportGridWnd

#endif // (!defined __EXT_MFC_NO_REPORTGRIDWND)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__EXT_REPORT_GRID_WND_H)


