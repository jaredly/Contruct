// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY


D3DCOLOR MixFilters(D3DCOLOR f1, D3DCOLOR f2)
{
	// If either of the filters does not affect, return the other filter
	if (f1 == 0xFFFFFFFF) return f2;
	if (f2 == 0xFFFFFFFF) return f1;

	// Else mix the filters
	float afilter = (float((f1 & 0xFF000000) >> 24) / 256.0f) * (float((f2 & 0xFF000000) >> 24) / 256.0f);
	afilter *= 256.0f;
	float rfilter = (float((f1 & 0x00FF0000) >> 16) / 256.0f) * (float((f2 & 0x00FF0000) >> 16) / 256.0f);
	rfilter *= 256.0f;
	float gfilter = (float((f1 & 0x0000FF00) >> 8) / 256.0f) * (float((f2 & 0x0000FF00) >> 8) / 256.0f);
	gfilter *= 256.0f;
	float bfilter = (float(f1 & 0x000000FF) / 256.0f) * (float(f2 & 0x000000FF) / 256.0f);
	bfilter *= 256.0f;

	D3DCOLOR mixedfilter = ((int)afilter << 24) + ((int)rfilter << 16) + ((int)gfilter << 8) + ((int)bfilter);
	return mixedfilter;
}



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
	ar.loading = true;

	ImageHandleInfo* imgHandle = NULL;
	useTexture = false;
	th = NULL;

	int Version = 0;
	ar >> Version;

	pp.Serialize(ar);

	imgHandle = pRuntime->LoadSerializedImageHandle(ar);
	ar >> useTexture;

	if (imgHandle == NULL)
		useTexture = false;
	else
		th = renderer->CreateTextureFromHandle(imgHandle);
	
	ar.detach();

	// Use the texture and collision mask of this image handle
	info.curTexture = 0;
	info.imgHandle = 0;
	info.w = info.editObject->eWidth;
	info.h = info.editObject->eHeight;

	info.x = info.editObject->eX;
	info.y = info.editObject->eY;
	info.pInfo->filter = info.editObject->eColor;
	sprayAngle = RADIANS(info.editObject->eAngle);
	pp.sprayCone = RADIANS(pp.sprayCone);
	info.angle = info.editObject->eAngle;

	col = cr::color(pp.color);
	col_random = cr::color(pp.colorRandom);
	col_fade = cr::color(pp.fadeColor);

	fadeTimef = pp.fadeoutTime;
	fadeColorTimef = pp.fadeColorTime;

	info.isFlipped = 0;
	info.isMirrored = 0;

	// Setup private vars
	SetupPrivateVariableVector(pRuntime, this, privateVars);

	// Update bounding box
	pRuntime->UpdateBoundingBox(this);

	info.pDisplayBoxOverride = &box;

	box.left = info.x;
	box.top = info.y;
	box.right = info.x + 1;
	box.bottom = info.y + 1;

	oldClock = pRuntime->GetLayoutClock(pLayout);
	delayNextParticle = 0;
	on = true;
	oneShotHasFired = false;

	lastX = info.x;
	lastY = info.y;
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
}

cr::color lerp_color(cr::color a, cr::color b, cr_float x)
{
	return a + (b - a) * x;
}

void Particle::Update(ExtObject* o)
{

	gravitySpeedx += o->pp.gravdx * o->timeDelta;
	gravitySpeedy += o->pp.gravdy * o->timeDelta;

	x += cos(angle) * speed * o->timeDelta + gravitySpeedx * o->timeDelta;
	y += sin(angle) * speed * o->timeDelta + gravitySpeedy * o->timeDelta;

	speed += o->pp.acc * o->timeDelta;
	if (speed < 0.0f) speed = 0.0f;

	// Determine opacity
	if (o->pp.fadeoutTime != 0)
		opacity = 1.0f - (o->curClockf - creationTimef) / o->fadeTimef;

	opacity *= o->pp.opacity;

	float angleRandomOffset = o->timeDelta * o->pp.angleRandomiser;
	angle += -(angleRandomOffset / 2.0f) + (o->pRuntime->Random() * angleRandomOffset);

	float speedRandomOffset = o->timeDelta * o->pp.speedRandom;
	speed += -(speedRandomOffset / 2.0f) + (o->pRuntime->Random() * speedRandomOffset);

	float opacityRandomOffset = o->timeDelta * o->pp.opacityRandom;
	opacity += -(opacityRandomOffset / 2.0f) + (o->pRuntime->Random() * opacityRandomOffset);
	if (opacity < 0.0f) {
		opacity = 0.0f;
		expired = true;
	}

	if (o->pp.fadeColor != o->pp.color && o->pp.fadeColorTime != 0) {
		float percent = (o->curClockf - creationTimef) / o->fadeColorTimef;


		if (percent < 0.0f)
			percent = 0.0f;

		if (percent >= 1.0f) 
			c = cr::color(o->pp.fadeColor);

		else
			c = lerp_color(o->col, o->col_fade, percent);
	}

	display_angle += display_angle_speed * o->timeDelta;
	scale += scale_speed * o->timeDelta;

	switch (o->pp.destroyMode) {
	case PDM_LIFETIME:
		if (creationTime + o->pp.lifeTime <= o->curClock)
			expired = true;
		break;
	case PDM_STOPPED:
		if (speed == 0.0f)
			expired = true;
		break;
	case PDM_INVISIBLE:
		if (opacity == 0.0f)
			expired = true;
		break;
	default:
		__assume(0);
	}

	c *= o->info.pInfo->filter;

	float edge = scale + o->pp.particleSize * 0.8;

	if (x - edge < o->box.left)
		o->box.left = x - edge;
	if (x + edge > o->box.right)
		o->box.right = x + edge;
	if (y - edge < o->box.top)
		o->box.top = y  - edge;
	if (y + edge > o->box.bottom)
		o->box.bottom = y + edge;
}


template<class tyA> tyA normaliseBetween(const tyA& min, const tyA& val, const tyA& max)
{
	if( min == max)
		return 0;
	else
		return (val - min) / (max - min);
}

template<class tyA, class tyB> tyA lerp(const tyA& a, const tyA& b, const tyB& r)
{
	return a * (1-r) + b * (r);
}


// Called every frame for you to update your object if necessary
BOOL ExtObject::OnFrame()
{
	sprayAngle = RADIANS(info.angle);

	// Calculate how many particles should be created
	curClock = pRuntime->GetLayoutClock(pLayout);
	curClockf = curClock;
	timeDelta = pRuntime->GetTimeDelta();
	if (timeDelta > 0.5f)
		timeDelta = 0.0f;

	// Update all particles and determine bounding box
	ParticleIterator p = particles.begin();
	ParticleIterator end = particles.end();

	box.left = info.x;
	box.right = info.x+1;
	box.top = info.y;
	box.bottom = info.y + 1;

	for ( ; p != end; p++)
		p->Update(this);

	if (on && pp.rate != 0) {

		if (!pp.oneShot) {

			// Use float rounding for accurate creation
			delayNextParticle -= timeDelta;

			int num = delayNextParticle;


			float oldtimeDelta = timeDelta;

			while(delayNextParticle <= 0) {

				float localTimeDelta = -delayNextParticle;
				float r = normaliseBetween( 0.0f, localTimeDelta, timeDelta );

				//local dt represents how long its been alive. So 1-r represents
				//how far it is from the start position to the end position...so we
				//use info.x, lastX, r as a shortcut instead of lastX, info.x, 1-r

				float oX = lerp(info.x, lastX, r);
				float oY = lerp(info.y, lastY, r);
		
				particles.push_back(Particle());
				Particle& p = particles.back();

				p.c = col;
				p.creationTime = curClock;
				p.creationTimef = curClock;
				p.gravitySpeedx = 0.0f;
				p.gravitySpeedy = 0.0f;
				p.opacity = 1.0f;
				p.speed = pp.speed + (pRuntime->Random() * pp.initialSpeedRandom);
				p.x = oX - (pp.xRandom / 2) + (pRuntime->Random() * pp.xRandom);
				p.y = oY - (pp.yRandom / 2) + (pRuntime->Random() * pp.yRandom);
				p.angle = sprayAngle - (pp.sprayCone / 2) + (pRuntime->Random() * pp.sprayCone);
				p.expired = false;

				p.display_angle = pp.particleDisplayAngle + (pRuntime->Random() * pp.daRandom);
				p.display_angle_speed = pp.daTurn + pp.daTurnRandom * (pRuntime->Random() - 0.5);
				p.scale = pp.sizeRandom * pRuntime->Random();
				p.scale_speed = pp.grow + pp.growRandom * pRuntime->Random();

				// - Advance this particle
				float dt_backup = timeDelta;
				timeDelta = localTimeDelta;
				p.Update(this);
				timeDelta = dt_backup;

				delayNextParticle += 1.0f / pp.rate;
			}

			timeDelta = oldtimeDelta;
		}
	}
	else
		delayNextParticle = 0;

	// Fire the one-shot particles
	if (pp.oneShot && !oneShotHasFired) {
		oneShotHasFired = true;
		int num = floor(pp.rate + 0.5f);

		float oldtimeDelta = timeDelta;

		for (int i = 0; i < num; i++) {
			particles.push_back(Particle());
			Particle& p = particles.back();

			p.c = col;
			p.creationTime = curClock;
			p.creationTimef = curClock;
			p.gravitySpeedx = 0.0f;
			p.gravitySpeedy = 0.0f;
			p.opacity = 1.0f;
			p.speed = pp.speed + (pRuntime->Random() * pp.initialSpeedRandom);
			p.x = info.x - (pp.xRandom / 2) + (pRuntime->Random() * pp.xRandom);
			p.y = info.y - (pp.yRandom / 2) + (pRuntime->Random() * pp.yRandom);
			p.angle = sprayAngle - (pp.sprayCone / 2) + (pRuntime->Random() * pp.sprayCone);
			p.expired = false;

			p.display_angle = pp.particleDisplayAngle + (pRuntime->Random() * pp.daRandom);
			p.display_angle_speed = pp.daTurn + pp.daTurnRandom * (pRuntime->Random() - 0.5);
			p.scale = pp.sizeRandom * pRuntime->Random();
			p.scale_speed = pp.grow + pp.growRandom * pRuntime->Random();

			// This avoids 'gaps' in particle creation
	//		timeDelta = oldtimeDelta * (float)i/(float)num;
	//		p.Update(this);
		}
		timeDelta = oldtimeDelta;
	}

	// Remove expired particles
	p = particles.begin();
	
	for ( ; p != particles.end(); ) {
		if (p->expired)
			p = particles.erase(p);
		else
			p++;

	}

	// Destroy when all particles gone
	if (pp.oneShot && oneShotHasFired && particles.empty())
		pRuntime->DestroyObject(this);

	oldClock = curClock;

	lastX = info.x;
	lastY = info.y;

	return 0;
}

BOOL ExtObject::OnFrame2()
{
	return 1;
}

// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your extension doesn't draw anything.


void ExtObject::Draw()
{
	// Get scale
	CRunLayer* pLayer = pRuntime->GetLayer(pLayout, info.layer);
	float totalZoom = pLayout->zoomX * pLayer->zoomXoffset;

	cr::renderstate_value src = renderer->GetRenderState(cr::rs_srcblend);
	cr::renderstate_value dest = renderer->GetRenderState(cr::rs_destblend);

	if(pp.renderAdditive)
		renderer->SetAdditiveBlending();

	renderWithPoints = false;
	if(pp.particleDisplayAngle == 0 && pp.daRandom == 0 && pp.daTurnRandom == 0 
		&& 	pp.sizeRandom == 0 && 	pp.daTurn == 0 && pp.grow == 0 && pp.growRandom == 0) 
		renderWithPoints = true;


	// Draw particles
	if(renderWithPoints){

		float ptSize = pp.particleSize * totalZoom;

		float oldPtSize = renderer->GetPointSize();
		renderer->SetPointSize(ptSize);

		cr::renderstate_value oldPointSpriteState = renderer->GetRenderState(cr::rs_pointsprite_enabled);

		if (useTexture) {
			renderer->SetRenderState(cr::rs_pointsprite_enabled, cr::rsv_enabled);
			renderer->SetTexture(th);
		}
		else
			renderer->SetTexture(NULL);

		ParticleIterator i = particles.begin();
		ParticleIterator end = particles.end();

		for ( ; i != end; i++) {
			cr::color col = i->c;
			col.a = i->opacity;
			renderer->Point(cr::point(i->x, i->y), col);
		}

		if (useTexture) {
			renderer->SetRenderState(cr::rs_pointsprite_enabled, oldPointSpriteState);
		}

		renderer->SetPointSize(oldPtSize);
	}
	else{
		if (useTexture)
			renderer->SetTexture(th);
		else
			renderer->SetTexture(NULL);

		ParticleIterator i = particles.begin();
		ParticleIterator end = particles.end();

		for ( ; i != end; i++) {
			cr::color col = i->c;
			col.a = i->opacity;

			float size = (i->scale + pp.particleSize) * totalZoom;

			cr::point hotspot(size / 2.0, size / 2.0);

			renderer->Quad_xywh(i->x, i->y, size, size, i->display_angle, hotspot, col);
		}
	}

	renderer->SetRenderState(cr::rs_srcblend, src);
	renderer->SetRenderState(cr::rs_destblend, dest);
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
	}
	else {
		ar << info;
	}
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

#else //ifdef RUN_ONLY

CRunObject* WINAPI RTCreateObject(bin& ar, VRuntime* pRuntime) 
{
	return NULL;
}

void WINAPI RTDestroyObject(ExtObject* object)
{
}

#endif