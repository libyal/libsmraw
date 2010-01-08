/*
 * Common functions for the smiotools
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

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>

#include <errno.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#if defined( HAVE_SYS_UTSNAME_H )
#include <sys/utsname.h>
#endif

/* If libtool DLL support is enabled set LIBSMIO_DLL_IMPORT
 * before including libsmio.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBSMIO_DLL_IMPORT
#endif

#include <libsmio.h>

#include <libsystem.h>

#include "smiocommon.h"

#if !defined( LIBSMIO_OPERATING_SYSTEM )
#define LIBSMIO_OPERATING_SYSTEM "Unknown"
#endif

/* Swaps the byte order of byte pairs within a buffer of a certain size
 * Returns 1 if successful, -1 on error
 */
int smiocommon_swap_byte_pairs(
     uint8_t *buffer,
     size_t size )
{
	static char *function = "smiocommon_swap_byte_pairs";
	uint8_t byte          = 0;
	size_t iterator       = 0;

	if( buffer == NULL )
	{
		libsystem_notify_printf(
		 "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libsystem_notify_printf(
		 "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < size; iterator += 2 )
	{
		byte                   = buffer[ iterator ];
		buffer[ iterator ]     = buffer[ iterator + 1 ];
		buffer[ iterator + 1 ] = byte;
	}
	return( 1 );
}

