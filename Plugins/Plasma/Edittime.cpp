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
	PROPERTY_BUTTON ("Edit", "Cold", "Click to edit the object's image", BtnEditImage(0));
	PROPERTY_BUTTON ("Edit", "Source", "Click to edit the object's image", BtnEditImage(1));

	PROPERTY_FLOAT  (xspeed, "Plasma X speed", "Horizontal speed of the plasma");
	PROPERTY_FLOAT  (yspeed, "Plasma Y speed", "Vertical speed of the plasma");

	PROPERTY_NEW_CATEGORY("Subtract Map", "Patterned texture which is subtracted from the plasma each frame");

	PROPERTY_COLOR  (cold_filter, "Filter", "A filter applied to the subtract map");
	PROPERTY_PERCENT(cold_opacityf, "Opacity", "Opacity of the subtract map");

	PROPERTY_FLOAT  (coldx, "X offset", "X offset of the patterned texture");
	PROPERTY_FLOAT  (coldy, "Y offset", "Y offset of the patterned texture");
	PROPERTY_FLOAT  (coldxspeed, "X speed", "X speed in pixels per second");
	PROPERTY_FLOAT  (coldyspeed, "Y speed", "Y speed in pixels per second");
	PROPERTY_FLOAT  (coldscale, "Scale", "Scale of the pattern texture");
	PROPERTY_FLOAT  (coldangle, "Display Angle", "Angle of the pattern texture");

	PROPERTY_NEW_CATEGORY("Display Effects", "Settings to how the flame renders on the screen");

	for(int i = 0; i < 6; i++){
		CString number;
		number.Format("%d", i+1);
		PROPERTY_COLOR  (render_filter[i],   "(" + number + ") Filter", "A filter applied to the render");
		PROPERTY_PERCENT(render_strength[i], "(" + number + ") Strength", "Strength of the render");
		PROPERTY_PERCENT(render_cutoff[i],   "(" + number + ") Cut-off", "Cut-off from the alpha values");
	}


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

	render_filter[0] = 0xffffff;
	render_strength[0] = 1;
	render_cutoff[0] = 0;

	for(int i = 1; i < 6; i++)
	{
		render_filter[i] = 0;
		render_strength[i] = 0;
		render_cutoff[i] = 0;
	}
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
	D3DCOLOR f = D3D_CONVERTCOLORREF(pInfo->filter);
	///f = D3D_ARGB((int)(opacityf*255), D3D_R(f), D3D_G(f), D3D_B(f));

	// Draw box
	pEditTime->SetTexture(-1);

	RECT rc;
	rc.left = pInfo->objectX;
	rc.top = pInfo->objectY;
	rc.right = rc.left + pInfo->objectWidth;
	rc.bottom = rc.top + pInfo->objectHeight;

	pEditTime->Box(rc, f);
}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and anything else DirectX volatile here.
void EditExt::Initialize()
{
}

void EditExt::OnPropertiesUpdate()
{
	Clamp<float>(cold_opacityf, 0.0f, 1.0f);
}

// When the object is inserted to the frame editor by the user.
void EditExt::OnPut()
{
	coldx = 0;
	coldy = 0;
	coldxspeed = 0;
	coldyspeed = -100;
	coldscale = 1;
	coldangle = 0;

	xspeed = 0;
	yspeed = -150;

	cold_filter = 0xffffff;
	cold_opacityf = 0.15;


	pInfo->objectWidth = 128;
	pInfo->objectHeight = 128;

	USEMYRESOURCES()
	m_cold = AddImageFromResource2(MAKEINTRESOURCE(IDR_COLD));
	m_source = AddImageFromResource2(MAKEINTRESOURCE(IDR_FLAME));


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
	int Version = 2;
	SerializeVersion(ar, Version);

	if (ar.loading) {
		// Serialize D3DCOLOR, but process with COLORREF and OPACITY values at edittime.
		pEditTime->SerializeImage(m_cold,ar);
		pEditTime->SerializeImage(m_source,ar);

		D3DCOLOR f;
		ar >> f;

		float cold_opacity = D3D_A(f);
		cold_opacityf = (float)cold_opacity / 255.0f;
		cold_filter = RGB(D3D_R(f), D3D_G(f), D3D_B(f));

		ar >> coldx
		>> coldy
		>> coldxspeed
		>> coldyspeed
		>> coldscale
		>> coldangle
		>> xspeed
		>> yspeed;

		if(Version >= 2){
			for(int i = 0; i < 6; i++){
				ar >> render_filter[i] 
			       >> render_strength[i]
				   >> render_cutoff[i];
			}
		}
	}
	else {
		// Code to save your object here
		pEditTime->SerializeImage(m_cold,ar);
		pEditTime->SerializeImage(m_source,ar);

		D3DCOLOR f = D3D_CONVERTCOLORREF(cold_filter);
		f = D3D_ARGB((int)(cold_opacityf * 255), D3D_R(f), D3D_G(f), D3D_B(f));
		
		ar << f;
		ar << coldx
		<< coldy
		<< coldxspeed
		<< coldyspeed
		<< coldscale
		<< coldangle
		<< xspeed
		<< yspeed;

		for(int i = 0; i < 6; i++){
			ar << render_filter[i] 
			   << render_strength[i]
			   << render_cutoff[i];
		}
	}
}


void EditExt::BtnEditImage(int id)
{
	if(id == 0)
		pEditTime->EditImage(m_cold);
	if(id == 1)
		pEditTime->EditImage(m_source);


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