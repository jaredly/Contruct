#pragma once

#include "..\structure.h"

class ObjectFunctor {
public:
	virtual void operator()(CObj* pObj) {}
};

class ObjectTypeFunctor {
public:
	virtual void operator()(CObjType* pType) {}
};

class FrameFunctor {
public:
	virtual void operator()(CLayout* pFrame) {}
};

class LayerFunctor {
public:
	virtual void operator()(CLayer* pLayer) {}
};

class ExpTokenFunctor {
public:
	// Return true if the action or condition owning the expression should be deleted.
	virtual bool operator()(Token& pt) { return false; }
};

class ExpParamFunctor {
public:
	// Return true if the action or condition owning the expression should be deleted.
	virtual bool operator()(CEditorParam* param) { return false; }
};
class PrivateVariableInstanceAdder : public ObjectFunctor {
public:
	int id;
	CString value;

	PrivateVariableInstanceAdder() {}
	PrivateVariableInstanceAdder(int _id, CString _value) : id(_id), value(_value) {}

	void operator()(CObj* pObj)
	{
		// Simply add the new private variable to the instance
		pObj->m_values.push_back(InitValue(value, id));
	}
};

void GetTypesInFolder(CApplication* application, CString folder, vector<CObjType*>* objects);
void ForEachObjectType(CApplication* pApp, ObjectTypeFunctor& f);
void ForEachObjectInstance(CObjType* pType, CApplication* pApp, ObjectFunctor& f);
void ForEachFrame(CApplication* pApp, FrameFunctor& f);
void ForEachLayer(CApplication* pApp, LayerFunctor& f);
void ForEachExpressionToken(CApplication* pApp, ExpTokenFunctor& f);
void ForEachParameter(CApplication* pApp, ExpParamFunctor& f);
void EraseFamilyNameReferences(CApplication* pApp, CString familyName);
void ErasePrivateVariableReferences(CApplication* pApp, CObjType* pType, int varid);
void EraseGlobalVariableReferences(CApplication* pApp, int varid);
//void RenamePrivateVariableReferences(CApplication* pApp, CObjType* pType, CString oldName, CString newName);
CObjType* GetTypeFromName(CApplication* pApp, CString objName);

class TypesUsingFamilyCounter : public ObjectTypeFunctor {
public:
	CString family;
	int count;

	TypesUsingFamilyCounter() : count(0) {}
	TypesUsingFamilyCounter(CString _f) : family(_f), count(0) { family.MakeLower(); }

	void operator()(CObjType* pType) {
		vector<Family*>::iterator i = pType->families.begin();

		for ( ; i != pType->families.end(); i++) {
			CString curName = (*i)->name;
			curName.MakeLower();
			if (curName == family)
				count++;
		}
	}
};

class CountObjectInstances : public ObjectFunctor {
public:
	int value;

	CountObjectInstances() : value(0) {}

	void operator()(CObj* pObj)
	{
		value ++;
	}
};


class GetVectorOfEmptyObjectTypes : public ObjectTypeFunctor {
public:
	list<CObjType*> objectTypesToRemove;
	CApplication* application;	
	
	GetVectorOfEmptyObjectTypes() : application(0){}
	GetVectorOfEmptyObjectTypes(CApplication* pApp) : application(pApp) {}

	void operator()(CObjType* pType) {
		CountObjectInstances counter;
		ForEachObjectInstance(pType, application, counter);

		if(counter.value == 0){
			objectTypesToRemove.push_back(pType);
		}
	}
};

class FrameNameTester : public FrameFunctor {
public:
	CString name;
	bool exists;

	FrameNameTester() : exists(false) {}

	void operator()(CLayout* pLayout)
	{
		CString l = pLayout->m_WndName;
		l.MakeLower();
		name.MakeLower();

		if (l == name)
			exists = true;
	}
};