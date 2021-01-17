/*
 * Mounts a storage media (split) RAW image file
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

#include <stdio.h>

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#include "mount_dokan.h"
#include "mount_fuse.h"
#include "mount_handle.h"
#include "smrawtools_getopt.h"
#include "smrawtools_glob.h"
#include "smrawtools_i18n.h"
#include "smrawtools_libcerror.h"
#include "smrawtools_libclocale.h"
#include "smrawtools_libcnotify.h"
#include "smrawtools_libsmraw.h"
#include "smrawtools_output.h"
#include "smrawtools_signal.h"
#include "smrawtools_unused.h"

mount_handle_t *smrawmount_mount_handle = NULL;
int smrawmount_abort                    = 0;

/* Prints usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use smrawmount to mount a storage media (split) RAW image file\n\n" );

	fprintf( stream, "Usage: smrawmount [ -X extended_options ] [ -hvV ] image mount_point\n\n" );

	fprintf( stream, "\timage:       a storage media (split) RAW image file\n\n" );
	fprintf( stream, "\tmount_point: the directory to serve as mount point\n\n" );

	fprintf( stream, "\t-h:          shows this help\n" );
	fprintf( stream, "\t-v:          verbose output to stderr, while smrawmount will remain running in the\n"
	                 "\t             foreground\n" );
	fprintf( stream, "\t-V:          print version\n" );
	fprintf( stream, "\t-X:          extended options to pass to sub system\n" );
}

/* Signal handler for smrawmount
 */
void smrawmount_signal_handler(
      smrawtools_signal_t signal SMRAWTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "smrawmount_signal_handler";

	SMRAWTOOLS_UNREFERENCED_PARAMETER( signal )

	smrawmount_abort = 1;

	if( smrawmount_mount_handle != NULL )
	{
		if( mount_handle_signal_abort(
		     smrawmount_mount_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal mount handle to abort.\n",
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
	system_character_t * const *sources         = NULL;
	libsmraw_error_t *error                     = NULL;
	system_character_t *mount_point             = NULL;
	system_character_t *option_extended_options = NULL;
	const system_character_t *path_prefix       = NULL;
	system_character_t *program                 = _SYSTEM_STRING( "smrawmount" );
	system_integer_t option                     = 0;
	size_t path_prefix_size                     = 0;
	int number_of_sources                       = 0;
	int result                                  = 0;
	int verbose                                 = 0;

#if !defined( HAVE_GLOB_H )
	smrawtools_glob_t *glob                     = NULL;
#endif

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	struct fuse_operations smrawmount_fuse_operations;

	struct fuse_args smrawmount_fuse_arguments  = FUSE_ARGS_INIT(0, NULL);
	struct fuse_chan *smrawmount_fuse_channel   = NULL;
	struct fuse *smrawmount_fuse_handle         = NULL;

#elif defined( HAVE_LIBDOKAN )
	DOKAN_OPERATIONS smrawmount_dokan_operations;
	DOKAN_OPTIONS smrawmount_dokan_options;
#endif

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
	                   _SYSTEM_STRING( "hvVX:" ) ) ) != (system_integer_t) -1 )
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

				return( EXIT_FAILURE );

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				smrawtools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'X':
				option_extended_options = optarg;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source image(s).\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	if( ( optind + 1 ) == argc )
	{
		fprintf(
		 stderr,
		 "Missing mount point.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	mount_point = argv[ argc - 1 ];

	libcnotify_verbose_set(
	 verbose );
	libsmraw_notify_set_stream(
	 stderr,
	 NULL );
	libsmraw_notify_set_verbose(
	 verbose );

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
	     argc - optind - 1,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to resolve glob.\n" );

		goto on_error;
	}
	if( smrawtools_glob_get_results(
	     glob,
	     &number_of_sources,
	     (system_character_t ***) &sources,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve glob results.\n" );

		goto on_error;
	}
#else
	sources           = &( argv[ optind ] );
	number_of_sources = argc - optind - 1;
#endif

	if( mount_handle_initialize(
	     &smrawmount_mount_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize mount handle.\n" );

		goto on_error;
	}
#if defined( WINAPI )
	path_prefix = _SYSTEM_STRING( "\\SMRAW" );
#else
	path_prefix = _SYSTEM_STRING( "/smraw" );
#endif
	path_prefix_size = 1 + system_string_length(
	                        path_prefix );

	if( mount_handle_set_path_prefix(
	     smrawmount_mount_handle,
	     path_prefix,
	     path_prefix_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set path prefix.\n" );

		goto on_error;
	}
	if( mount_handle_open(
	     smrawmount_mount_handle,
	     sources,
	     number_of_sources,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open source image(s)\n" );

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
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( option_extended_options != NULL )
	{
		/* This argument is required but ignored
		 */
		if( fuse_opt_add_arg(
		     &smrawmount_fuse_arguments,
		     "" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &smrawmount_fuse_arguments,
		     "-o" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &smrawmount_fuse_arguments,
		     option_extended_options ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
	}
	if( memory_set(
	     &smrawmount_fuse_operations,
	     0,
	     sizeof( struct fuse_operations ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear fuse operations.\n" );

		goto on_error;
	}
	smrawmount_fuse_operations.open       = &mount_fuse_open;
	smrawmount_fuse_operations.read       = &mount_fuse_read;
	smrawmount_fuse_operations.release    = &mount_fuse_release;
	smrawmount_fuse_operations.opendir    = &mount_fuse_opendir;
	smrawmount_fuse_operations.readdir    = &mount_fuse_readdir;
	smrawmount_fuse_operations.releasedir = &mount_fuse_releasedir;
	smrawmount_fuse_operations.getattr    = &mount_fuse_getattr;
	smrawmount_fuse_operations.destroy    = &mount_fuse_destroy;

	smrawmount_fuse_channel = fuse_mount(
	                           mount_point,
	                           &smrawmount_fuse_arguments );

	if( smrawmount_fuse_channel == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse channel.\n" );

		goto on_error;
	}
	smrawmount_fuse_handle = fuse_new(
	                          smrawmount_fuse_channel,
	                          &smrawmount_fuse_arguments,
	                          &smrawmount_fuse_operations,
	                          sizeof( struct fuse_operations ),
	                          smrawmount_mount_handle );

	if( smrawmount_fuse_handle == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse handle.\n" );

		goto on_error;
	}
	if( verbose == 0 )
	{
		if( fuse_daemonize(
		     0 ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to daemonize fuse.\n" );

			goto on_error;
		}
	}
	result = fuse_loop(
	          smrawmount_fuse_handle );

	if( result != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to run fuse loop.\n" );

		goto on_error;
	}
	fuse_destroy(
	 smrawmount_fuse_handle );

	fuse_opt_free_args(
	 &smrawmount_fuse_arguments );

	return( EXIT_SUCCESS );

#elif defined( HAVE_LIBDOKAN )
	if( memory_set(
	     &smrawmount_dokan_operations,
	     0,
	     sizeof( DOKAN_OPERATIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan operations.\n" );

		goto on_error;
	}
	if( memory_set(
	     &smrawmount_dokan_options,
	     0,
	     sizeof( DOKAN_OPTIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan options.\n" );

		goto on_error;
	}
	smrawmount_dokan_options.Version     = DOKAN_VERSION;
	smrawmount_dokan_options.ThreadCount = 0;
	smrawmount_dokan_options.MountPoint  = mount_point;

	if( verbose != 0 )
	{
		smrawmount_dokan_options.Options |= DOKAN_OPTION_STDERR;
#if defined( HAVE_DEBUG_OUTPUT )
		smrawmount_dokan_options.Options |= DOKAN_OPTION_DEBUG;
#endif
	}
/* This will only affect the drive properties
	smrawmount_dokan_options.Options |= DOKAN_OPTION_REMOVABLE;
*/

#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
	smrawmount_dokan_options.Options |= DOKAN_OPTION_KEEP_ALIVE;

	smrawmount_dokan_operations.CreateFile           = &mount_dokan_CreateFile;
	smrawmount_dokan_operations.OpenDirectory        = &mount_dokan_OpenDirectory;
	smrawmount_dokan_operations.CreateDirectory      = NULL;
	smrawmount_dokan_operations.Cleanup              = NULL;
	smrawmount_dokan_operations.CloseFile            = &mount_dokan_CloseFile;
	smrawmount_dokan_operations.ReadFile             = &mount_dokan_ReadFile;
	smrawmount_dokan_operations.WriteFile            = NULL;
	smrawmount_dokan_operations.FlushFileBuffers     = NULL;
	smrawmount_dokan_operations.GetFileInformation   = &mount_dokan_GetFileInformation;
	smrawmount_dokan_operations.FindFiles            = &mount_dokan_FindFiles;
	smrawmount_dokan_operations.FindFilesWithPattern = NULL;
	smrawmount_dokan_operations.SetFileAttributes    = NULL;
	smrawmount_dokan_operations.SetFileTime          = NULL;
	smrawmount_dokan_operations.DeleteFile           = NULL;
	smrawmount_dokan_operations.DeleteDirectory      = NULL;
	smrawmount_dokan_operations.MoveFile             = NULL;
	smrawmount_dokan_operations.SetEndOfFile         = NULL;
	smrawmount_dokan_operations.SetAllocationSize    = NULL;
	smrawmount_dokan_operations.LockFile             = NULL;
	smrawmount_dokan_operations.UnlockFile           = NULL;
	smrawmount_dokan_operations.GetFileSecurity      = NULL;
	smrawmount_dokan_operations.SetFileSecurity      = NULL;
	smrawmount_dokan_operations.GetDiskFreeSpace     = NULL;
	smrawmount_dokan_operations.GetVolumeInformation = &mount_dokan_GetVolumeInformation;
	smrawmount_dokan_operations.Unmount              = &mount_dokan_Unmount;

#else
	smrawmount_dokan_operations.ZwCreateFile         = &mount_dokan_ZwCreateFile;
	smrawmount_dokan_operations.Cleanup              = NULL;
	smrawmount_dokan_operations.CloseFile            = &mount_dokan_CloseFile;
	smrawmount_dokan_operations.ReadFile             = &mount_dokan_ReadFile;
	smrawmount_dokan_operations.WriteFile            = NULL;
	smrawmount_dokan_operations.FlushFileBuffers     = NULL;
	smrawmount_dokan_operations.GetFileInformation   = &mount_dokan_GetFileInformation;
	smrawmount_dokan_operations.FindFiles            = &mount_dokan_FindFiles;
	smrawmount_dokan_operations.FindFilesWithPattern = NULL;
	smrawmount_dokan_operations.SetFileAttributes    = NULL;
	smrawmount_dokan_operations.SetFileTime          = NULL;
	smrawmount_dokan_operations.DeleteFile           = NULL;
	smrawmount_dokan_operations.DeleteDirectory      = NULL;
	smrawmount_dokan_operations.MoveFile             = NULL;
	smrawmount_dokan_operations.SetEndOfFile         = NULL;
	smrawmount_dokan_operations.SetAllocationSize    = NULL;
	smrawmount_dokan_operations.LockFile             = NULL;
	smrawmount_dokan_operations.UnlockFile           = NULL;
	smrawmount_dokan_operations.GetFileSecurity      = NULL;
	smrawmount_dokan_operations.SetFileSecurity      = NULL;
	smrawmount_dokan_operations.GetDiskFreeSpace     = NULL;
	smrawmount_dokan_operations.GetVolumeInformation = &mount_dokan_GetVolumeInformation;
	smrawmount_dokan_operations.Unmounted            = NULL;
	smrawmount_dokan_operations.FindStreams          = NULL;
	smrawmount_dokan_operations.Mounted              = NULL;

#endif /* ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 ) */

	result = DokanMain(
	          &smrawmount_dokan_options,
	          &smrawmount_dokan_operations );

	switch( result )
	{
		case DOKAN_SUCCESS:
			break;

		case DOKAN_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: generic error\n" );
			break;

		case DOKAN_DRIVE_LETTER_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: bad drive letter\n" );
			break;

		case DOKAN_DRIVER_INSTALL_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unable to load driver\n" );
			break;

		case DOKAN_START_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: driver error\n" );
			break;

		case DOKAN_MOUNT_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unable to assign drive letter\n" );
			break;

		case DOKAN_MOUNT_POINT_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: mount point error\n" );
			break;

		default:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unknown error: %d\n",
			 result );
			break;
	}
	return( EXIT_SUCCESS );

#else
	fprintf(
	 stderr,
	 "No sub system to mount SMRAW format.\n" );

	return( EXIT_FAILURE );

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE ) */

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( smrawmount_fuse_handle != NULL )
	{
		fuse_destroy(
		 smrawmount_fuse_handle );
	}
	fuse_opt_free_args(
	 &smrawmount_fuse_arguments );
#endif
	if( smrawmount_mount_handle != NULL )
	{
		mount_handle_free(
		 &smrawmount_mount_handle,
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

