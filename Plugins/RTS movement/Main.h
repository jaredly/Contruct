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
#define IDE_FLAGS 	OF_MOVEMENTPLUGIN

#include "..\..\Common\ExpReturn.hpp"

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)

class RTSParameters {
public:
	RTSParameters();

	float av;		// angular velocity (turn speed) degrees/sec
	float acc;
	float dec;
	float maxspeed;

	int pathfinding;	// off, around solid, custom
	bool rotate;
	float blockedspeed;
	int cellsize;

	void Serialize(bin& ar);
};

class LayoutPfMap {
public:
	LayoutPfMap();

	cell_t* pfMap[10];
	int instanceCount[10];
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

	long cIsMoving		(LPVAL theParams);
	long cTrigger		(LPVAL theParams);

	long aStop			(LPVAL theParams);
	long aSetSpeed		(LPVAL theParams);
	long aSetMaxSpeed	(LPVAL theParams);
	long aSetAcc		(LPVAL theParams);
	long aSetDec		(LPVAL theParams);
	long aMoveToXY		(LPVAL theParams);
	long aMoveToObj		(LPVAL theParams);
	long aMoveToMouse	(LPVAL theParams);
	long aSetObjToAvoid (LPVAL theParams);
	long aSetMAngle		(LPVAL theParams);
	long aWaypointXY	(LPVAL theParams);
	long aWaypointObj	(LPVAL theParams);
	long aWaypointMouse (LPVAL theParams);
	long aSetActivated(LPVAL theParams);
	long aAddPfCollideType(LPVAL theParams);
	long aRegenerateObstaclesMap(LPVAL theParams);

	long eSpeed(LPVAL theParams, ExpReturn& ret);
	long eMaxSpeed(LPVAL theParams, ExpReturn& ret);
	long eAccRate(LPVAL theParams, ExpReturn& ret);
	long eDecRate(LPVAL theParams, ExpReturn& ret);
	long eMovAngle(LPVAL theParams, ExpReturn& ret);
	long eVectorX(LPVAL theParams, ExpReturn& ret);
	long eVectorY(LPVAL theParams, ExpReturn& ret);
	long eTargetX(LPVAL theParams, ExpReturn& ret);
	long eTargetY(LPVAL theParams, ExpReturn& ret);

	////////////////////////////////////////////////////
	// Data members

	// Use when private variables (OF_PRIVATEVARIABLES) are enabled.
	RTSParameters params;
	bool activated;

	bool  moving;
	float speed;
	float mAngle;	// Angle of motion (RADIANS)
	bool quickStop;

	float targetX;
	float targetY;

	ObjTypeList typesAvoid;
	void* pLastEvent;
	bool refreshGridNextTick;

	// Pathfinding cell dimensions
	int pfcellW, pfcellH, pfNumHCells, pfNumVCells;

	bool QueryCellCollision(int cellX, int cellY);
	bool CalculatePath(int targX, int targY, bool bClearWaypoints = false);

	ObjTypeList pfCollideTypes;	// Types for pathfinding to navigate around
	list<CPoint> waypoints;

	CPathfinder pathfinder;

	bool PathfindingSolidCollision();
	void PathfindingMoveOutOfSolids();
	bool PathfindingIsOverlapping();
	bool PathfindingIsOverlappingUnits();
	bool CellIsImpassable(int x, int y);
	bool MapValid();
	void IncrementCell(float x, float y);
	void UpdateAvoidanceMap();
	void GeneratePFMap();

	// Delayed pathfinding command
	struct {
		bool moveCommand;
		bool waypointCommand;
		float targetX;
		float targetY;
	} delayed;

	// Tick of last avoidance update
	static __int64 lastAvoidanceUpdate;

	float speedRet;	// for returning speed
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
	RTSParameters params;
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif