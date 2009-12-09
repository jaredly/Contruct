// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

// ExtObject constructor:
// Called when Construct creates an instance of your object.  Use your editTime data
// to initialise the object.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
{
	pRuntime = pVRuntime;
	info.editObject = editObject;
}

// This is called just after the constructor, when Construct has added details to your
// object such as siblings and type pointer.  If you need this data on startup, it is
// valid to read here.
void ExtObject::OnCreate()
{
	// Defaults
	steer = 0;
	
	speed = 0.0f;

	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	int Version = 0;
	ar >> Version;

	ar >> acc >> dec >> maxspeed >> av >> skidrecover;

	if (Version >= 2)
		ar >> player;
	else
		player = 0;

	ar.detach();

	// Momentum starts at object angle
	mAngle = info.angle;

	// Set up controls
	vk_left = pRuntime->GetControlVK("Steer left", player);
	vk_right = pRuntime->GetControlVK("Steer right", player);
	vk_acc = pRuntime->GetControlVK("Accelerate", player);
	vk_dec = pRuntime->GetControlVK("Brake", player);
	ignoringInput = false;
	activated = true;

	// Defaults
	if (vk_left == -1) vk_left = VK_LEFT;
	if (vk_right == -1) vk_right = VK_RIGHT;
	if (vk_acc == -1) vk_acc = VK_UP;
	if (vk_dec == -1) vk_dec = VK_DOWN;

	pSolid = pRuntime->GetTraitFromName("Solid");
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
	//
}

float RotateTowards(float start, float end, float step)
{
   float cs = cos(RADIANS(start));
   float ce = cos(RADIANS(end));
   float ss = sin(RADIANS(start));
   float se = sin(RADIANS(end));

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

// Called every frame for you to update your object if necessary
// Return values: 1 (do not call RTOnFrame again), 0 (continue calling)
// The frame call order goes like this:
// run events -> OnFrame -> Draw -> OnFrame2 -> run events -> ...
BOOL ExtObject::OnFrame()
{
	// Modify the object (first, to prevent glitches)
	// (angle in degrees)
	float angle = pLink->info.angle;

	float timedelta = pRuntime->GetTimeDelta();

	float left = 0;
	float right = 0;
	float up = 0;
	float down = 0;

	if (!ignoringInput) {
		left =  pRuntime->GetControlState("Move Left", player);
		right =  pRuntime->GetControlState("Move Right", player);
		up =  pRuntime->GetControlState("Accelerate", player);
		down =  pRuntime->GetControlState("Brake", player);
	}

	if (GetFocus() != pRuntime->GetFrameHwnd(pLink->pLayout)) {
		left = right = up = down = 0;
	}

	float oldX = pLink->info.x;
	float oldY = pLink->info.y;
	bool invertSpeed = false;

	float distToMove = speed * timedelta;

	// Check X motion allowed
	pLink->info.x += cos(mAngle) * distToMove;
	pRuntime->UpdateBoundingBox(pLink);

	if (pRuntime->QueryCollision(pLink, pSolid)) {
		pLink->info.x = oldX;
		pRuntime->UpdateBoundingBox(pLink);
		invertSpeed = true;
	}

	pLink->info.y += sin(mAngle) * distToMove;
	pRuntime->UpdateBoundingBox(pLink);

	if (pRuntime->QueryCollision(pLink, pSolid)) {
		pLink->info.y = oldY;
		pRuntime->UpdateBoundingBox(pLink);
		invertSpeed = true;
	}

	if (invertSpeed) {
		speed *= -1;

		pRuntime->GenerateEvent(info.oid, 1, this);
	}

	// Modify speed
	if (up && speed < maxspeed) {

		if (acc < 0)
			speed = maxspeed;
		else {
			speed += acc * timedelta;

			if (speed > maxspeed)
				speed = maxspeed;
		}
	}
	// slow down
	if (!up && speed > 0) {

		if (dec < 0)
			speed = 0;
		else {
			speed -= dec * timedelta;

			if (speed < 0)
				speed = 0;
		}
	}

	// Reverse
	if (down && speed > -maxspeed) {

		if (acc < 0)
			speed = -maxspeed;
		else {
			speed -= acc * timedelta* down;

			if (speed < -maxspeed)
				speed = -maxspeed;
		}
	}
	// slow down reverse
	if (!down && speed < 0) {

		if (dec < 0)
			speed = 0;
		else {
			speed += dec * timedelta* down;

			if (speed > 0)
				speed = 0;
		}
	}

	// Invert angle for reverses
	//float turnmodifier = 1.0f;//speed/4.0;// speed < 0 ? -1.0f : 1.0f;
	float turnmodifier = min(1,max(-1,speed/350.0f)); 
													// also tigs i added the min and max thing (cant remember the macro for value capping) because when u skid u tend to not turn fast anyway

	if(left)
		steer = -av * timedelta;
	if(right)
		steer = av * timedelta;
	
	if (left)	// counterclockwise
		angle += steer * turnmodifier * left;
	if (right)	// clockwise
		angle += steer * turnmodifier * right;

	// Normalise the angle (-180 to 180)
	if (angle > 180.0f) angle -= 360.0f;
	if (angle <= -180.0f) angle += 360.0f;

	if (speed != 0)
		mAngle = RADIANS(RotateTowards(DEGREES(mAngle), angle, skidrecover * timedelta));
	else
		mAngle = RADIANS(angle);

	// Write back angle, in degrees, if allowed
	float oldAngle = pLink->info.angle;
	pLink->info.angle = angle;
	pRuntime->UpdateBoundingBox(pLink);

	if (pRuntime->QueryCollision(pLink, pSolid)) {
		pLink->info.angle = oldAngle;
		pRuntime->UpdateBoundingBox(pLink);
	}

	// Update Bounding Box if we moved the object
	if (speed != 0 || left || right)
		pRuntime->UpdateBoundingBox(pLink);

	return !activated;
}

BOOL ExtObject::OnFrame2()
{
	return 1;
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

void ExtObject::DebuggerUpdateDisplay(ExpStore*& pPrivateVars)
{
	pRuntime->AddDebuggerItem("Speed", speed, false);
	pRuntime->AddDebuggerItem("Max speed", maxspeed, false);
	pRuntime->AddDebuggerItem("Acceleration", acc, false);
	pRuntime->AddDebuggerItem("Deceleration", dec, false);
	pRuntime->AddDebuggerItem("Rotate speed", av, false);
	pRuntime->AddDebuggerItem("Skid recover", skidrecover, false);
}

void ExtObject::OnDebuggerValueChanged(const char* name, const char* value)
{
	CString label = name;
	CString text = value;
	__int64 iValue = _atoi64(value);
	double fValue = atof(value);

	if (label == "Speed")
		speed = fValue;
	else if (label == "Max speed")
		maxspeed = fValue;
	else if (label == "Acceleration")
		acc = fValue;
	else if (label == "Deceleration")
		dec = fValue;
	else if (label == "Max Speed")
		maxspeed = fValue;
	else if (label == "Rotate speed")
		av = fValue;
	else if (label == "Skid recover")
		skidrecover = fValue;
}

void ExtObject::Serialize(bin& ar)
{
	if (ar.loading) {
		ar >> steer >> speed >> mAngle >> av >> acc >> dec >> maxspeed >> skidrecover
			>> ignoringInput >> activated;

		if (activated)
			pRuntime->CallOnFrame(this);
	}
	else {
		ar << steer << speed << mAngle << av << acc << dec << maxspeed << skidrecover
			<< ignoringInput << activated;
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
	switch (id) {

		// Write speed to param
	case CF_GETSPEED:
		*(float*)param = speed;
		break;
	}

	return 0;
}

// Return the Default Value.  This is the value to be returned if your object name
// is used without an expression name, eg. in "Counter + 1".
// Parameters can be passed, eg. MyArray(3,7).
long ExtObject::ReturnDefaultValue(LPVAL theParams, ExpReturn& ret)
{
	return ret = (0);
}

// Called for undefined expression names, if your extension allows it.
// Eg: MyObject.UndefinedExpression(3,4,5) calls this routine with
// "UndefinedExpression" as the expression name.
long ExtObject::ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret)
{
	return ret = (0);
}

#else //ifdef RUN_ONLY

CRunObject* WINAPI RTCreateObject(bin& ar, VRuntime* pRuntime) 
{
	return NULL;
}

void WINAPI RTDestroyObject(ExtObject* object)
{
	//
}

#endif