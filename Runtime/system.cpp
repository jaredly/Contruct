
#include "stdafx.h"

// Comparison do-er
template<typename T>
inline long DoComparison(int construct_operator, const T& l, const T& r)
{
	switch (construct_operator) {
	case 0:
		return l == r;
	case 1:
		return l != r;
	case 2:
		return l < r;
	case 3:
		return l <= r;
	case 4:
		return l > r;
	case 5:
		return l >= r;
	default:
		__assume(0);
	}
}

inline int MakeConditionTrigger(VRuntime* pRuntime, int truth)
{
	void* pCnd = pRuntime->GetCurrentConditionPtr();

	static map<void*, int> cndMap;

	map<void*, int>::iterator i = cndMap.find(pCnd);

	// No record
	if (i == cndMap.end()) {
		// Insert a record and allow event to run according to truth
		cndMap[pCnd] = truth;
		return truth;
	}
	else {
		int lastTruth = i->second;
		i->second = truth;

		// True when was false last time, and true this time.
		return !lastTruth && truth;
	}
}

// Get sys condition flags
short SystemObject::GetCndFlags(int cndID)
{
	switch (cndID) {

		// Triggers
	case SYSTEM_STARTOFFRAME:
	case SYSTEM_ENDOFFRAME:
	case SYSTEM_STARTOFAPP:
	case SYSTEM_ENDOFAPP:
	case SYSTEM_ONLOAD:
	case SYSTEM_ONDEVICERESET:
//	case SYSTEM_ONQUIT:
		return CND_TRIGGERED;
	case SYSTEM_ONLOOP:
		return CND_FASTTRIGGERED;

		// Manually selects its object param
	case SYSTEM_FOREACHOBJ:
	case SYSTEM_FOREACHOBJORDERED:
	case SYSTEM_POINTOVERLAP:
	case SYSTEM_ONCOLLISIONADVANCED:
	case SYSTEM_ISOVERLAPPINGADVANCED:
	case SYSTEM_OR:	// temporary, come up with real fix
		return SOL_MODIFIER;

		// Evaluated events
	default:
		return 0;
	}
}



ACROUTINE SystemObject::GetCndRoutine(int cndID)
{
	switch (cndID) {
		// Simple triggers use TriggerFunc to return true
	case SYSTEM_STARTOFFRAME:
	case SYSTEM_ENDOFFRAME:
	case SYSTEM_STARTOFAPP:
	case SYSTEM_ENDOFAPP:
	case SYSTEM_ONLOOP:
	case SYSTEM_ALWAYS:		// Always just returns true as well
//	case SYSTEM_ONQUIT:
	case SYSTEM_ELSE:		// Else just returns true as well
	case SYSTEM_OR:			// OR conditions are always removed, but they run through this function before OR parsing.
	case SYSTEM_ONLOAD:
	case SYSTEM_ONDEVICERESET:
		return (ACROUTINE)&SystemObject::TriggerFunc;
	case SYSTEM_COMPARE2:
		return (ACROUTINE)&SystemObject::cCompare2Values;
	case SYSTEM_EVALUATE:
		return (ACROUTINE)&SystemObject::cEvaluate;
	case SYSTEM_WHILE:
		return (ACROUTINE)&SystemObject::cWhile;
	case SYSTEM_FOR:
		return (ACROUTINE)&SystemObject::cFor;
	case SYSTEM_REPEAT:
		return (ACROUTINE)&SystemObject::cRepeat;
	case SYSTEM_FOREACHOBJ:
		return (ACROUTINE)&SystemObject::cForEach;
	case SYSTEM_COMPAREGLOBAL:
		return (ACROUTINE)&SystemObject::cCompareGlobal;
	case SYSTEM_EVERY_MS:
		return (ACROUTINE)&SystemObject::cEveryMs;
	case SYSTEM_EVERY_TICKS:
		return (ACROUTINE)&SystemObject::cEveryTicks;
	case SYSTEM_COMPARETIME:
		return (ACROUTINE)&SystemObject::cCompareTime;
	case SYSTEM_POINTOVERLAP:
		return (ACROUTINE)&SystemObject::cPointOverlap;
	case SYSTEM_POINTCOLLISION:
		return (ACROUTINE)&SystemObject::cPointCollision;
	case SYSTEM_MAKETRIGGER:
		return (ACROUTINE)&SystemObject::cMakeTrigger;
	case SYSTEM_VALUETYPE:
		return (ACROUTINE)&SystemObject::cValueIsType;
	case 25:
		return (ACROUTINE)&SystemObject::cCompareScript;
	case 26:
		return (ACROUTINE)&SystemObject::cMotionBlurSupported;
	case SYSTEM_FOREACHOBJORDERED:
		return (ACROUTINE)&SystemObject::cForEachOrder;
	case SYSTEM_ONCOLLISIONADVANCED:
		return (ACROUTINE)&SystemObject::cOnCollisionAdvanced;
	case SYSTEM_ISOVERLAPPINGADVANCED:
		return (ACROUTINE)&SystemObject::cIsOverlappingAdvanced;
	case SYSTEM_ISGROUPENABLED:
		return (ACROUTINE)&SystemObject::cIsGroupEnabled;
	case SYSTEM_NUMBERISBETWEEN:
		return (ACROUTINE)&SystemObject::cIsNumberBetween;
	case SYSTEM_ANGLECLOCKWISE:
		return (ACROUTINE)&SystemObject::cAngleClockwise;
	case SYSTEM_ISLAYERVISIBLE:
		return (ACROUTINE)&SystemObject::cIsLayerVisible;
	default:
		throw runtime_error("Invalid system condition referenced");
	}
}

int SysActScriptParamCount[] = {
	0, //"FlushClipboard",
	1, //"SetClipboard",
	4, //"WriteINI",
	2, //"PlaySound",
	1, //"ScrollX",
	1, //"ScrollY",			// 5
	1, //"SetAngle",
	1, //"SetFilter",
	2, //"StartLoop",
	1, //"StopLoop",
	0, //"Break",				// 10
	4, //"MessageBox",
	4, //"Create",
	5, //"Create2",				// uses same routine
	2, //"NextFrame",
	2, //"PrevFrame",			// 15
	3, //"GotoFrame",
	0, //"Quit",
	0, //"CancelClose",
	1, //"aSetFPS",
	2, //"SetGlobal",			// 20
	2, //"AddGlobal",
	2, //"SubGlobal",
	2, //"EnableGroup",
	1, //"ScrollObj",
	2, //"Serialize",			// 25
	0, //"EndModalFrame",
	4, //"CreateRelativePP",
	5, //"CreateRelativeIP",
	2, //"SetLayerScrollXRatio",
	2, //"SetLayerScrollYRatio",	// 30
	3, //"SetLayerZoomRatio",
	0, //"",
	0, //"",
	0, //"",
	2, //"SetLayerVisible",		// 35
	2, //"SetLayerOpacity",
	2, //"SetLayerFilter",
	2, //"SetZoom",
	2, //"SetLayoutSize",
	2, //"SetResolution",			// 40
	1, //"Quicksave",
	2, //"FxActivate",
	2, //"FxDeactivate",
	3, //"FxSetParam",
	1, //"RunScript",				// 45
	1,	// "SetMotionBlur"
	1,	// "SetFullscreen"
	1,	// "SetTimeScale"
	1,	// "SetLayerAngle"
	1,	// "ToggleGroup"			// 50
	2,	// "LoadTextures"
	3,  // "AddToAttribute"
	3,  // "EnableLayerEffect"
	4,  // "SetLayerEffectParam"

};

CString SysActScript[] = {
	"FlushClipboard",
	"SetClipboard",
	"WriteINI",
	"PlaySound",
	"SetScrollX",
	"SetScrollY",			// 5
	"SetAngle",
	"SetFilter",
	"StartLoop",
	"StopLoop",
	"Break",				// 10
	"MessageBox",
	"Create",
	"CreateAt",				// uses same routine
	"NextFrame",
	"PrevFrame",			// 15
	"GotoFrame",
	"Quit",
	"CancelClose",
	"aSetFPS",
	"SetGlobalVar",			// 20
	"AddGlobalVar",
	"SubGlobalVar",
	"EnableGroup",
	"ScrollObj",
	"Serialize",			// 25
	"EndModalFrame",
	"CreateRelativePP",
	"CreateRelativeIP",
	"SetLayerScrollXRatio",
	"SetLayerScrollYRatio",	// 30
	"SetLayerZoomRatio",
	"",
	"",
	"",
	"SetLayerVisible",		// 35
	"SetLayerOpacity",
	"SetLayerFilter",
	"SetZoom",
	"SetLayoutSize",
	"SetResolution",			// 40
	"Quicksave",
	"FxActivate",
	"FxDeactivate",
	"FxSetParam",
	"RunScript",				// 45
	"SetMotionBlur",
	"SetFullscreen",
	"SetTimeScale",
	"SetLayerAngle",
	"ToggleGroup",				// 50
	"LoadTextures",
	"AddToAttribute",
	"EnableLayerEffect",
	"SetLayerEffectParam"
};


SYSTEMROUTINE SysActRoutines[] = {
	&SystemObject::aFlushClipboard,
	&SystemObject::aSetClipboard,
	&SystemObject::aWriteINI,
	&SystemObject::aPlaySound,
	&SystemObject::aScrollX,
	&SystemObject::aScrollY,			// 5
	&SystemObject::aSetAngle,
	&SystemObject::aSetFilter,
	&SystemObject::aStartLoop,
	&SystemObject::aStopLoop,
	&SystemObject::aBreak,				// 10
	&SystemObject::aMessageBox,
	&SystemObject::aCreate,
	&SystemObject::aCreate,				// uses same routine
	&SystemObject::aNextFrame,
	&SystemObject::aPrevFrame,			// 15
	&SystemObject::aGotoFrame,
	&SystemObject::aQuit,
	&SystemObject::aCancelClose,
	&SystemObject::aSetFPS,
	&SystemObject::aSetGlobal,			// 20
	&SystemObject::aAddGlobal,
	&SystemObject::aSubGlobal,
	&SystemObject::aEnableGroup,
	&SystemObject::aScrollObj,
	&SystemObject::aSerialize,			// 25
	&SystemObject::aEndModalFrame,
	&SystemObject::aCreateRelativePP,
	&SystemObject::aCreateRelativeIP,
	&SystemObject::aSetLayerScrollXRatio,
	&SystemObject::aSetLayerScrollYRatio,	// 30
	&SystemObject::aSetLayerZoomOffset,
	&SystemObject::aSetLayerScrollXOffset,
	&SystemObject::aSetLayerScrollYOffset,
	&SystemObject::aSetLayerZoomRate,
	&SystemObject::aSetLayerVisible,		// 35
	&SystemObject::aSetLayerOpacity,
	&SystemObject::aSetLayerFilter,
	&SystemObject::aSetZoom,
	&SystemObject::aSetLayoutSize,
	&SystemObject::aSetResolution,			// 40
	&SystemObject::aQuicksave,
	&SystemObject::aFxActivate,
	&SystemObject::aFxDeactivate,
	&SystemObject::aFxSetParam,
	&SystemObject::aRunScript,				// 45
	&SystemObject::aSetMotionBlur,
	&SystemObject::aSetFullscreen,
	&SystemObject::aSetTimeScale,
	&SystemObject::aSetLayerAngle,
	&SystemObject::aToggleGroup,
	&SystemObject::aLoadTextures,
	&SystemObject::aAddToAttribute,
	&SystemObject::aEnableLayerEffect,
	&SystemObject::aSetLayerEffectParam,
};

#define SYSTEM_SETLAYERSCROLLXRATIO		29
#define SYSTEM_SETLAYERSCROLLYRATIO		30
#define SYSTEM_SETLAYERZOOMRATIO		31
#define SYSTEM_SETLAYERVISIBLE			35
#define SYSTEM_SETLAYEROPACITY			36
#define SYSTEM_SETLAYERFILTER			37

ACROUTINE SystemObject::GetActRoutine(int actID)
{
	return (ACROUTINE)SysActRoutines[actID];
}

short SystemObject::GetActFlags(int actID)
{
	switch (actID) {
	case SYSTEM_CREATE:
	case SYSTEM_CREATEBYNAME:
	case SYSTEM_CREATERELATIVEPP:
	case SYSTEM_CREATERELATIVEIP:
		return SOL_MODIFIER;
	default:
		return 0;
	}
}

struct ExpRoutineEntry {
	const char* name;
	SYSEXPROUTINE routine;
};


int	SysExpScriptParamCount[] = {
	0,	//"mousex"				0
	0,	//"mousey"
	1,	//"loopindex"
	1,	//"looplength"
	-1,	//"max"
	-1,	//"min"					 5
	0,  //totalobjects
	1,  //global
	1,	//formatdecimal
	2,	//zeropad
	0,	//newline				10
	2,  //left
	2,  //right
	3,  //mid
	1,  //uppercase
	1,  //lowercase				15
	3,	//Find
	2,  //remove
	3,  //replace
	1,  //trim
	1,	//reverse				20
	2,  //numtokens
	3,  //gettoken
	4,  //distance
	4,  //angle
	1,	//						25
	-2, //dont use
	0, //scrollx
	0, //scrolly
	0, //windowwidth
	0, //windowheight			30
	0, //pixelshaderversion
	3, //RGB
	1, //getred
	1, //green
	1, //blue					35
	0, //tick count
	0, //timer
	1, //layerzoomx
	1, //layerzoomy
	1, //layerscrollratex		40
	1, //layerscrollratey		
	1, //layeropacity
	1, //layername
	0, //layercount
	0, //zoomx					45
	0, //zoomy
	0, //timedelta
	0, //scrollxleft
	0, //scrollxright
	0, //scrollytop				50
	0, //scrollybottom
	0, //displayangle
	3, //clamp
	0, //app path
	0, //layout width
	0, //layoutheight
	1, //pyexpression
	3, //lerp
	0, //vram
	0, //displaydevice
	0, //timescale
	1, //layerangle
	7, //linearaim
	0, //layoutnumber
	0, //layoutname
	1, //sign
	4, //qarp
	3, //cosp
	2, //anglediff
	3, //rotateangle
	2, //normalrandom
	5, //cubic
	0, //getmultisamples
};



const ExpRoutineEntry ExpRoutineTable[] =	{
	{"mousex", &SystemObject::eMouseX},						// 0
	{"mousey", &SystemObject::eMouseY},
	{"loopindex", &SystemObject::eIndex},
	{"looplength", &SystemObject::eLoopLength},
	{"max", &SystemObject::eMax},
	{"min", &SystemObject::eMin},							// 5
	{"totalobjects", &SystemObject::eTotalObjects},
	{"global", &SystemObject::eGlobalValue},
	{"formatdecimal", &SystemObject::eFormatDecimal},
	{"zeropad", &SystemObject::ePadZero},
	{"newline", &SystemObject::eNewLine},					// 10
	{"left", &SystemObject::eLeftStr},
	{"right", &SystemObject::eRightStr},
	{"mid", &SystemObject::eMidStr},
	{"uppercase", &SystemObject::eUpperCase},
	{"lowercase", &SystemObject::eLowerCase},				// 15
	{"find", &SystemObject::eFind},
	{"remove", &SystemObject::eRemove},
	{"replace", &SystemObject::eReplace},
	{"trim", &SystemObject::eTrim},
	{"reverse", &SystemObject::eReverse},					// 20
	{"numtokens", &SystemObject::eNumTokens},
	{"gettoken", &SystemObject::eExtractSubstring},
	{"distance", &SystemObject::eDistance},
	{"angle", &SystemObject::eAngle},
	{"countmatching", &SystemObject::eCountMatching},		// 25
	{"_varindex", &SystemObject::eVarIndex},
	{"scrollx", &SystemObject::eScrollX},
	{"scrolly", &SystemObject::eScrollY},
	{"displaywidth", &SystemObject::eWinWidth},
	{"displayheight", &SystemObject::eWinHeight},			// 30
	{"ps_version", &SystemObject::eGetPsVersion},
	{"rgb", &SystemObject::eRGB},
	{"getred", &SystemObject::eGetRed},
	{"getgreen", &SystemObject::eGetGreen},
	{"getblue", &SystemObject::eGetBlue},					// 35
	{"tickcount", &SystemObject::eGetTickCount},
	{"timer", &SystemObject::eGetTimer},
	{"layerzoomx", &SystemObject::eLayerZoomX},
	{"layerzoomy", &SystemObject::eLayerZoomY},
	{"layerscrollratex", &SystemObject::eLayerScrollRateX},	// 40
	{"layerscrollratey", &SystemObject::eLayerScrollRateY},
	{"layeropacity", &SystemObject::eLayerOpacity},
	{"layername", &SystemObject::eLayerName},
	{"layercount", &SystemObject::eLayerCount},
	{"zoomx",	&SystemObject::eZoomX},
	{"zoomy",	&SystemObject::eZoomY},
	{"timedelta",	&SystemObject::eTimeDelta},
	{"scrollxleft", &SystemObject::eScrollXLeft},
	{"scrollxright", &SystemObject::eScrollXRight},
	{"scrollytop", &SystemObject::eScrollYTop},
	{"scrollybottom", &SystemObject::eScrollYBottom},
	{"displayangle", &SystemObject::eDisplayAngle},
	{"clamp", &SystemObject::eClamp},
	{"apppath", &SystemObject::eAppPath},
	{"layoutwidth", &SystemObject::eLayoutWidth},
	{"layoutheight", &SystemObject::eLayoutHeight},
	{"python", &SystemObject::eRunScript},
	{"lerp", &SystemObject::eLerp},
	{"vram", &SystemObject::eVram},
	{"displaydevice", &SystemObject::eDisplayDevice},
	{"timescale", &SystemObject::eGetTimeScale},
	{"layerangle", &SystemObject::eGetLayerAngle},
	{"linearaim", &SystemObject::eLinearAim},
	{"layoutnumber", &SystemObject::eLayoutNumber},
	{"layoutname", &SystemObject::eLayoutName},
	{"sign", &SystemObject::eSign},
	{"qarp", &SystemObject::eQarp},
	{"cosp", &SystemObject::eCosp},
	{"anglediff", &SystemObject::eAngleDiff},
	{"rotateangle", &SystemObject::eRotateAngle},
	{"normalrandom", &SystemObject::eNormalRandom},
	{"cubic", &SystemObject::eCubicInterpolation},
	{"multisamples", &SystemObject::eGetMultisamples},
	{"", NULL}
};

void SystemObject::GetExpRoutine(const CString& ident, ExpIdent* part)
{
	for (int i = 0; strlen(ExpRoutineTable[i].name) > 0; i++) {
		if (ident == ExpRoutineTable[i].name) {
			part->expID = i;
			part->routine = (EXPROUTINE)ExpRoutineTable[i].routine;
			return;
		}
	}
	// Not found
	part->expID = -1;
}

inline int SysGetNumParams(LPVAL theParams)
{
	LPVAL ptr = theParams;
	for ( ; ptr->Type() != EXPTYPE_NULL; ptr++);
	return ptr - theParams;	// pointer arithmetic subtraction
}

CRunLayer* SystemObject::GetLayerParam(const ExpBase& param) const
{
	switch (param.Type()) {

	// Interpret as layer number
	case EXPTYPE_INTEGER:
	case EXPTYPE_FLOAT:
		{
			int layerNum = param.GetInt() - 1;

			if (layerNum < 0 || layerNum >= pLayout->layers.size()) {
#ifdef CONSTRUCT_DEBUGGER
				CString msg;
				msg.Format("An invalid layer number (%d) was passed to an action or condition.", layerNum + 1);
				pCRuntime->LogMsg(msg);
#endif
				return NULL;
			}

			return pLayout->layers[layerNum];
		}

	// Interpret as layer name
	case EXPTYPE_STRING:
		{
			CString layerName = *(param.GetStringPtr());
			layerName.MakeLower();

			vector<CRunLayer*>::iterator y = pLayout->layers.begin();
			vector<CRunLayer*>::iterator layers_end = pLayout->layers.end();

			for ( ; y != layers_end; y++) {
				CString curName = (*y)->name;
				curName.MakeLower();

				if (layerName == curName)
					return *y;
			}

			// Not found
#ifdef CONSTRUCT_DEBUGGER
			CString msg;
			msg.Format("The layer name (\"%s\") does not exist.  Check the spelling of the layer name is correct.", layerName);
			pCRuntime->LogMsg(msg);
#endif
			return NULL;
		}

	}

	return NULL;
}

CRunLayout* SystemObject::GetLayoutParam(const ExpBase& param) const
{
	switch (param.Type()) {

	// Interpret as layout number
	case EXPTYPE_INTEGER:
	case EXPTYPE_FLOAT:
		{
			int layout_num = param.GetInt() - 1;

			if (layout_num < 0 || layout_num >= pCRuntime->Frames.size()) {
#ifdef CONSTRUCT_DEBUGGER
				CString msg;
				msg.Format("An invalid layout number (%d) was passed to an action or condition.", layout_num + 1);
				pCRuntime->LogMsg(msg);
#endif
				return NULL;
			}

			return pCRuntime->Frames[layout_num];
		}

	// Interpret as layer name
	case EXPTYPE_STRING:
		{
			CString layout_name = *(param.GetStringPtr());
			layout_name.MakeLower();

			vector<CRunLayout*>::iterator y = pCRuntime->Frames.begin();
			vector<CRunLayout*>::iterator end = pCRuntime->Frames.end();

			for ( ; y != end; ++y) {
				CString cur_name = (*y)->name;
				cur_name.MakeLower();

				if (layout_name == cur_name)
					return *y;
			}

			// Not found
#ifdef CONSTRUCT_DEBUGGER
			CString msg;
			msg.Format("The layout (\"%s\") does not exist.  Check the spelling of the layout name is correct.", layout_name);
			pCRuntime->LogMsg(msg);
#endif
			return NULL;
		}

	}

	return NULL;
}

// System debugging
void SystemObject::DebuggerUpdateDisplay(ExpStore*& privateVars)
{
	if (pLayout == NULL)
		return;

	CString str;

	pRuntime->AddDebuggerItem("Total objects", pCRuntime->totalObjects, true);

	str.Format("%d (%.2fms)", pCRuntime->fps, 1000.0 / pCRuntime->fps);
	pRuntime->AddDebuggerItem("FPS", str, true);

	str.Format("(%.0f, %.0f)", pLayout->mouseX, pLayout->mouseY);
	pRuntime->AddDebuggerItem("Mouse", str, true);

	str.Format("(%.1f, %.1f)", pLayout->scrollX /*+ pLayout->winWidth / 2*/, pLayout->scrollY /*+ pLayout->winHeight / 2*/);
	pRuntime->AddDebuggerItem("Scroll location", str, true);

	str.Format("(%d, %d)", pLayout->winWidth, pLayout->winHeight);
	pRuntime->AddDebuggerItem("Display size", str, true);

#ifdef CONSTRUCT_DIRECTX9
	str.Format("%.2f MB", (float)pCRuntime->renderer.GetRenderTargetMemoryUsage() / (1024.0f * 1024.0f));
	pRuntime->AddDebuggerItem("VRAM targets", str, true);

	str.Format("%.2f MB", (float)(pCRuntime->renderer.GetTotalTextureMemoryUsage() - pCRuntime->renderer.GetRenderTargetMemoryUsage()) / (1024.0f * 1024.0f));
	pRuntime->AddDebuggerItem("VRAM textures", str, true);

	str.Format("%.2f MB", (float)pCRuntime->renderer.GetTotalTextureMemoryUsage() / (1024.0f * 1024.0f));
	pRuntime->AddDebuggerItem("VRAM total", str, true);

	str.Format("%.2f %%", pCRuntime->lastSecondPresentTime * 100.0);
	pRuntime->AddDebuggerItem("CPU waiting on GPU", str, true);
#endif //DirectX 9

	vector<ExpStore>::iterator i = pCRuntime->globalVars.begin();
	vector<ExpStore>::iterator globalVars_end = pCRuntime->globalVars.end();
	int index = 0;

	for ( ; i != globalVars_end; i++, index++) {
		CString s;
		s.Format("'%s'", pCRuntime->globalNames[index]);
		pRuntime->AddDebuggerItem(s, i->GetString(), false);
	}
}

void SystemObject::OnDebuggerValueChanged(const char* name, const char* value)
{
	// Check for changes to global variables
	CString label = name;
	if (label.Left(1) == "'") {
		label = label.Mid(1, label.GetLength() - 2);
		int globalIndex = find_index(pCRuntime->globalNames.begin(), pCRuntime->globalNames.end(), label);
		ExpStore& globalVar = pCRuntime->globalVars[globalIndex];

		if (globalVar.Type() == EXPTYPE_STRING) {
			globalVar = value;		// Assign string to global
		}
		else {
			CString v = value;
			if (v.Find(".") == -1)
				globalVar = _atoi64(v);	// Assign integer to global
			else
				globalVar = atof(v);	// Assign float to global
		}
	}
}

// System trigger routine for Start of Frame etc.  Just returns true.
// To fit the ACROUTINE definition, the first parameter is a pointer to a rundata header.
// Typecast to CRuntime*.
long SystemObject::TriggerFunc(LPVAL params)
{
	return TRUE;
}

// param1/2: compare
// param3: 0-5 operator, !=, =, <, <=, >, >=
long SystemObject::cCompare2Values(LPVAL theParams)
{
	// Get operator ID
	int operatorID = theParams[1].GetInt();
	const ExpReturn& l = theParams[0];
	const ExpReturn& r = theParams[2];

	switch (theParams[0].Type()) {
	case EXPTYPE_INTEGER:
		return DoComparison(operatorID, l.GetInt64(), r.GetInt64());
	case EXPTYPE_FLOAT:
		return DoComparison(operatorID, l.GetDouble(), r.GetDouble());
	case EXPTYPE_STRING:
		return DoComparison(operatorID, *(l.GetStringPtr()), r.GetString());
	default:
		return false;
	}

	/*
	switch (operatorID) {
		// Not equal
	case 0:

		// Test the type of expression
		switch (theParams[0].Type()) {

			// Ints/floats: check bitwise eData content
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
			return (theParams[0].eData.iVal != theParams[2].eData.iVal);
			// Strings: use operator!=
		case EXPTYPE_STRING:
			return (*(theParams[0].eData.str) != *(theParams[2].eData.str));
			// Binary: check size matches before checking memory identical
		case EXPTYPE_BINARY:
			if (theParams[0].eData.bin.size != theParams[2].eData.bin.size)
				return TRUE;
			return (memcmp(theParams[0].eData.bin.data,
						  theParams[2].eData.bin.data,
						  theParams[0].eData.bin.size) != 0);
			// Object type: not yet implemented
		default:
			return FALSE;
		}

		// Equal
	case 1:

		// Test the type of expression
		switch (theParams[0].Type()) {

			// Ints/floats: check bitwise eData content
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
			return (theParams[0].eData.iVal == theParams[2].eData.iVal);
			// Strings: use operator!=
		case EXPTYPE_STRING:
			return (*(theParams[0].eData.str) == *(theParams[2].eData.str));
			// Binary: check size matches before checking memory identical
		case EXPTYPE_BINARY:
			if (theParams[0].eData.bin.size != theParams[2].eData.bin.size)
				return FALSE;
			return (memcmp(theParams[0].eData.bin.data,
						  theParams[2].eData.bin.data,
						  theParams[0].eData.bin.size) == 0);
			// Object type: not yet implemented
		default:
			return FALSE;
		}

		// Less
	case 2:

		switch (theParams[0].Type()) {

		case EXPTYPE_INTEGER:
			return (theParams[0].eData.iVal < theParams[2].eData.iVal);
		case EXPTYPE_FLOAT:
			return (theParams[0].eData.fVal < theParams[2].eData.fVal);
		default:
			return FALSE;

		}

		// Less or equal
	case 3:

		switch (theParams[0].Type()) {

		case EXPTYPE_INTEGER:
			return (theParams[0].eData.iVal <= theParams[2].eData.iVal);
		case EXPTYPE_FLOAT:
			return (theParams[0].eData.fVal <= theParams[2].eData.fVal);
		default:
			return FALSE;

		}

		// Greater
	case 4:

		switch (theParams[0].Type()) {

		case EXPTYPE_INTEGER:
			return (theParams[0].eData.iVal > theParams[2].eData.iVal);
		case EXPTYPE_FLOAT:
			return (theParams[0].eData.fVal > theParams[2].eData.fVal);
		default:
			return FALSE;

		}

		// Greater or equal
	case 5:

		switch (theParams[0].Type()) {

		case EXPTYPE_INTEGER:
			return (theParams[0].eData.iVal >= theParams[2].eData.iVal);
		case EXPTYPE_FLOAT:
			return (theParams[0].eData.fVal >= theParams[2].eData.fVal);
		default:
			return FALSE;

		}

	default:
		throw runtime_error("Error in compare values condition");

	}
	*/
}

///////////
// EVALUATE
// Simply return true if the parameter evaluates nonzero
long SystemObject::cEvaluate(LPVAL theParams)
{
	switch (theParams[0].Type()) {
	case EXPTYPE_INTEGER:
		return theParams[0].eData.iVal != 0;
	case EXPTYPE_FLOAT:
		return theParams[0].eData.fVal != 0.0;
	case EXPTYPE_STRING:
		return *(theParams[0].eData.str) != "";
	default:
		return FALSE;
	}
}

// WHILE LOOP
// Params:  <none>
long SystemObject::cWhile(LPVAL theParams)
{
	EventParametersInfo2 ev1;
	pCRuntime->GetCurrentEventPtrs(ev1);
	const EventParametersInfo2 ev = ev1;

	// True if conditions exist after this condition, hence RunConditions needed
	const bool cndsAfter = pCRuntime->CndsAfter(ev.pCnds);

	// If no conditions after and no subevents, it can only be breaking immediately: warn
	if (ev.pSubEvents == 0 && !cndsAfter)
		throw runtime_error("Bad 'While' event set up: event appears to run infinitely, or only once");

	// Register on while stack
	whileStack.push_back(1);	// running
	int& rRunning = whileStack.back();

#ifdef CONSTRUCT_PREVIEW
	int index = 0;
#endif

	// Run
	CEventLine* pThisEvent = pCRuntime->pCurrentEvent;

	// SOL modifications necessary: see cFor comments
	if (cndsAfter || pThisEvent->manualSolModifier) {
		
		ObjTypeList* modifiers = pThisEvent->modifiers;
		int modifiesSol = pThisEvent->modifiesSol;

		while (rRunning != 0) {
			
			if (modifiesSol)
				NewSOLCopy(modifiers);

			if (!pCRuntime->RunConditions(ev.pCnds)) {
				// Undo SOL changes
				RestoreSOL(modifiers);
				// While loops end when their conditions are no longer true
				break;
			}

			if (ev.pActs)
				pCRuntime->RunActions(ev.pActs);

			if (ev.pSubEvents)
				pCRuntime->RunSubEvents(ev.pSubEvents);

			pCRuntime->pCurrentEvent = pThisEvent;
			
			if (modifiesSol)
				RestoreSOL(modifiers);

#ifdef CONSTRUCT_PREVIEW
			if (index == 1000000) {
				if (WarnInfiniteLoop("while", -1)) break;
			}
			index++;
#endif
		}
	}
	// No SOL changes necessary (we're assuming a sub event is going to break this loop!)
	else {
		while (rRunning != 0) {

			if (ev.pActs)
				pCRuntime->RunActions(ev.pActs);

			if (ev.pSubEvents)
				pCRuntime->RunSubEvents(ev.pSubEvents);

			pCRuntime->pCurrentEvent = pThisEvent;

#ifdef CONSTRUCT_PREVIEW
			if (index == 1000000) {
				if (WarnInfiniteLoop("while", -1)) break;
			}
			index++;
#endif

		}
	}

	// Remove this while
	whileStack.pop_back();

	return 0;
}

bool SystemObject::WarnInfiniteLoop(CString type, __int64 numiterations)
{
	static bool warned = false;

	if (warned)
		return false;
	else
		warned = true;

	CString strIterations;

	if (numiterations == -1)
		strIterations = "unknown";
	else
		strIterations.Format("%I64d", numiterations);

	CString msg;
	msg.Format("A '%s' condition has reached one million iterations.  It may be looping forever which would result in "
		"the application hanging.  Do you want to terminate the application?  If you choose No the application will continue to run and you will not be warned again, "
		"but you may have to end it in Task Manager if it hangs.\n\n"
		"Event number: %d\nAttempted number of iterations: %s\n\nTerminate application?",
		
		type, pCRuntime->pCurrentEvent->number, strIterations);
	
	int id = MessageBox(NULL, msg , "Construct Preview", MB_ICONEXCLAMATION | MB_YESNO);

	if (id == IDYES) {
		pCRuntime->bRunning = false;	// Terminate
		return true;
	}

	return false;
}

// Simple event repeat
long SystemObject::cRepeat(LPVAL theParams)
{
	__int64 count = theParams[0].GetInt64();

	EventParametersInfo2 ev;
	pCRuntime->GetCurrentEventPtrs(ev);

	const bool cndsAfter = pCRuntime->CndsAfter(ev.pCnds);

	CEventLine* pThisEvent = pCRuntime->pCurrentEvent;

	// Conditions come after: preserve SOL (see cFor comments)
	if (cndsAfter || pThisEvent->manualSolModifier) {

		ObjTypeList* modifiers = pThisEvent->modifiers;
		int modifiesSol = pThisEvent->modifiesSol;

		for (__int64 i = 0; i < count; i++) {

			if (modifiesSol)
				NewSOLCopy(modifiers);

			if (!pCRuntime->RunConditions(ev.pCnds)) {

				// This loop iteration should not run
				if (modifiesSol)
					RestoreSOL(modifiers);
				continue;
			}

			if (ev.pActs)
				pCRuntime->RunActions(ev.pActs);

			if (ev.pSubEvents)
				pCRuntime->RunSubEvents(ev.pSubEvents);

			pCRuntime->pCurrentEvent = pThisEvent;
			
			if (modifiesSol)
				RestoreSOL(modifiers);

			// When previewing, alert on the 1 millionth iteration - could be infinite loop
#ifdef CONSTRUCT_PREVIEW
			if (i == 1000000) {
				if (WarnInfiniteLoop("repeat", count)) break;
			}
#endif
		}

	}
	// No conditions after - safe to run without SOL modifications
	else {
		for (__int64 i = 0; i < count; i++) {

			if (ev.pActs)
				pCRuntime->RunActions(ev.pActs);

			if (ev.pSubEvents)
				pCRuntime->RunSubEvents(ev.pSubEvents);

			pCRuntime->pCurrentEvent = pThisEvent;

			// When previewing, alert on the 1 millionth iteration - could be infinite loop
#ifdef CONSTRUCT_PREVIEW
			if (i == 1000000) {
				if (WarnInfiniteLoop("repeat", count)) break;
			}
#endif
		}
	}

	return 0;
}

// FOR LOOP
// Params: for (name, from, to)
long SystemObject::cFor(LPVAL theParams)
{
	// Get params
	const CString& loopname = *(theParams[0].GetStringPtr());
	if (&loopname == NULL) return 0;  // Null reference... icky
	const __int64 from = theParams[1].GetInt64();
	const __int64 to = theParams[2].GetInt64();

	EventParametersInfo2 ev;
	pCRuntime->GetCurrentEventPtrs(ev);

	// Make sure this loop doesnt already exist
	LoopsIterator p = loops.find(loopname);
	if (p != loops.end()) throw runtime_error("A nested loop was started with the same name as an already running loop.  Rename your loop to a unique name.");

	// Create loop struct and save
	LoopInfo loop;
	loop.index = from;
	loop.max = to;
	loops[loopname] = loop;
	loopstack.push_back(&(loops.find(loopname)->second));
	
	// Loop index reference
	p = loops.find(loopname);
	__int64& r = p->second.index;

	const bool cndsAfter = pCRuntime->CndsAfter(ev.pCnds);

#ifdef CONSTRUCT_PREVIEW
	int zeroIndex = 0;
	int iterationCount = to - r;
#endif

	CEventLine* pThisEvent = pCRuntime->pCurrentEvent;
	
	// If conditions come after this condition, we've potentially got SOL changes happening during
	// the loop.
	if (cndsAfter || pThisEvent->manualSolModifier) 
	{
		// Get a list of this event's modifiers.  It might clean up the SOL for conditions before
		// itself, but that's OK, the loop overhead is fairly big in comparison.
		ObjTypeList* modifiers = pThisEvent->modifiers;
		int modifiesSol = pThisEvent->modifiesSol;

		for ( ; r <= to; r++) {

			// New SOL copy for the For loop conditions
			if (modifiesSol)
				NewSOLCopy(modifiers);

			// Run For loop conditions, which may change the SOL copy
			if (!pCRuntime->RunConditions(ev.pCnds)) {
				// Revert SOL, may have been changed by RunConditions
				RestoreSOL(modifiers);
				continue;		// next iter
			}

			if (ev.pActs)
				pCRuntime->RunActions(ev.pActs);

			if (ev.pSubEvents)
				pCRuntime->RunSubEvents(ev.pSubEvents);

			pCRuntime->pCurrentEvent = pThisEvent;
				
			// Undo SOL changes by RunConditions
			if (modifiesSol)
				RestoreSOL(modifiers);

#ifdef CONSTRUCT_PREVIEW
			if (zeroIndex == 1000000) {
				if (WarnInfiniteLoop("for", iterationCount)) break;
			}
			zeroIndex++;
#endif
		}//for

	}
	// If no conditions come after this condition, we're free to run without explicit SOL
	// modifications (subevents, if any, will clean up after themselves)
	else {

		for ( ; r <= to; r++) {

			if (ev.pActs)
				pCRuntime->RunActions(ev.pActs);

			if (ev.pSubEvents)
				pCRuntime->RunSubEvents(ev.pSubEvents);

			pCRuntime->pCurrentEvent = pThisEvent;

#ifdef CONSTRUCT_PREVIEW
			if (zeroIndex == 1000000) {
				if (WarnInfiniteLoop("for", iterationCount)) break;
			}
			zeroIndex++;
#endif
			
		}//for

	}//else

	// Remove loop from loops
	loopstack.pop_back();
	loops.erase(loopname);

	return 0;
}

// Ctor to save SOLs for an object type - used in for each loop iterations since it destructively changes
// an object's SOL and family movement SOLs
SystemObject::ForEachSolSaver::ForEachSolSaver(CRunObjType* pType)
{
	// Take copy of SOL
	mySol = pType->pSol->sol;

	if (pType->IsFamily()) {

		// Take copies of a family's movements as well - For Each routine modifies these as well
		familyMovSols.reserve(pType->familyMovs.size());
		
		ObjTypeIterator t = pType->familyMovs.begin();
		ObjTypeIterator end = pType->familyMovs.end();

		for ( ; t != end; t++)
			familyMovSols.push_back((*t)->pSol->sol);
	}
}

// For Each instance of an object type
long SystemObject::cForEach(LPVAL theParams)
{
	// Get object parameter
	CRunObjType* pType = theParams[0].GetObjectParam(pCRuntime);
	if (pType == NULL) return 0;

	EventParametersInfo2 ev;
	pCRuntime->GetCurrentEventPtrs(ev);
	const bool cndsAfter = pCRuntime->CndsAfter(ev.pCnds);
	bool copiedSol = false;

	SelObjList* pSol = pType->pSol;
	ObjIterator i, instances_end, instances_begin;

	// Prepare a loop info
	LoopInfo* loopinfo = new LoopInfo;
	loopinfo->index = 1;

	// For family movement selection
	bool isSelectAll = (pSol->selectAll != 0);

	// Unselect all if all selected, and prepare pointers
	if (pSol->selectAll) {
		i = pType->instances.begin();
		instances_begin = pType->instances.begin();
		instances_end = pType->instances.end();
		pSol->selectAll = false;
		loopinfo->max = pType->instances.size();
	}
	else {
		// Copy the SOL: we manually select one at a time so can't iterate 'sol' directly
		forEachStack.push_back(ForEachSolSaver(pType));
		copiedSol = true;

		i = forEachStack.back().mySol.begin();
		instances_begin = forEachStack.back().mySol.begin();
		instances_end = forEachStack.back().mySol.end();
		loopinfo->max = forEachStack.back().mySol.size();
	}

	// Add to the loop stack
	loopstack.push_back(loopinfo);
	__int64& r = loopinfo->index;

	CEventLine* pThisEvent = pCRuntime->pCurrentEvent;

	// If conditions come after this condition, we've potentially got SOL changes happening during
	// the loop.
	if (cndsAfter || pThisEvent->manualSolModifier) {
		// Get a list of this event's modifiers.  It might clean up the SOL for conditions before
		// itself, but that's OK, the loop overhead is fairly big in comparison.
		ObjTypeList* modifiers = pThisEvent->modifiers;
		int modifiesSol = pThisEvent->modifiesSol;

		for ( ; i != instances_end; i++, r++) {

			// Select this instance
			pType->pSol->sol.resize(1);
			pType->pSol->sol.front() = *i;

			// Select siblings
			if (!(*i)->siblings.empty()) {
				ObjIterator j = (*i)->siblings.begin();
				ObjIterator siblings_end = (*i)->siblings.end();

				for ( ; j != siblings_end; j++) {
					(*j)->pType->pSol->sol.resize(1);
					(*j)->pType->pSol->sol.front() = *j;
					(*j)->pType->pSol->selectAll = false;
				}
			}

			// If this is a family, we need to select the corresponding family movement instances too
			if (!pType->familyMovs.empty()) {
				ObjTypeIterator t = pType->familyMovs.begin();
				ObjTypeIterator end = pType->familyMovs.end();
				int index = 0;

				for ( ; t != end; t++, index++) {

					CRunObject* pickInstance;
					
					// If we are selectAll here, the family movement must be as well.  So just pick out the corresponding index.
					if (isSelectAll)
						pickInstance = (*t)->instances[i - instances_begin];
					// Otherwise pick from the SOL in the same place.  Being a linked family movement, we can make these assumptions.
					else
						pickInstance = forEachStack.back().familyMovSols[index][i - instances_begin];

					(*t)->pSol->selectAll = false;
					(*t)->pSol->sol.resize(0);
					(*t)->pSol->sol.push_back(pickInstance);
				}
			}

			// New SOL copy for the conditions following
			if (modifiesSol)
				NewSOLCopy(modifiers);

			// Run For loop conditions, which may change the SOL copy
			if (cndsAfter && ev.pCnds && !pCRuntime->RunConditions(ev.pCnds)) {
				// Revert SOL, may have been changed by RunConditions
				RestoreSOL(modifiers);
				continue;		// next iter
			}

			if (ev.pActs)
				pCRuntime->RunActions(ev.pActs);

			if (ev.pSubEvents)
				pCRuntime->RunSubEvents(ev.pSubEvents);

			// Restore pCurrentEvent incase a function was called and changed it
			pCRuntime->pCurrentEvent = pThisEvent;
				
			// Undo SOL changes by RunConditions
			if (modifiesSol)
				RestoreSOL(modifiers);

#ifdef CONSTRUCT_PREVIEW
			if (r == 1000000) {
				if (WarnInfiniteLoop("for-each", -1)) break;
			}
#endif
		}
	}
	// If no conditions come after this condition, we're free to run without explicit SOL
	// modifications (subevents, if any, will clean up after themselves)
	else {

		for ( ; i != instances_end; i++, r++) {

			// Select this instance & siblings
			pSol->sol.resize(1);
			pSol->sol.front() = *i;

			// Select siblings
			if (!(*i)->siblings.empty()) {
				ObjIterator j = (*i)->siblings.begin();
				ObjIterator siblings_end = (*i)->siblings.end();

				for ( ; j != siblings_end; j++) {
					(*j)->pType->pSol->sol.resize(1);
					(*j)->pType->pSol->sol.front() = *j;
					(*j)->pType->pSol->selectAll = false;
				}
			}

			// If this is a family, we need to select the corresponding family movement instances too
			if (!pType->familyMovs.empty()) {
				ObjTypeIterator t = pType->familyMovs.begin();
				ObjTypeIterator end = pType->familyMovs.end();

				for ( ; t != end; t++) {

					(*t)->pSol->sol.resize(1);
					CRunObject*& pickInstance = (*t)->pSol->sol.front();
					
					// If we are selectAll here, the family movement must be as well.  So just pick out the corresponding index.
					if (isSelectAll)
						pickInstance = (*t)->instances[i - instances_begin];
					// Otherwise pick from the SOL in the same place.  Being a linked family movement, we can make these assumptions.
					else
						pickInstance = (*t)->pSol->sol[i - instances_begin];
				}
			}

			if (ev.pActs)
				pCRuntime->RunActions(ev.pActs);

			if (ev.pSubEvents)
				pCRuntime->RunSubEvents(ev.pSubEvents);

			// Restore the current event
			pCRuntime->pCurrentEvent = pThisEvent;

#ifdef CONSTRUCT_PREVIEW
			if (r == 1000000) {
				if (WarnInfiniteLoop("for-each", -1)) break;
			}
#endif
		}
			

	}//else

	// If we copied the SOL, pop back this SOL
	if (copiedSol)
		forEachStack.pop_back();

	// Free our loop info
	loopstack.pop_back();
	delete loopinfo;

	return false;
}

long SystemObject::cCompareGlobal(LPVAL theParams)
{
	const ExpStore& l = pCRuntime->globalVars[theParams[0].GetInt()];
	const ExpReturn& r = theParams[2];

	return DoComparison(theParams[1].GetInt(), (const ExpBase&)l, (const ExpBase&)r);
}

long SystemObject::cEveryMs(LPVAL theParams)
{
	void* curCnd = pCRuntime->GetCurrentConditionPtr();

	// If first eval of condition, add first entry of now, and allow to run
	map<void*, double>::iterator i = everyEvents.find(curCnd);

	if (i == everyEvents.end()) {
		everyEvents[curCnd] = pLayout->currentClock;
		return true;
	}
	// Else lookup the last time, and check that the given amount of milliseconds have passed.
	else {
		double now = pLayout->currentClock;
		double msInterval = theParams[0].GetDouble();

		double& lastTime = i->second;

		// Enough time elapsed to trigger again?
		if (now - lastTime >= msInterval) {

			// If within 20ms of the interval, we add on the last time.  This keeps continuously
			// repeating events accurately timed.
			if (now - lastTime - msInterval <= 20.0)
				lastTime += msInterval;

			// Otherwise, update the clock to now.  It's been >20ms since we last triggered
			// and we don't want multiple spurious triggers by adding several times.
			else
				lastTime = now;

			return true;
		}
		else return false;
	}
}

long SystemObject::cEveryTicks(LPVAL theParams)
{
	__int64 tickcount = theParams[0].GetInt64();

	if (tickcount == 0)
		return true;
	else
		return pLayout->frameCounter64 % theParams[0].GetInt64() == 0;
}

long SystemObject::cCompareTime(LPVAL theParams)
{
	clock_t now = pCRuntime->GetLayoutClock(pLayout);

	switch (theParams[0].GetInt()) {
	case 0:
		{
			// The clock will rarely identically match the millisecond time
			// compare greater or equal then never run again
			vector<void*>::iterator i = find(expiredEqualities.begin(), expiredEqualities.end(), pCRuntime->GetCurrentConditionPtr());

			// Not found
			if (i == expiredEqualities.end()) {
				if (__int64(now) >= theParams[1].GetInt64()) {
					expiredEqualities.push_back(pCRuntime->GetCurrentConditionPtr());
					return true;
				}
			}
			// Found: expired, return false
			return false;
		}
	case 1:
		return __int64(now) != theParams[1].GetInt64();
	case 2:
		return __int64(now) < theParams[1].GetInt64();
	case 3:
		return __int64(now) <= theParams[1].GetInt64();
	case 4:
		return __int64(now) > theParams[1].GetInt64();
	case 5:
		return __int64(now) >= theParams[1].GetInt64();
	default:
		__assume(0);
	}
}

long SystemObject::cValueIsType(LPVAL theParams)
{
	// Combo for type should match ExpType enum
	return theParams[0].Type() == (ExpType)theParams[1].GetInt();
}

long SystemObject::cPointOverlap(LPVAL theParams)
{
	CRunObjType* pType = theParams[0].GetObjectParam(pCRuntime);
	if (pType == NULL) return false;

	return pCRuntime->QuerySelectPointCollision(theParams[1].GetInt(), theParams[2].GetInt(), pType);
}

long SystemObject::cPointCollision(LPVAL theParams)
{
	// TODO
	return false;
}

long SystemObject::cMakeTrigger(LPVAL theParams)
{
	void* curCnd = pCRuntime->GetCurrentConditionPtr();

	map<void*, __int64>::iterator i = makeTriggers.find(curCnd);

	// Not found: allow 
	if (i == makeTriggers.end()) {
		makeTriggers[curCnd] = pLayout->frameCounter64;
		return true;
	}
	else {
		__int64& lastFrame = i->second;
		__int64 thisFrame = pLayout->frameCounter64;

		// Was last true last frame
		if (thisFrame - lastFrame == 1) {
			lastFrame = thisFrame;		// Also was last true this frame
			return false;				// But don't allow to run this frame
		}
		// Was last true over a frame ago
		else {
			lastFrame = thisFrame;
			return true;
		}
	}
}

// Checks the OR flag is on for OR expansions.  See or_expand.txt.  Also merges saved SOLs.
long SystemObject::cOR_FlagCheck(LPVAL theParams)
{
	int num = theParams[0].GetInt();
	int flag = orFlags[num];
	orFlags[num] = 0;	// Turn off flag

	// Merge SOLs
	if (flag) {

		if (orSolSaves.size() == 0)
			return flag;

		// Set SOLs to first SOL
		vector<SolSave>::iterator ss = orSolSaves.front().begin();
		vector<SolSave>::iterator ss_end = orSolSaves.front().end();

		for ( ; ss != ss_end; ss++) {
			ss->pType->pSol->selectAll = false;
			ss->pType->pSol->sol = ss->selection;
		}

		if (orSolSaves.size() == 1)
			return flag;

		// Now iterate the rest of the SOLs, merging them.
		vector<vector<SolSave>>::iterator s = orSolSaves.begin() + 1;
		vector<vector<SolSave>>::iterator s_end = orSolSaves.end();

		for ( ; s != s_end; s++) {

			// Iterate all SOLs from this event and merge them
			ss = s->begin();
			ss_end = s->end();

			for ( ; ss != ss_end; ss++) {

				// If nothing selected, copy selection
				if (ss->pType->pSol->selectAll) {
					ss->pType->pSol->selectAll = false;
					ss->pType->pSol->sol = ss->selection;
				}
				// Already some selection:  Merge!
				else {

					// Slow merge; todo: use davo's algo
					//OPTIMISE
					ObjIterator i = ss->selection.begin();
					ObjConstIterator selection_end = ss->selection.end();
					SelObjList* pSol = ss->pType->pSol;

					for ( ; i != selection_end; i++) {

						// This object is not selected
						if (!exists_element(pSol->sol.begin(), pSol->sol.end(), *i))
							// Select it
							pSol->sol.push_back(*i);
					}
				}

			} // for each saved SOL
		} // for each event's solSave
		
	}

	return flag;
}

long SystemObject::cMotionBlurSupported(LPVAL params)
{
#ifdef CONSTRUCT_DIRECTX9
	// Motion blur just needs a A16B16G16R16F texture for accumulation
	HRESULT hr = pCRuntime->renderer.GetD3D9()->CheckDeviceFormat(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									D3DFMT_X8R8G8B8,
									D3DUSAGE_RENDERTARGET,
									D3DRTYPE_TEXTURE,
									D3DFMT_A16B16G16R16F);

	return SUCCEEDED(hr);
#else
	return false;
#endif
}

class ForEachOrdered_Instance {
public:
	CRunObject* obj;
	double value;

	// Family movement instances have to sort with this instance
	ObjList familyMovInstances;

	ForEachOrdered_Instance() {}
	ForEachOrdered_Instance(CRunObject* o) : obj(o) {}
	ForEachOrdered_Instance(CRunObject* o, double v) : obj(o), value(v) {}

	// Family constructor; needs to copy in family movement instances to pick in sorted fashion
	ForEachOrdered_Instance(int index, CRunObjType* pType)
	{
		if (pType->pSol->selectAll)
			obj = pType->instances[index];
		else
			obj = pType->pSol->sol[index];

		ObjTypeIterator t = pType->familyMovs.begin();
		ObjTypeIterator end = pType->familyMovs.end();

		for ( ; t != end; t++) {

			if ((*t)->pSol->selectAll)
				familyMovInstances.push_back((*t)->instances[index]);
			else
				familyMovInstances.push_back((*t)->pSol->sol[index]);
		}
	}

	bool operator< (const ForEachOrdered_Instance& other) const { return value < other.value; }
};

// For Each instance of an object type
long SystemObject::cForEachOrder(LPVAL theParams)
{
	// Get object parameter
	CRunObjType* pType = theParams[0].GetObjectParam(pCRuntime);
	if (pType == NULL) return 0;

	bool descending = theParams[2].GetBool();
	SelObjList* pSol = pType->pSol;

	// Prepare a loop info
	LoopInfo* loopinfo = new LoopInfo;
	loopinfo->index = 1;

	ObjList* listToIterate;

	bool isSelectAll = (pSol->selectAll != 0);

	// Create a vector to sort and iterate
	if (pSol->selectAll)
		listToIterate = &(pType->instances);
	else
		listToIterate = &(pSol->sol);

	// Create ordered vector, not yet sorted
	vector<ForEachOrdered_Instance> orderedObjects;
	orderedObjects.reserve(listToIterate->size());

	// Copy the list to iterate to orderedObjects in unsorted order, no values yet evaluated
	ObjIterator i = listToIterate->begin(); 
	ObjIterator end = listToIterate->end();

	if (pType->IsFamily()) {

		for (int index = 0; i != end; i++, index++)
			orderedObjects.push_back(ForEachOrdered_Instance(index, pType));
	}
	else {
		for ( ; i != end; i++)
			orderedObjects.push_back(*i);
	}

	// Prepare SOL with single picked instance
	pSol->selectAll = false;
	pSol->sol.resize(1);

	// Re-iterate orderedObjects, and determine values
	vector<ForEachOrdered_Instance>::iterator oi = orderedObjects.begin();
	vector<ForEachOrdered_Instance>::iterator oend = orderedObjects.end();

	ExpStore e;

	for ( ; oi != oend; oi++) {
		// Reevaluate parameter #1, the object expression, with the current instance
		pCRuntime->ReEvaluateCndParameterWithInstance(1, (ExpReturn*)&e, oi->obj);
		oi->value = e.GetDouble();
	}

	// Sort orderedObjects
	sort(orderedObjects.begin(), orderedObjects.end());

	if (descending)
		reverse(orderedObjects.begin(), orderedObjects.end());

	// Add to the loop stack
	loopstack.push_back(loopinfo);
	__int64& r = loopinfo->index;

	// Now sorted, begin iterations of orderedObjects
	EventParametersInfo2 ev;
	pCRuntime->GetCurrentEventPtrs(ev);
	const bool cndsAfter = pCRuntime->CndsAfter(ev.pCnds);

	oi = orderedObjects.begin();
	oend = orderedObjects.end();
	vector<ForEachOrdered_Instance>::iterator obegin = orderedObjects.begin();

	CEventLine* pThisEvent = pCRuntime->pCurrentEvent;

	// If conditions come after this condition, we've potentially got SOL changes happening during the loop.
	if (cndsAfter || pThisEvent->manualSolModifier) {

		// Get a list of this event's modifiers.  It might clean up the SOL for conditions before
		// itself, but that's OK, the loop overhead is fairly big in comparison.
		ObjTypeList* modifiers = pThisEvent->modifiers;
		int modifiesSol = pThisEvent->modifiesSol;

		for ( ; oi != oend; oi++, r++) {

			CRunObject* o = oi->obj;

			// Select this instance
			pType->pSol->sol.resize(1);
			pType->pSol->sol.front() = o;

			// Select siblings
			if (!o->siblings.empty()) {
				ObjIterator j = o->siblings.begin();
				ObjIterator siblings_end = o->siblings.end();

				for ( ; j != siblings_end; j++) {
					(*j)->pType->pSol->sol.resize(1);
					(*j)->pType->pSol->sol.front() = *j;
					(*j)->pType->pSol->selectAll = false;
				}
			}

			// If this is a family, we need to select the corresponding family movement instances too
			if (!pType->familyMovs.empty()) {
				ObjTypeIterator t = pType->familyMovs.begin();
				ObjTypeIterator end = pType->familyMovs.end();
				int index = 0;

				for ( ; t != end; t++, index++) {

					(*t)->pSol->selectAll = false;
					(*t)->pSol->sol.resize(0);
					(*t)->pSol->sol.push_back(oi->familyMovInstances[index]);
				}
			}

			// New SOL copy for the conditions following
			if (modifiesSol)
				NewSOLCopy(modifiers);

			// Run For loop conditions, which may change the SOL copy
			if (cndsAfter && ev.pCnds && !pCRuntime->RunConditions(ev.pCnds)) {
				// Revert SOL, may have been changed by RunConditions
				RestoreSOL(modifiers);
				continue;		// next iter
			}

			if (ev.pActs)
				pCRuntime->RunActions(ev.pActs);

			if (ev.pSubEvents)
				pCRuntime->RunSubEvents(ev.pSubEvents);

			pCRuntime->pCurrentEvent = pThisEvent;
				
			// Undo SOL changes by RunConditions
			if (modifiesSol)
				RestoreSOL(modifiers);

#ifdef CONSTRUCT_PREVIEW
			if (r == 1000000) {
				if (WarnInfiniteLoop("for-each", -1)) break;
			}
#endif
		}
	}
	// If no conditions come after this condition, we're free to run without explicit SOL
	// modifications (subevents, if any, will clean up after themselves)
	else {

		for ( ; oi != oend; oi++, r++) {

			CRunObject* o = oi->obj;

			// Select this instance
			pType->pSol->sol.resize(1);
			pType->pSol->sol.front() = o;

			// Select siblings
			if (!o->siblings.empty()) {
				ObjIterator j = o->siblings.begin();
				ObjIterator siblings_end = o->siblings.end();

				for ( ; j != siblings_end; j++) {
					(*j)->pType->pSol->sol.resize(1);
					(*j)->pType->pSol->sol.front() = *j;
					(*j)->pType->pSol->selectAll = false;
				}
			}

			// If this is a family, we need to select the corresponding family movement instances too
			if (!pType->familyMovs.empty()) {
				ObjTypeIterator t = pType->familyMovs.begin();
				ObjTypeIterator end = pType->familyMovs.end();
				int index = 0;

				for ( ; t != end; t++, index++) {

					(*t)->pSol->selectAll = false;
					(*t)->pSol->sol.resize(0);
					(*t)->pSol->sol.push_back(oi->familyMovInstances[index]);
				}
			}

			if (ev.pActs)
				pCRuntime->RunActions(ev.pActs);

			if (ev.pSubEvents)
				pCRuntime->RunSubEvents(ev.pSubEvents);

			pCRuntime->pCurrentEvent = pThisEvent;

#ifdef CONSTRUCT_PREVIEW
			if (r == 1000000) {
				if (WarnInfiniteLoop("for-each", -1)) break;
			}
#endif
		}
			

	}

	// Free our loop info
	loopstack.pop_back();
	delete loopinfo;

	return false;
}

long SystemObject::cOnCollisionAdvanced(LPVAL params)
{
	CRunObjType* type_a = params[0].GetObjectParam(pCRuntime);
	bool pick_a = !params[1].GetBool();
	CRunObjType* type_b = params[2].GetObjectParam(pCRuntime);
	bool pick_b = !params[3].GetBool();

	// Point to the list of currently selected instances
	ObjList* a_selected_instances, * b_selected_instances;

	if (type_a->pSol->selectAll)
		a_selected_instances = &(type_a->instances);
	else
		a_selected_instances = &(type_a->pSol->sol);

	if (type_b->pSol->selectAll)
		b_selected_instances = &(type_b->instances);
	else
		b_selected_instances = &(type_b->pSol->sol);

	// Iterate all collision combinations and pick
	ObjIterator a = a_selected_instances->begin();
	ObjIterator a_end = a_selected_instances->end();
	ObjIterator b;
	ObjIterator b_end = b_selected_instances->end();

	bool anythingHappened = false;

	for ( ; a != a_end; a++) {
		for (b = b_selected_instances->begin(); b != b_end; b++) {
			if (pCRuntime->QueryCollision(*a, *b)) {
				anythingHappened = true;

				if (pick_a && !pCRuntime->IsSelectedF(*a, type_a))
					pCRuntime->Select(*a, type_a);
				if (pick_b && !pCRuntime->IsSelectedF(*b, type_b))
					pCRuntime->Select(*b, type_b);
			}
		}
	}

	return MakeConditionTrigger(pCRuntime, anythingHappened);
}

long SystemObject::cIsOverlappingAdvanced(LPVAL params)
{
	CRunObjType* type_a = params[0].GetObjectParam(pCRuntime);
	bool pick_a = !params[1].GetBool();
	CRunObjType* type_b = params[2].GetObjectParam(pCRuntime);
	bool pick_b = !params[3].GetBool();

	// Point to the list of currently selected instances
	ObjList* a_selected_instances, * b_selected_instances;

	if (type_a->pSol->selectAll)
		a_selected_instances = &(type_a->instances);
	else
		a_selected_instances = &(type_a->pSol->sol);

	if (type_b->pSol->selectAll)
		b_selected_instances = &(type_b->instances);
	else
		b_selected_instances = &(type_b->pSol->sol);

	// Iterate all collision combinations and pick
	ObjIterator a = a_selected_instances->begin();
	ObjIterator a_end = a_selected_instances->end();
	ObjIterator b;
	ObjIterator b_end = b_selected_instances->end();

	bool anythingHappened = false;

	for ( ; a != a_end; a++) {
		for (b = b_selected_instances->begin(); b != b_end; b++) {
			if (pCRuntime->QueryCollision(*a, *b)) {
				anythingHappened = true;

				if (pick_a && !pCRuntime->IsSelectedF(*a, type_a))
					pCRuntime->Select(*a, type_a);
				if (pick_b && !pCRuntime->IsSelectedF(*b, type_b))
					pCRuntime->Select(*b, type_b);
			}
		}
	}

	return anythingHappened;
}

long SystemObject::cIsGroupEnabled(LPVAL params)
{
	CString groupName = params[0].GetString();
	groupName.MakeLower();

	map<CString, vector<CEventGroup*> >::iterator i = pCRuntime->groupMap.find(groupName);

	if (i != pCRuntime->groupMap.end()) {

		// There may be multiple groups of this name instantiated in the event list for triggers,
		// but they always have the same state.  Just return the front group's state.
		return i->second.front()->Activated;
	}
#ifdef CONSTRUCT_DEBUGGER
	else {
		CString msg;
		msg.Format("'Is group enabled' condition:  the group name \"%s\" does not exist in the events.  Condition evaluated as false.", groupName);
		pCRuntime->LogMsg(msg);
		return false;
	}
#else
	return false;
#endif
}

long SystemObject::cIsNumberBetween(LPVAL params)
{
	// test a <= x <= b
	double x = params[0].GetDouble();
	double a = params[1].GetDouble();
	double b = params[2].GetDouble();

	return (a <= x) && (x <= b);
}

long SystemObject::cAngleClockwise(LPVAL params)
{
	float a = params[0].GetFloat();
	bool anticlockwise = params[1].GetBool();
	float b = params[2].GetFloat();

	float ca, cb, sa, sb;
	cr::sincosf(RADIANS(a), &sa, &ca);
	cr::sincosf(RADIANS(b), &sb, &cb);

	float z = ca*sb - sa*cb;

	if(z > 0)
		return anticlockwise;
	else
		return !anticlockwise;
}

long SystemObject::cIsLayerVisible(LPVAL params)
{
	CRunLayer* pLayer = GetLayerParam(params[0]);
	if (pLayer == NULL) return false;

	return pLayer->visible;
}

long SystemObject::aFlushClipboard(LPVAL params)
{
	CSystemObj::SetClipboardText("");
	return 0;
}

long SystemObject::aSetClipboard(LPVAL params)
{
	CSystemObj::SetClipboardText(params[0].GetString());
	return 0;
}

long SystemObject::aWriteINI(LPVAL params)
{
	CSystemObj::WriteINI(params[0].GetString(), params[1].GetString(), params[2].GetString(), params[3].GetString());
	return 0;
}

long SystemObject::aPlaySound(LPVAL params)
{
	CSystemObj::PlaySound(params[0].GetString(), params[1].GetString());
	return 0;
}

long SystemObject::aMessageBox(LPVAL params)
{
	CSystemObj::MessageBox(params[0].GetString(), params[1].GetString(), params[2].GetString());
	return 0;
}

long SystemObject::aScrollX(LPVAL params)
{
	pLayout->SetScrollX(params[0].GetFloat() - (pCRuntime->winWidth / 2.0));
	return 0;
}

long SystemObject::aScrollY(LPVAL params)
{
	pLayout->SetScrollY(params[0].GetFloat() - (pCRuntime->winHeight / 2.0));
	return 0;
}

long SystemObject::aSetAngle(LPVAL params)
{
	//pLayout->layers.front()->angle =
	pLayout->display_angle = params[0].GetFloat();
	return 0;
}

long SystemObject::aSetFilter(LPVAL params)
{
	// No longer implemented
	return 0;
}

#ifndef LLONG_MAX 
#define LLONG_MAX 9223372036854775807i64
#endif

long SystemObject::aStartLoop(LPVAL theParams)
{
	// On Loop takes 1 param, the loop name
	ExpReturn cndParamList = theParams[0];
	FASTTRIGGER f = pCRuntime->GetFastTriggerHandle(-1, 8, &cndParamList);	// 8 = On Loop
	if (f == NULL) return 0;	// Nothing to do

	// Get loop name and length
	const CString& loopname = *(theParams[0].GetStringPtr());
	if (&loopname == NULL) return 0;	 // Null reference

	// If length is -1 or less, run "infinite" (just very, very long)
	__int64 lmax = theParams[1].GetInt64();
	const __int64 max = lmax < 0 ? 0x7FFFFFFFFFFFFFFE : lmax;

	// Check if this loop already exists
	LoopsIterator p = loops.find(loopname);
	if (p != loops.end()) throw runtime_error("A nested loop was started with the same name as an already running loop.  Rename your loop to a unique name.");

	// Create loop struct and save
	LoopInfo loop;
	loop.index = 1;
	loop.max = max;
	loops[loopname] = loop;

	// Loop index reference
	p = loops.find(loopname);
	__int64& r = p->second.index;

	loopstack.push_back(&(p->second));

	// There is currently no way of telling the modifiers used by a set of triggers.
	// To ensure no SOL wiping, we have to step up the SOL for all objects.
	// It's a once-per-fastloop call though so it doesn't affect iteration speed.
	pCRuntime->NewSOL();

	// Start executing iterations
	for ( ; r <= max; r++) {
		pCRuntime->GenerateEventFast(f);

#ifdef CONSTRUCT_PREVIEW
		if (r == 1000000) {
			CString name;
			name.Format("on loop %s", loopname);
			if (WarnInfiniteLoop(name, max)) break;
		}
#endif
	}

	pCRuntime->RestoreSOL();

	// Remove loop from loops
	loopstack.pop_back();
	loops.erase(loopname);

	return 0;
}

long SystemObject::aStopLoop(LPVAL theParams)
{
	// Set index to max and the loop will stop
	CString& loopname = *(theParams[0].GetStringPtr());
	if (&loopname == NULL) return 0;

	LoopsConstIterator p = loops.find(loopname);

	if (p != loops.end())
		p->second.index = 0x7FFFFFFFFFFFFFFD; // for will increment another 1

	return 0;
}

long SystemObject::aBreak(LPVAL theParams)
{
	// Stop the top level while
	if (!whileStack.empty())
		whileStack.back() = 0;	// false
	else if (!loopstack.empty())
		loopstack.back()->index = loopstack.back()->max + 1;

	return 0;
}

long SystemObject::aCreate(LPVAL theParams)
{
	CRunLayer* pLayer = GetLayerParam(theParams[1]);
	if (pLayer == NULL) return 0;

	CRunObjType* pType = theParams[0].GetObjectParam(pCRuntime);
	
	CRunObject* obj = pCRuntime->CreateObject(pType, pLayer->number, pLayout);
	if (obj == NULL) return 0;

	pCRuntime->SelectAll(pType);
	pCRuntime->Select(obj, pType);

	obj->info.x = theParams[2].GetFloat();
	obj->info.y = theParams[3].GetFloat();

	pCRuntime->UpdateBoundingBox(obj);

	return 0;
}

long SystemObject::aCreateRelativePP(LPVAL theParams)
{
	CRunLayer* pLayer = GetLayerParam(theParams[1]);
	if (pLayer == NULL) return 0;

	CRunObjType* pType = theParams[0].GetObjectParam(pCRuntime);
	
	CRunObject* obj = pCRuntime->CreateObject(pType, pLayer->number, pLayout);
	if (obj == NULL) return 0;

	pCRuntime->SelectAll(pType);
	pCRuntime->Select(obj, pType);

	CRunObject* relativeObj = theParams[4].GetPairedObjectParam(pRuntime, obj);
	if (relativeObj == NULL) {
		obj->info.x = 0;
		obj->info.y = 0;
	}
	else {
		obj->info.x = relativeObj->info.x + theParams[2].GetFloat();
		obj->info.y = relativeObj->info.y + theParams[3].GetFloat();
	}

	pCRuntime->UpdateBoundingBox(obj);

	return 0;
}

long SystemObject::aCreateRelativeIP(LPVAL theParams)
{
	CRunLayer* pLayer = GetLayerParam(theParams[1]);
	if (pLayer == NULL) return 0;
	
	CRunObjType* pType = theParams[0].GetObjectParam(pCRuntime);

	CRunObject* obj = pCRuntime->CreateObject(pType, pLayer->number, pLayout);
	if (obj == NULL) return 0;

	pCRuntime->SelectAll(pType);
	pCRuntime->Select(obj, pType);

	CRunObject* relativeObj = theParams[4].GetPairedObjectParam(pRuntime, obj);

	POINTF pt;

	if (theParams[5].Type() == EXPTYPE_INTEGER) {
		if (pCRuntime->GetImagePoint(relativeObj, theParams[5].GetInt() - 1, pt)) {
			obj->info.x = pt.x + theParams[2].GetFloat();
			obj->info.y = pt.y + theParams[3].GetFloat();
		}
		else {
			obj->info.x = 0;
			obj->info.y = 0;
		}
	}
	else if (theParams[5].Type() == EXPTYPE_STRING) {
		if (pCRuntime->GetImagePoint(relativeObj, *(theParams[5].GetStringPtr()), pt)) {
			obj->info.x = pt.x + theParams[2].GetFloat();
			obj->info.y = pt.y + theParams[3].GetFloat();
		}
		else {
			obj->info.x = 0;
			obj->info.y = 0;
		}
	}
	else {
		obj->info.x = 0;
		obj->info.y = 0;
	}

	pCRuntime->UpdateBoundingBox(obj);

	return 0;
}

void SystemObject::DoFrameChange(int frameNum, CString transition, int msTime)
{
	transition.MakeLower();

#ifdef CONSTRUCT_DIRECTX9
	if (transition == "" || transition == "none" || frameNum == pLayout->number) {
		// Only allow one call to change frame per frame
		if (pCRuntime->changeFrame == -1) {
			pCRuntime->changeFrame = frameNum;

			// No more frames - quit
			if (pCRuntime->changeFrame >= pCRuntime->Frames.size())
				pCRuntime->bRunning = false;
		}
	}
	else if (pCRuntime->runningTransition == NULL) {

		// Lookup and activate transition
		list<CRunTransition>::iterator i = pCRuntime->Transitions.begin();

		for ( ; i != pCRuntime->Transitions.end(); i++) {
			if (i->name == transition) {
				pCRuntime->runningTransition = &(*i);
				CRunTransition& tr = *(pCRuntime->runningTransition);
				
				tr.fromFrame = pLayout;
				tr.msDuration = msTime;
				tr.msStart = pCRuntime->GetAppClock();
				tr.progress = 0.0f;
				tr.toFrame = pCRuntime->CreateRunningFrame(frameNum);

				// Single layout preview - quit
				if (tr.toFrame == NULL) {
					pCRuntime->bRunning = false;
					return;
				}

				// Transition draws these frames.
				tr.toFrame->systemDrawn = false;
				tr.fromFrame->systemDrawn = false;

				return;
			}
		}
#endif

		// Not found: ordinary frame change

		// Only allow one call to change frame per frame
		if (pCRuntime->changeFrame == -1) {
			pCRuntime->changeFrame = frameNum;

			// No more frames - quit
			if (pCRuntime->changeFrame >= pCRuntime->Frames.size() || pCRuntime->changeFrame < 0)
				pCRuntime->bRunning = false;
		}

#ifdef CONSTRUCT_DIRECTX9
	}
#endif
}

long SystemObject::aNextFrame(LPVAL theParams)
{
	DoFrameChange(pLayout->number + 1, theParams[0].GetString(), theParams[1].GetInt());
	return 0;
}

long SystemObject::aPrevFrame(LPVAL theParams)
{
	DoFrameChange(pLayout->number - 1, theParams[0].GetString(), theParams[1].GetInt());
	return 0;
}

long SystemObject::aGotoFrame(LPVAL theParams)
{
	if (theParams[0].Type() == EXPTYPE_STRING) {
		CString layoutName = *(theParams[0].GetStringPtr());
		layoutName.MakeLower();

		vector<CRunLayout*>::iterator f = pCRuntime->Frames.begin();

		for ( ; f != pCRuntime->Frames.end(); f++) {
			CString curName = (*f)->name;
			curName.MakeLower();

			if (layoutName == curName)
				DoFrameChange((*f)->number, theParams[1].GetString(), theParams[2].GetInt());
		}
	}
	else
		DoFrameChange(theParams[0].GetInt() - 1, theParams[1].GetString(), theParams[2].GetInt());

	return 0;
}

long SystemObject::aQuit(LPVAL theParams)
{
	// Falsify bRunning so the main while loop breaks
#if defined(_DEBUG) && !defined(APPRUNTIME)
	pCRuntime->renderer.SaveNextBatchLogToFile("C:\\batch.txt");
#else
	pCRuntime->bRunning = false;
#endif
	return 0;
}

long SystemObject::aCancelClose(LPVAL theParams)
{
	// Reset bRunning so the main while loop continues to run.
	pCRuntime->bRunning = true;
	return 0;
}

long SystemObject::aSetFPS(LPVAL theParams)
{
	int fps = theParams[0].GetInt();
	if (fps < 1) fps = 1;
	pCRuntime->userFps = fps;
	return 0;
}

long SystemObject::aSetGlobal(LPVAL theParams)
{
	pCRuntime->globalVars[theParams[0].GetInt()] = theParams[1];
	return 0;
}

long SystemObject::aAddGlobal(LPVAL theParams)
{
	pCRuntime->globalVars[theParams[0].GetInt()] += theParams[1];
	return 0;
}

long SystemObject::aSubGlobal(LPVAL theParams)
{
	pCRuntime->globalVars[theParams[0].GetInt()] -= theParams[1];
	return 0;
}

long SystemObject::aEnableGroup(LPVAL theParams)
{
	CString groupName = theParams[1].GetString();
	groupName.MakeLower();

	map<CString, vector<CEventGroup*> >::iterator i = pCRuntime->groupMap.find(groupName);

	if (i != pCRuntime->groupMap.end()) {

		bool status = theParams[0].GetBool();
		vector<CEventGroup*>::iterator g = i->second.begin();
		vector<CEventGroup*>::iterator end = i->second.end();

		for ( ; g != end; g++)
			(*g)->Activated = status;
	}
#ifdef CONSTRUCT_DEBUGGER
	else {
		CString msg;
		msg.Format("'Enable group' action:  the group name \"%s\" does not exist in the events.", groupName);
		pCRuntime->LogMsg(msg);
	}
#endif

	return 0;
}

/*
long SystemObject::aDisableGroup(LPVAL theParams)
{
	CString groupName = theParams[0].GetString();
	groupName.MakeLower();

	map<CString, CEventGroup*>::iterator i = pCRuntime->groupMap.find(groupName);

	if (i != pCRuntime->groupMap.end())
		i->second->Activated = false;

	return 0;
}
*/

long SystemObject::aScrollObj(LPVAL theParams)
{
	CRunObjType* pType = theParams[0].GetObjectParam(pCRuntime);
	if (pType == NULL) return 0;
	CRunObject* obj = pCRuntime->GetFirstInstance(pType);
	if (obj == NULL) return 0;

	pLayout->SetScrollX(obj->info.x  - (pCRuntime->winWidth / 2));
	pLayout->SetScrollY(obj->info.y  - (pCRuntime->winHeight / 2));
	return 0;
}

// Set the OR flag on.  See or_expand.txt.  Also saves the SOL for merging later.
long SystemObject::aOrFlagOn(LPVAL theParams)
{
	int num = theParams[0].GetInt();
	int oldFlag = orFlags[num];
	orFlags[num] = true;

	// First flag on?  Clear the saved SOLs
	if (!oldFlag)
		orSolSaves.resize(0);

	// Save any SOLs that were picked from.  Only check any which may have been modified (the modifiers list).
	if (pCRuntime->pCurrentEvent->modifiesSol) {
		ObjTypeIterator t = pCRuntime->pCurrentEvent->modifiers->begin();
		ObjTypeConstIterator objects_end = pCRuntime->pCurrentEvent->modifiers->end();

		orSolSaves.push_back(vector<SolSave>());
		vector<SolSave>& curSave = orSolSaves.back();

		for ( ; t != objects_end; t++) {

			if (!(*t)->pSol->selectAll) {
				curSave.push_back(SolSave());
				SolSave& ss = curSave.back();
				ss.pType = *t;
				ss.selection = (*t)->pSol->sol;

				// Save all container SOLs too
				ObjTypeIterator s = (*t)->sisterTypes.begin();
				ObjTypeIterator sis_end = (*t)->sisterTypes.end();

				for ( ; s != sis_end; s++) {
					curSave.push_back(SolSave());
					SolSave& ss = curSave.back();
					ss.pType = *s;
					ss.selection = (*s)->pSol->sol;
				}
			}
		}
	}

	return 0;
}

long SystemObject::aSerialize(LPVAL params)
{
	// 0 = load, 1 = save
	int mode = params[0].GetInt();
	CString path = params[1].GetString();

	if (mode == 0) {
		// Have to delay
		loading = true;
		loadPath = path;

		// Mark all nonserializing objects as dead so objects don't keep pointers to them (dropping them in OnFrame2)
		for (ObjTypeIterator t = pCRuntime->objects.begin(); t != pCRuntime->objects.end(); t++) {
			if (!(*t)->noSerialize)
				(*t)->MarkAllDestroying();
		}
	}
	else if (mode == 1) {
		saving = true;
		loadPath = path;
	}

	return 0;
}

long SystemObject::aFxActivate(LPVAL theParams)
{
#ifdef CONSTRUCT_DIRECTX9
	CRunObjType* pType = pCRuntime->objects[theParams[0].GetInt()];
	int effectIndex = theParams[1].GetInt();

	// Iterate all selected objects
	CRunObject** objList;
	int count;
	pCRuntime->GetTypeSelectedInstances(pType, objList, count);

	if (objList == NULL || count <= 0) return 0;

	CRunObject** i = objList;
	CRunObject** end = objList + count;

	for ( ; i != end; i++) {

		// Disable
		(*i)->effects[effectIndex].enabled = true;
		
	}
#endif
	return 0;
}

long SystemObject::aFxDeactivate(LPVAL theParams)
{
#ifdef CONSTRUCT_DIRECTX9
	CRunObjType* pType = pCRuntime->objects[theParams[0].GetInt()];
	int effectIndex = theParams[1].GetInt();

	// Iterate all selected objects
	CRunObject** objList;
	int count;
	pCRuntime->GetTypeSelectedInstances(pType, objList, count);

	if (objList == NULL || count <= 0) return 0;

	CRunObject** i = objList;
	CRunObject** end = objList + count;

	for ( ; i != end; i++) {

		// Disable
		(*i)->effects[effectIndex].enabled = false;
		
	}
#endif
	return 0;
}

long SystemObject::aFxSetParam(LPVAL theParams)
{
#ifdef CONSTRUCT_DIRECTX9
	double paramValue = theParams[0].GetDouble();
	CRunObjType* pType = pCRuntime->objects[theParams[1].GetInt()];
	int effectIndex = theParams[2].GetInt();
	int paramIndex = theParams[3].GetInt();

	// Iterate all selected objects
	CRunObject** objList;
	int count;
	pCRuntime->GetTypeSelectedInstances(pType, objList, count);

	if (objList == NULL || count <= 0) return 0;

	CRunObject** i = objList;
	CRunObject** end = objList + count;

	for ( ; i != end; i++) {

		if (effectIndex >= (*i)->effects.size())
			continue;

		CRunEffect* pFx = (*i)->effects[effectIndex].fx;
		CRunEffectParameter& curParam = (*i)->effects[effectIndex].params[paramIndex];
		// Update param
		if (curParam.type == 5) // Float
			curParam.fValue = paramValue;
		else if (curParam.type == 9) // Percent
			curParam.fValue = paramValue / 100.0;
	}
#endif
	return 0;
}

long SystemObject::aEndModalFrame(LPVAL theParams)
{
	pLayout->modal = false;
	pLayout->selfdestruct = true;
	return 0;
}

long SystemObject::aSetLayerScrollXRatio(LPVAL theParams)
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	if (pLayer == NULL) return 0;

	pLayer->scrollXf = theParams[1].GetFloat() / 100.0f;
	return 0;
}

long SystemObject::aSetLayerScrollYRatio(LPVAL theParams)
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	if (pLayer == NULL) return 0;

	pLayer->scrollYf = theParams[1].GetFloat() / 100.0f;
	return 0;
}

long SystemObject::aSetLayerZoomOffset(LPVAL theParams)
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	if (pLayer == NULL) return 0;

	pLayer->zoomXoffset = theParams[1].GetFloat() / 100.0f;
	pLayer->zoomYoffset = theParams[2].GetFloat() / 100.0f;

	if (!pLayout->unboundedScrolling) {
		float minZoomX = (float)pLayout->winWidth / (float)pLayout->width;
		float minZoomY = (float)pLayout->winHeight / (float)pLayout->height;

		if (minZoomX < minZoomY)
			minZoomX = minZoomY;
		else
			minZoomY = minZoomX;

		float minLayerZoomX = minZoomX / pLayout->zoomX;
		float minLayerZoomY = minZoomY / pLayout->zoomY;

		if (pLayer->zoomXoffset < minLayerZoomX)
			pLayer->zoomXoffset = minLayerZoomX;
		if (pLayer->zoomYoffset < minLayerZoomY)
			pLayer->zoomYoffset = minLayerZoomY;
	}

	return 0;
}

long SystemObject::aSetLayerScrollXOffset( LPVAL theParams )
{
	return 0;
}

long SystemObject::aSetLayerScrollYOffset( LPVAL theParams )
{
	return 0;
}

long SystemObject::aSetLayerZoomRate( LPVAL theParams )
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	if (pLayer == NULL) return 0;

	pLayer->zoomXf = theParams[1].GetFloat() / 100.0f;
	pLayer->zoomYf = theParams[2].GetFloat() / 100.0f;

	return 0;
}

long SystemObject::aSetLayerVisible(LPVAL theParams)
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	if (pLayer == NULL) return 0;
	
	pLayer->visible = theParams[1].GetBool();

	return 0;
}

long SystemObject::aSetLayerOpacity(LPVAL theParams)
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	if (pLayer == NULL) return 0;

	double opacity = theParams[1].GetDouble();
	if (opacity < 0) opacity = 0;
	if (opacity > 100) opacity = 100;
	
	pLayer->opacity = opacity;
	pLayer->UpdateFilter();

	return 0;
}

long SystemObject::aSetLayerFilter(LPVAL theParams)
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	if (pLayer == NULL) return 0;

	D3DCOLOR dc = theParams[1].GetD3DColor();

	pLayer->colourfilter = theParams[1].GetCrColor();
	pLayer->UpdateFilter();

	return 0;
}

long SystemObject::aSetTimeScale(LPVAL theParams)
{
	pCRuntime->timeScale = theParams[0].GetDouble();

	LBound(pCRuntime->timeScale, 0.0);

	return 0;
}

long SystemObject::aSetLayerAngle(LPVAL theParams)
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	if (pLayer == NULL) return 0;

	pLayer->angle = theParams[1].GetFloat();
	return 0;
}

long SystemObject::aToggleGroup(LPVAL theParams)
{
	CString groupName = theParams[0].GetString();
	groupName.MakeLower();

	map<CString, vector<CEventGroup*> >::iterator i = pCRuntime->groupMap.find(groupName);

	if (i != pCRuntime->groupMap.end()) {

		bool was_activated = i->second.front()->Activated != 0;

		vector<CEventGroup*>::iterator g = i->second.begin();
		vector<CEventGroup*>::iterator end = i->second.end();

		for ( ; g != end; g++)
			(*g)->Activated = !was_activated;
	}
#ifdef CONSTRUCT_DEBUGGER
	else {
		CString msg;
		msg.Format("'Toggle group' action:  the group name \"%s\" does not exist in the events.", groupName);
		pCRuntime->LogMsg(msg);
	}
#endif

	return 0;
}

long SystemObject::aLoadTextures(LPVAL theParams)
{
	bool load_textures = theParams[0].GetBool();
	CRunLayout* pLayout = GetLayoutParam(theParams[1]);

	if (pLayout == NULL)
		return 0;

	if (load_textures)
		pLayout->LoadLayoutTextures();
	else
		pLayout->FreeLayoutTextures(true);		// Force these textures freed

	return 0;
}

long SystemObject::aAddToAttribute(LPVAL theParams)
{
	CRunObjType* pType = theParams[0].GetObjectParam(pCRuntime);
	bool add = theParams[1].GetBool();
	CRunObjType* pAttribute = theParams[2].GetObjectParam(pCRuntime);

	SelObjList* pSol = pType->pSol;
	ObjIterator i, instances_end, instances_begin;
	if (pSol->selectAll) {
		i = instances_begin = pType->instances.begin();
		instances_end = pType->instances.end();
	}
	else {
		i = instances_begin = pType->sols.back().sol.begin();
		instances_end = pType->sols.back().sol.end();
	}

	if( add)
	{
		for(; i!=instances_end; i++)
		{
			push_back_unique(pAttribute->instances, *i);
		}
	}
	else
	{

	}

	return 0;
}


long SystemObject::aEnableLayerEffect(LPVAL theParams)
{
#ifndef APPRUNTIME
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	bool enabled = theParams[1].GetBool();
	CString effect_name = theParams[2].GetString();
	
	EffectList::iterator e = pLayer->effects.begin();
	EffectList::iterator e_end = pLayer->effects.end();
	for(; e !=  e_end; ++e)
	{
		if(e->fx->name.CompareNoCase(effect_name) == 0)
			e->enabled = enabled;
	}
#endif
	return 0;
}

long SystemObject::aSetLayerEffectParam(LPVAL theParams)
{
#ifndef APPRUNTIME
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	CString effect_name = theParams[1].GetString();
	CString effect_param_name = theParams[2].GetString();
	float value = theParams[3].GetFloat();
	
	EffectList::iterator e = pLayer->effects.begin();
	EffectList::iterator e_end = pLayer->effects.end();
	for(; e !=  e_end; ++e)
	{
		if(e->fx->name.CompareNoCase(effect_name) == 0)
		{
			vector<CRunEffectParameter>::iterator p = e->params.begin();
			vector<CRunEffectParameter>::iterator p_end = e->params.end();
			
			for(; p!= p_end; ++p)
			{
				if(p->name.CompareNoCase(effect_param_name) == 0)
				{
					p->fValue = value;
					p->value.Format("%f", value);
				}	
			}
		}
	}
#endif
	return 0;
}




/*
long SystemObject::aSetMultisampling(LPVAL theParams)
{
	int multisamples = theParams[0].GetInt();

	if (multisamples != pCRuntime->multisamples) {

		// Reset device
		changeresolution = true;
		changeResWidth = pCRuntime->winWidth;
		changeResHeight = pCRuntime->winHeight;
		
		changeMultisampling = multisamples;
	}

	return 0;
}
*/

float SystemObject::GetMinLayerZoomX() const
{
	float result = pLayout->layers.front()->zoomXoffset;
	vector<CRunLayer*>::iterator y = pLayout->layers.begin() + 1;

	for ( ; y != pLayout->layers.end(); y++) {
		if ((*y)->zoomXf < result)
			result = (*y)->zoomXoffset;
	}

	return result;
}

float SystemObject::GetMinLayerZoomY() const
{
	float result = pLayout->layers.front()->zoomYoffset;
	vector<CRunLayer*>::iterator y = pLayout->layers.begin() + 1;

	for ( ; y != pLayout->layers.end(); y++) {
		if ((*y)->zoomYf < result)
			result = (*y)->zoomYoffset;
	}

	return result;
}

long SystemObject::aSetZoom(LPVAL params)
{
	pLayout->zoomX = params[0].GetDouble()/100.0;
	pLayout->zoomY = params[1].GetDouble()/100.0;

	if (!pLayout->unboundedScrolling) {
		float minZoomX = (float)pLayout->winWidth / (float)pLayout->width;
		float minZoomY = (float)pLayout->winHeight / (float)pLayout->height;

		if (minZoomX < minZoomY)
			minZoomX = minZoomY;
		else
			minZoomY = minZoomX;

		float minDisplayZoomX = minZoomX / GetMinLayerZoomX();
		float minDisplayZoomY = minZoomY / GetMinLayerZoomY();

		if (pLayout->zoomX < minDisplayZoomX) {
			pLayout->zoomX = minDisplayZoomX;
		}
		if (pLayout->zoomY < minDisplayZoomX) {
			pLayout->zoomY = minDisplayZoomX;
		}
	}

	return 0;
}

long SystemObject::aSetLayoutSize(LPVAL params)
{
	pLayout->width = params[0].GetInt();
	pLayout->height = params[1].GetInt();

	return 0;
}

long SystemObject::aSetResolution(LPVAL params)
{
	changeresolution = true;
	changeResWidth = params[0].GetInt();
	changeResHeight = params[1].GetInt();
	//changeResFullscreen = -1;	// No change

	return 0;
}

long SystemObject::aSetFullscreen(LPVAL params)
{
	changeresolution = true;
	changeResFullscreen = params[0].GetBool();
	//changeResFullscreen = -1;
	//changeResWidth = pCRuntime->winWidth - 1;
	//changeResHeight = pCRuntime->winHeight - 1;

	return 0;
}

long SystemObject::aQuicksave(LPVAL params)
{
	bool isLoading = params[0].GetBool();

	if (isLoading) {
		quicksave.reset();
		loading = true;
		loadPath = "<quicksave>";

		// Mark all nonserializing objects as dead so objects dont keep pointers to them (dropping them in OnFrame2)
		for (ObjTypeIterator t = pCRuntime->objects.begin(); t != pCRuntime->objects.end(); t++) {
			if (!(*t)->noSerialize)
				(*t)->MarkAllDestroying();
		}
	}
	else {
		quicksave.clear();
		saving = true;
		loadPath = "<quicksave>";
	}

	return 0;
}

long SystemObject::aSetMotionBlur(LPVAL params)
{
#ifdef CONSTRUCT_DIRECTX9
	int motionBlurCombo[] = {
		0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20
	};

	int i = params[0].GetInt();
	if (i < 0) i = 0;
	if (i > 11) i = 11;
	int c = motionBlurCombo[i];

	if (c == 0) {
		// Delay turning off motion blur
		pCRuntime->turnOffMotionBlur = true;
		pCRuntime->turnOnMotionBlur = false;
	}
	else {
		pCRuntime->InitMotionBlur();
		pCRuntime->turnOnMotionBlur = true;
		pCRuntime->turnOffMotionBlur = false;
		pCRuntime->blurNumSteps = c;
	}

#endif
	return 0;
}

long SystemObject::eMouseX(LPVAL params, ExpReturn& ret)
{
	// If a parameter is supplied, use it as a layer param
	if (params[0].Type() != EXPTYPE_NULL) {
		CRunLayer* pLayer = GetLayerParam(params[0]);

		if (pLayer == NULL)
			return 0;

		float unused = 0;

#ifdef CONSTRUCT_DIRECTX9
		if (pCRuntime->motionBlur) {

			// Use interpolated mouse coords
			float mouseX = pCRuntime->linearMouseX;
			pCRuntime->ScreenToLayer(pLayout, pLayer->number, mouseX, unused);
			return ret = mouseX;

		}
		else {
#endif
			POINT mouse;
			GetCursorPos(&mouse);

			if (!pCRuntime->fullscreen)
				ScreenToClient(pCRuntime->hWnds.front(), &mouse);

			float mouseX = mouse.x;
			pCRuntime->ScreenToLayer(pLayout, pLayer->number, mouseX, unused);
			return ret = mouseX;
#ifdef CONSTRUCT_DIRECTX9
		}
#endif
	}

	// Get mouse X coord
	return ret = pLayout->mouseX;
}

long SystemObject::eMouseY(LPVAL params, ExpReturn& ret)
{
	// If a parameter is supplied, use it as a layer param
	if (params[0].Type() != EXPTYPE_NULL) {
		CRunLayer* pLayer = GetLayerParam(params[0]);

		if (pLayer == NULL)
			return 0;

		float unused = 0;

#ifdef CONSTRUCT_DIRECTX9
		if (pCRuntime->motionBlur) {

			// Use interpolated mouse coords
			float mouseY = pCRuntime->linearMouseY;
			pCRuntime->ScreenToLayer(pLayout, pLayer->number, unused, mouseY);
			return ret = mouseY;

		}
		else {
#endif
			POINT mouse;
			GetCursorPos(&mouse);

			if (!pCRuntime->fullscreen)
				ScreenToClient(pCRuntime->GetFrameHwnd(pLayout), &mouse);

			float mouseY = mouse.y;
			pCRuntime->ScreenToLayer(pLayout, pLayer->number, unused, mouseY);
			return ret = mouseY;
#ifdef CONSTRUCT_DIRECTX9
		}
#endif
	}

	// Get mouse Y coord
	return ret = pLayout->mouseY;
}

long SystemObject::eIndex(LPVAL theParams, ExpReturn& ret)
{
	// No parameter: get top level loop index
	if (theParams[0].Type() == EXPTYPE_NULL) {
		
		if (loopstack.empty())
			return ret = -1;
		else
			return ret = loopstack.back()->index;
	}
	// Otherwise use parameter
	else {
		CString* str = theParams[0].GetStringPtr();
		if (str == NULL) return ret = -1;

		LoopsConstIterator p = loops.find(*str);

		if (p != loops.end())
			return ret = p->second.index;
		else
			return ret = -1;
	}
}

long SystemObject::eLoopLength(LPVAL theParams, ExpReturn& ret)
{
	// No parameter: get top level loop index
	if (theParams[0].Type() == EXPTYPE_NULL) {

		if (loopstack.empty())
			return ret = -1;
		else
			return ret = loopstack.back()->max;
	}
	// Otherwise use parameter
	else {
		CString* str = theParams[0].GetStringPtr();
		if (str == NULL) return ret = -1;

		LoopsConstIterator p = loops.find(*str);

		if (p != loops.end())
			return ret = p->second.max;
		else
			return ret = -1;
	}
}

long SystemObject::eMax(LPVAL theParams, ExpReturn& ret)
{
	int nParams = SysGetNumParams(theParams);

	// Start off with param 1
	double maxval = theParams[0].GetDouble();

	// Loop other parameters
	LPVAL ptr = theParams;
	ptr++;

	for ( ; ptr->Type() != EXPTYPE_NULL; ptr++) {
		double curval = ptr->GetDouble();
		if (curval > maxval)
			maxval = curval;
	}

	return ret = maxval;
}

long SystemObject::eMin(LPVAL theParams, ExpReturn& ret)
{
	int nParams = SysGetNumParams(theParams);

	// Start off with param 1
	double minval = theParams[0].GetDouble();

	// Loop other parameters
	LPVAL ptr = theParams;
	ptr++;

	for ( ; ptr->Type() != EXPTYPE_NULL; ptr++) {
		double curval = ptr->GetDouble();
		if (curval < minval)
			minval = curval;
	}

	return ret = minval;
}

long SystemObject::eTotalObjects(LPVAL theParams, ExpReturn& ret)
{
	// No parameters
	if (theParams[0].Type() == EXPTYPE_NULL)
		return ret = pCRuntime->totalObjects;

	// A layer parameter passed
	else {
		CRunLayer* pLayer = GetLayerParam(theParams[0]);

		if (pLayer == NULL)
			return ret = pCRuntime->totalObjects;
		else
			return ret = pLayer->instances.size();
	}
}

long SystemObject::eGlobalValue(LPVAL theParams, ExpReturn& ret)
{
	return ret.ReturnCustom(pCRuntime, pCRuntime->globalVars[theParams[0].eData.vni.varIndex]);
}

long SystemObject::eFormatDecimal(LPVAL theParams, ExpReturn& ret)
{
	// gcvt to n decimal places
	char strbuf[32];
	_gcvt(theParams[0].GetDouble(), theParams[1].GetInt(), strbuf);

	return ret.ReturnString(pCRuntime, strbuf);
}

long SystemObject::ePadZero(LPVAL theParams, ExpReturn& ret)
{
	CString num = theParams[0].GetString();
	CString str;
	int digits = theParams[1].GetInt();
	int spare = digits - num.GetLength();

	for (int i = 0; i < spare; i++)
		str += '0';

	str += num;
	return ret.ReturnString(pCRuntime, str);
}

long SystemObject::eNewLine(LPVAL theParams, ExpReturn& ret)
{
	return ret.ReturnString(pCRuntime, "\n");
}

//Params: string str, int leftcount
long SystemObject::eLeftStr(LPVAL theParams, ExpReturn& ret)
{
	CString* strPtr = theParams[0].GetStringPtr();
	if (strPtr == NULL) return ret.ReturnString(pCRuntime, "");

	return ret.ReturnString(pCRuntime, strPtr->Left(theParams[1].GetInt()));
}

long SystemObject::eRightStr(LPVAL theParams, ExpReturn& ret)
{
	CString* strPtr = theParams[0].GetStringPtr();
	if (strPtr == NULL) return ret.ReturnString(pCRuntime, "");

	return ret.ReturnString(pCRuntime, strPtr->Right(theParams[1].GetInt()));
}

long SystemObject::eMidStr(LPVAL theParams, ExpReturn& ret)
{
	CString* strPtr = theParams[0].GetStringPtr();
	if (strPtr == NULL) return ret.ReturnString(pCRuntime, "");

	return ret.ReturnString(pCRuntime, strPtr->Mid(theParams[1].GetInt() - 1, theParams[2].GetInt()));
}

long SystemObject::eUpperCase(LPVAL theParams, ExpReturn& ret)
{
	CString* strPtr = theParams[0].GetStringPtr();
	if (strPtr == NULL) return ret.ReturnString(pCRuntime, "");

	// Get the return string location to avoid unncessary copies
	CString* strRet = ret.GetReturnString(pRuntime);
	*strRet = *strPtr;
	strRet->MakeUpper();
	return 0;
}

long SystemObject::eLowerCase(LPVAL theParams, ExpReturn& ret)
{
	CString* strPtr = theParams[0].GetStringPtr();
	if (strPtr == NULL) return ret.ReturnString(pCRuntime, "");

	// Get the return string location to avoid unncessary copies
	CString* strRet = ret.GetReturnString(pRuntime);
	*strRet = *strPtr;
	strRet->MakeLower();
	return 0;
}

long SystemObject::eFind(LPVAL theParams, ExpReturn& ret)
{
	CString* str = theParams[0].GetStringPtr();
	if (str == NULL) return ret = -1;

	int start = 0;

	// Start point specified
	if (SysGetNumParams(theParams) == 3)
		start = theParams[2].GetInt() - 1;

	return ret = str->Find(theParams[1].GetString(), start) + 1;
}

long SystemObject::eRemove(LPVAL theParams, ExpReturn& ret)
{
	CString* strPtr = theParams[0].GetStringPtr();
	if (strPtr == NULL || theParams[1].GetStringPtr() == NULL) return ret.ReturnString(pCRuntime, "");

	strPtr->Replace(*(theParams[1].GetStringPtr()), "");

	return ret = theParams[0];
}

long SystemObject::eReplace(LPVAL theParams, ExpReturn& ret)
{
	CString* strPtr = theParams[0].GetStringPtr();
	if (strPtr == NULL || theParams[1].GetStringPtr() == NULL || theParams[2].GetStringPtr() == NULL) return ret.ReturnString(pCRuntime, "");

	strPtr->Replace(*(theParams[1].GetStringPtr()), *(theParams[2].GetStringPtr()));

	// Multiple string parameters: invoke copy semantics
	return ret = theParams[0];
}

long SystemObject::eTrim(LPVAL theParams, ExpReturn& ret)
{
	CString* strPtr = theParams[0].GetStringPtr();
	if (strPtr == NULL) return ret.ReturnString(pCRuntime, "");

//#ifndef VC6  // screw VC6 users
	strPtr->Trim();
//#endif
	return ret = theParams[0];
}

long SystemObject::eReverse(LPVAL theParams, ExpReturn& ret)
{
	CString* strPtr = theParams[0].GetStringPtr();
	if (strPtr == NULL) return ret.ReturnString(pCRuntime, "");

	strPtr->MakeReverse();

	return ret = theParams[0];
}

// Number of tokens: string str, string delim
long SystemObject::eNumTokens(LPVAL theParams, ExpReturn& ret)
{
	CString* strPtr = theParams[0].GetStringPtr();
	if (strPtr == NULL) return ret = -1;

	CString* delimPtr = theParams[1].GetStringPtr();
	TCHAR delim = ',';

	if (delimPtr != NULL)
		delim = (*delimPtr)[0];
	
	int findPos = 0;
	int count = 0;

	do {
		findPos = strPtr->Find(delim, findPos);
		if (findPos != -1) {
			count++;
			findPos++;	// skip delim
		}
	} while (findPos != -1);

	return ret = count + 1;
}

// Extract substring: string str, int index, (char delim)
long SystemObject::eExtractSubstring(LPVAL theParams, ExpReturn& ret)
{
	// Check for 3rd delim parameter, else use comma
	int numParams = SysGetNumParams(theParams);
	TCHAR delim = ',';

	CString* delimPtr = theParams[2].GetStringPtr();

	if (delimPtr != NULL)
		delim = (*delimPtr)[0];

	if (theParams[0].GetStringPtr() == NULL)
		return ret.ReturnString(pCRuntime, "");

	const CString& full = *(theParams[0].GetStringPtr());
	CString part;

	AfxExtractSubString(part, full, theParams[1].GetInt()-1, delim);

	return ret.ReturnString(pCRuntime, part);
}

long SystemObject::eDistance(LPVAL theParams, ExpReturn& ret)
{
	// 2 parameters - find distance for arrays {x,y}, {x,y}
	// 4 parameters - find distance for x,y,x,y

	int numParams = SysGetNumParams(theParams);

	double x1 = 0, y1 = 0, x2 = 0, y2 = 0;

	if (numParams == 2) {
		if (theParams[0].GetArraySize() == 2) {
			const ExpStore* arr = theParams[0].GetArray();
			x1 = arr[0].GetDouble();
			y1 = arr[1].GetDouble();
		}
		if (theParams[1].GetArraySize() == 2) {
			const ExpStore* arr = theParams[1].GetArray();
			x1 = arr[0].GetDouble();
			y1 = arr[1].GetDouble();
		}
	}
	else if (numParams == 4) {
		x1 = theParams[0].GetDouble();
		y1 = theParams[1].GetDouble();
		x2 = theParams[2].GetDouble();
		y2 = theParams[3].GetDouble();
	}

	double dx = dif(x1, x2);
	double dy = dif(y1, y2);

	return ret = sqrt(dx*dx + dy*dy);
}

long SystemObject::eAngle(LPVAL theParams, ExpReturn& ret)
{
	// 2 parameters - find angle for arrays {x,y}, {x,y}
	// 4 parameters - find angle for x,y,x,y

	int numParams = SysGetNumParams(theParams);

	double x1 = 0, y1 = 0, x2 = 0, y2 = 0;

	if (numParams == 2) {
		if (theParams[0].GetArraySize() == 2) {
			const ExpStore* arr = theParams[0].GetArray();
			x1 = arr[0].GetDouble();
			y1 = arr[1].GetDouble();
		}
		if (theParams[1].GetArraySize() == 2) {
			const ExpStore* arr = theParams[1].GetArray();
			x1 = arr[0].GetDouble();
			y1 = arr[1].GetDouble();
		}
	}
	else if (numParams == 4) {
		x1 = theParams[0].GetDouble();
		y1 = theParams[1].GetDouble();
		x2 = theParams[2].GetDouble();
		y2 = theParams[3].GetDouble();
	}

	return ret = DEGREES(atan2(y2 - y1, x2 - x1));
}

long SystemObject::eCountMatching(LPVAL theParams, ExpReturn& ret)
{
	// Convert name or OID to obj type
	CRunObjType* pType;

	if (theParams[0].Type() == EXPTYPE_INTEGER)
		pType = pCRuntime->GetTypeFromOID(theParams[0].GetInt());
	else if (theParams[0].Type() == EXPTYPE_STRING)
		pType = pCRuntime->GetTypeFromName(*(theParams[0].GetStringPtr()));
	else
		return ret = 0;

	if (pType == NULL)
		return ret = 0;

	if (pType->pSol->selectAll)
		return ret = pType->instances.size();
	else
		return ret = pType->pSol->sol.size();
}

long SystemObject::eVarIndex(LPVAL theParams, ExpReturn& ret)
{
	// Get var index and OID
	int varIndex = theParams[0].GetInt();
	int oid = theParams[1].GetInt();

	// Get object type
	CRunObjType* pType = pCRuntime->objects[oid];

	// Make a Variable Index return to be translated by callee
	ret.eType = EXPTYPE_VARIABLENAME;
	ret.eData.vni.pOwnerType = pType;
	ret.eData.vni.varIndex = varIndex;

	return 0;
}

long SystemObject::eScrollX(LPVAL theParams, ExpReturn& ret)
{
	double v = pLayout->scrollX + (pLayout->winWidth / 2);

	if (!pLayout->unboundedScrolling)
		Clamp<double>(v, pLayout->winWidth / 2, pLayout->width - pLayout->winWidth / 2);

	return ret = v;
}

long SystemObject::eScrollY(LPVAL theParams, ExpReturn& ret)
{
	double v = pLayout->scrollY + (pLayout->winHeight / 2);

	if (!pLayout->unboundedScrolling)
		Clamp<double>(v, pLayout->winHeight / 2, pLayout->height - pLayout->winHeight / 2);

	return ret = v;
}

long SystemObject::eWinWidth(LPVAL theParams, ExpReturn& ret)
{
	// Return new value if waiting to change res next tick
	if (changeresolution)
		return ret = changeResWidth;
	else
		return ret = pLayout->winWidth;
}

long SystemObject::eWinHeight(LPVAL theParams, ExpReturn& ret)
{
	// Return new value if waiting to change res next tick
	if (changeresolution)
		return ret = changeResHeight;
	else
		return ret = pLayout->winHeight;
}

long SystemObject::eGetPsVersion(LPVAL theParams, ExpReturn& ret)
{
#ifdef CONSTRUCT_DIRECTX9

	UINT ps_major = D3DSHADER_VERSION_MAJOR(pCRuntime->renderer.GetCaps().PixelShaderVersion);
	UINT ps_minor = D3DSHADER_VERSION_MINOR(pCRuntime->renderer.GetCaps().PixelShaderVersion);

#ifdef CONSTRUCT_PREVIEW
	switch (pCRuntime->simShader) {
	case SS_NOSIM:
		{
			double ps_version = ps_major;
			ps_version += ((double)ps_minor) / 10.0;
			return ret = ps_version;
		}
	case SS_PS14:
		return ret = 1.4;
	case SS_PS11:
		return ret = 1.1;
	case SS_PS00:
		return ret = 0.0;
	default:
		return ret = 0.0;
	}
#else
	double ps_version = ps_major;
	ps_version += ((double)ps_minor) / 10.0;
	return ret = ps_version;
#endif
#else
	return ret = 0.0;
#endif
}

long SystemObject::eRGB(LPVAL theParams, ExpReturn& ret)
{
	int r = theParams[0].GetInt();
	int g = theParams[1].GetInt();
	int b = theParams[2].GetInt();

	return ret = (__int64)RGB(r,g,b);
}

long SystemObject::eGetRed(LPVAL theParams, ExpReturn& ret)
{
	int cr = theParams[0].GetInt();
	return ret = GetRValue(cr);
}

long SystemObject::eGetGreen(LPVAL theParams, ExpReturn& ret)
{
	int cr = theParams[0].GetInt();
	return ret = GetGValue(cr);
}

long SystemObject::eGetBlue(LPVAL theParams, ExpReturn& ret)
{
	int cr = theParams[0].GetInt();
	return ret = GetBValue(cr);
}

long SystemObject::eGetTickCount(LPVAL theParams, ExpReturn& ret)
{
	return ret = pLayout->frameCounter64;
}

long SystemObject::eGetTimer(LPVAL theParams, ExpReturn& ret)
{
	return ret = pLayout->currentClock;
}

long SystemObject::eLayerZoomX(LPVAL theParams, ExpReturn& ret)
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	
	if (pLayer == NULL) return ret = 0;

	return ret = pLayer->zoomXoffset * 100.0f;
}

long SystemObject::eLayerZoomY(LPVAL theParams, ExpReturn& ret)
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	
	if (pLayer == NULL) return ret = 0;

	return ret = pLayer->zoomYoffset * 100.0f;
}

long SystemObject::eLayerScrollRateX(LPVAL theParams, ExpReturn& ret)
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	
	if (pLayer == NULL) return ret = 0;

	return ret = pLayer->scrollXf * 100.0f;
}

long SystemObject::eLayerScrollRateY(LPVAL theParams, ExpReturn& ret)
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	
	if (pLayer == NULL) return ret = 0;

	return ret = pLayer->scrollYf * 100.0f;
}

long SystemObject::eLayerOpacity(LPVAL theParams, ExpReturn& ret)
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	
	if (pLayer == NULL) return ret = 0;

	return ret = pLayer->opacity;
}

long SystemObject::eLayerName(LPVAL theParams, ExpReturn& ret)
{
	int layerIndex = theParams[0].GetInt() - 1;
	if (layerIndex < 0 || layerIndex >= pLayout->layers.size())
		return ret = 0;

	return ret.ReturnString(pCRuntime, pLayout->layers[layerIndex]->name);
}

long SystemObject::eLayerCount(LPVAL theParams, ExpReturn& ret)
{
	return ret = pLayout->layers.size();
}

long SystemObject::eZoomX(LPVAL theParams, ExpReturn& ret)
{
	return ret = pLayout->zoomX * 100.0;
}

long SystemObject::eZoomY(LPVAL theParams, ExpReturn& ret)
{
	return ret = pLayout->zoomY * 100.0;
}

long SystemObject::eTimeDelta(LPVAL theParams, ExpReturn& ret)
{
	return ret = pCRuntime->GetTimeDelta();
}

long SystemObject::eScrollXLeft(LPVAL theParams, ExpReturn& ret)
{
	float winWidth = pCRuntime->winWidth;

	if (theParams[0].Type() == EXPTYPE_NULL) {

		float zoomedWindowWidth = ((1.0 / pLayout->zoomX) * winWidth);
		double v = pLayout->scrollX + (winWidth / 2.0f) - (zoomedWindowWidth / 2.0f);

		if (!pLayout->unboundedScrolling)
			Clamp(v, 0.0, (double)pLayout->width - zoomedWindowWidth);

		return ret = v;
	}
	// Use layer
	else {

		CRunLayer* pLayer = theParams[0].GetLayerParam(pCRuntime, pLayout);
		if (pLayer == NULL) return 0;

		float zoomedWindowWidth = ((1.0 / (1 + (pLayout->zoomX -1)* pLayer->zoomXf) / pLayer->zoomXoffset) * winWidth);
		double v = ((pLayout->scrollX * pLayer->scrollXf) + (winWidth / 2.0f)) - (zoomedWindowWidth / 2.0f);

		if (!pLayout->unboundedScrolling)
			Clamp(v, 0.0, ((double)pLayout->width - zoomedWindowWidth) * pLayer->scrollXf);

		return ret = v;

	}
}

long SystemObject::eScrollXRight(LPVAL theParams, ExpReturn& ret)
{
	float winWidth = pCRuntime->winWidth;

	if (theParams[0].Type() == EXPTYPE_NULL) {

		float zoomedWindowWidth = ((1.0 / pLayout->zoomX) * winWidth);
		double v = pLayout->scrollX + (winWidth / 2.0f) + (zoomedWindowWidth / 2.0f);

		if (!pLayout->unboundedScrolling)
			Clamp<double>(v, zoomedWindowWidth, pLayout->width);

		return ret = v;

	}
	// Use layer
	else {

		CRunLayer* pLayer = theParams[0].GetLayerParam(pCRuntime, pLayout);
		if (pLayer == NULL) return 0;

		float zoomedWindowWidth = ((1.0 / (1 + (pLayout->zoomX -1)* pLayer->zoomXf) / pLayer->zoomXoffset) * winWidth);
		double v = (pLayout->scrollX * pLayer->scrollXf) + (winWidth / 2.0f) + (zoomedWindowWidth / 2.0f);

		if (!pLayout->unboundedScrolling)
			Clamp<double>(v, zoomedWindowWidth, zoomedWindowWidth + (((double)pLayout->width - zoomedWindowWidth) * pLayer->scrollXf));

		return ret = v;

	}
}

long SystemObject::eScrollYTop(LPVAL theParams, ExpReturn& ret)
{
	float winHeight = pCRuntime->winHeight;

	if (theParams[0].Type() == EXPTYPE_NULL) {

		float zoomedWindowHeight = ((1.0 / pLayout->zoomY) * winHeight);
		double v = pLayout->scrollY + (winHeight / 2.0f) - (zoomedWindowHeight / 2.0f);

		if (!pLayout->unboundedScrolling)
			Clamp<double>(v, 0.0, (double)pLayout->height - zoomedWindowHeight);

		return ret = v;
	}
	// Use layer
	else {

		CRunLayer* pLayer = theParams[0].GetLayerParam(pCRuntime, pLayout);
		if (pLayer == NULL) return 0;

		float zoomedWindowHeight = ((1.0 / (1 + (pLayout->zoomY -1)* pLayer->zoomYf) / pLayer->zoomYoffset) * winHeight);
		double v = (pLayout->scrollY * pLayer->scrollYf) + (winHeight / 2.0f) - (zoomedWindowHeight / 2.0f);

		if (!pLayout->unboundedScrolling)
			Clamp<double>(v, 0.0, ((double)pLayout->height - zoomedWindowHeight) * pLayer->scrollYf);

		return ret = v;
	}
}

long SystemObject::eScrollYBottom(LPVAL theParams, ExpReturn& ret)
{
	float winHeight = pCRuntime->winHeight;

	if (theParams[0].Type() == EXPTYPE_NULL) {

		float zoomedWindowHeight = ((1.0 / pLayout->zoomY) * winHeight);
		double v = pLayout->scrollY + (winHeight / 2.0f) + (zoomedWindowHeight / 2.0f);

		if (!pLayout->unboundedScrolling)
			Clamp<double>(v, zoomedWindowHeight, pLayout->height);

		return ret = v;

	}
	// Use layer
	else {

		CRunLayer* pLayer = theParams[0].GetLayerParam(pCRuntime, pLayout);
		if (pLayer == NULL) return 0;

		float zoomedWindowHeight = ((1.0 / (1 + (pLayout->zoomY -1)* pLayer->zoomYf) / pLayer->zoomYoffset) * winHeight);
		double v = (pLayout->scrollY * pLayer->scrollYf) + (winHeight / 2.0f) + (zoomedWindowHeight / 2.0f);

		if (!pLayout->unboundedScrolling)
			Clamp<double>(v, zoomedWindowHeight, zoomedWindowHeight + (((double)pLayout->height - zoomedWindowHeight) * pLayer->scrollYf));

		return ret = v;
	}
}

long SystemObject::eDisplayAngle(LPVAL theParams, ExpReturn& ret)
{
	return ret = pLayout->display_angle;
}

long SystemObject::eClamp(LPVAL theParams, ExpReturn& ret)
{
	double value = theParams[0].GetDouble();
	double lesser = theParams[1].GetDouble();
	double greater = theParams[2].GetDouble();

	if (value < lesser) value = lesser;
	if (value > greater) value = greater;
	return ret = value;
}

long SystemObject::eAppPath(LPVAL theParams, ExpReturn& ret)
{
	// Preview: get the .cap's path
#ifdef CONSTRUCT_PREVIEW
	CString AppPath(pCRuntime->capFile);

#else	// EXE runtime: use module name
	char path[1024];
	GetModuleFileName(NULL, path, 1024);
	CString AppPath(path);
#endif

	int backslash = AppPath.ReverseFind('\\');
	AppPath.Delete(backslash + 1, AppPath.GetLength() - backslash - 1);	

	return ret.ReturnString(pCRuntime, AppPath);
}

long SystemObject::eLayoutWidth(LPVAL theParams, ExpReturn& ret)
{
	return ret = pLayout->width;
}

long SystemObject::eLayoutHeight(LPVAL theParams, ExpReturn& ret)
{
	return ret = pLayout->height;
}

double SystemObject::Lerp(double a, double b, double x)
{
	return a + x * (b - a);
}

double SystemObject::Qarp(double a, double b, double c, double x)
{
	return Lerp(Lerp(a, b, x), Lerp(b, c, x), x);
}

long SystemObject::eLerp(LPVAL theParams, ExpReturn& ret)
{
	// Calculate linear interpolation:
	// lerp(a, b, x) = a + x(b - a)
	double a = theParams[0].GetDouble();
	double b = theParams[1].GetDouble();
	double x = theParams[2].GetDouble();

	return ret = Lerp(a, b, x);
}

long SystemObject::eVram(LPVAL theParams, ExpReturn& ret)
{
#ifdef APPRUNTIME
	return ret = 0;
#else
	if (SysGetNumParams(theParams) == 1) {
		CString param = theParams[0].GetString();
		param.MakeLower();

		if (param == "usage" || param == "total")
			return ret = pCRuntime->renderer.GetTotalTextureMemoryUsage();
		if (param == "textures")
			return ret = pCRuntime->renderer.GetTotalTextureMemoryUsage() - pCRuntime->renderer.GetRenderTargetMemoryUsage();
		if (param == "targets")
			return ret = pCRuntime->renderer.GetRenderTargetMemoryUsage();

		return ret = 0;
	}
	else
		return ret = pCRuntime->renderer.GetDevice()->GetAvailableTextureMem();
#endif
}

long SystemObject::eDisplayDevice(LPVAL theParams, ExpReturn& ret)
{
#ifdef CONSTRUCT_DIRECTX9
	D3DADAPTER_IDENTIFIER9 id;
	pCRuntime->renderer.GetD3D9()->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &id);
	return ret.ReturnString(pCRuntime, id.Description);
#else
	return ret.ReturnString(pCRuntime, "(none)");
#endif
}

long SystemObject::eGetTimeScale(LPVAL theParams, ExpReturn& ret)
{
	return ret = pCRuntime->timeScale;
}

long SystemObject::eGetLayerAngle(LPVAL theParams, ExpReturn& ret)
{
	CRunLayer* pLayer = GetLayerParam(theParams[0]);
	
	if (pLayer == NULL) return ret = 0;

	return ret = pLayer->angle;
}

long SystemObject::eLinearAim(LPVAL theParams, ExpReturn& ret)
{
	double Gx = theParams[0].GetDouble();
	double Gy = theParams[1].GetDouble();
	double u = theParams[2].GetDouble();
	double Px = theParams[3].GetDouble();
	double Py = theParams[4].GetDouble();
	double s = theParams[5].GetDouble();
	double h = RADIANS(theParams[6].GetDouble() + 180.0);		// Target angle

	float sin_h, cos_h;
	cr::sincosf(h, &sin_h, &cos_h);

	double q = Py - Gy;
	double r = Px - Gx;
	double w = (s * sin_h * (Gx - Px) - s * cos_h * (Gy - Py)) / u;


	double a_radians = asin(w / sqrt(q*q + r*r)) - atan2(q, -r);
	double a_degrees = DEGREES(a_radians);

	// a is not a number: there is no solution. just aim directly
	if (a_radians != a_radians) {
		double dx = Gx - Px;
		double dy = Gy - Py;
		a_degrees = DEGREES(atan2(dy, dx));
	}

	return ret = a_degrees + 180.0;
}

long SystemObject::eLayoutNumber(LPVAL theParams, ExpReturn& ret)
{
	return ret = pLayout->number + 1;
}

long SystemObject::eLayoutName(LPVAL theParams, ExpReturn& ret)
{
	return ret.ReturnString(pCRuntime, pLayout->name);
}

long SystemObject::eSign(LPVAL theParams, ExpReturn& ret)
{
	double x = theParams[0].GetDouble();

	if (x == 0.0)
		return ret = 0.0;
	else
		return ret = x / abs(x);
}

long SystemObject::eQarp(LPVAL theParams, ExpReturn& ret)
{
	double a = theParams[0].GetDouble();
	double b = theParams[1].GetDouble();
	double c = theParams[2].GetDouble();
	double x = theParams[3].GetDouble();

	return ret = Qarp(a, b, c, x);
}

long SystemObject::eCosp(LPVAL theParams, ExpReturn& ret)
{
	double a = theParams[0].GetDouble();
	double b = theParams[1].GetDouble();
	double x = theParams[2].GetDouble();

	return ret = (a + b + (a - b) * cos(x * 3.1415926535897932384626433832795)) / 2.0;
}

long SystemObject::eAngleDiff(LPVAL theParams, ExpReturn& ret)
{
	float a = theParams[0].GetFloat();
	float b = theParams[1].GetFloat();

	// Prevent -1.#IND error
	if (a == b) return ret = 0.0;

	float ca, cb, sa, sb;
	cr::sincosf(RADIANS(a), &sa, &ca);
	cr::sincosf(RADIANS(b), &sb, &cb);

	float dot = ca*cb + sa*sb;
	return ret = DEGREES(acos(dot));
}

float RotateTowards_deg(float start, float end, float step)
{
	float cs, ce, ss, se;
	cr::sincosf(RADIANS(start), &ss, &cs);
	cr::sincosf(RADIANS(end), &se, &ce);

	float z = cs*se - ss*ce;
	float dot = cs*ce + ss*se;
	float angle = DEGREES(acos(dot));

	if(angle > step)
		if(z > 0)
			return start + step;
		else
			return start - step;
	else
		return end;
}

long SystemObject::eRotateAngle(LPVAL theParams, ExpReturn& ret)
{
	double a = theParams[0].GetDouble();
	double b = theParams[1].GetDouble();
	double step = theParams[2].GetDouble();

	return ret = RotateTowards_deg(a, b, step);
}

long SystemObject::eNormalRandom(LPVAL theParams, ExpReturn& ret)
{
#ifdef CONSTRUCT_RUNTIME_TR1
	double mean = theParams[0].GetDouble();
	double sigma = theParams[1].GetDouble();

	return ret = tr1::normal_distribution<double>(mean, sigma)(mtwister);
#else
	return ret = 0.0;
#endif
}

long SystemObject::eCubicInterpolation(LPVAL theParams, ExpReturn &ret)
{
	double a = theParams[0].GetDouble();
	double b = theParams[1].GetDouble();
	double c = theParams[2].GetDouble();
	double d = theParams[3].GetDouble();
	double x = theParams[4].GetDouble();

	return ret = Lerp(Qarp(a, b, c, x), Qarp(b, c, d, x), x);
}

long SystemObject::eGetMultisamples(LPVAL theParams, ExpReturn &ret)
{
	return ret = pCRuntime->multisamples + 1;
}

#ifndef PYTHON
long SystemObject::aRunScript(LPVAL theParams)
{
	return 0;
}
long SystemObject::cCompareScript(LPVAL theParams)
{
	return 0;
}
long SystemObject::eRunScript(LPVAL theParams, ExpReturn& ret)
{
	return 0;
}
#else

///////////////////////////////////////////////////////

SystemObject* g_pySystem;

CString GetLower(const CString& src)
{
	CString lowered = src;
	lowered.MakeLower();
	return lowered;
}


vector<ExpStore*> param_dump;
void ExpToPython(PyObject*& tupple, ExpStore* ret)
{
	ExpType type = ret->GetEType();
	switch(type)
	{
		case EXPTYPE_INTEGER:
			tupple = Py_BuildValue("l", ret->GetInt());
		break;

		case EXPTYPE_FLOAT:
			tupple = Py_BuildValue("f", ret->GetFloat());
		break;
		case EXPTYPE_STRING:
		{
			tupple = Py_BuildValue("s", ret->GetString());
		}
		break;
		case EXPTYPE_ARRAY:
			{
			int size = ret->GetArraySize();
				for(int i = 0; i < size; i++)
				{
					tupple = PyList_New(size);
					PyObject* child = NULL;
					ExpToPython(child, &ret->GetArray()[i]);
					Py_INCREF(child); //PyList_SetItem 'eats' a reference of child
					PyList_SetItem(tupple, i, child);
				}
			}
		break;
		case EXPTYPE_BINARY:

		break;
		case EXPTYPE_VARIABLENAME:
			tupple = Py_BuildValue("l", ret->GetInt());
		break;
	}
}


void GetPyParams(ExpStore* ParamList, PyObject* arg, int size = CRUNTIME_MAX_PARAMS)
{
	for(int i = 0; i < size; i++)
	{
		PyObject* ob;
		ExpStore* ret;
		if(PyList_Check(arg))
		{
			ob = PyList_GET_ITEM(arg, i);
			ret = (ExpStore*)(ParamList + i);
		}
		else
		{
			ob = arg;
			ret = (ExpStore*)(ParamList);
			i = size;
		}

		if(PyList_Check(ob))
		{
			int count = PyList_GET_SIZE(ob);
			ExpStore* child_ret = new ExpStore[count];
			
			//Dump management
			param_dump.push_back(child_ret);

			GetPyParams(child_ret, ob, count);

			ret->GetEType() = EXPTYPE_ARRAY;
			ret->GetArr().pArray = child_ret;
			ret->GetArrSize() = count;
		}
		if(PyFloat_Check(ob))
		{
			(*ret) = PyFloat_AS_DOUBLE(ob);
		}
		if(PyInt_Check(ob))
		{
			(*ret) = PyInt_AS_LONG(ob);
		}
		if(PyLong_Check(ob))
		{
			(*ret) = PyLong_AsLong(ob);
		}
	/*	if(PyBool_Check(ob))
		{
			if(Py_True(ob))
				(*ret) = (long)1;
			else
				(*ret) = (long)0;
		}*/
		if(PyString_Check(ob))
		{
			CString meh;
			meh.Format("%s", PyString_AS_STRING(ob) );

			(*ret) = ExpStore(meh);

		}

	}
}


long CallAC(ACROUTINE acroutine, CRunObject* obj, PyObject* pParams)
{
	// Params
	int size = 1;
	if(PyList_Check(pParams))
	{
		size = PyList_Size(pParams);
	}
	ExpStore ParamList[CRUNTIME_MAX_PARAMS];
	GetPyParams(&ParamList[0], pParams, size);

	return (obj->*acroutine)((ExpReturn*)ParamList);
}

long CallSysAC(SYSTEMROUTINE acroutine, PyObject* pParams)
{
	// Params
	int size = 1;
	if(PyList_Check(pParams))
	{
		size = PyList_Size(pParams);
	}
	ExpStore ParamList[CRUNTIME_MAX_PARAMS];
	GetPyParams(&ParamList[0], pParams, size);

	return ((*g_pySystem).*acroutine)((ExpReturn*)ParamList);
}

class ExpStorePy : public ExpStore
{
public:
	void PreventHeapCorruptionHack()
	{
		if(Type() == EXPTYPE_STRING)
			eData.str = new CString;
	}
};


long CallE(EXPROUTINE exproutine, CRunObject* obj, PyObject* pParams, PyObject*& pythonret)
{
	//assert(false);
	// Call the action
	ExpStorePy ret;
	
	// Params
	int size = 1;
	if(PyList_Check(pParams))
	{
		size = PyList_Size(pParams);
	}
	ExpStore ParamList[CRUNTIME_MAX_PARAMS];
	GetPyParams(&ParamList[0], pParams, size);

	// Call expression
	long returnval = (obj->*exproutine)((ExpReturn*)ParamList, *(ExpReturn*)&ret);

	// Convert
	pythonret = Py_None;
	ExpToPython(pythonret, &ret);
	if(pythonret == Py_None)
		Py_INCREF( Py_None );

	ret.PreventHeapCorruptionHack(); // We cant free the memory in eData.str because its allocated by a special runtime function
									 // So this hack forces the str to point to a dynamically allocated string, if its a string

	return returnval;
}

long CallSysE(SYSEXPROUTINE exproutine, PyObject* pParams, PyObject*& pythonret)
{
	// Call the action
	ExpStore ret;
	
	// Params
	int size = 1;
	if(PyList_Check(pParams))
	{
		size = PyList_Size(pParams);
	}
	ExpStore ParamList[CRUNTIME_MAX_PARAMS];
	GetPyParams(&ParamList[0], pParams, size);

	// Call expression
	long returnval = ((*g_pySystem).*exproutine)((ExpReturn*)ParamList, *(ExpReturn*)&ret);

	// Convert
	pythonret = Py_None;
	ExpToPython(pythonret, &ret);

	if(pythonret == Py_None)
		Py_INCREF( Py_None );
	return returnval;
}



///////////////////////
//	System ACE
PyObject* pySystemAction( PyObject* self, PyObject* args )
{
	long routine;
	PyObject* pParams;

	PyArg_ParseTuple( args, "lO", &routine,  &pParams );

	// Call the action
	CallSysAC(*(ACROUTINE*)routine, pParams);

	Py_INCREF( Py_None );
	return Py_None;
}

PyObject* pySystemCondition( PyObject* self, PyObject* args )
{
	long routine;
	PyObject* pParams;

	PyArg_ParseTuple( args, "lO", &routine,  &pParams );

	// Call the condition
	if(CallSysAC(*(ACROUTINE*)routine, pParams))
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

PyObject* pySystemExpression( PyObject* self, PyObject* args )
{
	long routine;
	PyObject* pParams;

	PyArg_ParseTuple( args, "lO", &routine,  &pParams );

	PyObject* pythonret;
	CallSysE(*(EXPROUTINE*)routine, pParams, pythonret);

	return pythonret;
}

///////////////////////////
//	Object Instances ACES


PyObject* pyObjAction( PyObject* self, PyObject* args )
{
	long object;
	long routine;
	PyObject* pParams;

	PyArg_ParseTuple( args, "llO", &object, &routine,  &pParams );

	// Call the action
	CallAC(*(ACROUTINE*)routine, (CRunObject*)object, pParams);

	Py_INCREF( Py_None );
	return Py_None;
}

PyObject* pyObjCondition( PyObject* self, PyObject* args )
{
	long object;
	long routine;
	PyObject* pParams;

	PyArg_ParseTuple( args, "llO", &object, &routine,  &pParams );

	// Call the condition
	if(CallAC(*(ACROUTINE*)routine, (CRunObject*)object, pParams))
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

PyObject* pyObjExpression( PyObject* self, PyObject* args )
{
	long object;
	long routine;
	PyObject* pParams;

	PyArg_ParseTuple( args, "llO", &object, &routine,  &pParams );

	PyObject* pythonret;
	CallE(*(EXPROUTINE*)routine, (CRunObject*)object, pParams, pythonret);

	return pythonret;
}

///////////////////
//	Object Type ACES

PyObject* pyObjTypeAction( PyObject* self, PyObject* args )
{
	long object;
	long routine;
	PyObject* pParams;

	PyArg_ParseTuple( args, "llO", &object, &routine,  &pParams );

	CRunObjType* pObj = (CRunObjType*)object; 

	if(pObj->instances.size() == 0)
	{
		Py_INCREF( Py_None );
		return Py_None;
	}

	// Call the action
	CallAC(*(ACROUTINE*)routine, pObj->instances[0], pParams);

	Py_INCREF( Py_None );
	return Py_None;
}

PyObject* pyObjTypeCondition( PyObject* self, PyObject* args )
{
	long object;
	long routine;
	PyObject* pParams;

	PyArg_ParseTuple( args, "llO", &object, &routine,  &pParams );

	CRunObjType* pObj = (CRunObjType*)object; 

	if(pObj->instances.size() == 0)
	{
		Py_INCREF( Py_None );
		return Py_None;
	}

	// Call the action
	if(CallAC(*(ACROUTINE*)routine, pObj->instances[0], pParams))
		return Py_BuildValue("i", 1 ); //true
	
	return Py_BuildValue("i", 0 ); //false
}

PyObject* pyObjTypeExpression( PyObject* self, PyObject* args )
{
	long object;
	long routine;
	PyObject* pParams;

	PyArg_ParseTuple( args, "llO", &object, &routine,  &pParams );

	CRunObjType* pObj = (CRunObjType*)object; 

	if(pObj->instances.size() == 0)
	{
		Py_INCREF( Py_None );
		return Py_None;
	}

	// Call the action
	PyObject* pythonret;
	CallE(*(EXPROUTINE*)routine, pObj->instances[0], pParams, pythonret);

	return pythonret;
}

///////////////////////////////////////////////////////////




PyObject* pyObjCount( PyObject* self, PyObject* args )
{
	//param1 - pointer to objecttype
	long pointer;
	PyArg_ParseTuple( args, "l", &pointer );

	CRunObjType* pType = (CRunObjType*)pointer; // we cant do any error checking here :(
	long size = pType->instances.size();

	return Py_BuildValue( "l", size );
}
PyObject* pyInstance( PyObject* self, PyObject* args )
{
	long pointer;
	long key;
	PyArg_ParseTuple( args, "ll", &pointer, &key );

	CRunObjType* pType = (CRunObjType*)pointer; 


	if(key >= 0 && key < pType->instances.size())
	{
		CRunObject* pObject = pType->instances[key];
		return Py_BuildValue( "l", pType->instances[key] );
	}

	return Py_BuildValue("l",-1);
	//Py_INCREF( Py_None );
	//return Py_None;
}

PyObject* test( PyObject* self, PyObject* args )
{
	MessageBox(0, "Test complete",0,0);
	Py_INCREF( Py_None );
	return Py_None;
}

PyObject* pySetObjectDictionary( PyObject* self, PyObject* args)
{
	long pointer;
	PyObject* pyObjectDictionary;
	PyArg_ParseTuple( args, "lO", &pointer, &pyObjectDictionary);

	CRunObject* pObject = (CRunObject*)pointer;
	pObject->info.pInfo->pDictionary = pyObjectDictionary;

	Py_INCREF( Py_None );
	return Py_None;
}

PyObject* pySetObjectAttribute( PyObject* self, PyObject* args)
{
	long pointer;
	char *PyName;
	PyObject* PyObjectValue;
	PyArg_ParseTuple( args, "lsO", &pointer, &PyName, &PyObjectValue);

	CString Name;
	Name.Format("%s", PyName);
	CString lowername = GetLower(Name);
	CRunObject* pObject = (CRunObject*)pointer;
	CRunObjType* pType = pObject->pType;

	map<CString, ACROUTINE>::iterator i = pType->PySetAttrib.find(lowername);

	if(i != pType->PySetAttrib.end())
	{
		CallAC(i->second, pObject, PyObjectValue);
	}
	else
	{
		PyDict_SetItemString( pObject->info.pInfo->pDictionary, Name, PyObjectValue);
	}	
	Py_INCREF( Py_None );
	return Py_None;
}

PyObject* pyGetObjectAttribute( PyObject* self, PyObject* args)
{
	long pointer;
	char *PyName;
	PyObject* PyObjectValue;
	PyArg_ParseTuple( args, "lsO", &pointer, &PyName, &PyObjectValue);
	
	CString Name;
	Name.Format("%s", PyName);
	CString lowername = GetLower(Name);

	CRunObject* pObject = (CRunObject*)pointer;
	CRunObjType* pType = pObject->pType;

	map<CString, EXPROUTINE>::iterator i = pType->PyGetAttrib.find(lowername);
	if(i != pType->PyGetAttrib.end())
	{
		PyObject* ret;
		CallE(i->second, pObject, PyObjectValue, ret);
		return ret;
	}
	else
	{
		PyObject* ret = PyDict_GetItemString( pObject->info.pInfo->pDictionary, Name);
		if(ret)
			return ret;
		else
		{
			PyErr_SetString(PyExc_AttributeError,Name);
			Py_INCREF( Py_None );
			return Py_None;
		}
	}
}

PyObject* pySetObjTypeAttribute( PyObject* self, PyObject* args)
{	
	long pointer;
	char *PyName;
	PyObject* PyObjectValue;
	PyArg_ParseTuple( args, "lsO", &pointer, &PyName, &PyObjectValue);

	CString Name;
	Name.Format("%s", PyName);
	CString lowername = GetLower(Name);

	CRunObjType* pType = (CRunObjType*)pointer; 
	map<CString, ACROUTINE>::iterator i = pType->PySetAttrib.find(lowername);

	if( pType->instances.size() == 0)
	{
		Py_INCREF( Py_None );
		return Py_None;
	}

	if(i !=  pType->PySetAttrib.end())
		CallAC(i->second, pType->instances[0], PyObjectValue);
	else
	{
		PyDict_SetItemString( pType->pDictionary, Name, PyObjectValue);
	}	
	Py_INCREF( Py_None );
	return Py_None;
}

PyObject* pyGetObjTypeAttribute( PyObject* self, PyObject* args)
{
	long pointer;
	char *PyName;
	PyObject* PyObjectValue;
	PyArg_ParseTuple( args, "lsO", &pointer, &PyName, &PyObjectValue);
	
	CString Name;
	Name.Format("%s", PyName);
	CString lowername = GetLower(Name);

	CRunObjType* pType = (CRunObjType*)pointer; 
	map<CString, EXPROUTINE>::iterator i = pType->PyGetAttrib.find(lowername);

	if( pType->instances.size() != 0)
	{
		if(i != pType->PyGetAttrib.end())
		{
			PyObject* ret;
			CallE(i->second, pType->instances[0], PyObjectValue, ret);
			return ret;
		}
		else
		{
			PyObject* ret = PyDict_GetItemString( pType->pDictionary, Name);
			if(ret)
				return ret;
		}
	}

	//raise AttributeError, name

	PyErr_SetString(PyExc_AttributeError, Name);
	return NULL;
}

PyObject* pySetObjTypeDictionary( PyObject* self, PyObject* args)
{
	long pointer;
	PyObject* pyObjectTypeDictionary;
	PyArg_ParseTuple( args, "lO", &pointer, &pyObjectTypeDictionary);

	CRunObjType* pType = (CRunObjType*)pointer; 
	pType->pDictionary = pyObjectTypeDictionary;

	Py_INCREF( Py_None );
	return Py_None;
}



PyObject* pySetSysAttribute( PyObject* self, PyObject* args)
{	
	char *PyName;
	PyObject* PyObjectValue;
	PyArg_ParseTuple( args, "sO", &PyName, &PyObjectValue);

	CString Name;
	Name.Format("%s", PyName);
	CString lowername = GetLower(Name);
	map<CString, SYSTEMROUTINE>::iterator i = g_pySystem->PySetAttrib.find(lowername);

	if(i !=  g_pySystem->PySetAttrib.end())
		CallSysAC(i->second, PyObjectValue);
	else
	{
		PyDict_SetItemString( g_pySystem->pDictionary, Name, PyObjectValue);
	}	
	Py_INCREF( Py_None );
	return Py_None;
}

PyObject* pyGetSysAttribute( PyObject* self, PyObject* args)
{
	char *PyName;
	PyObject* PyObjectValue;
	PyArg_ParseTuple( args, "sO", &PyName, &PyObjectValue);
	
	CString Name;
	Name.Format("%s", PyName);

	CString lowername = GetLower(Name);
	map<CString, SYSEXPROUTINE>::iterator i = g_pySystem->PyGetAttrib.find(lowername);

	if(i != g_pySystem->PyGetAttrib.end())
	{
		PyObject* ret;
		CallSysE(i->second, PyObjectValue, ret);
		return ret;
	}
	else
	{
		PyObject* ret = PyDict_GetItemString(g_pySystem->pDictionary, Name);
		if(ret)
			return ret;
		else
		{
			PyErr_SetString(PyExc_AttributeError,Name);
			Py_INCREF( Py_None );
			return Py_None;
		}

	}
}



PyMethodDef g_methodDefinitions[] =
{
	{"Test", test, METH_VARARGS, "Makes a message box"},
    { "sa", pySystemAction, METH_VARARGS, "Calls a system action" },
    { "sc", pySystemCondition, METH_VARARGS, "Calls a system condition" },
    { "se", pySystemExpression, METH_VARARGS, "Calls a system expression" },

    { "oa", pyObjAction, METH_VARARGS, "Calls an object instance action" },
    { "oc", pyObjCondition, METH_VARARGS, "Calls an object instance condition" },
    { "oe", pyObjExpression, METH_VARARGS, "Calls an object instance expression" },

    { "ta", pyObjTypeAction, METH_VARARGS, "Calls an object type action" },
    { "tc", pyObjTypeCondition, METH_VARARGS, "Calls an object type condition" },
    { "te", pyObjTypeExpression, METH_VARARGS, "Calls an object type expression" },

	{ "ins", pyInstance, METH_VARARGS, "Retrieves a pointer to an object given an object type and an index" },
	{ "cnt", pyObjCount, METH_VARARGS, "Retrieve a count of an objtype" },

	{ "SysGetAtt", pyGetSysAttribute, METH_VARARGS, "Obtain an attribute from the system" },
	{ "SysSetAtt", pySetSysAttribute, METH_VARARGS, "Obtain an attribute from the system" },
	{ "OTGetAtt", pyGetObjTypeAttribute, METH_VARARGS, "Obtain an attribute from an object type" },
	{ "OTSetAtt", pySetObjTypeAttribute, METH_VARARGS, "Set an attribute to an object type" },
	{ "OTSetDict", pySetObjTypeDictionary, METH_VARARGS, "Set a dictionary to an object type" },
	{ "ObjGetAtt", pyGetObjectAttribute, METH_VARARGS, "Obtain an attribute from an object instance" },
	{ "ObjSetAtt", pySetObjectAttribute, METH_VARARGS, "Set an attribute to an object instance" },
	{ "ObjSetDict", pySetObjectDictionary, METH_VARARGS, "Set a dictionary to an object instance" },

    {NULL, NULL}
};

void AddLine(CString& s, CString& stri)
{
	s += "\n" + stri;
	stri = "";
}

CString str(long number)
{
	CString ret;
	ret.Format("%d", number);
	return ret;
}


void Export(CString string)
{
	fpos_t startAt;
	FILE* op = fopen("C:/test.txt", "w");
	fsetpos(op, &startAt);
	fwrite( string.GetBuffer(),  sizeof(char),string.GetLength(), op);
	fclose(op);

//	MessageBox(0,string,0,0);
}

bool SystemObject::InitPython()
{
	if(!PythonInit)
	{
		// Try to find python25.dll
		DWORD dw = SearchPath(NULL, "python26.dll", NULL, 0, NULL, NULL);

		// Not found
		if (dw == 0) {
			throw runtime_error("Python26.dll was not found and is required to run this application or feature.  Reinstalling the application "
								"may fix this problem.");
		}

		Py_Initialize();
		
		pDictionary = PyDict_New();
	    PyDict_SetItemString( pDictionary, "__builtins__", PyEval_GetBuiltins() );

		CString script;
		CString s;

		//Setup the system
		s = "import sys";
		AddLine(script, s);


#ifdef CONSTRUCT_PREVIEW
		char currentDirectory[MAX_PATH];
		GetModuleFileName(NULL, currentDirectory, MAX_PATH);

		char pathDrive[MAX_PATH];
		char pathDir[MAX_PATH];
		_splitpath(currentDirectory, pathDrive, pathDir, NULL, NULL);

		CString myPath = pathDrive;
		myPath += pathDir;
		myPath += "Construct.ini";
	
		char installPath[MAX_PATH];

		GetPrivateProfileString("Path", "Install", "", installPath, MAX_PATH, myPath);
		CString curFile;
		curFile.Format("%sData\\Python.zip", installPath);	
		CString curDir;
		curDir.Format("%sData\\Python", installPath);	

		pCRuntime->pythonDir = curDir;
		pCRuntime->pythonZipPath = curFile;

/*		s = "sys.path.append('";
		s += curFile;
		s += "')";
		s.Replace("\\", "\\\\");
		AddLine(script, s);

		MessageBox(0,script,0,0);*/
	
#endif
		s = "sys.path.append('";
		s += pCRuntime->pythonDir;
		s += "')";
		s.Replace("\\", "\\\\");
		AddLine(script, s);
#ifndef CONSTRUCT_PREVIEW
		s = "sys.path.append('";
		s += pCRuntime->pythonZipPath;
		s += "')";
		s.Replace("\\", "\\\\");
		AddLine(script, s);
#endif

		PyRun_String(script, Py_file_input, pDictionary, pDictionary);
		HandlePyErrors();

		// Setup a system redirect for error messages
		script = "";

		s =	"import StringIO";
		AddLine(script, s);
		s +=	"sys.stderr = StringIO.StringIO()";
		AddLine(script, s);

		PyRun_String(script, Py_file_input, pDictionary, pDictionary);
		HandlePyErrors();

		//////////////////////////////////////////////
		// Unzip the zip file of libs

#define ADDLINE(x) s = x; AddLine(script,s);

		script = "";

#ifndef CONSTRUCT_PREVIEW
		ADDLINE("import zipfile, os, os.path")
		ADDLINE("def _mkdir(newdir):")
		ADDLINE("	if os.path.isdir(newdir):")
		ADDLINE("		pass")
		ADDLINE("	elif os.path.isfile(newdir):")
		ADDLINE("		pass")
		ADDLINE("	else:")
		ADDLINE("		head, tail = os.path.split(newdir)")
		ADDLINE("		if head and not os.path.isdir(head):")
		ADDLINE("			_mkdir(head)")
		ADDLINE("		if tail:")
		ADDLINE("			os.mkdir(newdir)")

		ADDLINE("def unzip_file_into_dir(file, dir):")
		ADDLINE("	_mkdir(dir)")
		ADDLINE("	zfobj = zipfile.ZipFile(file)")
		ADDLINE("	for name in zfobj.namelist():")
		ADDLINE("		if name.endswith('/'):")
		ADDLINE("			_mkdir(os.path.join(dir, name))")
		ADDLINE("		else:")
		ADDLINE("			filepath = os.path.join(dir,name)")
		ADDLINE("			head, tail = os.path.split(filepath)")
		ADDLINE("			_mkdir(head)")
		ADDLINE("			outfile = open(filepath, 'wb')")
		ADDLINE("			outfile.write(zfobj.read(name))")
		ADDLINE("			outfile.close()")

		s = "var1 = '"; s+= pCRuntime->pythonZipPath; s+= "'"; s.Replace("\\", "\\\\"); AddLine(script, s);
		s = "var2 = '"; s+= pCRuntime->pythonDir    ; s+= "'"; s.Replace("\\", "\\\\"); AddLine(script, s);

		ADDLINE("unzip_file_into_dir(var1, var2)")

		PyRun_String(script, Py_file_input, pDictionary, pDictionary);
		HandlePyErrors();
#endif


		//////////////////////////////////////////////

		// Init construct wrapper
		PyImport_AddModule( "__construct__" );
		Py_InitModule( "__construct__", g_methodDefinitions );

		script = "";

		s =		"import __construct__";
		AddLine(script, s);

		PyRun_String(script, Py_file_input, pDictionary, pDictionary);
		HandlePyErrors();



		script = "";

		int index;

		// Add system and objects

		/////////////////////////
		// SYSTEM OBJECT
		////////////////////////

		s += "class System:";
		AddLine(script, s);

		// For attributes, we will store a map of actions which we consider
		// possible canditates. They must have the following:
		// begin with Set
		// have 1 parameter
		map<CString, SYSTEMROUTINE> possibleAttribute;

		// Actions
		index = 0;
		for(; index != sizeof(SysActScript) / sizeof(CString) ; index++)
		{
			CString name = SysActScript[index];
			int paramCount = SysActScriptParamCount[index];
			if(name == "")
				continue;

			if(paramCount == 1)
			{
				if(name.GetLength() > 3)
				{
					if(name.Left(3) == "Set")
					{
						possibleAttribute[GetLower(name.Mid(3))] = SysActRoutines[index];
					}
				}
			}

			s += 	"	def ";	s += name;	s += "(self";
			for(int p = 0; p < paramCount; p++)
				s += ", p" + str(p);
			s += "):";
			AddLine(script, s);

			s +=	"		__construct__.sa(";	
			s += str((int)&SysActRoutines[index]);

			s += ", [";
			for(int p = 0; p < paramCount; p++)
			{
				s += "p";  s+= str(p);
				if(p < paramCount-1)
					s += ", ";
			}
			s += "])";
			AddLine(script, s);
		} // for each action


		// Expressions
		index = 0;
		for(; index != sizeof(ExpRoutineTable) / sizeof(ExpRoutineEntry) ; index++)
		{
			CString name;
			name.Format("%s", ExpRoutineTable[index].name);

			if(name == "global")
				name = "globalVar";
			int paramCount = SysExpScriptParamCount[index];

			if(name == "")
				continue;
			if(paramCount != 0)
			{
				s += 	"	def ";	s += name;	s += "(self";
				for(int p = 0; p < paramCount; p++)
					s += ", p" + str(p);
				s += "):";
				AddLine(script, s);

				s +=	"		return __construct__.se(";	
				s += str((int)&ExpRoutineTable[index].routine);

				s += ", [";
				for(int p = 0; p < paramCount; p++)
				{
					s += "p";  s+= str(p);
					if(p < paramCount-1)
						s += ", ";
				}
				s += "])";
				AddLine(script, s);

			}
			else
			{
				PyGetAttrib[GetLower(name)] = ExpRoutineTable[index].routine;
				map<CString, SYSTEMROUTINE>::iterator find = possibleAttribute.find(name);
				if(find != possibleAttribute.end())
				{
					PySetAttrib[GetLower(name)] = find->second;
				}
				else
				{
					//todo: handle readonly attributes
				}
			}
		} // for each expressions

		s +=	"	def __getattr__(self, name):";
		AddLine(script, s);
		s +=	"		return __construct__.SysGetAtt(name, [])";
		AddLine(script, s);


		s +=	"	def __setattr__(self, name, key):";
		AddLine(script, s);
		s +=	"		return __construct__.SysSetAtt(name, key)";
		AddLine(script, s);	

		s += "system = System()";
		AddLine(script, s);	
		s += "System = System()";
		AddLine(script, s);	




	// END SYSTEM OBJECT








		for(vector<CRunObjType*>::iterator o = pCRuntime->objects.begin();	o != pCRuntime->objects.end();	o ++)
		{
			PluginModule* plug = (*o)->pPlugin;

			if(!plug) // if this is null, its usually because theres an attribute in the application
			{
				continue;
			}

			///////////////////////////////
			// OBJECT INSTANCE
			///////////////////////////////

			CString name = (*o)->Name;
			name.Replace("&", "");

			s += "class ";	s += name; s += "Instance:";
			AddLine(script, s);
			//s += "	def __init__(self):";
			//AddLine(script, s);
			//s += "		self.__instance__ = 0";
			//AddLine(script, s);


			// For attributes, we will store a map of actions which we consider
			// possible canditates. They must have the following:
			// begin with Set
			// have 1 parameter
			map<CString, ACROUTINE> possibleAttribute;


			// Actions
			index = 0;
			for(vector<PyFunct>::iterator i = plug->py_actions.begin(); i != plug->py_actions.end(); i++, index++)
			{
				if(i->name == "")
					continue;

				if(i->paramCount == 1)
				{
					if(i->name.GetLength() > 3)
					{
						if(i->name.Left(3) == "Set")
						{
							possibleAttribute[i->name.Mid(3)] = (*plug->pvActRoutines)[index].routine;
						}
					}
				}

				s += 	"	def ";	s += i->name;	s += "(self";
				for(int p = 0; p < i->paramCount; p++)
					s += ", p" + str(p);
				s += "):";
				AddLine(script, s);

				s +=	"		__construct__.oa(self.__instance__, ";	
				s += str((int)&((*plug->pvActRoutines)[index]));

				s += ", [";
				for(int p = 0; p < i->paramCount; p++)
				{
					s += "p";  s+= str(p);
					if(p < i->paramCount-1)
						s += ", ";
				}
				s += "])";
				AddLine(script, s);
			} // for each action

			// Conditions
			index = 0;
			for(vector<PyFunct>::iterator i = plug->py_conditions.begin(); i != plug->py_conditions.end(); i++, index++)
			{
				if(i->name == "")
					continue;

				s += 	"	def ";	s += i->name;	s += "(self";
				for(int p = 0; p < i->paramCount; p++)
					s += ", p" + str(p);

				s += "):";
				AddLine(script, s);

				s +=	"		return __construct__.oc(self.__instance__, ";	
				s += str((int)&((*plug->pvCndRoutines)[index]));

				s += ", [";
				for(int p = 0; p < i->paramCount; p++)
				{
					s += "p";  s+= str(p);
					if(p < i->paramCount-1)
						s += ", ";
				}
				s += "])";
				AddLine(script, s);
			} // for each conditions

			// Expressions
			index = 0;
			for(vector<PyFunct>::iterator i = plug->py_expressions.begin(); i != plug->py_expressions.end(); i++, index++)
			{

				if(i->name == "" || i->name.Left(1) == "<")
					continue;
				if(i->paramCount == 0)
				{
					(*o)->PyGetAttrib[GetLower(i->name)] = (*plug->pvExpRoutines)[index];
					map<CString, ACROUTINE>::iterator find = possibleAttribute.find(i->name);
					if(find != possibleAttribute.end())
					{
						(*o)->PySetAttrib[GetLower(i->name)] = find->second;
						continue;
					}
				}
				
				s += 	"	def ";	s += i->name;	s += "(self";
				for(int p = 0; p < i->paramCount; p++)
					s += ", p" + str(p);
				s += "):";
				AddLine(script, s);

				s +=	"		return __construct__.oe(self.__instance__, ";	
				s += str((int)&((*plug->pvExpRoutines)[index]));

				s += ", [";
				for(int p = 0; p < i->paramCount; p++)
				{
					s += "p";  s+= str(p);
					if(p < i->paramCount-1)
						s += ", ";
				}
				s += "])";
				AddLine(script, s);

				

			} // for each expressions

			s +=	"	def __getattr__(self, name):";
			AddLine(script, s);
			s +=	"		return __construct__.ObjGetAtt(self.__instance__, name, [])";
			AddLine(script, s);


			s +=	"	def __setattr__(self, name, key):";
			AddLine(script, s);
			s +=	"		return __construct__.ObjSetAtt(self.__instance__, name, key)";
			AddLine(script, s);

			
			
			///////////////////////////////
			// OBJECT TYPE
			///////////////////////////////

			s += "class ";	s += name; s += ":";
			AddLine(script, s);

			s += "	def __init__(self):";
			AddLine(script, s);
			s += "		__construct__.OTSetDict(" + str((long)(*o)) + ", self.__dict__)";
			AddLine(script, s);

			// Actions
			index = 0;
			for(vector<PyFunct>::iterator i = plug->py_actions.begin(); i != plug->py_actions.end(); i++, index++)
			{
				if(i->name == "")
					continue;

				s += 	"	def ";	s += i->name;	s += "(self";
				for(int p = 0; p < i->paramCount; p++)
					s += ", p" + str(p);
				s += "):";
				AddLine(script, s);

				s +=	"		__construct__.ta(";	
				s += str((long)(*o));
				s += ", ";
				s += str((int)&((*plug->pvActRoutines)[index]));

				s += ", [";
				for(int p = 0; p < i->paramCount; p++)
				{
					s += "p" + str(p);
					if(p < i->paramCount-1)
						s += ", ";
				}
				s += "])";
				AddLine(script, s);
			} // for each action


			// Conditions
			index = 0;
			for(vector<PyFunct>::iterator i = plug->py_conditions.begin(); i != plug->py_conditions.end(); i++, index++)
			{
				if(i->name == "")
					continue;

				s += 	"	def ";	s += i->name;	s += "(self";
				for(int p = 0; p < i->paramCount; p++)
					s += ", p" + str(p);
				s += "):";
				AddLine(script, s);

				s +=	"		return __construct__.tc(";	
				s += str((long)(*o));
				s += ", ";
				s += str((int)&((*plug->pvCndRoutines)[index]));

				s += ", [";
				for(int p = 0; p < i->paramCount; p++)
				{
					s += "p";  s+= str(p);
					if(p < i->paramCount-1)
						s += ", ";
				}
				s += "])";
				AddLine(script, s);
			} // for each condition

			// Expressions
			index = 0;
			for(vector<PyFunct>::iterator i = plug->py_expressions.begin(); i != plug->py_expressions.end(); i++, index++)
			{
				if(i->name == "")
					continue;

				if(i->paramCount != 0)
				{
					s += 	"	def ";	s += i->name;	s += "(self";
					for(int p = 0; p < i->paramCount; p++)
						s += ", p"  + str(p);
					s += "):";
					AddLine(script, s);

					s +=	"		return __construct__.te(";	
					s += str((long)(*o));
					s += ", ";
					s += str((int)&((*plug->pvExpRoutines)[index]));

					s += ", [";
					for(int p = 0; p < i->paramCount; p++)
					{
						s += "p";  s+= str(p);
						if(p < i->paramCount-1)
							s += ", ";
					}
					s += "])";
					AddLine(script, s);
				}
			} // for each expression



			s +=	"	def __getattr__(self, name):";
			AddLine(script, s);
			s +=	"		return __construct__.OTGetAtt(";
			s += str((long)(*o));
			s += ", name, [])";
			AddLine(script, s);

			s +=	"	def __setattr__(self, name, key):";
			AddLine(script, s);
			s +=	"		return __construct__.OTSetAtt(";
			s += str((long)(*o));
			s += ", name, key)";
			AddLine(script, s);


			s +=	"	def __getitem__(self,key):";
			AddLine(script, s);
			s +=	"		point = __construct__.ins(";
			s += str((int)(*o));
			s += ", key)";
			AddLine(script, s);
			s +=	"		if(point <> -1):";
			AddLine(script, s);
			s +=	"			obj = " + name + "Instance()";
			AddLine(script, s);
	
			s +=	"			__construct__.ObjSetDict(point, obj.__dict__)";
			AddLine(script, s);

			s +=	"			__construct__.ObjSetAtt(point, \"__instance__\", point)";
			AddLine(script, s);

		//	s +=	"			__construct__.Test()";
		//	AddLine(script, s);	

			s +=	"			return obj";
			AddLine(script, s);
			s +=	"		else:";
			AddLine(script, s);
			s +=	"			raise IndexError()";
			AddLine(script, s);

			s +=	"	def __len__(self):";
			AddLine(script, s);
			s +=	"		return __construct__.cnt(";
			s += str((long)(*o));
			s += ")";
			AddLine(script, s);

			//Create an instance so we can refer to the type as Object instead of Object()

			s += name;
			s += " = ";
			s += (*o)->Name;
			s += "()";
			AddLine(script, s);

			
		} // for each object type

		PythonInit = true;

#ifdef DEBUG
		Export(script);
#endif
		PyRun_String(script, Py_file_input, pDictionary, pDictionary);
		HandlePyErrors();
	}
	return true;


}
void SystemObject::HandlePyErrors()
{
	if(PyErr_Occurred())
	{
		PyErr_Print();
		PyRun_String("__error__ = sys.stderr.getvalue()", Py_file_input, pDictionary, pDictionary);
		PyObject *pResult = PyDict_GetItemString( pDictionary, "__error__" );
		
		if(pResult)
		{
			if(PyString_Check(pResult))
			{
				CString error;
				error.Format("%s", PyString_AsString(pResult) );

				MessageBox(0,error,0,0);
			}
		}
		else
		{
			MessageBox(0,"An error occured in python, but construct is unable to obtain any error information without StringIO.pyc",0,0);
		}
		PyRun_String("sys.stderr.seek(0)", Py_file_input, pDictionary, pDictionary);
		PyRun_String("sys.stderr.truncate(0)", Py_file_input, pDictionary, pDictionary);
	}

}
long SystemObject::aRunScript(LPVAL theParams)
{
	InitPython();
	g_pySystem = this;
	CString script = theParams[0].GetString();
	script.Replace((char)13, '\n');
	PyRun_String( script, Py_file_input, pDictionary, pDictionary );
	g_pySystem = NULL;

	HandlePyErrors();

	return 0;
}

long SystemObject::cCompareScript(LPVAL theParams)
{
	InitPython();
	g_pySystem = this;
	CString script = theParams[0].GetString();
	script.Replace((char)13, '\n');

	PyObject* obj = NULL;
	obj = PyRun_String( script, Py_eval_input, pDictionary, pDictionary );
	g_pySystem = NULL;

	HandlePyErrors();

	if(obj == NULL)
		return false;


	// Get operator ID
	int operatorID = theParams[1].GetInt();
	ExpStore l;
	GetPyParams(&l, obj, 1);
	const ExpReturn& r = theParams[2];

	switch (theParams[0].Type()) {
	case EXPTYPE_INTEGER:
		return DoComparison(operatorID, l.GetInt64(), r.GetInt64());
	case EXPTYPE_FLOAT:
		return DoComparison(operatorID, l.GetDouble(), r.GetDouble());
	case EXPTYPE_STRING:
		return DoComparison(operatorID, *(l.GetStringPtr()), r.GetString());
	default:
		return false;
	}
	return 0;
}
long SystemObject::eRunScript(LPVAL theParams, ExpReturn& ret)
{
	InitPython();
	g_pySystem = this;
	CString script = theParams[0].GetString();
	script.Replace((char)13, '\n');
	PyObject* obj = NULL;
	obj = PyRun_String( script, Py_eval_input, pDictionary, pDictionary );
	g_pySystem = NULL;

	HandlePyErrors();

	if(obj == NULL)
		return 0;
	
	ExpStore p;
	GetPyParams(&p, obj, 1);

	ret.ReturnCustom(pCRuntime, p);
	return 0;
}


#endif