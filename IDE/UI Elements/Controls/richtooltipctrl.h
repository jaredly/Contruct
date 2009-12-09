////////////////////////////////////////////////////////////////////////////
// File:	RichToolTipCtrl.h
// Version:	1.2
// Created:	06-Oct-2008
//
// Author:	Paul S. Vickery
// E-mail:	developer@vickeryhome.freeserve.co.uk
//
// Class to provide a means of using rich text in a tool-tip, without using 
// the newer styles only available in later systems. Based on CToolTipCtrl.
//
// You are free to use or modify this code, with no restrictions, other than
// you continue to acknowledge me as the original author in this source code,
// or any code derived from it.
//
// If you use this code, or use it as a base for your own code, it would be 
// nice to hear from you simply so I know it's not been a waste of time!
//
// Copyright (c) 2005--2008 Paul S. Vickery
//
////////////////////////////////////////////////////////////////////////////
// Version History:
//
// Version 1.2 - 06-Oct-2008
// =========================
// - Changed to draw correctly on XP/Vista
//
// Version 1.1 - 22-Mar-2005
// =========================
// - Added static method for escaping plain text to make it RTF safe
// - Modified the positioning to work on multi-monitor systems
// - Removed dependency on RichToolTipCtrlDemo.h
// 
// Version 1.0 - 14-Mar-2005
// =========================
// Initial version
// 
////////////////////////////////////////////////////////////////////////////
// PLEASE LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHTOOLTIPCTRL_H__EFAAE1BA_CDFD_4C7B_B778_146FAC4E3309__INCLUDED_)
#define AFX_RICHTOOLTIPCTRL_H__EFAAE1BA_CDFD_4C7B_B778_146FAC4E3309__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RichToolTipCtrl.h : header file
//

#include <richole.h>  // for IRichEditOleCallback

#ifndef _UXTHEME_H_
// to avoid reliance on uxtheme.h, and uxtheme.lib, we'll load 
// the theme functions, but only once
typedef HANDLE HTHEME;
#endif // _UXTHEME_H_

typedef HTHEME(_stdcall *PFNOPENTHEMEDATA)(HWND, LPWSTR);
typedef HRESULT(_stdcall *PFNCLOSETHEMEDATA)(HTHEME);
typedef HRESULT(__stdcall *PFNDRAWTHEMEBACKGROUND)(HTHEME hTheme, HDC hdc, 
	int iPartId, int iStateId, const RECT *pRect,  const RECT *pClipRect);
typedef BOOL (__stdcall *PFNISTHEMEBACKGROUNDPARTIALLYTRANSPARENT)(HTHEME hTheme, int iPartId, int iStateId);
typedef HRESULT(__stdcall *PFNDRAWTHEMEPARENTBACKGROUND)(HWND, HDC, RECT*);
typedef BOOL(__stdcall *PFNISAPPTHEMED)(void);

#include <shlwapi.h>  // for DllGetVersion definitions

/////////////////////////////////////////////////////////////////////////////
// CRichToolTipCtrl window

class CRichToolTipCtrl : public CToolTipCtrl
{
	DECLARE_DYNAMIC(CRichToolTipCtrl);

// Construction
public:
	CRichToolTipCtrl();

// Attributes
public:

// Operations
public:
	static CString MakeTextRTFSafe(LPCTSTR lpszText);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichToolTipCtrl)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRichToolTipCtrl();

protected:
  /////////////////////////////////////////////////////////////////////////////
  // CRichToolTipRichEditCtrl window

  class CRichToolTipRichEditCtrl : public CRichEditCtrl
  {
  // Construction
  protected:
	  CRichToolTipRichEditCtrl();

  // Attributes
  protected:

  // Operations
  protected:
	  int StreamTextIn(LPCTSTR lpszText);

  // Overrides
	  // ClassWizard generated virtual function overrides
	  //{{AFX_VIRTUAL(CRichToolTipRichEditCtrl)
	  //}}AFX_VIRTUAL

  // Implementation
  protected:
	  virtual ~CRichToolTipRichEditCtrl();

	  // Generated message map functions
  protected:
	  //{{AFX_MSG(CRichToolTipRichEditCtrl)
	  //}}AFX_MSG

	  DECLARE_MESSAGE_MAP()

	  friend class CRichToolTipCtrl;
  };

public:
  class CRichToolTipXPStyle
  {
    // comctl32.dll, and some other DLLs, export a DllGetVersion method 
    // so we use that to see which version of comctl32.dll is currently loaded
    static DWORD GetComCtl32Version()
    {
      HINSTANCE hInstDll = LoadLibrary(_T("COMCTL32.DLL"));
      if (hInstDll == NULL)
	return 0;
      DLLGETVERSIONPROC pfnDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hInstDll, "DllGetVersion");
      if (pfnDllGetVersion == NULL)
	return 0;
      DLLVERSIONINFO dvi = { sizeof(dvi) };
      pfnDllGetVersion(&dvi);
      FreeLibrary(hInstDll);
      return dvi.dwMajorVersion;
    }
  public:
    CRichToolTipXPStyle()
    {
      m_nIsAppXPStyled = -1;
      m_hThemeDLL = LoadLibrary(_T("uxtheme.dll"));
      if (m_hThemeDLL != NULL)
      {
	m_pfnOpenThemeData = (PFNOPENTHEMEDATA)GetProcAddress(m_hThemeDLL, "OpenThemeData");
	m_pfnCloseThemeData = (PFNCLOSETHEMEDATA)GetProcAddress(m_hThemeDLL, "CloseThemeData");
	m_pfnDrawThemeBackground = (PFNDRAWTHEMEBACKGROUND)GetProcAddress(m_hThemeDLL, "DrawThemeBackground");
	m_pfnIsAppThemed = (PFNISAPPTHEMED)GetProcAddress(m_hThemeDLL, "IsAppThemed");
	m_pfnDrawThemeParentBackground = (PFNDRAWTHEMEPARENTBACKGROUND)GetProcAddress(m_hThemeDLL, "DrawThemeParentBackground");
	m_pfnIsThemeBackgroundPartiallyTransparent = (PFNISTHEMEBACKGROUNDPARTIALLYTRANSPARENT)GetProcAddress(m_hThemeDLL, "IsThemeBackgroundPartiallyTransparent");
      }
      else
      {
	m_pfnOpenThemeData = NULL;
	m_pfnCloseThemeData = NULL;
	m_pfnDrawThemeBackground = NULL;
	m_pfnIsAppThemed = NULL;
	m_pfnDrawThemeParentBackground = NULL;
	m_pfnIsThemeBackgroundPartiallyTransparent = NULL;
      }
    }
    ~CRichToolTipXPStyle()
      { if (m_hThemeDLL != NULL) FreeLibrary(m_hThemeDLL); }
    HTHEME OpenThemeData(HWND hwnd, LPWSTR pszClassList)
      { return (m_pfnOpenThemeData != NULL) ? m_pfnOpenThemeData(hwnd, pszClassList) : NULL; }
    HRESULT CloseThemeData(HTHEME hTheme)
      { return (m_pfnCloseThemeData != NULL) ? m_pfnCloseThemeData(hTheme) : E_FAIL; }
    HRESULT DrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect = NULL)
      { return (m_pfnDrawThemeBackground != NULL) ? m_pfnDrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect) : E_FAIL; }
    HRESULT DrawThemeParentBackground(HWND hwnd, HDC hdc, RECT *prc)
      { return (m_pfnDrawThemeParentBackground != NULL) ? m_pfnDrawThemeParentBackground(hwnd, hdc, prc) : S_OK; }
    BOOL IsThemeBackgroundPartiallyTransparent(HTHEME hTheme, int iPartId, int iStateId)
      { return (m_pfnIsThemeBackgroundPartiallyTransparent != NULL) ? m_pfnIsThemeBackgroundPartiallyTransparent(hTheme, iPartId, iStateId) : FALSE; }
    BOOL IsAppThemed(void)
      { return (m_pfnIsAppThemed != NULL) ? m_pfnIsAppThemed() : FALSE; }
    BOOL IsAppXPStyled()
    {
      if (m_nIsAppXPStyled == -1)
	// IsAppThemed returns TRUE even if the app has no manifest file 
	// The only way to really check is to test the major version of comctl32.dll
	m_nIsAppXPStyled = (IsAppThemed() && GetComCtl32Version() >= 6) ? 1 : 0;
      return (m_nIsAppXPStyled > 0);
    }
    void Reset() { m_nIsAppXPStyled = -1; }
  protected:
    HINSTANCE m_hThemeDLL;  // DLL handle for XP styling library
    PFNOPENTHEMEDATA m_pfnOpenThemeData;
    PFNCLOSETHEMEDATA m_pfnCloseThemeData;
    PFNDRAWTHEMEBACKGROUND m_pfnDrawThemeBackground;
    PFNISAPPTHEMED m_pfnIsAppThemed;
    PFNDRAWTHEMEPARENTBACKGROUND m_pfnDrawThemeParentBackground;
    PFNISTHEMEBACKGROUNDPARTIALLYTRANSPARENT m_pfnIsThemeBackgroundPartiallyTransparent;
    int m_nIsAppXPStyled;
  };

	// Generated message map functions
protected:
	CRichToolTipRichEditCtrl m_edit;
	CSize m_sizeEditMin;

	CSize CalculateMinimiumRichEditSize();

	//{{AFX_MSG(CRichToolTipCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShow(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

// Interface Map
public:
	BEGIN_INTERFACE_PART(RichEditOleCallback, IRichEditOleCallback)
		INIT_INTERFACE_PART(CRichToolTipCtrl, RichEditOleCallback)
		STDMETHOD(GetNewStorage) (LPSTORAGE*);
		STDMETHOD(GetInPlaceContext) (LPOLEINPLACEFRAME*, LPOLEINPLACEUIWINDOW*, LPOLEINPLACEFRAMEINFO);
		STDMETHOD(ShowContainerUI) (BOOL);
		STDMETHOD(QueryInsertObject) (LPCLSID, LPSTORAGE, LONG);
		STDMETHOD(DeleteObject) (LPOLEOBJECT);
		STDMETHOD(QueryAcceptData) (LPDATAOBJECT, CLIPFORMAT*, DWORD,BOOL, HGLOBAL);
		STDMETHOD(ContextSensitiveHelp) (BOOL);
		STDMETHOD(GetClipboardData) (CHARRANGE*, DWORD, LPDATAOBJECT*);
		STDMETHOD(GetDragDropEffect) (BOOL, DWORD, LPDWORD);
		STDMETHOD(GetContextMenu) (WORD, LPOLEOBJECT, CHARRANGE*, HMENU*);
	END_INTERFACE_PART(RichEditOleCallback)

	DECLARE_INTERFACE_MAP()
};

inline STDMETHODIMP_(ULONG) CRichToolTipCtrl::XRichEditOleCallback::AddRef()
  { METHOD_PROLOGUE_EX_(CRichToolTipCtrl, RichEditOleCallback)
    return (ULONG)pThis->InternalAddRef(); }
inline STDMETHODIMP_(ULONG) CRichToolTipCtrl::XRichEditOleCallback::Release()
  { METHOD_PROLOGUE_EX_(CRichToolTipCtrl, RichEditOleCallback)
    return (ULONG)pThis->InternalRelease(); }
inline STDMETHODIMP CRichToolTipCtrl::XRichEditOleCallback::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
  { METHOD_PROLOGUE_EX_(CRichToolTipCtrl, RichEditOleCallback)
    return (HRESULT)pThis->InternalQueryInterface(&iid, ppvObj); }
inline STDMETHODIMP CRichToolTipCtrl::XRichEditOleCallback::GetInPlaceContext(
	LPOLEINPLACEFRAME* /*lplpFrame*/, LPOLEINPLACEUIWINDOW* /*lplpDoc*/,
	LPOLEINPLACEFRAMEINFO /*lpFrameInfo*/)
  { return S_OK; }
inline STDMETHODIMP CRichToolTipCtrl::XRichEditOleCallback::ShowContainerUI(BOOL /*fShow*/)
  { return S_OK; }
inline STDMETHODIMP CRichToolTipCtrl::XRichEditOleCallback::QueryInsertObject(
	LPCLSID /*lpclsid*/, LPSTORAGE /*pstg*/, LONG /*cp*/)
  { return S_OK; }
inline STDMETHODIMP CRichToolTipCtrl::XRichEditOleCallback::DeleteObject(LPOLEOBJECT /*lpoleobj*/)
  { return S_OK; }
inline STDMETHODIMP CRichToolTipCtrl::XRichEditOleCallback::QueryAcceptData(
	LPDATAOBJECT /*lpdataobj*/, CLIPFORMAT* /*lpcfFormat*/, DWORD /*reco*/,
	BOOL /*fReally*/, HGLOBAL /*hMetaPict*/)
  { return S_OK; }
inline STDMETHODIMP CRichToolTipCtrl::XRichEditOleCallback::ContextSensitiveHelp(BOOL /*fEnterMode*/)
  { return E_NOTIMPL; }
inline STDMETHODIMP CRichToolTipCtrl::XRichEditOleCallback::GetClipboardData(
	CHARRANGE* /*lpchrg*/, DWORD /*reco*/, LPDATAOBJECT* /*lplpdataobj*/)
  { return S_OK; }
inline STDMETHODIMP CRichToolTipCtrl::XRichEditOleCallback::GetDragDropEffect(
	BOOL /*fDrag*/, DWORD /*grfKeyState*/, LPDWORD /*pdwEffect*/)
  { return S_OK; }
inline STDMETHODIMP CRichToolTipCtrl::XRichEditOleCallback::GetContextMenu(
	WORD /*seltype*/, LPOLEOBJECT /*lpoleobj*/, CHARRANGE* /*lpchrg*/, HMENU* /*lphmenu*/)
  { return S_OK; }

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHTOOLTIPCTRL_H__EFAAE1BA_CDFD_4C7B_B778_146FAC4E3309__INCLUDED_)
