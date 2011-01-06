/*
 * Storage media (SM) RAW library write testing program
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

#include <common.h>
#include <memory.h>

#include <libcstring.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include <libsmraw.h>

#include "digest_context.h"
#include "digest_hash.h"
#include "md5.h"

#define SMRAW_TEST_BUFFER_SIZE	4096

int smraw_test_write(
     const char *filename,
     size_t media_size,
     size_t maximum_segment_size,
     liberror_error_t **error )
{
	digest_hash_t md5_hash[ DIGEST_HASH_SIZE_MD5 ];

	md5_context_t md5_context;

	libsmraw_handle_t *handle = NULL;
	uint8_t *buffer           = NULL;
	static char *function     = "smraw_test_write";
	size_t md5_hash_size      = DIGEST_HASH_SIZE_MD5;
	size_t write_size         = 0;
	ssize_t write_count       = 0;
	int result                = 1;
	int sector_iterator       = 0;

	fprintf(
	 stdout,
	 "Testing writing media size: %" PRIzd ", with maximum segment size: %" PRIzd "\t",
	 media_size,
	 maximum_segment_size );

	if( md5_initialize(
	     &md5_context,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize MD5 context.",
		 function );

		goto on_error;
	}
	if( libsmraw_handle_initialize(
	     &handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_handle_open_wide(
	     handle,
	     (wchar_t * const *) &filename,
	     1,
	     LIBSMRAW_OPEN_WRITE,
	     error ) != 1 )
#else
	if( libsmraw_handle_open(
	     handle,
	     (char * const *) &filename,
	     1,
	     LIBSMRAW_OPEN_WRITE,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle.",
		 function );

		goto on_error;
	}
	if( media_size > 0 )
	{
		if( libsmraw_handle_set_media_size(
		     handle,
		     media_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable set media size.",
			 function );

			goto on_error;
		}
	}
	if( libsmraw_handle_set_maximum_segment_size(
	     handle,
	     maximum_segment_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable set maximum segment size.",
		 function );

		goto on_error;
	}
	buffer = (uint8_t *) memory_allocate(
	                      SMRAW_TEST_BUFFER_SIZE );

	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable create buffer.",
		 function );

		goto on_error;
	}
	write_size = 512;

	for( sector_iterator = 0;
	     sector_iterator < 26;
	     sector_iterator++ )
	{
		if( memory_set(
		     buffer,
		     (int) 'A' + sector_iterator,
		     write_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable set value in buffer.",
			 function );

			goto on_error;
		}
		md5_update(
		 &md5_context,
		 buffer,
		 write_size,
		 error );

		write_count = libsmraw_handle_write_buffer(
		               handle,
		               buffer,
		               write_size,
		               error );

		if( write_count != (ssize_t) write_size )
		{
			if( write_count != (ssize_t) media_size )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable write buffer of size: %" PRIzd ".",
				 function,
				 write_size );

				goto on_error;
			}
		}
		if( media_size > 0 )
		{
			media_size -= write_count;
		}
	}
	write_size = 3751;

	for( sector_iterator = 0;
	     sector_iterator < 26;
	     sector_iterator++ )
	{
		if( memory_set(
		     buffer,
		     (int) 'a' + sector_iterator,
		     write_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable set value in buffer.",
			 function );

			goto on_error;
		}
		md5_update(
		 &md5_context,
		 buffer,
		 write_size,
		 error );

		write_count = libsmraw_handle_write_buffer(
		               handle,
		               buffer,
		               write_size,
		               error );

		if( write_count != (ssize_t) write_size )
		{
			if( write_count != (ssize_t) media_size )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable write buffer of size: %" PRIzd ".",
				 function,
				 write_size );

				goto on_error;
			}
		}
		if( media_size > 0 )
		{
			media_size -= write_count;
		}
	}
	if( md5_finalize(
	     &md5_context,
	     md5_hash,
	     &md5_hash_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize MD5 context.",
		 function );

		goto on_error;
	}
	memory_free(
	 buffer );

	buffer = NULL;

	/* Clean up
	 */
	if( libsmraw_handle_close(
	     handle,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close handle.",
		 function );

		goto on_error;
	}
	if( libsmraw_handle_free(
	     &handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free handle.",
		 function );

		goto on_error;
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

on_error:
	if( buffer != NULL )
	{
		memory_free(
		 buffer );
	}
	if( handle != NULL )
	{
		libsmraw_handle_close(
		 handle,
		 NULL );
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
	libsmraw_error_t *error = NULL;

	if( smraw_test_write(
	     _LIBCSTRING_SYSTEM_STRING( "test1" ),
	     0,
	     0,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

		goto on_error;
	}
	if( smraw_test_write(
	     _LIBCSTRING_SYSTEM_STRING( "test2" ),
	     0,
	     10000,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

		goto on_error;
	}
	if( smraw_test_write(
	     _LIBCSTRING_SYSTEM_STRING( "test3" ),
	     100000,
	     0,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

		goto on_error;
	}
	if( smraw_test_write(
	     _LIBCSTRING_SYSTEM_STRING( "test4" ),
	     100000,
	     10000,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

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

