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
	// Define your ACE tables here.
	// Use ADDPARAM before each ADDACT/ADDCND/ADDEXP to add parameters.
	// See the documentation for full details.

	// Note in the display string, %o is your object icon and %0-%9 make up parameters.
	// Note the script name should be a name suitable for the routine as if it were a member function in a
	// scripting language, eg. "Clear Stored Items" -> "ClearStoredItems"

	/////////////////////////////
	// Conditions
	ADDPARAM(PARAM_OBJECT, "Object to check", "Object to test line of sight to");
	ADDCND("Has LOS to object", "Line Of Sight", "%o Has LOS to %0", &ExtObject::cHasLOSToObj, "HasLOSToObject", 0);

	ADDPARAM(PARAM_VALUE, "X", "X co-ordinate to check line of sight to");
	ADDPARAM(PARAM_VALUE, "Y", "Y co-ordinate to check line of sight to");
	ADDCND("Has LOS to position", "Line Of Sight", "%o Has LOS to %0, %1", &ExtObject::cHasLOSToPos, "HasLOSToXY", 0);

	ADDPARAM(PARAM_OBJECT, "Object to check", "Object to test line of sight to");
	ADDCND("On object in LOS", "Line Of Sight", "%o On LOS to %0", &ExtObject::cOnLOSToObj, "OnLOSToObject", 0);

	ADDPARAM(PARAM_VALUE, "X", "X co-ordinate to check line of sight to");
	ADDPARAM(PARAM_VALUE, "Y", "Y co-ordinate to check line of sight to");
	ADDCND("On position in LOS", "Line Of Sight", "%o On LOS to %0, %1", &ExtObject::cOnLOSToPos, "OnLOSToXY", 0);

	/////////////////////////////
	// Actions
	ADDACT("Clear obstacles", "Obstacles", "Clear LOS obstacles", &ExtObject::aClearObstacles, "ClearObstacles", 0);

	ADDPARAM(PARAM_OBJECT, "Obstacle object", "Pick an object to add as a custom obstacle.");
	ADDACT("Add obstacle", "Obstacles", "Add LOS obstacle %0", &ExtObject::aAddObstacle, "AddObstacle", 0);

	ADDPARAM(PARAM_VALUE, "Pixels", "The range in pixels");
	ADDACT("Set range", "Field of view", "Set range to %0", &ExtObject::aSetRange, "SetRange", 0);

	ADDPARAM(PARAM_VALUE, "Degrees", "The range of degrees");
	ADDACT("Set angle range", "Field of view", "Set angle range to %0", &ExtObject::aSetAngleRange, "SetAngleRange", 0);

	// This line includes your common ACEs as specified in Main.h
#include "..\..\Common\CommonAceTable.hpp"
}


