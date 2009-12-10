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

#if (!defined __EXT_DURATIONWND_H)
#define __EXT_DURATIONWND_H

#if (!defined __EXT_MFC_NO_DURATIONWND)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_CONTROLBAR_H)
	#include <ExtControlBar.h>
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// control styles
#define __EDWS_NO_HIDE_SELECTION		0x00000001L
#define __EDWS_USER_STYLE_30			0x00000002L
#define __EDWS_USER_STYLE_29			0x00000004L
#define __EDWS_USER_STYLE_28			0x00000008L
#define __EDWS_USER_STYLE_27			0x00000010L
#define __EDWS_USER_STYLE_26			0x00000020L
#define __EDWS_USER_STYLE_25			0x00000040L
#define __EDWS_USER_STYLE_24			0x00000080L
#define __EDWS_USER_STYLE_23			0x00000100L
#define __EDWS_USER_STYLE_22			0x00000200L
#define __EDWS_USER_STYLE_21			0x00000400L
#define __EDWS_USER_STYLE_20			0x00000800L
#define __EDWS_USER_STYLE_19			0x00001000L
#define __EDWS_USER_STYLE_18			0x00002000L
#define __EDWS_USER_STYLE_17			0x00004000L
#define __EDWS_USER_STYLE_16			0x00008000L
#define __EDWS_USER_STYLE_15			0x00010000L
#define __EDWS_USER_STYLE_14			0x00020000L
#define __EDWS_USER_STYLE_13			0x00040000L
#define __EDWS_USER_STYLE_12			0x00080000L
#define __EDWS_USER_STYLE_11			0x00100000L
#define __EDWS_USER_STYLE_10			0x00200000L
#define __EDWS_USER_STYLE_09			0x00400000L
#define __EDWS_USER_STYLE_08			0x00800000L
#define __EDWS_USER_STYLE_07			0x01000000L
#define __EDWS_USER_STYLE_06			0x02000000L
#define __EDWS_USER_STYLE_05			0x04000000L
#define __EDWS_USER_STYLE_04			0x08000000L
#define __EDWS_USER_STYLE_03			0x10000000L
#define __EDWS_USER_STYLE_02			0x20000000L
#define __EDWS_USER_STYLE_01			0x40000000L
#define __EDWS_USER_STYLE_00			0x80000000L

// default style set for m_dwStyle
#define __EDWS_DEFAULT_STYLES			0L

// general hit-testing values
#define __EDWH_NOWHERE				(0L)
#define __EDWH_CLIENT				(1L)
#define __EDWH_BUTTONS				(2L)
#define __EDWH_BUTTON_DROPDOWN		(3L)
#define __EDWH_BUTTON_SCROLL_UP		(4L)
#define __EDWH_BUTTON_SCROLL_DOWN	(5L)

// Duration wnd class name
#define __EXT_DURATIONWND_CLASS_NAME _T("ProfUIS-DurationWnd")

/////////////////////////////////////////////////////////////////////////////
// CExtDurationWnd window
 
class __PROF_UIS_API CExtDurationWnd
	: public CWnd
	, public CExtAnimationSingleton
{
// Construction
public:
	DECLARE_DYNCREATE( CExtDurationWnd );
	CExtDurationWnd();
 
// Attributes
protected:
	HFONT m_hFont;

public:
	enum eAlign_t
	{
		left = 0,
		right = 1,
		center = 2,
	};
	enum eStatus_t
	{
		valid = 0,
		invalid = 1,    // Invalid (out of range, etc.)
		null = 2,       // Literally has no value
	};
	enum eItem_t
	{
		label = 0,
		year = 1,
		month = 2,
		day = 3,
		hour = 4,
		minute = 5,
		second = 6,
		designator = 7
	};

	class ITEM_INFO;
	
	typedef
		CArray <
			ITEM_INFO *,
			ITEM_INFO *
			>
		ItemsArr_t;

	class __PROF_UIS_API ITEM_INFO
	{
	public:
		ITEM_INFO()
			: m_eItemType( CExtDurationWnd::label )
			, m_sText( _T("") )
			, m_nValue( 0 )
			, m_rcRect( 0, 0, 0, 0 )
			, m_bVisible( true )
			, m_bBefore( true )
		{
		}
		ITEM_INFO(
			eItem_t eItemType,
			CExtSafeString sText,
			INT nValue,
			CRect rcRect,
			bool bVisible,
			bool bBefore = true
			)
			: m_eItemType( eItemType )
			, m_sText( sText )
			, m_nValue( nValue )
			, m_rcRect( rcRect )
			, m_bVisible( bVisible )
			, m_bBefore( bBefore )
		{ 
		}
		void AddDependentItem( ITEM_INFO * pII )
		{
			ASSERT( pII != NULL );
			if( pII != NULL )
				m_arrDependentItems.Add( pII );
		}
 		eItem_t m_eItemType;
		CExtSafeString m_sText;
		INT m_nValue;
		CRect m_rcRect;
		bool m_bVisible:1;
		bool m_bBefore:1; // used only for the label items
		ItemsArr_t m_arrDependentItems;
	}; // struct ITEM_INFO

	class __PROF_UIS_API CHANGING_NOTIFICATION
	{
	public:
		CHANGING_NOTIFICATION(
			bool bChangedFinally,
			const COleDateTimeSpan & dtSpanOld,
			const COleDateTimeSpan & dtSpanNew,
			LPARAM lParamCookie = NULL
			);
		bool m_bChangedFinally;
		LPARAM m_lParamCookie;
		COleDateTimeSpan m_dtSpanOld;
		COleDateTimeSpan m_dtSpanNew;
		operator WPARAM() const;
		static const CHANGING_NOTIFICATION * FromWPARAM( WPARAM wParam );
		friend class CExtDurationWnd;
	private:
		LRESULT Notify( HWND hWndNotify ) const;
	}; // class CHANGING_NOTIFICATION

// Operations
public:
	LPARAM m_lParamCookie;
	static const UINT g_nMsgChangingNotification;
	HWND m_hWndNotificationReceiver;

	static HCURSOR g_hCursor;
	static bool RegisterWndClass();

	virtual bool Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( __EXT_MFC_IDC_STATIC ),
		DWORD dwWindowStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CCreateContext * pContext = NULL
		);
	
	void UpdateDurationWnd(
		bool bRecalcLayout = true,
		bool bUpdate = true
		);

	void ShowSpinButton( 
		bool bShow = true 
		);
	virtual bool IsSpinButtonVisible() const;

	void ShowDropDownButton( 
		bool bShow = true 
		);
	virtual bool IsDropDownButtonVisible() const;
	virtual ITEM_INFO * OnInitializeItemYear(
		__EXT_MFC_SAFE_LPCTSTR lpszTextBefore = NULL, 
		__EXT_MFC_SAFE_LPCTSTR lpszTextAfter = NULL 
		);
	virtual ITEM_INFO * OnInitializeItemMonth(
		__EXT_MFC_SAFE_LPCTSTR lpszTextBefore = NULL, 
		__EXT_MFC_SAFE_LPCTSTR lpszTextAfter = NULL 
		);
	virtual ITEM_INFO * OnInitializeItemDay(
		__EXT_MFC_SAFE_LPCTSTR lpszTextBefore = NULL, 
		__EXT_MFC_SAFE_LPCTSTR lpszTextAfter = NULL 
		);
	virtual ITEM_INFO * OnInitializeItemHour(
		__EXT_MFC_SAFE_LPCTSTR lpszTextBefore = NULL, 
		__EXT_MFC_SAFE_LPCTSTR lpszTextAfter = NULL 
		);
	virtual ITEM_INFO * OnInitializeItemMinute(
		__EXT_MFC_SAFE_LPCTSTR lpszTextBefore = NULL, 
		__EXT_MFC_SAFE_LPCTSTR lpszTextAfter = NULL 
		);
	virtual ITEM_INFO * OnInitializeItemSecond(
		__EXT_MFC_SAFE_LPCTSTR lpszTextBefore = NULL, 
		__EXT_MFC_SAFE_LPCTSTR lpszTextAfter = NULL 
		);
	virtual ITEM_INFO * OnInitializeItemDesignator(
		__EXT_MFC_SAFE_LPCTSTR lpszTextBefore = NULL, 
		__EXT_MFC_SAFE_LPCTSTR lpszTextAfter = NULL 
		);

	void SetContinuousScrolling( 
		bool bSet = true 
		);
	bool GetContinuousScrolling() const;

	void SetDuration( 
		const COleDateTimeSpan & dtSpan 
		);
	void SetDuration( 
		LONG nTotalSeconds 
		);
	void SetDuration( 
		double dSpan 
		);
	void SetDuration( 
		LONG nDays, 
		INT nHours, 
		INT nMins, 
		INT nSecs 
		);

	COleDateTimeSpan GetDuration() const;

	bool SelectItem( 
		eItem_t eItem 
		);
	ITEM_INFO * SelectionGet() const;

	bool SelectNextItem( 
		const ITEM_INFO * pII 
		);
	bool SelectPrevItem( 
		const ITEM_INFO * pII 
		);

	bool SelectFirsItem();
	bool SelectLastItem();

	bool IsFirstVisibleItem( 
		const ITEM_INFO * pII 
		) const;
	bool IsLastVisibleItem( 
		const ITEM_INFO * pII 
		) const;

	virtual DWORD StyleGet() const;
	virtual DWORD ModifyStyle(
		DWORD dwStyleAdd,
		DWORD dwStyleRemove = 0L,
		bool bRedraw = true
		);

	void AdjustSize();

	ITEM_INFO * ItemGet( 
		eItem_t eItem 
		) const;

	bool IsItemVisible( 
		eItem_t eItem 
		) const;
	void SetShowItem( 
		eItem_t eItem,
		bool bShow = true,
		bool bUpdate = true 
		);

	ITEM_INFO * HitTestItem( 
		const POINT & ptClient 
		) const;

	virtual void ScrollCurrentItem( 
		INT nDelta 
		);

	void SetBkColor( 
		COLORREF clrBk = COLORREF(-1L) 
		);
	COLORREF GetBkColor() const;

	void SetTextColor( 
		COLORREF clrText = COLORREF(-1L) 
		);
	COLORREF GetTextColor() const;

	void SetAlign( eAlign_t eAlign );
	eAlign_t GetAlign() const;

	virtual void SetStatus( 
		eStatus_t eStatus 
		);
	virtual eStatus_t GetStatus() const;

	void SetReadOnly( 
		bool bReadOnly = true 
		);
	bool IsReadOnly() const;

	bool NoHideSelectionGet() const;
	bool NoHideSelectionSet(
		bool bNoHideSelection,
		bool bRedraw = true
		);

	void SetMaxDuration( LONG nMaxDuration );
	LONG GetMaxDuration() const;

protected:
	static bool g_bWndClassRegistered;
	bool m_bDirectCreateCall:1;
	bool m_bInitialized:1;
	bool m_bUpdatingLayout:1;
	bool m_bMouseOver:1;
	bool m_bCanceling:1;

	bool m_bSpinButtonVisible:1;
	bool m_bDropDownButtonVisible:1;
	bool m_bContinuousScrolling:1;
	bool m_bReadOnly:1;

	DWORD m_dwStyle;

	enum e_MouseTrackType_t
	{
		__EMTT_NOTHING						= 0,
		__EMTT_CLIENT						= 1,
		__EMTT_BUTTON_DROPDOWN_PRESSED		= 2,
		__EMTT_BUTTON_DROPDOWN_HOVERED		= 3,
		__EMTT_BUTTON_SCROLL_UP_PRESSED		= 4,
		__EMTT_BUTTON_SCROLL_UP_HOVERED		= 5,
		__EMTT_BUTTON_SCROLL_DOWN_PRESSED	= 6,
		__EMTT_BUTTON_SCROLL_DOWN_HOVERED	= 7,
	};
	e_MouseTrackType_t m_eMTT;

	enum e_ScrollingType_t
	{
		__EST_NONE	= 0,
		__EST_UP	= 1,
		__EST_DOWN	= 2,
	};
	e_ScrollingType_t m_eST;

	CRect m_rcDropDown;
	CRect m_rcScrollUp;
	CRect m_rcScrollDown;
	CRect m_rcButtons;

	ITEM_INFO * m_pSelectedItem;
	ITEM_INFO * m_pLastInputItem;
	COleDateTimeSpan m_dtSpan;
	LONG m_nMaxDuration;

	ItemsArr_t m_arrItems;

	COLORREF m_clrBackground; 
	COLORREF m_clrText; 

	eStatus_t m_eStatus;
	eAlign_t m_eAlign;

	virtual bool _CreateHelper();
	virtual void _RecalcLayout();
	virtual void _RecalcDuration();
	virtual void _CancelActions();

	LONG _ItemGetIndexOf(
		const ITEM_INFO * pII
		) const;

	bool _ProcessMouseClick(
		CPoint point,
		bool bButtonPressed,
		INT nMouseButton, // MK_... values
		UINT nMouseEventFlags
		);

	bool _ProcessMouseMove(
		CPoint point,
		UINT nMouseEventFlags
		);
	
	virtual LONG HitTest( 
		const POINT & ptClient 
		) const;

	virtual void OnInitializeItemsArray();

	virtual void OnItemSyncVisibility( ITEM_INFO * pII );

	virtual void OnDurationDrawEntire(
		CDC & dc,
		const CRect & rcClient
		);

	virtual void OnDurationEraseClientArea(
		CDC & dc,
		const CRect & rcClient
		) const;

	virtual void OnDurationDrawBorder(
		CDC & dc,
		const CRect & rcClient
		) const;

	virtual CSize OnQueryBorderSize() const;
	virtual INT OnQuerySpinButtonWidth() const;
	virtual INT OnQueryDropDownButtonWidth() const;

	virtual CRect OnQueryItemMargins( const ITEM_INFO * pII ) const;
	virtual CExtSafeString OnQueryItemText( const ITEM_INFO * pII ) const;

	virtual CExtSafeString OnQueryTimeSeparator() const;
	virtual CExtSafeString OnQueryDateSeparator() const;
	virtual CExtSafeString OnQueryHourLeadingZeros() const;
	virtual CExtSafeString OnQueryDayLeadingZeros() const;
	virtual CExtSafeString OnQueryMonthLeadingZeros() const;
	virtual CExtSafeString OnQueryYearFormat() const;
	virtual CExtSafeString OnQueryTimeDesignatorAM() const;
	virtual CExtSafeString OnQueryTimeDesignatorPM() const;
	virtual CExtSafeString OnQueryTimeDesignatorPosition() const;
	
	virtual COLORREF OnQueryItemBkColor( const ITEM_INFO * pII ) const;
	virtual COLORREF OnQueryItemTextColor( const ITEM_INFO * pII ) const;

	virtual HFONT OnQueryFont() const;

	virtual bool OnQueryFocusedState() const;

	virtual void OnSelectItem( ITEM_INFO * pII );
	virtual void OnItemSelectionChanged();
	virtual void OnItemFinishInput();

	virtual void OnAutoSelectNextItem( 
		const ITEM_INFO * pII 
		);

	virtual CSize OnCalcContentSize() const;

	virtual bool OnValueChanging( 
		const COleDateTimeSpan & dtSpanOld,
		const COleDateTimeSpan & dtSpanNew
		);
	virtual bool OnValueChanged( 
		const COleDateTimeSpan & dtSpanOld,
		const COleDateTimeSpan & dtSpanNew
		);
	
	virtual void OnDigitPressed(
		UINT nDigit
		);

	LONG OnQueryMaxDuration() const;

	virtual bool OnShowDropDownMenu();
	
	virtual HWND OnQueryNotificationReceiver() const;

public:
	virtual void PmBridge_OnPaintManagerChanged(
		CExtPaintManager * pGlobalPM
		);
	virtual HWND AnimationSite_GetSafeHWND() const;
	virtual const CExtAnimationParameters *
		AnimationClient_OnQueryAnimationParameters(
			INT eAPT // __EAPT_*** animation type
			) const;
	virtual bool AnimationClient_CacheNextState(
		CDC & dc,
		const RECT & rcAcAnimationTarget,
		bool bAnimate,
		INT eAPT // __EAPT_*** animation type
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDurationWnd)
	public:
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtDurationWnd();
	bool m_bAutoDeleteWindow:1;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
// Message maps
	//{{AFX_MSG(CExtDurationWnd)
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	afx_msg LRESULT OnSetFont( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetFont( WPARAM, LPARAM );
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	//}}AFX_MSG
	
	afx_msg LRESULT OnMenuClosed( WPARAM wParam, LPARAM lParam );
#if _MFC_VER < 0x700
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#else
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
#endif
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThemeChanged( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

}; // class CExtDurationWnd

__PROF_UIS_API void AFXAPI __EXT_MFC_DDX_DurationWnd( CDataExchange * pDX, INT nIDC, COleDateTimeSpan & value );

#if (!defined __EXT_MFC_NO_DATETIMEWND)

/////////////////////////////////////////////////////////////////////////////
// CExtDateTimeWnd window

class __PROF_UIS_API CExtDateTimeWnd : public CExtDurationWnd
{
// Construction
public:
	DECLARE_DYNCREATE( CExtDateTimeWnd );
	CExtDateTimeWnd();
 
// Attributes
public:
 
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

	class __PROF_UIS_API CHANGING_NOTIFICATION
	{
	public:
		CHANGING_NOTIFICATION(
			bool bChangedFinally,
			COleDateTime dtDateTimeOld,
			COleDateTime dtDateTimeNew,
			LPARAM lParamCookie = NULL
			);
		bool m_bChangedFinally;
		LPARAM m_lParamCookie;
		COleDateTime m_dtDateTimeOld;
		COleDateTime m_dtDateTimeNew;
		operator WPARAM() const;
		static const CHANGING_NOTIFICATION * FromWPARAM( WPARAM wParam );
		friend class CExtDateTimeWnd;
	private:
		LRESULT Notify( HWND hWndNotify ) const;
	}; // class CHANGING_NOTIFICATION

// Operations
public:
	static const UINT g_nMsgChangingNotification;

	static INT stat_GetDaysInMonth( INT nYear, INT nMonth );

	void SetMode( eMode_t eMode );
	eMode_t GetMode() const;

	void SetTimeFormat( eTimeFormat_t eTimeFormat );
	eTimeFormat_t GetTimeFormat() const;
	
	void SetRange(
		const COleDateTime * pdtRangeMin,
		const COleDateTime * pdtRangeMax 
		);
	void SetRange(
		const COleDateTime & dtRangeMin,
		const COleDateTime & dtRangeMax 
		);
	void SetRange(
		const CTime * pdtRangeMin,
		const CTime * pdtRangeMax 
		);
	void SetRange(
		const CTime & dtRangeMin,
		const CTime & dtRangeMax 
		);

	void GetRange(
		COleDateTime * pdtRangeMin,
		COleDateTime * pdtRangeMax 
		) const;
	void GetRange(
		CTime * pdtRangeMin,
		CTime * pdtRangeMax 
		) const;
	
	void SetDateTime( 
		const COleDateTime & dateSrc, 
		bool bResetBlankMode = true,
		bool bUpdate = true 
		);
	void SetDateTime( 
		const VARIANT & varSrc, 
		bool bResetBlankMode = true,
		bool bUpdate = true 
		);
	void SetDateTime( 
		const DATE & dtSrc, 
		bool bResetBlankMode = true,
		bool bUpdate = true 
		);
	void SetDateTime( 
		time_t timeSrc, 
		bool bResetBlankMode = true,
		bool bUpdate = true 
		);
	void SetDateTime( 
		const SYSTEMTIME & systimeSrc, 
		bool bResetBlankMode = true,
		bool bUpdate = true 
		);
	void SetDateTime( 
		const FILETIME & filetimeSrc, 
		bool bResetBlankMode = true,
		bool bUpdate = true 
		);
	INT SetDateTime(
		INT nYear, INT nMonth, INT nDay,
		INT nHour, INT nMin, INT nSec, 
		bool bResetBlankMode = true,
		bool bUpdate = true
		);
	INT SetDate( 
		INT nYear, INT nMonth, INT nDay, 
		bool bResetBlankMode = true,
		bool bUpdate = true 
		);
	INT SetTime( 
		INT nHour, INT nMin, INT nSec, 
		bool bResetBlankMode = true,
		bool bUpdate = true 
		);

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

	const CExtDateTimeWnd & operator = ( const COleDateTime & dateSrc );
	const CExtDateTimeWnd & operator = ( const VARIANT & varSrc );
	const CExtDateTimeWnd & operator = ( DATE dtSrc );

	const CExtDateTimeWnd & operator = ( const time_t & timeSrc );
	const CExtDateTimeWnd & operator = ( const SYSTEMTIME & systimeSrc );
	const CExtDateTimeWnd & operator = ( const FILETIME & filetimeSrc );

	BOOL operator == ( const CExtDateTimeWnd & other ) const;
	BOOL operator != ( const CExtDateTimeWnd & other ) const;
	BOOL operator <  ( const CExtDateTimeWnd & other ) const;
	BOOL operator >  ( const CExtDateTimeWnd & other ) const;
	BOOL operator <= ( const CExtDateTimeWnd & other ) const;
	BOOL operator >= ( const CExtDateTimeWnd & other ) const;

	virtual DWORD OnDropDownCalendarQueryStyle() const;
	DWORD GetDropDownCalendarStyle() const;
	DWORD ModifyDropDownCalendarStyle(
		DWORD dwRemove,
		DWORD dwAdd = 0
		);

	virtual bool IsDropDownButtonVisible() const;
	virtual ITEM_INFO * OnInitializeItemYear( 
		__EXT_MFC_SAFE_LPCTSTR lpszTextBefore = NULL, 
		__EXT_MFC_SAFE_LPCTSTR lpszTextAfter = NULL 
		);
	virtual ITEM_INFO * OnInitializeItemMonth(
		__EXT_MFC_SAFE_LPCTSTR lpszTextBefore = NULL, 
		__EXT_MFC_SAFE_LPCTSTR lpszTextAfter = NULL 
		);
	virtual ITEM_INFO * OnInitializeItemDesignator(
		__EXT_MFC_SAFE_LPCTSTR lpszTextBefore = NULL, 
		__EXT_MFC_SAFE_LPCTSTR lpszTextAfter = NULL 
		);

	virtual void ScrollCurrentItem( 
		INT nDelta 
		);

	virtual void SetStatus( 
		eStatus_t eStatus 
		);
	virtual eStatus_t GetStatus() const;

	void SetBlank(
		bool bUpdate = true
		);

	void SetAutoSelectNext( 
		bool bSet = true 
		);
	bool GetAutoSelectNext() const;

	bool ShowDropDown();

protected:
	eMode_t m_eMode;		
	eTimeFormat_t m_eTimeFormat;		
	COleDateTime m_dtDate;
	COleDateTime m_dtRangeMin, m_dtRangeMax;

	bool m_bBlankYear:1;
	bool m_bBlankMonth:1;
	bool m_bBlankDay:1;
	bool m_bBlankHour:1;
	bool m_bBlankMinute:1;
	bool m_bBlankSecond:1;
	bool m_bBlankDesignator:1;

	bool m_bAutoSelectNext:1;

	DWORD m_dwDatePickerStyle;

	virtual bool _CreateHelper();
	virtual void _RecalcDuration();
	virtual BOOL _UpdateDurationFromOleDateTime( 
		bool bUpdate = false 
		);

	virtual void OnInitializeItemsArray();
	virtual CExtSafeString OnQueryItemText( 
		const ITEM_INFO * pII 
		) const;

	virtual void OnDigitPressed(
		UINT nDigit
		);
	virtual bool OnShowDropDownMenu();

	virtual bool OnQueryTimeFormat24Hours() const;

	virtual void OnItemSelectionChanged();
	virtual void OnItemFinishInput();

	virtual bool OnValueChanging(
		const COleDateTime & dtDateTimeOld,
		const COleDateTime & dtDateTimeNew
		) const;
	virtual bool OnValueChanged(
		const COleDateTime & dtDateTimeOld,
		const COleDateTime & dtDateTimeNew
		) const;

	virtual void OnQueryRange(
		COleDateTime * pdtRangeMin,
		COleDateTime * pdtRangeMax 
		) const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDateTimeWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtDateTimeWnd();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
// Message maps
	//{{AFX_MSG(CExtDateTimeWnd)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	LRESULT OnDropDownCalendarSelChanged( WPARAM wParam, LPARAM lParam );
	LRESULT OnDropDownCalendarInit( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

}; // class CExtDateTimeWnd

__PROF_UIS_API void AFXAPI __EXT_MFC_DDX_DateTimeWnd( CDataExchange * pDX, INT nIDC, CTime & value );
__PROF_UIS_API void AFXAPI __EXT_MFC_DDX_DateTimeWnd( CDataExchange * pDX, INT nIDC, COleDateTime & value );

#endif // (!defined __EXT_MFC_NO_DATETIMEWND)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // (!defined __EXT_MFC_NO_DURATIONWND)

#endif // (!defined __EXT_DURATIONWND_H)
