// Main.h
// Definitions and classes for the object workings

// Only include once
#ifndef MAIN_H
#define MAIN_H

#define IDE_FLAGS OF_NODEBUG | OF_NODRAW //OF_NOSIZE
#include "..\..\Common\ExpReturn.hpp"
#include "..\..\Common\bin.h"

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)

//////////// RUNTIME OBJECT ////////////
// Add any member functions or data you want to this class.
// Your extension inherits CRunObject.  See the definition
// for the default member values you have access to.
class ExtObject : public CRunObject
{
public:
	// Constructor (called when Construct creates the object)
	ExtObject(initialObject* editObject, VRuntime* pVRuntime);
	// Destructor (called when Construct destroys the object)
	~ExtObject();

	//////////////////////////
	// OnFrame: called once per frame just before Draw() (after the events list)
	// OnFrame2: called once per frame just after Draw() (before the events list)
	BOOL		OnFrame();
	BOOL		OnFrame2();
	// Draw: called when Construct wants you to render your object.
	void		Draw();
	// WindowProc is called if your extension creates a window and calls RegisterWindow().
	LRESULT		WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	BOOL		PreTranslateMessage(MSG* msg);
	long		ReturnDefaultValue(LPVAL theParams, ExpReturn& ret);
	long		ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret);
	void		OnCreate();
	// Misc functions
	void		OnFrameChange(int oldFrame, int newFrame);
	void		Serialize(bin& ar);
	// Data functions
	long		GetData(int id, void* param);
	long		CallFunction(int id, void* param);
	// Debugger
	void		DebuggerUpdateDisplay(ExpStore*& pPrivateVars) {}
	void		OnDebuggerValueChanged(const char* name, const char* value) {}

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS DEFINITIONS

	// Conditions
	long cTrigger(LPVAL params);
	long cMouseOverObject(LPVAL params);
	long cOnClickObject(LPVAL params);
	long cOnMouseButton(LPVAL params);
	long cOnMouseButtonReleased(LPVAL params);
	long cOnKey(LPVAL params);
	long cOnControl(LPVAL params);

	long cMouseButtonDown(LPVAL params);
	long cKeyDown(LPVAL params);
	long cControlDown(LPVAL params);

	long cKeyReleased(LPVAL params);

	long aSetCursorVisible(LPVAL params);
	long aSetMouseX(LPVAL params);
	long aSetMouseY(LPVAL params);
	long aSetControlState(LPVAL params);

	long eGetAbsoluteMouseX(LPVAL params, ExpReturn& ret);
	long eGetAbsoluteMouseY(LPVAL params, ExpReturn& ret);
	long eGetControlState(LPVAL params, ExpReturn& ret);

	// For triggers
	int triggeredMouseButton;
	int triggeredClickType;
	int triggeredKey;

	bool cursorVisible;
	bool keyReleaseFirst[256];

	bool KeyIsDown(int vk);

	
};

//////////// EDITTIME INFO ////////////
class EditExt
{
public:
	///////////////////////////
	// Class data
	EditExt(class VEditTime* pVEditTime, class editInfo* pEInfo);
	~EditExt();

	void Draw();
	int  GetMenuIcons(int ACEMenu);
	void Initialize();
	void OnPut();
	void OnRemoved();
	int  OnShowACEMenu(TABLE ACEType);
	BOOL OnSizeObject();
	void Serialize(bin& ar);
	void OnPropertiesUpdate();
	void GetAnimationHandle(int&) {}

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	///////////////////////////
	// Your edittime extension data goes here
	int iTexture;	// icon

	bool disableWinKey;
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

/*
typedef struct tagMOUSEINPUT {
    LONG    dx;
    LONG    dy;
    DWORD   mouseData;
    DWORD   dwFlags;
    DWORD   time;
    DWORD   dwExtraInfo;
} MOUSEINPUT, *PMOUSEINPUT, FAR* LPMOUSEINPUT;

typedef struct tagKEYBDINPUT {
    WORD    wVk;
    WORD    wScan;
    DWORD   dwFlags;
    DWORD   time;
    DWORD   dwExtraInfo;
} KEYBDINPUT, *PKEYBDINPUT, FAR* LPKEYBDINPUT;

typedef struct tagHARDWAREINPUT {
    DWORD   uMsg;
    WORD    wParamL;
    WORD    wParamH;
} HARDWAREINPUT, *PHARDWAREINPUT, FAR* LPHARDWAREINPUT;

typedef struct tagINPUT {
    DWORD   type;

    union
    {
        MOUSEINPUT      mi;
        KEYBDINPUT      ki;
        HARDWAREINPUT   hi;
    };
} INPUT, *PINPUT, FAR* LPINPUT;

#define INPUT_MOUSE     0
#define INPUT_KEYBOARD  1
#define INPUT_HARDWARE  2

WINUSERAPI
UINT
WINAPI
SendInput(
    IN UINT    cInputs,     // number of input in the array
    IN LPINPUT pInputs,     // array of inputs
    IN int     cbSize);     // sizeof(INPUT)
	*/

// Only include once
#endif