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

#if !defined( _LIBSMRAW_INTERNAL_HANDLE_H )
#define _LIBSMRAW_INTERNAL_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libsmraw_extern.h"
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

        /* The size of the basename
	 */
        size_t basename_size;

	/* The current file io pool entry
	 */
	int current_file_io_pool_entry;

	/* The total amount of pool entries
	 */
	int total_amount_of_file_io_pool_entries;

	/* A pool of file io handles
	 */
	libbfio_pool_t *file_io_pool;

	/* Value to indicate if the pool was created inside the library
	 */
	uint8_t file_io_pool_created_in_library;

	/* The maximum amount of open handles in the pool
	 */
	int maximum_amount_of_open_handles;
 
	/* A value to indicate if the read values have been initialized
	 */
	uint8_t read_values_initialized;

	/* A value to indicate if the write values have been initialized
	 */
	uint8_t write_values_initialized;

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

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

LIBSMRAW_EXTERN int libsmraw_handle_initialize(
                     libsmraw_handle_t **handle,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_free(
                     libsmraw_handle_t **handle,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_signal_abort(
                     libsmraw_handle_t *handle,
                     liberror_error_t **error );

int libsmraw_internal_handle_initialize_write_values(
     libsmraw_internal_handle_t *raw_io_handle,
     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_open(
                     libsmraw_handle_t *handle,
                     char * const filenames[],
                     int amount_of_filenames,
                     uint8_t flags,
                     liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBSMRAW_EXTERN int libsmraw_handle_open_wide(
                     libsmraw_handle_t *handle,
                     wchar_t * const filenames[],
                     int amount_of_filenames,
                     uint8_t flags,
                     liberror_error_t **error );
#endif

LIBSMRAW_EXTERN int libsmraw_handle_open_file_io_pool(
                     libsmraw_handle_t *handle,
                     libbfio_pool_t *file_io_pool,
                     uint8_t flags,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_close(
                     libsmraw_handle_t *handle,
                     liberror_error_t **error );

LIBSMRAW_EXTERN ssize_t libsmraw_handle_read_buffer(
                         libsmraw_handle_t *handle,
                         void *buffer,
                         size_t buffer_size,
                         liberror_error_t **error );

LIBSMRAW_EXTERN ssize_t libsmraw_handle_write_buffer(
                         libsmraw_handle_t *handle,
                         void *buffer,
                         size_t buffer_size,
                         liberror_error_t **error );

LIBSMRAW_EXTERN off64_t libsmraw_handle_seek_offset(
                         libsmraw_handle_t *handle,
                         off64_t offset,
                         int whence,
                         liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_offset(
                     libsmraw_handle_t *handle,
                     off64_t *offset,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_maximum_amount_of_open_handles(
                     libsmraw_handle_t *handle,
                     int maximum_amount_of_open_handles,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_segment_filename_size(
                     libsmraw_handle_t *handle,
                     size_t *filename_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_segment_filename(
                     libsmraw_handle_t *handle,
                     char *filename,
                     size_t filename_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_segment_filename(
                     libsmraw_handle_t *handle,
                     const char *filename,
                     size_t filename_length,
                     liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBSMRAW_EXTERN int libsmraw_handle_get_segment_filename_size_wide(
                     libsmraw_handle_t *handle,
                     size_t *filename_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_segment_filename_wide(
                     libsmraw_handle_t *handle,
                     wchar_t *filename,
                     size_t filename_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_segment_filename_wide(
                     libsmraw_handle_t *handle,
                     const wchar_t *filename,
                     size_t filename_length,
                     liberror_error_t **error );
#endif

LIBSMRAW_EXTERN int libsmraw_handle_get_segment_file_size(
                     libsmraw_handle_t *handle,
                     size64_t *segment_file_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_segment_file_size(
                     libsmraw_handle_t *handle,
                     size64_t segment_file_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_filename_size(
                     libsmraw_handle_t *handle,
                     size_t *filename_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_filename(
                     libsmraw_handle_t *handle,
                     char *filename,
                     size_t filename_size,
                     liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBSMRAW_EXTERN int libsmraw_handle_get_filename_size_wide(
                     libsmraw_handle_t *handle,
                     size_t *filename_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_filename_wide(
                     libsmraw_handle_t *handle,
                     wchar_t *filename,
                     size_t filename_size,
                     liberror_error_t **error );
#endif

LIBSMRAW_EXTERN int libsmraw_handle_get_file_io_handle(
                     libsmraw_handle_t *handle,
                     libbfio_handle_t **file_io_handle,
                     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

