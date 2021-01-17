/*
 * Library filename functions test program
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
#include "smraw_test_libsmraw.h"
#include "smraw_test_macros.h"
#include "smraw_test_memory.h"
#include "smraw_test_unused.h"

#include "../libsmraw/libsmraw_filename.h"

#if defined( __GNUC__ ) && !defined( LIBSMRAW_DLL_IMPORT )

/* Tests the libsmraw_filename_create function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_filename_create(
     void )
{
	libcerror_error_t *error     = NULL;
	system_character_t *filename = NULL;
	size_t filename_size         = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	result = libsmraw_filename_create(
	          &filename,
	          &filename_size,
	          _SYSTEM_STRING( "test" ),
	          5,
	          10,
	          3,
	          &error );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMRAW_TEST_ASSERT_IS_NOT_NULL(
	 "filename",
	 filename );

	SMRAW_TEST_ASSERT_EQUAL_SIZE(
	 "filename_size",
	 filename_size,
	 (size_t) 13 );

	SMRAW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = system_string_compare(
	          _SYSTEM_STRING( "test.raw.003" ),
	          filename,
	          12 );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 filename );

	filename = NULL;

	/* Test error cases
	 */
	result = libsmraw_filename_create(
	          NULL,
	          &filename_size,
	          _SYSTEM_STRING( "test" ),
	          5,
	          10,
	          3,
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

	result = libsmraw_filename_create(
	          &filename,
	          NULL,
	          _SYSTEM_STRING( "test" ),
	          5,
	          10,
	          3,
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

	result = libsmraw_filename_create(
	          &filename,
	          &filename_size,
	          NULL,
	          5,
	          10,
	          3,
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

	result = libsmraw_filename_create(
	          &filename,
	          &filename_size,
	          _SYSTEM_STRING( "test" ),
	          (size_t) SSIZE_MAX + 1,
	          10,
	          3,
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

	result = libsmraw_filename_create(
	          &filename,
	          &filename_size,
	          _SYSTEM_STRING( "test" ),
	          5,
	          -1,
	          3,
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

	result = libsmraw_filename_create(
	          &filename,
	          &filename_size,
	          _SYSTEM_STRING( "test" ),
	          5,
	          10,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( filename != NULL )
	{
		memory_free(
		 filename );
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
	 "libsmraw_filename_create",
	 smraw_test_filename_create );

#endif /* defined( __GNUC__ ) && !defined( LIBSMRAW_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

