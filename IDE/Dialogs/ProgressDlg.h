#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "..\resource.h"


// CProgressDlg dialog

class CProgressDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CProgressDlg)

	BOOL OnInitDialog();

public:
	CProgressDlg(CString status = "", CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressDlg();

	void SetProgress(int prog);
	void Start(CString status = "");
	void Finish();

// Dialog Data
	enum { IDD = IDD_PROGRESSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExtLabel m_Status;
	CProgressCtrl m_Progress;
	CString m_StatusText;

	int progress;
};
