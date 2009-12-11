// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"


TLVERTEX g_cube_vertices[] = {
	  //Front face
   { 0.5f,-0.5f, 0.5f,0xFFFFFFFF,0.0f,1.0f},{ 0.5f, 0.5f, 0.5f,0xFFFFFFFF,0.0f,0.0f},{-0.5f, 0.5f, 0.5f,0xFFFFFFFF,1.0f,0.0f},
   {-0.5f, 0.5f, 0.5f,0xFFFFFFFF,1.0f,0.0f},{-0.5f,-0.5f, 0.5f,0xFFFFFFFF,1.0f,1.0f},{ 0.5f,-0.5f, 0.5f,0xFFFFFFFF,0.0f,1.0f},
      //Back face
   {-0.5f,-0.5f, -0.5f,0xFFFFFFFF,0.0f,1.0f},{-0.5f, 0.5f, -0.5f,0xFFFFFFFF,0.0f,0.0f},{ 0.5f, 0.5f, -0.5f,0xFFFFFFFF,1.0f,0.0f},
   { 0.5f, 0.5f, -0.5f,0xFFFFFFFF,1.0f,0.0f},{ 0.5f,-0.5f, -0.5f,0xFFFFFFFF,1.0f,1.0f},{-0.5f,-0.5f, -0.5f,0xFFFFFFFF,0.0f,1.0f},
	  //Top face
   {-0.5f, 0.5f, -0.5f,0xFFFFFFFF,0.0f,1.0f},{-0.5f, 0.5f, 0.5f,0xFFFFFFFF,0.0f,0.0f},{ 0.5f, 0.5f, 0.5f,0xFFFFFFFF,1.0f,0.0f},
   { 0.5f, 0.5f, 0.5f,0xFFFFFFFF,1.0f,0.0f},{ 0.5f, 0.5f, -0.5f,0xFFFFFFFF,1.0f,1.0f},{-0.5f, 0.5f, -0.5f,0xFFFFFFFF,0.0f,1.0f},
      //Bottom face
   { 0.5f,-0.5f, -0.5f,0xFFFFFFFF,0.0f,1.0f},{ 0.5f,-0.5f, 0.5f,0xFFFFFFFF,0.0f,0.0f},{-0.5f,-0.5f, 0.5f,0xFFFFFFFF,1.0f,0.0f},
   {-0.5f,-0.5f, 0.5f,0xFFFFFFFF,1.0f,0.0f},{-0.5f,-0.5f, -0.5f,0xFFFFFFFF,1.0f,1.0f},{ 0.5f,-0.5f, -0.5f,0xFFFFFFFF,0.0f,1.0f},
      //Left face
   { 0.5f,-0.5f, -0.5f,0xFFFFFFFF,1.0f,1.0f},{ 0.5f, 0.5f, -0.5f,0xFFFFFFFF,0.0f,1.0f},{ 0.5f, 0.5f, 0.5f,0xFFFFFFFF,0.0f,0.0f},
   { 0.5f, 0.5f, 0.5f,0xFFFFFFFF,0.0f,0.0f},{ 0.5f,-0.5f, 0.5f,0xFFFFFFFF,1.0f,0.0f},{ 0.5f,-0.5f, -0.5f,0xFFFFFFFF,1.0f,1.0f},
      //Right face
   {-0.5f,-0.5f, 0.5f,0xFFFFFFFF,0.0f,0.0f},{-0.5f, 0.5f, 0.5f,0xFFFFFFFF,1.0f,0.0f},{-0.5f, 0.5f, -0.5f,0xFFFFFFFF,1.0f,1.0f},
   {-0.5f, 0.5f, -0.5f,0xFFFFFFFF,1.0f,1.0f},{-0.5f,-0.5f, -0.5f,0xFFFFFFFF,0.0f,1.0f},{-0.5f,-0.5f,0.5f,0xFFFFFFFF,0.0f,0.0f},
};


xyzuv cube_vertices[] = {
	  //Front face
	{{ 0.5f,-0.5f, 0.5f},{0.0f,1.0f}},{{0.5f, 0.5f, 0.5f},{0.0f,0.0f}},
	{{-0.5f, 0.5f, 0.5f},{1.0f,0.0f}},{{-0.5f,-0.5f, 0.5f},{1.0f,1.0f}},
      //Back face
	{{-0.5f,-0.5f, -0.5f},{0.0f,1.0f}},{{-0.5f, 0.5f, -0.5f},{0.0f,0.0f}},
	{{ 0.5f, 0.5f, -0.5f},{1.0f,0.0f}},{{ 0.5f,-0.5f, -0.5f},{1.0f,1.0f}},
	  //Top face
	{{-0.5f, 0.5f, -0.5f},{0.0f,1.0f}},{{-0.5f, 0.5f, 0.5f},{0.0f,0.0f}},
	{{ 0.5f, 0.5f, 0.5f},{1.0f,0.0f}},{{ 0.5f, 0.5f, -0.5f},{1.0f,1.0f}},
      //Bottom face
	{{ 0.5f,-0.5f, -0.5f},{0.0f,1.0f}},{{ 0.5f,-0.5f, 0.5f},{0.0f,0.0f}},
	{{-0.5f,-0.5f, 0.5f},{1.0f,0.0f}},{{-0.5f,-0.5f, -0.5f},{1.0f,1.0f}},
      //Left face
	{{ 0.5f,-0.5f, -0.5f},{1.0f,1.0f}},{{ 0.5f, 0.5f, -0.5f},{0.0f,1.0f}},
	{{ 0.5f, 0.5f, 0.5f},{0.0f,0.0f}},{{ 0.5f,-0.5f, 0.5f},{1.0f,0.0f}},
      //Right face
	{{-0.5f,-0.5f, 0.5f},{0.0f,0.0f}},{{-0.5f, 0.5f, 0.5f},{1.0f,0.0f}},
	{{-0.5f, 0.5f, -0.5f},{1.0f,1.0f}},{{-0.5f,-0.5f, -0.5f},{0.0f,1.0f}},
};

int cube_indexes[] = {
	1,2,3,  3,4,1,
	5,6,7,  7,8,5,
	9,10,11,  11,12,9,
	13,14,15,  15,16,13,
	17,18,19,  19,20,17,
	21,22,23,  23,24,21
};


//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

// ExtObject constructor:
// Only use for class initializer list.  Object initialisation must be done in OnCreate.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
: renderer(pVRuntime->pRenderer)
{
	pRuntime = pVRuntime;
	info.editObject = editObject;
	drawInit = false;
}

// Update vertices to reflect new object state
/*
void ExtObject::UpdateVertices()
{
	void* vertexData;
	pVertices->Lock(0, 0, &vertexData, 0);

	if (FAILED(hr))
		pRuntime->Throw("3D Cube object: Failed to lock vertex buffer");

	float imgU[6];
	float imgV[6];

	for (int i = 0; i < 6; i++) {
		SIZE imgArea;
		SIZE texArea;
		pRuntime->GetTextureSize(textures[i], &imgArea, &texArea);
		imgU[i] = (float)imgArea.cx / (float)texArea.cx;
		imgV[i] = (float)imgArea.cy / (float)texArea.cy;
	}

	// Copy the source cube vertices to the 
	TLVERTEX* vxSrc = g_cube_vertices;
	TLVERTEX* vxDest = currentVertices;
	TLVERTEX* end = vxSrc + NUMCUBEVERTICES;
	int index = 0;

	for ( ; vxSrc != end; vxSrc++, vxDest++, index++) {

		*vxDest = *vxSrc;

		// Update the UV coords to draw only this texture's image area
		if (vxDest->u == 1.0f)
			vxDest->u = imgU[index / 6];
		if (vxDest->v == 1.0f)
			vxDest->v = imgV[index / 6];
	}

	memcpy(vertexData, currentVertices, sizeof(currentVertices));

	pVertices->Unlock();
}
*/

// This is called just after the constructor.  You should perform all loading here.
void ExtObject::OnCreate()
{
	// Load the edittime data that was serialized.
	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	int Version = 0;
	ar >> Version;

	ar >> depth >> z >> info.angle;

	for (int i = 0; i < 6; i++) {
		ImageHandleInfo* imgHandle = pRuntime->LoadSerializedImageHandle(ar);
		textures[i] = renderer->CreateTextureFromHandle(imgHandle);

		//if (i == BACK_FACE)
			//info.collisionMaskHandle = imgHandle;
	}

	zbuffer = true;

	if (Version >= 2)
		ar >> zbuffer;

	// Finished reading data
	ar.detach();

	// Set default dimensions
	info.x = info.editObject->eX;
	info.y = info.editObject->eY;
	info.w = info.editObject->eWidth;
	info.h = info.editObject->eHeight;
	//info.x += info.w / 2;
	//info.y += info.h / 2;
	
	info.HotSpotX = info.w / 2;
	info.HotSpotY = info.h / 2;
	info.HotSpotAngle = atan2(info.HotSpotY, info.HotSpotX);
	info.HotSpotDist = sqrt(info.HotSpotY*info.HotSpotY + info.HotSpotX*info.HotSpotX);

	info.collMode = COLLISIONMODE_FINE;

	oldW = info.w;
	oldH = info.h;
	oldAngle = info.angle + 1; // force it to update when it sees angle is not oldangle

	yaw = pitch = 0.0f;

	// Point the custom display bounding box
	info.pDisplayBoxOverride = &displayBox;
	displayBox.left = 0;
	displayBox.top = 0;
	displayBox.right = pLayout->width;
	displayBox.bottom = pLayout->height;

	oldDetails.SetFrom(this);

	// Setup private vars
	SetupPrivateVariableVector(pRuntime, this, privateVars);

	// Update bounding box
	pRuntime->UpdateBoundingBox(this);
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
	for (int i = 0; i < 6; i++)
		renderer->ReleaseTexture(textures[i]);
}

// Called every frame, after the events and before drawing, for you to update your object if necessary
// Return 1 (do not call again) or 0 (continue calling)
BOOL ExtObject::OnFrame()
{
	return 1;	// Do not call again.
}

/*
POINTF ExtObject::ProjectWorldToScreen(float x, float y, float z)
{
	POINTF p;
	D3DXVECTOR3 screenSpace;
	D3DXVECTOR3 objSpace(x, y, z);
	D3DXVec3Project(&screenSpace, &objSpace, &viewport, &projection_matrix, &view_matrix, &worldMatrix);
	p.x = screenSpace.x;
	p.y = screenSpace.y;
	return p;
}
*/

void ObjectDetails::SetFrom(ExtObject* obj) {
	x = obj->info.x;
	y = obj->info.y;
	w = obj->info.w;
	h = obj->info.h;
	angle = obj->info.angle;
	scrollX = obj->pLayout->scrollX;
	scrollY = obj->pLayout->scrollY;
	yaw = obj->yaw;
	pitch = obj->pitch;
}

// Called every frame, before the events and after drawing, for you to update your object if necessary
// Return 1 (do not call again) or 0 (continue calling)
BOOL ExtObject::OnFrame2()
{
	float scrollX = pLayout->scrollX;
	float scrollY = pLayout->scrollY;

	Clamp(scrollX, 0.0f, (float)pLayout->width - (float)pLayout->winWidth);
	Clamp(scrollY, 0.0f, (float)pLayout->height - (float)pLayout->winHeight);

	// Update the display box only when any of the object properties changes
	ObjectDetails currentDetails;
	currentDetails.SetFrom(this);
	currentDetails.scrollX = scrollX;
	currentDetails.scrollY = scrollY;

	if (currentDetails != oldDetails) {

		oldDetails = currentDetails;

		/*
		D3DXMatrixIdentity(&worldMatrix);

		// Calculate the X and Y world coords
		ProjectScreenToWorld(&objectSpace, (info.x - scrollX) * pLayout->zoomX, (info.y - scrollY) * pLayout->zoomY, 0.0f);

		// Multiply the translation, rotation and scaling matrices together to the world matrix
		D3DXMatrixTranslation(&transMatrix, objectSpace.x, objectSpace.y, z);
		D3DXMatrixMultiply(&worldMatrix, &rotMatrix, &transMatrix);
		D3DXMatrixMultiply(&worldMatrix, &scaleMatrix, &worldMatrix);

		// Update the display bounding box
		float x = objectSpace.x;
		float y = objectSpace.y;
		*/
		cr::point3d objpos(info.x - scrollX, info.y - scrollY, z);
		cr::point sizefactor(info.w, info.h);

		cr::point3d vertices[8];
		cr::point3d unprojected[8];

		// Calculate the screen position of each of the cube's eight vertices
		vertices[0] = objpos + (sizefactor * cr::point(-0.5f, -0.5f)).make_rotated(cr::to_radians(info.angle)).make_3d(0.0);
		vertices[1] = objpos + (sizefactor * cr::point(-0.5f,  0.5f)).make_rotated(cr::to_radians(info.angle)).make_3d(0.0);
		vertices[2] = objpos + (sizefactor * cr::point( 0.5f, -0.5f)).make_rotated(cr::to_radians(info.angle)).make_3d(0.0);
		vertices[3] = objpos + (sizefactor * cr::point( 0.5f,  0.5f)).make_rotated(cr::to_radians(info.angle)).make_3d(0.0);
		vertices[4] = objpos + (sizefactor * cr::point(-0.5f, -0.5f)).make_rotated(cr::to_radians(info.angle)).make_3d(depth);
		vertices[5] = objpos + (sizefactor * cr::point(-0.5f,  0.5f)).make_rotated(cr::to_radians(info.angle)).make_3d(depth);
		vertices[6] = objpos + (sizefactor * cr::point( 0.5f, -0.5f)).make_rotated(cr::to_radians(info.angle)).make_3d(depth);
		vertices[7] = objpos + (sizefactor * cr::point( 0.5f,  0.5f)).make_rotated(cr::to_radians(info.angle)).make_3d(depth);

		renderer->Project(vertices, unprojected, 8);

		/*
		pts[0] = ProjectWorldToScreen(-0.5f, -0.5f, 0);
		pts[1] = ProjectWorldToScreen(0.5f, -0.5f, 0);
		pts[2] = ProjectWorldToScreen(-0.5f, 0.5f, 0);
		pts[3] = ProjectWorldToScreen(0.5f, 0.5f, 0);
		pts[4] = ProjectWorldToScreen(-0.5f, -0.5f, 1);
		pts[5] = ProjectWorldToScreen(0.5f, -0.5f, 1);
		pts[6] = ProjectWorldToScreen(-0.5f, 0.5f, 1);
		pts[7] = ProjectWorldToScreen(0.5f, 0.5f, 1);
		*/

		float maxX = unprojected[0].x;
		float minX = unprojected[0].x;
		float maxY = unprojected[0].y;
		float minY = unprojected[0].y;

		for (int i = 1; i < 8; i++) {
			if (unprojected[i].x > maxX)
				maxX = unprojected[i].x;
			if (unprojected[i].x < minX)
				minX = unprojected[i].x;
			if (unprojected[i].y > maxY)
				maxY = unprojected[i].y;
			if (unprojected[i].y < minY)
				minY = unprojected[i].y;
		}

		// Form bounding box
		displayBox.left = minX + scrollX;
		displayBox.top = minY + scrollY;
		displayBox.right = maxX + scrollX;
		displayBox.bottom = maxY + scrollY;
	}

	return 0;	// Call again
}

// WindowProc:  called when a window message, or WM_COMMAND specifying your window,
// is sent to Construct.  You must call RegisterWindow() before Construct will send
// you messages for the given window.
LRESULT ExtObject::WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

BOOL ExtObject::PreTranslateMessage(MSG* msg)
{
	return 0;
}

// For global objects
void ExtObject::OnFrameChange(int oldFrame, int newFrame)
{
	// Do anything your global object needs when the frame changes
	// oldFrame is -1 on start of app, newFrame is -1 on end of app
}

// User defined functions
long ExtObject::GetData(int id, void* param)
{
	return 0;
}

long ExtObject::CallFunction(int id, void* param)
{
	return 0;
}

#else //ifdef RUN_ONLY

CRunObject* WINAPI RTCreateObject(bin& ar, VRuntime* pRuntime) 
{
	return NULL;
}

void WINAPI RTDestroyObject(ExtObject* object)
{
}

#endif