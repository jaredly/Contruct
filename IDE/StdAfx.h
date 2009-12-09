// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

/*
	Construct - DirectX 9 Game and Application creation
    Copyright (C) 2008 Scirra

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#if !defined(AFX_STDAFX_H__931C3A96_D033_4434_8CB1_0FC555749337__INCLUDED_)
#define AFX_STDAFX_H__931C3A96_D033_4434_8CB1_0FC555749337__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define PYTHON

////////////////////////////////
// Construct Version Information
// Current version:

#define CONSTRUCT_VERSION_ID		9962
#define CONSTRUCT_VERSION_STRING	"0.99.62"

#define VC_EXTRALEAN				// Exclude rarely-used stuff from Windows headers
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#define XLISTCTRL_TRACE(a) 

//PORT: warning disables
#pragma warning(disable:4018)		// Signed/unsigned mismatch
#pragma warning(disable:4244)		// Conversion 'float' to 'int', etc
#pragma warning(disable:4800)		// 'bool' to 'int' conversion (performance warning)
#pragma warning(disable:4995)		// Function pragma deprecated

#define CONSTRUCT

//#define _WIN32_WINNT 0x0501

#define EDITTIME
#define CRUNTIME_MAX_PARAMS 18

#include <afxsock.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxole.h>			// MFC support for OLE
#include <afxtempl.h>
#include <afxcview.h>
#include <afxadv.h>

#include <winioctl.h>

#include <math.h>
//#ifdef DEBUGBUILD
#define __STATPROFUIS_WITH_DLLMFC__
//#endif

// Include Prof-UIs
#include "LIBs\Include\Prof-UIS.h"

#define IDC_OPTIONSTREE_OPTIONS 777

#pragma warning(disable: 4786) // debug info truncated, nobody cares kthx

// DirectX display engine includes
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <algorithm>
#include <set>
using namespace std;

#define CRMATH_DEFAULT_TYPEDEFS
#define CRMATH_NO_SSE
#define CRMATH_NO_CONTROLFP
#include "..\Common\Renderers\CRMath.h"

#include "..\Common\CxImage\CxImage\ximage.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "..\Common\CDisplay.h"
#define BIN_CXIMAGE
#include "..\Common\TigsUtil.hpp"
#include "Utilities\Token.h"
bool FileExists(LPCSTR Name);

#define XHTMLDRAGDROP
#include "UI Elements\XHTMLTree\XHTMLTree.h"

#define ACTION		0	
#define CONDITION	1
#define EXPRESSION	2
#define EVENT		3
#define OBJECT		4

//#include "vld.h"

extern class CMainFrame* g_MainFrame;

#define CHECKSERIAL CheckSerial(ar);

void CheckSerial(CArchive& ar);

class CObjectRectangle
{
public:
	double x;
	double y;
	double w;
	double h;
	double a;
	double ox;
	double oy;

	CObjectRectangle(double _x, double _y, double _w, double _h,double _a = 0, double _ox = 0.5, double _oy = 0.5);
	CRect GetBoundingRect();
	bool PtInRect(POINT point);
	void CalculateBlitValues(double& _x, double& _y, double& _w, double& _h, double& _a);
	CPoint GetPoint(double xratio, double yratio, double distance = 0);
	
};

struct ControlTableEntry {
	int vk;
	const char* name;
};

extern ControlTableEntry controlTable[];

class PreviewImage
{
public:
	PreviewImage()
	{
		bitmap = 0;
	}
	int frameno;
	CxImage* bitmap;

	void Init()
	{
		bitmap = new CxImage;
	}
	void Release()
	{
		if(bitmap)
			delete bitmap;
	}
};

// Platform SDK common controls
#include <CommCtrl.h>

#define MIIM_STRING      0x00000040
#define MIIbitmap      0x00000080
#define MIIM_FTYPE       0x00000100

// Includes that must come first
#include "Resource.h"
#include "Utilities\ddmgr.h"

// Controls
#include "UI Elements\Controls\TitleTip.h"
#include "UI Elements\Controls\Ruler.h"

#include "UI Elements\Controls\ParamTooltipCtrl.h"
#include "UI Elements\CtrlMessageBar.h"
#include "UI Elements\Controls\FlatToolTipCtrl.h"
#include "UI Elements\HTML\HTMLDrawer.h"
#include "Editors\ChronoEventEditor\ChronoEventEditor.h"
#include "UI Elements\Controls\FlatSplitterWnd.h"

// Other includes
#include "Structure\Languages\Languages.h"
#include "Utilities\miscfunc.h"

#include "UI Elements\mfxWhlPan.h"

// Dialogs: include last as they need the above
#include "Dialogs\Error Dlg.h"

extern vector<CString> g_Traits;

class CMemDC : public CDC {

private:
    CBitmap    bitmap;        // Offscreen bitmap
    CBitmap*       m_oldBitmap; // bitmap originally found in CMemDC
    CDC*       m_pDC;           // Saves CDC passed in constructor
   
    BOOL       m_bMemDC;        // TRUE if CDC really is a Memory DC.


public:
    CRect      m_rect;          // Rectangle of drawing area. 
    CMemDC(CDC* pDC, const CRect* pRect = NULL) : CDC()
    {
        ASSERT(pDC != NULL); 
 
        // Some initialization
        m_pDC = pDC;
        m_oldBitmap = NULL;
        m_bMemDC = !pDC->IsPrinting();
 
        // Get the rectangle to draw
        if (pRect == NULL) {
             pDC->GetClipBox(&m_rect);
        } else {
             m_rect = *pRect;
        }
 
        if (m_bMemDC) {
             // Create a Memory DC
             CreateCompatibleDC(pDC);
             pDC->LPtoDP(&m_rect);
 
             bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), 
                                                  m_rect.Height());
             m_oldBitmap = SelectObject(&bitmap);
 
             SetMapMode(pDC->GetMapMode());
 
             SetWindowExt(pDC->GetWindowExt());
             SetViewportExt(pDC->GetViewportExt());
 
             pDC->DPtoLP(&m_rect);
             SetWindowOrg(m_rect.left, m_rect.top);
        } else {
             // Make a copy of the relevent parts of the current 
             // DC for printing
             m_bPrinting = pDC->m_bPrinting;
             m_hDC       = pDC->m_hDC;
             m_hAttribDC = pDC->m_hAttribDC;
        }
 
        // Fill background 
        FillSolidRect(m_rect, pDC->GetBkColor());
    }
    
    ~CMemDC()      
    {          
        if (m_bMemDC) {
             // Copy the offscreen bitmap onto the screen.
             m_pDC->BitBlt(m_rect.left, m_rect.top, 
                           m_rect.Width(),  m_rect.Height(),
                  this, m_rect.left, m_rect.top, SRCCOPY);            
             
             //Swap back the original bitmap.
             SelectObject(m_oldBitmap);        
        } else {
             // All we need to do is replace the DC with an illegal
             // value, this keeps us from accidentally deleting the 
             // handles associated with the CDC that was passed to 
             // the constructor.              
             m_hDC = m_hAttribDC = NULL;
        }       
    }
    
    // Allow usage as a pointer    
    CMemDC* operator->() 
    {
        return this;
    }       
 
    // Allow usage as a pointer    
    operator CMemDC*() 
    {
        return this;
    }
};







class CPrecisionTimer
{
	LARGE_INTEGER lFreq, lStart;
	public:
	bool start;

	CPrecisionTimer()
	{
		QueryPerformanceFrequency(&lFreq);
		start = false;
	}

	inline void Start()
	{
		QueryPerformanceCounter(&lStart);
		start = true;
	}

	inline double Stop()
	{
		if(start)
		{
		// Return duration in seconds...
		LARGE_INTEGER lEnd;
		QueryPerformanceCounter(&lEnd);

	double msTime = (double(lEnd.QuadPart - lStart.QuadPart) / lFreq.QuadPart) * 1000.0;
	CString msg;
	msg.Format("Time taken :\n\n%.2fms", msTime);
	//MessageBox(NULL, msg, "Performance", MB_OK);
		}
		start = false;

	return 0;
	}
};

extern CPrecisionTimer Pres;


#define BEGINTIMER Pres.Start();
#define ENDTIMER Pres.Stop();

CString FloatToString(double x);

//#include "wincrypt.h"
#include <string>

// Object functions
#include "Structure\ObjectFunctions.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


//void SerializeACESEntry(ACESEntry* entry,  CArchive& ar );
void RemapACEMenu(CMenu* pMenu, int& count, ACEMenuMap& map, int type, int child);
void CopyMenu(CMenu* src, CMenu* dest);
// davo woz ere


// MessageBox wrapper (AG)
DWORD ExtMessageBox(HWND hWnd, CString msg, CString caption, DWORD flags);


#endif // !defined(AFX_STDAFX_H__931C3A96_D033_4434_8CB1_0FC555749337__INCLUDED_)
