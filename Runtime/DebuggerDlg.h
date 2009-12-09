#pragma once

#include "Tree\ResizableSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CDebuggerDlg dialog

class CDebugger : public CResizableSheet
{
// Construction
public:
	CDebugger(CWnd* pParent = NULL);	// standard constructor
	enum { IDD = IDD_DEBUGGER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	// Pointer to runtime
	class CRuntime* pRuntime;
	friend class CRuntime;

// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnMoving(UINT nSide, LPRECT lpRect);
	virtual void OnSizing(UINT nSide, LPRECT lpRect);
	virtual void OnClose();

	DECLARE_MESSAGE_MAP()
};
