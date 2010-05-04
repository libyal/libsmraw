/*
 * Filename functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>
#include <libnotify.h>

#include "libsmraw_definitions.h"
#include "libsmraw_handle.h"
#include "libsmraw_information_file.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_handle.h"
#include "libsmraw_types.h"

/* Creates a (split) RAW filename
 * Returns 1 if successful or -1 on error
 */
int libsmraw_filename_create(
     libcstring_system_character_t **filename,
     size_t *filename_size,
     libcstring_system_character_t *basename,
     size_t basename_size,
     int total_number_of_file_io_pool_entries,
     int current_file_io_pool_entry,
     liberror_error_t **error )
{
	static char *function    = "filename_create";
	size_t additional_length = 4;
	int print_count          = 0;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment filename.",
		 function );

		return( -1 );
	}
	if( *filename != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid segment filename already set.",
		 function );

		return( -1 );
	}
	if( filename_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment filename size.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( basename_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid basename size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( total_number_of_file_io_pool_entries <= 0 )
	 || ( total_number_of_file_io_pool_entries >= 1000 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid maximum pool entry value out of range.",
		 function );

		return( -1 );
	}
	if( ( current_file_io_pool_entry < 0 )
	 || ( current_file_io_pool_entry > total_number_of_file_io_pool_entries ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid current pool entry value out of range.",
		 function );

		return( -1 );
	}
	if( total_number_of_file_io_pool_entries > 1 )
	{
		additional_length = 8;
	}
	*filename_size = basename_size + additional_length;

	*filename = (libcstring_system_character_t *) memory_allocate(
	                                               sizeof( libcstring_system_character_t ) * *filename_size );

	if( *filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to allocate segment filename.",
		 function );

		return( -1 );
	}
	if( total_number_of_file_io_pool_entries == 1 )
	{
		print_count = libcstring_system_string_sprintf(
		               *filename,
		               *filename_size,
		               _LIBCSTRING_SYSTEM_STRING( "%" ) _LIBCSTRING_SYSTEM_STRING( PRIs_LIBCSTRING_SYSTEM ) _LIBCSTRING_SYSTEM_STRING( ".raw" ),
		               basename );
	}
	else
	{
		print_count = libcstring_system_string_sprintf(
		               *filename,
		               *filename_size,
		               _LIBCSTRING_SYSTEM_STRING( "%" ) _LIBCSTRING_SYSTEM_STRING( PRIs_LIBCSTRING_SYSTEM ) _LIBCSTRING_SYSTEM_STRING( ".raw.%03d" ),
		               basename,
		               current_file_io_pool_entry );
	}
	if( ( print_count < 0 )
	 && ( (size_t) print_count > *filename_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment filename.",
		 function );

		memory_free(
		 *filename );

		*filename = NULL;

		return( -1 );
	}
	return( 1 );
}

