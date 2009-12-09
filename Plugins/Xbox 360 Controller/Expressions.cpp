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

long ExtObject::eLeftThumbX(LPVAL params, ExpReturn& ret)
{
	CXBOXController* player = GetPlayer(params);
	if(!player) return 0; // error

	return ret = player->LeftThumbX();
}

long ExtObject::eLeftThumbY(LPVAL params, ExpReturn& ret)
{
	CXBOXController* player = GetPlayer(params);
	if(!player) return 0; // error

	return ret = player->LeftThumbY();
}

long ExtObject::eRightThumbX(LPVAL params, ExpReturn& ret)
{
	CXBOXController* player = GetPlayer(params);
	if(!player) return 0; // error

	return ret = player->RightThumbX();
}

long ExtObject::eRightThumbY(LPVAL params, ExpReturn& ret)
{
	CXBOXController* player = GetPlayer(params);
	if(!player) return 0; // error

	return ret = player->RightThumbY();
}

	

long ExtObject::eLeftTrigger(LPVAL params, ExpReturn& ret)
{
	CXBOXController* player = GetPlayer(params);
	if(!player) return 0; // error

	return ret = player->LeftTrigger();
}

long ExtObject::eRightTrigger(LPVAL params, ExpReturn& ret)
{
	CXBOXController* player = GetPlayer(params);
	if(!player) return 0; // error

	return ret = player->RightTrigger();
}

long ExtObject::eButtonState(LPVAL params, ExpReturn& ret)
{
	CXBOXController* player = GetPlayer(params);
	if(!player) return 0; // error

	return ret = player->ButtonState((XBoxControl::Control)params[1].GetInt());
}