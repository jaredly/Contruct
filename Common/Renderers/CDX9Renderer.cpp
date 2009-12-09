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

namespace cr {

	// Texture ctor
	d3d9texture::d3d9texture()
		: texture(),
		  texture_ptr(NULL),
		  surface_ptr(NULL),
		  is_multisampled(false)
	{}

	///////////////////////////////////////
	// Error class
	CDX9Renderer::error::error(PCTSTR _msg, HRESULT _hr)
		: msg(_msg),
		  hr(_hr)
	{}

	PCTSTR CDX9Renderer::error::GetMsg() const
	{
		return msg.c_str();
	}

	HRESULT CDX9Renderer::error::GetCode() const
	{
		return hr;
	}

	///////////////////////////////////////
	// Font class
	CDX9Font::CDX9Font(CDX9Renderer* _renderer)
		: renderer(_renderer),
		  font(NULL),
		  reference_count(0)
	{}

	// Reference counting functions
	void CDX9Font::IncrementReferences()
	{
		reference_count++;
	}

	void CDX9Font::DecrementReferences()
	{
		CRASSERT(reference_count > 0);
		reference_count--;
	}

	bool CDX9Font::HasNoReferences() const
	{
		return reference_count == 0;
	}

	bool CDX9Font::SameFont(const D3DXFONT_DESC& other) const
	{
		return (desc.Height == other.Height)
			&& (desc.Width == other.Width)
			&& (desc.Weight == other.Weight)
			&& (desc.Italic == other.Italic)
			&& (desc.Quality == other.Quality)
			&& (tstring(desc.FaceName) == tstring(other.FaceName));
	}

	///////////////////////////////////////
	// d3d9_init_params defaults
	d3d9_init_params::d3d9_init_params()
		: hFocusWnd(NULL),
		  backbuffer_format(texture_format_a8r8g8b8),
		  fullscreen(false),
		  backbuffer_width(640),
		  backbuffer_height(480),
		  fps_mode(framerate_vsynced),
		  fps(0),
		  swap(swap_discard),
		  multimonitor(multimonitor_singlescreen),
		  use_zbuffer(true),
		  multisamples(0)	// off
	{}

	void d3d9_init_params::check_valid()
	{
		CRASSERT(!hWnds.empty());
		CRASSERT(hFocusWnd != NULL);
		CRASSERT(backbuffer_width < 32768);
		CRASSERT(backbuffer_height < 32768);
	}

	///////////////////////////////////////
	// Conversion
	D3DSWAPEFFECT CDX9Renderer::get_d3d9_equiv(swap_effect se)
	{
		switch (se) {
		case swap_discard:
			return D3DSWAPEFFECT_DISCARD;
		case swap_flip:
			return D3DSWAPEFFECT_FLIP;
		case swap_copy:
			return D3DSWAPEFFECT_COPY;
		default:
			__assume(0);
		}
	}

	D3DFORMAT CDX9Renderer::get_d3d9_equiv(texture_format tf)
	{
		switch (tf) {
		case texture_format_null:
			throw error(_T("Texture format null was used"), -1);
		case texture_format_a8r8g8b8:
			return D3DFMT_A8R8G8B8;
		case texture_format_r8g8b8:
			return D3DFMT_R8G8B8;
		case texture_format_x8r8g8b8:
			return D3DFMT_X8R8G8B8;
		case texture_format_a16b16g16r16f:
			return D3DFMT_A16B16G16R16F;
		case texture_format_a32b32g32r32f:
			return D3DFMT_A32B32G32R32F;
		default:
			__assume(0);
		}
	}

	D3DRENDERSTATETYPE CDX9Renderer::get_d3d9_equiv(renderstate rs)
	{
		switch (rs) {
		case rs_null:
			throw error(_T("Render state null was used"), -1);
		case rs_srcblend:
			return D3DRS_SRCBLEND;
		case rs_destblend:
			return D3DRS_DESTBLEND;
		case rs_zbuffer_enabled:
			return D3DRS_ZENABLE;
		case rs_pointsprite_enabled:
			return D3DRS_POINTSPRITEENABLE;
		case rs_pointsize:
			return D3DRS_POINTSIZE;	// Use SetPointSize()/GetPointSize() instead of this renderstate
		case rs_blendop:
			return D3DRS_BLENDOP;
		case rs_zwrite_enabled:
			return D3DRS_ZWRITEENABLE;
		default:
			__assume(0);			
		}
	}

	uint CDX9Renderer::get_d3d9_equiv(renderstate rs, renderstate_value rsv)
	{
		switch (rsv) {
		case rsv_null:
			throw error(_T("Render state value null was used"), -1);

		case rsv_blend_zero:
			return D3DBLEND_ZERO;
		case rsv_blend_one:
			return D3DBLEND_ONE;
		case rsv_blend_srccolor:
			return D3DBLEND_SRCCOLOR;
		case rsv_blend_invsrccolor:
			return D3DBLEND_INVSRCCOLOR;
		case rsv_blend_srcalpha:
			return D3DBLEND_SRCALPHA;
		case rsv_blend_invsrcalpha:
			return D3DBLEND_INVSRCALPHA;
		case rsv_blend_destalpha:
			return D3DBLEND_DESTALPHA;
		case rsv_blend_invdestalpha:
			return D3DBLEND_INVDESTALPHA;
		case rsv_blendop_add:
			return D3DBLENDOP_ADD;
		case rsv_blendop_subtract:
			return D3DBLENDOP_SUBTRACT;
		case rsv_blendop_revsubtract:
			return D3DBLENDOP_REVSUBTRACT;
		case rsv_blendop_min:
			return D3DBLENDOP_MIN;
		case rsv_blendop_max:
			return D3DBLENDOP_MAX;
		case rsv_enabled:
			switch (rs) {
			case rs_zbuffer_enabled:
				return D3DZB_TRUE;
			case rs_pointsprite_enabled:
			case rs_zwrite_enabled:
				return TRUE;
			case rs_pointsize:
				return 0;		// unused, just a marker of changed state
			default:
				throw error(_T("Unknown renderstate value"), E_FAIL);
			}
		case rsv_disabled:
			switch (rs) {
			case rs_zbuffer_enabled:
				return D3DZB_FALSE;
			case rs_pointsprite_enabled:
			case rs_zwrite_enabled:
				return FALSE;
			default:
				throw error(_T("Unknown renderstate value"), E_FAIL);
			}
		default:
			__assume(0);
		}
	}

	renderstate_value CDX9Renderer::d3d9_to_cr_rsv(renderstate rs, DWORD value)
	{
		switch (rs) {
		case rs_zbuffer_enabled:
			return (value == D3DZB_TRUE ? rsv_enabled : rsv_disabled);
		case rs_pointsprite_enabled:
		case rs_zwrite_enabled:
			return (value == TRUE ? rsv_enabled : rsv_disabled);
		case rs_pointsize:
			return rsv_null;
		case rs_blendop:
			switch (value) {
			case D3DBLENDOP_ADD:
				return rsv_blendop_add;
			case D3DBLENDOP_SUBTRACT:
				return rsv_blendop_subtract;
			case D3DBLENDOP_REVSUBTRACT:
				return rsv_blendop_revsubtract;
			case D3DBLENDOP_MIN:
				return rsv_blendop_min;
			case D3DBLENDOP_MAX:
				return rsv_blendop_max;
			default:
				throw error(_T("Unrecognised render state value"), rs);
			}
		default:
			switch (value) {
			case D3DBLEND_ZERO:
				return rsv_blend_zero;
			case D3DBLEND_ONE:
				return rsv_blend_one;
			case D3DBLEND_SRCCOLOR:
				return rsv_blend_srccolor;
			case D3DBLEND_INVSRCCOLOR:
				return rsv_blend_invsrccolor;
			case D3DBLEND_SRCALPHA:
				return rsv_blend_srcalpha;
			case D3DBLEND_INVSRCALPHA:
				return rsv_blend_invsrcalpha;
			case D3DBLEND_DESTALPHA:
				return rsv_blend_destalpha;
			case D3DBLEND_INVDESTALPHA:
				return rsv_blend_invdestalpha;
			default:
				throw error(_T("Unrecognised render state value"), rs);
			}
		}
	}

	D3DSAMPLERSTATETYPE CDX9Renderer::get_d3d9_equiv(samplerstate ss)
	{
		switch (ss) {
		case ss_null:
			throw error(_T("Sampler state null was used"), -1);
		case ss_addressu:
			return D3DSAMP_ADDRESSU;
		case ss_addressv:
			return D3DSAMP_ADDRESSV;
		case ss_minfilter:
			return D3DSAMP_MINFILTER;
		case ss_magfilter:
			return D3DSAMP_MAGFILTER;
		default:
			__assume(0);
		}
	}

	uint CDX9Renderer::get_d3d9_equiv(samplerstate_value ssv)
	{
		switch (ssv) {
		case ssv_null:
			throw error(_T("Sampler state value null was used"), -1);
		case ssv_wrap:
			return D3DTADDRESS_WRAP;
		case ssv_mirror:
			return D3DTADDRESS_MIRROR;
		case ssv_clamp:
			return D3DTADDRESS_CLAMP;
		case ssv_border:
			return D3DTADDRESS_BORDER;
		case ssv_mirroronce:
			return D3DTADDRESS_MIRRORONCE;
		case ssv_point:
			return D3DTEXF_POINT;
		case ssv_linear:
			return D3DTEXF_LINEAR;
		default:
			__assume(0);
		}
	}

	D3DMULTISAMPLE_TYPE CDX9Renderer::get_d3d9_multisample(uint _multisamples)
	{
		switch (_multisamples) {
		case 0:			return D3DMULTISAMPLE_NONE;
		case 1:			return D3DMULTISAMPLE_NONE;			// 1 is same as off - one sample per pixel
		case 2:			return D3DMULTISAMPLE_2_SAMPLES;
		case 3:			return D3DMULTISAMPLE_3_SAMPLES;
		case 4:			return D3DMULTISAMPLE_4_SAMPLES;
		case 5:			return D3DMULTISAMPLE_5_SAMPLES;
		case 6:			return D3DMULTISAMPLE_6_SAMPLES;
		case 7:			return D3DMULTISAMPLE_7_SAMPLES;
		case 8:			return D3DMULTISAMPLE_8_SAMPLES;
		case 9:			return D3DMULTISAMPLE_9_SAMPLES;
		case 10:		return D3DMULTISAMPLE_10_SAMPLES;
		case 11:		return D3DMULTISAMPLE_11_SAMPLES;
		case 12:		return D3DMULTISAMPLE_12_SAMPLES;
		case 13:		return D3DMULTISAMPLE_13_SAMPLES;
		case 14:		return D3DMULTISAMPLE_14_SAMPLES;
		case 15:		return D3DMULTISAMPLE_15_SAMPLES;
		case 16:		return D3DMULTISAMPLE_16_SAMPLES;
		default:
			throw error(_T("Invalid multisampling parameter - must be 0-16"), E_FAIL);
		}
	}

	samplerstate_value CDX9Renderer::d3d9_to_cr_ssv(samplerstate ss, DWORD value)
	{ 
		switch (ss) {
			// Texture sampler values
			case ss_addressu:
			case ss_addressv:

				switch (value) {
				case D3DTADDRESS_WRAP:
					return ssv_wrap;
				case D3DTADDRESS_MIRROR:
					return ssv_mirror;
				case D3DTADDRESS_CLAMP:
					return ssv_clamp;
				case D3DTADDRESS_BORDER:
					return ssv_border;
				case D3DTADDRESS_MIRRORONCE:
					return ssv_mirroronce;
				default:
					throw error(_T("Unrecognised sampler state value"), ss);
				}

			// Linear/point sampling values
			case ss_minfilter:
			case ss_magfilter:

				switch (value) {
				case D3DTEXF_POINT:
					return ssv_point;
				case D3DTEXF_LINEAR:
					return ssv_linear;
				default:
					throw error(_T("Unrecognised sampler state value"), ss);
				}

			default:
				throw error(_T("Unrecognised sampler state"), ss);

		}
	}

	///////////////////////////////////////
	// CDX9Renderer implementation
	CDX9Renderer::CDX9Renderer()
		: d3d9_dll(NULL),
		  d3d9(NULL),
		  d3d9_device(NULL),
		  multimonitor(multimonitor_singlescreen),
		  hr(0),
		  batch_buffer(NULL),
		  index_buffer(NULL),
		  processed_vertices(0),
		  processed_indices(0),
		  pD3DXSprite(NULL),
		  pD3DXLine(NULL),
		  back_buffer(NULL),
		  display_surface(NULL),
		  current_rendertarget(NULL),
		  current_texture(NULL),
		  device_is_lost(false),
		  back_buffer_was_changed(false),
		  smoothline_index(-1),
		  processed_smoothlines(0),
		  text_index(0),
		  batches_per_present(0),
		  use_zbuffer(false),
		  multisamples(0),
		  multisample_quality(0),
		  device_got_reset(false)

#ifdef CR_DEBUG
		,
		debug_save_next_batch(false)
#endif //CR_DEBUG

	{
		// Zero out caps struct
		ZeroMemory(&d3d9_caps, sizeof(d3d9_caps));

		// Default transform
		state.transform.x_scale = 1.0;
		state.transform.y_scale = 1.0;
		state.transform.angle = 0.0;

		state.transform.old_x_scale = 1.0;
		state.transform.old_y_scale = 1.0;
		state.transform.old_angle = 0.0;
		
		// Reserve vertices and indices memory
		vertices_vector.reserve(d3d_max_vertices_size);
		indices_vector.reserve(d3d_max_indices_size);

		// Add a first batch block
		batch_memory.push_back(BatchAllocBlock());
		batch_memory.back().resize(allocation_block_size);

		current_batch_block = batch_memory.begin();
		current_alloc_pos = current_batch_block->begin();

		// Set null states
		for (int i = 0; i < renderstate_count; i++) {
			state.current_renderstates[i] = rsv_null;
			state.pending_renderstates[i] = rsv_null;
		}
		for (int i = 0; i < samplerstate_count; i++) {
			state.current_samplerstates[i] = ssv_null;
			state.pending_samplerstates[i] = ssv_null;
		}

		state.renderstate_bits.reset();
		state.samplerstate_bits.reset();

		D3DXMatrixIdentity(&identity);
		state.transform.current_world = identity;
		state.transform.pending_world = identity;
		state.transform.transform_is_pending = false;

		state.x_skew = 0.0;
		state.y_skew = 0.0;
		state.z_offset = 0.0;
		state.zbuffer_enabled = false;

		// Set 3D things
		world.origin = D3DXVECTOR3(0, 0, 0);
		world.eye_vector = D3DXVECTOR3(0, 0, 100.0);		// Eye 100 units above layout
		world.lookat_vector = world.origin;
		world.up_vector = D3DXVECTOR3(0, -1.0, 0);
	}

	CDX9Renderer::~CDX9Renderer()
	{
		Close();
	}

	// Load D3D DLLs and get device caps
	void CDX9Renderer::LoadD3D()
	{
		// Load d3d9.dll
		if (d3d9_dll == NULL)
			d3d9_dll = LoadLibrary(_T("d3d9.dll"));

		if (d3d9_dll == NULL)
			throw error(_T("Failed to load Direct3D 9 (d3d9.dll).  Please check your DirectX version is up-to-date."), GetLastError());

		LPDIRECT3DCREATE9 D3DCreate9 = (LPDIRECT3DCREATE9)GetProcAddress(d3d9_dll, _T("Direct3DCreate9"));

		if (D3DCreate9 == NULL)
			throw error(_T("Failed to initialise Direct3D 9!"), GetLastError());

		// Initialise D3D 9
		if (d3d9 == NULL)
			d3d9 = D3DCreate9(D3D_SDK_VERSION);

		if (d3d9 == NULL)
			throw error(_T("Failed to create Direct3D 9!"), GetLastError());

		//Get device capabilities
		hr = d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3d9_caps);
		
		if (FAILED(hr))
			throw error(_T("Failed to initialise Direct3D 9!"), hr);
	}

	// Create a D3D9 device
	void CDX9Renderer::CreateDevice(d3d9_init_params& params)
	{
		// Debug assertions
		params.check_valid();

		// Close up the device in case one is already open
		Close();

		// Load DLLs in case it was not called before now
		LoadD3D();

		// Copy relevant params locally
		hWnds = params.hWnds;
		use_zbuffer = params.use_zbuffer;
		multisamples = params.multisamples;
		backbuffer_format = params.backbuffer_format;

		// Setup present parameters vector
		d3d9_presents.resize(GetMultiHeadCount());
		ZeroMemory(&(d3d9_presents.front()), sizeof(D3DPRESENT_PARAMETERS) * d3d9_presents.size());

		// Setup first d3dPresent
		D3DPRESENT_PARAMETERS& present = d3d9_presents[0];

		// Initialise multisamples if enabled
		if (multisamples >= 2) {

			if (params.swap != swap_discard)
				throw error(_T("Multisampling can only be used with the discard swap effect."), E_FAIL);

			hr = d3d9->CheckDeviceMultiSampleType(
			  D3DADAPTER_DEFAULT,
			  D3DDEVTYPE_HAL,
			  get_d3d9_equiv(params.backbuffer_format),
			  !params.fullscreen,	// windowed?
			  get_d3d9_multisample(multisamples),
			  &multisample_quality
			);

			// Multisampling not supported.  Turn it off.
			if (hr != D3D_OK) {
				multisamples = 0;
				params.multisamples = 0;
				multisample_quality = 0;
			}
			else
				multisample_quality -= 1;		// Set to last valid quality level
		}
		else
			multisample_quality = 0;

		present.SwapEffect = get_d3d9_equiv(params.swap);
		present.hDeviceWindow = params.hWnds.front();
		present.BackBufferCount = 1;						// Single back buffer
		present.BackBufferFormat = get_d3d9_equiv(params.backbuffer_format);
		present.MultiSampleType = get_d3d9_multisample(multisamples);
		present.MultiSampleQuality = multisample_quality;
		present.EnableAutoDepthStencil = params.use_zbuffer;// Create auto Z buffer if one wanted
		present.AutoDepthStencilFormat = D3DFMT_D16;		// 16 bit float depth buffer
		present.Flags = (params.use_zbuffer ? D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL : 0);

		// VSynced FPS - use interval "one"
		if (params.fps_mode == framerate_vsynced)
			present.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		// Fixed or uncapped FPS use immediate presentation
		else
			present.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		// Fullscreen mode enabled: fullscreen backbuffer
		if (params.fullscreen) {

			present.Windowed = false;
			present.BackBufferWidth = params.backbuffer_width;
			present.BackBufferHeight = params.backbuffer_height;
		}
		// Windowed mode
		else {

			// Get window bounds
			RECT rWindow;
			GetClientRect(hWnds.front(), &rWindow);

			// Set up backbuffer in window client area
			present.Windowed = true;
			present.BackBufferWidth = rWindow.right - rWindow.left;
			present.BackBufferHeight = rWindow.bottom - rWindow.top;
		}

		// Save backbuffer size
		backbuffer_width = present.BackBufferWidth;
		backbuffer_height = present.BackBufferHeight;
		backbuffer_widthf = backbuffer_width;
		backbuffer_heightf = backbuffer_height;
		state.last_rendertarget_width = backbuffer_width;
		state.last_rendertarget_height = backbuffer_height;

		// Copy present params to be the same for all windows
		UpdatePresentsArray();

		// Use adaptergroup device flag for multimonitor displays
		DWORD create_flags = 0;
		if (multimonitor != multimonitor_singlescreen)			create_flags |= D3DCREATE_ADAPTERGROUP_DEVICE;
		if (d3d9_caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)	create_flags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else													create_flags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		// Create the device
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, params.hFocusWnd,
								create_flags, &(d3d9_presents.front()), &d3d9_device);  

		//Make sure device was created
		if (FAILED(hr) || d3d9_device == NULL)
			throw error(_T("Failed to create Direct3D 9 device."), hr);

		// Initialise the device resources eg. vertex buffers
		CreateDeviceResources();
	}

	// Create vertex buffers etc.
	void CDX9Renderer::CreateDeviceResources()
	{
		//Set vertex shader
		d3d9_device->SetVertexShader(NULL);
		d3d9_device->SetFVF(vertex_format);

		// Create the batching buffer
		if (batch_buffer == NULL) {
			d3d9_device->CreateVertexBuffer(sizeof(vertex) * d3d_max_vertices_size, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, vertex_format, D3DPOOL_DEFAULT,
											&batch_buffer, NULL);
		}

		// Create index buffer
		if (index_buffer == NULL) {
			d3d9_device->CreateIndexBuffer(sizeof(unsigned short) * d3d_max_indices_size, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_DEFAULT,
											&index_buffer, NULL);
		}

		// Initialise batching
		ResetBatch();

		// Create sprite for text rendering
		if (pD3DXSprite == NULL) {
			hr = D3DXCreateSprite(d3d9_device, &pD3DXSprite);

			if (FAILED(hr))
				throw error(_T("Failed to create sprite"), hr);
		}

		// Create line for smooth lines
		if (pD3DXLine == NULL) {
			hr = D3DXCreateLine(d3d9_device, &pD3DXLine);

			if (FAILED(hr))
				throw error(_T("Failed to create line drawer"), hr);

			pD3DXLine->SetAntialias(TRUE);
		}

		// Get the rendertarget for access to backbuffer surface
		d3d9_device->GetRenderTarget(0, &display_surface);
		back_buffer = display_surface;

		// Initialise 2D renderstates
		Prepare2D();
	}

	// Set render states & stream sources etc. for normal 2D rendering.  Done on startup, and should be called
	// any time you manually override renderstates for 3D rendering etc.
	void CDX9Renderer::Prepare2D()
	{
		// Flush anything waiting in the batch; directly modifying device
		EndBatch();

		d3d9_device->SetStreamSource(0, batch_buffer, 0, sizeof(vertex));
		d3d9_device->SetIndices(index_buffer);

		// Set up rendering states for 2D drawing
		d3d9_device->SetRenderState(D3DRS_LIGHTING, FALSE);
		d3d9_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		d3d9_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		d3d9_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		d3d9_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		d3d9_device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		d3d9_device->SetRenderState(D3DRS_LASTPIXEL, TRUE);
		d3d9_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);		// thin pixel lines
		d3d9_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // renders back faces so horizontal flips work

		if (use_zbuffer) {
			d3d9_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			d3d9_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			state.zbuffer_enabled = true;

			// Switch on alpha testing to stop empty alpha writing Z buffer values
			d3d9_device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			d3d9_device->SetRenderState(D3DRS_ALPHAREF, 0x00000000);
			d3d9_device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//d3d9_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			//d3d9_device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			//d3d9_device->SetRenderState(D3DRS_ZFUNC, D3DZB_TRUE);
		}

		d3d9_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);

		// Mipmapping stuff
		d3d9_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		d3d9_device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 1);
		float bias = -0.5f;
		d3d9_device->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD)(&bias)));

		// Prevent bleeding
		d3d9_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
		d3d9_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

		// Set transforms to identity
		D3DXMATRIX Identity;
		D3DXMatrixIdentity(&Identity);

		d3d9_device->SetTransform(D3DTS_PROJECTION, &Identity);
		d3d9_device->SetTransform(D3DTS_WORLD, &Identity);
		d3d9_device->SetTransform(D3DTS_VIEW, &Identity);

		current_rendertarget = NULL;
		current_texture = NULL;

		// Retrieve current state
		for (int i = 1; i < renderstate_count; i++) {
			DWORD value;
			d3d9_device->GetRenderState(get_d3d9_equiv((renderstate)i), &value);
			state.current_renderstates[i] = d3d9_to_cr_rsv((renderstate)i, value);
			state.pending_renderstates[i] = rsv_null;
		}

		for (int i = 1; i < samplerstate_count; i++) {
			DWORD value;
			d3d9_device->GetSamplerState(0, get_d3d9_equiv((samplerstate)i), &value);
			state.current_samplerstates[i] = d3d9_to_cr_ssv((samplerstate)i, value);
			state.pending_samplerstates[i] = ssv_null;
		}

		uint pt_size_temp;
		d3d9_device->GetRenderState(D3DRS_POINTSIZE, (DWORD*)&pt_size_temp);
		state.point_size = (cr_float&)pt_size_temp;

		Update3DMaths(backbuffer_widthf, backbuffer_heightf);
		//d3d9_device->SetTransform(D3DTS_VIEW, &world.view_matrix);

		UpdateTransform();
	}

	// Get the multihead number of adapters
	uint CDX9Renderer::GetMultiHeadCount() const
	{
		// Single screen mode uses only 1 swap chain/backbuffer/window
		if (multimonitor == multimonitor_singlescreen)
			return 1;
		// All other multimonitor modes use the number of adapters
		else
			return d3d9_caps.NumberOfAdaptersInGroup;
	}

	multimonitor_mode CDX9Renderer::GetMultiMonitorMode() const
	{
		return multimonitor;
	}

	// For multihead: update all present structures in the array
	void CDX9Renderer::UpdatePresentsArray()
	{
		int i;

		// Assign presentation structures
		for (i = 1; i < GetMultiHeadCount(); i++)
			d3d9_presents[i] = d3d9_presents[0];

		// Assign windows
		for (i = 0; i < GetMultiHeadCount(); i++)
			d3d9_presents[i].hDeviceWindow = hWnds[i];
	}

	void CDX9Renderer::SetXScale(cr_float xf)
	{
		state.transform.x_scale = xf;
	}

	cr_float CDX9Renderer::GetXScale() const
	{
		return state.transform.x_scale;
	}

	void CDX9Renderer::SetYScale(cr_float yf)
	{
		state.transform.y_scale = yf;
	}

	cr_float CDX9Renderer::GetYScale() const
	{
		return state.transform.y_scale;
	}

	void CDX9Renderer::SetScale(point xy)
	{
		state.transform.x_scale = xy.x;
		state.transform.y_scale = xy.y;
	}

	point CDX9Renderer::GetScale() const
	{
		return point(state.transform.x_scale, state.transform.y_scale);
	}

	void CDX9Renderer::SetAngle(cr_float rads)
	{
		state.transform.angle = rads;
	}

	cr_float CDX9Renderer::GetAngle() const
	{
		return state.transform.angle;
	}

	void CDX9Renderer::SetZOffset(cr_float z)
	{
		state.z_offset = z;
	}

	cr_float CDX9Renderer::GetZOffset() const
	{
		return state.z_offset;
	}

	point CDX9Renderer::Project(point3d pt, bool transform) const
	{
		const D3DXMATRIX* world_matrix;

		if (transform) {
			// Use the current world matrix whether its pending or current
			if (state.transform.transform_is_pending)
				world_matrix = &state.transform.pending_world;
			else
				world_matrix = &state.transform.current_world;
		}
		else
			world_matrix = &world.world_matrix;

		D3DXVECTOR3 screen_space;
		D3DXVECTOR3 object_space(pt.x, pt.y, pt.z);
		D3DXVec3Project(&screen_space, &object_space, &world.viewport, &world.projection_matrix, &world.view_matrix, world_matrix);
		return point(screen_space.x, screen_space.y);
	}

	void CDX9Renderer::Project(const point3d* pts, point3d* outs, uint count, bool transform) const
	{
		const D3DXMATRIX* world_matrix;

		if (transform) {
			// Use the current world matrix whether its pending or current
			if (state.transform.transform_is_pending)
				world_matrix = &state.transform.pending_world;
			else
				world_matrix = &state.transform.current_world;
		}
		else
			world_matrix = &world.world_matrix;

		D3DXVec3ProjectArray((D3DXVECTOR3*)outs, sizeof(point3d), (D3DXVECTOR3*)pts, sizeof(point3d), &world.viewport, &world.projection_matrix, &world.view_matrix, world_matrix, count);
	}

	point3d CDX9Renderer::Unproject(point3d pt, bool transform) const
	{
		const D3DXMATRIX* world_matrix;

		if (transform) {
			// Use the current world matrix whether its pending or current
			if (state.transform.transform_is_pending)
				world_matrix = &state.transform.pending_world;
			else
				world_matrix = &state.transform.current_world;
		}
		else
			world_matrix = &world.world_matrix;

		D3DXVECTOR3 screen_space(pt.x, pt.y, pt.z);
		D3DXVECTOR3 object_space;
		D3DXVec3Unproject(&object_space, &screen_space, &world.viewport, &world.projection_matrix, &world.view_matrix, world_matrix);
		return point3d(object_space.x, object_space.y, object_space.z);
	}

	void CDX9Renderer::Unproject(const point3d* pts, point3d* outs, uint count, bool transform) const
	{
		const D3DXMATRIX* world_matrix;

		if (transform) {
			// Use the current world matrix whether its pending or current
			if (state.transform.transform_is_pending)
				world_matrix = &state.transform.pending_world;
			else
				world_matrix = &state.transform.current_world;
		}
		else
			world_matrix = &world.world_matrix;

		D3DXVec3UnprojectArray((D3DXVECTOR3*)outs, sizeof(point3d), (D3DXVECTOR3*)pts, sizeof(point3d), &world.viewport, &world.projection_matrix, &world.view_matrix, world_matrix, count);
	}

	// Update matrices for 3D stuff
	void CDX9Renderer::Update3DMaths(cr_float rendertarget_width, cr_float rendertarget_height)
	{
		// Set the view matrix
		D3DXMatrixLookAtLH(&world.view_matrix, &world.eye_vector, &world.lookat_vector, &world.up_vector);
		d3d9_device->SetTransform(D3DTS_VIEW, &world.view_matrix);

		// Set the projection matrix
		D3DXMatrixPerspectiveFovLH(&world.projection_matrix,					//Result Matrix
								  D3DX_PI/4,									//Field of View, in radians.
								  rendertarget_width / rendertarget_height,		//Aspect ratio
								  1.0f,										//Near view plane
								  1100.0f);									//Far view plane

		d3d9_device->SetTransform(D3DTS_PROJECTION, &world.projection_matrix);

		// Set the viewport
		world.viewport.X = 0;
		world.viewport.Y = 0;
		world.viewport.Width = rendertarget_width;
		world.viewport.Height = rendertarget_height;
		world.viewport.MinZ = 0;
		world.viewport.MaxZ = 1;
		d3d9_device->SetViewport(&world.viewport);

		// Calculate world units per pixel with unscaled world
		D3DXVECTOR3 pt1, pt2;

		D3DXVECTOR3 topleft(0,0,0);
		D3DXVECTOR3 bottomright(1, 1, 0);
		
		D3DXVec3Project(&pt1, &topleft, &world.viewport, &world.projection_matrix, &world.view_matrix, &identity);
		D3DXVec3Project(&pt2, &bottomright, &world.viewport, &world.projection_matrix, &world.view_matrix, &identity);

		world.scale.x = 1.0 / (pt2.x - pt1.x);
		world.scale.y = 1.0 / (pt2.y - pt1.y);

		// Calculate world matrix
		D3DXMatrixScaling(&world.world_matrix, world.scale.x, world.scale.y, 1.0);

		// Apply the translation matrix
		D3DXMATRIX translation_matrix;

		D3DXMatrixTranslation(&translation_matrix,  (-rendertarget_width / 2.0 - 0.5),
													(-rendertarget_height / 2.0 - 0.5),
													 0);

		D3DXMatrixMultiply(&world.world_matrix, &translation_matrix, &world.world_matrix);
	}

	void CDX9Renderer::UpdateTransform(bool enable_transform)
	{
		/*
		// Not compatible with enable_transform yet
		// Nothing changed: cancel
		if	   (state.transform.x_scale == state.transform.old_x_scale
			 && state.transform.y_scale == state.transform.old_y_scale
			 && state.transform.angle == state.transform.old_angle)
		{
			return;
		}
		// Something's changed: update scaling values and update matrices
		else {
			state.transform.old_x_scale = state.transform.x_scale;
			state.transform.old_y_scale = state.transform.y_scale;
			state.transform.old_angle = state.transform.angle;
		}
		*/

		D3DXMATRIX new_world = world.world_matrix;

		if(enable_transform)
		{
			// Turn on Z buffer
			if (state.zbuffer_enabled)
				SetRenderState(rs_zbuffer_enabled, rsv_enabled);
			else
				SetRenderState(rs_zbuffer_enabled, rsv_disabled);

			if (state.transform.angle != 0.0f) {
				D3DXMATRIX angle_matrix;
				D3DXMatrixRotationYawPitchRoll(&angle_matrix, 0, 0, state.transform.angle);
				D3DXMatrixMultiply(&new_world, &angle_matrix, &new_world);
			}

			if (state.transform.x_scale != 1.0f || state.transform.y_scale != 1.0f) {
				D3DXMATRIX scale_matrix;

				// Scale Z by average zoom
				cr_float z_scale = (state.transform.x_scale + state.transform.y_scale) / 2.0;
				D3DXMatrixScaling(&scale_matrix, state.transform.x_scale, state.transform.y_scale, z_scale);
				D3DXMatrixMultiply(&new_world, &scale_matrix, &new_world);
			}
		}
		// Transform disabled: turn off Z buffer to allow plain screen blits
		else
			SetRenderState(rs_zbuffer_enabled, rsv_disabled);

		// Projection already in use
		// Immediately after a reset this is not a valid optimisation so skip the check
		if (new_world == state.transform.current_world && device_got_reset == false) {

			// Discard this command
			state.transform.transform_is_pending = false;
			return;
		}
		else {
			state.transform.pending_world = new_world;
			state.transform.transform_is_pending = true;
		}
	}

	// Texture functions
	TextureHandle CDX9Renderer::CreateRenderTargetTexture(uint width, uint height, texture_format format, bool multisampled_target)
	{
		// Can't call during lost device
		if (device_is_lost)
			return NULL;
		
		EndBatch();

		d3d9texture* tex = new d3d9texture;

		D3DSURFACE_DESC surface_desc;

		// Multisampled targets have to be a surface.  Otherwise make a texture.
		if (multisampled_target) {

			// Make surface with multisample support
			hr = d3d9_device->CreateRenderTarget(width, height, get_d3d9_equiv(format), get_d3d9_multisample(multisamples),
												multisample_quality, FALSE, &(tex->surface_ptr), NULL);

			if (tex->surface_ptr == NULL || FAILED(hr))
				throw error(_T("Failed to create render target"), hr);

			tex->is_multisampled = true;
			tex->surface_ptr->GetDesc(&surface_desc);
		}
		else {
			// Create render-target texture
			hr = D3DXCreateTexture(d3d9_device, width, height, 1, D3DUSAGE_RENDERTARGET,
								get_d3d9_equiv(format), D3DPOOL_DEFAULT, &(tex->texture_ptr));

			if (tex->texture_ptr == NULL || FAILED(hr))
				throw error(_T("Failed to create render target texture"), hr);

			// Initialise the texture
			tex->texture_ptr->GetLevelDesc(0, &surface_desc);
		}

		tex->rendertarget = true;		// Render target
		tex->format = format;

		// Get the surface & image sizes
		tex->surface_width = surface_desc.Width;
		tex->surface_height = surface_desc.Height;

		tex->image_width = width;
		tex->image_height = height;

		tex->image_widthf = tex->image_width;
		tex->image_heightf = tex->image_height;

		// Precompute the x and y image-to-surface ratios
		tex->xf = (cr_float)width / (cr_float)surface_desc.Width;
		tex->yf = (cr_float)height / (cr_float)surface_desc.Height;

		// Calculate size, in bytes
		tex->size_bytes = tex->surface_width * tex->surface_height * get_format_bytes_per_pixel(format);

		// Add to the texture list
		textures.insert(tex);

		// Return the pointer as the handle
		return tex;
	}

	void CDX9Renderer::ResizeRenderTargetTexture(TextureHandle th, uint new_width, uint new_height)
	{
		if (!th->rendertarget)
			throw error(_T("Can't resize a non-rendertarget texture"), E_FAIL);

		if (th->surface_ptr != NULL)
			throw error(_T("Resizing multisample render targets is not supported"), E_FAIL);

		// Not a batched command
		EndBatch();

		// Save state
		TextureHandle old_rendertarget = GetRenderTarget();
		TextureHandle old_texture = GetTexture();

		// Create new rendertarget of new size
		TextureHandle new_tex = CreateRenderTargetTexture(new_width, new_height, th->format);

		// Copy the image content
		SetRenderTarget(new_tex);
		ClearRenderTarget();
		SetTexture(th);
		Quad(0.0, 0.0);

		// Replace the old texture with the new one
		th->texture_ptr->Release();				// Free texture
		th->texture_ptr = new_tex->texture_ptr;	// Adopt new texture

		th->image_height = new_tex->image_height;
		th->image_heightf = new_tex->image_heightf;
		th->image_width = new_tex->image_width;
		th->image_widthf = new_tex->image_widthf;

		th->size_bytes = new_tex->size_bytes;
		th->surface_height = new_tex->surface_height;
		th->surface_width = new_tex->surface_width;
		th->xf = new_tex->xf;
		th->yf = new_tex->yf;

		// Get rid of the new texture's handle, its texture is used by th now
		textures.erase(textures.find(new_tex));

		// Restore state
		SetRenderTarget(old_rendertarget);
		SetTexture(old_texture);
	}

	// Protected member function to premultiply a nonpremultiplied texture handle.
	// Useful for loading stuff from files at runtime.  This is done by a texture lock though, and processed on the CPU,
	// because there's no other way to modify a non-rendertarget texture.  Sucks.
	// Only works with an A8R8G8B8 texture.  Still quite fast though.
	void CDX9Renderer::PremultiplyTextureCPU(TextureHandle th)
	{
		/*
		__int64 freq, start, end, dur;
		QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		QueryPerformanceCounter((LARGE_INTEGER*)&start);
		*/

		CRASSERT(TextureExists(th));

		if (th->format != texture_format_a8r8g8b8)
			throw error(_T("Only texture premultiplication of A8R8G8B8 formats is supported."), E_FAIL);

		IDirect3DTexture9* texture_ptr = th->texture_ptr;

		D3DLOCKED_RECT locked_rect;
		D3DSURFACE_DESC level_desc;
		RECT lock_area;
		lock_area.left = 0;
		lock_area.top = 0;

		uint levelcount = texture_ptr->GetLevelCount();

		// Premultiply every mipmap level
		for (uint level = 0; level < levelcount; level++) {

			texture_ptr->GetLevelDesc(level, &level_desc);

			// Only lock/process the image area on this surface
			// Add 0.5f for round-to-nearest
			uint width = ((cr_float)level_desc.Width * th->xf) + 0.5f;
			uint height = ((cr_float)level_desc.Height * th->yf) + 0.5f;

			lock_area.right = width;
			lock_area.bottom = height;

			hr = texture_ptr->LockRect(level, &locked_rect, &lock_area, 0);

			if (FAILED(hr))
				throw error(_T("Failed to premultiply texture"), hr);

			INT pitch = locked_rect.Pitch;
			BYTE* data = (BYTE*)locked_rect.pBits;

			for (uint y = 0; y < height; y++) {
				for (uint x = 0; x < width; x++) {
					
					D3DCOLOR& c = (D3DCOLOR&)data[(y * pitch) + (x * sizeof(D3DCOLOR))];

					uint alpha_byte = c >> 24;

					// Alpha component is 1.0f: multiplication has no effect, skip (speeds up a lot for opaque imagery)
					if (alpha_byte == 0xFF)
						continue;
					else {
						uint r_byte = (c & 0x00FF0000) >> 16;
						uint g_byte = (c & 0x0000FF00) >> 8;
						uint b_byte = (c & 0x000000FF);

						// Make floats and multiply
						float alpha_float = byte_to_float[alpha_byte];
						float r_float = byte_to_float[r_byte];
						float g_float = byte_to_float[g_byte];
						float b_float = byte_to_float[b_byte];

						// Premultiply!
						r_float *= alpha_float;
						g_float *= alpha_float;
						b_float *= alpha_float;

						// Convert back and write to c
						c &= 0xFF000000;

						// Could be outside the loop but still performs faster this way
						if (support_sse1)
							c |= (ftoi_sse1(r_float * 255.0f) << 16) | (ftoi_sse1(g_float * 255.0f) << 8) | ftoi_sse1(b_float * 255.0f);
						else
							c |= (ftoi_fpu(r_float * 255.0f) << 16) | (ftoi_fpu(g_float * 255.0f) << 8) | ftoi_fpu(b_float * 255.0f);
					}
				}
			}

			hr = texture_ptr->UnlockRect(level);

			if (FAILED(hr))
				throw error(_T("Failed to unlock when premultiplying texture"), hr);
		}

		/*
		QueryPerformanceCounter((LARGE_INTEGER*)&end);
		dur = end - start;
		double time_s = (double)dur / (double)freq;

		CString msg;
		msg.Format("Premultiplied in %.2f ms", time_s * 1000.0);
		MessageBox(NULL, msg, NULL, NULL);
		*/
		
	}

	TextureHandle CDX9Renderer::CreateTextureFromResource(HMODULE hModule, LPCTSTR resource, texture_format format, bool premultiply)
	{
		// Can't call during lost device
		if (device_is_lost)
			return NULL;
		
		EndBatch();

		d3d9texture* tex = new d3d9texture;

		D3DXIMAGE_INFO src_info;      // For getting image width/height

		// Load image from file
		hr = D3DXCreateTextureFromResourceEx(d3d9_device, hModule, resource, 0, 0, mipmap_levels, 0, 
			get_d3d9_equiv(format), D3DPOOL_MANAGED, D3DX_FILTER_NONE, mipmap_filter, 
			transparent_color.getD3DCOLOR(), &src_info, NULL, &(tex->texture_ptr));

		if (tex->texture_ptr == NULL || FAILED(hr))
			throw error(_T("Failed to create texture from file"), hr);

		// Initialise the texture
		D3DSURFACE_DESC surface_desc;
		tex->texture_ptr->GetLevelDesc(0, &surface_desc);

		tex->rendertarget = false;		// Not a render target
		tex->format = format;

		// Get the surface & image sizes
		tex->surface_width = surface_desc.Width;
		tex->surface_height = surface_desc.Height;

		tex->image_width = src_info.Width;
		tex->image_height = src_info.Height;

		tex->image_widthf = tex->image_width;
		tex->image_heightf = tex->image_height;

		// Precompute the x and y image-to-surface ratios
		tex->xf = (cr_float)src_info.Width / (cr_float)surface_desc.Width;
		tex->yf = (cr_float)src_info.Height / (cr_float)surface_desc.Height;

		// Calculate size, in bytes
		tex->size_bytes = tex->surface_width * tex->surface_height * get_format_bytes_per_pixel(format);

		// Add to the texture list
		textures.insert(tex);

		// Premultiply
		if (premultiply)
			PremultiplyTextureCPU(tex);

		// Return the pointer as the handle
		return tex;
	}

	TextureHandle CDX9Renderer::CreateTextureFromFile(PCTSTR filename, texture_format format, bool premultiply)
	{
		// Can't call during lost device
		if (device_is_lost)
			return NULL;
		
		EndBatch();

		d3d9texture* tex = new d3d9texture;

		D3DXIMAGE_INFO src_info;      // For getting image width/height

		// Load image from file
		hr = D3DXCreateTextureFromFileEx(d3d9_device, filename, 0, 0, mipmap_levels, 0, 
			get_d3d9_equiv(format), D3DPOOL_MANAGED, D3DX_FILTER_NONE, mipmap_filter, 
			transparent_color.getD3DCOLOR(), &src_info, NULL, &(tex->texture_ptr));

		if (tex->texture_ptr == NULL || FAILED(hr))
			throw error(_T("Failed to create texture from file"), hr);

		// Initialise the texture
		D3DSURFACE_DESC surface_desc;
		tex->texture_ptr->GetLevelDesc(0, &surface_desc);

		tex->rendertarget = false;		// Not a render target
		tex->format = format;

		// Get the surface & image sizes
		tex->surface_width = surface_desc.Width;
		tex->surface_height = surface_desc.Height;

		tex->image_width = src_info.Width;
		tex->image_height = src_info.Height;

		tex->image_widthf = tex->image_width;
		tex->image_heightf = tex->image_height;

		// Precompute the x and y image-to-surface ratios
		tex->xf = (cr_float)src_info.Width / (cr_float)surface_desc.Width;
		tex->yf = (cr_float)src_info.Height / (cr_float)surface_desc.Height;

		// Calculate size, in bytes
		tex->size_bytes = tex->surface_width * tex->surface_height * get_format_bytes_per_pixel(format);

		// Add to the texture list
		textures.insert(tex);

		// Premultiply
		if (premultiply)
			PremultiplyTextureCPU(tex);

		// Return the pointer as the handle
		return tex;
	}

	TextureHandle CDX9Renderer::CreateTextureFromFileInMemory(LPCVOID pData, uint len, texture_format format, bool premultiply)
	{
		// Can't call during lost device
		if (device_is_lost)
			return NULL;
		
		EndBatch();

		d3d9texture* tex = new d3d9texture;

		D3DXIMAGE_INFO src_info;      // For getting image width/height

		// Load image from file in memory
		hr = D3DXCreateTextureFromFileInMemoryEx(d3d9_device, pData, len, 0, 0, mipmap_levels, 0, 
				get_d3d9_equiv(format), D3DPOOL_MANAGED, D3DX_FILTER_NONE, mipmap_filter, 
				transparent_color.getD3DCOLOR(), &src_info, NULL, &(tex->texture_ptr));

		if (tex->texture_ptr == NULL || FAILED(hr))
			throw error(_T("Failed to create texture"), hr);

		// Initialise the texture
		D3DSURFACE_DESC surface_desc;
		tex->texture_ptr->GetLevelDesc(0, &surface_desc);

		tex->rendertarget = false;		// Not a render target
		tex->format = format;

		// Get the surface & image sizes
		tex->surface_width = surface_desc.Width;
		tex->surface_height = surface_desc.Height;

		tex->image_width = src_info.Width;
		tex->image_height = src_info.Height;

		tex->image_widthf = tex->image_width;
		tex->image_heightf = tex->image_height;

		// Precompute the x and y image-to-surface ratios
		tex->xf = (cr_float)src_info.Width / (cr_float)surface_desc.Width;
		tex->yf = (cr_float)src_info.Height / (cr_float)surface_desc.Height;

		// Calculate size, in bytes
		tex->size_bytes = tex->surface_width * tex->surface_height * get_format_bytes_per_pixel(format);

		// Add to the texture list
		textures.insert(tex);

		// Premultiply
		if (premultiply)
			PremultiplyTextureCPU(tex);

		// Return the pointer as the handle
		return tex;
	}

	void CDX9Renderer::AddTextureReference(TextureHandle t)
	{
		t->IncrementReferences();
	}

	bool CDX9Renderer::ReleaseTexture(TextureHandle t)
	{
		t->DecrementReferences();

		// No more references
		if (t->HasNoReferences()) {

			EndBatch(); // We must end the batch incase something uses this

			if (t->texture_ptr)
				t->texture_ptr->Release();	// Release DX9 texture
			else if (t->surface_ptr)
				t->surface_ptr->Release();
			
			// Find and remove from 'textures' list
			TextureIterator i = textures.find(t);

			CRASSERT(i != textures.end());

			textures.erase(i);

			return true;	// got deallocated
		}
		return false;		// still alive
	}

	void CDX9Renderer::DeallocateTexture(TextureHandle t)
	{
		if (t->texture_ptr)
			t->texture_ptr->Release();
		else if (t->surface_ptr)
			t->surface_ptr->Release();

		TextureIterator i = textures.find(t);

		CRASSERT(i != textures.end());

		textures.erase(i);
	}

	// Release all textures immediately
	void CDX9Renderer::ResetTextures()
	{
		TextureIterator i = textures.begin();
		TextureIterator end = textures.end();

		for ( ; i != end; ++i) {

			if ((*i)->texture_ptr)
				(*i)->texture_ptr->Release();		// Release DX9 texture
			else if ((*i)->surface_ptr)
				(*i)->surface_ptr->Release();
		}

		// Clear textures list
		textures.clear();
	}

	bool CDX9Renderer::TextureExists(TextureHandle th) const
	{
		return textures.find(th) != textures.end();
	}

	void CDX9Renderer::ReplaceTexture(TextureHandle oldTex, TextureHandle newTex)
	{
		if (oldTex->surface_ptr != NULL)
			throw error(_T("Replacing textures is not supported with multisample rendertargets"), E_FAIL);

		EndBatch(); // We must end the batch incase something uses this

		// Make the old handle become the new one by releasing the old texture and copying members
		oldTex->texture_ptr->Release();

		// Keep reference count
		unsigned int refcount = oldTex->reference_count;

		// Copy members
		*oldTex = *newTex;

		// Restore reference count
		oldTex->reference_count = refcount;

		// Remove new texture entry from texture list
		textures.erase(textures.find(newTex));
	}

	void CDX9Renderer::SetSkew(cr_float xk, cr_float yk)
	{
		state.x_skew = xk;
		state.y_skew = yk;
	}

	void CDX9Renderer::BeginScene()
	{
		batches_per_present = 0;

		CRASSERT(d3d9_device != NULL);

		hr = d3d9_device->BeginScene();

		// Clear Z buffer on start of scene if one used
		if (use_zbuffer)
			d3d9_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

		if (FAILED(hr))
			throw error(_T("Could not begin scene"), hr);
	}

	void CDX9Renderer::EndScene()
	{
		EndBatch();

		CRASSERT(d3d9_device != NULL);

		hr = d3d9_device->EndScene();

		if (FAILED(hr))
			throw error(_T("Could not end scene"), hr);
	}

	HRESULT CDX9Renderer::Present(RECT* pRec, HWND hWndOverride)
	{
		EndBatch();

		// Write log if one set
#ifdef CR_DEBUG
		if (debug_save_next_batch && !debug_batch_log.str().empty()) {
			FILE* f = _tfopen(this->debug_save_filename.c_str(), "wt");

			if (f == NULL)
				throw error(_T("Could not create debug log file"), E_FAIL);

			tstring log = debug_batch_log.str();
			fwrite(log.c_str(), 1, log.size(), f);
			fclose(f);

			debug_save_next_batch = false;
		}
#endif
		CRASSERT(d3d9_device != NULL);

		hr = d3d9_device->Present(pRec, NULL, hWndOverride, NULL);

		device_got_reset = false;

		return hr;
	}

	uint CDX9Renderer::GetBatchesPerPresent() const
	{
		return batches_per_present;
	}

	void CDX9Renderer::SetAdditiveBlending()
	{
		SetRenderState(rs_srcblend, rsv_blend_one);
		SetRenderState(rs_destblend, rsv_blend_one);
	}

	void CDX9Renderer::SetAlphaBlending()
	{
		SetRenderState(rs_srcblend, rsv_blend_one);
		SetRenderState(rs_destblend, rsv_blend_invsrcalpha);
	}

	TextureHandle CDX9Renderer::GetTexture() const
	{
		return current_texture;
	}

	TextureHandle CDX9Renderer::GetRenderTarget() const
	{
		return current_rendertarget;
	}

	uint CDX9Renderer::GetTotalTextureMemoryUsage() const
	{
		uint total = 0;

		TextureConstIterator i = textures.begin();
		TextureConstIterator end = textures.end();

		for ( ; i != end; ++i)
			total += (*i)->size_bytes;

		return total;
	}

	uint CDX9Renderer::GetRenderTargetMemoryUsage() const
	{
		uint total = 0;

		TextureConstIterator i = textures.begin();
		TextureConstIterator end = textures.end();

		for ( ; i != end; ++i) {
			if ((*i)->rendertarget)
				total += (*i)->size_bytes;
		}

		return total;
	}

	// Text rendering
	FontHandle CDX9Renderer::CreateCRFont(PCTSTR font_face, INT height, uint width, uint weight, BOOL italic, DWORD quality, BYTE charset, BYTE pitch_and_family)
	{
		D3DXFONT_DESC desc;
		desc.Height = height * font_factor;
		desc.Width = width;
		desc.Weight = weight;
		desc.MipLevels = D3DX_DEFAULT;		// Create complete mip chain
		desc.Italic = italic;
		desc.CharSet = charset;
		desc.OutputPrecision = OUT_DEFAULT_PRECIS;
		desc.Quality = quality;
		desc.PitchAndFamily = pitch_and_family;
		memcpy(desc.FaceName, font_face, LF_FACESIZE);

		// Check if this font is already created
		FontIterator i = fonts.begin();
		FontIterator end = fonts.end();
		
		for ( ; i != end; ++i) {
			if ((*i)->SameFont(desc)) {
				(*i)->IncrementReferences();	// Increase references
				return *i;						// Return this existing font handle
			}
		}

		// Need to create a new font
		EndBatch();

		CDX9Font* pFont = new CDX9Font(this);
		pFont->IncrementReferences();		// Start with 1 reference
		pFont->desc = desc;

		D3DXCreateFontIndirect(d3d9_device, &(pFont->desc), &(pFont->font));

		fonts.insert(pFont);
		return pFont;
	}

	bool CDX9Renderer::FontExists(FontHandle fh) const
	{
		return fonts.find(fh) != fonts.end();
	}

	void CDX9Renderer::ReleaseCRFont(FontHandle fh)
	{
		CRASSERT(FontExists(fh));

		fh->DecrementReferences();

		if (fh->HasNoReferences()) {

			EndBatch(); // We must end the batch incase something uses this

			fh->font->Release();
			fonts.erase(fonts.find(fh));
		}
	}

	void CDX9Renderer::SetPointSampling()
	{
		SetSamplerState(ss_minfilter, ssv_point);
		SetSamplerState(ss_magfilter, ssv_point);
	}

	void CDX9Renderer::SetLinearSampling()
	{
		SetSamplerState(ss_minfilter, ssv_linear);
		SetSamplerState(ss_magfilter, ssv_linear);
	}

	// Device handling
	void CDX9Renderer::SetDeviceLost(bool lost)
	{
		device_is_lost = lost;
	}

	/*
	void CDX9Renderer::OnLostDevice()
	{
		pD3DXSprite->OnLostDevice();
		pD3DXLine->OnLostDevice();

		FontIterator i = fonts.begin();
		FontIterator end = fonts.end();

		for ( ; i != end; ++i)
			(*i)->font->OnLostDevice();
	}

	void CDX9Renderer::OnResetDevice()
	{
		pD3DXSprite->OnResetDevice();
		pD3DXLine->OnResetDevice();

		FontIterator i = fonts.begin();
		FontIterator end = fonts.end();

		for ( ; i != end; ++i)
			(*i)->font->OnResetDevice();
	}
	*/

	const D3DCAPS9& CDX9Renderer::GetCaps() const
	{
		return d3d9_caps;
	}

	IDirect3DDevice9* CDX9Renderer::GetDevice() const
	{
		return d3d9_device;
	}

	void CDX9Renderer::Reset()
	{
		//UNHACK THIS
		// Recalculate multisampling quality if necessary
		DWORD multisample_quality;

		if (d3d9_presents.front().MultiSampleType != D3DMULTISAMPLE_NONE) {

			hr = d3d9->CheckDeviceMultiSampleType(
			  D3DADAPTER_DEFAULT,
			  D3DDEVTYPE_HAL,
			  get_d3d9_equiv(backbuffer_format),
			  !d3d9_presents.front().Windowed,
			  d3d9_presents.front().MultiSampleType,
			  &multisample_quality
			);

			// Multisampling not supported.  Turn it off.
			if (hr != D3D_OK)
				d3d9_presents.front().MultiSampleType = D3DMULTISAMPLE_NONE;
			else
				multisample_quality -= 1;		// Set to last valid quality level
		}
		else
			multisample_quality = 0;

		d3d9_presents.front().MultiSampleQuality = multisample_quality;

		UpdatePresentsArray();

		// Release our vertex buffers
		if (batch_buffer)	batch_buffer->Release();
		if (index_buffer)	index_buffer->Release();

		batch_buffer = NULL;
		index_buffer = NULL;

		// Lose D3DX things
		if (pD3DXSprite)
			pD3DXSprite->OnLostDevice();
		if (pD3DXLine)
			pD3DXLine->OnLostDevice();
		
		display_surface->Release();

		if(display_surface != back_buffer)
			back_buffer->Release();

		// Find and release all rendertarget textures (which cant be auto managed)
		TextureIterator t = textures.begin();
		TextureIterator end = textures.end();

		for ( ; t != end; ++t) {
			if ((*t)->rendertarget) {

				if ((*t)->texture_ptr)
					(*t)->texture_ptr->Release();
				else if ((*t)->surface_ptr)
					(*t)->surface_ptr->Release();
			}
		}

		// Lose all fonts
		FontIterator f = fonts.begin();
		FontIterator fonts_end = fonts.end();

		for ( ; f != fonts_end; ++f)
			(*f)->font->OnLostDevice();

		// Reset
		hr = d3d9_device->Reset(&(d3d9_presents.front()));

		if (FAILED(hr))
			throw error(_T("Failed to reset device"), hr);

		backbuffer_width = d3d9_presents[0].BackBufferWidth;
		backbuffer_height = d3d9_presents[0].BackBufferHeight;
		backbuffer_widthf = backbuffer_width;
		backbuffer_heightf = backbuffer_height;

		// Restore all fonts
		f = fonts.begin();

		for ( ; f != fonts_end; ++f)
			(*f)->font->OnResetDevice();

		// Recreate all lost rendertarget textures
		t = textures.begin();

		for ( ; t != end; ++t) {
			if ((*t)->rendertarget) {

				if ((*t)->is_multisampled) {

					hr = d3d9_device->CreateRenderTarget((*t)->image_width, (*t)->image_height, get_d3d9_equiv((*t)->format), get_d3d9_multisample(multisamples),
												multisample_quality, FALSE, &((*t)->surface_ptr), NULL);

					if (FAILED(hr) || (*t)->surface_ptr == NULL)
						throw error(_T("Failed to restore rendertarget"), hr);
				}
				else {
					// Restore this texture (non-multisampled)
					hr = D3DXCreateTexture(d3d9_device, (*t)->image_width, (*t)->image_height, 1, D3DUSAGE_RENDERTARGET,
											get_d3d9_equiv((*t)->format), D3DPOOL_DEFAULT, &((*t)->texture_ptr));

					if (FAILED(hr) || (*t)->texture_ptr == NULL)
						throw error(_T("Failed to restore rendertarget texture"), hr);
				}
			}
		}

		// Restore D3DX things
		if (pD3DXSprite)
			pD3DXSprite->OnResetDevice();
		if (pD3DXLine)
			pD3DXLine->OnResetDevice();

		// Reset last settings
		current_texture = unallocated_texture;
		current_rendertarget = unallocated_texture;

		device_got_reset = true;

		// Recreate device resources again
		CreateDeviceResources();
	}

	// Shut down renderer
	void CDX9Renderer::Close()
	{
		// Textures must all be explicitly released before calling Close().
		// If this asserts here, there are textures missing a release call.
		CRASSERT(textures.empty());

		// Release all fonts
		FontIterator i = fonts.begin();
		FontIterator end = fonts.end();

		for ( ; i != end; ++i)
			(*i)->font->Release();

		fonts.clear();

		// Release sprite for text rendering
		if (pD3DXSprite)						pD3DXSprite->Release();
		if (pD3DXLine)							pD3DXLine->Release();

		// Release vertex/index buffers
		if (batch_buffer)						batch_buffer->Release();
		if (index_buffer)						index_buffer->Release();

		// Release surfaces
		if (display_surface)					display_surface->Release();
		if (display_surface != back_buffer)		back_buffer->Release();

		// Release device and D3D9
		if (d3d9_device)						d3d9_device->Release();
		if (d3d9)								d3d9->Release();
		FreeLibrary(d3d9_dll);

		// Null out everything released
		pD3DXSprite = NULL;
		pD3DXLine = NULL;
		batch_buffer = NULL;
		index_buffer = NULL;
		display_surface = NULL;
		back_buffer = NULL;
		d3d9_device = NULL;
		d3d9 = NULL;
		d3d9_dll = NULL;

		// Reset other members
		d3d9_presents.resize(0);
		hWnds.resize(0);
		device_is_lost = false;
		current_texture = NULL;
		current_rendertarget = NULL;
	}
}