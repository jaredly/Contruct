// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aBounce		(LPVAL theParams)
{
	// Invert speed - basic effect but works
	dx *= -1;
	dy *= -1;

	return 0;
}

long ExtObject::aStop			(LPVAL theParams)
{
	// Reset speed
	dx = 0;
	dy = 0;
	return 0;
}

long ExtObject::aSetMaxXFloorSpeed	(LPVAL theParams)
{
	max_floor_speed = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetMaxXAirSpeed	(LPVAL theParams)
{
	max_air_speed = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetMaxYSpeed	(LPVAL theParams)
{
	max_fall = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetFloorAcc		(LPVAL theParams)
{
	floor_acc = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetFloorDec		(LPVAL theParams)
{
	floor_dec = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetAirAcc		(LPVAL theParams)
{
	air_acc = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetAirDec		(LPVAL theParams)
{
	air_dec = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aJump(LPVAL theParams)
{
	float ga = RADIANS(grav_dir * 90);

	int cosga = cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;

	if(singa)
		dy = -jump_strength * singa;
	if(cosga)
		dx = -jump_strength * cosga;

	return 0;
}

long ExtObject::aSetInvertControl(LPVAL theParams)
{
	inv = theParams[0].GetBool();
	return 0;
}

long ExtObject::aSetIgnoreInput(LPVAL theParams)
{
	ignoringInput = theParams[0].GetBool();
	return 0;
}

long ExtObject::aSetActivated(LPVAL theParams)
{
	activated = theParams[0].GetBool();

	if (activated)
		pRuntime->CallOnFrame(this);

	return 0;
}

long ExtObject::aSetJumpStrength(LPVAL theParams)
{
	jump_strength = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetJumpSustain(LPVAL theParams)
{
	jump_sustain = theParams[0].GetInt();
	return 0;
}

long ExtObject::aSetGravity(LPVAL theParams)
{
	grav_fall = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetJumpGravity(LPVAL theParams)
{
	grav_jump = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetJumpSustainGravity(LPVAL theParams)
{
	grav_jump_sustain = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetGravDir(LPVAL theParams)
{
	grav_dir = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetXSpeed(LPVAL theParams)
{
	dx = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetYSpeed(LPVAL theParams)
{
	dy = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetFallThroughPlatforms(LPVAL theParams)
{
	action_gothroughplatform = theParams[0].GetBool();
	return 0;
}
