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
	return ret = sqrt(dx * dx + dy * dy);//speed;
}

long ExtObject::eMaxXFloorSpeed(LPVAL theParams, ExpReturn& ret)
{
	return ret = max_floor_speed;
}

long ExtObject::eMaxXAirSpeed(LPVAL theParams, ExpReturn& ret)
{
	return ret = max_air_speed;
}

long ExtObject::eMaxYSpeed(LPVAL theParams, ExpReturn& ret)
{
	return ret = max_fall;
}

long ExtObject::eAccFloorRate(LPVAL theParams, ExpReturn& ret)
{
	return ret = floor_acc;
}

long ExtObject::eAccAirRate(LPVAL theParams, ExpReturn& ret)
{
	return ret = air_acc;
}

long ExtObject::eDecFloorRate(LPVAL theParams, ExpReturn& ret)
{
	return ret = floor_dec;
}

long ExtObject::eDecAirRate(LPVAL theParams, ExpReturn& ret)
{
	return ret = air_dec;
}

long ExtObject::eMovAngle(LPVAL theParams, ExpReturn& ret)
{
	return ret = DEGREES(atan2(dy, dx));
}

long ExtObject::eVectorX(LPVAL theParams, ExpReturn& ret)
{
	return ret = dx;
}

long ExtObject::eVectorY(LPVAL theParams, ExpReturn& ret)
{
	return ret = dy;
}

long ExtObject::eGrav(LPVAL theParams, ExpReturn& ret)
{
	return ret = grav_fall;
}

long ExtObject::eJumpGrav(LPVAL theParams, ExpReturn& ret)
{
	return ret = grav_jump;
}

long ExtObject::eJumpSustainGrav(LPVAL theParams, ExpReturn& ret)
{
	return ret = grav_jump_sustain;
}
long ExtObject::eGravAngle(LPVAL theParams, ExpReturn& ret)
{
	return ret = grav_dir * 90;
}

long ExtObject::eJumpStrength(LPVAL theParams, ExpReturn& ret)
{
	return ret = jump_strength;
}

long ExtObject::eJumpSustain(LPVAL theParams, ExpReturn& ret)
{
	return ret = jump_sustain;
}