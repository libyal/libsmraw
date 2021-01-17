/*
 * Handle class of libsmraw .net managed wrapper
 *
 * Copyright (C) 2010-2021, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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

		int GetAccessFlagsRead( void );
		int GetAccessFlagsReadWrite( void );
		int GetAccessFlagsWrite( void );

		array<System::String^>^ Glob( System::String^ filename );

		void Open( array<System::String^>^ filenames,
		           int access_flags );

		void Close( void );

		int ReadBuffer( array<System::Byte>^ buffer, int size );

		int WriteBuffer( array<System::Byte>^ buffer, int size );

		System::Int64 SeekOffset( System::Int64 offset, System::IO::SeekOrigin origin );

		System::Int64 GetOffset( void );

		System::UInt64 GetMediaSize( void );
};

} // namespace SMRaw

