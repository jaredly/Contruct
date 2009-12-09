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
#include "RendererCommon.h"

namespace cr {

	// Texture ctor
	texture::texture()
		: surface_width(0),
		  surface_height(0),
		  image_width(0),
		  image_height(0),
		  image_widthf(0.0),
		  image_heightf(0.0),
		  rendertarget(false),
		  format(texture_format_null),
		  size_bytes(0),
		  xf(1.0),
		  yf(1.0),
		  reference_count(0)
	{}

	// Texture reference counting
	void texture::IncrementReferences()
	{
		reference_count++;
	}

	void texture::DecrementReferences()
	{
		CRASSERT(reference_count > 0);

		reference_count--;
	}

	bool texture::HasNoReferences() const
	{
		return reference_count == 0;
	}

	uint get_format_bytes_per_pixel(texture_format tf)
	{
		switch (tf) {
		case texture_format_r8g8b8:
			return 3;		// 1 byte per RGB channel
		case texture_format_a8r8g8b8:
		case texture_format_x8r8g8b8:
			return 4;		// 1 byte per ARGB channel
		case texture_format_a16b16g16r16f:
			return 8;		// 2 bytes per ARGB channel
		case texture_format_a32b32g32r32f:
			return 16;		// 4 bytes per ARGB channel
		default:
			return 0;
		}
	}

	/////////////////////////////////
	// DEBUGGING FEATURES
	/////////////////////////////////
#ifdef CR_DEBUG
	tstring enum_name(gradient_direction dir)
	{
		switch (dir)
		{
		case dir_up:
			return _T("up");
		case dir_down:
			return _T("down");
		case dir_left:
			return _T("left");
		case dir_right:
			return _T("right");
		default:
			return _T("<unrecognised gradient direction>");
		}
	}

	tstring enum_name(texture_format tf)
	{
		switch (tf) {
		case texture_format_null:
			return _T("null");
		case texture_format_a8r8g8b8:
			return _T("a8r8g8b8");
		case texture_format_r8g8b8:
			return _T("r8g8b8");
		case texture_format_x8r8g8b8:
			return _T("x8r8g8b8");
		case texture_format_a16b16g16r16f:
			return _T("a16b16g16r16f");
		case texture_format_a32b32g32r32f:
			return _T("a32b32g32r32f");
		default:
			return _T("<unrecognised texture format>");
		}
	}

	tstring enum_name(renderstate rs)
	{
		switch (rs) {
		case rs_null:
			return _T("null");
		case rs_srcblend:
			return _T("srcblend");
		case rs_destblend:
			return _T("destblend");
		case rs_zbuffer_enabled:
			return _T("zbuffer_enabled");
		case rs_pointsprite_enabled:
			return _T("pointsprite_enabled");
		case rs_pointsize:
			return _T("pointsize");
		case rs_blendop:
			return _T("blendop");
		case rs_zwrite_enabled:
			return _T("zwrite_enabled");
		default:
			return _T("<unrecognised renderstate>");
		}
	}

	tstring enum_name(renderstate_value rsv)
	{
		switch (rsv) {
		case rsv_null:
			return _T("null");
		case rsv_blend_zero:
			return _T("blend_zero");
		case rsv_blend_one:
			return _T("blend_one");
		case rsv_blend_srccolor:
			return _T("blend_srccolor");
		case rsv_blend_invsrccolor:
			return _T("blend_invsrccolor");
		case rsv_blend_srcalpha:
			return _T("blend_srcalpha");
		case rsv_blend_invsrcalpha:
			return _T("blend_invsrcalpha");
		case rsv_blend_destalpha:
			return _T("blend_destalpha");
		case rsv_blend_invdestalpha:
			return _T("blend_invdestalpha");
		case rsv_enabled:
			return _T("enabled");
		case rsv_disabled:
			return _T("disabled");
		case rsv_blendop_add:
			return _T("blendop_add");
		case rsv_blendop_subtract:
			return _T("blendop_subtract");
		case rsv_blendop_revsubtract:
			return _T("blendop_revsubtract");
		case rsv_blendop_min:
			return _T("blendop_min");
		case rsv_blendop_max:
			return _T("blendop_max");
		default:
			return _T("<unrecognised renderstate value>");
		}
	}

	tstring enum_name(samplerstate ss)
	{
		switch (ss) {
		case ss_null:
			return _T("null");
		case ss_addressu:
			return _T("addressu");
		case ss_addressv:
			return _T("addressv");
		case ss_minfilter:
			return _T("minfilter");
		case ss_magfilter:
			return _T("magfilter");
		default:
			return _T("<unrecognised samplerstate>");		
		}
	}

	tstring enum_name(samplerstate_value ssv)
	{
		switch (ssv) {
		case ssv_null:
			return _T("null");
		case ssv_wrap:
			return _T("wrap");
		case ssv_mirror:
			return _T("mirror");
		case ssv_clamp:
			return _T("clamp");
		case ssv_border:
			return _T("border");
		case ssv_mirroronce:
			return _T("mirroronce");
		case ssv_point:
			return _T("point");
		case ssv_linear:
			return _T("linear");
		default:
			return _T("<unrecognised samplerstate value>");
		}
	}
#endif //CR_DEBUG
}