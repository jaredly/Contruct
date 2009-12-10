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

#if (!defined __EXT_POPUP_CTRL_MENU_H)
#define __EXT_POPUP_CTRL_MENU_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

#if (!defined __EXT_TOOLCONTROLBAR_H)
	#include <ExtToolControlBar.h>
#endif

#if (!defined __EXT_MFC_NO_DATE_PICKER_POPUP)
	#if (!defined __EXT_DATE_PICKER_H)
		#include <ExtDatePicker.h>
	#endif // (!defined __EXT_DATE_PICKER_H)
#endif

#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CExtPopupControlMenuWnd

class __PROF_UIS_API CExtPopupControlMenuWnd
	: public CExtPopupMenuWnd
{
public:
	DECLARE_DYNCREATE( CExtPopupControlMenuWnd );

	CExtPopupControlMenuWnd();
	virtual ~CExtPopupControlMenuWnd();

// Attributes
public:
	virtual void _ForceCancelCallbacs();

	CSize m_sizeChildControl; // child control size
	CSize m_sizeChildControlMin; // for ribbon gallery resizable mode
	CSize m_sizeChildControlMax; // for ribbon gallery resizable mode
	static UINT g_nMsgControlInputRetranslate;
	static bool g_bControlMenuWithShadows;
	bool m_bStdEscProcessing:1;

	virtual void _RecalcMinMaxResizingSizes();

// Implementation
protected:
	CRect m_rcChildControl;
	HWND m_hWndChildControl;

	virtual bool _IsPopupWithShadows() const;
	virtual bool _IsPopupWithShadowsDynamic() const;
public:
	virtual CRect _CalcTrackRect();
	virtual CSize _CalcTrackSize();
protected:
	virtual bool _CreateHelper(
		CWnd * pWndCmdReceiver
		);

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	// CExtCustomizeSite::ICustomizeDropTarget
	virtual DROPEFFECT OnCustomizeTargetOver(
		CExtCustomizeSite::CCmdDragInfo & _dragInfo,
		CPoint point,
		DWORD dwKeyState
		);
	virtual void OnCustomizeTargetLeave();
	virtual bool OnCustomizeTargetDrop(
		CExtCustomizeSite::CCmdDragInfo & _dragInfo,
		CPoint point,
		DROPEFFECT de
		);
	// CExtCustomizeSite::ICustomizeDropSource
	virtual void OnCustomizeSourceDragComplete(
		DROPEFFECT de,
		bool bCanceled,
		bool * p_bNoResetActiveItem
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

public:
	HWND GetChildCtrlHWND()
	{
		ASSERT_VALID( this );
		return m_hWndChildControl;
	}
	virtual bool IsAllItemsRarelyUsed() const;

	struct __PROF_UIS_API CONTROLINPUTRETRANSLATEINFO
	{
		enum eRetranslateType
		{
			__ERTT_MOUSE_MOVE = 0,
			__ERTT_MOUSE_CLICK = 1,
			__ERTT_MOUSE_WHEEL = 2,
			__ERTT_KEYBOARD = 3,
		};
		eRetranslateType m_eRTT;
		UINT m_nFlags; // mouse and keyboard
		CPoint m_point; // mouse
		bool & m_bNoEat; // mouse and keyboard - returned value
		UINT m_nChar; // keyboard
		UINT m_nRepCnt; // keyboard
		WPARAM m_wParam; // wheel
		LPARAM m_lParam; // wheel
		CExtPopupControlMenuWnd * m_pControlPopup;
		CONTROLINPUTRETRANSLATEINFO(
			CExtPopupControlMenuWnd * pControlPopup,
			bool bMouseMove, // true - move, false - click
			UINT nFlags,
			CPoint point,
			bool & bNoEat
			)
			: m_eRTT( bMouseMove ? __ERTT_MOUSE_MOVE : __ERTT_MOUSE_CLICK )
			, m_pControlPopup( pControlPopup )
			, m_nFlags( nFlags )
			, m_point( point )
			, m_bNoEat( bNoEat  )
			, m_nChar( 0 )
			, m_nRepCnt( 0 )
			, m_wParam( 0 )
			, m_lParam( 0 )
		{
			ASSERT_VALID( pControlPopup );
		}
		CONTROLINPUTRETRANSLATEINFO(
			CExtPopupControlMenuWnd * pControlPopup,
			UINT nChar,
			UINT nRepCnt,
			UINT nFlags,
			bool & bNoEat
			)
			: m_eRTT( __ERTT_KEYBOARD )
			, m_pControlPopup( pControlPopup )
			, m_nFlags( nFlags )
			, m_point( 0, 0 )
			, m_bNoEat( bNoEat )
			, m_nChar( nChar )
			, m_nRepCnt( nRepCnt )
			, m_wParam( 0 )
			, m_lParam( 0 )
		{
			ASSERT_VALID( pControlPopup );
		}
		CONTROLINPUTRETRANSLATEINFO(
			CExtPopupControlMenuWnd * pControlPopup,
			WPARAM wParam,
			LPARAM lParam,
			bool & bNoEat
			)
			: m_eRTT( __ERTT_MOUSE_WHEEL )
			, m_pControlPopup( pControlPopup )
			, m_nFlags( 0 )
			, m_point( 0, 0 )
			, m_bNoEat( bNoEat )
			, m_nChar( 0 )
			, m_nRepCnt( 0 )
			, m_wParam( wParam )
			, m_lParam( lParam )
		{
			ASSERT_VALID( pControlPopup );
		}
	}; // struct CONTROLINPUTRETRANSLATEINFO
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

	virtual bool _CanStartLevelTracking();

	virtual void _AdjustAnimation( e_animation_type_t & eAT );

	virtual void _DoAdjustControlMetrics(
		CSize _sizeAdjust
		);
	virtual void _RecalcLayoutImpl();
	virtual CRect _RecalcControlRect();

	virtual HWND OnCreateChildControl(
		const RECT & rcChildControl
		);
	virtual CSize OnAdjustChildControlSize();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtPopupControlMenuWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtPopupControlMenuWnd

/////////////////////////////////////////////////////////////////////////////
// CExtPopupInplaceListBox

class __PROF_UIS_API CExtPopupInplaceListBox
	: public CListBox
{
public:
	DECLARE_DYNCREATE( CExtPopupInplaceListBox );

	CExtPopupInplaceListBox();
	virtual ~CExtPopupInplaceListBox();

// Attributes
public:
	class __PROF_UIS_API POPUPLISTBOXITEM
	{
	public:
		POPUPLISTBOXITEM();
		~POPUPLISTBOXITEM();

		bool IsEnabled() const;
		void Enable(
			bool bEnable = true
			);

		void BackColorSet( 
			COLORREF clrBack 
			);
		COLORREF BackColorGet() const;

		void TextColorSet( 
			COLORREF clrText 
			);
		COLORREF TextColorGet() const;

	protected:
		bool m_bEnabled:1;
		COLORREF m_clrText;
		COLORREF m_clrBack;

	}; // class POPUPLISTBOXITEM

	typedef CTypedPtrArray < CPtrArray, POPUPLISTBOXITEM * > PopupListBoxItemsArr_t;
	
	enum e_sel_action_t
	{
		__SAT_SELCHANGE = 0,
		__SAT_SELENDOK = 1,
		__SAT_SELENDCANCEL = 2,
		__SAT_CLOSE = 3,
	};

	typedef
		bool (*pCbListBoxSelection)(
			CExtPopupInplaceListBox & wndListBox,
			LPVOID pListBoxCookie,
			int eSAT // e_sel_action_t
			);

	pCbListBoxSelection m_pCbListBoxSelection;
	LPVOID m_pSelectionCookie;

	typedef
		bool (*pCbListBoxItemClick)(
			CExtPopupInplaceListBox & wndListBox,
			LPVOID pListBoxCookie
			);

	pCbListBoxItemClick m_pCbListBoxItemClick;
	LPVOID m_pItemClickCookie;

	typedef
		bool (*pCbInitListBoxContent)(
			CExtPopupInplaceListBox & wndListBox,
			LPVOID pListBoxCookie
			);
	
	pCbInitListBoxContent m_pCbInitListBoxContent;
	LPVOID m_pInitListBoxCookie;

	typedef
		bool (*pCbListBoxItemDraw)(
			CExtPopupInplaceListBox & wndListBox,
			LPVOID pListBoxCookie,
			LPDRAWITEMSTRUCT pDIS
			);
	typedef
		bool (*pCbListBoxItemMeasure)(
			CExtPopupInplaceListBox & wndListBox,
			LPVOID pListBoxCookie,
			LPMEASUREITEMSTRUCT pMIS
			);
	pCbListBoxItemDraw m_pCbListBoxItemDraw;
	pCbListBoxItemMeasure m_pCbListBoxItemMeasure;
	LPVOID m_pListBoxItemCookie;

	LPARAM m_lParamCookie;
	static UINT g_nMsgPopupListBoxInitContent;
	static UINT g_nMsgPopupListBoxItemMeasure;
	static UINT g_nMsgPopupListBoxItemDraw;
	static UINT g_nMsgPopupListBoxSelEndOK;
	static UINT g_nMsgPopupListBoxSelEndCancel;
	static UINT g_nMsgPopupListBoxSelChange;
	static UINT g_nMsgPopupListBoxItemClick;

	struct __PROF_UIS_API POPUPLISTBOXITEMMEASUREINFO
	{
		CExtPopupInplaceListBox * m_pListBox;
		LPMEASUREITEMSTRUCT m_pMIS;
		POPUPLISTBOXITEMMEASUREINFO(
			CExtPopupInplaceListBox * pListBox,
			LPMEASUREITEMSTRUCT pMIS
			)
			: m_pListBox( pListBox )
			, m_pMIS( pMIS )
		{
			ASSERT_VALID( m_pListBox );
			ASSERT(
					m_pListBox->GetSafeHwnd() != NULL
				&&	::IsWindow( m_pListBox->GetSafeHwnd() )
				);
			ASSERT( m_pMIS != NULL );
		}
	}; // struct POPUPLISTBOXITEMMEASUREINFO

	struct __PROF_UIS_API POPUPLISTBOXITEMDRAWINFO
	{
		CExtPopupInplaceListBox * m_pListBox;
		LPDRAWITEMSTRUCT m_pDIS;
		POPUPLISTBOXITEMDRAWINFO(
			CExtPopupInplaceListBox * pListBox,
			LPDRAWITEMSTRUCT pDIS
			)
			: m_pListBox( pListBox )
			, m_pDIS( pDIS )
		{
			ASSERT_VALID( m_pListBox );
			ASSERT(
					m_pListBox->GetSafeHwnd() != NULL
				&&	::IsWindow( m_pListBox->GetSafeHwnd() )
				);
			ASSERT( m_pDIS != NULL );
			ASSERT( m_pDIS->hDC != NULL );
		}
	}; // struct POPUPLISTBOXITEMDRAWINFO
	
	struct __PROF_UIS_API POPUPLISTBOXITEMSELENDINFO
	{
		CExtPopupInplaceListBox * m_pListBox;
		INT m_nCurSel;
		POPUPLISTBOXITEMSELENDINFO(
			CExtPopupInplaceListBox * pListBox
			)
			: m_pListBox( pListBox )
		{
			ASSERT_VALID( m_pListBox );
			ASSERT(
					m_pListBox->GetSafeHwnd() != NULL
				&&	::IsWindow( m_pListBox->GetSafeHwnd() )
				);
			m_nCurSel = m_pListBox->GetCurSel();
		}
	}; // struct POPUPLISTBOXITEMSELENDINFO

	struct __PROF_UIS_API POPUPLISTBOXITEMCLICKINFO
	{
		CExtPopupInplaceListBox * m_pListBox;
		INT m_nCurSel;
		POPUPLISTBOXITEMCLICKINFO(
			CExtPopupInplaceListBox * pListBox
			)
			: m_pListBox( pListBox )
		{
			ASSERT_VALID( m_pListBox );
			ASSERT(
					m_pListBox->GetSafeHwnd() != NULL
				&&	::IsWindow( m_pListBox->GetSafeHwnd() )
				);
			m_nCurSel = m_pListBox->GetCurSel();
		}
	}; // struct POPUPLISTBOXITEMCLICKINFO

// Operations
public:
	bool LbItemIsEnabled(
		LONG nIndex
		) const;
	void LbItemEnable(
		LONG nIndex,
		bool bEnable = true
		);
	void LbItemBackColorSet( 
		LONG nIndex,
		COLORREF clrBack = COLORREF(-1L)
		);
	COLORREF LbItemBackColorGet( 
		LONG nIndex
		) const;

	void LbItemTextColorSet( 
		LONG nIndex,
		COLORREF clrText = COLORREF(-1L)
		);
	COLORREF LbItemTextColorGet(
		LONG nIndex
		) const;

protected:
	POPUPLISTBOXITEM * LbItemGet( 
		LONG nIndex 
		) const;
	INT LbItemGetCount() const;
	INT LbItemGetIndexOf( 
		const POPUPLISTBOXITEM * pItem 
		) const;
	INT LbItemInsert( 
		INT nIndex = -1 // append
		);
	bool LbItemRemove(
		LONG nIndex
		);
	void LbItemRemoveAll();

// Implementation
protected:
	PopupListBoxItemsArr_t m_arrPopupListBoxItems;
	bool m_bSelEndNotificationPassed:1;

	bool _IsLbItemEnabled( CPoint point );
	
	virtual void _DoSelEndOK();
	virtual void _DoSelEndCancel(
		bool bFinalDestroyMode = false
		);
	virtual void _DoSelChange();
	virtual bool _DoItemClick();

// Overrides
	//{{AFX_VIRTUAL(CExtPopupInplaceListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT pDIS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT pMIS);
	protected:
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtPopupInplaceListBox)
	afx_msg	void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnLBAddString( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnLBInsertString( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnLBDeleteString( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnLBResetContent( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnLBAddFile( WPARAM wParam, LPARAM lParam );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtPopupInplaceListBox

/////////////////////////////////////////////////////////////////////////////
// CExtPopupListBoxMenuWnd

class __PROF_UIS_API CExtPopupListBoxMenuWnd
	: public CExtPopupControlMenuWnd
{
public:
	DECLARE_DYNCREATE( CExtPopupListBoxMenuWnd );

	CExtPopupListBoxMenuWnd(
		LPARAM lParamListCookie = 0L,
		DWORD dwListBoxStyles = WS_CHILD|WS_VISIBLE|WS_VSCROLL|LBS_NOINTEGRALHEIGHT|LBS_HASSTRINGS|LBS_OWNERDRAWVARIABLE
		);
	virtual void Construct(
		LPARAM lParamListCookie = 0L,
		DWORD dwListBoxStyles = WS_CHILD|WS_VISIBLE|WS_VSCROLL|LBS_NOINTEGRALHEIGHT|LBS_HASSTRINGS|LBS_OWNERDRAWVARIABLE
		);
	virtual ~CExtPopupListBoxMenuWnd();

// Attributes
public:
	bool m_bForceCancelCallbacsAllowed:1;
	virtual void _ForceCancelCallbacs();

	LPARAM m_lParamListCookie;
	DWORD m_dwListBoxStyles;
	CExtPopupInplaceListBox::pCbListBoxSelection
		m_pCbListBoxSelection;
	LPVOID m_pSelectionCookie;
	CExtPopupInplaceListBox::pCbListBoxItemClick
		m_pCbListBoxItemClick;
	LPVOID m_pItemClickCookie;
	CExtPopupInplaceListBox::pCbInitListBoxContent
		m_pCbInitListBoxContent;
	LPVOID m_pInitListBoxCookie;
	CExtPopupInplaceListBox::pCbListBoxItemDraw
		m_pCbListBoxItemDraw;
	CExtPopupInplaceListBox::pCbListBoxItemMeasure
		m_pCbListBoxItemMeasure;
	LPVOID m_pListBoxItemCookie;

	typedef
		CSize (*pCbAdjustListBoxSize)(
			CExtPopupInplaceListBox & wndListBox,
			LPVOID pListBoxCookie
			);
	pCbAdjustListBoxSize m_pCbAdjustListBoxSize;
	LPVOID m_pAdjustListBoxSizeCookie;

	virtual bool _OnKeyDown(
		UINT nChar,
		UINT nRepCnt,
		UINT nFlags,
		bool & bNoEat
		);

// Implementation
protected:
	virtual CSize OnAdjustChildControlSize();
	virtual HWND OnCreateChildControl(
		const RECT & rcChildControl
		);

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtPopupListBoxMenuWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtPopupListBoxMenuWnd

#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)

/////////////////////////////////////////////////////////////////////////////
// CExtBarTextFieldButton

class __PROF_UIS_API CExtBarTextFieldButton : public CExtBarButton
{
public:
	class CInPlaceEditWnd;
	typedef
		bool (*pCbVerifyTextInput)(
			CInPlaceEditWnd & edit,
			LPVOID pCookie,
			__EXT_MFC_SAFE_LPCTSTR sTextOld,
			__EXT_MFC_SAFE_LPCTSTR sTextNew
			);
	typedef
		bool (*pCbInplaceEditWndProc)(
			LRESULT & lResult,
			UINT message,
			WPARAM wParam,
			LPARAM lParam,
			CInPlaceEditWnd & edit,
			LPVOID pCookie
			);
	static void _CancelInplaceEditor();
protected:
	CExtSafeString m_sTextField;
	CExtSafeString m_sTextInplaceBuffer;
	INT m_nTextFieldWidth;
	bool m_bComboField:1;
	bool m_bComboPopupDropped:1;
	bool m_bHelperFindListInitialItem:1;
	static bool stat_CbVerifyTextInput(
		CInPlaceEditWnd & edit,
		CExtBarTextFieldButton * pTextFieldTBB,
		__EXT_MFC_SAFE_LPCTSTR sTextOld,
		__EXT_MFC_SAFE_LPCTSTR sTextNew
		);
	static void stat_CbPutTextInputResult(
		CInPlaceEditWnd & edit,
		CExtBarTextFieldButton * pTextFieldTBB,
		__EXT_MFC_SAFE_LPCTSTR sTextNew
		);
	static bool stat_CbInplaceEditWndProc(
		LRESULT & lResult,
		UINT message,
		WPARAM wParam,
		LPARAM lParam,
		CEdit & wndEdit,
		CExtBarTextFieldButton * pTextFieldTBB
		);
	static bool stat_CbInitListBoxContent(
		CExtPopupInplaceListBox & wndListBox,
		CExtBarTextFieldButton * pTextFieldTBB
		);
	static CSize stat_CbAdjustListBoxSize(
		CExtPopupInplaceListBox & wndListBox,
		CExtBarTextFieldButton * pTextFieldTBB
		);
	static bool stat_CbListBoxSelection(
		CExtPopupInplaceListBox & wndListBox,
		CExtBarTextFieldButton * pTextFieldTBB,
		int eSAT // CExtPopupInplaceListBox::e_sel_action_t
		);
	static bool stat_CbListBoxItemDraw(
		CExtPopupInplaceListBox & wndListBox,
		CExtBarTextFieldButton * pTextFieldTBB,
		LPDRAWITEMSTRUCT pDIS
		);
	static bool stat_CbListBoxItemMeasure(
		CExtPopupInplaceListBox & wndListBox,
		CExtBarTextFieldButton * pTextFieldTBB,
		LPMEASUREITEMSTRUCT pMIS
		);
	bool CopyToClipboard(
		CExtSafeString & sText
		);

public:
	LPARAM m_lParamCookie;
	DWORD m_dwListBoxStyles; // popup list-box styles
	CExtSafeStringArray m_arrLbItems;
	INT m_nDropDownWidth; // (-1) - auto calc, (-2) - same as button area
	INT m_nDropDownHeightMax;
	bool m_bTextFieldIsNotEditable:1, m_bRunInplaceControlAfterPopupClosed:1;

	class CInPlaceEditWnd : public CEdit
	{
		CExtToolControlBar * m_pBar;
		CRuntimeClass * m_pBtnRTC;
		CExtBarTextFieldButton * m_pTbbTextField;
		pCbVerifyTextInput m_pCbVerifyTextInput;
		pCbInplaceEditWndProc m_pCbWndProc;
		LPVOID m_pCbCookie;
		CExtSafeString * m_pStr;
		bool m_bCanceling:1;
		bool _IsValidState();
	public:
		CInPlaceEditWnd(
			CExtBarTextFieldButton * pTextFieldTBB,
			CExtSafeString * pStr,
			pCbVerifyTextInput pCbVerify = NULL,
			pCbInplaceEditWndProc pCbWndProc = NULL,
			LPVOID pCbCookie = NULL
			);
		~CInPlaceEditWnd();
#ifdef _DEBUG
		virtual void AssertValid() const;
#endif // _DEBUG
		virtual bool Create();
		virtual LRESULT WindowProc(
			UINT message,
			WPARAM wParam,
			LPARAM lParam
			);
	protected:
		virtual void PostNcDestroy();
		virtual BOOL PreTranslateMessage( MSG * pMsg );
	public:
		CExtBarTextFieldButton * GetBarTextFieldButton()
		{
			ASSERT_VALID( this );
			return m_pTbbTextField;
		}
		const CExtBarTextFieldButton * GetBarTextFieldButton() const
		{
			ASSERT_VALID( this );
			return m_pTbbTextField;
		}
		static CInPlaceEditWnd * g_pWndInplaceEditor;
	}; // class CInPlaceEditWnd

	DECLARE_DYNCREATE( CExtBarTextFieldButton );

	CExtBarTextFieldButton(
		bool bComboField = false,
		INT nTextFieldWidth = __EXT_MENU_DEF_INPLACE_EDIT_WIDTH,
		CExtToolControlBar * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		);
	virtual ~CExtBarTextFieldButton();
	
	virtual INT GetTextFieldWidth() const;
	virtual INT SetTextFieldWidth( INT nTextFieldWidth );
	virtual __EXT_MFC_SAFE_LPCTSTR GetFieldText() const;
	virtual bool SetFieldText(
		__EXT_MFC_SAFE_LPCTSTR sText,
		bool bVerify = true
		);
	bool IsComboTextField() const
	{
		ASSERT_VALID( this );
		return m_bComboField;
	}
	bool IsComboPopupDropped() const
	{
		ASSERT_VALID( this );
		return (IsComboTextField() && m_bComboPopupDropped) ? true : false;
	}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	virtual bool OnKeyTipInvokeAction(
		bool & bContinueKeyTipMode
		);
#endif // from (!defined __EXT_MFC_NO_CUSTOMIZE)
	virtual bool AnimationClient_OnQueryEnabledState(
		INT eAPT // __EAPT_*** animation type
		) const;

	// CExtBarTextFieldButton overrides
	virtual CRect OnCalcComboDropRect( const RECT & rcBtnArea ) const;
	virtual CRect OnInplaceControlCalcRect( const RECT & rcBtnArea ) const;
	virtual CRect OnInplaceControlCalcMarginSizes() const;
	virtual CWnd * OnInplaceControlCreate();
	static CExtBarTextFieldButton * g_pBtnEditing;
	virtual void OnInplaceControlRun();
	virtual void OnInplaceControlSessionEnd();
	virtual void OnInplaceControlSessionCancel();
	virtual bool OnInplaceControlVerifyTextInput(
		CEdit * pEdit,
		__EXT_MFC_SAFE_LPCTSTR sTextOld,
		__EXT_MFC_SAFE_LPCTSTR sTextNew
		);
	virtual void OnInplaceControlPutTextInputResult(
		CEdit * pEdit,
		__EXT_MFC_SAFE_LPCTSTR sTextNew
		);
	virtual bool OnInplaceControlWndProcCall(
		LRESULT & lResult,
		UINT message,
		WPARAM wParam,
		LPARAM lParam,
		CEdit & wndEdit
		);
	virtual CExtPopupMenuWnd * OnCreateDropPopup(
		bool bContentExpand // true - inserting to content expand menu, false - tracking dropped popup
		);
	virtual CSize OnPopupListBoxMeasureTrackSize() const;
	virtual bool OnPopupListBoxInitContent(
		CExtPopupInplaceListBox & wndListBox
		);
	virtual CSize OnPopupListBoxAdjustSize(
		CExtPopupInplaceListBox & wndListBox
		);
	virtual bool OnPopupListBoxSelChange(
		CExtPopupInplaceListBox & wndListBox
		);
	virtual bool OnPopupListBoxSelEndOK(
		CExtPopupInplaceListBox & wndListBox
		);
	virtual bool OnPopupListBoxSelEndCancel(
		CExtPopupInplaceListBox & wndListBox
		);
	virtual bool OnPopupListBoxClose(
		CExtPopupInplaceListBox & wndListBox
		);
	virtual bool OnPopupListBoxItemDraw(
		CExtPopupInplaceListBox & wndListBox,
		LPDRAWITEMSTRUCT pDIS
		);
	virtual bool OnPopupListBoxItemMeasure(
		CExtPopupInplaceListBox & wndListBox,
		LPMEASUREITEMSTRUCT pMIS
		);
	virtual CSize OnPopupListBoxCalcItemExtraSizes() const;

	// CExtBarButton overrides
protected:
	virtual void _UpdateCtrl();
public:
	virtual CRect RectDropDown() const;
	virtual CExtSafeString GetText() const;
	virtual bool GetSeparatedDropDown() const;
	virtual void SetSeparatedDropDown(
		bool bSeparatedDropDown = true
		);
	virtual bool GetAutoChangeID() const;
	virtual void SetAutoChangeID(
		bool bAutoChangeID = true
		);
	virtual void SetMenu(
		HMENU hMenu,
		bool bPopupMenu,
		bool bAutoDestroyMenu
		);
	virtual HMENU GetMenu();
	virtual void SetRect( const RECT & rectButton );
	bool CanBePressedInDisabledState();
	virtual bool IsAbleToTrackMenu(
		bool bCustomizeMode = false
		) const;
	virtual bool IsContainerOfPopupLikeMenu();
	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);
#if (!defined __EXT_MFC_NO_RIBBON_BAR)
	virtual CSize RibbonILV_CalcSize(
		CDC & dc,
		INT nILV = -1 // -1 use current visual level
		) const;
#endif // (!defined __EXT_MFC_NO_RIBBON_BAR)
	virtual BOOL PutToPopupMenu(
		CExtPopupMenuWnd * pPopup
		);
	virtual UINT OnTrackPopup(
		CPoint point,
		bool bSelectAny,
		bool bForceNoAnimation
		);
	virtual void OnHover(
		CPoint point,
		bool bOn,
		bool & bSuspendTips
		);
	virtual void OnHovering(
		CPoint point,
		bool & bSuspendTips
		);
	virtual void OnClick(
		CPoint point,
		bool bDown
		);
	virtual bool OnRClick(
		CPoint point,
		bool bDown
		);
	virtual void OnDeliverCmd();
	virtual __EXT_MFC_INT_PTR OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		);
	virtual LRESULT OnHelpHitTest(
		CPoint point
		);
	virtual void OnUpdateCmdUI(
		CWnd * pTarget,
		BOOL bDisableIfNoHndler,
		int nIndex
		);
	virtual bool OnSetCursor( CPoint point );

	virtual void PaintCompound(
		CDC & dc,
		bool bPaintParentChain,
		bool bPaintChildren,
		bool bPaintOneNearestChildrenLevelOnly
		);

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	virtual int GetInitialResizingStateH( // -1 - left side resizing, 1 - right side resizing, 0 - no resizing at specified point (in bar's client coord)
		CPoint point,
		int * p_nWidthMin = NULL,
		int * p_nWidthMax = NULL
		);
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
/*
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
protected:
	virtual void _UpdateCmdNodeFromStyle();
public:
	virtual bool IsCustomizeActiveItem();
	virtual void SetCustomizeActiveItem();
	virtual int CalculateDropOverEffect(
		CPoint point,
		CExtCustomizeSite::CCmdDragInfo & _dragInfo,
		bool bAllowTrackNodePopup = true
		);
	virtual CExtCustomizeCmdTreeNode * GetCmdNode(
		bool bInitial = false
		);
	virtual void OnCustomizeUpdateProps(
		CExtCustomizeCmdTreeNode * pNode = NULL // update from basic node
		);
	virtual void SetBasicCmdNode(
		CExtCustomizeCmdTreeNode * pNode
		);
	virtual void SetCustomizedCmdNode(
		CExtCustomizeCmdTreeNode * pNode
		);
	virtual CExtCustomizeCmdTreeNode * OnCustomizeNodeInit(
		CExtCustomizeSite * pSite,
		CExtCustomizeCmdTreeNode * pParentNode // toolbar node
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
*/

}; // class CExtBarTextFieldButton

/////////////////////////////////////////////////////////////////////////////
// CExtBarDateFieldButton

#ifndef __EXT_MFC_NO_BUILTIN_DATEFIELD

class __PROF_UIS_API CExtBarDateFieldButton : public CExtBarTextFieldButton
{
public:
	DECLARE_DYNCREATE( CExtBarDateFieldButton );

	CExtBarDateFieldButton(
		INT nTextFieldWidth = __EXT_MENU_DEF_INPLACE_EDIT_WIDTH,
		CExtToolControlBar * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		);
	virtual ~CExtBarDateFieldButton();

	virtual CExtPopupMenuWnd * OnCreateDropPopup(
		bool bContentExpand // true - inserting to content expand menu, false - tracking dropped popup
		);

	virtual CSize OnPopupListBoxMeasureTrackSize() const
	{
		ASSERT( FALSE );
		return CSize(0,0);
	}
	virtual bool OnPopupListBoxInitContent(
		CExtPopupInplaceListBox & wndListBox
		)
	{
		UNREFERENCED_PARAMETER( wndListBox );
		ASSERT( FALSE );
		return false;
	}
	virtual bool OnPopupListBoxSelChange(
		CExtPopupInplaceListBox & wndListBox
		)
	{
		UNREFERENCED_PARAMETER( wndListBox );
		return false;
	}
	virtual bool OnPopupListBoxSelEndOK(
		CExtPopupInplaceListBox & wndListBox
		)
	{
		UNREFERENCED_PARAMETER( wndListBox );
		ASSERT( FALSE );
		return false;
	}
	virtual bool OnPopupListBoxSelEndCancel(
		CExtPopupInplaceListBox & wndListBox
		)
	{
		UNREFERENCED_PARAMETER( wndListBox );
		ASSERT( FALSE );
		return false;
	}
	virtual bool OnPopupListBoxItemDraw(
		CExtPopupInplaceListBox & wndListBox,
		LPDRAWITEMSTRUCT pDIS
		)
	{
		UNREFERENCED_PARAMETER( wndListBox );
		UNREFERENCED_PARAMETER( pDIS );
		ASSERT( FALSE );
		return false;
	}
	virtual bool OnPopupListBoxItemMeasure(
		CExtPopupInplaceListBox & wndListBox,
		LPMEASUREITEMSTRUCT pMIS
		)
	{
		UNREFERENCED_PARAMETER( wndListBox );
		UNREFERENCED_PARAMETER( pMIS );
		ASSERT( FALSE );
		return false;
	}
	virtual CSize OnPopupListBoxCalcItemExtraSizes() const
	{
		return CSize(0,0);
	}

	virtual bool OnPopupDatePickerInitContent(
		CExtDatePickerWnd & wndDatePicker
		);

	virtual bool OnPopupDatePickerSelChange(
		LPVOID pSelectionNotification
		);

protected:

	static bool stat_CbInitDatePickerContent(
		CExtDatePickerWnd & wndDatePicker,
		CExtBarDateFieldButton * pDateFieldTBB
		);
	static bool stat_CbDatePickerSelection(
		LPVOID pSelectionNotification,
		CExtBarDateFieldButton * pDateFieldTBB
		);

}; // class CExtBarDateFieldButton

#endif // __EXT_MFC_NO_BUILTIN_DATEFIELD

/////////////////////////////////////////////////////////////////////////////
// CExtBarSpinButton

class __PROF_UIS_API CExtBarSpinButton : public CExtBarTextFieldButton
{
public:
	DECLARE_DYNCREATE( CExtBarSpinButton );

	CExtBarSpinButton(
		INT nTextFieldWidth = __EXT_MENU_DEF_INPLACE_EDIT_WIDTH,
		CExtToolControlBar * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		);
	virtual ~CExtBarSpinButton();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	INT SetPos( 
		INT nPos 
		);
	INT GetPos() const;

	void SetRange( 
		INT nLower, 
		INT nUpper 
		);
	void GetRange(
		INT & nLower, 
		INT & nUpper
		) const;

	INT SetStep( 
		INT nStep
		);
	INT GetStep() const;

public:
	INT m_nRangeLower;
	INT m_nRangeUpper;
	INT m_nPos;
	INT m_nStep;

protected:
	CRect m_rcButtonUp;
	CRect m_rcButtonDown;

public:
	virtual bool IsAbleToTrackMenu(
		bool bCustomizeMode = false
		) const;
	virtual CRect OnInplaceControlCalcRect( 
		const RECT & rcBtnArea 
		) const;
	virtual void OnHover(
		CPoint point,
		bool bOn,
		bool & bSuspendTips
		);
	virtual void OnClick(
		CPoint point,
		bool bDown
		);
	virtual bool SetFieldText(
		__EXT_MFC_SAFE_LPCTSTR sText,
		bool bVerify = true
		);
	virtual bool OnInplaceControlWndProcCall(
		LRESULT & lResult,
		UINT message,
		WPARAM wParam,
		LPARAM lParam,
		CEdit & wndEdit
		);
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
	virtual CRect OnCalcSpinButtonsRect( 
		const RECT & rcBtnArea 
		) const;
	virtual CRect OnCalcSpinButtonsMarginSizes() const;

}; // class CExtBarSpinButton

#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)

#if (!defined __EXT_MFC_NO_DATE_PICKER_POPUP)

/////////////////////////////////////////////////////////////////////////////
// CExtPopupInplaceDatePicker

class __PROF_UIS_API CExtPopupInplaceDatePicker : public CExtDatePickerWnd
{
public:
	DECLARE_DYNCREATE( CExtPopupInplaceDatePicker );

	CExtPopupInplaceDatePicker();
	virtual ~CExtPopupInplaceDatePicker();

// Attributes
public:

	typedef
		bool (*pCbInitDatePickerContent)(
			CExtPopupInplaceDatePicker & wndDatePicker,
			LPVOID pInitDatePickerCookie
			);
	pCbInitDatePickerContent m_pCbInitDatePickerContent;
	LPVOID m_pInitDatePickerCookie;

	typedef
		bool (*pCbDatePickerSelection)(
			LPVOID pSelectionNotification,
			LPVOID pDatePickerSelectionCookie
			);
	pCbDatePickerSelection m_pCbDatePickerSelection;
	LPVOID m_pDatePickerSelectionCookie;

	static UINT g_nMsgPopupDatePickerInitContent;
	
	LPARAM m_lParamCookie;

// Implementation
protected:

// Overrides
	//{{AFX_VIRTUAL(CExtPopupInplaceDatePicker)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtPopupInplaceDatePicker)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtPopupInplaceDatePicker
	
	
/////////////////////////////////////////////////////////////////////////////
// CExtPopupDatePickerMenuWnd

class __PROF_UIS_API CExtPopupDatePickerMenuWnd : public CExtPopupControlMenuWnd
{
public:
	DECLARE_DYNCREATE( CExtPopupDatePickerMenuWnd );

	CExtPopupDatePickerMenuWnd(
		LPARAM lParamCookie = 0L,
		CSize sizeCalendarDimensions = CSize(1,1),
		DWORD dwDatePickerWindowStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,
		DWORD dwDatePickerStyle = __EDPWS_DEFAULT
		);
	virtual void Construct(
		LPARAM lParamCookie = 0L,
		CSize sizeCalendarDimensions = CSize(1,1),
		DWORD dwDatePickerWindowStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,
		DWORD dwDatePickerStyle = __EDPWS_DEFAULT
		);
	virtual ~CExtPopupDatePickerMenuWnd();

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

// Attributes
public:
	CExtDatePickerWnd::SELECTION_INFO * m_pExternalSelectionInfo;
	LPARAM m_lParamCookie;
	CSize m_sizeCalendarDimensions;
	DWORD m_dwDatePickerWindowStyle;
	DWORD m_dwDatePickerStyle;

	CExtPopupInplaceDatePicker::pCbDatePickerSelection
		m_pCbDatePickerSelection;
	LPVOID m_pDatePickerSelectionCookie;

	CExtPopupInplaceDatePicker::pCbInitDatePickerContent
		m_pCbInitDatePickerContent;
	LPVOID m_pInitDatePickerCookie;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtPopupDatePickerMenuWnd)
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
	virtual CRect OnQueryMenuBorderMetrics() const;
protected:
	virtual HWND OnCreateChildControl(
		const RECT & rcChildControl
		);

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtPopupDatePickerMenuWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtPopupDatePickerMenuWnd

#endif // (!defined __EXT_MFC_NO_DATE_PICKER_POPUP)

#ifndef __EXT_MFC_NO_UNDO_REDO_POPUP

// scrolling undo/redo list
#define __EDPW_SCROLL_UNDO_REDO_ACCELERATION_STEP  10  // pixels
#define __EDPW_SCROLL_UNDO_REDO_UP_TIMER_ID			2
#define __EDPW_SCROLL_UNDO_REDO_DOWN_TIMER_ID		3

#define __EDPW_SCROLL_UNDO_REDO_TIMER_ELAPSE1		500 // milliseconds
#define __EDPW_SCROLL_UNDO_REDO_TIMER_ELAPSE2		300 
#define __EDPW_SCROLL_UNDO_REDO_TIMER_ELAPSE3		200 
#define __EDPW_SCROLL_UNDO_REDO_TIMER_ELAPSE4		100  
#define __EDPW_SCROLL_UNDO_REDO_TIMER_ELAPSE5		50  

/////////////////////////////////////////////////////////////////////////////
// CExtPopupUndoRedoListBox

class __PROF_UIS_API CExtPopupUndoRedoListBox : public CExtPopupInplaceListBox
{
public:
	DECLARE_DYNCREATE( CExtPopupUndoRedoListBox );

	CExtPopupUndoRedoListBox();
	virtual ~CExtPopupUndoRedoListBox();

// Implementation
protected:
	bool m_bScrollingUp:1, m_bScrollingDown:1;
	UINT m_nLastElapseTimerUp, m_nLastElapseTimerDown;
	bool m_bLButtonPushed:1;

	void SetSelection( INT nSelCount );
	void StopScrolling();
	UINT GetScrollingSpeed( int nPixels );
	virtual void _DoSelChange();

// Overrides
	//{{AFX_VIRTUAL(CExtPopupUndoRedoListBox)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtPopupUndoRedoListBox)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtPopupUndoRedoListBox

/////////////////////////////////////////////////////////////////////////////
// CExtPopupUndoRedoMenuWnd

class __PROF_UIS_API CExtPopupUndoRedoMenuWnd : public CExtPopupControlMenuWnd
{
public:
	DECLARE_DYNCREATE( CExtPopupUndoRedoMenuWnd );

	CExtPopupUndoRedoMenuWnd(
		LPARAM lParamCookie = 0L,
		DWORD dwListBoxStyles = WS_CHILD|WS_VISIBLE|WS_VSCROLL|LBS_MULTIPLESEL|LBS_HASSTRINGS|LBS_OWNERDRAWVARIABLE
		);
	virtual ~CExtPopupUndoRedoMenuWnd();

	typedef
		bool (*pCbFormatCaption)(
			CExtSafeString & strCaption,
			CExtPopupUndoRedoMenuWnd * pPopupUndoRedoMenuWnd,
			LPVOID pFormatCaptionCookie
			);

// Attributes
public:
	LPARAM m_lParamCookie;
	DWORD m_dwListBoxStyles;

	CExtPopupUndoRedoMenuWnd::pCbFormatCaption
		m_pCbFormatCaption;
	LPVOID m_pFormatCaptionCookie;

	CExtPopupInplaceListBox::pCbListBoxSelection
		m_pCbListBoxSelection;
	LPVOID m_pSelectionCookie;

	CExtPopupInplaceListBox::pCbInitListBoxContent
		m_pCbInitListBoxContent;
	LPVOID m_pInitListBoxCookie;

	CExtPopupInplaceListBox::pCbListBoxItemDraw
		m_pCbListBoxItemDraw;

	CExtPopupInplaceListBox::pCbListBoxItemMeasure
		m_pCbListBoxItemMeasure;
	LPVOID m_pListBoxItemCookie;

	virtual int OnQueryTextAreaHeight() const;

	virtual void _RecalcLayoutImpl();
	virtual CRect _RecalcControlRect();
	
	virtual void _DoPaint( CDC & dcPaint, bool bUseBackBuffer = true );

	void SetActionsCount( INT nActionsCount );
	INT GetActionsCount() const;

// Implementation
protected:
	CRect m_rcTextArea;
	INT m_nActionsCount;

public:
	virtual CSize _CalcTrackSize();
	virtual HWND OnCreateChildControl(
		const RECT & rcChildControl
		);
protected:
	virtual void OnPaintUndoRedoCaption(
		CDC & dc,
		CRect rcItem
		);

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtPopupUndoRedoMenuWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtPopupUndoRedoMenuWnd

#endif // __EXT_MFC_NO_UNDO_REDO_POPUP

#endif // __EXT_POPUP_CTRL_MENU_H

