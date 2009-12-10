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

#if (!defined __EXTCUSTOMIZE_H)
#define __EXTCUSTOMIZE_H

#if (!defined __EXT_MFC_NO_CUSTOMIZE)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_HOOK_H)
	#include "../Src/ExtHook.h"
#endif

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

#if (!defined __ExtCmdManager_H)
	#include <ExtCmdManager.h>
#endif

#ifndef __AFXTEMPL_H__
	#include <AfxTempl.h>
#endif

#if (!defined __EXT_CHECK_LIST_H)
	#include <ExtCheckListWnd.h>
#endif

#if (!defined __EXT_EDIT_H)
	#include <ExtEdit.h>
#endif

class CExtPopupMenuWnd;
class CExtMenuControlBar;
class CExtToolControlBar;
class CExtControlBar;
class CExtCmdItem;
class CExtCustomizeSite;
class CExtBarButton;
class CExtBarColorButton;
class CExtCmdProfile;
class CExtCustomizeCommandListBox;
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	class CExtBarTextFieldButton;
	class CExtPopupControlMenuWnd;
	class CExtDatePickerWnd;
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
#ifndef __EXT_MFC_NO_UNDO_REDO_POPUP
	class CExtPopupUndoRedoMenuWnd;
#endif // __EXT_MFC_NO_UNDO_REDO_POPUP

/////////////////////////////////////////////////////////////////////////////
// __PROF_UIS_API CExtCustomizeCmdScreenTip

#define __ECCST_SEPARATOR								0x00000001
#define __ECCST_USER_DEFINED_30							0x00000002
#define __ECCST_USER_DEFINED_29							0x00000004
#define __ECCST_USER_DEFINED_28							0x00000008
#define __ECCST_USER_DEFINED_27							0x00000010
#define __ECCST_USER_DEFINED_26							0x00000020
#define __ECCST_USER_DEFINED_25							0x00000040
#define __ECCST_USER_DEFINED_24							0x00000080
#define __ECCST_USER_DEFINED_23							0x00000100
#define __ECCST_USER_DEFINED_22							0x00000200
#define __ECCST_USER_DEFINED_21							0x00000400
#define __ECCST_USER_DEFINED_20							0x00000800
#define __ECCST_USER_DEFINED_19							0x00001000
#define __ECCST_USER_DEFINED_18							0x00002000
#define __ECCST_USER_DEFINED_17							0x00004000
#define __ECCST_USER_DEFINED_16							0x00008000
#define __ECCST_USER_DEFINED_15							0x00010000
#define __ECCST_USER_DEFINED_14							0x00020000
#define __ECCST_USER_DEFINED_13							0x00040000
#define __ECCST_USER_DEFINED_12							0x00080000
#define __ECCST_USER_DEFINED_11							0x00100000
#define __ECCST_USER_DEFINED_10							0x00200000
#define __ECCST_USER_DEFINED_09							0x00400000
#define __ECCST_USER_DEFINED_08							0x00800000
#define __ECCST_USER_DEFINED_07							0x01000000
#define __ECCST_USER_DEFINED_06							0x02000000
#define __ECCST_USER_DEFINED_05							0x04000000
#define __ECCST_USER_DEFINED_04							0x08000000
#define __ECCST_USER_DEFINED_03							0x10000000
#define __ECCST_USER_DEFINED_02							0x20000000
#define __ECCST_USER_DEFINED_01							0x40000000
#define __ECCST_USER_DEFINED_00							0x80000000

#define __ECCST_DEFAUT									(__ECCST_SEPARATOR)

#define __ECCST_EX_USER_DEFINED_31						0x00000001
#define __ECCST_EX_USER_DEFINED_30						0x00000002
#define __ECCST_EX_USER_DEFINED_29						0x00000004
#define __ECCST_EX_USER_DEFINED_28						0x00000008
#define __ECCST_EX_USER_DEFINED_27						0x00000010
#define __ECCST_EX_USER_DEFINED_26						0x00000020
#define __ECCST_EX_USER_DEFINED_25						0x00000040
#define __ECCST_EX_USER_DEFINED_24						0x00000080
#define __ECCST_EX_USER_DEFINED_23						0x00000100
#define __ECCST_EX_USER_DEFINED_22						0x00000200
#define __ECCST_EX_USER_DEFINED_21						0x00000400
#define __ECCST_EX_USER_DEFINED_20						0x00000800
#define __ECCST_EX_USER_DEFINED_19						0x00001000
#define __ECCST_EX_USER_DEFINED_18						0x00002000
#define __ECCST_EX_USER_DEFINED_17						0x00004000
#define __ECCST_EX_USER_DEFINED_16						0x00008000
#define __ECCST_EX_USER_DEFINED_15						0x00010000
#define __ECCST_EX_USER_DEFINED_14						0x00020000
#define __ECCST_EX_USER_DEFINED_13						0x00040000
#define __ECCST_EX_USER_DEFINED_12						0x00080000
#define __ECCST_EX_USER_DEFINED_11						0x00100000
#define __ECCST_EX_USER_DEFINED_10						0x00200000
#define __ECCST_EX_USER_DEFINED_09						0x00400000
#define __ECCST_EX_USER_DEFINED_08						0x00800000
#define __ECCST_EX_USER_DEFINED_07						0x01000000
#define __ECCST_EX_USER_DEFINED_06						0x02000000
#define __ECCST_EX_USER_DEFINED_05						0x04000000
#define __ECCST_EX_USER_DEFINED_04						0x08000000
#define __ECCST_EX_USER_DEFINED_03						0x10000000
#define __ECCST_EX_USER_DEFINED_02						0x20000000
#define __ECCST_EX_USER_DEFINED_01						0x40000000
#define __ECCST_EX_USER_DEFINED_00						0x80000000

#define __ECCST_EX_DEFAUT								(0)

class __PROF_UIS_API CExtCustomizeCmdScreenTip : public CObject
{
protected:
	DWORD m_dwFlags, m_dwFlagsEx;
	CExtSafeString m_strCaptionMain, m_strCaptionSecondary, m_strTextMain, m_strTextSecondary;
	CExtBitmap m_bmpMain, m_bmpSecondary;
public:
	DECLARE_SERIAL( CExtCustomizeCmdScreenTip );
	CExtCustomizeCmdScreenTip();
	CExtCustomizeCmdScreenTip(
		const CExtCustomizeCmdScreenTip & other
		);
	virtual ~CExtCustomizeCmdScreenTip();
	virtual void Empty();
	virtual bool IsEmpty() const;
	virtual CExtCustomizeCmdScreenTip * Clone() const;
	virtual void Assign(
		const CExtCustomizeCmdScreenTip & other
		);
	CExtCustomizeCmdScreenTip & operator = (
		const CExtCustomizeCmdScreenTip & other
		);
	virtual bool IsEqualToOther(
		const CExtCustomizeCmdScreenTip & other
		) const;
	bool operator == (
		const CExtCustomizeCmdScreenTip & other
		) const;
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif // _DEBUG
	virtual void Serialize( CArchive & ar );
	CArchive & operator << ( CArchive & ar );
	CArchive & operator >> ( CArchive & ar );
	virtual DWORD GetFlags() const;
	virtual DWORD ModifyFlags(
		DWORD dwAdd,
		DWORD dwRemove = 0
		);
	virtual DWORD GetFlagsEx() const;
	virtual DWORD ModifyFlagsEx(
		DWORD dwAdd,
		DWORD dwRemove = 0
		);
	virtual __EXT_MFC_SAFE_LPCTSTR CaptionMainGet() const;
	virtual void CaptionMainSet(
		__EXT_MFC_SAFE_LPCTSTR strCaption = NULL
		);
	virtual __EXT_MFC_SAFE_LPCTSTR CaptionSecondaryGet() const;
	virtual void CaptionSecondarySet(
		__EXT_MFC_SAFE_LPCTSTR strCaption = NULL
		);
	virtual __EXT_MFC_SAFE_LPCTSTR TextMainGet() const;
	virtual void TextMainSet(
		__EXT_MFC_SAFE_LPCTSTR strText = NULL
		);
	virtual __EXT_MFC_SAFE_LPCTSTR TextSecondaryGet() const;
	virtual void TextSecondarySet(
		__EXT_MFC_SAFE_LPCTSTR strText = NULL
		);
	virtual CExtBitmap & BmpMainGet();
	const CExtBitmap & BmpMainGet() const;
	virtual void BmpMainSet(
		const CExtBitmap & _bmp
		);
	virtual CExtBitmap & BmpSecondaryGet();
	const CExtBitmap & BmpSecondaryGet() const;
	virtual void BmpSecondarySet(
		const CExtBitmap & _bmp
		);
	static CExtCustomizeCmdScreenTip * stat_CmdScreenTipFromArchive(
		CArchive & ar
		);
	static void stat_CmdScreenTipToArchive(
		CArchive & ar,
		CExtCustomizeCmdScreenTip * pCmdScreenTip
		);
	virtual CSize Measure(
		CDC & dc,
		CExtPaintManager * pPM = NULL
		);
	virtual void Paint(
		CDC & dc,
		const RECT & rcScreenTip,
		CExtPaintManager * pPM = NULL
		);
}; // class CExtCustomizeCmdScreenTip

/////////////////////////////////////////////////////////////////////////////
// CExtCustomizeCmdKeyTip

class __PROF_UIS_API CExtCustomizeCmdKeyTip : public CObject
{
protected:
	DWORD m_dwFlags, m_dwFlagsEx;
	CArray < DWORD, DWORD & > m_arrKeyInfo;
public:
	DECLARE_SERIAL( CExtCustomizeCmdKeyTip );
	CExtCustomizeCmdKeyTip();
	CExtCustomizeCmdKeyTip(
		const CExtCustomizeCmdKeyTip & other
		);
	CExtCustomizeCmdKeyTip(
		__EXT_MFC_SAFE_LPCTSTR str
		);
	CExtCustomizeCmdKeyTip(
		__EXT_MFC_SAFE_TCHAR _tchr
		);
	virtual ~CExtCustomizeCmdKeyTip();
	virtual void Empty();
	virtual bool IsEmpty() const;
	virtual CExtCustomizeCmdKeyTip * Clone() const;
	virtual void Assign(
		const CExtCustomizeCmdKeyTip & other
		);
	virtual void Assign(
		__EXT_MFC_SAFE_LPCTSTR str
		);
	void Assign(
		__EXT_MFC_SAFE_TCHAR _tchr
		);
	CExtCustomizeCmdKeyTip & operator = (
		const CExtCustomizeCmdKeyTip & other
		);
	virtual bool IsEqualToOther(
		const CExtCustomizeCmdKeyTip & other
		) const;
	bool operator == (
		const CExtCustomizeCmdKeyTip & other
		) const;
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif // _DEBUG
	virtual void Serialize( CArchive & ar );
	CArchive & operator << ( CArchive & ar );
	CArchive & operator >> ( CArchive & ar );
	static CExtCustomizeCmdKeyTip * stat_CmdKeyTipFromText(
		__EXT_MFC_SAFE_LPCTSTR str
		);
	static CExtCustomizeCmdKeyTip * stat_CmdKeyTipFromText(
		__EXT_MFC_SAFE_TCHAR _tchr
		);
	static CExtCustomizeCmdKeyTip * stat_CmdKeyTipFromArchive(
		CArchive & ar
		);
	static void stat_CmdKeyTipToArchive(
		CArchive & ar,
		CExtCustomizeCmdKeyTip * pCmdKeyTip
		);
	virtual CExtSafeString GetTipText(
		INT nIndent = 0
		) const;
	virtual DWORD GetFlags() const;
	virtual DWORD ModifyFlags(
		DWORD dwAdd,
		DWORD dwRemove = 0
		);
	virtual DWORD GetFlagsEx() const;
	virtual DWORD ModifyFlagsEx(
		DWORD dwAdd,
		DWORD dwRemove = 0
		);
	virtual INT KeyCodeGetCount() const;
	virtual DWORD KeyCodeGetAt( INT nIndex ) const;
	virtual void KeyCodeSetAt( INT nIndex, DWORD dwKeyCode );
	virtual void KeyCodeInsertAt( INT nIndex, DWORD dwKeyCode ); // nIndex=-1 - append to end
	void KeyCodeAdd( DWORD dwKeyCode );
	bool KeyCodeRemoveLast();
	virtual INT KeyCodeRemoveAt( INT nIndex, INT nCount );
	virtual void KeyCodeRemoveAll();
	virtual bool CompareWidthChain(
		CExtCustomizeCmdKeyTip & keyTipChain,
		INT nLengthToCompare = -1 // if negative - use keyTipChain.KeyCodeGetCount()
		);
}; // class CExtCustomizeCmdKeyTip

/////////////////////////////////////////////////////////////////////////////
// CExtCustomizeCmdTreeNode

// tree node flags

// text/icon display types
#define __ECTN_DISPLAY_DEFAULT_STYLE		0x00000000
#define __ECTN_DISPLAY_TEXTONLY_IN_MENU		0x00000001
#define __ECTN_DISPLAY_TEXTONLY_ALWAYS		0x00000002
#define __ECTN_DISPLAY_TEXT_AND_IMAGE		0x00000003
#define __ECTN_DISPLAY_MASK					0x00000003
// is pop-up menu item
#define __ECTN_AUTO_POPUP					0x00000004
// the dynamic command has been registered for this pop-up menu item
#define __ECTN_DYNAMIC_POPUP				0x00000008
// has separator before
#define __ECTN_GROUP_START					0x00000010
// append MDI menu to this button
#define __ECTN_TBB_APPEND_MDI_MENU			0x00000020
// use separated drop-down
#define __ECTN_TBB_SEPARATED_DROPDOWN		0x00000040
// autochange button ID (for drop-down buttons)
#define __ECTN_TBB_AUTOCHANGE_ID			0x00000080
// no rotate in vertically docked toolbars
#define __ECTN_TBB_NO_ROTATE_VL				0x00000100
// button can not be hidden in toolbar
#define __ECTN_TBB_CANNOT_HIDE				0x00000200
// button is hidden in toolbar
#define __ECTN_TBB_HIDDEN					0x00000400

#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
// button is text field
#define __ECTN_TBB_TEXT_FIELD				0x00000800
// text field is not editable
// (only with __ECTN_TBB_TEXT_FIELD)
#define __ECTN_TBB_TF_NE					0x00001000
// text field is able to drop list-box
// (only with __ECTN_TBB_TEXT_FIELD)
#define __ECTN_TBB_COMBO					0x00002000
// text field is able to drop date picker
// (only with __ECTN_TBB_TEXT_FIELD)
#define __ECTN_TBB_DATE						0x00080000
// text field is resizable
// (only with __ECTN_TBB_TEXT_FIELD)
#define __ECTN_TBB_RESIZABLE				0x00004000
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)

// color button
#define __ECTN_TBB_COLOR					0x00008000
// "default" button on color menu
#define __ECTN_TBB_MCB_DEFAULT				0x00010000
// "custom" button on color menu
#define __ECTN_TBB_MCB_CUSTOM				0x00020000

// undo/redo button
#define __ECTN_TBB_UNDO_REDO				0x00100000

// control is visible in vertical layout
#define __ECTN_TBB_CTRL_VIS_VL				0x00040000

#if (!defined __EXT_MFC_NO_RIBBON_BAR)

	// alignment inside ribbon groups
	#define __ECTN_RGA_HORIZONTAL			0x00200000
	#define __ECTN_RGA_VERTICAL				0x00400000
	#define __ECTN_RGA_TOOL_ROW_VERTICAL	0x00800000
	#define __ECTN_RGA_DEFAULT				(__ECTN_RGA_VERTICAL|__ECTN_RGA_TOOL_ROW_VERTICAL)

	// cloned node in ribbon's quick access area
	#define __ECTN_RIBBON_QA_CLONED_COPY	0x01000000

#endif // (!defined __EXT_MFC_NO_RIBBON_BAR)

// check box button
#define __ECTN_TBB_CHECK_BOX				0x02000000
// radio box button
#define __ECTN_TBB_RADIO_BOX				0x04000000

// bold font in menu
#define __ECTN_BOLD_IN_MENU					0x08000000

// big accent in menu
#define __ECTN_BIG_ACCENT_IN_MENU			0x10000000

// have extended flags (for serialization)
#define __ECTN_HAVE_EXTENDED_FLAGS			0x20000000

#define __ECTN_USER_DEFINED_01				0x40000000
#define __ECTN_USER_DEFINED_00				0x80000000

// tree node extended flags

// have customize cmd shortcut information (basic and drop-down)
#define __ECTN_EX_HAVE_CCS_BASIC			0x00000001
#define __ECTN_EX_HAVE_CCS_DD				0x00000002

#if (!defined __EXT_MFC_NO_RIBBON_BAR)
	// ribbon file button specific cmd shortcut information
	#define __ECTN_EX_HAVE_CCS_RFB			0x00000004
#endif // (!defined __EXT_MFC_NO_RIBBON_BAR)

#define __ECTN_EX_HAVE_SCREEN_TIP_BASIC		0x00000008
#define __ECTN_EX_HAVE_SCREEN_TIP_DD		0x00000010

#define __ECTN_EX_HAVE_OTHER_FLAGS			(0xFFFFFFFF&(~(__ECTN_EX_HAVE_CCS_BASIC|__ECTN_EX_HAVE_CCS_DD|__ECTN_EX_HAVE_SCREEN_TIP_BASIC|__ECTN_EX_HAVE_SCREEN_TIP_DD)))

#define __ECTN_EX_USER_DEFINED_26			0x00000020
#define __ECTN_EX_USER_DEFINED_25			0x00000040
#define __ECTN_EX_USER_DEFINED_24			0x00000080
#define __ECTN_EX_USER_DEFINED_23			0x00000100
#define __ECTN_EX_USER_DEFINED_22			0x00000200
#define __ECTN_EX_USER_DEFINED_21			0x00000400
#define __ECTN_EX_USER_DEFINED_20			0x00000800
#define __ECTN_EX_USER_DEFINED_19			0x00001000
#define __ECTN_EX_USER_DEFINED_18			0x00002000
#define __ECTN_EX_USER_DEFINED_17			0x00004000
#define __ECTN_EX_USER_DEFINED_16			0x00008000
#define __ECTN_EX_USER_DEFINED_15			0x00010000
#define __ECTN_EX_USER_DEFINED_14			0x00020000
#define __ECTN_EX_USER_DEFINED_13			0x00040000
#define __ECTN_EX_USER_DEFINED_12			0x00080000
#define __ECTN_EX_USER_DEFINED_11			0x00100000
#define __ECTN_EX_USER_DEFINED_10			0x00200000
#define __ECTN_EX_USER_DEFINED_09			0x00400000
#define __ECTN_EX_USER_DEFINED_08			0x00800000
#define __ECTN_EX_USER_DEFINED_07			0x01000000
#define __ECTN_EX_USER_DEFINED_06			0x02000000
#define __ECTN_EX_USER_DEFINED_05			0x04000000
#define __ECTN_EX_USER_DEFINED_04			0x08000000
#define __ECTN_EX_USER_DEFINED_03			0x10000000
#define __ECTN_EX_USER_DEFINED_02			0x20000000
#define __ECTN_EX_USER_DEFINED_01			0x40000000
#define __ECTN_EX_USER_DEFINED_00			0x80000000

class __PROF_UIS_API CExtCustomizeCmdTreeNode : public CObject
{
protected:
	UINT m_nCmdIdBasic, m_nCmdIdEffective;
	CExtCustomizeCmdTreeNode * m_pParentNode;
	CObArray m_arrChilds;
	DWORD m_dwFlags, m_dwFlagsEx;
	CExtSafeString m_strTextInToolbar, m_strTextInMenu,
		m_strTextUser, m_strTextMenuExtended, m_strTextMenuCaption;
	LPARAM m_lParam;
public:
	CExtCmdIcon m_iconCustomized;
protected:
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	INT m_nTextFieldWidth;
	INT m_nDropDownWidth; // (-1) - auto calc, (-2) - same as button area
	INT m_nDropDownHeightMax;
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	CExtCustomizeCmdKeyTip * m_pCmdKeyTipBasic, * m_pCmdKeyTipDropDown;
	CExtCustomizeCmdScreenTip * m_pCmdScreenTipBasic, * m_pCmdScreenTipDropDown;
public:
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	CExtSafeString m_sDefInplaceEditBuffer;
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	DECLARE_SERIAL( CExtCustomizeCmdTreeNode );
	CExtCustomizeCmdTreeNode(
		UINT nCmdIdBasic = 0L,
		UINT nCmdIdEffective = 0L,
		CExtCustomizeCmdTreeNode * pParentNode = NULL,
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
	CExtCustomizeCmdTreeNode(
		CExtCustomizeCmdTreeNode & other
		);
	CExtCustomizeCmdTreeNode(
		CExtCmdItem * pCmdItem, // NOT NULL!
		CExtCmdIcon * pIconCustomized = NULL
		);
	virtual ~CExtCustomizeCmdTreeNode();
	CExtCustomizeCmdTreeNode & operator = (
		CExtCustomizeCmdTreeNode & other
		);
	virtual void AssignFromOther(
		CExtCustomizeCmdTreeNode & other
		);
	virtual void AssignCmdProps(
		CExtCmdItem * pCmdItem, // NOT NULL!
		CExtCmdIcon * pIconCustomized = NULL
		);
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif // _DEBUG
	virtual void Serialize( CArchive & ar );
	CArchive & operator << ( CArchive & ar )
	{
		Serialize( ar );
		return ar;
	}
	CArchive & operator >> ( CArchive & ar )
	{
		Serialize( ar );
		return ar;
	}
	
	virtual UINT GetCmdID(
		bool bEffective = true
		) const;
	virtual UINT SetCmdID(
		UINT nCmdID,
		bool bEffectiveOnly = false
		);

	virtual CExtCustomizeCmdKeyTip * CmdKeyTipGet(
		bool bBasic = true
		);
	const CExtCustomizeCmdKeyTip * CmdKeyTipGet(
		bool bBasic = true
		) const;
	virtual CExtCustomizeCmdKeyTip * CmdKeyTipDetach(
		bool bBasic = true
		);
	void CmdKeyTipDestroy(
		bool bBasic = true
		);
	virtual void CmdKeyTipSet(
		CExtCustomizeCmdKeyTip * pCmdKeyTip,
		bool bClone,
		bool bBasic = true
		);
	void CmdKeyTipAttach(
		CExtCustomizeCmdKeyTip * pCmdKeyTip,
		bool bBasic = true
		);

	virtual CExtCustomizeCmdScreenTip * CmdScreenTipGet(
		bool bBasic = true
		);
	const CExtCustomizeCmdScreenTip * CmdScreenTipGet(
		bool bBasic = true
		) const;
	virtual CExtCustomizeCmdScreenTip * CmdScreenTipDetach(
		bool bBasic = true
		);
	void CmdScreenTipDestroy(
		bool bBasic = true
		);
	virtual void CmdScreenTipSet(
		CExtCustomizeCmdScreenTip * pCmdScreenTip,
		bool bClone,
		bool bBasic = true
		);
	void CmdScreenTipAttach(
		CExtCustomizeCmdScreenTip * pCmdScreenTip,
		bool bBasic = true
		);

	LONG GetIndent() const;
	virtual DWORD GetFlags() const;
	virtual DWORD ModifyFlags(
		DWORD dwAdd,
		DWORD dwRemove = 0
		);
	virtual DWORD GetFlagsEx() const;
	virtual DWORD ModifyFlagsEx(
		DWORD dwAdd,
		DWORD dwRemove = 0
		);

	virtual bool CanChangeDisplayStyle(
		CExtCmdItem * pCmdItem
		);

	virtual CExtCmdIcon * CurrentIconGetPtr(
		bool bAllowEmptyIcon,
		CExtCmdItem * pCmdItem
		);
	virtual CExtCmdIcon * GetIconPtrInToolbar(
		CExtCmdItem * pCmdItem,
		bool bForceBasic = false
		);
	virtual CExtCmdIcon * GetIconPtrInMenu(
		CExtCmdItem * pCmdItem,
		bool bForceBasic = false
		);
	virtual void ResetIcon(
		CExtCmdItem * pCmdItem
		);
	
	virtual __EXT_MFC_SAFE_LPCTSTR GetTextInToolbar(
		CExtCmdItem * pCmdItem,
		bool bDoNotTestStyles = false
		);
	virtual void SetTextInToolbar(
		__EXT_MFC_SAFE_LPCTSTR strText = NULL
		);

	virtual __EXT_MFC_SAFE_LPCTSTR GetTextInMenu(
		CExtCmdItem * pCmdItem,
		bool bDoNotTestStyles = false
		);
	virtual void SetTextInMenu(
		__EXT_MFC_SAFE_LPCTSTR strText = NULL
		);

	virtual __EXT_MFC_SAFE_LPCTSTR GetTextUser() const;
	virtual void SetTextUser( __EXT_MFC_SAFE_LPCTSTR strText = NULL );

	virtual __EXT_MFC_SAFE_LPCTSTR GetTextMenuExtended() const;
	virtual void SetTextMenuExtended( __EXT_MFC_SAFE_LPCTSTR strText = NULL );

	virtual __EXT_MFC_SAFE_LPCTSTR GetTextMenuCaption() const;
	virtual void SetTextMenuCaption( __EXT_MFC_SAFE_LPCTSTR strText = NULL );

	virtual LPARAM GetLParam() const;
	virtual LPARAM SetLParam( LPARAM lParam = 0L );

	virtual CExtCustomizeCmdTreeNode * GetParentNode();
	const CExtCustomizeCmdTreeNode * GetParentNode() const;
	virtual CExtCustomizeCmdTreeNode * SetParentNode(
		CExtCustomizeCmdTreeNode * pNewParentNode
		);
	virtual INT InsertClonedChildsNodesFrom(
		CExtCustomizeSite * pSite,
		CExtCustomizeCmdTreeNode * pNode
		);
	virtual CExtCustomizeCmdTreeNode * CloneNode(
		CExtCustomizeSite * pSite,
		bool bCloneChilds = true
		);
	
	// complete clone, serialize-based method, no customize site notifications
	CExtCustomizeCmdTreeNode * CloneNode();

	virtual bool LoadMenuTree(
		CWnd * pWndTop,
		CExtCustomizeSite * pSite,
		CMenu * pMenu, // all the menu commands should be registered in the command manager
		bool bPopupMenu = true
		);
	bool LoadMenuTree(
		CWnd * pWndTop,
		CExtCustomizeSite * pSite,
		UINT nResourceID,
		bool bPopupMenu = true
		);
	bool LoadMenuTree(
		CWnd * pWndTop,
		CExtCustomizeSite * pSite,
		__EXT_MFC_SAFE_LPCTSTR sResourceID,
		bool bPopupMenu = true
		);
	
	virtual void InsertBarNode(
		CExtCustomizeSite * pSite,
		CExtCustomizeCmdTreeNode * pNode,
		INT nPos = -1 // append
		);
	virtual void InsertNode(
		CExtCustomizeSite * pSite,
		CExtCustomizeCmdTreeNode * pNode,
		INT nPos = -1 // append
		);
	virtual INT RemoveNodes( // returns removed count
		INT nPos = 0,
		INT nCount = 1 // -1 - remove all
		);
	virtual void RemoveSelf(
		CExtCmdItem * pCmdItem,
		bool bAutoSyncNextSeparator = false
		);
	INT RemoveAllNodes()
	{
		ASSERT_VALID( this );
		return RemoveNodes( 0, -1 );
	}
	virtual void Swap(
		INT nIndex1,
		INT nIndex2,
		DWORD dwFlagsToLeave = 0
		);
	virtual INT GetNodeCount() const;
	virtual CExtCustomizeCmdTreeNode * ElementAt(
		INT nPos
		);
	CExtCustomizeCmdTreeNode * operator [] ( INT nPos )
	{
		return ElementAt( nPos );
	}
	CExtCustomizeCmdTreeNode * Head()
	{
#ifdef _DEBUG
		INT nNodeCount = GetNodeCount();
		ASSERT( nNodeCount > 0 );
		nNodeCount;
#endif // _DEBUG
		return ElementAt( 0 );
	}
	CExtCustomizeCmdTreeNode * Tail()
	{
		INT nNodeCount = GetNodeCount();
		ASSERT( nNodeCount > 0 );
		return ElementAt( nNodeCount - 1 );
	}
	virtual INT GetNodeIndex(
		CExtCustomizeCmdTreeNode * pNode
		);
	INT GetOwnIndex()
	{
		ASSERT_VALID( this );
		ASSERT( GetParentNode() != NULL );
		return GetParentNode()->GetNodeIndex( this );
	}
	INT RemoveCommand(
		UINT nCmdID,
		bool bUseEffectiveCmdID = false,
		bool bDeep = true
		);
	virtual INT RemoveCommandRange(
		UINT nMinCmdID,
		UINT nMaxCmdID,
		bool bUseEffectiveCmdID = false,
		bool bDeep = true
		);
	virtual INT SearchNode(
		UINT nCmdID,
		INT nPosStartSearch = -1, // start from 0
		bool bUseEffectiveCmdID = false
		);
	virtual INT SearchNodeBackward(
		UINT nCmdID,
		INT nPosStartSearch = -1, // start from (node_count - 1)
		bool bUseEffectiveCmdID = false
		);
	virtual INT SearchNode(
		__EXT_MFC_SAFE_LPCTSTR strText, // NULL or _T("") - unnamed
		INT nTextType, // 0-toolbar, 1-menu, 2-user
		INT nPosStartSearch = -1 // start from 0
		);
	virtual INT SearchNodeBackward(
		__EXT_MFC_SAFE_LPCTSTR strText, // NULL or _T("") - unnamed
		INT nTextType, // 0-toolbar, 1-menu, 2-user
		INT nPosStartSearch = -1 // start from (node_count - 1)
		);
	CExtCustomizeCmdTreeNode * SearchNodeElement(
		UINT nCmdID,
		INT nPosStartSearch = -1, // start from 0
		bool bUseEffectiveCmdID = false,
		bool bDeep = false
		);
	CExtCustomizeCmdTreeNode * SearchNodeElement(
		__EXT_MFC_SAFE_LPCTSTR strText, // NULL or _T("") - unnamed
		INT nTextType, // 0-toolbar, 1-menu, 2-user
		INT nPosStartSearch = -1 // start from 0
		);
	virtual bool IsChild(
		CExtCustomizeCmdTreeNode * pChildTest
		);
	virtual bool IsPopupNode() const;

#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	virtual INT TextFieldWidthGet() const;
	virtual void TextFieldWidthSet( INT nVal );
	virtual INT DropDownHeightMaxGet() const;
	virtual void DropDownHeightMaxSet( INT nVal );
	virtual INT DropDownWidthGet() const;
	virtual void DropDownWidthSet( INT nVal );
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)

#if (!defined __EXT_MFC_NO_RIBBON_BAR)
	virtual void Ribbon_InitCommandsListBox(
		CExtCustomizeCommandListBox & _LB,
		INT nDstIndex = -1,
		bool bInsertThisNode = true,
		bool bInsertChildrenNodes = true
		);
	virtual bool Ribbon_InitCommandProfile(
		CExtCmdProfile * pProfile,
		bool bInit
		);
	virtual bool Ribbon_InitCommandProfileIcon(
		CExtCmdProfile * pProfile,
		bool bInit
		);
	bool Ribbon_InitCommandProfile(
		__EXT_MFC_SAFE_LPCTSTR strCmdProfileName,
		bool bInit
		);
	bool Ribbon_InitCommandProfile(
		CWnd * pWnd,
		bool bInit
		);
	bool Ribbon_InitCommandProfile(
		HWND hWnd,
		bool bInit
		);
#endif // (!defined __EXT_MFC_NO_RIBBON_BAR)

	virtual bool PreConstructMenuItem(
		LPVOID pMenuItemData,
		HWND hWndCmdRecv,
		int nItemIndex,
		CExtPopupMenuWnd * pPopupParent
		);
	virtual void PostConstructMenuItem(
		LPVOID pMenuItemData,
		HWND hWndCmdRecv,
		int nItemIndex,
		CExtPopupMenuWnd * pPopupParent
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

}; // class CExtCustomizeCmdTreeNode

/////////////////////////////////////////////////////////////////////////////
// CExtCustomizeSite

// customize site flags

// allow toolbars page
#define __ECSF_BARS								0x00000001
// allow customize toolbar/menu commands
#define __ECSF_COMMANDS							0x00000002
// allow new/delete user defined toolbars
#define __ECSF_USER_BARS						0x00000004
// allow customize keyboard accelerators
#define __ECSF_ACCELERATORS						0x00000008
// allow keyboard accelerators without Ctrl/Shift/Alt
#define __ECSF_ALLOW_UNMODIFIED_ACCELERATORS	0x00000010
// allow parms page
#define __ECSF_PARMS							0x00000020
// disable "Personalized Menus and Toolbars" parms
#define __ECSF_PARMS_DISABLE_PERSONALIZED		0x00000040
// disable "Other" parms
#define __ECSF_PARMS_DISABLE_OTHER				0x00000080

#define __ECSF_DEFAULT \
	(	__ECSF_BARS \
		|__ECSF_COMMANDS \
		| __ECSF_USER_BARS \
		| __ECSF_ACCELERATORS \
		| __ECSF_PARMS \
	)

class __PROF_UIS_API CExtCustomizeSite
	: public CExtHookSink
	, public CExtPmBridge
{
public:
	class __PROF_UIS_API CResizerWnd : public CWnd
	{
		CExtCustomizeSite * m_pSite;
		HWND m_hWndBase;
		HCURSOR m_hCursor;
		COLORREF m_clrRect;
		CSize m_sizeRectMetric;
		CRect m_rcScreenCurrent;
		CRgn m_rgnCurrent;
		int m_nResizingHT, m_nExtentMin, m_nExtentMax;
		bool m_bHorz:1;
		void _RecalcCurrentRegion();
		void _ActivateState(
			bool bUseCurrentWindowRect
			);
		void _AdjustCurrentRect();
		void _RepaintBase();
	public:
		CResizerWnd(
			CExtCustomizeSite * pSite,
			bool bHorz = true
			);
		virtual ~CResizerWnd();
		bool DoResizerTracking(
			HWND hWndBase,
			CRect & rcItemClient, // in client of hWndParent
			int nResizingHT, // must be -1 or 1
			int nExtentMin,
			int nExtentMax
			);
		virtual LRESULT WindowProc(
			UINT message,
			WPARAM wParam,
			LPARAM lParam
			);
	}; // class CResizerWnd

	class __PROF_UIS_API CToolBarCustomizeInfo
	{
	public:
		bool m_bUserToolBar:1;
		CExtCustomizeCmdTreeNode
			* m_pHelperBarButtonsI,
			* m_pHelperBarButtonsC;
		CExtSafeString m_strDisplayName;
		CToolBarCustomizeInfo();
		~CToolBarCustomizeInfo();
		friend class CExtCustomizeSite;
	}; // class CToolBarCustomizeInfo
	
	static UINT g_nMsgQueryCustomizeSite;
	static UINT g_nMsgAssignAccelTable;
	static UINT g_nMsgSyncAccelTable;
	static UINT g_nMsgSyncContent;
private:
	static UINT g_nMsgDelayCustomization;
public:

	class __PROF_UIS_API CCmdKeyInfo
	{
		bool m_bVirt:1;
		bool m_bCtrl:1;
		bool m_bAlt:1;
		bool m_bShift:1;
		WORD m_nKey;
	public:
		CCmdKeyInfo( const ACCEL & _dataAccel );
		CCmdKeyInfo(
			bool bVirt = false,
			bool bCtrl = false,
			bool bAlt = false,
			bool bShift = false,
			WORD nKey = 0
			);
		CCmdKeyInfo( const CCmdKeyInfo & other );
		CCmdKeyInfo( BYTE nVirtFlags, WORD nKey );
		CCmdKeyInfo( DWORD dwKeyInfo );
		~CCmdKeyInfo();
		CCmdKeyInfo & operator = ( const ACCEL & _dataAccel );
		CCmdKeyInfo & operator = ( const CCmdKeyInfo & other );
		CCmdKeyInfo & operator = ( DWORD dwKeyInfo );
		bool operator != ( const CCmdKeyInfo & other ) const;
		bool operator == ( const CCmdKeyInfo & other ) const;
		bool operator <  ( const CCmdKeyInfo & other ) const;
		bool operator <= ( const CCmdKeyInfo & other ) const;
		bool operator >  ( const CCmdKeyInfo & other ) const;
		bool operator >= ( const CCmdKeyInfo & other ) const;
		operator DWORD() const;
		bool IsVirt() const;
		void SetVirt( bool bOn );
		bool IsCtrl() const;
		void SetCtrl( bool bOn );
		bool IsAlt() const;
		void SetAlt( bool bOn );
		bool IsShift() const;
		void SetShift( bool bOn );
		WORD GetKey() const;
		void SetKey( WORD nKey );
		bool IsEmpty() const;
		void Empty();
		void FillAccel( ACCEL & _dataAccel ) const;
		void Serialize( CArchive & ar );
	}; // class CCmdKeyInfo

	class __PROF_UIS_API CCmdKeySet
		: public CArray < CCmdKeyInfo, CCmdKeyInfo >
	{
	public:
		CCmdKeySet();
		CCmdKeySet( const CCmdKeySet & other );
		~CCmdKeySet();
		CCmdKeySet & operator = ( const CCmdKeySet & other );
		bool IsEmpty() const;
		void Empty();
		bool KeySetup(
			const CCmdKeyInfo & _cmdKeyInfo,
			int nPos = -1 // append
			);
		bool KeyRemove(
			const CCmdKeyInfo & _cmdKeyInfo
			);
		int KeyFindPos(
			const CCmdKeyInfo & _cmdKeyInfo
			) const;
		void Serialize( CArchive & ar );
	}; // class CCmdKeySet

	class __PROF_UIS_API CMapCmd2KeySet
		: public CMap < UINT, UINT, CCmdKeySet, CCmdKeySet >
	{
	public:
		CMapCmd2KeySet();
		~CMapCmd2KeySet();
		void CmdKeySetup(
			UINT nCmdID,
			const CCmdKeyInfo & _cmdKeyInfo,
			int nPos = -1 // append
			);
		void CmdKeyRemove(
			UINT nCmdID,
			const CCmdKeyInfo & _cmdKeyInfo
			);
		void CmdRemove(
			UINT nCmdID
			);
		void Serialize( CArchive & ar );
		CMapCmd2KeySet & operator = ( const CMapCmd2KeySet & other );
	}; // class CMapCmd2KeySet

	class __PROF_UIS_API CMapKey2Cmd
		: public CMap < DWORD, DWORD, UINT, UINT >
	{
	public:
		CMapKey2Cmd();
		~CMapKey2Cmd();
		void KeyCommandSet(
			const CCmdKeyInfo & _cmdKeyInfo,
			UINT nCmdID
			);
		UINT KeyCommandGet(
			const CCmdKeyInfo & _cmdKeyInfo
			) const;
		void KeyCommandRemove(
			const CCmdKeyInfo & _cmdKeyInfo
			);
		POSITION KeyGetStartPosition() const;
		void KeyGetNext(
			POSITION & pos,
			CCmdKeyInfo * pCmdKeyInfo,
			LPUINT p_nCmdID = NULL
			) const;
		void KeyFillCmdSet( CMapCmd2KeySet & _mapCmd2KeySet );
		HACCEL KeyGenAccelTable();
		CMapKey2Cmd & operator = ( const CMapKey2Cmd & other );
		void Serialize( CArchive & ar );
	}; // class CMapKey2Cmd

	class __PROF_UIS_API CCmdMenuInfo
	{
		CExtCustomizeSite * m_pSite;
		CExtSafeString m_strName;
		CExtCustomizeCmdTreeNode
			* m_pNodeI,
			* m_pNodeC;
		bool m_bDefaultMenu:1;
		CPtrList m_listRtcFrames, m_listRtcViews, m_listRtcDocs;
		CMapCmd2KeySet m_mapCmd2KeySet;
		CMapKey2Cmd m_mapKey2Cmd, m_mapInitialKey2Cmd;
		HACCEL m_hAccelTable;
		bool m_bAccelTableSyncronized:1;
	public:
		CCmdMenuInfo(
			CExtCustomizeSite * pSite,
			__EXT_MFC_SAFE_LPCTSTR strName
			);
		~CCmdMenuInfo();
		bool IsDefaultMenu() const
		{
			return m_bDefaultMenu;
		}
		bool InitializeEmpty(
			CWnd * pWndTop,
			bool bDefaultMenu
			);
		bool Initialize(
			CWnd * pWndTop,
			CMenu * pMenu,
			bool bDefaultMenu,
			bool bPopupMenu
			);
		bool Initialize(
			CWnd * pWndTop,
			__EXT_MFC_SAFE_LPCTSTR strResourceID,
			bool bDefaultMenu,
			bool bPopupMenu
			);
		bool Initialize(
			CWnd * pWndTop,
			UINT nResourceID,
			bool bDefaultMenu,
			bool bPopupMenu
			);
		bool IsInitialized() const;
		__EXT_MFC_SAFE_LPCTSTR GetName() const;
		bool AddRuntimeClassFrame(
			CRuntimeClass * pRTC
			);
		bool AddRuntimeClassView(
			CRuntimeClass * pRTC
			);
		bool AddRuntimeClassDocument(
			CRuntimeClass * pRTC
			);
		bool IsFrame(
			CObject * pObjTest
			);
		bool IsView(
			CObject * pObjTest
			);
		bool IsDocument(
			CObject * pObjTest
			);
		CExtCustomizeCmdTreeNode * GetNode(
			bool bInitial = false
			);
		HACCEL AccelTableGetSafe( bool bReturnClonedCopy );
		HACCEL AccelTableGet( bool bReturnClonedCopy );
		bool AccelTableSet(
			HACCEL hAccelTable,
			bool bUseTableCopy
			);
		bool AccelTableLoad(
			__EXT_MFC_SAFE_LPCTSTR strResourceID
			);
		bool AccelTableLoad(
			UINT nResourceID
			);
		bool AccelTableLoad(
			HINSTANCE hInst,
			__EXT_MFC_SAFE_LPCTSTR strResourceID
			);
		bool AccelTableLoad(
			HINSTANCE hInst,
			UINT nResourceID
			);
		void AccelTableSync();
		bool AccelTableCmdKeySetGet(
			UINT nCmdID,
			CCmdKeySet & _cmdKeySet
			) const;
		void AccelTableCmdKeyAssign(
			UINT nCmdID,
			const CCmdKeyInfo & _cmdKeyInfo,
			int nPosInList = -1 // append
			);
		UINT AccelTableCmdFromKey(
			const CCmdKeyInfo & _cmdKeyInfo
			) const;
		UINT AccelTableCmdKeyRemove(
			const CCmdKeyInfo & _cmdKeyInfo
			);
		int AccelTableCmdRemove(
			UINT nCmdID
			);
		void AccelTableReset();
		void AccelTableSerialize( CArchive & ar );
		CMapCmd2KeySet & GetMapCmd2KeySet();
		const CMapCmd2KeySet & GetMapCmd2KeySet() const;
		CMapKey2Cmd & GetMapKey2Cmd();
		const CMapKey2Cmd & GetMapKey2Cmd() const;
		CMapKey2Cmd & GetMapInitialKey2Cmd();
		const CMapKey2Cmd & GetMapInitialKey2Cmd() const;
	}; // class CCmdMenuInfo

	struct __PROF_UIS_API CCmdDragInfo
	{
		CExtCustomizeCmdTreeNode
			* m_pDropNodeI,
			* m_pDropNodeC;
		CCmdDragInfo(
			CExtCustomizeCmdTreeNode * pDropNodeI = NULL,
			CExtCustomizeCmdTreeNode * pDropNodeC = NULL
			);
		CCmdDragInfo(
			COleDataObject * pDataObject,
			CLIPFORMAT cf
			);
		CCmdDragInfo( CCmdDragInfo & other );
		CCmdDragInfo & operator = ( CCmdDragInfo & other );
		bool IsEmpty() const;
		void CacheGlobalData(
			COleDataSource & _oleDataSource,
			CLIPFORMAT cf
			);
	}; // struct CCmdDragInfo

	struct __PROF_UIS_API ICustomizeDropTarget
	{
		virtual DROPEFFECT OnCustomizeTargetEnter(
			CCmdDragInfo & _dragInfo,
			CPoint point,
			DWORD dwKeyState
			)
		{
			ASSERT( !_dragInfo.IsEmpty() );
			return OnCustomizeTargetOver( _dragInfo, point, dwKeyState );
		}
		virtual DROPEFFECT OnCustomizeTargetOver(
			CCmdDragInfo & _dragInfo,
			CPoint point,
			DWORD dwKeyState
			)
		{
			ASSERT( !_dragInfo.IsEmpty() );
			_dragInfo;
			point;
			dwKeyState;
			return DROPEFFECT_NONE;
		}
		virtual void OnCustomizeTargetLeave()
		{
		}
		virtual bool OnCustomizeTargetDrop(
			CCmdDragInfo & _dragInfo,
			CPoint point,
			DROPEFFECT de
			)
		{
			ASSERT( !_dragInfo.IsEmpty() );
			_dragInfo;
			point;
			de;
			// returns true if drop with any effect
			// has been performed
			return false;
		}
	}; // struct ICustomizeDropTarget

	struct __PROF_UIS_API ICustomizeDropSource
	{
		virtual bool OnCustomizeSourceAllowMoveDel()
		{
			// true - move/delete effects allowed
			// false - only copy effect allowed
			return true;
		}
		virtual void OnCustomizeSourceDragComplete(
			DROPEFFECT de,
			bool bCanceled,
			bool * p_bNoResetActiveItem
			)
		{
			de;
			bCanceled;
			p_bNoResetActiveItem;
		}
	}; // struct ICustomizeDropSource

	struct __PROF_UIS_API ICustomizeCategoryEnumSite
	{
		virtual bool OnCustomizeCategoryEnum(
			int nPos,
			CExtCustomizeCmdTreeNode * pRoot,
			bool bVisibleInKeyboardPage
			) = 0;
	}; // struct ICustomizeCategoryEnumSite

protected:
	class __PROF_UIS_API CCmdDropTarget : public COleDropTarget
	{
		ICustomizeDropTarget * m_pCustomizeDropTarget;
		CExtCustomizeSite * m_pSite;
	public:
		CCmdDropTarget(
			ICustomizeDropTarget * pCustomizeDropTarget,
			CExtCustomizeSite * pSite
			);
		~CCmdDropTarget();
#ifdef _DEBUG
		virtual void AssertValid() const;
#endif // _DEBUG
		virtual DROPEFFECT OnDragEnter(
			CWnd * pWnd,
			COleDataObject * pDataObject,
			DWORD dwKeyState,
			CPoint point
			);
		virtual void OnDragLeave(
			CWnd * pWnd
			);
		virtual DROPEFFECT OnDragOver(
			CWnd * pWnd,
			COleDataObject * pDataObject,
			DWORD dwKeyState,
			CPoint point
			);
		virtual BOOL OnDrop(
			CWnd * pWnd,
			COleDataObject * pDataObject,
			DROPEFFECT dropEffect,
			CPoint point
			);
	}; // class  __PROF_UIS_API CCmdDropTarget

	class __PROF_UIS_API CCmdDropSource : public COleDropSource
	{
		ICustomizeDropSource * m_pCustomizeDropSource;
		CExtCustomizeSite * m_pSite;
		HCURSOR m_hCursorDelete, m_hCursorMove, m_hCursorCopy;
		DROPEFFECT m_deLast;
		bool m_EscapePressed:1;
	public:
		CCmdDropSource(
			ICustomizeDropSource * pCustomizeDropSource,
			CExtCustomizeSite * pSite
			);
		~CCmdDropSource();
#ifdef _DEBUG
		virtual void AssertValid() const;
#endif // _DEBUG
		virtual BOOL OnBeginDrag(
			CWnd * pWnd
			);
		virtual SCODE QueryContinueDrag(
			BOOL bEscapePressed,
			DWORD dwKeyState
			);
		virtual SCODE GiveFeedback(
			DROPEFFECT dropEffect
			);
		bool IsCanceled() const
		{
			return m_EscapePressed;
		}
		DROPEFFECT GetLastEffect() const
		{
			return m_deLast;
		}
	}; // class CCmdDropSource

public:
	bool m_bCustomizeHookAllowed:1;
	bool m_bSeparatedPopupHookAllowed:1;

	enum e_category_role_t
	{
		__E_CATEGORY_ROLE_ORDINARY		= 0,
		__E_CATEGORY_ROLE_ALL_COMMANDS	= 1,
		__E_CATEGORY_ROLE_NEW_MENU		= 2,
	};

protected:
	class __PROF_UIS_API CCategoryCustomizeInfo
	{
		CExtCustomizeSite * m_pSite;
	public:
		e_category_role_t m_eRole;
		CExtCustomizeCmdTreeNode * m_pRoot;
		bool m_bVisibleInKeyboardPage:1;
		CCategoryCustomizeInfo(
			__EXT_MFC_SAFE_LPCTSTR strName,
			CExtCustomizeSite * pSite
			);
		~CCategoryCustomizeInfo();
		int MakeCmdsUnique();
		void ReloadLocalizedRoleParms();
	}; // class CCategoryCustomizeInfo

	DWORD m_dwCustomizeFlags;
	bool m_bInitComplete:1, m_bCustomizeMode:1;
	CWnd * m_pWndTop;
	HWND m_hWndCustomizeForm;
	CExtCustomizeCmdTreeNode
		* m_pNodeToolbarsI, * m_pNodeToolbarsC;
	CPtrArray m_arrMenuInfo;
	CCmdMenuInfo * m_pMenuInfoActiveCustomize;
	CExtMenuControlBar * m_pWndMenuBar;
	CMapPtrToPtr m_mapToolBars;
	CPtrList m_listHelperBarsForListBox;
	CPtrList m_listCategories;
	CLIPFORMAT m_cf;
	ICustomizeDropSource * m_pHelperCustomizeDropSource;
	CExtCustomizeCmdTreeNode * m_pHelperDragSourceNode;
	CMapPtrToPtr m_mapCmdTagets;
	CExtPopupMenuWnd * m_pPopupDropTargetTop, * m_pPopupDropTargetTopSaved;
	CExtCustomizeCmdTreeNode * m_pPopupDropTargetNode, * m_pPopupDropTargetNodeSaved;
	CExtToolControlBar * m_pBarDefCmdTarget;
	CMapWordToPtr m_mapHelperUserBars;
	CPtrList m_listHelperUserBarsOrder;
	CExtCustomizeCmdTreeNode * m_pNodeCustomized;
	LPVOID m_pActiveItem;
	HWND m_hWndActiveItemInvalidate;
	HWND m_hWndMdiClient;
	HWND m_hWndHelperPopupHook;
	typedef
		CMap < HWND, HWND, CExtCustomizeSite *, CExtCustomizeSite * >
		mapHelperPopupHooks_t;
	static mapHelperPopupHooks_t g_mapHelperPopupHooks;
	CExtBitmap m_bmpIconPalette;
	void _EmptyContainers();
	int _CategoryUpdateFromMenuImpl(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName,
		CMenu * pMenu
		);
	CCategoryCustomizeInfo * _CategoryGetInfoImpl(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName,
		bool bAllowRegisterNew = true
		);
	static CLIPFORMAT _RegisterClipFormat( HWND hWndTop );
	void _CategoryReloadLocalizedRoleParmsAllImpl();
	bool _CategorySetRoleImpl(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName,
		e_category_role_t eRole
		)
	{
		CCategoryCustomizeInfo * pCategoryInfo =
			_CategoryGetInfoImpl(
				strCategoryName,
				false
				);
		if( pCategoryInfo == NULL )
			return false;
		pCategoryInfo->m_eRole = eRole;
		pCategoryInfo->ReloadLocalizedRoleParms();
		return true;
	}
	int _CategoryUpdateImpl(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName,
		LPUINT pCmdList,
		int nCount = 1,
		bool bNoCheckCmdInsertAllowed = false
		);
	CExtCustomizeCmdTreeNode * m_pHelperResizingNode;
public:
	DECLARE_CExtPmBridge_MEMBERS( CExtCustomizeSite );

	CExtCustomizeSite();
	virtual ~CExtCustomizeSite();

	int MenuInfoGetCount();
	bool MenuInfoAddEmpty(
		CWnd * pWndTop,
		__EXT_MFC_SAFE_LPCTSTR strName = _T("Default"),
		bool bDefaultMenu = true,
		CRuntimeClass * pRtcFrame = NULL,
		CRuntimeClass * pRtcView = NULL,
		CRuntimeClass * pRtcDoc = NULL
		);
	bool MenuInfoAdd(
		CWnd * pWndTop,
		__EXT_MFC_SAFE_LPCTSTR strName,
		CMenu * pMenu,
		bool bDefaultMenu,
		bool bPopupMenu = false,
		CRuntimeClass * pRtcFrame = NULL,
		CRuntimeClass * pRtcView = NULL,
		CRuntimeClass * pRtcDoc = NULL
		);
	bool MenuInfoAdd(
		CWnd * pWndTop,
		__EXT_MFC_SAFE_LPCTSTR strName,
		__EXT_MFC_SAFE_LPCTSTR strResourceID,
		bool bDefaultMenu,
		bool bPopupMenu = false,
		CRuntimeClass * pRtcFrame = NULL,
		CRuntimeClass * pRtcView = NULL,
		CRuntimeClass * pRtcDoc = NULL
		);
	bool MenuInfoAdd(
		CWnd * pWndTop,
		__EXT_MFC_SAFE_LPCTSTR strName,
		UINT nResourceID,
		bool bDefaultMenu,
		bool bPopupMenu = false,
		CRuntimeClass * pRtcFrame = NULL,
		CRuntimeClass * pRtcView = NULL,
		CRuntimeClass * pRtcDoc = NULL
		);
	int MenuInfoRemove( // returns removed count
		int nPos = 0,
		int nCount = -1 // -1 - remove all
		);
	CCmdMenuInfo * MenuInfoGetAt(
		int nPos = 0
		);
	virtual CCmdMenuInfo * MenuInfoGetDefault();
	virtual CCmdMenuInfo * MenuInfoGetByName(
		__EXT_MFC_SAFE_LPCTSTR strName
		);
	virtual CCmdMenuInfo * MenuInfoFindForMenuBar();
	virtual CCmdMenuInfo * MenuInfoActiveGet();
	virtual CCmdMenuInfo * MenuInfoActiveSet( CCmdMenuInfo * pCmdMenuInfo );
	int MenuInfoFindIndex( CCmdMenuInfo * pCmdMenuInfoSearch );
	
	bool MenuInfoSetAccelTable(
		__EXT_MFC_SAFE_LPCTSTR strName,
		HACCEL hAccelTable,
		bool bUseTableCopy
		);
	bool MenuInfoLoadAccelTable(
		__EXT_MFC_SAFE_LPCTSTR strName,
		__EXT_MFC_SAFE_LPCTSTR strResourceID
		);
	bool MenuInfoLoadAccelTable(
		__EXT_MFC_SAFE_LPCTSTR strName,
		UINT nResourceID
		);

	virtual void CategoryAppendAllCommands();
	virtual void CategoryAppendNewMenu();
	virtual int CategoryGetCount();
	virtual CExtCustomizeCmdTreeNode * CategoryGetTreeAt( int nPos );
	virtual CExtCustomizeCmdTreeNode * CategoryGetTreeByName( __EXT_MFC_SAFE_LPCTSTR strCategoryName );
	virtual int CategoryEnum(
		ICustomizeCategoryEnumSite * pCategoryEnumSite
		);
	virtual int CategoryMakeCmdsUnique(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName
		);
	virtual int CategoryMakeAllCmdsUnique();
	virtual int CategoryMakeFromTree(
		CExtCustomizeCmdTreeNode * pNodeCategoryRoot,
		__EXT_MFC_SAFE_LPCTSTR strCategoryName = NULL
		);
	int CategoryMakeFromTreeN(
		CExtCustomizeCmdTreeNode * pNodeCategoriesRoot
		);
	virtual int CategoryUpdate(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName,
		LPUINT pCmdList,
		int nCount = 1
		);
	virtual int CategoryUpdate(
		CMenu * pMenu,
		bool bPopupMenu = false
		);
	int CategoryUpdate(
		UINT nMenuResourceID,
		bool bPopupMenu = false
		);
	int CategoryUpdate(
		__EXT_MFC_SAFE_LPCTSTR strMenuResourceID,
		bool bPopupMenu = false
		);
	virtual bool CategoryRemove(
		__EXT_MFC_SAFE_LPCTSTR strCategoryName
		);
	virtual void CategoryRemoveAll();

	virtual CWnd * GetCustomizeAccelUpdatingTargetWnd();
	virtual CWnd * GetCustomizeTopWnd();
	DWORD GetCustomizeFlags() const;
	virtual bool IsCustomizationInabled() const;
	virtual bool EnableCustomization(
		CWnd * pWndTop,
		DWORD dwCustomizeFlags = __ECSF_DEFAULT
		);
	virtual bool IsCustomizeMode() const;
	virtual HWND GetSafeCustomizeFormHWND();

	virtual CExtCustomizeCmdTreeNode *
		FindDynamicPopupInitialState(
			UINT nCmdID
			);

	virtual LPVOID ActiveItemSet(
		LPVOID pActiveItem = NULL,
		HWND hWndActiveItemInvalidate = NULL
		);
	virtual LPVOID ActiveItemGet(
		HWND * p_hWnd = NULL
		);
	bool IsActiveItem(
		LPVOID pItem
		);

	virtual CExtToolControlBar * BarDefCmdTargetGet();
	virtual CExtToolControlBar * BarDefCmdTargetSet(
		CExtToolControlBar * pBar
		);
	virtual bool BarIsRegistered(
		CExtToolControlBar * pBar
		);
	virtual bool BarReset(
		CExtToolControlBar * pBar,
		bool bRecalcLayout = true
		);
	virtual bool BarResetAsk(
		CExtToolControlBar * pBar
		);
	virtual bool IsUserBarCommand(
		UINT nCmdID
		);
	virtual CExtToolControlBar * GetUserBar(
		UINT nBarID
		);

	virtual CExtCustomizeCmdTreeNode * CustomizedNodeGet();
	virtual bool CustomizedNodeSet(
		CExtCustomizeCmdTreeNode * pNode = NULL
		);
	
	virtual CExtCustomizeCmdTreeNode * DraggedNodeGet();
	virtual CExtCustomizeCmdTreeNode * DraggedNodeSet(
		CExtCustomizeCmdTreeNode * pNode = NULL
		);
	virtual bool DoDragCmdNode(
		ICustomizeDropSource * pCustomizeDragSource,
		CExtCustomizeCmdTreeNode * pNodeI,
		CExtCustomizeCmdTreeNode * pNodeC,
		RECT & rcItem
		);
	virtual bool DoFormCustomization(
		bool bDelayShow = false
		);
	virtual void CancelCustomization();
	virtual bool UpdateCustomizationFormData();
	static CExtCustomizeSite * GetCustomizeSite(
		HWND hWndStartSearch
		);
	CPtrList & GetBarsForListBox();

	virtual void DropTargetPopupTrack(
		CExtCustomizeCmdTreeNode * pPopupDropTargetNode,
		const CRect & rcExcludeArea,
		CPoint point,
		DWORD dwTrackFlags
		);
	virtual void DropTargetPopupCancelEx( int nPushPop );
public:
	virtual void DropTargetPopupCancel();
	virtual CExtCustomizeCmdTreeNode * GetPopupDropTargetNode();
	virtual CExtPopupMenuWnd * GetPopupDropTargetMenu();

	virtual void RegisterCommandDropTarget(
		ICustomizeDropTarget * pCustomizeDropTarget,
		CWnd * pWnd
		);
	virtual void UnRegisterCommandDropTarget(
		ICustomizeDropTarget * pCustomizeDropTarget
		);
	virtual void UnRegisterAllCommandDropTargets();

	virtual bool GetToolbarCustomizeInfo(
		CExtToolControlBar * pBar,
		CExtCustomizeCmdTreeNode ** ppNodeC, // may be NULL
		CExtCustomizeCmdTreeNode ** ppNodeI = NULL,
		CExtSafeString * p_strDisplayName = NULL,
		bool * p_bUserBar = NULL
		);
	CExtCustomizeCmdTreeNode * GetToolbarCmdNode(
		CExtToolControlBar * pBar,
		bool bInitialNode = false
		);
	
	virtual void BuildToolbarLists(
		CPtrList * pListBarsBasic,
		CPtrList * pListBarsUser = NULL
		);
	virtual void DeleteAllUserBars();

	bool CustomizeStateLoad(
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%
		HKEY hKeyRoot = HKEY_CURRENT_USER,
		bool bEnableThrowExceptions = false
		);
	bool CustomizeStateSave(
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%\%sSectionNameProfile%
		HKEY hKeyRoot = HKEY_CURRENT_USER,
		bool bEnableThrowExceptions = false
		);
	virtual bool CustomizeStateSerialize(
		CArchive & ar,
		bool bEnableThrowExceptions = false
		);

	virtual INT RedrawCommandItems(
		UINT nCmdID,
		bool bUpdateWindows = true
		) const;
	virtual INT UpdateAllCommandItems() const;

protected:
	virtual void OnRegisterToolBar(
		CExtToolControlBar * pToolBar
		);
	virtual void OnRegisterAdditionalToolBars();
	virtual bool OnHookWndMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		UINT nMessage,
		WPARAM & wParam,
		LPARAM & lParam
		);
	virtual void OnCustomizeModeEnter();
	virtual void OnCustomizeModeLeave();
	virtual HWND OnCreateCustomizeForm();
public:
	virtual void OnBarStateChanged(
		CExtControlBar * pExtBar
		);
	ICustomizeDropSource * GetCustomizeDropSource()
	{
		ASSERT( this != NULL );
		return m_pHelperCustomizeDropSource;
	}

	virtual CExtBarButton *  OnCreateToolbarButton(
		CExtToolControlBar * pBar,
		CExtCustomizeCmdTreeNode * pNodeI,
		CExtCustomizeCmdTreeNode * pNodeC
		);
	virtual void OnInsertToolbarCmdNode(
		CCmdDragInfo & _dragInfo,
		CExtToolControlBar * pBar,
		INT nInsertPos,
		bool bRecalcLayout
		);
	virtual CExtCustomizeCmdTreeNode * OnCmdNodeCreate(
		UINT nCmdIdBasic = 0L,
		UINT nCmdIdEffective = 0L,
		CExtCustomizeCmdTreeNode * pParentNode = NULL,
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
	virtual void OnCmdNodeInserted(
		CExtCustomizeCmdTreeNode * pNode
		);

	virtual CExtToolControlBar * OnUserBarCreate(
		__EXT_MFC_SAFE_LPCTSTR strName,
		UINT nBarID = 0, // 0-alloc any, other-alloc specified
		bool bCreateInvisible = false,
		CExtToolControlBar * pBarNew = NULL
		);
	virtual void OnUserBarRename(
		CExtToolControlBar * pBar,
		__EXT_MFC_SAFE_LPCTSTR strName
		);
	virtual void OnUserBarDelete(
		CExtToolControlBar * pBar,
		bool bBarWindowIsSelfDestroyed = false
		);
	
	virtual void OnGetCustomizedNodeIconPaletteInfo(
		const CExtCustomizeCmdTreeNode * pNodeI,
		const CExtCustomizeCmdTreeNode * pNodeC,
		const CExtCmdItem * pCmdItem,
		const CObject * pObjEventSrc,
		UINT & nIdIconPaletteFirst,
		CExtBitmap & _bmp,
		SIZE & sizePaletteDimension,
		SIZE & sizeBitmapPart,
		COLORREF & clrTransparent
		);
	virtual bool OnCustomizeTreeNode(
		CExtCustomizeCmdTreeNode * pNodeI,
		CExtCustomizeCmdTreeNode * pNodeC,
		CExtCmdItem * pCmdItem,
		bool & bDelete,
		bool bCanAddRemoveSeparator,
		CObject * pObjEventSrc
		);

	virtual bool OnCanInsertCommandToAnyCategory(
		CExtCmdItem * pCmdItem
		);
	virtual bool OnCanInsertCommandToAllCommandsCategory(
		CExtCmdItem * pCmdItem
		);

	virtual bool OnCanDragCustomizeCommand(
		CExtCmdItem * pCmdItem
		);

	virtual CExtSafeString OnFormatCmdAccelText(
		const CCmdKeyInfo & _cmdKeyInfo
		);

	virtual void OnUpdateAccelGlobalInfo(
		bool bDelay = false
		);
	virtual int OnChooseCmdKeyInfo(
		CExtCmdItem * pCmdItem,
		const CCmdKeySet & _cmdKeySet
		);
	
	virtual void OnChangedToolbarIconSizeOption();

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
	virtual void OnGetCmdItemMinMaxSizeH(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		int * p_nWidthMin = NULL,
		int * p_nWidthMax = NULL
		);
	virtual bool OnCmdNodeResizeH(
		int nInitialResizingStateH,
		HWND hWndBase,
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		int nWidthMin,
		int nWidthMax,
		CRect & rcItem
		);
	virtual CExtCustomizeCmdTreeNode * GetResizingNode();

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

	friend class CCmdDropTarget;
}; // class CExtCustomizeSite

/////////////////////////////////////////////////////////////////////////////
// CExtCustomizeToolbarsListBox window

class __PROF_UIS_API CExtCustomizeToolbarsListBox : public CExtCheckListWnd
{
	DECLARE_DYNCREATE(CExtCustomizeToolbarsListBox);

// Construction
public:
	CExtCustomizeToolbarsListBox();
	virtual ~CExtCustomizeToolbarsListBox();

// Attributes
public:

// Operations
public:
	void SyncBars();
	void DelayRescanBars();
	virtual int AddBar(
		CExtControlBar * pExtBar,
		__EXT_MFC_SAFE_LPCTSTR strBarName = NULL // detect automatically
		);
	virtual void UpdateCheckFromBar( int nPos );
	virtual void UpdateBarFromCheck( int nPos );
	virtual int FindBarPos(
		CExtControlBar * pExtBarSearch
		);

	virtual CRect OnQueryItemMargins( INT nItem ) const;
	virtual CRect OnQueryItemCheckMargins( INT nItem ) const;	

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtCustomizeToolbarsListBox)
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
    //{{AFX_MSG(CExtCustomizeToolbarsListBox)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnDelayedRescanBars(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

}; // class CExtCustomizeToolbarsListBox

/////////////////////////////////////////////////////////////////////////////
// CExtCustomizeCommandListBox window

class __PROF_UIS_API CExtCustomizeCommandListBox
	: public CListBox
	, public CExtCustomizeSite::ICustomizeDropSource
	, public CExtPmBridge
{
// Construction
public:
	DECLARE_DYNCREATE( CExtCustomizeCommandListBox );
	DECLARE_CExtPmBridge_MEMBERS( CExtCustomizeCommandListBox );

	CExtCustomizeCommandListBox();
	virtual ~CExtCustomizeCommandListBox();

// Attributes
public:
	bool m_bAllowDragStart:1, m_bShowSelAlways:1,
		m_bRibbonQatbMode:1, m_bRibbonQatbTargetListBox:1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtCustomizeCommandListBox)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT pMIS);
	virtual void DrawItem(LPDRAWITEMSTRUCT pDIS);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	INT AddCommand(
		CExtCustomizeCmdTreeNode * pNode
		);
	INT InsertCommand(
		CExtCustomizeCmdTreeNode * pNode,
		INT nDstIndex = -1
		);
	void ResetContent();

	CExtSafeString m_strProfileName;
	CSize m_sizeLbIcon;
	CTypedPtrArray < CPtrArray, CExtCustomizeCmdTreeNode * > m_arrCmds;
protected:
	int m_nIconAreaWidth;
	bool m_bTrackingDragStart:1;
	CPoint m_ptTrackingDragStart;

	// CExtCustomizeSite::ICustomizeDropSource
	virtual bool OnCustomizeSourceAllowMoveDel();

protected:
    //{{AFX_MSG(CExtCustomizeCommandListBox)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

}; // class CExtCustomizeCommandListBox


/////////////////////////////////////////////////////////////////////////////
// CExtCustomizeForm window


class __PROF_UIS_API CExtCustomizeForm : public CExtResPS
{
	DECLARE_DYNCREATE(CExtCustomizeForm);

// Construction
public:
	CExtCustomizeForm();
	virtual ~CExtCustomizeForm();

// Attributes
public:
	bool m_bAutoDestroyCustomizeForm:1;

protected:
	CPtrList m_listHelperPages;

// Operations
public:
	virtual void AddPage( CPropertyPage * pPage );
	virtual void OnBarStateChanged(
		CExtControlBar * pExtBar
		);

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtCustomizeForm)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual bool OnQueryCloseForm();

protected:
    //{{AFX_MSG(CExtCustomizeForm)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT _OnSyncContent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

}; // class CExtCustomizeForm

/////////////////////////////////////////////////////////////////////////////
// CExtCustomizePageToolbars window

class __PROF_UIS_API CExtCustomizePageToolbars : public CExtResPP
{
	DECLARE_DYNCREATE(CExtCustomizePageToolbars);

// Construction
public:
	CExtCustomizePageToolbars();
	virtual ~CExtCustomizePageToolbars();

// Attributes
public:

// Dialog Data
	//{{AFX_DATA(CExtCustomizePageToolbars)
	CButton	m_btnTbReset;
	CButton	m_btnTbRename;
	CButton	m_btnTbDelete;
	CButton	m_btnTbCreate;
	CComboBox	m_wndComboMenu;
	CExtCustomizeToolbarsListBox	m_wndToolbarsList;
	//}}AFX_DATA

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtCustomizePageToolbars)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
	CExtSafeString m_strSelBarName;
	bool m_bUserBar;
	CExtToolControlBar * m_pToolBar;

    //{{AFX_MSG(CExtCustomizePageToolbars)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnSelchangeToolbarList();
	afx_msg void OnSelendokComboShowMenu();
	afx_msg void OnClickedToolbarCreate();
	afx_msg void OnClickedToolbarRename();
	afx_msg void OnClickedToolbarDelete();
	afx_msg void OnClickedToolbarReset();
	afx_msg LRESULT _OnSyncContent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

}; // class CExtCustomizePageToolbars

/////////////////////////////////////////////////////////////////////////////
// CExtCustomizePageCommands window

class __PROF_UIS_API CExtCustomizePageCommands
	: public CExtResPP
	, public CExtCustomizeSite::ICustomizeCategoryEnumSite
{
	DECLARE_DYNCREATE(CExtCustomizePageCommands);

// Construction
public:
	CExtCustomizePageCommands();
	virtual ~CExtCustomizePageCommands();

// Attributes
public:

// Dialog Data
	//{{AFX_DATA(CExtCustomizePageCommands)
	CListBox m_wndCategoryList;
	CExtCustomizeCommandListBox m_wndCommandList;
	CStatic m_wndStaticCmdDescription;
	//}}AFX_DATA

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtCustomizePageCommands)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
	// CExtCustomizeSite::ICustomizeCategoryEnumSite::OnCustomizeCategoryEnum
	virtual bool OnCustomizeCategoryEnum(
		int nPos,
		CExtCustomizeCmdTreeNode * pRoot,
		bool bVisibleInKeyboardPage
		);
	CExtCustomizeCmdTreeNode * m_pItems;
public:
	CExtSafeString m_strProfileName;

protected:
    //{{AFX_MSG(CExtCustomizePageCommands)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeCategoryList();
	afx_msg void OnSelChangeCommandList();
	//}}AFX_MSG
	afx_msg LRESULT _OnSyncContent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

}; // class CExtCustomizePageCommands

/////////////////////////////////////////////////////////////////////////////
// CExtCustomizePageKeyboard window

class __PROF_UIS_API CExtCustomizePageKeyboard
	: public CExtResPP
	, public CExtCustomizeSite::ICustomizeCategoryEnumSite
{
	DECLARE_DYNCREATE(CExtCustomizePageKeyboard);

// Construction
public:
	CExtCustomizePageKeyboard();
	virtual ~CExtCustomizePageKeyboard();

// Attributes
public:
	class __PROF_UIS_API CInternalHotKeyWnd : public CExtEditHotKeyBase
	{
	public:
		virtual void OnAccelChanged();
	}; // class CInternalHotKeyWnd

// Dialog Data
	//{{AFX_DATA(CExtCustomizePageKeyboard)
	CStatic	m_wndStaticCmdUsed;
	CStatic	m_wndStaticCmdDescription;
	CStatic	m_wndStaticAccelGroup;
	CExtCustomizeCommandListBox	m_wndCommandList;
	CListBox	m_wndListAccelKeys;
	CInternalHotKeyWnd m_wndHotKey;
	CComboBox	m_wndComboAccelGroup;
	CComboBox	m_wndComboAccelCategory;
	CButton	m_wndBtnResetAll;
	CButton	m_wndBtnRemove;
	CButton	m_wndBtnAssign;
	//}}AFX_DATA

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtCustomizePageKeyboard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
	// CExtCustomizeSite::ICustomizeCategoryEnumSite::OnCustomizeCategoryEnum
	virtual bool OnCustomizeCategoryEnum(
		int nPos,
		CExtCustomizeCmdTreeNode * pRoot,
		bool bVisibleInKeyboardPage
		);
	CExtCustomizeCmdTreeNode * m_pItems;
public:
	CExtSafeString m_strProfileName;
protected:
	CExtCustomizeSite::CCmdMenuInfo * m_pMenuInfo;
	UINT m_nCmdID;
	CExtCustomizeSite::CCmdKeySet m_cmdKeySet;
	CExtCustomizeSite::CCmdKeyInfo m_lastCmdKeyInfo;
	
	void _UpdateCmdButtons();

protected:
    //{{AFX_MSG(CExtCustomizePageKeyboard)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendOkAccelCategoryCombo();
	afx_msg void OnSelendOkAccelGroupCombo();
	afx_msg void OnSelchangeAccelKeysList();
	afx_msg void OnSelChangeCommandList();
	afx_msg void OnAccelAssign();
	afx_msg void OnAccelRemove();
	afx_msg void OnAccelResetAll();
	//}}AFX_MSG
	afx_msg LRESULT _OnSyncContent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	friend class CInternalHotKeyWnd;
}; // class CExtCustomizePageKeyboard

/////////////////////////////////////////////////////////////////////////////
// CExtCustomizePageParms window

class __PROF_UIS_API CExtCustomizePageParms : public CExtResPP
{
	DECLARE_DYNCREATE(CExtCustomizePageParms);

// Construction
public:
	CExtCustomizePageParms();
	virtual ~CExtCustomizePageParms();

private:
	bool m_bPageInitComplete:1;

// Attributes
public:

// Dialog Data
	//{{AFX_DATA(CExtCustomizePageParms)
	CButton m_wndCheckMenuExpandingAnimation;
	CButton m_wndCheckDisplayMenuShadows;
	CButton m_wndCheckHighlightRarelyMenuItems;
	CButton	m_wndCheckShortcutsInTT;
	CButton	m_wndCheckTipsInToolbars;
	CButton	m_wndCheckTipsInMenus;
	CButton	m_wndCheckLargeIconsInMenu;
	CButton	m_wndCheckLargeIconsInToolbar;
	CButton	m_wndCheckMenuDelayExpanding;
	CButton	m_wndCheckFullMenus;
	CComboBox	m_wndComboMenuAnimationType;
	//}}AFX_DATA

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtCustomizePageParms)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void _UpdateParmsItems();

protected:
    //{{AFX_MSG(CExtCustomizePageParms)
	virtual BOOL OnInitDialog();
	afx_msg void OnResetCmdStats();
	afx_msg void OnCheckFullMenus();
	afx_msg void OnCheckMenuDelayExpanding();
	afx_msg void OnCheckHighlightRarelyMenuItems();
	afx_msg void OnCheckMenuExpandingAnimation();
	afx_msg void OnCheckDisplayMenuShadows();
	afx_msg void OnCheckLargeIconsInMenu();
	afx_msg void OnCheckLargeIconsInToolbar();
	afx_msg void OnCheckTipsInMenus();
	afx_msg void OnCheckTipsInToolbars();
	afx_msg void OnCheckToolbarTipsWithShortcuts();
	afx_msg void OnSelendOkComboMenuAnimationType();
	//}}AFX_MSG
	afx_msg LRESULT _OnSyncContent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

}; // class CExtCustomizePageParms

#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

#endif // __EXTCUSTOMIZE_H

