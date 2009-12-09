////////////////////////////////////////////////////////////////////////////
// File:	CtrlMessageBar.h
// Version:	1.0
// Created:	08-Oct-2008
//
// Author:	Paul S. Vickery
// E-mail:	paul@vickeryhome.freeserve.co.uk
//
// Class to add a message-bar to (virtually) any control.
//
// You are free to use or modify this code, with no restrictions, other than
// you continue to acknowledge me as the original author in this source code,
// or any code derived from it.
//
// If you use this code, or use it as a base for your own code, it would be 
// nice to hear from you simply so I know it's not been a waste of time!
//
// Copyright (c) 2005-2008 Paul S. Vickery
//
////////////////////////////////////////////////////////////////////////////
// Version History:
//
// Version 1.0 - 08-Oct-2008
// =========================
// Initial version
// 
////////////////////////////////////////////////////////////////////////////
// PLEASE LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////////
// CCtrlMessageBarDblSubclassWnd based on CDblSubclassWnd class             //
// Written By Ralph Varjabedian, 08/02/2002, software@sandmik.net           //
// (see http://www.codeproject.com/KB/miscctrl/dblsubclasswnd.aspx)         //
// Stripped down by Paul S. Vickery for use in CCtrlMessageBar              //
//////////////////////////////////////////////////////////////////////////////

// special hit-test codes for message bar area
#define HTMESSAGEBAR	  0x100
#define HTMESSAGEBARCLOSE 0x101

#ifndef CLR_DEFAULT
#define CLR_DEFAULT             0xFF000000L
#endif // CLR_DEFAULT

class CCtrlMessageBar;

/* return FALSE to show default menu */
typedef BOOL (*PFNSHOWMENUCALLBACK)(CCtrlMessageBar* pBar, CWnd* pCtrl, UINT nHitTest, CPoint point);

// class to add a message bar strip to the top of a control window
class CCtrlMessageBar : public CObject
{
  DECLARE_DYNAMIC(CCtrlMessageBar)
public:
  CCtrlMessageBar();
  virtual ~CCtrlMessageBar();

  BOOL Attach(CWnd* pCtrl);
  BOOL Attach(CWnd& ctrl) { return Attach(&ctrl); }
  BOOL Attach(CWnd* pDlg, UINT nID);
  CWnd* Detach();

  CWnd* GetCtrl() const { return m_pCtrlMessageBarThis; }
  BOOL IsAttached() const { return GetCtrl() != NULL; }

  void SetImageList(CImageList* piml);

  CFont font;

  virtual void SetText(LPCTSTR lpszText, UINT nImage = -1, BOOL bShow = TRUE);
  virtual CString GetText() const { return m_sMessageBarText; }

  virtual void Show(BOOL bShow = TRUE);
  virtual BOOL IsShown() const { return m_bShowMessageBar; }

  void SetImage(UINT nImage, BOOL bShow = TRUE);
  UINT GetImage() const { return m_nMessageBarImage; }

  CRect GetRect(BOOL bDrawing = FALSE) const;
  CRect GetCloseButtonRect(CRect& rc) const;
  UINT GetMessageHeight() const { return m_nMessageBarHeight; }

  void SetCloseButton(BOOL bShow = TRUE) { m_bMessageBarCloseButton = bShow; }
  BOOL GetCloseButton() const { return m_bMessageBarCloseButton; }

  void SetWrapText(BOOL bWrap = TRUE) { m_bWrapMessageBarText = bWrap; Show(m_bShowMessageBar); }
  BOOL GetWrapText() const { return m_bWrapMessageBarText; }

  void SetHighlightOnMouseOver(BOOL bHighlight = TRUE) { m_bHighlightOnMouseOver = bHighlight; }
  BOOL GetHighlightOnMouseOver() const { return m_bHighlightOnMouseOver; }

  void SetColours(COLORREF crBackgroundNormal = CLR_DEFAULT, COLORREF crTextNormal = CLR_DEFAULT, COLORREF crBackgroundHilite = CLR_DEFAULT, COLORREF crTextHilite = CLR_DEFAULT);
  void GetColours(COLORREF& crBackgroundNormal, COLORREF& crTextNormal, COLORREF& crBackgroundHilite, COLORREF& crTextHilite) const;

  void SetResize(BOOL bResize = TRUE) { m_bResize = bResize; }
  BOOL GetResize() const { return m_bResize; }

  static CCtrlMessageBar* GetMessageBarCtrl(CWnd* pCtrl);

  void SetShowMenuCallback(PFNSHOWMENUCALLBACK pfnShowMenuCallback) { m_pfnShowMenuCallback = pfnShowMenuCallback; }

  LRESULT DoWindowProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT lResult);

protected:
  void DoNcPaint();
  void DoNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
  UINT DoNcHitTest(CPoint point, UINT nCode);
  void DoNcMouseButtonMsg(UINT nMsg, UINT nHitTest, CPoint point);

  virtual void GetCtrlMessageBarOffset(CRect& rcOffset) const { rcOffset.SetRectEmpty(); }

  BOOL m_bShowMessageBar;
  CString m_sMessageBarText;
  UINT m_nMessageBarHeight;
  CImageList m_imlMessageBar;
  UINT m_nMessageBarImage;
  BOOL m_bMessageBarCloseButton;
  BOOL m_bWrapMessageBarText;
  BOOL m_bResize;
  PFNSHOWMENUCALLBACK m_pfnShowMenuCallback;

  BOOL m_bTextTruncated;
  BOOL m_bHighlightOnMouseOver;
  BOOL m_bMouseHover;

  COLORREF m_crBackgroundNormal;
  COLORREF m_crTextNormal;
  COLORREF m_crBackgroundHilite;
  COLORREF m_crTextHilite;

  CToolTipCtrl m_tipMessageBar;

public:
  class _CCtrlMessageBarDblSubclassWnd  
  {
  public:
	  static BOOL SubclassWindow(HWND hWnd, CCtrlMessageBar* pBar = NULL);
	  static void UnSubclassWindow(HWND hWnd);
	  static BOOL IsSubclassed(HWND hWnd);
  private:
	  friend LRESULT AFXAPI OurCallWndProc(CWnd* pWnd, HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	  // _CTempWndSubclass window
  public:
	  class _CTempWndSubclass : public CWnd
	  {
	    DECLARE_DYNAMIC(_CTempWndSubclass)
	  // Construction
	  public:
		  _CTempWndSubclass()
			  { InterlockedIncrement(&_m_lObjectCount); }
		  static long _m_lObjectCount;
		  virtual ~_CTempWndSubclass()
			  { InterlockedDecrement(&_m_lObjectCount); }
		  void SetMessageBar(CCtrlMessageBar* pBar) { m_pBar = pBar; }
		  CCtrlMessageBar* GetMessageBar() const { return m_pBar; }
	  protected:
		  CCtrlMessageBar* m_pBar;
		  afx_msg void OnNcDestroy()
			  { CWnd::OnNcDestroy(); delete this; /* auto delete the object */ }
		  DECLARE_MESSAGE_MAP()
	  };
  };

private:
  CWnd* m_pCtrlMessageBarThis;  // the ctrl we are supporting
};

typedef CCtrlMessageBar::_CCtrlMessageBarDblSubclassWnd CCtrlMessageBarDblSubclassWnd;
typedef CCtrlMessageBar::_CCtrlMessageBarDblSubclassWnd::_CTempWndSubclass _CTempWndSubclass;

