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

#if (!defined __EXT_RIBBON_BAR_H)
#define __EXT_RIBBON_BAR_H

#if (!defined __EXT_MFC_NO_RIBBON_BAR)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_MENUCONTROLBAR_H)
	#include <ExtMenuControlBar.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXTCUSTOMIZE_H)
	#include <ExtCustomize.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

#if (!defined __EXT_POPUP_CTRL_MENU_H)
	#include <ExtPopupCtrlMenu.h>
#endif

#if (!defined __EXT_HOOK_H)
	#include <../Src/ExtHook.h>
#endif

#if (!defined __EXT_NC_FRAME_H)
	#include <ExtNcFrame.h>
#endif

#if (!defined __EXTTOOLBOXWND_H)
	#include <ExtToolBoxWnd.h>
#endif

#if (!defined __EXT_RESIZABLE_DIALOG_H)
	#include <ExtResizableDialog.h>
#endif

#if (!defined __EXT_LABEL_H)
	#include <ExtLabel.h>
#endif

#if (!defined __EXT_BUTTON_H)
	#include <ExtButton.h>
#endif

#if (!defined __EXT_COMBO_BOX_H)
	#include <ExtComboBox.h>
#endif

#if (!defined __AFXTEMPL_H__)
	#include <AfxTempl.h>
#endif

#if (!defined __EXT_SCROLLWND_H)
	#include <ExtScrollWnd.h>
#endif 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define __EXT_RIBBON_DEBUG_PAINTING

class CExtCmdProfile;
class CExtRibbonNode;
class CExtRibbonNodeGroup;
class CExtRibbonNodeToolButton;
class CExtRibbonNodeToolGroup;
class CExtRibbonNodeTabPage;
class CExtRibbonNodeTabPageCollection;
class CExtRibbonButton;
class CExtRibbonButtonFile;
class CExtRibbonButtonGroup;
class CExtRibbonButtonTabPage;
class CExtRibbonButtonToolGroup;
class CExtRibbonPage;
class CExtRibbonPopupMenuWnd;
class CExtRibbonBar;
class CExtRibbonButtonQuickAccessContentExpand;
class CExtRibbonNodeDialogLauncher;
class CExtRibbonButtonDialogLauncher;
class CExtRibbonGalleryInplaceScrollBar;
class CExtRibbonGalleryWnd;
class CExtRibbonGalleryPopupMenuWnd;
class CExtRibbonNodeGallery;
class CExtRibbonButtonGallery;
class CExtRibbonOptionsDialog;
class CExtRibbonOptionsPage;
class CExtRibbonOptionsTabsWnd;

//
// CExtRibbonPage::OnRibbonOptionsDialogTrack() parameter values
//
// display default page when ribbon options dialog is opened
#define __EXT_RIBBON_OPTIONS_DIALOG_PAGE_DEFAULT	0
// display quick access toolbar customization page
#define __EXT_RIBBON_OPTIONS_DIALOG_PAGE_CQATB		30525
// start value for user defined page identifiers
#define __EXT_RIBBON_OPTIONS_DIALOG_PAGE_UDS		1000

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonNode

class __PROF_UIS_API CExtRibbonNode : public CExtCustomizeCmdTreeNode
{
public:
	DECLARE_SERIAL( CExtRibbonNode );
protected:
	INT m_nRIL_VisualCurrent, m_nRIL_VisualMin, m_nRIL_VisualMax,
		m_nRIL_Effective, m_nRIL_EffectiveCollapsed;
	CArray < DWORD, DWORD > m_arrILEtoILV;
	virtual void _InitMembers();
public:
	UINT m_nTpmxAdditionalFlags;
	CExtCmdIcon m_iconBig, m_iconSmall;
	CExtRibbonNode(
		UINT nCmdIdBasic = 0L,
		UINT nCmdIdEffective = 0L,
		CExtRibbonNode * pParentNode = NULL,
		DWORD dwFlags = 0L,
		__EXT_MFC_SAFE_LPCTSTR strTextInToolbar = NULL,
		__EXT_MFC_SAFE_LPCTSTR strTextInMenu = NULL,
		__EXT_MFC_SAFE_LPCTSTR strTextUser = NULL,
		LPARAM lParam = 0L,
		CExtCmdIcon * pIconCustomized = NULL
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
		,
		INT nTextFieldWidth = 100,
		INT nDropDownWidth = -2, // (-1) - auto calc, (-2) - same as button area
		INT nDropDownHeightMax = 250
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
		);
	CExtRibbonNode(
		CExtRibbonNode & other
		);
//	CExtRibbonNode(
//		CExtCmdItem * pCmdItem, // NOT NULL!
//		CExtCmdIcon * pIconCustomized = NULL
//		);
	virtual ~CExtRibbonNode();
	CExtRibbonNode & operator = (
		CExtRibbonNode & other
		);
	virtual void AssignFromOther(
		CExtCustomizeCmdTreeNode & other
		);
//	virtual void AssignCmdProps(
//		CExtCmdItem * pCmdItem, // NOT NULL!
//		CExtCmdIcon * pIconCustomized = NULL
//		);
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif // _DEBUG
	virtual void Serialize( CArchive & ar );
	virtual CExtCmdIcon * GetIconPtrInMenu(
		CExtCmdItem * pCmdItem,
		bool bForceBasic = false
		);
	virtual CExtCmdIcon * GetIconPtrInToolbar(
		CExtCmdItem * pCmdItem,
		bool bForceBasic = false
		);

	bool RibbonWrapFromILE( INT nILE ) const;
	virtual INT RibbonILV_fromILE(
		INT nILE,
		bool * p_bIsWrap = NULL
		) const;
	virtual INT RibbonILV_Get(
		INT nType = 0 // -1 min, 0 current, 1 - max
		) const;
	virtual void RibbonILV_Set(
		INT nILV,
		INT nType = 0 // -1 min, 0 current, 1 - max
		);
	virtual INT RibbonILE_Get() const;
	virtual bool RibbonILE_Set( INT nILE ); // returns flag indicating whether ILV changed
	virtual INT RibbonILE_GetCollapsed() const;
	virtual void RibbonILE_SetCollapsed( INT nILE );
	virtual CArray < DWORD, DWORD > & RibbonILE_RuleArrayGet();
	virtual const CArray < DWORD, DWORD > & RibbonILE_RuleArrayGet() const;
	virtual void RibbonILE_RuleArraySet(
		const CArray < DWORD, DWORD > & arrILEtoILV
		);
	bool RibbonILE_RuleRemoveEntriesByILE(
		INT nMinILE,
		INT nMaxILE
		);
	bool RibbonILE_RuleRemoveEntriesByILV(
		bool bRemoveSmall,
		bool bRemoveNormal,
		bool bRemoveLarge
		);
	bool RibbonILE_RuleRemoveLargeILV();
	bool RibbonILE_RuleRemoveNormalILV();
	bool RibbonILE_RuleRemoveSmallILV();
	virtual bool Ribbon_InitCommandProfileIcon(
		CExtCmdProfile * pProfile,
		bool bInit
		);
	virtual CRuntimeClass * _OnRibbonGetButtonRTC();
	virtual CExtBarButton * OnRibbonCreateBarButton(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL
		);
	virtual bool Ribbon_InitBar(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL,
		bool bInsertThisNode = true
		);

	virtual bool OnGetCommandsListBoxInfo(
		CExtCustomizeCommandListBox * pLB, // IN (optional)
		CExtCustomizeSite * pSite = NULL, // IN (optional)
		CExtCmdItem * pCmdItem = NULL, // IN (optional)
		CExtSafeString * pStrLbText = NULL, // OUT (optional)
		CExtCmdIcon * pLbIcon = NULL, // OUT (optional)
		INT nDesiredIconWidth = 16, // IN (optional)
		INT nDesiredIconHeight = 16 // IN (optional)
		);

	virtual void OnSysColorChange(
		CExtPaintManager * pPM,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void OnSettingChange(
		UINT uFlags,
		__EXT_MFC_SAFE_LPCTSTR strSection,
		CExtPaintManager * pPM,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void OnDisplayChange(
		WPARAM _wParam,
		LPARAM _lParam,
		CExtPaintManager * pPM,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void OnThemeChanged(
		WPARAM _wParam,
		LPARAM _lParam,
		CExtPaintManager * pPM,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
}; // class CExtRibbonNode

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonNodeGroup

class __PROF_UIS_API CExtRibbonNodeGroup : public CExtRibbonNode
{
public:
	DECLARE_SERIAL( CExtRibbonNodeGroup );
	CExtRibbonNodeGroup(
		UINT nCmdIdBasic = 0L,
		CExtRibbonNode * pParentNode = NULL,
		DWORD dwFlags = 0L,
		__EXT_MFC_SAFE_LPCTSTR strTextInToolbar = NULL,
		LPARAM lParam = 0L
		);
	CExtRibbonNodeGroup(
		CExtRibbonNodeGroup & other
		);
	virtual ~CExtRibbonNodeGroup();
	virtual CRuntimeClass * _OnRibbonGetButtonRTC();

	virtual bool OnGetCommandsListBoxInfo(
		CExtCustomizeCommandListBox * pLB, // IN (optional)
		CExtCustomizeSite * pSite = NULL, // IN (optional)
		CExtCmdItem * pCmdItem = NULL, // IN (optional)
		CExtSafeString * pStrLbText = NULL, // OUT (optional)
		CExtCmdIcon * pLbIcon = NULL, // OUT (optional)
		INT nDesiredIconWidth = 16, // IN (optional)
		INT nDesiredIconHeight = 16 // IN (optional)
		);

}; // class CExtRibbonNodeGroup

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonNodeDialogLauncher

class __PROF_UIS_API CExtRibbonNodeDialogLauncher : public CExtRibbonNode
{
public:
	DECLARE_SERIAL( CExtRibbonNodeDialogLauncher );
	CExtRibbonNodeDialogLauncher(
		UINT nCmdIdBasic = 0L,
		CExtRibbonNode * pParentNode = NULL,
		LPARAM lParam = 0L
		);
	CExtRibbonNodeDialogLauncher(
		CExtRibbonNodeDialogLauncher & other
		);
	virtual ~CExtRibbonNodeDialogLauncher();
	virtual CRuntimeClass * _OnRibbonGetButtonRTC();
	virtual INT RibbonILV_Get(
		INT nType = 0 // -1 min, 0 current, 1 - max
		) const;
protected:
	virtual void _InitMembers();
public:
	virtual bool OnGetCommandsListBoxInfo(
		CExtCustomizeCommandListBox * pLB, // IN (optional)
		CExtCustomizeSite * pSite = NULL, // IN (optional)
		CExtCmdItem * pCmdItem = NULL, // IN (optional)
		CExtSafeString * pStrLbText = NULL, // OUT (optional)
		CExtCmdIcon * pLbIcon = NULL, // OUT (optional)
		INT nDesiredIconWidth = 16, // IN (optional)
		INT nDesiredIconHeight = 16 // IN (optional)
		);

}; // class CExtRibbonNodeDialogLauncher

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonNodeRightButtonsCollection

class __PROF_UIS_API CExtRibbonNodeRightButtonsCollection : public CExtRibbonNode
{
public:
	DECLARE_SERIAL( CExtRibbonNodeRightButtonsCollection );
	CExtRibbonNodeRightButtonsCollection(
		CExtRibbonNode * pParentNode = NULL
		);
	CExtRibbonNodeRightButtonsCollection(
		CExtRibbonNodeRightButtonsCollection & other
		);
	virtual ~CExtRibbonNodeRightButtonsCollection();
	virtual CRuntimeClass * _OnRibbonGetButtonRTC();
	virtual CExtBarButton * OnRibbonCreateBarButton(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL
		);
	virtual bool Ribbon_InitBar(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL,
		bool bInsertThisNode = true
		);

	virtual bool OnGetCommandsListBoxInfo(
		CExtCustomizeCommandListBox * pLB, // IN (optional)
		CExtCustomizeSite * pSite = NULL, // IN (optional)
		CExtCmdItem * pCmdItem = NULL, // IN (optional)
		CExtSafeString * pStrLbText = NULL, // OUT (optional)
		CExtCmdIcon * pLbIcon = NULL, // OUT (optional)
		INT nDesiredIconWidth = 16, // IN (optional)
		INT nDesiredIconHeight = 16 // IN (optional)
		);

}; // class CExtRibbonNodeRightButtonsCollection

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonNodeQuickAccessButtonsCollection

class __PROF_UIS_API CExtRibbonNodeQuickAccessButtonsCollection : public CExtRibbonNode
{
public:
	DECLARE_SERIAL( CExtRibbonNodeQuickAccessButtonsCollection );
	CExtRibbonNodeQuickAccessButtonsCollection(
		CExtRibbonNode * pParentNode = NULL
		);
	CExtRibbonNodeQuickAccessButtonsCollection(
		CExtRibbonNodeQuickAccessButtonsCollection & other
		);
	virtual ~CExtRibbonNodeQuickAccessButtonsCollection();
	virtual CRuntimeClass * _OnRibbonGetButtonRTC();
	virtual CExtBarButton * OnRibbonCreateBarButton(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL
		);
	virtual bool Ribbon_InitBar(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL,
		bool bInsertThisNode = true
		);

	virtual bool OnGetCommandsListBoxInfo(
		CExtCustomizeCommandListBox * pLB, // IN (optional)
		CExtCustomizeSite * pSite = NULL, // IN (optional)
		CExtCmdItem * pCmdItem = NULL, // IN (optional)
		CExtSafeString * pStrLbText = NULL, // OUT (optional)
		CExtCmdIcon * pLbIcon = NULL, // OUT (optional)
		INT nDesiredIconWidth = 16, // IN (optional)
		INT nDesiredIconHeight = 16 // IN (optional)
		);

}; // class CExtRibbonNodeQuickAccessButtonsCollection

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonNodeTabPageCollection

class __PROF_UIS_API CExtRibbonNodeTabPageCollection : public CExtRibbonNode
{
	INT m_nSelIdx;
public:
	DECLARE_SERIAL( CExtRibbonNodeTabPageCollection );
	CExtRibbonNodeTabPageCollection(
		CExtRibbonNode * pParentNode = NULL
		);
	CExtRibbonNodeTabPageCollection(
		CExtRibbonNodeTabPageCollection & other
		);
	virtual ~CExtRibbonNodeTabPageCollection();
	virtual INT PageSelectionGet() const;
	virtual void PageSelectionSet( INT nSelIdx );
	virtual void AssignFromOther(
		CExtCustomizeCmdTreeNode & other
		);
	virtual void Serialize( CArchive & ar );
	virtual CRuntimeClass * _OnRibbonGetButtonRTC();
	virtual CExtBarButton * OnRibbonCreateBarButton(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL
		);
	virtual bool Ribbon_InitBar(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL,
		bool bInsertThisNode = true
		);

	virtual bool OnGetCommandsListBoxInfo(
		CExtCustomizeCommandListBox * pLB, // IN (optional)
		CExtCustomizeSite * pSite = NULL, // IN (optional)
		CExtCmdItem * pCmdItem = NULL, // IN (optional)
		CExtSafeString * pStrLbText = NULL, // OUT (optional)
		CExtCmdIcon * pLbIcon = NULL, // OUT (optional)
		INT nDesiredIconWidth = 16, // IN (optional)
		INT nDesiredIconHeight = 16 // IN (optional)
		);

}; // class CExtRibbonNodeTabPageCollection

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonNodeTabPage

class __PROF_UIS_API CExtRibbonNodeTabPage : public CExtRibbonNode
{
public:
	DECLARE_SERIAL( CExtRibbonNodeTabPage );
	CExtRibbonNodeTabPage(
		UINT nCmdIdBasic = 0L,
		CExtRibbonNode * pParentNode = NULL,
		__EXT_MFC_SAFE_LPCTSTR strTextInToolbar = NULL,
		LPARAM lParam = 0L
		);
	CExtRibbonNodeTabPage(
		CExtRibbonNodeTabPage & other
		);
	virtual ~CExtRibbonNodeTabPage();
	virtual CRuntimeClass * _OnRibbonGetButtonRTC();
	virtual CExtBarButton * OnRibbonCreateBarButton(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL
		);
	virtual bool Ribbon_InitBar(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL,
		bool bInsertThisNode = true
		);

	virtual bool OnGetCommandsListBoxInfo(
		CExtCustomizeCommandListBox * pLB, // IN (optional)
		CExtCustomizeSite * pSite = NULL, // IN (optional)
		CExtCmdItem * pCmdItem = NULL, // IN (optional)
		CExtSafeString * pStrLbText = NULL, // OUT (optional)
		CExtCmdIcon * pLbIcon = NULL, // OUT (optional)
		INT nDesiredIconWidth = 16, // IN (optional)
		INT nDesiredIconHeight = 16 // IN (optional)
		);

}; // class CExtRibbonNodeTabPage

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonNodeToolGroup

class __PROF_UIS_API CExtRibbonNodeToolGroup : public CExtRibbonNodeGroup
{
public:
	DECLARE_SERIAL( CExtRibbonNodeToolGroup );
	CExtRibbonNodeToolGroup(
		UINT nCmdIdBasic = 0L,
		CExtRibbonNode * pParentNode = NULL,
		DWORD dwFlags = 0L,
		__EXT_MFC_SAFE_LPCTSTR strTextInToolbar = NULL,
		LPARAM lParam = 0L
		);
	CExtRibbonNodeToolGroup(
		CExtRibbonNodeToolGroup & other
		);
	virtual ~CExtRibbonNodeToolGroup();
	virtual CRuntimeClass * _OnRibbonGetButtonRTC();
}; // class CExtRibbonNodeButtonToolGroup

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonNodeToolButton

class __PROF_UIS_API CExtRibbonNodeToolButton : public CExtRibbonNode
{
public:
	DECLARE_SERIAL( CExtRibbonNodeToolButton );
	CExtRibbonNodeToolButton(
		UINT nCmdIdBasic = 0L,
		CExtRibbonNode * pParentNode = NULL,
		DWORD dwFlags = 0L,
		__EXT_MFC_SAFE_LPCTSTR strTextInToolbar = NULL,
		LPARAM lParam = 0L
		);
	CExtRibbonNodeToolButton(
		CExtRibbonNodeToolButton & other
		);
	virtual ~CExtRibbonNodeToolButton();
	virtual CRuntimeClass * _OnRibbonGetButtonRTC();
	virtual INT RibbonILV_Get(
		INT nType = 0 // -1 min, 0 current, 1 - max
		) const;
protected:
	virtual void _InitMembers();
}; // class CExtRibbonNodeToolButton

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonNodeLabel

class __PROF_UIS_API CExtRibbonNodeLabel : public CExtRibbonNode
{
public:
	DECLARE_SERIAL( CExtRibbonNodeLabel );
	CExtRibbonNodeLabel(
		UINT nCmdIdBasic = 0L,
		CExtRibbonNode * pParentNode = NULL,
		DWORD dwFlags = 0L,
		__EXT_MFC_SAFE_LPCTSTR strTextInToolbar = NULL,
		LPARAM lParam = 0L
		);
	CExtRibbonNodeLabel(
		CExtRibbonNodeLabel & other
		);
	virtual ~CExtRibbonNodeLabel();
	virtual CRuntimeClass * _OnRibbonGetButtonRTC();
	virtual INT RibbonILE_Get() const;
	virtual INT RibbonILV_Get(
		INT nType = 0 // -1 min, 0 current, 1 - max
		) const;
protected:
	virtual void _InitMembers();
}; // class CExtRibbonNodeLabel

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonButtonQuickAccessContentExpand

class __PROF_UIS_API CExtRibbonButtonQuickAccessContentExpand : public CExtBarButton
{
public:
	DECLARE_DYNCREATE( CExtRibbonButtonQuickAccessContentExpand );
	CExtRibbonButtonQuickAccessContentExpand(
		CExtRibbonPage * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		);
	virtual ~CExtRibbonButtonQuickAccessContentExpand();
	virtual CExtSafeString GetText() const;
	virtual bool IsDisabled() const;
	virtual bool IsSeparator() const;
	virtual bool IsNoRibbonLayout() const;
	virtual INT RibbonILV_Get(
		INT nType = 0 // -1 min, 0 current, 1 - max
		) const;
	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);
	virtual void PaintCompound(
		CDC & dc,
		bool bPaintParentChain,
		bool bPaintChildren,
		bool bPaintOneNearestChildrenLevelOnly
		);
	virtual void OnUpdateCmdUI(
		CWnd * pTarget,
		BOOL bDisableIfNoHndler,
		int nIndex
		);
	virtual bool IsAbleToTrackMenu(
		bool bCustomizeMode = false
		) const;
	virtual UINT OnTrackPopup(
		CPoint point,
		bool bSelectAny,
		bool bForceNoAnimation
		);
}; // class CExtRibbonButtonQuickAccessContentExpand

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonButtonDialogLauncher

class __PROF_UIS_API CExtRibbonButtonDialogLauncher : public CExtBarButton
{
public:
	DECLARE_DYNCREATE( CExtRibbonButtonDialogLauncher );
	CExtRibbonButtonDialogLauncher(
		CExtRibbonPage * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		);
	virtual ~CExtRibbonButtonDialogLauncher();
	virtual bool IsRibbonPaintingMode() const;
	virtual CExtSafeString GetText() const;
	virtual CExtCmdIcon * GetIconPtr();
	virtual bool IsSeparator() const;
	virtual bool IsVisible() const;
	virtual bool IsNoRibbonLayout() const;
	virtual INT RibbonILV_Get(
		INT nType = 0 // -1 min, 0 current, 1 - max
		) const;
	virtual CSize RibbonILV_CalcSize(
		CDC & dc,
		INT nILV = -1 // -1 use current visual level
		) const;
	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);
	virtual void PaintCompound(
		CDC & dc,
		bool bPaintParentChain,
		bool bPaintChildren,
		bool bPaintOneNearestChildrenLevelOnly
		);
}; // class CExtRibbonButtonDialogLauncher

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonNodeMdiRightButtons

class __PROF_UIS_API CExtRibbonNodeMdiRightButtons : public CExtRibbonNode
{
public:
	DECLARE_SERIAL( CExtRibbonNodeMdiRightButtons );
	CExtRibbonNodeMdiRightButtons(
		CExtRibbonNode * pParentNode = NULL,
		LPARAM lParam = 0L
		);
	CExtRibbonNodeMdiRightButtons(
		CExtRibbonNodeMdiRightButtons & other
		);
	virtual ~CExtRibbonNodeMdiRightButtons();
	virtual CRuntimeClass * _OnRibbonGetButtonRTC();
	virtual CExtBarButton * OnRibbonCreateBarButton(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL
		);
	virtual bool Ribbon_InitBar(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL,
		bool bInsertThisNode = true
		);

	virtual bool OnGetCommandsListBoxInfo(
		CExtCustomizeCommandListBox * pLB, // IN (optional)
		CExtCustomizeSite * pSite = NULL, // IN (optional)
		CExtCmdItem * pCmdItem = NULL, // IN (optional)
		CExtSafeString * pStrLbText = NULL, // OUT (optional)
		CExtCmdIcon * pLbIcon = NULL, // OUT (optional)
		INT nDesiredIconWidth = 16, // IN (optional)
		INT nDesiredIconHeight = 16 // IN (optional)
		);

}; // class CExtRibbonNodeMdiRightButtons

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonButton

class __PROF_UIS_API CExtRibbonButton : public CExtBarButton
{
public:
	DECLARE_DYNCREATE( CExtRibbonButton );
	CExtRibbonButton(
		CExtRibbonPage * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		);
	virtual ~CExtRibbonButton();
	virtual bool IsRibbonPaintingMode() const;
	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);
	CExtRibbonPage * GetRibbonPage();
	const CExtRibbonPage * GetRibbonPage() const;
	virtual CRect OnRibbonGetContentPadding() const;
	virtual INT OnRibbonGetSeparatorExtent( bool bHorz );
	virtual bool IsAbleToTrackMenu(
		bool bCustomizeMode = false
		) const;
	virtual void OnUpdateCmdUI(
		CWnd * pTarget,
		BOOL bDisableIfNoHndler,
		int nIndex
		);
	virtual CExtCmdIcon * GetIconPtr();
	virtual CExtSafeString GetText() const;
	virtual void PaintCompound(
		CDC & dc,
		bool bPaintParentChain,
		bool bPaintChildren,
		bool bPaintOneNearestChildrenLevelOnly
		);
	virtual CSize OnRibbonCalcLargeTextSize( CDC & dc );
	virtual CRect RectDropDown() const;
	virtual CRect RectWithoutDropDown() const;
}; // class CExtRibbonButton

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonButtonTabPage

class __PROF_UIS_API CExtRibbonButtonTabPage : public CExtBarButton
{
public:
	DECLARE_DYNCREATE( CExtRibbonButtonTabPage );
	CExtRibbonButtonTabPage(
		CExtRibbonPage * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		);
	virtual ~CExtRibbonButtonTabPage();
	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);
	CExtRibbonPage * GetRibbonPage();
	const CExtRibbonPage * GetRibbonPage() const;
	virtual bool IsAbleToTrackMenu(
		bool bCustomizeMode = false
		) const;
	virtual void OnClick(
		CPoint point,
		bool bDown
		);
	virtual bool OnDblClick(
		CPoint point
		);
	virtual void OnDeliverCmd();
	virtual bool OnKeyTipInvokeAction(
		bool & bContinueKeyTipMode
		);
	virtual void OnUpdateCmdUI(
		CWnd * pTarget,
		BOOL bDisableIfNoHndler,
		int nIndex
		);
	virtual bool IsSeparator() const;
	virtual CExtCmdIcon * GetIconPtr();
	virtual CExtSafeString GetText() const;
	virtual BYTE Get2007SeparatorAlpha() const;
	virtual void PaintCompound(
		CDC & dc,
		bool bPaintParentChain,
		bool bPaintChildren,
		bool bPaintOneNearestChildrenLevelOnly
		);
	virtual bool IsSelectedRibbonPage() const;
}; // class CExtRibbonButtonTabPage

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonButtonGroup

class __PROF_UIS_API CExtRibbonButtonGroup : public CExtRibbonButton
{
protected:
	bool m_bTopCollapsedState:1, m_bSettingLayoutRect:1, m_bRibbonPopupVisible:1;
	CRect m_rcCaptionTextAlignmentDLB;
public:
	DECLARE_DYNCREATE( CExtRibbonButtonGroup );
	CExtRibbonButtonGroup(
		CExtRibbonPage * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		);
	virtual ~CExtRibbonButtonGroup();
	virtual bool AnimationClient_OnQueryEnabledState(
		INT eAPT // __EAPT_*** animation type
		) const;
	virtual UINT OnTrackPopup(
		CPoint point,
		bool bSelectAny,
		bool bForceNoAnimation
		);
	virtual bool IsAbleToTrackMenu(
		bool bCustomizeMode = false
		) const;
	virtual bool TopCollapsedStateGet() const;
	virtual void TopCollapsedStateSet( bool bTopCollapsedState );
	virtual bool RibbonILE_SetCollapsed( INT nILE ); // returns flag indicating whether collapsed state changed
	virtual bool OnRibbonIsCaptionVisible() const;
	virtual CRect OnRibbonGetCaptionRect() const;
	virtual void OnUpdateCmdUI(
		CWnd * pTarget,
		BOOL bDisableIfNoHndler,
		int nIndex
		);
	virtual __EXT_MFC_INT_PTR OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		);
	virtual void PaintCompound(
		CDC & dc,
		bool bPaintParentChain,
		bool bPaintChildren,
		bool bPaintOneNearestChildrenLevelOnly
		);
	virtual INT OnRibbonGetOuterGroupDistance(
		bool bDistanceBefore
		) const;
	virtual CRect OnRibbonGetContentPadding() const;
	virtual void SetLayoutRect( CDC &dc, const RECT & rectButton );
	virtual CSize RibbonILV_CalcSize(
		CDC & dc,
		INT nILV = -1 // -1 use current visual level
		) const;
	virtual CSize OnRibbonCalcCollapsedGroupSize( CDC & dc );
	virtual CSize OnRibbonCalcLayout(
		CDC & dc,
		CPoint & ptCalcLayoutOffset,
		bool bSetupRects
		);
	virtual CRect OnRibbonGetContentAlignmentRect() const;
	virtual void OnRibbonGetSimpleGroupContentAlignmentFlags(
		CDC & dc,
		bool & bUseHorizontalAlignment,
		bool & bUseVerticalAlignment
		) const;
	virtual void OnRibbonAlignDialogLauncherButtons( CDC & dc );
	virtual void OnRibbonAlignContent( CDC & dc );
	virtual void OnRibbonPopupShow(
		CExtRibbonPopupMenuWnd * pPopup,
		bool bShow
		);
	virtual bool OnQueryFlatTrackingEnabled() const;
	virtual bool IsPressed() const;
	virtual CRect GetCaptionTextAlignmentRect() const;
	virtual void OnKeyTipTrackingQuery(
		bool bShow,
		CExtCustomizeCmdKeyTip * pKeyTipChain,
		HDWP & hPassiveModeDWP
		);
	virtual void OnKeyTipTrackingQueryNested(
		bool bShow,
		CExtCustomizeCmdKeyTip * pKeyTipChain,
		bool bApplyToThisItem,
		HDWP & hPassiveModeDWP
		);
}; // class CExtRibbonButtonGroup

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonButtonToolGroup

class __PROF_UIS_API CExtRibbonButtonToolGroup : public CExtRibbonButtonGroup
{
public:
	DECLARE_DYNCREATE( CExtRibbonButtonToolGroup );
	CExtRibbonButtonToolGroup(
		CExtRibbonPage * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		);
	virtual ~CExtRibbonButtonToolGroup();
	virtual void PaintCompound(
		CDC & dc,
		bool bPaintParentChain,
		bool bPaintChildren,
		bool bPaintOneNearestChildrenLevelOnly
		);
	virtual CSize OnRibbonCalcLayout(
		CDC & dc,
		CPoint & ptCalcLayoutOffset,
		bool bSetupRects
		);
	virtual void OnRibbonGetToolGroupContentAlignmentFlags(
		CDC & dc,
		bool & bUseHorizontalAlignment,
		bool & bUseVerticalAlignment,
		bool & bUseToolRowVerticalAlignment
		) const;
	virtual void OnRibbonAlignContent( CDC & dc );
}; // class CExtRibbonButtonToolGroup

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonPage

class __PROF_UIS_API CExtRibbonPage
	: public CExtMenuControlBar
	, public CExtCustomizeSite
	
{
public:
	DECLARE_DYNCREATE( CExtRibbonPage );
// Construction
public:
	CExtRibbonPage();
	virtual ~CExtRibbonPage();

	virtual CExtPaintManager * PmBridge_GetPM() const;
	virtual void PmBridge_OnPaintManagerChanged(
		CExtPaintManager * pGlobalPM
		);

// Attributes
public:

	HWND m_hWndSrcRibbonPage, m_hWndParentRibbonPage;
	bool m_bHelperPopupMode:1, m_bHelperAutoHideMode:1;
	CExtRibbonPopupMenuWnd * m_pPopupPageMenuGroup;
	CExtRibbonPopupMenuWnd * m_pPopupPageMenuAutoHide;

	virtual INT Ribbon_PageSelectionGet() const;
	virtual bool Ribbon_PageSelectionSet(
		INT nSelIdx,
		bool bEnableAnimation = true
		);

	CExtRibbonNode * Ribbon_GetRootNode();
	const CExtRibbonNode * Ribbon_GetRootNode() const;

	CExtRibbonPage * GetMainRibbonPage();
	const CExtRibbonPage * GetMainRibbonPage() const;
	CExtRibbonPage * GetParentRibbonPage();
	const CExtRibbonPage * GetParentRibbonPage() const;

	virtual bool CustomizeStateSerialize(
		CArchive & ar,
		bool bEnableThrowExceptions = false
		);
	virtual CWnd * GetCustomizeAccelUpdatingTargetWnd();
	virtual CWnd * GetCustomizeTopWnd();
	virtual void OnRibbonBuildCommandCategories(
		CExtCustomizeCmdTreeNode * pNode,
		__EXT_MFC_SAFE_LPCTSTR strCommandCategoryName = NULL
		);

protected:
	bool m_bHelperDwmPaintingMode:1;
	CExtRibbonNode * m_pRibbonNode;
	CArray < CExtRibbonButtonGroup *, CExtRibbonButtonGroup * > m_arrGroupButtons;
	CArray < CExtRibbonButtonTabPage *, CExtRibbonButtonTabPage * > m_arrTabPageButtons;
	CArray < CExtBarButton *, CExtBarButton * > m_arrRightButtons;
	CArray < CExtBarButton *, CExtBarButton * > m_arrQuickAccessButtons;
	CExtRibbonButtonQuickAccessContentExpand * m_pQACEB;
	INT m_nMaxILE, m_nMinILE, m_nRslaLastContentWidth,
		m_nRslaLastDesiredWidth, m_nRslaLastILE;
	bool m_bRibbonSimplifiedLayoutAlgorithm:1, m_bRslaLastStateHasCollapsedGroups:1,
		m_bRibbonSimplifiedLayoutDelayedFlush:1;
	virtual void _RemoveAllButtonsImpl();
	virtual void _RibbonPageRslaResetStateData();
public:
	virtual void Ribbon_UpdateLayout(
		bool bDelay = false
		);
	virtual bool Ribbon_UseSimplifiedLayoutAlgorithmGet() const;
	virtual void Ribbon_UseSimplifiedLayoutAlgorithmSet( bool bSet );

// Operations
public:
	virtual void OnRegisterAdditionalToolBars();
	virtual bool RibbonLayout_IsDwmCaptionIntegration() const;

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtRibbonPage)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual bool OnHookWndMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		UINT nMessage,
		WPARAM & wParam,
		LPARAM & lParam
		);
	virtual bool DoDragCmdNode(
		ICustomizeDropSource * pCustomizeDragSource,
		CExtCustomizeCmdTreeNode * pNodeI,
		CExtCustomizeCmdTreeNode * pNodeC,
		RECT & rcItem
		);

	mutable CExtPopupScreenTipWnd m_wndScreenTip;
	virtual CExtPopupMenuTipWnd * OnAdvancedPopupMenuTipWndGet(
		CObject * pHelperSrc = NULL,
		LPARAM lParam = 0
		) const;
	virtual void OnAdvancedPopupMenuTipWndDisplay(
		CExtPopupMenuTipWnd & _ATTW,
		const RECT & rcExcludeArea,
		__EXT_MFC_SAFE_LPCTSTR strTipText,
		CObject * pHelperSrc = NULL,
		LPARAM lParam = 0
		) const;
public:
	virtual CExtBarContentExpandButton * OnCreateBarRightBtn();
	virtual CExtBarButton * OnCreateBarCommandBtn(
		UINT nCmdID,
		UINT nStyle = 0
		);
#ifdef _DEBUG
	virtual BOOL LoadToolBar(
		__EXT_MFC_SAFE_LPCTSTR lpszResourceName,
		COLORREF clrTransparent = RGB(192,192,192)
		);
	virtual BOOL InsertButton(
		int nPos = -1, // append
		UINT nCmdID = ID_SEPARATOR,
		BOOL bDoRecalcLayout = TRUE
		);
	virtual BOOL RemoveButton(
		int nPos,
		BOOL bDoRecalcLayout = TRUE
		);
#endif // _DEBUG
	virtual BOOL InsertSpecButton(
		int nPos, // -1 - append
		CExtBarButton * pButton,
		BOOL bDoRecalcLayout = TRUE
		);

public:
	virtual void OnFlatTrackingStart(
		HDWP & hPassiveModeDWP
		);
	virtual void OnFlatTrackingStop();

	virtual void _CheckHitTestablePoint(
		CPoint & point
		);
	virtual bool _UpdateHoverButton(
		CPoint point = CPoint(-1,-1), // default is use ::GetCursorPos()
		bool bEnableUpdateWindow = true
		);
	virtual void _DelayUpdateMenuBar();
	virtual BOOL _UpdateMenuBar(
		BOOL bDoRecalcLayout = TRUE
		);
	virtual BOOL _InstallMdiDocButtons(
		BOOL bDoRecalcLayout = TRUE
		);
	virtual void _CloseCustomMenus();
	bool _GetFullRowMode() const;
	virtual BOOL SetButtons(
		const UINT * lpIDArray = NULL,
		int nIDCount = 0
		);
	virtual BOOL SetButtons(
		CExtCustomizeCmdTreeNode * pNode
		);

	virtual void _CalcInsideRect(
		CRect & rect,
		BOOL bHorz
		) const;
	virtual CSize _CalcLayout(
		DWORD dwMode,
		int nLength = -1
		);
	virtual bool _AdjustBGInfo();
	virtual void _RecalcPositionsImpl();

	virtual void DoPaintNC( CDC * pDC );
	virtual void DoEraseBk( CDC * pDC );
	virtual void DoPaint( CDC * pDC );
	virtual CFont * OnGetToolbarFont(
		bool bVert,
		bool bMeasureFont
		);

	virtual void Ribbon_OnRecalcLayout();

	virtual INT RibbonRightButton_GetIndexOf( const CExtBarButton * pTBB ) const;
	virtual CExtBarButton * RibbonRightButton_GetAt( INT nPos );
	const CExtBarButton * RibbonRightButton_GetAt( INT nPos ) const;
	virtual INT RibbonRightButton_GetCount() const;

protected:
	bool m_bRibbonPageIsExpandedMode:1;
public:
	virtual bool RibbonPage_ExpandedModeGet() const;
	virtual void RibbonPage_ExpandedModeSet(
		bool bRibbonPageIsExpandedMode,
		bool bRecalcLayout
		);
protected:
	bool m_bRibbonQuickAccessBarIsAboveTheRibbon:1;
public:
	virtual bool RibbonQuickAccessBar_AboveTheRibbonGet() const;
	virtual void RibbonQuickAccessBar_AboveTheRibbonSet(
		bool bRibbonQuickAccessBarIsAboveTheRibbon,
		bool bRecalcLayout
		);
	virtual INT RibbonQuickAccessBar_GetBottomHeight() const;
	virtual INT RibbonQuickAccessBar_GetButtonsAlignment() const;

	virtual INT RibbonQuickAccessButton_GetIndexOf( const CExtBarButton * pTBB ) const;
	virtual CExtBarButton * RibbonQuickAccessButton_GetAt( INT nPos );
	const CExtBarButton * RibbonQuickAccessButton_GetAt( INT nPos ) const;
	virtual INT RibbonQuickAccessButton_GetCount() const;

	virtual INT RibbonTabPageButton_GetIndexOf( const CExtRibbonButtonTabPage * pTBB ) const;
	virtual CExtRibbonButtonTabPage * RibbonTabPageButton_GetAt( INT nPos );
	const CExtRibbonButtonTabPage * RibbonTabPageButton_GetAt( INT nPos ) const;
	virtual INT RibbonTabPageButton_GetCount() const;

	virtual INT RibbonGroupButton_GetIndexOf( const CExtRibbonButtonGroup * pTBB ) const;
	virtual CExtRibbonButtonGroup * RibbonGroupButton_GetAt( INT nPos );
	const CExtRibbonButtonGroup * RibbonGroupButton_GetAt( INT nPos ) const;
	virtual INT RibbonGroupButton_GetCount() const;

	virtual void RibbonLayout_IleReset(
		CExtBarButton * pStartTBB = NULL,
		INT nILE = __EXT_RIBBON_ILE_MAX,
		bool bForceShowChildren = false,
		bool bForceHideChildren = false,
		bool bForceCleanCollapsedState = false
		);
	virtual INT RibbonLayout_FindIlvInGroup(
		CExtBarButton * pStartTBB,
		INT nILE,
		bool bEnableCollapsing
		);
	virtual INT RibbonLayout_FindIlvChangingCount(
		CExtBarButton * pStartTBB = NULL,
		INT nILE = __EXT_RIBBON_ILE_MAX,
		INT nMaxCount = 32767,
		bool bUp = false,
		bool bOneCollapsedLevelOnly = false
		);
	virtual CRect _OnRibbonContentPaddingGetOuter() const;
	virtual CRect _OnRibbonContentPaddingGetGroup(
		const CExtRibbonButtonGroup * pGroupTBB
		) const;
	virtual INT _OnRibbonContentPaddingGetOuterGroupDistance(
		const CExtRibbonButtonGroup * pGroupTBB,
		bool bDistanceBefore
		) const;
	virtual CRect _OnRibbonContentPaddingGetButton(
		const CExtBarButton * pTBB
		) const;
	virtual INT OnRibbonGetSeparatorExtent(
		CExtBarButton * pTBB,
		bool bHorz
		);
	virtual void Ribbon_OnCalcMinMaxILE();
	virtual CSize RibbonLayout_Calc(
		CDC & dc,
		CPoint ptLayoutOffset = CPoint( 0, 0 ),
		bool bSetupRects = true,
		INT * p_nCountLayouted = NULL
		);
	virtual void RibbonLayout_AlignPageContent(
		CDC & dc,
		CRect rcAlignContent
		);
	virtual void RibbonLayout_AlignTabLineContent(
		CDC & dc,
		CRect rcAlignContent
		);
	BYTE m_byteHelper2007SeparatorAlpha;
	virtual BYTE RibbonLayout_Get2007SeparatorAlpha(
		const CExtRibbonButtonTabPage * pTBB
		) const;
	virtual bool RibbonLayout_IsFrameIntegrationEnabled() const;
	virtual void RibbonLayout_AdjustTabLineRect( CRect & rcTabLine );
	virtual INT RibbonLayout_GetGroupHeight(
		CExtRibbonButtonGroup * pGroupTBB
		) const;
	virtual CRect RibbonLayout_GetGroupContentAlignmentRect(
		const CExtRibbonButtonGroup * pGroupTBB
		) const;
	virtual bool RibbonLayout_GroupCaptionIsVisible(
		const CExtRibbonButtonGroup * pGroupTBB
		) const;
	virtual CRect RibbonLayout_GroupCaptionRectGet(
		const CExtRibbonButtonGroup * pGroupTBB
		) const;
	virtual bool RibbonLayout_GroupCaptionIsTopAligned(
		const CExtRibbonButtonGroup * pGroupTBB
		) const;
	virtual INT RibbonLayout_GroupCaptionGetHeight(
		const CExtRibbonButtonGroup * pGroupTBB
		) const;
	virtual void RibbonLayout_GetSimpleGroupContentAlignmentFlags(
		CDC & dc,
		const CExtRibbonButtonGroup * pGroupTBB,
		bool & bUseHorizontalAlignment,
		bool & bUseVerticalAlignment
		) const;
	virtual void RibbonLayout_GetToolGroupContentAlignmentFlags(
		CDC & dc,
		const CExtRibbonButtonToolGroup * pToolGroupTBB,
		bool & bUseHorizontalAlignment,
		bool & bUseVerticalAlignment,
		bool & bUseToolRowVerticalAlignment
		) const;

	virtual bool IsForceHoverWhenMenuTracking() const;
	virtual bool OnQueryHoverBasedMenuTracking(
		const CExtBarButton * pTBB
		) const;
	virtual bool OnDeliverCmd( CExtBarButton * pTBB );

	virtual INT RibbonLayout_GetTabIntersectionHeight() const;
	virtual INT RibbonLayout_GetTabLineHeight() const;
	virtual INT RibbonLayout_GetFrameCaptionHeight(
		INT * p_nTopBorderHeight = NULL
		) const;
	virtual INT RibbonLayout_GetBottomLineHeight() const;

	virtual bool _IsSimplifiedDropDownButtons() const;
	virtual void _RedrawOnPosChanged();
	virtual void _ActivateOnClick();
	virtual BOOL TranslateMainFrameMessage(MSG* pMsg);
	virtual INT OnCalcFlatTrackingIndex(
		/*__EXT_MFC_SAFE_TCHAR*/ INT vkTCHAR,
		INT nStartSearchIndex
		);
	virtual bool OnCalcFlatTrackingIndexCheckPass(
		/*__EXT_MFC_SAFE_TCHAR*/ INT vkTCHAR,
		INT nPassIndex,
		const CExtBarButton * pPrevTBB,
		const CExtBarButton * pNextTBB
		) const;
	virtual bool OnCalcFlatTrackingTabOrder(
		CTypedPtrArray < CPtrArray, CExtBarButton * > & arrTabOrder
		);
	virtual bool OnCalcFlatTrackingIndexCheckIntersectionEnabled(
		CExtBarButton * pTBB,
		/*__EXT_MFC_SAFE_TCHAR*/ INT vkTCHAR
		);

	virtual bool SetupHookWndSink(
		HWND hWnd,
		bool bRemove = false,
		bool bAddToHead = false
		);
	virtual	bool _OnMouseMoveMsg(UINT nFlags, CPoint point);
	virtual bool OnRibbonProcessMouseWheel( UINT fFlags, short zDelta, CPoint point );

	virtual void OnRibbonGalleryInitContent(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtRibbonButtonGallery * pRibbonGalleryTBB
		);
	virtual void OnRibbonGalleryItemSelEndOK(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtRibbonButtonGallery * pRibbonGalleryTBB,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI
		);
	virtual void OnRibbonGalleryItemInsert(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtRibbonButtonGallery * pRibbonGalleryTBB,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI
		);
	virtual void OnRibbonGalleryItemRemove(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtRibbonButtonGallery * pRibbonGalleryTBB,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI
		);
	virtual void OnRibbonGalleryItemActivate(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtRibbonButtonGallery * pRibbonGalleryTBB,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_Old,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_New
		);
	virtual void OnRibbonGalleryItemSelChange(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtRibbonButtonGallery * pRibbonGalleryTBB,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_Old,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_New
		);
	virtual void OnRibbonGalleryItemHoverChange(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtRibbonButtonGallery * pRibbonGalleryTBB,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_Old,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_New
		);

	virtual void OnRibbonFileMenuButtonQuery(
		CExtPopupMenuWnd::RIBBONFILEMENUBUTTONQUERY & _rfmbq,
		CExtCmdIcon & _icon,
		CExtSafeString & sText
		);
	virtual void OnRibbonFileMenuButtonInvocation(
		CExtPopupMenuWnd::RIBBONFILEMENUBUTTONINVOCATION & _rfmbi
		);
	virtual void OnRibbonExitCommand();
	virtual void OnRibbonOptionsDialogTrack(
		UINT nSelectedPageID = __EXT_RIBBON_OPTIONS_DIALOG_PAGE_DEFAULT
		);
	virtual void OnRibbonOptionsDialogInitPageRTCs(
		CExtRibbonOptionsDialog & dlgRibbonOptions
		);

	virtual CExtCustomizeSite * GetCustomizeSite();

#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	virtual CSize OnPopupListBoxCalcItemExtraSizes(
		const CExtBarButton * pTBB,
		const CExtCustomizeCmdTreeNode * pNode
		) const;
	virtual bool OnPopupListBoxInitContent(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CListBox & wndListBox
		);
	virtual CSize OnPopupListBoxAdjustSize(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CListBox & wndListBox
		);
	virtual CSize OnPopupListBoxMeasureTrackSize(
		const CExtBarButton * pTBB,
		const CExtCustomizeCmdTreeNode * pNode
		) const;
	virtual bool OnPopupListBoxItemDraw(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CListBox & wndListBox,
		LPDRAWITEMSTRUCT pDIS
		);
	virtual bool OnPopupListBoxItemMeasure(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CListBox & wndListBox,
		LPMEASUREITEMSTRUCT pMIS
		);
	virtual bool OnPopupListBoxSelChange(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CListBox & wndListBox
		);
	virtual bool OnPopupListBoxSelEndOK(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CListBox & wndListBox
		);
	virtual bool OnPopupListBoxSelEndCancel(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CListBox & wndListBox
		);
	virtual bool OnPopupListBoxClose(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CListBox & wndListBox
		);
	virtual bool OnPopupListBoxGetStyles(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		DWORD & dwListBoxStyles
		) const;
	virtual bool OnPopupDatePickerGetStyles(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CSize & szCalendarDimensions,
		DWORD & dwDatePickerStyle
		) const;

	virtual bool OnTextFieldVerify(
		CExtBarTextFieldButton * pTextFieldTBB,
		CExtCustomizeCmdTreeNode * pNode,
		__EXT_MFC_SAFE_LPCTSTR sTextOld,
		__EXT_MFC_SAFE_LPCTSTR sTextNew
		);
	virtual void OnTextFieldInplaceTextGet(
		const CExtBarTextFieldButton * pTextFieldTBB,
		const CExtCustomizeCmdTreeNode * pNode,
		CExtSafeString & sTextFieldBuffer
		);
	virtual void OnTextFieldInplaceTextSet(
		CExtBarTextFieldButton * pTextFieldTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CExtSafeString & sTextFieldBuffer,
		__EXT_MFC_SAFE_LPCTSTR sTextNew
		);
	virtual bool OnTextFieldWndProcHook(
		LRESULT & lResult,
		UINT message,
		WPARAM wParam,
		LPARAM lParam,
		CEdit & wndEdit,
		CExtBarTextFieldButton * pTextFieldTBB,
		CExtCustomizeCmdTreeNode * pNode
		);
	virtual CExtPopupControlMenuWnd * OnTextFieldCreateDropPopup(
		CExtBarTextFieldButton * pTextFieldTBB,
		CExtCustomizeCmdTreeNode * pNode,
		HWND hWndCmdReceiver,
		bool bContentExpanding
		);

#ifndef __EXT_MFC_NO_BUILTIN_DATEFIELD
	virtual bool OnPopupDatePickerInitContent(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CExtDatePickerWnd & wndDatePicker
		);
	virtual bool OnPopupDatePickerSelChange(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		LPVOID pSelectionNotification
		);
#endif // __EXT_MFC_NO_BUILTIN_DATEFIELD

#ifndef __EXT_MFC_NO_UNDO_REDO_POPUP
	virtual bool OnPopupUndoRedoFormatCaption(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CExtPopupUndoRedoMenuWnd * pUndoRedoPopupMenuWnd,
		CExtSafeString & strCaption
		);
#endif // __EXT_MFC_NO_UNDO_REDO_POPUP

#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)

	virtual void OnColorItemGenerateIcon(
		CExtBarColorButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		COLORREF clr,
		CExtCmdIcon & icon
		);
	virtual bool OnColorItemValueGet(
		CExtBarColorButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		COLORREF & clr,
		bool bSelected
		);
	virtual bool OnColorItemValueSet(
		CExtBarColorButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		COLORREF & clr,
		bool bSelected
		);
	virtual bool OnColorItemChanged(
		CExtBarColorButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		bool bFinalChanging,
		COLORREF clr,
		LPARAM lParam
		);
	virtual bool OnColorItemCustom(
		CExtCustomizeCmdTreeNode * pNode
		);
	virtual bool OnColorItemGetBtnTextDefault(
		CExtBarColorButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CExtSafeString & sBtnText
		);
	virtual bool OnColorItemGetBtnTextCustom(
		CExtBarColorButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CExtSafeString & sBtnText
		);

	virtual INT RedrawCommandItems(
		UINT nCmdID,
		bool bUpdateWindows = true
		) const;
	virtual INT UpdateAllCommandItems() const;

	virtual CCmdMenuInfo * MenuInfoGetDefault();
	virtual CCmdMenuInfo * MenuInfoFindForMenuBar();
	virtual CCmdMenuInfo * MenuInfoActiveGet();
	virtual CCmdMenuInfo * MenuInfoActiveSet( CCmdMenuInfo * pCmdMenuInfo );

protected:
    //{{AFX_MSG(CExtRibbonPage)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange( UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection );
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThemeChanged( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnMenuClosed( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT _OnRibbonFileMenuButtonQuery( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT _OnRibbonFileMenuButtonInvocation( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
public:
	virtual void _OnRibbonPageExpandedModeSet( bool bRibbonPageIsExpandedMode );
	friend class CExtRibbonPopupMenuWnd;
	friend class CExtRibbonButtonGroup;
}; // class CExtRibbonPage

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonGalleryInplaceScrollBar window

class __PROF_UIS_API CExtRibbonGalleryInplaceScrollBar : public CExtScrollBar
{
// Construction
public:
	DECLARE_DYNCREATE( CExtRibbonGalleryInplaceScrollBar );
	CExtRibbonGalleryInplaceScrollBar();

// Attributes
public:
	enum e_ButtonType_t
	{
		__BTT_UP   = 0,
		__BTT_DOWN = 1,
		__BTT_MENU = 2,
		__BTT_COUNT_OF_BUTTONS = 3,
		__BTT_NONE = 3,
	};
	enum e_ButtonState_t
	{
		__BST_DISABLED = 0,
		__BST_NORMAL   = 1,
		__BST_HOVER    = 2,
		__BST_PRESSED  = 3,
	};
	INT m_nTrackingTimerBaseID, m_nTrackingTimerEllpase, m_nTrackingTimerStepPX;
	bool m_bPressedTracking:1;
	e_ButtonType_t m_nTrackedButton;
	e_ButtonState_t m_arrButtons[ __BTT_COUNT_OF_BUTTONS ];
	virtual void RgBtnClearState(
		bool bClearHover = false,
		bool bClearPressed = false
		);
	virtual e_ButtonType_t RgBtnHitTest( CPoint point );
	e_ButtonType_t RgBtnHitTest();
	virtual CRect RgBtnGetRect( e_ButtonType_t nBtnIndex );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtRibbonGalleryInplaceScrollBar)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void _ScanSysMertics();
public:
	virtual void OnSbPaint( CDC & dc );

	virtual INT _GetScrollPos(
		bool bTrackPos = false
		) const;
	virtual void _SetScrollPos(
		INT nPos,
		bool bTrackPos = false,
		bool bRedraw = true
		);
	bool m_bCheckScrollPosByItemTopFailOnce:1;
	virtual bool _CheckScrollPosByItemTop(
		INT nOffset = 0
		);

	virtual ~CExtRibbonGalleryInplaceScrollBar();
	virtual void PostNcDestroy();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtRibbonGalleryInplaceScrollBar)
	afx_msg void OnCancelMode();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
}; // class CExtRibbonGalleryInplaceScrollBar

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonGalleryWnd

class __PROF_UIS_API CExtRibbonGalleryWnd : public CExtToolBoxWnd
{
public:
	DECLARE_DYNCREATE( CExtRibbonGalleryWnd );
	CExtRibbonGalleryWnd();
	virtual ~CExtRibbonGalleryWnd();

	CExtRibbonButtonGallery * m_pRibbonGalleryTBB;

	virtual CFont * OnToolBoxWndQueryItemFont(
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI
		);
	virtual CSize OnToolBoxWndMasureItem(
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI,
		CDC & dc
		);
	virtual void OnToolBoxWndDrawItem(
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI,
		const CRect & rcItem,
		CDC & dc
		);
	virtual void OnToolBoxWndEraseEntire(
		CDC & dc,
		const CRect & rcClient,
		const CRect & rcActiveChildArea,
		CRgn & rgnActiveChildArea,
		const CRect & rcActiveChildArea2,
		CRgn & rgnActiveChildArea2
		);
	virtual bool ItemIsExpanded(
		const CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI
		) const;
	virtual bool ItemExpand(
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI,
		int nExpandCode = TVE_EXPAND, // TVE_COLLAPSE, TVE_EXPAND or TVE_TOGGLE
		bool bUpdateCtrlNow = false,
		bool bEnsureVisibleSelection = false
		);
	virtual bool ItemExpandAll(
		int nExpandCode = TVE_EXPAND, // TVE_COLLAPSE, TVE_EXPAND or TVE_TOGGLE
		bool bUpdateCtrlNow = false,
		bool bEnsureVisibleSelection = false
		);
	bool m_bInPlaceGallery:1;
	virtual bool _IsKeyFocusGallery();
	bool _IsKeyFocusGallery() const;
	virtual bool _IsInplaceGallery();
	bool _IsInplaceGallery() const;
	virtual CRect OnSwGetClientRect() const;
	virtual void _SetToolBoxFocus();
	virtual bool OnToolBoxWndCanHandleHover() const;
	bool m_bDisableMouseWheel:1;
	virtual bool OnSwDoMouseWheel(
		UINT fFlags,
		short zDelta,
		CPoint point
		);
	virtual bool OnSwCanAutoHideScrollBar( bool bHorz ) const;
	virtual void OnSwUpdateScrollBars();
	virtual bool OnSwDoScrollBy(
		CSize sizeScroll,
		bool bDoScroll = true
		);
	virtual bool OnInplaceScrollBarButtonClick(
		CExtRibbonGalleryInplaceScrollBar::e_ButtonType_t nBT,
		bool bLButtonDown
		);
	CExtRibbonGalleryInplaceScrollBar m_wndScrollBarV;
protected:
	virtual bool _CreateScrollBar();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtRibbonGalleryWnd)
	public:
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExtRibbonGalleryWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnToolBoxWndItemSelEndOK(
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI
		);
	virtual void OnToolBoxWndItemInsert(
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI
		);
	virtual void OnToolBoxWndItemRemove(
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI
		);
	virtual void OnToolBoxWndItemActivate(
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_Old,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_New
		);
	virtual void OnToolBoxWndItemSelChange(
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_Old,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_New
		);
	virtual void OnToolBoxWndItemHoverChange(
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_Old,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_New
		);

}; // class CExtRibbonGalleryWnd

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonNodeGallery

class __PROF_UIS_API CExtRibbonNodeGallery : public CExtRibbonNode
{
protected:
	virtual void _InitMembers();
public:
	CSize m_sizePopupGalleryControl, m_sizePopupGalleryControlMin, m_sizePopupGalleryControlMax;
	
	virtual void AssignFromOther(
		CExtCustomizeCmdTreeNode & other
		);

	DECLARE_SERIAL( CExtRibbonNodeGallery );
	
	CExtRibbonNodeGallery(
		UINT nCmdId = 0L,
		CExtRibbonNode * pParentNode = NULL,
		DWORD dwFlags = 0L,
		__EXT_MFC_SAFE_LPCTSTR strTextInToolbar = NULL,
		__EXT_MFC_SAFE_LPCTSTR strTextInMenu = NULL,
		__EXT_MFC_SAFE_LPCTSTR strTextUser = NULL,
		LPARAM lParam = 0L
		);
	CExtRibbonNodeGallery(
		CExtRibbonNodeGallery & other
		);
	virtual ~CExtRibbonNodeGallery();
	virtual CRuntimeClass * _OnRibbonGetButtonRTC();
	virtual CExtBarButton * OnRibbonCreateBarButton(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL
		);
	virtual bool Ribbon_InitBar(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL,
		bool bInsertThisNode = true
		);
	virtual void RibbonILV_Set(
		INT nILV,
		INT nType = 0 // -1 min, 0 current, 1 - max
		);
	virtual INT RibbonILV_fromILE(
		INT nILE,
		bool * p_bIsWrap = NULL
		) const;
	virtual void Serialize( CArchive & ar );
	virtual void PostConstructMenuItem(
		LPVOID pMenuItemData,
		HWND hWndCmdRecv,
		int nItemIndex,
		CExtPopupMenuWnd * pPopupParent
		);
}; // class CExtRibbonNodeGallery

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonNodeFile

class __PROF_UIS_API CExtRibbonNodeFile : public CExtRibbonNodeGallery
{
public:
	DECLARE_SERIAL( CExtRibbonNodeFile );
	CExtCustomizeCmdKeyTip m_keyTipOptionsButton, m_keyTipExitButton;

	CExtRibbonNodeFile(
		CExtRibbonNode * pParentNode = NULL
		);
	CExtRibbonNodeFile(
		CExtRibbonNodeFile & other
		);
	virtual ~CExtRibbonNodeFile();
	virtual CRuntimeClass * _OnRibbonGetButtonRTC();
	virtual CExtBarButton * OnRibbonCreateBarButton(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL
		);
	virtual bool Ribbon_InitBar(
		CExtRibbonPage * pBar,
		CExtBarButton * pParentTBB = NULL,
		bool bInsertThisNode = true
		);
	virtual void Serialize( CArchive & ar );

	virtual bool OnGetCommandsListBoxInfo(
		CExtCustomizeCommandListBox * pLB, // IN (optional)
		CExtCustomizeSite * pSite = NULL, // IN (optional)
		CExtCmdItem * pCmdItem = NULL, // IN (optional)
		CExtSafeString * pStrLbText = NULL, // OUT (optional)
		CExtCmdIcon * pLbIcon = NULL, // OUT (optional)
		INT nDesiredIconWidth = 16, // IN (optional)
		INT nDesiredIconHeight = 16 // IN (optional)
		);

}; // class CExtRibbonNodeFile

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonButtonGallery

class __PROF_UIS_API CExtRibbonButtonGallery : public CExtRibbonButton
{
	CExtRibbonGalleryWnd m_wndIG;
	bool m_bGalleryPopupVisible:1;
public:
	DECLARE_DYNCREATE( CExtRibbonButtonGallery );
	CExtRibbonButtonGallery(
		CExtRibbonPage * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		);
	virtual ~CExtRibbonButtonGallery();
	virtual CSize RibbonILV_CalcSize(
		CDC & dc,
		INT nILV = -1 // -1 use current visual level
		) const;
	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);
	virtual void PaintCompound(
		CDC & dc,
		bool bPaintParentChain,
		bool bPaintChildren,
		bool bPaintOneNearestChildrenLevelOnly
		);
	virtual void SetBar( CExtToolControlBar * pBar );
	virtual bool IsPressed() const;
	virtual void OnGalleryInitContent(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup
		);
	virtual void OnGalleryPopupShow(
		CExtRibbonGalleryPopupMenuWnd * pPopup,
		bool bShow
		);
	virtual bool IsAbleToTrackMenu(
		bool bCustomizeMode = false
		) const;
	virtual UINT OnTrackPopup(
		CPoint point,
		bool bSelectAny,
		bool bForceNoAnimation
		);
	virtual CWnd * CtrlGet();
	virtual void OnRibbonAlignContent( CDC & dc );
	virtual bool IsInplaceGalleryMode() const;
	virtual CExtRibbonGalleryWnd & InplaceGalleryGet();
	const CExtRibbonGalleryWnd & InplaceGalleryGet() const;
	virtual void InplaceGalleryEnsureCreated();

	virtual void OnRibbonGalleryItemSelEndOK(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI
		);
	virtual void OnRibbonGalleryItemInsert(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI
		);
	virtual void OnRibbonGalleryItemRemove(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI
		);
	virtual void OnRibbonGalleryItemActivate(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_Old,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_New
		);
	virtual void OnRibbonGalleryItemSelChange(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_Old,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_New
		);
	virtual void OnRibbonGalleryItemHoverChange(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_Old,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_New
		);

	virtual bool OnInvalidateButton();

	CPoint OnKeyTipGetGuideLines();
	virtual bool OnKeyTipInvokeAction(
		bool & bContinueKeyTipMode
		);
}; // class CExtRibbonButtonGallery

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonButtonFile

#define __EXT_MFC_ID_RIBBON_APPLICATION_BUTTON 1

class __PROF_UIS_API CExtRibbonButtonFile : public CExtRibbonButtonGallery
{
public:
	DECLARE_DYNCREATE( CExtRibbonButtonFile );
	CExtCmdIcon m_iconForLargeMode;
	CExtRibbonButtonFile(
		CExtRibbonPage * pBar = NULL,
		UINT nCmdID = __EXT_MFC_ID_RIBBON_APPLICATION_BUTTON,
		UINT nStyle = 0
		);
	virtual ~CExtRibbonButtonFile();
	virtual CExtCustomizeCmdTreeNode * GetCmdNode(
		bool bInitial = false
		);
	virtual CExtSafeString GetText() const;
	virtual bool IsSeparator() const;
	virtual bool IsNoRibbonLayout() const;
	virtual bool IsInplaceGalleryMode() const;
	virtual void InplaceGalleryEnsureCreated();
	virtual INT RibbonILV_Get(
		INT nType = 0 // -1 min, 0 current, 1 - max
		) const;
	bool m_bHelperCustomLayoutApplied:1;
	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);
	virtual void PaintCompound(
		CDC & dc,
		bool bPaintParentChain,
		bool bPaintChildren,
		bool bPaintOneNearestChildrenLevelOnly
		);
	virtual void OnUpdateCmdUI(
		CWnd * pTarget,
		BOOL bDisableIfNoHndler,
		int nIndex
		);
	virtual bool IsAbleToTrackMenu(
		bool bCustomizeMode = false
		) const;
	virtual void OnClick(
		CPoint point,
		bool bDown
		);
	virtual bool OnKeyTipInvokeAction(
		bool & bContinueKeyTipMode
		);
	virtual bool OnDblClick(
		CPoint point
		);
	virtual void OnHover(
		CPoint point,
		bool bOn,
		bool & bSuspendTips
		);
	virtual UINT OnTrackPopup(
		CPoint point,
		bool bSelectAny,
		bool bForceNoAnimation
		);
	virtual bool OnRibbonFileMenuButtonQuery(
		CExtPopupMenuWnd::RIBBONFILEMENUBUTTONQUERY & _rfmbq,
		CExtCmdIcon & _icon,
		CExtSafeString & sText
		);
	virtual bool OnRibbonFileMenuButtonInvocation(
		CExtPopupMenuWnd::RIBBONFILEMENUBUTTONINVOCATION & _rfmbi
		);
	virtual void AnimationClient_NextStatePrepare(
		CDC & dc,
		const RECT & rcAcAnimationTarget,
		bool bAnimate,
		INT eAPT // __EAPT_*** animation type
		);
	virtual void AnimationClient_NextStateAdjust(
		CExtBitmap & _bmp
		);
	virtual CPoint OnKeyTipGetGuideLines();
}; // class CExtRibbonButtonFile

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonGalleryPopupMenuWnd

class __PROF_UIS_API CExtRibbonGalleryPopupMenuWnd
	: public CExtPopupControlMenuWnd
	, public CExtHookSpy
{
public:
	DECLARE_DYNCREATE( CExtRibbonGalleryPopupMenuWnd );

	CExtRibbonGalleryPopupMenuWnd(
		HWND hWndSrcRibbonPage = NULL,
		CExtBarButton * pSrcTrackingButton = NULL
		);
	virtual ~CExtRibbonGalleryPopupMenuWnd();

// Attributes
public:
	HWND m_hWndSrcRibbonPage;
	bool m_bGalleryTrackingMode:1;
	bool m_bRibbonFileMenuMode:1;
	bool m_bHelperKeyboardProcessing:1;

	CExtBarButton * m_pRibbonSrcTrackingButton;
	INT m_nRibbonSrcMenuTrackingIdx;
	CExtRibbonGalleryWnd m_wndRibbonGallery;

	virtual bool IsSyncFullRowItems();
	CExtRibbonPage * GetMainRibbonPage();
	const CExtRibbonPage * GetMainRibbonPage() const;

// Implementation
protected:
	virtual CRect _RecalcControlRect();
	virtual void _RecalcLayoutImpl();
	virtual HWND OnCreateChildControl(
		const RECT & rcChildControl
		);
public:
	virtual CSize _CalcTrackSize();
	virtual bool _OnMouseWheel(
		WPARAM wParam,
		LPARAM lParam,
		bool & bNoEat
		);
	virtual bool _OnMouseMove(
		UINT nFlags,
		CPoint point,
		bool & bNoEat
		);
	virtual bool _OnMouseClick(
		UINT nFlags,
		CPoint point,
		bool & bNoEat
		);
	virtual bool _OnKeyDown(
		UINT nChar,
		UINT nRepCnt,
		UINT nFlags,
		bool & bNoEat
		);
	virtual bool OnHookSpyMouseWheelMsg(
		MSG * pMSG
		);
	virtual bool OnHookSpyMouseMoveMsg(
		MSG * pMSG
		);
	virtual bool OnHookSpyMouseClickMsg(
		MSG * pMSG
		);
	virtual bool OnHookSpyKeyMsg(
		MSG * pMSG
		);
	virtual BOOL DestroyWindow();
	virtual LRESULT WindowProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam
		);
	virtual CRect OnQueryMenuBorderMetrics() const;

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtRibbonGalleryPopupMenuWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtRibbonGalleryPopupMenuWnd

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonPopupMenuWnd

class __PROF_UIS_API CExtRibbonPopupMenuWnd
	: public CExtPopupControlMenuWnd
	, public CExtHookSpy
{
public:
	DECLARE_DYNCREATE( CExtRibbonPopupMenuWnd );

	CExtRibbonPopupMenuWnd(
		HWND hWndSrcRibbonPage = NULL,
		CExtRibbonButtonGroup * pSrcTrackingButtonGroup = NULL,
		CExtRibbonButtonTabPage * pSrcTrackingButtonTabPage = NULL
		);
	virtual ~CExtRibbonPopupMenuWnd();

// Attributes
public:

	CExtRibbonButtonGroup * m_pSrcTrackingButtonGroup;
	CExtRibbonButtonTabPage * m_pSrcTrackingButtonTabPage;
	INT m_nSrcMenuTrackingIdx;
	CExtRibbonPage m_wndRibbonPage;

	CExtRibbonPage * GetMainRibbonPage();
	const CExtRibbonPage * GetMainRibbonPage() const;
	CExtRibbonPage * GetParentRibbonPage();
	const CExtRibbonPage * GetParentRibbonPage() const;

// Implementation
protected:
	virtual HWND OnCreateChildControl(
		const RECT & rcChildControl
		);

public:
//	virtual bool _OnMouseWheel(
//		WPARAM wParam,
//		LPARAM lParam,
//		bool & bNoEat
//		);
//	virtual bool _OnMouseMove(
//		UINT nFlags,
//		CPoint point,
//		bool & bNoEat
//		);
//	virtual bool _OnMouseClick(
//		UINT nFlags,
//		CPoint point,
//		bool & bNoEat
//		);
//	virtual bool _OnKeyDown(
//		UINT nChar,
//		UINT nRepCnt,
//		UINT nFlags,
//		bool & bNoEat
//		);
	virtual bool OnHookSpyMouseWheelMsg(
		MSG * pMSG
		);
	virtual bool OnHookSpyMouseMoveMsg(
		MSG * pMSG
		);
	virtual bool OnHookSpyMouseClickMsg(
		MSG * pMSG
		);
	virtual bool OnHookSpyKeyMsg(
		MSG * pMSG
		);
	virtual bool _IsFadeOutAnimation() const;
	virtual void _DeleteFadeOutMenu();
	virtual BOOL DestroyWindow();
	virtual LRESULT WindowProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam
		);
	virtual CRect OnQueryMenuBorderMetrics() const;

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtRibbonPopupMenuWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtRibbonPopupMenuWnd

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonOptionsPage

class __PROF_UIS_API CExtRibbonOptionsPage
	: public CExtResizableDialog
{
public:
	DECLARE_DYNAMIC( CExtRibbonOptionsPage );

	CExtRibbonOptionsPage(
		UINT nPageID
		);
	virtual ~CExtRibbonOptionsPage();
	virtual CExtRibbonBar * RibbonOptionsPage_GetBar();
	const CExtRibbonBar * RibbonOptionsPage_GetBar() const;
	virtual CExtRibbonOptionsDialog * RibbonOptionsPage_GetContainer();
	const CExtRibbonOptionsDialog * RibbonOptionsPage_GetContainer() const;
	virtual UINT RibbonOptionsPage_GetID() const = 0;
	virtual bool RibbonOptionsPage_InitContent();
	virtual void RibbonOptionsPage_Apply();
	virtual void RibbonOptionsPage_Cancel();

	class __PROF_UIS_API CSimpleButton : public CExtButton
	{
	public:
		CSimpleButton()
		{
		}
		virtual ~CSimpleButton()
		{
		}
	protected:
		virtual void _DrawImpl(
			CDC & dc,
			bool bTransparent = false,
			bool bCombinedContent = false
			)
		{
			if( ! PmBridge_GetPM()->Ribbon_OptionsPageBackgroundIsDefault() )
				bTransparent = true;
			CExtButton::_DrawImpl(
				dc,
				bTransparent,
				bCombinedContent
				);
		}
	}; // class CSimpleButton

	class __PROF_UIS_API CSimpleMarlettButton : public CSimpleButton
	{
	public:
		CSimpleMarlettButton()
		{
		}
		virtual ~CSimpleMarlettButton()
		{
		}
	protected:
		virtual void PreSubclassWindow()
		{
			CSimpleButton::PreSubclassWindow();
			CRect rcButton;
			GetClientRect( &rcButton );
			CFont font;
			INT nPt = max( 6, ::MulDiv( min( rcButton.Width(), rcButton.Height() ), 2, 3 ) );
			CWindowDC dc( NULL );
			INT nPPI = ::GetDeviceCaps( dc.m_hDC, LOGPIXELSX );
			INT nPointSize = ::MulDiv( nPt * 10, 96, nPPI );
			font.CreatePointFont( nPointSize, _T("Marlett") );
			SetFont( &font, false );
			font.Detach();
		}
	}; // class CSimpleMarlettButton

	class __PROF_UIS_API CConsistentLabel : public CExtLabel
	{
	public:
		CConsistentLabel()
		{
		}
		virtual ~CConsistentLabel()
		{
		}
		virtual void OnEraseBackground(
			CDC & dc,
			const CRect & rcClient
			);
	};

// Attributes
public:
	CFont m_fontPageCaption;
	CExtBitmap m_bmpPageWaterMark;
	COLORREF m_clrPageText, m_clrPageBkgnd;
	CExtSafeString m_strPageTitleShort, m_strPageTitleLong;

	virtual void OnEraseBackground(
		CDC & dc,
		const CRect & rcClient
		);

// Dialog Data
	//{{AFX_DATA(CExtRibbonOptionsPage)
	//}}AFX_DATA

// Implementation
protected:
    //{{AFX_VIRTUAL(CExtRibbonOptionsPage)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtRibbonOptionsPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtRibbonOptionsPage

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonOptionsPageCustomizeQATB

class __PROF_UIS_API CExtRibbonOptionsPageCustomizeQATB
	: public CExtRibbonOptionsPage
{
public:
	DECLARE_DYNCREATE( CExtRibbonOptionsPageCustomizeQATB );

	CExtRibbonOptionsPageCustomizeQATB();
	virtual ~CExtRibbonOptionsPageCustomizeQATB();
	virtual UINT RibbonOptionsPage_GetID() const;
	virtual bool RibbonOptionsPage_InitContent();
	virtual void RibbonOptionsPage_Apply();
	virtual void RibbonOptionsPage_Cancel();

// Attributes
public:
	CExtRibbonNodeQuickAccessButtonsCollection * m_pNodeColQATB;
	void _UpdateButtons();

// Dialog Data
	//{{AFX_DATA(CExtRibbonOptionsPageCustomizeQATB)
	CConsistentLabel m_labelChooseCommandsFrom;
	CExtComboBox m_comboChooseCommandsFrom;
	CSimpleButton m_buttonAdd;
	CSimpleButton m_buttonRemove;
	CConsistentLabel m_labelCustomizeTarget;
	CExtComboBox m_comboCustomizeTarget;
	CSimpleButton m_buttonReset;
	CSimpleButton m_buttonModify;
	CSimpleMarlettButton m_buttonMoveUp;
	CSimpleMarlettButton m_buttonMoveDown;
	CExtCheckBox m_checkShowQatbBelow;
	CExtLabel m_labelKB;
	CSimpleButton m_buttonKB;
	//}}AFX_DATA

	CExtNCSB < CExtWFF < CExtCustomizeCommandListBox > > m_listBoxChooseCommandsFrom;
	CExtNCSB < CExtWFF < CExtCustomizeCommandListBox > > m_listBoxCustomizeTarget;

// Implementation
protected:
    //{{AFX_VIRTUAL(CExtRibbonOptionsPageCustomizeQATB)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtRibbonOptionsPageCustomizeQATB)
	afx_msg void OnShowQatbBelowTheRibbon();
	afx_msg void OnCustomuzeKeyboard();
	afx_msg void OnQatbButtonAdd();
	afx_msg void OnQatbButtonRemove();
	afx_msg void OnQatbButtonReset();
	afx_msg void OnQatbButtonModify();
	afx_msg void OnQatbButtonMoveUp();
	afx_msg void OnQatbButtonMoveDown();
	afx_msg void OnSrcComboSelEndOK();
	afx_msg void OnDstComboSelEndOK();
	afx_msg void OnSrcListBoxSelEndOK();
	afx_msg void OnDstListBoxSelEndOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtRibbonOptionsPageCustomizeQATB

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonOptionsTabsWnd

class __PROF_UIS_API CExtRibbonOptionsTabsWnd
	: public CExtRibbonGalleryWnd
{
// Construction
public:
	DECLARE_DYNCREATE( CExtRibbonOptionsTabsWnd );

	CExtRibbonOptionsTabsWnd();
	virtual ~CExtRibbonOptionsTabsWnd();

// Attributes
public:

// Operations
public:

// Overrides
    //{{AFX_VIRTUAL(CExtRibbonOptionsTabsWnd)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool AddPage(
		CExtRibbonOptionsPage * pPage,
		bool bSelect = false
		);
	virtual bool AddPage(
		CExtRibbonOptionsPage * pPage,
		CExtCmdIcon & _icon,
		bool bSelect = false
		);
	virtual INT GetIndexOfPage(
		const CExtRibbonOptionsPage * pPage
		) const;
	virtual bool SelectPage(
		CExtRibbonOptionsPage * pPage
		);
	virtual void ResetContent();

	virtual bool OnSwCanAutoHideScrollBar( bool bHorz ) const;
	virtual void OnToolBoxWndItemSelChange(
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_Old,
		CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_New
		);

protected:
    //{{AFX_MSG(CExtRibbonOptionsTabsWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

}; // class CExtRibbonOptionsTabsWnd

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonOptionsDialog

class
#if ( _MFC_VER != 0x700 )
	__PROF_UIS_API
#endif
	CExtRibbonOptionsDialog
	: public CExtNCW < CExtResizableDialog >
{
public:
	DECLARE_DYNCREATE( CExtRibbonOptionsDialog );

	CExtRibbonOptionsDialog(
		UINT nSelectedPageID = __EXT_RIBBON_OPTIONS_DIALOG_PAGE_DEFAULT,
		CExtRibbonBar * pRibbonBar = NULL,
		CWnd * pWndParent = NULL
		);
	virtual ~CExtRibbonOptionsDialog();

// Attributes
public:
	CExtSafeString m_strCommandProfileName;
	UINT m_nSelectedPageID;
	CExtRibbonBar * m_pRibbonBar;
	CTypedPtrArray < CPtrArray, CRuntimeClass * > m_arrPageRTCs;

// Dialog Data
	//{{AFX_DATA(CExtRibbonOptionsDialog)
	CExtButton m_buttonOK;
	CExtButton m_buttonCancel;
	CExtLabel m_labelEmptyFrame;
	//}}AFX_DATA
	CExtRibbonOptionsTabsWnd m_wndOptionsTabs;

// Implementation
public:
	virtual void OnPageSelected( CExtRibbonOptionsPage * pPage );
protected:
	virtual void OnOK();
	virtual void OnCancel();

    //{{AFX_VIRTUAL(CExtRibbonOptionsDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtRibbonOptionsDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtRibbonOptionsDialog

#if ( _MFC_VER == 0x700 ) && (! defined __EXT_PROFUIS_STATIC_LINK)
	class __PROF_UIS_API CExtRibbonOptionsDialog;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtRibbonBar

class __PROF_UIS_API CExtRibbonBar
	: public CExtRibbonPage
	, public CExtNcFrameImpl::IExtNcFrameImplBridge
{
public:
	DECLARE_DYNCREATE( CExtRibbonBar );
// Construction
public:
	CExtRibbonBar();
	virtual ~CExtRibbonBar();
// Attributes
public:
	mutable INT m_nHelperCaptionHeight, m_nHelperTopBorderHeight;
	CExtNcFrameImpl * m_pExtNcFrameImpl;
	CExtSafeString m_strCaptionDelimiter;
protected:
	INT m_nHelperCtxIndexQA, m_nPmBridgeCounter;
	mutable bool m_bHelperErasingNC:1;
	bool m_bHelperKeyTipTopLevelMode:1;
	UINT m_nLastNcHT;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtRibbonBar)
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void OnFlatTrackingStart(
		HDWP & hPassiveModeDWP
		);
	virtual void OnFlatTrackingStop();
	virtual void _RibbonBarKeyTipSetupForQATB();
	virtual void _RibbonBarKeyTipShow(
		bool bShowKeyTipsAtTopLevel,
		bool bShowKeyTipsAtPageLevel
		);

public:
	virtual BOOL SetButtons(
		CExtCustomizeCmdTreeNode * pNode
		);
	virtual CSize _CalcLayout(
		DWORD dwMode,
		int nLength = -1
		);
	virtual void _RecalcPositionsImpl();

	virtual void DoPaintNC( CDC * pDC );
	virtual void DoEraseBk( CDC * pDC );
	virtual void DoPaint( CDC * pDC );

	virtual bool RibbonLayout_IsDwmCaptionIntegration() const;
	virtual CSize RibbonLayout_Calc(
		CDC & dc,
		CPoint ptLayoutOffset = CPoint( 0, 0 ),
		bool bSetupRects = true,
		INT * p_nCountLayouted = NULL
		);
	virtual void RibbonLayout_AlignPageContent(
		CDC & dc,
		CRect rcAlignContent
		);
	virtual void RibbonLayout_AlignTabLineContent(
		CDC & dc,
		CRect rcAlignContent
		);
	virtual void RibbonLayout_AdjustTabLineRect( CRect & rcTabLine );
	virtual bool OnDeliverCmd( CExtBarButton * pTBB );

	virtual bool OnQueryBarHoverProcessingEnabled() const;
	virtual	bool _OnMouseMoveMsg(UINT nFlags, CPoint point);

	virtual void PmBridge_OnPaintManagerChanged(
		CExtPaintManager * pGlobalPM
		);

	virtual INT RibbonLayout_GetTabIntersectionHeight() const;
	virtual INT RibbonLayout_GetTabLineHeight() const;
	virtual HRGN NcFrameImplBridge_GetNcExcludeHRGN() const;
	virtual HRGN NcFrameImplBridge_GetNcResizingHRGN() const;
	virtual bool NcFrameImplBridge_GetNcScRect( UINT nSC, CRect & rc ) const;
	virtual bool NcFrameImpl_GetNcHtRect(
		CRect & rc,
		UINT nHT,
		bool bScreenMapping = true,
		bool bLayoutBordersH = false,
		bool bLayoutBordersV = false,
		LPMINMAXINFO pMinMaxInfo = NULL,
		LPCRECT pRectWnd = NULL
		) const;
	virtual void NcFrameImplBridge_OnOverPaint(
		CDC & dc,
		CRect rcWnd,
		CRect rcClient
		) const;
	virtual void NcFrameImplBridge_OnDrawCaptionText(
		CDC & dc,
		__EXT_MFC_SAFE_LPCTSTR strCaption,
		CRect rcDrawText
		) const;
	virtual bool NcFrameImplBridge_OnQueryDrawIcon(
		CRect & rcAdjustLocation
		) const;
	virtual bool NcFrameImplBridge_OnQueryDrawCaptionText(
		CRect & rcAdjustLocation,
		UINT & nAdjustDrawTextFlags
		) const;
	virtual void NcFrameImpl_PreSetWindowPlacement( const WINDOWPLACEMENT & _wp );
	virtual void NcFrameImpl_PostSetWindowPlacement( const WINDOWPLACEMENT & _wp );
	virtual HWND NcFrameImplBridge_GetSafeHwnd() const;
	virtual bool NcFrameImplBridge_OnQueryCaptionMergeMode() const;
	virtual bool RibbonLayout_IsFrameIntegrationEnabled() const;
	virtual INT RibbonLayout_GetFrameCaptionHeight(
		INT * p_nTopBorderHeight = NULL
		) const;
	virtual INT RibbonLayout_GetBottomLineHeight() const;

	virtual BOOL _TrackFrameSystemMenu(
		CWnd * pFrame,
		CPoint * pPoint = NULL, // NULL means calc meny track area automatically
		BOOL bSelectAny = FALSE,
		LPCRECT rcExcludeArea = NULL,
		UINT nTrackFlags = (UINT)(-1),
		BOOL bCombinedMode = FALSE
		);

	virtual bool OnRibbonProcessMouseWheel( UINT fFlags, short zDelta, CPoint point );
	virtual bool _TranslateMouseMsg(
		UINT message,
		UINT nFlags,
		CPoint point
		);

protected:
	bool m_bAutoHideModeEnabled:1;
public:
	virtual bool Ribbon_AutoHideModeEnabledGet() const;
	virtual void Ribbon_AutoHideModeEnabledSet( bool bAutoHideModeEnabled );
	virtual bool Ribbon_AutoHideModeDoExpanding(
		CExtRibbonButtonTabPage * pTBB,
		bool bKeyboardTrackingEvent
		);

	virtual bool Ribbon_FileButtonInit();
protected:
	mutable CRect m_rcHelperQA;
	mutable CRect m_rcHelperEmbeddedCaptionText;
	CExtRibbonButtonFile * m_pFileTBB;
public:
	virtual CExtRibbonButtonQuickAccessContentExpand * Ribbon_QuickAccessContentExpandButtonGet();
	const CExtRibbonButtonQuickAccessContentExpand * Ribbon_QuickAccessContentExpandButtonGet() const;
	virtual bool Ribbon_QuickAccessContentExpandButtonCalcLayout( CDC & dc );
	virtual bool Ribbon_QuickAccessContentExpandButtonPaint( CDC & dc );
	virtual CExtRibbonButtonFile * Ribbon_FileButtonGet();
	const CExtRibbonButtonFile * Ribbon_FileButtonGet() const;
	virtual bool Ribbon_FileButtonCalcLayout( CDC & dc );
	virtual bool Ribbon_FileButtonPaint( CDC & dc );

	virtual bool OnRibbonTrackButtonContextMenu(
		CExtBarButton * pTBB,
		UINT nFlags,
		CPoint point
		);
	virtual bool OnRibbonTrackBarContextMenu(
		UINT nFlags,
		CPoint point
		);
	virtual BOOL TranslateMainFrameMessage(MSG* pMsg);
	virtual INT OnCalcFlatTrackingIndex(
		/*__EXT_MFC_SAFE_TCHAR*/ INT vkTCHAR,
		INT nStartSearchIndex
		);
	virtual bool OnCalcFlatTrackingIndexCheckPass(
		/*__EXT_MFC_SAFE_TCHAR*/ INT vkTCHAR,
		INT nPassIndex,
		const CExtBarButton * pPrevTBB,
		const CExtBarButton * pNextTBB
		) const;
	virtual bool OnCalcFlatTrackingTabOrder(
		CTypedPtrArray < CPtrArray, CExtBarButton * > & arrTabOrder
		);
	virtual bool OnCalcFlatTrackingIndexCheckIntersectionEnabled(
		CExtBarButton * pTBB,
		/*__EXT_MFC_SAFE_TCHAR*/ INT vkTCHAR
		);

	CExtRibbonNodeFile * Ribbon_GetFileRootNode();
	const CExtRibbonNodeFile * Ribbon_GetFileRootNode() const;

	CExtRibbonNodeQuickAccessButtonsCollection * Ribbon_GetQuickAccessRootNode();
	const CExtRibbonNodeQuickAccessButtonsCollection * Ribbon_GetQuickAccessRootNode() const;

	CExtRibbonNodeRightButtonsCollection * Ribbon_GetRightRootNode();
	const CExtRibbonNodeRightButtonsCollection * Ribbon_GetRightRootNode() const;

	CExtRibbonNodeTabPageCollection * Ribbon_GetTabPageRootNode();
	const CExtRibbonNodeTabPageCollection * Ribbon_GetTabPageRootNode() const;

	virtual bool Ribbon_OnPageSelectionChanging(
		INT nOldSelIdx,
		INT nNewSelIdx,
		bool & bEnableAnimation
		);
	virtual void Ribbon_OnPageSelectionChanged(
		INT nOldSelIdx,
		INT nNewSelIdx
		);
	virtual INT Ribbon_PageSelectionGet() const;
	virtual bool Ribbon_PageSelectionSet(
		INT nSelIdx,
		bool bEnableAnimation = true
		);

	virtual void OnRibbonFileMenuButtonQuery(
		CExtPopupMenuWnd::RIBBONFILEMENUBUTTONQUERY & _rfmbq,
		CExtCmdIcon & _icon,
		CExtSafeString & sText
		);
	virtual void OnRibbonFileMenuButtonInvocation(
		CExtPopupMenuWnd::RIBBONFILEMENUBUTTONINVOCATION & _rfmbi
		);
	virtual void OnRibbonOptionsDialogTrack(
		UINT nSelectedPageID = __EXT_RIBBON_OPTIONS_DIALOG_PAGE_DEFAULT
		);
	virtual void OnRibbonOptionsDialogInitPageRTCs(
		CExtRibbonOptionsDialog & dlgRibbonOptions
		);

	virtual bool CustomizeStateSerialize(
		CArchive & ar,
		bool bEnableThrowExceptions = false
		);

protected:
    //{{AFX_MSG(CExtRibbonBar)
	afx_msg void OnPaint();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcMouseMove( UINT nHitTest, CPoint point );
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void _OnCmdRibbonQuickAccessToolbarPlaceAbove();
	afx_msg void _OnCmdRibbonQuickAccessToolbarPlaceBelow();
	afx_msg void _OnCmdRibbonQuickAccessToolbarCustomize();
	afx_msg void _OnCmdRibbonQuickAccessToolbarReset();
	void _OnCmdRibbonQuickAccessToolbarReset(
		CWnd * pWndForMsgBox
		);
	afx_msg void _OnCmdRibbonQuickAccessToolbarAddTo();
	afx_msg void _OnCmdRibbonQuickAccessToolbarRemoveFrom();
	afx_msg void _OnCmdRibbonMinimize();
	virtual void _OnRibbonPageExpandedModeSet( bool bRibbonPageIsExpandedMode );

	friend class CExtRibbonOptionsPageCustomizeQATB;
	friend class CExtRibbonButtonTabPage;
}; // class CExtRibbonBar

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // (!defined __EXT_MFC_NO_RIBBON_BAR)

#endif // (!defined __EXT_RIBBON_BAR_H)
