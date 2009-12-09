#pragma once

class CImageResource
{
public:
	CImageResource();
	CxImage bitmap;
	CxImage m_Collision;
	CxImage	small_thumbnail;
	CxImage large_thumbnail;

	CString m_Name;  //null if it doesn't have one
	CString m_Dimensions; 

	CPoint m_Hotspot;

	map<CString, CPoint> m_Action;

	CxImage* GetThumbnail(bool large);
	HBITMAP GetHBitmap(bool thumbnail, bool large);

	int	 m_FixedID;

	bool Serialize(CArchive& ar);

	void Premultiply();	// If you load a frame using CxImage then u probably want to call this function
	void Predivide();
	void ColourReplace(COLORREF col1, COLORREF col2);

	void Mirror();
	void Flip();

protected:

	void UpdateThumbnails();

	HBITMAP bitmap_data;
	HBITMAP small_thumbnail_data;
	HBITMAP large_thumbnail_data;
public:
	void Copy(CImageResource* other);
	bool CopyToImageAndAddGutter(CxImage& emptyimage); // dest usually will become a row/column of pixels larger
};