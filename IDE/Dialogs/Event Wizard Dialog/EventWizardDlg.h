#pragma once

// Application structure for Object map
#include "..\..\Structure.h"

#include "..\..\Utilities\anchor.h"
#include "..\..\Utilities\dlgman.h"
#include "..\..\UI Elements\grouplistctrl.h"
#include "IconEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CNewTriggerDlg dialog

class TriggerItemData
{
public:

	TriggerItemData()
	{
		oT = 0;
		family = 0;
	}

	CObjType*	oT;
	Family*	family;
};

class CParametersDlg;

class CEventWizardDlg : public CDialog
{
// Construction
public:
	CEventWizardDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewTriggerDlg)
	enum { IDD = IDD_EVENTWIZARD };

	void UpdateButtons();
	CExtButton		m_Cancel;
	CExtButton		m_Next;
	CExtButton		m_Back;
	CIconEdit		m_Find;

	CGroupListCtrl	m_List;
	CGroupListCtrl	m_ACEList;

	CCtrlMessageBar	first_run;

	//////////////////////////////////
	// New to 0.96.3
	//////////////////////////////////
	void SetStage(int Stage, int ObjID, CString Name)
	{
		m_InitialStage = Stage;
		m_InitialSelect = ObjID;
		m_OldName = Name;
	}

	int		m_InitialStage;
	int		m_InitialSelect;
	CString	m_OldName;
	///////////////////////////////////

	ACESEntry2* pAce;

	CFont m_Font;
	CFont m_FontBold;

	//}}AFX_DATA

	CLayout*		layout;
	CApplication*	application;

	long m_pSelectedObject;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewTriggerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;   

	void OnSwitchTab();
public:
	MenuMapItem chosenItem;

public:
	// Object map foshizzle
	CObjTypeMap* objMap;
	void SetMap(CObjTypeMap* objTypeMap) { objMap = objTypeMap; }
	vector<CObjType*> m_otList;
	map<CString, int> object_folders;
	
	// Dialog information
	int			m_aceType; 
	int			m_iStep;
	int aceIndex;

	// To prevent temp obj types going out of scope
	CObjType tmp;

	bool	    PreChangeStep();
	void		ChangeStep();

	// Chosen item data
	bool		bFamily;
	CString		CFamily;
	CStringArray param_strings;		// parameters
	vector<int>  param_types;
	int m_aceIndex;
	int m_childid;			// -1 = normal, 0 = movement,  -2, -3 = effect etc etc

	// Images
	CImageList m_ObjectImages;


	class CCategoryTab : public CExtTabFlatWnd
	{
		virtual bool OnTabWndSelectionChange(LONG nOldItemIndex, LONG nNewItemIndex, bool bPreSelectionTest)
		{
			bool bRetVal = CExtTabFlatWnd::OnTabWndSelectionChange(nOldItemIndex, nNewItemIndex, bPreSelectionTest);
			if( bRetVal )
			{  
				LRESULT End = 0;
				((CEventWizardDlg*)GetParent())->OnSwitchTab();
			}
			return bRetVal;
		}
	} m_CategoryTabs; // all, common, cat1, cat2,  etc

	int tabSelectionIndex;

	class CTab : public CExtTabFlatWnd
	{
		virtual bool OnTabWndSelectionChange(LONG nOldItemIndex, LONG nNewItemIndex, bool bPreSelectionTest)
		{
			bool bRetVal = CExtTabFlatWnd::OnTabWndSelectionChange(nOldItemIndex, nNewItemIndex, bPreSelectionTest);
			if( bRetVal )
			{  
				LRESULT End = 0;
				CEventWizardDlg* pDlg = (CEventWizardDlg*)GetParent();
				pDlg->tabSelectionIndex = nNewItemIndex;
				pDlg->OnSwitchTab();
			}
			return bRetVal;
		}
	} m_Tabs; // object, movement 1, movement 2, effect 1, etc
	

	bool m_locked;

	CParametersDlg* m_Params;

	void UpdateAceList();

	// New - filter
	CString m_Filter;
	void UpdateDisplay(CString Previous, bool addTabs = true);
	bool ApproveItem(CString Item);

	bool m_bOK;
	CString SelectedCategory;
	map<CString, int> categories;
	CString SelectedTabText;
	int SelectedTabIndex;
	CBehavior* pMov;
	CEffect* pEff;

	// Crude hacks by Ashley
	CObjType* step2_type;

	void OnBnClickedNextClicked(){OnBnClickedNext();}
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewTriggerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedBack();
	afx_msg void OnPaint();
	afx_msg void OnHdnItemdblclickObjectlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnUpdateButtons(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFilterChange();
};