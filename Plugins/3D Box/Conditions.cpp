// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::cCompareZ(LPVAL params)
{
	return DoComparison(params[0].GetInt(), z, params[1].GetFloat());
}

long ExtObject::cCompareDepth(LPVAL params)
{
	return DoComparison(params[0].GetInt(), depth, params[1].GetFloat());
}

long ExtObject::cValueCmp(LPVAL params)
{
	const ExpStore& l = privateVars[params[0].GetVariableIndex(pRuntime, pType)];
	const ExpReturn& r = params[2];

	return DoComparison(params[1].GetInt(), (const ExpBase&)l, (const ExpBase&)r);
}