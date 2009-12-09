#pragma once

// Scintilla Header
#include "Scintilla\ScintillaWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CScriptEditor view

class CScriptEditor : public CView
{
public:
	CScriptEditor();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScriptEditor)  

// Attributes
public:
	CLayout*		m_pLayout;
	CApplication*		m_app;
	BOOL		m_bIsCreated;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptEditor)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

	void UpdateScrollBars();

CScintillaWnd scintWin;
HMODULE m_hDll;

// Implementation
protected:
	virtual ~CScriptEditor();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CScriptEditor)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnGenerate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};