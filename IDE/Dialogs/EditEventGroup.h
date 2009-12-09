#pragma once


#include "..\..\Utilities\anchor.h"
#include "..\..\Utilities\dlgman.h"
// CEditEventGroup dialog

class CEditEventGroup : public CExtWS<CDialog>
{
	DECLARE_DYNAMIC(CEditEventGroup)

public:
	CEditEventGroup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditEventGroup();

	virtual void OnOK();

	bool m_Active;
	CString m_Title;
	CString m_Description;

	CCtrlMessageBar	tips;
	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;   

	CExtButton m_OK, m_Cancel;
	BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_EGROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void OnDestroy();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
