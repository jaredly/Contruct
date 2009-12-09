 /////////////////////////////////////////////////////////////////////////////
 // Common renderer definitions												//
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

// Make math.h define M_PI etc
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

// STL includes
#include <vector>
#include <list>
#include <set>
#include <string>
#include <algorithm>

// Debugging features
#ifdef _DEBUG
#define CR_DEBUG
#endif // _DEBUG

#ifdef CR_DEBUG
#include <sstream>			// Generating log
#include <stdio.h>			// Disk i/o
#endif //CR_DEBUG

#include "CR.h"

namespace cr {

	// Typedefs
	//typedef float							cr_float;
	typedef std::basic_string<TCHAR>		tstring;

	typedef point_base<cr_float>			point;
	typedef point3d_base<cr_float>			point3d;
	typedef rect_base<cr_float>				rect;
	typedef quad_base<cr_float>				quad;
	typedef color_base<cr_float>			color;
	typedef color_base<cr_float>::rgb_t		rgb_t;

	// Constants
	const point origin(0.0, 0.0);
	const rect zero_rect(0.0, 0.0, 0.0, 0.0);
	const rect unit_rect(0.0, 0.0, 1.0, 1.0);

	const color transparent_black(0.0, 0.0, 0.0, 0.0);
	const color opaque_black(1.0, 0.0, 0.0, 0.0);
	const color opaque_white(1.0, 1.0, 1.0, 1.0);

	inline color cr_colorref_opacity(COLORREF cref, cr_float opacity)
	{
		color c(cref);
		c.a = opacity;
		return c;
	}

#ifdef CR_DEBUG
	typedef std::basic_stringstream<TCHAR> tstringstream;
#endif //CR_DEBUG

	// Gradient directions
	enum gradient_direction {
		dir_up,
		dir_down,
		dir_left,
		dir_right
	};

	// Texture formats
	enum texture_format {
		texture_format_null,
		texture_format_a8r8g8b8,			// Standard integer ARGB format (8 bits per channel)
		texture_format_r8g8b8,				// Standard integer RGB format (8 bits per channel)
		texture_format_x8r8g8b8,			// Standard integer RGB format with X byte (8 bits per channel)
		texture_format_a16b16g16r16f,		// Floating-point ARGB format (16 bits per channel)
		texture_format_a32b32g32r32f		// Floating-point ARGB format (32 bits per channel)
	};

	uint get_format_bytes_per_pixel(texture_format tf);

	// Texture class
	class texture {
	public:
		texture();
		
		// Surface size, often a power-of-two texture eg. 256x256 or 512x512
		uint				surface_width;
		uint				surface_height;

		// Image content size, can be any size, and must be smaller or equal in size to the surface
		uint				image_width;
		uint				image_height;

		// Float equivalents to save int-float computation during rendering
		cr_float			image_widthf;
		cr_float			image_heightf;

		// Is a render target?
		bool				rendertarget;

		// Format
		texture_format		format;
		uint				size_bytes;		// Size in bytes in VRAM

		// Precomputed image-to-surface ratios
		cr_float			xf;
		cr_float			yf;

		// Reference count
		uint reference_count;

		// Reference counting functions
		void IncrementReferences();
		void DecrementReferences();
		bool HasNoReferences() const;
	};

	// Framerate modes
	enum framerate_mode {
		framerate_vsynced,
		framerate_uncapped,
		framerate_fixed
	};

	// Multimonitor modes
	enum multimonitor_mode {
		multimonitor_singlescreen,		// Fullscreen on primary monitor only
		multimonitor_clone				// Copy same display to all monitors
	};

	// Render states
	enum renderstate {		// D3D9 equivalents for reference
		rs_null,
		rs_srcblend,			// D3DRS_SRCBLEND
		rs_destblend,			// D3DRS_DESTBLEND
		rs_zbuffer_enabled,		// D3DRS_ZENABLE
		rs_pointsprite_enabled,	// D3DRS_POINTSPRITEENABLE
		rs_pointsize,			// Do not use directly, use SetPointSize()/GetPointSize()
		rs_blendop,				// D3DRS_BLENDOP
		rs_zwrite_enabled,		// D3DRS_ZWRITEENABLE

		rs_last_entry		// for determining number of possible values
	};

	const uint renderstate_count = (uint)rs_last_entry;

	enum renderstate_value {		// D3D9 equivalents
		rsv_null,
		rsv_blend_zero,				// D3DBLEND_ZERO
		rsv_blend_one,				// D3DBLEND_ONE
		rsv_blend_srccolor,			// D3DBLEND_SRCCOLOR
		rsv_blend_invsrccolor,		// D3DBLEND_INVSRCCOLOR
		rsv_blend_srcalpha,			// D3DBLEND_SRCALPHA
		rsv_blend_invsrcalpha,		// D3DBLEND_INVSRCALPHA
		rsv_blend_destalpha,		// D3DBLEND_DESTALPHA
		rsv_blend_invdestalpha,		// D3DBLEND_INVDESTALPHA
		rsv_enabled,				// Depends on state
		rsv_disabled,				// Depends on state
		rsv_blendop_add,			// D3DBLENDOP_ADD
		rsv_blendop_subtract,		// D3DBLENDOP_SUBTRACT
		rsv_blendop_revsubtract,	// D3DBLENDOP_REVSUBTRACT
		rsv_blendop_min,			// D3DBLENDOP_MIN
		rsv_blendop_max,			// D3DBLENDOP_MAX

	};

	// Sampler states
	enum samplerstate {		// D3D9 equivalents for reference
		ss_null,
		ss_addressu,		// D3DSAMP_ADDRESSU
		ss_addressv,		// D3DSAMP_ADDRESSV
		ss_minfilter,		// D3DSAMP_MINFILTER
		ss_magfilter,		// D3DSAMP_MAGFILTER

		ss_last_entry		// for determining number of possible values
	};

	const uint samplerstate_count = (uint)ss_last_entry;

	enum samplerstate_value {	// D3D9 equivalents
		ssv_null,
		ssv_wrap,				// D3DTADDRESS_WRAP
		ssv_mirror,				// D3DTADDRESS_MIRROR
		ssv_clamp,				// D3DTADDRESS_CLAMP
		ssv_border,				// D3DTADDRESS_BORDER
		ssv_mirroronce,			// D3DTADDRESS_MIRRORONCE
		ssv_point,				// D3DTEXF_POINT
		ssv_linear,				// D3DTEXF_LINEAR
	};

	// enum-to-string conversion for debug log
#ifdef CR_DEBUG
	tstring enum_name(gradient_direction dir);
	tstring enum_name(texture_format tf);
	tstring enum_name(renderstate rs);
	tstring enum_name(renderstate_value rsv);
	tstring enum_name(samplerstate ss);
	tstring enum_name(samplerstate_value ssv);
#endif //CR_DEBUG
}