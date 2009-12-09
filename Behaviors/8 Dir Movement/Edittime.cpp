// Edittime.cpp - Edittime implementation of object
//

// Include StdAfx and Properties.h
#include "StdAfx.h"

#ifndef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// Property table
//////////////////////////////////////////////////////////////////////////////////

// The property table houses settings for your object that you wish to 
// be configurable before runtime
//
// For example, a button object would have settings relating to the text 
// that will be displayed, and what type of button it is
//
// There are several types of properties currently available to use:
//
// PROPERTY_EDIT  , a text input field
// PROPERTY_STATIC, a static display of text
//
// Here is a brief example of them all:
//
// PROPERTY_EDIT  (myVariable,   "Label", "Description); // myVariable is a string
// PROPERTY_STATIC(myVariable,   "Label", "Description); // myVariable is a string
// PROPERTY_VALUE (myVariable,   "Label", "Description); // myVariable is an int
// PROPERTY_BUTTON("ButtonText", "Label", "Description", Function()); // Function is a void with no parameters

BEGIN_PROPERTY_TABLE();
	PROPERTY_PLAYER	(player);
	PROPERTY_FLOAT  (maxspeed,	"Max Speed", "Maximum speed the object can travel at (pixels per second)");
	PROPERTY_FLOAT  (acc,		"Acceleration", "Rate of acceleration (pixels per second per second)");
	PROPERTY_FLOAT  (dec,		"Deceleration",	"Rate of deceleration (pixels per second per second)");
	PROPERTY_COMBO	(rotateMode, "Rotation", "Set the object's angle during movement.", "None|8-direction|360 degree");
	PROPERTY_COMBO	(directionsMode, "Directions", "The number of directions of motion available.", "2 (up/down)|2 (left/right)|4 directions|8 directions");
END_PROPERTY_TABLE  ();

/////////////////////////////////////////////////////////////////////////////////
// Edittime functions
//////////////////////////////////////////////////////////////////////////////////

// Constructor: when your object is created during edittime or application loaded.
// Initialise values here.
EditExt::EditExt(VEditTime* pVEditTime, editInfo* pEInfo)
{
	// Save the edittime and einfo pointers
	pEditTime = pVEditTime;
	pInfo = pEInfo;

	pInfo->objectWidth  = 32;
	pInfo->objectHeight = 32;

	// Defaults for 8-dir movement
	dec = 500.0f;
	acc = 600.0f;
	maxspeed = 200.0f;
	rotateMode = 2;	// 360 degree
	directionsMode = DIRMODE_8;	// 8 direction
	player = 0;
}

// Destructor: when your object is deleted or the application closed:
// close any allocated resources here
EditExt::~EditExt()
{
	//
}

// Draw your object in the frame editor.
// Note the co-ordinates are automatically adjusted; draw at pInfo->objectX/Y
void EditExt::Draw()
{
	//
}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and anything else DirectX volatile here.
void EditExt::Initialize()
{
	// 
}

// When the object is inserted to the frame editor by the user.
void EditExt::OnPut()
{
	//
}

// When the object is removed from the frame editor by the user.
void EditExt::OnRemoved()
{
}

// Properties updated
void EditExt::OnPropertiesUpdate()
{

}

void EditExt::GetAnimationHandle(int& iHandle)
{
	iHandle = -1;
}

// On object resized
BOOL EditExt::OnSizeObject()
{
	// TRUE = allow sizing to occur, FALSE = disallow
	return TRUE;
}

void EditExt::Serialize(bin& ar)
{
	int Version = 2;
	SerializeVersion(ar, Version);

	if (ar.loading)
	{
		// Code to load your object here
		ar >> maxspeed >> acc >> dec >> rotateMode >> directionsMode;

		if (Version >= 2)
			ar >> player;
	}
	else
	{
		// Code to save your object here
		ar << maxspeed << acc << dec << rotateMode << directionsMode;
		ar << player;
	}
}

#else //ifndef RUN_ONLY

BOOL WINAPI ETCreateObject(editInfo* editObject) {return FALSE;}
BOOL WINAPI ETInitializeObject(editInfo* editObject, VEditTime* pVEditTime) {return FALSE;}
BOOL WINAPI ETDestroyObject(editInfo* editObject) {return FALSE;}
int WINAPI ETDrawObject(editInfo *editObject) {return 0;}
void ETOnPropertyChanged(editInfo* editObject) {}
long LinkPropTable() {return 0;}
void WINAPI ETSerialize(editInfo* editObject, CArchive& ar) {}
void WINAPI ETPutObject(editInfo* editObject) {}
void WINAPI ETRemoveObject(editInfo* editObject) {}
void WINAPI GetInfo(LPOINFO objectInfo) {}
int WINAPI ETGetMenuIcons(editInfo* editObject, int ACEMenu) {return 0;}
int WINAPI ETShowACEMenu(editInfo* editObject, TABLE ACEType)  {return 0;}
BOOL WINAPI ETSizeObject(editInfo* editObject)  {return FALSE;}

#endif // RUN_ONLY