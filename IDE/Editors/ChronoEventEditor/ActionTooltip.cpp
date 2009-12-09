// ActionTooltip.cpp : implementation file
//

#include "stdafx.h"

#include "ActionTooltip.h"
#include "..\Event Sheet Editor\Event Sheet Editor.h"
#include "..\..\UI Elements\HTML\HTMLFont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHTMLDrawer g_HTMLDraw;

extern CFontMgr gMyFontManager;


#define TIPWIDTH 300


#define TIMER_SHOW		100
#define TIMER_FADEIN	101
#define TIMER_FADEOUT	102
#define TIMER_HIDE		103

#define WS_EX_LAYERED           0x00080000
#define ULW_ALPHA               0x00000002

/////////////////////////////////////////////////////////////////////////////
// CActionTooltip

CActionTooltip::CActionTooltip()
{
	m_created = false;
	m_redraw = false;
	m_fade = 0;


		
		CRect WindowRect = CRect(0,0,TIPWIDTH,30);
		CSize size = CSize(WindowRect.Width(),WindowRect.Height());
		CPoint	ptSrc(0, 0);

		HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
		m_pUpdateLayeredWindow = (lpfnUpdateLayeredWindow)GetProcAddress(hUser32, "UpdateLayeredWindow");
		m_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, "SetLayeredWindowAttributes");



		const char *m_lpszClassName;
		m_lpszClassName = AfxRegisterWndClass( CS_HREDRAW|CS_VREDRAW ,
			NULL,CreateSolidBrush(RGB(0,0,0)) ,NULL);

		this->CreateEx(WS_EX_LAYERED|WS_EX_TOOLWINDOW,m_lpszClassName,NULL,WS_POPUP,WindowRect, NULL, NULL, NULL);


		m_pUpdateLayeredWindow(GetSafeHwnd(), NULL, NULL, &size, ::GetDC(GetSafeHwnd()),
														&ptSrc, 0, &m_Blend, ULW_ALPHA);
		
		m_pSetLayeredWindowAttributes(m_hWnd,RGB(0,0,0),0,2);
		
		m_pt.x = 0;
		m_pt.y = 0;
		m_created = true;












}

CActionTooltip::~CActionTooltip()
{
	int x;
	for(x = 0; x < m_edit_buttons.size(); x ++)
		delete m_edit_buttons[x];
	for(x = 0; x < m_delete_buttons.size(); x ++)
		delete m_delete_buttons[x];
}


BEGIN_MESSAGE_MAP(CActionTooltip, CWnd)
	//{{AFX_MSG_MAP(CActionTooltip)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CActionTooltip_EditButton, CButton)
	//{{AFX_MSG_MAP(CHTMLStatic)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP(CActionTooltip_DeleteButton, CButton)
	//{{AFX_MSG_MAP(CHTMLStatic)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionTooltip message handlers

void CActionTooltip::ShowAt(CPoint pt,SelectedActionVector& actions, CWnd& Parent)
{
	m_pChrono = (EventSheetEditor*)&Parent;

	if(m_created)
	{
		Parent.ClientToScreen(&pt);
		if((pt != m_pt) || !this->IsWindowVisible())
		{
			int x;
			for(x = 0; x < m_edit_buttons.size(); x ++)
				delete m_edit_buttons[x];
			for(x = 0; x < m_delete_buttons.size(); x ++)
				delete m_delete_buttons[x];
			while(m_edit_buttons.size() > 0)
				m_edit_buttons.pop_back();
			while(m_delete_buttons.size() > 0)
				m_delete_buttons.pop_back();

			const int ButtonSize = 13;	
		
			ShowWindow(false);





			m_pt = pt;	
			m_actions = actions;
			OnPaint();
			// Create all the buttons

			int y = 0;
			
			for(x = 0; x < m_actionHeights.size(); x ++)
			{

			CRect clientRect;
			GetClientRect(clientRect);
		
			if((m_actions.at(x))->params.size() > 0)
			{
				CActionTooltip_EditButton* edit = new CActionTooltip_EditButton;
				edit->Create(3,y + 3,ButtonSize,ButtonSize,this,m_actions.at(x));
				m_edit_buttons.push_back(edit);
			}
		//	CActionTooltip_DeleteButton* del = new CActionTooltip_DeleteButton;
		//	del->Create(ButtonSize-3,y + 3,ButtonSize,ButtonSize, this,m_actions.at(x));

			y += m_actionHeights.at(x)+1;
			
		//	m_delete_buttons.push_back(del);

			}


			Invalidate();
			m_redraw = true;
			SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
			m_pSetLayeredWindowAttributes(m_hWnd,0,1,2);
			SetTimer(TIMER_SHOW,0,0);
			KillTimer(TIMER_HIDE);
		}
	}

	


}

void CActionTooltip::Hide()
{
	if(m_created)
		SetTimer(TIMER_HIDE,0,0);
}

void CActionTooltip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect ClientRect;
	this->GetClientRect(&ClientRect);
	
	//CEditorAction** action;
	CRect Pos(1+17,1,299,0);
	m_actionHeights.clear();

	//PORT: What the hell are you trying to do here?  CEditorAction** is not an iterator and VS2005
	// isnt falling for your dirty tricks!
	// Using iterator definition

	// Davo: Same dog different leg action
	SelectedActionVector::iterator action;

	for(action = m_actions.begin(); action != m_actions.end(); action ++)
	{
	/*	COLORREF ActionBG = (*action)->m_select? ACTION_BG_SEL : ACTION_BG_EVSEL;

		CHTMLFont f;
		f.SetBold(false);
		f.SetSize(8);
		f.SetName("Arial");
		f.SetImage(false);

		CRect Calc = Pos;
		g_HTMLDraw.DrawText(&dc, (*action)->m_Text, f, Calc, 0, NULL);
		Pos.bottom = Pos.top + Calc.Height()+1;
		Calc = Pos;
		dc.FillSolidRect(Pos,ActionBG);
		{
				g_HTMLDraw.DrawText(&dc, (*action)->m_Text, f, Calc, 0, NULL);
		}
		Pos.OffsetRect(0,Pos.Height());*/
	COLORREF ActionBG	=(*action)->m_select(m_pChrono)? ACTION_BG_SEL :  ACTION_BG;
		CHTMLFont f;
			f.SetBold(false);
			f.SetSize(8);
			f.SetName("Arial");
			f.SetImage(false);

			CRect Calc = Pos;

			CString Out = (*action)->m_Text;
			g_HTMLDraw.DrawText(&dc, Out, f, Calc, 0, m_pChrono, &(*action)->m_imgs, &(*action)->params);
			(*action)->m_Text = Out;

			Pos.bottom = Pos.top + Calc.Height();
			Calc = Pos;
			CRect temp = Pos;
			temp.left -= 17;
			dc.FillSolidRect(temp,ActionBG);
			if((*action)->m_select(m_pChrono))
				dc.Draw3dRect(temp,ACTION_BG_SEL2,ACTION_BG_SEL2);
			
			g_HTMLDraw.DrawText(&dc, (*action)->m_Text, f, Calc, 0, m_pChrono,&(*action)->m_imgs, &(*action)->params);


		Pos.OffsetRect(0,Pos.Height()+1);

		m_actionHeights.push_back(Pos.Height());

		dc.FillSolidRect(Pos.left-17,Pos.top-1,Pos.Width()+17,1,RGB(254,240,197));
	}
	ClientRect.bottom = Pos.top;
	dc.Draw3dRect(ClientRect,0,0);
	CRect WindowRect;
	this->GetWindowRect(WindowRect);
	WindowRect.OffsetRect(CPoint(0,0)-WindowRect.TopLeft());
	WindowRect.OffsetRect(m_pt);
	if(	WindowRect.Height() != Pos.top || m_redraw)
	{
		m_redraw = false;
		WindowRect.bottom = WindowRect.top + Pos.top;
		//this->MoveWindow(WindowRect,true);
		SetWindowPos(&wndTop, WindowRect.left,WindowRect.top,WindowRect.Width(),WindowRect.Height(), SWP_NOACTIVATE);
	}
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}

void CActionTooltip::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TIMER_SHOW:
		KillTimer(TIMER_SHOW);
		KillTimer(TIMER_FADEOUT);
		SetTimer(TIMER_FADEIN,10,0);
		break;

	case TIMER_HIDE:
		KillTimer(TIMER_HIDE);
		KillTimer(TIMER_FADEIN);
		SetTimer(TIMER_FADEOUT,10,0);

		break;
	case TIMER_FADEIN:
		{
			m_fade += 20;
			if(m_fade > 255)
			{
				m_fade = 255;
				KillTimer(TIMER_FADEIN);
			}
			if(m_created)
				m_pSetLayeredWindowAttributes(m_hWnd,RGB(0,0,0),m_fade,2);
		}
		break;
	case TIMER_FADEOUT:
		{
			m_fade -= 20;
			if(m_fade <0)
			{
				m_fade = 0;
				m_pt.x = 0;
				m_pt.y = 0;
				ShowWindow(false);
				KillTimer(TIMER_FADEOUT);
			}
			if(m_created)
				m_pSetLayeredWindowAttributes(m_hWnd,RGB(0,0,0),m_fade,2);
			
		
		}
		break;


	}
	CWnd::OnTimer(nIDEvent);
}

CEditorAction* CActionTooltip::ActionAt(int y)
{
	int ly =0 ;
	for(int a = 0; a < m_actionHeights.size(); a ++)
	{
		ly += m_actionHeights.at(a);
		if(ly > y)
			return m_actions.at(a);
		
	}

	return 0;
}



void CActionTooltip::OnLButtonDown(UINT nFlags, CPoint point) 
{

	/////////////////////
	// Select Action
				
	CEditorAction* pMyAction = ActionAt(point.y);

	if(pMyAction)
	{
		//Deselect selected events or conditions
		m_pChrono->DeselectEvents();
		m_pChrono->DeselectConditions();

		// Deselect any selected conditions

					
		if(pMyAction->m_select(m_pChrono)) // if we clicked on something already selected...wait till release
			m_pChrono->m_ClickedSelectedAction = true;
		else
		{
			if(!m_pChrono->m_CtrlDown)
			{
				m_pChrono->DeselectActions();
			}
			pMyAction->m_select(m_pChrono) = true;
			m_pChrono->InvalidateRect(pMyAction->m_rect(m_pChrono)); // update chrono
			Invalidate();
		}
		m_pChrono->m_FocusAction = pMyAction;
		// We may be wanting to drag these conditions...
		m_pChrono->m_Drag.dragtype = ACTION;
		m_pChrono->m_Drag.StoredPos = point;
		m_pChrono->m_Drag.IsDragging = false;
	}	

	CWnd::OnLButtonDown(nFlags, point);
}

void CActionTooltip::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CActionTooltip::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// as the mouse has faded over it we should hide it
	if(m_fade < 255)
	{
		KillTimer(TIMER_SHOW);
		KillTimer(TIMER_FADEOUT);

		m_fade = 255;
		if(m_created)
			m_pSetLayeredWindowAttributes(m_hWnd,RGB(0,0,0),m_fade,2);

	}
	CWnd::OnMouseMove(nFlags, point);
}

void CActionTooltip_EditButton::OnClicked()
{
	CActionTooltip* p_actionTooltip = (CActionTooltip*)GetParent();
	p_actionTooltip->Hide();
	
	((EventSheetEditor*)p_actionTooltip->m_pChrono)->EditAction(pAction);
}

void CActionTooltip_DeleteButton::OnClicked()
{
	CActionTooltip* p_actionTooltip = (CActionTooltip*)GetParent();
	p_actionTooltip->Hide();
	pAction->m_Anim.Start(    p_actionTooltip->m_pChrono->m_Ellapsed );
	
}





















































/////////////////////////////////////////////////////////////////////////////
// CEditorHeaderTooltip

BOOL CEditorHeaderTooltip::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return ((CChronoEventEditor*)GetParent())->OnCommand(wParam, lParam);


}


CEditorHeaderTooltip::CEditorHeaderTooltip()
{
	m_object = 0;
	m_pChrono = 0;

	m_created = false;
	m_redraw = false;
	m_fade = 0;

		
		CRect WindowRect = CRect(0,0,TIPWIDTH,34);
		CSize size = CSize(WindowRect.Width(),WindowRect.Height());
		CPoint	ptSrc(0, 0);

		HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
		m_pUpdateLayeredWindow = (lpfnUpdateLayeredWindow)GetProcAddress(hUser32, "UpdateLayeredWindow");
		m_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, "SetLayeredWindowAttributes");



		const char *m_lpszClassName;
		m_lpszClassName = AfxRegisterWndClass( CS_HREDRAW|CS_VREDRAW ,
			NULL,CreateSolidBrush(RGB(0,0,0)) ,NULL);

		this->CreateEx(WS_EX_LAYERED|WS_EX_TOOLWINDOW,m_lpszClassName,NULL,WS_POPUP,WindowRect, NULL, NULL, NULL);


		m_pUpdateLayeredWindow(GetSafeHwnd(), NULL, NULL, &size, ::GetDC(GetSafeHwnd()),
														&ptSrc, 0, &m_Blend, ULW_ALPHA);
		
		m_pSetLayeredWindowAttributes(m_hWnd,RGB(0,0,0),0,2);
		
		m_pt.x = 0;
		m_pt.y = 0;
		m_created = true;












}

CEditorHeaderTooltip::~CEditorHeaderTooltip()
{

}


BEGIN_MESSAGE_MAP(CEditorHeaderTooltip, CWnd)
	//{{AFX_MSG_MAP(CActionTooltip)
	ON_WM_PAINT()
	ON_WM_TIMER()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CActionTooltip message handlers

void CEditorHeaderTooltip::ShowAt(CPoint pt,CWnd& Parent)
{

	m_pChrono = (CChronoEventEditor*) &Parent;
	if(m_created)
	{
		

		//work out what object we are over

		CEditorObject* object = m_pChrono->m_pHeader->GetObjectAtX(pt.x);


		if(object && m_pChrono->m_pHeader->m_rect.PtInRect(pt) && !m_pChrono->m_pHeader->m_isDrag)
		{


			if(object != m_object || !this->IsWindowVisible())
			{
				m_object = object;
	
			
				ShowWindow(false);

				m_pt = object->m_rect.TopLeft() + CPoint(0,32);

				Parent.ClientToScreen(&m_pt);
					
	
	
				OnPaint();	//calculates size
				Invalidate();	//makes sure it's right :P

				m_redraw = true;
				SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
				m_pSetLayeredWindowAttributes(m_hWnd,0,1,2);
				SetTimer(TIMER_SHOW,0,0);
				KillTimer(TIMER_HIDE);
			}
		}
		else
			Hide();
	}

	


}

void CEditorHeaderTooltip::Hide()
{
	if(m_created)
		SetTimer(TIMER_HIDE,0,0);
}

void CEditorHeaderTooltip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect ClientRect;
	this->GetClientRect(&ClientRect);

	dc.Draw3dRect(ClientRect,0,0);
	ClientRect.DeflateRect(1,1);
	dc.FillSolidRect(ClientRect, GetSysColor(COLOR_INFOBK));
	
	if(m_object)
	{
		CObjType* ot = 0;

		long oid = m_object->oid;

		ot = ((EventSheetEditor*)m_pChrono)->application->FindObjTypeFromNumber(oid);

		if(ot)
		{

			HDC hdcScreenCompat = CreateCompatibleDC(dc.m_hDC);
			HBITMAP obj = ot->m_Image.MakeBitmap();

			HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcScreenCompat, obj);
			BitBlt(dc.m_hDC, 1, 1, 32, 32, hdcScreenCompat, 0, 0, SRCCOPY);
			SelectObject(hdcScreenCompat, hbmpOld);
			DeleteDC(hdcScreenCompat);

			ClientRect.left += 38;
			ClientRect.top += 10;
			dc.SetBkMode(1);
			dc.SelectObject(m_pChrono->MyFont);
			dc.SetTextColor(GetSysColor(COLOR_INFOTEXT));
			dc.DrawText(ot->GetName(), ClientRect, DT_WORDBREAK);

			DeleteObject(obj);
			DeleteObject(hdcScreenCompat);
		}

		else		//system object probably
		{
		/*	HDC hdcScreenCompat = CreateCompatibleDC(dc.m_hDC);
			HBITMAP systemBitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SYSTEM));


			HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcScreenCompat, systemBitmap);
				BitBlt(dc.m_hDC, 1, 1, 32, 32, hdcScreenCompat, 0, 0, SRCCOPY);
				SelectObject(hdcScreenCompat, hbmpOld);
				DeleteDC(hdcScreenCompat);

				ClientRect.left += 38;
				ClientRect.top += 10;
				dc.SetBkMode(1);
				dc.SelectObject(m_pChrono->MyFont);
				dc.SetTextColor(GetSysColor(COLOR_INFOTEXT));
				dc.DrawText("System", ClientRect, DT_WORDBREAK);

				*/





			//DeleteObject(systemBitmap);
			//DeleteObject(hdcScreenCompat);


		}
	}

	CRect WindowRect;
	this->GetWindowRect(WindowRect);
	WindowRect.OffsetRect(CPoint(0,0)-WindowRect.TopLeft());
	WindowRect.OffsetRect(m_pt);
	if(m_redraw)
	{
		m_redraw = false;
		SetWindowPos(&wndTop, WindowRect.left,WindowRect.top,WindowRect.Width(),WindowRect.Height(), SWP_NOACTIVATE);
	}
}

void CEditorHeaderTooltip::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TIMER_SHOW:
		KillTimer(TIMER_SHOW);
		KillTimer(TIMER_FADEOUT);
		SetTimer(TIMER_FADEIN,10,0);
		break;

	case TIMER_HIDE:
		KillTimer(TIMER_HIDE);
		KillTimer(TIMER_FADEIN);
		SetTimer(TIMER_FADEOUT,10,0);

		break;
	case TIMER_FADEIN:
		{
			m_fade += 20;
			if(m_fade > 255)
			{
				m_fade = 255;
				KillTimer(TIMER_FADEIN);
			}
			if(m_created)
				m_pSetLayeredWindowAttributes(m_hWnd,RGB(0,0,0),m_fade,2);
		}
		break;
	case TIMER_FADEOUT:
		{
			m_fade -= 20;
			if(m_fade <0)
			{
				m_fade = 0;
				m_pt.x = 0;
				m_pt.y = 0;
				ShowWindow(false);
				KillTimer(TIMER_FADEOUT);
			}
			if(m_created)
				m_pSetLayeredWindowAttributes(m_hWnd,RGB(0,0,0),m_fade,2);
			
		
		}
		break;


	}
	CWnd::OnTimer(nIDEvent);
}





















BEGIN_MESSAGE_MAP(CHoverButton, CWnd)
	//{{AFX_MSG_MAP(CActionTooltip)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CHoverButton::CHoverButton()
{
	CRect WindowRect = CRect(0,0,100,19);
	CSize size = CSize(WindowRect.Width(),WindowRect.Height());
	CPoint	ptSrc(0, 0);

	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	m_pUpdateLayeredWindow = (lpfnUpdateLayeredWindow)GetProcAddress(hUser32, "UpdateLayeredWindow");
	m_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, "SetLayeredWindowAttributes");



	const char *m_lpszClassName;
	m_lpszClassName = AfxRegisterWndClass( CS_HREDRAW|CS_VREDRAW ,
			NULL,CreateSolidBrush(RGB(0,0,0)) ,NULL);

	this->CreateEx(WS_EX_LAYERED|WS_EX_TOOLWINDOW,m_lpszClassName,NULL,WS_POPUP,WindowRect, NULL, NULL, NULL);


	m_pUpdateLayeredWindow(GetSafeHwnd(), NULL, NULL, &size, ::GetDC(GetSafeHwnd()),
														&ptSrc, 0, &m_Blend, ULW_ALPHA);
		
	m_pSetLayeredWindowAttributes(m_hWnd,RGB(0,0,0),0,2);

	m_IsSub = false;

}

void CHoverButton::ShowAt(CPoint pt,CEditorAction* pAction, CWnd& Parent)
{
	GetCursorPos(&m_old);

	Parent.ClientToScreen(&pt);
	m_pt = pt;
	SetTimer(1,1000,NULL);

	//if(CWnd::IsWindowVisible())
	{
		OnTimer(0);
	}
}
void CHoverButton::ShowAt(CPoint pt,CEditorCondition* pCondition, CWnd& Parent)
{
	GetCursorPos(&m_old);

	Parent.ClientToScreen(&pt);
	m_pt = pt;
	SetTimer(1,1000,NULL);

	//if(CWnd::IsWindowVisible())
	{
		OnTimer(0);
	}
}
void CHoverButton::ShowAt(CPoint pt,CEditorEvent* pEvent, CWnd& Parent)
{
	GetCursorPos(&m_old);

	Parent.ClientToScreen(&pt);
	m_pt = pt;
	SetTimer(1,1000,NULL);

	//if(CWnd::IsWindowVisible())
	{
		OnTimer(0);
	}
}
void CHoverButton::Hide()
{
	SetWindowPos(&wndTop, 0, 0,0,0, SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
}
void CHoverButton::SetOnClick(void (*pFunction)())
{
	m_pFunction = pFunction;
}

void CHoverButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect ClientRect;
	this->GetClientRect(&ClientRect);
	
	dc.FillSolidRect(ClientRect,RGB(255,255,255));
	dc.Draw3dRect(ClientRect,0,0);

	CFont* NewFont = gMyFontManager.RequestFont(14, 0, 0, 0, 0 ,0,
		   0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
		   FF_DONTCARE,"Arial");

	dc.SelectObject(NewFont);

	ClientRect.left += 4;
	ClientRect.top += 2;
	dc.SetTextColor(255);
	dc.SetBkMode(TRANSPARENT);

	CString text = " + New action";

	if(m_InsertType == CONDITION)
		text = " + New condition";

	if(m_InsertType == EVENT){
		text = " + New event";
		if(m_IsSub){
			text = " + New sub event";
		}
	}



	dc.DrawText( text, ClientRect, DT_CENTER);
	dc.SetTextColor(0);
}

void CHoverButton::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1)
	{
		Hide();
	}
	if(nIDEvent == 0)
	{
		m_pSetLayeredWindowAttributes(m_hWnd,RGB(0,0,0),230,2);
		SetWindowPos(&wndTop, m_pt.x, m_pt.y,0,0, SWP_NOACTIVATE|SWP_NOSIZE|SWP_SHOWWINDOW);
	}
	KillTimer(nIDEvent);
}

void CHoverButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_IsAfter) {
		if(m_IsSub) {
			pEventEditor->Insert_ClickSub();
		}
		else{
			pEventEditor->Insert_ClickAfter();
		}
	}
	else {
		pEventEditor->Insert_ClickBefore();
	}
}