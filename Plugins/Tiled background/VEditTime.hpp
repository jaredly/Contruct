#ifndef VEDITTIME_H
#define VEDITTIME_H

class VEditTime {
public:

	virtual void		Fill(RECT rc, D3DCOLOR color) = 0;
	virtual void		Line(int x1, int y1, int x2, int y2, D3DCOLOR color) = 0;
	virtual void		Line(int x1, int y1, int x2, int y2, D3DCOLOR color1, D3DCOLOR color2) = 0;
	virtual void		Box(RECT rc, D3DCOLOR color) = 0;
	virtual void		SetTexture(int index) = 0;
	virtual int			AddDefaultTexture() = 0;
	virtual int			AddTextureFromResource(HMODULE hModule, LPCTSTR pResource) = 0;
	virtual void		Blitrc(float x, float y, float w, float h, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF) = 0;

	virtual int			CreateFont(char* fontface, unsigned int pxSize, bool italics, bool bold) = 0;
	virtual void		DrawText(int iFont, const char* text, int x, int y, D3DCOLOR color, DWORD format) = 0;
	virtual void		DestroyFont(int iFont) = 0;
	virtual HWND		GetFrameHwnd() = 0;

	// Non virtual workarounds
	void UseMyResources() { AFX_MANAGE_STATE(AfxGetStaticModuleState( )); }
};
#endif // Only include once

// Handy define for loading resources from this ext
#define AddTextureFromResource2(p) AddTextureFromResource(AfxGetResourceHandle(), (p))