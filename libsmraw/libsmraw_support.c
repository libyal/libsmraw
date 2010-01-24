/*
 * libsmraw support functions
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

#include <common.h>
#include <narrow_string.h>
#include <memory.h>
#include <wide_string.h>
#include <types.h>

#include <liberror.h>

#include "libsmraw_definitions.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_support.h"

/* Returns the library version
 */
const char *libsmraw_get_version(
             void )
{
	return( (const char *) LIBSMRAW_VERSION_STRING );
}

/* Globs the files according to common (split) RAW naming schemas
 * Returns 1 if successful or -1 on error
 */
int libsmraw_glob(
     const char *filename,
     size_t filename_length,
     char **filenames[],
     int *amount_of_filenames,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	void *reallocation               = NULL;
	char *segment_filename           = NULL;
	static char *function            = "libsmraw_glob";
	size_t additional_length         = 0;
	size_t segment_filename_length   = 0;
	ssize_t print_count              = 0;
	int result                       = 0;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( amount_of_filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of filenames.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file io handle.",
		 function );

		return( -1 );
	}
	/* Test if the full filename was provided
	 */
	segment_filename_length = filename_length + 1;

	segment_filename = (char *) memory_allocate(
	                             sizeof( char ) * segment_filename_length );

	if( segment_filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment filename.",
		 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	if( narrow_string_copy(
	     segment_filename,
	     filename,
	     filename_length ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment filename.",
		 function );

		memory_free(
		 segment_filename );
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	segment_filename[ filename_length ] = 0;

	if( libbfio_file_set_name(
	     file_io_handle,
	     segment_filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name in file IO handle.",
		 function );

		memory_free(
		 segment_filename );
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	result = libbfio_handle_exists(
	          file_io_handle,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if file: %s exists.",
		 function,
		 segment_filename );

		memory_free(
		 segment_filename );
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	else if( result != 0 )
	{
		reallocation = memory_reallocate(
		                *filenames,
		                ( sizeof( char * ) * 1 ) );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable reallocate filenames.",
			 function );

			memory_free(
			 segment_filename );
			libbfio_handle_free(
			 &file_io_handle,
			 NULL );

			return( -1 );
		}
		*filenames = reallocation;

		( *filenames )[ 0 ] = segment_filename;

		*amount_of_filenames = 1;

		return( 1 );
	}
	/* Test if the filename was provided without the extension (.raw)
	 */
	else if( result == 0 )
	{
		memory_free(
		 segment_filename );

		/* Test if only the basename was provided
		 */
		 if( filename[ filename_length - 4 ] != (char) '.' )
		{
			segment_filename_length = filename_length + 5;

			segment_filename = (char *) memory_allocate(
			                             sizeof( char ) * segment_filename_length );

			if( segment_filename == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create segment filename.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
			print_count = narrow_string_snprintf(
				       segment_filename,
				       segment_filename_length,
				       "%s.raw",
				       filename );

			if( ( print_count < 0 )
			 || ( (size_t) print_count > segment_filename_length ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set segment filename.",
				 function );

				memory_free(
				 segment_filename );
				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
			if( libbfio_file_set_name(
			     file_io_handle,
			     segment_filename,
			     segment_filename_length - 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle.",
				 function );

				memory_free(
				 segment_filename );
				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
			result = libbfio_handle_exists(
			          file_io_handle,
				  error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_GENERIC,
				 "%s: unable to determine if file: %s exists.",
				 function,
				 segment_filename );

				memory_free(
				 segment_filename );
				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
		}
	}
	if( result != 0 )
	{
		reallocation = memory_reallocate(
				*filenames,
				( sizeof( char * ) * 1 ) );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable reallocate filenames.",
			 function );

			memory_free(
			 segment_filename );
			libbfio_handle_free(
			 &file_io_handle,
			 NULL );

			return( -1 );
		}
		*filenames = reallocation;

		( *filenames )[ 0 ] = segment_filename;

		*amount_of_filenames = 1;

		return( 1 );
	}
	/* Test if the filename was provided without the extension (.raw.###)
	 */
	while( *amount_of_filenames < INT_MAX )
	{
		segment_filename_length = filename_length + 9;

		segment_filename = (char *) memory_allocate(
				             sizeof( char ) * segment_filename_length );

		if( segment_filename == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create segment filename.",
			 function );

			libbfio_handle_free(
			 &file_io_handle,
			 NULL );

			return( -1 );
		}
		if( additional_length == 0 )
		{
			if( narrow_string_copy(
			     segment_filename,
			     filename,
			     filename_length ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set segment filename.",
				 function );

				memory_free(
				 segment_filename );
				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
			segment_filename[ filename_length ] = 0;
		}
		else
		{
			print_count = narrow_string_snprintf(
				       segment_filename,
				       segment_filename_length,
				       "%s.raw.%03d",
				       filename,
				       *amount_of_filenames );

			if( ( print_count < 0 )
			 || ( (size_t) print_count > segment_filename_length ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set segment filename.",
				 function );

				memory_free(
				 segment_filename );
				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
		}
		if( libbfio_file_set_name(
		     file_io_handle,
		     segment_filename,
		     segment_filename_length - 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set name in file IO handle.",
			 function );

			memory_free(
			 segment_filename );
			libbfio_handle_free(
			 &file_io_handle,
			 NULL );

			return( -1 );
		}
		result = libbfio_handle_exists(
			  file_io_handle,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to determine if file: %s exists.",
			 function,
			 segment_filename );

			memory_free(
			 segment_filename );
			libbfio_handle_free(
			 &file_io_handle,
			 NULL );

			return( -1 );
		}
		else if( result == 0 )
		{
			memory_free(
			 segment_filename );

			break;
		}
		reallocation = memory_reallocate(
		                *filenames,
		                ( sizeof( char * ) * ( *amount_of_filenames + 1 ) ) );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable reallocate filenames.",
			 function );

			memory_free(
			 segment_filename );
			libbfio_handle_free(
			 &file_io_handle,
			 NULL );

			return( -1 );
		}
		*filenames = reallocation;

		( *filenames )[ *amount_of_filenames ] = segment_filename;

		*amount_of_filenames += 1;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free file io handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Frees the globbed filenames
 * Returns 1 if successful or -1 on error
 */
int libsmraw_glob_free(
     char *filenames[],
     int amount_of_filenames,
     liberror_error_t **error )
{
	static char *function = "libsmraw_glob_free";
	int filename_iterator = 0;

	if( filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( amount_of_filenames < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid amount of filenames value less than zero.",
		 function );

		return( -1 );
	}
	for( filename_iterator = 0;
	     filename_iterator < amount_of_filenames;
	     filename_iterator++ )
	{
		memory_free(
		 filenames[ filename_iterator ] );

	}
	memory_free(
	 filenames );

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Globs the files according to common (split) RAW naming schemas
 * Returns 1 if successful or -1 on error
 */
int libsmraw_glob_wide(
     const wchar_t *filename,
     size_t filename_length,
     wchar_t **filenames[],
     int *amount_of_filenames,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	void *reallocation               = NULL;
	wchar_t *segment_filename        = NULL;
	static char *function            = "libsmraw_glob_wide";
	size_t additional_length         = 0;
	size_t segment_filename_length   = 0;
	ssize_t print_count              = 0;
	int result                       = 0;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( amount_of_filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of filenames.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file io handle.",
		 function );

		return( -1 );
	}
	/* Test if the full filename was provided
	 */
	segment_filename_length = filename_length + 1;

	segment_filename = (wchar_t *) memory_allocate(
	                                sizeof( wchar_t ) * segment_filename_length );

	if( segment_filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment filename.",
		 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	if( wide_string_copy(
	     segment_filename,
	     filename,
	     filename_length ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment filename.",
		 function );

		memory_free(
		 segment_filename );
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	segment_filename[ filename_length ] = 0;

	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     segment_filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name in file IO handle.",
		 function );

		memory_free(
		 segment_filename );
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	result = libbfio_handle_exists(
	          file_io_handle,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if file: %s exists.",
		 function,
		 segment_filename );

		memory_free(
		 segment_filename );
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	else if( result != 0 )
	{
		reallocation = memory_reallocate(
		                *filenames,
		                ( sizeof( wchar_t * ) * 1 ) );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable reallocate filenames.",
			 function );

			memory_free(
			 segment_filename );
			libbfio_handle_free(
			 &file_io_handle,
			 NULL );

			return( -1 );
		}
		*filenames = reallocation;

		( *filenames )[ 0 ] = segment_filename;

		*amount_of_filenames = 1;

		return( 1 );
	}
	/* Test if the filename was provided without the extension (.raw)
	 */
	else if( result == 0 )
	{
		memory_free(
		 segment_filename );

		/* Test if only the basename was provided
		 */
		 if( filename[ filename_length - 4 ] != (wchar_t) '.' )
		{
			segment_filename_length = filename_length + 5;

			segment_filename = (wchar_t *) memory_allocate(
			                                sizeof( wchar_t ) * segment_filename_length );

			if( segment_filename == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create segment filename.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
			print_count = wide_string_snwprintf(
				       segment_filename,
				       segment_filename_length,
				       L"%s.raw",
				       filename );

			if( ( print_count < 0 )
			 || ( (size_t) print_count > segment_filename_length ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set segment filename.",
				 function );

				memory_free(
				 segment_filename );
				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
			if( libbfio_file_set_name_wide(
			     file_io_handle,
			     segment_filename,
			     segment_filename_length - 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle.",
				 function );

				memory_free(
				 segment_filename );
				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
			result = libbfio_handle_exists(
			          file_io_handle,
				  error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_GENERIC,
				 "%s: unable to determine if file: %s exists.",
				 function,
				 segment_filename );

				memory_free(
				 segment_filename );
				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
		}
	}
	if( result != 0 )
	{
		reallocation = memory_reallocate(
				*filenames,
				( sizeof( wchar_t * ) * 1 ) );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable reallocate filenames.",
			 function );

			memory_free(
			 segment_filename );
			libbfio_handle_free(
			 &file_io_handle,
			 NULL );

			return( -1 );
		}
		*filenames = reallocation;

		( *filenames )[ 0 ] = segment_filename;

		*amount_of_filenames = 1;

		return( 1 );
	}
	/* Test if the filename was provided without the extension (.raw.###)
	 */
	while( *amount_of_filenames < INT_MAX )
	{
		segment_filename_length = filename_length + 9;

		segment_filename = (wchar_t *) memory_allocate(
				                sizeof( wchar_t ) * segment_filename_length );

		if( segment_filename == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create segment filename.",
			 function );

			libbfio_handle_free(
			 &file_io_handle,
			 NULL );

			return( -1 );
		}
		if( additional_length == 0 )
		{
			if( wide_string_copy(
			     segment_filename,
			     filename,
			     filename_length ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set segment filename.",
				 function );

				memory_free(
				 segment_filename );
				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
			segment_filename[ filename_length ] = 0;
		}
		else
		{
			print_count = wide_string_snwprintf(
				       segment_filename,
				       segment_filename_length,
				       L"%s.raw.%03d",
				       filename,
				       *amount_of_filenames );

			if( ( print_count < 0 )
			 || ( (size_t) print_count > segment_filename_length ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set segment filename.",
				 function );

				memory_free(
				 segment_filename );
				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
		}
		if( libbfio_file_set_name_wide(
		     file_io_handle,
		     segment_filename,
		     segment_filename_length - 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set name in file IO handle.",
			 function );

			memory_free(
			 segment_filename );
			libbfio_handle_free(
			 &file_io_handle,
			 NULL );

			return( -1 );
		}
		result = libbfio_handle_exists(
			  file_io_handle,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to determine if file: %s exists.",
			 function,
			 segment_filename );

			memory_free(
			 segment_filename );
			libbfio_handle_free(
			 &file_io_handle,
			 NULL );

			return( -1 );
		}
		else if( result == 0 )
		{
			memory_free(
			 segment_filename );

			break;
		}
		reallocation = memory_reallocate(
		                *filenames,
		                ( sizeof( wchar_t * ) * ( *amount_of_filenames + 1 ) ) );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable reallocate filenames.",
			 function );

			memory_free(
			 segment_filename );
			libbfio_handle_free(
			 &file_io_handle,
			 NULL );

			return( -1 );
		}
		*filenames = reallocation;

		( *filenames )[ *amount_of_filenames ] = segment_filename;

		*amount_of_filenames += 1;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free file io handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Frees the globbed filenames
 * Returns 1 if successful or -1 on error
 */
int libsmraw_glob_free_wide(
     wchar_t *filenames[],
     int amount_of_filenames,
     liberror_error_t **error )
{
	static char *function = "libsmraw_glob_free";
	int filename_iterator = 0;

	if( filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( amount_of_filenames < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid amount of filenames value less than zero.",
		 function );

		return( -1 );
	}
	for( filename_iterator = 0;
	     filename_iterator < amount_of_filenames;
	     filename_iterator++ )
	{
		memory_free(
		 filenames[ filename_iterator ] );

	}
	memory_free(
	 filenames );

	return( 1 );
}

#endif

