/*
 * Filename functions
 *
 * Copyright (C) 2010-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBSMRAW_FILENAME_H )
#define _LIBSMRAW_FILENAME_H

#include <common.h>
#include <types.h>

#include "libsmraw_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libsmraw_filename_create(
     system_character_t **filename,
     size_t *filename_size,
     system_character_t *basename,
     size_t basename_size,
     int number_of_segments,
     int segment_index,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBSMRAW_FILENAME_H ) */

