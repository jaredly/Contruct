 /////////////////////////////////////////////////////////////////////////////
 // RendererLayer.cpp														//
 // Copyright (c) 2009 Scirra (Ashley Gullen, Rich Whilding, David Clark)	//
 /////////////////////////////////////////////////////////////////////////////
 /*
	This program is free software you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
// Implementations of the render layer.
#include "stdafx.h"

CRenderLayer::CRenderLayer(CRuntime* _runtime)
: runtime(_runtime),
  renderer(&(_runtime->renderer))
{}

TextureHandle CRenderLayer::CreateRenderTargetTexture(UINT width, UINT height, texture_format format)
{
	TextureHandle th = renderer->CreateRenderTargetTexture(width, height, format);
	th->IncrementReferences();
	return th;
}

void CRenderLayer::ResizeRenderTargetTexture(TextureHandle th, UINT new_width, UINT new_height)
{
	return renderer->ResizeRenderTargetTexture(th, new_width, new_height);
}

TextureHandle CRenderLayer::CreateTextureFromFile(PCTSTR filename, texture_format format)
{
	TextureHandle th = renderer->CreateTextureFromFile(filename, format, true);
	th->IncrementReferences();
	return th;
}

TextureHandle CRenderLayer::CreateTextureFromFileInMemory(LPCVOID pData, UINT len, texture_format format)
{
	TextureHandle th = renderer->CreateTextureFromFileInMemory(pData, len, format, true);
	th->IncrementReferences();
	return th;
}

TextureHandle CRenderLayer::CreateTextureFromResource(HMODULE hModule, LPCTSTR resource, texture_format format)
{
	TextureHandle th = renderer->CreateTextureFromResource(hModule, resource, format, true);
	th->IncrementReferences();
	return th;
}

TextureHandle CRenderLayer::CreateTextureFromHandle(ImageHandleInfo* ihi)
{
	if (ihi == NULL)
		return unallocated_texture;

	// Already is in VRAM: return the handle to this texture
	if (ihi->IsInVRAM(*renderer)) {
		ihi->th->IncrementReferences();	// Add a new reference (will have a corresponding release)
		return ihi->th;
	}
	// Not in VRAM: transfer since it's wanted now and return handle
	else {
		ihi->TransferToVRAM(*renderer);
		ihi->th->IncrementReferences(); // reference count initialised at 1
		return ihi->th;
	}
}

void CRenderLayer::AddTextureReference(TextureHandle t)
{
	renderer->AddTextureReference(t);
}

void CRenderLayer::ReleaseTexture(TextureHandle t)
{
	if (t)
		renderer->ReleaseTexture(t);
}

void CRenderLayer::DeallocateTexture(TextureHandle t)
{
	renderer->DeallocateTexture(t);
}

bool CRenderLayer::TextureExists(TextureHandle th) const
{
	return renderer->TextureExists(th);
}

void CRenderLayer::ReplaceTexture(TextureHandle oldTex, TextureHandle newTex)
{
	renderer->ReplaceTexture(oldTex, newTex);
}

void CRenderLayer::EndBatch()
{
	renderer->EndBatch();
}

void CRenderLayer::SetRenderState(renderstate rs, renderstate_value rsv)
{
	renderer->SetRenderState(rs, rsv);
}

void CRenderLayer::SetSamplerState(samplerstate ss, samplerstate_value ssv)
{
	renderer->SetSamplerState(ss, ssv);
}

renderstate_value CRenderLayer::GetRenderState(renderstate rs) const
{
	return renderer->GetRenderState(rs);
}

samplerstate_value CRenderLayer::GetSamplerState(samplerstate ss) const
{
	return renderer->GetSamplerState(ss);
}


void CRenderLayer::SetAdditiveBlending()
{
	renderer->SetAdditiveBlending();
}

void CRenderLayer::SetAlphaBlending()
{
	renderer->SetAlphaBlending();
}

void CRenderLayer::ClearRenderTarget(const cr_color& c, const rect* area)
{
	if (area == NULL)
		renderer->ClearRenderTarget(c.premultiplied());
	else {
		D3DRECT rc;
		rc.x1 = area->left;
		rc.y1 = area->top;
		rc.x2 = area->right;
		rc.y2 = area->bottom;
		renderer->ClearRenderTarget(rc, c.premultiplied());
	}
}

void CRenderLayer::SetTexture(TextureHandle th)
{
	renderer->SetTexture(th);
}

void CRenderLayer::SetRenderTarget(TextureHandle th)
{
	rendertarget_stack.push_back(renderer->GetRenderTarget());
	renderer->SetRenderTarget(th);
}

void CRenderLayer::RestoreRenderTarget()
{
	if (!rendertarget_stack.empty()) {
		renderer->SetRenderTarget(rendertarget_stack.back());
		rendertarget_stack.pop_back();
	}
}

TextureHandle CRenderLayer::GetTexture() const
{
	return renderer->GetTexture();
}

TextureHandle CRenderLayer::GetRenderTarget() const
{
	return renderer->GetRenderTarget();
}

void CRenderLayer::Quad_xy(cr_float x, cr_float y, cr_float angle, point hotspot, const cr_color& filter, const rect* _uv, cr_float z_elevation)
{
	runtime->Translate(x, y);
	renderer->SetZOffset(z_elevation);
	renderer->Quad(x, y, cr::to_radians(angle), hotspot, filter.premultiplied(), _uv);
	renderer->SetZOffset(0.0);
}

void CRenderLayer::Quad_xywh(cr_float x, cr_float y, cr_float width, cr_float height, cr_float angle, point hotspot, const cr_color& filter, const rect* _uv, cr_float x_skew, cr_float y_skew, const cr_color* vertexcolors, cr_float z_elevation)
{
	runtime->Translate(x, y);
	renderer->SetSkew(x_skew, y_skew);
	renderer->SetZOffset(z_elevation);

	if (vertexcolors == NULL)
		renderer->Quad(x, y, width, height, cr::to_radians(angle), hotspot, filter.premultiplied(), _uv);
	else {
		cr_color vs[4];

		for (int i = 0; i < 4; i++)
			vs[i] = (vertexcolors[i] * filter).premultiplied();	// mix filters

		rect r(cr::rect_xywh(x, y, width, height));

		if (angle == 0.0) {
			quad q(r - hotspot);
			renderer->Quad(q, vs, _uv);
		}
		else {
			quad q((r - hotspot).rotate_to_quad(angle, r.topleft()));
			renderer->Quad(q, vs, _uv);
		}
	}

	renderer->SetSkew(0.0, 0.0);
	renderer->SetZOffset(0.0);
}

void CRenderLayer::Point(point p, const cr_color& c)
{
	runtime->Translate(p);
	renderer->Point(p, c.premultiplied());
}

// Aliased lines
void CRenderLayer::Line(point p1, point p2, const cr_color& c)
{
	runtime->Translate(p1);
	runtime->Translate(p2);
	renderer->Line(p1, p2, c.premultiplied());
}

void CRenderLayer::Line2(point p1, point p2, const cr_color& c1, const cr_color& c2)
{
	runtime->Translate(p1);
	runtime->Translate(p2);
	renderer->Line(p1, p2, c1.premultiplied(), c2.premultiplied());
}

// Antialiased ID3DXLines
void CRenderLayer::SmoothLine(point p1, point p2, const cr_color& c, cr_float width)
{
	runtime->Translate(p1);
	runtime->Translate(p2);
	renderer->SmoothLine(p1, p2, width, c);	// dont premultiply
}

void CRenderLayer::SmoothLine3D(point3d p1, point3d p2, const cr_color& c, cr_float width)
{
	runtime->Translate(p1);
	runtime->Translate(p2);
	renderer->SmoothLine3D(p1, p2, width, c);	// dont premultiply
}

void CRenderLayer::Box(const rect& r, const cr_color& c, cr_float angle, point hotspot)
{
	renderer->Box(runtime->TranslateRect(r), cr::to_radians(angle), hotspot, c.premultiplied());
}

void CRenderLayer::Fill(const rect& r, const cr_color& c, cr_float angle, point hotspot)
{
	renderer->Fill(runtime->TranslateRect(r), cr::to_radians(angle), hotspot, c.premultiplied());
}

void CRenderLayer::FillGradient(const rect& r, const cr_color& c1, const cr_color& c2, gradient_direction dir, cr_float angle, point hotspot)
{
	renderer->FillGradient(runtime->TranslateRect(r), cr::to_radians(angle), hotspot, c1.premultiplied(), c2.premultiplied(), dir);
}

// Text rendering.  Names involve CR to prevent conflict with WINAPI names
FontHandle CRenderLayer::CreateCRFont(PCTSTR font_face, INT height, UINT width, UINT weight, BOOL italic, DWORD quality, BYTE charset, BYTE pitch_and_family)
{
	return renderer->CreateCRFont(font_face, height, width, weight, italic, quality, charset, pitch_and_family);
}

bool CRenderLayer::FontExists(FontHandle fh) const
{
	return renderer->FontExists(fh);
}

void CRenderLayer::DrawTextCR(FontHandle fh, PCTSTR text, const rect& r, DWORD format, const cr_color& c)
{
	// Don't premultiply text colour...
	renderer->DrawTextCR(fh, text, runtime->TranslateRect(r), format, c);
}

void CRenderLayer::GetTextSize(FontHandle fh, PCTSTR text, rect& r)
{
	renderer->GetTextSize(fh, text, r);
}

void CRenderLayer::ReleaseCRFont(FontHandle fh)
{
	renderer->ReleaseCRFont(fh);
}

void* CRenderLayer::GetDevice() const
{
	return (void*)renderer->GetDevice();
}

void CRenderLayer::SetScreenTranslation(bool enable)
{
	if (enable) {
		runtime->translationMode = TRANSLATE_SCREEN;
		renderer->UpdateTransform(false);
	}
	else {
		runtime->translationMode = TRANSLATE_SCROLL;
		renderer->UpdateTransform(true);
	}
}

void CRenderLayer::SaveTextureToFileInMemory(void** lpBuffer, int* pBufferSize, TextureHandle th, int iImageFormat)
{
	if (th->is_multisampled)
		throw runtime_error("Cannot save multisampled textures");

	// Talk about a hack...
	LPD3DXBUFFER buff = (LPD3DXBUFFER)(*lpBuffer);

	D3DXSaveTextureToFileInMemory(
		&buff,
		(D3DXIMAGE_FILEFORMAT)iImageFormat,
		th->texture_ptr,
		NULL
	);

	if(pBufferSize)
		*pBufferSize = buff->GetBufferSize();

	*lpBuffer = buff;
}

void* CRenderLayer::GetTextureFileBuffer(void** lpBuffer)
{
	return ((LPD3DXBUFFER)(*lpBuffer))->GetBufferPointer();
}

void CRenderLayer::FreeTextureFileMemory(void** lpBuffer)
{
	if(*lpBuffer)
		((LPD3DXBUFFER)(*lpBuffer))->Release();
}

void CRenderLayer::BeginBatchQuads(UINT vertices, UINT indices)
{
	if (renderer->vertices_vector.size() >= cr::d3d_max_vertices_size - vertices) {
		renderer->FlushState();
		renderer->EndBatch();
	}
	if (renderer->indices_vector.size() >= cr::d3d_max_indices_size - indices) {
		renderer->FlushState();
		renderer->EndBatch();
	}

	renderer->BeginBatch(cr::batch_quads);
}

void CRenderLayer::AddIndex(unsigned short index)
{
	cr::CBatch_Draw* cmd = reinterpret_cast<cr::CBatch_Draw*>(renderer->GetTopBatchItem());
	unsigned short base_index = cmd->vertex_count;
	renderer->AddIndex(base_index + index);
	cmd->index_count++;
}

void CRenderLayer::AddVertex(cr::point3d xyz, point uv, const cr_color& c)
{
	runtime->Translate(xyz.x, xyz.y);
	renderer->AddVertex(c.premultiplied().getD3DCOLOR(), xyz, uv);
	reinterpret_cast<cr::CBatch_Draw*>(renderer->GetTopBatchItem())->vertex_count++;
}

void CRenderLayer::SetPointSize(cr_float point_size)
{
	renderer->SetPointSize(point_size);
}

cr_float CRenderLayer::GetPointSize() const
{
	return renderer->GetPointSize();
}

point CRenderLayer::Project(cr::point3d pt) const
{
	return renderer->Project(pt);
}

void CRenderLayer::Project(const cr::point3d* pts, cr::point3d* outs, UINT count) const
{
	renderer->Project(pts, outs, count);
}

point3d CRenderLayer::Unproject(cr::point3d pt) const
{
	return renderer->Unproject(pt);
}

void CRenderLayer::Unproject(const cr::point3d* pts, cr::point3d* outs, UINT count) const
{
	renderer->Unproject(pts, outs, count);
}