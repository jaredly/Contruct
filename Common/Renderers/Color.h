 /////////////////////////////////////////////////////////////////////////////
 // Color																	//
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

// ARGB floating-point color definition
#pragma once

namespace cr {

	// ARGB color type.  To prevent confusion, the American spelling "color" is always used.
	template<typename T>
	class color_base {
	public:
		typedef point3d_base<T> rgb_t;

		// 0.0 - 1.0
		T		a;
		T		r;
		T		g;
		T		b;

		// Constructors - don't construct with a D3DCOLOR, will use the COLORREF overload - use color_from_D3DCOLOR()
		color_base()
		{}

		color_base(const color_base& c)
			: a(c.a), r(c.r), g(c.g), b(c.b)
		{}

		color_base(T _a, T _r, T _g, T _b)
			: a(_a), r(_r), g(_g), b(_b)
		{}

		color_base(T _a, rgb_t _rgb)
			: a(_a), r(_rgb.r), g(_rgb.g), b(_rgb.b)
		{}

		explicit color_base(COLORREF c)
		{
			a = 1.0;

			uint ir = GetRValue(c);
			uint ig = GetGValue(c);
			uint ib = GetBValue(c);

			r = byte_to_float[ir];
			g = byte_to_float[ig];
			b = byte_to_float[ib];
		}

		// Operators
		bool operator ==(const color_base& other) const
		{
			return (a == other.a) && (r == other.r) && (g == other.g) && (b == other.b);
		}

		bool operator !=(const color_base& other) const
		{
			return !(operator ==(other));
		}

		color_base<T> operator +(const color_base& c) const
		{
			return color_base<T>(a + c.a, r + c.r, g + c.g, b + c.b);
		}

		color_base<T>& operator +=(const color_base& c)
		{
			a += c.a;
			r += c.r;
			g += c.g;
			b += c.b;
		}

		color_base<T> operator -(const color_base& c) const
		{
			return color_base<T>(a - c.a, r - c.r, g - c.g, b - c.b);
		}

		color_base<T>& operator -=(const color_base& c)
		{
			a -= c.a;
			r -= c.r;
			g -= c.g;
			b -= c.b;
		}

		color_base<T> operator *(const color_base& c) const
		{
			return color_base<T>(a * c.a, r * c.r, g * c.g, b * c.b);
		}

		color_base<T>& operator *=(const color_base& c)
		{
			a *= c.a;
			r *= c.r;
			g *= c.g;
			b *= c.b;
			return (*this);
		}

		color_base<T> operator /(const color_base& c) const
		{
			return color_base<T>(a / c.a, r / c.r, g / c.g, b / c.b);
		}

		color_base<T>& operator /=(const color_base& c)
		{
			a /= c.a;
			r /= c.r;
			g /= c.g;
			b /= c.b;
			return (*this);
		}

		color_base<T> operator *(T scalar) const
		{
			return color_base<T>(a * scalar, r * scalar, g * scalar, b * scalar);
		}

		color_base<T>& operator *=(T scalar)
		{
			a *= scalar;
			r *= scalar;
			g *= scalar;
			b *= scalar;
			return (*this);
		}

		color_base<T> operator /(T scalar) const
		{
			return color_base<T>(a / scalar, r / scalar, g / scalar, b / scalar);
		}

		color_base<T>& operator /=(T scalar)
		{
			a /= scalar;
			r /= scalar;
			g /= scalar;
			b /= scalar;
			return (*this);
		}

		color_base<T> premultiplied() const
		{
			return color_base(a, r * a, g * a, b * a);
		}

		rgb_t get_rgb() const
		{
			return rgb_t(r, g, b);
		}

		void set_rgb(const rgb_t& _rgb)
		{
			r = _rgb.r;
			g = _rgb.g;
			b = _rgb.b;
		}

#ifdef CRMATH_COLOR_D3DCOLOR

		inline D3DCOLOR getD3DCOLOR() const
		{		
			// Convert floats to uints
			uint ia = ftoi_fpu(a * 255.0);
			uint ir = ftoi_fpu(r * 255.0);
			uint ig = ftoi_fpu(g * 255.0);
			uint ib = ftoi_fpu(b * 255.0);

			// Truncate to last byte in case float outside 0.0-1.0 range
			//ia &= 0x000000FF;		// <<24 has same effect
			ir &= 0x000000FF;
			ig &= 0x000000FF;
			ib &= 0x000000FF;

			// Make D3DCOLOR (0xAARRGGBB)
			return (ia << 24) | (ir << 16) | (ig << 8) | ib;
		}

#endif //CRMATH_COLOR_D3DCOLOR

		COLORREF getCOLORREF() const
		{
			uint ir = ftoi_fpu(r * 255.0);
			uint ig = ftoi_fpu(g * 255.0);
			uint ib = ftoi_fpu(b * 255.0);
		
			return RGB(ir, ig, ib);
		}
	};

	// Convert D3DCOLOR to cr::color
#ifdef CRMATH_COLOR_D3DCOLOR
	template<typename T>
	color_base<T> color_from_D3DCOLOR(D3DCOLOR c)
	{
		uint ia = (c & 0xFF000000) >> 24;
		uint ir = (c & 0x00FF0000) >> 16;
		uint ig = (c & 0x0000FF00) >> 8;
		uint ib = (c & 0x000000FF);

		return color_base<T>(byte_to_float[ia], byte_to_float[ir], byte_to_float[ig], byte_to_float[ib]);
	}
#endif //CRMATH_COLOR_D3DCOLOR

		// SSE conversion
	#ifndef CRMATH_NO_SSE

	class sse_constants_t {
	public:
		__m128 m_255;
		__m128i m_FF;

		sse_constants_t()
		{
			const float _255 = 255.0f;
			m_255 = _mm_load1_ps(&_255);
			const int _FF = 0xFF;
			__m128 temp = _mm_load1_ps((float*)&_FF);
			m_FF = *(__m128i*)&temp;
		}
	};

	static const sse_constants_t sse_constants;

	inline D3DCOLOR sse2_D3DCOLOR_from_color(const color_base<float>& c)
	{
		/*
		const float _255 = 255.0f;
		const __m128 m_255 = _mm_load1_ps(&_255);
		const int _FF = 0xFF;
		const __m128 m_FF = _mm_load1_ps((float*)&_FF);
		*/

		__m128 as_floats = _mm_loadu_ps((float*)&c);
		__m128 as_ranged_floats = _mm_mul_ps(as_floats, sse_constants.m_255);

		__m128i as_integers = _mm_cvttps_epi32(as_ranged_floats);
		__m128i masked = _mm_and_si128(as_integers, sse_constants.m_FF);

		__declspec(align(16)) int ivals[4];

		_mm_store_si128((__m128i*)ivals, masked);

		return (ivals[0] << 24) | (ivals[1] << 16) | (ivals[2] << 8) | ivals[3];
	}
	#endif
}