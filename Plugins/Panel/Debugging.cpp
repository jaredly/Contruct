// Include StdAfx
#include "StdAfx.h"

// Called to update the debugger display.
// Use calls to pRuntime->AddDebuggerItem to display items.
void ExtObject::DebuggerUpdateDisplay(ExpStore*& pPrivateVars)
{
	CString nameAttach;
	
	if (objAttach == NULL)
		nameAttach = "(none)";
	else
		nameAttach.Format("%s #%d", objAttach->pType->Name, pRuntime->GetInstanceIndex(objAttach));

	pRuntime->AddDebuggerItem("Attached to:", nameAttach, true);

	if (!privateVars.empty())
		pPrivateVars = &privateVars[0];
}

// Called when a debugger value has been changed.
void ExtObject::OnDebuggerValueChanged(const char* name, const char* value)
{
	//CheckDebugVariableChange(name, value, privateVars, this, pRuntime);
}