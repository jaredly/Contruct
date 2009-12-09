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
#define IDE_FLAGS 	OF_EFFECTS | OF_MOVEMENTS | OF_PRIVATEVARIABLES | OF_ALLOWANGLES

#include "..\..\Common\ExpReturn.hpp"

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)


const DWORD D3DFVF_TLVERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
struct TLVERTEX
{
    float x;
    float y;
    float z;
    D3DCOLOR colour;
    float u;
    float v;
};

extern TLVERTEX g_cube_vertices[];
/*
extern TLVERTEX cube_vertices[];
extern int cube_indexes[];
*/

struct xyz {
	float x, y, z;
};
struct uv {
	float u, v;
};

struct xyzuv {
	xyz _xyz;
	uv _uv;
};
extern xyzuv cube_vertices[];

#define NUMCUBEVERTICES 36

#define FRONT_FACE		0
#define BACK_FACE		1
#define TOP_FACE		2
#define BOTTOM_FACE		3
#define LEFT_FACE		4
#define RIGHT_FACE		5

// Common ACES
#define COMMONACE_POSITION
#define COMMONACE_DIMENSION
#define COMMONACE_ZORDER
#define COMMONACE_VISIBILITY
#define COMMONACE_COUNT_DESTROY

// For testing if an object has changed
struct ObjectDetails {
	float x, y, w, h, angle, scrollX, scrollY, yaw, pitch;

	void SetFrom(class ExtObject* obj);

	bool operator!=(const ObjectDetails& r) const {
		return x != r.x || y != r.y || w != r.w || h != r.h || angle != r.angle || scrollX != r.scrollX
			|| scrollY != r.scrollY || yaw != r.yaw || pitch != r.pitch;
	}
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

	long cCompareZ(LPVAL params);
	long cCompareDepth(LPVAL params);
	long cValueCmp(LPVAL params);

	long aSetZ(LPVAL params);
	long aSetDepth(LPVAL params);
	long aSetYaw(LPVAL params);
	long aSetPitch(LPVAL params);
	long aSetRoll(LPVAL params);
	long aSetValue(LPVAL params);
	long aSubValue(LPVAL params);
	long aAddValue(LPVAL params);

	long eGetZ(LPVAL params, ExpReturn& ret);
	long eGetDepth(LPVAL params, ExpReturn& ret);
	long eGetYaw(LPVAL params, ExpReturn& ret);
	long eGetPitch(LPVAL params, ExpReturn& ret);
	long eGetRoll(LPVAL params, ExpReturn& ret);
	long eGetValue(LPVAL params, ExpReturn& ret);

	void UpdateAngle();

	////////////////////////////////////////////////////
	// Data members
	RECTF displayBox;		// Custom display bounding box

	// Used when private variables (OF_PRIVATEVARIABLES) are enabled.
	vector<ExpStore> privateVars;

	// Device + vertices for 3D box
	//IDirect3DDevice9* pDevice;
	//IDirect3DVertexBuffer9* pVertices;
	TextureHandle textures[6];			// 6 texture handles, for each face

	HRESULT hr;

	// Coordinate conversion
	//void ProjectScreenToWorld(D3DXVECTOR3* pOut, float screenX, float screenY, float worldZ);
	//POINTF ProjectWorldToScreen(float x, float y, float z);

	//void UpdateVertices();
	//TLVERTEX currentVertices[NUMCUBEVERTICES];

	float worldPxX;		// World units per pixel X
	float worldPxY;		// World units per pixel Y
	bool drawInit;
	float depth;
	float z;
	double yaw;
	double pitch;

	/*
	// Z buffer
	IDirect3DSurface9* pDepthStencil;

	// Matrices & vectors
	D3DXMATRIX view_matrix;
	D3DXMATRIX projection_matrix;
	D3DXVECTOR3 eye_vector;
	D3DXVECTOR3 lookat_vector;
	D3DXVECTOR3 up_vector;
	float aspect;
	D3DXVECTOR3 objectSpace;
	D3DXMATRIX worldMatrix;
	D3DVIEWPORT9 viewport;
	D3DXPLANE plane;
	D3DXVECTOR3 orig;
	D3DXVECTOR3 normal;
	*/
	D3DXMATRIX transMatrix;
	D3DXMATRIX rotMatrix;
	D3DXMATRIX scaleMatrix;

	float oldW;
	float oldH;
	float oldAngle;

	bool zbuffer;

	ObjectDetails oldDetails;
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

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	///////////////////////////
	// Your edittime extension data goes here
	int iTexture;

	// Handles to the surface images & textures
	int imageHandles[6];
	Texture textures[6];

	float height;
	float z;

	void EditFace(int face);

	IDirect3DDevice9* pDevice;
	IDirect3DVertexBuffer9* pVertices;
	IDirect3DSurface9* pDepthStencil;

	HRESULT hr;

	void ProjectScreenToWorld(D3DXVECTOR3* pOut, float screenX, float screenY, float worldZ);
	void UpdateVertices();
	TLVERTEX currentVertices[NUMCUBEVERTICES];

	float worldPxX;		// World units per pixel X
	float worldPxY;		// World units per pixel Y
	bool drawInit;

	D3DXMATRIX view_matrix;
	D3DXMATRIX projection_matrix;
	D3DXVECTOR3 eye_vector;
	D3DXVECTOR3 lookat_vector;
	D3DXVECTOR3 up_vector;
	float aspect;
	D3DXMATRIX worldMatrix;
	D3DVIEWPORT9 viewport;
	D3DXPLANE plane;
	D3DXVECTOR3 orig;
	D3DXVECTOR3 normal;

	D3DXMATRIX transMatrix;
	D3DXMATRIX rotMatrix;
	D3DXMATRIX scaleMatrix;

	bool zbuffer;

	void D3DInit();
	void UpdateTextures();
	void CreateZBuffer();
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif