// Main.h
// Definitions and classes for the object workings

// Only include once
#ifndef MAIN_H
#define MAIN_H

#define MINIMUM_VERSION	1
#define IDE_FLAGS 	OF_MOVEMENTS | OF_EFFECTS | OF_PRIVATEVARIABLES | OF_ALLOWANGLES | OF_DEFAULTVALUE_PRIVATEVAR
#include "..\..\Common\ExpReturn.hpp"

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
	long		ReturnDefaultValue(LPVAL theParams, ExpReturn& ret);
	long		ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret);
	void		OnCreate();
	// Misc functions
	void		OnFrameChange(int oldFrame, int newFrame);
	void		Serialize(bin& ar);
	// Data functions
	long		GetData(int id, void* param);
	long		CallFunction(int id, void* param);

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS DEFINITIONS
	long aSetPlasmaXSpeed(LPVAL theParams);
	long aSetPlasmaYSpeed(LPVAL theParams);
	long aSetPlasmaSpeed(LPVAL theParams);
	long aSetPlasmaAngle(LPVAL theParams);
	long aSetPatternXSpeed(LPVAL theParams);
	long aSetPatternYSpeed(LPVAL theParams);
	long aSetPatternSpeed(LPVAL theParams);
	long aSetPatternMoveAngle(LPVAL theParams);
	long aSetPatternScale(LPVAL theParams);
	long aSetPatternAngle(LPVAL theParams);
	long aPasteObject(LPVAL theParams);

	COLORREF render_filter[6];
	float render_strength[6];
	float render_cutoff[6];

	bool visible;		// Is Visible?

	ImageHandleInfo* image1;
	ImageHandleInfo* image2;

	// For XY ret
	ExpStore xyRet[2];

	//Quad q;

	TextureHandle iSource;
	TextureHandle iSurface;
	TextureHandle iSurface2;
	TextureHandle iCold;

	cr::color coldfilter;
	float coldx;
	float coldy;
	float coldxspeed;
	float coldyspeed;
	float coldscale;
	float coldangle;

	float xspeed;
	float yspeed;

	void	DebuggerUpdateDisplay(class ExpStore*& pPrivateVars){}
	void	OnDebuggerValueChanged(const char* name, const char* value){}

	#include "..\..\Common\CommonAceDecl.hpp"
};

//////////// EDITTIME INFO ////////////
class EditExt
{
public:

	///////////////////////////
	// Class data
	EditExt(class VEditTime* pVEditTime, class editInfo* pEInfo);
	~EditExt();
	void GetAnimationHandle(int anim){};

	void Draw();
	int  GetMenuIcons(int ACEMenu);
	void Initialize();
	void OnPut();
	void OnRemoved();
	int  OnShowACEMenu(TABLE ACEType);
	BOOL OnSizeObject();
	void Serialize(bin& ar);
	void OnPropertiesUpdate();

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info


	void BtnEditImage(int id);
	///////////////////////////
	// Your edittime extension data goes here

	COLORREF cold_filter;
	float cold_opacityf;

	int m_cold;
	int m_source;

	float coldx;
	float coldy;
	float coldxspeed;
	float coldyspeed;
	float coldscale;
	float coldangle;

	float xspeed;
	float yspeed;

	//////////////////

	COLORREF render_filter[6];
	float render_strength[6];
	float render_cutoff[6];
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif