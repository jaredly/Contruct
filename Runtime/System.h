

///////////////////////////
// SYSTEM OBJECT DEFINITION
#ifndef dif
#define dif(a,b) max(a,b) - min(a,b)
#endif

// Convert radians to degrees
#ifndef DEGREES
#define DEGREES(r) ((r) * 57.295779513082320876798154814105)
#endif
// Degrees to radians
#ifndef RADIANS
#define RADIANS(r) ((r) / 57.295779513082320876798154814105)
#endif

// Conditions

// Internal
#define SYSTEM_OR_FLAGCHECK		-1
#define SYSTEM_STARTOFFRAME		1
#define SYSTEM_ENDOFFRAME		2
#define SYSTEM_STARTOFAPP		3
#define SYSTEM_ENDOFAPP			4
#define SYSTEM_ALWAYS			5
#define SYSTEM_COMPARE2			6
#define SYSTEM_EVALUATE			7
#define SYSTEM_ONLOOP			8
#define SYSTEM_WHILE			9
#define SYSTEM_FOR				10
#define SYSTEM_REPEAT			11
#define SYSTEM_FOREACHOBJ		12

#define SYSTEM_COMPAREGLOBAL	13
#define SYSTEM_EVERY_MS			14
#define SYSTEM_EVERY_TICKS		15
#define SYSTEM_COMPARETIME		16
#define SYSTEM_ELSE				17
#define SYSTEM_OR				18
//#define SYSTEM_ONQUIT			18
#define SYSTEM_POINTOVERLAP		19
#define SYSTEM_POINTCOLLISION	20
#define SYSTEM_MAKETRIGGER		21
#define SYSTEM_VALUETYPE		22
#define SYSTEM_FOREACHOBJORDERED 27
#define SYSTEM_ONCOLLISIONADVANCED 28
#define SYSTEM_ISOVERLAPPINGADVANCED 29
#define SYSTEM_ISGROUPENABLED	30
#define SYSTEM_NUMBERISBETWEEN	31
#define SYSTEM_ANGLECLOCKWISE	32
#define SYSTEM_ONLOAD			33
#define SYSTEM_ONDEVICERESET	34
#define SYSTEM_ISLAYERVISIBLE	35

// Actions
#define SYSTEM_FLUSHCLIPBOARD	0
#define SYSTEM_SETCLIPBOARD		1
#define SYSTEM_WRITEINI			2
#define SYSTEM_PLAYSOUND		3
#define SYSTEM_SCROLLX			4
#define SYSTEM_SCROLLY			5
#define SYSTEM_SETANGLE			6
#define SYSTEM_SETFILTER		7
#define SYSTEM_STARTLOOP		8
#define SYSTEM_STOPLOOP			9
#define SYSTEM_BREAK			10
#define SYSTEM_MESSAGEBOX		11
#define SYSTEM_CREATE			12
#define SYSTEM_CREATEBYNAME		13
#define SYSTEM_NEXTFRAME		14
#define SYSTEM_PREVFRAME		15
#define SYSTEM_GOTOFRAME		16
#define SYSTEM_QUIT				17
#define SYSTEM_CANCELCLOSE		18
#define SYSTEM_SETFPS			19
#define SYSTEM_SETGLOBAL		20
#define SYSTEM_ADDGLOBAL		21
#define SYSTEM_SUBGLOBAL		22
#define SYSTEM_ENABLEGROUP		23
//#define SYSTEM_DISABLEGROUP		24
#define SYSTEM_SCROLLOBJ		24
#define SYSTEM_SERIALIZE		25
#define SYSTEM_ENDMODALFRAME	26
#define SYSTEM_CREATERELATIVEPP 27
#define SYSTEM_CREATERELATIVEIP 28

#define SYSTEM_SETLAYERSCROLLXRATIO		29
#define SYSTEM_SETLAYERSCROLLYRATIO		30
#define SYSTEM_SETLAYERZOOMRATIO		31
#define SYSTEM_SETLAYERVISIBLE			35
#define SYSTEM_SETLAYEROPACITY			36
#define SYSTEM_SETLAYERFILTER			37

#define SYSTEM_SETRSOLUTION				40
#define SYSTEM_QUICKSAVE				41

#define SYSTEM_FX_ACTIVATE		42
#define SYSTEM_FX_DEACTIVATE	43
#define SYSTEM_FX_SETPARAM		44

/*
#define SYSTEM_XMOUSE			0
#define SYSTEM_YMOUSE			1
#define SYSTEM_INDEX			2
#define SYSTEM_LOOPLENGTH		3
*/
#define SYSTEM_VARINDEX			26

//#define DEFINE_SYS_ROUTINE(r) long WINAPI r(struct RunObjectHeaderInfo* pRuntime, class ExpReturn* params)

struct LoopInfo {
	__int64 index;
	__int64 max;
};

// For OR saving across events
struct SolSave {
	CRunObjType* pType;
	ObjList selection;
};

typedef const map<CString, LoopInfo>::iterator LoopsConstIterator;
typedef map<CString, LoopInfo>::iterator LoopsIterator;

typedef long (SystemObject::*SYSTEMROUTINE)(LPVAL theParams);
typedef long (SystemObject::*SYSEXPROUTINE)(LPVAL theParams, ExpReturn& ret);

//////////////
// Prototypes

class SystemObject : public CRunObject
{
public:
	static short GetCndFlags(int cndID);
	static short GetActFlags(int actID);

	SystemObject() : quicksave(g_allocator)
#ifdef CONSTRUCT_RUNTIME_TR1
					 , mtwister(time(0))
#endif
	{
		loading = false; changeresolution = false; PythonInit = false;
		changeResFullscreen = -1;
		changeMultisampling = -1;
		pLayout = NULL;
	}

	int OnFrame() { return 0; }
	int OnFrame2() { return 0; }
	void OnCreate() { }
	void Draw() { }
	LRESULT	WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) { return 0; }
	BOOL	PreTranslateMessage(MSG* pMsg) { return 0; }
	long GetData(int id, void* p) { return 0; }
	long CallFunction(int id, void* p) { return 0; }
	void OnFrameChange(int oldFrame, int newFrame) { }
	void Serialize(class bin& ar) { }
	long ReturnDefaultValue(const ExpReturn* theParams, ExpReturn& ret) { return 0; }
	long ReturnUndefinedExpression(CString& expName, const ExpReturn* theParams, ExpReturn& ret) { return 0; }

	// System object debugging
	void DebuggerUpdateDisplay(ExpStore*& privateVars);
	void OnDebuggerValueChanged(const char* name, const char* value);

	// Get ACE routines
	static ACROUTINE GetCndRoutine(int cndID);
	static ACROUTINE GetActRoutine(int actID);
	static void GetExpRoutine(const CString& ident, ExpIdent* part);

	// CRuntime
	CRuntime* pCRuntime;

	// Conditions
	long	TriggerFunc(LPVAL theParams);
	long	cCompare2Values(LPVAL theParams);		// cndID 6
	long	cEvaluate(LPVAL theParams);				// cndID 7
	long	cWhile(LPVAL theParams);				// cndID 9
	long	cFor(LPVAL theParams);					// cndID 10
	long	cRepeat(LPVAL theParams);				// cndID 11
	long	cForEach(LPVAL theParams);				// cndID 12
	long	cCompareGlobal(LPVAL theParams);
	long	cEveryMs(LPVAL theParams);
	long	cEveryTicks(LPVAL theParams);
	long	cCompareTime(LPVAL theParams);
	long	cValueIsType(LPVAL theParams);
	long	cPointOverlap(LPVAL theParams);
	long	cPointCollision(LPVAL theParams);
	long	cMakeTrigger(LPVAL theParams);

	// OR conditions
	long	cOR_FlagCheck(LPVAL theParams);			// -1

	long	cCompareScript(LPVAL theParams);
	long	cMotionBlurSupported(LPVAL theParams);
	long	cForEachOrder(LPVAL theParams);				// cndID 27
	long	cOnCollisionAdvanced(LPVAL theParams);		// cndID 28
	long	cIsOverlappingAdvanced(LPVAL theParams);	// cndID 29
	long	cIsGroupEnabled(LPVAL theParams);			// cndID 30
	long	cIsNumberBetween(LPVAL theParams);			// cndID 31
	long	cAngleClockwise(LPVAL theParams);			// cndID 32

	long	cIsLayerVisible(LPVAL theParams);

	// Actions
	long	aFlushClipboard(LPVAL theParams);		// actID 0
	long	aSetClipboard(LPVAL theParams);			// actID 1
	long	aWriteINI(LPVAL theParams);				// actID 2
	long	aPlaySound(LPVAL theParams);			// actID 3
	long	aScrollX(LPVAL theParams);				// actID 4
	long	aScrollY(LPVAL theParams);				// actID 5
	long	aSetAngle(LPVAL theParams);				// actID 6
	long	aSetFilter(LPVAL theParams);			// actID 7
	long	aStartLoop(LPVAL theParams);			// actID 8
	long	aStopLoop(LPVAL theParams);				// actID 9
	long	aBreak(LPVAL theParams);				// actID 10
	long	aMessageBox(LPVAL theParams);			// actID 11
	long	aCreate(LPVAL theParams);				// actID 12
	long	aCreateByName(LPVAL theParams);				// actID 13
	long	aNextFrame(LPVAL theParams);				// actID 14
	long	aPrevFrame(LPVAL theParams);				// actID 15
	long	aGotoFrame(LPVAL theParams);				// actID 16
	long	aQuit(LPVAL theParams);					// actID 17
	long	aCancelClose(LPVAL theParams);				// actID 18
	long	aSetFPS(LPVAL theParams);				// actID 19
	long	aSetGlobal(LPVAL theParams);				// actID 20
	long	aAddGlobal(LPVAL theParams);				// actID 21
	long	aSubGlobal(LPVAL theParams);				// actID 22
	long	aEnableGroup(LPVAL theParams);				// actID 23
	//long	aDisableGroup(LPVAL theParams);				// actID 24
	long	aScrollObj(LPVAL theParams);				// actID 25

	long	aOrFlagOn(LPVAL theParams);
	long	aSerialize(LPVAL theParams);
	long	aEndModalFrame(LPVAL theParams);
	long	aCreateRelativeIP(LPVAL theParams);
	long	aCreateRelativePP(LPVAL theParams);

	long	aSetLayerScrollXRatio(LPVAL theParams);
	long	aSetLayerScrollYRatio(LPVAL theParams);
	long	aSetLayerZoomRatio(LPVAL theParams);
	long	aSetLayerVisible(LPVAL theParams);
	long	aSetLayerOpacity(LPVAL theParams);
	long	aSetLayerFilter(LPVAL theParams);

	long	aSetZoom(LPVAL theParams);
	long	aSetLayoutSize(LPVAL theParams);
	long	aSetResolution(LPVAL theParams);
	long	aSetFullscreen(LPVAL theParams);
	long	aQuicksave(LPVAL theParams);

	long	aFxActivate(LPVAL theParams);	
	long	aFxDeactivate(LPVAL theParams);	
	long	aFxSetParam(LPVAL theParams);	

	long	aRunScript(LPVAL theParams);
	long	aSetMotionBlur(LPVAL theParams);
	long	aSetTimeScale(LPVAL theParams);
	long	aSetLayerAngle(LPVAL theParams);

	long	aToggleGroup(LPVAL theParams);
	long	aLoadTextures(LPVAL theParams);

	long	aAddToAttribute(LPVAL theParams);
	long    aEnableLayerEffect(LPVAL theParams);
	long    aSetLayerEffectParam(LPVAL theParams);

	//long	aSetMultisampling(LPVAL theParams);

	// Expressions
	long	eMouseX(LPVAL theParams, ExpReturn& ret);				// expID 0
	long	eMouseY(LPVAL theParams, ExpReturn& ret);				// expID 1
	long	eIndex(LPVAL theParams, ExpReturn& ret);				// expID 2
	long	eLoopLength(LPVAL theParams, ExpReturn& ret);			// expID 3
	long	eMax(LPVAL theParams, ExpReturn& ret);					// expID 4
	long	eMin(LPVAL theParams, ExpReturn& ret);					// expID 5
	long	eTotalObjects(LPVAL theParams, ExpReturn& ret);			// expID 6
	long	eGlobalValue(LPVAL theParams, ExpReturn& ret);			// expID 7
	long	eFormatDecimal(LPVAL theParams, ExpReturn& ret);		// expID 8
	long	ePadZero(LPVAL theParams, ExpReturn& ret);				// expID 9
	long	eNewLine(LPVAL theParams, ExpReturn& ret);				// expID 10
	long	eLeftStr(LPVAL theParams, ExpReturn& ret);				// expID 11
	long	eRightStr(LPVAL theParams, ExpReturn& ret);				// expID 12
	long	eMidStr(LPVAL theParams, ExpReturn& ret);				// expID 13
	long	eUpperCase(LPVAL theParams, ExpReturn& ret);			// expID 14
	long	eLowerCase(LPVAL theParams, ExpReturn& ret);			// expID 15
	long	eFind(LPVAL theParams, ExpReturn& ret);					// expID 16
	long	eRemove(LPVAL theParams, ExpReturn& ret);				// expID 17
	long	eReplace(LPVAL theParams, ExpReturn& ret);				// expID 18
	long	eTrim(LPVAL theParams, ExpReturn& ret);					// expID 19
	long	eReverse(LPVAL theParams, ExpReturn& ret);				// expID 20
	long	eNumTokens(LPVAL theParams, ExpReturn& ret);			// expID 21
	long	eExtractSubstring(LPVAL theParams, ExpReturn& ret);		// expID 22
	long	eDistance(LPVAL theParams, ExpReturn& ret);				// expID 23
	long	eAngle(LPVAL theParams, ExpReturn& ret);				// expID 24
	long	eCountMatching(LPVAL theParams, ExpReturn& ret);		// expID 25

	// _varindex internal expression
	long	eVarIndex(LPVAL theParams, ExpReturn& ret);

	long	eScrollX(LPVAL theParams, ExpReturn& ret);
	long	eScrollY(LPVAL theParams, ExpReturn& ret);
	long	eWinWidth(LPVAL theParams, ExpReturn& ret);
	long	eWinHeight(LPVAL theParams, ExpReturn& ret);			// 30
	long	eGetPsVersion(LPVAL theParams, ExpReturn& ret);			// 31
	long	eRGB(LPVAL theParams, ExpReturn& ret);
	long	eGetRed(LPVAL theParams, ExpReturn& ret);
	long	eGetGreen(LPVAL theParams, ExpReturn& ret);
	long	eGetBlue(LPVAL theParams, ExpReturn& ret);				// 35
	long	eGetTickCount(LPVAL theParams, ExpReturn& ret);
	long	eGetTimer(LPVAL theParams, ExpReturn& ret);

	long	eLayerZoomX(LPVAL theParams, ExpReturn& ret);
	long	eLayerZoomY(LPVAL theParams, ExpReturn& ret);
	long	eLayerScrollRateX(LPVAL theParams, ExpReturn& ret);
	long	eLayerScrollRateY(LPVAL theParams, ExpReturn& ret);
	long	eLayerOpacity(LPVAL theParams, ExpReturn& ret);
	long	eLayerName(LPVAL theParams, ExpReturn& ret);
	long	eLayerCount(LPVAL theParams, ExpReturn& ret);

	long	eZoomX(LPVAL theParams, ExpReturn& ret);
	long	eZoomY(LPVAL theParams, ExpReturn& ret);
	long	eTimeDelta(LPVAL theParams, ExpReturn& ret);

	long	eScrollXLeft(LPVAL theParams, ExpReturn& ret);
	long	eScrollXRight(LPVAL theParams, ExpReturn& ret);
	long	eScrollYTop(LPVAL theParams, ExpReturn& ret);
	long	eScrollYBottom(LPVAL theParams, ExpReturn& ret);
	long	eDisplayAngle(LPVAL theParams, ExpReturn& ret);
	long	eClamp(LPVAL theParams, ExpReturn& ret);
	long	eAppPath(LPVAL theParams, ExpReturn& ret);
	long	eLayoutWidth(LPVAL theParams, ExpReturn& ret);
	long	eLayoutHeight(LPVAL theParams, ExpReturn& ret);

	long	eRunScript(LPVAL theParams, ExpReturn& ret);
	long	eLerp(LPVAL theParams, ExpReturn& ret);
	long	eVram(LPVAL theParams, ExpReturn& ret);
	long	eDisplayDevice(LPVAL theParams, ExpReturn& ret);
	long	eGetTimeScale(LPVAL theParams, ExpReturn& ret);
	long	eGetLayerAngle(LPVAL theParams, ExpReturn& ret);
	long	eLinearAim(LPVAL theParams, ExpReturn& ret);
	long	eLayoutNumber(LPVAL theParams, ExpReturn& ret);
	long	eLayoutName(LPVAL theParams, ExpReturn& ret);

	long	eSign(LPVAL theParams, ExpReturn& ret);
	long	eQarp(LPVAL theParams, ExpReturn& ret);
	long	eCosp(LPVAL theParams, ExpReturn& ret);
	long	eAngleDiff(LPVAL theParams, ExpReturn& ret);
	long	eRotateAngle(LPVAL theParams, ExpReturn& ret);
	long	eNormalRandom(LPVAL theParams, ExpReturn& ret);
	long	eCubicInterpolation(LPVAL theParams, ExpReturn& ret);

	long	eGetMultisamples(LPVAL theParams, ExpReturn& ret);

	void	DoFrameChange(int frameNum, CString transition, int msTime);

	CRunLayer* GetLayerParam(const ExpBase& param) const;
	CRunLayout* GetLayoutParam(const ExpBase& param) const;
	float	GetMinLayerZoomX() const;
	float	GetMinLayerZoomY() const;

	bool WarnInfiniteLoop(CString type, __int64 numiterations);

	double Lerp(double a, double b, double x);
	double Qarp(double a, double b, double c, double x);


	// Loop indices
	map<CString, LoopInfo>	loops;
	vector<LoopInfo*> loopstack;		// for most recent loop
	vector<int> whileStack;			// stack for while loops

	// SOL-saving class for iterating SOLs in for-each loops
	class ForEachSolSaver {
	public:
		// SOL for the iterated object
		ObjList mySol;

		// SOLs for associated family movements; indices correspond to familyMovs
		vector<ObjList> familyMovSols;

		// ctors
		ForEachSolSaver() {}

		ForEachSolSaver(const ForEachSolSaver& other)
		: mySol(other.mySol),
	  	  familyMovSols(other.familyMovSols)
		{}

		ForEachSolSaver(CRunObjType* pType);
	};

	// For for-each (list since it must not reallocate)
	list<ForEachSolSaver> forEachStack;

	// For OR conditions
	vector<int>		orFlags;
	vector<vector<SolSave> > orSolSaves;

	// For Every events
	map<void*, double> everyEvents;

	// For timer-equal events
	vector<void*> expiredEqualities;

	// For Make Trigger conditions
	map<void*, __int64> makeTriggers;

	// For saving&loading
	bool loading;
	bool saving;
	CString loadPath;

	// For quicksave
	bin quicksave;

	bool changeresolution;
	int changeResWidth;
	int changeResHeight;
	int changeResFullscreen; // -1 = no change, 0 = make windowed, 1 = make fullscreen
	int changeMultisampling;	// -1 = no change; 0+ = set multisampling

	bool PythonInit;

#ifdef PYTHON
	bool InitPython();
	void HandlePyErrors();
	PyObject *pDictionary;

	map<CString, SYSTEMROUTINE> PySetAttrib;
	map<CString, SYSEXPROUTINE> PyGetAttrib;



#endif

#ifdef CONSTRUCT_RUNTIME_TR1
	tr1::mt19937 mtwister;
#endif
};
