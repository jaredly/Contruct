// Main.h
// Definitions and classes for the object workings

// Only include once
#ifndef MAIN_H
#define MAIN_H

#define MINIMUM_VERSION 1
#define IDE_FLAGS OF_PRIVATEVARIABLES | OF_DEFAULTVALUE_PRIVATEVAR
#include "..\..\Common\ExpReturn.hpp"
#include "..\..\Common\bin.h"

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)

#define COMMONACE_POSITION
#define COMMONACE_DIMENSION
#define COMMONACE_VISIBILITY
#define COMMONACE_IDS
#define COMMONACE_COUNT_DESTROY
#define COMMONACE_PRIVATE_VARIABLES


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
	// Debugging
	void		DebuggerUpdateDisplay(ExpStore*& pPrivateVars);
	void		OnDebuggerValueChanged(const char* name, const char* value);

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS DEFINITIONS
	#include "..\..\Common\CommonAceDecl.hpp"

	// Conditions
	long cOnChanged(LPVAL theParams);
	long cCompareText(LPVAL params);
	long cOnMaxText(LPVAL theParams);

	// Actions
	long aSetText			(LPVAL theParams);
	long aFocusOn			(LPVAL theParams);
	long aFocusOff			(LPVAL theParams);
	long aEnable			(LPVAL theParams);
	long aDisable			(LPVAL theParams);
	long aAppendText		(LPVAL theParams);
	long aClearSelection	(LPVAL theParams);
	long aSelectAll			(LPVAL theParams);
	long aSetSelection		(LPVAL theParams);
	long aCut				(LPVAL theParams);
	long aCopy				(LPVAL theParams);
	long aPaste				(LPVAL theParams);
	long aUndo				(LPVAL theParams);
	long aClearUndoBuffer	(LPVAL theParams);
	long aLimitText			(LPVAL theParams);
	long aReplaceSelection	(LPVAL theParams);
	long aLoadFile			(LPVAL theParams);	
	long aSaveFile			(LPVAL theParams);	
	long aSetReadOnly		(LPVAL theParams);	
	long aDestroy			(LPVAL theParams);	
	long aSetVisible		(LPVAL theParams);	

	// Expressions
	long eGetText			(LPVAL theParams, ExpReturn& ret);
	long eGetLineCount		(LPVAL theParams, ExpReturn& ret);
	long eGetTextLimit		(LPVAL theParams, ExpReturn& ret);
	long eGetSelectionStart	(LPVAL theParams, ExpReturn& ret);
	long eGetSelectionEnd	(LPVAL theParams, ExpReturn& ret);
	long eGetSelection		(LPVAL theParams, ExpReturn& ret);

	//ExpStore cmn_xyRet[2];	// For .xy returns

	//long ExtObject::cmn_eGetXY(LPVAL params, ExpReturn &ret);

	///////////////////////////
	// You can add any data or classes your object will need
	CEdit edit;
	CFont font;
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
	void GetAnimationHandle(int) {}

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	///////////////////////////
	// Your edittime extension data goes here
	CString text;
	int autoHscroll, autoVscroll;
	int password;
	int enabled;

	int multiline;
	int vscroll, hscroll;

	int m_Lowercase;
	int m_Uppercase;

	int eFont;

	int border;
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif