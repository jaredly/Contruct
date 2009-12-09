/*
	CXAudio2 wrapper class for 2D game audio
	Copyright (C) Ashley Gullen 2008

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "stdafx.h"
#include "cxaudio2.h"
#include "wavefile.h"
#include "XAPOFX.h"

// CXAudio2Error throwing class
CXAudio2Error::CXAudio2Error(HRESULT _hr, std::string _name)
:	hr(_hr),
	name(_name)
{
}

const char* CXAudio2Error::GetName() const
{
	return name.c_str();
}

HRESULT CXAudio2Error::GetHR() const
{
	return hr;
}

// SetupParameters constructor: set defaults
CXAudio2::SetupParameters::SetupParameters()
:	numChannels(64),							// Default 64 channels
	maxFrequencyRatio(XAUDIO2_DEFAULT_FREQ_RATIO),
	masterGain(-3.0f),							// Master defaults to -3dB, this will help prevent clipping
	commandBatching(CXAudio2::CBT_IMMEDIATE),	// Commit audio functions immediately
	cacheMode(CACHE_NOTOGG),					// Don't cache OGG files, cache everything else
	cacheLimit(3*1024*1024),					// 3mb default cache limit
	limiter_enable(true),
	limiter_release(6),
	limiter_threshold(1000)
{
}

// CXAudio2 constructor
CXAudio2::CXAudio2()
:	numChannels(0),		// No channels at construction
	pXAudio2(NULL),
	maxFrequencyRatio(XAUDIO2_DEFAULT_FREQ_RATIO),
	totalCacheSize(0),
	nextChannel(0),
	operationSet(XAUDIO2_COMMIT_NOW),
	master(this),
	cbt(CBT_IMMEDIATE),
	cacheMode(CACHE_NOTOGG),
	cacheLimit(3*1024*1024),
	timeScale(1.0f)
{
}

// Initialisation of the CXAudio2 engine
void CXAudio2::Init(const CXAudio2SetupParameters& params)
{
	// Copy parameters
	numChannels = params.numChannels;
	maxFrequencyRatio = params.maxFrequencyRatio;
	SetCommandBatching(params.commandBatching);
	SetCacheMode(params.cacheMode);
	SetCacheLimit(params.cacheLimit);

	// Initialize COM
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

#ifdef _DEBUG
	// Request debug XAudio2 engine
	hr = XAudio2Create(&pXAudio2, XAUDIO2_DEBUG_ENGINE,	XAUDIO2_DEFAULT_PROCESSOR);
#else
	// Request standard XAudio2 engine
	hr = XAudio2Create(&pXAudio2, 0,					XAUDIO2_DEFAULT_PROCESSOR);
#endif

	// XAudio2Create failed
	if (FAILED(hr))
		throw CXAudio2Error(hr, "Failed to create XAudio2");

	// Mastering limiter & volume meter effect chain for mastering voice
	XAUDIO2_EFFECT_DESCRIPTOR descriptor[2];
	int count = 0;

	// Create the mastering limiter if enabled
	if (params.limiter_enable) {
		IUnknown* pLimiter;
		hr = CreateFX(__uuidof(FXMasteringLimiter), &pLimiter);

		if (FAILED(hr))
			throw CXAudio2Error(hr, "Failed to create mastering limiter");

		descriptor[0].InitialState = true;
		descriptor[0].OutputChannels = 2;
		descriptor[0].pEffect = pLimiter;

		// Write volume meter to next descriptor
		count++;
	}

	// Create the volume meter for the mastering voice
#ifdef _DEBUG
	hr = XAudio2CreateVolumeMeter(&(master.pVolumeMeter), XAUDIO2FX_DEBUG);
#else
	hr = XAudio2CreateVolumeMeter(&(master.pVolumeMeter), 0);
#endif

	if (FAILED(hr))
		throw CXAudio2Error(hr, "Failed to create XAudio2 volume meter");

	// Set up the effect chain to apply the volume metering to the mastering voice
	descriptor[count].InitialState = true;
	descriptor[count].OutputChannels = 2;
	descriptor[count].pEffect = master.pVolumeMeter;
	master.volumeMeterEffectIndex = count;
	count++;

	XAUDIO2_EFFECT_CHAIN chain;
	chain.EffectCount = count;
	chain.pEffectDescriptors = descriptor;

	// Create stereo mastering voice
	hr = pXAudio2->CreateMasteringVoice(&(master.pMaster), 2, XAUDIO2_DEFAULT_SAMPLERATE, 
										0, 0, &chain);

	if (FAILED(hr))
		throw CXAudio2Error(hr, "Failed to create XAudio2 mastering voice");

	// Get the device details
	pXAudio2->GetDeviceDetails(0, &deviceDetails);

	// Set up the channel strip
	channels.reserve(numChannels);

	for (int i = 0; i < numChannels; i++)
		channels.push_back(Channel(this, i));

	// Set the master volume as per parameters; -3.0dB by default to help prevent clipping
	master.SetVolume(params.masterGain);
}

void CXAudio2::Close()
{
	// Free channels
	ChannelIterator i = channels.begin();

	for ( ; i != channels.end(); i++)
		i->Free();

	if (master.pVolumeMeter) {
		master.pVolumeMeter->Release();
		master.pVolumeMeter = NULL;
	}

	if (master.pMaster) {
		master.pMaster->DestroyVoice();
		master.pMaster = NULL;
	}

	if (pXAudio2) {
		pXAudio2->Release();
		pXAudio2 = NULL;
	}
}

CXAudio2::~CXAudio2()
{
	Close();
}

void CXAudio2::SetCommandBatching(CXAudio2::CommandBatchType _cbt)
{
	cbt = _cbt;

	switch (cbt) {
	case CBT_IMMEDIATE:
		operationSet = XAUDIO2_COMMIT_NOW;
		break;
	case CBT_NEXTTICK:
		operationSet = 1;	// All operations occur on operation set 1
		break;
	}
}

CXAudio2::CommandBatchType CXAudio2::GetCommandBatching() const
{
	return cbt;
}

// Update the audio engine
void CXAudio2::Tick(float _timeScale)
{
	if (pXAudio2 == NULL)
		return;

	// Update performance statistics
	pXAudio2->GetPerformanceData(&performanceData);

	// Update the volume meter
	master.UpdateVolumeMeter(deviceDetails.OutputFormat.Format.nChannels);

	// Update timescaling
	if (_timeScale != timeScale) {
		timeScale = _timeScale;

		// Update all channels to the new timescale
		ChannelIterator i = channels.begin();

		for ( ; i != channels.end(); i++) {

			// Updates the frequency ratio to the new timescaled value
			i->SetFrequencyRatio(i->GetFrequencyRatio());
		}
	}

	// Commit any pending audio operations
	if (cbt == CBT_NEXTTICK)
		pXAudio2->CommitChanges(XAUDIO2_COMMIT_ALL);

	// Check the cache
	if (cacheMode != CACHE_ALL) {
		ChannelIterator i = channels.begin();

		for ( ; i != channels.end(); i++) {

			// Buffer finished and idle
			if (i->HasFinishedReadingBuffer()) {
				switch (cacheMode) {

				// Cache off: free buffer to minimise memory usage since its no longer in use
				case CACHE_OFF:
					i->Free();
					break;

				// Cache limited: free buffer if it's over the cache limit
				case CACHE_LIMIT:
					if (i->pCacheEntry != NULL && i->pCacheEntry->size > cacheLimit)
						i->Free();
					break;

				// Don't cache OGG files: free buffer if it's an OGG buffer
				case CACHE_NOTOGG:
					if (i->pCacheEntry != NULL && i->pCacheEntry->isOgg)
						i->Free();
					break;
				}
			}
		}
	}
}

float CXAudio2::GetPeakLevel(int chan) const
{
	if (chan < 0 || chan >= master.peakLevels.size())
		return 0.0f;
	
	return master.peakLevels[chan];
}

float CXAudio2::GetRMSLevel(int chan) const
{
	if (chan < 0 || chan >= master.rmsLevels.size())
		return 0.0f;

	return master.rmsLevels[chan];
}

const XAUDIO2_PERFORMANCE_DATA& CXAudio2::GetPerformanceData() const
{
	return performanceData;
}

const XAUDIO2_DEVICE_DETAILS& CXAudio2::GetDeviceDetails() const
{
	return deviceDetails;
}

int CXAudio2::GetTotalCacheSize() const
{
	return totalCacheSize;
}

// Load a file to a channel
void CXAudio2::LoadFile(std::string filename, int chan, bool loop)
{
	if (pXAudio2 == NULL)
		return;
	if (chan < 0 || chan >= channels.size())
		return;

	// Load file in to cache
	StringCacheIterator i = _PreCacheFile(filename);

	GENERICWAVEFILE* pgwf = &(i->second.gwf);
	DWORD size = i->second.size;
	BYTE* pBuffer = i->second.pBuffer;
	i->second.refCount++;		// Increase cache entry reference count

	// Ready to set the channel
	Channel& c = channels[chan];
	c.Free();
	c.pCacheEntry = &(i->second);	// Point channel to source buffer in cache
	hr = pXAudio2->CreateSourceVoice(&(c.pVoice), (WAVEFORMATEX*)pgwf, 0, maxFrequencyRatio, NULL, NULL, NULL);

	if (FAILED(hr))
		throw CXAudio2Error(hr, "Failed to create a source voice");
 
	// Submit the buffer
	XAUDIO2_BUFFER voicebuffer;
	voicebuffer.Flags = XAUDIO2_END_OF_STREAM;	// no more data to follow
	voicebuffer.AudioBytes = size;
	voicebuffer.pAudioData = pBuffer;
	voicebuffer.pContext = &(*i);	// Take address of CacheEntry to decrement reference count when done
	voicebuffer.PlayBegin = 0;
	voicebuffer.PlayLength = 0;		// play entire buffer
	voicebuffer.LoopBegin = 0;
	voicebuffer.LoopLength = 0;
	voicebuffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

	bool isXWma = i->second.isWma;
	XAUDIO2_BUFFER_WMA wmaBuffer;

	// Cache entry is xWMA format
	if (isXWma) {
		wmaBuffer.PacketCount = i->second.packetCount;
		wmaBuffer.pDecodedPacketCumulativeBytes = &(i->second.decodeArr.front());		
	}

	hr = c.pVoice->SubmitSourceBuffer(&voicebuffer, isXWma ? &wmaBuffer : NULL);

	if (FAILED(hr))
		throw CXAudio2Error(hr, "Failed to submit a source buffer");
}

// Load a resource to a channel
void CXAudio2::LoadResource(int ID, int chan, bool loop)
{
	if (pXAudio2 == NULL)
		return;
	if (chan < 0 || chan >= channels.size())
		return;

	// Load file in to cache
	IntCacheIterator i = _PreCacheResource(ID);

	GENERICWAVEFILE* pgwf = &(i->second.gwf);
	DWORD size = i->second.size;
	BYTE* pBuffer = i->second.pBuffer;
	i->second.refCount++;		// Increase cache entry reference count

	// Ready to set the channel
	Channel& c = channels[chan];
	c.Free();
	c.pCacheEntry = &(i->second);	// Point channel to source buffer in cache
	hr = pXAudio2->CreateSourceVoice(&(c.pVoice), (WAVEFORMATEX*)pgwf, 0, maxFrequencyRatio, NULL, NULL, NULL);

	if (FAILED(hr))
		throw CXAudio2Error(hr, "Failed to create a source voice");
 
	// Submit the buffer
	XAUDIO2_BUFFER voicebuffer;
	voicebuffer.Flags = XAUDIO2_END_OF_STREAM;	// no more data to follow
	voicebuffer.AudioBytes = size;
	voicebuffer.pAudioData = pBuffer;
	voicebuffer.pContext = &(*i);	// Take address of CacheEntry to decrement reference count when done
	voicebuffer.PlayBegin = 0;
	voicebuffer.PlayLength = 0;		// play entire buffer
	voicebuffer.LoopBegin = 0;
	voicebuffer.LoopLength = 0;
	voicebuffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

	bool isXWma = i->second.isWma;
	XAUDIO2_BUFFER_WMA wmaBuffer;

	// Cache entry is xWMA format
	if (isXWma) {
		wmaBuffer.PacketCount = i->second.packetCount;
		wmaBuffer.pDecodedPacketCumulativeBytes = &(i->second.decodeArr.front());		
	}

	hr = c.pVoice->SubmitSourceBuffer(&voicebuffer, isXWma ? &wmaBuffer : NULL);

	if (FAILED(hr))
		throw CXAudio2Error(hr, "Failed to submit a source buffer");
}

void CXAudio2::FreeChannel(int chan)
{
	if (chan < 0 || chan >= channels.size())
		return;

	channels[chan].Free();
}

void CXAudio2::ClearCache()
{
	// Stop reading from cache
	FreeAllChannels();

	// Reset cache
	fileCache.clear();
	resourceCache.clear();

	// Cache now has 0 size
	totalCacheSize = 0;
}

void CXAudio2::ClearUnusedCache()
{
	StringCacheIterator s = fileCache.begin();

	for ( ; s != fileCache.end(); ) {

		// Unused cache element: erase
		if (s->second.refCount == 0) {
			totalCacheSize -= s->second.size;
			s = fileCache.erase(s);
		}
		else
			s++;
	}

	IntCacheIterator i = resourceCache.begin();

	for ( ; i != resourceCache.end(); ) {

		// Unused cache element: erase
		if (i->second.refCount == 0) {
			totalCacheSize -= i->second.size;
			i = resourceCache.erase(i);
		}
		else
			i++;
	}
}

void CXAudio2::SetCacheMode(CXAudio2::CacheMode cm)
{
	cacheMode = cm;
}

CXAudio2::CacheMode CXAudio2::GetCacheMode() const
{
	return cacheMode;
}

void CXAudio2::SetCacheLimit(int bytes)
{
	cacheLimit = bytes;
}

int CXAudio2::GetCacheLimit() const
{
	return cacheLimit;
}

CXAudio2::StringCacheIterator CXAudio2::_PreCacheFile(std::string filename)
{
	// File already in cache
	StringCacheIterator i = fileCache.find(filename);

	if (i != fileCache.end())
		return i;

	// Check for OGG file
	std::string ext = filename;
	ext.erase(0, ext.length() - 4);

	// Load OGG file
	if (_stricmp(ext.c_str(), ".ogg") == 0 || _stricmp(ext.c_str(), ".oga") == 0) {
#ifdef XAUDIO2_OGGVORBIS
		FILE* f = fopen(filename.c_str(), "rb");

		if (f == NULL)
			throw CXAudio2Error(E_FAIL, std::string("Error loading file '") + filename + "'.  Check the file exists and is valid.");

		OggVorbis_File oggFile;
		ov_open(f, &oggFile, NULL, 0);	// Takes control of f; no need to call fclose()
		vorbis_info* pInfo = ov_info(&oggFile, -1);

		int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
		int bytesRead = 0;
		int bitStream;

		// Read the file data
		vector<char> oggData;
		char readbuffer[XAUDIO2_OGGVORBIS_BUFFERSIZE];

		do {
			// Read up to a buffer's worth of decoded sound data
			bytesRead = ov_read(&oggFile, readbuffer, XAUDIO2_OGGVORBIS_BUFFERSIZE, endian, 2, 1, &bitStream);

			// Append to end of buffer
		   oggData.insert(oggData.end(), readbuffer, readbuffer + bytesRead);
		}
		while (bytesRead > 0);

		// Fill out a WAVEFORMATEX for the decompressed OGG
		WAVEFORMATEX wfx;
		memset(&wfx, 0, sizeof(WAVEFORMATEX));
		wfx.cbSize = sizeof(WAVEFORMATEX);
		wfx.nChannels = pInfo->channels;
		wfx.nSamplesPerSec = pInfo->rate;
		wfx.wBitsPerSample = 16;		// OGG is always 16 bit
		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nChannels * 2;
		wfx.nBlockAlign = 2 * wfx.nChannels;
		wfx.wFormatTag = WAVE_FORMAT_PCM;

		ov_clear(&oggFile);

		// Copy the vector which is local scope and will deallocate: the cache takes responsibility for deleting
		BYTE* pBuffer = new BYTE[oggData.size()];
		memcpy(pBuffer, &(oggData.front()), oggData.size());

		// Make a cache entry of the WAV data now
		// Avoid invocation of CacheEntry copy constructor which copies pBuffer's memory
		//fileCache[filename] = CacheEntry(pwfx, pBuffer, size);
		fileCache[filename] = CacheEntry(this);
		i = fileCache.find(filename);
		CacheEntry& ce = i->second;
		ce.Set((GENERICWAVEFILE*)&wfx, pBuffer, oggData.size());
		ce.refCount = 0;	// Just precached, nothing referencing yet
		ce.isOgg = true;
		totalCacheSize += oggData.size();

		return i;
#else
		// Not built with OGG Vorbis support; throw an error
		throw CXAudio2Error(E_FAIL, "OGG Vorbis is not supported by this version of XAudio2.");
#endif
	}
	else {
		// Load WAV (PCM, ADPCM, xWMA)

		// Wave file reading class; from the SDK samples
		CWaveFile wav;

		// Dumb function definition needs an LPSTR, not an LPCSTR
		TCHAR* fileNameStr = new TCHAR[filename.size() + 1];
		memcpy(fileNameStr, filename.c_str(), filename.size() + 1);	// copy null terminator in
		hr = wav.Open(fileNameStr, 0, WAVEFILE_READ);
		delete[] fileNameStr;

		if (FAILED(hr)) throw CXAudio2Error(hr, "Failed to load file from disk.  Check the filename exists and that the file is of a supported format.");

		DWORD size = wav.GetSize();
		BYTE* pBuffer = new BYTE[size];

		// Read WAV data
		hr = wav.Read(pBuffer, size, &size);

		if (FAILED(hr)) {
			delete[] pBuffer;
			throw CXAudio2Error(hr, "Failed to read WAV data.  Check the WAV file encoding is supported.");
		}

		// Make a cache entry of the WAV data now
		// Avoid invocation of CacheEntry copy constructor which copies pBuffer's memory
		//fileCache[filename] = CacheEntry(pwfx, pBuffer, size);
		fileCache[filename] = CacheEntry(this);
		i = fileCache.find(filename);
		CacheEntry& ce = i->second;

		// xWMA
		if (wav.m_bIsXWMA) {

			ce.SetWma((GENERICWAVEFILE*)wav.GetFormat(), pBuffer, size, wav.m_nPacketCount, wav.m_aDecodedPacketCumulativeBytes);	
		}
		// PCM or ADPCM
		else {
			// Cache entry now has reference count 1 and responsibility for deleting pBuffer
			ce.Set((GENERICWAVEFILE*)wav.GetFormat(), pBuffer, size);
		}
		ce.refCount = 0;	// Just precached, nothing referencing yet
		totalCacheSize += size;

		return i;
	}
}

CXAudio2::IntCacheIterator CXAudio2::_PreCacheResource(int ID)
{
	// File already in cache
	IntCacheIterator i = resourceCache.find(ID);

	if (i != resourceCache.end())
		return i;

	// Load WAV (PCM, ADPCM, xWMA).  OGG not supported via resources.

	// Wave file reading class; from the SDK samples
	CWaveFile wav;

	hr = wav.Open(MAKEINTRESOURCE(ID), 0, WAVEFILE_READ, true);

	if (FAILED(hr)) throw CXAudio2Error(hr, "Failed to load file from disk.  Check the filename exists and that the file is of a supported format.");

	DWORD size = wav.GetSize();
	BYTE* pBuffer = new BYTE[size];

	// Read WAV data
	hr = wav.Read(pBuffer, size, &size);

	if (FAILED(hr)) {
		delete[] pBuffer;
		throw CXAudio2Error(hr, "Failed to read WAV data.  Check the WAV file encoding is supported.");
	}

	// Make a cache entry of the WAV data now
	// Avoid invocation of CacheEntry copy constructor which copies pBuffer's memory
	//fileCache[filename] = CacheEntry(pwfx, pBuffer, size);
	resourceCache[ID] = CacheEntry(this);
	i = resourceCache.find(ID);
	CacheEntry& ce = i->second;

	// xWMA
	if (wav.m_bIsXWMA) {

		ce.SetWma((GENERICWAVEFILE*)wav.GetFormat(), pBuffer, size, wav.m_nPacketCount, wav.m_aDecodedPacketCumulativeBytes);	
	}
	// PCM or ADPCM
	else {
		// Cache entry now has reference count 1 and responsibility for deleting pBuffer
		ce.Set((GENERICWAVEFILE*)wav.GetFormat(), pBuffer, size);
	}
	ce.refCount = 0;	// Just precached, nothing referencing yet
	totalCacheSize += size;

	return i;
}

void CXAudio2::PreCacheFile(std::string filename)
{
	// Call private function but don't return iterator
	_PreCacheFile(filename);
}

int CXAudio2::GetChannelCount() const
{
	return channels.size();
}

void CXAudio2::StopAllChannels()
{
	for (int i = 0; i < channels.size(); i++)
		Stop(i);
}

void CXAudio2::FreeAllChannels()
{
	ChannelIterator i = channels.begin();

	for ( ; i != channels.end(); i++)
		i->Free();
}

// Mastering channel
CXAudio2::Master::Master(CXAudio2* _pCXAudio2)
:	dbGain(0.0f),		// No gain adjustment
	pMaster(NULL),
	pVolumeMeter(NULL),
	pCXAudio2(_pCXAudio2),
	muted(false),
	volumeMeterEffectIndex(0)
{
}

void CXAudio2::Master::SetVolume(float db)
{
	dbGain = db;

	if (!muted)
		pMaster->SetVolume(XAudio2DecibelsToAmplitudeRatio(db), pCXAudio2->operationSet);
}

float CXAudio2::Master::GetVolume() const
{
	return dbGain;
}

void CXAudio2::Master::UpdateVolumeMeter(int nChannels)
{
	if (pMaster == NULL)
		return;

	// Call GetEffectParameters and pass the XAUDIO2FX_VOLUMEMETER_LEVELS struct
	rmsLevels.resize(nChannels);
	peakLevels.resize(nChannels);

	XAUDIO2FX_VOLUMEMETER_LEVELS levels;
	levels.ChannelCount = nChannels;
	levels.pPeakLevels = &(peakLevels.front());
	levels.pRMSLevels = &(rmsLevels.front());
	pMaster->GetEffectParameters(volumeMeterEffectIndex, &levels, sizeof(XAUDIO2FX_VOLUMEMETER_LEVELS));
}

void CXAudio2::Master::SetMuted(bool mute)
{
	muted = mute;

	if (muted)
		pMaster->SetVolume(0, pCXAudio2->operationSet);			// silence
	else
		pMaster->SetVolume(XAudio2DecibelsToAmplitudeRatio(dbGain), pCXAudio2->operationSet);	// restore old volume
}

bool CXAudio2::Master::IsMuted() const
{
	return muted;
}

// Channel
CXAudio2::Channel::Channel()
:	pVoice(NULL),
	reserved(false),
	number(0),
	started(false),
	pCXAudio2(NULL),
	pan_db(0.0f),
	freqRatio(1.0f),
	dbGain(0.0f),
	pCacheEntry(NULL),
	positionOffset(0),
	muted(false)
{
}

CXAudio2::Channel::Channel(CXAudio2* _pCXAudio2, int num)
:	pVoice(NULL),
	reserved(false),
	number(num),
	started(false),
	pCXAudio2(_pCXAudio2),
	pan_db(0.0f),
	freqRatio(1.0f),
	dbGain(0.0f),
	pCacheEntry(NULL),
	positionOffset(0),
	muted(false)
{
}

void CXAudio2::Channel::Free()
{
	if (pVoice != NULL) {

		// Destroyed source voice will no longer reference buffer
		if (pCacheEntry != NULL) {
			pCacheEntry->refCount--;

			if (pCXAudio2 != NULL)
				pCXAudio2->CheckToFreeCacheEntry(pCacheEntry);

			pCacheEntry = NULL;
		}
		pVoice->Stop(0);
		pVoice->DestroyVoice();
		pVoice = NULL;
	}

	// Restore defaults (next voice will adopt these)
	started = false;
	dbGain = 0.0f;
	pan_db = 0.0f;
	freqRatio = 1.0f;
}

void CXAudio2::Channel::SetReserved(bool isReserved)
{
	reserved = isReserved;
}

bool CXAudio2::Channel::IsReserved() const
{
	return reserved;
}

bool CXAudio2::Channel::IsPlaying() const
{
	return started && !HasFinishedReadingBuffer();
}

bool CXAudio2::Channel::HasFinishedReadingBuffer() const
{
	if (pVoice == NULL) return false;

	XAUDIO2_VOICE_STATE state;
	pVoice->GetState(&state);
	
	// Voice is finished when the buffer is no longer queued
	return state.BuffersQueued == 0;
}

void CXAudio2::Channel::Play()
{
	if (pVoice == NULL || pCXAudio2 == NULL)
		return;

	if (pCXAudio2->timeScale != 1.0f)
		SetFrequencyRatio(GetFrequencyRatio());

	pVoice->Start(0, pCXAudio2->operationSet);

	started = true;	// Voice has been started
}

void CXAudio2::Channel::Stop()
{
	if (pVoice == NULL || pCXAudio2 == NULL)
		return;

	pVoice->Stop(0, pCXAudio2->operationSet);

	started = false;
}

void CXAudio2::Channel::SetMuted(bool _mute)
{
	if (pVoice == NULL || pCXAudio2 == NULL)
		return;

	muted = _mute;

	if (muted)
		pVoice->SetVolume(0, pCXAudio2->operationSet);		// silence
	else
		pVoice->SetVolume(XAudio2DecibelsToAmplitudeRatio(dbGain), pCXAudio2->operationSet);	// restore old volume
}

bool CXAudio2::Channel::GetMuted() const
{
	return muted;
}

void CXAudio2::Channel::SetVolume(float db)
{
	if (pVoice == NULL || pCXAudio2 == NULL)
		return;

	dbGain = db;

	if (!muted)
		pVoice->SetVolume(XAudio2DecibelsToAmplitudeRatio(dbGain), pCXAudio2->operationSet);
}

float CXAudio2::Channel::GetVolume() const
{
	return dbGain;
}

void CXAudio2::Channel::SetPan(float _pan)
{
	if (pVoice == NULL || pCXAudio2 == NULL)
		return;

	// Fail on setting mono sounds (XAudio2 crashes)
	if (pCacheEntry && pCacheEntry->gwf.pcm.nChannels == 1)
		return;

	if (_pan <= PAN_HARDLEFT) {
		pan_db = PAN_HARDLEFT;
		float channelVolumes[2] = {1.0f, 0.0f};
		pVoice->SetChannelVolumes(2, channelVolumes, pCXAudio2->operationSet);
	}
	else if (_pan >= PAN_HARDRIGHT) {
		pan_db = PAN_HARDRIGHT;
		float channelVolumes[2] = {0.0f, 1.0f};
		pVoice->SetChannelVolumes(2, channelVolumes, pCXAudio2->operationSet);
	}
	else {
		pan_db = _pan;
		float ratio = XAudio2DecibelsToAmplitudeRatio(_pan > 0 ? -_pan : _pan);

		float channelVolumes[2] = {1.0f, 1.0f};
		
		// Left
		if (_pan <= 0)
			channelVolumes[1] = ratio;	// Attenuate right (note 'ratio' negative)
		else
			channelVolumes[0] = ratio;	// Attenuate left

		pVoice->SetChannelVolumes(2, channelVolumes, pCXAudio2->operationSet);
	}
}

float CXAudio2::Channel::GetPan() const
{
	return pan_db;
}

void CXAudio2::Channel::SetFrequencyRatio(float _freqRatio)
{
	if (pVoice == NULL || pCXAudio2 == NULL)
		return;

	freqRatio = _freqRatio;

	pVoice->SetFrequencyRatio(freqRatio * pCXAudio2->timeScale, pCXAudio2->operationSet);
}

float CXAudio2::Channel::GetFrequencyRatio() const
{
	return freqRatio;
}

void CXAudio2::Channel::SetPosition(float pos)
{
	if (pVoice == NULL || pCacheEntry == NULL)
		return;

	if (pos < 0.0f || pos >= pCacheEntry->length)
		return;

	bool wasStarted = started;

	// For GetPosition() to return a custom position, the sample offset must negate the already processed samples
	XAUDIO2_VOICE_STATE state;
	pVoice->GetState(&state);
	positionOffset = 0 - state.SamplesPlayed;

	// Resubmit buffer with new play location
	pVoice->Stop(0);
	pVoice->FlushSourceBuffers();

	// Calculate the start position in samples
	float samplesPerSec = pCacheEntry->gwf.pcm.nSamplesPerSec;
	float startSamplef = pos * samplesPerSec;
	positionOffset += startSamplef;		// For GetPosition()

	UINT32 startSample = startSamplef;

	// ADPCM format: position has to be a multiple of wSamplesPerBlock
	if (pCacheEntry->gwf.adpcm.wfx.wFormatTag == WAVE_FORMAT_ADPCM) {
		// Integer rounding
		startSample /= pCacheEntry->gwf.adpcm.wSamplesPerBlock;
		startSample *= pCacheEntry->gwf.adpcm.wSamplesPerBlock;
	}

	XAUDIO2_BUFFER voicebuffer;
	voicebuffer.Flags = XAUDIO2_END_OF_STREAM;	// no more data to follow
	voicebuffer.AudioBytes = pCacheEntry->size;
	voicebuffer.pAudioData = pCacheEntry->pBuffer;
	voicebuffer.pContext = pCacheEntry;
	voicebuffer.PlayBegin = startSample;
	voicebuffer.PlayLength = 0;		// play entire buffer
	voicebuffer.LoopBegin = XAUDIO2_NO_LOOP_REGION;
	voicebuffer.LoopLength = 0;
	voicebuffer.LoopCount = 0;

	bool isXWma = pCacheEntry->isWma;
	XAUDIO2_BUFFER_WMA wmaBuffer;

	// Cache entry is xWMA format
	if (isXWma) {
		wmaBuffer.PacketCount = pCacheEntry->packetCount;
		wmaBuffer.pDecodedPacketCumulativeBytes = &(pCacheEntry->decodeArr.front());		
	}

	HRESULT hr = pVoice->SubmitSourceBuffer(&voicebuffer, isXWma ? &wmaBuffer : NULL);

	if (wasStarted)
		Play();
}

float CXAudio2::Channel::GetPosition() const
{
	if (pVoice == NULL || pCacheEntry == NULL)
		return 0.0f;

	XAUDIO2_VOICE_STATE state;
	pVoice->GetState(&state);

	double totalSamplesProcessed = positionOffset + state.SamplesPlayed;
	double samplesPerSec = pCacheEntry->gwf.pcm.nSamplesPerSec;

	return totalSamplesProcessed / samplesPerSec;
}

float CXAudio2::Channel::GetLength() const
{
	if (pVoice == NULL || pCacheEntry == NULL)
		return 0.0f;

	return pCacheEntry->length;
}

void CXAudio2::Channel::ExitLoop() const
{
	if (pVoice == NULL || pCXAudio2 == NULL)
		return;

	pVoice->ExitLoop(pCXAudio2->operationSet);
}

CXAudio2::Channel& CXAudio2::GetChannel(int chan)
{
	if (chan < 0 || chan >= channels.size())
		throw CXAudio2Error(E_INVALIDARG, "Channel number out of bounds");

	return channels[chan];
}

void CXAudio2::Play(int chan)
{
	if (chan < 0 || chan >= channels.size())
		return;

	channels[chan].Play();
}

bool CXAudio2::IsPlaying(int chan) const
{
	if (chan < 0 || chan >= channels.size())
		return false;

	return channels[chan].IsPlaying();
}

void CXAudio2::Stop(int chan)
{
	if (chan < 0 || chan >= channels.size())
		return;

	channels[chan].Stop();
}

void CXAudio2::SetChannelReserved(int chan, bool isReserved)
{
	if (chan < 0 || chan >= channels.size())
		return;

	channels[chan].SetReserved(isReserved);
}

bool CXAudio2::IsChannelReserved(int chan) const
{
	if (chan < 0 || chan >= channels.size())
		return false;

	return channels[chan].IsReserved();
}

void CXAudio2::SetAllChannelsReserved(bool isReserved)
{
	ChannelIterator i = channels.begin();

	for ( ; i != channels.end(); i++)
		i->SetReserved(isReserved);
}

void CXAudio2::SetMuted(int chan, bool _mute)
{
	if (chan < 0 || chan >= channels.size())
		return;

	channels[chan].SetMuted(_mute);
}

bool CXAudio2::GetMuted(int chan) const
{
	if (chan < 0 || chan >= channels.size())
		return false;

	return channels[chan].GetMuted();
}

void CXAudio2::SetVolume(int chan, float db)
{
	if (chan < 0 || chan >= channels.size())
		return;

	channels[chan].SetVolume(db);
}

float CXAudio2::GetVolume(int chan) const
{
	if (chan < 0 || chan >= channels.size())
		return 0.0f;

	return channels[chan].GetVolume();
}

void CXAudio2::SetPan(int chan, float pan)
{
	if (chan < 0 || chan >= channels.size())
		return;

	channels[chan].SetPan(pan);
}

float CXAudio2::GetPan(int chan) const
{
	if (chan < 0 || chan >= channels.size())
		return 0.0f;

	return channels[chan].GetPan();
}

void CXAudio2::SetFrequencyRatio(int chan, float freqRatio)
{
	if (chan < 0 || chan >= channels.size())
		return;

	channels[chan].SetFrequencyRatio(freqRatio);
}

float CXAudio2::GetFrequencyRatio(int chan) const
{
	if (chan < 0 || chan >= channels.size())
		return 1.0f;

	return channels[chan].GetFrequencyRatio();
}

void CXAudio2::SetPosition(int chan, float pos)
{
	if (chan < 0 || chan >= channels.size())
		return;

	channels[chan].SetPosition(pos);
}

float CXAudio2::GetPosition(int chan) const
{
	if (chan < 0 || chan >= channels.size())
		return 0.0f;

	return channels[chan].GetPosition();
}

float CXAudio2::GetLength(int chan) const
{
	if (chan < 0 || chan >= channels.size())
		return 0.0f;

	return channels[chan].GetLength();
}

void CXAudio2::ExitLoop(int chan) const
{
	if (chan < 0 || chan >= channels.size())
		return;

	channels[chan].ExitLoop();
}

int CXAudio2::GetUnusedChannel()
{
	// First try to find a non-playing unreserved channel
	ChannelIterator i = channels.begin();

	for ( ; i != channels.end(); i++) {
		if (!i->IsReserved() && !i->IsPlaying()) {
			nextChannel = (i->number + 1) % numChannels;	// Next channel to overwrite if all are playing

			return i->number;
		}
	}

	// All channels are either reserved or playing: first try to find an unreserved channel to play on
	int checked = 0;

	while (IsChannelReserved(nextChannel) && checked < numChannels) {
		nextChannel = (nextChannel + 1) % numChannels;
		checked++;
	}

	// An unreserved channel was found.
	if (checked < numChannels)
		return nextChannel;

	// Every channel was checked and they were all reserved.  Return -1.
	return -1;
}

// Cache entry
CXAudio2::CacheEntry::CacheEntry()
:	pBuffer(NULL),
	size(0),
	refCount(0),
	isWma(false),
	pCXAudio2(NULL),
	isOgg(false),
	length(0.0f)
{
}

CXAudio2::CacheEntry::CacheEntry(CXAudio2* _pCXAudio2)
:	pBuffer(NULL),
	size(0),
	refCount(0),
	isWma(false),
	pCXAudio2(_pCXAudio2),
	isOgg(false),
	length(0.0f)
{
}

// Copy ctor moves responsibility for deleting pBuffer from ce to this (std::map requires)
CXAudio2::CacheEntry::CacheEntry(const CXAudio2::CacheEntry& ce)
:	gwf(ce.gwf),
	size(ce.size),
	refCount(0),
	isWma(false),
	pCXAudio2(NULL),
	isOgg(false),
	length(0.0f)
{
	pBuffer = new BYTE[size];
	memcpy(pBuffer, ce.pBuffer, size);
}

// Assume responsibility for deletion of _pBuffer
void CXAudio2::CacheEntry::Set(const GENERICWAVEFILE* pgwf, BYTE* _pBuffer, DWORD _size)
{
	gwf = *pgwf;
	pBuffer = _pBuffer;
	size = _size;

	// XAudio2 isn't clever enough to pan mono sounds.  Make mono wavs stereo so we can pan them.
	if (gwf.pcm.wFormatTag == WAVE_FORMAT_PCM && gwf.pcm.nChannels == 1) {

		BYTE* stereoBuffer = new BYTE[size * 2];	// exactly double the size

		// Loop mono buffer and duplicate every sample
		int sampleBytes = gwf.pcm.wBitsPerSample / 8;

		BYTE* src = pBuffer;
		BYTE* src_end = pBuffer + size;
		BYTE* dest = stereoBuffer;

		for ( ; src != src_end; src += sampleBytes) {
			memcpy(dest, src, sampleBytes);
			dest += sampleBytes;
			memcpy(dest, src, sampleBytes);
			dest += sampleBytes;
		}

		// Free old buffer and use stereo buffer
		delete[] pBuffer;
		pBuffer = stereoBuffer;

		// Update the wfx to use stereo
		gwf.pcm.nChannels = 2;
		gwf.pcm.nBlockAlign = 2 * sampleBytes;
		gwf.pcm.nAvgBytesPerSec = gwf.pcm.nSamplesPerSec * gwf.pcm.nBlockAlign;

		size *= 2;	// Size doubled
	}

	length = (float)size / (float)gwf.pcm.nAvgBytesPerSec;
}

// Assume responsibility for deletion of _pBuffer
void CXAudio2::CacheEntry::SetWma(const GENERICWAVEFILE* pgwf, BYTE* _pBuffer, DWORD _size, UINT32 _packetCount, UINT32* pDecodeArr)
{
	Set(pgwf, _pBuffer, _size);

	// WMA packet data to be stored
	isWma = true;
	packetCount = _packetCount;
	decodeArr.reserve(packetCount);

	// Copy decode array
	for (int i = 0; i < packetCount; i++)
		decodeArr.push_back(pDecodeArr[i]);

	length = (float)size / (float)gwf.pcm.nAvgBytesPerSec;
}

CXAudio2::CacheEntry::~CacheEntry()
{
	// Release memory
	if (pBuffer)
		delete[] pBuffer;
}

void CXAudio2::CheckToFreeCacheEntry(CXAudio2::CacheEntry* pce)
{
	// Buffer no longer referenced
	if (pce->refCount == 0) {

		bool remove = false;

		// See what should be done
		switch (cacheMode) {

		// Cache off: remove all unreferenced buffers
		case CXAudio2::CACHE_OFF:
			remove = true;
			break;

		// Cache limited: remove if above the cache limit
		case CXAudio2::CACHE_LIMIT:
			if (pce->size >= cacheLimit)
				remove = true;
			break;

		// Don't cache OGG files: remove
		case CXAudio2::CACHE_NOTOGG:
			if (pce->isOgg)
				remove = true;
			break;

		// If caching all leave in memory
		case CXAudio2::CACHE_ALL:
			break;

		}

		// Free the buffer
		if (remove) {

			// Search the file cache and remove by matching address
			StringCacheIterator i = fileCache.begin();
			StringCacheIterator end = fileCache.end();

			for ( ; i != end; i++) {
				if (&(i->second) == pce) {
					fileCache.erase(i);
					return;		// done
				}
			}

			// Search the resource cache
			IntCacheIterator j = resourceCache.begin();
			IntCacheIterator iend = resourceCache.end();

			for ( ; j != iend; j++) {
				if (&(j->second) == pce) {
					resourceCache.erase(j);
					return;		// done
				}
			}
		}
	}
}