/////////////////////////////////////////////////////////////////////////////
//
// Disclaimer Notice
// ------------------
// The Author cannot guarantee this software is error free.
// The author also disclaims all responsibility for damages or 
// loss of profits caused by the failure of this software
//. Your usage of these modifications indicates your willingness to 
// accept complete responsibility.
//
//
// Developed by: Norm Almond for www.codeproject.com 
//
// Please use this software freely, if you fix or make any modifications to
// the software you can let me know so I can improve future versions.
//
// Version 1.00 - First Released 03 March 2001
//
/////////////////////////////////////////////////////////////////////////////

// PaletteWnd.cpp : implementation file
//

#include "stdafx.h"
//#include "../../SelColorDlg.h"
#include "PaletteWnd.h"
#include "ColorChooser.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BTN_SIZE 10

static COLORREF ColorMap[8][8]  = 
{
	{ RGB(255,255,255),  RGB(224, 224, 224), RGB(192, 192, 192), RGB(128, 128, 128), RGB(64,64,64), RGB(0, 0, 0),      RGB(255,255,255),  RGB(255,255,255)},
	{ RGB(255,192,192),  RGB(255, 128, 128), RGB(255, 0, 0),     RGB(192, 0, 0),     RGB(128,0, 0), RGB(64, 0, 0),     RGB(255,255,255),  RGB(255,255,255)},
	{ RGB(255,224, 192), RGB(255, 224, 128), RGB(255, 192, 0),   RGB(255, 128, 0),   RGB(128, 64, 0), RGB(128, 64, 64),RGB(255,255,255),  RGB(255,255,255)},
	{ RGB(255,255, 192), RGB(255, 255, 128), RGB(255, 255, 0),   RGB(192, 192, 0),   RGB(128,128, 0), RGB(64, 64, 0),  RGB(255,255,255),  RGB(255,255,255)},
	{ RGB(192,255, 192), RGB(128, 255, 128), RGB(0, 255, 0),     RGB(0, 192, 0),     RGB(0, 128, 0), RGB(0, 64, 0),    RGB(255,255,255),  RGB(255,255,255)},
	{ RGB(192,255,255),  RGB(128, 255, 255), RGB(0, 255, 255),   RGB(0, 192, 192),   RGB(0,128,128), RGB(0, 64, 64),   RGB(255,255,255),  RGB(255,255,255)},
	{ RGB(192,192,255),  RGB(128, 128, 255), RGB(0, 0, 255),     RGB(0, 0, 192),     RGB(0,0,128), RGB(0, 0, 64),      RGB(255,255,255),  RGB(255,255,255)},
	{ RGB(255,192,255),  RGB(255, 128, 255), RGB(255, 0, 255),   RGB(192, 0, 192),   RGB(128,0,128), RGB(64, 0, 64),   RGB(255,255,255),  RGB(255,255,255)}
};


/////////////////////////////////////////////////////////////////////////////
// CPaletteWnd

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::CPaletteWnd
// 
// DESCRIPTION:		C'tor, restores custom colors to user custom color settings
// 
// ACCESSIBILTY:	Public
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
CPaletteWnd::CPaletteWnd()
{
	m_bDrag = false;
	m_rc.SetRectEmpty();
	m_nIndex = -1;

	int nIndex = 0;
	for (int nRow=0; nRow < 8; nRow++)
	{
		for (int nCol=6; nCol < 8; nCol++)
		{
			COLORREF cr;
			UINT	 nColor=0;
			CString strIndex;
			strIndex.Format("Color:%d",nIndex++);
			
			PBYTE pData = (PBYTE) &cr;
			if (!AfxGetApp()->GetProfileBinary(_T("Custom Colors"), strIndex, &pData, &nColor)) {
				int nIndex2 = 0;
				for (int nRow2=0; nRow2 < 8; nRow2++)
				{
					for (int nCol2=6; nCol2 < 8; nCol2++)
					{
						COLORREF cr2 = ColorMap[nRow2][nCol2];			

						UINT	 nSize2=0;
						CString strIndex2;
						strIndex2.Format("Color:%d",nIndex2++);
						
						AfxGetApp()->WriteProfileBinary(_T("Custom Colors"), strIndex2, (PBYTE) &cr2, sizeof(COLORREF));		
					}
				}
				AfxGetApp()->GetProfileBinary(_T("Custom Colors"), strIndex, &pData, &nColor);
			}

			if (nColor == 0)
				cr = RGB(255, 255, 255);
			else
			{
				// Copy over 
				CopyMemory(&cr, pData, nColor);
				delete pData; // Delete 
			}

			// Flood Custom Colors
			ColorMap[nRow][nCol] = cr;			
		}
	}	
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::~CPaletteWnd
// 
// DESCRIPTION:		D'tor, saves user custom color settings
// 
// ACCESSIBILTY:	Public
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
CPaletteWnd::~CPaletteWnd()
{
	int nIndex = 0;
	for (int nRow=0; nRow < 8; nRow++)
	{
		for (int nCol=6; nCol < 8; nCol++)
		{
			COLORREF cr = ColorMap[nRow][nCol];			

			UINT	 nSize=0;
			CString strIndex;
			strIndex.Format("Color:%d",nIndex++);
			
			AfxGetApp()->WriteProfileBinary(_T("Custom Colors"), strIndex, (PBYTE) &cr, sizeof(COLORREF));		
		}
	}	
}


BEGIN_MESSAGE_MAP(CPaletteWnd, CWnd)
	//{{AFX_MSG_MAP(CPaletteWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPaletteWnd message handlers

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::Create
// 
// DESCRIPTION:		Window creation routine
// 
// ACCESSIBILTY:	Public
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
BOOL CPaletteWnd::Create(CPoint pt, CWnd *pParent, UINT nID)
{
	DWORD dwStyle = WS_CHILD ;

	int nBorderX = GetSystemMetrics(SM_CXBORDER);
	int nBorderY = GetSystemMetrics(SM_CYBORDER);

	CRect rc;
	rc.SetRect(pt.x, pt.y, pt.x + (BTN_SIZE * 8) + 11 + (nBorderX*2), pt.y + (BTN_SIZE * 8) + 11 + (nBorderY * 2));

	return CWnd::Create(NULL, NULL, dwStyle, rc, pParent, nID);
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::OnPaint
// 
// DESCRIPTION:		Draw palette 
// 
// ACCESSIBILTY:	Public
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
void CPaletteWnd::OnPaint() 
{

	CPaintDC dc(this); // device context for painting

	CRect rc(0,0,89,89);
	GetClientRect(rc);
	
//	CPenDC hi;
	
	CPen pen2;
	pen2.CreatePen(PS_SOLID, 1,RGB(255,255,255));
	CPen* pPen2 = dc.SelectObject(&pen2);

	dc.Rectangle(rc);

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(211, 211, 211));
	CPen* pPen = dc.SelectObject(&pen);

	HBRUSH hBr = (HBRUSH ) GetCurrentObject(dc, OBJ_BRUSH);

	int nX = 1;

	for (int nColorRow = 0; nColorRow < 8; nColorRow++)
	{
		int nY = 1;
		for (int nColorCol = 0; nColorCol < 8; nColorCol++)
		{
			rc.SetRect(nX, nY, nX + BTN_SIZE, nY + BTN_SIZE);
			
			CBrush br;

			br.CreateSolidBrush(ColorMap[nColorRow][nColorCol]);
			CBrush* pBrush = dc.SelectObject(&br);
			dc.Rectangle(rc);
			dc.SelectObject(pBrush);


			nY += BTN_SIZE + 1;
		}

		nX += BTN_SIZE + 1;
	}

	// Draw 'Selected' item
	if (m_nIndex != -1)
	{
		CBrush br;

		br.CreateSolidBrush(RGB(255,255,255));
	
		rc.SetRect(m_nIndex % 8*(BTN_SIZE+1) , m_nIndex / 8*(BTN_SIZE+1), (BTN_SIZE+1) + m_nIndex % 8*(BTN_SIZE+1)+1, (BTN_SIZE+1) + m_nIndex / 8*(BTN_SIZE+1)+1);	
		
	//	dc.FillSolidRect(rc,ColorMap[m_nIndex % 8][m_nIndex / 8]);
	
	
	//	dc.Draw3dRect(rc.left , rc.top , rc.Width()   , rc.Height()   ,g_PaintManager->GetColor(COLOR_HIGHLIGHT)  ,g_PaintManager->GetColor(COLOR_HIGHLIGHT));
		rc.DeflateRect(1,1);
		dc.Draw3dRect(rc.left , rc.top , rc.Width()   , rc.Height()   ,RGB(255,255,255)  ,RGB(255,255,255));

		rc.DeflateRect(1,1);
		dc.Draw3dRect(rc.left , rc.top , rc.Width()   , rc.Height()   ,RGB(211, 211, 211)  ,g_PaintManager->GetColor(COLOR_HIGHLIGHT));
		rc.DeflateRect(1,1);		
		//dc.FrameRect(m_rc, &br);

		// Give select a rounded feel
	//	dc.SetPixel(CPoint(m_rc.left+2, m_rc.top+2),RGB(255, 255, 255));
	//	dc.SetPixel(CPoint(m_rc.right-3, m_rc.bottom-3),RGB(255, 255, 255));
	//	dc.SetPixel(CPoint(m_rc.right-3, m_rc.top+2), RGB(255,255, 255));
	//	dc.SetPixel(CPoint(m_rc.left+2, m_rc.bottom-3) ,RGB(255, 255, 255));

	}


	dc.SelectObject(pPen);

	::SelectObject(dc, hBr);
	
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::OnLButtonDown
// 
// DESCRIPTION:		Process user selection
// 
// ACCESSIBILTY:	Protected
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
void CPaletteWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);

	m_bDrag = true;
	SetCapture();


	SelectColor(point);
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::OnLButtonUp
// 
// DESCRIPTION:		Process user selection
// 
// ACCESSIBILTY:	Protected
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
void CPaletteWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bDrag = false;
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);

	if (SelectColor(point))
	{
		COLOR_NOTIFY cn;

		cn.hdr.hwndFrom = GetSafeHwnd();
		cn.hdr.idFrom   = GetDlgCtrlID();
		cn.hdr.code     = COLOR_CHANGED;
		cn.color = ColorMap[m_nIndex % 8][m_nIndex / 8];

		GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM) &cn); 

	}
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::OnMouseMove
// 
// DESCRIPTION:		Process user selection
// 
// ACCESSIBILTY:	Protected
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
void CPaletteWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	// TODO: Add your message handler code here and/or call default
	if (m_bDrag)
		SelectColor(point);
}


////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::SelectColor
// 
// DESCRIPTION:		Select actual color
// 
// ACCESSIBILTY:	Protected
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
BOOL CPaletteWnd::SelectColor(CPoint& pt)
{
	CRect rc;
	GetClientRect(rc);

	if (!rc.PtInRect(pt))
		return FALSE;

	int nX = (pt.x / (BTN_SIZE+2));
	int nY = (pt.y / (BTN_SIZE+2));

	int nIndex = nX + (nY * 8);
	if (m_nIndex == nIndex)
		return TRUE;

	m_nIndex = -1;
	InvalidateRect(m_rc, TRUE);
	UpdateWindow();
	
	m_nIndex = nIndex;
	
	m_rc.SetRect((nX * (BTN_SIZE + 1) + 1), 
				 (nY * (BTN_SIZE + 1) + 1),
				 (nX * (BTN_SIZE + 1) + 1) + BTN_SIZE, 				 
				 (nY * (BTN_SIZE + 1) + 1) + BTN_SIZE);


	m_rc.InflateRect(3,3);
	InvalidateRect(m_rc, TRUE);

	UpdateWindow();
	
	return TRUE;

}


////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::SetCustomColor
// 
// DESCRIPTION:		Sets specified custom color
// 
// ACCESSIBILTY:	Public
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
void CPaletteWnd::SetCustomColor(UINT nIndex, COLORREF crCustom)
{
	ASSERT(nIndex > 16);
	if (nIndex > 16)
		return;

	ColorMap[(nIndex / 2)][5 + (nIndex / 8)] = crCustom;
}


////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::OnRButtonUp
// 
// DESCRIPTION:		Sets specified custom color
// 
// ACCESSIBILTY:	Protected
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
void CPaletteWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonUp(nFlags, point);

	if (SelectColor(point) && m_nIndex > 48)
	{
		CColorDialog cr(0, CC_ANYCOLOR|CC_FULLOPEN, this);

		TRACE("ColorMap\n");
		TRACE("========\n");

		COLORREF crCustColors[16];
		int nIndex = 0;
		for (int nCol=6; nCol < 8; nCol++)
		{
			for (int nRow=0; nRow < 8; nRow++)
			{
				
//				crCustColors[nCol == 6 ? nIndex : (nIndex+1) * 2] = ColorMap[nRow][nCol];			
				crCustColors[nIndex++] = ColorMap[nRow][nCol];			
				TRACE("Color %x\n",ColorMap[nRow][nCol]);
			}
			
		}

		cr.m_cc.lpCustColors = crCustColors;
		if (cr.DoModal() == IDOK)
		{
			ColorMap[m_nIndex % 8][m_nIndex / 8] = cr.GetColor();
			InvalidateRect(m_rc, TRUE);
			UpdateWindow();
		}
		
	}

}


////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::CreateToolTips
// 
// DESCRIPTION:		Creates tool tip hittest rectangles
// 
// ACCESSIBILTY:	Protected
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
void CPaletteWnd::CreateToolTips()
{
    if (!m_wndToolTip.Create(this)) 
		return;

	int nX = 1;

	for (int nColorRow = 0; nColorRow < 8; nColorRow++)
	{
		int nY = 1;
		for (int nColorCol = 0; nColorCol < 8; nColorCol++)
		{
			CRect rc;
			rc.SetRect(nX, nY, nX + BTN_SIZE, nY + BTN_SIZE);


			CString strColor;
			strColor.Format("RGB(%d,%d,%d)", GetRValue(ColorMap[nColorRow][nColorCol]),
											 GetGValue(ColorMap[nColorRow][nColorCol]),
											 GetBValue(ColorMap[nColorRow][nColorCol]));

			m_wndToolTip.AddTool(this, strColor, rc, 1);	

			nY += BTN_SIZE + 1;
		}

		nX += BTN_SIZE + 1;
	}
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::OnCreate
// 
// DESCRIPTION:		Called to create tooltips
// 
// ACCESSIBILTY:	Protected
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
int CPaletteWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateToolTips();	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::PreTranslateMessage
// 
// DESCRIPTION:		Relay message for tooltip window
// 
// ACCESSIBILTY:	Protected
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
BOOL CPaletteWnd::PreTranslateMessage(MSG* pMsg) 
{
    m_wndToolTip.RelayEvent(pMsg);
	
	return CWnd::PreTranslateMessage(pMsg);
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:		CPaletteWnd::SetCurrentColor
// 
// DESCRIPTION:		Sets current color
// 
// ACCESSIBILTY:	Public
// 
// MODIFICATIONS:
// Name                  Date       Version    Comments
// N T ALMOND            03MAR2001  1.0        Origin
// 
////////////////////////////////////////////////////////////////////////////////
void  CPaletteWnd::SetCurrentColor(COLORREF crCurrent)
{
	int nIndex = 0;
	for (int nColorRow = 0; nColorRow < 8; nColorRow++)
	{
		for (int nColorCol = 0; nColorCol < 8; nColorCol++)
		{
			if (ColorMap[nColorRow][nColorCol] == crCurrent)
			{
				m_nIndex = nIndex;
				m_rc.SetRect((nColorRow * (BTN_SIZE + 1) + 1), 
							 (nColorCol * (BTN_SIZE + 1) + 1),
							 (nColorRow * (BTN_SIZE + 1) + 1) + BTN_SIZE, 				 
							 (nColorCol * (BTN_SIZE + 1) + 1) + BTN_SIZE);


				RedrawWindow();
				break;
			}
			nIndex++;
		}
	}
}


void CPaletteWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CColorDialog Meh;
if(	Meh.DoModal()== IDOK)
{
//	Meh.GetColor();



	int nX = (point.x / (BTN_SIZE+2));
	int nY = (point.y / (BTN_SIZE+2));

	int nIndex = nX + (nY * 8);


ColorMap[nX][nY]=(Meh.GetColor());

Invalidate();


}

}
