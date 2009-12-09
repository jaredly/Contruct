/* ==========================================================================
	CIconEdit
	 
	Author :		Johan Rosengren, Abstrakt Mekanik AB
	
	Date :			2004-03-09
	
	Purpose :		The class represents an editbox with an icon to the left	
	
	Description :	The class sets the edit rect of the editbox, and adds a
					CStatic derived control painting the icon to the left of 
					the text.
	
	Usage :			As a CEdit. The class can be instantiated both  
					dynamically and from a dialog template. The edit box 
					must have the  ES_MULTILINE style set (as the edit rect 
					will not work otherwise). The icon can either be set as 
					an icon handle, with CIconEdit::SetIcon(HICON icon), or 
					directly from the application resources with 
					CIconEdit::SetIcon(UINT iconres). In the latter case, 
					the icon will be loaded and deleted by the class, in the 
					former, the caller must delete it.
	
					Note that SetWindowPlacement will destroy the edit rect, 
					if the control needs to be moved or resized, MoveWindow 
					or SetWindowPos should be used instead.

   ========================================================================
	Changes 
		27/5 2004	Bug found by David Pritchard - HICON version of SetIcon 
					did not create the icon control. Moved icon control 
					creation code to a separate function, called from both 
					versions of SetIcon.
   ========================================================================
		10/6 2004	Yet another bug found by David Pritchard - and this is 
					such a small project!. Filling the background with 
					COLOR_WINDOW instead of GetBkColor to make the 
					background being properly colored on Win2000 as well.
   ========================================================================*/

#include "stdafx.h"
#include "IconEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconEdit

CIconEdit::CIconEdit()
/*============================================================
	Function :		CIconEdit::CIconEdit

	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :			
  ============================================================*/
{

	m_internalIcon = NULL;
	m_icon.m_hWnd = NULL;

}

CIconEdit::~CIconEdit()
/*============================================================
	Function :		CIconEdit::~CIconEdit

	Description :	
	Return :		destructor	-	
	Parameters :
	Usage :			
  ============================================================*/
{

	// Clean up
	if( m_internalIcon )
		::DestroyIcon( m_internalIcon );

}

BEGIN_MESSAGE_MAP(CIconEdit, CEdit)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CIconEdit::PreSubclassWindow() 
/*============================================================
	Function :		CIconEdit::PreSubclassWindow

	Description :	This function is called before the control
					is subclassed for a control on a dialog 
					template, and during creation for 
					dynamically created controls.

	Return :		void
	Parameters :	

	Usage :			
  ============================================================*/
{
	// We must have a multiline edit
	// to be able to set the edit rect
	ASSERT( GetStyle() & ES_MULTILINE );

	// We set the edit rect
	Prepare();

}

/////////////////////////////////////////////////////////////////////////////
// CIconEdit implementation

void CIconEdit::SetIcon( HICON icon ) 
/*============================================================
	Function :		CIconEdit::SetIcon

	Description :	Sets a new icon and updates the control
					
	Return :		void	
	Parameters :	HICON icon	-	Handle to the icon

	Usage :			The caller must destroy the icon

  ============================================================*/
{

	CreateIcon();

	// Update the icon control
	m_icon.SetIcon( icon );

}

void CIconEdit::SetIcon( UINT iconres ) 
/*============================================================
	Function :		CIconEdit::SetIcon

	Description :	Sets a new icon and updates the control
					
	Return :		void
	Parameters :	UINT iconres	-	Resource id of the icon

	Usage :			The class will load and destroy the icon.

  ============================================================*/
{

	CreateIcon();

	// If we already have an icon, we destroy it
	if( m_internalIcon )
		::DestroyIcon( m_internalIcon );

	// Loading the new icon
	m_internalIcon = ( HICON ) ::LoadImage( AfxGetResourceHandle(),
								MAKEINTRESOURCE( iconres ),
								IMAGE_ICON,
								16,
								16,
								LR_DEFAULTCOLOR );

	ASSERT( m_internalIcon != NULL );

	// Update the icon control
	m_icon.SetIcon( m_internalIcon );

}

/////////////////////////////////////////////////////////////////////////////
// CIconEdit implementation

void CIconEdit::Prepare() 
/*============================================================
	Function :		CIconEdit::Prepare

	Description :	Sets the edit rect of the control

	Return :		void
	Parameters :	none

	Usage :			Internal function. Call as soon as the 
					edit rect needs to be (re)established

  ============================================================*/
{

	// Creating the edit rect
	int width = GetSystemMetrics( SM_CXSMICON );

	CRect editRect;
	GetRect( &editRect );
	editRect.left += width + 6;

	SetRect( &editRect );

}

/////////////////////////////////////////////////////////////////////////////
// CIconEdit message handlers

LRESULT CIconEdit::OnSetFont( WPARAM wParam, LPARAM lParam )
/*============================================================
	Function :		CIconEdit::OnSetFont

	Description :	Mapped to WM_SETFONT
					
	Return :		LRESULT	-	
	Parameters :	WPARAM wParam	-	From Windows
					LPARAM lParam	-	From Windows

	Usage :			Called from Windows

  ============================================================*/
{

	DefWindowProc( WM_SETFONT, wParam, lParam );

	// We reset the edit rect
	Prepare();

	return 0;

}

void CIconEdit::OnSize( UINT nType, int cx, int cy ) 
/*============================================================
	Function :		CIconEdit::OnSize

	Description :	Mapped to WM_SIZE
					
	Return :		void
	Parameters :	UINT nType	-	From Windows
					int cx		-	From Windows
					int cy		-	From Windows

	Usage :			Called from Windows

  ============================================================*/
{

	CEdit::OnSize( nType, cx, cy );

	// We reset the edit rect
	Prepare();

}

/////////////////////////////////////////////////////////////////////////////
// CIconEdit private helpers

void CIconEdit::CreateIcon()
/* ============================================================
	Function :		CIconEdit::CreateIcon
	Description :	A helper that creates the icon window if 
					it's not already created.
					
	Return :		void
	Parameters :	none

	Usage :			Called internally when the icon is set.

   ============================================================*/
{

	if( !m_icon.m_hWnd )
	{
		
		// If the icon window doesn't exist,
		// we create it
		CRect editRect;
		GetRect( &editRect );
		CRect clientRect;
		GetClientRect( &clientRect );
		CRect iconRect( 0, 0, editRect.left, clientRect.Height() );
		if(m_icon.m_hWnd==NULL)
			m_icon.Create("", WS_CHILD | WS_VISIBLE, iconRect, this, 1 );

	}

}

/////////////////////////////////////////////////////////////////////////////
// CIconWnd


CIconWnd::CIconWnd()
/*============================================================
	Function :		CIconWnd::CIconWnd

	Description :	constructor
					
	Return :		void
	Parameters :	

	Usage :			

  ============================================================*/
{

	m_icon = NULL;

}

CIconWnd::~CIconWnd()
/*============================================================
	Function :		CIconWnd::~CIconWnd

	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

  ============================================================*/
{
}

BEGIN_MESSAGE_MAP(CIconWnd, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconWnd message handlers

void CIconWnd::OnPaint() 
/*============================================================
	Function :		CIconWnd::OnPaint

	Description :	Mapped to WM_PAINT. Draws the icon with the
					small icon size (regardless of original)
					
	Return :		void
	Parameters :	none

	Usage :			Called from Windows

  ============================================================*/
{

	CPaintDC dc( this );
	if( m_icon )
	{
		CRect rect;
		GetClientRect( &rect );

		// Clearing the background
		dc.FillSolidRect( rect, GetSysColor(COLOR_WINDOW) );

		// Drawing the icon
		int width = GetSystemMetrics( SM_CXSMICON );
		int height = GetSystemMetrics( SM_CYSMICON );
		::DrawIconEx( dc.m_hDC, 1, 1, m_icon, width, height, 0, NULL, DI_NORMAL );
	}

}

BOOL CIconWnd::OnEraseBkgnd( CDC* ) 
/*============================================================
	Function :		CIconWnd::OnEraseBkgnd

	Description :	Mapped to the WM_ERASEBKGND
					
	Return :		BOOL		-	Always TRUE
	Parameters :	CDC*		-	From Windows
				
	Usage :			Called from Windows

  ============================================================*/
{
	return TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// CIconWnd implementation

void CIconWnd::SetIcon( HICON icon )
/*============================================================
	Function :		CIconWnd::SetIcon

	Description :	Called to set/change the icon
					
	Return :		void
	Parameters :	HICON icon	-	icon to use
				
	Usage :			The function is called from the CEditIcon 
					class

  ============================================================*/
{
	m_icon = icon;
	if( ::IsWindow( m_hWnd ) )
		RedrawWindow();
}
