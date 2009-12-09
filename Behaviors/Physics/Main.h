// Main.h
// Definitions and classes for the object workings

// Only include once
#ifndef MAIN_H
#define MAIN_H

// Movement object
#define IDE_FLAGS OF_MOVEMENTPLUGIN | OF_NODEBUG

#include "..\..\Common\ExpReturn.hpp"
#include "..\..\Common\bin.h"



extern int physicsCount;
#define UNIQUE_PHYSICS_ID 13371337

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)


#define colshape int
#define shape_none 0
#define shape_rectangle 1
#define	shape_circle 2
#define shape_polygon 3



class CollisionShape
{
public:
	CollisionShape(){
		m_Shape = shape_none;
		width = 0;
		height = 0;
		angle = 0;
		radius = 0;
	}

	void Serialize(bin& ar);

	colshape m_Shape;
	vector<POINTF> m_pts; // points are in relation to an object positioned at 0,0 with size 1,1
	float width;
	float height;
	float angle;
	float radius;
};

#ifdef RUN_ONLY

struct HingeToObjectAction {
	class ExtObject* pPhysics;
	float offsetX;
	float offsetY;
	double stiffness;
};

class Box2DCollisionDetector : public b2ContactListener
{
public:
	VRuntime* pRuntime;
	void Add(const b2ContactPoint* point);
};

class Box2DCollisionFilter : public b2ContactFilter
{
public:
	VRuntime* pRuntime;
	bool ShouldCollide(b2Shape* shape1, b2Shape* shape2);
};

template<typename T, typename E>
bool exists_element(T start, T end, const E& elem)
{
	return std::find(start, end, elem) != end;
}

//////////// RUNTIME OBJECT ////////////
// Add any member functions or data you want to this class.
// Your extension inherits CRunObject.  See the definition
// for the default member values you have access to.
class ExtObject : public CRunObject
{
public:
	// Constructor (called when Construct creates the object)
	ExtObject(initialObject* editObject, VRuntime* pVRuntime);
	// Destructor (called when Construct destroys the object)
	~ExtObject();

	//////////////////////////
	// OnFrame: called once per frame just before Draw() (after the events list)
	// OnFrame2: called once per frame just after Draw() (before the events list)
	BOOL		OnFrame();
	BOOL		OnFrame2();
	// Draw: called when Construct wants you to render your object.
	void		Draw();
	// WindowProc is called if your extension creates a window and calls RegisterWindow().
	LRESULT		WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	BOOL		PreTranslateMessage(MSG* msg);
	long		ReturnDefaultValue(LPVAL theParams, ExpReturn& ret);
	long		ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret);
	void		OnCreate();
	// Misc functions
	void		OnFrameChange(int oldFrame, int newFrame);
	void		Serialize(bin& ar);
	// Data functions
	long		GetData(int id, void* param);
	long		CallFunction(int id, void* param);
	// Debugging
	void		DebuggerUpdateDisplay(ExpStore*& pPrivateVars);
	void		OnDebuggerValueChanged(const char* name, const char* value);

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS DEFINITIONS
	long		aAddForce(LPVAL params);
	long		aSetForce(LPVAL params);
	long		aAddForceTowardsPosition(LPVAL params);
	long		aSetForceTowardsPosition(LPVAL params);
	long		aSpringToBody(LPVAL params);

	long		aSetGravity(LPVAL params);
	long		aSetGravityForce(LPVAL params);
	long		aSetMass(LPVAL params);
	long		aSetLinearDamp(LPVAL params);

	long		aAddTorque(LPVAL params);
	long		aSetTorque(LPVAL params);
	long		aAddTorqueTowardsAngle(LPVAL params);
	long		aSetTorqueTowardsAngle(LPVAL params);

	long		aHingeToXY(LPVAL params);
	long		aHingeToObj(LPVAL params);

	long		aSetVelocity(LPVAL params);
	long		aSetOmega(LPVAL params);

	long		aDisableCollision(LPVAL params);
	long		aResetDisabledCollisions(LPVAL params);
	
	long		aSetImmovable(LPVAL params);
	long		aSetNoRotations(LPVAL params);

	long		eGetMass(LPVAL params, ExpReturn& ret);
	long		eGetLinearDamp(LPVAL params, ExpReturn& ret);
	long		eGetWorldXscale(LPVAL params, ExpReturn& ret);
	long		eGetWorldYscale(LPVAL params, ExpReturn& ret);

	long		eGetVelocityX(LPVAL params, ExpReturn& ret);
	long		eGetVelocityY(LPVAL params, ExpReturn& ret);
	long		eGetOmega(LPVAL params, ExpReturn& ret);


	void		SetMass(float mass);

	void		UpdateOldValues();
	void		CheckForBodyChange();


	int physicsID;	// Used to check that a ExtObject* pointer really is a physics instance

	b2Shape *collision;
	b2Body *body;


	static double worldXscale;
	static double worldYscale;
	static double worldGravity;
	static int worldSolver;
	static int worldFriction;
	static b2World *world;
	static b2Body* staticbody;

	float contactFriction;
	float contactElasticity;

	// Fix for calls to hinge actions at unsafe times
	//vector<HingeToObjectAction> waitingHingeActions;
	void DoWaitingHinges();

	template<typename T>
	void TransformFrameToWorld(T& x, T& y) {
		x *= worldXscale;
		y *= worldYscale;
	}
	template<typename T>
	void TransformWorldToFrame(T& x, T& y) {
		x /= worldXscale;
		y /= worldYscale;
	}

	//void SetTransformCallback(const NewtonBody* body, const dFloat* matrix);

	void CreateBody();

	bool gravity;
	bool immovable;
	bool norotation;
	double mass;
	int shape;
	float lineardamp;
	float angulardamp;

	float forceX;
	float forceY;
	float torque;

	float oldforceX;
	float oldforceY;
	float oldtorque;

	float lastKnownX;
	float lastKnownY;
	float lastKnownAngle;
	float lastKnownW;
	float lastKnownH;

	bool do_recreate;

	bool editMode;
	int boolAddingPoints;
	int AddingPointsIndex;
	int AddingPointsCollision;
	vector<CollisionShape> m_CompoundCollisions;

	// For collisions
	static Box2DCollisionDetector* collisionDetector;
	static Box2DCollisionFilter* collisionFilter;

	ObjTypeList disabledCollisions;
};
#endif



//////////// EDITTIME INFO ////////////
class EditExt
{
public:
	///////////////////////////
	// Class data
	EditExt(class VEditTime* pVEditTime, class editInfo* pEInfo);
	~EditExt();

	void Draw();
	int  GetMenuIcons(int ACEMenu);
	void Initialize();
	void OnPut();
	void OnRemoved();
	int  OnShowACEMenu(TABLE ACEType);
	BOOL OnSizeObject();
	void Serialize(bin& ar);
	void OnPropertiesUpdate();
	void OnPropertyUpdated(CString label);
	void OnBeginPropertyRead(CString label);
	void GetAnimationHandle(int&) {}
	void EditCustom();
	void CloseEdit();
	void ClearEdit();
	void OnMessage(int message);

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	bool gravity;
	bool immovable;
	float mass;
	float lineardamp;
	float angulardamp;
	int shape;
	bool norotation;

	// Global settings
	float worldGravity;
	float worldXscale;
	float worldYscale;
	int worldSolver;
	int worldFriction;

	bool editMode;
	int boolAddingPoints;
	int AddingPointsIndex;
	int AddingPointsCollision;
	vector<CollisionShape> m_CompoundCollisions;

	float contactFriction;
	float contactElasticity;
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif