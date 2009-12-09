// CONSTRUCT SDK DEFINITIONS
#ifndef _SDKDEFS_H_
#define _SDKDEFS_H_

// IDE properties
#define OF_NOSIZE				0x0001		// Extension cannot be resized at edittime
#define OF_NODRAW				0x0002		// Extension is not drawn at edittime
#define OF_ALLOWANGLES			0x0004		// Allow edittime angles?
#define OF_PRIVATEVARIABLES		0x0008		// Has private values
#define OF_MOVEMENTPLUGIN		0x0010		// Is a Movement Extension
#define OF_MOVEMENTS			0x0020		// Allow movements
#define OF_EFFECTS				0x0040		// Allow effects
#define OF_NOCOMMONDEBUG		0x0080		// Don't show 'Common' field (X/Y/W/H/Angle/Opacity) in debugger
#define OF_NODEBUG				0x0100		// Don't show at all in debugger
#define OF_UNDEFINEDEXPRESSIONS 0x0200		// Allow ReturnUndefinedExpression to be called
#define OF_DEFAULTVALUE_PRIVATEVAR 0x0400	// Tells the type checker the only valid syntax for default value is Myobject('Myvalue')
#define OF_SINGLEDRAWOP			0x0800		// Rendering optimisation for plugins that simply call one draw command eg. Blitrc() eg. Sprite

// Warning disables
#pragma warning(disable: 4786)	// debug info truncated 255 chs
#pragma warning(disable: 4018)	// signed/unsigned mismatch
#pragma warning(disable: 4244)	// conversion from x to y
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

// PARAM struct for ACEs
struct PARAM
{
	unsigned short type;
	CString  name;
	CString  desc;
	CString  initStr;
};

// For abstracting STL and MFC
class MicroEdittime {
public:
	virtual void Proc(void* p) = 0;						// Vector push_back abstraction
	virtual void Assign(void* p, const char* s) = 0;	// CString operator= abstraction
};

#define ACETYPE_CONDITION	0
#define ACETYPE_ACTION		1
#define ACETYPE_EXPRESSION	2
#define ACETYPE_CNDFUNC		3
#define ACETYPE_ACTFUNC		4
#define ACETYPE_EXPFUNC		5
#define ACETYPE_EXPNAME		6

// Edittime compiles dont care about ACROUTINE but need MicroAcetime to compile
#ifdef CONSTRUCT
#define ACROUTINE int

// For abstracting STL usage in ACE code
class MicroAceTime {
public:
	virtual void RunAce(ACROUTINE f, const char* p, int type, int flags) = 0;
	virtual void Param(int type, const char* name, const char* desc, const char* init) = 0;
	virtual void EditAce(const char* aceName, const char* aceCategory, const char* aceDisplay, const char* scriptName, short flags, int type, int index = -1, int reserved = 0) = 0;
};

typedef struct PARAM PARAM2;
typedef struct ACESEntry ACESEntry2;

#endif //CONSTRUCT

// ACESEntry: describes an Action, Condition or Expression
struct ACESEntry
{
	//////////////////////////////////////////
	short			resourceID;				// DEPRECATED
	CString			aceName;				// DEPRECATED
	CString			aceDescription;			// DEPRECATED
	//////////////////////////////////////////

	short			retrn;				// flags
	vector<PARAM>	params;

	CString aceListName;				// Name to appear in step 2 of the insert wizard (categorised list of ACEs)
	CString aceCategory;				// ACE category for step 2
	CString aceDisplayText;				// Formatted string to display in event sheet eg. "%o On Object %0 clicked"
	CString scriptName;					// The script name.
	CString auxStr;						// Sys params/additional info
};

// Shortcut to object data
#define editData editObject->objectData

// saved editTime object
class initialObject
{
public:

	long        eX,
		        eY,
		        eWidth,
		        eHeight;
	float       eAngle;
	DWORD       extID;
	long        instanceID,
		        objectID;

	BYTE* eData;
	long eSize;

	//D3DCOLOR eFilter;
	cr::color eColor;
};

struct CPropItem
{
	int     Type;
	CString Label;
	CString Description;

	CString Text;
};

// SDK abstraction
struct CVirtualPropItem
{
	int      Type;
	void*    Label;
	void*    Description;

	void*    Text;
};

// Property enumeration
enum PROPERTY 
{
	PROPERTY_END = -1,
	PROPERTY_STATIC = 1,
	PROPERTY_TEXT,
	PROPERTY_VALUE,
	PROPERTY_BOOL,
	PROPERTY_COMBO,
	PROPERTY_COLOR,
	PROPERTY_FONT,
	PROPERTY_BUTTON
};

// Property table entry structure
struct PropertyTableEntry
{
    PROPERTY	type;
	long		parentId;
	char		szLabel[256];
	char		szInfo[256];
	long		textLen;
	char		comboOptions[256];
	long		offset;
};

#ifdef CONSTRUCT
struct AceListEntry {
	CString listname;		// Name to appear in list

	int aceID;				// Condition ID, Action ID or Expression ID (index to original vector)
	ACESEntry* pAce;		// Pointer to ACE

	bool operator<(const AceListEntry& other) const {
		CString l = listname;
		CString r = other.listname;

		l.MakeLower();
		r.MakeLower();

		if (l.Left(1) == "*")
			l.Delete(0);
		if (r.Left(1) == "*")
			r.Delete(0);

		return l < r;
	}
};

struct AceCategory : public NameComparableCIS {

	AceCategory() {}
	AceCategory(const CString& _name) : NameComparableCIS(_name) {}

	vector<AceListEntry> aces;
};

#endif

class MicroDependency {
public:
    virtual void Add(const char* dll) = 0;
};

// Object info struct
struct OINFO
{
	int ideFlags;
	HINSTANCE hInstLib;
	DWORD oID;
	DWORD eSize;
	PropertyTableEntry* PropTable;
	RECT rc;
	LPBYTE objectDataUnused;
	char extFileName[MAX_PATH];
	char extName[256];

	int MinimumVersion;

	HBITMAP smallIcon;

#ifdef CONSTRUCT

	vector<ACESEntry2>* GetTable(int type);
	ACESEntry* GetACEEntry(int type, int index);

	vector<ACESEntry2>	conditions;
	vector<ACESEntry2>	actions;
	vector<ACESEntry2>	expressions;

	vector<AceCategory> cndCategories;
	vector<AceCategory> actCategories;
	vector<AceCategory> expCategories;

	ETDRAWOBJECT        ETDrawObject;
	ETCREATEOBJECT      ETCreateObject;
	ETDESTROYOBJECT     ETDestroyObject;
	ETINITIALIZEOBJECT  ETInitializeObject;
	ETONPROPERTYCHANGED ETOnPropertyChanged;
	ETSERIALIZE         ETSerialize;
	ETPUTOBJECT         ETPutObject;
	ETREMOVEOBJECT      ETRemoveObject;
	ETADDDEFAULTEVENT   ETAddDefaultEvent;
	ETONMESSAGE			ETOnMessage;
	ETONNOTIFY			ETOnNotify;
	ETGETANIMATION		ETGetAnimationHandle;

	// Icons for object
	HBITMAP hLargeIcon;
	HBITMAP hSmallIcon;

	vector<CPropItem> Properties;

	// Has animations?
	bool bHasAnimations;
	
#endif

};

typedef OINFO* LPOINFO;

// ACE flags
enum RETURNS 
{
	RETURN_INTEGER = 0x0001,		// Expression returning integer
	RETURN_STRING = 0x0002,			// Expression returning string
	CND_TRIGGERED = 0x0004,			// Trigger, by GenerateEvent
	CND_FASTTRIGGERED = 0x0008,		// Fast trigger, by GenerateEventFast
	RETURN_ARRAY = 0x0010,			// Expression returning an array
	SOL_MODIFIER = 0x0020,			// Condition or action calling Select, QuerySelect, on other objects
	RETURN_FLOAT = 0x0040,			// Expression returning a float
	CND_STATIC = 0x0080,			// Condition not called for all instances and must call Select() on self
};

#define RETURN_VALUE	RETURN_INTEGER	//backwards compatability


#endif // _SDKDEFS_H_