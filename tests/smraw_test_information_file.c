/*
 * Library information_file type test program
 *
 * Copyright (C) 2010-2022, Joachim Metz <joachim.metz@gmail.com>
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

#include "smraw_test_functions.h"
#include "smraw_test_libcerror.h"
#include "smraw_test_libsmraw.h"
#include "smraw_test_macros.h"
#include "smraw_test_memory.h"
#include "smraw_test_unused.h"

#include "../libsmraw/libsmraw_information_file.h"

#if defined( __GNUC__ ) && !defined( LIBSMRAW_DLL_IMPORT )

/* Tests the libsmraw_information_file_initialize function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_information_file_initialize(
     void )
{
	libcerror_error_t *error                      = NULL;
	libsmraw_information_file_t *information_file = NULL;
	int result                                    = 0;

#if defined( HAVE_SMRAW_TEST_MEMORY )
	int number_of_malloc_fail_tests               = 1;
	int number_of_memset_fail_tests               = 1;
	int test_number                               = 0;
#endif

	/* Test regular cases
	 */
	result = libsmraw_information_file_initialize(
	          &information_file,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "information_file",
	 information_file );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmraw_information_file_free(
	          &information_file,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "information_file",
	 information_file );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmraw_information_file_initialize(
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

	information_file = (libsmraw_information_file_t *) 0x12345678UL;

	result = libsmraw_information_file_initialize(
	          &information_file,
	          &error );

	information_file = NULL;

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
		/* Test libsmraw_information_file_initialize with malloc failing
		 */
		smraw_test_malloc_attempts_before_fail = test_number;

		result = libsmraw_information_file_initialize(
		          &information_file,
		          &error );

		if( smraw_test_malloc_attempts_before_fail != -1 )
		{
			smraw_test_malloc_attempts_before_fail = -1;

			if( information_file != NULL )
			{
				libsmraw_information_file_free(
				 &information_file,
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
			 "information_file",
			 information_file );

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
		/* Test libsmraw_information_file_initialize with memset failing
		 */
		smraw_test_memset_attempts_before_fail = test_number;

		result = libsmraw_information_file_initialize(
		          &information_file,
		          &error );

		if( smraw_test_memset_attempts_before_fail != -1 )
		{
			smraw_test_memset_attempts_before_fail = -1;

			if( information_file != NULL )
			{
				libsmraw_information_file_free(
				 &information_file,
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
			 "information_file",
			 information_file );

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
	if( information_file != NULL )
	{
		libsmraw_information_file_free(
		 &information_file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmraw_information_file_free function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_information_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libsmraw_information_file_free(
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

/* Tests the libsmraw_information_file_open function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_information_file_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error                      = NULL;
	libsmraw_information_file_t *information_file = NULL;
	int result                                    = 0;

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

	result = libsmraw_information_file_initialize(
	          &information_file,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "information_file",
	 information_file );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libsmraw_information_file_open(
	          information_file,
	          narrow_source,
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
	result = libsmraw_information_file_open(
	          information_file,
	          narrow_source,
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
	result = libsmraw_information_file_free(
	          &information_file,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "information_file",
	 information_file );

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
	if( information_file != NULL )
	{
		libsmraw_information_file_free(
		 &information_file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmraw_information_file_close function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_information_file_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libsmraw_information_file_close(
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
	 "libsmraw_information_file_initialize",
	 smraw_test_information_file_initialize );

	SMRAW_TEST_RUN(
	 "libsmraw_information_file_free",
	 smraw_test_information_file_free );

	/* TODO: add tests for libsmraw_information_file_set_name */

	/* TODO: add tests for libsmraw_information_file_read_section */

	/* TODO: add tests for libsmraw_information_file_write_section */

#endif /* defined( __GNUC__ ) && !defined( LIBSMRAW_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

