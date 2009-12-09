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

	PROPERTY_COLOR  (vertex[0],   "Filter top-left"  , "Top left filter.");
	PROPERTY_COLOR  (vertex[1],   "Filter top-right"  , "Top right filter.");
	PROPERTY_COLOR  (vertex[2],   "Filter bottom-left"  , "Bottom left filter.");
	PROPERTY_COLOR  (vertex[3],   "Filter bottom-right"  , "Bottom right filter.");

	PROPERTY_PERCENT(vertex_opacity[0], "Opacity top-left", "Top left opacity");
	PROPERTY_PERCENT(vertex_opacity[1], "Opacity top-right", "Top right opacity");
	PROPERTY_PERCENT(vertex_opacity[2], "Opacity bottom-left", "Bottom left opacity");
	PROPERTY_PERCENT(vertex_opacity[3], "Opacity bottom-right", "Bottom right opacity");

	PROPERTY_VALUE (image_left_margin, "Image left-margin", "The number of pixels the margin uses in the source image.");
	PROPERTY_VALUE (image_right_margin, "Image right-margin", "The number of pixels the margin uses in the source image.");
	PROPERTY_VALUE (image_top_margin, "Image top-margin", "The number of pixels the margin uses in the source image.");
	PROPERTY_VALUE (image_bottom_margin, "Image bottom-margin", "The number of pixels the margin uses in the source image.");

	PROPERTY_BUTTON ("Edit", "Image", "Click to edit the object's image", BtnEditImage());

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
/*	Clamp<float>(c1_opacityf, 0.0f, 1.0f);
	Clamp<float>(c2_opacityf, 0.0f, 1.0f);
	Clamp<float>(fill_opacityf, 0.0f, 1.0f);

	if (sysc1 > 0)
		c1 = GetSysColor(sysc1 - 1);
	if (sysc2 > 0)
		c2 = GetSysColor(sysc2 - 1);
	if (sysfill > 0)
		fill = GetSysColor(sysfill - 1);*/
}

// When the object is inserted to the layout editor by the user.
void EditExt::OnPut()
{
	pInfo->objectWidth = 50;
	pInfo->objectHeight = 50;


	for(int i = 0; i < 4; i++)
	{
		vertex[i] = 0xffffff;
		vertex_opacity[i] = 1.0;
	}

	USEMYRESOURCES()
	imgTexture = AddImageFromResource2(MAKEINTRESOURCE(IDR_DEFAULT));

	image_left_margin = 6;
	image_right_margin = 6;
	image_top_margin = 6;
	image_bottom_margin = 6;
}

void EditExt::BtnEditImage()
{
	pEditTime->EditImage(imgTexture);

	pEditTime->GenerateIcon(pInfo->ObjectIdentifier, false, imgTexture);
	pEditTime->GenerateIcon(pInfo->ObjectIdentifier, true, imgTexture);
}

BOOL EditExt::OnNotify(int notify)
{
	if(notify == 1)
	{
		BtnEditImage();
	}
	return TRUE;
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

BOOL WINAPI ETOnNotify(editInfo* editObject, int notification)
{
#ifdef RUN_ONLY
	return 0;
#else
	return ((EditExt*)editObject->objectPtr)->OnNotify(notification);
#endif
}