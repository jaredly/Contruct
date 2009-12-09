 /////////////////////////////////////////////////////////////////////////////
 // CRMath																	//
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

// Construct Runtime mathematics library, with vector/rect/quad math, optimizations etc
#pragma once

// Configuration defines
//CRMATH_NO_ASSEMBLY			// Disable inline assembly optimisations for faster conversion & calculation
//CRMATH_NO_SSE					// Disable SSE.  When enabled, only uses SSE intrinsics if the user CPU supports it.
//CRMATH_STRUCTTYPES_CONVERSION	// Enable conversions between cr::rect <-> RECT/RECTF, cr::point <->POINT/POINTF
//CRMATH_COLOR_D3DCOLOR			// Enable conversions between cr::color <-> D3DCOLOR
//CRMATH_CRFLOAT_DOUBLE			// Enable double precision floating point operations.

#include <float.h>				// _finite(), _isnan()

#ifdef CRMATH_NO_ASSEMBLY
#include <math.h>			// sin, cos for unoptimized sincosf
#endif

#ifndef CONSTRUCT

	#ifndef APPRUNTIME
		// Define cr::rect <-> RECTF, cr::point <-> POINTF conversion functions
		#define CRMATH_STRUCTTYPES_CONVERSION
	#endif

	// Define color/d3dcolor conversions
	#define CRMATH_COLOR_D3DCOLOR
#endif

#pragma warning(disable: 4305)	// 'initializing' : truncation from 'double' to 'const cr::cr_float'
								// We use doubles for constants in the source, and supress this warning for cr_float being single precision

namespace cr {

	// Floating point type used by CR library
#ifdef CRMATH_CRFLOAT_DOUBLE
	typedef double cr_float;
#else
	typedef float cr_float;
#endif

	typedef unsigned int uint;

	// Constants
	static const double pi = 3.1415926535897932384626433832795;
	static const double _2pi = pi * 2.0;
	static const double _180_pi = (180.0 / pi);

	// Math utilities
	template<typename T>
	inline T to_radians(T degrees)
	{
		return degrees / (T)_180_pi;
	}

	template<typename T>
	inline T to_degrees(T radians)
	{
		return radians * (T)_180_pi;
	}

	// Keep angle within (-pi, pi] range for optimal precision
	template<typename T>
	inline void clamp_angle(T& angle)
	{
		if (angle <= (T)-pi)
			angle += (T)_2pi;
		else if (angle > (T)pi)
			angle -= (T)_2pi;
	}

	// Clamp to (-180, 180] range
	template<typename T>
	inline void clamp_angle_degrees(T& angle)
	{
		if (angle <= -180.0)
			angle += 360.0;
		else if (angle > 180.0)
			angle -= 360.0;
	}

	// min and max #defines always conflict with std::min/max
	template<typename T>
	T cr_min(const T& a, const T& b)
	{	return a < b ? a : b;	}

	template<typename T>
	T cr_max(const T& a, const T& b)
	{	return a > b ? a : b;	}

	// Fast sin/cos calculation
	inline void sincosf(float angle, float* psin, float* pcos)
	{
		// Unoptimized version
#ifdef CRMATH_NO_ASSEMBLY
		*psin = sin(angle);
		*pcos = cos(angle);
#else	// Optimized version
	    __asm {
			mov eax, psin
			mov edx, pcos
			fld angle
			fsincos
			fstp DWORD ptr [edx]
			fstp DWORD ptr [eax]
		}
#endif //CRMATH_NO_ASSEMBLY
	}

	inline void sincosf(double angle, double* psin, double* pcos)
	{
		// Unoptimized version
#ifdef CRMATH_NO_ASSEMBLY
		*psin = sin(angle);
		*pcos = cos(angle);
#else	// Optimized version
	    __asm {
			mov eax, psin
			mov edx, pcos
			fld angle
			fsincos
			fstp QWORD ptr [edx]
			fstp QWORD ptr [eax]
		}
#endif //CRMATH_NO_ASSEMBLY
	}

	// Determine SSE/MMX support.
	inline uint get_cpu_feature_flags()
	{
		uint features;

	    __asm
        {
			// Save registers
            push	eax
            push	ebx
            push	ecx
            push	edx

			//
			// Get the feature flags (eax=1) from edx
			//
            mov		eax, 1
			cpuid
            mov		features, edx

			// Restore registers
            pop		edx
            pop		ecx
            pop		ebx
            pop		eax
        }

		return features;
	}

	static const uint cpu_feature_flags = get_cpu_feature_flags();
	static const bool support_mmx = (cpu_feature_flags & 0x00800000)!=0;		// MMX bit 23
	static const bool support_sse1 = (cpu_feature_flags & 0x02000000)!=0;		// SSE bit 25
	static const bool support_sse2 = (cpu_feature_flags & 0x04000000)!=0;		// SSE2 bit 26

}//cr

#include "crfloat.h"
#include "conversion.h"
#include "point.h"
#include "point3d.h"
#include "rect.h"
#include "quad.h"
#include "color.h"

#ifdef CRMATH_DEFAULT_TYPEDEFS
namespace cr {
	typedef point_base<cr_float> point;
	typedef point3d_base<cr_float> point3d;
	typedef rect_base<cr_float> rect;
	typedef quad_base<cr_float> quad;
	typedef color_base<cr_float> color;
}
#endif