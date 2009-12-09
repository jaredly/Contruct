#include "StdAfx.h"
#include "CImageResource.h"

CImageResource::CImageResource()
{
	m_Hotspot.x = 0;
	m_Hotspot.y = 0;

	bitmap_data = 0;
	small_thumbnail_data = 0;
	large_thumbnail_data = 0;
}

void CImageResource::Mirror()
{
	bitmap.Mirror();

	large_thumbnail.Destroy();
	small_thumbnail.Destroy();

	if (large_thumbnail_data)
	{
		DeleteObject(large_thumbnail_data);
		large_thumbnail_data = NULL;
	}

	if (small_thumbnail_data)
	{
		DeleteObject(small_thumbnail_data);
		small_thumbnail_data = NULL;
	}
}

bool IsPowerOfTwo(int value)
{
	return (value & -value) == value;
}

bool CImageResource::CopyToImageAndAddGutter(CxImage& copy)
{
	int x,y;
	// This function will put the image resource into emptyimage
	RGBQUAD transparent = {0,0,0,0};

	copy = bitmap;

	
	if(IsPowerOfTwo(bitmap.GetWidth()) && IsPowerOfTwo(bitmap.GetHeight()))
	{
		return false; // it doesn't need the gutter
	}

	copy.Expand(0,0,1,1,transparent);
	copy.AlphaCreate();
	copy.AlphaClear();
	for(x = 0; x < bitmap.GetWidth(); x++)
	{
		for(y = 0; y < bitmap.GetHeight(); y++)
		{
			BYTE arg = bitmap.AlphaGet(x,y);
			copy.AlphaSet(x,y+1,arg);
		}
	}

	for(x = 0; x < copy.GetWidth(); x++)
	{
		RGBQUAD colour = copy.GetPixelColor(x,1, true);
		copy.SetPixelColor(x,0,colour, true);
	}
	for(y = 0; y < copy.GetHeight(); y++)
	{
		RGBQUAD colour = copy.GetPixelColor(copy.GetWidth() - 2,y, true);
		copy.SetPixelColor(copy.GetWidth() - 1,y,colour, true);
	}
	return true;
}

void CImageResource::Copy(CImageResource* other)
{
	bitmap = other->bitmap;
	m_Collision = other->m_Collision;
	small_thumbnail = other->small_thumbnail;
	large_thumbnail = other->large_thumbnail;

	m_Name = other->m_Name;
	m_Dimensions = other->m_Dimensions; 

	m_Hotspot = other->m_Hotspot;

	m_Action = other->m_Action;
	m_FixedID = other->m_FixedID;
	
	bitmap_data = other->bitmap_data;
	small_thumbnail_data = other->small_thumbnail_data;
	large_thumbnail_data = other->large_thumbnail_data;

}

void CImageResource::Flip()
{
	bitmap.Flip();

	large_thumbnail.Destroy();
	small_thumbnail.Destroy();

	if (large_thumbnail_data)
	{
		DeleteObject(large_thumbnail_data);
		large_thumbnail_data = NULL;
	}

	if (small_thumbnail_data)
	{
		DeleteObject(small_thumbnail_data);
		small_thumbnail_data = NULL;
	}
}

CxImage* CImageResource::GetThumbnail(bool large)
{
	if(large)
	{
		if(!large_thumbnail.IsValid())
		{
			if(large_thumbnail_data)
				DeleteObject(large_thumbnail_data);
			large_thumbnail_data = 0;


			CreatePremultipliedThumbnail(&bitmap, &large_thumbnail, 285, 226,0xffffff, true);
		}
		return &large_thumbnail;
	}
	else
	{
		if(!small_thumbnail.IsValid())
		{
			if(small_thumbnail_data)
				DeleteObject(small_thumbnail_data);
			small_thumbnail_data = 0;

			CreatePremultipliedThumbnail(&bitmap, &small_thumbnail, 32,32, 0xffffff, 0);
		}
		return &small_thumbnail;
	}
}

HBITMAP CImageResource::GetHBitmap(bool thumbnail, bool large)
{
	if(thumbnail)
	{
		if(large)
		{
			GetThumbnail(true);
			if(!large_thumbnail_data)
				large_thumbnail_data = GetThumbnail(true)->MakeBitmap();


			return large_thumbnail_data;
		}
		else
		{
			GetThumbnail(false);
			if(!small_thumbnail_data)
				small_thumbnail_data = GetThumbnail(false)->MakeBitmap();
			return small_thumbnail_data;
		}
	}
	else
	{
		if(bitmap_data)
			DeleteObject(bitmap_data);
		return bitmap_data = bitmap.MakeBitmap();
	}
}

void CImageResource::Premultiply()
{
	if(bitmap.AlphaIsValid())
	{
		for(int x = 0; x < bitmap.GetWidth(); x++)
		{
			for(int y = 0; y < bitmap.GetHeight(); y++)
			{
				RGBQUAD c;
				c = bitmap.GetPixelColor(x,y);
				float ratio = c.rgbReserved / 255.0;
				c.rgbRed *= ratio;
				c.rgbGreen *= ratio;
				c.rgbBlue *= ratio;
				bitmap.SetPixelColor(x,y,c);
			}
		}
	}
	else
	{
		bitmap.AlphaCreate();
		bitmap.AlphaClear();
		bitmap.AlphaInvert();
	}
}

void CImageResource::Predivide()
{
	if(bitmap.AlphaIsValid())
	{
		for(int x = 0; x < bitmap.GetWidth(); x++)
		{
			for(int y = 0; y < bitmap.GetHeight(); y++)
			{
				RGBQUAD c;
				c = bitmap.GetPixelColor(x,y);
				float ratio = c.rgbReserved / 255.0;
				if(ratio != 0)
				{
					c.rgbRed /= ratio;
					c.rgbGreen /= ratio;
					c.rgbBlue /= ratio;
					bitmap.SetPixelColor(x,y,c);
				}
			}
		}
	}
	else
	{
		bitmap.AlphaCreate();
		bitmap.AlphaClear();
		bitmap.AlphaInvert();
	}
}

void CImageResource::ColourReplace(COLORREF col1, COLORREF col2)
{
	RGBQUAD r;
	r.rgbRed = GetRValue(col2);
	r.rgbGreen = GetGValue(col2);
	r.rgbBlue = GetBValue(col2);
	r.rgbReserved = col2 >> 24;

	for(int x = 0; x < bitmap.GetWidth(); x++)
	{
		for(int y = 0; y < bitmap.GetHeight(); y++)
		{
			RGBQUAD c;
			c.rgbRed = GetRValue(col1);
			c.rgbGreen = GetGValue(col1);
			c.rgbBlue = GetBValue(col1);

			RGBQUAD src = bitmap.GetPixelColor(x,y);
			c.rgbReserved = src.rgbReserved;
			float ratio =  src.rgbReserved / 255.0;
			c.rgbRed *= ratio;
			c.rgbGreen *= ratio;
			c.rgbBlue *= ratio;

			if(*((int*)&c) == *((int*)&src))
			{
				bitmap.SetPixelColor(x,y,r, true);
			}	
		}
	}
}

bool CImageResource::Serialize(CArchive& ar)
{
	CString ExpectedName = "CImageResource";
	int     Version      = 2;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return false;

	if(ar.IsLoading())
	{
		ar >> m_Name;
		ar >> m_FixedID;

		ar >> m_Dimensions;
		ar >> m_Hotspot.x;
		ar >> m_Hotspot.y;
		UINT actionsize;
		ar >> actionsize;

		for(int a = 0; a < actionsize; a ++)
		{
			CPoint pt;
			CString name;
			ar >> pt.x;
			ar >> pt.y;
			ar >> name;
			m_Action[name] = pt;
		}
			
		long size;
		ar >> size;
		BYTE* buffer = new BYTE[size];
		ar.Read( buffer,size);
			
		bitmap.Decode(buffer,size,CXIMAGE_FORMAT_PNG);
		delete[] buffer;

		if(Version >= 2)
		{
			ar >> size;
			if(size > 0)
			{
				int format;
				ar >> format;
				BYTE* buffer = new BYTE[size];
				ar.Read( buffer,size);

				m_Collision.Decode(buffer,size,format);
				delete[] buffer;
			}
		}
	}

	else
	{
		ar << m_Name;
		ar << m_FixedID;
		ar << m_Dimensions;
		ar << m_Hotspot.x;
		ar << m_Hotspot.y;
		ar << m_Action.size();

		for(map<CString, CPoint>::iterator i = m_Action.begin(); i!= m_Action.end(); i++)
		{
			ar << i->second.x;
		    ar << i->second.y;
			ar << i->first;
		}

		BYTE *buffer = 0;
		long size;
		bitmap.Encode(buffer,size,CXIMAGE_FORMAT_PNG);
		ar << size;
		ar.Write(buffer,size);
		bitmap.FreeMemory(buffer);

		size = 0;
		if(m_Collision.IsValid())
		{
			BYTE *buffer = 0;
			m_Collision.Encode(buffer,size,CXIMAGE_FORMAT_PNG);
			ar << size;
			ar << (int)CXIMAGE_FORMAT_PNG;
			ar.Write(buffer,size);
			m_Collision.FreeMemory(buffer);
		}
		else
			ar << size;
	
	}
	
	return true;
}
