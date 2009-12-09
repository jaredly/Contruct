// ConstructSDK.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ConstructSDK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConstructSDKApp

BEGIN_MESSAGE_MAP(CConstructSDKApp, CWinApp)
	//{{AFX_MSG_MAP(CConstructSDKApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConstructSDKApp construction

CConstructSDKApp::CConstructSDKApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CConstructSDKApp object

CConstructSDKApp theApp;

void CConstructSDKApp::AddToRecentFileList(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWinApp::AddToRecentFileList(lpszPathName);
}
