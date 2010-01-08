/*
 * smioimage
 * Reads data from a file or device and writes it in storage media file(s)
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

#include <errno.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#include <stdio.h>

/* If libtool DLL support is enabled set LIBSMIO_DLL_IMPORT
 * before including libsmio.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBSMIO_DLL_IMPORT
#endif

#include <libsmio.h>

#include <libsystem.h>

#include "byte_size_string.h"
#include "smiocommon.h"
#include "smioinput.h"
#include "smiooutput.h"
#include "imaging_handle.h"
#include "platform.h"
#include "process_status.h"

#define SMIOIMAGE_INPUT_BUFFER_SIZE	64

imaging_handle_t *smioimage_imaging_handle = NULL;
int smioimage_abort                        = 0;

/* Prints the executable usage information to the stream
 */
void usage_fprint(
      FILE *stream )
{
	libsystem_character_t default_maximum_segment_size_string[ 16 ];
	libsystem_character_t minimum_maximum_segment_size_string[ 16 ];
	libsystem_character_t maximum_32bit_maximum_segment_size_string[ 16 ];
	libsystem_character_t maximum_64bit_maximum_segment_size_string[ 16 ];

	int result = 0;

	if( stream == NULL )
	{
		return;
	}
	result = byte_size_string_create(
	          default_maximum_segment_size_string,
	          16,
	          SMIOCOMMON_DEFAULT_SEGMENT_SIZE,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          NULL );

	if( result == 1 )
	{
		result = byte_size_string_create(
			  minimum_maximum_segment_size_string,
			  16,
			  SMIOCOMMON_MINIMUM_SEGMENT_SIZE,
			  BYTE_SIZE_STRING_UNIT_MEBIBYTE,
		          NULL );
	}
	if( result == 1 )
	{
		result = byte_size_string_create(
			  maximum_32bit_maximum_segment_size_string,
			  16,
			  SMIOCOMMON_MAXIMUM_SEGMENT_SIZE_32BIT,
			  BYTE_SIZE_STRING_UNIT_MEBIBYTE,
		          NULL );
	}
	if( result == 1 )
	{
		result = byte_size_string_create(
			  maximum_64bit_maximum_segment_size_string,
			  16,
			  SMIOCOMMON_MAXIMUM_SEGMENT_SIZE_64BIT,
			  BYTE_SIZE_STRING_UNIT_MEBIBYTE,
		          NULL );
	}
	fprintf( stream, "Use smioimage to image a device and store it in storage media file(s).\n\n" );

	fprintf( stream, "Usage: smioimage [ -b amount_of_sectors ] [ -B amount_of_bytes ]\n"
	                 "                 [ -c compression_type ] [ -C case_number ] [ -d digest_type ]\n"
	                 "                 [ -D description ] [ -e examiner_name ] [ -E evidence_number ]\n"
	                 "                 [ -f format ] [ -g amount_of_sectors ] [ -l log_filename ]\n"
	                 "                 [ -m media_type ] [ -M volume_type ] [ -N notes ] [ -o offset ]\n"
	                 "                 [ -p process_buffer_size ] [ -P bytes_per_sector ]\n"
	                 "                 [ -r read_error_retries ] [ -S maximum_segment_size ]\n"
	                 "                 [ -t target ] [ -hqsuvVw ] source\n\n" );

	fprintf( stream, "\tsource: the source device\n\n" );

	fprintf( stream, "\t-b:     specify the amount of sectors to read at once (per block), options:\n"
	                 "\t        64 (default), 128, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768\n" );
	fprintf( stream, "\t-B:     specify the amount of bytes to acquire (default is all bytes)\n" );
	fprintf( stream, "\t-c:     specify the compression type, options: none (default), empty-block, fast\n"
	                 "\t        or best\n" );
	fprintf( stream, "\t-C:     specify the case number (default is case_number).\n" );
	fprintf( stream, "\t-d:     calculate additional digest (hash) types besides md5, options: sha1\n" );
	fprintf( stream, "\t-D:     specify the description (default is description).\n" );
	fprintf( stream, "\t-e:     specify the examiner name (default is examiner_name).\n" );
	fprintf( stream, "\t-E:     specify the evidence number (default is evidence_number).\n" );
	fprintf( stream, "\t-g      specify the amount of sectors to be used as error granularity, options:\n"
	                 "\t        64 (default), 128, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768\n" );
	fprintf( stream, "\t-f:     specify the EWF file format to write to, options: ewf, smart, ftk,\n"
	                 "\t        encase2, encase3, encase4, encase5, encase6 (default), linen5, linen6,\n"
	                 "\t        ewfx\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-l:     logs imaging errors and the digest (hash) to the log_filename\n" );
	fprintf( stream, "\t-m:     specify the media type, options: fixed (default), removable, optical, memory\n" );
	fprintf( stream, "\t-M:     specify the volume type, options: logical, physical (default)\n" );
	fprintf( stream, "\t-N:     specify the notes (default is notes).\n" );
	fprintf( stream, "\t-o:     specify the offset to start to acquire (default is 0)\n" );
	fprintf( stream, "\t-p:     specify the process buffer size (default is the block size or %d)\n",
	 64 * 512 );
	fprintf( stream, "\t-P:     specify the amount of bytes per sector (default is 512)\n"
	                 "\t        (use this to override the automatic bytes per sector detection)\n" );
	fprintf( stream, "\t-q:     quiet shows no status information\n" );
	fprintf( stream, "\t-r:     specify the amount of retries when a read error occurs (default is 2)\n" );
	fprintf( stream, "\t-s:     swap byte pairs of the media data (from AB to BA)\n"
	                 "\t        (use this for big to little endian conversion and vice versa)\n" );

	if( result == 1 )
	{
		fprintf( stream, "\t-S:     specify the segment file size in bytes (default is %" PRIs_LIBSYSTEM ")\n"
		                 "\t        (minimum is %" PRIs_LIBSYSTEM ", maximum is %" PRIs_LIBSYSTEM " for encase6 format\n"
		                 "\t        and %" PRIs_LIBSYSTEM " for other formats)\n",
		 default_maximum_segment_size_string,
		 minimum_maximum_segment_size_string,
		 maximum_64bit_maximum_segment_size_string,
		 maximum_32bit_maximum_segment_size_string );
	}
	else
	{
		fprintf( stream, "\t-S:     specify the segment file size in bytes (default is %" PRIu32 ")\n"
		                 "\t        (minimum is %" PRIu32 ", maximum is %" PRIu64 " for encase6 format\n"
		                 "\t        and %" PRIu32 " for other formats)\n",
		 (uint32_t) SMIOCOMMON_DEFAULT_SEGMENT_SIZE,
		 (uint32_t) SMIOCOMMON_MINIMUM_SEGMENT_SIZE,
		 (uint64_t) SMIOCOMMON_MAXIMUM_SEGMENT_SIZE_64BIT,
		 (uint32_t) SMIOCOMMON_MAXIMUM_SEGMENT_SIZE_32BIT );
	}

	fprintf( stream, "\t-t:     specify the target file (without extension) to write to (default is acquire)\n" );
	fprintf( stream, "\t-u:     unattended mode (disables user interaction)\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
	fprintf( stream, "\t-w:     wipe sectors on read error (mimic EnCase like behavior)\n" );
}

/* Prints an overview of the imaging parameters and asks the for confirmation
 * Return 1 if confirmed by user, 0 otherwise or -1 on error
 */
int8_t smioimage_confirm_imaging_parameters(
        FILE *stream,
        libsystem_character_t *input_buffer,
        size_t input_buffer_size,
        libsystem_character_t *filename,
        libsystem_character_t *case_number,
        libsystem_character_t *description,
        libsystem_character_t *evidence_number,
        libsystem_character_t *examiner_name,
        libsystem_character_t *notes,
        uint8_t media_type,
        uint8_t volume_type,
        int compression_type,
        int compression_level,
        int compression_flags,
        int storage_media_type,
        int ewf_format,
        off64_t imaging_offset,
        size64_t imaging_size,
        size64_t maximum_segment_size,
        size_t bytes_per_sector,
        uint32_t sectors_per_block,
        uint32_t sector_error_granularity,
        uint8_t read_error_retry,
        uint8_t wipe_block_on_read_error,
        liberror_error_t **error )
{
	libsystem_character_t imaging_size_string[ 16 ];
	libsystem_character_t maximum_segment_size_string[ 16 ];

	libsystem_character_t *fixed_string_variable = NULL;
	static char *function                        = "smioimage_confirm_imaging_parameters";
	int8_t input_confirmed                       = -1;
	int result                                   = 0;

	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "The following imaging parameters were provided:\n" );

	fprintf(
	 stream,
	 "Image path and filename:\t%" PRIs_LIBSYSTEM ".",
	 filename );

	if( ewf_format == LIBSMIO_EWF_FORMAT_SMART )
	{
		fprintf(
		 stream,
		 "s01\n" );
	}
	else if( ( ewf_format == LIBSMIO_EWF_FORMAT_EWF )
	      || ( ewf_format == LIBSMIO_EWF_FORMAT_EWFX ) )
	{
		fprintf(
		 stream,
		 "e01\n" );
	}
	else
	{
		fprintf(
		 stream,
		 "E01\n" );
	}
	fprintf(
	 stream,
	 "Case number:\t\t\t" );

	if( case_number != NULL )
	{
		fprintf(
		 stream,
		 "%" PRIs_LIBSYSTEM "",
		 case_number );
	}
	fprintf(
	 stream,
	 "\n" );

	fprintf(
	 stream,
	 "Description:\t\t\t" );

	if( description != NULL )
	{
		fprintf(
		 stream,
		 "%" PRIs_LIBSYSTEM "",
		 description );
	}
	fprintf(
	 stream,
	 "\n" );

	fprintf(
	 stream,
	 "Evidence number:\t\t" );

	if( evidence_number != NULL )
	{
		fprintf(
		 stream,
		 "%" PRIs_LIBSYSTEM "",
		 evidence_number );
	}
	fprintf(
	 stream,
	 "\n" );

	fprintf(
	 stream,
	 "Examiner name:\t\t\t" );

	if( examiner_name != NULL )
	{
		fprintf(
		 stream,
		 "%" PRIs_LIBSYSTEM "",
		 examiner_name );
	}
	fprintf(
	 stream,
	 "\n" );

	fprintf(
	 stream,
	 "Notes:\t\t\t\t" );

	if( notes != NULL )
	{
		fprintf(
		 stream,
		 "%" PRIs_LIBSYSTEM "",
		 notes );
	}
	fprintf(
	 stream,
	 "\n" );

	fprintf(
	 stream,
	 "Media type:\t\t\t" );

	if( media_type == LIBSMIO_MEDIA_TYPE_FIXED )
	{
		fprintf(
		 stream,
		 "fixed disk\n" );
	}
	else if( media_type == LIBSMIO_MEDIA_TYPE_REMOVABLE )
	{
		fprintf(
		 stream,
		 "removable disk\n" );
	}
	else if( media_type == LIBSMIO_MEDIA_TYPE_OPTICAL )
	{
		fprintf(
		 stream,
		 "optical disk (CD/DVD/BD)\n" );
	}
	else if( media_type == LIBSMIO_MEDIA_TYPE_MEMORY )
	{
		fprintf(
		 stream,
		 "memory (RAM)\n" );
	}
	fprintf(
	 stream,
	 "Volume type:\t\t\t" );

	if( volume_type == LIBSMIO_VOLUME_TYPE_LOGICAL )
	{
		fprintf(
		 stream,
		 "logical\n" );
	}
	else if( volume_type == LIBSMIO_VOLUME_TYPE_PHYSICAL )
	{
		fprintf(
		 stream,
		 "physical\n" );
	}
	fprintf(
	 stream,
	 "Compression used:\t\t" );

	if( compression_level == LIBSMIO_COMPRESSION_LEVEL_FAST )
	{
		fprintf(
		 stream,
		 "fast\n" );
	}
	else if( compression_level == LIBSMIO_COMPRESSION_LEVEL_BEST )
	{
		fprintf(
		 stream,
		 "best\n" );
	}
	else if( compression_level == LIBSMIO_COMPRESSION_LEVEL_NONE )
	{
		if( ( compression_flags & LIBSMIO_FLAG_COMPRESS_EMPTY_BLOCK ) == LIBSMIO_FLAG_COMPRESS_EMPTY_BLOCK )
		{
			fprintf(
			 stream,
			 "empty block\n" );
		}
		else
		{
			fprintf(
			 stream,
			 "none\n" );
		}
	}
	fprintf(
	 stream,
	 "EWF file format:\t\t" );

	if( ewf_format == LIBSMIO_EWF_FORMAT_EWF )
	{
		fprintf(
		 stream,
		 "original EWF\n" );
	}
	else if( ewf_format == LIBSMIO_EWF_FORMAT_SMART )
	{
		fprintf(
		 stream,
		 "SMART\n" );
	}
	else if( ewf_format == LIBSMIO_EWF_FORMAT_FTK )
	{
		fprintf(
		 stream,
		 "FTK Imager\n" );
	}
	else if( ewf_format == LIBSMIO_EWF_FORMAT_ENCASE1 )
	{
		fprintf(
		 stream,
		 "EnCase 1\n" );
	}
	else if( ewf_format == LIBSMIO_EWF_FORMAT_ENCASE2 )
	{
		fprintf(
		 stream,
		 "EnCase 2\n" );
	}
	else if( ewf_format == LIBSMIO_EWF_FORMAT_ENCASE3 )
	{
		fprintf(
		 stream,
		 "EnCase 3\n" );
	}
	else if( ewf_format == LIBSMIO_EWF_FORMAT_ENCASE4 )
	{
		fprintf(
		 stream,
		 "EnCase 4\n" );
	}
	else if( ewf_format == LIBSMIO_EWF_FORMAT_ENCASE5 )
	{
		fprintf(
		 stream,
		 "EnCase 5\n" );
	}
	else if( ewf_format == LIBSMIO_EWF_FORMAT_ENCASE6 )
	{
		fprintf(
		 stream,
		 "EnCase 6\n" );
	}
	else if( ewf_format == LIBSMIO_EWF_FORMAT_LINEN5 )
	{
		fprintf(
		 stream,
		 "linen 5\n" );
	}
	else if( ewf_format == LIBSMIO_EWF_FORMAT_LINEN6 )
	{
		fprintf(
		 stream,
		 "linen 6\n" );
	}
	else if( ewf_format == LIBSMIO_EWF_FORMAT_EWFX )
	{
		fprintf(
		 stream,
		 "extended EWF (libsmio)\n" );
	}
	else
	{
		fprintf(
		 stream,
		 "\n" );
	}
	fprintf(
	 stream, "Imaging start offet:\t\t%" PRIi64 "\n",
	 imaging_offset );

	result = byte_size_string_create(
	          imaging_size_string,
	          16,
	          imaging_size,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          NULL );

	fprintf(
	 stream,
	 "Amount of bytes to acquire:\t" );

	if( imaging_size == 0 )
	{
		fprintf(
		 stream,
		 "%" PRIu64 " (until end of input)",
		 imaging_size );
	}
	else if( result == 1 )
	{
		fprintf(
		 stream,
		 "%" PRIs_LIBSYSTEM " (%" PRIu64 " bytes)",
		 imaging_size_string, imaging_size );
	}
	else
	{
		fprintf(
		 stream,
		 "%" PRIu64 " bytes",
		 imaging_size );
	}
	fprintf(
	 stream,
	 "\n" );

	result = byte_size_string_create(
	          maximum_segment_size_string,
	          16,
	          maximum_segment_size,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          NULL );

	fprintf(
	 stream,
	 "Evidence segment file size:\t" );

	if( result == 1 )
	{
		fprintf(
		 stream, "%" PRIs_LIBSYSTEM " (%" PRIu64 " bytes)",
		 maximum_segment_size_string,
		 maximum_segment_size );
	}
	else
	{
		fprintf(
		 stream,
		 "%" PRIu64 " bytes",
		 maximum_segment_size );
	}
	fprintf(
	 stream,
	 "\n" );

	fprintf(
	 stream,
	 "Bytes per sector:\t\t%" PRIzd "\n",
	 bytes_per_sector );
	fprintf(
	 stream,
	 "Block size:\t\t\t%" PRIu32 " sectors\n",
	 sectors_per_block );
	fprintf(
	 stream,
	 "Error granularity:\t\t%" PRIu32 " sectors\n",
	 sector_error_granularity );
	fprintf(
	 stream,
	 "Retries on read error:\t\t%" PRIu8 "\n",
	 read_error_retry );

	fprintf(
	 stream,
	 "Wipe sectors on read error:\t" );

	if( wipe_block_on_read_error == 0 )
	{
		fprintf(
		 stream,
		 "no\n" );
	}
	else
	{
		fprintf(
		 stream,
		 "yes\n" );
	}
	fprintf(
	 stream,
	 "\n" );

	/* Ask for confirmation
	 */
	while( input_confirmed == -1 )
	{
		if( smioinput_get_fixed_string_variable(
		     stream,
		     input_buffer,
		     input_buffer_size,
		     _LIBSYSTEM_CHARACTER_T_STRING( "Continue imaging with these values" ),
		     smioinput_yes_no,
		     2,
		     0,
		     &fixed_string_variable,
		     NULL ) == -1 )
		{
			fprintf(
			 stream,
			 "Unable to determine answer.\n" );
		}
		else if( smioinput_determine_yes_no(
		          fixed_string_variable,
		          (uint8_t *) &input_confirmed,
		          NULL ) != 1 )
		{
			fprintf(
			 stream,
			 "Selected option not supported, please try again or terminate using Ctrl^C.\n" );

			input_confirmed = -1;
		}
	}
	fprintf(
	 stream,
	 "\n" );

	return( input_confirmed );
}

/* Reads data from a file descriptor and writes it in EWF format
 * Returns the amount of bytes written or -1 on error
 */
ssize64_t smioimage_read_input(
           imaging_handle_t *imaging_handle,
           size64_t media_size,
           size64_t imaging_size,
           off64_t imaging_offset,
           size_t bytes_per_sector,
           uint8_t swap_byte_pairs,
           uint32_t sector_error_granularity,
           uint8_t read_error_retry,
           uint8_t wipe_block_on_read_error,
           size_t process_buffer_size,
           libsystem_character_t *calculated_md5_hash_string,
           size_t calculated_md5_hash_string_size,
           libsystem_character_t *calculated_sha1_hash_string,
           size_t calculated_sha1_hash_string_size,
           process_status_t *process_status,
           liberror_error_t **error )
{
	libsmio_buffer_t *storage_media_buffer = NULL;
	static char *function                  = "smioimage_read_input";
	ssize64_t imaging_count                = 0;
	size_t block_size                      = 0;
	size_t write_size                      = 0;
	ssize_t read_count                     = 0;
	ssize_t process_count                  = 0;
	ssize_t write_count                    = 0;
	uint32_t byte_error_granularity        = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
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
        if( ( imaging_size == 0 )
         || ( imaging_size > media_size )
         || ( imaging_size > (ssize64_t) INT64_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid imaging size value out of range.",
		 function );

		return( -1 );
	}
	if( imaging_offset > 0 )
	{
		if( imaging_offset >= (off64_t) imaging_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid imaging offset.",
			 function );

			return( -1 );
		}
		if( ( imaging_size + imaging_offset ) > media_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: unable to image beyond size of media.",
			 function );

			return( -1 );
		}
		if( imaging_handle_seek_offset(
		     imaging_handle,
		     imaging_offset,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to find imaging offset.",
			 function );

			return( -1 );
		}
	}
	byte_error_granularity = sector_error_granularity * bytes_per_sector;

	if( imaging_handle_get_block_size(
	     imaging_handle,
	     &block_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve block size.",
		 function );

		return( -1 );
	}
	if( block_size == 0 )
	{
		block_size = process_buffer_size;
	}
	else
	{
		process_buffer_size = block_size;
	}
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
	while( imaging_count < (ssize64_t) imaging_size )
	{
		write_size = process_buffer_size;

		if( ( imaging_size - imaging_count ) < write_size )
		{
			write_size = (size_t) ( imaging_size - imaging_count );
		}
		read_count = imaging_handle_read_buffer(
		              imaging_handle,
		              storage_media_buffer,
		              write_size,
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
		process_count = imaging_handle_prepare_read_buffer(
		                 imaging_handle,
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
		if( process_count > (ssize_t) write_size )
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
		 && ( imaging_handle_swap_byte_pairs(
		       imaging_handle,
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
		if( imaging_handle_update_integrity_hash(
		     imaging_handle,
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
		process_count = imaging_handle_prepare_write_buffer(
		                 imaging_handle,
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
		write_count = imaging_handle_write_buffer(
		               imaging_handle,
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
		imaging_count += read_count;

		if( process_status_update(
		     process_status,
		     (size64_t) imaging_count,
		     imaging_count,
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
		if( smioimage_abort != 0 )
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
	write_count = imaging_handle_finalize(
	               imaging_handle,
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
	imaging_count += write_count;

	return( imaging_count );
}

/* Signal handler for smioimage
 */
void smioimage_signal_handler(
      libsystem_signal_t signal )
{
	liberror_error_t *error = NULL;
	static char *function   = "smioimage_signal_handler";

	smioimage_abort = 1;

	if( ( smioimage_imaging_handle != NULL )
	 && ( imaging_handle_signal_abort(
	       smioimage_imaging_handle,
	       &error ) != 1 ) )
	{
		libsystem_notify_printf(
		 "%s: unable to signal imaging handle to abort.\n",
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
	libsystem_character_t imaging_operating_system[ 32 ];
	libsystem_character_t input_buffer[ SMIOIMAGE_INPUT_BUFFER_SIZE ];

	liberror_error_t *error                            = NULL;

	process_status_t *process_status                   = NULL;

	libsystem_character_t *imaging_software_version    = NULL;
	libsystem_character_t *calculated_md5_hash_string  = NULL;
	libsystem_character_t *calculated_sha1_hash_string = NULL;
	libsystem_character_t *case_number                 = NULL;
	libsystem_character_t *description                 = NULL;
	libsystem_character_t *evidence_number             = NULL;
	libsystem_character_t *examiner_name               = NULL;
	libsystem_character_t *fixed_string_variable       = NULL;
	libsystem_character_t *log_filename                = NULL;
	libsystem_character_t *notes                       = NULL;
	libsystem_character_t *option_case_number          = NULL;
	libsystem_character_t *option_description          = NULL;
	libsystem_character_t *option_examiner_name        = NULL;
	libsystem_character_t *option_evidence_number      = NULL;
	libsystem_character_t *option_notes                = NULL;
	libsystem_character_t *option_target_filename      = NULL;
	libsystem_character_t *program                     = _LIBSYSTEM_CHARACTER_T_STRING( "smioimage" );
	libsystem_character_t *target_filename             = NULL;
	libsystem_character_t *value_separator             = NULL;

	FILE *log_file_stream                              = NULL;

	libsystem_integer_t option                         = 0;
	ssize64_t write_count                              = 0;
	size_t bytes_per_sector                            = 512;
	size_t string_length                               = 0;
	uint64_t imaging_offset                            = 0;
	uint64_t imaging_size                              = 0;
	uint64_t input_size_variable                       = 0;
	uint64_t maximum_segment_size_maximum              = 0;
	uint64_t media_size                                = 0;
	uint64_t process_buffer_size                       = 64 * 512;
	uint64_t maximum_segment_size                      = 0;
	uint32_t sector_error_granularity                  = 0;
	uint32_t sectors_per_block                         = 0;
	uint8_t calculate_md5                              = 1;
	uint8_t calculate_sha1                             = 0;
	uint8_t media_type                                 = LIBSMIO_MEDIA_TYPE_FIXED;
	uint8_t print_status_information                   = 1;
	uint8_t read_error_retry                           = 2;
	uint8_t swap_byte_pairs                            = 0;
	uint8_t verbose                                    = 0;
	uint8_t volume_type                                = LIBSMIO_VOLUME_TYPE_LOGICAL;
	uint8_t wipe_block_on_read_error                   = 0;
	int8_t imaging_parameters_confirmed                = 0;
	int argument_set_compression                       = 0;
	int argument_set_bytes_per_sector                  = 0;
	int argument_set_format                            = 0;
	int argument_set_media_type                        = 0;
	int argument_set_offset                            = 0;
	int argument_set_read_error_retry                  = 0;
	int argument_set_sector_error_granularity          = 0;
	int argument_set_sectors_per_block                 = 0;
	int argument_set_maximum_segment_size              = 0;
	int argument_set_size                              = 0;
	int argument_set_volume_type                       = 0;
	int argument_set_wipe_block_on_read_error          = 0;
	int compression_flags                              = 0;
	int compression_level                              = LIBSMIO_COMPRESSION_LEVEL_NONE;
	int compression_type                               = LIBSMIO_COMPRESSION_TYPE_NONE;
	int default_volume_type                            = 0;
	int error_abort                                    = 0;
	int interactive_mode                               = 1;
	int output_ewf_format                              = LIBSMIO_EWF_FORMAT_ENCASE6;
	int output_storage_media_type                      = 0;
	int result                                         = 0;
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

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBSYSTEM_CHARACTER_T_STRING( "b:B:c:C:d:D:e:E:f:g:hl:m:M:N:o:p:P:qr:sS:t:uvVw" ) ) ) != (libsystem_integer_t) -1 )
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

			case (libsystem_integer_t) 'b':
				if( smioinput_determine_sectors_per_block(
				     optarg,
				     &sector_error_granularity,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					sector_error_granularity = 64;

					fprintf(
					 stderr,
					 "Unsuported amount of sector error granularity defaulting to: 64.\n" );
				}
				else
				{
					argument_set_sector_error_granularity = 1;
				}
				break;

			case (libsystem_integer_t) 'B':
				string_length = libsystem_string_length(
				                 optarg );

				if( libsystem_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &imaging_size,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					imaging_size = 0;

					fprintf(
					 stderr,
					 "Unsupported imaging size defaulting to: all bytes.\n" );
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

			case (libsystem_integer_t) 'C':
				option_case_number = optarg;

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

			case (libsystem_integer_t) 'D':
				option_description = optarg;

				break;

			case (libsystem_integer_t) 'e':
				option_examiner_name = optarg;

				break;

			case (libsystem_integer_t) 'E':
				option_evidence_number = optarg;

				break;

			case (libsystem_integer_t) 'f':
				if( smioinput_determine_storage_media_type(
				     optarg,
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
				else
				{
					argument_set_format = 1;
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

			case (libsystem_integer_t) 'g':
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
					 "Unsuported amount of sectors per block defaulting to: 64.\n" );
				}
				else
				{
					argument_set_sectors_per_block = 1;
				}
				break;

			case (libsystem_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libsystem_integer_t) 'l':
				log_filename = optarg;

				break;

			case (libsystem_integer_t) 'm':
				if( smioinput_determine_media_type(
				     optarg,
				     &media_type,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					media_type = LIBSMIO_MEDIA_TYPE_FIXED;

					fprintf(
					 stderr,
					 "Unsupported media type defaulting to: fixed.\n" );
				}
				else
				{
					argument_set_media_type = 1;
				}
				break;

			case (libsystem_integer_t) 'M':
				if( smioinput_determine_volume_type(
				     optarg,
				     &volume_type,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stderr,
					 "Unsupported volume type defaulting to: physical.\n" );

					volume_type = LIBSMIO_VOLUME_TYPE_PHYSICAL;
				}
				else
				{
					argument_set_volume_type = 1;
				}
				break;

			case (libsystem_integer_t) 'N':
				option_notes = optarg;

				break;

			case (libsystem_integer_t) 'o':
				string_length = libsystem_string_length(
				                 optarg );

				if( libsystem_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &imaging_offset,
				     &error ) != 1 )
				{
					imaging_offset = 0;

					fprintf(
					 stderr,
					 "Unsupported imaging offset defaulting to: %" PRIu64 ".\n",
					 imaging_offset );

					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
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

			case (libsystem_integer_t) 'P':
				string_length = libsystem_string_length(
				                 optarg );

				result = byte_size_string_convert(
				          optarg,
				          string_length,
				          &input_size_variable,
				          &error );

				if( result != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
				}
				if( ( result != 1 )
				 || ( input_size_variable > (uint64_t) UINT32_MAX ) )
				{
					input_size_variable = 512;

					fprintf(
					 stderr,
					 "Unsupported amount of bytes per sector defaulting to: %" PRIu64 ".\n",
					 input_size_variable );
				}
				else
				{
					argument_set_bytes_per_sector = 1;
				}
				bytes_per_sector = (size_t) input_size_variable;

				break;

			case (libsystem_integer_t) 'q':
				print_status_information = 0;

				break;

			case (libsystem_integer_t) 'r':
				string_length = libsystem_string_length(
				                 optarg );

				result = libsystem_string_to_uint64(
				          optarg,
				          string_length + 1,
				          &input_size_variable,
				          &error );

				if( result != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
				}
				if( ( result != 1 )
				 || ( input_size_variable > 255 ) )
				{
					input_size_variable = 2;

					fprintf(
					 stderr,
					 "Unsupported amount of read error retries defaulting to: %" PRIu64 ".\n",
					 input_size_variable );
				}
				read_error_retry = (uint8_t) input_size_variable;

				argument_set_read_error_retry = 1;

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
				          NULL );

				if( result != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
				}
				argument_set_maximum_segment_size = 1;

				if( ( result != 1 )
				 || ( maximum_segment_size < SMIOCOMMON_MINIMUM_SEGMENT_SIZE )
				 || ( ( output_ewf_format == LIBSMIO_EWF_FORMAT_ENCASE6 )
				  && ( maximum_segment_size >= (uint64_t) SMIOCOMMON_MAXIMUM_SEGMENT_SIZE_64BIT ) )
				 || ( ( output_ewf_format != LIBSMIO_EWF_FORMAT_ENCASE6 )
				  && ( maximum_segment_size >= (uint64_t) SMIOCOMMON_MAXIMUM_SEGMENT_SIZE_32BIT ) ) )
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
				 stdout );

				return( EXIT_SUCCESS );

			case (libsystem_integer_t) 'w':
				wipe_block_on_read_error              = 1;
				argument_set_wipe_block_on_read_error = 1;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

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

	/* Check if to read from stdin
	 */
	if( libsystem_string_compare(
	     argv[ optind ],
	     _LIBSYSTEM_CHARACTER_T_STRING( "-" ),
	     1 ) == 0 )
	{
		fprintf(
		 stderr,
		 "Reading from stdin not supported.\n" );

		return( EXIT_FAILURE );
	}
	if( imaging_handle_initialize(
	     &smioimage_imaging_handle,
	     calculate_md5,
	     calculate_sha1,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create imaging handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	/* Open the input file or device size
	 */
	if( imaging_handle_open_input(
	     smioimage_imaging_handle,
	     argv[ optind ],
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open file or device: %" PRIs_LIBSYSTEM ".\n",
		 argv[ optind ] );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		imaging_handle_free(
		 &smioimage_imaging_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( imaging_handle_get_input_media_size(
	     smioimage_imaging_handle,
	     &media_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve input media size.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		imaging_handle_close(
		 smioimage_imaging_handle,
		 NULL );
		imaging_handle_free(
		 &smioimage_imaging_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( ( argument_set_bytes_per_sector == 0 )
	 && ( imaging_handle_get_input_bytes_per_sector(
	       smioimage_imaging_handle,
	       &bytes_per_sector,
	       &error ) != 1 ) )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve input bytes per sector.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		imaging_handle_close(
		 smioimage_imaging_handle,
		 NULL );
		imaging_handle_free(
		 &smioimage_imaging_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( ( imaging_size == 0 )
	 || ( imaging_size > ( media_size - imaging_offset ) ) )
	{
		imaging_size = media_size - imaging_offset;
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
				 "Unable to create target filename string.\n" );

				error_abort = 1;
			}
			else if( memory_copy(
				  target_filename,
				  option_target_filename,
				  string_length + 1 ) == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to set target filename string.\n" );

				error_abort = 1;
			}
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
			if( memory_copy(
			     target_filename,
			     _LIBSYSTEM_CHARACTER_T_STRING( "imaging" ),
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
	if( error_abort == 0 )
	{
		if( option_case_number != NULL )
		{
			string_length = libsystem_string_length(
					 option_case_number );

			if( string_length > 0 )
			{
				case_number = (libsystem_character_t *) memory_allocate(
				                                         sizeof( libsystem_character_t ) * ( string_length + 1 ) );

				if( case_number == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to create case number string.\n" );

					error_abort = 1;
				}
				else if( memory_copy(
					  case_number,
					  option_case_number,
					  string_length + 1 ) == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to set case number string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			case_number = (libsystem_character_t *) memory_allocate(
			                                         sizeof( libsystem_character_t ) * 256 );

			if( case_number == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create case number string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort == 0 )
	{
		if( option_description != NULL )
		{
			string_length = libsystem_string_length(
					 option_description );

			if( string_length > 0 )
			{
				description = (libsystem_character_t *) memory_allocate(
				                                         sizeof( libsystem_character_t ) * ( string_length + 1 ) );

				if( description == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to create description string.\n" );

					error_abort = 1;
				}
				else if( memory_copy(
					  description,
					  option_description,
					  string_length + 1 ) == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to set description string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			description = (libsystem_character_t *) memory_allocate(
			                                         sizeof( libsystem_character_t ) * 256 );

			if( description == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create description string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort == 0 )
	{
		if( option_examiner_name != NULL )
		{
			string_length = libsystem_string_length(
					 option_examiner_name );

			if( string_length > 0 )
			{
				examiner_name = (libsystem_character_t *) memory_allocate(
				                                           sizeof( libsystem_character_t ) * ( string_length + 1 ) );

				if( examiner_name == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to create examiner name string.\n" );

					error_abort = 1;
				}
				else if( memory_copy(
					  examiner_name,
					  option_examiner_name,
					  string_length + 1 ) == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to set examiner name string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			evidence_number = (libsystem_character_t *) memory_allocate(
			                                             sizeof( libsystem_character_t ) * 256 );

			if( evidence_number == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create evidence number string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort == 0 )
	{
		if( option_evidence_number != NULL )
		{
			string_length = libsystem_string_length(
					 option_evidence_number );

			if( string_length > 0 )
			{
				evidence_number = (libsystem_character_t *) memory_allocate(
				                                             sizeof( libsystem_character_t ) * ( string_length + 1 ) );

				if( evidence_number == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to create evidence number string.\n" );

					error_abort = 1;
				}
				else if( memory_copy(
					  evidence_number,
					  option_evidence_number,
					  string_length + 1 ) == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to set evidence number string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			examiner_name = (libsystem_character_t *) memory_allocate(
			                                           sizeof( libsystem_character_t ) * 256 );

			if( examiner_name == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create examiner name string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort == 0 )
	{
		if( option_notes != NULL )
		{
			string_length = libsystem_string_length(
					 option_notes );

			if( string_length > 0 )
			{
				notes = (libsystem_character_t *) memory_allocate(
				                                   sizeof( libsystem_character_t ) * ( string_length + 1 ) );

				if( notes == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to create notes string.\n" );

					error_abort = 1;
				}
				else if( memory_copy(
					  notes,
					  option_notes,
					  string_length + 1 ) == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to set notes string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			notes = (libsystem_character_t *) memory_allocate(
			                                   sizeof( libsystem_character_t ) * 256 );

			if( notes == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create notes string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort != 0 )
	{
		if( case_number != NULL )
		{
			memory_free(
			 case_number );
		}
		if( description != NULL )
		{
			memory_free(
			 description );
		}
		if( examiner_name != NULL )
		{
			memory_free(
			 examiner_name );
		}
		if( evidence_number != NULL )
		{
			memory_free(
			 evidence_number );
		}
		if( notes != NULL )
		{
			memory_free(
			 notes );
		}
		if( target_filename != NULL )
		{
			memory_free(
			 target_filename );
		}
		imaging_handle_close(
		 smioimage_imaging_handle,
		 NULL );
		imaging_handle_free(
		 &smioimage_imaging_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	/* Request the necessary case data
	 */
	while( ( interactive_mode == 1 )
	 && ( imaging_parameters_confirmed == 0 ) )
	{
		fprintf(
		 stdout,
		 "Imaging parameters required, please provide the necessary input\n" );

		/* Target filename
		 */
		if( option_target_filename == NULL )
		{
			while( smioinput_get_string_variable(
				stdout,
				_LIBSYSTEM_CHARACTER_T_STRING( "Image path and filename without extension" ),
				target_filename,
				1024,
			        NULL ) != 1 )
			{
				fprintf(
				 stdout,
				 "Filename is required, please try again or terminate using Ctrl^C.\n" );
			}
		}
		/* Case number
		 */
		if( ( option_case_number == NULL )
		 && ( smioinput_get_string_variable(
		       stdout,
		       _LIBSYSTEM_CHARACTER_T_STRING( "Case number" ),
		       case_number,
		       256,
		       &error ) == -1 ) )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			case_number[ 0 ] = 0;

			fprintf(
			 stdout,
			 "Unable to set case number string.\n" );
		}
		/* Description
		 */
		if( ( option_description == NULL )
		 && ( smioinput_get_string_variable(
		       stdout,
		       _LIBSYSTEM_CHARACTER_T_STRING( "Description" ),
		       description,
		       256,
		       &error ) == -1 ) )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			description[ 0 ] = 0;

			fprintf(
			 stdout,
			 "Unable to set description string.\n" );
		}
		/* Evidence number
		 */
		if( ( option_evidence_number == NULL )
		 && ( smioinput_get_string_variable(
		       stdout,
		       _LIBSYSTEM_CHARACTER_T_STRING( "Evidence number" ),
		       evidence_number,
		       256,
		       &error ) == -1 ) )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			evidence_number[ 0 ] = 0;

			fprintf(
			 stdout,
			 "Unable to set evidence number string.\n" );
		}
		/* Examiner name
		 */
		if( ( option_examiner_name == NULL )
		 && ( smioinput_get_string_variable(
		       stdout,
		       _LIBSYSTEM_CHARACTER_T_STRING( "Examiner name" ),
		       examiner_name,
		       256,
		       &error ) == -1 ) )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			examiner_name[ 0 ] = 0;

			fprintf(
			 stdout,
			 "Unable to set examiner name string.\n" );
		}
		/* Notes
		 */
		if( ( option_notes == NULL )
		 && ( smioinput_get_string_variable(
		       stdout,
		       _LIBSYSTEM_CHARACTER_T_STRING( "Notes" ),
		       notes,
		       256,
		       &error ) == -1 ) )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			notes[ 0 ] = 0;

			fprintf(
			 stdout,
			 "Unable to set notes string.\n" );
		}
		/* Media type
		 */
		if( argument_set_media_type == 0 )
		{
			if( smioinput_get_fixed_string_variable(
			     stdout,
			     input_buffer,
			     SMIOIMAGE_INPUT_BUFFER_SIZE,
			     _LIBSYSTEM_CHARACTER_T_STRING( "Media type" ),
			     smioinput_media_types,
			     SMIOINPUT_MEDIA_TYPES_AMOUNT,
			     SMIOINPUT_MEDIA_TYPES_DEFAULT,
			     &fixed_string_variable,
			     &error ) == -1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				media_type = LIBSMIO_MEDIA_TYPE_FIXED;
	
				fprintf(
				 stdout,
				 "Unable to determine media type defaulting to: fixed.\n" );
			}
			else if( smioinput_determine_media_type(
				  fixed_string_variable,
				  &media_type,
			          &error ) != 1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				media_type = LIBSMIO_MEDIA_TYPE_FIXED;

				fprintf(
				 stdout,
				 "Unsupported media type defaulting to: fixed.\n" );
			}
		}
		/* Volume type
		 */
		if( argument_set_volume_type == 0 )
		{
			default_volume_type = SMIOINPUT_VOLUME_TYPES_DEFAULT;

			if( ( media_type == LIBSMIO_MEDIA_TYPE_REMOVABLE )
			 || ( media_type == LIBSMIO_MEDIA_TYPE_OPTICAL ) )
			{
				default_volume_type = 0;
			}
			if( smioinput_get_fixed_string_variable(
			     stdout,
			     input_buffer,
			     SMIOIMAGE_INPUT_BUFFER_SIZE,
			     _LIBSYSTEM_CHARACTER_T_STRING( "Volume type" ),
			     smioinput_volume_types,
			     SMIOINPUT_VOLUME_TYPES_AMOUNT,
			     (uint8_t) default_volume_type,
			     &fixed_string_variable,
			     &error ) == -1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				volume_type = LIBSMIO_VOLUME_TYPE_PHYSICAL;

				fprintf(
				 stdout,
				 "Unable to determine volume type defaulting to: physical.\n" );
			}
			else if( smioinput_determine_volume_type(
				  fixed_string_variable,
				  &volume_type,
			          &error ) != 1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				volume_type = LIBSMIO_VOLUME_TYPE_PHYSICAL;

				fprintf(
				 stdout,
				 "Unsupported volume type defaulting to: physical.\n" );
			}
		}
		/* File format
		 */
		if( argument_set_format == 0 )
		{
			if( smioinput_get_fixed_string_variable(
			     stderr,
			     input_buffer,
			     SMIOIMAGE_INPUT_BUFFER_SIZE,
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
				     SMIOIMAGE_INPUT_BUFFER_SIZE,
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
				/* Compression
				 */
				if( argument_set_compression == 0 )
				{
					if( smioinput_get_fixed_string_variable(
					     stdout,
					     input_buffer,
					     SMIOIMAGE_INPUT_BUFFER_SIZE,
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
						 stdout,
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
						 stdout,
						 "Unsupported compression type defaulting to: none.\n" );
					}
				}
			}
			else if( output_storage_media_type == LIBSMIO_TYPE_RAW )
			{
				compression_level = LIBSMIO_COMPRESSION_LEVEL_NONE;
				compression_flags = 0;
			}
		}
		/* Offset of data to acquire
		 */
		if( ( argument_set_offset == 0 )
		 && ( smioinput_get_size_variable(
		       stdout,
		       input_buffer,
		       SMIOIMAGE_INPUT_BUFFER_SIZE,
		       _LIBSYSTEM_CHARACTER_T_STRING( "Start to acquire at offset" ),
		       0,
		       media_size,
		       0,
		       &imaging_offset,
		       &error ) == -1 ) )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			imaging_offset = 0;

			fprintf(
			 stdout,
			 "Unable to determine imaging offset defaulting to: %" PRIu64 ".\n",
			 imaging_offset );
		}
		/* Size of data to acquire
		 */
		if( ( argument_set_size == 0 )
		 && ( smioinput_get_size_variable(
		       stdout,
		       input_buffer,
		       SMIOIMAGE_INPUT_BUFFER_SIZE,
		       _LIBSYSTEM_CHARACTER_T_STRING( "The amount of bytes to acquire" ),
		       0,
		       ( media_size - imaging_offset ),
		       ( media_size - imaging_offset ),
		       &imaging_size,
		       &error ) == -1 ) )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			imaging_size = media_size - imaging_offset;

			fprintf(
			 stdout,
			 "Unable to determine input size defaulting to: %" PRIu64 ".\n",
			 imaging_size );
		}
		/* Segment file size
		 */
		if( argument_set_maximum_segment_size == 0 )
		{
			if( output_ewf_format == LIBSMIO_EWF_FORMAT_ENCASE6 )
			{
				maximum_segment_size_maximum = SMIOCOMMON_MAXIMUM_SEGMENT_SIZE_64BIT;
			}
			else
			{
				maximum_segment_size_maximum = SMIOCOMMON_MAXIMUM_SEGMENT_SIZE_32BIT;
			}
			if( smioinput_get_byte_size_variable(
			     stdout,
			     input_buffer,
			     SMIOIMAGE_INPUT_BUFFER_SIZE,
			     _LIBSYSTEM_CHARACTER_T_STRING( "Evidence segment file size in bytes" ),
			     ( SMIOCOMMON_MINIMUM_SEGMENT_SIZE ),
			     ( maximum_segment_size_maximum ),
			     ( SMIOCOMMON_DEFAULT_SEGMENT_SIZE ),
			     &maximum_segment_size,
			     &error ) == -1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				maximum_segment_size = SMIOCOMMON_DEFAULT_SEGMENT_SIZE;

				fprintf(
				 stdout,
				 "Unable to determine segment file size defaulting to: %" PRIu64 ".\n",
				 maximum_segment_size );
			}
			/* Make sure the segment file size is smaller than or equal to the maximum
			 */
			if( maximum_segment_size > maximum_segment_size_maximum )
			{
				maximum_segment_size = maximum_segment_size_maximum;
			}
		}
		/* Bytes per sector
		 */
		if( argument_set_bytes_per_sector == 0 )
		{
			if( smioinput_get_size_variable(
			     stdout,
			     input_buffer,
			     SMIOIMAGE_INPUT_BUFFER_SIZE,
			     _LIBSYSTEM_CHARACTER_T_STRING( "The amount of bytes per sector" ),
			     0,
			     UINT32_MAX,
			     bytes_per_sector,
			     &input_size_variable,
			     &error ) == -1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				input_size_variable = 512;

				fprintf(
				 stdout,
				 "Unable to determine bytes per sector defaulting to: %" PRIu64 ".\n",
				 input_size_variable );
			}
			bytes_per_sector = (size_t) input_size_variable;
		}
		/* Chunk size (sectors per block)
		 */
		if( argument_set_sectors_per_block == 0 )
		{
			if( smioinput_get_fixed_string_variable(
			     stdout,
			     input_buffer,
			     SMIOIMAGE_INPUT_BUFFER_SIZE,
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
				 stdout,
				 "Unable to determine sectors per block defaulting to: 64.\n" );
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
				 stdout,
				 "Unsupported sectors per block defaulting to: 64.\n" );
			}
		}
		/* Error granularity
		 */
		if( argument_set_sector_error_granularity == 0 )
		{
			if( smioinput_get_size_variable(
			     stdout,
			     input_buffer,
			     SMIOIMAGE_INPUT_BUFFER_SIZE,
			     _LIBSYSTEM_CHARACTER_T_STRING( "The amount of sectors to be used as error granularity" ),
			     1,
			     (uint64_t) sectors_per_block,
			     64,
			     &input_size_variable,
			     &error ) == -1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				input_size_variable = 64;

				fprintf(
				 stdout,
				 "Unable to determine sector error granularity defaulting to: %" PRIu64 ".\n",
				 input_size_variable );
			}
			sector_error_granularity = (uint32_t) input_size_variable;
		}
		/* The amount of read error retry
		 */
		if( argument_set_read_error_retry == 0 )
		{
			if( smioinput_get_size_variable(
			     stdout,
			     input_buffer,
			     SMIOIMAGE_INPUT_BUFFER_SIZE,
			     _LIBSYSTEM_CHARACTER_T_STRING( "The amount of retries when a read error occurs" ),
			     0,
			     255,
			     2,
			     &input_size_variable,
			     &error ) == -1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				input_size_variable = 2;

				fprintf(
				 stdout,
				 "Unable to determine read error retry defaulting to: %" PRIu64 ".\n",
				 input_size_variable );
			}
			read_error_retry = (uint8_t) input_size_variable;
		}
		/* Wipe the sector on error
		 */
		if( argument_set_wipe_block_on_read_error == 0 )
		{
			if( smioinput_get_fixed_string_variable(
			     stdout,
			     input_buffer,
			     SMIOIMAGE_INPUT_BUFFER_SIZE,
			     _LIBSYSTEM_CHARACTER_T_STRING( "Wipe sectors on read error (mimic EnCase like behavior)" ),
			     smioinput_yes_no,
			     2,
			     1,
			     &fixed_string_variable,
			     &error ) == -1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				fprintf(
				 stdout,
				 "Unable to determine wipe block on error defaulting to: no.\n" );

				wipe_block_on_read_error = 0;
			}
			else if( smioinput_determine_yes_no(
				  fixed_string_variable,
				  &wipe_block_on_read_error,
			          &error ) != 1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				fprintf(
				 stdout,
				 "Unsupported wipe block on error defaulting to: no.\n" );

				wipe_block_on_read_error = 0;
			}
		}
		fprintf(
		 stdout,
		 "\n" );

		/* Check if user is content with the imaging parameters
		 */
		imaging_parameters_confirmed = smioimage_confirm_imaging_parameters(
		                                stdout,
		                                input_buffer,
		                                SMIOIMAGE_INPUT_BUFFER_SIZE,
		                                target_filename,
		                                case_number,
		                                description,
		                                evidence_number,
		                                examiner_name,
		                                notes,
		                                media_type,
		                                volume_type,
		                                compression_type,
		                                compression_level,
		                                compression_flags,
		                                output_storage_media_type,
		                                output_ewf_format,
		                                (off64_t) imaging_offset,
		                                (size64_t) imaging_size,
		                                (size64_t) maximum_segment_size,
		                                bytes_per_sector,
		                                sectors_per_block,
		                                sector_error_granularity,
		                                read_error_retry,
		                                wipe_block_on_read_error,
		                                &error );

		if( imaging_parameters_confirmed == -1 )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			fprintf(
			 stdout,
			 "Unable to determine if imaging parameters are correct aborting.\n" );

			smioimage_abort = 1;

			break;
		}
		/* Reset all parameters provided as command line arguments
		 */
		else if( imaging_parameters_confirmed == 0 )
		{
			argument_set_compression              = 0;
			argument_set_format                   = 0;
			argument_set_media_type               = 0;
			argument_set_offset                   = 0;
			argument_set_read_error_retry         = 0;
			argument_set_sector_error_granularity = 0;
			argument_set_sectors_per_block        = 0;
			argument_set_maximum_segment_size        = 0;
			argument_set_size                     = 0;
			argument_set_volume_type              = 0;
			argument_set_wipe_block_on_read_error = 0;
		}
	}
	if( libsystem_signal_attach(
	     smioimage_signal_handler,
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
	if( smioimage_abort == 0 )
	{
		if( platform_get_operating_system(
		     imaging_operating_system,
		     32,
		     &error ) != 1 )
		{
			fprintf(
			 stdout,
			 "Unable to determine operating system.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			imaging_operating_system[ 0 ] = 0;
		}
		imaging_software_version = _LIBSYSTEM_CHARACTER_T_STRING( LIBSMIO_VERSION_STRING );

		if( imaging_handle_open_output(
		     smioimage_imaging_handle,
		     target_filename,
		     output_storage_media_type,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to open output file(s).\n" );

			error_abort = 1;
		}
		if( ( error_abort == 0 )
		 && ( output_storage_media_type == LIBSMIO_TYPE_EWF )
		 && ( imaging_handle_set_output_ewf_format(
		       smioimage_imaging_handle,
		       output_ewf_format,
		       &error ) != 1 ) )
		{
			fprintf(
			 stderr,
			 "Unable to initialize EWF output format.\n" );

			error_abort = 1;
		}
		if( ( error_abort == 0 )
		 && ( imaging_handle_set_output_values(
		       smioimage_imaging_handle,
		       case_number,
		       description,
		       evidence_number,
		       examiner_name,
		       notes,
		       imaging_operating_system,
		       program,
		       imaging_software_version,
		       bytes_per_sector,
		       imaging_size,
		       media_type,
		       volume_type,
		       compression_type,
		       compression_level,
		       compression_flags,
		       maximum_segment_size,
		       0,
		       sectors_per_block,
		       sector_error_granularity,
		       &error ) != 1 ) )
		{
			fprintf(
			 stderr,
			 "Unable to initialize output settings.\n" );

			error_abort = 1;
		}
	}
	memory_free(
	 notes );
	memory_free(
	 examiner_name );
	memory_free(
	 evidence_number );
	memory_free(
	 description );
	memory_free(
	 case_number );
	memory_free(
	 target_filename );

	if( error_abort != 0 )
	{
		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		imaging_handle_close(
		 smioimage_imaging_handle,
		 NULL );
		imaging_handle_free(
		 &smioimage_imaging_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( calculate_md5 == 1 )
	{
		calculated_md5_hash_string = (libsystem_character_t *) memory_allocate(
		                                                        sizeof( libsystem_character_t ) * DIGEST_HASH_STRING_SIZE_MD5 );

		if( calculated_md5_hash_string == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to create calculated MD5 hash string.\n" );

			imaging_handle_close(
			 smioimage_imaging_handle,
			 NULL );
			imaging_handle_free(
			 &smioimage_imaging_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	if( calculate_sha1 == 1 )
	{
		calculated_sha1_hash_string = (libsystem_character_t *) memory_allocate(
		                                                         sizeof( libsystem_character_t ) * DIGEST_HASH_STRING_SIZE_SHA1 );

		if( calculated_sha1_hash_string == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to create calculated SHA1 hash string.\n" );

			memory_free(
			 calculated_md5_hash_string );

			imaging_handle_close(
			 smioimage_imaging_handle,
			 NULL );
			imaging_handle_free(
			 &smioimage_imaging_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	if( smioimage_abort == 0 )
	{
		if( process_status_initialize(
		     &process_status,
		     _LIBSYSTEM_CHARACTER_T_STRING( "Imaging" ),
		     _LIBSYSTEM_CHARACTER_T_STRING( "acquired" ),
		     _LIBSYSTEM_CHARACTER_T_STRING( "Written" ),
		     stdout,
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
			imaging_handle_close(
			 smioimage_imaging_handle,
			 NULL );
			imaging_handle_free(
			 &smioimage_imaging_handle,
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
			imaging_handle_close(
			 smioimage_imaging_handle,
			 NULL );
			imaging_handle_free(
			 &smioimage_imaging_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		/* Start acquiring data
		 */
		write_count = smioimage_read_input(
		               smioimage_imaging_handle,
			       media_size,
			       imaging_size,
			       imaging_offset,
			       bytes_per_sector,
			       swap_byte_pairs,
		               sector_error_granularity,
		               read_error_retry,
		               wipe_block_on_read_error,
			       (size_t) process_buffer_size,
			       calculated_md5_hash_string,
			       DIGEST_HASH_STRING_SIZE_MD5,
			       calculated_sha1_hash_string,
			       DIGEST_HASH_STRING_SIZE_SHA1,
			       process_status,
			       &error );

		if( write_count <= -1 )
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
	if( smioimage_abort != 0 )
	{
		status = PROCESS_STATUS_ABORTED;
	}
	if( process_status_stop(
	     process_status,
	     (size64_t) write_count,
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
		imaging_handle_close(
		 smioimage_imaging_handle,
		 NULL );
		imaging_handle_free(
		 &smioimage_imaging_handle,
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
		imaging_handle_close(
		 smioimage_imaging_handle,
		 NULL );
		imaging_handle_free(
		 &smioimage_imaging_handle,
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
		if( imaging_handle_imaging_errors_fprint(
		     smioimage_imaging_handle,
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
		if( ( log_file_stream != NULL )
		 && ( imaging_handle_imaging_errors_fprint(
		       smioimage_imaging_handle,
		       log_file_stream,
		       &error ) != 1 ) )
		{
			fprintf(
			 stderr,
			 "Unable to write imaging errors in log file.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	if( imaging_handle_close(
	     smioimage_imaging_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close output file(s).\n" );

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
		imaging_handle_free(
		 &smioimage_imaging_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( imaging_handle_free(
	     &smioimage_imaging_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free imaging handle.\n" );

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
		 stdout,
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
		 stdout,
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

