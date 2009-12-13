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
// There are several types of properties currently available to use:-
//
// PROPERTY_EDIT  , a text input field
// PROPERTY_STATIC, a static display of text
//
// Here is a brief example of them all:
//
// PROPERTY_EDIT  (myVariable, "Label", "Description);
// PROPERTY_STATIC(myVariable, "Label", "Description);

BEGIN_PROPERTY_TABLE();
//	PROPERTY_COLOR  (filter,   "Colour filter"  , "Tint the object a different color, white gives original colors");
//	PROPERTY_PERCENT(opacityf, "Opacity", "Semitransparency of the object.");
	PROPERTY_FLOAT  (angle , "Angle" , "Orientation of the object, in degrees.");
	PROPERTY_BUTTON ("Edit", "Image", "Click to edit the object's image", BtnEditImage());
	PROPERTY_COMBO	(grabFrame, "Grab Layout", "Enable ability to process effects on the display underneath the canvas.", "Off|After drawing|Before drawing");
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

	grabFrame = 0;

}

void EditExt::BtnEditImage()
{
	pEditTime->EditImage(imgTexture);
	iTexture = pEditTime->AddImageAsTexture(imgTexture);
}

void EditExt::BtnEditAnimations()
{
	pEditTime->EditAnimation(m_iAnimationRoot);
}

// Destructor: when your object is deleted or the application closed:
// close any allocated resources here
EditExt::~EditExt()
{

}

// Draw your object in the frame editor.
// Note the co-ordinates are automatically adjusted; draw at pInfo->objectX/Y
void EditExt::Draw()
{
	// Device lost
	if (!pEditTime->TextureExists(iTexture)) {
		iTexture = pEditTime->AddImageAsTexture(imgTexture);

		// Device still lost
		if (iTexture == -2)
			return;
	}

//	D3DCOLOR f = D3D_CONVERTCOLORREF(filter);
//	f = D3D_ARGB(opacity, D3D_R(f), D3D_G(f), D3D_B(f));

	// Simply blit the object icon texture to the frame
	pEditTime->SetTexture(iTexture);

	pEditTime->Blitrc(pInfo->objectX, pInfo->objectY, pInfo->objectWidth, pInfo->objectHeight, (float)angle, pInfo->filter);
}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and anything else DirectX volatile here.
void EditExt::Initialize()
{
	// Make a texture from the object's image
	iTexture = pEditTime->AddImageAsTexture(imgTexture);
}

void EditExt::OnPropertiesUpdate()
{
}

// When the object is inserted to the frame editor by the user.
void EditExt::OnPut()
{
	imgTexture = pEditTime->AddImage(200,200);
	pEditTime->EditImage(imgTexture);
	iTexture  = pEditTime->AddImageAsTexture(imgTexture);

	//m_iAnimationRoot = pEditTime->AddAnimation(32, 32, 1, "LOL", -1);

	angle = 0.0f;
	grabFrame = 0;

	SIZE s;
	pEditTime->GetTextureSize(iTexture, &s);

	pInfo->objectWidth = s.cx;
	pInfo->objectHeight = s.cy;
}


// When the object is removed from the frame editor by the user.
void EditExt::OnRemoved()
{

}

// Get any icons needed to show this ACEs menu
int EditExt::GetMenuIcons(int ACEMenu)
{
	// Todo: return the parameter number of a bitmap carrying an imagelist
	//       to show in the menu indicated by ACEMenu

	return -1; // -1 indicates no icons will be used
}


int EditExt::OnShowACEMenu(TABLE ACEType)
{
	// You can return the resource number indicating the menu to show for the
	//       ACE type indicated in ACEType
	// This overrides the default menu

	return -1;	// Return -1 for standard menu to be loaded
}

// On object resized
BOOL EditExt::OnSizeObject()
{
	// TRUE = allow sizing to occur, FALSE = disallow
	return TRUE;
}

void EditExt::Serialize(bin& ar)
{
	int Version = 3;
	SerializeVersion(ar, Version);

	if (ar.loading) {

		// Serialize D3DCOLOR, but process with COLORREF and OPACITY values at edittime.
		D3DCOLOR f;

		ar >> angle;

		// Old versions serialize a filter; this is now moved to common
		if (Version <= 2) {
			ar >> f;
			pInfo->filter = f;
		}
		pEditTime->SerializeImage(imgTexture, ar);

		// V1: was a bool, now an int
		if (Version == 1) {
			bool oldSetting;
			ar >> oldSetting;
			grabFrame = oldSetting ? 1 : 0;
		}
		else
			ar >> grabFrame;
	}
	else {

		ar << angle;
		//ar << f; // Filter removed V3; using common properties instead
		pEditTime->SerializeImage(imgTexture, ar);
		ar << grabFrame;
	}
}

BOOL EditExt::OnNotify(int notify)
{
	// Double clicked
	if(notify == 1)
	{
		BtnEditImage();
	}

	return TRUE;
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

BOOL WINAPI ETOnNotify(editInfo* editObject, int notification)
{
#ifdef RUN_ONLY
	return 0;
#else
	return ((EditExt*)editObject->objectPtr)->OnNotify(notification);
#endif
}