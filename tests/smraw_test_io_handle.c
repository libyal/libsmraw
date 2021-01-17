/*
 * Library io_handle type test program
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "smraw_test_libcerror.h"
#include "smraw_test_libsmraw.h"
#include "smraw_test_macros.h"
#include "smraw_test_memory.h"
#include "smraw_test_unused.h"

#include "../libsmraw/libsmraw_io_handle.h"

#if defined( __GNUC__ ) && !defined( LIBSMRAW_DLL_IMPORT )

/* Tests the libsmraw_io_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_io_handle_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libsmraw_io_handle_t *io_handle = NULL;
	int result                      = 0;

#if defined( HAVE_SMRAW_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libsmraw_io_handle_initialize(
	          &io_handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmraw_io_handle_free(
	          &io_handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_io_handle_initialize(
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

	io_handle = (libsmraw_io_handle_t *) 0x12345678UL;

	result = libsmraw_io_handle_initialize(
	          &io_handle,
	          &error );

	io_handle = NULL;

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
		/* Test libsmraw_io_handle_initialize with malloc failing
		 */
		smraw_test_malloc_attempts_before_fail = test_number;

		result = libsmraw_io_handle_initialize(
		          &io_handle,
		          &error );

		if( smraw_test_malloc_attempts_before_fail != -1 )
		{
			smraw_test_malloc_attempts_before_fail = -1;

			if( io_handle != NULL )
			{
				libsmraw_io_handle_free(
				 &io_handle,
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
			 "io_handle",
			 io_handle );

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
		/* Test libsmraw_io_handle_initialize with memset failing
		 */
		smraw_test_memset_attempts_before_fail = test_number;

		result = libsmraw_io_handle_initialize(
		          &io_handle,
		          &error );

		if( smraw_test_memset_attempts_before_fail != -1 )
		{
			smraw_test_memset_attempts_before_fail = -1;

			if( io_handle != NULL )
			{
				libsmraw_io_handle_free(
				 &io_handle,
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
			 "io_handle",
			 io_handle );

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
	if( io_handle != NULL )
	{
		libsmraw_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmraw_io_handle_free function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_io_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libsmraw_io_handle_free(
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

/* Tests the libsmraw_io_handle_clear function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_io_handle_clear(
     void )
{
	libcerror_error_t *error        = NULL;
	libsmraw_io_handle_t *io_handle = NULL;
	int result                      = 0;

	/* Initialize test
	 */
	result = libsmraw_io_handle_initialize(
	          &io_handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libsmraw_io_handle_clear(
	          io_handle,
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
	result = libsmraw_io_handle_clear(
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

#if defined( HAVE_SMRAW_TEST_MEMORY )

	/* Test libsmraw_io_handle_clear with memset failing
	 */
	smraw_test_memset_attempts_before_fail = 0;

	result = libsmraw_io_handle_clear(
	          io_handle,
	          &error );

	if( smraw_test_memset_attempts_before_fail != -1 )
	{
		smraw_test_memset_attempts_before_fail = -1;
	}
	else
	{
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
#endif /* defined( HAVE_SMRAW_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libsmraw_io_handle_free(
	          &io_handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( io_handle != NULL )
	{
		libsmraw_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmraw_io_handle_get_basename_size function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_io_handle_get_basename_size(
     void )
{
	libcerror_error_t *error        = NULL;
	libsmraw_io_handle_t *io_handle = NULL;
	size_t basename_size            = 0;
	int basename_size_is_set        = 0;
	int result                      = 0;

	/* Initialize test
	 */
	result = libsmraw_io_handle_initialize(
	          &io_handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libsmraw_io_handle_get_basename_size(
	          io_handle,
	          &basename_size,
	          &error );

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	basename_size_is_set = result;

	/* Test error cases
	 */
	result = libsmraw_io_handle_get_basename_size(
	          NULL,
	          &basename_size,
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

	if( basename_size_is_set != 0 )
	{
		result = libsmraw_io_handle_get_basename_size(
		          io_handle,
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
	/* Clean up
	 */
	result = libsmraw_io_handle_free(
	          &io_handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( io_handle != NULL )
	{
		libsmraw_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libsmraw_io_handle_get_basename_size_wide function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_io_handle_get_basename_size_wide(
     void )
{
	libcerror_error_t *error        = NULL;
	libsmraw_io_handle_t *io_handle = NULL;
	size_t basename_size_wide       = 0;
	int basename_size_wide_is_set   = 0;
	int result                      = 0;

	/* Initialize test
	 */
	result = libsmraw_io_handle_initialize(
	          &io_handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libsmraw_io_handle_get_basename_size_wide(
	          io_handle,
	          &basename_size_wide,
	          &error );

	SMRAW_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	basename_size_wide_is_set = result;

	/* Test error cases
	 */
	result = libsmraw_io_handle_get_basename_size_wide(
	          NULL,
	          &basename_size_wide,
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

	if( basename_size_wide_is_set != 0 )
	{
		result = libsmraw_io_handle_get_basename_size_wide(
		          io_handle,
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
	/* Clean up
	 */
	result = libsmraw_io_handle_free(
	          &io_handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( io_handle != NULL )
	{
		libsmraw_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libsmraw_io_handle_get_maximum_segment_size function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_io_handle_get_maximum_segment_size(
     void )
{
	libcerror_error_t *error        = NULL;
	libsmraw_io_handle_t *io_handle = NULL;
	size64_t maximum_segment_size   = 0;
	int maximum_segment_size_is_set = 0;
	int result                      = 0;

	/* Initialize test
	 */
	result = libsmraw_io_handle_initialize(
	          &io_handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libsmraw_io_handle_get_maximum_segment_size(
	          io_handle,
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
	result = libsmraw_io_handle_get_maximum_segment_size(
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
		result = libsmraw_io_handle_get_maximum_segment_size(
		          io_handle,
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
	/* Clean up
	 */
	result = libsmraw_io_handle_free(
	          &io_handle,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( io_handle != NULL )
	{
		libsmraw_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBSMRAW_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc SMRAW_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] SMRAW_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc SMRAW_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] SMRAW_TEST_ATTRIBUTE_UNUSED )
#endif
{
	SMRAW_TEST_UNREFERENCED_PARAMETER( argc )
	SMRAW_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBSMRAW_DLL_IMPORT )

	SMRAW_TEST_RUN(
	 "libsmraw_io_handle_initialize",
	 smraw_test_io_handle_initialize );

	SMRAW_TEST_RUN(
	 "libsmraw_io_handle_free",
	 smraw_test_io_handle_free );

	SMRAW_TEST_RUN(
	 "libsmraw_io_handle_clear",
	 smraw_test_io_handle_clear );

	/* TODO: add tests for libsmraw_io_handle_create_segment */

	/* TODO: add tests for libsmraw_io_handle_read_segment_data */

	/* TODO: add tests for libsmraw_io_handle_write_segment_data */

	/* TODO: add tests for libsmraw_io_handle_seek_segment_offset */

#if defined( TODO )

	SMRAW_TEST_RUN(
	 "libsmraw_io_handle_get_basename_size",
	 smraw_test_io_handle_get_basename_size );

	/* TODO: add tests for libsmraw_io_handle_get_basename */

	/* TODO: add tests for libsmraw_io_handle_set_basename */

#if defined( HAVE_WIDE_CHARACTER_TYPE )

	SMRAW_TEST_RUN(
	 "libsmraw_io_handle_get_basename_size_wide",
	 smraw_test_io_handle_get_basename_size_wide );

	/* TODO: add tests for libsmraw_io_handle_get_basename_wide */

	/* TODO: add tests for libsmraw_io_handle_set_basename_wide */

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#endif /* defined( TODO ) */

	SMRAW_TEST_RUN(
	 "libsmraw_io_handle_get_maximum_segment_size",
	 smraw_test_io_handle_get_maximum_segment_size );

	/* TODO: add tests for libsmraw_io_handle_set_maximum_segment_size */

#endif /* defined( __GNUC__ ) && !defined( LIBSMRAW_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

