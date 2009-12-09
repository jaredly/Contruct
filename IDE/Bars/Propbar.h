#if !defined(AFX_PROFSTUDIOPROPERTIES_H__0C90A6DC_F02D_495D_954E_5773278F2CF4__INCLUDED_)
#define AFX_PROFSTUDIOPROPERTIES_H__0C90A6DC_F02D_495D_954E_5773278F2CF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProfStudioProperties.h : header file
//

#if (defined  __EXT_MFC_NO_PROPERTYGRIDWND)

#if (!defined __AFXPRIV_H__)
	#include <AfxPriv.h>
#endif

#if (!defined AFX_PROFSTUDIOTREECTRL_H__8804C1F7_CD90_45A4_87F4_82DBB63EA81D__INCLUDED_)
//	#include "ProfStudioTreeCtrl.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfStudioProperties dialog

class CProfStudioProperties : public CExtResizableDialog
{
	class CInnerToolControlBar : public CExtToolControlBar
	{
		virtual CExtBarContentExpandButton * OnCreateBarRightBtn()
		{
			return NULL;
		}
	};

	class CPropertiesNameBox : public CWnd
	{
	public:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
			if( message == WM_ERASEBKGND )
				return 1;
			if( message == WM_PAINT )
			{
				CRect rcClient;
				GetClientRect( &rcClient );
				CPaintDC dcPaint( this );
				CExtMemoryDC dc( &dcPaint, &rcClient );
				bool bNoFill = false;
				if( g_PaintManager->GetCb2DbTransparentMode(this) )
					bNoFill =
						g_PaintManager->PaintDockerBkgnd(
							true,
							dc,
							this
							);
				if( !bNoFill )
					dc.FillSolidRect(
						&rcClient,
						g_PaintManager->GetColor(
							CExtPaintManager::CLR_3DFACE_OUT,
							this
							)
						);
				COLORREF clrThinFrame =
					g_PaintManager->GetColor(
						COLOR_3DSHADOW,
						this
						);
				dc.Draw3dRect(
					&rcClient,
					clrThinFrame,
					clrThinFrame
					);
				CRect rcText( rcClient );
				if( rcText.left < rcText.right )
				{
					CString strWindowText;
					GetWindowText( strWindowText );
					if( !strWindowText.IsEmpty() )
					{
						int nOldBkMode = dc.SetBkMode( TRANSPARENT );
						COLORREF clrOldText =
							dc.SetTextColor(
								g_PaintManager->GetColor( COLOR_BTNTEXT, this )
								);
						CFont * pOldFont = dc.SelectObject( &g_PaintManager->m_FontNormal );
						dc.DrawText(
							strWindowText,
							&rcText,
							DT_SINGLELINE|DT_LEFT|DT_VCENTER
							);
						dc.SelectObject( pOldFont );
						dc.SetTextColor( clrOldText );
						dc.SetBkMode( nOldBkMode );
					} // if( !strWindowText.IsEmpty() )
				} // if( rcText.left < rcText.right )
				return 0;
			} // if( message == WM_PAINT )
			return CWnd::WindowProc(message,wParam,lParam);
		}
	};

	class CPropertiesComboBox : public CComboBox
	{
	public:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch( message )
			{
			case WM_SIZEPARENT:
				{
					CRect rcWnd;
					CSize _sizeNeeded;
					
					AFX_SIZEPARENTPARAMS * lpLayout =
						(AFX_SIZEPARENTPARAMS*)lParam;
					ASSERT( lpLayout != NULL );
					
					GetWindowRect( &rcWnd );
					_sizeNeeded = rcWnd.Size();
					lpLayout->rect.top += _sizeNeeded.cy;
					lpLayout->sizeTotal.cy += _sizeNeeded.cy;

					CWnd * pWndFrame = GetParent()->GetDlgItem(IDC_STATIC_PROPERTY_FRAME);
					pWndFrame->GetWindowRect( &rcWnd );
					_sizeNeeded = rcWnd.Size();
					_sizeNeeded.cy += 4;
					lpLayout->rect.bottom -= _sizeNeeded.cy;
					lpLayout->sizeTotal.cy += _sizeNeeded.cy;
				}
				return 0;
			}
			return CComboBox::WindowProc(message,wParam,lParam);
		}
	};

	CImageList m_ImageList;
	void _InitPropertiesList();
	bool m_bAlphabeticView:1;

// Construction
public:
	CProfStudioProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProfStudioProperties)
	enum { IDD = IDD_DIALOG_PROPERTIES };
	CExtLabel	m_wndStaticPropertyComment2;
	CExtLabel	m_wndStaticPropertyComment1;
	//}}AFX_DATA

	CExtWFF < CListCtrl > m_ListProperties;
	CPropertiesComboBox	m_ComboProperties;
	CInnerToolControlBar m_wndToolBar;
	CPropertiesNameBox m_wndBottomBox;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfStudioProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProfStudioProperties)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPropsAlphabetic();
	afx_msg void OnPropsCategorized();
	afx_msg void OnPropsEvents();
	afx_msg void OnPropsMessages();
	afx_msg void OnPropsOverrides();
	afx_msg void OnPropsProperties();
	afx_msg void OnPropsPropertyPages();
	afx_msg void OnUpdatePropsAlphabetic(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePropsCategorized(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CProfStudioProperties

#else // (defined  __EXT_MFC_NO_PROPERTYGRIDWND)

// Overridden for itemExpand

/*
class CExtPropertyGridWndCategorizedEx : public CExtPropertyGridWndCategorized {
public:
	CExtPropertyGridWndCategorizedEx(CExtPropertyGridCtrl* pPropertyGridCtrl)
	{ CExtPropertyGridWndCategorized::CExtPropertyGridWndCategorized(pPropertyGridCtrl); }

	virtual void ItemExpand(HTREEITEM hTreeItem, INT nActionTVE = 3, bool bRedraw = true);
};
*/

class CExtPropertyGridWndCategorizedEx : public
CExtPropertyGridWndCategorized {
public:
    CExtPropertyGridWndCategorizedEx(
        CExtPropertyGridCtrl * pPropertyGridCtrl = NULL
        )
        : CExtPropertyGridWndCategorized( pPropertyGridCtrl )
    {
    }

	vector<map<CString, bool>*> expandSaves;

	void OnItemExpand(CExtPropertyItem* pItem)
	{
		vector<map<CString, bool>*>::iterator i = expandSaves.begin();

		for ( ; i != expandSaves.end(); i++)
			(**i)[pItem->NameGet()] = !pItem->ExpandedGet();
	}
    
    virtual void ItemExpand(
        HTREEITEM hTreeItem,
        INT nActionTVE = TVE_TOGGLE,
        bool bRedraw = true
        )
    {
        CExtPropertyGridWndCategorized::ItemExpand(
            hTreeItem,
            nActionTVE,
            bRedraw
            );

		OnItemExpand(PropertyItemFromTreeItem(hTreeItem));
    }
};

/////////////////////////////////////////////////////////////////////////////
// CProfStudioPropertyGridCtrl window
class CProfStudioPropertyGridCtrl : public CExtPropertyGridCtrl
{
public:
	CExtPropertyStore m_PS;
	CProfStudioPropertyGridCtrl();
	~CProfStudioPropertyGridCtrl();

	CLayoutEditor* pCurView;
	class PropertiesBar* pPropBar;
	CExtPropertyGridWndCategorizedEx* pGridCategorized;

	// Delete all items and clear the property value managers map
	void Reset();

	// Override input completion for saving values
	virtual void OnPgcInputComplete(CExtPropertyGridWnd * pPGW, CExtPropertyItem * pPropertyItem);
	virtual bool OnPgcCreateGrids();

	void DoItemChange(CExtPropertyItem* pItem);

	map<CExtPropertyItem*, CPropValueMgr*> propValueMgrs;

protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
}; // class CProfStudioPropertyGridCtrl


#endif // else from (defined  __EXT_MFC_NO_PROPERTYGRIDWND)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFSTUDIOPROPERTIES_H__0C90A6DC_F02D_495D_954E_5773278F2CF4__INCLUDED_)
