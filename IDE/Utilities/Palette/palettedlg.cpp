// PaletteDlg.cpp : implementation file
//

#include "stdafx.h"

#include "PaletteDlg.h"
#include "..\..\Editors\ImageEditor.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaletteDlg dialog


CPaletteDlg::CPaletteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteDlg::IDD, pParent)
{
	m_left = false;
	m_right = false;
	//{{AFX_DATA_INIT(CPaletteDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPaletteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaletteDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPaletteDlg, CDialog)
	//{{AFX_MSG_MAP(CPaletteDlg)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaletteDlg message handlers

int CPaletteDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	for(int x = 0; x < 256; x ++)
		Palette[x] = RGB(rand()%255,rand()%255,rand()%255);
	// TODO: Add your specialized creation code here
	color1 = 0;
	color2 = 1;
	return 0;
}

void CPaletteDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect Size;
	GetClientRect(&Size);
	
	int Cx = (Size.Width()-1) / 12;
	int Cy = (Size.Height()-1) / 12;

	int x = 0;
	int y = 0;

	for(int count = 0; count < 64; count ++)
	{
		if(x >= Cx) {x = 0; y ++;}
		if(y >= Cy) break;


	
		
		dc.FillSolidRect(x*12,y*12,13,13,0);
		
		if(color1 == count)
		{
			dc.FillSolidRect(x*12+2,y*12+2,13-4,13-4,0xffffff);
			dc.FillSolidRect(x*12+1+2,y*12+1+2,11-4,11-4,Palette[count]);
		}
		else if(color2 == count)
		{
			dc.FillSolidRect(x*12+2,y*12+2,13-4,13-4,0xffffff);
			dc.FillSolidRect(x*12+1+3,y*12+1+3,11-6,11-6,Palette[count]);
		}
		else
			dc.FillSolidRect(x*12+1,y*12+1,11,11,Palette[count]);
		

		x ++;




	}

//	dc.FillSolidRect(0,0,1000,1000,255);
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CPaletteDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	Invalidate(true);
	// TODO: Add your message handler code here
	
}

void CPaletteDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_left = true;
	m_right = false;
	SetCapture();
	OnMouseMove(nFlags,point);
	CDialog::OnLButtonDown(nFlags, point);
}

void CPaletteDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnMouseMove(nFlags,point);
	m_left = false;
	ReleaseCapture();

	CDialog::OnLButtonUp(nFlags, point);
}


void CPaletteDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCapture();
	m_right = true;
	m_left = false;
	OnMouseMove(nFlags,point);
	CDialog::OnRButtonDown(nFlags, point);

}

void CPaletteDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	m_right = false;
	OnMouseMove(nFlags,point);
	CDialog::OnRButtonUp(nFlags, point);
}

void CPaletteDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	int x = point.x;
	int y = point.y;
	if(m_left || m_right)
		{
			CRect Size;
			GetClientRect(&Size);
			int Cx = (Size.Width()-1) / 12;
			int Cy = (Size.Height()-1) / 12;



			
			int Position = (x-1)/12   + (y-1)/12 * Cx;
			if(Position >= 0 && Position <= 255)
			{
				if(m_left)
				{
					if(m_shiftheld) color1 = Position;
					*pColor1= Palette[Position];
					m_pImageView->UpdateTool();

				}
				if(m_right)
				{
					if(m_shiftheld) color2 = Position;
					*pColor2= Palette[Position];
					m_pImageView->UpdateTool();

				}
			
	


				Invalidate(false);
			}

		

		}	
	CDialog::OnMouseMove(nFlags, point);
}

void CPaletteDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	switch (nChar)
	{
		case VK_SHIFT:
			{
			m_shiftheld = true;
			SetCapture();
			}
	}
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPaletteDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	ReleaseCapture();
	m_shiftheld = false;
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}
