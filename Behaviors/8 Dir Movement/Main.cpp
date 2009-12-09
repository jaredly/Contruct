// Main.cpp - Main implementation of object
//

// Include StdAfx
#include "StdAfx.h"

// If run only
#ifdef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::cIsMoving		(LPVAL theParams)
{
	return speed > 0;
}

long ExtObject::cOnBounce		(LPVAL theParams)
{
	return TRUE;
}

long ExtObject::cCompareSpeed	(LPVAL params)
{
	return DoComparison(params[0].GetInt(), speed, params[1].GetFloat());
}


//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aBounce		(LPVAL theParams)
{
	// Invert dx,dy
	dx *= -1.00f;
	dy *= -1.00f;

	return 0;
}

long ExtObject::aStop			(LPVAL theParams)
{
	// Reset dx,dy
	dx = 0.0f;
	dy = 0.0f;
	return 0;
}

long ExtObject::aSetSpeed		(LPVAL theParams)
{
	SetSpeed(theParams[0].GetFloat());
	return 0;
}

long ExtObject::aSetMaxSpeed	(LPVAL theParams)
{
	maxspeed = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetAcc		(LPVAL theParams)
{
	acc = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetDec		(LPVAL theParams)
{
	dec = theParams[0].GetFloat();
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

long ExtObject::aSetMAngle(LPVAL theParams)
{
	float mAngle = theParams[0].GetFloat();
	mAngle = RADIANS(mAngle);
	float curSpeed = GetSpeed();

	dx = cos(mAngle) * curSpeed;
	dy = sin(mAngle) * curSpeed;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Expressions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::eSpeed(LPVAL theParams, ExpReturn& ret)
{
	return ret = GetSpeed();
}

long ExtObject::eMaxSpeed(LPVAL theParams, ExpReturn& ret)
{
	return ret = maxspeed;
}

long ExtObject::eAccRate(LPVAL theParams, ExpReturn& ret)
{
	return ret = acc;
}

long ExtObject::eDecRate(LPVAL theParams, ExpReturn& ret)
{
	return ret = dec;
}

long ExtObject::eMovAngle(LPVAL theParams, ExpReturn& ret)
{
	return ret = DEGREES(atan2(dy,dx));
}

long ExtObject::eVectorX(LPVAL theParams, ExpReturn& ret)
{
	return ret = dx;
}

long ExtObject::eVectorY(LPVAL theParams, ExpReturn& ret)
{
	return ret = dy;
}

#endif // #ifdef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// ACE tables
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
	// Define your ACE tables here.
	// Use ADDPARAM before each ADDACT/ADDCND/ADDEXP to add formal parameters.
	
	// Format:
	// ADDPARAM(type, title, description)
	//
	// type = the type of parameter.  See the PARAMS enum.
	// title = title of parameter, in expression editor
	// description = text to go in the text box describing this parameter.  Try to give
	//				 a helpful description.

	/////////////////////////////
	// Conditions

	// Format:
	// ADDCND(menuID, routine, scriptname, displaytext, flags)
	//
	// menuID = ID in the menu resource, must start at 2000
	// routine = address of the function associated
	// scriptname = reserved for future script use, e.g. "Set X position" script name could be "SetX"
	// displaytext = text to display in event editor, %0 is first param, %1 second etc
	// flags: can be
	// 0: ordinary evaluated event
	// CND_TRIGGERED: ordinary event trigger

	ADDCND("Is moving", "Movement", "%o is moving", &ExtObject::cIsMoving, "IsMoving", 0);
	ADDCND("On bounce", "Movement", "%o On Bounce", &ExtObject::cOnBounce, "OnBounce", CND_TRIGGERED);

	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAM(PARAM_VALUE, "Speed", "Speed to compare to, in pixels per second.");
	ADDCND("Compare speed", "Movement", "%o Speed %0 %1", &ExtObject::cCompareSpeed, "CompareSpeed", 0);

	/////////////////////////////
	// Actions

	// ADDACT(menuID, routine, scriptname, displaytext)
	// Same format as ADDCND, but actions have no flags.

	ADDACT("Bounce", "Movement", "Bounce", &ExtObject::aBounce, "Bounce", 0);
	ADDACT("Stop", "Movement", "Stop", &ExtObject::aStop, "Stop", 0);

	ADDPARAM(PARAM_VALUE, "Speed", "The speed the object should move at, in units of pixels per second");
	ADDACT("Set speed", "Movement", "Set speed to %0", &ExtObject::aSetSpeed, "SetSpeed", 0);

	ADDPARAM(PARAM_VALUE, "Max speed", "The maximum speed the object can move, in units of pixels per second");
	ADDACT("Set maximum speed", "Movement", "Set maximum speed to %0", &ExtObject::aSetMaxSpeed, "SetMaxSpeed", 0);

	ADDPARAM(PARAM_VALUE, "Acceleration", "The rate the object accelerates, in pixels per second per second");
	ADDACT("Set acceleration", "Movement", "Set acceleration to %0", &ExtObject::aSetAcc, "SetAcc", 0);

	ADDPARAM(PARAM_VALUE, "Deceleration", "The rate the object slows down, in pixels per second per second");
	ADDACT("Set deceleration", "Movement", "Set deceleration to %0", &ExtObject::aSetDec, "SetDec", 0);

	ADDPARAMCOMBO("Ignore input?", "Choose whether to ignore the user controls.", "Stop ignoring|Start ignoring");
	ADDACT("Set ignoring input", "Settings", "%0 user input", &ExtObject::aSetIgnoreInput, "SetIgnore", 0);

	ADDPARAMCOMBO("Activation", "Choose whether the movement is activated or deactivated", "Deactivate|Activate");
	ADDACT("Set activated", "Settings", "%0 8-direction movement", &ExtObject::aSetActivated, "SetActivated", 0);

	ADDPARAM(PARAM_VALUE, "Angle of motion", "Angle of motion, in degrees");
	ADDACT("Set angle of motion", "Movement", "Set 8-dir angle of motion to %0", &ExtObject::aSetMAngle, "SetMAngle", 0);

	/////////////////////////////
	// Expressions

	// ADDEXP(menuID, routine, expressionName, displaytext, flags)
	// Same format as conditions, except with the expression editor text.
	// expressionName = the name in the expression editor, e.g "Width" would
	// associate this expression when the user types YourObject.Width
	// Flags:  RETURN_VALUE, RETURN_STRING, RETURN_UNDEFINED

	ADDEXP("Get speed", "Movement", "Speed", &ExtObject::eSpeed, RETURN_VALUE);
	ADDEXP("Get max speed", "Movement", "MaxSpeed", &ExtObject::eMaxSpeed, RETURN_VALUE);
	ADDEXP("Get acceleration rate", "Movement", "AccRate", &ExtObject::eAccRate, RETURN_VALUE);
	ADDEXP("Get deceleration rate", "Movement", "DecRate", &ExtObject::eDecRate, RETURN_VALUE);
	ADDEXP("Get angle of motion", "Movement", "MovAngle", &ExtObject::eMovAngle, RETURN_VALUE);
	ADDEXP("Get X component of motion", "Movement", "VectorX", &ExtObject::eVectorX, RETURN_VALUE);
	ADDEXP("Get Y component of motion", "Movement", "VectorY", &ExtObject::eVectorY, RETURN_VALUE);
}

