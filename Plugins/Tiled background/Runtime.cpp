// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

// ExtObject constructor:
// Called when Construct creates an instance of your object.  Use your editTime data
// to initialise the object.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
: renderer(pVRuntime->pRenderer)
{
	pRuntime = pVRuntime;
	info.editObject = editObject;
}

// This is called just after the constructor, when Construct has added details to your
// object such as siblings and type pointer.  If you need this data on startup, it is
// valid to read here.
void ExtObject::OnCreate()
{
	ImageHandleInfo* imgTexture;
//	double opacity;
//	COLORREF filter;

	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	int Version = 0;
	ar >> Version;

	imgTexture = pRuntime->LoadSerializedImageHandle(ar);
	ar.detach();

	info.curTexture = th = renderer->CreateTextureFromHandle(imgTexture);

	info.w = info.editObject->eWidth;
	info.h = info.editObject->eHeight;

	info.x = info.editObject->eX;
	info.y = info.editObject->eY;

	info.pInfo->filter = info.editObject->eColor;
	info.angle = info.editObject->eAngle;

	info.HotSpotX = 0;
	info.HotSpotY = 0;
	info.angle = 0.0f;
	info.visible = true;

	iAngle = 0;
	iScale.x = 1; 
	iScale.y = 1; 
	iOffset.x = 0;
	iOffset.y = 0;

	info.collMode = COLLISIONMODE_BOX;

	has_file_loaded = false;

	// Update bounding box
	pRuntime->UpdateBoundingBox(this);

	SetupPrivateVariableVector(pRuntime, this, privateVars);
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
	renderer->ReleaseTexture(th);
}

// Called every frame for you to update your object if necessary
// Return values: 1 (do not call RTOnFrame again), 0 (continue calling)
// The frame call order goes like this:
// run events -> OnFrame -> Draw -> OnFrame2 -> run events -> ...
BOOL ExtObject::OnFrame()
{
	return 1;	// Don't call again
}

BOOL ExtObject::OnFrame2()
{
	return 1;
}

bool IsPowerOfTwo(int value)
{
	return (value & -value) == value;
}

typedef enum D3DTEXTUREADDRESS
{
    D3DTADDRESS_WRAP = 1,
    D3DTADDRESS_MIRROR = 2,
    D3DTADDRESS_CLAMP = 3,
    D3DTADDRESS_BORDER = 4,
    D3DTADDRESS_MIRRORONCE = 5,
    D3DTADDRESS_FORCE_DWORD = 0x7fffffff,
} D3DTEXTUREADDRESS, *LPD3DTEXTUREADDRESS;

void FPointTranslate(POINTF& pt, float x, float y){
	pt.x += x;
	pt.y += y;
}

void FPointRotate(POINTF& pt, float angle){
	float oldx = pt.x;
	float oldy = pt.y;
	pt.x = oldx * cos(RADIANS(angle)) + oldy * sin(RADIANS(angle));
	pt.y = oldy * cos(RADIANS(angle)) - oldx * sin(RADIANS(angle));
}

void FPointScale(POINTF& pt, float x, float y){
	pt.x /= x;
	pt.y /= y;
}

// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your extension doesn't draw anything.
void ExtObject::Draw()
{
	if (IsPowerOfTwo(th->image_width) && IsPowerOfTwo(th->image_height)) {

		float endx = info.w / th->image_widthf;
		float endy = info.h / th->image_heightf;

		// Special UV drawing for power-of-two textures
		POINTF uv[4];
		uv[0].x = 0.0f; uv[0].y = 0.0f;
		uv[1].x = endx; uv[1].y = 0.0f;
		uv[2].x = 0.0f; uv[2].y = endy;
		uv[3].x = endx; uv[3].y = endy;

		for(int i = 0; i < 4; i++){
			FPointTranslate(uv[i], -endx/2, -endy/2);
						
			FPointRotate(uv[i], iAngle);
			FPointTranslate(uv[i], iOffset.x * iScale.x / th->image_widthf, iOffset.y * iScale.y / th->image_heightf);
			FPointScale(uv[i], iScale.x, iScale.y);
			FPointTranslate(uv[i], endx/2, endy/2);
		}

		// Save sampler states
		cr::samplerstate_value oldU = renderer->GetSamplerState(cr::ss_addressu);
		cr::samplerstate_value oldV = renderer->GetSamplerState(cr::ss_addressv);

		// Set wrapping sampler
		renderer->SetSamplerState(cr::ss_addressu, cr::ssv_wrap);
		renderer->SetSamplerState(cr::ss_addressv, cr::ssv_wrap);

		// Draw
		renderer->SetTexture(th);

		renderer->BeginBatchQuads(4, 6);

		renderer->AddIndex(0);
		renderer->AddIndex(1);
		renderer->AddIndex(2);
		renderer->AddIndex(2);
		renderer->AddIndex(1);
		renderer->AddIndex(3);

		renderer->AddVertex(cr::point3d(info.x,				info.y,				0.0), uv[0], info.pInfo->filter);
		renderer->AddVertex(cr::point3d(info.x + info.w,	info.y,				0.0), uv[1], info.pInfo->filter);
		renderer->AddVertex(cr::point3d(info.x,				info.y + info.h,	0.0), uv[2], info.pInfo->filter);
		renderer->AddVertex(cr::point3d(info.x + info.w,	info.y + info.h,	0.0), uv[3], info.pInfo->filter);

		// Restore sampler states
		renderer->SetSamplerState(cr::ss_addressu, oldU);
		renderer->SetSamplerState(cr::ss_addressv, oldV);
	}
	else {

		// Seamless tiling with non-power-of-two requires a 1px overlap in SCREEN coordinates
		CRunLayer* pLayer = pRuntime->GetLayer(pLayout, info.layer);
		float totalZoom = pLayout->zoomX * pLayer->zoomXoffset;
		float seamingOffset = 1.0f / totalZoom;	// eg. at 50% zoom, we need to move two layout pixels to reach one screen pixel
		if (seamingOffset < 1.0f) seamingOffset = 1.0f;	// seems to still seam if zoomed in, works nicely zoomed out though

		// Special case: object size is exact: don't have any seaming
		if (info.w == info.curTexture->image_widthf && info.h == info.curTexture->image_heightf)
			seamingOffset = 0;

		float sizexf = th->image_widthf;
		float sizeyf = th->image_heightf;

		// Above code just messes up scrolling
		sizexf -= seamingOffset;
		sizeyf -= seamingOffset;;

		float scrollx = pLayout->scrollX;
		float scrolly = pLayout->scrollY;

		// Meh... these calculations... just make it bigger
		if (pLayout->display_angle != 0.0f) {
			float x = pLayout->zoomedWindowWidth;
			float y = pLayout->zoomedWindowHeight;
			float dist = sqrt(x * x + y * y);
			scrollx -= dist;
			scrolly -= dist;
		}

		int cols = (int)info.w / (int)sizexf;
		int rows = (int)info.h / (int)sizeyf;

		int firstVisCol = (scrollx - info.x) / (sizexf);
		int firstVisRow = (scrolly - info.y) / (sizeyf);

		if (firstVisCol < 0) firstVisCol = 0;
		if (firstVisRow < 0) firstVisRow = 0;

		float gutterx = info.x + (float)(cols * sizexf);
		float guttery = info.y + (float)(rows * sizeyf);

		float gutterw = info.w - (float)(cols * sizexf);
		float gutterh = info.h - (float)(rows * sizeyf);

		cr::rect vertuv;
		vertuv.left = 0.0f;
		vertuv.top = 0.0f;
		vertuv.right = 1.0f;
		vertuv.bottom = float(gutterh) / float(sizeyf);

		cr::rect horizuv;
		horizuv.left = 0.0f;
		horizuv.top = 0.0f;
		horizuv.right = float(gutterw) / float(sizexf);
		horizuv.bottom = 1.0f;
		
		renderer->SetTexture(th);

		int x, y;


		for (x = firstVisCol; x < cols; x++) {
			for (y = firstVisRow; y < rows; y++)
				renderer->Quad_xywh(info.x + x * sizexf, info.y + y * sizeyf,
									sizexf + seamingOffset, sizeyf + seamingOffset,
									0.0, cr::origin, info.pInfo->filter);
		}

		// Draw the vertical gutter
		if (gutterw > 0) {
			for (y = firstVisRow; y < rows; y++)
				renderer->Quad_xywh(gutterx, info.y + y * sizeyf,
									gutterw, sizeyf + seamingOffset,
									0.0, cr::origin, info.pInfo->filter, &horizuv);
		}

		// Draw the horizontal gutter
		if (gutterh > 0) {
			for (x = firstVisCol; x < cols; x++)
				renderer->Quad_xywh(info.x + x * sizexf, guttery,
									sizexf + seamingOffset, gutterh,
									0.0, cr::origin, info.pInfo->filter, &vertuv);
		}

		// Draw the bottom right tile
		if (gutterw > 0 && gutterh > 0) {
			horizuv.bottom = vertuv.bottom;

			renderer->Quad_xywh(gutterx, guttery,
								gutterw, gutterh,
								0.0, cr::origin, info.pInfo->filter, &horizuv);
		}
	}

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

void ExtObject::Serialize(bin& ar)
{
	int Version = 1;
	SerializeVersion(ar, Version);

	if (ar.loading) {
		ar >> info;
	
		ar >> iAngle;
		ar >> iScale.x; 
		ar >> iScale.y;
		ar >> iOffset.x;
		ar >> iOffset.y;

		ar >> privateVars;
	}
	else {
		ar << info;

		ar << iAngle;
		ar << iScale.x; 
		ar << iScale.y;
		ar << iOffset.x;
		ar << iOffset.y;

		ar << privateVars;

	}
}

long ExtObject::GetData(int id, void* param)
{
	switch (id) {
	// If your extension uses a window, you can return it here
	case GET_WINDOW:
		break;
	}

	return 0;
}

long ExtObject::CallFunction(int id, void* param)
{
	return 0;
}

// Return the Default Value.  This is the value to be returned if your object name
// is used without an expression name, eg. in "Counter + 1".
// Parameters can be passed, eg. MyArray(3,7).
long ExtObject::ReturnDefaultValue(LPVAL theParams, ExpReturn& ret)
{
	// ACCESS PRIVATE VALUE
	if (theParams[0].Type() == EXPTYPE_VARIABLENAME) {
		return ret.ReturnCustom(pRuntime, privateVars[theParams[0].GetVariableIndex(pRuntime, pType)]);
	}
	else {
		return ret = 0;
	}
}

// Called for undefined expression names, if your extension allows it.
// Eg: MyObject.UndefinedExpression(3,4,5) calls this routine with
// "UndefinedExpression" as the expression name.
long ExtObject::ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

// Debugging
void ExtObject::DebuggerUpdateDisplay(ExpStore*& pPrivateVars)
{
	if (!privateVars.empty())
		pPrivateVars = &(privateVars[0]); 
}

void ExtObject::OnDebuggerValueChanged(const char* name, const char* value)
{
	// Check if a private variable was modified
	if (CheckDebugVariableChange(name, value, privateVars, this, pRuntime))
		return;
}

#else //ifdef RUN_ONLY

CRunObject* WINAPI RTCreateObject(bin& ar, VRuntime* pRuntime) 
{
	return NULL;
}

void WINAPI RTDestroyObject(ExtObject* object)
{
	//
}

#endif