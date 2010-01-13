// StdAfx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#if !defined(AFX_STDAFX_H__30647434_AC5A_4EA9_A23D_7F52082D1A78__INCLUDED_)
#define AFX_STDAFX_H__30647434_AC5A_4EA9_A23D_7F52082D1A78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ASSERT(x) x

// ABI compatibility (VS8 for MFC/STL compat, debug for heap compat)
#if _MSC_VER == 1400 && !defined(_DEBUG)
#define CSDK_COMPATIBLE_COMPILER
#else
#pragma message("Warning: compiler or settings not compatible with Construct Runtime ABI. Enabling workaround functions. See SDK docs on 'Runtime ABI'.")
#endif

#pragma warning(disable: 4786)	// debug info truncated 255 chs
#pragma warning(disable: 4018)	// signed/unsigned mismatch
#pragma warning(disable: 4244)	// conversion from x to y

#define _CRT_SECURE_NO_DEPRECATE

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include "resource.h"
#include <windows.h>
#include <atlstr.h>
#include <atltypes.h>

// Custom headers
#include <time.h>
#include <math.h>
#include <vector>
#include <list>
#include <string>
#include <map>
using namespace std;


#define CONSTRUCT_SDK

#include "..\..\Common\DXSnips.h"
#include "..\..\Common\Renderers\RendererCommon.h"
typedef cr::texture*	TextureHandle;
typedef void*	FontHandle;

#define Deg(x) cr::to_degrees(x)
#define Rad(x) cr::to_radians(x)

#include "..\..\Common\VEdittime.h"
#include "..\..\Common\Rundefs.h"
#include "..\..\Common\VRuntime.h"

// Global SDK definitions
#include "..\..\Common\SDKdefs.h"

// Actions, Conditions and Expressions ACESEntry arrays
//extern vector<ACESEntry> Conditions;
//extern vector<ACESEntry> Actions;
//extern vector<ACESEntry> Expressions;

// Routine lists
//extern vector<ConditionRoutine> CndRoutines;
//extern vector<EXTROUTINE> ActRoutines;
//extern vector<EXTROUTINE> ExpRoutines;
//extern vector<CString> ExpNames;

#include "Main.h"

//#include <stdlib.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__30647434_AC5A_4EA9_A23D_7F52082D1A78__INCLUDED_)
