/*
 *
 *		Snippets and defs from DirectX used by the display engine
 *
 *
 */
#ifndef _DXSNIPS_H_
#define _DXSNIPS_H_

#define D3D_MAKECOLOR(c) (0xFF000000 | (c))
#define D3D_CONVERTCOLORREF(c) (0xFF000000 | (GetRValue(c) << 16) | (GetGValue(c) << 8) | GetBValue(c))
#define COLORREF_CONVERTD3D(c) (0x00FFFFFF & RGB(D3D_R(c), D3D_G(c), D3D_B(c)))
#define D3D_SYSCOLOR(c) (0xFF000000 | D3D_CONVERTCOLORREF(GetSysColor(c)))

#define D3D_ARGB(a,r,g,b) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))
#define D3D_RGB(r,g,b) D3D_ARGB(0xFF, r, g, b)

#define D3D_A(c) (((c) & 0xFF000000)>>24)
#define D3D_R(c) (((c) & 0x00FF0000)>>16)
#define D3D_G(c) (((c) & 0x0000FF00)>>8)
#define D3D_B(c) ((c) & 0x000000FF)

#define D3D_OPACITYCOLORREF(a, c) (D3D_ARGB(a, GetRValue(c), GetGValue(c), GetBValue(c)))

// From CD3DDisplay.h
// Batch types
enum CD3DBATCH {
	BATCH_NULL,
	BATCH_POINTS,
	BATCH_LINES,
	BATCH_LINESTRIP,
	BATCH_BOXES,
	BATCH_QUADS
};

// FPS mode
enum CD3DFPSMode {
	CD3DFPS_VSYNCED,		// Synchronised to screen refresh
	CD3DFPS_UNLIMITED,		// Uncapped fps (continuous loop)
	CD3DFPS_FIXED			// Fixed FPS rate (unsupported)
};

// Render to main screen
#define RENDERTARGET_DISPLAY -1
#define TEXTURE_NONE -1

// Gradient direction
enum Direction {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};

// Floating point rect
struct RECTF
{
	float left;
	float top;
	float right;
	float bottom;
};

// DX snips

#ifndef MAKEFOURCC
    #define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |       \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */

#endif // _DXSNIPS_H_