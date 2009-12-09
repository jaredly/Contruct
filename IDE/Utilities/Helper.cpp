#include "stdafx.h"
#include "Helper.h"

void GetTypesInFolder(CApplication* application, CString folder, vector<CObjType*>* objects)
{
	long unused;

	POSITION position = application->object_types.GetStartPosition();

	while (position) 
	{
		CObjType* type;
		application->object_types.GetNextAssoc(position, unused, type);

		if (type->GetFolder() == folder)
			objects->push_back(type);
	}
}

void ForEachObjectInstance(CObjType* pType, CApplication* pApp, ObjectFunctor& f)
{
	long unused;

	// Iterate each layout
	POSITION layoutPos = pApp->layouts.GetHeadPosition();

	while (layoutPos) {
		CLayout* layout = pApp->layouts.GetNext(layoutPos);

		// Iterate each instance
		POSITION instancePos = layout->objects.GetStartPosition();

		while (instancePos) {
			CObj* pObj;
			layout->objects.GetNextAssoc(instancePos, unused, pObj);

			// Call operator() on the function object if it's the type we want
			if (pObj->editObject->ObjectIdentifier == pType->ObjectIdentifier)
				f(pObj);
		}
	}
}

void ForEachObjectType(CApplication* pApp, ObjectTypeFunctor& f)
{
	// These containers suck.  Dear god.  Why why why.
	long unused;

	// Iterate object types
	POSITION typePos = pApp->object_types.GetStartPosition();

	while (typePos) {
		CObjType* pType;
		pApp->object_types.GetNextAssoc(typePos, unused, pType);

		// Call operator() on the function object
		f(pType);
	}
}

void ForEachFrame(CApplication* pApp, FrameFunctor& f)
{
	// Iterate each layout
	POSITION layoutPos = pApp->layouts.GetHeadPosition();

	while (layoutPos) {
		CLayout* layout = pApp->layouts.GetNext(layoutPos);

		f(layout);
	}
}

void ForEachLayer(CApplication* pApp, LayerFunctor& f)
{
	// Iterate each layout
	POSITION layoutPos = pApp->layouts.GetHeadPosition();

	while (layoutPos)
	{
		CLayout* layout = pApp->layouts.GetNext(layoutPos);

		POSITION layerPos = layout->layers.GetHeadPosition();

		while (layerPos) 
		{
			CLayer* pLayer = layout->layers.GetNext(layerPos);

			f(pLayer);
		}
	}
}

CObjType* GetTypeFromName(CApplication* pApp, CString objName)
{
	long unused;
	objName.MakeLower();

	// Iterate object types
	POSITION typePos = pApp->object_types.GetStartPosition();

	while (typePos) {
		CObjType* pType;
		pApp->object_types.GetNextAssoc(typePos, unused, pType);

		CString curName = pType->GetName();
		curName.MakeLower();

		if (objName == curName)
			return pType;
	}

	return NULL;
}

class FamilyNameEraser : public ExpTokenFunctor {
public:
	CString familyName;

	FamilyNameEraser(CString _fname) : familyName(_fname) { familyName.MakeLower(); }

	bool operator()(Token& t) {
		// Token is a family name
		if (t.t == T_IDENTIFIER && t.tsub == TT_FAMILYNAME) {
			CString curName = t.str;
			curName.MakeLower();

			// Should erase!
			if (curName == familyName)
				return true;
		}

		return false;
	}
};

class FamilyObjectParamEraser : public ExpParamFunctor {
public:
	CString familyName;
	Family* pFamily;

	FamilyObjectParamEraser(CApplication* pApp, CString _fname) : familyName(_fname)
	{
		pFamily = &*find(pApp->families.begin(), pApp->families.end(), familyName);	
	}

	bool operator()(CEditorParam* param) {

		// Is object param
		if (param->m_type == EDITORPARAM_OBJECT) {
			if (pFamily->identifier == atoi(param->ttokens.front().str))
				return true;	// delete me
		}

		return false;
	}


};

void EraseFamilyNameReferences(CApplication* pApp, CString familyName)
{
	FamilyNameEraser eraser(familyName);
	ForEachExpressionToken(pApp, eraser);

	FamilyObjectParamEraser eraser2(pApp, familyName);
	ForEachParameter(pApp, eraser2);
}

class PrivateVariableEraser : public ExpTokenFunctor {
public:
	long oid;
	int varid;

	PrivateVariableEraser(long _oid, int _varid) : oid(_oid), varid(_varid) {}

	bool operator()(Token& t) {
	
		// Is a variable name of this object
		if (t.t == T_VARIABLENAME && t.oidOwner == oid && t.id == varid)
			return true;

		return false;
	}
};

class PrivateVariableParameterEraser : public ExpParamFunctor {
public:
	long oid;
	int varid;

	PrivateVariableParameterEraser(long _oid, int _varid) : oid(_oid), varid(_varid) {}

	bool operator()(CEditorParam* param) {

		if (param->m_type == 14 && param->oid == oid && atoi(param->m_reserved) == varid)
			return true;

		return false;
	}
};

void ErasePrivateVariableReferences(CApplication* pApp, CObjType* pType, int varid)
{
	PrivateVariableEraser eraser(pType->ObjectIdentifier, varid);
	ForEachExpressionToken(pApp, eraser);

	//PrivateVariableParameterEraser parameraser(pType->ObjectIdentifier, varid);
	//ForEachParameter(pApp, parameraser);
}

class GlobalVariableEraser : public ExpTokenFunctor {
public:
	int varid;

	GlobalVariableEraser(int _varid) : varid(_varid) {}

	bool operator()(Token& t) {
		
		// Is a variable name of this object
		if (t.t == T_VARIABLENAME && t.oidOwner == -1 && t.id == varid)
			return true;

		return false;
	}
};

class GlobalVariableParameterEraser : public ExpParamFunctor {
public:
	int varid;

	GlobalVariableParameterEraser(int _varid) : varid(_varid) {}

	bool operator()(CEditorParam* param) {

		if (param->m_type == 18 && atoi(param->m_reserved) == varid)
			return true;

		return false;
	}
};

void EraseGlobalVariableReferences(CApplication* pApp, int varid)
{
	GlobalVariableEraser eraser(varid);
	ForEachExpressionToken(pApp, eraser);

	//GlobalVariableParameterEraser parameraser(varid);
	//ForEachParameter(pApp, parameraser);
}

/*
class PrivateVariableRenamer : public ExpTokenFunctor {
public:
	long oid;
	CString oldName;
	CString newName;

	PrivateVariableRenamer(long _oid, CString _oldName, CString _newName)
		: oid(_oid), oldName(_oldName), newName(_newName)
	{
		oldName.MakeLower();
	}

	bool operator()(Token& t) {
		CString curVar = t.str;
		curVar.MakeLower();
		
		// Is a variable name of this object
		if (t.t == T_VARIABLENAME && t.oidOwner == oid && curVar == oldName)
			t.str = newName;

		return false;
	}
};

class PrivateVariableParameterRenamer : public ExpParamFunctor {
public:
	long oid;
	CString oldName;
	CString newName;

	PrivateVariableParameterRenamer(long _oid, CString _oldName, CString _newName)
		: oid(_oid), oldName(_oldName), newName(_newName)
	{
		oldName.MakeLower();
	}

	bool operator()(CEditorParam* param) {
		CString curVar = param->m_reserved;
		curVar.MakeLower();
		
		// Is a variable name of this object
		if (param->m_type == 14 && curVar == oldName && param->oid == oid)
			param->m_reserved = newName;

		return false;
	}
};

void RenamePrivateVariableReferences(CApplication* pApp, CObjType* pType, CString oldName, CString newName)
{
	PrivateVariableRenamer renamer(pType->ObjectIdentifier, oldName, newName);
	ForEachExpressionToken(pApp, renamer);

	PrivateVariableParameterRenamer paramrenamer(pType->ObjectIdentifier, oldName, newName);
	ForEachParameter(pApp, paramrenamer);
}
*/

void ForEachExpressionTokenInEventList(ExpTokenFunctor& f, EventVector& eventList)
{
	// For each event
	EventVector::iterator e = eventList.begin();
	EventVector::iterator eventList_end = eventList.end();

	for ( ; e != eventList_end; e++) {

		// For each condition
		ConditionVector::iterator c = (*e)->m_Conditions.begin();

		for ( ; c != (*e)->m_Conditions.end(); ) {

			bool eraseCondition = false;
			
			// For each parameter
			ParamVector::iterator p = (*c)->params.begin();
			ParamVector::iterator params_end = (*c)->params.end();

			for ( ; p != params_end; p++) {

				// For each token
				vector<Token>::iterator t = (*p)->ttokens.begin();
				vector<Token>::iterator tokens_end = (*p)->ttokens.end();

				for (  ; t != tokens_end; t++) {
					
					// Call functor
					eraseCondition = f(*t);

					if (eraseCondition) break;
				}

				if (eraseCondition) break;
			}

			// Erase the condition if it included a family expression
			if (eraseCondition) {
				delete *c;
				c = (*e)->m_Conditions.erase(c);
			}
			else
				c++;

		}//for each condition

		// For each action
		ActionVector::iterator a = (*e)->m_Actions.begin();

		for ( ; a != (*e)->m_Actions.end(); ) {

			bool eraseAction = false;
			
			// For each parameter
			ParamVector::iterator p = (*a)->params.begin();
			ParamVector::iterator params_end = (*a)->params.end();

			for ( ; p != params_end; p++) {

				// For each token
				vector<Token>::iterator t = (*p)->ttokens.begin();
				vector<Token>::iterator tokens_end = (*p)->ttokens.end();

				for (  ; t != tokens_end; t++) {

					// Call functor
					eraseAction = f(*t);

					if (eraseAction) break;
				}

				if (eraseAction) break;
			}

			// Erase the condition if it included a family expression
			if (eraseAction) {
				delete *a;
				a = (*e)->m_Actions.erase(a);
			}
			else
				a++;

		}//for each action

		// Now do subevents
		ForEachExpressionTokenInEventList(f, (*e)->m_EventList);
	}
}

void ForEachExpressionToken(CApplication* pApp, ExpTokenFunctor& f)
{
	// For each event sheet
	list<EventSheet*>::iterator i = pApp->event_sheets.begin();
	list<EventSheet*>::iterator eventSheets_end = pApp->event_sheets.end();

	for ( ; i != eventSheets_end; i++) {

		// For each event
		ForEachExpressionTokenInEventList(f, (*i)->m_EventList);
		
	}
}

void ForEachParameterInEventList(ExpParamFunctor& f, EventVector& eventList)
{
	// For each event
	EventVector::iterator e = eventList.begin();
	EventVector::iterator eventList_end = eventList.end();

	for ( ; e != eventList_end; e++) {

		// For each condition
		ConditionVector::iterator c = (*e)->m_Conditions.begin();

		for ( ; c != (*e)->m_Conditions.end(); ) {

			bool eraseCondition = false;
			
			// For each parameter
			ParamVector::iterator p = (*c)->params.begin();
			ParamVector::iterator params_end = (*c)->params.end();

			for ( ; p != params_end; p++) {

				// Call functor
				eraseCondition = f(*p);

				if (eraseCondition) break;

			}

			// Erase the condition if it included a family expression
			if (eraseCondition) {
				delete *c;
				c = (*e)->m_Conditions.erase(c);
			}
			else
				c++;

		}//for each condition

		// For each action
		ActionVector::iterator a = (*e)->m_Actions.begin();

		for ( ; a != (*e)->m_Actions.end(); ) {

			bool eraseAction = false;
			
			// For each parameter
			ParamVector::iterator p = (*a)->params.begin();
			ParamVector::iterator params_end = (*a)->params.end();

			for ( ; p != params_end; p++) {

				// Call functor
				eraseAction = f(*p);

				if (eraseAction) break;
			}

			// Erase the condition if it included a family expression
			if (eraseAction) {
				delete *a;
				a = (*e)->m_Actions.erase(a);
			}
			else
				a++;

		}//for each action

		// Now do subevents
		ForEachParameterInEventList(f, (*e)->m_EventList);
	}
}

void ForEachParameter(CApplication* pApp, ExpParamFunctor& f)
{
	// For each event sheet
	list<EventSheet*>::iterator i = pApp->event_sheets.begin();
	list<EventSheet*>::iterator eventSheets_end = pApp->event_sheets.end();

	for ( ; i != eventSheets_end; i++) {

		// For each event
		ForEachParameterInEventList(f, (*i)->m_EventList);
		
	}
}