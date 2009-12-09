
//////////////////////////////////////////////
//	CDisplay.h
//
//	Copyright (c) Ashley Gullen 2006

//////////////////////////////////////////////
// DEFINITIONS AND DATA STRUCTURES

// Vertex format
const DWORD D3DFVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

// Magenta transparent colour
#define CD3DDISPLAY_TRANSPARENT 0xFFFF00FF

// Render to main screen
#define RENDERTARGET_DISPLAY -1
#define TEXTURE_NONE -1

// Make a system color in to an opaque D3D color
#define D3D_SYSCOLOR(c) (0xFF000000 | GetSysColor(c))
#define D3D_MAKECOLOR(c) (0xFF000000 | (c))
#define D3D_CONVERTCOLORREF(c) (0xFF000000 | (GetRValue(c) << 16) | (GetGValue(c) << 8) | GetBValue(c))

// Degree to radian conversion
#define Rad(a) float((a) / 57.29577951308f)
#define Deg(a) float((a) * 57.29577951308f)

// Custom vertex
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

// Pops up a message box for debugging:
//		Str1: val1
//		Str2: val2
void DbgShowInts(char* str1, int val1, char* str2, int val2);

// TextureInfo struct: describes a texture
struct TextureInfo {

	// Width and height of hardware-level texture (in powers of 2 eg. 256x256)
	int texwidth;
	int texheight;

	// Width and height of actual image on the texture (anything, e.g. 56x78)
	int imgwidth;
	int imgheight;

	// The texture
	IDirect3DTexture9* texture;

	// Can be set as a render target?
	bool rendertarget;

	// Format, if rendertarget set
	D3DFORMAT format;

	// Texture to image ratios
	float xf;
	float yf;
};

// Effect
struct EffectInfo {
	ID3DXEffect* pEffect;
	D3DXHANDLE fxTexture;
	D3DXHANDLE fxFrameCounter;
	D3DXHANDLE fxOriginalTexture;
	D3DXHANDLE fxxPixToTex;
	D3DXHANDLE fxyPixToTex;
	D3DXHANDLE fxTexWidth;
	D3DXHANDLE fxTexHeight;
	D3DXHANDLE fxParamTex;

	RECTF		region;
	bool		hasRegion;
};

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
	CD3DFPS_UNLIMITED,		// Uncapped fps (continuous loop)
	CD3DFPS_FIXED			// Fixed FPS rate (unsupported)
};

// CD3DDisplaySetup: initialisation parameters for CD3DDisplay::Init()
struct CD3DDisplaySetup
{
	HWND hWnd;				// hWnd to window to use
	HWND hFocusWnd;			// Focus hwnd
	D3DFORMAT resFormat;	// Must be D3DFMT_A8R8G8B8
	BOOL bFullscreen;		// Fullscreen change resolution mode
	int resWidth;			// Width of fullscreen resolution if bFullscreen is true
	int resHeight;			// Height of fullscreen resolution if bFullscreen is true
	CD3DFPSMode fpsMode;	// FPS mode (Vsynced, unlimited, fixed)
	int fps;				// If CD3DFPS_FIXED selected, the FPS rate
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
	POINTF p1;		// Top left; origin; remains same throughout transformations
	POINTF p2;		// Top right
	POINTF p3;		// Bottom left
	POINTF p4;		// Bottom right

	// Create from a stretched rect
	void Stretched(RECTF& rc, float xf, float yf);

	// Create from a rotated rect.  Angle in degrees
	void Rotated(RECTF& rc, float angle);

	// Create from a stretched then rotated rect
	void StretchRotated(RECTF& rc, float xf, float yf, float angle);

	// Create from rect
	operator=(RECTF rc);
	operator=(RECT rc);
};

//////////////////////////////////////////////
// CD3DDISPLAYFONT
// Wrapper helper class for DirectX fonts
class CD3DDisplayFont {
public:
	CD3DDisplayFont();
	~CD3DDisplayFont();

	void Destroy();

	friend class CD3DDisplay;

private:
	ID3DXFont* d3dxfont;
};

//////////////////////////////////////////////
// CD3DDISPLAY
// Hardware accelerated 2D DirectX 9.0c display rendering engine.

// Function typedefs for loading from dlls
typedef IDirect3D9* (WINAPI *LPDIRECT3DCREATE9)(UINT SDKVersion);

#define CD3DDISPLAY_VERSION			950
#define CD3DDISPLAY_VERSION_STR		"0.95a"
#define CD3DDISPLAY_VERSION_FULL	"Construct DirectX 9 2D Display Engine v0.95a"

// Batch definitions
#define CD3D_MAX_BATCH_SIZE 10000	// Maximum number of vertices for a batch

// Batch types
enum CD3DBATCH {
	BATCH_POINTS,
	BATCH_LINES,
	BATCH_LINESTRIP,
	BATCH_BOXES,
	BATCH_QUADS
};

class CD3DDisplay {
public:

	///////////////
	// Constructor; destructor
	CD3DDisplay();
	~CD3DDisplay();

	///////////////
	// Initialise; close
	bool	Init(CD3DDisplaySetup* settings);
	bool	Close();

	///////////////
	// Display control functions
	void	ClearRT(D3DCOLOR color = 0x00000000);	// Clear render target
	void	BeginScene();		// Begin drawing operations
	void	EndScene();			// End drawing operations
	void	Present(RECT* pRec = NULL);

	///////////////
	// Texture control functions
	// Adding textures from file or resource: loads the texture to an internal store.  Returns
	// a texture index (an integer) to the texture in the store, which you must then use
	// to reference the texture.
	int				AddTextureRT(int w, int h, D3DFORMAT format = D3DFMT_A8R8G8B8);		// Add a custom, render-target texture
	int				AddTextureFromFile(char* filename);
	int				AddTextureFromResource(HMODULE hModule, LPCTSTR pResource);
	void			ClearTextures();				// Clear all stored textures
	IDirect3DTexture9* TextureAt(int index);		// Get texture pointer at index
	void			SetTexture(int index);			// Set current drawing texture
	TextureInfo*	TexInfo(int index);				// Get TextureInfo pointer at (shortcut)
	bool			ResizeTextureRT(int iTexture, int w, int h);

	// Render targets (to render to a texture)
	bool			SetRenderTarget(int iTexture);

	///////////////
	// Effect control functions
	// Returns an effect index
	// Include functions for compiling effects?  (~600kb)
#ifdef CD3D_USE_HLSL_COMPILER
	int				AddEffectFromFile(char* str, RECTF* region = NULL);
#endif
	void			ClearEffects();
	//void			EnableSingleGlobalEffect(int iEffect);
	void			EnableGlobalEffects();
	void			DisableGlobalEffects();
	void			AddGlobalEffect(int iEffect);
	void			RemoveGlobalEffects();
	void			DoGlobalEffects();
	ID3DXEffect*	GetEffectPtr(int iEffect);
	void			SetEffectRect(int iEffect, RECTF* region);

	// States
	void			SetRenderState(D3DRENDERSTATETYPE rstype, UINT value);
	void			SetSamplerState(D3DSAMPLERSTATETYPE sstype, UINT value);

	///////////////
	// Drawing operations
	void	CreateFont(char* fontface, unsigned int pxSize, bool italics, bool bold, CD3DDisplayFont* pFont);
	void	DrawText(CD3DDisplayFont& font, const char* text, int x, int y, D3DCOLOR color);

	// Primitive drawing operations
	void	Fill(const RECT rc, D3DCOLOR color);
	void	Fill(int x, int y, int w, int h, D3DCOLOR color);
	void	FillGradient(RECT rc, D3DCOLOR color1, D3DCOLOR color2, Direction direction);
	void	FillGradient(int x, int y, int w, int h, D3DCOLOR color1, D3DCOLOR color2, Direction direction);
	
	// Primitive batching
	void	BeginBatch(CD3DBATCH type, int num);
	void	EndBatch();

	void	Point(int x, int y, D3DCOLOR color);
	void	Line(int x1, int y1, int x2, int y2, D3DCOLOR color);
	void	Line(int x1, int y1, int x2, int y2, D3DCOLOR color1, D3DCOLOR color2);
	void	Box(int x, int y, int w, int h, D3DCOLOR color);
	void	Box(const RECT rc, D3DCOLOR color);
	void	Quad(Quadrilateral q, D3DCOLOR color);

	// Blit current texture to x,y; optional stretching and rotating
	void	Blit(float x, float y, float xf = 1.0f, float yf = 1.0f, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, RECTF* uv = NULL);
	void	Blit(float x, float y, int w, int h, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, RECTF* uv = NULL);
	void	Blit(RECTF& rc, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, RECTF* uv = NULL);
	void	Blit(Quadrilateral& quad, D3DCOLOR filter = 0xFFFFFFFF, RECTF* uv = NULL);

	void	BlitEffect(int iEffect, float x, float y, float xf, float yf, float angle, D3DCOLOR filter = 0xFFFFFFFF);

	// Global filter: perform full screen tinting
	// Format: 0xAARRGGBB (alpha, red, green, blue)
	// E.g. 0xFFFF0000 is normal transparency but only red colours.
	// 0xFF888888 is normal transparency at half brightness.
	void	SetGlobalFilter(D3DCOLOR color);
	void	SetGlobalFilter(unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue);
	D3DCOLOR GetGlobalFilter();

	// Get private vars
	IDirect3DDevice9* GetD3DDevice();
	D3DCAPS9*		  GetD3DCaps();
	HWND			  GetHwnd();

	// Version
	int Version() { return CD3DDISPLAY_VERSION; }
	void VersionStr(char* str) { strcpy(str, CD3DDISPLAY_VERSION_STR); }
	void VersionFull(char* str) { strcpy(str, CD3DDISPLAY_VERSION_FULL); }

	// Mix filters
	D3DCOLOR MixFilters(D3DCOLOR f1, D3DCOLOR f2);

	// For testing new features
	void	TestRoutine();

	friend class CRuntime;

private:

	HWND					hWnd;			// hwnd drawing to
	IDirect3D9*				d3d;			// d3d interface
	IDirect3DDevice9*		d3dDevice;		// d3d device
	D3DCAPS9				d3dCaps;		// d3d capabilities
	D3DPRESENT_PARAMETERS	d3dPresent;		// d3d presentation parameters
	IDirect3DVertexBuffer9*	vertexBuffer;	// Vertex buffer
	IDirect3DVertexBuffer9* batchBuffer;	// Buffer for batch operations
	HRESULT					hr;				// for returns

	HMODULE					d3d9Dll;		// d3d9.dll

	// TextureInfo store
	vector<TextureInfo>		txi;
	// Current texture number
	int						curTexture;

	// EffectInfo store
	vector<EffectInfo>		fxi;
	// Current effect number
	int						curEffect;
	// Global effect enabled?
	bool					globalFx;
	// Global effect chain
	vector<int>				globalEffects;
	// Global effect rendertarget chain
	vector<int>				globalTextures;

	// Backbuffer width/height
	int bbWidth, bbHeight;
	
	float					frameCounter;

	// Render surface
	IDirect3DSurface9*		pDisplaySurface;

	// BlitTex: blits according to texture dimensions and not image dimentions like
	// the ordinary Blit functions.  Called by the Blit functions.
	void	BlitTex(Quadrilateral& q, D3DCOLOR filter, RECTF* uv = NULL);
	void	BlitTex(Quadrilateral& q, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, RECTF* uv = NULL);

	// Global filter
	D3DCOLOR				globalcolor;

	// Batching
	int batchIndex;
	CD3DBATCH batchType;
	bool batching;
	TLVERTEX* batchVertices;
	int batchCount;

	int temp;
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
