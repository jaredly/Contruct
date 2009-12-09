#pragma once

class CImageEditor;

class CRotateDlg : public CDialog
{
	DECLARE_DYNAMIC(CRotateDlg)

public:
	CRotateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRotateDlg();

	CComboBoxEx		m_Angle;
	CImageList		m_AngleIcons;
	CExtButton		m_Rotate;
	CImageEditor*	m_pParent;

// Dialog Data
	enum { IDD = IDD_POPUPROTATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnPaint() ;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedRotate();
};

#include "..\ScriptDlg.h"