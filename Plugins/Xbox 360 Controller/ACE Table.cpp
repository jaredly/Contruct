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
	// Format:
	// ADDCND(List name, Category, Display string, Function address, Script name, Flags)
	ADDPARAMDEF(PARAM_VALUE, "Controller", "The controller number (1-4)", "1");
	ADDPARAMCOMBO("Control", "The button or trigger or thumbs", XboxControlString);
	ADDCND("Control is down", "Control", "%o(%0): %1 is down", &ExtObject::cButtonDown, "IsButtonDown", 0);

	/////////////////////////////
	// Actions
	// Format:
	// ADDCND(List name, Category, Display string, Function address, Script name, Flags)

	ADDPARAMDEF(PARAM_VALUE, "Controller", "The controller number (1-4)", "1");
	ADDPARAMCOMBO("Vibrate", "Select which side", "Left|Right|Both");
	ADDPARAMDEF(PARAM_VALUE, "Strength", "0 (none) - 1 (full)", "1");
	ADDPARAMDEF(PARAM_VALUE, "Duration", "Milliseconds (use negative for infinite)", "1000");
	ADDPARAMCOMBO("Method", "Select the method", "Constant|Fade Out");
	ADDACT("Vibrate Controller", "Vibrates the controller", "(%0): Vibrate %1 to %2 (%3 ms, method: %4)",  &ExtObject::aVibrate, "Vibrate", 0);

	/////////////////////////////
	// Expressions
	// ADDEXP(List name, Category, Display string, Function address, Flags)
	ADDPARAM(PARAM_VALUE, "1", "Controller (1-4)");
	ADDEXP("Left Thumb X", "Analogue", "LeftThumbX", &ExtObject::eLeftThumbX, RETURN_FLOAT);
	ADDPARAM(PARAM_VALUE, "1", "Controller (1-4)");
	ADDEXP("Left Thumb Y", "Analogue", "LeftThumbY", &ExtObject::eLeftThumbY, RETURN_FLOAT);
	ADDPARAM(PARAM_VALUE, "1", "Controller (1-4)");
	ADDEXP("Right Thumb X", "Analogue", "RightThumbX", &ExtObject::eRightThumbX, RETURN_FLOAT);
	ADDPARAM(PARAM_VALUE, "1", "Controller (1-4)");
	ADDEXP("Right Thumb Y", "Analogue", "RightThumbY", &ExtObject::eRightThumbY, RETURN_FLOAT);

	ADDPARAM(PARAM_VALUE, "1", "Controller (1-4)");
	ADDEXP("Left Trigger", "Triggers", "LeftTrigger", &ExtObject::eLeftTrigger, RETURN_FLOAT);
	ADDPARAM(PARAM_VALUE, "1", "Controller (1-4)");
	ADDEXP("Right Trigger", "Triggers", "RightTrigger", &ExtObject::eRightTrigger, RETURN_FLOAT);

	ADDPARAM(PARAM_VALUE, "1", "Controller (1-4)");
	ADDPARAM(PARAM_VALUE, "Button", "0");
	ADDEXP("Button State", "Buttons", "ButtonState", &ExtObject::eButtonState, RETURN_INTEGER);

	// This line includes your common ACEs as specified in Main.h
#include "..\..\Common\CommonAceTable.hpp"
}


