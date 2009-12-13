#include "StdAfx.h"
#include "CPictureEditor.h"
#include "ImageEditor.h"
#include "..\Dialogs\ImageEditorDlg.h"

// Todo: use the following check to see if D3DFMT_A16B16G16R16F is supported:
/*
BOOL FloatTexture16Supported()
{
	HRESULT hr = Display.d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									D3DFMT_X8R8G8B8,
									D3DUSAGE_RENDERTARGET,
									D3DRTYPE_TEXTURE,
									D3DFMT_A16B16G16R16F);

	return SUCCEEDED(hr);
}
*/
//#define FORMAT D3DFMT_A16B16G16R16F
//#define FORMAT D3DFMT_A8R8G8B8

// Brush filter - used to create psp style brushes...wish i could have worked out the maths for this
static BYTE BrushFilter[256] = {0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 4, 5, 5, 6, 6, 7, 7, 8, 10, 10, 11, 13, 13, 14, 16, 16, 17, 19, 20, 21, 22, 24, 25, 27, 28, 29, 31, 33, 34, 35, 37, 39, 40, 42, 43, 45, 47, 49, 51, 52, 54, 56, 58, 60, 62, 63, 66, 67, 69, 71, 73, 75, 77, 78, 81, 82, 85, 87, 89, 91, 92, 95, 97, 98, 101, 103, 105, 107, 109, 111, 113, 115, 117, 119, 121, 123, 124, 127, 129, 130, 133, 135, 137, 138, 140, 142, 144, 146, 148, 150, 151, 153, 155, 157, 159, 160, 162, 164, 166, 167, 169, 170, 172, 174, 175, 177, 178, 180, 181, 182, 185, 185, 187, 188, 190, 192, 193, 194, 196, 197, 199, 199, 200, 201, 203, 204, 205, 206, 207, 209, 211, 211, 213, 213, 214, 215, 217, 217, 218, 219, 220, 221, 222, 222, 223, 224, 225, 226, 227, 228, 228, 229, 230, 231, 231, 232, 232, 233, 234, 234, 235, 235, 236, 236, 237, 238, 239, 239, 239, 240, 240, 241, 241, 241, 242, 242, 243, 243, 244, 244, 244, 244, 245, 245, 246, 246, 246, 247, 247, 247, 248, 248, 248, 248, 249, 249, 249, 249, 249, 249, 250, 250, 250, 250, 250, 250, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255};

inline void PremultiplyFilter(D3DCOLOR& filter)
{
	// Premultiply filter
	int a, r, g, b;
	a = D3D_A(filter);

	if (a == 255) return;

	float af = (float)a / 255.0f;
	r = (float)D3D_R(filter) * af;
	g = (float)D3D_G(filter) * af;
	b = (float)D3D_B(filter) * af;

	filter = D3D_ARGB(a, r, g, b);
}


D3DFORMAT CPictureEditor::GetTextureFormat()
{
	/*
	HRESULT hr = display->d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									D3DFMT_X8R8G8B8,
									D3DUSAGE_RENDERTARGET,
									D3DRTYPE_TEXTURE,
									D3DFMT_A16B16G16R16F);

	if(SUCCEEDED(hr))
	{
		return D3DFMT_A8R8G8B8;
		//return D3DFMT_A16B16G16R16F;
	}
	else
	{
	*/
		return D3DFMT_A8R8G8B8;
	//}

}

// Co-ordinate Conversions
void CPictureEditor::ScreentoCanvas(CFloatPoint& pt)
{
	CSize Offset = GetPositionOffset();

	pt.x = (pt.x -Offset.cx) / m_Zoom - m_RectTempToCanvas.left;
	pt.y = (pt.y -Offset.cy) / m_Zoom - m_RectTempToCanvas.top;
}

void CPictureEditor::CanvastoScreen(CFloatPoint& pt)
{
	CSize Offset = GetPositionOffset();

	pt.x = (pt.x + m_RectTempToCanvas.left) * m_Zoom +Offset.cx;
	pt.y = (pt.y + m_RectTempToCanvas.top) * m_Zoom +Offset.cy;

}

void CPictureEditor::Initialise(CD3DDisplay* mydisplay,CxImage* MyImage)
{
	display = mydisplay;
	
	m_Zoom = 1.0;

	m_ScrollX = 0;
	m_ScrollY = 0;

	timer = 0;


	SaveDisplayState();
	display->SetRenderTarget(-1);
	display->SetTexture(-1);

	// Create pattern bg
	SaveDisplayState();

	m_Background = display->AddTextureRT(16,16);
	
	
	display->SetRenderTarget(m_Background);
	display->ClearRT(0xffffffff);
	display->BeginScene();
	display->Fill(0,0,8,8,0xffeeeeee);
	display->Fill(8,8,8,8,0xffeeeeee);

	RestoreDisplayState();
		

	// Load our canvas from CxImage
	int m_CanvasBG = display->AddTextureFromCxImage(*MyImage);

	// Create our canvas ( we need to convert it to a render target )
	m_CanvasWidth = m_Width = MyImage->GetWidth();
	m_CanvasHeight = m_Height = MyImage->GetHeight();


	layers.push_back(display->AddTextureRT(m_Width,m_Height, GetTextureFormat()));
	m_pCanvas = &layers.at(0);
	m_pLayer = m_pCanvas;
	if (*m_pCanvas < 0) 
		MessageBox(NULL, "Error creating the canvas", "Error", MB_OK);
	SaveDisplayState();
	display->SetRenderTarget(*m_pCanvas);
	display->SetTexture(m_CanvasBG);
	display->ClearRT();
			
	display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);

	display->Blit(0,0);
	RestoreDisplayState();

	m_TempCanvas = -2;
	m_Mask = -2;
	m_CanvasPromoted = -2;
	m_hasMask = false;
	


//	SaveDisplayState();
//	display->SetRenderTarget(m_Mask);
//	display->ClearRT(0xff000000); // make opaque
//	display->SetRenderTarget(-1);
//	RestoreDisplayState();

	// Make the translation between Temp->Canvas normal
	m_RectTempToCanvas.SetRect(0,0,m_Width,m_Height);

//	m_pCanvas = &m_Canvas;



	// Create a brush ( default tool )
	

	CurrentTool = 0;


	CanvasToTemp();


	CreateUndo();
	m_modded = false;
//	Promote();

	display->RemoveTexture(m_CanvasBG);
	display->EndBatch();
	RestoreDisplayState();
	
}







inline void SetVertex(TLVERTEX* vertex, D3DCOLOR color, float x, float y, float u, float v)
{
	vertex->colour = color;
	vertex->x = x;
	vertex->y = y;
	vertex->z = 0.0f;
#ifndef USE_3D_CORDS
	vertex->rhw = 1.0f;
#endif
	vertex->u = u;
	vertex->v = v;

}
void CPictureEditor::CreateUndo(CString reason)
{
	CPicUndo* undo = new CPicUndo;
	undo->Save(this, reason);

	m_undo.push_back(undo);


	for(int a = 0; a < m_redo.size(); a ++)
	{
		m_redo[a]->Release();
		delete m_redo[a];
	}
	m_redo.clear(); 

	m_modded = true;

}




void CPictureEditor::Undo()
{
	if(m_undo.size() > 1)
	{
		CPicUndo* undo = m_undo[m_undo.size()-1];
		CPicUndo* viewed = m_undo[m_undo.size()-2];
		viewed->Restore(this);
		
		m_redo.push_back(undo);
		m_undo.pop_back();
		
	}
}
void CPictureEditor::Redo()
{
	if(m_redo.size() > 0)
	{
		CPicUndo* undo = m_redo.back();
		undo->Restore(this);
		
		m_redo.pop_back();
		m_undo.push_back(undo);
	}
}

D3DTEXTUREFILTERTYPE CPictureEditor::GetBlendType()
{
	if(CurrentTool)
		if(CurrentTool->m_pProp->bSmooth)
			return D3DTEXF_LINEAR;
	
	return D3DTEXF_POINT;
}


void CPictureEditor::Render()
{
	timer ++;

	if (m_TempCanvas == -2)
		m_TempCanvas = display->AddTextureRT(m_CanvasWidth,m_CanvasHeight, GetTextureFormat());

	//display->BeginScene();
	display->SetRenderTarget(m_TempCanvas);
	display->SetTexture(-1);
	if(CurrentTool)
		CurrentTool->Render();

	display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);


	display->SetRenderTarget(-1);

	display->ClearRT(0xff000000);
	display->SetTexture(m_Background);

	CheckerBackground();




	// Find size of window
	CSize Offset = GetPositionOffset();

	int BlitWidth = m_Width * m_Zoom;
	int BlitHeight = m_Height * m_Zoom;

	for(int a = 0; a < layers.size(); a ++)
	{
		display->SetTexture(layers.at(a));
		if(layers.at(a) == *m_pLayer)
		{
			if(m_pLayer == m_pCanvas)	// Drawing onto the layer
			{
				display->SetSamplerState(D3DSAMP_MAGFILTER, D3DTEXF_POINT);
				if(CurrentTool)
					CurrentTool->OverlayCallback();
				else
					display->Blit(Offset.cx,Offset.cy,BlitWidth,BlitHeight);
				display->SetSamplerState(D3DSAMP_MAGFILTER, GetBlendType());
			}
			else	// We have a promoted selection
			{
				display->SetSamplerState(D3DSAMP_MAGFILTER, D3DTEXF_POINT);
				display->Blit(Offset.cx,Offset.cy,BlitWidth,BlitHeight);
				

				SaveDisplayState();
				if(m_Mask != -2)
				{
					display->SetTexture(m_Mask);
					display->SetRenderTarget(*m_pCanvas);
					display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ZERO);
					display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_SRCALPHA);
					display->Blit(0,0);
				}
				RestoreDisplayState();
				display->SetTexture(*m_pCanvas);

				if(CurrentTool)
					CurrentTool->OverlayCallback();
				else
				{
					display->SetSamplerState(D3DSAMP_MAGFILTER, D3DTEXF_POINT);
					display->Blit(Offset.cx,Offset.cy,BlitWidth,BlitHeight);
				}
				display->SetSamplerState(D3DSAMP_MAGFILTER, GetBlendType());
			}
		}
		else
		{
			display->SetSamplerState(D3DSAMP_MAGFILTER, D3DTEXF_POINT);
			display->Blit(Offset.cx,Offset.cy,BlitWidth,BlitHeight);
			display->SetSamplerState(D3DSAMP_MAGFILTER, GetBlendType());
		}
	}

	if(CurrentTool)
		CurrentTool->PostRender();
	display->SetTexture(-1);

	// draw the outside bo for the texture
	display->Box(Offset.cx - 1, Offset.cy - 1, BlitWidth + 2, BlitHeight + 2, 0xff000000);

	if(bEditingCollision)
	{
		display->Box(Offset.cx - 2, Offset.cy - 2, BlitWidth + 4, BlitHeight + 4, 0xffff0000); // red outline
	}

	// if we're drawing a grid, whack that in too
	if (grid)
	{
		CRect grid_rect(Offset.cx - 1, Offset.cy - 1, BlitWidth + 2, BlitHeight + 2);

		for (int i = 0; i < m_Height; i += 2)
			display->Line(grid_rect.left, grid_rect.top + (i * m_Zoom), grid_rect.left + grid_rect.right, grid_rect.top + (i * m_Zoom), 0xff000000);
	}

	if(GetKeyState(VK_LBUTTON) >> 2 || GetKeyState(VK_RBUTTON) >> 2)
	{
		// Dont draw selection if the mouse is down
	}
	else
	{
		DrawSelection();
	}

	// End
	display->SetTexture(-1);
	display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
	display->SetRenderTarget(m_TempCanvas);
	display->EndBatch();
	
	//display->EndScene();
	


}
void CPictureEditor::PostRender()
{
	display->BeginScene();
}


void CPictureEditor::Save(CxImage* SaveTo)
{

	DropSelection();


	display->EndBatch();


	LPD3DXBUFFER ppDestBuf;

	D3DXSaveTextureToFileInMemory(
	  &ppDestBuf,
	  D3DXIFF_PNG,
	  display->TextureAt(*m_pCanvas),
	  NULL
	);

	ppDestBuf->GetBufferSize();


	SaveTo->Decode((BYTE*)ppDestBuf->GetBufferPointer()
				   ,ppDestBuf->GetBufferSize(),CXIMAGE_FORMAT_PNG);


	ppDestBuf->Release();
	// NOTE: do i have to deallate the buffer???

}

void CPictureEditor::Rotate(int Angle)
{
	DropSelection();

	RGBQUAD trans = {0,0,0,0};
	CxImage& picture = m_undo.back()->src;
	picture.Rotate2(Angle, NULL, CxImage::IM_BILINEAR,
		CxImage::OM_COLOR, &trans);

	Load(&picture);
	ClearTemp();
	CreateUndo("Rotate");
}

void CPictureEditor::MakeDottedSelectionFromMask()
{
		CxImage SaveTo;
		if(m_hasMask)
		{
			LPD3DXBUFFER ppDestBuf;

			D3DXSaveTextureToFileInMemory(
				  &ppDestBuf,
				  D3DXIFF_PNG,
				  display->TextureAt(m_Mask),
				  NULL
			);

			ppDestBuf->GetBufferSize();


			SaveTo.Decode((BYTE*)ppDestBuf->GetBufferPointer()
							   ,ppDestBuf->GetBufferSize(),CXIMAGE_FORMAT_PNG);

			ppDestBuf->Release();
		}
		else
		{
			SaveTo.Create(m_CanvasWidth , m_CanvasHeight, 32);
		}
		SaveTo.SelectionCreate();
		SaveTo.SelectionInvert();

		CreateSelectionFromCxImage(SaveTo);
}

void CPictureEditor::Flip()
{
	SaveDisplayState();
	CanvasToTemp(true);
	display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);

	display->SetRenderTarget(*m_pCanvas);
	display->SetTexture(m_TempCanvas);
	display->Blit(0,m_CanvasHeight, (float)1.0, (float)-1.0);

	if(m_hasMask)
	{
		// and the mask too
		int newmask = display->AddTextureRT(m_RectTempToCanvas.Width(),m_RectTempToCanvas.Height(), GetTextureFormat());
		display->SetTexture(m_Mask);
		display->SetRenderTarget(newmask);
		display->Blit(0,m_RectTempToCanvas.Height(),(float)1.0,(float)-1.0);
			
		display->RemoveTexture(m_Mask);
		m_Mask = newmask;
		MakeDottedSelectionFromMask();
	}

	RestoreDisplayState();

	ClearTemp();
	CreateUndo("Flip");
}

void CPictureEditor::Mirror()
{
	SaveDisplayState();
	CanvasToTemp(true);
	display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);

	display->SetRenderTarget(*m_pCanvas);
	display->SetTexture(m_TempCanvas);
	display->Blit(m_CanvasWidth,0, (float)-1.0, (float)1.0);

	if(m_hasMask)
	{
		// and the mask too
		int newmask = display->AddTextureRT(m_RectTempToCanvas.Width(),m_RectTempToCanvas.Height(), GetTextureFormat());
		display->SetTexture(m_Mask);
		display->SetRenderTarget(newmask);
		display->Blit(m_RectTempToCanvas.Width(),0,(float)-1.0,(float)1.0);
			
		display->RemoveTexture(m_Mask);
		m_Mask = newmask;
		MakeDottedSelectionFromMask();
	}

	RestoreDisplayState();

	ClearTemp();
	CreateUndo("Mirror");
}
void CPictureEditor::Load(CxImage* loadfrom)
{
	CPictureEditor* PicEd = this;


	

	PicEd->m_Width = PicEd->m_CanvasWidth = loadfrom->GetWidth();
	PicEd->m_Height = PicEd->m_CanvasHeight = loadfrom->GetHeight();


	for(int a = 0;a < PicEd->layers.size();a ++)
	{
		PicEd->display->ResizeTextureRT(
		PicEd->layers.at(a),  PicEd->m_Width,PicEd->m_Height
		);
	}
	PicEd->CanvasToTemp();


PicEd->SaveDisplayState();

	int tempimage = PicEd->display->AddTextureFromCxImage(*loadfrom);
	

	PicEd->display->SetRenderTarget(*PicEd->m_pCanvas);

	PicEd->display->SetTexture(tempimage);
	PicEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);

	PicEd->display->Blit(0,0);
	PicEd->display->SetTexture(-1);

	PicEd->display->RemoveTexture(tempimage);
	
	PicEd->RestoreDisplayState();

	PicEd->CanvasToTemp();





	









}

CRect CropImage(CxImage& picture, bool border)
{
	int w = picture.GetWidth();
	int h = picture.GetHeight();
	CRect crop(-1,-1,w,h);
	// left)
	for(int x = 0; x < w; x++)
	{
		for(int y = 0; y < h; y++)
		{
			if(picture.GetPixelColor(x,y, true).rgbReserved != 0)
			{
				crop.left = x-1;
				x = w;
				y = h;
			}
		}
	}

	// right
	for(int x = w-1; x >= 0; x--)
	{
		for(int y = 0; y < h; y++)
		{
			if(picture.GetPixelColor(x,y, true).rgbReserved != 0)
			{
				crop.right = x+1;
				x = -1;
				y = h;
			}
		}
	}

	// top
	for(int y = 0; y < h; y++)
	{
		for(int x = 0; x < w; x++)
		{
			if(picture.GetPixelColor(x,y, true).rgbReserved != 0)
			{
				crop.top = y-1;
				x = w;
				y = h;
			}
		}
	}

	// bottom
	for(int y = h-1; y >= 0; y--)
	{
		for(int x = 0; x < w; x++)
		{
			if(picture.GetPixelColor(x,y, true).rgbReserved != 0)
			{
				crop.bottom = y+1;
				x = w;
				y = -1;
			}
		}
	}

	crop.left ++;

	if(border)
		crop.InflateRect(1,1,1,1);

	crop.top = picture.GetHeight() - crop.top - 1;
	crop.bottom = picture.GetHeight() - crop.bottom - 1;
	crop.bottom ++;



	picture.Crop(crop, &picture);

	return crop;
}
void CPictureEditor::OnEditCrop()
{
	if(*m_pCanvas != *m_pLayer)	//crop to the selection
	{

		m_Width = m_CanvasWidth = m_RectTempToCanvas.Width();
		m_Height = m_CanvasHeight = m_RectTempToCanvas.Height();


		for(int a = 0; a < layers.size(); a++)
		{
			display->ResizeTextureRT(layers.at(a), m_Width,m_Height	);
			display->SetRenderTarget(layers.at(a));
			display->ClearRT();
		}
		
		CanvasToTemp();
	
		m_RectTempToCanvas.OffsetRect( 0 - m_RectTempToCanvas.left, 0 - m_RectTempToCanvas.top);
		DropSelection();
	}
	else
	{
		CxImage& picture = m_undo.back()->src;
		CRect crop = CropImage(picture, (GetKeyState(VK_SHIFT)>>4));
		//Save(&picture);

		CPoint* hotspot = GetHotspot();
		if(hotspot)
		{
			hotspot->x -= crop.left;
			hotspot->y -= crop.bottom;
		}

		map<CString, CPoint>* actions = GetAction();
		if(actions)
		{
			for( map<CString, CPoint>::iterator it = actions->begin(); it != actions->end(); it++)
			{
				CPoint& action = it->second;
				action.x -= crop.left;
				action.y -= crop.bottom;
			}
		}
		
		// We need to crop each side. 
		
		if(GetKeyState(VK_MENU) >> 4)
		{
			this->m_pImageEditor->m_pImgEdDlg->CropAll();
		}

		Load(&picture);
	}
	ClearTemp();
	CreateUndo("Crop");
}




void CPictureEditor::UpdateStatus()
{
	CString ZoomText;
	ZoomText.Format("%d%%", (int)(this->m_Zoom*100));
	this->m_pImageEditor->m_pImgEdDlg->m_pStatusBar->SetPaneText(1, ZoomText);



	CString status;
	status.Format("{%d, %d} , {%d, %d} = [%d, %d]", m_StatusCoordStart.x,
		 m_StatusCoordStart.y, m_StatusCoordEnd.x, m_StatusCoordEnd.y,
		 abs(m_StatusCoordStart.x - m_StatusCoordEnd.x), abs(m_StatusCoordStart.y - m_StatusCoordEnd.y) );

	this->m_pImageEditor->m_pImgEdDlg->m_pStatusBar->SetPaneText(2, status);

}

void CPictureEditor::OnKeyDown(long key)
{

	CurrentTool->OnKeyDown(key);

}

void CPictureEditor::OnLMouseDown(CFloatPoint pt)
{	
	m_MouseX = pt.x;
	m_MouseY = pt.y;

	ScreentoCanvas(pt);
	//TempToCanvas();	// As the canvas already has been previewed	
	
	m_StatusCoordStart.x = pt.x;
	m_StatusCoordStart.y = pt.y;
	
	UpdateStatus();

	if(GetKeyState(VK_CONTROL) >> 2)
	{
		RGBQUAD colour = m_undo.back()->src.GetPixelColor(pt.x, this->m_undo.back()->src.GetHeight()- pt.y);

		RGBQUAD c2 = colour;
		if(c2.rgbReserved != 0)
		{
			c2.rgbRed /= c2.rgbReserved/255.0;
			c2.rgbGreen /= c2.rgbReserved/255.0;
			c2.rgbBlue /= c2.rgbReserved/255.0;
		}
		if(c2.rgbReserved != 0) // that is...alpha is 0 and we cant work out the colour it was before
		{
			*this->m_pImageEditor->m_pColor_Mixer->pColor = RGB(c2.rgbRed, c2.rgbGreen, c2.rgbBlue);
			Colour1 = colour.rgbBlue + (colour.rgbGreen << 8 )+ (colour.rgbRed << 16) + (colour.rgbReserved << 24);
		}
		m_pImageEditor->UpdateTool();
		return;
	}
	
	if(RightMouseDown)
	{
		RightMouseDown = false;
		CurrentTool->m_draw = false;
		CurrentTool->Cancel();

	}
	else
	{
		LeftMouseDown = true;			
		CurrentTool->OnMouseDown(pt);
	}
}
void CPictureEditor::OnRMouseDown(CFloatPoint pt)
{
	m_MouseX = pt.x;
	m_MouseY = pt.y;

	ScreentoCanvas(pt);
	//TempToCanvas();

	m_StatusCoordStart.x = pt.x;
	m_StatusCoordStart.y = pt.y;

	UpdateStatus();

	if(LeftMouseDown)
	{
		LeftMouseDown = false;
		CurrentTool->m_draw = false;
		CurrentTool->Cancel();
	}
	else
	{
		RightMouseDown = true;
		CurrentTool->OnMouseDown(pt);
	}
}
void CPictureEditor::OnMouseMove(CFloatPoint pt)
{
	if (m_pImageEditor->drawingDisabled) {
		m_modded = false;
		m_pImageEditor->m_pImgEdDlg->EndDialog(0);
		return;
	}

	m_MouseX = pt.x;
	m_MouseY = pt.y;
	
	ScreentoCanvas(pt);

	m_StatusCoordEnd.x = pt.x;
	m_StatusCoordEnd.y = pt.y;

	if(!LeftMouseDown && !RightMouseDown)
	{
		m_StatusCoordStart.x = pt.x;
		m_StatusCoordStart.y = pt.y;
	}

	UpdateStatus();
	CurrentTool->OnMouseMove(pt);
}
void CPictureEditor::OnLMouseUp(CFloatPoint pt)
{
	m_MouseX = pt.x;
	m_MouseY = pt.y;

	if(CurrentTool->m_draw)
	{
		ScreentoCanvas(pt);
		CurrentTool->OnMouseUp(pt);
		CreateUndo();
	}
	LeftMouseDown = false;

}
void CPictureEditor::OnLMouseDouble(CFloatPoint pt)
{
	m_MouseX = pt.x;
	m_MouseY = pt.y;
	ScreentoCanvas(pt);
	CurrentTool->OnMouseDouble(pt);
}
void CPictureEditor::OnRMouseDouble(CFloatPoint pt)
{
	m_MouseX = pt.x;
	m_MouseY = pt.y;
	ScreentoCanvas(pt);
	CurrentTool->OnMouseDouble(pt);
}
void CPictureEditor::OnRMouseUp(CFloatPoint pt)
{
	m_MouseX = pt.x;
	m_MouseY = pt.y;

	if(CurrentTool->m_draw)
	{
		ScreentoCanvas(pt);
		CurrentTool->OnMouseUp(pt);
		CreateUndo();
	}
	RightMouseDown = false;
}
void CPictureEditor::TempToCanvas(bool replace)
{
	SaveDisplayState();

	if(replace)
	{
		display->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		display->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	}
		
	display->SetRenderTarget(*m_pCanvas);
	display->SetTexture(m_TempCanvas);
	display->Blit(0,0);
	RestoreDisplayState();	

}
void CPictureEditor::CanvasToTemp(bool replace)
{
	// If our temp canvas is the wrong size...change it
	if(    m_CanvasWidth  != m_TempCanvasWidth
		|| m_CanvasHeight != m_TempCanvasHeight )
	{
		if(m_TempCanvas!= -2)
			display->RemoveTexture(m_TempCanvas);
		m_TempCanvas = display->AddTextureRT(m_CanvasWidth,m_CanvasHeight, GetTextureFormat());
		m_TempCanvasWidth	= m_CanvasWidth;
		m_TempCanvasHeight	= m_CanvasHeight;
	}
	display->SetRenderTarget(-1);
	display->SetTexture(-1);
	SaveDisplayState();
	{
		if(replace)
		{
			display->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			display->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		}
		display->SetRenderTarget(m_TempCanvas);
		display->SetTexture(*m_pCanvas);

		display->Blit(0,0);

	}
	RestoreDisplayState();	

}


CSize CPictureEditor::GetPositionOffset()
{
	CRect WindowRect;
	GetWindowRect(m_pImageEditor->GetSafeHwnd(),WindowRect);

	float OffsetX = 0-m_ScrollX;
	float OffsetY = 0-m_ScrollY;
	int BlitWidth = m_Width * m_Zoom;
	int BlitHeight = m_Height * m_Zoom;
	if(WindowRect.Width() > BlitWidth)
		OffsetX += (WindowRect.Width()- BlitWidth)/2.0;
	if(WindowRect.Height() > BlitHeight)
		OffsetY += (WindowRect.Height()- BlitHeight)/2.0;

	if(displayOffsetEnabled)
	{
		if(WindowRect.Width() <= BlitWidth)
			OffsetX += (WindowRect.Width()- BlitWidth)/2.0;
		if(WindowRect.Height() <= BlitHeight)
			OffsetY += (WindowRect.Height()- BlitHeight)/2.0;
	
		//OffsetX += ((int)(floor(m_Width/2.0) - displayOffsetX ) * m_Zoom);
		//OffsetY += ((int)(floor(m_Height/2.0) - displayOffsetY ) * m_Zoom);
		
		OffsetX += m_Width/2.0  * m_Zoom;
		OffsetY += m_Height/2.0 * m_Zoom;
	
		OffsetX -= displayOffsetX * m_Zoom;
		OffsetY -= displayOffsetY * m_Zoom;


	}

	return CSize(OffsetX,OffsetY);


}


void CPictureEditor::Promote(CRect rc)
{
	m_CanvasWidth  = rc.Width();
	m_CanvasHeight = rc.Height();
	if(m_CanvasPromoted!= -2)
		display->RemoveTexture(m_CanvasPromoted);
	m_CanvasPromoted = display->AddTextureRT(m_CanvasWidth,m_CanvasHeight, GetTextureFormat());


	m_pCanvas = &m_CanvasPromoted;
	
	m_RectTempToCanvas = rc;
}

void CPictureEditor::SaveDisplayState()
{
	CDisplayState state;
	state.colourwrite = display->GetRenderState(D3DRS_COLORWRITEENABLE);
	state.destblend = display->GetRenderState(D3DRS_DESTBLEND);
	state.srcblend = display->GetRenderState(D3DRS_SRCBLEND);
	state.rendertarget = display->GetRenderTarget();
	state.currenttexture = display->GetTexture();

	m_displayState.push_back(state);
}

void CPictureEditor::RestoreDisplayState()
{
	CDisplayState& state = m_displayState.at(m_displayState.size()-1);
	display->SetRenderState(D3DRS_COLORWRITEENABLE, state.colourwrite );
	display->SetRenderState(D3DRS_DESTBLEND, state.destblend );
	display->SetRenderState(D3DRS_SRCBLEND, state.srcblend );
	display->SetRenderTarget(state.rendertarget);
	display->SetTexture(state.currenttexture);
	m_displayState.pop_back();
}

void CPictureEditor::DeleteSelection()
{
	if(m_CanvasPromoted!= -2)
		display->RemoveTexture(m_CanvasPromoted);
	if(m_Mask != -2)
		display->RemoveTexture(m_Mask);
	m_Mask = -2;
	m_CanvasPromoted = -2;
	m_pCanvas = m_pLayer;


	m_RectTempToCanvas.SetRect(0,0,m_Width,m_Height);
	m_CanvasWidth = m_Width;
	m_CanvasHeight = m_Height;

	m_hasMask = false;

	display->SetRenderTarget(*m_pLayer);
	display->SetTexture(-1);
	m_Sel2.clear(); // removed dotted line coz its unimportant

	ClearTemp();

	CreateUndo("Delete Selection");
}

void CPictureEditor::CheckerBackground()
{


	// I dont know why but sometimes I get
	// a border on the bottom right...but increasing the 
	// size to +32 on the width and height seems to fix it
	//if(display->batching)
		display->EndBatch();
	CRect WindowRect;
	GetWindowRect(m_pImageEditor->GetSafeHwnd(),WindowRect);

	Quadrilateral q;
	q.p1.x = 0;						q.p1.y = 0;
	q.p2.x = WindowRect.Width();	q.p2.y = 0;
	q.p3.x = 0;						q.p3.y = WindowRect.Height();
	q.p4.x = WindowRect.Width();	q.p4.y = WindowRect.Height();

	float PatX = WindowRect.Width()/16.0;
	float PatY = WindowRect.Height()/16.0;
	// Render the pattern background
	{

		RECTF rec;
		rec.left = 0;
		rec.top = 0;
		rec.right = PatX;
		rec.bottom = PatY;
		if(BackInvert)
			display->BlitTex(q,0xff444444,&rec);	
		else
			display->BlitTex(q,-1,&rec);

	}

}


void CPictureEditor::DrawSelection()
{


	if (m_Sel2.size()>2)
	{


		for(int i=0;i<m_Sel2.size();i++)
		{
			
			switch (m_Sel2[i].type)
			{
			case 1:	
			DrawDottedLine((int)(m_Sel2[i].x +1),
				    (int)(m_Sel2[i].y ),
					(int)(m_Sel2[i].x ),
					(int)(m_Sel2[i].y ));
				break;
			case 2:
			DrawDottedLine((int)(m_Sel2[i].x +1),
				    (int)(m_Sel2[i].y),
					(int)(m_Sel2[i].x  + 1),
					(int)(m_Sel2[i].y  + 1));
				break;
			case 3:
			DrawDottedLine((int)(m_Sel2[i].x +1),
				    (int)(m_Sel2[i].y + 1),
					(int)(m_Sel2[i].x ),
					(int)(m_Sel2[i].y + 1));
				break;
			case 4:
			DrawDottedLine((int)(m_Sel2[i].x ),
				    (int)(m_Sel2[i].y ),
					(int)(m_Sel2[i].x ),
					(int)(m_Sel2[i].y + 1));
				break;

			}

		}
	}


}

void CPictureEditor::ClearTemp()
{
// If our temp canvas is the wrong size...change it
	if(    m_CanvasWidth  != m_TempCanvasWidth
		|| m_CanvasHeight != m_TempCanvasHeight )
	{
		bool rendertarget_affected = display->GetRenderTarget() == m_TempCanvas ? true : false;
		if(m_TempCanvas != -2)
			display->RemoveTexture(m_TempCanvas);
		m_TempCanvas = display->AddTextureRT(m_CanvasWidth,m_CanvasHeight, GetTextureFormat());

		display->SetRenderTarget(m_TempCanvas);
		m_TempCanvasWidth	= m_CanvasWidth;
		m_TempCanvasHeight	= m_CanvasHeight;
	}
	SaveDisplayState();
	display->SetRenderTarget(m_TempCanvas);
	display->ClearRT(0);
	RestoreDisplayState();



}

void CPictureEditor::DrawDottedLine(int x, int y, int endx, int endy)
{
	float xf = 1.0;
	float yf = 1.0;
	if(m_pLayer != m_pCanvas) // is selection
	{
		xf = m_RectTempToCanvas.Width() / (m_CanvasWidth+0.0);
		yf = m_RectTempToCanvas.Height() / (m_CanvasHeight+0.0);


	}

	// Convert CFloatPoint!
	CFloatPoint pt(0,0);
	pt.x = x*xf;
	pt.y = y*yf;
	CanvastoScreen(pt);
	int oldx = pt.x;
	int oldy = pt.y;

	pt.x = endx*xf;
	pt.y = endy*yf;
	CanvastoScreen(pt);
	int localx = pt.x;
	int localy = pt.y;

//#define DOTCOLOUR 0xff000000

#define SELCOLOUR (  0xff000000 + (0x00ffffff)* (((10000 - lx - ly + clock()/50)%6)/3)       )
	if( abs(localx - oldx) > abs(localy - oldy))
	{	
		int step =(localx - oldx > 0)?1:-1;
		bool draw = false;
		for(int x = oldx; x != localx; x+= step)
		{
			int lx = x;
			int ly = oldy + (oldy - localy) * (x-oldx)/( oldx-localx+0.0);
			display->Point(lx,ly,SELCOLOUR);
		}
	}	
	else
	{	
		int step =(localy - oldy > 0)?1:-1;
		bool draw = false;
		for(int y = oldy; y != localy; y+= step)
		{
			int lx = oldx + (oldx - localx) * (y-oldy)/( oldy-localy+0.0);
			int ly = y;
			display->Point(lx,ly,SELCOLOUR);
		}
	}	



}

void CPictureEditor::DropSelection()
{

	display->SetRenderTarget(*m_pLayer);
	display->SetTexture(-1);
	m_Sel2.clear(); // removed dotted line coz its unimportant

	if(m_pLayer == m_pCanvas)
		return; // We haven't ACTUALLY got a selection?!
	
	SaveDisplayState();

	display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);

	display->SetTexture(*m_pCanvas);
	display->Blit(m_RectTempToCanvas.left,m_RectTempToCanvas.top,m_RectTempToCanvas.Width(),m_RectTempToCanvas.Height());

	if(m_CanvasPromoted!= -2)
		display->RemoveTexture(m_CanvasPromoted);
	if(m_Mask!= -2)
		display->RemoveTexture(m_Mask);
	m_Mask = -2;
	m_CanvasPromoted = -2;
	m_pCanvas = m_pLayer;
	RestoreDisplayState();

	m_RectTempToCanvas.SetRect(0,0,m_Width,m_Height);
	m_CanvasWidth = m_Width;
	m_CanvasHeight = m_Height;

	m_hasMask = false;

	ClearTemp();

	CreateUndo("Drop Selection");

}
void CPictureEditor::OnEditNew()
{
	SaveDisplayState();

	for(int i = 0; i != layers.size(); i++)
	{
		display->SetRenderTarget(layers.at(i));
		display->ClearRT();
	}

	RestoreDisplayState();
	CreateUndo("Clear");
}
void CPictureEditor::OnEditCut()
{


}

void CPictureEditor::OnEditCopy()
{
	CxImage SaveTo;

	LPD3DXBUFFER ppDestBuf;

	display->EndBatch();
	D3DXSaveTextureToFileInMemory(
	  &ppDestBuf,
	  D3DXIFF_PNG,
	  display->TextureAt(*m_pCanvas),
	  NULL
	);

	ppDestBuf->GetBufferSize();


	SaveTo.Decode((BYTE*)ppDestBuf->GetBufferPointer()
				   ,ppDestBuf->GetBufferSize(),CXIMAGE_FORMAT_PNG);

	
	

	CDragImage CopyImage;
	CopyImage.pImage = &SaveTo; 

	DROPEFFECT de = DROPEFFECT_NONE;
	ddm.PrepareDrop(DO_CLIPBOARD,"PNG",&CopyImage,&de);	

	HANDLE hDIB=SaveTo.CopyToHandle();

	if (::OpenClipboard(AfxGetApp()->m_pMainWnd->GetSafeHwnd())) {
		if (::SetClipboardData(CF_DIB,hDIB) == NULL ) {
			AfxMessageBox( "Unable to set Clipboard data" );
		}
	}
	CloseClipboard();


}

void CPictureEditor::OnEditSelectAll()
{
	// This is a hack because the functions I need are 
	// located in the rectangle tool.
	
	DropSelection(); // incase there is already a selection
	
	//POINT meh;
		
	CSelectFreeTool Selection;

	Selection.PicEd = this;

	Selection.AddSelectionPoint(CFloatPoint(0,0));
	Selection.AddSelectionPoint(CFloatPoint(m_CanvasWidth,0));
	Selection.AddSelectionPoint(CFloatPoint(m_CanvasWidth,m_CanvasHeight));
	Selection.AddSelectionPoint(CFloatPoint(0,m_CanvasHeight));
		
	Selection.OnMouseUp(CFloatPoint(0,0));

}

void CPictureEditor::OnEditPaste()
{
	DropSelection();
	CreateUndo("Paste");

	CxImage curImage;
	CxImage clipboard;

	CDragImage CopyImage;
	CopyImage.pImage = &curImage; 
	if(ddm.DoDrop(&CopyImage, NULL, "PNG"))
	{



	}
	else
	{
		HANDLE hBitmap=NULL;

		CBitmap bmpClip;
		if (OpenClipboard(0))
			hBitmap=GetClipboardData(CF_DIB);
		if (hBitmap)
		{
			curImage.CreateFromHANDLE(hBitmap);
		}
		CloseClipboard();
	}
			
	if(curImage.IsValid())
	{
		SaveDisplayState();

		clipboard = curImage; //copy
		CPictureEditor* PicEd = this;
		curImage.SelectionCreate();
		curImage.SelectionInvert();
		CRect r = PicEd->CreateSelectionFromCxImage(curImage);



		if(PicEd->m_Mask!= -2)
			PicEd->display->RemoveTexture(PicEd->m_Mask);
		PicEd->m_Mask = -2;
		
		int clipboardtexture = PicEd->display->AddTextureFromCxImage(clipboard);

		int canvas = *PicEd->m_pCanvas;
		PicEd->Promote(r); // Create our canvas to promote to


		PicEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
		PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);
	
		// Stick whats on the canvas onto our promoted layer
		PicEd->display->SetTexture(clipboardtexture);
		PicEd->display->SetRenderTarget(*PicEd->m_pCanvas);
		PicEd->display->Blit(0,0);

		curImage.Destroy();
		PicEd->display->RemoveTexture(clipboardtexture);
		clipboard.Destroy();

		PicEd->RestoreDisplayState();
		ClearTemp();
	}
}








/////////////////////////////////////////////////////////////
//////
//////             Tools
//////------------------------------------------------------

//
//   Base Class
//



void CTool::OnMouseDown(CFloatPoint pt)
{

	m_draw = true;

	FirstMouseCoords = pt;
	MouseCoords = pt;
	MouseSteps = 0;	
}
void CTool::OnMouseUp(CFloatPoint pt)
{
	PicEd->SaveDisplayState();
	PicEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
	PicEd->TempToCanvas(false);
	PicEd->RestoreDisplayState();
	m_draw = false;
	FirstMouseCoords = pt;
}
void CTool::OnMouseDouble(CFloatPoint pt)
{


}

void CTool::Render()
{
	// Base class doesn't do anything
}


void CTool::OnMouseMove(CFloatPoint pt)
{
	PicEd->ScreentoCanvas(pt);
}


void CTool::Initialise(ToolProperties* props)
{
	maxsteps = props->Step;
	MouseSteps = 0;
	m_pProp = props;
	m_draw = false;

	PicEd->ClearTemp();

}
void CTool::Proportion()
{
	if(GetKeyState(VK_SHIFT)>>4)
	{
		int average =( abs(FirstMouseCoords.x - MouseCoords.x) + abs(FirstMouseCoords.y - MouseCoords.y))/2;
		if(MouseCoords.x < FirstMouseCoords.x)
			MouseCoords.x = FirstMouseCoords.x - average;
		else
			MouseCoords.x = FirstMouseCoords.x + average;
		if(MouseCoords.y < FirstMouseCoords.y)
			MouseCoords.y = FirstMouseCoords.y - average;
		else
			MouseCoords.y = FirstMouseCoords.y + average;
	}


}
void CTool::PostRender()
{

}

void CTool::Cancel()
{

}
COLORREF CTool::GetColor()
{
	if(PicEd->RightMouseDown)
		return PicEd->Colour2;
	else
		return PicEd->Colour1;
}
COLORREF CTool::GetColor2()
{
	if(PicEd->RightMouseDown)
		return PicEd->Colour1;
	else
		return PicEd->Colour2;
}
void CTool::OverlayCallback()
{
	// Find size of window
	CSize Offset = PicEd->GetPositionOffset();

	int BlitWidth = PicEd->m_Width * PicEd->m_Zoom;
	int BlitHeight = PicEd->m_Height * PicEd->m_Zoom;


	//if(m_draw)
	{


		
		// If our temp canvas is the wrong size...change it
		if(    PicEd->m_CanvasWidth  != PicEd->m_DisplayStepWidth
			|| PicEd->m_CanvasHeight != PicEd->m_DisplayStepHeight )
		{
			if(PicEd->m_DisplayStep != -2)
				PicEd->display->RemoveTexture(PicEd->m_DisplayStep);
			PicEd->m_DisplayStep = PicEd->display->AddTextureRT(PicEd->m_CanvasWidth,PicEd->m_CanvasHeight, PicEd->GetTextureFormat());
			PicEd->m_DisplayStepWidth	= PicEd->m_CanvasWidth;
			PicEd->m_DisplayStepHeight	= PicEd->m_CanvasHeight;
		}


		int texturetodrawwith = PicEd->m_DisplayStep;


		PicEd->display->SetTexture(-1);
		
		PicEd->display->SetRenderTarget(PicEd->m_DisplayStep);
		PicEd->display->ClearRT(0);
		PicEd->display->SetRenderTarget(PicEd->m_DisplayStep);
		PicEd->display->SetTexture(*PicEd->m_pCanvas);
		PicEd->display->Blit(0,0);
		PicEd->SaveDisplayState();

		OverlayCallbackBegin();
		PicEd->display->SetTexture(PicEd->m_TempCanvas);
		PicEd->display->Blit(0,0);
		OverlayCallbackEnd();

		if(PicEd->m_pLayer != PicEd->m_pCanvas)	// Drawing onto the layer
		{
			if(PicEd->m_Mask != -2)	// this prevents us from seeing stuff that we are drawing outside the selection
			{
				PicEd->display->SetTexture(PicEd->m_Mask);
				//PicEd->display->SetRenderTarget(*PicEd->m_pCanvas);
				PicEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ZERO);
				PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_SRCALPHA);
				PicEd->display->Blit(0,0);
			}
		}

		
		PicEd->RestoreDisplayState();
		PicEd->display->SetRenderTarget(-1);
		
		PicEd->display->SetTexture(PicEd->m_TempCanvas);
		//PicEd->display->Blit(Offset.cx,Offset.cy,BlitWidth,BlitHeight);




		if(PicEd->m_pLayer == PicEd->m_pCanvas)	// Drawing onto the layer
		{
			PicEd->display->SetTexture(texturetodrawwith);
			PicEd->display->Blit(Offset.cx,Offset.cy,BlitWidth,BlitHeight);
		}
		else
		{
			PicEd->display->SetTexture(texturetodrawwith);
			PicEd->display->Blit(
						Offset.cx+PicEd->m_RectTempToCanvas.left*PicEd->m_Zoom,
						Offset.cy+PicEd->m_RectTempToCanvas.top*PicEd->m_Zoom,
						(int)(PicEd->m_RectTempToCanvas.Width()* PicEd->m_Zoom),
						(int)(PicEd->m_RectTempToCanvas.Height()* PicEd->m_Zoom));

		}
	}
	//else
	//{
	//	PicEd->display->Blit(Offset.cx,Offset.cy,BlitWidth,BlitHeight);
	//}

}

////////////////////////////
////
////   Brush Tool
////

void CBrushTool::DrawLine(float x, float y, float endx, float endy, float& MouseSteps,float& maxsteps)
{
	if (maxsteps < 1) maxsteps = 1;
	if (MouseSteps < 0) MouseSteps = 0;


	//MouseSteps = 9;
	float localx = endx;
	float localy = endy;
	float oldx = x;
	float oldy = y;

	//PenMark(x,y, 1, 1);


	float distance = 0;
	if(!m_pProp->bSmooth)
		distance = max(abs(localx-oldx),abs(localy-oldy));
	else
		distance = sqrt(pow(localx-oldx,2) + pow(localy - oldy,2));
	
	float rounded_distance = max(0,floor((maxsteps - MouseSteps + distance) / maxsteps) * maxsteps);
	int stepcount = rounded_distance / maxsteps;
	

	for(int s = 0; s < stepcount; s++)
	{
		double r = 0;
		if(distance != 0)
			r = (s * maxsteps + MouseSteps)/distance;
		double xx = localx * r + oldx * (1-r);
		double yy = localy * r + oldy * (1-r);
		PenMark(xx,yy, 1, 1);
	}

	MouseSteps -= distance;
	MouseSteps = (MouseSteps - (floor(MouseSteps/maxsteps)*maxsteps));

	return;


	//PORT: HOLY BALONEY HOW MANY TIMES DO YOU WANT TO DEFINE x AND y?
	// I've made changes to conform to for scoping
	if(abs(localx - oldx) > abs(localy - oldy))
	{	
		int step =(localx - oldx > 0)?1:-1;
		bool draw = false;

		//PORT: naming ambiguity, changing index 'x' (already defined) to 'a'
		float a;
		for(a = oldx; (int)a != (int)localx; a += step)
		{

			//PenMark(x+0.5,oldy + (oldy - localy) * (x-oldx)/( oldx-localx+0.0)+0.5,1,1);
			if((int)MouseSteps == 0)
			{
				if(draw)
					// update: congratulations, u caused a major bug removin the 0.0...it converts it to float...GOSH
					PenMark(a, oldy + (oldy - localy) * (a - oldx) / (oldx - localx+0.0), 1, 1);

				MouseSteps += maxsteps;				
			}
			MouseSteps --;
			draw = true;
		}
		//PORT: this is very ambiguous usage of the 'x' variable - assuming you meant the loop
		// index above due to similary to for body, changing 'x' to 'a'
		if((int)MouseSteps == 0)
		{
			PenMark(a, oldy + (oldy - localy) * (a - oldx) / (oldx - localx+0.0), 1, 1);
			MouseSteps += maxsteps;
		}
	}	
	else
	{	
		int step =(localy - oldy > 0)?1:-1;
		bool draw = false;

		//PORT: naming ambiguity, changing y (already defined) to 'b'
		float b;
		for(b = oldy; (int)b != (int)localy; b += step)
		{
		//	PenMark(oldx + (oldx - localx) * (y-oldy)/( oldy-localy+0.0)+0.5,y+0.5,1,1);
			if((int)MouseSteps == 0)
			{
				if(draw)
					//PORT: removed +0.0, had no effect
					PenMark(oldx + (oldx - localx) * (b - oldy) / (oldy - localy+0.0), b, 1, 1);

				MouseSteps += maxsteps;
			}
			MouseSteps --;
			draw = true;
		}
		//PORT: this is very ambiguous usage of 'y': assuming loop index intended
		// Renaming 'y' to 'b'
		if((int)MouseSteps == 0)
		{
			PenMark(oldx + (oldx - localx) * (b - oldy) / (oldy - localy+0.0), b, 1, 1);
			MouseSteps += maxsteps;
		}
	}	


}



void CBrushTool::OnMouseMove(CFloatPoint pt)
{
	if(!m_pProp->bSmooth)
	{
		pt.x = floor(pt.x)+0.5;
		pt.y = floor(pt.y)+0.5;
	}

	if(MouseCoords.x == pt.x && MouseCoords.y == pt.y )
		return; // ie. the mouse hasn't moved :P

	float oldx = MouseCoords.x;
	float oldy = MouseCoords.y;
	MouseCoords = pt;
	float localx = pt.x;
	float localy = pt.y;


	if(m_draw)
	{
		PicEd->SaveDisplayState();

		PicEd->display->SetRenderTarget(PicEd->m_TempCanvas);
		PicEd->display->SetRenderState(D3DRS_SRCBLEND ,  D3DBLEND_ONE);
		PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);
		DrawLine(oldx,oldy,localx,localy,MouseSteps,maxsteps);

		PicEd->RestoreDisplayState();
	}
	else
		m_Preview = true;

}
void CBrushTool::OnMouseDown(CFloatPoint pt)
{
	if(!m_pProp->bSmooth)
	{
		pt.x = floor(pt.x)+0.5;
		pt.y = floor(pt.y)+0.5;
	}

	if(PicEd->LeftMouseDown)
		m_Brush = m_pProp->Brush1;
	else
		m_Brush = m_pProp->Brush2;

	PicEd->ClearTemp();

	m_draw = true;

	if(!(GetKeyState(VK_SHIFT) >> 4))
	{
		FirstMouseCoords = pt;
		MouseCoords = pt;
		MouseSteps = 0;
	}
	else
	{
		MouseCoords = FirstMouseCoords;
	}
		
	//The drawline function doesn't draw unless
	// the mouse has moved...even if its a tiny tiny tiny
	// bit...so thats what we do :P. 
	MouseCoords.x -= 0.0001;

	OnMouseMove(pt);
/*

	
	PicEd->SaveDisplayState();
	PicEd->display->SetRenderTarget(PicEd->m_TempCanvas);
	PenMark(pt.x,pt.y,1,1);
	//DrawLine(pt.x,pt.y,pt.x,pt.y+1,MouseSteps,maxsteps);
	PicEd->RestoreDisplayState();
	MouseSteps = 1;*/
	
}
void CBrushTool::OnMouseUp(CFloatPoint pt)
{
	PicEd->SaveDisplayState();
	PicEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
	OverlayCallbackBegin();
	PicEd->TempToCanvas(false);
	OverlayCallbackEnd();
	PicEd->RestoreDisplayState();
	m_draw = false;
	FirstMouseCoords = pt;
}





float brushfunction(float x,float w, float h,float hardness)
{

	x = sqrt(100*x);
	x /= 10;

	float oldx = 1-x;

	if(w>3)
	if(w*x > w+0.5) 
		return 255;

	//Hardness
	if(x >= hardness)
	{
		if(x >= 1)
			x = 1;
		else if(x == hardness)
		{
			x = 0;
		}
		else
		{
			x -= hardness;
			if(hardness > 0.99)
				x = 0;
			else
				x = x / (1-hardness);
			if(x < 0 )
				x = 0;
			if(x > 1 )
				x = 1;
		}
	}
	else 
		x = 0;

	int a = (int)(255*x);

	//x+= rand()%2;
	float fa = 255*x - a;
	float fb = 1-fa;

	//if(rand()%2 == 0)
	//	a ++;
	if(a < 0 )
		a = 0;
	if(a >= 255)
		a = 254;
	x = BrushFilter[a]*fb + BrushFilter[a+1]*fa ;

	return x;
}


void CreateCircle(CD3DDisplay* display,int target, int size, int thickness,float hardness, int angle, D3DCOLOR c)
{

	TextureInfo* Info = display->TexInfo(target);

	int w = Info->imgwidth;
	int h = Info->imgheight;





	CxImage image;
	image.Create( w, h, 32);
	image.AlphaCreate();
	//image.Clear();
	//image.AlphaClear();
	float red = COLOR_RED(c);// (COLOR_ALPHA(c)/255.0);
	float green = COLOR_GREEN(c);//*(COLOR_ALPHA(c)/255.0);
	float blue = COLOR_BLUE(c);//*(COLOR_ALPHA(c)/255.0);
	float alpha = (COLOR_ALPHA(c)/255.0);
	float sina, cosa;
	cr::sincosf(Rad(angle), &sina, &cosa);
	
	if(thickness == 0)
		thickness = 0.01;
	for(int x = 0; x < w; x ++)
	{
		for(int y = 0; y < h; y ++)
		{
			#define POWER2(a) (a)*(a)
			#define HALFW ((w-1)/2.0)
			#define HALFH ((h-1)/2.0)//brushfunction
			float xdist = (x-HALFW)/HALFW;
			float ydist = (y-HALFH)/HALFH;
			//apply thickness
			xdist /= thickness/100.0;

			//apply angle
			float xx = cosa*xdist + sina*ydist;
			float yy = cosa*ydist + sina*xdist;
			
			
			int value = brushfunction(POWER2(xx)+POWER2(yy),w,h,hardness);
			RGBQUAD q;
			q.rgbRed = red;
			q.rgbGreen = green ;
			q.rgbBlue = blue;
			q.rgbReserved = (255-value) * alpha;
			image.SetPixelColor(x,y,q,true);
		}
	}


	//image.Save("C:/image.png", CXIMAGE_FORMAT_PNG);
	int temp = display->AddTextureFromCxImage(image);
	display->SetRenderTarget(target);
	display->SetTexture(temp);
	display->SetRenderState(D3DRS_SRCBLEND ,  D3DBLEND_ONE);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);

/*	display->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
	display->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	display->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	display->Blit(0,0);

	display->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA);
	display->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	display->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);*/

	display->Blit(0,0);

	display->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	display->Blit(0,0);

		// Resume normal render states
	display->SetRenderState(D3DRS_SRCBLEND ,  D3DBLEND_SRCALPHA);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);

	display->SetRenderTarget(-1);
	display->SetTexture(-1);
	display->RemoveTexture(temp);
}



#define CVT_COL(x)  *((COLORREF*)&(x))
void Fill(CxImage* src,  RGBQUAD& c,RGBQUAD& newcol, int x, int y)
{
	src->SetPixelColor(x,y,newcol, true);

	if(x < 0 || y < 0 || x >= src->GetWidth() || y >= src->GetHeight())
		return;

	int lx = x;
	int xstart, xend;

	while(lx--)
	{
		if(CVT_COL(src->GetPixelColor(lx, y)) == CVT_COL(c))
		{
			src->SetPixelColor(lx,y,newcol, true);
		}
		else
			break;
	}
	xstart = lx+1;
	lx = x;
	int width = src->GetWidth();
	while(++lx < width)
	{
		if(CVT_COL(src->GetPixelColor(lx, y)) == CVT_COL(c))
		{
			src->SetPixelColor(lx,y,newcol, true);
		}
		else
			break;
	}
	xend = lx-1;

	for(lx = xstart; lx <= xend; lx++)
	{
		if(CVT_COL(src->GetPixelColor(lx, y-1)) == CVT_COL(c))
			Fill(src,  c, newcol, lx, y-1);	
		if(CVT_COL(src->GetPixelColor(lx, y+1)) == CVT_COL(c))
			Fill(src,  c, newcol, lx, y+1);
	}
}

void FillSelection(CxImage* src,  RGBQUAD& c, int x, int y)
{
	src->SelectionAddPixel(x, src->GetHeight() -1 - y);
	if(x < 0 || y < 0 || x >= src->GetWidth() || y >= src->GetHeight())
		return;

	int lx = x;
	int xstart, xend;

	while(lx--)
	{
		if(CVT_COL(src->GetPixelColor(lx, y)) == CVT_COL(c))
		{
			src->SelectionAddPixel(lx,src->GetHeight() -1 - y);
		}
		else
			break;
	}
	xstart = lx+1;
	lx = x;
	int width = src->GetWidth();
	while(++lx < width)
	{
		if(CVT_COL(src->GetPixelColor(lx, y)) == CVT_COL(c) && !src->SelectionIsInside(lx,src->GetHeight() -1 - y))
		{
			src->SelectionAddPixel(lx, src->GetHeight()-1-y);
		}
		else
			break;
	}
	xend = lx-1;

	for(lx = xstart; lx <= xend; lx++)
	{
		if(CVT_COL(src->GetPixelColor(lx, y-1)) == CVT_COL(c) && !src->SelectionIsInside(lx,src->GetHeight() -1 - y+1))
			FillSelection(src, c, lx, y-1);	
		if(CVT_COL(src->GetPixelColor(lx, y+1)) == CVT_COL(c) && !src->SelectionIsInside(lx,src->GetHeight() -1 - y-1))
			FillSelection(src, c, lx, y+1);
	}
}


void  CBrushTool::OverlayCallbackBegin()
{
	int opacity = (int)m_pProp->Opacity;
	Clamp(opacity, 0, 255);
	D3DCOLOR col = D3D_ARGB(opacity,opacity,opacity,opacity);
	PicEd->display->SetGlobalFilter(col);

}
void  CBrushTool::OverlayCallbackEnd()
{
	PicEd->display->SetGlobalFilter(-1);
}

void CBrushTool::ModifyBrush()
{

	if(m_pProp->Size <= 0)
		m_pProp->Size = 1;
	m_pProp->Brush_Width = m_pProp->Size+2;
	m_pProp->Brush_Height = m_pProp->Size+2;

	//Now its time to create our actual brush
	CD3DDisplay* display = PicEd->display;
	display->SetRenderState(D3DRS_SRCBLEND ,  D3DBLEND_ONE);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);
	display->SetTexture(-1);

	// Left click brush
	if(m_pProp->Brush1 == -2)
		m_pProp->Brush1 = display->AddTextureRT(m_pProp->Brush_Width,m_pProp->Brush_Height);
	else
		display->ResizeTextureRT(m_pProp->Brush1,m_pProp->Brush_Width,m_pProp->Brush_Height);

	// Right click brush
	if(m_pProp->Brush2 == -2)
		m_pProp->Brush2 = display->AddTextureRT(m_pProp->Brush_Width,m_pProp->Brush_Height);
	else
		display->ResizeTextureRT(m_pProp->Brush2,m_pProp->Brush_Width,m_pProp->Brush_Height);


	CreateCircle(display, m_pProp->Brush1, m_pProp->Size, m_pProp->Thickness, m_pProp->Hardness, m_pProp->Angle, PicEd->Colour1);
	CreateCircle(display, m_pProp->Brush2, m_pProp->Size, m_pProp->Thickness, m_pProp->Hardness, m_pProp->Angle, PicEd->Colour2);



	display->SetRenderTarget(RENDERTARGET_DISPLAY);
	display->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_ALPHA);

	// Resume normal render states
	display->SetRenderState(D3DRS_SRCBLEND ,  D3DBLEND_SRCALPHA);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
}

void CBrushTool::Brush(char *filename)
{
	CD3DDisplay* display = PicEd->display;

	m_BrushImage = display->AddTextureFromFile(filename);
	if(m_BrushImage < 0)
		MessageBox(NULL, "Could not load the brush", "Error", MB_OK);
}

void CPictureEditor::Blit(float x, float y, float w, float h, float angle, D3DCOLOR filter, const RECTF* uv)
{
	if(bSeamless)
	{
		for(int xo = -m_CanvasWidth; xo <= m_CanvasWidth; xo += m_CanvasWidth)
			for(int yo = -m_CanvasHeight; yo <= m_CanvasHeight; yo += m_CanvasHeight)
				display->Blit(x+xo,y+yo,w,h,angle,filter,uv);

	}
	else
		display->Blit(x,y,w,h,angle,filter,uv);
}

void CBrushTool::PenMark(float x, float y, float W, float H)
{




	CD3DDisplay* display = PicEd->display;



	float localx = x-0.5;
	float localy = y-0.5;
	PicEd->display->SetTexture(m_Brush);
	display->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
	display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_SRCALPHA);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
	PicEd->Blit(localx-display->TexInfo(m_Brush)->imgwidth/2, localy-display->TexInfo(m_Brush)->imgheight/2, W, H);
	
	display->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA);

	display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
	PicEd->Blit(localx-display->TexInfo(m_Brush)->imgwidth/2, localy-display->TexInfo(m_Brush)->imgheight/2, W, H);
	


	display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
	display->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_ALPHA);


}



void CBrushTool::Render()
{
	if(!m_draw && m_Preview)
	{
		PicEd->ClearTemp();
		int old = m_Brush;
		m_Brush = m_pProp->Brush1;
		PenMark(MouseCoords.x,MouseCoords.y,1,1);
		m_Brush = old;
	}
	
}

////////////////////////////
////
////   Line Tool
////




void CLineTool::OnMouseMove(CFloatPoint pt)
{
	if(!m_pProp->bSmooth)
	{
		pt.x = floor(pt.x)+0.5;
		pt.y = floor(pt.y)+0.5;
	}

	MouseCoords = pt;
}

void CLineTool::Render()
{
	if(m_draw)
	{
		PicEd->ClearTemp();
		int old = m_Brush;
		m_Brush = m_pProp->Brush1;
		//PenMark(FirstMouseCoords.x,FirstMouseCoords.y,1,1);
		float ms = MouseSteps;
		DrawLine(FirstMouseCoords.x, FirstMouseCoords.y, MouseCoords.x,MouseCoords.y,MouseSteps,maxsteps);
		MouseSteps = ms;
		m_Brush = old;
	}
	if(!m_draw)
	{
		PicEd->ClearTemp();
		int old = m_Brush;
		m_Brush = m_pProp->Brush1;
		PenMark(MouseCoords.x,MouseCoords.y,1,1);
		m_Brush = old;
	}
}





////////////////////////////
////
////   Pen Tool
////

void CPenTool::ModifyBrush()
{
	m_pProp->Brush_Width = m_pProp->Size;
	m_pProp->Brush_Height = m_pProp->Size;

	//Now its time to create our actual brush
	CD3DDisplay* display = PicEd->display;
	display->SetRenderState(D3DRS_SRCBLEND ,  D3DBLEND_ONE);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);
	display->SetTexture(-1);

	// Left click brush
	if(!m_pProp->Brush1)
		m_pProp->Brush1 = display->AddTextureRT(1,1);
	else
		display->ResizeTextureRT(m_pProp->Brush1,1,1);

	// Right click brush
	if(!m_pProp->Brush2)
		m_pProp->Brush2 = display->AddTextureRT(1,1);
	else
		display->ResizeTextureRT(m_pProp->Brush2,1,1);


	CreateCircle(display, m_pProp->Brush1, 1, 1, 1, 0, PicEd->Colour1);
	CreateCircle(display, m_pProp->Brush2, 1, 1, 1, 0, PicEd->Colour2);



	display->SetRenderTarget(RENDERTARGET_DISPLAY);
	display->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_ALPHA);

	// Resume normal render states
	display->SetRenderState(D3DRS_SRCBLEND ,  D3DBLEND_SRCALPHA);
	display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
}

///////////////////////////////////
////////////////////////////
//
//   Erasor Tool
//
////////////////////////////
//////////////////////////////////

void CErasorTool::OverlayCallbackBegin()
{
	CBrushTool::OverlayCallbackBegin();
	PicEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ZERO);
	PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
}



//////////////////////
//
//	Selection

void CSelectFreeTool::DrawGrippers()
{
	if(PicEd->m_pLayer != PicEd->m_pCanvas)
	{

		CRect Rects[9];
		GetDragRects(Rects);

#define RECTANGLE(a)	PicEd->display->Fill(a.left,a.top,a.Width(),a.Height(),0xff000000);

		for(int a = 0; a < 8; a ++)

		RECTANGLE(Rects[a]);


		PicEd->RestoreDisplayState();

	}
}





void CSelectFreeTool::PostRender()
{
	PicEd->display->SetTexture(-1);
	int count=0;
	if(m_Sel.size() > 0)
	{
		for(int a = 1; a < m_Sel.size(); a ++)
			DrawDottedLine( m_Sel.at(a-1).x,
							m_Sel.at(a-1).y,
							m_Sel.at(a).x,
							m_Sel.at(a).y,&count);
	}
	// if we actually have a selection...draw grippers around it

	DrawGrippers();
}

void CSelectFreeTool::DrawDottedLine(int x, int y, int endx, int endy,int* count)
{

	// Convert CFloatPoint!
	CFloatPoint pt;
	pt.x = x;
	pt.y = y;
	PicEd->CanvastoScreen(pt);
	int oldx = pt.x +PicEd->m_Zoom/2 - PicEd->m_Zoom*PicEd->m_RectTempToCanvas.left;
	int oldy = pt.y +PicEd->m_Zoom/2 - PicEd->m_Zoom*PicEd->m_RectTempToCanvas.top;

	pt.x = endx;
	pt.y = endy;
	PicEd->CanvastoScreen(pt);
	int localx = pt.x +PicEd->m_Zoom/2 - PicEd->m_Zoom*PicEd->m_RectTempToCanvas.left;
	int localy = pt.y +PicEd->m_Zoom/2 - PicEd->m_Zoom*PicEd->m_RectTempToCanvas.top;


#define DOTCOLOUR (   0xff000000 + (0x00ffffff)*(((*count) % 4)/2)       )

	if( abs(localx - oldx) > abs(localy - oldy))
	{	
		int step =(localx - oldx > 0)?1:-1;
		bool draw = false;
		for(int x = oldx; x != localx; x+= step)
		{
			(*count)++;
			int lx = x;
			int ly = oldy + (oldy - localy) * (x-oldx)/( oldx-localx+0.0);
			PicEd->display->Point(lx,ly,DOTCOLOUR);
		}
	}	
	else
	{	
		int step =(localy - oldy > 0)?1:-1;
		bool draw = false;
		for(int y = oldy; y != localy; y+= step)
		{
			(*count)++;
			int lx = oldx + (oldx - localx) * (y-oldy)/( oldy-localy+0.0);
			int ly = y;
			PicEd->display->Point(lx,ly,DOTCOLOUR);
		}
	}	

}
void CSelectFreeTool::AddSelectionPoint(CFloatPoint pt)
{
	POINT point;
	point.x = pt.x;
	point.y = pt.y;

		if(point.x < 0) point.x = 0;
		if(point.y < 0) point.y = 0;
		if(point.x > PicEd->m_Width-1) point.x = PicEd->m_Width-1;
		if(point.y > PicEd->m_Height-1) point.y = PicEd->m_Height-1;
		if(m_draw 
			&& !m_resizeleft && !m_resizeright && !m_resizetop && !m_resizebottom)
		{
			m_Sel.push_back(point);
			if(m_Sel.size()>1)
				if(m_Sel.at(m_Sel.size()-1).x == m_Sel.at(m_Sel.size()-2).x
				 && m_Sel.at(m_Sel.size()-1).y == m_Sel.at(m_Sel.size()-2).y)
						m_Sel.pop_back();
		}

}


void CSelectFreeTool::ResizeMove(CFloatPoint pt)
{
	POINT point;
	point.x = pt.x;
	point.y = pt.y;

	int Xoffset = point.x - FirstMouseCoords.x;
	int Yoffset = point.y - FirstMouseCoords.y;
	if(m_resizeleft)
		PicEd->m_RectTempToCanvas.left += Xoffset;
	if(m_resizeright)
		PicEd->m_RectTempToCanvas.right += Xoffset;
	if(m_resizetop)
		PicEd->m_RectTempToCanvas.top += Yoffset;
	if(m_resizebottom)
		PicEd->m_RectTempToCanvas.bottom += Yoffset;
	
	FirstMouseCoords = pt;
	FirstMouseCoords.x = (int)FirstMouseCoords.x;
	FirstMouseCoords.y = (int)FirstMouseCoords.y;

}
bool CSelectFreeTool::IsResizing()
{
	return m_resizeleft || m_resizeright || m_resizetop || m_resizebottom;
}


void CSelectFreeTool::OnMouseMove(CFloatPoint pt)
{
	pt.x += PicEd->m_RectTempToCanvas.left;
	pt.y += PicEd->m_RectTempToCanvas.top;

	POINT point;
	point.x = pt.x;
	point.y = pt.y;
	CTool::OnMouseMove(pt);

	if(IsResizing())
	{
		ResizeMove(pt);
	}
	else
	{
		AddSelectionPoint(pt);
	}
}
void CSelectFreeTool::OnMouseDownResize(CFloatPoint pt)
{
	CRect drag[9];
	GetDragRects(drag);

	m_resizeleft = m_resizeright = m_resizetop = m_resizebottom = false;
	if(PicEd->m_pCanvas != PicEd->m_pLayer) // has selection
	{
		CPoint mouse(PicEd->m_MouseX,PicEd->m_MouseY);
		for(int x = 0; x < 9; x ++)
			if(drag[x].PtInRect(mouse))
			{
				
				if(x==0||x==3||x==5||x==8)
					m_resizeleft = true;
				if(x==2||x==4||x==7||x==8)
					m_resizeright = true;
				if(x==0||x==1||x==2||x==8)
					m_resizetop = true;
				if(x==5||x==6||x==7||x==8)
					m_resizebottom = true;
			}

		m_Sel.clear();
				
	}
}
void CSelectFreeTool::OnMouseDown(CFloatPoint pt)
{
	pt.x += PicEd->m_RectTempToCanvas.left;
	pt.y += PicEd->m_RectTempToCanvas.top;

	FirstMouseCoords.x = pt.x;
	FirstMouseCoords.y = pt.y;

	CTool::OnMouseDown(pt);

	OnMouseDownResize(pt);

	if(!IsResizing())
	{
		m_Sel.clear();

		AddSelectionPoint(pt);
	}	
}

bool CSelectFreeTool::CreateSelectionForCxImage(CxImage& curImage)
{
//PORT: cannot convert m_Sel.begin() to POINT*, assuming address of front() intended
	curImage.SelectionAddPolygon(&(m_Sel.front()), m_Sel.size());
	return true;
}

void CSelectFreeTool::OnMouseUpDrawingSelection(CFloatPoint pt)
{



		CTool::OnMouseUp(pt);
		PicEd->m_Sel2.clear();

		PicEd->DropSelection();
		PicEd->CanvasToTemp();

		if(m_Sel.size() > 2)
		{
			

			// Okay! hard part where i'll probably stuff up.
			PicEd->display->SetRenderTarget(*PicEd->m_pLayer);
			PicEd->m_pCanvas = PicEd->m_pLayer; //variables and stuff dont matter at this point
											// as we wont be keeping the canvas as this number
			CxImage curImage = PicEd->m_undo.back()->src;
			//PicEd->Save(&curImage);
			curImage.SelectionClear();
			
			if(!CreateSelectionForCxImage(curImage))
			{
				PicEd->ClearTemp();
				return;
			}



					

			CRect r = PicEd->CreateSelectionFromCxImage(curImage);



			if(PicEd->m_Mask != -2)
				PicEd->display->RemoveTexture(PicEd->m_Mask);
			PicEd->m_Mask = PicEd->display->AddTextureFromCxImage(curImage);

			int canvas = *PicEd->m_pCanvas;
			PicEd->Promote(r); // Create our canvas to promote to
			PicEd->SaveDisplayState();

			PicEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
			PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);
			PicEd->display->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_ALPHA);

			// Stick whats on the canvas onto our promoted layer
			PicEd->display->SetTexture(canvas);
			PicEd->display->SetRenderTarget(*PicEd->m_pCanvas);
			PicEd->display->Blit(0-r.left,0-r.top);

			// Cut off whats under the promoted layer
			PicEd->display->SetTexture(PicEd->m_Mask);
			PicEd->display->SetRenderTarget(canvas);
			PicEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ZERO);
			PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
			PicEd->display->Blit(r.left,r.top);

			PicEd->RestoreDisplayState();
			PicEd->m_hasMask = true;
			curImage.Destroy();




		}

		PicEd->ClearTemp();

}

void CSelectFreeTool::FinishResizing(CFloatPoint pt)
{
		// finished resizing
	if(m_resizeleft | m_resizeright | m_resizetop | m_resizebottom)
	{
		m_Sel.clear();
		if( m_resizeleft && m_resizeright)
		{
			m_resizeleft = m_resizeright = m_resizetop = m_resizebottom = false;
			return;
		}
		if( m_resizetop && m_resizebottom)
		{
			m_resizeleft = m_resizeright = m_resizetop = m_resizebottom = false;
			return;
		}
		
		m_resizeleft = m_resizeright = m_resizetop = m_resizebottom = false;
		if(PicEd->m_pCanvas != PicEd->m_pLayer)
		{
			PicEd->SaveDisplayState();
			int newcanvas = PicEd->display->AddTextureRT(
				PicEd->m_RectTempToCanvas.Width(),PicEd->m_RectTempToCanvas.Height(), PicEd->GetTextureFormat());

			

			PicEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
			PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);
			PicEd->display->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_ALPHA);

			// Stick whats on the canvas onto our promoted layer
			PicEd->display->SetTexture(*PicEd->m_pCanvas);
			PicEd->display->SetRenderTarget(newcanvas);
			PicEd->display->Blit(0,0,PicEd->m_RectTempToCanvas.Width(),PicEd->m_RectTempToCanvas.Height());

			// and the mask too
			if(PicEd->m_hasMask)
			{
				int newmask = PicEd->display->AddTextureRT(
				PicEd->m_RectTempToCanvas.Width(),PicEd->m_RectTempToCanvas.Height(), PicEd->GetTextureFormat());
				PicEd->display->SetTexture(PicEd->m_Mask);
				PicEd->display->SetRenderTarget(newmask);
				PicEd->display->Blit(0,0,PicEd->m_RectTempToCanvas.Width(),PicEd->m_RectTempToCanvas.Height());
				
				// Shit...everytime we resized a selection the old mask wasn't freed!
				PicEd->display->RemoveTexture(PicEd->m_Mask);

				PicEd->m_Mask = newmask;
			}


			PicEd->RestoreDisplayState();

			PicEd->display->RemoveTexture(*PicEd->m_pCanvas);
			if(PicEd->display->GetRenderTarget() == *PicEd->m_pCanvas)
				PicEd->display->SetRenderTarget(newcanvas);
			if(PicEd->display->GetTexture() == *PicEd->m_pCanvas)
				PicEd->display->SetTexture(newcanvas);
			
			*PicEd->m_pCanvas = newcanvas;

			PicEd->m_CanvasWidth = PicEd->m_RectTempToCanvas.Width();
			PicEd->m_CanvasHeight = PicEd->m_RectTempToCanvas.Height();

			// Now update the dotted lines.
			PicEd->display->EndBatch();


			PicEd->MakeDottedSelectionFromMask();



		}
	}
}

void CSelectFreeTool::OnMouseUp(CFloatPoint pt)
{
	if( m_resizeleft || m_resizeright || m_resizetop || m_resizebottom)
	{
		FinishResizing(pt);
		//m_RectTempToCanvas is a bounding rectangle for the selection
	}
	else
	{
		if(m_Sel.size() != 0)
		{
			pt.x+=PicEd->m_RectTempToCanvas.left;
			pt.y+=PicEd->m_RectTempToCanvas.top;
			AddSelectionPoint(pt);
			pt.x = m_Sel.at(0).x;
			pt.y = m_Sel.at(0).y;
			AddSelectionPoint(pt);
		}
		OnMouseUpDrawingSelection(pt);

	} // after checking we have something selected
	CTool::OnMouseUp(pt);
	m_Sel.clear();
}

void CSelectFreeTool::GetDragRects(CRect* Rect)
{
		CFloatPoint tl;	
		tl.x = 0;
		tl.y = 0;

		CFloatPoint br;
		br.x = PicEd->m_RectTempToCanvas.Width();
		br.y = PicEd->m_RectTempToCanvas.Height();

		PicEd->CanvastoScreen(tl);
		PicEd->CanvastoScreen(br);
		PicEd->SaveDisplayState();
		
		int count = 0;
#define RECTANGLE2(x,y)	(Rect+count)->SetRect((x)-3,(y)-3,(x)+3,(y)+3); count++;

		RECTANGLE2(tl.x-3,			tl.y-3)
		RECTANGLE2((tl.x+br.x) / 2, tl.y-3)
		RECTANGLE2(br.x+3,			tl.y-3)

		RECTANGLE2(tl.x-3,			(tl.y+br.y) / 2)
		RECTANGLE2(br.x+3,			(tl.y+br.y) / 2)

		RECTANGLE2(tl.x-3,			br.y+3)
		RECTANGLE2((tl.x+br.x) / 2,	br.y+3)
		RECTANGLE2(br.x+3,			br.y+3)

		(Rect+count)->SetRect(tl.x,tl.y,br.x,br.y);



}


///////////////////////////////////////////////
///////////////////////////////////////////////


void CSelectPolygonTool::OnMouseUp(CFloatPoint pt)
{
	

	FinishResizing(pt);
		//m_RectTempToCanvas is a bounding rectangle for the selection
	
	if(m_Sel.size() > 0)
		OnMouseDown(pt);
	if(m_Sel.size()==1)
	{
		PicEd->DropSelection();
		m_Sel.clear();
	}

	PicEd->ClearTemp();
	CTool::OnMouseUp(pt);
//	m_Sel.clear();
//	PicEd->CanvasToTemp();
}

void CSelectPolygonTool::OnMouseDown(CFloatPoint pt)
{
	pt.x += PicEd->m_RectTempToCanvas.left;
	pt.y += PicEd->m_RectTempToCanvas.top;

	FirstMouseCoords.x = pt.x;
	FirstMouseCoords.y = pt.y;

	CTool::OnMouseDown(pt);

	if(m_Sel.size() == 0)
		OnMouseDownResize(pt);

	if(!IsResizing())
	{
		//m_Sel.clear();
		AddSelectionPoint(pt);

		if(m_Sel.size() > 1)
		{
			if(pt.x == m_Sel[0].x
				&& pt.y == m_Sel[0].y)
			{
				if(!IsResizing())
				{
					OnMouseDouble(pt);
				} //after checking we have something selected
			}
		}
	}	
}

void CSelectPolygonTool::OnMouseMove(CFloatPoint pt)
{
	pt.x += PicEd->m_RectTempToCanvas.left;
	pt.y += PicEd->m_RectTempToCanvas.top;

	//POINT point;
	//point.x = pt.x;
	//point.y = pt.y;
	CTool::OnMouseMove(pt);

	if(IsResizing())
	{
		ResizeMove(pt);
	}
	else
	{
		//AddSelectionPoint(pt);
	}
}

void CSelectPolygonTool::PostRender()
{
	
	PicEd->display->SetTexture(-1);
	int count=0;
	if(m_Sel.size() > 0)
	{
		for(int a = 1; a < m_Sel.size(); a ++)
			DrawDottedLine( m_Sel.at(a-1).x,
							m_Sel.at(a-1).y,
							m_Sel.at(a).x,
							m_Sel.at(a).y,&count);
		CFloatPoint pt;
		pt.x = PicEd->m_MouseX;
		pt.y = PicEd->m_MouseY;
		pt.x += PicEd->m_RectTempToCanvas.left*PicEd->m_Zoom;
		pt.y += PicEd->m_RectTempToCanvas.top*PicEd->m_Zoom;
		PicEd->ScreentoCanvas(pt);

		DrawDottedLine(m_Sel.at(m_Sel.size()-1).x, m_Sel.at(m_Sel.size()-1).y, pt.x, pt.y, &count); 
	}
	// if we actually have a selection...draw grippers around it

	DrawGrippers();
}

void CSelectPolygonTool::OnMouseDouble(CFloatPoint pt)
{
	pt.x += PicEd->m_RectTempToCanvas.left;
	pt.y += PicEd->m_RectTempToCanvas.top;

	if(m_Sel.size() != 0)
	{
		AddSelectionPoint(pt);
		OnMouseUpDrawingSelection(pt);	
		m_Sel.clear();
	}


}


///////////////////////////////////////////////

void CSelectRectangleTool::OnMouseUp(CFloatPoint pt)
{
	if( m_resizeleft || m_resizeright || m_resizetop || m_resizebottom)
	{
		FinishResizing(pt);
		//m_RectTempToCanvas is a bounding rectangle for the selection
	}
	else
	{
		m_Sel.clear();
		
		CFloatPoint bottomright;
		bottomright.x = PicEd->m_MouseX;
		bottomright.y = PicEd->m_MouseY;
		bottomright.x += PicEd->m_RectTempToCanvas.left*PicEd->m_Zoom;
		bottomright.y += PicEd->m_RectTempToCanvas.top*PicEd->m_Zoom;
		PicEd->ScreentoCanvas(bottomright);

		CFloatPoint topleft = FirstMouseCoords;
		CRect Rectangle(topleft.x, topleft.y, bottomright.x, bottomright.y);
		
		
		if(Rectangle.Width() != 0 && Rectangle.Height() != 0)
		{
			// we need to trick the child class to thinking we have nodes, otherwise it ignores this whole operation
			POINT meh;
			meh.x = FirstMouseCoords.x;
			meh.y = FirstMouseCoords.y;
			m_Sel.push_back(meh);
			m_Sel.push_back(meh);
			CSelectFreeTool::OnMouseUp(pt);
		}
		else
		{
			m_Sel.clear();
			PicEd->DropSelection();
		}
	}
	CTool::OnMouseUp(pt);
	// note that the function will use CreateSelectionForCxImage to work out what the shape is
}
bool CSelectRectangleTool::CreateSelectionForCxImage(CxImage& curImage)
{

		CFloatPoint bottomright;
		bottomright.x = PicEd->m_MouseX;
		bottomright.y = PicEd->m_MouseY;
		bottomright.x += PicEd->m_RectTempToCanvas.left*PicEd->m_Zoom;
		bottomright.y += PicEd->m_RectTempToCanvas.top*PicEd->m_Zoom;
		PicEd->ScreentoCanvas(bottomright);

		CFloatPoint topleft = FirstMouseCoords;


		CRect Rectangle(min(topleft.x, bottomright.x), min(topleft.y, bottomright.y), max(topleft.x, bottomright.x), max(topleft.y, bottomright.y));

		CRect Screen(0,0, PicEd->m_Width, PicEd->m_Height);
		if(Rectangle.right < 0 || Rectangle.bottom < 0 || Rectangle.left > PicEd->m_Width || Rectangle.top > PicEd->m_Height)
			return false;

	//	Rectangle.bottom ++;
	//	Rectangle.right ++;
//PORT: cannot convert m_Sel.begin() to POINT*, assuming address of front() intended
		curImage.SelectionAddRect(Rectangle);
		return true;
}

void CSelectRectangleTool::PostRender()
{
	
	PicEd->display->SetTexture(-1);
	int count;
	if(m_Sel.size() > 0)
	{
		CFloatPoint bottomright;
		bottomright.x = PicEd->m_MouseX;
		bottomright.y = PicEd->m_MouseY;
		bottomright.x += PicEd->m_RectTempToCanvas.left*PicEd->m_Zoom;
		bottomright.y += PicEd->m_RectTempToCanvas.top*PicEd->m_Zoom;
		PicEd->ScreentoCanvas(bottomright);

		CFloatPoint topleft = FirstMouseCoords;

		count=0;
		DrawDottedLine(topleft.x, topleft.y, bottomright.x, topleft.y, &count);
		count=0;
		DrawDottedLine(topleft.x, topleft.y, topleft.x, bottomright.y, &count); 
		count=0;
		DrawDottedLine(topleft.x, bottomright.y, bottomright.x, bottomright.y, &count); 
		count=0;
		DrawDottedLine(bottomright.x, topleft.y, bottomright.x, bottomright.y, &count); 

	}
	// if we actually have a selection...draw grippers around it

	DrawGrippers();
}



///////////////////////////////////////////////
/*
void CSelectEllipseTool::OnMouseUp(CFloatPoint pt)
{
	if( m_resizeleft || m_resizeright || m_resizetop || m_resizebottom)
	{
		FinishResizing(pt);
		//m_RectTempToCanvas is a bounding rectangle for the selection
	}
	else
	{
		m_Sel.clear();
		
		CFloatPoint bottomright;
		bottomright.x = PicEd->m_MouseX;
		bottomright.y = PicEd->m_MouseY;
		bottomright.x += PicEd->m_RectTempToCanvas.left*PicEd->m_Zoom;
		bottomright.y += PicEd->m_RectTempToCanvas.top*PicEd->m_Zoom;
		PicEd->ScreentoCanvas(bottomright);

		CFloatPoint topleft = FirstMouseCoords;
		CRect Rectangle(topleft.x, topleft.y, bottomright.x, bottomright.y);
		
	
		if(Rectangle.Width() != 0 && Rectangle.Height() != 0)
		{
			// we need to trick the child class to thinking we have nodes, otherwise it ignores this whole operation
			POINT meh;
			m_Sel.push_back(meh);
			m_Sel.push_back(meh);
			CSelectFreeTool::OnMouseUp(pt);
		}
		else
		{
			m_Sel.clear();

			PicEd->DropSelection();
		}

		//
	}
	CTool::OnMouseUp(pt);

	// note that the function will use CreateSelectionForCxImage to work out what the shape is
}*/
bool CSelectEllipseTool::CreateSelectionForCxImage(CxImage& curImage)
{

		CFloatPoint bottomright;
		bottomright.x = PicEd->m_MouseX;
		bottomright.y = PicEd->m_MouseY;
		bottomright.x += PicEd->m_RectTempToCanvas.left*PicEd->m_Zoom;
		bottomright.y += PicEd->m_RectTempToCanvas.top*PicEd->m_Zoom;
		PicEd->ScreentoCanvas(bottomright);

		CFloatPoint topleft = FirstMouseCoords;


		CRect Rectangle(min(topleft.x, bottomright.x), min(topleft.y, bottomright.y), max(topleft.x, bottomright.x), max(topleft.y, bottomright.y));

		Rectangle.top --;
		Rectangle.left --;
		Rectangle.right ++;
		Rectangle.bottom ++;

		if(Rectangle.right < 0 || Rectangle.bottom < 0 || Rectangle.left > PicEd->m_Width || Rectangle.top > PicEd->m_Height)
			return false;
//PORT: cannot convert m_Sel.begin() to POINT*, assuming address of front() intended
	curImage.SelectionAddEllipse(Rectangle);
	return true;
}

void CSelectEllipseTool::PostRender()
{
	
	PicEd->display->SetTexture(-1);
	int count = 0;
	if(m_Sel.size() > 0)
	{
		CFloatPoint bottomright;
		bottomright.x = PicEd->m_MouseX;
		bottomright.y = PicEd->m_MouseY;
		bottomright.x += PicEd->m_RectTempToCanvas.left*PicEd->m_Zoom;
		bottomright.y += PicEd->m_RectTempToCanvas.top*PicEd->m_Zoom;
		PicEd->ScreentoCanvas(bottomright);

		CFloatPoint topleft = FirstMouseCoords;

		
		float middlex = ((int)topleft.x + (int)bottomright.x)/2.0;
		float middley = ((int)topleft.y + (int)bottomright.y)/2.0;

		float radx = ((int)bottomright.x - (int)topleft.x)/2.0;
		float rady = ((int)bottomright.y - (int)topleft.y)/2.0;

		
		for(float a = 0; a < 360; a++)
		{
			float angle = Rad(a);
			float sin_a, cos_a;
			cr::sincosf(angle, &sin_a, &cos_a);

			CPoint p1;
			p1.x = middlex + sin_a * (radx+0.5);
			p1.y = middley + cos_a * (rady+0.5);
			angle = Rad(a+1);
			CPoint p2;
			p2.x = middlex + sin_a * (radx+0.5);
			p2.y = middley + cos_a * (rady+0.5);
			DrawDottedLine(p1.x, p1.y, p2.x, p2.y, &count);
		}
	}
	// if we actually have a selection...draw grippers around it

	DrawGrippers();
}



///////////////////////////////////////////////
void CSelectColorTool::OnMouseUp(CFloatPoint pt)
{
	if(IsResizing())
	{
		FinishResizing(pt);
		//m_RectTempToCanvas is a bounding rectangle for the selection
	}

	return;
}
void CSelectColorTool::OnMouseMove(CFloatPoint pt)
{
/*	pt.x += PicEd->m_RectTempToCanvas.left;
	pt.y += PicEd->m_RectTempToCanvas.top;

	CTool::OnMouseMove(pt);

	if(IsResizing())
	{
		ResizeMove(pt);
	}
	return;*/

	pt.x += PicEd->m_RectTempToCanvas.left;
	pt.y += PicEd->m_RectTempToCanvas.top;

	//POINT point;
	//point.x = pt.x;
	//point.y = pt.y;
	CTool::OnMouseMove(pt);

	if(IsResizing())
	{
		ResizeMove(pt);
	}
	else
	{
		//AddSelectionPoint(pt);
	}





}
void CSelectColorTool::OnMouseDown(CFloatPoint pt)
{

	CSelectFreeTool::OnMouseDown(pt);
	if(!IsResizing())
	{
		PicEd->DropSelection();
		m_Sel.clear();

		
		
		// we need to trick the child class to thinking we have nodes, otherwise it ignores this whole operation
		POINT meh;
		m_Sel.push_back(meh);
		m_Sel.push_back(meh);
		CSelectFreeTool::OnMouseUp(pt);
	}

	if(PicEd->RightMouseDown)
		PicEd->DropSelection();
		
}

bool CSelectColorTool::CreateSelectionForCxImage(CxImage& curImage)
{
		CFloatPoint point;
		point.x = PicEd->m_MouseX;
		point.y = PicEd->m_MouseY;
		point.x += PicEd->m_RectTempToCanvas.left*PicEd->m_Zoom;
		point.y += PicEd->m_RectTempToCanvas.top*PicEd->m_Zoom;
		PicEd->ScreentoCanvas(point);

		if(curImage.IsInside(point.x, point.y))
		{
			RGBQUAD c = curImage.GetPixelColor(point.x, curImage.GetHeight()-1- point.y, true);
			{
				for(int x = 0; x < curImage.GetWidth(); x++)
				for(int y = 0; y < curImage.GetHeight(); y ++)
				{
					// note: if we do tolerance, we need to account for premultiplied rgb
					RGBQUAD c2 = curImage.GetPixelColor(x,y,true);
					if(c2.rgbBlue == c.rgbBlue
						&& c2.rgbRed == c.rgbRed
						&& c2.rgbGreen == c.rgbGreen
						&& c2.rgbReserved == c.rgbReserved)
						curImage.SelectionAddPixel(x, curImage.GetHeight()-1-y);
				}
			}
			return true;
		}
	
		return false;
		
		// note: the selectionAddColor function is broken ... the color channels appear to be flipped vertically?
		//curImage.SelectionAddColor(curImage.GetPixelColor(point.x, point.y, true));
}

/////////////////////////////////////////////////////


bool CSelectWandTool::CreateSelectionForCxImage(CxImage& curImage)
{
		CFloatPoint point;
		point.x = PicEd->m_MouseX;
		point.y = PicEd->m_MouseY;
		point.x += PicEd->m_RectTempToCanvas.left*PicEd->m_Zoom;
		point.y += PicEd->m_RectTempToCanvas.top*PicEd->m_Zoom;
		PicEd->ScreentoCanvas(point);

		if(!curImage.IsInside(point.x, point.y))
			return false;
		RGBQUAD c = curImage.GetPixelColor(point.x, curImage.GetHeight()-1- point.y);


		FillSelection(&curImage,c, point.x, curImage.GetHeight()-1- point.y);

		return true;
}


///////////////////////////////////////////////////////



bool SelectionIsInside2(int x, int y,CxImage& img)
{
	if(x<0) return true;
	if(y<0) return true;
	if(x>=img.GetWidth()) return true;
	if(y>=img.GetHeight()) return true;
	return img.SelectionIsInside(x,y);
}



CRect CPictureEditor::CreateSelectionFromCxImage(CxImage &curImage)
{


		m_Sel2.clear();
		int m_Sel2Count = 0;
		Line line;
		m_Sel2.push_back(line);
		RECT r;
		curImage.SelectionGetBox(r); // sometimes bottom is top!

		if(r.bottom < r.top)
			swap(r.top,r.bottom);
		if(r.right < r.left)
			swap(r.left,r.right);
		

		bool SelectTransparent = false;
		curImage.SelectionInvert();
		if(!curImage.AlphaIsValid())
			SelectTransparent = true;

		if(!SelectTransparent)
		{
			if(RightMouseDown)
				SelectTransparent = true;


		}

		if(r.left != r.right || r.bottom != r.top)
		{	
			for( int x = r.left ; x < r.right ; x ++ )
				for( int y = r.top  ; y < r.bottom; y++ )
				{
					if(!SelectTransparent)
					{
						if(!SelectionIsInside2(x,y,curImage)	)
						{
							
							if(curImage.AlphaGet(x,curImage.GetHeight() - y-1) == 0)	
							{
								curImage.SelectionAddPixel(x,y);
							}
							else
								curImage.AlphaSet(x,curImage.GetHeight() - y-1,255);
						}
						else
							curImage.AlphaSet(x,curImage.GetHeight() - y-1,0);
					}
					else if(curImage.AlphaIsValid())
					{
						curImage.AlphaSet(x,curImage.GetHeight() - y-1,255);
					}
					
							
					if(!curImage.SelectionIsInside(x,y)
						&& (SelectTransparent? true :  
							curImage.AlphaGet(x,curImage.GetHeight() - y-1)!= 0 ))
							{
								if(SelectionIsInside2(x,y-1,curImage)
								|| (SelectTransparent? false : 
								 curImage.AlphaGet(x,curImage.GetHeight()-y+1-1)== 0))
								{
									m_Sel2[m_Sel2Count].x = x-r.left;
									m_Sel2[m_Sel2Count].y = y-r.top;
									m_Sel2[m_Sel2Count].type = 1;
									m_Sel2Count++;
									m_Sel2.push_back(line);
								}

								if(SelectionIsInside2(x+1,y,curImage)
								|| (SelectTransparent? false :
								curImage.AlphaGet(x+1,curImage.GetHeight()-y-1)== 0))
								{
									m_Sel2[m_Sel2Count].x = x-r.left;
									m_Sel2[m_Sel2Count].y = y-r.top;
									m_Sel2[m_Sel2Count].type = 2;
									m_Sel2Count++;
									m_Sel2.push_back(line);
								}

								if(SelectionIsInside2(x,y+1,curImage)
								|| (SelectTransparent? false :
								 curImage.AlphaGet(x,curImage.GetHeight()-y-1-1)== 0))
								{
									m_Sel2[m_Sel2Count].x = x-r.left;
									m_Sel2[m_Sel2Count].y = y-r.top;
									m_Sel2[m_Sel2Count].type = 3;
									m_Sel2Count++;
									m_Sel2.push_back(line);
								}

								if(SelectionIsInside2(x-1,y,curImage)
								|| (SelectTransparent? false : 
								 curImage.AlphaGet(x-1,curImage.GetHeight()-y-1)== 0))
								{
									m_Sel2[m_Sel2Count].x = x-r.left;
									m_Sel2[m_Sel2Count].y = y-r.top;
									m_Sel2[m_Sel2Count].type = 4;
									m_Sel2Count++;
									m_Sel2.push_back(line);
								}
							}
						}

						curImage.SelectionInvert();
						m_Sel2.pop_back();

		}
						//CRectTracker(
						//	pDoc->Stopwatch(1);
						//	pDoc->UpdateStatusBar();
		//	int top = r.top;
		//	r.top = r.bottom;
		//	r.bottom = top; // Tempory fix for cxImage bug ;P
			curImage.Crop(r);// crop it to the selection area
		
			return r;
}




////////////////
// Hotspot
////////////////


void CHotspotTool::OnMouseMove(CFloatPoint pt)
{
	if(m_draw)
	{
		hotspot.x = pt.x;
		hotspot.y = pt.y;

		if(GetKeyState(VK_MENU) >> 4)	//alt clicks
			PicEd->m_pImageEditor->m_pImgEdDlg->ApplyHotspotToAll(hotspot.x, hotspot.y);
	}

}



void CHotspotTool::PostRender()
{

	CFloatPoint hotspot2;
			hotspot2.x = ((int)hotspot.x);
			hotspot2.y = ((int)hotspot.y);

	PicEd->CanvastoScreen(hotspot2);
	hotspot2.x += PicEd->m_Zoom*0.5;
	hotspot2.y += PicEd->m_Zoom*0.5;
	PicEd->SaveDisplayState();
	PicEd->display->SetTexture(-1);

	// Create a cross hair
	// Black
	PicEd->display->Point(hotspot2.x, hotspot2.y,0xffff0000);

	PicEd->display->Point(hotspot2.x-2, hotspot2.y,0xffff0000);
	PicEd->display->Point(hotspot2.x-3, hotspot2.y,0xffff0000);
	PicEd->display->Point(hotspot2.x+2, hotspot2.y,0xffff0000);
	PicEd->display->Point(hotspot2.x+3, hotspot2.y,0xffff0000);
	PicEd->display->Point(hotspot2.x, hotspot2.y-2,0xffff0000);
	PicEd->display->Point(hotspot2.x, hotspot2.y-3,0xffff0000);
	PicEd->display->Point(hotspot2.x, hotspot2.y+2,0xffff0000);
	PicEd->display->Point(hotspot2.x, hotspot2.y+3,0xffff0000);
	// White
	PicEd->display->Point(hotspot2.x-1, hotspot2.y,0xffffff00);
	PicEd->display->Point(hotspot2.x+1, hotspot2.y,0xffffff00);
	PicEd->display->Point(hotspot2.x, hotspot2.y-1,0xffffff00);
	PicEd->display->Point(hotspot2.x, hotspot2.y+1,0xffffff00);

	PicEd->display->Point(hotspot2.x-4, hotspot2.y,0xffffff00);
	PicEd->display->Point(hotspot2.x+4, hotspot2.y,0xffffff00);
	PicEd->display->Point(hotspot2.x, hotspot2.y-4,0xffffff00);
	PicEd->display->Point(hotspot2.x, hotspot2.y+4,0xffffff00);

	PicEd->display->Point(hotspot2.x-3, hotspot2.y-3,0xffffff00);
	PicEd->display->Point(hotspot2.x+3, hotspot2.y+3,0xffffff00);
	PicEd->display->Point(hotspot2.x+3, hotspot2.y-3,0xffffff00);
	PicEd->display->Point(hotspot2.x-3, hotspot2.y+3,0xffffff00);



//	DrawLine(pt.x-1,pt.y,pt.x,pt.y,MouseSteps,maxsteps);
//	PicEd->display->SetRenderTarget(RENDERTARGET_DISPLAY);


	PicEd->RestoreDisplayState();

}

void CHotspotTool::OnMouseDown(CFloatPoint pt)
{
	CTool::OnMouseDown(pt);

	OnMouseMove(pt);
}

void CHotspotTool::OnMouseUp(CFloatPoint pt)
{
	CTool::OnMouseUp(pt);
	CPoint* image_hotspot = PicEd->GetHotspot(); 
	if(image_hotspot)
	{
		image_hotspot->x = hotspot.x;
		image_hotspot->y = hotspot.y;
	}
	UpdateDisplayOffset();
}

void CHotspotTool::Initialise(ToolProperties *props)
{
	CTool::Initialise(props);
	Modify();
}
void CHotspotTool::OnKeyDown(long key)
{
	bool update = false;
	switch (key)
	{
	case VK_LEFT:
		hotspot.x ++;
		update = true;
		break;
	case VK_RIGHT:
		hotspot.x --;
		update = true;
		break;
	case VK_UP:
		hotspot.y ++;
		update = true;
		break;
	case VK_DOWN:
		hotspot.y --;
		update = true;
		break;
	}
	if(update)
	{
		CPoint* saved_hotspot = PicEd->GetHotspot();
		if(saved_hotspot)
		{
			saved_hotspot->x = hotspot.x;
			saved_hotspot->y = hotspot.y;
		}

		PicEd->m_modded = true;
		UpdateDisplayOffset();
		PicEd->m_pImageEditor->Invalidate();
	}
}

void CHotspotTool::Modify()
{
	if( PicEd->GetHotspot() )
	{
		hotspot.x = PicEd->GetHotspot()->x;
		hotspot.y = PicEd->GetHotspot()->y;
	}
	UpdateDisplayOffset();
}
void CHotspotTool::UpdateDisplayOffset()
{
	PicEd->displayOffsetX = (int)(hotspot.x);
	PicEd->displayOffsetY = (int)(hotspot.y);
}
void CHotspotTool::Cancel()
{
	if( PicEd->GetHotspot() )
	{
		hotspot.x = PicEd->GetHotspot()->x;
		hotspot.y = PicEd->GetHotspot()->y;
	}

}
////////////////////////////
////
////   Action Point



void CActionTool::PostRender()
{

	CFloatPoint hotspot2;
			hotspot2.x = (int)hotspot.x;
			hotspot2.y = (int)hotspot.y;

	PicEd->CanvastoScreen(hotspot2);
	hotspot2.x += PicEd->m_Zoom*0.5;
	hotspot2.y += PicEd->m_Zoom*0.5;
	PicEd->SaveDisplayState();
	PicEd->display->SetTexture(-1);

	// Create a cross hair
	// Black
	PicEd->display->Point(hotspot2.x, hotspot2.y,0xff0000ff);

	PicEd->display->Point(hotspot2.x-2, hotspot2.y,0xff0000ff);
	PicEd->display->Point(hotspot2.x-3, hotspot2.y,0xff0000ff);
	PicEd->display->Point(hotspot2.x+2, hotspot2.y,0xff0000ff);
	PicEd->display->Point(hotspot2.x+3, hotspot2.y,0xff0000ff);
	PicEd->display->Point(hotspot2.x, hotspot2.y-2,0xff0000ff);
	PicEd->display->Point(hotspot2.x, hotspot2.y-3,0xff0000ff);
	PicEd->display->Point(hotspot2.x, hotspot2.y+2,0xff0000ff);
	PicEd->display->Point(hotspot2.x, hotspot2.y+3,0xff0000ff);
	// White
	PicEd->display->Point(hotspot2.x-1, hotspot2.y,0xffffff00);
	PicEd->display->Point(hotspot2.x+1, hotspot2.y,0xffffff00);
	PicEd->display->Point(hotspot2.x, hotspot2.y-1,0xffffff00);
	PicEd->display->Point(hotspot2.x, hotspot2.y+1,0xffffff00);

	PicEd->display->Point(hotspot2.x-4, hotspot2.y,0xffffff00);
	PicEd->display->Point(hotspot2.x+4, hotspot2.y,0xffffff00);
	PicEd->display->Point(hotspot2.x, hotspot2.y-4,0xffffff00);
	PicEd->display->Point(hotspot2.x, hotspot2.y+4,0xffffff00);

	PicEd->display->Point(hotspot2.x-3, hotspot2.y-3,0xffffff00);
	PicEd->display->Point(hotspot2.x+3, hotspot2.y+3,0xffffff00);
	PicEd->display->Point(hotspot2.x+3, hotspot2.y-3,0xffffff00);
	PicEd->display->Point(hotspot2.x-3, hotspot2.y+3,0xffffff00);

	PicEd->RestoreDisplayState();

}




void CActionTool::OnMouseUp(CFloatPoint pt)
{
	CTool::OnMouseUp(pt);
	CString action;
	this->PicEd->m_pImageEditor->m_pImgEdDlg->m_Tool_Settings.m_ActionPointCombo.GetWindowText(action);
	
	if(action!= "" && PicEd->GetAction())
	{
		(*PicEd->GetAction())[action].x = hotspot.x;
		(*PicEd->GetAction())[action].y = hotspot.y;
	}
}

void CActionTool::Initialise(ToolProperties *props)
{
	CTool::Initialise(props);

	Modify();

}

void CActionTool::Modify()
{
	CString action;
	this->PicEd->m_pImageEditor->m_pImgEdDlg->m_Tool_Settings.m_ActionPointCombo.GetWindowText(action);
	
	if(action!="" && PicEd->GetAction())
	{
		if(PicEd->GetAction()->find(action) != PicEd->GetAction()->end())
		{
			hotspot.x = (*PicEd->GetAction())[action].x;
			hotspot.y = (*PicEd->GetAction())[action].y;
		}
	}


}

void CActionTool::UpdateDisplayOffset()
{
	PicEd->displayOffsetX = (int)(hotspot.x);
	PicEd->displayOffsetY = (int)(hotspot.y);
}




////////////////////////////
////
////   Rectangle tool
		

void CRectangleTool::OnMouseMove(CFloatPoint pt)
{
	if(MouseCoords.x == pt.x && MouseCoords.y== pt.y )
		return; // ie. the mouse hasn't moved :P

	MouseCoords = pt;
}

void CRectangleTool::Render()
{
	int linesize = m_pProp->LineThickness;

	PicEd->display->SetRenderState(D3DRS_SRCBLEND ,  D3DBLEND_ONE);
	PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);

	D3DCOLOR c1 = GetColor();
	PremultiplyFilter(c1);
	D3DCOLOR c2 = GetColor2();
	PremultiplyFilter(c2);

	if(m_draw)
	{
		PicEd->ClearTemp();
		Proportion();
		CRect Box = CRect(	min(FirstMouseCoords.x,MouseCoords.x),
				min(FirstMouseCoords.y,MouseCoords.y),					
				max(FirstMouseCoords.x,MouseCoords.x)+1,
				max(FirstMouseCoords.y,MouseCoords.y)+1)
				;
		CRect Box2 = Box;

		switch(PicEd->m_Props.Style)
		{
		case 1:

			Box2 = Box;
			if(Box.Width() > linesize && Box.Height() > linesize)
			{
				Box2.DeflateRect(linesize, linesize);
		
				PicEd->display->Fill(Box2
									,c2);
			}

			// let it fall into the outline so we draw an outline
		case 0:	// outline
			// top
			if(Box.Width() > linesize && Box.Height() > linesize)
			{
			Box2 = Box;
			Box2.bottom = Box2.top + linesize;
			PicEd->display->Fill(Box2, c1);

			//bottom
			Box2 = Box;
			Box2.top = Box2.bottom - linesize;
			PicEd->display->Fill(Box2, c1);

			//left
			Box2 = Box;
			Box2.top += linesize;
			Box2.bottom -= linesize;
			Box2.right = Box2.left + linesize;
			PicEd->display->Fill(Box2, c1);

			//right
			Box2 = Box;
			Box2.top += linesize;
			Box2.bottom -= linesize;
			Box2.left = Box2.right - linesize;
			PicEd->display->Fill(Box2, c1);
			}
			else
			{
				PicEd->display->Fill(Box
								, c1);
			}
			break;
						

		case 2:	//fill
			PicEd->display->Fill(Box2
								,c1);
			break;
		case 3:	// gradient
			PicEd->display->FillGradient(Box2
								,c1, c2, DIR_RIGHT );
			break;
		case 4:	// gradient
			PicEd->display->FillGradient(Box2
								, c1, c2, DIR_DOWN );
			break;

		}
	}
	else
	{
		PicEd->ClearTemp();
	
		PicEd->display->Fill(
				CRect(
					min(MouseCoords.x,MouseCoords.x),
					min(MouseCoords.y,MouseCoords.y),					
					max(MouseCoords.x,MouseCoords.x)+1,
					max(MouseCoords.y,MouseCoords.y)+1)
					,c1);



	}
	
}




//////////////////////
//      Flood Fill




//////////////////////
//      Polygon Tool

void CPolygonTool::Render()
{
	PicEd->ClearTemp();
	int a;
	PicEd->display->SetTexture(-1);

	D3DCOLOR c1 = GetColor();
	PremultiplyFilter(c1);
	D3DCOLOR c2 = GetColor2();
	PremultiplyFilter(c2);


	if(m_CFloatPoint.size() != 0)
	{
		for(a = 0; a < (int)(m_CFloatPoint.size() )-1; a++)
		{
			POINT* i = &m_CFloatPoint[a];
			PicEd->display->Line(i->x, i->y, (i+1)->x, (i+1)->y, c1);
		}
		POINT* i = &m_CFloatPoint[a];
		PicEd->display->Line(i->x, i->y, (int)MouseCoords.x,  (int)MouseCoords.y, c1);
	}
}
void CPolygonTool::OnMouseDouble(CFloatPoint pt)
{
	if(m_CFloatPoint.size() != 0)
	{
		AddPoint(pt);
		PicEd->ClearTemp();
		OnMouseUpDrawPolygon(pt);
		m_CFloatPoint.clear();
	}
}
void CPolygonTool::AddPoint(CFloatPoint pt)
{
	POINT point;
	point.x = pt.x;
	point.y = pt.y;
	if(m_CFloatPoint.size() != 0)
	{
		if(m_CFloatPoint.back().x != point.x
			&& m_CFloatPoint.back().y != point.y)
				m_CFloatPoint.push_back(point);
	}
	else
		m_CFloatPoint.push_back(point);
}
void CPolygonTool::OnMouseDown(CFloatPoint pt)
{
	m_draw = true;
	//CTool::OnMouseDown(pt);


	AddPoint(pt);

}
void CPolygonTool::OnMouseUp(CFloatPoint pt)
{

	if(m_CFloatPoint.size() > 0)
		OnMouseDown(pt);
	m_draw = false;

}
void CPolygonTool::Cancel()
{
	CTool::Cancel();

}

void CPolygonTool::OnMouseMove(CFloatPoint pt)
{
	CTool::OnMouseMove(pt);
	MouseCoords = pt;
}

void CPolygonTool::OnMouseUpDrawPolygon(CFloatPoint pt)
{
	if(m_CFloatPoint.size() < 2)
		return;
	CxImage saved;

	D3DCOLOR c1 = GetColor();
	PremultiplyFilter(c1);
	D3DCOLOR c2 = GetColor2();
	PremultiplyFilter(c2);
	
	PicEd->SaveDisplayState();

	CDC *pDesktopDC = CDC::FromHandle (::GetDCEx(NULL, NULL, 0));

	// We use device contexts to actually render the polygon

	CDC dc;
	dc.CreateCompatibleDC(pDesktopDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, PicEd->m_CanvasWidth, PicEd->m_CanvasHeight);
	CBrush brush;
	brush.CreateSolidBrush(0xffffff);
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, 0xffffff);

	CBitmap* old_bitmap = dc.SelectObject(&bitmap);
	CPen*	old_pen = dc.SelectObject(&pen);
	CBrush* old_brush = dc.SelectObject(&brush);

	dc.FillSolidRect(CRect(0,0,PicEd->m_CanvasWidth, PicEd->m_CanvasHeight),0);
	dc.Polygon(&(m_CFloatPoint.front()), m_CFloatPoint.size());

	CBitmap* newbitmap = dc.SelectObject(old_bitmap);
	dc.SelectObject(old_pen);
	dc.SelectObject(old_brush);


	CxImage Image;
	Image.CreateFromHBITMAP((HBITMAP)newbitmap->GetSafeHandle());
	Image.AlphaDelete();
	Image.AlphaCreate();

	for(int x = 0; x < Image.GetWidth(); x++)
	{
		for(int y = 0; y < Image.GetHeight(); y++)
		{
			Image.AlphaSet(x,y,Image.GetPixelColor(x,y,true).rgbRed);
		}
	}



	int tempimage = PicEd->display->AddTextureFromCxImage(Image);
	

	PicEd->display->SetRenderTarget(*PicEd->m_pCanvas);

	PicEd->display->SetTexture(tempimage);
	PicEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);

	PicEd->display->Blit(0,0,1.0f,1.0f,0.0f,c1,NULL);
	PicEd->display->SetTexture(-1);

	PicEd->display->RemoveTexture(tempimage);
	
	PicEd->RestoreDisplayState();

	PicEd->CreateUndo("Polygon");



}













void CFillTool::OnMouseDown(CFloatPoint pt)
{
	PicEd->SaveDisplayState();
	CxImage src;
	CD3DDisplay* display = PicEd->display;
	m_draw = true;

	CxImage saved = PicEd->m_undo.back()->src;
	
	/*if(PicEd->m_undo.size() > 0)
	{
		saved = this->PicEd->m_undo[0]->src;
	}*/

	//else
/*	{
		
		LPD3DXBUFFER ppDestBuf;

		D3DXSaveTextureToFileInMemory(
		  &ppDestBuf,
		  D3DXIFF_PNG,
		  display->TextureAt(*PicEd->m_pCanvas),
		  NULL
		);

		ppDestBuf->GetBufferSize();


		
		saved.Decode((BYTE*)ppDestBuf->GetBufferPointer()
				   ,ppDestBuf->GetBufferSize(),CXIMAGE_FORMAT_PNG);

		ppDestBuf->Release();
	 }
	*/

	
	// we have our canvas in a CxImage...commence flood fill.

	int x = pt.x;
	int y =  saved.GetHeight()-1-(int)pt.y;
	RGBQUAD c = saved.GetPixelColor(x,y);
	RGBQUAD newcolour;

	D3DCOLOR c1 = GetColor();
	PremultiplyFilter(c1);
	D3DCOLOR c2 = GetColor2();
	PremultiplyFilter(c2);

	COLORREF* ptr = (COLORREF*)&newcolour;
	*ptr = c1;



	if(CVT_COL(newcolour) != CVT_COL(c))
	{
		Fill(&saved, c, newcolour, x, y);
	
		// All done now reload
		
		// Load our canvas from CxImage
		int m_CanvasBG = display->AddTextureFromCxImage(saved);

		display->SetRenderTarget(*PicEd->m_pCanvas);
		display->SetTexture(m_CanvasBG);
		display->ClearRT();
		PicEd->SaveDisplayState();
		display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
		display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ZERO);
		display->Blit(0,0);
		PicEd->RestoreDisplayState();

		display->RemoveTexture(m_CanvasBG);
	}
	PicEd->RestoreDisplayState();

}

void CShapeTool::Initialise(ToolProperties* props)
{
	CTool::Initialise(props);
	switch (m_shape)
	{
	case ID_ELLIPSE:
		if(PicEd->m_shapes.find(ID_ELLIPSE) == PicEd->m_shapes.end())
		{
			//Create a new circle
			int texture = PicEd->display->AddTextureRT(512,512);
			CreateCircle(PicEd->display,texture,256,100,1, 0,0xffffffff);
			PicEd->display->Premultiply(texture);

			PicEd->m_shapes[ID_ELLIPSE] = texture;

			//PicEd->display->SaveTexture(texture);
		}
		break;
	}

}


void CShapeTool::Render()
{
	D3DCOLOR c1 = GetColor();
	PremultiplyFilter(c1);
	D3DCOLOR c2 = GetColor2();
	PremultiplyFilter(c2);

	int linesize = m_pProp->LineThickness;

	PicEd->display->SetRenderState(D3DRS_SRCBLEND ,  D3DBLEND_ONE);
	PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);

	int texture = PicEd->m_shapes[ID_ELLIPSE];
	PicEd->display->SetTexture(texture);

	if(m_draw)
	{
		PicEd->ClearTemp();
		
		Proportion();
		CRect Box = CRect(	min(FirstMouseCoords.x,MouseCoords.x),
				min(FirstMouseCoords.y,MouseCoords.y),					
				max(FirstMouseCoords.x,MouseCoords.x)+1,
				max(FirstMouseCoords.y,MouseCoords.y)+1)
				;
		CRect area = Box;


		
		bool dontfill = true;

		switch(PicEd->m_Props.Style)
		{
			case 1:	//outline + fill
				dontfill = false;

			case 0: // outline
				if(linesize == 1)
				{
					PicEd->display->SetRenderState(D3DRS_SRCBLEND ,  D3DBLEND_ONE);
					PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);	
					
					PicEd->display->Blit(area.left+1, area.top+0, area.Width()-2, area.Height()-2,0, c1, NULL);
					PicEd->display->Blit(area.left+1, area.top+2, area.Width()-2, area.Height()-2,0, c1, NULL);
					PicEd->display->Blit(area.left+0, area.top+1, area.Width()-2, area.Height()-2,0, c1, NULL);
					PicEd->display->Blit(area.left+2, area.top+1, area.Width()-2, area.Height()-2,0, c1, NULL);

					PicEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ZERO);
					PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);

					area.DeflateRect(linesize, linesize, linesize, linesize);
					PicEd->display->Blit(area.left, area.top, area.Width(), area.Height(),0, 0xffffffff, NULL);

					PicEd->display->SetRenderState(D3DRS_SRCBLEND ,  D3DBLEND_ONE);
					PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);	
				}
				else
				{
					PicEd->display->SetRenderState(D3DRS_SRCBLEND ,  D3DBLEND_ONE);
					PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);	

					PicEd->display->Blit(area.left, area.top, area.Width(), area.Height(),0, c1, NULL);
					area.DeflateRect(linesize, linesize, linesize, linesize);
			
					PicEd->display->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ZERO);
					PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);

					PicEd->display->Blit(area.left, area.top, area.Width(), area.Height(),0,  0xffffffff, NULL);				
			
					PicEd->display->SetRenderState(D3DRS_SRCBLEND ,  D3DBLEND_ONE);
					PicEd->display->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);	
				}
				if(dontfill)
					break;
				// we need to fill for option 1.
				PicEd->display->Blit(area.left, area.top, area.Width(), area.Height(),0, c2, NULL);
				break;

			case 2:
				PicEd->display->Blit(area.left, area.top, area.Width(), area.Height(),0, c1, NULL);
				break;

			case 3:	// need a way of changing the vertex colours on a normal blit :/
				PicEd->display->Blit(area.left, area.top, area.Width(), area.Height(),0, -1, c1,c2,c1,c2,NULL);
				break;

			case 4:	// need a way of changing the vertex colours on a normal blit :/
				PicEd->display->Blit(area.left, area.top, area.Width(), area.Height(),0, -1, c1,c1,c2,c2,NULL);
				break;
				


		}
	}
	else
	{
		PicEd->ClearTemp();
	
		PicEd->display->Fill(
				CRect(
					min(MouseCoords.x,MouseCoords.x),
					min(MouseCoords.y,MouseCoords.y),					
					max(MouseCoords.x,MouseCoords.x)+1,
					max(MouseCoords.y,MouseCoords.y)+1)
					,c1);



	}
	
}



CPoint* CPictureEditor::GetHotspot()
{
	CImageEditorDlg* dlg = m_pImageEditor->m_pImgEdDlg;

	if (dlg->m_sourceImages.size() > 0)
		return &dlg->m_newHotspots[ dlg->m_sourceImages.at(dlg->m_animation_frame_index) ];
	
	return NULL;
}

map<CString, CPoint>* CPictureEditor::GetAction()
{
	CImageEditorDlg* dlg = m_pImageEditor->m_pImgEdDlg;

	if (dlg->m_sourceImages.size() > 0)
		return &dlg->m_newAction[ dlg->m_sourceImages.at(dlg->m_animation_frame_index) ];	
	
	return NULL;
}

//////////ZOOM

void CZoomTool::OnMouseDown(CFloatPoint pt)
{
	if(PicEd->LeftMouseDown)
		PicEd->m_pImageEditor->m_zoom_val +=120;
	else
		PicEd->m_pImageEditor->m_zoom_val -=120;


	if(PicEd->m_pImageEditor->m_zoom_val >= 0 )
		PicEd->m_Zoom = (PicEd->m_pImageEditor->m_zoom_val/120.0)+1;
	if(PicEd->m_pImageEditor->m_zoom_val < 0 ) 
		PicEd->m_Zoom = 1.0/(1-(PicEd->m_pImageEditor->m_zoom_val/120.0));

}


///////////////

void CArrowTool::OnMouseDown(CFloatPoint pt)
{
	CxImage copy = PicEd->m_undo.back()->src;

	RGBQUAD color = copy.GetPixelColor(pt.x, copy.GetHeight()-1-(int)pt.y);

	D3DCOLOR c1 = GetColor();
	PremultiplyFilter(c1);
	D3DCOLOR c2 = GetColor2();
	PremultiplyFilter(c2);

	COLORREF nc = c1;
	RGBQUAD newcolor;
	newcolor.rgbBlue = GetRValue(nc);
	newcolor.rgbGreen = GetGValue(nc);
	newcolor.rgbRed = GetBValue(nc);
	newcolor.rgbReserved = nc >> 24;

	
	for(int x = 0; x < copy.GetWidth(); x++)
	{
		for(int y = 0; y < copy.GetHeight(); y++)
		{
			if(*((int*)&copy.GetPixelColor(x,y)) == *((int*)&color))
				copy.SetPixelColor(x,y, newcolor, true);
		}
	}
	PicEd->Load(&copy);

	PicEd->CreateUndo("Replace Colour");
}


///////////////


void CPickTool::OnMouseDown(CFloatPoint pt)
{
	RGBQUAD colour = PicEd->m_undo.back()->src.GetPixelColor(pt.x, PicEd->m_undo.back()->src.GetHeight()- pt.y);

	RGBQUAD c2 = colour;
	if(c2.rgbReserved != 0)
	{
		c2.rgbRed /= c2.rgbReserved/255.0;
		c2.rgbGreen /= c2.rgbReserved/255.0;
		c2.rgbBlue /= c2.rgbReserved/255.0;
	}
	*PicEd->m_pImageEditor->m_pColor_Mixer->pColor = RGB(c2.rgbRed, c2.rgbGreen, c2.rgbBlue);
	PicEd->Colour1 = colour.rgbBlue + (colour.rgbGreen << 8 )+ (colour.rgbRed << 16) + (colour.rgbReserved << 24);

		


	PicEd->m_pImageEditor->UpdateTool();

	PicEd->m_pImageEditor->m_tool = lasttool;
	PicEd->m_pImageEditor->ChangeTool();

	return;

}

void CPictureEditor::UpdateTool()
{
	m_pImageEditor->UpdateTool();
}

CPictureEditor::~CPictureEditor()
{
	while(m_undo.size()>0)
	{
		m_undo.back()->Release();
		delete m_undo.back();
		m_undo.pop_back();
	}
	while(m_redo.size()>0)
	{
		m_redo.back()->Release();
		delete m_redo.back();
		m_redo.pop_back();
	}

	if(m_Mask != -2)
		display->RemoveTexture(m_Mask);
	if(m_Props.Brush1 != -2)
		display->RemoveTexture(m_Props.Brush1);
	if(m_Props.Brush2 != -2)
		display->RemoveTexture(m_Props.Brush2);
	for(int i = 0; i < layers.size(); i++){
		if(layers.at(i))
			display->RemoveTexture(layers.at(i));
	}
	if(m_TempCanvas != -2)
		display->RemoveTexture(m_TempCanvas);
	if(m_DisplayStep != -2)
		display->RemoveTexture(m_DisplayStep);
	if(m_Background != -2)
		display->RemoveTexture(m_Background);
	if(m_CanvasPromoted != -2)
		display->RemoveTexture(m_CanvasPromoted);
	for(map<int, int>::iterator j = m_shapes.begin(); j!= m_shapes.end(); j++){
		display->RemoveTexture((*j).second);
	}

	display->SetTexture(-1);
	display->SetRenderTarget(-1);

	switch(m_pImageEditor->m_app->game_information.sampler) {
	case CApplication::GameInformation::sample_point:
		display->SetSamplerState(D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		break;
	case CApplication::GameInformation::sample_linear:
		display->SetSamplerState(D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		break;
	}
}

void CPictureEditor::SetEditCollision( bool val )
{
	bEditingCollision = val;
}