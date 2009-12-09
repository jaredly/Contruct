// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::cChannelPlaying(LPVAL params)
{
	return XAudio2.IsPlaying(params[0].GetInt() - 1);
}

long ExtObject::cAnyChannelPlaying(LPVAL params)
{
	for (int i = 0; i < XAudio2.GetChannelCount(); i++) {
		if (XAudio2.IsPlaying(i))
			return true;
	}

	return false;
}

long ExtObject::cChannelReserved(LPVAL params)
{
	return XAudio2.IsChannelReserved(params[0].GetInt() - 1);
}

long ExtObject::cMasterMuted(LPVAL params)
{
	return XAudio2.master.IsMuted();
}

long ExtObject::cChannelMuted(LPVAL params)
{
	return XAudio2.GetMuted(params[0].GetInt() - 1);
}