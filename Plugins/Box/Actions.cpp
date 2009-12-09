// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aSetValue(LPVAL theParams)
{
	privateVars[theParams[0].GetVariableIndex(pRuntime, pType)] = theParams[1];
	return 0;
}

long ExtObject::aSetOpacity(LPVAL theParams)
{
	info.pInfo->filter.a = theParams[0].GetFloat() / 100.0;
	Clamp(info.pInfo->filter.a, 0.0f, 1.0f);

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

long ExtObject::aAttachToObj(LPVAL theParams)
{
	objAttach = theParams[0].GetPairedObjectParam(pRuntime, this);
	padding = theParams[1].GetInt();
	pRuntime->CallOnFrame2(this);
	return 0;
}

long ExtObject::aSetColor1(LPVAL theParams)
{
	c1 = theParams[0].GetInt();
	UpdateMyColors();
	return 0;
}

long ExtObject::aSetColor2(LPVAL theParams)
{
	c2 = theParams[0].GetInt();
	UpdateMyColors();
	return 0;
}

long ExtObject::aSetFill(LPVAL theParams)
{
	fill = theParams[0].GetInt();
	UpdateMyColors();
	return 0;
}

long ExtObject::aSetTransparent(LPVAL theParams)
{
	transparent = theParams[0].GetBool();
	return 0;
}
