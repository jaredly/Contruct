// Edittime.cpp - Edittime implementation of object
//

// Include StdAfx and Properties.h
#include "StdAfx.h"

#ifndef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// Dependencies
//////////////////////////////////////////////////////////////////////////////////

// Add any DLLs your plugin depends on here.
void WINAPI ETAddDependencies(MicroDependency& dependencies)
{
	// Any DLL files added here are retrieved from the Plugins\Runtime directory, and compiled in to exported EXEs.
	// When the EXE runs, the DLL is extracted to the current directory before loading your plugin, so calls such
	// as LoadLibrary("example.dll") will succeed at runtime.  This greatly simplifies redistribution for end users.

	// Eg.
	//dependencies.Add("example.dll");
}

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

	PROPERTY_VALUE(data.numChannels, "Channels", "Number of channels of audio (maximum simultaneous sounds)");
	PROPERTY_FLOAT(data.maxFrequencyRatio, "Max frequency ratio", "Maximum ratio for changing channel frequency: lower values conserve memory");
	PROPERTY_FLOAT(data.masterGain, "Master volume", "Default master volume: -3 dB or lower is recommended to help prevent clipping.");
	PROPERTY_COMBO(cacheMode_int, "Cache", "Choose which sounds to save in memory.", "Off|Sounds under 3mb|All but OGG|All");

	PROPERTY_NEW_CATEGORY("Positioned sounds", "Properties relating to sounds played at a position");

	PROPERTY_FLOAT(data.maxVol, "Max volume", "Loudest a positioned sound will play.");
	PROPERTY_FLOAT(data.minVol, "Min volume", "Quietest a positioned sound will play.");
	PROPERTY_FLOAT(data.minDist, "Min distance", "Distance from listener at which sound plays at max volume.");
	PROPERTY_PERCENT(data.rolloff, "Rolloff factor", "Rate at which sounds reduce in level with distance.");
	PROPERTY_FLOAT(data.maxPan, "Max pan", "Most panned left/right a positioned sound will become.");
	PROPERTY_FLOAT(data.panDist, "Pan distance", "Horizontal distance in pixels from listener at which max pan is achieved.");

	PROPERTY_NEW_CATEGORY("Limiter", "Properties for the limiter, which helps prevent distortion.");

	PROPERTY_BOOL(data.limiter_enable, "Limiter enabled", "Add a limiter, which helps prevent distortion when many sounds are playing at once.");
	PROPERTY_VALUE(data.limiter_release, "Release", "Limiter release time, 1-20");
	PROPERTY_VALUE(data.limiter_threshold, "Threshold", "Limiter threshold, 1-1800");
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

	cacheMode_int = 1;	// sounds under 3mb
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
	Clamp(data.limiter_release, 1, 20);
	Clamp(data.limiter_threshold, 1, 1800);
}

// When the object is inserted to the layout editor by the user.
void EditExt::OnPut()
{
	// Default object size
	pInfo->objectWidth = 32;
	pInfo->objectHeight = 32;
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