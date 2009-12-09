 /////////////////////////////////////////////////////////////////////////////
 // Point																	//
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

// x,y point definition, effectively a vector2
#pragma once

namespace cr {

	template<typename T>
	class rect_base; // forwards decl for make_rect()

	template<typename T>
	class point3d_base;	// forwards decl for make_3d()

	template<typename T>
	class point_base {
	public:
		union {
			T x;
			T u;
		};
		union {
			T y;
			T v;
		};

		// ctors
		//template<typename T>
		point_base()
		{}

		template<typename U>
		point_base(const point_base<U>& p)
		: x(p.x), y(p.y)
		{}

		point_base(T _x, T _y)
		: x(_x), y(_y)
		{}

		// Conversion to/from POINTF struct
#ifdef CRMATH_STRUCTTYPES_CONVERSION
		point_base(const POINTF& pf)
		: x(pf.x), y(pf.y)
		{}

		operator POINTF() const
		{
			POINTF pf;
			pf.x = x;
			pf.y = y;
			return pf;
		}
#endif //CRMATH_STRUCTTYPES_CONVERSION

		point_base<T>& operator =(const point_base<T>& p)
		{
			x = p.x;
			y = p.y;
			return (*this);
		}

		bool operator ==(const point_base<T>& p) const
		{
			return (x == p.x) && (y == p.y);
		}

		bool operator !=(const point_base<T>& p) const
		{
			return !(operator==(p));
		}

		point_base<T> operator *(T scalar) const
		{
			return point_base<T>(x * scalar, y * scalar);
		}

		point_base<T>& operator *=(T scalar)
		{
			x *= scalar;
			y *= scalar;
			return (*this);
		}

		point_base<T> operator *(const point_base<T>& p) const
		{
			return point_base<T>(x * p.x, y * p.y);
		}

		point_base<T>& operator *=(const point_base<T>& p)
		{
			x *= p.x;
			y *= p.y;
			return (*this);
		}

		point_base<T> operator /(T scalar) const
		{
			return point_base(x / scalar, y / scalar);
		}

		point_base<T>& operator /=(T scalar)
		{
			x /= scalar;
			y /= scalar;
			return (*this);
		}

		point_base<T> operator /(const point_base<T>& p) const
		{
			return point_base<T>(x / p.x, y / p.y);
		}

		point_base<T>& operator /=(const point_base<T>& p)
		{
			x /= p.x;
			y /= p.y;
			return (*this);
		}

		point_base<T> operator +(const point_base<T>& p) const
		{
			return point_base<T>(x + p.x, y + p.y);
		}

		point_base<T>& operator +=(const point_base<T>& p)
		{
			x += p.x;
			y += p.y;
			return (*this);
		}

		point_base<T> operator -(const point_base<T>& p) const
		{
			return point_base<T>(x - p.x, y - p.y);
		}

		point_base<T>& operator -=(const point_base<T>& p)
		{
			x -= p.x;
			y -= p.y;
			return (*this);
		}

		T angle() const
		{
			// Angle relative to 0,0
			return atan2(y, x);
		}

		T distance() const
		{
			return sqrt((x * x) + (y * y));
		}

		void rotate(T rads)
		{
			if (rads == 0.0)
				return;

			cr_float sin_a;
			cr_float cos_a;
			sincosf(rads, &sin_a, &cos_a);

			cr_float x_temp = (x * cos_a) - (y * sin_a);
			y = (y * cos_a) + (x * sin_a);
			x = x_temp;
		}

		void rotate(T rads, const point_base<T>& p)
		{
			(*this) -= p;
			rotate(rads);
			(*this) += p;
		}

		void rotate(T rads, T orig_x, T orig_y)
		{
			rotate(rads, point_base<T>(orig_x, orig_y));
		}

		void elliptical_rotate(T rads, T ratio)
		{
			y *= ratio;
			rotate(rads);
			y /= ratio;
		}

		void elliptical_rotate(T rads, const point_base<T>& p, T ratio)
		{
			(*this) -= p;
			y *= ratio;
			rotate(rads);
			y /= ratio;
			(*this) += p;
		}

		void elliptical_rotate(T rads, T orig_x, T orig_y, T ratio)
		{
			elliptical_rotate(rads, point_base<T>(orig_x, orig_y), ratio);
		}

		void move(T rads, T distance)
		{
			cr_float sin_a;
			cr_float cos_a;
			sincosf(rads, &sin_a, &cos_a);

			x += cos_a * distance;
			y += sin_a * distance;
		}

		point_base<T> make_rotated(T rads) const
		{
			point_base<T> temp(*this);
			temp.rotate(rads);
			return temp;
		}

		rect_base<T> make_rect() const
		{
			return rect_base<T>(0.0, 0.0, x, y);
		}

		point3d_base<T> make_3d(T zval = 0.0) const
		{
			return point3d_base<T>(x, y, zval);
		}

		template<typename C>
		C convert() const
		{
			return C(x, y);
		}
	};

}//cr