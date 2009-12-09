// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Built-in expressions
//////////////////////////////////////////////////////////////////////////////////

// Return the Default Value.  This is the value to be returned if your object name
// is used without an expression name, eg. in "Counter + 1".
// Parameters can be passed, eg. MyObject(3,7).  You can check the Type() or GetNumParams() to check for overloads.
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

long ExtObject::eGetZ(LPVAL params, ExpReturn& ret)
{
	return ret = z;
}

long ExtObject::eGetDepth(LPVAL params, ExpReturn& ret)
{
	return ret = depth;
}

long ExtObject::eGetYaw(LPVAL params, ExpReturn& ret)
{
	return ret = DEGREES(yaw);
}

long ExtObject::eGetPitch(LPVAL params, ExpReturn& ret)
{
	return ret = DEGREES(pitch);
}

long ExtObject::eGetRoll(LPVAL params, ExpReturn& ret)
{
	return ret = info.angle;
}

long ExtObject::eGetValue(LPVAL params, ExpReturn& ret)
{
	return ret.ReturnCustom(pRuntime, privateVars[params[0].GetVariableIndex(pRuntime, pType)]);
}