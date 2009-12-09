// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aSetValue(LPVAL params)
{
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] = params[1];

	return 0;
}

long ExtObject::aShootObject(LPVAL params)
{
	CRunObjType* pObjType = params[0].GetObjectParam(pRuntime);

	if (pObjType == NULL) return 0;

	CRunLayer* pLayer = params[1].GetLayerParam(pRuntime, pLayout);

	if (pLayer == NULL) return 0;

	// Create an object of the given type on my layer
	CRunObject* pObject = pRuntime->CreateObject(pObjType, pLayer->number, pLayout);

	// Disabled with 'Destroy when shader not available'
	if (pObject == NULL) return 0;

	pObject->info.x = info.x;
	pObject->info.y = info.y;

	POINTF ip;

	// Orient and position this object as I am
	if (params[2].Type() == EXPTYPE_INTEGER) {
		int ipNumber = params[2].GetInt() - 1;

		if (ipNumber >= 0 && pRuntime->GetImagePoint(this, ipNumber, ip)) {
			pObject->info.x = ip.x;
			pObject->info.y = ip.y;
		}
	}
	else if (params[2].Type() == EXPTYPE_STRING) {
		CString ptName = params[2].GetString();

		if (pRuntime->GetImagePoint(this, ptName, ip)) {
			pObject->info.x = ip.x;
			pObject->info.y = ip.y;
		}
	}

	pObject->info.angle = info.angle;
	pRuntime->UpdateBoundingBox(pObject);

	// Select the object fired - but don't select it if selecting would modify this object type's SOL (crashes)
	CRunObject** i;
	int count;
	pRuntime->GetObjectSiblings(pObject, i, count);

	CRunObject** end = i + count;

	for ( ; i != end; i++) {
		if ((*i)->pType == pType)
			return 0;
	}

	if (pObjType == pType)
		return 0;

	// Select, now that we can assume object is not modifying this->pType's SOL
	pRuntime->SelectAll(pObjType);
	pRuntime->SelectF(pObject, pObjType);

	return 0;
}

float RotateTowards(float start, float end, float step)
{
   float cs = cos(RADIANS(start));
   float ce = cos(RADIANS(end));
   float ss = sin(RADIANS(start));
   float se = sin(RADIANS(end));

   float z = cs*se - ss*ce;
   float dot = cs*ce + ss*se;
   float angle = DEGREES(acos(dot));
  
   if(angle > step)
      if(z > 0)
         return start + step;
      else
         return start - step;
   else
      return end;
}

long ExtObject::aRotateToObject(LPVAL params)
{
	CRunObject* pTarget = params[0].GetPairedObjectParam(pRuntime, this);
	if (pTarget == NULL) return 0;

	float x1 = info.x;
	float y1 = info.y;
	float x2 = pTarget->info.x;
	float y2 = pTarget->info.y;

	float targetAngle = DEGREES(atan2(y2 - y1, x2 - x1));
	
	info.angle = RotateTowards(info.angle, targetAngle, params[1].GetFloat());
	pRuntime->UpdateBoundingBox(this);

	return 0;
}

long ExtObject::aRotateToPosition(LPVAL params)
{
	float x1 = info.x;
	float y1 = info.y;
	float x2 = params[0].GetFloat();
	float y2 = params[1].GetFloat();

	float targetAngle = DEGREES(atan2(y2 - y1, x2 - x1));
	
	info.angle = RotateTowards(info.angle, targetAngle, params[2].GetFloat());
	pRuntime->UpdateBoundingBox(this);

	return 0;
}

long ExtObject::aAddValue(LPVAL params)
{
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] += params[1];

	return 0;
}

long ExtObject::aSubValue(LPVAL params)
{
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] -= params[1];

	return 0;
}

long ExtObject::aFlash(LPVAL params)
{
	flashInterval = params[0].GetDouble();
	flashTime = params[1].GetDouble();
	pRuntime->CallOnFrame(this);
	return 0;
}

long ExtObject::aPlayAnimation(LPVAL params)
{
	m_Animation.playing = true;
	m_Animation.counter = 0;
	m_Animation.updated = true;
	m_Animation.repeat = m_Animation.pAnim->repeat;
	m_Animation.repeat_to = m_Animation.pAnim->repeat_to;
	m_Animation.pingpong = m_Animation.pAnim->pingpong;

	pRuntime->SetAnimationFrame(this, 0);

	return 0;
}

long ExtObject::aPauseAnimation(LPVAL params)
{
	m_Animation.playing = false;

	return 0;
}

long ExtObject::aResumeAnimation(LPVAL params)
{
	m_Animation.playing = true;

	return 0;
}

long ExtObject::aSetAnimationSpeed(LPVAL params)
{
	m_Animation.speed = params[0].GetFloat();
	//TODO
	return 0;
}
long ExtObject::aSetAnimation(LPVAL params)
{
	CString animation_name = params[0].GetString();

	vector<CString> names;
	CRunAnimation* curAnim = pRuntime->GetAnimationPtr(info.curAnim->handle);

	// We may need to begin ticking animations again.
	pRuntime->CallOnFrame(this);

	// step one, feed up the animation tree till we find the specified animation
	while(curAnim)
	{
		if(curAnim->parent)
			names.push_back(curAnim->name);

		int subAnimCount = pRuntime->GetSubAnimationCount(curAnim);

		// Check sub animations of curAnim to see if we have the specified one
		for(int i = 0; i < subAnimCount; i++)
		{
			// Copy anim name to a local CString using the const char* overload.
			// This is the only safe way to read a CString across the runtime/DLL boundary.
			CRunAnimation* ii = pRuntime->GetSubAnimation(curAnim, i);
			CString curName = (const char*)(ii->name);
	
			if(!curName.CollateNoCase(animation_name))
			{
				curAnim = ii;
				break;
			}
		}
		
		CString curName = (const char*)curAnim->name;

		if(!curName.CollateNoCase(animation_name) && curAnim->parent &&!curAnim->is_angle ) //! means they are equal !
		{	
			break;
		}
		
		curAnim = curAnim->parent;
	}
	
	if(curAnim)
	{
		// okay we have an animation...go through as many subanimations as we can to maintain it.
		while(names.size())
		{
			bool found = false;
			int subAnimCount = pRuntime->GetSubAnimationCount(curAnim);
			for(int a = 0; a < subAnimCount; a++)
			{
				CRunAnimation* i = pRuntime->GetSubAnimation(curAnim, a);
				if(i->name == names.back())
				{
					names.pop_back();
					curAnim = &*i;
					a = subAnimCount; // escape....
					found = true;
				}
			}
			if(!found)
			{
				names.pop_back();
			}
		}
		int anim = pRuntime->GetAnimationByNearestAngle(curAnim->handle, this->info.angle);

		if(info.curAnim->handle != anim){
			pRuntime->SetAnimation(anim, this);
		}
	}
	

	return 0;
}

long ExtObject::aSetCollMode(LPVAL params)
{
	info.collMode = (CollisionMode)params[0].GetInt();
	return 0;
}

long ExtObject::aLoadFrameFromFile(LPVAL params)
{
	TextureHandle newTex;

	try {
		newTex = renderer->CreateTextureFromFile(params[0].GetString());
	}
	catch (...) {
		return 0;
	}

	// Swap out the old texture for the new one
	renderer->ReplaceTexture(info.curTexture, newTex);

	CRunAnimationFrame* pAnimFrame = pRuntime->GetAnimationFrame(m_Animation.pAnim, m_Animation.curFrame);

	float xScale = info.w / pAnimFrame->width;
	float yScale = info.h / pAnimFrame->height;

	pAnimFrame->width = info.curTexture->image_widthf;
	pAnimFrame->height = info.curTexture->image_heightf;
	info.w = xScale * info.curTexture->image_widthf;
	info.h = yScale * info.curTexture->image_heightf;
	pRuntime->UpdateBoundingBox(this);

	// Update collision mask
	pRuntime->GenerateCollisionMaskFromTexture(this, info.curTexture);

	return 0;
}

long ExtObject::aSetFramePivotPoint(LPVAL params)
{
	CRunAnimationFrame* pAnimFrame = pRuntime->GetAnimationFrame(m_Animation.pAnim, m_Animation.curFrame);

	pAnimFrame->pivotX = params[0].GetInt();
	pAnimFrame->pivotY = params[1].GetInt();

	pAnimFrame->pivotAngle = atan2((float)pAnimFrame->pivotY, (float)pAnimFrame->pivotX);
	pAnimFrame->pivotDist = sqrt((double)pAnimFrame->pivotX * pAnimFrame->pivotX + pAnimFrame->pivotY * pAnimFrame->pivotY);

	pRuntime->UpdateBoundingBox(this);

	return 0;
}

long ExtObject::aSetAnimFrame(LPVAL params)
{
	m_Animation.counter = 0;
	m_Animation.updated = true;
	m_Animation.playing = false;

	pRuntime->SetAnimationFrame(this, params[0].GetInt() - 1);
	
	return 0;
}

long ExtObject::aSetVertex(LPVAL params)
{
	int indx = params[0].GetInt();
	COLORREF col = params[1].GetInt();
	cr_float opacity = params[2].GetFloat();

	Clamp(indx, 0, 3);
	Clamp(opacity, 0.0f, 100.0f);

	cr::color c(col);
	c.a = opacity / 100.0f;

	vertexcolors[indx] = c;

	// If any of the vertices aren't opaque white, set the modified flag.  Otherwise use the flag to ignore them
	for (int i = 0; i < 4; i++) {
		if (vertexcolors[i] != opaque_white) {
			vertices_changed = true;
			return 0;
		}
	}
	vertices_changed = false;

	return 0;
}

long ExtObject::aSetSkewX(LPVAL params)
{
	info.pInfo->skewX = params[0].GetFloat();
	UpdateBoundingBox();
	return 0;
}

long ExtObject::aSetSkewY(LPVAL params)
{
	info.pInfo->skewY = params[0].GetFloat();
	UpdateBoundingBox();
	return 0;
}

long ExtObject::aSetDistortMapSize(LPVAL theParams)
{
	long x = theParams[0].GetInt() + 1;
	long y = theParams[1].GetInt() + 1;

	// DX9 renderer only supports up to 20,000 vertices per batch; fail if too many vertices created
	if (x * y >= 20000 || (x-1) * (y-1) * 6 >= 30000) {
		pRuntime->AddDebugLogMessage("Sprite: cannot support a distort map that large (W x H x 6 must be < 30000)");
		return 0;
	}

	distort.resize(x);

	for(int i = 0; i < x; i++)
		distort[i].resize(y);

	return 0;
}

long ExtObject::aSetDisplacementAt(LPVAL theParams)
{
	long x = theParams[0].GetInt();
	long y = theParams[1].GetInt();	

	if(x < distort.size()){
		if(y < distort[x].size()){
			distort[x][y].x = theParams[2].GetFloat() / info.w;
			distort[x][y].y = theParams[3].GetFloat() / info.h;
			distort[x][y].u = theParams[4].GetFloat() / info.w;
			distort[x][y].v = theParams[5].GetFloat() / info.h;

			cr::color c(theParams[6].GetColorRef());	// vertex rgb color
			c.a = theParams[7].GetFloat() / 100.0f;		// vertex opacity
			Clamp(c.a, 0.0f, 1.0f);
			distort[x][y].filter = c;

			distort[x][y].z = theParams[8].GetFloat();

		}
	}
	return 0;
}

long ExtObject::aSetDisplacementRealAt(LPVAL theParams)
{
	long x = theParams[0].GetInt();
	long y = theParams[1].GetInt();	

	if(x < distort.size()){
		if(y < distort[x].size()){
			distort[x][y].x = (theParams[2].GetFloat() - info.x + info.HotSpotX) / info.w  - x / float(distort.size()-1);
			distort[x][y].y = (theParams[3].GetFloat() - info.y + info.HotSpotY) / info.h - y / float(distort[0].size()-1);
			distort[x][y].u = theParams[4].GetFloat() / info.w;
			distort[x][y].v = theParams[5].GetFloat() / info.h;
			distort[x][y].z = theParams[8].GetFloat();

			cr::color c(theParams[6].GetColorRef());	// vertex rgb color
			c.a = theParams[7].GetFloat() / 100.0f;		// vertex opacity
			Clamp(c.a, 0.0f, 1.0f);
			distort[x][y].filter = c;

		}
	}
	return 0;
}

// Update the animation angle
long ExtObject::aSetAnimationAngle(LPVAL theParams)
{
	float given_angle = theParams[0].GetFloat();

	// If animation angles are locked to object angle, the only way to set animation angle is to set the object angle
	if (locked_animation_angles) {
		info.angle = given_angle;
		UpdateBoundingBox();
	}
	else {
		// Set the animation angle nearest to passed angle
		if(pRuntime->GetSubAngleAnimationCount(info.curAnim->pAnim->parent) != 1)
		{
			int anim = pRuntime->GetAnimationByNearestAngle(info.curAnim->handle, given_angle);
			if(info.curAnim->handle != anim)
				pRuntime->SetAnimation(anim, this);	
			pRuntime->UpdateAnimationFrame(this);
		}
	}

	return 0;
}

class plugin_allocator : public bin_allocator {
public:
	BYTE* allocNew(unsigned int _size) {
		return new BYTE[_size];
	}
	void allocDelete(void* ptr) {
		delete[] ptr;
	}
} g_plugin_allocator;

long ExtObject::aSaveMesh(LPVAL theParams)
{
	if (distort.empty())
		return 0;

	bin ar(g_plugin_allocator);
	ar.loading = false;

	UINT version = 1;

	const DWORD meshcode = ('M' << 24) | ('E' << 16) | ('S' << 8) | 'H';
	ar << meshcode;
	ar << version;

	UINT cx = distort.size();
	UINT cy = distort.front().size();

	// Save X and Y sizes
	ar << cx;
	ar << cy;

	for (UINT x = 0; x < cx; x++) {
		for (UINT y = 0; y < cy; y++) {
			const DistortInfo& di = distort[x][y];

			// Export x and y as proportion of original object size
			ar << di.x << di.y << di.z << di.u << di.v << di.filter;
		}
	}

	ar.save(theParams[0].GetString());

	return 0;
}

long ExtObject::aLoadMesh(LPVAL theParams)
{
	bin ar(g_plugin_allocator);
	ar.loading = true;

	ar.load(theParams[0].GetString());

	// Not big enough for code, version, size X, size Y
	if (ar.size() < 16) {
		pRuntime->AddDebugLogMessage("'Load mesh from file': Invalid mesh file");
		return 0;
	}

	const DWORD meshcode = ('M' << 24) | ('E' << 16) | ('S' << 8) | 'H';

	DWORD loadedcode;
	UINT version;

	ar >> loadedcode;

	if (loadedcode != meshcode) {
		pRuntime->AddDebugLogMessage("'Load mesh from file': Invalid mesh file");
		return 0;
	}

	ar >> version;

	if (version > 1) {
		pRuntime->AddDebugLogMessage("'Load mesh from file': File appears to be created with a newer version of Construct.");
		return 0;
	}

	UINT cx, cy;
	ar >> cx >> cy;

	distort.resize(cx);

	for (UINT i = 0; i < cx; i++)
		distort[i].resize(cy);

	for (UINT x = 0; x < cx; x++) {
		for (UINT y = 0; y < cy; y++) {
			DistortInfo& di = distort[x][y];

			ar >> di.x >> di.y >> di.z >> di.u >> di.v >> di.filter;
		}
	}

	return 0;
}