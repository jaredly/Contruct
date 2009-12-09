// Include StdAfx
#include "StdAfx.h"

// Include any used common ACE definitions.
#ifdef RUN_ONLY
#include "..\..\Common\CommonAceDef.hpp"
#endif

//////////////////////////////////////////////////////////////////////////////////
// ACE table
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
	// This line includes your common ACEs as specified in Main.h
	#include "..\..\Common\CommonAceTable.hpp"
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

	ADDPARAMCOMBO("Comparison", "Select the way to compare gravity", COMPARISON_COMBO_TEXT);
	ADDPARAM(PARAM_VALUE, "Gravity", "Enter the gravity value to compare.");
	ADDCND("Compare gravity", "Movement", "%o Gravity %0 %1", &ExtObject::cCompareGravity, "CompareGravity", 0);

	ADDCND("Is walking", "Movement", "%o is walking", &ExtObject::cIsWalking, "IsWalking", 0);
	ADDCND("Is jumping", "Movement", "%o is jumping", &ExtObject::cIsJumping, "IsJumping", 0);
	ADDCND("Is falling", "Movement", "%o is falling", &ExtObject::cIsFalling, "IsFalling", 0);
	ADDCND("Is on ground", "Movement", "%o is on ground", &ExtObject::cIsOnGround, "IsOnGround", 0);
	ADDCND("Wall to the left", "Movement", "%o has wall to left", &ExtObject::cWallLeft, "WallLeft", 0);
	ADDCND("Wall to the right", "Movement", "%o has wall to right", &ExtObject::cWallRight, "WallRight", 0);
	ADDCND("Ceiling just above", "Movement", "%o has ceiling just above", &ExtObject::cCeilingAbove, "CeilingAbove", 0);

	/////////////////////////////
	// Actions

	// ADDACT(menuID, routine, scriptname, displaytext)
	// Same format as ADDCND, but actions have no flags.

	ADDACT("Bounce", "Movement", "Bounce", &ExtObject::aBounce, "Bounce", 0);
	ADDACT("Stop", "Movement", "Stop", &ExtObject::aStop, "Stop", 0);

	ADDPARAM(PARAM_VALUE, "Max floor speed", "The maximum X speed the object can move, in units of pixels per second");
	ADDACT("Set maximum floor speed", "Movement", "Set maximum X speed to %0", &ExtObject::aSetMaxXFloorSpeed, "SetMaxFloorSpeed", 0);

	ADDPARAM(PARAM_VALUE, "Acceleration", "The rate the object accelerates, in pixels per second per second");
	ADDACT("Set floor acceleration", "Movement", "Set acceleration to %0", &ExtObject::aSetFloorAcc, "SetFloorAcc", 0);

	ADDPARAM(PARAM_VALUE, "Deceleration", "The rate the object slows down, in pixels per second per second");
	ADDACT("Set floor deceleration", "Movement", "Set deceleration to %0", &ExtObject::aSetFloorDec, "SetFloorDec", 0);

	ADDACT("Jump", "Movement", "Jump", &ExtObject::aJump, "Jump", 0);

	ADDPARAMCOMBO("Set Gravity Direction", "Select which direction you want gravity", "Right|Down|Left|Up");
	ADDACT("Set gravity direction", "Settings", "Set gravity direction: %0", &ExtObject::aSetGravDir, "SetGravDir", 0);

	ADDPARAMCOMBO("Ignore input?", "Choose whether to ignore the user controls.", "Stop ignoring|Start ignoring");
	ADDACT("Set ignoring input", "Settings", "%0 user input", &ExtObject::aSetIgnoreInput, "SetIgnore", 0);

	ADDPARAMCOMBO("Activation", "Choose whether the movement is activated or deactivated", "Deactivate|Activate");
	ADDACT("Set activated", "Settings", "%0 Platform movement", &ExtObject::aSetActivated, "SetActivated", 0);

	ADDPARAM(PARAM_VALUE, "Jump strength", "Speed at which jumps start, in vertical pixels per second.");
	ADDACT("Set jump strength", "Settings", "Set jump strength to %0", &ExtObject::aSetJumpStrength, "SetJumpStrength", 0);

	ADDPARAM(PARAM_VALUE, "Jump sustain", "Max time in milliseconds a jump can sustain, to allow variable height jumps.");
	ADDACT("Set jump sustain", "Settings", "Set jump sustain to %0", &ExtObject::aSetJumpSustain, "SetJumpSustain", 0);

	ADDPARAM(PARAM_VALUE, "Gravity", "Force of gravity (downwards acceleration pixels per second per second).");
	ADDACT("Set gravity", "Settings", "Set gravity to %0", &ExtObject::aSetGravity, "SetGravity", 0);

	ADDPARAM(PARAM_VALUE, "Set Vector X", "Horizontal speed, in pixels per second.");
	ADDACT("Set X component of motion", "Movement", "Set horizontal speed to %0", &ExtObject::aSetXSpeed, "SetVectorX", 0);

	// NEW
	ADDPARAM(PARAM_VALUE, "Set Vector Y", "Vertical speed, in pixels per second.");
	ADDACT("Set Y component of motion", "Movement", "Set vertical speed to %0", &ExtObject::aSetYSpeed, "SetVectorY", 0);

	ADDPARAM(PARAM_VALUE, "Max air speed", "The maximum X speed the object can move, in units of pixels per second");
	ADDACT("Set maximum air speed", "Movement", "Set maximum air x speed to %0", &ExtObject::aSetMaxXAirSpeed, "SetMaxAirSpeed", 0);

	ADDPARAM(PARAM_VALUE, "Acceleration", "The rate the object accelerates, in pixels per second per second");
	ADDACT("Set air acceleration", "Movement", "Set air acceleration to %0", &ExtObject::aSetAirAcc, "SetAcc", 0);

	ADDPARAM(PARAM_VALUE, "Deceleration", "The rate the object slows down, in pixels per second per second");
	ADDACT("Set air deceleration", "Movement", "Set air deceleration to %0", &ExtObject::aSetAirDec, "SetDec", 0);

	ADDPARAM(PARAM_VALUE, "Max Y speed", "The maximum Y speed the object can fall at, in units of pixels per second");
	ADDACT("Set maximum fall speed", "Movement", "Set maximum Y speed to %0", &ExtObject::aSetMaxYSpeed, "SetMaxYSpeed", 0);

	ADDPARAMCOMBO("Fall through platforms", "Allow the player to fall through platform", "No|Yes");
	ADDACT("Fall through platforms?", "Movement", "Fall through platforms ? %0", &ExtObject::aSetFallThroughPlatforms, "SeFallThroughPlatforms", 0);

	ADDPARAMCOMBO("Invert?", "Choose whether to invert the controls.", "Normal|Inverted");
	ADDACT("Set invert input", "Settings", "%0 user input", &ExtObject::aSetInvertControl, "SetInvert", 0);

	// NEW
	ADDPARAM(PARAM_VALUE, "Gravity", "Force of gravity (downwards acceleration pixels per second per second).");
	ADDACT("Set jump gravity", "Settings", "Set jump gravity to %0", &ExtObject::aSetJumpGravity, "SetGravity", 0);
	
	ADDPARAM(PARAM_VALUE, "Gravity", "Force of gravity (downwards acceleration pixels per second per second).");
	ADDACT("Set jump sustain gravity", "Settings", "Set jump sustain gravity to %0", &ExtObject::aSetJumpSustainGravity, "SetGravity", 0);


	/////////////////////////////
	// Expressions

	// ADDEXP(menuID, routine, expressionName, displaytext, flags)
	// Same format as conditions, except with the expression editor text.
	// expressionName = the name in the expression editor, e.g "Width" would
	// associate this expression when the user types YourObject.Width
	// Flags:  RETURN_VALUE, RETURN_STRING, RETURN_UNDEFINED

	ADDEXP("Get speed", "Movement", "Speed", &ExtObject::eSpeed, RETURN_VALUE);
	ADDEXP("Get max floor x speed", "Movement", "MaxFloorSpeed", &ExtObject::eMaxXFloorSpeed, RETURN_VALUE);
	ADDEXP("Get floor acceleration rate", "Movement", "FloorAccRate", &ExtObject::eAccFloorRate, RETURN_VALUE);
	ADDEXP("Get floor deceleration rate", "Movement", "FloorDecRate", &ExtObject::eDecFloorRate, RETURN_VALUE);
	ADDEXP("Get angle of motion", "Movement", "MovAngle", &ExtObject::eMovAngle, RETURN_VALUE);
	ADDEXP("Get X component of motion", "Movement", "VectorX", &ExtObject::eVectorX, RETURN_VALUE);
	ADDEXP("Get Y component of motion", "Movement", "VectorY", &ExtObject::eVectorY, RETURN_VALUE);
	ADDEXP("Get max air x speed", "Movement", "MaxAirSpeed", &ExtObject::eMaxXAirSpeed, RETURN_VALUE);
	ADDEXP("Get max y speed", "Movement", "MaxYSpeed", &ExtObject::eMaxYSpeed, RETURN_VALUE);
	ADDEXP("Get air acceleration rate", "Movement", "FloorAccRate", &ExtObject::eAccAirRate, RETURN_VALUE);
	ADDEXP("Get air deceleration rate", "Movement", "FloorDecRate", &ExtObject::eDecAirRate, RETURN_VALUE);
	ADDEXP("Get fall gravity", "Movement", "Gravity", &ExtObject::eGrav, RETURN_VALUE);
	ADDEXP("Get gravity angle", "Movement", "GravityAngle", &ExtObject::eGravAngle, RETURN_VALUE);
	ADDEXP("Get jump strength", "Movement", "JumpStrength", &ExtObject::eJumpStrength, RETURN_VALUE);
	ADDEXP("Get jump strength", "Movement", "JumpSustain", &ExtObject::eJumpSustain, RETURN_VALUE);
	ADDEXP("Get jump gravity", "Movement", "Gravity", &ExtObject::eJumpGrav, RETURN_VALUE);
	ADDEXP("Get jump sustain gravity", "Movement", "Gravity", &ExtObject::eJumpSustainGrav, RETURN_VALUE);

}


