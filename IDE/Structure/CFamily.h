#pragma once
#include "CObjectType.h"

class FamilyAssociate
{
public:
	int object;
	int item_id;
	FamilyAssociate(){}
	FamilyAssociate(int _object, int _item_id){object = _object; item_id = _item_id;}
};

class FamilyShare
{
public:
	// Associate a single id with a bunch of other id's
	int unique_id;
	vector<FamilyAssociate> reference_ids;
	CString name;

	bool valid;

	bool Serialize(CArchive& ar);
};

class FamilyBehavior : public FamilyShare
{
public:
	int m_DLLIndex;
};

class FamilyEffect : public FamilyShare
{
public:
	CString effect_file;
};

class FamilyVariable : public FamilyShare
{
public:

	bool operator == (const CString& other_name) const 
	{
		CString l = name;
		CString r = other_name;

		l.MakeLower();
		r.MakeLower();

		return l == r;
	}
};

class Family
{
public:

	Family() :
		is_in_use(true)
	{
	}

	CString name;
	CxImage image;
	CxImage small_image;

	int	 identifier;

	bool Serialize(CArchive&);
	void UpdateFamilyData();
	bool VariableNameExists(CString);

	bool is_in_use;

	// note: check that 'valid' flag is on for these to ensure they really exist.
	// 'valid' is turned off when they are deleted but they are not removed from the vector; this is for the
	// undo system.
	vector<FamilyBehavior>		behaviors;
	vector<FamilyEffect>		effects;
	vector<FamilyVariable>		variables;

	CObjType					m_FamilyTemp; 

	CBehavior*					GetReferenceBehavior(int);
	CEffect*					GetReferenceEffect(int);
	PrivateValue*				GetReferenceVariable(int);

	DWORD DLLIndex;
	void GetObjectTypes(vector<CObjType*>& objects);
	void GetObjectTypesKeys(vector<long>& objects);
	CApplication* application;	// pointer to owner application

	bool operator == (const Family& other) const 
	{
		CString l = name;
		CString r = other.name;

		l.MakeLower();
		r.MakeLower();

		return l == r;
	}

	bool operator == (const CString& name_) const 
	{
 		CString l = name;
		CString r = name_;

		l.MakeLower();
		r.MakeLower();

		return l == r;
	}
};