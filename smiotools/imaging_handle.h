/* 
 * Imaging handle
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

#if !defined( _IMAGING_HANDLE_H )
#define _IMAGING_HANDLE_H

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

typedef struct imaging_handle imaging_handle_t;

struct imaging_handle
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

	/* The libsmio input handle
	 */
	libsmio_handle_t *input_handle;

	/* The libsmio output handle
	 */
	libsmio_handle_t *output_handle;

	/* The amount of bytes per sector
	 */
	uint32_t bytes_per_sector;
};

int imaging_handle_initialize(
     imaging_handle_t **imaging_handle,
     uint8_t calculate_md5,
     uint8_t calculate_sha1,
     liberror_error_t **error );

int imaging_handle_free(
     imaging_handle_t **imaging_handle,
     liberror_error_t **error );

int imaging_handle_signal_abort(
     imaging_handle_t *imaging_handle,
     liberror_error_t **error );

int imaging_handle_open_input(
     imaging_handle_t *imaging_handle,
     const libsystem_character_t *filename,
     liberror_error_t **error );

int imaging_handle_open_output(
     imaging_handle_t *imaging_handle,
     const libsystem_character_t *filename,
     int output_storage_media_type,
     liberror_error_t **error );

ssize_t imaging_handle_prepare_read_buffer(
         imaging_handle_t *imaging_handle,
         libsmio_buffer_t *storage_media_buffer,
         liberror_error_t **error );

ssize_t imaging_handle_read_buffer(
         imaging_handle_t *imaging_handle,
         libsmio_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error );

ssize_t imaging_handle_prepare_write_buffer(
         imaging_handle_t *imaging_handle,
         libsmio_buffer_t *storage_media_buffer,
         liberror_error_t **error );

ssize_t imaging_handle_write_buffer(
         imaging_handle_t *imaging_handle,
         libsmio_buffer_t *storage_media_buffer,
         size_t write_size,
         liberror_error_t **error );

off64_t imaging_handle_seek_offset(
         imaging_handle_t *imaging_handle,
         off64_t offset,
         liberror_error_t **error );

int imaging_handle_swap_byte_pairs(
     imaging_handle_t *imaging_handle,
     libsmio_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error );

int imaging_handle_update_integrity_hash(
     imaging_handle_t *imaging_handle,
     libsmio_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error );

int imaging_handle_close(
     imaging_handle_t *imaging_handle,
     liberror_error_t **error );

int imaging_handle_get_input_media_size(
     imaging_handle_t *imaging_handle,
     size64_t *media_size,
     liberror_error_t **error );

int imaging_handle_get_input_bytes_per_sector(
     imaging_handle_t *imaging_handle,
     size_t *bytes_per_sector,
     liberror_error_t **error );

int imaging_handle_get_block_size(
     imaging_handle_t *imaging_handle,
     size_t *block_size,
     liberror_error_t **error );

int imaging_handle_set_output_values(
     imaging_handle_t *imaging_handle,
     libsystem_character_t *case_number,
     libsystem_character_t *description,
     libsystem_character_t *evidence_number,
     libsystem_character_t *examiner_name,
     libsystem_character_t *notes,
     libsystem_character_t *acquiry_operating_system,
     libsystem_character_t *acquiry_software,
     libsystem_character_t *acquiry_software_version,
     uint32_t bytes_per_sector,
     size64_t media_size,
     int media_type,
     int volume_type,
     int compression_type,
     int compression_level,
     int compression_flags,
     size64_t maximum_segment_size,
     int maximum_possible_segment_size,
     uint32_t sectors_per_block,
     uint32_t sector_error_granularity,
     liberror_error_t **error );

int imaging_handle_set_output_ewf_format(
     imaging_handle_t *imaging_handle,
     uint8_t ewf_format,
     liberror_error_t **error );

int imaging_handle_set_information_value(
     imaging_handle_t *imaging_handle,
     char *information_value_identifier,
     size_t information_value_identifier_length,
     libsystem_character_t *information_value,
     size_t information_value_length,
     liberror_error_t **error );

int imaging_handle_set_integrity_hash_value(
     imaging_handle_t *imaging_handle,
     char *integrity_hash_value_identifier,
     size_t integrity_hash_value_identifier_length,
     libsystem_character_t *integrity_hash_value,
     size_t integrity_hash_value_length,
     liberror_error_t **error );

int imaging_handle_add_read_error(
      imaging_handle_t *imaging_handle,
      off64_t start_offset,
      size_t amount_of_bytes,
      liberror_error_t **error );

ssize_t imaging_handle_finalize(
         imaging_handle_t *imaging_handle,
         libsystem_character_t *calculated_md5_hash_string,
         size_t calculated_md5_hash_string_size,
         libsystem_character_t *calculated_sha1_hash_string,
         size_t calculated_sha1_hash_string_size,
         liberror_error_t **error );

int imaging_handle_imaging_errors_fprint(
     imaging_handle_t *imaging_handle,
     FILE *stream,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

