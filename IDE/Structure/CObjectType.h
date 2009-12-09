#include "..\StdAfx.h"
#pragma once

typedef list<CBehavior>::iterator BehaviorIterator;
typedef const list<CBehavior>::const_iterator BehaviorConstIterator;

typedef list<CEffect>::iterator EffectIterator;
typedef const list<CEffect>::const_iterator EffectConstIterator;

class MenuMapItem
{
	public:
		int type; // 0 = normal, 1 = movement, 2 = effect, 3 = custom ace 
		int menuid;
		int child; // movement 0 = 0, movement 1 = 1 etc..
};
typedef CMap<int,int,MenuMapItem,MenuMapItem> ACEMenuMap;


struct FamilyLookup
{
	CString		m_FamilyName;
	vector<int> m_ObjectTypeID;
	class Family* m_Family;
};

#define PRIVATEVARTYPE_NUMBER	0
#define PRIVATEVARTYPE_TEXT		1

struct PrivateValue {
	CString name;
	int type;		// 0 = number, 1 = text, use above defs instead of numbers
	int identifier;

	PrivateValue() {}
	PrivateValue(const CString& p_name, int p_type, int p_identifier) : name(p_name), type(p_type), identifier(p_identifier) {}

	// Compare based on name: for exists_element()
	bool operator==(const CString& _r) const {
		CString r = _r;
		CString l = name;
		r.MakeLower();
		l.MakeLower();
		return l == r;
	}
};

class CObjType
{
public:
    // Constructors
	CObjType() :
		tree(NULL),
		folder("Default")
	{
		SetName(_T(""));
		m_bIsGroupType = FALSE;
		m_pContainer = NULL;
		m_bGlobal = false;
		VariableID = 0;
		destroyWhen = SVA_NOSETTING;
		m_pEffect = NULL;
		m_pBehavior = NULL;
		m_bShowInEventSheetEditor = true;
		category = "Object";
	}

	~CObjType() ;

	// For sorting
	bool operator < (const CObjType& other) const {
		return strcmp(m_Name, other.GetName());
	}

    // Attributes
    void SetName(LPCSTR);

	void SetFolder(LPCSTR folder_);
	CString GetFolder();

	CString GetName() const 
	{
		return m_Name;
	}

    // Operations
	bool Serialize(CArchive& ar);

	// Get a table's count
	int GetTableCount(int table);
	vector<ACESEntry2>* GetTable(int table);

	// Get a table entry
	ACESEntry2* GetACESEntry(int table, long index);
	ACESEntry2* GetACESEntry(int table, MenuMapItem& ChosenItem, int& i);
	int GetBehaviorIndexFromUniqueID(int id);

	// Load bitmaps
	void LoadBitmaps();

	// Identifiers and name
	DWORD   DLLIndex;
	long    ObjectIdentifier;
	BOOL	m_bIsGroupType;

	// Bitmap
	bool    b_IsCustom;
	CxImage m_Image;
	CxImage small_image;

	// tree
	HTREEITEM tree;

	list  <CBehavior>		behaviors;
	CBehavior*				GetBehaviorByIndex(int id);
	CBehavior*				GetBehaviorByUniqueID(int id);
	CBehavior*				GetBehaviorByName(CString name);
	list  <CEffect>			effects;
	CEffect*				GetEffect(int id);
	CEffect*				GetEffectByIndex(int index);
	vector<CString>			traits;
	vector<PrivateValue>	m_PrivateValues;
	PrivateValue*			GetPrivateVariable(int id);
	PrivateValue*			GetPrivateVariable(CString varName);
	bool					PrivateVariableExists(CString varName);

	CString					GetUniqueEffectName(CString startName);
	CString					GetUniqueBehaviorName(CString startName);

	// For custom aces
	vector<ACESEntry2> m_CustomActions;
	vector<ACESEntry2> m_CustomConditions;
	vector<ACESEntry2> m_CustomExpressions;

	// Families belonged to
	vector<class Family*> families;

	bool m_bGlobal; // Global object

	DisableShaderWhen destroyWhen;	// Destroy based on pixel shader version

	// Containers - this is done by pContainer now.
	class CContainer* m_pContainer;

	int VariableID;
	CEffect* m_pEffect; // used if the CObjType has become an effect
	CBehavior* m_pBehavior; // used if the CObjType has become a movement

	CObjType(const CObjType &obj)
	{

	}

	void CreateFromBehavior(CBehavior* pMov);
	void CreateFromEffect(CEffect* pEff);

	bool m_bShowInEventSheetEditor;

	CString category;

protected:

	CString m_Name;
	CString folder;

};

typedef CMap<long, long&, CObjType*, CObjType*&> CObjTypeMap;

// Utility function prototypes
CObjType* LookupObjectType(CString name, CObjTypeMap* objMap);
CObjType* LookupObjectType(int oid, CObjTypeMap* objMap);
CBehavior* LookupBehavior(CObjType* pType, CString naem);