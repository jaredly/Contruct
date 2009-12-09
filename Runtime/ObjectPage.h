#pragma once

#include "DebuggerDlg.h"
#include "afxcmn.h"
#include "afxwin.h"

#include "Tree\ResizablePage.h"

enum ObjectInspectionArea {
	INSPECT_NONE,
	INSPECT_COMMON,
	INSPECT_PROPERTIES,
	INSPECT_PRIVATEVARS,
	INSPECT_MOVEMENT,
	INSPECT_ALL
};

class CObjectsPage : public CResizablePage
{
	DECLARE_DYNCREATE(CObjectsPage)

// Construction
public:
	CObjectsPage();
	~CObjectsPage();

	class CRuntime* pRuntime;
	friend class CRuntime;

	// Current object to inspect
	CRunObject* pMyObject;
	ObjectInspectionArea inspectArea;
	ExpStore* pCurrentVars;
	CRunObject* pCurMovement;

	// Object that owns the current debugger line being added
	CRunObject* pCurLineOwner;
	vector<CRunObject*> lineOwners;		// Objects that own their respective lines

	// Current object type to inspect
	CRunObjType* pMyType;

	void SetObjectToInspect(CRunObject* obj, ObjectInspectionArea area);
	void SetTypeToInspect(CRunObjType* type);

	void AddCommonInspectItems(CRunObject* pObj);
	void AddPrivateVariableInspectItems();

	void RefreshInspection();
	void UpdateObjectTree();

	vector<CString> hiddenTypes;
	map<CString, HTREEITEM> typeToItemMap;

	void AddListHeader(CString text);
	void AddListLine(CString item, CString value, bool readOnly, bool bold = false, bool combo = false, int comboSel = -1);
	void AddListLine(CString item, int value, bool readOnly, bool bold = false);

	LRESULT OnEditEnd(WPARAM nItem, LPARAM nSubItem);

	vector<DebuggerInspectionLine> objDebugDisplay;

// Dialog Data
	//{{AFX_DATA(CMyPropertyPage1)
	enum { IDD = IDD_OBJECTSPAGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
	CXListCtrl m_List;
	void OnRButtonDown(NMHDR* pNMHDR, LRESULT* pResult);
	void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	void OnPause();
	void OnStep();
	void OnRestart();

	CDebugger* m_pDebugger;
	class CWatchPage* pWatchPage;

	bool pausedUpdating;
	bool suspended;
	bool invalidateNextRefresh;	// To invalidate tree on next refresh
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMyPropertyPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	public:
	virtual BOOL OnSetActive();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMyPropertyPage1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CTreeCtrl m_ItemTree;
public:
	afx_msg void OnTvnSelChangedTree(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CButton m_PauseBtn;
public:
	CButton m_Step;
public:
	afx_msg void OnRClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRClickTree(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnListKeydown(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CComboBox m_FramesCombo;
public:
	afx_msg void OnSelchangeFrames();
};