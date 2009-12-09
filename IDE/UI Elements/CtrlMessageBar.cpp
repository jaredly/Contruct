////////////////////////////////////////////////////////////////////////////
// File:	CtrlMessageBar.cpp
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

#include "stdafx.h"
#include "CtrlMessageBar.h"


#if _MFC_VER >= 0x0700
  #pragma warning( disable : 4312 ) // 'variable' : conversion from 'type' to 'type' of greater size
  #pragma warning( disable : 4311 )  // 'variable' : pointer truncation from 'type' to 'type'
#endif

#define ID_TOOLTIP 123

#ifndef WM_QUERYAFXWNDPROC
  #define WM_QUERYAFXWNDPROC  0x0360  
#endif

static const UINT s_nMsgGetMessageBarHeight = RegisterWindowMessage(_T("CtrlMessageBar_GetMessageBarHeight"));

static const TCHAR szInstancePropertyName[] = _T("CtrlMessageBarInstance");

IMPLEMENT_DYNAMIC(_CTempWndSubclass, CWnd)

static LRESULT CALLBACK _PostSubclassHeaderWndProc(
	CWnd* pWnd, HWND /*hWnd*/, UINT nMsg, WPARAM wParam, LPARAM lParam, LRESULT lResult)
{
  if (pWnd != NULL && pWnd->m_hWnd != NULL && ::IsWindow(pWnd->m_hWnd))
  {
    if (nMsg == HDM_LAYOUT)
    {
      int nExtraHeight = 0;
      // if the parent window is a CListCtrl, then this is the list's header
      // so we need to find out the height of the list's message bar
      // so we can adjust the header layout results
      CListCtrl* pListCtrl = DYNAMIC_DOWNCAST(CListCtrl, pWnd->GetParent());
      if (pListCtrl != NULL)
	nExtraHeight = (int)pListCtrl->SendMessage(s_nMsgGetMessageBarHeight);
      LPHDLAYOUT lphdl = (LPHDLAYOUT)lParam;
      lphdl->prc->bottom -= nExtraHeight;
    }

    // TODO: if the control that the message bar is added to has its own idea of what to put 
    // in the non-client area, then code may need to be added in here to tell it to use a 
    // different area from the one it has calculated for itself, as is the case above with 
    // list controls, which use the non-client area to position the header, and send an 
    // HDM_LAYOUT message which allows us to re-position the list's idea of where the header 
    // should be.

    CCtrlMessageBar* pBar = CCtrlMessageBar::GetMessageBarCtrl(pWnd);
    if (pBar != NULL)
      lResult = pBar->DoWindowProc(nMsg, wParam, lParam, lResult);
  }
  return lResult;
}

LRESULT AFXAPI OurCallWndProc(CWnd* pWnd, HWND hWnd, UINT nMsg,	WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult = AfxCallWndProc(pWnd, hWnd, nMsg, wParam, lParam);
  lResult = _PostSubclassHeaderWndProc(pWnd, hWnd, nMsg, wParam, lParam, lResult);
  return lResult;
}

LRESULT CALLBACK OurWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
  // special message which identifies the window as using AfxWndProc
  if (nMsg == WM_QUERYAFXWNDPROC)
    return 1;

  // all other messages route through message map
  CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);

  ASSERT(pWnd != NULL);
  ASSERT(pWnd->m_hWnd == hWnd);

  return OurCallWndProc(pWnd, hWnd, nMsg, wParam, lParam);
}

BOOL CCtrlMessageBarDblSubclassWnd::SubclassWindow(HWND hWnd, CCtrlMessageBar* pBar/*=NULL*/)
{
  if (hWnd == NULL)
    return FALSE;
  CWnd* pWnd = CWnd::FromHandlePermanent(hWnd); // if not subclassed once, let us subclass.
  if (pWnd == NULL)
  {
    _CTempWndSubclass *pNewWnd = new _CTempWndSubclass;
    ASSERT(pNewWnd != NULL);
    VERIFY(pNewWnd->SubclassWindow(hWnd));
    ASSERT(CWnd::FromHandlePermanent(hWnd) != NULL);
  }
  if (IsSubclassed(hWnd)) // we've already subclassed it
    return TRUE;
  // attach the bar to the window, using properties
  SetProp(hWnd, szInstancePropertyName, (HANDLE)pBar);
#ifdef _DEBUG
  WNDPROC oldWndProc = (WNDPROC)
#endif // _DEBUG
      ::SetWindowLong(hWnd, GWL_WNDPROC, (DWORD)OurWndProc);
  ASSERT(oldWndProc == AfxGetAfxWndProc());
  ASSERT(oldWndProc != OurWndProc);
  return TRUE;
}

void CCtrlMessageBarDblSubclassWnd::UnSubclassWindow(HWND hWnd)
{
  if (hWnd == NULL)
    return;
  if (IsSubclassed(hWnd))
    SetWindowLong(hWnd, GWL_WNDPROC, (LONG)AfxGetAfxWndProc());
}

BOOL CCtrlMessageBarDblSubclassWnd::IsSubclassed(HWND hWnd)
{
  ASSERT(::IsWindow(hWnd));
  WNDPROC WndProc = (WNDPROC)::GetWindowLong(hWnd, GWL_WNDPROC);
  return (WndProc == OurWndProc); // subclassed by us
}

/////////////////////////////////////////////////////////////////////////////
// _CTempWndSubclass

long _CTempWndSubclass::_m_lObjectCount = 0;

BEGIN_MESSAGE_MAP(_CTempWndSubclass, CWnd)
	//{{AFX_MSG_MAP(_CTempWndSubclass)
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#if _MFC_VER >= 0x0700
  #pragma warning( default : 4312 )
  #pragma warning( default : 4311 )
#endif

/////////////////////////////////////////////////////////////////////////////
// CCtrlMessageBar

static LPCTSTR const CLOSE_BUTTON_FONT = _T("Marlett");
static UINT const CLOSE_BUTTON_FONT_SIZE = 90;	// 9 pt
static LPCTSTR const CLOSE_BUTTON_CHAR = _T("r"); // close button 'X' in Marlett font
static UINT const CLOSE_BUTTON_DRAW_FLAGS = (DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

static const DWORD s_dwSWPFlags = SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE/* | SWP_SHOWWINDOW*/;

CCtrlMessageBar::CCtrlMessageBar()
{
  m_pCtrlMessageBarThis = NULL;
  m_bShowMessageBar = FALSE;
  m_nMessageBarHeight = 0;
  m_nMessageBarImage = (UINT)-1;
  m_bMessageBarCloseButton = TRUE;
  m_bWrapMessageBarText = FALSE;
  m_bTextTruncated = FALSE;
  m_bHighlightOnMouseOver = FALSE;
  m_bMouseHover = FALSE;
  m_crBackgroundNormal = CLR_DEFAULT;
  m_crTextNormal = CLR_DEFAULT;
  m_crBackgroundHilite = CLR_DEFAULT;
  m_crTextHilite = CLR_DEFAULT;
  m_bResize = FALSE;
  m_pfnShowMenuCallback = NULL;
  	font.CreateFont(14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
}

CCtrlMessageBar::~CCtrlMessageBar()
{
  m_imlMessageBar.DeleteImageList();
}

// CCtrlMessageBar member functions

BOOL CCtrlMessageBar::Attach(CWnd* pCtrl)
{
  m_pCtrlMessageBarThis = pCtrl;
  if (pCtrl != NULL && pCtrl->m_hWnd != NULL && ! CCtrlMessageBarDblSubclassWnd::IsSubclassed(pCtrl->m_hWnd))
    return CCtrlMessageBarDblSubclassWnd::SubclassWindow(pCtrl->m_hWnd, this);
  return FALSE;
}

BOOL CCtrlMessageBar::Attach(CWnd* pDlg, UINT nID)
{
  if (pDlg != NULL && pDlg->m_hWnd != NULL && ::IsWindow(pDlg->m_hWnd))
    return Attach(pDlg->GetDlgItem(nID));
  return FALSE;
}

CWnd* CCtrlMessageBar::Detach()
{
  CWnd* pCtrl = m_pCtrlMessageBarThis;
  if (m_pCtrlMessageBarThis != NULL && m_pCtrlMessageBarThis->m_hWnd != NULL)
  {
    // remove message bar from control, then un-subclass it
    Show(FALSE);
    CCtrlMessageBarDblSubclassWnd::UnSubclassWindow(m_pCtrlMessageBarThis->m_hWnd);
  }
  m_pCtrlMessageBarThis = NULL;
  return pCtrl;
}

IMPLEMENT_DYNAMIC(CCtrlMessageBar, CObject)

/*static*/CCtrlMessageBar* CCtrlMessageBar::GetMessageBarCtrl(CWnd* pCtrl)
{
  // get the message bar from the control, if it has one
  // if this wasn't set, or it's not a valid CCtrlMessageBar pointer, 
  // then it will return NULL
  CCtrlMessageBar* pBar = (CCtrlMessageBar*)::GetProp(pCtrl->m_hWnd, szInstancePropertyName);
  try {
    if (! AfxIsValidAddress(pBar, sizeof(CObject)) || 
	! pBar->IsKindOf(RUNTIME_CLASS(CCtrlMessageBar)))
      pBar = NULL;
  } catch(...) {
    pBar = NULL;
  }
  return pBar;
}

CRect CCtrlMessageBar::GetRect(BOOL bDrawing/*=FALSE*/) const
{
  // for drawing, the co-ords need to be relative to the 
  // top-left of the window. else get it in client co-ords
  CRect rc, rcClient;
  m_pCtrlMessageBarThis->GetWindowRect(rc);
  m_pCtrlMessageBarThis->ScreenToClient(rc);
  m_pCtrlMessageBarThis->GetClientRect(rcClient);
  if (bDrawing)  // we are drawing to the window dc, so adjust the co-ords
    rcClient.OffsetRect(-rc.left, -rc.top);
  rc.OffsetRect(-rc.left, -rc.top);
  CRect rcOffset;
  GetCtrlMessageBarOffset(rcOffset);
  rc.left = rcOffset.left + rcClient.left;  // take off left border
  rc.right -= rcClient.left;	// take off right border (assumes right and left are equal)
  rc.right += rcOffset.right;
  rc.top = rcClient.top - m_nMessageBarHeight;  // take off top border
  rc.top += rcOffset.top;
  rc.bottom = rc.top + m_nMessageBarHeight; // set the height to the message bar height
  rc.bottom += rcOffset.bottom;
  if (! bDrawing)
  {
    rc.right -= 2;
    rc.bottom -= 2;
  }
  return rc;
}

CRect CCtrlMessageBar::GetCloseButtonRect(CRect& rc) const
{
  // given the message bar rect, return the close button rect
  if (! m_bMessageBarCloseButton)
    return CRect(rc.right, rc.top, rc.right, rc.top);
  CRect rcClose(rc);
  CDC* pDC = m_pCtrlMessageBarThis->GetDC();
  CFont font;
  font.CreatePointFont(CLOSE_BUTTON_FONT_SIZE, CLOSE_BUTTON_FONT, pDC);
  CFont* pFontOld = pDC->SelectObject(&font);
  pDC->DrawText(CLOSE_BUTTON_CHAR, rcClose, DT_CALCRECT | CLOSE_BUTTON_DRAW_FLAGS);
  pDC->SelectObject(pFontOld);
  rcClose.bottom += 1 + 1;
  rcClose.right += 2 + 2;
  // line up the top-right of the close button with the top-right of the message bar
  rcClose.OffsetRect(rc.right - rcClose.right, 0);
  // adjust the message bar text area
  rc.right = rcClose.left - 2;
  return rcClose;
}

void CCtrlMessageBar::SetImageList(CImageList* piml)
{
  m_imlMessageBar.DeleteImageList();
  if (piml != NULL)
    m_imlMessageBar.Create(piml);
}

void CCtrlMessageBar::SetText(LPCTSTR lpszText, UINT nImage/*=-1*/, BOOL bShow/*=TRUE*/)
{
  m_sMessageBarText = lpszText;
  m_nMessageBarImage = nImage;
  Show(bShow);
}

void CCtrlMessageBar::SetImage(UINT nImage, BOOL bShow/*=TRUE*/)
{
  m_nMessageBarImage = nImage;
  Show(bShow);
}

void CCtrlMessageBar::SetColours(COLORREF crBackgroundNormal/*=CLR_DEFAULT*/, 
				 COLORREF crTextNormal/*=CLR_DEFAULT*/, 
				 COLORREF crBackgroundHilite/*=CLR_DEFAULT*/, 
				 COLORREF crTextHilite/*=CLR_DEFAULT*/)
{
  m_crBackgroundNormal = crBackgroundNormal;
  m_crTextNormal = crTextNormal;
  m_crBackgroundHilite = crBackgroundHilite;
  m_crTextHilite = crTextHilite;
  m_pCtrlMessageBarThis->SetWindowPos(NULL, 0, 0, 0, 0, s_dwSWPFlags);
}

void CCtrlMessageBar::GetColours(COLORREF& crBackgroundNormal, 
				 COLORREF& crTextNormal, 
				 COLORREF& crBackgroundHilite, 
				 COLORREF& crTextHilite) const
{
  crBackgroundNormal = m_crBackgroundNormal;
  crTextNormal = m_crTextNormal;
  crBackgroundHilite = m_crBackgroundHilite;
  crTextHilite = m_crTextHilite;
}

void CCtrlMessageBar::Show(BOOL bShow/*=TRUE*/)
{
  _ASSERTE(m_pCtrlMessageBarThis != NULL);
  if (m_pCtrlMessageBarThis != NULL)
  {
    if (m_bResize)
    {
      UINT nMsgHeight = 0;
      CRect rcCtrl;
      m_pCtrlMessageBarThis->GetWindowRect(&rcCtrl);
      // we need to resize the dialog to make it fit
      if (bShow && ! IsShown())
      {
	m_bShowMessageBar = bShow;
	// use SetWindowPos to make sure the frame gets re-calc'd
	m_pCtrlMessageBarThis->SetWindowPos(NULL, 0, 0, 0, 0, s_dwSWPFlags);
	nMsgHeight = GetMessageHeight();
	rcCtrl.bottom += nMsgHeight;
      }
      else if (! bShow && IsShown())
      {
	nMsgHeight = GetMessageHeight();
	m_bShowMessageBar = bShow;
	// use SetWindowPos to make sure the frame gets re-calc'd
	m_pCtrlMessageBarThis->SetWindowPos(NULL, 0, 0, 0, 0, s_dwSWPFlags);
	rcCtrl.bottom -= nMsgHeight;
      }
      if (m_pCtrlMessageBarThis->GetStyle() & WS_CHILD && m_pCtrlMessageBarThis->GetParent() != NULL)
	m_pCtrlMessageBarThis->GetParent()->ScreenToClient(&rcCtrl);
      m_pCtrlMessageBarThis->MoveWindow(rcCtrl);
    }
    m_bShowMessageBar = bShow;

    // use SetWindowPos to make sure the frame gets re-calc'd
    m_pCtrlMessageBarThis->SetWindowPos(NULL, 0, 0, 0, 0, s_dwSWPFlags);
    // ListView controls don't redraw properly when the bar is hidden
    // so update its items
    CListCtrl* pListCtrl = DYNAMIC_DOWNCAST(CListCtrl, m_pCtrlMessageBarThis);
    if (pListCtrl != NULL)
      pListCtrl->Update(-1);
    // make sure the client area gets re-painted
    m_pCtrlMessageBarThis->Invalidate();
  }
}

static const DWORD s_dwDrawTextFlags = DT_SINGLELINE | DT_NOPREFIX | DT_LEFT | DT_END_ELLIPSIS;
static const DWORD s_dwDrawTextFlagsWrap = DT_WORDBREAK | DT_NOPREFIX | DT_LEFT;

#ifndef DFCS_TRANSPARENT
#define DFCS_TRANSPARENT        0x0800
#define DFCS_HOT                0x1000
#endif // DFCS_TRANSPARENT

void CCtrlMessageBar::DoNcPaint()
{
  if (m_pCtrlMessageBarThis == NULL)
    return;
  if (! m_bShowMessageBar && m_nMessageBarHeight > 0)
    m_pCtrlMessageBarThis->SetWindowPos(NULL, 0, 0, 0, 0, s_dwSWPFlags);
  if (! m_bShowMessageBar || m_sMessageBarText.IsEmpty() || m_nMessageBarHeight == 0)
    return;

  CDC* pDC = m_pCtrlMessageBarThis->GetWindowDC();

  COLORREF crBackground = m_bMouseHover ? (m_crBackgroundHilite == CLR_DEFAULT ? 
				GetSysColor(COLOR_HIGHLIGHT) : m_crBackgroundHilite) : 
			    (m_crBackgroundNormal == CLR_DEFAULT ? 
				GetSysColor(COLOR_INFOBK) : m_crBackgroundNormal);
  COLORREF crText = m_bMouseHover ? (m_crTextHilite == CLR_DEFAULT ? 
				GetSysColor(COLOR_HIGHLIGHTTEXT) : m_crTextHilite) : 
			    (m_crTextNormal == CLR_DEFAULT ? 
				GetSysColor(COLOR_INFOTEXT) : m_crTextNormal);

  if (pDC != NULL)
  {
    CRect rc = GetRect(TRUE); // get rect in window co-ords
    pDC->FillSolidRect(rc, crBackground);
    COLORREF crBkOld = pDC->SetBkColor(crBackground);
    COLORREF crTextOld = pDC->SetTextColor(crText);
	

    CFont* pFontOld = pDC->SelectObject(&font);
    rc.left += 2; // offset the text a little
    rc.right -= 1;

    if (m_nMessageBarImage != -1 && m_imlMessageBar.m_hImageList != NULL)
    {
      rc.top += 2;
      CPoint pt(rc.TopLeft());
      m_imlMessageBar.Draw(pDC, m_nMessageBarImage, pt, ILD_NORMAL);
      IMAGEINFO ii;
      m_imlMessageBar.GetImageInfo(m_nMessageBarImage, &ii);
      rc.left += abs(ii.rcImage.right - ii.rcImage.left) + 2;
    }

    if (m_bMessageBarCloseButton)
    {
      CRect rcClose = GetCloseButtonRect(rc);
      CFont font;
      font.CreatePointFont(CLOSE_BUTTON_FONT_SIZE, CLOSE_BUTTON_FONT, pDC);
      CFont* pFontOld2 = pDC->SelectObject(&font);
      pDC->DrawText(CLOSE_BUTTON_CHAR, rcClose, CLOSE_BUTTON_DRAW_FLAGS);
      pDC->SelectObject(pFontOld2);
    }

    CString sText(m_sMessageBarText);
    pDC->DrawText(sText.GetBuffer(sText.GetLength() + 4), -1, &rc, m_bWrapMessageBarText ? s_dwDrawTextFlagsWrap : s_dwDrawTextFlags | DT_MODIFYSTRING);
    sText.ReleaseBuffer();
    m_bTextTruncated = (sText != m_sMessageBarText);
    pDC->SetBkColor(crBkOld);
    pDC->SetTextColor(crTextOld);
    pDC->SelectObject(pFontOld);
    if (m_bTextTruncated)
    {
      // add tool-tip tool
      if (m_tipMessageBar.m_hWnd == NULL || ! ::IsWindow(m_tipMessageBar.m_hWnd))
	m_tipMessageBar.Create(m_pCtrlMessageBarThis);
      m_tipMessageBar.DelTool(m_pCtrlMessageBarThis, ID_TOOLTIP);
      m_tipMessageBar.AddTool(m_pCtrlMessageBarThis, m_sMessageBarText, GetRect(), ID_TOOLTIP);
    }
  }
}

void CCtrlMessageBar::DoNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
  m_nMessageBarHeight = 0;
  if (m_bShowMessageBar && bCalcValidRects && ! m_sMessageBarText.IsEmpty())
  {
    int nHeight = 15;
    int nHeightMin = 0;
    int nLeftOffset = 2;
    CFont* pFont = m_pCtrlMessageBarThis->GetFont();
    CDC* pDC = m_pCtrlMessageBarThis->GetDC();
    if (m_nMessageBarImage != -1 && m_imlMessageBar.m_hImageList != NULL)
    {
      IMAGEINFO ii;
      m_imlMessageBar.GetImageInfo(m_nMessageBarImage, &ii);
      nHeightMin = abs(ii.rcImage.bottom - ii.rcImage.top);
      nLeftOffset += abs(ii.rcImage.right - ii.rcImage.left) + 2 + 2;
      nHeightMin += 2;
    }
    if (m_bMessageBarCloseButton)
    {
      CFont font;
      font.CreatePointFont(CLOSE_BUTTON_FONT_SIZE, CLOSE_BUTTON_FONT, pDC);
      CFont* pFontOld2 = pDC->SelectObject(&font);
      CRect rcClose(0, 0, 1, 1);
      pDC->DrawText(CLOSE_BUTTON_CHAR, rcClose, DT_CALCRECT | CLOSE_BUTTON_DRAW_FLAGS);
      nHeightMin = max(nHeightMin, rcClose.Height() + 1 + 1);
      nLeftOffset += rcClose.Width() + 2 + 2;
      pDC->SelectObject(pFontOld2);
    }
    if (pDC != NULL)
    {
      CFont* pFontOld = pDC->SelectObject(pFont);
      CRect rc;
      m_pCtrlMessageBarThis->GetClientRect(&rc);
      rc.left += nLeftOffset;
      nHeight = pDC->DrawText(m_sMessageBarText, rc, DT_CALCRECT | (m_bWrapMessageBarText ? s_dwDrawTextFlagsWrap : s_dwDrawTextFlags));
      pDC->SelectObject(pFontOld);
    }
    nHeight += 2;
    nHeight = max(nHeight, nHeightMin);
    nHeight += 2;
    lpncsp->rgrc[0].top += nHeight;
    m_nMessageBarHeight = nHeight;
  }
}

UINT CCtrlMessageBar::DoNcHitTest(CPoint point, UINT nCode)
{
  if (nCode == HTNOWHERE || nCode == HTCAPTION)
  {
    // lets see if we really want to consider it as being in the message-bar
    CRect rc = GetRect(); // get rect in client co-ords
    CPoint pt(point);
    m_pCtrlMessageBarThis->ClientToScreen(&rc);
    CRect rcClose = GetCloseButtonRect(rc);
    if (rcClose.PtInRect(pt) && m_bMessageBarCloseButton)
      nCode = HTMESSAGEBARCLOSE;
    else if (rc.PtInRect(pt))
      nCode = HTMESSAGEBAR;
  }
  return nCode;
}

void CCtrlMessageBar::DoNcMouseButtonMsg(UINT nMsg, UINT nHitTest, CPoint point)
{
#if _DEBUG
  if (nHitTest == HTMESSAGEBAR)
    TRACE("Mouse down on HTMESSAGEBAR\n");
#endif // _DEBUG
  if (nHitTest == HTMESSAGEBARCLOSE && nMsg == WM_NCLBUTTONDOWN)
    Show(FALSE);
  else if ((nHitTest == HTMESSAGEBAR || nHitTest == HTMESSAGEBARCLOSE) && (nMsg == WM_NCRBUTTONDOWN || nMsg == WM_NCLBUTTONDOWN))
  {
    if (m_pfnShowMenuCallback == NULL || ! m_pfnShowMenuCallback(this, m_pCtrlMessageBarThis, nHitTest, point))
    {
      // show a menu to allow the bar to be hidden
      CMenu menu;
      menu.CreatePopupMenu();
      menu.AppendMenu(MF_STRING, 1, _T("Hide"));
      int nCmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | 
						  TPM_NONOTIFY | TPM_RETURNCMD, 
						  point.x, point.y, m_pCtrlMessageBarThis, NULL);
      if (nCmd == 1)
	Show(FALSE);
    }
  }
}

#ifndef WM_NCMOUSELEAVE
#define WM_NCMOUSELEAVE	0x02A2
#endif // WM_NCMOUSELEAVE
#ifndef TME_NONCLIENT
#define TME_NONCLIENT 0x10
#endif // TME_NONCLIENT
#ifndef TrackMouseEvent
#define TrackMouseEvent _TrackMouseEvent
#endif // TrackMouseEvent

LRESULT CCtrlMessageBar::DoWindowProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT lResult) 
{
  // respond to our special message to get the message bar height
  if (message == s_nMsgGetMessageBarHeight)
    return m_nMessageBarHeight;
  if (m_tipMessageBar.m_hWnd != NULL && ::IsWindow(m_tipMessageBar.m_hWnd))
  {
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    msg.hwnd = m_pCtrlMessageBarThis->m_hWnd;
    msg.lParam = lParam;
    msg.message = message;
    msg.wParam = wParam;
    m_tipMessageBar.RelayEvent(&msg);
  }
  switch (message)
  {
  case WM_CREATE:
    if (::IsWindow(m_pCtrlMessageBarThis->m_hWnd))
      m_tipMessageBar.Create(m_pCtrlMessageBarThis);
    break;

  case WM_NCLBUTTONDOWN:
  case WM_NCMBUTTONDOWN:
  case WM_NCRBUTTONDOWN:
  case WM_NCLBUTTONUP:
  case WM_NCMBUTTONUP:
  case WM_NCRBUTTONUP:
  case WM_NCLBUTTONDBLCLK:
  case WM_NCMBUTTONDBLCLK:
  case WM_NCRBUTTONDBLCLK:
    DoNcMouseButtonMsg(message, (UINT)wParam, CPoint(lParam));
    break;

  case WM_NCCALCSIZE:
    {
      // If the window is a CListCtrl, then we need to intercept the 
      // HDM_LAYOUT message that it sends to its header so that we can 
      // adjust the layout, so subclass its header.
      // The header may already be subclassed, so we need to use a double-
      // subclassing technique to make sure we don't break its existing subclassing
      CListCtrl* pListCtrl = DYNAMIC_DOWNCAST(CListCtrl, m_pCtrlMessageBarThis);
      if (pListCtrl != NULL && pListCtrl->m_hWnd != NULL && ! CCtrlMessageBarDblSubclassWnd::IsSubclassed(pListCtrl->m_hWnd))
	CCtrlMessageBarDblSubclassWnd::SubclassWindow(::GetDlgItem(pListCtrl->m_hWnd, 0));
    }
    DoNcCalcSize((BOOL)wParam, (NCCALCSIZE_PARAMS*)lParam);
    break;

  case WM_NCHITTEST:
    return (LRESULT)DoNcHitTest(CPoint(lParam), (UINT)lResult);
    break;

  case WM_NCPAINT:
    DoNcPaint();
    break;

  case WM_SIZE:
    m_pCtrlMessageBarThis->SetWindowPos(NULL, 0, 0, 0, 0, s_dwSWPFlags);
    break;

  case WM_VSCROLL:
    if (m_bShowMessageBar && m_pCtrlMessageBarThis->IsKindOf(RUNTIME_CLASS(CListCtrl)))
    {
      // ListView controls base their client areas on the window size, and 
      // so using WM_NCCALCSIZE to resize the client causes the scrolling 
      // to go wrong. There doesn't seem to be any way of influencing this, 
      // so the best we can do is to fiddle the scrolling to behave the 
      // way it should be. This, however, still isn't perfect and the 
      // window scrolls back up a little after scrolling to the very bottom.
      CListCtrl* pList = (CListCtrl*)m_pCtrlMessageBarThis;
      UINT nStyle = pList->GetStyle() & LVS_TYPEMASK;
      if (nStyle == LVS_ICON || nStyle == LVS_SMALLICON)
      {
	SCROLLINFO si = { sizeof(si), SIF_RANGE };
	pList->GetScrollInfo(SB_VERT, &si);
	CRect rcView;
	pList->GetViewRect(&rcView);
	si.nMax = rcView.Height() + m_nMessageBarHeight;
	pList->SetScrollInfo(SB_VERT, &si);
      }
    }
    break;

  case WM_NCMOUSEMOVE:
  case WM_NCMOUSELEAVE:
  case WM_MOUSELEAVE:
    // if we're over the bar, then set the bar to draw highlighted
    if (m_bHighlightOnMouseOver)
    {
      if (message == WM_NCMOUSEMOVE)
      {
	// make sure we get leave
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE | TME_NONCLIENT;
	tme.dwHoverTime = HOVER_DEFAULT;
	tme.hwndTrack = m_pCtrlMessageBarThis->m_hWnd;
	TrackMouseEvent(&tme);
      }
      BOOL bOldMouseOver = m_bMouseHover;
      if (message == WM_NCMOUSEMOVE && DoNcHitTest(CPoint(lParam), (UINT)lResult) == HTMESSAGEBAR)
	m_bMouseHover = TRUE;
      else
	m_bMouseHover = FALSE;
      if (m_bMouseHover != bOldMouseOver)
	m_pCtrlMessageBarThis->SetWindowPos(NULL, 0, 0, 0, 0, s_dwSWPFlags);
    }
    break;
  }
  return lResult;
}
