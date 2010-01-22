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
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
: renderer(pVRuntime->pRenderer)
{
	pRuntime = pVRuntime;
	info.editObject = editObject;
}

// This is called just after the constructor.  You should perform all loading here.
void ExtObject::OnCreate()
{
	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);
	
	int Version = 0;
	ar >> Version;


	image1 = pRuntime->LoadSerializedImageHandle(ar);
	image2 = pRuntime->LoadSerializedImageHandle(ar);

	info.collMode = COLLISIONMODE_BOX;

	info.w = info.editObject->eWidth;
	info.h = info.editObject->eHeight;
	info.HotSpotX = info.w / 2;
	info.HotSpotY = info.h / 2;
	info.HotSpotAngle = atan(float(info.HotSpotY) / float(info.HotSpotX));
	info.HotSpotDist = sqrt((double)info.HotSpotX * info.HotSpotX + info.HotSpotY * info.HotSpotY);

	info.x = info.editObject->eX + info.HotSpotX;
	info.y = info.editObject->eY + info.HotSpotY;
	info.pInfo->filter = info.editObject->eColor;
	info.angle = info.editObject->eAngle;


	visible = true;

	iSource = renderer->CreateTextureFromHandle(image2);
	iCold = renderer->CreateTextureFromHandle(image1);
	iSurface = renderer->CreateRenderTargetTexture(info.w, info.h);
	iSurface2 = renderer->CreateRenderTargetTexture(info.w, info.h);

	renderer->SetRenderTarget(iSurface);
	renderer->ClearRenderTarget();

	D3DCOLOR cf_old;
	ar >> cf_old;
	coldfilter = cr::color_from_D3DCOLOR<cr_float>(cf_old);

	ar  >> coldx
		>> coldy
		>> coldxspeed
		>> coldyspeed
		>> coldscale
		>> coldangle
		>> xspeed
		>> yspeed;

	for(int i = 0; i < 6; i++)
	{
		ar >> render_filter[i] 
		   >> render_strength[i]
		   >> render_cutoff[i];

		render_filter[i] = COLORREF_CONVERTD3D(render_filter[i]) + 0xff000000;
	}

	ar.detach();

	// Update bounding box
	pRuntime->UpdateBoundingBox(this);

}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
}

// Called every frame for you to update your object if necessary
// Return values: 1 (do not call RTOnFrame again), 0 (continue calling)
// The frame call order goes like this:
// run events -> OnFrame -> Draw -> OnFrame2 -> run events -> ...


typedef enum _D3DCMPFUNC {
    D3DCMP_NEVER                = 1,
    D3DCMP_LESS                 = 2,
    D3DCMP_EQUAL                = 3,
    D3DCMP_LESSEQUAL            = 4,
    D3DCMP_GREATER              = 5,
    D3DCMP_NOTEQUAL             = 6,
    D3DCMP_GREATEREQUAL         = 7,
    D3DCMP_ALWAYS               = 8,
    D3DCMP_FORCE_DWORD          = 0x7fffffff, /* force 32-bit size enum */
} D3DCMPFUNC;



BOOL ExtObject::OnFrame()
{

	return 0;
}

BOOL ExtObject::OnFrame2()
{
	return 1;		// no attached object: don't call again
}
#define D3DCOLORWRITEENABLE_RED     (1L<<0)
#define D3DCOLORWRITEENABLE_GREEN   (1L<<1)
#define D3DCOLORWRITEENABLE_BLUE    (1L<<2)
#define D3DCOLORWRITEENABLE_ALPHA   (1L<<3)


bool IsPowerOfTwo(int value)
{
	return (value & -value) == value;
}
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
	TextureHandle display = renderer->GetRenderTarget();

	float dt = pRuntime->GetTimeDelta();
	float blur = 60 * dt;

	pRuntime->SetScreenTranslation(true);

	renderer->SetRenderState(cr::rs_srcblend , cr::rsv_blend_one);
	renderer->SetRenderState(cr::rs_destblend , cr::rsv_blend_invsrcalpha);

	renderer->SetRenderTarget(iSurface2);
	renderer->ClearRenderTarget();
	
	renderer->SetTexture(iSurface);

	renderer->SetRenderState(cr::rs_srcblend , cr::rsv_blend_one);
	renderer->SetRenderState(cr::rs_destblend , cr::rsv_blend_one);

	float xo = xspeed * dt;
	float yo = yspeed * dt;

	const cr::color quarter_filter(0.25, 1.0, 1.0, 1.0);

	renderer->Quad_xy(-blur + xo, yo,    0.0, cr::origin, quarter_filter);
	renderer->Quad_xy( blur + xo, yo,    0.0, cr::origin, quarter_filter);
	renderer->Quad_xy(xo, blur  + yo,    0.0, cr::origin, quarter_filter);
	renderer->Quad_xy(xo, -blur + yo,    0.0, cr::origin, quarter_filter);

	TextureHandle temp = iSurface;
	iSurface = iSurface2;
	iSurface2 = temp;		//flip the textures

	renderer->SetRenderState(cr::rs_srcblend , cr::rsv_blend_one);
	renderer->SetRenderState(cr::rs_destblend , cr::rsv_blend_invsrcalpha);
	renderer->SetTexture(iSource);
	renderer->Quad_xywh(0, 0, info.w, info.h);


	// Perform the subtraction pattern
	renderer->SetRenderState(cr::rs_srcblend , cr::rsv_blend_one);
	renderer->SetRenderState(cr::rs_destblend ,cr::rsv_blend_invsrcalpha);
	renderer->SetRenderState(cr::rs_blendop, cr::rsv_blendop_revsubtract);

	renderer->SetTexture(iCold);
	coldx += coldxspeed * dt;
	coldy += coldyspeed * dt;

	const cr::color blurred_coldfilter = coldfilter * blur;

	// Render cold map
	{

		
		float iAngle;
		POINTF iScale;
		POINTF iOffset;

		iAngle = coldangle;
		iScale.x = coldscale;
		iScale.y = coldscale;
		iOffset.x = -coldx;
		iOffset.y = -coldy;

		float endx = info.w / iCold->image_widthf;
		float endy = info.h / iCold->image_heightf;
		// Special UV drawing for power-of-two textures
		POINTF uv[4];
		uv[0].x = 0.0f; uv[0].y = 0.0f;
		uv[1].x = endx; uv[1].y = 0.0f;
		uv[2].x = 0.0f; uv[2].y = endy;
		uv[3].x = endx; uv[3].y = endy;

		for(int i = 0; i < 4; i++){
			FPointTranslate(uv[i], -endx/2, -endy/2);
						
			FPointRotate(uv[i], iAngle);
			FPointTranslate(uv[i], iOffset.x * iScale.x / iCold->image_widthf, iOffset.y * iScale.y / iCold->image_heightf);
			FPointScale(uv[i], iScale.x, iScale.y);
			FPointTranslate(uv[i], endx/2, endy/2);
		}

		// Save sampler states
		cr::samplerstate_value oldAddressU = renderer->GetSamplerState(cr::ss_addressu);
		cr::samplerstate_value oldAddressV = renderer->GetSamplerState(cr::ss_addressv);

		// Set wrapping sampler
		renderer->SetSamplerState(cr::ss_addressu, cr::ssv_wrap);
		renderer->SetSamplerState(cr::ss_addressv, cr::ssv_wrap);

		// Draw
		renderer->BeginBatchQuads(4, 6);

		renderer->AddIndex(0);
		renderer->AddIndex(1);
		renderer->AddIndex(2);
		renderer->AddIndex(3);
		renderer->AddIndex(2);
		renderer->AddIndex(1);
		
		renderer->AddVertex(cr::point3d(0,		0,		0), cr::point(uv[0]), blurred_coldfilter);
		renderer->AddVertex(cr::point3d(info.w,	0,		0), cr::point(uv[1]), blurred_coldfilter);
		renderer->AddVertex(cr::point3d(0,		info.h, 0), cr::point(uv[2]), blurred_coldfilter);
		renderer->AddVertex(cr::point3d(info.w,	info.h, 0), cr::point(uv[3]), blurred_coldfilter);

		// Restore sampler states
		renderer->SetSamplerState(cr::ss_addressu, oldAddressU);
		renderer->SetSamplerState(cr::ss_addressv, oldAddressV);
	}




	renderer->SetRenderState(cr::rs_blendop, cr::rsv_blendop_add);
	renderer->SetRenderState(cr::rs_srcblend , cr::rsv_blend_one);
	renderer->SetRenderState(cr::rs_destblend , cr::rsv_blend_invsrcalpha);

	


	info.curTexture = iSurface;
	
	renderer->RestoreRenderTarget();

	pRuntime->SetScreenTranslation(false);

	// Render 6 parses
	for(int i = 0; i < 6; i++)
	{
		renderer->SetTexture(info.curTexture);
		if(render_cutoff[i] != 0){
			renderer->SetRenderTarget(iSurface2);
			renderer->SetRenderState(cr::rs_srcblend , cr::rsv_blend_one);
			renderer->SetRenderState(cr::rs_destblend ,cr::rsv_blend_one);

			renderer->ClearRenderTarget();
			renderer->Quad_xy(0,0);

			// Render the subtract
			renderer->SetRenderState(cr::rs_srcblend , cr::rsv_blend_one);
			renderer->SetRenderState(cr::rs_destblend ,cr::rsv_blend_invsrcalpha);
			renderer->SetRenderState(cr::rs_blendop, cr::rsv_blendop_revsubtract);

			cr::color subtract(render_cutoff[i], 1.0, 1.0, 1.0);

			renderer->SetTexture(NULL);
			renderer->Quad_xywh(0, 0, info.w, info.h, 0.0, cr::origin, subtract);
			
			renderer->SetRenderState(cr::rs_blendop, cr::rsv_blendop_add);
			renderer->SetRenderState(cr::rs_srcblend , cr::rsv_blend_one);
			renderer->SetRenderState(cr::rs_destblend , cr::rsv_blend_invsrcalpha);

			renderer->SetTexture(iSurface2);
			renderer->RestoreRenderTarget();
		}

		for(int j = 0; j < ceil(render_strength[i]); j++)
		{
			cr::color mix(cr::color(render_filter[i]) * info.pInfo->filter);
			mix.a = render_strength[i] - j;

			renderer->Quad_xywh(info.x, info.y, info.w, info.h, info.angle, point(info.HotSpotX, info.HotSpotY), mix);
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
	// Implement saving/loading of your object
	int Version = 1;
	SerializeVersion(ar, Version);

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

	return ret = 0;
}

// Called for undefined expression names, if your extension allows it.
// Eg: MyObject.UndefinedExpression(3,4,5) calls this routine with
// "UndefinedExpression" as the expression name.
long ExtObject::ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
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