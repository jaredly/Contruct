#if !defined(AFX_COLORMIXER_H__705C10B9_0BBB_4B47_A048_04ED59AAF90F__INCLUDED_)
#define AFX_COLORMIXER_H__705C10B9_0BBB_4B47_A048_04ED59AAF90F__INCLUDED_

//#include "..\CXIMAGE\CXIMAGE\ximage.h"	// Added by ClassView

#include "..\..\UI Elements\EditSpinSlider.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorMixer.h : header file
//
//#include "..\Bars\MyBar.h"
//#include ".\.\.\UI Elements\MyBar.h"
/////////////////////////////////////////////////////////////////////////////
// CColorMixer window

class CImageEditor;

class CColorMixer : public CDialog//CExtResizableDialog
{
// Construction
public:
	CColorMixer();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorMixer)
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	//}}AFX_VIRTUAL
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompareValuesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
public:

	BOOL PreTranslateMessage(MSG* pMsg);



// Dialog Data
	//{{AFX_DATA(CImageEditorDlg)
	enum { IDD = IDD_COLORMIXER};
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CEditSpinSlider	m_GreenBox;
	CEditSpinSlider	m_RedBox;
	CEditSpinSlider	m_BlueBox;

	bool lock_field_change;

	void OnTextFieldChange();
	//}}AFX_DATA









	bool m_rainbow_click;
	bool m_grad_click;
	void FindRainbowPos();
	void FindGradPos();
	int m_grad_pos;
	int m_rainbow_y;
	int m_rainbow_x;
	CDialogBar* m_pwndPalette;


	COLORREF* pColor;

	bool m_leftdown;

	COLORREF* pColor1;
	COLORREF* pColor2;

	void SetColor( COLORREF c );
	COLORREF GetColor();
	void UpdateEditBoxs();
	bool UseHSL;
	CxImage Map;
	virtual ~CColorMixer();
	bool OnButtonDown(UINT nFlags, CPoint point) ;
	CImageEditor*	m_pImageView;

	friend class CImageEditor;	
	// Generated message map functions
protected:
	//{{AFX_MSG(CColorMixer)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSwap();
	void UpdateColour();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORMIXER_H__705C10B9_0BBB_4B47_A048_04ED59AAF90F__INCLUDED_)
