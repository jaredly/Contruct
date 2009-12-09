#pragma once
#include "..\Editors\LayoutEditor.h"

/////////////////////////////////////////////////////////////////////////////
// CArrayPasteDlg dialog

class CArrayPasteDlg : public CExtNCW<CExtResizableDialog>
{
// Construction
public:
	CArrayPasteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CArrayPasteDlg)
	enum { IDD = IDD_ARRAYPASTE };
	CExtButton m_OK;
	CExtButton m_Cancel;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CLayoutEditor *layout_editor;

	BOOL bMoveTotal, bRotateTotal, bScaleTotal, bMoveTotal2, bDuplicate, b2D, bDialogInitialized;
	long moveX, moveY, rotateX, scaleX, scaleY, moveX2, moveY2, count1, count2, objCount;
	CString amt;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArrayPasteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CArrayPasteDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnApDim1();
	afx_msg void OnApDim2();
	virtual void OnOK();
	afx_msg void OnDeltaposApCountDim1Spin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposApCountDim2Spin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeApCountDim1();
	afx_msg void OnChangeApCountDim2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};