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

	PROPERTY_VALUE	(pp.rate,		"Rate",		"Number of particles to create per second, or in total for one-shot effects");
	PROPERTY_BOOL	(pp.oneShot,	"One-shot",	"Create all the particles in one go, then destroy when all particles expired");
	PROPERTY_BUTTON ("Edit", "Texture", "Edit", BtnEditTexture());
	PROPERTY_BOOL	(useTexture, "Use texture", "Enable textured particles.");
	PROPERTY_BOOL   (pp.renderAdditive, "Render Additive", "Tick to render each particle as 'additive'");

PROPERTY_NEW_CATEGORY("Creation Settings", "Properties for the particles that are created");
	PROPERTY_FLOAT	(pp.particleSize, "Size", "Size of particles, in pixels");
	PROPERTY_FLOAT  (pp.particleDisplayAngle, "Display Angle", "Display angle of the particles, in degrees"); //
	PROPERTY_PERCENT(pp.opacity, "Opacity", "Initial particle opacity");
	PROPERTY_FLOAT	(pp.speed, "Speed", "Particle speed, in pixels per second");

PROPERTY_NEW_CATEGORY("Creation Randomization", "Properties for the particles that are created");
	PROPERTY_FLOAT	(pp.sprayCone, "Spray cone", "Cone of spraying, in degrees");
	PROPERTY_FLOAT	(pp.xRandom, "X randomiser", "Random X co-ordinate offset on creation");
	PROPERTY_FLOAT	(pp.yRandom, "Y randomiser", "Random Y co-ordinate offset on creation");
	PROPERTY_FLOAT  (pp.sizeRandom, "Size randomiser", "Random size of particle on creation");//
	PROPERTY_FLOAT	(pp.daRandom, "Display angle randomiser", "Randomisation of the textures angle on creation"); //
	PROPERTY_FLOAT	(pp.initialSpeedRandom, "Speed randomiser ", "Particle speed, in pixels per second");
	PROPERTY_FLOAT  (pp.growRandom, "Grow randomiser", "Randomisation of the particle size change, in pixels per second");//
	PROPERTY_FLOAT  (pp.daTurnRandom, "Display angle turn randomiser", "Randomisation of the display angle turn speed");

PROPERTY_NEW_CATEGORY("Simulation Settings", "Properties for the particles that are created");
	PROPERTY_FLOAT	(pp.acc, "Acceleration", "Particle acceleration, in pixels per second per second");
	PROPERTY_FLOAT	(pp.gravity, "Gravity", "Particle gravity, in pixels per second per second");
	PROPERTY_FLOAT  (pp.gravityangle, "Gravity angle", "Particle gravity angle (0 = right, 90 = down, 180 = left... )"); //
	PROPERTY_FLOAT  (pp.grow, "Grow", "Particle size change, in pixels per second");//
	PROPERTY_FLOAT  (pp.daTurn, "Display angle turn speed", "Display angle rotation of particles, in degrees per second");//

PROPERTY_NEW_CATEGORY("Simulation Randomization", "Properties for the particles that are created");
	PROPERTY_FLOAT	(pp.angleRandomiser, "Angle randomiser", "Maximum random angle deflection per second");
	PROPERTY_FLOAT	(pp.speedRandom, "Speed randomiser", "Maximum speed adjustment per second");
	//PROPERTY_COLOR	(pp.colorRandom, "Color randomiser", "Maximum colour adjustment per second");
	PROPERTY_FLOAT	(pp.opacityRandom, "Opacity randomiser", "Maximum opacity adjustment per second");

PROPERTY_NEW_CATEGORY("Life Settings", "Propertys for things relating to the life of the particles");

	PROPERTY_COMBO	(pp.destroyMode, "Destroy mode", "Choose when each particle is destroyed", "Timeout expired|Particle stopped|Faded to invisible");
	PROPERTY_COLOR	(pp.color, "Color", "Initial color of particles");
	PROPERTY_COLOR	(pp.fadeColor, "Fade to color", "Color for particle to fade to over fade colour time");
	PROPERTY_VALUE	(pp.fadeColorTime, "Fade colour time", "Time for particles to fade to above color");
	PROPERTY_VALUE	(pp.fadeoutTime, "Fade out time", "Time to fade out, in milliseconds");
	PROPERTY_VALUE	(pp.lifeTime, "Timeout", "Timeout, in milliseconds, before destroying particle");

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
	imgHandle = -1;
	useTexture = false;
	pp.oneShot = false;
}


// Destructor: when your object is deleted or the application closed:
// close any allocated resources here
EditExt::~EditExt()
{

}

void EditExt::BtnEditTexture()
{
	if (imgHandle == -1)
		imgHandle = pEditTime->AddImage(32, 32);

	pEditTime->EditImage(imgHandle);
}

// Draw your object in the frame editor.
// Note the co-ordinates are automatically adjusted; draw at pInfo->objectX/Y
void EditExt::Draw()
{
	D3DCOLOR c1 = D3D_CONVERTCOLORREF(pp.color);
	D3DCOLOR c2 = D3D_CONVERTCOLORREF(pp.fadeColor);

	// Determine centre
	float x = pInfo->objectX;
	float y = pInfo->objectY;

	float radius = min(pInfo->objectWidth / 2, pInfo->objectHeight / 2);
	float angle = RADIANS(pInfo->objectAngle);
	float coneOff = RADIANS(pp.sprayCone / 2);

	pEditTime->SetTexture(-1);

	float endX = x + cos(angle - coneOff) * radius;
	float endY = y + sin(angle - coneOff) * radius;
	pEditTime->Line(x, y, endX, endY, c1, c2);

	endX = x + cos(angle + coneOff) * radius;
	endY = y + sin(angle + coneOff) * radius;
	pEditTime->Line(x, y, endX, endY, c1, c2);
}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and anything else DirectX volatile here.
void EditExt::Initialize()
{

}

void EditExt::OnPropertiesUpdate()
{

}

// When the object is inserted to the frame editor by the user.
void EditExt::OnPut()
{
	pInfo->objectWidth = 150;
	pInfo->objectHeight = 150;
	pInfo->originX = 0.5f;
	pInfo->originY = 0.5f;
	imgHandle = pEditTime->AddImage(32, 32);
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
	int Version = 5;
	SerializeVersion(ar, Version);

	if(Version >= 4)
		pp.Serialize(ar);

	if (ar.loading) {
		if(Version < 4){
			 ar >> pp.sprayCone >> pp.particleSize >> pp.color >> pp.opacity >> pp.speed >> pp.acc >> pp.destroyMode >> pp.lifeTime
				>> pp.angleRandomiser >> pp.rate >> pp.xRandom >> pp.yRandom >> pp.speedRandom >> pp.gravity >> pp.colorRandom
				>> pp.opacityRandom >> pp.fadeoutTime >> pp.fadeColor >> pp.fadeColorTime;
		}

		if (Version >= 2) {
			pEditTime->SerializeImage(imgHandle, ar);
			ar >> useTexture;
		}
		// Only version 3 uses this
		if (Version == 3) {
			ar >> pp.oneShot;
		}

	}
	else {
		if (imgHandle == -1)
			ar << imgHandle;
		else
			pEditTime->SerializeImage(imgHandle, ar);
		ar << useTexture;
	}
}

BOOL EditExt::OnNotify(int notify)
{
	return FALSE;
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