/*
 * User input functions for the ewftools
 *
 * Copyright (c) 2006-2010, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _SMIOINPUT_H )
#define _SMIOINPUT_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <libsystem.h>

#if defined( __cplusplus )
extern "C" {
#endif

#define SMIOINPUT_COMPRESSION_LEVELS_AMOUNT		4
#define SMIOINPUT_COMPRESSION_LEVELS_DEFAULT		0

#define SMIOINPUT_EWF_FORMAT_TYPES_AMOUNT		12
#define SMIOINPUT_EWF_FORMAT_TYPES_DEFAULT		8

#define SMIOINPUT_MEDIA_TYPES_AMOUNT			4
#define SMIOINPUT_MEDIA_TYPES_DEFAULT			0

#define SMIOINPUT_VOLUME_TYPES_AMOUNT			2
#define SMIOINPUT_VOLUME_TYPES_DEFAULT			1

#define SMIOINPUT_SECTOR_PER_BLOCK_SIZES_AMOUNT		10
#define SMIOINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT	0

#if defined( LIBSMIO_AFF_SUPPORT )
#define SMIOINPUT_STORAGE_MEDIA_TYPES_AFF_CORRECTION	1
#else
#define SMIOINPUT_STORAGE_MEDIA_TYPES_AFF_CORRECTION	0
#endif

#define SMIOINPUT_STORAGE_MEDIA_TYPES_AMOUNT		2 \
							+ SMIOINPUT_STORAGE_MEDIA_TYPES_AFF_CORRECTION
#define SMIOINPUT_STORAGE_MEDIA_TYPES_DEFAULT		1 \
							+ SMIOINPUT_STORAGE_MEDIA_TYPES_AFF_CORRECTION

extern libsystem_character_t *smioinput_compression_levels[ 4 ];
extern libsystem_character_t *smioinput_ewf_format_types[ 12 ];
extern libsystem_character_t *smioinput_media_types[ 4 ];
extern libsystem_character_t *smioinput_volume_types[ 2 ];
extern libsystem_character_t *smioinput_sector_per_block_sizes[ 10 ];
extern libsystem_character_t *smioinput_yes_no[ 2 ];

extern libsystem_character_t *smioinput_storage_media_types[];

int smioinput_determine_storage_media_type(
     const libsystem_character_t *argument,
     int *storage_media_type,
     liberror_error_t **error );

int smioinput_determine_ewf_format(
     const libsystem_character_t *argument,
     int *ewf_format,
     liberror_error_t **error );

int smioinput_determine_sectors_per_block(
     const libsystem_character_t *argument,
     uint32_t *sectors_per_block,
     liberror_error_t **error );

int smioinput_determine_compression_level(
     const libsystem_character_t *argument,
     int *compression_level,
     int *compression_flags,
     liberror_error_t **error );

int smioinput_determine_media_type(
     const libsystem_character_t *argument,
     uint8_t *media_type,
     liberror_error_t **error );

int smioinput_determine_volume_type(
     const libsystem_character_t *argument,
     uint8_t *volume_type,
     liberror_error_t **error );

#ifdef TODO
int smioinput_determine_header_codepage(
     const libsystem_character_t *argument,
     int *header_codepage,
     liberror_error_t **error );
#endif

int smioinput_determine_yes_no(
     const libsystem_character_t *argument,
     uint8_t *yes_no_value,
     liberror_error_t **error );

int smioinput_get_string_variable(
     FILE *stream,
     libsystem_character_t *request_string,
     libsystem_character_t *string_variable,
     size_t string_variable_size,
     liberror_error_t **error );

int smioinput_get_size_variable(
     FILE *stream,
     libsystem_character_t *input_buffer,
     size_t input_buffer_size,
     libsystem_character_t *request_string,
     uint64_t minimum,
     uint64_t maximum,
     uint64_t default_value,
     uint64_t *size_variable,
     liberror_error_t **error );

int smioinput_get_byte_size_variable(
     FILE *stream,
     libsystem_character_t *input_buffer,
     size_t input_buffer_size,
     libsystem_character_t *request_string,
     uint64_t minimum,
     uint64_t maximum,
     uint64_t default_value,
     uint64_t *byte_size_variable,
     liberror_error_t **error );

int smioinput_get_fixed_string_variable(
     FILE *stream,
     libsystem_character_t *input_buffer,
     size_t input_buffer_size,
     libsystem_character_t *request_string,
     libsystem_character_t **values,
     uint8_t amount_of_values,
     uint8_t default_value,
     libsystem_character_t **fixed_string_variable,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

