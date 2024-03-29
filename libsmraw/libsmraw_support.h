/*
 * Support functions
 *
 * Copyright (C) 2010-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBSMRAW_SUPPORT_H )
#define _LIBSMRAW_SUPPORT_H

#include <common.h>
#include <types.h>

#include "libsmraw_extern.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBSMRAW )

LIBSMRAW_EXTERN \
const char *libsmraw_get_version(
             void );

LIBSMRAW_EXTERN \
int libsmraw_get_access_flags_read(
     void );

LIBSMRAW_EXTERN \
int libsmraw_get_access_flags_read_write(
     void );

LIBSMRAW_EXTERN \
int libsmraw_get_access_flags_write(
     void );

LIBSMRAW_EXTERN \
int libsmraw_get_codepage(
     int *codepage,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_set_codepage(
     int codepage,
     libcerror_error_t **error );

#endif /* !defined( HAVE_LOCAL_LIBSMRAW ) */

int libsmraw_glob_append_segment_file(
     char **filenames[],
     int *number_of_filenames,
     const char *segment_filename,
     libcerror_error_t **error );

int libsmraw_glob_exists_segment_file(
     libbfio_handle_t *file_io_handle,
     const char *prefix,
     size_t prefix_length,
     const char *suffix,
     size_t suffix_length,
     char **segment_filename,
     size_t *segment_filename_size,
     libcerror_error_t **error );

int libsmraw_glob_determine_naming_schema(
     const char *suffix,
     size_t suffix_length,
     uint8_t *naming_schema,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_glob(
     const char *filename,
     size_t filename_length,
     char **filenames[],
     int *number_of_filenames,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_glob_free(
     char *filenames[],
     int number_of_filenames,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
int libsmraw_glob_wide_append_segment_file(
     wchar_t **filenames[],
     int *number_of_filenames,
     const wchar_t *segment_filename,
     libcerror_error_t **error );

int libsmraw_glob_wide_exists_segment_file(
     libbfio_handle_t *file_io_handle,
     const wchar_t *prefix,
     size_t prefix_length,
     const wchar_t *suffix,
     size_t suffix_length,
     wchar_t **segment_filename,
     size_t *segment_filename_size,
     libcerror_error_t **error );

int libsmraw_glob_wide_determine_naming_schema(
     const wchar_t *suffix,
     size_t suffix_length,
     uint8_t *naming_schema,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_glob_wide(
     const wchar_t *filename,
     size_t filename_length,
     wchar_t **filenames[],
     int *number_of_filenames,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_glob_wide_free(
     wchar_t *filenames[],
     int number_of_filenames,
     libcerror_error_t **error );

#endif

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBSMRAW_SUPPORT_H ) */

