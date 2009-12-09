#include "StdAfx.h"
#include "LayerBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CLayerBar* g_pLayerBar;

/////////////////////////////////////////////////////////////////////////////
// CLayerBar

CLayerBar::CLayerBar()
{
}

CLayerBar::~CLayerBar()
{
}


BEGIN_MESSAGE_MAP(CLayerBar, CExtControlBar)
	//{{AFX_MSG_MAP(CLayerBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define IDC_LAYERLISTBOX 124

/////////////////////////////////////////////////////////////////////////////
// CLayerBar message handlers

int CLayerBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CExtControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rc;

	// Init the control's size to cover the entire client area
	GetClientRect(rc);
	m_layerDlg.Create(IDD_BARDIALOG, this);
	m_layerDlg.ShowWindow(SW_SHOW);

	return 0;
}


void CLayerBar::OnSize(UINT nType, int cx, int cy) 
{
	if (cx >= 90)
		CExtControlBar::OnSize(nType, cx, cy);
}