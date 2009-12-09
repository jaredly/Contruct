// LayoutEditor.cpp : implementation of the CLayoutEditor class
//

// File includes
#include "StdAfx.h"
#include "LayoutEditor.h"
#include "..\Construct.h"
#include "..\Construct Doc.h"

#include "..\Bars\LayerBar.h"
#include "..\ChildFrame.h"
#include "..\Dialogs\Insert Object Dlg.h"
#include "..\Dialogs\ArrayPasteDlg.h"
#include "..\Dialogs\EditGridDlg.h"
#include "..\MainFrm.h"
#include "..\Dialogs\Used Plugins Dlg.h"
#include "..\Dialogs\New Behavior Dlg.h"
#include "..\Dialogs\NewHLSLDlg.h"
#include "..\Dialogs\ImageEditorDlg.h"
#include "..\Dialogs\ProgressDlg.h"
extern bool g_SuspendFrameEditor;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayoutEditor

IMPLEMENT_DYNCREATE(CLayoutEditor, CView)

BEGIN_MESSAGE_MAP(CLayoutEditor, CView)
	//{{AFX_MSG_MAP(CLayoutEditor)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_WM_MBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONUP()
	ON_WM_SHOWWINDOW()
	ON_WM_MBUTTONUP()
	ON_COMMAND(ID_FILE_NEW_TEMPLATE, OnNewTemplate)
	ON_WM_CANCELMODE()
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(IDFR_NEWMOVEMENT, OnUpdateNewMovement)
	ON_UPDATE_COMMAND_UI(IDFR_NEWEFFECT, OnUpdateNewEffect)
	ON_UPDATE_COMMAND_UI(IDFR_REMOVEALLMOVEMENTS, OnUpdateRemoveMovements)
	ON_UPDATE_COMMAND_UI(IDFR_REMOVEALLEFFECTS, OnUpdateRemoveEffects)
	ON_UPDATE_COMMAND_UI(IDFR_GROUP, OnUpdateGroups)
	ON_UPDATE_COMMAND_UI(ID_GRID_SNAPMOVEMENT, OnUpdateSnapMovement)
	ON_UPDATE_COMMAND_UI(ID_GRID_SNAPRESIZE, OnUpdateSnapResize)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_EDIT_DELETE, OnToolbarDelete)
	ON_COMMAND(ID_EDIT_CUT, OnToolbarCut)
	ON_COMMAND(ID_EDIT_COPY, OnToolbarCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnToolbarPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnToolbarUndo)
	ON_COMMAND(ID_EDIT_REDO, OnToolbarRedo)
	ON_COMMAND(ID_EDIT_SELECTALL, OnToolbarSelectAll)
	ON_COMMAND(ID_EDIT_UNSELECT, OnToolbarUnselect)
	ON_COMMAND(ID_TOGGLEGRID, ToggleGrid)
	ON_COMMAND(ID_EDITGRID, EditGrid)
	ON_COMMAND(ID_GRID_SNAPMOVEMENT, OnSnapMovement)
	ON_COMMAND(ID_GRID_SNAPRESIZE, OnSnapResize)
	ON_COMMAND(ID_LAYERS_SHOWBOX, OnShowBoxes)
	ON_COMMAND(ID_EDIT_INVERTSELECTION, OnToolbarInvertSelection)
	ON_COMMAND_RANGE(33000, 40000, OnFrameAction)
	ON_COMMAND_RANGE(8989, 9500, OnAddEvent)
	ON_REGISTERED_MESSAGE(CExtPopupBaseWnd::g_nMsgPrepareOneMenuLevel, OnExtMenuPrepareOneLevel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayoutEditor construction/destruction

extern CLayerBar* g_pLayerBar;
BOOL g_bPaste = FALSE;
BOOL g_bPasteClone = FALSE;
long g_addToGroup = -1;

extern PropertiesBar* g_PropertyBar;
extern CDragObjects *g_dragInfo;
extern CLayoutEditor* g_pLayoutView;

#define GRIDSIZE_X 16
#define GRIDSIZE_Y 16

CLayoutEditor::CLayoutEditor() :
	m_sizeGridStep( 8, 8 ), 
	m_bTrackingCreation(false),
	m_rcTrackingCreation( 0, 0, 0, 0 ),
	layout(NULL)
{
	m_bLayerBoxes = false;
	m_bInsert = FALSE;
	m_bDraggingRect = FALSE;
	m_bMouseDown = FALSE;
	m_bPanel = false;
	m_bMovingObject = FALSE;
	m_bLayersAfterMove = FALSE;
	m_bSizingObject = FALSE;
	m_bSetLayerList = FALSE;
	m_bUpdateFrame = TRUE;
	application = NULL;
	HasCreated = false;
	m_display = NULL;
	m_mdragging = false;
	m_usermoved = true;
	m_UpdatePreview = true;
	m_undo = 0;
	m_bSuspendDrawing = FALSE;
	m_bCachedID = FALSE;
	m_AutoIncrement = false;
	g_pLayoutView = this;
	iThumbnail = 0;
	iOriginalTexture = 0;
	iEffectTargets = 0;
	iDisplayTexture = 0;
	iLayerTexture = 0;
	m_PriorClonedObject = 0;
	m_display = 0;
	ScrollPos.x = 0; ScrollPos.y = 0;
	m_undo = 0;
	m_LastRibbon = 2;
	m_last_rendered_layers_clock = 0;
	scrolllock = 0;
	lastEnterPressed = false;
}

void CLayoutEditor::GetListOfAllGroups(CObjList* dest, CObjList* src)
{
	CObj*  o;
	POSITION pos = src->GetHeadPosition();

	long id;
	for (int i = 0; i < src->GetCount(); i++)
	{
		id = src->GetNext(pos);
		layout->objects.Lookup(id, o);

		if (o->m_bIsGroup)
		{	
			dest->AddTail(id);
			GetListOfAllGroups(dest, &o->m_groupObjects);

		}
	}
}

void CLayoutEditor::SerializeAllZLists(CArchive* ar)
{
	// This is not a normal serialize function - it is never used to save and load. It is only used by the undo system.
	if(ar->IsLoading())
	{
/*
TODO: clear everything , free everything, reserialze, update
*/

		// We need to remove existing data.
		POSITION pos = layout->layers.GetHeadPosition();
		for (int j = 0; j < layout->layers.GetCount(); j++)
		{
			delete layout->layers.GetNext(pos);
		}
		layout->layers.RemoveAll();
		

		int c;
		*ar >> c;

		for (int j = 0; j < c; j++)
		{
			CLayer* lyr = new CLayer("", 0);
			lyr->Serialize(*ar);
			layout->layers.AddTail(lyr);
	
			int groupcount;
			*ar >> (int)groupcount;

			for(int j = 0; j < groupcount; j++)
			{
				int id;
				*ar >> id;

				CObj* group;
				long longid = id;
				layout->objects.Lookup(longid, group);
				group->m_groupObjects.RemoveAll();

				int zcount;
				*ar >> zcount;


				for (int i = 0; i < zcount; i++)
				{
					long instance;
					*ar >> instance;
					
					group->m_groupObjects.AddTail(instance);
				}

			}	
		}
	}
	
	else
	{
		*ar << (int)layout->layers.GetCount();

		POSITION pos = layout->layers.GetHeadPosition();
		for (int j = 0; j < layout->layers.GetCount(); j++)
		{
			CLayer* lyr = layout->layers.GetNext(pos);
			lyr->Serialize(*ar);
		

			CObjList m_groupslist;
			GetListOfAllGroups(&m_groupslist, &lyr->m_zOrder);


			*ar << (int)m_groupslist.GetCount();

			POSITION pos2 = m_groupslist.GetHeadPosition();

			for (int j = 0; j < m_groupslist.GetCount(); j++)
			{
				long id = m_groupslist.GetNext(pos2);
				
				CObj* group;
				layout->objects.Lookup(id, group);
				POSITION pos = group->m_groupObjects.GetHeadPosition();

				*ar << id;
		
				*ar << (int) group->m_groupObjects.GetCount();

				for (int j = 0; j < group->m_groupObjects.GetCount(); j++)
				{
					long id = group->m_groupObjects.GetNext(pos);
					*ar << id;
				}
			}

		}



	}
	// we now have a list of all the groups
}


CLayoutEditor::~CLayoutEditor()
{
	// in case it is locked to an object we must unlock it
	//layout->ReleaseFrameEditor(layout->m_ObjectFrameIsLockedTo);
	
	if (layout->m_LayoutEditor == this) {
		layout->m_LayoutEditor = NULL;
	}
}

// Toggle grid
void CLayoutEditor::ToggleGrid()
{
	layout->m_Grid = !layout->m_Grid;
	Invalidate();
}

void CLayoutEditor::OnTimer(UINT nIDEvent) 
{	
	if( nIDEvent == m_nTimerID )
	{

		// See ondragover
		/*
		if( pt.y < rect.top + 10 && pt.y >= rect.top )
			ScrollToPosition(GetScrollPosition()+CPoint(0,-10));
		else if( pt.y > rect.bottom - 10 && pt.y <= rect.bottom  )
			ScrollToPosition(GetScrollPosition()+CPoint(0,10));
		if( pt.x < rect.left + 10 && pt.x >= rect.left  )
			ScrollToPosition(GetScrollPosition()+CPoint(-10,0));
		else if( pt.x > rect.right - 10 && pt.x <= rect.right   )
			ScrollToPosition(GetScrollPosition()+CPoint(10,0));*/
	}

	else {
		KillTimer(500);
		m_bPanel = false;
		Invalidate();

		CView::OnTimer(nIDEvent);
	}
}

void CLayoutEditor::AddLayout()
{
	application->AddLayout();
}

void CLayoutEditor::RemoveLayout()
{
	//application->RemoveLayout(layout);
}

void CLayoutEditor::OnSnapMovement()
{
	layout->m_SnapMovements = !layout->m_SnapMovements;
	Invalidate();
}

void CLayoutEditor::OnSnapResize()
{
	layout->m_SnapResize = !layout->m_SnapResize;
	Invalidate();
}

void CLayoutEditor::OnUpdateSnapMovement( CCmdUI * pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(layout->m_SnapMovements);
}

void CLayoutEditor::OnUpdateSnapResize( CCmdUI * pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(layout->m_SnapResize);
}


// Edit grid
void CLayoutEditor::EditGrid()
{
	CEditGridDlg Dlg;

	// Set old settings
	Dlg.m_OldHeight.Format("%d", layout->m_GridHeight);
	Dlg.m_OldWidth.Format("%d", layout->m_GridWidth);

	//Dlg.m_OldSnapTo = layout->m_SnapTo;

	if (Dlg.DoModal() == IDOK)
	{
		layout->m_GridHeight = atoi(Dlg.m_OldHeight);
		layout->m_GridWidth = atoi(Dlg.m_OldWidth);
		//layout->m_SnapTo = Dlg.m_OldSnapTo;
	}

	Invalidate();
}

BOOL CLayoutEditor::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

void CLayoutEditor::LockScrollUpdate()
{
	scrolllock ++;
	CScrollBar* pBar0 = GetScrollBarCtrl(0);
	CScrollBar* pBar1 = GetScrollBarCtrl(1);

	if (pBar0 != NULL)
		pBar0->SetRedraw(false);
	if (pBar1 != NULL)
		pBar1->SetRedraw(false);
}
void CLayoutEditor::UnlockScrollUpdate()
{
	scrolllock --;
	if(scrolllock == 0)
	{
		GetScrollBarCtrl(0)->SetRedraw(true);
		GetScrollBarCtrl(1)->SetRedraw(true);
		GetScrollBarCtrl(0)->Invalidate();
		GetScrollBarCtrl(1)->Invalidate();
		GetScrollBarCtrl(0)->RedrawWindow();
		GetScrollBarCtrl(1)->RedrawWindow();

	}
}
void CLayoutEditor::UpdateScrollbars()
{
	if(layout)
	{
		LockScrollUpdate();
		
		////CSize scroll;
		CSize scroll = GetTotalSize();

		


		CSize Desired(CSize((layout->GetW() + (FRAME_MARGIN_W * 2))*m_Zoom,( layout->GetH() + (FRAME_MARGIN_H * 2))*m_Zoom));
		SetScrollSizes(MM_TEXT, Desired);

		UnlockScrollUpdate();

	}


}

BOOL CLayoutEditor::PreTranslateMessage(MSG* pMsg) 
{
	if( m_ToolTip.m_hWnd != NULL )
		m_ToolTip.RelayEvent(pMsg);

	return CView::PreTranslateMessage(pMsg);
}

void CLayoutEditor::OnClose()
{
	//::MessageBox(NULL, "OnClose", NULL, NULL);
}

void CLayoutEditor::OnDestroy()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CLayoutEditor::GetParentOwner()));

	// application was just deleted if this flag set
	if (pMainFrame->has_been_closed)
		return;

	application->m_display_refcount--;

	//::MessageBox(NULL, "Decrementing ref count", NULL, NULL);

	if (application->m_display_refcount == 0) {

		//::MessageBox(NULL, "Releasing device", NULL, NULL);

		// All our textures are now dead
		application->iLayerTexture = -2;
		application->iOriginalTexture = -2;
		application->iEffectTargets[0] = -2;
		application->iEffectTargets[1] = -2;
		application->iDisplayTexture = -2;
		application->iPicture = -2;
		application->iThumbnail = -2;

		if(application->m_image_to_texture.size() != 0)
			application->m_image_to_texture.clear();

		layout->hdcTexture = -2;

		delete application->m_display;
		application->m_display = NULL;

	}
}

void CLayoutEditor::CreateDirectXFrame()
{
	// Initialise DirectX resources for layout editor
	m_image_to_texture = &application->m_image_to_texture;
	iLayerTexture = &application->iLayerTexture;
	iOriginalTexture = &application->iOriginalTexture;
	iEffectTargets = &application->iEffectTargets[0];
	iDisplayTexture = &application->iDisplayTexture;
	iPicture = &application->iPicture;
	iThumbnail = &application->iThumbnail;

	CD3DDisplaySetup setup;
	setup.bFullscreen = false;								// Windowed
	setup.fpsMode = CD3DFPS_VSYNCED;						// VSynced
	setup.hWnds.push_back(m_hWnd);
	setup.hFocusWnd = GetTopWindow()->GetSafeHwnd();
	setup.resFormat = D3DFMT_A8R8G8B8;						// 32 bit colour
	setup.resWidth = GetSystemMetrics(SM_CXSCREEN);			// Create to size of resolution
	setup.resHeight = GetSystemMetrics(SM_CYSCREEN);
	setup.swapEffect = D3DSWAPEFFECT_COPY;					// Enable Present() rect


	if((*m_display) == NULL)
	{
		//None of the frame editors in any of the pLayouts have initialized direct x
		// we will use our own pointers to do so
		(*m_display) = new CD3DDisplay;
		(*m_display)->Init(&setup);

		// Display linear filtering / arial default font
		(*m_display)->SetSamplerState(D3DSAMP_MINFILTER, application->game_information.sampler == 0 ? D3DTEXF_POINT : D3DTEXF_LINEAR);
		(*m_display)->SetSamplerState(D3DSAMP_MAGFILTER, application->game_information.sampler == 0 ? D3DTEXF_POINT : D3DTEXF_LINEAR);
	}

	// Allocate our initial textures
	if (*iDisplayTexture == -2)
		(*iDisplayTexture) = (*m_display)->AddTextureRT(setup.resWidth, setup.resHeight);

	if (*iPicture == -2)
	{
		CxImage img;
		img.LoadResource(FindResource(NULL, MAKEINTRESOURCE(IDR_FRAMEIMAGE), "PNG"), 0, NULL);
		(*iPicture) = (*m_display)->AddTextureFromCxImage(img);
	}
	// Add the layer thumbnail texture
	if (*iThumbnail == -2)
		(*iThumbnail) = (*m_display)->AddTextureRT(LYR_THUMB_W, LYR_THUMB_H);

	// We are now going to search other windows to see if we can find an undo buffer the same as ours
	
	CMainFrame* pMainWnd = (CMainFrame*)(this->GetParent()->GetParent()->GetParent()->GetParent());
	HWND clientWindow;
	clientWindow = ::GetWindow(pMainWnd->m_hWndMDIClient, GW_CHILD);


	CWnd* pane;
	CLayoutEditor* display_owner;

	while (clientWindow != NULL)
	{
		CChildFrame* child = (CChildFrame*)CWnd::FromHandle(clientWindow);

		for(int a = 0; a < 4; a ++)
		{
			display_owner = child->layout_editor[a%2][a/2];
			if(display_owner)
			{
				if(display_owner->layout == this->layout)
				{
					
					if(display_owner->m_undo)
					{
						
							m_undo = display_owner->m_undo;

							return;

					}
				}

			}
			// as we check the frames listed, we need to check the current panes as well
			// as they aren't reserved until they are deactivated
			pane = child->splitter.GetPane(a%2,a/2);
			if(pane)
			{
				for(int b = 0; b < 4; b++)
				{
					if(child->m_pEventView[b%2][b/2] == pane)
						b=4;
		
					if(b==3)
					{
						display_owner = (CLayoutEditor*)pane;
						if(display_owner->layout == this->layout)
						{
							if(display_owner->m_undo)
							{
								
								m_undo = display_owner->m_undo;

								return;
							}
						}
					}
				}
			}
			// end pane stuff


		}

		clientWindow = :: GetWindow(clientWindow, GW_HWNDNEXT);
	}

	m_undo = new CUndo(this);
}

int CLayoutEditor::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_Zoom = 1;
	m_zoom_val = 0;
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CProgressDlg ProgressDlg("Loading layout");
	ProgressDlg.Start();
	ProgressDlg.SetProgress(0);

	m_ToolTip.Create(this);

	CMainFrame* pMainFrame = ((CMainFrame*)(CLayoutEditor::GetParentOwner()));

	g_pLayoutView = this;

	m_bMovingObject = FALSE;
	m_bLayersAfterMove = FALSE;
	m_bSizingObject = FALSE;
	m_sizeType = 0;
	m_sizeCur = IDC_ARROW;

	ddm.MakeDropTarget(this);
	ddm.AddFormat(DRAGDROP_OBJECTS);

	// DX creates the backbuffer size to the screen size.
	SetWindowPos(NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOMOVE);
		
	// Initialise frame
	try {
		// Child window & property window management
		CWnd* childWindow = this->GetParent()->GetWindow(GW_CHILD);
		m_drawCount = 0;

		if (childWindow == this)
		{
			g_pLayerBar->m_layerDlg.m_layerListBox.layout_editor = this;
			g_pLayerBar->m_layerDlg.layout_editor = this;
		}

		// Get layout and windows
		CChildFrame* pChildFrame = (CChildFrame*)GetParentFrame();

		if (pChildFrame->layout == NULL)
			throw;

		// Set up pointers and windowing
		pChildFrame->layout_editor[0][0] = this;
		pChildFrame->SetActiveView(this);
		layout = pChildFrame->layout;
		application = pChildFrame->application;

		// Point to the application display
		m_display = &application->m_display;
		application->m_display_refcount++;

		layout->m_LayoutEditor = this;

		UpdateScrollbars();
		
		g_PropertyBar->Update(this, TYPE_LAYOUT, NULL, layout, NULL);

		if(InputLocked())
		{
			g_PropertyBar->Update(this, TYPE_OBJECT_CAPTURED, NULL, layout, NULL, application); // show object props
		}

		ProgressDlg.SetProgress(10);

		// Initialise DirectX stuff
		CreateDirectXFrame();

		ProgressDlg.SetProgress(20);

		// Set up objects
		CObj *o;
		OINFO *oInfo;
		DWORD nKey2;
		CObjMap* objMap = &(layout->objects);
		POSITION pos = objMap->GetStartPosition();
		long nKey;
		int index = 0;
		while (pos != NULL)
		{
			layout->m_LayoutEditor = this;
			// get object and dll info
			objMap->GetNextAssoc(pos, nKey, o);

			//REVISE
			// Redecleration of 'pos':  Ought to be avoided
			POSITION pos2 = g_dlls->GetStartPosition();

			while (pos2 != NULL) { // locate the object DLL to use
				g_dlls->GetNextAssoc(pos2, nKey2, oInfo);
				if (oInfo->oID == o->GetDLLIdentifier())
					break;
			}
			if (oInfo->oID != o->GetDLLIdentifier())
				continue;

			// call object creation function
			oInfo->ETInitializeObject(o->editObject, layout);

			// Initializing objects adds textures and can be slow.
			// Use 20-100% for this
			ProgressDlg.SetProgress(20 + (80 * index) / objMap->GetCount());

			index++;
		}
		if(layout->m_oTemporyBehaviorInfo)
		{
			layout->m_oTemporyBehaviorInfo->ETInitializeObject(layout->m_pTempMovExt, layout);
		}
		// We need to call initialize on any objects that are in a layer that is inherrited
		POSITION layer_pos = layout->layers.GetHeadPosition();
		while(layer_pos)
		{
			CLayer* pLayer = layout->layers.GetNext(layer_pos);
			if(pLayer)
			{
				CLayout* pFoundLayout;
				CLayer* pInherrited = application->FindLayerFromNumber(pLayer->m_inherrit_layerID, &pFoundLayout);
				if(pInherrited && pFoundLayout)
				{
					POSITION inherit_pos = pInherrited->m_zOrder.GetHeadPosition();
					while(inherit_pos)
					{
						long objID = pInherrited->m_zOrder.GetNext(inherit_pos);
						CObj* o;
						pFoundLayout->objects.Lookup(objID, o);

						if(o)
						{
							DWORD dllid = o->GetDLLIdentifier();
							g_dlls->Lookup(dllid, oInfo);
							// call object creation function
							oInfo->ETInitializeObject(o->editObject, layout);
						}
					} // end while
				} 
			}
		} //end while

	}//try
	// Handle out of memory/general message_bar
	catch (CD3DError e) {

		if (e.hr = D3DERR_OUTOFVIDEOMEMORY) {
			CErrorDlg err;
			err.Error("Frame editor error", "There is not enough video memory to open this frame editor.  Please close some other editors and try again.");
			return -1;
		}
		CErrorDlg err;
		err.Error("Frame editor error", "Could not open the frame editor:\n\n" + e.GetString());
		return -1;
	}
	catch (...) {
		CErrorDlg err;
		err.Error("Frame editor error", "Error opening the frame editor.  Please report this bug to Scirra.");
		return -1;
	}

	ScrollPos = layout->scroll_position;
	SetScrollPos(0, layout->scroll_position.x);
	SetScrollPos(1, layout->scroll_position.y);
	doMove = false;

	if(layout->scroll_position.x == 0 && layout->scroll_position.y == 0)
		doMove = true;

	ProgressDlg.SetProgress(100);
	ProgressDlg.Finish();

	pMainFrame->m_Ribbon.Ribbon_PageSelectionSet(2);

	return 0;
}

void CLayoutEditor::OnInitialUpdate()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CLayoutEditor::GetParentOwner()));

	pMainFrame->m_Ribbon.Ribbon_PageSelectionSet(2);

	CView::OnInitialUpdate();
}

void CLayoutEditor::OnSize(UINT nType, int cx, int cy) 
{
	UpdateScrollbars();

	CView::OnSize(nType, cx, cy);
}

BOOL CLayoutEditor::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	LPNMHDR nmh = (LPNMHDR)lParam;

	return CView::OnNotify(wParam, lParam, pResult);
}

void CLayoutEditor::UpdateRulersInfo(int nMessage, CPoint ScrollPos, CPoint Pos)
{
	//DAVOTODO
	if (!layout)
		return;
}

/////////////////////////////////////////////////////////////////////////////
// CLayoutEditor diagnostics

#ifdef _DEBUG
void CLayoutEditor::AssertValid() const
{
	CView::AssertValid();
}

void CLayoutEditor::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLayoutEditor message handlers

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Other ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CBehavior::CreateDefault(CLayout* pLayout)
{
	if (eData) {
		delete[] eData;
		eData = NULL;
	}

	// Update oInfo
	oInfo = GetOINFO(BehaviorDLLIndex);

	// Make temp Behavior
	editInfo* tempMovExt = new editInfo;
	tempMovExt->DLLIndex = BehaviorDLLIndex;
	tempMovExt->instanceID = -1;
	tempMovExt->peditInfo = pLayout;

	pLayout->m_pTempMovExt = tempMovExt;
	pLayout->m_oTemporyBehaviorInfo = oInfo;
	pLayout->m_pControlBehaviorData = &eData;
	pLayout->m_pControlBehaviorDataSize = &eSize;

	oInfo->ETCreateObject(tempMovExt, pLayout);

	// Create a buffer from the default object
	bin movData(g_allocator);
	movData.allocate(1024);
	movData.loading = false;
	oInfo->ETSerialize(tempMovExt, movData);

	eData = (BYTE*)movData.ptr();
	eSize = movData.size();

	// Don't dealloc the buffer
	movData.detach();


	if(pLayout->m_ObjectFrameIsLockedTo == -1)
	{
		// the tempory Behavior has been locked...so we dont want to free it.
	}
	else
	{
		// Now destroy the temporary
		oInfo->ETDestroyObject(tempMovExt);
		pLayout->m_pTempMovExt = 0;
		pLayout->m_oTemporyBehaviorInfo = 0;
		pLayout->m_pControlBehaviorData = 0;
		pLayout->m_pControlBehaviorDataSize = 0;
		delete tempMovExt;
	}

}

// Item changed in property window. Update.
//
/*
void CLayoutEditor::OnItemChanged(COptionTreeItem* item)
{
	if (!layout)
		return;

	SetCursor(LoadCursor(NULL, IDC_WAIT));

	// Modifying frame properties
	if (m_sel.IsEmpty()) {
		// Do the change, check if refresh needed
		if (item->pValueMgr->OnChange() == 1) {
			propertyWindow->Update(this, propertyWindow->m_type, NULL, layout, NULL, NULL, 0, propertyWindow->pLastLayer);
		}
	}

	// Otherwise modifying object properties
	else 
	{
		long ctrlID = item->GetCtrlID();
		CObj* o;
		POSITION pos = m_sel.GetHeadPosition();

		for (int i = 0; i < m_sel.GetCount(); i++)
		{
			long object = m_sel.GetNext(pos);

			if (!layout->objects.Lookup(object, o))
				continue;
			
			// Do the property change for an object
			if (item->pValueMgr) if (item->pValueMgr->OnChange() == 1) {
				// Property bar needs updating (for the Add New Behavior button)
				// Show object props, expand Behaviors menu
				propertyWindow->Update(this, TYPE_OBJECT, &m_sel, layout, &layout->objects, NULL, 1);
			}
		}
	}

	m_bUpdateFrame = TRUE;
	RedrawWindow();
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
*/

CObj* CLayoutEditor::GetObject(long instanceID) 
{
	CObj* o;
	if (layout->objects.Lookup(instanceID, o))
		return o;
	else
		return NULL;
}

void CLayoutEditor::OnShowWindow(BOOL bShow, UINT nStatus) 
{

	CView::OnShowWindow(bShow, nStatus);	
}

// Frame editor draw functions
void CLayoutEditor::FrameBox(int width,RECT rc, D3DCOLOR filter)
{
	rc.left += (m_Zoom*FRAME_MARGIN_W - scrollX);
	rc.right += (m_Zoom*FRAME_MARGIN_W - scrollX);
	rc.top += (m_Zoom*FRAME_MARGIN_H - scrollY);
	rc.bottom += (m_Zoom*FRAME_MARGIN_H - scrollY);

	for(int a = 0; a < width; a ++)
	{
		rc.left --;
		rc.right ++;
		rc.top --;
		rc.bottom ++;
		(*m_display)->Box(rc, filter);
	}
}
void CLayoutEditor::FrameBox(RECT rc, D3DCOLOR filter)
{
	rc.left += (m_Zoom*FRAME_MARGIN_W - scrollX);
	rc.right += (m_Zoom*FRAME_MARGIN_W - scrollX);
	rc.top += (m_Zoom*FRAME_MARGIN_H - scrollY);
	rc.bottom += (m_Zoom*FRAME_MARGIN_H - scrollY);

	(*m_display)->Box(rc, filter);
}

void CLayoutEditor::FrameFill(RECT rc, D3DCOLOR filter)
{
	rc.left += (m_Zoom*FRAME_MARGIN_W - scrollX);
	rc.right += (m_Zoom*FRAME_MARGIN_W - scrollX);
	rc.top += (m_Zoom*FRAME_MARGIN_H - scrollY);
	rc.bottom += (m_Zoom*FRAME_MARGIN_H - scrollY);

	(*m_display)->Fill(rc, filter);
}

void CLayoutEditor::FrameLine(int x1, int y1, int x2, int y2, D3DCOLOR color)
{
	x1 += (FRAME_MARGIN_W*m_Zoom - scrollX);
	x2 += (FRAME_MARGIN_W*m_Zoom - scrollX);
	y1 += (FRAME_MARGIN_H*m_Zoom - scrollY);
	y2 += (FRAME_MARGIN_H*m_Zoom - scrollY);

	(*m_display)->Line(x1, y1, x2, y2, color);
}

/////////////////////////////////////////////////////////////////////////////
///////////////////////////// Toolbar handlers///////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CLayoutEditor::OnToolbarDelete()
{
	if(InputLocked())
	{
		layout->Message(WM_KEYDOWN);
		return;
	}

	DeleteSelection();
}

void CLayoutEditor::OnToolbarCut()
{
	if(InputLocked())
	{
		layout->Message(WM_KEYDOWN);
		return;
	}

	CRect rcSel = GetSelRect();
	DROPEFFECT drop = DROPEFFECT_NONE;

	g_dragInfo->layout_editor = this;
	g_dragInfo->layout = layout;
	g_dragInfo->m_pSel = &m_sel;
	g_dragInfo->application = application;
	g_dragInfo->m_curPt = rcSel.TopLeft();
	g_dragInfo->clipboard = true;

	if (m_sel.GetCount() == 1 && (GetOINFO(layout->GetObj(m_sel.GetHead())->GetObjectType(application)->DLLIndex)->ideFlags & OF_NODRAW))
		g_dragInfo->nonlayout = true;

	ddm.PrepareDrop(DO_CLIPBOARD, DRAGDROP_OBJECTS, g_dragInfo, &drop);

	DeleteSelection(true);
	m_sel.RemoveAll();

	m_UpdatePreview = true;

	CChildFrame* pCF = (CChildFrame*)GetParentFrame();
	pCF->object_bar.Refresh();

	Invalidate();
}

void CLayoutEditor::OnToolbarCopy()
{
	if(InputLocked())
	{
		layout->Message(WM_KEYDOWN);
		return;
	}

	CRect rcSel = GetSelRect();
	DROPEFFECT drop = DROPEFFECT_NONE;

	g_dragInfo->layout_editor = this;
	g_dragInfo->layout = layout;
	g_dragInfo->m_pSel = &m_sel;
	g_dragInfo->application = application;
	g_dragInfo->m_curPt = rcSel.TopLeft();
	g_dragInfo->clipboard = true;

	g_pLayoutView = this;
	g_pFrame = layout;
	g_pSel = &m_sel;
	g_pApp = application;

	if (m_sel.GetCount() == 1 && (GetOINFO(layout->GetObj(m_sel.GetHead())->GetObjectType(application)->DLLIndex)->ideFlags & OF_NODRAW))
		g_dragInfo->nonlayout = true;

	ddm.PrepareDrop(DO_CLIPBOARD, DRAGDROP_OBJECTS, g_dragInfo, &drop);

	m_UpdatePreview = true;

	Invalidate();
}

void CLayoutEditor::OnToolbarPaste()
{
	if(InputLocked())
	{
		layout->Message(WM_KEYDOWN);
		return;
	}

	if (ddm.OkToPaste())
	{
		if (g_dragInfo->nonlayout)
		{
			g_dragInfo->nonlayout = false;

			DROPEFFECT drop = DROPEFFECT_NONE;
			CPoint p(-10000, -10000);
			g_pLayoutView = this;
			g_pFrame = layout;
			g_pSel = &m_sel;
			g_pApp = application;
			g_pPt = &p;
			g_pDrop = &drop;
			g_bDuplicate = FALSE;

			CDragObjects *dragInfo = new CDragObjects;

			if (g_bPasteClone)
			{
				g_bPasteClone = FALSE;
				dragInfo->clone = true;
			}

			if (!ddm.DoDrop(dragInfo, NULL, DRAGDROP_OBJECTS)) 
			{
				CErrorDlg error;
				error.Error("Drag/drop error", "An error occured whilst dropping objects.");
			}

			delete dragInfo;
			m_bUpdateFrame = TRUE;

			// add object to correct layer
			layout->current_layer->m_zOrder.RemoveTail();
			layout->layers.GetHead()->m_zOrder.AddTail(m_sel.GetHead());

			g_PropertyBar->Update(this, TYPE_OBJECT, &m_sel, layout, &layout->objects, application); // show object props

			CChildFrame* pCF = (CChildFrame*)GetParentFrame();
			pCF->object_bar.Refresh();

			layout->SetChanged(true);

			return;
		}

		g_bPaste = TRUE;
		m_UpdatePreview = true;
		Invalidate();
	}
}

void CLayoutEditor::OnToolbarSelectAll()
{
	if(InputLocked())
	{
		layout->Message(WM_KEYDOWN);
		return;
	}

	m_sel.RemoveAll();

	long nKey = 0;
	CObj* o;
	POSITION pos = layout->objects.GetStartPosition();

	while (pos != NULL) 
	{
		// Iterate through objects and add all to selection list
		layout->objects.GetNextAssoc(pos, nKey, o);

		if (!o->m_bLocked && !o->m_bVisible)
			m_sel.AddTail(nKey);
	}

	m_UpdatePreview = true;

	// Draw selections
	Invalidate();
}

void CLayoutEditor::OnToolbarUnselect()
{
	if(InputLocked())
	{
		layout->Message(WM_KEYDOWN);
		return;
	}

	m_sel.RemoveAll();

	m_UpdatePreview = true;

	// Draw.. lack of selections
	Invalidate();
}

void CLayoutEditor::OnToolbarInvertSelection()
{
	if(InputLocked())
	{
		layout->Message(WM_KEYDOWN);
		return;
	}

	m_UpdatePreview = true;

	long nKey = 0;
	CObj* o;
	POSITION pos = layout->objects.GetStartPosition();

	while (pos != NULL) 
	{
		// Iterate through objects and add all to selection list
		layout->objects.GetNextAssoc(pos, nKey, o);

		if (!o->m_bInGroup || (o->m_bInGroup && o->m_group->m_bGroupOpen))
		{
			if (m_sel.Find(nKey) == NULL)
				m_sel.AddTail(nKey);

			else
			{
				POSITION Lookup = m_sel.Find(nKey);
				m_sel.RemoveAt(Lookup);
			}
		}
	}

	m_UpdatePreview = true;

	Invalidate();
}

void CLayoutEditor::OnToolbarUndo()
{
	if (GetFocus() == this)
	{
		if(InputLocked())
		{
			layout->Message(WM_KEYDOWN);
			return;
		}

		m_undo->Undo();
		Invalidate();
		m_UpdatePreview = true;

		// Also, update the object bar
		CChildFrame* pCF = (CChildFrame*)GetParentFrame();
		pCF->object_bar.Refresh();	
	}
}

void CLayoutEditor::OnToolbarRedo()
{
	if (GetFocus() == this)
	{
		if(InputLocked())
		{
			layout->Message(WM_KEYDOWN);
			return;
		}

		m_undo->Redo();
		Invalidate();
		m_UpdatePreview = true;

		// Also, update the object bar
		CChildFrame* pCF = (CChildFrame*)GetParentFrame();
		pCF->object_bar.Refresh();	
	}
}

// Should ribbon let objects be grouped
void CLayoutEditor::OnUpdateGroups(CCmdUI* pCmdUI) 
{
	if (m_sel.GetCount() != 0) 
	{
		pCmdUI->Enable(TRUE);
		return;
	}

	pCmdUI->Enable(FALSE);
}

// Should ribbon let Behaviors/effects be inserted
void CLayoutEditor::OnUpdateNewMovement(CCmdUI* pCmdUI) 
{
	if (m_sel.GetCount() != 0) 
	{
		CObj* o;
		bool bAllow = true;

		POSITION pos = m_sel.GetHeadPosition();

		for (int i=0; i < m_sel.GetCount(); i++) 
		{
			if(o = GetObject(m_sel.GetNext(pos)))
				if(CObjType* type = o->GetObjectType(application))
					if(OINFO* pInfo = GetOINFO(type->DLLIndex))
						if (!(pInfo->ideFlags & OF_MOVEMENTS))
							bAllow = false;
		}

		if (bAllow)
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}

	pCmdUI->Enable(FALSE);
}

void CLayoutEditor::OnShowBoxes()
{
	// Invert setting
	m_bLayerBoxes = true;
}

void CLayoutEditor::OnUpdateNewEffect(CCmdUI* pCmdUI) 
{
	if (m_sel.GetCount() != 0) 
	{
		CObj* o;
		bool bAllow = true;

		POSITION pos = m_sel.GetHeadPosition();

		for (int i=0; i < m_sel.GetCount(); i++) 
		{
			if(o = GetObject(m_sel.GetNext(pos)))
				if(CObjType* type = o->GetObjectType(application))
					if(OINFO* pInfo = GetOINFO(type->DLLIndex))
						if (!(pInfo->ideFlags & OF_EFFECTS))
							bAllow = false;
		}

		if (bAllow)
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}

	pCmdUI->Enable(FALSE);
}

void CLayoutEditor::OnUpdateRemoveMovements(CCmdUI* pCmdUI) 
{
	if (m_sel.GetCount() != 0) 
	{
		CObj* o;
		bool bAllow = true;

		POSITION pos = m_sel.GetHeadPosition();

		for (int i=0; i < m_sel.GetCount(); i++) 
		{
			if(o = GetObject(m_sel.GetNext(pos)))
				if(CObjType* type = o->GetObjectType(application))
					if(OINFO* pInfo = GetOINFO(type->DLLIndex))
						if (!(pInfo->ideFlags & OF_MOVEMENTS))
							bAllow = false;
		}

		if (bAllow)
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}

	pCmdUI->Enable(FALSE);
}

void CLayoutEditor::OnUpdateRemoveEffects(CCmdUI* pCmdUI) 
{
	if (m_sel.GetCount() != 0) 
	{
		CObj* o;
		bool bAllow = true;

		POSITION pos = m_sel.GetHeadPosition();

		for (int i=0; i < m_sel.GetCount(); i++) 
		{
			if(o = GetObject(m_sel.GetNext(pos)))
				if(CObjType* type = o->GetObjectType(application))
					if(OINFO* pInfo = GetOINFO(type->DLLIndex))
						if (!(pInfo->ideFlags & OF_EFFECTS))
							bAllow = false;
		}

		if (bAllow)
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}

	pCmdUI->Enable(FALSE);
}

void CLayoutEditor::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_undo->CanUndo());
}

void CLayoutEditor::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_undo->CanRedo());
}

void CLayoutEditor::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_sel.IsEmpty());
}

void CLayoutEditor::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_sel.IsEmpty());
}

void CLayoutEditor::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(ddm.OkToPaste());
}

void CLayoutEditor::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_sel.IsEmpty());
}

void CLayoutEditor::OnNewTemplate() 
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CLayoutEditor::GetParentOwner()));

	pMainFrame->OnFileTemplate();
}
void CLayoutEditor::SwitchEditor(void)
{
	// this is where we update the bars because its called when we change editors
	// either by selecting a different window or by clicking the tab and switching
	// editor types

	////////////////////////////////////
	//   Add layers to the layer bar 
	///////////////////////////////////
		
	g_pLayerBar->m_layerDlg.layout_editor = this;
	g_pLayerBar->m_layerDlg.RefreshLayers();
}

CSize CLayoutEditor::CalcActualViewSize( CDC * pDC )
{
	ASSERT_VALID( this );

CSize sizeLayout;
	sizeLayout.cx += 4;
	sizeLayout.cy += 4;
	return sizeLayout;
}

CRect CLayoutEditor::CalcActualViewRect( CDC * pDC )
{
	ASSERT_VALID( this );
	return
		CRect(
			CPoint( 20, 40 ),
			CalcActualViewSize(pDC)
			);
}
LRESULT CLayoutEditor::OnDrawRuler(WPARAM wParam, LPARAM lParam)
{
	
	return 0L;
}

void CreatePremultipliedThumbnail(CxImage* src, CxImage* dest, int width, int height, RGBQUAD background, bool checkers)
{
	CreatePremultipliedThumbnail(src, dest, width, height, RGB(background.rgbRed, background.rgbGreen, background.rgbBlue), checkers);
}

void CreatePremultipliedThumbnail(CxImage* src, CxImage* dest, int width, int height, COLORREF background, bool checkers)
{
	if(!src) 
		return;
	
	int originalwidth = src->GetWidth();
	int originalheight = src->GetHeight();

	if(originalwidth * originalheight == 0)
		return;



	int newwidth = width;
	int newheight = height * ((float)originalheight/(float)originalwidth);

	if(originalwidth < originalheight)
	{
		newheight = height;
		newwidth = width * ((float)originalwidth/(float)originalheight);
	}

	if(newwidth> originalwidth)
	{
		newwidth = originalwidth;
		newheight = originalheight;
	}

	int xoffset = (width - newwidth)/2;
	int yoffset = (height - newheight)/2;

	CxImage newimage;

	src->Resample2(newwidth, newheight,CxImage::IM_BICUBIC2, CxImage::OM_REPEAT,&newimage);
	CxImage* mod = src;
	if(dest)
		mod = dest;
	mod->Create(width, height, background, 0);

	RGBQUAD bg;
	bg.rgbRed = GetRValue(background);
	bg.rgbGreen = GetGValue(background);
	bg.rgbBlue = GetBValue(background);
	bg.rgbReserved = 255;

	{
		for(int x = 0; x < width; x++)
			for(int y = 0; y < height; y++)
			{
				mod->SetPixelColor(x,y, bg, true);
			}
		}
	{
		for(int x = 0; x < newwidth; x++)
			for(int y = 0; y < newheight; y++)
			{
				RGBQUAD colour = newimage.GetPixelColor(x,y,true);
				colour.rgbBlue += GetRValue(background) * (1-(colour.rgbReserved/255.0));
				colour.rgbGreen += GetGValue(background) * (1-(colour.rgbReserved/255.0));
				colour.rgbRed += GetRValue(background) * (1-(colour.rgbReserved/255.0));
				colour.rgbReserved = 255;

				mod->SetPixelColor(x+xoffset, y+yoffset, colour, true);
			}
	}
			


}


bool CLayoutEditor::InputLocked()
{
	return layout->m_ObjectFrameIsLockedTo != 0;




}