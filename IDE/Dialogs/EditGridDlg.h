#pragma once

// CEditGridDlg dialog
class CEditGridDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CEditGridDlg)

public:
	CEditGridDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditGridDlg();

// Dialog Data
	enum { IDD = IDD_PICED_RESIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExtButton m_OK;
	CExtButton m_Cancel;
	CExtButton m_SnapTo;
	CExtEdit m_Height;
	CExtEdit m_Width;

	CString m_OldWidth, m_OldHeight;
	bool m_OldSnapTo;

	afx_msg void OnOK();
	afx_msg BOOL OnInitDialog();
};
