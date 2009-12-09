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

	PROPERTY_BUTTON("Make 1:1", "Size", "Click to make the object the original size.", BtnMakeOriginalSize());
	PROPERTY_EDIT (startAnim, "Start Animation", "Name of the animation that is playing on startup");
	PROPERTY_VALUE (startFrame, "Start Frame", "Number of the first frame to play (starting at 1)");
	PROPERTY_COMBO(collMode, "Collisions", "Per Pixel", "None|Point|Bounding box|Per Pixel");
PROPERTY_NEW_CATEGORY("Appearance", "Options for appearance");
	PROPERTY_BOOL	(hideAtStart, "Invisible on start", "Make the object invisible when the layout starts.");
	PROPERTY_FLOAT  (skewX, "Skew X", "Horizontal skew of the object, in pixels.");
	PROPERTY_FLOAT  (skewY, "Skew Y", "Vertical skew of the object, in pixels.");
PROPERTY_NEW_CATEGORY("Angle", "Affects how the object appears at certain angles, noticable with movements");
	PROPERTY_BOOL(autoMirror, "Auto mirror", "When your object faces the left, it will automatically mirror it to face that direction");
	PROPERTY_BOOL(autoFlip, "Auto flip", "When your object faces upward, it will automatically flip it to face that direction");
	PROPERTY_COMBO(autoRotationsCombo, "Rotation", "Determines how construct rotates your object", "Normal|No rotation|N angles");
	PROPERTY_VALUE(autoRotations, "Rotations Count", "When 'Rotation' is 'N angles', number of angles in a full circle.");
	PROPERTY_BOOL(locked_animation_angles, "Lock animation angles", "Always set the animation angle to the object angle.");

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

	skewX = skewY = 0;
	startAnim = "Default";
	startFrame = 1;

	locked_animation_angles = true;

}

int FindAnAnimationWithName(VEditTime* pEditTime, int anim, CString name)
{
	if(pEditTime->QueryAnimationFrameCount(anim) > 0 && name.CompareNoCase(pEditTime->GetAnimationName(anim)) == 0)
		return anim;
	else
	{
		int count = pEditTime->GetSubAnimationCount(anim);
		for(int a = 0; a < count; a++)
		{
			int result = FindAnAnimationWithName(pEditTime, pEditTime->GetSubAnimation(anim, a), name);
			if(result != -1)
				return result;
		}
	}
	return -1;
}

int FindFirstAnimation(VEditTime* pEditTime, int anim)
{
	if(pEditTime->QueryAnimationFrameCount(anim) > 0)
		return anim;
	else
	{
		int count = pEditTime->GetSubAnimationCount(anim);
		for(int a = 0; a < count; a++)
		{
			int result = FindFirstAnimation(pEditTime, pEditTime->GetSubAnimation(anim, a));
			if(result != -1)
				return result;
		}
	}
	return -1;
}

int FindAnAnimation(VEditTime* pEditTime, int anim, CString name)
{
	int a = FindAnAnimationWithName(pEditTime, anim, name);
	if(a == -1)
		a = FindFirstAnimation(pEditTime, anim);
	return a;
}

void EditExt::UpdateIcons()
{
	int image;

	image = FindAFrame(pEditTime, m_iAnimationRoot, startAnim, startFrame);

	pEditTime->GenerateIcon(this->pInfo->ObjectIdentifier,false,image);
	pEditTime->GenerateIcon(this->pInfo->ObjectIdentifier,true,image);
}

void EditExt::BtnMakeOriginalSize()
{
	int image = FindAFrame(pEditTime, m_iAnimationRoot, startAnim, startFrame);
	SIZE s = pEditTime->GetImageSize(image);
	pInfo->objectWidth = s.cx;
	pInfo->objectHeight = s.cy;
}

// Destructor: when your object is deleted or the application closed:
// close any allocated resources here
EditExt::~EditExt()
{

}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and anything else DirectX volatile here.
void EditExt::Initialize()
{
	// Make a texture from the object's image
	int image = FindAFrame(pEditTime, m_iAnimationRoot, startAnim, startFrame);
	iTexture = pEditTime->AddImageAsTexture(image);

}

void EditExt::OnPropertiesUpdate()
{
	int image = FindAFrame(pEditTime, m_iAnimationRoot, startAnim, startFrame);
	SIZE newimgsize = pEditTime->GetImageSize(image);

	if(imgsize.cx && imgsize.cy)
	{
		pInfo->objectWidth *= newimgsize.cx / (float)imgsize.cx ;
		pInfo->objectHeight *= newimgsize.cy / (float)imgsize.cy ;
	}

}

// When the object is inserted to the frame editor by the user.
void EditExt::OnPut()
{
	pInfo->objectWidth = 120;
	pInfo->objectHeight = 120;
	collMode = 3;
 
	m_iAnimationRoot = pEditTime->AddAnimation(0, 0, 0, "Animation", -1);
	int anim = pEditTime->AddAnimation(0, 0, 0, "Default", m_iAnimationRoot);
	int subanim = pEditTime->AddAngle(128,128,1,0.0, anim);


	pEditTime->EditAnimation(subanim);
	int image = FindAFrame(pEditTime, m_iAnimationRoot, startAnim, startFrame);
	if(image == -1)
		return;

	iTexture  = pEditTime->AddImageAsTexture(image);

	SIZE s = pEditTime->GetImageSize(image);

	pInfo->objectWidth = s.cx;
	pInfo->objectHeight = s.cy;

	autoFlip = false;
	autoMirror = false;
	autoRotationsCombo = 0;
	autoRotations = 8;
	hideAtStart = false;

	pEditTime->GenerateIcon(this->pInfo->ObjectIdentifier,false,image);
	pEditTime->GenerateIcon(this->pInfo->ObjectIdentifier,true,image);
}


// When the object is removed from the frame editor by the user.
void EditExt::OnRemoved()
{

}

// On object resized
BOOL EditExt::OnSizeObject()
{
	// TRUE = allow sizing to occur, FALSE = disallow
	return TRUE;
}

void EditExt::GetAnimationHandle(int& iAnimation)
{
	iAnimation = m_iAnimationRoot;
}

BOOL EditExt::OnNotify(int notify)
{
	if(notify == 1)
	{
		int anim = FindAnAnimation(pEditTime, m_iAnimationRoot, startAnim);
		pEditTime->EditAnimation(anim);

		UpdateIcons();
	}
	if(notify == 3)
	{
		UpdateIcons();
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