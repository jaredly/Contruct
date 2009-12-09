#pragma once

class FamilyManagerDialog;

class CFMBehaviors : public CPropertyPage
{
	DECLARE_DYNAMIC(CFMBehaviors)

public:
	CFMBehaviors(CWnd* pParent = NULL);   // standard constructor

	// Family data
	void ShowFamily(Family* pFamily);
	Family* m_pFamily;

	// Standard items
	CListCtrl			m_Items;
	CExtButton			m_Remove;

	// FamilyManagerDlg
	FamilyManagerDialog*	m_pParent;

// Dialog Data
	enum { IDD = IDD_FMBEHAVIORS };

	afx_msg BOOL OnSetActive();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnRemove();
	afx_msg void OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
};
