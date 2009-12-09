 /////////////////////////////////////////////////////////////////////////////
 // Quad																	//
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

// Arbitrary quadrilateral shape defined by four points
#pragma once

namespace cr {

	template<typename T>
	class quad_base {
	public:
		typedef point_base<T> qpoint;

		qpoint tl;	// top-left
		qpoint tr;	// top-right
		qpoint bl;	// bottom-left
		qpoint br;	// bottom-right

		quad_base()
		{}

		template<typename U>
		quad_base(const quad_base<U>& q)
			: tl(q.tl), tr(q.tr), bl(q.bl), br(q.br)
		{}

		quad_base(qpoint _tl, qpoint _tr, qpoint _bl, qpoint _br)
			: tl(_tl), tr(_tr), bl(_bl), br(_br)
		{}
		quad_base(const rect_base<T>& r)
			: tl(r.left, r.top), tr(r.right, r.top), bl(r.left, r.bottom), br(r.right, r.bottom)
		{}

		template<typename U>
		quad_base& operator =(const quad_base<U>& q)
		{
			tl = q.tl;
			tr = q.tr;
			bl = q.bl;
			br = q.br;
			return (*this);
		}

		template<typename U>
		bool operator ==(const quad_base<U>& q) const
		{
			return (tl == q.tl) && (tr == q.tr) && (bl == q.bl) && (br == q.br);
		}

		template<typename U>
		bool operator !=(const quad_base<U>& q) const
		{
			return !(operator ==(q));
		}

		quad_base<T> operator +(const qpoint& p) const
		{
			return quad(tl + p, tr + p, bl + p, br + p);
		}

		quad_base<T>& operator +=(const qpoint& p)
		{
			tl += p;
			tr += p;
			bl += p;
			br += p;
			return (*this);
		}

		quad_base<T> operator -(const qpoint& p) const
		{
			return quad<T>(tl - p, tr - p, bl - p, br - p);
		}

		quad_base<T>& operator -=(const qpoint& p)
		{
			tl -= p;
			tr -= p;
			bl -= p;
			br -= p;
			return (*this);
		}

		rect_base<T> boundingbox() const
		{
			return rect_base<T>(
				cr_min(cr_min(tl.x, tr.x), cr_min(bl.x, br.x)),		// left
				cr_min(cr_min(tl.y, tr.y), cr_min(bl.y, br.y)),		// top
				cr_max(cr_max(tl.x, tr.x), cr_max(bl.x, br.x)),		// right
				cr_max(cr_max(tl.y, tr.y), cr_max(bl.y, br.y))		// bottom
				);
		}

		void rotate(T rads)
		{
			if (rads == 0.0)
				return;

			T sin_a;
			T cos_a;
			sincosf(rads, &sin_a, &cos_a);

			T x_temp = (tl.x * cos_a) - (tl.y * sin_a);
			tl.y = (tl.y * cos_a) + (tl.x * sin_a);
			tl.x = x_temp;

			x_temp = (tr.x * cos_a) - (tr.y * sin_a);
			tr.y = (tr.y * cos_a) + (tr.x * sin_a);
			tr.x = x_temp;

			x_temp = (bl.x * cos_a) - (bl.y * sin_a);
			bl.y = (bl.y * cos_a) + (bl.x * sin_a);
			bl.x = x_temp;

			x_temp = (br.x * cos_a) - (br.y * sin_a);
			br.y = (br.y * cos_a) + (br.x * sin_a);
			br.x = x_temp;
		}

		void rotate(T rads, const qpoint& p)
		{
			(*this) -= p;
			rotate(rads);
			(*this) += p;
		}

		void rotate(T rads, T orig_x, T orig_y)
		{
			rotate(rads, qpoint(orig_x, orig_y));
		}

		void elliptical_rotate(T rads, T orig_x, T orig_y, T ratio)
		{
			tl.elliptical_rotate(rads, orig_x, orig_y, ratio);
			tr.elliptical_rotate(rads, orig_x, orig_y, ratio);
			bl.elliptical_rotate(rads, orig_x, orig_y, ratio);
			br.elliptical_rotate(rads, orig_x, orig_y, ratio);
		}

		void skew(T x_skew, T y_skew)
		{
			tl.x += x_skew;
			tr.x += x_skew;
			tr.y += y_skew;
			br.y += y_skew;
		}
	};
}