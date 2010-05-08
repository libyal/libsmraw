/*
 * Storage media (SM) RAW library read compare testing program
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
#include <memory.h>

#include <libcstring.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

/* If libtool DLL support is enabled set LIBSMRAW_DLL_IMPORT
 * before including libsmraw.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBSMRAW_DLL_IMPORT
#endif

#include <libsmraw.h>

#include "digest_context.h"
#include "digest_hash.h"
#include "md5.h"

#define SMRAW_TEST_READ_BUFFER_SIZE	4096

/* Tests libsmraw_handle_seek_offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_seek_offset(
     libsmraw_handle_t *handle,
     off64_t input_offset,
     int input_whence,
     off64_t output_offset )
{
	const char *whence_string = NULL;
	libsmraw_error_t *error   = NULL;
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

	if( result_offset == -1 )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
	}
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

	return( result );
}

/* Tests libsmraw_seek_offset and libsmraw_handle_read_buffer and calculates
 * the MD5 integrity hash of the data read
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_read_hash(
     libsmraw_handle_t *handle,
     digest_hash_t *md5_hash,
     size_t *md5_hash_size,
     off64_t input_offset,
     int input_whence,
     size64_t input_size,
     off64_t output_offset,
     size64_t output_size )
{
	uint8_t buffer[ SMRAW_TEST_READ_BUFFER_SIZE ];

	md5_context_t md5_context;

	libsmraw_error_t *error = NULL;
	off64_t result_offset   = 0;
	size64_t remaining_size = 0;
	size64_t result_size    = 0;
	size_t read_size        = 0;
	ssize_t read_count      = 0;
	int result              = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	if( md5_initialize(
	     &md5_context,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize MD5 context.\n" );

		return( -1 );
	}
	result_offset = libsmraw_handle_seek_offset(
	                 handle,
	                 input_offset,
	                 input_whence,
	                 &error );

	if( result_offset == -1 )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
	}
	if( result_offset != output_offset )
	{
		fprintf(
		 stderr,
		 "Unexpected result offset: %" PRIi64 "\n",
		 result_offset );
	}
	else if( result_offset == -1 )
	{
		result = 1;
	}
	else if( result_offset >= 0 )
	{
		remaining_size = input_size;

		while( remaining_size > 0 )
		{
			read_size = SMRAW_TEST_READ_BUFFER_SIZE;

			if( remaining_size < (size64_t) read_size )
			{
				read_size = (size_t) remaining_size;
			}
			read_count = libsmraw_handle_read_buffer(
				      handle,
				      buffer,
				      read_size,
				      &error );

			if( read_count < 0 )
			{
				libsmraw_error_backtrace_fprint(
				 error,
				 stderr );

				libsmraw_error_free(
				 &error );

				break;
			}
			remaining_size -= (size64_t) read_count;
			result_size    += (size64_t) read_count;

			md5_update(
			 &md5_context,
			 buffer,
			 read_count,
			 &error );

			if( error != NULL )
			{
				fprintf(
				 stderr,
				 "Unable to update MD5 context.\n" );

				libsmraw_error_backtrace_fprint(
				 error,
				 stderr );

				libsmraw_error_free(
				 &error );

				return( -1 );
			}
			if( read_count != (ssize_t) read_size )
			{
				break;
			}
		}
		if( output_size == result_size )
		{
			result = 1;
		}
		else
		{
			fprintf(
			 stderr,
			 "Unexpected read count: %" PRIu64 "\n",
			 result_size );
		}
	}
	else
	{
		fprintf(
		 stderr,
		 "Unexpected result offset: %" PRIi64 "\n",
		 result_offset );
	}
	if( md5_finalize(
	     &md5_context,
	     md5_hash,
	     md5_hash_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to finalize MD5 context.\n" );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );

		return( -1 );
	}
	return( result );
}

/* Tests libsmraw_seek_offset and libsmraw_handle_read_buffer by computing MD5 integrity hashes
 * on identical single and multiple RAW image files and comparing them
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_read_compare(
     libsmraw_handle_t *handle_single,
     libsmraw_handle_t *handle_multi,
     off64_t input_offset,
     int input_whence,
     size64_t input_size,
     off64_t output_offset,
     size64_t output_size )
{
	libcstring_system_character_t md5_hash_string[ DIGEST_HASH_STRING_SIZE_MD5 ];

	digest_hash_t md5_hash_multi[ DIGEST_HASH_SIZE_MD5 ];
	digest_hash_t md5_hash_single[ DIGEST_HASH_SIZE_MD5 ];

	libsmraw_error_t *error     = NULL;
	const char *whence_string   = NULL;
	size_t md5_hash_size_multi  = DIGEST_HASH_SIZE_MD5;
	size_t md5_hash_size_single = DIGEST_HASH_SIZE_MD5;
	int result                  = 0;

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
	 "Testing reading range with offset: %" PRIi64 ", whence: %s (%" PRIi64 ") and size: %" PRIu64 " ",
	 input_offset,
	 whence_string,
	 output_offset,
	 input_size );

	result = smraw_test_read_hash(
	          handle_single,
	          md5_hash_single,
	          &md5_hash_size_single,
	          input_offset,
	          input_whence,
	          input_size,
	          output_offset,
	          output_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer from single handle.\n" );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = smraw_test_read_hash(
			  handle_multi,
			  md5_hash_multi,
			  &md5_hash_size_multi,
			  input_offset,
			  input_whence,
			  input_size,
			  output_offset,
			  output_size );
	}
	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer for multi handle.\n" );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( memory_compare(
		     md5_hash_single,
		     md5_hash_multi,
		     DIGEST_HASH_SIZE_MD5 ) != 0 )
		{
			fprintf(
			 stderr,
			 "Mismatch in MD5 hashes.\n" );

			result = 0;
		}
	}
	if( result != 0 )
	{
		if( digest_hash_copy_to_string(
		     md5_hash_single,
		     DIGEST_HASH_SIZE_MD5,
		     md5_hash_string,
		     DIGEST_HASH_STRING_SIZE_MD5,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to create MD5 hash string.\n" );

			libsmraw_error_backtrace_fprint(
			 error,
			 stderr );

			libsmraw_error_free(
			 &error );

			return( -1 );
		}
		fprintf(
		 stdout,
		 "MD5: %" PRIs_LIBCSTRING_SYSTEM "\t",
		 md5_hash_string );
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

	return( result );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	wchar_t **filenames_multi        = NULL;
#else
	char **filenames_multi           = NULL;
#endif

	libsmraw_error_t *error          = NULL;
	libsmraw_handle_t *handle_multi  = NULL;
	libsmraw_handle_t *handle_single = NULL;
	off64_t test_offset              = 0;
	size64_t media_size_multi        = 0;
	size64_t media_size_single       = 0;
	size64_t read_size               = 0;
	int number_of_filenames_multi    = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing single filename.\n" );

		return( EXIT_FAILURE );
	}
	if( argc < 3 )
	{
		fprintf(
		 stderr,
		 "Missing basename of mutiple files.\n" );

		return( EXIT_FAILURE );
	}
	/* Initialization single
	 */
	if( libsmraw_handle_initialize(
	     &handle_single,
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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_handle_open_wide(
	     handle_single,
	     &( argv[ 1 ] ),
	     1,
	     LIBSMRAW_OPEN_READ,
	     &error ) != 1 )
#else
	if( libsmraw_handle_open(
	     handle_single,
	     &( argv[ 1 ] ),
	     1,
	     LIBSMRAW_OPEN_READ,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open file(s).\n" );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libsmraw_handle_get_media_size(
	     handle_single,
	     &media_size_single,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve media size.\n" );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( media_size_single > (size64_t) INT64_MAX )
	{
		fprintf(
		 stderr,
		 "Media size exceeds maximum.\n" );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	/* Initialization multi
	 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_glob_wide(
	     argv[ 1 ],
	     strlen(
	      argv[ 1 ] ),
	     &filenames_multi,
	     &number_of_filenames_multi,
	     &error ) != 1 )
#else
	if( libsmraw_glob(
	     argv[ 1 ],
	     strlen(
	      argv[ 1 ] ),
	     &filenames_multi,
	     &number_of_filenames_multi,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to glob filenames.\n" );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libsmraw_handle_initialize(
	     &handle_multi,
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
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_handle_open_wide(
	     handle_multi,
	     filenames_multi,
	     number_of_filenames_multi,
	     LIBSMRAW_OPEN_READ,
	     &error ) != 1 )
#else
	if( libsmraw_handle_open(
	     handle_multi,
	     filenames_multi,
	     number_of_filenames_multi,
	     LIBSMRAW_OPEN_READ,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open file(s).\n" );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
		libsmraw_handle_free(
		 &handle_multi,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libsmraw_handle_get_media_size(
	     handle_multi,
	     &media_size_multi,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve media size.\n" );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
		libsmraw_handle_close(
		 handle_multi,
		 NULL );
		libsmraw_handle_free(
		 &handle_multi,
		 NULL );
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( media_size_multi > (size64_t) INT64_MAX )
	{
		fprintf(
		 stderr,
		 "Media size exceeds maximum.\n" );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
		libsmraw_handle_close(
		 handle_multi,
		 NULL );
		libsmraw_handle_free(
		 &handle_multi,
		 NULL );
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( media_size_single != media_size_multi )
	{
		fprintf(
		 stderr,
		 "Mismatch in media size.\n" );

		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Media size: %" PRIu64 " bytes\n",
	 media_size_single );

	/* -----------------------------------------------------------------------
	 * Case 0: test if MD5 hashes of both files match
	 * ----------------------------------------------------------------------- */

	/* Test: offset: 0 whence: SEEK_SET size: <media_size>
	 * Expected result: offset: 0 size: <media_size>
	 */
	if( smraw_test_read_compare(
	     handle_single,
	     handle_multi,
	     0,
	     SEEK_SET,
	     media_size_single,
	     0,
	     media_size_single ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read and compare.\n" );

		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	/* -----------------------------------------------------------------------
	 * Case 1: Test SEEK_SET and read
	 * ----------------------------------------------------------------------- */

	while( test_offset < (off64_t) media_size_single )
	{
		read_size = 16 * 1024;

		if( ( (size64_t) test_offset + read_size ) > media_size_single )
		{
			read_size = media_size_single - (size64_t) test_offset;
		}
		/* Test: offset: <test_offset> whence: SEEK_SET size: <read_size>
		 * Expected result: offset: <test_offset> size: <read_size>
		 */
		if( smraw_test_read_compare(
		     handle_single,
		     handle_multi,
		     test_offset,
		     SEEK_SET,
		     read_size,
		     test_offset,
		     read_size ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read and compare.\n" );

			libsmraw_handle_close(
			 handle_single,
			 NULL );
			libsmraw_handle_free(
			 &handle_single,
			 NULL );
			libsmraw_handle_close(
			 handle_single,
			 NULL );
			libsmraw_handle_free(
			 &handle_single,
			 NULL );

			return( EXIT_FAILURE );
		}
		test_offset += 17 * 1024;
	}
	/* -----------------------------------------------------------------------
	 * Case 2: Test SEEK_CUR and read
	 * ----------------------------------------------------------------------- */

	/* Test: SEEK_SET offset: 0
	 * Expected result: 0
	 */
	if( smraw_test_seek_offset(
	     handle_single,
	     0,
	     SEEK_SET,
	     0 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	/* Test: SEEK_SET offset: 0
	 * Expected result: 0
	 */
	if( smraw_test_seek_offset(
	     handle_multi,
	     0,
	     SEEK_SET,
	     0 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	test_offset = 0;

	while( test_offset < (off64_t) media_size_single )
	{
		read_size    = 16 * 1024;
		test_offset += 17 * 1024;

		if( ( (size64_t) test_offset + read_size ) > media_size_single )
		{
			read_size = media_size_single - (size64_t) test_offset;
		}
		/* Test: offset: <17 * 1024> whence: SEEK_CUR size: <read_size>
		 * Expected result: offset: <test_offset> size: <read_size>
		 */
		if( smraw_test_read_compare(
		     handle_single,
		     handle_multi,
		     17 * 1024,
		     SEEK_CUR,
		     read_size,
		     test_offset,
		     read_size ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read and compare.\n" );

			libsmraw_handle_close(
			 handle_single,
			 NULL );
			libsmraw_handle_free(
			 &handle_single,
			 NULL );
			libsmraw_handle_close(
			 handle_single,
			 NULL );
			libsmraw_handle_free(
			 &handle_single,
			 NULL );

			return( EXIT_FAILURE );
		}
		test_offset += read_size;
	}
	/* -----------------------------------------------------------------------
	 * Case 3: Test SEEK_END and read
	 * ----------------------------------------------------------------------- */

	/* Test: SEEK_SET offset: 0
	 * Expected result: 0
	 */
	if( smraw_test_seek_offset(
	     handle_single,
	     0,
	     SEEK_SET,
	     0 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	/* Test: SEEK_SET offset: 0
	 * Expected result: 0
	 */
	if( smraw_test_seek_offset(
	     handle_multi,
	     0,
	     SEEK_SET,
	     0 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	test_offset = 9 * 1024;

	while( test_offset < (off64_t) media_size_single )
	{
		read_size = 16 * 1024;

		if( (size64_t) test_offset < read_size )
		{
			read_size = (size64_t) test_offset;
		}
		/* Test: offset: <-1 * test_offset> whence: SEEK_END size: <read_size>
		 * Expected result: offset: <media_size - test_offset> size: <read_size>
		 */
		if( smraw_test_read_compare(
		     handle_single,
		     handle_multi,
		     -1 * test_offset,
		     SEEK_END,
		     read_size,
		     media_size_single - test_offset,
		     read_size ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read and compare.\n" );

			libsmraw_handle_close(
			 handle_single,
			 NULL );
			libsmraw_handle_free(
			 &handle_single,
			 NULL );
			libsmraw_handle_close(
			 handle_single,
			 NULL );
			libsmraw_handle_free(
			 &handle_single,
			 NULL );

			return( EXIT_FAILURE );
		}
		test_offset += 17 * 1024;
	}
	/* Clean up multi
	 */
	if( libsmraw_handle_close(
	     handle_multi,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file(s).\n" );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
		libsmraw_handle_free(
		 &handle_multi,
		 NULL );
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libsmraw_handle_free(
	     &handle_multi,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free handle.\n" );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	/* Clean up single
	 */
	if( libsmraw_handle_close(
	     handle_single,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file(s).\n" );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libsmraw_handle_free(
	     &handle_single,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free handle.\n" );

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

