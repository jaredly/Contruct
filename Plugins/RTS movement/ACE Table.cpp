// Include StdAfx
#include "StdAfx.h"

// Include any used common ACE definitions.
#ifdef RUN_ONLY
#include "..\..\Common\CommonAceDef.hpp"
#endif

RTSParameters::RTSParameters()
: acc(150.0f),
  dec(300.0f),
  maxspeed(250.0f),
  av(180.0f),
  pathfinding(1),	// around solid
  rotate(true),
  blockedspeed(75.0f),
  cellsize(4)		// 60x60
{
}

void RTSParameters::Serialize(bin& ar)
{
	int Version = 4;
	SerializeVersion(ar, Version);

	if (ar.loading)
	{
		bool oldPathfinding = false;
		pathfinding = 0;	// Off

		switch (Version) {
		case 1:
			ar >> acc >> dec >> maxspeed >> av >> oldPathfinding >> rotate;

			if (oldPathfinding)
				pathfinding = 1;	// Around solid

			break;
		case 2:
			// Version 2: added blocked speed
			ar >> acc >> dec >> maxspeed >> av >> oldPathfinding >> rotate >> blockedspeed;

			if (oldPathfinding)
				pathfinding = 1;	// Around solid

			break;
		case 3:
			// Version 3: added cell size
			ar >> acc >> dec >> maxspeed >> av >> oldPathfinding >> rotate >> blockedspeed >> cellsize;

			if (oldPathfinding)
				pathfinding = 1;	// Around solid
			break;

		case 4:
			// Version 4: pathfinding checkbox changed to Off / Around solid / Custom combo
			ar >> acc >> dec >> maxspeed >> av >> pathfinding >> rotate >> blockedspeed >> cellsize;
			break;
		}
	}
	else
	{
		ar << acc << dec << maxspeed << av << pathfinding << rotate << blockedspeed << cellsize;
	}
}


//////////////////////////////////////////////////////////////////////////////////
// ACE table
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
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
	ADDCND("On arrived at target", "Movement", "%o arrived at target", &ExtObject::cTrigger, "OnArrive", CND_TRIGGERED);
	ADDCND("On arrived at waypoint", "Movement", "%o arrived at a waypoint", &ExtObject::cTrigger, "OnWaypoint", CND_TRIGGERED);
	ADDCND("On failed to find path", "Movement", "%o Failed to find path", &ExtObject::cTrigger, "OnFailed", CND_TRIGGERED);

	/////////////////////////////
	// Actions

	// ADDACT(menuID, routine, scriptname, displaytext)
	// Same format as ADDCND, but actions have no flags.

	ADDPARAM(PARAM_VALUE, "Speed", "The speed the object should move at, in pixels per frame.");
	ADDACT("Set speed", "Movement", "Set speed to %0", &ExtObject::aSetSpeed, "SetSpeed", 0);

	ADDACT("Stop", "Command", "Stop", &ExtObject::aStop, "Stop", 0);

	ADDPARAM(PARAM_VALUE, "Max speed", "The maximum speed the object can move, in pixels per frame.");
	ADDACT("Set max speed", "Movement", "Set max speed to %0", &ExtObject::aSetMaxSpeed, "SetMaxSpeed", 0);


	ADDPARAM(PARAM_VALUE, "Acceleration", "The rate the object accelerates");
	ADDACT("Set acceleration", "Movement", "Set acceleration to %0", &ExtObject::aSetAcc, "SetAcceleration", 0);

	ADDPARAM(PARAM_VALUE, "Deceleration", "The rate the object slows down");
	ADDACT("Set deceleration", "Movement", "Set deceleration to %0", &ExtObject::aSetDec, "SetDeceleration", 0);

	ADDPARAM(PARAM_VALUE, "X co-ordinate", "The X co-ordinate to move the object to, in pixels");
	ADDPARAM(PARAM_VALUE, "Y co-ordinate", "The Y co-ordinate to move the object to, in pixels");
	ADDACT("Move to position", "Command", "Move to (%0, %1)", &ExtObject::aMoveToXY, "MoveTo", 0);

	ADDPARAM(PARAM_OBJECT, "Object to move to", "The object to move to");
	ADDACT("Move to object", "Command", "Move to %0", &ExtObject::aMoveToObj, "MoveToObject", 0);

	ADDACT("Move to mouse", "Command", "Move to mouse", &ExtObject::aMoveToMouse, "MoveToMouse", 0);

	ADDPARAM(PARAM_OBJECT, "Object type", "A type of object to avoid moving over.");
	ADDACT("Set object type to avoid", "Movement", "Avoid moving over %0", &ExtObject::aSetObjToAvoid, "SetObjToAvoid", 0);

	ADDPARAM(PARAM_VALUE, "X co-ordinate", "The X co-ordinate to move the object to, in pixels");
	ADDPARAM(PARAM_VALUE, "Y co-ordinate", "The Y co-ordinate to move the object to, in pixels");
	ADDACT("Add waypoint at position", "Command", "Add waypoint at (%0, %1)", &ExtObject::aWaypointXY, "AddWaypoint", 0);

	ADDPARAM(PARAM_OBJECT, "Object to move to", "The object to move to");
	ADDACT("Add waypoint at object", "Command", "Add waypoint at %0", &ExtObject::aWaypointObj, "AddWaypointAtObject", 0);

	ADDACT("Add waypoint at mouse", "Command", "Add waypoint at mouse", &ExtObject::aWaypointMouse, "AddWaypointAtMouse", 0);

	ADDPARAMCOMBO("Activation", "Choose whether the movement is activated or deactivated", "Deactivate|Activate");
	ADDACT("Set activated", "Settings", "%0 RTS movement", &ExtObject::aSetActivated, "SetActivated", 0);

	ADDPARAM(PARAM_VALUE, "Angle of motion", "The new angle of motion, in degrees.");
	ADDACT("Set angle of motion", "Movement", "Set angle of motion to %0", &ExtObject::aSetMAngle, "SetMAngle", 0);

	ADDPARAM(PARAM_OBJECT, "Object type", "An object type or attribute to navigate around with pathfinding.");
	ADDACT("Add pathfinding obstacle", "Settings", "Add pathfinding obstacle %0", &ExtObject::aAddPfCollideType, "AddPfType", 0);

	ADDACT("Regenerate obstacles map", "Settings", "Regenerate obstacles map", &ExtObject::aRegenerateObstaclesMap, "RegenerateObstacles", 0);

	/////////////////////////////
	// Expressions

	// ADDEXP(menuID, routine, expressionName, displaytext, flags)
	// Same format as conditions, except with the expression editor text.
	// expressionName = the name in the expression editor, e.g "Width" would
	// associate this expression when the user types YourObject.Width
	// Flags:  RETURN_VALUE, RETURN_STRING, RETURN_UNDEFINED

	ADDEXP("Get speed", "Movement", "Speed", &ExtObject::eSpeed, RETURN_FLOAT);
	ADDEXP("Get max speed", "Movement", "MaxSpeed", &ExtObject::eMaxSpeed, RETURN_FLOAT);
	ADDEXP("Get acceleration rate", "Movement", "AccRate", &ExtObject::eAccRate, RETURN_FLOAT);
	ADDEXP("Get deceleration rate", "Movement", "DecRate", &ExtObject::eDecRate, RETURN_FLOAT);
	ADDEXP("Get angle of motion", "Movement", "MovAngle", &ExtObject::eMovAngle, RETURN_FLOAT);
	ADDEXP("Get X component of motion", "Movement", "VectorX", &ExtObject::eVectorX, RETURN_FLOAT);
	ADDEXP("Get Y component of motion", "Movement", "VectorY", &ExtObject::eVectorY, RETURN_FLOAT);
	ADDEXP("Get destination X", "Movement", "DestX", &ExtObject::eTargetX, RETURN_FLOAT);
	ADDEXP("Get destination Y", "Movement", "DestY", &ExtObject::eTargetY, RETURN_FLOAT);

	// This line includes your common ACEs as specified in Main.h
#include "..\..\Common\CommonAceTable.hpp"
}


