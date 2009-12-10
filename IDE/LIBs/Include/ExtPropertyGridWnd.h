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

#if (!defined __EXTPROPERTYGRIDWND_H)
#define __EXTPROPERTYGRIDWND_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if (!defined __EXT_MFC_NO_PROPERTYGRIDWND)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXTTREEGRIDWND_H)
	#include <ExtTreeGridWnd.h>
#endif

#if (!defined __EXT_TOOLCONTROLBAR_H)
	#include <ExtToolControlBar.h>
#endif

#if (!defined __EXT_COMBO_BOX_H)
	#include <ExtComboBox.h>
#endif

class CExtPopupMenuWnd;
class CExtPropertyItem;
class CExtPropertyValue;
class CExtPropertyCategory;
class CExtPropertyStore;
class CExtPropertyGridCellArea;
class CExtPropertyGridWnd;
class CExtPropertyGridWndSorted;
class CExtPropertyGridWndCategorized;
class CExtPropertyGridComboBoxBar;
class CExtPropertyGridToolBar;
class CExtPropertyGridTipBar;
class CExtPropertyGridCtrl;

#define __EXT_MGC_PROPERTY_GRID_DEFAULT_LEVEL_INDENT 14

#define __EXT_MFC_ID_PROPERTY_GRID_TOOLBAR		0x1001
#define __EXT_MFC_ID_PROPERTY_GRID_TIPBAR		0x1002
#define __EXT_MFC_ID_PROPERTY_GRID_COMBO_BOX	0x1003
#define __EXT_MFC_ID_PROPERTY_GRID_SORTED		30389
#define __EXT_MFC_ID_PROPERTY_GRID_CATEGORIZED	30388

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyItem

class __PROF_UIS_API CExtPropertyItem : public CObject
{
protected:
	CExtPropertyItem * m_pParent;
	CExtSafeString m_strName, m_strDescription;
	bool m_bExpanded:1, m_bInputEnabled:1;
	INT m_nHeightPx;
	LPARAM m_lParam;
public:
	struct __PROF_UIS_API IPropertyItemEnumSite
	{
		virtual bool OnPropertyItemEnum(
			CExtPropertyItem * pItem,
			LPVOID pCookie = NULL
			) = 0;
	}; // struct IPropertyItemEnumSite

	DECLARE_SERIAL( CExtPropertyItem );
	CExtPropertyItem(
		__EXT_MFC_SAFE_LPCTSTR strName = NULL
		);
	virtual ~CExtPropertyItem();
	virtual bool CanBeInsertedIntoPropertyGrid() const;
	virtual void Delete();
	virtual void DeleteChildItem(
		CExtPropertyItem * pChildItem
		);
	virtual bool ChildrenHaveDifferentDefaultValues() const;
	virtual bool ChildrenHaveDifferentActiveValues() const;
	virtual void Combine(
		CExtPropertyItem * pOtherItem
		);
	virtual void AdjustIntersection(
		INT nIntersectionItemCount
		);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif
	virtual ULONG Enum(
		IPropertyItemEnumSite * pEnumSite,
		LPVOID pCookie = NULL,
		bool bEnumThisItem = false,
		bool bEnumChilrenItems = true,
		bool * p_bCancelWalking = NULL
		);
	virtual INT QueryHeightPx(
		const CExtPropertyGridWnd * pPGW
		) const;
	virtual INT HeightPxGet() const;
	virtual void HeightPxSet( INT nHeightPx );
	virtual LPARAM LPARAM_Get() const;
	virtual void LPARAM_Set( LPARAM lParam );
	virtual bool ExpandedGet() const;
	virtual void ExpandedSet( bool bExpanded );
	virtual bool OnInputEnabledGet( bool bUseNestedFlags ) const;
	virtual void OnInputEnabledSet( bool bUseNestedFlags, bool bInputEnabled );
	virtual bool InputEnabledGet() const;
	virtual void InputEnabledSet( bool bInputEnabled );
	virtual __EXT_MFC_SAFE_LPCTSTR NameGet() const;
	virtual void NameSet( __EXT_MFC_SAFE_LPCTSTR strName );
	virtual void ValueDefaultFromActive();
	virtual void ValueActiveFromDefault();
	const CExtGridCell & ValueDefaultGetRef() const;
	CExtGridCell & ValueDefaultGetRef();
	const CExtGridCell * ValueDefaultGet() const;
	virtual CExtGridCell * ValueDefaultGet();
	CExtGridCell * ValueDefaultGetByRTC(
		CRuntimeClass * pInitRTC
		);
	virtual bool ValueDefaultSet(
		CExtGridCell * pValue,
		bool bClone = false
		);
	const CExtGridCell & ValueActiveGetRef() const;
	CExtGridCell & ValueActiveGetRef();
	const CExtGridCell * ValueActiveGet() const;
	virtual CExtGridCell * ValueActiveGet();
	CExtGridCell * ValueActiveGetByRTC(
		CRuntimeClass * pInitRTC
		);
	virtual bool ValueActiveSet(
		CExtGridCell * pValue,
		bool bClone = false
		);
	virtual void Apply(
		CExtGridCell * pValue = NULL
		);
	virtual __EXT_MFC_SAFE_LPCTSTR DescriptionGet() const;
	virtual void DescriptionSet(
		__EXT_MFC_SAFE_LPCTSTR strDescription
		);
	const CExtPropertyStore & GetStoreRef() const;
	CExtPropertyStore & GetStoreRef();
	const CExtPropertyStore * GetStore() const;
	virtual CExtPropertyStore * GetStore();
	const CExtPropertyItem & ItemParentGetRef() const;
	CExtPropertyItem & ItemParentGetRef();
	const CExtPropertyItem * ItemParentGet() const;
	virtual CExtPropertyItem * ItemParentGet();
	virtual void ItemParentSet(
		CExtPropertyItem * pItem
		);
	virtual INT ItemGetCount() const;
	const CExtPropertyItem & operator [] ( INT nIndex ) const;
	CExtPropertyItem & operator [] ( INT nIndex );
	const CExtPropertyItem & ItemGetRefAt( INT nIndex ) const;
	CExtPropertyItem & ItemGetRefAt( INT nIndex );
	const CExtPropertyItem * ItemGetAt( INT nIndex ) const;
	virtual CExtPropertyItem * ItemGetAt( INT nIndex );
	virtual CExtPropertyItem * ItemGetByName(
		__EXT_MFC_SAFE_LPCTSTR strName,
		INT & nIndex,
		CRuntimeClass * pRTC = NULL
		);
	const CExtPropertyItem * ItemGetByName(
		__EXT_MFC_SAFE_LPCTSTR strName,
		INT & nIndex,
		CRuntimeClass * pRTC = NULL
		) const;
	CExtPropertyItem * ItemGetByName(
		__EXT_MFC_SAFE_LPCTSTR strName,
		CRuntimeClass * pRTC = NULL
		);
	const CExtPropertyItem * ItemGetByName(
		__EXT_MFC_SAFE_LPCTSTR strName,
		CRuntimeClass * pRTC = NULL
		) const;
	virtual bool ItemInsert(
		CExtPropertyItem * pItem,
		INT nIndexInsertBefore = -1
		);
	virtual bool ItemRemove(
		INT nIndex = 0,
		INT nCount = -1
		);

	virtual void OnGridRowInitialized(
		CExtPropertyGridWnd & wndPG,
		HTREEITEM hTreeItem,
		CExtGridCell * pCellCaption,
		CExtGridCell * pCellValue
		);

	virtual void Serialize( CArchive & ar );

	virtual void BuildLinearList(
		CTypedPtrList < CPtrList, CExtPropertyItem * > & _list,
		bool bIncludeThisItem,
		bool bAddToTail = true
		);
	virtual CExtSafeString BuildCompoundTextDefault() const;
	virtual CExtSafeString BuildCompoundTextActive() const;

	virtual bool IsModified() const;
	virtual bool Reset();
}; // class CExtPropertyItem

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyValue

class __PROF_UIS_API CExtPropertyValue : public CExtPropertyItem
{
protected:
	CExtGridCell * m_pValueDefault, * m_pValueActive;
public:
	DECLARE_SERIAL( CExtPropertyValue );
	CExtPropertyValue(
		__EXT_MFC_SAFE_LPCTSTR strName = NULL,
		CExtGridCell * pValueActive = NULL,
		CExtGridCell * pValueDefault = NULL,
		bool bCloneActive = false,
		bool bCloneDefault = false
		);
	virtual ~CExtPropertyValue();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif
	virtual void ValueDefaultFromActive();
	virtual void ValueActiveFromDefault();
	virtual CExtGridCell * ValueDefaultGet();
	virtual bool ValueDefaultSet(
		CExtGridCell * pValue,
		bool bClone = false
		);
	virtual CExtGridCell * ValueActiveGet();
	virtual bool ValueActiveSet(
		CExtGridCell * pValue,
		bool bClone = false
		);

	virtual CExtSafeString BuildCompoundTextDefault() const;
	virtual CExtSafeString BuildCompoundTextActive() const;

	virtual bool IsModified() const;
	virtual bool Reset();
}; // class CExtPropertyValue

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyValueSingleCell

class __PROF_UIS_API CExtPropertyValueSingleCell : public CExtPropertyValue
{
public:
	DECLARE_SERIAL( CExtPropertyValueSingleCell );
	CExtPropertyValueSingleCell(
		__EXT_MFC_SAFE_LPCTSTR strName = NULL,
		CExtGridCell * pValue = NULL,
		bool bClone = false
		);
	virtual ~CExtPropertyValueSingleCell();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif
	virtual void ValueDefaultFromActive();
	virtual void ValueActiveFromDefault();
	virtual CExtGridCell * ValueDefaultGet();
	virtual bool ValueDefaultSet(
		CExtGridCell * pValue,
		bool bClone = false
		);
	virtual CExtSafeString BuildCompoundTextDefault() const;

	virtual bool IsModified() const;
	virtual bool Reset();
}; // class CExtPropertyValueSingleCell

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyGridCellCompound

class __PROF_UIS_API CExtPropertyGridCellCompound
	: public CExtGridCellString
{
public:
	DECLARE_SERIAL( CExtPropertyGridCellCompound );
	IMPLEMENT_ExtGridCell_Clone( CExtPropertyGridCellCompound, CExtGridCellString );
	CExtPropertyGridCellCompound(
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtPropertyGridCellCompound( const CExtGridCell & other );
	virtual void Assign( const CExtGridCell & other );
	virtual void Serialize( CArchive & ar );
}; // class CExtPropertyGridCellCompound

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyValueCompound

class __PROF_UIS_API CExtPropertyValueCompound : public CExtPropertyValue
{
	CTypedPtrArray < CPtrArray, CExtPropertyValue * > m_arrItems;
public:
	DECLARE_SERIAL( CExtPropertyValueCompound );
	CExtPropertyValueCompound(
		__EXT_MFC_SAFE_LPCTSTR strName = NULL
		);
	virtual ~CExtPropertyValueCompound();
	virtual bool CanBeInsertedIntoPropertyGrid() const;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif
	virtual INT ItemGetCount() const;
	virtual CExtPropertyItem * ItemGetAt( INT nIndex );
	virtual bool ItemInsert(
		CExtPropertyItem * pItem,
		INT nIndexInsertBefore = -1
		);
	virtual bool ItemRemove(
		INT nIndex = 0,
		INT nCount = -1
		);
	virtual void ValueDefaultFromActive();
	virtual void ValueActiveFromDefault();
	virtual CExtGridCell * ValueDefaultGet();
	virtual bool ValueDefaultSet(
		CExtGridCell * pValue,
		bool bClone = false
		);
	virtual CExtGridCell * ValueActiveGet();
	virtual bool ValueActiveSet(
		CExtGridCell * pValue,
		bool bClone = false
		);
	virtual CExtSafeString BuildCompoundTextDefault() const;
	virtual CExtSafeString BuildCompoundTextActive() const;
	virtual void SynchronizeCompoundCellDefault();
	virtual void SynchronizeCompoundCellActive();
	virtual void SynchronizeWithControl(
		CExtPropertyGridCtrl * pPGC,
		bool bResetValues,
		bool bSynchronizeChildren
		);
	virtual void SynchronizeWithControl(
		CExtPropertyGridWnd * pPGW,
		bool bResetValues,
		bool bSynchronizeChildren
		);
	virtual __EXT_MFC_SAFE_LPCTSTR SynchronizeChildrenTreeDefault(
		__EXT_MFC_SAFE_LPCTSTR strCompoundPath,
		bool bParseOnly,
		CExtPropertyValue ** ppOutValueParseError = NULL,
		__EXT_MFC_SAFE_LPCTSTR _strListSeparator = NULL,
		__EXT_MFC_SAFE_LPCTSTR strTrimLeftRight = NULL
		);
	virtual __EXT_MFC_SAFE_LPCTSTR SynchronizeChildrenTreeActive(
		__EXT_MFC_SAFE_LPCTSTR strCompoundPath,
		bool bParseOnly,
		CExtPropertyValue ** ppOutValueParseError = NULL,
		__EXT_MFC_SAFE_LPCTSTR _strListSeparator = NULL,
		__EXT_MFC_SAFE_LPCTSTR strTrimLeftRight = NULL
		);
	static __EXT_MFC_SAFE_LPCTSTR stat_ExpandListPart(
		__EXT_MFC_SAFE_LPCTSTR strWalk,
		CExtSafeString & strOutListItem,
		__EXT_MFC_SAFE_LPCTSTR _strListSeparator = NULL,
		__EXT_MFC_SAFE_LPCTSTR strTrimLeftRight = NULL
		);
	virtual void SynchronizeCompoundParentPathDefault();
	virtual void SynchronizeCompoundParentPathActive();

	virtual bool IsModified() const;
	virtual bool Reset();

	virtual void OnGridRowInitialized(
		CExtPropertyGridWnd & wndPG,
		HTREEITEM hTreeItem,
		CExtGridCell * pCellCaption,
		CExtGridCell * pCellValue
		);
}; // class CExtPropertyValueCompound

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyValueMixed

class __PROF_UIS_API CExtPropertyValueMixed : public CExtPropertyValue
{
protected:
	CTypedPtrArray < CPtrArray, CExtPropertyItem * > m_arrItems;
public:
	DECLARE_DYNCREATE( CExtPropertyValueMixed );
	CExtPropertyValueMixed(
		__EXT_MFC_SAFE_LPCTSTR strName = NULL
		);
	virtual ~CExtPropertyValueMixed();
	virtual bool CanBeInsertedIntoPropertyGrid() const;
	virtual void DeleteChildItem(
		CExtPropertyItem * pChildItem
		);
	virtual void Combine(
		CExtPropertyItem * pOtherItem
		);
	virtual void AdjustIntersection(
		INT nIntersectionItemCount
		);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif
	virtual INT ItemGetCount() const;
	virtual CExtPropertyItem * ItemGetAt( INT nIndex );
	virtual bool ItemInsert(
		CExtPropertyItem * pItem,
		INT nIndexInsertBefore = -1
		);
	virtual bool ItemRemove(
		INT nIndex = 0,
		INT nCount = -1
		);
	virtual void ValueDefaultFromActive();
	virtual void ValueActiveFromDefault();
	virtual CExtGridCell * ValueDefaultGet();
	virtual bool ValueDefaultSet(
		CExtGridCell * pValue,
		bool bClone = false
		);
	virtual CExtGridCell * ValueActiveGet();
	virtual bool ValueActiveSet(
		CExtGridCell * pValue,
		bool bClone = false
		);
	virtual void Apply(
		CExtGridCell * pValue = NULL
		);

	virtual void OnGridRowInitialized(
		CExtPropertyGridWnd & wndPG,
		HTREEITEM hTreeItem,
		CExtGridCell * pCellCaption,
		CExtGridCell * pCellValue
		);

	virtual void Serialize( CArchive & ar );

	virtual CExtSafeString BuildCompoundTextDefault() const;
	virtual CExtSafeString BuildCompoundTextActive() const;

	virtual bool IsModified() const;
	virtual bool Reset();

	virtual void ExpandedSet( bool bExpanded );

	virtual INT HeightPxGet() const;
	virtual void HeightPxSet( INT nHeightPx );
}; // class CExtPropertyValueMixed

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyValueMixedCompound

class __PROF_UIS_API CExtPropertyValueMixedCompound : public CExtPropertyValueMixed
{
protected:
	CTypedPtrArray < CPtrArray, CExtPropertyValueMixed * > m_arrCompoundParts;
public:
	DECLARE_DYNCREATE( CExtPropertyValueMixedCompound );
	CExtPropertyValueMixedCompound(
		__EXT_MFC_SAFE_LPCTSTR strName = NULL
		);
	virtual ~CExtPropertyValueMixedCompound();
	virtual void DeleteChildItem(
		CExtPropertyItem * pChildItem
		);
	virtual void Combine(
		CExtPropertyItem * pOtherItem
		);
	virtual void AdjustIntersection(
		INT nIntersectionItemCount
		);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif
	virtual INT ItemGetCount() const;
	virtual CExtPropertyItem * ItemGetAt( INT nIndex );
	virtual bool ItemInsert(
		CExtPropertyItem * pItem,
		INT nIndexInsertBefore = -1
		);
	virtual bool ItemRemove(
		INT nIndex = 0,
		INT nCount = -1
		);
	virtual void ValueDefaultFromActive();
	virtual void ValueActiveFromDefault();
	virtual CExtGridCell * ValueDefaultGet();
	virtual bool ValueDefaultSet(
		CExtGridCell * pValue,
		bool bClone = false
		);
	virtual CExtGridCell * ValueActiveGet();
	virtual bool ValueActiveSet(
		CExtGridCell * pValue,
		bool bClone = false
		);
	virtual void Apply(
		CExtGridCell * pValue = NULL
		);

	virtual void OnGridRowInitialized(
		CExtPropertyGridWnd & wndPG,
		HTREEITEM hTreeItem,
		CExtGridCell * pCellCaption,
		CExtGridCell * pCellValue
		);

	virtual void Serialize( CArchive & ar );

	virtual CExtSafeString BuildCompoundTextDefault() const;
	virtual CExtSafeString BuildCompoundTextActive() const;

	virtual INT CompoundPartGetCount() const;
	const CExtPropertyValueMixed & CompoundPartGetRefAt( INT nIndex ) const;
	CExtPropertyValueMixed & CompoundPartGetRefAt( INT nIndex );
	const CExtPropertyValueMixed * CompoundPartGetAt( INT nIndex ) const;
	virtual CExtPropertyValueMixed * CompoundPartGetAt( INT nIndex );
	virtual CExtPropertyValueMixed * CompoundPartGetByName(
		__EXT_MFC_SAFE_LPCTSTR strName,
		INT & nIndex,
		CRuntimeClass * pRTC = NULL
		);
	const CExtPropertyValueMixed * CompoundPartGetByName(
		__EXT_MFC_SAFE_LPCTSTR strName,
		INT & nIndex,
		CRuntimeClass * pRTC = NULL
		) const;
	virtual bool CompoundPartInsert(
		CExtPropertyValueMixed * pItem,
		INT nIndexInsertBefore = -1
		);
	virtual bool CompoundPartRemove(
		INT nIndex = 0,
		INT nCount = -1
		);

	virtual __EXT_MFC_SAFE_LPCTSTR SynchronizeChildrenTreeDefault(
		__EXT_MFC_SAFE_LPCTSTR strCompoundPath,
		bool bParseOnly,
		CExtPropertyValue ** ppOutValueParseError = NULL,
		__EXT_MFC_SAFE_LPCTSTR _strListSeparator = NULL,
		__EXT_MFC_SAFE_LPCTSTR strTrimLeftRight = NULL
		);
	virtual __EXT_MFC_SAFE_LPCTSTR SynchronizeChildrenTreeActive(
		__EXT_MFC_SAFE_LPCTSTR strCompoundPath,
		bool bParseOnly,
		CExtPropertyValue ** ppOutValueParseError = NULL,
		__EXT_MFC_SAFE_LPCTSTR _strListSeparator = NULL,
		__EXT_MFC_SAFE_LPCTSTR strTrimLeftRight = NULL
		);

	virtual void SynchronizeCompoundParentPathDefault();
	virtual void SynchronizeCompoundParentPathActive();

	virtual bool IsModified() const;
	virtual bool Reset();

	virtual void SynchronizeWithControl(
		CExtPropertyGridCtrl * pPGC,
		bool bResetValues,
		bool bSynchronizeChildren
		);
	virtual void SynchronizeWithControl(
		CExtPropertyGridWnd * pPGW,
		bool bResetValues,
		bool bSynchronizeChildren
		);
}; // class CExtPropertyValueMixedCompound

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyCategory

class __PROF_UIS_API CExtPropertyCategory : public CExtPropertyItem
{
protected:
	CTypedPtrArray < CPtrArray, CExtPropertyItem * > m_arrItems;
public:
	DECLARE_SERIAL( CExtPropertyCategory );
	CExtPropertyCategory(
		__EXT_MFC_SAFE_LPCTSTR strName = NULL
		);
	virtual ~CExtPropertyCategory();
	virtual bool CanBeInsertedIntoPropertyGrid() const;
	virtual void Combine(
		CExtPropertyItem * pOtherItem
		);
	virtual void AdjustIntersection(
		INT nIntersectionItemCount
		);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif
	virtual INT ItemGetCount() const;
	virtual CExtPropertyItem * ItemGetAt( INT nIndex );
	virtual bool ItemInsert(
		CExtPropertyItem * pItem,
		INT nIndexInsertBefore = -1
		);
	virtual bool ItemRemove(
		INT nIndex = 0,
		INT nCount = -1
		);

	virtual bool IsModified() const;
	virtual bool Reset();

	virtual bool OnInputEnabledGet( bool bUseNestedFlags ) const;
	virtual void OnInputEnabledSet( bool bUseNestedFlags, bool bInputEnabled );
	virtual void ExpandedSet( bool bExpanded );
}; // class CExtPropertyCategory

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyStore

class __PROF_UIS_API CExtPropertyStore : public CExtPropertyCategory
{
public:
	DECLARE_SERIAL( CExtPropertyStore );
	CExtPropertyStore(
		__EXT_MFC_SAFE_LPCTSTR strName = NULL
		);
	virtual ~CExtPropertyStore();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif
}; // class CExtPropertyStore

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyGridCellArea

class __PROF_UIS_API CExtPropertyGridCellArea
	: public CExtGridCellStringDM
{
public:
	DECLARE_SERIAL( CExtPropertyGridCellArea );
	IMPLEMENT_ExtGridCell_Clone( CExtPropertyGridCellArea, CExtGridCellStringDM );
	CExtPropertyGridCellArea(
		CExtGridDataProvider * pDataProvider = NULL
		);
	CExtPropertyGridCellArea( const CExtGridCell & other );
	virtual bool OnClick(
		CExtGridWnd & wndGrid,
		const CExtGridHitTestInfo & htInfo,
		UINT nChar, // VK_LBUTTON, VK_RBUTTON or VK_MBUTTON only
		UINT nRepCnt, // 0 - button up, 1 - single click, 2 - double click, 3 - post single click & begin editing
		UINT nFlags // mouse event flags
		);
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
	virtual void OnHitTestInfoAdjust(
		const CExtGridWnd & wndGrid,
		CExtGridHitTestInfo & htInfo,
		bool bComputeOuterDropAfterState
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
}; // class CExtPropertyGridCellArea

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyGridWnd window

class __PROF_UIS_API CExtPropertyGridWnd : public CExtTreeGridWnd
{
public:
	DECLARE_DYNCREATE( CExtPropertyGridWnd );
	CExtPropertyGridWnd(
		CExtPropertyGridCtrl * pPropertyGridCtrl = NULL
		);
	virtual ~CExtPropertyGridWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif
	virtual bool Create(
		CWnd * pWndParent,
		UINT nDlgCtrlID,
		bool bVisible = false
		);

	void SetProportionalColumnWidth(
		double lfPercentLeft = 0.5 // 0.0 < lfPercentLeft < 1.0
		);

	virtual CExtPropertyGridCtrl * PropertyGridCtrlSet(
		CExtPropertyGridCtrl * pPS
		);
	virtual CExtPropertyGridCtrl * PropertyGridCtrlGet();
	const CExtPropertyGridCtrl * PropertyGridCtrlGet() const;
	CExtPropertyGridCtrl & PropertyGridCtrlGetRef();
	const CExtPropertyGridCtrl & PropertyGridCtrlGetRef() const;

	virtual CExtPropertyStore * PropertyStoreGet();
	const CExtPropertyStore * PropertyStoreGet() const;
	CExtPropertyStore & PropertyStoreGetRef();
	const CExtPropertyStore & PropertyStoreGetRef() const;

	virtual void PropertyStoreSynchronizeAll();
	virtual void PropertyStoreSynchronizeOneLevel(
		CExtPropertyItem * pPropertyItem,
		CExtPropertyItem * pParentItem = NULL
		);

	virtual HTREEITEM PropertyItemInsert(
		CExtPropertyItem * pPropertyItem,
		LONG nIndex = -1,
		HTREEITEM htiParent = TVI_ROOT,
		bool bRedraw = true
		);
	virtual HTREEITEM PropertyItemInsertAlphabetic(
		CExtPropertyItem * pPropertyItem,
		HTREEITEM htiParent = TVI_ROOT,
		bool bRedraw = true
		);
	virtual HTREEITEM PropertyItemToTreeItem(
		const CExtPropertyItem * pPropertyItem
		);
	virtual CExtPropertyItem * PropertyItemFromTreeItem(
		HTREEITEM hTreeItem
		);
	const CExtPropertyItem * PropertyItemFromTreeItem(
		HTREEITEM hTreeItem
		) const;
	virtual void PropertyItemRemove(
		CExtPropertyItem * pPropertyItem = NULL, // NULL - remove all
		bool bRedraw = true
		);
	virtual void PropertyItemRemove(
		HTREEITEM hTreeItem = TVI_ROOT, // TVI_ROOT - remove all
		bool bRedraw = true
		);

	virtual CExtGridHitTestInfo & HitTest(
		CExtGridHitTestInfo & htInfo,
		bool bReAlignCellResizing,
		bool bSupressZeroTopCellResizing,
		bool bComputeOuterDropAfterState = false
		) const;
	virtual void OnPgwSynchronizeCompoundValue(
		CExtPropertyValueCompound * pCompoundValue
		);
	virtual void OnPgwSynchronizeCompoundValueChildren(
		CExtPropertyValueCompound * pCompoundValue
		);
	virtual void OnPgwSynchronizeCompoundValueParentPath(
		CExtPropertyValueCompound * pCompoundValue
		);
	virtual void OnPgwSynchronizeCompoundValue(
		CExtPropertyValueMixedCompound * pMixedCompound
		);
	virtual void OnPgwSynchronizeCompoundValueChildren(
		CExtPropertyValueMixedCompound * pMixedCompound
		);
	virtual void OnPgwSynchronizeCompoundValueParentPath(
		CExtPropertyValueMixedCompound * pMixedCompound
		);
	virtual void OnPgwContextMenu(
		CWnd * pWndHit,
		CExtGridHitTestInfo & htInfo,
		CExtPropertyItem * pPropertyItem // can be NULL
		);

	virtual void ItemExpand(
		HTREEITEM hTreeItem,
		INT nActionTVE = TVE_TOGGLE, // TVE_COLLAPSE, TVE_EXPAND, TVE_TOGGLE only
		bool bRedraw = true
		);

	INT m_nConstantIndent, m_nPxResizeStepByKeyboard;
	bool m_bAutoDeleteWindow:1, m_bThemeBackground:1,
		m_bFullRowSelection:1, m_bDrawFocusRect:1,
		m_bSortedCategories:1, m_bSortedValues:1,
	m_bInputEnabledCheckMarksInCategories:1, m_bInputEnabledCheckMarksInValues:1, m_bInputEnabledNestedFlags:1,
		m_bSynchronizeColumnWidth:1, m_bHighlightReadOnlyValues:1,
		m_bMakeBoldModifiedNames:1, m_bMakeBoldModifiedValues:1;
	CBrush m_brushCategoryBk, m_brushOutlineBk;
protected:
	CMap < HTREEITEM, HTREEITEM, CExtPropertyItem *, CExtPropertyItem * >
		m_mapItems;
	CMap < CExtPropertyItem *, CExtPropertyItem *, HTREEITEM, HTREEITEM >
		m_mapProperties;
	CExtPropertyGridCtrl * m_pPropertyGridCtrl;
	CExtScrollBar m_wndScrollBarV;
	bool m_bSynchronizingFocus:1;

	virtual void _InitTreeGridWnd();
	virtual void _SyncInnerRowHeight(
		bool bSyncScrollingParms = true
		);
	virtual void _SyncColumnWidths();
	void _DoColumnLayout();
	virtual void _DoColumnLayout(
		const CRect & rcClient
		);

public:
	virtual void _OnSynchronizeInputEnabledCheckMark(
		CExtPropertyItem * pPropertyItem,
		bool bSychronizeChildren = true
		);
	virtual void _OnPropertyInputEnabled(
		bool bInputEnabled,
		LONG nRowNo
		);

protected:
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
	virtual CPoint FocusSet(
		const POINT & ptNewFocus,
		bool bEnsureVisibleColumn = true,
		bool bEnsureVisibleRow = true,
		bool bResetSelectionToFocus = true,
		bool bRedraw = true,
		bool * p_bCanceled = NULL
		);
	virtual void OnGbwResizingStateApply(
		bool bHorz,
		LONG nItemNo,
		INT nItemExtent
		);
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
	virtual CRect OnSwGetClientRect() const;
	virtual COLORREF OnSiwGetReadOnlyTextColor() const;
	virtual COLORREF OnSiwGetReadOnlyBackgroundColor() const;
public:
	virtual void OnGridCellInputComplete(
		CExtGridCell & _cell,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		HWND hWndInputControl = NULL
		);
protected:
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
	virtual bool SelectionGetForCell(
		LONG nColNo,
		LONG nRowNo
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
	virtual bool OnGbwAnalyzeCellKeyEvent(
		bool bKeyDownEvent, // true - key-down event, false - key-up event
		UINT nChar, // virtual key code
		UINT nRepCnt, // key-down/key-up press count
		UINT nFlags // key-down/key-up event flags
		);
	virtual void OnSiwDrawFocusRect(
		CDC &dc,
		LPCRECT pRect,
		CObject * pObjSrc = NULL,
		LPARAM lParam = 0L
		) const;
	virtual void OnGbwAdjustRects(
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		RECT & rcCellExtraA,
		RECT & rcCellA
		) const;
	virtual void OnSiwPaintBackground(
		CDC & dc,
		bool bFocusedControl
		) const;
	virtual CRect OnSwRecalcLayout(
		bool bDoLayout,
		LPCRECT pRectClientSrc = NULL
		);
	virtual bool OnSwHasScrollBar( bool bHorz ) const;
	virtual CSize OnGbwCellJoinQueryInfo(
		LONG nColNo,
		LONG nRowNo,
		INT nColType, // = 0
		INT nRowType // = 0
		) const;

public:
	// Overrides
	//{{AFX_VIRTUAL(CExtPropertyGridWnd)
	virtual void PostNcDestroy();
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//{{AFX_MSG(CExtPropertyGridWnd)
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg void OnSize(UINT nType, int cx, int cy) ;
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnResetCmdFromCtxMenu();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

}; // class CExtPropertyGridWnd

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyGridWndSorted window

class __PROF_UIS_API CExtPropertyGridWndSorted : public CExtPropertyGridWnd
{
public:
	DECLARE_DYNCREATE( CExtPropertyGridWndSorted );
	CExtPropertyGridWndSorted(
		CExtPropertyGridCtrl * pPropertyGridCtrl = NULL
		);
	virtual ~CExtPropertyGridWndSorted();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif

	virtual void PropertyStoreSynchronizeOneLevel(
		CExtPropertyItem * pPropertyItem,
		CExtPropertyItem * pParentItem = NULL
		);

public:
	// Overrides
	//{{AFX_VIRTUAL(CExtPropertyGridWndSorted)
	//}}AFX_VIRTUAL

public:
	//{{AFX_MSG(CExtPropertyGridWndSorted)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

}; // class CExtPropertyGridWndSorted

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyGridWndCategorized window

class __PROF_UIS_API CExtPropertyGridWndCategorized : public CExtPropertyGridWnd
{
public:
	DECLARE_DYNCREATE( CExtPropertyGridWndCategorized );
	CExtPropertyGridWndCategorized(
		CExtPropertyGridCtrl * pPropertyGridCtrl = NULL
		);
	virtual ~CExtPropertyGridWndCategorized();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif

public:
	// Overrides
	//{{AFX_VIRTUAL(CExtPropertyGridWndCategorized)
	//}}AFX_VIRTUAL

public:
	//{{AFX_MSG(CExtPropertyGridWndCategorized)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

}; // class CExtPropertyGridWndCategorized

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyGridComboBoxBar window

class __PROF_UIS_API CExtPropertyGridComboBoxBar : public CExtComboBox
{
protected:
	CExtPropertyGridCtrl * m_pPropertyGridCtrl;
public:
	DECLARE_DYNCREATE( CExtPropertyGridComboBoxBar );
	CExtPropertyGridComboBoxBar(
		CExtPropertyGridCtrl * pPropertyGridCtrl = NULL
		);
	~CExtPropertyGridComboBoxBar();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif

	bool m_bAutoDeleteWindow:1;

	virtual bool Create(
		CWnd * pWndParent,
		bool bVisible = true
		);

	virtual void PropertyGridCtrlSet(
		CExtPropertyGridCtrl * pPropertyGridCtrl
		);
	virtual CExtPropertyGridCtrl * PropertyGridCtrlGet();
	const CExtPropertyGridCtrl * PropertyGridCtrlGet() const;
	CExtPropertyGridCtrl & PropertyGridCtrlGetRef();
	const CExtPropertyGridCtrl & PropertyGridCtrlGetRef() const;

	virtual INT PropertyStoreGetCount() const;
	virtual INT PropertyStoreInsert(
		CExtPropertyStore * pPropertyStore,
		INT nPos = -1
		);
	virtual bool PropertyStoreRemove(
		INT nPos = -1 // if -1 - remove all
		);
	virtual CExtPropertyStore * PropertyStoreGetAt(
		INT nPos
		);
	const CExtPropertyStore * PropertyStoreGetAt(
		INT nPos
		) const;

	void SynchronizeCurSel();
protected:
	CTypedPtrArray < CPtrArray, CExtPropertyStore * > m_arrPropertyStores;
	virtual void _SynchronizeCurSel();

public:
	// Overrides
	//{{AFX_VIRTUAL(CExtPropertyGridComboBoxBar)
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	//{{AFX_MSG(CExtPropertyGridComboBoxBar)
	//}}AFX_MSG
	afx_msg void OnSelEndOK();
	afx_msg LRESULT OnSizeParent( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

}; // class CExtPropertyGridComboBoxBar

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyGridToolBar window

class __PROF_UIS_API CExtPropertyGridToolBar : public CExtToolControlBar
{
protected:
	CExtPropertyGridCtrl * m_pPropertyGridCtrl;

public:
	DECLARE_DYNCREATE( CExtPropertyGridToolBar );
	CExtPropertyGridToolBar(
		CExtPropertyGridCtrl * pPropertyGridCtrl = NULL
		);
	virtual ~CExtPropertyGridToolBar();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif

	bool m_bAutoDeleteWindow:1;

	virtual bool Create(
		CWnd * pWndParent,
		bool bVisible = true
		);

	virtual void PropertyGridCtrlSet(
		CExtPropertyGridCtrl * pPropertyGridCtrl
		);
	virtual CExtPropertyGridCtrl * PropertyGridCtrlGet();
	const CExtPropertyGridCtrl * PropertyGridCtrlGet() const;
	CExtPropertyGridCtrl & PropertyGridCtrlGetRef();
	const CExtPropertyGridCtrl & PropertyGridCtrlGetRef() const;
public:
	virtual CExtBarContentExpandButton * OnCreateBarRightBtn();

public:
	// Overrides
	//{{AFX_VIRTUAL(CExtPropertyGridToolBar)
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	//{{AFX_MSG(CExtPropertyGridToolBar)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

}; // class CExtPropertyGridToolBar

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyGridTipBar window

#define __EXT_PROPERTY_TIP_BAR_CLASS_NAME _T("ProfUIS-PropertyTipBar")

class __PROF_UIS_API CExtPropertyGridTipBar
	: public CWnd
	, public CExtPmBridge
{
protected:
	CExtPropertyGridCtrl * m_pPropertyGridCtrl;
	static bool g_bPropertyTipBarClassRegistered;
	static bool RegisterPropertyTipBarClass();
	INT m_nSeparatorHeight, m_nHeightMin, m_nHeightMax,
		m_nHeightDesired;
	HCURSOR m_hCursorResizing;
public:
	DECLARE_DYNCREATE( CExtPropertyGridTipBar );
	DECLARE_CExtPmBridge_MEMBERS( CExtPropertyGridTipBar );

	CExtPropertyGridTipBar(
		CExtPropertyGridCtrl * pPropertyGridCtrl = NULL
		);
	virtual ~CExtPropertyGridTipBar();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif

	bool m_bAutoDeleteWindow:1;
	INT m_nUpperSizingGap;

	virtual bool Create(
		CWnd * pWndParent,
		bool bVisible = true
		);

	virtual CExtSafeString PropertyNameGet();
	virtual CExtSafeString PropertyDescriptionGet();
	bool DoSetCursor();
	virtual bool DoSetCursor( CPoint ptCursor );
	virtual bool IsResizingAllowed() const;
	virtual bool TrackerIsDrawn() const;
	virtual void TrackerDraw();
	virtual void TrackerFlush();
	virtual CRect TrackerGetRect();
	virtual INT HeightMinGet() const;
	virtual void HeightMinSet( INT nHeight );
	virtual INT HeightMaxGet() const;
	virtual void HeightMaxSet( INT nHeight );
	virtual INT HeightSeparatorGet() const;
	virtual void HeightSeparatorSet( INT nHeight );

	virtual void PropertyGridCtrlSet(
		CExtPropertyGridCtrl * pPropertyGridCtrl
		);
	virtual CExtPropertyGridCtrl * PropertyGridCtrlGet();
	const CExtPropertyGridCtrl * PropertyGridCtrlGet() const;
	CExtPropertyGridCtrl & PropertyGridCtrlGetRef();
	const CExtPropertyGridCtrl & PropertyGridCtrlGetRef() const;

protected:
	enum e_mouse_track_type_t
	{
	__EMTT_NONE = 0,
	__EMTT_RESIZING = 1,
	};
	e_mouse_track_type_t m_eMTT;
	bool m_bCanceling:1, m_bTrackerDrawn:1;

protected:
	virtual void DoPaint( CDC * pDC );

public:
	// Overrides
	//{{AFX_VIRTUAL(CExtPropertyGridTipBar)
	virtual void PostNcDestroy();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//{{AFX_MSG(CExtPropertyGridTipBar)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnPaint();
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
#if _MFC_VER < 0x700
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#else
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
#endif
	afx_msg LRESULT OnSizeParent( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

}; // class CExtPropertyGridTipBar

/////////////////////////////////////////////////////////////////////////////
// CExtPropertyGridCtrl window

#define __EXT_PROPERTY_GRID_CTRL_CLASS_NAME _T("ProfUIS-PropertyGridCtrl")
#define __EXT_PROPERTY_GRID_CTRL_WND_STYLES_DEFAULT (WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP)
#define __EXT_PROPERTY_GRID_CTRL_WND_EX_STYLES_DEFAULT 0

class __PROF_UIS_API CExtPropertyGridCtrl : public CWnd
{
public:
	DECLARE_DYNCREATE( CExtPropertyGridCtrl );
	CExtPropertyGridCtrl();
	virtual ~CExtPropertyGridCtrl();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif

	virtual bool Create(
		CWnd * pWndParent,
		UINT nDlgCtrlID = UINT(__EXT_MFC_IDC_STATIC),
		CRect rc = CRect( 0, 0, 0, 0 ),
		DWORD dwWndStyles = __EXT_PROPERTY_GRID_CTRL_WND_STYLES_DEFAULT,
		DWORD dwWndStylesEx = __EXT_PROPERTY_GRID_CTRL_WND_EX_STYLES_DEFAULT
		);
	static bool RegisterPropertyGridCtrlClass();

	CWnd * GetChildByRTC(
		CRuntimeClass * pRTC,
		CWnd * pWndStartSearchAfter = NULL
		);
	virtual void RedrawFocusDependentChildren();

	const CExtPropertyGridWnd * GetActiveGrid() const;
	virtual CExtPropertyGridWnd * GetActiveGrid();

	virtual void PropertyValueSynchronizeSimple(
		const CExtPropertyValue * pPV,
		bool bUseActiveGridCell = true,
		bool bUpdateActiveTreeGridWindow = false
		);
	virtual void PropertyStoreSynchronize();

	virtual CExtPropertyStore * PropertyStoreSet(
		CExtPropertyStore * pPS
		);
	virtual CExtPropertyStore * PropertyStoreGet();
	const CExtPropertyStore * PropertyStoreGet() const;
	CExtPropertyStore & PropertyStoreGetRef();
	const CExtPropertyStore & PropertyStoreGetRef() const;

	static const UINT g_nMsgInitPropertyGridCtrl;
protected:
	CExtSafeString m_strCommandProfile;
	CExtPropertyStore * m_pPS;
	static bool g_bPropertyGridCtrlClassRegistered;
	bool m_bHelperDirectCreateCall:1, m_bInPreTranslateMessage:1,
		m_bInConrolBarUpdate:1;
public:
	bool m_bEnabledControlBarUpdate:1;

	virtual void OnPgcStoreSelect( // added by Mr. Eyal Cohen
		CExtPropertyStore * pPS
		);
	virtual void OnPgcInputComplete(
		CExtPropertyGridWnd * pPGW,
		CExtPropertyItem * pPropertyItem
		);
	bool m_bCreateComboBoxBar:1, m_bCreateToolBar:1, m_bCreateTipBar:1,
		m_bCreateCategorizedGrid:1, m_bCreateSortedGrid:1,
		m_bEnableResetCommand:1;
	virtual bool OnPgcCreateBars();
	virtual bool OnPgcCreateGrids();
	virtual void OnPgcQueryGrids(
		CTypedPtrArray < CPtrArray, CExtPropertyGridWnd * > & arrGrids
		);
	virtual bool OnPgcProcessChildWindowCommand(
		CWnd * pWndChild,
		UINT nID,
		int nCode,
		void * pExtra,
		AFX_CMDHANDLERINFO * pHandlerInfo
		);
	virtual void OnPgcSynchronizeCompoundValue(
		CExtPropertyValueCompound * pCompoundValue
		);
	virtual void OnPgcSynchronizeCompoundValue(
		CExtPropertyValueMixedCompound * pMixedCompound
		);
	virtual void OnPgcContextMenuTrack(
		CWnd * pWndHit,
		CPoint ptScreen,
		CExtGridHitTestInfo * pHtInfo, // can be NULL
		CExtPropertyItem * pPropertyItem, // can be NULL
		CExtPropertyGridWnd * pPGW // can be NULL
		);
	virtual bool OnPgcContextMenuReconstruct(
		CExtPopupMenuWnd * pPopup,
		CWnd * pWndHit,
		CPoint ptScreen,
		CExtGridHitTestInfo * pHtInfo, // can be NULL
		CExtPropertyItem * pPropertyItem, // can be NULL
		CExtPropertyGridWnd * pPGW // can be NULL
		);
	virtual void OnPgcPropertyInputEnabled(
		CExtPropertyItem * pPropertyItem,
		bool bInputEnabled,
		bool bInputEnabledNestedFlags
		);
	virtual void OnPgcResetValue(
		CExtPropertyGridWnd * pPGW,
		CExtPropertyValue * pPropertyValue
		);
	virtual void RecalcLayout();

protected:
	bool m_bInitialized:1;
	virtual void _EnsureInitialized();
public:
	// Overrides
	//{{AFX_VIRTUAL(CExtPropertyGridCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	virtual BOOL OnCmdMsg( UINT nID, int nCode, void * pExtra, AFX_CMDHANDLERINFO * pHandlerInfo );
	//}}AFX_VIRTUAL

public:
	//{{AFX_MSG(CExtPropertyGridCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	afx_msg void OnSetFocus(CWnd* pOldWnd) ;
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMsgInitPropertyGridCtrl( WPARAM wParam, LPARAM lParam );

}; // class CExtPropertyGridCtrl

#endif // (!defined __EXT_MFC_NO_PROPERTYGRIDWND)

#endif /// __EXTPROPERTYGRIDWND_H



