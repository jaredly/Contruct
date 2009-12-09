// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

/////////////////////////////
// RUNTIME drawing
// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your plugin doesn't draw anything.  This is not called when OF_NODRAW is set.
void ExtObject::UpdateMyColors()
{
	cr_colors.c1.set_rgb(cr::color(c1).get_rgb());
	cr_colors.c2.set_rgb(cr::color(c2).get_rgb());
	cr_colors.fill.set_rgb(cr::color(fill).get_rgb());
}

void ExtObject::Draw()
{
	const point hotspot(info.HotSpotX, info.HotSpotY);
	const rect r = cr::rect_xywh(info.x, info.y, info.w, info.h);
	const cr::color& c = info.pInfo->filter;

	quad q((r - hotspot).rotate_to_quad(cr::to_radians(info.angle), r.topleft()));

	if (!transparent)
		renderer->Fill(r, cr_colors.fill * c, info.angle, hotspot);

	// Draw
	if (smoothLines) {

		renderer->SmoothLine(q.tl, q.tr, cr_colors.c1 * c, 1.0);
		renderer->SmoothLine(q.tl, q.bl, cr_colors.c1 * c, 1.0);
		renderer->SmoothLine(q.br, q.tr, cr_colors.c2 * c, 1.0);
		renderer->SmoothLine(q.br, q.bl, cr_colors.c2 * c, 1.0);

	}
	else {
		
		renderer->Line(q.tl, q.tr, cr_colors.c1 * c);
		renderer->Line(q.tl, q.bl, cr_colors.c1 * c);
		renderer->Line(q.br, q.tr, cr_colors.c2 * c);
		renderer->Line(q.br, q.bl, cr_colors.c2 * c);
	}
}

#else // RUN_ONLY

/////////////////////////////
// EDITTIME drawing
// Draw your object in the layout editor.
void EditExt::Draw()
{
	D3DCOLOR d1 = D3D_CONVERTCOLORREF(c1);
	d1 = D3D_ARGB((int)(c1_opacityf * D3D_A(pInfo->filter)), (D3D_R(d1)* D3D_R(pInfo->filter)) >> 8, (D3D_G(d1)* D3D_G(pInfo->filter)) >> 8, (D3D_B(d1)* D3D_B(pInfo->filter)) >> 8);

	D3DCOLOR d2 = D3D_CONVERTCOLORREF(c2);
	d2 = D3D_ARGB((int)(c2_opacityf * D3D_A(pInfo->filter)), (D3D_R(d2)* D3D_R(pInfo->filter)) >> 8, (D3D_G(d2)* D3D_G(pInfo->filter)) >> 8, (D3D_B(d2)* D3D_B(pInfo->filter)) >> 8);

	D3DCOLOR dfill = D3D_CONVERTCOLORREF(fill);
	dfill = D3D_ARGB((int)(fill_opacityf * D3D_A(pInfo->filter)), (D3D_R(dfill)* D3D_R(pInfo->filter)) >> 8, (D3D_G(dfill)* D3D_G(pInfo->filter)) >> 8, (D3D_B(dfill)* D3D_B(pInfo->filter)) >> 8);

	// Draw box
	pEditTime->SetTexture(-1);


	double hsx;
	double hsy;
	switch ((hotspot_position)hotspot_pos) {
	case hs_topleft:
		hsx = 0.0;
		hsy = 0.0;
		break;
	case hs_top:
		hsx = 0.5;
		hsy = 0.0;
		break;
	case hs_topright:
		hsx = 1.0;
		hsy = 0.0;
		break;
	case hs_left:
		hsx = 0.0;
		hsy = 0.5;
		break;
	case hs_center:
		hsx = 0.5;
		hsy = 0.5;
		break;
	case hs_right:
		hsx = 1.0;
		hsy = 0.5;
		break;
	case hs_bottomleft:
		hsx = 0.0;
		hsy = 1.0;
		break;
	case hs_bottom:
		hsx = 0.5;
		hsy = 1.0;
		break;
	case hs_bottomright:
		hsx = 1.0;
		hsy = 1.0;
		break;
	}

	pInfo->originX = hsx;
	pInfo->originY = hsy;

	CObjectRectangle rect(pInfo->objectX, pInfo->objectY, pInfo->objectWidth, pInfo->objectHeight, pInfo->objectAngle, hsx, hsy);

	double x,y,w,h,a;
	rect.CalculateBlitValues(x,y,w,h,a);

	
	#define GETPOINT88(A,B,C) (rect.GetPoint(A,B,C))
	#define DRAWLINE88(A,B,C,D,E)	pEditTime->Line(GETPOINT88(A,B,0).x, GETPOINT88(A,B,0).y, GETPOINT88(C,D,0).x, GETPOINT88(C,D,0).y,E)


	RECT rc;
	rc.left = pInfo->objectX;
	rc.top = pInfo->objectY;
	rc.right = rc.left + pInfo->objectWidth - 1;
	rc.bottom = rc.top + pInfo->objectHeight - 1;

	if (!transparent)
		pEditTime->Blitrc(x,y,w,h,a,dfill);

	DRAWLINE88(0,0,0,1,d1);
	DRAWLINE88(0,1,1,1,d1);
	DRAWLINE88(1,1,1,0,d2);
	DRAWLINE88(1,0,0,0,d2);

	/*pEditTime->Line(rc.left, rc.top, rc.right, rc.top, d1);
	pEditTime->Line(rc.left, rc.top, rc.left, rc.bottom, d1);
	pEditTime->Line(rc.right, rc.bottom, rc.left, rc.bottom, d2);
	pEditTime->Line(rc.right, rc.bottom, rc.right, rc.top, d2);*/
}

#endif // RUN_ONLY