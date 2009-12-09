//// ParamTooltipCtrl.cpp: archivo de implementación
//

#include "stdafx.h"
#include ".\paramtooltipctrl.h"


// CParamTooltipCtrl

IMPLEMENT_DYNAMIC(CParamTooltipCtrl, CWnd)

BEGIN_MESSAGE_MAP(CParamTooltipCtrl, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


CParamTooltipCtrl::CParamTooltipCtrl()
	: m_cBackground(::g_PaintManager->GetColor(COLOR_INFOBK))
	, m_cText(::g_PaintManager->GetColor(COLOR_INFOTEXT))
{
}

CParamTooltipCtrl::CParamTooltipCtrl(COLORREF cBackground, COLORREF cText)
	: m_cBackground(cBackground)
	, m_cText(cText)
{
}

BOOL CParamTooltipCtrl::Create(CWnd* pParentWnd)
{
	CString sClass = AfxRegisterWndClass(CS_CLASSDC|CS_HREDRAW|CS_SAVEBITS|CS_VREDRAW, NULL, NULL, NULL);

	if( !CWnd::CreateEx( 0, sClass, "ParamTooltip", WS_POPUP|WS_BORDER, CRect(0,0,0,0), pParentWnd, 0) ) {
		TRACE("Failed to create ParamTooltip Window");
		return FALSE;
	}

	return TRUE;
}

void CParamTooltipCtrl::AddMethod(const CString& sMethod, const vector<CString>& setParams)
{
	m_setMethods.push_back(METHOD(sMethod, setParams));
	m_nCurParam = 0;
}

void CParamTooltipCtrl::FlushMethods()
{
	m_setMethods.resize(0);
}

void CParamTooltipCtrl::SetCurMethod(UINT nCurMethod)
{
	m_nCurMethod = nCurMethod;
}

void CParamTooltipCtrl::SetCurParam(UINT nCurParam)
{
	m_nCurParam = nCurParam;
}

void CParamTooltipCtrl::ShowTooltip(const CPoint& ptClient)
{
	m_ptOldClient = ptClient;
	CPoint ptScreen(ptClient);
	GetParent()->ClientToScreen(&ptScreen);

	CClientDC dc(this);
	CSize size = Draw(dc, false);

	SetWindowPos(NULL, ptScreen.x, ptScreen.y, size.cx, size.cy, SWP_SHOWWINDOW|SWP_NOACTIVATE);
}

void CParamTooltipCtrl::ShowPrevMethod()
{
	ShowWindow(SW_HIDE);
	m_nCurMethod--;
	if( m_nCurMethod < 0 )
		m_nCurMethod = (int)(m_setMethods.size())-1;
	m_nCurParam = 0;

	ShowTooltip(m_ptOldClient);
}

void CParamTooltipCtrl::ShowNextMethod()
{
	ShowWindow(SW_HIDE);

	m_nCurMethod++;
	if( m_nCurMethod == m_setMethods.size() )
		m_nCurMethod = 0;
	m_nCurParam = 0;

	ShowTooltip(m_ptOldClient);
}

void CParamTooltipCtrl::ShowPrevParam()
{
	if( 0 == m_nCurParam )
		return;

	ShowWindow(SW_HIDE);
	m_nCurParam--;
	ShowTooltip(m_ptOldClient);
}

void CParamTooltipCtrl::ShowNextParam()
{
	if( m_nCurParam == m_setMethods[m_nCurMethod].m_setParams.size()-1 )
		return;

	ShowWindow(SW_HIDE);
	m_nCurParam++;
	ShowTooltip(m_ptOldClient);
}

void CParamTooltipCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(&rect, m_cBackground);

	Draw(dc, true);
}

CSize CParamTooltipCtrl::Draw( CDC& dc, bool bDraw )
{
	CString& sMethod = m_setMethods[m_nCurMethod].m_sName;
	vector<CString>& setParams = m_setMethods[m_nCurMethod].m_setParams;

	CFont fontNormal;
	VERIFY(fontNormal.CreateFont(
		-12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial"));                 // lpszFacename

	CFont fontBold;
	VERIFY(fontBold.CreateFont(
		-12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,	               // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial"));                 // lpszFacename

	CFont* pOldFont = dc.SelectObject(&fontNormal);

	int cx = 0;

	if( m_setMethods.size() > 1 ) {
		// I we need arrows, first we create the bitmaps in memory. 
		// Because we won't use resources, we will create it in memory. 
		BYTE arrow[] = {
				127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000,
				127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000,
				127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000,
				127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 000, 000, 000, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000,
				127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000,
				127, 127, 127, 000, 127, 127, 127, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 127, 127, 127, 000, 127, 127, 127, 000,
				127, 127, 127, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 127, 127, 127, 000,
				127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000,
				127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000,
				127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000,
				127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000, 127, 127, 127, 000,
		};

		BITMAPINFO bi;
		memset(&bi, 0, sizeof(bi));
		bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
		bi.bmiHeader.biWidth = 9;
		bi.bmiHeader.biHeight = -11;
		bi.bmiHeader.biPlanes = 1;
		bi.bmiHeader.biBitCount = 32;
		bi.bmiHeader.biCompression = BI_RGB;

		cx = 1;
		if( bDraw ) ::SetDIBitsToDevice(dc, cx, 2, 9, 11, 0, 0,0, 11, arrow, &bi,DIB_RGB_COLORS);
		cx += 1 + 10;

		CString sString;
		sString.Format("%d of %d", 1 + m_nCurMethod, m_setMethods.size() );

		if( bDraw ) dc.TextOut(cx,0,sString); 
		cx += 1 + dc.GetTextExtent(sString).cx;

		bi.bmiHeader.biHeight = 11;
		if( bDraw ) ::SetDIBitsToDevice(dc, cx, 2, 9, 11, 0, 0,0, 11, arrow, &bi,DIB_RGB_COLORS);

		cx += 1 + 10;
	}

	if( bDraw ) dc.TextOut(cx,0, sMethod); 
	cx += 1 + dc.GetTextExtent(sMethod).cx;
	
	if( bDraw )dc.TextOut(cx,0, "("); 
	cx += 1 + dc.GetTextExtent(CString("(")).cx;

	for( size_t i=0; i<setParams.size(); i++ ) {
		if( i != m_nCurParam ) {
			if( bDraw ) dc.TextOut(cx,0,setParams[i]); 
			cx += 1 + dc.GetTextExtent(setParams[i]).cx;
		} else {
			dc.SelectObject(&fontBold);
			if( bDraw) dc.TextOut(cx,0,setParams[i]); 
			cx += 1 + dc.GetTextExtent(setParams[i]).cx;
			dc.SelectObject(&fontNormal);
		}

		if( i < setParams.size()-1 ) {
			if( bDraw ) dc.TextOut(cx,0, CString(", ")); 
			cx += 1 + dc.GetTextExtent(CString(", ")).cx;
		}
	}

	if( bDraw ) dc.TextOut(cx,0, CString(")")); 
	cx += 1 + dc.GetTextExtent(CString(")")).cx;

	return CSize(2 + cx, 2 + dc.GetTextExtent(CString(")")).cy);
}
