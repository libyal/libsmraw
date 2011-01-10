/*
 * Handle class of libsmraw .net managed wrapper
 *
 * Copyright (c) 2010-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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
using namespace System::IO;

namespace SMRaw {

public ref class Handle sealed
{
	private:
		System::IntPtr smraw_handle;

	public:
		Handle( void );
		~Handle( void );

		System::Int GetAccessFlagsRead( void );
		System::Int GetAccessFlagsReadWrite( void );
		System::Int GetAccessFlagsWrite( void );

		array<System::String^>^ Glob( System::String^ filename );

		void Open( array<System::String^>^ filenames,
		           System::Int access_flags );

		void Close( void );

		int ReadBuffer( array<System::Byte>^ buffer, int size );

		int WriteBuffer( array<System::Byte>^ buffer, int size );

		System::Int64 SeekOffset( System::Int64 offset, System::IO::SeekOrigin origin );

		System::Int64 GetOffset( void );

		System::UInt64 GetMediaSize( void );
};

} // namespace SMRaw

