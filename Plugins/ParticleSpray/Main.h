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
#define COMMONACE_ZORDER
#define COMMONACE_ANGLE
#define COMMONACE_VISIBILITY
#define COMMONACE_OPACITY
#define COMMONACE_FILTER
#define COMMONACE_IDS
#define COMMONACE_COUNT_DESTROY

enum ParticleDestroyMode {
	PDM_LIFETIME,		// Destroy when lifetime expired
	PDM_STOPPED,		// Destroy when speed = 0
	PDM_INVISIBLE,		// Destroy when opacity = 0
	PDM_FORCE_DWORD = 0x7FFFFFFF
};

struct ParticleParameters {

	// v1
	float sprayCone;
	float particleSize;
	float particleDisplayAngle;  // v4
	COLORREF color;
	float opacity;
	float speed;
	float acc;
	int destroyMode;
	int lifeTime;
	float angleRandomiser;
	float rate;
	float xRandom;
	float yRandom;
	float daRandom;// v4
	float daTurnRandom;// v4
	float sizeRandom;// v4
	float speedRandom;
	float initialSpeedRandom; //v5
	float gravity;
	float gravityangle; // v4

	float gravdx;
	float gravdy;

	float daTurn;// v4
	float grow; // v4
	float growRandom;// v4
	COLORREF colorRandom;
	float opacityRandom;
	int fadeoutTime;
	COLORREF fadeColor;
	int fadeColorTime;

	// v2
	bool oneShot;
	bool renderAdditive;

	ParticleParameters() :
		sprayCone(60.0f),
		particleSize(3.0f),
		color(RGB(255,255,0)),
		opacity(1.0f),
		speed(300.0f),
		acc(-200.0f),
		destroyMode(PDM_STOPPED),
		lifeTime(1000),
		angleRandomiser(5.0f),
		rate(200),
		xRandom(0),
		yRandom(0),
		speedRandom(1000.0f),
		gravity(0.0f),
		colorRandom(0),
		opacityRandom(0.0f),
		fadeoutTime(1000),
		fadeColor(RGB(255,0,0)),
		fadeColorTime(1000),
		oneShot(false),

		particleDisplayAngle(0),
		daRandom(0),
		daTurnRandom(0),
		sizeRandom(0),
		gravityangle(90),
		daTurn(0),
		grow(0),
		growRandom(0),
		initialSpeedRandom(0),
		renderAdditive(false)

		{}


	void Serialize(bin& ar);
};

struct Particle {
	float x;
	float y;
	float angle;
	cr::color c;
	float opacity;
	float speed;
	float gravitySpeedx;
	float gravitySpeedy;
	int creationTime;
	float creationTimef;
	bool expired;

	float scale;
	float scale_speed;
	float display_angle;
	float display_angle_speed;

	void Update(class ExtObject* o);
};

typedef list<Particle> ParticleList;
typedef list<Particle>::iterator ParticleIterator;

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

	void OnNotify(int notify);

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

	long cValueCmp(LPVAL params);
	long cTrigger(LPVAL params);
	long cPickLowestVar(LPVAL params);
	long cPickHighestVar(LPVAL params);
	long cPickRandom(LPVAL params);
	long cOn(LPVAL params);

	long aSetValue(LPVAL params);
	long aRotateToObject(LPVAL params);
	long aRotateToPosition(LPVAL params);
	long aAddValue(LPVAL params);
	long aSubValue(LPVAL params);

	long aSetSprayCone(LPVAL params);
	long aSetParticleSize(LPVAL params);
	long aSetSpeed(LPVAL params);
	long aSetAcc(LPVAL params);
	long aSetLifetime(LPVAL params);
	long aSetAngleRandomiser(LPVAL params);
	long aSetRate(LPVAL params);
	long aSetXRandom(LPVAL params);
	long aSetYRandom(LPVAL params);
	long aSetSpeedSimulationRandom(LPVAL params);
	long aSetGravity(LPVAL params);
	long aSetOn(LPVAL params);
	long aSetFadeoutTime(LPVAL params);

	// New actions

	long aSetOneShot(LPVAL params);
	long aSetUseTexture(LPVAL params);
	long aSetRenderAdditive(LPVAL params);
	long aSetDisplayAngle(LPVAL params);
	long aSetSizeRandomiser(LPVAL params);
	long aSetDisplayAngleRandomiser(LPVAL params);
	long aSetGrowRandomiser(LPVAL params);
	long aSetDisplayAngleTurnRandomiser(LPVAL params);
	long aSetGravityAngle(LPVAL params);
	long aSetGrow(LPVAL params);
	long aSetDisplayAngleTurnSpeed(LPVAL params);
	long aSetOpacityRandomiser(LPVAL params);
	long aSetDestroyMode(LPVAL params);
	long aSetColor(LPVAL params);
	long aSetFadeToColor(LPVAL params);
	long aSetOpacity(LPVAL params);
	long aSetFadeColorTime(LPVAL params);
	long aSetSpeedRandom(LPVAL params);

	long eGetValue(LPVAL params, ExpReturn& ret);
	long eNumPVs(LPVAL params, ExpReturn& ret);
	long eNumParticles(LPVAL params, ExpReturn& ret);
	long eSprayCone(LPVAL params, ExpReturn& ret);
	long eParticleSize(LPVAL params, ExpReturn& ret);
	long eSpeed(LPVAL params, ExpReturn& ret);
	long eAcc(LPVAL params, ExpReturn& ret);
	long eLifetime(LPVAL params, ExpReturn& ret);
	long eAngleRandomiser(LPVAL params, ExpReturn& ret);
	long eRate(LPVAL params, ExpReturn& ret);
	long eXRandom(LPVAL params, ExpReturn& ret);
	long eYRandom(LPVAL params, ExpReturn& ret);
	long eSpeedSimulationRandom(LPVAL params, ExpReturn& ret);
	long eGravity(LPVAL params, ExpReturn& ret);
	long eOpacityRandom(LPVAL params, ExpReturn& ret);
	long eFadeoutTime(LPVAL params, ExpReturn& ret);
	long eFadeColorTime(LPVAL params, ExpReturn& ret);

	// New Expressions
	long eOneShot(LPVAL params, ExpReturn& ret);
	long eUseTexture(LPVAL params, ExpReturn& ret);
	long eRenderAdditive(LPVAL params, ExpReturn& ret);
	long eDisplayAngle(LPVAL params, ExpReturn& ret);
	long eOpacity(LPVAL params, ExpReturn& ret);
	long eSizeRandomiser(LPVAL params, ExpReturn& ret);
	long eDisplayAngleRandomiser(LPVAL params, ExpReturn& ret);
	long eSpeedRandomiser(LPVAL params, ExpReturn& ret);
	long eGrowRandomiser(LPVAL params, ExpReturn& ret);
	long eDisplayAngleTurnRandomiser(LPVAL params, ExpReturn& ret);
	long eGravityAngle(LPVAL params, ExpReturn& ret);
	long eGrow(LPVAL params, ExpReturn& ret);
	long eDisplayAngleTurn(LPVAL params, ExpReturn& ret);
	long eDestroyMode(LPVAL params, ExpReturn& ret);
	long eColor(LPVAL params, ExpReturn& ret);
	long eFadeToColor(LPVAL params, ExpReturn& ret);

	////////////////////////////////////////////////////
	// Private vars
	vector<ExpStore> privateVars;

	RECTF box;

	ParticleParameters pp;
	float sprayAngle;
	bool on;

	cr::color col;
	cr::color col_random;
	cr::color col_fade;

	ParticleList particles;

	clock_t oldClock;
	clock_t curClock;
	float curClockf;

	float timeDelta;
	float delayNextParticle;
	float fadeTimef;
	float fadeColorTimef;

	bool useTexture;
	TextureHandle th;

	bool oneShotHasFired;

	bool renderWithPoints;

	float lastX;
	float lastY;
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
	void GetAnimationHandle(int& handle) {}

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	///////////////////////////
	// Your edittime extension data goes here
	ParticleParameters pp;
	int imgHandle;
	bool useTexture;

	void BtnEditTexture();
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif