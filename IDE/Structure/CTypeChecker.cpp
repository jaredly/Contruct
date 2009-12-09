#include "StdAfx.h"
#include "CTypeChecker.h"

bool CTypeChecker::ObjectNameExists(CString objName)
{
	CObjType* pType = LookupObjectType(objName, objMap);

	// Object name not found: try a family name
	if (pType == NULL)
	{
		list<Family>::iterator f = find(pApp->families.begin(), pApp->families.end(), objName);
		if (f != pApp->families.end()) {
			if (f->is_in_use)
				return true;
		}

		// Davo, don't forget this :P
		// objName was not found in pApp->families.
		return false;
	}
	// pType was NULL
	else
		return true;
}

bool CTypeChecker::ObjectExpressionExists(CString objName, CString expName)
{
	objName.MakeLower();
	expName.MakeLower();

	CString movementName;
	ParseBehaviorExpression(objName, objName, movementName);

	CObjType* oT = LookupObjectType(objName, objMap);
	int DLLIndex;

	// Couldn't find object type: try family
	if (oT == NULL) {
		list<Family>::iterator f = find(pApp->families.begin(), pApp->families.end(), objName);

		// Not found
		if (f == pApp->families.end() || !f->is_in_use)
			return false;
		else {
			DLLIndex = f->DLLIndex;
			oT = pApp->FindObjTypeFromNumber(f->identifier);
		}
	}
	else DLLIndex = oT->DLLIndex;

	// Behavior under object: objName[movementName]
	if (movementName != "") {
		CBehavior* m = LookupBehavior(oT, movementName);
		if (m == NULL) return false;

		OINFO* oInfo = GetOINFO(m->BehaviorDLLIndex);

		int Count = oInfo->GetTable(2)->size();

		for (int i = 0; i < Count; i++) {
			CString Exp = oInfo->GetACEEntry(2, i)->aceDisplayText;
			Exp.MakeLower();
			if (Exp == expName)
				return true;
		}

		return false;
	}

	OINFO* oInfo = GetOINFO(DLLIndex);

	if (oInfo == NULL)
		return false;

	// get count
	int Count = oInfo->GetTable(2)->size();

	for (int i = 0; i < Count; i++)
	{
		CString Exp = oInfo->GetACEEntry(2, i)->aceDisplayText;
		Exp.MakeLower();
		if (Exp == expName)
			return true;
	}

	// Expression name not found: allow if OF_UNDEFINEDEXPRESSIONS set
	return (oInfo->ideFlags & OF_UNDEFINEDEXPRESSIONS)!=0;
}

bool CTypeChecker::ObjectBehaviorExists(CString objName, CString movementName)
{
	CObjType* oT = LookupObjectType(objName, objMap);

	// Couldn't find object type: try family
	if (oT == NULL) {
		list<Family>::iterator f = find(pApp->families.begin(), pApp->families.end(), objName);

		// Not found
		if (f == pApp->families.end() || !f->is_in_use)
			return false;
		else 
			oT = pApp->FindObjTypeFromNumber(f->identifier);
	}

	return exists_element(oT->behaviors.begin(), oT->behaviors.end(), movementName);
}

ExpType CTypeChecker::GetExpressionType(CString objName, CString expName)
{
	objName.MakeLower();
	expName.MakeLower();

	// No ACE system for default params yet.  Return any.
	if (expName == "<default>")
		return EXPTYPE_NULL;

	CString movementName;
	ParseBehaviorExpression(objName, objName, movementName);

	CObjType* oT = LookupObjectType(objName, objMap);
	int DLLIndex;

	// Couldn't find object type: try family
	if (oT == NULL) {
		list<Family>::iterator f = find(pApp->families.begin(), pApp->families.end(), objName);

		// Not found
		if (f == pApp->families.end() || !f->is_in_use)
			return EXPTYPE_NULL;
		else
			DLLIndex = f->DLLIndex;

	}
	else DLLIndex = oT->DLLIndex;

	vector<ACESEntry2>* pTable;

	if (movementName != "") {
		CBehavior* m = LookupBehavior(oT, movementName);
		if (m == NULL) return EXPTYPE_NULL;

		OINFO* oInfo = GetOINFO(m->BehaviorDLLIndex);
		pTable = oInfo->GetTable(2);
	}
	else
		pTable = GetOINFO(DLLIndex)->GetTable(2);

	// Find expression and get type
	int Count = pTable->size();
	for (int i = 0; i < Count; i++)
	{
		CString Exp = pTable->at(i).aceDisplayText;
		Exp.MakeLower();
		if (Exp == expName)
		{
			switch (pTable->at(i).retrn)
			{
			case 0:
				return EXPTYPE_NULL;
			case RETURN_INTEGER:
				return EXPTYPE_INTEGER;
			case RETURN_FLOAT:
				return EXPTYPE_FLOAT;
			case RETURN_STRING:
				return EXPTYPE_STRING;
			case RETURN_ARRAY:
				return EXPTYPE_ARRAY;
			}
		}
	}

	// Return type is 'any' if not found
	return EXPTYPE_NULL;
}

bool CTypeChecker::CheckParamCount(CString objName, CString expName, int numParams)
{
	objName.MakeLower();
	expName.MakeLower();

	CString movementName;
	ParseBehaviorExpression(objName, objName, movementName);

	CObjType* oT = LookupObjectType(objName, objMap);
	int DLLIndex;

	// Couldn't find object type: try family
	if (oT == NULL) {
		list<Family>::iterator f = find(pApp->families.begin(), pApp->families.end(), objName);

		// Not found
		if (f == pApp->families.end() || !f->is_in_use)
			return false;
		else {
			DLLIndex = f->DLLIndex;
			oT = pApp->FindObjTypeFromNumber(f->identifier);
		}

	}
	else DLLIndex = oT->DLLIndex;

	// Default value, eg. ObjectName(5)
	if (expName == "<default>") {
		// The default value is private variable flag requires 1 parameter for default value.
		if (GetOINFO(DLLIndex)->ideFlags & OF_DEFAULTVALUE_PRIVATEVAR)
			return numParams == 1;
		// There is no other way of defining the default value parameters
		else
			return true;
	}

	vector<ACESEntry2>* pTable;

	if (movementName != "") {
		CBehavior* m = LookupBehavior(oT, movementName);
		if (m == NULL) return false;

		OINFO* oInfo = GetOINFO(m->BehaviorDLLIndex);
		pTable = oInfo->GetTable(2);
	}
	else
		pTable = GetOINFO(DLLIndex)->GetTable(2);

	// get count
	int Count = pTable->size();
	for (int i = 0; i < Count; i++)
	{
		CString Exp = pTable->at(i).aceDisplayText;
		Exp.MakeLower();
		if (Exp == expName)
		{
			return pTable->at(i).params.size() == numParams;
		}
	}

	// Not found - undefined expression - allow.
	return true;
}

bool CTypeChecker::PrivateVariableExists(CString objName, CString expName, int paramNum, int numParams, CString varName)
{
	CString actualvarName = varName;
	objName.MakeLower();
	expName.MakeLower();
	varName.MakeLower();

	if (objName == "<system>")
		if(exists_element(pApp->global_variables.begin(), pApp->global_variables.end(), varName))
			return true;
		else
		{
			// Variable is undefined, add it to our list of undefined variables
			push_back_unique(m_UndefinedVariables, CUndefinedVariable(true, 0, varName));
			return true;
		}

	CObjType* oT;

	POSITION pos = objMap->GetStartPosition();
	long mapKey;

	// Loop through and add the objects
	while (pos != NULL) 
	{
		objMap->GetNextAssoc(pos, mapKey, oT);

		CString Name = oT->GetName();
		Name.MakeLower();

		if (Name == objName)
		{
			if(exists_element(oT->m_PrivateValues.begin(), oT->m_PrivateValues.end(), varName))
				return true;
			else
			{
				// Variable undefined, add it to our list of undefined variables
				push_back_unique(m_UndefinedVariables, CUndefinedVariable(false, oT, actualvarName));
				return true;
			}
		}
	}

	// Check families
	list<Family>::iterator f = pApp->families.begin();

	for ( ; f != pApp->families.end(); f++) {
		if(f->is_in_use)
		{
			CString familyName = f->name;
			familyName.MakeLower();

			f->UpdateFamilyData();

			if (objName == familyName) {

				return f->VariableNameExists(varName);

			}
		}
	}

	return false;
}

bool CTypeChecker::ParamValid(CString objName, CString expName, int paramNum, int numParams, ExpType type)
{
	// Impossible to tell if this parameter is correct using an undefined type
	if (type == EXPTYPE_NULL)
		return true;

	objName.MakeLower();
	expName.MakeLower(); 

	CString movementName;
	ParseBehaviorExpression(objName, objName, movementName);

	CObjType* oT = LookupObjectType(objName, objMap);
	int DLLIndex;

	// Couldn't find object type: try family
	if (oT == NULL) {
		list<Family>::iterator f = find(pApp->families.begin(), pApp->families.end(), objName);

		// Not found
		if (f == pApp->families.end() || !f->is_in_use)
			return false;
		else
			DLLIndex = f->DLLIndex;

	}
	else DLLIndex = oT->DLLIndex;

	// Default value, eg. ObjectName(5)
	if (expName == "<default>") {

		// Default value is private variable flag requires one private variable parameter
		if (GetOINFO(DLLIndex)->ideFlags & OF_DEFAULTVALUE_PRIVATEVAR)
			return type == EXPTYPE_VARIABLENAME;
		// No other way of defining default value parameters
		else
			return true;
	}

	vector<ACESEntry2>* pTable;

	if (movementName != "") {
		CBehavior* m = LookupBehavior(oT, movementName);
		if (m == NULL) return false;

		OINFO* oInfo = GetOINFO(m->BehaviorDLLIndex);
		pTable = oInfo->GetTable(2);
	}
	else
		pTable = GetOINFO(DLLIndex)->GetTable(2);

	// Lookup the entry, and get its param type.
	int Count = pTable->size();
	for (int i = 0; i < Count; i++)
	{
		CString Exp = pTable->at(i).aceDisplayText;
		Exp.MakeLower();
		if (Exp == expName)
		{
			switch (pTable->at(i).params[paramNum].type)
			{
			case 1:		// Value
				return (type == EXPTYPE_INTEGER || type == EXPTYPE_FLOAT);
				
			case 2:		// String
				return (type == EXPTYPE_STRING);
			
			case 14:	// Private variable
				return (type == EXPTYPE_VARIABLENAME);

			}
		}
	}

	// Not found - undefined expression - allow
	return true;
}