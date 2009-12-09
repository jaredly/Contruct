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
	// CND_FASTTRIGGERED: fast trigger

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to test.");
	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAM(PARAM_VALUE, "Value", "Value to compare to");
	ADDCND("Compare private variable", "Private variables", "%o Value %0 %1 %2", &ExtObject::cValueCmp, "CompareValue", 0);

	/////////////////////////////
	// Actions

	// ADDACT(menuID, routine, scriptname, displaytext)
	// Same format as ADDCND, but actions have no flags.

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to store");
	ADDACT("Set value", "Private variables", "Set %0 to %1", &ExtObject::aSetValue, "SetValue", 0);

	ADDPARAM(PARAM_VALUE, "Opacity", "New semitransparency value, 0 (invisible) to 100 (opaque)");
	ADDACT("Set opacity", "Appearance", "Set opacity to %0", &ExtObject::aSetOpacity, "SetOpacity", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to add.");
	ADDACT("Add to value", "Private variables", "Add %1 to %0", &ExtObject::aAddValue, "AddValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to subtract.");
	ADDACT("Subtract from value", "Private variables", "Subtract %1 from %0", &ExtObject::aSubValue, "SubValue", 0);

	ADDPARAM(PARAM_OBJECT, "Object", "Object to attach to.");
	ADDPARAM(PARAM_VALUE, "Padding", "Pixels padding around the object to attach.");
	ADDACT("Attach to object", "Attach", "Attach to object %0 with %1 padding", &ExtObject::aAttachToObj, "Attach", 0);

	ADDPARAM(PARAM_OBJECT, "Object", "Object to use texture from");
	ADDACT("Copy image from object", "Image", "Use image from %0", &ExtObject::aCopyImageFromObj, "CopyImage", 0);

	ADDPARAMCOMBO("Corner", "Select the corner of the image", "Top-left|Top-right|Bottom-left|Bottom-right");
	ADDPARAMDEF(PARAM_COLOR, "Colour", "Colour", "16777215");
	ADDPARAMDEF(PARAM_VALUE, "Opacity", "0 - 100", "100");
	ADDACT("Set corner filter", "Appearance", "Set corner %0 colour to %1 (%2%)", &ExtObject::aSetVertex, "SetCornerFilter", 0);

	ADDPARAM(PARAM_VALUE, "Left Margin", "Pixels");
	ADDPARAM(PARAM_VALUE, "Top Margin", "Pixels");
	ADDPARAM(PARAM_VALUE, "Right Margin", "Pixels");
	ADDPARAM(PARAM_VALUE, "Bottom Margin", "Pixels");
	ADDACT("Set Image Margin", "Image", "Set image margin to (%0,%1,%2,%3)", &ExtObject::aSetMargin, "SetMargin", 0);


	/////////////////////////////
	// Expressions

	// ADDEXP(menuID, routine, expressionName, displaytext, flags)
	// Same format as conditions, except with the expression editor text.
	// expressionName = the name in the expression editor, e.g "Width" would
	// associate this expression when the user types YourObject.Width
	// Flags:  RETURN_VALUE, RETURN_STRING, RETURN_UNDEFINED
	ADDEXP("Get opacity", "Appearance", "Opacity", &ExtObject::eGetOpacity, RETURN_FLOAT);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Value", "Name of value.");
	ADDEXP("Get private variable", "Private variables", "Value", &ExtObject::eGetValue, 0);

#include "..\..\Common\CommonAceTable.hpp"
}


