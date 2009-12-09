
/*
 * File:		FlatSplitterWnd.h
 *
 * Author:		Marc Richarme <devix@devix.cjb.net>
 *
 * Created:		28 Jan. 2001
 * Modified:	28 Jan. 2001
 *
 * Use as much as you want, wherever you want...
 * Claim you coded it, I don't mind.
 *
 */

#if !defined(AFX_FLATSPLITTERWND_H__61D2F7E7_7AAA_11D1_9F4C_008029E98A75__INCLUDED_)
#define AFX_FLATSPLITTERWND_H__61D2F7E7_7AAA_11D1_9F4C_008029E98A75__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CFlatSplitterWnd

#include "CustomTabCtrl.h"

class CFlatSplitterWnd : public CExtSplitterWnd
{
public:
	CFlatSplitterWnd();
	virtual ~CFlatSplitterWnd();

	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg);
	virtual void RecalcLayout();    // call after changing sizes
	void RecalcLayout2(long w1, long w);

	static void DeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
		CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar);
	static void LayoutRowCol(CExtSplitterWnd::CRowColInfo* pInfoArray,
		int nMax, int nSize, int nSizeSplitter);

	virtual BOOL CreateScrollBarCtrl(DWORD dwStyle, UINT nID);
	virtual BOOL DoScroll(CView* pViewFrom, UINT nScrollCode, BOOL bDoScroll);
	virtual void SetActivePane(int row, int col, CWnd *pWnd = 0);
	//virtual CFlatSplitterWnd::SetScrollStyle(DWORD dwStyle)
	int cRow;
	int cCol;
	int tab;
	long m_w1, m_w;

	CExtTabFlatWnd* m_pTabs;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlatSplitterWnd)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CFlatSplitterWnd)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLATSPLITTERWND_H__61D2F7E7_7AAA_11D1_9F4C_008029E98A75__INCLUDED_)
