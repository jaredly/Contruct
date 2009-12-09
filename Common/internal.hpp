// Internal.h
// Definitions and setups for the SDK

// Only include once
#ifndef INTERNAL_H
#define INTERNAL_H

#pragma once
#include "bin.h"

// Struct prototypes
struct PropertyTableEntry;
struct EINFO;

// DefineACES prototype
void DefineACES(MicroAceTime* at);

void WINAPI ETDllLoad(MicroAceTime* at)
{
	DefineACES(at);
}

int WINAPI RTDllLoad(MicroAceTime* at)
{
	DefineACES(at);
	return IDE_FLAGS;
}

inline void SerializeVersion(bin& ar, int& v)
{
	if (ar.loading)
		ar >> v;
	else
		ar << v;
}

inline bool CompareStrings(const CString& l, const CString& r, bool caseSensitive = false)
{
	if (caseSensitive)
		return l == r;
	else {
		CString _l = l;
		CString _r = r;
		_l.MakeLower();
		_r.MakeLower();
		return _l == _r;
	}
}

// Edit data stores important information such as identifier, X, Y, W, H and data
//
class editInfo
{
public:
	float  objectX,
		    objectY,
		    objectWidth,
		    objectHeight,
	        objectAngle,
			originX,
			originY;
	EditExt*   objectPtr;
	DWORD DLLIndex;
	long    instanceID,
		    ObjectIdentifier,
		    globalID;

	VEditTime* pEditTime;
	RECTF* BoundingRect;

	unsigned long	filter;

};

// Parameter enumeration
enum PARAMS 
{
	PARAM_VALUE = 1,
	PARAM_STRING,
	PARAM_COLOR,
	PARAM_POSITION,
	PARAM_ZONE,
	PARAM_OBJECT,
	PARAM_KEYBOARD,
	PARAM_MOUSE,
	PARAM_COMBO,
	PARAM_UNDEFINED,
	PARAM_ARRAY,
	PARAM_LAYER,
	PARAM_GLOBALVARIABLE,
	PARAM_PRIVATEVARIABLE,
	PARAM_MENURESOURCE,
	PARAM_BINARYRESOURCE,
	PARAM_TRANSITION,
	PARAM_CUST1 = 128,
	PARAM_CUST2,
	PARAM_CUST3,
	PARAM_CUST4,
	PARAM_CUST5,
	PARAM_CUST6,
	PARAM_CUST7,
	PARAM_CUST8,
	PARAM_CUST9,
	PARAM_CUST10,
	PARAM_CUST11,
	PARAM_CUST12,
	PARAM_CUST13,
	PARAM_CUST14,
	PARAM_CUST15,
	PARAM_CUST16,
	PARAM_OPTIONAL = 1 << 8,
	PARAM_VARIABLE = 1 << 9,
	PARAM_NOCAST = 1 << 10,
};

// Object information
#define OBJ_ID	0x00000000

// Convert radians to degrees
#ifndef DEGREES
#define DEGREES(r) ((r) * 57.295779513082320876798154814105)
#endif
// Degrees to radians
#ifndef RADIANS
#define RADIANS(r) ((r) / 57.295779513082320876798154814105)
#endif

// General value clamping function
template<class T>
inline void Clamp(T& value, T lower, T upper)
{
	if (value < lower) value = lower;
	if (value > upper) value = upper;
}

/*
// Premultiply D3DCOLOR
inline void Premultiply(D3DCOLOR& c)
{
	float r = (float)D3D_R(c) / 255.0f;
	float g = (float)D3D_G(c) / 255.0f;
	float b = (float)D3D_B(c) / 255.0f;
	int _a = D3D_A(c);
	float a = (float)_a / 255.0f;

	r *= a;
	g *= a;
	b *= a;

	c = D3D_ARGB(_a, (int)(r * 255.0f), (int)(g * 255.0f), (int)(b * 255.0f));
}
*/

#include "..\Common\ExpReturn.hpp"

/*
inline void SerializeVersion(bin& ar, int& v)
{
	if (ar.loading)
		ar >> v;
	else
		ar << v;
}
*/

//////////////////////////////////////////////////////////////////////////////////
// EDITTIME
#ifndef RUN_ONLY

BOOL WINAPI ETCreateObject(editInfo* editObject, VEditTime* pVEditTime) 
{
	editObject->pEditTime = pVEditTime;

	// Allocate and construct edit object
	editObject->objectPtr = new EditExt(pVEditTime, editObject);

	return TRUE; // No errors
}

// The object is initialized every time the frame editor is opened.  This is where you create
// textures, fonts, and any other frame editor volatile data
BOOL WINAPI ETInitializeObject(editInfo* editObject, VEditTime* pVEditTime) 
{
	editObject->objectPtr->pEditTime = pVEditTime;
	editObject->objectPtr->Initialize();

	return TRUE; // No errors
}

// This object is being destroyed
BOOL WINAPI ETDestroyObject(editInfo* editObject) 
{
	delete editObject->objectPtr;

	return TRUE; // No errors
}

// Draw object at edittime
int WINAPI ETDrawObject(editInfo *editObject) 
{
	editObject->objectPtr->Draw();

	return 0;
}

void WINAPI ETAddDefaultEvent(editInfo* editObject, int& DefaultCondition)
{
	//editObject->objectPtr->AddDefaultEvent(DefaultCondition);
}

void WINAPI ETSerialize(editInfo* editObject, bin& ar)
{
	editObject->objectPtr->Serialize(ar);
}

void WINAPI ETPutObject(editInfo* editObject)
{
	editObject->objectPtr->OnPut();
}

void WINAPI ETRemoveObject(editInfo* editObject)
{
	editObject->objectPtr->OnRemoved();
}

BOOL WINAPI ETSizeObject(editInfo* editObject) 
{
	return editObject->objectPtr->OnSizeObject();
}

void WINAPI ETGetAnimationHandle(editInfo* editObject, int& iAnimation) 
{
	editObject->objectPtr->GetAnimationHandle(iAnimation);
}

// No need for routines at edittime
void* WINAPI GetRoutines(TABLE table) 
{
	return NULL;
}

// Link variables to properties, then return the address of the property table
PropertyTableEntry* WINAPI LinkPropTable();

// Draw the object
BOOL WINAPI ETCreateObject(editInfo* editObject, VEditTime* pEditTime);
BOOL WINAPI ETCreateClone(editInfo* editObject);
int WINAPI ETDrawObject(editInfo *editObject, HDC dc);
void WINAPI ETOnPropertyChanged(editInfo *editObject);

PropertyTableEntry PropTable[];

// Edittime DefineACES macros
#define ADDPARAM(t, n, d)			at->Param(t, n, d, "")
#define ADDPARAMDEF(t, n, d, v)		at->Param(t, n, d, v)
#define ADDPARAMCOMBO(n, d, v)		at->Param(PARAM_COMBO, n, d, v)
#define ADDPARAMRESOURCE(n, d, v)	at->Param(PARAM_RESOURCE, n, d, v)
#define ADDCND(n, c, d, f, s, r)	at->EditAce(n, c, d, s, r, ACETYPE_CONDITION)
#define ADDACT(n, c, d, f, s, r)	at->EditAce(n, c, d, s, r, ACETYPE_ACTION)
#define ADDEXP(n, c, d, f, r)		at->EditAce(n, c, d, d, r, ACETYPE_EXPRESSION)

// Get object information
#ifndef MINIMUM_VERSION
#define MINIMUM_VERSION 1
#endif

void WINAPI GetInfo(LPOINFO objectInfo) 
{
	objectInfo->MinimumVersion	= MINIMUM_VERSION;
	objectInfo->ideFlags		= IDE_FLAGS;
}

////////////////////////////////////////////////////////////////////////////////////////
// RUNTIME
#else

inline bool AsyncKeyDown(int vk)
{
	return (GetAsyncKeyState(vk) >> 15) != 0;
}

// No need for tables at runtime
vector<ACESEntry>* WINAPI GetTable(TABLE table) 
{
	return NULL;
}

inline int MakeConditionTrigger(VRuntime* pRuntime, int truth)
{
	void* pCnd = pRuntime->GetCurrentConditionPtr();

	static map<void*, int> cndMap;

	map<void*, int>::iterator i = cndMap.find(pCnd);

	// No record
	if (i == cndMap.end()) {
		// Insert a record and allow event to run according to truth
		cndMap[pCnd] = truth;
		return truth;
	}
	else {
		int lastTruth = i->second;
		i->second = truth;

		// True when was false last time, and true this time.
		return !lastTruth && truth;
	}
}

// Runtime DefineACES macros
#define ADDPARAM(t, n, d)
#define ADDPARAMDEF(t, n, d, v)	 
#define ADDPARAMCOMBO(n, d, v)
#define ADDPARAMRESOURCE(n, d, v)
#define ADDCND(n, c, d, f, s, r) at->RunAce((ACROUTINE)f, s, ACETYPE_CNDFUNC, r)
#define ADDACT(n, c, d, f, s, r) at->RunAce((ACROUTINE)f, s, ACETYPE_ACTFUNC, r)
#define ADDEXP(n, c, d, f, r) at->RunAce((ACROUTINE)f, d, ACETYPE_EXPFUNC, 0)

inline void SetInitialPrivateVariable(VRuntime* pRuntime, CRunObject* obj, int index, ExpStore& v)
{
	int type;
	const char* str;

	pRuntime->GetPrivateVariableInfo(obj, index, type, str);

	// Set the store
	switch (type) {
	case 0:	// Numeric type - set as float
		v = (double)atof(str);
		return;
	case 1: // String type
		v = str;
		return;
	}
}

inline void SetupPrivateVariableVector(VRuntime* pRuntime, CRunObject* obj, vector<ExpStore>& vec)
{
	int numVars = pRuntime->GetPrivateVariableCount(obj);
	vec.resize(numVars);

	for (int i = 0; i < numVars; i++)
		SetInitialPrivateVariable(pRuntime, obj, i, vec[i]);
}

inline bool CheckDebugVariableChange(const char* label, const char* value, vector<ExpStore>& vec, CRunObject* obj, VRuntime* pRuntime)
{
	// Check if variable name
	CString varName = label;

	if (varName.Left(1) == "'") {

		// Strip quotes
		varName = varName.Mid(1, varName.GetLength() - 2);

		// Get index
		int index = pRuntime->GetVariableIndexFromName(varName, obj);

		// Update.  Don't allow private variables to change type this way, just change their value.
		ExpStore& var = vec[index];

		switch(var.Type()) {
		case EXPTYPE_INTEGER:
			var = _atoi64(value);
			break;
		case EXPTYPE_FLOAT:
			var = atof(value);
			break;
		case EXPTYPE_STRING:
			var = value;
			break;
			// Other types unsupported
		}
		return true;
	}
	else return false;
}

/*
inline void ExpReturnInteger(VRuntime* pRuntime, __int64 i)
{
	pRuntime->ExpReturnStack[pRuntime->curExpStackDepth]->SetRaw(i);
}

inline void ExpReturnFloat(VRuntime* pRuntime, double f)
{
	pRuntime->ExpReturnStack[pRuntime->curExpStackDepth]->SetRaw(f);
}

inline void ExpReturnString(VRuntime* pRuntime, CString& s)
{
	CString* pStr = &(pRuntime->tempStrings[pRuntime->curtempstr]);

	pRuntime->ExpReturnStack[pRuntime->curExpStackDepth]->SetRaw(pStr);

#ifdef CSDK_COMPATIBLE_COMPILER
	// Compatible compilers just assign the CString
	*pStr = s;
	pRuntime->curtempstr++;
	pRuntime->curtempstr %= 32;
#else
	// Incompatible compilers assign through VRuntime (slower but works)
	pRuntime->ReturnStringWorkaround((const char*)s);
#endif
}

// VRuntime::AssignCString needed with return value
inline CString* ExpGetReturnString(VRuntime* pRuntime)
{
	ExpReturn* er = pRuntime->ExpReturnStack[pRuntime->curExpStackDepth];
	CString* ret = &(pRuntime->tempStrings[pRuntime->curtempstr]);

	er->SetRaw(ret);
	pRuntime->curtempstr++;
	pRuntime->curtempstr %= 32;

	return ret;
}

inline void ExpSetReturnString(VRuntime* pRuntime, CString* pStr)
{
	pRuntime->ExpReturnStack[pRuntime->curExpStackDepth]->SetRaw(pStr);
}

inline void ExpReturnCustom(VRuntime* pRuntime, ExpReturn* value)
{
	pRuntime->ExpReturnStack[pRuntime->curExpStackDepth]->SetRaw(*value);
}

inline void ExpReturnArray(VRuntime* pRuntime, ExpReturn* array, int count)
{
	pRuntime->ExpReturnStack[pRuntime->curExpStackDepth]->SetRaw(array, count);
}
*/

// Allocate and construct object
CRunObject* WINAPI RTCreateObject(initialObject* editObject, VRuntime* pRuntime) 
{
	return new ExtObject(editObject, pRuntime);
}

// Destruct and deallocate object
void WINAPI RTDestroyObject(ExtObject* object)
{
	delete object;
}

// Handy parameter defs
#define GetIntParam(a, b) (a[b].GetInt())
#define GetInt64Param(a, b) (a[b].GetInt64())
#define GetFloatParam(a, b) (a[b].GetFloat())
#define GetDoubleParam(a, b) (a[b].GetDouble())
#define GetStringParam(a, b) ((a[b].GetString()))
#define GetColourParam(a, b) (a[b].GetInt()) //richtodo

// Get number parameters
inline int GetNumParams(LPVAL theParams)
{
	LPVAL ptr = theParams;
	for ( ; ptr->Type() != EXPTYPE_NULL; ptr++);
	return ptr - theParams;
}

template<typename T>
inline long DoComparison(int construct_operator, const T& l, const T& r)
{
	switch (construct_operator) {
	case 0:
		return l == r;
	case 1:
		return l != r;
	case 2:
		return l < r;
	case 3:
		return l <= r;
	case 4:
		return l > r;
	case 5:
		return l >= r;
	default:
		__assume(0);
	}
}
/*
#define ReturnInt(a) ExpReturnInteger(pRuntime, a); return 0

#define ReturnFloat(a) ExpReturnFloat(pRuntime, a); return 0

#define ReturnString(a) ExpReturnString(pRuntime, a); return 0

#define ReturnArray(a, s) ExpReturnArray(pRuntime, a, s); return 0

#define GetReturnString() ExpGetReturnString(pRuntime)

#define SetReturnString(a) ExpSetReturnString(pRuntime, a)

#define ReturnCustom(a) ExpReturnCustom(pRuntime, a); return 0
  */
		
#endif //ifndef else RUNTIME

#endif //INTERNAL_H