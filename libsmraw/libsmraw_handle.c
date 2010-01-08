/*
 * libsmraw main handle
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
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>
#include <libnotify.h>

#include "libsmraw_definitions.h"
#include "libsmraw_handle.h"
#include "libsmraw_information_file.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_handle.h"
#include "libsmraw_system_string.h"
#include "libsmraw_types.h"

/* Initializes the (split) RAW handle
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_initialize(
     libsmraw_handle_t **handle,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_initialize";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle == NULL )
	{
		internal_handle = (libsmraw_internal_handle_t *) memory_allocate(
		                                                  sizeof( libsmraw_internal_handle_t ) );

		if( internal_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create internal handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_handle,
		     0,
		     sizeof( libsmraw_internal_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear IO handle.",
			 function );

			memory_free(
			 internal_handle );

			return( -1 );
		}
		internal_handle->maximum_pool_entry   = 999;
		internal_handle->maximum_segment_size = LIBSMRAW_DEFAULT_MAXIMUM_SEGMENT_SIZE;
	}
	return( 1 );
}

/* Frees the (split) RAW handle
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_free(
     libsmraw_handle_t *handle,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_internal_handle_free";
	int result                                  = 1;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) *handle;

	if( internal_handle->file_io_pool != NULL )
	{
		if( libbfio_pool_free(
		     &( internal_handle->file_io_pool ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file io pool.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->information_file != NULL )
	{
		if( libsmraw_information_file_free(
		     &( internal_handle->information_file ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free information file.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->information_values_table != NULL )
	{
		if( libsmraw_values_table_free(
		     &( internal_handle->information_values_table ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free information values table.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->integrity_hash_values_table != NULL )
	{
		if( libsmraw_values_table_free(
		     &( internal_handle->integrity_hash_values_table ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free integrity hash values table.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->basename != NULL )
	{
		memory_free(
		 internal_handle->basename );
	}
	memory_free(
	 internal_handle );

	return( result );
}

/* Sets the basename
 * Returns 1 if successful or -1 on error
 */
int libsmraw_internal_handle_set_basename(
     libsmraw_internal_handle_t *raw_io_handle,
     libsmraw_system_character_t *basename,
     size_t basename_length,
     liberror_error_t **error )
{
	static char *function = "libsmraw_internal_handle_set_basename";

	if( raw_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RAW IO handle.",
		 function );

		return( -1 );
	}
	if( raw_io_handle->basename != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid RAW IO handle - basename already set.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( basename_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid basename length value exceeds maximum.",
		 function );

		return( -1 );
	}
	raw_io_handle->basename = (libsmraw_system_character_t *) memory_allocate(
	                                                          sizeof( libsmraw_system_character_t ) * basename_length );

	if( raw_io_handle->basename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create basename.",
		function );

		return( -1 );
	}
	if( narrow_string_copy(
	     raw_io_handle->basename,
	     basename,
	     basename_length ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to set basename.",
		function );

		memory_free(
		 raw_io_handle->basename );

		raw_io_handle->basename = NULL;

		return( -1 );
	}
	raw_io_handle->basename[ basename_length ] = 0;
	raw_io_handle->basename_length             = basename_length;

	return( 1 );
}

/* Creates a raw filename
 * Returns 1 if successful or -1 on error
 */
int libsmraw_internal_handle_create_segment_filename(
     libsmraw_internal_handle_t *raw_io_handle,
     libsmraw_system_character_t **segment_filename,
     size_t *segment_filename_size,
     liberror_error_t **error )
{
	static char *function    = "libsmraw_internal_handle_create_segment_filename";
	ssize_t print_count      = 0;
	size_t additional_length = 5;

	if( raw_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RAW IO handle.",
		 function );

		return( -1 );
	}
	if( raw_io_handle->basename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid RAW IO handle - missing basename.",
		 function );

		return( -1 );
	}
	if( ( raw_io_handle->maximum_pool_entry <= 0 )
	 || ( raw_io_handle->maximum_pool_entry >= 1000 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid RAW IO handle - unsupported maximum pool entry.",
		 function );

		return( -1 );
	}
	if( ( raw_io_handle->current_pool_entry < 0 )
	 || ( raw_io_handle->current_pool_entry > raw_io_handle->maximum_pool_entry ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid RAW IO handle - current pool entry value out of range.",
		 function );

		return( -1 );
	}
	if( raw_io_handle->maximum_pool_entry > 1 )
	{
		additional_length = 9;
	}
	if( segment_filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment filename.",
		 function );

		return( -1 );
	}
	if( *segment_filename != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid segment filename already set.",
		 function );

		return( -1 );
	}
	if( segment_filename_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment filename size.",
		 function );

		return( -1 );
	}
	*segment_filename_size = raw_io_handle->basename_length + additional_length;

	*segment_filename = (libsmraw_system_character_t *) memory_allocate(
	                                                    sizeof( libsmraw_system_character_t ) * *segment_filename_size );

	if( *segment_filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to allocate segment filename.",
		 function );

		return( -1 );
	}
	if( raw_io_handle->maximum_pool_entry == 1 )
	{
		print_count = libsmraw_system_string_snprintf(
		               *segment_filename,
		               *segment_filename_size,
		               "%" PRIs_LIBSMRAW_SYSTEM ".raw",
		               raw_io_handle->basename );
	}
	else
	{
		print_count = libsmraw_system_string_snprintf(
		               *segment_filename,
		               *segment_filename_size,
		               "%" PRIs_LIBSMRAW_SYSTEM ".raw.%03d",
		               raw_io_handle->basename,
		               raw_io_handle->current_pool_entry );
	}
	if( ( print_count < 0 )
	 && ( (size_t) print_count > *segment_filename_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment filename.",
		 function );

		memory_free(
		 *segment_filename );

		*segment_filename = NULL;

		return( -1 );
	}
	return( 1 );
}

/* Initializes the write values
 * Returns 1 if successful or -1 on error
 */
int libsmraw_internal_handle_initialize_write_values(
     libsmraw_internal_handle_t *raw_io_handle,
     liberror_error_t **error )
{
	static char *function = "libsmraw_internal_handle_initialize_write_values";

	if( raw_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RAW IO handle.",
		 function );

		return( -1 );
	}
	if( ( raw_io_handle->media_size > 0 )
	 && ( raw_io_handle->maximum_segment_size > 0 ) )
	{
		raw_io_handle->maximum_pool_entry = raw_io_handle->media_size / raw_io_handle->maximum_segment_size;

		if( ( raw_io_handle->media_size % raw_io_handle->maximum_segment_size ) != 0 )
		{
			raw_io_handle->maximum_pool_entry += 1;
		}
	}
	return( 1 );
}

/* Globs the filenames for the RAW handle
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_glob(
     intptr_t *io_handle,
     const char *filename,
     size_t filename_length,
     char **filenames[],
     int *amount_of_filenames,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	void *reallocation               = NULL;
	char *segment_filename           = NULL;
	static char *function            = "libsmraw_handle_glob";
	size_t additional_length         = 0;
	size_t segment_filename_length   = 0;
	ssize_t print_count              = 0;
	int result                       = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
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

/* Opens a RAW handle
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_open(
     intptr_t *io_handle,
     char * const filenames[],
     int amount_of_filenames,
     int flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle       = NULL;
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	char *information_filename             = NULL;
	static char *function                  = "libsmraw_handle_open";
	size64_t file_size                     = 0;
	size_t filename_length                 = 0;
	size_t information_filename_length     = 0;
	ssize_t print_count                    = 0;
	int file_io_flags                      = 0;
	int filename_iterator                  = 0;
	int pool_entry                         = 0;
	int result                             = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->file_io_pool != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle - file IO pool already exists.",
		 function );

		return( -1 );
	}
	if( raw_io_handle->basename != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle - basename already exists.",
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
	if( ( amount_of_filenames == 0 )
	 && ( amount_of_filenames > (int) INT_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid amount of filenames value out of range.",
		 function );

		return( -1 );
	}
	/* Open for read only or read/write
	 */
	if( ( flags & LIBSMRAW_FLAG_READ ) == LIBSMRAW_FLAG_READ )
	{
		if( ( flags & LIBSMRAW_FLAG_WRITE ) == LIBSMRAW_FLAG_WRITE )
		{
			file_io_flags = LIBBFIO_OPEN_READ_WRITE;
		}
		else
		{
			file_io_flags = LIBBFIO_OPEN_READ;
		}
		/* Set the basename
		 */
		filename_length = narrow_string_length(
				   filenames[ 0 ] );

		if( filename_length <= 4 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: missing basename.",
			 function );

			return( -1 );
		}
		if( libsmraw_internal_handle_set_basename(
		     raw_io_handle,
		     filenames[ 0 ],
		     filename_length - 4,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set basename.",
			function );

			return( -1 );
		}
		/* Initialize the file io pool 
		 */
		if( libbfio_pool_initialize(
		     &( raw_io_handle->file_io_pool ),
		     amount_of_filenames,
		     LIBBFIO_POOL_UNLIMITED_AMOUNT_OF_OPEN_HANDLES,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file IO pool.",
			 function );

			return( -1 );
		}
		for( filename_iterator = 0;
		     filename_iterator < amount_of_filenames;
		     filename_iterator++ )
		{
			filename_length = narrow_string_length(
					   filenames[ filename_iterator ] );

			if( filename_length == 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: missing filename: %d.",
				 function,
				 filename_iterator );

				return( -1 );
			}
			file_io_handle = NULL;

			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file IO handle.",
				 function );

				return( -1 );
			}
			if( libbfio_file_set_name(
			     file_io_handle,
			     filenames[ filename_iterator ],
			     filename_length + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
			if( libbfio_pool_add_handle(
			     raw_io_handle->file_io_pool,
			     &pool_entry,
			     file_io_handle,
			     file_io_flags,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append file IO handle to pool.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
			if( libbfio_pool_open(
			     raw_io_handle->file_io_pool,
			     pool_entry,
			     file_io_flags,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open file: %s.",
				 function,
				 filenames[ filename_iterator ] );

				return( -1 );
			}
			if( libbfio_pool_get_size(
			     raw_io_handle->file_io_pool,
			     pool_entry,
			     &file_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve file size.",
				 function );

				return( -1 );
			}
			raw_io_handle->media_size += file_size;
		}
	}
	/* Open for write only
	 */
	else if( ( flags & LIBSMRAW_FLAG_WRITE ) == LIBSMRAW_FLAG_WRITE )
	{
		/* Set the basename
		 */
		filename_length = narrow_string_length(
				   filenames[ 0 ] );

		if( filename_length == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: missing basename.",
			 function );

			return( -1 );
		}
		if( libsmraw_internal_handle_set_basename(
		     raw_io_handle,
		     filenames[ 0 ],
		     filename_length,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set basename.",
			function );

			return( -1 );
		}
		/* Initialize the file io pool 
		 */
		if( libbfio_pool_initialize(
		     &( raw_io_handle->file_io_pool ),
		     0,
		     LIBBFIO_POOL_UNLIMITED_AMOUNT_OF_OPEN_HANDLES,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file IO pool.",
			 function );

			return( -1 );
		}
	}
	/* Open the information file
	 */
	if( raw_io_handle->basename != NULL )
	{
		information_filename_length = raw_io_handle->basename_length + 9;

		information_filename = (libsmraw_system_character_t *) memory_allocate(
		                                                       sizeof( libsmraw_system_character_t ) * ( information_filename_length + 1 ) );

		if( information_filename == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create information filename.",
			 function );

			return( -1 );
		}
		print_count = narrow_string_snprintf(
			       information_filename,
			       information_filename_length + 1,
			       "%s.raw.info",
			       raw_io_handle->basename );

		if( ( print_count < 0 )
		 || ( (size_t) print_count > ( information_filename_length + 1 ) ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set information filename.",
			 function );

			memory_free(
			 information_filename );

			return( -1 );
		}
		if( libsmraw_information_file_initialize(
		     &( raw_io_handle->information_file ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create information file.",
			 function );

			memory_free(
			 information_filename );

			return( -1 );
		}
		if( libsmraw_information_file_set_name(
		     raw_io_handle->information_file,
		     information_filename,
		     narrow_string_length(
		      information_filename ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to set information filename.",
			 function );

			memory_free(
			 information_filename );

			return( -1 );
		}
		if( ( flags & LIBSMRAW_FLAG_READ ) == LIBSMRAW_FLAG_READ )
		{
			file_io_handle = NULL;

			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file IO handle.",
				 function );

				memory_free(
				 information_filename );

				return( -1 );
			}
			if( libbfio_file_set_name(
			     file_io_handle,
			     information_filename,
			     information_filename_length,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );
				memory_free(
				 information_filename );

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
				 "%s: unable to determine if information filename exists.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );
				memory_free(
				 information_filename );

				return( -1 );
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

				memory_free(
				 information_filename );

				return( -1 );
			}
			if( result == 1 )
			{
				if( libsmraw_information_file_open(
				     raw_io_handle->information_file,
				     FILE_STREAM_OPEN_READ,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to open information file.",
					 function );

					memory_free(
					 information_filename );

					return( -1 );
				}
				if( libsmraw_information_file_read_section(
				     raw_io_handle->information_file,
				     (uint8_t *) "information_values",
				     18,
				     raw_io_handle->information_values_table,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read information values from information file.",
					 function );

					return( -1 );
				}
				if( libsmraw_information_file_read_section(
				     raw_io_handle->information_file,
				     (uint8_t *) "integrity_hash_values",
				     21,
				     raw_io_handle->integrity_hash_values_table,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read integrity hash values from information file.",
					 function );

					return( -1 );
				}
				if( libsmraw_information_file_close(
				     raw_io_handle->information_file,
				     error ) != 0 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to close information file.",
					 function );

					memory_free(
					 information_filename );

					return( -1 );
				}
			}
		}
		memory_free(
		 information_filename );

		if( ( flags & LIBSMRAW_FLAG_WRITE ) == LIBSMRAW_FLAG_WRITE )
		{
			raw_io_handle->write_information_on_close = 1;
		}
	}
	return( 1 );
}

/* Closes a RAW handle
 * Returns the 0 if succesful or -1 on error
 */
int libsmraw_handle_close(
     intptr_t *io_handle,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_close";
	int result                             = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->write_information_on_close != 0 )
	{
		if( libsmraw_information_file_open(
		     raw_io_handle->information_file,
		     FILE_STREAM_OPEN_WRITE,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open information file.",
			 function );

			return( -1 );
		}
		if( libsmraw_information_file_write_section(
		     raw_io_handle->information_file,
		     (uint8_t *) "information_values",
		     18,
		     raw_io_handle->information_values_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write information values to information file.",
			 function );

			return( -1 );
		}
		if( libsmraw_information_file_write_section(
		     raw_io_handle->information_file,
		     (uint8_t *) "integrity_hash_values",
		     21,
		     raw_io_handle->integrity_hash_values_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write integrity hash values to information file.",
			 function );

			return( -1 );
		}
		if( libsmraw_information_file_close(
		     raw_io_handle->information_file,
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to close information file.",
			 function );

			return( -1 );
		}
	}
	if( ( raw_io_handle->file_io_pool != NULL )
	 && ( libbfio_pool_close_all(
	       raw_io_handle->file_io_pool,
	       error ) != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_GENERIC,
		 "%s: unable close file IO pool.",
		 function );

		result = -1;
	}
	if( ( raw_io_handle->information_file != NULL )
	 && ( libsmraw_information_file_close(
	       raw_io_handle->information_file,
	       error ) != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close information file.",
		 function );

		result = -1;
	}
	return( result );
}

/* Prepares a buffer after reading it
 * Returns the resulting buffer size or -1 on error
 */
ssize_t libsmraw_handle_prepare_read_buffer(
         intptr_t *io_handle,
         intptr_t *buffer,
         liberror_error_t **error )
{
        libsmraw_internal_buffer_t *internal_buffer = NULL;
	static char *function                      = "libsmraw_handle_prepare_read_buffer";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	internal_buffer = (libsmraw_internal_buffer_t *) buffer;

	if( internal_buffer->raw_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal buffer - missing raw buffer.",
		 function );

		return( -1 );
	}
	return( internal_buffer->raw_buffer_amount );
}

/* Reads a buffer
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libsmraw_handle_read_buffer(
         intptr_t *io_handle,
         intptr_t *buffer,
         size_t read_size,
         liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle     = NULL;
        libsmraw_internal_buffer_t *internal_buffer = NULL;
	static char *function                      = "libsmraw_handle_read_buffer";
	off64_t file_offset                        = 0;
	size64_t file_size                         = 0;
	size_t read_buffer_size                    = 0;
	ssize_t read_count                         = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	if( ( raw_io_handle->current_pool_entry < 0 )
	 || ( raw_io_handle->current_pool_entry > raw_io_handle->maximum_pool_entry ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid RAW IO handle - current pool entry value out of range.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	internal_buffer = (libsmraw_internal_buffer_t *) buffer;

	if( internal_buffer->raw_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal buffer - missing raw buffer.",
		 function );

		return( -1 );
	}
	if( read_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( read_size == 0 )
	{
		return( 0 );
	}
	if( read_size > internal_buffer->raw_buffer_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
		 "%s: invalid size value exceeds raw buffer size.",
		 function );

		return( -1 );
	}
	if( ( (size64_t) read_size + (size64_t) raw_io_handle->offset ) > raw_io_handle->media_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: read beyond media size.",
		 function );

		return( -1 );
	}
	internal_buffer->is_compressed              = 0;
	internal_buffer->data_in_compression_buffer = 0;
	internal_buffer->raw_buffer_amount          = 0;

	while( read_size > 0 )
	{
		if( libbfio_pool_get_size(
		     raw_io_handle->file_io_pool,
		     raw_io_handle->current_pool_entry,
		     &file_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file size from entry: %d.",
			 function,
			 raw_io_handle->current_pool_entry );

			return( -1 );
		}
		if( libbfio_pool_get_offset(
		     raw_io_handle->file_io_pool,
		     raw_io_handle->current_pool_entry,
		     &file_offset,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file offset from entry: %d.",
			 function,
			 raw_io_handle->current_pool_entry );

			return( -1 );
		}
		read_buffer_size = read_size;

		if( ( (size64_t) read_buffer_size + (size64_t) file_offset ) > file_size )
		{
			read_buffer_size = file_size - file_offset;
		}
		if( read_buffer_size == 0 )
		{
			break;
		}
		read_count = libbfio_pool_read(
		              raw_io_handle->file_io_pool,
		              raw_io_handle->current_pool_entry,
		              &( ( internal_buffer->raw_buffer )[ internal_buffer->raw_buffer_amount ] ),
		              read_buffer_size,
		              error );

		if( read_count < 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data from entry: %d.",
			 function,
			 raw_io_handle->current_pool_entry );

			return( -1 );
		}
		raw_io_handle->offset += read_count;

		if( ( (size64_t) file_offset + (size64_t) read_count ) == file_size )
		{
			raw_io_handle->current_pool_entry += 1;
		}
		read_size                          -= read_count;
		internal_buffer->raw_buffer_amount += (size_t) read_count;
	}
	return( (ssize_t) internal_buffer->raw_buffer_amount );
}

/* Prepares a buffer before writing
 * Returns the resulting buffer size or -1 on error
 */
ssize_t libsmraw_handle_prepare_write_buffer(
         intptr_t *io_handle,
         intptr_t *buffer,
         liberror_error_t **error )
{
        libsmraw_internal_buffer_t *internal_buffer = NULL;
	static char *function                      = "libsmraw_handle_prepare_write_buffer";
	ssize_t write_count                        = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	internal_buffer = (libsmraw_internal_buffer_t *) buffer;

	if( internal_buffer->raw_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal buffer - missing raw buffer.",
		 function );

		return( -1 );
	}
	write_count = (ssize_t) internal_buffer->raw_buffer_amount;

	return( write_count );
}

/* Writes a buffer
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libsmraw_handle_write_buffer(
         intptr_t *io_handle,
         intptr_t *buffer,
         size_t write_size,
         liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle             = NULL;
	libsmraw_internal_handle_t *raw_io_handle       = NULL;
        libsmraw_internal_buffer_t *internal_buffer   = NULL;
	libsmraw_system_character_t *segment_filename = NULL;
	uint8_t *data_buffer                         = NULL;
	static char *function                        = "libsmraw_handle_write_buffer";
	off64_t file_offset                          = 0;
	size64_t file_size                           = 0;
	size_t buffer_offset                         = 0;
	size_t segment_filename_size                 = 0;
	size_t write_buffer_size                     = 0;
	ssize_t write_count                          = 0;
	int amount_of_handles                        = 0;
	int pool_entry                               = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	if( ( raw_io_handle->current_pool_entry < 0 )
	 || ( raw_io_handle->current_pool_entry > raw_io_handle->maximum_pool_entry ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid RAW IO handle - current pool entry value out of range.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	internal_buffer = (libsmraw_internal_buffer_t *) buffer;

	if( internal_buffer->raw_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal buffer - missing raw buffer.",
		 function );

		return( -1 );
	}
	if( write_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid write size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( write_size == 0 )
	{
		return( 0 );
	}
	if( internal_buffer->data_in_compression_buffer == 1 )
	{
		data_buffer = internal_buffer->compression_buffer;
	}
	else
	{
		data_buffer = internal_buffer->raw_buffer;
	}
	if( libbfio_pool_get_amount_of_handles(
	     raw_io_handle->file_io_pool,
	     &amount_of_handles,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file size from entry: %d.",
		 function,
		 pool_entry );

		return( -1 );
	}
	while( write_size > 0 )
	{
		if( raw_io_handle->current_pool_entry >= amount_of_handles )
		{
			if( libsmraw_internal_handle_create_segment_filename(
			     raw_io_handle,
			     &segment_filename,
			     &segment_filename_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create segment filename number: %d.",
				 function,
				 raw_io_handle->current_pool_entry );

				return( -1 );
			}
			file_io_handle = NULL;

			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file IO handle.",
				 function );

				return( -1 );
			}
			if( libbfio_file_set_name(
			     file_io_handle,
			     segment_filename,
			     segment_filename_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
			if( libbfio_pool_add_handle(
			     raw_io_handle->file_io_pool,
			     &pool_entry,
			     file_io_handle,
			     LIBBFIO_OPEN_WRITE_TRUNCATE,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append file IO handle to pool.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
			amount_of_handles++;

			if( libbfio_pool_open(
			     raw_io_handle->file_io_pool,
			     pool_entry,
			     LIBBFIO_OPEN_WRITE_TRUNCATE,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open file: %" PRIs_LIBSMRAW_SYSTEM ".",
				 function,
				 segment_filename );

				return( -1 );
			}
		}
		if( libbfio_pool_get_size(
		     raw_io_handle->file_io_pool,
		     raw_io_handle->current_pool_entry,
		     &file_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file size from entry: %d.",
			 function,
			 raw_io_handle->current_pool_entry );

			return( -1 );
		}
		if( libbfio_pool_get_offset(
		     raw_io_handle->file_io_pool,
		     raw_io_handle->current_pool_entry,
		     &file_offset,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file offset from entry: %d.",
			 function,
			 raw_io_handle->current_pool_entry );

			return( -1 );
		}
		write_buffer_size = write_size;

		if( ( raw_io_handle->maximum_segment_size != 0 )
		 && ( ( (size64_t) write_buffer_size + (size64_t) file_offset ) >= raw_io_handle->maximum_segment_size ) )
		{
			write_buffer_size = raw_io_handle->maximum_segment_size - file_offset;
		}
		if( write_buffer_size == 0 )
		{
			break;
		}
		write_count = libbfio_pool_write(
		               raw_io_handle->file_io_pool,
		               raw_io_handle->current_pool_entry,
		               &( data_buffer[ buffer_offset ] ),
		               write_buffer_size,
		               error );

		if( write_count < 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write data to entry: %d.",
			 function,
			 raw_io_handle->current_pool_entry );

			return( -1 );
		}
		raw_io_handle->offset += write_count;

		if( ( raw_io_handle->maximum_segment_size != 0 )
		 && ( ( (size64_t) write_buffer_size + (size64_t) file_offset ) >= raw_io_handle->maximum_segment_size ) )
		{
			raw_io_handle->current_pool_entry += 1;
		}
		write_size    -= write_count;
		buffer_offset += write_count;
	}
	return( (ssize_t) buffer_offset );
}

/* Seeks a certain offset
 * Returns the offset or -1 on error
 */
off64_t libsmraw_handle_seek_offset(
         intptr_t *io_handle,
         off64_t offset,
         int whence,
         liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_seek_offset";
	off64_t file_offset                    = 0;
	size64_t file_size                     = 0;
	int amount_of_handles                  = 0;
	int pool_entry                         = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	if( ( offset < 0 )
	 || ( offset > (off64_t) raw_io_handle->media_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid offset value out of range.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_get_amount_of_handles(
	    raw_io_handle->file_io_pool,
	    &amount_of_handles,
	    error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file size from entry: %d.",
		 function,
		 pool_entry );

		return( -1 );
	}
	file_offset = offset;

	for( pool_entry = 0;
	     pool_entry < amount_of_handles;
	     pool_entry++ )
	{
		if( libbfio_pool_get_size(
		     raw_io_handle->file_io_pool,
		     pool_entry,
		     &file_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file size from entry: %d.",
			 function,
			 pool_entry );

			return( -1 );
		}
		if( file_offset > (off64_t) file_size )
		{
			break;
		}
		file_offset -= (off64_t) file_size;
	}
	if( pool_entry >= amount_of_handles )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid pool entry value out of range.",
		 function );

		return( -1 );
	}
	offset = libbfio_pool_seek_offset(
	          raw_io_handle->file_io_pool,
	          pool_entry,
	          file_offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset: %" PRIu64 " in entry: %d.",
		 function,
		 file_offset,
		 pool_entry );

		return( -1 );
	}
	raw_io_handle->current_pool_entry = pool_entry;
	raw_io_handle->offset             = offset;

	return( offset );
}

/* Retrieves the media size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_media_size(
     intptr_t *io_handle,
     size64_t *media_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_media_size";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( media_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media size.",
		 function );

		return( -1 );
	}
	*media_size = raw_io_handle->media_size;

	return( 1 );
}

/* Sets the media size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_media_size(
     intptr_t *io_handle,
     size64_t media_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_set_media_size";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	/* TODO protect value */
	raw_io_handle->media_size = media_size;

	if( libsmraw_internal_handle_initialize_write_values(
	     raw_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize write values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_bytes_per_sector(
     intptr_t *io_handle,
     size_t *bytes_per_sector,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_bytes_per_sector";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	/* TODO */

	*bytes_per_sector = 0;

	return( 1 );
}

/* Sets the bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_bytes_per_sector(
     intptr_t *io_handle,
     size_t bytes_per_sector,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_set_bytes_per_sector";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( bytes_per_sector > (size_t) UINT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid bytes per sector value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* TODO */

	return( 1 );
}

/* Retrieves the block size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_block_size(
     intptr_t *io_handle,
     size_t *block_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_block_size";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( block_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block size.",
		 function );

		return( -1 );
	}
	/* TODO */

	*block_size = 0;

	return( 1 );
}

/* Sets the block size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_block_size(
     intptr_t *io_handle,
     size_t block_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_set_block_size";
	uint32_t raw_sectors_per_chunk         = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( block_size > (size_t) UINT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid block size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* TODO */

	return( 1 );
}

/* Sets the maximum segment size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_maximum_segment_size(
     intptr_t *io_handle,
     size64_t maximum_segment_size,
     int maximum_possible,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_set_maximum_segment_size";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	/* TODO protect value */
	if( maximum_possible == 0 )
	{
		raw_io_handle->maximum_segment_size = maximum_segment_size;
	}
	else
	{
		raw_io_handle->maximum_segment_size = (size64_t) UINT64_MAX;
	}
	if( libsmraw_internal_handle_initialize_write_values(
	     raw_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize write values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the compression values
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_compression_values(
     intptr_t *io_handle,
     int *type,
     int *level,
     int *flags,
     liberror_error_t **error )
{
	static char *function         = "libsmraw_handle_get_compression_values";
	int8_t raw_compression_level  = 0;
	uint8_t raw_compression_flags = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	if( level == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid level.",
		 function );

		return( -1 );
	}
	if( flags == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid flags.",
		 function );

		return( -1 );
	}
	*type  = LIBSMRAW_COMPRESSION_TYPE_NONE;
	*level = LIBSMRAW_COMPRESSION_LEVEL_NONE;
	*flags = 0;

	return( 1 );
}

/* Sets the compression values
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_compression_values(
     intptr_t *io_handle,
     int type,
     int level,
     int flags,
     liberror_error_t **error )
{
	static char *function = "libsmraw_handle_set_compression_values";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( type != LIBSMRAW_COMPRESSION_TYPE_NONE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression type: 0x%02x.",
		 function,
		 type );

		return( -1 );
	}
	if( level != LIBSMRAW_COMPRESSION_LEVEL_NONE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression level: 0x%02x.",
		 function,
		 level );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the media values
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_media_values(
     intptr_t *io_handle,
     int *media_type,
     int *volume_type,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_media_values";
	uint8_t raw_media_type                 = 0;
	uint8_t raw_media_flags                = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( media_type == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media type.",
		 function );

		return( -1 );
	}
	if( volume_type == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume type.",
		 function );

		return( -1 );
	}
	/* TODO */

	return( 1 );
}

/* Sets the media values
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_media_values(
     intptr_t *io_handle,
     int media_type,
     int volume_type,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_set_media_values";
	uint8_t raw_media_type                 = 0;
	uint8_t raw_media_flags                = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	switch( media_type )
	{
		case LIBSMRAW_MEDIA_TYPE_FIXED:
			break;

		case LIBSMRAW_MEDIA_TYPE_MEMORY:
			break;

		case LIBSMRAW_MEDIA_TYPE_OPTICAL:
			break;

		case LIBSMRAW_MEDIA_TYPE_REMOVABLE:
			break;

		default:
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported media type: 0x%02x.",
			 function,
			 media_type );

			return( -1 );
	}
	switch( volume_type )
	{
		case LIBSMRAW_VOLUME_TYPE_LOGICAL:
			break;

		case LIBSMRAW_VOLUME_TYPE_PHYSICAL:
			break;

		default:
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported volume type: 0x%02x.",
			 function,
			 volume_type );

			return( -1 );
	}
	/* TODO */

	return( 1 );
}

/* Retrieves a filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_filename_size(
     intptr_t *io_handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libbfio_handle_t *file_handle          = NULL;
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_filename_size";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	if( ( raw_io_handle->current_pool_entry < 0 )
	 || ( raw_io_handle->current_pool_entry > raw_io_handle->maximum_pool_entry ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid RAW IO handle - current pool entry value out of range.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_get_handle(
	     raw_io_handle->file_io_pool,
	     raw_io_handle->current_pool_entry,
	     &file_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file handle from entry: %d.",
		 function,
		 raw_io_handle->current_pool_entry );

		return( -1 );
	}
	if( libbfio_file_get_name_size(
	     file_handle,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file handle from entry: %d.",
		 function,
		 raw_io_handle->current_pool_entry );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_filename(
     intptr_t *io_handle,
     char *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libbfio_handle_t *file_handle          = NULL;
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_filename";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	if( ( raw_io_handle->current_pool_entry < 0 )
	 || ( raw_io_handle->current_pool_entry > raw_io_handle->maximum_pool_entry ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid RAW IO handle - current pool entry value out of range.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_get_handle(
	     raw_io_handle->file_io_pool,
	     raw_io_handle->current_pool_entry,
	     &file_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file handle from entry: %d.",
		 function,
		 raw_io_handle->current_pool_entry );

		return( -1 );
	}
	if( libbfio_file_get_name(
	     file_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file handle from entry: %d.",
		 function,
		 raw_io_handle->current_pool_entry );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of information values
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_amount_of_information_values(
     intptr_t *io_handle,
     int *amount_of_information_values,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_amount_of_information_values";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->information_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing information values table.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_get_amount_of_values(
	     raw_io_handle->information_values_table,
	     amount_of_information_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of information values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves an information value identifier size
 * The identifier size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_information_value_identifier_size(
     intptr_t *io_handle,
     int information_value_index,
     size_t *identifier_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_information_value_identifier_size";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->information_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing information values table.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_get_identifier_size(
	     raw_io_handle->information_values_table,
	     information_value_index,
	     identifier_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve information value identifier size for index: %d.",
		 function,
		 information_value_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves an information value identifier
 * The strings are encoded in UTF-8
 * The identifier size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_information_value_identifier(
     intptr_t *io_handle,
     int information_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_information_value_identifier";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->information_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing information values table.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_get_identifier(
	     raw_io_handle->information_values_table,
	     information_value_index,
	     identifier,
	     identifier_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve information value identifier for index: %d.",
		 function,
		 information_value_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves an information value size
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_information_value_size(
     intptr_t *io_handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_information_value_size";
	int result                             = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->information_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing information values table.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libsmraw_values_table_get_value_size(
	          raw_io_handle->information_values_table,
	          identifier,
	          identifier_length,
	          value_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve information value size for identifier: %s.",
		 function,
		 identifier );

		return( -1 );
	}
	return( result );
}

/* Retrieves an information value
 * The strings are encoded in UTF-8
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_information_value(
     intptr_t *io_handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_information_value";
	int result                             = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->information_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing information values table.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libsmraw_values_table_get_value(
	          raw_io_handle->information_values_table,
	          identifier,
	          identifier_length,
	          value,
	          value_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve information value for identifier: %s.",
		 function,
		 identifier );

		return( -1 );
	}
	return( result );
}

/* Sets an information value
 * The strings are encoded in UTF-8
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_information_value(
     intptr_t *io_handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_set_information_value";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->information_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing information values table.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_set_value(
	     raw_io_handle->information_values_table,
	     identifier,
	     identifier_length,
	     value,
	     value_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set information value for identifier: %s.",
		 function,
		 identifier );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of integrity hash values
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_amount_of_integrity_hash_values(
     intptr_t *io_handle,
     int *amount_of_integrity_hash_values,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_amount_of_integrity_hash_values";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->integrity_hash_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing integrity hash values table.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_get_amount_of_values(
	     raw_io_handle->integrity_hash_values_table,
	     amount_of_integrity_hash_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of integrity hash values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves an integrity hash value identifier size
 * The identifier size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_integrity_hash_value_identifier_size(
     intptr_t *io_handle,
     int integrity_hash_value_index,
     size_t *identifier_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_integrity_hash_value_identifier_size";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->integrity_hash_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing integrity hash values table.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_get_identifier_size(
	     raw_io_handle->integrity_hash_values_table,
	     integrity_hash_value_index,
	     identifier_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve integrity hash value identifier size for index: %d.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves an integrity hash value identifier
 * The strings are encoded in UTF-8
 * The identifier size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_integrity_hash_value_identifier(
     intptr_t *io_handle,
     int integrity_hash_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_integrity_hash_value_identifier";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->integrity_hash_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing integrity hash values table.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_get_identifier(
	     raw_io_handle->integrity_hash_values_table,
	     integrity_hash_value_index,
	     identifier,
	     identifier_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve integrity hash value identifier for index: %d.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves an integrity hash value size
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_integrity_hash_value_size(
     intptr_t *io_handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_integrity_hash_value_size";
	int result                             = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->integrity_hash_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing integrity hash values table.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libsmraw_values_table_get_value_size(
	          raw_io_handle->integrity_hash_values_table,
	          identifier,
	          identifier_length,
	          value_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve integrity hash value size for identifier: %s.",
		 function,
		 identifier );

		return( -1 );
	}
	return( result );
}

/* Retrieves an integrity hash value
 * The strings are encoded in UTF-8
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_integrity_hash_value(
     intptr_t *io_handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_get_integrity_hash_value";
	int result                             = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->integrity_hash_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing integrity hash values table.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libsmraw_values_table_get_value(
	          raw_io_handle->integrity_hash_values_table,
	          identifier,
	          identifier_length,
	          value,
	          value_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve integrity hash value for identifier: %s.",
		 function,
		 identifier );

		return( -1 );
	}
	return( result );
}

/* Sets an integrity hash value
 * The strings are encoded in UTF-8
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_integrity_hash_value(
     intptr_t *io_handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *raw_io_handle = NULL;
	static char *function                  = "libsmraw_handle_set_integrity_hash_value";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	raw_io_handle = (libsmraw_internal_handle_t *) io_handle;

	if( raw_io_handle->integrity_hash_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing integrity hash values table.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_set_value(
	     raw_io_handle->integrity_hash_values_table,
	     identifier,
	     identifier_length,
	     value,
	     value_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set integrity hash value for identifier: %s.",
		 function,
		 identifier );

		return( -1 );
	}
	return( 1 );
}

