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
//OF_MOVEMENTEXTENSION - Is a movement extension
//OF_MOVEMENTS - Allow movement extensions to be added
//OF_EFFECTS - Allow display effects
//OF_NOCOMMONDEBUG - Don't show 'Common' field (X/Y/W/H/Angle/Opacity) in debugger
//OF_NODEBUG - Don't show at all in debugger
//OF_UNDEFINEDEXPRESSIONS - Allow ReturnUndefinedExpression to be called
#define IDE_FLAGS 	OF_NOCOMMONDEBUG

#include "..\..\Common\ExpReturn.hpp"

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)



class CTile
{
public:
	int index;
	CTile()
	{
		index = -1;
	}
};


//////////// RUNTIME OBJECT ////////////
// Add any member functions or data you want to this class.
// Your extension inherits CRunObject.  See the definition
// for the default member values you have access to.
class ExtObject : public CRunObject
{
public:
	// Constructor
	ExtObject(initialObject* editObject, VRuntime* pVRuntime);
	// Destructor (called when Construct destroys the object)
	~ExtObject();

	//////////////////////////
	// OnFrame: called once per frame just before the event list
	// OnFrame2: called once per frame just after the event list
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
	// ACTIONS, CONDITIONS AND EXPRESSIONS
#include "..\..\Common\CommonAceDecl.hpp"

	long cMyCondition(LPVAL params);

	long aMyAction(LPVAL params);

	long eMyExpression(LPVAL params, ExpReturn& ret);
	cr::point GetTileSize();

	vector< vector<CTile> > tiles;

	map<CMapPoint,int, less_point> m_vt_sel;
	int m_vt_sel_x;
	int m_vt_sel_y;
	bool m_vt_IsSel;
	bool m_vt_IsSelBox;
	bool m_vt_AddSel;

	map<CMapPoint, int, less_point> m_sel;
	int m_sel_x;
	int m_sel_y;
	bool m_IsSel;
	bool m_IsSelBox;
	bool m_AddSel;
	bool m_SelectingFromBoard;

	map<CMapPoint, int, less_point> m_ghost;

	int m_iAnimationRoot;
	bool m_EditMode;
	bool m_ViewTiles;



	map<int, CollisionMask*> collision;
	map<int, TextureHandle> textures;

	float tileWidth;
	float tileHeight;

	////////////////////////////////////////////////////
	// Data

	// Use when private variables (OF_PRIVATEVARIABLES) are enabled.
	//vector<ExpStore> privateVars;
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

	void BtnEditMode();
	void GetAnimationHandle(int& anim);
	void OnMessage(int message);

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	///////////////////////////
	// Your edittime extension data goes here
	void GetViewTileInfo(float* _xoffset = NULL, float* _yoffset = NULL, int* _tilecount = NULL, int* _tilecols = NULL, int* _tilerows = NULL);
	cr::point GetTileSize();
	void MouseToBoard(CPoint& mouse);
	void MouseToViewTile(CPoint& mouse);
	int GetViewTileAt(CPoint xy);
	void SetTileGhost(map<CMapPoint,int, less_point>& sel);

	int GetTileImage(int index);

	vector< vector<CTile> > tiles;
	int GetTile(int x, int y);
	int SetTile(int x, int y, int index);
	void ResizeBoardToObjectSize();
	map<CMapPoint,int, less_point> m_vt_sel;
	int m_vt_sel_x;
	int m_vt_sel_y;
	bool m_vt_IsSel;
	bool m_vt_IsSelBox;
	bool m_vt_AddSel;

	map<CMapPoint, int, less_point> m_sel;
	int m_sel_x;
	int m_sel_y;
	bool m_IsSel;
	bool m_IsSelBox;
	bool m_AddSel;
	bool m_SelectingFromBoard;

	map<CMapPoint, int, less_point> m_ghost;

	int m_iAnimationRoot;
	bool m_EditMode;
	bool m_ViewTiles;

	bool LeftMouse;

	// Edittime data
	float tileWidth;
	float tileHeight;
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif