/*
 * Library seek testing program
 *
 * Copyright (c) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "smraw_test_libcerror.h"
#include "smraw_test_libcstring.h"
#include "smraw_test_libsmraw.h"
#include "smraw_test_unused.h"

/* Define to make smraw_test_seek generate verbose output
#define SMRAW_TEST_SEEK_VERBOSE
 */

/* Tests libsmraw_handle_seek_offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_seek_offset(
     libsmraw_handle_t *handle,
     off64_t input_offset,
     int input_whence,
     off64_t output_offset )
{
	libcerror_error_t *error  = NULL;
	const char *whence_string = NULL;
	off64_t result_offset     = 0;
	int result                = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	if( input_whence == SEEK_CUR )
	{
		whence_string = "SEEK_CUR";
	}
	else if( input_whence == SEEK_END )
	{
		whence_string = "SEEK_END";
	}
	else if( input_whence == SEEK_SET )
	{
		whence_string = "SEEK_SET";
	}
	else
	{
		whence_string = "UNKNOWN";
	}
	fprintf(
	 stdout,
	 "Testing seek of offset: %" PRIi64 " and whence: %s\t",
	 input_offset,
	 whence_string );

	result_offset = libsmraw_handle_seek_offset(
	                 handle,
	                 input_offset,
	                 input_whence,
	                 &error );

	if( result_offset == output_offset )
	{
		result = 1;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( error != NULL )
	{
		if( result != 1 )
		{
			libsmraw_error_backtrace_fprint(
			 error,
			 stderr );
		}
		libsmraw_error_free(
		 &error );
	}
	return( result );
}

/* Tests seeking in a handle
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_seek(
     libsmraw_handle_t *handle,
     size64_t media_size )
{
	int result = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	if( media_size > (size64_t) INT64_MAX )
	{
		fprintf(
		 stderr,
		 "Media size exceeds maximum.\n" );

		return( -1 );
	}
	/* Test: SEEK_SET offset: 0
	 * Expected result: 0
	 */
	result = smraw_test_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          0 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_SET offset: <media_size>
	 * Expected result: <media_size>
	 */
	result = smraw_test_seek_offset(
	          handle,
	          (off64_t) media_size,
	          SEEK_SET,
	          (off64_t) media_size );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_SET offset: <media_size / 5>
	 * Expected result: <media_size / 5>
	 */
	result = smraw_test_seek_offset(
	          handle,
	          (off64_t) ( media_size / 5 ),
	          SEEK_SET,
	          (off64_t) ( media_size / 5 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_SET offset: <media_size + 987>
	 * Expected result: <media_size + 987>
	 */
	result = smraw_test_seek_offset(
	          handle,
	          (off64_t) ( media_size + 987 ),
	          SEEK_SET,
	          (off64_t) ( media_size + 987 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_SET offset: -987
	 * Expected result: -1
	 */
	result = smraw_test_seek_offset(
	          handle,
	          -987,
	          SEEK_SET,
	          -1 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_CUR offset: 0
	 * Expected result: <media_size + 987>
	 */
	result = smraw_test_seek_offset(
	          handle,
	          0,
	          SEEK_CUR,
	          (off64_t) ( media_size + 987 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_CUR offset: <-1 * (media_size + 987)>
	 * Expected result: 0
	 */
	result = smraw_test_seek_offset(
	          handle,
	          -1 * (off64_t) ( media_size + 987 ),
	          SEEK_CUR,
	          0 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_CUR offset: <media_size / 3>
	 * Expected result: <media_size / 3>
	 */
	result = smraw_test_seek_offset(
	          handle,
	          (off64_t) ( media_size / 3 ),
	          SEEK_CUR,
	          (off64_t) ( media_size / 3 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	if( media_size == 0 )
	{
		/* Test: SEEK_CUR offset: <-2 * (media_size / 3)>
		 * Expected result: 0
		 */
		result = smraw_test_seek_offset(
		          handle,
		          -2 * (off64_t) ( media_size / 3 ),
		          SEEK_CUR,
		          0 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test seek offset.\n" );

			return( result );
		}
	}
	else
	{
		/* Test: SEEK_CUR offset: <-2 * (media_size / 3)>
		 * Expected result: -1
		 */
		result = smraw_test_seek_offset(
		          handle,
		          -2 * (off64_t) ( media_size / 3 ),
		          SEEK_CUR,
		          -1 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test seek offset.\n" );

			return( result );
		}
	}
	/* Test: SEEK_END offset: 0
	 * Expected result: <media_size>
	 */
	result = smraw_test_seek_offset(
	          handle,
	          0,
	          SEEK_END,
	          (off64_t) media_size );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_END offset: <-1 * media_size>
	 * Expected result: 0
	 */
	result = smraw_test_seek_offset(
	          handle,
	          -1 * (off64_t) media_size,
	          SEEK_END,
	          0 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_END offset: <-1 * (media_size / 4)>
	 * Expected result: <media_size - (media_size / 4)>
	 */
	result = smraw_test_seek_offset(
	          handle,
	          -1 * (off64_t) ( media_size / 4 ),
	          SEEK_END,
	          (off64_t) media_size - (off64_t) ( media_size / 4 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_END offset: 542
	 * Expected result: <media_size + 542>
	 */
	result = smraw_test_seek_offset(
	          handle,
	          542,
	          SEEK_END,
	          (off64_t) ( media_size + 542 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_END offset: <-1 * (media_size + 542)>
	 * Expected result: -1
	 */
	result = smraw_test_seek_offset(
	          handle,
	          -1 * (off64_t) ( media_size + 542 ),
	          SEEK_END,
	          -1 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: UNKNOWN (88) offset: 0
	 * Expected result: -1
	 */
	result = smraw_test_seek_offset(
	          handle,
	          0,
	          88,
	          -1 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	return( result );
}

/* Tests seeking in a file
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_seek_file(
     libcstring_system_character_t *source,
     libcerror_error_t **error )
{
	libcstring_system_character_t **filenames = NULL;
	libsmraw_handle_t *handle                 = NULL;
	size64_t media_size                       = 0;
	size_t string_length                      = 0;
	int number_of_filenames                   = 0;
	int result                                = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	string_length = libcstring_wide_string_length(
	                 source );

	if( libsmraw_glob_wide(
	     source,
	     string_length,
	     &filenames,
	     &number_of_filenames,
	     error ) != 1 )
#else
	string_length = libcstring_narrow_string_length(
	                 source );

	if( libsmraw_glob(
	     source,
	     string_length,
	     &filenames,
	     &number_of_filenames,
	     error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to glob filenames.\n" );

		goto on_error;
	}
	if( number_of_filenames < 0 )
	{
		fprintf(
		 stderr,
		 "Invalid number of filenames.\n" );

		goto on_error;
	}
	else if( number_of_filenames == 0 )
	{
		fprintf(
		 stderr,
		 "Missing filenames.\n" );

		goto on_error;
	}
	if( libsmraw_handle_initialize(
	     &handle,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create handle.\n" );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_handle_open_wide(
	     handle,
	     (wchar_t * const *) filenames,
	     number_of_filenames,
	     LIBSMRAW_OPEN_READ,
	     error ) != 1 )
#else
	if( libsmraw_handle_open(
	     handle,
	     (char * const *) filenames,
	     number_of_filenames,
	     LIBSMRAW_OPEN_READ,
	     error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open handle.\n" );

		goto on_error;
	}
	if( libsmraw_handle_get_media_size(
	     handle,
	     &media_size,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve media size.\n" );

		goto on_error;
	}
	result = smraw_test_seek(
	          handle,
	          media_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to seek in handle.\n" );

		goto on_error;
	}
	if( libsmraw_handle_close(
	     handle,
	     error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close handle.\n" );

		goto on_error;
	}
	if( libsmraw_handle_free(
	     &handle,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free handle.\n" );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_glob_wide_free(
	     filenames,
	     number_of_filenames,
	     error ) != 1 )
#else
	if( libsmraw_glob_free(
	     filenames,
	     number_of_filenames,
	     error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

		goto on_error;
	}
	return( result );

on_error:
	if( handle != NULL )
	{
		libsmraw_handle_close(
		 handle,
		 NULL );
		libsmraw_handle_free(
		 &handle,
		 NULL );
	}
	if( filenames != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		libsmraw_glob_wide_free(
		 filenames,
		 number_of_filenames,
		 NULL );
#else
		libsmraw_glob_free(
		 filenames,
		 number_of_filenames,
		 NULL );
#endif
	}
	return( -1 );
}

/* Tests seeking in a file without opening it
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_seek_file_no_open(
     libcstring_system_character_t *source SMRAW_TEST_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libsmraw_handle_t *handle = NULL;
	off64_t result_offset     = 0;
	int result                = 0;

	SMRAW_TEST_UNREFERENCED_PARAMETER( source );

	if( libsmraw_handle_initialize(
	     &handle,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create handle.\n" );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Testing seek without open: \t" );

	result_offset = libsmraw_handle_seek_offset(
	                 handle,
	                 0,
	                 SEEK_SET,
	                 error );

	if( result_offset == -1 )
	{
		result = 1;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( ( error != NULL )
	 && ( *error != NULL ) )
	{
		if( result != 1 )
		{
			libsmraw_error_backtrace_fprint(
			 *error,
			 stderr );
		}
		libsmraw_error_free(
		 error );
	}
	if( libsmraw_handle_free(
	     &handle,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free handle.\n" );

		goto on_error;
	}
	return( result );

on_error:
	if( handle != NULL )
	{
		libsmraw_handle_free(
		 &handle,
		 NULL );
	}
	return( -1 );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error              = NULL;
	libcstring_system_character_t *source = NULL;
	int result                            = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

		return( EXIT_FAILURE );
	}
	source = argv[ 1 ];

#if defined( HAVE_DEBUG_OUTPUT ) && defined( SMRAW_TEST_SEEK_VERBOSE )
	libsmraw_notify_set_verbose(
	 1 );
	libsmraw_notify_set_stream(
	 stderr,
	 NULL );
#endif
	result = smraw_test_seek_file(
	          source,
	          &error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to seek in file.\n" );

		goto on_error;
	}
	result = smraw_test_seek_file_no_open(
	          source,
	          &error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to seek in file without open.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );
		libsmraw_error_free(
		 &error );
	}
	return( EXIT_FAILURE );
}

