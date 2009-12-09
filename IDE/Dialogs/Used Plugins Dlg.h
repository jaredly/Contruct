#pragma once

#include "..\resource.h"
#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"

class UsedPluginDialog : public CExtNCW<CExtResizableDialog>
{
public:

	UsedPluginDialog(CApplication*);  

protected:

	enum { IDD = IDD_USEDPLUGINS };
	CExtButton information;
	CExtButton remove;
	CExtButton close;
	CListCtrl objects;

	CApplication* application;

	CDlgAnchor dialog_anchor;
	CDlgMan dialog_manager;   

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnInformation();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};