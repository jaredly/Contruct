
#include "stdafx.h"

// Frame constructor/destructor
CRunLayout::CRunLayout(CRuntime* pRun)
{
	pRuntime = pRun;

	bg_color = cr::opaque_white;
	width = 640;
	height = 480;
	scrollX = 0;
	scrollY = 0;
	display_angle = 0;
	zoomX = 1;
	zoomY = 1;
	frameCounter = 0;
	frameCounter64 = 0;
	hWnd = NULL;
	systemDrawn = false;
	pSwapChain = NULL;
	modal = false;
	selfdestruct = false;
	has_loaded_textures = false;
}

CRunLayout::~CRunLayout()
{
	// Free editdatas
	list<initialObject>::iterator i;

	for (i = initObjects.begin(); i != initObjects.end(); i++) {

		i->Free();

	}
}

void CRunLayout::SetScrollX(double sx)
{
	scrollX = sx;

	if (!unboundedScrolling) {

		float Xscale = (1 + (zoomX-1)) * pRuntime->system.GetMinLayerZoomX();
		float maxZoomedWindowWidth = ((1.0 / Xscale) * (float)framedWidth);

		if (scrollX < (maxZoomedWindowWidth / 2) - (framedWidth / 2))
			scrollX = (maxZoomedWindowWidth / 2) - (framedWidth / 2);
		if (scrollX > width - (maxZoomedWindowWidth / 2) - (framedWidth / 2))
			scrollX = width - (maxZoomedWindowWidth / 2) - (framedWidth / 2);
	}
}

void CRunLayout::SetScrollY(double sy)
{
	scrollY = sy;

	if (!unboundedScrolling) {

		float Yscale = (1 + (zoomY-1)) * pRuntime->system.GetMinLayerZoomY();
		float maxZoomedWindowHeight  = ((1.0 / Yscale) * (float)framedHeight);

		if (scrollY < (maxZoomedWindowHeight / 2) - (framedHeight / 2))
			scrollY = (maxZoomedWindowHeight / 2) - (framedHeight / 2);
		if (scrollY > height - (maxZoomedWindowHeight / 2) - (framedHeight / 2))
			scrollY = height - (maxZoomedWindowHeight / 2) - (framedHeight / 2);
	}
}

void CRunLayout::DestroyNonGlobalObjects()
{
	ObjTypeIterator i = typeReferences.begin();
	ObjTypeConstIterator types_end = typeReferences.end();

	for ( ; i != types_end; i++) {
		
		ObjIterator j = (*i)->instances.begin();
		ObjConstIterator instances_end = (*i)->instances.end();

		ObjList toDestroy;

		for ( ; j != instances_end; j++) {
			if ((*j)->pLayout == this) {
				if ((*j)->info.global)
					(*j)->pLayout = pRuntime->GetSystemFrame();
				else
					pRuntime->DestroyObject(*j);
			}
		}
	}
}

// Prepare this layout for running
void CRunLayout::Load()
{
	// Update back colour
	pRuntime->bkColor = bg_colorref;

	// Set initial clock
	currentClock = 0;
	startClock = pRuntime->GetAppClockf();

	// Load textures if they arent already loaded
	LoadLayoutTextures();
}

void CRunLayout::CreateInitialObjects()
{
	// Load is called before the event list is loaded, hence generating an event in
	// Load won't work.  Creating objects after events means we can generate events
	// in constructors.

	list<initialObject>::iterator c = initObjects.begin();
	const list<initialObject>::const_iterator initObjects_end = initObjects.end();
	ObjList created;

	for ( ; c != initObjects_end; ) {

		if (c->disabled) {
			c++;
			continue;
		}

		// If belongs to a "Destroy on Startup", don't create.
		CRunObjType* pType = pRuntime->objects[c->objectID];

		ObjTypeIterator u = pType->teams.begin();
		bool cont = false;

		for ( ; u != pType->teams.end(); u++) {
			if ((*u)->Name == "Destroy on Startup") {
				cont = true;
				break;
			}
		}
		if (cont) {
			c++;
			continue;
		}
			
		CRunObject* obj = pRuntime->CreateRunObject(&(*c), c->layer, this, false);

		if (obj == NULL) {
			c++;
			continue;
		}

		obj->OnCreate();
		created.push_back(obj);

#ifndef APPRUNTIME
		ImageHandleInfo* ih = (ImageHandleInfo*)obj->info.imgHandle;
		if (ih != NULL)
			obj->info.pInfo->collisionMaskHandle = &ih->cm;
#endif

		// If the object is global, it must be removed from the initial object list: coming back to this
		// frame must not recreate the object.
		if (obj->info.global)
			//c = initObjects.erase(c);		// Still used by create object calls - just disable it
			c->disabled = true;
		else
			c++;

		// Needs a movement to go with me?
		ObjTypeIterator t = obj->pType->sisterTypes.begin();
		ObjTypeConstIterator sisters_end = obj->pType->sisterTypes.end();

		for ( ; t != sisters_end; t++) {

			// A movement for the created object
			if ((*t)->movement && (*t)->pLinkType == obj->pType) {

				// Create a movement instance
				CRunObject* mObj = pRuntime->CreateRunObject((*t)->createParams, obj->info.layer, this, false, obj);
				mObj->OnCreate();
				created.push_back(mObj);
			}
		}
	}

	// List of objects that need OnCreate calling
	ObjList need_OnCreate_called;

	// If there are not enough instances of an object type to fill all container pairs, create extras to suffice
	pRuntime->FlushDelayedObjects();	// Update instance lists
	ObjTypeIterator u = pRuntime->objects.begin();
	ObjTypeIterator objects_end = pRuntime->objects.end();

	for ( ; u != objects_end; ++u) {
		
		// We need this to link movements back to their instances
		ObjList objectsCreatedHere;

		ObjTypeIterator t = (*u)->sisterTypes.begin();
		ObjTypeIterator end = (*u)->sisterTypes.end();

		for ( ; t != end; ++t) {

			if ((*t)->IsFamily()) continue;	// Don't do this for families

			// Not enough instances
			int diff = (*u)->instances.size() - (*t)->instances.size();
			int matchingIndex = (*t)->instances.size();

			for ( ; diff > 0; diff--) {

				// Determine the layer for the new instance - same as first 't' if any exist, otherwise same as first 'u'
				int layer;

				if ((*t)->instances.empty())
					layer = (*u)->instances.front()->info.layer;	// no 't' to get layer from, use 'u'
				else
					layer = (*t)->instances.front()->info.layer;

				// Create instance of 't'
				CRunObject* o = pRuntime->CreateRunObject((*t)->createParams, layer, this, false);

				// CreateRunObject puts the object in 'delayed' by default.  Flush delayed.
				(*t)->FlushDelayed();

				// Don't create here - no pLink set, which would crash behaviors
				//o->OnCreate();
				created.push_back(o);
				need_OnCreate_called.push_back(o);

				o->info.x = (*u)->instances[matchingIndex]->info.x;
				o->info.y = (*u)->instances[matchingIndex]->info.y;
				o->UpdateBoundingBox();
				objectsCreatedHere.push_back(o);

				matchingIndex++;
			}
		}

		// Loop objects we just created to fill out this container - we need to link movements to instances via pLink
		ObjIterator i = objectsCreatedHere.begin();
		ObjIterator objectsCreatedHere_end = objectsCreatedHere.end();

		for ( ; i != objectsCreatedHere_end; i++) {

			if (*i == NULL) continue;

			// Movement
			if ((*i)->pType->movement && (*i)->pLink == NULL) {

				// Find associated type
				ObjIterator j = objectsCreatedHere.begin();
				
				for ( ; j != objectsCreatedHere.end(); j++) {
					if (i == j || *j == NULL) continue;

					// Matches the type we want
					if ((*j)->pType == (*i)->pType->pLinkType) {

						(*i)->pLink = *j;

						// Now remove j once we're done with it.  No easy way to erase here without invalidating
						// iterators, just mark it NULL so we don't come over it again.
						//if ((*j)->siblings.size() == (*j)->pType->sisterTypes.size())
						*j = NULL;

						break;
					}
				}//for
			}

		}//for
	}

	pRuntime->LinkContainersTogether(created);

	// Call oncreate for anything that needs it
	ObjIterator i = need_OnCreate_called.begin();
	ObjIterator end = need_OnCreate_called.end();

	for ( ; i != end; i++)
		(*i)->OnCreate();
}

void CRuntime::LinkContainersTogether(ObjList& created)
{
	FlushDelayedObjects();

	// At this point, instances exist, but container objects are not yet linked on an instance (sibling)
	// level - only on the types (sisters) level.  Find and link all related instances.
	ObjIterator i = created.begin();
	ObjConstIterator created_end = created.end();

	ObjList toLink;

	for ( ; i != created_end; i++) {

		// Contained but not linked
		if ((*i)->siblings.size() < (*i)->pType->sisterTypes.size()) {

			// For each sister type
			ObjTypeIterator t = (*i)->pType->sisterTypes.begin();
			ObjTypeConstIterator sisters_end = (*i)->pType->sisterTypes.end();

			toLink.resize(0);
			toLink.push_back(*i);

			for ( ; t != sisters_end; t++) {

				// Find the first instance of this type that is not matched
				ObjIterator j = created.begin();
				
				for ( ; j != created_end; j++) {
					// Got an unmatched object of this type
					if ((*j)->pType == *t && (*j)->siblings.size() < (*j)->pType->sisterTypes.size()) {
						
						push_back_unique(toLink, *j);

						// Also throw in to the container any existing siblings (ie movements)
						ObjIterator k = (*j)->siblings.begin();

						for ( ; k != (*j)->siblings.end(); k++)
							push_back_unique(toLink, *k);

						break;
					}
				}//for objects
			}//for sistertypes

			// Now link up the instances to be lunk
			ObjIterator o = toLink.begin();
			ObjConstIterator toLink_end = toLink.end();

			for ( ; o != toLink_end; o++) {

				ObjIterator p = toLink.begin();

				for ( ; p != toLink_end; p++) {
					if (*o == *p) continue;

					push_back_unique((*o)->siblings, *p);
				}
			}//linking instances
		}// instance unlinked
	}
}

void CRunLayer::UpdateFilter()
{
	filter = cr::color(opacity / 100.0, colourfilter.r, colourfilter.g, colourfilter.b).premultiplied();
}

// Assign incrementing values for info.z in layer instances
void CRunLayer::ReIndexInstanceZValues()
{
	list<CRunObject*>::iterator i = instances.begin();
	list<CRunObject*>::iterator end = instances.end();
	int index = 0;

	for ( ; i != end; i++, index++)
		(*i)->info.z = index;

	zIndicesSorted = true;
}

#ifdef CONSTRUCT_DIRECTX9

void CRunEffect::InitialiseFrom(CRuntime* pRuntime, CRunEffect* fx)
{
	// Clone the effect
	fx->pEffect->CloneEffect(pRuntime->renderer.GetDevice(), &(this->pEffect));

	GetParameters();

	SetupCustomParameters();
}

void CRunEffect::SetupCustomParameters()
{
	// Obtain all parameter handles and set defaults
	vector<CRunEffectParameter>::iterator i = params.begin();
	const vector<CRunEffectParameter>::const_iterator end = params.end();

	for ( ; i != end; i++) {
		i->handle = pEffect->GetParameterByName(NULL, i->varname);
		
		switch(i->type) {
			// Float
			case 5:
			// Percent
			case 9:
				pEffect->SetFloat(i->handle, i->fValue);
				break;
		}
	}
}

void CRunEffect::GetParameters()
{
	D3DXHANDLE technique = pEffect->GetTechniqueByName("ConstructEffect");
	pEffect->SetTechnique(technique);
	vForegroundTexture = pEffect->GetParameterByName(NULL, "ForegroundTexture");
	vBackgroundTexture = pEffect->GetParameterByName(NULL, "BackgroundTexture");
	vSourceTexture = pEffect->GetParameterByName(NULL, "SourceTexture");
	vPreviousTexture = pEffect->GetParameterByName(NULL, "PreviousTexture");
	vFrameCounter = pEffect->GetParameterByName(NULL, "frameCounter");
	vBoxLeft = pEffect->GetParameterByName(NULL, "boxLeft");
	vBoxTop = pEffect->GetParameterByName(NULL, "boxTop");
	vBoxRight = pEffect->GetParameterByName(NULL, "boxRight");
	vBoxBottom = pEffect->GetParameterByName(NULL, "boxBottom");
	vBoxWidth = pEffect->GetParameterByName(NULL, "boxWidth");
	vBoxHeight = pEffect->GetParameterByName(NULL, "boxHeight");
	vHotspotX = pEffect->GetParameterByName(NULL, "hotspotX");
	vHotspotY = pEffect->GetParameterByName(NULL, "hotspotY");
	vPixelWidth = pEffect->GetParameterByName(NULL, "pixelWidth");
	vPixelHeight = pEffect->GetParameterByName(NULL, "pixelHeight");
	vBgStart = pEffect->GetParameterByName(NULL, "bgStart");
	vBgEnd = pEffect->GetParameterByName(NULL, "bgEnd");
}

bool CRunLayer::HasEffects()
{
	// True if any of the fixed effects are set or if effects active.
#ifdef CONSTRUCT_DIRECTX9
	return (filter != cr::opaque_white || !effects.empty());
#else
	return (filter != cr::opaque_white);
#endif
}

inline void MakeUVFromDimensions(const D3DRECTF& dim, float w, float h, RECTF& uv)
{
	uv.left = dim.x1 / w;
	uv.top = dim.y1 / h;
	uv.right = dim.x2 / w;
	uv.bottom = dim.y2 / h;
}

inline void ClipToWindow(D3DRECT& r, float w, float h)
{
	if(r.x1 > r.x2)
	{
		float x = r.x2;
		r.x2 = r.x1;
		r.x1 = x;
	}
	if(r.y1 > r.y2)
	{
		float y = r.y2;
		r.y2 = r.y1;
		r.y1 = y;
	}

	if (r.x1 < 0) r.x1 = 0;
	if (r.x2 > w) r.x2 = w;
	if (r.y1 < 0) r.y1 = 0;
	if (r.y2 > h) r.y2 = h;
}

inline void ClipToWindow(D3DRECTF& r, float w, float h)
{
	if(r.x1 > r.x2)
	{
		float x = r.x2;
		r.x2 = r.x1;
		r.x1 = x;
	}
	if(r.y1 > r.y2)
	{
		float y = r.y2;
		r.y2 = r.y1;
		r.y1 = y;
	}

	if (r.x1 < 0) r.x1 = 0;
	if (r.x2 > w) r.x2 = w;
	if (r.y1 < 0) r.y1 = 0;
	if (r.y2 > h) r.y2 = h;
}

#ifdef CONSTRUCT_DIRECTX9

// Render a list of effects.
void CRunLayout::RenderEffectChain(EffectList& effects, TextureHandle sourceTexture, TextureHandle destTexture, D3DRECT& clearZone, D3DRECTF& drawZone, RECTF& tbox, float hsX, float hsY)
{
	EffectIterator e = effects.begin();
	EffectConstIterator fx_first = e;
	EffectConstIterator fx_end = effects.end();
	EffectConstIterator fx_last = fx_end - 1;

	Renderer& renderer = pRuntime->renderer;

	// Multisample targets sample from the real texture
	TextureHandle background_texture = destTexture;

	// If multisampling enabled and any of the effects will use the background, we need to update the
	// texture so it's available to the shader (which can't sample a surface).
	// This is not great (since a lot of background blending things will keep copying the multisample thing)
	// but there's no other way provided by D3D9.
	if (pRuntime->multisamples > 0) {

		EffectIterator i = effects.begin();
		EffectIterator end = effects.end();

		for ( ; i != end; ++i) {

			if (i->fx->vBackgroundTexture != NULL) {

				if (background_texture == pRuntime->multisampleTargets[0]) {
					background_texture = pRuntime->tempLayerTarget;

					renderer.CopyMultisampleSurfaceToTexture(pRuntime->multisampleTargets[0], pRuntime->tempLayerTarget, (const RECT&)clearZone);
				}
				else if (background_texture == pRuntime->multisampleTargets[1]) {
					background_texture = pRuntime->tempDisplayTarget;

					renderer.CopyMultisampleSurfaceToTexture(pRuntime->multisampleTargets[1], pRuntime->tempDisplayTarget, (const RECT&)clearZone);
				}

			}

		}
	}

	rect drawZoneRectF = *(rect*)(&drawZone);

	int targetSwitch = 1;

	// Object bounding box in texture coords, clipped.
	rect uv;
	MakeUVFromDimensions(drawZone, winWidth, winHeight, (RECTF&)uv);

	// The next SetTexture source.
	TextureHandle fxSource = sourceTexture;

	// For each effect
	while (e != fx_end) {

		bool nonShaderEffect = (e->fx->majorVer == 0 && e->fx->minorVer == 0);
		TextureHandle nextSource = NULL;

		// Last PS0.0 effect draws to target.
		if (e == fx_last && nonShaderEffect) {
			renderer.SetRenderTarget(destTexture);
			renderer.UpdateTransform(false);
			targetSwitch = !targetSwitch;
		}
		// Else alternating between temp textures.
		else {
			renderer.SetRenderTarget(pRuntime->tempRenderTarget[targetSwitch]);
			renderer.UpdateTransform(false);
			nextSource = pRuntime->tempRenderTarget[targetSwitch];
			targetSwitch = !targetSwitch;	// Toggle render target

			// Clear the render target area in preparation for the draw.
			//Display.EndBatch();
			renderer.ClearRenderTarget(clearZone, cr::transparent_black);
		}

		// Batching doesn't use nPasses yet!!
		unsigned int nPasses = 1;
		ID3DXEffect* curFx = e->fx->pEffect;

		cr::CBatchX_FX_Begin* pBatch = new (&renderer) cr::CBatchX_FX_Begin(&renderer, e->fx);
		cr::EffectParameterBatch& params = pBatch->params;

		// Update parameters
		if (e->fx->vForegroundTexture)	params.foregroundTexture = pRuntime->tempRenderTarget[targetSwitch]->texture_ptr;
		if (e->fx->vBackgroundTexture)	params.backgroundTexture = background_texture->texture_ptr;
		if (e->fx->vSourceTexture)		params.sourceTexture = sourceTexture->texture_ptr;
		if (e->fx->vPreviousTexture)	params.previousTexture = (e == fx_first ? pRuntime->tempDisplayTarget : pRuntime->tempRenderTarget[targetSwitch])->texture_ptr;
		if (e->fx->vFrameCounter)		params.frameCounter = frameCounter;
		if (e->fx->vBoxLeft)			params.boxLeft = tbox.left;
		if (e->fx->vBoxTop)				params.boxTop = tbox.top;
		if (e->fx->vBoxRight)			params.boxRight = tbox.right;
		if (e->fx->vBoxBottom)			params.boxBottom = tbox.bottom;
		if (e->fx->vBoxWidth)			params.boxWidth = tbox.right - tbox.left;
		if (e->fx->vBoxHeight)			params.boxHeight = tbox.bottom - tbox.top;
		if (e->fx->vHotspotX)			params.hotspotX = tbox.left + (float)(hsX) / zoomedWindowWidth;
		if (e->fx->vHotspotY)			params.hotspotY = tbox.top + (float)(hsY) / zoomedWindowHeight;

		if (e->fx->vPixelWidth)			params.pixelWidth = (zoomX) / (float)winWidth;
		if (e->fx->vPixelHeight)		params.pixelHeight = (zoomY) / (float)winWidth;
	

		if (e->fx->vBgStart) {
			// Re-purposed for scrolling
			params.bgStart[0] = scrollX / (float)winWidth;
			params.bgStart[1] = scrollY / (float)winHeight;
		}
		if (e->fx->vBgEnd) {
			params.bgEnd[0] = 1.0f;
			params.bgEnd[1] = 1.0f;
		}

		// Update all parameters
		vector<CRunEffectParameter>::iterator p = e->params.begin();
		vector<CRunEffectParameter>::iterator params_end = e->params.end();

		cr::CustomParamBatch cpb;

		for ( ; p != params_end; p++) {
			switch (p->type) {
				// Float
				case 5:
				// Percent
				case 9:
					cpb.handle = p->handle;
					cpb.value = p->fValue;
					params.customParams.push_back(cpb);
					break;
			}
		}


		// Batch this item
		renderer.AddBatchExtension(pBatch);

		// Run passes
		for (int i = 0; i < nPasses; i++) {

			//curFx->BeginPass(i);
			renderer.AddBatchExtension(new (&renderer) cr::CBatchX_FX_BeginPass(&renderer, curFx, i));

			renderer.SetTexture(fxSource);

			renderer.Quad(drawZoneRectF, 0.0, origin, cr::opaque_white, &uv);

			renderer.AddBatchExtension(new (&renderer) cr::CBatchX_FX_EndPass(&renderer, curFx));

		}//for each pass

		renderer.AddBatchExtension(new (&renderer) cr::CBatchX_FX_End(&renderer, curFx));

		// Last effect used cross sampling - is still on a temp texture
		//if (e == fx_last && e->fx->crossSampling) {
		if (e == fx_last && !nonShaderEffect) {
			renderer.SetRenderTarget(destTexture);
			renderer.UpdateTransform(false);
			renderer.SetTexture(pRuntime->tempRenderTarget[!targetSwitch]);

			renderer.Quad(drawZoneRectF, 0.0, origin, cr::opaque_white, &uv);
		}

		fxSource = nextSource;
		e++;

	}//for each effect	

}

#endif

#endif

// Check if an object bounding box is onscreen
bool CRunLayout::ObjectBoxOnScreen(const RECTF& obox)
{
	//switch (t) {
	//case TRANSLATE_SCROLL:
		return !((obox.right < viewport.left) || (obox.bottom < viewport.top) || (obox.left > viewport.right) || (obox.top > viewport.bottom));
	//case TRANSLATE_SCREEN:
	//	return !(obox.right < 0 || obox.bottom < 0 || obox.left > framedWidth || obox.top > framedHeight);
	//case TRANSLATE_ZERO:
	//default:
	//	return true;
	//}
}

void CRunLayout::CalculateZElevatedBoundingBox(CRunObject* obj) const
{
#ifndef APPRUNTIME
	const RECTF& objBox = obj->info.box;
	RECTF* pDisplayBox = obj->info.pDisplayBoxOverride;

	// 0 = top left of bounding box
	// 1 = bottom right of bounding box
	point3d world3d[2];
	world3d[0] = point3d(objBox.left - scrollX, objBox.top - scrollY, obj->info.pInfo->z_elevation);
	world3d[1] = point3d(objBox.right - scrollX, objBox.bottom - scrollY, obj->info.pInfo->z_elevation);

	point3d projected[2];
	pRuntime->renderer.Project(world3d, projected, 2);

	pDisplayBox->left = projected[0].x + scrollX;
	pDisplayBox->top = projected[0].y + scrollY;
	pDisplayBox->right = projected[1].x + scrollX;
	pDisplayBox->bottom = projected[1].y + scrollY;
#endif
}

const RECTF& CRunLayout::GetObjectBox(CRunObject* obj) const
{
	// If the object has a separate display box to replace the bounding box, use that instead.
	if (obj->info.pDisplayBoxOverride) {

		// If the object is Z elevated, calculate the projected box for it
		if (obj->info.pInfo->z_elevation != 0.0f)
			CalculateZElevatedBoundingBox(obj);

		return *(obj->info.pDisplayBoxOverride);
	}
	else
		return obj->info.box;
}

#ifdef CONSTRUCT_DIRECTX9
// Direct-X version
void CRunLayout::DrawRunObjectOnScreen(CRunObject* q, const RECTF& obox, EffectList* effects)
{		
	Renderer& renderer = pRuntime->renderer;

	//RECTF& obox = GetObjectBox(q);

	// Actual effects to run.
	effectsToRun.resize(0);

	if(effects) {
		effectsToRun = *effects; // copy entire vector		
	}
	else if (!q->effects.empty()) {
		EffectIterator pe = q->effects.begin();
		EffectConstIterator effects_end = q->effects.end();

		// Don't process effects for any disabled effects.  Make a list of the enabled effects to process.
		for ( ; pe != effects_end; pe++) {
			if (pe->enabled)
				effectsToRun.push_back(*pe);
		}
	}

	// If any effects, use the shaders
	if (effectsToRun.empty()) {

		// No effects: just draw directly
		D_ENTER(DCS_PLUGIN, q);

		q->Draw();

		D_RUNTIME();
	}
	// Effects: invoke shaders
	else {

		///////////////////////////
		// RENDER WITH AN EFFECT //
		///////////////////////////
		TextureHandle objectDisplayTarget = renderer.GetRenderTarget();

		// Running single effect optimisation for PS 0.0 effects: skip sourcetexture
		if (effectsToRun.front().fx->renderWithRenderStates && (q->pType->pPlugin->ideFlags & OF_SINGLEDRAWOP) && effectsToRun.size() == 1) 
		{

			// Renderstates only
			InstanceEffect& ie = effectsToRun.front();
			renderer.SetRenderState(cr::rs_srcblend, Renderer::d3d9_to_cr_rsv(cr::rs_srcblend, ie.fx->SrcBlend));
			renderer.SetRenderState(cr::rs_destblend, Renderer::d3d9_to_cr_rsv(cr::rs_destblend, ie.fx->DestBlend));

			D_ENTER(DCS_PLUGIN, q);

			q->Draw();

			D_RUNTIME();

			// Restore alphablending
			renderer.SetAlphaBlending();

			/*
			//Display.EndBatch();
			//renderer.UpdateTransform(false);
			RenderSimpleEffect(effectsToRun.front(), q, objectDisplayTarget);
			//Display.EndBatch();
			//renderer.UpdateTransform(true);
			*/

			
		}
		// Running multiple effects
		else
		{
			// Clear the temp target zone where the object will be drawn.
			D3DRECTF drawZone;
			drawZone.x1 = obox.left;
			drawZone.y1 = obox.top;
			drawZone.x2 = obox.right;
			drawZone.y2 = obox.bottom;

			float unrotatedScrollX = scrollCentreX - zoomedWindowWidth / 2.0;
			float unrotatedScrollY = scrollCentreY - zoomedWindowHeight / 2.0;
			drawZone.x1 -= unrotatedScrollX;
			drawZone.y1 -= unrotatedScrollY;
			drawZone.x2 -= unrotatedScrollX;
			drawZone.y2 -= unrotatedScrollY;

			drawZone.x1 *= renderer.GetXScale();
			drawZone.y1 *= renderer.GetYScale();
			drawZone.x2 *= renderer.GetXScale();
			drawZone.y2 *= renderer.GetYScale();

			// Rotate about the centre of the screen if screen is rotated
			if (curAngle != 0.0f) {
				point rotation_point(framedWidth / 2, framedHeight / 2);

				rect r = *(rect*)&drawZone;
				quad q(r);
				q.rotate(cr::to_radians(curAngle), rotation_point);
				r = q.boundingbox();

				drawZone.x1 = r.left;
				drawZone.y1 = r.top;
				drawZone.x2 = r.right;
				drawZone.y2 = r.bottom;
			}

			// Object bounding box in texture coords
			RECTF tbox;
			MakeUVFromDimensions(drawZone, winWidth, winHeight, tbox);

			// Clip to window area
			ClipToWindow(drawZone, winWidth, winHeight);
			D3DRECTF clearZone = drawZone;

			//if (layerScaled) {
				// For fun make it a little bigger
				clearZone.x1 -= 2;
				clearZone.y1 -= 2;
				clearZone.x2 += 2;
				clearZone.y2 += 2;
				ClipToWindow(clearZone, winWidth, winHeight);
			//}

			// Check for bordering mode: clear an area twice as large, and extend the draw area to
			// 50% as large.  Helps effects like Warp stop clipping to bounding box.
			if (effectsToRun.front().fx->borderMode) {
				int xAdjust = max((drawZone.x2 - drawZone.x1) / 2, 32);
				int yAdjust = max((drawZone.y2 - drawZone.y1) / 2, 32);
				float xUvAdjust = ((float)xAdjust / (float)zoomedWindowWidth) / 2.0f;
				float yUvAdjust = ((float)yAdjust / (float)zoomedWindowHeight) / 2.0f;
				clearZone.x1 -= xAdjust;// * zoomX;
				clearZone.x2 += xAdjust;// * zoomY;
				clearZone.y1 -= yAdjust;// * zoomX;
				clearZone.y2 += yAdjust;// * zoomY;
				drawZone.x1 -= xAdjust / 2;
				drawZone.x2 += xAdjust / 2;
				drawZone.y1 -= yAdjust / 2;
				drawZone.y2 += yAdjust / 2;
				// Clip to window area
				ClipToWindow(clearZone, winWidth, winHeight);
				ClipToWindow(drawZone, winWidth, winHeight);
			}


			// Clear and draw object to the first temp target.
			// This is necessary because we can't assume all objects simply blit a texture, so info.curTexture
			// is not useful.  Also, check if the source texture must be drawn to.
			TextureHandle sourceTextureTarget; 
			if (effectsToRun.front().fx->usesSourceTexture)
				sourceTextureTarget = pRuntime->tempSourceTarget;
			else
				sourceTextureTarget = pRuntime->tempRenderTarget[0];


			D3DRECT clearZoneRect;
			clearZoneRect.x1 = clearZone.x1;		// round down
			clearZoneRect.y1 = clearZone.y1;		// round down
			clearZoneRect.x2 = clearZone.x2 + 0.5f; // round up
			clearZoneRect.y2 = clearZone.y2 + 0.5f; // round up

			renderer.SetRenderTarget(sourceTextureTarget);
			renderer.ClearRenderTarget(clearZoneRect, cr::transparent_black);

			D_ENTER(DCS_PLUGIN, q);

			q->Draw();		// Render object to the temp surface.

			D_RUNTIME(); 

			RECTF tboxrf = tbox;

			renderer.UpdateTransform(false);


			/*
			if (pRuntime->multisamples > 0) {
				EffectIterator e = effectsToRun.begin();
				EffectIterator end = effectsToRun.end();

				for ( ; e != end; ++e) {
					if (e->fx->vBackgroundTexture != NULL) {

						// We need to copy the multisample thing to a texture for the shader to access it
						// Find out which multisample target we're using
						if (objectDisplayTarget == pRuntime->multisampleTargets[0]) {
							// Is a layer texture being drawn
							renderer.CopyMultisampleSurfaceToTexture(pRuntime->multisampleTargets[0], pRuntime->tempLayerTarget, (const RECT&)clearZoneRect);
							break;	// don't do this again!
						}
						else if (objectDisplayTarget == pRuntime->multisampleTargets[1]) {
							// Is a display texture being drawn
							renderer.CopyMultisampleSurfaceToTexture(pRuntime->multisampleTargets[1], pRuntime->tempDisplayTarget, (const RECT&)clearZoneRect);
							break;	// don't do this again!
						}
						//else
						//	throw runtime_error("Unexpected multisampling rendering error!  Please report this bug.");
					}
				}
			}
			*/

			RenderEffectChain(effectsToRun, sourceTextureTarget, objectDisplayTarget, clearZoneRect,
								drawZone, tboxrf, q->info.HotSpotX, q->info.HotSpotY);

			renderer.UpdateTransform(true);

		}

		
	}
}


void CRunLayout::ApplyScrollBounding()
{
	// Adjust allowed width and height for parallax
	float horizExtent = max((float)width - zoomedWindowWidth, 0.0f) * (*y)->scrollXf;
	float vertExtent = max((float)height - zoomedWindowHeight, 0.0f) * (*y)->scrollYf;

	Clamp<double>(scrollX, 0.0f, horizExtent);
	Clamp<double>(scrollY, 0.0f, vertExtent);
}

void CRunLayout::CopyScreenToObject(ScreenCopyingObject& sco)
{
	Renderer& renderer = pRuntime->renderer;
	TextureHandle objectDisplayTarget = renderer.GetRenderTarget();
	TextureHandle curTexture = renderer.GetTexture();

	renderer.SetRenderTarget(sco.rt);
	renderer.SetTexture(objectDisplayTarget);

	rect srcUv = *(rect*)(&(sco.obj->info.box));

	float boxW = srcUv.right - srcUv.left;
	float boxH = srcUv.bottom - srcUv.top;

	pRuntime->TranslateBox(srcUv.left, srcUv.top, srcUv.right, srcUv.bottom);

	srcUv.left /= (float)winWidth;
	srcUv.right /= (float)winWidth;
	srcUv.top /= (float)winHeight;
	srcUv.bottom /= (float)winHeight;

	renderer.Quad(0.0, 0.0, boxW, boxH, 0.0, cr::origin, cr::opaque_white, &srcUv);

	renderer.SetRenderTarget(objectDisplayTarget);
	renderer.SetTexture(curTexture);
}

bool sort_objects_by_z(CRunObject* a, CRunObject* b)
{
	return a->info.pInfo->z_elevation < b->info.pInfo->z_elevation;
}

void CRunLayout::Draw(bool incrementFrameCounter)
{
	////////////////////
	// DRAW THE FRAME //
	////////////////////

	Renderer& renderer = pRuntime->renderer;
	TextureHandle frameRenderTarget = renderer.GetRenderTarget();

	UINT multisamples = pRuntime->multisamples;

	// Clear screen with color
	renderer.ClearRenderTarget(bg_color);

	if (incrementFrameCounter) {
		frameCounter++;
		frameCounter64++;
	}

	framedWidth = min(winWidth, width);
	framedHeight = min(winHeight, height);

	// Caps on zoomed scrolling.
	if (!unboundedScrolling) {
		float minZoomX = pRuntime->system.GetMinLayerZoomX();
		float minZoomY = pRuntime->system.GetMinLayerZoomY();

		float minScrollX = (((float)framedWidth / minZoomX) - (float)framedWidth) / 2;
		float minScrollY = (((float)framedHeight / minZoomY) - (float)framedHeight) / 2;
		LBound(minScrollX, 0.0f);
		LBound(minScrollY, 0.0f);
		float maxScrollX = (float)width - minScrollX - (float)framedWidth;
		float maxScrollY = (float)height - minScrollY - (float)framedHeight;

		Clamp<double>(scrollX, minScrollX, maxScrollX);
		Clamp<double>(scrollY, minScrollY, maxScrollY);
	}

	originalScrollX = scrollX;
	originalScrollY = scrollY;

	y = layers.begin();
	const vector<CRunLayer*>::const_iterator layers_end = layers.end();

	// Iterate all layers
	for (y = layers.begin(); y != layers_end; ++y) {

		// Layer not visible?  moooove on
		if (!(*y)->visible)
			continue;

		// Set appropriate sampler
		switch ((*y)->sampler) {

		// Use application setting
		case CRunLayer::sampler_default:
			// point
			if (pRuntime->props.sampler == 0)
				renderer.SetPointSampling();
			else
				renderer.SetLinearSampling();
			break;
		case CRunLayer::sampler_point:
			renderer.SetPointSampling();
			break;
		case CRunLayer::sampler_linear:
			renderer.SetLinearSampling();
			break;
		}

		// Enable/disable 3D layering (Z buffer/write enable/disable)
		if ((*y)->enable3d) {
			renderer.SetZBufferEnabled(true);
			renderer.SetRenderState(cr::rs_zwrite_enabled, cr::rsv_enabled);
		}
		else {
			renderer.SetZBufferEnabled(false);
			renderer.SetRenderState(cr::rs_zwrite_enabled, cr::rsv_disabled);
		}
	
		// Scrolling
		scrollX = originalScrollX * (*y)->scrollXf + (*y)->scrollXoffset;
		scrollY = originalScrollY * (*y)->scrollYf + (*y)->scrollYoffset;

		// Scaling
		//curXscale = (1 + ((zoomX-1) * (*y)->zoomXf)) * (*y)->zoomXoffset;
		//curYscale = (1 + ((zoomY-1) * (*y)->zoomYf)) * (*y)->zoomYoffset;
		curXscale = zoomX;
		curYscale = zoomY;
		if((*y)->zoomXf != 1)
			curXscale = pow(curXscale, (*y)->zoomXf);
		if((*y)->zoomYf != 1)
			curYscale = pow(curYscale, (*y)->zoomYf);
		curXscale *= (*y)->zoomXoffset;
		curYscale *= (*y)->zoomYoffset;

		(*y)->currentAngle = curAngle = display_angle + (*y)->angle;

		renderer.SetScale(point(curXscale, curYscale));
		renderer.SetAngle(cr::to_radians(curAngle));

		// Now calculate everything that we need
		zoomedWindowWidth = ((1.0 / curXscale) * (float)framedWidth);
		zoomedWindowHeight  = ((1.0 / curYscale) * (float)framedHeight);
		
		if (!unboundedScrolling) {
			if (zoomedWindowWidth > width)
				zoomedWindowWidth = width;
			if (zoomedWindowHeight > height)
				zoomedWindowHeight = height;
		}

		bool layerScaled = multisamples > 0;

		scrollCentreX = scrollX + framedWidth / 2;
		scrollCentreY = scrollY + framedHeight / 2;

		// If the screen is zoomed, adjust scrollX/scrollY to be centralised on the user's set point still
		if (curXscale != 1.0f || curYscale != 1.0f || curAngle != 0.0f) {

			cr::point centrept(scrollCentreX, scrollCentreY);
			cr::point scrollpt(zoomedWindowWidth / -2.0, zoomedWindowHeight / -2.0);

			scrollpt.elliptical_rotate(cr::to_radians(-curAngle), curYscale / curXscale);
			scrollpt += centrept;

			scrollX = scrollpt.x;
			scrollY = scrollpt.y;

			layerScaled = true;
		}

		if (curAngle == 0.0f) {
			// Angle is 0: the scroll point is the viewport beginning point
			viewport.left = scrollX;
			viewport.top = scrollY;
		}
		else {
			// Angle is not 0:  the scroll point is calculated as per necessary for object offsets,
			// but the viewport needs to use an unrotated begin point.  It is adjusted below.
			viewport.left = scrollCentreX - (zoomedWindowWidth / 2.0);
			viewport.top = scrollCentreY - (zoomedWindowHeight / 2.0);
		}

		viewport.right = viewport.left + zoomedWindowWidth;
		viewport.bottom = viewport.top + zoomedWindowHeight;
	/*
		cr::point vp1(scrollX, scrollY);
		cr::point vp2(scrollX, scrollY);
		vp2.move(cr::to_radians(curAngle), zoomedWindowWidth);
		vp2.move(cr::to_radians(curAngle + 90), zoomedWindowHeight);

		viewport.left = min(vp1.x, vp2.x);
		viewport.top = min(vp1.y, vp2.y);
		viewport.right = max(vp1.x, vp2.x);
		viewport.bottom = max(vp1.y, vp2.y);
	*/


		// Limit scrolling
		if (!unboundedScrolling && curAngle == 0.0f) {
			ApplyScrollBounding();
		}

		if (curAngle != 0.0f) {

			// Here, the rotated display is currently using a viewport that is only scaled.
			// Rotate the viewport using a quadrilateral, then recalculate the new viewport as a bounding box
			// of the rotated viewport.
			RECTF oldViewport = viewport;

			float dx = (viewport.left + viewport.right) / 2.0f;
			float dy = (viewport.top + viewport.bottom) / 2.0f;

			quad q(viewport);
			//q.rotate(cr::to_radians(curAngle), dx, dy);
			q.elliptical_rotate(cr::to_radians(curAngle), dx, dy, curYscale / curXscale);
			viewport = q.boundingbox();

			/*
			viewport.left -= dx;
			viewport.right -= dx;
			viewport.top -= dy;
			viewport.bottom -= dy;
			q.RotatedAll(viewport, curAngle);
			q.GetBoundingRect(viewport);
			viewport.left += dx;
			viewport.right += dx;
			viewport.top += dy;
			viewport.bottom += dy;
			*/

			// If rotating the display on a bounded layout and the user tries to rotate in such a way that the
			// outside of the layout would be shown, set the display angle back to 0.
			if (!unboundedScrolling) {

				if (viewport.left < 0 || viewport.top < 0 || viewport.right > width || viewport.bottom > height) {

					viewport = oldViewport;
					(*y)->currentAngle = curAngle = 0.0;
					renderer.SetAngle(0.0);
					scrollX = scrollCentreX - (zoomedWindowWidth / 2.0);
					scrollY = scrollCentreY - (zoomedWindowHeight / 2.0);
					ApplyScrollBounding();

				}
			}//if
		}//if

		if(viewport.left > viewport.right)
		{
			float f = viewport.right;
			viewport.right = viewport.left;
			viewport.left = f;
		}
		if(viewport.top > viewport.bottom)
		{
			float f = viewport.top;
			viewport.top = viewport.bottom;
			viewport.bottom = f;
		}

		// If not a display layer, skip
		if ((*y)->type != 0)
			continue;

		renderer.UpdateTransform();

		//pRuntime->render_layer.Box(viewport, cr::color(1.0, 0.0, 0.0, 1.0));

		bool layerUsingEffects = (*y)->HasEffects() || (*y)->forceOwnTexture;

		// Use a layer texture where necessary
		if (layerUsingEffects) {
			if (pRuntime->tempLayerTarget == unallocated_texture)
				pRuntime->tempLayerTarget = pRuntime->CreateDisplayTargetTexture();

			// Use multisampling target if enabled
			if (multisamples > 0) {

				if (pRuntime->multisampleTargets[0] == unallocated_texture)
					pRuntime->multisampleTargets[0] = renderer.CreateRenderTargetTexture(winWidth, winHeight, cr::texture_format_a8r8g8b8, true);
				
				renderer.SetRenderTarget(pRuntime->multisampleTargets[0]);
			}
			else
				renderer.SetRenderTarget(pRuntime->tempLayerTarget);

			renderer.ClearRenderTarget((*y)->backColor);
			renderer.SetTexture(NULL);
		}

		// When drawing to the screen, don't clear unless a back colour specified
		else if ((*y)->backColor != cr::transparent_black)
			renderer.ClearRenderTarget((*y)->backColor);

		// Draw objects.  If the layer has 3D layering enabled, we need to sort by Z elevation and draw back-to-front
		// Stable sort maintains the relative draw ordering (<3 STL)
		if ((*y)->enable3d)
			stable_sort((*y)->instances.begin(), (*y)->instances.end(), sort_objects_by_z);

		LayerObjConstIterator instances_end = (*y)->instances.end();

		// Optimized loop in case there are no screen copying objects - no need to make checks every iteration
		if ((prerender_screenCopiers.empty() && postrender_screenCopiers.empty()) || multisamples > 0)
		{
			// Loop layer instances and only render
			for (LayerObjReadIterator q = (*y)->instances.begin(); q != instances_end; ++q) {

				// Draw only if in viewport
				const RECTF& obox = GetObjectBox(*q);

				// Object must be on screen and visible
				if ((*q)->info.visible && ObjectBoxOnScreen(obox))
				{
					pRuntime->solIndex = (*q)->info.iid;

					DrawRunObjectOnScreen(*q, obox);

					// View bounding boxes for debugging
					//pRuntime->render_layer.Box(obox, cr::color(1.0, 1.0, 0.0, 0.0));
				}
			}//for each instance
		}
		else {
			// Loop layer instances and render supporting screen copiers
			for (LayerObjReadIterator q = (*y)->instances.begin(); q != instances_end; ++q) {

				// Draw only if in viewport
				const RECTF& obox = GetObjectBox(*q);

				// Object must be on screen and visible
				if (ObjectBoxOnScreen(obox))
				{
					if (!prerender_screenCopiers.empty() && multisamples == 0) {

						vector<ScreenCopyingObject>::iterator sc = prerender_screenCopiers.begin();
						vector<ScreenCopyingObject>::iterator sc_end = prerender_screenCopiers.end();

						for ( ; sc != sc_end; sc++) {

							// Found this object wanting a copy
							if (sc->obj == *q) {

								CopyScreenToObject(*sc);

							}
						}

					}

					if ((*q)->info.visible) {

						DrawRunObjectOnScreen(*q, obox);

					}

					// Check if anyone wants to receive copies of the screen
					if (!postrender_screenCopiers.empty() && multisamples == 0) {

						vector<ScreenCopyingObject>::iterator sc = postrender_screenCopiers.begin();
						vector<ScreenCopyingObject>::iterator sc_end = postrender_screenCopiers.end();

						for ( ; sc != sc_end; sc++) {

							// Found this object wanting a copy
							if (sc->obj == *q) {

								CopyScreenToObject(*sc);
							}
						}
					}//if
				}
			}//for each instance
		}//if

		// If not drawn to backbuffer, blit layer to frame target
		if (layerUsingEffects) {

			renderer.SetZBufferEnabled(false);

			// Copy the multisample target to the layer texture if we've been drawing to the multisampled surface
			if (multisamples > 0)
				renderer.CopyMultisampleSurfaceToTexture(pRuntime->multisampleTargets[0], pRuntime->tempLayerTarget);
					
			EffectList layerEffectsToRun;

			EffectIterator pe = (*y)->effects.begin();
			EffectConstIterator effects_end = (*y)->effects.end();

			// Don't process effects for any disabled effects.  Make a list of the enabled effects to process.
			for ( ; pe != effects_end; pe++) 
			{
				if (pe->enabled)
					layerEffectsToRun.push_back(*pe);
			}


			// Are there effects?
			if (!layerEffectsToRun.empty()) {

				// Layers draw full screen.
				D3DRECT clearZone;
				clearZone.x1 = 0;
				clearZone.y1 = 0;
				clearZone.x2 = framedWidth;
				clearZone.y2 = framedHeight;
				D3DRECTF drawZone;
				drawZone.x1 = 0;
				drawZone.y1 = 0;
				drawZone.x2 = framedWidth;
				drawZone.y2 = framedHeight;
				RECTF tbox;
				tbox.left = tbox.top = 0.0f;
				tbox.right = tbox.bottom = 1.0f;

				// The layer comes from the temp layer target.  If it has a modified filter, however,
				// then it must blit the filter first then process the effect.
				TextureHandle sourceLayerTarget = pRuntime->tempLayerTarget;

				renderer.UpdateTransform(false);

				if ((*y)->filter != cr::opaque_white) {
					renderer.SetTexture(pRuntime->tempLayerTarget);
					renderer.SetRenderTarget(pRuntime->tempRenderTarget[0]);
					renderer.ClearRenderTarget();
					renderer.Quad(0.0, 0.0, 0.0, cr::origin, (*y)->filter);
					sourceLayerTarget = pRuntime->tempRenderTarget[0];
				}

				// Faking these values make it work.  They're updated next iteration anyway.
				zoomedWindowWidth = framedWidth;
				zoomedWindowHeight = framedHeight;

				// Draw with effect chain
				RenderEffectChain(layerEffectsToRun, sourceLayerTarget, frameRenderTarget, clearZone,
									drawZone, tbox, zoomedWindowWidth / 2, zoomedWindowHeight / 2);

				//Display.trans_ScaleX = curXscale;
				//Display.trans_ScaleY = curYscale;
				//renderer.UpdateTransform();
			}
			else if (layerScaled) {
		
				renderer.UpdateTransform(false);

				renderer.SetRenderTarget(frameRenderTarget);
				renderer.SetTexture(pRuntime->tempLayerTarget);

				// If angle set, adjust xy rotational offset
				//if ((*y)->angle == 0.0f) {

				renderer.Quad(0.0, 0.0, 0.0, cr::origin, (*y)->filter);

			}
			else {
			
				renderer.SetRenderTarget(frameRenderTarget);
				renderer.SetTexture(pRuntime->tempLayerTarget);

				// If angle set, adjust xy rotational offset
				//if ((*y)->angle == 0.0f) {

				renderer.Quad(0.0, 0.0, pRuntime->tempLayerTarget->image_widthf / curXscale,
										pRuntime->tempLayerTarget->image_heightf / curYscale,
										0.0, cr::origin, (*y)->filter);

				//}
				//else {
				//	float dx = (*y)->hsDist * cos(Rad((*y)->angle) + (*y)->hsAngle);
				//	float dy = (*y)->hsDist * sin(Rad((*y)->angle) + (*y)->hsAngle);
				//	Display.Blit((winWidth/2)-dx, (winHeight/2)-dy, 1.0f, 1.0f, (*y)->angle, (*y)->filter);
				//}
			}
		}
	}

	//Display.EndBatch();

	scrollX = originalScrollX;
	scrollY = originalScrollY;

/*	curXscale = zoomX;
	curYscale = zoomY;
	zoomedWindowWidth = ((1.0 / curXscale) * (float)winWidth);
	zoomedWindowHeight  = ((1.0 / curYscale) * (float)winHeight);
		
	if (!unboundedScrolling) {
		if (zoomedWindowWidth > width)
			zoomedWindowWidth = width;
		if (zoomedWindowHeight > height)
			zoomedWindowHeight = height;
	}*/


	renderer.SetScale(point(1.0, 1.0));
	renderer.SetAngle(0.0);
	renderer.UpdateTransform(true);
}

#endif // DX version

#ifdef CONSTRUCT_SDL

// SDL version
void CRunLayout::Draw()
{
	RECTF Rect;
	Rect.left = 0;
	Rect.top = 0;
	Rect.right = winWidth;
	Rect.bottom = winHeight;

	// Do background
	pRuntime->Display.Fill(Rect, color);

	frameCounter++;
	frameCounter64++;

	// Caps on zoomed scrolling.
	if (!unboundedScrolling) {
		float minZoomX = pRuntime->system.GetMinLayerZoomX();
		float minZoomY = pRuntime->system.GetMinLayerZoomY();

		float minScrollX = (((float)winWidth / minZoomX) - (float)winWidth) / 2;
		float minScrollY = (((float)winHeight / minZoomY) - (float)winHeight) / 2;
		LBound(minScrollX, 0.0f);
		LBound(minScrollY, 0.0f);
		float maxScrollX = (float)width - minScrollX - (float)winWidth;
		float maxScrollY = (float)height - minScrollY - (float)winHeight;

		Clamp<double>(scrollX, minScrollX, maxScrollX);
		Clamp<double>(scrollY, minScrollY, maxScrollY);
	}

	originalScrollX = scrollX;
	originalScrollY = scrollY;

	vector<CRunLayer*>::iterator y = layers.begin();
	const vector<CRunLayer*>::const_iterator layers_end = layers.end();

	// Iterate all layers
	for (y = layers.begin(); y != layers_end; ++y) {

		// Layer not visible?  moooove on
		if (!(*y)->visible)
			continue;
	
		//Scrolling
		scrollX = originalScrollX * (*y)->scrollXf + (*y)->scrollXoffset;
		scrollY = originalScrollY * (*y)->scrollYf + (*y)->scrollYoffset;

		zoomedWindowWidth = ((1.0 / zoomX / (*y)->zoomXf) * (float)winWidth);
		zoomedWindowHeight  = ((1.0 / zoomY / (*y)->zoomYf) * (float)winHeight);
		
		if (!unboundedScrolling) {
			if (zoomedWindowWidth > width)
				zoomedWindowWidth = width;
			if (zoomedWindowHeight > height)
				zoomedWindowHeight = height;
		}

		bool layerScaled = false;

		// If the screen is zoomed, adjust scrollX/scrollY to be centralised on the user's set point still
		if (curXscale != 1.0f || curYscale != 1.0f || curAngle != 0.0f) {

			// Scroll to central point first
			scrollX += winWidth / 2;
			scrollY += winHeight / 2;

			// Subtract away the zoomed window
			double a = RADIANS(curAngle);

			scrollX -= (cos(a) * (zoomedWindowWidth ) + sin(a) * zoomedWindowHeight) / 2;
			scrollY -= (cos(a) * (zoomedWindowHeight) - sin(a) * zoomedWindowWidth ) / 2;

			layerScaled = true;
		}

		// Limit scrolling
		if (!unboundedScrolling) {

			// Adjust allowed width and height for parallax
			float horizExtent = max((float)width - zoomedWindowWidth, 0.0f) * (*y)->scrollXf;
			float vertExtent = max((float)height - zoomedWindowHeight, 0.0f) * (*y)->scrollYf;

			Clamp<double>(scrollX, 0.0f, horizExtent);
			Clamp<double>(scrollY, 0.0f, vertExtent);
		}

		viewport.left = scrollX;
		viewport.top = scrollY;
		viewport.right = viewport.left + zoomedWindowWidth;
		viewport.bottom = viewport.top + zoomedWindowHeight;

		if(curAngle != 0.0)
		{
			double a = RADIANS(curAngle);

			double x = (winWidth - cos(a)*zoomedWindowWidth - sin(a)*zoomedWindowHeight)/2;
			double y = (winHeight - cos(a)*zoomedWindowHeight + sin(a)*zoomedWindowWidth)/2;

			viewport.left -= x;
			viewport.right -= x;
			viewport.top -= y;
			viewport.bottom -= y;

			int dist = zoomedWindowWidth*0.75;
			viewport.left -= dist;
			viewport.top -= dist;
			viewport.right += dist;
			viewport.bottom += dist;
		}

		if(viewport.left > viewport.right)
		{
			float f = viewport.right;
			viewport.right = viewport.left;
			viewport.left = f;
		}

		if(viewport.top > viewport.bottom)
		{
			float f = viewport.top;
			viewport.top = viewport.bottom;
			viewport.bottom = f;
		}

		// If not a display layer, skip
		if ((*y)->type != 0)
			continue;

		LayerObjConstIterator instances_end = (*y)->instances.end();

		// Loop layer instances and render
		for (LayerObjReadIterator q = (*y)->instances.begin(); q != instances_end; ++q) 
		{
			// Draw only if in viewport
			RECTF& obox = GetObjectBox(*q);

			// Object must be on screen and visible
			if (ObjectBoxOnScreen(obox))
			{
				if ((*q)->info.visible) 
				{	
					D_ENTER(DCS_PLUGIN, *q);
					(*q)->Draw();
					D_RUNTIME();
				}
			}
		}
	}

	SDL_UpdateRect(pRuntime->Display.screen, 0, 0, width, height);

	scrollX = originalScrollX;
	scrollY = originalScrollY;
}
void CRunLayout::DrawRunObjectOnScreen(CRunObject* q, const RECTF& obox, EffectList* effects)
{	

}
#endif
