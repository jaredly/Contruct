 /////////////////////////////////////////////////////////////////////////////
 // Construct Runtime library - Windows operating system implementation		//
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
#pragma once

#include <windows.h>

namespace cr {

	namespace os {

		// Performance timing
		typedef __int64		timer_t;

		inline timer_t		GetPerformanceTimerFrequency()
		{
			timer_t ret;
			QueryPerformanceFrequency((LARGE_INTEGER*)&ret);
			return ret;
		}

		inline timer_t		GetPerformanceTimerCounter()
		{
			timer_t ret;
			QueryPerformanceCounter((LARGE_INTEGER*)&ret);
			return ret;
		}

	}
}