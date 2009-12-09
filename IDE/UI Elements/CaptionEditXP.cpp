// CaptionEditXP.cpp : implementation file
//

#include "stdafx.h"
#include "captioneditxp.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaptionEditXP

CCaptionEditXP::CCaptionEditXP()
{
}

CCaptionEditXP::~CCaptionEditXP()
{
}


BEGIN_MESSAGE_MAP(CCaptionEditXP, CDialog)
	//{{AFX_MSG_MAP(CCaptionEditXP)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_CREATE()
	ON_CONTROL(EN_CHANGE,100, OnChange)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCaptionEditXP message handlers

void CCaptionEditXP::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CRect myRect;
	GetClientRect(myRect);
	myRect.top += 12;
	myRect.bottom --;
	myRect.left ++;
	m_Edit.MoveWindow(myRect);
}

int CCaptionEditXP::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;


	CRect EditBoxPos;
	GetClientRect(EditBoxPos);
	EditBoxPos.top += 12;
	


	m_Edit.Create(WS_CHILD|WS_VISIBLE,
        CRect(EditBoxPos.left ,EditBoxPos.top ,  EditBoxPos.right ,EditBoxPos.bottom-3), this, 100);

	m_EditFont.CreateFont( 14, 
												0, 0, 0, 
												0,
												0, 0, 0, ANSI_CHARSET,
												OUT_DEFAULT_PRECIS,
												CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY,
												FF_MODERN,
												"Arial");	
	m_CaptionFont.CreateFont( 14, 
												0, 0, 0, 
												0,
												0, 0, 0, ANSI_CHARSET,
												OUT_DEFAULT_PRECIS,
												CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY,
												FF_MODERN,
												"Arial");		
	
	

	m_Edit.SetFont(&m_EditFont);

//	CDialog::ShowSizeGrip(0);


	return 0;
}

void CCaptionEditXP::OnPaint() 
{

	switch (m_type)
	{
	case ID_BRUSHSIZE:
		m_CaptionString = "Size";
		break;
	case ID_BRUSHTHICKNESS:
		m_CaptionString = "Thickness";
		break;
	case ID_BRUSHHARDNESS:
		m_CaptionString = "Hardness";
		break;
	case ID_BRUSHANGLE:
		m_CaptionString = "Angle";
		break;
	case ID_BRUSHSTEP:
		m_CaptionString = "Step";
		break;
	case ID_LINETHICKNESS:
		m_CaptionString = "Edge";
		break;
	case ID_OPACITY:
		m_CaptionString = "Opacity";
		break;
	case ID_FLOW:
		m_CaptionString = "Flow";
		break;
	case ID_ALPHA1:
		m_CaptionString = "Alpha 1";
		break;
	case ID_ALPHA2:
		m_CaptionString = "Alpha 2";
		break;
	}

	CRect rcClient;
	GetClientRect( &rcClient );


	CPaintDC dc(this);
	/*CExtMemoryDC dc(
		&dcPaint,
		&rcClient
		);*/



	rcClient.bottom = rcClient.top + 12;

	CRgn rgnClient;
	if( rgnClient.CreateRectRgnIndirect( &rcClient ) )
		dc.SelectClipRgn( &rgnClient );
	
	g_PaintManager->PaintDockerBkgnd( true, dc, this );

	CRect Rect;
	this->GetClientRect(Rect);


	Rect.left += 2;

	 // device context for painting
//	dc.Draw3dRect(Rect,0,0);
	dc.SetBkMode(TRANSPARENT);
	//PmBridge_GetPM()->PaintDockerBkgnd( true, dc, this );

	//dc.SetBkColor(g_PaintManager->GetColor(COLOR_3DFACE));
	CFont* Old = dc.SelectObject(&m_CaptionFont);
//	COLORREF Outline = g_PaintManager->GetColor (m_MouseMgr.MouseOver() ? COLOR_HIGHLIGHT : COLOR_3DSHADOW);

	Rect.top--;
	Rect.top--;
	Rect.top--;

	dc.DrawText(m_CaptionString,Rect,DT_LEFT|DT_SINGLELINE   );
	dc.SelectObject(Old);



	if( rgnClient.GetSafeHandle() != NULL )
		dc.SelectClipRgn( &rgnClient );




}

void CCaptionEditXP::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDialog::OnMouseMove(nFlags, point);
	Invalidate();
}


void CCaptionEditXP::OnMouseOut ()
{
	Invalidate();
}


void CCaptionEditXP::OnChange()
{
	if(pPicEd)
		if(pPicEd->CurrentTool)
		{
			CString String;
			m_Edit.GetWindowText(String);
			switch(m_type)
			{
				case ID_BRUSHSIZE:
					pPicEd->CurrentTool->ChangeBrushSize(atol(String));
					break;
				case ID_BRUSHHARDNESS:
					pPicEd->CurrentTool->ChangeHardness(atol(String));
					break;
				case ID_BRUSHTHICKNESS:
					pPicEd->CurrentTool->ChangeThickness(atol(String));
					break;
				case ID_BRUSHANGLE:
					pPicEd->CurrentTool->ChangeAngle(atol(String));
					break;
				case ID_BRUSHSTEP:
					pPicEd->CurrentTool->ChangeStep(atol(String));
					break;
				case ID_LINETHICKNESS:
					pPicEd->CurrentTool->ChangeLineThickness(atol(String));
					break;
				case ID_OPACITY:
					pPicEd->CurrentTool->ChangeOpacity(atol(String));
					break;
				case ID_FLOW:
					pPicEd->CurrentTool->ChangeFlow(atol(String));
					pPicEd->UpdateTool();
					break;
				case ID_ALPHA1:
					pPicEd->CurrentTool->ChangeAlpha1(atol(String));
					pPicEd->UpdateTool();
					break;
				case ID_ALPHA2:
					pPicEd->CurrentTool->ChangeAlpha2(atol(String));
					pPicEd->UpdateTool();
					break;
				
			}
			pPicEd->CurrentTool->Modify();
		}
}





////////////////////////////////////////
//	COutlineFillOption


BEGIN_MESSAGE_MAP(COutlineFillOption, CDialog)
	//{{AFX_MSG_MAP(CCaptionEditXP)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()




int COutlineFillOption::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;



//	CDialog::ShowSizeGrip(0);


	return 0;
}
void COutlineFillOption::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	CRect Area(4,0,13,21);
	Area.OffsetRect(0,11);

	for(int a = 0; a < 5; a++)
	{
		if(Area.PtInRect(point))
			pPicEd->m_Props.Style = a;
		Area.OffsetRect(12,0);
	}
	Invalidate();
	CDialog::OnLButtonDown(nFlags, point);
}

void COutlineFillOption::OnPaint()
{
	CPaintDC dc(this); // device context for painting


	CRect Rect;
	this->GetClientRect(Rect);

	Rect.left += 2;


	dc.SetBkMode(TRANSPARENT);
	g_PaintManager->PaintDockerBkgnd( true, dc, this );


	CFont m_CaptionFont;
	
	m_CaptionFont.CreateFont( 14, 
												0, 0, 0, 
												0,
												0, 0, 0, ANSI_CHARSET,
												OUT_DEFAULT_PRECIS,
												CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY,
												FF_MODERN,
												"Arial");	

	CFont* Old = dc.SelectObject(&m_CaptionFont);

	Rect.top--;
	Rect.top--;
	Rect.top--;

	dc.DrawText("Style:",Rect,DT_LEFT|DT_SINGLELINE   );
	dc.SelectObject(Old);


	int style = pPicEd->m_Props.Style;
	CRect Selection(1,1,16,21);
	Selection.OffsetRect(0,11);
	Selection.OffsetRect(12 * style, 0);
	dc.FillSolidRect(Selection, g_PaintManager->GetColor(COLOR_3DSHADOW));

	


	int Yoffest = 12;

	dc.Draw3dRect(CRect(0,11,65,32),g_PaintManager->GetColor(COLOR_3DSHADOW),g_PaintManager->GetColor(COLOR_3DSHADOW));

	CRect Area(3,14,14,29);
	dc.Draw3dRect(Area, RGB(0,0,65),RGB(0,0,65) );
	Area.OffsetRect(12,0);
	dc.FillSolidRect(Area, RGB(114,114, 255));
	dc.Draw3dRect(Area, RGB(0,0,65), RGB(0,0,65));
	Area.OffsetRect(12,0);	
	dc.FillSolidRect(Area, RGB(114,114, 255));
	Area.OffsetRect(12,0);	
	
	float ratio = 0;
	for(int x = Area.left; x < Area.right; x++)
	{
		ratio += 1/(Area.Width()+0.0);
		COLORREF col = RGB(11 + 102*ratio, 11 + 102*ratio, 65 + 190*ratio);
		dc.FillSolidRect(x, Area.top, 1, Area.Height(), col );
	}

	Area.OffsetRect(12,0);	
	ratio = 0;
	for(int y = Area.top; y < Area.bottom; y++)
	{
		ratio += 1/(Area.Height()+0.0);
		COLORREF col = RGB(11 + 102*ratio, 11 + 102*ratio, 65 + 190*ratio);
		dc.FillSolidRect(Area.left,y, Area.Width(), 1, col );
	}







	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}
