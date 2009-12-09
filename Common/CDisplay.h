//////////////////////////////////////////////
//	CDisplay.h
//
//	Copyright (c) Ashley Gullen 2006

//////////////////////////////////////////////
// DEFINITIONS AND DATA STRUCTURES
#ifndef _CDISPLAY_H_
#define _CDISPLAY_H_

#define USE_3D_CORDS
//#define BATCH_OPTIMISE

// Vertex format
#ifdef USE_3D_CORDS
const DWORD D3DFVF_TLVERTEX = D3DFVF_XYZ    | D3DFVF_DIFFUSE | D3DFVF_TEX1;
#else
const DWORD D3DFVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
#endif

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
#define COLORREF_CONVERTD3D(c) (0x00FFFFFF & RGB(D3D_R(c), D3D_G(c), D3D_B(c)))

#define D3D_CONVERTCOLORREFALPHA(c) (c&0xFF000000 |    ((int)(GETALPHAFLOAT(c) * GetRValue(c))<< 16) | ((int)(GETALPHAFLOAT(c) * GetGValue(c))<< 8) |((int)(GETALPHAFLOAT(c) * GetBValue(c))) )   
#define D3D_CONVERTCOLORREFALPHAPRE(c) (c&0xFF000000 | (GetRValue(c) << 16) | (GetGValue(c) << 8) | GetBValue(c))
#define D3D_ARGB(a,r,g,b) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))
#define D3D_RGB(r,g,b) D3D_ARGB(0xFF, r,g,b)
#define D3D_A(a) ((a) >> 24)
#define D3D_R(r) (((r) & 0x00FF0000) >> 16)
#define D3D_G(g) (((g) & 0x0000FF00) >> 8)
#define D3D_B(b) (((b) & 0x000000FF))

// Degree to radian conversion
#define Rad(a) ((a) / 57.295779513082320876798154814105)
#define Deg(a) ((a) * 57.295779513082320876798154814105)

// 1mb chunk sizes
#define CDISPLAY_HEAP_CHUNK_SIZE 1000000

class CDisplayHeapChunk {
public:
	BYTE* mem;

	CDisplayHeapChunk() {
		mem = new BYTE[CDISPLAY_HEAP_CHUNK_SIZE];
	}

	CDisplayHeapChunk(const CDisplayHeapChunk& h) {
		mem = new BYTE[CDISPLAY_HEAP_CHUNK_SIZE];
	}

	~CDisplayHeapChunk() {
		delete[] mem;
	}
};

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

#define CD3D_NUM_RENDERSTATES 210
#define CD3D_NUM_TRANSFORMS   260
#define CD3D_NUM_SAMPLERSTATES 14

// SetRectF
void SetRectF(RECTF* prc, float xLeft, float yTop, float xRight, float yBottom);

// TextureInfo struct: describes a texture
class TextureInfo {
public:

	// Width and height of hardware-level texture (usually in powers of 2 eg. 256x256)
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

	int referenceCount;

	TextureInfo() : referenceCount(1) {}	// TextureInfo is instantiated in the AddTextureXXX functions,
											// which imply the added reference
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
	CD3DFPS_UNLIMITED,		// Uncapped fps (no waiting)
	CD3DFPS_FIXED			// Fixed FPS rate (ugly)
};

enum MultiMonitorMode {
	MMM_SINGLESCREEN,		// Default (fullscreen on primary monitor only)
	MMM_CLONE,				// One screen, but cloned across all monitors
};

// CD3DDisplaySetup: initialisation parameters for CD3DDisplay::Init()
struct CD3DDisplaySetup
{
	vector<HWND> hWnds;		// hWnd of windows to use (only one element needed for non-multi-monitor systems)
	HWND hFocusWnd;			// Focus hwnd
	D3DFORMAT resFormat;	// Must be D3DFMT_A8R8G8B8
	BOOL bFullscreen;		// Fullscreen change resolution mode
	int resWidth;			// Width of fullscreen resolution if bFullscreen is true
	int resHeight;			// Height of fullscreen resolution if bFullscreen is true
	CD3DFPSMode fpsMode;	// FPS mode (Vsynced, unlimited, fixed)
	int fps;				// If CD3DFPS_FIXED selected, the FPS rate
	D3DSWAPEFFECT swapEffect;
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
	POINTF p1;		// Top left; origin
	POINTF p2;		// Top right
	POINTF p3;		// Bottom left
	POINTF p4;		// Bottom right

	int width;  // used by davo to clamp uv
	int height; // used by davo to clamp uv

	// Create from a stretched rect
	void Stretched(RECTF& rc, float xf, float yf);

	// Create from a rotated rect.  Angle in degrees
	void Rotated(RECTF& rc, float angle);
	void RotatedAll(RECTF& rc, float angle);
	void Rotated(RECTF& rc, float angle, float skewX, float skewY);
	void Skew(float skewX, float skewY);

	// Create from a stretched then rotated rect
	void StretchRotated(RECTF& rc, float xf, float yf, float angle);
	void StretchRotatedAll(RECTF& rc, float xf, float yf, float angle);
	void GetBoundingRect(RECTF& rc);

	// Create from rect
	void operator=(RECTF rc);
	void operator=(RECT rc);
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

	ID3DXFont* d3dxfont;
	CD3DDisplay* pDisplay;

	int pxSize;
	DWORD bold;
	bool italics;
	CString fontface;
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
#define CD3D_MAX_BATCH_SIZE			20000	// Maximum number of vertices for a batch
#define CD3D_MAX_INDEX_SIZE			30000	// 6 indices for 4 vertices max

// Batch types
enum CD3DBATCH {
	BATCH_NULL,
	BATCH_POINTS,
	BATCH_LINES,
	BATCH_LINESTRIP,
	BATCH_BOXES,
	BATCH_QUADS
};
class CBatchBase
{
public:
	CBatchBase()
	{
		batchCount = 0;
		indexCount = 0;
		batchType = BATCH_NULL;
	}
	virtual ~CBatchBase() {}

	virtual void Do(CD3DDisplay* pDisplay) {};

	void* operator new(unsigned int num_bytes, CD3DDisplay* pDisplay);

	// Prevent compiler warnings: delete does nothing; call BatchMemoryReset()
	void operator delete(void*, CD3DDisplay* pDisplay) {}
	void operator delete(void*) {}

	// Batching
	int batchCount;
	int indexCount;
	CD3DBATCH batchType;
};

class CBatch_SetTexture : public CBatchBase
{
public:
	CBatch_SetTexture(IDirect3DBaseTexture9* tex) : CBatchBase()
	{
		texture = tex;
	}
	virtual void Do(CD3DDisplay* pDisplay);

	bool SameAs(IDirect3DBaseTexture9* _texture)
	{
		return texture == _texture;
	}

	IDirect3DBaseTexture9* texture;

};

class CBatch_SetRenderTarget : public CBatchBase
{
public:
	CBatch_SetRenderTarget(IDirect3DTexture9* _tex) : CBatchBase()
	{
		tex = _tex;
	}
	virtual void Do(CD3DDisplay* pDisplay);

	bool SameAs(IDirect3DTexture9* _tex)
	{
		return tex == _tex;
	}

	IDirect3DTexture9* tex;
};

class CBatch_Update3dCoords : public CBatchBase
{
public:
	CBatch_Update3dCoords(const D3DXMATRIX& _Ortho2D) : CBatchBase()
	{
		Ortho2D = _Ortho2D;
	}
	virtual void Do(CD3DDisplay* pDisplay);

	bool SameAs(D3DXMATRIX& _Ortho2D)
	{
		return Ortho2D == _Ortho2D ? true : false;
	}

	D3DXMATRIX Ortho2D; 
};


class CBatch_SetRenderState : public CBatchBase
{
public:
	CBatch_SetRenderState(D3DRENDERSTATETYPE _rstype, UINT _value) : CBatchBase()
	{
		rstype = _rstype;
		value = _value;
	}

	bool SameAs(D3DRENDERSTATETYPE _rstype, UINT _value)
	{
		return rstype == _rstype && value == _value;
	}

	virtual void Do(CD3DDisplay* pDisplay);

	D3DRENDERSTATETYPE rstype;
	UINT value;
};

class CBatch_SetSamplerState : public CBatchBase
{
public:
	CBatch_SetSamplerState(D3DSAMPLERSTATETYPE _sstype, UINT _value) : CBatchBase()
	{
		sstype = _sstype;
		value = _value;
	}

	bool SameAs(D3DSAMPLERSTATETYPE _sstype, UINT _value)
	{
		return sstype == _sstype && value == _value;
	}

	virtual void Do(CD3DDisplay* pDisplay);

	D3DSAMPLERSTATETYPE sstype;
	UINT value;
};

class CBatch_SetBackBuffer : public CBatchBase
{
public:
	CBatch_SetBackBuffer(IDirect3DSurface9* s) : CBatchBase()
	{
		pSurf = s;
	}
	virtual void Do(CD3DDisplay* pDisplay);

	bool SameAs(IDirect3DSurface9* s)
	{
		return pSurf == s;
	}

	IDirect3DSurface9* pSurf;
};

struct CustomParamBatch {
	D3DXHANDLE handle;
	float value;
};

struct EffectParameterBatch {
	IDirect3DBaseTexture9*	foregroundTexture,
						 *	backgroundTexture,
						 *	sourceTexture,
						 *	previousTexture;
	float	frameCounter,
			boxLeft,
			boxTop,
			boxRight,
			boxBottom,
			boxWidth,
			boxHeight,
			hotspotX,
			hotspotY,
			pixelWidth,
			pixelHeight;
	float bgStart[2];
	float bgEnd[2];

	vector<CustomParamBatch> customParams;
};

class CBatch_Fx_Begin : public CBatchBase
{
public:
	CBatch_Fx_Begin(class CRunEffect* eff) : CBatchBase()
	{
		curEff = eff;
	}

	virtual void Do(CD3DDisplay* pDisplay);

	CRunEffect* curEff;

public:
	EffectParameterBatch params;

};

class CBatch_Fx_End : public CBatchBase
{
public:
	CBatch_Fx_End(ID3DXEffect* eff) : CBatchBase()
	{
		curEff = eff;
	}

	virtual void Do(CD3DDisplay* pDisplay);

	ID3DXEffect* curEff;
};

class CBatch_Fx_BeginPass : public CBatchBase
{
public:
	CBatch_Fx_BeginPass(ID3DXEffect* eff, int p) : CBatchBase()
	{
		curEff = eff;
		i = p;
	}
	virtual void Do(CD3DDisplay* pDisplay);

	ID3DXEffect* curEff;
	int i;
};

class CBatch_Fx_EndPass : public CBatchBase
{
public:
	CBatch_Fx_EndPass(ID3DXEffect* eff, int p) : CBatchBase()
	{
		curEff = eff;
		i = p;
	}
	virtual void Do(CD3DDisplay* pDisplay);

	ID3DXEffect* curEff;
	int i;
};

class CBatch_ClearRT : public CBatchBase
{
public:
	CBatch_ClearRT(D3DCOLOR _color) : CBatchBase()
	{
		color = _color;
		clearAll = true;
	}
	CBatch_ClearRT(D3DCOLOR _color, const D3DRECT& clear) : CBatchBase()
	{
		color = _color;
		clearrect = clear;
		clearAll = false;
	}
	virtual void Do(CD3DDisplay* pDisplay);
	
	D3DCOLOR color;
	D3DRECT clearrect;
	bool clearAll;
};

class CBatch_SetTransform : public CBatchBase
{
public:

	CBatch_SetTransform(D3DTRANSFORMSTATETYPE _state, D3DXMATRIX* pMatrix) : CBatchBase()
	{
		state = _state;
		matrix = *pMatrix;
	}

	bool SameAs(D3DTRANSFORMSTATETYPE _state, D3DXMATRIX* pMatrix)
	{
		return (_state == state && matrix == *pMatrix);	
	}

	virtual void Do(CD3DDisplay* pDisplay);

	D3DTRANSFORMSTATETYPE state;
	D3DXMATRIX matrix;
};

class CD3DDisplay {
public:

	friend class CLayoutEditor;
	friend class CBatch_SetTexture;
	friend class CBatch_SetRenderTarget;
	friend class CBatch_SetBackBuffer;

	vector<CBatchBase*> batch;

	///////////////
	// Constructor; destructor
	CD3DDisplay();
	~CD3DDisplay();

	///////////////
	// Device level control
	void	PreInit();
	bool	Init(CD3DDisplaySetup* settings);
	bool	Close();
	void	Reset();
	void	SetupDevice();
	void	ResetBatch();

	///////////////
	// Display control functions
	void	ClearRT(D3DCOLOR color = 0x00000000);	// Clear render target
	void	ClearRT(D3DCOLOR color, const D3DRECT& rect);	// Clear render target
	void	BeginScene();							// Begin drawing operations
	void	EndScene();								// End drawing operations
	void	Present(RECT* pRec = NULL, HWND hWndOverride = NULL);

	///////////////
	// Texture control functions
	// Adding textures from file or resource: loads the texture to an internal store.  Returns
	// a texture index (an integer) to the texture in the store, which you must then use
	// to reference the texture.D3DFMT_A32B32G32R32F
	int				AddTextureRT(int w, int h, D3DFORMAT format = D3DFMT_A8R8G8B8, DWORD usage = D3DUSAGE_RENDERTARGET, D3DPOOL pool = D3DPOOL_DEFAULT);
	int				AddTextureFromFile(const char* filename);
	int				AddTextureFromFileInMemory(LPCVOID pData, UINT len);
#ifndef CD3D_NOCXIMAGE
	int				AddTextureFromCxImage(CxImage& cx);
#endif
	int				AddTextureFromBuffer(BYTE *buffer, long size);
	int				AddTextureFromResource(HMODULE hModule, LPCTSTR pResource);
	void			Premultiply(int& iTexture);
	void			ConvertToRT(int& iTexture);
	void			ClearTextures();				// Clear all stored textures
	IDirect3DTexture9* TextureAt(int index);		// Get texture pointer at index
	void			SetTexture(int index);			// Set current drawing texture
	int				GetTexture();
	TextureInfo*	TexInfo(int index);				// Get TextureInfo pointer at (shortcut)
	bool			ResizeTextureRT(int& iTexture, int w, int h);
	void			RemoveTexture(int iTexture);	// Delete texture
	void			DecrementTextureReference(int iTexture, bool deleteIfZero);	// Decrement reference counter
	void			IncrementTextureReference(int iTexture, int references = 1);
	bool			TextureExists(int iTexture);
	void			ApplyTextureGutter(int iTexture);

	// Render targets (to render to a texture)
	bool			SetRenderTarget(int iTexture);
	int				GetRenderTarget();

	// States
	void			SetRenderState(D3DRENDERSTATETYPE rstype, UINT value);
	unsigned long	GetRenderState(D3DRENDERSTATETYPE rstype);
	void			SetSamplerState(D3DSAMPLERSTATETYPE sstype, UINT value);
	unsigned long	GetSamplerState(D3DSAMPLERSTATETYPE sstype);
	void            SetTransform(D3DTRANSFORMSTATETYPE _state, D3DXMATRIX* pMatrix);
	void			SetAdditiveBlending();
	void			SetAlphaBlending();

	///////////////
	// Drawing operations
	void	CreateFont(const char* fontface, unsigned int pxSize, bool italics, bool bold, CD3DDisplayFont* pFont);
	void	DrawText(CD3DDisplayFont& font, const char* text, RECTF& rc, D3DCOLOR color, DWORD format);
	void	DrawText(CD3DDisplayFont& font, const char* text, RECT& rc, D3DCOLOR color, DWORD format);
	void	SetTextQuality(DWORD quality);	// CLEARTYPE_QUALITY, ANTIALIASED_QUALITY, etc
	void	UpdateAllExistingFonts();		// Set quality across all existing fonts

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
	void	DoBatch(CBatchBase* pBase);
	void	ClearBatchData();
	void	OptimiseBatchStack();


	void	Point(float x, float y, D3DCOLOR color);
	void	Line(float x1, float y1, float x2, float y2, D3DCOLOR color);
	void	Line(float x1, float y1, float x2, float y2, D3DCOLOR color1, D3DCOLOR color2);
	void	SmoothLineBegin();
	void	SmoothLineEnd();
	void	SmoothLine(float x1, float y1, float x2, float y2, D3DCOLOR color);
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
	void	Blit(RECTF& rc, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv = NULL, float skewX = 0.0f, float skewY = 0.0f);	
	
	// Backbuffer overriding
	void	SetBackBuffer(LPDIRECT3DSURFACE9 pSurf = NULL);

	void	Setup2DMode();

	void ReplaceTexture(int oldTex, int newTex);

	void OnLostDevice();
	void OnResetDevice();

	// Global filter: perform full screen tinting.  Deprecated.
	// Format: 0xAARRGGBB (alpha, red, green, blue)
	// E.g. 0xFFFF0000 is normal transparency but only red colours.
	// 0xFF888888 is normal transparency at half brightness.
	void	SetGlobalFilter(D3DCOLOR color);
	void	SetGlobalFilter(unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue);
	D3DCOLOR GetGlobalFilter();

	// Get private data
	IDirect3DDevice9* GetD3DDevice() const;
	D3DCAPS9*		  GetD3DCaps();
	//HWND			  GetHwnd();

	// Version
	int Version() { return CD3DDISPLAY_VERSION; }
	void VersionStr(char* str) { strcpy_s(str, strlen(str), CD3DDISPLAY_VERSION_STR); }
	void VersionFull(char* str) { strcpy_s(str, strlen(str), CD3DDISPLAY_VERSION_FULL); }

	// Mix filters
	D3DCOLOR MixFilters(D3DCOLOR f1, D3DCOLOR f2);

	friend class CRuntime;
	friend class CPictureEditor;
	friend class SystemObject;

private:

	vector<HWND>			hWnds;			// hwnd drawing to
	IDirect3D9*				d3d;			// d3d interface
	IDirect3DDevice9*		d3dDevice;		// d3d device
	D3DCAPS9				d3dCaps;		// d3d capabilities
	D3DPRESENT_PARAMETERS*	d3dPresents;	// d3d presentation parameters
	//IDirect3DVertexBuffer9*	vertexBuffer;	// Vertex buffer
	IDirect3DVertexBuffer9* batchBuffer;	// Buffer for batch operations
	IDirect3DIndexBuffer9*  indexBuffer;	// For batching quads

public:

	void UpdatePresentsArray();				// For multihead, copy d3dPresents[0] through array
	int GetMultiHeadCount() const;

	DWORD renderStates[CD3D_NUM_RENDERSTATES];
	DWORD samplerStates[CD3D_NUM_SAMPLERSTATES];
	D3DXMATRIX transforms[CD3D_NUM_TRANSFORMS];

	int iRenderStates[CD3D_NUM_RENDERSTATES];
	int lastiRenderStates[CD3D_NUM_RENDERSTATES];

	int iSamplerStates[CD3D_NUM_SAMPLERSTATES];
	int lastiSamplerStates[CD3D_NUM_SAMPLERSTATES];

	int iTransform[CD3D_NUM_TRANSFORMS];
	int lastiTransform[CD3D_NUM_TRANSFORMS];

	int iFxEndPass;
	int lastiFxEndPass;
	
	int iFxEnd;
	int lastiFxEnd;

	int iSetTexture;
	int lastiSetTexture;

	int iUpdate3dCoords;
	int lastiUpdate3dCoords;

	int iSetRenderTarget;
	int lastiSetRenderTarget;

	int batchesPerPresent;
	bool changedBackBuffer;


public:
	// CDisplay memory management
	list<CDisplayHeapChunk> memChunks;
	list<CDisplayHeapChunk>::iterator curChunk;
	BYTE* chunkPtr;

	void* BatchAllocate(unsigned int s);
	void BatchMemoryReset();

	// Batching from system memory
	vector<TLVERTEX> verticesVec;
	vector<unsigned short> indicesVec;

	inline void AddVertex(D3DCOLOR color, float x, float y, float u, float v);
	 void AddVertex3d(D3DCOLOR color, float x, float y, float z, float u, float v);
	inline void AddIndex(unsigned short i);

	/*
	int mem_iSetRenderState;
	int mem_iSetSamplerState;
	int mem_iFx_Begin;
	int mem_iFx_End;
	int mem_iFx_BeginPass;
	int mem_iFx_EndPass;
	int mem_iSetTexture;
	int mem_iSetRenderTarget;
	int mem_iUpdate3dCoords;
	int mem_iClearRT;
	int mem_iBase;
	int mem_iSetTransform;
	*/

	int batch_lastdraw;

private:


	HRESULT					hr;				// for returns

	HMODULE					d3d9Dll;		// d3d9.dll

	ID3DXSprite*  m_pD3DXSprite;
	vector<class CD3DDisplayFont*> fontRefs;
	friend class CD3DDisplayFont;
	DWORD textQuality;

	// TextureInfo store
	map<int, TextureInfo*>		textures;
	int							curTextureId;

public:
	
	TextureInfo*				GetTexture(int index);
	bool						haveIGotMyDevice;
	
private:
	// Current texture number
	int						curTexture;

	// Backbuffer width/height
	int bbWidth, bbHeight;

	// Render surface
	IDirect3DSurface9*		pDisplaySurface;
	IDirect3DSurface9*		pBackBuffer;		// Overridable from SetBackBuffer

	int renderTex;

	// BlitTex: blits according to texture dimensions and not image dimentions like
	// the ordinary Blit functions.  Called by the Blit functions.
	void	BlitTex(Quadrilateral& q, D3DCOLOR filter, const RECTF* uv = NULL);
	void	BlitTex(Quadrilateral& q, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv = NULL);

	// Global filter
	D3DCOLOR				globalcolor;

	// Batching
	int batchRunIndex;
	int indexRunIndex;
	
	int batchIndex;
	int indexIndex;

	MultiMonitorMode multiMonitorMode;
	ID3DXLine* lineDrawer;

public:

	void SetMultiMonitorMode(MultiMonitorMode mmm);
	MultiMonitorMode GetMultiMonitorMode() const;

	// Transformations
	void Update3dCoords(bool translate = true);
	D3DXMATRIX Ortho2D;
	float trans_PanX;
	float trans_PanY;
	float trans_Angle;
	float trans_ScaleX;
	float trans_ScaleY;

	// Memory tracking
	static int vramtargetbytes;
	static int vramtexturebytes;
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

#endif