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
	return ret = 0;
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

long ExtObject::eSpeed(LPVAL theParams, ExpReturn& ret)
{
	return ret = speedRet;
}

long ExtObject::eMaxSpeed(LPVAL theParams, ExpReturn& ret)
{
	return ret = params.maxspeed;
}

long ExtObject::eAccRate(LPVAL theParams, ExpReturn& ret)
{
	return ret = params.acc;
}

long ExtObject::eDecRate(LPVAL theParams, ExpReturn& ret)
{
	return ret = params.dec;
}

long ExtObject::eMovAngle(LPVAL theParams, ExpReturn& ret)
{
	return ret = DEGREES(mAngle);
}

long ExtObject::eVectorX(LPVAL theParams, ExpReturn& ret)
{
	return ret = (cos(mAngle) * speed);
}

long ExtObject::eVectorY(LPVAL theParams, ExpReturn& ret)
{
	return ret = (sin(mAngle) * speed);
}

long ExtObject::eTargetX(LPVAL theParams, ExpReturn& ret)
{
	return ret = targetX;
}

long ExtObject::eTargetY(LPVAL theParams, ExpReturn& ret)
{
	return ret = targetY;
}