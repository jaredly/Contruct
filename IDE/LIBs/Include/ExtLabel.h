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

#if (!defined __EXT_LABEL_H)
#define __EXT_LABEL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

class CExtPopupMenuTipWnd;

/////////////////////////////////////////////////////////////////////////////
// CExtLabel window

class __PROF_UIS_API CExtLabel
	: public CStatic
	, public CExtPmBridge
{
public:
	DECLARE_DYNCREATE( CExtLabel );
	DECLARE_CExtPmBridge_MEMBERS( CExtLabel );

	CExtLabel();

// Attributes
public:
	CRect m_rcLabelTextMargins;
	
// Operations
public:
	void SetFontBold( bool bSet = true );
	void SetFontItalic( bool bSet = true );
	void SetFontUnderline( bool bSet = true );
	void SetFontStrikeOut( bool bSet = true );
	void SetBkColor( COLORREF clrBk );
	void SetTextColor(
		bool bEnabled,
		COLORREF clrText = COLORREF(-1L)
		);
	
	bool GetFontBold();
	bool GetFontItalic();
	bool GetFontUnderline();
	bool GetFontStrikeOut();
	COLORREF GetBkColor() const;
	COLORREF GetTextColor(
		bool bEnabled
		) const;
	
	enum e_ImageMode_t
	{
		eAlign = 0,		// The image is aligned according to the text aligning styles (the SS_LEFT, SS_CENTER and SS_RIGHT styles)
		eTile = 1,		// The image is repeated until the entire available area is filled. 
		eStretch = 2,	// The image is stretched to fit all the available area. 
	};

	e_ImageMode_t SetImageMode(
		e_ImageMode_t eImageMode
		);
	e_ImageMode_t GetImageMode() const;

	const CExtBitmap * GetBitmapEx() const;
	CExtBitmap * GetBitmapEx();
	bool SetBitmapEx( 
		CExtBitmap * pBmp 
		);

	void SetTooltipText( int nId );
	void SetTooltipText( __EXT_MFC_SAFE_LPCTSTR sText );
	INT m_nAdvancedTipStyle; // CExtPopupMenuTipWnd::__ETS_RECTANGLE_NO_ICON
	bool m_bHelper_AutoAdjust_SS_NOTIFY_style:1;
	static bool g_bHelper_AutoAdjust_SS_NOTIFY_style;
protected:
	CExtSafeString m_strToolTipText;
	virtual CExtPopupMenuTipWnd * OnAdvancedPopupMenuTipWndGet() const;
	virtual void OnAdvancedPopupMenuTipWndDisplay(
		CExtPopupMenuTipWnd & _ATTW,
		const RECT & rcExcludeArea
		) const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtLabel)
protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtLabel();
	virtual void OnEraseBackground(
		CDC & dc,
		const CRect & rcClient
		);
	virtual void OnDrawLabelText(
		CDC & dc,
		const RECT & rcText,
		__EXT_MFC_SAFE_LPCTSTR strText,
		DWORD dwDrawTextFlags,
		bool bEnabled
		);
	virtual COLORREF OnQueryTextColor(
		bool bEnabled
		) const;

protected:
	bool m_bFontBold:1;
	bool m_bFontItalic:1;
	bool m_bFontUnderline:1;
	bool m_bFontStrikeOut:1;
	COLORREF m_clrTextNormal, m_clrTextDisabled;
	COLORREF m_clrBackground;

	CExtSafeString m_sText;
	bool m_bInitText:1;

	CExtBitmap m_bmp;
	e_ImageMode_t m_eImageMode;

	virtual HFONT OnQueryFont() const;
	virtual void DoPaint( 
		CDC * pDC,
		CRect & rcClient
		);

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtLabel)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtLabel

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // (!defined __EXT_LABEL_H)
