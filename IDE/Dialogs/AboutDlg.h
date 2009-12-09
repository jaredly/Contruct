#ifndef _ABOUTDLG_H_
#define _ABOUTDLG_H_

class CAboutDlg : public CExtNCW<CExtResizableDialog>
{
public:
	CAboutDlg();


	enum { IDD = IDD_ABOUTBOX };
	CExtLabel	m_People;
	CExtLabel	m_BuildDate;

	CExtButton     m_OK;
	CExtGroupBox   m_Group;
	//}}AFX_DATA

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
