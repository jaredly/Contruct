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
// PROPERTY_EDIT  (myVariable, "Label", "Description);
// PROPERTY_STATIC(myVariable, "Label", "Description);

BEGIN_PROPERTY_TABLE();
	PROPERTY_BOOL   (autoHscroll, "Auto scroll horizontally", "If enabled, the user can type beyond the end of the text box horizontally");
	PROPERTY_BOOL   (autoVscroll, "Auto scroll vertically", "If enabled, the user can type beyond the end of the text box vertically");
	PROPERTY_BOOL   (border, "Border", "Show a border");
	PROPERTY_BOOL   (enabled, "Enabled", "Control is highlighted on startup");
	PROPERTY_BOOL   (hscroll, "Horizontal scrollbar", "Show a horizontal scrollbar");
	PROPERTY_EDIT   (text,   "Initial text"  , "Text to be shown on startup");
	PROPERTY_BOOL   (m_Lowercase, "Lowercase", "All textual input is lowercase");
	PROPERTY_BOOL   (multiline, "Multiline", "Text can be entered on multiple lines");
	PROPERTY_BOOL   (password, "Password mode", "Conceals the entered text");
	PROPERTY_BOOL   (m_Uppercase, "Uppercase", "All textual input is uppercase");
	PROPERTY_BOOL   (vscroll, "Vertical scrollbar", "Show a vertical scrollbar");
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

}

// Draw your object in the frame editor.
// Note the co-ordinates are automatically adjusted; draw at pInfo->objectX/Y
void EditExt::Draw()
{
	pEditTime->SetTexture(-1);

	CRect rc;
	SetRect(&rc, pInfo->objectX, pInfo->objectY, pInfo->objectX + pInfo->objectWidth, pInfo->objectY + pInfo->objectHeight);

	pEditTime->Box(rc, D3D_RGB(0, 0, 0));

	rc.DeflateRect(1, 1, 1, 1);

	pEditTime->Box(rc, D3D_SYSCOLOR(COLOR_BTNSHADOW));

	// now see if we're disabled
	if (!enabled)
	{
		rc.DeflateRect(1, 1, 1, 1);
		pEditTime->Fill(rc, D3D_RGB(245, 245, 245));
		rc.InflateRect(1, 1, 1, 1);
	}

	rc.DeflateRect(2, 2, 2, 2);
	rc.left += 2;
	rc.top += 2;

	CString render = text;
	if (password) render = "*******";

	if (enabled)
		pEditTime->DrawText(eFont, render, rc, D3D_SYSCOLOR(COLOR_BTNTEXT), 0);
	else
		pEditTime->DrawText(eFont, render, rc, D3D_SYSCOLOR(COLOR_GRAYTEXT), 0);
}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and anything else DirectX volatile here.
void EditExt::Initialize()
{
	eFont = pEditTime->CreateFont("Arial", 14, false, false);
}

void EditExt::OnPropertiesUpdate()
{
}

// When the object is inserted to the frame editor by the user.
void EditExt::OnPut()
{
	text = "";
	autoHscroll = true;
	autoVscroll = true;
	multiline = true;
	vscroll = false;
	hscroll = false;
	password = false;
	enabled = true;

	m_Uppercase = 0;
	m_Lowercase = 0;

	border = 1;

	pInfo->objectWidth = 150;
	pInfo->objectHeight = 25;
}

// When the object is removed from the frame editor by the user.
void EditExt::OnRemoved()
{

}

// On object resized
BOOL EditExt::OnSizeObject()
{
	return TRUE;
}

void EditExt::Serialize(bin& ar)
{
	int Version = 2;
	SerializeVersion(ar, Version);

	if (ar.loading) {
		// Code to load your object here
		ar >> text;
		ar >> autoHscroll;
		ar >> password;
		ar >> enabled;
		ar >> m_Lowercase >> m_Uppercase >> vscroll >> hscroll >> multiline >> autoVscroll;

		if (Version == 2)
			ar >> border;
	}
	else {
		// Code to save your object here
		ar << text;
		ar << autoHscroll;
		ar << password;
		ar << enabled;
		ar << m_Lowercase << m_Uppercase << vscroll << hscroll << multiline << autoVscroll;

		ar << border;
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