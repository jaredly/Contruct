// Include StdAfx
#include "StdAfx.h"
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

	/////////////////////////////
	// Conditions
	ADDPARAMCOMBO("Comparison", "Select how to compare the Z position", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAM(PARAM_VALUE, "Z", "Z to compare to");
	ADDCND("Compare Z position", "Size & Position", "%o Z %0 %1", &ExtObject::cCompareZ, "CompareZ", 0);

	ADDPARAMCOMBO("Comparison", "Select how to compare the depth", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAM(PARAM_VALUE, "Depth", "Depth to compare to");
	ADDCND("Compare depth", "Size & Position", "%o Depth %0 %1", &ExtObject::cCompareDepth, "CompareDepth", 0);

	// PRIVATE VARIABLES
	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to test.");
	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAM(PARAM_VALUE, "Value", "Value to compare to");
	ADDCND("Compare a private variable", "Private variables", "%o Value %0 %1 %2", &ExtObject::cValueCmp, "CompareValue", 0);


	/////////////////////////////
	// Actions
	ADDPARAM(PARAM_VALUE, "Z", "Enter the new Z position");
	ADDACT("Set Z", "Size & Position", "Set Z to %0", &ExtObject::aSetZ, "SetZ", 0);

	ADDPARAM(PARAM_VALUE, "Depth", "Enter the new depth");
	ADDACT("Set depth", "Size & Position", "Set depth to %0", &ExtObject::aSetDepth, "SetDepth", 0);

	ADDPARAM(PARAM_VALUE, "Yaw", "Enter new yaw rotation, in degrees.");
	ADDACT("Set yaw", "Angle", "Set yaw to %0 degrees", &ExtObject::aSetYaw, "SetYaw", 0);

	ADDPARAM(PARAM_VALUE, "Pitch", "Enter new pitch rotation, in degrees.");
	ADDACT("Set pitch", "Angle", "Set pitch to %0 degrees", &ExtObject::aSetPitch, "SetPitch", 0);

	ADDPARAM(PARAM_VALUE, "Roll", "Enter new roll rotation, in degrees.");
	ADDACT("Set roll", "Angle", "Set roll to %0 degrees", &ExtObject::aSetRoll, "SetRoll", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to store");
	ADDACT("Set value", "Private variables", "Set %0 to %1", &ExtObject::aSetValue, "SetValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to add.");
	ADDACT("Add to value", "Private variables", "Add %1 to %0", &ExtObject::aAddValue, "AddValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to subtract.");
	ADDACT("Subtract from value", "Private variables", "Subtract %1 from %0", &ExtObject::aSubValue, "SubValue", 0);

	/////////////////////////////
	// Expressions
	ADDEXP("Get Z", "Size & Position", "Z", &ExtObject::eGetZ, RETURN_FLOAT);
	ADDEXP("Get depth", "Size & Position", "Depth", &ExtObject::eGetDepth, RETURN_FLOAT);
	ADDEXP("Get yaw", "Angle", "Yaw", &ExtObject::eGetYaw, RETURN_FLOAT);
	ADDEXP("Get pitch", "Angle", "Pitch", &ExtObject::eGetPitch, RETURN_FLOAT);
	ADDEXP("Get roll", "Angle", "Roll", &ExtObject::eGetRoll, RETURN_FLOAT);
	ADDPARAM(PARAM_STRING, "Value", "Name of value.");
	ADDEXP("Get private variable", "Private variables", "Value", &ExtObject::eGetValue, 0);


#include "..\..\Common\CommonAceTable.hpp"
}


