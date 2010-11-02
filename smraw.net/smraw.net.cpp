/*
 * Library function of libsmraw .net managed wrapper
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

#include <libsmraw.h>

#pragma managed( pop )

#include <vcclr.h>

#include "smraw.net.h"

#using <mscorlib.dll>

using namespace System;

namespace SMRaw {

System::String^ GetVersion( void )
{
	return( gcnew System::String(
		           libsmraw_get_version() ) );
}

System::Byte GetAccessFlagsRead( void )
{
	return( System::Byte(
		     (unsigned char) libsmraw_get_access_flags_read() ) );
}

System::Byte GetAccessFlagsReadWrite( void )
{
	return( System::Byte(
		     (unsigned char) libsmraw_get_access_flags_read_write() ) );
}

System::Byte GetAccessFlagsWrite( void )
{
	return( System::Byte(
		     (unsigned char) libsmraw_get_access_flags_write() ) );
}

array<System::String^>^ Glob( System::String^ filename )
{
	array<System::String^>^ filenames        = nullptr;
	libsmraw_error_t *error                  = NULL;
	wchar_t **smraw_filenames                = NULL;
	pin_ptr<const wchar_t> smraw_filename    = nullptr;
	System::String^ function                 = "Glob";
	size_t smraw_filename_length             = 0;
	int smraw_filename_index                 = 0;
	int smraw_number_of_filenames            = 0;

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
		/* TODO something with error */

		libsmraw_error_free(
		 &error );

		throw gcnew System::Exception(
		             "smraw.net " + function + ": unable to glob filenames." );
	}
	filenames = gcnew array<String^, 1>(
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
		/* TODO something with error */

		libsmraw_error_free(
		 &error );

		throw gcnew System::Exception(
			"smraw.net " + function + ": unable to free globbed filenames." );
	}
	return( filenames );
}

} // namespace SMRaw
