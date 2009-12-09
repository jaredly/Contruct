#pragma once

class CConstructMDITabs : public CExtTabMdiWhidbeyWnd
{
public:
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	LPCTSTR OnTabWndQueryItemText(const CExtTabWnd::TAB_ITEM_INFO * pTii);
};
