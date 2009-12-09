//////////////////////////////////////////////
//	CSDLDisplay.h

#pragma once

#include "SDLGFX\SDL_gfxPrimitives.h"

// Magenta transparent colour
#define CD3DDISPLAY_TRANSPARENT 0xFFFF00FF

// Render to main screen
#define RENDERTARGET_DISPLAY -1
#define TEXTURE_NONE -1

// Make a system color in to an opaque D3D color
#define D3D_SYSCOLOR(c) (0xFF000000 | g_PaintManager->GetColor(c))
#define D3D_MAKECOLOR(c) (0xFF000000 | (c))
#define D3D_CONVERTCOLORREF(c) (0xFF000000 | (GetRValue(c) << 16) | (GetGValue(c) << 8) | GetBValue(c))
#define GETALPHAFLOAT(c) ((float)((c>>24)/255.0))

#define D3D_CONVERTCOLORREFALPHA(c) (c&0xFF000000 |    ((int)(GETALPHAFLOAT(c) * GetRValue(c))<< 16) | ((int)(GETALPHAFLOAT(c) * GetGValue(c))<< 8) |((int)(GETALPHAFLOAT(c) * GetBValue(c))) )   
#define D3D_CONVERTCOLORREFALPHAPRE(c) (c&0xFF000000 | (GetRValue(c) << 16) | (GetGValue(c) << 8) | GetBValue(c))
#define D3D_ARGB(a,r,g,b) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))
#define D3D_RGB(r,g,b) D3D_ARGB(0xFF, r,g,b)
#define D3D_A(a) ((a) >> 24)
#define D3D_R(r) (((r) & 0x00FF0000) >> 16)
#define D3D_G(g) (((g) & 0x0000FF00) >> 8)
#define D3D_B(b) (((b) & 0x000000FF))

// Degree to radian conversion
#define Rad(a) float((a) / 57.29577951308f)
#define Deg(a) float((a) * 57.29577951308f)

// Custom vertex
#ifdef USE_3D_CORDS
struct TLVERTEX
{
    float x;
    float y;
    float z;
    D3DCOLOR colour;
    float u;
    float v;
};
#else
struct TLVERTEX
{
    float x;
    float y;
    float z;
    float rhw;
    D3DCOLOR colour;
    float u;
    float v;
};
#endif


// Floating point co-ordinates
// (redef from ocidl.h)
//struct POINTF
//{
//	float x;
//	float y;
//};

// Floating point rect
struct RECTF
{
	float left;
	float top;
	float right;
	float bottom;
};

// SetRectF
void SetRectF(RECTF* prc, float xLeft, float yTop, float xRight, float yBottom);

// Gradient direction
enum Direction {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};

// FPS mode
enum CD3DFPSMode {
	CD3DFPS_VSYNCED,		// Synchronised to screen refresh
	CD3DFPS_UNLIMITED,		// Uncapped fps (no waiting)
	CD3DFPS_FIXED			// Fixed FPS rate (ugly)
};

class CSDLTexture
{
public:
	SDL_Surface* pImage;

	void Attach()
	{		
		GLfloat coord[4];
		
		imgwidth  = static_cast<float>(pImage->w);
		imgheight = static_cast<float>(pImage->h);

		texwidth  = static_cast<float>(pImage->w);
		texheight = static_cast<float>(pImage->h);

//		rTexID = SurfaceToTexture(pImage, coord);
//		rTexMinX = coord[0];
//		rTexMinY = coord[1];
//		rTexMaxX = coord[2];
//		rTexMaxY = coord[3];
	}

	GLfloat rTexMinX;
	GLfloat rTexMinY;
	GLfloat rTexMaxX;
	GLfloat rTexMaxY;
	unsigned int rTexID;

	// Image width/height
	GLfloat imgwidth;
	GLfloat imgheight;

	GLfloat texwidth;
	GLfloat texheight;
};

//////////////////////////////////////////////
// QUADRILATERAL CLASS
// Defines four co-ordinates to describe a transformed texture.
//
// i.e. four points for resizing and stretching etc
//		p1 ******** p2
//		   *      *
//      p3 ******** p4
//	All transformations are relative to p1; p1 never moves.
class Quadrilateral
{
public:
	Quadrilateral()
	{
		width = 0;
		height = 0;
	}
	POINTF p1;		// Top left; origin; remains same throughout transformations
	POINTF p2;		// Top right
	POINTF p3;		// Bottom left
	POINTF p4;		// Bottom right

	int width;  // used by davo to clamp uv
	int height; // used by davo to clamp uv
	// Create from a stretched rect
	void Stretched(RECTF& rc, float xf, float yf);

	// Create from a rotated rect.  Angle in degrees
	void Rotated(RECTF& rc, float angle);

	// Create from a stretched then rotated rect
	void StretchRotated(RECTF& rc, float xf, float yf, float angle);

	// Create from rect
	void operator=(RECTF rc);
	void operator=(RECT rc);
};

//////////////////////////////////////////////
// CSDLDISPLAYFONT
// Wrapper helper class for SDL fonts
class CSDLDisplayFont {
public:
	CSDLDisplayFont();
	~CSDLDisplayFont();

	void Destroy();

	friend class CSDLDisplay;
};

//////////////////////////////////////////////
// CD3DDISPLAY
// Hardware accelerated 2D DirectX 9.0c display rendering engine.

// Function typedefs for loading from dlls
typedef IDirect3D9* (WINAPI *LPDIRECT3DCREATE9)(UINT SDKVersion);

#define CD3DDISPLAY_VERSION			1000
#define CD3DDISPLAY_VERSION_STR		"1.0"
#define CD3DDISPLAY_VERSION_FULL	"Construct DirectX 9 2D Display Engine v1.0"

// Batch definitions
#define CD3D_MAX_BATCH_SIZE 2000	// Maximum number of vertices for a batch
#define CD3D_MAX_INDEX_SIZE	3000	// 2000 quad vertices will require 3000 indices

// Batch types
enum CD3DBATCH {
	BATCH_POINTS,
	BATCH_LINES,
	BATCH_LINESTRIP,
	BATCH_BOXES,
	BATCH_QUADS
};

class CSDLDisplay {
public:

	friend class CLayoutEditor;
	///////////////
	// Constructor; destructor
	CSDLDisplay();
	~CSDLDisplay();

	///////////////
	// Device level control
	bool	Init();
	bool	Close();
	void	Reset();
	void	SetupDevice();

	SDL_Surface* screen;

	///////////////
	// Display control functions
	void	ClearRT(D3DCOLOR color = 0x00000000);	// Clear render target
	void	BeginScene();							// Begin drawing operations
	void	EndScene();								// End drawing operations
	void	Present(RECT* pRec = NULL, HWND hWndOverride = NULL);

	///////////////
	// Texture control functions
	// Adding textures from file or resource: loads the texture to an internal store.  Returns
	// a texture index (an integer) to the texture in the store, which you must then use
	// to reference the texture.
	int				AddTextureRT(int w, int h, D3DFORMAT format = D3DFMT_A8R8G8B8);		// Add a custom, render-target texture
	int				AddTextureFromFile(const char* filename);
	int				AddTextureFromFileInMemory(LPCVOID pData, UINT len);
	int				AddTextureFromBuffer(BYTE *buffer, long size);
	int				AddTextureFromResource(HMODULE hModule, LPCTSTR pResource);
	void			Premultiply(int& iTexture);
	void			ConvertToRT(int& iTexture);
	void			ClearTextures();				// Clear all stored textures
	CSDLTexture		TextureAt(int index);		// Get texture pointer at index
	void			SetTexture(int index);			// Set current drawing texture
	int				GetTexture();
	CSDLTexture*	TexInfo(int index);				// Get TextureInfo pointer at (shortcut)
	bool			ResizeTextureRT(int& iTexture, int w, int h);
	void			RemoveTexture(int iTexture);	// Delete texture

	// Render targets (to render to a texture)
	bool			SetRenderTarget(int iTexture);\
	int				GetRenderTarget();

	// States
	void			SetRenderState(D3DRENDERSTATETYPE rstype, UINT value);
	unsigned long	GetRenderState(D3DRENDERSTATETYPE rstype);
	void			SetSamplerState(D3DSAMPLERSTATETYPE sstype, UINT value);
	unsigned long	GetSamplerState(D3DSAMPLERSTATETYPE sstype);

	///////////////
	// Drawing operations
	void	CreateFont(const char* fontface, unsigned int pxSize, bool italics, bool bold, CSDLDisplayFont* pFont);
	void	DrawText(CSDLDisplayFont& font, const char* text, RECTF& rc, D3DCOLOR color, DWORD format);
	void	DrawText(CSDLDisplayFont& font, const char* text, RECT& rc, D3DCOLOR color, DWORD format);

	// Primitive drawing operations
	void	Fill(const RECTF& rc, D3DCOLOR color);
	void	Fill(const RECT& rc, D3DCOLOR color);
	void	Fill(int x, int y, int w, int h, D3DCOLOR color);
	void	FillGradient(const RECTF& rc, D3DCOLOR color1, D3DCOLOR color2, Direction direction);
	void	FillGradient(const RECT& rc, D3DCOLOR color1, D3DCOLOR color2, Direction direction);
	void	FillGradient(int x, int y, int w, int h, D3DCOLOR color1, D3DCOLOR color2, Direction direction);
	
	// Primitive batching
	void	BeginBatch(CD3DBATCH type, int num);
	void	EndBatch();

	void	Point(float x, float y, D3DCOLOR color);
	void	Line(float x1, float y1, float x2, float y2, D3DCOLOR color);
	void	Line(float x1, float y1, float x2, float y2, D3DCOLOR color1, D3DCOLOR color2);
	void	Box(int x, int y, int w, int h, D3DCOLOR color);
	void	Box(RECTF rc, D3DCOLOR color);
	void	Box(RECT rc, D3DCOLOR color);
	void	Quad(Quadrilateral q, D3DCOLOR color);

	// Blit current texture to x,y; optional stretching and rotating
	void	Blit(float x, float y, float xf = 1.0f, float yf = 1.0f, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, const RECTF* uv = NULL);
	void	Blit(float x, float y, int w, int h, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, const RECTF* uv = NULL);
	void	Blit(RECTF& rc, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, const RECTF* uv = NULL);
	void	Blit(Quadrilateral& quad, D3DCOLOR filter = 0xFFFFFFFF, const RECTF* uv = NULL);

	void	Blit(float x, float y, float xf, float yf, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv = NULL);
	void	Blit(float x, float y, int w, int h, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv = NULL);
	void	Blit(RECTF& rc, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv = NULL);	
	
	// Backbuffer overriding
	void	SetBackBuffer(LPDIRECT3DSURFACE9 pSurf = NULL);

	void	Setup2DMode();

	void ReplaceTexture(int oldTex, int newTex);

	// Global filter: perform full screen tinting.  Deprecated.
	// Format: 0xAARRGGBB (alpha, red, green, blue)
	// E.g. 0xFFFF0000 is normal transparency but only red colours.
	// 0xFF888888 is normal transparency at half brightness.
	void	SetGlobalFilter(D3DCOLOR color);
	void	SetGlobalFilter(unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue);
	D3DCOLOR GetGlobalFilter();

	// Get private data
	HWND			  GetHwnd();

	// Version
	int Version() { return CD3DDISPLAY_VERSION; }
	void VersionStr(char* str) { strcpy_s(str, strlen(str), CD3DDISPLAY_VERSION_STR); }
	void VersionFull(char* str) { strcpy_s(str, strlen(str), CD3DDISPLAY_VERSION_FULL); }

	// Mix filters
	D3DCOLOR MixFilters(D3DCOLOR f1, D3DCOLOR f2);

	friend class CRuntime;
	friend class CPictureEditor;

private:

	HWND					hWnd;			// hwnd drawing to

public:
	// TextureInfo store
	map<int, CSDLTexture>		textures;
	int							curTextureId;

public:
	
	CSDLTexture*				GetTexture(int index);
	
private:
	// Current texture number
	int						curTexture;

	// Backbuffer width/height
	int bbWidth, bbHeight;

	int renderTex;

	// BlitTex: blits according to texture dimensions and not image dimentions like
	// the ordinary Blit functions.  Called by the Blit functions.
	void	BlitTex(Quadrilateral& q, D3DCOLOR filter, const RECTF* uv = NULL);
	void	BlitTex(Quadrilateral& q, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv = NULL);

	// Global filter
	D3DCOLOR				globalcolor;

	// Batching
	int batchIndex;
	CD3DBATCH batchType;
	bool batching;
	TLVERTEX* batchVertices;
	int batchCount;

	// Indexing
	unsigned short* indexPtr;

	int temp;

public:
	// Transformations
	void Update3dCoords(bool translate = true);
	float trans_PanX;
	float trans_PanY;
	float trans_Angle;
	float trans_ScaleX;
	float trans_ScaleY;
};

// For throwing
class CD3DError {
public:
	CD3DError();
	CD3DError(HRESULT _hr);
	CD3DError(CString msg);

	HRESULT hr;
	bool isHr;

	CString errStr;

	CString GetString();
};

#define COLOR_BLUE(a) (BYTE)a
#define COLOR_GREEN(a) (BYTE)(a>>8)
#define COLOR_RED(a) (BYTE)(a>>16)
#define COLOR_ALPHA(a) (BYTE)(a>>24)

#define MULTIPLYALPHA(a) D3DCOLOR_ARGB(COLOR_ALPHA(a),COLOR_RED(a)*COLOR_ALPHA(a)>>8,COLOR_GREEN(a)*COLOR_ALPHA(a)>>8,COLOR_BLUE(a)*COLOR_ALPHA(a)>>8)

#define DIVIDEALPHA(a) COLOR_ALPHA(a)==0?0:D3DCOLOR_ARGB(COLOR_ALPHA(a),COLOR_RED(a)>>8/COLOR_ALPHA(a),COLOR_GREEN(a)>>8/COLOR_ALPHA(a,COLOR_BLUE(a)>>8/COLOR_ALPHA(a))