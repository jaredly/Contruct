#if !defined(AFX_EDITSPINSLIDER_H__B6DA75E6_2CFC_428D_A361_BB7AC2D2A1FD__INCLUDED_)
#define AFX_EDITSPINSLIDER_H__B6DA75E6_2CFC_428D_A361_BB7AC2D2A1FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//class CEditSpinSliderPopup;

class CEditSpinSlider;






/////////////////////////////////////////////////////////////////////////////
// CEditSpinSliderPopup window

class CEditSpinSliderPopup : public CWnd
{
// Construction
public:
	CEditSpinSliderPopup();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditSpinSliderPopup)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	void CreateGradientBG(COLORREF left, COLORREF right);

	int m_value;
	virtual ~CEditSpinSliderPopup();
	CBitmap m_Bg;

	int			m_BGtype;
	COLORREF	m_BGColorLeft;
	COLORREF	m_BGColorRight;

	CEditSpinSlider* m_Parent;
	// Generated message map functions
protected:
	//{{AFX_MSG(CEditSpinSliderPopup)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};



















// EditSpinSlider.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditSpinSlider window

class CEditSpinSlider : public CEdit
{
 friend class CEditSpinSliderPopup;
// Construction
public:
	CEditSpinSlider();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditSpinSlider)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	void Refresh();
	bool before;
	void CreateGradientBG(COLORREF left, COLORREF right);
	COLORREF m_Color;
	CEditSpinSliderPopup m_wndPopup;
	int m_Min;
	int m_Max;
	int m_downOn;
	int m_speed;

	virtual ~CEditSpinSlider();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditSpinSlider)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcPaint();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSPINSLIDER_H__B6DA75E6_2CFC_428D_A361_BB7AC2D2A1FD__INCLUDED_)
