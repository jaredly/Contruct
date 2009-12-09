
//////////////////////////////////////////////
//	CapReader
//
//	Reads the CAP file format (Construct Application)

// CAP eventlist tokens
#define CAP_BEGINEVENTLIST ((BYTE)1)
#define CAP_BEGINEVENT ((BYTE)2)
#define CAP_BEGINCONDITIONS ((BYTE)3)
#define CAP_BEGINCONDITION ((BYTE)4)
#define CAP_ENDCONDITION ((BYTE)5)
#define CAP_ENDCONDITIONS ((BYTE)6)
#define CAP_BEGINACTIONS ((BYTE)7)
#define CAP_BEGINACTION ((BYTE)8)
#define CAP_ENDACTION ((BYTE)9)
#define CAP_ENDACTIONS ((BYTE)10)
#define CAP_BEGINPARAM ((BYTE)11)
#define CAP_ENDPARAM ((BYTE)12)
#define CAP_ENDEVENT ((BYTE)13)
#define CAP_ENDEVENTLIST ((BYTE)14)
#define CAP_BEGINGROUP ((BYTE)15)
#define CAP_ENDGROUP ((BYTE)16)

// editTime object
#ifndef INITIALOBJECT_DEFINED
#define INITIALOBJECT_DEFINED
class initialObject
{
public:
	long    eX,
		    eY,
		    eWidth,
		    eHeight;
	float   eAngle;
	
	DWORD   pluginID;
	long    instanceID,
		    objectID;


	BYTE* eData;
	long eSize;

	cr::color eColor;

	int		layer;
	
	vector<CString> initVars;	// initial private variables

	void Free() {
		if (eData) {
			delete[] eData;
			eData = NULL;
		}
	}

	bool disabled;

	initialObject() {
		disabled = false;
	}


};
#endif

// Helper class: use an OID and a movement DLL as a key for map<>
class MovementMapper {
public:
	int oid;
	int movDll;

	MovementMapper(int _oid, int _movDll);
	bool operator<(const MovementMapper& mm) const;	// for map<>
};

class DelayedFamilyMovementAddition {
public:
	MovementMapper key;
	CRunObjType* pFMType;

	DelayedFamilyMovementAddition(int _oid, int _movDll, CRunObjType* _fmType)
		: key(_oid, _movDll), pFMType(_fmType) {}
};

// App properties
struct AppProperties {
	CString appTitle;
	int winWidth;
	int winHeight;
	int	UseMenu;
	cr::framerate_mode fpsMode;
	int fps;
	bool fullscreen;
	int sampler;
	bool screensaver;
};

///////////////////////////
// CapReader class
class CCapReader {
public:
	CCapReader() { previousEvent = NULL; }

	// Parse app properties
	void			ReadAppProperties(BYTE* _pData, int _len, AppProperties& props);
	void			ReadPythonResources(BYTE* _pData, int _len);

	// Parse a binary event list in to tree form
	void			ReadEventList(BYTE* _pData, int _len);
	void			SendEvent(CEventLine* ev);
	void			ParseAndSendEvent();

	// Parse a binary frame block and load
	void			ReadFrameData(BYTE* _pData, int _len);

	// Parse menus
	void			ReadMenuResources(BYTE* _pData, int _len, vector<HMENU>& menus);
	void			BuildMenu(HMENU Menu, bin& ar);

	// Read PNG image files to textures in CD3DDisplay
#ifndef APPRUNTIME
	void			ReadImageData(BYTE* _pData, int _len, ImageMap& imageMap, bool texturesOnly = false);
	void			ReadHLSLData(BYTE* _pData, int _len);
#ifdef CONSTRUCT_DIRECTX9
	bool			ParseEffects(EffectList& effects, CRuntime* pRuntime, int numFX);
#endif
	void			ParseAnimations(Animations& animList, bin& ar);
#else				// App runtime
	void			ReadImageData(BYTE* _pData, int _len, map<int, HBITMAP>& imageBmps);
#endif

	friend class	CRuntime;
	class CRuntime* pRuntime;

	// Additional events that need to be added, eg. spawned by OR conditions
	vector<CEventLine*>	auxEvents;

private:
	// Internal parsers for ReadEventList
	CEventLine*		ParseEventLine();
	CEventLine*		ParseEvent();
	CEventGroup*	ParseEventGroup();
	bool			IsElseEvent(CEventLine* ev);
	bool			CheckForElseEvent(CEventLine* ev);
	CEventLine**	previousEvent;		// for Else shuffling

	bool			SliceAndDiceOrConditions(ConditionList& cnds, ActionList& acts);

	short			GetConditions(vector<condition>& cndList);
	short			GetActions(vector<action>& actList);
	void			GetParams(ExpressionList& expList, int oid);

	CRunLayout*		pLayout;		// Current parsing frame

	map<MovementMapper, int> movementMap;

	void			TranslateAction(action& a, int movementID);

	UINT ps_major;
	UINT ps_minor;

	UINT max_ps_major;
	UINT max_ps_minor;

	int runWithoutPs;

	// Internal data holders
	BYTE* pData;
	int len;
	int cursor;
};