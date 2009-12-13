// Main.h
// Definitions and classes for the object workings

// Only include once
#ifndef MAIN_H
#define MAIN_H

#define MINIMUM_VERSION	1
#define IDE_FLAGS 	OF_MOVEMENTS | OF_EFFECTS | OF_PRIVATEVARIABLES
#include "..\..\Common\ExpReturn.hpp"
#include "..\..\Common\bin.h"

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)

#define COMMONACE_POSITION
#define COMMONACE_DIMENSION
#define COMMONACE_ZORDER
#define COMMONACE_ANGLE
#define COMMONACE_VISIBILITY
#define COMMONACE_OPACITY
#define COMMONACE_FILTER
#define COMMONACE_IDS
#define COMMONACE_COUNT_DESTROY

// Draw command queue
class DrawCommand {
public:
	virtual void Do(class ExtObject* obj) = 0;
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

	void GenerateCollision();
	void PerformDrawingQueue();
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
	void		DebuggerUpdateDisplay(ExpStore*& pPrivateVars);
	void		OnDebuggerValueChanged(const char* name, const char* value);

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS DEFINITIONS
#include "..\..\Common\CommonAceDecl.hpp"

	long cValueCmp(LPVAL theParams);

	long aSetValue(LPVAL theParams);
	long aAddValue(LPVAL theParams);
	long aSubValue(LPVAL theParams);
	long aDestroy(LPVAL theParams);

	long aSetDebuggingPf(LPVAL aprams);

	long aClearTransparent(LPVAL theParams);
	long aClearColour(LPVAL theParams);
	long aFill(LPVAL theParams);
	long aPoint(LPVAL theParams);
	long aLine(LPVAL theParams);
	long aCopyScreen(LPVAL theParams);
	long aPasteObject(LPVAL theParams);
	long aGenerateMask(LPVAL theParams);
	long aLoadFromFile(LPVAL params);

	long eGetValue(LPVAL theParams, ExpReturn& ret);

	////////////////////////////////////////////////////
	// Private values
	vector<ExpStore> privateVars;

	int firstFrame;
	TextureHandle startTexture;
	int grabbingFrame;

	vector<DrawCommand*> drawQueue;

	// Flash counters
	int flashTime;		// Time left to flash (counts down to 0)
	int flashCounter;	// Increments to flashInterval
	int flashInterval;	// Num frames between flashes
	CRect oldRect;

	bool textureChanged;
	bool generateMask;

	// debug stuff
	bool pfdebug;
	list<CPoint>* pWaypoints;
	int pfcellW;
	int pfcellH;
	unsigned short* pfMap;

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
	BOOL OnNotify(int notify);

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	///////////////////////////
	// Your edittime extension data goes here
	int iTexture;		// DX texture
	int imgTexture;		// Construct image handle

	float angle;
	//COLORREF filter;
	//int opacity;
	//float opacityf;
	int collMode;
	int grabFrame;

	// Animations
	int m_iAnimationRoot;

	void BtnEditImage();
	void BtnEditAnimations();
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

class DrawClear : public DrawCommand {
public:
	cr::color c;
	DrawClear(const cr::color& _c)
		:	c(_c)
	{
	}

	void Do(ExtObject* obj) {
		obj->renderer->ClearRenderTarget(c);
	}
};

class DrawFill : public DrawCommand {
public:
	RECTF rc;
	cr::color c;
	DrawFill(const cr::color& _c, CRect& rec)
		: c(_c)
	{
		rc.left = rec.left;
		rc.top = rec.top;
		rc.right = rec.right;
		rc.bottom = rec.bottom;
	}

	void Do(ExtObject* obj) {
		obj->renderer->Fill(rc, c);
	}
};

class DrawLine : public DrawCommand {
public:
	point p1, p2;
	cr::color c;
	DrawLine(float _x1, float _y1, float _x2, float _y2, const cr::color& _c)
		: p1(_x1, _x2), p2(_x2, _y2), c(_c) {}

	void Do(ExtObject* obj) {
		obj->renderer->Line(p1, p2, c);
	}
};

class DrawPoint : public DrawCommand {
public:
	point p;
	cr::color c;
	DrawPoint(float _x, float _y, const cr::color& _c)
		: p(_x, _y), c(_c) {}

	void Do(ExtObject* obj) {
		obj->renderer->Point(p, c);
	}
};

// Only include once
#endif