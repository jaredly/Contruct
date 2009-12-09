#include "stdafx.h"
#include "mditabwnd.h"
#include "..\mainfrm.h"

extern CMainFrame* g_MainFrame;

LPCTSTR CConstructMDITabs::OnTabWndQueryItemText(const CExtTabWnd::TAB_ITEM_INFO * pTii)
{                                                               
	ASSERT_VALID(pTii);
	HWND hWndMdiChild = (HWND)pTii->LParamGet();

	if (hWndMdiChild == NULL || (! ::IsWindow(hWndMdiChild)))
		return _T("");

	CChildFrame* pWnd = (CChildFrame*)CWnd::FromHandle(hWndMdiChild);   

	ASSERT(pWnd != NULL);

	ASSERT_KINDOF(CMDIChildWnd, pWnd);
	static CString Text(_T(""));

	// If it has 2 tabs it's a layout
	if (pWnd->m_tabs.ItemGetCount() == 2)
	{
		if (pWnd->m_tabs.SelectionGet() == 0)
			Text.Format("%s : %s", pWnd->application->file_information.name, pWnd->layout->GetName());
		else
			Text.Format("%s : %s", pWnd->application->file_information.name, pWnd->event_sheet->GetName());
	}

	// Otherwise it's an event sheet
	else
		Text.Format("%s : %s", pWnd->application->file_information.name, pWnd->event_sheet->GetName());

	// Modified
	if (pWnd->layout)
	{
		if (pWnd->layout->GetChanged() && pWnd->m_tabs.SelectionGet() == 0)
			Text += "*";

		else if (pWnd->event_sheet->GetChanged())
			Text += "*";
	}

	else if (pWnd->event_sheet->GetChanged())
		Text += "*";
				
	return (Text.IsEmpty()) ? _T("") : LPCTSTR(Text);         
}

LRESULT CConstructMDITabs::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MBUTTONUP:
		{
			CPoint point = DWORD(lParam);
			CPoint ptClient = point;
			//ScreenToClient(&ptClient);
			LONG Item = ItemHitTest(ptClient);

			HWND hWndMdiChild = (HWND)ItemLParamGet(Item);
			CChildFrame* pWnd = (CChildFrame*)CWnd::FromHandle(hWndMdiChild);   

			pWnd->DestroyWindow();
			Invalidate();
		}
		return 0;
		break;

	case WM_CONTEXTMENU:
		CPoint point = DWORD(lParam);
		CPoint ptClient = point;
		ScreenToClient(&ptClient);
		LONG Item = ItemHitTest(ptClient);
		if(Item < 0) return 0;

		// Context menu now
		POINT CursorPosition;
		GetCursorPos(&CursorPosition);

		// Load popup context menu
		CExtPopupMenuWnd* popup = new CExtPopupMenuWnd;
		popup->CreatePopupMenu(m_hWnd);

		popup->ItemInsertCommand(1, -1, "Close", NULL, NULL);
		popup->ItemInsertCommand(2, -1, "Close all but this", NULL, NULL);

		// Display it
		UINT ChosenItem = 0;

		// i dont know why, but no wm and no cmd actually ment - post cmd and ui while the popup is open :S
		popup->TrackPopupMenu(TPMX_DO_MESSAGE_LOOP | TPMX_NO_WM_COMMAND, CursorPosition.x, CursorPosition.y, NULL, NULL, NULL, &ChosenItem);

		if (ChosenItem == 1)
		{
			// Close tab
			HWND hWndMdiChild = (HWND)ItemLParamGet(Item);
			CChildFrame* pWnd = (CChildFrame*)CWnd::FromHandle(hWndMdiChild);   

			pWnd->DestroyWindow();
			Invalidate();
		}

		if (ChosenItem == 2)
		{
			HWND ClientWindow;
			ClientWindow = ::GetWindow(g_MainFrame->m_hWndMDIClient, GW_CHILD);

			HWND hWndMdiChild = (HWND)ItemLParamGet(Item);

			vector<HWND> ToDestroy;

			// Now find any layouts within this application
			while (ClientWindow != NULL)
			{
				if (ClientWindow != hWndMdiChild)
					ToDestroy.push_back(ClientWindow);
				ClientWindow = :: GetWindow(ClientWindow, GW_HWNDNEXT);
			}
			
			for (int i = 0; i < ToDestroy.size(); i++)
			{
				CChildFrame* pWnd = (CChildFrame*)CWnd::FromHandle(ToDestroy[i]);   
				pWnd->DestroyWindow();
			}

			Invalidate();
		}
		return 0;
		break;
	}

	return CExtTabMdiWhidbeyWnd::WindowProc(message, wParam, lParam);
}