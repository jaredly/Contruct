// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"

LayoutPfMap::LayoutPfMap()
{
	for (int i = 0; i < 10; i++) {
		pfMap[i] = NULL;
		instanceCount[i] = 0;
	}
}

// Possible pathfinding maps for any active layout
static map<CRunLayout*, LayoutPfMap> pfLayoutMaps;
__int64 ExtObject::lastAvoidanceUpdate = -1; // for writing higher cell values for where units are sitting

#ifndef M_PI_2
#define M_PI_2 1.570796327
#endif

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

// ExtObject constructor:
// Only use for class initializer list.  Object initialisation must be done in OnCreate.
// It is not safe to make runtime calls here: do so in OnCreate.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
{
	pRuntime = pVRuntime;
	info.editObject = editObject;
}

int CellSizeCombo[] = {
	10,
	20,
	30,
	40,
	60,
	80,
	100,
	150,
	200,
	300
};

// This is called just after the constructor when your object has been created.  Construct has set
// up your object here so it is safe to make runtime calls.
void ExtObject::OnCreate()
{
	speed = 0.0f;
	moving = false;
	params.blockedspeed = 75.0f;

	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	int Version = 0;
	ar >> Version;

	params.cellsize = 4; // 60x60
	ar >> params.acc >> params.dec >> params.maxspeed >> params.av;
	
	if (Version < 4) {
		bool oldPathfinding;
		ar >> oldPathfinding;

		if (oldPathfinding)
			params.pathfinding = 1;		// Around 'Solid'
		else params.pathfinding = 0;		// Off
	}
	else
		ar >> params.pathfinding;

	ar >> params.rotate;
	
	if (Version >= 2)
		ar >> params.blockedspeed;
	if (Version >= 3)
		ar >> params.cellsize;

	ar.detach();

	// Momentum starts at object angle
	mAngle = RADIANS(pLink->info.angle);

	// Convert angular velocity to radians
	params.av = RADIANS(params.av);

	info.collMode = COLLISIONMODE_BOX;	// For cell collision queries

	pfcellW = CellSizeCombo[params.cellsize];
	pfcellH = CellSizeCombo[params.cellsize];
	pfNumHCells = (pLayout->width / pfcellW) + 1;
	pfNumVCells = (pLayout->height / pfcellH) + 1;

	quickStop = false;
	activated = true;

	LayoutPfMap& lpf = pfLayoutMaps[pLayout];
	lpf.instanceCount[params.cellsize]++;

	if (params.pathfinding == 1)	// Avoid 'solid'
		pfCollideTypes.push_back(pRuntime->GetTraitFromName("Solid"));

	refreshGridNextTick = true;
	delayed.moveCommand = false;
	delayed.waypointCommand = false;
	//lastAvoidanceUpdate = -1;

	speedRet = 0.0f;
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
	LayoutPfMap& lpf = pfLayoutMaps[pLayout];

	lpf.instanceCount[params.cellsize]--;

	if (lpf.pfMap[params.cellsize] != NULL && lpf.instanceCount[params.cellsize] == 0) {
		delete[] lpf.pfMap[params.cellsize];
		lpf.pfMap[params.cellsize] = NULL;
	}
}

// inputs are in degrees
float RotateTowards_deg(float start, float end, float step)
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

// inputs are in radians
float RotateTowards_rad(float start, float end, float step)
{
   if (step == 0) return start;

   float cs = cos(start);
   float ce = cos(end);
   float ss = sin(start);
   float se = sin(end);

   float z = cs*se - ss*ce;
   float dot = cs*ce + ss*se;
   float angle = acos(dot);
  
   if(angle > step)
      if(z > 0)
         return start + step;
      else
         return start - step;
   else
      return end;
}

bool AngleWithin_rad(float start, float end, float diff)
{
	return RotateTowards_rad(start, end, diff) == end;
}

// inputs are in radians
float GetAngleDiff_rad(float start, float end)
{
   float cs = cos(start);
   float ce = cos(end);
   float ss = sin(start);
   float se = sin(end);

   float dot = cs*ce + ss*se;
   return acos(dot);
}

bool ExtObject::QueryCellCollision(int cellX, int cellY)
{
	// Get coords of this cell
	RECTF& box = info.box;

	box.left = cellX * pfcellW;
	box.top = cellY * pfcellH;
	box.right = box.left + pfcellW;
	box.bottom = box.top + pfcellH;

	// Now box collision check the collision types
	ObjTypeIterator t = pfCollideTypes.begin();
	ObjTypeConstIterator end = pfCollideTypes.end();

	for ( ; t != end; t++) {
		if (pRuntime->QueryCollision(this, *t))
			return true;
	}

	return false;
}

void ExtObject::PathfindingMoveOutOfSolids()
{
	if (params.pathfinding == 0) return;

	pLink->UpdateBoundingBox();

	// Is overlapping a solid
	if (PathfindingIsOverlapping()) {

		// Try moving out in the positive and negative x and y directions.
		// Stop at whichever is not colliding first.
		float offset = 1.0f;
		int dir = 0;
		float origX = pLink->info.x;
		float origY = pLink->info.y;

		do {
			switch (dir) {
			case 0:
				pLink->info.x = origX - offset;
				pLink->info.y = origY;
				break;
			case 1:
				pLink->info.x = origX + offset;
				pLink->info.y = origY;
				break;
			case 2:
				pLink->info.x = origX;
				pLink->info.y = origY - offset;
				break;
			case 3:
				pLink->info.x = origX;
				pLink->info.y = origY + offset;
				break;
			}

			dir++;
			if (dir >= 4) {
				dir = 0;
				offset++;
			}

			pRuntime->UpdateBoundingBox(pLink);
		}
		while (PathfindingIsOverlapping());
	}

}

bool ExtObject::PathfindingSolidCollision()
{
	if (params.pathfinding != 0) {
		pLink->UpdateBoundingBox();
		return PathfindingIsOverlapping();
	}
	
	// Not needed for non pathfinding mode
	return false;
}

bool ExtObject::PathfindingIsOverlapping()
{
	// Check all the collision types
	ObjTypeIterator t = pfCollideTypes.begin();
	ObjTypeConstIterator end = pfCollideTypes.end();

	for ( ; t != end; t++) {
		if (pRuntime->QueryCollision(pLink, *t))
			return true;
	}

	return false;
}

/*
bool ExtObject::PathfindingIsOverlappingUnits()
{
	if (PathfindingIsOverlapping())
		return true;

	else if (!typesAvoid.empty()) {
		vector<CRunObjType*>::iterator t = typesAvoid.begin();
		vector<CRunObjType*>::iterator end = typesAvoid.end();

		for ( ; t != end; t++) {
			if (pRuntime->QueryCollision(pLink, *t))
				return true;
		}
	}

	return false;
}
*/

bool PointInBox(const RECTF& box, float x, float y)
{
	return x > box.left && x < box.right && y > box.top && y < box.bottom;
}

// Called every frame, before the events and after drawing, for you to update your object if necessary
// Return 1 (do not call again) or 0 (continue calling)
BOOL ExtObject::OnFrame()
{
		// If delayed move command, run now
	if (delayed.moveCommand) {

		// Move commands issued on Start of Layout need the map to be created, by default it's done in OnFrame2
		if (!MapValid()) {
				cell_t*& curMap = pfLayoutMaps[pLayout].pfMap[params.cellsize];

			// Allocate the map if one isnt made yet
			if (curMap == NULL)
				curMap = new cell_t[pfNumHCells * pfNumVCells];

			GeneratePFMap();
		}

		// Don't do this more than once a tick
		if (lastAvoidanceUpdate < pLayout->frameCounter64) {
			UpdateAvoidanceMap();
			lastAvoidanceUpdate = pLayout->frameCounter64;
		}

		if (CalculatePath(delayed.targetX, delayed.targetY, true)) {
			moving = true;
			targetX = delayed.targetX;
			targetY = delayed.targetY;
		}
		delayed.moveCommand = false;
	}
	if (delayed.waypointCommand) {

		// Don't do this more than once a tick
		if (lastAvoidanceUpdate < pLayout->frameCounter64) {
			UpdateAvoidanceMap();
			lastAvoidanceUpdate = pLayout->frameCounter64;
		}

		if (CalculatePath(delayed.targetX, delayed.targetY)) {
			moving = !waypoints.empty();
			targetX = delayed.targetX;
			targetY = delayed.targetY;
		}
		delayed.waypointCommand = false;
	}

	// Current target: first waypoint
	CPoint* curTarget = NULL;

	if (!waypoints.empty())
		curTarget = &waypoints.front();

	float curMaxSpeed = params.maxspeed;
	float curAV = params.av;
	float timeDelta = pRuntime->GetTimeDelta();

	// When moving
	if (moving) {

		// Get angle to target
		float targetAngle = atan2(curTarget->y - pLink->info.y, curTarget->x - pLink->info.x);

		// Check for types to avoid
		if (!typesAvoid.empty()) {

			// Calculate co-ordinates of various edges
			RECTF& box = pLink->info.box;
			float halfW = (box.right - box.left) / 2;
			float halfH = (box.bottom - box.top) / 2;
			float centerX = box.left + halfW;
			float centerY = box.top + halfH;
			int frontX = centerX + cos(mAngle) * (halfW + 10);
			int frontY = centerY + sin(mAngle) * (halfW + 10);

			// Loop all avoidance types
			ObjTypeIterator t = typesAvoid.begin();
			ObjTypeConstIterator typesAvoid_end = typesAvoid.end();

			for ( ; t != typesAvoid_end; t++) {

				// Loop every instance in this type
				int count;
				CRunObject** instances;
				pRuntime->GetTypeInstances(*t, instances, count);

				if (count <= 0) continue;

				bool frontblocked = false;

				// Object blocked by object in front: slow to blocked speed.
				if (pRuntime->QueryPointCollision(frontX, frontY, *t))
					frontblocked = true;

				// Loop and check for instances overlapping
				CRunObject** i = instances;
				CRunObject** end = instances + count;

				bool blocked = false;
				bool bodyblocked = false;
				bool overlappinganystopped = false;
				float overlapasX, overlapasY;

				for ( ; i != end; i++) {
					if (*i == pLink) continue;

					// Find if this object has an RTS movement
					CRunObject** siblings;
					int sibcount;
					pRuntime->GetObjectSiblings(*i, siblings, sibcount);

					if (sibcount <= 0) continue;

					CRunObject** s = siblings;
					CRunObject** sibs_end = siblings + sibcount;

					for ( ; s != sibs_end; s++) {
						// Also an RTS movement on this object
						if ((*s)->pType->pPlugin == pType->pPlugin) {

							// If these objects are heading for the same target, but the one i've bumped in to has arrived - stop
							ExtObject* rts = (ExtObject*)(*s);

							if (pRuntime->QueryCollision(pLink, *i)) {

								// Overlapping a moving unit
								if (rts->moving) {

									
									// Find angle to target
									float dx_to_them = rts->pLink->info.x - pLink->info.x;
									float dy_to_them = rts->pLink->info.y - pLink->info.y;
									float angle_to_them = atan2(dy_to_them, dx_to_them);

									float dx_to_me = pLink->info.x - rts->pLink->info.x;
									float dy_to_me = pLink->info.y - rts->pLink->info.y;
									float angle_to_me = atan2(dy_to_me, dx_to_me);

									bool theyre_infrontof_me = AngleWithin_rad(mAngle, angle_to_them, RADIANS(90));
									bool im_infrontof_them = AngleWithin_rad(rts->mAngle, angle_to_me, RADIANS(90));

									if (!im_infrontof_them && theyre_infrontof_me)
										curMaxSpeed = 0;
									else if (im_infrontof_them && theyre_infrontof_me) {
										if (speed < rts->speed)
											curMaxSpeed = 0;
										if (speed == rts->speed && pLink->info.uid < rts->pLink->info.uid)
											curMaxSpeed = 0;
									}
									else if (!im_infrontof_them && !theyre_infrontof_me) {
										if (speed < rts->speed)
											curMaxSpeed = 0;
										if (speed == rts->speed && pLink->info.uid < rts->pLink->info.uid)
											curMaxSpeed = 0;
									}
									
								}
								// Other object is stopped: try to move around it
								else {
									overlappinganystopped = true;
									overlapasX = rts->pLink->info.x;
									overlapasY = rts->pLink->info.y;
								}

								// Going to same target
								if (targetX == rts->targetX && targetY == rts->targetY && !rts->moving) {

									moving = false;		// Stop
									curMaxSpeed = 0;	// We've bumped in to the arrived object and dont want to
														// overlap it too much
									if (speed < 0) speed = 0;
									pRuntime->GenerateEvent(info.oid, 1, this);	// On Arrived
								}

							}

							break;
						}
					}

				}//for

				if (overlappinganystopped) {
					// Slow down and rotate to avoid unit
					float angle_to_blockage = atan2(overlapasY - pLink->info.y, overlapasX - pLink->info.x);

					targetAngle = RotateTowards_rad(targetAngle, angle_to_blockage, RADIANS(-45));
					curMaxSpeed = params.blockedspeed;
				}
			}// if types to avoid
		}

		mAngle = RotateTowards_rad(mAngle, targetAngle, curAV * timeDelta);

		// Update the object's display angle
		if (params.rotate) {
			pLink->info.angle = DEGREES(mAngle);
			pLink->UpdateBoundingBox();
			PathfindingMoveOutOfSolids();
		}

		// Check if we need to slowdown on the target approach
		float dx = pLink->info.x - curTarget->x;
		float dy = pLink->info.y - curTarget->y;
		float distToTarget = sqrt(dx*dx + dy*dy);

		// Calculate the distance at which the object must begin to slow down (summation of finite series)
		// (m^2) / (2d)
		float distToSlow = (params.maxspeed * params.maxspeed) / (2 * params.dec);

		float angleDiff = DEGREES(GetAngleDiff_rad(mAngle, targetAngle));

		// I am facing more than 60 degrees from my target: manouevre
		if (angleDiff > 60.0f)
			curMaxSpeed = params.blockedspeed;

		// More than 135 degrees off: might be going to miss target - go really slow
		if (angleDiff > 135.0f)
			curMaxSpeed = params.blockedspeed / 3;

		// I am within stopping distance of this target
		if (distToTarget <= distToSlow) {

			// This is the last waypoint, so I am coming to a halt: set max speed
			if (waypoints.size() == 1)
				curMaxSpeed = min(curMaxSpeed, (distToTarget / distToSlow) * params.maxspeed);

			if (angleDiff > 60.0f)
				curMaxSpeed = 0;//min(curMaxSpeed, blockedspeed / 3);
		}
	}

	if (speed > 0 || moving) {

		float dx = cos(mAngle) * speed * timeDelta;
		float dy = sin(mAngle) * speed * timeDelta;

		float oldx = pLink->info.x;
		float oldy = pLink->info.y;

		bool movedX = true;
		bool movedY = true;

		// Try X movement
		pLink->info.x += dx;

		if (PathfindingSolidCollision()) {

			// Moved in to something
			pLink->info.x = oldx;			// Move back
			pLink->UpdateBoundingBox();
			movedX = false;
		}

		// Try Y movement
		pLink->info.y += dy;

		if (PathfindingSolidCollision()) {
			
			// Moved in to something
			pLink->info.y = oldy;			// Move back
			pLink->UpdateBoundingBox();
			movedY = false;
		}

		// Stuck!  Didn't move at all.  Reverse direction as a last ditch attempt.
		if (!movedX && !movedY)
			mAngle += RADIANS(180);
	}

	// Handle acceleration & deceleration
	if (moving && speed < curMaxSpeed) {

		if (params.acc < 0)
			speed = curMaxSpeed;
		else {
			speed += params.acc * timeDelta;
			if (speed > curMaxSpeed)
				speed = curMaxSpeed;
		}
	}

	if (speed > curMaxSpeed)
		speed = curMaxSpeed;

	speedRet = speed;

	if (!moving && speed > 0) {

		if (dec < 0)
			speed = 0;
		else {
			speed -= params.dec * timeDelta;
			if (speed < 0)
				speed = 0;
		}
	}

	// Check if I have arrived.  Use a point collision test, but temporarily override the collision
	// mode of the object to BOX if 'none' or 'hotspot', else it'll probably fail.
	if (moving) {
		
		float dx = pLink->info.x - curTarget->x;
		float dy = pLink->info.y - curTarget->y;

		bool hasArrived;

		// Last waypoint: move to within 10px
		if (waypoints.size() == 1)
			hasArrived = dx*dx + dy*dy < 10*10;
		else
			hasArrived = PointInBox(pLink->info.box, curTarget->x, curTarget->y);

		if (hasArrived) {
			
			// Remove this waypoint
			waypoints.pop_front();

			// On Waypoint event
			pRuntime->GenerateEvent(info.oid, 2, this);		// On Waypoint

			// No other waypoints: come to a stop
			if (waypoints.empty()) {
				moving = false;
				pRuntime->GenerateEvent(info.oid, 1, this);	// On Arrived
			}
		}
	}

	return !activated;
}

void ExtObject::IncrementCell(float x, float y)
{
	cell_t* pfMap = pfLayoutMaps[pLayout].pfMap[params.cellsize];
	int cellX = x / pfcellW;
	int cellY = y / pfcellH;

	if (cellX < 0 || cellY < 0 || cellX >= pfNumHCells || cellY >= pfNumVCells)
		return;

	cell_t& curCell = pfMap[(cellY * pfNumHCells) + cellX];

	if (curCell + PFCELL_OCCUPIED_COST < PFCELL_IMPASSABLE - 1)
		curCell += PFCELL_OCCUPIED_COST;
}

// Update the pathfinding map including higher cell costs where units sit
void ExtObject::UpdateAvoidanceMap()
{
	cell_t* curMap = pfLayoutMaps[pLayout].pfMap[params.cellsize];

	// Reset all cells to their default zero value
	pathfinder.ClearCellCosts();

	// Iterate all instances of the types to avoid and if they are stopped, increase the cell cost
	ObjTypeIterator t = typesAvoid.begin();
	ObjTypeConstIterator typesAvoid_end = typesAvoid.end();

	for ( ; t != typesAvoid_end; t++) {
		int count;
		CRunObject** instances;
		pRuntime->GetTypeInstances(*t, instances, count);

		if (count <= 0) continue;

		CRunObject** end = instances + count;
		CRunObject** i = instances;

		for ( ; i != end; i++) {

			// Get siblings to look for RTS
			int siblingCount;
			CRunObject** siblings;
			pRuntime->GetObjectSiblings(*i, siblings, siblingCount);

			if (siblingCount <= 0)
				continue;

			CRunObject** siblings_end = siblings + siblingCount;
			CRunObject** s = siblings;

			for ( ; s != siblings_end; s++) {
				
				// Is an RTS movement?
				if ((*s)->pType->pPlugin == pType->pPlugin) {
					
					// Check its compatible and is using the same map and is stopped
					ExtObject* rts = (ExtObject*)(*s);

					if (rts->params.cellsize != params.cellsize || rts->pLayout != pLayout || rts->moving || rts->delayed.moveCommand || rts->delayed.waypointCommand)
						continue;

					const RECTF& box = rts->pLink->info.box;

					// Increment each of the corners in case covering multiple cells
					IncrementCell(box.left, box.top);
					IncrementCell(box.right, box.top);
					IncrementCell(box.left, box.bottom);
					IncrementCell(box.right, box.bottom);
				}
			}
		}
	}
}

void ExtObject::GeneratePFMap()
{
	cell_t* curMap = pfLayoutMaps[pLayout].pfMap[params.cellsize];

	for (int x = 0; x < pfNumHCells; x++) {
		for (int y = 0; y < pfNumVCells; y++) {

			// Check if cell is overlapping an obstacle
			curMap[(y * pfNumHCells) + x] = (QueryCellCollision(x, y) ? PFCELL_IMPASSABLE : PFCELL_CLEAR);

		}
	}

	pathfinder.SetMap(curMap, pfNumHCells, pfNumVCells);	
}

// Called every frame, after the events and before drawing, for you to update your object if necessary
// Return 1 (do not call again) or 0 (continue calling)
// It is not safe to destroy objects in OnFrame2().  If you have to do this, use OnFrame().
// If you are storing any pointers to CRunObjects, check the info.destroying flag here.  If it is true,
// you must reset the pointer to NULL as it is no longer valid.
BOOL ExtObject::OnFrame2()
{
	LayoutPfMap& lpf = pfLayoutMaps[pLayout];

	// Allocate the map if one isnt made yet
	if (lpf.pfMap[params.cellsize] == NULL)
		lpf.pfMap[params.cellsize] = new cell_t[pfNumHCells * pfNumVCells];

	// Generate pathfinding map
	// We do this here because not all Solids will have been created in OnCreate
	if (params.pathfinding != 0 && refreshGridNextTick) {

		GeneratePFMap();

	}

	refreshGridNextTick = false;

	// Call again
	return 0;
}

bool ExtObject::MapValid()
{
	return pfLayoutMaps[pLayout].pfMap[params.cellsize] != NULL;
}

bool ExtObject::CellIsImpassable(int x, int y)
{
	// Everything off the map is an obstacle
	if (x < 0 || x >= pfNumHCells || y < 0 || y >= pfNumVCells)
		return true;

	return pfLayoutMaps[pLayout].pfMap[params.cellsize][(y * pfNumHCells) + x] == PFCELL_IMPASSABLE;
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
	switch (id) {
	// If your extension uses a window, you can return it here
	case 1337:
		return (long)(pfLayoutMaps[pLayout].pfMap[params.cellsize]);
	case 1338:
		return (long)&waypoints;
	case 1339:
		return pfcellW;
	case 1340:
		return pfcellH;
	}

	return 0;
}

long ExtObject::CallFunction(int id, void* param)
{
	switch (id) {

		// Write speed to param
	case CF_GETSPEED:
		*(float*)param = speed;
		break;
	}

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