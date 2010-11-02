/*
 * Handle class of libsmraw .net managed wrapper
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#using <mscorlib.dll>

using namespace System;

#pragma managed( push, off )
#include <libsmraw.h>
#pragma managed( pop )

namespace SMRaw {

public ref class Handle sealed
{
	private:
		System::IntPtr smraw_handle;

	public:
		Handle( void );
		~Handle( void );

		void Open( array<System::String^>^ filenames,
			       System::Byte access_flags );

		void Close( void );
};

} // namespace SMRaw
