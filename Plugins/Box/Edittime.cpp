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

	PROPERTY_COLOR  (c1,   "Colour 1"  , "Top and left colour.");
	PROPERTY_COMBO  (sysc1, "Colour 1 (system)", "Alternative system colour for this item.", "(use custom)|Scrollbar|Background|Active caption|Inactive caption|Menu|Window|Window frame|Menu text|Window text|Caption text|Active border|Inactive border|App workspace|Highlight|Highlight text|3D Face|3D Shadow|Gray text|Button text|Inactive caption text|Button highlight");
	PROPERTY_PERCENT(c1_opacityf, "Colour 1 Opacity", "Semitransparency of the top and left colour.");
	PROPERTY_COLOR  (c2,   "Colour 2"  , "Bottom and right colour.");
	PROPERTY_COMBO  (sysc2, "Colour 2 (system)", "Alternative system colour for this item.", "(use custom)|Scrollbar|Background|Active caption|Inactive caption|Menu|Window|Window frame|Menu text|Window text|Caption text|Active border|Inactive border|App workspace|Highlight|Highlight text|3D Face|3D Shadow|Gray text|Button text|Inactive caption text|Button highlight");
	PROPERTY_PERCENT(c2_opacityf, "Colour 2 Opacity", "Semitransparency of the bottom and right colour.");
	PROPERTY_BOOL	(transparent, "Transparent fill", "Transparent inside the box.");
	PROPERTY_COLOR  (fill,   "Fill"  , "Fill colour.");
	PROPERTY_COMBO  (sysfill, "Fill (system)", "Alternative system colour for this item.", "(use custom)|Scrollbar|Background|Active caption|Inactive caption|Menu|Window|Window frame|Menu text|Window text|Caption text|Active border|Inactive border|App workspace|Highlight|Highlight text|3D Face|3D Shadow|Gray text|Button text|Inactive caption text|Button highlight");
	PROPERTY_PERCENT(fill_opacityf, "Fill Opacity", "Semitransparency of the fill colour.");
	PROPERTY_BOOL	(smoothLines, "Antialiased", "Draw box with smooth lines");
	PROPERTY_COMBO	(hotspot_pos, "Hotspot", "Choose where the hotspot lies relative to the box", "Top-left|Top|Top-right|Left|Center|Right|Bottom-left|Bottom|Bottom-right");
	
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

	pInfo->originX = 0.5;
	pInfo->originY = 0.5;

	smoothLines = false;
	hotspot_pos = 4;	// center
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
	Clamp<float>(c1_opacityf, 0.0f, 1.0f);
	Clamp<float>(c2_opacityf, 0.0f, 1.0f);
	Clamp<float>(fill_opacityf, 0.0f, 1.0f);

	if (sysc1 > 0)
		c1 = GetSysColor(sysc1 - 1);
	if (sysc2 > 0)
		c2 = GetSysColor(sysc2 - 1);
	if (sysfill > 0)
		fill = GetSysColor(sysfill - 1);
}

// When the object is inserted to the layout editor by the user.
void EditExt::OnPut()
{
	c1 = RGB(0,255,0);
	c2 = RGB(0,255,0);
	fill = RGB(0,0,0);
	transparent = true;
	
	sysc1 = 0;
	sysc2 = 0;
	sysfill = 0;

	c1_opacityf = 1.0f;
	c2_opacityf = 1.0f;
	fill_opacityf = 1.0f;

	pInfo->objectWidth = 50;
	pInfo->objectHeight = 50;
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