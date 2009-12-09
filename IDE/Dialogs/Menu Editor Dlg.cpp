// MenuEditorDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "MenuEditor.h"
#include "MenuEditorDlg.h"
#include "..\resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ITEMWIDTH 90
#define ITEMHEIGHT 18

void MenuEditorItem::Paint(CDC& dc, CRect& rect)
{
	switch( m_type)
	{
	case 0 : // menu
		PaintMenu(dc,rect);
		break;
	case 1: // popup
		PaintPopup(dc,rect);
		break; 
	case 2 : // separator
		PaintSeparator(dc,rect);
		break;		
	}
}
void MenuEditorItem::PaintPopup(CDC& dc, CRect& rect)
{
	// draw this item

	dc.FillSolidRect(rect, 0xffffff);
	dc.Draw3dRect(rect,GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	m_rect = rect;

	CRect temp = rect;
	
	if(pEditor->selected == this)
	{
		for(int x = temp.left; x < temp.right; x+=2)
		{
			dc.SetPixel(x,temp.top, 0);
			dc.SetPixel(x + 1- temp.Height()%2,temp.bottom-1, 0);
		}
		for(int y = temp.top; y < temp.bottom; y+=2)
		{
			dc.SetPixel(temp.left,y, 0);
			dc.SetPixel(temp.right-1,y +1- temp.Width()%2, 0);
		}
	}

	temp.DeflateRect(2,2);

	if(pEditor->selected == this)
		dc.FillSolidRect(temp, GetSysColor(COLOR_MENUHILIGHT));
	
	temp.left += 20;
	
	if(inserttext || m_Disabled)
		dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	else if(pEditor->selected == this)
		dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	else
		dc.SetTextColor(GetSysColor(COLOR_MENUTEXT));
	dc.DrawText(m_Text, temp, 0);

	//checkbox
	if(m_Checked)
	{
		COLORREF color = dc.GetTextColor();
		CRect drawzone = rect;
		drawzone.top += 8;
		drawzone.left += 5;
		drawzone.bottom = drawzone.top + 3;
		drawzone.right = drawzone.left + 1;
		for(int a = 0; a < 2;a++)
		{
			dc.FillSolidRect(drawzone, color);
			drawzone.OffsetRect(1,1);
		}
		for(int b = 0; b < 5;b++)
		{
			dc.FillSolidRect(drawzone, color);	
			drawzone.OffsetRect(1,-1);

		}

	}

	if (m_items.size() != 1 && m_items.size() != 0)
	{
		COLORREF color = dc.GetTextColor();
		CRect rect;
		rect = temp;
		rect.left = rect.right - 5;
		rect.right = rect.left + 1;
		rect.top += 4;
		rect.bottom -= 4;
		for(int a = 0; a < 4; a ++)
		{
			dc.FillSolidRect(rect, color);
			rect.top ++;
			rect.bottom --;
			rect.left ++;
			rect.right ++;
		}

	}

	///////////////////////

	rect.OffsetRect(0, rect.Height()+1);

}
void MenuEditorItem::PaintMenu(CDC& dc, CRect& rect)
{
	// draw this item

	dc.FillSolidRect(rect, 0xffffff);
	dc.Draw3dRect(rect,GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	m_rect = rect;

	CRect temp = rect;
	
	if(pEditor->selected == this)
	{
		for(int x = temp.left; x < temp.right; x+=2)
		{
			dc.SetPixel(x,temp.top, 0);
			dc.SetPixel(x + 1- temp.Height()%2,temp.bottom-1, 0);
		}
		for(int y = temp.top; y < temp.bottom; y+=2)
		{
			dc.SetPixel(temp.left,y, 0);
			dc.SetPixel(temp.right-1,y +1- temp.Width()%2, 0);
		}
	}

	temp.DeflateRect(2,2);

	if(pEditor->selected == this)
		dc.FillSolidRect(temp, GetSysColor(COLOR_MENUHILIGHT));
	

	temp.left += 20;
	
	if(inserttext)
		dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	else if(pEditor->selected == this)
		dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	else
		dc.SetTextColor(GetSysColor(COLOR_MENUTEXT));
	dc.DrawText(m_Text, temp, 0);

	rect.OffsetRect(rect.Width() + 1,0);

}
void MenuEditorItem::PaintSeparator(CDC& dc, CRect& rect)
{
	// Draw separator

	CRect sep = rect;
	sep.bottom = sep.top + 2;
	dc.FillSolidRect(sep, 0xffffff);
	dc.Draw3dRect(sep,GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	rect.OffsetRect(0, sep.Height()+1);
}

BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

HBRUSH CEditEx::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	if(bg == 0)
	{
		bg = new CBrush;
		bg->CreateSolidBrush(GetSysColor(COLOR_MENUHILIGHT)+0x202020);
	}

	//set text color
	pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));

	//set the text's background color
	pDC->SetBkColor(GetSysColor(COLOR_MENUHILIGHT)+0x202020);
	
	return (HBRUSH)bg->m_hObject;
}

CMenuEditorDlg::CMenuEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuEditorDlg::IDD, pParent)
{
	m_pMenu = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CMenuEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_SEPARATOR, m_Separator);
	DDX_Control(pDX, IDC_CHECKED, m_Checked);
	DDX_Control(pDX, IDC_DISABLED, m_Disabled);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMenuEditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(DM_GETDEFID, OnGetDefID)
	ON_COMMAND(IDENTER, OnEnter)
	//}}AFX_MSG_MAP
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CMenuEditor, CWnd)
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CMenuEditorDlg::OnEnter()
{
	m_Editor.OnEnter();
}

void CMenuEditor::OnEnter() 
{
//
} 

// accelerators
BOOL CMenuEditorDlg::PreTranslateMessage(MSG* pMsg) 
{ 
  if (pMsg->message == WM_KEYDOWN)
  {
	  if(GetFocus() != GetDlgItem(IDC_EDIT1))
	  {
		  m_Editor.OnKeyDown(0,0,0);
		  m_Editor.m_edit.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	  }
  }
	if (WM_KEYFIRST <= pMsg->message && 
        pMsg->message <= WM_KEYLAST) 
    { 
        if (::TranslateAccelerator(m_hWnd, m_Accelerators, pMsg)) 
            return TRUE; 
    } 

	//

    return CDialog::PreTranslateMessage(pMsg); 
} 

// stop enter closing dialog
LRESULT CMenuEditorDlg::OnGetDefID(WPARAM wp, LPARAM lp) 
{
    return MAKELONG(0,DC_HASDEFID); 
} 

BOOL CMenuEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

    m_Accelerators = ::LoadAccelerators(AfxGetResourceHandle(), 
        MAKEINTRESOURCE(IDR_ACCELERATOR2)); 

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_Editor.m_item.inserttext = false;


	if(m_pMenu)
	{
		
		m_Editor.LoadFromResource(m_pMenu);


	}
	else
	{
		MenuEditorItem* last = new MenuEditorItem() ;
		m_Editor.m_item.m_items.push_back(last);
		last->init(&m_Editor);
		last->m_type = 0;
		last->openned = 0;

	}


	m_Editor.m_items.push_back(   &m_Editor.m_item.m_items );	// show first row


	m_Editor.m_offsets.push_back(CPoint(2,2));
	m_Editor.m_edit.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), this, 10);
	m_Editor.m_edit.SetFont(this->GetFont());

	m_Editor.selected = 0;

	CRect Window;
	GetWindowRect(&Window);
	m_Editor.Create(NULL, "Editor", WS_CHILD | WS_VISIBLE | WS_BORDER|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(5, 5, Window.Width() - 11,198), this, 100);

	CString Title;
	Title.Format("Construct : %s", ME_TITLE);
	SetWindowText(Title);

	m_Checked.SetWindowText(ME_CHECKED);
	m_Separator.SetWindowText(ME_SEPARATOR);
	m_Disabled.SetWindowText(ME_DISABLED);
	m_Cancel.SetWindowText(CANCEL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMenuEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMenuEditor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	dc.SelectObject((GetParent())->GetFont());
	dc.SetBkMode(TRANSPARENT);
	CRect fill;
	GetClientRect(fill);

	dc.FillSolidRect(fill,0xffffff);

	int x = 0;

	for( vector< vector<MenuEditorItem*>* >::iterator popup = m_items.begin(); popup != m_items.end(); popup ++,x++)
	{
		// loop each item in the popup
		SetOffsetForNextPopup(m_offsets[x].x, m_offsets[x].y);

		CRect rect(nextoffsetx,nextoffsety,ITEMWIDTH + nextoffsetx,ITEMHEIGHT+nextoffsety);
		CRect rect2 = rect;
		CRect store;

		for( vector<MenuEditorItem*>::iterator item = (*popup)->begin(); item != (*popup)->end(); item ++)
		{
			store = rect2;	
			(*item)->Paint(dc, rect2);
		}

			CRect bounding;
			bounding.left = min(rect.left, store.left);
			bounding.right = max(rect.right, store.right);
			bounding.top = min(rect.top, store.top);
			bounding.bottom = max(rect.bottom, store.bottom);

			bounding.InflateRect(1,1);

			dc.Draw3dRect(bounding,0xffffff,0xffffff);
		
			bounding.InflateRect(1,1);
			dc.Draw3dRect(bounding, GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNSHADOW));


			
			// draw rectangle around everything

		}
	
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMenuEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMenuEditor::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	// set our item text to the text of the editbox

	MenuEditorItem* oldselected = selected;


	if(selected)
	{
		CString content;
		m_edit.GetWindowText(content);
		if(content != "")
		{
			selected->m_Text = content;
			selected->inserttext = false;
		}
		else
		{
			if(selected->inserttext == true)
			{
				// we need to remove the child item and the next
				// work out what its a sub of
				for( vector< vector<MenuEditorItem*>* >::iterator popup = m_items.begin(); popup != m_items.end(); popup ++)
				{
					for( vector<MenuEditorItem*>::iterator item = (*popup)->begin(); item != (*popup)->end(); item ++)
					{
						if((*item)->GetPtr() == selected)
						{
							// we need to add a new item to the parent


							MenuEditorItem* myitem = (*popup)->back();
							delete myitem;

							(*popup)->pop_back();
							break;


						}

					}
				}
				// and now add a new child item
				MenuEditorItem* myitem = selected->m_items.back();
				delete myitem;

				selected->m_items.pop_back();




			}



		}

		// Save properties to this
		// Okay we have selected the item - update the properties in the frame to reflect this.
		selected->m_Checked = ((CButton*)GetParent()->GetDlgItem(IDC_CHECKED))->GetCheck( );
		if(selected->m_type == 2 || selected->m_type == 1)
		{
			selected->m_type = 1 + ((CButton*)GetParent()->GetDlgItem(IDC_SEPARATOR))->GetCheck() == 1? true: false;
		}
		CString edit;
		GetParent()->GetDlgItem(IDC_EDIT1)->SetWindowText(edit);
		selected->m_ID = atoi(edit);

		selected->m_Disabled = ((CButton*)GetParent()->GetDlgItem(IDC_DISABLED))->GetCheck() == 1? true: false;

	//	SetCheck( !selected->m_Disabled);

	}
	// loop every visible item and see if we clicked it...rememmber e will have to loop backward

	selected = 0;
	for( vector< vector<MenuEditorItem*>* >::iterator popup = m_items.begin(); popup != m_items.end(); popup ++)
	{
		for( vector<MenuEditorItem*>::iterator item = (*popup)->begin(); item != (*popup)->end(); item ++)
		{
			if((*item)->m_rect.PtInRect(point))
			{
				selected = (*item)->GetPtr();
				break;

			}
		}
	}

	if(selected)
	{
		// Okay we have selected the item - update the properties in the frame to reflect this.
		((CButton*)GetParent()->GetDlgItem(IDC_CHECKED))->SetCheck( selected->m_Checked? 1:0);
		((CButton*)GetParent()->GetDlgItem(IDC_SEPARATOR))->SetCheck( selected->m_type == 2? 1: 0);
		CString edit;
		edit.Format("%d", selected->m_ID);
		GetParent()->GetDlgItem(IDC_EDIT1)->SetWindowText(edit);
		((CButton*)GetParent()->GetDlgItem(IDC_DISABLED))->SetCheck( selected->m_Disabled? 1: 0);



		bool escape = false;
		// work out what its a sub of
		int x = 0;
		for( vector< vector<MenuEditorItem*>* >::iterator popup = m_items.begin(); popup != m_items.end(); popup ++, x++)
		{
			for( vector<MenuEditorItem*>::iterator item = (*popup)->begin(); item != (*popup)->end(); item ++)
			{
				if((*item)->GetPtr() == selected)
				{
					// work out if its a 'Add Text' item
					if(selected->inserttext)
					{
						// we need to add a new item to the parent
						MenuEditorItem* item = new MenuEditorItem();
						item->init(this);
						item->m_type = selected->m_type;
						(*popup)->push_back(item);
					
						// and now add a new child item
						MenuEditorItem* myitem = new MenuEditorItem;
						myitem->init(this);
						myitem->m_type = 1;
						selected->m_items.push_back(myitem);					
					}

					//	now we need to remove all the items above this
					popup++;
					x++;
					//while(popup != m_items.end())
					///{
					//	m_items.pop_back();
					//}
					m_items.erase(popup, m_items.end() );
					m_offsets.erase(m_offsets.begin() + x, m_offsets.end());
					

					// and last we add the child of the selected
					m_items.push_back(&selected->m_items);
					if(selected->m_type == 0)
						m_offsets.push_back(CPoint(selected->m_rect.left+2, selected->m_rect.bottom+3));
					else
						m_offsets.push_back(CPoint(selected->m_rect.right+3, selected->m_rect.top+2));

					escape = true;
					break;

				}
			}
			if(escape) break;

		}





	
	// reposition our editbox

		CRect pos = selected->m_rect;
		pos.DeflateRect(2,2);
		pos.left += 17+4; // icon offset
		pos.top += 4;
		pos.right += 4;
		pos.bottom += 4;



		m_edit.MoveWindow(pos);

		if(selected->inserttext)
			m_edit.SetWindowText(_T(""));
		else
			m_edit.SetWindowText(selected->m_Text);
		if(selected == oldselected)
		{
			m_edit.ShowWindow(true);
			m_edit.SetFocus();
		}
		else
			m_edit.ShowWindow(false);

	}
	else
	{
		// hide editbox
		m_edit.MoveWindow(CRect(0,-32,0,0));

		m_items.erase(m_items.begin()+1, m_items.end() );
		m_offsets.erase(m_offsets.begin() + 1, m_offsets.end());
		m_edit.ShowWindow(false);


	}

	Invalidate();

	CWnd::OnLButtonDown(nFlags, point);
}

void CMenuEditor::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_edit.ShowWindow(true);
	m_edit.SetFocus();
	Invalidate();
	CWnd::OnLButtonDblClk(nFlags, point);

}

void  CMenuEditor::LoadFromResource(MenuResource* res)
{
	MenuResourceItem item;
	item.items = res->items;
	


	m_item.CreateFromMenuResource(item, this);
	for(int i = 0; i < m_item.m_items.size(); i++)
	{
		m_item.m_items[i]->m_type = 0;
	}


}
void  CMenuEditor::SaveToResource(MenuResource* res)
{
	MenuResourceItem item;
	m_item.SaveToMenuResource(item);

	res->items.clear();

	for(int a = 0; a < item.items.size(); a++)
		res->items.push_back(item.items[a]);



}
void CMenuEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	m_edit.ShowWindow(true);
	m_edit.SetFocus();
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}



void CMenuEditorDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_Editor.m_edit.ShowWindow(true);
	m_Editor.m_edit.SetFocus();
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
