#pragma once

// CPythonLibs dialog
class CPythonLibs : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CPythonLibs)

public:
	CPythonLibs(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPythonLibs();

	CApplication* application;

// Dialog Data
	enum { IDD = IDD_PYTHONLIBS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CCheckListBox	m_List;
	CExtButton		m_OK, m_Cancel, m_All, m_None;

	DECLARE_MESSAGE_MAP()
	
	BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAll();
	afx_msg void OnBnClickedNone();
};
