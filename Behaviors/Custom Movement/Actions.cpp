// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

// Motion Angle
long ExtObject::aChangeMotionDirection(LPVAL params)
{
	float newangle = DirectionToRadians(params[0].GetFloat());
	float step = params[1].GetFloat();
	
	m_angle = RotateTowards(m_angle, newangle,  step);

	return 0;
}
long ExtObject::aChangeMotionAngle(LPVAL params)
{
	float newangle = RADIANS(params[0].GetFloat());
	float step = params[1].GetFloat();
	
	m_angle = RotateTowards(m_angle, newangle,  step);

	return 0;
}

long ExtObject::aChangeMotionAngleBy(LPVAL params)
{
	float rot = RADIANS(params[0].GetFloat());
	
	m_angle += rot;

	return 0;
}

long ExtObject::aChangeMotionAngleTowardsPos(LPVAL params)
{
	float x = params[0].GetFloat();
	float y = params[1].GetFloat();
	float step = params[2].GetFloat();
	float newangle = AngleToFrom(x,y, pLink->info.x, pLink->info.y);

	m_angle = RotateTowards(m_angle, newangle,  step);
	return 0;
}
long ExtObject::aChangeMotionAngleTowardsObj(LPVAL params)
{
	CRunObject* pTarget = params[0].GetPairedObjectParam(pRuntime, this);
	if (pTarget == NULL) return 0;
	float step = params[1].GetFloat();
	float newangle = AngleToFrom(pTarget->info.x,pTarget->info.y, pLink->info.x, pLink->info.y);

	m_angle = RotateTowards(m_angle, newangle,  step);
	return 0;
}


// Motion Rotation
long ExtObject::aChangeMotionRotateSpeed(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();
	int mode = params[0].GetInt();
	float speed = params[1].GetFloat();

	m_rotation = SetAddAccelerateDecelerate(m_rotation, speed, dt, mode);

	return 0;
}

long ExtObject::aChangeMotionRotateSpeedTowardsDirection(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();
	int mode = params[0].GetInt();
	float speed = params[1].GetFloat();	
	float angle = DirectionToRadians(params[2].GetInt());


	if(!RotateTowardsIsPositive(m_angle, angle))
		speed *= -1;

	m_rotation = SetAddAccelerateDecelerate(m_rotation, speed, dt, mode);

	return 0;
}

long ExtObject::aChangeMotionRotateSpeedTowardsAngle(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();
	int mode = params[0].GetInt();
	float speed = params[1].GetFloat();
	float angle = RADIANS(params[2].GetFloat());


	if(!RotateTowardsIsPositive(m_angle, angle))
		speed *= -1;

	m_rotation = SetAddAccelerateDecelerate(m_rotation, speed, dt, mode);

	return 0;
}

long ExtObject::aChangeMotionRotateSpeedTowardsPos(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();
	int mode = params[0].GetInt();
	float speed = params[1].GetFloat();
	float x = params[2].GetFloat();
	float y = params[3].GetFloat();
	
	float angle = AngleToFrom(x, y, pLink->info.x, pLink->info.y);


	if(!RotateTowardsIsPositive(m_angle, angle))
		speed *= -1;

	m_rotation = SetAddAccelerateDecelerate(m_rotation, speed, dt, mode);

	return 0;
}
long ExtObject::aChangeMotionRotateSpeedTowardsObj(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();

	CRunObject* pTarget = params[0].GetPairedObjectParam(pRuntime, this);
	if (pTarget == NULL) return 0;

	float angle = AngleToFrom(pTarget->info.x, pTarget->info.y, pLink->info.x, pLink->info.y);
	int mode = params[2].GetInt();
	float speed = params[3].GetFloat();

	if(!RotateTowardsIsPositive(m_angle, angle))
		speed *= -1;

	m_rotation = SetAddAccelerateDecelerate(m_rotation, speed, dt, mode);

	return 0;
}

// Speed
long ExtObject::aChangeSpeed(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();
	int mode = params[0].GetInt();
	float speed = params[1].GetFloat();

	m_speed = SetAddAccelerateDecelerate(m_speed, speed, dt, mode);

	return 0;
}
long ExtObject::aChangeHorizontalSpeed(LPVAL params)
{
	int mode = params[0].GetInt();
	float speed = params[1].GetFloat();
	float orientation = RADIANS(params[2].GetFloat());

	float dt = pRuntime->GetTimeDelta();

	float dx, dy;
	ComponentToVector(m_speed, m_angle, dx, dy, orientation);

	dx = SetAddAccelerateDecelerate(dx, speed, dt, mode);

	VectorToComponent(dx, dy, m_speed, m_angle, orientation);

	return 0;
}
long ExtObject::aChangeVerticalSpeed(LPVAL params)
{
	int mode = params[0].GetInt();
	float speed = params[1].GetFloat();
	float orientation = RADIANS(params[2].GetFloat());

	float dt = pRuntime->GetTimeDelta();
	float dx, dy;
	ComponentToVector(m_speed, m_angle, dx, dy, orientation);

	dy = SetAddAccelerateDecelerate(dy, speed, dt, mode);

	VectorToComponent(dx, dy, m_speed, m_angle, orientation);

	return 0;
}
long ExtObject::aChangeSpeedTowardsDirection(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();

	float dx, dy;
	ComponentToVector(m_speed, m_angle, dx, dy);

	int mode = params[0].GetInt();
	float speed = params[1].GetFloat();
	float angle = DirectionToRadians(params[2].GetInt());

	float speedx = cos(angle) * speed;
	float speedy = sin(angle) * speed;

	dx = SetAddAccelerateDecelerate(dx, speedx, dt, mode);
	dy = SetAddAccelerateDecelerate(dy, speedy, dt, mode);

	VectorToComponent(dx, dy, m_speed, m_angle);

	return 0;
}
long ExtObject::aChangeSpeedTowardsAngle(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();

	float dx, dy;
	ComponentToVector(m_speed, m_angle, dx, dy);

	int mode = params[0].GetInt();
	float speed = params[1].GetFloat();
	float angle = RADIANS(params[2].GetFloat());

	float speedx = cos(angle) * speed;
	float speedy = sin(angle) * speed;

	dx = SetAddAccelerateDecelerate(dx, speedx, dt, mode);
	dy = SetAddAccelerateDecelerate(dy, speedy, dt, mode);

	VectorToComponent(dx, dy, m_speed, m_angle);

	return 0;
}
long ExtObject::aChangeSpeedTowardsPosition(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();

	float dx, dy;
	ComponentToVector(m_speed, m_angle, dx, dy);

	int mode = params[0].GetInt();
	float speed = params[1].GetFloat();


	float x = params[2].GetFloat();
	float y = params[3].GetFloat();

	float angle = AngleToFrom(x,y, pLink->info.x, pLink->info.y);

	float speedx = cos(angle) * speed;
	float speedy = sin(angle) * speed;

	dx = SetAddAccelerateDecelerate(dx, speedx, dt, mode);
	dy = SetAddAccelerateDecelerate(dy, speedy, dt, mode);

	VectorToComponent(dx, dy, m_speed, m_angle);

	return 0;
}
long ExtObject::aChangeSpeedTowardsObject(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();

	float dx, dy;
	ComponentToVector(m_speed, m_angle, dx, dy);

	int mode = params[0].GetInt();
	float speed = params[1].GetFloat();

	CRunObject* pTarget = params[2].GetPairedObjectParam(pRuntime, this);
	if (pTarget == NULL) return 0;

	float angle = AngleToFrom(pTarget->info.x,pTarget->info.y, pLink->info.x, pLink->info.y);

	float speedx = cos(angle) * speed;
	float speedy = sin(angle) * speed;

	dx = SetAddAccelerateDecelerate(dx, speedx, dt, mode);
	dy = SetAddAccelerateDecelerate(dy, speedy, dt, mode);

	VectorToComponent(dx, dy, m_speed, m_angle);

	return 0;
}
// Friction
long ExtObject::aAddFriction(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();
	float friction = params[0].GetFloat();
	m_speed *= pow(friction, dt);
	return 0;
}
long ExtObject::aAddFrictionHorizontally(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();
	float friction = params[0].GetFloat();

	float dx, dy;
	ComponentToVector(m_speed, m_angle, dx, dy);
	dx *= pow(friction, dt);
	VectorToComponent(dx, dy, m_speed, m_angle);

	return 0;
}
long ExtObject::aAddFrictionVertically(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();
	float friction = params[0].GetFloat();

	float dx, dy;
	ComponentToVector(m_speed, m_angle, dx, dy);
	dy *= pow(friction, dt);
	VectorToComponent(dx, dy, m_speed, m_angle);

	return 0;
}
long ExtObject::aAddFrictionToMotionRotation(LPVAL params)
{
	float dt = pRuntime->GetTimeDelta();
	float friction = params[0].GetFloat();
	m_rotation = m_rotation * pow(friction, dt);
	return 0;
}

// Collisions
long ExtObject::aPushOut(LPVAL params)
{
	CRunObjType* pTarget = params[0].GetObjectParam(pRuntime);
	int method = params[1].GetInt();

	float dx, dy;
	ComponentToVector(m_speed, m_angle, dx, dy);


	//Opposite angle|Horizontally|Vertically|Closest direction|Normal (use for slide)

	switch(method)
	{
	case 0:
		PushOut(pLink, pTarget, m_angle + RADIANS(180), m_prevx, m_prevy);
		break;
	case 1:
		if(dx > 0)
			PushOut(pLink, pTarget, RADIANS(180), m_prevx, m_prevy);
		else
			PushOut(pLink, pTarget, RADIANS(0), m_prevx, m_prevy);
		break;
	case 2:
		if(dy > 0)
			PushOut(pLink, pTarget, RADIANS(90), m_prevx, m_prevy);
		else
			PushOut(pLink, pTarget, RADIANS(270), m_prevx, m_prevy);
		break;
	case 3:
		PushOutClosest(pLink, pTarget);
		break;
	case 4:
		float normal = CalculateNormal(pLink, pTarget);
		PushOut(pLink, pTarget, normal, m_prevx, m_prevy);
	}

	return 0;
}
long ExtObject::aPushOutAtAngle(LPVAL params)
{
	CRunObjType* pTarget = params[0].GetObjectParam(pRuntime);
	float angle = RADIANS(params[1].GetFloat());
	
	PushOut(pLink, pTarget, angle, m_prevx, m_prevy);

	return 0;
}
long ExtObject::aPushOutInDirection(LPVAL params)
{
	CRunObjType* pTarget = params[0].GetObjectParam(pRuntime);
	float angle = DirectionToRadians(params[1].GetInt());

	PushOut(pLink, pTarget, angle, m_prevx, m_prevy);

	return 0;
}
long ExtObject::aReverse(LPVAL params)
{
	int type = params[0].GetInt();
	if(type == 2){
		m_angle += RADIANS(180);
	}
	else{
		float dx, dy;
		ComponentToVector(m_speed, m_angle, dx, dy);
		if(type == 0) 
			dx *= -1;
		if(type == 1)
			dy *= -1;
		VectorToComponent(dx, dy, m_speed, m_angle);
	}
	return 0;
}
long ExtObject::aBounce(LPVAL params)
{
	CRunObjType* pTarget = params[0].GetObjectParam(pRuntime);
	float friction = params[1].GetFloat();

	float normal;
	normal = CalculateNormal(pLink, pTarget);

	if(PushOut(pLink, pTarget, normal, m_prevx, m_prevy))
	{
		float dx, dy;
		bool concave;
		normal = CalculateNormal(pLink, pTarget, &concave);
		ComponentToVector(m_speed, m_angle, dx, dy, normal);
		if(concave)
		{
			dx = -dx * friction;
			dy = -dy * friction;
		}
		else
		{
			dx = dx * -1 * friction;
		}
		VectorToComponent(dx, dy, m_speed, m_angle, normal);
	}

	return 0;
}

long ExtObject::aSetMovementStyle(LPVAL params)
{
	movestyle = params[0].GetInt();
	stepstyle = params[1].GetInt();
	steps = params[2].GetInt();
	if(steps < 0) steps = 1;

	return 0;
}

void RestrainDistance(float& x, float& y, float ox, float oy, float distance)
{
	double angle = atan2(y-oy, x-ox);
	x = ox + cos(angle) * distance;
	y = oy + sin(angle) * distance;
}

long ExtObject::aRestrainDistanceFromPosition(LPVAL params)
{
	double distance = params[0].GetFloat();
	double ox = params[1].GetFloat();
	double oy = params[2].GetFloat();
	int combo = params[3].GetInt();
	
	double dx = pLink->info.x - ox;
	double dy = pLink->info.y - oy;

	double dist = sqrt(dx*dx + dy*dy);
	switch (combo)
	{
	case 0:
		if(dist > distance)
		{
			RestrainDistance(pLink->info.x, pLink->info.y, ox, oy, distance);
			pLink->UpdateBoundingBox();
		}
		break;
	case 1:
		if(dist < distance)
		{
			RestrainDistance(pLink->info.x, pLink->info.y, ox, oy, distance);
			pLink->UpdateBoundingBox();
		}
		break;
	case 2:
		RestrainDistance(pLink->info.x, pLink->info.y, ox, oy, distance);
		pLink->UpdateBoundingBox();
		break;
	}
	return 0;
}

long ExtObject::aRestrainDistanceFromObject(LPVAL params)
{
	double distance = params[0].GetFloat();
	CRunObject* pTarget = params[1].GetPairedObjectParam(pRuntime, this);
	if (pTarget == NULL) return 0;

	double ox = pTarget->info.x;
	double oy = pTarget->info.y;
	int combo = params[2].GetInt();
	
	double dx = pLink->info.x - ox;
	double dy = pLink->info.y - oy;

	double dist = sqrt(dx*dx + dy*dy);
	switch (combo)
	{
	case 0:
		if(dist > distance)
		{
			RestrainDistance(pLink->info.x, pLink->info.y, ox, oy, distance);
			pLink->UpdateBoundingBox();
		}
		break;
	case 1:
		if(dist < distance)
		{
			RestrainDistance(pLink->info.x, pLink->info.y, ox, oy, distance);
			pLink->UpdateBoundingBox();
		}
		break;
	case 2:
		RestrainDistance(pLink->info.x, pLink->info.y, ox, oy, distance);
		pLink->UpdateBoundingBox();
		break;
	}
	return 0;
}

double Lerp(double a, double b, double x)
{
	return a + x * (b - a);
}

long ExtObject::aScaleDistanceFromPosition(LPVAL params)
{
	float scale = params[0].GetFloat();
	float ox = params[1].GetFloat();
	float oy = params[2].GetFloat();

	pLink->info.x = Lerp(ox, pLink->info.x, scale);
	pLink->info.y = Lerp(oy, pLink->info.y, scale);

	pLink->UpdateBoundingBox();

	return 0;
}

long ExtObject::aScaleDistanceFromObject(LPVAL params)
{
	float scale = params[0].GetFloat();

	CRunObject* pTarget = params[1].GetPairedObjectParam(pRuntime, this);
	if (pTarget == NULL) return 0;

	float ox = pTarget->info.x;
	float oy = pTarget->info.y;

	pLink->info.x = Lerp(ox, pLink->info.x, scale);
	pLink->info.y = Lerp(oy, pLink->info.y, scale);

	pLink->UpdateBoundingBox();
	return 0;
}

long ExtObject::aStopStep(LPVAL params)
{
	m_StopStep = true;
	return 0;
}