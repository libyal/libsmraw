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
#pragma managed( push, off )
#include <common.h>

#include <stdlib.h>

#include <libcstring.h>

#include <libsmraw.h>
#pragma managed( pop )

#include "smraw.net_handle.h"

#using <mscorlib.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace SMRaw {

Handle::Handle( void )
{
	libsmraw_handle_t *handle = NULL;
	libsmraw_error_t *error   = NULL;

	if( libsmraw_handle_initialize(
		 &handle,
		 &error ) != 1 )
	{
		/* TODO something with error */

		libsmraw_error_free(
		 &error );
	}
	this->smraw_handle = Marshal::ReadIntPtr(
	                      IntPtr( handle ) );
}

Handle::~Handle( void )
{
	libsmraw_handle_t *handle = NULL;
	libsmraw_error_t *error   = NULL;

	Marshal::WriteIntPtr(
	 IntPtr( handle ),
	 this->smraw_handle );

	if( libsmraw_handle_free(
		 &handle,
		 &error ) != 1 )
	{
		/* TODO something with error */

		libsmraw_error_free(
		 &error );
	}
}

void Handle::Open( array<System::String^>^ filenames,
                   System::Byte access_flags )
{
	libsmraw_handle_t *handle     = NULL;
	libsmraw_error_t *error       = NULL;
	wchar_t **smraw_filenames     = NULL;
	uint8_t smraw_access_flags    = 0;
	int smraw_number_of_filenames = 0;

	Marshal::WriteIntPtr(
	 IntPtr( handle ),
	 this->smraw_handle );

	/* TODO filenames */

	Marshal::WriteByte(
	 IntPtr( smraw_access_flags ),
	 access_flags );

	if( libsmraw_handle_open_wide(
		 handle,
		 (wchar_t * const *) smraw_filenames,
		 smraw_number_of_filenames,
		 smraw_access_flags,
		 &error ) != 1 )
	{
		/* TODO something with error */

		libsmraw_error_free(
		 &error );
	}
}

void Handle::Close( void )
{
	libsmraw_handle_t *handle = NULL;
	libsmraw_error_t *error   = NULL;

	Marshal::WriteIntPtr(
	 IntPtr( handle ),
	 this->smraw_handle );

	if( libsmraw_handle_close(
		 handle,
		 &error ) != 1 )
	{
		/* TODO something with error */

		libsmraw_error_free(
		 &error );
	}
}

} // namespace SMRaw
