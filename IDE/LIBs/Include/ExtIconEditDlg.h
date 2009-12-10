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

#if (!defined __EXT_ICONEDITDLG_H)
#define __EXT_ICONEDITDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined __EXT_MFC_NO_ICONEDITDLG)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_IMAGEEDITWND_H)
	#include <ExtImageEditWnd.h>
#endif

#if (!defined __EXT_COLORPALETTE_WND_H)
	#include <ExtColorPaletteWnd.h>
#endif

#if (!defined __EXT_TOOLCONTROLBAR_H)
	#include <ExtToolControlBar.h>
#endif

#if (!defined __EXT_RESIZABLE_DIALOG_H)
	#include <ExtResizableDialog.h>
#endif

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

#if (!defined __EXT_NC_FRAME_H)
	#include <ExtNcFrame.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtIconEditDlg dialog

#if (!defined __EXT_MFC_NO_NC_FRAME)
	#define __BASEOF_CExtIconEditDlg__ CExtNCW < CExtResizableDialog >
#else
	#define __BASEOF_CExtIconEditDlg__ CExtResizableDialog
#endif

#if _MFC_VER == 0x700
	#ifndef __EXT_PROFUIS_STATIC_LINK
		template class __PROF_UIS_API __BASEOF_CExtIconEditDlg__;
	#endif  // #ifndef __EXT_PROFUIS_STATIC_LINK
#endif

class __PROF_UIS_API CExtIconEditDlg
	: public __BASEOF_CExtIconEditDlg__
{
// Construction

	CExtImageEditWnd::e_tool_t
		m_eLastToolBrush, m_eLastToolAirBrush,
		m_eLastToolLine,
		m_eLastToolRectangle, m_eLastToolOutlinedRectangle,
		m_eLastToolRoundedRectangle, m_eLastToolOutlinedRoundedRectangle,
		m_eLastToolEllipse, m_eLastToolOutlinedEllipse;

public:
	DECLARE_DYNAMIC( CExtIconEditDlg );
	CExtIconEditDlg(CWnd* pParent = NULL);
	virtual ~CExtIconEditDlg();

	class __PROF_UIS_API CIconToolsToolbar : public CExtToolControlBar
	{
	public:
		CIconToolsToolbar();
		~CIconToolsToolbar();
		virtual bool IsLargeIcons() const;
	}; // class CIconToolsToolbar

	class __PROF_UIS_API CSurfaceEditor : public CExtImageEditWnd
	{
		void _TrackCommandMenu( UINT nCmdID );
		void _SyncViewerSurface();
	public:
		CSurfaceEditor();
		~CSurfaceEditor();
		virtual bool IedPasteFromClipboard(
			bool bRedrawWnd = true
			);
		virtual INT IedUndo(
			INT nSteps
			);
		virtual void OnIedRectChange(
			const CRect & rcSelection,
			bool bTrackingSelection
			);
		virtual void OnIedActiveColorChange(
			bool bForeground,
			COLORREF clrOld,
			COLORREF clrNew
			);
		virtual void OnIedPaintTransparencyBk( CDC & dc );
		virtual void OnIedQuerySelParms(
			INT nMouseMsgFlags,
			bool bAllImage,
			bool * p_bStretch,
			bool * p_bHalftone,
			COLORREF * p_clrTransparent,
			COLORREF * p_clrOuterParts
			);
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
		void _TraceMousePoint();
		CPoint m_ptMouseTrace;
		CSize m_sizeSelTrace;
		HBRUSH m_hBrushTransparentBk;
		bool m_bSyncingColorForeground:1, m_bSyncingColorBackground:1;
		friend class CExtIconEditDlg;
	}; // class CSurfaceEditor

	class __PROF_UIS_API CSurfaceViewer : public CExtImageEditWnd
	{
	public:
		CSurfaceViewer();
		~CSurfaceViewer();
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	protected:
		friend class CExtIconEditDlg;
	}; // class CSurfaceViewer

	class __PROF_UIS_API CPaletteViewer : public CExtColorPaletteWnd
	{
	public:
		CPaletteViewer();
		~CPaletteViewer();
		virtual bool OnCpwActiveColorChange(
			bool bForeground,
			COLORREF clrOld,
			COLORREF clrNew,
			bool bRepaint = true
			);
		virtual void OnCpwPaintItem(
			CDC & dc,
			INT nItemID,
			const CRect & rcItem,
			COLORREF clrItem,
			COLORREF clrBorder
			);
		virtual CExtSafeString OnCpwGetTooltipText(
			INT nItem,
			CPoint ptClient,
			const CRect & rcItem,
			COLORREF clr
			) const;
	protected:
		HBRUSH m_hBrushTransparentBk;
		bool m_bSyncingColorForeground:1, m_bSyncingColorBackground:1;
		COLORREF m_clrTransparentOption;
		friend class CExtIconEditDlg;
	}; // class CPaletteViewer

	class __PROF_UIS_API CInfoWnd : public CWnd
	{
	public:
		CExtCmdIcon m_icon;
		CExtSafeString m_strInitialText;
		CInfoWnd(
			UINT nIdResource = 0,
			INT cx = 16,
			INT cy = 16
			);
		~CInfoWnd();
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual void PreSubclassWindow();
	}; // class CInfoWnd

	CExtCmdIcon m_icon;

	CBitmap m_bmpTransparentBk;
	CBrush m_brushTransparentBk;

	CSurfaceEditor m_wndIconEditor;
	CSurfaceViewer m_wndIconViewer;
	CPaletteViewer m_wndColorPalette;
	CIconToolsToolbar m_wndIconTools;
	CInfoWnd m_wndInfoSelectionSize;
	CInfoWnd m_wndInfoMousePos;
	
	DWORD
		m_dwIconEditorStyles,
		m_dwIconViewerStyles,
		m_dwColorPaletteStyles;

	bool m_bEatTransparentColor:1;

	CExtSafeString
		m_strAlternateCaption,
		m_strProfileSection,
		m_strProfileEntryWindow,
		m_strProfileEntryIconEditor;

public:

	virtual void OnAskSaveChanges(
		bool & bAllowCloseDialog,
		bool & bReturnOkCode
		);
	virtual CExtSafeString OnGetUndoTypeDescription(
		INT nUndoType
		);
	virtual void OnUpdateInfoWindows();
	virtual void SaveWindowRect() const;
	virtual void LoadWindowRect(
		bool bForceHideWindow = false
		);

// Dialog Data
	//{{AFX_DATA(CExtIconEditDlg)
	CExtButton	m_BtnCancel;
	CExtButton	m_BtnOK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtIconEditDlg)
	public:
	virtual __EXT_MFC_INT_PTR DoModal();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	virtual bool OnQueryAutomaticRTLTransform() const
	{
		return true;
	}

// Implementation
protected:
	HICON m_hIcon;
	virtual void OnOK();
	virtual void OnCancel();

	// Generated message map functions
	//{{AFX_MSG(CExtIconEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnIedToggleGrid();
	afx_msg void OnUpdateIedToggleGrid(CCmdUI* pCmdUI);
	afx_msg void OnIedClear();
	//}}AFX_MSG
	afx_msg void OnIedEditUndo();
	afx_msg void OnUpdateIedEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnIedEditRedo();
	afx_msg void OnUpdateIedEditRedo(CCmdUI* pCmdUI);

	afx_msg void OnIedEditSelectAll();

	afx_msg void OnIedEditCopy();
	afx_msg void OnUpdateIedEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnIedEditCut();
	afx_msg void OnUpdateIedEditCut(CCmdUI* pCmdUI);
	afx_msg void OnIedEditPaste();
	afx_msg void OnUpdateIedEditPaste(CCmdUI* pCmdUI);

	afx_msg void OnIedToolRectSelection();
	afx_msg void OnUpdateIedToolRectSelection(CCmdUI* pCmdUI);
	
	afx_msg void OnIedToolColorSelection();
	afx_msg void OnUpdateIedToolColorSelection(CCmdUI* pCmdUI);

	afx_msg void OnIedToolFill();
	afx_msg void OnUpdateIedToolFill(CCmdUI* pCmdUI);

	afx_msg void OnIedToolMagnification();
	afx_msg void OnUpdateIedToolMagnification(CCmdUI* pCmdUI);
	afx_msg void OnIedCmdRangeMagnification( UINT nCmdID );
	afx_msg void OnUpdateIedRangeMagnification(CCmdUI* pCmdUI);

	afx_msg void OnIedToolPencil();
	afx_msg void OnUpdateIedToolPencil(CCmdUI* pCmdUI);

	afx_msg void OnIedToolBrush();
	afx_msg void OnUpdateIedToolBrush(CCmdUI* pCmdUI);
	afx_msg void OnIedCmdRangeBrush( UINT nCmdID );
	afx_msg void OnUpdateIedToolBrushRange(CCmdUI* pCmdUI);

	afx_msg void OnIedToolAirBrush();
	afx_msg void OnUpdateIedToolAirBrush(CCmdUI* pCmdUI);
	afx_msg void OnIedCmdRangeAirBrush( UINT nCmdID );
	afx_msg void OnUpdateIedToolAirBrushRange(CCmdUI* pCmdUI);

	afx_msg void OnIedToolLine();
	afx_msg void OnUpdateIedToolLine(CCmdUI* pCmdUI);
	afx_msg void OnIedCmdRangeLine( UINT nCmdID );
	afx_msg void OnUpdateIedToolLineRange(CCmdUI* pCmdUI);

	afx_msg void OnIedToolText();
	afx_msg void OnUpdateIedToolText(CCmdUI* pCmdUI);
	afx_msg void OnIedCmdRangeText( UINT nCmdID );
	afx_msg void OnUpdateIedToolTextRange(CCmdUI* pCmdUI);

	afx_msg void OnIedToolRectangle();
	afx_msg void OnUpdateIedToolRectangle(CCmdUI* pCmdUI);
	afx_msg void OnIedCmdRangeRectangle( UINT nCmdID );
	afx_msg void OnUpdateIedToolRectangleRange(CCmdUI* pCmdUI);

	afx_msg void OnIedToolOutlinedRectangle();
	afx_msg void OnUpdateIedToolOutlinedRectangle(CCmdUI* pCmdUI);
	afx_msg void OnIedCmdRangeOutlinedRectangle( UINT nCmdID );
	afx_msg void OnUpdateIedToolOutlinedRectangleRange(CCmdUI* pCmdUI);

	afx_msg void OnIedToolFilledRectangle();
	afx_msg void OnUpdateIedToolFilledRectangle(CCmdUI* pCmdUI);

	afx_msg void OnIedToolRoundedRectangle();
	afx_msg void OnUpdateIedToolRoundedRectangle(CCmdUI* pCmdUI);
	afx_msg void OnIedCmdRangeRoundedRectangle( UINT nCmdID );
	afx_msg void OnUpdateIedToolRoundedRectangleRange(CCmdUI* pCmdUI);

	afx_msg void OnIedToolOutlinedRoundedRectangle();
	afx_msg void OnUpdateIedToolOutlinedRoundedRectangle(CCmdUI* pCmdUI);
	afx_msg void OnIedCmdRangeOutlinedRoundedRectangle( UINT nCmdID );
	afx_msg void OnUpdateIedToolOutlinedRoundedRectangleRange(CCmdUI* pCmdUI);

	afx_msg void OnIedToolFilledRoundedRectangle();
	afx_msg void OnUpdateIedToolFilledRoundedRectangle(CCmdUI* pCmdUI);

	afx_msg void OnIedToolEllipse();
	afx_msg void OnUpdateIedToolEllipse(CCmdUI* pCmdUI);
	afx_msg void OnIedCmdRangeEllipse( UINT nCmdID );
	afx_msg void OnUpdateIedToolEllipseRange(CCmdUI* pCmdUI);

	afx_msg void OnIedToolOutlinedEllipse();
	afx_msg void OnUpdateIedToolOutlinedEllipse(CCmdUI* pCmdUI);
	afx_msg void OnIedCmdRangeOutlinedEllipse( UINT nCmdID );
	afx_msg void OnUpdateIedToolOutlinedEllipseRange(CCmdUI* pCmdUI);

	afx_msg void OnIedToolFilledEllipse();
	afx_msg void OnUpdateIedToolFilledEllipse(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
}; // class CExtIconEditDlg

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // (!defined __EXT_MFC_NO_ICONEDITDLG)

#endif // (!defined __EXT_ICONEDITDLG_H)
