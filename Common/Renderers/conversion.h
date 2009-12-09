 /////////////////////////////////////////////////////////////////////////////
 // Optimized type converting toolkit										//
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

// Implements:
//		byte_to_float[]: use a byte value (0-255) to convert byte to normalized float via lookup table
//		ftoi_fpu: convert float to int using x86 FPU instructions
//		ftoi_sse: convert float to int using SSE: check support_ftoi_sse before using
//		ftoi_fast: convert float to int using ftoi_sse where available otherwise ftoi_fpu
#pragma once

#ifndef CRMATH_NO_SSE
#include <emmintrin.h>		// SSE support
#endif

namespace cr {

	// Convert byte to normalized float 0.0 - 1.0 with lookup table
	template<typename T>
	class byte_to_float_lookup {
	public:
		T table[256];

		byte_to_float_lookup()
		{
			// Generate lookup table
			for (uint i = 0; i < 256; i++)
				table[i] = static_cast<T>(i) / static_cast<T>(255.0);
		}

		inline T operator[](uint index) const
		{
			CRASSERT(index < 256);
			return table[index];
		}
	};

	// Instances of lookup tables
	static const byte_to_float_lookup<float> byte_to_float;
	static const byte_to_float_lookup<double> byte_to_double;
	static const byte_to_float_lookup<cr_float> byte_to_crfloat;

	// Float->int with fistp x86 instruction, truncates due to above controlfp startup setting
	inline int ftoi_fpu(float f)
	{
		// Assembly is disabled? Use the built in slow cast
#ifdef CRMATH_NO_ASSEMBLY
		return (int)f;

		// Optimized version
#else
		int i;

		__asm
		{
			fld f
			fistp i
		}

		return i;
#endif
	}

	inline int ftoi_fpu(double f)
	{
		// Assembly is disabled? Use the built in slow cast
#ifdef CRMATH_NO_ASSEMBLY
		return (int)f;

		// Optimized version
#else
		int i;

		__asm
		{
			fld f
			fistp i
		}

		return i;
#endif
	}

#ifndef CRMATH_NO_SSE

	// Fast ftoi with SSE requires SSE1 for float or SSE2 for double
	inline int ftoi_sse2(double f)
	{
		return _mm_cvttsd_si32(_mm_load_sd(&f));	// SSE2 instructions for double->int
	}

	inline int ftoi_sse1(float f)
	{
		return _mm_cvtt_ss2si(_mm_load_ss(&f));		// SSE1 instructions for float->int
	}

	inline int ftoi_sse(cr_float f)
	{
	#ifdef CRMATH_CRFLOAT_DOUBLE
		return ftoi_sse2(f);
	#else
		return ftoi_sse1(f);
	#endif
	}

	#ifdef CRMATH_CRFLOAT_DOUBLE
		static const bool support_ftoi_sse = support_sse2;	// SSE2 for double->int
	#else
		static const bool support_ftoi_sse = support_sse1;	// SSE1 for float->int
	#endif

#endif //CRMATH_NO_SSE

	// Float->int using SSE where available.  Use the support_sse condition manually if calling multiple times.
	inline int ftoi_fast(cr_float f)
	{
#ifndef CRMATH_NO_SSE

		// Calls depending on double/float config for cr_float
		if (support_ftoi_sse)
			return ftoi_sse(f);

		else
#endif //CRMATH_NO_SSE
			return ftoi_fpu(f);
	}
}//cr