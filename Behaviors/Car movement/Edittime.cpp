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
	PROPERTY_PLAYER(player);
	PROPERTY_FLOAT(maxspeed, "Max speed", "Maximum speed, in pixels per second.");
	PROPERTY_FLOAT(acc, "Acceleration", "Acceleration, in pixels per second per second.");
	PROPERTY_FLOAT(dec, "Deceleration", "Deceleration, in pixels per second per second.");
	PROPERTY_FLOAT(av, "Rotate speed", "Turn speed, in degrees per second.");
	PROPERTY_FLOAT(skidrecover, "Skid recover rate", "Rate at which skids recover, degrees per second.  Lower causes more skidding, higher gives tighter turns.");
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

	dec = 200.0f;
	acc = 200.0f;
	maxspeed = 350.0f;
	av = 225.0f;
	skidrecover = 185.0f;
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
	//
}

// Properties updated
void EditExt::OnPropertiesUpdate()
{
	//
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
		ar >> acc >> dec >> maxspeed >> av >> skidrecover;
		if (Version >= 2)
			ar >> player;
	}
	else
	{
		ar << acc << dec << maxspeed << av << skidrecover;
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