/*	CXAudio2 wrapper class for 2D game audio
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

#pragma once

#define XAUDIO2_HELPER_FUNCTIONS				// XAudio2DecibelsToAmplitudeRatio
#define XAUDIO2_OGGVORBIS_BUFFERSIZE	65536	// 64kb buffer reads

#include <vector>
#include <XAudio2.h>
#include <XAudio2fx.h>
#include <XAPOFX.h>

#ifdef XAUDIO2_OGGVORBIS
#include <vorbis\vorbisfile.h>
#endif

// Error class
class CXAudio2Error {
public:
	CXAudio2Error(HRESULT _hr, std::string _name);
	const char* GetName() const;
	HRESULT GetHR() const;
private:
	HRESULT hr;
	std::string name;
};

// CXAudio2 instance class
class CXAudio2 {
public:

	// ctor, dtor
	CXAudio2();
	~CXAudio2();

	// For CXAudio2SetupParameters
	enum CommandBatchType {
		CBT_IMMEDIATE,		// All audio commands (play, stop, set volume...) take effect immediately
		CBT_NEXTTICK		// All audio commands are batched and happen together in the next Tick() call
	};

	// For determining which audio buffers to keep in memory
	enum CacheMode {
		CACHE_OFF,		// Free sound buffers ASAP to reduce memory overhead.  Subsequent LoadFile calls slower; read from disk.
		CACHE_LIMIT,	// Free sound buffers which are over the cache limit (default 3mb) ASAP.  Use SetCacheLimit() to adjust
		CACHE_NOTOGG,	// Cache everything except decompressed OGG buffers.
		CACHE_ALL		// Leave everything in the cache.
	};

	// Setup parameters to pass to CXAudio2::Init()
	// Typedeffed globally as CXAudio2SetupParameters
	class SetupParameters {
	public:
		SetupParameters();

		int numChannels;						// Max simultaneous sonds
		float maxFrequencyRatio;				// XAudio2's maximum frequency ratio
		float masterGain;						// Master volume, in dB
		CommandBatchType commandBatching;		// Command batching mode
		CacheMode cacheMode;					// Buffer caching mode
		int cacheLimit;							// If cacheMode is CACHE_LIMIT

		// Mastering limiter
		bool limiter_enable;
		int limiter_release;					// 1 - 20, default 6 (presumably milliseconds, not specified by doc)
		int limiter_threshold;					// 1 - 1800, default 1000.  Units appear totally arbitrary, not specified by doc
	};

	// Initialisation
	void Init(const SetupParameters& params);

	// Shut down (called in dtor)
	void Close();

	// Pan definitions
	enum PanFlag {
		PAN_HARDLEFT = -1000,
		PAN_HARDRIGHT = 1000
	};

	// Master channel (only one instance; 'master')
	class Master {
	public:
		void SetVolume(float db);
		float GetVolume() const;
		void SetMuted(bool mute);
		bool IsMuted() const;

		friend class CXAudio2;

	private:
		// No public construction
		Master(CXAudio2* _pCXAudio2);

		IXAudio2MasteringVoice* pMaster;
		IUnknown* pVolumeMeter;
		float dbGain;						// Gain, decibels

		// Volume metering
		void UpdateVolumeMeter(int nChannels);
		typedef std::vector<float> LevelsList;
		LevelsList peakLevels;
		LevelsList rmsLevels;
		int volumeMeterEffectIndex;
		bool muted;

		CXAudio2* pCXAudio2;
	} master;

	class CacheEntry;	// Forwards decl

	// Class for one channel; CXAudio2 creates 'numChannels' of these
	class Channel {
	public:
		void Free();

		// Prevent GetUnusedChannel() returning this channel
		void SetReserved(bool isReserved);
		bool IsReserved() const;
		bool IsPlaying() const;

		// Playback control
		void Play();
		void Stop();
		void SetMuted(bool _mute);
		bool GetMuted() const;
		void SetVolume(float db);
		float GetVolume() const;
		void SetPan(float _pan);
		float GetPan() const;
		void SetFrequencyRatio(float _freqRatio);
		float GetFrequencyRatio() const;

		void SetPosition(float pos);	// Set position, in seconds
		float GetPosition() const;		// Get position, in seconds
		float GetLength() const;		// Get sample length, in seconds
		void ExitLoop() const;

		friend class CXAudio2;
	private:
		// No public construction
		Channel();
		Channel(CXAudio2* _pCXAudio2, int num);

		bool HasFinishedReadingBuffer() const;

		IXAudio2SourceVoice* pVoice;
		bool reserved;
		int number;		// Channel number (1-numChannels)
		bool started;	// True if the channel has been started.  Used to determine if a buffer is really playing

		bool muted;
		float dbGain;
		float pan_db;
		float freqRatio;

		INT64 positionOffset;		// Start position, in samples

		// Pointer to parent class
		CXAudio2* pCXAudio2;

		// Pointer to cache holding this channel's source buffer
		CacheEntry* pCacheEntry;
	};

	typedef std::vector<CXAudio2::Channel>				ChannelList;
	typedef std::vector<CXAudio2::Channel>::iterator	ChannelIterator;

	class CacheEntry {
	public:
		union GENERICWAVEFILE {
			WAVEFORMATEX pcm;
			// The default ADPCM struct defines aCoef[], so using that here won't give us enough actual memory
			// for the full details of the structure.  This custom definition makes sure aCoef has memory allocated.
			struct ADPCM_ACOEF32 {
				WAVEFORMATEX wfx;
				WORD wSamplesPerBlock;
				WORD wNumCoef;
				ADPCMCOEFSET aCoef[32];  // Always 7 coefficient pairs for MS ADPCM, use 32 to be on the safe side
			} adpcm;
			WAVEFORMATEXTENSIBLE xwma;
		};
		CacheEntry();
		CacheEntry(CXAudio2* _pCXAudio2);
		CacheEntry(const CacheEntry& ce);
		void Set(const GENERICWAVEFILE* pgwf, BYTE* _pBuffer, DWORD _size);
		void SetWma(const GENERICWAVEFILE* pgwf, BYTE* _pBuffer, DWORD _size, UINT32 _packetCount, UINT32* pDecodeArr);
		~CacheEntry();
		friend class CXAudio2;
		friend class VoiceCallback;

		GENERICWAVEFILE gwf;
		BYTE* pBuffer;
		DWORD size;
		CXAudio2* pCXAudio2;
		float length;		// Length in seconds of buffer

		// WMA
		bool isWma;
		UINT32 packetCount;
		std::vector<UINT32> decodeArr;

		// OGG
		bool isOgg;

		// Reference count for ClearUnusedCache()
		int refCount;
	};

	typedef std::map<std::string, CacheEntry>			StringCache;
	typedef std::map<std::string, CacheEntry>::iterator	StringCacheIterator;
	typedef std::map<int, CacheEntry>					IntCache;
	typedef std::map<int, CacheEntry>::iterator			IntCacheIterator;
	typedef CacheEntry::GENERICWAVEFILE GENERICWAVEFILE;

	// Caching functions
	void SetCacheMode(CacheMode cm);
	CacheMode GetCacheMode() const;

	// Set the limit for CACHE_LIMIT mode.  Buffers above this size will be freed as soon as they are no longer referenced.
	void SetCacheLimit(int bytes);
	int GetCacheLimit() const;

	void ClearCache();			// Frees all memory and stops & frees every channel
	void ClearUnusedCache();	// Frees only cache entries not currently being played by anything
	void PreCacheFile(std::string filename);	// Loads a file in to the cache so the next LoadFile() call does not access disk

	// Command batching (see CommandBatchType definition)
	void SetCommandBatching(CommandBatchType _cbt);
	CommandBatchType GetCommandBatching() const;

	// Update the audio engine
	void Tick(float _timeScale = 1.0f);

	// Get audio statistics
	float GetPeakLevel(int chan) const;
	float GetRMSLevel(int chan) const;
	const XAUDIO2_PERFORMANCE_DATA& GetPerformanceData() const;
	const XAUDIO2_DEVICE_DETAILS& GetDeviceDetails() const;
	int GetTotalCacheSize() const;
	int GetChannelCount() const;

	// Audio functions
	void FreeChannel(int chan);
	void LoadFile(std::string filename, int chan, bool loop = false);
	void LoadResource(int ID, int chan, bool loop = false);
	void Play(int chan);
	void Stop(int chan);
	bool IsPlaying(int chan) const;
	void StopAllChannels();
	void FreeAllChannels();
	void SetMuted(int chan, bool mute);
	bool GetMuted(int chan) const;
	void SetVolume(int chan, float db);
	float GetVolume(int chan) const;
	void SetPan(int chan, float pan);
	float GetPan(int chan) const;
	void SetFrequencyRatio(int chan, float freqRatio);
	float GetFrequencyRatio(int chan) const;
	void SetPosition(int chan, float pos);
	float GetPosition(int chan) const;
	float GetLength(int chan) const;
	void ExitLoop(int chan) const;

	// Get a silent, unreserved channel to play on.  Useful for miscellaneous sound effects.
	// May return a playing channel if all channels are playing; if all channels are reserved, will
	// return -1.
	int  GetUnusedChannel();

	// Retrieve a reference to a channel.  WARNING:  Throws a CXAudio2Error if 'chan' is out of bounds.
	// If you use this, either be prepared to handle the exception, use your own bounds checking, or use
	// the equivalent CXAudio2 functions like Play(int chan) or Stop(int chan) - these just call the appropriate
	// function's channel, but return and do nothing if the channel is out of bounds.
	Channel& GetChannel(int chan);

	// Setting a channel's 'reserved' flag on means GetUnusedChannel() will not pick it.
	// For example if you want to autoplay sounds and you don't want an ambient loop to be
	// overwritten with a sound effect, reserve the channel playing the loop.
	void SetChannelReserved(int chan, bool isReserved = true);
	bool IsChannelReserved(int chan) const;
	void SetAllChannelsReserved(bool isReserved);

private:

	// Settings
	int numChannels;
	float maxFrequencyRatio;
	CommandBatchType cbt;

	int operationSet;	// For batching XAudio2 calls

	// Channel strip
	ChannelList		channels;

	// Cache
	StringCache		fileCache;
	IntCache		resourceCache;
	CacheMode		cacheMode;
	int				cacheLimit;	// bytes

	StringCacheIterator _PreCacheFile(std::string filename);	// Private version returns iterator
	IntCacheIterator _PreCacheResource(int ID);					// Private version returns iterator
	void CheckToFreeCacheEntry(CacheEntry* pce);

	// Performance & device data
	XAUDIO2_PERFORMANCE_DATA performanceData;
	XAUDIO2_DEVICE_DETAILS deviceDetails;
	int totalCacheSize;

	// Timescaling
	float timeScale;

	// Interfaces
	IXAudio2* pXAudio2;
	HRESULT hr;

	// For GetUnusedChannel when all are playing
	int nextChannel;
};

typedef CXAudio2::SetupParameters CXAudio2SetupParameters;