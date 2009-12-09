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
#define IDE_FLAGS 	OF_MOVEMENTS | OF_EFFECTS | OF_PRIVATEVARIABLES | OF_ALLOWANGLES | OF_DEFAULTVALUE_PRIVATEVAR | OF_SINGLEDRAWOP

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
#define COMMONACE_Z_ELEVATION

// For distortion maps
class DistortInfo {
public:
	float x;
	float y;
	float u;
	float v;
	cr::color filter;
	float z;
	
	DistortInfo(){
		x = 0;
		y = 0;
		u = 0;
		v = 0;
		z = 0;
		filter = cr::color(1.0, 1.0, 1.0, 1.0);
	}

	void Serialize(bin& ar);
};

int FindAFrame(VEditTime* pEditTime, int anim, CString startAnim, int index);

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

	// Renderer
	IRenderer* const renderer;

	void OnNotify(int notify);
	void RenderDistorted();

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
	long		ReturnDefaultValue(LPVAL params, ExpReturn& ret);
	long		ReturnUndefinedExpression(CString& expName, LPVAL params, ExpReturn& ret);
	void		OnCreate();
	// Misc functions
	void		OnFrameChange(int oldFrame, int newFrame);
	void		Serialize(bin& ar);
	// Data functions
	long		GetData(int id, void* param);
	long		CallFunction(int id, void* param);
	// Debugging
	void	DebuggerUpdateDisplay(ExpStore*& pPrivateVars);
	void	OnDebuggerValueChanged(const char* name, const char* value);

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS DEFINITIONS
#include "..\..\Common\CommonAceDecl.hpp"

	long cOverlap(LPVAL params);
	long cOverlapAdvanced(LPVAL params);
	long cOnCollide(LPVAL params);
	long cValueCmp(LPVAL params);
	long cTrigger(LPVAL params);
	long cPickLowestVar(LPVAL params);
	long cPickHighestVar(LPVAL params);
	long cPickRandom(LPVAL params);
	long cOnAnimationFinish(LPVAL params);
	long cAnimationPlaying(LPVAL params);
	long cCompareAnimFrame(LPVAL params);
	long cPickClosest(LPVAL params);
	long cPickFarthest(LPVAL params);
	long cOnFrameChange(LPVAL params);

	long aSetValue(LPVAL params);
	long aShootObject(LPVAL params);
	long aRotateToObject(LPVAL params);
	long aRotateToPosition(LPVAL params);

	long aAddValue(LPVAL params);
	long aSubValue(LPVAL params);
	long aSetAnimation(LPVAL params);
	long aPlayAnimation(LPVAL params);
	long aPauseAnimation(LPVAL params);
	long aResumeAnimation(LPVAL params);
	long aSetAnimationSpeed(LPVAL params);
	long aFlash(LPVAL params);
	long aSetCollMode(LPVAL params);
	long aLoadFrameFromFile(LPVAL params);
	long aSetFramePivotPoint(LPVAL params);
	long aSetAnimFrame(LPVAL params);
	long aSetVertex(LPVAL params);
	long aSetSkewX(LPVAL params);
	long aSetSkewY(LPVAL params);

	long aSetDistortMapSize(LPVAL theParams);
	long aSetDisplacementAt(LPVAL theParams);
	long aSetDisplacementRealAt(LPVAL theParams);

	long aSetAnimationAngle(LPVAL theParams);

	long aSaveMesh(LPVAL theParams);
	long aLoadMesh(LPVAL theParams);



	long eGetValue(LPVAL params, ExpReturn& ret);
	long eOriginalWidth(LPVAL params, ExpReturn& ret);
	long eOriginalHeight(LPVAL params, ExpReturn& ret);
	long eGetImagePointX(LPVAL params, ExpReturn& ret);
	long eGetImagePointY(LPVAL params, ExpReturn& ret);
	long eNumPVs(LPVAL params, ExpReturn& ret);
	long eSkewX(LPVAL params, ExpReturn& ret);
	long eSkewY(LPVAL params, ExpReturn& ret);

	long eGetAnimFrame(LPVAL params, ExpReturn& ret);
	long eGetAnimName(LPVAL params, ExpReturn& ret);
	long eGetFrameCount(LPVAL params, ExpReturn& ret);
	long eGetAnimSpeed(LPVAL params, ExpReturn& ret);

	long eGetMeshCols(LPVAL params, ExpReturn& ret);
	long eGetMeshRows(LPVAL params, ExpReturn& ret);

	long eGetMeshX(LPVAL params, ExpReturn& ret);
	long eGetMeshY(LPVAL params, ExpReturn& ret);
	long eGetMeshZ(LPVAL params, ExpReturn& ret);
	long eGetMeshU(LPVAL params, ExpReturn& ret);
	long eGetMeshV(LPVAL params, ExpReturn& ret);
	long eGetMeshFilter(LPVAL params, ExpReturn& ret);
	long eGetMeshOpacity(LPVAL params, ExpReturn& ret);

	////////////////////////////////////////////////////
	// Private vars
	vector<ExpStore> privateVars;

	// For triggering "On collision" - remembering previous state
	map<void*, set<CRunObject*> > collisionMemory;

	bool NeedsUpdating;

	// Flash counters
	double flashTime;		// Time left to flash (counts down to 0)
	double flashCounter;	// Increments to flashInterval
	double flashInterval;	// Seconds between flashes

	// Current animation
	int m_AnimRoot;

	int autoMirror;
	int autoFlip;
	int autoRotations;

	bool vertices_changed;
	cr::color vertexcolors[4];

	AnimInfo m_Animation;

	void UpdateCoords();

	vector< vector< DistortInfo > > distort;

	CRunAnimation* pRoot;
	bool staticAnims;		// one animation, one angle, one frame: no need to tick animation
	bool locked_animation_angles;
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
	BOOL OnNotify(int notify);
	void GetAnimationHandle(int& handle);

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	///////////////////////////
	// Your edittime extension data goes here
	int iTexture;		// DX texture
	int collMode;


	int autoMirror;
	int autoFlip;
	int autoRotations;
	int autoRotationsCombo;
	bool hideAtStart;

	CString startAnim;
	int startFrame;

	// Animations
	int m_iAnimationRoot;

	void UpdateIcons();
	void BtnMakeOriginalSize();

	SIZE imgsize; // used so if we edit a frame we know what size it used to be
	float skewX, skewY;

	bool locked_animation_angles;
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif