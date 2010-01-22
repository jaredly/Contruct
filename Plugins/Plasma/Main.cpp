// Main.cpp - Main implementation of object
//

// Include StdAfx
#include "StdAfx.h"

// If run only
#ifdef RUN_ONLY

#include "..\..\Common\CommonAceDef.hpp"

long ExtObject::aSetPlasmaXSpeed(LPVAL theParams)
{
	xspeed = theParams[0].GetFloat();
	return 0;
}
long ExtObject::aSetPlasmaYSpeed(LPVAL theParams)
{
	yspeed = theParams[0].GetFloat();
	return 0;
}
long ExtObject::aSetPlasmaSpeed(LPVAL theParams)
{
	return 0;
}
long ExtObject::aSetPlasmaAngle(LPVAL theParams)
{
	return 0;
}
long ExtObject::aSetPatternXSpeed(LPVAL theParams)
{
	coldxspeed = theParams[0].GetFloat();
	return 0;
}
long ExtObject::aSetPatternYSpeed(LPVAL theParams)
{
	coldyspeed = theParams[0].GetFloat();
	return 0;
}
long ExtObject::aSetPatternSpeed(LPVAL theParams)
{
	return 0;
}
long ExtObject::aSetPatternMoveAngle(LPVAL theParams)
{
	return 0;
}
long ExtObject::aSetPatternScale(LPVAL theParams)
{
	coldscale = theParams[0].GetFloat();
	return 0;
}
long ExtObject::aSetPatternAngle(LPVAL theParams)
{
	coldangle = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aPasteObject(LPVAL params)
{
	
	CRunObjType* pType = params[0].GetObjectParam(pRuntime);
	int count;
	CRunObject** objs;
	pRuntime->GetTypeSelectedInstances(pType, objs, count);

	for(int i = 0; i < count; i++)
	{
		CRunObject* pObj = objs[i];

		OBJHEADER restore = pObj->info;
		pObj->info.x -= info.x - info.HotSpotX;
		pObj->info.y -= info.y - info.HotSpotY;
		pObj->UpdateBoundingBox();

		renderer->SetRenderTarget(info.curTexture);
		pRuntime->DrawObject(pObj);
		renderer->RestoreRenderTarget();

		pObj->info = restore;
		pObj->UpdateBoundingBox();
	}
	return 0;
}



#endif // #ifdef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// ACE tables
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
	ADDPARAM(PARAM_VALUE, "Set the x speed in pixels per second", "X speed");
	ADDACT("Set x speed", "Plasma", "Set plasma x speed to %0", &ExtObject::aSetPlasmaXSpeed, "SetPlasmaXSpeed", 0);

	ADDPARAM(PARAM_VALUE, "Set the y speed in pixels per second", "Y speed");
	ADDACT("Set y speed", "Plasma", "Set plasma y speed to %0", &ExtObject::aSetPlasmaYSpeed, "SetPlasmaYSpeed", 0);

	ADDPARAM(PARAM_VALUE, "Set the speed in pixels per second", "Speed");
	ADDACT("Set speed", "Plasma", "Set plasma speed to %0", &ExtObject::aSetPlasmaSpeed, "SetPlasmaSpeed", 0);

	ADDPARAM(PARAM_VALUE, "Set the angle of the plasma", "Angle");
	ADDACT("Set angle", "Plasma", "Set plasma angle to %0", &ExtObject::aSetPlasmaAngle, "SetPlasmaAngle", 0);

	ADDPARAM(PARAM_VALUE, "Set the x speed in pixels per second", "X Speed");
	ADDACT("Set pattern x speed", "Subtract Pattern", "Set subtract patter x speed to %0", &ExtObject::aSetPatternXSpeed, "SetPatternXSpeed", 0);

	ADDPARAM(PARAM_VALUE, "Set the y speed in pixels per second", "Y Speed");
	ADDACT("Set pattern y speed", "Subtract Pattern", "Set subtract patter y speed to %0", &ExtObject::aSetPatternYSpeed, "SetPatternYSpeed", 0);

	ADDPARAM(PARAM_VALUE, "Set the speed in pixels per second", "Speed");
	ADDACT("Set pattern speed", "Subtract Pattern", "Set subtract patter speed to %0", &ExtObject::aSetPatternSpeed, "SetPatternSpeed", 0);

	ADDPARAM(PARAM_VALUE, "Set the angle", "Angle");
	ADDACT("Set pattern move angle", "Subtract Pattern", "Set subtract pattern move angle to %0", &ExtObject::aSetPatternMoveAngle, "SetPatternMoveAngle", 0);

	ADDPARAM(PARAM_VALUE, "Set the scale", "Scale");
	ADDACT("Set pattern scale", "Subtract Pattern", "Set subtract pattern scale to %0", &ExtObject::aSetPatternScale, "SetPatternScale", 0);

	ADDPARAM(PARAM_VALUE, "Set the angle", "Angle");
	ADDACT("Set pattern angle", "Subtract Pattern", "Set subtract pattern angle to %0", &ExtObject::aSetPatternAngle, "SetPatternAngle", 0);

	ADDPARAM(PARAM_OBJECT, "Object", "Object to draw");
	ADDACT("Paste Object", "Paste", "Paste %0 into plasma", &ExtObject::aPasteObject, "PasteObject", 0);




#include "..\..\Common\CommonAceTable.hpp"

}


