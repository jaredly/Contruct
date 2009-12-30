 /////////////////////////////////////////////////////////////////////////////
 // CDX9Renderer															//
 // Copyright (c) 2009 Scirra												//
 // Written by Ashley Gullen												//
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
#include "stdafx.h"
#include "CDX9Renderer.h"

#ifdef CDX9RENDERER_USE_BATCH_EXTENSIONS
#include "..\Rundefs.h"
#endif //CDX9RENDERER_USE_BATCH_EXTENSIONS

namespace cr {

	CBatchBase::CBatchBase(CDX9Renderer* _renderer, batch_operation _op)
		: renderer(_renderer),
		  op(_op)
	{}

	// Allocator for batch objects
	void* CBatchBase::operator new(uint num_bytes, CDX9Renderer* _renderer)
	{
		return _renderer->BatchAllocate(num_bytes);	// Let renderer do a fast allocation
	}

	// CDX9Renderer allocator for batch objects
	void* CDX9Renderer::BatchAllocate(uint num_bytes)
	{
		// Check if we can fit this size in to the current chunk
		uint bytes_free = allocation_block_size - (current_alloc_pos - current_batch_block->begin());

		// We can fit this in the current block
		if (num_bytes < bytes_free) {
			// Return memory from this allocation block
			void* ret = &(*current_alloc_pos);
			current_alloc_pos += num_bytes;
			return ret;
		}
		// Will not fit in this chunk
		else {
			BatchMemIterator next = current_batch_block;
			++next;

			// Run out of chunks
			if (next == batch_memory.end()) {
				// Add a new chunk
				batch_memory.push_back(BatchAllocBlock());
				batch_memory.back().resize(allocation_block_size);
			}

			// Move on to next chunk
			++current_batch_block;
			current_alloc_pos = current_batch_block->begin();

			// Return some memory
			void* ret = &(*current_alloc_pos);
			current_alloc_pos += num_bytes;
			return ret;
		}
	}

	void CDX9Renderer::BeginBatch(batch_type type)
	{
		// add any pending state changes to batch
		FlushState();

		// If out of vertices start a new batch
		if (vertices_vector.size() >= d3d_max_vertices_size - 5)
			EndBatch();

		bool new_batch_needed = false;

		// Nothing in batch, need a new entry
		if (batch.empty())
			new_batch_needed = true;
		// Last operation isn't a CBatch_Draw - need to add another
		else if (batch.back()->op != batchop_draw)
			new_batch_needed = true;
		// Last operation is a CBatch_Draw but it's for a different type (eg. lines and we want quads)
		else if (reinterpret_cast<CBatch_Draw*>(batch.back())->type != type)
			new_batch_needed = true;

		if (!new_batch_needed)
			return;

		// Add a new draw op
		batch.push_back(new (this) CBatch_Draw(this, type));
	}

	CBatchBase* CDX9Renderer::GetTopBatchItem() const
	{
		return batch.back();
	}

	void CDX9Renderer::ResetBatch()
	{
		// Erase all vertices and indices (maintain capacity)
		vertices_vector.resize(0);
		indices_vector.resize(0);

		// Reset the batch memory allocator, effectively throwing away all the memory but maintaining capacity
		current_batch_block = batch_memory.begin();
		current_alloc_pos = current_batch_block->begin();

		processed_vertices = 0;
		processed_indices = 0;

		smoothline_index = -1;
		processed_smoothlines = 0;

		// Resize all existing entries to 0; prevent reallocations
		SmoothLineListIterator i = smoothlines.begin();
		SmoothLineListIterator end = smoothlines.end();

		for ( ; i != end; ++i)
			i->resize(0);

		text_index = 0;
		texts_to_render.resize(0);
		batch.resize(0);
	}

	void CDX9Renderer::EndBatch()
	{
		if (batch.empty())
			return;

		if(begin_scene_nest == 0)
		{
			// Its better if this doesn't happen...but if it does...force direct x to have 
			// a begin scene otherwise it will crash. This messes up parallel processing, 
			// but its better than the runtime crashing! - Davo
			CRASSERT(d3d9_device != NULL);
			hr = d3d9_device->BeginScene();
		}

		batches_per_present++;

#ifdef CR_DEBUG
		if (debug_save_next_batch)
			DoBatchLog();
#endif

		// Lock exactly the right number of indices and vertices
		vertex* vertex_mem;
		unsigned short* index_mem;

		unsigned int vertex_bytes = vertices_vector.size() * sizeof(vertex);
		unsigned int index_bytes = indices_vector.size() * sizeof(unsigned short);

		// Update only when bytes are to be copied (D3D interprets 0 as 'lock entire buffer'!)
		if (vertex_bytes != 0) {

			batch_buffer->Lock(0, vertex_bytes, (void**)&vertex_mem, D3DLOCK_DISCARD);
			memcpy(vertex_mem, &(vertices_vector.front()), vertex_bytes);
			batch_buffer->Unlock();
		}
		if (index_bytes != 0) {

			index_buffer->Lock(0, index_bytes, (void**)&index_mem, D3DLOCK_DISCARD);
			memcpy(index_mem, &(indices_vector.front()), index_bytes);
			index_buffer->Unlock();
		}

		// Loop the batch doing every item
		BatchIterator i = batch.begin();
		BatchIterator batch_end = batch.end();

		for ( ; i != batch_end; ++i)
			(*i)->Do();

		if(begin_scene_nest == 0)
		{
			// Its better if this doesn't happen...but if it does...force direct x to have 
			// a begin scene otherwise it will crash. This messes up parallel processing, 
			// but its better than it crashing! - Davo
			CRASSERT(d3d9_device != NULL);
			hr = d3d9_device->EndScene();
		}

		// Put everything back
		ResetBatch();
	}

	// Copy multisample surface to texture (DX wants us to use StretchRect for multisampled stuff)
	void CDX9Renderer::CopyMultisampleSurfaceToTexture(TextureHandle ms, TextureHandle th)
	{
		CRASSERT(TextureExists(ms) && TextureExists(th));

		// Check the multisample surface was created with multisampled = true in CreateRenderTargetTexture
		// and that the th parameter wasn't!
		CRASSERT(ms->surface_ptr != NULL && th->texture_ptr != NULL);

		batch.push_back(new (this) CBatch_CopySurfaceToTexture(this, ms, th));
	}

	void CDX9Renderer::CopyMultisampleSurfaceToTexture(TextureHandle ms, TextureHandle th, const RECT& area)
	{
		CRASSERT(TextureExists(ms) && TextureExists(th));

		// Check the multisample surface was created with multisampled = true in CreateRenderTargetTexture
		// and that the th parameter wasn't!
		CRASSERT(ms->surface_ptr != NULL && th->texture_ptr != NULL);

		batch.push_back(new (this) CBatch_CopySurfaceToTexture(this, ms, th, area));
	}

	// Use stretchrect: the multisampled target is really a surface, not a texture
	void CBatch_CopySurfaceToTexture::Do()
	{
		IDirect3DSurface9* tex_surface;
		th->texture_ptr->GetSurfaceLevel(0, &tex_surface);

		const RECT* area_ptr = (all ? NULL : &area);

		renderer->d3d9_device->StretchRect(ms->surface_ptr, area_ptr, tex_surface, area_ptr, D3DTEXF_POINT);
		tex_surface->Release();
	}

	// Clear render target
	void CDX9Renderer::ClearRenderTarget(const color& c)
	{
		batch.push_back(new (this) CBatch_ClearRenderTarget(this, c));
	}

	void CDX9Renderer::ClearRenderTarget(const D3DRECT& rect, const color& c)
	{
		batch.push_back(new (this) CBatch_ClearRenderTarget(this, c, rect));
	}

	void CBatch_ClearRenderTarget::Do()
	{
		HRESULT hr;

		// Clear entire surface
		if (clearAll)
			hr = renderer->d3d9_device->Clear(0, NULL, D3DCLEAR_TARGET, c, 0.0f, 0);
		// Clear area specified by D3DRECT
		else
			hr = renderer->d3d9_device->Clear(1, &rect, D3DCLEAR_TARGET, c, 0.0f, 0);

		if (FAILED(hr))
			throw CDX9Renderer::error(_T("Failed to clear render target"), hr);
	}

	// Set texture
	void CDX9Renderer::SetTexture(TextureHandle th)
	{
		// Already the current texture, discard
		if (th == current_texture)
			return;

		// Add a command
		if (th != NULL) {
			CRASSERT(TextureExists(th));

			// Multisampled rendertargets are really surfaces.  You can't call SetTexture for them.
			if (th->is_multisampled)
				throw error(_T("Cannot set a multisampled rendertarget as texture"), E_FAIL);
		}

		current_texture = th;

		batch.push_back(new (this) CBatch_SetTexture(this, th));
	}

	void CBatch_SetTexture::Do()
	{
		renderer->d3d9_device->SetTexture(0, th == NULL ? NULL : th->texture_ptr);
	}

	// Set rendertarget
	void CDX9Renderer::SetRenderTarget(TextureHandle th)
	{
		// If backbuffer was changed a render target of NULL means something different
		if (back_buffer_was_changed)
			back_buffer_was_changed = false;
		// Check not the same render target (if backbuffer was not changed)
		else if (th == current_rendertarget)
			return;

		current_rendertarget = th;

		uint new_rt_width, new_rt_height;

		if (th != NULL) {
			CRASSERT(TextureExists(th));

			if (!th->rendertarget)
				throw error(_T("Texture not a render target"), E_FAIL);

			new_rt_width = th->image_width;
			new_rt_height = th->image_height;
		}
		else {
			new_rt_width = backbuffer_width;
			new_rt_height = backbuffer_height;
		}

		bool update_3d_math = false;

		// If rendertarget has changed dimensions we need new 3D math
		if (new_rt_width != state.last_rendertarget_width || new_rt_height != state.last_rendertarget_height) {
			update_3d_math = true;
			state.last_rendertarget_width = new_rt_width;
			state.last_rendertarget_height = new_rt_height;
		}

		// Add command
		batch.push_back(new (this) CBatch_SetRenderTarget(this, th, update_3d_math));
	}

	void CBatch_SetRenderTarget::Do()
	{
		if (th == NULL)
			renderer->d3d9_device->SetRenderTarget(0, renderer->back_buffer);
		else {
			if (th->is_multisampled) {
				renderer->d3d9_device->SetRenderTarget(0, th->surface_ptr);
			}
			else {
				IDirect3DSurface9* s;
				th->texture_ptr->GetSurfaceLevel(0, &s);
				renderer->d3d9_device->SetRenderTarget(0, s);
				s->Release();
			}
		}

		if (update_3d_math) {
			
			if (th == NULL)
				renderer->Update3DMaths(renderer->backbuffer_widthf, renderer->backbuffer_heightf);
			else
				renderer->Update3DMaths(th->image_widthf, th->image_heightf);

			renderer->UpdateTransform();

			// UpdateTransform only sets a flag to add something to the batch if necessary
			// Since we are running the batch, and there won't be a FlushState() call, check
			// to set the transform now
			if (renderer->state.transform.transform_is_pending) {

				renderer->d3d9_device->SetTransform(D3DTS_WORLD, &(renderer->state.transform.pending_world));

				renderer->state.transform.current_world = renderer->state.transform.pending_world;
				renderer->state.transform.transform_is_pending = false;
			}
		}
	}

	// Setting render states & sampler states
	void CDX9Renderer::SetRenderState(renderstate rs, renderstate_value rsv)
	{
		// Is this the same as the current device state?
		if (state.current_renderstates[rs] == rsv) {
			state.pending_renderstates[rs] = rsv_null;	// make no change and quit
			state.renderstate_bits.set(rs, false);		// this state is no longer changed
			return;
		}

		// Update the pending state
		state.pending_renderstates[rs] = rsv;
		state.renderstate_bits.set(rs, true);	// this state has changed
	}

	void CDX9Renderer::SetSamplerState(samplerstate ss, samplerstate_value ssv)
	{
		// Is this the same as the current device state?
		if (state.current_samplerstates[ss] == ssv) {
			state.pending_samplerstates[ss] = ssv_null;	// make no change and quit
			state.samplerstate_bits.set(ss, false);		// this state is no longer changed
			return;
		}

		// Update the pending state
		state.pending_samplerstates[ss] = ssv;
		state.samplerstate_bits.set(ss, true);	// this state has changed
	}

	void CDX9Renderer::SetPointSize(cr_float _point_size)
	{
		if (state.point_size == _point_size)
			return;

		state.point_size = _point_size;

		// Update the pending state
		state.pending_renderstates[rs_pointsize] = rsv_enabled;
		state.renderstate_bits.set(rs_pointsize, true);	// this state has changed
	}

	cr_float CDX9Renderer::GetPointSize() const
	{
		return state.point_size;
	}

	void CBatch_SetPointSize::Do()
	{
		renderer->d3d9_device->SetRenderState(D3DRS_POINTSIZE, *(DWORD*)&pt_size);
	}

	void CDX9Renderer::SetZBufferEnabled(bool enable)
	{
		state.zbuffer_enabled = enable;

		if (enable)
			SetRenderState(rs_zbuffer_enabled, rsv_enabled);
		else
			SetRenderState(rs_zbuffer_enabled, rsv_disabled);
	}

	bool CDX9Renderer::GetZBufferEnabled() const
	{
		return state.zbuffer_enabled;
	}

	void CDX9Renderer::FlushState()
	{
		// If projection has changed add it to the batch
		if (state.transform.transform_is_pending) {
			state.transform.transform_is_pending = false;
			state.transform.current_world = state.transform.pending_world;

			batch.push_back(new (this) CBatch_SetTransform(this, D3DTS_WORLD, state.transform.pending_world));
		}

		// If renderstates have changed add them to the batch
		if (state.renderstate_bits.any()) {
			state.renderstate_bits.reset();

			for (int i = 1; i < renderstate_count; i++) {
				if (state.pending_renderstates[i] != rsv_null) {

					switch ((renderstate)i) {
					case rs_pointsize:
						batch.push_back(new (this) CBatch_SetPointSize(this, state.point_size));
						break;
					default:
						batch.push_back(new (this) CBatch_SetRenderState(this, (renderstate)i, state.pending_renderstates[i]));
					}

					state.current_renderstates[i] = state.pending_renderstates[i];
					state.pending_renderstates[i] = rsv_null;
				}
			}
		}

		// If sampler states have changed add them to the batch
		if (state.samplerstate_bits.any()) {
			state.samplerstate_bits.reset();

			for (int i = 1; i < samplerstate_count; i++) {
				if (state.pending_samplerstates[i] != ssv_null) {
					batch.push_back(new (this) CBatch_SetSamplerState(this, (samplerstate)i, state.pending_samplerstates[i]));

					state.current_samplerstates[i] = state.pending_samplerstates[i];
					state.pending_samplerstates[i] = ssv_null;
				}
			}
		}
	}

	void CBatch_SetRenderState::Do()
	{
		renderer->d3d9_device->SetRenderState(CDX9Renderer::get_d3d9_equiv(rs), CDX9Renderer::get_d3d9_equiv(rs, rsv));
	}

	void CBatch_SetSamplerState::Do()
	{
		renderer->d3d9_device->SetSamplerState(0, CDX9Renderer::get_d3d9_equiv(ss), CDX9Renderer::get_d3d9_equiv(ssv));
	}

	renderstate_value CDX9Renderer::GetRenderState(renderstate rs) const
	{
		if (state.pending_renderstates[rs] != rsv_null)
			return state.pending_renderstates[rs];

		return state.current_renderstates[rs];
	}

	samplerstate_value CDX9Renderer::GetSamplerState(samplerstate ss) const
	{
		if (state.pending_samplerstates[ss] != ssv_null)
			return state.pending_samplerstates[ss];

		return state.current_samplerstates[ss];
	}

	// Do transform
	void CBatch_SetTransform::Do()
	{
		renderer->d3d9_device->SetTransform(transform, &matrix);
	}

	void CDX9Renderer::AddVertex(D3DCOLOR c, point3d xyz, point uv)
	{
		vertices_vector.push_back(vertex());
		vertex& vx = vertices_vector.back();

		vx.x = xyz.x;
		vx.y = xyz.y;
		vx.z = xyz.z + state.z_offset;
		vx.color = c;
		vx.u = uv.x;
		vx.v = uv.y;
	}

	void CDX9Renderer::AddVertex(D3DCOLOR c, point xy, point uv)
	{
		AddVertex(c, xy.x, xy.y, uv.x, uv.y);
	}

	void CDX9Renderer::AddVertex(D3DCOLOR c, point xy, float u, float v)
	{
		AddVertex(c, xy.x, xy.y, u, v);
	}

	void CDX9Renderer::AddVertex(D3DCOLOR c, float x, float y, float u, float v)
	{
		vertices_vector.push_back(vertex());
		vertex& vx = vertices_vector.back();

		vx.x = x;
		vx.y = y;
		vx.z = state.z_offset;
		vx.color = c;
		vx.u = u;
		vx.v = v;
	}

	void CDX9Renderer::AddIndex(unsigned short i)
	{
		indices_vector.push_back(i);
	}

	// Draw quad
	void CDX9Renderer::Quad(const quad& q, const color& filter, const rect* _uv)
	{
		if (current_texture != NULL && current_texture == current_rendertarget)
			throw error(_T("Illegal rendering operation attempted"), E_FAIL);	// Not allowed to draw with texture being the render target

		BeginBatch(batch_quads);

		CBatch_Draw* draw_op = reinterpret_cast<CBatch_Draw*>(batch.back());

		rect uv;

		// No texture: UV coords don't matter
		if (current_texture == NULL)
			uv = zero_rect;
		else {
			
			// Texture supplied but no UV given: draw entire texture
			if (_uv == NULL)
				uv = point(current_texture->xf, current_texture->yf).make_rect();
			else
				uv = (*_uv) * point(current_texture->xf, current_texture->yf);
	
	/*
			// ??????? What's going on with UV coordinates?  Why does this work?  lol
			if (_uv == NULL)
				uv = unit_rect;
			else
				uv = *_uv;
	*/
	
		}

		//D3DCOLOR c = filter.getD3DCOLOR();
		//D3DCOLOR c = 0xFFFFFFFF;
		D3DCOLOR c = cr::sse2_D3DCOLOR_from_color(filter);

		AddVertex(c, q.tl + point(state.x_skew, 0.0),			uv.topleft());
		AddVertex(c, q.tr + point(state.x_skew, state.y_skew),	uv.topright());
		AddVertex(c, q.bl,										uv.bottomleft());
		AddVertex(c, q.br + point(0.0, state.y_skew),			uv.bottomright());

		unsigned short index = draw_op->vertex_count;
		AddIndex(index);
		AddIndex(index + 1);
		AddIndex(index + 2);
		AddIndex(index + 2);
		AddIndex(index + 1);
		AddIndex(index + 3);

		draw_op->vertex_count += 4;
		draw_op->index_count += 6;
	}

	void CDX9Renderer::Quad(const quad& q, const color* vertexcolors, const rect* _uv)
	{
		if (current_texture != NULL && current_texture == current_rendertarget)
			throw error(_T("Illegal rendering operation attempted"), E_FAIL);	// Not allowed to draw with texture being the render target

		BeginBatch(batch_quads);

		CBatch_Draw* draw_op = reinterpret_cast<CBatch_Draw*>(batch.back());

		rect uv;

		// No texture: UV coords don't matter
		if (current_texture == NULL)
			uv = zero_rect;
		else {
			// Texture supplied but no UV given: draw entire texture
			if (_uv == NULL)
				uv = unit_rect;
			else
				uv = *_uv;
		}

		AddVertex(vertexcolors[0].getD3DCOLOR(), q.tl + point(state.x_skew, 0.0),			uv.topleft());
		AddVertex(vertexcolors[1].getD3DCOLOR(), q.tr + point(state.x_skew, state.y_skew),	uv.topright());
		AddVertex(vertexcolors[2].getD3DCOLOR(), q.bl,										uv.bottomleft());
		AddVertex(vertexcolors[3].getD3DCOLOR(), q.br + point(0.0, state.y_skew),			uv.bottomright());

		unsigned short index = draw_op->vertex_count;
		AddIndex(index);
		AddIndex(index + 1);
		AddIndex(index + 2);
		AddIndex(index + 2);
		AddIndex(index + 1);
		AddIndex(index + 3);

		draw_op->vertex_count += 4;
		draw_op->index_count += 6;
	}

	// This function draws a texture at a single given point - a texture has to be set otherwise it can't get the width and height to draw
	void CDX9Renderer::Quad(const point& p, cr_float angle, point hotspot, const color& filter, const rect* _uv)
	{
		// Check a texture is set
		if (current_texture == NULL)
			throw error(_T("Can't draw a quad using this overload with no texture set"), E_FAIL);

		const rect r(p.x, p.y, p.x + current_texture->image_widthf, p.y + current_texture->image_heightf);
		Quad(r, angle, origin, filter, _uv);
	}

	void CDX9Renderer::Quad(cr_float x, cr_float y, cr_float angle, point hotspot, const color& filter, const rect* _uv)
	{
		Quad(point(x, y), angle, hotspot, filter, _uv);
	}

	void CDX9Renderer::Quad(cr_float x, cr_float y, cr_float width, cr_float height, cr_float angle, point hotspot, const color& filter, const rect* _uv)
	{
		Quad(rect(x, y, x + width, y + height), angle, hotspot, filter, _uv);
	}

	void CDX9Renderer::Quad(const rect& r, cr_float angle, point hotspot, const color& filter, const rect* _uv)
	{
		// Avoid math for unrotated rects
		if (angle == 0.0)
			Quad(r - hotspot, filter, _uv);
		else 
			Quad((r - hotspot).rotate_to_quad(angle, r.topleft()), filter, _uv);
	}

	void CBatch_Draw::Do()
	{
		HRESULT hr;

		switch (type) {
		case batch_points:
			// 1 vertex to a point
			hr = renderer->d3d9_device->DrawPrimitive(D3DPT_POINTLIST, renderer->processed_vertices, vertex_count);
			break;
		case batch_linestrip:
			// 1 vertex per segment of linestrip; uses indices
			hr = renderer->d3d9_device->DrawIndexedPrimitive(D3DPT_LINESTRIP, renderer->processed_vertices, 0, vertex_count, renderer->processed_indices, vertex_count);
			break;
		case batch_lines:
			// 2 vertices per line
			hr = renderer->d3d9_device->DrawPrimitive(D3DPT_LINELIST, renderer->processed_vertices, vertex_count / 2);
			break;
		case batch_quads:
			// 3 vertices per triangle
			hr = renderer->d3d9_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, renderer->processed_vertices, 0, vertex_count, renderer->processed_indices, index_count / 3);
			break;
		}

		if (FAILED(hr))
			throw CDX9Error(_T("Failed drawing"), hr);

		renderer->processed_vertices += vertex_count;
		renderer->processed_indices += index_count;
	}

	void CDX9Renderer::Point(point p, const color& c)
	{
		BeginBatch(batch_points);
		CBatch_Draw* draw_op = reinterpret_cast<CBatch_Draw*>(batch.back());

		AddVertex(c.getD3DCOLOR(), p, origin);
		draw_op->vertex_count++;
	}

	void CDX9Renderer::Point(cr_float x, cr_float y, const color& c)
	{
		Point(point(x, y), c);
	}

	void CDX9Renderer::Line(cr_float x1, cr_float y1, cr_float x2, cr_float y2, const color& c)
	{
		Line(point(x1, y1), point(x2, y2), c);
	}

	void CDX9Renderer::Line(cr_float x1, cr_float y1, cr_float x2, cr_float y2, const color& c1, const color& c2)
	{
		Line(point(x1, y1), point(x2, y2), c1, c2);
	}

	void CDX9Renderer::Line(point p1, point p2, const color& c)
	{
		// No support for textured lines
		SetTexture(NULL);

		BeginBatch(batch_lines);
		CBatch_Draw* draw_op = reinterpret_cast<CBatch_Draw*>(batch.back());

		D3DCOLOR color = c.getD3DCOLOR();

		AddVertex(color, p1, 0.0f, 0.0f);
		AddVertex(color, p2, 1.0f, 1.0f);
		draw_op->vertex_count += 2;
	}

	void CDX9Renderer::Line(point p1, point p2, const color& c1, const color& c2)
	{
		// No support for textured lines
		SetTexture(NULL);

		BeginBatch(batch_lines);
		CBatch_Draw* draw_op = reinterpret_cast<CBatch_Draw*>(batch.back());

		AddVertex(c1.getD3DCOLOR(), p1, 0.0f, 0.0f);
		AddVertex(c2.getD3DCOLOR(), p2, 1.0f, 1.0f);
		draw_op->vertex_count += 2;
	}

	void CDX9Renderer::Box(cr_float x, cr_float y, cr_float width, cr_float height, const color& c)
	{
		Box(rect_xywh(x, y, width, height), c);
	}

	void CDX9Renderer::Box(const rect& r, const color& c)
	{
		// No support for textured lines
		SetTexture(NULL);

		BeginBatch(batch_linestrip);
		CBatch_Draw* draw_op = reinterpret_cast<CBatch_Draw*>(batch.back());

		D3DCOLOR color = c.getD3DCOLOR();

		// 4 vertices and use 5th index to repeat first vertex closing the strip as a box
		AddVertex(color, r.topleft(), 0.0f, 0.0f);
		AddVertex(color, r.topright(), 0.0f, 0.0f);
		AddVertex(color, r.bottomright(), 0.0f, 0.0f);
		AddVertex(color, r.bottomleft(), 0.0f, 0.0f);

		unsigned short index = draw_op->vertex_count;
		AddIndex(index);
		AddIndex(index + 1);
		AddIndex(index + 2);
		AddIndex(index + 3);
		AddIndex(index);

		draw_op->vertex_count += 4;
		draw_op->index_count += 5;
	}

	void CDX9Renderer::Box(const rect& r, cr_float angle, point hotspot, const color& c)
	{
		// No support for textured lines
		SetTexture(NULL);

		quad q((r - hotspot).rotate_to_quad(angle, r.topleft()));

		BeginBatch(batch_linestrip);
		CBatch_Draw* draw_op = reinterpret_cast<CBatch_Draw*>(batch.back());

		D3DCOLOR color = c.getD3DCOLOR();

		// 4 vertices and use 5th index to repeat first vertex closing the strip as a box
		AddVertex(color, q.tl, 0.0f, 0.0f);
		AddVertex(color, q.tr, 0.0f, 0.0f);
		AddVertex(color, q.br, 0.0f, 0.0f);
		AddVertex(color, q.bl, 0.0f, 0.0f);

		unsigned short index = draw_op->vertex_count;
		AddIndex(index);
		AddIndex(index + 1);
		AddIndex(index + 2);
		AddIndex(index + 3);
		AddIndex(index);

		draw_op->vertex_count += 4;
		draw_op->index_count += 5;
	}

	void CDX9Renderer::Fill(const rect& r, const color& c)
	{
		// Fill is just a quad with no texture
		SetTexture(NULL);
		Quad(r, c, NULL);
	}

	void CDX9Renderer::Fill(cr_float x, cr_float y, cr_float width, cr_float height, const color& c)
	{
		Fill(rect_xywh(x, y, width, height), c);
	}

	void CDX9Renderer::Fill(const rect& r, cr_float angle, point hotspot, const color& c)
	{
		quad q((r - hotspot).rotate_to_quad(angle, r.topleft()));

		SetTexture(NULL);
		Quad(q, c, NULL);
	}

	void SetVertices(color* vs, const color& c1, const color& c2, const color& c3, const color& c4)
	{
		vs[0] = c1;
		vs[1] = c2;
		vs[2] = c3;
		vs[3] = c4;
	}

	void CDX9Renderer::FillGradient(const rect& r, const color& c1, const color& c2, gradient_direction dir)
	{
		SetTexture(NULL);

		color vs[4];

		switch (dir) {
		case dir_up:
			SetVertices(vs, c2, c2, c1, c1);
			Quad(r, vs);
			break;
		case dir_down:
			SetVertices(vs, c1, c1, c2, c2);
			Quad(r, vs);
			break;
		case dir_left:
			SetVertices(vs, c2, c1, c2, c1);
			Quad(r, vs);
			break;
		case dir_right:
			SetVertices(vs, c1, c2, c1, c2);
			Quad(r, vs);
			break;
		}
	}

	void CDX9Renderer::FillGradient(cr_float x, cr_float y, cr_float width, cr_float height, const color& c1, const color& c2, gradient_direction dir)
	{
		FillGradient(rect_xywh(x, y, width, height), c1, c2, dir);
	}

	void CDX9Renderer::FillGradient(const rect& r, cr_float angle, point hotspot, const color& c1, const color& c2, gradient_direction dir)
	{
		quad q((r - hotspot).rotate_to_quad(angle, r.topleft()));

		SetTexture(NULL);

		color vs[4];

		switch (dir) {
		case dir_up:
			SetVertices(vs, c2, c2, c1, c1);
			Quad(r, vs);
			break;
		case dir_down:
			SetVertices(vs, c1, c1, c2, c2);
			Quad(r, vs);
			break;
		case dir_left:
			SetVertices(vs, c2, c1, c2, c1);
			Quad(r, vs);
			break;
		case dir_right:
			SetVertices(vs, c1, c2, c1, c2);
			Quad(r, vs);
			break;
		}
	}

	void CDX9Renderer::SmoothLine(cr_float x1, cr_float y1, cr_float x2, cr_float y2, cr_float width, const color &c)
	{
		SmoothLine(point(x1, y1), point(x2, y2), width, c);
	}

	void CDX9Renderer::SmoothLine(point p1, point p2, cr_float width, const color& c)
	{
		SmoothLine3D(point3d(p1.x, p1.y, 0), point3d(p2.x, p2.y, 0), width, c);
	}


	void CDX9Renderer::SmoothLine3D(point3d p1, point3d p2, cr_float width, const color& c)
	{
		FlushState();

		//width *= (state.transform.x_scale + state.transform.y_scale) / 2.0;

		// Ensure the top entry is a smoothline
		bool need_new_batch = false;

		if (batch.empty())
			need_new_batch = true;
		else if (batch.back()->op != batchop_smoothlines)
			need_new_batch = true;
		else {
			CBatch_SmoothLine* last = reinterpret_cast<CBatch_SmoothLine*>(batch.back());

			if (last->width != width || last->c != c)
				need_new_batch = true;
		}

		if (need_new_batch) {
			D3DXMATRIX transform;
			D3DXMatrixMultiply(&transform, &state.transform.current_world, &world.view_matrix);
			D3DXMatrixMultiply(&transform, &transform, &world.projection_matrix);
			batch.push_back(new (this) CBatch_SmoothLine(this, width, c, transform));

			if (smoothline_index == smoothlines.size() - 1)
				smoothlines.push_back(SmoothLineList());

			smoothline_index++;
		}

		// Now we have the right CBatch_SmoothLine object on the top of the batch - add the vertices to it
		CBatch_SmoothLine* last = reinterpret_cast<CBatch_SmoothLine*>(batch.back());

		SmoothLineList& lines = smoothlines[smoothline_index];

		lines.push_back(D3DXVECTOR3(p1.x, p1.y, p1.z));
		lines.push_back(D3DXVECTOR3(p2.x, p2.y, p2.z));

		last->count++;
	}

	void CBatch_SmoothLine::Do()
	{
		renderer->pD3DXLine->SetWidth(width);
		renderer->pD3DXLine->Begin();

		const SmoothLineList& current_lines = renderer->smoothlines[renderer->processed_smoothlines];
		renderer->processed_smoothlines++;

		D3DCOLOR d3dcol = c.getD3DCOLOR();

		for (int i = 0; i < count; i++)
			renderer->pD3DXLine->DrawTransform(&current_lines[i * 2], 2, &projection, d3dcol);

		renderer->pD3DXLine->End();
	}

	// Text rendering
	void CDX9Renderer::DrawTextCR(FontHandle fh, PCTSTR text, const rect &r, DWORD format, const color &c)
	{
		FlushState();

		CRASSERT(FontExists(fh));

		texts_to_render.push_back(text);

		batch.push_back(new (this) CBatch_DrawText(this, fh, r, c, format));
	}

	void CBatch_DrawText::Do()
	{
		renderer->pD3DXSprite->Begin(D3DXSPRITE_OBJECTSPACE | D3DXSPRITE_ALPHABLEND);

		// For floating point offsets
		D3DXMATRIX offset, scale;
		D3DXMatrixTranslation(&offset, r.left - floor(r.left), r.top - floor(r.top), 0.0);
		renderer->pD3DXSprite->SetTransform(&offset);

		RECT rc;
		SetRect(&rc, r.left, r.top, r.right, r.bottom);

		// Restore the sampler state
		//renderer->d3d9_device->SetSamplerState(0, CDX9Renderer::get_d3d9_equiv(ss_minfilter), CDX9Renderer::get_d3d9_equiv(renderer->state.current_samplerstates[ss_minfilter]));
		//renderer->d3d9_device->SetSamplerState(0, CDX9Renderer::get_d3d9_equiv(ss_magfilter), CDX9Renderer::get_d3d9_equiv(renderer->state.current_samplerstates[ss_magfilter]));

		fh->font->DrawText(renderer->pD3DXSprite, renderer->texts_to_render[renderer->text_index].c_str(), -1, &rc, format, c.getD3DCOLOR());

		renderer->text_index++;

		renderer->pD3DXSprite->End();
	}

	void CDX9Renderer::GetTextSize(FontHandle fh, PCTSTR text, rect& r)
	{
		RECT rc;
		SetRect(&rc, 0, 0, 0, 0);
		fh->font->DrawText(NULL, text, -1, &rc, DT_CALCRECT, 0xFF000000);
		r.right = rc.right;
		r.bottom = rc.bottom;
	}

	void CDX9Renderer::SetBackBuffer(LPDIRECT3DSURFACE9 s)
	{
		batch.push_back(new (this) CBatch_SetBackBuffer(this, s));
	}

	void CBatch_SetBackBuffer::Do()
	{
		if (s == NULL)
			renderer->back_buffer = renderer->display_surface;
		else
			renderer->back_buffer = s;

		renderer->back_buffer_was_changed = true;
	}

	// Batch extensions
	void CDX9Renderer::AddBatchExtension(CBatchBase* pCmd)
	{
		batch.push_back(pCmd);
	}

	// Debugging
#ifdef CR_DEBUG

	void CDX9Renderer::SaveNextBatchLogToFile(tstring filename)
	{
		debug_save_next_batch = true;
		debug_save_filename = filename;
		debug_batch_log.clear();
	}

	void CDX9Renderer::DoBatchLog()
	{
		// Use 2 decimal places float output
		debug_batch_log.precision(2);

		uint smoothline_debug_index = 0;
		uint text_debug_index = 0;

		BatchIterator i = batch.begin();
		BatchIterator end = batch.end();

		for ( ; i != end; ++i) {
			
			switch ((*i)->op) {
			case batchop_draw:
				{
					CBatch_Draw* cmd = reinterpret_cast<CBatch_Draw*>(*i);
					debug_batch_log << _T("DRAW ");

					switch (cmd->type) {
					case batch_null:
						debug_batch_log << _T("NULL ");
						break;
					case batch_points:
						debug_batch_log << _T("POINTS ");
						break;
					case batch_lines:
						debug_batch_log << _T("LINES ");
						break;
					case batch_linestrip:
						debug_batch_log << _T("LINE STRIP ");
						break;
					case batch_quads:
						debug_batch_log << _T("QUADS ");
						break;
					}

					debug_batch_log << _T("(") << cmd->vertex_count << _T(" vertices, ") << cmd->index_count << _T(" indices)");
					break;
				}
			case batchop_clearrendertarget:
				{
					CBatch_ClearRenderTarget* cmd = reinterpret_cast<CBatch_ClearRenderTarget*>(*i);
					debug_batch_log << _T("CLEAR ");

					if (cmd->clearAll)
						debug_batch_log << _T("all");
					else
						debug_batch_log << _T("region ") << (cmd->rect.x2 - cmd->rect.x1) << _T(" x ") << (cmd->rect.y2 - cmd->rect.y1);

					debug_batch_log << _T(", ");
					LogColor(color_from_D3DCOLOR<cr_float>(cmd->c));
					break;
				}
			case batchop_settexture:
				{
					CBatch_SetTexture* cmd = reinterpret_cast<CBatch_SetTexture*>(*i);
					debug_batch_log << _T("SET TEXTURE ");
					LogTexture(cmd->th);
					break;
				}
			case batchop_setrendertarget:
				{
					CBatch_SetRenderTarget* cmd = reinterpret_cast<CBatch_SetRenderTarget*>(*i);
					debug_batch_log << _T("SET RENDERTARGET ");

					if (cmd->th == NULL)
						debug_batch_log << _T("to backbuffer (") << (void*)cmd->renderer->back_buffer << _T(")");
					else {
						debug_batch_log << _T("to Texture");
						LogTexture(cmd->th);
					}

					if (cmd->update_3d_math)
						debug_batch_log << _T(" + 3D math");

					break;
				}
			case batchop_setrenderstate:
				{
					CBatch_SetRenderState* cmd = reinterpret_cast<CBatch_SetRenderState*>(*i);
					debug_batch_log << _T("SET RENDERSTATE ") << enum_name(cmd->rs) << _T(" TO ") << enum_name(cmd->rsv);
					break;
				}
			case batchop_setsamplerstate:
				{
					CBatch_SetSamplerState* cmd = reinterpret_cast<CBatch_SetSamplerState*>(*i);
					debug_batch_log << _T("SET SAMPLERSTATE ") << enum_name(cmd->ss) << _T(" TO ") << enum_name(cmd->ssv);
					break;
				}
			case batchop_settransform:
				{
					CBatch_SetTransform* cmd = reinterpret_cast<CBatch_SetTransform*>(*i);
					debug_batch_log << _T("SET TRANSFORM");
					break;
				}
			case batchop_smoothlines:
				{
					CBatch_SmoothLine* cmd = reinterpret_cast<CBatch_SmoothLine*>(*i);
					debug_batch_log << _T("SMOOTHLINES ");
					debug_batch_log << cmd->count;
					debug_batch_log << _T(" lines, ") << cmd->width << _T(" width, ");
					LogColor(cmd->c);
					break;
				}
			case batchop_text:
				{
					CBatch_DrawText* cmd = reinterpret_cast<CBatch_DrawText*>(*i);
					debug_batch_log << _T("TEXT \"");
					debug_batch_log << texts_to_render[text_debug_index++] << _T("\" from (");
					debug_batch_log << cmd->r.left << _T(",") << cmd->r.top << _T(") ");
					LogColor(cmd->c);
					break;
				}
#ifdef CDX9RENDERER_USE_BATCH_EXTENSIONS
			case batchop_fx_begin:
				{
					debug_batch_log << _T("FX BEGIN");
					break;
				}
			case batchop_fx_beginpass:
				{
					CBatchX_FX_BeginPass* cmd = reinterpret_cast<CBatchX_FX_BeginPass*>(*i);
					debug_batch_log << _T("FX BEGINPASS ") << cmd->pass;
					break;
				}
			case batchop_fx_endpass:
				{
					debug_batch_log << _T("FX ENDPASS");
					break;
				}
			case batchop_fx_end:
				{
					debug_batch_log << _T("FX END");
					break;
				}
#endif
			case batchop_setbackbuffer:
				{
					CBatch_SetBackBuffer* cmd = reinterpret_cast<CBatch_SetBackBuffer*>(*i);
					debug_batch_log << _T("SET BACK BUFFER ") << (void*)cmd->s;
					break;
				}
			case batchop_setpointsize:
				{
					CBatch_SetPointSize* cmd = reinterpret_cast<CBatch_SetPointSize*>(*i);
					debug_batch_log << _T("SET POINT SIZE to ") << cmd->pt_size;
					break;
				}
			case batchop_copysurfacetotexture:
				{
					CBatch_CopySurfaceToTexture* cmd = reinterpret_cast<CBatch_CopySurfaceToTexture*>(*i);
					debug_batch_log << _T("COPY SURFACE TO TEXTURE surface(");
					LogTexture(cmd->ms);
					debug_batch_log << _T(") to texture(");
					LogTexture(cmd->th);
					debug_batch_log << _T(")");
					break;
				}
			default:
				debug_batch_log << _T("??????");
				break;

			}//switch

			debug_batch_log << std::endl;

		}//for

		// End of batch: mark in log
		debug_batch_log << _T("****** END OF BATCH: ") << vertices_vector.size() << _T(" vertices and ") << indices_vector.size() << _T(" indices processed ******");
		debug_batch_log << std::endl << std::endl;

	}//()

	void CDX9Renderer::LogColor(const color& c)
	{
		debug_batch_log << _T("ARGB(") << c.a << _T(", ") << c.r << _T(", ") << c.g << _T(", ") << c.b << _T(")");
	}

	void CDX9Renderer::LogTexture(TextureHandle th)
	{
		if (th == NULL) {
			debug_batch_log << _T("<null texture>");
			return;
		}
	
		debug_batch_log << _T("(") << (void*)th << _T(", ") << th->image_width << _T(" x ") << th->image_height;

		if (th->rendertarget)
			debug_batch_log << _T(", rendertarget");

		if (th->is_multisampled)
			debug_batch_log << _T(" multisampled");

		debug_batch_log << _T(")");
	}

#endif//CR_DEBUG
}