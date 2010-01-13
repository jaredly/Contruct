// Main.h
// Definitions and classes for the object workings

// Only include once
#ifndef MAIN_H
#define MAIN_H

// Movement object
#define IDE_FLAGS OF_MOVEMENTPLUGIN

#define HANDLE_DISTANCE 50

#include "..\..\Common\ExpReturn.hpp"
#include "..\..\Common\bin.h"

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)

double lerp(double a, double b, float f);

class ObjectBackup
{
public:
	double x,y,w,h,a;
	COLORREF filter;
	editInfo* pInfo;

	ObjectBackup(class editInfo* _pInfo);
	void Restore();
};

class KeyFrame;



class Limb
{
public:
	Limb(){
		cx = 0;
		cy = 0;
		px = 0;
		py = 0;
		w = 1;
		h = 1;
		filter = 0xffffff;
		opacity = 1;
		a = 0;

		pivot_pixelx = 0;
		pivot_pixely = 0;
		save = true;

		instanceID = -1;
		ObjectIdentifier = -1;

		mouseover = false;

#ifdef RUN_ONLY
		object = NULL;
#endif
	}
	// All values are scales (eg. an unchanged width would be 1)
	double cx; //origin from hotspot to pivot (for child)
	double cy; 
	
	double px; //origin from hotspot to pivot (for parent)
	double py;
	
	double w; //width scale
	double h;

	COLORREF filter;
	float opacity;	

	double a;

	double pivot_pixelx;
	double pivot_pixely;

	int instanceID;
	int ObjectIdentifier;
	int globalID;
	list<Limb> limbs;
	CString object_name;

	bool save;

	bool mouseover;

	void RemoveLimb(Limb* pRemove);



public:

#ifdef RUN_ONLY
	CRunObject* object;

	void Update(CRunObject* pParent, bool beforeevents, class ExtObject* behavior);
#else
	void Update(class editInfo* pParent,class EditExt* ext);
#endif

	void Serialize(bin& ar);

	void UpdateAllLimbs(vector<Limb*>& allLimbs);

	Limb* LookForParent(Limb* child);
	void RotateChildren(double dAngle);

	void SetBonesToFrame(KeyFrame* pFrame);
	void SetFrameToBones(KeyFrame* pFrame);

	void Tick(KeyFrame* pFrame, double ratio);
};

class LimbData
{
public:

	LimbData(){
		cx = 0;
		cy = 0;
		px = 0;
		py = 0;
		w = 0;
		h = 0;
		a = 0;
		filter = 0xffffff;
		opacity = 1;

	}

	double cx; //origin from hotspot to pivot (for child)
	double cy; 
	
	double px; //origin from hotspot to pivot (for parent)
	double py;
	
	double w; //width scale
	double h;

	COLORREF filter;
	float opacity;

	double a;

	void Serialize(bin& ar);
};


class KeyFrame
{
public:
	KeyFrame(){
		name = "";
		speed = 100;
	}
	map< Limb*, LimbData> limb;
	CString name;
	float speed;

	void Serialize(bin& ar, vector<Limb*>& allLimbs);


};

class KeyAnimation
{
public:
	KeyAnimation(){
		name = "";
		animSpeed = 50;
		animRepeat = true;
		animRepeatNumber = 0;
		animRepeatTo = 0;
		animPingPong = false;
	}

	vector<KeyFrame*> frames;
	CString name;



	float animSpeed;
	bool animRepeat;
	int animRepeatNumber;
	int animRepeatTo;
	bool animPingPong;

	void Serialize(bin& ar, map<int, KeyFrame>& frames);

	void RemoveAllFramesThatUseKeyframe(KeyFrame* pFrame);
};

int GetLimbIndex(Limb* pLimb, vector<Limb*>& allLimbs);
int GetFrameIndex(KeyFrame* pFrame, map<int, KeyFrame>& allframes);




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
	long		ReturnDefaultValue(LPVAL theParams,  ExpReturn& ret);
	long		ReturnUndefinedExpression(CString& expName, LPVAL theParams,  ExpReturn& ret);
	void		OnCreate();
	// Misc functions
	void		OnFrameChange(int oldFrame, int newFrame);
	void		Serialize(bin& ar);
	// Data functions
	long		GetData(int id, void* param);
	long		CallFunction(int id, void* param);

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS DEFINITIONS

	
	long eGetAnimFrame(LPVAL params, ExpReturn& ret);
	long eGetAnimName(LPVAL params, ExpReturn& ret);

	long aSetAnimFrame(LPVAL params);
	long aPlayAnimation(LPVAL params);
	long aPauseAnimation(LPVAL params);
	long aSetAnimationSpeed(LPVAL params);
	long aSetAnimation(LPVAL params);

	long cOnAnimationFinish(LPVAL params);
	long cAnimationPlaying(LPVAL params);
	long cCompareAnimFrame(LPVAL params);

	long aSetScale(LPVAL params);
	long aSetMirrored(LPVAL params);
	long aSetFlipped(LPVAL params);
	long aSetAngle(LPVAL params);
	long aForceBoneRepositioning(LPVAL params);




public:

	int keyframe;
	int animation;
	double delaytillnextframe;
	bool playing;
	bool pingpong;
	int repeatcount;


	Limb limb;
	map<int, KeyFrame> frame;
	map<int, KeyAnimation> anim;
	vector<Limb*> allLimbs;

	float scale;
	float prev_scale;

	bool flip;
	bool mirror;
	bool prev_flip;
	bool prev_mirror;
	float angle;

public: 

	void	DebuggerUpdateDisplay(class ExpStore*& pPrivateVars){}
	void	OnDebuggerValueChanged(const char* name, const char* value){}


};

//////////// EDITTIME INFO ////////////
class EditExt
{
public:
	///////////////////////////
	// Class data
	EditExt(class VEditTime* pVEditTime, class editInfo* pEInfo);
	~EditExt();

	void SetBonesToFrame(int frame);
	void SetFrameToBones(int frame);
	void Draw();
	int  GetMenuIcons(int ACEMenu);
	void Initialize();
	void OnPut();
	void OnRemoved();
	int  OnShowACEMenu(TABLE ACEType);
	BOOL OnSizeObject();
	void Serialize(bin& ar);
	void OnPropertiesUpdate();
	void OnMessage(int message);

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info
	class editInfo*  pParentInfo;		// Pointer to object edittime info

	class editInfo* pMouseOver;

	class editInfo*  m_AttachLimb;
	class editInfo*  m_AttachToLimb;

	vector<ObjectBackup> m_Backup;

	KeyFrame* GetFrame(int curframe);
	KeyAnimation* GetAnimation(int curAnim);

	void Edit();
	void CloseEdit();
	void CancelObject();
	void RemoveAnimation();
	void RemoveFrame();
	void AddObject();
	bool m_Edit;
	int m_Add;

	void BeginDraw();
	void EndDraw();

	int iFont;
	int iDot;
	POINTF dragstart;
	POINTF dragoffset;

public:
	// Data
	Limb limb;
	Limb* pSelectedLimb;
	map<int, KeyFrame> frame;
	map<int, KeyAnimation> anim;
	
	Limb* GetLimb(editInfo* pInfo);
	Limb* GetParent(Limb* pChild);
	vector<Limb*> allLimbs;
	int GetLimbIndex(Limb* pLimb);

public:
	// data only used by inline editor
	Limb* drag_handle;
	double drag_olda;

	Limb* drag_point;
	double drag_oldx;
	double drag_oldy;
	double drag_objectX;
	double drag_objectY;
	bool getObjectDragInfo;

	int curFrame;
	int curAnim;
	int comboBoxHack;
	CString editBoxHack;
	int unique_id;

public:


	void GetAnimationHandle(int texture){}
	void DrawDot(float x, float y, D3DCOLOR filter = 0xffffffff);

};
#define PI 3.1415926535897932384626433832795
// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif