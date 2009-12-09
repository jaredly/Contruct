// mfxWhlPan.cpp : implementation file
//

#include "stdafx.h"
#include "../resource.h"
#include "mfxWhlPan.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_DELAY_MS 10

void MfxTrackAutoPan(CWnd* pParentWnd, WORD wStyle, CAutoPanParameters* pAutoPanParameters)
{
	CWheelWnd* pWP=new CWheelWnd;
	pWP->Create(pParentWnd, wStyle, pAutoPanParameters);
}

const UINT CAutoPanParametersRegMessage::uOriginWindowUpdateMessage = ::RegisterWindowMessage(OWND_WINDOW_UPDATE);


/////////////////////////////////////////////////////////////////////////////
// CAutoPanParameters

CAutoPanParameters::CAutoPanParameters(LPCTSTR szClassName, UINT nWindowResID)
{
	if (szClassName) m_szClassName = _tcsdup(szClassName);
	else m_szClassName = NULL;

	if (nWindowResID) m_nWindowResID = nWindowResID;
	else m_nWindowResID = IDB_AUTOPAN;
}

CAutoPanParameters::~CAutoPanParameters()
{
	if (m_szClassName) free(m_szClassName);
}

CWnd* CAutoPanParameters::PanThisWindow(CWnd* pParentWnd) const
{
	if (m_szClassName == NULL) return pParentWnd;

	TCHAR szClassName[MAX_PATH];
	::GetClassName(pParentWnd->m_hWnd, szClassName, MAX_PATH);

	if (lstrcmp(m_szClassName, szClassName) == 0)
	{
		return pParentWnd;
	}

	return NULL;
}

void CAutoPanParameters::CreateWindowRegion(CRgn& rgn, CWnd* /*pParentWnd*/) const
{
	rgn.CreateEllipticRgn(2, 2, 31, 31);
}

void CAutoPanParameters::GetBitmapDimensions(CSize& size, CWnd* pParentWnd) const
{
	CBitmap bmp; BITMAP bmpdata;
	bmp.LoadBitmap(GetWindowResID(pParentWnd));
	bmp.GetBitmap(&bmpdata);

	size.cx = bmpdata.bmWidth / 3;
	size.cy = bmpdata.bmHeight;
}
 
bool CAutoPanParameters::NoVertScroll(CWnd* pParentWnd) const
{
	DWORD dwStyle = pParentWnd->GetStyle();

	CScrollBar* pVBar = pParentWnd->GetScrollBarCtrl(SB_VERT);

	INT MinPos, MaxPos;
	pParentWnd->GetScrollRange(SB_VERT, &MinPos, &MaxPos);

	if (pParentWnd->GetScrollLimit(SB_VERT) == 0) return true;
	if (MinPos == MaxPos) return true;
	if (pVBar != NULL && !pVBar->IsWindowEnabled()) return true;

	return false;
}

bool CAutoPanParameters::NoHorzScroll(CWnd* pParentWnd) const
{
	DWORD dwStyle = pParentWnd->GetStyle();

	CScrollBar* pSBar = pParentWnd->GetScrollBarCtrl(SB_HORZ);

	INT MinPos, MaxPos;
	pParentWnd->GetScrollRange(SB_HORZ, &MinPos, &MaxPos);

	if (pParentWnd->GetScrollLimit(SB_HORZ) == 0) return true;
	if (MinPos == MaxPos) return true;
	if (pSBar != NULL && !pSBar->IsWindowEnabled()) return true;

	return false;
}

int CAutoPanParameters::GetCursorResID(
	int nScrollX, int nScrollY, 
	bool bNoHorzScroll, bool bNoVertScroll) const
{
	if (nScrollY>0)	// Pan DOWN
	{
		if (nScrollX==0 && nScrollY<200)                return IDC_PAN_DOWN;
		else if (nScrollX>0 && nScrollY+nScrollX<200)   return IDC_PAN_DOWNRIGHT;
		else if (nScrollX<0 && nScrollY-nScrollX<200)   return IDC_PAN_DOWNLEFT;
		else if (nScrollX==0)                           return IDC_PAN_DOWNFAST;
		else if (nScrollX>0)                            return IDC_PAN_DOWNRIGHTFAST;
		else                                            return IDC_PAN_DOWNLEFTFAST;
	}
	else if (nScrollY<0) // Pan UP
	{
		if (nScrollX==0 && nScrollY>-200)               return IDC_PAN_UP;
		else if (nScrollX>0 && nScrollY-nScrollX>-200)  return IDC_PAN_UPRIGHT;
		else if (nScrollX<0 && nScrollY+nScrollX>-200)  return IDC_PAN_UPLEFT;
		else if (nScrollX==0)                           return IDC_PAN_UPFAST;
		else if (nScrollX>0)                            return IDC_PAN_UPRIGHTFAST;
		else                                            return IDC_PAN_UPLEFTFAST;
	}
	else if (nScrollX != 0) // Pan LEFT/RIGHT
	{
		if (nScrollX>0 && nScrollX<200)                 return IDC_PAN_RIGHT;
		else if (nScrollX<0 && nScrollX>-200)           return IDC_PAN_LEFT;
		else if (nScrollX>0)                            return IDC_PAN_RIGHTFAST;
		else                                            return IDC_PAN_LEFTFAST;
	}
	else if (bNoVertScroll)                             return IDC_PAN_LEFTRIGHT;
	else if (bNoHorzScroll)                             return IDC_PAN_UPDOWN;
	else                                                return IDC_PAN_ALL;
}

bool CAutoPanParameters::DoScroll(CWnd* pParentWnd, int nScrollStepsX, int nScrollStepsY) const
{
    int nOriginX, nScrollToX, nMaxX, nSubstepsInAStepX;
    int nOriginY, nScrollToY, nMaxY, nSubstepsInAStepY;

	nSubstepsInAStepX = GetSubstepsInAStepX(pParentWnd, nScrollStepsX);
	nSubstepsInAStepY = GetSubstepsInAStepY(pParentWnd, nScrollStepsY);

    // Adjust current positions based on scroll bar constraints.
    nOriginX = nScrollToX = pParentWnd->GetScrollPos(SB_HORZ) * nSubstepsInAStepX;
    nOriginY = nScrollToY = pParentWnd->GetScrollPos(SB_VERT) * nSubstepsInAStepY;
	
	// Calculate the new positions
	if (nScrollStepsX != 0)
	{
	    nMaxX = pParentWnd->GetScrollLimit(SB_HORZ) * nSubstepsInAStepX;
		nScrollToX += nScrollStepsX * nSubstepsInAStepX;

		//Sanity checks.
		if (nScrollToX < 0)	nScrollToX = 0;
		else if (nScrollToX > nMaxX)	nScrollToX = nMaxX;
	}
   
	if (nScrollStepsY != 0)
	{
		nMaxY = pParentWnd->GetScrollLimit(SB_VERT) * nSubstepsInAStepY;
		nScrollToY += nScrollStepsY * nSubstepsInAStepY;

		//Sanity checks.
		if (nScrollToY < 0)	nScrollToY = 0;
	    else if (nScrollToY > nMaxY) nScrollToY = nMaxY;
    }

    // If nothing changed, just return, no work to do.
    if (nScrollToX == nOriginX && nScrollToY == nOriginY) 
	{
		return false;
	}

    return DoScrollWindow(pParentWnd, nScrollToX, nOriginX, nScrollToY, nOriginY);
}

bool CAutoPanParameters::DoScrollWindow(CWnd* pParentWnd, int nScrollToX, int nOriginX, int nScrollToY, int nOriginY) const
{
	if (nScrollToX != nOriginX) pParentWnd->SetScrollPos(SB_HORZ, nScrollToX);
	if (nScrollToY != nOriginY)	pParentWnd->SetScrollPos(SB_VERT, nScrollToY);

	pParentWnd->ScrollWindow(nOriginX-nScrollToX, nOriginY-nScrollToY);
	pParentWnd->UpdateWindow();

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// type handling
//PORT: this gives "warning C4995: 'DestructElements': name was marked as #pragma deprecated" (?)
//PORT: in comments is the old version.  I don't understand the purpose of this function -
// it doesnt appear to use the template...
/*
template <> void AFXAPI DestructElements <CAutoPanParameters*> ( CAutoPanParameters** ppAutoPanParameters, int nCount )
{
    for ( int i = 0; i < nCount; i++, ppAutoPanParameters++ )    
	{
        delete *ppAutoPanParameters;    
	}
}
*/

CList<CAutoPanParameters*, CAutoPanParameters*> CWheelWnd::m_lstAutoPanParameters;

bool CWheelWnd::m_bIsInitialized = CWheelWnd::Initialize();
bool CWheelWnd::Initialize()
{
	//LEAK HERE!  NONE OF THESE ARE DELETED!
	m_lstAutoPanParameters.AddTail(new CAutoPanParametersComboBox());
	m_lstAutoPanParameters.AddTail(new CAutoPanParametersmsctls_updown32());

	m_lstAutoPanParameters.AddTail(new CAutoPanParametersSysTreeView32());
	m_lstAutoPanParameters.AddTail(new CAutoPanParametersRICHEDIT());
	m_lstAutoPanParameters.AddTail(new CAutoPanParametersEdit());
	m_lstAutoPanParameters.AddTail(new CAutoPanParametersSysListView32());
	m_lstAutoPanParameters.AddTail(new CAutoPanParametersListBox());

	// must be last:
	m_lstAutoPanParameters.AddTail(new CAutoPanParameters());

	return true;
}

#define CWheelWnd_Register(class) CWheelWnd::Register(new class())
void CWheelWnd::Register(CAutoPanParameters* pAutoPanParameters)
{
	m_lstAutoPanParameters.AddHead(pAutoPanParameters);
}


/////////////////////////////////////////////////////////////////////////////
// CWheelWnd

CAutoPanParametersMessage CWheelWnd::m_AutoPanParametersMessage;
CAutoPanParametersLineMessages CWheelWnd::m_AutoPanParametersLineMessages;
CAutoPanParametersRegMessage CWheelWnd::m_AutoPanParametersRegMessage;
CAutoPanParametersMessageThumbTrack CWheelWnd::m_AutoPanParametersMessageThumbTrack;


CWheelWnd::CWheelWnd()
{
	m_pParentWnd   = NULL;
	m_pRealisation = NULL;

	m_bNoVertScroll=m_bNoHorzScroll=TRUE;
	m_wStyle=MFX_WHEELWNDSTYLE_DEFAULT;
	m_ptScrollSum.x = 0;
	m_ptScrollSum.y = 0;
}

CWheelWnd::~CWheelWnd()
{
}

BOOL CWheelWnd::Create(CWnd* pParentWnd, WORD wStyle, CAutoPanParameters* pAutoPanParameters)
{
	ASSERT(m_bIsInitialized);
	ASSERT(::IsWindow(pParentWnd->m_hWnd));

	// Is there a window to pan?
	if (!pParentWnd || !::IsWindow(pParentWnd->m_hWnd))
	{
		delete this;
		return FALSE;
	}

	// copy style
	m_wStyle = wStyle;

#ifdef _DEBUG
	TCHAR szClassName[MAX_PATH];
	::GetClassName(pParentWnd->m_hWnd, szClassName, MAX_PATH);
	TRACE1("We are panning a \"%s\"\n", szClassName);
#endif //_DEBUG

	if (pAutoPanParameters == NULL)
	{
		if (wStyle & MFX_WHEELWNDSTYLE_SCROLLBYMESSAGE)
		{
			pAutoPanParameters = &m_AutoPanParametersMessage;
		}
		else if (wStyle & MFX_WHEELWNDSTYLE_SCROLLBYMESSAGEEX)
		{
			pAutoPanParameters = &m_AutoPanParametersLineMessages;
		}
		else if (wStyle & MFX_WHEELWNDSTYLE_SCROLLBYMESSAGEREG)
		{
			pAutoPanParameters = &m_AutoPanParametersRegMessage;
		}
		else if (wStyle & MFX_WHEELWNDSTYLE_SCROLLBYMESSAGETRK)
		{
			pAutoPanParameters = &m_AutoPanParametersMessageThumbTrack;
		}
	}

	if (pAutoPanParameters == NULL)
	{
		// Set algorithms for common controls...
		CAutoPanParameters* pRealisation;
		for (POSITION pos = m_lstAutoPanParameters.GetHeadPosition(); pos != NULL; )
		{
			pRealisation = m_lstAutoPanParameters.GetNext(pos);
			m_pParentWnd = pRealisation->PanThisWindow(pParentWnd);

			if (m_pParentWnd)
			{
				m_pRealisation = pRealisation;
				break;
			}
		}
	}
	else
	{
		m_pRealisation = pAutoPanParameters;
		m_pParentWnd   = pParentWnd;
	}

	if (m_pRealisation && m_pParentWnd)
	{
		m_bNoVertScroll  = (wStyle & MFX_WHEELWNDSTYLE_LEFTRIGHTONLY)?true:false;
		m_bNoVertScroll |= m_pRealisation->NoVertScroll(m_pParentWnd);

		m_bNoHorzScroll  = (wStyle & MFX_WHEELWNDSTYLE_UPDOWNONLY)?true:false;
		m_bNoHorzScroll |= m_pRealisation->NoHorzScroll(m_pParentWnd);
	}

	// Is there a way to pan this window?
	// Can we pan the window to any direction?
	if(!m_pRealisation || !m_pParentWnd || (m_bNoVertScroll && m_bNoHorzScroll))
	{
		delete this;
		return FALSE;
	}

	// start xtraverse mods
//	HWND app = ::GetAncestor(pParentWnd->GetSafeHwnd(), GA_ROOTOWNER);
//	::SendMessage(app, WM_SETREDRAW, FALSE, 0);
	// end xtraverse mods

	// Get the Window!
	if (!CreateEx(
			WS_EX_TOOLWINDOW,
			AfxRegisterWndClass(CS_SAVEBITS), NULL, WS_CLIPSIBLINGS|WS_POPUP,
			0, 0, 1, 1,
			NULL, NULL))
	{
		TRACE0("Failed to create window in CreateEx\n");
		ASSERT(0);
		return FALSE;
	}

	CRgn rgn;
	m_pRealisation->GetBitmapDimensions(bitmapSize, m_pParentWnd);
	m_pRealisation->CreateWindowRegion(rgn, m_pParentWnd);

	CRect rcWndDrag;
	rgn.GetRgnBox(&rcWndDrag);

	m_WindowSize.cx = rcWndDrag.Width();
	m_WindowSize.cy = rcWndDrag.Height();

	rcWndDrag.TopLeft().x = 0;
	rcWndDrag.TopLeft().y = 0;
	rcWndDrag.OffsetRect(-rcWndDrag.Width()/2, -rcWndDrag.Height()/2);

	GetCursorPos(&m_ptWheelOrg);
	rcWndDrag.OffsetRect(m_ptWheelOrg.x, m_ptWheelOrg.y);

	SetWindowRgn(rgn, FALSE);
	MoveWindow(rcWndDrag);

	if (m_pRealisation->GetWindowResID(m_pParentWnd) != 0) 
	{
		m_bmpOrigin.LoadBitmap(m_pRealisation->GetWindowResID(m_pParentWnd));
		ShowWindow(SW_SHOW);
	}

	SetCapture();
	m_nWheelTimer = SetTimer(1968, TIMER_DELAY_MS, NULL);

	m_nOriginBmpIndex=bitmapSize.cx;
	if(!m_bNoVertScroll)
	{
		m_nOriginBmpIndex=bitmapSize.cx * 2;
		if(!m_bNoHorzScroll)
			m_nOriginBmpIndex=0;
	}
	
	// start xtraverse mods
//	::SendMessage(app, WM_NCACTIVATE, TRUE, 0);
//	::SendMessage(app, WM_SETREDRAW, TRUE, 0);
	// end xtraverse mods

	return TRUE;
}


BEGIN_MESSAGE_MAP(CWheelWnd, CWnd)
	//{{AFX_MSG_MAP(CWheelWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_KILLFOCUS, DestroyThisWindow)
	ON_MESSAGE(WM_RBUTTONDOWN, DestroyThisWindow)
	ON_MESSAGE(WM_MBUTTONDOWN, DestroyThisWindow)
	ON_MESSAGE(WM_LBUTTONDOWN, DestroyThisWindow)
	ON_MESSAGE(WM_CANCELMODE, DestroyThisWindow)
	ON_MESSAGE(WM_SYSKEYDOWN, DestroyThisWindow)
	ON_MESSAGE(WM_KEYDOWN, DestroyThisWindow)
	ON_MESSAGE(WM_CHAR, DestroyThisWindow)
	ON_MESSAGE(WM_MOUSEWHEEL, DestroyThisWindow)
//	ON_MESSAGE(WM_CAPTURECHANGED, DestroyThisWindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWheelWnd message handlers

void CWheelWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(rect);

	// Draw bitmap
	if (rect.Width() >= bitmapSize.cx && rect.Height() >= bitmapSize.cy)
	{
		// must have at least the first bitmap loaded before calling DrawItem
		ASSERT(m_bmpOrigin.m_hObject != NULL);     // required

		int x = rect.left + max(1, (rect.Width()-bitmapSize.cx)/2);
		int y = rect.top + max(1, (rect.Height()-bitmapSize.cy)/2);

		CDC dcMem;
		dcMem.CreateCompatibleDC(NULL);
		CBitmap* pOldBmp=dcMem.SelectObject(&m_bmpOrigin);
		dc.BitBlt(x, y, bitmapSize.cx, bitmapSize.cy, &dcMem, m_nOriginBmpIndex, 0, SRCCOPY);
		dcMem.SelectObject(pOldBmp);
	}
}

void CWheelWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == m_nWheelTimer)
	{
		CPoint ptCur;
		GetCursorPos(&ptCur);
		CPoint pt = ptCur - m_ptWheelOrg;

		// Number of pixels to scroll per second, depending on 
		// distance from center
		int nScrollX, nScrollY;

		// calculate the scrolling distances
		// if one direction is not possible, set it to 0
		if (m_bNoHorzScroll || abs(pt.x) < m_WindowSize.cx/2) nScrollX = 0;
		else if (m_wStyle & MFX_WHEELWNDSTYLE_NOSUBPIXELSCROLLING) nScrollX = (pt.x/8)*16;
		else if (abs(pt.x) > m_WindowSize.cx/2+42) nScrollX = ((pt.x>=0)?2:-2) * ((int) pow(abs(pt.x)-m_WindowSize.cx/2-42,1.1) + 43);
		else nScrollX = (pt.x+((pt.x>0)?-(m_WindowSize.cx/2-1):(m_WindowSize.cx/2-1)))*2;

		if (m_bNoVertScroll || abs(pt.y) < m_WindowSize.cy/2) nScrollY = 0;
		else if (m_wStyle & MFX_WHEELWNDSTYLE_NOSUBPIXELSCROLLING) nScrollY = (pt.y/8)*16;
		else if (abs(pt.y) > m_WindowSize.cy/2+42) nScrollY = ((pt.y>=0)?2:-2) * ((int) pow(abs(pt.y)-m_WindowSize.cy/2-42,1.1) + 43);
		else nScrollY = (pt.y+((pt.y>0)?-(m_WindowSize.cy/2-1):(m_WindowSize.cy/2-1)))*2;

		// if only one direction is allowed same time,
		// set the smaller value to 0
		if (m_wStyle & MFX_WHEELWNDSTYLE_ONEDIRECTION)
		{
			if (abs(pt.x) > abs(pt.y)) nScrollY = 0;
			else nScrollX = 0;
		}

		// Calculate number of 100ths of a pixel to scroll in this time slice
		// and Accumulate the 100ths of a pixel that we need to scroll.
		m_ptScrollSum.x += MulDiv(nScrollX,TIMER_DELAY_MS,10);
		m_ptScrollSum.y += MulDiv(nScrollY,TIMER_DELAY_MS,10);

		// Some Windows Scroll by Pixel, some by Line
		// and the ListView in REPORT Mode does some calculations internal...
		int nPixelsInAStepX = m_pRealisation->GetPixelsInAStepX(m_pParentWnd, nScrollX);
		int nPixelsInAStepY = m_pRealisation->GetPixelsInAStepY(m_pParentWnd, nScrollY);

		// How many steps to scroll?
		int nScrollStepsX=m_ptScrollSum.x/nPixelsInAStepX/100;
		int nScrollStepsY=m_ptScrollSum.y/nPixelsInAStepY/100;

		// Dof we really need to scroll?
		if (nScrollStepsX || nScrollStepsY)
		{
			// Scroll it.
			m_pRealisation->DoScroll(m_pParentWnd, nScrollStepsX, nScrollStepsY);

			// Correct the accumulated sum by the number of 100ths of 
			// a pixel we just scrolled.
			m_ptScrollSum.x-=nScrollStepsX*nPixelsInAStepX*100;
			m_ptScrollSum.y-=nScrollStepsY*nPixelsInAStepY*100;
		}

		SetCursor(m_pRealisation->GetCursorResID(nScrollX, nScrollY, m_bNoHorzScroll?true:false, m_bNoVertScroll?true:false));
	}
	else
		CWnd::OnTimer(nIDEvent);
}

LONG CWheelWnd::DestroyThisWindow(WPARAM, LPARAM)
{
	VERIFY(DestroyWindow());
	return 0;
}

void CWheelWnd::OnDestroy() 
{
	ReleaseCapture();
	KillTimer(m_nWheelTimer);
	::SetCursor(NULL);

	CWnd::OnDestroy();
}

void CWheelWnd::PostNcDestroy()
{
	CWnd::PostNcDestroy();
	delete this;
}

void CWheelWnd::OnMButtonUp(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);

	if ((abs(point.x-m_ptWheelOrg.x) > 12 || abs(point.y-m_ptWheelOrg.y) > 12)) VERIFY(DestroyWindow());
	else CWnd::OnMButtonUp(nFlags, point);
}

void CWheelWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);

	if ((abs(point.x-m_ptWheelOrg.x) > 12 || abs(point.y-m_ptWheelOrg.y) > 12)) VERIFY(DestroyWindow());
	else CWnd::OnLButtonUp(nFlags, point);
}

void CWheelWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);

	if ((abs(point.x-m_ptWheelOrg.x) > 12 || abs(point.y-m_ptWheelOrg.y) > 12)) VERIFY(DestroyWindow());
	else CWnd::OnRButtonUp(nFlags, point);
}

void CWheelWnd::SetCursor(int nCursor)
{
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nCursor),RT_GROUP_CURSOR);
	HICON hCursor = ::LoadCursor(hInst,MAKEINTRESOURCE(nCursor));
	::SetCursor(hCursor);
}
