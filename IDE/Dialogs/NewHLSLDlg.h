#pragma once

#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"

class CAddEffect
{
public:
	CString Name;
	CString Author;
	CString Description;
	double	Version;
};

// CNewHLSLDlg dialog
class CNewHLSLDlg : public CExtNCW<CExtResizableDialog>
{
public:
	CNewHLSLDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewHLSLDlg();
	CListCtrl	m_List;
	CExtButton	m_Cancel;
	CExtButton	m_OK;
	CExtButton	m_Editor;
	CExtEdit	m_Description;

	vector<CAddEffect> m_Effects;

	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;   
	CCustomTabCtrl m_Tabs;

	float m_Version;
	
	CString m_Text, m_Filename;

	void OnChangeCategory();

// Dialog Data
	enum { IDD = IDD_NEWHLSL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
};
