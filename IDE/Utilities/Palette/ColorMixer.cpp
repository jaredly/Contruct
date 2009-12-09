// ColorMixer.cpp : implementation file
//

// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "ColorMixer.h"
#include "..\..\Editors\ImageEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define DAVOSPECIALCOLOUR g_PaintManager->GetColor(COLOR_3DSHADOW)//RGB(171,188,203)
/////////////////////////////////////////////////////////////////////////////
// CColorMixer

typedef DWORD HLSCOLOR;
#define HLS(h,l,s) ((HLSCOLOR)(((BYTE)(h)|((WORD)((BYTE)(l))<<8))|(((DWORD)(BYTE)(s))<<16)))

///////////////////////////////////////////////////////////////////////////////
#define HLS_H(hls) ((BYTE)(hls))
#define HLS_L(hls) ((BYTE)(((WORD)(hls)) >> 8))
#define HLS_S(hls) ((BYTE)((hls)>>16))

///////////////////////////////////////////////////////////////////////////////
HLSCOLOR RGB2HLS (COLORREF rgb);
COLORREF HLS2RGB (HLSCOLOR hls);

HLSCOLOR RGB2HLS (COLORREF rgb)
{
    unsigned char minval = min(GetRValue(rgb), min(GetGValue(rgb), GetBValue(rgb)));
    unsigned char maxval = max(GetRValue(rgb), max(GetGValue(rgb), GetBValue(rgb)));
    float mdiff  = float(maxval) - float(minval);
    float msum   = float(maxval) + float(minval);
   
    float luminance = msum / 510.0f;
    float saturation = 0.0f;
    float hue = 0.0f; 

    if ( maxval != minval )
    { 
        float rnorm = (maxval - GetRValue(rgb)  ) / mdiff;      
        float gnorm = (maxval - GetGValue(rgb)) / mdiff;
        float bnorm = (maxval - GetBValue(rgb) ) / mdiff;   

        saturation = (luminance <= 0.5f) ? (mdiff / msum) : (mdiff / (510.0f - msum));

        if (GetRValue(rgb) == maxval) hue = 60.0f * (6.0f + bnorm - gnorm);
        if (GetGValue(rgb) == maxval) hue = 60.0f * (2.0f + rnorm - bnorm);
        if (GetBValue(rgb) == maxval) hue = 60.0f * (4.0f + gnorm - rnorm);
        if (hue > 360.0f) hue = hue - 360.0f;
    }
    return HLS ((hue*255)/360, luminance*255, saturation*255);
}

///////////////////////////////////////////////////////////////////////////////
static BYTE _ToRGB (float rm1, float rm2, float rh)
{
  if      (rh > 360.0f) rh -= 360.0f;
  else if (rh <   0.0f) rh += 360.0f;
 
  if      (rh <  60.0f) rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;   
  else if (rh < 180.0f) rm1 = rm2;
  else if (rh < 240.0f) rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;      
                   
  return (BYTE)(rm1 * 255);
}

///////////////////////////////////////////////////////////////////////////////
COLORREF HLS2RGB (HLSCOLOR hls)
{
    float hue        = ((int)HLS_H(hls)*360)/255.0f;
    float luminance  = HLS_L(hls)/255.0f;
    float saturation = HLS_S(hls)/255.0f;

    if ( saturation == 0.0f )
    {
        return RGB (HLS_L(hls), HLS_L(hls), HLS_L(hls));
    }
    float rm1, rm2;
     
    if ( luminance <= 0.5f ) rm2 = luminance + luminance * saturation;  
    else                     rm2 = luminance + saturation - luminance * saturation;
    rm1 = 2.0f * luminance - rm2;   
    BYTE red   = _ToRGB (rm1, rm2, hue + 120.0f);   
    BYTE green = _ToRGB (rm1, rm2, hue);
    BYTE blue  = _ToRGB (rm1, rm2, hue - 120.0f);

    return RGB (red, green, blue);
}


CColorMixer::CColorMixer()
{
	lock_field_change = false;
	pColor1 = NULL;
	pColor2 = NULL;
}

CColorMixer::~CColorMixer()
{
}


BEGIN_MESSAGE_MAP(CColorMixer, CDialog)
	//{{AFX_MSG_MAP(CColorMixer)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()

	ON_EN_CHANGE(IDC_RED, &CColorMixer::OnTextFieldChange)
	ON_EN_CHANGE(IDC_GREEN, &CColorMixer::OnTextFieldChange)
	ON_EN_CHANGE(IDC_BLUE, &CColorMixer::OnTextFieldChange)
	ON_EN_CHANGE(IDC_ALPHA, &CColorMixer::OnTextFieldChange)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SWAP, &CColorMixer::OnBnClickedSwap)
END_MESSAGE_MAP()





void CColorMixer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpressionEditor)
	DDX_Control(pDX, IDC_GREEN, m_GreenBox);
	DDX_Control(pDX, IDC_RED, m_RedBox);
	DDX_Control(pDX, IDC_BLUE, m_BlueBox);

	//}}AFX_DATA_MAP
m_RedBox.Refresh();
m_GreenBox.Refresh();
m_BlueBox.Refresh();
}









/////////////////////////////////////////////////////////////////////////////
// CColorMixer message handlers

int CColorMixer::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

m_rainbow_click = false;
m_grad_click = false;

m_rainbow_x = 0;
m_rainbow_y = 0;
m_grad_pos = 0;

	Map.Create(64,67,24);
for(int x = 0; x < 64; x++)
{
	for(int y = 0; y < 67; y++)
	{
//	RGB2HLS (COLORREF rgb)
	COLORREF temp = HLS2RGB(HLS((BYTE)(x/63.0*255),128,(BYTE)(y/66.0*255)));
//	COLORREF temp = RGB(255,0,0);
	Map.SetPixelColor(x,y,temp);
	}
}

//COLORREF C = GetColor();
UseHSL = false;


m_leftdown = false;

	// TODO: Add your specialized creation code here
	//RGB2HLS (COLORREF rgb)
	return 0;
}

void CColorMixer::OnPaint() 
{
	{
CRect rcClient;
GetClientRect( &rcClient );
if( rcClient.IsRectEmpty() )
    return;

CPaintDC dc( this );
/*
CExtMemoryDC dc(
    &dcPaint,
    &rcClient
    );
*/
//CRgn rgnClient;
//if( rgnClient.CreateRectRgnIndirect( &rcClient ) )
  //  dc.SelectClipRgn( &rgnClient );






//		this->ShowSizeGrip(false);
	//CPaintDC dc2(this); // device context for painting


	rcClient.bottom = rcClient.top + 12;

	
	g_PaintManager->PaintDockerBkgnd( true, dc, this );






	//CMemDC dc(&dc2);
	
	CRect meh;
	GetClientRect(meh);
	//dc.DrawEdge(meh,BDR_RAISEDINNER|BDR_SUNKENOUTER,BF_RECT); 

	// TODO: Add your message handler code here
	dc.Draw3dRect(CRect(6,6,74,77),DAVOSPECIALCOLOUR,DAVOSPECIALCOLOUR); // round H/S
	dc.Draw3dRect(CRect(73,6,87,77),DAVOSPECIALCOLOUR,DAVOSPECIALCOLOUR); // round L
	dc.Draw3dRect(CRect(6,81,87,194),DAVOSPECIALCOLOUR,DAVOSPECIALCOLOUR); // round params


	//Rainbow map
	Map.Draw2(dc.m_hDC,8,8);
	dc.Draw3dRect(CRect(m_rainbow_x -2+8,m_rainbow_y - 2+8,
						m_rainbow_x +2+8,m_rainbow_y + 2+8),DAVOSPECIALCOLOUR,DAVOSPECIALCOLOUR);


	//Gradient map



	RGBQUAD gradmapcolor = Map.GetPixelColor(m_rainbow_x,Map.GetHeight()-1-m_rainbow_y,false);
	COLORREF Color = RGB(gradmapcolor.rgbRed,gradmapcolor.rgbGreen,gradmapcolor.rgbBlue);

	CPen MyPen;
	BYTE hue = HLS_H(RGB2HLS(Color));
	BYTE sat = HLS_S(RGB2HLS(Color));
	for(int y = 8;y < 75;y++ )
		dc.FillSolidRect(75,y,10,1,HLS2RGB(HLS(hue,(75-y)/67.0 * 255.0,sat)));

	dc.Draw3dRect(CRect(75+2,m_grad_pos - 2+8,
	75+8,m_grad_pos + 2+8),DAVOSPECIALCOLOUR,DAVOSPECIALCOLOUR);



//////////////	

	dc.FillSolidRect(CRect(7,82,86,193),RGB(255,255,255));
	MyPen.CreatePen(PS_SOLID,1,DAVOSPECIALCOLOUR);
	dc.SelectObject(&MyPen);
	dc.MoveTo(10,163);
	dc.LineTo(82,163);
	MyPen.DeleteObject();
	
	CBrush MyBrush;
	MyBrush.CreateSolidBrush(RGB(255,255,255));
	CPoint P[4];
	P[0].x  = 6;
	P[0].y  = 193;
	P[1].x  = 14;
	P[1].y  = 208;
	P[2].x  = 44-1;
	P[2].y  = 208;
	P[3].x  = 52-1;
	P[3].y  = 193;
	CPoint P2[4];

	P2[0].x  = 6+35;
	P2[0].y  = 193;
	P2[1].x  = 14+35;
	P2[1].y  = 208;
	P2[2].x  = 44+34;
	P2[2].y  = 208;
	P2[3].x  = 52+34;
	P2[3].y  = 193;
	dc.SetBkMode(TRANSPARENT );
if(UseHSL)
{
	CBrush Brush;
	Brush.CreateSolidBrush(g_PaintManager->GetColor(COLOR_3DFACE));
	dc.SelectObject(Brush);
dc.Polygon(P,4);
	Brush.DeleteObject();
	Brush.CreateSolidBrush(RGB(255,255,255));
	dc.SelectObject(Brush);
dc.Polygon(P2,4);
}
else
{
	CBrush Brush;
	Brush.CreateSolidBrush(g_PaintManager->GetColor(COLOR_3DFACE));
	dc.SelectObject(Brush);
dc.Polygon(P2,4);
	Brush.DeleteObject();
	Brush.CreateSolidBrush(RGB(255,255,255));
	dc.SelectObject(Brush);
dc.Polygon(P,4);
}




CFont MyFont;
			MyFont.CreateFont( 10, 
												0, 0, 0, 
												0,
												0, 0, 0, ANSI_CHARSET,
												OUT_DEFAULT_PRECIS,
												CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY,
												FF_MODERN,
												"Arial");




	dc.SelectObject(MyFont);
	dc.SetTextColor(RGB(0,0,0));
	dc.DrawText("RGB",CRect(19,196,45,207),DT_WORDBREAK);
			
	dc.DrawText("HSL",CRect(19+36,196,45+36,207),DT_WORDBREAK);

/////Draw R G B H S L
	CFont MyFont2;
			MyFont2.CreateFont( 14, 
												0, 0, 0, 
												0,
												0, 0, 0, ANSI_CHARSET,
												OUT_DEFAULT_PRECIS,
												CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY,
												FF_MODERN,
												"Arial"
										);
dc.SelectObject(MyFont2);
dc.SetTextColor(DAVOSPECIALCOLOUR);
CRect r(11,89,23,100);

if(!UseHSL)
{
	dc.DrawText("R:",r,DT_WORDBREAK);
	r.OffsetRect(0,26);
	dc.DrawText("G:",r,DT_WORDBREAK);
	r.OffsetRect(0,26);
	dc.DrawText("B:",r,DT_WORDBREAK);
}
else
{
	dc.DrawText("H:",r,DT_WORDBREAK);
	r.OffsetRect(0,26);
	dc.DrawText("S:",r,DT_WORDBREAK);
	r.OffsetRect(0,26);
	dc.DrawText("L:",r,DT_WORDBREAK);

}
//r.OffsetRect(0,30);
//dc.DrawText("O:",r,DT_WORDBREAK);

/////



	
//	MyPen.CreatePen(PS_SOLID,1,DAVOSPECIALCOLOUR);
	
	// Do not call CDialog::OnPaint() for painting messages



	MyPen.DeleteObject();	
	MyPen.CreatePen(PS_SOLID,1,RGB(255,255,255));
	dc.SelectObject(MyPen);
	int x = 7;

if(UseHSL)  x += 36;

	dc.MoveTo(x,193);
	dc.LineTo(x+ 44 - (UseHSL? 1:0),193);






/// And finally...draw the current colours!

// work out colours here!

	COLORREF Color1 = *pColor1;
	COLORREF Color2 = *pColor2;



	///Color2
	{
	CRect rc(32+2,219+2,65+3,240+3);
	dc.FillSolidRect(rc,DAVOSPECIALCOLOUR);
	rc.DeflateRect(1,1,1,1);
	dc.FillSolidRect(rc,0xffffff);
	rc.DeflateRect(2,2,2,2);

	CImageEditor* pView = m_pImageView;
/*	COLORREF Colora = RGB(255*(1-Color2Alpha/255.0) + (Color2Alpha/255.0)*GetRValue(Color2)
							,255*(1-Color2Alpha/255.0) + (Color2Alpha/255.0)*GetGValue(Color2)
							,255*(1-Color2Alpha/255.0) + (Color2Alpha/255.0)*GetBValue(Color2)
							);
	COLORREF Colorb = RGB(220*(1-Color2Alpha/255.0 )+ (Color2Alpha/255.0)*GetRValue(Color2)
							,220*(1-Color2Alpha/255.0) + (Color2Alpha/255.0)*GetGValue(Color2)
							,220*(1-Color2Alpha/255.0) + (Color2Alpha/255.0)*GetBValue(Color2)
							);*/

	COLORREF Colora = Color2;
	COLORREF Colorb = Color2;

	bool a = true;
	for(int y = rc.top; y < rc.bottom; y += 4)
		for(int x = rc.left; x < rc.right; x += 4)
		{
			a = !a;
 			if(a)  dc.FillSolidRect(CRect(x,y,x+4,y+4),Colora);
			else   dc.FillSolidRect(CRect(x,y,x+4,y+4),Colorb);
		}
	}
	///Color1
	{
	CRect rc(32+2-9,219+2-8,65+3-9,240+3-8);
	dc.FillSolidRect(rc,DAVOSPECIALCOLOUR);
	rc.DeflateRect(1,1,1,1);
	dc.FillSolidRect(rc,0xffffff);
	rc.DeflateRect(2,2,2,2);

	CImageEditor* pView = m_pImageView;
/*	COLORREF Colora = RGB(255*(1-Color1Alpha/255.0) + (Color1Alpha/255.0)*GetRValue(Color1)
							,255*(1-Color1Alpha/255.0) + (Color1Alpha/255.0)*GetGValue(Color1)
							,255*(1-Color1Alpha/255.0) + (Color1Alpha/255.0)*GetBValue(Color1)
							);
	COLORREF Colorb = RGB(220*(1-Color1Alpha/255.0 )+ (Color1Alpha/255.0)*GetRValue(Color1)
							,220*(1-Color1Alpha/255.0) + (Color1Alpha/255.0)*GetGValue(Color1)
							,220*(1-Color1Alpha/255.0) + (Color1Alpha/255.0)*GetBValue(Color1)
							);*/

	COLORREF Colora = Color1;
	COLORREF Colorb = Color1;


	bool a = true;
	for(int y = rc.top; y < rc.bottom; y += 4)
		for(int x = rc.left; x < rc.right; x += 4)
		{
			a = !a;
 			if(a)  dc.FillSolidRect(CRect(x,y,x+4,y+4),Colora);
			else   dc.FillSolidRect(CRect(x,y,x+4,y+4),Colorb);
		}
	}

//if( rgnClient.GetSafeHandle() != NULL )
//    dc.SelectClipRgn( &rgnClient );



}









	






}
bool CColorMixer::OnButtonDown(UINT nFlags, CPoint point) 
{
	if(!UseHSL)	
	{
		if(point.x >= 47 && point.x <= 86 && point.y>=193 && point.y<=208)
		{
			UseHSL = true;
			Invalidate(false);
			UpdateEditBoxs();
			return true;
		}
	}
	else
	{
		if(point.x >= 6 && point.x <= 47 && point.y>193   && point.y<=208)
		{
			UseHSL = false;
			Invalidate(false);
			UpdateEditBoxs();
			return true;
		}

	}
	if(point.x >= 8 && point.x <= 64+8 && point.y >= 8 && point.y <= 67 + 8)
	{
		//We are clicking on the rainbow
		m_leftdown = true;
		m_rainbow_click = true;
		CWnd::SetCapture();//SetCapture();

		OnMouseMove(nFlags,point);
		UpdateEditBoxs();
		return true;
	}
	else if(point.x >= 74 && point.x <= 85 && point.y >= 8 && point.y <= 74)
	{
		//We are clicking on the gradient
		m_leftdown = true;
		m_grad_click = true;
		CWnd::SetCapture();


		OnMouseMove(nFlags,point);
		UpdateEditBoxs();

		return true;
	}

	else if(CRect(25,213,58,234).PtInRect(point)) // left clicking on colour1
	{
		pColor = pColor1;
		FindGradPos();
		FindRainbowPos();
		UpdateEditBoxs();
		return true;
	}
	else if(CRect(35,221,67,242).PtInRect(point)) // left clicking on colour2
	{
		pColor = pColor2;
		FindGradPos();
		FindRainbowPos();
		UpdateEditBoxs();
		return true;
	}


return false;

}

void CColorMixer::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pColor = pColor1;
	if(OnButtonDown(nFlags,point)) return;
	CDialog::OnLButtonDown(nFlags, point);


}
void CColorMixer::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pColor = pColor2;

	if(OnButtonDown(nFlags,point)) return;
		CDialog::OnRButtonDown(nFlags, point);
}

void CColorMixer::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_leftdown = false;
	m_grad_click = false;
	m_rainbow_click = false;
	CDialog::OnLButtonUp(nFlags, point);

}


void CColorMixer::OnRButtonUp(UINT nFlags, CPoint point) 
{

	ReleaseCapture();
	m_leftdown = false;
	m_grad_click = false;
	m_rainbow_click = false;

	CDialog::OnRButtonUp(nFlags, point);
}




void CColorMixer::UpdateEditBoxs()
{
	if(lock_field_change)
		return;
	lock_field_change = true;
	if(UseHSL)
	{
		COLORREF hls = RGB2HLS(*pColor);
		CString format;
		format.Format("%d", GetRValue(hls));
		m_RedBox.SetWindowText(format);
		format.Format("%d", GetGValue(hls));
		m_GreenBox.SetWindowText(format);
		format.Format("%d", GetBValue(hls));
		m_BlueBox.SetWindowText(format);

	}
	else
	{
		CString format;
		format.Format("%d", GetRValue(*pColor));
		m_RedBox.SetWindowText(format);
		format.Format("%d", GetGValue(*pColor));
		m_GreenBox.SetWindowText(format);
		format.Format("%d", GetBValue(*pColor));
		m_BlueBox.SetWindowText(format);
	}
	lock_field_change = false;
}

COLORREF CColorMixer::GetColor()
{
//	COLORREF Color = m_pImageView->Color2;
//	return Color;
	return 0;
}

void CColorMixer::SetColor(COLORREF c)
{

}







void CColorMixer::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_rainbow_click)
	{
		point.Offset(-8,-8);

		//round the point
		if(point.x < 0) point.x = 0;
		if(point.y < 0 ) point.y = 0;
		if(point.x >= Map.GetWidth()) point.x = Map.GetWidth() - 1;
		if(point.y >= Map.GetHeight()) point.y = Map.GetHeight() - 1;


		RGBQUAD Colour =	Map.GetPixelColor(point.x,Map.GetHeight() - point.y-1);
		*pColor = RGB(Colour.rgbRed,Colour.rgbGreen,Colour.rgbBlue);
		
		m_rainbow_x = point.x;
		m_rainbow_y = point.y;



	}
	if(m_grad_click)
	{
		point.Offset(-75,-8);

		//round the point
		if(point.x < 0) point.x = 0;
		if(point.y < 0 ) point.y = 0;
		if(point.x >= Map.GetWidth()) point.x = Map.GetWidth() - 1;
		if(point.y >= Map.GetHeight()) point.y = Map.GetHeight() - 1;
		m_grad_pos  = point.y;



	}
	if(m_grad_click || m_rainbow_click)
	{
		//Should have made this a module :(
		RGBQUAD gradmapcolor = Map.GetPixelColor(m_rainbow_x,Map.GetHeight()-1-m_rainbow_y,false);
		COLORREF Color = RGB(gradmapcolor.rgbRed,gradmapcolor.rgbGreen,gradmapcolor.rgbBlue);

		BYTE hue = HLS_H(RGB2HLS(Color));
		BYTE sat = HLS_S(RGB2HLS(Color));
		int y = m_grad_pos;
		*pColor = HLS2RGB(HLS(hue,(67-y)/67.0 * 255.0,sat));
		Invalidate(false);
		m_pImageView->UpdateTool();

		UpdateEditBoxs();
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CColorMixer::FindGradPos()
{
	//We calculate the lightness we are after
	m_grad_pos = ( (255-HLS_L(RGB2HLS(*pColor))) * (Map.GetHeight()-1) ) / 255;
	Invalidate(false);
}

void CColorMixer::FindRainbowPos()
{
	m_rainbow_x = ( HLS_H(RGB2HLS(*pColor)) * (Map.GetWidth()-1) ) / 255;
	m_rainbow_y = ( (255-HLS_S(RGB2HLS(*pColor))) * (Map.GetHeight()-1) ) / 255;
	Invalidate(false);
}
BOOL CColorMixer::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return true;
}


int color_clamp(int a)
{
	if( a < 0)
		return 0;
	if( a > 255)
		return 255;
	return a;
}

void CColorMixer::OnTextFieldChange()
{


	if( lock_field_change)
		return;


	if(UseHSL)
	{
		CString h, s, l;
		m_RedBox.GetWindowText(h);
		m_GreenBox.GetWindowText(s);
		m_BlueBox.GetWindowText(l);
		
		*pColor = HLS2RGB( HLS( color_clamp(atoi(h)), color_clamp(atoi(l)), color_clamp(atoi(s))));
	}
	else
	{
		CString r, g, b;
		m_RedBox.GetWindowText(r);
		m_GreenBox.GetWindowText(g);
		m_BlueBox.GetWindowText(b);
		
		*pColor =  RGB( color_clamp(atoi(r)), color_clamp(atoi(g)), color_clamp(atoi(b)));
	}
	lock_field_change = true;

	FindGradPos();
	FindRainbowPos();



	m_pImageView->UpdateTool();

	lock_field_change = false;

}
void CColorMixer::OnBnClickedSwap()
{
	COLORREF Color1 = *pColor1;
	COLORREF Color2 = *pColor2;

	*pColor1 = Color2;
	*pColor2 = Color1;

	m_pImageView->UpdateTool();
	// TODO: Add your control notification handler code here
}

BOOL CColorMixer::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
			return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
