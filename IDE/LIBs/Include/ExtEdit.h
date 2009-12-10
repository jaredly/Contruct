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

#if (!defined __EXT_EDIT_H)
#define __EXT_EDIT_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CExtEditBase window

class __PROF_UIS_API CExtEditBase
	: public CEdit
	, public CExtPmBridge
{
public:
	DECLARE_DYNCREATE( CExtEditBase );
	DECLARE_CExtPmBridge_MEMBERS( CExtEditBase );

	DECLARE_PROF_UIS_WINDOW_METHODS;

	CExtEditBase();

// Attributes
public:
	bool m_bHandleCtxMenus:1;
protected:
	CToolTipCtrl m_wndToolTip;
	void InitToolTip();

	COLORREF m_clrBack;
	COLORREF m_clrBackPrev;
	CBrush m_brBack;
	COLORREF m_clrText;

protected:
    CExtSafeString m_strCueBanner;

public:
    bool SetCueBanner( __EXT_MFC_SAFE_LPCTSTR lpcText );
    bool GetCueBanner( __EXT_MFC_SAFE_LPTSTR lpText, int cchText ) const;
    CExtSafeString GetCueBanner() const;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtEditBase)
	virtual BOOL PreTranslateMessage( MSG * pMsg );
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtEditBase();

	void SetTooltipText(
		int nId,
		BOOL bActivate =
		TRUE
		);
	void SetTooltipText(
		CExtSafeString * spText,
		BOOL bActivate = TRUE
		);
	void SetTooltipText(
		CExtSafeString & sText,
		BOOL bActivate = TRUE
		);
	void SetTooltipText(
		__EXT_MFC_SAFE_LPCTSTR sText,
		BOOL bActivate = TRUE
		);
	void ActivateTooltip(
		BOOL bEnable = TRUE
		);

	void SetBkColor( COLORREF clrBk );
	COLORREF GetBkColor() const;

	void SetTextColor( COLORREF clrText );
	COLORREF GetTextColor() const;
	
protected:
	virtual __EXT_MFC_INT_PTR OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		) const;

	virtual COLORREF OnQueryBackColor() const;
	virtual COLORREF OnQueryTextColor() const;

	virtual int OnQueryMaxTipWidth( 
		__EXT_MFC_SAFE_LPCTSTR lpszText 
		);

public:
	static void stat_DoPaintNc(
		bool bEnabled,
		bool bMouseOver,
		CWnd * pWnd,
		CExtPmBridge * pPmBridge,
		CDC * pDC
		);
	static void stat_DrawEditImpl(
		bool bEnabled,
		bool bMouseOver,
		CWnd * pWnd,
		CExtPmBridge * pPmBridge,
		CRect rectClient,
		CDC * pDC
		);
	static void stat_PrintClient(
		bool bEnabled,
		bool bMouseOver,
		UINT message,
		LPARAM lParam,
		CWnd * pWnd,
		CExtPmBridge * pPmBridge,
		CDC * pDC
		);

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtEditBase)
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd,CPoint pos );	
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
}; // class CExtEditBase

/////////////////////////////////////////////////////////////////////////////
// CExtED (for CExtEdit and CExtEditHotKey classes)

#define __EXT_EDIT_UPDATE_TIMER			1
#define __EXT_EDIT_UPDATE_TIMER_PERIOD	100

template < class _BED >
class CExtED : public _BED
{
public:
	CExtED()
		: m_bMouseOver( false )
	{
	}

protected:
	virtual void _DoPaintNC( CDC * pDC )
	{
		ASSERT_VALID( this );
		ASSERT_VALID( pDC );
		ASSERT( pDC->GetSafeHdc() != NULL );
		CExtEditBase::stat_DoPaintNc( 
			OnQueryWindowEnabledState(), 
			m_bMouseOver, 
			this, 
			this, 
			pDC 
			);
	}
	virtual void _PostRedraw()
	{
		if(		m_hWnd != NULL 
			&&	::IsWindow( m_hWnd )
			)
		{
			VERIFY(
				RedrawWindow(
				NULL,
				NULL,
				RDW_INVALIDATE | RDW_UPDATENOW
				| RDW_ERASE | RDW_ERASENOW
				| RDW_FRAME | RDW_ALLCHILDREN
				)
				);
		}
	}
	virtual void _DrawEditImpl(
		CRect rectClient,
		CDC * pDC = NULL
		)
	{
		CExtEditBase::stat_DrawEditImpl(
			OnQueryWindowEnabledState(),
			m_bMouseOver,
			this,
			this,
			rectClient,
			pDC
			);
	}
	bool m_bMouseOver:1;

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch( message )
		{
		case WM_PRINT:
		case WM_PRINTCLIENT:
			{
				CDC * pDC = CDC::FromHandle( (HDC) wParam );
				CExtEditBase::stat_PrintClient(
					OnQueryWindowEnabledState(),
					m_bMouseOver,
					message,
					lParam,
					this,
					this,
					pDC
					);
				return (!0L);
			}
			break;
		case WM_TIMER:
			{
				if( wParam == __EXT_EDIT_UPDATE_TIMER )
				{
					POINT pt;
					if(		IsWindowVisible()
						&&	::GetCursorPos( &pt ) 
						)
					{
						bool bOldMouseOver = m_bMouseOver;
						CRect rectItem;
						GetWindowRect( &rectItem );
						if( !rectItem.PtInRect(pt) )
						{
							KillTimer( __EXT_EDIT_UPDATE_TIMER );
							m_bMouseOver = false;
						}
						else 
							if(	::WindowFromPoint( pt ) == m_hWnd )
								m_bMouseOver = true;
							if( bOldMouseOver != m_bMouseOver )
								_PostRedraw();
					}
					return 0L;
				}
			}
			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			{
				LRESULT lRes = _BED::WindowProc(message, wParam, lParam);
				_PostRedraw();
				return lRes;
			}
			break;
		case WM_SETCURSOR:
			{
				if( HIWORD( lParam ) == WM_MOUSEMOVE )
				{
					CRect rcWnd;
					GetWindowRect( &rcWnd );
					if( rcWnd.PtInRect( GetCurrentMessage()->pt ) )
					{
						SetTimer( 
							__EXT_EDIT_UPDATE_TIMER, 
							__EXT_EDIT_UPDATE_TIMER_PERIOD, 
							NULL 
							);
						SendMessage( WM_TIMER, __EXT_EDIT_UPDATE_TIMER, 0L );
					}
				}
			}
			break;
		case WM_NCPAINT:
			{
				CWindowDC dc( this );
				_DoPaintNC( &dc );
				return 0L;
			}
			break;
		}
		return _BED::WindowProc(message, wParam, lParam);		
	}
}; // class CExtED

/////////////////////////////////////////////////////////////////////////////
// CExtEdit window

#if _MFC_VER >= 0x700
	#ifndef __EXT_PROFUIS_STATIC_LINK
		template class __PROF_UIS_API CExtED < CExtEditBase >;
	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
#endif // _MFC_VER >= 0x700

class __PROF_UIS_API CExtEdit : public CExtED < CExtEditBase >
{
public:
	DECLARE_DYNCREATE( CExtEdit );
	CExtEdit();
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
}; // class CExtEdit

/////////////////////////////////////////////////////////////////////////////
// CExtIPAddressCtrl window

class __PROF_UIS_API CExtIPAddressCtrl 
	: public CIPAddressCtrl
	, public CExtPmBridge
{
public:
	DECLARE_DYNCREATE( CExtIPAddressCtrl );
	DECLARE_CExtPmBridge_MEMBERS( CExtIPAddressCtrl );

	CExtIPAddressCtrl();
	virtual ~CExtIPAddressCtrl();
	
// Attributes
public:

// Operations
public:
	BOOL Create(
		DWORD dwStyle, 
		const RECT & rect,
		CWnd * pParentWnd, 
		UINT nID
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtIPAddressCtrl)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual bool _CreateHelper();
	
	virtual void _DoPaintNC( CDC * pDC );
	virtual void _PostRedraw();
	virtual void _DrawEditImpl(
		CRect rectClient,
		CDC * pDC = NULL
		);

	bool m_bDirectCreateCall:1;
	bool m_bInitialized:1;
	bool m_bMouseOver:1;

	WNDPROC m_pWNDPROC[4];
	HWND m_hWndHooked[4];
	static CTypedPtrList < CPtrList, CExtIPAddressCtrl * > g_listTargets;

	static LRESULT CALLBACK g_HookWndProc(
		HWND hWnd,
		UINT nMessage,
		WPARAM wParam,
		LPARAM lParam
		);
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CExtIPAddressCtrl)
 	afx_msg void OnDestroy();
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
}; // class CExtIPAddressCtrl

/////////////////////////////////////////////////////////////////////////////
// CExtEditHotKeyBase window

class __PROF_UIS_API CExtEditHotKeyBase : public CExtEditBase
{
// Construction
public:
	DECLARE_DYNCREATE(CExtEditHotKeyBase);
	CExtEditHotKeyBase();
	
// Attributes
protected:
	bool m_bAllowTabKeyInput:1;
	BYTE m_nSimpleKeyModifiers;
	ACCEL m_dataAccel;
	void _SyncEditText();
	void _Empty();

// Operations
public:
	static CExtSafeString stat_GetSingleKeyName(
		HKL hKeyboardLayout,
		WORD nKey,
		__EXT_MFC_SAFE_LPCTSTR strAppendIfNonEmpty = NULL,
		bool bMakeOnlyFirstLettersUpper = true
		);
	static CExtSafeString stat_FormatAccelText(
		const ACCEL & _dataAccel
		);

	bool IsAccelDefined() const;
	bool IsEmpty() const;
	void Empty();
	
	const ACCEL & AccelGet() const;
	void AccelSet( 
		const ACCEL & _dataAccel 
		);
	
	WORD KeyGet() const;
	void KeySet( 
		WORD nKey 
		);
	
	BYTE ModifiersGet() const;
	void ModifiersSet( 
		BYTE nModifiers 
		);
	
	BYTE SimpleKeyModifiersGet() const;
	void SimpleKeyModifiersSet( 
		BYTE nModifiers 
		);
	
	bool AllowTabKeyInputGet() const;
	void AllowTabKeyInputSet( 
		bool bAllowTabKeyInput 
		);

	virtual CExtSafeString OnGetEmptyText();
	virtual CExtSafeString OnGetAccelText();
	virtual bool OnAccelChanging(
		const ACCEL & _dataAccelOld,
		const ACCEL & _dataAccelNew
		);
	virtual void OnAccelChanged();
	virtual void OnAccelDefined();

	virtual bool OnTestEraseAccel( 
		const ACCEL & _dataAccel 
		);
	virtual bool OnTestSimpleKey( 
		WORD nKey 
		);
	virtual bool OnTestDisabledKey( 
		WORD nKey 
		);

// Implementation
public:
	virtual ~CExtEditHotKeyBase();

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtEditHotKeyBase)
	virtual BOOL PreTranslateMessage( MSG * pMsg );
	protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	//}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(CExtEditHotKeyBase)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

}; // class CExtEditHotKeyBase

/////////////////////////////////////////////////////////////////////////////
// CExtEditHotKey window

#if _MFC_VER >= 0x700
	#ifndef __EXT_PROFUIS_STATIC_LINK
		template class __PROF_UIS_API CExtED < CExtEditHotKeyBase >;
	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
#endif // _MFC_VER >= 0x700

class __PROF_UIS_API CExtEditHotKey : public CExtED < CExtEditHotKeyBase >
{
public:
	DECLARE_DYNCREATE( CExtEditHotKey );
	CExtEditHotKey();
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtEditHotKey)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtEditHotKey();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtEditHotKey)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
}; // class CExtEditHotKey

__PROF_UIS_API void AFXAPI __EXT_MFC_DDX_HotKey( CDataExchange * pDX, INT nIDC, ACCEL & value );

/////////////////////////////////////////////////////////////////////////////
// CExtEditWithBehavior

class __PROF_UIS_API CExtEditWithBehavior : public CExtEdit
{
public:
	DECLARE_DYNAMIC( CExtEditWithBehavior );
	CExtEditWithBehavior();
	virtual ~CExtEditWithBehavior();
	void TextSet( __EXT_MFC_SAFE_LPCTSTR strText );
	CExtSafeString TextGet() const;
	CExtSafeString TextGetTrimmed() const;
protected:
	virtual void _UpdateContent();
public:
	virtual CExtSafeString TextGetValid() const;
	virtual bool Behavior_OnQueryCharEnabledTyping( __EXT_MFC_SAFE_TCHAR _tchr ) const;
private:
	enum e_InternalStateFlags_t
	{
		__EISF_CLEAN = 0x0000,
		__EISF_TCWAS = 0x0001, // text color was already set
	};
	UINT m_nInternalFlags;
public:
	class __PROF_UIS_API CBehaviorBase
	{
	public:
		CBehaviorBase();
		virtual ~CBehaviorBase();
		bool _FlagsModify( UINT uAdd, UINT uRemove );
		UINT _FlagsGet() const;
		virtual CExtSafeString Behavior_GetValidText() const = 0;
		virtual void Behavior_OnChar( UINT uChar, UINT nRepCnt, UINT nFlags ) = 0;
		virtual void Behavior_OnKeyDown( UINT uChar, UINT nRepCnt, UINT nFlags );
		virtual void Behavior_OnKillFocus( CWnd * pNewWnd );
		virtual LRESULT Behavior_OnPaste( WPARAM wParam, LPARAM lParam );
	protected:
		virtual LRESULT _Default();
		virtual void _Redraw();
	public:
		virtual bool Behavior_OnQueryCharEnabledTyping(__EXT_MFC_SAFE_TCHAR _tchr ) const;
		CExtEditWithBehavior * m_pEditWithBehaviorWnd;
	protected:
		UINT m_nFlags;
	}; /// class CBehaviorBase
	friend class CBehaviorBase;
	class __PROF_UIS_API CBehaviorMasked : public CBehaviorBase
	{
	public:
		CBehaviorMasked(
			__EXT_MFC_SAFE_LPCTSTR strMask = _T("")
			);
		void MaskSet( __EXT_MFC_SAFE_LPCTSTR strMask );
		__EXT_MFC_SAFE_LPCTSTR MaskGet() const;
		CExtSafeString GetNumericText() const;
		void InsertModeSet( bool bInsertMode );
		bool InsertModeGet() const;
		void NullableModeSet( bool bNullableMode );
		bool NullableModeGet() const;
		void NullCharSet( __EXT_MFC_SAFE_TCHAR _tchrNullChar );
		__EXT_MFC_SAFE_TCHAR NullCharGet() const;
		void FieldSelectionModeSet( bool bFieldSelectionMode );
		bool FieldSelectionModeGet() const;
		class __PROF_UIS_API CharRule
		{
		public:
			#ifndef _UNICODE
				typedef int   (*ValidationFunction)(UINT);	// similar to _istdigit, _istalpha
				typedef UINT  (*ConversionFunction)(UINT);	// similar to _totupper, _totlower
			#else
				typedef int   (*ValidationFunction)(WCHAR);
				typedef WCHAR (*ConversionFunction)(WCHAR);
			#endif
			CharRule();
			CharRule( __EXT_MFC_SAFE_TCHAR cSymbol, ValidationFunction fnValidation, ConversionFunction fnConversion = NULL );
			virtual ~CharRule();
			virtual bool Validate( __EXT_MFC_SAFE_TCHAR _tchr ) const;
			virtual __EXT_MFC_SAFE_TCHAR Convert( __EXT_MFC_SAFE_TCHAR _tchr ) const;
			void Set( __EXT_MFC_SAFE_TCHAR cSymbol );
			__EXT_MFC_SAFE_TCHAR Get() const;
			operator __EXT_MFC_SAFE_TCHAR() const;
			__EXT_MFC_SAFE_TCHAR m_cSymbol;
			ValidationFunction m_fnValidation;
			ConversionFunction m_fnConversion;
		}; /// class CharRule
		class __PROF_UIS_API CharRuleArray : public CArray < CharRule, CharRule & >
		{
		public:
			INT FindPos( __EXT_MFC_SAFE_TCHAR tchrMask ) const
			{
				INT nIndex, nCount = INT( GetSize() );
				for( nIndex = 0; nIndex < nCount; nIndex ++ )
				{
					const CharRule & _rule = (*this)[ nIndex ];
					__EXT_MFC_SAFE_TCHAR tchrMaskRule = _rule.Get();
					if( tchrMask == tchrMaskRule )
						return nIndex;
				}
				return -1;
			}
			bool CheckRule(
				__EXT_MFC_SAFE_TCHAR tchrMask,
				__EXT_MFC_SAFE_TCHAR tchrInput,
				bool * p_bRuleWasFound = NULL,
				bool bConvert = false
				) const
			{
				INT nIndex = FindPos( tchrMask );
				if( nIndex >= 0 )
				{
					if( p_bRuleWasFound != NULL )
						(*p_bRuleWasFound) = true;
					const CharRule & _rule = (*this)[ nIndex ];
					if( bConvert )
						tchrInput = _rule.Convert( tchrInput );
					return _rule.Validate( tchrInput );
				}
				else
				{
					if( p_bRuleWasFound != NULL )
						(*p_bRuleWasFound) = false;
					if( tchrMask == tchrInput )
						return true;
					else
						return false;
				}
			}
		}; /// class CharRuleArray
		CharRuleArray & GetSymbolArray();
	protected:
		virtual CExtSafeString Behavior_GetValidText() const;
		virtual void Behavior_OnChar( UINT uChar, UINT nRepCnt, UINT nFlags );
		virtual void Behavior_OnKeyDown( UINT uChar, UINT nRepCnt, UINT nFlags );
		void _GetMaskBasedGroupArrays(
			CExtSafeStringArray & arrMaskGroups,
			CExtSafeStringArray * p_arrEnteredValues = NULL,
			bool bJoinSimilarRules = true,
			bool bJoinDifferentRules = false,
			bool bPutNullableCharIntoValueArray = false
			);
	public:
		virtual void OnCheckText( bool bAppendEndChars );
		virtual void OnInputChar( CExtSafeString & str );
	protected:
		bool _IsNullChar( int & _pos ) const;
		void _ShowNullableMask();
		bool _GetFieldSelection( int _currentPosition );
		void _DefineNewPosition( int & _nStart, int & _nEnd );
		void _MaskSymbolDelete( CExtSafeString & _str );
		void _MaskSymbolInsert( CExtSafeString _str );
		void _SymbolInsert( CExtSafeString _str );
		bool _NullCharBefore( int _nPos );
		bool _AddSymbolMaskToEnd();
		INT _GetCountMaskSymbolsFromEnd();
	public:
		CharRuleArray m_arrRules;
	protected:
		CExtSafeString m_strMask;
		bool m_bInsertMode:1;
		bool m_bNullableMode:1;
		bool m_bFieldSelectionMode:1;
		__EXT_MFC_SAFE_TCHAR m_tchrNullChar;
		bool m_bBack:1;
	}; /// class CBehaviorMasked
protected:
	//{{AFX_MSG(CExtEditWithBehavior)
	afx_msg void OnChange();
	afx_msg void OnUpdate();
	//}}AFX_MSG
	afx_msg LRESULT OnCut( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnPaste( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnClear( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSetText( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
}; /// class CExtEditWithBehavior

/////////////////////////////////////////////////////////////////////////////
// CExtEditMasked window

class __PROF_UIS_API CExtEditMasked
	: public CExtEditWithBehavior
	, public CExtEditWithBehavior::CBehaviorMasked
{
public:
	DECLARE_DYNCREATE( CExtEditMasked );
	CExtEditMasked(
		__EXT_MFC_SAFE_LPCTSTR strMask = _T("")
		);
protected:
	virtual CExtSafeString TextGetValid() const;
public:
	//{{AFX_VIRTUAL(CExtEditMasked)
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CExtEditMasked)
	afx_msg void OnChar( UINT uChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnKeyDown( UINT uChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnKillFocus( CWnd * pNewWnd );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; /// CExtEditMasked

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __EXT_EDIT_H
