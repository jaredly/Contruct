/////////////////////////////////////////////////////////////////////////////
// Parameters dialog
/////////////////////////////////////////////////////////////////////////////

// Scintilla file

// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "..\ParametersDlg.h"
extern CMainFrame* g_MainFrame;

// SetScintillaColour(CScintillaWnd* pWindow, long Colour)
// Set a scintilla control with colours, spacings and margins.
void CParametersDlg::SetScintillaColour(CScintillaWnd* pWindow, long Colour)
{
	pWindow->SendMessage(SCI_SETVIEWWS, SCWS_VISIBLEALWAYS, 0); 
	pWindow->SendMessage(SCI_SETWHITESPACEBACK, (long)true, Colour); 
	pWindow->SendMessage(SCI_SETWHITESPACEFORE, (long)true, Colour);
	pWindow->SendMessage(SCI_STYLESETBACK,STYLE_DEFAULT, Colour);
	pWindow->SendMessage(SCI_STYLESETBACK,0, Colour);
	//pWindow->SendMessage(SCI_STYLECLEARALL, 0, 0);
	pWindow->SendMessage(SCI_SETMARGINWIDTHN, 0, 0);


}

// OnChar(NMHDR* pNMHDR, LRESULT* pResult, CScintillaWnd& ScinillaWindow)
// Triggered when a character is typed; validate content.
void CParametersDlg::OnChar(NMHDR* pNMHDR, LRESULT* pResult, CScintillaWnd& ScintillaWindow)
{
	m_pWindow = &ScintillaWindow;
	SetTimer(500, 200, NULL); // Check content in 200 ms

	CParamScintilla::OnChar(pNMHDR, pResult, ScintillaWindow, objectMap,
							m_Tooltip,
							m_ParameterTooltipIsVisible,
							m_QuotesOpen, application);

	m_Stack.RearrangeStack();
}

// CParametersDlg::ResizeScintilla(CScintillaWnd& ScinillaWindow)
// Resize a scintilla window to fit text.
void CParametersDlg::ResizeScintilla(CScintillaWnd& ScinillaWindow)
{
	int SelStart	= ScinillaWindow.SendMessage(SCI_GETSELECTIONSTART, 0, 0);
	int SelEnd		= ScinillaWindow.SendMessage(SCI_GETSELECTIONEND, 0, 0);

	CRect Pos;
	ScinillaWindow.GetWindowRect(Pos);
	Pos.bottom = Pos.top+13;
	ScinillaWindow.SetRedraw(false);
	ScinillaWindow.SetWindowPos(NULL, 0,0, Pos.Width(), Pos.Height(),
								SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);

	int ScrollPos = ScinillaWindow.SendMessage(SCI_GETCURRENTPOS, 0, 0);
	int End = ScinillaWindow.SendMessage(SCI_GETTEXTLENGTH, 0, 0);
	ScinillaWindow.SendMessage(SCI_GOTOPOS, End, 0);

	int NewLines = ScinillaWindow.SendMessage(SCI_GETFIRSTVISIBLELINE, 0, 0);

	ScinillaWindow.SetWindowPos(0, 0, 0, Pos.Width(), 
								NewLines*13 + Pos.Height(),
								SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);

	ScinillaWindow.SendMessage(SCI_GOTOPOS, ScrollPos, 0);
	ScinillaWindow.SendMessage(SCI_SETSELECTIONSTART, SelStart, 0);
	ScinillaWindow.SendMessage(SCI_SETSELECTIONEND, SelEnd,0);

	ScinillaWindow.SetRedraw(true);
}