// ToolSettings.cpp : implementation file
//

// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "ToolSettings.h"
#include "..\..\Dialogs\ManageActionPointsDlg.h"
#include "..\..\Editors\ImageEditor.h"
#include "..\..\Dialogs\ImageEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolSettings

CToolSettings::CToolSettings()
{
}

CToolSettings::~CToolSettings()
{
}


BEGIN_MESSAGE_MAP(CToolSettings, CExtPanelControlBar)
	//{{AFX_MSG_MAP(CToolSettings)
	ON_EN_CHANGE(100, OnEditUpdate)
	ON_BN_CLICKED(128, OnActionPointModify)
	ON_BN_CLICKED(130, OnSmoothModify)
	ON_CBN_SELCHANGE(127, &CToolSettings::ActionSelectionChange )
	ON_CBN_DROPDOWN(127, &CToolSettings::ToolChanged )
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolSettings message handlers

void CToolSettings::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		m_pImgEd->m_PicEd.CurrentTool->Initialise(&m_pImgEd->m_PicEd.m_Props);
		KillTimer(1);
		m_pImgEd->Invalidate();

	}





}

void CToolSettings::OnEditUpdate()
{

}

void CToolSettings::ActionSelectionChange()
{
	SetTimer(1, 100, NULL);
}
void CToolSettings::ToolChanged()
{
	#define SHOW(x) Tools.push_back(&x);
		
	CString text;	
	m_ActionPointCombo.GetWindowText(text);

	// Hide everything
	m_BrushAngle.ShowWindow(false);
	m_BrushHardness.ShowWindow(false);
	m_BrushSize.ShowWindow(false);
	m_BrushStep.ShowWindow(false);
	m_BrushThickness.ShowWindow(false);
	m_OutlineFill.ShowWindow(false);
	m_LineThickness.ShowWindow(false);
	m_Smooth.ShowWindow(false);
	m_Opacity.ShowWindow(false);
	m_Flow.ShowWindow(false);
	m_Alpha1.ShowWindow(false);
	m_Alpha2.ShowWindow(false);


	m_ActionPointCombo.ShowWindow(false);
	m_ActionPointAdd.ShowWindow(false);

	vector<CWnd*> Tools;

	switch (*pTool)
	{
	case ID_PEN: 
		SHOW(m_Opacity)
		SHOW(m_Flow)
	break;
	case ID_FILL:
		SHOW(m_Alpha1)
		break;
	case ID_BRUSH:
	case ID_LINE:
	case ID_ERASOR:
		SHOW(m_BrushSize)
		SHOW(m_BrushThickness)
		SHOW(m_BrushHardness)
		SHOW(m_BrushAngle)
		//SHOW(m_BrushStep)
		SHOW(m_Opacity)
		SHOW(m_Flow)
		SHOW(m_Smooth)
		break;
	case ID_ELLIPSE:
	case ID_RECTANGLE:
		SHOW(m_LineThickness)
		SHOW(m_OutlineFill)
		SHOW(m_Alpha1)
		SHOW(m_Alpha2)
		break;
	case ID_ACTIONPOINT:
		m_ActionPointCombo.ShowWindow(SW_SHOW);
		m_ActionPointAdd.ShowWindow(SW_SHOW);
		m_ActionPointAdd.EnableWindow(SW_SHOW);


		map<CString, int> actions;
		this->m_pImgEd->m_pImgEdDlg->GetActionNames(actions);
		while(
			m_ActionPointCombo.DeleteString(0) > 0);
		
		for(map<CString, int>::iterator i = actions.begin(); i!= actions.end(); i++)
		{
			m_ActionPointCombo.AddString(i->first);
		}
		if(m_ActionPointCombo.GetCount() == 0)
		{
			m_ActionPointCombo.AddString("point");
		}
		if(text == "")
			m_ActionPointCombo.GetLBText(0, text);
		m_ActionPointCombo.SetWindowText(text);
		break;


	}


	int x = 5;
	for(int a = 0; a < Tools.size();a ++)
	{
		CRect wndrect;
		Tools.at(a)->GetClientRect(&wndrect);
		wndrect.OffsetRect(x,5);
		Tools.at(a)->MoveWindow(wndrect);
		Tools.at(a)->ShowWindow(true);
		x+= wndrect.Width()+5;
	}
	Invalidate();
}

void CToolSettings::OnBrushSizeChange()
{

}

void CToolSettings::OnActionPointModify()
{
	CManageActionPointsDlg Dlg;
	Dlg.m_pToolSettings = this;
	Dlg.DoModal();
}
void CToolSettings::OnSmoothModify()
{
	m_Smooth.SetCheck(1 - m_Smooth.GetCheck());
	if(m_pImgEd->m_PicEd.CurrentTool)
	{
		if(m_pImgEd->m_PicEd.CurrentTool->m_pProp)
		{
			if(m_pImgEd->m_PicEd.CurrentTool->m_pProp)
				m_pImgEd->m_PicEd.CurrentTool->m_pProp->bSmooth = m_Smooth.GetCheck();

		}
	}	
}

////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAnimationToolbar, CExtPanelControlBar)
	//{{AFX_MSG_MAP(CToolSettings)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CAnimationToolbar::CAnimationToolbar()
{
	m_pImgEd = NULL;
}


void CAnimationToolbar::OnPaint()
{
	// For some reason we're not ready to paint yet
	if (m_pImgEd == NULL)
		return;

	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);

	CRect middle;
	int index = m_pImgEd->m_pImgEdDlg->m_animation_index - rect.Width()/32/2;
	for(int x = 0; x < rect.Width(); x += 32, index++)
	{
		dc.FillSolidRect(x,0, 32, 40, 0x000000);
		dc.FillSolidRect(x,4, 32, 32,0x999999);

		if(index == m_pImgEd->m_pImgEdDlg->m_animation_index)
			middle = CRect(x,4,32+x,32+4);
		for(int o = 0; o < 34; o+= 2)
		{
			dc.FillSolidRect(x+o,1,  1,2,0xffffff);
			dc.FillSolidRect(x+o,37, 1,2,0xffffff);
		}
		m_thumbnails[index].Draw(dc.GetSafeHdc(), x, 4, 32, 32, 0, 0);
		dc.FillSolidRect(x+31,0, 1, 40, 0x000000);
	}

	middle.right --;
	middle.InflateRect(2,2);
	dc.Draw3dRect(middle, RGB(255,255,0), RGB(255,255,0));
	middle.InflateRect(1,1);
	dc.Draw3dRect(middle, 255, 255);

}


void CAnimationToolbar::UpdateThumbnail(int index)
{
	CxImage& dest = m_thumbnails[index];
	if(index < 0) return;
	if(index > this->m_pImgEd->m_pImgEdDlg->m_sourceImages.size() ) return;

	CImageResource* resource = m_pImgEd->m_pImgEdDlg->m_sourceImages.at(index);

	CxImage* source = &resource->bitmap;
	if(m_pImgEd->m_pImgEdDlg->m_newImages.find(resource) != m_pImgEd->m_pImgEdDlg->m_newImages.end())
	{
		source = &m_pImgEd->m_pImgEdDlg->m_newImages[resource];
	}
	CreatePremultipliedThumbnail(source, &dest, 32,32, 0xffffff, 0);
}


void CAnimationToolbar::ChangeFrame(int newindex)
{
	if (m_pImgEd == NULL) return;

	CImageEditorDlg* Dlg = m_pImgEd->m_pImgEdDlg;
	m_pImgEd->SetRedraw(false);

	int oldindex = Dlg->m_animation_index;

	if(newindex >= Dlg->m_sourceImages.size())
		return m_pImgEd->SetRedraw(true);	//todo: add new frame
	if(newindex < 0)
		return m_pImgEd->SetRedraw(true); //todo: add new frame before
	{
		if(m_pImgEd->m_PicEd.m_modded)
		{
			m_pImgEd->m_PicEd.Save(&
			Dlg->m_newImages[ Dlg->m_sourceImages.at(oldindex) ] );
		// this->m_sourceImages[m_animation_index] = m_newImages[ m_sourceImages.at(m_animation_index) ] 
		}
		this->UpdateThumbnail(oldindex);



		Dlg->m_animation_index = newindex;
		CImageResource* newimage = Dlg->m_sourceImages.at(newindex);


		
		CPictureEditor* pEd = &m_pImgEd->m_PicEd;
		Dlg->m_pMyImage = &Dlg->m_sourceImages.at(newindex)->bitmap;
		if(Dlg->m_newImages.find(newimage) != Dlg->m_newImages.end())
		{
			Dlg->m_pMyImage = &Dlg->m_newImages[newimage];
		}
		
		// we need to free each layer.
		for(int a = 0; a < pEd->layers.size(); a++)
		{
			pEd->display->RemoveTexture(pEd->layers.at(a));
		}
		pEd->layers.clear();

		// Now its time to load up the layers in the new image we are editing
		int m_CanvasBG = pEd->display->AddTextureFromCxImage(*Dlg->m_pMyImage);

		pEd->m_CanvasWidth = pEd->m_Width = Dlg->m_pMyImage->GetWidth();
		pEd->m_CanvasHeight = pEd->m_Height = Dlg->m_pMyImage->GetHeight();

		pEd->layers.push_back(pEd->display->AddTextureRT(pEd->m_Width,pEd->m_Height));
		pEd->m_pCanvas = &pEd->layers.at(0);
		pEd->m_pLayer = pEd->m_pCanvas;



		
		pEd->display->SetRenderTarget(*pEd->m_pCanvas);
		pEd->display->SetTexture(-1);

		pEd->SaveDisplayState();
		pEd->display->SetTexture(m_CanvasBG);
		
		
		pEd->display->ClearRT();
				
		pEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
		pEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);

		pEd->display->Blit(0,0);
		pEd->RestoreDisplayState();





		pEd->display->RemoveTexture(m_CanvasBG);

		pEd->m_RectTempToCanvas.SetRect(0,0,pEd->m_Width,pEd->m_Height);
		pEd->CanvasToTemp();

		pEd->m_pImageEditor->ChangeTool();

		m_pImgEd->UpdateScrollbars();

		while(pEd->m_undo.size()>0)
		{
			pEd->m_undo.back()->Release();
			delete pEd->m_undo.back();
			pEd->m_undo.pop_back();
		}
		while(pEd->m_redo.size()>0)
		{
			pEd->m_redo.back()->Release();
			delete pEd->m_redo.back();
			pEd->m_redo.pop_back();
		}
		
		m_pImgEd->SetRedraw(true);
		Invalidate();
		m_pImgEd->RedrawWindow();
		pEd->CreateUndo();
	}
}

void CAnimationToolbar::AdvanceFrames(int offset)
{
	CImageEditorDlg* Dlg = m_pImgEd->m_pImgEdDlg;
	int newindex = Dlg->m_animation_index + offset;
	

	if(newindex >= Dlg->m_sourceImages.size())
		return;
	if(newindex < 0)
		return;
	
	ChangeFrame(newindex);
}

void CAnimationToolbar::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);

	int newindex = -1;
	bool found = false;

	int index = m_pImgEd->m_pImgEdDlg->m_animation_index - rect.Width()/32/2;
	for(int x = 0; x < rect.Width(); x += 32, index++)
	{
		CRect zone(x,4, x+32,4+32);
		if(zone.PtInRect(point))
		{
			found = true;
			newindex = index;
		}
	}

	if(found)
		ChangeFrame(newindex);

}