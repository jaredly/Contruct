// mfxWhlPan.h : header file
//

#if !defined(AFX_MFXWHLPAN_H__4E9C2711_CFCD_11D1_87BA_400011900025__INCLUDED_)
#define AFX_MFXWHLPAN_H__4E9C2711_CFCD_11D1_87BA_400011900025__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


/////////////////////////////////////////////////////////////////////////////
// Styles for special configurations

#define MFX_WHEELWNDSTYLE_ONEDIRECTION          0x0001
#define MFX_WHEELWNDSTYLE_UPDOWNONLY            0x0002
#define MFX_WHEELWNDSTYLE_LEFTRIGHTONLY         0x0004
#define MFX_WHEELWNDSTYLE_NOSUBPIXELSCROLLING   0x0008

#define MFX_WHEELWNDSTYLE_SCROLLBYMESSAGE       0x0010
#define MFX_WHEELWNDSTYLE_SCROLLBYMESSAGEEX     0x0020
#define MFX_WHEELWNDSTYLE_SCROLLBYMESSAGEREG    0x0040
#define MFX_WHEELWNDSTYLE_SCROLLBYMESSAGETRK    0x0080

#define MFX_WHEELWNDSTYLE_DEFAULT               0x0000
#define MFX_WHEELWNDSTYLE_MASK_SCROLL           0x00F0


/////////////////////////////////////////////////////////////////////////////
// Message Name for "MFX_WHEELWNDSTYLE_SCROLLBYMESSAGEREG"

#define OWND_WINDOW_UPDATE _T("rjf_OriginWindowUpdate")


/////////////////////////////////////////////////////////////////////////////
// Macros for simplyfied usage

// To use the AutoPanning in a CWnd, add a handler for "WM_MBUTTONDOWN"
// and use the "CWheelWnd_OnMButtonDown" macro as the first call.
#define CWheelWnd_OnMButtonDown if(!(nFlags&MK_CONTROL || nFlags&MK_SHIFT)) { MfxTrackAutoPan(this, MFX_WHEELWNDSTYLE_NOSUBPIXELSCROLLING); return; }
#define CWheelWnd_OnLButtonDown if(nFlags&MK_RBUTTON && !(nFlags&MK_CONTROL || nFlags&MK_SHIFT)) { MfxTrackAutoPan(this); return; }
#define CWheelWnd_OnRButtonDown if(nFlags&MK_LBUTTON && !(nFlags&MK_CONTROL || nFlags&MK_SHIFT)) { MfxTrackAutoPan(this); return; }

// If used in a dialog, add the virtual function "PreTranslateMessage"
// and use the "CWheelWnd_PreTranslateMessage" as the first call
// and use CWheelWnd_OnMButtonDown_Dlg instead of CWheelWnd_OnMButtonDown
#define CWheelWnd_PreTranslateMessage \
	if (pMsg && pMsg->message==WM_MBUTTONDOWN) \
	{ OnMButtonDown(pMsg->wParam, CPoint(pMsg->pt)); return TRUE; }

#define CWheelWnd_OnMButtonDown_Dlg \
	if(!(nFlags&MK_CONTROL || nFlags&MK_SHIFT)) \
	{  CWnd* pWnd = WindowFromPoint(point);	\
	   if (pWnd) { MfxTrackAutoPan(pWnd); return; } }

// If used in a property page, add the virtual function "PreTranslateMessage"
// and use the "CWheelWnd_PreTranslateMessage" as the first call
// and use CWheelWnd_OnMButtonDown_PropPg instead of CWheelWnd_OnMButtonDown
#define CWheelWnd_OnMButtonDown_PropPg \
	if(!(nFlags&MK_CONTROL || nFlags&MK_SHIFT)) \
	{  CWnd* pWnd = WindowFromPoint(point); if (pWnd) \
	   {  pWnd->ScreenToClient(&point); \
	      pWnd = pWnd->ChildWindowFromPoint(point); \
	      if (pWnd) { MfxTrackAutoPan(pWnd); return; } } }


// Used to register own panning implementations
// e.g. for using own functions for panning or for adding
// a different behavior for some purpose
#define CWheelWnd_Register(class) CWheelWnd::Register(new class())

// The default "shortcut" for AutoPanning
class CAutoPanParameters;
void MfxTrackAutoPan(CWnd* pParentWnd, WORD wStyle = MFX_WHEELWNDSTYLE_DEFAULT, CAutoPanParameters* pAutoPanParameters = NULL);


/////////////////////////////////////////////////////////////////////////////
// CAutopanDialog dialog

class CAutoPanParameters
{
public:
	// szClassName: classname for supported class; NULL to use for all
	// nWindowResID: resourceID for backgound bitmap
	CAutoPanParameters(LPCTSTR szClassName = NULL, UINT nWindowResID = 0);
	virtual ~CAutoPanParameters();

	// Which background bitmap shall we show?
	virtual UINT GetWindowResID(CWnd* /*pParentWnd*/) const {return m_nWindowResID;};
	virtual void GetBitmapDimensions(CSize& size, CWnd* pParentWnd) const;
	virtual void CreateWindowRegion(CRgn& rgn, CWnd* /*pParentWnd*/) const;

	// Can we use this class to pan which window?
	virtual CWnd* PanThisWindow(CWnd* pParentWnd) const;

	// We are scrolling! So which cursor to show?
	virtual int GetCursorResID(int nScrollX, int nScrollY, bool bNoHorzScroll, bool bNoVertScroll) const;

	// Implementation of scrolling:
	// This one is called if we have to scroll at least one step. The default
	// implementation assumes scrollbars.
	virtual bool DoScroll(CWnd* pParentWnd, int nScrollStepsX, int nScrollStepsY) const;
	// And here we know the Origin and Destination, so really Do scroll the window 
	// to the new position. It is only needed for the default "DoScroll"
	virtual bool DoScrollWindow(CWnd* pParentWnd, int nScrollToX, int nOriginX, int nScrollToY, int nOriginY) const;

	// Scroll one step if you have to scroll ? pixels
	virtual int GetPixelsInAStepX  (CWnd* /*pParentWnd*/, int /*nScrollX*/) const {return 1;};
	virtual int GetPixelsInAStepY  (CWnd* /*pParentWnd*/, int /*nScrollY*/) const {return 1;};
	// Scroll by ? substeps if scrolling one step
	virtual int GetSubstepsInAStepX(CWnd* /*pParentWnd*/, int /*nScrollX*/) const {return 1;};
	virtual int GetSubstepsInAStepY(CWnd* /*pParentWnd*/, int /*nScrollY*/) const {return 1;};

	// Can scroll this window?
	virtual bool NoVertScroll(CWnd* pParentWnd) const;
	virtual bool NoHorzScroll(CWnd* pParentWnd) const;

protected:
	LPTSTR  m_szClassName;
	UINT    m_nWindowResID;
};

/*	control specific classes 
	all derived from "CAutoPanParameters"

	class CAutoPanParametersMessage;
	class CAutoPanParametersLineMessages;
	class CAutoPanParametersRegMessage;

	class CAutoPanParametersSysTreeView32;
	class CAutoPanParametersRICHEDIT;
	class CAutoPanParametersEdit;
	class CAutoPanParametersSysListView32;
	class CAutoPanParametersListBox;
	class CAutoPanParametersComboBox;
	class CAutoPanParametersmsctls_updown32;
*/

#include "mfxWhlPan.inl"



/////////////////////////////////////////////////////////////////////////////
// CWheelWnd window

class CWheelWnd : public CWnd
{
// Construction
public:
	CWheelWnd();
	virtual ~CWheelWnd();

	BOOL Create(CWnd* pParentWnd, WORD wStyle = MFX_WHEELWNDSTYLE_DEFAULT, CAutoPanParameters* pAutoPanParameters = NULL);
	
	static void Register(CAutoPanParameters* pAutoPanParameters);

// Attributes
private:
	CWnd*	m_pParentWnd;
	CAutoPanParameters* m_pRealisation;

	CPoint	m_ptWheelOrg;
	CBitmap	m_bmpOrigin;
	CSize   bitmapSize;
	CSize   m_WindowSize;

	int		m_nOriginBmpIndex;
	UINT	m_nWheelTimer;
	bool	m_bNoVertScroll;
	bool	m_bNoHorzScroll;
	WORD    m_wStyle;
	CPoint  m_ptScrollSum;

	static CAutoPanParametersMessage             m_AutoPanParametersMessage;
	static CAutoPanParametersLineMessages        m_AutoPanParametersLineMessages;
	static CAutoPanParametersRegMessage          m_AutoPanParametersRegMessage;
	static CAutoPanParametersMessageThumbTrack   m_AutoPanParametersMessageThumbTrack;

	static CList<CAutoPanParameters*, CAutoPanParameters*> m_lstAutoPanParameters;
	static bool m_bIsInitialized;
	static bool Initialize();


// Operations
private:
	void SetCursor(int nCursor);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWheelWnd)
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CWheelWnd)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg LONG DestroyThisWindow(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//template <> void AFXAPI DestructElements <CAutoPanParameters*> ( CAutoPanParameters** ppAutoPanParameters, int nCount );

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFXWHLPAN_H__4E9C2711_CFCD_11D1_87BA_400011900025__INCLUDED_)
