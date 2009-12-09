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
#define IDE_FLAGS 	OF_EFFECTS | OF_PRIVATEVARIABLES | OF_MOVEMENTS | OF_ALLOWANGLES

#define COMMONACE_POSITION
#define COMMONACE_DIMENSION
#define COMMONACE_FILTER
#define COMMONACE_ANGLE
#define COMMONACE_ZORDER
#define COMMONACE_COUNT_DESTROY
#define COMMONACE_VISIBILITY
#define COMMONACE_IDS

#include "..\..\Common\ExpReturn.hpp"

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)

typedef map<CString, ExpStore>						PrivateValueMap;
typedef map<CString, ExpStore>::iterator			PrivateValueIterator;
typedef const map<CString, ExpStore>::const_iterator PrivateValueConstIterator;

struct POINTD {
	double x;
	double y;
};

class CObjectRectangle
{
public:
	double x;
	double y;
	double w;
	double h;
	double a;
	double ox;
	double oy;

	CObjectRectangle(double _x, double _y, double _w, double _h,double _a = 0, double _ox = 0.5, double _oy = 0.5);
	CRect GetBoundingRect();
	bool PtInRect(POINT point);
	void CalculateBlitValues(double& _x, double& _y, double& _w, double& _h, double& _a);
	CPoint GetPoint(double xratio, double yratio, double distance = 0);
	
};

class Quad {
public:
	POINTD p1, p2, p3, p4;

	Quad() {}
	void operator=(const RECTF& rc) {
		p1.x = p3.x = rc.left;
		p2.x = p4.x = rc.right;
		p1.y = p2.y = rc.top;
		p3.y = p4.y = rc.bottom;
	}
	void Rotated(const RECTF& rc, double angle);
};

enum hotspot_position {
	hs_topleft,
	hs_top,
	hs_topright,
	hs_left,
	hs_center,
	hs_right,
	hs_bottomleft,
	hs_bottom,
	hs_bottomright
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

	IRenderer* const renderer;

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

	long cValueCmp(LPVAL theParams);
	long cTrigger(LPVAL theParams);

	long aSetValue(LPVAL theParams);
	long aSetOpacity(LPVAL theParams);

	long aAddValue(LPVAL theParams);
	long aSubValue(LPVAL theParams);

	long aSetColor1(LPVAL theParams);
	long aSetColor2(LPVAL theParams);
	long aSetFill(LPVAL theParams);
	long aSetTransparent(LPVAL theParams);

	long aAttachToObj(LPVAL theParams);

	long eGetOpacity(LPVAL theParams, ExpReturn& ret);
	long eGetValue(LPVAL theParams, ExpReturn& ret);


	////////////////////////////////////////////////////
	// Data members

	// Use when private variables (OF_PRIVATEVARIABLES) are enabled.
	vector<ExpStore> privateVars;

	// For triggering "On collision" - remembering previous state
	map<void*, bool> collisionMemory;

	// Colours
	COLORREF c1;
	COLORREF c2;
	COLORREF fill;

	bool transparent;
	//int c1_opacity;
	//int c2_opacity;
	//int fill_opacity;

	// For XY ret
	ExpStore xyRet[2];

	//Quad q;

	CRunObject* objAttach;		// Attached object
	int padding;				// Padding for attached object

	bool smoothLines;
	hotspot_position hotspot_pos;
	float hs_xf;
	float hs_yf;
	float oldw, oldh;

	struct cr_colors_t {
		cr::color c1;
		cr::color c2;
		cr::color fill;
	} cr_colors;

	void UpdateMyColors();
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
	void GetAnimationHandle(int& handle);

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	///////////////////////////
	// Your edittime extension data goes here

	///////////////////////////
	// Your edittime extension data goes here
	float angle;

	// Colours
	COLORREF c1;
	COLORREF c2;
	COLORREF fill;
	int sysc1;
	int sysc2;
	int sysfill;
	bool transparent;

	float c1_opacityf;
	float c2_opacityf;
	float fill_opacityf;

	int c1_opacity(){return c1_opacityf * 255;}
	int c2_opacity(){return c2_opacityf * 255;}
	int fill_opacity(){return fill_opacityf * 255;}
	Quad q;

	bool smoothLines;
	int hotspot_pos;
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif