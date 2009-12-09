#pragma once

#include "..\UI Elements\XListCtrl\XListCtrl.h"

// CAddFamilyVarsDlg dialog

class CAddFamilyVarsDlg : public CExtWS<CDialog>
{
	DECLARE_DYNAMIC(CAddFamilyVarsDlg)

public:
	CAddFamilyVarsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddFamilyVarsDlg();

	BOOL OnInitDialog();

	CXListCtrl m_VarsList;
	CExtEdit m_ObjName;
	CExtEdit m_FamilyName;

	Family* pFamily;
	CObjType* pType;

	int totalCheckboxes;

	vector<CString> varsToAdd;

// Dialog Data
	enum { IDD = IDD_ADDFAMILYVARS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
