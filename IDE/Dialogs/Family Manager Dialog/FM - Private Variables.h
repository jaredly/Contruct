#pragma once

class FamilyManagerDialog;

class CFMPrivateVariables : public CPropertyPage
{
	DECLARE_DYNAMIC(CFMPrivateVariables)

public:
	CFMPrivateVariables(CWnd* pParent = NULL);   // standard constructor

	// Family data
	void ShowFamily(Family* pFamily);
	Family* m_pFamily;

	// Standard items
	CListCtrl			m_Items;
	CExtButton			m_Add;
	CExtButton			m_Remove;

	// FamilyManagerDlg
	FamilyManagerDialog*	m_pParent;

// Dialog Data
	enum { IDD = IDD_FMPRIVATEVARIABLES };

	afx_msg BOOL OnSetActive();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnRemove();
	afx_msg void OnAdd();
	afx_msg void OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
};
