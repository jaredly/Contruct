// Edittime.cpp - Edittime implementation of object
//

// Include StdAfx and Properties.h
#include "StdAfx.h"
#include "ViewTextDlg.h"

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

BEGIN_PROPERTY_TABLE()
	PROPERTY_EDIT  (text,   "Text", "The text to display")
	PROPERTY_BUTTON("View all...", "", "Click to view all text in a dialog.", OnDoubleClick());
	PROPERTY_EDIT  (fontface,   "Font", "Name of the font eg. Verdana")
	PROPERTY_VALUE (pxSize,   "Size", "Font size")
	PROPERTY_BOOL  (bold,   "Bold", "Show bold text?")
	PROPERTY_BOOL  (italics,   "Italics", "Show italic text?")
	PROPERTY_BOOL	(hide_at_start, "Invisible on start", "Make the object invisible when the layout starts.");
	PROPERTY_COMBO (horizAlign, "Horizontal alignment", "The horizontal text alignment.", "Left|Center|Right")
	PROPERTY_COMBO (vertAlign, "Vertical alignment", "The vertical text alignment.", "Top|Center|Bottom")
	PROPERTY_COLOR (color,	"Colour",	"The text colour")
	PROPERTY_PERCENT (opacityf, "Opacity", "The semitransparency of the text")
END_PROPERTY_TABLE  ()

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

	hide_at_start = false;
}

// Destructor: when your object is deleted or the application closed:
// close any allocated resources here
EditExt::~EditExt()
{
	//
}

void EditExt::OnDoubleClick()
{
	USEMYRESOURCES();

	CViewTextDlg dlg;
	dlg.pExt = this;
	dlg.DoModal();
}

// Draw your object in the frame editor.
// Note the co-ordinates are automatically adjusted; draw at pInfo->objectX/Y
void EditExt::Draw()
{
	D3DCOLOR c = D3D_CONVERTCOLORREF(color);
	c = D3D_ARGB((int)(opacityf * 255.0f), D3D_R(c), D3D_G(c), D3D_B(c));

	DWORD dwFlags = DT_WORDBREAK;

	switch (horizAlign) {
		case 0: dwFlags |= DT_LEFT; break;
		case 1: dwFlags |= DT_CENTER; break;
		case 2: dwFlags |= DT_RIGHT; break;
	}

	switch (vertAlign) {
		case 0: dwFlags |= DT_TOP; break;
		case 1: dwFlags |= DT_VCENTER; break;
		case 2: dwFlags |= DT_BOTTOM; break;
	}

	//CString textDraw = text;
	//textDraw.Replace("\\n", "\n");

	RECT rc;
	SetRect(&rc, pInfo->objectX, pInfo->objectY, pInfo->objectX + pInfo->objectWidth, pInfo->objectY + pInfo->objectHeight);
	if (iFont != -1)
		pEditTime->DrawText(iFont, text, rc, c, dwFlags);
}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and anything else DirectX volatile here.
void EditExt::Initialize()
{
	iFont = pEditTime->CreateFont(fontface, pxSize, italics != 0, bold != 0);
}

// When the object is inserted to the frame editor by the user.
void EditExt::OnPut()
{
	text = "Text";
	fontface = "Arial";
	pxSize = 20;
	bold = false;
	italics = false;
	color = RGB(0,0,0);
	opacityf = 1.0f;
	horizAlign = 0;
	vertAlign = 0;

	pInfo->objectWidth = 150;
	pInfo->objectHeight = 20;
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
	if (iFont != -1)
		pEditTime->DestroyFont(iFont);

	Clamp(pxSize, 1, 500);
	Clamp(opacityf, 0.0f, 1.0f);

	iFont = pEditTime->CreateFont(fontface, pxSize, italics != 0, bold != 0);
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
		ar>>text>>fontface>>pxSize>>italics>>bold>>color>>opacityf>>horizAlign>>vertAlign;

		if (Version >= 2)
			ar >> hide_at_start;
	}
	else
	{
		ar<<text<<fontface<<pxSize<<italics<<bold<<color<<opacityf<<horizAlign<<vertAlign;

		ar << hide_at_start;
	}
}


BOOL WINAPI ETOnDoubleClick(editInfo* editObject)
{
	editObject->objectPtr->OnDoubleClick();
	return TRUE;
}

BOOL EditExt::OnNotify(int notify)
{
	// Double clicked
	if(notify == 1)
	{
		OnDoubleClick();
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
BOOL WINAPI ETOnDoubleClick(editInfo* editObject){	return FALSE; }

#endif // RUN_ONLY


BOOL WINAPI ETOnNotify(editInfo* editObject, int notification)
{
#ifdef RUN_ONLY
	return 0;
#else
	return ((EditExt*)editObject->objectPtr)->OnNotify(notification);
#endif
}