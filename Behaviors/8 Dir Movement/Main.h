// Main.h
// Definitions and classes for the object workings

// Only include once
#ifndef MAIN_H
#define MAIN_H

// Movement object
#define IDE_FLAGS OF_MOVEMENTPLUGIN
#define MINIMUM_VERSION 1

#define DIRMODE_UPDOWN		0
#define DIRMODE_LEFTRIGHT	1
#define DIRMODE_4			2
#define DIRMODE_8			3


#include "..\..\Common\ExpReturn.hpp"
#include "..\..\Common\bin.h"

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
	void DebuggerUpdateDisplay(ExpStore*& pPrivateVars);
	void OnDebuggerValueChanged(const char* name, const char* value);

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS DEFINITIONS

	long cIsMoving		(LPVAL theParams);
	long cOnBounce		(LPVAL theParams);
	long cCompareSpeed	(LPVAL params);

	long aBounce		(LPVAL theParams);
	long aStop			(LPVAL theParams);
	long aSetSpeed		(LPVAL theParams);
	long aSetMaxSpeed	(LPVAL theParams);
	long aSetAcc		(LPVAL theParams);
	long aSetDec		(LPVAL theParams);
	long aSetIgnoreInput(LPVAL theParams);
	long aSetActivated(LPVAL theParams);
	long aSetMAngle(LPVAL theParams);

	long eSpeed(LPVAL theParams, ExpReturn& ret);
	long eMaxSpeed(LPVAL theParams, ExpReturn& ret);
	long eAccRate(LPVAL theParams, ExpReturn& ret);
	long eDecRate(LPVAL theParams, ExpReturn& ret);
	long eMovAngle(LPVAL theParams, ExpReturn& ret);
	long eVectorX(LPVAL theParams, ExpReturn& ret);
	long eVectorY(LPVAL theParams, ExpReturn& ret);

	int directionsMode;	// 0 = 2 (up/down), 1 = 2 (left/right), 2 = 4-dir, 3 = 8-dir

	// Motion vector
	float dx;
	float dy;

	// Key states
	int vk_left, vk_up, vk_down, vk_right;

	bool ignoringInput;
	bool activated;

	float speed;	// read-only, update dx and dy using SetSpeed

	float acc;		// Acceleration
	float dec;		// Deceleration
	float maxspeed;	// Max speed
	int rotateMode;	// 0 = don't rotate, 1 = 8-dir, 2 = true 360
	CRunObjType* pSolid;	// The Solid trait

	int player;

	inline float GetSpeed()
	{
		return sqrt(dx*dx + dy*dy);
	}

	inline void SetSpeed(float s)
	{
		if (s > maxspeed)
			s = maxspeed;
		if (s < 0)
			s = 0;

		speed = s;

		// axis aligned optimisation
		if (dx == 0 && dy != 0) {
			if (dy < 0)
				dy = -s;
			else
				dy = s;
		}
		else if (dx != 0 && dy == 0) {
			if (dx < 0)
				dx = -s;
			else
				dx = s;
		}
		// adjust the vector
		else {
			float angle = atan2(dy,dx);
			// using hypotenuse 's', generate dx and dy
			dx = s * cos(angle);
			dy = s * sin(angle);
		}
		
	}

	bool IsOverlappingAndRegister(CRunObjType* pSolid);
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
	void Initialize();
	void OnPut();
	void OnRemoved();
	void GetAnimationHandle(int& iHandle);
	BOOL OnSizeObject();
	void Serialize(bin& ar);
	void OnPropertiesUpdate();

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	float acc;
	float dec;
	float maxspeed;
	int rotateMode;	// 0 = don't rotate, 1 = 8-dir, 2 = true 360
	int directionsMode;	// 0 = 2 (up/down), 1 = 2 (left/right), 2 = 4-dir, 3 = 8-dir

	int player;
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif