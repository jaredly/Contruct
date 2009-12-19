// ImageEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\MainFrm.h"
extern CMainFrame* pMainWnd;
#include <afxpriv.h>

#include "..\Utilities\FileDialogEx.H"

/////////////////////////////////////////////////////////////////////////////
// CImageEditorDlg dialog


CImageEditorDlg::CImageEditorDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CImageEditorDlg::IDD, pParent)
{
	m_pFrameWnd = 0;
	m_pXDVView = 0;
	m_pMyImage = 0;
	m_Animation = false;
	m_animation_frame_index = 0;
	m_result = 0;

	m_Icon = false;

	m_editCollision = false;
	application = NULL;

	m_noAlpha = false;

	//{{AFX_DATA_INIT(CImageEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CImageEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CExtResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageEditorDlg)
	DDX_Control(pDX, IDC_MAIN_MENUBAR, m_Main); 
	DDX_Control(pDX, IDC_TOOLS, m_Tools); 
	DDX_Control(pDX, IDC_MIXERBAR, m_Color_Mixer_bar);
	DDX_Control(pDX, IDC_OPTIONS,m_Tool_Settings);
	DDX_Control(pDX, IDC_PALETTE,m_PaletteBar);
	DDX_Control(pDX, IDC_ANIMATION, m_Tool_Animation);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImageEditorDlg, CExtResizableDialog)
	//{{AFX_MSG_MAP(CImageEditorDlg)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()


	ON_COMMAND(ID_FILE_OPEN,OnFileOpen)
	ON_COMMAND(ID_SAVE, OnFileSave)

	// tools
	ON_COMMAND(ID_SELECT_FREE, nothing)
	ON_COMMAND(ID_SELECT_POLYGON, nothing)
	ON_COMMAND(ID_SELECT_RECTANGLE, nothing)
	ON_COMMAND(ID_SELECT_ELLIPSE, nothing)
	ON_COMMAND(ID_SELECT_WAND, nothing)	
	ON_COMMAND(ID_SELECT_COLOR, nothing)
	ON_COMMAND(ID_ZOOM, nothing)
	ON_COMMAND(ID_PICK, nothing)
	ON_COMMAND(ID_PEN, nothing)
	ON_COMMAND(ID_BRUSH, nothing)
	//ON_COMMAND(ID_CUSTOMBRUSH,nothing)
	//ON_COMMAND(ID_TUBE, nothing)
	ON_COMMAND(ID_ERASOR, nothing)
	//ON_COMMAND(ID_TEXT, nothing)
	ON_COMMAND(ID_FILL, nothing)
	ON_COMMAND(ID_ARROW, nothing)
	ON_COMMAND(ID_LINE, nothing)
	ON_COMMAND(ID_RECTANGLE, nothing)
	ON_COMMAND(ID_POLYGON, nothing)
	ON_COMMAND(ID_ELLIPSE, nothing)
	ON_COMMAND(ID_HOTSPOT, nothing)
	ON_COMMAND(ID_ACTIONPOINT, nothing)
	ON_COMMAND(ID_BACKGROUNDCOLOR, nothing)
	ON_COMMAND(ID_WRAP, nothing)
	ON_COMMAND(ID_MASK, nothing)
	// main toolbar
	ON_COMMAND(ID_RESIZE,nothing)
	ON_COMMAND(ID_UNDO,nothing)
	ON_COMMAND(ID_REDO,nothing)
	ON_COMMAND(ID_MIRROR,nothing)
	ON_COMMAND(ID_FLIP,nothing)

	ON_COMMAND(ID_NEW, nothing)
	//ON_COMMAND(ID_CUT, nothing)
	ON_COMMAND(ID_COPY, nothing)
	ON_COMMAND(ID_PASTE, nothing)
	ON_COMMAND(ID_CROP, nothing)

	ON_COMMAND(ID_ROTATE,nothing)

	ON_COMMAND(128, nothing)
	ON_COMMAND(130, nothing)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageEditorDlg message handlers


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};



BOOL CImageEditorDlg::OnInitDialog() 
{
	SetRedraw(false);

	m_haccel=LoadAccelerators(AfxGetInstanceHandle(), 
        MAKEINTRESOURCE(IDR_MAINFRAME));

	CExtResizableDialog::OnInitDialog();
	

	// Create image editor window
	CRect rect(0,0,400,400);

    CRuntimeClass* pClass = RUNTIME_CLASS(CFrameWnd);
    m_pFrameWnd = (CFrameWnd*)pClass->CreateObject();
    m_pFrameWnd->Create(NULL, NULL, WS_CHILDWINDOW, rect, this);
    m_pFrameWnd->ShowWindow(SW_SHOW);

	m_pXDVView = new CImageEditor;
	m_pXDVView->m_display = application->m_display;
    m_pXDVView->SetScrollSizes(MM_TEXT, rect.Size());


	// Create tool properties
	ToolProperties& Props = m_pXDVView->m_PicEd.m_Props;

	Props.Hardness =		pMainWnd->m_INI.GetDouble("ImageEditor", "Hardness", 1);
	Props.Angle =		pMainWnd->m_INI.GetDouble("ImageEditor", "Angle", 0);
	Props.Thickness =		pMainWnd->m_INI.GetDouble("ImageEditor", "Thickness", 100);
	Props.Size =		pMainWnd->m_INI.GetDouble("ImageEditor", "Size", 1);
	Props.Step = 		pMainWnd->m_INI.GetDouble("ImageEditor", "Step", 30);
	Props.LineThickness =		pMainWnd->m_INI.GetDouble("ImageEditor", "LineThickness",1);
	Props.bSmooth =		pMainWnd->m_INI.GetBool("ImageEditor", "Smooth",0);
	Props.Opacity =		pMainWnd->m_INI.GetDouble("ImageEditor", "Opacity",255);
	Props.Flow =		pMainWnd->m_INI.GetDouble("ImageEditor", "Flow",255);
	Props.Alpha1 =		pMainWnd->m_INI.GetDouble("ImageEditor", "Alpha1",255);
	Props.Alpha2 =		pMainWnd->m_INI.GetDouble("ImageEditor", "Alpha2",255);

	Props.src1 = NULL;
	Props.src2 = NULL;
	Props.Brush1 = -2;
	Props.Brush2 = -2;
	Props.Style = 0;

	// Initialize image editor 
	m_pXDVView->m_app = application;

    m_pXDVView->Create(NULL, NULL, WS_CHILDWINDOW,
        rect, m_pFrameWnd, AFX_IDW_PANE_FIRST+1, NULL);
    m_pXDVView->ShowWindow(SW_SHOW);
	
	CImageEditorFrame* pMainFrame = (CImageEditorFrame*)m_pFrameWnd;

	// Copy the hotspot and stuff over from the input
	m_pXDVView->m_PicEd.m_pImageEditor = m_pXDVView;
	m_pXDVView->m_pImgEdDlg = this;

	// Load toolbar
	m_Main.ModifyStyle(0, CBRS_TOOLTIPS);
	if (!m_Main.LoadToolBar(IDR_PIC_MAIN))
			MessageBox("Unable to make the main toolbar");

	// Tool Settings
	{

		m_Tool_Settings.pTool = &m_pXDVView->m_tool;
		m_Tool_Settings.m_pImgEd = m_pXDVView;
		

		m_Tool_Settings.SetButtons(0,1);
		m_Tool_Settings.SetWindowText("Tool Settings");
		m_Tool_Settings.ShowWindow(true);
	
		// Brush Size
		m_Tool_Settings.m_BrushSize.Create(WS_CHILD|WS_CLIPSIBLINGS | WS_TABSTOP|ES_MULTILINE , CRect(0,0,52,32), &m_Tool_Settings, 121);
		m_Tool_Settings.m_BrushSize.m_type = ID_BRUSHSIZE;
		m_Tool_Settings.m_BrushSize.pPicEd = &m_pXDVView->m_PicEd;
		m_Tool_Settings.m_BrushSize.m_Edit.m_Max = 128;
		m_Tool_Settings.m_LineThickness.m_Edit.m_Min = 1;

		// Brush Thickness
		m_Tool_Settings.m_BrushThickness.Create(WS_CHILD|WS_CLIPSIBLINGS | WS_TABSTOP|ES_MULTILINE , CRect(0,0,52,32), &m_Tool_Settings, 122);
		m_Tool_Settings.m_BrushThickness.m_type = ID_BRUSHTHICKNESS;
		m_Tool_Settings.m_BrushThickness.pPicEd = &m_pXDVView->m_PicEd;
		m_Tool_Settings.m_BrushThickness.m_Edit.m_Max = 100;
		m_Tool_Settings.m_LineThickness.m_Edit.m_Min = 0;

		// Brush Hardness
		m_Tool_Settings.m_BrushHardness.Create(WS_CHILD|WS_CLIPSIBLINGS | WS_TABSTOP|ES_MULTILINE , CRect(0,0,52,32), &m_Tool_Settings, 123);
		m_Tool_Settings.m_BrushHardness.m_type = ID_BRUSHHARDNESS;
		m_Tool_Settings.m_BrushHardness.pPicEd = &m_pXDVView->m_PicEd;
		m_Tool_Settings.m_BrushHardness.m_Edit.m_Max = 100;
		m_Tool_Settings.m_LineThickness.m_Edit.m_Min = 0;

		// Brush Angle
		m_Tool_Settings.m_BrushAngle.Create(WS_CHILD|WS_CLIPSIBLINGS | WS_TABSTOP|ES_MULTILINE , CRect(0,0,52,32), &m_Tool_Settings, 124);
		m_Tool_Settings.m_BrushAngle.m_type = ID_BRUSHANGLE;
		m_Tool_Settings.m_BrushAngle.pPicEd = &m_pXDVView->m_PicEd;
		m_Tool_Settings.m_BrushAngle.m_Edit.m_Max = 360;
		m_Tool_Settings.m_LineThickness.m_Edit.m_Min = 0;

		// Brush Step
		m_Tool_Settings.m_BrushStep.Create(WS_CHILD|WS_CLIPSIBLINGS | WS_TABSTOP|ES_MULTILINE , CRect(0,0,52,32), &m_Tool_Settings, 125);
		m_Tool_Settings.m_BrushStep.m_type = ID_BRUSHSTEP;
		m_Tool_Settings.m_BrushStep.pPicEd = &m_pXDVView->m_PicEd;
		m_Tool_Settings.m_BrushStep.m_Edit.m_Max = 100;

		// Outline / Fill
		m_Tool_Settings.m_OutlineFill.Create(WS_CHILD|WS_CLIPSIBLINGS | WS_TABSTOP|ES_MULTILINE , CRect(0,0,65,32), &m_Tool_Settings, 126);
		m_Tool_Settings.m_OutlineFill.pPicEd = &m_pXDVView->m_PicEd;

		// Actionpoints
		m_Tool_Settings.m_ActionPointCombo.Create(CBS_DROPDOWN |WS_CHILD, CRect(120,16,150 + 120,400 + 16), &m_Tool_Settings, 127);
		m_Tool_Settings.m_ActionPointCombo.SetFont( this->GetFont());
		m_Tool_Settings.m_ActionPointAdd.Create("+",WS_CHILD, CRect(150 + 120,16,32+150 + 120, 20+16), &m_Tool_Settings, 128);
		m_Tool_Settings.m_ActionPointAdd.SetFont( this->GetFont());
		

		// Line Thickness
		m_Tool_Settings.m_LineThickness.Create(WS_CHILD|WS_CLIPSIBLINGS | WS_TABSTOP|ES_MULTILINE , CRect(0,0,52,32), &m_Tool_Settings, 129);
		m_Tool_Settings.m_LineThickness.m_type = ID_LINETHICKNESS;
		m_Tool_Settings.m_LineThickness.pPicEd = &m_pXDVView->m_PicEd;
		m_Tool_Settings.m_LineThickness.m_Edit.m_Min = 1;
		m_Tool_Settings.m_LineThickness.m_Edit.m_Max = 99;

		// Smooth
		m_Tool_Settings.m_Smooth.Create("Smooth",WS_CHILD, CRect(150,0,64+150, 43), &m_Tool_Settings, 130);
		m_Tool_Settings.m_Smooth.SetFont( this->GetFont());
		m_Tool_Settings.m_Smooth.SetCheck(Props.bSmooth);

		// Opacity
		m_Tool_Settings.m_Opacity.Create(WS_CHILD|WS_CLIPSIBLINGS | WS_TABSTOP|ES_MULTILINE , CRect(0,0,52,32), &m_Tool_Settings, 131);
		m_Tool_Settings.m_Opacity.m_type = ID_OPACITY;
		m_Tool_Settings.m_Opacity.pPicEd = &m_pXDVView->m_PicEd;
		m_Tool_Settings.m_Opacity.m_Edit.m_Max = 255;

		// Flow
		m_Tool_Settings.m_Flow.Create(WS_CHILD|WS_CLIPSIBLINGS | WS_TABSTOP|ES_MULTILINE , CRect(0,0,52,32), &m_Tool_Settings, 131);
		m_Tool_Settings.m_Flow.m_type = ID_FLOW;
		m_Tool_Settings.m_Flow.pPicEd = &m_pXDVView->m_PicEd;
		m_Tool_Settings.m_Flow.m_Edit.m_Max = 255;

		// Alpha 1
		m_Tool_Settings.m_Alpha1.Create(WS_CHILD|WS_CLIPSIBLINGS | WS_TABSTOP|ES_MULTILINE , CRect(0,0,52,32), &m_Tool_Settings, 131);
		m_Tool_Settings.m_Alpha1.m_type = ID_ALPHA1;
		m_Tool_Settings.m_Alpha1.pPicEd = &m_pXDVView->m_PicEd;
		m_Tool_Settings.m_Alpha1.m_Edit.m_Max = 255;

		// Alpha 2
		m_Tool_Settings.m_Alpha2.Create(WS_CHILD|WS_CLIPSIBLINGS | WS_TABSTOP|ES_MULTILINE , CRect(0,0,52,32), &m_Tool_Settings, 131);
		m_Tool_Settings.m_Alpha2.m_type = ID_ALPHA2;
		m_Tool_Settings.m_Alpha2.pPicEd = &m_pXDVView->m_PicEd;
		m_Tool_Settings.m_Alpha2.m_Edit.m_Max = 255;

		// PosX
		m_Tool_Settings.m_posX.Create(WS_CHILD|WS_CLIPSIBLINGS | WS_TABSTOP|ES_MULTILINE , CRect(0,0,52,32), &m_Tool_Settings, 131);
		m_Tool_Settings.m_posX.m_type = ID_POSX;
		m_Tool_Settings.m_posX.pPicEd = &m_pXDVView->m_PicEd;

		// PosY
		m_Tool_Settings.m_posY.Create(WS_CHILD|WS_CLIPSIBLINGS | WS_TABSTOP|ES_MULTILINE , CRect(0,0,52,32), &m_Tool_Settings, 131);
		m_Tool_Settings.m_posY.m_type = ID_POSY;
		m_Tool_Settings.m_posY.pPicEd = &m_pXDVView->m_PicEd;

	}


	if(m_Animation)
	{
		m_Tool_Animation.m_pImgEd = m_pXDVView;



		for(int i = 0; i < m_sourceImages.size(); i++)
		{
			m_Tool_Animation.UpdateThumbnail(i);
		}
	}
	else
	{
		m_Tool_Animation.DestroyWindow();
	}

	// Tools 

	if (//!m_Tools.Create("Tools",pMainFrame) ||
		!m_Tools.LoadToolBar(IDR_PIC_TOOLS))
		MessageBox("Unable to make the tools toolbar");
	m_Tools.SetWindowText("Image Tools");



	m_pStatusBar = new CExtStatusControlBar;
	m_pStatusBar->Create(this);
	m_pStatusBar->AddPane(0, 0);
	m_pStatusBar->SetPaneWidth(0, 70);
	m_pStatusBar->AddPane(1, 1);
	m_pStatusBar->SetPaneWidth(1, 70);
	m_pStatusBar->AddPane(2, 2);
	m_pStatusBar->SetPaneWidth(2, 220);

	// Colour Mixer 

	m_Color_Mixer.Create(IDD_COLORMIXER, &m_Color_Mixer_bar);

	//m_Color_Mixer.Create(IDD_COLORMIXER, this);
	m_Color_Mixer.ShowWindow(true);


	m_Color_Mixer.UpdateData(FALSE);
	m_Color_Mixer.SetWindowText("Colour Mixer");
	m_Color_Mixer.m_pImageView = m_pXDVView;

	m_Color_Mixer.pColor1 = new COLORREF; 
	m_Color_Mixer.pColor2 = new COLORREF;
	m_Color_Mixer.pColor = m_Color_Mixer.pColor1;


	// Initial Colours
	*m_Color_Mixer.pColor1 = g_MainFrame->m_INI.GetInt("ImageEditor", "Colour1", 10);
	*m_Color_Mixer.pColor2 = g_MainFrame->m_INI.GetInt("ImageEditor", "Colour2", 10);

	m_Color_Mixer.FindGradPos();
	m_Color_Mixer.FindRainbowPos();
	m_Color_Mixer.UpdateEditBoxs();



	m_Color_Mixer_bar.ShowWindow(SW_SHOW);


	/* Palette Bar */
	/*if(!m_PaletteBar.Create("Colour palette", this->GetParentOwner(),AFX_IDW_TOOLBAR+2))
	{
		MessageBox("Unable to make the palette bar");
	}*/
//	m_PaletteBar.SetBarStyle(  CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC|CBRS_ALIGN_BOTTOM);
 

	int style = //WS_CHILD|WS_VISIBLE
				//	|CBRS_TOP|CBRS_TOOLTIPS
				//	|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
0x50002034 &~CBRS_TOP   |  CBRS_RIGHT
					;



	m_PaletteBar.m_wndChild.m_pImageView = m_pXDVView;
 	m_PaletteBar.m_wndChild.pColor1 = m_Color_Mixer.pColor1;
	m_PaletteBar.m_wndChild.pColor2 = m_Color_Mixer.pColor2;
	
	if (!m_PaletteBar.m_wndChild.Create(m_PaletteBar.m_wndChild.IDD,&m_PaletteBar))
		return -1;


	m_PaletteBar.m_wndChild.ShowWindow(SW_SHOW);
	// end create controls



	// give pointers to the control in the view







	m_pXDVView->m_pTool_Settings = &m_Tool_Settings;
	m_pXDVView->m_pTools = &m_Tools;
	m_pXDVView->m_pPaletteBar = &m_PaletteBar;
	m_pXDVView->m_pMain = &m_Main;
	m_pXDVView->m_pColor_Mixer = &m_Color_Mixer;






	








	try {

		m_pXDVView->EditImage(m_pMyImage);

	}
	catch (CD3DError e) {
		
		m_pXDVView->drawingDisabled = true;	// Prevent crash when messagebox comes up
		CString msg("An error was encountered in the display engine while opening the picture editor.  The message is:\n\n");
		msg += e.GetString();
		this->MessageBox(msg, "Display error", MB_ICONHAND);
		EndDialog(0);
		return FALSE;
	}

	


	CRect rcClientNow;
	SetWindowLong(m_pFrameWnd->GetSafeHwnd(), GWL_ID,12345);



	// Size code
	{
	SetRedraw(false);

	RepositionBars(0,0xFFFF,12345); 

	CRect rcWindow;

	if(m_pFrameWnd)m_pFrameWnd->GetWindowRect(rcWindow);
	if(m_pXDVView)  m_pXDVView->MoveWindow(0,0,rcWindow.Width()-4,rcWindow.Height()-4,FALSE);
		
	SetRedraw(true);
	Invalidate(false);
	}
	// end size code
		
	m_pXDVView->m_tool = ID_BRUSH;
	m_pXDVView->ChangeTool();
	m_pXDVView->UpdateTool();

	m_pXDVView->m_tool = pMainWnd->m_INI.GetInt("ImageEditor", "Tool",ID_BRUSH);
	m_pXDVView->ChangeTool();
	m_pXDVView->UpdateTool();

	m_pXDVView->m_oldtool = m_pXDVView->m_tool;

	
	

	m_pXDVView->SetFocus();

	SetRedraw(true);
	RedrawWindow();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CImageEditorDlg::AnimLeft()
{
	if(m_Animation)
	{
		m_Tool_Animation.AdvanceFrames(-1);
	}
}

void CImageEditorDlg::AnimRight()
{
	if(m_Animation)
	{
		m_Tool_Animation.AdvanceFrames(1);
	}
}


void CImageEditorDlg::SaveImageInternally(bool updateThumbnails)
{
	CPictureEditor& m_PicEd = m_pXDVView->m_PicEd;
	if(m_PicEd.m_modded)
	{
		if(m_sourceImages.size() == 0)
			m_PicEd.Save(m_pMyImage);
		else if(m_editCollision)
			m_PicEd.Save(&m_newCollision[ m_sourceImages.at(m_animation_frame_index) ] );
		else
			m_PicEd.Save(&m_newImages[ m_sourceImages.at(m_animation_frame_index) ] );
	}
	if(m_Animation)
		m_Tool_Animation.UpdateThumbnail(m_animation_frame_index);
}

void CImageEditorDlg::SetNoAlpha( bool val )
{
	m_noAlpha = val;
}


void CImageEditorDlg::LoadImageInternally()
{
	CPictureEditor* pEd = &m_pXDVView->m_PicEd;

	pEd->m_modded = false; // when you go to a new internal image it hasn't been edited until you do something

	if(m_sourceImages.size() == 0) // using the image editor to edit a single image
	{
		// Dont change m_pMyImage
	}
	else // using the image editor to edit an animation or an image resource.
	{
		CImageResource* img_res = m_sourceImages.at(m_animation_frame_index);

		m_pMyImage = NULL;

		if(m_editCollision) 
		{	
			if(m_newCollision.find(img_res) != m_newCollision.end())
			{
				if(m_newCollision[img_res].IsValid())
					m_pMyImage = &m_newCollision[img_res];
			}
			else
				if(img_res->m_Collision.IsValid())
					m_pMyImage = &img_res->m_Collision;
		}

		// Note: If we are editing a collision mask, but there isn't currently a collision mask, we load
		// the CxImage of the bitmap.
		
		if(!m_pMyImage)
		{
			if(m_newImages.find(img_res) != m_newImages.end())
				m_pMyImage = &m_newImages[img_res];
			else
				m_pMyImage = &img_res->bitmap;
		}
	}

	
	// we need to free each layer.
	for(int a = 0; a < pEd->layers.size(); a++)
	{
		pEd->display->RemoveTexture(pEd->layers.at(a));
	}
	pEd->layers.clear();

	// Now its time to load up the layers in the new image we are editing
	pEd->m_CanvasWidth = pEd->m_Width = m_pMyImage->GetWidth();
	pEd->m_CanvasHeight = pEd->m_Height = m_pMyImage->GetHeight();

	pEd->layers.push_back(pEd->display->AddTextureRT(pEd->m_Width,pEd->m_Height));
	pEd->m_pCanvas = &pEd->layers.at(0);
	pEd->m_pLayer = pEd->m_pCanvas;


	pEd->display->SetRenderTarget(*pEd->m_pCanvas);
	pEd->display->SetTexture(-1);


	// Copy the image onto the canvas
	int m_CanvasBG = pEd->display->AddTextureFromCxImage(*m_pMyImage);

	pEd->SaveDisplayState();
	pEd->display->SetTexture(m_CanvasBG);
	pEd->display->ClearRT();
	pEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	pEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);
	pEd->display->Blit(0,0);
	pEd->RestoreDisplayState();

	pEd->display->RemoveTexture(m_CanvasBG);

	// Set stuff
	pEd->m_RectTempToCanvas.SetRect(0,0,pEd->m_Width,pEd->m_Height);
	pEd->CanvasToTemp();

	pEd->m_pImageEditor->ChangeTool();

	m_pXDVView->UpdateScrollbars();

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
	pEd->CreateUndo();
}


LRESULT CImageEditorDlg::OnKickIdle(WPARAM, LPARAM)
{
/*	CToolBar* pToolbar = &m_pXDVView->m_Tools;
	CCmdUI cmdUI;
	for(UINT n =0 ; n < 19; n ++ )
	{

	cmdUI.m_nID = pToolbar->GetItemID(n);
	cmdUI.m_pOther = pToolbar;
	cmdUI.m_nIndex = n;
	cmdUI.DoUpdate(this,FALSE);



	}

*/


//	m_pXDView->OnKickIdle();
 /*CMenu* pMainMenu = GetMenu();
 CCmdUI cmdUI;
 for (UINT n = 0; n < pMainMenu->GetMenuItemCount(); ++n)
 {
  CMenu* pSubMenu = pMainMenu->GetSubMenu(n);
  cmdUI.m_nIndexMax = pSubMenu->GetMenuItemCount();
  for (UINT i = 0; i < cmdUI.m_nIndexMax;++i)
  {
   cmdUI.m_nIndex = i;
   cmdUI.m_nID = pSubMenu->GetMenuItemID(i);
   cmdUI.m_pMenu = pSubMenu;
   cmdUI.DoUpdate(this, FALSE);
  }
 }*/
 return TRUE;
}








void CImageEditorDlg::OnSize(UINT nType, int cx, int cy) 
{
	SetRedraw(false);

	RepositionBars(0,0xFFFF,12345); 

	CRect rcWindow;

	if(m_pFrameWnd)m_pFrameWnd->GetWindowRect(rcWindow);
	if(m_pXDVView)  m_pXDVView->MoveWindow(0,0,rcWindow.Width()-4,rcWindow.Height()-4,FALSE);
		
	SetRedraw(true);
	Invalidate(false);
}


BOOL CImageEditorDlg::OnEraseBkgnd(CDC* pDC) 
{
	return CExtResizableDialog::OnEraseBkgnd(pDC);
}

void CImageEditorDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

INT_PTR CImageEditorDlg::EditImage( CxImage* MyImage, bool noAlphaChannel )
{
	m_pMyImage = MyImage;
	m_Animation = false;
	m_noAlpha = noAlphaChannel;
	DoModal();
	return m_result;
}

INT_PTR CImageEditorDlg::EditImage(CImageResource* image, CApplication* pApp)
{	
	m_animation_frame_index = 0;

	m_Animation = false;
	m_sourceImages.push_back(image);
	m_pMyImage = &image->bitmap;
	if(!m_pMyImage)
		return false; 
	CopyNeededDataFromImages();

	DoModal();

	if(m_result)
	{
		CopyNeededDataToImages();
		application->resources.images_changed = true;
		return true;
	}
	else
	{
		return false;
	}
}

INT_PTR CImageEditorDlg::EditAnimation(int animation_id, int index, CApplication* pApp)
{	
	CAnimation* anim = pApp->resources.FindAnimationFromNumber(animation_id);
	if(!anim)
		return 0;

	if(!anim->m_Images.size()) // there should be at least 1 image
		return 0;
	
	if(anim->m_Images.size() <= index) // fix wrong index's
		index = 0;
	if(index < 0)
		index = 0;

	m_animation_frame_index = index;

	m_Animation = true;
	for(int i = 0; i < anim->m_Images.size(); i++)
	{
		m_sourceImages.push_back(pApp->resources.FindImageResourceFromNumber( anim->m_Images.at(i)));
	}
	
	m_pMyImage = &this->m_sourceImages.at(index)->bitmap;
	if(!m_pMyImage)
		return 0;

	CopyNeededDataFromImages();

	// Launch
	DoModal();
	// Done

	if(m_result)
	{
		CopyNeededDataToImages();
		application->resources.images_changed = true;
		return true;
	}
	else
	{
		return false;
	}
}

void CImageEditorDlg::OnClose() 
{
	pMainWnd->m_INI.WriteInt("ImageEditor", "Colour1", *m_Color_Mixer.pColor1);
	pMainWnd->m_INI.WriteInt("ImageEditor", "Colour2", *m_Color_Mixer.pColor2);
	pMainWnd->m_INI.WriteInt("ImageEditor", "Tool", this->m_pXDVView->m_tool);

	ToolProperties& settings = m_pXDVView->m_PicEd.m_Props;	
	pMainWnd->m_INI.WriteDouble("ImageEditor", "Hardness", settings.Hardness);
	pMainWnd->m_INI.WriteDouble("ImageEditor", "Angle", settings.Angle);
	pMainWnd->m_INI.WriteDouble("ImageEditor", "Thickness", settings.Thickness);
	pMainWnd->m_INI.WriteDouble("ImageEditor", "Size",settings.Size);
	pMainWnd->m_INI.WriteDouble("ImageEditor", "Step",settings.Step);
	pMainWnd->m_INI.WriteDouble("ImageEditor", "LineThickness",settings.LineThickness);
	pMainWnd->m_INI.WriteBool("ImageEditor", "Smooth", settings.bSmooth);
	pMainWnd->m_INI.WriteDouble("ImageEditor", "Opacity", settings.Opacity);
	pMainWnd->m_INI.WriteDouble("ImageEditor", "Flow", settings.Flow);
	pMainWnd->m_INI.WriteDouble("ImageEditor", "Alpha1", settings.Alpha1);
	pMainWnd->m_INI.WriteDouble("ImageEditor", "Alpha2", settings.Alpha2);


	if(m_pXDVView->m_PicEd.m_modded)
	{		
		int result = MessageBox("Do you want to save changes to this image?","Construct",MB_YESNOCANCEL);
		if(result == IDYES)
		{
			m_result = true;

			if(m_Animation)
			{
				SaveImageInternally(false);
			}
			else
			{
				SaveImageInternally(false);


				if(m_sourceImages.size() == 0) // If theres no source image its an icon.
				{
					if(m_noAlpha) // we have requested for there to be no alpha channel so flatten it against a white background
					{
						for(int x = 0; x < m_pMyImage->GetWidth(); x++)
						{
							for( int y = 0; y < m_pMyImage->GetHeight(); y++)
							{
								RGBQUAD c = m_pMyImage->GetPixelColor(x, y, true);
								c.rgbRed += (255-c.rgbReserved);
								c.rgbGreen += (255-c.rgbReserved);
								c.rgbBlue += (255-c.rgbReserved);
								c.rgbReserved = 255;
								m_pMyImage->SetPixelColor(x,y,c,true);
							}
						}
					}
				}
			}
		}

		if(result == IDCANCEL)
			return;
	}

	CExtResizableDialog::OnClose();
}

BOOL CImageEditorDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pXDVView->OnCommand(wParam,lParam))
		return TRUE;
	return CExtResizableDialog::OnCommand(wParam, lParam);
}

void CImageEditorDlg::OnEditPaste() 
{

	//m_pXDVView->OnEditPaste();
	// TODO: Add your command handler code here
	
}

void CImageEditorDlg::OnFileSave()
{
	const char saveFilters[] =  "Windows Bitmap (*.bmp)|*.bmp|"
		"Portable Network Graphics (*.png)|*.png|"
		"JPEG (*.jpg)|*.jpg|"
		"TGA (*.tga)|*.tga|"
		"DirectDraw surface (*.dds)|*.dds|"
		"All files (*.*)|*.*||";

	CFileDialog dlg(FALSE, ".bmp", "", OFN_OVERWRITEPROMPT | OFN_ENABLESIZING, saveFilters);

	if (dlg.DoModal() == IDOK) {

		CString ext = dlg.GetPathName();
		ext = ext.Right(3);
		ext.MakeLower();

		// Default = BMP format
		D3DXIMAGE_FILEFORMAT fileformat = D3DXIFF_BMP;

		if (ext == "png")
			fileformat = D3DXIFF_PNG;
		if (ext == "jpg" || ext == "peg") // (Right(3) of .jpeg)
			fileformat = D3DXIFF_JPG;
		if (ext == "tga")
			fileformat = D3DXIFF_TGA;
		if (ext == "dds")
			fileformat = D3DXIFF_DDS;

		CPictureEditor& picEd = m_pXDVView->m_PicEd;
		D3DXSaveTextureToFile(dlg.GetPathName(), fileformat, picEd.display->GetTexture(*picEd.m_pCanvas)->texture, NULL);
	}

}

void CImageEditorDlg::OnFileOpen()
{
	try
	{
		const char openFilters[]=
		"Supported image files (*.png, *.jpg, *.bmp, *.tga, *.dds)|*.png; *.jpg; *.bmp; *.tga; *.dds|All Files (*.*)|*.*||";

		// Load selector
		CFileDialogEx dlg(TRUE, "Image Files", "", OFN_FILEMUSTEXIST | OFN_ENABLESIZING, openFilters, this, SHVIEW_THUMBNAIL);
		if (dlg.DoModal() == IDOK)
			m_pXDVView->OnFileOpen(dlg.GetPathName());
	}

	catch (CD3DError)
	{
		CErrorDlg Dlg;
		Dlg.Error("Error loading image", "There was an error loading this image. It may require more VRAM than is installed.");
	}
}

BOOL CImageEditorDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(m_haccel)
	{
		if (::TranslateAccelerator(m_hWnd, m_haccel, pMsg)) 
			return(TRUE);
    }	

	if (pMsg->message == WM_KEYDOWN)
	{
		CHotspotTool* pHSTool = dynamic_cast<CHotspotTool*>(m_pXDVView->m_PicEd.CurrentTool);
		CActionTool* pActTool = dynamic_cast<CActionTool*>(m_pXDVView->m_PicEd.CurrentTool);
		CFloatPoint hs;
		if(pHSTool)
			hs = pHSTool->hotspot;
		if(pActTool)
			hs = pActTool->hotspot;

		int w = m_pXDVView->m_PicEd.m_Width;
		int h = m_pXDVView->m_PicEd.m_Height;

		bool interestingKey = false;

		switch (pMsg->wParam) 
		{
		case VK_NUMPAD1:
			hs.x = 0;
			hs.y = h;
			interestingKey = true;
			break;
		case VK_NUMPAD2:
			hs.x = w / 2;
			hs.y = h;
			interestingKey = true;
			break;
		case VK_NUMPAD3:
			hs.x = w;
			hs.y = h;
			interestingKey = true;
			break;
		case VK_NUMPAD4:
			hs.x = 0;
			hs.y = h / 2;
			interestingKey = true;
			break;
		case VK_NUMPAD5:
			hs.x = w / 2;
			hs.y = h / 2;
			interestingKey = true;
			break;
		case VK_NUMPAD6:
			hs.x = w;
			hs.y = h / 2;
			interestingKey = true;
			break;
		case VK_NUMPAD7:
			hs.x = 0;
			hs.y = 0;
			interestingKey = true;
			break;
		case VK_NUMPAD8:
			hs.x = w / 2;
			hs.y = 0;
			interestingKey = true;
			break;
		case VK_NUMPAD9:
			hs.x = w;
			hs.y = 0;
			interestingKey = true;
			break;
		case VK_RIGHT:
			{
				if(m_Animation && GetKeyState(VK_SHIFT)>>4)
				{
					this->m_Tool_Animation.AdvanceFrames(1);
					return TRUE;
				}
				else
				{
					if(m_pXDVView->m_PicEd.m_pLayer != m_pXDVView->m_PicEd.m_pCanvas)
					{
						m_pXDVView->m_PicEd.m_RectTempToCanvas.OffsetRect(1,0);
						m_pXDVView->Invalidate();
						m_pXDVView->m_PicEd.m_modded = true;
						return TRUE;
					}
					else
					{
						m_pXDVView->m_PicEd.OnKeyDown(pMsg->wParam);
						return TRUE;
					}
				}
			}
			break;
		case VK_LEFT:
			{
				if(m_Animation && GetKeyState(VK_SHIFT)>>4)
				{
					this->m_Tool_Animation.AdvanceFrames(-1);
					return TRUE;
				}
				else
				{
					if(m_pXDVView->m_PicEd.m_pLayer != m_pXDVView->m_PicEd.m_pCanvas)
					{
						m_pXDVView->m_PicEd.m_RectTempToCanvas.OffsetRect(-1,0);
						m_pXDVView->Invalidate();
						m_pXDVView->m_PicEd.m_modded = true;
						return TRUE;
					}
					else
					{
						m_pXDVView->m_PicEd.OnKeyDown(pMsg->wParam);
						return TRUE;
					}
				}
			}
			break;


		case VK_UP:
			{
				if(m_pXDVView->m_PicEd.m_pLayer != m_pXDVView->m_PicEd.m_pCanvas)
				{
					m_pXDVView->m_PicEd.m_RectTempToCanvas.OffsetRect(0,-1);
					m_pXDVView->Invalidate();
					m_pXDVView->m_PicEd.m_modded = true;
					return TRUE;
				}
				else
				{
					m_pXDVView->m_PicEd.OnKeyDown(pMsg->wParam);
					return TRUE;
				}
			}
			break;
		case VK_DOWN:
			{
				if(m_pXDVView->m_PicEd.m_pLayer != m_pXDVView->m_PicEd.m_pCanvas)
				{
					m_pXDVView->m_PicEd.m_RectTempToCanvas.OffsetRect(0,1);
					m_pXDVView->Invalidate();
					m_pXDVView->m_PicEd.m_modded = true;
					return TRUE;
				}
				else
				{
					m_pXDVView->m_PicEd.OnKeyDown(pMsg->wParam);
					return TRUE;
				}
			}
			break;

		case VK_DELETE:
			{
				m_pXDVView->m_PicEd.DeleteSelection();
				break;
			}
	
		case VK_ESCAPE:
		case VK_RETURN:
			{
				OnClose();
				EndDialog(0);
			}

			return TRUE;
		}

		if (interestingKey) {
			if (pHSTool) {
				pHSTool->hotspot.x = hs.x;
				pHSTool->hotspot.y = hs.y;
				pHSTool->OnMouseUp(hs);
				this->m_pXDVView->m_PicEd.m_modded = true;
				pHSTool->UpdateDisplayOffset();
			}
			if(pActTool){
				pActTool->hotspot.x = hs.x;
				pActTool->hotspot.y = hs.y;
				pActTool->OnMouseUp(hs);
				this->m_pXDVView->m_PicEd.m_modded = true;
				pActTool->UpdateDisplayOffset();
			}
	

			m_pXDVView->Invalidate();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CImageEditorDlg::CopyNeededDataFromImages()
{

	// Images and Collisions are read from the source and copied if they are modified. We just copying all
	// the hotspots and action points because it makes the code a lot easier

	for(vector<CImageResource*>::iterator i = m_sourceImages.begin(); i!= m_sourceImages.end(); i++)
	{
		m_newHotspots[*i] = (*i)->m_Hotspot;

		for(map<CString, CPoint>::iterator j = (*i)->m_Action.begin(); j!= (*i)->m_Action.end(); j++)
		{
			(m_newAction[*i])[j->first] = j->second;
		}
	}
}


void CImageEditorDlg::CopyNeededDataToImages()
{
	// This assumes that all went okay, and we want to copy our data back again
	
	// Copy the images
	{
		for(map<CImageResource*, CxImage>::iterator i = m_newImages.begin(); i!= m_newImages.end(); i++)
		{
			// Copy image over
			i->first->bitmap = i->second;
				
			// If we have a texture for it, we need to delete the texture. Construct will autmatically make a new one when it is needed
			// note that we also destroy the thumbnails...which construct also recreates when needed.
			map<int,int>::iterator imageit = application->m_image_to_texture.find(i->first->m_FixedID);
			if(imageit != application->m_image_to_texture.end())
				application->m_image_to_texture.erase( imageit);

			i->first->small_thumbnail.Destroy();
			i->first->large_thumbnail.Destroy();

		}
	}

	// Copy the collisions.
	{
		for(map<CImageResource*, CxImage>::iterator i = m_newCollision.begin(); i!= m_newCollision.end(); i++)
		{
			// Copy image over
			i->first->m_Collision = i->second;
		}
	}


	// Copy over hotspots
	{
		for(map<CImageResource*, CPoint>::iterator i = m_newHotspots.begin(); i!= m_newHotspots.end(); i++)
		{
			i->first->m_Hotspot = i->second;
		}
	}
	// Copy over actions
	{
		for(map<CImageResource*, map<CString, CPoint> >::iterator i = m_newAction.begin(); i!= m_newAction.end(); i++)
		{
			i->first->m_Action.clear();
			for(map<CString, CPoint>::iterator it = i->second.begin(); it!= i->second.end(); it++)
			{
				i->first->m_Action[it->first] = it->second;
			}
		}
	}


}


void CImageEditorDlg::GetActionNames(map<CString, int>& actions)
{
	CImageEditorDlg* imgDlg = this;
	for(map<CImageResource*,  map<CString, CPoint> >::iterator ii = imgDlg->m_newAction.begin();
		ii!= imgDlg->m_newAction.end(); ii++)
	{
		for(map<CString, CPoint>::iterator i = ii->second.begin(); i!= ii->second.end(); i++)
		{
			actions[i->first] = actions[i->first] + 1;
		}
	}



}



BOOL CImageEditorDlg::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
    ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

    // if there is a top level routing frame then let it handle the message
    if (GetRoutingFrame() != NULL) return FALSE;

    // to be thorough we will need to handle UNICODE versions of the message also !!
    TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
    TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
    TCHAR szFullText[512];
    CString strTipText;
    UINT nID = pNMHDR->idFrom;

    if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
        pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
    {
        // idFrom is actually the HWND of the tool 
        nID = ::GetDlgCtrlID((HWND)nID);
    }

    if (nID != 0) // will be zero on a separator
    {
		AfxLoadString(nID, szFullText);
        strTipText=szFullText;

#ifndef _UNICODE
        if (pNMHDR->code == TTN_NEEDTEXTA)
        {
            lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
        }
        else
        {
            _mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
        }
#else
        if (pNMHDR->code == TTN_NEEDTEXTA)
        {
            _wcstombsz(pTTTA->szText, strTipText,sizeof(pTTTA->szText));
        }
        else
        {
            lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
        }
#endif

        *pResult = 0;

        // bring the tooltip window above other popup windows
        ::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
            SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);
        
        return TRUE;
    }

    return FALSE;
}


void CImageEditorDlg::ApplyHotspotToAll(int x, int y)
{

	for(vector<CImageResource*>::iterator i = m_sourceImages.begin(); i!= m_sourceImages.end(); i++)
	{
		m_newHotspots[(*i)] = CPoint(x,y);
	}

	// any added frames
	for(map<CImageResource*, CxImage>::iterator i = m_newImages.begin(); i!= m_newImages.end(); i++)
	{
		m_newHotspots[i->first] = CPoint(x,y);
	}


	



}




void CImageEditorDlg::CropAll()
{
	for(int a = 0; a < m_sourceImages.size(); a++)
	{
		if(a == m_animation_frame_index)
			continue;
		CImageResource* img = m_sourceImages.at(a);

		CxImage* image = &img->bitmap;
		map<CImageResource*, CxImage>::iterator i = m_newImages.find(m_sourceImages.at(a));
		if(i == m_newImages.end())
		{
			m_newImages[img] = *image;		 // copy the image and make it point to the new image
		}
		image = &m_newImages[img];

			


		// This is a little workaround so we can obtain information about the image
		// we trick the CPictureEditor by setting m_animation_index to the index of the image
		// we are after...we just need to make sure we dont forget to change the value back!

		CPictureEditor* pEd = &m_pXDVView->m_PicEd;
		int backup = m_animation_frame_index;
		m_animation_frame_index = a;

		CRect crop = CropImage(*image, !(GetKeyState(VK_SHIFT)>>2));

		CPoint* hotspot = pEd->GetHotspot();
		if(hotspot)
		{
			hotspot->x -= crop.left;
			hotspot->y -= crop.bottom;
		}


		map<CString, CPoint>* actions = pEd->GetAction();

		if(actions)
		{
			for( map<CString, CPoint>::iterator it = actions->begin(); it != actions->end(); it++)
			{
				CPoint& action = it->second;
				action.x -= crop.left;
				action.y -= crop.bottom;
			}
		}
	
				
		// Revert the animation index back
		m_animation_frame_index = backup;
	}


}


CxImage* CImageEditorDlg::getOtherImageForThisFrame()
{
	if(this->m_sourceImages.size() == 0)
		return NULL;

	CImageResource* res = m_sourceImages[m_animation_frame_index];

	if(m_editCollision)
	{
		if( m_newImages.find(res) == m_newImages.end() )
			return &(m_newImages[res] = res->bitmap);			
		else
			return &m_newImages[res]; 
	}
	else
	{
		if( m_newCollision.find(res) == m_newCollision.end() )
			return &(m_newCollision[res] = res->m_Collision);			
		else
			return &m_newCollision[res]; 
	}
}




void CImageEditorDlg::AskToDeleteCollisionMask()
{
	if(this->m_sourceImages.size() == 0)
		return;
	int result = MessageBox("Are you sure you want to delete the collison mask?", "Construct Picture Editor", MB_YESNOCANCEL);
	if( result == IDYES)
	{
		CImageResource* res = m_sourceImages[m_animation_frame_index];
		if( m_newCollision.find(res) != m_newCollision.end() )
		{
			m_newCollision[res].Destroy();
		}
		else
		{
			res->m_Collision.Destroy();
		}
	}

}

//////////////////////////////////
//
//		CPicEdSettingsDlg
//
/////////////////////////////////


CPicEdSettingsDlg::CPicEdSettingsDlg(CWnd* pParent)
	: CExtNCW<CExtResizableDialog>(CPicEdSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPicEdSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CExtResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageEditorDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPicEdSettingsDlg, CExtResizableDialog)
	//{{AFX_MSG_MAP(CImageEditorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()