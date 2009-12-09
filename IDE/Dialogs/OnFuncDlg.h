#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// COnFuncDlg dialog

class COnFuncDlg : public CDialog
{
	DECLARE_DYNAMIC(COnFuncDlg)

public:
	COnFuncDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COnFuncDlg();

	bool ready;
// Dialog Data
	enum { IDD = IDD_ONFUNCTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult);

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;

public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedAdd();
public:
	afx_msg void OnBnClickedRemove();
public:
	afx_msg void OnBnClickedUp();
public:
	afx_msg void OnBnClickedDown();
public:
	CListCtrl m_Objects;
public:

	void SetMap(CObjTypeMap* objTypeMap) { objMap = objTypeMap; }
	// Object map
	CObjTypeMap* objMap;

	// Imagelists
	CImageList imageList;

	void LoadObjects();
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedOk2();
public:
	afx_msg void OnBnClickedOk3();
public:
	CTreeCtrl m_Params;
public:
	CListBox m_Functions;
	void RefreshFunctions();
	bool SystemSelected();
	CObjType* GetNonSystemObject();
	bool ActionFunctions();
	bool ConditionFunctions();
	bool ExpressionFunctions();
	CLayout*	m_pLayout;
	vector<ACESEntry2>* COnFuncDlg::GetACEsVector();
	bool ItemSelected();
	void ShowControls(bool show);
public:
	afx_msg void OnLvnItemchangedObjects(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLbnSelchangeList2();
};
