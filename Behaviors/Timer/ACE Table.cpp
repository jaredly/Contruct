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
	ADDPARAM(PARAM_STRING, "Timer name", "Identifying timer name");
	ADDCND("On timer", "Timers", "%o On timer %0", &ExtObject::cOnTimer, "OnTimer", CND_TRIGGERED);

	/////////////////////////////
	// Actions
	// Format:
	// ADDCND(List name, Category, Display string, Function address, Script name, Flags)
	ADDPARAM(PARAM_STRING, "Timer name", "Identifying timer name");
	ADDPARAM(PARAM_VALUE, "Timer length", "Timer length in milliseconds");
	ADDPARAMCOMBO("Repeat", "Whether the timer should repeat till stopped", "Don't repeat|Repeat");
	ADDPARAMCOMBO("Destroy", "Destroy object when finished", "Don't destroy|Destroy");
	ADDACT("Start timer", "Timers", "Start timer %0 with length %1, %2, %3 when finished", &ExtObject::aStartTimer, "StartTimer", 0);

	ADDPARAM(PARAM_STRING, "Timer name", "Identifying timer name");
	ADDACT("Stop timer", "Timers", "Stop timer %0", &ExtObject::aStopTimer, "StopTimer", 0);

#include "..\..\Common\CommonAceTable.hpp"
}


