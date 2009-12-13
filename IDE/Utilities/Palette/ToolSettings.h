#if !defined(AFX_TOOLSETTINGS_H__942A5E43_DB0E_4D9B_9F38_D2F0E512C37E__INCLUDED_)
#define AFX_TOOLSETTINGS_H__942A5E43_DB0E_4D9B_9F38_D2F0E512C37E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolSettings.h : header file
//

#include "..\..\UI Elements\CaptionEditXP.h"
/////////////////////////////////////////////////////////////////////////////
// CToolSettings window



class CImageEditor;



class CToolSettings : public CExtPanelControlBar
{
// Construction
public:
	CToolSettings();
	void OnTimer(UINT_PTR nIDEvent);
// Attributes
public:


CSize OnQueryFixedBarNewSize(
	CExtPanelControlBar::e_fixed_bar_size_query_t eWhichSizeNeedToKnow
	) const
{
		ASSERT_VALID( this );
		return CSize(460,36);
}



// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolSettings)
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnBrushSizeChange();

	//int* pType;
	bool m_leftdown;
	int m_toolpressed;
	int m_toolhighlight;
	void ToolChanged();
	int* pTool;
	virtual ~CToolSettings();
	int m_type;
	CCaptionEditXP		m_BrushSize;
	CCaptionEditXP		m_BrushThickness;
	CCaptionEditXP		m_BrushHardness;
	CCaptionEditXP		m_BrushAngle;
	CCaptionEditXP		m_BrushStep;
	CCaptionEditXP		m_LineThickness;
	CCaptionEditXP		m_Opacity;
	CCaptionEditXP		m_Flow;
	CCaptionEditXP		m_Alpha1;
	CCaptionEditXP		m_Alpha2;

	COutlineFillOption	m_OutlineFill;	

	CExtComboBox			m_ActionPointCombo;
	CExtButton				m_ActionPointAdd;
	CExtCheckBox			m_Smooth;		



	CFont theFont;

	CImageEditor* m_pImgEd;

	void OnActionPointModify();
	void ActionSelectionChange();
	void OnSmoothModify();


	// Generated message map functions
protected:
	//{{AFX_MSG(CToolSettings)
	afx_msg void OnEditUpdate();


	DECLARE_MESSAGE_MAP()
};


class CAnimationToolbar : public CExtPanelControlBar
{
public:
	CAnimationToolbar();
	CImageEditor* m_pImgEd;
	void OnPaint();
	void UpdateThumbnail(int index);
	void ChangeFrame(int newindex);
	void AdvanceFrames(int offset);
	void OnLButtonDown(UINT nFlags, CPoint point);
public:
	map<int, CxImage> m_thumbnails;	//int is index in animation
	
	// Generated message map functions

// Attributes
public:


CSize OnQueryFixedBarNewSize(
	CExtPanelControlBar::e_fixed_bar_size_query_t eWhichSizeNeedToKnow
	) const
{
		ASSERT_VALID( this );
	
		return CSize(4000,40);
}




protected:
	//{{AFX_MSG(CToolSettings)

	DECLARE_MESSAGE_MAP()


};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLSETTINGS_H__942A5E43_DB0E_4D9B_9F38_D2F0E512C37E__INCLUDED_)
