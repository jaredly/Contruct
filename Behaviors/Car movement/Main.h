// Main.h
// Definitions and classes for the object workings

// Only include once
#ifndef MAIN_H
#define MAIN_H

// Movement object
#define IDE_FLAGS OF_MOVEMENTPLUGIN


#include "..\..\Common\ExpReturn.hpp"

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
	void		DebuggerUpdateDisplay(ExpStore*& pPrivateVars);
	void		OnDebuggerValueChanged(const char* name, const char* value);

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS DEFINITIONS

	long cIsMoving		(LPVAL theParams);
	long cOnBounce		(LPVAL theParams);

	long aBounce		(LPVAL theParams);
	long aStop			(LPVAL theParams);
	long aSetSpeed		(LPVAL theParams);
	long aSetMaxSpeed	(LPVAL theParams);
	long aSetAcc		(LPVAL theParams);
	long aSetDec		(LPVAL theParams);
	long aSetIgnoreInput(LPVAL theParams);
	long aSetActivated(LPVAL theParams);
	long aSetSkidRecover(LPVAL theParams);
	long aSetRotateSpeed(LPVAL theParams);

	long eSpeed(LPVAL theParams, ExpReturn& ret);
	long eMaxSpeed(LPVAL theParams, ExpReturn& ret);
	long eAccRate(LPVAL theParams, ExpReturn& ret);
	long eDecRate(LPVAL theParams, ExpReturn& ret);
	long eMovAngle(LPVAL theParams, ExpReturn& ret);
	long eVectorX(LPVAL theParams, ExpReturn& ret);
	long eVectorY(LPVAL theParams, ExpReturn& ret);
	long eGetRotateSpeed(LPVAL theParams, ExpReturn& ret);

	// Key states
	int vk_left, vk_right, vk_acc, vk_dec;

	bool ignoringInput;
	bool activated;

	float steer;
	//note: need a turn rate
	// and a max turn

	// also i've seen really awsome car movements where the car actually drives like the wheels are 
	// at the front and not like the wheels are in the middle so i might investigate that for more
	// realism...i remember there was a 'girls cant park' game that used it

	float speed;

	float mAngle;	// Angle of momentum (gives skids when differs from info.Angle)
					// in RADIANS

	float av;		// angular velocity of rotations (rads/frame)

	float acc;		// Acceleration
	float dec;		// Deceleration
	float maxspeed;	// Max speed

	float skidrecover;	// Skid recover rate

	int player;

	CRunObjType* pSolid;
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
	BOOL OnSizeObject();
	void Serialize(bin& ar);
	void OnPropertiesUpdate();
	void GetAnimationHandle(int& iHandle) { iHandle = -1; }

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	float av;		// angular velocity
	float acc;		// Acceleration
	float dec;		// Deceleration
	float maxspeed;	// Max speed

	float skidrecover;
	int player;
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif