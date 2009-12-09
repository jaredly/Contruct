#pragma once

#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"

#include "..\Utilities\Helper.h"

// Updates an object's private variable to a new value.
class PrivateVariableInstanceDeleter : public ObjectFunctor {
public:
	int pvIndex;

	PrivateVariableInstanceDeleter() {}
	PrivateVariableInstanceDeleter(int _index) : pvIndex(_index) {}

	void operator()(CObj* pObj)
	{
		// Simply erase the private variable from the instance
		pObj->m_values.erase(pObj->m_values.begin() + pvIndex);
	}
};

class CManageVariableDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CManageVariableDlg)

public:
	CManageVariableDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManageVariableDlg();

	CExtButton m_Add, m_Remove, m_Edit, m_Close, m_Cancel, m_MoveUp, m_MoveDown;
	BOOL OnInitDialog();
	afx_msg void OnBnClickedAdd();
	CListCtrl m_List;

	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;

	CObjType* pType;
	CLayout* layout;
	CApplication* application;

	bool globalsMode;	// Modifying global vars instead of object vars.

	void ConfirmDeleteSelectedItems();
	CString GetTypeName(int type);
	int GetNameType(CString type);
	bool VariableNameExists(CString name);
	bool VariableHasMultipleValues(CString name, CString& val);

	void RefreshVariablesList();

// Dialog Data
	enum { IDD = IDD_MANAGEVARIABLES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedRename();
	afx_msg void OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedMoveup();
	afx_msg void OnBnClickedMovedown();
public:
	afx_msg void OnDblClick(NMHDR *pNMHDR, LRESULT *pResult);
};
