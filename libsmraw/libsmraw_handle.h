/*
 * Handle functions
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

#if !defined( _LIBSMRAW_HANDLE_H )
#define _LIBSMRAW_HANDLE_H

#include <common.h>
#include <types.h>

#include "libsmraw_extern.h"
#include "libsmraw_information_file.h"
#include "libsmraw_io_handle.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_libcerror.h"
#include "libsmraw_libcthreads.h"
#include "libsmraw_libfdata.h"
#include "libsmraw_libfvalue.h"
#include "libsmraw_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libsmraw_internal_handle libsmraw_internal_handle_t;

struct libsmraw_internal_handle
{
	/* The IO handle
	 */
	libsmraw_io_handle_t *io_handle;

	/* The segments (file data) stream
	 */
	libfdata_stream_t *segments_stream;

	/* The pool of file IO handles
	 */
	libbfio_pool_t *file_io_pool;

	/* Value to indicate if the file IO pool was created inside the library
	 */
	uint8_t file_io_pool_created_in_library;

	/* The maximum number of open handles in the pool
	 */
	int maximum_number_of_open_handles;

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

	/* The media values table
	 */
	libfvalue_table_t *media_values;

	/* The information values table
	 */
	libfvalue_table_t *information_values;

	/* The integrity hash values table
	 */
	libfvalue_table_t *integrity_hash_values;

#if defined( HAVE_LIBSMRAW_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

LIBSMRAW_EXTERN \
int libsmraw_handle_initialize(
     libsmraw_handle_t **handle,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_free(
     libsmraw_handle_t **handle,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_signal_abort(
     libsmraw_handle_t *handle,
     libcerror_error_t **error );

int libsmraw_internal_handle_initialize_write_values(
     libsmraw_internal_handle_t *raw_io_handle,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_open(
     libsmraw_handle_t *handle,
     char * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBSMRAW_EXTERN \
int libsmraw_handle_open_wide(
     libsmraw_handle_t *handle,
     wchar_t * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBSMRAW_EXTERN \
int libsmraw_handle_open_file_io_pool(
     libsmraw_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libcerror_error_t **error );

int libsmraw_handle_read_information_file(
     libsmraw_internal_handle_t *internal_handle,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_close(
     libsmraw_handle_t *handle,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
ssize_t libsmraw_handle_read_buffer(
         libsmraw_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBSMRAW_EXTERN \
ssize_t libsmraw_handle_read_buffer_at_offset(
         libsmraw_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBSMRAW_EXTERN \
ssize_t libsmraw_handle_write_buffer(
         libsmraw_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBSMRAW_EXTERN \
ssize_t libsmraw_handle_write_buffer_at_offset(
         libsmraw_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBSMRAW_EXTERN \
off64_t libsmraw_handle_seek_offset(
         libsmraw_handle_t *handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_offset(
     libsmraw_handle_t *handle,
     off64_t *offset,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_maximum_number_of_open_handles(
     libsmraw_handle_t *handle,
     int maximum_number_of_open_handles,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_segment_filename_size(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_segment_filename(
     libsmraw_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_segment_filename(
     libsmraw_handle_t *handle,
     const char *filename,
     size_t filename_length,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBSMRAW_EXTERN \
int libsmraw_handle_get_segment_filename_size_wide(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_segment_filename_wide(
     libsmraw_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_segment_filename_wide(
     libsmraw_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBSMRAW_EXTERN \
int libsmraw_handle_get_maximum_segment_size(
     libsmraw_handle_t *handle,
     size64_t *maximum_segment_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_maximum_segment_size(
     libsmraw_handle_t *handle,
     size64_t maximum_segment_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_filename_size(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_filename(
     libsmraw_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBSMRAW_EXTERN \
int libsmraw_handle_get_filename_size_wide(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_filename_wide(
     libsmraw_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBSMRAW_EXTERN \
int libsmraw_handle_get_file_io_handle(
     libsmraw_handle_t *handle,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_media_size(
     libsmraw_handle_t *handle,
     size64_t *media_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_media_size(
     libsmraw_handle_t *handle,
     size64_t media_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_bytes_per_sector(
     libsmraw_handle_t *handle,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_bytes_per_sector(
     libsmraw_handle_t *handle,
     uint32_t bytes_per_sector,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_media_type(
     libsmraw_handle_t *handle,
     uint8_t *media_type,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_media_type(
     libsmraw_handle_t *handle,
     uint8_t media_type,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_media_flags(
     libsmraw_handle_t *handle,
     uint8_t *media_flags,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_media_flags(
     libsmraw_handle_t *handle,
     uint8_t media_flags,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_number_of_information_values(
     libsmraw_handle_t *handle,
     int *number_of_information_values,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_information_value_identifier_size(
     libsmraw_handle_t *handle,
     int information_value_index,
     size_t *identifier_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_information_value_identifier(
     libsmraw_handle_t *handle,
     int information_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf8_information_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf8_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_utf8_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf16_information_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf16_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_utf16_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_number_of_integrity_hash_values(
     libsmraw_handle_t *handle,
     int *number_of_integrity_hash_values,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_integrity_hash_value_identifier_size(
     libsmraw_handle_t *handle,
     int integrity_hash_value_index,
     size_t *identifier_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_integrity_hash_value_identifier(
     libsmraw_handle_t *handle,
     int integrity_hash_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf8_integrity_hash_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf8_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_utf8_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf16_integrity_hash_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf16_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_utf16_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBSMRAW_HANDLE_H ) */

