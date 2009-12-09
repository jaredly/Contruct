// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::cValueCmp(LPVAL params)
{
	const ExpStore& l = privateVars[params[0].GetVariableIndex(pRuntime, pType)];
	const ExpReturn& r = params[2];

	return DoComparison(params[1].GetInt(), (const ExpBase&)l, (const ExpBase&)r);
}

long ExtObject::cCompareStartX(LPVAL params)
{
	return DoComparison(params[0].GetInt(), start.x, params[1].GetFloat());
}

long ExtObject::cCompareStartY(LPVAL params)
{
	return DoComparison(params[0].GetInt(), start.y, params[1].GetFloat());
}

long ExtObject::cCompareStartZ(LPVAL params)
{
	return DoComparison(params[0].GetInt(), start.z, params[1].GetFloat());
}

long ExtObject::cCompareEndX(LPVAL params)
{
	return DoComparison(params[0].GetInt(), end.x, params[1].GetFloat());
}

long ExtObject::cCompareEndY(LPVAL params)
{
	return DoComparison(params[0].GetInt(), end.y, params[1].GetFloat());
}

long ExtObject::cCompareEndZ(LPVAL params)
{
	return DoComparison(params[0].GetInt(), end.z, params[1].GetFloat());
}

long ExtObject::cCompareLineWidth(LPVAL params)
{
	return DoComparison(params[0].GetInt(), line_width, params[1].GetFloat());
}
