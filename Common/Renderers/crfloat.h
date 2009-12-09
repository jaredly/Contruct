 /////////////////////////////////////////////////////////////////////////////
 // CRFloat																	//
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

// Floating point utilities
#pragma once

namespace cr {
	bool is_finite(double value)
	{
		return _finite(value)!=0;
	}

	bool is_nan(double value)
	{
		return _isnan(value)!=0;
	}

	// Kahan summation wrapper
	template<typename FloatType = double>
	class KahanCounter {
	protected:
		FloatType c, y, t, sum;
	public:
		KahanCounter() : c(0.0), sum(0.0) {}
		
		void add(FloatType v)
		{
			y = v - c;
			t = sum + y;
			c = (t - sum) - y;
			sum = t;
		}

		FloatType total() const
		{
			return sum;
		}
	};
}