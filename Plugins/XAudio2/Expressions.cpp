// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Built-in expressions
//////////////////////////////////////////////////////////////////////////////////

// Return the Default Value.  This is the value to be returned if your object name
// is used without an expression name, eg. in "Counter + 1".
// Parameters can be passed, eg. MyObject(3,7).  You can check ExpReturn::Type() or GetNumParams() to check for overloads.
long ExtObject::ReturnDefaultValue(LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

// Called for undefined expression names, if your extension allows it.
// Eg: MyObject.UndefinedExpression(3,4,5) calls this routine with
// "UndefinedExpression" as the expression name.  OF_UNDEFINEDEXPRESSIONS must be specified in Main.h.
long ExtObject::ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Expressions
//////////////////////////////////////////////////////////////////////////////////

/*
long ExtObject::eMyExpression(LPVAL params, ExpReturn& ret)
{
	// Return 0 using ExpReturn's operator=
	// Use ret.ReturnString() for strings.
	return ret = 0;
}
*/
long ExtObject::eGetPeakLevel(LPVAL params, ExpReturn& ret)
{
	return ret = XAudio2.GetPeakLevel(params[0].GetInt() - 1);
}

long ExtObject::eGetRMSLevel(LPVAL params, ExpReturn& ret)
{
	return ret = XAudio2.GetRMSLevel(params[0].GetInt() - 1);
}

long ExtObject::eGetAudioCPUUsage(LPVAL params, ExpReturn& ret)
{
	const XAUDIO2_PERFORMANCE_DATA& pd = XAudio2.GetPerformanceData();
	float cpu = 100.0f * ((float)pd.AudioCyclesSinceLastQuery / (float)pd.TotalCyclesSinceLastQuery);
	return ret = cpu;
}

long ExtObject::eGetMemoryUsage(LPVAL params, ExpReturn& ret)
{
	return ret = XAudio2.GetTotalCacheSize() + XAudio2.GetPerformanceData().MemoryUsageInBytes;
}

long ExtObject::eGetLatency(LPVAL params, ExpReturn& ret)
{
	float samples = XAudio2.GetPerformanceData().CurrentLatencyInSamples;
	float outputRate = XAudio2.GetDeviceDetails().OutputFormat.Format.nSamplesPerSec;
	return ret = (samples * 1000.0f) / outputRate;		// return in ms
}

long ExtObject::eGetActiveSources(LPVAL params, ExpReturn& ret)
{
	return ret = XAudio2.GetPerformanceData().ActiveSourceVoiceCount;
}

long ExtObject::eGetMasterVolume(LPVAL params, ExpReturn& ret)
{
	return ret = XAudio2.master.GetVolume();
}

long ExtObject::eGetChannelVolume(LPVAL params, ExpReturn& ret)
{
	return ret = XAudio2.GetVolume(params[0].GetInt() - 1);
}

long ExtObject::eGetChannelPan(LPVAL params, ExpReturn& ret)
{
	return ret = XAudio2.GetPan(params[0].GetInt() - 1);
}

long ExtObject::eGetChannelFreqRatio(LPVAL params, ExpReturn& ret)
{
	return ret = XAudio2.GetFrequencyRatio(params[0].GetInt() - 1);
}

long ExtObject::eGetChannelPosition(LPVAL params, ExpReturn& ret)
{
	return ret = XAudio2.GetPosition(params[0].GetInt() - 1);
}

long ExtObject::eGetChannelLength(LPVAL params, ExpReturn& ret)
{
	return ret = XAudio2.GetLength(params[0].GetInt() - 1);
}

long ExtObject::eGetLastAutoplayChannel(LPVAL params, ExpReturn& ret)
{
	return ret = lastAutoplayChannel + 1;
}

long ExtObject::eGetMaxVol(LPVAL params, ExpReturn& ret)
{
	return ret = data.maxVol;
}

long ExtObject::eGetMinVol(LPVAL params, ExpReturn& ret)
{
	return ret = data.minVol;
}

long ExtObject::eGetMinDist(LPVAL params, ExpReturn& ret)
{
	return ret = data.minDist;
}

long ExtObject::eGetRolloff(LPVAL params, ExpReturn& ret)
{
	return ret = data.rolloff * 100.0f;
}

long ExtObject::eGetMaxPan(LPVAL params, ExpReturn& ret)
{
	return ret = data.maxPan;
}

long ExtObject::eGetPanDist(LPVAL params, ExpReturn& ret)
{
	return ret = data.panDist;
}

long ExtObject::eGetListenerX(LPVAL params, ExpReturn& ret)
{
	return ret = listenerX;
}

long ExtObject::eGetListenerY(LPVAL params, ExpReturn& ret)
{
	return ret = listenerY;
}
