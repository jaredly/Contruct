// TelltaleButton.cpp : implementation file
//

#include "stdafx.h"
#include "TelltaleButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTelltaleButton

CTelltaleButton::CTelltaleButton()
{
}

CTelltaleButton::~CTelltaleButton()
{
}


BEGIN_MESSAGE_MAP(CTelltaleButton, CButton)
	//{{AFX_MSG_MAP(CTelltaleButton)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTelltaleButton message handlers

void CTelltaleButton::OnClicked() 
{
	GetParent()->SendMessage( WM_RUBRIC_WND_CLICKED_ON, (WPARAM)this->m_hWnd );
}
