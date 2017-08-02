/*
 * Library handle type test program
 *
 * Copyright (C) 2010-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "smraw_test_getopt.h"
#include "smraw_test_libbfio.h"
#include "smraw_test_libcerror.h"
#include "smraw_test_libclocale.h"
#include "smraw_test_libsmraw.h"
#include "smraw_test_libuna.h"
#include "smraw_test_macros.h"
#include "smraw_test_memory.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make smraw_test_handle generate verbose output
#define SMRAW_TEST_HANDLE_VERBOSE
 */

/* Retrieves source as a narrow string
 * Returns 1 if successful or -1 on error
 */
int smraw_test_handle_get_narrow_source(
     const system_character_t *source,
     char *narrow_string,
     size_t narrow_string_size,
     libcerror_error_t **error )
{
	static char *function     = "smraw_test_handle_get_narrow_source";
	size_t narrow_source_size = 0;
	size_t source_length      = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result                = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( narrow_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid narrow string.",
		 function );

		return( -1 );
	}
	if( narrow_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid narrow string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine narrow string size.",
		 function );

		return( -1 );
	}
#else
	narrow_source_size = source_length + 1;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( narrow_string_size < narrow_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: narrow string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
#else
	if( system_string_copy(
	     narrow_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
	narrow_string[ source_length ] = 0;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves source as a wide string
 * Returns 1 if successful or -1 on error
 */
int smraw_test_handle_get_wide_source(
     const system_character_t *source,
     wchar_t *wide_string,
     size_t wide_string_size,
     libcerror_error_t **error )
{
	static char *function   = "smraw_test_handle_get_wide_source";
	size_t source_length    = 0;
	size_t wide_source_size = 0;

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result              = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( wide_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wide string.",
		 function );

		return( -1 );
	}
	if( wide_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid wide string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wide_source_size = source_length + 1;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine wide string size.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( wide_string_size < wide_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: wide string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( system_string_copy(
	     wide_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}
	wide_string[ source_length ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Creates and opens a source handle
 * Returns 1 if successful or -1 on error
 */
int smraw_test_handle_open_source(
     libsmraw_handle_t **handle,
     const system_character_t *source,
     libcerror_error_t **error )
{
	system_character_t **filenames = NULL;
	static char *function          = "smraw_test_handle_open_source";
	size_t source_length           = 0;
	int number_of_filenames        = 0;
	int result                     = 0;

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
	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libsmraw_glob_wide(
	          source,
	          source_length,
	          &filenames,
	          &number_of_filenames,
	          error );
#else
	result = libsmraw_glob(
	          source,
	          source_length,
	          &filenames,
	          &number_of_filenames,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to resolve filename(s).",
		 function );

		goto on_error;
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libsmraw_handle_open_wide(
	          *handle,
	          (wchar_t * const *) filenames,
	          number_of_filenames,
	          LIBSMRAW_OPEN_READ,
	          error );
#else
	result = libsmraw_handle_open(
	          *handle,
	          (char * const *) filenames,
	          number_of_filenames,
	          LIBSMRAW_OPEN_READ,
	          error );
#endif
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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libsmraw_glob_wide_free(
	          filenames,
	          number_of_filenames,
	          error );
#else
	result = libsmraw_glob_free(
	          filenames,
	          number_of_filenames,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free globbed filenames.",
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

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        SMRAW_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	handle = NULL;

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

/* Tests the libsmraw_handle_open functions
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
	result = smraw_test_handle_get_narrow_source(
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
	result = smraw_test_handle_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

SMRAW_TEST_FPRINT_ERROR( error );

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

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
	uint8_t buffer[ 16 ];

	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	ssize_t read_count       = 0;
	off64_t offset           = 0;

	/* Determine size
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
	if( size > 16 )
	{
		read_count = libsmraw_handle_read_buffer(
		              handle,
		              buffer,
		              16,
		              &error );

		SMRAW_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 16 );

		SMRAW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
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
	read_count = libsmraw_handle_read_buffer(
	              NULL,
	              buffer,
	              16,
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
	int offset_is_set        = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libsmraw_handle_get_offset(
	          handle,
	          &offset,
	          &error );

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset_is_set = result;

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

	if( offset_is_set != 0 )
	{
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

/* Tests the libsmraw_handle_get_segment_filename_size function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_segment_filename_size(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error         = NULL;
	size_t segment_filename_size     = 0;
	int result                       = 0;
	int segment_filename_size_is_set = 0;

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
	size_t segment_filename_size_wide     = 0;
	int result                            = 0;
	int segment_filename_size_wide_is_set = 0;

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
	libcerror_error_t *error        = NULL;
	size64_t maximum_segment_size   = 0;
	int maximum_segment_size_is_set = 0;
	int result                      = 0;

	/* Test regular cases
	 */
	result = libsmraw_handle_get_maximum_segment_size(
	          handle,
	          &maximum_segment_size,
	          &error );

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	maximum_segment_size_is_set = result;

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

	if( maximum_segment_size_is_set != 0 )
	{
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

/* Tests the libsmraw_handle_get_filename_size function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_filename_size(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size_t filename_size     = 0;
	off64_t offset           = 0;
	int filename_size_is_set = 0;
	int result               = 0;

	/* Determine size
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

	/* libsmraw_handle_get_filename_size will fail on an empty file
	 */
	if( offset == 0 )
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

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	filename_size_is_set = result;

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

	if( filename_size_is_set != 0 )
	{
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

/* Tests the libsmraw_handle_get_filename_size_wide function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_filename_size_wide(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error      = NULL;
	size_t filename_size_wide     = 0;
	off64_t offset                = 0;
	int filename_size_wide_is_set = 0;
	int result                    = 0;

	/* Determine size
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

	/* libsmraw_handle_get_filename_size_wide will fail on an empty file
	 */
	if( offset == 0 )
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

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	filename_size_wide_is_set = result;

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

	if( filename_size_wide_is_set != 0 )
	{
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

#ifdef TODO

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

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

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

#endif /* TODO */

/* Tests the libsmraw_handle_get_number_of_information_values functions
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_number_of_information_values(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error         = NULL;
	int number_of_information_values = 0;
	int result                       = 0;

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

/* Tests the libsmraw_handle_get_number_of_integrity_hash_values functions
 * Returns 1 if successful or 0 if not
 */
int smraw_test_handle_get_number_of_integrity_hash_values(
     libsmraw_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int number_of_integrity_hash_values    = 0;
	int result               = 0;

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
	libcerror_error_t *error   = NULL;
	system_character_t *source = NULL;
	libsmraw_handle_t *handle        = NULL;
	system_integer_t option    = 0;
	int result                 = 0;

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

#if defined( LIBSMRAW_HAVE_BFIO )

		/* TODO add test for libsmraw_handle_open_file_io_handle */

#endif /* defined( LIBSMRAW_HAVE_BFIO ) */

		SMRAW_TEST_RUN(
		 "libsmraw_handle_close",
		 smraw_test_handle_close );

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_open_close",
		 smraw_test_handle_open_close,
		 source );

		/* Initialize test
		 */
		result = smraw_test_handle_open_source(
		          &handle,
		          source,
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

		/* TODO: add tests for libsmraw_handle_open_file_io_pool */

#if defined( __GNUC__ ) && !defined( LIBSMRAW_DLL_IMPORT )

		/* TODO: add tests for libsmraw_handle_read_information_file */

#endif /* defined( __GNUC__ ) && !defined( LIBSMRAW_DLL_IMPORT ) */

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_read_buffer",
		 smraw_test_handle_read_buffer,
		 handle );

		/* TODO: add tests for libsmraw_handle_read_buffer_at_offset */

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

#ifdef TODO
		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_file_io_handle",
		 smraw_test_handle_get_file_io_handle,
		 handle );
#endif /* TODO */

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_number_of_information_values",
		 smraw_test_handle_get_number_of_information_values,
		 handle );

		SMRAW_TEST_RUN_WITH_ARGS(
		 "libsmraw_handle_get_number_of_integrity_hash_values",
		 smraw_test_handle_get_number_of_integrity_hash_values,
		 handle );

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
		smraw_test_handle_close_source(
		 &handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

