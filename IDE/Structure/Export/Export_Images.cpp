// File includes
#include "StdAfx.h"
#include "Export.h"

void CExport::CreateCollisionMask(CxImage& image, BYTE*& bits, int& pitch)
{
	// Determine size of our buffer.  All buffers are rounded up to 128 bit pitch, just in case SSE can be used.
	const int align_pitch_bits = 64;
	const int align_pitch_bytes = align_pitch_bits / 8;

	pitch = image.GetWidth() / 8;
	if (image.GetWidth() % 8 != 0)
		pitch++;		// 11 pixel width needs 2 bytes not 1 rounded

	// Eg. a 20 byte pitch must round up to 32 (+12, 16 - 4)
	if (pitch % align_pitch_bytes != 0)
		pitch += align_pitch_bytes - (pitch % align_pitch_bytes);

	// If the pitch does not leave at least a 64 pixel gutter, increase it by 64 pixels.
	// This prevents false positives when a 64 pixel check from the far right edge can wrap around to the next line.
	if ((pitch * 8) - image.GetWidth() < align_pitch_bits)
		pitch += align_pitch_bytes;

	// Allocate and zero the memory
	bits = new BYTE[image.GetHeight() * pitch];
	memset(bits, 0, image.GetHeight() * pitch);

/*	// Set as per a DWORD pointer to prevent endian corruption

	DWORD* ptr = (DWORD*)bits;
	int ipitch = pitch / sizeof(DWORD);

	// Loop each pixel and set the bit in the bitmask
	for (int x = 0; x < image.GetWidth(); x++) {
		for (int y = 0; y < image.GetHeight(); y++) {

			// Set the bit (check alpha component)
			int bit = image.GetPixelColor(x, y).rgbReserved > 0 ? 1 : 0;

			// Invert Y (CxImage stupidness)
			ptr[(image.GetHeight() - y - 1) * ipitch + (x/32)] |= bit << (31 - (x % 32));
		}
	}
	*/ 
	// sorry tigs trying byte method
	BYTE* ptr = (BYTE*)bits;
	int ipitch = pitch / sizeof(BYTE);
	

	// Loop each pixel and set the bit in the bitmask
	for (int x = 0; x < image.GetWidth(); x++) {
		for (int y = 0; y < image.GetHeight(); y++) {

			// What is this for??? Commented out 22/11/08 since it has no effect anyway -AG
			//if(x == 0)
			//	ptr[(image.GetHeight() - y - 1) * ipitch] = 0; //first byte must be 0

			// Set the bit (check alpha component)
			int bit = image.GetPixelColor(x, y).rgbReserved > 0 ? 1 : 0;

			// Invert Y (CxImage stupidness)
			ptr[(image.GetHeight() - y - 1) * ipitch + (x/8)] |= bit << (7 - (x % 8));
		}
	}

}

static CApplication* pLastAppCachedImageBlock = NULL;

bool fileexists(const char * filename)
{
	return GetFileAttributes(filename) != 0xFFFFFFFF;
}

void CExport::GenerateImages()
{
	// 25-80% progress
	BYTE* tempBuffer = NULL;
	long theSize;
	int index = 0;

	CString oldAppPath = main_frame->m_INI.GetString("General", "ImageCacheApp");
	CString newAppPath = application->file_information.file_path;

	CPath path;
	path.SetToCurrentDirectory();
	CString cachePath = path.GetFullPath();
	cachePath += "Data\\imagecache.bin";

	// Check if there is a cached image block we can use
	if (application == pLastAppCachedImageBlock && !application->resources.images_changed && fileexists(cachePath) && oldAppPath == newAppPath) {
		imageBlock.load(cachePath);
		return;
	}

	// Write the app path to the INI so the cache doesnt get used with the wrong application
	main_frame->m_INI.WriteString("General", "ImageCacheApp", newAppPath);

	// Write image count
	imageBlock<<(int)application->resources.images.size();

	// Loop and add resources.images
	for (list<CImageResource>::iterator i = application->resources.images.begin(); i != application->resources.images.end(); i++, index++)
	{
		CxImage& im = i->bitmap;
		
		if (application->runtime == CApplication::rt_application)
			im.Encode(tempBuffer, theSize, CXIMAGE_FORMAT_BMP);
		else
			im.Encode(tempBuffer, theSize, CXIMAGE_FORMAT_PNG);

		// Write image ID
		imageBlock<<(int)i->m_FixedID;

		// Write image hotspot
		imageBlock << i->m_Hotspot.x;
		imageBlock << i->m_Hotspot.y;

		// Write image action points
		int apointsize = i->m_Action.size();
		imageBlock << apointsize;
		for(map<CString, CPoint>::iterator it = i->m_Action.begin(); it!= i->m_Action.end(); it++)
		{
			imageBlock << it->second.x << it->second.y << it->first;
		}

		// Write image data size
		imageBlock<<(int)theSize;

		// Write image data code
		imageBlock.append(tempBuffer, theSize);	
		
		// Free memory using CxImage (for malloc/new compatibility)
		im.FreeMemory(tempBuffer);

		// Generate a collision mask
		BYTE* bits;
		int pitch;
		CxImage* pCol = &im;
		if(i->m_Collision.IsValid())
		{
			pCol = & i->m_Collision;
		}

		CreateCollisionMask(*pCol, bits, pitch);

		imageBlock<< pCol->GetWidth();
		imageBlock<< pCol->GetHeight();
		imageBlock<< pitch;
		imageBlock.append(bits, pitch * pCol->GetHeight());

		// CreateCollisionMask allocated bits so free it
		delete[] bits;

		// CxImage requires the tempBuffer parameter be NULL (for some reason)
		tempBuffer = NULL;

		// Progress 25-80% for this function
		ProgressDlg.SetProgress(25 + (index * 55) / application->resources.images.size());
	}

	// Dump the image block to file
	imageBlock.save(cachePath);

	pLastAppCachedImageBlock = application;
	application->resources.images_changed = false;
}