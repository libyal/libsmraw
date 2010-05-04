/*
 * Storage media (SM) RAW library read testing program
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

#include <common.h>

#include <libcstring.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include <libsmraw.h>

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	char **filenames          = NULL;
	libsmraw_error_t *error   = NULL;
	libsmraw_handle_t *handle = NULL;
	off_t offset              = 1024;
	int amount_of_filenames   = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Filename missing.\n" );

		return( EXIT_FAILURE );
	}
	if( libsmraw_handle_initialize(
	     &handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create handle.\n" );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( libsmraw_handle_open(
	     handle,
	     filenames,
	     amount_of_filenames,
	     LIBSMRAW_OPEN_READ,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open file(s).\n" );

		libsmraw_handle_free(
		 &handle,
		 NULL );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( libsmraw_handle_seek_offset(
	     handle,
	     offset,
	     SEEK_SET,
	     &error ) == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to seek offset: %jd.\n",
		 (intmax_t) offset );

		libsmraw_handle_free(
		 &handle,
		 NULL );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( libsmraw_handle_close(
	     handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file(s).\n" );

		libsmraw_handle_free(
		 &handle,
		 NULL );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( libsmraw_handle_free(
	     &handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free handle.\n" );

		libsmraw_handle_free(
		 &handle,
		 NULL );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

