/////////////////////////////////////////////////////////////////////////////
// Rulers. Written By Stefan Ungureanu (stefanu@usa.net)
//

#include "stdafx.h"
#include "Ruler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRulerSplitterWnd

CRulerSplitterWnd::CRulerSplitterWnd()
{
	m_cxSplitter     = 0;
	m_cySplitter     = 0;
	m_cxBorderShare  = 0;
	m_cyBorderShare  = 0;
	m_cxSplitterGap  = 1;
	m_cySplitterGap  = 1;
    m_bRulersVisible = FALSE;
}

CRulerSplitterWnd::~CRulerSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CRulerSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CRulerSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CRulerSplitterWnd::CreateRulers(CFrameWnd* pParent, CCreateContext* pContext)
{
	if (!CreateStatic(pParent, 2, 2))				
		return FALSE;														
																			
	if (!CreateView(0, 0, RUNTIME_CLASS(CRulerCornerView), CSize(0,0), pContext))				
		return FALSE;														
	if (!CreateView(0, 1, RUNTIME_CLASS(CRulerView), CSize(0,0), pContext))					
		return FALSE;														
	if (!CreateView(1, 0, RUNTIME_CLASS(CRulerView), CSize(0,0), pContext))					
		return FALSE;														
	if (!CreateView(1, 1, pContext->m_pNewViewClass, CSize(0,0), pContext))					
		return FALSE;														
																			
	SetColumnInfo(0, 0, 0);				
	SetRowInfo(0, 0, 0);				
																			
	((CRulerView*)GetPane(0, 1))->SetRulerType(RT_HORIZONTAL);										
	((CRulerView*)GetPane(1, 0))->SetRulerType(RT_VERTICAL);
    
    SetActivePane(1, 1);	
    
    return TRUE;
}

void CRulerSplitterWnd::ShowRulers(BOOL bShow, BOOL bSave)
{
    int nSize       = (bShow)?RULER_SIZE:0;
    int nSizeBorder = (bShow)?3:1;

    SetRowInfo(0, nSize, 0);
    SetColumnInfo(0, nSize, 0);
    m_cxSplitterGap  = nSizeBorder;
	m_cySplitterGap  = nSizeBorder;
    m_bRulersVisible = (bSave)?bShow:m_bRulersVisible;
    RecalcLayout();
}

void CRulerSplitterWnd::UpdateRulersInfo(stRULER_INFO stRulerInfo)
{
	((CRulerView*)GetPane(0, 1))->UpdateRulersInfo(stRulerInfo);
	((CRulerView*)GetPane(1, 0))->UpdateRulersInfo(stRulerInfo);

    if (((int)(stRulerInfo.DocSize.cx*stRulerInfo.fZoomFactor) < stRulerInfo.DocSize.cx) ||
        ((int)(stRulerInfo.DocSize.cy*stRulerInfo.fZoomFactor) < stRulerInfo.DocSize.cy))
        ShowRulers(FALSE, FALSE);
    else if (m_bRulersVisible)
        ShowRulers(TRUE, FALSE);
}

void CRulerSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
    //Lock Splitter
    //CSplitterWnd::OnLButtonDown(nFlags, point);
}

void CRulerSplitterWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
    //Lock Splitter
	//CSplitterWnd::OnMouseMove(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
// CRulerSplitterWnd message handlers

/////////////////////////////////////////////////////////////////////////////
// CRulerView

IMPLEMENT_DYNCREATE(CRulerView, CView)

CRulerView::CRulerView()
{
	m_rulerType   = RT_HORIZONTAL;
	m_scrollPos   = 0;
	m_lastPos     = 0;
    m_fZoomFactor = 1;
}

CRulerView::~CRulerView()
{
}


BEGIN_MESSAGE_MAP(CRulerView, CView)
	//{{AFX_MSG_MAP(CRulerView)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRulerView drawing
void CRulerView::UpdateRulersInfo(stRULER_INFO stRulerInfo)
{
    m_DocSize     = stRulerInfo.DocSize;
    m_fZoomFactor = stRulerInfo.fZoomFactor;
    m_scrollPos   = stRulerInfo.ScrollPos;

    if (stRulerInfo.uMessage == RW_POSITION) {
        DrawCursorPos(stRulerInfo.Pos);
    }
    else if ((m_rulerType == RT_HORIZONTAL) && (stRulerInfo.uMessage == RW_HSCROLL) ||
             (m_rulerType == RT_VERTICAL) && (stRulerInfo.uMessage == RW_VSCROLL)) {
    
        CDC* pDC = GetDC();
        OnDraw(pDC);
        ReleaseDC(pDC);
    }
    else
        Invalidate();
} 

void CRulerView::OnDraw(CDC* pDC)
{ 
	m_lastPos = 0;
    
	// set the map mode right
	int oldMapMode=pDC->SetMapMode(MM_TEXT);

	CFont vFont;
	CFont hFont;
	vFont.CreateFont(10, 0, 900, 900, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY, VARIABLE_PITCH|FF_ROMAN, "Times New Roman");
	hFont.CreateFont(12, 0, 000, 000, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY, VARIABLE_PITCH|FF_ROMAN, "Times New Roman");
	
    CFont* pOldFont  = pDC->SelectObject((m_rulerType==RT_HORIZONTAL)?&hFont:&vFont);
	int oldTextAlign = pDC->SetTextAlign((m_rulerType==RT_HORIZONTAL)?(TA_RIGHT|TA_TOP):(TA_LEFT|TA_TOP));
	int oldBkMode    = pDC->SetBkMode(TRANSPARENT);

	CRect rulerRect;
	GetClientRect(&rulerRect);

	if (m_rulerType==RT_HORIZONTAL)
	{
		rulerRect.right = (int)(m_DocSize.cx*m_fZoomFactor);
    }
	else //(m_rulerType==RT_VERTICAL)
	{
		rulerRect.bottom = (int)(m_DocSize.cy*m_fZoomFactor);
	}

	pDC->FillSolidRect(rulerRect, RGB(255, 255, 255));

    DrawTicker(pDC, rulerRect, 100);
    DrawTicker(pDC, rulerRect, 10, 10, FALSE);
    DrawTicker(pDC, rulerRect, 5, 12, FALSE);

	// restore DC settings
	pDC->SetMapMode(oldMapMode);
    pDC->SelectObject(pOldFont);
	pDC->SetTextAlign(oldTextAlign);
	pDC->SetBkMode(oldBkMode);
}

void CRulerView::DrawTicker(CDC* pDC, CRect rulerRect, int nFactor, int nBegin, BOOL bShowPos)
{
    int nSize  = (m_rulerType == RT_HORIZONTAL)?rulerRect.right:rulerRect.bottom;
    int nTick  = (int)(nFactor*m_fZoomFactor);
 
    for (int i=1; i<=nSize/nFactor; i++)
	{
		char buffer[100];
		sprintf(buffer, "%d", i*nFactor);

		if (m_rulerType==RT_HORIZONTAL)
		{
            pDC->PatBlt(nTick*i-m_scrollPos.x, rulerRect.top+nBegin, 1, rulerRect.bottom, BLACKNESS);
			if (bShowPos)
                pDC->TextOut(nTick*i-m_scrollPos.x, rulerRect.top, CString(buffer));
		}
		else //(m_rulerType==RT_VERTICAL)
		{
            pDC->PatBlt(rulerRect.left+nBegin, nTick*i-m_scrollPos.y, rulerRect.right, 1, BLACKNESS);
			if (bShowPos)
			    pDC->TextOut(rulerRect.left, nTick*i-m_scrollPos.y, CString(buffer));
		}
	}
}

void CRulerView::DrawCursorPos(CPoint NewPos)
{
	if (((m_rulerType == RT_HORIZONTAL) && (NewPos.x > m_DocSize.cx*m_fZoomFactor)) ||
		((m_rulerType == RT_VERTICAL) && ((NewPos.y) > m_DocSize.cy*m_fZoomFactor)))
		return;

	CDC* pDC = GetDC();
	// set the map mode right
	int oldMapMode = pDC->SetMapMode(MM_TEXT);

	CRect clientRect;
	GetClientRect(&clientRect);
	if (m_rulerType==RT_HORIZONTAL)
	{
		// erase the previous position
        pDC->PatBlt(m_lastPos.x, clientRect.top, 1, clientRect.bottom, DSTINVERT);
		// draw the new position
		m_lastPos.x = NewPos.x;
        pDC->PatBlt(m_lastPos.x, clientRect.top, 1, clientRect.bottom, DSTINVERT);
	}
	else // (m_rulerType==RT_VERTICAL)
	{
		// erase the previous position
        pDC->PatBlt(clientRect.left, m_lastPos.y, clientRect.right, 1, DSTINVERT);
		// draw the new position
		m_lastPos.y = NewPos.y;
        pDC->PatBlt(clientRect.left, m_lastPos.y, clientRect.right, 1, DSTINVERT);
	}

	pDC->SetMapMode(oldMapMode);
	ReleaseDC(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CRulerView diagnostics

#ifdef _DEBUG
void CRulerView::AssertValid() const
{
	CView::AssertValid();
}

void CRulerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRulerView message handlers

BOOL CRulerView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// create a brush using the same color as the background
	if (cs.lpszClass == NULL)
	{
		HBRUSH hBr=CreateSolidBrush(g_PaintManager->GetColor(COLOR_MENU));
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS|CS_BYTEALIGNWINDOW, NULL, hBr);
	}

	return CView::PreCreateWindow(cs);
}

void CRulerView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
    ((CSplitterWnd*)GetParent())->SetActivePane(1, 1);	
}

void CRulerView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView

IMPLEMENT_DYNCREATE(CRulerCornerView, CView)

CRulerCornerView::CRulerCornerView()
{
}

CRulerCornerView::~CRulerCornerView()
{
}


BEGIN_MESSAGE_MAP(CRulerCornerView, CView)
	//{{AFX_MSG_MAP(CRulerCornerView)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView drawing

void CRulerCornerView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView diagnostics

#ifdef _DEBUG
void CRulerCornerView::AssertValid() const
{
	CView::AssertValid();
}

void CRulerCornerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView message handlers

BOOL CRulerCornerView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// create a brush using the same color as the background
	if (cs.lpszClass == NULL)
	{
		HBRUSH hBr=CreateSolidBrush(g_PaintManager->GetColor(COLOR_MENU));
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS|CS_BYTEALIGNWINDOW, NULL, hBr);
	}

	return CView::PreCreateWindow(cs);
}

void CRulerCornerView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
    ((CSplitterWnd*)GetParent())->SetActivePane(1, 1);	
}

