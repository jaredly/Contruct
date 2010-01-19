// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
// 
#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#pragma warning(disable: 4786)	// "Debug name truncated"
#pragma warning(disable: 4018)	// "Signed/unsigned mismatch"
#pragma warning(disable: 4244)	// "Conversion from X to Y, possible loss of data"
#pragma warning(disable: 4355)	// "this in initializer list" - used to legitimately initialise interfaces

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_DEPRECATE // Not security software

// If in debug build, don't pad CRunObject to account for the larger size of vectors
#ifdef _DEBUG
#define CONSTRUCT_RUNTIME_DEBUG
#endif

#define CONSTRUCT_RUNTIME_TR1	// Support TR1 features (mersenne twister, normal distribution etc)
 
#include "resource.h"

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcmn.h>			// MFC support for Windows Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Lib headers
//#include <windows.h> // MFC apps dont include windows.h

// Exclude DX stuff from app runtime
#ifndef APPRUNTIME
#include <d3d9.h>
#include <d3dx9.h>
#endif 

#pragma warning(disable: 4096)
#include <vector>
#include <string>
#include <list>
#include <map>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <utility>
using namespace std;
#include <time.h>
#include <direct.h>
#include <math.h>
#include "mtrand.h"
#include <cfloat>
// If you disable CONSTRUCT_RUNTIME_TR1 normalrandom will return 0 since it requires it
//#undef CONSTRUCT_RUNTIME_TR1
#ifdef CONSTRUCT_RUNTIME_TR1
#include <random>
#endif

struct RECTF
{
	float left;
	float top;
	float right;
	float bottom;
};

// MMX and SSE intrinsics
#include <emmintrin.h>
#include <xmmintrin.h>

#ifdef CONSTRUCT_DEBUGGER
#define XLISTCTRL_TRACE(a)  
#define XLISTCTRLLIB_STATIC
#define XLISTCTRLLIB_NOAUTOLIB
#include "XListCtrl.h"
#endif

//#define SOL_DEBUGGING		// Enable SOL verification code (slow)

// Hack:  VS2005 debugger is getting confused between CEvent and an MFC CSyncObject-deriving CEvent.
#define CEvent _CEvent

#define RUNTIME

#include "RuntimeFunctions.hpp"

void ShowInt(char* msg, int num);
void ShowStr(char* msg, const char* str);

#define CSTR(a) ((const char*)(a))

// Custom headers
// Don't include compiler
//#define CD3D_USE_HLSL_COMPILER

//#define PYTHON
#define PYTHONIMPORT

#ifndef APPRUNTIME
	#ifdef CONSTRUCT_SDL
		#include "SDL\include\SDL.h"
		#include "SDLImage\include\SDL_Image.h"
		#include "SDLGFX\SDL_rotozoom.h"
		#include "SDL\include\SDL_opengl.h"
		#include "..\Common\CSDLDisplay.h"

		#pragma comment(lib, "SDL\\lib\\SDL.lib") 
		#pragma comment(lib, "SDL\\lib\\SDLmain.lib") 
		#pragma comment(lib, "SDL\\lib\\SDL.lib") 
		#pragma comment(lib, "SDLGFX\\libgfx.lib") 
		#pragma comment(lib, "SDLImage\\lib\\SDL_image.lib") 
	#endif

	#ifdef CONSTRUCT_DIRECTX9
		#define CD3D_NOCXIMAGE
		#define USE_3D_CORDS

		#include "..\Common\Renderers\CDX9Renderer.h"
		// DirectInput for games
		#define DIRECTINPUT_VERSION 0x0800
		#include <dinput.h>

		typedef cr::CDX9Renderer Renderer;
		typedef cr::TextureHandle TextureHandle;
		typedef cr::point point;
		typedef cr::point3d point3d;
		typedef cr::rect rect;
		typedef cr::quad quad;
		const TextureHandle unallocated_texture((TextureHandle)-1);
	#endif
#endif
#ifdef APPRUNTIME

	#define NO_DEFINE_RECTF
	#include "App_only.h"
#endif

#include "..\Common\TigsUtil.hpp"

#ifdef PYTHON

	#ifdef _DEBUG
		#define ____DEBUG
		#undef _DEBUG /* Link with python24.lib and not python24_d.lib */
	#endif
	#include <Python.h>
	#ifdef ____DEBUG
		#define _DEBUG
	#endif
#endif

// Display typedefs
#ifdef CONSTRUCT_SDL
typedef CSDLDisplay			CDisplay;
typedef CSDLDisplayFont		CDisplayFont;
#endif

// TriggerMap[oid][cndID][triggerIndex][0..n]
typedef vector<class CEventLine*> ParamList;			// List of the event lines matching all 3
typedef vector<ParamList> ParamIndexList;		// List of matching condition parameters
typedef vector<ParamIndexList> TriggerList;		// List of matching condition IDs
typedef vector<TriggerList> TriggerMap;			// List of object IDs

#include "..\Common\Rundefs.h"
#include "..\Common\VRuntime.h"
#include "CExpression.h"

#include "..\Common\bin.h"
#include "RendererLayer.h"

// Global allocator for runtime bin usage
class runtime_allocator : public bin_allocator {
public:
	BYTE* allocNew(unsigned int _size) {
		return new BYTE[_size];
	}
	void allocDelete(void* ptr) {
		delete[] ptr;
	}
};

extern runtime_allocator g_allocator;

#include "Eventtree.h"
#include "CapReader.h"
#include "System.h"
#include "CRuntime.h"
#include "ExpTree.h"
#include "Inlines.h"

#ifdef CONSTRUCT_DEBUGGER
#include "DebuggerDlg.h"
#include "ObjectPage.h"
#include "LogPage.h"
#include "WatchPage.h"
#endif

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
