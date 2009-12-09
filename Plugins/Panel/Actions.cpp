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

long ExtObject::aCopyImageFromObj(LPVAL theParams)
{
	CRunObject* pObj = theParams[0].GetPairedObjectParam(pRuntime, this);
	if(pObj->info.curTexture)
	{
		iTexture = pObj->info.curTexture;
	}
	return 0;
}

long ExtObject::aSetVertex(LPVAL params)
{
	int indx = params[0].GetInt();
	COLORREF col = params[1].GetInt();
	cr_float opacity = params[2].GetFloat();

	Clamp(indx, 0, 3);
	Clamp(opacity, 0.0f, 100.0f);

	cr::color c(col);
	c.a = opacity / 100.0f;

	vertex_filter[indx] = c;

	return 0;
}

long ExtObject::aSetMargin(LPVAL params)
{
	image_left_margin = params[0].GetInt();
	image_top_margin = params[1].GetInt();
	image_right_margin = params[2].GetInt();
	image_bottom_margin = params[3].GetInt();
	
	return 0;
}