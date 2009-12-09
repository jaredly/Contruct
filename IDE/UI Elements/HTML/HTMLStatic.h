#if !defined(AFX_HTMLSTATIC_H__7748F58D_2B68_44C7_84FC_4DDDE152C9AF__INCLUDED_)
#define AFX_HTMLSTATIC_H__7748F58D_2B68_44C7_84FC_4DDDE152C9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HTMLStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHTMLStatic window

#include "HTMLDrawer.h"
#include "HTMLFont.h"

class CHTMLStatic : public CStatic
{
// Construction
public:
	CHTMLStatic();

// Attributes
protected:
	COLORREF	m_rgbBackground;
	COLORREF	m_rgbHighlight;
	bool		m_isUnderlineWhenHighlight;

	bool		m_isTextPrepared;
	CHTMLDrawer m_htmlDrawer;
	CHTMLFont	m_Font;
	CString		m_strScript;
	HCURSOR		m_hLinkCursor;
	HCURSOR		m_hPrevCursor;
	
	
	bool		m_isAtomHighlighted;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHTMLStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHTMLStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHTMLStatic)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnClicked();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	void SetDefaultCursor();
	HINSTANCE GotoURL(LPCTSTR url, int showcmd);
	LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
	void ReportError(int nError);

public:

	void SetScript(LPCTSTR strScript)
	{
		m_strScript = strScript;
		m_isTextPrepared = false;
		Invalidate(true);
	}

	const CHTMLFont & GetFont() const
	{
		return m_Font;
	}
	void  SetFont(const CHTMLFont & Font)
	{
		m_isTextPrepared = false;
		m_Font = Font;
		Invalidate(true);
	}
	//
	const CString& GetScript() const 
	{
		return m_strScript;
	}
	//
	COLORREF GetBackgroundColour() const 
	{
		return m_rgbBackground;
	}
	void SetBackgroundColour(COLORREF nBackground)
	{
		m_rgbBackground = nBackground;
	}
	//
	COLORREF GetHighlightColour() const 
	{
		return m_rgbHighlight;
	}
	void SetHighlightColour(COLORREF nHighlight)
	{
		m_rgbHighlight = nHighlight;
	}
	//
	bool IsUnderlineWhenHighlight() const 
	{
		return m_isUnderlineWhenHighlight;
	}
	void SetUnderlineWhenHighlight(bool isUnderlineWhenHighlight)
	{
		m_isUnderlineWhenHighlight = isUnderlineWhenHighlight;
	}
	//

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLSTATIC_H__7748F58D_2B68_44C7_84FC_4DDDE152C9AF__INCLUDED_)
