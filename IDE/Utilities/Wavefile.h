//-----------------------------------------------------------------------------
// File: WaveFile.h
//
// Copyright (c) Microsoft Corp. All rights reserved.

// Thanks to Leonardo Barbero for CWaveFile implementation that supports reading xWMA WAVs.
// http://forums.xna.com/forums/t/17384.aspx
// https://profile.xna.com/profile.aspx?crn=Leonardo+Barbero
// Other modifications by Ashley Gullen.

//-----------------------------------------------------------------------------
#ifndef DXUTWAVEFILE_H
#define DXUTWAVEFILE_H

#include "CXAudio2.h"

typedef CXAudio2::CacheEntry::GENERICWAVEFILE GENERICWAVEFILE;

//-----------------------------------------------------------------------------
// Typing macros 
//-----------------------------------------------------------------------------
#define WAVEFILE_READ   1
#define WAVEFILE_WRITE  2

//-----------------------------------------------------------------------------
// Name: class CWaveFile
// Desc: Encapsulates reading or writing sound data to or from a wave file
//-----------------------------------------------------------------------------
class CWaveFile  
{  
public:  
    WAVEFORMATEX* m_pwfx;        // Pointer to WAVEFORMATEX structure  
    HMMIO m_hmmio;       // MM I/O handle for the WAVE  
    MMCKINFO m_ck;          // Multimedia RIFF chunk  
    MMCKINFO m_ckRiff;      // Use in opening a WAVE file  
    DWORD m_dwSize;      // The size of the wave file  
    MMIOINFO m_mmioinfoOut;  
    DWORD m_dwFlags;  
    BOOL m_bIsReadingFromMemory;  
    BYTE* m_pbData;  
    BYTE* m_pbDataCur;  
    ULONG m_ulDataSize;  
    CHAR* m_pResourceBuffer;  
    bool m_bIsXWMA;  
    UINT32 m_nPacketCount;  
    UINT32* m_aDecodedPacketCumulativeBytes;  
 
protected:  
    HRESULT ReadMMIO();  
    HRESULT WriteMMIO( WAVEFORMATEX* pwfxDest );  
 
public:  
    CWaveFile();  
    ~CWaveFile();  
 
    HRESULT Open( LPSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags, bool asResource = false);  
    HRESULT OpenFromMemory( BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags );  
    HRESULT Close();  
 
    HRESULT Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead );  
    HRESULT Write( UINT nSizeToWrite, BYTE* pbData, UINT* pnSizeWrote );  
 
    DWORD   GetSize();  
    HRESULT ResetFile();  
    WAVEFORMATEX* GetFormat()  
    {  
        return m_pwfx;  
    };  
}; 


#endif // DXUTWAVEFILE_H
