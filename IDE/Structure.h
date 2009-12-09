// Structure.h
// Application and Extention Structure

#pragma once

typedef CList<long,long &> CObjList;

// Construct load/save version number for cna format
#define CNA_FORMAT "Construct1.7"
#define CNA_FORMAT_LENGTH 13

#include <vector>
using namespace std;

bool SerializeClassHeader(CString& Name, int& Version, CArchive& Ar);
void SerializeClassPadding(int NumberStrings, int NumberInts, CArchive& Ar);

// Get common virtual editInfo
#include "Structure\SDKDefs.h"
#include "..\Common\bin.h"	// bin class for binary management
#include "CPlugin.h"

// Global allocator for edittime bin usage
class edittime_allocator : public bin_allocator {
public:
	BYTE* allocNew(unsigned int _size) {
		return new BYTE[_size];
	}
	void allocDelete(void* ptr) {
		delete[] ptr;
	}
};

extern edittime_allocator g_allocator;

OINFO* GetOINFO			(int objectID);
OINFO* GetOINFO			(CString Str);
OINFO* GetOINFOByFile	(CString Str);

// New functions, use whenever possible
const CPlugin&	GetPlugin		(int ID);
const CPlugin&	GetPlugin		(LPCSTR Name);
const CPlugin&	GetPluginByFile	(LPCSTR FileName);

class CLayout;

// Key access struct
struct KeyData {
	// Expansion, for strings
	int		iType; // 0 = normal, 1 = string
	bool	bSave; // save to .cap
	void*	pData; // data
	CString	String; // string
};

// Key access class
class KeyCollection {
public:
	map<CString, KeyData> keys;

	int GetSavedKeyCount();
	void Serialize(CArchive& ar, bool bNew);
};

#include "Structure\Effect.h"
#include "Structure\CMovement.h"
#include "Structure\CTransition.h"

#include "Structure\CObjectType.h"
#include "Structure\CObject.h"

#include "Structure\CLayer.h"

typedef CList<CLayer*, CLayer*&> CLayerList;
typedef CMap<long, long&, CObj*, CObj*&> CObjMap;

#include "Structure\CTypeChecker.h"
#include "Structure\CLayout.h"

// Resources
#include "Structure\Resources.h"
#include "Structure\CImageResource.h"
#include "Structure\CAnimation.h"

#include "Structure\EventSheet.h"

#include "Structure\ObjectFunctions.h"

#include "Structure\CApplication.h"
#include "Structure\CFamily.h"
#include "Structure\CContainer.h"

// Margin sizes
#define FRAME_MARGIN_W 480
#define FRAME_MARGIN_H 480

#define CAP_BEGINEVENTLIST ((BYTE)1)
#define CAP_BEGINEVENT ((BYTE)2)
#define CAP_BEGINCONDITIONS ((BYTE)3)
#define CAP_BEGINCONDITION ((BYTE)4)
#define CAP_ENDCONDITION ((BYTE)5)
#define CAP_ENDCONDITIONS ((BYTE)6)
#define CAP_BEGINACTIONS ((BYTE)7)
#define CAP_BEGINACTION ((BYTE)8)
#define CAP_ENDACTION ((BYTE)9)
#define CAP_ENDACTIONS ((BYTE)10)
#define CAP_BEGINPARAM ((BYTE)11)
#define CAP_ENDPARAM ((BYTE)12)
#define CAP_ENDEVENT ((BYTE)13)
#define CAP_ENDEVENTLIST ((BYTE)14)
#define CAP_BEGINGROUP ((BYTE)15)
#define CAP_ENDGROUP ((BYTE)16)

// Types
enum TYPE {
	TYPE_LAYOUT = 1,
	TYPE_APP,
	TYPE_OBJECT,
	TYPE_LAYER,
	TYPE_RESOURCE,
	TYPE_OBJECT_CAPTURED,
	TYPE_ANIMATION
};

typedef BOOL WINAPI RTCreateObject(editInfo *editObject);
typedef void WINAPI RTDestroyObject(editInfo *editObject);

typedef void (WINAPI * ETDLLLOAD)(MicroAceTime* at);

typedef void WINAPI GetInfo(LPOINFO);
typedef CMap<DWORD, DWORD&, OINFO*, OINFO*&> CDLLMap;
typedef map<int, CPlugin> CPluginMap;
typedef map<int, CPlugin>::iterator CPluginIterator;
extern CDLLMap *g_dlls;
extern vector<CString> g_PythonLib;

// Serialize bitmap
void SerializeBitmap(CxImage& hBitmap, CArchive& ar);

#include "Editors\LayoutEditor.h"

extern	CObjList		*g_pSel;
extern	CLayout			*g_pFrame;
extern	CApplication	*g_pApp;
extern	CLayoutEditor	*g_gLayoutView;
extern	CPoint			*g_pPt;
extern	DROPEFFECT		*g_pDrop;
extern	BOOL			g_bDuplicate;

//dragdrop types
#define DRAGDROP_OBJECTS "Construct_Objects"
class CLayoutEditor;

#include "Structure\CDragObjects.h"
