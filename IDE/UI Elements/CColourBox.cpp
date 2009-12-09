// CColourBox.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CColourBox.h"

// Class definitions

BEGIN_MESSAGE_MAP(CColourBox, CWnd)
	//{{AFX_MSG_MAP(CColourBox)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_EN_CHANGE(100, OnEditChange)
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Constructor
CColourBox::CColourBox()
{
	curValue = 0;
	progressPosition = 0;
}

// Create controls
void CColourBox::Initialise()
{
	CRect theRect; this->GetWindowRect(theRect);

	controlWidth = theRect.right;
	controlHeight = theRect.bottom;

	if (!baseEdit.IsWindowVisible())
	{
		baseEdit.Create(ES_MULTILINE | ES_NUMBER | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        CRect(0, 0, controlWidth - 5, 25), this, 100);
	}

	if (!spinControl.IsWindowVisible())
	{
		spinControl.Create(UDS_ALIGNRIGHT | WS_CHILD | WS_VISIBLE,
        CRect(0, 0, 5, 5), this, 101);

		spinControl.SetBuddy(&baseEdit);

		spinControl.SetRange(0, 255);
	}
}

// Do drawing
//
void CColourBox::OnPaint() 
{
	CPaintDC dcOrigin(this);

	dcOrigin.Draw3dRect(0, 25, controlWidth, 1, RGB(2, 2, 2), RGB(2, 2, 2));

	if (!isOver)
	{
		dcOrigin.FillSolidRect(0, 26, controlWidth - 2, 8, RGB(backR, backG, backB));

		dcOrigin.FillSolidRect(0, 26, progressPosition - 2, 8, RGB(progR, progG, progB));
	}

	else
	{
		dcOrigin.FillSolidRect(0, 26, controlWidth - 2, 8, RGB(200, 200, 200));

		dcOrigin.FillSolidRect(0, 26, progressPosition - 2, 8, RGB(223, 223, 223));

		isOver = false;
	}
}

// Erase background
//
BOOL CColourBox::OnEraseBkgnd(CDC* pDC) 
{	
	return TRUE;
}

// When the user enters a new integer, update
//
void CColourBox::OnEditChange() 
{
	// Get new value
	CString theText;
	baseEdit.GetWindowText(theText);

	curValue = atoi(theText);

	if (curValue > 255)
		curValue = 255;

	// Work out new progress
	progressPosition = ((curValue / 255.0) * controlWidth);

	// Redraw
	Invalidate(true);
}

// Spin control updated
//
void CColourBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (nSBCode == SB_THUMBPOSITION)
	{
		curValue = nPos;

		if (curValue > 255)
			curValue = 255;

		CString curText;
		curText.Format("%d", nPos);
		baseEdit.SetWindowText(curText);

		// Work out new progress
		progressPosition = ((curValue / 255.0) * controlWidth);

		// Redraw
		Invalidate(true);
	}

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CColourBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPoint curPos;
	GetCursorPos(&curPos);

	Invalidate(true);

	isOver = true;
	
	CWnd::OnMouseMove(nFlags, point);
}

void CColourBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CColourBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CColourPopup* m_gradientChooser = new CColourPopup();
			m_gradientChooser->Create(WS_POPUP,this,0);
		
	CWnd::OnLButtonDown(nFlags, point);
}


BEGIN_MESSAGE_MAP(CColourPopup, CWnd)
	//{{AFX_MSG_MAP(CColourPopup)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CColourPopup::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

#include "XP/MemDC.H"

void CColourPopup::OnPaint() 
{
	// Create a memory DC for a non flickering draw
	CPaintDC paintDC(this);
	CMemDC dc(&paintDC);

	// Draw the gradient bar

	CRect rect;
	GetClientRect(&rect);

	COLORREF color = RGB(128, 128, 128);
	int type = 1;

	if (type == 1)
	{
		for(int i=0;i<256;i++)
		{
			CPen pen(PS_SOLID,1,RGB(i,GetGValue(color),GetBValue(color)));
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(i,0);
			dc.LineTo(i,14);
			dc.SelectObject(oldPen);
		}
	}
	else if (type == 2)
	{
		for(int i=0;i<256;i++)
		{
			CPen pen(PS_SOLID,1,RGB(GetRValue(color),i,GetBValue(color)));
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(i,0);
			dc.LineTo(i,14);
			dc.SelectObject(oldPen);
		}
	}
	else if (type == 3)
	{
		for(int i=0;i<256;i++)
		{
			CPen pen(PS_SOLID,1,RGB(GetRValue(color),GetGValue(color),i));
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(i,0);
			dc.LineTo(i,14);
			dc.SelectObject(oldPen);
		}
	}
/*	else if (type == 4)
	{
		BYTE hue,sat,light;
		m_comp->GetOwnerSet()->GetColor(&hue,&sat,&light);

		for(int i=0;i<256;i++)
		{
			CPen pen(PS_SOLID,1,GWCColorFunc::HSLtoRGB((double)i/255.0,(double)sat/255.,(double)light/255.));
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(i,0);
			dc.LineTo(i,14);
			dc.SelectObject(oldPen);
		}
	}*/
/*	else if (type == 5)
	{
		BYTE hue,sat,light;
		m_comp->GetOwnerSet()->GetColor(&hue,&sat,&light);

		for(int i=0;i<256;i++)
		{
			CPen pen(PS_SOLID,1,GWCColorFunc::HSLtoRGB((double)hue/255.,(double)i/255.0,(double)light/255.));
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(i,0);
			dc.LineTo(i,14);
			dc.SelectObject(oldPen);
		}
	}*/
/*	else if (type == 6)
	{
		BYTE hue,sat,light;
		m_comp->GetOwnerSet()->GetColor(&hue,&sat,&light);

		for(int i=0;i<256;i++)
		{
			CPen pen(PS_SOLID,1,GWCColorFunc::HSLtoRGB((double)hue/255.,(double)sat/255.,(double)i/255.0));
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(i,0);
			dc.LineTo(i,14);
			dc.SelectObject(oldPen);
		}
	}*/

	// Draw a mark for the current value
	//BYTE hue,sat,light;
	//m_comp->GetOwnerSet()->GetColor(&hue,&sat,&light);
	//double dHue = (double)hue/255. + 0.5;
	//if (dHue > 1.0) dHue -= 1.0;
	//COLORREF markColor = GWCColorFunc::HSLtoRGB(dHue,1.0,0.5);
	//CPen pen(PS_SOLID,1,markColor);
	//CPen* oldPen = dc.SelectObject(&pen);
	//BYTE value = m_comp->GetValue();
	//dc.MoveTo(value,0);
	//dc.LineTo(value,14);
	//dc.MoveTo(value-1,0);
	//dc.LineTo(value-1,14);
	//dc.MoveTo(value+1,0);
	//dc.LineTo(value+1,14);
	//dc.SelectObject(oldPen);
}