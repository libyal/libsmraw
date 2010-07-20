/*
 * Meta data functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBSMRAW_METADATA_H )
#define _LIBSMRAW_METADATA_H

#include <common.h>
#include <types.h>

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
                     uint32_t *bytes_per_sector,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_bytes_per_sector(
                     libsmraw_handle_t *handle,
                     uint32_t bytes_per_sector,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_media_type(
                     libsmraw_handle_t *handle,
                     uint8_t *media_type,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_media_type(
                     libsmraw_handle_t *handle,
                     uint8_t media_type,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_media_flags(
                     libsmraw_handle_t *handle,
                     uint8_t *media_flags,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_media_flags(
                     libsmraw_handle_t *handle,
                     uint8_t media_flags,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_number_of_information_values(
                     libsmraw_handle_t *handle,
                     int *number_of_information_values,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_utf8_information_value_identifier_size(
                     libsmraw_handle_t *handle,
                     int information_value_index,
                     size_t *identifier_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_utf8_information_value_identifier(
                     libsmraw_handle_t *handle,
                     int information_value_index,
                     uint8_t *identifier,
                     size_t identifier_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_utf8_information_value_size(
                     libsmraw_handle_t *handle,
                     const uint8_t *identifier,
                     size_t identifier_length,
                     size_t *value_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_utf8_information_value(
                     libsmraw_handle_t *handle,
                     const uint8_t *identifier,
                     size_t identifier_length,
                     uint8_t *value,
                     size_t value_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_utf8_information_value(
                     libsmraw_handle_t *handle,
                     const uint8_t *identifier,
                     size_t identifier_length,
                     const uint8_t *value,
                     size_t value_length,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_number_of_integrity_hash_values(
                     libsmraw_handle_t *handle,
                     int *number_of_integrity_hash_values,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_utf8_integrity_hash_value_identifier_size(
                     libsmraw_handle_t *handle,
                     int integrity_hash_value_index,
                     size_t *identifier_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_utf8_integrity_hash_value_identifier(
                     libsmraw_handle_t *handle,
                     int integrity_hash_value_index,
                     uint8_t *identifier,
                     size_t identifier_length,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_utf8_integrity_hash_value_size(
                     libsmraw_handle_t *handle,
                     const uint8_t *identifier,
                     size_t identifier_length,
                     size_t *value_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_get_utf8_integrity_hash_value(
                     libsmraw_handle_t *handle,
                     const uint8_t *identifier,
                     size_t identifier_length,
                     uint8_t *value,
                     size_t value_size,
                     liberror_error_t **error );

LIBSMRAW_EXTERN int libsmraw_handle_set_utf8_integrity_hash_value(
                     libsmraw_handle_t *handle,
                     const uint8_t *identifier,
                     size_t identifier_length,
                     const uint8_t *value,
                     size_t value_length,
                     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

