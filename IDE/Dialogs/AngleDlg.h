#pragma once
#include "afxwin.h"
#include "..\resource.h"
// CAngleDlg dialog

class CAngleDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CAngleDlg)

public:
	CAngleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAngleDlg();

	vector<float> m_Angles;

	CExtButton m_OK, m_Cancel;
	CComboBoxEx m_AddAngles;

	void OnBnClickedOk();

	CImageList m_AngleIcons;

	BOOL OnInitDialog() ;

// Dialog Data
	enum { IDD = IDD_ANGLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
