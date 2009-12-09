#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "..\AddVariableDlg.h"
#include "FM - Private Variables.h"
#include "FM - Effects.h"
#include "FM - Behaviors.h"

// CFamilyManagaerDlg dialog
class FamilyManagerDialog : public CExtWS<CDialog>
{
	DECLARE_DYNAMIC(FamilyManagerDialog)

public:

	FamilyManagerDialog(CApplication*);  
	CApplication* application;

protected:
	
	enum { IDD = IDD_FAMILYMANAGER };

	void RefreshSelectedFamily(int ID);

	CImageList object_images;
	CImageList family_images;
	CListCtrl objects;
	CListCtrl families;
	CExtButton ok;
	CExtButton add;
	CExtButton remove;

	CPropertySheet property_sheet;
	CFMBehaviors behaviors_page;
	CFMEffects effects_page;
	CFMPrivateVariables	private_variables_page;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	BOOL OnInitDialog();
	afx_msg void OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
};
