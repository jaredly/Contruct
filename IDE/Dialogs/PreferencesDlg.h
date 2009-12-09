#pragma once

// CPreferencesDlg dialog
class CPreferencesDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CPreferencesDlg)

public:
	CPreferencesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPreferencesDlg();

// Dialog Data
	enum { IDD = IDD_PREFERENCES };
	void OnOK();
	BOOL OnInitDialog();

//	CExtPropertyGridCtrl m_PGC;
	CExtLabel				 m_Preferences;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
