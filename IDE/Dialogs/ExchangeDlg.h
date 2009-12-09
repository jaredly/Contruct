#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CExchangeDlg dialog
class CExchangeDlg : public CDialog
{
	DECLARE_DYNAMIC(CExchangeDlg)

public:
	CExchangeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExchangeDlg();

// Dialog Data
	enum { IDD = IDD_EXCHANGE };
	BOOL OnInitDialog();

	vector<CString> m_URLs;
	CString m_RemoteURL;
	CString m_LocalFile;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	CExtButton m_Download;
	CProgressCtrl m_Progress;
	CExtButton m_OK;
public:
	afx_msg void OnBnClickedDownload();
};
