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
//OF_MOVEMENTPLUGIN - Is a movement plugin
//OF_MOVEMENTS - Allow movement plugins to be added
//OF_EFFECTS - Allow display effects
//OF_NOCOMMONDEBUG - Don't show 'Common' field (X/Y/W/H/Angle/Opacity) in debugger
//OF_NODEBUG - Don't show at all in debugger
//OF_UNDEFINEDEXPRESSIONS - Allow ReturnUndefinedExpression to be called
#define IDE_FLAGS 	OF_NODRAW

#include "..\..\Common\ExpReturn.hpp"

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)

// For serializing CXAudio2SetupParameters
class CXAudio2Data : public CXAudio2SetupParameters
{
public:
	// Positional sound parameters
	float maxVol;
	float minVol;
	float minDist;
	float rolloff;
	float maxPan;
	float panDist;

	CXAudio2Data();
	void Serialize(bin& ar);
};

class PositionedSound {
public:
	PositionedSound() :
		x(0.0f),
		y(0.0f),
		obj(NULL),
		active(false)
	{}

	float x, y;
	CRunObject* obj;
	bool active;
};

typedef vector<PositionedSound>				PositionedSoundList;
typedef vector<PositionedSound>::iterator	PositionedSoundIterator;

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

	long cChannelPlaying(LPVAL params);
	long cAnyChannelPlaying(LPVAL params);
	long cChannelReserved(LPVAL params);
	long cMasterMuted(LPVAL params);
	long cChannelMuted(LPVAL params);

	long aAutoplayFile(LPVAL params);
	long aAutoplayResource(LPVAL params);
	long aLoadFile(LPVAL params);
	long aLoadResource(LPVAL params);
	long aPlay(LPVAL params);
	long aStop(LPVAL params);
	long aSetMasterVolume(LPVAL params);
	long aSetMasterMuted(LPVAL params);
	long aSetChannelReserved(LPVAL params);
	long aUnreserveAll(LPVAL params);
	long aSetChannelVolume(LPVAL params);
	long aSetChannelPan(LPVAL params);
	long aSetChannelFreqRatio(LPVAL params);
	long aCacheFile(LPVAL params);
	long aCacheDirectory(LPVAL params);
	long aClearCache(LPVAL params);
	long aExitLoop(LPVAL params);
	long aStopAll(LPVAL params);
	long aSetPosition(LPVAL params);
	long aSetCacheLimit(LPVAL params);

	long aPlayMusic(LPVAL params);
	long aPauseMusic(LPVAL params);
	long aResumeMusic(LPVAL params);
	long aSetMusicVolume(LPVAL params);
	long aPlayMusicResource(LPVAL params);
	long aSetMusicLooping(LPVAL params);
	long aSetChannelMuted(LPVAL params);

	///////////////////////////////////////

	long aSetMaxVol(LPVAL params);
	long aSetMinVol(LPVAL params);
	long aSetMinDist(LPVAL params);
	long aSetRolloff(LPVAL params);
	long aSetMaxPan(LPVAL params);
	long aSetPanDist(LPVAL params);

	long aSetListenerPos(LPVAL params);
	long aSetListenerObj(LPVAL params);

	long aAutoplayFilePos(LPVAL params);
	long aAutoplayFileObj(LPVAL params);
	long aAutoplayResourcePos(LPVAL params);
	long aAutoplayResourceObj(LPVAL params);

	long aAttachChannelToObj(LPVAL params);

	///////////////////////////////////////

	long eGetPeakLevel(LPVAL params, ExpReturn& ret);
	long eGetRMSLevel(LPVAL params, ExpReturn& ret);
	long eGetAudioCPUUsage(LPVAL params, ExpReturn& ret);
	long eGetMemoryUsage(LPVAL params, ExpReturn& ret);
	long eGetLatency(LPVAL params, ExpReturn& ret);
	long eGetActiveSources(LPVAL params, ExpReturn& ret);
	long eGetMasterVolume(LPVAL params, ExpReturn& ret);
	long eGetChannelVolume(LPVAL params, ExpReturn& ret);
	long eGetChannelPan(LPVAL params, ExpReturn& ret);
	long eGetChannelFreqRatio(LPVAL params, ExpReturn& ret);
	long eGetChannelPosition(LPVAL params, ExpReturn& ret);
	long eGetChannelLength(LPVAL params, ExpReturn& ret);
	long eGetLastAutoplayChannel(LPVAL params, ExpReturn& ret);

	//////////////////////////////////////

	long eGetMaxVol(LPVAL params, ExpReturn& ret);
	long eGetMinVol(LPVAL params, ExpReturn& ret);
	long eGetMinDist(LPVAL params, ExpReturn& ret);
	long eGetRolloff(LPVAL params, ExpReturn& ret);
	long eGetMaxPan(LPVAL params, ExpReturn& ret);
	long eGetPanDist(LPVAL params, ExpReturn& ret);

	long eGetListenerX(LPVAL params, ExpReturn& ret);
	long eGetListenerY(LPVAL params, ExpReturn& ret);

	////////////////////////////////////////////////////
	// Data members

	CXAudio2Data data;
	
	CXAudio2 XAudio2;
	Mp3 mp3;
	bool loopMp3;
	map<int, CString> extractedMusic;

	void OnXAudio2Error(const CXAudio2Error& e);

	int lastAutoplayChannel;

	// Positioned sounds data
	float listenerX, listenerY;
	CRunObject* listenerObj;
	PositionedSoundList positionedSounds;

	void SetChannelPositioned(int channel, float x, float y, CRunObject* obj = NULL);
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
	void GetAnimationHandle(int& handle);

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	///////////////////////////
	// Your edittime extension data goes here
	int iTexture;		// DX texture

	int cacheMode_int;



	CXAudio2Data data;
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif