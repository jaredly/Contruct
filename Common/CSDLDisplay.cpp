#include "StdAfx.h"

#ifdef CONSTRUCT_SDL

#define m_mipmaplevels 0
#define m_mipmap_filter D3DX_FILTER_NONE

//////////////////////////////////////////////
// CSDLDisplay FUNCTIONS
#ifndef _CDISPLAY_CPP_
#define _CDISPLAY_CPP_

#ifndef APPRUNTIME

// Constructor
CSDLDisplay::CSDLDisplay()
{
	// Null default values
	hWnd = NULL; 
	curTextureId = bbWidth = bbHeight = 0;

	globalcolor = 0xFFFFFFFF;

	curTexture = -1;

	trans_PanX = 0;
	trans_PanY = 0;
	trans_Angle = 0;
	trans_ScaleX = 1;
	trans_ScaleY = 1;
}

// Destructor
CSDLDisplay::~CSDLDisplay()
{
	// Shut down
	Close();
}

// Initialise the SDL engine
bool CSDLDisplay::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
		exit(1);

	// Register SDL_Quit to be called at exit; makes sure things are cleaned up when we quit.
	atexit(SDL_Quit);

    //Successfully initalized Direct3D
    return true;
}

// Initialise the D3D Device
void CSDLDisplay::SetupDevice()
{
	// Not needed
}

// Reset the device
void CSDLDisplay::Reset()
{
	// Not needed
}

void CSDLDisplay::Setup2DMode()
{
	// Not needed
}

// Shut down SDL engine
bool CSDLDisplay::Close()
{
	// Release textures
	ClearTextures();

    return true;
}

// Begin drawing operations
void CSDLDisplay::BeginScene()
{

}

// Get a TextureInfo pointer from a handle
CSDLTexture* CSDLDisplay::GetTexture(int index)
{
	if (textures.find(index) != textures.end())
		return &textures[index];
	else {
		MessageBox(NULL, "An invalid texture was referenced.\n\nThis is a bug, please report it!", "CSDLDisplay", MB_OK | MB_ICONHAND);
		throw;
	}
}

// End drawing operations
void CSDLDisplay::EndScene()
{
	// Not needed
}

// Show frame
void CSDLDisplay::Present(RECT* pRec, HWND hWndOverride)
{
	// Not needed
}

// Add a texture which can be used as a rendertarget
int CSDLDisplay::AddTextureRT(int w, int h, D3DFORMAT format)
{
	return -1;

	// Not implemented
}

// Clear rendertarget
void CSDLDisplay::ClearRT(D3DCOLOR color)
{
	// Not needed
}

// Resize a rendertarget texture
bool CSDLDisplay::ResizeTextureRT(int& iTexture, int w, int h)
{
	return false;
}

// Add texture from disk file
int CSDLDisplay::AddTextureFromFile(const char* filename)
{
	SDL_Surface* temp = IMG_Load(filename);

	// Create alpha surface
	SDL_Surface* pNew = SDL_DisplayFormatAlpha(temp);
	SDL_FreeSurface(temp);

	// Alpha
	SDL_SetAlpha(pNew, SDL_SRCALPHA, 255);

	CSDLTexture Texture;
	Texture.pImage = pNew;
	Texture.Attach();

	// Add the texture immediately
	int textureHandle = textures.size();

	textures[textureHandle] = Texture;

	// Add the map entry (which texture ID this handle now maps to)
//	imageMap[imageHandle].iTexture = textureHandle;

	// Return texture index
	return curTextureId - 1;
}

int CSDLDisplay::AddTextureFromFileInMemory(LPCVOID pData, UINT len)
{
	// Return texture index
	return -1;

	// not implemented
}

// Add texture from file in a Data resource
int CSDLDisplay::AddTextureFromResource(HMODULE hModule, LPCTSTR pResource)
{
	return -1;
}

int CSDLDisplay::AddTextureFromBuffer(BYTE *buffer, long size)
{
	return -1;
}

// Release all textures
void CSDLDisplay::ClearTextures()
{
	textures.clear();
}

// Retrieve texture at index
CSDLTexture CSDLDisplay::TextureAt(int index)
{
	//return GetTexture(index)->texture;
	return *GetTexture(index);
}

// Set render target
bool CSDLDisplay::SetRenderTarget(int iTexture)
{
	return true; 
	//STLTODO
}

// Handle 2D display transformations using DirectX's 3D features
void CSDLDisplay::Update3dCoords(bool translate)
{
#ifdef USE_3D_CORDS
	
	int iTexture = GetRenderTarget();
	float Width = bbWidth;
	float Height = bbHeight;
	if (iTexture >= 0) 
	{
		TextureInfo* pTex = TexInfo(iTexture);
		
		 Width = pTex->texwidth;
		 Height = pTex->texheight;
	}
	float l = 0;
	float t = 0;
	float r = Width;
	float b = Height;
	if(translate)
	{
		r /= trans_ScaleX;
		b /= trans_ScaleY;
	}

	l += 0.5;
	t += 0.5;
	r += 0.5;
	b += 0.5;


	D3DXMATRIX Ortho2D; 
	D3DXMATRIX Identity;
	D3DXMATRIX Rotate;
	D3DXMatrixOrthoOffCenterRH(&Ortho2D, l, r, b, t, 0, 1);
	D3DXMatrixIdentity(&Identity);

	if(translate)
	{
		D3DXMatrixIdentity(&Rotate);
		D3DXMatrixRotationYawPitchRoll(&Rotate, 0, 0, Rad(trans_Angle));
		D3DXMatrixMultiply(&Ortho2D, &Rotate, &Ortho2D);
	}

	d3dDevice->SetTransform(D3DTS_PROJECTION, &Ortho2D);
	d3dDevice->SetTransform(D3DTS_WORLD, &Identity);
	d3dDevice->SetTransform(D3DTS_VIEW, &Identity);
	
#endif

}

int CSDLDisplay::GetRenderTarget()
{
	return renderTex;
}

void CSDLDisplay::Blit(float x, float y, int w, int h, float angle, D3DCOLOR filter, const RECTF* uv)
{
	// Use the rect blit.
	RECTF rc;
	SetRectF(&rc, x, y, x+w, y+h);
	Blit(rc, angle, filter, uv);
}

void CSDLDisplay::Blit(float x, float y, float xf, float yf, float angle, D3DCOLOR filter, const RECTF* uv)
{
	float w = xf * textures[curTexture].imgwidth;
	float h = yf * textures[curTexture].imgheight;

	SDL_Rect RectT;	
	RectT.x = (Sint16)x;
	RectT.y = (Sint16)y; 
	RectT.w = (Sint16)w; 
	RectT.h = (Sint16)h; 

	if (angle != 0.0f || xf != 1 || yf != 1)
	{
		SDL_Surface* pTemp = rotozoomSurfaceXY(textures[curTexture].pImage, angle, 1, xf, yf);
		SDL_BlitSurface(pTemp,  NULL, screen, &RectT);
		SDL_FreeSurface(pTemp);
	}

	else
		SDL_BlitSurface(textures[curTexture].pImage,  NULL, screen, &RectT);
}

void CSDLDisplay::Blit(float x, float y, int w, int h, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv)
{
	SDL_Rect RectT;	
	RectT.x = (Sint16)x;
	RectT.y = (Sint16)y; 
	RectT.w = (Sint16)w; 
	RectT.h = (Sint16)h; 

	if (angle != 0.0f || textures[curTexture].imgwidth != w || textures[curTexture].imgheight != h)
	{
		float xSize = w / textures[curTexture].imgwidth;
		float ySize = h / textures[curTexture].imgheight;

		SDL_Surface* pTemp = rotozoomSurfaceXY(textures[curTexture].pImage, angle, 1, xSize, ySize);
		SDL_BlitSurface(pTemp,  NULL, screen, &RectT);
		SDL_FreeSurface(pTemp);
	}

	else
		SDL_BlitSurface(textures[curTexture].pImage,  NULL, screen, &RectT);
}

void CSDLDisplay::Blit(float x, float y, float xf, float yf, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv)
{
	float w = xf * textures[curTexture].imgwidth;
	float h = yf * textures[curTexture].imgheight;

	SDL_Rect RectT;	
	RectT.x = (Sint16)x;
	RectT.y = (Sint16)y; 
	RectT.w = (Sint16)w; 
	RectT.h = (Sint16)h; 

	if (angle != 0.0f || xf != 1 || yf != 1)
	{
		SDL_Surface* pTemp = rotozoomSurfaceXY(textures[curTexture].pImage, angle, 1, xf, yf);
		SDL_BlitSurface(pTemp,  NULL, screen, &RectT);
		SDL_FreeSurface(pTemp);
	}

	else
		SDL_BlitSurface(textures[curTexture].pImage,  NULL, screen, &RectT);
}

void CSDLDisplay::Blit(RECTF& rc, float angle, D3DCOLOR filter,D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv)
{
	float w = rc.right - rc.left;
	float h = rc.bottom - rc.top;

	SDL_Rect RectT;	
	RectT.x = (Sint16)rc.left;
	RectT.y = (Sint16)rc.top; 
	RectT.w = (Sint16)w; 
	RectT.h = (Sint16)h; 

	if (angle != 0.0f || textures[curTexture].imgwidth != w || textures[curTexture].imgheight != h)
	{
		float xSize = w / textures[curTexture].imgwidth;
		float ySize = h / textures[curTexture].imgheight;

		SDL_Surface* pTemp = rotozoomSurfaceXY(textures[curTexture].pImage, angle, 1, xSize, ySize);
		SDL_BlitSurface(pTemp,  NULL, screen, &RectT);
		SDL_FreeSurface(pTemp);
	}

	else
		SDL_BlitSurface(textures[curTexture].pImage,  NULL, screen, &RectT);
}

void CSDLDisplay::Blit(RECTF& rc, float angle, D3DCOLOR filter, const RECTF* uv)
{
	float w = rc.right - rc.left;
	float h = rc.bottom - rc.top;

	SDL_Rect RectT;	
	RectT.x = (Sint16)rc.left;
	RectT.y = (Sint16)rc.top; 
	RectT.w = (Sint16)w; 
	RectT.h = (Sint16)h; 

	if (angle != 0.0f || textures[curTexture].imgwidth != w || textures[curTexture].imgheight != h)
	{
		float xSize = w / textures[curTexture].imgwidth;
		float ySize = h / textures[curTexture].imgheight;

		SDL_Surface* pTemp = rotozoomSurfaceXY(textures[curTexture].pImage, angle, 1, xSize, ySize);
		SDL_BlitSurface(pTemp,  NULL, screen, &RectT);
		SDL_FreeSurface(pTemp);
	}

	else
		SDL_BlitSurface(textures[curTexture].pImage,  NULL, screen, &RectT);
}

inline void CSDLDisplay::Blit(Quadrilateral& quad, D3DCOLOR filter, const RECTF* uv)
{

}

// Mix filters: e.g. if f1 is 50% red and f2 is 60% red, the result is a 30% red filter
D3DCOLOR CSDLDisplay::MixFilters(D3DCOLOR f1, D3DCOLOR f2)
{
	return f1;
}

inline void SetVertex(TLVERTEX* vertex, D3DCOLOR color, float x, float y, float u, float v)
{

}

// Blit a texture-dimensioned image with image-wide filtering
inline void CSDLDisplay::BlitTex(Quadrilateral& q, D3DCOLOR filter, const RECTF* uv)
{
	BlitTex(q, filter, filter, filter, filter, uv);
}

void CSDLDisplay::BlitTex(Quadrilateral& q, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv)
{

}

// Set current texture for Blit operations
void CSDLDisplay::SetTexture(int index)
{
	curTexture = index;
}

int CSDLDisplay::GetTexture()
{
	return curTexture;
}

// Delete a texture and invalidate its reference
void CSDLDisplay::RemoveTexture(int iTexture)
{
	CSDLTexture tex = textures[iTexture];

	SDL_FreeSurface(tex.pImage);

	textures.erase(iTexture);
}

// Shortcut to get TextureInfo pointer from index
CSDLTexture* CSDLDisplay::TexInfo(int index)
{
	return GetTexture(index);
}

// Premultiply a texture
void CSDLDisplay::Premultiply(int& iTexture)
{
/*	int w = TexInfo(iTexture).imgwidth;
	int h = TexInfo(iTexture).imgheight;
	int oldRT = renderTex; // old rendertarget
	int oldTex = curTexture; // old texture

	int newTex = AddTextureRT(w,h);

	SetTexture(iTexture);
	SetRenderTarget(newTex);
	BeginScene();
	ClearRT();

	SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
	SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	Blit(0,0);

	SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA);
	SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	Blit(0,0);

	// Reset to std drawing
	SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (oldRT == iTexture)
		SetRenderTarget(newTex);
	else
		SetRenderTarget(oldRT);

	if (oldTex == iTexture)
		SetTexture(newTex);
	else
		SetTexture(oldTex);

	// Remove old texture and update parameter
	RemoveTexture(iTexture);
	iTexture = newTex;

	EndScene();
//	SetRenderTarget(-1);
*/
}

void CSDLDisplay::ReplaceTexture(int oldTex, int newTex)
{
	CSDLTexture* previous = GetTexture(oldTex);
	CSDLTexture* next = GetTexture(newTex);

	// Release old surface
	SDL_FreeSurface(previous->pImage);

	// Copy across texture info
	//previous = next;
	//SDLTODO

	textures.erase(newTex);
}

// Convert to RT
void CSDLDisplay::ConvertToRT(int& iTexture)
{
	// Not implemented
}

// Set global filter fx
void CSDLDisplay::SetGlobalFilter(D3DCOLOR color)
{
	globalcolor = color;
}

// Set global filter fx
void CSDLDisplay::SetGlobalFilter(unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue)
{
	globalcolor = (alpha << 24) + (red << 16) + (green << 8) + blue;
}

// CreateFont wrapper
void CSDLDisplay::CreateFont(const char* fontface, unsigned int pxSize, bool italics, bool bold, CSDLDisplayFont* pFont)
{
/*	D3DXCreateFont(d3dDevice, pxSize, 0, (bold ? FW_BOLD : FW_NORMAL), 0, italics,
				   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE,
				   fontface, &(pFont->d3dxfont));*/
}

void CSDLDisplay::DrawText(CSDLDisplayFont& font, const char* text, RECT& rc, D3DCOLOR color, DWORD format)
{
	/*RECTF rcf;
	rcf.left = rc.left;
	rcf.top = rc.top;
	rcf.right = rc.right;
	rcf.bottom = rc.bottom;
	DrawText(font, text, rcf, color, format);*/
}

// Render text
void CSDLDisplay::DrawText(CSDLDisplayFont& font, const char* text, RECTF& rc, D3DCOLOR color, DWORD format)
{
/*    m_pD3DXSprite->Begin( D3DXSPRITE_OBJECTSPACE|D3DXSPRITE_ALPHABLEND );
	RECT rect;
	SetRect(&rect, rc.left, rc.top, rc.right, rc.bottom);
	// Sadley you are limited to a rect that is integer based with drawtext :(
	// to fix this we do a little hack to prevent blurring
	font.d3dxfont->DrawText(m_pD3DXSprite, text, -1, &rect, format, MixFilters(globalcolor, color));
	m_pD3DXSprite->End();*/
}

// Fill rect
void CSDLDisplay::Fill(const RECTF& rc, D3DCOLOR color)
{
	float x, y, w, h;
	x = rc.left;
	y = rc.top;
	w = rc.right - rc.left;
	h = rc.bottom - rc.top;

	float i, j;
	for (i = 0; i < h; i++)
	{
		if ((y + i) >= 0 && (y + i) < h)
		{
			float len = w;
			float xofs = x;

			if (xofs < 0)
			{
				len += xofs;
				xofs = 0;
			}

			if (xofs + len >= w)
				len -= (xofs + len) - w;

			float ofs = (i + y) * (screen->pitch / 4) + xofs;

			for (j = 0; j < len; j++)
				((unsigned int*)screen->pixels)[(int)(ofs + j)] = color;
		}
	}	
}

void CSDLDisplay::Fill(const RECT& rc, D3DCOLOR color)
{
	RECTF rcf;
	rcf.left = rc.left;
	rcf.top = rc.top;
	rcf.right = rc.right;
	rcf.bottom = rc.bottom;
	Fill(rcf, color);
}

// Fill rect overload
void CSDLDisplay::Fill(int x, int y, int w, int h, D3DCOLOR color)
{
	// No texture for flood fill
	SetTexture(-1);

	RECTF rc;
	SetRectF(&rc, x, y, x + w, y + h);

	Quadrilateral q;
	q = rc;

	BlitTex(q, MixFilters(color, globalcolor));
}

void CSDLDisplay::FillGradient(const RECT& rc, D3DCOLOR color1, D3DCOLOR color2, Direction direction)
{
	RECTF rcf;
	rcf.left = rc.left;
	rcf.top = rc.top;
	rcf.right = rc.right;
	rcf.bottom = rc.bottom;
	FillGradient(rcf, color1, color2, direction);
}

// Fill a rect with a gradient, from color1 to color2
void CSDLDisplay::FillGradient(const RECTF& rc, D3DCOLOR color1, D3DCOLOR color2, Direction direction)
{
	// No texture
	/*SetTexture(-1);

	color1 = MixFilters(color1, globalcolor);
	color2 = MixFilters(color2, globalcolor);

	Quadrilateral q;
	q = rc;

	switch(direction) {
	case DIR_UP:
		BlitTex(q, color2, color2, color1, color1);
		break;
	case DIR_DOWN:
		BlitTex(q, color1, color1, color2, color2);
		break;
	case DIR_LEFT:
		BlitTex(q, color2, color1, color2, color1);
		break;
	case DIR_RIGHT:
		BlitTex(q, color1, color2, color1, color2);
		break;
	}*/

	Fill(rc, color1);
}

// Fill gradient with x,y,w,h
void CSDLDisplay::FillGradient(int x, int y, int w, int h, D3DCOLOR color1, D3DCOLOR color2, Direction direction)
{
	RECTF rc;
	SetRectF(&rc, x, y, x + w, y + h);
	FillGradient(rc, color1, color2, direction);
}

// Set a RECTF
void SetRectF(RECTF* prc, float xLeft, float yTop, float xRight, float yBottom)
{
	if (prc != NULL) {
		prc->left = xLeft;
		prc->top = yTop;
		prc->right = xRight;
		prc->bottom = yBottom;
	}
}

// Draw a point
void CSDLDisplay::Point(float x, float y, D3DCOLOR color)
{

}

// Draw a line
void CSDLDisplay::Line(float x1, float y1, float x2, float y2, D3DCOLOR color1, D3DCOLOR color2)
{

}

void CSDLDisplay::Line(float x1, float y1, float x2, float y2, D3DCOLOR color)
{
	Line(x1, y1, x2, y2, color, color);
}

// Draw a box of lines
void CSDLDisplay::Box(int x, int y, int w, int h, D3DCOLOR color)
{
	RECTF rc;
	SetRectF(&rc, x, y, x+w, y+h);
	Box(rc, color);
}

void CSDLDisplay::Box(RECT rc, D3DCOLOR color)
{
	RECTF rcf;
	rcf.left = rc.left;
	rcf.top = rc.top;
	rcf.right = rc.right;
	rcf.bottom = rc.bottom;
	Box(rcf, color);
}

// Rect overload
void CSDLDisplay::Box(RECTF rc, D3DCOLOR color)
{

}

// Draw the outlines of a Quad
void CSDLDisplay::Quad(Quadrilateral q, D3DCOLOR color)
{

}

// Get private vars
D3DCOLOR CSDLDisplay::GetGlobalFilter()
{
	return globalcolor;
}

HWND CSDLDisplay::GetHwnd()
{
	return hWnd;
}

void CSDLDisplay::SetBackBuffer(LPDIRECT3DSURFACE9 pSurf)
{

}

unsigned long CSDLDisplay::GetRenderState(D3DRENDERSTATETYPE rstype)
{
	return 0;
}

// States
void CSDLDisplay::SetRenderState(D3DRENDERSTATETYPE rstype, UINT value)
{

}

void CSDLDisplay::SetSamplerState(D3DSAMPLERSTATETYPE sstype, UINT value)
{

}

unsigned long CSDLDisplay::GetSamplerState(D3DSAMPLERSTATETYPE sstype)
{
	return 0;
}

// Batching
// Begin batch: lock the batch buffer
void CSDLDisplay::BeginBatch(CD3DBATCH type, int num)
{

}

// End batch
void CSDLDisplay::EndBatch()
{

}

//////////////////////////////////////////////
// CD3DError functions
CD3DError::CD3DError()
{
	hr = 0;
	isHr = false;
}

CD3DError::CD3DError(CString msg)
{
	hr = 0;
	isHr = false;
	errStr = msg;
}

CD3DError::CD3DError(HRESULT _hr)
{
	hr = _hr;
	isHr = true;
}

CString CD3DError::GetString()
{
	if (isHr) {
		switch (hr) {
		case D3D_OK:
			return "Universal Logic Fault: 1 = 0, please jump out your nearest wormhole";
		case D3DERR_DEVICELOST:
			return "The device has been lost";
		case D3DERR_DRIVERINTERNALERROR:
			return "There was a problem with your graphics drivers";
		case D3DERR_INVALIDCALL:
			return "An invalid call was made";
		case D3DXERR_INVALIDDATA:
			return "Invalid data";
		case D3DERR_OUTOFVIDEOMEMORY:
			{
				const float megabyte = 1024 * 1024;

				CString msg;

				return msg;
			}
		case E_FAIL:
			return "Unspecified error";
		case E_OUTOFMEMORY:
			return "Out of memory";
		case 0x8876086a:
			return "3D not available; your graphics card does not support this application's drawing mode.";
		default:
			return "Unknown error";
		}
	}
	else
		return errStr;
}

//////////////////////////////////////////////
// QUADRILATERAL FUNCTIONS

// Create from stretched floating point rect
void Quadrilateral::Stretched(RECTF& rc, float xf, float yf)
{
	// Set from rc
	operator = (rc);

	// Width and height
	float w = rc.right - rc.left;
	float h = rc.bottom - rc.top;

	// p1 does not move

	// stretch p2 relative to p1
	p2.x = p1.x + (w * xf);

	// stretch p3 relative to p1
	p3.y = p1.y + (h * yf);

	// stretch p4 relative to p1
	p4.x = p2.x;
	p4.y = p3.y;

	
	// quick flip changes
	/*if( xf < 0)
	{
		p1.x -= (w * xf);
		p2.x -= (w * xf);
		p3.x -= (w * xf);
		p4.x -= (w * xf);
	}
	if( yf < 0)
	{
		p1.y -= (w * yf);
		p2.y -= (w * yf);
		p3.y -= (w * yf);
		p4.y -= (w * yf);
	}*/


}

// Create from rotated floating point rect
void Quadrilateral::Rotated(RECTF& rc, float angle)
{
	// Set from rc
	operator = (rc);

	if (angle == 0)
		return;

	// Covert mathematical angle to radians and flip (for bearing)
	angle = Rad(-angle);

	float w = rc.right - rc.left;
	float h = rc.bottom - rc.top;
/*	float t = atan(w/h);
	float d = sqrt((w * w) + (h * h));	// diagonal distance

	// p1 does not move

	// rotate p2 about p1
	p2.x = p1.x + (w * sin(angle + Rad(90)));
	p2.y = p1.y + (w * cos(angle + Rad(90)));

	// rotate p3 about p1
	p3.x = p1.x + (h * sin(angle));
	p3.y = p1.y + (h * cos(angle));

	// rotate p4 about p1
	p4.x = p1.x + (d * sin(angle + t));
	p4.y = p1.y + (d * cos(angle + t));*/

	float sina = sin(angle);
	float cosa = cos(angle);

	p2.x = p1.x + w * cosa;
	p2.y = p1.y - w * sina;

	p3.x = p1.x + h * sina;
	p3.y = p1.y + h * cosa;

	p4.x = p1.x + w * cosa + h * sina;
	p4.y = p1.y - w * sina + h * cosa;


}

// Create from stretched then rotated floating point rect
void Quadrilateral::StretchRotated(RECTF& rc, float xf, float yf, float angle)
{
	// First create from stretch
	Stretched(rc, xf, yf);

	// Make new rect
	RECTF stretched;
	stretched.left = p1.x;
	stretched.top = p1.y;
	stretched.right = p4.x;
	stretched.bottom = p4.y;

	// Create from rotated
	if (angle != 0)
		Rotated(stretched, angle);
}

// Create from rectf
void Quadrilateral::operator =(RECTF rc)
{
	p1.x = rc.left;
	p1.y = rc.top;

	p2.x = rc.right;
	p2.y = rc.top;

	p3.x = rc.left;
	p3.y = rc.bottom;

	p4.x = rc.right;
	p4.y = rc.bottom;
}

// Create from rect
void Quadrilateral::operator =(RECT rc)
{
	p1.x = rc.left;
	p1.y = rc.top;

	p2.x = rc.right;
	p2.y = rc.top;

	p3.x = rc.left;
	p3.y = rc.bottom;

	p4.x = rc.right;
	p4.y = rc.bottom;
}

//////////////////////////////////////////////
// CSDLDisplayFONT FUNCTIONS
CSDLDisplayFont::CSDLDisplayFont()
{

}

CSDLDisplayFont::~CSDLDisplayFont()
{
}

void CSDLDisplayFont::Destroy()
{

}

#endif // APPRUNTIME


#endif // include once
#endif