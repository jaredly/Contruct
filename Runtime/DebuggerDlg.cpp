// DebuggerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DebuggerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDebugger dialog

CDebugger::CDebugger(CWnd* pParent /*=NULL*/)
	: CResizableSheet("Construct : Runtime Debugger", pParent)
{
	// Add pages
}

void CDebugger::DoDataExchange(CDataExchange* pDX)
{
	CResizableSheet::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDebugger, CResizableSheet)
	//{{AFX_MSG_MAP(CDebugger)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOVING()
	ON_WM_SIZING()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CDebugger::OnInitDialog()
{
	CResizableSheet::OnInitDialog();

	ModifyStyle(0, WS_SIZEBOX);
	ModifyStyleEx(0, WS_EX_TOOLWINDOW);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	
	// set minimal size
	CRect rc;
	GetWindowRect(&rc);

	SetMinTrackSize(CSize(GetMinWidth(), rc.Height()));

	// enable save/restore, with active page
//	EnableSaveRestore(_T("Debugger"), TRUE, TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDebugger::OnMoving(UINT nSide, LPRECT lpRect)
{
	// Force runtime to refresh
	//pRuntime->Frame(true);
}

void CDebugger::OnSizing(UINT nSide, LPRECT lpRect)
{
	// Force runtime to refresh
	//pRuntime->Frame(true);
}

void CDebugger::OnClose()
{
	// Hide me
	ShowWindow(SW_HIDE);
}