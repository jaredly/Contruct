// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

/////////////////////////////
// RUNTIME drawing
// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your plugin doesn't draw anything.  This is not called when OF_NODRAW is set.
void ExtObject::Draw()
{
	cr::renderstate_value old_zwrite_state = renderer->GetRenderState(cr::rs_zwrite_enabled);

	if (old_zwrite_state != cr::rsv_disabled)
		renderer->SetRenderState(cr::rs_zwrite_enabled, cr::rsv_disabled);

	renderer->SmoothLine3D(start, end, info.pInfo->filter, line_width);

	if (old_zwrite_state != cr::rsv_disabled)
		renderer->SetRenderState(cr::rs_zwrite_enabled, old_zwrite_state);
}

#else // RUN_ONLY

/////////////////////////////
// EDITTIME drawing
// Draw your object in the layout editor.
void EditExt::Draw()
{
	RECT rc;
	rc.left = pInfo->objectX;
	rc.top = pInfo->objectY;
	rc.right = pInfo->objectX + pInfo->objectWidth;
	rc.bottom = pInfo->objectY + pInfo->objectHeight;

	float drawX = pInfo->objectX + cos(RADIANS(pInfo->objectAngle - 90.0)) * (pInfo->objectHeight / 2.0);
	float drawY = pInfo->objectY + sin(RADIANS(pInfo->objectAngle - 90.0)) * (pInfo->objectHeight / 2.0);

	pEditTime->SetTexture(-1);
	pEditTime->Blitrc(drawX, drawY, pInfo->objectWidth, pInfo->objectHeight, pInfo->objectAngle, pInfo->filter);
}

#endif // RUN_ONLY