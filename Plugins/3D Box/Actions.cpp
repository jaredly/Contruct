// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aSetZ(LPVAL params)
{
	z = params[0].GetFloat();

	return 0;
}

long ExtObject::aSetDepth(LPVAL params)
{
	depth = params[0].GetFloat();

	return 0;
}

long ExtObject::aSetYaw(LPVAL params)
{
	yaw = RADIANS(params[0].GetDouble());
	UpdateAngle();
	return 0;
}

long ExtObject::aSetPitch(LPVAL params)
{
	pitch = RADIANS(params[0].GetDouble());
	UpdateAngle();
	return 0;
}

long ExtObject::aSetRoll(LPVAL params)
{
	info.angle = params[0].GetDouble();
	UpdateAngle();
	pRuntime->UpdateBoundingBox(this);
	return 0;
}

long ExtObject::aSetValue(LPVAL params)
{
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] = params[1];

	return 0;
}

long ExtObject::aAddValue(LPVAL params)
{
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] += params[1];

	return 0;
}

long ExtObject::aSubValue(LPVAL params)
{
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] -= params[1];

	return 0;
}