 /////////////////////////////////////////////////////////////////////////////
 // Rect																	//
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

// Axis-aligned rect_baseangle definition
#pragma once

namespace cr {

	template<typename T>
	class quad_base;

	template<typename T>
	class rect_base {
	public:
		T left;
		T top;
		T right;
		T bottom;

		rect_base()
		{}

		template<typename U>
		rect_base(const rect_base<U>& r)
			: left(r.left), top(r.top), right(r.right), bottom(r.bottom)
		{}

		rect_base(T _left, T _top, T _right, T _bottom)
			: left(_left), top(_top), right(_right), bottom(_bottom)
		{}

		// Conversion to/from RECTF struct
#ifdef CRMATH_STRUCTTYPES_CONVERSION
		rect_base(const RECTF& rf)
			: left(rf.left), top(rf.top), right(rf.right), bottom(rf.bottom) {}

		operator RECTF() const {
			RECTF rf;
			rf.left = left;
			rf.top = top;
			rf.right = right;
			rf.bottom = bottom;
			return rf;
		}
#endif	//CRMATH_STRUCTTYPES_CONVERSION

		rect_base<T>& operator =(const rect_base<T>& r)
		{
			left = r.left;
			top = r.top;
			right = r.right;
			bottom = r.bottom;
			return (*this);
		}

		bool operator ==(const rect_base<T>& r) const
		{
			return (left == r.left) && (top == r.top) && (right == r.right) && (bottom == r.bottom);
		}

		bool operator !=(const rect_base<T>& r) const
		{
			return !(operator ==(r));
		}

		// Translate rectangle
		template<typename U>
		rect_base<T> operator +(const point_base<U>& p) const
		{
			return rect_base<T>(left + p.x, top + p.y, right + p.x, bottom + p.y);
		}

		template<typename U>
		rect_base<T>& operator +=(const point_base<U>& p)
		{
			left += p.x;
			top += p.y;
			right += p.x;
			bottom += p.y;
			return (*this);
		}

		template<typename U>
		rect_base<T> operator -(const point_base<U>& p) const
		{
			return rect_base<T>(left - p.x, top - p.y, right - p.x, bottom - p.y);
		}

		template<typename U>
		rect_base<T>& operator -=(const point_base<U>& p)
		{
			left -= p.x;
			top -= p.y;
			right -= p.x;
			bottom -= p.y;
			return (*this);
		}

		template<typename U>
		rect_base<T> operator *(const point_base<U>& p) const
		{
			return rect_base<T>(left * p.x, top * p.y, right * p.x, bottom * p.y);
		}

		template<typename U>
		rect_base<T>& operator *=(const point_base<U>& p)
		{
			left *= p.x;
			top *= p.y;
			right *= p.x;
			bottom *= p.y;
			return (*this);
		}

		template<typename U>
		rect_base<T> operator /(const point_base<U>& p) const
		{
			return rect_base<T>(left / p.x, top / p.y, right / p.x, bottom / p.y);
		}

		template<typename U>
		rect_base<T>& operator /=(const point_base<U>& p)
		{
			left /= p.x;
			top /= p.y;
			right /= p.x;
			bottom /= p.y;
			return (*this);
		}

		// Size functions
		void inflate(T amt)
		{
			left -= amt;
			top -= amt;
			right += amt;
			bottom += amt;
		}

		void deflate(T amt)
		{
			left += amt;
			top += amt;
			right -= amt;
			bottom -= amt;
		}

		T width() const
		{
			return right - left;
		}

		T height() const
		{
			return bottom - top;
		}

		point_base<T> topleft() const
		{
			return point_base<T>(left, top);
		}

		point_base<T> topright() const
		{
			return point_base<T>(right, top);
		}

		point_base<T> bottomleft() const
		{
			return point_base<T>(left, bottom);
		}

		point_base<T> bottomright() const
		{
			return point_base<T>(right, bottom);
		}

		quad_base<T> rotate_to_quad(T rads) const
		{
			// Avoid calculation when no rotation
			if (rads == 0.0)
				return quad_base<T>(topleft(), topright(), bottomleft(), bottomright());

			cr_float sin_a;
			cr_float cos_a;
			sincosf(rads, &sin_a, &cos_a);

			cr_float left_sin_a = left * sin_a;
			cr_float top_sin_a = top * sin_a;
			cr_float right_sin_a = right * sin_a;
			cr_float bottom_sin_a = bottom * sin_a;

			cr_float left_cos_a = left * cos_a;
			cr_float top_cos_a = top * cos_a;
			cr_float right_cos_a = right * cos_a;
			cr_float bottom_cos_a = bottom * cos_a;

			return quad_base<T>(point_base<T>(left_cos_a - top_sin_a, top_cos_a + left_sin_a),
						point_base<T>(right_cos_a - top_sin_a, top_cos_a + right_sin_a),
						point_base<T>(left_cos_a - bottom_sin_a, bottom_cos_a + left_sin_a),
						point_base<T>(right_cos_a - bottom_sin_a, bottom_cos_a + right_sin_a)
					);
		}

		quad_base<T> rotate_to_quad(T rads, const point_base<T>& p) const
		{
			const rect_base<T> r((*this) - p);
			quad_base<T> q(r.rotate_to_quad(rads));
			q += p;
			return q;
		}

		quad_base<T> rotate_to_quad(T rads, T orig_x, T orig_y) const
		{
			return rotate_to_quad(rads, point_base<T>(orig_x, orig_y));
		}
	};

	template<typename T>
	rect_base<T> rect_xywh(T x, T y, T width, T height)
	{
		return rect_base<T>(x, y, x + width, y + height);
	}
}//cr