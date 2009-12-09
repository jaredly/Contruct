// HTMLStatic.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////
// (c) Jerome Sopocko 2003
// this code worked last time I saw it
//////////////////////////////////////////////////////////////////////
// v1.0 : 25/3/2003 
//        First version
// v1.1 : 11/4/2003
//        Added support for <P> and <A HREF>- thanks to Hockey who pointed me to 
//        Hans Dietrich's article: http://www.codeproject.com/staticctrl/XHTMLStatic.asp
//        Corrected alignment of fonts of different sizes (thanks to Chopper's idea)
// v1.2 : 4/8/2003
//        Corrected bug on resize + Flickering of mouse cursor (thanks to Pranavamhari's remark)
//////////////////////////////////////////////////////////////////////
//  Parts of the code are "borrowed" from Hans Dietrich, Chris Maunder and
//  Paul DiLascia
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HTMLStatic.h"

#include "HTMLDrawer.h"
#include "HTMLAtom.h"

#include "atlconv.h"    // for Unicode conversion - requires #include <afxdisp.h> // MFC OLE automation classes

#define WMT_TIMER 101

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHTMLStatic

CHTMLStatic::CHTMLStatic()
{
	m_isTextPrepared = false;

	m_rgbBackground	= ::g_PaintManager->GetColor(COLOR_BTNFACE);

	m_rgbHighlight	= RGB(255,0,0);
	m_isUnderlineWhenHighlight	= true;
	
	m_hLinkCursor = NULL;
	m_hPrevCursor = NULL;
	m_isAtomHighlighted = false;

	SetDefaultCursor();
}

CHTMLStatic::~CHTMLStatic()
{
}


BEGIN_MESSAGE_MAP(CHTMLStatic, CStatic)
	//{{AFX_MSG_MAP(CHTMLStatic)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHTMLStatic message handlers

void CHTMLStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void CHTMLStatic::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == WMT_TIMER )
	{
		// Get the mouse position
		CPoint pntMouse;
		GetCursorPos(&pntMouse);

		// Transform into client coordinates
		ScreenToClient(&pntMouse);

		// If we are on an anchor
		CHTMLAtom * pAnchor;
		if ( m_htmlDrawer.IsPointInAnchor(pntMouse,pAnchor) )
		{
			CRect rctToInvalidate;
			if ( m_htmlDrawer.HighlightAnchor(pAnchor,rctToInvalidate) )
				InvalidateRect(rctToInvalidate);

			
			// Change the cursor to the hand
			if ( !m_isAtomHighlighted )
			{
				if (m_hLinkCursor)
				{
					m_hPrevCursor = ::SetCursor(m_hLinkCursor);
				}
			}
			m_isAtomHighlighted = true;
		}
		else
		{
			CRect rctToInvalidate;
			if ( m_htmlDrawer.HighlightAnchor(NULL,rctToInvalidate) )
				InvalidateRect(rctToInvalidate);

			if ( m_isAtomHighlighted )
				::SetCursor(m_hPrevCursor);

			m_isAtomHighlighted = false;
		}
	}
	
	CStatic::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// The following is "borrowed" from
// Hans Dietrich's article: http://www.codeproject.com/staticctrl/XHTMLStatic.asp
// who borrowed it from Chris Maunder's http://www.codeproject.com/miscctrl/hyperlink.asp
// himself inspired by Paul DiLascia's Jan 1998 MSJ articles.
/////////////////////////////////////////////////////////////////////////////
// It loads a "hand" cursor from the winhlp32.exe module
void CHTMLStatic::SetDefaultCursor()
{
    if (m_hLinkCursor == NULL)                // No cursor handle - load our own
    {
        // Get the windows directory
        CString strWndDir;
        GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
        strWndDir.ReleaseBuffer();

        strWndDir += _T("\\winhlp32.exe");
        // This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
        HMODULE hModule = LoadLibrary(strWndDir);
        if (hModule) {
            HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
            if (hHandCursor)
                m_hLinkCursor = CopyCursor(hHandCursor);
        }
        FreeLibrary(hModule);
    }
}

BOOL CHTMLStatic::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{
	// To avoid flickering of cursor
	if ( m_isAtomHighlighted )
	{
		// Change the cursor to the hand
		if (m_hLinkCursor)
		{
			::SetCursor(m_hLinkCursor);
			return TRUE;
		}
	}

    return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// The following is "borrowed" from
// Hans Dietrich's article: http://www.codeproject.com/staticctrl/XHTMLStatic.asp
// who borrowed it from Chris Maunder's http://www.codeproject.com/miscctrl/hyperlink.asp
// himself inspired "GotoURL" function by Stuart Patterson
// As seen in the August, 1997 Windows Developer's Journal.
// Copyright 1997 by Miller Freeman, Inc. All rights reserved.
// Modified by Chris Maunder to use TCHARs instead of chars.
/////////////////////////////////////////////////////////////////////////////
HINSTANCE CHTMLStatic::GotoURL(LPCTSTR url, int showcmd)
{
    TCHAR key[MAX_PATH + MAX_PATH];

    // First try ShellExecute()
    HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR) {

        if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) {
            lstrcat(key, _T("\\shell\\open\\command"));

            if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) {
                TCHAR *pos;
                pos = _tcsstr(key, _T("\"%1\""));
                if (pos == NULL) {                     // No quotes found
                    pos = _tcsstr(key, _T("%1"));      // Check for %1, without quotes 
                    if (pos == NULL)                   // No parameter at all...
                        pos = key+lstrlen(key)-1;
                    else
                        *pos = '\0';                   // Remove the parameter
                }
                else
                    *pos = '\0';                       // Remove the parameter

                lstrcat(pos, _T(" "));
                lstrcat(pos, url);

                USES_CONVERSION;
                result = (HINSTANCE) WinExec(T2A(key),showcmd);
            }
        }
    }

    return result;
}


LONG CHTMLStatic::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) {
        long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, NULL, data, &datasize);
        lstrcpy(retdata,data);
        RegCloseKey(hkey);
    }

    return retval;
}

void CHTMLStatic::ReportError(int nError)
{
    CString str;
    switch (nError) {
        case 0:                       str = _T("The operating system is out\nof memory or resources."); break;
        case SE_ERR_PNF:              str = _T("The specified path was not found."); break;
        case SE_ERR_FNF:              str = _T("The specified file was not found."); break;
        case ERROR_BAD_FORMAT:        str = _T("The .EXE file is invalid\n(non-Win32 .EXE or error in .EXE image)."); break;
        case SE_ERR_ACCESSDENIED:     str = _T("The operating system denied\naccess to the specified file."); break;
        case SE_ERR_ASSOCINCOMPLETE:  str = _T("The filename association is\nincomplete or invalid."); break;
        case SE_ERR_DDEBUSY:          str = _T("The DDE transaction could not\nbe completed because other DDE transactions\nwere being processed."); break;
        case SE_ERR_DDEFAIL:          str = _T("The DDE transaction failed."); break;
        case SE_ERR_DDETIMEOUT:       str = _T("The DDE transaction could not\nbe completed because the request timed out."); break;
        case SE_ERR_DLLNOTFOUND:      str = _T("The specified dynamic-link library was not found."); break;
        case SE_ERR_NOASSOC:          str = _T("There is no application associated\nwith the given filename extension."); break;
        case SE_ERR_OOM:              str = _T("There was not enough memory to complete the operation."); break;
        case SE_ERR_SHARE:            str = _T("A sharing violation occurred. ");
        default:                      str.Format(_T("Unknown Error (%d) occurred."), nError); break;
    }
	CString strFinal;
	strFinal.Format(_T("Unable to open hyperlink:\r\n%s"),str);
    AfxMessageBox(strFinal);
}


void CHTMLStatic::PreSubclassWindow() 
{
    // We want to get mouse clicks via STN_CLICKED
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
	
	CStatic::PreSubclassWindow();
}

void CHTMLStatic::OnClicked() 
{
	// Get the mouse position
	CPoint pntMouse;
	GetCursorPos(&pntMouse);

	// Transform into client coordinates
	ScreenToClient(&pntMouse);

	CHTMLAtom * pAnchor;
	if ( m_htmlDrawer.IsPointInAnchor(pntMouse,pAnchor) )
	{
		HINSTANCE result = GotoURL(pAnchor->GetURL(), SW_SHOW);
		if ((UINT)result <= HINSTANCE_ERROR)
			ReportError((int)result);
	}
	
}

void CHTMLStatic::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	// Recalculate the positions of controls
	m_isTextPrepared = false;
	Invalidate(true);
}
