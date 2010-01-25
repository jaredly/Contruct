// Main.h
// Definitions and classes for the object workings

// Only include once
#ifndef MAIN_H
#define MAIN_H

#define IDE_FLAGS OF_EFFECTS | OF_MOVEMENTS | OF_PRIVATEVARIABLES
#include "..\..\Common\ExpReturn.hpp"
#include "..\..\Common\bin.h"

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)

#define COMMONACE_POSITION
#define COMMONACE_DIMENSION
#define COMMONACE_VISIBILITY
#define COMMONACE_FILTER
#define COMMONACE_OPACITY
#define COMMONACE_ZORDER
#define COMMONACE_COUNT_DESTROY
#define COMMONACE_Z_ELEVATION

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

	// Debugger (no debug properties)
	void DebuggerUpdateDisplay(ExpStore*& pPrivateVars);
	void OnDebuggerValueChanged(const char* name, const char* value);

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS DEFINITIONS

	long cValueCmp(LPVAL params);
	long cPickLowestVar(LPVAL params);
	long cPickHighestVar(LPVAL params);
	long cPickRandom(LPVAL params);

	long aLoadTextureFromFile(LPVAL params);
	long aSetImageOffset(LPVAL theParams);
	long aSetImageScale(LPVAL theParams);
	long aSetImageAngle(LPVAL theParams);

	long aSetValue(LPVAL params);
	long aAddValue(LPVAL params);
	long aSubValue(LPVAL params);

	long eGetValue(LPVAL params, ExpReturn& ret);
	long eNumPVs(LPVAL params, ExpReturn& ret);

#include "..\..\Common\CommonAceDecl.hpp"

	TextureHandle th;
	ExpStore xyRet[2];

	float iAngle;
	POINTF iScale;
	POINTF iOffset;

	vector<ExpStore> privateVars;

	bool has_file_loaded;
	CString filename;
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
	BOOL OnNotify(int notify);

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	void BtnEditImage();

	int iTexture;
	int imgTexture;


	//double opacity;
	//COLORREF filter;
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif