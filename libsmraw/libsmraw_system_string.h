/*
 * System character type string functions
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

#if !defined( _LIBSMRAW_SYSTEM_STRING_H )
#define _LIBSMRAW_SYSTEM_STRING_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libsmraw_libuna.h"

#if defined( _cplusplus )
extern "C" {
#endif

extern int libsmraw_system_narrow_string_codepage;

/* Detect if the code is being compiled with Windows Unicode support
 */
#if defined( WINAPI ) && ( defined( _UNICODE ) || defined( UNICODE ) )
#define LIBSMRAW_HAVE_WIDE_SYSTEM_CHARACTER		1
#endif

/* The system string type is either UTF-16 or UTF-32
 */
#if defined( LIBSMRAW_HAVE_WIDE_SYSTEM_CHARACTER )

typedef wchar_t libsmraw_system_character_t;

#define PRIc_LIBSMRAW_SYSTEM	"lc"
#define PRIs_LIBSMRAW_SYSTEM	"ls"

/* Intermediate version of the macro required
 * for correct evaluation predefined string
 */
#define _LIBSMRAW_SYSTEM_STRING_INTERMEDIATE( string ) \
	L ## string

#define _LIBSMRAW_SYSTEM_STRING( string ) \
	_LIBSMRAW_SYSTEM_STRING_INTERMEDIATE( string )

#define libsmraw_system_string_copy( destination, source, size ) \
	wide_string_copy( destination, source, size )

#define libsmraw_system_string_length( string ) \
	wide_string_length( string )

#define libsmraw_system_string_snprintf( target, size, ... ) \
	wide_string_swprintf( target, size, __VA_ARGS__ )

/* The system string type is either UTF-8 or extended ASCII with a codepage
 */
#else

typedef char libsmraw_system_character_t;

#define PRIc_LIBSMRAW_SYSTEM	"c"
#define PRIs_LIBSMRAW_SYSTEM	"s"

#define _LIBSMRAW_SYSTEM_STRING( string ) \
	string

#define libsmraw_system_string_copy( destination, source, size ) \
	narrow_string_copy( destination, source, size )

#define libsmraw_system_string_length( string ) \
	narrow_string_length( string )

#define libsmraw_system_string_snprintf( target, size, ... ) \
	narrow_string_snprintf( target, size, __VA_ARGS__ )

#endif

#if defined( _cplusplus )
}
#endif

#endif

