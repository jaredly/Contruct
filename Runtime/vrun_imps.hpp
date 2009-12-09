
// VIRTUAL RUNTIME IMPLEMENTATIONS


// Game runtime versions
#ifndef APPRUNTIME

	bool deprecated_TextureExists(Texture iTexture)
	{		return false;	}

	int deprecated_AddTextureFromResource(HMODULE hModule, LPCTSTR pResource)
	{
		return -2;
	}

	int deprecated_AddTextureFromFile(const char* filename)
	{
		return -2;
	}

	int deprecated_AddTextureFromFileInMemory(LPCVOID pData, UINT len)
	{
		return -2;
	}

	int deprecated_AddTextureRT(int w, int h)
	{	return -2;	}

	void deprecated_ResizeTextureRT(int* tex, int w, int h)
	{	}

	void deprecated_ClearRT(D3DCOLOR color)
	{	}

	void deprecated_DeleteTexture(int handle)
	{	}

	void deprecated_SetTexture(int index)
	{	}

	void deprecated_SetRenderTarget(int index)
	{	}

	int deprecated_GetRenderTarget()
	{	return -2;	}

	void deprecated_RestoreRenderTarget()
	{	}

	void deprecated_GetTextureSize(int tex, SIZE* imgSize, SIZE* texSize)
	{	}

	void deprecated_Blit(float x, float y, float xf = 1.0f, float yf = 1.0f, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, RECTF* uv = NULL)
	{
	}

	void deprecated_Blitrc(float x, float y, float w, float h, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, RECTF* uv = NULL)
	{
	}

	void deprecated_Blit(float x, float y, float xf, float yf, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4,  RECTF* uv = NULL)
	{
	}

	void deprecated_Blitrc(float x, float y, float w, float h, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, RECTF* uv = NULL)
	{
	}

	void deprecated_BlitrcSkew(float x, float y, float w, float h, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, RECTF* uv = NULL, float skewX = 0.0f, float skewY = 0.0f)
	{
	}

	void deprecated_SetRenderState(D3DRENDERSTATETYPE rstype, UINT value)
	{	}

	void deprecated_SetTransform(D3DTRANSFORMSTATETYPE state, D3DXMATRIX * pMatrix)
	{	}

	unsigned long deprecated_GetRenderState(D3DRENDERSTATETYPE rstype)
	{	return 0; }

	void deprecated_SetSamplerState(D3DSAMPLERSTATETYPE sstype, UINT value)
	{	}

	unsigned long deprecated_GetSamplerState(D3DSAMPLERSTATETYPE sstype)
	{	return 0; }

	void deprecated_SetGlobalFilter(D3DCOLOR color)
	{	}

	D3DCOLOR deprecated_GetGlobalFilter()
	{	return 0xFFFFFFFF;	}	// not implemented

	int deprecated_CreateFont(const char* fontface, unsigned int pxSize, bool italics, bool bold)
	{
		return -1;
	}

	void deprecated_DrawText(int iFont, const char* text, RECTF& rc, D3DCOLOR color, DWORD format)
	{
	}

	void deprecated_Fill(const RECTF& rc, D3DCOLOR color)
	{
	}

	void deprecated_FillGradient(const RECTF& rc, D3DCOLOR c1, D3DCOLOR c2, Direction dir)
	{
	}

	void deprecated_Point(float x, float y, D3DCOLOR color)
	{
	}

	void deprecated_Line(float x1, float y1, float x2, float y2, D3DCOLOR color)
	{
	}

	void deprecated_Box(const RECTF& rc, D3DCOLOR color)
	{
	}

	void deprecated_BeginBatch(CD3DBATCH type, int num)
	{
	}

	void deprecated_EndBatch()
	{	}

	void deprecated_DestroyFont(int iFont)
	{
	}

	int deprecated_AddTextureFromHandle(int handle)
	{
		return -2;
	}

	POINT GetImageCenterPoint(BankImage handle)
	{	
		ImageHandleInfo* ih = (ImageHandleInfo*)handle;

		if (ih == NULL) {
			POINT nothing = {0, 0};
			return nothing;
		}

		return ih->hotspot;
	}

	bool GetImagePoint(CRunObject* obj, int number, POINTF& pt)
	{	
		if (obj == NULL) return false;

		ImageHandleInfo* ih = (ImageHandleInfo*)obj->info.imgHandle;

		if (ih == NULL)
			return false;
		else {

			if (number < 0 || number >= ih->points.size())
				return false;
			else {
				ImageHandleInfo& ihi = *ih;
				float x = ihi.points[number].x;
				float y = ihi.points[number].y;

				float xScale = obj->info.w / ihi.cm.width;
				float yScale = obj->info.h / ihi.cm.height;
				float angle = RADIANS(obj->info.displayangle);

				float skewX = obj->info.pInfo->skewX;
				float skewY = obj->info.pInfo->skewY;

				if (skewX != 0.0f || skewY != 0.0f) {
					
					// For skewX, the image point offset dx = (skewX * dy) / h
					x += (skewX * (obj->info.h - y)) / obj->info.h;
					y += (skewY * x) / obj->info.w;
				}

				if (xScale != 1.0f || yScale != 1.0f || angle != 0.0f) {
					x -= obj->info.HotSpotX / xScale;
					y -= obj->info.HotSpotY / yScale;
					x *= xScale;
					y *= yScale;
					RotatePoint(x, y, angle);
				}
				else {
					x -= obj->info.HotSpotX;
					y -= obj->info.HotSpotY;
				}

				pt.x = obj->info.x + x;
				pt.y = obj->info.y + y;
				return true;
			}
		}
	}

	// NOTE: someone should make an inbetween step function, one that you simply pass a 
	// CRunObject and an ImageMap* because its silly having 2 functions that have to maintained
	// with the calculations...really the 2 functions below should work out the map ur after and then call a third
	// function...
	bool GetImagePoint(CRunObject* obj, const char* pointName, POINTF& pt)
	{	
		if (obj == NULL) return false;

		ImageHandleInfo* ih = (ImageHandleInfo*)obj->info.imgHandle;

		if (ih == NULL)
			return false;
		else {

			vector<ImagePoint>::iterator m = find(ih->points.begin(), ih->points.end(), pointName);

			if (m == ih->points.end())
				return false;
			else {
				ImageHandleInfo& ihi = *ih;
				float x = m->x;
				float y = m->y;

				float xScale = obj->info.w / ihi.cm.width;
				float yScale = obj->info.h / ihi.cm.height;
				float angle = RADIANS(obj->info.displayangle);

				if (xScale != 1.0f || yScale != 1.0f || angle != 0.0f) {
					x -= obj->info.HotSpotX / xScale;
					y -= obj->info.HotSpotY / yScale;
					x *= xScale;
					y *= yScale;
					RotatePoint(x, y, angle);
				}
				else {
					x -= obj->info.HotSpotX;
					y -= obj->info.HotSpotY;
				}

				pt.x = obj->info.x + x;
				pt.y = obj->info.y + y;
				return true;
			}
		}
	}

	void* deprecated_GetDisplayDevice() const
	{
		return NULL;
	}

	void deprecated_ReplaceTexture(Texture oldTexture, Texture newTexture)
	{
	}

	void deprecated_Restore2DMode()
	{
	}

	void deprecated_Setup2DMode()
	{
	}

	void deprecated_AddVertex(D3DCOLOR color, float x, float y, float u, float v)
	{
	}

	void deprecated_AddVertex3d(D3DCOLOR color, float x, float y, float z, float u, float v)
	{
	}

	void deprecated_AddIndex(unsigned int i)
	{
	}

	void deprecated_ReleaseTexture(Texture tex)
	{
	}

	void SetScreenTranslation(bool enable)
	{
		if (enable) {
			translationMode = TRANSLATE_SCREEN;
			renderer.UpdateTransform(false);
		}
		else {
			translationMode = TRANSLATE_SCROLL;
			renderer.UpdateTransform(true);
		}
	}

	void deprecated_SmoothLineBegin()
	{	}

	void deprecated_SmoothLine(float x1, float y1, float x2, float y2, D3DCOLOR color)
	{	}

	void deprecated_SmoothLineEnd()
	{	}

	CRunAnimation* GetPresetAnimation(CRunAnimation* pRoot, int tagID);

// Application runtime versions: do nothing
#else

	void deprecated_ReleaseTexture(Texture rex)
	{				}

	bool deprecated_TextureExists(Texture iTexture)
	{
		return false;
	}

	int deprecated_AddTextureFromResource(HMODULE hModule, LPCTSTR pResource)
	{	return -1;	}

	int deprecated_AddTextureFromFile(const char* filename)
	{	return -1;	}

	int deprecated_AddTextureFromFileInMemory(LPCVOID pData, UINT len)
	{	return -1;	}

	int deprecated_AddTextureRT(int w, int h)
	{	return -1;	}

	void deprecated_ResizeTextureRT(int* tex, int w, int h)
	{}

	void deprecated_ClearRT(D3DCOLOR color)
	{}

	void deprecated_DeleteTexture(int handle)
	{}

	void deprecated_SetTexture(int index)
	{}

	void deprecated_SetRenderTarget(int index)
	{}

	Texture deprecated_GetRenderTarget()
	{return 0;}

	void deprecated_RestoreRenderTarget()
	{}

	void deprecated_GetTextureSize(int tex, SIZE* imgSize, SIZE* texSize)
	{}

	void deprecated_Blit(float x, float y, float xf = 1.0f, float yf = 1.0f, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, RECTF* uv = NULL)
	{}

	void deprecated_Blitrc(float x, float y, float w, float h, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, RECTF* uv = NULL)
	{}

	void deprecated_Blit(float x, float y, float xf, float yf, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4,  RECTF* uv = NULL)
	{
	}

	void deprecated_Blitrc(float x, float y, float w, float h, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, RECTF* uv = NULL)
	{
	}

	void deprecated_SetRenderState(D3DRENDERSTATETYPE rstype, UINT value)
	{}

	unsigned long deprecated_GetRenderState(D3DRENDERSTATETYPE rstype)
	{ return 0; }

	void deprecated_SetSamplerState(D3DSAMPLERSTATETYPE sstype, UINT value)
	{}

	DWORD deprecated_GetSamplerState(D3DSAMPLERSTATETYPE sstype)
	{	return 0;	}

	void deprecated_SetGlobalFilter(D3DCOLOR color)
	{}

	D3DCOLOR deprecated_GetGlobalFilter()
	{ return 0; }

	int deprecated_CreateFont(const char* fontface, unsigned int pxSize, bool italics, bool bold)
	{	return -1;	}

	void deprecated_DrawText(int iFont, const char* text, RECTF& rc, D3DCOLOR color, DWORD format)
	{}

	void deprecated_Fill(const RECTF& rc, D3DCOLOR color)
	{}

	void deprecated_FillGradient(const RECTF& rc, D3DCOLOR c1, D3DCOLOR c2, Direction dir)
	{}

	void deprecated_Point(float x, float y, D3DCOLOR color)
	{}

	void deprecated_Line(float x1, float y1, float x2, float y2, D3DCOLOR color)
	{}

	void deprecated_Box(const RECTF& rc, D3DCOLOR color)
	{}

	void deprecated_BeginBatch(CD3DBATCH type, int num)
	{}

	void deprecated_EndBatch()
	{}

	void deprecated_DestroyFont(int iFont)
	{}

	int deprecated_AddTextureFromHandle(int handle)
	{	return -1;	}

	void* deprecated_GetDisplayDevice() const
	{	return NULL;	}

	POINT	GetImageCenterPoint(BankImage handle)
	{ POINT p; p.x = 0; p.y = 0; return p; }

	bool GetImagePoint(CRunObject* obj, const char* pointName, POINTF& pt)
	{	return false;	}

	bool GetImagePoint(CRunObject* obj, int pointNum, POINTF& pt)
	{	return false;	}

	void deprecated_ReplaceTexture(Texture oldTexture, Texture newTexture)
	{
	}

	void deprecated_Restore2DMode()
	{}

	void deprecated_BlitrcSkew(float x, float y, float w, float h, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, RECTF* uv = NULL, float skewX = 0.0f, float skewY = 0.0f)
	{
	}

	void deprecated_AddVertex(D3DCOLOR color, float x, float y, float u, float v)
	{
	}

	void deprecated_AddIndex(unsigned int i)
	{
	}

	void deprecated_AddVertex3d(D3DCOLOR color, float x, float y, float z, float u, float v)
	{
	}

	void SetTransform(int, int)
	{
	}

	void SetScreenTranslation(bool enable)
	{
	}

	void deprecated_Setup2DMode() {}

	void	deprecated_SmoothLineBegin() {}
	void	deprecated_SmoothLine(float x1, float y1, float x2, float y2, D3DCOLOR color) {}
	void	deprecated_SmoothLineEnd() {}
	CRunAnimation* GetPresetAnimation(CRunAnimation* pRoot, int tagID)
	{	return NULL;	}

#endif

	HWND GetAppHwnd() const
	{
		return hWnds.front();
	}

	HINSTANCE GetAppHInstance() const
	{
		return hInstance;
	}

	CRunObject* CreateObject(CRunObjType* pType, int layer, CRunLayout* pLayout)
	{
		if (pType == NULL || layer < 0 || pLayout == NULL)
			return NULL;

		// Family passed: pick a random sub-type
		if (pType->IsFamily())
			pType = pType->teams[(unsigned int)(Random() * (double)pType->teams.size())];
		
		if (pType->createParams == NULL)
			return NULL;

		return CreateRunObject(pType->createParams, layer, pLayout);
	}

	void DestroyObject(CRunObject* pObject)
	{
		if (pObject == NULL) return;

		// Add to death row
		if (pObject->siblings.empty()) {
			if (!(pObject->info.destroying)) {
				DeathRow.push_back(pObject);
				pObject->info.destroying = true;
			}
		}
		else {

			if (!(pObject->info.destroying)) {
				DeathRow.push_back(pObject);
				pObject->info.destroying = true;

				// Mark all siblings as destroying
				ObjIterator i = pObject->siblings.begin();
				ObjConstIterator sibs_end = pObject->siblings.end();

				for ( ; i != sibs_end; i++)
					(*i)->info.destroying = true;
			}
		}
	}

	// UNIMPLEMENTED
	long CallEx(int id, long param1, long param2)
	{
		return 0;
	}

	void GetCurrentEventPtrs(EventParametersInfo2& epi) const
	{
		pCurrentEvent->GetEventInfo((EventParametersInfo&)epi);
	}

	CRunObjType* GetTypeFromOID(int oid) const {

		if (oid < 0 || oid >= objects.size())
			return NULL;

		return objects[oid];
	}

	int GetZIndex(CRunObject* obj){
		if (obj == NULL) 
			return NULL;

		CRunLayer* pLayer = obj->pLayout->layers[obj->info.layer];

		if(!pLayer->zIndicesSorted)
			pLayer->ReIndexInstanceZValues();

		return obj->info.z;
	}

	int GetNumLayers(CRunLayout* pLayout) const
	{
		return pLayout->layers.size();
	}

	void ToLayer(CRunObject* obj, int layer)
	{
		if (obj == NULL) return;
		vector<CRunLayer*>& layers = obj->pLayout->layers;

		if (layer < 0 || layer >= obj->pLayout->layers.size()) return;

		// If not already on this layer
		if (obj->info.layer != layer) {
			LayerObjList& LayerInstancesOld = layers[obj->info.layer]->instances;
			LayerObjList& LayerInstancesNew = layers[layer]->instances;
			LayerObjIterator& rIter = *(obj->info.pLayerIterator);

			// First delete from old layer
			LayerInstancesOld.erase(rIter);

			// Add to back (foremost) of new layer and update iterator
			LayerInstancesNew.push_back(obj);
			rIter = LayerInstancesNew.end();
			rIter--;

			// Old layer z indices
			obj->pLayout->layers[obj->info.layer]->zIndicesSorted = false;

			// Update layer value
			obj->info.layer = layer;

			// New layer z indices
			obj->pLayout->layers[obj->info.layer]->zIndicesSorted = false;
		}
	}

	void ToFront(CRunObject* obj)
	{
		if (obj == NULL) return;

		// Get reference to the object's layer and the object's layer iterator
		LayerObjList& LayerInstances = obj->pLayout->layers[obj->info.layer]->instances;
		LayerObjIterator& rIter = *(obj->info.pLayerIterator);

		// Delete the layer entry, push it to the back (last drawn appears foremost), then update iterator
		LayerInstances.erase(rIter);
		LayerInstances.push_back(obj);
		rIter = LayerInstances.end();
		rIter--;
		obj->pLayout->layers[obj->info.layer]->zIndicesSorted = false;
	}

	void ToBack(CRunObject* obj)
	{
		if (obj == NULL) return;

		// Get reference to the object's layer and the object's layer iterator
		LayerObjList& LayerInstances = obj->pLayout->layers[obj->info.layer]->instances;
		LayerObjIterator& rIter = *(obj->info.pLayerIterator);

		// Delete the layer entry, push it to the front (first drawn appears backmost), then update iterator
		LayerInstances.erase(rIter);
		LayerInstances.push_front(obj);
		rIter = LayerInstances.begin();

		obj->pLayout->layers[obj->info.layer]->zIndicesSorted = false;
	}

	void InFront(CRunObject* obj, CRunObject* other)
	{
		if (obj == NULL || other == NULL) return;
		if (obj->pLayout != other->pLayout) return;

		vector<CRunLayer*>& layers = obj->pLayout->layers;

		// Firstly if obj is on a different layer to other, move obj to other's layer
		if (obj->info.layer != other->info.layer)
			ToLayer(obj, other->info.layer);

		// Get references
		LayerObjList& LayerInstances = layers[other->info.layer]->instances;
		LayerObjIterator& ObjIter = *(obj->info.pLayerIterator);
		LayerObjIterator& OtherIter = *(other->info.pLayerIterator);

		// First erase obj
		LayerInstances.erase(ObjIter);

		// Now move to 1 in front of the other's iterator and store iterator back
		LayerObjIterator pos = OtherIter;
		pos++;
		ObjIter = LayerInstances.insert(pos, obj);

		obj->pLayout->layers[obj->info.layer]->zIndicesSorted = false;
	}

	void ToBehind(CRunObject* obj, CRunObject* other)
	{
		if (obj == NULL || other == NULL) return;
		if (obj->pLayout != other->pLayout) return;	// Cannot move between frames (yet)

		vector<CRunLayer*>& layers = obj->pLayout->layers;

		// Firstly if obj is on a different layer to other, move obj to other's layer
		if (obj->info.layer != other->info.layer)
			ToLayer(obj, other->info.layer);

		// Get references
		LayerObjList& LayerInstances = layers[other->info.layer]->instances;
		LayerObjIterator& ObjIter = *(obj->info.pLayerIterator);
		LayerObjIterator& OtherIter = *(other->info.pLayerIterator);

		// First erase obj
		LayerInstances.erase(ObjIter);

		// Now move to other's iterator and store iterator back
		ObjIter = LayerInstances.insert(OtherIter, obj);

		obj->pLayout->layers[obj->info.layer]->zIndicesSorted = false;
	}

	CEventLine2* GetCurrentEventPtr() const
	{
		return (CEventLine2*)pCurrentEvent;
	}

	// Workaround for VC6 ABI/heap issues with CString
	void ReturnStringWorkaround(const char* str) {
		tempStrings[curtempstr] = str;
		curtempstr++;
		curtempstr %= 32;
	}

	// General workaround in case of further heap issues with VC6
	void AssignCString(void* mfc_cstr, const char* str)
	{
		*((CString*)mfc_cstr) = str;
	}

	// Vector-avoiding read-only virtual functions for non ABI compatible compilers
	void GetObjectSiblings(CRunObject* obj, CRunObject**& siblingList, int& count) const {
		if (obj != NULL) {
			count = obj->siblings.size();

			if (count > 0)
				siblingList = &(obj->siblings[0]);
			else
				siblingList = NULL;
		}
		else {
			count = 0;
			siblingList = NULL;
		}
	}
	void GetLayers(CRunLayout* pLayout, CRunLayer**& layerList, int& count) const {
		if (pLayout == NULL) {
			count = 0;
			layerList = NULL;
			return;
		}
		count = pLayout->layers.size();
		layerList = &(pLayout->layers[0]);
	}
	void GetTypes(CRunObjType**& typesList, int& count) {
		count = objects.size();
		typesList = &(objects[0]);
	}
	void GetTypeInstances(CRunObjType* pType, CRunObject**& instancesList, int& count) const {
		if (pType != NULL) {
			count = pType->instances.size();

			if (count > 0)
				instancesList = &(pType->instances[0]);
			else
				instancesList = NULL;
		}
		else {
			count = 0;
			instancesList = NULL;
		}
	}
	void GetTypeSelectedInstances(CRunObjType* pType, CRunObject**& instancesList, int& count) const {
		if (pType != NULL) {
			if (pType->pSol->selectAll)
				GetTypeInstances(pType, instancesList, count);
			else {
				count = pType->pSol->sol.size();

				if (count > 0)
					instancesList = &(pType->pSol->sol[0]);
				else
					instancesList = NULL;
			}
		}
		else {
			count = 0;
			instancesList = NULL;
		}
	}

	CRunObject* GetFirstInstance(CRunObjType* pType) const
	{
		if (pType == NULL) return NULL;

		if (pType->pSol->selectAll) {
			if (pType->instances.size() > 0)
				return pType->instances[0];
			else
				return NULL;
		}
		else {
			if (pType->pSol->sol.size() > 0)
				return pType->pSol->sol[0];
			else
				return NULL;
		}
	}

	int GetRuntimeType() const
	{

#ifdef CONSTRUCT_DIRECTX9
		return DIRECTX9_RUNTIME;
#endif
#ifdef APPRUNTIME
		return APPLICATION_RUNTIME;
#endif
#ifdef CONSTRUCT_SDL
//		return SDL_RUNTIME;
		return DIRECTX9_RUNTIME;
#endif
	}

	void UpdateBoundingBox(CRunObject* obj);

	bool ObjectOffscreen(CRunObject* obj) const
	{
		if (obj == NULL)
			return false;

		RECTF screen;
		screen.left = obj->pLayout->scrollX;
		screen.top = obj->pLayout->scrollY;
		screen.right = screen.left + props.winWidth;
		screen.bottom = screen.top + props.winHeight;

		RECTF& o = obj->info.box;

		if (o.left > screen.right || o.right < screen.left || o.top > screen.bottom || o.bottom < screen.top)
			return true;
		return false;
	}

	bool ObjectOutsideFrame(CRunObject* obj) const
	{
		if (obj == NULL)
			return false;

		int frameW = obj->pLayout->width;
		int frameH = obj->pLayout->height;

		RECTF& o = obj->info.box;

		if (o.right < 0 || o.bottom < 0 || o.left > frameW || o.top > frameH)
			return true;
		return false;
	}

	// Return random [0, 1)
	double Random() {
		return mtRand53();
	}

	void GetMousePos(int& x, int& y) {
		POINT mouse;
		GetCursorPos(&mouse);

		if (!fullscreen)
			ScreenToClient(hWnds.front(), &mouse);

		x = mouse.x;
		y = mouse.y;
	}

	const SimpleInputState* GetInputStatePtr()
	{
		return &inputState;
	}

	CRunObjType* GetTraitFromName(const char* name) 
	{
		if (name == NULL)
			return NULL;

		CString traitName = name;
		traitName.MakeLower();

		// Loop and find the trait with the name given
		ObjTypeIterator t = objects.begin();
		ObjTypeConstIterator objects_end = objects.end();

		for ( ; t != objects_end; t++) {

			CString tName = (*t)->Name;
			tName.MakeLower();

			if (tName == traitName && (*t)->teamID > -1)
				return (*t);
		}

		// Not found/does not exist
		return NULL;
	}

	void Throw(const char* errorMsg)
	{
		throw runtime_error(errorMsg);
	}

	void* GetCurrentConditionPtr() const {
		return (void*)(&(**(pCurrentEvent->pCurCnd)));
	}

	bool ConditionIsNegated() const
	{
		return (*(pCurrentEvent->pCurCnd))->isNegated != 0;
	}

	void CallOnFrame(CRunObject* obj) {
		if (obj == NULL) return;

		if (callingOnFrames) {
			if (!exists_element(calleeBuffer.begin(), calleeBuffer.end(), obj) && !exists_element(onFrameCallees.begin(), onFrameCallees.end(), obj))
				calleeBuffer.push_back(obj);
		}
		else {
			if (find(onFrameCallees.begin(), onFrameCallees.end(), obj) == onFrameCallees.end())
				onFrameCallees.push_back(obj);
		}
	}

	void CallOnFrame2(CRunObject* obj) {
		if (obj == NULL) return;

		if (callingOnFrame2s) {
			if (!exists_element(calleeBuffer.begin(), calleeBuffer.end(), obj) && !exists_element(onFrame2Callees.begin(), onFrame2Callees.end(), obj))
				calleeBuffer.push_back(obj);
		}
		else {
			if (find(onFrame2Callees.begin(), onFrame2Callees.end(), obj) == onFrame2Callees.end())
				onFrame2Callees.push_back(obj);
		}
	}

	CRunObject* GetPairedObject(CRunObjType* pType, CRunObject* obj) const
	{
		if (pType == NULL || obj == NULL) return NULL;
		
		SelObjList& rSol = *(pType->pSol);

		ObjList* ol;
		if (rSol.selectAll)
			ol = &(pType->instances);
		else
			ol = &(rSol.sol);

		if (ol->empty())
			return NULL;	// No object available
		else
			// Return paired instance (used to use obj->info.idd)
			return ol->operator [](solIndex % ol->size());
	}

	void SetCopyFrameContent(CRunObject* obj, TextureHandle rt, bool postrender, bool bSet)
	{
		if (obj == NULL || rt < 0) return;

		ScreenCopyingObject sco;
		sco.obj = obj;
		sco.rt = rt;

		vector<ScreenCopyingObject>* sc;

		if (postrender)
			sc = &(obj->pLayout->postrender_screenCopiers);
		else
			sc = &(obj->pLayout->prerender_screenCopiers);

		vector<ScreenCopyingObject>::iterator i = find(sc->begin(), sc->end(), sco);

		if (bSet) {

			if (i == sc->end())
				sc->push_back(sco);

		}
		// Remove this from obj's frame
		else {

			if (i != sc->end())
				sc->erase(i);
		}
	}

	HWND GetFrameHwnd(CRunLayout* pLayout) {
		if (pLayout == NULL)
			return NULL;

		if (pLayout->hWnd == NULL)
			return GetAppHwnd();
		else
			return pLayout->hWnd;
	}

	SIZE GetWindowSize() const {
		SIZE s;
		s.cx = winWidth;
		s.cy = winHeight;
		return s;
	}

	int	TranslatePrivateVariableIndex(CRunObjType* pCallerType, CRunObjType* pOriginalType, int index) const
	{
		// Calling from ExpBase guarantees pCallerType != pOriginalType
		// Hence pOriginalType is the family type, and needs to map the private variable to the relevant
		// index for pCallerType.
		int ret = pOriginalType->teamPvMap[pCallerType][index];
		return ret;
	}

	// Debugger funcs
	void AddDebuggerItem(const char* label, const char* value, bool readOnly);
	void AddDebuggerCombo(const char* label, const char* items, int sel);

	void AddDebuggerItem(const char* label, int value, bool readOnly)
	{
		char strbuf[50];
		_itoa(value, strbuf, 10);
		AddDebuggerItem(label, strbuf, readOnly);
	}
	void AddDebuggerItem(const char* label, __int64 value, bool readOnly)
	{
		char strbuf[50];
		_i64toa(value, strbuf, 10);
		AddDebuggerItem(label, strbuf, readOnly);
	}
	void AddDebuggerItem(const char* label, float value, bool readOnly)
	{
		char strbuf[50];
		_gcvt(value, 6, strbuf);
		int len = strlen(strbuf) - 1;
		if (len >= 0 && strbuf[len] == '.')
			strbuf[len] = '\0';
		AddDebuggerItem(label, strbuf, readOnly);
	}
	void AddDebuggerItem(const char* label, double value, bool readOnly)
	{
		char strbuf[50];
		_gcvt(value, 6, strbuf);
		int len = strlen(strbuf) - 1;
		if (len >= 0 && strbuf[len] == '.')
			strbuf[len] = '\0';
		AddDebuggerItem(label, strbuf, readOnly);
	}

	void AddDebugLogMessage(const char* msg)
	{
#ifdef CONSTRUCT_DEBUGGER
		LogMsg(msg);
#endif
	}

	int	GetVariableIndexFromName(const char* name, CRunObject* obj)
	{
		if (obj == NULL) return -1;

		CString varName = name;
		return find_index(obj->pType->privateVars.begin(), obj->pType->privateVars.end(), varName);
	}

	int GetInstanceIndex(CRunObject* obj)
	{
		if (obj == NULL) return -1;
		return find_index(obj->pType->instances.begin(), obj->pType->instances.end(), obj);
	}

	void* RTNew(unsigned int sz)
	{
		return new char[sz];
	}

	void RTDelete(void* ptr, bool arr)
	{
		if (arr)
			delete[] ptr;
		else
			delete ptr;
	}

	CRunAnimationFrame* GetAnimationFrame(CRunAnimation* pAnim, int frameIndex)
	{
		if (pAnim == NULL) return NULL;

		if (frameIndex < 0 || frameIndex >= pAnim->frames.size())
			return NULL;

		

		AnimationFrames::iterator anim = pAnim->frames.begin();
		advance(anim, frameIndex);
		return &(*anim);
	}

	void SetAnimationFrame(CRunObject* obj, int frameIndex)
	{
#ifndef APPRUNTIME
		if (obj == NULL) return;

		int oldframe = obj->info.curAnim->curFrame;

		if (frameIndex < 0) frameIndex = 0;
		if (frameIndex >= obj->info.curAnim->pAnim->frames.size())
			frameIndex = obj->info.curAnim->pAnim->frames.size() - 1;

		if (frameIndex == oldframe) return;

		obj->info.curAnim->curFrame = frameIndex;

		UpdateObjectSizeForFrameChange(obj, oldframe,  obj->info.curAnim->handle, frameIndex, obj->info.curAnim->handle);

		UpdateAnimationFrame(obj);
#endif
	}

	int GetControlVK(const char* controlName, int player)
	{
		if (controlName == NULL) return -1;

		CString name = controlName;
		name.MakeLower();

		ControlsMap::iterator i = keyControls.find(make_pair(name, player));

		if (i == keyControls.end())
			return -1;
		else
			return i->second.vk;
	}

	float GetControlState(const char* controlName, int player)
	{
		if (controlName == NULL) return -1;

		CString name = controlName;
		name.MakeLower();

		ControlsMap::iterator i = keyControls.find(make_pair(name, player));

		if (i == keyControls.end())
			return -1;
		else
			return i->second.state;
	}

	bool SetControlState(const char* controlName, int player, float val)
	{
		if (controlName == NULL) return -1;

		CString name = controlName;
		name.MakeLower();

		ControlsMap::iterator i = keyControls.find(make_pair(name, player));

		if (i == keyControls.end())
			return false;
		else{
			i->second.state = val;
			return true;
		}
	}

	int GetControlDIK(const char* controlName, int player)
	{
		// TODO: convert to DIK
		return GetControlVK(controlName, player);
	}

	int GetAnimationFrameCount(CRunAnimation* pAnim)
	{
		if (pAnim == NULL) return 0;
		return pAnim->frames.size();
	}

	void SetGlobalKey(const char* name, void* param)
	{
		pluginKeys[name] = param;
	}

	void* GetGlobalKey(const char* name)
	{
		map<CString, void*>::iterator i = pluginKeys.find(name);

		if (i == pluginKeys.end())
			return NULL;
		else
			return i->second;
	}

	const char* GetGlobalKeyString(const char* name)
	{
		map<CString, CString>::iterator i = pluginKeysStrings.find(name);

		if (i == pluginKeysStrings.end())
			return "";
		else
			return i->second;
	}

	bool GlobalKeyExists(const char* name)
	{
		map<CString, void*>::iterator i = pluginKeys.find(name);

		return i != pluginKeys.end();
	}

	void SetLayoutKey(CRunLayout* pLayout, const char* name, void* param)
	{
		if (pLayout == NULL) return;

		pLayout->frameKeys[name] = param;
	}

	void* GetLayoutKey(CRunLayout* pLayout, const char* name)
	{
		if (pLayout == NULL) return NULL;

		map<CString, void*>::iterator i = pLayout->frameKeys.find(name);

		if (i == pLayout->frameKeys.end())
			return NULL;
		else
			return i->second;
	}

	bool LayoutKeyExists(CRunLayout* pLayout, const char* name)
	{
		if (pLayout == NULL) return false;

		map<CString, void*>::iterator i = pLayout->frameKeys.find(name);

		return i != pLayout->frameKeys.end();
	}

	const char* GetLayoutKeyString(CRunLayout* pLayout, const char* name)
	{
		if (pLayout == NULL) return "";

		map<CString, CString>::iterator i = pLayout->frameKeysStrings.find(name);

		if (i == pLayout->frameKeysStrings.end())
			return "";
		else
			return i->second;
	}

	CRunLayer* GetObjectLayer(CRunObject* obj) const
	{
		if (obj == NULL) return NULL;

		CRunLayout* pLayout = obj->pLayout;
		if (pLayout == NULL) return NULL;

		if (obj->info.layer < 0 || obj->info.layer >= pLayout->layers.size())
			return NULL;

		return pLayout->layers[obj->info.layer];
	}

	double GetTimeDelta() const
	{
		if (overrideTimeDelta) {
			double td = 1.0 / overrideTimeDeltaFPS;
			td *= timeScale;

#ifdef CONSTRUCT_DIRECTX9
			if (motionBlur)
				td /= blurNumSteps;
#endif

			return td;
		}
		else {
			if (minimum_fps == 0.0f)
				return timedelta * timeScale;
			// Minimum FPS specified
			else {
				double td = timedelta;
				if (td > (1 / minimum_fps))
					td = 1 / minimum_fps;
				return td * timeScale;
			}
		}
	}

	clock_t GetAppClock()
	{
		return (clock_t)(timer * 1000.0);// + clockTemporalOffset - clockOffset;
	}

	double GetAppClockf()
	{
		return timer * 1000.0;
	}

	HMENU	GetMenuResource(int id)
	{
		if (id < 0 || id >= menus.size())
			return NULL;

		return menus[id];
	}

	virtual CRunLayer* GetLayer(CRunLayout* pLayout, int number)
	{
		if (pLayout == NULL) return NULL;

		if (number < 0 || number >= pLayout->layers.size())
			return NULL;
		else return pLayout->layers[number];
	}

	virtual CRunLayer* GetLayer(CRunLayout* pLayout, const char* name)
	{
		if (pLayout == NULL) return NULL;

		CString layerName = name;
		layerName.MakeLower();

		vector<CRunLayer*>::iterator y = pLayout->layers.begin();
		vector<CRunLayer*>::iterator layers_end = pLayout->layers.end();

		for ( ; y != layers_end; y++) {
			CString curName = (*y)->name;
			curName.MakeLower();

			if (layerName == curName)
				return *y;
		}

		// Not found
		return NULL;
	}

	void SerializeObjectPtr(CRunObject*& obj, bin& ar)
	{
		if (ar.loading) {
			int uid;
			ar >> uid;

			if (uid != -1) {
				// Add a record that this UID needs to be found and written to 'obj' later on
				// (the object with that UID may not be created yet)
				DelayedSerializeObjectPtr dsop;
				dsop.obj = &obj;
				dsop.uid = uid;
				delayedSerializeObjects.push_back(dsop);
			}
		}
		else {
			// Save UID
			if (obj == NULL)
				ar << -1;
			else
				ar << obj->info.uid;
		}
	}

	void SerializeObjectType(CRunObjType*& pType, bin& ar)
	{
		if (ar.loading) {
			int oid;
			ar >> oid;

			if (oid == -1)
				pType = NULL;
			else
				pType = objects[oid];
		}
		else {
			if (pType == NULL)
				ar << -1;
			else
				ar << pType->oid;	
		}
	}

	void GetCurrentEventSolInfo(EventSolInfo& esi) const
	{
		esi.modifiesSol = pCurrentEvent->modifiesSol != 0;
		esi.modifiers = pCurrentEvent->modifiers;
	}

	void DoFrame()
	{
		if (Frame())
			bRunning = false;
	}

	clock_t GetLayoutClock(CRunLayout* pLayout);

	void RunActions(ACTS_PTR pActions);
	void RunSubEvents(SUBEVENTS_PTR pSubEvents);
	void RegisterWindow(HWND hWnd, CRunObject* runData);
	void UnRegisterWindow(HWND hWnd, CRunObject* runData);
	void CallPreTranslateMessage(CRunObject* pObject, bool bCall = true);
	CRunAnimation* GetAnimationPtr(Animation anim);
	TickAnimRet TickAnimation(CRunObject* obj);
	Animation GetAnimationByName(Animation parent, const char* name);
	Animation GetAnimationByNearestAngle(Animation parent, float angle);
	CRunAnimationFrame* GetCurAnimationFramePtr(AnimInfo* info);
	CRunAnimation* SetAnimation(Animation anim, CRunObject* obj);
	void UpdateAnimationFrame(CRunObject* obj);

	void SelectF(CRunObject* obj, CRunObjType* pType)
	{	Select(obj, pType);		}

	bool IsSelectedF(CRunObject* obj, CRunObjType* pType)
	{	return IsSelected(obj, pType);		}

	void UnselectF(CRunObject* obj, CRunObjType* pType)
	{	Unselect(obj, pType);		}

	void SetTimeScale(double ts)
	{
		timeScale = ts;
		if (timeScale < 0.0) timeScale = 0.0;
	}

	double GetTimeScale() const
	{	return timeScale;	}


	void deprecated_SaveTextureToFileInMemory(void** lpBuffer, int* pBufferSize, int iTextureHandle, int iImageFormat)
	{
		/*
#ifdef CONSTRUCT_DIRECTX9
		LPD3DXBUFFER buff = (LPD3DXBUFFER)(*lpBuffer);
		D3DXSaveTextureToFileInMemory(
		  &buff,
		 (D3DXIMAGE_FILEFORMAT)iImageFormat,
		  Display.TextureAt(iTextureHandle),
		  NULL
		);

		if(pBufferSize)
			*pBufferSize = buff->GetBufferSize();

		*lpBuffer = buff;
#endif
		*/
	}

	void deprecated_FreeTextureFileMemory(void** lpBuffer)
	{
		/*
#ifdef CONSTRUCT_DIRECTX9
		if(*lpBuffer)
			((LPD3DXBUFFER)(*lpBuffer))->Release();
#endif
			*/
	}

	void* deprecated_GetTextureFileBuffer(void ** lpBuffer)
	{
#ifdef CONSTRUCT_DIRECTX9
		return ((LPD3DXBUFFER)(*lpBuffer))->GetBufferPointer();
#else
		return NULL;
#endif
	}

	void GetFamilyTypes(CRunObjType* pType, CRunObjType**& pTypesList, int& count)
	{
		if (pType == NULL) {
			count = 0;
			pTypesList = NULL;
			return;
		}
	
		pTypesList = &(pType->teams.front());
		count = pType->teams.size();
	}

	void ScreenToScreen(POINT& pt)
	{
		if (!fullscreen)
			ScreenToClient(hWnds.front(), &pt);
		//pt.x += winWidthOffset;
		//pt.y += winHeightOffset;
	}

	ImageHandleInfo* LoadSerializedImageHandle(bin& ar)
	{
#ifdef APPRUNTIME
		return NULL;
#else
		if (!ar.loading)
			throw runtime_error("Serialization error: cannot load serialized image handle: archive is not loading");

		return imagehandle_to_address[ar.read<int>()];
#endif
	}

	void LoadAnimation(CRunAnimation* pRootAnim)
	{
#ifndef APPRUNTIME
		if (pRootAnim != NULL)
			pRootAnim->Load(renderer);
#endif
	}

	void ReleaseAnimation(CRunAnimation* pRootAnim)
	{
#ifndef APPRUNTIME
		if (pRootAnim != NULL)
			pRootAnim->Unload(renderer);
#endif
	}

	void SuppressElseEvent()
	{
		suppress_else_event = true;
	}