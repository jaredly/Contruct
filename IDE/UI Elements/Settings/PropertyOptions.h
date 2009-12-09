#if !defined(AFX_PROPPAGE2_H__C001F710_48E9_46CD_BC19_4C184481E21F__INCLUDED_)
#define AFX_PROPPAGE2_H__C001F710_48E9_46CD_BC19_4C184481E21F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoSave.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoSave dialog

class CAutoSave : public CPropertyPage
{
	DECLARE_DYNCREATE(CAutoSave)

// Construction
public:
	CAutoSave();
	~CAutoSave();

// Dialog Data
	//{{AFX_DATA(CAutoSave)
	enum { IDD = IDD_PROPERTY_PAGE_2 };
	CExtButton m_Clear;
	CExtButton m_Browse;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAutoSave)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	// Settings
	bool	m_bAutoSave;
	int		m_AutoSaveTime;

	CButton m_AutoSave;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAutoSave)
	afx_msg void OnClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGE2_H__C001F710_48E9_46CD_BC19_4C184481E21F__INCLUDED_)
