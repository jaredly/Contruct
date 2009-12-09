// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aStop			(LPVAL theParams)
{
	// Turn off moving flag
	moving = false;
	delayed.moveCommand = false;
	delayed.waypointCommand = false;

	// Prevent other units bumping in to this unit and thinking this unit has arrived
	targetX = pRuntime->Random();
	targetY = pRuntime->Random();
	return 0;
}

long ExtObject::aSetSpeed		(LPVAL theParams)
{
	speed = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetMaxSpeed	(LPVAL theParams)
{
	params.maxspeed = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetAcc		(LPVAL theParams)
{
	params.acc = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetDec		(LPVAL theParams)
{
	params.dec = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetMAngle		(LPVAL theParams)
{
	mAngle = RADIANS(theParams[0].GetDouble());
	return 0;
}

bool ExtObject::CalculatePath(int targX, int targY, bool bClearWaypoints)
{
	CPoint target(targX / pfcellW, targY / pfcellH);
	CPoint curPos;

	if (!MapValid()) return false;

	// Sent to an obstacle cell: find the nearest cell that is not an obstacle and move there instead.
	if (CellIsImpassable(target.x, target.y)) {

		int bestSqDist = 1000000;
		int bestx = 0, besty = 0;
		
		// This could be a better algorithm, but hey.
		for (int x = 0; x < pfNumHCells; x++) {
			for (int y = 0; y < pfNumVCells; y++) {

				// For each non obstacle cell
				if (!CellIsImpassable(x, y)) {

					int dx = target.x - x;
					int dy = target.y - y;
					int curSqDist = dx*dx + dy*dy;

					if (curSqDist < bestSqDist) {
						bestSqDist = curSqDist;
						bestx = x;
						besty = y;
					}
					
				}
			}//for
		}//for

		target.x = bestx;
		target.y = besty;

		// Also modify the object destination
		targX = targetX = target.x * pfcellW + (pfcellW / 2);
		targY = targetY = target.y * pfcellH + (pfcellH / 2);
	}

	// No waypoints - take movement from object position
	if (bClearWaypoints) {
		curPos.x = pLink->info.x / pfcellW;
		curPos.y = pLink->info.y / pfcellH;
	}
	// Else take movement from last waypoint position
	else {
		curPos = waypoints.back();
		curPos.x /= pfcellW;
		curPos.y /= pfcellH;
	}

	// Find a path
	if (pathfinder.FindPath(curPos, target)) {

		if (bClearWaypoints)
			waypoints.resize(0);

		// Add end waypoint
		waypoints.push_back(CPoint(targX, targY));

		list<CPoint>::iterator insertPoint = waypoints.end();
		insertPoint--;

		// Create waypoints along the route
		CPathfinder::Node* curNode = pathfinder.pathEnd.parent;

		NodeDir oldDir = NODEDIR_NONE;

		if (curNode != NULL) {

			oldDir = pathfinder.DirToNode(&(pathfinder.pathEnd), curNode);

			while (curNode->parent != NULL) {

				pathfinder.AddToCellUsage(curNode->cell.x, curNode->cell.y);

				CPoint curPt;
				curPt.x = curNode->cell.x * pfcellW + (pfcellW / 2);
				curPt.y = curNode->cell.y * pfcellH + (pfcellH / 2);

				// If the direction has changed
				if (oldDir != pathfinder.DirToNode(curNode, curNode->parent)) {

					// Place a waypoint
					insertPoint = waypoints.insert(insertPoint, curPt);
					oldDir = pathfinder.DirToNode(curNode, curNode->parent);

				}

				curNode = curNode->parent;
			}//while
		}//if

		return true;
	}
	// No path found
	else {
		pRuntime->NewSOL();
		pRuntime->GenerateEvent(info.oid, 3, this);		// On failed to find path
		pRuntime->RestoreSOL();
		return false;
	}
}

long ExtObject::aMoveToXY(LPVAL theParams)
{
	if (params.pathfinding) {
		//if (FindPath(targetX, targetY, true))
		//	moving = true;
		delayed.moveCommand = true;
		delayed.targetX = theParams[0].GetFloat();
		delayed.targetY = theParams[1].GetFloat();
	}
	else {
		targetX = theParams[0].GetFloat();
		targetY = theParams[1].GetFloat();

		moving = true;
		waypoints.resize(0);
		waypoints.push_back(CPoint(targetX, targetY));
	}
	return 0;
}

long ExtObject::aMoveToObj(LPVAL theParams)
{
	CRunObject* obj = theParams[0].GetObjectParamFirstInstance(pRuntime);
	if (obj == NULL) return 0;

	if (params.pathfinding) {
		
		//if (FindPath(targetX, targetY, true))
		//	moving = true;
		delayed.moveCommand = true;
		delayed.targetX = obj->info.x;
		delayed.targetY = obj->info.y;
	}
	else {
		targetX = obj->info.x;
		targetY = obj->info.y;

		moving = true;
		waypoints.resize(0);
		waypoints.push_back(CPoint(targetX, targetY));
	}
	return 0;
}

long ExtObject::aMoveToMouse(LPVAL theParams)
{
	CRunLayer* pLayer = pRuntime->GetObjectLayer(pLink);

	if (params.pathfinding) {
		//if (FindPath(targetX, targetY, true))
		//	moving = true;
		delayed.moveCommand = true;
		delayed.targetX = pLayer->mouseX;
		delayed.targetY = pLayer->mouseY;
	}
	else {
		targetX = pLayer->mouseX;
		targetY = pLayer->mouseY;

		moving = true;
		waypoints.resize(0);
		waypoints.push_back(CPoint(targetX, targetY));
	}
	return 0;
}

long ExtObject::aWaypointXY(LPVAL theParams)
{
	if (params.pathfinding) {
		//if (FindPath(targetX, targetY))
		//	moving = !waypoints.empty();
		delayed.waypointCommand = true;
		delayed.targetX = theParams[0].GetFloat();
		delayed.targetY = theParams[1].GetFloat();

		waypoints.push_back(CPoint(delayed.targetX, delayed.targetY));
	}
	else {
		targetX = theParams[0].GetFloat();
		targetY = theParams[1].GetFloat();

		moving = true;
		waypoints.push_back(CPoint(targetX, targetY));
	}
	return 0;
}

long ExtObject::aWaypointObj(LPVAL theParams)
{
	CRunObject* obj = theParams[0].GetObjectParamFirstInstance(pRuntime);
	if (obj == NULL) return 0;

	if (params.pathfinding) {
		//if (FindPath(targetX, targetY))
		//	moving = !waypoints.empty();
		delayed.waypointCommand = true;
		delayed.targetX = obj->info.x;
		delayed.targetY = obj->info.y;

		waypoints.push_back(CPoint(delayed.targetX, delayed.targetY));
	}
	else {
		targetX = obj->info.x;
		targetY = obj->info.y;

		moving = true;
		waypoints.push_back(CPoint(targetX, targetY));
	}
	return 0;
}

long ExtObject::aWaypointMouse(LPVAL theParams)
{
	CRunLayer* pLayer = pRuntime->GetObjectLayer(pLink);

	if (params.pathfinding) {
		//if (FindPath(targetX, targetY))
		//	moving = !waypoints.empty();
		delayed.waypointCommand = true;
		delayed.targetX = pLayer->mouseX;
		delayed.targetY = pLayer->mouseY;

		waypoints.push_back(CPoint(delayed.targetX, delayed.targetY));
	}
	else {
		targetX = pLayer->mouseX;
		targetY = pLayer->mouseY;

		moving = true;
		waypoints.push_back(CPoint(targetX, targetY));
	}
	return 0;
}

long ExtObject::aSetObjToAvoid(LPVAL params)
{
	CRunObjType* pType = params[0].GetObjectParam(pRuntime);
	if (pType == NULL) return 0;

	if (find(typesAvoid.begin(), typesAvoid.end(), pType) == typesAvoid.end())
		typesAvoid.push_back(pType);

	return 0;
}

long ExtObject::aAddPfCollideType(LPVAL params)
{
	CRunObjType* pType = params[0].GetObjectParam(pRuntime);
	if (pType == NULL) return 0;

	if (find(pfCollideTypes.begin(), pfCollideTypes.end(), pType) == pfCollideTypes.end())
		pfCollideTypes.push_back(pType);

	refreshGridNextTick = true;

	return 0;
}

long ExtObject::aSetActivated(LPVAL theParams)
{
	activated = theParams[0].GetBool();

	if (activated)
		pRuntime->CallOnFrame(this);

	return 0;
}

long ExtObject::aRegenerateObstaclesMap(LPVAL theParams)
{
	refreshGridNextTick = true;	// do next tick

	return 0;
}
