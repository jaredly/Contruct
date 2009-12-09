// StackedWndCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "StackedWndCtrl.h"
#include "..\Dialogs\ParametersDlg.h"
#include "..\Utilities\Helper.h"

#include "..\Dialogs\AddVariableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CStackedWndCtrl, CExtResizableDialog)
	//{{AFX_MSG_MAP(CStackedWndCtrl)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_KEYDOWN()
	ON_NOTIFY(SCN_CHARADDED, 10000, OnChar)
	ON_NOTIFY(SCN_MODIFIED, 10000, OnChange)
	ON_CBN_SELCHANGE(555, OnChange)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RUBRIC_WND_CLICKED_ON, OnRubricWndClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStackedWndCtrl

CStackedWndCtrl::CStackedWndCtrl()
{
	m_iRubricHeight = 20;
	updateTwice = false;
}

void CStackedWndCtrl::OnChange() // For variable combo boxs only
{
	int addid = 0;
	// first we check what the item is.
	CWnd* pWnd = GetFocus();
	if(!pWnd) 
		return;

	if(!dynamic_cast<CExtComboBox*>(pWnd))
		return;

	CExtComboBox* combo = (CExtComboBox*)pWnd;

	if(combo->GetCurSel() != combo->GetCount()-1)
		return;
	
	CParametersDlg* dlg = (CParametersDlg*)GetWindowLong(m_hWnd, GWL_USERDATA);
	CObjType* pType = dlg->m_ID;

	CAddVariableDlg Dlg;
	if (Dlg.DoModal() != IDOK)
	{
		// set selection to -1
		combo->SetCurSel(-1);
		return;
	}

	// Check this variable name does not already exist
	if (exists_element(pType->m_PrivateValues.begin(), pType->m_PrivateValues.end(), Dlg.Name)) {
		MessageBox(CString("The variable name '") + Dlg.Name + "' is already used in this object.  Please choose another name.", "Private Variable", MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	if(pType->ObjectIdentifier == -1) // System
	{
		addid = dlg->application->m_varID;
		dlg->application->AddGlobalVariable((CApplication::GlobalVariable(Dlg.Name, Dlg.Type, Dlg.Value, dlg->application->m_varID++)));
	}
	else
	{
		// Check if it's a family
		if (dlg->application->FindFamilyFromNumber(pType->ObjectIdentifier) == NULL)
		{
			int varID = pType->VariableID++;
			addid = varID;
			//pType->m_PrivateValues.erase(pType->m_PrivateValues.begin() + 4);
			pType->m_PrivateValues.push_back(PrivateValue(Dlg.Name, Dlg.Type, varID));

			PrivateVariableInstanceAdder updater(varID, Dlg.Value);
			ForEachObjectInstance(pType, dlg->application, updater);
		}

		// If it is, add to all members
		else
		{
			Family* pFamily = dlg->application->FindFamilyFromNumber(pType->ObjectIdentifier);

			vector<CObjType*> objects;
			pFamily->GetObjectTypes(objects);

			for (int i = 0; i < objects.size(); i++)
			{
				int Var = objects[i]->VariableID++;
				addid = Var;
				objects[i]->m_PrivateValues.push_back(PrivateValue(Dlg.Name, Dlg.Type, Var));
	
				PrivateVariableInstanceAdder updater(Var, Dlg.Value);
				ForEachObjectInstance(objects[i], dlg->application, updater);
			}
		}
	}

	combo->InsertString(combo->GetCount()-1,Dlg.Name);
	combo->SetItemData(combo->GetCount()-2, addid);
	combo->SetCurSel(combo->GetCount() -2 );
}

CStackedWndCtrl::~CStackedWndCtrl()
{
	for( int i = 0; i < m_arrPanes.GetSize(); i++ )
	{
		// Delete the rubric window
		m_arrPanes[ i ]->rubric->DestroyWindow();

		delete m_arrPanes[ i ]->rubric;
		
		// Delete the content window
		m_arrPanes[ i ]->content_window->DestroyWindow();

		delete m_arrPanes[ i ]->content_window;
		
		// Delete structure
		delete m_arrPanes[ i ];
	}

	m_arrPanes.RemoveAll();
}

int CStackedWndCtrl::AddPane( CWnd* pwndRubric, TDS_PANE Pane )
{
	// Create a new pane structure
	PTDS_PANE pPane = new TDS_PANE;

	if( pPane == NULL )
	{
		AfxMessageBox( "Failed to add a new pane to the stack.\n\nOut of memory." );
		return -1;
	}

	// Copy the pointers to the rubric and content windows
	// Also, set this pane as open
	pPane->rubric		= pwndRubric;
	pPane->content_window   = Pane.content_window;
	pPane->type				= Pane.type;
	pPane->open			= TRUE;
	pPane->keep_string		= Pane.keep_string;
	pPane->use_private_data	= Pane.use_private_data;
	
	// Add the new pane to the end of the stack
	int iIndex = m_arrPanes.Add( pPane );

	// Rearrange the stack
	RearrangeStack();

	// Return the index of the new pane
	return iIndex;
}

void CStackedWndCtrl::RearrangeStack()
{

#define STACK_SPACE 3
    CRect rFrame;
    GetClientRect( &rFrame );
	rFrame.OffsetRect(0,-m_ScrollY);
	int offsety = rFrame.top;

	

    for( int i = 0; i < m_arrPanes.GetSize(); i++ )
    {
		// Rubric windows are always visible
        m_arrPanes[ i ]->rubric->SetWindowPos(  NULL,
                                                    0,
                                                    rFrame.top,
                                                    rFrame.Width(),
                                                    m_iRubricHeight,
                                                    SWP_NOZORDER | SWP_SHOWWINDOW );
		m_arrPanes[ i ]->rubric->Invalidate();
		// The top of the frame is offset by the height of a rubric window
        rFrame.OffsetRect(0,m_iRubricHeight);

		// Only the content window of the flagged pane is shown
		// All others are hidden if they aren't already
        if( m_arrPanes[ i ]->open )
        {

			// From the bottom of the frame, take off as many rubric
			// window's heights as there are left to display
           // int iContentWndBottom = rFrame.bottom - ( ( m_arrPanes.GetSize() - i ) * m_iRubricHeight );

			CRect WindowRect;
			m_arrPanes[ i ]->content_window->GetWindowRect(WindowRect);
            m_arrPanes[ i ]->content_window->SetWindowPos(   NULL,
                                                            0,
                                                            rFrame.top +STACK_SPACE,
                                                            rFrame.Width(),
                                                            WindowRect.Height(),//iContentWndBottom - rFrame.top,
                                                            SWP_NOZORDER | SWP_SHOWWINDOW );
			CScintillaWnd* wnd = dynamic_cast<CScintillaWnd*>(m_arrPanes[i]->content_window);
			if(wnd)
			{
				CParametersDlg* dlg = dynamic_cast<CParametersDlg*>(GetParent());
				if(dlg)
					dlg->ResizeScintilla(*wnd);
			}
			m_arrPanes[ i ]->content_window->GetWindowRect(WindowRect);
			m_arrPanes[ i ]->content_window->Invalidate();
			// The next rubric window will be placed right below
			// this pane's content window
            rFrame.OffsetRect(0,WindowRect.Height()+STACK_SPACE+STACK_SPACE);//iContentWndBottom; //note: changed to +=
        }
        else
            m_arrPanes[ i ]->content_window->ShowWindow( SW_HIDE );


    }
	// All done...phew!
	// Now we need to adjust the scrollbar :)

	SCROLLINFO info;
	GetScrollInfo(SB_VERT,&info);
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_RANGE | SIF_PAGE;
	info.nMax = rFrame.top-offsety;
	info.nMin = 0;
	info.nPage = rFrame.Height();

	if(info.nMax-(int)(info.nPage)+1 < info.nPos) // we need to adjust the scrollpos
	{
		info.nPos = max(0,info.nMax-(int)(info.nPage)+1);
		info.fMask |= SIF_POS;
		m_ScrollY = info.nPos;
	}

	this->SetScrollInfo(SB_VERT,&info);
	

	Invalidate();
	if(!updateTwice)
	{
		if(this->GetSafeHwnd())
		{
			if(GetParent()->GetSafeHwnd())
			{
				GetParent()->SetTimer(15,10,NULL);
			}
		}
	}

	updateTwice = 1 - updateTwice;
}

void CStackedWndCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch( nChar ) {
		case VK_TAB:
			1;
			break;
     }
	CExtResizableDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CStackedWndCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SetRedraw(false);
    int iScrollBarPos = GetScrollPos( SB_VERT );

    CRect rFrame;

    GetClientRect( rFrame );

    switch( nSBCode )
    {
        case SB_LINEUP:
            iScrollBarPos = max( iScrollBarPos - 1, 0 );
        break;

        case SB_LINEDOWN:
            iScrollBarPos = min( iScrollBarPos + 1, 
                                 GetScrollLimit( SB_VERT ) );
        break;

        case SB_PAGEUP:
            iScrollBarPos = max( iScrollBarPos - rFrame.Height(), 0 );
        break;

        case SB_PAGEDOWN:
            iScrollBarPos = min( iScrollBarPos + rFrame.Height(), 
                                 GetScrollLimit( SB_VERT ) );
        break;

        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
        {
            SCROLLINFO si;

            ZeroMemory( &si, sizeof(SCROLLINFO) );

            si.cbSize = sizeof(SCROLLINFO);
            si.fMask  = SIF_TRACKPOS;

            if( GetScrollInfo( SB_VERT, &si, SIF_TRACKPOS ) )
                iScrollBarPos = si.nTrackPos;
            else
                iScrollBarPos = (UINT)nPos;
            break;
        }
    }        

    SetScrollPos( SB_VERT, iScrollBarPos );


	m_ScrollY = iScrollBarPos;


	
    RearrangeStack();

	Invalidate(true);
	SetRedraw(true);
	RedrawWindow();
//	CView::OnVScroll(nSBCode, nPos, pScrollBar);

}



/////////////////////////////////////////////////////////////////////////////
// CStackedWndCtrl message handlers

void CStackedWndCtrl::PreSubclassWindow() 
{
	// Remove the black frame and clip children to reduce flickering
	//ModifyStyle( SS_BLACKFRAME, WS_CLIPCHILDREN );
	ModifyStyle( SS_BLACKFRAME, 0);
	m_ScrollY = 0;
	CExtResizableDialog::PreSubclassWindow();
}

void CStackedWndCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CExtResizableDialog::OnSize(nType, cx, cy);

	RearrangeStack();
}

LRESULT CStackedWndCtrl::OnRubricWndClicked(WPARAM wParam, LPARAM /*lParam*/)
{
	GetParent()->SendMessage(1000, 0, 0);
	HWND hwndRubric = (HWND)wParam;
	BOOL bRearrange = FALSE;

	for( int i = 0; i < m_arrPanes.GetSize(); i++ )
		if( m_arrPanes[ i ]->rubric->m_hWnd == hwndRubric )
		{
			// Rearrange the control only if a rubric window
			// other than the one belonging to the pane that
			// is currently open is clicked on
		//	if( m_arrPanes[ i ]->open == FALSE )
		//	{
				m_arrPanes[ i ]->open = !m_arrPanes[ i ]->open;
				if (m_arrPanes[ i ]->open)
					m_pPane = m_arrPanes[i];

				bRearrange = TRUE;
			//}
		}
	//	else
//			m_arrPanes[ i ]->open = FALSE;

	if( bRearrange )
		RearrangeStack();

	// In case the rubric window that has sent the message wants to know
	// if the control has been rearranged, return the flag
	return bRearrange;
}

int CStackedWndCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	ModifyStyle(0,WS_VSCROLL);
	CExtResizableDialog::OnCreate(lpCreateStruct);


	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;
	info.nMax = 20;
	info.nMin = 0;
	info.nPage = 10;
	info.nPos = 0;
	info.nTrackPos = 0;

	this->SetScrollInfo(SB_VERT,&info,false);

	ShowSizeGrip(0);
	
	return 0;
}

void CStackedWndCtrl::OnChar(NMHDR *pNMHDR, LRESULT *pResult)
{
	CWnd* pWnd = GetFocus();
	if(pWnd)
		if(dynamic_cast<CScintillaWnd*>(pWnd))
			((CParametersDlg*)GetParent())->OnChar(pNMHDR, pResult,*(CScintillaWnd*)(pWnd));
}

void CStackedWndCtrl::OnChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	CWnd
		* pWnd = GetFocus();
	if(pWnd)
		if(dynamic_cast<CScintillaWnd*>(pWnd))
			((CParametersDlg*)GetParent())->OnChange(pNMHDR, pResult,*(CScintillaWnd*)(pWnd));
}

BOOL CStackedWndCtrl::CreateScrollBarCtrl(DWORD dwStyle, UINT nID) 
{
	CExtScrollBar* myscroll = new CExtScrollBar;
	myscroll->Create(dwStyle, CRect(0,0,256,20), this, nID);
	
	return TRUE;
}

LRESULT CStackedWndCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND) {

		// A lovely hack to redraw the parameters dialog and stop the description string glitching.
		if (wParam >> 16 == SCEN_SETFOCUS) {
			((CParametersDlg*)GetParent())->OnFocusChange(0);
		}
		if (wParam >> 16 == SCEN_KILLFOCUS) {
			((CParametersDlg*)GetParent())->OnFocusChange(0);
		}

		((CParametersDlg*)GetParent())->Invalidate();
		
	}

	return CExtResizableDialog::WindowProc(message, wParam, lParam);
}

BOOL CStackedWndCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult)
{

	NMHDR *phDR;
	phDR = (NMHDR*)lParam;

   
   // Find what window it is from.
	CScintillaWnd* pScintellaWndFrom = 0;
	for(int i =0; i < 	m_arrPanes.GetSize(); i ++)  {
		if(m_arrPanes.GetAt(i)->content_window->m_hWnd == phDR->hwndFrom) {
			pScintellaWndFrom = (CScintillaWnd*)m_arrPanes.GetAt(i)->content_window;
		}
	}

	((CParametersDlg*)GetParent())->OnFocusChange(0);

	if(!pScintellaWndFrom) 
		return CExtResizableDialog::OnNotify(wParam, lParam, pResult);

// does notification come from my scintilla window?
  // if (phDR != NULL && phDR->hwndFrom == m_ScinCtrl.m_hWnd)
   {
      SCNotification *pMsg = (SCNotification*)lParam;
	//	if(phDR->code != 2013)

	  switch (phDR->code)
      {
		  case SCEN_SETFOCUS:
			  {

			  }
			  break;
         case SCN_STYLENEEDED:
         break;
         case SCN_CHARADDED:
         break;
		 case SCN_MODIFIED:
		 {
			 bool ret = CExtResizableDialog::OnNotify(wParam, lParam, pResult);
//			((CParametersDlg*)this->GetParent())->ResizeScintilla(*pScintellaWndFrom);
			this->RearrangeStack();
			return ret;
		 }
		 break;
         case SCN_SAVEPOINTREACHED:
         break;
//       called when the document is changed - mark document modified
         case SCN_SAVEPOINTLEFT:
         break;
         case SCN_MODIFYATTEMPTRO:
         break;
         case SCN_KEY:
         break;
         case SCN_DOUBLECLICK:
         break;
//       called when something changes and we want to show new indicator state or brace matching
         case SCN_UPDATEUI:
         {
            pScintellaWndFrom->UpdateUI();
         }
         break;
         case SCN_MACRORECORD:
         break;
//       user clicked margin - try folding action
         case SCN_MARGINCLICK:
         {
         //   pScintellaWndFrom->DoDefaultFolding(pMsg->margin, pMsg->position);
         }
         break;
         case SCN_NEEDSHOWN:
         break;
         case SCN_PAINTED:
         break;
         case SCN_USERLISTSELECTION:
         break;
         case SCN_URIDROPPED:
         break;
         case SCN_DWELLSTART:
         break;
         case SCN_DWELLEND:
         break;
      }
     // return TRUE; // we processed the message
   }
	return CExtResizableDialog::OnNotify(wParam, lParam, pResult);

}

BOOL CStackedWndCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		// Check for moving thru param methods
		if (pMsg->wParam == VK_UP) {

			CParametersDlg* paramDlg = (CParametersDlg*)GetParent();
			if (paramDlg->m_Tooltip.IsWindowVisible()) {
				if (paramDlg->pWndFocusParam->SendMessage(SCI_AUTOCACTIVE, 0, 0)) {
					int current = paramDlg->pWndFocusParam->SendMessage(SCI_AUTOCGETCURRENT, 0, 0);
						paramDlg->pWndFocusParam->SendMessage(SCI_AUTOCSELECT, 0,
							(LPARAM)(const char*)paramDlg->application->m_sort.GetAt(max(0,current-1)));
				}
				paramDlg->m_Tooltip.ShowPrevMethod();
				return TRUE;
			}

			if(GetKeyState(VK_CONTROL) >> 4)
				return this->GetParent()->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		}
		else if (pMsg->wParam == VK_DOWN) {
			CParametersDlg* paramDlg = (CParametersDlg*)GetParent();
			if (paramDlg->m_Tooltip.IsWindowVisible()) {
				if (paramDlg->pWndFocusParam->SendMessage(SCI_AUTOCACTIVE, 0, 0)) {
					int current = paramDlg->pWndFocusParam->SendMessage(SCI_AUTOCGETCURRENT, 0, 0);
						paramDlg->pWndFocusParam->SendMessage(SCI_AUTOCSELECT, 0,
							(LPARAM)(const char*)paramDlg->application->m_sort.GetAt(
								min(paramDlg->application->m_sort.GetCount()-1,current+1)));
				}
				paramDlg->m_Tooltip.ShowPrevMethod();
				return TRUE;
			}

			if(GetKeyState(VK_CONTROL) >> 4)
				return this->GetParent()->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		}
		else if (pMsg->wParam == VK_RETURN)
		{
			if(!(GetKeyState(VK_SHIFT)>>4))
			{
				CParametersDlg* paramDlg = (CParametersDlg*)GetParent();
				paramDlg->pWndFocusParam->SendMessage(SCI_AUTOCCOMPLETE, 0, 0); 
				
				if(GetKeyState(VK_CONTROL) >> 4)
					return this->GetParent()->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

				return TRUE;
			}
		}
	}

	return CExtResizableDialog::PreTranslateMessage(pMsg);
}