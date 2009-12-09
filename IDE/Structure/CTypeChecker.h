#pragma once

#include "Structure.h"
#include "..\Editors\Expressions\CExpression.h"
#include "..\Editors\Expressions\ExpTree.h"

class CUndefinedVariable
{
public:
	bool global;
	CObjType* pType;
	CString Name;

	CUndefinedVariable(bool _global, CObjType* _pType,CString _Name)
	{
		global = _global;
		pType = _pType;
		Name = _Name;
	}
	bool operator==(const CUndefinedVariable& other) {
		CString l = Name;
		CString r = other.Name;
		l.MakeLower();
		r.MakeLower();
		return (l == r && pType == other.pType && global == other.global);
	}

};

// Type checker deriving from CExpression
class CTypeChecker : public CExpression {
public:
	CObjTypeMap* objMap;
	CApplication* pApp;
	list<CUndefinedVariable> m_UndefinedVariables;

	// Return 'true' if 'objName' is a valid name of an object
	// Dont be case sensitive!
	virtual bool ObjectNameExists(CString objName);

	virtual bool ObjectBehaviorExists(CString objName, CString movementName);

	// Return 'true' if 'objName' can be used as a default value, eg. without a dot following it.
	// Return 'false' if objName is not 
	// Eg. Edit as opposed to Edit.Text.  We need to make an SDK option to enable the default value first.
	virtual bool ObjectHasDefaultValue(CString objName)
	{
		// Temporary solution: allow any object to use a default value
		// TODO: ACEs for default values
		return ObjectNameExists(objName);
	}

	// Return 'true' if 'expName' exists under 'objName', eg. Object.X
	virtual bool ObjectExpressionExists(CString objName, CString expName);

	// Get the type of a plugin expression.
	// expName is "<default>" if its asking for the default overload, eg. Sprite(a,b,c)
	virtual ExpType GetExpressionType(CString objName, CString expName);

	// Return 'true' if objName.expName accepts numParams parameters.
	// expName is "<default>" if its asking for the default overload, eg. Sprite(a,b,c)
	virtual bool CheckParamCount(CString objName, CString expName, int numParams);

	// Parameters valid: return 'true' if the given parameter is acceptable for the given expression
	// This is only called if num params > 0
	virtual bool ParamValid(CString objName, CString expName, int paramNum, int numParams, ExpType type);

	virtual bool PrivateVariableExists(CString objName, CString expName, int paramNum, int numParams, CString varName);
};
