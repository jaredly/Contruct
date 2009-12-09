#pragma once
 
// CACEFilterDlg dialog

class CACEFilterDlg : public CDialog
{
	DECLARE_DYNAMIC(CACEFilterDlg)

public:
	CACEFilterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CACEFilterDlg();

	CExtEdit	m_Search;
	CListCtrl	m_Results;
	CExtButton	m_Filter;

	CImageList	m_Images;

	bool		m_bGame;

// Dialog Data
	enum { IDD = IDD_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnPaint() ;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedFilter();
	afx_msg void OnNMDblclkResults(NMHDR *pNMHDR, LRESULT *pResult);
};
