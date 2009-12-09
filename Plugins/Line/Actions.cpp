// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

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

long ExtObject::aSetStartX(LPVAL params)
{
	start.x = params[0].GetFloat();
	UpdateInfoBox();
	return 0;
}

long ExtObject::aSetStartY(LPVAL params)
{
	start.y = params[0].GetFloat();
	UpdateInfoBox();
	return 0;
}

long ExtObject::aSetStartZ(LPVAL params)
{
	start.z = params[0].GetFloat();
	UpdateInfoBox();
	return 0;
}

long ExtObject::aSetStartPos(LPVAL params)
{
	start.x = params[0].GetFloat();
	start.y = params[1].GetFloat();
	start.z = params[2].GetFloat();
	UpdateInfoBox();
	return 0;
}

long ExtObject::aSetEndX(LPVAL params)
{
	end.x = params[0].GetFloat();
	UpdateInfoBox();
	return 0;
}

long ExtObject::aSetEndY(LPVAL params)
{
	end.y = params[0].GetFloat();
	UpdateInfoBox();
	return 0;
}

long ExtObject::aSetEndZ(LPVAL params)
{
	end.z = params[0].GetFloat();
	UpdateInfoBox();
	return 0;
}

long ExtObject::aSetEndPos(LPVAL params)
{
	end.x = params[0].GetFloat();
	end.y = params[1].GetFloat();
	end.z = params[2].GetFloat();
	UpdateInfoBox();
	return 0;
}

long ExtObject::aSetLineWidth(LPVAL params)
{
	line_width = params[0].GetFloat();
	UpdateInfoBox();
	return 0;
}
