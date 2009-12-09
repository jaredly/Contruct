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

long ExtObject::eGetValue(LPVAL params, ExpReturn& ret)
{
	return ret.ReturnCustom(pRuntime, privateVars[params[0].GetVariableIndex(pRuntime, pType)]);
}

long ExtObject::eOriginalWidth(LPVAL params, ExpReturn& ret)
{
	return ret = info.curTexture->image_width;
}

long ExtObject::eOriginalHeight(LPVAL params, ExpReturn& ret)
{
	return ret = info.curTexture->image_height;
}

long ExtObject::eGetImagePointX(LPVAL params, ExpReturn& ret)
{
	POINTF pt;

	// Check for action points
	if (params[0].Type() == EXPTYPE_STRING) {

		if (pRuntime->GetImagePoint(this, *(params[0].GetStringPtr()), pt))
			return ret = pt.x;
	}
	else if (params[0].Type() == EXPTYPE_INTEGER) {

		if (pRuntime->GetImagePoint(this, params[0].GetInt() - 1, pt))
			return ret = pt.x;
	}

	return ret = 0.0f;
}

long ExtObject::eGetImagePointY(LPVAL params, ExpReturn& ret)
{
	POINTF pt;

	// Check for action points
	if (params[0].Type() == EXPTYPE_STRING) {

		if (pRuntime->GetImagePoint(this, *(params[0].GetStringPtr()), pt))
			return ret = pt.y;
	}
	else if (params[0].Type() == EXPTYPE_INTEGER) {

		if (pRuntime->GetImagePoint(this, params[0].GetInt() - 1, pt))
			return ret = pt.y;
	}

	return ret = 0.0f;
}

long ExtObject::eNumPVs(LPVAL params, ExpReturn& ret)
{
	return ret = privateVars.size();
}

long ExtObject::eSkewX(LPVAL params, ExpReturn& ret)
{
	return ret = info.pInfo->skewX;
}

long ExtObject::eSkewY(LPVAL params, ExpReturn& ret)
{
	return ret = info.pInfo->skewY;
}

long ExtObject::eGetAnimFrame(LPVAL params, ExpReturn& ret)
{
	return ret = m_Animation.curFrame + 1;
}

long ExtObject::eGetAnimName(LPVAL params, ExpReturn& ret)
{
	if (m_Animation.pAnim == NULL)
		return false;

	if(m_Animation.pAnim->is_angle && m_Animation.pAnim->parent)
		return ret.ReturnString(pRuntime, m_Animation.pAnim->parent->name);
	else 
		return ret.ReturnString(pRuntime, m_Animation.pAnim->name);
}

long ExtObject::eGetFrameCount(LPVAL params, ExpReturn& ret)
{
	if (m_Animation.pAnim)
		return ret = pRuntime->GetAnimationFrameCount(m_Animation.pAnim);
	else
		return ret = 0;
}

long ExtObject::eGetAnimSpeed(LPVAL params, ExpReturn& ret)
{
	return m_Animation.speed;
}

long ExtObject::eGetMeshCols(LPVAL params, ExpReturn& ret)
{
	return ret = distort.size() - 1;
}

long ExtObject::eGetMeshRows(LPVAL params, ExpReturn& ret)
{
	if (distort.empty())
		return ret = 0;

	return ret = distort.front().size() - 1;
}

long ExtObject::eGetMeshX(LPVAL params, ExpReturn& ret)
{
	UINT col = params[0].GetInt();
	UINT row = params[1].GetInt();

	if (distort.empty() || col < 0 || row < 0 || col >= distort.size() || row >= distort.front().size())
		return ret = 0;

	return ret = distort[col][row].x;
}

long ExtObject::eGetMeshY(LPVAL params, ExpReturn& ret)
{
	UINT col = params[0].GetInt();
	UINT row = params[1].GetInt();

	if (distort.empty() || col < 0 || row < 0 || col >= distort.size() || row >= distort.front().size())
		return ret = 0;

	return ret = distort[col][row].y;
}

long ExtObject::eGetMeshZ(LPVAL params, ExpReturn& ret)
{
	UINT col = params[0].GetInt();
	UINT row = params[1].GetInt();

	if (distort.empty() || col < 0 || row < 0 || col >= distort.size() || row >= distort.front().size())
		return ret = 0;

	return ret = distort[col][row].z;
}

long ExtObject::eGetMeshU(LPVAL params, ExpReturn& ret)
{
	UINT col = params[0].GetInt();
	UINT row = params[1].GetInt();

	if (distort.empty() || col < 0 || row < 0 || col >= distort.size() || row >= distort.front().size())
		return ret = 0;

	return ret = distort[col][row].u * info.w;
}

long ExtObject::eGetMeshV(LPVAL params, ExpReturn& ret)
{
	UINT col = params[0].GetInt();
	UINT row = params[1].GetInt();

	if (distort.empty() || col < 0 || row < 0 || col >= distort.size() || row >= distort.front().size())
		return ret = 0;

	return ret = distort[col][row].v * info.h;
}

long ExtObject::eGetMeshFilter(LPVAL params, ExpReturn& ret)
{
	UINT col = params[0].GetInt();
	UINT row = params[1].GetInt();

	if (distort.empty() || col < 0 || row < 0 || col >= distort.size() || row >= distort.front().size())
		return ret = 0;

	return ret = (UINT)distort[col][row].filter.getCOLORREF();
}

long ExtObject::eGetMeshOpacity(LPVAL params, ExpReturn& ret)
{
	UINT col = params[0].GetInt();
	UINT row = params[1].GetInt();

	if (distort.empty() || col < 0 || row < 0 || col >= distort.size() || row >= distort.front().size())
		return ret = 0;

	return ret = distort[col][row].filter.a * 100.0;
}
