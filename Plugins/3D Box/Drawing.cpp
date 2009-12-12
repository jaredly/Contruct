// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

/*
// Project the mouse cursor from screen space to object space
void ExtObject::ProjectScreenToWorld(D3DXVECTOR3* pOut, float screenX, float screenY, float worldZ)
{
	D3DXVECTOR3 lineBegin, lineEnd;

	// Unproject the near and far points given by the screen X,Y coords
	D3DXVECTOR3 screenSpace(screenX, screenY, 0.0f);
	D3DXVec3Unproject(&lineBegin, &screenSpace, &viewport, &projection_matrix, &view_matrix, &worldMatrix);
	screenSpace.z = 1.0f;
	D3DXVec3Unproject(&lineEnd, &screenSpace, &viewport, &projection_matrix, &view_matrix, &worldMatrix);

	// Using a plane intersection, we can determine the object space coordinates of the screen space coords
	// at a certain Z depth, intersecting the line given above.
	orig.z = worldZ;
	D3DXPlaneFromPointNormal(&plane, &orig, &normal);
	D3DXPlaneIntersectLine(pOut, &plane, &lineBegin, &lineEnd);
}
*/

/////////////////////////////
// RUNTIME drawing
// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your extension doesn't draw anything.
void ExtObject::UpdateAngle()
{
	D3DXMatrixRotationYawPitchRoll(&rotMatrix, yaw, pitch, RADIANS(info.angle));
}

void ExtObject::Draw()
{
	// Enable the Z stencil
	cr::renderstate_value old_zbuffer_state = renderer->GetRenderState(cr::rs_zbuffer_enabled);
	renderer->SetRenderState(cr::rs_zbuffer_enabled, (zbuffer ? cr::rsv_enabled : cr::rsv_disabled));

	cr::point3d objpos(info.x, info.y, z + (depth / 2.0));

	// Draw each face separately
	for (int i = 0; i < 6; i++) {

		renderer->SetTexture(textures[i]);

		float u = textures[i]->xf;
		float v = textures[i]->yf;

		//pRuntime->EndBatch();
		renderer->BeginBatchQuads(24, 36);

		renderer->AddIndex(0);
		renderer->AddIndex(1);
		renderer->AddIndex(2);
		renderer->AddIndex(2);
		renderer->AddIndex(3);
		renderer->AddIndex(0);

		cr_float sin_a, sin_b, sin_c;
		cr_float cos_a, cos_b, cos_c;

		cr_float temp;
		cr_float zz;

		cr::sincosf(-yaw, &sin_a, &cos_a);
		cr::sincosf(-pitch, &sin_b, &cos_b);
		cr::sincosf(cr::to_radians(info.angle), &sin_c, &cos_c);

		for(int b = i*4; b < i*4 + 4; b++){
			cr::point vertexpos(cube_vertices[b]._xyz.x * info.w, cube_vertices[b]._xyz.y * info.h);
			zz = cube_vertices[b]._xyz.z * depth;

			temp = (vertexpos.x * cos_a) - (zz * sin_a);
			zz = (zz * cos_a) + (vertexpos.x * sin_a);
			vertexpos.x = temp;
			
			temp = (zz * cos_b) - (vertexpos.y * sin_b);
			vertexpos.y = (vertexpos.y * cos_b) + (zz * sin_b);
			zz = temp;

			vertexpos.rotate(cr::to_radians(info.angle));
			cr::point3d vertexpos3d(-vertexpos.x, -vertexpos.y, zz);

			cr::point3d vertex = objpos + vertexpos3d;
			vertex.z *= riseScale;

			renderer->AddVertex(vertex, 
				cr::point(cube_vertices[b]._uv.u * u, cube_vertices[b]._uv.v * v),
				cr::opaque_white);
		}
	}

	renderer->SetRenderState(cr::rs_zbuffer_enabled, old_zbuffer_state);
}

#else // RUN_ONLY

// Project the mouse cursor from screen space to object space
void EditExt::ProjectScreenToWorld(D3DXVECTOR3* pOut, float screenX, float screenY, float worldZ)
{
	D3DXVECTOR3 lineBegin, lineEnd;

	// Unproject the near and far points given by the screen X,Y coords
	D3DXVECTOR3 screenSpace(screenX, screenY, 0.0f);
	D3DXVec3Unproject(&lineBegin, &screenSpace, &viewport, &projection_matrix, &view_matrix, &worldMatrix);
	screenSpace.z = 1.0f;
	D3DXVec3Unproject(&lineEnd, &screenSpace, &viewport, &projection_matrix, &view_matrix, &worldMatrix);

	// Using a plane intersection, we can determine the object space coordinates of the screen space coords
	// at a certain Z depth, intersecting the line given above.
	orig.z = worldZ;
	D3DXPlaneFromPointNormal(&plane, &orig, &normal);
	D3DXPlaneIntersectLine(pOut, &plane, &lineBegin, &lineEnd);
}

/////////////////////////////
// EDITTIME drawing
// Draw your object in the frame editor.
void EditExt::Draw()
{
	float ox = pInfo->objectX - (pInfo->objectWidth / 2);
	float oy = pInfo->objectY - (pInfo->objectHeight / 2);
	CRect objectRec(ox, oy, ox + pInfo->objectWidth, oy + pInfo->objectHeight);

	float scaledZ = z / (pInfo->pEditTime->GetEyeDistance3d() / 8.0 * riseScale);
	float scaledHeight = height / (pInfo->pEditTime->GetEyeDistance3d() / 8.0 * riseScale);

	pEditTime->EndBatch();

	if (pDevice == NULL)
		D3DInit();

	HRESULT hr;

	// Ensure Z buffer created
	CreateZBuffer();

	// Clear the Z buffer if necessary
	unsigned int curFrame = pEditTime->GetFrameCount();
	unsigned int lastFrame = (unsigned int)pEditTime->GetLayoutKey("zbufferclearframe");

	if (lastFrame < curFrame) {
		hr = pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

		if (FAILED(hr)) MessageBox(NULL, "Failed to clear Z buffer", "3D Box object", MB_OK | MB_ICONHAND);

		pEditTime->SetLayoutKey("zbufferclearframe", (void*)curFrame, false);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);      //Default culling
	pDevice->SetTransform(D3DTS_VIEW, &view_matrix);
	pDevice->SetTransform(D3DTS_PROJECTION, &projection_matrix);
	pDevice->GetViewport(&viewport);
	D3DXMatrixIdentity(&worldMatrix);

	// Enable the Z stencil
	if (zbuffer) {
		hr = pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

		if (FAILED(hr)) MessageBox(NULL, "Failed to enable Z buffer", "3D Box object", MB_OK | MB_ICONHAND);
	}

	if (!drawInit) {

		// Calculate world units per pixel
		D3DXVECTOR3 pt1, pt2;
		ProjectScreenToWorld(&pt1, 0.0f, 0.0f, 0.0f);
		ProjectScreenToWorld(&pt2, 1.0f, 1.0f, 0.0f);
		worldPxX = pt1.x - pt2.x;
		worldPxY = pt1.y - pt2.y;
		if (worldPxX < 0) worldPxX = -worldPxX;
		if (worldPxY < 0) worldPxY = -worldPxY;

		drawInit = true;
	}

	// Calculate scaling matrix
	D3DXMatrixScaling(&scaleMatrix, pInfo->objectWidth * worldPxX, pInfo->objectHeight * worldPxY, scaledHeight * pEditTime->GetZoom());

	// Calculate the X and Y world coords to place the box at.
	D3DXVECTOR3 objectSpace;
	float objX = pInfo->objectX;// + pInfo->objectWidth / 2;
	float objY = pInfo->objectY;// + pInfo->objectHeight / 2;
	pEditTime->TranslateFrameToScreenCoords(objX, objY);
	ProjectScreenToWorld(&objectSpace, objX, objY, 0.0f);

	D3DXMatrixRotationYawPitchRoll(&rotMatrix, 0.0f, 0.0f, RADIANS(pInfo->objectAngle));

	// Multiply the translation, rotation and scaling matrices together to the world matrix
	D3DXMatrixTranslation(&transMatrix, objectSpace.x, objectSpace.y, (scaledZ+scaledHeight/2.0f)*pEditTime->GetZoom());
	D3DXMatrixMultiply(&worldMatrix, &rotMatrix, &transMatrix);
	D3DXMatrixMultiply(&worldMatrix, &scaleMatrix, &worldMatrix);

	// Use our translated, rotated and scaled matrix as the world matrix
	hr = pDevice->SetTransform(D3DTS_WORLD, &worldMatrix);

	if (FAILED(hr)) MessageBox(NULL, "Failed to set transform", "3D Box object", MB_OK | MB_ICONHAND);

	// Use our vertex stream
	hr = pDevice->SetStreamSource(0, pVertices, 0, sizeof(TLVERTEX));

	if (FAILED(hr)) MessageBox(NULL, "Failed to set stream source", "3D Box object", MB_OK | MB_ICONHAND);

	UpdateTextures();
	
	// Draw each face separately
	for (int i = 0; i < 6; i++) {
		pEditTime->SetTexture(textures[i]);
		pEditTime->EndBatch();
		pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, //PrimitiveType
								   i * 6,
								   2);      // Each face is 2 triangles
	}

	// Disable the Z stencil
	hr = pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	if (FAILED(hr)) MessageBox(NULL, "Failed to disable Z buffer", "3D Box object", MB_OK | MB_ICONHAND);

	// Restore 2D mode
	pEditTime->SetTexture(-1);
	pEditTime->EndBatch();

	pEditTime->Restore2DMode();

	// Draw a 2D shadow to show where the box lies in the 2D layout
	float ow = pInfo->objectWidth;
	float oh = pInfo->objectHeight;

	double HotSpotAngle = atan2(oh, ow);
	double HotSpotDist = sqrt(ow*ow + oh*oh) / 2;

	float dx = HotSpotDist * cos(RADIANS(pInfo->objectAngle) + HotSpotAngle);
	float dy = HotSpotDist * sin(RADIANS(pInfo->objectAngle) + HotSpotAngle);

	
	pEditTime->Blitrc(pInfo->objectX - dx, pInfo->objectY - dy, ow, oh, pInfo->objectAngle, 0x40404040);
}

void EditExt::UpdateTextures()
{
	for (int i = 0; i < 6; i++)
		textures[i] = pEditTime->AddImageAsTexture(imageHandles[i]);
}

#endif // RUN_ONLY