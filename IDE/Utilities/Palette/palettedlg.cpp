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
	
	//for(int x = 0; x < 256; x ++)
	//	Palette[x] = RGB(rand()%255,rand()%255,rand()%255);

	// Hardcoded palette

	Palette[0] = RGB(0,0,0);
	Palette[1] = RGB(255,255,255);

	Palette[2] = RGB(77,0,0);
	Palette[3] = RGB(124,0,0);
	Palette[4] = RGB(179,0,0);
	Palette[5] = RGB(255,0,0);
	Palette[6] = RGB(255,148,148);

	Palette[7] = RGB(60,18,0);
	Palette[8] = RGB(117,35,0);
	Palette[9] = RGB(195,59,0);
	Palette[10] = RGB(255,77,0);
	Palette[11] = RGB(255,153,109);

	Palette[12] = RGB(54,54,0);
	Palette[13] = RGB(121,121,0);
	Palette[14] = RGB(193,193,0);
	Palette[15] = RGB(255,255,0);
	Palette[16] = RGB(255,255,143);

	Palette[17] = RGB(47,71,0);
	Palette[18] = RGB(74,113,0);
	Palette[19] = RGB(126,192,0);
	Palette[20] = RGB(168,255,0);
	Palette[21] = RGB(221,255,155);

	Palette[22] = RGB(0,47,0);
	Palette[23] = RGB(0,101,0);
	Palette[24] = RGB(0,164,0);
	Palette[25] = RGB(0,255,0);
	Palette[26] = RGB(119,255,119);

	Palette[27] = RGB(0,43,43);
	Palette[28] = RGB(0,92,92);
	Palette[29] = RGB(0,167,167);
	Palette[30] = RGB(0,255,255);
	Palette[31] = RGB(132,255,255);

	Palette[32] = RGB(0,0,0);
	Palette[33] = RGB(255,255,255);

	Palette[34] = RGB(0,24,44);
	Palette[35] = RGB(0,50,92);
	Palette[36] = RGB(0,82,152);
	Palette[37] = RGB(0,138,255);
	Palette[38] = RGB(131,198,255);

	Palette[39] = RGB(0,0,46);
	Palette[40] = RGB(0,0,97);
	Palette[41] = RGB(0,0,255);
	Palette[42] = RGB(82,82,255);
	Palette[43] = RGB(141,141,255);

	Palette[44] = RGB(23,0,23);
	Palette[45] = RGB(64,0,92);
	Palette[46] = RGB(110,0,158);
	Palette[47] = RGB(178,0,255);
	Palette[48] = RGB(219,133,255);

	Palette[49] = RGB(39,0,39);
	Palette[50] = RGB(96,0,96);
	Palette[51] = RGB(198,0,198);
	Palette[52] = RGB(255,0,255);
	Palette[53] = RGB(255,125,255);

	Palette[54] = RGB(0,0,0);
	Palette[55] = RGB(36,36,36);
	Palette[56] = RGB(64,64,64);
	Palette[57] = RGB(104,104,104);
	Palette[58] = RGB(128,128,128);
	Palette[59] = RGB(153,153,153);
	Palette[60] = RGB(187,187,187);
	Palette[61] = RGB(216,216,216);
	Palette[62] = RGB(236,236,236);
	Palette[63] = RGB(255,255,255);





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
					m_pImageView->m_pColor_Mixer->UpdateColour();

				}
				if(m_right)
				{
					if(m_shiftheld) color2 = Position;
					*pColor2= Palette[Position];
					m_pImageView->UpdateTool();
					m_pImageView->m_pColor_Mixer->UpdateColour();
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
