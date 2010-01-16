// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

/////////////////////////////
// RUNTIME drawing
// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your plugin doesn't draw anything.  This is not called when OF_NODRAW is set.


cr::point ExtObject::GetTileSize()
{
	return cr::point(tileWidth,tileHeight);
}



void ExtObject::Draw()
{
	cr::point size = GetTileSize(); 

	int startx = max(0, (pLayout->scrollX - info.x ) / size.x);
	int starty = max(0, (pLayout->scrollY - info.y ) / size.y);


	int cols_end = ceil( min(info.w, pLayout->winWidth + (startx+1)*size.x ) / float(size.x));
	int rows_end = ceil( min(info.h, pLayout->winHeight + (starty+1)*size.y) / float(size.y));



	cr_color col = info.pInfo->filter;

	//IMPORTANT: During drawing, the pInfo is modified by construct for the zooming
	// so to use MousetoBoard we have to unzoom

//	int w = max(1, cols+1);
//	int h = max(1, rows+1);

	int x,y;
		
	cr::rect uv;
	uv.left = 0.0f;
	uv.top = 0.0f;

	for (x = startx; x < cols_end; x++)
	{
		for (y = starty; y < rows_end; y++)
		{
			cr::point blitsize;
			blitsize.x = min(size.x, info.w - x*size.x);
			blitsize.y = min(size.y, info.h - y*size.y);

			uv.right =  min(1, blitsize.x / float(size.x));
			uv.bottom = min(1, blitsize.y / float(size.y));

			int tile = tiles[x][y].index;
			if(tile != -1)
			{
				TextureHandle iTexture = textures[tile];
				pRuntime->pRenderer->SetTexture(iTexture);
				pRuntime->pRenderer->Quad_xywh(info.x + x*size.x, info.y + y*size.y, blitsize.x, blitsize.y, 0.0f, origin, col, &uv);
			}
		}
	}
}

#else // RUN_ONLY

/////////////////////////////
// EDITTIME drawing
// Draw your object in the layout editor.

cr::point EditExt::GetTileSize()
{
	return cr::point(tileWidth,tileHeight);
}

int EditExt::GetTile(int x, int y)
{
	if(x < 0)
		return -1;
	if(y < 0)
		return -1;
	if( x >= tiles.size())
		return -1;
	if( y >= tiles[x].size() )
		return -1;
	return tiles[x][y].index;
}
BOOL EditExt::SetTile(int x, int y, int index)
{
	if(x < 0)
		return FALSE;
	if(y < 0)
		return FALSE;
	if( x >= tiles.size())
		return FALSE;
	if( y >= tiles[x].size() )
		return FALSE;
	tiles[x][y].index = index;
	return TRUE;
}
void EditExt::Draw()
{
	cr::point size = GetTileSize(); 

	size.x *= pEditTime->GetZoom();
	size.y *= pEditTime->GetZoom();

	int cols = ceil(pInfo->objectWidth / float(size.x));
	int rows = ceil(pInfo->objectHeight / float(size.y));

	D3DCOLOR dc = pInfo->filter; //D3D_OPACITYCOLORREF((int)(opacity * 255.0), filter);

	//IMPORTANT: During drawing, the pInfo is modified by construct for the zooming
	// so to use MousetoBoard we have to unzoom
	CPoint cursor = pEditTime->GetCursor();
	float zoom = pEditTime->GetZoom();
	pInfo->objectX /= zoom;
	pInfo->objectY /= zoom;
	MouseToBoard(cursor);
	pInfo->objectX *= zoom;
	pInfo->objectY *= zoom;

	int x,y;
		
	RECTF uv;
	uv.left = 0.0f;
	uv.top = 0.0f;

	CRect selection_box;
	selection_box.left   = min(m_sel_x, cursor.x);
	selection_box.top    = min(m_sel_y, cursor.y);
	selection_box.right  = max(m_sel_x, cursor.x)+1;
	selection_box.bottom = max(m_sel_y, cursor.y)+1;


	if(m_EditMode)
	{
		if((GetKeyState(VK_SHIFT) >> 4 || GetKeyState(VK_CONTROL) >> 4) && !m_ViewTiles)
		{
			pEditTime->SetTexture(-1);
			CRect rc(pInfo->objectX, pInfo->objectY, pInfo->objectX + pInfo->objectWidth, pInfo->objectY + pInfo->objectHeight);
			pEditTime->Fill(rc, 0xffffffff);
	
			dc = 0x88ffffff;
		}
	}


	for (x = 0; x < cols; x++)
	{
		for (y = 0; y < rows; y++)
		{
			cr::point blitsize;
			blitsize.x = min(size.x, pInfo->objectWidth - x*size.x);
			blitsize.y = min(size.y, pInfo->objectHeight - y*size.y);

			uv.right =  min(1, blitsize.x / float(size.x));
			uv.bottom = min(1, blitsize.y / float(size.y));

			int tile = GetTile(x,y); 

			bool select = false;
			bool mouseover = false;

			CMapPoint pos(x, y);
			if(m_SelectingFromBoard)
			{
				if(m_sel.find(pos) != m_sel.end())
					select = true;

				if(selection_box.PtInRect(pos) && m_IsSel)
					select = m_AddSel;
			}
			if(cursor.x == pos.x && cursor.y == pos.y && !m_IsSel)
			{
				mouseover = true;
			}		

			if(select)
			{
				pEditTime->SetTexture(-1);
				pEditTime->Blitrc(pInfo->objectX + x*size.x, pInfo->objectY + y*size.y, blitsize.x, blitsize.y, 0.0f, 0x330000ff);
			}

			if(tile != -1)
			{
				int iTexture = pEditTime->AddImageAsTexture(GetTileImage(tile));
				pEditTime->SetTexture(iTexture);
				if(select || mouseover)
					pEditTime->Blitrc(pInfo->objectX + x*size.x, pInfo->objectY + y*size.y, blitsize.x, blitsize.y, 0.0f, 0xffffffff, &uv);
				else
					pEditTime->Blitrc(pInfo->objectX + x*size.x, pInfo->objectY + y*size.y, blitsize.x, blitsize.y, 0.0f, dc, &uv);
			}
			// Mouse hover
			if(mouseover && m_EditMode)
			{
				pEditTime->SetTexture(-1);
				pEditTime->Blitrc(pInfo->objectX + x*size.x, pInfo->objectY + y*size.y, blitsize.x, blitsize.y, 0.0f, 0x330000ff);
			}		

			if(m_EditMode && !m_ViewTiles)
			{
				CMapPoint pt(x - cursor.x, y - cursor.y);

				if(m_ghost.find(pt) != m_ghost.end())
				{
					tile = GetTileImage(m_ghost[pt]);
					pEditTime->SetTexture(-1);
					pEditTime->Blitrc(pInfo->objectX + x*size.x, pInfo->objectY + y*size.y, blitsize.x, blitsize.y, 0.0f, 0x330000ff, &uv);

					if(tile == -1)
						continue;
					
					int iTexture = pEditTime->AddImageAsTexture(tile);
					pEditTime->SetTexture(iTexture);
					pEditTime->Blitrc(pInfo->objectX + x*size.x, pInfo->objectY + y*size.y, blitsize.x, blitsize.y, 0.0f, 0x88ffffff, &uv);

				}
			}
		}
	}


	if(m_EditMode)
	{
		pEditTime->SetTexture(-1);

		// Create a dark blue glow
		CRect rc(pInfo->objectX, pInfo->objectY, pInfo->objectX + pInfo->objectWidth, pInfo->objectY + pInfo->objectHeight);
		rc.InflateRect(1,1);
		pEditTime->Box(rc, 0xff111199);
		rc.InflateRect(1,1);
		pEditTime->Box(rc, 0x88111199);
		rc.InflateRect(1,1);
		pEditTime->Box(rc, 0x55111199);
		rc.InflateRect(1,1);
		pEditTime->Box(rc, 0x22111199);

		CRect client;
		GetClientRect(pEditTime->GetFrameHwnd(), &client);

		float x = 0.0f;
		float y = 0.0f;
		pEditTime->TranslateFrameToScreenCoords(x,y);

		// Show our tileset
		if(m_ViewTiles)
		{
			pEditTime->Blitrc(-x, -y, client.Width(), client.Height(), 0.0f, 0xaaffffff);

			float xoffset, yoffset;
			int tilecount, tilecols, tilerows;

			GetViewTileInfo(&xoffset, &yoffset, &tilecount, &tilecols, &tilerows);
	
			pEditTime->Blitrc(xoffset-1, -y, tilecols * size.x+2, client.Height(), 0.0f, 0x77000099);
			pEditTime->Blitrc(xoffset, -y, tilecols * size.x, client.Height(), 0.0f, 0xeeffffff);

			float x = xoffset;
			float y = yoffset;

			CPoint cursor = pEditTime->GetCursor();
			MouseToViewTile(cursor);

			CRect selection_box;
			selection_box.left   = min(m_vt_sel_x, cursor.x);
			selection_box.top    = min(m_vt_sel_y, cursor.y);
			selection_box.right  = max(m_vt_sel_x, cursor.x)+1;
			selection_box.bottom = max(m_vt_sel_y, cursor.y)+1;

			for(int i = 0; i < tilecount; i ++)
			{
				int tile = pEditTime->GetAnimationFrame(m_iAnimationRoot,i);
				int iTexture = pEditTime->AddImageAsTexture(tile);
			
				bool select = false;
				bool mouseover = false;

				CMapPoint pos(i%tilecols, i/tilecols);

				if(m_vt_sel.find(pos) != m_vt_sel.end())
				{
					select = true;
				}
				if(!(GetKeyState(VK_SHIFT)>>4 != 0) && m_vt_IsSelBox)
				{
					// If we are dragging without shift, hide the current selection
					select = false;
				}

				if(selection_box.PtInRect(pos) && m_vt_IsSel)
				{
					select = m_vt_AddSel;
				}

				if(cursor.x == pos.x && cursor.y == pos.y && !m_vt_IsSel)
					mouseover = true;

				if(select)
				{
					pEditTime->SetTexture(-1);
					pEditTime->Blitrc(x, y, size.x, size.y, 0.0f, 0x330000ff);
				}

				pEditTime->SetTexture(iTexture);

				if(select || mouseover || (m_vt_sel.size() == 0 && !LeftMouse))
					pEditTime->Blitrc(x, y, size.x, size.y, 0.0f, dc);			
				else
					pEditTime->Blitrc(x, y, size.x, size.y, 0.0f, 0x88ffffff);





				// Mouse hover
				if(mouseover)
				{
					pEditTime->SetTexture(-1);
					pEditTime->Blitrc(x, y, size.x, size.y, 0.0f, 0x330000ff);
				}

				x += size.x;
				if((i+1)%tilecols == 0 && i!= 0)
				{
					y += size.y;
					x = xoffset;
				}
			}



		}




	}


	
}

#endif // RUN_ONLY