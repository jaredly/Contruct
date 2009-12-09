/////////////////////////////////////////////////////////////////////////////
// Event sheet editor - drawing
/////////////////////////////////////////////////////////////////////////////
 
// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "..\..\Construct Doc.h"
#include "Event Sheet Editor.h"

void EventSheetEditor::OnDraw(CDC* pDC)
{
	CPoint scroll = GetScrollPosition();
	scroll.x;
	scroll.y;

	CRect Window;
	m_tmpScrollCount=0;

	pDC->SetViewportOrg(0,0);
	GetClientRect(&Window);

	CMemDC dc(pDC);
	dc.FillSolidRect(0,0,Window.Width(),Window.Height(),RGB(245,245,245));

	if(!m_pHeader)
		return;

	// Lines to make indentions easy to see
	for(int x = 28; x < m_pHeader->m_Split ; x += 20)
	{
		dc.FillSolidRect(x,0,1, Window.Height(), RGB(235,235,235));
	}

	dc.SetBkMode(TRANSPARENT);

	CRect Header(-ScrollPos.x,-30,Window.Width(),0);//30);
	if(!m_pHeader)
		return;
	m_pHeader->m_rect = Header;

	if(m_Drag.IsDragging)
	{
		switch (m_Drag.dragtype)
		{
		case OBJECT:
			{
				for(int a = 0; a < this->m_pHeader->m_Objects.size(); a++)
				{
					CRect bound = m_pHeader->m_Objects.at(a).m_rect;
					if(bound.PtInRect(m_Mouse))
					{
						dc.Draw3dRect(bound, 0, 0);
						bound.InflateRect(1,1);
						dc.Draw3dRect(bound, 0, 0);	
					}
				}
			}
			break;
		}
	}

	CRgn Rgn;
	Rgn.CreateRectRgn(0, Header.bottom-dc.m_rect.top, Window.right, Window.bottom);
	dc.SelectClipRgn(&Rgn);
	CRect Pos(2+24,Header.bottom+2,m_pHeader->m_Split,Header.bottom+2);
	Pos.OffsetRect(0, (int)-ScrollPos.y);
	dc.SelectObject(MyFont);

	int num = 1;
	NumerateEventList(m_pEventList,num);

	int prevy = Pos.top+9999;

	if(m_pEventList->size()) {
		for(int x = 0; x < (*m_pEventList).size(); x++)
		{
			(*m_pEventList)[x]->Render(Pos,dc,false,*this, prevy, m_pEventList, x);
		}
	}
	else	// no events
	{
		Window.top += Header.bottom + 5;
		dc.DrawText("Right click to begin",Window, DT_CENTER | DT_SINGLELINE);
	}

	// Remove the indention lines that go past the events because Rich and Ashley tell me they look ugly and its 2 against 1 :(
	for(int x = 28; x < m_pHeader->m_Split ; x += 20)
	{
		dc.FillSolidRect(x,Pos.top,1, Window.Height(), RGB(245,245,245));
	}


	UpdateScrollbars();

	// Dragging
	if(m_Drag.IsDragging)
	{
		switch (m_Drag.dragtype)
		{
			case EVENT:
			{
				CEditorEvent* tmpEvent=EventAt(m_Mouse);
				if(tmpEvent)
				{
					CRect EventRect=tmpEvent->m_rect(this);
					EventRect.left=tmpEvent->m_handle(this).left;
					EventRect.right=tmpEvent->m_handle(this).right;
					if(tmpEvent->m_select(this) && !m_Drag.CtrlDown)
						return;// Our handle depends on if we are going up or down with the mouse.

					if(EventRect.left + 32 < m_Mouse.x && tmpEvent->canAddEvents())
						dc.FillSolidRect(EventRect.left+10,EventRect.bottom-1,EventRect.Width()-10,3,0);

					else if(m_Drag.GoingUp)
						dc.FillSolidRect(EventRect.left,EventRect.top-2,EventRect.Width(),3,0);

					else
						dc.FillSolidRect(EventRect.left,EventRect.bottom-1,EventRect.Width(),3,0);
				}
			}
			break;

			case CONDITION:
			{
				CEditorEvent* tmpEvent=EventAt(m_Mouse);
				
				if(tmpEvent == 0 ) 
					return; // eep we lost our event that held the condition our mouse is over!!!

				CEditorCondition* tmpCondition = tmpEvent->conditionAt(m_Mouse, this);
				
				if(tmpCondition == 0)
					return; // eep we lost our selected events! Might want to do something here!

				CRect ConditionRect = tmpCondition->m_rect(this);
				if(tmpCondition->m_select(this) && !m_Drag.CtrlDown) return;
				if(m_Drag.GoingUp)
					dc.FillSolidRect(ConditionRect.left,ConditionRect.top-1,ConditionRect.Width(),3,0);
				else
					dc.FillSolidRect(ConditionRect.left,ConditionRect.bottom-1,ConditionRect.Width(),3,0);
			}
			break;	

		case ACTION:
			{
				CEditorEvent* tmpEvent = EventAt(m_Mouse);
				
				if(tmpEvent == 0 ) 
					return; // eep we lost our event that held the condition our mouse is over!!!

				CEditorAction* tmpAction = tmpEvent->actionAt(m_Mouse, this);
				
				if(tmpAction == 0)
					return; // eep we lost our selected events! Might want to do something here!

				CRect ActionRect = tmpAction->m_rect(this);
				if(tmpAction->m_select(this) && !m_Drag.CtrlDown) return;

				if(tmpEvent->m_displayActionType(this) == 1) // chrono
				{
					if(m_Drag.GoingUp)
						dc.FillSolidRect(ActionRect.left-1,ActionRect.top,3,ActionRect.Height(),0);
					else
						dc.FillSolidRect(ActionRect.right-1,ActionRect.top,3,ActionRect.Height(),0);
				}
				else // list
				{
					if(m_Drag.GoingUp)
						dc.FillSolidRect(ActionRect.left,ActionRect.top-1,ActionRect.Width(),3,0);
					else
						dc.FillSolidRect(ActionRect.left,ActionRect.bottom-1,ActionRect.Width(),3,0);
				}
			}
			break;
		}
	}
/*
	if(m_FocusEvent)
		dc.Draw3dRect(m_FocusEvent->m_handle(this), 0, 0);
	if(m_FocusAction)
		dc.Draw3dRect(m_FocusAction->m_rect(this), 0, 0);
	if(m_FocusCondition)
		dc.Draw3dRect(m_FocusCondition->m_rect(this), 0, 0);
	*/
}

int CEditorEvent::Render(CRect &Pos, CDC &dc,bool ParentSelected,EventSheetEditor& Ed, int& previousy, EventVector* pEventList, int index)
{
	//#define OUTLINE_EVENTS
	static HBITMAP addIcon = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_ICONADD));

	CRect Window;
	Ed.GetClientRect(&Window);

	#define IS_ABOVE_VISIBLE			(Ed.ScrollPos.y > Ed.m_tmpScrollCount+abs(m_handle(&Ed).Height()) )
	#define IS_BELOW_VISIBLE			(Pos.bottom > Window.bottom)
	#define IS_VISIBLE_FUNCTION			(!(IS_ABOVE_VISIBLE || IS_BELOW_VISIBLE))

	bool Selected = ParentSelected? true : m_select(&Ed);
	
	COLORREF TabOutline =	Selected? EVENT_TAB_OUTLINE_SEL:EVENT_TAB_OUTLINE;
	COLORREF TabBG =		Selected? EVENT_TAB_BG_SEL:EVENT_TAB_BG;
	COLORREF Outline =		Selected? EVENT_OUTLINE_SEL:EVENT_OUTLINE;

	COLORREF BG;

	BG = Selected? EVENT_BG_SEL:EVENT_BG;
	COLORREF ActionBG = Selected? ACTION_BG_SEL:ACTION_BG;

	int importantyoffset = 0;
	
	m_visible(&Ed) = false; // we dont know if the event is visible till later
	m_rect(&Ed) = Pos;

	// Maybe its being deleted and in an animation?
	if(m_Anim.m_bDestroyed)
	{
		Ed.m_tmpScrollCount+= m_Anim.Space;
		Pos.OffsetRect(0,m_Anim.Space);
		return 0;
	}

	CRect ConditionRect = Pos;
	int OffsetY = 0;	// used when actions have a grid under them

	if(IS_VISIBLE_FUNCTION) // This is really poor coding!
	{
		CRect Box = Pos; // Box is used for rendering
		if(m_type!= 0)
			Box.left -= 2;

		//The first thing we need to do is draw the conditions and actions;
		ConditionRect.left += EVENT_TAB_SIZE;
		ConditionRect.top += 2;
		ConditionRect.right -= 2;
		ConditionRect.bottom = ConditionRect.top;

		ForceTriggersToTop();

		switch (m_type)
		{
			case COMMENT_TYPE: // Comment	
				if( IS_VISIBLE_FUNCTION)
				{
					Box.right = Window.right;
					m_visible(&Ed) = true;

					CFont* NewFont = gMyFontManager.RequestFont(m_cFontSize,0,0,0,m_cbFontBold? FW_BOLD : FW_NORMAL
													,m_cbFontItalic,
								   this->m_cbFontUnderline,this->m_cbFontStrike,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
								   FF_DONTCARE,this->m_cFontName);

					CFont* FontOld = dc.SelectObject(NewFont);

					CRect Box2 = Box;
					Box2.DeflateRect(2,2,2,2);

					// Replace & with && to prevent underscoring
					CString drawText = m_cText;
					drawText.Replace("&", "&&");
					dc.DrawText(drawText, Box2, DT_WORDBREAK | DT_CALCRECT);

					Box2.right = Box.right;
					Box2.left = Box.left;
					Box2.top -= 2;
					Box2.bottom += 2;
					dc.FillSolidRect(Box2,m_cTextBG);		
					COLORREF old = dc.SetTextColor(m_cTextColour);
					Box2.DeflateRect(2,2,2,2);
					dc.DrawText(drawText, Box2, DT_WORDBREAK);
					dc.SetTextColor(old);
					
					Box.bottom = Box2.bottom+2;
					dc.SelectObject(FontOld);
				}
			break;
			case SCRIPT_TYPE:
				if( IS_VISIBLE_FUNCTION)
				{
					Box.right = Window.right;
					m_visible(&Ed) = true;


					CFont* NewFont = gMyFontManager.RequestFont(m_cFontSize,0,0,0,m_cbFontBold? FW_BOLD : FW_NORMAL
													,m_cbFontItalic,
								   this->m_cbFontUnderline,this->m_cbFontStrike,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
								   FF_DONTCARE,this->m_cFontName);


					CFont* FontOld = dc.SelectObject(NewFont);

					CRect Box2 = Box;
					Box2.DeflateRect(2+PYTHON_MARGIN,2,2,2);
					dc.DrawText(m_cText,Box2,DT_WORDBREAK|DT_EXPANDTABS|DT_CALCRECT);
					Box2.right = Box.right;
					Box2.left = Box.left;
					Box2.top -= 2;
					Box2.bottom += 2;
					
					dc.FillSolidRect(Box2,m_cTextBG);	
						

					COLORREF old = dc.SetTextColor(m_cTextColour);
					Box2.DeflateRect(2,2,2,2);
					Box2.left += PYTHON_MARGIN;
					dc.DrawText(m_cText, Box2, DT_WORDBREAK | DT_EXPANDTABS);
					dc.SetTextColor(old);
					
					Box.bottom = Box2.bottom+2;
					dc.SelectObject(FontOld);
				}
			break;
			case GROUP_TYPE:	//event group
				if( IS_VISIBLE_FUNCTION)
				{

					CFont* TitleFont = gMyFontManager.RequestFont(20,0,0,0,FW_BOLD,0,
									   0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
									   FF_DONTCARE,"Arial");
						
					CFont* DescripionFont = gMyFontManager.RequestFont(12,0,0,0,FW_NORMAL,0,
									   0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
									   FF_DONTCARE,"Arial");
								  

					m_visible(&Ed) = true;


					Box.right = Window.right;
					int title_height;
					int desc_height;
					;
					for(int a = 0; a < 2; a++)
					{
						int drawstyle = DT_WORDBREAK|DT_CALCRECT;
						if(a == 1)
							drawstyle = DT_WORDBREAK;
					

						CRect TextBox = Box;

						//Title
						{
							CString drawText = m_sTitle;
							drawText.Replace("&", "&&");

							TextBox.top  += 4;//note the 4
							TextBox.left += 20;
							CRect Box2 = TextBox;
							COLORREF old = dc.SetTextColor(m_sActive ? RGB(0,0,0) : RGB(192,192,192));
							CFont* pFontOld = dc.SelectObject(TitleFont);
							dc.DrawText(drawText, Box2, drawstyle);
							dc.SelectObject(pFontOld);
							dc.SetTextColor(old);
							
							if(a == 0)
								title_height = max(19,Box2.Height());
			
							TextBox.OffsetRect(3,title_height);
						}
						

						//Description
						{
							CString drawText = m_sDescription;
							drawText.Replace("&", "&&");

							TextBox.left  += 4;
							CRect Box2 = TextBox;
							CFont* pFontOld = dc.SelectObject(DescripionFont);
							COLORREF old = dc.SetTextColor(m_sActive ? RGB(128,128,128) : RGB(224,224,224));
							dc.DrawText(drawText, Box2, drawstyle);
							dc.SelectObject(pFontOld);
							dc.SetTextColor(old);
							
							if(a == 0)
								desc_height = max(2,Box2.Height());
						}
		

						if(a == 0)
						{
							Box.bottom = Box.top + title_height + desc_height + 4 + 2;
							dc.FillSolidRect(Box,RGB(255,255,255));	

						}
				
						
					}
				}
			break;

			case INCLUDE_TYPE:
				if( IS_VISIBLE_FUNCTION)
				{
					Box.right = Window.right;
					m_visible(&Ed) = true;

					CFont* NewFont = gMyFontManager.RequestFont(14,0,0,0,FW_BOLD 
													,0,
								   0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
								   FF_DONTCARE,"Arial");

					CFont* NewFont2 = gMyFontManager.RequestFont(14,0,0,0,0 
													,0,
								   0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
								   FF_DONTCARE,"Arial");


					

					CString EventSheetName = "Right click here";
					EventSheet* sheet = ((EventSheetEditor*)(&Ed))->application->FindEventSheetFromNumber(event_sheet_include);
					if(sheet)
						EventSheetName = sheet->GetName();

					CRect Box2 = Box;
					Box2.DeflateRect(4,4,2,2);
					
					CFont* FontOld = dc.SelectObject(NewFont);
					dc.DrawText("Include Event Sheet : ",Box2,DT_CALCRECT);

					Box2.right = Box.right;
					Box2.left = Box.left;
					Box2.top -= 2;
					Box2.bottom += 4;

					dc.FillSolidRect(Box2,0xffffff);		

					Box2.left = Box.left;
					Box2.right = Box.right;
					Box2.DeflateRect(4,4,2,2);

					COLORREF old = dc.SetTextColor(0xaa0000);


					dc.SelectObject(NewFont);
					dc.DrawText("Include Event Sheet : ", Box2, DT_LEFT);



					Box2.left = Box.left;
					Box2.right = Box.right;
					Box2.DeflateRect(125,0,6,0);
					dc.SelectObject(NewFont2);
					dc.DrawText(EventSheetName, Box2, DT_LEFT);

					Box2.InflateRect(2,2);
					dc.Draw3dRect(Box2, 0xddaaaa, 0xddaaaa);
									
		
					dc.SetTextColor(old);
					dc.SelectObject(FontOld);

					Box.bottom = Box2.bottom+4;

				}
			break;
			case EVENT_TYPE: // Types which have conditions
			{
				// Render all conditions
				int OffsetY=0;
				if(m_displayEventType(&Ed) == 0)
				{
					ConditionRect.left = Pos.right;
					ConditionRect.right = Window.right;
					ConditionRect.right = max(ConditionRect.left + 100, ConditionRect.right);

					Box.right = Window.right;
				}
				int ConditionRectLeftOriginal = ConditionRect.left;
				if(IS_VISIBLE_FUNCTION)
					m_visible(&Ed) = true;

				for(int a = 0; a < m_Conditions.size() ; a ++ )
				{
					m_Conditions[a]->m_visible = false;
					if(m_Conditions[a]->ValidToDisplay(((EventSheetEditor*)&Ed)->application))
					{
						m_Conditions[a]->m_visible = true;
						int oldtop = ConditionRect.top;
						int oldleft = ConditionRect.left;
						
						m_Conditions[a]->Render(ConditionRect,dc,Selected,Ed, m_bBookmarked, m_bEnabled && m_Conditions[a]->m_bEnabled);

						dc.FillSolidRect(ConditionRect.left,ConditionRect.top,ConditionRect.Width(),1,BG);
						if(ConditionRectLeftOriginal!= ConditionRect.left) // We need to fill in the space..
						{
							dc.FillSolidRect(CRect(ConditionRectLeftOriginal, oldtop, oldleft,ConditionRect.top),BG);
						}
						ConditionRect.OffsetRect(CPoint(0,1));
						m_visible(&Ed) = true;
					}
				}

				ConditionRect.left = ConditionRectLeftOriginal;
				// We have a min height of 16...we cant go below this when deleting conditions
				if(m_visible(&Ed) && (ConditionRect.top < Box.top+16))
				{
					dc.FillSolidRect(ConditionRect.left,ConditionRect.top,ConditionRect.Width(),16-ConditionRect.top+Box.top,BG);
					ConditionRect.top = Box.top +16;
					ConditionRect.bottom=Box.top+16;
				}
				if(m_visible(&Ed)) 
					Box.bottom = ConditionRect.top+1;
			}
			break;
		} //END CASE

		if(m_visible(&Ed))
		{
			if(m_type == 0) // Types which have conditions
			{
				// Tab bit	
#ifdef OUTLINE_EVENTS
				dc.SetPixel(Box.left + 2,Box.top,TabOutline);
				dc.SetPixel(Box.left,Box.top+2,TabOutline);
					
				dc.SetPixel(Box.left + 2,Box.bottom - 1,TabOutline);
				dc.SetPixel(Box.left    ,Box.bottom - 3,TabOutline);

				dc.FillSolidRect(Box.left,Box.top + 3,1,Box.Height() - 6,TabOutline);
#else
				//COLORREF tabbything = (ParentSelected? true : m_select(&Ed))? EVENT_TAB_BG_SEL : EVENT_TAB_BG;
				dc.Draw3dRect(Box.left,Box.top,Box.Width(),Box.Height(),TabBG, TabBG);
#endif
				dc.FillSolidRect(Box.left + 1, Box.top+1,2,Box.Height() -2,TabBG);
#ifdef OUTLINE_EVENTS
				dc.SetPixel(Box.left + 1,Box.top+1,TabOutline);
				dc.SetPixel(Box.left + 1,Box.bottom-2,TabOutline);

				// Outlines
				
				dc.FillSolidRect(Box.left + 3, Box.top,Box.Width()-3,1,Outline);
				dc.FillSolidRect(Box.left + 3, Box.bottom -1,Box.Width()-3,1,Outline);
				dc.FillSolidRect(Box.right - 1, Box.top,1,Box.Height(),Outline);
#endif
				int eventtabsize = ConditionRect.left -Box.left;

				dc.FillSolidRect(Box.left + 3, Box.top+1,eventtabsize-3,Box.Height() -2,BG);
				dc.FillSolidRect(Box.left + eventtabsize, Box.top+1,Box.Width()-eventtabsize-1,1,BG);
				dc.FillSolidRect(Box.left + eventtabsize, Box.bottom-2,Box.Width()-eventtabsize-1,1,BG);
				dc.FillSolidRect(Box.right-2, Box.top+1,1,Box.Height() -2,BG);

				// draw the number
				CFont* NewFont = gMyFontManager.RequestFont(14,0,0,0,FW_NORMAL
															,0,
										   0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
										   FF_DONTCARE,"Arial");
				CFont* old = dc.SelectObject(NewFont);
				CString num;
				num.Format("%d", this->m_number);
				CRect NumberRect(Box.left - 32, (Box.top+Box.bottom)/2 - 10, Box.left-3, Box.bottom - 2);
				if (m_bBookmarked)
					dc.FillSolidRect(NumberRect, RGB(255, 167, 167));
				dc.DrawText(num, NumberRect, DT_RIGHT | DT_SINGLELINE);
				dc.SelectObject(old);
			}

			else if(m_type == SCRIPT_TYPE)
			{
				dc.FillSolidRect(Box.left, Box.top, PYTHON_MARGIN, Box.Height(),BG);
				dc.Draw3dRect(Box.left, Box.top,Box.Width(),Box.Height(),TabOutline,TabOutline);

				// draw the number
				CFont* NewFont = gMyFontManager.RequestFont(14,0,0,0,FW_NORMAL
															,0,
										   0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
										   FF_DONTCARE,"Arial");
				CFont* old = dc.SelectObject(NewFont);
				CString num;
				num.Format("%d", this->m_number);
				CRect NumberRect(Box.left - 32, (Box.top+Box.bottom)/2 - 10, Box.left-3, Box.bottom - 2);
				if (m_bBookmarked)
					dc.FillSolidRect(NumberRect, RGB(255, 167, 167));
				dc.DrawText(num, NumberRect, DT_RIGHT | DT_SINGLELINE);
				dc.SelectObject(old);
				
			}

			else{
				if(m_type != COMMENT_TYPE || Selected) // we dont want an outline around comments that arn't selected
					dc.Draw3dRect(Box.left, Box.top,Box.Width(),Box.Height(),TabOutline,TabOutline);
			}

			if(m_EventList.size() > 0 || m_type == GROUP_TYPE)
			{
				if(m_visible(&Ed))
				{
					dc.Draw3dRect(Box.left + EVENT_TAB_BUTTONX,Box.bottom - EVENT_TAB_BUTTONY,9,9,TabBG,TabBG);
					dc.FillSolidRect(Box.left + EVENT_TAB_BUTTONX+2,Box.bottom - EVENT_TAB_BUTTONY+4,5,1,TabOutline); 
				
					if(!m_open(&Ed))
						dc.FillSolidRect(Box.left + EVENT_TAB_BUTTONX+4,Box.bottom - EVENT_TAB_BUTTONY+2,1,5,TabOutline); 
				}
			}

			if(m_type == 0) // Types which have actions
			{
				if(m_displayEventType(&Ed) == 0)
				{
					importantyoffset = ConditionRect.bottom - Pos.bottom+2;
					Pos.OffsetRect(0,importantyoffset);
					Ed.m_tmpScrollCount += importantyoffset;
					importantyoffset += 4;
				}

				////////////////////////////////
				//  Render Actions
				////////////////////////////////

				CRgn Rgn;
				CMemDC* dc2=(CMemDC*)&dc;
				Rgn.CreateRectRgn(Ed.m_pHeader->m_Split - dc2->m_rect.left,Ed.m_pHeader->m_rect.bottom - dc2->m_rect.top,dc2->m_rect.right,dc2->m_rect.bottom);
				dc.SelectClipRgn(&Rgn);

				switch (m_displayActionType(&Ed)) 
				{
					//=====================
					// Chrono Mode
					case 1:
					{
						// Work out how many lines we need
						int linecount = 1;
						{
							CRect ActionPos(ConditionRect.right+2+4,
							Pos.top+2,ConditionRect.right+2+4+16,Pos.top+2+16);
					
							for(int b = 0; b < m_Actions.size(); b ++ )
							{
								if(m_Actions[b]->ValidToDisplay(((EventSheetEditor*)&Ed)->application))
								{
									ActionPos.OffsetRect(30,0);
									if(ActionPos.right+14 > Window.Width()&& b != m_Actions.size()-1)
									{
										ActionPos.OffsetRect(0-ActionPos.left+ConditionRect.right+6,21);
										linecount ++;
									}
								}
							}	
						}
						
						int width = Window.Width() - ConditionRect.right-5;
						for(int line = 0; line < linecount; line ++)
							dc.FillSolidRect(CRect(ConditionRect.right+4,Pos.top+line*21,Window.Width(),Pos.top+20 + line*21),ActionBG);

						// Draw grid
						OffsetY = 20 + (linecount-1)*21;
		
						CRect ActionPos(ConditionRect.right+2+4,
							Pos.top+2,ConditionRect.right+2+4+16,Pos.top+2+16);
					
						for(int b = 0; b < m_Actions.size(); b ++ )
						{
							m_Actions[b]->m_visible = false;

							if(m_Actions[b]->ValidToDisplay(((EventSheetEditor*)&Ed)->application))
							{
								m_Actions[b]->m_visible = true;
								m_Actions[b]->Render(ActionPos,dc,Selected,Ed,1);
							
								ActionPos.OffsetRect(30,0);
								if(ActionPos.right+14 > Window.Width() && b != m_Actions.size()-1)
									ActionPos.OffsetRect(0-ActionPos.left+ConditionRect.right+6,21);
							}
						}
						// last square
						ActionPos.right = ActionPos.left + 16;
						ActionPos.bottom = ActionPos.top + 16;

						HDC dcMem = CreateCompatibleDC(dc.m_hDC);
						HBITMAP hdcAdd = (HBITMAP)SelectObject(dcMem, addIcon);
						BitBlt(dc.m_hDC, ActionPos.left, ActionPos.top, 16, 16, dcMem, 0, 0, SRCCOPY);
						SelectObject(dcMem, hdcAdd);
						DeleteDC(dcMem);

						m_gridRec(&Ed) = ActionPos;
					}
					break;

					// =======
					// Flash list iconic
					case 3:
					{
						// Render the actions
						
						for(int b = 0; b < m_Actions.size(); b ++ )
						{
							m_Actions[b]->m_visible = false;
							if(m_Actions[b]->ValidToDisplay(((EventSheetEditor*)&Ed)->application))
							{
								m_Actions[b]->m_visible = true;
								CRect rect;
								rect.left = Pos.right+1;
								rect.right = Window.Width();
								rect.right = max(rect.left + 200, rect.right);
								rect.bottom = Pos.top;
								rect.top = rect.bottom;
								rect.OffsetRect(0,OffsetY);

								m_Actions[b]->Render(rect,dc,Selected, Ed, 3, m_bEnabled && m_Actions[b]->m_bEnabled);
								m_Actions[b]->m_rect(&Ed).OffsetRect(-Ed.ScrollPos.x,0);
								OffsetY = rect.bottom - Pos.top;
								OffsetY += 1;
							}
						}

						CRect rect(Ed.m_pHeader->m_Split,0,dc2->m_rect.right, 17);
						rect.OffsetRect(0, Pos.top+OffsetY);

						dc.FillSolidRect(rect, RGB(255,255,255));
						CFont* NewFont;

						if (mouseOverNewAction) {
							// Underlined
							NewFont = gMyFontManager.RequestFont(14,0,0,0,0 
								,0,1,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
							   FF_DONTCARE,"Arial");
						}
						else {
							NewFont = gMyFontManager.RequestFont(14,0,0,0,0 
								,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
							   FF_DONTCARE,"Arial");
						}
						dc.SelectObject(NewFont);

						m_gridRec(&Ed) = rect;
						rect.left += 4;
						rect.top += 2;
						dc.SetTextColor(255);
						dc.DrawText( " + New action", rect, DT_LEFT);
						dc.SetTextColor(0);
						OffsetY += 17;						
					}
					break;
				}

				Rgn.DeleteObject();
				Rgn.CreateRectRgn(0,Ed.m_pHeader->m_rect.bottom - dc2->m_rect.top,dc2->m_rect.Width(),dc2->m_rect.Height());
				dc.SelectClipRgn(&Rgn);

				int ybottom = Pos.bottom-Box.bottom+OffsetY-2;

				bool extraline = false;
#ifndef OUTLINE_EVENT
				if(ybottom <= 0)
				{
					ybottom = 3;
					extraline = true;
				}
#endif

				if( ybottom > 0)
					dc.FillSolidRect(Box.left+12, Box.bottom, Ed.m_pHeader->m_Split-Box.left-12, ybottom+2,(ParentSelected? true : m_select(&Ed))? EVENT_TAB_BG_SEL : EVENT_TAB_BG);
			
				if(extraline)
				{
					Box.bottom += 5;
				}
			}
		}
		
		m_CondArea(&Ed) = Box;
		Box.bottom=max(Box.bottom,Pos.top+ OffsetY);
		m_handle(&Ed) = Box;

		// Restore region
		CMemDC* dc2 = (CMemDC*)&dc;
		CRgn Rgn;
		Rgn.CreateRectRgn(0,Ed.m_pHeader->m_rect.bottom - dc2->m_rect.top,dc2->m_rect.Width(),dc2->m_rect.Height());
		dc.SelectClipRgn(&Rgn);
	}

	Pos.OffsetRect(0,1);
	Ed.m_tmpScrollCount ++;
	CRect EventRect = Pos;
	EventRect.OffsetRect(CPoint(0,max(0,m_handle(&Ed).Height())-importantyoffset));
	EventRect.left += EVENT_INDENT;		

	int previousy2 = Pos.top+999;
	long tmpScrollCount = Ed.m_tmpScrollCount;
	Ed.m_tmpScrollCount += EventRect.top-Pos.top; 

	if(m_EventList.size() > 0)
	{
		if(m_open(&Ed))
		{
			for(int a = 0; a < m_EventList.size() ; a ++ )
			{				
				m_EventList[a]->Render(EventRect,dc,/*Selected*/false,Ed, previousy2, &m_EventList, a);
			}
		}
	}
	//Footers
	if(m_open(&Ed))
	{
		if(m_type == GROUP_TYPE)
		{
			if( IS_VISIBLE_FUNCTION)
			{
				CRect footer = EventRect;
				footer.left -=  2;
				footer.right = Window.right;
				footer.bottom += 18;
				dc.FillSolidRect(footer,RGB(255,255,255));
				dc.Draw3dRect(footer,TabOutline,TabOutline);

				m_visible(&Ed) = true;
	
				CFont* Footer;
				if (mouseOverFooter) {
							// Underlined
						Footer = gMyFontManager.RequestFont(14,0,0,0,0 
							,0,1,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
						  FF_DONTCARE,"Arial");
				}
				else {
							Footer = gMyFontManager.RequestFont(14,0,0,0,0 
								,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
							   FF_DONTCARE,"Arial");
				}

/*

				CFont* Footer = gMyFontManager.RequestFont(14,0,0,0,FW_BOLD,0,
										  0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
										  FF_DONTCARE,"Arial");	*/

				CFont* pFontOld = dc.SelectObject(Footer);
				COLORREF old = dc.SetTextColor(255);


				CString drawText = m_sTitle;
				drawText.Format("+ New event to: %s", drawText);
				drawText.Replace("&", "&&");

				footer.DeflateRect(3,3);

				dc.DrawText(drawText, footer, DT_WORDBREAK);
				dc.SelectObject(pFontOld);
				dc.SetTextColor(old);

			}
			EventRect.OffsetRect(0,19);
		}
	}
	Ed.m_tmpScrollCount = tmpScrollCount;
	Ed.m_tmpScrollCount += EventRect.top-Pos.top;
	Pos.OffsetRect(CPoint(0,EventRect.top-Pos.top));

	m_rect(&Ed).bottom = Pos.top;

	m_rect(&Ed).right = Window.Width();
	m_rect(&Ed).left -= 24;

	m_Anim.Space = m_rect(&Ed).Height();

	if(previousy < m_handle(&Ed).top)
	{
		if(IsElseEvent())
		{
			if(m_visible(&Ed))
			{
			
				COLORREF TabOutline =	0? EVENT_TAB_OUTLINE_SEL:EVENT_TAB_OUTLINE;
				COLORREF TabBG =		0? EVENT_TAB_BG_SEL:EVENT_TAB_BG;
				COLORREF Outline =		0? EVENT_OUTLINE_SEL:EVENT_OUTLINE;
				COLORREF BG =			0? EVENT_BG_SEL:EVENT_BG;
				COLORREF ActionBG	=	0? ACTION_BG_SEL:ACTION_BG;

				int indx = index;
				int prevy = -10; // the padding of events and stuff makes this become 0

				CEditorEvent* event_above = pEventList->at(index - 1);
				if(event_above->m_visible(&Ed))
				{
					prevy = event_above->m_handle(&Ed).bottom-4;
					if(event_above->m_Conditions.size() > 0)
						prevy = event_above->m_Conditions.back()->m_rect(&Ed).bottom;
				}

				CRect Area = CRect(ConditionRect.left-EVENT_TAB_SIZE, prevy, ConditionRect.left+3-EVENT_TAB_SIZE, m_handle(&Ed).top+2);
				if(pEventList->at(index - 1)->m_type != 0)
					Area.top +=2;

				bool foundtop;
				bool foundbottom = true;

				while(true)
				{
					foundtop = (pEventList->at(indx-1)->m_type == 0);

					// Small adjustments
					if(!foundtop)
						Area.top ++;
					if(!foundbottom)
						Area.bottom --;

					// makes the left part the same size as the right part for bars going between comments
					if(!foundtop)
						Area.top ++;
					if(!foundbottom)
						Area.bottom --;
	#ifdef OUTLINE_EVENT
					dc.FillSolidRect(Area, TabOutline);
	#endif
					
					
					dc.FillSolidRect(Area, TabBG);
					if(!foundtop)
						Area.top --;
					if(!foundbottom)
						Area.bottom ++;

					Area.left += 3;
					Area.right += 2;
					Area.top ++;
					Area.bottom --;

					dc.FillSolidRect(Area, BG);
					Area.left += 2;
					Area.right += 2;
					
					dc.FillSolidRect(Area, TabBG);	
					Area.left += 2;
					Area.right ++;
	#ifdef OUTLINE_EVENT
					dc.FillSolidRect(Area, Outline);
	#endif
					indx --;
					if(pEventList->at(indx)->m_type == 0)
						indx = 0;

					if(indx == 0)
						break;

					Area = CRect(ConditionRect.left-EVENT_TAB_SIZE,
						pEventList->at(indx - 1)->m_handle(&Ed).bottom-2,
						ConditionRect.left+3-EVENT_TAB_SIZE, 
						pEventList->at(indx)->m_handle(&Ed).top+2);

					if(pEventList->at(indx-1)->m_type == 0)
					{
						Area = CRect(ConditionRect.left-EVENT_TAB_SIZE,
						previousy-1,
						ConditionRect.left+3-EVENT_TAB_SIZE, 
						pEventList->at(indx)->m_handle(&Ed).top+2);
					}

					foundbottom = false;
				}
			}
		}
	}

	if(m_type == EVENT_TYPE || m_type == INCLUDE_TYPE)
	previousy = ConditionRect.top;
	if(m_type == GROUP_TYPE)
		previousy = 9999999;
	
	return 0;
}
