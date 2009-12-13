// ImageEditor.cpp : implementation file
//

// File includes
#include "StdAfx.h"
#include "..\Construct.h"
#include "ImageEditor.h"
#include "..\Dialogs\ImageEditorDlg.h"
#include "..\MainFrm.h"
#include "..\Dialogs\PicEdResizeDlg.h"
#include"..\..\LIBS\Include\Resources\Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE ghInst;

/////////////////////////////////////////////////////////////////////////////
// CImageEditor

IMPLEMENT_DYNCREATE(CImageEditor, CScrollView)

CImageEditor::CImageEditor()
{
	m_usermoved = true;
	m_app = NULL;
	m_pLayout = NULL;
	m_display = NULL;
	drawingDisabled = false;
}

CImageEditor::~CImageEditor()
{
	//if (m_display)
	//	delete m_display;
//	CScrollView::~CScrollView();
}

CFloatPoint MakePOINTS(CPoint Point)
{
	CFloatPoint m;
	m.x = Point.x;
	m.y = Point.y;
	return m;
}

BEGIN_MESSAGE_MAP(CImageEditor, CScrollView)
	//{{AFX_MSG_MAP(CImageEditor)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()

	// tools
	ON_COMMAND(ID_SELECT_FREE, OnSelectFree)
	ON_COMMAND(ID_SELECT_POLYGON, OnSelectPolygon)
	ON_COMMAND(ID_SELECT_RECTANGLE, OnSelectRectangle)
	ON_COMMAND(ID_SELECT_ELLIPSE, OnSelectEllipse)
	ON_COMMAND(ID_SELECT_WAND, OnSelectWand)	
	ON_COMMAND(ID_SELECT_COLOR, OnSelectColor)
	ON_COMMAND(ID_ZOOM, OnZoom)
	ON_COMMAND(ID_PICK, OnPick)
	ON_COMMAND(ID_PEN, OnPen)
	ON_COMMAND(ID_BRUSH, OnBrush)
	ON_COMMAND(ID_CUSTOMBRUSH,OnCustomBrush)
	ON_COMMAND(ID_TUBE, OnTube)
	ON_COMMAND(ID_ERASOR, OnErasor)
	ON_COMMAND(ID_TEXT, OnText)
	ON_COMMAND(ID_FILL, OnFill)
	ON_COMMAND(ID_ARROW, OnArrow)
	ON_COMMAND(ID_LINE, OnLine)
	ON_COMMAND(ID_RECTANGLE, OnRectangle)
	ON_COMMAND(ID_POLYGON, OnPolygon)
	ON_COMMAND(ID_ELLIPSE, OnEllipse)
	ON_COMMAND(ID_HOTSPOT, OnHotspot)
	ON_COMMAND(ID_ACTIONPOINT, OnActionpoint)
	ON_COMMAND(ID_BACKGROUNDCOLOR, OnBackground)

	// main toolbar
	ON_COMMAND(ID_RESIZE,OnResize)
	ON_COMMAND(ID_UNDO,OnUndo)
	ON_COMMAND(ID_EDIT_UNDO,OnUndo)
	ON_COMMAND(ID_REDO,OnRedo)
	ON_COMMAND(ID_EDIT_REDO,OnRedo)
	ON_COMMAND(ID_MIRROR,OnMirror)
	ON_COMMAND(ID_FLIP,OnFlip)
	ON_COMMAND(ID_ROTATE, OnRotate)
	ON_COMMAND(ID_GRID, OnGrid)

	ON_COMMAND(ID_NEW, OnNew)
	ON_COMMAND(ID_CUT, OnCut)
	ON_COMMAND(ID_EDIT_CUT, OnCut)
	ON_COMMAND(ID_COPY, OnCopy)
	ON_COMMAND(ID_EDIT_COPY, OnCopy)
	ON_COMMAND(ID_PASTE, OnPaste)
	ON_COMMAND(ID_EDIT_PASTE, OnPaste)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectAll)
	ON_COMMAND(ID_WRAP, OnWrap)
	ON_COMMAND(ID_MASK, OnMask)

	ON_COMMAND(ID_CROP, OnCrop)

	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONUP()
	ON_WM_ENTERIDLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CImageEditor::OnDraw(CDC* pDC)
{
	if (drawingDisabled) {
		m_PicEd.m_modded = false;
		m_pImgEdDlg->EndDialog(0);
		return;
	}

	m_PicEd.m_ScrollX = GetScrollPos(0);
	m_PicEd.m_ScrollY = GetScrollPos(1);
	m_PicEd.Render();

	if(m_PicEd.m_Sel2.size() > 0)
		SetTimer(0,30,0);

	// Get window size
	RECT clientRec;
	GetClientRect(&clientRec);

	m_PicEd.display->EndScene();
	m_display->Present(&clientRec, this->GetSafeHwnd());
	m_PicEd.display->BeginScene();

	//m_PicEd.PostRender();
}


/////////////////////////////////////////////////////////////////////////////
// CImageEditor diagnostics

#ifdef _DEBUG
void CImageEditor::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageEditor::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageEditor message handlers


int CImageEditor::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CImageEditor::GetParentOwner()));

	m_zoom_val = 0; 	

	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;


	



	/* The Picture Editor itself*/
	//m_display = new CD3DDisplay;
	SetWindowPos(NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOMOVE);

	/*
	CD3DDisplaySetup setup;
	setup.bFullscreen = false;
	setup.fpsMode = CD3DFPS_VSYNCED;
	setup.hWnd = m_hWnd;
	setup.hFocusWnd = GetTopWindow()->GetSafeHwnd();
	setup.resFormat = D3DFMT_A8R8G8B8;
	setup.resWidth = GetSystemMetrics(SM_CXSCREEN);
	setup.resHeight = GetSystemMetrics(SM_CYSCREEN);
	setup.swapEffect = D3DSWAPEFFECT_COPY; // for windowing		
	m_display->Init(&setup);
	*/

	
	
	return 0;
}

void CImageEditor::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
	Invalidate(false);
}

void CImageEditor::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
	Invalidate(false);
}

void CImageEditor::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	UpdateScrollbars();
}

void CImageEditor::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//Color1 = RGB(rand()%255,rand()%255,rand()%255);
	m_PicEd.OnLMouseDown(MakePOINTS(point));
	SetCapture();
	Invalidate();	
	UpdateCursor();
	
	CScrollView::OnLButtonDown(nFlags, point);
}

BOOL CImageEditor::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return 0;
}

void CImageEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	m_pPaletteBar->m_wndChild.OnKeyDown(nChar,nRepCnt,nFlags);
	//m_pColor_Mixer->OnKeyDown(nChar,nRepCnt,nFlags);

	UpdateCursor();

	// Commented out due to C4060 warning
	/*
	switch (nChar)
	{
		break;
	}
	*/

	m_PicEd.OnKeyDown(nChar);


	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CImageEditor::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	m_pPaletteBar->m_wndChild.OnKeyUp(nChar,nRepCnt,nFlags);
//	m_pColor_Mixer->OnKeyUp(nChar,nRepCnt,nFlags);

	UpdateCursor();
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);

}

void CImageEditor::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	m_MouseX = point.x;
	m_MouseY = point.y;
	m_PicEd.OnLMouseDouble(MakePOINTS(point));
	UpdateCursor();
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CImageEditor::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	m_MouseX = point.x;
	m_MouseY = point.y;
	m_PicEd.OnLMouseDouble(MakePOINTS(point));
	UpdateCursor();
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CImageEditor::OnMouseMove(UINT nFlags, CPoint point) 
{
	UpdateCursor();

	// Middle click drag?
	if (((GetKeyState(VK_MBUTTON)>>4) || ((GetKeyState(VK_RMENU)>>4)& GetKeyState(VK_LBUTTON)>>4)) && m_usermoved) 
	{
		// Scroll this movement

		CPoint pt = point;

		SetRedraw(false);
//		LockScrollUpdate();
		int dx = m_MouseX - pt.x;
		int dy = m_MouseY - pt.y;

		m_usermoved = false;
		ScrollToPosition(GetScrollPosition() + CPoint(dx,dy));

		SetRedraw(true);

		// Update old mouse position
		

	//	UnlockScrollUpdate();
		Invalidate();
		RedrawWindow();

		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
	
		m_MouseX = point.x;
		m_MouseY = point.y;

		m_usermoved = true;
		return;
	}









	m_MouseX = point.x;
	m_MouseY = point.y;

	m_PicEd.OnMouseMove(MakePOINTS(point));
	Invalidate();
	CScrollView::OnMouseMove(nFlags, point);

}

void CImageEditor::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_MouseX = point.x;
	m_MouseY = point.y;

	ReleaseCapture();
	m_PicEd.OnLMouseUp(MakePOINTS(point));
	Invalidate();
	CScrollView::OnLButtonUp(nFlags, point);

}

void CImageEditor::OnTimer(UINT nIDEvent) 
{
	KillTimer(0);
	Invalidate(false);

	UpdateCursor();
	CScrollView::OnTimer(nIDEvent);
}
void CImageEditor::ChangeTool()
{
	m_PicEd.displayOffsetEnabled = false;

	for(int a = 0; a < 30; a++)
	m_pImgEdDlg->m_Tools.SetButtonStyle(a,TBBS_CHECKBOX);

	//m_pImgEdDlg->m_Tools.GetButton(index)->SetStyle(TBBS_CHECKED);
	//m_pImgEdDlg->m_Tools.GetToolBarCtrl().CheckButton(m_tool,false);
	
	//SetButtonStyle(index,TBBS_CHECKBOX);

	// Don't do anything if the user is trying to use an image point in an icon
	if ((m_tool == ID_ACTIONPOINT || m_tool == ID_HOTSPOT) && m_pImgEdDlg->m_Icon)
		return;

	if(m_PicEd.CurrentTool)
		delete m_PicEd.CurrentTool;

	switch (m_tool)
	{
	case ID_SELECT_FREE:
		m_PicEd.CurrentTool = new CSelectFreeTool();
		break;
	case ID_SELECT_POLYGON:
		m_PicEd.CurrentTool = new CSelectPolygonTool();
		break;
	case ID_SELECT_RECTANGLE:
		m_PicEd.CurrentTool = new CSelectRectangleTool();
		break;
	case ID_SELECT_ELLIPSE:
		m_PicEd.CurrentTool = new CSelectEllipseTool();
		break;
	case ID_SELECT_WAND:
		m_PicEd.CurrentTool = new CSelectWandTool();
		break;
	case ID_SELECT_COLOR:
		m_PicEd.CurrentTool = new CSelectColorTool();
		break;
	case ID_ZOOM:
		m_PicEd.CurrentTool = new CZoomTool();
		break;
	case ID_PICK:
		m_PicEd.CurrentTool = new CPickTool();
		((CPickTool*)m_PicEd.CurrentTool)->lasttool = m_oldtool;
		break;
	case ID_PEN:
		m_PicEd.CurrentTool = new CPenTool();
		break;
	case ID_BRUSH:
		m_PicEd.CurrentTool = new CBrushTool();
		break;
	case ID_CUSTOMBRUSH:
		m_PicEd.CurrentTool = new CCustomBrushTool();
		break;
	case ID_TUBE:
		m_PicEd.CurrentTool = new CTubeTool();
		break;
	case ID_ERASOR:
		m_PicEd.CurrentTool = new CErasorTool();
		break;
	case ID_TEXT:
		m_PicEd.CurrentTool = new CTextTool();
		break;
	case ID_FILL:
		m_PicEd.CurrentTool = new CFillTool();
		break;
	case ID_ARROW:
		m_PicEd.CurrentTool = new CArrowTool();
		break;
	case ID_LINE:
		m_PicEd.CurrentTool = new CLineTool();
		break;
	case ID_RECTANGLE:
		m_PicEd.CurrentTool = new CRectangleTool();
		break;
		// CPolygonTool commented out due to unresolved externals.  AG 11 Feb
	case ID_POLYGON:
		m_PicEd.CurrentTool = new CPolygonTool();
		break;
	case ID_ELLIPSE:
		m_PicEd.CurrentTool = new CShapeTool();
		((CShapeTool*)m_PicEd.CurrentTool)->m_shape = ID_ELLIPSE;
		// later i might have custom shapes the user can draw in a separate image
		break;
	case ID_HOTSPOT:
		{
			m_PicEd.displayOffsetEnabled = true;
			m_PicEd.CurrentTool = new CHotspotTool();
		break;
		}
	case ID_ACTIONPOINT:
		{
			m_PicEd.displayOffsetEnabled = true;
			m_PicEd.CurrentTool = new CActionTool();
		break;
		}
	default:
		{
			//We have an unknown tool...default to the brush tool
			m_tool = ID_BRUSH;
			m_PicEd.CurrentTool = new CBrushTool();
		}
	}

	m_pTool_Settings->ToolChanged();
	m_PicEd.CurrentTool->PicEd = &m_PicEd;
	m_PicEd.CurrentTool->Initialise(&m_PicEd.m_Props);
	UpdateTool();

	int index = m_pImgEdDlg->m_Tools.CommandToIndex(m_tool);
	m_pImgEdDlg->m_Tools.SetButtonStyle(index,TBBS_CHECKED);

	m_oldtool = m_tool;
	
	UpdateCursor();

	
	Invalidate();


}

void CImageEditor::OnSelectFree() 
{
	m_tool = ID_SELECT_FREE;
	ChangeTool();
}

void CImageEditor::OnSelectPolygon() 
{
	m_tool = ID_SELECT_POLYGON;
	ChangeTool();
}
void CImageEditor::OnSelectRectangle() 
{
	m_tool = ID_SELECT_RECTANGLE;
	ChangeTool();
}

void CImageEditor::OnSelectEllipse() 
{
	m_tool = ID_SELECT_ELLIPSE;
	ChangeTool();
}

void CImageEditor::OnSelectWand() 
{
	m_tool = ID_SELECT_WAND;
	ChangeTool();
}

void CImageEditor::OnSelectColor() 
{
	m_tool = ID_SELECT_COLOR;
	ChangeTool();
}

void CImageEditor::OnZoom() 
{
	m_tool = ID_ZOOM;
	ChangeTool();
}

void CImageEditor::OnPick() 
{
	m_tool = ID_PICK;
	ChangeTool();
}

void CImageEditor::OnPen() 
{
	m_tool = ID_PEN;
	ChangeTool();
}

void CImageEditor::OnBrush() 
{
	m_tool = ID_BRUSH;
	ChangeTool();
}

void CImageEditor::OnCustomBrush() 
{
	m_tool = ID_CUSTOMBRUSH;
	ChangeTool();
}

void CImageEditor::OnTube() 
{
	m_tool = ID_TUBE;
	ChangeTool();
}

void CImageEditor::OnErasor()
{
	m_tool = ID_ERASOR;
	ChangeTool();
}

void CImageEditor::OnText() 
{
	m_tool = ID_TEXT;
	ChangeTool();
}

void CImageEditor::OnFill() 
{
	m_tool = ID_FILL;
	ChangeTool();
}

void CImageEditor::OnArrow() 
{
	m_tool = ID_ARROW;
	ChangeTool();
}

void CImageEditor::OnLine() 
{
	m_tool = ID_LINE;
	ChangeTool();
}

void CImageEditor::OnRectangle() 
{
	m_tool = ID_RECTANGLE;
	ChangeTool();
}

void CImageEditor::OnPolygon() 
{
	m_tool = ID_POLYGON;
	ChangeTool();
}

void CImageEditor::OnEllipse() 
{
	m_tool = ID_ELLIPSE;
	ChangeTool();
	
}

void CImageEditor::OnHotspot() 
{
	m_tool = ID_HOTSPOT;
	ChangeTool();
}

void CImageEditor::OnActionpoint() 
{
	m_tool = ID_ACTIONPOINT;
	ChangeTool();
}



///////////////////////////
//  Main toolbar




void CImageEditor::OnResize()
{
	m_PicEd.m_modded = true;
	CRect rect;
	this->GetClientRect(&rect);
	CPicEdResizeDlg MyCanvas;
	MyCanvas.m_pPicEd = &m_PicEd;

	m_PicEd.DropSelection();
	if(MyCanvas.DoModal()== IDOK)
	{

		m_PicEd.m_Width = m_PicEd.m_CanvasWidth = MyCanvas.m_width;
		m_PicEd.m_Height = m_PicEd.m_CanvasHeight = MyCanvas.m_height;


		for(int a = 0;a < m_PicEd.layers.size();a ++)
		{
			m_PicEd.display->ResizeTextureRT(
				m_PicEd.layers.at(a),  MyCanvas.m_width,MyCanvas.m_height
				);
		}

		m_PicEd.display->SetRenderTarget(-1);
		m_PicEd.display->SetTexture(-1);

		m_PicEd.ClearTemp();

		m_PicEd.CreateUndo();
	}
}

void CImageEditor::OnEditPaste()
{
	m_PicEd.OnEditPaste();
}

void CImageEditor::OnFileOpen(CString path)
{
	CxImage MyImage;
	MyImage.Load(path);

	CD3DDisplay& Display = *(m_PicEd.display);

	int newTextureHandle = -1;
	int OldTexture = -1;

	UINT oldwidth, oldheight;
	UINT width = oldwidth = MyImage.GetWidth();
	UINT height = oldheight = MyImage.GetHeight();
	UINT miplevels = 0;
	D3DFORMAT fmt = D3DFMT_A8R8G8B8;

	// Check image is valid size.
	D3DXCheckTextureRequirements(Display.GetD3DDevice(), &width, &height, &miplevels, 0, &fmt, D3DPOOL_MANAGED);

	// Max size exceeded
	if (width < oldwidth || height < oldheight) {
		CString msg;
		msg.Format("This image is larger than your video card supports.  It is %d x %d, but will be cropped to %d x %d if you choose to load it.  Do you want to load a cropped image?",
			oldwidth, oldheight, width, height);
		if (MessageBox(msg, "Load image", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
			return;
	}

	// Loading large images can fail on some hardware.
	try 
	{
		newTextureHandle = Display.AddTextureFromFile(path);
		width = Display.TexInfo(newTextureHandle)->imgwidth;
		height = Display.TexInfo(newTextureHandle)->imgheight;
	}

	catch (...)
	{
		CErrorDlg Dlg;
		Dlg.Error("Load image", "Unable to load this image.  The format may be unsupported, or the image may be too large for your hardware.");

		return;
	}

	try
	{
		OldTexture = *m_PicEd.m_pCanvas;

		Display.SetRenderTarget(-1);
		Display.SetTexture(-1);
		*m_PicEd.m_pCanvas = newTextureHandle;
		Display.Premultiply(*m_PicEd.m_pCanvas);

		m_PicEd.m_CanvasWidth = m_PicEd.m_Width = width;
		m_PicEd.m_CanvasHeight = m_PicEd.m_Height = height;
		m_PicEd.CanvasToTemp();

		if(m_PicEd.GetHotspot())
		{
			m_PicEd.GetHotspot()->x = width / 2;
			m_PicEd.GetHotspot()->y = height / 2;
		}
	
		m_PicEd.ClearTemp();
		m_PicEd.CreateUndo("Load");
	}

	catch (...)
	{
		CErrorDlg Dlg;
		Dlg.Error("Load image", "Unable to load this image.  The format may be unsupported, or the image may be too large for your hardware.");

		// Clean up
		Display.RemoveTexture(newTextureHandle);

		Display.SetRenderTarget(-1);
		Display.SetTexture(-1);
		*m_PicEd.m_pCanvas = OldTexture;
		Display.Premultiply(*m_PicEd.m_pCanvas);

		return;
	}

	// All went OK, remove the old texture
	Display.RemoveTexture(OldTexture);
}

void CImageEditor::OnUndo()
{
	m_PicEd.Undo();
	Invalidate();
}
void CImageEditor::OnRedo()
{
	m_PicEd.Redo();
	Invalidate();
}
void CImageEditor::OnMirror()
{
	m_PicEd.Mirror();

	CxImage* other = m_pImgEdDlg->getOtherImageForThisFrame();
	if(other) other->Mirror();

	Invalidate();
}

void CImageEditor::OnFlip()
{
	m_PicEd.Flip();

	CxImage* other = m_pImgEdDlg->getOtherImageForThisFrame();
	if(other) other->Flip();

	Invalidate();
}

void CImageEditor::OnRotate()
{
	if (m_RotateDlg.m_hWnd)
		m_RotateDlg.DestroyWindow();

	CPoint Pt;
	::GetCursorPos(&Pt);

	m_RotateDlg.m_pParent = this;
	m_RotateDlg.Create(IDD_POPUPROTATE, this);
	m_RotateDlg.SetWindowPos(NULL, Pt.x, Pt.y, -1, -1, SWP_NOSIZE);
	m_RotateDlg.ShowWindow(SW_SHOW);
}

void CImageEditor::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_MouseX = point.x;
	m_MouseY = point.y;

	SetCapture();
	m_PicEd.OnRMouseDown(MakePOINTS(point));
	CScrollView::OnRButtonDown(nFlags, point);
	
	UpdateCursor();
	Invalidate();
}




BOOL CImageEditor::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	SetRedraw(false);
	// I am pasting code from the frame editor so shuttup...m_Zoom is a member variable...indirectly :P
	float &m_Zoom = m_PicEd.m_Zoom;

	ScreenToClient(&pt);



	CRect Clientrect;
	GetClientRect(Clientrect);
	// TODO: Add your message handler code here and/or call default
	float oldzoom = m_Zoom;
	m_zoom_val += zDelta;

	if(m_zoom_val >= 0 )
		m_Zoom = (m_zoom_val/120.0)+1;
	if(m_zoom_val < 0 ) 
		m_Zoom = 1.0/(1-(m_zoom_val/120.0));

	CPoint centerScrollPosition = GetScrollPosition();
	CRect ClientRect(0,0,0,0);
	GetClientRect(&ClientRect);
	centerScrollPosition.x += ClientRect.right/2;
	centerScrollPosition.y += ClientRect.bottom/2;

	centerScrollPosition.x += (pt.x  -ClientRect.right/2)/2;
	centerScrollPosition.y += (pt.y  -ClientRect.bottom/2)/2;



	int newXScrollPosition = centerScrollPosition.x * m_Zoom / oldzoom;
	int newYScrollPosition = centerScrollPosition.y * m_Zoom / oldzoom;

	newXScrollPosition -= ClientRect.right/2;
	newYScrollPosition -= ClientRect.bottom/2;

	

	UpdateScrollbars();
	ScrollToPosition(CPoint(newXScrollPosition,newYScrollPosition));
	UpdateScrollbars();

	m_pImgEdDlg->OnSize(0,0,0);

	
	SetRedraw(true);
	RedrawWindow();

	return TRUE;//CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CImageEditor::OnRButtonUp(UINT nHitTest, CPoint point) 
{
	m_MouseX = point.x;
	m_MouseY = point.y;
	
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	m_PicEd.OnRMouseUp(MakePOINTS(point));
	Invalidate();
	UpdateCursor();
	CScrollView::OnRButtonUp(nHitTest, point);
}

void CImageEditor::UpdateTool()
{
//This function is called if the colour changes...or the brush size maybe ? hmm


	float alpha1 = m_PicEd.m_Props.Alpha1;

	// This is a hack sadly
	if(this->m_pImgEdDlg->m_Tool_Settings.m_Flow.IsWindowVisible())
		alpha1 = m_PicEd.m_Props.Flow;

	m_PicEd.Colour1 = D3DCOLOR_RGBA((int)(GetRValue(*m_pColor_Mixer->pColor1)),
									(int)(GetGValue(*m_pColor_Mixer->pColor1)),
									(int)(GetBValue(*m_pColor_Mixer->pColor1)),
									(int)(alpha1));

	float alpha2 = m_PicEd.m_Props.Alpha2;

	// This is a hack sadly
	if(this->m_pImgEdDlg->m_Tool_Settings.m_Flow.IsWindowVisible())
		alpha2 = m_PicEd.m_Props.Flow;
	m_PicEd.Colour2 = D3DCOLOR_RGBA((int)(GetRValue(*m_pColor_Mixer->pColor2)),
									(int)(GetGValue(*m_pColor_Mixer->pColor2)),
									(int)(GetBValue(*m_pColor_Mixer->pColor2)),
									(int)(alpha2));

	m_PicEd.CurrentTool->Modify();

	m_pPaletteBar->m_wndChild.Palette[m_pPaletteBar->m_wndChild.color1] = *m_pPaletteBar->m_wndChild.pColor1;
	m_pPaletteBar->m_wndChild.Palette[m_pPaletteBar->m_wndChild.color2] = *m_pPaletteBar->m_wndChild.pColor2;
	m_pPaletteBar->m_wndChild.Invalidate(false);
	m_pColor_Mixer->UpdateEditBoxs();
	m_pColor_Mixer->Invalidate(false);
}

void CImageEditor::UpdateScrollbars()
{
	SetScrollSizes(MM_TEXT,
		CSize(m_PicEd.m_Width*m_PicEd.m_Zoom, m_PicEd.m_Height*m_PicEd.m_Zoom)
		);
}

void CImageEditor::EditImage(CxImage* MyImage, CxImage* MyCollision)
{
//	BEGINTIMER

	// Initial stuff
	CString text;
	text.Format("%d", (int)m_PicEd.m_Props.Angle);
	m_pTool_Settings->m_BrushAngle.m_Edit.SetWindowText(text);
	m_pTool_Settings->m_BrushAngle.m_Edit.SetModify(false);

	text.Format("%d", (int)m_PicEd.m_Props.Hardness*100);
	m_pTool_Settings->m_BrushHardness.m_Edit.SetWindowText(text);
	m_pTool_Settings->m_BrushHardness.m_Edit.SetModify(false);

	text.Format("%d", (int)m_PicEd.m_Props.Size);
	m_pTool_Settings->m_BrushSize.m_Edit.SetWindowText(text);
	m_pTool_Settings->m_BrushSize.m_Edit.SetModify(false);

	text.Format("%d", (int)m_PicEd.m_Props.Step);
	m_pTool_Settings->m_BrushStep.m_Edit.SetWindowText(text);
	m_pTool_Settings->m_BrushStep.m_Edit.SetModify(false);

	text.Format("%d", (int)m_PicEd.m_Props.Thickness);
	m_pTool_Settings->m_BrushThickness.m_Edit.SetWindowText(text);
	m_pTool_Settings->m_BrushThickness.m_Edit.SetModify(false);

	text.Format("%d", (int)m_PicEd.m_Props.Opacity);
	m_pTool_Settings->m_Opacity.m_Edit.SetWindowText(text);
	m_pTool_Settings->m_Opacity.m_Edit.SetModify(false);

	text.Format("%d", (int)m_PicEd.m_Props.Flow);
	m_pTool_Settings->m_Flow.m_Edit.SetWindowText(text);
	m_pTool_Settings->m_Flow.m_Edit.SetModify(false);

	text.Format("%d", (int)m_PicEd.m_Props.Alpha1);
	m_pTool_Settings->m_Alpha1.m_Edit.SetWindowText(text);
	m_pTool_Settings->m_Alpha1.m_Edit.SetModify(false);

	text.Format("%d", (int)m_PicEd.m_Props.Alpha2);
	m_pTool_Settings->m_Alpha2.m_Edit.SetWindowText(text);
	m_pTool_Settings->m_Alpha2.m_Edit.SetModify(false);



	m_PicEd.Initialise(m_display,MyImage);


//	ENDTIMER
}



void CImageEditor::OnEnterIdle(UINT nWhy, CWnd* pWho) 
{

}

void CImageEditor::OnIdleUpdateCmdUI()
{

}

void CImageEditor::OnNew()
{
	 m_PicEd.OnEditNew();
	 Invalidate();
}

 void CImageEditor::OnCut()
 {
	 m_PicEd.OnEditCut();
	 Invalidate();
 }
 void CImageEditor::OnCopy()
 {
	 m_PicEd.OnEditCopy();
 }
 void CImageEditor::OnPaste()
 {
	 m_PicEd.OnEditPaste();
	 Invalidate();
 }
 void CImageEditor::OnEditSelectAll()
 {
	 m_PicEd.OnEditSelectAll();
	 Invalidate();
 }
 void CImageEditor::OnWrap()
 {
	m_PicEd.bSeamless = !m_PicEd.bSeamless;
	Invalidate();
 }

 void CImageEditor::OnGrid()
 {
	m_PicEd.grid = !m_PicEd.grid;
	Invalidate();
 }

 void CImageEditor::OnMask()
 {
	if( GetKeyState(VK_SHIFT) >> 4 )
	{
		m_pImgEdDlg->AskToDeleteCollisionMask();
	}
	else
	{
		m_pImgEdDlg->SaveImageInternally(true);
		m_pImgEdDlg->m_editCollision = !m_pImgEdDlg->m_editCollision;
		m_pImgEdDlg->LoadImageInternally();
		m_PicEd.SetEditCollision( m_pImgEdDlg->m_editCollision );
	}

 }

 void CImageEditor::OnCrop()
 {
	 m_PicEd.OnEditCrop();
	 UpdateTool();
	 Invalidate();
 }

 void CImageEditor::OnBackground()
 {
	m_PicEd.BackInvert = !m_PicEd.BackInvert;
	Invalidate();
 }

void CImageEditor::UpdateCursor()
{
//	HCURSOR cursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS));
/*	switch( m_tool )
	{
	case ID_PEN:
		{
			cursor = LoadCursor(::AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_EXT_IED_TOOL_PENCIL));
		}
		break;
	case ID_BRUSH:
		{
			cursor = LoadCursor(::AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_EXT_IED_TOOL_BRUSH));
		}
		break;
	case ID_FILL:
		{
			cursor = LoadCursor(::AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_EXT_IED_TOOL_FILL));
		}
		break;
	}*/
	// I added some cursors but when I use their IDs ID_PICED_BRUSH etc
	// they come out as icons used in profui...but when i use the cursors
	// in profui they dont appear

/*SetClassLong(m_hWnd,
		      GCL_HCURSOR,
	                     (LONG)LoadCursor(NULL,
  				 IDC_CROSS));*/

}


BOOL CImageEditor::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HCURSOR cursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS));
	::SetCursor(cursor);

	return TRUE;
}











/////////////////////////////////////////////////////////////////////////////
// CPalette Bar

CPaletteBar::CPaletteBar()
{
}

CPaletteBar::~CPaletteBar()
{
}


BEGIN_MESSAGE_MAP(CPaletteBar, CExtControlBar)
	//{{AFX_MSG_MAP(CMyBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyBar message handlers

int CPaletteBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CExtControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}



