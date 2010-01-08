/*
 * smioexport
 * Export media data from and to storage media file(s)
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
#include "digest_context.h"
#include "smiocommon.h"
#include "smioinput.h"
#include "smiooutput.h"
#include "export_handle.h"
#include "md5.h"
#include "platform.h"
#include "process_status.h"
#include "sha1.h"

#define SMIOEXPORT_INPUT_BUFFER_SIZE	64

export_handle_t *smioexport_export_handle = NULL;
int smioexport_abort                      = 0;

/* Prints the executable usage information to the stream
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use smioexport to export data from and to storage media file(s).\n\n" );

	fprintf( stream, "Usage: smioexport [ -b amount_of_sectors ] [ -B amount_of_bytes ]\n"
	                 "                  [ -c compression_type ] [ -d digest_type ] [ -f format ]\n"
	                 "                  [ -l log_filename ] [ -o offset ] [ -p process_buffer_size ]\n"
	                 "                  [ -S maximum_segment_size ] [ -t target ] [ -hqsuvVw ] files\n\n" );

	fprintf( stream, "\tfiles: the first or the entire set of storage media segment files\n\n" );

	fprintf( stream, "\t-b:    specify the amount of sectors to read at once (per block), options:\n"
	                 "\t       64 (default), 128, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768\n"
	                 "\t       (not used for raw format)\n" );
	fprintf( stream, "\t-B:    specify the amount of bytes to export (default is all bytes)\n" );
	fprintf( stream, "\t-c:    specify the compression type, options: none (default), empty-block,\n"
	                 "\t       fast or best (not used for raw format)\n" );
	fprintf( stream, "\t-d:    calculate additional digest (hash) types besides md5, options: sha1\n"
	                 "\t       (not used for raw format)\n" );
	fprintf( stream, "\t-f:    specify the storage media type to write to, options: aff, ewf, raw (default)\n" );
#if defined( LIBSMIO_AFF_SUPPORT )
	fprintf( stream, "aff, " );
#endif
	fprintf( stream, "ewf, " );
	fprintf( stream, "raw (default)\n" );
	fprintf( stream, "\t       a sub type can be defined using : as the separator, e.g. ewf:ewfx\n" );
	fprintf( stream, "\t       supported ewf sub types: ewf, smart, encase1, encase2, encase3, encase4,\n"
	                 "\t       encase5, encase6 (default), linen5, linen6, ewfx\n" );

	fprintf( stream, "\t-h:    shows this help\n" );
	fprintf( stream, "\t-l:    logs export errors and the digest (hash) to the log_filename\n" );
	fprintf( stream, "\t-o:    specify the offset to start the export (default is 0)\n" );
	fprintf( stream, "\t-p:    specify the process buffer size (default is the block size or %d)\n",
	 64 * 512 );
	fprintf( stream, "\t-q:    quiet shows no status information\n" );
	fprintf( stream, "\t-s:    swap byte pairs of the media data (from AB to BA)\n"
	                 "\t       (use this for big to little endian conversion and vice versa)\n" );
	fprintf( stream, "\t-S:    specify the segment file size in bytes.\n" );
	fprintf( stream, "\t-t:    specify the target file to export to, use - for stdout (default is\n"
	                 "\t       export) stdout is only supported for the raw format\n" );
	fprintf( stream, "\t-u:    unattended mode (disables user interaction)\n" );
	fprintf( stream, "\t-v:    verbose output to stderr\n" );
	fprintf( stream, "\t-V:    print version\n" );
	fprintf( stream, "\t-w:    wipe sectors on read error\n" );
}

/* Reads the media data and exports it
 * Returns a -1 on error, the amount of bytes read on success
 */
ssize64_t smioexport_read_input(
           export_handle_t *export_handle,
           size64_t media_size,
           size64_t export_size,
           off64_t read_offset,
           libsystem_character_t *calculated_md5_hash_string,
           size_t calculated_md5_hash_string_size,
           libsystem_character_t *calculated_sha1_hash_string,
           size_t calculated_sha1_hash_string_size,
           uint8_t swap_byte_pairs,
           size_t process_buffer_size,
           process_status_t *process_status,
	   liberror_error_t **error )
{
	libsmio_buffer_t *storage_media_buffer = NULL;
	static char *function                  = "smioexport_read_input";
	ssize64_t export_count                 = 0;
	size_t input_block_size                = 0;
	size_t read_size                       = 0;
	ssize_t process_count                  = 0;
	ssize_t read_count                     = 0;
	ssize_t write_count                    = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( process_buffer_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid process buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( process_status == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
		 function );

		return( -1 );
	}
	if( export_handle_get_input_block_size(
	     export_handle,
	     &input_block_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine the input block size.",
		 function );

		return( -1 );
	}
	if( input_block_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid input block size.",
		 function );

		return( -1 );
	}
	if( ( export_size == 0 )
	 || ( export_size > media_size )
	 || ( export_size > (ssize64_t) INT64_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid size value out of range.",
		 function );

		return( -1 );
	}
	if( read_offset > 0 )
	{
		if( read_offset >= (off64_t) media_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid offset.",
			 function );

			return( -1 );
		}
		if( ( export_size + read_offset ) > media_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: unable to export beyond size of media.",
			 function );

			return( -1 );
		}
		if( export_handle_seek_offset(
		     export_handle,
		     read_offset,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			"%s: unable to seek offset.",
			 function );

			return( -1 );
		}
	}
	process_buffer_size = (size_t) input_block_size;

	if( libsmio_buffer_initialize(
	     &storage_media_buffer,
	     process_buffer_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create storage media buffer.",
		 function );

		return( -1 );
	}
	while( export_count < (ssize64_t) export_size )
	{
		read_size = process_buffer_size;

		if( ( media_size - export_count ) < read_size )
		{
			read_size = (size_t) ( media_size - export_count );
		}
		read_count = export_handle_read_buffer(
		              export_handle,
		              storage_media_buffer,
		              read_size,
		              error );

		if( read_count < 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			"%s: unable to read data.",
			 function );

			libsmio_buffer_free(
			 &storage_media_buffer,
			 NULL );

			return( -1 );
		}
		if( read_count == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unexpected end of data.",
			 function );

			libsmio_buffer_free(
			 &storage_media_buffer,
			 NULL );

			return( -1 );
		}
		process_count = export_handle_prepare_read_buffer(
		                 export_handle,
		                 storage_media_buffer,
		                 error );

		if( process_count < 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			"%s: unable to prepare buffer after read.",
			 function );

			libsmio_buffer_free(
			 &storage_media_buffer,
			 NULL );

			return( -1 );
		}
		if( process_count > (ssize_t) read_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: more bytes read than requested.",
			 function );

			libsmio_buffer_free(
			 &storage_media_buffer,
			 NULL );

			return( -1 );
		}
		/* Swap byte pairs
		 */
		if( ( swap_byte_pairs == 1 )
		 && ( export_handle_swap_byte_pairs(
		       export_handle,
		       storage_media_buffer,
		       process_count,
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to swap byte pairs.",
			 function );

			libsmio_buffer_free(
			 &storage_media_buffer,
			 NULL );

			return( -1 );
		}
		/* Digest hashes are calcultated after swap
		 */
		if( export_handle_update_integrity_hash(
		     export_handle,
		     storage_media_buffer,
		     process_count,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to update integrity hash(es).",
			 function );

			libsmio_buffer_free(
			 &storage_media_buffer,
			 NULL );

			return( -1 );
		}
		export_count += process_count;

		process_count = export_handle_prepare_write_buffer(
		                 export_handle,
		                 storage_media_buffer,
		                 error );

		if( process_count < 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			"%s: unable to prepare buffer before write.",
			 function );

			libsmio_buffer_free(
			 &storage_media_buffer,
			 NULL );

			return( -1 );
		}

		write_count = export_handle_write_buffer(
		               export_handle,
		               storage_media_buffer,
		               process_count,
		               error );

		if( write_count < 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write data to file.",
			 function );

			libsmio_buffer_free(
			 &storage_media_buffer,
			 NULL );

			return( -1 );
		}
		if( process_status_update(
		     process_status,
		     (size64_t) export_count,
		     export_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update process status.",
			 function );

			libsmio_buffer_free(
			 &storage_media_buffer,
			 NULL );

			return( -1 );
		}
		if( smioexport_abort != 0 )
		{
			break;
		}
  	}
	if( libsmio_buffer_free(
	     &storage_media_buffer,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free storage media buffer.",
		 function );

		return( -1 );
	}
	write_count = export_handle_finalize(
	               export_handle,
	               calculated_md5_hash_string,
	               calculated_md5_hash_string_size,
	               calculated_sha1_hash_string,
	               calculated_sha1_hash_string_size,
	               error );

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to finalize.",
		 function );

		return( -1 );
	}
	return( export_count );
}

/* Signal handler for smioexport
 */
void smioexport_signal_handler(
      libsystem_signal_t signal )
{
	liberror_error_t *error = NULL;
	static char *function   = "smioexport_signal_handler";

	smioexport_abort = 1;

	if( ( smioexport_export_handle != NULL )
	 && ( export_handle_signal_abort(
	       smioexport_export_handle,
	       &error ) != 1 ) )
	{
		libsystem_notify_printf(
		 "%s: unable to signal export handle to abort.\n",
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
	libsystem_character_t acquiry_operating_system[ 32 ];
	libsystem_character_t input_buffer[ SMIOEXPORT_INPUT_BUFFER_SIZE ];

	libsystem_character_t * const *argv_filenames      = NULL;

	export_handle_t *export_handle                     = NULL;

#if !defined( HAVE_GLOB_H )
	libsystem_glob_t *glob                             = NULL;
#endif
	liberror_error_t *error                            = NULL;

	process_status_t *process_status                   = NULL;

	libsystem_character_t *acquiry_software_version    = NULL;
	libsystem_character_t *calculated_md5_hash_string  = NULL;
	libsystem_character_t *calculated_sha1_hash_string = NULL;
	libsystem_character_t *fixed_string_variable       = NULL;
	libsystem_character_t *log_filename                = NULL;
	libsystem_character_t *option_target_filename      = NULL;
	libsystem_character_t *program                     = _LIBSYSTEM_CHARACTER_T_STRING( "smioexport" );
	libsystem_character_t *target_filename             = NULL;
	libsystem_character_t *value_separator             = NULL;

	FILE *log_file_stream                              = NULL;

	libsystem_integer_t option                         = 0;
	size64_t media_size                                = 0;
	ssize64_t export_count                             = 0;
	size_t string_length                               = 0;
	uint64_t export_offset                             = 0;
	uint64_t export_size                               = 0;
	uint64_t maximum_segment_size                      = 0;
	uint64_t maximum_segment_size_maximum              = 0;
	uint64_t process_buffer_size                       = 64 * 512;
	uint32_t sectors_per_block                         = 64;
	uint8_t calculate_md5                              = 1;
	uint8_t calculate_sha1                             = 0;
	uint8_t print_status_information                   = 1;
	uint8_t swap_byte_pairs                            = 0;
	uint8_t wipe_block_on_error                        = 0;
	uint8_t verbose                                    = 0;
	int amount_of_filenames                            = 0;
	int argument_set_compression                       = 0;
	int argument_set_format                            = 0;
	int argument_set_offset                            = 0;
	int argument_set_sectors_per_block                 = 0;
	int argument_set_maximum_segment_size              = 0;
	int argument_set_size                              = 0;
	int compression_flags                              = 0;
	int compression_level                              = LIBSMIO_COMPRESSION_LEVEL_NONE;
	int compression_type                               = LIBSMIO_COMPRESSION_TYPE_NONE;
	int error_abort                                    = 0;
	int interactive_mode                               = 1;
	int maximum_possible_segment_size                  = 0;
	int output_ewf_format                              = LIBSMIO_EWF_FORMAT_ENCASE6;
	int output_storage_media_type                      = LIBSMIO_TYPE_RAW;
	int result                                         = 1;
	int status                                         = 0;

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


#if defined( WINAPI )
	if( _setmode(
	     _fileno(
	      stdout ),
	     _O_BINARY ) == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to set stdout to binary mode.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
#endif

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBSYSTEM_CHARACTER_T_STRING( "b:B:c:d:f:hl:o:p:qsS:t:uvVw" ) ) ) != (libsystem_integer_t) -1 )
	{
		switch( option )
		{
			case (libsystem_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBSYSTEM ".\n",
				 argv[ optind ] );

				usage_fprint(
				 stderr );

				return( EXIT_FAILURE );

			case (libsystem_integer_t) 'b':
				if( smioinput_determine_sectors_per_block(
				     optarg,
				     &sectors_per_block,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					sectors_per_block = 64;

					fprintf(
					 stderr,
					 "Unsupported amount of sectors per block defaulting to: 64.\n" );
				}
				else
				{
					argument_set_sectors_per_block = 1;
				}
				break;

			case (libsystem_integer_t) 'B':
				string_length = libsystem_string_length(
				                 optarg );

				if( libsystem_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &export_size,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					export_size = 0;

					fprintf(
					 stderr,
					 "Unsupported export size defaulting to: all bytes.\n" );
				}
				argument_set_size = 1;

				break;

			case (libsystem_integer_t) 'c':
				if( smioinput_determine_compression_level(
				     optarg,
				     &compression_level,
				     &compression_flags,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					compression_level = LIBSMIO_COMPRESSION_LEVEL_NONE;
					compression_flags = 0;

					fprintf(
					 stderr,
					 "Unsupported compression type defaulting to: none.\n" );
				}
				else
				{
					argument_set_compression = 1;
				}
				break;

			case (libsystem_integer_t) 'd':
				if( libsystem_string_compare(
				     optarg,
				     _LIBSYSTEM_CHARACTER_T_STRING( "sha1" ),
				     4 ) == 0 )
				{
					calculate_sha1 = 1;
				}
				else
				{
					fprintf(
					 stderr,
					 "Unsupported digest type.\n" );
				}
				break;

			case (libsystem_integer_t) 'f':
				if( smioinput_determine_storage_media_type(
				     optarg,
				     &output_storage_media_type,
				     &error ) == 1 )
				{
					argument_set_format = 1;
				}
				else
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					output_storage_media_type = LIBSMIO_TYPE_RAW;

					fprintf(
					 stderr,
					 "Unsupported storage media type defaulting to: raw.\n" );
				}
				if( output_storage_media_type == LIBSMIO_TYPE_EWF )
				{
					string_length = libsystem_string_length(
					                 optarg );

					value_separator = libsystem_string_search(
					                   optarg,
					                   ':',
					                   string_length );

					if( ( value_separator != NULL )
					 && ( smioinput_determine_ewf_format(
					       value_separator,
					       &output_ewf_format,
					       &error ) != 1 ) )
					{
						libsystem_notify_print_error_backtrace(
						 error );
						liberror_error_free(
						 &error );

						output_ewf_format = LIBSMIO_EWF_FORMAT_ENCASE6;

						fprintf(
						 stderr,
						 "Unsupported EWF format defaulting to: encase6.\n" );
					}

				}
				break;

			case (libsystem_integer_t) 'h':
				usage_fprint(
				 stderr );

				return( EXIT_SUCCESS );

			case (libsystem_integer_t) 'l':
				log_filename = optarg;

				break;

			case (libsystem_integer_t) 'o':
				string_length = libsystem_string_length(
				                 optarg );

				if( libsystem_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &export_offset,
				     NULL ) != 1 )
				{
					export_offset = 0;

					fprintf(
					 stderr,
					 "Unsupported export offset defaulting to: %" PRIu64 ".\n",
					 export_offset );
				}
				argument_set_offset = 1;

				break;

			case (libsystem_integer_t) 'p':
				string_length = libsystem_string_length(
				                 optarg );

				result = byte_size_string_convert(
				          optarg,
				          string_length,
				          &process_buffer_size,
				          &error );

				if( result != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
				}
				if( ( result != 1 )
				 || ( process_buffer_size > (uint64_t) SSIZE_MAX ) )
				{
					process_buffer_size = 0;

					fprintf(
					 stderr,
					 "Unsupported process buffer size defaulting to: block size.\n" );
				}
				break;

			case (libsystem_integer_t) 'q':
				print_status_information = 0;

				break;

			case (libsystem_integer_t) 's':
				swap_byte_pairs = 1;

				break;

			case (libsystem_integer_t) 'S':
				string_length = libsystem_string_length(
				                 optarg );

				result = byte_size_string_convert(
				          optarg,
				          string_length,
				          &maximum_segment_size,
				          &error );

				if( result != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
				}
				argument_set_maximum_segment_size = 1;

				/* TODO check for output_storage_media_type = LIBSMIO_TYPE_EWF */
				if( ( result != 1 )
				 || ( maximum_segment_size < SMIOCOMMON_MINIMUM_SEGMENT_SIZE )
				 || ( ( output_ewf_format == LIBSMIO_EWF_FORMAT_ENCASE6 )
				  && ( maximum_segment_size >= (int64_t) SMIOCOMMON_MAXIMUM_SEGMENT_SIZE_64BIT ) )
				 || ( ( output_ewf_format != LIBSMIO_EWF_FORMAT_ENCASE6 )
				  && ( maximum_segment_size >= (int64_t) SMIOCOMMON_MAXIMUM_SEGMENT_SIZE_32BIT ) ) )
				{
					maximum_segment_size = SMIOCOMMON_DEFAULT_SEGMENT_SIZE;

					fprintf(
					 stderr,
					 "Unsupported segment file size defaulting to: %" PRIu64 ".\n",
					 maximum_segment_size );
				}
				break;

			case (libsystem_integer_t) 't':
				option_target_filename = optarg;

				break;

			case (libsystem_integer_t) 'u':
				interactive_mode = 0;

				break;

			case (libsystem_integer_t) 'v':
				verbose = 1;

				break;

			case (libsystem_integer_t) 'V':
				smiooutput_copyright_fprint(
				 stderr );

				return( EXIT_SUCCESS );

			case (libsystem_integer_t) 'w':
				wipe_block_on_error = 1;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing storage media file(s).\n" );

		usage_fprint(
		 stderr );

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
	     smioexport_signal_handler,
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

	if( export_handle_initialize(
	     &export_handle,
	     calculate_md5,
	     calculate_sha1,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create export handle.\n" );

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
	result = export_handle_open_input(
	          export_handle,
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

	if( ( smioexport_abort == 0 )
	 && ( result != 1 ) )
	{
		fprintf(
		 stderr,
		 "Unable to open storage media file(s)" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		export_handle_free(
		 &export_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( ( smioexport_abort == 0 )
	 && ( export_handle_get_input_media_size(
	       export_handle,
	       &media_size,
	       &error ) != 1 ) )
	{
		fprintf(
		 stderr,
		 "Unable to determine input media size.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		export_handle_close(
		 export_handle,
		 NULL );
		export_handle_free(
		 &export_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( ( export_size == 0 )
	 || ( export_size > ( media_size - export_offset ) ) )
	{
		export_size = media_size - export_offset;
	}
	/* Create the input buffers
	 */
	if( option_target_filename != NULL )
	{
		string_length = libsystem_string_length(
				 option_target_filename );

		if( string_length > 0 )
		{
			target_filename = (libsystem_character_t *) memory_allocate(
			                                             sizeof( libsystem_character_t ) * ( string_length + 1 ) );

			if( target_filename == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create target filename.\n" );

				error_abort = 1;
			}
			else if( libsystem_string_copy(
				  target_filename,
				  option_target_filename,
				  string_length ) == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to set target filename.\n" );

				error_abort = 1;
			}
			target_filename[ string_length ] = 0;
		}
	}
	else
	{
		target_filename = (libsystem_character_t *) memory_allocate(
		                                             sizeof( libsystem_character_t ) * 1024 );

		if( target_filename == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to create target filename string.\n" );

			error_abort = 1;
		}
		/* Make sure to set the target filename if in unattended mode
		 */
		else if( interactive_mode == 0 )
		{
			if( libsystem_string_copy(
			     target_filename,
			     _LIBSYSTEM_CHARACTER_T_STRING( "export" ),
			     7 ) == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to set target filename string.\n" );

				error_abort = 1;
			}
			target_filename[ 7 ] = 0;
		}
	}
	if( error_abort != 0 )
	{
		memory_free(
		 target_filename );

		export_handle_close(
		 export_handle,
		 NULL );
		export_handle_free(
		 &export_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	/* Request the necessary case data
	 */
	if( ( smioexport_abort == 0 )
	 && ( interactive_mode == 1 ) )
	{
		if( libsystem_signal_detach(
		     &error) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to detach signal handler.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
		fprintf(
		 stderr,
		 "Information for export required, please provide the necessary input\n" );

		/* File format
		 */
		if( argument_set_format == 0 )
		{
			if( smioinput_get_fixed_string_variable(
			     stderr,
			     input_buffer,
			     SMIOEXPORT_INPUT_BUFFER_SIZE,
			     _LIBSYSTEM_CHARACTER_T_STRING( "Export to storage media type" ),
			     smioinput_storage_media_types,
			     SMIOINPUT_STORAGE_MEDIA_TYPES_AMOUNT,
			     SMIOINPUT_STORAGE_MEDIA_TYPES_DEFAULT,
			     &fixed_string_variable,
			     &error ) == -1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				output_storage_media_type = LIBSMIO_TYPE_RAW;

				fprintf(
				 stderr,
				 "Unable to determine storage media type defaulting to: raw.\n" );
			}
			else if( smioinput_determine_storage_media_type(
			          fixed_string_variable,
			          &output_storage_media_type,
			          &error ) != 1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				output_storage_media_type = LIBSMIO_TYPE_RAW;

				fprintf(
				 stderr,
				 "Unsupported storage media type defaulting to: raw.\n" );
			}
			if( output_storage_media_type == LIBSMIO_TYPE_EWF )
			{
				if( smioinput_get_fixed_string_variable(
				     stderr,
				     input_buffer,
				     SMIOEXPORT_INPUT_BUFFER_SIZE,
				     _LIBSYSTEM_CHARACTER_T_STRING( "Export to EWF format" ),
				     smioinput_ewf_format_types,
				     SMIOINPUT_EWF_FORMAT_TYPES_AMOUNT,
				     SMIOINPUT_EWF_FORMAT_TYPES_DEFAULT,
				     &fixed_string_variable,
				     &error ) == -1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					output_ewf_format = LIBSMIO_EWF_FORMAT_ENCASE6;

					fprintf(
					 stderr,
					 "Unable to determine EWF format defaulting to: encase6.\n" );
				}
				else if( smioinput_determine_ewf_format(
				          fixed_string_variable,
				          &output_ewf_format,
				          &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					output_ewf_format = LIBSMIO_EWF_FORMAT_ENCASE6;

					fprintf(
					 stderr,
					 "Unsupported EWF format defaulting to: encase6.\n" );
				}
			}
		}
		/* Target filename
		 */
		if( option_target_filename == NULL )
		{
			while( smioinput_get_string_variable(
				stderr,
				_LIBSYSTEM_CHARACTER_T_STRING( "Target path and filename without extension" ),
				target_filename,
				1024,
			        NULL ) != 1 )
			{
				fprintf(
				 stderr,
				 "Filename is required, please try again or terminate using Ctrl^C.\n" );
			}
		}
		/* Compression
		 */
		if( argument_set_compression == 0 )
		{
			if( output_storage_media_type != LIBSMIO_TYPE_RAW )
			{
				if( smioinput_get_fixed_string_variable(
				     stderr,
				     input_buffer,
				     SMIOEXPORT_INPUT_BUFFER_SIZE,
				     _LIBSYSTEM_CHARACTER_T_STRING( "Use compression" ),
				     smioinput_compression_levels,
				     SMIOINPUT_COMPRESSION_LEVELS_AMOUNT,
				     SMIOINPUT_COMPRESSION_LEVELS_DEFAULT,
				     &fixed_string_variable,
				     &error ) == -1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					compression_level = LIBSMIO_COMPRESSION_LEVEL_NONE;
					compression_flags = 0;

					fprintf(
					 stderr,
					 "Unable to determine compression type defaulting to: none.\n" );
				}
				else if( smioinput_determine_compression_level(
				          fixed_string_variable,
				          &compression_level,
				          &compression_flags,
				          &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					compression_level = LIBSMIO_COMPRESSION_LEVEL_NONE;
					compression_flags = 0;

					fprintf(
					 stderr,
					 "Unsupported compression type defaulting to: none.\n" );
				}
			}
		}
		/* Segment file size
		 */
		if( argument_set_maximum_segment_size == 0 )
		{
			/* TODO test if segment size should be restricted ? */
			if( output_storage_media_type == LIBSMIO_TYPE_EWF )
			{
				if( output_ewf_format == LIBSMIO_EWF_FORMAT_ENCASE6 )
				{
					maximum_segment_size_maximum = SMIOCOMMON_MAXIMUM_SEGMENT_SIZE_64BIT;
				}
				else
				{
					maximum_segment_size_maximum = SMIOCOMMON_MAXIMUM_SEGMENT_SIZE_32BIT;
				}
			}
			if( smioinput_get_byte_size_variable(
			     stderr,
			     input_buffer,
			     SMIOEXPORT_INPUT_BUFFER_SIZE,
			     _LIBSYSTEM_CHARACTER_T_STRING( "Evidence segment file size in bytes" ),
			     SMIOCOMMON_MINIMUM_SEGMENT_SIZE,
			     maximum_segment_size_maximum,
			     SMIOCOMMON_DEFAULT_SEGMENT_SIZE,
			     &maximum_segment_size,
			     &error ) == -1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				maximum_segment_size = SMIOCOMMON_DEFAULT_SEGMENT_SIZE;

				fprintf(
				 stderr,
				 "Unable to determine segment file size defaulting to: %" PRIu64 ".\n",
				 maximum_segment_size );
			}
			/* Make sure the maximum segment size is smaller than or equal to its maximum
			 */
			if( maximum_segment_size > maximum_segment_size_maximum )
			{
				maximum_segment_size = maximum_segment_size_maximum;
			}
		}
		/* Block size (sectors per block)
		 */
		if( argument_set_sectors_per_block == 0 )
		{
			if( output_storage_media_type != LIBSMIO_TYPE_RAW )
			{
				if( smioinput_get_fixed_string_variable(
				     stderr,
				     input_buffer,
				     SMIOEXPORT_INPUT_BUFFER_SIZE,
				     _LIBSYSTEM_CHARACTER_T_STRING( "The amount of sectors to read at once" ),
				     smioinput_sector_per_block_sizes,
				     SMIOINPUT_SECTOR_PER_BLOCK_SIZES_AMOUNT,
				     SMIOINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT,
				     &fixed_string_variable,
				     &error ) == -1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					sectors_per_block = 64;

					fprintf(
					 stderr,
					 "Unable to determine sectors per block on error defaulting to: 64.\n" );
				}
				else if( smioinput_determine_sectors_per_block(
				          fixed_string_variable,
				          &sectors_per_block,
				          &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					sectors_per_block = 64;

					fprintf(
					 stderr,
					 "Unsupported sectors per block on error defaulting to: 64.\n" );
				}
			}
		}
                /* Offset of data to export
		 */
		if( ( argument_set_offset == 0 )
		 && ( smioinput_get_size_variable(
		       stderr,
		       input_buffer,
		       SMIOEXPORT_INPUT_BUFFER_SIZE,
		       _LIBSYSTEM_CHARACTER_T_STRING( "Start export at offset" ),
		       0,
		       media_size,
		       export_offset,
		       &export_offset,
		       &error ) == -1 ) )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			export_offset = 0;

			fprintf(
			 stderr,
			 "Unable to determine export offset defaulting to: %" PRIu64 ".\n",
			 export_offset );
		}
                /* Size of data to export
		 */
		if( ( argument_set_size == 0 )
		 && ( smioinput_get_size_variable(
		       stderr,
		       input_buffer,
		       SMIOEXPORT_INPUT_BUFFER_SIZE,
		       _LIBSYSTEM_CHARACTER_T_STRING( "Amount of bytes to export" ),
		       0,
		       ( media_size - export_offset ),
		       export_size,
		       &export_size,
		       &error ) == -1 ) )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			export_size = media_size - export_offset;

			fprintf(
			 stderr,
			 "Unable to determine export size defaulting to: %" PRIu64 ".\n",
			 export_size );
		}
		if( libsystem_signal_attach(
		     smioexport_signal_handler,
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
	}
	if( calculate_md5 == 1 )
	{
		calculated_md5_hash_string = (libsystem_character_t *) memory_allocate(
		                                                        sizeof( libsystem_character_t )* DIGEST_HASH_STRING_SIZE_MD5 );

		if( calculated_md5_hash_string == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to create calculated MD5 hash string.\n" );

			memory_free(
			 target_filename );

			export_handle_close(
			 export_handle,
			 NULL );
			export_handle_free(
			 &export_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	if( calculate_sha1 == 1 )
	{
		calculated_sha1_hash_string = (libsystem_character_t *) memory_allocate(
		                                                         sizeof( libsystem_character_t )* DIGEST_HASH_STRING_SIZE_SHA1 );

		if( calculated_sha1_hash_string == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to create calculated SHA1 hash string.\n" );

			if( calculate_md5 == 1 )
			{
				memory_free(
				 calculated_md5_hash_string );
			}
			memory_free(
			 target_filename );

			export_handle_close(
			 export_handle,
			 NULL );
			export_handle_free(
			 &export_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	if( smioexport_abort == 0 )
	{
		fprintf(
		 stderr,
		 "\n" );

		if( process_status_initialize(
		     &process_status,
		     _LIBSYSTEM_CHARACTER_T_STRING( "Export" ),
		     _LIBSYSTEM_CHARACTER_T_STRING( "exported" ),
		     _LIBSYSTEM_CHARACTER_T_STRING( "Written" ),
		     stderr,
		     print_status_information,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to initialize process status.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			if( calculate_sha1 == 1 )
			{
				memory_free(
				 calculated_sha1_hash_string );
			}
			if( calculate_md5 == 1 )
			{
				memory_free(
				 calculated_md5_hash_string );
			}
			memory_free(
			 target_filename );

			export_handle_close(
			 export_handle,
			 NULL );
			export_handle_free(
			 &export_handle,
			 NULL );
			
			return( EXIT_FAILURE );
		}
		if( process_status_start(
		     process_status,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to start process status.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			process_status_free(
			 &process_status,
			 NULL );

			if( calculate_sha1 == 1 )
			{
				memory_free(
				 calculated_sha1_hash_string );
			}
			if( calculate_md5 == 1 )
			{
				memory_free(
				 calculated_md5_hash_string );
			}
			memory_free(
			 target_filename );

			export_handle_close(
			 export_handle,
			 NULL );
			export_handle_free(
			 &export_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		if( export_handle_open_output(
		     export_handle,
		     target_filename,
		     output_storage_media_type,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to open output.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			process_status_free(
			 &process_status,
			 NULL );

			if( calculate_sha1 == 1 )
			{
				memory_free(
				 calculated_sha1_hash_string );
			}
			if( calculate_md5 == 1 )
			{
				memory_free(
				 calculated_md5_hash_string );
			}
			memory_free(
			 target_filename );

			export_handle_close(
			 export_handle,
			 NULL );
			export_handle_free(
			 &export_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		memory_free(
		 target_filename );

		if( platform_get_operating_system(
		     acquiry_operating_system,
		     32,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to determine operating system.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			acquiry_operating_system[ 0 ] = 0;
		}
		acquiry_software_version = _LIBSYSTEM_CHARACTER_T_STRING( LIBSMIO_VERSION_STRING );

		/* TODO set EWF format */

		if( export_handle_set_output_values(
		     export_handle,
		     acquiry_operating_system,
		     program,
		     acquiry_software_version,
		     export_size,
		     compression_type,
		     compression_level,
		     compression_flags,
		     maximum_segment_size,
		     maximum_possible_segment_size,
		     sectors_per_block,
		     wipe_block_on_error,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set output values.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			process_status_free(
			 &process_status,
			 NULL );

			if( calculate_sha1 == 1 )
			{
				memory_free(
				 calculated_sha1_hash_string );
			}
			if( calculate_md5 == 1 )
			{
				memory_free(
				 calculated_md5_hash_string );
			}
			export_handle_close(
			 export_handle,
			 NULL );
			export_handle_free(
			 &export_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		/* Start exporting data
		 */
		export_count = smioexport_read_input(
				export_handle,
				media_size,
				export_size,
				export_offset,
				calculated_md5_hash_string,
				DIGEST_HASH_STRING_SIZE_MD5,
				calculated_sha1_hash_string,
				DIGEST_HASH_STRING_SIZE_SHA1,
				swap_byte_pairs,
				(size_t) process_buffer_size,
				process_status,
				&error );

		if( export_count <= -1 )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			status = PROCESS_STATUS_FAILED;
		}
		else
		{
			status = PROCESS_STATUS_COMPLETED;
		}
	}
	if( smioexport_abort != 0 )
	{
		status = PROCESS_STATUS_ABORTED;
	}
	if( process_status_stop(
	     process_status,
	     (size64_t) export_count,
	     status,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to stop process status.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		process_status_free(
		 &process_status,
		 NULL );

		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
		export_handle_close(
		 export_handle,
		 NULL );
		export_handle_free(
		 &export_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( process_status_free(
	     &process_status,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free process status.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
		export_handle_close(
		 export_handle,
		 NULL );
		export_handle_free(
		 &export_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( status == PROCESS_STATUS_COMPLETED )
	{
		if( log_filename != NULL )
		{
			log_file_stream = libsystem_file_stream_open(
					   log_filename,
					   _LIBSYSTEM_CHARACTER_T_STRING( "a" ) );

			if( log_file_stream == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to open log file: %s.\n",
				 log_filename );
			}
		}
		if( export_handle_export_errors_fprint(
		     export_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print export errors.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
		if( ( log_file_stream != NULL )
		 && ( export_handle_export_errors_fprint(
		       export_handle,
		       log_file_stream,
		       &error ) != 1 ) )
		{
			fprintf(
			 stderr,
			 "Unable to write export errors in log file.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	if( export_handle_close(
	     export_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close storage media file(s).\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		if( log_file_stream != NULL )
		{
			libsystem_file_stream_close(
			 log_file_stream );
		}
		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
		export_handle_free(
		 &export_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( export_handle_free(
	     &export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free export handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		if( log_file_stream != NULL )
		{
			libsystem_file_stream_close(
			 log_file_stream );
		}
		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
		return( EXIT_FAILURE );
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
	if( status != PROCESS_STATUS_COMPLETED )
	{
		if( log_file_stream != NULL )
		{
			libsystem_file_stream_close(
			 log_file_stream );
		}
		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
		return( EXIT_FAILURE );
	}
	if( calculate_md5 == 1 )
	{
		fprintf(
		 stderr,
		 "MD5 hash calculated over data:\t%" PRIs_LIBSYSTEM "\n",
		 calculated_md5_hash_string );

		if( log_file_stream != NULL )
		{
			fprintf(
			 log_file_stream,
			 "MD5 hash calculated over data:\t%" PRIs_LIBSYSTEM "\n",
			 calculated_md5_hash_string );
		}
		memory_free(
		 calculated_md5_hash_string );
	}
	if( calculate_sha1 == 1 )
	{
		fprintf(
		 stderr,
		 "SHA1 hash calculated over data:\t%" PRIs_LIBSYSTEM "\n",
		 calculated_sha1_hash_string );

		if( log_file_stream != NULL )
		{
			fprintf(
			 log_file_stream,
			 "SHA1 hash calculated over data:\t%" PRIs_LIBSYSTEM "\n",
			 calculated_sha1_hash_string );
		}
		memory_free(
		 calculated_sha1_hash_string );
	}
	if( log_file_stream != NULL )
	{
		if( libsystem_file_stream_close(
		     log_file_stream ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to close log file: %s.\n",
			 log_filename );
		}
	}
	return( EXIT_SUCCESS );
}

