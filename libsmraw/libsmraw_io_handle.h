/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBSMRAW_IO_HANDLE_H )
#define _LIBSMRAW_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libsmraw_libbfio.h"
#include "libsmraw_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t *smraw_file_signature;

typedef struct libsmraw_io_handle libsmraw_io_handle_t;

struct libsmraw_io_handle
{
        /* The basename
	 */
        system_character_t *basename;

        /* The basename size
	 */
        size_t basename_size;

	/* The media size
	 */
        size64_t media_size;

	/* The maximum segment size
	 */
        size64_t maximum_segment_size;

	/* The number of segments, used for filename creation during write
	 */
	int number_of_segments;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libsmraw_io_handle_initialize(
     libsmraw_io_handle_t **io_handle,
     libcerror_error_t **error );

int libsmraw_io_handle_free(
     libsmraw_io_handle_t **io_handle,
     libcerror_error_t **error );

int libsmraw_io_handle_clear(
     libsmraw_io_handle_t *io_handle,
     libcerror_error_t **error );

int libsmraw_io_handle_create_segment(
     libsmraw_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     int segment_index,
     int *segment_file_index,
     off64_t *segment_offset,
     size64_t *segment_size,
     uint32_t *segment_flags,
     libcerror_error_t **error );

ssize_t libsmraw_io_handle_read_segment_data(
         libsmraw_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error );

ssize_t libsmraw_io_handle_write_segment_data(
         libsmraw_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int segment_index,
         int segment_file_index,
         const uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t write_flags,
         libcerror_error_t **error );

off64_t libsmraw_io_handle_seek_segment_offset(
         libsmraw_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int segment_index,
         int segment_file_index,
         off64_t segment_offset,
         libcerror_error_t **error );

int libsmraw_io_handle_get_basename_size(
     libsmraw_io_handle_t *io_handle,
     size_t *basename_size,
     libcerror_error_t **error );

int libsmraw_io_handle_get_basename(
     libsmraw_io_handle_t *io_handle,
     char *basename,
     size_t basename_size,
     libcerror_error_t **error );

int libsmraw_io_handle_set_basename(
     libsmraw_io_handle_t *io_handle,
     const char *basename,
     size_t basename_length,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

int libsmraw_io_handle_get_basename_size_wide(
     libsmraw_io_handle_t *io_handle,
     size_t *basename_size,
     libcerror_error_t **error );

int libsmraw_io_handle_get_basename_wide(
     libsmraw_io_handle_t *io_handle,
     wchar_t *basename,
     size_t basename_size,
     libcerror_error_t **error );

int libsmraw_io_handle_set_basename_wide(
     libsmraw_io_handle_t *io_handle,
     const wchar_t *basename,
     size_t basename_length,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

int libsmraw_io_handle_get_maximum_segment_size(
     libsmraw_io_handle_t *io_handle,
     size64_t *maximum_segment_size,
     libcerror_error_t **error );

int libsmraw_io_handle_set_maximum_segment_size(
     libsmraw_io_handle_t *io_handle,
     size64_t maximum_segment_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBSMRAW_IO_HANDLE_H ) */

