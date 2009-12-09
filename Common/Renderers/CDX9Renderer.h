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

// Renderer types/definitions prepend "CR", standing for Construct Renderer.
// This class is designed to be ported to future DirectX versions and alternative
// engines such as OpenGL, using the common interface used by this reference
// DirectX 9 powered implementation.
#pragma once

// Include Construct Runtime-specific batching extensions
#define CDX9RENDERER_USE_BATCH_EXTENSIONS

// Include common renderer definitions
#include "RendererCommon.h"

// For state tracking
#include <bitset>

// D3D 9 includes
#include <d3d9.h>
#include <d3dx9.h>

class CRenderLayer;

namespace cr {

	// Config
	const uint allocation_block_size = 1024 * 1024;			// Batch allocator uses 1mb chunks
	const color transparent_color(1.0, 1.0, 0.0, 1.0);		// Default transparency is magenta
	const uint mipmap_levels = 0;							// Mipmap levels, 0 = full chain
	const DWORD mipmap_filter = D3DX_DEFAULT;				// Equivalent to D3DX_FILTER_BOX
	const uint d3d_max_vertices_size = 20000;				// About 5000 quads
	const uint d3d_max_indices_size = 30000;				// 6 indices for 4 vertices in a quad

	const uint font_factor = 1;

	// Vertex definition
	const DWORD vertex_format = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	struct vertex
	{
		float x;
		float y;
		float z;
		D3DCOLOR color;
		float u;
		float v;
	};

	// Texture class
	class d3d9texture : public texture {
	public:

		d3d9texture();

		// Pointer to D3D9 texture
		IDirect3DTexture9*	texture_ptr;		// Standard textures
		IDirect3DSurface9*	surface_ptr;		// Multisampled rendertargets

		// True = use surface_ptr, false = use texture_ptr
		bool is_multisampled;
	};

	// D3D9 typedefs
	typedef IDirect3D9* (WINAPI *LPDIRECT3DCREATE9)(uint SDKVersion);

	typedef std::vector<HWND>								HWNDList;
	typedef std::vector<HWND>::iterator						HWNDIterator;
	typedef std::vector<D3DPRESENT_PARAMETERS>				PresentParamsList;
	typedef std::vector<D3DPRESENT_PARAMETERS>::iterator	PresentParamsIterator;
	typedef std::vector<vertex>								VertexList;
	typedef std::vector<vertex>::iterator					VertexIterator;
	typedef std::vector<unsigned short>						IndexList;
	typedef std::vector<unsigned short>::iterator			IndexIterator;
	typedef std::set<d3d9texture*>							TextureList;
	typedef std::set<d3d9texture*>::iterator				TextureIterator;
	typedef std::set<d3d9texture*>::const_iterator			TextureConstIterator;
	typedef std::vector<char>								BatchAllocBlock;
	typedef std::vector<char>::iterator						BatchAllocBlockIterator;
	typedef std::list<BatchAllocBlock>						BatchMem;
	typedef std::list<BatchAllocBlock>::iterator			BatchMemIterator;

	typedef std::bitset<renderstate_count>					RenderStateBits;
	typedef std::bitset<samplerstate_count>					SamplerStateBits;
	typedef std::vector<D3DXVECTOR3>						SmoothLineList;
	typedef std::vector<D3DXVECTOR3>::iterator				SmoothLineIterator;
	typedef std::vector<SmoothLineList>						SmoothLineListList;
	typedef std::vector<SmoothLineList>::iterator			SmoothLineListIterator;
	typedef std::vector<tstring>							TStringList;
	typedef std::vector<tstring>::iterator					TStringIterator;

	typedef d3d9texture*									TextureHandle;

	// Font class
	class CDX9Font {
	public:
		D3DXFONT_DESC desc;
		ID3DXFont* font;
		class CDX9Renderer* renderer;

		uint reference_count;

		CDX9Font(class CDX9Renderer* _renderer);

		// Reference counting functions
		void IncrementReferences();
		void DecrementReferences();
		bool HasNoReferences() const;

		// For saving on font duplication
		bool SameFont(const D3DXFONT_DESC& other) const;
	};

	typedef CDX9Font*						FontHandle;
	typedef std::set<CDX9Font*>				FontList;
	typedef std::set<CDX9Font*>::iterator	FontIterator;

	// Swap effects
	enum swap_effect {
		swap_discard,
		swap_flip,
		swap_copy
	};

	// Setup data
	class d3d9_init_params
	{
	public:
		d3d9_init_params();
		void check_valid();

		HWNDList		hWnds;				// hWnd of windows to use (only one element needed for non-multi-monitor systems)
		HWND			hFocusWnd;			// Focus hwnd
		texture_format	backbuffer_format;
		BOOL			fullscreen;
		uint			backbuffer_width;
		uint			backbuffer_height;
		framerate_mode	fps_mode;
		uint			fps;				// If framerate_fixed selected, the FPS rate
		swap_effect		swap;
		multimonitor_mode multimonitor;		// Multimonitor setting

		bool			use_zbuffer;		// Enable Z-buffering for 3D
		uint			multisamples;		// 0 for off
	};
}//namespace cr

// Include batching definitions
#include "CDX9Renderer_Batch.h"

#ifdef CDX9RENDERER_USE_BATCH_EXTENSIONS
#include "CDX9Renderer_Batch_Extensions.h"
#endif //CDX9RENDERER_USE_BATCH_EXTENSIONS

namespace cr {

	// Renderer
	class CDX9Renderer {
	public:
		// Ctor/dtor
		CDX9Renderer();
		~CDX9Renderer();

		// Device control
		void			LoadD3D();								// Load DirectX DLLs
		void			CreateDevice(d3d9_init_params& params);	// Create device
		void			Prepare2D();							// Set render states & prepare for 2D rendering
		void			Reset();								// Reset device
		void			Close();								// Shut down device (called by destructor)

		void			SetDeviceLost(bool lost = true);
		//void			OnLostDevice();
		//void			OnResetDevice();
		const D3DCAPS9&	GetCaps() const;
		IDirect3DDevice9* GetDevice() const;					// Avoid using if you can, not portable!

		// Texture functions
		TextureHandle	CreateRenderTargetTexture(uint width, uint height, texture_format format = texture_format_a8r8g8b8, bool multisampled_target = false);
		void			ResizeRenderTargetTexture(TextureHandle th, uint new_width, uint new_height);
		TextureHandle	CreateTextureFromFile(PCTSTR filename, texture_format format = texture_format_a8r8g8b8, bool premultiply = false);
		TextureHandle	CreateTextureFromFileInMemory(LPCVOID pData, uint len, texture_format format = texture_format_a8r8g8b8, bool premultiply = false);
		TextureHandle	CreateTextureFromResource(HMODULE hModule, LPCTSTR resource, texture_format format = texture_format_a8r8g8b8, bool premultiply = false);

		void			AddTextureReference(TextureHandle t);
		bool			ReleaseTexture(TextureHandle t);	// Free reference
		void			DeallocateTexture(TextureHandle t);	// Immediately free

		void			ResetTextures();		// Free everything immediately
		bool			TextureExists(TextureHandle th) const;
		void			ReplaceTexture(TextureHandle oldTex, TextureHandle newTex);

		uint			GetTotalTextureMemoryUsage() const;
		uint			GetRenderTargetMemoryUsage() const;

		// Multimonitor support
		uint			GetMultiHeadCount() const;
		multimonitor_mode GetMultiMonitorMode() const;

		// Batching control
		void			EndBatch();

		// State control
		void			SetRenderState(renderstate rs, renderstate_value rsv);
		void			SetSamplerState(samplerstate ss, samplerstate_value ssv);
		renderstate_value			GetRenderState(renderstate rs) const;
		samplerstate_value			GetSamplerState(samplerstate ss) const;

		void			SetPointSampling();
		void			SetLinearSampling();

		void			SetPointSize(cr_float _point_size);
		cr_float		GetPointSize() const;

		void			SetZBufferEnabled(bool enable);
		bool			GetZBufferEnabled() const;

		void			SetAdditiveBlending();
		void			SetAlphaBlending();

		// World transform control
		void			SetXScale(cr_float xf);
		cr_float		GetXScale() const;
		void			SetYScale(cr_float yf);
		cr_float		GetYScale() const;
		void			SetScale(point xy);
		point			GetScale() const;
		void			SetAngle(cr_float rads);
		cr_float		GetAngle() const;

		void			UpdateTransform(bool enable_transform = true);
		void			SetZOffset(cr_float z);
		cr_float		GetZOffset() const;

		point			Project(point3d pt, bool transform = true) const;
		void			Project(const point3d* pts, point3d* outs, uint count, bool transform = true) const;
		point3d			Unproject(point3d pt, bool transform = true) const;
		void			Unproject(const point3d* pts, point3d* outs, uint count, bool transform = true) const;

		// Drawing
		void			BeginScene();
		void			EndScene();
		HRESULT			Present(RECT* pRec = NULL, HWND hWndOverride = NULL);
		uint			GetBatchesPerPresent() const;

		void			ClearRenderTarget(const color& c = transparent_black);
		void			ClearRenderTarget(const D3DRECT& rect, const color& c = transparent_black);

		// Transfer multisample surface to texture
		void			CopyMultisampleSurfaceToTexture(TextureHandle ms, TextureHandle th);
		void			CopyMultisampleSurfaceToTexture(TextureHandle ms, TextureHandle th, const RECT& area);

		void			SetTexture(TextureHandle th);
		void			SetRenderTarget(TextureHandle th);
		TextureHandle	GetTexture() const;
		TextureHandle	GetRenderTarget() const;

		void			SetBackBuffer(LPDIRECT3DSURFACE9 s);

		void			SetSkew(cr_float xk, cr_float yk);
		void			Quad(const point& p, cr_float angle = 0.0, point hotspot = origin, const color& filter = opaque_white, const rect* _uv = NULL);
		void			Quad(cr_float x, cr_float y, cr_float angle = 0.0, point hotspot = origin, const color& filter = opaque_white, const rect* _uv = NULL);
		void			Quad(cr_float x, cr_float y, cr_float width, cr_float height, cr_float angle = 0.0, point hotspot = origin, const color& filter = opaque_white, const rect* _uv = NULL);
		void			Quad(const rect& r, cr_float angle = 0.0, point hotspot = origin, const color& filter = opaque_white, const rect* _uv = NULL);
		void			Quad(const quad& q, const color& filter = opaque_white, const rect* _uv = NULL);
		void			Quad(const quad& q, const color* vertexcolors, const rect* _uv = NULL);

		void			Point(cr_float x, cr_float y, const color& c);
		void			Point(point p, const color& c);

		// Aliased lines
		void			Line(cr_float x1, cr_float y1, cr_float x2, cr_float y2, const color& c);
		void			Line(cr_float x1, cr_float y1, cr_float x2, cr_float y2, const color& c1, const color& c2);
		void			Line(point p1, point p2, const color& c);
		void			Line(point p1, point p2, const color& c1, const color& c2);

		// Antialiased ID3DXLines
		void			SmoothLine(cr_float x1, cr_float y1, cr_float x2, cr_float y2, cr_float width, const color& c);
		void			SmoothLine(point p1, point p2, cr_float width, const color& c);
		void			SmoothLine3D(point3d p1, point3d p2, cr_float width, const color& c);

		void			Box(cr_float x, cr_float y, cr_float width, cr_float height, const color& c);
		void			Box(const rect& r, const color& c);
		void			Box(const rect& r, cr_float angle, point hotspot, const color& c);

		void			Fill(const rect& r, const color& c);
		void			Fill(cr_float x, cr_float y, cr_float width, cr_float height, const color& c);
		void			Fill(const rect& r, cr_float angle, point hotspot, const color& c);

		void			FillGradient(const rect& r, const color& c1, const color& c2, gradient_direction dir);
		void			FillGradient(cr_float x, cr_float y, cr_float width, cr_float height, const color& c1, const color& c2, gradient_direction dir);
		void			FillGradient(const rect& r, cr_float angle, point hotspot, const color& c1, const color& c2, gradient_direction dir);

		// Text rendering
		FontHandle		CreateCRFont(PCTSTR font_face, INT height, uint width = 0, uint weight = FW_NORMAL, BOOL italic = false, DWORD quality = ANTIALIASED_QUALITY, BYTE charset = DEFAULT_CHARSET, BYTE pitch_and_family = DEFAULT_PITCH|FF_DONTCARE);
		bool			FontExists(FontHandle fh) const;
		void			DrawTextCR(FontHandle fh, PCTSTR text, const rect& r, DWORD format, const color& c);
		void			ReleaseCRFont(FontHandle fh);

		// Batch extensions
		void			AddBatchExtension(CBatchBase* pCmd);

		//////////////
		// Error class
		class error {
		public:
			error(PCTSTR _msg, HRESULT _hr);

			PCTSTR	GetMsg() const;
			HRESULT GetCode() const;
		protected:
			tstring msg;	// error message
			HRESULT hr;		// error code (may be GetLastError())
		};

		// Friend classes
		friend CBatchBase;
		friend CBatch_Draw;
		friend CBatch_ClearRenderTarget;
		friend CBatch_SetTexture;
		friend CBatch_SetRenderTarget;
		friend CBatch_SetRenderState;
		friend CBatch_SetSamplerState;
		friend CBatch_SetTransform;
		friend CBatch_SmoothLine;
		friend CBatch_DrawText;
		friend CBatch_SetBackBuffer;
		friend CBatch_SetPointSize;
		friend CBatch_CopySurfaceToTexture;

#ifdef CDX9RENDERER_USE_BATCH_EXTENSIONS
		friend CBatchX_FX_Begin;
		friend CBatchX_FX_BeginPass;
		friend CBatchX_FX_EndPass;
		friend CBatchX_FX_End;
#endif //CDX9RENDERER_USE_BATCH_EXTENSIONS

		friend CRenderLayer;

		// Hacks
		IDirect3D9*				GetD3D9() const { return d3d9; }
		D3DPRESENT_PARAMETERS&	GetFirstPresent() { return d3d9_presents.front(); }

	protected:
		/////////////
		// D3D9 stuff
		HMODULE					d3d9_dll;		// Handle to d3d9.dll
		IDirect3D9*				d3d9;			// d3d interface
		IDirect3DDevice9*		d3d9_device;	// d3d device
		D3DCAPS9				d3d9_caps;		// d3d capabilities
		PresentParamsList		d3d9_presents;	// d3d presentation parameters array (for each monitor)
		IDirect3DVertexBuffer9* batch_buffer;	// Buffer for batch operations
		IDirect3DIndexBuffer9*  index_buffer;	// For batching quads

		ID3DXSprite*			pD3DXSprite;	// For text rendering
		ID3DXLine*				pD3DXLine;		// For smooth lines

		// Surfaces
		IDirect3DSurface9*		display_surface;
		IDirect3DSurface9*		back_buffer;				// Overridable from SetBackBuffer
		bool					back_buffer_was_changed;	// For overriding efficiency check in SetRenderTarget()

		bool					use_zbuffer;

		// Vectors for copying to buffer when filled
		VertexList				vertices_vector;
		IndexList				indices_vector;

		// Indices in to each of the above vectors
		uint					processed_vertices;
		uint					processed_indices;

		// Smoothlines
		SmoothLineListList		smoothlines;
		int						smoothline_index;
		uint					processed_smoothlines;

		// Create vertex, index buffers etc - called by CreateDevice(), Reset()
		void					CreateDeviceResources();

		// Texturing members
		TextureList				textures;		// List of all textures

		// To premultiply a texture on CPU
		void					PremultiplyTextureCPU(TextureHandle th);

		FontList				fonts;			// List of all fonts
		TStringList				texts_to_render;
		uint					text_index;

		// Batching control
		Batch					batch;
		void					ResetBatch();
		uint					batches_per_present;
		void*					BatchAllocate(uint num_bytes);

	public:
		void					BeginBatch(batch_type type);
		void					AddVertex(D3DCOLOR c, float x, float y, float u, float v);
		void					AddVertex(D3DCOLOR c, point xy, float u, float v);
		void					AddVertex(D3DCOLOR c, point xy, point uv);
		void					AddVertex(D3DCOLOR c, point3d xyz, point uv);
		void					AddIndex(unsigned short i);
		CBatchBase*				GetTopBatchItem() const;
	protected:

		BatchMem				batch_memory;
		BatchAllocBlockIterator	current_alloc_pos;
		BatchMemIterator		current_batch_block;

		// State control
		void					FlushState();		// Add renderstates etc. to the batch

		struct State {

			RenderStateBits renderstate_bits;		// for changed states
			renderstate_value current_renderstates[renderstate_count];
			renderstate_value pending_renderstates[renderstate_count];

			SamplerStateBits samplerstate_bits;
			samplerstate_value current_samplerstates[samplerstate_count];
			samplerstate_value pending_samplerstates[samplerstate_count];

			struct Transform {
				cr_float x_scale;
				cr_float y_scale;
				cr_float angle;	// radians

				cr_float old_x_scale;
				cr_float old_y_scale;
				cr_float old_angle;

				// Projection matrix and its pending version
				D3DXMATRIX current_world;
				D3DXMATRIX pending_world;
				bool transform_is_pending;

			} transform;

			cr_float x_skew;
			cr_float y_skew;

			cr_float point_size;

			cr_float z_offset;
			bool zbuffer_enabled;

			uint last_rendertarget_width;
			uint last_rendertarget_height;

		} state;

		// 3D stuff
		struct World {
			D3DXVECTOR3 origin;
			D3DXVECTOR3 eye_vector;
			D3DXVECTOR3 lookat_vector;
			D3DXVECTOR3 up_vector;

			D3DXMATRIX view_matrix;		// D3DXMatrixLookAtLH
			D3DXMATRIX world_matrix;
			D3DXMATRIX projection_matrix;

			D3DVIEWPORT9 viewport;

			point scale;				// World units per pixel
		} world;

		void	Update3DMaths(cr_float rendertarget_width, cr_float rendertarget_height);

		// CDX9Renderer non-public members
		HWNDList				hWnds;
		multimonitor_mode		multimonitor;	// multimonitor mode
		void					UpdatePresentsArray();	// For multihead, copy d3dPresents[0] through array

		uint					backbuffer_width, backbuffer_height;
		cr_float				backbuffer_widthf, backbuffer_heightf;
		texture_format			backbuffer_format;

		TextureHandle			current_rendertarget;
		TextureHandle			current_texture;

		bool					device_is_lost;		// True when device in lost state and waiting for reset
		bool					device_got_reset;	// True immediately after a device got reset

		uint					multisamples;
		DWORD					multisample_quality;

		D3DXMATRIX identity;	// saved for efficiency

		// HRESULT for any function to use
		HRESULT					hr;

		// Debugging features
#ifdef CR_DEBUG
	public:
		void					SaveNextBatchLogToFile(tstring filename);
	protected:
		bool					debug_save_next_batch;
		tstring					debug_save_filename;
		tstringstream			debug_batch_log;
		
		void					DoBatchLog();
		void					LogColor(const color& c);
		void					LogTexture(TextureHandle th);

#endif //CR_DEBUG

		// enum conversions
		static D3DSWAPEFFECT			get_d3d9_equiv(swap_effect se);
		static D3DFORMAT				get_d3d9_equiv(texture_format tf);
		static D3DRENDERSTATETYPE		get_d3d9_equiv(renderstate rs);
		static uint						get_d3d9_equiv(renderstate rs, renderstate_value rsv);
		static D3DSAMPLERSTATETYPE		get_d3d9_equiv(samplerstate ss);
		static uint						get_d3d9_equiv(samplerstate_value ssv);

	public:
		static renderstate_value		d3d9_to_cr_rsv(renderstate rs, DWORD value);
		static samplerstate_value		d3d9_to_cr_ssv(samplerstate ss, DWORD value);

		//UNHACK - should be private
		static D3DMULTISAMPLE_TYPE		get_d3d9_multisample(uint _multisamples);
	};

	typedef CDX9Renderer::error CDX9Error;

}