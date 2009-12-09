#pragma once

#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"

class CEmptyListCtrl : public CListCtrl
{
// Construction
public:
	CFont m_Font;

	CEmptyListCtrl()
	{
		m_Font.CreateFont(14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
	}

protected:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CObjectInfoDlg dialog

class CObjectInfoDlg : public CExtNCW<CExtResizableDialog>
{
// Construction
public:
	CObjectInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectInfoDlg)
	enum { IDD = IDD_OBJECTINFO };
	CExtButton		m_Close;
	CExtComboBox	m_AceType;
	CEmptyListCtrl	m_AceList;
	CExtEdit		m_Info;
	CExtLabel			m_Icon;
	CExtLabel			m_Name;

	// Object
	CString objname;

	// Resizable
	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;   

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnChangeACE();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};