/* 
 * Verification handle
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

#if !defined( _VERIFICATION_HANDLE_H )
#define _VERIFICATION_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

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

typedef struct verification_handle verification_handle_t;

struct verification_handle
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

	/* The block size
	 */
	size_t block_size;

	/* The amount of bytes per sector
	 */
	size_t bytes_per_sector;

	/* Value to indicate if the block should be wiped on error
	 */
	int wipe_block_on_error;
};

int verification_handle_initialize(
     verification_handle_t **verification_handle,
     uint8_t calculate_md5,
     uint8_t calculate_sha1,
     liberror_error_t **error );

int verification_handle_free(
     verification_handle_t **verification_handle,
     liberror_error_t **error );

int verification_handle_signal_abort(
     verification_handle_t *verification_handle,
     liberror_error_t **error );

int verification_handle_open_input(
     verification_handle_t *verification_handle,
     libsystem_character_t * const * filenames,
     int amount_of_filenames,
     liberror_error_t **error );

ssize_t verification_handle_prepare_read_buffer(
         verification_handle_t *verification_handle,
         libsmio_buffer_t *storage_media_buffer,
         liberror_error_t **error );

ssize_t verification_handle_read_buffer(
         verification_handle_t *verification_handle,
         libsmio_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error );

int verification_handle_update_integrity_hash(
     verification_handle_t *verification_handle,
     libsmio_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error );

int verification_handle_close(
     verification_handle_t *verification_handle,
     liberror_error_t **error );

int verification_handle_get_values(
     verification_handle_t *verification_handle,
     size64_t *media_size,
     size_t *block_size,
     liberror_error_t **error );

int verification_handle_get_amount_of_read_errors(
     verification_handle_t *verification_handle,
     int *amount_of_errors,
     liberror_error_t **error );

int verification_handle_get_integrity_hash_value(
     verification_handle_t *verification_handle,
     char *integrity_hash_value_identifier,
     size_t integrity_hash_value_identifier_length,
     libsystem_character_t *integrity_hash_value,
     size_t integrity_hash_value_size,
     liberror_error_t **error );

int verification_handle_set_input_values(
     verification_handle_t *verification_handle,
     int wipe_block_on_error,
     liberror_error_t **error );

int verification_handle_finalize(
     verification_handle_t *verification_handle,
     libsystem_character_t *calculated_md5_hash_string,
     size_t calculated_md5_hash_string_size,
     libsystem_character_t *stored_md5_hash_string,
     size_t stored_md5_hash_string_size,
     int *stored_md5_hash_available,
     libsystem_character_t *calculated_sha1_hash_string,
     size_t calculated_sha1_hash_string_size,
     libsystem_character_t *stored_sha1_hash_string,
     size_t stored_sha1_hash_string_size,
     int *stored_sha1_hash_available,
     liberror_error_t **error );

int verification_handle_additional_integrity_hash_values_fprint(
     verification_handle_t *verification_handle,
     FILE *stream,
     liberror_error_t **error );

int verification_handle_read_errors_fprint(
     verification_handle_t *verification_handle,
     FILE *stream,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

