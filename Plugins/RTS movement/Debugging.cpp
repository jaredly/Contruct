// Include StdAfx
#include "StdAfx.h"

// Called to update the debugger display.
// Use calls to pRuntime->AddDebuggerItem to display items.
void ExtObject::DebuggerUpdateDisplay(ExpStore*& pPrivateVars)
{
	pRuntime->AddDebuggerItem("Speed", speed, false);
	pRuntime->AddDebuggerItem("Max Speed", params.maxspeed, false);
	pRuntime->AddDebuggerItem("Acceleration", params.acc, false);
	pRuntime->AddDebuggerItem("Deceleration", params.dec, false);
	pRuntime->AddDebuggerItem("Rotate Speed", DEGREES(params.av), false);
	pRuntime->AddDebuggerItem("Target X", targetX, false);
	pRuntime->AddDebuggerItem("Target Y", targetY, false);
}

// Called when a debugger value has been changed.
void ExtObject::OnDebuggerValueChanged(const char* name, const char* value)
{
	CString label = name;
	CString text = value;
	__int64 iValue = _atoi64(value);
	double fValue = atof(value);

	if (label == "Speed")
		speed = fValue;
	else if (label == "Max Speed")
		params.maxspeed = fValue;
	else if (label == "Rotate Speed")
		params.av = RADIANS(fValue);
	else if (label == "Target X")
		targetX = fValue;
	else if (label == "Target Y")
		targetY = fValue;
	else if (label == "Acceleration")
		params.acc = fValue;
	else if (label == "Deceleration")
		params.dec = fValue;
}