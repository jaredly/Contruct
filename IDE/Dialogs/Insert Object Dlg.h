#pragma once

#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"
#include "..\UI Elements\grouplistctrl.h"
#include "Popup dialogs\ACE Filter Dlg.h"

/////////////////////////////////////////////////////////////////////////////
// CInsertObject dialog

class InsertObjectDialog : public CExtNCW<CExtResizableDialog>
{
	friend class CCategoryTab;

public:
	InsertObjectDialog(CApplication&);  

	enum { IDD = IDD_INSERTOBJECTS };

	CExtButton ok, cancel, exchange, list_objects, filter;
	CGroupListCtrl objects;
	CExtLabel tab_rect;
	CExtEdit info;
	CEdit name;
	CComboBox folders;

	bool onclicklisttimeractivated;

	CCtrlMessageBar	first_run;

	class CObjectTab : public CExtTabFlatWnd
	{
		virtual bool OnTabWndSelectionChange(LONG nOldItemIndex, LONG nNewItemIndex, bool bPreSelectionTest)
		{
			bool bRetVal = CExtTabFlatWnd::OnTabWndSelectionChange(nOldItemIndex, nNewItemIndex, bPreSelectionTest);
			if( bRetVal )
			{  
				LRESULT End = 0;
				((InsertObjectDialog*)GetParent())->OnSelchangeCategories();
			}
			return bRetVal;
		}
	}  tabs;

	// Resizable
	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;   

	long m_curSel;
	CxImage resizeIcon;

	// Object map
	CApplication& application;

	// Imagelists
	CImageList m_LargeImages, 
		       small_images;

	void LoadObjects();
	BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnTimer(UINT_PTR nIDEvent);

	CACEFilterDlg filter_dialog;


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeObjectView();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnRClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeCategory();
	afx_msg void OnSelchangeCategories();
	afx_msg void OnExchange();
	afx_msg void OnBnClickedFilter();
	afx_msg void OnSelected(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};