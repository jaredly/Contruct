#pragma once
#include "afxwin.h"

// CAddVariableDlg dialog
class CAddVariableDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CAddVariableDlg)

public:
	CAddVariableDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddVariableDlg();
	BOOL OnInitDialog();

	CExtButton m_OK, m_Cancel;
	CCtrlMessageBar	tips;

// Dialog Data
	enum { IDD = IDD_ADDVARIABLE };

	CString Name, Value, Caption;
	int Type;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Name;
	CEdit m_Value;
	afx_msg void OnBnClickedOk();

	CComboBox m_Type;
};
