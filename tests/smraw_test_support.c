/*
 * Library support functions test program
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "smraw_test_libcerror.h"
#include "smraw_test_libsmraw.h"
#include "smraw_test_macros.h"
#include "smraw_test_unused.h"

/* Tests the libsmraw_get_version function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_get_version(
     void )
{
	const char *version_string = NULL;
	int result                 = 0;

	version_string = libsmraw_get_version();

	result = narrow_string_compare(
	          version_string,
	          LIBSMRAW_VERSION_STRING,
	          9 );

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libsmraw_get_access_flags_read function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_get_access_flags_read(
     void )
{
	int access_flags = 0;

	access_flags = libsmraw_get_access_flags_read();

	SMRAW_TEST_ASSERT_EQUAL_INT(
	 "access_flags",
	 access_flags,
	 LIBSMRAW_ACCESS_FLAG_READ );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libsmraw_get_codepage function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_get_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int codepage             = 0;
	int result               = 0;

	result = libsmraw_get_codepage(
	          &codepage,
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
	result = libsmraw_get_codepage(
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

/* Tests the libsmraw_set_codepage function
 * Returns 1 if successful or 0 if not
 */
int smraw_test_set_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = libsmraw_set_codepage(
	          0,
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
	result = libsmraw_set_codepage(
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
	return( 0 );
}

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

	SMRAW_TEST_RUN(
	 "libsmraw_get_version",
	 smraw_test_get_version );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

