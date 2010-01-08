/*
 * libsmraw interface functions
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

#if !defined( _LIBSMRAW_INTERFACE_H )
#define _LIBSMRAW_INTERFACE_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>

#include "libsmraw_extern.h"
#include "libsmraw_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBSMRAW_EXTERN int libsmraw_handle_get_media_size(
                    libsmraw_handle_t *handle,
                    size64_t *media_size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_media_size(
                    libsmraw_handle_t *handle,
                    size64_t media_size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_bytes_per_sector(
                    libsmraw_handle_t *handle,
                    size_t *bytes_per_sector,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_bytes_per_sector(
                    libsmraw_handle_t *handle,
                    size_t bytes_per_sector,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_maximum_segment_size(
                    libsmraw_handle_t *handle,
                    size64_t *maximum_segment_size,
                    int *maximum_possible,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_maximum_segment_size(
                    libsmraw_handle_t *handle,
                    size64_t maximum_segment_size,
                    int maximum_possible,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_block_size(
                    libsmraw_handle_t *handle,
                    size_t *block_size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_block_size(
                    libsmraw_handle_t *handle,
                    size_t block_size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_compression_values(
                    libsmraw_handle_t *handle,
                    int *type,
                    int *level,
                    int *flags,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_compression_values(
                    libsmraw_handle_t *handle,
                    int type,
                    int level,
                    int flags,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_media_values(
                    libsmraw_handle_t *handle,
                    int *media_type,
                    int *volume_type,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_media_values(
                    libsmraw_handle_t *handle,
                    int media_type,
                    int volume_type,
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

LIBSMRAW_EXTERN int libsmraw_handle_get_amount_of_information_values(
                    libsmraw_handle_t *handle,
                    int *amount_of_information_values,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_information_value_identifier_size(
                    libsmraw_handle_t *handle,
                    int information_value_index,
                    size_t *identifier_size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_information_identifier_value(
                    libsmraw_handle_t *handle,
                    int information_value_index,
                    uint8_t *identifier,
                    size_t identifier_size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_information_value_size(
                    libsmraw_handle_t *handle,
                    const uint8_t *identifier,
                    size_t identifier_length,
                    size_t *value_size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_information_value(
                    libsmraw_handle_t *handle,
                    const uint8_t *identifier,
                    size_t identifier_length,
                    uint8_t *value,
                    size_t value_size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_information_value(
                    libsmraw_handle_t *handle,
                    const uint8_t *identifier,
                    size_t identifier_length,
                    const uint8_t *value,
                    size_t value_length,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_amount_of_integrity_hash_values(
                    libsmraw_handle_t *handle,
                    int *amount_of_integrity_hash_values,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_integrity_hash_value_identifier_size(
                    libsmraw_handle_t *handle,
                    int integrity_hash_value_index,
                    size_t *identifier_size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_integrity_hash_identifier_value(
                    libsmraw_handle_t *handle,
                    int integrity_hash_value_index,
                    uint8_t *identifier,
                    size_t identifier_size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_integrity_hash_value_size(
                    libsmraw_handle_t *handle,
                    const uint8_t *identifier,
                    size_t identifier_length,
                    size_t *value_size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_integrity_hash_value(
                    libsmraw_handle_t *handle,
                    const uint8_t *identifier,
                    size_t identifier_length,
                    uint8_t *value,
                    size_t value_size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_integrity_hash_value(
                    libsmraw_handle_t *handle,
                    const uint8_t *identifier,
                    size_t identifier_length,
                    const uint8_t *value,
                    size_t value_length,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_amount_of_read_errors(
                    libsmraw_handle_t *handle,
                    int *amount_of_errors,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_read_error(
                    libsmraw_handle_t *handle,
                    int index,
                    off64_t *offset,
                    size64_t *size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_add_read_error(
                    libsmraw_handle_t *handle,
                    off64_t offset,
                    size64_t size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_amount_of_write_errors(
                    libsmraw_handle_t *handle,
                    int *amount_of_errors,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_write_error(
                    libsmraw_handle_t *handle,
                    int index,
                    off64_t *offset,
                    size64_t *size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_add_write_error(
                    libsmraw_handle_t *handle,
                    off64_t offset,
                    size64_t size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_amount_of_sessions(
                    libsmraw_handle_t *handle,
                    int *amount_of_sessions,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_session(
                    libsmraw_handle_t *handle,
                    int index,
                    off64_t *offset,
                    size64_t *size,
                    liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_add_session(
                    libsmraw_handle_t *handle,
                    off64_t offset,
                    size64_t size,
                    liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

