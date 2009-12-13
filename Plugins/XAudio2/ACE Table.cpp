// Include StdAfx
#include "StdAfx.h"

// Include any used common ACE definitions.
#ifdef RUN_ONLY
#include "..\..\Common\CommonAceDef.hpp"
#endif

/////////////////////////////////////
// Common runtime & edittime code
CXAudio2Data::CXAudio2Data()
:	CXAudio2SetupParameters(),
	maxVol(0.0f),
	minVol(-40.0f),
	minDist(100.0f),
	rolloff(1.0f),
	maxPan(18.0f),
	panDist(2000.0f),
	enableTimescale(true)
{
}

void CXAudio2Data::Serialize(bin& ar)
{
	int Version = 4;

	if (ar.loading) {
		ar >> Version;

		int temp;
		ar >> numChannels >> maxFrequencyRatio >> masterGain >> temp >> cacheLimit;

		cacheMode = (CXAudio2::CacheMode)temp;

		if (Version >= 2)
			ar >> maxVol >> minVol >> minDist >> rolloff >> maxPan >> panDist;

		if (Version >= 3)
			ar >> limiter_enable >> limiter_release >> limiter_threshold;

		if (Version >= 4)
			ar >> enableTimescale;
	}
	else {
		ar << Version;

		ar << numChannels << maxFrequencyRatio << masterGain << (int)cacheMode << cacheLimit;

		//v2
		ar << maxVol << minVol << minDist << rolloff << maxPan << panDist;

		//v3
		ar << limiter_enable << limiter_release << limiter_threshold;

		//v4
		ar << enableTimescale;
	}
}

//////////////////////////////////////////////////////////////////////////////////
// ACE table
//////////////////////////////////////////////////////////////////////////////////

// Standard channel parameter referring to an XAudio2 channel
#define ADDCHANNELPARAM() ADDPARAMDEF(PARAM_VALUE, "Channel", "Channel number to load to, from 1 to the number of channels.", "1")
#define ADDVOLUMEPARAM() ADDPARAMDEF(PARAM_VALUE, "Volume", "Volume in decibels.  Negative values attenuate, positive values amplify, 0 leaves unchanged.", "0")
#define ADDPANPARAM() ADDPARAMDEF(PARAM_VALUE, "Pan", "Pan value, in decibels.  +/-18 dB is generally perceived as totally one sided.", "0")
#define ADDFREQRATIOPARAM() ADDPARAMDEF(PARAM_VALUE, "Frequency ratio", "Frequency ratio of playback, eg. 0.5 is half as fast.", "1.0")
#define ADDLOOPPARAM() ADDPARAMCOMBO("Loop?", "Choose whether to loop the file when played.", "No loop|Loop")

void DefineACES(MicroAceTime* at)
{
	// Define your ACE tables here.
	// Use ADDPARAM before each ADDACT/ADDCND/ADDEXP to add parameters.
	// See the documentation for full details.

	// Note in the display string, %o is your object icon and %0-%9 make up parameters.
	// Note the script name should be a name suitable for the routine as if it were a member function in a
	// scripting language, eg. "Clear Stored Items" -> "ClearStoredItems"

	/////////////////////////////
	// Conditions
	// Format:
	// ADDCND(List name, Category, Display string, Function address, Script name, Flags)
	ADDCHANNELPARAM();
	ADDCND("Channel is playing?", "Channel", "%o Channel %0 is playing", &ExtObject::cChannelPlaying, "IsPlaying", 0);

	ADDCND("Any channel is playing?", "Channel", "%o Any channel is playing", &ExtObject::cAnyChannelPlaying, "AnyChannelPlaying", 0);

	ADDCHANNELPARAM();
	ADDCND("Channel is reserved?", "Channel", "%o Channel %0 is reserved", &ExtObject::cChannelReserved, "IsReserved", 0);

	ADDCND("Master is muted?", "Master", "%o Master is muted", &ExtObject::cMasterMuted, "IsMasterMuted", 0);

	ADDCHANNELPARAM();
	ADDCND("Channel is muted?", "Channel", "%o Channel %0 is muted", &ExtObject::cChannelMuted, "IsChannelMuted", 0);

	/////////////////////////////
	// Actions
	// Format:
	// ADDACT(List name, Category, Display string, Function address, Script name, Flags)
	//ADDPARAM(PARAM_VALUE, "Example parameter", "Here is an example parameter.");
	//ADDACT("My action", "My category", "Example action (%0)", &ExtObject::aMyAction, "MyAction", 0);

	ADDPARAMDEF(PARAM_STRING, "Filename to load", "Filename of an audio file to load: WAV (PCM, ADPCM, xWMA format), or .OGG - see documentation for details.", "AppPath & \"example.wav\"");
	ADDLOOPPARAM();
	ADDACT("*Autoplay file", "Files", "Autoplay file %0 (%1)", &ExtObject::aAutoplayFile, "AutoplayFile", 0);

	ADDPARAMDEF(PARAM_STRING, "Filename to load", "Filename of an audio file to load: WAV (PCM, ADPCM, xWMA format), or .OGG - see documentation for details.", "AppPath & \"example.wav\"");
	ADDCHANNELPARAM();
	ADDLOOPPARAM();
	ADDACT("Load file", "Files", "Load file %0 to channel %1 (%2)", &ExtObject::aLoadFile, "LoadFile", 0);

	ADDCHANNELPARAM();
	ADDACT("Play", "Channel", "Play channel %0", &ExtObject::aPlay, "Play", 0);

	ADDCHANNELPARAM();
	ADDACT("Stop", "Channel", "Stop channel %0", &ExtObject::aStop, "Stop", 0);

	ADDCHANNELPARAM();
	ADDVOLUMEPARAM();
	ADDACT("Set volume", "Channel", "Set channel %0 volume to %1 dB", &ExtObject::aSetChannelVolume, "SetVolume", 0);

	ADDCHANNELPARAM();
	ADDPANPARAM();
	ADDACT("Set pan", "Channel", "Set channel %0 pan to %1", &ExtObject::aSetChannelPan, "SetPan", 0);

	ADDCHANNELPARAM();
	ADDPARAM(PARAM_VALUE, "Position, in seconds", "The position, in seconds, to set the channel to.");
	ADDACT("Set position", "Channel", "Set channel %0 position to %1 seconds", &ExtObject::aSetPosition, "SetPosition", 0);

	ADDCHANNELPARAM();
	ADDFREQRATIOPARAM();
	ADDACT("Set frequency ratio", "Channel", "Set channel %0 frequency ratio to %1", &ExtObject::aSetChannelFreqRatio, "SetFreqRatio", 0);

	ADDCHANNELPARAM();
	ADDPARAMCOMBO("Reserved?", "Select whether to set this channel reserved or unreserved.", "Reserved|Unreserved");
	ADDACT("Set reserved", "Channel", "Set channel %0 %1", &ExtObject::aSetChannelReserved, "SetChannelReserved", 0);

	ADDACT("Unreserve all channels", "Channel", "Unreserve all channels", &ExtObject::aUnreserveAll, "UnreserveAll", 0);

	ADDCHANNELPARAM();
	ADDACT("Exit loop", "Channel", "Exit loop on channel %0", &ExtObject::aExitLoop, "ExitLoop", 0);

	ADDACT("Stop all channels", "Channel", "Stop all channels", &ExtObject::aStopAll, "StopAll", 0);

	ADDVOLUMEPARAM();
	ADDACT("Set volume", "Master", "Set master volume to %0", &ExtObject::aSetMasterVolume, "SetMasterVolume", 0);

	ADDPARAMCOMBO("Muted?", "Select whether the master output is muted (silent).", "Unmuted|Muted");
	ADDACT("Set muted", "Master", "Set master %0", &ExtObject::aSetMasterMuted, "SetMasterMuted", 0);

	ADDPARAM(PARAM_STRING, "Filename to load", "Filename of an audio file to cache.");
	ADDACT("Cache file", "Cache", "Cache file %0", &ExtObject::aCacheFile, "CacheFile", 0);

	ADDPARAMDEF(PARAM_STRING, "Directory to cache", "Caches all *.wav,*.xwm,*.xwma files in directory.  This will not cache OGG files.", "AppPath");
	ADDACT("Cache directory", "Cache", "Cache directory %0", &ExtObject::aCacheDirectory, "CacheDirectory", 0);

	ADDPARAMCOMBO("What to free", "Choose which parts of the cache to free.", "Free unused cache|Free entire cache (stops all sounds)");
	ADDACT("Free cache", "Cache", "%0", &ExtObject::aClearCache, "ClearCache", 0);

	ADDPARAMDEF(PARAM_VALUE, "Cache limit (megabytes)", "When cache limiting is enabled, the maximum size sound to keep in memory, in megabytes.", "3.0");
	ADDACT("Set cache limit", "Cache", "Set cache limit to %0", &ExtObject::aSetCacheLimit, "SetCacheLimit", 0);

	ADDPARAM(PARAM_BINARYRESOURCE, "Resource to load", "Audio resource to load.  Must be .wav file.");
	ADDLOOPPARAM();
	ADDACT("*Autoplay resource", "Resources", "Autoplay resource %0 (%1)", &ExtObject::aAutoplayResource, "AutoplayResource", 0);

	ADDPARAM(PARAM_BINARYRESOURCE, "Resource to load", "Audio resource to load.  Must be .wav file.");
	ADDCHANNELPARAM();
	ADDLOOPPARAM();
	ADDACT("Load resource", "Resources", "Load resource %0 to channel %1 (%2)", &ExtObject::aLoadResource, "LoadResource", 0);



	ADDPARAMDEF(PARAM_STRING, "Music file", "Filename of music to play: any Directshow compatible format, eg. WMA, MP3 - see documentation for details.", "AppPath & \"example.mp3\"");
	ADDACT("*Play music from file", "Music", "Play music %0", &ExtObject::aPlayMusic, "PlayMusic", 0);

	ADDPARAM(PARAM_BINARYRESOURCE, "Resource", "Resource to load music from: any Directshow compatible format, eg. WMA, MP3 - see documentation for details.");
	ADDACT("*Play music from resource", "Music", "Play music %0", &ExtObject::aPlayMusicResource, "PlayMusicResource", 0);

	ADDACT("Pause music", "Music", "Pause music", &ExtObject::aPauseMusic, "PauseMusic", 0);

	ADDACT("Resume music", "Music", "Resume music", &ExtObject::aResumeMusic, "ResumeMusic", 0);

	ADDPARAMDEF(PARAM_VALUE, "Volume", "New music volume, from 0-100", "100");
	ADDACT("Set volume", "Music", "Set music volume to %0", &ExtObject::aSetMusicVolume, "SetMusicVolume", 0);

	ADDPARAMCOMBO("Loop music", "Choose whether to loop the music.", "Don't loop|Loop");
	ADDACT("Set music looping", "Music", "%0 music", &ExtObject::aSetMusicLooping, "SetMusicLooping", 0);

	ADDCHANNELPARAM();
	ADDPARAMCOMBO("Muted?", "Select whether the channel is muted (silent).", "Unmuted|Muted");
	ADDACT("Set muted", "Channel", "Set channel %0 %1", &ExtObject::aSetChannelMuted, "SetChannelMuted", 0);

	ADDPARAM(PARAM_VALUE, "Maximum volume", "The maximum volume for positioned sounds, in dB attenuation");
	ADDACT("Set max volume", "Settings", "Set max volume to %0", &ExtObject::aSetMaxVol, "SetMaxVol", 0);

	ADDPARAM(PARAM_VALUE, "Minimum volume", "The minimum volume for positioned sounds, in dB attenuation");
	ADDACT("Set min volume", "Settings", "Set min volume to %0", &ExtObject::aSetMinVol, "SetMinVol", 0);

	ADDPARAM(PARAM_VALUE, "Minimum distance", "The range in which positioned sounds play at the maximum volume, in pixels.");
	ADDACT("Set min distance", "Settings", "Set minimum distance to %0", &ExtObject::aSetMinDist, "SetMinDist", 0);

	ADDPARAMDEF(PARAM_VALUE, "Rolloff factor", "The rolloff factor, as a percentage 0-100, determining how quickly sounds attenuate with distance.", "100");
	ADDACT("Set rolloff", "Settings", "Set rolloff factor to %0 %", &ExtObject::aSetRolloff, "SetRolloff", 0);

	ADDPARAM(PARAM_VALUE, "Maximum pan", "The maximum pan range, in dB attenuation");
	ADDACT("Set max pan", "Settings", "Set max pan to %0", &ExtObject::aSetMaxPan, "SetMaxPan", 0);

	ADDPARAM(PARAM_VALUE, "Pan distance", "The maximum horizontal distance, in pixels, at which sounds reach maximum pan.");
	ADDACT("Set pan distance", "Settings", "Set pan distance to %0", &ExtObject::aSetPanDist, "SetPanDist", 0);

	ADDPARAM(PARAM_VALUE, "Listener X", "X co-ordinate of the listener for positioned sounds.");
	ADDPARAM(PARAM_VALUE, "Listener Y", "Y co-ordinate of the listener for positioned sounds.");
	ADDACT("Set listener position", "Positioned sounds", "Set listener position to %0, %1", &ExtObject::aSetListenerPos, "SetListenerPos", 0);

	ADDPARAM(PARAM_OBJECT, "Listener object", "Choose an object to represent the listener for positioned sounds.");
	ADDACT("Set listener object", "Positioned sounds", "Set listener to %0", &ExtObject::aSetListenerObj, "SetListenerObj", 0);

	ADDPARAMDEF(PARAM_STRING, "Filename to load", "Filename of an audio file to load: WAV (PCM, ADPCM, xWMA format), or .OGG - see documentation for details.", "AppPath & \"example.wav\"");
	ADDLOOPPARAM();
	ADDPARAM(PARAM_VALUE, "X co-ordinate", "X co-ordinate at which to position this sound.");
	ADDPARAM(PARAM_VALUE, "Y co-ordinate", "Y co-ordinate at which to position this sound.");
	ADDACT("*Autoplay file at position", "Positioned sounds", "Autoplay file %0 (%1) at %2, %3", &ExtObject::aAutoplayFilePos, "AutoplayFilePos", 0);

	ADDPARAMDEF(PARAM_STRING, "Filename to load", "Filename of an audio file to load: WAV (PCM, ADPCM, xWMA format), or .OGG - see documentation for details.", "AppPath & \"example.wav\"");
	ADDLOOPPARAM();
	ADDPARAM(PARAM_OBJECT, "Object", "Choose the object to position the sound at.");
	ADDACT("*Autoplay file at object", "Positioned sounds", "Autoplay file %0 (%1) at %2", &ExtObject::aAutoplayFileObj, "AutoplayFileObj", 0);

	ADDPARAM(PARAM_BINARYRESOURCE, "Resource to load", "Audio resource to load: .wav files only.");
	ADDLOOPPARAM();
	ADDPARAM(PARAM_VALUE, "X co-ordinate", "X co-ordinate at which to position this sound.");
	ADDPARAM(PARAM_VALUE, "Y co-ordinate", "Y co-ordinate at which to position this sound.");
	ADDACT("*Autoplay resource at position", "Positioned sounds", "Autoplay resource %0 (%1) at %2, %3", &ExtObject::aAutoplayResourcePos, "AutoplayResourcePos", 0);

	ADDPARAM(PARAM_BINARYRESOURCE, "Resource to load", "Audio resource to load: .wav files only.");
	ADDLOOPPARAM();
	ADDPARAM(PARAM_OBJECT, "Object", "Choose the object to position the sound at.");
	ADDACT("*Autoplay resource at object", "Positioned sounds", "Autoplay resource %0 (%1) at %2", &ExtObject::aAutoplayResourceObj, "AutoplayResourceObj", 0);

	ADDCHANNELPARAM();
	ADDPARAM(PARAM_OBJECT, "Object", "Choose the object to position the channel at.");
	ADDACT("Attach channel to object", "Positioned sounds", "Attach channel %0 to %1", &ExtObject::aAttachChannelToObj, "AttachChannelTo", 0);

	/////////////////////////////
	// Expressions
	// ADDEXP(List name, Category, Display string, Function address, Flags)
	ADDPARAMDEF(PARAM_VALUE, "Output channel", "Output channel to get stats for.", "1");
	ADDEXP("Get peak level", "Audio statistics", "PeakLevel", &ExtObject::eGetPeakLevel, RETURN_FLOAT);

	ADDPARAMDEF(PARAM_VALUE, "Output channel", "Output channel to get stats for.", "1");
	ADDEXP("Get RMS level", "Audio statistics", "RMSLevel", &ExtObject::eGetRMSLevel, RETURN_FLOAT);

	ADDEXP("Get audio CPU usage %", "Audio statistics", "CPUUsage", &ExtObject::eGetAudioCPUUsage, RETURN_FLOAT);
	ADDEXP("Get memory usage", "Audio statistics", "MemBytes", &ExtObject::eGetMemoryUsage, RETURN_INTEGER);
	ADDEXP("Get latency", "Audio statistics", "Latency", &ExtObject::eGetLatency, RETURN_FLOAT);
	ADDEXP("Get active channels", "Audio statistics", "ActiveChannels", &ExtObject::eGetActiveSources, RETURN_INTEGER);

	ADDEXP("Get master volume", "Master", "MasterVolume", &ExtObject::eGetMasterVolume, RETURN_FLOAT);

	ADDPARAMDEF(PARAM_VALUE, "Channel", "Channel", "1");
	ADDEXP("Get position", "Channel", "Position", &ExtObject::eGetChannelPosition, RETURN_FLOAT);

	ADDPARAMDEF(PARAM_VALUE, "Channel", "Channel", "1");
	ADDEXP("Get length", "Channel", "Length", &ExtObject::eGetChannelLength, RETURN_FLOAT);

	ADDEXP("Get last autoplayed channel", "Autoplay", "LastChannel", &ExtObject::eGetLastAutoplayChannel, RETURN_INTEGER);

	ADDEXP("Get max volume", "Positioned sounds", "MaxVol", &ExtObject::eGetMaxVol, RETURN_FLOAT);
	ADDEXP("Get min volume", "Positioned sounds", "MinVol", &ExtObject::eGetMinVol, RETURN_FLOAT);
	ADDEXP("Get min distance", "Positioned sounds", "MinDist", &ExtObject::eGetMinDist, RETURN_FLOAT);
	ADDEXP("Get rolloff", "Positioned sounds", "Rolloff", &ExtObject::eGetRolloff, RETURN_FLOAT);
	ADDEXP("Get max pan", "Positioned sounds", "MaxPan", &ExtObject::eGetMaxPan, RETURN_FLOAT);
	ADDEXP("Get pan distance", "Positioned sounds", "PanDist", &ExtObject::eGetPanDist, RETURN_FLOAT);
	ADDEXP("Get listener X", "Positioned sounds", "ListenerX", &ExtObject::eGetListenerX, RETURN_FLOAT);
	ADDEXP("Get listener Y", "Positioned sounds", "ListenerY", &ExtObject::eGetListenerY, RETURN_FLOAT);

	ADDPARAMDEF(PARAM_VALUE, "Channel", "Channel", "1");
	ADDEXP("Get volume", "Channel", "Volume", &ExtObject::eGetChannelVolume, RETURN_FLOAT);

	ADDPARAMDEF(PARAM_VALUE, "Channel", "Channel", "1");
	ADDEXP("Get pan", "Channel", "Pan", &ExtObject::eGetChannelPan, RETURN_FLOAT);

	ADDPARAMDEF(PARAM_VALUE, "Channel", "Channel", "1");
	ADDEXP("Get frequency ratio", "Channel", "FreqRatio", &ExtObject::eGetChannelFreqRatio, RETURN_FLOAT);

#include "..\..\Common\CommonAceTable.hpp"
}


