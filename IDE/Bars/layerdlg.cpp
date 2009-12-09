#include "StdAfx.h"
#include "LayerBar.h"
#include "PropertiesBar.h"

#include "..\MainFrm.h"
extern CMainFrame* g_MainFrame;

extern PropertiesBar* g_PropertyBar;

/////////////////////////////////////////////////////////////////////////////
// CLayerDlg dialog

CLayerDlg::CLayerDlg(CWnd* pParent /*=NULL*/)
	: CExtResizableDialog(IDD_BARDIALOG, pParent)
{
	m_layerListBox.layout_editor = NULL;
}

void CLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CExtResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_MENUBAR, m_Toolbar);
}

BEGIN_MESSAGE_MAP(CLayerDlg, CExtResizableDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()

	// Layer options
	ON_LBN_SELCHANGE(126, OnSelchange)
	ON_COMMAND(ID_MENU_ADDLAYOUT, OnLayerAdd)
	ON_COMMAND(ID_MENU_REMOVELAYOUT, OnLayerDel)
	ON_COMMAND(ID_MENU_ADDLAYOUTDOWN, OnLayerAddDown)

	ON_COMMAND(ID_LAYER_LOCK, OnLockLayer)
	ON_COMMAND(ID_LAYER_HIDE, OnHideLayer)

	ON_COMMAND(ID_LAYER_UP, OnLayerUp)
	ON_COMMAND(ID_LAYER_DOWN, OnLayerDown)
END_MESSAGE_MAP()

void CLayerDlg::OnLockLayer() 
{
	int item = m_layerListBox.GetCurSel();
	CLayer* pLayer = (CLayer*)m_layerListBox.GetItemDataPtr(item);

	if (item >= 0) 
	{
		pLayer->m_state = (pLayer->m_state & 0x01) + ((!ISLAYERLOCKED(pLayer->m_state)) <<1);
		layout_editor->m_bUpdateFrame = true;
		layout_editor->Invalidate();
	}
}

void CLayerDlg::OnHideLayer() 
{
	int item = m_layerListBox.GetCurSel();
	CLayer* pLayer = (CLayer*)m_layerListBox.GetItemDataPtr(item);

	if (item >= 0) 
	{
		pLayer->m_state = (pLayer->m_state & 0x02) + (ISLAYERVISIBLE(pLayer->m_state));
		layout_editor->m_bUpdateFrame = true;
		layout_editor->Invalidate();
	}
}

void CLayerDlg::OnSelchange() 
{
	if (((CLayer*)m_layerListBox.GetItemDataPtr(m_layerListBox.GetCurSel()))->m_layerType == LAYER_NORMAL)
	{
		m_layerListBox.layout_editor->layout->current_layer = (CLayer*)(m_layerListBox.GetItemDataPtr(m_layerListBox.GetCurSel()));
		CChildFrame* pCF = (CChildFrame*)m_layerListBox.layout_editor->GetParentFrame();
		pCF->object_bar.Refresh(true);
	}

	else
	{
		CLayer* previous = m_layerListBox.layout_editor->layout->current_layer;

		m_layerListBox.layout_editor->layout->current_layer = (CLayer*)(m_layerListBox.GetItemDataPtr(m_layerListBox.GetCurSel()));
		CChildFrame* pCF = (CChildFrame*)m_layerListBox.layout_editor->GetParentFrame();
		pCF->object_bar.Refresh(true);

		m_layerListBox.layout_editor->layout->current_layer = previous;
	}
}

void CLayerDlg::RefreshLayers()
{
	CLayerListBox *layerListBox = &m_layerListBox;

	// There's no layout available, clear and return
	if(layout_editor == NULL)
	{
		while (layerListBox->GetCount()) // remove all items from list
			layerListBox->DeleteString(0);
		return;
	}

	POSITION pos;

	int cursel = 0;
	CLayer* cursellayer =  layout_editor->layout->current_layer;

	CLayout* layout = layout_editor->layout;
	if (IsWindow(layerListBox->GetSafeHwnd())) 
	{
		if (layout != layerListBox->layout) 
		{
			int nIndex;
			layerListBox->layout_editor = layout_editor;

			while (layerListBox->GetCount()) // remove all items from list
				layerListBox->DeleteString(0);

			pos = layout->layers.GetTailPosition();

			for (int i = 0; i < layout->layers.GetCount(); i++)
			{
				CLayer *layer = layout->layers.GetPrev(pos);
				if( layer == cursellayer)
					cursel = i;
				nIndex = layerListBox->AddString(layer->m_layerType != LAYER_NONFRAME ? "56" : "24");
				layerListBox->SetItemDataPtr(nIndex, layer);
			}
		}
	}

	m_layerListBox.SetCurSel(cursel);

	layout_editor->layout->current_layer = (CLayer*)m_layerListBox.GetItemData(cursel);
}

void CLayerDlg::OnSize(UINT nType, int cx, int cy) 
{
	CExtResizableDialog::OnSize(nType, cx, cy);
	if (IsWindow(m_layerListBox.m_hWnd)) 
	{
		CRect Toolbar, List;
		m_Toolbar.GetWindowRect(Toolbar);

		List.top = Toolbar.Height();
		List.left = 0;
		List.right = cx;
		List.bottom = cy;

		m_layerListBox.MoveWindow(List);
	}
}

int CLayerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CExtResizableDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CLayerDlg::OnLayerAdd() 
{
	if (!m_layerListBox.layout_editor)
	{
		CErrorDlg error;
		error.Error("No layout is open.", "Before adding a layer, ensure a layout is open.");
		return;
	}

	// Get layer count for name
	CString Name;
	Name.Format("%s %d", LAYER, m_layerListBox.layout_editor->layout->layers.GetCount());

	CLayer* layer = new CLayer(Name, LAYER_NORMAL);

	layer->m_layerID = layout_editor->application->m_layerID++;
	m_layerListBox.layout_editor->layout->layers.InsertAfter(
		m_layerListBox.layout_editor->layout->layers.GetTailPosition(), layer);

	long nIndex = m_layerListBox.InsertString(0, "56");
	m_layerListBox.SetItemDataPtr(nIndex, layer);
	m_layerListBox.SetCurSel(nIndex);

	layout_editor->layout->SetChanged(true);

	OnSelchange();
}

void CLayerDlg::OnLayerAddDown() 
{
	if (!m_layerListBox.layout_editor)
	{
		CErrorDlg error;
		error.Error("No layout is open.", "Before adding a layer, ensure a layout is open.");
		return;
	}

	// Get layer count for name
	CString Name;
	Name.Format("%s %d", LAYER, m_layerListBox.layout_editor->layout->layers.GetCount());

	CLayer* layer = new CLayer(Name, LAYER_NORMAL);

	layer->m_layerID = layout_editor->application->m_layerID++;
	m_layerListBox.layout_editor->layout->layers.InsertAfter(
		m_layerListBox.layout_editor->layout->layers.Find(m_layerListBox.layout_editor->layout->layers.GetHead()), layer);

	long nIndex = m_layerListBox.InsertString(m_layerListBox.GetCount() - 1, "56");
	m_layerListBox.SetItemDataPtr(nIndex, layer);
	m_layerListBox.SetCurSel(nIndex);

	layout_editor->layout->SetChanged(true);

	OnSelchange();
}

void CLayerDlg::OnLayerDown() 
{
	// Get selection
	int Sel = m_layerListBox.GetCurSel();
	if (Sel == -1) return;

	if (((CLayer*)m_layerListBox.GetItemDataPtr(m_layerListBox.GetCurSel()))->m_layerType != LAYER_NORMAL) return;

	// Bounds check
	if (Sel == m_layerListBox.GetCount() - 2) return;

	// Delete it and move it down one space
	CLayer* pLayer = (CLayer*)m_layerListBox.GetItemDataPtr(Sel);
	if (pLayer->m_layerType == LAYER_NONFRAME) return;

	m_layerListBox.DeleteString(Sel);
	int Item = m_layerListBox.InsertString(Sel + 1, pLayer->GetName());
	m_layerListBox.SetItemDataPtr(Item, (void*)pLayer);

	// Get the layerlist, remove all items
	CLayerList* pLayerList = &layout_editor->layout->layers;
	while (pLayerList->GetCount() > 0)
		pLayerList->RemoveHead();

	// Add the layers back into the layerlist, in the order they are in the listbox
	CLayer *j;
	for (int i = 0; i < m_layerListBox.GetCount(); i++) {
		j = (CLayer*)m_layerListBox.GetItemDataPtr(i);
		pLayerList->AddHead(j);
	}

	layout_editor->layout->SetChanged(true);
	layout_editor->m_bUpdateFrame = true;

	m_layerListBox.SetRedraw();
	m_layerListBox.Invalidate();
	Invalidate();
	RefreshLayers();
}

void CLayerDlg::OnLayerUp() 
{
	// Get selection
	int Sel = m_layerListBox.GetCurSel();
	if (Sel == -1) return;

	// Bounds check
	if (Sel == 0) return;
	if (Sel == m_layerListBox.GetCount() - 1) return;

	// Delete it and move it down one space
	CLayer* pLayer = (CLayer*)m_layerListBox.GetItemDataPtr(Sel);

	if (pLayer->m_layerType == LAYER_NONFRAME) return;

	m_layerListBox.DeleteString(Sel);
	int Item = m_layerListBox.InsertString(Sel - 1, pLayer->GetName());
	m_layerListBox.SetItemDataPtr(Item, (void*)pLayer);

	// Get the layerlist, remove all items
	CLayerList* pLayerList = &layout_editor->layout->layers;
	while (pLayerList->GetCount() > 0)
		pLayerList->RemoveHead();

	// Add the layers back into the layerlist, in the order they are in the listbox
	CLayer *j;
	for (int i = 0; i < m_layerListBox.GetCount(); i++) {
		j = (CLayer*)m_layerListBox.GetItemDataPtr(i);
		pLayerList->AddHead(j);
	}

	layout_editor->layout->SetChanged(true);
	layout_editor->m_bUpdateFrame = true;
	layout_editor->Invalidate();

	m_layerListBox.SetRedraw();
	m_layerListBox.Invalidate();
	Invalidate();
	RefreshLayers();
}

void CLayerDlg::OnLayerDel() 
{
	if(!m_layerListBox.layout_editor)
		return;
	if(m_layerListBox.GetCurSel() == -1)
		return;

	CLayer *layer = (CLayer*)m_layerListBox.GetItemDataPtr(m_layerListBox.GetCurSel());
		if (layer->m_layerType == LAYER_NONFRAME) return;

	if (m_layerListBox.layout_editor->layout->layers.GetCount() == 2) 
	{
		CErrorDlg error;
		error.Error("Error removing layer", "You can't remove the last layer.");
		return;
	}

	bool do_delete = false;

	if (layer) 
	{
		CExtMsgBox msg(NULL, CONF_DELETE, CONFIRMATION, __EXT_MB_YESNO | __EXT_MB_ICONINFORMATION | __EXT_MB_DO_NOT_ASK_AGAIN, 0, "DeleteLayer", __EXT_MB_EX_CHECK_BOX_IS_NOT_CHECKED_INTIALLY);

		int result = msg.DoModal();

		if (result == IDYES)
			do_delete = true;

		else
			msg.ResetMsgBox();
	}

	if (do_delete)
	{
		POSITION pos = layer->m_zOrder.GetHeadPosition();
		vector<long> deletelist;
		for (int i = 0; i < layer->m_zOrder.GetCount(); i++) {
			deletelist.push_back(layer->m_zOrder.GetNext(pos));
		}

		for(int i = 0; i < deletelist.size(); i++)
		{
			m_layerListBox.layout_editor->DeleteObject(deletelist.at(i), TRUE);
		}

		// START UNDO INFO //
		CAction *action = new CAction();
		action->m_type = ACTION_CHANGEZORDER;
		CArchive *ar = action->Prepare1();
		m_layerListBox.layout_editor->SerializeAllZLists(ar);
		
		action->Prepare2();
		m_layerListBox.layout_editor->m_undo->AddAction(action);
		// END UNDO INFO //

		m_layerListBox.layout_editor->layout->layers.RemoveAt(m_layerListBox.layout_editor->layout->layers.Find(layer));
		m_layerListBox.DeleteString(m_layerListBox.GetCurSel());

		m_layerListBox.SetCurSel(0);
		OnSelchange();

		m_layerListBox.layout_editor->Invalidate();

		CChildFrame* pCF = (CChildFrame*)m_layerListBox.layout_editor->GetParentFrame();
		pCF->object_bar.Refresh();

		layout_editor->layout->SetChanged(true);
	}
}

BOOL CLayerDlg::OnInitDialog() 
{
	CExtResizableDialog::OnInitDialog();

	CRect rc;
	this->GetClientRect(&rc);
	rc.top = 21;
	rc.bottom = 50;

	m_layerListBox.Create(WS_CHILD|WS_VISIBLE|LBS_OWNERDRAWVARIABLE|//LBS_NOINTEGRALHEIGHT|
				LBS_HASSTRINGS|LBS_NOTIFY|WS_VSCROLL, rc, this, 126);

	CWinApp * pApp = ::AfxGetApp();

	m_Toolbar.LoadToolBar( IDR_ADDTOOLBAR );
	g_CmdManager->ProfileWndAdd(pApp->m_pszProfileName, m_hWnd);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
