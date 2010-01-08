/* 
 * Export handle
 *
 * Copyright (C) 2008-2010, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _EXPORT_HANDLE_H )
#define _EXPORT_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

/* If libtool DLL support is enabled set LIBSMIO_DLL_IMPORT
 * before including libsmio_extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBSMIO_DLL_IMPORT
#endif

#include <libsmio.h>

#include <libsystem.h>

#include "digest_context.h"
#include "digest_hash.h"
#include "md5.h"
#include "sha1.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum EXPORT_HANDLE_OUTPUT_FORMATS
{
	EXPORT_HANDLE_OUTPUT_FORMAT_AFF		= (int) 'a',
	EXPORT_HANDLE_OUTPUT_FORMAT_EWF		= (int) 'e',
	EXPORT_HANDLE_OUTPUT_FORMAT_RAW		= (int) 'r'
};

typedef struct export_handle export_handle_t;

struct export_handle
{
	/* Value to indicate if the MD5 digest hash should be calculated
	 */
	uint8_t calculate_md5;

	/* Value to indicate if the SHA1 digest hash should be calculated
	 */
	uint8_t calculate_sha1;

	/* The MD5 digest context
	 */
	md5_context_t md5_context;

	/* The SHA1 digest context
	 */
	sha1_context_t sha1_context;

	/* The input storage media type
	 */
	int input_storage_media_type;

	/* The libsmio input handle
	 */
	libsmio_handle_t *input_handle;

	/* The output format
	 */
	uint8_t output_format;

	/* The libsmio output handle
	 */
	libsmio_handle_t *output_handle;

	/* The block size
	 */
	size_t block_size;

	/* The amount of bytes per sector
	 */
	size_t bytes_per_sector;
};

int export_handle_initialize(
     export_handle_t **export_handle,
     uint8_t calculate_md5,
     uint8_t calculate_sha1,
     liberror_error_t **error );

int export_handle_free(
     export_handle_t **export_handle,
     liberror_error_t **error );

int export_handle_signal_abort(
     export_handle_t *export_handle,
     liberror_error_t **error );

int export_handle_open_input(
     export_handle_t *export_handle,
     libsystem_character_t * const * filenames,
     int amount_of_filenames,
     liberror_error_t **error );

int export_handle_open_output(
     export_handle_t *export_handle,
     const libsystem_character_t *filename,
     int output_storage_media_type,
     liberror_error_t **error );

int export_handle_close(
     export_handle_t *export_handle,
     liberror_error_t **error );

ssize_t export_handle_prepare_read_buffer(
         export_handle_t *export_handle,
         libsmio_buffer_t *storage_media_buffer,
         liberror_error_t **error );

ssize_t export_handle_read_buffer(
         export_handle_t *export_handle,
         libsmio_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error );

ssize_t export_handle_prepare_write_buffer(
         export_handle_t *export_handle,
         libsmio_buffer_t *storage_media_buffer,
         liberror_error_t **error );

ssize_t export_handle_write_buffer(
         export_handle_t *export_handle,
         libsmio_buffer_t *storage_media_buffer,
         size_t write_size,
         liberror_error_t **error );

off64_t export_handle_seek_offset(
         export_handle_t *export_handle,
         off64_t offset,
         liberror_error_t **error );

int export_handle_swap_byte_pairs(
     export_handle_t *export_handle,
     libsmio_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error );

int export_handle_update_integrity_hash(
     export_handle_t *export_handle,
     libsmio_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error );

int export_handle_get_input_media_size(
     export_handle_t *export_handle,
     size64_t *media_size,
     liberror_error_t **error );

int export_handle_get_input_block_size(
     export_handle_t *export_handle,
     size_t *block_size,
     liberror_error_t **error );

int export_handle_set_output_values(
     export_handle_t *export_handle,
     libsystem_character_t *acquiry_operating_system,
     libsystem_character_t *acquiry_software,
     libsystem_character_t *acquiry_software_version,
     size64_t media_size,
     int compression_type,
     int compression_level,
     int compression_flags,
     size64_t maximum_segment_size,
     int maximum_possible_segment_size,
     uint32_t sectors_per_block,
     uint8_t clear_buffer_on_error,
     liberror_error_t **error );

int export_handle_set_output_ewf_format(
     export_handle_t *export_handle,
     uint8_t ewf_format,
     liberror_error_t **error );

int export_handle_set_information_value(
     export_handle_t *export_handle,
     char *information_value_identifier,
     size_t information_value_identifier_length,
     libsystem_character_t *information_value,
     size_t information_value_length,
     liberror_error_t **error );

int export_handle_set_integrity_hash_value(
     export_handle_t *export_handle,
     char *integrity_hash_value_identifier,
     size_t integrity_hash_value_identifier_length,
     libsystem_character_t *integrity_hash_value,
     size_t integrity_hash_value_length,
     liberror_error_t **error );

ssize_t export_handle_finalize(
         export_handle_t *export_handle,
         libsystem_character_t *calculated_md5_hash_string,
         size_t calculated_md5_hash_string_size,
         libsystem_character_t *calculated_sha1_hash_string,
         size_t calculated_sha1_hash_string_size,
         liberror_error_t **error );

int export_handle_export_errors_fprint(
     export_handle_t *export_handle,
     FILE *stream,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

