// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////


long ExtObject::aAutoplayFile(LPVAL params)
{
	CString file = params[0].GetString();

	lastAutoplayChannel = XAudio2.GetUnusedChannel();

	try {
		XAudio2.LoadFile((const char*)file, lastAutoplayChannel, params[1].GetBool());
	}
	catch (CXAudio2Error e) {
		OnXAudio2Error(e);
		return 0;
	}

	XAudio2.Play(lastAutoplayChannel);

	return 0;
}

long ExtObject::aAutoplayResource(LPVAL params)
{
	int ID = params[0].GetInt();

	lastAutoplayChannel = XAudio2.GetUnusedChannel();

	try {
		XAudio2.LoadResource(ID, lastAutoplayChannel, params[1].GetBool());
	}
	catch (CXAudio2Error e) {
		OnXAudio2Error(e);
		return 0;
	}

	XAudio2.Play(lastAutoplayChannel);

	return 0;
}

long ExtObject::aLoadFile(LPVAL params)
{
	CString file = params[0].GetString();

	try {
		XAudio2.LoadFile((const char*)file, params[1].GetInt() - 1, params[2].GetBool());
	}
	catch (CXAudio2Error e) {
		OnXAudio2Error(e);
		return 0;
	}

	return 0;
}

long ExtObject::aLoadResource(LPVAL params)
{
	int ID = params[0].GetInt();

	try {
		XAudio2.LoadResource(ID, params[1].GetInt() - 1, params[2].GetBool());
	}
	catch (CXAudio2Error e) {
		OnXAudio2Error(e);
		return 0;
	}

	return 0;
}

long ExtObject::aCacheFile(LPVAL params)
{
	CString file = params[0].GetString();

	try {
		XAudio2.PreCacheFile((const char*)file);
	}
	catch (CXAudio2Error e) {
		OnXAudio2Error(e);
		return 0;
	}

	return 0;
}

long ExtObject::aPlay(LPVAL params)
{
	int chan = params[0].GetInt();

	XAudio2.Play(chan - 1);

	return 0;
}

long ExtObject::aStop(LPVAL params)
{
	int chan = params[0].GetInt();

	XAudio2.Stop(chan - 1);

	return 0;
}

long ExtObject::aSetMasterVolume(LPVAL params)
{
	XAudio2.master.SetVolume(params[0].GetFloat());

	return 0;
}

long ExtObject::aSetMasterMuted(LPVAL params)
{
	XAudio2.master.SetMuted(params[0].GetBool());

	return 0;
}

long ExtObject::aSetChannelReserved(LPVAL params)
{
	XAudio2.SetChannelReserved(params[0].GetInt() - 1, !params[1].GetBool());	// Reserved/unreserved combo order

	return 0;
}

long ExtObject::aUnreserveAll(LPVAL params)
{
	XAudio2.SetAllChannelsReserved(false);

	return 0;
}

long ExtObject::aSetChannelVolume(LPVAL params)
{
	XAudio2.SetVolume(params[0].GetInt() - 1, params[1].GetFloat());

	return 0;
}

long ExtObject::aSetChannelPan(LPVAL params)
{
	XAudio2.SetPan(params[0].GetInt() - 1, params[1].GetFloat());

	return 0;
}

long ExtObject::aSetChannelFreqRatio(LPVAL params)
{
	XAudio2.SetFrequencyRatio(params[0].GetInt() - 1, params[1].GetFloat());

	return 0;
}

long ExtObject::aCacheDirectory(LPVAL params)
{
	CString originalpath = params[0].GetString();

	// Add backslash if not present
	if (originalpath.Right(1) != "\\")
		originalpath += "\\";

	const char extensions[3][7] = {"*.wav", "*.xwm", "*.xwma"};

	for (int i = 0; i < 3; i++) {

		CString path = originalpath + extensions[i];

		WIN32_FIND_DATA fd;
		DWORD dwAttr = FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN;
		HANDLE hFind = FindFirstFile(path, &fd);

		if(hFind != INVALID_HANDLE_VALUE)
		{
			do {
				if(!(fd.dwFileAttributes & dwAttr)) {
					try {
						XAudio2.PreCacheFile((const char*)(originalpath + fd.cFileName));
					}
					catch (CXAudio2Error e) {}	// File load failed; do nothing, just failed to cache
				}

			} while (FindNextFile(hFind, &fd));

			FindClose(hFind);
		}
	}//for

	return 0;
}

long ExtObject::aClearCache(LPVAL params)
{
	int option = params[0].GetInt();

	// 0 = clear unused, 1 = clear all
	switch (option) {
	case 0:
		XAudio2.ClearUnusedCache();
		break;
	case 1:
		XAudio2.ClearCache();
		break;
	}

	return 0;
}

long ExtObject::aExitLoop(LPVAL params)
{
	XAudio2.ExitLoop(params[0].GetInt() - 1);
	
	return 0;
}

long ExtObject::aStopAll(LPVAL params)
{
	XAudio2.StopAllChannels();
	
	return 0;
}

long ExtObject::aSetPosition(LPVAL params)
{
	XAudio2.SetPosition(params[0].GetInt() - 1, params[1].GetFloat());
	
	return 0;
}

long ExtObject::aSetCacheLimit(LPVAL params)
{
	XAudio2.SetCacheLimit(params[0].GetFloat() * 1024.0f * 1024.0f);
	
	return 0;
}

long ExtObject::aPlayMusic(LPVAL theParams)
{
	CString path = theParams[0].GetString();

	mp3.Cleanup();
	mp3.Load(path);
	mp3.Play();

	return 0;
}

long ExtObject::aPauseMusic(LPVAL theParams)
{
	mp3.Pause();
	return 0;
}

long ExtObject::aResumeMusic(LPVAL theParams)
{
	mp3.Play();
	return 0;
}

long ExtObject::aSetMusicVolume(LPVAL theParams)
{
	mp3.SetVolume(theParams[0].GetDouble());
	return 0;
}

HGLOBAL OpenResourceBinary(int id, const char* type, BYTE*& pData, int& size)
{
	HRSRC theResource = FindResource(NULL, MAKEINTRESOURCE(id), type);

	int e = GetLastError();
	if (theResource == NULL) {
		return NULL;
	}

	HGLOBAL theData = LoadResource(NULL, theResource);
	pData = (BYTE*)LockResource(theData);
	size = SizeofResource(NULL, theResource);
	return theData;	// Must call FreeResource() on this
}

long ExtObject::aPlayMusicResource(LPVAL theParams)
{
	int resID = theParams[0].GetInt();

	// Only extract resources once.
	map<int, CString>::iterator i = extractedMusic.find(resID);

	CString path;

	if (i == extractedMusic.end()) {
		// Add new
		BYTE* pData;
		int size;
		HGLOBAL hResource = OpenResourceBinary(resID, "FILES", pData, size);

		char strbuf[MAX_PATH];
		GetTempPath(MAX_PATH, strbuf);
		path.Format("%scsres%d.bin", strbuf, resID);	// eg. csres5.bin

		FILE* tempfile = fopen(path, "wb");

		// If the file couldn't be loaded, it's already been extracted.
		if (tempfile != NULL) {
			fwrite(pData, size, 1, tempfile);
			fclose(tempfile);

			FreeResource(hResource);

			// Insert record for resource ID
			extractedMusic[resID] = path;
		}
	}
	else
		path = i->second;

	mp3.Cleanup();
	mp3.Load(path);
	mp3.Play();

	return 0;
}

long ExtObject::aSetMusicLooping(LPVAL params)
{
	loopMp3 = params[0].GetBool();
	return 0;
}

long ExtObject::aSetChannelMuted(LPVAL params)
{
	XAudio2.SetMuted(params[0].GetInt() - 1, params[1].GetBool());
	return 0;
}

long ExtObject::aSetMaxVol(LPVAL params)
{
	data.maxVol = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetMinVol(LPVAL params)
{
	data.minVol = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetMinDist(LPVAL params)
{
	data.minDist = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetRolloff(LPVAL params)
{
	data.rolloff = params[0].GetFloat() / 100.0f;
	return 0;
}

long ExtObject::aSetMaxPan(LPVAL params)
{
	data.maxPan = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetPanDist(LPVAL params)
{
	data.panDist = params[0].GetFloat();
	return 0;
}


long ExtObject::aSetListenerPos(LPVAL params)
{
	listenerX = params[0].GetFloat();
	listenerY = params[1].GetFloat();
	return 0;
}

long ExtObject::aSetListenerObj(LPVAL params)
{
	listenerObj = params[0].GetObjectParamFirstInstance(pRuntime);
	return 0;
}

long ExtObject::aAutoplayFilePos(LPVAL params)
{
	// First params are entirely compatible with a normal autoplay file action
	aAutoplayFile(params);

	SetChannelPositioned(lastAutoplayChannel, params[2].GetFloat(), params[3].GetFloat());

	return 0;
}

long ExtObject::aAutoplayFileObj(LPVAL params)
{
	// First params are entirely compatible with a normal autoplay file action
	aAutoplayFile(params);

	SetChannelPositioned(lastAutoplayChannel, 0, 0, params[2].GetObjectParamFirstInstance(pRuntime));

	return 0;
}

long ExtObject::aAutoplayResourcePos(LPVAL params)
{
	// First params are entirely compatible with a normal autoplay resource action
	aAutoplayResource(params);

	SetChannelPositioned(lastAutoplayChannel, params[2].GetFloat(), params[3].GetFloat());

	return 0;
}

long ExtObject::aAutoplayResourceObj(LPVAL params)
{
	// First params are entirely compatible with a normal autoplay resource action
	aAutoplayResource(params);

	SetChannelPositioned(lastAutoplayChannel, 0, 0, params[2].GetObjectParamFirstInstance(pRuntime));

	return 0;
}


long ExtObject::aAttachChannelToObj(LPVAL params)
{
	// Channel must be playing first
	if (XAudio2.IsPlaying(params[0].GetInt() - 1))
		SetChannelPositioned(params[0].GetInt() - 1, 0, 0, params[1].GetObjectParamFirstInstance(pRuntime));

	return 0;
}