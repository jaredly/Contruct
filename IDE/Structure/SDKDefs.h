#pragma once

#include "..\..\Common\Vedittime.h"

// sdk symbols
#define OBJ_NAME                        1
#define OBJ_AUTHOR                      2
#define OBJ_VERSION                     3
#define OBJ_DESCRIPTION                 4
#define OBJ_DATE                        5
#define OBJ_COPYRIGHT                   6
#define OBJ_WEB                         7
#define OBJ_EMAIL                       8
#define OBJ_CATEGORY                    9
#define OBJ_ICON                        101
#define OBJ_SICON                       102

#define EINFO LPBYTE


class editInfo
{
public:
	editInfo()
	{
		objectX = 0;
		objectY = 0;
		objectWidth = 0;
		objectHeight = 0;
		objectAngle = 0;
		originX = 0;
		originY = 0;
		objectPtr = 0;
		DLLIndex = -1;
		ObjectIdentifier = -1;
		globalID = -1;
		peditInfo = 0;
		BoundingRect = 0;
		filter = -1;
	}
	float  objectX,
		    objectY,
		    objectWidth,
		    objectHeight,
			objectAngle,
			originX,
			originY;
	void*   objectPtr;
	DWORD   DLLIndex;
	long    instanceID,
		    ObjectIdentifier,
		    globalID;

	VEditTime* peditInfo;
	RECTF* BoundingRect;

	unsigned long	filter;
};

typedef void (WINAPI * ETDRAWOBJECT)       (class editInfo* editData);
typedef BOOL (WINAPI * ETCREATEOBJECT)     (class editInfo* editData, VEditTime* pVeditInfo);
typedef BOOL (WINAPI * ETINITIALIZEOBJECT) (editInfo* EditObject, VEditTime* pVeditInfo);
typedef void (WINAPI * ETONPROPERTYCHANGED)(editInfo* EditObject, LPCSTR szLabel, int iMode, struct CVirtualPropItem* item, class PropReturn* Property, class MicroEdittime* pm);
typedef void (WINAPI * ETSERIALIZE)        (editInfo* EditObject, bin& ar);
typedef void (WINAPI * ETPUTOBJECT)        (editInfo* EditObject);
typedef void (WINAPI * ETREMOVEOBJECT)     (editInfo* EditObject);
typedef void (WINAPI * ETDESTROYOBJECT)    (editInfo* EditObject);
typedef void (WINAPI * ETADDDEFAULTEVENT)  (editInfo* EditObject, int& ConditionNumber);
typedef void (WINAPI * ETONMESSAGE)		   (editInfo* EditObject, int message);
typedef void (WINAPI * ETONNOTIFY)		   (editInfo* EditObject, int notify);
typedef void (WINAPI * ETGETANIMATION)	   (editInfo* EditObject, int& iAnimation);

#include "..\..\Common\SDKdefs.h"

typedef void (WINAPI * ETADDDEPENDENCIES)  (MicroDependency& Deps);

// SDK
/*enum PROPERTY {
	PROPERTY_END = -1,
	PROPERTY_STATIC = 1,
	PROPERTY_TEXT,
	PROPERTY_VALUE,
	PROPERTY_BOOL,
	PROPERTY_COMBO,
	PROPERTY_COLOR,
	PROPERTY_FONT,
	PROPERTY_BUTTON
};*/

/*typedef struct {
    PROPERTY	type;
	long		parentId;
	char		szLabel[256];
	char		szInfo[256];
	long		textLen;
	char		comboOptions[256];
	long		offset;
} PropertyTableEntry;*/

// PARAM and ACESEntry
// new versions converted
typedef struct PARAM PARAM2;
typedef struct ACESEntry ACESEntry2;

/*struct CPropItem
{
	int     Type;
	CString Label;
	CString Description;
	CString Text;
};*/

/*struct CVirtualPropItem
{
	int     Type;
	void*   Label;
	void*   Description;
	void*   Text;
};*/

#include "..\..\..\Common\PropReturn.hpp"

// Micro edittime tool
class MicroPropertyVector : public MicroEdittime {
public:
	MicroPropertyVector(vector<CPropItem>* p, CPropItem* i);
	MicroPropertyVector();
	
	void Proc(void* x);

	void Assign(void* s, const char* p);

	vector<CPropItem>* pVec;
	CPropItem* pItem;
};

typedef vector<ACESEntry2> ACEList;

// For abstracting STL usage in ACE code
class MicroAceTool : public MicroAceTime {
public:
	MicroAceTool(ACEList* c, ACEList* a, ACEList* e);

	void RunAce(ACROUTINE f, const char* scriptName, int type, int flags);
	void Param(int type, const char* name, const char* desc, const char* init);

	// OLD FORMAT
	// Menu ID, ACE Name, ACE Desc, flags, type, index
	//void EditAce(short resID, const char* aceName, const char* aceDesc, short flags, int type, int index = -1);

	// NEW FORMAT
	// ACE Name, ACE Display text, Flags, Type, Index
	void EditAce(const char* aceName, const char* aceCategory, const char* aceDisplay, const char* scriptName, short flags, int type, int index = -1, int reserved = 0);

	ACESEntry2 ace;
	ACEList* cnds;
	ACEList* acts;
	ACEList* exps;
};