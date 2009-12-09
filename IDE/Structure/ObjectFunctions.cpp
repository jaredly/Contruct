// ObjectFunctions.cpp
// Useful common functions

#include "StdAfx.h"
#include "ObjectFunctions.h"

extern CPluginMap g_Plugins;

/////////////////////////////////
// Miscellaneous functions
/////////////////////////////////

// Get plugin information by ID
const CPlugin& GetPlugin(int ID)
{
	CPluginIterator i = g_Plugins.find(ID);

	if (i != g_Plugins.end())
		return i->second;

	CString msg;
	msg.Format("An invalid plugin was referenced (#%d)", ID);
	throw runtime_error((const char*)msg);
}

// Get plugin information by file name
const CPlugin& GetPlugin(LPCSTR Name)
{
	map<int, CPlugin>::iterator i = g_Plugins.begin();

	for ( ; i != g_Plugins.end(); i++) 
	{
		if (strcmp(i->second.m_Name, Name) == 0)
			return i->second;
	}

	throw runtime_error((const char*)(CString("An invalid plugin was referenced (") + Name + ")"));
}

// Get plugin information by file path
const CPlugin& GetPluginByFile(LPCSTR Name)
{
	map<int, CPlugin>::iterator i = g_Plugins.begin();

	for ( ; i != g_Plugins.end(); i++) 
	{
		if (strcmp(i->second.m_FileName, Name) == 0)
			return i->second;
	}

	throw runtime_error("GetPlugin error");
}

// Get an OINFO by name
OINFO* GetOINFO(CString Str)
{
	OINFO* returnInfo;
	POSITION pos = g_dlls->GetStartPosition();
	DWORD nKey;

	// Find the DLL
	while (pos != NULL) 
	{
		g_dlls->GetNextAssoc(pos, nKey, returnInfo);
		CString CStr = returnInfo->extName;

		Str.MakeLower();
		CStr.MakeLower();
	
		// Got it
		if (CStr == Str)
			return returnInfo;
	}

	return NULL;
}

// Get an OINFO by filename
OINFO* GetOINFOByFile(CString Str)
{
	OINFO* returnInfo;
	POSITION pos = g_dlls->GetStartPosition();
	DWORD nKey;

	// Find the DLL
	while (pos != NULL) 
	{
		g_dlls->GetNextAssoc(pos, nKey, returnInfo);
		CString CStr = returnInfo->extFileName;

		// Got it
		if (CStr == Str)
			return returnInfo;
	}

	return NULL;
}

// Get an OINFO for a specific ID
OINFO* GetOINFO(int objectID)
{
	OINFO* returnInfo;
	POSITION pos = g_dlls->GetStartPosition();
	DWORD nKey;

	// Find the DLL
	while (pos != NULL) 
	{
		g_dlls->GetNextAssoc(pos, nKey, returnInfo);
	
		// Got it
		if (returnInfo->oID == objectID)
		{
			return returnInfo;
		}
	}

	return NULL;
}

void SerializeBitmap(CxImage& Image, CArchive& ar)
{
	if(ar.IsStoring())
	{
		long OutSize = 0;
		
		if(Image.IsValid())
		{
			BYTE *lpBits = NULL;
			Image.Encode(lpBits, OutSize, CXIMAGE_FORMAT_BMP);

			// Now write image information
			ar << OutSize;

			// Now write the bits
			ar.Write(lpBits, OutSize);

			delete[] lpBits;
		}
		else
		{
			ar << (long)0;
		}
	}

	else
	{
		// Read information
		int Size = 0;
		ar >> Size;

		if(Size)
		{
			BYTE *lpBits = new BYTE[Size];

			// Create HBITMAP
			ar.Read(lpBits, Size);
			Image.Decode(lpBits, Size, CXIMAGE_FORMAT_BMP);

			delete[] lpBits;
		}
	}
}

bool SerializeClassHeader(CString& Name, int& Version, CArchive& Ar)
{
	CString LoadName;
	if(Ar.IsLoading())
	{		
		Ar >> LoadName >> Version;

		if (LoadName == Name) return true;
		else
		{
			CString Error;
			Error.Format("Error with loading: expecting class (%s), but recieved (%s) ", Name, LoadName);
			throw runtime_error((const char*)Error);
		}
	}

	else
	{
		Ar << Name << Version;

		return true;
	}

	return false;
}

void SerializeClassPadding(int NumberStrings, int NumberInts, CArchive& Ar)
{
	if(Ar.IsLoading())
	{
		int Number = 0;
		CString String;

		for (int i = 0; i < NumberStrings; i++)
			Ar >> String;

		for (int i = 0; i < NumberInts; i++)
			Ar >> Number;
	}

	else
	{
		int Number = 0;
		CString String;

		for (int i = 0; i < NumberStrings; i++)
			Ar << String;

		for (int i = 0; i < NumberInts; i++)
			Ar << Number;
	}
}
	
int CSortStringArray::Compare(const CString * pstr1, const CString * pstr2)
{
	ASSERT(pstr1);
	ASSERT(pstr2);
	return pstr1->Compare(*pstr2);
}

void CSortStringArray::Sort(STRINGCOMPAREFN pfnCompare)
{
	CString * prgstr = GetData();
	qsort(prgstr,GetSize(),sizeof(CString),(GENERICCOMPAREFN)pfnCompare);
}