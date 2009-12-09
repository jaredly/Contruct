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

PROPERTY_COMBO	(accuracy,	"Accuracy", "The tradeoff between speed and the smallest gap that can be seen through.",
							"100px (fastest)|50px|30px|10px|8px|4px|2px|1px (slowest)");
PROPERTY_COMBO	(obstacles,	"Obstacles", "Choose which objects cannot be seen through.", "'Solid' objects|Custom");
PROPERTY_FLOAT (range, "Range", "The furtherest distance an object can be in pixels to be seen");
PROPERTY_FLOAT (angleRange, "Angle Range", "The angle range (based on the object's current angle)");

END_PROPERTY_TABLE  ();


/////////////////////////////////////////////////////////////////////////////////
// Edittime functions
//////////////////////////////////////////////////////////////////////////////////

// Constructor
EditExt::EditExt(VEditTime* pVEditTime, editInfo* pEInfo)
{
	// Store the edittime and einfo pointers
	pEditTime = pVEditTime;
	pInfo = pEInfo;

	pInfo->objectWidth  = 32;
	pInfo->objectHeight = 32;

	accuracy = 3;		// 10px
	obstacles = 0;		// Use 'solid'
	range = 500;
	angleRange = 360;

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
	// Default object size
	pInfo->objectWidth = 64;
	pInfo->objectHeight = 64;
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