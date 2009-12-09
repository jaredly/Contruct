
#ifndef VRUNTIME_HPP
#define VRUNTIME_HPP

#pragma once

#ifndef D3DCOLOR
#define D3DCOLOR unsigned long
#endif

//////////////////////////////////////////////
// CONSTRUCT VIRTUAL RUNTIME
class VRuntime {
public:

	// Display engine virtual runtime functions
	virtual int		AddDefaultTexture() = 0;
	virtual int		AddTextureFromResource(HMODULE hModule, LPCTSTR pResource) = 0;
	virtual int		AddTextureFromFile(char* filename) = 0;
	virtual void	Blit(float x, float y, float xf = 1.0f, float yf = 1.0f, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF) = 0;
	virtual void	Blitrc(float x, float y, float w, float h, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF) = 0;
	virtual void	SetTexture(int index) = 0;

	virtual void	Fill(const RECT rc, D3DCOLOR color) = 0;
	virtual void	Point(int x, int y, D3DCOLOR color) = 0;
	virtual void	Line(int x1, int y1, int x2, int y2, D3DCOLOR color) = 0;
	virtual void	Box(const RECT rc, D3DCOLOR color) = 0;

	virtual void	BeginBatch(CD3DBATCH type, int num) = 0;
	virtual void	EndBatch() = 0;



	virtual int		CreateFont(char* fontface, unsigned int pxSize, bool italics, bool bold) = 0;
	virtual void	DrawText(int iFont, const char* text, int x, int y, D3DCOLOR color) = 0;
	virtual void	DestroyFont(int iFont) = 0;
	virtual HWND	GetAppHwnd() = 0;
	virtual HINSTANCE GetAppHInstance() = 0;

	// Runtime engine virtual functions
	virtual ExpReturn* GetReturnExp() = 0;
	
	// System object virtual runtime functions
	virtual CString	GetClipboardText() = 0;
	virtual int		GetNumberOfSubstrings(LPCSTR string, LPCSTR substrings, int startpos) = 0;
	virtual void	GetSubstring(LPCSTR string, int substr, CString& ret, TCHAR sep) = 0;
	virtual CString	ReadFile(LPCSTR filepath, __int64 startAt, int readBytes) = 0;
	virtual CString	ReadINI(LPCSTR iniFile, LPCSTR section, LPCSTR item) = 0;
	virtual ULONG	SizeOfFile(LPCSTR filepath) = 0;
	virtual void	WriteFile(LPCSTR filePath, __int64 startAt, int readBytes, char* toWrite) = 0;
	virtual void	WriteINI(LPCSTR iniFile, LPCSTR section, LPCSTR item, LPCSTR toWrite) = 0;

};


//////////////////////////////////////////////
// QUADRILATERAL CLASS
// Defines four co-ordinates to describe a transformed texture.
//
// i.e. four points for resizing and stretching etc
//		p1 ******** p2
//		   *      *
//      p3 ******** p4
//	All transformations are relative to p1; p1 never moves.

/*
struct RECTF {
	float left;
	float top;
	float right;
	float bottom;
};
*/


#define Rad(a) float((a) / 57.29577951308f)
#define Deg(a) float((a) * 57.29577951308f)


class Quadrilateral
{
public:
	POINTF p1;		// Top left; origin; remains same throughout transformations
	POINTF p2;		// Top right
	POINTF p3;		// Bottom left
	POINTF p4;		// Bottom right

	// Create from a stretched rect
	void Stretched(RECTF rc, float xf, float yf)
	{
		// Set from rc
		operator = (rc);

		// Width and height
		float w = rc.right - rc.left;
		float h = rc.bottom - rc.top;

		// p1 does not move

		// stretch p2 relative to p1
		p2.x = p1.x + (w * xf);

		// stretch p3 relative to p1
		p3.y = p1.y + (h * yf);

		// stretch p4 relative to p1
		p4.x = p2.x;
		p4.y = p3.y;
	}

	// Create from a rotated rect.  Angle in degrees
	void Rotated(RECTF rc, float angle)
	{
		// Set from rc
		operator = (rc);

		if (angle == 0)
			return;

		// Covert mathematical angle to radians and flip (for bearing)
		angle = Rad(-angle);

		float w = rc.right - rc.left;
		float h = rc.bottom - rc.top;
		float t = atanf(w/h);
		float d = sqrtf((w * w) + (h * h));	// diagonal distance

		// p1 does not move

		// rotate p2 about p1
		p2.x = p1.x + (w * sinf(angle + Rad(90)));
		p2.y = p1.y + (w * cosf(angle + Rad(90)));

		// rotate p3 about p1
		p3.x = p1.x + (h * sinf(angle));
		p3.y = p1.y + (h * cosf(angle));

		// rotate p4 about p1
		p4.x = p1.x + (d * sinf(angle + t));
		p4.y = p1.y + (d * cosf(angle + t));
	}

	// Create from a stretched then rotated rect
	void StretchRotated(RECTF rc, float xf, float yf, float angle)
	{
		// First create from stretch
		Stretched(rc, xf, yf);

		// Make new rect
		RECTF stretched;
		stretched.left = p1.x;
		stretched.top = p1.y;
		stretched.right = p4.x;
		stretched.bottom = p4.y;

		// Create from rotated
		if (angle != 0)
			Rotated(stretched, angle);
	}

	// Create from rect
	operator=(RECTF rc)
	{
		p1.x = rc.left;
		p1.y = rc.top;

		p2.x = rc.right;
		p2.y = rc.top;

		p3.x = rc.left;
		p3.y = rc.bottom;

		p4.x = rc.right;
		p4.y = rc.bottom;
	}

	operator=(RECT rc)
	{
		p1.x = (float)rc.left;
		p1.y = (float)rc.top;

		p2.x = (float)rc.right;
		p2.y = (float)rc.top;

		p3.x = (float)rc.left;
		p3.y = (float)rc.bottom;

		p4.x = (float)rc.right;
		p4.y = (float)rc.bottom;
	}
};


#endif