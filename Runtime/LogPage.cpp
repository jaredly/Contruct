// MyPropertyPages.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "uxtheme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CLogPage, CResizablePage)

/////////////////////////////////////////////////////////////////////////////
// CLogPage property page
CString IntToStr(int x)
{
	CString s;
	s.Format("%d", x);
	return s;
}

CLogPage::CLogPage() : CResizablePage(CLogPage::IDD)
{
	//{{AFX_DATA_INIT(CLogPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	active = false;
	pausedUpdating = false;
}

CLogPage::~CLogPage()
{
}

void CLogPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogPage)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LOG, m_Log);
	DDX_Control(pDX, IDC_MESSAGE, m_Info);
}


BEGIN_MESSAGE_MAP(CLogPage, CResizablePage)
	//{{AFX_MSG_MAP(CLogPage)
	ON_NOTIFY(NM_CLICK, IDC_LOG, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CLogPage::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;
	}

	m_Info.SetWindowText(m_Log.GetItemText(nItem, 0));

	*pResult = 0;
}

BOOL CLogPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();

	m_Log.InsertColumn(0, "Message", 0, 150);
	m_Log.InsertColumn(1, "Event", 0, 50);
	//m_Log.InsertColumn(2, "Condition", 0, 50);	// TODO: inaccurate responses
	m_Log.InsertColumn(3, "Action", 0, 50);
	m_Log.InsertColumn(4, "Count", 0, 50);

	AddAnchor(IDC_LOG, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_MESSAGE, BOTTOM_LEFT, BOTTOM_RIGHT);

	return TRUE;
}

BOOL CLogPage::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_NEXT);
	active = true;
	BOOL ret = CResizablePage::OnSetActive();

	// Update the log
	m_Log.DeleteAllItems();

	vector<LogMessage>::iterator i = log.begin();

	for ( ; i != log.end(); i++) {
		int item = m_Log.GetItemCount();
		m_Log.InsertItem(item, i->message);
		m_Log.SetItemText(item, 1, i->line == -1 ? "n/a" : IntToStr(i->line));
		//m_Log.SetItemText(item, 2, i->cnd == -1 ? "n/a" : IntToStr(i->cnd));
		m_Log.SetItemText(item, 2, i->act == -1 ? "n/a" : IntToStr(i->act));
		m_Log.SetItemText(item, 3, IntToStr(i->line));
	}
	
	return ret;
}

BOOL CLogPage::OnKillActive()
{
	active = false;
	return CResizablePage::OnKillActive();
}

void CLogPage::AddLogMessage(CString msg, int evNum, int cndNum, int actNum)
{
	// Check for this message on this event/condition before
	LogMessage lm;
	lm.message = msg;
	lm.line = evNum;
	lm.cnd = cndNum;
	lm.act = actNum;

	// Has previously existed?
	vector<LogMessage>::iterator i = find(log.begin(), log.end(), lm);

	// Doesn't exist:  add new log line
	if (i == log.end()) {
		lm.count = 1;
		log.push_back(lm);

		CString evStr = (lm.line == -1 ? "n/a" : IntToStr(lm.line));
		CString actStr = (lm.act == -1 ? "n/a" : IntToStr(lm.act));

		// Update the log control if active
		if (active) {
			int item = m_Log.GetItemCount();
			m_Log.InsertItem(item, lm.message);
			m_Log.SetItemText(item, 1, evStr);
			//m_Log.SetItemText(item, 2, lm.cnd == -1 ? "n/a" : IntToStr(lm.cnd));
			m_Log.SetItemText(item, 2, actStr);
			m_Log.SetItemText(item, 3, "1");
		}

		// Messagebox the message if it's new and we're not fullscreen
#ifdef APPRUNTIME

		CString msg;
		msg.Format("Debugger first-time message:\n\n%s\n\nEvent: %s\nAction: %s", lm.message, evStr, actStr);
		::MessageBox(NULL, msg, "Debugger", MB_ICONEXCLAMATION | MB_OK);

#else
		if (!pRuntime->fullscreen) {
			CString msg;
			msg.Format("Debugger first-time message:\n\n%s\n\nEvent: %s\nAction: %s", lm.message, evStr, actStr);
			::MessageBox(NULL, msg, "Debugger", MB_ICONEXCLAMATION | MB_OK);
		}
#endif
	}
	// Exists: just update count
	else {
		i->count++;
		int item = i - log.begin();

		if (active)
			m_Log.SetItemText(item, 3, IntToStr(i->count));
	}
}