#pragma once
#include "afxcmn.h"
#include "..\..\UI Elements\XListCtrl\XListCtrl.h"
#include "..\ManageVariableDlg.h" //defines PrivateVariableInstanceDeleter
#include "..\AddVariableDlg.h" //for adding variables

// CRemoveFamilyVarDlg dialog
class CRemoveFamilyVarDlg : public CExtWS<CDialog>
{
	DECLARE_DYNAMIC(CRemoveFamilyVarDlg)

public:
	CRemoveFamilyVarDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRemoveFamilyVarDlg();

	BOOL OnInitDialog();
	CApplication* m_app;

// Dialog Data
	enum { IDD = IDD_REMOVEFAMILYVAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CXListCtrl m_Objects;

	CString varName;
	vector<CObjType*> objectsToList;
public:
	afx_msg void OnBnClickedOk();
};
