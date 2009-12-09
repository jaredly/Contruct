
/*
 * File:		FlatSplitterWnd.cpp
 *
 * Author:		Marc Richarme <devix@devix.cjb.net>
 *
 * Created:		28 Jan. 2001
 * Modified:	28 Jan. 2001
 * Modified:	25 Jan. 2003 by Kris <krisoftware@interia.pl>
 *
 * Use as much as you want, wherever you want...
 * Claim you coded it, I don't mind.
 *
 */

#include "stdafx.h"

#include "FlatSplitterWnd.h"

#include <afxpriv.h>
#include "..\..\ChildFrame.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlatSplitterWnd

#define FOCUS_HILIGHT_COLOR_ULO RGB(180, 75, 25)
#define FOCUS_HILIGHT_COLOR_LRO RGB(245, 5, 25)
#define FOCUS_HILIGHT_COLOR_ULI RGB(145, 95, 75)
#define FOCUS_HILIGHT_COLOR_LRI RGB(220, 65, 40)
#define SPLITTER_CX 5
#define SPLITTER_CY 5
#define SPLITTER_GAPX 5
#define SPLITTER_GAPY 5

CFlatSplitterWnd::CFlatSplitterWnd()
{
	m_cxSplitter = m_cySplitter = 3 + 1 + 1;
	m_cxBorderShare = m_cyBorderShare = 0;
	m_cxSplitterGap = m_cySplitterGap = 3 + 1 + 1;
	m_cxBorder = m_cyBorder = 1;
	cRow = 0;
	cCol = 0;
	tab = 0;
	m_w = 0;
	m_w1 = 0;

	m_cxSplitter = SPLITTER_CX;
	m_cySplitter = SPLITTER_CY;
	m_cxSplitterGap = SPLITTER_GAPX;
	m_cySplitterGap = SPLITTER_GAPY;

}

CFlatSplitterWnd::~CFlatSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CFlatSplitterWnd, CExtSplitterWnd)
	//{{AFX_MSG_MAP(CFlatSplitterWnd)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFlatSplitterWnd message handlers

//#ifdef false
void CFlatSplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
	// Let CExtSplitterWnd handle everything but the border-drawing
	if((nType != splitBorder) || (pDC == NULL))
	{
		CExtSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);
		return;
	}

	ASSERT_VALID(pDC);

	pDC->Draw3dRect(rectArg, g_PaintManager->GetColor(COLOR_BTNSHADOW), g_PaintManager->GetColor(COLOR_BTNHIGHLIGHT));

	if(((GetRowCount()>1) || (GetColumnCount()>1)) && (nType == splitBorder)) {
		int pRow = 0;
		int pCol = 0;
		if(rectArg.top) {
			pRow = 1;
		}
		if(rectArg.left) {
			pCol = 1;
		}
		GetActivePane(&cRow, &cCol);
		if(cRow < 0) cRow = 0;
		if(cCol < 0) cCol = 0;
		if((cRow == pRow) && (cCol == pCol)) 
		{
			cCol = pCol;
			cRow = pRow;
			CRect rect = rectArg;
		//	GetActivePane()->GetWindowRect(rect);
			//ScreenToClient(rect);
			//rect.InflateRect(1,1);

			if (pDC == NULL) {
				RedrawWindow(rect, NULL, RDW_INVALIDATE|RDW_NOCHILDREN);
				return;
			}
			ASSERT_VALID(pDC);
		
			
			pDC->Draw3dRect(rect, FOCUS_HILIGHT_COLOR_ULO, FOCUS_HILIGHT_COLOR_LRO);
			rect.InflateRect(-GetSystemMetrics(SM_CXBORDER), -GetSystemMetrics(SM_CYBORDER));
			pDC->Draw3dRect(rect, FOCUS_HILIGHT_COLOR_ULI, FOCUS_HILIGHT_COLOR_LRI);
			return;
		}
	}

}

/* Original Method Implementation:
*
* AFX_STATIC void AFXAPI _AfxLayoutRowCol(CExtSplitterWnd::CRowColInfo* pInfoArray,
*	int nMax, int nSize, int nSizeSplitter)
*
* In WINSPLIT.CPP
*
* Credit for this adaptation goes to Kris.
*/
// Generic routine:
//  for X direction: pInfo = m_pColInfo, nMax = m_nMaxCols, nSize = cx
//  for Y direction: pInfo = m_pRowInfo, nMax = m_nMaxRows, nSize = cy


//#endif
void CFlatSplitterWnd::LayoutRowCol(CExtSplitterWnd::CRowColInfo* pInfoArray,
	int nMax, int nSize, int nSizeSplitter)
{
	ASSERT(pInfoArray != NULL);
	ASSERT(nMax > 0);
	ASSERT(nSizeSplitter > 0);

	CExtSplitterWnd::CRowColInfo* pInfo;
	int i;

	if (nSize < 0)
		nSize = 0;  // if really too small, layout as zero size

	// start with ideal sizes
	for (i = 0, pInfo = pInfoArray; i < nMax-1; i++, pInfo++)
	{
		if (pInfo->nIdealSize < pInfo->nMinSize)
			pInfo->nIdealSize = 0;      // too small to see
		pInfo->nCurSize = pInfo->nIdealSize;
	}
	pInfo->nCurSize = INT_MAX;  // last row/column takes the rest

	for (i = 0, pInfo = pInfoArray; i < nMax; i++, pInfo++)
	{
		ASSERT(nSize >= 0);
		if (nSize == 0)
		{
			// no more room (set pane to be invisible)
			pInfo->nCurSize = 0;
			continue;       // don't worry about splitters
		}
		else if (nSize < pInfo->nMinSize && i != 0)
		{
			// additional panes below the recommended minimum size
			//   aren't shown and the size goes to the previous pane
			pInfo->nCurSize = 0;

			// previous pane already has room for splitter + border
			//   add remaining size and remove the extra border
			(pInfo-1)->nCurSize += nSize + 2;
			nSize = 0;
		}
		else
		{
			// otherwise we can add the second pane
			ASSERT(nSize > 0);
			if (pInfo->nCurSize == 0)
			{
				// too small to see
				if (i != 0)
					pInfo->nCurSize = 0;
			}
			else if (nSize < pInfo->nCurSize)
			{
				// this row/col won't fit completely - make as small as possible
				pInfo->nCurSize = nSize;
				nSize = 0;
			}
			else
			{
				// can fit everything
				nSize -= pInfo->nCurSize;
			}
		}

		// see if we should add a splitter
		ASSERT(nSize >= 0);
		if (i != nMax - 1)
		{
			// should have a splitter
			if (nSize > nSizeSplitter)
			{
				nSize -= nSizeSplitter; // leave room for splitter + border
				ASSERT(nSize > 0);
			}
			else
			{
				// not enough room - add left over less splitter size
				pInfo->nCurSize += nSize;
				if (pInfo->nCurSize > (nSizeSplitter - 2))
					pInfo->nCurSize -= (nSizeSplitter - 2);
				nSize = 0;
			}
		}
	}
	ASSERT(nSize == 0); // all space should be allocated
}

/* Original Method Implementation:
*
* AFX_STATIC void AFXAPI _AfxDeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
*	CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar)
*
* In WINSPLIT.CPP
*
* Credit for this adaptation goes to Kris.
*/
// repositions client area of specified window
// assumes everything has WS_BORDER or is inset like it does
//  (includes scroll bars)
void CFlatSplitterWnd::DeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
	CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar)
{
	ASSERT(pWnd != NULL);
	ASSERT(pWnd->m_hWnd != NULL);

	if (bScrollBar)
	{
		// if there is enough room, draw scroll bar without border
		// if there is not enough room, set the WS_BORDER bit so that
		//   we will at least get a proper border drawn
		BOOL bNeedBorder = (cx <= 1 || cy <= 1);
		pWnd->ModifyStyle(bNeedBorder ? 0 : 1, bNeedBorder ? 1 : 0);
	}
	CRect rect(x, y, x+cx, y+cy);

	// adjust for 3d border (splitter windows have implied border)
	if ((pWnd->GetExStyle() & WS_EX_CLIENTEDGE) || pWnd->IsKindOf(RUNTIME_CLASS(CExtSplitterWnd)))
	{
		rect.InflateRect(1, 1); // for proper draw CFlatSplitterWnd in view
//		rect.InflateRect(afxData.cxBorder2, afxData.cyBorder2);
	}

	// first check if the new rectangle is the same as the current
	CRect rectOld;
	pWnd->GetWindowRect(rectOld);
	pWnd->GetParent()->ScreenToClient(&rectOld);
	if (rect != rectOld)
		AfxRepositionWindow(lpLayout, pWnd->m_hWnd, rect);
}

/* Original Method Implementation:
*
* void CExtSplitterWnd::RecalcLayout()
*
* In WINSPLIT.CPP
*
* Credit for this adaptation goes to Kris.
*/

#define IDC_TAB 189

void CFlatSplitterWnd::RecalcLayout()
{
	ASSERT_VALID(this);
	ASSERT(m_nRows > 0 && m_nCols > 0); // must have at least one pane

	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.InflateRect(-m_cxBorder, -m_cyBorder);

	CRect rectInside;
	GetInsideRect(rectInside);

	// layout columns (restrict to possible sizes)
	LayoutRowCol(m_pColInfo, m_nCols, rectInside.Width(), m_cxSplitterGap);
	LayoutRowCol(m_pRowInfo, m_nRows, rectInside.Height(), m_cySplitterGap);

	// adjust the panes (and optionally scroll bars)

	// give the hint for the maximum number of HWNDs
	AFX_SIZEPARENTPARAMS layout;
	layout.hDWP = ::BeginDeferWindowPos((m_nCols + 1) * (m_nRows + 1) + 1 + 1); //xtra + 1

	// size of scrollbars
	int cx = rectClient.right - rectInside.right;
	int cy = rectClient.bottom - rectInside.bottom;

	// reposition size box
	if (m_bHasHScroll && m_bHasVScroll)
	{
		CWnd* pScrollBar = GetDlgItem(AFX_IDW_SIZE_BOX);
		ASSERT(pScrollBar != NULL);

		// fix style if necessary
		BOOL bSizingParent = (GetSizingParent() != NULL);
		// modifyStyle returns TRUE if style changes
		if (pScrollBar->ModifyStyle(SBS_SIZEGRIP|SBS_SIZEBOX,
				bSizingParent ? SBS_SIZEGRIP : SBS_SIZEBOX))
			pScrollBar->Invalidate();
		pScrollBar->EnableWindow(bSizingParent);

		// reposition the size box
		DeferClientPos(&layout, pScrollBar,
			rectInside.right,
			rectInside.bottom, cx, cy, TRUE);
	}

	// reposition scroll bars
	if (m_bHasHScroll)
	{
		int cxSplitterBox = m_cxSplitter;// split box bigger
		int x = rectClient.left;
		int y = rectInside.bottom;
		for (int col = 0; col < m_nCols; col++)
		{
			CWnd* pScrollBar = GetDlgItem(AFX_IDW_HSCROLL_FIRST + col);
			ASSERT(pScrollBar != NULL);
			int cx = m_pColInfo[col].nCurSize;
			if (col == 0 && m_nCols < m_nMaxCols)
				x += cxSplitterBox, cx -= cxSplitterBox;
			if (col == 0) {
				CWnd* pTab = GetDlgItem(IDC_TAB);
				ASSERT(pTab != NULL);
				CRect rc;
				m_pTabs->GetWindowRect(&rc);
				if (m_w != 0) {
					double ratio = ((double)m_w1) / ((double)m_w);
					long width = (long)min(240,max(150, min(((double)cx) * ratio, cx - 32)));
					DeferClientPos(&layout, pTab, x, y, width, cy/*+4*/, TRUE);
					DeferClientPos(&layout, pScrollBar, x+width, y, cx-width, cy, TRUE);
				}
				else {
					DeferClientPos(&layout, pTab, x, y, rc.Width(), cy/*+4*/, TRUE);
					DeferClientPos(&layout, pScrollBar, x+rc.Width(), y, cx-rc.Width(), cy, TRUE);
					m_w1 = rc.Width();
					m_w = cx;
				}
			}
			else {
				DeferClientPos(&layout, pScrollBar, x, y, cx, cy, TRUE);
			}
			x += cx + m_cxSplitterGap;
		}
	}

	if (m_bHasVScroll)
	{
		int cySplitterBox = m_cySplitter;// split box bigger
		int x = rectInside.right;
		int y = rectClient.top;
		for (int row = 0; row < m_nRows; row++)
		{
			CWnd* pScrollBar = GetDlgItem(AFX_IDW_VSCROLL_FIRST + row);
			ASSERT(pScrollBar != NULL);
			int cy = m_pRowInfo[row].nCurSize;
			if (row == 0 && m_nRows < m_nMaxRows)
				y += cySplitterBox, cy -= cySplitterBox;
			DeferClientPos(&layout, pScrollBar, x, y, cx, cy, TRUE);
			y += cy + m_cySplitterGap;
		}
	}

	//BLOCK: Reposition all the panes
	{
		int x = rectClient.left;
		for (int col = 0; col < m_nCols; col++)
		{
			int cx = m_pColInfo[col].nCurSize;
			int y = rectClient.top;
			for (int row = 0; row < m_nRows; row++)
			{
				int cy = m_pRowInfo[row].nCurSize;
				CWnd* pWnd = GetPane(row, col);
				DeferClientPos(&layout, pWnd, x, y, cx, cy, FALSE);
				y += cy + m_cySplitterGap;
			}
			x += cx + m_cxSplitterGap;
		}
	}

	// move and resize all the windows at once!
	if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
		TRACE0("Warning: DeferWindowPos failed - low system resources.\n");

	// invalidate all the splitter bars (with NULL pDC)
	DrawAllSplitBars(NULL, rectInside.right, rectInside.bottom);
}

BOOL CFlatSplitterWnd::CreateScrollBarCtrl(DWORD dwStyle, UINT nID) 
{
	if (nID == AFX_IDW_HSCROLL_FIRST) {
		m_pTabs->Create(this, CRect(0,0,256,20), IDC_TAB, WS_CHILD | WS_VISIBLE, __ETWS_ORIENT_BOTTOM);
		tab = 1;

		m_pTabs->ItemInsert(TAB_LAYOUTEDITOR, NULL, NULL, NULL, 0, NULL, NULL);
		m_pTabs->ItemInsert(TAB_EVENTSHEETEDITOR, NULL, NULL, NULL, 1, NULL, NULL);
		m_pTabs->SelectionSet(0);
	}


	//CExtSplitterWnd::CreateScrollBarCtrl(dwStyle, nID);
	
	CExtScrollBar* myscroll = new CExtScrollBar;
	if(nID == AFX_IDW_VSCROLL_FIRST)
	{
		myscroll->m_eSO = CExtScrollBar::__ESO_RIGHT;
	}
	myscroll->Create(dwStyle, CRect(0,0,256,20), this, nID);

	

//	CScrollBar* myscroll = new CScrollBar;
	//myscroll->Create(dwStyle, CRect(0,0,256,20), this, nID);


	/*HWND hWnd = ::CreateWindow(_T("SCROLLBAR"), NULL, dwStyle | WS_VISIBLE | WS_CHILD,
						0, 0, 1, 1, m_hWnd, (HMENU)nID, AfxGetInstanceHandle(), NULL);*/
		
	return TRUE;
}



void CFlatSplitterWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	ASSERT(pScrollBar != NULL);
	int col = pScrollBar->GetDlgCtrlID() - AFX_IDW_HSCROLL_FIRST;
	if(col > 1) 
		return;

	int activecol, activerow;

	GetActivePane(&activerow, &activecol);
	if(activerow < 0) activerow = 0;
	if(activecol < 0) activecol = 0;

	if(col!= activecol)
		SetActivePane(activerow, col);


	GetPane(activerow,col)->SendMessage(WM_HSCROLL,
			MAKELONG(nSBCode, nPos), (LPARAM)pScrollBar->m_hWnd);

}




void CFlatSplitterWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	ASSERT(pScrollBar != NULL);
	int row = pScrollBar->GetDlgCtrlID() - AFX_IDW_VSCROLL_FIRST;
	if(row > 1) 
		return;

	int activecol = 0, activerow = 0;

	GetActivePane(&activerow, &activecol);
	if(activerow < 0) activerow = 0;
	if(activecol < 0) activecol = 0;

	if(row != activerow)
		SetActivePane(row, activecol);


	GetPane(row, activecol)->SendMessage(WM_VSCROLL,
			MAKELONG(nSBCode, nPos), (LPARAM)pScrollBar->m_hWnd);
}




void CFlatSplitterWnd::SetActivePane(int row, int col, CWnd *pWnd)
{
	/*
	if(row != -1 && col != -1)
	{
		//update scrollbars
		CWnd* pane = GetPane(row, col);
		if (pane != NULL) {
			int x = pane->GetScrollPos(0);
			int y = pane->GetScrollPos(1);
		}
	}
	*/
	
	Invalidate();
	
	CExtSplitterWnd::SetActivePane(row, col, pWnd);


}


void CFlatSplitterWnd::RecalcLayout2(long w1, long w) 
{
	m_w1 = w1;
	m_w = w;
	RecalcLayout();
}

BOOL CFlatSplitterWnd::DoScroll(CView* pViewFrom, UINT nScrollCode, BOOL bDoScroll)
{
	//CExtSplitterWnd
	return FALSE;
}

BOOL CFlatSplitterWnd::OnMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
	bool result = CExtSplitterWnd::OnMouseWheel(fFlags, zDelta, point);

	CChildFrame* parent = dynamic_cast<CChildFrame*>(GetParent());
	if(parent)
	{
		if(parent->m_pEventView[cCol][cRow])
		{
			parent->m_pEventView[cCol][cRow]->OnMouseWheel(fFlags, zDelta, point);

		}
		
	}

	return result;

}