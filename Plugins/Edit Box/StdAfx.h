// StdAfx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#if !defined(AFX_STDAFX_H__30647434_AC5A_4EA9_A23D_7F52082D1A78__INCLUDED_)
#define AFX_STDAFX_H__30647434_AC5A_4EA9_A23D_7F52082D1A78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ABI compatibility (VS8 for MFC/STL compat, debug for heap compat)
#if _MSC_VER == 1400 && !defined(_DEBUG)
#define CSDK_COMPATIBLE_COMPILER
#else
#pragma message("Warning: compiler or settings not compatible with Construct Runtime ABI. Enabling workaround functions. See SDK docs on 'Runtime ABI'.")
#endif

#pragma warning(disable: 4786)	// debug info truncated 255 chs
#pragma warning(disable: 4018)	// signed/unsigned mismatch
#pragma warning(disable: 4244)	// conversion from x to y
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include "resource.h"

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Custom headers
#include <math.h>
#include <vector>
#include <list>
#include <string>
#include <map>
using namespace std;

#define CONSTRUCT_SDK

// Includes for renderer stdafx.h - replace where aggressiveoptimize.h used to be
// to vruntime.h in old SDKs with this snippet in stdafx.h
#include "..\..\Common\DXSnips.h"
#include "..\..\Common\Renderers\RendererCommon.h"

// SDK uses the base class for texture handles with no renderer-specific information.  Font handles
// do not have their pointed-to class defined so are void*
typedef cr::texture*			TextureHandle;
typedef void*					FontHandle;

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
