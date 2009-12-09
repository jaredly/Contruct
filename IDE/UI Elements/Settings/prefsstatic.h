#if !defined(AFX_PREFSSTATIC_H__1B15B006_9152_11D3_A10C_00500402F30B__INCLUDED_)
#define AFX_PREFSSTATIC_H__1B15B006_9152_11D3_A10C_00500402F30B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PrefsStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrefsStatic window

class CPrefsStatic : public CStatic
{
// Construction
public:
	CPrefsStatic();

// Attributes
public:

// Operations
public:
   CString m_csFontName;

	void		SetConstantText(const char *pText)	{m_csConstantText = pText;}

   int m_fontSize, m_fontWeight;
   BOOL m_grayText;
   COLORREF m_textClr;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrefsStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPrefsStatic();

protected:
   CFont m_captionFont, m_nameFont;

   CBitmap m_bm;

	CString m_csConstantText;
	

   void MakeCaptionBitmap();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPrefsStatic)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnSetText (WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFSSTATIC_H__1B15B006_9152_11D3_A10C_00500402F30B__INCLUDED_)
