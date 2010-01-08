/*
 * libsmraw main handle
 *
 * Copyright (c) 2008-2010, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBSMRAW_RAW_H )
#define _LIBSMRAW_RAW_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>

#include "libsmraw_handle.h"
#include "libsmraw_information_file.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_system_string.h"
#include "libsmraw_types.h"
#include "libsmraw_values_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libsmraw_internal_handle libsmraw_internal_handle_t;

struct libsmraw_internal_handle
{
        /* The basename
	 */
        libsmraw_system_character_t *basename;

        /* The length of the basename
	 */
        size_t basename_length;

	/* The current pool entry
	 */
	int current_pool_entry;

	/* The maximum pool entry
	 */
	int maximum_pool_entry;

	/* A pool of file io handles
	 */
	libbfio_pool_t *file_io_pool;

	/* A value to indicate if the information file
	 * needs to be written on close
	 */
	uint8_t write_information_on_close;

	/* The information file
	 */
	libsmraw_information_file_t *information_file;

	/* The media size
	 */
        size64_t media_size;

	/* The current media offset
	 */
	off64_t offset;

	/* The maximum segment size
	 */
	size64_t maximum_segment_size;

	/* The information values table
	 */
	libsmraw_values_table_t *information_values_table;

	/* The integrity hash values table
	 */
	libsmraw_values_table_t *integrity_hash_values_table;
};

LIBSMRAW_EXTERN int libsmraw_handle_initialize(
                     libsmraw_handle_t **handle,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_free(
                     libsmraw_handle_t **handle,
                     liberror_error_t **error );

int libsmraw_internal_handle_free(
     intptr_t *io_handle,
     liberror_error_t **error );

int libsmraw_internal_handle_set_basename(
     libsmraw_internal_handle_t *raw_io_handle,
     libsmraw_system_character_t *basename,
     size_t basename_length,
     liberror_error_t **error );

int libsmraw_internal_handle_create_segment_filename(
     libsmraw_internal_handle_t *raw_io_handle,
     libsmraw_system_character_t **segment_filename,
     size_t *segment_filename_size,
     liberror_error_t **error );

int libsmraw_internal_handle_initialize_write_values(
     libsmraw_internal_handle_t *raw_io_handle,
     liberror_error_t **error );

int libsmraw_handle_glob(
     intptr_t *io_handle,
     const char *filename,
     size_t filename_length,
     char **filenames[],
     int *amount_of_filenames,
     liberror_error_t **error );

int libsmraw_handle_open(
     intptr_t *io_handle,
     char * const filenames[],
     int amount_of_filenames,
     int flags,
     liberror_error_t **error );

int libsmraw_handle_close(
     intptr_t *io_handle,
     liberror_error_t **error );

ssize_t libsmraw_handle_prepare_read_buffer(
         intptr_t *io_handle,
         intptr_t *buffer,
         liberror_error_t **error );

ssize_t libsmraw_handle_read_buffer(
         intptr_t *io_handle,
         intptr_t *buffer,
         size_t read_size,
         liberror_error_t **error );

ssize_t libsmraw_handle_prepare_write_buffer(
         intptr_t *io_handle,
         intptr_t *buffer,
         liberror_error_t **error );

ssize_t libsmraw_handle_write_buffer(
         intptr_t *io_handle,
         intptr_t *buffer,
         size_t write_size,
         liberror_error_t **error );

off64_t libsmraw_handle_seek_offset(
         intptr_t *io_handle,
         off64_t offset,
         int whence,
         liberror_error_t **error );

int libsmraw_handle_get_media_size(
     intptr_t *io_handle,
     size64_t *media_size,
     liberror_error_t **error );

int libsmraw_handle_set_media_size(
     intptr_t *io_handle,
     size64_t media_size,
     liberror_error_t **error );

int libsmraw_handle_get_bytes_per_sector(
     intptr_t *io_handle,
     size_t *bytes_per_sector,
     liberror_error_t **error );

int libsmraw_handle_set_bytes_per_sector(
     intptr_t *io_handle,
     size_t bytes_per_sector,
     liberror_error_t **error );

int libsmraw_handle_get_block_size(
     intptr_t *io_handle,
     size_t *block_size,
     liberror_error_t **error );

int libsmraw_handle_set_block_size(
     intptr_t *io_handle,
     size_t block_size,
     liberror_error_t **error );

int libsmraw_handle_set_maximum_segment_size(
     intptr_t *io_handle,
     size64_t maximum_segment_size,
     int maximum_possible,
     liberror_error_t **error );

int libsmraw_handle_get_compression_values(
     intptr_t *io_handle,
     int *type,
     int *level,
     int *flags,
     liberror_error_t **error );

int libsmraw_handle_set_compression_values(
     intptr_t *io_handle,
     int type,
     int level,
     int flags,
     liberror_error_t **error );

int libsmraw_handle_get_media_values(
     intptr_t *io_handle,
     int *media_type,
     int *volume_type,
     liberror_error_t **error );

int libsmraw_handle_set_media_values(
     intptr_t *io_handle,
     int media_type,
     int volume_type,
     liberror_error_t **error );


int libsmraw_handle_get_filename_size(
     intptr_t *io_handle,
     size_t *filename_size,
     liberror_error_t **error );

int libsmraw_handle_get_filename(
     intptr_t *io_handle,
     char *filename,
     size_t filename_size,
     liberror_error_t **error );

int libsmraw_handle_get_amount_of_information_values(
     intptr_t *io_handle,
     int *amount_of_information_values,
     liberror_error_t **error );

int libsmraw_handle_get_information_value_identifier_size(
     intptr_t *io_handle,
     int information_value_index,
     size_t *identifier_size,
     liberror_error_t **error );

int libsmraw_handle_get_information_value_identifier(
     intptr_t *io_handle,
     int information_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     liberror_error_t **error );

int libsmraw_handle_get_information_value_size(
     intptr_t *io_handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     liberror_error_t **error );

int libsmraw_handle_get_information_value(
     intptr_t *io_handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     liberror_error_t **error );

int libsmraw_handle_set_information_value(
     intptr_t *io_handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     liberror_error_t **error );

int libsmraw_handle_get_amount_of_integrity_hash_values(
     intptr_t *io_handle,
     int *amount_of_integrity_hash_values,
     liberror_error_t **error );

int libsmraw_handle_get_integrity_hash_value_identifier_size(
     intptr_t *io_handle,
     int integrity_hash_value_index,
     size_t *identifier_size,
     liberror_error_t **error );

int libsmraw_handle_get_integrity_hash_value_identifier(
     intptr_t *io_handle,
     int integrity_hash_value_index,
     uint8_t *identifier,
     size_t identifier_length,
     liberror_error_t **error );

int libsmraw_handle_get_integrity_hash_value_size(
     intptr_t *io_handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     liberror_error_t **error );

int libsmraw_handle_get_integrity_hash_value(
     intptr_t *io_handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     liberror_error_t **error );

int libsmraw_handle_set_integrity_hash_value(
     intptr_t *io_handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

