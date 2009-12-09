// Include StdAfx
#include "StdAfx.h"

// Include any used common ACE definitions.
#ifdef RUN_ONLY
#include "..\..\Common\CommonAceDef.hpp"
#endif

//////////////////////////////////////////////////////////////////////////////////
// ACE table
//////////////////////////////////////////////////////////////////////////////////

#define COMPARISON_TEXT "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal"

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
	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to test.");
	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAM(PARAM_VALUE, "Value", "Value to compare to");
	ADDCND("*Compare a private variable", "Private variables", "%o %n:  Value %0 %1 %2", &ExtObject::cValueCmp, "CompareValue", 0);

	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", COMPARISON_TEXT);
	ADDPARAM(PARAM_VALUE, "X to compare", "X of the start position to compare.");
	ADDCND("Compare start X", "Line", "%o %n: Start X %0 %1", &ExtObject::cCompareStartX, "CompareStartX", 0);

	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", COMPARISON_TEXT);
	ADDPARAM(PARAM_VALUE, "Y to compare", "Y of the start position to compare.");
	ADDCND("Compare start Y", "Line", "%o %n: Start Y %0 %1", &ExtObject::cCompareStartY, "CompareStartY", 0);

	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", COMPARISON_TEXT);
	ADDPARAM(PARAM_VALUE, "Z to compare", "Z of the start position to compare.");
	ADDCND("Compare start Z", "Line", "%o %n: Start Z %0 %1", &ExtObject::cCompareStartZ, "CompareStartZ", 0);

	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", COMPARISON_TEXT);
	ADDPARAM(PARAM_VALUE, "X to compare", "X of the end position to compare.");
	ADDCND("Compare end X", "Line", "%o %n: End X %0 %1", &ExtObject::cCompareEndX, "CompareEndX", 0);

	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", COMPARISON_TEXT);
	ADDPARAM(PARAM_VALUE, "Y to compare", "Y of the end position to compare.");
	ADDCND("Compare end Y", "Line", "%o %n: End Y %0 %1", &ExtObject::cCompareEndY, "CompareEndY", 0);

	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", COMPARISON_TEXT);
	ADDPARAM(PARAM_VALUE, "Z to compare", "Z of the end position to compare.");
	ADDCND("Compare end Z", "Line", "%o %n: End Z %0 %1", &ExtObject::cCompareEndZ, "CompareEndZ", 0);

	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", COMPARISON_TEXT);
	ADDPARAM(PARAM_VALUE, "Line width to compare", "Width of the line to compare, in pixels.");
	ADDCND("Compare line width", "Line", "%o %n: Line width %0 %1", &ExtObject::cCompareLineWidth, "CompareLineWidth", 0);

	/////////////////////////////
	// Actions
	// Format:
	// ADDCND(List name, Category, Display string, Function address, Script name, Flags)
	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to store");
	ADDACT("*Set value", "Private variables", "Set %0 to %1", &ExtObject::aSetValue, "SetValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to add.");
	ADDACT("Add to value", "Private variables", "Add %1 to %0", &ExtObject::aAddValue, "AddValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to subtract.");
	ADDACT("Subtract from value", "Private variables", "Subtract %1 from %0", &ExtObject::aSubValue, "SubValue", 0);

	ADDPARAM(PARAM_VALUE, "X", "X of line start to set.");
	ADDACT("Set start X", "Line", "Set start X to %0", &ExtObject::aSetStartX, "SetStartX", 0);

	ADDPARAM(PARAM_VALUE, "Y", "Y of line start to set.");
	ADDACT("Set start Y", "Line", "Set start Y to %0", &ExtObject::aSetStartY, "SetStartY", 0);

	ADDPARAM(PARAM_VALUE, "Z", "Z of line start to set.");
	ADDACT("Set start Z", "Line", "Set start Z to %0", &ExtObject::aSetStartZ, "SetStartZ", 0);

	ADDPARAM(PARAM_VALUE, "X", "X of line start to set.");
	ADDPARAM(PARAM_VALUE, "Y", "Y of line start to set.");
	ADDPARAM(PARAM_VALUE, "Z", "Z of line start to set.");
	ADDACT("Set start position", "Line", "Set start position to (%0, %1, %2)", &ExtObject::aSetStartPos, "SetStartPos", 0);

	ADDPARAM(PARAM_VALUE, "X", "X of line end to set.");
	ADDACT("Set end X", "Line", "Set end X to %0", &ExtObject::aSetEndX, "SetEndX", 0);

	ADDPARAM(PARAM_VALUE, "Y", "Y of line end to set.");
	ADDACT("Set end Y", "Line", "Set end Y to %0", &ExtObject::aSetEndY, "SetEndY", 0);

	ADDPARAM(PARAM_VALUE, "Z", "Z of line end to set.");
	ADDACT("Set end Z", "Line", "Set end Z to %0", &ExtObject::aSetEndZ, "SetEndZ", 0);

	ADDPARAM(PARAM_VALUE, "X", "X of line end to set.");
	ADDPARAM(PARAM_VALUE, "Y", "Y of line end to set.");
	ADDPARAM(PARAM_VALUE, "Z", "Z of line end to set.");
	ADDACT("Set end position", "Line", "Set end position to (%0, %1, %2)", &ExtObject::aSetEndPos, "SetEndPos", 0);

	ADDPARAM(PARAM_VALUE, "Line width", "Width of the line, in pixels.");
	ADDACT("Set line width", "Line", "Set line width to %0", &ExtObject::aSetLineWidth, "SetLineWidth", 0);


	/////////////////////////////
	// Expressions
	// ADDEXP(List name, Category, Display string, Function address, Flags)
	ADDPARAM(PARAM_PRIVATEVARIABLE, "'Variable name'", "Name of value.");
	ADDEXP("*Get private variable", "Private variables", "Value", &ExtObject::eGetValue, 0);

	ADDEXP("Get private variable count", "Private variables", "NumVars", &ExtObject::eNumPVs, RETURN_VALUE);

	ADDEXP("Get start X", "Line", "StartX", &ExtObject::eGetStartX, RETURN_VALUE);
	ADDEXP("Get start Y", "Line", "StartY", &ExtObject::eGetStartY, RETURN_VALUE);
	ADDEXP("Get start Z", "Line", "StartZ", &ExtObject::eGetStartZ, RETURN_VALUE);
	ADDEXP("Get end X", "Line", "EndX", &ExtObject::eGetEndX, RETURN_VALUE);
	ADDEXP("Get end Y", "Line", "EndY", &ExtObject::eGetEndY, RETURN_VALUE);
	ADDEXP("Get end Z", "Line", "EndZ", &ExtObject::eGetEndZ, RETURN_VALUE);
	ADDEXP("Get line width", "Line", "LineWidth", &ExtObject::eGetLineWidth, RETURN_VALUE);

	// This line includes your common ACEs as specified in Main.h
#include "..\..\Common\CommonAceTable.hpp"
}


