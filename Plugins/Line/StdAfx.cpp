// Include StdAfx.h
#include "StdAfx.h"

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
