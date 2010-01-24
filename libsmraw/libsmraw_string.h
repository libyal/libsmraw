/*
 * Character type string functions
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

#if !defined( _LIBSMRAW_STRING_H )
#define _LIBSMRAW_STRING_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>

#include "libsmraw_libuna.h"

#if defined( _cplusplus )
extern "C" {
#endif

/* The internal string type contains UTF-8
 */
typedef libuna_utf8_character_t libsmraw_character_t;

#define PRIc_LIBSMRAW	"c"
#define PRIs_LIBSMRAW	"s"

#define _LIBSMRAW_STRING( string ) \
	(libsmraw_character_t *) string

#if defined( _cplusplus )
}
#endif

#endif

