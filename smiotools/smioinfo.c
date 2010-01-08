/*
 * smioinfo
 * Shows information stored in storage media file(s)
 *
 * Copyright (c) 2006-2010, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

/* If libtool DLL support is enabled set LIBSMIO_DLL_IMPORT
 * before including libsmio.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBSMIO_DLL_IMPORT
#endif

#include <libsmio.h>

#include <libsystem.h>

#include "byte_size_string.h"
#include "smioinput.h"
#include "smiooutput.h"
#include "guid.h"
#include "info_handle.h"

info_handle_t *smioinfo_info_handle = NULL;
int smioinfo_abort                  = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use smioinfo to determine information about the storage media file(s).\n\n" );

	fprintf( stream, "Usage: smioinfo [ -ehimvV ] files\n\n" );

	fprintf( stream, "\tfiles: the first or the entire set of storage media segment files\n\n" );

	fprintf( stream, "\t-e:    only show read error information\n" );
	fprintf( stream, "\t-h:    shows this help\n" );
	fprintf( stream, "\t-i:    only show imaging information\n" );
	fprintf( stream, "\t-m:    only show media information\n" );
	fprintf( stream, "\t-v:    verbose output to stderr\n" );
	fprintf( stream, "\t-V:    print version\n" );
}

/* Signal handler for smioinfo
 */
void smioinfo_signal_handler(
      libsystem_signal_t signal )
{
	liberror_error_t *error = NULL;
	static char *function   = "smioinfo_signal_handler";

	smioinfo_abort = 1;

	if( ( smioinfo_info_handle != NULL )
	 && ( info_handle_signal_abort(
	       smioinfo_info_handle,
	       &error ) != 1 ) )
	{
		libsystem_notify_printf(
		 "%s: unable to signal info handle to abort.\n",
		 function );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return;
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
	if( libsystem_file_io_close(
	     0 ) != 0 )
	{
		libsystem_notify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libsystem_character_t * const *argv_filenames = NULL;

#if !defined( HAVE_GLOB_H )
	libsystem_glob_t *glob                        = NULL;
#endif
	info_handle_t *info_handle                    = NULL;
	liberror_error_t *error                       = NULL;

	libsystem_character_t *program                = _LIBSYSTEM_CHARACTER_T_STRING( "smioinfo" );
	libsystem_integer_t option                    = 0;
	uint8_t verbose                               = 0;
	char info_option                              = 'a';
	int amount_of_filenames                       = 0;
	int result                                    = 0;

	libsystem_notify_set_stream(
	 stderr,
	 NULL );
	libsystem_notify_set_verbose(
	 1 );

	if( libsystem_initialize(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	smiooutput_version_fprint(
	 stdout,
	 program );

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBSYSTEM_CHARACTER_T_STRING( "ehimvV" ) ) ) != (libsystem_integer_t) -1 )
	{
		switch( option )
		{
			case (libsystem_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBSYSTEM "\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libsystem_integer_t) 'e':
				if( info_option != 'a' )
				{
					fprintf(
					 stderr,
					 "Conflicting options: %" PRIc_LIBSYSTEM " and %c\n",
					 option,
					 info_option );

					usage_fprint(
					 stdout );

					return( EXIT_FAILURE );
				}
				info_option = 'e';

				break;

			case (libsystem_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libsystem_integer_t) 'i':
				if( info_option != 'a' )
				{
					fprintf(
					 stderr,
					 "Conflicting options: %" PRIc_LIBSYSTEM " and %c\n",
					 option,
					 info_option );

					usage_fprint(
					 stdout );

					return( EXIT_FAILURE );
				}
				info_option = 'i';

				break;

			case (libsystem_integer_t) 'm':
				if( info_option != 'a' )
				{
					fprintf(
					 stderr,
					 "Conflicting options: %" PRIc_LIBSYSTEM " and %c\n",
					 option,
					 info_option );

					usage_fprint(
					 stdout );

					return( EXIT_FAILURE );
				}
				info_option = 'm';

				break;

			case (libsystem_integer_t) 'v':
				verbose = 1;

				break;

			case (libsystem_integer_t) 'V':
				smiooutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing image file(s).\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	libsystem_notify_set_verbose(
	 verbose );
	libsmio_notify_set_stream(
	 stderr,
	 NULL );
	libsmio_notify_set_verbose(
	 verbose );

	if( libsystem_signal_attach(
	     smioinfo_signal_handler,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
#if !defined( HAVE_GLOB_H )
	if( libsystem_glob_initialize(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize glob.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( libsystem_glob_resolve(
	     glob,
	     &argv[ optind ],
	     argc - optind,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to resolve glob.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		libsystem_glob_free(
		 &glob,
		 NULL );

		return( EXIT_FAILURE );
	}
	argv_filenames      = glob->result;
	amount_of_filenames = glob->amount_of_results;
#else
	argv_filenames      = &argv[ optind ];
	amount_of_filenames = argc - optind;

#endif

	if( info_handle_initialize(
	     &info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create info handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

#if !defined( HAVE_GLOB_H )
		libsystem_glob_free(
		 &glob,
		 NULL );
#endif

		return( EXIT_FAILURE );
	}
	result = info_handle_open_input(
	          info_handle,
	          argv_filenames,
	          amount_of_filenames,
	          &error );

#if !defined( HAVE_GLOB_H )
	if( libsystem_glob_free(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
#endif

	if( ( smioinfo_abort == 0 )
	 && ( result != 1 ) )
	{
		fprintf(
		 stderr,
		 "Unable to open storage media file(s)" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		info_handle_free(
		 &info_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( ( smioinfo_abort == 0 )
	 && ( ( info_option == 'a' )
	  || ( info_option == 'i' ) ) )
	{
		if( info_handle_information_values_fprint(
		     info_handle,
		     stdout,
		     &error ) != 1 )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	if( ( smioinfo_abort == 0 )
	 && ( ( info_option == 'a' )
	  || ( info_option == 'm' ) ) )
	{
		if( info_handle_media_information_fprint(
		     info_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print media information.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
		if( info_handle_integrity_hash_values_fprint(
		     info_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print hash values.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
		if( info_handle_sessions_fprint(
		     info_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print sessions.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	if( ( smioinfo_abort == 0 )
	 && ( ( info_option == 'a' )
	  || ( info_option == 'e' ) ) )
	{
		if( info_handle_imaging_errors_fprint(
		     info_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print imaging errors.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	if( libsystem_signal_detach(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	if( info_handle_close(
	     info_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close storage media file(s).\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		info_handle_free(
		 &info_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( info_handle_free(
	     &info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free info handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( smioinfo_abort != 0 )
	{
		fprintf(
		 stdout,
		 "%s: ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

