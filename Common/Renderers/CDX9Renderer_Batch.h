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
#pragma once

namespace cr {

	// Forward declaration of CDX9Renderer
	class CDX9Renderer;

	// Batch types
	enum batch_type {
		batch_null,
		batch_points,
		batch_lines,
		batch_linestrip,
		batch_quads
	};

	// Fast RTTI for CBatchBase
	enum batch_operation {
		batchop_draw,
		batchop_clearrendertarget,
		batchop_settexture,
		batchop_setrendertarget,
		batchop_setrenderstate,
		batchop_setsamplerstate,
		batchop_settransform,
		batchop_smoothlines,		// ID3DXLines
		batchop_text,				// DrawTextCR()
		batchop_setbackbuffer,
		batchop_setpointsize,
		batchop_copysurfacetotexture,	// CopyMultisampleSurfaceToTexture
		batchop_fx_begin,			// CDX9Renderer_Batch_Extensions
		batchop_fx_beginpass,
		batchop_fx_endpass,
		batchop_fx_end,
	};

	// Batch base class
	class CBatchBase {
	public:
		// ctor/dtor
		CBatchBase(CDX9Renderer* _renderer, batch_operation _op);
		~CBatchBase() {}		// CBatchBase and derivatives are not allowed destructors - they are not called!

		CDX9Renderer* renderer;
		batch_operation op;

		// Do function and allocator
		virtual void Do() = 0;
		void* operator new(uint num_bytes, CDX9Renderer* _renderer);

		// Silence compiler warnings about no operator delete definition; CDX9Renderer handles all allocation
		void operator delete(void*, CDX9Renderer* _renderer) {}
		void operator delete(void*) {}
	};

	typedef std::vector<cr::CBatchBase*>						Batch;
	typedef std::vector<cr::CBatchBase*>::iterator				BatchIterator;

	class CBatch_Draw : public CBatchBase {
	public:
		CBatch_Draw(CDX9Renderer* _renderer, batch_type _type)
			: CBatchBase(_renderer, batchop_draw), type(_type), vertex_count(0), index_count(0) {}

		void Do();

		// Number of vertices & indices that this operation consumes
		batch_type type;
		uint vertex_count;
		uint index_count;
	};

	// Clear render target batch
	class CBatch_ClearRenderTarget : public CBatchBase {
	public:
		CBatch_ClearRenderTarget(CDX9Renderer* _renderer, const color& _c)
			: CBatchBase(_renderer, batchop_clearrendertarget),
			  c(_c.getD3DCOLOR()),
			  clearAll(true)
		{}

		CBatch_ClearRenderTarget(CDX9Renderer* _renderer, const color& _c, const D3DRECT& _rect)
			: CBatchBase(_renderer, batchop_clearrendertarget),
			  c(_c.getD3DCOLOR()),
			  rect(_rect),
			  clearAll(false)
		{}

		void Do();

		D3DCOLOR c;
		D3DRECT rect;
		bool clearAll;

	};

	class CBatch_SetTexture : public CBatchBase {
	public:
		CBatch_SetTexture(CDX9Renderer* _renderer, TextureHandle _th)
			: CBatchBase(_renderer, batchop_settexture),
			  th(_th)
		{}

		void Do();
		TextureHandle th;
	};

	class CBatch_SetRenderTarget : public CBatchBase {
	public:
		CBatch_SetRenderTarget(CDX9Renderer* _renderer, TextureHandle _th, bool _update_3d_math)
			: CBatchBase(_renderer, batchop_setrendertarget),
			  th(_th),
			  update_3d_math(_update_3d_math)
		{}

		void Do();
		TextureHandle th;
		bool update_3d_math;
	};

	class CBatch_SetRenderState : public CBatchBase {
	public:
		CBatch_SetRenderState(CDX9Renderer* _renderer, renderstate _rs, renderstate_value _rsv)
			: CBatchBase(_renderer, batchop_setrenderstate),
			  rs(_rs),
			  rsv(_rsv)
		{}

		void Do();
		renderstate rs;
		renderstate_value rsv;
	};

	class CBatch_SetPointSize : public CBatchBase {
	public:
		CBatch_SetPointSize(CDX9Renderer* _renderer, float _pt_size)
			: CBatchBase(_renderer, batchop_setpointsize),
			  pt_size(_pt_size)
		{}

		void Do();
		float pt_size;
	};

	class CBatch_SetSamplerState : public CBatchBase {
	public:
		CBatch_SetSamplerState(CDX9Renderer* _renderer, samplerstate _ss, samplerstate_value _ssv)
			: CBatchBase(_renderer, batchop_setsamplerstate),
			  ss(_ss),
			  ssv(_ssv)
		{}

		void Do();
		samplerstate ss;
		samplerstate_value ssv;
	};

	class CBatch_SetTransform : public CBatchBase {
	public:
		CBatch_SetTransform(CDX9Renderer* _renderer, D3DTRANSFORMSTATETYPE _transform, const D3DXMATRIX& _matrix)
			: CBatchBase(_renderer, batchop_settransform),
			  transform(_transform),
			  matrix(_matrix)
		{}

		void Do();
		D3DTRANSFORMSTATETYPE transform;
		D3DXMATRIX matrix;
	};

	// Smooth line to draw
	struct SmoothLine {
		point p1;
		point p2;
		color c;
		cr_float width;
	};

	class CBatch_SmoothLine : public CBatchBase {
	public:
		CBatch_SmoothLine(CDX9Renderer* _renderer, cr_float _width, const color& _c, const D3DXMATRIX& _projection)
			: CBatchBase(_renderer, batchop_smoothlines),
			  width(_width),
			  c(_c),
			  projection(_projection),
			  count(0)
		{}

		cr_float width;
		color c;
		uint count;
		D3DXMATRIX projection;

		void Do();
	};

	class CBatch_DrawText : public CBatchBase {
	public:
		CBatch_DrawText(CDX9Renderer* _renderer, FontHandle _fh, const rect& _r, const color& _c, DWORD _format)
			: CBatchBase(_renderer, batchop_text),
			  fh(_fh),
			  r(_r),
			  c(_c),
			  format(_format)
		{}

		FontHandle fh;
		rect r;
		color c;
		DWORD format;

		void Do();
	};

	class CBatch_SetBackBuffer : public CBatchBase {
	public:
		CBatch_SetBackBuffer(CDX9Renderer* _renderer, IDirect3DSurface9* _s)
			: CBatchBase(_renderer, batchop_setbackbuffer),
			  s(_s)
		{}

		IDirect3DSurface9* s;

		void Do();
	};

	class CBatch_CopySurfaceToTexture : public CBatchBase {
	public:
		CBatch_CopySurfaceToTexture(CDX9Renderer* _renderer, TextureHandle _ms, TextureHandle _th)
			: CBatchBase(_renderer, batchop_copysurfacetotexture),
			  ms(_ms),
			  th(_th),
			  all(true)
		{}

		CBatch_CopySurfaceToTexture(CDX9Renderer* _renderer, TextureHandle _ms, TextureHandle _th, const RECT& _area)
			: CBatchBase(_renderer, batchop_copysurfacetotexture),
			  ms(_ms),
			  th(_th),
			  all(false),
			  area(_area)
		{}

		bool all;
		RECT area;
		TextureHandle ms;
		TextureHandle th;

		void Do();
	};
}