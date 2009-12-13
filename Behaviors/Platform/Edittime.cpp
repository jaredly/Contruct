// Edittime.cpp - Edittime implementation of object
//

// Include StdAfx and Properties.h
#include "StdAfx.h"

#ifndef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// Property table
//////////////////////////////////////////////////////////////////////////////////

// The property table defines the list of properties that appear in the property
// grid when your object is selected in the layout editor.
//
// See the SDK docs for full documentation, but to get you started:-
//
// PROPERTY_EDIT  (myVariable, "Label", "Description");		// A text field property.
// PROPERTY_VALUE (myVariable, "Label 2", "Description");	// An integer property.
// PROPERTY_FLOAT (myVariable, "Label 3", "Description");	// A double/float property
//
// The labels must be unique!

BEGIN_PROPERTY_TABLE();

	PROPERTY_PLAYER (player);

	PROPERTY_FLOAT	(floor_acc, "Floor Acceleration", "Horizontal acceleration when on the floor (pixels per second per second).");
	PROPERTY_FLOAT	(floor_dec, "Floor Deceleration", "Horizontal deceleration when on the floor (pixels per second per second).");
	PROPERTY_FLOAT	(max_floor_speed, "Max floor speed", "Maximum horizontal speed the object can travel at on the floor (pixels per second).");
	
	PROPERTY_FLOAT	(air_acc, "Air Acceleration", "Horizontal acceleration when in the air (pixels per second per second).");
	PROPERTY_FLOAT	(air_dec, "Air Deceleration", "Horizontal deceleration when in the air (pixels per second per second).");
	PROPERTY_FLOAT	(max_air_speed, "Max air speed", "Maximum horizontal speed the object can travel at in the air (pixels per second).");
	
	PROPERTY_FLOAT	(max_fall, "Max fall", "Maximum vertical speed the object can fall at (pixels per second).");
	PROPERTY_FLOAT	(jump_strength, "Jump strength", "Speed at which jumps start, in vertical pixels per second.");
	PROPERTY_FLOAT	(jump_sustain,	"Jump sustain time",	"Max time in milliseconds a jump can sustain, to allow variable height jumps.");

	PROPERTY_FLOAT	(grav_fall,		"Fall Gravity",		"Force of gravity (downwards acceleration pixels per second per second) while the player falls");
	PROPERTY_FLOAT	(grav_jump,		"Jump Gravity",		"Force of gravity (downwards acceleration pixels per second per second) while the player is jumping (but isn't sustaining the jump)");
	PROPERTY_FLOAT	(grav_jump_sustain,		"Jump Sustain Gravity",		"Force of gravity (downwards acceleration pixels per second per second). while the player is jumping and sustaining the jump");

	PROPERTY_COMBO	(grav_dir,		"Gravity Direction", "The direction gravity pulls you.", "Right|Down|Left|Up");
	PROPERTY_BOOL   (inv,  "Inverse Control", "Inverse so left becomes right and vise versa.");
	PROPERTY_BOOL   (bunnyhop, "Allow bunny hop", "Allow the jump key to be held down and the character to bounce on the spot");
	PROPERTY_BOOL   (CanPressDown, "Allow down button", "Allow the user to fall through platforms by pressing the down button");
	PROPERTY_BOOL   (autochangeangle, "Auto Rotate", "Automatically sets the angle of the sprite to 180 when moving left. You might want to enable 'Auto mirror' if you're using a Sprite");

END_PROPERTY_TABLE  ();


/////////////////////////////////////////////////////////////////////////////////
// Edittime functions
//////////////////////////////////////////////////////////////////////////////////

// Constructor
EditExt::EditExt(VEditTime* pVEditTime, editInfo* pEInfo)
{
	// Save the edittime and einfo pointers
	pEditTime = pVEditTime;
	pInfo = pEInfo;

	pInfo->objectWidth  = 32;
	pInfo->objectHeight = 32;

	floor_acc = air_acc = 1500;
	floor_dec = air_dec = 1500;
	max_floor_speed = max_air_speed = 330;
	max_fall = 1000;
	jump_strength = 800;
	jump_sustain = 0; 
	grav_jump_sustain = 0;
	grav_dir = DIR_DOWN;
	grav_fall = grav_jump = 1500;
	bunnyhop = true;
	inv = false;
	CanPressDown = true;
	player = 0;
	

	autochangeangle = false;
}

// Destructor: when your object is deleted or the application closed:
// close any allocated resources here
EditExt::~EditExt()
{

}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and any other DirectX volatile data here.
void EditExt::Initialize()
{

}

// Called when any property has been changed.
void EditExt::OnPropertiesUpdate()
{
}

// When the object is inserted to the layout editor by the user.
void EditExt::OnPut()
{

}


// When the object is removed from the layout editor by the user.
void EditExt::OnRemoved()
{

}

// On object resized
BOOL EditExt::OnSizeObject()
{
	return TRUE;
}

void EditExt::GetAnimationHandle(int& handle)
{
	
}

#else //ifndef RUN_ONLY

BOOL WINAPI ETCreateObject(editInfo* editObject) {return FALSE;}
BOOL WINAPI ETInitializeObject(editInfo* editObject, VEditTime* pVEditTime) {return FALSE;}
BOOL WINAPI ETDestroyObject(editInfo* editObject) {return FALSE;}
int WINAPI ETDrawObject(editInfo *editObject) {return 0;}
void ETOnPropertyChanged(editInfo* editObject) {}
long LinkPropTable() {return 0;}
void WINAPI ETSerialize(editInfo* editObject, bin& ar) {}
void WINAPI ETPutObject(editInfo* editObject) {}
void WINAPI ETRemoveObject(editInfo* editObject) {}
void WINAPI GetInfo(LPOINFO objectInfo) {}
int WINAPI ETGetMenuIcons(editInfo* editObject, int ACEMenu) {return 0;}
int WINAPI ETShowACEMenu(editInfo* editObject, TABLE ACEType)  {return 0;}
BOOL WINAPI ETSizeObject(editInfo* editObject)  {return FALSE;}

#endif // RUN_ONLY