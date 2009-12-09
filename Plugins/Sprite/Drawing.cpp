// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

/////////////////////////////
// RUNTIME drawing
// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your plugin doesn't draw anything.  This is not called when OF_NODRAW is set.
void ExtObject::Draw()
{
	renderer->SetTexture(info.curTexture);

	// No distort map
	if (distort.empty()) {

		renderer->Quad_xywh(info.x, info.y, info.w, info.h, info.displayangle, point(info.HotSpotX, info.HotSpotY),
			info.pInfo->filter, NULL, info.pInfo->skewX, info.pInfo->skewY, vertices_changed ? vertexcolors : NULL, info.pInfo->z_elevation);
	}

	// Distort map
	else {
		RenderDistorted();
	}
}

float lerp(float a, float b, float x)
{
	return a + x * (b - a);
}

point lerp2D(point a, point b, point i)
{
	return point(lerp(a.x, b.x, i.x), lerp(a.y, b.y, i.y));
}

void ExtObject::RenderDistorted()
{
	// Enable tiling for distort maps.
	cr::samplerstate_value old_addressu = renderer->GetSamplerState(cr::ss_addressu);
	cr::samplerstate_value old_addressv = renderer->GetSamplerState(cr::ss_addressv);

	renderer->SetSamplerState(cr::ss_addressu, cr::ssv_wrap);
	renderer->SetSamplerState(cr::ss_addressv, cr::ssv_wrap);

	const point hotspot(info.HotSpotX, info.HotSpotY);

	rect rc(cr::rect_xywh(info.x, info.y, info.w, info.h));
	quad q;

	if (info.angle == 0.0)
		q = quad(rc - hotspot);
	else
		q = (rc - hotspot).rotate_to_quad(cr::to_radians(info.angle), hotspot);

	rect unrotated_box(rc - hotspot);

	int cx = distort.size();
	int cy = distort.front().size();

	UINT vertex_count = cx * cy;
	UINT index_count = (cx - 1) * (cy - 1) * 6;
	renderer->BeginBatchQuads(vertex_count, index_count);

	for (int y = 0; y < cy - 1; y++) {
		for (int x = 0; x < cx - 1; x++) {
			// Add six indices for this quad
			renderer->AddIndex(x + y * cx);
			renderer->AddIndex((x+1) + y * cx);
			renderer->AddIndex(x + (y+1) * cx);
			renderer->AddIndex(x + (y+1) * cx);
			renderer->AddIndex((x+1) + y * cx);
			renderer->AddIndex((x+1) + (y+1) * cx);
		}
	}

	float xsize = cx;
	float ysize = cy;

	point tex_scale(info.curTexture->xf, info.curTexture->yf);
	point object_scale_factor(info.w / info.curTexture->image_widthf, info.h / info.curTexture->image_heightf);

	float average_scale_factor = (object_scale_factor.x + object_scale_factor.y) / 2.0f;

	for (int y = 0; y < cy; y++) {
		for (int x = 0; x < cx; x++) {

			const DistortInfo& di = distort[x][y];

			point current_ratio((float)x / (xsize - 1.0f), (float)y / (ysize - 1.0f));
			point p(lerp2D(unrotated_box.topleft(), unrotated_box.bottomright(), current_ratio));
			p += point(di.x * info.w, di.y * info.h) * object_scale_factor;		// xy offset

			if (info.angle != 0.0)
				p.rotate(cr::to_radians(info.angle), info.x, info.y);

			cr::point3d final_pt(p.x, p.y, (di.z + info.pInfo->z_elevation) * average_scale_factor);
			
			point uv_offset(di.u, di.v);
			point uv(current_ratio + uv_offset);

			renderer->AddVertex(final_pt, uv * tex_scale, di.filter * info.pInfo->filter);
		}
	}

	// Restore previous wrap mode
	renderer->SetSamplerState(cr::ss_addressu, old_addressu);
	renderer->SetSamplerState(cr::ss_addressv, old_addressv);
}

#else // RUN_ONLY

/////////////////////////////
// EDITTIME drawing
// Draw your object in the layout editor.

int FindAFrameWithAnimationName(VEditTime* pEditTime, int anim, CString startAnim, int index)
{
	if(startAnim.CompareNoCase(pEditTime->GetAnimationName(anim)) == 0 // Equal 
		&& pEditTime->QueryAnimationFrameCount(anim) > 0)
	{
		int frame = pEditTime->GetAnimationFrame(anim, index-1);
		if(frame != -1)
			return frame;
		else
			return pEditTime->GetAnimationFrame(anim, 0);
	}
	else
	{
		int count = pEditTime->GetSubAnimationCount(anim);
		for(int a = 0; a < count; a++)
		{
			int result = FindAFrameWithAnimationName(pEditTime, pEditTime->GetSubAnimation(anim, a), startAnim, index);
			if(result != -1)
				return result;
		}
	}
	return -1;
}

int FindAFrameOfFirstAnimation(VEditTime* pEditTime, int anim, int index)
{
	if(pEditTime->QueryAnimationFrameCount(anim) > 0)
	{
		int frame = pEditTime->GetAnimationFrame(anim, index-1);
		if(frame != -1)
			return frame;
		else
			return pEditTime->GetAnimationFrame(anim, 0);
	}
	else
	{
		int count = pEditTime->GetSubAnimationCount(anim);
		for(int a = 0; a < count; a++)
		{
			int result = FindAFrameOfFirstAnimation(pEditTime, pEditTime->GetSubAnimation(anim, a), index);
			if(result != -1)
				return result;
		}
	}
	return -1;
}

int FindAFrame(VEditTime* pEditTime, int anim, CString startAnim, int index)
{
	int frame = FindAFrameWithAnimationName(pEditTime, anim, startAnim, index);
	if(frame != -1)
		return frame;
	else
		return FindAFrameOfFirstAnimation(pEditTime, anim, index);	
}

void EditExt::Draw()
{

	//D3DCOLOR f = D3D_CONVERTCOLORREF(filter);
	//f = D3D_ARGB(opacity, D3D_R(f), D3D_G(f), D3D_B(f));
	D3DCOLOR f = pInfo->filter;


	// Simply blit the object icon texture to the frame
	int image = FindAFrame(pEditTime, m_iAnimationRoot, startAnim, startFrame);
	if(image == -1)
		return;

	iTexture = pEditTime->AddImageAsTexture(image);

	pEditTime->SetTexture(iTexture);
	POINT srcHotSpot = pEditTime->GetImageCenterPoint(image);
	imgsize  = pEditTime->GetImageSize(image);


	pInfo->originX = (float)(srcHotSpot.x) * pInfo->objectWidth / (float)max(0, imgsize.cx);
	pInfo->originY = (float)(srcHotSpot.y) * pInfo->objectHeight / (float)max(0, imgsize.cy);
	float HotSpotAngle = atan2((float)pInfo->originY, (float)pInfo->originX);
	float HotSpotDist = sqrt((float)pInfo->originX * pInfo->originX + pInfo->originY * pInfo->originY);


	if (pInfo->originX == 0 && pInfo->originY == 0)
		pEditTime->BlitrcSkew(pInfo->objectX, pInfo->objectY, pInfo->objectWidth, pInfo->objectHeight, (float)pInfo->objectAngle, f, NULL, skewX, skewY);
	else 
	{
		float sin_a, cos_a;
		cr::sincosf(cr::to_radians(pInfo->objectAngle) + HotSpotAngle, &sin_a, &cos_a);
		float dx = HotSpotDist * cos_a;
		float dy = HotSpotDist * sin_a;
		pEditTime->BlitrcSkew(pInfo->objectX-dx, pInfo->objectY-dy, pInfo->objectWidth, pInfo->objectHeight, (float)pInfo->objectAngle, f, NULL, skewX, skewY);
	}
	
	pInfo->originX = (float)(srcHotSpot.x) / (float)max(1, imgsize.cx);
	pInfo->originY = (float)(srcHotSpot.y) / (float)max(1, imgsize.cy);
}

#endif // RUN_ONLY