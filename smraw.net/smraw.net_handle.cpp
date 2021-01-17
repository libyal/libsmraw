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
#pragma managed( push, off )
#include <common.h>
#include <memory.h>

#include <stdlib.h>

#include <libsmraw.h>
#pragma managed( pop )

#include <vcclr.h>

#include "smraw.net.h"
#include "smraw.net_handle.h"

#using <mscorlib.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace SMRaw {

Handle::Handle( void )
{
	char smraw_error_string[ SMRAW_NET_ERROR_STRING_SIZE ];

	libsmraw_error_t *error      = NULL;
	libsmraw_handle_t *handle    = NULL;
	System::String^ error_string = nullptr;
	System::String^ function     = "Handle::Handle";

	if( libsmraw_handle_initialize(
	     &handle,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "smraw.net " + function + ": unable to create smraw handle." );

		if( libsmraw_error_backtrace_sprint(
		     error,
		     &( smraw_error_string[ 1 ] ),
		     SMRAW_NET_ERROR_STRING_SIZE - 1 ) > 0 )
		{
			smraw_error_string[ 0 ] = '\n';

			error_string = System::String::Concat(
			                error_string,
			                gcnew System::String(
			                       smraw_error_string ) );
		}
		libsmraw_error_free(
		 &error );

		throw gcnew System::Exception(
			     error_string );
	}
	this->smraw_handle = Marshal::ReadIntPtr(
	                      (IntPtr) &handle );
}

Handle::~Handle( void )
{
	char smraw_error_string[ SMRAW_NET_ERROR_STRING_SIZE ];

	libsmraw_error_t *error      = NULL;
	libsmraw_handle_t *handle    = NULL;
	System::String^ error_string = nullptr;
	System::String^ function     = "Handle::~Handle";

	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->smraw_handle );

	if( libsmraw_handle_free(
	     &handle,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "smraw.net " + function + ": unable to free smraw handle." );

		if( libsmraw_error_backtrace_sprint(
		     error,
		     &( smraw_error_string[ 1 ] ),
		     SMRAW_NET_ERROR_STRING_SIZE - 1 ) > 0 )
		{
			smraw_error_string[ 0 ] = '\n';

			error_string = System::String::Concat(
			                error_string,
			                gcnew System::String(
			                       smraw_error_string ) );
		}
		libsmraw_error_free(
		 &error );

		throw gcnew System::Exception(
			     error_string );
	}
}

int Handle::GetAccessFlagsRead( void )
{
	return( libsmraw_get_access_flags_read() );
}

int Handle::GetAccessFlagsReadWrite( void )
{
	return( libsmraw_get_access_flags_read_write() );
}

int Handle::GetAccessFlagsWrite( void )
{
	return( libsmraw_get_access_flags_write() );
}

array<System::String^>^ Handle::Glob( System::String^ filename )
{
	char smraw_error_string[ SMRAW_NET_ERROR_STRING_SIZE ];

	array<System::String^>^ filenames     = nullptr;
	libsmraw_error_t *error               = NULL;
	wchar_t **smraw_filenames             = NULL;
	pin_ptr<const wchar_t> smraw_filename = nullptr;
	System::String^ error_string          = nullptr;
	System::String^ function              = "SMRaw::Glob";
	size_t smraw_filename_length          = 0;
	int smraw_filename_index              = 0;
	int smraw_number_of_filenames         = 0;

	smraw_filename = PtrToStringChars(
	                  filename );

	smraw_filename_length = wcslen(
	                         smraw_filename );

	if( libsmraw_glob_wide(
	     smraw_filename,
	     smraw_filename_length,
	     &smraw_filenames,
	     &smraw_number_of_filenames,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "smraw.net " + function + ": unable to glob filenames." );

		if( libsmraw_error_backtrace_sprint(
		     error,
		     &( smraw_error_string[ 1 ] ),
		     SMRAW_NET_ERROR_STRING_SIZE - 1 ) > 0 )
		{
			smraw_error_string[ 0 ] = '\n';

			error_string = System::String::Concat(
			                error_string,
			                gcnew System::String(
			                       smraw_error_string ) );
		}
		libsmraw_error_free(
		 &error );

		throw gcnew System::Exception(
			     error_string );
	}
	filenames = gcnew array<System::String^, 1>(
	                   smraw_number_of_filenames );

	for( smraw_filename_index = 0;
	     smraw_filename_index < smraw_number_of_filenames;
	     smraw_filename_index++ )
	{
		filenames[ smraw_filename_index ] = gcnew System::String(
		                                           smraw_filenames[ smraw_filename_index ] );
	}
	if( libsmraw_glob_wide_free(
	     smraw_filenames,
	     smraw_number_of_filenames,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "smraw.net " + function + ": unable to free globbed filenames." );

		if( libsmraw_error_backtrace_sprint(
		     error,
		     &( smraw_error_string[ 1 ] ),
		     SMRAW_NET_ERROR_STRING_SIZE - 1 ) > 0 )
		{
			smraw_error_string[ 0 ] = '\n';

			error_string = System::String::Concat(
			                error_string,
			                gcnew System::String(
			                       smraw_error_string ) );
		}
		libsmraw_error_free(
		 &error );

		throw gcnew System::Exception(
			     error_string );
	}
	return( filenames );
}

void Handle::Open( array<System::String^>^ filenames,
                   int access_flags )
{
	char smraw_error_string[ SMRAW_NET_ERROR_STRING_SIZE ];

	libsmraw_handle_t *handle       = NULL;
	libsmraw_error_t *error         = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "Handle::Open";
	wchar_t **smraw_filenames       = NULL;
	pin_ptr<const wchar_t> filename = nullptr;
	int smraw_filename_index        = 0;
	int smraw_number_of_filenames   = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->smraw_handle );

	smraw_number_of_filenames = filenames->Length;

	if( smraw_number_of_filenames <= 0 )
	{
		throw gcnew System::Exception(
			"smraw.net " + function + ": missing filenames." );
	}
	smraw_filenames = (wchar_t **) memory_allocate(
	                                sizeof( wchar_t* ) * smraw_number_of_filenames );

	if( smraw_filenames == NULL )
	{
		throw gcnew System::Exception(
		             "smraw.net " + function + ": unable to create filenames." );
	}
	for( smraw_filename_index = 0;
	     smraw_filename_index < smraw_number_of_filenames;
	     smraw_filename_index++ )
	{
		filename = PtrToStringChars(
		            filenames[ smraw_filename_index ] );

		smraw_filenames[ smraw_filename_index ] = (wchar_t *) filename;
	}
	if( libsmraw_handle_open_wide(
	     handle,
	     (wchar_t * const *) smraw_filenames,
	     smraw_number_of_filenames,
	     access_flags,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "smraw.net " + function + ": unable to open smraw handle." );

		if( libsmraw_error_backtrace_sprint(
		     error,
		     &( smraw_error_string[ 1 ] ),
		     SMRAW_NET_ERROR_STRING_SIZE - 1 ) > 0 )
		{
			smraw_error_string[ 0 ] = '\n';

			error_string = System::String::Concat(
			                error_string,
			                gcnew System::String(
			                       smraw_error_string ) );
		}
		libsmraw_error_free(
		 &error );

		memory_free(
		 smraw_filenames );

		throw gcnew System::Exception(
			     error_string );
	}
	memory_free(
	 smraw_filenames );
}

void Handle::Close( void )
{
	char smraw_error_string[ SMRAW_NET_ERROR_STRING_SIZE ];

	libsmraw_error_t *error      = NULL;
	libsmraw_handle_t *handle    = NULL;
	System::String^ error_string = nullptr;
	System::String^ function     = "Handle::Close";

	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->smraw_handle );

	if( libsmraw_handle_close(
	     handle,
	     &error ) != 0 )
	{
		error_string = gcnew System::String(
		                      "smraw.net " + function + ": unable to close smraw handle." );

		if( libsmraw_error_backtrace_sprint(
		     error,
		     &( smraw_error_string[ 1 ] ),
		     SMRAW_NET_ERROR_STRING_SIZE - 1 ) > 0 )
		{
			smraw_error_string[ 0 ] = '\n';

			error_string = System::String::Concat(
			                error_string,
			                gcnew System::String(
			                       smraw_error_string ) );
		}
		libsmraw_error_free(
		 &error );

		throw gcnew System::Exception(
			     error_string );
	}
}

int Handle::ReadBuffer( array<System::Byte>^ buffer,
                        int size )
{
	char smraw_error_string[ SMRAW_NET_ERROR_STRING_SIZE ];

	libsmraw_error_t *error       = NULL;
	libsmraw_handle_t *handle     = NULL;
	System::String^ error_string  = nullptr;
	System::String^ function      = "Handle::ReadBuffer";
	pin_ptr<uint8_t> smraw_buffer = nullptr;
	size_t read_count             = 0;

	if( size < 0 )
	{
		throw gcnew System::ArgumentException(
			     "smraw.net " + function + ": invalid size" );
	}
	if( size == 0 )
	{
		return( 0 );
	}
	if( size > buffer->Length )
	{
		throw gcnew System::ArgumentException(
			     "smraw.net " + function + ": buffer too small" );
	}
	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->smraw_handle );

	smraw_buffer = &( buffer[ 0 ] );

	read_count = libsmraw_handle_read_buffer(
	              handle,
	              smraw_buffer,
	              (size_t) size,
	              &error );

	if( read_count == -1 )
	{
		error_string = gcnew System::String(
		                      "smraw.net " + function + ": unable to read buffer from smraw handle." );

		if( libsmraw_error_backtrace_sprint(
		     error,
		     &( smraw_error_string[ 1 ] ),
		     SMRAW_NET_ERROR_STRING_SIZE - 1 ) > 0 )
		{
			smraw_error_string[ 0 ] = '\n';

			error_string = System::String::Concat(
			                error_string,
			                gcnew System::String(
			                       smraw_error_string ) );
		}
		libsmraw_error_free(
		 &error );

		throw gcnew System::Exception(
			     error_string );
	}
	return( (int) read_count );
}

int Handle::WriteBuffer( array<System::Byte>^ buffer,
                         int size )
{
	char smraw_error_string[ SMRAW_NET_ERROR_STRING_SIZE ];

	libsmraw_error_t *error             = NULL;
	libsmraw_handle_t *handle           = NULL;
	System::String^ error_string        = nullptr;
	System::String^ function            = "Handle::WriteBuffer";
	pin_ptr<const uint8_t> smraw_buffer = nullptr;
	size_t write_count                  = 0;

	if( size < 0 )
	{
		throw gcnew System::ArgumentException(
			     "smraw.net " + function + ": invalid size" );
	}
	if( size == 0 )
	{
		return( 0 );
	}
	if( size > buffer->Length )
	{
		throw gcnew System::ArgumentException(
			     "smraw.net " + function + ": buffer too small" );
	}
	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->smraw_handle );

	smraw_buffer = &( buffer[ 0 ] );

	write_count = libsmraw_handle_write_buffer(
	               handle,
	               smraw_buffer,
	               (size_t) size,
	               &error );

	if( write_count == -1 )
	{
		error_string = gcnew System::String(
		                      "smraw.net " + function + ": unable to write buffer to smraw handle." );

		if( libsmraw_error_backtrace_sprint(
		     error,
		     &( smraw_error_string[ 1 ] ),
		     SMRAW_NET_ERROR_STRING_SIZE - 1 ) > 0 )
		{
			smraw_error_string[ 0 ] = '\n';

			error_string = System::String::Concat(
			                error_string,
			                gcnew System::String(
			                       smraw_error_string ) );
		}
		libsmraw_error_free(
		 &error );

		throw gcnew System::Exception(
			     error_string );
	}
	return( (int) write_count );
}

System::Int64 Handle::SeekOffset( System::Int64 offset,
                                  System::IO::SeekOrigin origin )
{
	char smraw_error_string[ SMRAW_NET_ERROR_STRING_SIZE ];

	libsmraw_error_t *error      = NULL;
	libsmraw_handle_t *handle    = NULL;
	System::String^ error_string = nullptr;
	System::String^ function     = "Handle::SeekOffset";
	off64_t smraw_offset         = 0;
	int smraw_whence             = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->smraw_handle );

	Marshal::WriteInt64(
	 (IntPtr) &smraw_offset,
	 offset );

	if( origin == System::IO::SeekOrigin::Begin )
	{
		smraw_whence = SEEK_SET;
	}
	else if( origin == System::IO::SeekOrigin::Current )
	{
		smraw_whence = SEEK_CUR;
	}
	else if( origin == System::IO::SeekOrigin::End )
	{
		smraw_whence = SEEK_END;
	}
	else
	{
		throw gcnew System::ArgumentException(
			     "smraw.net " + function + ": unsupported origin" );
	}
	smraw_offset = libsmraw_handle_seek_offset(
	                handle,
	                smraw_offset,
	                smraw_whence,
	                &error );

	if( smraw_offset == -1 )
	{
		error_string = gcnew System::String(
		                      "smraw.net " + function + ": unable to seek offset in smraw handle." );

		if( libsmraw_error_backtrace_sprint(
		     error,
		     &( smraw_error_string[ 1 ] ),
		     SMRAW_NET_ERROR_STRING_SIZE - 1 ) > 0 )
		{
			smraw_error_string[ 0 ] = '\n';

			error_string = System::String::Concat(
			                error_string,
			                gcnew System::String(
			                       smraw_error_string ) );
		}
		libsmraw_error_free(
		 &error );

		throw gcnew System::Exception(
			     error_string );
	}
	offset = Marshal::ReadInt64(
	          (IntPtr) &smraw_offset );

	return( offset );
}

System::Int64 Handle::GetOffset( void )
{
	char smraw_error_string[ SMRAW_NET_ERROR_STRING_SIZE ];

	libsmraw_error_t *error      = NULL;
	libsmraw_handle_t *handle    = NULL;
	System::String^ error_string = nullptr;
	System::String^ function     = "Handle::GetOffset";
	off64_t smraw_offset         = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->smraw_handle );

	if( libsmraw_handle_get_offset(
	     handle,
	     &smraw_offset,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "smraw.net " + function + ": unable to retrieve offset from smraw handle." );

		if( libsmraw_error_backtrace_sprint(
		     error,
		     &( smraw_error_string[ 1 ] ),
		     SMRAW_NET_ERROR_STRING_SIZE - 1 ) > 0 )
		{
			smraw_error_string[ 0 ] = '\n';

			error_string = System::String::Concat(
			                error_string,
			                gcnew System::String(
			                       smraw_error_string ) );
		}
		libsmraw_error_free(
		 &error );

		throw gcnew System::Exception(
			     error_string );
	}
	return( Marshal::ReadInt64(
	         (IntPtr) &smraw_offset ) );
}

System::UInt64 Handle::GetMediaSize( void )
{
	char smraw_error_string[ SMRAW_NET_ERROR_STRING_SIZE ];

	libsmraw_error_t *error      = NULL;
	libsmraw_handle_t *handle    = NULL;
	System::String^ error_string = nullptr;
	System::String^ function     = "Handle::GetMediaSize";
	size64_t smraw_media_size    = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->smraw_handle );

	if( libsmraw_handle_get_media_size(
	     handle,
	     &smraw_media_size,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "smraw.net " + function + ": unable to retrieve media size from smraw handle." );

		if( libsmraw_error_backtrace_sprint(
		     error,
		     &( smraw_error_string[ 1 ] ),
		     SMRAW_NET_ERROR_STRING_SIZE - 1 ) > 0 )
		{
			smraw_error_string[ 0 ] = '\n';

			error_string = System::String::Concat(
			                error_string,
			                gcnew System::String(
			                       smraw_error_string ) );
		}
		libsmraw_error_free(
		 &error );

		throw gcnew System::Exception(
			     error_string );
	}
	return( Marshal::ReadInt64(
	         (IntPtr) &smraw_media_size ) );
}

} // namespace SMRaw

