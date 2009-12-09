// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

static int physicsCount;
static __int64 lastFrameCountUpdate = -1;

b2World* ExtObject::world;
double ExtObject::worldXscale;
double ExtObject::worldYscale;
double ExtObject::worldGravity;
int ExtObject::worldSolver;
int ExtObject::worldFriction;
Box2DCollisionDetector* ExtObject::collisionDetector;
Box2DCollisionFilter* ExtObject::collisionFilter;

b2Body* ExtObject::staticbody;

#define POW2(x) ((x)*(x))

// ExtObject constructor:
// Called when Construct creates an instance of your object.  Use your editTime data
// to initialise the object.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
{
	pRuntime = pVRuntime;
	info.editObject = editObject;

	//body = NULL;
	//upvector = NULL;
	//hinge = NULL;
	physicsID = UNIQUE_PHYSICS_ID;
}

void Box2DCollisionDetector::Add(const b2ContactPoint *point)
{
	ExtObject* a = (ExtObject*)point->shape1->GetUserData();
	ExtObject* b = (ExtObject*)point->shape2->GetUserData();

	// Register collisions between the linked actual instances, not the Physics Movement instances
	pRuntime->RegisterCollision(a->pLink, b->pLink);
}

bool Box2DCollisionFilter::ShouldCollide(b2Shape *shape1, b2Shape *shape2)
{
	ExtObject* a = (ExtObject*)shape1->GetUserData();
	ExtObject* b = (ExtObject*)shape2->GetUserData();

	// Shape 2 = 'none' for collision mask - simulate but don't collide
	if (a->shape == 2 || b->shape == 2)
		return false;

	// Neither have disabled anything: ok to go
	if (a->disabledCollisions.empty() && b->disabledCollisions.empty())
		return true;

	// b's type exists in a's disabled list
	if (exists_element(a->disabledCollisions.begin(), a->disabledCollisions.end(), b->pLink->pType))
		return false;

	// a's type exists in b's disabled list
	if (exists_element(b->disabledCollisions.begin(), b->disabledCollisions.end(), a->pLink->pType))
		return false;

	return true;
}

/*
// add force and torque to rigid body
void PhysicsApplyForceAndTorque(const NewtonBody* body)
{
	ExtObject* pMov = (ExtObject*)NewtonBodyGetUserData(body);

	if (pMov->gravity)
		pMov->forceY += pMov->worldGravity * pMov->mass;		// f = mg

	if (pMov->forceX != 0.0 || pMov->forceY != 0.0) {
		//NewtonWorldUnfreezeBody(pMov->nWorld, body);
		dVector force(pMov->forceX, pMov->forceY, 0.0);
		NewtonBodySetForce(body, &force[0]);
	}

	if (pMov->torque != 0.0) {
		dVector torqueVec(0.0, 0.0, pMov->torque);
		NewtonBodySetTorque(body, &torqueVec[0]);

		pMov->torque = 0.0;
	}
}
*/
/*
dFloat AngleFromMatrix(dFloat m11, dFloat m21, dFloat m31)
{
	dFloat beta = atan2(-m31, sqrt(m11*m11 + m21*m21));
	dFloat alpha = atan2(m21 / cos(beta), m11 / cos(beta));
	return alpha;
}

void MatrixFromAngle(dFloat radians, dFloat& m11, dFloat& m21, dFloat& m31)
{
	dFloat cosAng = cos(radians);
	dFloat sinAng = sin(radians);

	m11 = cosAng;
	m21 = sinAng;
	m31 = 0.0f;
}

void ExtObject::SetTransformCallback(const NewtonBody* body, const dFloat* matrix)
{
	dFloat unused;

	dFloat mat[16];
	memcpy(mat, matrix, sizeof(dFloat) * 16);
	mat[2] = 0;
	mat[6] = 0;
	mat[8] = 0;
	mat[9] = 0;
	mat[10] = 1;
	mat[14] = 0;

	// Check if the object's angle or position has been changed
	if (lastKnownX != pLink->info.x) {
		mat[12] = pLink->info.x;
		TransformFrameToWorld(mat[12], unused);
	}

	if (lastKnownY != pLink->info.y) {
		mat[13] = pLink->info.y;
		TransformFrameToWorld(unused, mat[13]);
	}
	

	
	
	NewtonBodySetMatrix(body, mat);

	pLink->info.x = mat[12];
	pLink->info.y = mat[13];
	pLink->info.angle = DEGREES(AngleFromMatrix(mat[0], mat[1], mat[2]));

	TransformWorldToFrame(pLink->info.x, pLink->info.y);
	pRuntime->UpdateBoundingBox(pLink);

	lastKnownX = pLink->info.x;
	lastKnownY = pLink->info.y;
}

// set the transformation of a rigid body
void PhysicsSetTransform(const NewtonBody* body, const dFloat* matrix)
{
	ExtObject* pMov = (ExtObject*)NewtonBodyGetUserData(body);
	pMov->SetTransformCallback(body, matrix);
}
*/
float MakePtrFloat(void* ptr)
{
	int data = (int)ptr;
	return *((float*)&data);
}
void* MakeFloatPtr(float x)
{
	void* ret;
	*((float*)&ret) = x;
	return ret;
}
/*
*/
// This is called just after the constructor, when Construct has added details to your
// object such as siblings and type pointer.  If you need this data on startup, it is
// valid to read here.
void ExtObject::OnCreate()
{
	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	float fMass;

	int Version = 0;
	ar >> Version;

	ar >> gravity >> fMass >> immovable >> lineardamp >> shape >> angulardamp
		>> norotation;

	// Default values for older versions
	contactFriction = 0.8f;
	contactElasticity = 0.4f;

	if(Version >= 2){
				ar >> editMode
			 >> boolAddingPoints
			 >> AddingPointsIndex
			 >> AddingPointsCollision;
			int size = m_CompoundCollisions.size();
			ar >> size;
			for(int i = 0; i < size; i++)
			{
				CollisionShape myshape;
				myshape.Serialize(ar);
				m_CompoundCollisions.push_back(myshape);
			}
	}
	if(Version >= 3){
		ar >> contactFriction
			>> contactElasticity;
	}

	mass = fMass;
	worldGravity = MakePtrFloat(pRuntime->GetLayoutKey(pLayout, "worldGravity"));
	worldXscale = MakePtrFloat(pRuntime->GetLayoutKey(pLayout, "worldXscale"));
	worldYscale = MakePtrFloat(pRuntime->GetLayoutKey(pLayout, "worldYscale"));

	worldSolver = (int)pRuntime->GetLayoutKey(pLayout, "worldSolver");
	worldFriction = (int)pRuntime->GetLayoutKey(pLayout, "worldFriction");

	ar.detach();

	forceX = 0;
	forceY = 0;
	torque = 0;
	//hinge = NULL;

	// Create the physics world if no other object has.
	if (physicsCount == 0) {
		/*
		nWorld.Create();

		NewtonSetPlatformArchitecture(nWorld, 2);	// Attempt to use best platform config

		int solver = 0;

		switch (worldSolver) {
		case 0:		// Linear
			solver = 8;		// 8-pass linear
			break;
		case 1:		// Adaptive
			solver = 1;		// Adaptive
			break;
		case 2:		// Exact
			solver = 0;	// Exact
			break;
		}

		// Default: exact friction model
		int friction = 0;

		// User picked 'Adaptive'
		if (worldFriction == 0)
			friction = 1;		// Adaptive

		NewtonSetSolverModel(nWorld, solver);
		NewtonSetFrictionModel(nWorld, friction);

		dVector worldBegin(-100, -100, -10.0);
		dVector worldEnd(300, 300, 10.0);
		NewtonSetWorldSize(nWorld, &worldBegin[0], &worldEnd[0]);*/
	
		b2AABB worldAABB;
		worldAABB.lowerBound.Set(-100, -100);
		worldAABB.upperBound.Set(400.0f, 400.0f);

		// Define the gravity vector.
		b2Vec2 gravity(0.0f, 0.0f);

		bool doSleep = true;

		world = new b2World(worldAABB, gravity, doSleep);
		collisionDetector = new Box2DCollisionDetector;
		collisionDetector->pRuntime = pRuntime;
		collisionFilter = new Box2DCollisionFilter;
		collisionFilter->pRuntime = pRuntime;

		world->SetContactListener(collisionDetector);
		world->SetContactFilter(collisionFilter);

		//b2World::s_enablePositionCorrection = true;
		//b2World::s_enableWarmStarting = true;
		
		// We need a static body so joints can connect to the world
		b2BodyDef BodyDef;
		staticbody = world->CreateBody(&BodyDef);
	}

	physicsCount++;

	//worldXscale = 1.0f;
	//worldYscale = 1.0f;

	CreateBody();
	//body = NULL;
	do_recreate = false;
	
	//int def = NewtonMaterialGetDefaultGroupID(nWorld);

	//NewtonMaterialSetContinuousCollisionMode(nWorld, def, def, 1);
	//NewtonSetMinimumFrameRate(nWorld,120);
}

void ExtObject::CreateBody()
{
	//MessageBox(0,0,0,0);
//	float moment;

	float Mass = mass;// * pLink->info.w * pLink->info.h;
	if(immovable)
		Mass = 0.0f;


	// Create a collision body.
	/*switch (shape) {
		case 0:		// Bounding box
			{*/
		/*	cpVect verts[] = 
				{
				cpv(- pLink->info.w * worldXscale/2,  - pLink->info.h * worldYscale/2),
				cpv(- pLink->info.w * worldXscale/2,    pLink->info.h * worldYscale/2),
				cpv(pLink->info.w * worldXscale/2  ,    pLink->info.h * worldYscale/2),
				cpv(pLink->info.w * worldXscale/2  ,  - pLink->info.h * worldYscale/2)
				};	*/
			
	b2PolygonDef ShapeDefBox;
	b2PolygonDef ShapeDefCustom;
	b2CircleDef ShapeDefCircle;
	b2ShapeDef* pShapeDef = NULL;

	if(shape == 0 || shape == 2) { // Shape 2 is 'none', but it needs a shape to simulate.  The collision filter makes it not collide.
		ShapeDefBox.SetAsBox(pLink->info.w * worldXscale/2.0, pLink->info.h * worldYscale/2.0);
		
		pShapeDef = &ShapeDefBox;
	}
	else if(shape == 1){
		if(pLink->info.w != pLink->info.h)
		{
			shape = 3;
			pShapeDef = &ShapeDefCustom;

			m_CompoundCollisions.clear();
			m_CompoundCollisions.push_back(CollisionShape());
			CollisionShape& ellipse = m_CompoundCollisions[0];
			ellipse.m_Shape = shape_circle;
			ellipse.width = pLink->info.w;
			ellipse.height = pLink->info.h;
			ellipse.angle = 0;
		}
		else{
			ShapeDefCircle.radius = pLink->info.w * worldXscale/2.0;
			pShapeDef = &ShapeDefCircle;
		}
	}
	else if(shape == 3){
		pShapeDef = &ShapeDefCustom;
	}

	if (pShapeDef != NULL) {
		pShapeDef->density = Mass;
		pShapeDef->friction = contactFriction;
		pShapeDef->restitution = contactElasticity;
		pShapeDef->userData = this;

		if(immovable)
			pShapeDef->density = 0;
	}

	b2BodyDef BodyDef;
	double HotSpotAngle = atan2((float)(pLink->info.HotSpotX - pLink->info.w / 2.0f), (float)(pLink->info.HotSpotY - pLink->info.h / 2.0f));
	double HotSpotDist = sqrt((double)POW2(pLink->info.HotSpotX - pLink->info.w / 2.0f) + POW2(pLink->info.HotSpotY - pLink->info.h / 2.0f));
	float hotspotOffsetX = cos(HotSpotAngle + RADIANS(pLink->info.angle)) * HotSpotDist;
	float hotspotOffsetY = sin(HotSpotAngle + RADIANS(pLink->info.angle)) * HotSpotDist;




	BodyDef.position.Set((pLink->info.x - hotspotOffsetX) * worldXscale , (pLink->info.y - hotspotOffsetY)* worldYscale);
	BodyDef.angle = RADIANS(pLink->info.angle);
	BodyDef.isBullet = false;
	if(norotation)
		BodyDef.fixedRotation = true;

	BodyDef.linearDamping = lineardamp;
	BodyDef.angularDamping = angulardamp;
	
		
	body = world->CreateBody(&BodyDef);

	if(shape == 0 || shape == 1 || shape == 2)
		body->CreateShape(pShapeDef);

	// custom collision (if theres any)
	if(shape == 3)
	{
		float w = pLink->info.w * worldXscale; 
		float h = pLink->info.h * worldYscale; 

		float xo = hotspotOffsetX * worldXscale;
		float yo = hotspotOffsetY * worldYscale;

		for(vector<CollisionShape>::iterator i = m_CompoundCollisions.begin(); i!= m_CompoundCollisions.end(); i++)
		{
			switch (i->m_Shape)
			{
				case shape_polygon:
					{
						vector< b2Vec2> vert;
						for(vector<POINTF>::iterator p = i->m_pts.begin(); p != i->m_pts.end(); p++)
						{
							vert.push_back(b2Vec2(p->x * w + xo*2, p->y * h + yo*2));
						}
						if(vert.size())
						{
							vert.push_back(vert[0]);
						}
						b2Polygon pgon(&vert[0], vert.size());
						DecomposeConvexAndAddTo(&pgon, body, (b2PolygonDef*)pShapeDef);
					}
				case shape_circle:
					{
						vector< b2Vec2> vert;
						double w = i->width * worldXscale;
						double h = i->height * worldYscale;
						double a = i->angle;

						for(int p = 0; p <= 32; p++)
						{
							double angle = RADIANS(p * 360 / 32.0f);
							if(p == 32) angle = 0;
							vert.push_back(  b2Vec2(sin(angle) * w/2 + xo, cos(angle) * h/2 + yo) );
						}
						b2Polygon pgon(&vert[0], vert.size());
						DecomposeConvexAndAddTo(&pgon, body, (b2PolygonDef*)pShapeDef);
					}
				break;
			}
		}
	}
	
	body->SetMassFromShapes();

	// Set the object's last known x/y/a, so we can keep track of user changes
	lastKnownX = pLink->info.x;
	lastKnownY = pLink->info.y;
	lastKnownAngle = pLink->info.angle;
	lastKnownW = pLink->info.w;
	lastKnownH = pLink->info.h;
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
	if(body)
	{
		world->DestroyBody(body);
		body = NULL;
	}
	/*
	if (hinge != NULL)
		NewtonDestroyJoint(nWorld, hinge);

	if (upvector != NULL)
		NewtonDestroyJoint(nWorld, upvector);

	NewtonDestroyBody(nWorld, body);*/

	physicsCount--;
	if (physicsCount == 0){
		if (world) {
			delete world;
			world = NULL;
		}
		if (collisionDetector) {
			delete collisionDetector;
			collisionDetector = NULL;
		}
		if (collisionFilter) {
			delete collisionFilter;
			collisionFilter = NULL;
		}
	}
	
}

// Called every frame for you to update your object if necessary
// Return values: 1 (do not call RTOnFrame again), 0 (continue calling)

// Change actual co-ordinates and cause rotation in OnFrame since these must both be
// overridable in the following events

void ExtObject::CheckForBodyChange()
{
	if(!body) CreateBody();

	b2Vec2 position = body->GetPosition();
	float32 rotation = body->GetAngle();
	b2Vec2 velocity = body->GetLinearVelocity();
	float32 omega = body->GetAngularVelocity();

	float unused;

	// Check for dimension changes
	if (lastKnownW != pLink->info.w) {
		lastKnownW = pLink->info.w;
		do_recreate = true;
	}
	if (lastKnownH != pLink->info.h) {
		lastKnownH = pLink->info.h;
		do_recreate = true;
	}

	if (do_recreate)
	{
		world->DestroyBody(body);
		CreateBody();

		body->SetAngularVelocity(omega);
		body->SetLinearVelocity(velocity);

		do_recreate = false;
	}

	if (lastKnownX != pLink->info.x) {
		position.x = pLink->info.x;
		TransformFrameToWorld(position.x, unused);
		body->SetXForm(position, rotation);
	}

	if (lastKnownY != pLink->info.y) {
		position.y = pLink->info.y;
		TransformFrameToWorld(unused, position.y);
		body->SetXForm(position, rotation);
	}

	if(lastKnownAngle != pLink->info.angle) {
		rotation = RADIANS(pLink->info.angle);
		body->SetXForm(position, rotation);
	}
}

void ExtObject::UpdateOldValues()
{
	if(!body) CreateBody();
	b2Vec2 position = body->GetPosition();
	float32 rotation = body->GetAngle();



	pLink->info.x = position.x;
	pLink->info.y = position.y;
	pLink->info.angle = DEGREES(rotation);

	TransformWorldToFrame(pLink->info.x, pLink->info.y);
	
	double HotSpotAngle = atan2((float)(pLink->info.HotSpotX - pLink->info.w / 2.0f), (float)(pLink->info.HotSpotY - pLink->info.h / 2.0f));
	double HotSpotDist = sqrt((double)POW2(pLink->info.HotSpotX - pLink->info.w / 2.0f) + POW2(pLink->info.HotSpotY - pLink->info.h / 2.0f));
	float hotspotOffsetX = cos(HotSpotAngle + RADIANS(pLink->info.angle)) * HotSpotDist;
	float hotspotOffsetY = sin(HotSpotAngle + RADIANS(pLink->info.angle)) * HotSpotDist;

	pLink->info.x += hotspotOffsetX;
	pLink->info.y += hotspotOffsetY;

	pRuntime->UpdateBoundingBox(pLink);

	lastKnownX = pLink->info.x;
	lastKnownY = pLink->info.y;
	lastKnownAngle = pLink->info.angle;
}


BOOL ExtObject::OnFrame()
{
	if(!body) CreateBody();
	CheckForBodyChange();


	if (lastFrameCountUpdate != pLayout->frameCounter64) {
		lastFrameCountUpdate = pLayout->frameCounter64;

		const int steps = 10;
		world->Step(pRuntime->GetTimeDelta(), steps);
		
	}

	forceX *= 0.05;
	forceY *= 0.05;

	bool sleeping = body->IsSleeping();

	if (gravity)
		forceY += worldGravity * 0.4 * body->GetMass();		// f = mg

	if (forceX != 0.0 || forceY != 0.0) {
		b2Vec2 force; 
		force.x = forceX; 
		force.y = forceY;
		b2Vec2 zero(0,0);
		bool sleeping = body->IsSleeping();
		body->ApplyForce(force, body->GetWorldCenter());
		if(sleeping && forceX == oldforceX && forceY == oldforceY)
			body->PutToSleep();
	}

	torque *= 0.035;
	if (torque != 0.0) {

		bool sleeping = body->IsSleeping();
		body->ApplyTorque(torque);
		if(sleeping && torque == oldtorque)
			body->PutToSleep();
	}

	UpdateOldValues();

	oldforceX = forceX;
	oldforceY = forceY;
	oldtorque = torque;

	forceX = 0.0;
	forceY = 0.0;
	torque = 0.0;

	
	return 0;
}

BOOL ExtObject::OnFrame2()
{
	return 0;

}

// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your extension doesn't draw anything.
void ExtObject::Draw()
{
	//
}

// WindowProc:  called when a window message, or WM_COMMAND specifying your window,
// is sent to Construct.  You must call RegisterWindow() before Construct will send
// you messages for the given window.
LRESULT ExtObject::WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	return -2;
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

void ExtObject::Serialize(bin& ar)
{
	if (ar.loading) {
		ar >> gravity >> immovable >> norotation >> mass >> shape >> lineardamp >> angulardamp;

		CreateBody();
	}
	else {
		ar << gravity << immovable << norotation << mass << shape << lineardamp << angulardamp;
	}
}

long ExtObject::GetData(int id, void* param)
{
	switch (id) {
	// If your extension uses a window, you can return it here
	case GET_WINDOW:
		break;
	}

	return 0;
}

long ExtObject::CallFunction(int id, void* param)
{
	return 0;
}

// Return the Default Value.  This is the value to be returned if your object name
// is used without an expression name, eg. in "Counter + 1".
// Parameters can be passed, eg. MyArray(3,7).
long ExtObject::ReturnDefaultValue(LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

// Called for undefined expression names, if your extension allows it.
// Eg: MyObject.UndefinedExpression(3,4,5) calls this routine with
// "UndefinedExpression" as the expression name.
long ExtObject::ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

void ExtObject::DebuggerUpdateDisplay(ExpStore*& pPrivateVars)
{
	pRuntime->AddDebuggerItem("Mass", mass, false);
	pRuntime->AddDebuggerItem("Linear damping", lineardamp * 100.0, false);
	pRuntime->AddDebuggerItem("Angular damping", angulardamp * 100.0, false);
	pRuntime->AddDebuggerItem("World gravity", worldGravity, false);
}

void ExtObject::OnDebuggerValueChanged(const char* name, const char* value)
{
/*	CString label = name;
	CString text = value;
	__int64 iValue = _atoi64(text);
	double fValue = atof(text);

	if (label == "Mass")
		SetMass(fValue);
	else if (label == "Linear damping") {

		NewtonBodySetLinearDamping(body, fValue / 100.0);

		NewtonWorldUnfreezeBody(nWorld, body);

	}
	else if (label == "Angular damping") {

		dVector angularDamp(fValue / 100.0, fValue / 100.0, fValue / 100.0);
		NewtonBodySetAngularDamping(body, &(angularDamp[0]));

		angulardamp = fValue / 100.0;

		NewtonWorldUnfreezeBody(nWorld, body);

	}
	else if (label == "World gravity")
		worldGravity = fValue;*/
}

#else //ifdef RUN_ONLY

CRunObject* WINAPI RTCreateObject(bin& ar, VRuntime* pRuntime) 
{
	return NULL;
}

void WINAPI RTDestroyObject(class ExtObject* object)
{
	//
}

#endif