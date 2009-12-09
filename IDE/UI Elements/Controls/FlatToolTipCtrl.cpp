// FlatToolTipCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "FlatToolTipCtrl.h"

// CToolTipCtrlEx

IMPLEMENT_DYNAMIC(CToolTipCtrlEx, CToolTipCtrl)
CToolTipCtrlEx::CToolTipCtrlEx()
{
	m_arrowColor=RGB(55, 10, 53);
	m_bkColor=RGB(255,255,255);
	m_leftColor=RGB(255, 210, 83);
	m_frameColor=RGB(155, 110, 53);
	m_textColor=RGB(0,0,0);
}

CToolTipCtrlEx::~CToolTipCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CToolTipCtrlEx, CToolTipCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()



// CToolTipCtrlEx message handlers


void CToolTipCtrlEx::PreSubclassWindow()
{
 //   ModifyStyle (WS_BORDER,0, 0);
	CWnd::PreSubclassWindow();
}

void CToolTipCtrlEx::OnPaint()
{
	POINT Pos;
	GetCursorPos(&Pos);
	CRect PRect;
	WindowFromPoint(Pos)->GetWindowRect(PRect);

	CPaintDC dc(this); // device context for painting
	CRect	Rect,R1;
	CRect	WRect;

	Orientations Orient=NW;
	BOOL	Over=0,Left=0;
	Over=Pos.y	>(PRect.top+(PRect.bottom-PRect.top)/2);
	Left=Pos.x	<(PRect.left+(PRect.right-PRect.left)/2);
	if(Over & Left)Orient=NW;
	else
	if(Over & !Left)Orient=NE;
	else
	if(!Over & Left)Orient=SW;
	else
	if(!Over & !Left)Orient=SE;

	dc.SelectObject(GetFont());

	CString	Tip=_T(""),TStr=_T("");
	GetWindowText(Tip);
	UINT Width=0;
	UINT Rows=1;
	UINT iPos=0;
	Tip.Replace(_T("\r"),_T(""));
	while(iPos<Tip.GetLength())
	{
		if(Tip.GetAt(iPos)=='\n')
		{
			CSize Sz1=dc.GetTextExtent(TStr);
			Width=(Width > Sz1.cx) ? Width : Sz1.cx;
			Rows+=1;
			TStr=_T("");
		}
		else
		{
			TStr+=Tip.GetAt(iPos);
		}
		iPos++;
	}
	if(TStr.GetLength())
	{
		CSize Sz1=dc.GetTextExtent(TStr);
		Width=(Width > Sz1.cx) ? Width : Sz1.cx;
	}
	if(Rows==1)
	{
		CSize Sz1=dc.GetTextExtent(Tip);
		Width=(Width > Sz1.cx) ? Width : Sz1.cx;
	}
	Width+=2;

	GetWindowRect(WRect);
	TEXTMETRIC TM;
	dc.GetTextMetrics(&TM);
	if(Over)
	{
		WRect.bottom=PRect.top;
		WRect.top=WRect.bottom-(Rows*(TM.tmHeight)+4);
	}
	else
	{
		WRect.top=PRect.bottom;
		WRect.bottom=WRect.top+(Rows*(TM.tmHeight)+4);
	}
	UINT T=WRect.Width()-(20+Width);
	WRect.left=Pos.x;
	WRect.right=WRect.left+20+Width;
	if(WRect.right>GetSystemMetrics(SM_CXSCREEN)-25)
	{
		WRect.OffsetRect(-(WRect.right-(GetSystemMetrics(SM_CXSCREEN)-25)),0);
	}
	MoveWindow(&WRect,1);
	ShowWindow(1);

	GetClientRect(Rect);
	dc.FillSolidRect(Rect,m_bkColor);
	R1=Rect;
	R1.right=R1.left+15;
	dc.FillSolidRect(R1,m_leftColor);
	Rect.left=R1.right+1;

	dc.SetBkMode(TRANSPARENT);
	Rect.top+=1;
	UINT iT=(Rect.Width()-Width)/2;
	Rect.left+=iT;
	dc.SetTextColor(m_textColor);
	if(Rows==1)
	{
		dc.DrawText(Tip,Rect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}
	else
	{
		dc.DrawText(Tip,Rect,DT_TOP|DT_LEFT|DT_WORDBREAK);
	}
	Rect.top-=1;
	Rect.left-=iT;
	GetClientRect(Rect);
//	Rect.InflateRect(1,1,1,1);
	dc.Draw3dRect(Rect,m_frameColor,m_frameColor);

	CPen	Pen,*OldPen=0;
	Pen.CreatePen(PS_SOLID,1,m_arrowColor);
	OldPen=dc.SelectObject(&Pen);
	switch(Orient)
	{
	case SE:
		{
			CPoint	ArrowPt=CPoint(R1.left+2,R1.top+2);
			POINT	Pt[18]=
			{
				{ArrowPt.x,     ArrowPt.y},
				{ArrowPt.x,		ArrowPt.y + 6},
				{ArrowPt.x + 1, ArrowPt.y + 6}, 
				{ArrowPt.x + 1, ArrowPt.y},
				{ArrowPt.x + 6, ArrowPt.y},
				{ArrowPt.x + 6, ArrowPt.y + 1},
				{ArrowPt.x + 2, ArrowPt.y + 1},
				{ArrowPt.x + 2, ArrowPt.y + 4},
				{ArrowPt.x + 5, ArrowPt.y + 7},
				{ArrowPt.x + 6, ArrowPt.y + 7},
				{ArrowPt.x + 3, ArrowPt.y + 4},
				{ArrowPt.x + 3, ArrowPt.y + 3},
				{ArrowPt.x + 6, ArrowPt.y + 6},
				{ArrowPt.x + 7, ArrowPt.y + 6},
				{ArrowPt.x + 3, ArrowPt.y + 2},
				{ArrowPt.x + 4, ArrowPt.y + 2},
				{ArrowPt.x + 7, ArrowPt.y + 5},
				{ArrowPt.x + 7, ArrowPt.y + 6}
			};
			dc.Polyline(Pt,18);
		}
		break;
	case SW:
		{
			CPoint	ArrowPt=CPoint(R1.right - 3, R1.top + 2);
			POINT	Pt[18]=
			{
				{ArrowPt.x,     ArrowPt.y},     {ArrowPt.x, ArrowPt.y + 6},
                {ArrowPt.x - 1, ArrowPt.y + 6}, {ArrowPt.x - 1, ArrowPt.y},
                {ArrowPt.x - 6, ArrowPt.y},     {ArrowPt.x - 6, ArrowPt.y + 1},
                {ArrowPt.x - 2, ArrowPt.y + 1}, {ArrowPt.x - 2, ArrowPt.y + 4},
                {ArrowPt.x - 5, ArrowPt.y + 7}, {ArrowPt.x - 6, ArrowPt.y + 7},
                {ArrowPt.x - 3, ArrowPt.y + 4}, {ArrowPt.x - 3, ArrowPt.y + 3},
                {ArrowPt.x - 6, ArrowPt.y + 6}, {ArrowPt.x - 7, ArrowPt.y + 6},
                {ArrowPt.x - 3, ArrowPt.y + 2}, {ArrowPt.x - 4, ArrowPt.y + 2},
                {ArrowPt.x - 7, ArrowPt.y + 5}, {ArrowPt.x - 7, ArrowPt.y + 6}
			};
			dc.Polyline(Pt,18);
		}
		break;
	case NE:
		{
			CPoint	ArrowPt=CPoint(R1.left+ 3, R1.bottom - 3);
			POINT	Pt[18]=
			{
				{ArrowPt.x,     ArrowPt.y},     {ArrowPt.x, ArrowPt.y - 6},
				{ArrowPt.x + 1, ArrowPt.y - 6}, {ArrowPt.x + 1, ArrowPt.y},
				{ArrowPt.x + 6, ArrowPt.y},     {ArrowPt.x + 6, ArrowPt.y - 1},
				{ArrowPt.x + 2, ArrowPt.y - 1}, {ArrowPt.x + 2, ArrowPt.y - 4},
				{ArrowPt.x + 5, ArrowPt.y - 7}, {ArrowPt.x + 6, ArrowPt.y - 7},
				{ArrowPt.x + 3, ArrowPt.y - 4}, {ArrowPt.x + 3, ArrowPt.y - 3},
				{ArrowPt.x + 6, ArrowPt.y - 6}, {ArrowPt.x + 7, ArrowPt.y - 6},
				{ArrowPt.x + 3, ArrowPt.y - 2}, {ArrowPt.x + 4, ArrowPt.y - 2},
				{ArrowPt.x + 7, ArrowPt.y - 5}, {ArrowPt.x + 7, ArrowPt.y - 6}
			};
			dc.Polyline(Pt,18);
		}
		break;
	case NW:
		{
			CPoint	ArrowPt=CPoint(R1.right- 3, R1.bottom - 3);
			POINT	Pt[18]=
			{
				{ArrowPt.x,     ArrowPt.y},     {ArrowPt.x, ArrowPt.y - 6},
				{ArrowPt.x - 1, ArrowPt.y - 6}, {ArrowPt.x - 1, ArrowPt.y},
				{ArrowPt.x - 6, ArrowPt.y},     {ArrowPt.x - 6, ArrowPt.y - 1},
				{ArrowPt.x - 2, ArrowPt.y - 1}, {ArrowPt.x - 2, ArrowPt.y - 4},
				{ArrowPt.x - 5, ArrowPt.y - 7}, {ArrowPt.x - 6, ArrowPt.y - 7},
				{ArrowPt.x - 3, ArrowPt.y - 4}, {ArrowPt.x - 3, ArrowPt.y - 3},
				{ArrowPt.x - 6, ArrowPt.y - 6}, {ArrowPt.x - 7, ArrowPt.y - 6},
				{ArrowPt.x - 3, ArrowPt.y - 2}, {ArrowPt.x - 4, ArrowPt.y - 2},
				{ArrowPt.x - 7, ArrowPt.y - 5}, {ArrowPt.x - 7, ArrowPt.y - 6}
			};
			dc.Polyline(Pt,18);
		}
		break;
	};
	dc.SelectObject(OldPen);
	Pen.DeleteObject();
}

BOOL CToolTipCtrlEx::OnEraseBkgnd(CDC* pDC)
{
	return CToolTipCtrl::OnEraseBkgnd(pDC);
}

void CToolTipCtrlEx::OnNcPaint()
{
//return CToolTipCtrl::OnNCPaint();
}

LRESULT CToolTipCtrlEx::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
	case WM_PRINT:	// eat this message, tooltips under XP / 2000 use WM_PRINT + WM_PRINTCLIENT to draw
		return 0;	// if we eat this message, the WM_PRINTCLIENT will be skipped and WM_PAINT will be used
		break;		// which is what we expect with our overloaded CToolTipCtrlEx - SLC
	case WM_PRINTCLIENT:
		return 0;	// we should never get this... but just in case - SLC
		break;
	default:
		//TRACE( _T("OTHER :0x%x\r\n"), message );
		break;
	}
	return CToolTipCtrl::WindowProc( message, wParam, lParam );
}