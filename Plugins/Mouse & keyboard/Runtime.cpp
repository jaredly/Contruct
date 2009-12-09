// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

HHOOK g_hKeyboardHook = NULL;
/*
typedef struct tagKBDLLHOOKSTRUCT {
    DWORD   vkCode;
    DWORD   scanCode;
    DWORD   flags;
    DWORD   time;
    unsigned long* dwExtraInfo;
} KBDLLHOOKSTRUCT, FAR *LPKBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;
*/
#define WH_KEYBOARD_LL     13

/*
LRESULT CALLBACK DisableWinKeyKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

	if (nCode == HC_ACTION && (wParam==WM_KEYDOWN || wParam==WM_SYSKEYDOWN || 
wParam == WM_KEYUP || wParam == WM_SYSKEYUP) && (p->vkCode == VK_LWIN || p->vkCode == VK_RWIN))
		return 1;

    return CallNextHookEx( g_hKeyboardHook, nCode, wParam, lParam );
}
*/

// ExtObject constructor:
// Called when Construct creates an instance of your object.  Use your editTime data
// to initialise the object.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
{
	info.editObject = editObject;
	pRuntime = pVRuntime;

	info.x = editObject->eX;
	info.y = editObject->eY;
	info.w = editObject->eWidth;
	info.h = editObject->eHeight;
}

// This is called just after the constructor, when Construct has added details to your
// object such as siblings and type pointer.  If you need this data on startup, it is
// valid to read here.
void ExtObject::OnCreate()
{
	bool disableWinKey;

	cursorVisible = true;

	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	int Version = 0;
	ar >> Version;

	ar >> disableWinKey;
	ar.detach();

	pRuntime->RegisterWindow(pRuntime->GetFrameHwnd(pLayout), this);

	for (int i = 0; i < 256; i++)
		keyReleaseFirst[i] = true;
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
	if (g_hKeyboardHook != NULL) {
		UnhookWindowsHookEx(g_hKeyboardHook);
		g_hKeyboardHook = NULL;
	}
}

// Called every frame for you to update your object if necessary
// Return values: 1 (do not call RTOnFrame again), 0 (continue calling)
// The frame call order goes like this:
// run events -> OnFrame -> Draw -> OnFrame2 -> run events -> ...
BOOL ExtObject::OnFrame()
{
	return 1;	// Don't call again
}

BOOL ExtObject::OnFrame2()
{
	return 1;
}

// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your extension doesn't draw anything.
void ExtObject::Draw()
{
	// Don't draw
}

// WindowProc:  called when a window message, or WM_COMMAND specifying your window,
// is sent to Construct.  You must call RegisterWindow() before Construct will send
// you messages for the given window.
LRESULT ExtObject::WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	// Remember -2 allows message to fall through to another object.
	// This object never intercepts a message - merely watches it then passes it
	// on through the runtime again.

	switch (uMessage) {
	case WM_LBUTTONDOWN:
		triggeredMouseButton = 0;		// left
		triggeredClickType = 0;			// Single

		pRuntime->GenerateEvent2(0);	// "On Click"
		pRuntime->GenerateEvent2(1);	// "On any mouse button"
		pRuntime->GenerateEvent2(4);	// "Object clicked"

		triggeredClickType = 2;			// Either
		pRuntime->GenerateEvent2(0);	// "On Click"
		pRuntime->GenerateEvent2(4);	// "Object clicked"

		return -2;
	case WM_MBUTTONDOWN:
		triggeredMouseButton = 1;		// middle
		triggeredClickType = 0;			// Single

		pRuntime->GenerateEvent2(0);	// "On Click"
		pRuntime->GenerateEvent2(1);	// "On any mouse button"
		pRuntime->GenerateEvent2(4);	// "Object clicked"

		triggeredClickType = 2;			// Either
		pRuntime->GenerateEvent2(0);	// "On Click"
		pRuntime->GenerateEvent2(4);	// "Object clicked"

		return -2;
	case WM_RBUTTONDOWN:
		triggeredMouseButton = 2;		// right
		triggeredClickType = 0;			// Single

		pRuntime->GenerateEvent2(0);	// "On Click"
		pRuntime->GenerateEvent2(1);	// "On any mouse button"
		pRuntime->GenerateEvent2(4);	// "Object clicked"

		triggeredClickType = 2;			// Either
		pRuntime->GenerateEvent2(0);	// "On Click"
		pRuntime->GenerateEvent2(4);	// "Object clicked"

		return -2;
	case WM_LBUTTONDBLCLK:
		triggeredMouseButton = 0;		// left
		triggeredClickType = 1;			// Double

		pRuntime->GenerateEvent2(0);	// "On Click"
		pRuntime->GenerateEvent2(4);	// "Object clicked"

		triggeredClickType = 2;			// Either
		pRuntime->GenerateEvent2(0);	// "On Click"
		pRuntime->GenerateEvent2(4);	// "Object clicked"

		return -2;
	case WM_MBUTTONDBLCLK:
		triggeredMouseButton = 1;		// middle
		triggeredClickType = 1;			// Double

		pRuntime->GenerateEvent2(0);	// "On Click"
		pRuntime->GenerateEvent2(4);	// "Object clicked"

		triggeredClickType = 2;			// Either
		pRuntime->GenerateEvent2(0);	// "On Click"
		pRuntime->GenerateEvent2(4);	// "Object clicked"

		return -2;
	case WM_RBUTTONDBLCLK:
		triggeredMouseButton = 2;		// right
		triggeredClickType = 1;			// Double

		pRuntime->GenerateEvent2(0);	// "On Click"
		pRuntime->GenerateEvent2(4);	// "Object clicked"

		triggeredClickType = 2;			// Either
		pRuntime->GenerateEvent2(0);	// "On Click"
		pRuntime->GenerateEvent2(4);	// "Object clicked"

		return -2;
	case WM_LBUTTONUP:
		triggeredMouseButton = 0;		// left
		pRuntime->GenerateEvent2(2);	// "On Released"

		return -2;
	case WM_MBUTTONUP:
		triggeredMouseButton = 1;		// middle
		pRuntime->GenerateEvent2(2);	// "On Released"

		return -2;
	case WM_RBUTTONUP:
		triggeredMouseButton = 2;		// right
		pRuntime->GenerateEvent2(2);	// "On Released"

		return -2;
	case WM_KEYDOWN:
		triggeredKey = wParam;

		// No autorepeats: bit 30 is previous status, 1 if down, 0 if up - must be up before
		if (!(lParam & 0x40000000))
			pRuntime->GenerateEvent2(10);	// "Any key"

		return -2;
		/*
	case WM_KEYUP:
		triggeredKey = wParam;
		pRuntime->GenerateEvent2(13);	// "On released"
		return -2;
		*/
	case WM_MOUSEWHEEL:
		{
		short zDelta = HIWORD(wParam);
		if (zDelta >= 0)
			pRuntime->GenerateEvent2(6);	// On Mouse Wheel Up
		else
			pRuntime->GenerateEvent2(7);	// On Mouse Wheel Down
		}
		return -2;		
	}

	return -2;
}

BOOL ExtObject::PreTranslateMessage(MSG* msg)
{
	return 0;
}

// For global objects
void ExtObject::OnFrameChange(int oldFrame, int newFrame)
{
	// Do anything your global object needs when the frame changes
	// oldFrame is -1 on start of app, newFrame is -1 on end of app
}

void ExtObject::Serialize(bin& ar)
{
	// Implement saving/loading of your object
	int Version = 1;
	SerializeVersion(ar, Version);

}

long ExtObject::GetData(int id, void* param)
{
	switch (id) {
	// If your extension uses a window, you can return it here
	case GET_WINDOW:
		break;
	}

	return 0;
}

long ExtObject::CallFunction(int id, void* param)
{
	return 0;
}

// Return the Default Value.  This is the value to be returned if your object name
// is used without an expression name, eg. in "Counter + 1".
// Parameters can be passed, eg. MyArray(3,7).
long ExtObject::ReturnDefaultValue(LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

// Called for undefined expression names, if your extension allows it.
// Eg: MyObject.UndefinedExpression(3,4,5) calls this routine with
// "UndefinedExpression" as the expression name.
long ExtObject::ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

#else //ifdef RUN_ONLY

CRunObject* WINAPI RTCreateObject(bin& ar, VRuntime* pRuntime) 
{
	return NULL;
}

void WINAPI RTDestroyObject(ExtObject* object)
{
}

#endif