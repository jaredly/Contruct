// Construct.h : main header file for Construct
//

#if !defined(AFX_CONSTRUCT_H__CF923FD8_45BA_4B78_8AFE_5DBE07AC0D73__INCLUDED_)
#define AFX_CONSTRUCT_H__CF923FD8_45BA_4B78_8AFE_5DBE07AC0D73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "Structure.h"

/////////////////////////////////////////////////////////////////////////////
// Construct:
// See Construct Doc.cpp for the implementation of this class
//

class Construct : public CWinApp
{

public:
	Construct();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void LoadDLLs();
	void UnloadDLLs();
	void SortCategories(vector<ACESEntry2>& aces, vector<AceCategory>& cats);
	CMap<DWORD, DWORD&, OINFO*, OINFO*&> m_dlls;

// Implementation
	//{{AFX_MSG(Construct)
	afx_msg void OnExportEXE();
	afx_msg void OnAppNew();
	afx_msg void OnInsert();
	afx_msg void OnInsertFrame();
	afx_msg void nothing(){}
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	HMODULE m_hDll; //scintella dll
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSTRUCT_H__CF923FD8_45BA_4B78_8AFE_5DBE07AC0D73__INCLUDED_)
