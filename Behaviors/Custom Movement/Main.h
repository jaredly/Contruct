// Main.h
// Definitions and classes for the object workings

// Only include once
#ifndef MAIN_H
#define MAIN_H

#define MINIMUM_VERSION	1

// IDE FLAGS
// Can be a combination of:
//OF_NOSIZE	- Extension cannot be resized at edittime
//OF_NODRAW - Extension does not display anything
//OF_ALLOWANGLES - Can be rotated at edittime
//OF_PRIVATEVARIABLES - Uses private variables
//OF_MOVEMENTPLUGIN - Is a movement plugin
//OF_MOVEMENTS - Allow movement plugins to be added
//OF_EFFECTS - Allow display effects
//OF_NOCOMMONDEBUG - Don't show 'Common' field (X/Y/W/H/Angle/Opacity) in debugger
//OF_NODEBUG - Don't show at all in debugger
//OF_UNDEFINEDEXPRESSIONS - Allow ReturnUndefinedExpression to be called
#define IDE_FLAGS 	OF_MOVEMENTPLUGIN

#include "..\..\Common\ExpReturn.hpp"



// Handy functions
void VectorToComponent(float dx, float dy, float& speed, float& angle, float normal = 0);
void ComponentToVector(float speed, float angle, float& dx, float& dy, float normal = 0);
float DirectionToRadians(int direction);
bool RotateTowardsIsPositive(float start, float end);
float RotateTowards(float start, float end, float step);
float AngleToFrom(float targetx, float targety, float x, float y);
float SetAddAccelerateDecelerate(float initial, float value, float timedelta, int mode);

bool Between(float lower, float value, float upper);
bool PushOut(CRunObject* pLink, CRunObjType* pTarget, float angle, float prevx, float prevy);
void PushOutClosest(CRunObject* pLink, CRunObjType* pTarget);
float CalculateNormal(CRunObject* obj, CRunObjType* bounceOff, bool* concave = NULL);

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)

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
	// Return default expression editor value and undefined expressions when enabled.
	long		ReturnDefaultValue(LPVAL params, ExpReturn& ret);
	long		ReturnUndefinedExpression(CString& expName, LPVAL params, ExpReturn& ret);
	// Called just after the constructor when your object has been prepared.
	void		OnCreate();
	// Called when a frame changes and your object is global
	void		OnFrameChange(int oldFrame, int newFrame);
	// Called when your object should save/load itself at runtime
	void		Serialize(bin& ar);
	// Data functions
	long		GetData(int id, void* param);
	long		CallFunction(int id, void* param);
	// Debugging
	void		DebuggerUpdateDisplay(ExpStore*& pPrivateVars);
	void		OnDebuggerValueChanged(const char* name, const char* value);

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS DEFINITIONS
#include "..\..\Common\CommonAceDecl.hpp"

	////////////////////////////////////////////////////
	// CONDITIONS
	long cOnStep (LPVAL theParams);
	long cOnHorizontalStep (LPVAL theParams);
	long cOnVerticalStep (LPVAL theParams);

	////////////////////////////////////////////////////
	// ACTIONS

	// Motion Angle
	long aChangeMotionDirection			(LPVAL theParams);
	long aChangeMotionAngle				(LPVAL theParams);
	long aChangeMotionAngleBy			(LPVAL theParams);
	long aChangeMotionAngleTowardsPos	(LPVAL theParams);
	long aChangeMotionAngleTowardsObj	(LPVAL theParams);

	// Motion Rotation
	long aChangeMotionRotateSpeed					(LPVAL theParams);
	long aChangeMotionRotateSpeedTowardsDirection	(LPVAL theParams);
	long aChangeMotionRotateSpeedTowardsAngle		(LPVAL theParams);
	long aChangeMotionRotateSpeedTowardsPos			(LPVAL theParams);
	long aChangeMotionRotateSpeedTowardsObj			(LPVAL theParams);

	// Speed
	long aChangeSpeed					(LPVAL theParams);
	long aChangeHorizontalSpeed			(LPVAL theParams);
	long aChangeVerticalSpeed			(LPVAL theParams);
	long aChangeSpeedTowardsDirection	(LPVAL theParams);
	long aChangeSpeedTowardsAngle		(LPVAL theParams);
	long aChangeSpeedTowardsPosition	(LPVAL theParams);
	long aChangeSpeedTowardsObject		(LPVAL theParams);

	// Friction
	long aAddFriction					(LPVAL theParams);
	long aAddFrictionHorizontally		(LPVAL theParams);
	long aAddFrictionVertically			(LPVAL theParams);
	long aAddFrictionToMotionRotation	(LPVAL theParams);

	// Collisions
	long aPushOut						(LPVAL theParams);
	long aPushOutAtAngle				(LPVAL theParams);
	long aPushOutInDirection			(LPVAL theParams);
	long aReverse						(LPVAL theParams);
	long aBounce						(LPVAL theParams);

	// Movement
	long aSetMovementStyle				(LPVAL theParams);
	
	// Distance
	long aRestrainDistanceFromPosition	(LPVAL theParams);
	long aRestrainDistanceFromObject	(LPVAL theParams);
	long aScaleDistanceFromPosition		(LPVAL theParams);
	long aScaleDistanceFromObject		(LPVAL theParams);

	long aStopStep						(LPVAL params);

	////////////////////////////////////////////////////
	// EXPRESSIONS
	long eSpeed							(LPVAL theParams, ExpReturn& ret);
	long eAngle							(LPVAL theParams, ExpReturn& ret);
	long eXspeed						(LPVAL theParams, ExpReturn& ret);
	long eYSpeed						(LPVAL theParams, ExpReturn& ret);

	////////////////////////////////////////////////////
	void MoveWithStepping(float dx, float dy,  int steps, int stepStyle);
	void MoveWithSubStepping(float dx, float dy,  int steps);
	void MoveWithPixelStepping(float dx, float dy,  int pixelStep);



	////////////////////////////////////////////////////
	// Data members

	float m_speed;			// pixels per second
	float m_angle;			// Radians
	float m_rotation;		// Radians per second

	float m_prevx;
	float m_prevy;

	float m_startx;
	float m_starty;

	int movestyle;
	int stepstyle;
	int steps;

	bool m_StopStep;

	bool activated;			// Movement is activated
};

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
	void GetAnimationHandle(int& handle);

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	///////////////////////////
	// Your edittime extension data goes here
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif