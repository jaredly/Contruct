// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

bool ExtObject::QueryObstaclePoint(float x, float y)
{
	ObjTypeIterator t = obstacles.begin();
	ObjTypeIterator end = obstacles.end();

	for ( ; t != end; t++) {

		// Obstacle found
		if (pRuntime->QueryPointCollision(x, y, *t))
			return true;
	}

	// No obstacle found
	return false;
}

bool ExtObject::QueryObstacleBox(float x, float y)
{
	ObjTypeIterator t = obstacles.begin();
	ObjTypeIterator end = obstacles.end();

	for ( ; t != end; t++) {

		float r = resolution / 2;

		// Set up bounding box about x,y
		info.x = x - r;
		info.y = y - r;
		info.w = resolution;
		info.h = resolution;
		pRuntime->UpdateBoundingBox(this);

		// Obstacle found
		if (pRuntime->QueryCollision(this, *t))
			return true;
		
	}

	// No obstacle found
	return false;
}

float radAngleDiff(float a, float b)
{
	float ca = cos((a));
	float cb = cos((b));
	float sa = sin((a));
	float sb = sin((b));

	float dot = ca*cb + sa*sb;
	return acos(dot);
}



bool ExtObject::LOSToPos(float endX, float endY)
{
	// Get start X/Y
	float x = pLink->info.x;
	float y = pLink->info.y;

	float dx = endX - x;
	float dy = endY - y;

	float distToTarget = sqrt(dx*dx + dy*dy);
	float angleToTarget = atan2(dy, dx);
	float angleDiff = radAngleDiff(angleToTarget, RADIANS(pLink->info.angle));

	if(distToTarget > range && range > 0)
		return false;
	if(angleDiff > RADIANS(angleRange) / 2)
		return false;

	// 'resolution' is the step distance, so divide the LOS test in to that many slices
	int numSteps = distToTarget / resolution;

	if (numSteps == 0) return true;	// Must be very close

	// Work out the step increments on X and Y axes by lerping (no need for trig)
	float incX = dx / (float)numSteps;
	float incY = dy / (float)numSteps;

	// For every step
	for (int i = 0; i < numSteps; i++, x += incX, y += incY) {

		// Point tests at 1px
		if (resolution == 1) {
			if (QueryObstaclePoint(x, y))
				return false;
		}
		// Box tests at >1px
		else {
			if (QueryObstacleBox(x, y))
				return false;
		}
		
	}

	return true;
}


long ExtObject::cHasLOSToObj(LPVAL theParams)
{
	CRunObject* pObj = theParams[0].GetPairedObjectParam(pRuntime, this);

	if (pObj == NULL) return false;

	return LOSToPos(pObj->info.x, pObj->info.y);
}

long ExtObject::cHasLOSToPos(LPVAL theParams)
{
	float x = theParams[0].GetFloat();
	float y = theParams[1].GetFloat();

	return LOSToPos(x, y);
}

long ExtObject::cOnLOSToObj(LPVAL theParams)
{
	CRunObject* pObj = theParams[0].GetPairedObjectParam(pRuntime, this);

	if (pObj == NULL) return false;

	return MakeConditionTrigger(pRuntime, LOSToPos(pObj->info.x, pObj->info.y));
}

long ExtObject::cOnLOSToPos(LPVAL theParams)
{
	float x = theParams[0].GetFloat();
	float y = theParams[1].GetFloat();

	return MakeConditionTrigger(pRuntime, LOSToPos(x, y));
}

long ExtObject::aClearObstacles(LPVAL theParams)
{
	obstacles.resize(0);
	return 0;
}
