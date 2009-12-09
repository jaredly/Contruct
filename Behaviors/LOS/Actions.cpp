// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////


long ExtObject::aAddObstacle(LPVAL theParams)
{
	pType = theParams[0].GetObjectParam(pRuntime);

	if (pType)
		obstacles.push_back(pType);

	return 0;
}

long ExtObject::aSetRange(LPVAL theParams)
{
	range = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetAngleRange(LPVAL theParams)
{
	angleRange = theParams[0].GetFloat();
	return 0;
}