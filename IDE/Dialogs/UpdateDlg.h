#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "..\Utilities\CWebUpdate.h"

#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"

// CUpdateDlg dialog

class CUpdateDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CUpdateDlg)

public:
	CUpdateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUpdateDlg();

	CWebUpdate m_UpdateObj;

	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;

// Dialog Data
	enum { IDD = IDD_UPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl	m_Progress;
	CListCtrl		m_Files;
	CExtButton		m_Download, m_Cancel;
	HRESULT			hr;

	CStringArray	m_URLs;
	CStringArray	m_Local;

	// Download in progress
	CString			m_RemoteURL;
	CString			m_LocalFile;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDownload();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};
