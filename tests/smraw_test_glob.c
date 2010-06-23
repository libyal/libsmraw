/*
 * Storage media (SM) RAW library glob testing program
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

#include <libcstring.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include <libsmraw.h>

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	wchar_t **filenames     = NULL;
#else
	char **filenames        = NULL;
#endif
	libsmraw_error_t *error = NULL;
	int number_of_filenames = 0;
	int filename_iterator   = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing filename(s).\n" );

		return( EXIT_FAILURE );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_glob_wide(
	     argv[ 1 ],
	     libcstring_wide_string_length(
	      argv[ 1 ] ),
	     &filenames,
	     &number_of_filenames,
	     &error ) != 1 )
#else
	if( libsmraw_glob(
	     argv[ 1 ],
	     libcstring_narrow_string_length(
	      argv[ 1 ] ),
	     &filenames,
	     &number_of_filenames,
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

		return( EXIT_FAILURE );
	}
	if( number_of_filenames < 0 )
	{
		fprintf(
		 stderr,
		 "Invalid number of filenames.\n" );

		return( EXIT_FAILURE );
	}
	else if( number_of_filenames == 0 )
	{
		fprintf(
		 stderr,
		 "Missing filenames.\n" );

		return( EXIT_FAILURE );
	}
	for( filename_iterator = 0;
	     filename_iterator < number_of_filenames;
	     filename_iterator++ )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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

		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

