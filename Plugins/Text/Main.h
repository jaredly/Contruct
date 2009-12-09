// Main.h
// Definitions and classes for the object workings

// Only include once
#ifndef MAIN_H
#define MAIN_H

#define IDE_FLAGS OF_EFFECTS | OF_PRIVATEVARIABLES | OF_MOVEMENTS

#include "..\..\Common\ExpReturn.hpp"
#include "..\..\Common\bin.h"

#include "XColorStatic.h"

#define OBJECTRECT CRect(info.x, info.y, info.x + info.w, info.y + info.h)

#define COMMONACE_POSITION
#define COMMONACE_DIMENSION
#define COMMONACE_ZORDER
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
	// Debugger functions
	void	DebuggerUpdateDisplay(class ExpStore*& pPrivateVars);
	void	OnDebuggerValueChanged(const char* name, const char* value);

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS DEFINITIONS
#include "..\..\Common\CommonAceDecl.hpp"

	long cValueCmp(LPVAL params);
	long cCompareText(LPVAL params);
	long cWriteFinished(LPVAL params);

	long aSetText      (LPVAL theParams);
	long aSetTextColour(LPVAL theParams);

	long aSetTextSize	(LPVAL theParams);
	long aSetBold		(LPVAL theParams);
	long aSetItalic		(LPVAL theParams);
	long aSetHorizAlign (LPVAL theParams);
	long aSetVertAlign  (LPVAL theParams);
	long aSetFont(LPVAL theParams);

	long aSetValue(LPVAL params);
	long aAddValue(LPVAL params);
	long aSubValue(LPVAL params);

	long aWriteText(LPVAL params);
	long aSetWriteSpeed(LPVAL params);

	long eGetText	(LPVAL theParams, ExpReturn& ret);
	long eGetValue	(LPVAL params, ExpReturn& ret);

	long eGetTextWidth	(LPVAL params, ExpReturn& ret);
	long eGetTextHeight	(LPVAL params, ExpReturn& ret);
	long eGetWriteSpeed	(LPVAL params, ExpReturn& ret);

	void UpdateFont();

	CString text;
	CString fontface;
	int pxSize;
	int bold;
	int italics;
	FontHandle iFont;
	DWORD dwFlags;

	vector<ExpStore> privateVars;

	// Application runtime
	CXColorStatic	m_Text;
	COLORREF		m_Colour;
	CRect			m_rc;

	// writing text
	CString write_text;
	int current_letter;
	float write_speed;
	float elapsed;
	bool writing;
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
	void OnDoubleClick();
	void GetAnimationHandle(int&) {}
	BOOL OnNotify(int notify);

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	CString text;
	CString fontface;
	int pxSize;
	int bold;
	int italics;
	int iFont;
	COLORREF color;
	float opacityf;

	int horizAlign;
	int vertAlign;

	bool hide_at_start;
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif