/*
 * Storage media (SM) RAW library write testing program
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

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "smraw_test_libcerror.h"
#include "smraw_test_libcpath.h"
#include "smraw_test_libhmac.h"
#include "smraw_test_libsmraw.h"

#define SMRAW_TEST_BUFFER_SIZE	4096

int smraw_test_write(
     const system_character_t *filename,
     size_t media_size,
     size_t maximum_segment_size,
     libcerror_error_t **error )
{
	uint8_t md5_hash[ LIBHMAC_MD5_HASH_SIZE ];

	libhmac_md5_context_t *md5_context = NULL;
	libsmraw_handle_t *handle          = NULL;
	uint8_t *buffer                    = NULL;
	static char *function              = "smraw_test_write";
	size_t write_size                  = 0;
	ssize_t write_count                = 0;
	int result                         = 1;
	int sector_iterator                = 0;

	fprintf(
	 stdout,
	 "Testing writing media size: %" PRIzd ", with maximum segment size: %" PRIzd "\t",
	 media_size,
	 maximum_segment_size );

	if( libhmac_md5_initialize(
	     &md5_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize MD5 context.",
		 function );

		goto on_error;
	}
	if( libsmraw_handle_initialize(
	     &handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable set maximum segment size.",
		 function );

		goto on_error;
	}
	buffer = (uint8_t *) memory_allocate(
	                      SMRAW_TEST_BUFFER_SIZE );

	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable set value in buffer.",
			 function );

			goto on_error;
		}
		if( libhmac_md5_update(
		     md5_context,
		     buffer,
		     write_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable update MD5 context.",
			 function );

			goto on_error;
		}
		write_count = libsmraw_handle_write_buffer(
		               handle,
		               buffer,
		               write_size,
		               error );

		if( write_count != (ssize_t) write_size )
		{
			if( write_count != (ssize_t) media_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable set value in buffer.",
			 function );

			goto on_error;
		}
		if( libhmac_md5_update(
		     md5_context,
		     buffer,
		     write_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable update MD5 context.",
			 function );

			goto on_error;
		}
		write_count = libsmraw_handle_write_buffer(
		               handle,
		               buffer,
		               write_size,
		               error );

		if( write_count != (ssize_t) write_size )
		{
			if( write_count != (ssize_t) media_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
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
	if( libhmac_md5_finalize(
	     md5_context,
	     md5_hash,
	     LIBHMAC_MD5_HASH_SIZE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to finalize MD5 context.",
		 function );

		goto on_error;
	}
	if( libhmac_md5_free(
	     &md5_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MD5 context.",
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close handle.",
		 function );

		goto on_error;
	}
	if( libsmraw_handle_free(
	     &handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
	if( md5_context != NULL )
	{
		libhmac_md5_free(
		 &md5_context,
		 NULL );
	}
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libsmraw_error_t *error      = NULL;
	system_character_t *filename = NULL;
	size_t filename_size         = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing test path.\n" );

		return( EXIT_FAILURE );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcpath_path_join_wide(
	     &filename,
	     &filename_size,
	     argv[ 1 ],
	     system_string_length(
	      argv[ 1 ] ),
	     _SYSTEM_STRING( "test1" ),
	     5,
	     &error ) != 1 )
#else
	if( libcpath_path_join(
	     &filename,
	     &filename_size,
	     argv[ 1 ],
	     system_string_length(
	      argv[ 1 ] ),
	     _SYSTEM_STRING( "test1" ),
	     5,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to create filename.\n" );

		goto on_error;
	}
	if( smraw_test_write(
	     filename,
	     0,
	     0,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

		goto on_error;
	}
	memory_free(
	 filename );

	filename = NULL;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcpath_path_join_wide(
	     &filename,
	     &filename_size,
	     argv[ 1 ],
	     system_string_length(
	      argv[ 1 ] ),
	     _SYSTEM_STRING( "test2" ),
	     5,
	     &error ) != 1 )
#else
	if( libcpath_path_join(
	     &filename,
	     &filename_size,
	     argv[ 1 ],
	     system_string_length(
	      argv[ 1 ] ),
	     _SYSTEM_STRING( "test2" ),
	     5,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to create filename.\n" );

		goto on_error;
	}
	if( smraw_test_write(
	     filename,
	     0,
	     10000,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

		goto on_error;
	}
	memory_free(
	 filename );

	filename = NULL;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcpath_path_join_wide(
	     &filename,
	     &filename_size,
	     argv[ 1 ],
	     system_string_length(
	      argv[ 1 ] ),
	     _SYSTEM_STRING( "test3" ),
	     5,
	     &error ) != 1 )
#else
	if( libcpath_path_join(
	     &filename,
	     &filename_size,
	     argv[ 1 ],
	     system_string_length(
	      argv[ 1 ] ),
	     _SYSTEM_STRING( "test3" ),
	     5,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to create filename.\n" );

		goto on_error;
	}
	if( smraw_test_write(
	     filename,
	     100000,
	     0,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

		goto on_error;
	}
	memory_free(
	 filename );

	filename = NULL;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcpath_path_join_wide(
	     &filename,
	     &filename_size,
	     argv[ 1 ],
	     system_string_length(
	      argv[ 1 ] ),
	     _SYSTEM_STRING( "test4" ),
	     5,
	     &error ) != 1 )
#else
	if( libcpath_path_join(
	     &filename,
	     &filename_size,
	     argv[ 1 ],
	     system_string_length(
	      argv[ 1 ] ),
	     _SYSTEM_STRING( "test4" ),
	     5,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to create filename.\n" );

		goto on_error;
	}
	if( smraw_test_write(
	     filename,
	     100000,
	     10000,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

		goto on_error;
	}
	memory_free(
	 filename );

	filename = NULL;

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
	if( filename != NULL )
	{
		memory_free(
		 filename );
	}
	return( EXIT_FAILURE );
}

