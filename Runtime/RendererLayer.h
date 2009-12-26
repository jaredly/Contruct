 /////////////////////////////////////////////////////////////////////////////
 // RendererLayer.h															//
 // Copyright (c) 2009 Scirra (Ashley Gullen, Rich Whilding, David Clark)	//
 /////////////////////////////////////////////////////////////////////////////
 /*
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
// This is the layer between the SDK IRenderer interface and the actual runtime renderer.
// Coordinate translation, filter premultiplication etc. happens here since the SDK calls
// the renderer functions with non-premultiplied colors and layout co-ordinates.
#pragma once

class CRenderLayer : public IRenderer {
public:
	CRenderLayer(CRuntime* _runtime);

	CRuntime* const runtime;
	Renderer* const renderer;

	TextureHandle	CreateRenderTargetTexture(UINT width, UINT height, texture_format format = cr::texture_format_a8r8g8b8);
	void			ResizeRenderTargetTexture(TextureHandle th, UINT new_width, UINT new_height);
	TextureHandle	CreateTextureFromFile(PCTSTR filename, texture_format format = cr::texture_format_a8r8g8b8);
	TextureHandle	CreateTextureFromFileInMemory(LPCVOID pData, UINT len, texture_format format = cr::texture_format_a8r8g8b8);
	TextureHandle	CreateTextureFromResource(HMODULE hModule, LPCTSTR resource, texture_format format = cr::texture_format_a8r8g8b8);

	TextureHandle	CreateTextureFromHandle(ImageHandleInfo* ihi);

	void			AddTextureReference(TextureHandle th);	// Increment references to a texture
	void			ReleaseTexture(TextureHandle th);		// Decrement references and deallocate if zero
	void			DeallocateTexture(TextureHandle th);	// Immediately release from VRAM
	bool			TextureExists(TextureHandle th) const;	// Test if a texture exists
	void			ReplaceTexture(TextureHandle oldTex, TextureHandle newTex);

	void			SaveTextureToFileInMemory(void** lpBuffer, int* pBufferSize, TextureHandle th, int iImageFormat);
	void*			GetTextureFileBuffer(void** lpBuffer);
	void			FreeTextureFileMemory(void** lpBuffer);

	// Batching control - avoid this function if possible, as it flushes the GPU command queue
	void			EndBatch();
	void			BeginBatchQuads(UINT vertices, UINT indices);
	void			AddIndex(unsigned short index);
	void			AddVertex(cr::point3d xyz, point uv, const cr_color& c);

	// State control
	void			SetRenderState(renderstate rs, renderstate_value rsv);
	void			SetSamplerState(samplerstate ss, samplerstate_value ssv);

	renderstate_value	GetRenderState(renderstate rs) const;
	samplerstate_value	GetSamplerState(samplerstate ss) const;

	void			SetAdditiveBlending();
	void			SetAlphaBlending();

	// Drawing
	void			ClearRenderTarget(const cr_color& c = transparent_black, const rect* area = NULL);

	void			SetTexture(TextureHandle th);
	void			SetRenderTarget(TextureHandle th);
	void			RestoreRenderTarget();
	TextureHandle	GetTexture() const;
	TextureHandle	GetRenderTarget() const;

	// Overloads don't seem to work in Construct<->SDK interfaces, so they are named differently here
	void			Quad_xy(cr_float x, cr_float y, cr_float angle = 0.0, point hotspot = origin, const cr_color& filter = opaque_white, const rect* _uv = NULL, cr_float z_elevation = 0.0);
	void			Quad_xywh(cr_float x, cr_float y, cr_float width, cr_float height, cr_float angle = 0.0, point hotspot = origin, const cr_color& filter = opaque_white, const rect* _uv = NULL, cr_float x_skew = 0.0, cr_float y_skew = 0.0, const cr_color* vertexcolors = NULL, cr_float z_elevation = 0.0);
	
	void			Point(point p, const cr_color& c);

	// Aliased lines
	void			Line(point p1, point p2, const cr_color& c);
	void			Line2(point p1, point p2, const cr_color& c1, const cr_color& c2);

	// Antialiased ID3DXLines
	void			SmoothLine(point p1, point p2, const cr_color& c, cr_float width = 1.0);
	void			SmoothLine3D(point3d p1, point3d p2, const cr_color& c, cr_float width = 1.0);
	void			Box(const rect& r, const cr_color& c, cr_float angle = 0.0, point hotspot = origin);
	void			Fill(const rect& r, const cr_color& c, cr_float angle = 0.0, point hotspot = origin);
	void			FillGradient(const rect& r, const cr_color& c1, const cr_color& c2, gradient_direction dir, cr_float angle = 0.0, point hotspot = origin);

	// Text rendering.  Names involve CR to prevent conflict with WINAPI names
	FontHandle		CreateCRFont(PCTSTR font_face, INT height, UINT width = 0, UINT weight = FW_NORMAL, BOOL italic = false, DWORD quality = ANTIALIASED_QUALITY, BYTE charset = DEFAULT_CHARSET, BYTE pitch_and_family = DEFAULT_PITCH|FF_DONTCARE);
	bool			FontExists(FontHandle fh) const;
	void			DrawTextCR(FontHandle fh, PCTSTR text, const rect& r, DWORD format, const cr_color& c);
	void			ReleaseCRFont(FontHandle fh);
	void			GetTextSize(FontHandle fh, PCTSTR text, rect& r);

	void*			GetDevice() const;		// Avoid using if possible, non-portable engine specific device pointer!

	void			SetScreenTranslation(bool enable = true);

	void			SetPointSize(cr_float point_size);
	cr_float		GetPointSize() const;

	point			Project(cr::point3d pt) const;
	void			Project(const cr::point3d* pts, cr::point3d* outs, UINT count) const;
	point3d			Unproject(cr::point3d pt) const;
	void			Unproject(const cr::point3d* pts, cr::point3d* outs, UINT count) const;

	// For setting & restoring rendertargets
	std::vector<TextureHandle>		rendertarget_stack;
};