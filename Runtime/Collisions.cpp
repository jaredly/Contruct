#include "StdAfx.h"

//#include "..\Common\CxImage\CxImage\ximage.h"

#ifndef APPRUNTIME

// Debug:  generate a BMP file from a bitmask to show bits on/off
void DbgCollisionMaskToFile(CollisionMask& cm, int index, int id)
{
	/*
#ifdef _DEBUG
	CxImage im;
	im.Create(cm.pitch * 8, cm.height, 24, CXIMAGE_FORMAT_BMP);
	im.Clear(0);
	BYTE* ptr = (BYTE*)cm.bits[index];
	int ipitch = cm.pitch / sizeof(BYTE);

	// Loop every bit
	for (int x = 0; x < cm.pitch * 8; x++) {
		for (int y = 0; y < cm.height; y++) {
			DWORD cur = ptr[((int)cm.height - y - 1) * ipitch + (x / 8)];
			DWORD mask = 1 << (7 - (x % 8));
			if (cur & mask)
				im.SetPixelColor(x, y, RGB(255,255,255));
		}
	} 

	CString filename;
	filename.Format("C:\\images\\debug\\_bitmask%d.bmp", id);
	// nah
	im.Save(filename, CXIMAGE_FORMAT_BMP);
#endif
	*/
		
}

//////////////////////////////
// GENERATE COLLISION MASKS //
//////////////////////////////
// Generate the 1-7 shifted maps from the unshifted map
void CRuntime::GenerateShiftedMasks(CollisionMask& cm)
{
	// Temp - skip the 32x32 spare exports
	//if (cm.width == 32.0f && cm.height == 32.0f)
	//	return;

	// Can't process with NaN values
	if (!_finite(cm.width) || !_finite(cm.height) || cm.height < 0.0)
		return;

	// Remember pitch is aligned to 16 byte segments.  32 bit ints fit nicely
	int maskSize = cm.mem_size;

	// tigs- Pitch as integers
	//const int ipitch = cm.pitch / sizeof(int);
	// davo
	const int ipitch = cm.pitch / sizeof(BYTE);

	////////////////////// 
	//static int id = 0;
	//DbgCollisionMaskToFile(cm, 0, id++);
	//////////////////////

	for (int shift = 1; shift < 8; shift++) {

		cm.bits[shift] = (BYTE*)_aligned_malloc(maskSize, 16);

		if (cm.bits[shift] == NULL)
			throw runtime_error("Out of memory!");

		//memcpy(cm.bits[shift], cm.bits[0], maskSize);
		memset(cm.bits[shift], 0, maskSize);


		// tigs method
		/*

		BYTE* ptr = cm.bits[0];
		// Take the previous map and shift left by one
		DWORD* sourcePtr = (DWORD*)(cm.bits[shift - 1]);
		DWORD* sourceEnd = sourcePtr + (ipitch * (int)cm.height);
		
		DWORD* const destPtr = (DWORD*)(cm.bits[shift]);

		DWORD* destVertEnd = destPtr + (ipitch * (DWORD)cm.height);
		DWORD* srcVert = sourcePtr;

		for (DWORD* destVert = destPtr; destVert != destVertEnd; destVert += ipitch, srcVert += ipitch) {
			
			DWORD* destHoriz = destVert;
			DWORD* destHorizEnd = destVert + ipitch;
			DWORD* destHorizLast = destHorizEnd - 1;

			DWORD* srcHoriz = srcVert;

			for ( ; destHoriz != destHorizEnd; destHoriz++) {
				*destHoriz = *srcHoriz << 1;

				// If not last in row, shift in the upper bit from the next DWORD
				if (destHoriz != destHorizLast) {

					*destHoriz |= (*(++srcHoriz) >> 31);
				}

			}
		}
		*/

		// Davos method
		//BYTE* ptr = cm.bits[0];
		// Take the previous map and shift left by one
		BYTE* sourcePtr = (BYTE*)(cm.bits[shift - 1]);
		BYTE* sourceEnd = sourcePtr + (ipitch * (int)cm.height);
		
		BYTE* const destPtr = (BYTE*)(cm.bits[shift]);

		BYTE* destVertEnd = destPtr + (ipitch * (int)cm.height);
		BYTE* srcVert = sourcePtr;

		for (BYTE* destVert = destPtr; destVert != destVertEnd; destVert += ipitch, srcVert += ipitch) {
			
			BYTE* destHoriz = destVert;
			BYTE* destHorizEnd = destVert + ipitch;
			BYTE* destHorizLast = destHorizEnd - 1;

			BYTE* srcHoriz = srcVert;

			for ( ; destHoriz != destHorizEnd; destHoriz++) {
				*destHoriz = *srcHoriz << 1;

				// If not last in row, shift in the upper bit from the next DWORD
				if (destHoriz != destHorizLast) {

					*destHoriz |= (*(++srcHoriz) >> 7);
				}

			}
		}
		//static int id = 0;
		//DbgCollisionMaskToFile(cm, shift, id++);
	}
}

//////////////////////
// QUERY COLLISIONS //
//////////////////////
// For dispatching to the correct collision algorithm.

// Box overlaps.  No NOT operator (faster for where not needed)
inline bool NoBoxOverlap(const RECTF& a, const RECTF& b)
{
	return (a.right <= b.left || a.left >= b.right || a.bottom <= b.top || a.top >= b.bottom);
}

// Point/box overlaps.
inline bool PointInsideBox(float x, float y, const RECTF& box)
{
	return (x >= box.left && x < box.right && y >= box.top && y < box.bottom);
}

// True if size and angle identical to mask
inline bool IsObjectUnscaled(CRunObject* o, CollisionMask& coll)
{
	return (o->info.w == coll.width && o->info.h == coll.height && o->info.displayangle == 0.0f);
}

bool CRuntime::QueryCollision(CRunObject *a, CRunObject *b)
{
	if (a == NULL || b == NULL) return false;
	if (a == b) return false;

	// Check the collision algorithms A and B want
	switch (a->info.collMode) {

	// 'A' using no collisions: fail
	case COLLISIONMODE_NONE:
		return false;

	// 'A' using point (hotspot) only collisions
	case COLLISIONMODE_POINT:
		
		switch (b->info.collMode) {

		// B using no collisions: fail
		case COLLISIONMODE_NONE:
			return false;

		// A and B using point collisions: match X/Y (unlikely but these are the settings)
		case COLLISIONMODE_POINT:
			return ((a->info.x == b->info.x) && (a->info.y == b->info.y));

		// A is using points and B is using boxes: point-in-box check
		case COLLISIONMODE_BOX:
			return PointInsideBox(a->info.x, a->info.y, b->info.box);

		// A is a point and B is using bitmasks:  check one pixel from the mask
		case COLLISIONMODE_FINE:
			
			// First check if the pixel is inside the region
			//if (!PointInsideBox(a->info.x, a->info.y, b->info.box)) - IsOverlapPointInMask performs this now
			//	return false;
			
			// Else use the bitmask of B to find the overlap.
			return IsOverlapPointInMask(a->info.x, a->info.y, b);
		}

	// 'A' using bounding boxes
	case COLLISIONMODE_BOX:

		switch (b->info.collMode) {

		// B using no collisions: fail
		case COLLISIONMODE_NONE:
			return false;

		// A using a box and B using a point:  point-in-box check
		case COLLISIONMODE_POINT:
			return PointInsideBox(b->info.x, b->info.y, a->info.box);

		// A and B using boxes:  do a box check
		case COLLISIONMODE_BOX:
			return !NoBoxOverlap(a->info.box, b->info.box);

		// A is using box checks and B is using fine collision: box-on-mask check
		case COLLISIONMODE_FINE:
			// TODO
			//DAVODAVODAVO {
		
			// First test bounding boxes
			if (NoBoxOverlap(a->info.box, b->info.box))
				return false;

			// If first has invalid textures, assume box collision (we know box overlap from above)
			if (b->info.pInfo->collisionMaskHandle == NULL)
				return true;

			// Okay use MMX
			CollisionMask& collB = GetActiveMask(b);
			return IsOverlapBoxInMask64MMX(b, collB, a->info.box)!=0;

			//}DAVODAVODAVO
		}

	// 'A' using bitmasks for fine collision
	case COLLISIONMODE_FINE:

		switch (b->info.collMode) {

		// B using no collisions: fail
		case COLLISIONMODE_NONE:
			return false;

		// A using mask and B using point: check one pixel from the mask
		case COLLISIONMODE_POINT:
			
			// First check the point is inside the object
			//if (!PointInsideBox(b->info.x, b->info.y, a->info.box)) - IsOverlapPointInMask performs this now
			//	return false;

			// Perform a fine test
			return IsOverlapPointInMask(b->info.x, b->info.y, a);

		// A using bitmasks and B using boxes:  box-in-mask check
		case COLLISIONMODE_BOX:
			
			// TODO
			//DAVODAVODAVO
			{
		
			// First test bounding boxes
			if (NoBoxOverlap(a->info.box, b->info.box))
				return false;

			// If first has invalid textures, assume box collision (we know box overlap from above)
			if (a->info.pInfo->collisionMaskHandle == NULL)
				return true;

			// Okay use MMX
			CollisionMask& collA = GetActiveMask(a);
			return IsOverlapBoxInMask64MMX(a, collA, b->info.box)!=0;


			}
			//}DAVODAVODAVO
			

		// A and B using masks:  mask-on-mask check
		case COLLISIONMODE_FINE:
			
			// First test bounding boxes
			if (NoBoxOverlap(a->info.box, b->info.box))
				return false;

			// If either have invalid textures, assume box collision (we know box overlap from above)
			if (a->info.pInfo->collisionMaskHandle == NULL || b->info.pInfo->collisionMaskHandle == NULL)
				return true;

			CollisionMask& collA = GetActiveMask(a);
			CollisionMask& collB = GetActiveMask(b);

			// Either object is using an invalid mask - no collision can happen.
			if (&collA == NULL || &collB == NULL)
				return false;

			// Check for any bounding box bitmasks (solid bitmasks)
			/*if (collA.boundingbox) {
				if (collB.boundingbox)
					return true;	// Bounding box overlap, known to be true from above
				else
					// TODO: box-on-mask check
					throw runtime_error("Unsupported collision query (box(mask)-on-mask): Not yet implemented");
			}
			else if (collB.boundingbox)
				// TODO: mask-on-box check
				throw runtime_error("Unsupported collision query (box(mask)-on-mask): Not yet implemented");*/

			// If possible, use the SSE algorithm.
			// TODO: Not maintained.
			/*
			if (supportSSE) {
				int offset = (int)abs(a->info.box.left - b->info.box.left) % 128;
				if (offset < 8 && offset >= 0)
					return IsOverlapMaskInMask128SSE(a, b, collA, collB)!=0;
			}
			*/
			// X offset is 8 pixels after 128 bit alignments: SSE algorithm
			return IsOverlapMaskInMask64MMX(a, b, collA, collB)!=0;

		}
	}

	// Shouldn't really arrive here, something must be wrong so return false
	return false;
}

bool CRuntime::QueryCollision(CRunObject *obj, CRunObjType *type)
{
	if (obj == NULL || type == NULL) return false;

	// Loop all instances of "type", and check them
	ObjIterator i = type->instances.begin();
	ObjConstIterator instances_end = type->instances.end();

	for ( ; i != instances_end; i++) {
		if (obj == *i) continue;
		if (QueryCollision(obj, *i))
			return true;
	}

	// No instance overlapped
	return false;
}

// QuerySelectCollision:  Returns true if the object collides, and also selects the 'b'
// object if it overlaps
bool CRuntime::QuerySelectCollision(CRunObject *a, CRunObject *b, CRunObjType* b_type)
{
	if (a == NULL || b == NULL) return false;
	if (a == b) return false;	// Objects don't overlap themselves

	if (QueryCollision(a, b)) {

		if (!IsSelected(b, b_type))
			Select(b, b_type);

		return true;
	}
	return false;
}

bool CRuntime::QuerySelectCollision(CRunObject *obj, CRunObjType *type)
{
	if (obj == NULL || type == NULL) return false;

	// Loop all instances of "type", and check them
	ObjIterator i = type->instances.begin();
	ObjConstIterator instances_end = type->instances.end();

	bool ret = false;

	for ( ; i != instances_end; i++) {
		if (obj == *i) continue;

		// Don't break when one detected - we want to select ALL objects from the type that overlap
		if (QuerySelectCollision(obj, *i, type))
			ret = true;
	}

	return ret;
}

bool CRuntime::QueryPointCollision(int x, int y, CRunObject *obj)
{
	// Take the screen coordinates and test them on the object's specific layer
	float fx = x;
	float fy = y;
	//ScreenToLayer(obj->pLayout, obj->info.layer, fx, fy);

	switch (obj->info.collMode) {

	// using no collisions: fail
	case COLLISIONMODE_NONE:
		return false;

	// using point collisions: match X/Y (unlikely but these are the settings)
	case COLLISIONMODE_POINT:
		return (fx == obj->info.x) && (fy == obj->info.y);

	// using boxes: point-in-box check
	case COLLISIONMODE_BOX:
		return PointInsideBox(fx, fy, obj->info.box);

	// using bitmasks:  check one pixel from the mask
	case COLLISIONMODE_FINE:
		
		// First check if the pixel is inside the region - IsOverlapPointInMask performs this now
		//if (!PointInsideBox(fx, fy, obj->info.box))
		//	return false;
		
		// Else use the bitmask of B to find the overlap.
		return IsOverlapPointInMask(fx, fy, obj);
	}

	// Shouldn't end up here, bad settings?
	return false;
}

bool CRuntime::QueryPointCollision(int x, int y, CRunObjType* type)
{
	// Loop all instances of "type", and check them
	ObjIterator i = type->instances.begin();
	ObjConstIterator instances_end = type->instances.end();

	for ( ; i != instances_end; i++) {
		// CRunObject* overload
		if (QueryPointCollision(x, y, *i))
			return true;
	}

	return false;
}

bool CRuntime::QuerySelectPointCollision(int x, int y, CRunObject* obj, CRunObjType* pType)
{
	// No type to select provided: select from object's own type
	if (pType == NULL)
		pType = obj->pType;

	if (QueryPointCollision(x, y, obj)) {
		if (!IsSelected(obj, pType))
			Select(obj, pType);
		return true;
	}
	return false;
}

bool CRuntime::QuerySelectPointCollision(int x, int y, CRunObjType* type)
{
	// Loop all instances of "type", and check them
	ObjIterator i = type->instances.begin();
	ObjConstIterator instances_end = type->instances.end();

	bool ret = false;

	for ( ; i != instances_end; i++) {
		// CRunObject* overload
		if (QuerySelectPointCollision(x, y, *i, type))
			ret = true;
	}

	return ret;
}

bool CRuntime::QueryScreenPointInstanceCollision(int x, int y, CRunObject* obj)
{
	float layerX = x;
	float layerY = y;
	ScreenToLayer(obj->pLayout, obj->info.layer, layerX, layerY);

	return QueryPointCollision(layerX, layerY, obj);
}

bool CRuntime::QueryScreenPointCollision(int x, int y, CRunObjType* type)
{
	// Loop all instances of "type", and check them
	ObjIterator i = type->instances.begin();
	ObjConstIterator instances_end = type->instances.end();

	for ( ; i != instances_end; i++) {
		
		float layerX = x;
		float layerY = y;
		ScreenToLayer((*i)->pLayout, (*i)->info.layer, layerX, layerY);

		if (QueryPointCollision(layerX, layerY, *i))
			return true;
	}

	return false;
}

bool CRuntime::QuerySelectScreenPointCollision(int x, int y, CRunObjType* type)
{
	// Loop all instances of "type", and check them
	ObjIterator i = type->instances.begin();
	ObjConstIterator instances_end = type->instances.end();

	bool ret = false;

	for ( ; i != instances_end; i++) {
		
		float layerX = x;
		float layerY = y;
		ScreenToLayer((*i)->pLayout, (*i)->info.layer, layerX, layerY);

		if (QueryPointCollision(layerX, layerY, *i)) {
			ret = true;
			if (!IsSelected(*i, type))
				Select(*i, type);
		}
	}

	return ret;
}

//////////////////////////
// COLLISION ALGORITHMS //
//////////////////////////

void CRuntime::FreeMask(CollisionMask &m)
{
	// Aligned free each of the shifted masks in CollisionMask
	for (int i = 0; i < 8; i++) {
		if (m.bits[i]) {
			_aligned_free(m.bits[i]);
			m.bits[i] = NULL;
			m.mem_size = 0;
		}
	}
}

void CRuntime::FreeMask(CollisionMask* m)
{
	if (m) FreeMask(*m);
}

//OPTIMISE: blatant candidate for optimisation
// With many fine collision detecting sprites rotating/scaling, this could drag down performance
// It performs aligned mallocs and complex memory copies on buffers 100kb+
void CRuntime::GenerateScaledMask(CRunObject* obj, CollisionMask* src)
{
	// No valid source: can't generate a scaled mask!
	if (src == NULL) {
		obj->info.dynMask.curMask = NULL;
		return;
	}

	//OPTIMISE: search other instances for matching mask to copy?
	const int align_pitch_bits = 64;
	const int align_pitch_bytes = align_pitch_bits / 8;

	// Allocate a new CollisionMask for obj
	ObjCollisionMask& dynMask = obj->info.dynMask;
	dynMask.curMask = new CollisionMask;
	CollisionMask& dest = *(dynMask.curMask);
	dest.boundingbox = false;

	// In case exceptions throw during generation
	for (int i = 0; i < 8; i++)
		dynMask.curMask->bits[i] = NULL;

	// Determine new mask size
	dest.width = (int)(obj->info.box.right - obj->info.box.left);
	dest.height = (int)(obj->info.box.bottom - obj->info.box.top);
	dest.pitch = dest.width / 8;

	if ((int)dest.width % 8 != 0)
		dest.pitch++;		// 11 pixel width needs 2 bytes not 1 rounded

	// Eg. a 20 byte pitch must round up to 32 (+12, 16 - 4)
	if (dest.pitch % align_pitch_bytes != 0)
		dest.pitch += align_pitch_bytes - (dest.pitch % align_pitch_bytes);

	// Require at least 64 pixels of gutter space
	if ((dest.pitch * 8) - (int)dest.width < align_pitch_bits)
		dest.pitch += align_pitch_bytes;
	
	// Allocate unshifted mask
	dest.mem_size = dest.pitch * dest.height + 16;
	dest.bits[0] = (BYTE*)_aligned_malloc(dest.mem_size, 16);
	
	BYTE* bits = dest.bits[0];	// Shortcut to let us write directly to unshifted mask
	BYTE* srcbits = src->bits[0];

	// Clear the whole mask
	//OPTIMISE: clear only gutter area?
	memset(bits, 0, dest.mem_size);

	int pxWidth = int(dest.width+7)/8*8;	// OWNED. pxWidth has to be a modulus of 8, or the bit goes to the wrong spot
	int pxHeight = dest.height;

	int srcPitch = src->pitch;
	int destPitch = dest.pitch;

	float scaleX = src->width / obj->info.w;
	float scaleY = src->height / obj->info.h;

	static int id = 0;

	//OPTIMISE: loop by pointer arithmetic?

	// Loop every pixel (bit) of the dest mask to determine its bit

// Tigs i dont know how to test for performance so define TUSER to compare urs to mine
	// urs now works btw :) It'll be slower than before however coz ur bugged version
	// didnt completely convert the pixels. I highly recommend we create a mask where each 
	// byte represents 1 pixel so we can easily read from it coz the method used atm is
	// probably rather slow. My orignal collision class had one :P

//#define TUSER
#ifdef TUSER

	float srcXf = 0;

	for (int x = 0; x < pxWidth; x++, srcXf += scaleX) {

		float srcYf = 0;
		for (int y = 0; y < pxHeight; y++, srcYf += scaleY) {

			int srcX = srcXf;
			int srcY = srcYf;

			// Extract byte for srcX,srcY
			BYTE b = srcbits[srcY * srcPitch + (srcX/8)];

			// Extract bit from byte 
			b &= 1 << (7 - (srcX % 8));
			b = (b >> (7-srcX % 8)) << (7-x % 8);

			// Write bit to dest
			bits[y * destPitch + (x/8)] |= b;
		}
	}//for

#else	// davos method
	
	int count = 0;
	BYTE bEight;//[8];
	float angle = obj->info.displayangle;

	// Faster stretch-only algo
	if (angle == 0.0f) {
		float srcYf = 0.0f;
		for (int y = 0; y < pxHeight; y++, srcYf += scaleY) 
		{
			float srcXf = 0.0f;
			bEight = 0;
			count = 0;
			bits[y * destPitch] = 0;
			for (int x = 0; x < pxWidth; x++, srcXf += scaleX) 
			{
				// Rounding removed for 0.89: causes mask loss resizing 32 to 10,000 of 100 pixels or more.
				int srcX = srcXf;// + 0.5f;
				int srcY = srcYf;// + 0.5f;

				// Extract byte for srcX,srcY (will be 0 or 1)
				BYTE b = srcbits[srcY * srcPitch + (srcX/8)];
				//bEight |= b & (1 << (7 - (srcX % 8)));
				b &= 1 << (7 - (srcX % 8));
				b >>= (7 - (srcX % 8));
				//b = (b >> (7-srcX % 8))%2;	// Optimization of the 2 above
								
				bEight = (bEight << 1) | b;
				count++;
				if(count == 8)
				{
					bits[y * destPitch + (x/8)] = bEight;
					count = 0;
					bEight =0;
				}

				//further optimisations : we have bEight as an array of 8 and we create
				// the other masks at the same time. Prevents reading and writing 8 times
				// to instead just writing 8 times. - davo
			}
		}//for

		//DbgCollisionMaskToFile(dest, 0, id++);
	}// if angle == 0.0f
	// Slower, rotated, per-pixel transforming loop
	else {

		float invAngle = 0 - RADIANS(angle);

		// A little piece of genius... using offsetted matrix rotations, we can move the sin/cos out the loop
		float sinA, cosA;
		cr::sincosf(invAngle, &sinA, &cosA);

		float cosAscaleX = cosA * scaleX;
		float cosAscaleY = cosA * scaleY;
		float sinAscaleX = sinA * scaleX;
		float sinAscaleY = sinA * scaleY;

		// Coords of centre of bounding box
		float hsX = dest.width / 2.0 -0.01;	// the -0.01 is for rounding error
		float hsY = dest.height / 2.0 -0.01;	// the -0.01 is for rounding error

		float srcWidth = src->width;
		float srcHeight = src->height;

		// Offset by 0.5 for the rounded bounds check (9.9 -> 10.4 -> 10 int, possibly OOB)
		//srcWidth -= 0.5f;
		//srcHeight -= 0.5f;

		float srcOffsetX = src->width / 2.0;
		float srcOffsetY = src->height / 2.0;

		// Pre-add 0.5f.  Used inner-loop to round the floats to nearest int, but can be lifted out the loop.
		//srcOffsetX += 0.5f;
		//srcOffsetY += 0.5f;

		// Commenting the above 0.5 offsets seemed to also fix it

		float fycy = (-hsY) * cosA * scaleY;
		float fysx = (-hsY) * sinA * scaleX;

		for (int y = 0; y < pxHeight; y++, fycy += cosAscaleY, fysx += sinAscaleX) 
		{
			bEight = 0;
			count = 0;

			// Start translated
			float fxcx = (-hsX) * cosA * scaleX;
			float fxsy = (-hsX) * sinA * scaleY;

			bits[y * destPitch] = 0;
			
			for (int x = 0; x < pxWidth; x++, fxcx += cosAscaleX, fxsy += sinAscaleY) 
			{
				// Transform this pixel (x,y) to a source pixel

				//float fx = x;		// convert to floats
				//float fy = y;

				// Translate to (0,0)
				//fx -= hsX;
				//fy -= hsY;

				// Rotation using a matrix:
				// x' = x cos a - y sin a
				// y' = x sin a + y cos a

				// The formula is
				// (fx * cosA - fy * sinA) * scaleX + srcOffsetX;
				// (fx * sinA + fy * cosA) * scaleY + srcOffsetY;
				// =
				// scaleX * cosA * fx - scaleX * sinA * fy + srcOffsetX
				// scaleY * sinA * fx + scaleY * cosA * fy + srcOffsetY
				// Since we know fx and fy increment by 1, we use precalculated
				// incrementing values such as 'scaleX * cosA'.
				float fSrcX = (fxcx - fysx) + srcOffsetX;
				float fSrcY = (fxsy + fycy) + srcOffsetY;

				//float fSrcX = (fxcx) - (fy * scaleX * sinA) + srcOffsetX;
				//float fSrcY = (fx * scaleY * sinA) + (fy * scaleY * cosA) + srcOffsetY;

				//float fSrcX = (fx * cosA - fy * sinA) * scaleX + srcOffsetX;
				//float fSrcY = (fx * sinA + fy * cosA) * scaleY + srcOffsetY;

				// Bounds check, due to slack space in bounding box
				if (fSrcX > 0 && fSrcY > 0 && fSrcX < srcWidth && fSrcY < srcHeight)
				{

					// Convert to integer
					// Float to int is an expensive conversion.  Keep this cast inside the IF block!
					int iSrcX = fSrcX;
					int iSrcY = fSrcY;

					// Access bit
					BYTE b = srcbits[iSrcY * srcPitch + (iSrcX/8)];
					b &= 1 << (7 - (iSrcX % 8));
					b >>= (7 - (iSrcX % 8));

					// b is either gonna contain a 0 or a 1, so we can use OR to add it.
					bEight = (bEight << 1) | b;
				}
				else
					// Out of bounds: b = 0, shift in a zero
					bEight <<= 1;

				count++;
				if(count == 8)
				{
					bits[y * destPitch + (x/8)] = bEight;
					count = 0;
					bEight = 0;
				}
				

				//further optimisations : we have bEight as an array of 8 and we create
				// the other masks at the same time. Prevents reading and writing 8 times
				// to instead just writing 8 times. - davo
			}
		}//for

		//DbgCollisionMaskToFile(dest, 0, id++);
	}//else


#endif
	// Generate the shifted versions of this mask!
	GenerateShiftedMasks(dest);

	//DbgCollisionMaskToFile(dest, 0, id++);
	//DbgCollisionMaskToFile(*src, 0, 776);
	// Save to dynmask the state of the object
	dynMask.angle = obj->info.displayangle;
	dynMask.w = obj->info.w;
	dynMask.h = obj->info.h;
}

// Get the collision mask referenced by collisionMaskHandle
CollisionMask* CRuntime::GetDefaultMask(CRunObject* obj)
{
	if (obj->info.pInfo->generatedMask == NULL)
		return obj->info.pInfo->collisionMaskHandle;
	else
		return obj->info.pInfo->generatedMask;
}

// For comparing a bounding box & angle
class CBoxChecker {
public:
	const float& w;
	const float& h;
	const float& angle;
	CBoxChecker(const float& _w, const float& _h, const float& _angle) : w(_w), h(_h), angle(_angle) {}
	int operator==(const CBoxChecker& r) const { return w == r.w && h == r.h && angle == r.angle; }
};

CollisionMask& CRuntime::GetActiveMask(CRunObject *obj)
{
	// Get the object's dynamic mask
	ObjCollisionMask& dynMask = obj->info.dynMask;

	// Get the source mask
	CollisionMask* srcMask = GetDefaultMask(obj);

	// Does the object have its own active mask?
	if (dynMask.curMask) {

		// Is the object actually now unscaled, hence no longer needing a dynamic mask?
		if (IsObjectUnscaled(obj, *srcMask)) {

			// Free active mask, return unscaled mask
			FreeMask(dynMask.curMask);
			delete dynMask.curMask;
			dynMask.curMask = NULL;

			return *srcMask;
		}

		// Is the current active mask's box still valid compared to the current object box?
		if (CBoxChecker(dynMask.w, dynMask.h, dynMask.angle) == CBoxChecker(obj->info.w, obj->info.h, obj->info.displayangle))
			// OK - the current mask is still valid.
			return *(dynMask.curMask);

		// The object has an active mask, but it is no longer valid (the object has scaled/rotated).
		// Free the old one, then generate a new one.
		else {
			FreeMask(dynMask.curMask);
			delete dynMask.curMask;

			// Allocates a new curMask, generates scaled mask and shifted masks
			GenerateScaledMask(obj, srcMask);

			// Freshly allocated
			return *(dynMask.curMask);
		}
	}

	// Active mask is NULL.  No active mask present.
	else {

		// Ehh... ugly... but fixes crashes when no coll mask present
		if (srcMask == NULL)
			return *srcMask;

		// Needs to generate a scaled mask?
		if (IsObjectUnscaled(obj, *srcMask)) {

			// OK to return source mask
			return *srcMask;

		}
		// Object is scaled
		else {

			// Allocates a new curMask, generates scaled mask and shifted masks
			GenerateScaledMask(obj, srcMask);

			// Return freshly allocated
			return *(dynMask.curMask);
		}

	}

}

bool CRuntime::IsOverlapPointInMask(int x, int y, CRunObject *obj)
{
	// Get a mask for this object
	CollisionMask* m = &GetActiveMask(obj);

	// Is a mask present?
	if (m == NULL)
		return false;

	// Make X and Y coords relative to obj and bitwise.
	x -= obj->info.box.left;
	y -= obj->info.box.top;

	if(x < 0 || y < 0 || x >= m->width || y >= m->height)
		return false;

	// Calculate the bit representing this pixel
	// 0 -> high order bit, 7 -> low order bit
	BYTE b = 1 << (7 - (x % 8));

	// Make x go by byte.
	x /= 8;

	// Test our bit fron unshifted mask
	return (m->bits[0][y * m->pitch + x] & b)!=0;
}

//DAVODAVODAVO {//
int CRuntime::IsOverlapBoxInMask64MMX(CRunObject* a, CollisionMask& collA, RECTF rect)
{
	const int algorithm_integral_bits = 64;				// 64 bit MMX

	// As we are comparing the box to the map, the rect is in respect to the map
	RECTF& boxA = a->info.box;
	RECT overlapA;
	float overlapA_bottomf, overlapA_topf;
	overlapA.left = rect.left - boxA.left;
	overlapA.top = overlapA_topf = rect.top - boxA.top;
	overlapA.right = rect.right - boxA.left;
	overlapA.bottom = overlapA_bottomf = rect.bottom - boxA.top;

	// Check for A contained within 'rect'
	if (overlapA.left < 0) overlapA.left = 0;
	if (overlapA.top < 0) overlapA.top = overlapA_topf = 0;
	if (overlapA.bottom > collA.height) overlapA.bottom = overlapA_bottomf = collA.height;
	if (overlapA.right > collA.width) overlapA.right = collA.width;

	BYTE* bitsA = collA.bits[overlapA.left % 8];
	__m64* xStart = (__m64*)(bitsA + (overlapA.top * collA.pitch) + (overlapA.left / 8));
	__m64* xa = xStart;
	__m64* xEnd = xa + (overlapA.right - overlapA.left) / algorithm_integral_bits;

	// Box overlap doesn't need this.  We don't want to check the gutter, extra code below does that.
	//if ((overlapA.right - overlapA.left) % algorithm_integral_bits != 0)
	//	xEnd++;

	__m64* ya;
	__m64* yEnd;

	int yastep = collA.pitch / (algorithm_integral_bits / 8);
	int yaEndDiff = yastep * (int)(overlapA_bottomf - overlapA_topf); // correct rounding error

	// For each horizontal step in the bitmask
	for ( ; xa != xEnd; xa++) 
	{
		// Start Y pointers from X
		ya = xa;

		// Find end
		yEnd = ya + yaEndDiff;

		for ( ; ya != yEnd; ya += yastep)
		{
			// MMX 64-bit logical AND
			//result = _m_pand(*ya,*yb);

			// Native 32-bit check for collision
			if ((*ya).m64_u32[0] || (*ya).m64_u32[1])
			{
				_mm_empty();
				return true;
			}
		}
	}

	// Its important we create this using a byte array because the collision masks are
	// created with a byte array, and if we dont use bytes for everything that stuff gets 
	// reversed and flipped in the wrong spots.
	int distance = (overlapA.right - overlapA.left) % algorithm_integral_bits;
	if(distance <= 0) {
		_mm_empty();
		return false; //we have finished the test...no overlaps
	}


	// okay we still have a lip to go
	int dist = distance;
	BYTE edge[8] = {0,0,0,0,0,0,0,0};
	int x = 0;
	for( ; dist > 0; dist -= 8, x++)
	{
		if(dist >= 8)
			edge[x] = 0xFF;
		else 
			edge[x] = 0xFF << (8-dist);
	}

	__m64* mask = (__m64*)&edge[0];
	
	__m64 result;

	ya = xStart;
	yEnd = ya + yaEndDiff;

	for ( ; ya != yEnd; ya += yastep)
	{
		// MMX 64-bit logical AND
		result = _m_pand(*ya,*mask);

		// Native 32-bit check for collision
		if (result.m64_u32[0] || result.m64_u32[1])
		{
			_mm_empty();
			return true;
		}
	}

	// Dont forget to empty MMX state davo! -Ash
	_mm_empty();
	return false;
}

int Round(float f)
{
	return floor(f + 0.5);
	//return (int)f;
}

// 64-pixel overlap tester using MMX instructions (32-bit native)
int CRuntime::IsOverlapMaskInMask64MMX(CRunObject *a, CRunObject *b, CollisionMask& coll1, CollisionMask& coll2)
{
	const int algorithm_integral_bits = 64;				// 64 bit MMX

	bool ALeftOfB = (a->info.box.left <= b->info.box.left);

	// Swap A and B such that A is the leftmost object.
	OBJHEADER& infoA = (ALeftOfB ? a->info : b->info);
	OBJHEADER& infoB = (ALeftOfB ? b->info : a->info);

	CollisionMask& collA = (ALeftOfB ? coll1 : coll2);
	CollisionMask& collB = (ALeftOfB ? coll2 : coll1);

	assert(collA.mem_size > 0);
	assert(collB.mem_size > 0);
	
	RECTF& boxA = infoA.box;
	RECTF& boxB = infoB.box;

	bool AaboveB = (boxA.top <= boxB.top);

	__m64 *xa, *xb, *ya, *yb, *xEnd, *yEnd;
	BYTE *bitsA, *bitsB;
	
	// Overlapping rects relative to A and B
	RECT overlapA, overlapB;

	// Determine the relative overlap rectanges for A and B, knowing A is to the left of B
	overlapA.left = Round(boxB.left - boxA.left);
	overlapB.left = 0;
	
	if (AaboveB) {
		overlapA.top = Round(boxB.top - boxA.top); 
		overlapB.top = 0;
	}
	// B above A
	else {
		overlapA.top = 0;
		overlapB.top = Round(boxA.top - boxB.top);
	}

	if (boxB.left + collB.width > boxA.left + collA.width) {
		overlapA.right = Round(collA.width); 
		overlapB.right = Round(boxA.left + collA.width - boxB.left); 
	}
	else {
		overlapA.right = Round(boxB.left + collB.width - boxA.left); 
		overlapB.right = Round(boxB.left + collB.width - boxB.left);
	}

	if (boxB.top + collB.height > boxA.top + collA.height) {
		overlapA.bottom = Round(collA.height); 
		overlapB.bottom = Round(boxA.top + collA.height - boxB.top); 
	}
	else {
		overlapA.bottom = Round(boxB.top + collB.height - boxA.top); 
		overlapB.bottom = Round(boxB.top + collB.height - boxB.top);
	}

	if(overlapA.bottom <= overlapA.top || overlapB.bottom <= overlapB.top)
		return false;

	// A is leftmost: use preshifted mask to shunt A's bits in line to B's.
	//A 11101110 against
	//B    00110011
	//A    01110??? shifted by 3 against
	//B	   00110011
	bitsA = collA.bits[overlapA.left % 8];
	bitsB = collB.bits[0];

	// Initialise pointers ready to run
	xa = (__m64*)(bitsA + (overlapA.top * collA.pitch) + (overlapA.left / 8));
	xb = (__m64*)(bitsB + (overlapB.top * collB.pitch) + (overlapB.left / 8));

	xEnd = xa + (overlapA.right - overlapA.left) / algorithm_integral_bits;

	__m64* mem_end = xa + (collA.mem_size / sizeof(__m64));

	// Make sure we hit additional pixels (eg. width 70 / 64 = 1, whereas we need 2 iters)
	if ((overlapA.right - overlapA.left) % algorithm_integral_bits != 0)
		xEnd++;

	int yastep = collA.pitch / (algorithm_integral_bits / 8);
	int ybstep = collB.pitch / (algorithm_integral_bits / 8);
	
	//int yaEndDiff = yastep * floor(overlapA_bottomf - overlapA_topf);
	int yaEndDiff = yastep * (overlapA.bottom - overlapA.top); // This used to not work because bottom and top weren't rounded right, but now it does
	__m64 result;

	// For each horizontal step in the bitmask
	for ( ; xa != xEnd; xa++, xb++) {
		
		// Start Y pointers from X
		ya = xa;
		yb = xb;

		// Find end
		yEnd = ya + yaEndDiff;

		// Make sure we're not going to overrun the end of the memory buffer
		assert(yEnd <= mem_end);

		for ( ; ya != yEnd; ya += yastep, yb += ybstep) {

			// MMX 64-bit logical AND
			result = _m_pand(*ya,*yb);

			// Native 32-bit check for collision
			if (result.m64_u32[0] || result.m64_u32[1]) {
				_mm_empty();
				return true;
			}
		}
	}

	// Never found anything.  After all that!
	_mm_empty();
	return false;
}

// 128-pixel overlap tester using MMX instructions (32-bit native)
// This will ONLY WORK when the X offset is 0-7, from offsets of 128 pixels!!  This is due to SSE's requirement
// to align everything to 16-byte boundaries, and our eight bitmasks are the only things we have aligned to
// such a boundary.  Since SSE can shift by multiples of 8 bits it may be possible to write a fully SSE mask algo.
//TODO: Why the hell does this run slower than the MMX algo???
int CRuntime::IsOverlapMaskInMask128SSE(CRunObject *a, CRunObject *b, CollisionMask& coll1, CollisionMask& coll2)
{
	// Faster this way (!)
	return IsOverlapMaskInMask64MMX(a, b, coll1, coll2);

	/*
	const int algorithm_integral_bits = 64;				// 64 bit MMX

	bool ALeftOfB = (a->info.box.left <= b->info.box.left);

	// Swap A and B such that A is the leftmost object.
	OBJHEADER& infoA = (ALeftOfB ? a->info : b->info);
	OBJHEADER& infoB = (ALeftOfB ? b->info : a->info);

	CollisionMask& collA = (ALeftOfB ? coll1 : coll2);
	CollisionMask& collB = (ALeftOfB ? coll2 : coll1);
	
	RECTF& boxA = infoA.box;
	RECTF& boxB = infoB.box;

	bool AaboveB = (boxA.top <= boxB.top);

	__m128 *xa, *xb, *ya, *yb, *xEnd, *yEnd;
	BYTE *bitsA, *bitsB;
	
	// Overlapping rects relative to A and B
	RECT overlapA, overlapB;

	// Determine the relative overlap rectanges for A and B, knowing A is to the left of B
	overlapA.left = boxB.left - boxA.left;
	overlapB.left = 0;
	
	if (AaboveB) {
		overlapA.top = boxB.top - boxA.top;
		overlapB.top = 0;
	}
	// B above A
	else {
		overlapA.top = 0;
		overlapB.top = boxA.top - boxB.top;
	}

	if (boxB.right > boxA.right) {
		overlapA.right = boxA.right - boxA.left;
		overlapB.right = boxA.right - boxB.left;
	}
	else {
		overlapA.right = boxB.right - boxA.left;
		overlapB.right = boxB.right - boxB.left;
	}

	if (boxB.bottom > boxA.bottom) {
		overlapA.bottom = boxA.bottom - boxA.top;
		overlapB.bottom = boxA.bottom - boxB.top;
	}
	else {
		overlapA.bottom = boxB.bottom - boxA.top;
		overlapB.bottom = boxB.bottom - boxB.top;
	}

	// A is leftmost: use preshifted mask to shunt A's bits in line to B's.
	//A 11101110 against
	//B    00110011
	//A    01110??? shifted by 3 against
	//B	   00110011
	bitsA = collA.bits[overlapA.left % 8];
	bitsB = collB.bits[0];

	// Initialise pointers ready to run
	xa = (__m128*)(bitsA + (overlapA.top * collA.pitch) + (overlapA.left / 8));
	xb = (__m128*)(bitsB + (overlapB.top * collB.pitch) + (overlapB.left / 8));

	xEnd = xa + (overlapA.right - overlapA.left) / algorithm_integral_bits;

	// Make sure we hit additional pixels (eg. width 70 / 64 = 1, whereas we need 2 iters)
	if ((overlapA.right - overlapA.left) % algorithm_integral_bits != 0)
		xEnd++;

	int yastep = collA.pitch / (algorithm_integral_bits / 8);
	int ybstep = collB.pitch / (algorithm_integral_bits / 8);
	int yaEndDiff = yastep * (overlapA.bottom - overlapA.top);

	__m128 result;

	// For each horizontal step in the bitmask
	for ( ; xa != xEnd; xa++, xb++) {
		
		// Start Y pointers from X
		ya = xa;
		yb = xb;

		// Find end
		yEnd = ya + yaEndDiff;

		for ( ; ya != yEnd; ya += yastep, yb += ybstep) {

			// SSE 128-bit logical AND
			result = _mm_and_ps(*ya,*yb);

			// Native 32-bit check for collision
			if (result.m128_u32[0] || result.m128_u32[1] || result.m128_u32[2] || result.m128_u32[3]) {
				//_mm_empty();
				return true;
			}
		}
	}

	// Never found anything.  After all that!
	//_mm_empty();
	return false;
	*/
}




void CRuntime::AddMaskToMask(CollisionMask* src, CollisionMask* dest, int _x, int _y)
{
	const int algorithm_integral_bits = 64;				// 64 bit MMX

	CollisionMask* collA = src;
	CollisionMask* collB = dest;

	if(_x > dest->width - src->width)
		return;
	if(_y > dest->height - src->height)
		return;
	if(_x < 0)
		return;
	if(_y < 0)
		return;

	for(int i = 0; i < 8; i++)
	{
		int x = _x;
		int y = _y;

		__m64 *xa, *xb, *ya, *yb, *xEnd, *yEnd;

		if(x%8 != 0)
			x+= 8;
		if((x+i)%8 < x%8)
			x-= 8;

		BYTE* bitsA = collA->bits[7-(7+x-i)%8];
		BYTE* bitsB = collB->bits[i];

		xa = (__m64*)(bitsA);
		xb = (__m64*)(bitsB + (y * collB->pitch) + (x / 8));

		xEnd = xa + ((int)src->width) / algorithm_integral_bits;
		if ((int)src->width % algorithm_integral_bits != 0) // hit extra iteration
			xEnd++;

		int yastep = collA->pitch / (algorithm_integral_bits / 8);
		int ybstep = collB->pitch / (algorithm_integral_bits / 8);
		int yaEndDiff = yastep * collA->height;


		if(i < x)
		{
			BYTE* bitsA = collA->bits[0];
			BYTE* bitsB = collB->bits[i];
			bitsB += collB->pitch * y + (int)x/8-1;
			BYTE* bitsB_End = bitsB + collB->pitch * (int)collA->height;
			for(;bitsB != bitsB_End; bitsB += collB->pitch)
			{
				BYTE tmp = *bitsA;
				tmp = tmp >> (8-((x+i)%8));
				*bitsB |= tmp;

				bitsA += collA->pitch;
			}
		}



		
		// For each horizontal step in the bitmask
		for ( ; xa != xEnd; xa++, xb++) {
			
			// Start Y pointers from X
			ya = xa;
			yb = xb;

			// Find end
			yEnd = ya + yaEndDiff;

			for ( ; ya != yEnd; ya += yastep, yb += ybstep) {

				// MMX 64-bit logical AND
				*yb = _m_por(*ya,*yb);

			}
		}
		_mm_empty();
		//DbgCollisionMaskToFile(*dest, i, i);
	}



}

void CRuntime::ExportCollisionToFile(CollisionMask* mask, int index)
{
	for(int i = 0; i < 8; i++)
		DbgCollisionMaskToFile(*mask, i, index+i);
}

void CRuntime::GenerateCollisionMaskFromTexture(CRunObject* obj, TextureHandle th)
{
	if (th->is_multisampled)
		throw runtime_error("Cannot create collision masks from multisampled textures");

	renderer.EndBatch();

	CollisionMask*& generatedMask = obj->info.pInfo->generatedMask;

	// Free any existing generated mask
	if (generatedMask) {
		FreeMask(generatedMask);
		delete generatedMask;
	}

	generatedMask = new CollisionMask;
	generatedMask->boundingbox = 0;
	generatedMask->width = obj->info.w;
	generatedMask->height = obj->info.h;
	
	// Determine pitch
	const int align_pitch_bits = 64;
	const int align_pitch_bytes = align_pitch_bits / 8;

	int width = obj->info.w + 0.5f;
	int height = obj->info.h + 0.5f;

	int& pitch = generatedMask->pitch;
	pitch = width / 8;
	if (width % 8 != 0)
		pitch++;		// 11 pixel width needs 2 bytes not 1 rounded

	// Eg. a 20 byte pitch must round up to 32 (+12, 16 - 4)
	if (pitch % align_pitch_bytes != 0)
		pitch += align_pitch_bytes - (pitch % align_pitch_bytes);

	// If the pitch does not leave at least a 64 pixel gutter, increase it by 64 pixels.
	if ((pitch * 8) - width < align_pitch_bits)
		pitch += align_pitch_bytes;

	// Allocate bits and start with an empty mask
	generatedMask->mem_size = height * pitch + 16;
	generatedMask->bits[0] = (BYTE*)_aligned_malloc(generatedMask->mem_size, 16);
	
	memset(generatedMask->bits[0], 0, generatedMask->mem_size);

	HRESULT hr;

	// Create the system memory surface to copy the texture to
	IDirect3DSurface9* pSystemSurface = NULL, *pSourceSurface = NULL;
	D3DLOCKED_RECT lockRect;

	// if the texture is a render target, we need to copy the render target data
	if (th->rendertarget) {
		hr = renderer.GetDevice()->CreateOffscreenPlainSurface(width, height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pSystemSurface, NULL);

		// Obtain surface for the texture
		hr = th->texture_ptr->GetSurfaceLevel(0, &pSourceSurface);

		// Get the texture data
		hr = renderer.GetDevice()->GetRenderTargetData(pSourceSurface, pSystemSurface);

		// Obtain data from the system surface
		hr = pSystemSurface->LockRect(&lockRect, NULL, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_READONLY);
	}
	// If the texture isn't a render target, we can just grab the top mip level
	else {
		hr = th->texture_ptr->GetSurfaceLevel(0, &pSourceSurface);

		// Obtain data from the texture surface
		hr = pSourceSurface->LockRect(&lockRect, NULL, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_READONLY);
	}

	int byteIndex = 0;
	BYTE* pBits = (BYTE*)lockRect.pBits;
	BYTE* maskBits = generatedMask->bits[0];

	// Iterate over the data
	for (int y = 0; y < height; y++) {

		byteIndex = y * lockRect.Pitch + 3;

		// Iterate each alpha byte
		for (int x = 0; x < width; x++, byteIndex += sizeof(int)) {

			// Extract alpha byte
			BYTE a = pBits[byteIndex];
			a = (a != 0) ? 1 : 0;

			// Set the corresponding mask bit
			maskBits[y * pitch + x / 8] |= a << (7 - (x % 8));
		}
	}

	if (pSystemSurface)
		pSystemSurface->UnlockRect();

	pSourceSurface->Release();

	if (pSystemSurface)
		pSystemSurface->Release();

	GenerateShiftedMasks(*generatedMask);

	//DbgCollisionMaskToFile(*generatedMask, 0, 1);
}

float CRuntime::CalculateBounceAngle(CRunObject* obj, float input_angle, CRunObjType* bounceOff)
{
	// work out the angle
	float x = obj->info.x;
	float y = obj->info.y;

	// Rounding improves bounce reliability
	x = floor(x+0.5);
	y = floor(y+0.5);

	const float PI = 3.1415926535897932384626433832795;
	const float col_radius = 16.0;
	const float col_count = PI * col_radius * 2;
	const float col_half_count = col_count / 2.0;
	
	float col_vector_x = 0;
	float col_vector_y = 0;

	int found_count = 0;

	for(float a = 0; a <= col_count; a++)
	{
		float angle = (a * 2 * PI) / col_count;

		float sin_a, cos_a;
		cr::sincosf(angle, &sin_a, &cos_a);

		float ox = cos_a * col_radius;
		float oy = sin_a * col_radius;

		obj->info.x = x + ox;
		obj->info.y = y + oy;
		UpdateBoundingBox(obj);
		if(QueryCollision(obj, bounceOff))
		{
			col_vector_x += ox;
			col_vector_y += oy;
			found_count++;
		}
	}

	obj->info.x = x;
	obj->info.y = y;
	UpdateBoundingBox(obj);					

	float normal;

	if (found_count == 0)
		normal = input_angle + PI;	// normal opposes the motion
	else
		normal = atan2(col_vector_y, col_vector_x) + PI;

	// Using reflected vector r = v - 2 (v . n) n
	float sin_a, cos_a;
	cr::sincosf(input_angle, &sin_a, &cos_a);
	float vx = cos_a;
	float vy = sin_a;
	cr::sincosf(normal, &sin_a, &cos_a);
	float nx = cos_a;
	float ny = sin_a;

	float v_dot_n = vx * nx + vy * ny;

	float rx = vx - 2 * v_dot_n * nx;
	float ry = vy - 2 * v_dot_n * ny;

	// Compute angle of reflected vector and return
	return atan2(ry, rx);
}

//map<CRunObject*, ObjList> registeredCollisions;

// 'On collision' in the Sprite object is really an 'Is overlapping' with a 'Trigger once' bolted on to it.
// This means it misses collisions happening in behaviors which then move the object off the obstacle.
// Behaviors doing that can call RegisterCollision to make a temporary, one-tick record that a collision happened,
// which Sprite also checks for.
void CRuntime::RegisterCollision(CRunObject* a, CRunObject* b)
{
	if (a == NULL || b == NULL) return;

	// Add both way rounds so we can look up by object immediately for whichever object
	registeredCollisions[a].push_back(b);
	registeredCollisions[b].push_back(a);
}

bool CRuntime::QueryRegisterCollision(CRunObject* obj, CRunObjType* type)
{
	if (obj == NULL || type == NULL) return false;

	// Loop all instances of "type", and check them
	ObjIterator i = type->instances.begin();
	ObjConstIterator instances_end = type->instances.end();

	bool toReturn = false;

	for ( ; i != instances_end; i++) {
		if (obj == *i) continue;

		// If colliding register the collision
		if (QueryCollision(obj, *i)) {
			toReturn = true;
			RegisterCollision(obj, *i);
		}
	}

	return toReturn;
}

// Select any objects that have previously registered collisions as per the collision query
bool CRuntime::SelectRegisteredCollisions(CRunObject* obj, CRunObjType* pType)
{
	// Ensure a record exists
	if (registeredCollisions.empty())
		return false;

	map<CRunObject*, ObjList>::iterator i = registeredCollisions.find(obj);

	if (i == registeredCollisions.end())
		return false;

	// Found a list of object instances it registered collisions with
	ObjList& objList = i->second;

	// For each instance 'obj' registered a collision with
	ObjIterator o = objList.begin();
	ObjIterator objList_end = objList.end();

	bool toReturn = false;

	for ( ; o != objList_end; o++) {

		// If pType is a family, we have to check that o is in that family
		if (pType->IsFamily()) {

			if (exists_element((*o)->pType->teams.begin(), (*o)->pType->teams.end(), pType)) {

				toReturn = true;

				if (!IsSelected(*o, pType))
					Select(*o, pType);
			}
		}
		// If pType isn't a family, we can check o's pType directly
		else if ((*o)->pType == pType) {

			toReturn = true;

			if (!IsSelected(*o, (*o)->pType))
				Select(*o, (*o)->pType);
		}
	}

	return toReturn;
}





#else // for app runtime, empties

bool CRuntime::QueryCollision(CRunObject *a, CRunObject *b)
{	return false;	}

bool CRuntime::QueryCollision(CRunObject *obj, CRunObjType *type)
{	return false;	}

bool CRuntime::QuerySelectCollision(CRunObject *obj, CRunObjType *type)
{	return false;	}

void CRuntime::FreeMask(CollisionMask* m)
{ }

bool CRuntime::QuerySelectCollision(CRunObject *a, CRunObject *b, CRunObjType *b_type)
{	return false;	}

bool CRuntime::QueryPointCollision(int x, int y, CRunObject *obj)
{	return false;	}

bool CRuntime::QueryPointCollision(int x, int y, CRunObjType *type)
{	return false;	}

bool CRuntime::QuerySelectPointCollision(int x, int y, CRunObject *obj, CRunObjType* pType)
{	return false;	}

bool CRuntime::QuerySelectPointCollision(int x, int y, CRunObjType *type)
{	return false;	}

bool CRuntime::QueryScreenPointCollision(int x, int y, CRunObjType* type)
{	return false;	}

bool CRuntime::QueryScreenPointInstanceCollision(int x, int y, CRunObject* obj)
{	return false;	}

bool CRuntime::QuerySelectScreenPointCollision(int x, int y, CRunObjType* type)
{	return false;	}

void CRuntime::GenerateCollisionMaskFromTexture(class CRunObject *, TextureHandle th)
{					}

float CRuntime::CalculateBounceAngle(CRunObject* _This, float input_angle, CRunObjType* bounceOff)
{	return 0.0f;	}

void CRuntime::RegisterCollision(CRunObject* a, CRunObject* b)
{					}

bool CRuntime::QueryRegisterCollision(CRunObject* a, CRunObjType* b)
{	return false;	}

bool CRuntime::SelectRegisteredCollisions(CRunObject* obj, CRunObjType* pType)
{	return false;	}

#endif