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
	return speed != 0;
}

long ExtObject::cOnBounce		(LPVAL theParams)
{
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aBounce		(LPVAL theParams)
{
	// Invert speed - basic effect but works
	speed *= -1.0f;

	pRuntime->GenerateEvent(info.oid, 1, this); // On bounce

	return 0;
}

long ExtObject::aStop			(LPVAL theParams)
{
	// Reset speed
	speed = 0.0f;
	return 0;
}

long ExtObject::aSetSpeed		(LPVAL theParams)
{
	speed = theParams[0].GetFloat();
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

long ExtObject::aSetSkidRecover(LPVAL theParams)
{
	skidrecover = theParams[0].GetFloat();

	return 0;
}

long ExtObject::aSetRotateSpeed(LPVAL theParams)
{
	av = theParams[0].GetFloat();

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Expressions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::eSpeed(LPVAL theParams, ExpReturn& ret)
{
	return ret = speed;
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
	return ret = mAngle;
}

long ExtObject::eVectorX(LPVAL theParams, ExpReturn& ret)
{
	return ret =(cos(mAngle) * speed);
}

long ExtObject::eVectorY(LPVAL theParams, ExpReturn& ret)
{
	return ret =(sin(mAngle) * speed);
}

long ExtObject::eGetRotateSpeed(LPVAL theParams, ExpReturn& ret)
{
	return ret = av;
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
	ADDACT("Set activated", "Settings", "%0 Car movement", &ExtObject::aSetActivated, "SetActivated", 0);

	ADDPARAM(PARAM_VALUE, "Skid recover rate", "The skid recover rate, in degrees per second.");
	ADDACT("Set skid recover rate", "Movement", "Set skid recover rate to %0", &ExtObject::aSetSkidRecover, "SetSkidRecover", 0);

	ADDPARAM(PARAM_VALUE, "Rotate speed", "The rotate speed, in degrees per second.");
	ADDACT("Set rotate speed", "Movement", "Set rotate speed to %0 degrees per second", &ExtObject::aSetRotateSpeed, "SetRotateSpeed", 0);
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
	ADDEXP("Get rotate speed", "Movement", "RotateSpeed", &ExtObject::eGetRotateSpeed, RETURN_VALUE);
}
