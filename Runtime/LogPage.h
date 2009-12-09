#pragma once
#include "afxcmn.h"

#include "Tree\ResizablePage.h"

// A log message to appear in the logger.
struct LogMessage {
	CString message;
	int line;
	int cnd;
	int act;
	int count;

	// Compare without count
	bool operator==(const LogMessage& r) const {
		return message == r.message && line == r.line && cnd == r.cnd && act == r.act;
	}
};

class CLogPage : public CResizablePage
{
	DECLARE_DYNCREATE(CLogPage)

// Construction
public:
	CLogPage();
	~CLogPage();

	CRuntime* pRuntime;

	void AddLogMessage(CString msg, int evNum, int cndNum, int actNum);

	vector<LogMessage> log;
	bool active;
	bool pausedUpdating;

// Dialog Data
	//{{AFX_DATA(CMyPropertyPage1)
	enum { IDD = IDD_LOGPAGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMyPropertyPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMyPropertyPage1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CXListCtrl	m_Log;
	CStatic		m_Info;
	void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
};