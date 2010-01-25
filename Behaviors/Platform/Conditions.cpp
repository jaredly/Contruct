// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::cIsMoving		(LPVAL theParams)
{
	return  dx != 0 || dy != 0;
}

long ExtObject::cOnBounce		(LPVAL theParams)
{
	return TRUE;
}

long ExtObject::cCompareGravity (LPVAL params)
{
	return DoComparison(params[0].GetInt(), grav_fall, params[1].GetFloat());
}

long ExtObject::cIsWalking (LPVAL theParams)
{
	switch (grav_dir)
	{
	case GRAV_UP:
	case GRAV_DOWN:
		return dx != 0;
	case GRAV_LEFT:
	case GRAV_RIGHT:
		return dy != 0;
	}

	return false;
}

long ExtObject::cIsJumping (LPVAL theParams)
{
	switch (grav_dir) {
	case GRAV_UP:
		return dy > 0;
	case GRAV_DOWN:
		return dy < 0;
	case GRAV_LEFT:
		return dx > 0;
	case GRAV_RIGHT:
		return dx < 0;
	}

	return false;

}

long ExtObject::cIsFalling (LPVAL theParams)
{
	if (IsOnFloor(true)) return false;

	switch (grav_dir) {
	case GRAV_UP:
		return dy < 0;
	case GRAV_DOWN:
		return dy > 0;
	case GRAV_LEFT:
		return dx < 0;
	case GRAV_RIGHT:
		return dx > 0;
	}
	return false;
}

long ExtObject::cIsOnGround (LPVAL theParams)
{
	return IsOnFloor(true);
}

long ExtObject::cCeilingAbove (LPVAL theParams)
{
	return IsOnCeiling();
}

long ExtObject::cWallRight (LPVAL theParams)
{
	return !CanMoveRight();
}

long ExtObject::cWallLeft (LPVAL theParams)
{
	return !CanMoveLeft();
}