/*
 * Library handle type test program
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "smraw_test_functions.h"
#include "smraw_test_getopt.h"
#include "smraw_test_libbfio.h"
#include "smraw_test_libcerror.h"
#include "smraw_test_libsmraw.h"
#include "smraw_test_macros.h"
#include "smraw_test_memory.h"

#include "../libsmraw/libsmraw_handle.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make smraw_test_handle generate verbose output
#define SMRAW_TEST_HANDLE_VERBOSE
 */

#define SMRAW_TEST_HANDLE_READ_BUFFER_SIZE	4096

#if !defined( LIBSMRAW_HAVE_BFIO )

LIBSMRAW_EXTERN \
int libsmraw_handle_open_file_io_pool(
     libsmraw_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libsmraw_error_t **error );

#endif /* !defined( LIBSMRAW_HAVE_BFIO ) */

/* Creates and opens a source handle
 * Returns 1 if successful or -1 on error
 */
int smraw_test_handle_open_source(
     libsmraw_handle_t **handle,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error )
{
	static char *function = "smraw_test_handle_open_source";
	int result            = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO pool.",
		 function );

		return( -1 );
	}
	if( libsmraw_handle_initialize(
	     handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize handle.",
		 function );

		goto on_error;
	}
	result = libsmraw_handle_open_file_io_pool(
	          *handle,
	          file_io_pool,
	          LIBSMRAW_OPEN_READ,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *handle != NULL )
	{
		libsmraw_handle_free(
		 handle,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source handle
 * Returns 1 if successful or -1 on error
 */
int smraw_test_handle_close_source(
     libsmraw_handle_t **handle,
     libcerror_error_t **error )
{
	static char *function = "smraw_test_handle_close_source";
	int result            = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libsmraw_handle_close(
	     *handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close handle.",
		 function );

		result = -1;
	}
	if( libsmraw_handle_free(
	     handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free handle.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libsmraw_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libsmraw_handle_t *handle       = NULL;
	int result                      = 0;

#if defined( HAVE_SMRAW_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libsmraw_handle_initialize(
	          &handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmraw_handle_free(
	          &handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_handle_initialize(
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	handle = (libsmraw_handle_t *) 0x12345678UL;

	result = libsmraw_handle_initialize(
	          &handle,
	          &error );

	handle = NULL;

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_SMRAW_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libsmraw_handle_initialize with malloc failing
		 */
		smraw_test_malloc_attempts_before_fail = test_number;

		result = libsmraw_handle_initialize(
		          &handle,
		          &error );

		if( smraw_test_malloc_attempts_before_fail != -1 )
		{
			smraw_test_malloc_attempts_before_fail = -1;

			if( handle != NULL )
			{
				libsmraw_handle_free(
				 &handle,
				 NULL );
			}
		}
		else
		{
			SMRAW_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			SMRAW_TEST_ASSERT_IS_NULL(
			 "handle",
			 handle );

			SMRAW_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libsmraw_handle_initialize with memset failing
		 */
		smraw_test_memset_attempts_before_fail = test_number;

		result = libsmraw_handle_initialize(
		          &handle,
		          &error );

		if( smraw_test_memset_attempts_before_fail != -1 )
		{
			smraw_test_memset_attempts_before_fail = -1;

			if( handle != NULL )
			{
				libsmraw_handle_free(
				 &handle,
				 NULL );
			}
		}
		else
		{
			SMRAW_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			SMRAW_TEST_ASSERT_IS_NULL(
			 "handle",
			 handle );

			SMRAW_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_SMRAW_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libsmraw_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_free function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libsmraw_handle_free(
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_open function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error    = NULL;
	libsmraw_handle_t *handle   = NULL;
	char **filenames            = NULL;
	size_t narrow_source_length = 0;
	int number_of_filenames     = 0;
	int result                  = 0;

	/* Initialize test
	 */
	result = smraw_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	narrow_source_length = narrow_string_length(
	                        narrow_source );

	result = libsmraw_glob(
	          narrow_source,
	          narrow_source_length,
	          &filenames,
	          &number_of_filenames,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "filenames",
	 filenames );

	SMRAW_TEST_ASSERT_GREATER_THAN_INT(
	 "number_of_filenames",
	 number_of_filenames,
	 0 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmraw_handle_initialize(
	          &handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libsmraw_handle_open(
	          handle,
	          (char * const *) filenames,
	          number_of_filenames,
	          LIBSMRAW_OPEN_READ,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libsmraw_handle_close(
	          handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmraw_handle_free(
	          &handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmraw_glob_free(
	          filenames,
	          number_of_filenames,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libsmraw_handle_free(
		 &handle,
		 NULL );
	}
	if( filenames != NULL )
	{
		libsmraw_glob_free(
		 filenames,
		 number_of_filenames,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libsmraw_handle_open_wide functions
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_open_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error  = NULL;
	libsmraw_handle_t *handle = NULL;
	wchar_t **filenames       = NULL;
	size_t wide_source_length = 0;
	int number_of_filenames   = 0;
	int result                = 0;

	/* Initialize test
	 */
	result = smraw_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	wide_source_length = wide_string_length(
	                      wide_source );

	result = libsmraw_glob_wide(
	          wide_source,
	          wide_source_length,
	          &filenames,
	          &number_of_filenames,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "filenames",
	 filenames );

	SMRAW_TEST_ASSERT_GREATER_THAN_INT(
	 "number_of_filenames",
	 number_of_filenames,
	 0 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmraw_handle_initialize(
	          &handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libsmraw_handle_open_wide(
	          handle,
	          (wchar_t * const *) filenames,
	          number_of_filenames,
	          LIBSMRAW_OPEN_READ,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libsmraw_handle_close(
	          handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmraw_handle_free(
	          &handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmraw_glob_wide_free(
	          filenames,
	          number_of_filenames,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libsmraw_handle_free(
		 &handle,
		 NULL );
	}
	if( filenames != NULL )
	{
		libsmraw_glob_wide_free(
		 filenames,
		 number_of_filenames,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libsmraw_handle_open_file_io_pool function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_open_file_io_pool(
     const system_character_t *source )
{
	libbfio_handle_t *file_io_handle = NULL;
	libbfio_pool_t *file_io_pool     = NULL;
	libcerror_error_t *error         = NULL;
	libsmraw_handle_t *handle        = NULL;
	system_character_t **filenames   = NULL;
	size_t string_length             = 0;
	int filename_index               = 0;
	int number_of_filenames          = 0;
	int result                       = 0;

	/* Initialize test
	 */
	string_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libsmraw_glob_wide(
	          source,
	          string_length,
	          &filenames,
	          &number_of_filenames,
	          &error );
#else
	result = libsmraw_glob(
	          source,
	          string_length,
	          &filenames,
	          &number_of_filenames,
	          &error );
#endif

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "filenames",
	 filenames );

	SMRAW_TEST_ASSERT_GREATER_THAN_INT(
	 "number_of_filenames",
	 number_of_filenames,
	 0 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_pool_initialize(
	          &file_io_pool,
	          number_of_filenames,
	          LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        SMRAW_TEST_ASSERT_IS_NOT_NULL(
         "file_io_pool",
         file_io_pool );

        SMRAW_TEST_ASSERT_IS_NULL(
         "error",
         error );

	for( filename_index = 0;
	     filename_index < number_of_filenames;
	     filename_index++ )
	{
		result = libbfio_file_initialize(
		          &file_io_handle,
		          &error );

		SMRAW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        SMRAW_TEST_ASSERT_IS_NOT_NULL(
	         "file_io_handle",
	         file_io_handle );

	        SMRAW_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		string_length = system_string_length(
		                 filenames[ filename_index ] );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_set_name_wide(
		          file_io_handle,
		          filenames[ filename_index ],
		          string_length,
		          &error );
#else
		result = libbfio_file_set_name(
		          file_io_handle,
		          filenames[ filename_index ],
		          string_length,
		          &error );
#endif
		SMRAW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libbfio_pool_set_handle(
		          file_io_pool,
		          filename_index,
		          file_io_handle,
		          LIBBFIO_OPEN_READ,
		          &error );

		SMRAW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        SMRAW_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		file_io_handle = NULL;
	}
	result = libsmraw_handle_initialize(
	          &handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libsmraw_handle_open_file_io_pool(
	          handle,
	          file_io_pool,
	          LIBSMRAW_OPEN_READ,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_handle_open_file_io_pool(
	          NULL,
	          file_io_pool,
	          LIBSMRAW_OPEN_READ,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmraw_handle_open_file_io_pool(
	          handle,
	          NULL,
	          LIBSMRAW_OPEN_READ,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmraw_handle_open_file_io_pool(
	          handle,
	          file_io_pool,
	          -1,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test open when already opened
	 */
	result = libsmraw_handle_open_file_io_pool(
	          handle,
	          file_io_pool,
	          LIBSMRAW_OPEN_READ,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libsmraw_handle_free(
	          &handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_pool_free(
	          &file_io_pool,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
         "file_io_pool",
         file_io_pool );

        SMRAW_TEST_ASSERT_IS_NULL(
         "error",
         error );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libsmraw_glob_wide_free(
	          filenames,
	          number_of_filenames,
	          &error );
#else
	result = libsmraw_glob_free(
	          filenames,
	          number_of_filenames,
	          &error );
#endif

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libsmraw_handle_free(
		 &handle,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( filenames != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
	return( 0 );
}

/* Tests the libsmraw_handle_close function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libsmraw_handle_close(
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_open and libsmraw_handle_close functions
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_open_close(
     const system_character_t *source )
{
	libcerror_error_t *error       = NULL;
	libsmraw_handle_t *handle      = NULL;
	system_character_t **filenames = NULL;
	size_t source_length           = 0;
	int number_of_filenames        = 0;
	int result                     = 0;

	/* Initialize test
	 */
	source_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libsmraw_glob_wide(
	          source,
	          source_length,
	          &filenames,
	          &number_of_filenames,
	          &error );
#else
	result = libsmraw_glob(
	          source,
	          source_length,
	          &filenames,
	          &number_of_filenames,
	          &error );
#endif

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "filenames",
	 filenames );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmraw_handle_initialize(
	          &handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libsmraw_handle_open_wide(
	          handle,
	          (wchar_t * const *) filenames,
	          number_of_filenames,
	          LIBSMRAW_OPEN_READ,
	          &error );
#else
	result = libsmraw_handle_open(
	          handle,
	          (char * const *) filenames,
	          number_of_filenames,
	          LIBSMRAW_OPEN_READ,
	          &error );
#endif

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmraw_handle_close(
	          handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libsmraw_handle_open_wide(
	          handle,
	          (wchar_t * const *) filenames,
	          number_of_filenames,
	          LIBSMRAW_OPEN_READ,
	          &error );
#else
	result = libsmraw_handle_open(
	          handle,
	          (char * const *) filenames,
	          number_of_filenames,
	          LIBSMRAW_OPEN_READ,
	          &error );
#endif

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmraw_handle_close(
	          handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libsmraw_handle_free(
	          &handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libsmraw_glob_wide_free(
	          filenames,
	          number_of_filenames,
	          &error );
#else
	result = libsmraw_glob_free(
	          filenames,
	          number_of_filenames,
	          &error );
#endif
	filenames = NULL;

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libsmraw_handle_free(
		 &handle,
		 NULL );
	}
	if( filenames != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
	return( 0 );
}

/* Tests the libsmraw_handle_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_signal_abort(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libsmraw_handle_signal_abort(
	          handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_handle_signal_abort(
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_read_buffer function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_read_buffer(
     libsmraw_handle_t *handle )
{
	uint8_t buffer[ SMRAW_TEST_HANDLE_READ_BUFFER_SIZE ];

	libcerror_error_t *error      = NULL;
	time_t timestamp              = 0;
	size64_t media_size           = 0;
	size64_t remaining_media_size = 0;
	size_t read_size              = 0;
	ssize_t read_count            = 0;
	off64_t offset                = 0;
	int number_of_tests           = 1024;
	int random_number             = 0;
	int result                    = 0;
	int test_number               = 0;

#if defined( SMRAW_TEST_HANDLE_VERBOSE )
	off64_t media_offset          = 0;
#endif

	/* Determine size
	 */
	result = libsmraw_handle_get_media_size(
	          handle,
	          &media_size,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libsmraw_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_size = SMRAW_TEST_HANDLE_READ_BUFFER_SIZE;

	if( media_size < SMRAW_TEST_HANDLE_READ_BUFFER_SIZE )
	{
		read_size = (size_t) media_size;
	}
	read_count = libsmraw_handle_read_buffer(
	              handle,
	              buffer,
	              SMRAW_TEST_HANDLE_READ_BUFFER_SIZE,
	              &error );

	SMRAW_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) read_size );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( media_size > 8 )
	{
		/* Set offset to media_size - 8
		 */
		offset = libsmraw_handle_seek_offset(
		          handle,
		          -8,
		          SEEK_END,
		          &error );

		SMRAW_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 (int64_t) media_size - 8 );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer on media_size boundary
		 */
		read_count = libsmraw_handle_read_buffer(
		              handle,
		              buffer,
		              SMRAW_TEST_HANDLE_READ_BUFFER_SIZE,
		              &error );

		SMRAW_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond media_size boundary
		 */
		read_count = libsmraw_handle_read_buffer(
		              handle,
		              buffer,
		              SMRAW_TEST_HANDLE_READ_BUFFER_SIZE,
		              &error );

		SMRAW_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Stress test read buffer
	 */
	timestamp = time(
	             NULL );

	srand(
	 (unsigned int) timestamp );

	offset = libsmraw_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	remaining_media_size = media_size;

	for( test_number = 0;
	     test_number < number_of_tests;
	     test_number++ )
	{
		random_number = rand();

		SMRAW_TEST_ASSERT_GREATER_THAN_INT(
		 "random_number",
		 random_number,
		 -1 );

		read_size = (size_t) random_number % SMRAW_TEST_HANDLE_READ_BUFFER_SIZE;

#if defined( SMRAW_TEST_HANDLE_VERBOSE )
		fprintf(
		 stdout,
		 "libsmraw_handle_read_buffer: at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 media_offset,
		 media_offset,
		 read_size );
#endif
		read_count = libsmraw_handle_read_buffer(
		              handle,
		              buffer,
		              read_size,
		              &error );

		if( read_size > remaining_media_size )
		{
			read_size = (size_t) remaining_media_size;
		}
		SMRAW_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) read_size );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		remaining_media_size -= read_count;

#if defined( SMRAW_TEST_HANDLE_VERBOSE )
		media_offset += read_count;
#endif

		if( remaining_media_size == 0 )
		{
			offset = libsmraw_handle_seek_offset(
			          handle,
			          0,
			          SEEK_SET,
			          &error );

			SMRAW_TEST_ASSERT_EQUAL_INT64(
			 "offset",
			 offset,
			 (int64_t) 0 );

			SMRAW_TEST_ASSERT_IS_NULL(
			 "error",
			 error );

			remaining_media_size = media_size;

#if defined( SMRAW_TEST_HANDLE_VERBOSE )
			media_offset = 0;
#endif
		}
	}
	/* Test error cases
	 */
	read_count = libsmraw_handle_read_buffer(
	              NULL,
	              buffer,
	              SMRAW_TEST_HANDLE_READ_BUFFER_SIZE,
	              &error );

	SMRAW_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libsmraw_handle_read_buffer(
	              handle,
	              NULL,
	              SMRAW_TEST_HANDLE_READ_BUFFER_SIZE,
	              &error );

	SMRAW_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libsmraw_handle_read_buffer(
	              handle,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              &error );

	SMRAW_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_read_buffer_at_offset function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_read_buffer_at_offset(
     libsmraw_handle_t *handle )
{
	uint8_t buffer[ SMRAW_TEST_HANDLE_READ_BUFFER_SIZE ];

	libcerror_error_t *error      = NULL;
	time_t timestamp              = 0;
	size64_t media_size           = 0;
	size64_t remaining_media_size = 0;
	size_t read_size              = 0;
	ssize_t read_count            = 0;
	off64_t media_offset          = 0;
	off64_t offset                = 0;
	int number_of_tests           = 1024;
	int random_number             = 0;
	int result                    = 0;
	int test_number               = 0;

	/* Determine size
	 */
	result = libsmraw_handle_get_media_size(
	          handle,
	          &media_size,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_size = SMRAW_TEST_HANDLE_READ_BUFFER_SIZE;

	if( media_size < SMRAW_TEST_HANDLE_READ_BUFFER_SIZE )
	{
		read_size = (size_t) media_size;
	}
	read_count = libsmraw_handle_read_buffer_at_offset(
	              handle,
	              buffer,
	              SMRAW_TEST_HANDLE_READ_BUFFER_SIZE,
	              0,
	              &error );

	SMRAW_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) read_size );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( media_size > 8 )
	{
		/* Read buffer on media_size boundary
		 */
		read_count = libsmraw_handle_read_buffer_at_offset(
		              handle,
		              buffer,
		              SMRAW_TEST_HANDLE_READ_BUFFER_SIZE,
		              media_size - 8,
		              &error );

		SMRAW_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond media_size boundary
		 */
		read_count = libsmraw_handle_read_buffer_at_offset(
		              handle,
		              buffer,
		              SMRAW_TEST_HANDLE_READ_BUFFER_SIZE,
		              media_size + 8,
		              &error );

		SMRAW_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Stress test read buffer
	 */
	timestamp = time(
	             NULL );

	srand(
	 (unsigned int) timestamp );

	for( test_number = 0;
	     test_number < number_of_tests;
	     test_number++ )
	{
		random_number = rand();

		SMRAW_TEST_ASSERT_GREATER_THAN_INT(
		 "random_number",
		 random_number,
		 -1 );

		if( media_size > 0 )
		{
			media_offset = (off64_t) random_number % media_size;
		}
		read_size = (size_t) random_number % SMRAW_TEST_HANDLE_READ_BUFFER_SIZE;

#if defined( SMRAW_TEST_HANDLE_VERBOSE )
		fprintf(
		 stdout,
		 "libsmraw_handle_read_buffer_at_offset: at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 media_offset,
		 media_offset,
		 read_size );
#endif
		read_count = libsmraw_handle_read_buffer_at_offset(
		              handle,
		              buffer,
		              read_size,
		              media_offset,
		              &error );

		remaining_media_size = media_size - media_offset;

		if( read_size > remaining_media_size )
		{
			read_size = (size_t) remaining_media_size;
		}
		SMRAW_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) read_size );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		remaining_media_size -= read_count;

		if( remaining_media_size == 0 )
		{
			offset = libsmraw_handle_seek_offset(
			          handle,
			          0,
			          SEEK_SET,
			          &error );

			SMRAW_TEST_ASSERT_EQUAL_INT64(
			 "offset",
			 offset,
			 (int64_t) 0 );

			SMRAW_TEST_ASSERT_IS_NULL(
			 "error",
			 error );

			remaining_media_size = media_size;
		}
	}
	/* Test error cases
	 */
	read_count = libsmraw_handle_read_buffer_at_offset(
	              NULL,
	              buffer,
	              SMRAW_TEST_HANDLE_READ_BUFFER_SIZE,
	              0,
	              &error );

	SMRAW_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libsmraw_handle_read_buffer_at_offset(
	              handle,
	              NULL,
	              SMRAW_TEST_HANDLE_READ_BUFFER_SIZE,
	              0,
	              &error );

	SMRAW_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libsmraw_handle_read_buffer_at_offset(
	              handle,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              0,
	              &error );

	SMRAW_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libsmraw_handle_read_buffer_at_offset(
	              handle,
	              buffer,
	              SMRAW_TEST_HANDLE_READ_BUFFER_SIZE,
	              -1,
	              &error );

	SMRAW_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_seek_offset function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_seek_offset(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	off64_t offset           = 0;

	/* Test regular cases
	 */
	offset = libsmraw_handle_seek_offset(
	          handle,
	          0,
	          SEEK_END,
	          &error );

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	size = (size64_t) offset;

	offset = libsmraw_handle_seek_offset(
	          handle,
	          1024,
	          SEEK_SET,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 1024 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libsmraw_handle_seek_offset(
	          handle,
	          -512,
	          SEEK_CUR,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 512 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libsmraw_handle_seek_offset(
	          handle,
	          (off64_t) ( size + 512 ),
	          SEEK_SET,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) ( size + 512 ) );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libsmraw_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	offset = libsmraw_handle_seek_offset(
	          NULL,
	          0,
	          SEEK_SET,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libsmraw_handle_seek_offset(
	          handle,
	          -1,
	          SEEK_SET,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libsmraw_handle_seek_offset(
	          handle,
	          -1,
	          SEEK_CUR,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	offset = libsmraw_handle_seek_offset(
	          handle,
	          (off64_t) ( -1 * ( size + 1 ) ),
	          SEEK_END,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_get_offset function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_offset(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	off64_t offset           = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libsmraw_handle_get_offset(
	          handle,
	          &offset,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_handle_get_offset(
	          NULL,
	          &offset,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmraw_handle_get_offset(
	          handle,
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_get_segment_filename_size function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_segment_filename_size(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error         = NULL;
	size64_t media_size              = 0;
	size_t segment_filename_size     = 0;
	off64_t offset                   = 0;
	int result                       = 0;
	int segment_filename_size_is_set = 0;

	/* Determine size
	 */
	result = libsmraw_handle_get_media_size(
	          handle,
	          &media_size,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* libsmraw_handle_get_segment_filename_size will fail on an empty file
	 */
	if( media_size == 0 )
	{
		return( 1 );
	}
	/* Reset offset to 0
	 */
	offset = libsmraw_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libsmraw_handle_get_segment_filename_size(
	          handle,
	          &segment_filename_size,
	          &error );

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	segment_filename_size_is_set = result;

	/* Test error cases
	 */
	result = libsmraw_handle_get_segment_filename_size(
	          NULL,
	          &segment_filename_size,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( segment_filename_size_is_set != 0 )
	{
		result = libsmraw_handle_get_segment_filename_size(
		          handle,
		          NULL,
		          &error );

		SMRAW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		SMRAW_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libsmraw_handle_get_segment_filename_size_wide function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_segment_filename_size_wide(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error              = NULL;
	size64_t media_size                   = 0;
	size_t segment_filename_size_wide     = 0;
	off64_t offset                        = 0;
	int result                            = 0;
	int segment_filename_size_wide_is_set = 0;

	/* Determine size
	 */
	result = libsmraw_handle_get_media_size(
	          handle,
	          &media_size,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* libsmraw_handle_get_segment_filename_size_wide will fail on an empty file
	 */
	if( media_size == 0 )
	{
		return( 1 );
	}
	/* Reset offset to 0
	 */
	offset = libsmraw_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libsmraw_handle_get_segment_filename_size_wide(
	          handle,
	          &segment_filename_size_wide,
	          &error );

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	segment_filename_size_wide_is_set = result;

	/* Test error cases
	 */
	result = libsmraw_handle_get_segment_filename_size_wide(
	          NULL,
	          &segment_filename_size_wide,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( segment_filename_size_wide_is_set != 0 )
	{
		result = libsmraw_handle_get_segment_filename_size_wide(
		          handle,
		          NULL,
		          &error );

		SMRAW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		SMRAW_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libsmraw_handle_get_maximum_segment_size function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_maximum_segment_size(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error      = NULL;
	size64_t maximum_segment_size = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libsmraw_handle_get_maximum_segment_size(
	          handle,
	          &maximum_segment_size,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_handle_get_maximum_segment_size(
	          NULL,
	          &maximum_segment_size,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmraw_handle_get_maximum_segment_size(
	          handle,
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_get_filename_size function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_filename_size(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size64_t media_size      = 0;
	size_t filename_size     = 0;
	off64_t offset           = 0;
	int result               = 0;

	/* Determine size
	 */
	result = libsmraw_handle_get_media_size(
	          handle,
	          &media_size,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* libsmraw_handle_get_filename_size will fail on an empty file
	 */
	if( media_size == 0 )
	{
		return( 1 );
	}
	/* Reset offset to 0
	 */
	offset = libsmraw_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libsmraw_handle_get_filename_size(
	          handle,
	          &filename_size,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_handle_get_filename_size(
	          NULL,
	          &filename_size,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmraw_handle_get_filename_size(
	          handle,
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libsmraw_handle_get_filename_size_wide function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_filename_size_wide(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error  = NULL;
	size64_t media_size       = 0;
	size_t filename_size_wide = 0;
	off64_t offset            = 0;
	int result                = 0;

	/* Determine size
	 */
	result = libsmraw_handle_get_media_size(
	          handle,
	          &media_size,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* smraw_test_handle_get_filename_size_wide will fail on an empty file
	 */
	if( media_size == 0 )
	{
		return( 1 );
	}
	/* Reset offset to 0
	 */
	offset = libsmraw_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libsmraw_handle_get_filename_size_wide(
	          handle,
	          &filename_size_wide,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_handle_get_filename_size_wide(
	          NULL,
	          &filename_size_wide,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmraw_handle_get_filename_size_wide(
	          handle,
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( TODO )

/* Tests the libsmraw_handle_get_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_file_io_handle(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error        = NULL;
	libbfio_handle_t file_io_handle = 0;
	int file_io_handle_is_set       = 0;
	int result                      = 0;

	/* Test regular cases
	 */
	result = libsmraw_handle_get_file_io_handle(
	          handle,
	          &file_io_handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	file_io_handle_is_set = result;

	/* Test error cases
	 */
	result = libsmraw_handle_get_file_io_handle(
	          NULL,
	          &file_io_handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( file_io_handle_is_set != 0 )
	{
		result = libsmraw_handle_get_file_io_handle(
		          handle,
		          NULL,
		          &error );

		SMRAW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		SMRAW_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( TODO ) */

/* Tests the libsmraw_handle_get_media_size function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_media_size(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size64_t media_size      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libsmraw_handle_get_media_size(
	          handle,
	          &media_size,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_handle_get_media_size(
	          NULL,
	          &media_size,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmraw_handle_get_media_size(
	          handle,
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_get_bytes_per_sector function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_bytes_per_sector(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error  = NULL;
	uint32_t bytes_per_sector = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libsmraw_handle_get_bytes_per_sector(
	          handle,
	          &bytes_per_sector,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_handle_get_bytes_per_sector(
	          NULL,
	          &bytes_per_sector,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmraw_handle_get_bytes_per_sector(
	          handle,
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_get_media_type function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_media_type(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	uint8_t media_type       = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libsmraw_handle_get_media_type(
	          handle,
	          &media_type,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_handle_get_media_type(
	          NULL,
	          &media_type,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmraw_handle_get_media_type(
	          handle,
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_get_media_flags function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_media_flags(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	uint8_t media_flags      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libsmraw_handle_get_media_flags(
	          handle,
	          &media_flags,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_handle_get_media_flags(
	          NULL,
	          &media_flags,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmraw_handle_get_media_flags(
	          handle,
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_get_number_of_information_values function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_number_of_information_values(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error         = NULL;
	int number_of_information_values = 0;
	int result                       = 0;

	/* Test regular cases
	 */
	result = libsmraw_handle_get_number_of_information_values(
	          handle,
	          &number_of_information_values,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_handle_get_number_of_information_values(
	          NULL,
	          &number_of_information_values,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmraw_handle_get_number_of_information_values(
	          handle,
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmraw_handle_get_number_of_integrity_hash_values function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_number_of_integrity_hash_values(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error            = NULL;
	int number_of_integrity_hash_values = 0;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libsmraw_handle_get_number_of_integrity_hash_values(
	          handle,
	          &number_of_integrity_hash_values,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_handle_get_number_of_integrity_hash_values(
	          NULL,
	          &number_of_integrity_hash_values,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmraw_handle_get_number_of_integrity_hash_values(
	          handle,
	          NULL,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libbfio_handle_t *file_io_handle = NULL;
	libbfio_pool_t *file_io_pool     = NULL;
	libcerror_error_t *error         = NULL;
	libsmraw_handle_t *handle        = NULL;
	system_character_t **filenames   = NULL;
	system_character_t *source       = NULL;
	system_integer_t option          = 0;
	size_t string_length             = 0;
	int filename_index               = 0;
	int number_of_filenames          = 0;
	int result                       = 0;

	while( ( option = smraw_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( SMRAW_TEST_HANDLE_VERBOSE )
	libsmraw_notify_set_verbose(
	 1 );
	libsmraw_notify_set_stream(
	 stderr,
	 NULL );
#endif

	SMRAW_TEST_RUN(
	 "libsmraw_handle_initialize",
	 smraw_test_handle_initialize );

	SMRAW_TEST_RUN(
	 "libsmraw_handle_free",
	 smraw_test_handle_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		string_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libsmraw_glob_wide(
		          source,
		          string_length,
		          &filenames,
		          &number_of_filenames,
		          &error );
#else
		result = libsmraw_glob(
		          source,
		          string_length,
		          &filenames,
		          &number_of_filenames,
		          &error );
#endif

		SMRAW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		SMRAW_TEST_ASSERT_IS_NOT_NULL(
		 "filenames",
		 filenames );

		SMRAW_TEST_ASSERT_GREATER_THAN_INT(
		 "number_of_filenames",
		 number_of_filenames,
		 0 );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libbfio_pool_initialize(
		          &file_io_pool,
		          number_of_filenames,
		          LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES,
		          &error );

		SMRAW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        SMRAW_TEST_ASSERT_IS_NOT_NULL(
	         "file_io_pool",
	         file_io_pool );

	        SMRAW_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		for( filename_index = 0;
		     filename_index < number_of_filenames;
		     filename_index++ )
		{
			result = libbfio_file_initialize(
			          &file_io_handle,
			          &error );

			SMRAW_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 1 );

		        SMRAW_TEST_ASSERT_IS_NOT_NULL(
		         "file_io_handle",
		         file_io_handle );

		        SMRAW_TEST_ASSERT_IS_NULL(
		         "error",
		         error );

			string_length = system_string_length(
			                 filenames[ filename_index ] );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libbfio_file_set_name_wide(
			          file_io_handle,
			          filenames[ filename_index ],
			          string_length,
			          &error );
#else
			result = libbfio_file_set_name(
			          file_io_handle,
			          filenames[ filename_index ],
			          string_length,
			          &error );
#endif
			SMRAW_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 1 );

			SMRAW_TEST_ASSERT_IS_NULL(
			 "error",
			 error );

			result = libbfio_pool_set_handle(
			          file_io_pool,
			          filename_index,
			          file_io_handle,
			          LIBBFIO_OPEN_READ,
			          &error );

			SMRAW_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 1 );

		        SMRAW_TEST_ASSERT_IS_NULL(
		         "error",
		         error );

			file_io_handle = NULL;
		}
		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_open",
		 smraw_test_handle_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_open_wide",
		 smraw_test_handle_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_open_file_io_pool",
		 smraw_test_handle_open_file_io_pool,
		 source );

		SMRAW_TEST_RUN(
		 "libsmraw_handle_close",
		 smraw_test_handle_close );

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_open_close",
		 smraw_test_handle_open_close,
		 source );

		/* Initialize handle for tests
		 */
		result = smraw_test_handle_open_source(
		          &handle,
		          file_io_pool,
		          &error );

		SMRAW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		SMRAW_TEST_ASSERT_IS_NOT_NULL(
		 "handle",
		 handle );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_signal_abort",
		 smraw_test_handle_signal_abort,
		 handle );

#if defined( __GNUC__ ) && !defined( LIBSMRAW_DLL_IMPORT )

		/* TODO: add tests for libsmraw_handle_read_information_file */

#endif /* defined( __GNUC__ ) && !defined( LIBSMRAW_DLL_IMPORT ) */

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_read_buffer",
		 smraw_test_handle_read_buffer,
		 handle );

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_read_buffer_at_offset",
		 smraw_test_handle_read_buffer_at_offset,
		 handle );

		/* TODO: add tests for libsmraw_handle_write_buffer */

		/* TODO: add tests for libsmraw_handle_write_buffer_at_offset */

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_seek_offset",
		 smraw_test_handle_seek_offset,
		 handle );

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_offset",
		 smraw_test_handle_get_offset,
		 handle );

		/* TODO: add tests for libsmraw_handle_set_maximum_number_of_open_handles */

#if defined( TODO )

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_segment_filename_size",
		 smraw_test_handle_get_segment_filename_size,
		 handle );

		/* TODO: add tests for libsmraw_handle_get_segment_filename */

		/* TODO: add tests for libsmraw_handle_set_segment_filename */

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_segment_filename_size_wide",
		 smraw_test_handle_get_segment_filename_size_wide,
		 handle );

		/* TODO: add tests for libsmraw_handle_get_segment_filename_wide */

		/* TODO: add tests for libsmraw_handle_set_segment_filename_wide */

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#endif /* defined( TODO ) */

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_maximum_segment_size",
		 smraw_test_handle_get_maximum_segment_size,
		 handle );

		/* TODO: add tests for libsmraw_handle_set_maximum_segment_size */

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_filename_size",
		 smraw_test_handle_get_filename_size,
		 handle );

		/* TODO: add tests for libsmraw_handle_get_filename */

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_filename_size_wide",
		 smraw_test_handle_get_filename_size_wide,
		 handle );

		/* TODO: add tests for libsmraw_handle_get_filename_wide */

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( TODO )

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_file_io_handle",
		 smraw_test_handle_get_file_io_handle,
		 handle );

#endif /* defined( TODO ) */

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_media_size",
		 smraw_test_handle_get_media_size,
		 handle );

		/* TODO: add tests for libsmraw_handle_set_media_size */

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_bytes_per_sector",
		 smraw_test_handle_get_bytes_per_sector,
		 handle );

		/* TODO: add tests for libsmraw_handle_set_bytes_per_sector */

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_media_type",
		 smraw_test_handle_get_media_type,
		 handle );

		/* TODO: add tests for libsmraw_handle_set_media_type */

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_media_flags",
		 smraw_test_handle_get_media_flags,
		 handle );

		/* TODO: add tests for libsmraw_handle_set_media_flags */

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_number_of_information_values",
		 smraw_test_handle_get_number_of_information_values,
		 handle );

		/* TODO: add tests for libsmraw_handle_get_information_value_identifier_size */

		/* TODO: add tests for libsmraw_handle_get_information_value_identifier */

		/* TODO: add tests for libsmraw_handle_get_utf8_information_value_size */

		/* TODO: add tests for libsmraw_handle_get_utf8_information_value */

		/* TODO: add tests for libsmraw_handle_set_utf8_information_value */

		/* TODO: add tests for libsmraw_handle_get_utf16_information_value_size */

		/* TODO: add tests for libsmraw_handle_get_utf16_information_value */

		/* TODO: add tests for libsmraw_handle_set_utf16_information_value */

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_number_of_integrity_hash_values",
		 smraw_test_handle_get_number_of_integrity_hash_values,
		 handle );

		/* TODO: add tests for libsmraw_handle_get_integrity_hash_value_identifier_size */

		/* TODO: add tests for libsmraw_handle_get_integrity_hash_value_identifier */

		/* TODO: add tests for libsmraw_handle_get_utf8_integrity_hash_value_size */

		/* TODO: add tests for libsmraw_handle_get_utf8_integrity_hash_value */

		/* TODO: add tests for libsmraw_handle_set_utf8_integrity_hash_value */

		/* TODO: add tests for libsmraw_handle_get_utf16_integrity_hash_value_size */

		/* TODO: add tests for libsmraw_handle_get_utf16_integrity_hash_value */

		/* TODO: add tests for libsmraw_handle_set_utf16_integrity_hash_value */

		/* Clean up
		 */
		result = smraw_test_handle_close_source(
		          &handle,
		          &error );

		SMRAW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "handle",
		 handle );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libbfio_pool_free(
		          &file_io_pool,
		          &error );

		SMRAW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		SMRAW_TEST_ASSERT_IS_NULL(
	         "file_io_pool",
	         file_io_pool );

	        SMRAW_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libsmraw_glob_wide_free(
		          filenames,
		          number_of_filenames,
		          &error );
#else
		result = libsmraw_glob_free(
		          filenames,
		          number_of_filenames,
		          &error );
#endif

		SMRAW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libsmraw_handle_free(
		 &handle,
		 NULL );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( filenames != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
	return( EXIT_FAILURE );
}

