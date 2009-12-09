#if !defined(AFX_ADDCOMMENTDLG_H__94CD0C14_EDDB_4E7B_8A38_8972BEB5040D__INCLUDED_)
#define AFX_ADDCOMMENTDLG_H__94CD0C14_EDDB_4E7B_8A38_8972BEB5040D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddCommentDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddCommentDlg dialog

#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"

class CAddCommentDlg : public CExtNCW<CExtResizableDialog>
{
// Construction
public:
	CAddCommentDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddCommentDlg)
	enum { IDD = IDD_ECOMMENT };
	CExtButton m_Font;
	CEdit m_Comment;
	CExtButton m_Color;
	CExtButton m_Close;
	CExtButton m_Add;
	//}}AFX_DATA

	CString theComment;

	// Colour
	COLORREF colour;
	
	COLORREF fontcolour;

	LOGFONT lf;

	// Resizable
	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;   
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddCommentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddCommentDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnClose();
	afx_msg void OnFont();
	afx_msg void OnColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDCOMMENTDLG_H__94CD0C14_EDDB_4E7B_8A38_8972BEB5040D__INCLUDED_)
