// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

// ExtObject constructor:
// Only use for class initializer list.  Object initialisation must be done in OnCreate.
// It is not safe to make runtime calls here: do so in OnCreate.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
: renderer(pVRuntime->pRenderer)
{
	pRuntime = pVRuntime;
	info.editObject = editObject;
}

// This is called just after the constructor when your object has been created.  Construct has set
// up your object here so it is safe to make runtime calls.
void ExtObject::OnCreate()
{
	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	int Version = 0;
	ar >> Version;


	ImageHandleInfo* imgTexture = pRuntime->LoadSerializedImageHandle(ar);
	iTexture = renderer->CreateTextureFromHandle(imgTexture);

	COLORREF vertex[4];
	float vertex_opacity[4];

	ar >> vertex[0] >> vertex[1] >> vertex[2] >> vertex[3];
	ar >> vertex_opacity[0] >> vertex_opacity[1] >> vertex_opacity[2] >> vertex_opacity[3];

	for(int i = 0; i < 4; i++)
	{
		vertex_filter[i].r = GetRValue(vertex[i]) / 255.0f;
		vertex_filter[i].g = GetGValue(vertex[i]) / 255.0f;
		vertex_filter[i].b = GetBValue(vertex[i]) / 255.0f;
		vertex_filter[i].a = vertex_opacity[i];
	}

	ar >> image_left_margin	>> image_right_margin >> image_top_margin >> image_bottom_margin;

	int hs;
	ar >> hs;
	hotspot_pos = (hotspot_position)hs;

	ar.detach();

	info.collMode = COLLISIONMODE_BOX;

	info.w = info.editObject->eWidth;
	info.h = info.editObject->eHeight;

	switch (hotspot_pos) {
	case hs_topleft:
		hs_xf = 0.0f;
		hs_yf = 0.0f;
		break;
	case hs_top:
		hs_xf = 0.5f;
		hs_yf = 0.0f;
		break;
	case hs_topright:
		hs_xf = 1.0f;
		hs_yf = 0.0f;
		break;
	case hs_left:
		hs_xf = 0.0f;
		hs_yf = 0.5f;
		break;
	case hs_center:
		hs_xf = 0.5f;
		hs_yf = 0.5f;
		break;
	case hs_right:
		hs_xf = 1.0f;
		hs_yf = 0.5f;
		break;
	case hs_bottomleft:
		hs_xf = 0.0f;
		hs_yf = 1.0f;
		break;
	case hs_bottom:
		hs_xf = 0.5f;
		hs_yf = 1.0f;
		break;
	case hs_bottomright:
		hs_xf = 1.0f;
		hs_yf = 1.0f;
		break;
	}

	info.HotSpotX = info.w * hs_xf;
	info.HotSpotY = info.h * hs_yf;
	oldw = info.w;
	oldh = info.h;

	info.HotSpotAngle = atan(float(info.HotSpotY) / float(info.HotSpotX));
	info.HotSpotDist = sqrt((double)info.HotSpotX * info.HotSpotX + info.HotSpotY * info.HotSpotY);

	info.x = info.editObject->eX;
	info.y = info.editObject->eY;

	info.pInfo->filter = info.editObject->eColor;
	info.angle = info.editObject->eAngle;

	info.visible = true;
	padding = 0;

	// Setup private vars
	SetupPrivateVariableVector(pRuntime, this, privateVars);

	objAttach = NULL;

	// Update bounding box
	pRuntime->UpdateBoundingBox(this);
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
}

// Called every frame, before the events and after drawing, for you to update your object if necessary
// Return 1 (do not call again) or 0 (continue calling)
BOOL ExtObject::OnFrame()
{
	return 1;	// Do not call again
}

// Called every frame, after the events and before drawing, for you to update your object if necessary
// Return 1 (do not call again) or 0 (continue calling)
// It is not safe to destroy objects in OnFrame2().  If you have to do this, use OnFrame().
// If you are storing any pointers to CRunObjects, check the info.destroying flag here.  If it is true,
// you must reset the pointer to NULL as it is no longer valid.
BOOL ExtObject::OnFrame2()
{
	if (objAttach) {
		// Attached object: update box
		info.x = objAttach->info.x;
		info.y = objAttach->info.y;
		info.w = objAttach->info.w + padding;
		info.h = objAttach->info.h + padding;

		if (padding == 0) {
			info.HotSpotX = objAttach->info.HotSpotX;
			info.HotSpotY = objAttach->info.HotSpotY;
			info.HotSpotAngle = objAttach->info.HotSpotAngle;
			info.HotSpotDist = objAttach->info.HotSpotDist;
		}
		else {
			info.HotSpotX = floor(info.w / 2 + 0.5f);
			info.HotSpotY = floor(info.h / 2 + 0.5f);
			info.HotSpotAngle = atan2(info.HotSpotY, info.HotSpotX);
			info.HotSpotDist = sqrt((double)info.HotSpotX * info.HotSpotX + info.HotSpotY * info.HotSpotY);
		}

		info.angle = objAttach->info.angle;

		pRuntime->UpdateBoundingBox(this);

		// Check if destroying
		if (objAttach->info.destroying) {
			objAttach = NULL;
			
			// Destroy me
			pRuntime->DestroyObject(this);

			return 0;
		}
		return 0;
	}

	return 1;
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