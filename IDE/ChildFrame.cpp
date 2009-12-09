// ChildFrm.cpp : implementation of the CChildFrame class
//

// Standard include
#include "stdafx.h"
#include "Construct.h"
#include "MainFrm.h"

// Header
#include "ChildFrame.h"

// Dialogs
#include "Dialogs\ExchangeDlg.h"
#include "Dialogs\Used Plugins Dlg.h"
#include "Dialogs\NewTransitionDlg.h"
#include "Dialogs\Family Manager dialog\Family Manager Dlg.h"

// Tab ID
#define IDC_TAB 189

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_APPLICATION_VIEWUSEDPLUGINS, OnPlugins)
	ON_COMMAND(ID_PREVIEW, OnPreview)
	ON_COMMAND(ID_PREVIEWLAYOUT, OnPreviewLayout)
	ON_COMMAND(ID_DEBUG, OnDebug)
	ON_COMMAND(ID_DEBUGLAYOUT, OnDebugLayout)
	ON_COMMAND(ID_EXPORT_EXE, OnExportEXE)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_APP_PREF, OnFilePreferences)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVEIT, OnFileSave)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveAs)
	ON_COMMAND(ID_SHEETEDITOR, OnEventSheetEditor)
	ON_COMMAND(ID_LAYOUTEDITOR, OnLayoutEditor)
	ON_COMMAND(ID_VIEW_LAYERBAR, OnLayerBar)
	ON_COMMAND(ID_VIEW_SIDEBAR, OnProjectBar)
	ON_COMMAND(ID_VIEW_PROPERTIESBAR, OnPropertiesBar)
	ON_COMMAND(ID_EXCHANGE, OnExchange)
	ON_COMMAND(ID_TRANSITIONS, OnManageTransitions)
	ON_COMMAND(ID_MANAGEFAMILIES, OnManageFamilies)
	ON_COMMAND(ID_HIDEALL, OnHideAll)
	ON_NOTIFY(CTCN_SELCHANGE, IDC_TAB, OnSelChangeTab)
	ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

extern Construct *gApp;
extern CString gInsert;

extern CMainFrame* pMainWnd;

void CChildFrame::OnAddLayout()
{
	application->AddLayout();
}

void CChildFrame::OnRemoveLayout()
{
	application->RemoveLayout(layout);

	DestroyWindow();
}

void CChildFrame::OnExchange()
{
	CExchangeDlg Dlg;
	Dlg.DoModal();
}

void CChildFrame::OnManageTransitions()
{
	CNewTransitionDlg Dlg;
	if (Dlg.DoModal() == IDOK)
	{
		// Add to application
		CTransition& tr = Dlg.m_Transition;

		CString attemptName = tr.m_Name;
		int counter = 2;

		// Check unique name
		while (exists_element(application->m_Transitions.begin(), application->m_Transitions.end(), attemptName))
			attemptName.Format("%s %d", tr.m_Name, counter++);

		tr.m_Name = attemptName;

		tr.Init();

		application->m_Transitions.push_back(Dlg.m_Transition);
	}
}

void CChildFrame::OnManageFamilies()
{
	FamilyManagerDialog dialog(application);
	dialog.DoModal();
	application->ChangeModifiedStatus(true);

	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));
	pMainFrame->m_PropertiesBar.Refresh();	
}

// USED PLUGINS
void CChildFrame::OnPlugins()
{
	UsedPluginDialog dialog(application);
	dialog.DoModal();
}

// PREVIEW
void CChildFrame::OnPreview()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	pMainFrame->m_pPApp = application;
	pMainFrame->OnPreview();
}

// PREVIEW LAYOUT
void CChildFrame::OnPreviewLayout()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	pMainFrame->m_pPApp = application;
	pMainFrame->OnPreviewLayout(layout);
}

// DEBUG
void CChildFrame::OnDebug()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	pMainFrame->m_pPApp = application;
	pMainFrame->OnDebug();
}

void CChildFrame::OnDebugLayout()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	pMainFrame->m_pPApp = application;
	pMainFrame->OnDebugLayout();
}

// EXPORT EXE
void CChildFrame::OnExportEXE()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	pMainFrame->OnExportEXE();
}

// FILE NEW
void CChildFrame::OnFileNew()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	pMainFrame->OnFileNew();
}

// FILE OPEN
void CChildFrame::OnFileOpen()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	pMainFrame->OnFileOpen();
}

// FILE SAVE
void CChildFrame::OnFileSave()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	pMainFrame->OnFileSave(application);
}

// FILE SAVE
void CChildFrame::OnFileSaveAs()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	pMainFrame->OnFileSaveAs(application);
}

// FILE PREFERENCES
void CChildFrame::OnFilePreferences()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	pMainFrame->OnPreferences();
}

// EVENT EDITOR
void CChildFrame::OnEventSheetEditor()
{
	m_tabs.SelectionSet(1);
	LRESULT End = 0;
	OnSelChangeTab(NULL, &End);
	Invalidate();
}

// FRAME EDITOR
void CChildFrame::OnLayoutEditor()
{
	m_tabs.SelectionSet(0);
	LRESULT End = 0;
	OnSelChangeTab(NULL, &End);
	Invalidate();
}

// Hide all
void CChildFrame::OnHideAll()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	if (!pMainFrame->bIsMinimized)
	{
		// Store states
		pMainFrame->bWasOpenLayers = pMainFrame->m_LayerBar.IsVisible();
		pMainFrame->bWasOpenProject = pMainFrame->project_bar.IsVisible();
		pMainFrame->bWasOpenProperties = pMainFrame->m_PropertiesBar.IsVisible();

		if (pMainFrame->m_LayerBar.IsVisible())
			pMainFrame->OnLayerBar();

		if (pMainFrame->project_bar.IsVisible())
			pMainFrame->OnProjectBar();

		if (pMainFrame->m_PropertiesBar.IsVisible())
			pMainFrame->OnPropertiesBar();

		pMainFrame->bIsMinimized = true;
	}

	else
	{
		// Load states
		if (pMainFrame->bWasOpenLayers)
			pMainFrame->OnLayerBar();

		if (pMainFrame->bWasOpenProject)
			pMainFrame->OnProjectBar();

		if (pMainFrame->bWasOpenProperties)
			pMainFrame->OnPropertiesBar();

		pMainFrame->bIsMinimized = false;
	}
}

// LAYER BAR
void CChildFrame::OnLayerBar()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	pMainFrame->OnLayerBar();
}

// PROJECT BAR
void CChildFrame::OnProjectBar()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	pMainFrame->OnProjectBar();
}

// PROPERTIES BAR
void CChildFrame::OnPropertiesBar()
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	pMainFrame->OnPropertiesBar();
}

CObjType* CObj::GetObjectType(CApplication* pApplication) 
{
	if(this->m_bIsGroup)
		return NULL;

	CObjType *oT;

	long ObjectIdentifier = GetGlobalID();

	if (!pApplication->object_types.Lookup(ObjectIdentifier, oT))
	{
		if (!pApplication->m_DeletedObjectTypes.Lookup(ObjectIdentifier, oT))
		{
			return NULL;
		}
		pApplication->m_DeletedObjectTypes.RemoveKey(ObjectIdentifier);
		pApplication->object_types[ObjectIdentifier] = oT;

		pMainWnd->project_bar.UpdateApplication(pApplication);

		for(vector<class Family*>::iterator f = oT->families.begin(); f!= oT->families.end(); f++)
		{
			(*f)->is_in_use = true;
		}
	}

	return oT;
}

CChildFrame::CChildFrame(CApplication* application_, CLayout* layout_, EventSheet* event_sheet_) :
	application(application_),
	layout(layout_),
	event_sheet(event_sheet_),
	object_bar(*this)
{
	m_bInitSplitter = FALSE;

	for(int a = 0; a < 4; a++)
	{
		layout_editor[a%2][a/2] = 0;
		m_pEventView[a%2][a/2] = 0;
	}

	id = 79;
}

CChildFrame::~CChildFrame()
{
}

// Tab changed
void CChildFrame::OnSelChangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	int col, row;
	CView *pActiveView = (CView*)splitter.GetActivePane(&row, &col);

	if (pActiveView == NULL)
		return;

	// it is possible that we have to create other editors
	CCreateContext context;
	context.m_pCurrentDoc = pActiveView->GetDocument();

	CRect rect;

	UINT m_ID = AFX_IDW_PANE_FIRST;// + 1;
	

	// Okay first work out if the view is already owned. If it isn't
	// we assume its the frame editor as the frame editor is the first
	// view created

	bool found = false;
	for(int a = 0; a < 4; a ++)
	{
		if(m_pEventView[a%2][a/2] == pActiveView)
			found = true;
		if(layout_editor[a%2][a/2] == pActiveView)
			found = true;
	}
	
	if (m_tabs.ItemGetCount() == 1)
	{
		if(!found)
			{
				int a;
				for(a = 0; a < 4; a ++)
					if(!m_pEventView[a%2][a/2])
					{
						m_pEventView[row][col] = (EventSheetEditor*)pActiveView;
						break;

					}
				if(a==4)
					MessageBox("Error while switching editors: Memory may be leaked.");
			}
	}
	else
	{
		if(!found)
		{
			int a;
			for(a = 0; a < 4; a ++)
				if(!layout_editor[a%2][a/2])
				{
					layout_editor[row][col] = (CLayoutEditor*)pActiveView;
					break;

				}
			if(a==4)
				MessageBox("Error while switching editors: Memory may be leaked.");
		}
	}

	SetRedraw(false);	

	CView* pNewView = NULL;
	CRect rc;

	if(m_tabs.ItemGetCount() == 1)
		return ; // we have nothing more to do if its an event sheet only child window :/

	switch(m_tabs.SelectionGet())
	{
	case -1:
		return 	SetRedraw(true);
	case 0:
		if(!layout_editor[row][col])
		{
			layout_editor[row][col] = new CLayoutEditor;
			layout_editor[row][col]->layout = layout;
			layout_editor[row][col]->application = application;
			layout_editor[row][col]->Create(NULL, NULL, WS_CHILD, rect, this->GetWindow(GW_CHILD), m_ID, &context);
			layout_editor[row][col]->OnInitialUpdate();
		}

		pNewView = layout_editor[row][col];
		
		break;
	case 1:
		if(!m_pEventView[row][col])
		{
			m_pEventView[row][col] = new EventSheetEditor;
			m_pEventView[row][col]->layout = layout;
			m_pEventView[row][col]->application = application;
			EventSheet* sheet = event_sheet;
			m_pEventView[row][col]->Create(NULL, NULL, WS_CHILD, rect, this->GetWindow(GW_CHILD), m_ID, &context);
			m_pEventView[row][col]->OnInitialUpdate();
		}

		pNewView = m_pEventView[row][col];
		break;
	}

	if (pNewView) 
	{
		pNewView->SetRedraw(false);
		::SetWindowLong(pActiveView->m_hWnd, GWL_ID, 0);
		::SetWindowLong(pNewView->m_hWnd, GWL_ID, AFX_IDW_PANE_FIRST);

		pActiveView->GetClientRect(&rc);
		pNewView->MoveWindow(rc.left + 1, rc.top + 1, rc.Width(), rc.Height(), TRUE);
		pActiveView->ShowWindow(SW_HIDE);
		pNewView->ShowWindow(SW_SHOW);
		pNewView->OnInitialUpdate();
		

		// we need to tell the splitter to change the pane
		 ::SetWindowLong(pActiveView->m_hWnd, GWL_ID, 0);
		::SetWindowLong(pNewView->m_hWnd, GWL_ID, splitter.IdFromRowCol(row, col));

		this->SetActiveView(pNewView);
		RecalcLayout();
		splitter.RecalcLayout();
			
		SetRedraw(true);
		pNewView->SetRedraw(true);
		RedrawWindow();
		pNewView->RedrawWindow();
		pNewView->GetScrollBarCtrl(0)->Invalidate();
		pNewView->GetScrollBarCtrl(1)->Invalidate();

		

		//pNewView->Invalidate();
		//pNewView->SendMessage(WM_PAINT);

		CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));
		/*HTREEITEM item = pMainFrame->project_bar.application_tree.GetItemGetFirstVisibleItem();
		while(item){
			HTREEITEM item2 = pMainFrame->project_bar.application_tree.GetNextSiblingItem(
			item = pMainFrame->project_bar.application_tree.GetNextItem(item);


		}*/
	}

	pMainFrame->tabs.Invalidate();

	*pResult = 0;
	SwitchEditor();
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

extern CLayout* pNewLayout;
extern CApplication* pNewLayoutApp;
extern CDocument *g_pNewDoc;

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//layout = pNewLayout;
	//application = pNewLayoutApp;

	return 0;
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	CRect cr; 
	GetClientRect( &cr);

	splitter.m_pTabs = &m_tabs;

	BOOL ret = splitter.Create(this, 2, 2, CSize(48, 48), pContext);

	object_bar_parent.Create(BAR_OBJECTS, this, 127);
	object_bar_parent.EnableDocking(CBRS_ALIGN_ANY);
	object_bar_parent.DockControlBar(AFX_IDW_DOCKBAR_LEFT, true);
	object_bar_parent.SetInitDesiredSizeVertical(CSize(96, 400));

	object_bar.Create(IDD_BARDIALOG, &object_bar_parent);
	object_bar.ShowWindow(SW_SHOW);

	object_bar.SetLayout(layout, application);

	return ret;
}

void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
}

void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

		CMainFrame* pMainFrame = ((CMainFrame*)(CChildFrame::GetParentOwner()));

	if(bActivate)
		SwitchEditor();
	else
	{
		pMainFrame->m_PropertiesBar->Update(NULL, NULL, NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		pMainFrame->m_LayerBar.m_layerDlg.layout_editor = 0;
		pMainFrame->m_LayerBar.m_layerDlg.m_layerListBox.layout_editor = 0;
		pMainFrame->m_LayerBar.m_layerDlg.RefreshLayers();
	}
}

// we have changed editors or child windows
void CChildFrame::SwitchEditor(void)
{
	switch(m_tabs.SelectionGet())
		{
		case -1:
			return;
		case 0:
		//	pNewView = layout_editor;
			for(int a = 0; a < 4; a ++)
			{
				if(layout_editor[a%2][a/2])
					layout_editor[a%2][a/2]->SwitchEditor();
				if(m_pEventView[a%2][a/2])
					m_pEventView[a%2][a/2]->OnSwitchToLayoutEditor();
			}
			break;
		case 1:
		//	pNewView = m_pEventView;
			break;
		}
}


void CChildFrame::UpdateAllEditors(CLayoutEditor* donotupdate) //sync frame editors!
{
	for (int a = 0; a < 4; a ++)
		if (layout_editor[a%2][a/2])
			if (layout_editor[a%2][a/2] != donotupdate)
				layout_editor[a%2][a/2]->Invalidate();
}


void CChildFrame::InitialUpdateAllEditors()
{
	CWnd* pClientWindow = GetWindow(GW_CHILD)->GetWindow(GW_CHILD);

	while (pClientWindow != NULL)
	{
		EventSheetEditor* pEditor = dynamic_cast<EventSheetEditor*>(pClientWindow);

		if(pEditor)
			pEditor->OnInitialUpdate();

		pClientWindow = pClientWindow->GetWindow(GW_HWNDNEXT);
	}
}