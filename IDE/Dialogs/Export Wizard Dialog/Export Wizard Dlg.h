#pragma once

// Application structure for Object map
#include "..\..\Structure.h"

#include "..\..\Utilities\anchor.h"
#include "..\..\Utilities\dlgman.h"

class CExportWizardDlg : public CDialog
{
public:
	CExportWizardDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_EXPORTWIZARD };

	void			UpdateButtons();
	void			ChangeStep(int iStep);
	int				m_iStep;

	CExtButton		m_Cancel;
	CExtButton		m_Next;
	CExtButton		m_Back;

	// Step 1 controls
	CButton			m_EnablePython;
	CButton			m_CopyDLL;
	CStatic			m_Text1;
	CCheckListBox	m_Modules;

	// Step 2 controls
	CStatic			m_Text2;
	CEdit			m_Output;
	CExtButton		m_Browse;
	CButton			m_RunAfter;
	CButton			m_Installer;
	CExtButton		m_InstallSettings;
	CButton			m_Screensaver;

	CApplication*	application;

	CDlgAnchor		m_DlgAnchor;
	CDlgMan			m_DlgManager;   

	// Drawing
	CFont			m_FontBold;

	// Output
		// Standard
		CString			m_OutputPath;
		bool			m_bScreensaver;

		// Installer
		CString			m_License;
		CString			m_InstallPath;
		bool			m_bInstaller;
		bool			m_bFullscreen;
		int				m_OS;

protected:
	afx_msg void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedInstallSettings();
	afx_msg void OnCheckUsePython();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};