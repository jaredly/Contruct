// Include StdAfx.h
#include "StdAfx.h"

// Actions, Conditions and Expressions ACESEntry arrays
//vector<ACESEntry> Conditions;
//vector<ACESEntry> Actions;
//vector<ACESEntry> Expressions;

// Routine lists
//vector<ConditionRoutine> CndRoutines;
//vector<EXTROUTINE> ActRoutines;
//vector<EXTROUTINE> ExpRoutines;

//vector<CString> ExpNames;

void EndProperties(int iMode, CVirtualPropItem* item, MicroEdittime* pm)
{
	if (iMode == MODE_INIT) {
		ASSIGN_CSTRING(item->Description, "");
		ASSIGN_CSTRING(item->Label, "");
		ASSIGN_CSTRING(item->Text, "");
		item->Type = -1;	// end
		pm->Proc(item);		// post end to property vector
	}
}
