 /////////////////////////////////////////////////////////////////////////////
 // 3D Point																//
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

// x,y,z point definition, effectively a vector3
#pragma once

namespace cr {

	template<typename T>
	class point3d_base {
	public:
		union {
			T x;
			T r;
		};
		union {
			T y;
			T g;
		};
		union {
			T z;
			T b;
		};

		// ctors
		//template<typename T>
		point3d_base()
		{}

		template<typename U>
		point3d_base(const point3d_base<U>& p)
		: x(p.x), y(p.y), z(p.z)
		{}

		template<typename U>
		point3d_base(const point_base<U>& p)
		: x(p.x), y(p.y), z(0.0)
		{}

		point3d_base(T _x, T _y, T _z)
		: x(_x), y(_y), z(_z)
		{}

		point3d_base<T>& operator =(const point3d_base<T>& p)
		{
			x = p.x;
			y = p.y;
			z = p.z;
			return (*this);
		}

		bool operator ==(const point3d_base<T>& p) const
		{
			return (x == p.x) && (y == p.y) && (z == p.z);
		}

		bool operator !=(const point3d_base<T>& p) const
		{
			return !(operator==(p));
		}

		point3d_base<T> operator *(T scalar) const
		{
			return point3d_base<T>(x * scalar, y * scalar, z * scalar);
		}

		point3d_base<T>& operator *=(T scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return (*this);
		}

		point3d_base<T> operator /(T scalar) const
		{
			return point(x / scalar, y / scalar, z / scalar);
		}

		point3d_base<T>& operator /=(T scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return (*this);
		}

		point3d_base<T> operator *(const point3d_base<T>& p) const
		{
			return point3d_base<T>(x * p.x, y * p.y, z * p.z);
		}

		point3d_base<T>& operator *=(const point3d_base<T>& p)
		{
			x *= p.x;
			y *= p.y;
			z *= p.z;
			return (*this);
		}

		point3d_base<T> operator /(const point3d_base<T>& p) const
		{
			return point(x / p.x, y / p.y, z / p.z);
		}

		point3d_base<T>& operator /=(const point3d_base<T>& p)
		{
			x /= p.x;
			y /= p.y;
			z /= p.z;
			return (*this);
		}

		point3d_base<T> operator +(const point3d_base<T>& p) const
		{
			return point3d_base<T>(x + p.x, y + p.y, z + p.z);
		}

		point3d_base<T>& operator +=(const point3d_base<T>& p)
		{
			x += p.x;
			y += p.y;
			z += p.z;
			return (*this);
		}

		point3d_base<T> operator -(const point3d_base<T>& p) const
		{
			return point3d_base<T>(x - p.x, y - p.y, z - p.z);
		}

		point3d_base<T>& operator -=(const point3d_base<T>& p)
		{
			x -= p.x;
			y -= p.y;
			z -= p.z
			return (*this);
		}

		T distance() const
		{
			return sqrt((x * x) + (y * y) + (z * z));
		}

		template<typename C>
		C convert() const
		{
			return C(x, y, z);
		}

		point_base<T> make_2d() const
		{
			return point_base<T>(x, y);
		}
	};

}//cr