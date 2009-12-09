// Include StdAfx
#include "StdAfx.h"

// Include any used common ACE definitions.
#ifdef RUN_ONLY
#include "..\..\Common\CommonAceDef.hpp"
#endif

//////////////////////////////////////////////////////////////////////////////////
// ACE table
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
	// Define your ACE tables here.
	// Use ADDPARAM before each ADDACT/ADDCND/ADDEXP to add parameters.
	// See the documentation for full details.

	// Note in the display string, %o is your object icon and %0-%9 make up parameters.
	// Note the script name should be a name suitable for the routine as if it were a member function in a
	// scripting language, eg. "Clear Stored Items" -> "ClearStoredItems"

	/////////////////////////////
	// Conditions
	// Format:
	// ADDCND(List name, Category, Display string, Function address, Script name, Flags)

	ADDCND("On step", "Moving", "On step", &ExtObject::cOnStep, "OnStep", CND_TRIGGERED);
	ADDCND("On horizontal step", "Moving", "On horizontal step", &ExtObject::cOnHorizontalStep, "OnHorizontalStep", CND_TRIGGERED);
	ADDCND("On vertical step", "Moving", "On vertical step", &ExtObject::cOnVerticalStep, "OnVerticalStep", CND_TRIGGERED);


	/////////////////////////////
	// Actions
	// Format:
	// ADDCND(List name, Category, Display string, Function address, Script name, Flags)
	// Motion Angle
	ADDPARAMCOMBO("Direction", "Select a direction to move in", "Right|Bottom|Left|Up");
	ADDPARAMDEF(PARAM_VALUE, "Change limit", "Limit the number of degrees it can turn", "360"); 
	ADDACT("Change direction", "Angle Of Motion",  "Change motion angle to %0 degrees (limit:%1)", &ExtObject::aChangeMotionDirection, "ChangeMotionDirection", 0);

	ADDPARAM(PARAM_VALUE, "Angle", "Angle in degrees");
	ADDPARAMDEF(PARAM_VALUE, "Change limit", "Limit the number of degrees it can turn", "360"); 
	ADDACT("Change angle", "Angle Of Motion", "Change motion angle to %0 degrees (limit:%1)", &ExtObject::aChangeMotionAngle, "ChangeMotionAngle", 0);

	ADDPARAM(PARAM_VALUE, "Degrees (Clockwise)", "Number of degrees to rotate clockwise");
	ADDACT("Change angle by", "Angle Of Motion", "Rotate motion angle by %0 degrees (clockwise)", &ExtObject::aChangeMotionAngleBy, "ChangeMotionAngleBy", 0);

	ADDPARAM(PARAM_VALUE, "X", "X position");
	ADDPARAM(PARAM_VALUE, "Y", "Y position");
	ADDPARAMDEF(PARAM_VALUE, "Change limit", "Limit the number of degrees it can turn", "360"); 
	ADDACT("Change angle towards position", "Angle Of Motion",  "Change motion angle towards %0, %1 (limit:%2)", &ExtObject::aChangeMotionAngleTowardsPos, "ChangeMotionAngleTowardsPos", 0);

	ADDPARAM(PARAM_OBJECT, "Target", "Object to rotate towards");
	ADDPARAMDEF(PARAM_VALUE, "Change limit", "Limit the number of degrees it can turn", "360"); 
	ADDACT("Change angle towards object", "Angle Of Motion",  "Change angle towards %0 (limit %1)", &ExtObject::aChangeMotionAngleTowardsObj, "ChangeMotionAngleTowardsObj", 0);

	// Motion Angle Rotation Speed
	ADDPARAMCOMBO("Mode", "Select the mode for this action", "Set|Add to|Accelerate|Decelerate");
	ADDPARAM(PARAM_VALUE, "Speed", "Degrees per second (clockwise)");
	ADDACT("Change rotate speed", "Angle Of Motion Rotation Speed",  "%0 rotate speed : %1", &ExtObject::aChangeMotionRotateSpeed, "ChangeMotionRotateSpeed", 0);

	ADDPARAMCOMBO("Direction", "Select a direction to move in", "Right|Bottom|Left|Up");	
	ADDPARAMCOMBO("Mode", "Select the mode for this action", "Set|Add to|Accelerate");
	ADDPARAM(PARAM_VALUE, "Speed", "Degrees per second (clockwise)");
	ADDACT("Change rotate speed towards direction", "Angle Of Motion Rotation Speed", "%0 rotate speed : %1 towards %2", &ExtObject::aChangeMotionRotateSpeedTowardsDirection, "ChangeMotionRotateSpeedTowardsDirection", 0);
	

	ADDPARAMCOMBO("Mode", "Select the mode for this action", "Set|Add to|Accelerate");
	ADDPARAM(PARAM_VALUE, "Speed", "Degrees per second (clockwise)");
	ADDPARAM(PARAM_VALUE, "Angle", "Angle to rotate towards (degrees)");
	ADDACT("Change rotate speed towards angle", "Angle Of Motion Rotation Speed", "%0 rotate speed : %1 towards %2 degrees", &ExtObject::aChangeMotionRotateSpeedTowardsAngle, "ChangeMotionRotateSpeedTowardsAngle", 0);

	ADDPARAMCOMBO("Mode", "Select the mode for this action", "Set|Add to|Accelerate");
	ADDPARAM(PARAM_VALUE, "Speed", "Degrees per second (clockwise)");	
	ADDPARAM(PARAM_VALUE, "X", "X position");
	ADDPARAM(PARAM_VALUE, "Y", "Y position");	
	ADDACT("Change rotate speed towards position", "Angle Of Motion Rotation Speed", "%0 rotate speed : %1 towards (%2,%3)", &ExtObject::aChangeMotionRotateSpeedTowardsPos, "ChangeMotionRotateSpeedTowardsPos", 0);

	ADDPARAMCOMBO("Mode", "Select the mode for this action", "Set|Add to|Accelerate");
	ADDPARAM(PARAM_VALUE, "Speed", "Degrees per second (clockwise)");
	ADDPARAM(PARAM_OBJECT, "Target", "Object to rotate towards");
	ADDACT("Change rotate speed towards object", "Angle Of Motion Rotation Speed", "%0 rotate speed : %1 towards %2", &ExtObject::aChangeMotionRotateSpeedTowardsObj, "ChangeMotionRotateSpeedTowardsObj", 0);

	// Speed
	ADDPARAMCOMBO("Mode", "Select the mode for this action", "Set|Add to|Accelerate|Decelerate");
	ADDPARAM(PARAM_VALUE, "Speed", "pixels per second");
	ADDACT("Change speed", "Acceleration & Speed", "%0 speed : %1", &ExtObject::aChangeSpeed, "ChangeSpeed", 0);

	ADDPARAMCOMBO("Mode", "Select the mode for this action", "Set|Add to|Accelerate|Decelerate");
	ADDPARAM(PARAM_VALUE, "Speed", "pixels per second");
	ADDPARAM(PARAM_VALUE, "Orientation", "Degrees. Useful if you rotate the screen and want moving left to actually move upwards etc");
	ADDACT("Change horizontal speed", "Acceleration & Speed", "%0 horizontal speed : %1", &ExtObject::aChangeHorizontalSpeed, "ChangeHorizontalSpeed", 0);

	ADDPARAMCOMBO("Mode", "Select the mode for this action", "Set|Add to|Accelerate|Decelerate");
	ADDPARAM(PARAM_VALUE, "Speed", "pixels per second");
	ADDPARAM(PARAM_VALUE, "Orientation", "Degrees. Useful if you rotate the screen and want moving left to actually move upwards etc");
	ADDACT("Change vertical speed", "Acceleration & Speed", "%0 vertical speed : %1", &ExtObject::aChangeVerticalSpeed, "ChangeVerticalSpeed", 0);
	
	ADDPARAMCOMBO("Mode", "Select the mode for this action", "Set|Add to|Accelerate");
	ADDPARAM(PARAM_VALUE, "Speed", "pixels per second");
	ADDPARAMCOMBO("Direction", "Select a direction to move in", "Right|Bottom|Left|Up");
	ADDACT("Change speed towards direction", "Acceleration & Speed", "%0 speed : %1 towards %2", &ExtObject::aChangeSpeedTowardsDirection, "ChangeSpeedTowardsDirection", 0);

	ADDPARAMCOMBO("Mode", "Select the mode for this action", "Set|Add to|Accelerate");
	ADDPARAM(PARAM_VALUE, "Speed", "pixels per second");
	ADDPARAM(PARAM_VALUE, "Angle", "Angle in degrees");
	ADDACT("Change speed towards angle", "Acceleration & Speed", "%0 speed : %1 towards %2 degrees", &ExtObject::aChangeSpeedTowardsAngle, "ChangeSpeedTowardsAngle", 0);

	ADDPARAMCOMBO("Mode", "Select the mode for this action", "Set|Add to|Accelerate");
	ADDPARAM(PARAM_VALUE, "Speed", "pixels per second");
	ADDPARAM(PARAM_VALUE, "X", "X position");
	ADDPARAM(PARAM_VALUE, "Y", "Y position");
	ADDACT("Change speed towards position", "Acceleration & Speed", "%0 speed : %1 towards (%2,%3)", &ExtObject::aChangeSpeedTowardsPosition, "ChangeSpeedTowardsPosition", 0);

	ADDPARAMCOMBO("Mode", "Select the mode for this action", "Set|Add to|Accelerate");
	ADDPARAM(PARAM_VALUE, "Speed", "pixels per second");
	ADDPARAM(PARAM_OBJECT, "Target", "Object to rotate towards");
	ADDACT("Change speed towards object", "Acceleration & Speed", "%0 speed : %1 towards %2", &ExtObject::aChangeSpeedTowardsObject, "ChangeSpeedTowardsObject", 0);

	// Friction
	ADDPARAMDEF(PARAM_VALUE, "Friction", "1 = no friction, 0.5 = half speed in a second", "0.2");
	ADDACT("Add friction", "Friction", "Apply friction of %0 to speed", &ExtObject::aAddFriction, "AddFriction", 0);

	ADDPARAMDEF(PARAM_VALUE, "Friction", "1 = no friction, 0.5 = half speed in a second", "0.2");
	ADDACT("Add friction horizontally", "Friction", "Apply friction of %0 to horizontal speed", &ExtObject::aAddFrictionHorizontally, "AddFrictionHorizontally", 0);

	ADDPARAMDEF(PARAM_VALUE, "Friction", "1 = no friction, 0.5 = half speed in a second", "0.2");
	ADDACT("Add friction vertically", "Friction", "Apply friction of %0 to vertical speed", &ExtObject::aAddFrictionVertically, "AddFrictionVertically", 0);

	ADDPARAMDEF(PARAM_VALUE, "Friction", "1 = no friction, 0.5 = half speed in a second", "0.2");
	ADDACT("Add friction to motion rotation", "Friction", "Apply friction of %0 to motion rotation", &ExtObject::aAddFrictionToMotionRotation, "AddFrictionToMotionRotation", 0);

	// Collision
	ADDPARAM(PARAM_OBJECT, "Object to push out of", "Object to push out of");
	ADDPARAMCOMBO("Method", "Select the method of getting out", "Opposite angle|Horizontally|Vertically|Closest direction|Normal (use for slide)");
	ADDACT("Push out", "Overlap", "Push out of %0 : %1", &ExtObject::aPushOut, "PushOutInOppositeAngle", 0);
		
	ADDPARAM(PARAM_OBJECT, "Object to push out of", "Object to push out of");
	ADDPARAMCOMBO("Direction", "Select a direction to move in", "Right|Bottom|Left|Up");
	ADDACT("Push out in direction", "Overlap", "Push out of %0 to the %1", &ExtObject::aPushOutInDirection, "PushOutAtAngle", 0);
	
	ADDPARAM(PARAM_OBJECT, "Objects to push out of", "Object to push out of");
	ADDPARAM(PARAM_VALUE, "Angle", "Angle in degrees");
	ADDACT("Push out at angle", "Overlap", "Push out of %0 at %1 degrees", &ExtObject::aPushOutAtAngle, "PushOutAtAngle", 0);

	ADDPARAM(PARAM_OBJECT, "Objects to bounce off", "Objects to push out of");
	ADDPARAMDEF(PARAM_VALUE, "Bounce Friction", "1 = no friction, 0.5 = half the speed", "1");
	ADDACT("Bounce", "Collision", "Bounce off %0 (friction: %1)", &ExtObject::aBounce, "Bounce", 0);

	ADDPARAMCOMBO("Type", "Select what you want to reverse", "Horizontally|Vertically|Both axis");
	ADDACT("Reverse", "Collision", "Reverse (%0)", &ExtObject::aReverse, "Reverse", 0);

	ADDPARAMCOMBO("Method", "Select the method of getting out", "None (Manual)|Linear|Horizontal then Vertical|Vertical then Horizontal");
	ADDPARAMCOMBO("Step Method", "Select the step method", "Total Steps|Pixels per step");
	ADDPARAMDEF(PARAM_VALUE, "Steps", "Steps", "1");
	ADDACT("Set moving method", "Movement", "Set moving style to %0 : steps %1", &ExtObject::aSetMovementStyle, "SetMovementStyle", 0);


	ADDPARAM(PARAM_VALUE, "Distance", "The distance value");
	ADDPARAM(PARAM_VALUE, "Origin X", "The origin of the restraint");
	ADDPARAM(PARAM_VALUE, "Origin Y", "The origin of the restraint");
	ADDPARAMCOMBO("Condition", "Select the condition of the restraint", "Distance greater|Distance lower|Always");
	ADDACT("Restrain distance from position", "Distance", "Restrain distance to %0 from (%1,%2) when (%3)", &ExtObject::aRestrainDistanceFromPosition, "RestrainDistanceFromPosition", 0);

	ADDPARAM(PARAM_VALUE, "Distance", "The distance value");
	ADDPARAM(PARAM_OBJECT, "Object", "The distance value");
	ADDPARAMCOMBO("Condition", "Select the condition of the restraint", "Distance greater|Distance lower|Always");
	ADDACT("Restrain distance from object", "Distance", "Restrain distance to %0 from %1 when (%2)", &ExtObject::aRestrainDistanceFromObject, "RestrainDistanceFromObject", 0);

	ADDPARAM(PARAM_VALUE, "Scale", "0.5 = half the distance, 2 = double, etc");
	ADDPARAM(PARAM_VALUE, "Origin X", "The origin of the restraint");
	ADDPARAM(PARAM_VALUE, "Origin Y", "The origin of the restraint");
	ADDACT("Scale distance from position", "Distance", "Scale distance by %0 from (%1,%2) when (%3)", &ExtObject::aScaleDistanceFromPosition, "ScaleDistanceFromPosition", 0);

	ADDPARAM(PARAM_VALUE, "Scale", "0.5 = half the distance, 2 = double, etc");
	ADDPARAM(PARAM_OBJECT, "Object", "The distance value");
	ADDACT("Scale distance from object", "Distance", "Scale distance by %0 from %1 when (%2)", &ExtObject::aScaleDistanceFromObject, "ScaleDistanceFromObject", 0);

	ADDACT("Stop Stepping", "Movement", "Use this in events triggered by On Step", &ExtObject::aStopStep, "StopStep", 0);



	/////////////////////////////
	// Expressions
	// ADDEXP(List name, Category, Display string, Function address, Flags)

	ADDEXP("Get Speed", "Speed", "Speed", &ExtObject::eSpeed, RETURN_VALUE);
	ADDEXP("Get Angle", "Speed", "Angle", &ExtObject::eAngle, RETURN_VALUE);
	ADDEXP("Get X speed", "Speed", "XSpeed", &ExtObject::eXspeed, RETURN_VALUE);
	ADDEXP("Get Y speed", "Speed", "YSpeed", &ExtObject::eYSpeed, RETURN_VALUE);


	// This line includes your common ACEs as specified in Main.h
#include "..\..\Common\CommonAceTable.hpp"
}

// Body for our handy functions
void VectorToComponent(float dx, float dy, float& speed, float& angle, float normal)
{
	speed = sqrt(dy*dy + dx*dx); 

	if(dx != 0 || dy != 0)
		angle = atan2(dy, dx) + normal;	
}
void ComponentToVector(float speed, float angle, float& dx, float& dy, float normal)
{
	dx = cos(angle - normal) * speed;
	dy = sin(angle - normal) * speed;
}
float DirectionToRadians(int direction)
{
	return RADIANS(direction * 90);
}
bool RotateTowardsIsPositive(float start, float end)
{
   float cs = cos(start);
   float ce = cos(end);
   float ss = sin(start);
   float se = sin(end);

   float z = cs*se - ss*ce;
  
   if(z > 0)
	   return true;
   else
	   return false;
}
float RotateTowards(float start, float end, float step)
{
   float cs = cos(start);
   float ce = cos(end);
   float ss = sin(start);
   float se = sin(end);

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
float AngleToFrom(float targetx, float targety, float x, float y)
{
	return atan2(targety - y, targetx - x);
}
float SetAddAccelerateDecelerate(float initial, float value, float timedelta, int mode)
{
	switch (mode)
	{
		case 0: //set
			return value;
		case 1: // add
			return initial + value;
		case 2: // accelerate
			return initial + value * timedelta;
		case 3:
			if(initial > 0)
				return max(0, initial - value * timedelta);
			else
				return min(0, initial + value * timedelta);
	}
	return initial;
}

void PushOutClosest(CRunObject* pLink, CRunObjType* pTarget)
{
	int count = 0;
	int dir = 0;
	float startX = pLink->info.x;
	float startY = pLink->info.y;

	while (pLink->pRuntime->QueryCollision(pLink, pTarget))
	{
		pLink->info.x = startX;
		pLink->info.y = startY;

		switch (dir) {
			case 0:		// up
				pLink->info.y = floor(startY - count);
				break;
			case 1:		// left
				pLink->info.x = floor(startX - count);
				break;
			case 2:		// right
				pLink->info.x = floor(startX + count);
				break;
			case 3:		// down
				pLink->info.y = floor(startY + count);
				break;
		}

		pLink->UpdateBoundingBox();

		dir++;

		if (dir == 4) {
			dir = 0;
			count++;
		}
	}
}

bool Between(float lower, float value, float upper)
{
	return value <= upper && value >= lower;
}
template<class _T>
_T distance(_T x1, _T y1, _T x2, _T y2)
{
	_T x = x1-x2;
	_T y = y2-y1;
	return sqrt(x*x + y*y);
}

bool PushOut(CRunObject* pLink, CRunObjType* pTarget, float angle, float prevx, float prevy)
{
	// Work out if previous position is safe
	float limit = 1000;

	float x = pLink->info.x;
	float y = pLink->info.y;

	pLink->info.x = prevx;
	pLink->info.y = prevy;
	pLink->UpdateBoundingBox();

	if (!pLink->pRuntime->QueryCollision(pLink, pTarget))
		limit = distance(prevx, prevy, x, y) + 5;

	pLink->info.x = x;
	pLink->info.y = y;
	pLink->UpdateBoundingBox();

	float dx = cos(angle);
	float dy = sin(angle);

	if (pLink->pRuntime->QueryCollision(pLink, pTarget))
	{
		// This final step rounds the collisions if its going in a straight left or right direction
		/*if(Between(-0.01, dy, 0.01))
			pLink->info.x = floor(pLink->info.x);
		if(Between(-0.01, dx, 0.01))
			pLink->info.y = floor(pLink->info.y);*/

		pLink->UpdateBoundingBox();
	}
	else
	{
		return false;
	}
	while (pLink->pRuntime->QueryCollision(pLink, pTarget) && limit > 0)
	{
		pLink->info.x += dx;
		pLink->info.y += dy;
		limit --;

		// This final step rounds the collisions if its going in a straight left or right direction
		/*if(Between(-0.0001, dy, 0.0001))
			pLink->info.x = floor(pLink->info.x );
		if(Between(-0.0001, dx, 0.0001))
			pLink->info.y = floor(pLink->info.y);*/

		pLink->UpdateBoundingBox();
	}
	// If limit is lower than 0...we failed to push out :/
	if(limit < 0)
	{
		pLink->info.x = prevx;
		pLink->info.y = prevy;
		pLink->UpdateBoundingBox();
		return true;
	}
	//if(Between(-0.0001, dy, 0.0001) || (Between(-0.0001, dx, 0.0001)))
	//	return true; 

	// Last and final step, for more precision
	pLink->info.x -= dx;
	pLink->info.y -= dy;
	pLink->UpdateBoundingBox();

	dx *= 0.05;
	dy *= 0.05;

	while (pLink->pRuntime->QueryCollision(pLink, pTarget))
	{
		pLink->info.x += dx;
		pLink->info.y += dy;

		pLink->UpdateBoundingBox();
	}

	// Insanely more...
	pLink->info.x -= dx;
	pLink->info.y -= dy;
	pLink->UpdateBoundingBox();

	dx *= 0.1;
	dy *= 0.1;

	while (pLink->pRuntime->QueryCollision(pLink, pTarget))
	{
		pLink->info.x += dx;
		pLink->info.y += dy;

		pLink->UpdateBoundingBox();
	}
	return true;
}


float CalculateNormal(CRunObject* obj, CRunObjType* bounceOff, bool* concave)
{
	// work out the angle
	double x = obj->info.x;
	double y = obj->info.y;

	// Rounding improves bounce reliability
//	x = floor(x+0.5);
	//y = floor(y+0.5);
	//x += 0.5;
	//y += 0.5;

	const double PI = 3.1415926535897932384626433832795;
	double col_radius = 16.0;
	if(concave)
		col_radius = 4.0;
	const double col_count = 64;
	const double col_half_count = col_count / 2.0;
	
	double col_vector_x = 0;
	double col_vector_y = 0;

	int found_count = 0;

	if(concave)
		*concave = false;
	for(double a = 0; a < col_count; a++)
	{
		double angle = ((a) * 2 * PI ) / col_count;

		double ox = cos(angle) * col_radius;
		double oy = sin(angle) * col_radius;

		obj->info.x = floor(x + ox);
		obj->info.y = floor(y + oy);

		

		//obj->info.x = floor(obj->info.x + 0.5);
		//obj->info.y = floor(obj->info.y + 0.5);

		obj->UpdateBoundingBox();
		if(obj->pRuntime->QueryCollision(obj, bounceOff))
		{
			col_vector_x += ox;
			col_vector_y += oy;
			found_count++;
		}
	}
	if(found_count >  40)
		if(concave)
			*concave = true;

	obj->info.x = x;
	obj->info.y = y;
	obj->UpdateBoundingBox();					

	double normal;

	if (found_count == 0)
		normal = 0;//input_angle + PI;	// normal opposes the motion
	else
		normal = atan2(col_vector_y, col_vector_x) + PI;

	return normal;
}