// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Built-in expressions
//////////////////////////////////////////////////////////////////////////////////

// Return the Default Value.  This is the value to be returned if your object name
// is used without an expression name, eg. in "Counter + 1".
// Parameters can be passed, eg. MyObject(3,7).  You can check ExpReturn::Type() or GetNumParams() to check for overloads.
long ExtObject::ReturnDefaultValue(LPVAL theParams, ExpReturn& ret)
{
	// ACCESS PRIVATE VALUE
	if (theParams[0].Type() == EXPTYPE_VARIABLENAME) {
		return ret.ReturnCustom(pRuntime, privateVars[theParams[0].GetVariableIndex(pRuntime, pType)]);
	}
	else {
		return ret = 0;
	}
}

// Called for undefined expression names, if your extension allows it.
// Eg: MyObject.UndefinedExpression(3,4,5) calls this routine with
// "UndefinedExpression" as the expression name.  OF_UNDEFINEDEXPRESSIONS must be specified in Main.h.
long ExtObject::ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Expressions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::eGetValue(LPVAL params, ExpReturn& ret)
{
	return ret.ReturnCustom(pRuntime, privateVars[params[0].GetVariableIndex(pRuntime, pType)]);
}

long ExtObject::eNumPVs(LPVAL params, ExpReturn& ret)
{
	return ret = privateVars.size();
}

long ExtObject::eGetStartX(LPVAL params, ExpReturn& ret)
{
	return ret = start.x;
}

long ExtObject::eGetStartY(LPVAL params, ExpReturn& ret)
{
	return ret = start.y;
}

long ExtObject::eGetStartZ(LPVAL params, ExpReturn& ret)
{
	return ret = start.z;
}

long ExtObject::eGetEndX(LPVAL params, ExpReturn& ret)
{
	return ret = end.x;
}

long ExtObject::eGetEndY(LPVAL params, ExpReturn& ret)
{
	return ret = end.y;
}

long ExtObject::eGetEndZ(LPVAL params, ExpReturn& ret)
{
	return ret = end.z;
}

long ExtObject::eGetLineWidth(LPVAL params, ExpReturn& ret)
{
	return ret = line_width;
}
