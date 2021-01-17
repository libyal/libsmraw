/*
 * Verifies the integrity of a storage media (split) RAW image file
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
#include <system_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_GLOB_H )
#include <glob.h>
#endif

#include "byte_size_string.h"
#include "digest_hash.h"
#include "log_handle.h"
#include "smrawtools_getopt.h"
#include "smrawtools_glob.h"
#include "smrawtools_libcerror.h"
#include "smrawtools_libclocale.h"
#include "smrawtools_libcnotify.h"
#include "smrawtools_libsmraw.h"
#include "smrawtools_output.h"
#include "smrawtools_signal.h"
#include "smrawtools_unused.h"
#include "verification_handle.h"

verification_handle_t *smrawverify_verification_handle = NULL;
int smrawverify_abort                                  = 0;

/* Prints the executable usage information to the stream
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use smrawverify to verify data stored in the storage media (split)\n"
	                 "RAW image file format.\n\n" );

	fprintf( stream, "Usage: smrawverify [ -d digest_type ] [ -l log_filename ]\n"
	                 "                   [ -p process_buffer_size ] [ -hqvV ]\n"
	                 "                   smraw_files\n\n" );

	fprintf( stream, "\tsmraw_files: the first or the entire set of SMRAW segment files\n\n" );

	fprintf( stream, "\t-d:          calculate additional digest (hash) types besides md5,\n"
	                 "\t             options: sha1, sha256\n" );
	fprintf( stream, "\t-h:          shows this help\n" );
	fprintf( stream, "\t-l:          logs verification errors and the digest (hash) to the\n"
	                 "\t             log_filename\n" );
	fprintf( stream, "\t-p:          specify the process buffer size (default is the 32768)\n" );
	fprintf( stream, "\t-q:          quiet shows minimal status information\n" );
	fprintf( stream, "\t-v:          verbose output to stderr\n" );
	fprintf( stream, "\t-V:          print version\n" );
}

/* Signal handler for smrawverify
 */
void smrawverify_signal_handler(
      smrawtools_signal_t signal SMRAWTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "smrawverify_signal_handler";

	SMRAWTOOLS_UNREFERENCED_PARAMETER( signal )

	smrawverify_abort = 1;

	if( smrawverify_verification_handle != NULL )
	{
		if( verification_handle_signal_abort(
		     smrawverify_verification_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal verification handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error                           = NULL;

	system_character_t * const *argv_filenames         = NULL;

#if !defined( HAVE_GLOB_H )
	smrawtools_glob_t *glob                            = NULL;
#endif

	log_handle_t *log_handle                           = NULL;
	system_character_t *log_filename                   = NULL;
	system_character_t *option_additional_digest_types = NULL;
	system_character_t *option_process_buffer_size     = NULL;
	system_character_t *program                        = _SYSTEM_STRING( "smrawverify" );
	system_integer_t option                            = 0;
	uint8_t calculate_md5                              = 1;
	uint8_t print_status_information                   = 1;
	uint8_t verbose                                    = 0;
	int number_of_filenames                            = 0;
	int result                                         = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "smrawtools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
	if( smrawtools_output_initialize(
	     _IONBF,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	smrawtools_output_version_fprint(
	 stdout,
	 program );

	while( ( option = smrawtools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "d:hl:p:qvV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				goto on_error;

			case (system_integer_t) 'd':
				option_additional_digest_types = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (system_integer_t) 'p':
				option_process_buffer_size = optarg;

				break;

			case (system_integer_t) 'q':
				print_status_information = 0;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				smrawtools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing SMRAW image file(s).\n" );

		usage_fprint(
		 stdout );

		goto on_error;
	}
	libcnotify_verbose_set(
	 verbose );

#if !defined( HAVE_LOCAL_LIBSMRAW )
	libsmraw_notify_set_verbose(
	 verbose );
	libsmraw_notify_set_stream(
	 stderr,
	 NULL );
#endif

	if( verification_handle_initialize(
	     &smrawverify_verification_handle,
	     calculate_md5,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create verification handle.\n" );

		goto on_error;
	}
	if( option_process_buffer_size != NULL )
	{
		result = verification_handle_set_process_buffer_size(
			  smrawverify_verification_handle,
			  option_process_buffer_size,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set process buffer size.\n" );

			goto on_error;
		}
		else if( ( result == 0 )
		      || ( smrawverify_verification_handle->process_buffer_size > (size_t) SSIZE_MAX ) )
		{
			smrawverify_verification_handle->process_buffer_size = 0;

			fprintf(
			 stderr,
			 "Unsupported process buffer size defaulting to: 32768.\n" );
		}
	}
	if( option_additional_digest_types != NULL )
	{
		result = verification_handle_set_additional_digest_types(
			  smrawverify_verification_handle,
			  option_additional_digest_types,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set additional digest types.\n" );

			goto on_error;
		}
	}
#if !defined( HAVE_GLOB_H )
	if( smrawtools_glob_initialize(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize glob.\n" );

		goto on_error;
	}
	if( smrawtools_glob_resolve(
	     glob,
	     &( argv[ optind ] ),
	     argc - optind,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to resolve glob.\n" );

		goto on_error;
	}
	if( smrawtools_glob_get_results(
	     glob,
	     &number_of_filenames,
	     (system_character_t ***) &argv_filenames,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve glob results.\n" );

		goto on_error;
	}
#else
	argv_filenames      = &( argv[ optind ] );
	number_of_filenames = argc - optind;
#endif

	if( smrawtools_signal_attach(
	     smrawverify_signal_handler,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	result = verification_handle_open_input(
	          smrawverify_verification_handle,
	          argv_filenames,
	          number_of_filenames,
	          &error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open SMRAW image file(s).\n" );

		goto on_error;
	}
#if !defined( HAVE_GLOB_H )
	if( smrawtools_glob_free(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

		goto on_error;
	}
#endif
	if( log_filename != NULL )
	{
		if( log_handle_initialize(
		     &log_handle,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to create log handle.\n" );

			goto on_error;
		}
		if( log_handle_open(
		     log_handle,
		     log_filename,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to open log file: %" PRIs_SYSTEM ".\n",
			 log_filename );

			goto on_error;
		}
	}
	result = verification_handle_verify_input(
		  smrawverify_verification_handle,
		  print_status_information,
		  log_handle,
		  &error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to verify input.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( log_handle != NULL )
	{
		if( log_handle_close(
		     log_handle,
		     &error ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to close log handle.\n" );

			goto on_error;
		}
		if( log_handle_free(
		     &log_handle,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to free log handle.\n" );

			goto on_error;
		}
	}
	if( smrawtools_signal_detach(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( verification_handle_close(
	     smrawverify_verification_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close verification handle.\n" );

		goto on_error;

	}
	if( verification_handle_free(
	     &smrawverify_verification_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free verification handle.\n" );

		goto on_error;
	}
	if( smrawverify_abort != 0 )
	{
		fprintf(
		 stdout,
		 "%" PRIs_SYSTEM ": ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	if( result != 1 )
	{
		fprintf(
		 stdout,
		 "%" PRIs_SYSTEM ": FAILURE\n",
		 program );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "%" PRIs_SYSTEM ": SUCCESS\n",
	 program );

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( log_handle != NULL )
	{
		log_handle_close(
		 log_handle,
		 NULL );
		log_handle_free(
		 &log_handle,
		 NULL );
	}
	if( smrawverify_verification_handle != NULL )
	{
		verification_handle_close(
		 smrawverify_verification_handle,
		 NULL );
		verification_handle_free(
		 &smrawverify_verification_handle,
		 NULL );
	}
#if !defined( HAVE_GLOB_H )
	if( glob != NULL )
	{
		smrawtools_glob_free(
		 &glob,
		 NULL );
	}
#endif
	return( EXIT_FAILURE );
}

