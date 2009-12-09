// Include StdAfx
#include "StdAfx.h"

// Called to update the debugger display.
// Use calls to pRuntime->AddDebuggerItem to display items.
void ExtObject::DebuggerUpdateDisplay(ExpStore*& pPrivateVars)
{
	pRuntime->AddDebuggerItem("Max Speed", max_floor_speed, false);
	pRuntime->AddDebuggerItem("Acceleration", floor_acc, false);
	pRuntime->AddDebuggerItem("Deceleration", floor_dec, false);
	pRuntime->AddDebuggerItem("Gravity (fall)", grav_fall, false);
	pRuntime->AddDebuggerItem("Gravity (jump)", grav_jump, false);
	pRuntime->AddDebuggerItem("Gravity (jump_sustain)", grav_jump_sustain, false);
	pRuntime->AddDebuggerItem("Jump Strength", jump_strength, false);
	pRuntime->AddDebuggerItem("Jump Sustain", jump_sustain, false);
}

// Called when a debugger value has been changed.
void ExtObject::OnDebuggerValueChanged(const char* name, const char* value)
{
	CString label = name;
	CString text = value;
	__int64 iValue = _atoi64(value);
	double fValue = atof(value);

	if (label == "Max Speed")
		max_floor_speed = fValue;
	else if (label == "Acceleration")
		floor_acc = fValue;
	else if (label == "Deceleration")
		floor_dec = fValue;
	else if (label == "Gravity (fall)")
		grav_fall = fValue;
	else if (label == "Gravity (jump)")
		grav_jump = fValue;
	else if (label == "Gravity (sustain)")
		grav_jump_sustain = fValue;
	else if (label == "Jump Strength")
		jump_strength = fValue;
	else if (label == "Jump Sustain")
		jump_sustain = iValue;
}