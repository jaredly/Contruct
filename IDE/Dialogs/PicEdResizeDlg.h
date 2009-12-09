#if !defined(AFX_PICEDRESIZEDLG_H__162B7386_5462_49D3_AF86_28A47259C14A__INCLUDED_)
#define AFX_PICEDRESIZEDLG_H__162B7386_5462_49D3_AF86_28A47259C14A__INCLUDED_

#include "..\Editors/CPictureEditor.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PicEdResizeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPicEdResizeDlg dialog

class CPicEdResizeDlg : public CExtNCW<CExtResizableDialog>
{
// Construction
public:
	void EditHeightPercentage();
	void EditWidthPercentage();
	void EditHeight();
	void EditWidth();
	CPictureEditor* m_pPicEd;
	CPicEdResizeDlg(CWnd* pParent = NULL);   // standard constructor

	int m_height;
	int m_width;

// Dialog Data
	//{{AFX_DATA(CPicEdResizeDlg)
	enum { IDD = IDD_PICED_RESIZE };
	CExtButton	m_Cancel;
	CExtButton	m_Resize;
	CExtButton	m_Reset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicEdResizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPicEdResizeDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICEDRESIZEDLG_H__162B7386_5462_49D3_AF86_28A47259C14A__INCLUDED_)
