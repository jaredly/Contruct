// Edittime.cpp - Edittime implementation of object
//

// Include StdAfx and Properties.h
#include "StdAfx.h"

#ifndef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// Property table
//////////////////////////////////////////////////////////////////////////////////

// The property table defines the list of properties that appear in the property
// grid when your object is selected in the layout editor.
//
// See the SDK docs for full documentation, but to get you started:-
//
// PROPERTY_EDIT  (myVariable, "Label", "Description");		// A text field property.
// PROPERTY_VALUE (myVariable, "Label 2", "Description");	// An integer property.
// PROPERTY_FLOAT (myVariable, "Label 3", "Description");	// A double/float property
//
// The labels must be unique!

BEGIN_PROPERTY_TABLE();
if(!editObject)
	return;
if(!editObject->objectPtr)
	return;
if(editObject->objectPtr->m_EditMode)
{
	PROPERTY_BUTTON ("Edit", "Edit Mode", "Click to switch edit mode", BtnEditMode());
}
else
{
	PROPERTY_BUTTON ("Close", "Edit Mode", "Click to switch edit mode", BtnEditMode());
}

END_PROPERTY_TABLE  ();


/////////////////////////////////////////////////////////////////////////////////
// Edittime functions
//////////////////////////////////////////////////////////////////////////////////


void EditExt::BtnEditMode()
{
	if(m_EditMode)
	{
		pEditTime->ReleaseFrameEditor(pInfo->instanceID);
		m_EditMode = false;
	}
	else
	{
		ResizeBoardToObjectSize();
		pEditTime->CaptureFrameEditor(pInfo->instanceID);
		m_EditMode = true;
	}
}
void EditExt::GetAnimationHandle(int& anim)
{
	anim = m_iAnimationRoot;
}

// Constructor
EditExt::EditExt(VEditTime* pVEditTime, editInfo* pEInfo)
{
	// Store the edittime and einfo pointers
	pEditTime = pVEditTime;
	pInfo = pEInfo;

	LeftMouse = false;
}

// Destructor: when your object is deleted or the application closed:
// close any allocated resources here
EditExt::~EditExt()
{

}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and any other DirectX volatile data here.
void EditExt::Initialize()
{

}

// Called when any property has been changed.
void EditExt::OnPropertiesUpdate()
{
}

// When the object is inserted to the layout editor by the user.
void EditExt::OnPut()
{
	m_iAnimationRoot = pEditTime->AddAnimation(32, 32, 0, "Tileset", -1);

	pEditTime->SetAnimationModificationSettings(m_iAnimationRoot,	
		true, false, true, false, true);

	pInfo->objectWidth = 640;
	pInfo->objectHeight = 480;
	m_EditMode = false;
	m_ViewTiles = false;

	m_IsSel = false;
	m_IsSelBox = false;
	m_SelectingFromBoard = false;
	m_vt_IsSelBox = false;
	m_vt_IsSel = false;

	USEMYRESOURCES();

	int tile1 = pEditTime->AddImageFromResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_PNG2));
	int tile2 = pEditTime->AddImageFromResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_PNG1));


	pEditTime->InsertFrameIntoAnimation(m_iAnimationRoot, 0, tile1);
	pEditTime->InsertFrameIntoAnimation(m_iAnimationRoot, 0, tile2);
	
	ResizeBoardToObjectSize(); //call this so we resize the map

	LeftMouse = false;


}


// When the object is removed from the layout editor by the user.
void EditExt::OnRemoved()
{

}

// Get any icons needed to show this ACEs menu
int EditExt::GetMenuIcons(int ACEMenu)
{
	// Todo: return the parameter number of a bitmap carrying an imagelist
	//       to show in the menu indicated by ACEMenu

	return -1; // -1 indicates no icons will be used
}


int EditExt::OnShowACEMenu(TABLE ACEType)
{
	// You can return the resource number indicating the menu to show for the
	//       ACE type indicated in ACEType
	// This overrides the default menu

	return -1;	// Return -1 for standard menu to be loaded
}

void EditExt::ResizeBoardToObjectSize()
{
	cr::point size = GetTileSize(); 

	size.x *= pEditTime->GetZoom();
	size.y *= pEditTime->GetZoom();

	int cols = ceil(pInfo->objectWidth / float(size.x));
	int rows = ceil(pInfo->objectHeight / float(size.y));

	pInfo->objectWidth = cols * size.x;
	pInfo->objectHeight = rows * size.y;

	int w = max(1, cols);
	int h = max(1, rows);

	if(w != tiles.size())
		tiles.resize(w);

	for(int i = 0; i < tiles.size(); i++)
		if(h != tiles[i].size())
			tiles[i].resize(h);
}

// On object resized
BOOL EditExt::OnSizeObject()
{
	ResizeBoardToObjectSize();

	return TRUE;
}
void WINAPI ETOnMessage(editInfo* editObject, int message)
{
	editObject->objectPtr->OnMessage(message);
}
void EditExt::GetViewTileInfo(float* _xoffset, float* _yoffset, int* _tilecount, int* _tilecols, int* _tilerows)
{
	cr::point size = GetTileSize();
	size.x *= pEditTime->GetZoom();
	size.y *= pEditTime->GetZoom();

	CRect client;
	GetClientRect(pEditTime->GetFrameHwnd(), &client);

	float x = 0.0f;
	float y = 0.0f;
	pEditTime->TranslateFrameToScreenCoords(x,y);

	float xoffset = -x + client.Width() / 2.0;
	float yoffset = -y + client.Height() / 2.0;

	int tilecount = pEditTime->QueryAnimationFrameCount(m_iAnimationRoot);
	int tilecols = 10;
	int tilerows = ceil((float)tilecount / tilecols);

	xoffset -= tilecols / 2.0 * size.x;
	yoffset -= tilerows / 2.0 * size.y;

	if(_xoffset)
		*_xoffset = xoffset;
	if(_yoffset)
		*_yoffset = yoffset;
	if(_tilecount)
		*_tilecount = tilecount;
	if(_tilecols)
		*_tilecols = tilecols;
	if(_tilerows)
		*_tilerows = tilerows;
}


void EditExt::MouseToBoard(CPoint& mouse)
{	
	cr::point size = GetTileSize();

	float ox = pInfo->objectX;
	float oy = pInfo->objectY;
	float zoom = pEditTime->GetZoom();
	mouse.x /= zoom;
	mouse.y /= zoom;
	mouse.x -= ox;
	mouse.y -= oy;
	mouse.x = floor(mouse.x / size.x);
	mouse.y = floor(mouse.y / size.y);
}

void EditExt::MouseToViewTile(CPoint& mouse)
{	
	cr::point size = GetTileSize();

	float xoffset, yoffset;
	int tilecount, tilecols, tilerows;
	GetViewTileInfo(&xoffset, &yoffset, &tilecount, &tilecols, &tilerows);

	float ox = xoffset;
	float oy = yoffset;
	float zoom = pEditTime->GetZoom();
	mouse.x -= ox;
	mouse.y -= oy;
	mouse.x /= zoom;
	mouse.y /= zoom;
	mouse.x = floor(mouse.x / size.x);
	mouse.y = floor(mouse.y / size.y);
}

int EditExt::GetTileImage(int index)
{
	if(index < 0)
		return -1;
	if(index >= pEditTime->QueryAnimationFrameCount(m_iAnimationRoot))
		return -1;
	return pEditTime->GetAnimationFrame(m_iAnimationRoot,index);
}

int EditExt::GetViewTileAt(CPoint xy)
{
	float xoffset, yoffset;
	int tilecount, tilecols, tilerows;
	GetViewTileInfo(&xoffset, &yoffset, &tilecount, &tilecols, &tilerows);

	if(xy.x < 0 || xy.y < 0)
		return -1;
	if(xy.x >= tilecols || xy.y >= tilerows)
		return -1;

	int i = xy.x + xy.y * tilecols;
	if(i < 0)
		return -1;
	if(i >= pEditTime->QueryAnimationFrameCount(m_iAnimationRoot))
		return -1;
	return i;//pEditTime->GetAnimationFrame(m_iAnimationRoot,i);
}

void EditExt::SetTileGhost(map<CMapPoint,int, less_point>& sel)
{
	CRect bounding;
	if(sel.size() > 0)
	{
		bounding.left = bounding.right = sel.begin()->first.x;
		bounding.top = bounding.bottom = sel.begin()->first.y;
	}
	// Loop all the items in the sel and work out a bounding box of co-ordinates
	for(map<CMapPoint,int, less_point>::iterator i = sel.begin(); i!= sel.end(); i++)
	{
		const CMapPoint& pt = i->first;
		bounding.left = min(bounding.left, pt.x);
		bounding.right = max(bounding.right, pt.x);

		bounding.top = min(bounding.top, pt.y);
		bounding.bottom = max(bounding.bottom, pt.y);
	}
	
	// Calculate an offset
	int offsetx = bounding.Width() / 2 + bounding.left;
	int offsety = bounding.Height() / 2 + bounding.top;

	m_ghost.clear();

	// Loop, add and offset
	for(map<CMapPoint,int, less_point>::iterator i = sel.begin(); i!= sel.end(); i++)
	{
		const CMapPoint& pt = i->first;
		CMapPoint point(pt.x - offsetx, pt.y - offsety);

		m_ghost[point] = i->second;
	}
}

void EditExt::OnMessage(int message)
{
	float ox = pInfo->objectX;
	float oy = pInfo->objectY;
	float zoom = pEditTime->GetZoom();
	CPoint mouse = pEditTime->GetCursor();




	switch (message)
	{
	case WM_RBUTTONDOWN:
		{
			if(!(m_vt_IsSelBox && m_vt_IsSel && m_IsSelBox && m_IsSel)) // Otherwise, it hides shows the 'view tiles' screen
			{
				m_ViewTiles = !m_ViewTiles;

				if(m_ViewTiles)
				{
					// Initialise




				}
				else
				{
					// End
					SetTileGhost(m_vt_sel);
					m_vt_sel.clear();
				}
			}

			CWnd* FrameWindow = CWnd::FromHandle(pEditTime->GetFrameHwnd());
			FrameWindow->Invalidate();
		}
		break;
	case WM_KEYDOWN:
		{
			if(GetKeyState(VK_BACK) >> 4)
			{
				map<CMapPoint,int, less_point> erase;
				erase[CMapPoint(0,0)] = -1;
				SetTileGhost(erase);
			}

		}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		{

			// This seems hell hacky but basically the layout editor doesn't capture the mouse so if it leaves the screen and releases...then
			// comes back on the screen we have to check if its registered as not being down by the win32 GetKeyState function
			if(message == WM_LBUTTONDOWN)
				LeftMouse = true;
			if(message == WM_LBUTTONUP)
				LeftMouse = false;
			if( (GetKeyState(VK_LBUTTON)>>4) == 0 )
				LeftMouse = false;

			////////////////////////////////////////////////
			if(m_ViewTiles) // View tile mode
			{
				CPoint cursor = pEditTime->GetCursor();
				MouseToViewTile(cursor);

				if(m_vt_IsSel != LeftMouse)
				{
					m_vt_IsSel = LeftMouse;
					
					if(LeftMouse)
					{
						m_vt_sel_x = cursor.x;
						m_vt_sel_y = cursor.y;

						CMapPoint pt(cursor.x, cursor.y);
						if(m_vt_sel.find(pt) == m_vt_sel.end())
							m_vt_AddSel = true;
						else
							m_vt_AddSel = false;
						if(!(GetKeyState(VK_SHIFT)>>4) && !(GetKeyState(VK_CONTROL)>>4) )
							m_vt_AddSel = true;

					}
					else //release
					{
						if(m_vt_IsSelBox)
						{
							CRect box;
							box.left   = min(m_vt_sel_x, cursor.x);
							box.top    = min(m_vt_sel_y, cursor.y);
							box.right  = max(m_vt_sel_x, cursor.x);
							box.bottom = max(m_vt_sel_y, cursor.y);

							if(!(GetKeyState(VK_SHIFT)>>4) && !(GetKeyState(VK_CONTROL)>>4))
							{
								m_vt_AddSel = true;
								m_vt_sel.clear();
							}

							for(int y = box.top; y <= box.bottom; y++)
								for(int x = box.left; x <= box.right; x++)
								{
									CMapPoint pt(x,y);
									int tile = GetViewTileAt(pt);
									if(m_vt_AddSel)
									{
										if(tile != -1)
											m_vt_sel[pt] = tile;
									}
									else
									{
										m_vt_sel[pt] = 0; 
										m_vt_sel.erase(m_vt_sel.find(pt));
									}
								}
						}
								
					}
			
				}

				m_vt_IsSelBox = LeftMouse;

				if(GetKeyState(VK_CONTROL)>>4)
				{
					m_vt_sel_x = cursor.x;
					m_vt_sel_y = cursor.y;
					m_vt_IsSelBox = false;

					if(LeftMouse)
					{
						CMapPoint pt(cursor.x, cursor.y);
						int tile = GetViewTileAt(cursor);
						
						if(m_vt_AddSel)
						{
							if(tile != -1)
								m_vt_sel[pt] = tile;
						}
						else
						{
							m_vt_sel[pt] = 0; 
							m_vt_sel.erase(m_vt_sel.find(pt));
						}
					}

				}

				CWnd* FrameWindow = CWnd::FromHandle(pEditTime->GetFrameHwnd());
				FrameWindow->Invalidate();
			}
			//////////////////////////////////////
			else //board mode
			{
				if(m_SelectingFromBoard)
				{
					CPoint cursor = pEditTime->GetCursor();
					MouseToBoard(cursor);

					if(m_IsSel != LeftMouse)
					{
						m_IsSel = LeftMouse;
						
						if(LeftMouse)
						{
							m_sel_x = cursor.x;
							m_sel_y = cursor.y;

							CMapPoint pt(cursor.x, cursor.y);
							if(m_sel.find(pt) == m_sel.end())
								m_AddSel = true;
							else
								m_AddSel = false;
							if(!(GetKeyState(VK_SHIFT)>>4) && !(GetKeyState(VK_CONTROL)>>4) )
								m_AddSel = true;

						}
						else //release
						{
							if(m_IsSelBox)
							{
								CRect box;
								box.left   = min(m_sel_x, cursor.x);
								box.top    = min(m_sel_y, cursor.y);
								box.right  = max(m_sel_x, cursor.x);
								box.bottom = max(m_sel_y, cursor.y);

								for(int y = box.top; y <= box.bottom; y++)
									for(int x = box.left; x <= box.right; x++)
									{
										CMapPoint pt(x,y);
										int tile = GetTile(x,y);
										if(m_AddSel)
										{
											if(tile != -1)
												m_sel[pt] = tile;
										}
										else
										{
											m_sel[pt] = 0; 
											m_sel.erase(m_sel.find(pt));
										}
									}
							}
									
						}
				
					}

					m_IsSelBox = LeftMouse;

					if(GetKeyState(VK_CONTROL)>>4)
					{
						m_sel_x = cursor.x;
						m_sel_y = cursor.y;
						m_IsSelBox = false;

						if(LeftMouse)
						{
							CMapPoint pt(cursor.x, cursor.y);
							
							int tile = GetTile(cursor.x, cursor.y);
							
							if(m_AddSel)
							{
								if(tile != -1)
									m_sel[pt] = tile;
							}
							else
							{
								m_sel[pt] = 0; 
								m_sel.erase(m_sel.find(pt));
							}
						
						}
					}

				}
				if(m_SelectingFromBoard != (GetKeyState(VK_SHIFT)>>4 || GetKeyState(VK_CONTROL)>>4))
				{
					if(!m_SelectingFromBoard)
					{
						//Init Select
						m_ghost.clear();
					}
					else
					{
						SetTileGhost(m_sel);
						m_sel.clear();
					}
					m_SelectingFromBoard = (GetKeyState(VK_SHIFT)>>4 || GetKeyState(VK_CONTROL)>>4);

				}
				// END SELECTION STUFF
				if(!m_SelectingFromBoard && LeftMouse)
				{
					MouseToBoard(mouse);
					if(!m_IsSelBox)
					{
						for(map<CMapPoint,int, less_point>::iterator i = m_ghost.begin(); i!= m_ghost.end(); i++)
						{
							CPoint pt(i->first.x + mouse.x, i->first.y + mouse.y);

							
							int image = i->second;
							SetTile(pt.x, pt.y, image);
							
						}
					}
				}

				CWnd* FrameWindow = CWnd::FromHandle(pEditTime->GetFrameHwnd());
				FrameWindow->Invalidate();
			}
		}
	}
}



#else //ifndef RUN_ONLY

BOOL WINAPI ETCreateObject(editInfo* editObject) {return FALSE;}
BOOL WINAPI ETInitializeObject(editInfo* editObject, VEditTime* pVEditTime) {return FALSE;}
BOOL WINAPI ETDestroyObject(editInfo* editObject) {return FALSE;}
int WINAPI ETDrawObject(editInfo *editObject) {return 0;}
void ETOnPropertyChanged(editInfo* editObject) {}
long LinkPropTable() {return 0;}
void WINAPI ETSerialize(editInfo* editObject, CArchive& ar) {}
void WINAPI ETPutObject(editInfo* editObject) {}
void WINAPI ETRemoveObject(editInfo* editObject) {}
void WINAPI GetInfo(LPOINFO objectInfo) {}
int WINAPI ETGetMenuIcons(editInfo* editObject, int ACEMenu) {return 0;}
int WINAPI ETShowACEMenu(editInfo* editObject, TABLE ACEType)  {return 0;}
BOOL WINAPI ETSizeObject(editInfo* editObject)  {return FALSE;}
void WINAPI ETOnMessage(editInfo* editObject, int message){}
#endif // RUN_ONLY

BOOL WINAPI ETOnNotify(editInfo* editObject, int notification)
{
#ifdef RUN_ONLY
	return 0;
#else
	if(notification == 1)
	{
		((EditExt*)editObject->objectPtr)->BtnEditMode();
		return TRUE;
	}
	if(notification == 4)
	{
		((EditExt*)editObject->objectPtr)->OnSizeObject();
		return TRUE;
	}
	return 0;
#endif
}