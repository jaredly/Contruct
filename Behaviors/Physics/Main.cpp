// Main.cpp - Main implementation of object
//

// Include StdAfx
#include "StdAfx.h"

// If run only
#ifdef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////
long ExtObject::aAddForce(LPVAL params)
{
	forceX += params[0].GetDouble();
	forceY += params[1].GetDouble();

//	NewtonWorldUnfreezeBody(nWorld, body);

	return 0;
}

long ExtObject::aSetForce(LPVAL params)
{
	forceX = params[0].GetDouble();
	forceY = params[1].GetDouble();

//	NewtonWorldUnfreezeBody(nWorld, body);

	return 0;
}

void AddSpringForce(b2Body& bA, b2Vec2& localA, b2Body& bB, b2Vec2& localB, float32 k, float32 friction, float32 desiredDist)
{

        b2Vec2 pA = bA.GetWorldPoint(localA);
        b2Vec2 pB = bB.GetWorldPoint(localB);
        b2Vec2 diff = pB - pA;
        //Find velocities of attach points
		b2Vec2 vA = bA.GetLinearVelocity() - b2Cross(bA.GetWorldVector(localA), bA.GetAngularVelocity());
        b2Vec2 vB = bB.GetLinearVelocity() - b2Cross(bB.GetWorldVector(localB), bB.GetAngularVelocity());
        b2Vec2 vdiff = vB-vA;
        float32 dx = diff.Normalize(); //normalizes diff and puts length into dx
        float32 vrel = vdiff.x*diff.x + vdiff.y*diff.y;
        float32 forceMag = -k*(dx-desiredDist) - friction*vrel;
        diff *= forceMag; // diff *= forceMag
        bB.ApplyForce(diff, bA.GetWorldPoint(localA));
		diff *= -1.0f;
        bA.ApplyForce(diff, bB.GetWorldPoint(localB));
        bA.WakeUp();
        bB.WakeUp();

}

long ExtObject::aSpringToBody(LPVAL params)
{
	CRunObject* obj = params[0].GetPairedObjectParam(pRuntime, this);

	b2Vec2 zero(0.0f,0.0f);

	if (obj == NULL) return 0;

	CRunObject** i;
	int count;
	pRuntime->GetObjectSiblings(obj, i, count);

	if (i == NULL || count <= 0) return 0;

	ExtObject* pPhysics = NULL;

	CRunObject** end = i + count;

	// Find the first physics movement on the given object
	for ( ; i != end; i++) {
		try {
			if (pPhysics = dynamic_cast<ExtObject*>(*i))
				break;
		}
		catch (...) {
			// Nothing to do when the dynamic_cast fails
		}
	}

	if (pPhysics == NULL) return 0;
	if (pPhysics->physicsID != UNIQUE_PHYSICS_ID) return 0;	// Dynamic casts seem to sometimes give false positives
	if(!pPhysics->body) pPhysics->CreateBody();


	float k = params[1].GetDouble();
	float damp = params[2].GetDouble();
	float distance = params[3].GetDouble() * worldXscale;

	if(this->body !=  pPhysics->body)
		AddSpringForce(*this->body, zero, *pPhysics->body, zero, k, damp, distance);

	return 0;
}

long ExtObject::aAddForceTowardsPosition(LPVAL params)
{
	double x = params[0].GetDouble();
	double y = params[1].GetDouble();
	double force = params[2].GetDouble();

	double dx = x - pLink->info.x;
	double dy = y - pLink->info.y;

	double angle = atan2(dy, dx);

	forceX += cos(angle) * force;
	forceY += sin(angle) * force;

//	NewtonWorldUnfreezeBody(nWorld, body);

	return 0;
}

long ExtObject::aSetForceTowardsPosition(LPVAL params)
{
	double x = params[0].GetDouble();
	double y = params[1].GetDouble();
	double force = params[2].GetDouble();

	double dx = x - pLink->info.x;
	double dy = y - pLink->info.y;

	double angle = atan2(dy, dx);

	forceX = cos(angle) * force;
	forceY = sin(angle) * force;

//	NewtonWorldUnfreezeBody(nWorld, body);

	return 0;
}

long ExtObject::aSetGravity(LPVAL params)
{
	gravity = params[0].GetBool();
	body->WakeUp();

//	NewtonWorldUnfreezeBody(nWorld, body);

	return 0;
}

long ExtObject::aSetImmovable(LPVAL params)
{
	immovable = params[0].GetBool();
	do_recreate = true;

	return 0;
}

long ExtObject::aSetNoRotations(LPVAL params)
{
	norotation = params[0].GetBool();
	do_recreate = true;

	return 0;
}

long ExtObject::aSetGravityForce(LPVAL params)
{
	worldGravity = params[0].GetDouble();
	body->WakeUp();
	return 0;
}

long ExtObject::aSetMass(LPVAL params)
{
	if(mass < 0) return 0;

	do_recreate = true;
	mass = params[0].GetFloat();

	return 0;
}

long ExtObject::aSetLinearDamp(LPVAL params)
{
	lineardamp = params[0].GetDouble() / 100.0;

//	NewtonBodySetLinearDamping(body, lineardamp);

//	NewtonWorldUnfreezeBody(nWorld, body);

	return 0;
}

long ExtObject::aAddTorque(LPVAL params)
{
	torque += params[0].GetDouble();
//	NewtonWorldUnfreezeBody(nWorld, body);
	return 0;
}

long ExtObject::aSetTorque(LPVAL params)
{
	torque = params[0].GetDouble();
//	NewtonWorldUnfreezeBody(nWorld, body);
	return 0;
}

long ExtObject::aSetVelocity(LPVAL params)
{
	b2Vec2 v;
	v.x = params[0].GetDouble() / 30;
	v.y = params[1].GetDouble() / 30;

	if(!body) CreateBody();
	body->SetLinearVelocity(v);

	body->WakeUp();

//	NewtonWorldUnfreezeBody(nWorld, body);
//	NewtonBodySetVelocity(body, &v[0]);

	return 0;
}

long ExtObject::aSetOmega(LPVAL params)
{
	if(!body) CreateBody();
	body->SetAngularVelocity(params[0].GetDouble());
//	dVector m;
//	m.m_x = 0.0;
//	m.m_y = 0.0;
//	m.m_z = params[0].GetDouble();

//	NewtonWorldUnfreezeBody(nWorld, body);
//	NewtonBodySetOmega(body, &m[0]);
	body->WakeUp();

	return 0;
}

long ExtObject::aAddTorqueTowardsAngle(LPVAL params)
{
	double start = pLink->info.angle;
	double end = params[1].GetFloat();

	double cs = cos(RADIANS(start));
	double ce = cos(RADIANS(end));
	double ss = sin(RADIANS(start));
	double se = sin(RADIANS(end));

	double z = cs*se - ss*ce;

	if(z > 0)
		torque += params[0].GetDouble();
	else
		torque -= params[0].GetDouble();

//	NewtonWorldUnfreezeBody(nWorld, body);

	return 0;
}

long ExtObject::aSetTorqueTowardsAngle(LPVAL params)
{
	double start = pLink->info.angle;
	double end = params[1].GetFloat();

	double cs = cos(RADIANS(start));
	double ce = cos(RADIANS(end));
	double ss = sin(RADIANS(start));
	double se = sin(RADIANS(end));

	double z = cs*se - ss*ce;

	if(z > 0)
		torque = params[0].GetDouble();
	else
		torque = -params[0].GetDouble();

//	NewtonWorldUnfreezeBody(nWorld, body);

	return 0;
}

long ExtObject::aHingeToXY(LPVAL params)
{
	// Incase someone used 'set x' or 'set width' etc before adding the hinge
	if(!body) CreateBody();
	CheckForBodyChange();
	UpdateOldValues();

	double x = params[0].GetDouble();
	double y = params[1].GetDouble();
	double stiffness = params[2].GetDouble() / 100.0;
	Clamp(stiffness, 0.0, 1.0);

	TransformFrameToWorld(x, y);

//	NewtonWorldUnfreezeBody(nWorld, body);

//	dVector pivotPoint(x, y, 0);
//	dVector pinDir(0, 0, 1);	// only rotates over Z axis
//	hinge = NewtonConstraintCreateHinge(nWorld, &pivotPoint[0], &pinDir[0], body, NULL);
//	NewtonJointSetStiffness(hinge, stiffness);

	b2Vec2 point;
	point.x = x;
	point.y = y;

	b2RevoluteJointDef jointDef;
	jointDef.Initialize(body, staticbody, point);

	// This might be a memory leak...i dont know
	world->CreateJoint(&jointDef);
	

	return 0;
}

long ExtObject::aHingeToObj(LPVAL params)
{
	if(!body) CreateBody();
	// Incase someone used 'set x' or 'set width' etc before adding the hinge
	CheckForBodyChange();
	UpdateOldValues();

	CRunObject* obj = params[0].GetPairedObjectParam(pRuntime, this);
	if (obj == NULL) return 0;

	CRunObject** i;
	int count;
	pRuntime->GetObjectSiblings(obj, i, count);

	if (i == NULL || count <= 0) return 0;

	ExtObject* pPhysics = NULL;

	CRunObject** end = i + count;

	// Find the first physics movement on the given object
	for ( ; i != end; i++) {
		try {
			if (pPhysics = dynamic_cast<ExtObject*>(*i))
				break;
		}
		catch (...) {
			// Nothing to do when the dynamic_cast fails
		}
	}

	if (pPhysics == NULL) return 0;
	if (pPhysics->physicsID != UNIQUE_PHYSICS_ID) return 0;	// Dynamic casts seem to sometimes give false positives
	if(!pPhysics->body) pPhysics->CreateBody();
	pPhysics->CheckForBodyChange();

	double stiffness = params[2].GetDouble() / 100.0;
	Clamp(stiffness, 0.0, 1.0);

	HingeToObjectAction htoa;
	htoa.pPhysics = pPhysics;
	htoa.stiffness = stiffness;
	htoa.offsetX = obj->info.x;
	htoa.offsetY = obj->info.y;

	// Check for image points
	if (params[1].Type() == EXPTYPE_STRING) {
		POINTF pt;

		if (pRuntime->GetImagePoint(obj, *(params[1].GetStringPtr()), pt)) {
			htoa.offsetX = pt.x;
			htoa.offsetY = pt.y;
		}
	}
	else if (params[1].Type() == EXPTYPE_INTEGER) {
		POINTF pt;

		if (pRuntime->GetImagePoint(obj, params[1].GetInt() - 1, pt)) {
			htoa.offsetX = pt.x;
			htoa.offsetY = pt.y;
		}
	}

	//waitingHingeActions.push_back(htoa);

	TransformFrameToWorld(htoa.offsetX, htoa.offsetY);

	b2Vec2 point;
	point.x = htoa.offsetX;
	point.y = htoa.offsetY;

	b2RevoluteJointDef jointDef;
	jointDef.Initialize(htoa.pPhysics->body, body, point);

	// This might be a memory leak...i dont know
	world->CreateJoint(&jointDef);

	return 0;
}

long ExtObject::aDisableCollision(LPVAL params)
{
	CRunObjType* pParamType = params[0].GetObjectParam(pRuntime);

	// Is a family - don't add a family type, add its member types
	if (pParamType->teamID > -1) {
		int count;
		CRunObjType** pTypesList;
		pRuntime->GetFamilyTypes(pParamType, pTypesList, count);

		if (count > 0 && pTypesList != NULL) {
			CRunObjType** i = pTypesList;
			CRunObjType** end = pTypesList + count;

			for ( ; i != end; i++) {
				if (!exists_element(disabledCollisions.begin(), disabledCollisions.end(), (*i)))
					disabledCollisions.push_back(*i);
			}
		}
	
	}
	else {
		if (!exists_element(disabledCollisions.begin(), disabledCollisions.end(), pParamType))
			disabledCollisions.push_back(pParamType);
	}

	return 0;
}

long ExtObject::aResetDisabledCollisions(LPVAL params)
{
	disabledCollisions.resize(0);

	return 0;
}

void ExtObject::DoWaitingHinges()
{
/*	vector<HingeToObjectAction>::iterator i = waitingHingeActions.begin();

	for ( ; i != waitingHingeActions.end(); i++) {

		TransformFrameToWorld(i->offsetX, i->offsetY);
		NewtonWorldUnfreezeBody(nWorld, body);

		dMatrix location(GetIdentityMatrix());
		NewtonBodyGetMatrix(i->pPhysics->body, &location[0][0]);

		dVector pivotPoint = location.m_posit;
		pivotPoint.m_x += i->offsetX;
		pivotPoint.m_y += i->offsetY;

		dVector pinDir(0, 0, 1);	// only rotates over Z axis
		hinge = NewtonConstraintCreateHinge(nWorld, &pivotPoint[0], &pinDir[0], body, i->pPhysics->body);
		NewtonJointSetStiffness(hinge, i->stiffness);
	}

	waitingHingeActions.resize(0);*/
}

//////////////////////////////////////////////////////////////////////////////////
// Expressions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::eGetMass(LPVAL params, ExpReturn& ret)
{
	return ret = mass;
}

long ExtObject::eGetLinearDamp(LPVAL params, ExpReturn& ret)
{
	return ret = lineardamp;
}

long ExtObject::eGetWorldXscale(LPVAL params, ExpReturn& ret)
{
	return ret = worldXscale;
}

long ExtObject::eGetWorldYscale(LPVAL params, ExpReturn& ret)
{
	return ret = worldYscale;
}

long ExtObject::eGetVelocityX(LPVAL params, ExpReturn& ret)
{
	if(!body) CreateBody();
	b2Vec2 v = body->GetLinearVelocity();
	v *= 30;
	return ret = v.x;
}

long ExtObject::eGetVelocityY(LPVAL params, ExpReturn& ret)
{
	if(!body) CreateBody();
	b2Vec2 v = body->GetLinearVelocity();
	v *= 30;
	return ret = v.y;
}

long ExtObject::eGetOmega(LPVAL params, ExpReturn& ret)
{
	if(!body) CreateBody();
	return ret = body->GetAngularVelocity();
}

#endif // #ifdef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// ACE tables
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
	// Define your ACE tables here.
	// Use ADDPARAM before each ADDACT/ADDCND/ADDEXP to add formal parameters.
	
	// Format:
	// ADDPARAM(type, title, description)
	//
	// type = the type of parameter.  See the PARAMS enum.
	// title = title of parameter, in expression editor
	// description = text to go in the text box describing this parameter.  Try to give
	//				 a helpful description.

	/////////////////////////////
	// Conditions

	// Format:
	// ADDCND(menuID, routine, scriptname, displaytext, flags)
	//
	// menuID = ID in the menu resource, must start at 2000
	// routine = address of the function associated
	// scriptname = reserved for future script use, e.g. "Set X position" script name could be "SetX"
	// displaytext = text to display in event editor, %0 is first param, %1 second etc
	// flags: can be
	// 0: ordinary evaluated event
	// CND_TRIGGERED: ordinary event trigger


	/////////////////////////////
	// Actions

	// ADDACT(menuID, routine, scriptname, displaytext)
	// Same format as ADDCND, but actions have no flags.
	ADDPARAM(PARAM_VALUE, "X component", "X component of the force to add.");
	ADDPARAM(PARAM_VALUE, "Y component", "Y component of the force to add.");
	ADDACT("Add force", "Force", "Add force (%0, %1)", &ExtObject::aAddForce, "AddForce", 0);

	ADDPARAM(PARAM_VALUE, "X component", "X component of the force to set.");
	ADDPARAM(PARAM_VALUE, "Y component", "Y component of the force to set.");
	ADDACT("Set force", "Force", "Set force (%0, %1)", &ExtObject::aSetForce, "SetForce", 0);

	ADDPARAM(PARAM_VALUE, "X co-ordinate", "X position to add force towards.");
	ADDPARAM(PARAM_VALUE, "Y co-ordinate", "Y position to add force towards.");
	ADDPARAM(PARAM_VALUE, "Force", "Force to add.");
	ADDACT("Add force towards position", "Force", "Add force %2 towards (%0, %1)", &ExtObject::aAddForceTowardsPosition, "AddForceToward", 0);

	ADDPARAM(PARAM_VALUE, "X co-ordinate", "X position to set force towards.");
	ADDPARAM(PARAM_VALUE, "Y co-ordinate", "Y position to set force towards.");
	ADDPARAM(PARAM_VALUE, "Force", "Force to set.");
	ADDACT("Set force towards position", "Force", "Set force %2 towards (%0, %1)", &ExtObject::aSetForceTowardsPosition, "SetForceToward", 0);

	ADDPARAM(PARAM_VALUE, "Gravity", "Choose whether or not gravity is enabled.");
	ADDACT("Set gravity enabled", "Settings", "Set gravity: %0", &ExtObject::aSetGravity, "SetGravityEnabled", 0);

	ADDPARAM(PARAM_VALUE, "Mass", "The new object mass.");
	ADDACT("Set mass", "Settings", "Set mass to %0", &ExtObject::aSetMass, "SetMass", 0);

	ADDPARAM(PARAM_VALUE, "Linear damping", "Enter a value between 0 and 100 for damping the freely moving object.");
	ADDACT("Set linear damping", "Settings", "Set linear damping to %0", &ExtObject::aSetLinearDamp, "SetLinearDamping", 0);

	ADDPARAM(PARAM_VALUE, "Torque", "The rotational force to add to the object.");
	ADDACT("Add torque", "Torque", "Add torque %0", &ExtObject::aAddTorque, "AddTorque", 0);

	ADDPARAM(PARAM_VALUE, "Torque", "The rotational force to set to the object.");
	ADDACT("Set torque", "Torque", "Set torque %0", &ExtObject::aSetTorque, "SetTorque", 0);

	ADDPARAM(PARAM_VALUE, "Torque", "The rotational force to add to the object.");
	ADDPARAM(PARAM_VALUE, "Angle", "The angle to apply torque towards.");
	ADDACT("Add torque towards angle", "Torque", "Add torque %0 towards %1", &ExtObject::aAddTorqueTowardsAngle, "AddTorqueTowards", 0);

	ADDPARAM(PARAM_VALUE, "Torque", "The rotational force to set to the object.");
	ADDPARAM(PARAM_VALUE, "Angle", "The angle to apply torque towards.");
	ADDACT("Set torque towards angle", "Torque", "Set torque %0 towards %1", &ExtObject::aSetTorqueTowardsAngle, "SetTorqueTowards", 0);

	ADDPARAM(PARAM_VALUE, "X", "The X origin of the hinge");
	ADDPARAM(PARAM_VALUE, "Y", "The Y origin of the hinge");
	ADDPARAMDEF(PARAM_VALUE, "Stiffness", "The stiffness of the joint, from 0 to 100.", "90");
	ADDACT("Create hinge to position", "Hinges", "Create hinge about (%0, %1) with stiffness %2", &ExtObject::aHingeToXY, "HingeToXY", 0);

	ADDPARAM(PARAM_OBJECT, "Object to hinge to", "Pick an object with a physics movement to hinge to.");
	ADDPARAM(PARAM_VALUE, "Image point name or number (optional)", "Leave 0 to use the object's position, or enter either the name or the number of the image point.");
	ADDPARAMDEF(PARAM_VALUE, "Stiffness", "The stiffness of the joint, from 0 to 100.", "90");
	ADDACT("Create hinge to object", "Hinges", "Create hinge to %0 (image point %1) with stiffness %2", &ExtObject::aHingeToObj, "HingeToObject", 0);

	ADDPARAM(PARAM_VALUE, "Force of gravity", "The new force of gravity, 9.8 being Earth gravity.");
	ADDACT("Set force of gravity", "Settings", "Set force of gravity to %0", &ExtObject::aSetGravityForce, "SetGravityForce", 0);

	ADDPARAM(PARAM_VALUE, "X component", "The X component of the new velocity.");
	ADDPARAM(PARAM_VALUE, "Y component", "The Y component of the new velocity.");
	ADDACT("Set velocity", "Velocity", "Set velocity to (%0, %1)", &ExtObject::aSetVelocity, "SetVelocity", 0);

	ADDPARAM(PARAM_VALUE, "Angular velocity", "The new angular velocity of the object.");
	ADDACT("Set angular velocity", "Velocity", "Set angular velocity to %0", &ExtObject::aSetOmega, "SetOmega", 0);

	ADDPARAM(PARAM_OBJECT, "Object to disable collisions with", "Choose an object type or family to stop colliding with.");
	ADDACT("Disable collisions with object", "Collisions", "Disable physics collisions with %0", &ExtObject::aDisableCollision, "DisableCollision", 0);

	ADDACT("Reset disabled collisions", "Collisions", "Reset disabled collisions", &ExtObject::aResetDisabledCollisions, "ResetDisabledCollisions", 0);

	ADDPARAM(PARAM_OBJECT, "Object", "Object");
	ADDPARAMDEF(PARAM_VALUE, "k", "Spring constant k","500");
	ADDPARAMDEF(PARAM_VALUE, "Damping", "Damping of the spring", "5");
	ADDPARAMDEF(PARAM_VALUE, "Distance", "Spring distance, in pixels", "64");
	ADDACT("Apply Spring Force", "Spring", "Apply spring force (%0, %1, %2, %3)", &ExtObject::aSpringToBody, "ApplySpring", 0);

	ADDPARAMCOMBO("Immovable", "Choose whether the object is immovable or not", "Yes|No");
	ADDACT("Set immovable", "Settings", "Set immovable: %0", &ExtObject::aSetImmovable, "SetImmovable", 0);

	ADDPARAMCOMBO("No rotation", "Choose whether the object rotates or not", "Yes|No");
	ADDACT("Set rotations disabled", "Settings", "Set rotations disabled: %0", &ExtObject::aSetNoRotations, "SetRotationsDisabled", 0);

	/////////////////////////////
	// Expressions
	ADDEXP("Get mass", "Settings", "Mass", &ExtObject::eGetMass, RETURN_FLOAT);
	ADDEXP("Get linear damping", "Settings", "LinearDamp", &ExtObject::eGetLinearDamp, RETURN_FLOAT);
	ADDEXP("Get world X scale", "Settings", "WorldXscale", &ExtObject::eGetWorldXscale, RETURN_FLOAT);
	ADDEXP("Get world Y scale", "Settings", "WorldYscale", &ExtObject::eGetWorldYscale, RETURN_FLOAT);

	ADDEXP("Get velocity X component", "Velocity", "VelocityX", &ExtObject::eGetVelocityX, RETURN_FLOAT);
	ADDEXP("Get velocity Y component", "Velocity", "VelocityY", &ExtObject::eGetVelocityY, RETURN_FLOAT);
	ADDEXP("Get angular velocity", "Velocity", "AngularVelocity", &ExtObject::eGetOmega, RETURN_FLOAT);

	// ADDEXP(menuID, routine, expressionName, displaytext, flags)
	// Same format as conditions, except with the expression editor text.
	// expressionName = the name in the expression editor, e.g "Width" would
	// associate this expression when the user types YourObject.Width
	// Flags:  RETURN_VALUE, RETURN_STRING, RETURN_UNDEFINED

}
