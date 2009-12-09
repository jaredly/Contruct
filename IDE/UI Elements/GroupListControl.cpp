// GroupListCtrl.cpp

#include "stdafx.h"
#include "GroupListCtrl.h"

BEGIN_MESSAGE_MAP(CGroupListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

BOOL CGroupListCtrl::GetCellRect(int iRow, int iCol, int nArea, CRect &rect)
{
	return GetItemRect(iRow, rect, nArea);
}

void CGroupListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	switch(lplvcd->nmcd.dwDrawStage)
	{
		case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW; // Ask for item notifications
		}
		break;

		case CDDS_ITEMPREPAINT:
		{
			int iRow = lplvcd->nmcd.dwItemSpec;

			CString Item = GetItemText(iRow, 0);

			if (bold_items[iRow])
			{				
				lplvcd->clrTextBk = RGB(255, 255, 170);

				*pResult= CDRF_NEWFONT;
			}

			else
				*pResult = CDRF_DODEFAULT;

			break;
		}
		break;

		default:
		{
			*pResult = CDRF_DODEFAULT;
		}
	}
}

void CGroupListCtrl::AddGroup(int Index, LPCSTR Name)
{
	int len = strlen(Name) + 1;
	wchar_t *wText = new wchar_t[len];

	memset(wText, 0, len);
	MultiByteToWideChar(CP_ACP, NULL, Name, -1, wText, len);

	LVGROUP lg;
	lg.cbSize = sizeof(LVGROUP);
	lg.mask = LVGF_HEADER | LVGF_GROUPID;

	lg.pszHeader = wText;
	lg.iGroupId  = Index;
	ListView_InsertGroup(m_hWnd, -1, &lg);

	delete []wText;
}