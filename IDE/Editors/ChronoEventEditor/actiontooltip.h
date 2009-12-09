#if !defined(AFX_ACTIONTOOLTIP_H__9BF4E026_40B9_40AD_8539_1BEF4200353B__INCLUDED_)
#define AFX_ACTIONTOOLTIP_H__9BF4E026_40B9_40AD_8539_1BEF4200353B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionTooltip.h : header file
//
#include "ChronoEventEditor.h"

class CEditorEvent;
class CEditorAction;




typedef BOOL (WINAPI *lpfnUpdateLayeredWindow)(HWND hWnd, HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc, 
							POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);


typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)(HWND hWnd, COLORREF cr,BYTE bAlpha, DWORD dwFlags);
																								




class CChronoEventEditor;








/////////////////////////////////////////////////////////////////////////////
// CActionTooltip window


class CActionTooltip_BaseButton: public CButton
{
public:
	void Create(int x, int y, int w, int h, CWnd* parent,CEditorAction* plAction)
	{
		CButton::Create("",WS_CHILD|WS_VISIBLE,CRect(x,y,x+w,y+h),parent,100);
		pAction = plAction;
	}
	CEditorAction* pAction;
};


class CActionTooltip_EditButton: public CActionTooltip_BaseButton
{


public:
	void OnClicked();

	DECLARE_MESSAGE_MAP()

};



class CActionTooltip_DeleteButton: public CActionTooltip_BaseButton
{

public:
	void OnClicked();

	DECLARE_MESSAGE_MAP()
};


class CEditorObject;
class CEditorHeaderTooltip: public CWnd
{
// Construction
public:
	CEditorHeaderTooltip();
	virtual ~CEditorHeaderTooltip();
// Attributes
public:
	CChronoEventEditor* m_pChrono;
	CEditorObject* m_object;

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);





// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionTooltip)
	//}}AFX_VIRTUAL

// Implementation
public:

	bool m_redraw;
	CPoint m_pt;
	bool m_created;
	void Hide();
	void ShowAt(CPoint pt, CWnd& Parent);


	int m_fade;

	lpfnUpdateLayeredWindow			m_pUpdateLayeredWindow;
	lpfnSetLayeredWindowAttributes	m_pSetLayeredWindowAttributes;
	BLENDFUNCTION m_Blend;


	// Generated message map functions
protected:
	//{{AFX_MSG(CActionTooltip)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()




};


class CHoverButton : public CWnd
{
public:
	CString text;
	// todo: other stuff

	void (*m_pFunction)();
	void SetOnClick(void (*pFunction)());
	void Hide();
	CHoverButton();
	CPoint m_pt;
	CPoint m_old; // for mouse
	lpfnUpdateLayeredWindow			m_pUpdateLayeredWindow;
	lpfnSetLayeredWindowAttributes	m_pSetLayeredWindowAttributes;
	BLENDFUNCTION m_Blend;

	EventSheetEditor*	pEventEditor;
	CEditorEvent*		m_pInsertEventSpot;

	int m_InsertType;
	int m_InsertIndex;
	bool m_IsAfter;
	bool m_IsSub;


	void ShowAt(CPoint pt,CEditorCondition* pCondition, CWnd& Parent);
	void ShowAt(CPoint pt,CEditorAction* pCondition, CWnd& Parent);
	void ShowAt(CPoint pt,CEditorEvent* pEvent, CWnd& Parent);
	void OnLButtonDown(UINT nFlags, CPoint point);

protected:
	//{{AFX_MSG(CActionTooltip)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};

class CActionTooltip : public CWnd
{



public:
	vector<CActionTooltip_EditButton*> m_edit_buttons;
	vector<CActionTooltip_DeleteButton*> m_delete_buttons;

// Construction
public:
	CActionTooltip();
	virtual ~CActionTooltip();
// Attributes
public:
	EventSheetEditor* m_pChrono;
	SelectedActionVector m_actions;
	vector<int> m_actionHeights;
	
	void Hide();





// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionTooltip)
	//}}AFX_VIRTUAL

// Implementation
public:

	bool m_clickedSelected;
	CEditorAction* ActionAt(int y);
	bool* m_CtrlDown;
	bool m_redraw;
	CPoint m_pt;
	bool m_created;
	void ShowAt(CPoint pt,SelectedActionVector& actions, CWnd& Parent);


	int m_fade;

	lpfnUpdateLayeredWindow			m_pUpdateLayeredWindow;
	lpfnSetLayeredWindowAttributes	m_pSetLayeredWindowAttributes;
	BLENDFUNCTION m_Blend;


	// Generated message map functions
protected:
	//{{AFX_MSG(CActionTooltip)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONTOOLTIP_H__9BF4E026_40B9_40AD_8539_1BEF4200353B__INCLUDED_)
