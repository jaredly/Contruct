// Main.cpp - Main implementation of object
//

// Include StdAfx
#include "StdAfx.h"

int mouseButtonTable[] = {
	VK_LBUTTON,
	VK_MBUTTON,
	VK_RBUTTON
};

struct ControlTableEntry {
	int vk;
	const char* name;
};

ControlTableEntry keyTable[] = {
	{VK_LBUTTON,	"Left mouse button"},
	{VK_MBUTTON,	"Middle mouse button"},
	{VK_RBUTTON,	"Right mouse button"},
	{VK_TAB,		"Tab"},
	{VK_RETURN,		"Enter"},
	{VK_SHIFT,		"Shift"},
	{VK_LSHIFT,		"Shift (left)"},
	{VK_RSHIFT,		"Shift (right)"},
	{VK_CONTROL,	"Control"},
	{VK_LCONTROL,	"Control (left)"},
	{VK_RCONTROL,	"Control (right)"},
	{VK_MENU,		"Alt"},
	{VK_LMENU,		"Alt (left)"},
	{VK_RMENU,		"Alt (right)"},
	{VK_PAUSE,		"Pause"},
	{VK_ESCAPE,		"Escape"},
	{VK_SPACE,		"Space"},
	{VK_HOME,		"Home"},
	{VK_END,		"End"},
	{VK_UP,			"Up arrow"},
	{VK_DOWN,		"Down arrow"},
	{VK_LEFT,		"Left arrow"},
	{VK_RIGHT,		"Right arrow"},
	{VK_INSERT,		"Insert"},
	{VK_DELETE,		"Delete"},
	{'0',			"0"},
	{'1',			"1"},
	{'2',			"2"},
	{'3',			"3"},
	{'4',			"4"},
	{'5',			"5"},
	{'6',			"6"},
	{'7',			"7"},
	{'8',			"8"},
	{'9',			"9"},
	{'A',			"A"},
	{'B',			"B"},
	{'C',			"C"},
	{'D',			"D"},
	{'E',			"E"},
	{'F',			"F"},
	{'G',			"G"},
	{'H',			"H"},
	{'I',			"I"},
	{'J',			"J"},
	{'K',			"K"},
	{'L',			"L"},
	{'M',			"M"},
	{'N',			"N"},
	{'O',			"O"},
	{'P',			"P"},
	{'Q',			"Q"},
	{'R',			"R"},
	{'S',			"S"},
	{'T',			"T"},
	{'U',			"U"},
	{'V',			"V"},
	{'W',			"W"},
	{'X',			"X"},
	{'Y',			"Y"},
	{'Z',			"Z"},
	{VK_BACK,		"Backspace"},
	{VK_NUMPAD0,	"Num pad 0"},
	{VK_NUMPAD1,	"Num pad 1"},
	{VK_NUMPAD2,	"Num pad 2"},
	{VK_NUMPAD3,	"Num pad 3"},
	{VK_NUMPAD4,	"Num pad 4"},
	{VK_NUMPAD5,	"Num pad 5"},
	{VK_NUMPAD6,	"Num pad 6"},
	{VK_NUMPAD7,	"Num pad 7"},
	{VK_NUMPAD8,	"Num pad 8"},
	{VK_NUMPAD9,	"Num pad 9"},
	{VK_ADD,		"Num pad Add"},
	{VK_SUBTRACT,	"Num pad Subtract"},
	{VK_MULTIPLY,	"Num pad Mulitply"},
	{VK_DIVIDE,		"Num pad Divide"},
	{VK_DECIMAL,	"Num pad Decimal"},
	{VK_F1,			"F1"},
	{VK_F2,			"F2"},
	{VK_F3,			"F3"},
	{VK_F4,			"F4"},
	{VK_F5,			"F5"},
	{VK_F6,			"F6"},
	{VK_F7,			"F7"},
	{VK_F8,			"F8"},
	{VK_F9,			"F9"},
	{VK_F10,		"F10"},
	{VK_F11,		"F11"},
	{VK_F12,		"F12"},
	{-1,			""},
};

// If run only
#ifdef RUN_ONLY

bool ExtObject::KeyIsDown(int vk)
{
	HWND parent = GetParent(GetFocus());
	HWND parent2 = GetParent(parent);
	HWND layoutHwnd = pRuntime->GetFrameHwnd(pLayout);

	if (GetFocus() == layoutHwnd || parent == layoutHwnd || parent2 == layoutHwnd)
		return AsyncKeyDown(vk);
	else return false;
}

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::cTrigger(LPVAL Params)
{
	return TRUE;
}

long ExtObject::cOnClickObject(LPVAL params)
{
	CRunObjType* pType = params[2].GetObjectParam(pRuntime);
	if (pType == NULL) return FALSE;

	int mouseButton = params[0].GetInt();
	int clickType = params[1].GetInt();

	if (mouseButton != triggeredMouseButton || clickType != triggeredClickType)
		return false;

	// Get mouse position in screen coords
	POINT mouse;
	GetCursorPos(&mouse);
	pRuntime->ScreenToScreen(mouse);

	return pRuntime->QuerySelectScreenPointCollision(mouse.x, mouse.y, pType);
}

long ExtObject::cMouseOverObject(LPVAL params)
{
	CRunObjType* pType = params[0].GetObjectParam(pRuntime);
	if (pType == NULL) return FALSE;

	// Get mouse position in screen coords
	POINT mouse;
	GetCursorPos(&mouse);
	pRuntime->ScreenToScreen(mouse);

	return pRuntime->QuerySelectScreenPointCollision(mouse.x, mouse.y, pType);
}

long ExtObject::cOnMouseButton(LPVAL params)
{
	int mouseButton = params[0].GetInt();
	int clickType = params[1].GetInt();
	return mouseButton == triggeredMouseButton && clickType == triggeredClickType;
}

long ExtObject::cOnMouseButtonReleased(LPVAL params)
{
	int mouseButton = params[0].GetInt();
	return mouseButton == triggeredMouseButton;
}

long ExtObject::cOnControl(LPVAL params)
{
	CString* str = params[0].GetStringPtr();
	if (str == NULL) return false;

	bool controlDown = pRuntime->GetControlState(*str, params[1].GetInt() - 1) > 0;
	return MakeConditionTrigger(pRuntime, controlDown);
}

long ExtObject::cMouseButtonDown(LPVAL params)
{
	int mouseButton = params[0].GetInt();
	return KeyIsDown(mouseButtonTable[mouseButton]);
}

long ExtObject::cKeyDown(LPVAL params)
{
	return KeyIsDown(keyTable[params[0].GetInt()].vk);
}

long ExtObject::cOnKey(LPVAL params)
{
	int key = params[0].GetInt();
	return MakeConditionTrigger(pRuntime, KeyIsDown(keyTable[key].vk));
}

long ExtObject::cControlDown(LPVAL params)
{
	CString* str = params[0].GetStringPtr();
	if (str == NULL) return false;

	bool controlDown = pRuntime->GetControlState(*str, params[1].GetInt() - 1) > 0;
	return controlDown;
}

long ExtObject::cKeyReleased(LPVAL params)
{
	int key = params[0].GetInt();

	bool& first = keyReleaseFirst[keyTable[key].vk];
	bool ret = MakeConditionTrigger(pRuntime, !KeyIsDown(keyTable[key].vk)) && !first;
	first = false;
	return ret;
}

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aSetCursorVisible(LPVAL params)
{
	bool show = params[0].GetBool();

	if (show && !cursorVisible) {
		ShowCursor(show);
		cursorVisible = true;
	}
	else if (!show && cursorVisible) {
		ShowCursor(show);
		cursorVisible = false;
	}

	return 0;
}

long ExtObject::aSetMouseX(LPVAL params)
{
	POINT p;
	GetCursorPos(&p);
	double cx = GetSystemMetrics(SM_CXSCREEN);
	double cy = GetSystemMetrics(SM_CYSCREEN);

	double normX = (params[0].GetDouble() * 65536.0) / cx;
	double normY = ((double)p.y * 65536.0) / cy;

	INPUT i;
	i.type = INPUT_MOUSE;
	i.mi.dx = normX + 0.5;
	i.mi.dy = normY + 0.5;
	i.mi.mouseData = 0;
	i.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	i.mi.time = 0;
	i.mi.dwExtraInfo = 0;
	SendInput(1, &i, sizeof(INPUT));

	return 0;
}

long ExtObject::aSetMouseY(LPVAL params)
{
	POINT p;
	GetCursorPos(&p);
	double cx = GetSystemMetrics(SM_CXSCREEN);
	double cy = GetSystemMetrics(SM_CYSCREEN);

	double normX = ((double)p.x * 65536.0) / cx;
	double normY = (params[0].GetDouble() * 65536.0) / cy;

	INPUT i;
	i.type = INPUT_MOUSE;
	i.mi.dx = normX + 0.5;
	i.mi.dy = normY + 0.5;
	i.mi.mouseData = 0;
	i.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	i.mi.time = 0;
	i.mi.dwExtraInfo = 0;
	SendInput(1, &i, sizeof(INPUT));

	return 0;
}

long ExtObject::aSetControlState(LPVAL params)
{
	pRuntime->SetControlState(params[0].GetString(), params[1].GetInt(), params[2].GetFloat());

	return 1;
}



//////////////////////////////////////////////////////////////////////////////////
// Expressions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::eGetAbsoluteMouseX(LPVAL params, ExpReturn& ret)
{
	POINT p;
	GetCursorPos(&p);

	return ret = p.x;
}

long ExtObject::eGetAbsoluteMouseY(LPVAL params, ExpReturn& ret)
{
	POINT p;
	GetCursorPos(&p);

	return ret = p.y;
}

long ExtObject::eGetControlState(LPVAL params, ExpReturn& ret)
{
	CString* str = params[0].GetStringPtr();
	if (str == NULL) return ret = 0;

	return ret = pRuntime->GetControlState(*str, params[1].GetInt() - 1);
}


#endif // #ifdef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// ACE tables
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
	// Add conditions
	ADDPARAMCOMBO("Mouse button", "Select the mouse button to check for.", "Left|Middle|Right");
	ADDPARAMCOMBO("Click type",	"Select whether to check for a click or double click.", "Clicked|Double-clicked|Either single or double clicked");
	ADDCND("On click", "Mouse", "%o On %0 mouse button %1", &ExtObject::cOnMouseButton, "OnClick", CND_TRIGGERED);

	ADDCND("On any click", "Mouse", "%o On any click", &ExtObject::cTrigger, "OnAnyClick", CND_TRIGGERED);

	ADDPARAMCOMBO("Mouse button", "Select the mouse button to check for.", "Left|Middle|Right");
	ADDCND("On mouse button released", "Mouse", "%o On %0 mouse button released", &ExtObject::cOnMouseButtonReleased, "OnRelease", CND_TRIGGERED);

	ADDPARAMCOMBO("Mouse button", "Select the mouse button to check for.", "Left|Middle|Right");
	ADDCND("Mouse button is down?", "Mouse", "%o %0 mouse button is down", &ExtObject::cMouseButtonDown, "MouseButtonDown", 0);

	ADDPARAMCOMBO("Mouse button", "Select the mouse button to check for.", "Left|Middle|Right");
	ADDPARAMCOMBO("Click type",	"Select whether to check for a click or double click.", "Clicked|Double-clicked|Either single or double clicked");
	ADDPARAM(PARAM_OBJECT, "Object", "Object to test clicked.");
	ADDCND("On object clicked", "Mouse", "%o On %0 %1 on %2", &ExtObject::cOnClickObject, "OnClickObject", CND_TRIGGERED | SOL_MODIFIER);

	ADDPARAM(PARAM_OBJECT, "Object", "Object to test for mouse over.");
	ADDCND("Mouse is over object?", "Mouse", "%o Mouse is over %0", &ExtObject::cMouseOverObject, "MouseOverObject", SOL_MODIFIER);

	ADDCND("On mouse wheel up", "Mouse", "%o On Mouse Wheel Up", &ExtObject::cTrigger, "OnWheelUp", CND_TRIGGERED);
	ADDCND("On mouse wheel down", "Mouse", "%o On Mouse Wheel Down", &ExtObject::cTrigger, "OnWheelDown", CND_TRIGGERED);

	CString keysCombo;
	for (int i = 0; keyTable[i].vk != -1; i++) {
		keysCombo += keyTable[i].name;

		if (keyTable[i+1].vk != -1)
			keysCombo += "|";
	}

	ADDPARAMCOMBO("Key", "Key to check if pressed.", keysCombo);
	ADDCND("On key pressed", "Keyboard", "%o On key %0 pressed", &ExtObject::cOnKey, "OnKey", 0);

	ADDPARAMCOMBO("Key", "Key to check if down.", keysCombo);
	ADDCND("Key is down?", "Keyboard", "%o Key %0 is down", &ExtObject::cKeyDown, "KeyDown", 0);

	ADDCND("On any key pressed", "Keyboard", "%o On any key pressed", &ExtObject::cTrigger, "OnAnyKey", CND_TRIGGERED);

	ADDPARAM(PARAM_STRING, "Control", "Name of the control to test.");
	ADDPARAMDEF(PARAM_VALUE, "Player (1-10)", "Player number who has this control.", "1");
	ADDCND("On control pressed", "Controls", "%o On player %1 pressed %0", &ExtObject::cOnControl, "OnControl", 0);

	ADDPARAM(PARAM_STRING, "Control", "Name of the control to test.");
	ADDPARAMDEF(PARAM_VALUE, "Player (1-10)", "Player number who has this control.", "1");
	ADDCND("Control is down?", "Controls", "%o Player %1 %0 is down", &ExtObject::cControlDown, "ControlDown", 0);

	ADDPARAMCOMBO("Key", "Key to check if released.", keysCombo);
	ADDCND("On key released", "Keyboard", "%o On key %0 released", &ExtObject::cKeyReleased, "OnRelease", 0);

	///////////////////////////////////

	ADDPARAMCOMBO("Cursor visibility", "Select whether to hide or show the cursor.", "Invisible|Visible");
	ADDACT("Set cursor visible", "Mouse", "Make cursor %0", &ExtObject::aSetCursorVisible, "SetCursorVisible", 0);

	ADDPARAM(PARAM_VALUE, "X co-ordinate on screen", "The X co-ordinate to place the cursor on screen.");
	ADDACT("Set mouse X", "Mouse", "Set mouse X to %0", &ExtObject::aSetMouseX, "SetMouseX", 0);

	ADDPARAM(PARAM_VALUE, "Y co-ordinate on screen", "The Y co-ordinate to place the cursor on screen.");
	ADDACT("Set mouse Y", "Mouse", "Set mouse Y to %0", &ExtObject::aSetMouseY, "SetMouseY", 0);

	ADDPARAM(PARAM_STRING, "Control", "Name of the control to set.");
	ADDPARAMDEF(PARAM_VALUE, "1", "Player number who has this control.", "1");
	ADDPARAM(PARAM_VALUE, "State Value", "The Y co-ordinate to place the cursor on screen.");
	ADDACT("Set control state", "Controls", "Set control state of %0 to %1", &ExtObject::aSetControlState, "SetControlState", 0);


	// Shortcuts to sys expression
	ADDEXP("Get mouse absolute X", "Screen", "AbsMouseX", &ExtObject::eGetAbsoluteMouseX, RETURN_VALUE);
	ADDEXP("Get mouse absolute Y", "Screen", "AbsMouseY", &ExtObject::eGetAbsoluteMouseY, RETURN_VALUE);
	ADDEXP("Get mouse X", "Mouse", "<MouseX>", NULL, 0);
	ADDEXP("Get mouse Y", "Mouse", "<MouseY>", NULL, 0);

	ADDPARAM(PARAM_STRING, "\"\"", "Name of the control to test.");
	ADDPARAMDEF(PARAM_VALUE, "1", "Player number who has this control.", "1");
	ADDEXP("Get control state", "Controls", "ControlState", &ExtObject::eGetControlState, RETURN_VALUE);
}
