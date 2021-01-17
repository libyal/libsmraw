/*
 * Storage media (SM) RAW library glob testing program
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
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "smraw_test_libsmraw.h"

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wchar_t **filenames     = NULL;
#else
	char **filenames        = NULL;
#endif
	libsmraw_error_t *error = NULL;
	size_t string_length    = 0;
	int number_of_filenames = 0;
	int filename_iterator   = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing filename(s).\n" );

		return( EXIT_FAILURE );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	string_length = wide_string_length(
	                 argv[ 1 ] );

	if( libsmraw_glob_wide(
	     argv[ 1 ],
	     string_length,
	     &filenames,
	     &number_of_filenames,
	     &error ) != 1 )
#else
	string_length = narrow_string_length(
	                 argv[ 1 ] );

	if( libsmraw_glob(
	     argv[ 1 ],
	     string_length,
	     &filenames,
	     &number_of_filenames,
	     &error ) != 1 )
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
	for( filename_iterator = 0;
	     filename_iterator < number_of_filenames;
	     filename_iterator++ )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		fprintf(
		 stdout,
		 "%ls",
		 filenames[ filename_iterator ] );
#else
		fprintf(
		 stdout,
		 "%s",
		 filenames[ filename_iterator ] );
#endif
		if( filename_iterator == ( number_of_filenames - 1 ) )
		{
			fprintf(
			 stdout,
			 "\n" );
		}
		else
		{
			fprintf(
			 stdout,
			 " " );
		}
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_glob_wide_free(
	     filenames,
	     number_of_filenames,
	     &error ) != 1 )
#else
	if( libsmraw_glob_free(
	     filenames,
	     number_of_filenames,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

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

