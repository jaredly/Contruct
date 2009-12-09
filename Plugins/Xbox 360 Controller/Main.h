// Main.h
// Definitions and classes for the object workings

// Only include once
#ifndef MAIN_H
#define MAIN_H

#define MINIMUM_VERSION	1

// IDE FLAGS
// Can be a combination of:
//OF_NOSIZE	- Extension cannot be resized at edittime
//OF_NODRAW - Extension does not display anything
//OF_ALLOWANGLES - Can be rotated at edittime
//OF_PRIVATEVARIABLES - Uses private variables
//OF_MOVEMENTPLUGIN - Is a movement plugin
//OF_MOVEMENTS - Allow movement plugins to be added
//OF_EFFECTS - Allow display effects
//OF_NOCOMMONDEBUG - Don't show 'Common' field (X/Y/W/H/Angle/Opacity) in debugger
//OF_NODEBUG - Don't show at all in debugger
//OF_UNDEFINEDEXPRESSIONS - Allow ReturnUndefinedExpression to be called
#define IDE_FLAGS 	OF_NOCOMMONDEBUG|OF_NODRAW

#include "..\..\Common\ExpReturn.hpp"

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)

class Control{
public:
	Control()
	{
		control = ""; 
		player = 0; 
		c = 0;
	}
	Control(int _player, std::string _control, int comboc)
	{
		player = _player;
		control = _control; 
		c = comboc;
	}
	Control(const Control& other)
	{
		control = other.control; 
		player = other.player; 
		c = other.c;
	}
	std::string control;
	int player;
	int c; // combo value;
};

class RunControl{
public:
	RunControl()
	{
		control = ""; 
		player = 0; 
		controller = 0; 
		xboxControl = XBoxControl::DPadUp;
	}
	RunControl(Control& ed, int initController, XBoxControl::Control initXboxControl)
	{
		control = ed.control; 
		player = ed.player; 
		controller = initController; 
		xboxControl = initXboxControl;
	}
	RunControl(const RunControl& other)
	{
		control = other.control; 
		player = other.player; 
		controller = other.controller;
		xboxControl = other.xboxControl;
	}

	std::string control;
	int player;
	int controller;
	XBoxControl::Control xboxControl;
};

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
	// Return default expression editor value and undefined expressions when enabled.
	long		ReturnDefaultValue(LPVAL params, ExpReturn& ret);
	long		ReturnUndefinedExpression(CString& expName, LPVAL params, ExpReturn& ret);
	// Called just after the constructor when your object has been prepared.
	void		OnCreate();
	// Called when a frame changes and your object is global
	void		OnFrameChange(int oldFrame, int newFrame);
	// Called when your object should save/load itself at runtime
	void		Serialize(bin& ar);
	// Data functions
	long		GetData(int id, void* param);
	long		CallFunction(int id, void* param);
	// Debugging
	void		DebuggerUpdateDisplay(ExpStore*& pPrivateVars);
	void		OnDebuggerValueChanged(const char* name, const char* value);

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS DEFINITIONS
#include "..\..\Common\CommonAceDecl.hpp"

	long cButtonDown(LPVAL params);

	long aVibrate(LPVAL params);

	long eLeftThumbX(LPVAL params, ExpReturn& ret);
	long eLeftThumbY(LPVAL params, ExpReturn& ret);
	long eRightThumbX(LPVAL params, ExpReturn& ret);
	long eRightThumbY(LPVAL params, ExpReturn& ret);
	long eLeftTrigger(LPVAL params, ExpReturn& ret);
	long eRightTrigger(LPVAL params, ExpReturn& ret);
	long eButtonState(LPVAL params, ExpReturn& ret);

	CXBOXController* GetPlayer(LPVAL params);
	CXBOXController* Player[4];

	vector<RunControl> controls;

	////////////////////////////////////////////////////
	// Data members

	// Use when private variables (OF_PRIVATEVARIABLES) are enabled.
	//vector<ExpStore> privateVars;
};

#define CONTROLLERS 4
#define BUTTONS 26
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
	void GetAnimationHandle(int& handle);

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	///////////////////////////
	// Your edittime extension data goes here
	Control controls[CONTROLLERS][BUTTONS]; 
	int controllerCombo;
};

bin& operator<<(bin& b, const Control& c);
bin& operator>>(bin& b, Control& c);

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif