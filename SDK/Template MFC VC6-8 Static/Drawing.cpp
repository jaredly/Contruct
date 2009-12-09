// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

/////////////////////////////
// RUNTIME drawing
// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your plugin doesn't draw anything.  This is not called when OF_NODRAW is set.
void ExtObject::Draw()
{
	// Don't draw anything
}

#else // RUN_ONLY

/////////////////////////////
// EDITTIME drawing
// Draw your object in the layout editor.
void EditExt::Draw()
{
	CRect rc(pInfo->objectX, pInfo->objectY, pInfo->objectX + pInfo->objectWidth, pInfo->objectY + pInfo->objectHeight);

	// Draw a simple rectangle
	pEditTime->Box(rc, D3D_CONVERTCOLORREF(RGB(0, 0, 0)));
}

#endif // RUN_ONLY