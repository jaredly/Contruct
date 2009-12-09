// JazzUpTellTaleButton.cpp : implementation file
//

#include "stdafx.h"
#include "JazzUpTellTaleButton.h"
#include "..\Dialogs\ParametersDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJazzUpTellTaleButton

CJazzUpTellTaleButton::CJazzUpTellTaleButton()
{
}

CJazzUpTellTaleButton::~CJazzUpTellTaleButton()
{
}

void CJazzUpTellTaleButton::SetIcon(HICON nID )
{
//	CShadeButtonST::SetShade( SHS_VSHADE );
	CExtButton::SetIcon( nID );
}

BEGIN_MESSAGE_MAP(CJazzUpTellTaleButton, CExtButton)
	//{{AFX_MSG_MAP(CJazzUpTellTaleButton)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_SIZE()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJazzUpTellTaleButton message handlers

void CJazzUpTellTaleButton::OnClicked() 
{
	GetParent()->SendMessage( WM_RUBRIC_WND_CLICKED_ON, (WPARAM)this->m_hWnd );
}

void CJazzUpTellTaleButton::OnSize(UINT nType, int cx, int cy) 
{
	CExtButton::OnSize(nType, cx, cy);
}

void CJazzUpTellTaleButton::OnRButtonUp(UINT nFlags, CPoint point)
{
	// Mouse position
	POINT cursorPosition;
	GetCursorPos(&cursorPosition);

	CParametersDlg* ParamDlg = dynamic_cast<CParametersDlg*>(GetParent()->GetParent());
	if(!ParamDlg) return;

	int index = GetWindowLong(m_hWnd,GWL_ID);

	int type = ParamDlg->m_Stack.m_arrPanes[index]->type;

	if (type == EDITORPARAM_GLOBALVARIABLE)
		return;

	CExtPopupMenuWnd* popup =  new CExtPopupMenuWnd;
	popup->CreatePopupMenu(m_hWnd);
	popup->ItemInsertCommand(1,-1, "Use Expression",NULL,NULL);
	popup->ItemInsert(CExtPopupMenuWnd::TYPE_SEPARATOR);
	popup->ItemInsertCommand(2,-1, "Default",NULL,NULL);
	UINT Item = 0;
	if(!popup->TrackPopupMenu(TPMX_DO_MESSAGE_LOOP|TPMX_NO_WM_COMMAND|TPMX_NO_CMD_UI, cursorPosition.x, cursorPosition.y, NULL, NULL, NULL, &Item))
		return;

	if(ParamDlg && Item != 0 && Item != -1)
	{
		CString paraminfo = ParamDlg->m_Stack.m_arrPanes[index]->GetParameterString();
		ParamDlg->SetRedraw(false);
		ParamDlg->m_Stack.m_arrPanes[index]->content_window->DestroyWindow();

		int type = EDITORPARAM_USERPARAM;
		if(Item == 2)
			type = ParamDlg->m_pACEEntry->params.at(index).type;
		
		ParamDlg->CreateParameter(index,&ParamDlg->m_Stack,*ParamDlg->m_Stack.m_arrPanes[index], type, paraminfo);
			
		
		ParamDlg->m_Stack.RearrangeStack();
		ParamDlg->SetRedraw(true);
		ParamDlg->Invalidate();
	}
	else
	{
		CErrorDlg dlg;
		dlg.Error("Construct Error", "Error in JazzUpTellTaleButton - parent window could not be found");
	}


}