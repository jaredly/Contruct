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

#if (!defined __EXT_IMAGEEDITWND_H)
#define __EXT_IMAGEEDITWND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined __EXT_MFC_NO_IMAGEEDITWND)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_SCROLLWND_H)
	#include <ExtScrollWnd.h>
#endif 

/////////////////////////////////////////////////////////////////////////////
// CExtImageEditWnd window

// center image in client area
#define	__EIEDWS_CENTER					0x00000001L
// read-only mode
#define __EIEDWS_READONLY				0x00000002L
// show outer area
#define __EIEDWS_OUTER_AREA				0x00000004L
// show tools in outer area (with __EIEDWS_OUTER_AREA)
#define __EIEDWS_OUTER_TOOLS			0x00000008L
// allow image resizing
// (with __EIEDWS_OUTER_AREA and __EIEDWS_OUTER_TOOLS)
#define __EIEDWS_OUTER_RESIZING			0x00000010L
// request focus on mouse clicks
#define __EIEDWS_REQUEST_FOCUS			0x00000020L
// paint grid lines
#define __EIEDWS_GRID_LINES				0x00000040L
// paint solid grid lines (with __EIEDWS_GRID_LINES)
#define __EIEDWS_SOLID_GRID				0x00000080L
// do not change magnification (with mouse wheel)
#define __EIEDWS_NO_MAGNIFY				0x00000100L
// draw button frame instead of __EIEDWS_OUTER_TOOLS
// (with __EIEDWS_OUTER_AREA and __EIEDWS_OUTER_TOOLS)
#define __EIEDWS_BUTTON_FRAME			0x00000200L
// use single __EUT_MOVING_SELECTION undo action
//#define __EIEDWS_USE_SINGLEMOVING_UNDO	0x00000400L
#define __EIEDWS_USER_STYLE_21			0x00000400L

#define __EIEDWS_USER_STYLE_20			0x00000800L
#define __EIEDWS_USER_STYLE_19			0x00001000L
#define __EIEDWS_USER_STYLE_18			0x00002000L
#define __EIEDWS_USER_STYLE_17			0x00004000L
#define __EIEDWS_USER_STYLE_16			0x00008000L
#define __EIEDWS_USER_STYLE_15			0x00010000L
#define __EIEDWS_USER_STYLE_14			0x00020000L
#define __EIEDWS_USER_STYLE_13			0x00040000L
#define __EIEDWS_USER_STYLE_12			0x00080000L
#define __EIEDWS_USER_STYLE_11			0x00100000L
#define __EIEDWS_USER_STYLE_10			0x00200000L
#define __EIEDWS_USER_STYLE_09			0x00400000L
#define __EIEDWS_USER_STYLE_08			0x00800000L
#define __EIEDWS_USER_STYLE_07			0x01000000L
#define __EIEDWS_USER_STYLE_06			0x02000000L
#define __EIEDWS_USER_STYLE_05			0x04000000L
#define __EIEDWS_USER_STYLE_04			0x08000000L
#define __EIEDWS_USER_STYLE_03			0x10000000L
#define __EIEDWS_USER_STYLE_02			0x20000000L
#define __EIEDWS_USER_STYLE_01			0x40000000L
#define __EIEDWS_USER_STYLE_00			0x80000000L

#define __EIEDWS_DEFAULT_STYLES \
	( __EIEDWS_CENTER \
	| __EIEDWS_OUTER_AREA \
	| __EIEDWS_OUTER_TOOLS \
	| __EIEDWS_OUTER_RESIZING \
	| __EIEDWS_GRID_LINES \
	)


#define __IED_COLORREF_NO_TRANSPARENCY ((COLORREF)(-1L))

#define __IED_DEF_AIRBRUSH_TIMER_ID	13
#define __IED_DEF_AIRBRUSH_TIMER_ELLAPSE 50

class __PROF_UIS_API CExtImageEditWnd : public CExtScrollWnd
{
// Construction
public:
	DECLARE_DYNCREATE( CExtImageEditWnd );

	CExtImageEditWnd();

// Attributes
public:
	enum e_tool_t
	{
		__IEDT_RECT_SELECTION						= 0,
		__IEDT_COLOR_SELECTION						= 1,
		
		__IEDT_FILL									= 2,
		__IEDT_MAGNIFICATON							= 3,
		__IEDT_PENCIL								= 4,
		
		__IEDT_BRUSH_CIRCLE_SMALL					= 5,
		__IEDT_BRUSH_CIRCLE_MEDIUM					= 6,
		__IEDT_BRUSH_CIRCLE_LARGE					= 7,
		__IEDT_BRUSH_SQUARE_SMALL					= 8,
		__IEDT_BRUSH_SQUARE_MEDIUM					= 9,
		__IEDT_BRUSH_SQUARE_LARGE					= 10,
		__IEDT_BRUSH_LINE_LD_SMALL					= 11,
		__IEDT_BRUSH_LINE_LD_MEDIUM					= 12,
		__IEDT_BRUSH_LINE_LD_LARGE					= 13,
		__IEDT_BRUSH_LINE_RD_SMALL					= 14,
		__IEDT_BRUSH_LINE_RD_MEDIUM					= 15,
		__IEDT_BRUSH_LINE_RD_LARGE					= 16,
		__IEDT_BRUSH_VALUE_MIN						= 5,
		__IEDT_BRUSH_VALUE_MAX						= 16,
		
		__IEDT_AIRBRUSH_SMALL						= 17,
		__IEDT_AIRBRUSH_MEDIUM						= 18,
		__IEDT_AIRBRUSH_LARGE						= 19,
		__IEDT_AIRBRUSH_VALUE_MIN					= 17,
		__IEDT_AIRBRUSH_VALUE_MAX					= 19,

		__IEDT_LINE_CIRCLE_SMALL					= 20,
		__IEDT_LINE_CIRCLE_MEDIUM					= 21,
		__IEDT_LINE_CIRCLE_LARGE					= 22,
		__IEDT_LINE_SQUARE_SMALL					= 23,
		__IEDT_LINE_SQUARE_MEDIUM					= 24,
		__IEDT_LINE_SQUARE_LARGE					= 25,
		__IEDT_LINE_LINE_LD_SMALL					= 26,
		__IEDT_LINE_LINE_LD_MEDIUM					= 27,
		__IEDT_LINE_LINE_LD_LARGE					= 28,
		__IEDT_LINE_LINE_RD_SMALL					= 29,
		__IEDT_LINE_LINE_RD_MEDIUM					= 30,
		__IEDT_LINE_LINE_RD_LARGE					= 31,
		__IEDT_LINE_VALUE_MIN						= 20,
		__IEDT_LINE_VALUE_MAX						= 31,

		__IEDT_RECT_SMALLER							= 32,
		__IEDT_RECT_SMALL							= 33,
		__IEDT_RECT_MEDIUM							= 34,
		__IEDT_RECT_LARGE							= 35,
		__IEDT_RECT_LARGER							= 36,
		__IEDT_RECT_VALUE_MIN						= 32,
		__IEDT_RECT_VALUE_MAX						= 36,

		__IEDT_RECT_OUTLINED_SMALLER				= 37,
		__IEDT_RECT_OUTLINED_SMALL					= 38,
		__IEDT_RECT_OUTLINED_MEDIUM					= 39,
		__IEDT_RECT_OUTLINED_LARGE					= 40,
		__IEDT_RECT_OUTLINED_LARGER					= 41,
		__IEDT_RECT_OUTLINED_VALUE_MIN				= 37,
		__IEDT_RECT_OUTLINED_VALUE_MAX				= 41,

		__IEDT_ROUNDED_RECT_SMALLER					= 42,
		__IEDT_ROUNDED_RECT_SMALL					= 43,
		__IEDT_ROUNDED_RECT_MEDIUM					= 44,
		__IEDT_ROUNDED_RECT_LARGE					= 45,
		__IEDT_ROUNDED_RECT_LARGER					= 46,
		__IEDT_ROUNDED_RECT_VALUE_MIN				= 42,
		__IEDT_ROUNDED_RECT_VALUE_MAX				= 46,

		__IEDT_ROUNDED_RECT_OUTLINED_SMALLER		= 47,
		__IEDT_ROUNDED_RECT_OUTLINED_SMALL			= 48,
		__IEDT_ROUNDED_RECT_OUTLINED_MEDIUM			= 49,
		__IEDT_ROUNDED_RECT_OUTLINED_LARGE			= 50,
		__IEDT_ROUNDED_RECT_OUTLINED_LARGER			= 51,
		__IEDT_ROUNDED_RECT_OUTLINED_VALUE_MIN		= 47,
		__IEDT_ROUNDED_RECT_OUTLINED_VALUE_MAX		= 51,

		__IEDT_ELLIPSE_SMALLER						= 52,
		__IEDT_ELLIPSE_SMALL						= 53,
		__IEDT_ELLIPSE_MEDIUM						= 54,
		__IEDT_ELLIPSE_LARGE						= 55,
		__IEDT_ELLIPSE_LARGER						= 56,
		__IEDT_ELLIPSE_VALUE_MIN					= 52,
		__IEDT_ELLIPSE_VALUE_MAX					= 56,

		__IEDT_ELLIPSE_OUTLINED_SMALLER				= 57,
		__IEDT_ELLIPSE_OUTLINED_SMALL				= 58,
		__IEDT_ELLIPSE_OUTLINED_MEDIUM				= 59,
		__IEDT_ELLIPSE_OUTLINED_LARGE				= 60,
		__IEDT_ELLIPSE_OUTLINED_LARGER				= 61,
		__IEDT_ELLIPSE_OUTLINED_VALUE_MIN			= 57,
		__IEDT_ELLIPSE_OUTLINED_VALUE_MAX			= 61,

		__IEDT_RECT_FILLED							= 62,
		__IEDT_ROUNDED_RECT_FILLED					= 63,
		__IEDT_ELLIPSE_FILLED						= 64,

		__IEDT_2D_GEOMETRY_VALUE_MIN				= 32,
		__IEDT_2D_GEOMETRY_VALUE_MAX				= 64,

		__IEDT_VALUE_MIN							= 0,
		__IEDT_VALUE_MAX							= 64,

		__IEDT_NONE									= -1
	};

	enum e_undo_type_t
	{
		__EUT_UNKNOWN_OPERATION				= 0L,
		__EUT_TRANSPARENCY_MODE				= 1L,
		__EUT_RESIZING_IMAGE				= 2L,
		__EUT_RESIZING_SELECTION			= 3L,
		__EUT_FILL							= 4L,
		__EUT_PENCIL						= 5L,
		__EUT_BRUSH							= 6L,
		__EUT_AIRBRUSH						= 7L,
		__EUT_LINE							= 8L,
		__EUT_RECTANGLE						= 9L,
		__EUT_RECTANGLE_OUTLINED			= 10L,
		__EUT_RECTANGLE_FILLED				= 11L,
		__EUT_ROUNDED_RECTANGLE				= 12L,
		__EUT_ROUNDED_RECTANGLE_OUTLINED	= 13L,
		__EUT_ROUNDED_RECTANGLE_FILLED		= 14L,
		__EUT_ELLIPSE						= 15L,
		__EUT_ELLIPSE_OUTLINED				= 16L,
		__EUT_ELLIPSE_FILLED				= 17L,
		__EUT_MOVING_SELECTION				= 18L,
		__EUT_SELECTION_DELETE				= 19L,
		__EUT_IMAGE_CUT						= 20L,
		__EUT_SELECTION_CUT					= 21L,
		__EUT_PASTE							= 22L,
		__EUT_USER_DEFINED_0				= 23L,
	};

	class __PROF_UIS_API CSurfaceDC : public CDC
	{
		HGDIOBJ m_hSurfaceOld;
	public:
		CSurfaceDC( CBitmap & bmpSurface )
			: m_hSurfaceOld( NULL )
		{
			ASSERT( bmpSurface.GetSafeHandle() != NULL );
			if( !CreateCompatibleDC( NULL ) )
			{
				ASSERT( FALSE );
				return;
			}
			m_hSurfaceOld =
				::SelectObject(
					m_hDC,
					(HGDIOBJ)bmpSurface.GetSafeHandle()
					);
		}
		~CSurfaceDC()
		{
			if( GetSafeHdc() != NULL )
			{
				::SelectObject(
					m_hDC,
					m_hSurfaceOld
					);
			}
		}
	}; // class CSurfaceDC

	class __PROF_UIS_API IMAGEEDITDATA
	{
		CBitmap m_bmpSurface;
		CSize m_sizeBmp;
		bool m_bEmpty:1;
		COLORREF m_clrTransparency;
		INT m_nUndoType;
	public:
		CBitmap m_bmpResizingCache;
		CSize m_sizeResizingCache;
		bool m_bSeparatedCacheState:1, m_bMoveErased:1, m_bNewUserSelection:1;
		INT m_nCountOfCompressedMovements;
		CRect m_rcSelection, m_rcSelectionTS;
		void Draw(
			CDC & dc,
			UINT nMagnification,
			CBitmap & bmpSelectionCache,
			CRect rcSelection,
			COLORREF clrCacheTransparent
			);
		void AttachBitmap( HBITMAP hBitmap );
		void SizeReset();
		CSize SizeGet() const;
		bool SizeSet(
			CSize _size,
			CBrush & brushFillSurface
			);
		bool AssignFromOther(
			const IMAGEEDITDATA &other,
			bool bAssignUndoType
			);
		void MoveFromOther(
			IMAGEEDITDATA &other,
			bool bMoveUndoType
			);
		bool IsEmpty() const;
		void Empty();
		INT UndoTypeGet() const
		{
			return m_nUndoType;
		}
		INT UndoTypeSet(
			INT nUndoType
			)
		{
			INT nUndoTypeOld = m_nUndoType;
			m_nUndoType = nUndoType;
			return nUndoTypeOld;
		}

		IMAGEEDITDATA();
		IMAGEEDITDATA(
			const IMAGEEDITDATA & other
			);
		~IMAGEEDITDATA();
		IMAGEEDITDATA & operator = ( const IMAGEEDITDATA & other );
		
		CBitmap & GetBmpSurface()
		{
			return m_bmpSurface;
		}
		const CBitmap & GetBmpSurface() const
		{
			return m_bmpSurface;
		}
		COLORREF TransparencyColorGet() const
		{
			return m_clrTransparency;
		}
		void TransparencyColorSet(
			COLORREF clrTransparency
			)
		{
			m_clrTransparency = clrTransparency;
		}
	}; // class IMAGEEDITDATA

protected:
	DWORD m_dwIedStyles;
	CArray < IMAGEEDITDATA *, IMAGEEDITDATA * > m_arrImgEditData;

	e_tool_t m_eToolSelected, m_eToolSelPrev, m_eToolTracked;
	bool m_bCancelingActions:1;
	INT m_nUndoDepthMax, m_nUndoPos;
	UINT m_nMagnification;
	UINT m_nTrackStep;
	CArray < CPoint, CPoint > m_arrTrackPoints;
	CRect m_rcTrackingSel;
	COLORREF m_clrForeground, m_clrBackground;
	INT m_nGripToolHT;
	CPoint m_ptPicMoveLastOffs;
	COLORREF m_clrLastCacheTransparency;
	bool m_bPresubclassDialogMode:1;
public:
	CExtScrollBar m_wndScrollBarH, m_wndScrollBarV;

// Operations
protected:
	bool m_bDirectCreateCall;
	virtual bool _CreateHelper();
public:
	bool Create(
		CWnd * pWndParent,
		DWORD dwIedStyles = __EIEDWS_DEFAULT_STYLES,
		DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,
		DWORD dwExStyle = 0L,
		UINT nDlgCtrlID = ((UINT)(__EXT_MFC_IDC_STATIC)),
		__EXT_MFC_SAFE_LPCTSTR strWindowText = NULL,
		CRect rc = CRect( 0, 0, 0, 0 )
		);
	INT IedDataEntry_GetCount() const
	{
		ASSERT_VALID( this );
		INT nCount = (INT)m_arrImgEditData.GetSize();
		return nCount;
	}
	const IMAGEEDITDATA * IedDataEntry_GetAt( INT nPos ) const
	{
		ASSERT_VALID( this );
		ASSERT( 0 <= nPos && nPos < m_arrImgEditData.GetSize() );
		const IMAGEEDITDATA * pData = m_arrImgEditData[nPos];
		ASSERT( pData != NULL );
		return pData;
	}
	IMAGEEDITDATA * IedDataEntry_GetAt( INT nPos )
	{
		ASSERT_VALID( this );
		ASSERT( 0 <= nPos && nPos < m_arrImgEditData.GetSize() );
		IMAGEEDITDATA * pData = m_arrImgEditData[nPos];
		ASSERT( pData != NULL );
		return pData;
	}
	CExtImageEditWnd::e_tool_t IedToolGet() const;
	virtual CExtImageEditWnd::e_tool_t IedToolSet(
		CExtImageEditWnd::e_tool_t eTool = CExtImageEditWnd::__IEDT_RECT_SELECTION
		);

	CSize IedSizeGet() const;
	virtual bool IedSizeSet(
		CSize _size,
		CBrush & brushFillSurface,
		bool bRedraw
		);
	bool IedIsTransparencyEnabled() const;
	COLORREF IedGetTransparencyColor() const;
	virtual void IedEnableTransparency(
		COLORREF clrTransparency = __IED_COLORREF_NO_TRANSPARENCY // disable
		);
	virtual DWORD IedGetStyle() const;
	virtual DWORD IedModifyStyle(
		DWORD dwStyleRemove,
		DWORD dwStyleAdd,
		bool bRepaint
		);
	bool IedIsEmpty() const;
	virtual void IedEmpty( bool bRedraw );
	INT IedUndoPosGet() const;
	INT IedUndoDepthGet() const;
	INT IedUndoDepthMaxGet() const;
	virtual void IedUndoDepthMaxSet(
		INT nUndoDepthMax = 1 // 1 - disable undo
		);
	virtual INT IedUndo( // returns positive number of undo - redo steps
		INT nSteps // if nSteps < 0 - redo
		);
	virtual void IedEmptyUndoBuffer();
	INT IedUndoTypeGet(
		INT nIndex = -1 // -1 - current position
		) const;
	
	virtual COLORREF IedColorForegroundSet( COLORREF clr );
	virtual COLORREF IedColorForegroundGet() const;
	virtual COLORREF IedColorBackgroundSet( COLORREF clr );
	virtual COLORREF IedColorBackgroundGet() const;

	virtual void IedPushUndo(
		INT nUndoType
		);

	virtual HBITMAP IedBitmapGet();
	virtual void IedBitmapSet(
		HBITMAP hBitmap,
		bool bRedraw = true
		);
	bool IedBitmapLoadResource(
		UINT nResourceID,
		bool bRedraw = true
		);
	virtual bool IedBitmapLoadResource(
		__EXT_MFC_SAFE_LPCTSTR sResourceID,
		bool bRedraw = true
		);

	UINT IedMagnificationGet() const;
	virtual void IedMagnificationSet( UINT nMagnification, bool bRedraw );

	virtual bool IedCopyToClipboard(
		bool bSelectionOnly = false
		) const;
	virtual bool IedCutToClipboard(
		bool bSelectionOnly = false,
		bool bRedrawWnd = true
		);
	virtual bool IedPasteFromClipboard(
		bool bRedrawWnd = true
		);
	virtual bool IedCanCopy(
		bool bSelectionOnly = false
		) const;
	virtual bool IedCanCut(
		bool bSelectionOnly = false
		) const;
	virtual bool IedCanPaste() const;
	HBITMAP IedSelectionGetHBITMAP(
		bool bSelectionOnly = false,
		bool bReturnClone = false
		) const;
	CRect IedSelectionRectGet(
		bool * p_bAllImage = NULL,
		bool * p_bSelectionExist = NULL
		) const;
	void IedSelectionRectSet(
		LPCRECT pRcSelection = NULL, // NULL - select all
		bool bRedraw = true
		);
	void IedRender( CDC & dc );

	void IedClear(
		bool bSelectionOnly = true,
		bool bRedraw = true,
		bool * p_bCanceledAction = NULL
		);

	static void stat_IedDrawToolLine(
		CExtImageEditWnd::e_tool_t eTool,
		CBitmap & bmpSurface,
		COLORREF clrPaint,
		CPoint ptFrom,
		CPoint ptTo
		);
	static void stat_IedDrawTool2dGeometry(
		CExtImageEditWnd::e_tool_t eTool,
		CBitmap & bmpSurface,
		COLORREF clrForward,
		COLORREF clrBackward,
		CPoint ptFrom,
		CPoint ptTo,
		bool bRightButton = false
		);
	static __EXT_MFC_SAFE_LPCTSTR stat_GetCurResFromToolHT(
		INT nToolHT
		);
	static HBRUSH stat_GetHalftoneLightBrush(
		int nLighterSpec
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtImageEditWnd)
	public:
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtImageEditWnd();

	virtual CSize OnSwGetTotalSize() const;
	virtual CSize OnSwGetPageSize( int nDirection ) const;
	virtual CSize OnSwGetLineSize( int nDirection ) const;
	virtual CPoint OnSwGetScrollPaintPos() const;
	virtual void OnSwPaint( CDC & dc );
	virtual void OnSwEnsurePointAvail( CPoint point );
	virtual bool OnSwDoScrollBy(
		CSize sizeScroll,
		bool bDoScroll = true
		);
	virtual bool OnSwDoMouseWheel(
		UINT fFlags,
		short zDelta,
		CPoint point
		);

	virtual void OnIedPaintGrid( CDC & dc );
	virtual void OnIedPaintTransparencyBk( CDC & dc );
	virtual void OnIedCancelActions(
		bool bPopTrackingAction
		);
	virtual void OnIedQueryAirbrushTimerParms(
		LPUINT p_nAirbrushTimerID,
		LPUINT p_nAirbrushTimerEllapse = NULL,
		CExtImageEditWnd::e_tool_t eTool = __IEDT_AIRBRUSH_VALUE_MIN,
		LPINT p_nRadius = NULL,
		LPINT p_nPixelsPerStep = NULL
		);
	
	virtual CPoint OnIedClientToPic( CPoint _point, bool bAdjust = true );
	virtual CRect OnIedClientToPic( CRect _rect, bool bAdjust = true );
	virtual CSize OnIedClientToPic( CSize _size );
	
	virtual CPoint OnIedPicToClient( CPoint _point, bool bAdjust = true );
	virtual CRect OnIedPicToClient( CRect _rect, bool bAdjust = true );
	virtual CSize OnIedPicToClient( CSize _size );

	virtual void OnIedPaintTrackingSelectionRect();
	virtual void OnIedPaintSelectionTools(
		CDC & dc,
		CRect _rect
		);
	virtual CSize OnIedMeasureToolSize() const;

	virtual bool OnIedSetCursor();

	virtual bool OnIedMouseClick(
		bool bMousePressed,
		UINT nFlags,
		CPoint point
		);
	virtual bool OnIedMouseMove(
		UINT nFlags,
		CPoint point
		);
	virtual void OnIedQuerySelParms(
		INT nMouseMsgFlags, // [in] WM_MOUSEMOVE message flags
		bool bAllImage, // [in] true - all image, false - selection
		bool * p_bStretch, // [out] true - stretch image content, false - only resize image (all image only)
		bool * p_bHalftone, // [out] true - use smooth stretching algorithm
		COLORREF * p_clrTransparent, // [out] ((COLORREF)(-1L)) - no transparent, other - color of transparent pixels when stretching (selection only)
		COLORREF * p_clrOuterParts // [out] color of outer parts (all image only)
		);

	virtual void OnIedActiveColorChange(
		bool bForeground, // true - foreground color, false - background color
		COLORREF clrOld,
		COLORREF clrNew
		);
	virtual void OnIedRectChange(
		const CRect & rcSelection,
		bool bTrackingSelection
		);

protected:
	virtual void _NotifyTrackPoints(
		CPoint pt0,
		CPoint pt1
		);
	virtual void _RequestFocus();
	virtual void _RedrawWnd();
	virtual INT _HitTestToolRects(
		CPoint ptCursor, // in client
		CRect rcSelection,
		__EXT_MFC_SAFE_LPCTSTR * p_lpszCursorResourceID
		);
	virtual void _CalcToolRects(
		CRect rcSelection,
		CRect * pRcTools // 8 items
		);
	virtual void _EmptyCache();
	virtual bool _OnKeyEsc();
	virtual bool _OnKeyDel();

protected:
	static bool g_bImageEditWndClassRegistered;
public:
	static bool RegisterImageEditWndClass();
	static HCURSOR g_hCursor;
	bool m_bEraseBkBeforeDraggingSelection:1, m_bEraseBkBeforeResizingSelection:1;
	bool m_bUseDocumentAreaBackground:1;

#ifdef _DEBUG
	virtual void AssertValid() const
	{
		CExtScrollWnd::AssertValid();
		ASSERT( m_nUndoDepthMax > 1 );
		INT nImgCount = (INT)m_arrImgEditData.GetSize();
		ASSERT( nImgCount >= 1 );
		ASSERT( 0 <= m_nUndoPos && m_nUndoPos < nImgCount );
		for( INT nImgIdx = 0; nImgIdx < nImgCount; nImgIdx++ )
		{
			IMAGEEDITDATA * pImgEditData =
				m_arrImgEditData[nImgIdx];
			ASSERT( pImgEditData != NULL );
		}
	}
	
	virtual void Dump( CDumpContext & dc ) const
	{
		CExtScrollWnd::Dump( dc );
	}
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtImageEditWnd)
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtImageEditWnd

#define __EXT_IMAGE_EDIT_WND_CLASS_NAME _T("ProfUIS-ImageEditWindow")

#endif // (!defined __EXT_MFC_NO_IMAGEEDITWND)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // (!defined __EXT_IMAGEEDITWND_H)
