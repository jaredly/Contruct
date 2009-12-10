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

#if (!defined __EXTTREEGRIDWND_H)
#define __EXTTREEGRIDWND_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if (!defined __EXT_MFC_NO_TREEGRIDWND)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_GRIDWND_H)
	#include <ExtGridWnd.h>
#endif // __EXT_GRIDWND_H

class CExtTreeGridCellNode;
class CExtTreeGridDataProvider;
class CExtTreeGridWnd;

#if (!defined __EXT_MFC_NO_REPORTGRIDWND)
	class CExtReportGridDataProvider;
#endif // (!defined __EXT_MFC_NO_REPORTGRIDWND)

#define __EXT_MGC_TREE_GRID_DEFAULT_LEVEL_INDENT 19

/////////////////////////////////////////////////////////////////////////////
// CExtTreeGridCellNode

class __PROF_UIS_API CExtTreeGridCellNode : public CExtGCE < CExtGridCell >
{
	ULONG m_nContentWeightAll, m_nContentWeightExpanded;
public:
	DECLARE_DYNCREATE( CExtTreeGridCellNode );
	IMPLEMENT_ExtGridCell_Clone( CExtTreeGridCellNode, CExtGCE < CExtGridCell > );
	CExtTreeGridCellNode(
		CExtGridDataProvider * pDataProvider = NULL
		);
	virtual ~CExtTreeGridCellNode();
//#ifdef _DEBUG
//	CExtTreeGridCellNode( CExtGridCell & other )
//	{
//		other;
//		// this constructor must never be invoked
//		ASSERT( FALSE );
//	}
//	virtual void Serialize( CArchive & ar )
//	{
//		ar;
//		// this method must never be invoked
//		ASSERT( FALSE );
//	}
//#endif // _DEBUG
	typedef
		CTypedPtrArray < CPtrArray, CExtTreeGridCellNode * >
		NodeArr_t;
	NodeArr_t m_arrChildren;
protected:
	CExtTreeGridCellNode * m_pNodeParent, * m_pNodeNext, * m_pNodePrev;
	ULONG m_nIndentPx, m_nOptIndex;
public:
	operator HTREEITEM () const;
	static CExtTreeGridCellNode * FromHTREEITEM(
		HTREEITEM hTreeItem
		);
	CExtTreeGridCellNode * TreeNodeGetRoot();
	const CExtTreeGridCellNode * TreeNodeGetRoot() const;
	CExtTreeGridCellNode * TreeNodeGetParent();
	const CExtTreeGridCellNode * TreeNodeGetParent() const;

	CExtTreeGridCellNode * _TreeNodeBrowseNextImpl(
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bWalkDeeper,
		bool bIncludeHidden
		);
	const CExtTreeGridCellNode * _TreeNodeBrowseNextImpl(
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bWalkDeeper,
		bool bIncludeHidden
		) const;
	CExtTreeGridCellNode * _TreeNodeBrowsePrevImpl(
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bWalkDeeper,
		bool bIncludeHidden
		);
	const CExtTreeGridCellNode * _TreeNodeBrowsePrevImpl(
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bWalkDeeper,
		bool bIncludeHidden
		) const;

	CExtTreeGridCellNode * TreeNodeGetNext(
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bIncludeHidden
		);
	const CExtTreeGridCellNode * TreeNodeGetNext(
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bIncludeHidden
		) const;
	CExtTreeGridCellNode * TreeNodeGetPrev(
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bIncludeHidden
		);
	const CExtTreeGridCellNode * TreeNodeGetPrev(
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bIncludeHidden
		) const;

	ULONG TreeNodeGetSiblingIndex() const;
	ULONG TreeNodeGetChildCount() const;
	CExtTreeGridCellNode * TreeNodeGetChildAt( ULONG nPos );
	const CExtTreeGridCellNode * TreeNodeGetChildAt( ULONG nPos ) const;
	CExtTreeGridCellNode * TreeNodeGetFirstSibling();
	const CExtTreeGridCellNode * TreeNodeGetFirstSibling() const;
	CExtTreeGridCellNode * TreeNodeGetLastSibling();
	const CExtTreeGridCellNode * TreeNodeGetLastSibling() const;
	CExtTreeGridCellNode * TreeNodeGetFirstChild();
	const CExtTreeGridCellNode * TreeNodeGetFirstChild() const;
	CExtTreeGridCellNode * TreeNodeGetLastChild();
	const CExtTreeGridCellNode * TreeNodeGetLastChild() const;
	virtual ULONG TreeNodeIndentCompute() const;
	virtual ULONG TreeNodeIndentPxGet() const;
	virtual void TreeNodeIndentPxSet( ULONG nIndentPx );
	enum e_expand_box_shape_t
	{
		__EEBS_NONE = 0,
		__EEBS_EXPANDED = 1,
		__EEBS_COLLAPSED = 2,
	};
	virtual e_expand_box_shape_t TreeNodeGetExpandBoxShape() const;
	virtual bool TreeNodeHiddenGet() const;
	virtual void TreeNodeHiddenSet(
		CExtTreeGridDataProvider & _DP,
		bool bHide
		);
	virtual bool TreeNodeIsDisplayed() const;
	virtual void TreeNodeMarkExpanded(
		bool bExpand
		);
	virtual bool TreeNodeIsExpanded() const;
	virtual ULONG TreeNodeCalcOffset(
		bool bExpandedOnly,
		bool bIncludeHidden = true
		) const;
protected:
	bool m_bExpanded:1;
public:
	ULONG _ContentWeight_Get(
		bool bExpandedOnly
		) const;
	void _ContentWeight_Increment(
		ULONG nWeight,
		bool bExpandedOnly
		);
	void _ContentWeight_Decrement(
		ULONG nWeight,
		bool bExpandedOnly
		);
	void _ContentWeight_IncrementNonExpanded(
		ULONG nWeight
		);
	void _ContentWeight_DecrementNonExpanded(
		ULONG nWeight
		);
	void _ContentWeight_Adjust();
	ULONG _ContentWeight_CalcVisible() const;
	void _Content_FillVisibleArray(
		NodeArr_t & arr,
		ULONG & nOffset
		);
	friend class CExtTreeGridDataProvider;
#if (!defined __EXT_MFC_NO_REPORTGRIDWND)
	friend class CExtReportGridDataProvider;
#endif // (!defined __EXT_MFC_NO_REPORTGRIDWND)
}; // class CExtTreeGridCellNode

/////////////////////////////////////////////////////////////////////////////
// CExtTreeGridDataProvider

class __PROF_UIS_API CExtTreeGridDataProvider : public CExtGridDataProvider
{
public:
	struct __PROF_UIS_API ITreeDataProviderEvents
	{
		virtual void OnTreeDataProviderSortEnter() = 0;
		virtual void OnTreeDataProviderSortLeave() = 0;
		virtual void OnTreeDataProviderSwapSeries(
			HTREEITEM hTreeItem1,
			HTREEITEM hTreeItem2,
			LONG nSwapCounter
			) = 0;
	}; // struct ITreeDataProviderEvents
protected:
	CExtGridDataProviderMemory m_DP;
	CExtTreeGridCellNode * m_pCellRoot;
	CExtTreeGridCellNode::NodeArr_t m_arrGridRef;
	CExtTreeGridCellNode::NodeArr_t m_arrGridVis;
protected:
	virtual CExtTreeGridCellNode * _Tree_NodeGetRoot();
	virtual CExtTreeGridCellNode * _Tree_NodeGetByVisibleRowIndex( ULONG nRowNo );
	virtual ULONG _Tree_GetRowIndentPx( ULONG nRowNo );
	virtual CExtTreeGridCellNode * _Tree_NodeInsert(
		CExtTreeGridCellNode * pNodeParent = NULL, // if NULL - root
		ULONG nIdxInsertBefore = (ULONG(-1L)), // if (ULONG(-1L)) - insert to end
		ULONG nInsertCount = 1
		); // returns pointer to first inserted
	virtual bool _Tree_NodeCopyMoveTest(
		CExtTreeGridCellNode * pNodeCopyMove,
		CExtTreeGridCellNode * pNodeNewParent, // if NULL - root
		ULONG nIdxInsertBefore = (ULONG(-1L)), // if (ULONG(-1L)) - insert to end
		bool bEnableCopyingIntoInnerBranch = true,
		bool * p_bCopyingIntoInnerBranch = NULL
		);
	virtual CExtTreeGridCellNode * _Tree_NodeCopyMove(
		CExtTreeGridCellNode * pNodeCopyMove,
		CExtTreeGridCellNode * pNodeNewParent, // if NULL - root
		ULONG nIdxInsertBefore = (ULONG(-1L)), // if (ULONG(-1L)) - insert to end
		bool bMove = true,
		bool bCloneOnMove = true,
		bool bCloneCellObjects = true,
		INT nExpandAction = TVE_TOGGLE, // TVE_TOGGLE in this case - keep expanded state
		bool bEnableCopyingIntoInnerBranch = true,
		bool * p_bCopyingIntoInnerBranch = NULL
		);
	virtual bool _Tree_NodeMoveImpl(
		CExtTreeGridCellNode * pNodeCopyMove,
		CExtTreeGridCellNode * pNodeNewParent,
		ULONG nIdxInsertBefore,
		INT nExpandAction
		);
	virtual CExtTreeGridCellNode * _Tree_NodeCopyImpl(
		CExtTreeGridCellNode * pNodeCopyMove,
		CExtTreeGridCellNode * pNodeNewParent,
		CExtTreeGridCellNode * pNodeSkip,
		ULONG nIdxInsertBefore,
		INT nExpandAction,
		bool bCloneCellObjects
		);
	virtual bool _Tree_SortOrderCheck(
		const CExtGridDataSortOrder & _gdso
		);
	virtual bool  _Tree_NodeSortChildren(
		CExtTreeGridCellNode * pNode,
		const CExtGridDataSortOrder & _gdso,
		CExtTreeGridDataProvider::ITreeDataProviderEvents * pDPE
		);
	virtual bool _Tree_SortStep(
		CExtTreeGridCellNode * pNodeParent,
		CExtTreeGridDataProvider::ITreeDataProviderEvents * pDPE,
		const CExtGridDataSortOrder & _gdso,
		LONG & nSwapCounter,
		LONG nLow,
		LONG nHigh
		);
	virtual int _Tree_SortCmpCells(
		CExtGridCell * pCell_1,
		CExtGridCell * pCell_2
		);
	virtual int _Tree_SortCmpSeries(
		LONG nRowNo1,
		LONG nRowNo2,
		CExtTreeGridCellNode * pNodeParent,
		const CExtGridDataSortOrder & _gdso
		);
	virtual bool _Tree_SortSwapSeries(
		LONG nRowNo1,
		LONG nRowNo2,
		LONG & nSwapCounter,
		CExtTreeGridDataProvider::ITreeDataProviderEvents * pDPE,
		CExtTreeGridCellNode * pNodeParent
		);
	virtual ULONG _Tree_NodeRemove(
		CExtTreeGridCellNode * pNode,
		bool bChildrenOnly = false
		); // returns count of removed items
	virtual bool _Tree_NodeExpand(
		CExtTreeGridCellNode * pNode,
		INT nActionTVE = TVE_TOGGLE // TVE_COLLAPSE, TVE_EXPAND, TVE_TOGGLE only
		);
	virtual ULONG _Tree_GetDisplayedCount() const;
	virtual ULONG _Tree_MapColToCache( ULONG nColNo ) const;
	virtual ULONG _Tree_MapRowToCache( ULONG nRowNo ) const;
private:
	ULONG m_nReservedColCount, m_nReservedRowCount;
protected:
	virtual ULONG _Tree_ReservedColumnsGet() const;
	virtual bool _Tree_ReservedColumnsSet( ULONG nReservedCount );
	virtual bool _Tree_ReservedColumnsMoveToReserve(
		ULONG nSrcIdx,
		ULONG nSrcCount,
		ULONG nDstIdx
		);
	virtual bool _Tree_ReservedColumnsMoveFromReserve(
		ULONG nSrcIdx,
		ULONG nSrcCount,
		ULONG nDstIdx
		);
	virtual ULONG _Tree_ReservedRowsGet() const;
	virtual bool _Tree_ReservedRowsSet( ULONG nReservedCount );
public:
	virtual CExtGridDataProvider & _Tree_GetCacheDP();
	const CExtGridDataProvider & _Tree_GetCacheDP() const;
	CRuntimeClass * m_pTreeNodeDefaultRTC;
	ULONG m_nIndentPxDefault;
	CExtTreeGridWnd * m_pTreeGridWnd;
protected:
	virtual CRuntimeClass * _Tree_NodeGetRTC();
	virtual void _Tree_NodeAdjustProps(
		CExtTreeGridCellNode * pNode
		);
public:
	DECLARE_DYNCREATE( CExtTreeGridDataProvider );
	CExtTreeGridDataProvider();
	virtual ~CExtTreeGridDataProvider();
	CExtTreeGridCellNode * TreeNodeGetRoot();
	const CExtTreeGridCellNode * TreeNodeGetRoot() const;
	CExtTreeGridCellNode * TreeNodeGetByVisibleRowIndex( ULONG nRowNo );
	const CExtTreeGridCellNode * TreeNodeGetByVisibleRowIndex( ULONG nRowNo ) const;
	ULONG TreeGetDisplayedCount() const;
	ULONG TreeGetRowIndentPx( ULONG nRowNo ) const;
	CExtTreeGridCellNode * TreeNodeInsert(
		CExtTreeGridCellNode * pNodeParent = NULL, // if NULL - root
		ULONG nIdxInsertBefore = (ULONG(-1L)), // if (ULONG(-1L)) - insert to end
		ULONG nInsertCount = 1
		); // returns pointer to first inserted
	bool TreeNodeCopyMoveTest(
		const CExtTreeGridCellNode * pNodeCopyMove,
		const CExtTreeGridCellNode * pNodeNewParent, // if NULL - root
		ULONG nIdxInsertBefore = (ULONG(-1L)), // if (ULONG(-1L)) - insert to end
		bool bEnableCopyingIntoInnerBranch = true,
		bool * p_bCopyingIntoInnerBranch = NULL
		) const;
	CExtTreeGridCellNode * TreeNodeCopyMove(
		CExtTreeGridCellNode * pNodeCopyMove,
		CExtTreeGridCellNode * pNodeParent, // if NULL - root
		ULONG nIdxInsertBefore = (ULONG(-1L)), // if (ULONG(-1L)) - insert to end
		bool bMove = true,
		bool bCloneCellObjects = true,
		INT nExpandAction = TVE_TOGGLE, // TVE_TOGGLE in this case - keep expanded state
		bool bEnableCopyingIntoInnerBranch = true,
		bool * p_bCopyingIntoInnerBranch = NULL
		);
	bool  TreeNodeSortChildren(
		CExtTreeGridCellNode * pNode,
		const CExtGridDataSortOrder & _gdso,
		CExtTreeGridDataProvider::ITreeDataProviderEvents * pDPE
		);
	ULONG TreeNodeRemove(
		CExtTreeGridCellNode * pNode,
		bool bChildrenOnly = false
		); // returns count of removed items
	bool TreeNodeExpand(
		CExtTreeGridCellNode * pNode,
		INT nActionTVE = TVE_TOGGLE // TVE_COLLAPSE, TVE_EXPAND, TVE_TOGGLE only
		);
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
	virtual CExtGridCell * ExtractGridCell(
		CExtTreeGridCellNode * pNode,
		ULONG nColNo,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true,
		bool bUseColumnDefaultValue = true
		);
	virtual CExtGridCell * ExtractGridCellWithReplacing(
		CExtTreeGridCellNode * pNode,
		ULONG nColNo,
		const CExtGridCell * pCellNewValue = NULL, // if NULL - empty existing cell values
		bool bReplace = false, // false - assign to existing cell instances or column/row type created cells, true - create new cloned copies of pCellNewValue
		CRuntimeClass * pInitRTC = NULL // runtime class for new cell instance (used if bReplace=false)
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
	virtual void OnNodeRemoved(
		CExtTreeGridCellNode * pNode
		);

	friend class CExtTreeGridCellNode;
}; // class CExtTreeGridDataProvider

/////////////////////////////////////////////////////////////////////////////
// CExtTreeGridWnd

class __PROF_UIS_API CExtTreeGridWnd
	: public CExtGridWnd
	, public CExtTreeGridDataProvider::ITreeDataProviderEvents
{
public:
	DECLARE_DYNCREATE( CExtTreeGridWnd );
	CExtTreeGridWnd();
	virtual ~CExtTreeGridWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // _DEBUG

public:
	virtual CExtGridDataProvider & OnGridQueryDataProvider();
	CExtTreeGridDataProvider & GetTreeData();
	const CExtTreeGridDataProvider & GetTreeData() const;
private:
	CExtTreeGridDataProvider & _GetTreeData() const;
public:
	virtual HTREEITEM ItemGetRoot() const;
	virtual HTREEITEM ItemGetByVisibleRowIndex( LONG nRowNo ) const;
	virtual HTREEITEM ItemGetParent( HTREEITEM hTreeItem ) const;
	virtual HTREEITEM _ItemBrowseNextImpl(
		HTREEITEM hTreeItem,
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bWalkDeeper,
		bool bIncludeHidden
		) const;
	virtual HTREEITEM _ItemBrowsePrevImpl(
		HTREEITEM hTreeItem,
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bWalkDeeper,
		bool bIncludeHidden
		) const;
	virtual HTREEITEM ItemGetNext(
		HTREEITEM hTreeItem,
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bIncludeHidden
		) const;
	virtual HTREEITEM ItemGetPrev(
		HTREEITEM hTreeItem,
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bIncludeHidden
		) const;
	virtual HTREEITEM ItemJumpNext(
		HTREEITEM hTreeItem,
		LONG nJump,
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bIncludeHidden
		) const;
	virtual HTREEITEM ItemJumpPrev(
		HTREEITEM hTreeItem,
		LONG nJump,
		bool bSiblingOnly,
		bool bExpandedWalk,
		bool bIncludeHidden
		) const;
	virtual CSize ItemCellJoinGet(
		HTREEITEM hti,
		LONG nColNo,
		INT nColType = 0
		) const;
	virtual bool ItemCellJoinTest(
		CSize sizeJoin,
		HTREEITEM hti,
		LONG nColNo,
		INT nColType = 0,
		bool bCheckCellsPresentInJoinedArea = false,
		bool bCheckFrozenAreaIntersection = true
		) const;
	virtual bool ItemCellJoinSet(
		CSize sizeJoin,
		HTREEITEM hti,
		LONG nColNo,
		INT nColType = 0,
		bool bValidateJoin = false,
		bool bCheckCellsPresentInJoinedArea = false,
		bool bCheckFrozenAreaIntersection = true
		);
	virtual LONG ItemGetChildCount( HTREEITEM hTreeItem ) const;
	virtual HTREEITEM ItemGetChildAt( HTREEITEM hTreeItem, LONG nPos ) const;
	virtual HTREEITEM ItemGetFirstSibling( HTREEITEM hTreeItem ) const;
	virtual HTREEITEM ItemGetLastSibling( HTREEITEM hTreeItem ) const;
	virtual HTREEITEM ItemGetFirstChild( HTREEITEM hTreeItem ) const;
	virtual HTREEITEM ItemGetLastChild( HTREEITEM hTreeItem ) const;
	virtual HTREEITEM ItemInsert(
		HTREEITEM hTreeItemParent, // if NULL - root
		ULONG nIdxInsertBefore = (ULONG(-1L)), // if (ULONG(-1L)) - insert to end
		ULONG nInsertCount = 1,
		bool bRedraw = true
		); // returns handle of first inserted
	virtual bool ItemCopyMoveTest(
		HTREEITEM hTreeItemCopyMove,
		HTREEITEM hTreeItemNewParent, // if NULL - root
		ULONG nIdxInsertBefore = (ULONG(-1L)), // if (ULONG(-1L)) - insert to end
		bool bEnableCopyingIntoInnerBranch = true,
		bool * p_bCopyingIntoInnerBranch = NULL
		) const;
	virtual HTREEITEM ItemCopyMove(
		HTREEITEM hTreeItemCopyMove,
		HTREEITEM hTreeItemNewParent, // if NULL - root
		ULONG nIdxInsertBefore = (ULONG(-1L)), // if (ULONG(-1L)) - insert to end
		bool bMove = true,
		bool bCloneCellObjects = true,
		INT nExpandAction = TVE_TOGGLE, // TVE_TOGGLE in this case - keep expanded state
		bool bEnableCopyingIntoInnerBranch = true,
		bool bRedraw = true,
		bool * p_bCopyingIntoInnerBranch = NULL
		);
	virtual bool TreeGridSortOrderSetup( // default paremters will sort root with all deep children
		const CExtGridDataSortOrder & _gdsoUpdate,
		bool bUpdateExistingSortOrder = false,
		bool bInvertIntersectionSortOrder = true,
		bool bUseDeepSorting = true,
		HTREEITEM htiSort = NULL, // NULL - use root
		bool bRedraw = true
		);
	virtual bool GridSortOrderSetup(
		bool bSortColumns,
		const CExtGridDataSortOrder & _gdsoUpdate,
		bool bUpdateExistingSortOrder = false,
		bool bInvertIntersectionSortOrder = true,
		bool bRedraw = true
		);
	virtual bool ItemSortChildren(
		HTREEITEM hTreeItem,
		const CExtGridDataSortOrder & _gdso,
		bool bRestoreFocus = true,
		bool bRedraw = true
		);
	virtual bool ItemSortChildrenDeep(
		HTREEITEM hTreeItem,
		const CExtGridDataSortOrder & _gdso,
		bool bRestoreFocus = true,
		bool bRedraw = true
		);
	virtual LONG ItemRemove(
		HTREEITEM hTreeItem,
		bool bChildrenOnly = false,
		bool bRedraw = true
		); // returns count of removed items
	CExtGridCell * ItemGetCell(
		HTREEITEM hTreeItem,
		LONG nColNo,
		INT nColType = 0,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true,
		bool bUseColumnDefaultValue = true
		);
	virtual const CExtGridCell * ItemGetCell(
		HTREEITEM hTreeItem,
		LONG nColNo,
		INT nColType = 0,
		CRuntimeClass * pInitRTC = NULL,
		bool bAutoFindValue = true,
		bool bUseColumnDefaultValue = true
		) const;
	virtual bool ItemSetCell(
		HTREEITEM hTreeItem,
		LONG nColNo,
		INT nColType = 0,
		const CExtGridCell * pCellNewValue = NULL, // if NULL - empty existing cell values
		bool bReplace = false, // false - assign to existing cell instances or column/row type created cells, true - create new cloned copies of pCellNewValue
		CRuntimeClass * pInitRTC = NULL // runtime class for new cell instance (used if bReplace=false)
		);
	bool ItemIsExpanded(
		HTREEITEM hTreeItem
		) const;
	virtual CExtTreeGridCellNode::e_expand_box_shape_t ItemGetExpandBoxShape(
		HTREEITEM hTreeItem
		) const;
	virtual void ItemExpand(
		HTREEITEM hTreeItem,
		INT nActionTVE = TVE_TOGGLE, // TVE_COLLAPSE, TVE_EXPAND, TVE_TOGGLE only
		bool bRedraw = true
		);
	virtual void ItemEnsureVisibleBranch(
		HTREEITEM hTreeItem,
		bool bRedraw = true
		);
	virtual void ItemExpandAll(
		HTREEITEM hTreeItem,
		INT nActionTVE = TVE_TOGGLE, // TVE_COLLAPSE, TVE_EXPAND, TVE_TOGGLE only
		bool bRedraw = true
		);
	virtual HTREEITEM ItemFocusGet() const;
protected:
	mutable LONG m_nItemFocusLockCounter;
public:
	virtual bool ItemFocusIsLocked() const;
	virtual void ItemFocusLock( bool bLock ) const;
	virtual bool ItemFocusSet(
		HTREEITEM hTreeItem,
		LONG nColNo,
		bool bRedraw
		);
	bool ItemFocusSet(
		HTREEITEM hTreeItem,
		bool bRedraw = true
		);
	virtual bool ItemEnsureExpanded(
		HTREEITEM hTreeItem,
		bool bRedraw = true
		);
	LONG ItemGetSiblingIndexOf(
		HTREEITEM hTreeItem
		) const;
	LONG ItemGetVisibleIndexOf(
		HTREEITEM hTreeItem
		) const;
protected:
	virtual bool _IsTreeGridWndInitialized() const;
	virtual void _InitTreeGridWnd();
	mutable ULONG m_nGbwAdjustRectsLock;
public:
	virtual CExtGridHitTestInfo & HitTest(
		CExtGridHitTestInfo & htInfo,
		bool bReAlignCellResizing,
		bool bSupressZeroTopCellResizing,
		bool bComputeOuterDropAfterState = false
		) const;
	virtual void OnGbwResizingStateUpdate(
		bool bInitial,
		bool bFinal,
		const CPoint * p_ptClient = NULL
		);
	virtual void OnGbwResizingStateApply(
		bool bHorz,
		LONG nItemNo,
		INT nItemExtent
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
	virtual bool OnGbwAnalyzeCellMouseClickEvent(
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nRepCnt, // 0 - button up, 1 - single click, 2 - double click, 3 - post single click & begin editing
		UINT nFlags, // mouse event flags
		CPoint point // mouse pointer in client coordinates
		);
	virtual bool OnGbwAnalyzeCellKeyEvent(
		bool bKeyDownEvent, // true - key-down event, false - key-up event
		UINT nChar, // virtual key code
		UINT nRepCnt, // key-down/key-up press count
		UINT nFlags // key-down/key-up event flags
		);
	virtual void OnGbwAdjustRects(
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		RECT & rcCellExtraA,
		RECT & rcCellA
		) const;
	virtual void OnSwUpdateWindow();
	virtual LONG RowCountGet() const;

	bool m_bAdustNullIndent:1, m_bDrawFilledExpandGlyphs:1;
	mutable CExtCmdIcon m_iconTreeBoxExpanded, m_iconTreeBoxCollapsed;

	virtual bool OnTreeGridQueryPpvwVisibilityForItem(
		const CExtTreeGridCellNode * pNode
		) const;

	virtual void OnTreeGridNodeRemoved(
		CExtTreeGridCellNode * pNode
		);
	virtual void OnTreeGridPaintExpandButton(
		CDC & dc,
		HTREEITEM hTreeItem,
		bool bExpanded,
		const CRect & rcIndent
		) const;
	virtual bool OnTreeGridQueryColumnOutline(
		LONG nColNo
		) const;
	virtual void OnTreeGridToggleItemExpandedState(
		LONG nRowNo,
		CExtGridHitTestInfo * pHtInfo = NULL,
		INT nActionTVE = TVE_TOGGLE // TVE_COLLAPSE, TVE_EXPAND, TVE_TOGGLE only
		);
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
	virtual bool OnTreeQueryPlainGridModeVeticalLayout() const;
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

	virtual void OnTreeDataProviderSortEnter();
	virtual void OnTreeDataProviderSortLeave();
	virtual void OnTreeDataProviderSwapSeries(
		HTREEITEM hTreeItem1,
		HTREEITEM hTreeItem2,
		LONG nSwapCounter
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

	virtual bool RowHide(
		LONG nRowNo,
		LONG nHideCount = 1,
		bool bRedraw = true
		);
	virtual bool RowUnHideAll(
		bool bRedraw = true
		);
	virtual void OnGridFilterCalcRowIndicesForFilteringColumns(
		LONG nScanColNo,
		bool bLeft,
		CList < LONG, LONG & > & _listIndices
		);
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
	virtual void OnTreeGridFilterCalcPlainUniqueValuesForColumn(
		HTREEITEM hti,
		CExtArray < CExtSafeString > & arrUniqueStringValues,
		LONG nColNo,
		bool bLeavesOnly,
		bool bIncludeHidden = true
		) const;
	virtual bool OnGridFilterCalcPlainUniqueValuesForColumn(
		CExtArray < CExtSafeString > & arrUniqueStringValues,
		LONG nColNo
		) const;
	virtual bool OnGridFilterCalcPlainUniqueValuesForRow(
		CExtArray < CExtSafeString > & arrUniqueStringValues,
		LONG nRowNo
		) const;
	virtual void OnGridFilterUpdateForRows(
		LONG nScanRowNo,
		LONG nEventSrcColNo,
		bool bTop,
		CExtGridCellHeaderFilter * pHeaderFilterCell,
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
	virtual bool FilterStateForOuterColumnSerialize(
		CArchive & ar,
		bool bLeft = true,
		LONG nColNo = 0L,
		bool bRedrawAfterLoading = true,
		bool bEnableThrowExcetions = false
		);
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

	bool ItemIsDisplayed(
		HTREEITEM hti
		) const;
	bool ItemIsHidden(
		HTREEITEM hti
		) const;
	void ItemHide(
		HTREEITEM hti,
		bool bHide,
		bool bRedraw = true
		);

	//{{AFX_VIRTUAL(CExtTreeGridWnd)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExtTreeGridWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtTreeGridWnd

#endif // (!defined __EXT_MFC_NO_TREEGRIDWND)

#endif /// __EXTTREEGRIDWND_H



