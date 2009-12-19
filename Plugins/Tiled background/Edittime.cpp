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
	PROPERTY_BUTTON ("Edit", "Image", "Click to edit the object's image", BtnEditImage());
//	PROPERTY_PERCENT(opacity, "Opacity", "The semitransparency of the image, from 0 (transparent) to 100 (opaque)");
//	PROPERTY_COLOR(filter, "Filter", "The colour filter for the image.");
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
}

// Destructor: when your object is deleted or the application closed:
// close any allocated resources here
EditExt::~EditExt()
{
	//
}

void EditExt::BtnEditImage()
{
	pEditTime->EditImage(imgTexture);

	//if (iTexture != -1)
	//	pEditTime->DeleteTexture(iTexture);

	iTexture = pEditTime->AddImageAsTexture(imgTexture);

	pEditTime->GenerateIcon(pInfo->ObjectIdentifier, false, imgTexture);
	pEditTime->GenerateIcon(pInfo->ObjectIdentifier, true, imgTexture);
}

bool IsPowerOfTwo(int value)
{
	return (value & -value) == value;
}

// Draw your object in the frame editor.
// Note the co-ordinates are automatically adjusted; draw at pInfo->objectX/Y
void EditExt::Draw()
{
	SIZE size = pEditTime->GetImageSize(imgTexture);

	size.cx *= pEditTime->GetZoom();
	size.cy *= pEditTime->GetZoom();

	SIZE seam_size = size;

	// Runtime does this for anti-seaming, so reproduce in layout editor.
	if (!IsPowerOfTwo(size.cx) || !IsPowerOfTwo(size.cy)) {
		seam_size.cx--;
		seam_size.cy--;
	}

	int cols = pInfo->objectWidth / seam_size.cx;
	int rows = pInfo->objectHeight / seam_size.cy;

	int gutterx = pInfo->objectX + (cols * seam_size.cx);
	int guttery = pInfo->objectY + (rows * seam_size.cy);

	int gutterw = pInfo->objectWidth - (cols * seam_size.cx);
	int gutterh = pInfo->objectHeight - (rows * seam_size.cy);

	RECTF uv;
	uv.left = 0.0f;
	uv.top = 0.0f;
	uv.right = float(seam_size.cx) / float(size.cx);
	uv.bottom = float(seam_size.cy) / float(size.cy);

	RECTF vertuv;
	vertuv.left = 0.0f;
	vertuv.top = 0.0f;
	vertuv.right = uv.right;
	vertuv.bottom = float(gutterh) / float(size.cy);

	RECTF horizuv;
	horizuv.left = 0.0f;
	horizuv.top = 0.0f;
	horizuv.right = float(gutterw) / float(size.cx);
	horizuv.bottom = uv.bottom;

	D3DCOLOR dc = pInfo->filter; //D3D_OPACITYCOLORREF((int)(opacity * 255.0), filter);

	iTexture = pEditTime->AddImageAsTexture(imgTexture); 	
	pEditTime->SetTexture(iTexture);

	int x,y;

	for (x = 0; x < cols; x++) {
		for (y = 0; y < rows; y++)
			pEditTime->Blitrc(pInfo->objectX + x * seam_size.cx, pInfo->objectY + y * seam_size.cy, seam_size.cx, seam_size.cy, 0.0f, dc);
	}

	// Draw the vertical gutter
	if (gutterw > 0) {
		for (y = 0; y < rows; y++)
			pEditTime->Blitrc(gutterx, pInfo->objectY + y*seam_size.cy, gutterw, seam_size.cy, 0.0f, dc, &horizuv);
	}

	// Draw the horizontal gutter
	if (gutterh > 0) {
		for (x = 0; x < cols; x++)
			pEditTime->Blitrc(pInfo->objectX + x*seam_size.cx, guttery, seam_size.cx, gutterh, 0.0f, dc, &vertuv);
	}

	// Draw the bottom right tile
	if (gutterw > 0 && gutterh > 0) {
		horizuv.bottom = vertuv.bottom;
		pEditTime->Blitrc(gutterx, guttery, gutterw, gutterh, 0.0f, dc, &horizuv);
	}
	
}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and anything else DirectX volatile here.
void EditExt::Initialize()
{
}

// When the object is inserted to the frame editor by the user.
void EditExt::OnPut()
{
	imgTexture = pEditTime->AddImage(128,128);
	pEditTime->EditImage(imgTexture);
	iTexture  = pEditTime->AddImageAsTexture(imgTexture);

	pInfo->objectWidth = 100;
	pInfo->objectHeight = 100;

	pEditTime->GenerateIcon(pInfo->ObjectIdentifier, false, imgTexture);
	pEditTime->GenerateIcon(pInfo->ObjectIdentifier, true, imgTexture);

//	opacity = 1.0;
//	filter = RGB(255,255,255);
}

// When the object is removed from the frame editor by the user.
void EditExt::OnRemoved()
{
	//
}

// Get any icons needed to show this ACEs menu
int EditExt::GetMenuIcons(int ACEMenu)
{
	// Todo: return the parameter number of a bitmap carrying an imagelist
	//       to show in the menu indicated by ACEMenu

	return -1; // -1 indicates no icons will be used
}

// Properties updated
void EditExt::OnPropertiesUpdate()
{
	//
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
	int Version = 2;
	SerializeVersion(ar, Version);

	if (ar.loading)
	{
		pEditTime->SerializeImage(imgTexture, ar);

		if(Version < 2)
		{
			double opacity;	COLORREF filter;
			ar >> opacity >> filter;
			pInfo->filter = D3D_OPACITYCOLORREF((int)(opacity * 255.0), filter);
		}
		
	}
	else
	{
		pEditTime->SerializeImage(imgTexture, ar);
//		ar << opacity << filter;
	}
}

BOOL EditExt::OnNotify(int notify)
{
	if(notify == 1)
	{
		BtnEditImage();
	}
	/*
	if(notify == 2)
	{
		BtnEditAnimations();
	}
	*/
	return TRUE;
}

void WINAPI ETOnDoubleClick(editInfo* editObject)
{
	editObject->objectPtr->BtnEditImage();
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
void WINAPI ETOnDoubleClick(editInfo* editObject) {}

#endif // RUN_ONLY

BOOL WINAPI ETOnNotify(editInfo* editObject, int notification)
{
#ifdef RUN_ONLY
	return 0;
#else
	return ((EditExt*)editObject->objectPtr)->OnNotify(notification);
#endif
}