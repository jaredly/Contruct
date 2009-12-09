// mfxWhlPan.inl : inline file
//

// control specific classes 
// all derived from "CAutoPanParameters"

// Controls with Message Processing
class CAutoPanParametersMessage : public CAutoPanParameters
{
public:
	CAutoPanParametersMessage(LPCTSTR szClassName = NULL, UINT nWindowResID = 0) : CAutoPanParameters(szClassName, nWindowResID) {};

	virtual bool DoScrollWindow(CWnd* pParentWnd, int nScrollToX, int /*nOriginX*/, int nScrollToY, int /*nOriginY*/) const
	{
		pParentWnd->SendMessage(WM_HSCROLL, MAKELONG(SB_THUMBPOSITION, nScrollToX), NULL);
		pParentWnd->SendMessage(WM_VSCROLL, MAKELONG(SB_THUMBPOSITION, nScrollToY), NULL);

		return true;
	}
};

class CAutoPanParametersLineMessages : public CAutoPanParameters
{
public:
	CAutoPanParametersLineMessages(LPCTSTR szClassName = NULL, UINT nWindowResID = 0) : CAutoPanParameters(szClassName, nWindowResID) {};

	virtual bool DoScrollWindow(CWnd* pParentWnd, int nScrollToX, int nOriginX, int nScrollToY, int nOriginY) const
	{
		for (nScrollToX -= nOriginX; nScrollToX>0; nScrollToX--) pParentWnd->SendMessage(WM_HSCROLL, SB_LINERIGHT, NULL);
		for (;nScrollToX<0; nScrollToX++)                        pParentWnd->SendMessage(WM_HSCROLL, SB_LINELEFT, NULL);
		for (nScrollToY -= nOriginY; nScrollToY>0; nScrollToY--) pParentWnd->SendMessage(WM_VSCROLL, SB_LINEDOWN, NULL);
		for (;nScrollToY<0; nScrollToY++)                        pParentWnd->SendMessage(WM_VSCROLL, SB_LINEUP, NULL);

		return true;
	}
};

class CAutoPanParametersRegMessage : public CAutoPanParameters
{
public:
	CAutoPanParametersRegMessage(LPCTSTR szClassName = NULL, UINT nWindowResID = 0) : CAutoPanParameters(szClassName, nWindowResID) {};

	virtual bool DoScroll(CWnd* pParentWnd, int nScrollStepsX, int nScrollStepsY) const
	{
		CSize sizeLocalDistance(nScrollStepsX, nScrollStepsY);
		return pParentWnd->SendMessage(uOriginWindowUpdateMessage, 0, reinterpret_cast<LPARAM>(&sizeLocalDistance)) != 0;
	}

	static const UINT uOriginWindowUpdateMessage;
};

class CAutoPanParametersMessageThumbTrack : public CAutoPanParameters
{
public:
	CAutoPanParametersMessageThumbTrack(LPCTSTR szClassName = NULL, UINT nWindowResID = 0) : CAutoPanParameters(szClassName, nWindowResID) {};

	virtual bool DoScrollWindow(CWnd* pParentWnd, int nScrollToX, int /*nOriginX*/, int nScrollToY, int /*nOriginY*/) const
	{
		pParentWnd->SendMessage(WM_HSCROLL, MAKELONG(SB_THUMBTRACK, nScrollToX), NULL);
		pParentWnd->SendMessage(WM_VSCROLL, MAKELONG(SB_THUMBTRACK, nScrollToY), NULL);

		return true;
	}
};

// SysTreeView32
class CAutoPanParametersSysTreeView32 : public CAutoPanParametersMessage
{
public:
	CAutoPanParametersSysTreeView32() : CAutoPanParametersMessage(_T("SysTreeView32")) {};
	virtual int GetPixelsInAStepY(CWnd* pParentWnd, int nScrollY) const
	{
		CRect rc;
		HTREEITEM treeitem2, treeitem = static_cast<CTreeCtrl*>(pParentWnd)->GetFirstVisibleItem();

		if (treeitem && (nScrollY < 0))
			treeitem2 = static_cast<CTreeCtrl*>(pParentWnd)->GetPrevVisibleItem(treeitem);
		else
			treeitem2 = NULL;
		if (treeitem2) treeitem = treeitem2;

		if (treeitem && static_cast<CTreeCtrl*>(pParentWnd)->GetItemRect(treeitem, &rc, FALSE))
			return min(max(rc.Height(), 1), 42);
		else
			return CAutoPanParametersMessage::GetPixelsInAStepY(pParentWnd, nScrollY);
	}
};

// RICHEDIT
class CAutoPanParametersRICHEDIT : public CAutoPanParameters
{
public:
	CAutoPanParametersRICHEDIT() : CAutoPanParameters(_T("RICHEDIT")) {};
	virtual bool DoScrollWindow(CWnd* pParentWnd, int nScrollToX, int /*nOriginX*/, int nScrollToY, int nOriginY) const
	{
		static_cast<CRichEditCtrl*>(pParentWnd)->LineScroll(nScrollToY-nOriginY);
		pParentWnd->SendMessage(WM_HSCROLL, MAKELONG(SB_THUMBPOSITION, nScrollToX), NULL);
		return true;
	}
	virtual int GetPixelsInAStepY(CWnd* /*pParentWnd*/, int /*nScrollY*/) const {return 16;};
};

// Edit
class CAutoPanParametersEdit : public CAutoPanParametersMessage
{
public:
	CAutoPanParametersEdit() : CAutoPanParametersMessage(_T("Edit")) {};
	virtual int GetPixelsInAStepY(CWnd* /*pParentWnd*/, int /*nScrollY*/) const {return 16;};
};


// SysListView32
class CAutoPanParametersSysListView32 : public CAutoPanParameters
{
public:
	CAutoPanParametersSysListView32() : CAutoPanParameters(_T("SysListView32")) {};
	virtual bool DoScrollWindow(CWnd* pParentWnd, int nScrollToX, int nOriginX, int nScrollToY, int nOriginY) const
	{
		static_cast<CListCtrl*>(pParentWnd)->Scroll(CSize(nScrollToX-nOriginX, nScrollToY-nOriginY));
		return true;
	}

	virtual int GetPixelsInAStepX(CWnd* pParentWnd, int nScrollX) const
	{
		DWORD dwStyle = pParentWnd->GetStyle() & LVS_TYPEMASK;

		if (dwStyle == LVS_LIST)
		{
			return max(min(static_cast<CListCtrl*>(pParentWnd)->GetColumnWidth(0), 1), 42);
		}
		else
		{
			return CAutoPanParameters::GetPixelsInAStepX(pParentWnd, nScrollX);
		}
	}

	virtual int GetPixelsInAStepY(CWnd* pParentWnd, int nScrollY) const
	{
		DWORD dwStyle = pParentWnd->GetStyle() & LVS_TYPEMASK;

		if (dwStyle == LVS_REPORT)
		{
			CRect rc;
			int nItem2, nItem = static_cast<CListCtrl*>(pParentWnd)->GetTopIndex();

			if ((nItem != -1) && (nScrollY < 0))
				nItem2 = static_cast<CListCtrl*>(pParentWnd)->GetNextItem(nItem, LVNI_ABOVE);
			else 
				nItem2 = -1;

			if (nItem2 != -1) nItem = nItem2;
			
			if ((nItem != -1) && static_cast<CListCtrl*>(pParentWnd)->GetItemRect(nItem, &rc, LVIR_BOUNDS))
				return max(rc.Height(), 1);
			else
				return CAutoPanParameters::GetPixelsInAStepY(pParentWnd, nScrollY);
		}
		else
		{
			return CAutoPanParameters::GetPixelsInAStepY(pParentWnd, nScrollY);
		}
	}

	virtual int GetSubstepsInAStepY(CWnd* pParentWnd, int nScrollY) const
	{
		return GetPixelsInAStepY(pParentWnd, nScrollY);
	}
};

class CAutoPanParametersListBox : public CAutoPanParametersMessage
{
public:
	CAutoPanParametersListBox() : CAutoPanParametersMessage(_T("ListBox")) {};

	virtual int GetPixelsInAStepY(CWnd* pParentWnd, int nScrollY) const
	{
		int nItem = static_cast<CListBox*>(pParentWnd)->GetTopIndex();

		if (nScrollY < 0) nItem--;
		if (nItem < LB_OKAY) nItem = 0;

		return max(min(static_cast<CListBox*>(pParentWnd)->GetItemHeight(nItem), 42), 1);
	}
};


// Other Controls with special Handling
class CAutoPanParametersComboBox : public CAutoPanParameters
{
public:
	CAutoPanParametersComboBox() : CAutoPanParameters(_T("ComboBox")) {};
	
	virtual CWnd* PanThisWindow(CWnd* pParentWnd) const
	{
		CWnd* pWnd = CAutoPanParameters::PanThisWindow(pParentWnd);
		if (pWnd) return pWnd;

		TCHAR szClassName[MAX_PATH];
		::GetClassName(pParentWnd->m_hWnd, szClassName, MAX_PATH);
			
		if (lstrcmp(szClassName, _T("Edit")) != 0) return NULL;

		pWnd = pParentWnd->GetParent();
		if (pWnd)
		{
			::GetClassName(pWnd->m_hWnd, szClassName, MAX_PATH);

			if (lstrcmp(m_szClassName, szClassName) == 0)
			{
				return pWnd;
			}
		}

		return NULL;
	}

	virtual UINT GetWindowResID(CWnd* /*pParentWnd*/) const {return 0;};
	virtual void GetBitmapDimensions(CSize& size, CWnd* /*pParentWnd*/) const {size.cx=1;size.cy=1;};
	virtual int GetCursorResID(int, int, bool, bool) const {return IDC_PAN_SPIN;};

	virtual bool NoVertScroll(CWnd* /*pParentWnd*/) const {return false;};
	virtual bool NoHorzScroll(CWnd* /*pParentWnd*/) const {return true;};

	virtual int GetPixelsInAStepY(CWnd* /*pParentWnd*/, int /*nScrollY*/) const {return 16;};

	virtual bool DoScroll(CWnd* pParentWnd, int /*nScrollStepsX*/, int nScrollStepsY) const
	{
		int range; int pos;
		range = static_cast<CComboBox*>(pParentWnd)->GetCount()-1;
		pos   = static_cast<CComboBox*>(pParentWnd)->GetCurSel();

		if (pos == CB_ERR) pos = 0;
		else pos += nScrollStepsY;

		if (pos > range) pos = range;
		if (pos < 0) pos = 0;
		static_cast<CComboBox*>(pParentWnd)->SetCurSel(pos);

		return true;
	}
};

class CAutoPanParametersmsctls_updown32 : public CAutoPanParameters
{
public:
	CAutoPanParametersmsctls_updown32() : CAutoPanParameters(_T("msctls_updown32")) {};
	
	virtual CWnd* PanThisWindow(CWnd* pParentWnd) const
	{
		CWnd* pWnd = CAutoPanParameters::PanThisWindow(pParentWnd);
		if (pWnd) return pWnd;
			
		TCHAR szClassName[MAX_PATH];
		::GetClassName(pParentWnd->m_hWnd, szClassName, MAX_PATH);
			
		if (lstrcmp(szClassName, _T("Edit")) != 0) return NULL;

		pWnd = pParentWnd->GetNextWindow();
		if (pWnd)
		{
			::GetClassName(pWnd->m_hWnd, szClassName, MAX_PATH);

			if (lstrcmp(m_szClassName, szClassName) == 0)
			{
				return pWnd;
			}
		}

		return NULL;
	}

	virtual UINT GetWindowResID(CWnd* /*pParentWnd*/) const {return 0;};
	virtual void GetBitmapDimensions(CSize& size, CWnd* /*pParentWnd*/) const {size.cx=1;size.cy=1;};
	virtual int GetCursorResID(int, int, bool, bool) const {return IDC_PAN_SPIN;};

	virtual bool NoVertScroll(CWnd* /*pParentWnd*/) const {return false;};
	virtual bool NoHorzScroll(CWnd* /*pParentWnd*/) const {return true;};
	
	virtual int GetPixelsInAStepY(CWnd* /*pParentWnd*/, int /*nScrollY*/) const {return 16;};

	virtual bool DoScroll(CWnd* pParentWnd, int /*nScrollStepsX*/, int nScrollStepsY) const
	{
		DWORD range; int pos;
		range = static_cast<CSpinButtonCtrl*>(pParentWnd)->GetRange();
		pos   = static_cast<CSpinButtonCtrl*>(pParentWnd)->GetPos();

		if (HIWORD(range) < LOWORD(range))
		{
			if (HIWORD(pos)) pos = 0;
			else pos = LOWORD(pos) - nScrollStepsY;

			pos = max(pos, HIWORD(range));
			pos = min(pos, LOWORD(range));
		}
		else
		{
			if (HIWORD(pos)) pos = 0;
			else pos = LOWORD(pos) + nScrollStepsY;

			pos = min(pos, HIWORD(range));
			pos = max(pos, LOWORD(range));
		}

		static_cast<CSpinButtonCtrl*>(pParentWnd)->SetPos(pos);

		return true;
	}
};
