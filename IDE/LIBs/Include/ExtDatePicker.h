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

#if (!defined __EXT_DATE_PICKER_H)
#define __EXT_DATE_PICKER_H

#if (!defined __EXT_MFC_NO_DATE_PICKER) || (!defined __EXT_MFC_NO_DATE_BROWSER)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

#if (!defined __EXT_CONTROLBAR_H)
	#include <ExtControlBar.h>
#endif

// #if _MSC_VER > 1000
// 	#pragma once
// #endif // _MSC_VER > 1000

#if (!defined __EXT_MFC_NO_DATE_PICKER)

#define __EXT_DATE_PICKER_CLASS_NAME		_T("ProfUIS-DatePickerWnd")

// general hit-testing values
#define __EDPWH_NOWHERE						(0L)
#define __EDPWH_BTN_NONE					(1L)
#define __EDPWH_BTN_TODAY					(2L)
#define __EDPWH_INNER_AREA					(3L)
// month-picker button values
#define __EDPWH_BTN_BACKWARD				(4L)
#define __EDPWH_BTN_FORWARD					(5L)
// month-picker date values
#define __EDPWH_HEADER_DATE					(6L)
#define __EDPWH_DAYS_OF_WEEK				(7L)
#define __EDPWH_DAYS_AREA					(8L)

// scrolling via buttons
#define __EDPW_SCROLL_TIMER_ID				1
#define __EDPW_SCROLL_TIMER_PERIOD			150 

// scrolling via popup month list
#define __EDPW_SCROLL_MONTH_LIST_ACCELERATION_STEP  10  // pixels
#define __EDPW_SCROLL_MONTH_LIST_UP_TIMER_ID		2
#define __EDPW_SCROLL_MONTH_LIST_DOWN_TIMER_ID		3

#define __EDPW_SCROLL_MONTH_LIST_TIMER_ELAPSE1		500 // milliseconds
#define __EDPW_SCROLL_MONTH_LIST_TIMER_ELAPSE2		300 
#define __EDPW_SCROLL_MONTH_LIST_TIMER_ELAPSE3		200 
#define __EDPW_SCROLL_MONTH_LIST_TIMER_ELAPSE4		100  
#define __EDPW_SCROLL_MONTH_LIST_TIMER_ELAPSE5		50  

// date picker styles
#define __EDPWS_BORDER_RESERVE				0x00000001L
#define __EDPWS_BORDER_SHOW					0x00000002L
#define __EDPWS_BORDER						(__EDPWS_BORDER_RESERVE|__EDPWS_BORDER_SHOW)
#define __EDPWS_BUTTON_TODAY				0x00000004L
#define __EDPWS_BUTTON_NONE					0x00000008L
#define __EDPWS_BUTTON_ANY					(__EDPWS_BUTTON_TODAY|__EDPWS_BUTTON_NONE)
#define __EDPWS_BUTTON_SEPARATOR				0x00000010L
#define __EDPWS_BOLD_HEADER					0x00000020L
#define __EDPWS_BOLD_HEADER_POPUP			0x00000040L
#define __EDPWS_BOLD_WEEK_DAYS				0x00000080L
#define __EDPWS_BOLD_DAY_CELLS				0x00000100L
#define __EDPWS_BOLD_ANY					(__EDPWS_BOLD_HEADER|__EDPWS_BOLD_HEADER_POPUP|__EDPWS_BOLD_WEEK_DAYS|__EDPWS_BOLD_DAY_CELLS)
#define __EDPWS_SHOW_NON_MONTH_DAYS			0x00000200L
#define __EDPWS_HIDE_INNER_NON_MONTH_DAYS	0x00000400L
#define __EDPWS_SHOW_TODAY_HIGHLIGHT		0x00000800L
#define __EDPWS_SHOW_MONTH_LIST_SHADOW		0x00001000L
#define __EDPWS_MULTIPLE_SELECTION			0x00002000L
#define __EDPWS_AUTO_DIMENSION_MODE			0x00004000L
#define __EDPWS_NO_HEADER_BACKGROUND		0x00008000L
#define __EDPWS_CENTER_HORIZONTALLY			0x00010000L
#define __EDPWS_CENTER_VERTICALLY			0x00020000L
#define __EDPWS_USER_STYLE_13				0x00040000L
#define __EDPWS_USER_STYLE_12				0x00080000L
#define __EDPWS_USER_STYLE_11				0x00100000L
#define __EDPWS_USER_STYLE_10				0x00200000L
#define __EDPWS_USER_STYLE_09				0x00400000L
#define __EDPWS_USER_STYLE_08				0x00800000L
#define __EDPWS_USER_STYLE_07				0x01000000L
#define __EDPWS_USER_STYLE_06				0x02000000L
#define __EDPWS_USER_STYLE_05				0x04000000L
#define __EDPWS_USER_STYLE_04				0x08000000L
#define __EDPWS_USER_STYLE_03				0x10000000L
#define __EDPWS_USER_STYLE_02				0x20000000L
#define __EDPWS_USER_STYLE_01				0x40000000L
#define __EDPWS_USER_STYLE_00				0x80000000L

// default set of styles
#define __EDPWS_DEFAULT \
	( __EDPWS_BORDER \
	| __EDPWS_SHOW_NON_MONTH_DAYS \
	| __EDPWS_HIDE_INNER_NON_MONTH_DAYS \
	| __EDPWS_SHOW_TODAY_HIGHLIGHT \
	| __EDPWS_SHOW_MONTH_LIST_SHADOW \
	| __EDPWS_BUTTON_TODAY \
	| __EDPWS_BUTTON_SEPARATOR \
	)


// The COleDateTime class handles dates from 1 January 100 to 31 December 9999. 
#define __EXT_DATE_YEAR_MIN	100
#define __EXT_DATE_YEAR_MAX	9999

//////////////////////////////////////////////////////////////////////////
// class CExtDatePickerHeaderPopupWnd
//////////////////////////////////////////////////////////////////////////

class CExtDatePickerWnd;

class __PROF_UIS_API CExtDatePickerHeaderPopupWnd : public CWnd
{
protected:
	CSize m_sizeDesiredSize, m_sizeItemSize;
	INT	m_nMonth, m_nYear, m_nSelMonth, m_nSelYear, m_nItemsPerPage;
	CExtDatePickerWnd * m_pDPW;
	CExtWndShadow m_wndShadow;
	CExtShadowCtrl m_ctrlShadow;
	bool m_bScrollingUp:1, m_bScrollingDown:1, m_bUseShadow:1;
	UINT m_nLastElapseTimerUp, m_nLastElapseTimerDown;

public:
	DECLARE_DYNAMIC( CExtDatePickerHeaderPopupWnd );
	CExtDatePickerHeaderPopupWnd(
		CExtDatePickerWnd * pDPW,
		bool bUseShadow
		);
	virtual ~CExtDatePickerHeaderPopupWnd();
	void SetInitialDate( INT nMonth, INT nYear );
	bool GetSelectedDate( INT & nMonth, INT & nYear ) const;
	void SetItemsPerPage( INT nItemsPerPage );
	CSize GetSize(
		bool bOnlyClientArea = false
		) const;
	UINT GetScrollingSpeed( int nPixels );
	void AnalyzeChangings();

protected:
	void _CalcSize();
	INT _HitTest( CPoint pt );

	//{{AFX_VIRTUAL(CExtDatePickerHeaderPopupWnd)
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExtDatePickerHeaderPopupWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	
}; /// class CExtDatePickerHeaderPopupWnd
	
//////////////////////////////////////////////////////////////////////////
// class CExtDatePickerWnd
//////////////////////////////////////////////////////////////////////////

class __PROF_UIS_API CExtDatePickerWnd
	: public CWnd
	, public CExtAnimationSingleton
{
public:
	class __PROF_UIS_API CExtOleDateTime : public COleDateTime
	{
	public:
		CExtOleDateTime() 
			: COleDateTime()
		{
		}
		CExtOleDateTime( const COleDateTime & dateSrc )
			: COleDateTime( dateSrc )
		{
		}
		CExtOleDateTime( const VARIANT & varSrc )
			: COleDateTime( varSrc )
		{
		}
		CExtOleDateTime( DATE dtSrc )
			: COleDateTime( dtSrc )
		{
		}
		CExtOleDateTime( time_t timeSrc )
			: COleDateTime( timeSrc )
		{
		}
		CExtOleDateTime( const SYSTEMTIME & systimeSrc )
			: COleDateTime( systimeSrc )
		{
		}
		CExtOleDateTime( const FILETIME & filetimeSrc )
			: COleDateTime( filetimeSrc )
		{
		}
		CExtOleDateTime( int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec )
			: COleDateTime( nYear, nMonth, nDay, nHour, nMin, nSec )
		{
		}
		CExtOleDateTime( WORD wDosDate, WORD wDosTime )
			: COleDateTime( wDosDate, wDosTime )
		{
		}
		
	public:
		inline DateTimeStatus GetStatus() const
		{
			DateTimeStatus status = COleDateTime::GetStatus();
			if(		status == COleDateTime::valid 
				&&	(	COleDateTime::GetYear() <= 0
					||	COleDateTime::GetMonth() <= 0
					||	COleDateTime::GetDay() <= 0
					||	COleDateTime::GetHour() < 0
					||	COleDateTime::GetMinute() < 0
					||	COleDateTime::GetSecond() < 0
					)
				)
				status = COleDateTime::invalid;
			return status;
		}
	};

protected:
	class __PROF_UIS_API MONTH_INFO : public CObject
	{
	public:
		class __PROF_UIS_API DATE_INFO : public CObject
		{
		protected:
			CRect m_rcCell;
			CExtOleDateTime m_dt;
			CExtDatePickerWnd * m_pDPW;
			MONTH_INFO * m_pMI;
		public:
			DATE_INFO(
				CExtDatePickerWnd * pDPW = NULL
				);
			~DATE_INFO();
#ifdef _DEBUG
			void AssertValid() const;
			void Dump( CDumpContext & dc ) const;
#endif //_DEBUG
			void Clear();
			void SetDatePickerWnd( 
				CExtDatePickerWnd * pDPW,
				MONTH_INFO * pMI
				);
			void SetRect( const RECT & rcCell );
			CRect GetRect() const;
			void SetDate( const COleDateTime & dt );
			COleDateTime GetDate() const;
			void Draw( 
				CDC & dc,
				bool bShowNonMonthDays = true,
				bool bShowTodayHighlight = true
				) const;
		}; // class DATE_INFO
	
	protected:
		DATE_INFO m_aDays[6][7];
		INT m_nMonth;
		INT m_nYear;
		CRect	m_rcScrollBtnBackward, m_rcScrollBtnForward,
				m_rcHeaderDate, m_rcHeader, m_rcHeaderWithoutBorders,
				m_rcDaysCells, m_rcDaysOfWeek;
		CExtDatePickerWnd * m_pDPW;
	public:
		INT m_nRow, m_nCol;
		CRect m_rcClient;
		MONTH_INFO( 
			INT nRow, 
			INT nCol, 
			INT nMonth, 
			INT nYear, 
			CRect rcClient,
			CExtDatePickerWnd * pDPW
			);
#ifdef _DEBUG
		void AssertValid() const;
		void Dump( CDumpContext & dc) const;
#endif // _DEBUG

	protected:
		void _RecalcLayout();
	public:
		CRect GetRect() const;
		CRect GetHeaderRect() const;
		void GetMonth( INT & nMonth, INT & nYear ) const;
		COleDateTime GetMonthInfoDT() const;
		const DATE_INFO * HitTestDay( const POINT & ptClient ) const;
		LONG HitTest(
			const POINT & ptClient,
			COleDateTime * pDT = NULL
			) const;
		void Draw( CDC & dc ) const;
	}; // class MONTH_INFO 
	
protected:
	enum e_MouseTrackType_t
	{
		__EMTT_NOTHING				= 0,
		__EMTT_SCROLLING_BACKWARD	= 1,
		__EMTT_SCROLLING_FORWARD	= 2,
		__EMTT_BTN_NONE_PRESSED		= 3,
		__EMTT_BTN_NONE_HOVERED		= 4,
		__EMTT_BTN_TODAY_PRESSED	= 5,
		__EMTT_BTN_TODAY_HOVERED	= 6,
		__EMTT_DAYS_SELECTING		= 7
	};
	e_MouseTrackType_t m_eMTT;
	DWORD m_dwDatePickerStyle;
	CSize	m_sizeCalendarDimensionsMin,
			m_sizeCalendarDimensionsCurrent,
			m_sizeCalendarDimensionsMax;
	CRect	m_rcDatePickerClient, m_rcDatePickerInnerArea,
			m_rcBtnNone, m_rcBtnToday;
	CTypedPtrArray < CPtrArray, MONTH_INFO * > m_arrMonths;

	CExtOleDateTime m_dtCurrentDate; // currently selected date
	CExtOleDateTime m_dtSelBegin;
	CExtOleDateTime m_dtSelEnd;
	CExtOleDateTime m_dtDisplayedBegin;
	CExtOleDateTime m_dtDisplayedEnd;
	
	static bool g_bDatePickerWndClassRegistered;
	bool	m_bDirectCreateCall:1, m_bCanceling:1, m_bUpdatingLayout:1;
	CSize	m_sizeMonthPicker, m_sizeDateCell,
			m_sizePushButton, m_sizeScrollButton;
	INT		m_nMonthHeaderHeight, m_nButtonAreaHeight,
			m_nDaysOfWeekHeight, m_nIndentSpace;
	
	virtual bool _CreateHelper();
	virtual void _RecalcLayout();
	virtual void _RecalcCalendar();
	virtual void _CancelActions();

	MONTH_INFO * _HitTestMonth( 
		const POINT & ptClient 
		) const;
	const MONTH_INFO * _FindMonth( INT nRow, INT nCol ) const;

public:
	DECLARE_DYNCREATE( CExtDatePickerWnd );
	CExtDatePickerWnd();
	virtual ~CExtDatePickerWnd();

	struct __PROF_UIS_API SELECTION_INFO
	{
		CExtOleDateTime m_dtBegin, m_dtEnd;
		SELECTION_INFO(
			const COleDateTime & dt
			)
			: m_dtBegin( dt )
			, m_dtEnd( dt )
		{
		}
		SELECTION_INFO(
			const COleDateTime & dtBegin,
			const COleDateTime & dtEnd
			)
			: m_dtBegin( dtBegin )
			, m_dtEnd( dtEnd )
		{
		}
		SELECTION_INFO(
			const SELECTION_INFO * pOther = NULL
			)
		{
			if( pOther != NULL )
			{
				m_dtBegin = pOther->m_dtBegin;
				m_dtEnd = pOther->m_dtEnd;
			} // if( pOther != NULL )
		}
		SELECTION_INFO(
			const SELECTION_INFO & other
			)
			: m_dtBegin( other.m_dtBegin )
			, m_dtEnd( other.m_dtEnd )
		{
		}
		SELECTION_INFO & operator = (
			const SELECTION_INFO & other
			)
		{
			m_dtBegin = other.m_dtBegin;
			m_dtEnd = other.m_dtEnd;
			return (*this);
		}
		SELECTION_INFO & operator = (
			const COleDateTime & dt
			)
		{
			m_dtBegin = dt;
			m_dtEnd = dt;
			return (*this);
		}
		bool operator == (
			const SELECTION_INFO & other
			) const
		{
			if(		m_dtBegin == other.m_dtBegin
				&&	m_dtEnd == other.m_dtEnd
				)
				return true;
			if(		m_dtBegin == other.m_dtEnd
				&&	m_dtEnd == other.m_dtBegin
				)
				return true;
			return false;
		}
		bool operator == (
			const COleDateTime & dt
			) const
		{
			if(		m_dtBegin == dt
				&&	m_dtEnd == dt
				)
				return true;
			return false;
		}
		bool IsEmptySelection() const
		{
			if(		m_dtBegin.GetStatus() != COleDateTime::valid
				||	m_dtEnd.GetStatus() != COleDateTime::valid
				)
				return true;
			return false;
		}
	}; // struct SELECTION_INFO

	struct __PROF_UIS_API SELECTION_NOTIFICATION : public SELECTION_INFO
	{
		HWND m_hWndDatePicker;
		LPARAM m_lParamCookie;
		bool m_bFinalSelectionChanging:1,
			m_bAllowFinalParentCancelMode:1;
		SELECTION_INFO * m_pExternalSelectionInfo;
		SELECTION_NOTIFICATION(
			SELECTION_INFO * pExternalSelectionInfo,
			HWND hWndDatePicker,
			LPARAM lParamCookie,
			bool bFinalSelectionChanging,
			const COleDateTime & dtBegin,
			const COleDateTime & dtEnd,
			bool bAllowFinalParentCancelMode = true
			);
		SELECTION_NOTIFICATION(
			SELECTION_INFO * pExternalSelectionInfo,
			const CExtDatePickerWnd & wndDatePicker,
			bool bFinalSelectionChanging,
			bool bAllowFinalParentCancelMode = true
			);
		operator WPARAM() const;
		static const SELECTION_NOTIFICATION * FromWPARAM( WPARAM wParam );
		friend class CExtDatePickerWnd;
	private:
		void Notify( HWND hWndNotify ) const;
	}; // struct SELECTION_NOTIFICATION

// Attributes
public:
	LPARAM m_lParamCookie;
	static const UINT g_nMsgSelectionNotification;
	HWND m_hWndNotificationReceiver;
	SELECTION_INFO * m_pExternalSelectionInfo;

protected:
	HFONT m_hFont;

// Operations
public:
	static bool RegisterDatePickerWndClass();
	virtual bool Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( __EXT_MFC_IDC_STATIC ),
		CSize sizeCalendarDimensionsMin = CSize(1,1),
		CSize sizeCalendarDimensionsMax = CSize(1,1),
		DWORD dwWindowStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,
		DWORD dwDatePickerStyle = __EDPWS_DEFAULT,
		CCreateContext * pContext = NULL
		);
	void AdjustSize( const SIZE & sizeDim );
	void AdjustSize( INT nDimCols, INT nDimRows );
	virtual DWORD GetDatePickerStyle() const;
	virtual DWORD ModifyDatePickerStyle(
		DWORD dwRemove,
		DWORD dwAdd = 0,
		bool bUpdateDatePickerWnd = false
		);
	virtual void UpdateDatePickerWnd(
		bool bRecalcLayout = true,
		bool bRecalcCalendar = true,
		bool bUpdate = true
		);
	virtual bool SelectionGetForDate( 
		const COleDateTime & dt 
		) const;
	virtual bool EnsureVisible(
		const COleDateTime & dt,
		bool bUpdateNow = false
		);
	void SelectionClear( 
		bool bUpdateNow = false
		);
	virtual void SelectionSet(
		const COleDateTime & dtSelBegin, 
		const COleDateTime & dtSelEnd, 
		bool bNormalize = false,
		bool bUpdateNow = false
		);
	virtual void SelectionGet(
		COleDateTime & dtSelBegin, 
		COleDateTime & dtSelEnd
		) const;
	virtual void GetDisplayedRange(
		COleDateTime & dtBegin, 
		COleDateTime & dtEnd
		) const;
	virtual void CurrentDateSet( 
		const COleDateTime & dtCurrentDate, 
		bool bUpdateNow = false
		);
	void CurrentDateSet( 
		INT nYear,
		INT nMonth,
		INT nDay = 1,
		bool bUpdateNow = false
		);
	virtual COleDateTime CurrentDateGet() const;
	void CurrentDateGet( 
		INT & nYear, 
		INT & nMonth, 
		INT & nDay 
		) const;
	virtual void DimSet( 
		CSize sizeCalendarDimensionsMin = CSize(0,0),
		CSize sizeCalendarDimensionsMax = CSize(0,0),
		bool bUpdateNow = false
		);
	virtual void DimGet( 
		SIZE * pSizeCalendarDimensionsMin = NULL,
		SIZE * pSizeCalendarDimensionsMax = NULL,
		SIZE * pSizeCalendarDimensionsCurrent = NULL
		) const;

	LONG HitTest( 
		const POINT & ptClient,
		COleDateTime * pDT = NULL
		) const;

	virtual CExtSafeString OnDatePickerGetMonthName(
		INT nMonth,
		bool bMakeUpper = false
		) const;
	virtual INT OnDatePickerGetFirstDayOfWeek() const;
	virtual CExtSafeString OnDatePickerGetShortDayName(
		INT nDayOfWeek,
		bool bMakeUpper = false
		) const;
	virtual COleDateTime OnDatePickerGetToday() const;

	virtual HWND OnDatePickerGetNotificationReceiver() const;
	virtual void OnDatePickerDrawTodayBorder(
		CDC & dc,
		bool bShowNonMonthDays,
		bool bShowTodayHighlight,
		bool bDrawNonMonthDay,
		bool bDrawToday,
		const CRect & rcCell,
		const COleDateTime & dt,
		bool bDrawSelected
		) const;
	virtual void OnDatePickerDrawDateCell(
		CDC & dc,
		bool bShowNonMonthDays,
		bool bShowTodayHighlight,
		bool bDrawNonMonthDay,
		bool bDrawToday,
		const CRect & rcCell,
		const COleDateTime & dt,
		bool bDrawSelected
		) const;
	virtual void OnDatePickerDrawButtonsSeparator(
		CDC & dc,
		const CRect & rcSeparator
		) const;
	virtual CExtSafeString OnDatePickerQueryPushButtonCaptionText(
		LONG nButtonType // __EDPWH_BTN_NONE or __EDPWH_BTN_TODAY
		) const;
	virtual void OnDatePickerDrawPushButton(
		CDC & dc,
		const CRect & rcButton,
		LONG nButtonType, // __EDPWH_BTN_NONE or __EDPWH_BTN_TODAY
		__EXT_MFC_SAFE_LPCTSTR strCaption,
		bool bFlat,
		bool bDrawBorder,
		bool bPushed,
		bool bHover,
		HFONT hFont
		) const;
	virtual void OnDatePickerEraseHeader(
		INT nCol,
		INT nRow,
		CDC & dc,
		INT nYear,
		INT nMonth,
		const CRect & rcHeader,
		const CRect & rcHeaderWithoutBorders,
		const CRect & rcHeaderDate,
		const CRect & rcScrollBtnBackward,
		const CRect & rcScrollBtnForward
		) const;
	virtual void OnDatePickerDrawHeader(
		INT nCol,
		INT nRow,
		CDC & dc,
		INT nYear,
		INT nMonth,
		const CRect & rcHeader,
		const CRect & rcHeaderWithoutBorders,
		const CRect & rcHeaderDate,
		const CRect & rcScrollBtnBackward,
		const CRect & rcScrollBtnForward
		) const;
	virtual void OnDatePickerDrawDaysOfWeek(
		INT nYear,
		INT nMonth,
		CDC & dc,
		const CRect & rcClient
		) const;
	virtual void OnDatePickerDrawEntire(
		CDC & dc,
		const CRect & rcClient
		);
	virtual void OnDatePickerEraseClientArea(
		CDC & dc,
		const CRect & rcClient
		) const;
	virtual void OnDatePickerDrawBorder(
		CDC & dc,
		const CRect & rcClient
		) const;

	virtual CRect OnDatePickerGetBorderMetrics() const;

	virtual void OnDatePickerQueryHeaderPopupFont(
		CFont * pFont
		) const;
	virtual void OnDatePickerQueryDayCellFont(
		CFont * pFont,
		const COleDateTime & dt
		) const;
	virtual void OnDatePickerQueryMonthHeaderFont(
		CFont * pFont,
		INT nYear,
		INT nMonth
		) const;
	virtual void OnDatePickerQueryDaysOfWeekFont(
		CFont * pFont,
		INT nYear,
		INT nMonth
		) const;

	virtual INT OnDatePickerQueryIndentSpace() const;
	virtual CSize OnDatePickerQueryDateCellSize() const;
	virtual CSize OnDatePickerQueryPushButtonSize() const;
	virtual CSize OnDatePickerQueryScrollButtonSize() const;
	virtual INT OnDatePickerQueryMonthHeaderHeight() const;
	virtual INT OnDatePickerQueryDaysOfWeekHeight() const;
	virtual INT OnDatePickerQueryButtonAreaHeight() const;
	virtual CSize OnDatePickerQueryMonthPickerSize() const;
	virtual void OnDatePickerRecalcMetrics(
		CDC & dcMeasure,
		CSize & sizeMonthPicker,
		CSize & sizeDateCell,
		CSize & sizePushButton,
		CSize & sizeScrollButton,
		INT & nMonthHeaderHeight,
		INT & nButtonAreaHeight,
		INT & nDaysOfWeekHeight,
		INT & nIndentSpace
		);
	virtual CSize OnDatePickerCalcContentSize(
		const SIZE & sizeDim
		) const;

protected:
	virtual HFONT OnQueryFont() const;

	void _RecalcMetrics();

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

	virtual void OnDatePickerDoScroll(
		INT nMonthCount = 1,
		bool bSmoothScroll = true
		);
	
public:
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
	//{{AFX_VIRTUAL(CExtDatePickerWnd)
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool m_bAutoDeleteWindow:1;
	static volatile HWND g_hWndHeaderPopup;
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CExtDatePickerWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnSetFont( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetFont( WPARAM, LPARAM );
	//}}AFX_MSG
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
#if _MFC_VER < 0x700
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#else
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
#endif
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg void OnTimeChange();
	afx_msg void OnSysColorChange();
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThemeChanged( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

}; /// class CExtDatePickerWnd

__PROF_UIS_API void AFXAPI __EXT_MFC_DDX_DatePickerWnd( CDataExchange * pDX, INT nIDC, CTime & value1, CTime & value2 );
__PROF_UIS_API void AFXAPI __EXT_MFC_DDX_DatePickerWnd( CDataExchange * pDX, INT nIDC, COleDateTime & value1, COleDateTime & value2 );
__PROF_UIS_API void AFXAPI __EXT_MFC_DDX_DatePickerWnd( CDataExchange * pDX, INT nIDC, CTime & value );
__PROF_UIS_API void AFXAPI __EXT_MFC_DDX_DatePickerWnd( CDataExchange * pDX, INT nIDC, COleDateTime & value );

#endif // (!defined __EXT_MFC_NO_DATE_PICKER)

#if (!defined __EXT_MFC_NO_DATE_BROWSER)

/////////////////////////////////////////////////////////////////////////////
// CExtDateBrowserWnd window

#define __EXT_DATE_BROWSER_CENTURY_COLUMN_COUNT			4
#define __EXT_DATE_BROWSER_CENTURY_ROW_COUNT			3
#define __EXT_DATE_BROWSER_CENTURY_ITEM_COUNT			(__EXT_DATE_BROWSER_CENTURY_COLUMN_COUNT*__EXT_DATE_BROWSER_CENTURY_ROW_COUNT)

#define __EXT_DATE_BROWSER_YEAR_DECADE_COLUMN_COUNT		4
#define __EXT_DATE_BROWSER_YEAR_DECADE_ROW_COUNT		3
#define __EXT_DATE_BROWSER_YEAR_DECADE_ITEM_COUNT		(__EXT_DATE_BROWSER_YEAR_DECADE_COLUMN_COUNT*__EXT_DATE_BROWSER_YEAR_DECADE_ROW_COUNT)

#define __EXT_DATE_BROWSER_ONE_YEAR_COLUMN_COUNT		4
#define __EXT_DATE_BROWSER_ONE_YEAR_ROW_COUNT			3
#define __EXT_DATE_BROWSER_ONE_YEAR_ITEM_COUNT			(__EXT_DATE_BROWSER_ONE_YEAR_COLUMN_COUNT*__EXT_DATE_BROWSER_ONE_YEAR_ROW_COUNT)

#define __EXT_DATE_BROWSER_MONTH_COLUMN_COUNT			7
#define __EXT_DATE_BROWSER_MONTH_ROW_COUNT				7
#define __EXT_DATE_BROWSER_MONTH_ITEM_COUNT				(__EXT_DATE_BROWSER_MONTH_COLUMN_COUNT*__EXT_DATE_BROWSER_MONTH_ROW_COUNT)

#define __EXT_DATE_BROWSER_ANIMATION_ZOOM_STEP_COUNT	8
#define __EXT_DATE_BROWSER_ANIMATION_ZOOM_STEP_TIME		30
#define __EXT_DATE_BROWSER_ANIMATION_SCROLL_STEP_COUNT	8
#define __EXT_DATE_BROWSER_ANIMATION_SCROLL_STEP_TIME	30

#define  __EXT_DATE_BROWSER_WND_CLASS_NAME				 _T("ProfUIS-DateBrowserWindow")

class __PROF_UIS_API CExtDateBrowserWnd
	: public CWnd
	, public CExtPmBridge
{
protected:
	bool m_bDirectCreateCall:1;
	virtual bool _CreateHelper();
// Construction
public:
	DECLARE_DYNCREATE( CExtDateBrowserWnd );
	DECLARE_CExtPmBridge_MEMBERS_GENERIC( CExtDateBrowserWnd );
	CExtDateBrowserWnd();
	virtual ~CExtDateBrowserWnd();

	static bool g_bDateBrowserWndClassRegistered;
	virtual bool RegisterDateBrowserWndClass();
	virtual bool Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( __EXT_MFC_IDC_STATIC ),
		DWORD dwWindowStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CCreateContext * pContext = NULL
		);

	virtual HWND PmBridge_GetSafeHwnd() const;
	virtual void PmBridge_OnPaintManagerChanged(
		CExtPaintManager * pGlobalPM
		);

// Attributes
public:
	enum e_view_mode_t
	{
		__EVM_CENTURY					= 0,
		__EVM_YEAR_DECADE				= 1,
		__EVM_ONE_YEAR					= 2,
		__EVM_MONTH						= 3,
	};

	enum e_item_state_t
	{
		__EIST_NORMAL					= 0,
		__EIST_DISABLED					= 1,
		__EIST_HOVER					= 2,
		__EIST_PRESSED					= 3,
		__EIST_SELECTED_NORMAL			= 4,
		__EIST_SELECTED_HOVER			= 5,
		__EIST_SELECTED_PRESSED			= 6,
		__EIST_COUNT					= 7,
	};

	enum e_hit_test_info_area_flags_t
	{
		__EHTIAF_NOWHERE				= 0x00000000,
		__EHTIAF_ON_BUTTON_PREVIOUS		= 0x00000001,
		__EHTIAF_ON_BUTTON_NEXT			= 0x00000002,
		__EHTIAF_ON_CAPTION				= 0x00000004,
		__EHTIAF_ON_ITEM				= 0x00000008,
		__EHTIAF_INSIDE_CONTROL_MASK	= 0x0000000F,
		__EHTIAF_ON_MONTH_HEADER_ITEM	= 0x00000010,
		__EHTIAF_OUTSIDE_AT_TOP			= 0x00000100,
		__EHTIAF_OUTSIDE_AT_BOTTOM		= 0x00000200,
		__EHTIAF_OUTSIDE_AT_LEFT		= 0x00000400,
		__EHTIAF_OUTSIDE_AT_RIGHT		= 0x00000800,
		__EHTIAF_OUTSIDE_CONTROL_MASK	= (__EHTIAF_OUTSIDE_AT_TOP|__EHTIAF_OUTSIDE_AT_BOTTOM|__EHTIAF_OUTSIDE_AT_LEFT|__EHTIAF_OUTSIDE_AT_RIGHT),
		__EHTIAF_OUTSIDE_TOP_LEFT		= (__EHTIAF_OUTSIDE_AT_TOP|__EHTIAF_OUTSIDE_AT_LEFT),
		__EHTIAF_OUTSIDE_TOP_RIGHT		= (__EHTIAF_OUTSIDE_AT_TOP|__EHTIAF_OUTSIDE_AT_RIGHT),
		__EHTIAF_OUTSIDE_BOTTOM_RIGHT	= (__EHTIAF_OUTSIDE_AT_BOTTOM|__EHTIAF_OUTSIDE_AT_RIGHT),
		__EHTIAF_OUTSIDE_BOTTOM_LEFT	= (__EHTIAF_OUTSIDE_AT_BOTTOM|__EHTIAF_OUTSIDE_AT_LEFT),
	};

	enum e_paint_style_t
	{
		__EPST_SIMPLE					= 0,
		__EPST_SKIN						= 1,
		__EPST_PAINT_MANAGER			= 2,
	};

	enum e_painted_item_type_t
	{
		__EPIT_NORMAL_ITEM				= 0,
		__EPIT_OUT_OF_RANGE_ITEM		= 1,
		__EPIT_WEEK_CAPTION_ITEM		= 2,
	};

	struct __PROF_UIS_API HITTESTINFO
	{
		CPoint m_ptClient;
		INT m_nAreaFlags;
		INT m_nItemIndex;
		COleDateTime m_time;
		HITTESTINFO()
			: m_ptClient( 0,0 )
			, m_nAreaFlags( INT(__EHTIAF_NOWHERE) )
			, m_nItemIndex( -1 )
		{
			m_time.SetStatus( COleDateTime::invalid );
		}
		HITTESTINFO( const POINT & ptClient )
			: m_ptClient( ptClient )
			, m_nAreaFlags( INT(__EHTIAF_NOWHERE) )
			, m_nItemIndex( -1 )
		{
			m_time.SetStatus( COleDateTime::invalid );
		}
		HITTESTINFO( const HITTESTINFO & other )
		{
			m_ptClient		= other.m_ptClient;
			m_nAreaFlags	= other.m_nAreaFlags;
			m_nItemIndex	= other.m_nItemIndex;
			m_time			= other.m_time;
		}
		HITTESTINFO & operator = ( const HITTESTINFO & other )
		{
			m_ptClient		= other.m_ptClient;
			m_nAreaFlags	= other.m_nAreaFlags;
			m_nItemIndex	= other.m_nItemIndex;
			m_time			= other.m_time;
			return (*this);
		}
		bool operator == ( const HITTESTINFO & other ) const
		{
			if(		/*
					m_ptClient		== other.m_ptClient
				&&	*/
					m_nAreaFlags	== other.m_nAreaFlags
				&&	m_nItemIndex	== other.m_nItemIndex
					/*
				&&	m_time			== other.m_time
					*/
				)
				return true;
			else
				return false;
		}
		bool operator != ( const HITTESTINFO & other ) const
		{
			if( (*this) == other )
				return false;
			else
				return true;
		}
		e_item_state_t GetCaptionItemState( bool bPressedTracking )
		{
			if( ( m_nAreaFlags & __EHTIAF_ON_CAPTION ) != 0 )
				return bPressedTracking ? __EIST_PRESSED : __EIST_HOVER;
			else
				return __EIST_NORMAL;
		}
		e_item_state_t GetButtonItemState( bool bPrevious, bool bPressedTracking )
		{
			if( bPrevious )
			{
				if( ( m_nAreaFlags & __EHTIAF_ON_BUTTON_PREVIOUS ) != 0 )
					return bPressedTracking ? __EIST_PRESSED : __EIST_HOVER;
			}
			else
			{
				if( ( m_nAreaFlags & __EHTIAF_ON_BUTTON_NEXT ) != 0 )
					return bPressedTracking ? __EIST_PRESSED : __EIST_HOVER;
			}
			return __EIST_NORMAL;
		}
		e_item_state_t GetPaletteItemState( INT nPaletteItemIndex, bool bSelected, bool bPressedTracking )
		{
			if(		( m_nAreaFlags & __EHTIAF_ON_ITEM ) != 0
				&&	m_nItemIndex == nPaletteItemIndex
				)
				return bPressedTracking
					? ( bSelected ? __EIST_SELECTED_PRESSED : __EIST_PRESSED )
					: ( bSelected ? __EIST_SELECTED_HOVER : __EIST_HOVER )
					;
			else
				return bSelected
					? __EIST_SELECTED_NORMAL
					: __EIST_NORMAL
					;
		}
	};

	struct __PROF_UIS_API SKINDATA
	{
		CExtBitmap m_bmpTodayOver;
		CExtBitmap m_bmpAnyDisabled;
		CExtBitmap m_bmpAnyNormal,       m_bmpAnyHover,       m_bmpAnyPressed,       m_bmpAnyUnfocusedWnd;
		CExtBitmap m_bmpSelectedNormal,  m_bmpSelectedHover,  m_bmpSelectedPressed,  m_bmpSelectedUnfocusedWnd;
		COLORREF m_clrTextColorNormal, m_clrTextColorHover, m_clrTextColorSelected, m_clrTextColorPressed, m_clrTextColorDisabled,
			m_clrTextColorOutOfRange, m_clrBackground, m_clrCaptionTextColorNormal, m_clrCaptionTextColorHover, m_clrCaptionTextColorPressed,
			m_clrTextColorToday;
		CRect m_rcPadding, m_rcFocusRectPadding;
		SKINDATA()
			: m_rcPadding( 0, 0, 0, 0 )
			, m_clrTextColorNormal( COLORREF(-1L) )
			, m_clrTextColorHover( COLORREF(-1L) )
			, m_clrTextColorSelected( COLORREF(-1L) )
			, m_clrTextColorPressed( COLORREF(-1L) )
			, m_clrTextColorDisabled( COLORREF(-1L) )
			, m_clrTextColorOutOfRange( COLORREF(-1L) )
			, m_clrBackground( COLORREF(-1L) )
			, m_clrCaptionTextColorNormal( COLORREF(-1L) )
			, m_clrCaptionTextColorHover( COLORREF(-1L) )
			, m_clrCaptionTextColorPressed( COLORREF(-1L) )
			, m_clrTextColorToday( COLORREF(-1L) )
			, m_rcFocusRectPadding( -1, -1, -1, -1 )
		{
		}
		void EmptySkin()
		{
			m_rcPadding.SetRect( 0, 0, 0, 0 );
			m_bmpTodayOver.Empty();
			m_bmpAnyDisabled.Empty();
			m_bmpAnyNormal.Empty();
			m_bmpAnyHover.Empty();
			m_bmpAnyPressed.Empty();
			m_bmpAnyUnfocusedWnd.Empty();
			m_bmpSelectedNormal.Empty();
			m_bmpSelectedHover.Empty();
			m_bmpSelectedPressed.Empty();
			m_bmpSelectedUnfocusedWnd.Empty();
			m_clrTextColorNormal = m_clrTextColorHover = m_clrTextColorSelected =
				m_clrTextColorPressed = m_clrTextColorDisabled = m_clrTextColorOutOfRange = m_clrBackground =
				m_clrCaptionTextColorNormal = m_clrCaptionTextColorHover = m_clrCaptionTextColorPressed = m_clrTextColorToday =
				COLORREF(-1L);
			m_rcFocusRectPadding.SetRect( -1, -1, -1, -1 );
		}
		void LoadVistaSkin();
		COLORREF GetCaptionTextColor(
			e_item_state_t eIST,
			e_painted_item_type_t ePIT,
			bool bWindowFocused
			);
		COLORREF GetItemTextColor(
			e_item_state_t eIST,
			e_painted_item_type_t ePIT,
			bool bWindowFocused,
			bool bToday
			);
		void PaintItemBackground(
			CDC & dc,
			CRect rcItem,
			e_item_state_t eIST,
			e_painted_item_type_t ePIT,
			bool bWindowFocused,
			bool bToday
			);
	};

protected:
	HITTESTINFO m_hitTestLast;

	bool m_bPressedTracking:1;
	bool m_bPressedLeftRightButton:1;

	COLORREF m_clrBackground, m_clrMonthSeparator,
		m_arrColorsItemText[4][__EIST_COUNT],
		m_arrColorsItemBackground[4][__EIST_COUNT];
	e_view_mode_t m_eVM;
	e_paint_style_t m_ePS;
	COleDateTime m_timeCurrent, m_timeMin, m_timeMax;
	int m_nScrollingDirection;
	int m_nMonthSeparatorHeight;
	UINT m_nTimerID, m_nTimerEllapse;
public:
	bool m_bShowItemsAfterCentury:1;
	bool m_bShowItemsBeforeCentury:1;
	bool m_bShowItemsAfterYearDecade:1;
	bool m_bShowItemsBeforeYearDecade:1;
	bool m_bShowDaysAfterMonth:1;
	bool m_bShowDaysBeforeMonth:1;
	bool m_bShowOutOfRangeItems:1;

	INT m_nFirstDayOfWeek; // 0-Sun, 1-Mon, ..., -1 - get from locale
	CRect m_rcFocusRectPadding;
	CExtCmdIcon m_iconBtnPrevious, m_iconBtnNext;
	SKINDATA m_skin;
	INT m_nAnimationStepCountZoom,
		m_nAnimationStepTimeZoom,
		m_nAnimationStepCountScroll,
		m_nAnimationStepTimeScroll;

protected:
	static INT stat_GetColorArraySliceIndex(
		bool bWindowFocused,
		bool bToday
		);
public:
	virtual COLORREF ItemTextColorGet(
		e_item_state_t eIST,
		bool bWindowFocused,
		bool bToday
		) const;
	virtual void ItemTextColorSet( 
		e_item_state_t eIST,
		bool bWindowFocused,
		bool bToday,
		COLORREF clr
		);
	virtual void ItemTextColorSet( 
		e_item_state_t eIST,
		bool bWindowFocused,
		COLORREF clr
		);
	virtual void ItemTextColorSet( 
		e_item_state_t eIST,
		COLORREF clr
		);
	virtual COLORREF ItemBackgroundColorGet(
		e_item_state_t eIST,
		bool bWindowFocused,
		bool bToday
		) const;
	virtual void ItemBackgroundColorSet( 
		e_item_state_t eIST,
		bool bWindowFocused,
		bool bToday,
		COLORREF clr
		);
	virtual void ItemBackgroundColorSet( 
		e_item_state_t eIST,
		bool bWindowFocused,
		COLORREF clr
		);
	virtual void ItemBackgroundColorSet( 
		e_item_state_t eIST,
		COLORREF clr
		);

	virtual COleDateTime TimeGet(
		INT nType = 0
		) const;
	virtual bool TimeSet(
		const COleDateTime & _time,
		INT nType = 0
		);
	virtual bool OnDateBrowserTimeChanging(
		const COleDateTime & _timeOld,
		const COleDateTime & _timeNew
		);
	virtual void OnDateBrowserTimeChanged(
		const COleDateTime & _timeOld,
		const COleDateTime & _timeNew
		);
	
	virtual e_view_mode_t ViewModeGet() const;
	virtual void ViewModeSet(
		e_view_mode_t eVM
		);
	virtual bool OnDateBrowserViewModeChanging(
		e_view_mode_t eVM_old,
		e_view_mode_t eVM_new
		);
	virtual void OnDateBrowserViewModeChanged(
		e_view_mode_t eVM_old,
		e_view_mode_t eVM_new
		);

	virtual bool OnDateBrowserQueryTodayFlag(
		const COleDateTime & _time,
		e_view_mode_t eVM,
		const COleDateTime * pCurrentTime = NULL
		) const;

	virtual COLORREF OnDateBrowserGetBackgroundColor() const;
	virtual COLORREF BackgroundColorGet() const;
	virtual void BackgroundColorSet( COLORREF clr );

// Operations
public:
	virtual HITTESTINFO & HitTest(
		HITTESTINFO & _hitTestInfo
		) const;

	virtual void OnDateBrowserPaintCaption(
		CDC & dc,
		e_view_mode_t _eVM,
		__EXT_MFC_SAFE_LPCTSTR strText,
		CRect rcItem,
		e_item_state_t eIST,
		e_painted_item_type_t ePIT,
		bool bWindowFocused
		);
	virtual void OnDateBrowserPaintButton(
		CDC & dc,
		bool bPrevious,
		CRect rcItem,
		e_item_state_t eIST
		);
	virtual CFont * OnDateBrowserQueryCaptionFont(
		e_view_mode_t _eVM
		) const;
	virtual CExtCmdIcon & OnDateBrowserQueryButtonIcon(
		bool bPrevious
		);
	virtual CFont * OnDateBrowserQueryItemFont(
		e_view_mode_t _eVM
		) const;

	virtual bool OnDateBrowserQueryWindowFocusedState() const;

	virtual void OnDateBrowserPaintEntire(
		CDC & dc,
		const COleDateTime & _time,
		e_view_mode_t eVM,
		CRect rcEntire,
		bool bWindowFocused
		);

	virtual bool OnDateBrowserHoverChanging(
		const HITTESTINFO & _hitTestOld,
		const HITTESTINFO & _hitTestNew
		);
	virtual void OnDateBrowserHoverChanged(
		const HITTESTINFO & _hitTestOld,
		const HITTESTINFO & _hitTestNew
		);

	/////////////////////////////////////////////////////////////////////////////////////////
	// general layout calculation

	virtual CSize OnDateBrowserCalcSize(
		CDC & dcMeasure,
		const COleDateTime & _time
		) const;
	CSize CalcSize(
		const COleDateTime & _time
		) const;
	CSize CalcSize() const;
	virtual CSize OnDateBrowserCalcCaptionSize(
		CDC & dcMeasure,
		e_view_mode_t _eVM,
		const COleDateTime & _time
		) const;
	virtual void OnDateBrowserCalcLayoutGeneral(
		CDC & dcMeasure,
		e_view_mode_t _eVM,
		const COleDateTime & _time,
		CRect rcEntire,
		CRect & rcCaption,
		CRect & rcButtonPrevious,
		CRect & rcButtonNext,
		CRect & rcRest
		);
	virtual CSize OnDateBrowserCalcButtonSize(
		CDC & dcMeasure,
		bool bPrevious
		);
	virtual CExtSafeString OnDateBrowserQueryCaptionText(
		const COleDateTime & _time,
		e_view_mode_t _eVM
		) const;
	virtual CRect OnDateBrowserQueryButtonPadding(
		bool bPrevious
		) const;
	virtual CRect OnDateBrowserQueryCaptionPadding() const;

	virtual bool OnDateBrowserQueryPaletteItemIndexSelectedState(
		const COleDateTime & _time,
		const COleDateTime & _timeCurrent,
		e_view_mode_t _eVM
		) const;

	virtual COLORREF OnDateBrowserQueryPaletteItemTextColor(
		e_view_mode_t _eVM,
		e_item_state_t eIST,
		e_painted_item_type_t ePIT,
		bool bWindowFocused,
		bool bToday
		);
	virtual COLORREF OnDateBrowserQueryPaletteItemBackgroundColor(
		e_view_mode_t _eVM,
		e_item_state_t eIST,
		e_painted_item_type_t ePIT,
		bool bWindowFocused,
		bool bToday
		);

	virtual COLORREF OnDateBrowserQueryCaptionTextColor(
		e_view_mode_t _eVM,
		e_item_state_t eIST,
		e_painted_item_type_t ePIT,
		bool bWindowFocused
		);
	virtual COLORREF OnDateBrowserQueryCaptionBackgroundColor(
		e_view_mode_t _eVM,
		e_item_state_t eIST,
		e_painted_item_type_t ePIT,
		bool bWindowFocused
		);
	static void stat_CalcMilleniumDisplayYearRange(
		LONG nSourceYear,
		LONG & nFirstYear,
		LONG & nLastYear
		);

	/////////////////////////////////////////////////////////////////////////////////////////
	// everything related to the __EVM_CENTURY view mode

	virtual void OnDateBrowserPaintEntireCentury(
		CDC & dc,
		const COleDateTime & _time,
		CRect rcEntire,
		bool bWindowFocused
		);
	static void stat_CalcCenturyDisplayYearRange(
		LONG nSourceYear,
		LONG & nFirstYear,
		LONG & nLastYear
		);
	virtual CSize OnDateBrowserCalcCenturyItemSize(
		CDC & dcMeasure
		) const;
	virtual CSize OnDateBrowserMeasureLayoutCentury(
		CDC & dcMeasure,
		const COleDateTime & _time
		) const;
	virtual void OnDateBrowserCalcLayoutCentury(
		CDC & dcMeasure,
		const COleDateTime & _time,
		CRect rcEntire,
		CRect & rcCaption,
		CRect & rcButtonPrevious,
		CRect & rcButtonNext,
		CList < CRect, CRect & > & listItems,
		CList < COleDateTime, COleDateTime & > & listTimes,
		INT & nInvisibleCountAtStart,
		INT & nInvisibleCountAtEnd
		);
	virtual CRect OnDateBrowserQueryCenturyItemPadding() const;
	virtual bool OnDateBrowserQueryCenturyItemVisibility( const COleDateTime & _time ) const;
	virtual void OnDateBrowserPaintCenturyItem(
		CDC & dc,
		__EXT_MFC_SAFE_LPCTSTR strText,
		CRect rcItem,
		e_item_state_t eIST,
		e_painted_item_type_t ePIT,
		bool bWindowFocused,
		bool bToday
		);
	virtual CExtSafeString OnDateBrowserQueryCenturyItemText(
		const COleDateTime & _time
		) const;

	virtual void OnDateBrowserQueryShowPrevNextCenturyItems( 
		bool & bShowItemsBeforeCentury, 
		bool & bShowItemsAfterCentury 
		);

	/////////////////////////////////////////////////////////////////////////////////////////
	// everything related to the __EVM_YEAR_DECADE view mode

	virtual void OnDateBrowserPaintEntireYearDecade(
		CDC & dc,
		const COleDateTime & _time,
		CRect rcEntire,
		bool bWindowFocused
		);
	static void stat_CalcYearDecadeDisplayYearRange(
		LONG nSourceYear,
		LONG & nFirstYear,
		LONG & nLastYear
		);
	virtual CSize OnDateBrowserCalcYearDecadeItemSize(
		CDC & dcMeasure
		) const;
	virtual CSize OnDateBrowserMeasureLayoutYearDecade(
		CDC & dcMeasure,
		const COleDateTime & _time
		) const;
	virtual void OnDateBrowserCalcLayoutYearDecade(
		CDC & dcMeasure,
		const COleDateTime & _time,
		CRect rcEntire,
		CRect & rcCaption,
		CRect & rcButtonPrevious,
		CRect & rcButtonNext,
		CList < CRect, CRect & > & listItems,
		CList < COleDateTime, COleDateTime & > & listTimes,
		INT & nInvisibleCountAtStart,
		INT & nInvisibleCountAtEnd
		);
	virtual CRect OnDateBrowserQueryYearDecadeItemPadding() const;
	virtual bool OnDateBrowserQueryYearDecadeItemVisibility( const COleDateTime & _time ) const;
	virtual void OnDateBrowserPaintYearDecadeItem(
		CDC & dc,
		__EXT_MFC_SAFE_LPCTSTR strText,
		CRect rcItem,
		e_item_state_t eIST,
		e_painted_item_type_t ePIT,
		bool bWindowFocused,
		bool bToday
		);
	virtual CExtSafeString OnDateBrowserQueryYearDecadeItemText(
		const COleDateTime & _time
		) const;

	virtual void OnDateBrowserQueryShowPrevNextYearDecadeItems( 
		bool & bShowItemsBeforeYearDecade, 
		bool & bShowItemsAfterYearDecade 
		);

	/////////////////////////////////////////////////////////////////////////////////////////
	// everything related to the __EVM_ONE_YEAR view mode

	virtual void OnDateBrowserPaintEntireOneYear(
		CDC & dc,
		const COleDateTime & _time,
		CRect rcEntire,
		bool bWindowFocused
		);
	virtual CSize OnDateBrowserCalcOneYearItemSize(
		CDC & dcMeasure
		) const;
	virtual CSize OnDateBrowserMeasureLayoutOneYear(
		CDC & dcMeasure,
		const COleDateTime & _time
		) const;
	virtual void OnDateBrowserCalcLayoutOneYear(
		CDC & dcMeasure,
		const COleDateTime & _time,
		CRect rcEntire,
		CRect & rcCaption,
		CRect & rcButtonPrevious,
		CRect & rcButtonNext,
		CList < CRect, CRect & > & listItems,
		CList < COleDateTime, COleDateTime & > & listTimes,
		INT & nInvisibleCountAtStart,
		INT & nInvisibleCountAtEnd
		);
	virtual CRect OnDateBrowserQueryOneYearItemPadding() const;
	virtual bool OnDateBrowserQueryOneYearItemVisibility( const COleDateTime & _time ) const;
	virtual void OnDateBrowserPaintOneYearItem(
		CDC & dc,
		__EXT_MFC_SAFE_LPCTSTR strText,
		CRect rcItem,
		e_item_state_t eIST,
		e_painted_item_type_t ePIT,
		bool bWindowFocused,
		bool bToday
		);
	virtual CExtSafeString OnDateBrowserQueryOneYearItemText(
		const COleDateTime & _time
		) const;

	/////////////////////////////////////////////////////////////////////////////////////////
	// everything related to the __EVM_MONTH view mode

	virtual void OnDateBrowserPaintEntireMonth(
		CDC & dc,
		const COleDateTime & _time,
		CRect rcEntire,
		bool bWindowFocused
		);
	virtual CSize OnDateBrowserCalcMonthItemSize(
		CDC & dcMeasure
		) const;
	virtual CSize OnDateBrowserMeasureLayoutMonth(
		CDC & dcMeasure,
		const COleDateTime & _time
		) const;
	virtual void OnDateBrowserCalcLayoutMonth(
		CDC & dcMeasure,
		const COleDateTime & _time,
		CRect rcEntire,
		CRect & rcCaption,
		CRect & rcMonthSeparator,
		CRect & rcButtonPrevious,
		CRect & rcButtonNext,
		CList < CRect, CRect & > & listItems,
		CList < COleDateTime, COleDateTime & > & listTimes,
		INT & nInvisibleCountAtStart,
		INT & nInvisibleCountAtEnd
		);
	virtual CRect OnDateBrowserQueryMonthItemPadding() const;
	virtual bool OnDateBrowserQueryMonthItemVisibility( const COleDateTime & _time ) const;
	virtual void OnDateBrowserPaintMonthItem(
		CDC & dc,
		__EXT_MFC_SAFE_LPCTSTR strText,
		CRect rcItem,
		e_item_state_t eIST,
		e_painted_item_type_t ePIT,
		bool bWindowFocused,
		bool bToday,
		INT nX,
		INT nY,
		INT _dayOfWeek // 0-Sun, 1-Mon, ...
		);
	virtual CExtSafeString OnDateBrowserQueryMonthItemText(
		const COleDateTime & _time
		) const;
	virtual void OnDateBrowserQueryShowPrevNextMonthDays( 
		bool & bShowDaysBeforeMonth, 
		bool & bShowDaysAfterMonth 
		);

	virtual CExtSafeString OnDateBrowserQueryWeekItemText(
		INT _dayOfWeek // 0-Sun, 1-Mon, ...
		) const;
	virtual INT OnDateBrowserQueryFirstDayOfWeekInMonthView() const;

	virtual e_paint_style_t PaintStyleGet() const;
	virtual void PaintStyleSet( e_paint_style_t _ePS );

	virtual int MonthSeparatorHeightGet() const;
	virtual void MonthSeparatorHeightSet( int nMonthSeparatorHeight );
	virtual int OnDateBrowserQueryMonthSeparatorHeight() const;
	virtual void OnDateBrowserPaintMonthSeparator( 
		CDC & dc,
		CRect rcItem
		);
	virtual COLORREF MonthSeparatorColorGet() const;
	virtual void MonthSeparatorColorSet( COLORREF clrMonthSeparator );
	virtual COLORREF OnDateBrowserQueryMonthSeparatorColor() const;
	virtual bool OnDateBrowserAnimateTimeChanging(
		const COleDateTime & _timeOld,
		const COleDateTime & _timeNew
		) const;
	virtual bool OnDateBrowserAnimateViewModeChanging(
		e_view_mode_t eVM_old,
		e_view_mode_t eVM_new
		) const;

protected:
	CRect GetSelectedItemRect(
		CDC & dcMeasure,
		e_view_mode_t _eVM,
		const COleDateTime & _time,
		CRect rcEntire
		) const;
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDateBrowserWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtDateBrowserWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThemeChanged( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
}; /// class CExtDateBrowserWnd

#endif // (!defined __EXT_MFC_NO_DATE_BROWSER)

#endif // (!defined __EXT_MFC_NO_DATE_PICKER) || (!defined __EXT_MFC_NO_DATE_BROWSER)

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __EXT_DATE_PICKER_H

