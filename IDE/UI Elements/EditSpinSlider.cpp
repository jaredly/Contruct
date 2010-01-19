// EditSpinSlider.cpp : implementation file
//

#include "stdafx.h"

#include "EditSpinSlider.h"
#include "..\..\Utilities\Palette\ToolSettings.h"
#include "..\Editors\ImageEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditSpinSlider

CEditSpinSlider::CEditSpinSlider()
{
	m_Max = 255;
	m_Min = 0;
	m_Color = ::g_PaintManager->GetColor(COLOR_3DSHADOW);
	before = false;
}

CEditSpinSlider::~CEditSpinSlider()
{
}


BEGIN_MESSAGE_MAP(CEditSpinSlider, CEdit)
	//{{AFX_MSG_MAP(CEditSpinSlider)
	ON_WM_CREATE()
	ON_WM_NCPAINT()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_TIMER()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
//	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditSpinSlider message handlers

int CEditSpinSlider::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}



#define HLINE(p1,p2,y) p_Dc->FillSolidRect(CRect(p1,y,p2+1,y+1),selected? 0xffffff:g_PaintManager->GetColor(COLOR_3DSHADOW));
#define DRAWARROWUP(x,y) HLINE(2+x,3+x,0+y) HLINE(1+x,4+x,1+y)  HLINE(0+x,1+x,2+y)  HLINE(4+x,5+x,2+y)
#define DRAWARROWDOWN(x,y) HLINE(2+x,3+x,2+y) HLINE(1+x,4+x,1+y) HLINE(0+x,1+x,0+y) HLINE(4+x,5+x,0+y)



void CEditSpinSlider::OnNcPaint() 
{

	
	if (!before) { //If first time, the OnNcCalcSize function will be called
			SetWindowPos(NULL,0,0,64,32,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE); 
			before=true;
	}	
	
	
	
	// TODO: Add your message handler code here
//	CPaintDC dc(this); // device context for painting
	CDC* p_Dc = GetDC();
	bool selected = false;
	if(HIWORD(CEdit::GetMargins())!= 24)
		CEdit::SetMargins(0,0);

	CRect Client;
	GetClientRect(&Client);

	Client.InflateRect(1,1,23,4);
	p_Dc->Draw3dRect(Client,g_PaintManager->GetColor(COLOR_3DSHADOW),g_PaintManager->GetColor(COLOR_3DSHADOW));

	Client.DeflateRect(1,1,23,4);

	CRect BG(Client.right,Client.top,Client.right+22,Client.bottom);

	selected = (m_downOn==30);
	p_Dc->FillSolidRect(BG,0xffffff);
	CRect Up(Client.right,Client.top+1,Client.right+10,Client.top+(Client.Height()/2)-1);
	p_Dc->FillSolidRect(Up,selected?g_PaintManager->GetColor(COLOR_3DSHADOW):g_PaintManager->GetColor(COLOR_3DFACE));

	DRAWARROWUP(((Up.left+Up.right)/2-3),((Up.top+Up.bottom)/2-1))
//DRAWARROWUP(0,0)

	selected = (m_downOn==31);
	CRect Down = Up;
	Down.top = Up.bottom+1;
	Down.bottom = Client.bottom-1;
	
	p_Dc->FillSolidRect(Down,selected?g_PaintManager->GetColor(COLOR_3DSHADOW):g_PaintManager->GetColor(COLOR_3DFACE));
	DRAWARROWDOWN(((Down.left+Down.right)/2-3),((Down.top+Down.bottom)/2-1))

	CRect Right = Up;
	Right.bottom = Down.bottom;
	Right.left = Up.right+1;
	Right.right = Client.right+21;
	selected = (m_downOn==32);
	p_Dc->FillSolidRect(Right,selected?g_PaintManager->GetColor(COLOR_3DSHADOW):g_PaintManager->GetColor(COLOR_3DFACE));
	DRAWARROWDOWN(((Right.left+Right.right)/2-3),((Right.top+Right.bottom)/2-1))

	CRect Bottom = CRect(Client.left,Client.bottom,Client.right+22,Client.bottom + 3);
	p_Dc->FillSolidRect(Bottom,g_PaintManager->GetColor(COLOR_3DFACE));

	CRect Show = Bottom;
	CString Content;
	GetWindowText(Content);
	
	Show.right = Show.left + (Show.Width() * (atoi(Content)-m_Min)/(m_Max-m_Min+0.0));
	p_Dc->FillSolidRect(Show,m_Color);
	// Do not call CEdit::OnNcPaint() for painting messages
}



//DEL void CEditSpinSlider::OnNcLButtonUp(UINT nHitTest, CPoint point) 
//DEL {
//DEL 	m_downOn = 0;
//DEL 	ReleaseCapture();
//DEL 	OnNcPaint();
//DEL 	CEdit::OnNcMButtonUp(nHitTest, point);
//DEL }

//PORT: LRESULT return (was UINT)
LRESULT CEditSpinSlider::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT ret = CEdit::OnNcHitTest(point);	
	if(ret == HTNOWHERE)
	{
		ScreenToClient(&point);
		CRect Client;
		GetClientRect(&Client);


		CRect BG(Client.right,Client.top,Client.right+23,Client.bottom);

		CRect Up(Client.right,Client.top+1,Client.right+10,Client.top+(Client.Height()/2)-1);

		CRect Down = Up;
		Down.top = Up.bottom+1;
		Down.bottom = Client.bottom-1;

		CRect Right = Up;
		Right.bottom = Down.bottom;
		Right.left = Up.right+1;
		Right.right = Client.right+22;
			
		if(Up.PtInRect(point)) 
			return 30;
		if(Down.PtInRect(point))	
			return 31;	
		if(Right.PtInRect(point))
			return 32;	

	}
	return ret;

}

void CEditSpinSlider::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
 	// TODO: Add your message handler code here and/or call default
 	lpncsp->rgrc[0].right -= 23;
	lpncsp->rgrc[0].bottom -= 3; //Bottom
 
 	CEdit::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CEditSpinSlider::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		m_speed ++;
		short result=GetKeyState(VK_LBUTTON);
		if(result >= 0)
		{

			KillTimer(30);
			KillTimer(31);
			KillTimer(32);
			KillTimer(1);
			ReleaseCapture();
			m_downOn = 0;
			OnNcPaint();
			GetParent()->SendMessage(1234,100,(long)this->m_hWnd);

			//((CToolSettings*)(GetParent()->GetParent()))->m_pImgEd->SetFocus();

		}
	}

	if(nIDEvent == 30)
	{
		CString Content;

		GetWindowText(Content);
		Content.Format("%d",atoi(Content)+1);
		SetWindowText(Content);
		OnNcPaint();
		KillTimer(30);
		SetTimer(30,100-min(95,(m_speed/4)),0);
		GetParent()->SendMessage(1234,100,(long)this->m_hWnd);

		((CToolSettings*)(GetParent()->GetParent()))->m_pImgEd->SetFocus();
	}
	if(nIDEvent == 31)
	{
		CString Content;
		GetWindowText(Content);
		Content.Format("%d",atoi(Content)-1);
		SetWindowText(Content);
		OnNcPaint();
		KillTimer(31);
		SetTimer(31,100-min(95,(m_speed/4)),0);
		GetParent()->SendMessage(1234,100,(long)this->m_hWnd);

		((CToolSettings*)(GetParent()->GetParent()))->m_pImgEd->SetFocus();
	}
	if(nIDEvent == 32)
	{
		GetParent()->SendMessage(1234,100,(long)this->m_hWnd);

		((CToolSettings*)(GetParent()->GetParent()))->m_pImgEd->SetFocus();
	}
	CEdit::OnTimer(nIDEvent);
}
void CEditSpinSlider::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (nHitTest==30) //Up scroll Pressed
	{

		m_speed = 0;
		m_downOn = 30;	
		SetCapture();
		OnTimer(30);
		SetTimer(1,5,0);
		SetTimer(30,500,0);

	}
	if (nHitTest==31) //Down scroll Pressed
	{

		m_speed = 0;
		m_downOn = 31;
		SetCapture();
		OnTimer(31);
		SetTimer(1,5,0);
		SetTimer(31,500,0);

	}
	if (nHitTest==32)
	{
		m_speed = 0;
		SetTimer(1,5,0);
		m_downOn = 32;
		m_wndPopup.m_Parent = this;
		m_wndPopup.Create(0,0,0,CRect(0,0,0,0),this,0);
		m_wndPopup.SetCapture();



	}

	
	CEdit::OnNcLButtonDown(nHitTest, point);
}

void CEditSpinSlider::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (nHitTest==30) //Up scroll Pressed
	{
		m_speed = 0;
		SetTimer(30,200,0);
		SetTimer(1,5,0);
		m_downOn = 30;
		SetCapture();
		OnTimer(30);
	}
	if (nHitTest==31) //Down scroll Pressed
	{
		m_speed = 0;
		SetTimer(31,200,0);
		SetTimer(1,5,0);
		m_downOn = 31;
		SetCapture();
		OnTimer(31);
	}
	
	CEdit::OnNcLButtonDblClk(nHitTest, point);
}

void CEditSpinSlider::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CEdit::OnLButtonDown(nFlags, point);
}

void CEditSpinSlider::OnChange() 
{



		CString Content;

		GetWindowText(Content);
		int a = atoi(Content);
		int b = a;
		if(a < m_Min) b = m_Min;
		if(a > m_Max) b = m_Max; 
		CString Content2;
		Content2.Format("%d",b);
		if(Content.Compare(Content2)!= 0)
			SetWindowText(Content2);
		

		this->GetParent()->SendMessage(EN_CHANGE,100,(long)this->m_hWnd);


	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
/////////////////////////////////////////////////////////////////////////////
// CEditSpinSliderPopup

CEditSpinSliderPopup::CEditSpinSliderPopup()
{
	m_value = 0;
}

CEditSpinSliderPopup::~CEditSpinSliderPopup()
{
}


BEGIN_MESSAGE_MAP(CEditSpinSliderPopup, CWnd)
	//{{AFX_MSG_MAP(CEditSpinSliderPopup)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEditSpinSliderPopup message handlers

//DEL BOOL CEditSpinSlider::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
//DEL {
//DEL 	// TODO: Add your specialized code here and/or call the base class
//DEL 	
//DEL 	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
//DEL }

BOOL CEditSpinSliderPopup::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	CRect Client;
	pParentWnd->GetClientRect(&Client);
	pParentWnd->ClientToScreen(&Client);

	CRect rect2(0,0,100,100);
	rect2.top = Client.bottom + 8-3;
	rect2.left = Client.right+17-3;
	rect2.bottom = Client.bottom+8+12+3;
	rect2.right = Client.right+17+100+3;
	
	CString Content;
	pParentWnd->GetWindowText(Content);
	int Offset = atoi(Content);
	Offset = (Offset-m_Parent->m_Min) * 100/( m_Parent->m_Max-m_Parent->m_Min+0.0);
	rect2.OffsetRect(-Offset,0);
	// TODO: Add your specialized code here and/or call the base class

	return CWnd::CreateEx(0, AfxRegisterWndClass(CS_CLASSDC),0,WS_DLGFRAME  |WS_POPUP|WS_VISIBLE, rect2, pParentWnd->GetParent(), nID, pContext);
}

void CEditSpinSliderPopup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect Client;
	GetClientRect(&Client);


	CDC MyDC2;
	MyDC2.CreateCompatibleDC(this->GetDC());
	MyDC2.SelectObject(&m_Bg);

	dc.BitBlt(0,0,Client.Width(),Client.Height(),&MyDC2,0,0,SRCCOPY);
//	dc.FillSolidRect(Client,g_PaintManager->GetColor(COLOR_3DFACE));	
//	dc.DrawEdge(Client,BDR_SUNKENOUTER,BF_RECT);


	dc.DrawEdge(CRect(m_value-2,Client.top,m_value+2,Client.bottom),BDR_RAISEDOUTER,BF_RECT);
	dc.DrawEdge(CRect(m_value-1,Client.top+1,m_value+1,Client.bottom-1),BDR_RAISEDINNER,BF_RECT);

	//dc.FillSolidRect(0,0,100,100,RGB(255,0,0));
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}

int CEditSpinSliderPopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect Client;
	GetClientRect(&Client);

	m_Bg.DeleteObject();
	m_Bg.CreateCompatibleBitmap(this->GetDC(),Client.Width(),Client.Height());
	CDC MyDC2;
	MyDC2.CreateCompatibleDC(this->GetDC());
	CBitmap *pOld = MyDC2.SelectObject(&m_Bg);

	if(m_BGtype == 1)
	{
		for(int a = 0; a < 100; a ++)
		{
			COLORREF clr = RGB((int)(GetRValue(m_BGColorRight) * a/99.0 + GetRValue(m_BGColorLeft) * (99-a)/99.0),
								(int)(GetGValue(m_BGColorRight) * a/99.0 + GetGValue(m_BGColorLeft) * (99-a)/99.0),
								(int)(GetBValue(m_BGColorRight) * a/99.0 + GetBValue(m_BGColorLeft) * (99-a)/99.0));
			MyDC2.FillSolidRect(a,0,a+1,Client.Height(),clr);


		}
	}
	
	else
	{
	MyDC2.FillSolidRect(Client,g_PaintManager->GetColor(COLOR_3DFACE));	
	MyDC2.DrawEdge(Client,BDR_SUNKENOUTER,BF_RECT);	
	}
//	m_Bg. = MyDC2.SelectObject(pOld);

	// TODO: Add your specialized creation code here
	
	return 0;
}

void CEditSpinSliderPopup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	this->DestroyWindow();
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}

void CEditSpinSliderPopup::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_value = min(100,max(0,point.x));

	
	
int Value = m_Parent->m_Min+(m_value*(m_Parent->m_Max-m_Parent->m_Min))/100;



	
		CString Content;
		Content.Format("%d",Value);
		m_Parent->SetWindowText(Content);
		m_Parent->OnNcPaint();	
		m_Parent->Invalidate();
	
	
	
	
	
	
	
	
	
	
	
	
	Invalidate();









	CWnd::OnMouseMove(nFlags, point);
}

void CEditSpinSlider::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
/*	if(nChar == VK_LBUTTON)
	{
		if(m_downOn==30)
		{
			OnTimer(30);
		}
		if(m_downOn==31)
		{
			OnTimer(31);
		}


	}*/
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEditSpinSliderPopup::CreateGradientBG(COLORREF left, COLORREF right)
{

}

void CEditSpinSlider::CreateGradientBG(COLORREF left, COLORREF right)
{
	m_wndPopup.m_BGtype = 1;
	m_wndPopup.m_BGColorLeft = left;
	m_wndPopup.m_BGColorRight = right;
}

void CEditSpinSlider::Refresh()
{
	this->OnNcPaint();
}
