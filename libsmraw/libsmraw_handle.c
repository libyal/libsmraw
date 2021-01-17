/*
 * Handle functions
 *
 * Copyright (C) 2010-2021, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libsmraw_definitions.h"
#include "libsmraw_handle.h"
#include "libsmraw_information_file.h"
#include "libsmraw_io_handle.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_libcerror.h"
#include "libsmraw_libcnotify.h"
#include "libsmraw_libcthreads.h"
#include "libsmraw_libfdata.h"
#include "libsmraw_libfvalue.h"
#include "libsmraw_libuna.h"
#include "libsmraw_types.h"

/* Creates a handle
 * Make sure the value handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_initialize(
     libsmraw_handle_t **handle,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_initialize";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle value already set.",
		 function );

		return( -1 );
	}
	internal_handle = memory_allocate_structure(
	                   libsmraw_internal_handle_t );

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create internal handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_handle,
	     0,
	     sizeof( libsmraw_internal_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear handle.",
		 function );

		memory_free(
		 internal_handle );

		return( -1 );
	}
	if( libsmraw_io_handle_initialize(
	     &( internal_handle->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( libfvalue_table_initialize(
	     &( internal_handle->media_values ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create media values table.",
		 function );

		goto on_error;
	}
	if( libfvalue_table_initialize(
	     &( internal_handle->information_values ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create information values table.",
		 function );

		goto on_error;
	}
	if( libfvalue_table_initialize(
	     &( internal_handle->integrity_hash_values ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create integrity hash values table.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBSMRAW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_handle->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_handle->maximum_number_of_open_handles = LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES;

	*handle = (libsmraw_handle_t *) internal_handle;

	return( 1 );

on_error:
	if( internal_handle != NULL )
	{
		if( internal_handle->integrity_hash_values != NULL )
		{
			libfvalue_table_free(
			 &( internal_handle->integrity_hash_values ),
			 NULL );
		}
		if( internal_handle->information_values != NULL )
		{
			libfvalue_table_free(
			 &( internal_handle->information_values ),
			 NULL );
		}
		if( internal_handle->media_values != NULL )
		{
			libfvalue_table_free(
			 &( internal_handle->media_values ),
			 NULL );
		}
		if( internal_handle->io_handle != NULL )
		{
			libsmraw_io_handle_free(
			 &( internal_handle->io_handle ),
			 NULL );
		}
		memory_free(
		 internal_handle );
	}
	return( -1 );

}

/* Frees a handle
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_free(
     libsmraw_handle_t **handle,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_internal_handle_free";
	int result                                  = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		internal_handle = (libsmraw_internal_handle_t *) *handle;

		if( internal_handle->file_io_pool != NULL )
		{
			if( libsmraw_handle_close(
			     *handle,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close handle.",
				 function );

				result = -1;
			}
		}
		*handle = NULL;

#if defined( HAVE_LIBSMRAW_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_handle->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		if( libsmraw_io_handle_free(
		     &( internal_handle->io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
			 function );

			result = -1;
		}
		if( libfvalue_table_free(
		     &( internal_handle->media_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free media values table.",
			 function );

			result = -1;
		}
		if( libfvalue_table_free(
		     &( internal_handle->information_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free information values table.",
			 function );

			result = -1;
		}
		if( libfvalue_table_free(
		     &( internal_handle->integrity_hash_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free integrity hash values table.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_handle );
	}
	return( result );
}

/* Signals the handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_signal_abort(
     libsmraw_handle_t *handle,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_signal_abort";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_handle->io_handle->abort = 1;

	return( 1 );
}

/* Initializes the write values
 * Returns 1 if successful or -1 on error
 */
int libsmraw_internal_handle_initialize_write_values(
     libsmraw_internal_handle_t *internal_handle,
     libcerror_error_t **error )
{
	static char *function       = "libsmraw_internal_handle_initialize_write_values";
	size64_t last_segment_size  = 0;
	size64_t number_of_segments = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->write_values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: write values were initialized and cannot be initialized anymore.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle->maximum_segment_size == 0 )
	{
		internal_handle->io_handle->number_of_segments = 1;
	}
	else if( internal_handle->io_handle->media_size == 0 )
	{
		internal_handle->io_handle->number_of_segments = 0;
	}
	else
	{
		number_of_segments = internal_handle->io_handle->media_size
		                   / internal_handle->io_handle->maximum_segment_size;

		last_segment_size = internal_handle->io_handle->media_size
		                  % internal_handle->io_handle->maximum_segment_size;

		if( last_segment_size != 0 )
		{
			number_of_segments += 1;
		}
		if( number_of_segments > (size64_t) INT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of segments value out of bounds.",
			 function );

			return( -1 );
		}
		internal_handle->io_handle->number_of_segments = (int) number_of_segments;
	}
	internal_handle->write_values_initialized = 1;

	return( 1 );
}

/* Opens a set of storage media RAW files
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_open(
     libsmraw_handle_t *handle,
     char * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle            = NULL;
	libbfio_pool_t *file_io_pool                = NULL;
	libsmraw_internal_handle_t *internal_handle = NULL;
	system_character_t *information_filename    = NULL;
	static char *function                       = "libsmraw_handle_open";
	size_t filename_index                       = 0;
	size_t filename_length                      = 0;
	size_t information_filename_length          = 0;
	int bfio_access_flags                       = 0;
	int filename_iterator                       = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->file_io_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - file IO pool value already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->information_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - information file value already set.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of filenames value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	/* Open for read only or read/write
	 */
	if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
	{
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
		{
			bfio_access_flags |= LIBBFIO_ACCESS_FLAG_READ;
		}
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
		{
			bfio_access_flags |= LIBBFIO_ACCESS_FLAG_WRITE;
		}
		/* Set the basename
		 */
		filename_length = narrow_string_length(
				   filenames[ 0 ] );

		if( filename_length <= 4 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: missing basename.",
			 function );

			goto on_error;
		}
		if( libsmraw_io_handle_set_basename(
		     internal_handle->io_handle,
		     filenames[ 0 ],
		     filename_length - 4,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set basename.",
			function );

			goto on_error;
		}
		if( libbfio_pool_initialize(
		     &file_io_pool,
		     number_of_filenames,
		     internal_handle->maximum_number_of_open_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file IO pool.",
			 function );

			goto on_error;
		}
		for( filename_iterator = 0;
		     filename_iterator < number_of_filenames;
		     filename_iterator++ )
		{
			filename_length = narrow_string_length(
					   filenames[ filename_iterator ] );

			if( filename_length == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing filename: %d.",
				 function,
				 filename_iterator );

				goto on_error;
			}
			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file IO handle: %d.",
				 function,
				 filename_iterator );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libbfio_handle_set_track_offsets_read(
			     file_io_handle,
			     1,
			     error ) != 1 )
			{
		                libcerror_error_set(
		                 error,
		                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		                 "%s: unable to set track offsets read in file IO handle: %d.",
		                 function,
				 filename_iterator );

				goto on_error;
			}
#endif
			if( libbfio_file_set_name(
			     file_io_handle,
			     filenames[ filename_iterator ],
			     filename_length + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle: %d.",
				 function,
				 filename_iterator );

				goto on_error;
			}
			if( libbfio_pool_set_handle(
			     file_io_pool,
			     filename_iterator,
			     file_io_handle,
			     bfio_access_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set file IO handle: %d in pool.",
				 function,
				 filename_iterator );

				goto on_error;
			}
			file_io_handle = NULL;
		}
	}
	/* Open for write only
	 */
	else if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
	{
		/* Set the basename
		 */
		filename_length = narrow_string_length(
				   filenames[ 0 ] );

		if( filename_length == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: missing basename.",
			 function );

			goto on_error;
		}
		if( libsmraw_io_handle_set_basename(
		     internal_handle->io_handle,
		     filenames[ 0 ],
		     filename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set basename.",
			function );

			goto on_error;
		}
		/* Initialize the file IO pool
		 */
		if( libbfio_pool_initialize(
		     &file_io_pool,
		     0,
		     internal_handle->maximum_number_of_open_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file IO pool.",
			 function );

			goto on_error;
		}
	}
	if( libsmraw_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using file IO pool.",
		 function );

		goto on_error;
	}
	file_io_pool = NULL;

	/* Open the information file
	 */
	if( internal_handle->io_handle->basename != NULL )
	{
		information_filename_length = internal_handle->io_handle->basename_size + 9;

		information_filename = system_string_allocate(
		                        information_filename_length + 1 );

		if( information_filename == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create information filename.",
			 function );

			goto on_error;
		}
		if( system_string_copy(
		     information_filename,
		     internal_handle->io_handle->basename,
		     internal_handle->io_handle->basename_size - 1 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy basename to information filename.",
			 function );

			goto on_error;
		}
		filename_index = internal_handle->io_handle->basename_size - 1;

		if( system_string_copy(
		     &( information_filename[ filename_index ] ),
		     _SYSTEM_STRING( ".raw.info" ),
		     9 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy extension to information filename.",
			 function );

			goto on_error;
		}
		filename_index += 9;

		information_filename[ filename_index ] = 0;

		if( libsmraw_information_file_initialize(
		     &( internal_handle->information_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create information file.",
			 function );

			goto on_error;
		}
		information_filename_length = system_string_length(
		                               information_filename );

		if( libsmraw_information_file_set_name(
		     internal_handle->information_file,
		     information_filename,
		     information_filename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to set information filename.",
			 function );

			goto on_error;
		}
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
		{
			if( libsmraw_handle_read_information_file(
			     internal_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read information file.",
				 function );

				goto on_error;
			}
		}
		memory_free(
		 information_filename );

		information_filename = NULL;

		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
		{
			internal_handle->write_information_on_close = 1;
		}
	}
	internal_handle->file_io_pool_created_in_library = 1;

	return( 1 );

on_error:
	if( information_filename != NULL )
	{
		memory_free(
		 information_filename );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( internal_handle->information_file != NULL )
	{
		libsmraw_information_file_initialize(
		 &( internal_handle->information_file ),
		 NULL );
	}
	if( internal_handle->file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &( internal_handle->file_io_pool ),
		 NULL );
	}
	libsmraw_io_handle_clear(
	 internal_handle->io_handle,
	 NULL );

	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a set of storage media RAW files
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_open_wide(
     libsmraw_handle_t *handle,
     wchar_t * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle            = NULL;
	libbfio_pool_t *file_io_pool                = NULL;
	libsmraw_internal_handle_t *internal_handle = NULL;
	system_character_t *information_filename    = NULL;
	static char *function                       = "libsmraw_handle_open_wide";
	size_t filename_index                       = 0;
	size_t filename_length                      = 0;
	size_t information_filename_length          = 0;
	int bfio_access_flags                       = 0;
	int filename_iterator                       = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->file_io_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - file IO pool value already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->information_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - information file value already set.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of filenames value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	/* Open for read only or read/write
	 */
	if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
	{
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
		{
			bfio_access_flags |= LIBBFIO_ACCESS_FLAG_READ;
		}
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
		{
			bfio_access_flags |= LIBBFIO_ACCESS_FLAG_WRITE;
		}
		/* Set the basename
		 */
		filename_length = wide_string_length(
				   filenames[ 0 ] );

		if( filename_length <= 4 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: missing basename.",
			 function );

			goto on_error;
		}
		if( libsmraw_io_handle_set_basename_wide(
		     internal_handle->io_handle,
		     filenames[ 0 ],
		     filename_length - 4,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set basename.",
			function );

			goto on_error;
		}
		if( libbfio_pool_initialize(
		     &file_io_pool,
		     number_of_filenames,
		     internal_handle->maximum_number_of_open_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file IO pool.",
			 function );

			goto on_error;
		}
		for( filename_iterator = 0;
		     filename_iterator < number_of_filenames;
		     filename_iterator++ )
		{
			filename_length = wide_string_length(
					   filenames[ filename_iterator ] );

			if( filename_length == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing filename: %d.",
				 function,
				 filename_iterator );

				goto on_error;
			}
			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file IO handle: %d.",
				 function,
				 filename_iterator );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libbfio_handle_set_track_offsets_read(
			     file_io_handle,
			     1,
			     error ) != 1 )
			{
		                libcerror_error_set(
		                 error,
		                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		                 "%s: unable to set track offsets read in file IO handle: %d.",
		                 function,
				 filename_iterator );

		                goto on_error;
			}
#endif
			if( libbfio_file_set_name_wide(
			     file_io_handle,
			     filenames[ filename_iterator ],
			     filename_length + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle: %d.",
				 function,
				 filename_iterator );

				goto on_error;
			}
			if( libbfio_pool_set_handle(
			     file_io_pool,
			     filename_iterator,
			     file_io_handle,
			     bfio_access_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set file IO handle: %d in pool.",
				 function,
				 filename_iterator );

				goto on_error;
			}
			file_io_handle = NULL;
		}
	}
	/* Open for write only
	 */
	else if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
	{
		/* Set the basename
		 */
		filename_length = wide_string_length(
				   filenames[ 0 ] );

		if( filename_length == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: missing basename.",
			 function );

			goto on_error;
		}
		if( libsmraw_io_handle_set_basename_wide(
		     internal_handle->io_handle,
		     filenames[ 0 ],
		     filename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set basename.",
			function );

			goto on_error;
		}
		/* Initialize the file IO pool
		 */
		if( libbfio_pool_initialize(
		     &file_io_pool,
		     0,
		     internal_handle->maximum_number_of_open_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file IO pool.",
			 function );

			goto on_error;
		}
	}
	if( libsmraw_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using file IO pool.",
		 function );

		goto on_error;
	}
	file_io_pool = NULL;

	/* Open the information file
	 */
	if( internal_handle->io_handle->basename != NULL )
	{
		information_filename_length = internal_handle->io_handle->basename_size + 9;

		information_filename = system_string_allocate(
		                        information_filename_length + 1 );

		if( information_filename == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create information filename.",
			 function );

			goto on_error;
		}
		if( system_string_copy(
		     information_filename,
		     internal_handle->io_handle->basename,
		     internal_handle->io_handle->basename_size - 1 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy basename to information filename.",
			 function );

			goto on_error;
		}
		filename_index = internal_handle->io_handle->basename_size - 1;

		if( system_string_copy(
		     &( information_filename[ filename_index ] ),
		     _SYSTEM_STRING( ".raw.info" ),
		     9 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy extension to information filename.",
			 function );

			goto on_error;
		}
		filename_index += 9;

		information_filename[ filename_index ] = 0;

		if( libsmraw_information_file_initialize(
		     &( internal_handle->information_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create information file.",
			 function );

			goto on_error;
		}
		information_filename_length = system_string_length(
		                               information_filename );

		if( libsmraw_information_file_set_name(
		     internal_handle->information_file,
		     information_filename,
		     information_filename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to set information filename.",
			 function );

			goto on_error;
		}
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
		{
			if( libsmraw_handle_read_information_file(
			     internal_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read information file.",
				 function );

				goto on_error;
			}
		}
		memory_free(
		 information_filename );

		information_filename = NULL;

		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
		{
			internal_handle->write_information_on_close = 1;
		}
	}
	internal_handle->file_io_pool_created_in_library = 1;

	return( 1 );

on_error:
	if( information_filename != NULL )
	{
		memory_free(
		 information_filename );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( internal_handle->information_file != NULL )
	{
		libsmraw_information_file_initialize(
		 &( internal_handle->information_file ),
		 NULL );
	}
	if( internal_handle->file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &( internal_handle->file_io_pool ),
		 NULL );
	}
	libsmraw_io_handle_clear(
	 internal_handle->io_handle,
	 NULL );

	return( -1 );
}

#endif

/* Opens a set of storage media RAW files using a Basic File IO (bfio) pool
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_open_file_io_pool(
     libsmraw_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle            = NULL;
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_open_file_io_pool";
	size64_t file_io_handle_size                = 0;
	int number_of_file_io_handles               = 0;
	int bfio_access_flags                       = 0;
	int file_io_handle_index                    = 0;
	int file_io_handle_is_open                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->file_io_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - file IO pool value already set.",
		 function );

		return( -1 );
	}
	if( file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO pool.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_get_number_of_handles(
	     file_io_pool,
	     &number_of_file_io_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of handles in the file IO pool.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     &( internal_handle->segments_stream ),
	     (intptr_t *) internal_handle->io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, int, int *, off64_t *, size64_t *, uint32_t *, libcerror_error_t **)) &libsmraw_io_handle_create_segment,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libsmraw_io_handle_read_segment_data,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, const uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libsmraw_io_handle_write_segment_data,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libsmraw_io_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segments stream.",
		 function );

		goto on_error;
	}
	if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
	{
		if( number_of_file_io_handles <= 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing file IO handles.",
			 function );

			goto on_error;
		}
		if( libfdata_stream_resize(
		     internal_handle->segments_stream,
		     number_of_file_io_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize segments stream.",
			 function );

			goto on_error;
		}
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
		{
			bfio_access_flags = LIBBFIO_OPEN_READ_WRITE;
		}
		else
		{
			bfio_access_flags = LIBBFIO_OPEN_READ;
		}
		for( file_io_handle_index = 0;
		     file_io_handle_index < number_of_file_io_handles;
		     file_io_handle_index++ )
		{
			if( libbfio_pool_get_handle(
			     file_io_pool,
			     file_io_handle_index,
			     &file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to retrieve file IO handle from pool entry: %d.",
				 function,
				 file_io_handle_index );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: processing pool entry: %d.\n",
				 function,
				 file_io_handle_index );
			}
#endif
			file_io_handle_is_open = libbfio_handle_is_open(
			                          file_io_handle,
			                          error );

			if( file_io_handle_is_open == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to determine if file IO handle is open.",
				 function );

				goto on_error;
			}
			else if( file_io_handle_is_open == 0 )
			{
				if( libbfio_pool_open(
				     file_io_pool,
				     file_io_handle_index,
				     bfio_access_flags,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to open pool entry: %d.",
					 function,
					 file_io_handle_index );

					goto on_error;
				}
			}
			if( libbfio_pool_get_size(
			     file_io_pool,
			     file_io_handle_index,
			     &file_io_handle_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve file size for pool entry: %d.",
				 function,
				 file_io_handle_index );

				goto on_error;
			}
			if( libfdata_stream_set_segment_by_index(
			     internal_handle->segments_stream,
			     file_io_handle_index,
			     file_io_handle_index,
			     0,
			     file_io_handle_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set segment: %d in segments stream.",
				 function,
				 file_io_handle_index );

				goto on_error;
			}
			if( file_io_handle_index == 0 )
			{
				internal_handle->io_handle->maximum_segment_size = file_io_handle_size;
			}
			else if( file_io_handle_size > internal_handle->io_handle->maximum_segment_size )
			{
				internal_handle->io_handle->maximum_segment_size = file_io_handle_size;
			}
		}
		if( libfdata_stream_get_size(
		     internal_handle->segments_stream,
		     &( internal_handle->io_handle->media_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size from segments stream.",
			 function );

			goto on_error;
		}
		internal_handle->read_values_initialized = 1;
	}
	internal_handle->file_io_pool = file_io_pool;

	return( 1 );

on_error:
	if( internal_handle->segments_stream != NULL )
	{
		libfdata_stream_free(
		 &( internal_handle->segments_stream ),
		 NULL );
	}
	internal_handle->read_values_initialized = 0;

	return( -1 );
}

/* Read an information file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_read_information_file(
     libsmraw_internal_handle_t *internal_handle,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libsmraw_handle_read_information_file";
	int result                       = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->information_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing information file.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     internal_handle->information_file->name,
	     internal_handle->information_file->name_size - 1,
	     error ) != 1 )
#else
	if( libbfio_file_set_name(
	     file_io_handle,
	     internal_handle->information_file->name,
	     internal_handle->information_file->name_size - 1,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name in file IO handle.",
		 function );

		goto on_error;
	}
	result = libbfio_handle_exists(
		  file_io_handle,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if information filename exists.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	/* Only read the information file if it exists
	 */
	if( result == 1 )
	{
		if( libsmraw_information_file_open(
		     internal_handle->information_file,
		     internal_handle->information_file->name,
		     LIBSMRAW_OPEN_READ,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open information file.",
			 function );

			goto on_error;
		}
		if( libsmraw_information_file_read_section(
		     internal_handle->information_file,
		     (uint8_t *) "media_values",
		     12,
		     internal_handle->media_values,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read media values from information file.",
			 function );

			libsmraw_information_file_close(
			 internal_handle->information_file,
			 NULL );

			goto on_error;
		}
		if( libsmraw_information_file_read_section(
		     internal_handle->information_file,
		     (uint8_t *) "information_values",
		     18,
		     internal_handle->information_values,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read information values from information file.",
			 function );

			libsmraw_information_file_close(
			 internal_handle->information_file,
			 NULL );

			goto on_error;
		}
		if( libsmraw_information_file_read_section(
		     internal_handle->information_file,
		     (uint8_t *) "integrity_hash_values",
		     21,
		     internal_handle->integrity_hash_values,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read integrity hash values from information file.",
			 function );

			libsmraw_information_file_close(
			 internal_handle->information_file,
			 NULL );

			goto on_error;
		}
		if( libsmraw_information_file_close(
		     internal_handle->information_file,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to close information file.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Closes a RAW handle
 * Returns the 0 if succesful or -1 on error
 */
int libsmraw_handle_close(
     libsmraw_handle_t *handle,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_close";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->write_information_on_close != 0 )
	{
		if( libsmraw_information_file_open(
		     internal_handle->information_file,
		     internal_handle->information_file->name,
		     LIBSMRAW_OPEN_WRITE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open information file.",
			 function );

			return( -1 );
		}
		if( libsmraw_information_file_write_section(
		     internal_handle->information_file,
		     (uint8_t *) "information_values",
		     18,
		     internal_handle->information_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write information values to information file.",
			 function );

			return( -1 );
		}
		if( libsmraw_information_file_write_section(
		     internal_handle->information_file,
		     (uint8_t *) "integrity_hash_values",
		     21,
		     internal_handle->integrity_hash_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write integrity hash values to information file.",
			 function );

			return( -1 );
		}
		if( libsmraw_information_file_close(
		     internal_handle->information_file,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close information file.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->file_io_pool_created_in_library != 0 )
	{
		if( libbfio_pool_close_all(
		     internal_handle->file_io_pool,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable close file IO pool.",
			 function );

			result = -1;
		}
		if( libbfio_pool_free(
		     &( internal_handle->file_io_pool ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO pool.",
			 function );

			result = -1;
		}
		internal_handle->file_io_pool_created_in_library = 0;
	}
	internal_handle->file_io_pool             = NULL;
	internal_handle->read_values_initialized  = 0;
	internal_handle->write_values_initialized = 0;

	if( libsmraw_io_handle_clear(
	     internal_handle->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	if( libfdata_stream_free(
	     &( internal_handle->segments_stream ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free segments stream.",
		 function );

		result = -1;
	}
	if( internal_handle->information_file != NULL )
	{
		if( libsmraw_information_file_free(
		     &( internal_handle->information_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free information file.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Reads a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libsmraw_handle_read_buffer(
         libsmraw_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_read_buffer";
	off64_t current_offset                      = 0;
	ssize_t read_count                          = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle->media_size == 0 )
	{
		return( 0 );
	}
#if defined( HAVE_LIBSMRAW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libfdata_stream_get_offset(
	     internal_handle->segments_stream,
	     &current_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset from segments stream.",
		 function );

		goto on_error;
	}
	if( current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid current offset value out of bounds.",
		 function );

		goto on_error;
	}
	/* Bail out early for requests to read beyond the media size
	 */
	if( (size64_t) current_offset >= internal_handle->io_handle->media_size )
	{
		buffer_size = 0;
	}
	else if( ( (size64_t) current_offset + buffer_size ) >= internal_handle->io_handle->media_size )
	{
		buffer_size = (size_t) ( internal_handle->io_handle->media_size - (size64_t) current_offset );
	}
	if( buffer_size > 0 )
	{
		read_count = libfdata_stream_read_buffer(
		              internal_handle->segments_stream,
		              (intptr_t *) internal_handle->file_io_pool,
		              (uint8_t *) buffer,
		              buffer_size,
		              0,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer from segments stream.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_LIBSMRAW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );

on_error:
#if defined( HAVE_LIBSMRAW_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	  internal_handle->read_write_lock,
	  NULL );
#endif
	return( -1 );
}

/* Reads (media) data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libsmraw_handle_read_buffer_at_offset(
         libsmraw_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libsmraw_handle_read_buffer_at_offset";
	ssize_t read_count    = 0;

	if( libsmraw_handle_seek_offset(
	     handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	read_count = libsmraw_handle_read_buffer(
	              handle,
	              buffer,
	              buffer_size,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Writes a buffer
 * Returns the number of bytes written or -1 on error
 */
ssize_t libsmraw_handle_write_buffer(
         libsmraw_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_write_buffer";
	off64_t current_offset                      = 0;
	ssize_t write_count                         = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->write_values_initialized == 0 )
	{
		if( libsmraw_internal_handle_initialize_write_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize write values.",
			 function );

			return( -1 );
		}
	}
	/* Bail out early for requests to write empty buffers
	 */
	if( buffer_size == 0 )
	{
		return( 0 );
	}
	if( libfdata_stream_get_offset(
	     internal_handle->segments_stream,
	     &current_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset from segments stream.",
		 function );

		return( -1 );
	}
	if( current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle->media_size > 0 )
	{
		/* Bail out early for requests to write beyond the media size
		 */
		if( (size64_t) current_offset >= internal_handle->io_handle->media_size )
		{
			return( 0 );
		}
		if( ( (size64_t) current_offset + buffer_size ) >= internal_handle->io_handle->media_size )
		{
			buffer_size = (size_t) ( internal_handle->io_handle->media_size - (size64_t) current_offset );
		}
	}
	write_count = libfdata_stream_write_buffer(
	               internal_handle->segments_stream,
	               (intptr_t *) internal_handle->file_io_pool,
	               (uint8_t *) buffer,
	               buffer_size,
	               0,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write buffer to segments stream.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Writes (media) data at a specific offset,
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libsmraw_handle_write_buffer_at_offset(
         libsmraw_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libsmraw_handle_write_buffer_at_offset";
	ssize_t write_count   = 0;

	if( libsmraw_handle_seek_offset(
	     handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	write_count = libsmraw_handle_write_buffer(
	               handle,
	               buffer,
	               buffer_size,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write buffer.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Seeks a certain offset
 * Returns the offset or -1 on error
 */
off64_t libsmraw_handle_seek_offset(
         libsmraw_handle_t *handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_seek_offset";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	offset = libfdata_stream_seek_offset(
	          internal_handle->segments_stream,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to seek offset in segments stream.",
		 function );

		return( -1 );
	}
	return( offset );
}

/* Retrieves the current offset of the (media) data
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_offset(
     libsmraw_handle_t *handle,
     off64_t *offset,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_offset";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( libfdata_stream_get_offset(
	     internal_handle->segments_stream,
	     offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve offset from segments stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the maximum number of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_set_maximum_number_of_open_handles(
     libsmraw_handle_t *handle,
     int maximum_number_of_open_handles,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_maximum_number_of_open_handles";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool != NULL )
	{
		if( libbfio_pool_set_maximum_number_of_open_handles(
		     internal_handle->file_io_pool,
		     maximum_number_of_open_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set maximum number of open handles in file IO handle.",
			 function );

			return( -1 );
		}
	}
	internal_handle->maximum_number_of_open_handles = maximum_number_of_open_handles;

	return( 1 );
}

/* Retrieves the segment filename size
 * The segment filename size includes the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_get_segment_filename_size(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_segment_filename_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( libsmraw_io_handle_get_basename_size(
	     internal_handle->io_handle,
	     filename_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve basename size from IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the segment filename
 * The segment filename size should include the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_get_segment_filename(
     libsmraw_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_segment_filename";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( libsmraw_io_handle_get_basename(
	     internal_handle->io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve basename from IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the segment filename
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_set_segment_filename(
     libsmraw_handle_t *handle,
     const char *filename,
     size_t filename_length,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_segment_filename";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle->basename != NULL )
	{
		if( internal_handle->file_io_pool != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: basename value already set: %" PRIs_SYSTEM ".",
			 function,
			 internal_handle->io_handle->basename );

			return( -1 );
		}
	}
	if( libsmraw_io_handle_set_basename(
	     internal_handle->io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set basename in IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the segment filename size
 * The segment filename size includes the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_get_segment_filename_size_wide(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_segment_filename_size_wide";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( libsmraw_io_handle_get_basename_size_wide(
	     internal_handle->io_handle,
	     filename_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve basename size from IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the segment filename
 * The segment filename size should include the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_get_segment_filename_wide(
     libsmraw_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_segment_filename_wide";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( libsmraw_io_handle_get_basename_wide(
	     internal_handle->io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve basename from IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the segment filename
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_set_segment_filename_wide(
     libsmraw_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_segment_filename_wide";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle->basename != NULL )
	{
		if( internal_handle->file_io_pool != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: basename value already set: %" PRIs_SYSTEM ".",
			 function,
			 internal_handle->io_handle->basename );

			return( -1 );
		}
	}
	if( libsmraw_io_handle_set_basename_wide(
	     internal_handle->io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set basename in IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Retrieves the segment file size
 * 0 represents the maximum possible segment file size
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_maximum_segment_size(
     libsmraw_handle_t *handle,
     size64_t *maximum_segment_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_maximum_segment_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( libsmraw_io_handle_get_maximum_segment_size(
	     internal_handle->io_handle,
	     maximum_segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve maximum segment size from IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the segment file size
 * 0 represents the maximum possible segment file size
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_set_maximum_segment_size(
     libsmraw_handle_t *handle,
     size64_t maximum_segment_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_maximum_segment_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->write_values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: maximum segment size cannot be changed.",
		 function );

		return( -1 );
	}
	if( libsmraw_io_handle_set_maximum_segment_size(
	     internal_handle->io_handle,
	     maximum_segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set maximum segment size in IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the filename size of the segment file of the current offset
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_filename_size(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libsmraw_handle_get_filename_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libsmraw_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current offset.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name_size(
	     file_io_handle,
	     filename_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the filename of the segment file of the current offset
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_filename(
     libsmraw_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libsmraw_handle_get_filename";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libsmraw_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current offset.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the filename size of the segment file of the current offset
 * The filename size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_filename_size_wide(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libsmraw_handle_get_filename_size_wide";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libsmraw_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current offset.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name_size_wide(
	     file_io_handle,
	     filename_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the filename of the segment file of the current offset
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_filename_wide(
     libsmraw_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libsmraw_handle_get_filename_wide";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libsmraw_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current offset.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name_wide(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif

/* Retrieves the file IO handle of the segment file of the current offset
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_file_io_handle(
     libsmraw_handle_t *handle,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_file_io_handle";
	off64_t current_offset                      = 0;
	off64_t segment_data_offset                 = 0;
	int segment_index                           = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	if( libfdata_stream_get_offset(
	     internal_handle->segments_stream,
	     &current_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset from segments stream.",
		 function );

		return( -1 );
	}
	if( libfdata_stream_get_segment_index_at_offset(
	     internal_handle->segments_stream,
	     current_offset,
	     &segment_index,
	     &segment_data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment index at offset: %" PRIi64 " (0x%08" PRIx64 ") from segments stream.",
		 function,
		 current_offset,
		 current_offset );

		return( -1 );
	}
	if( libbfio_pool_get_handle(
	     internal_handle->file_io_pool,
	     segment_index,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for pool entry: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 segment_index,
		 current_offset,
		 current_offset );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the media size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_media_size(
     libsmraw_handle_t *handle,
     size64_t *media_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_media_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBSMRAW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*media_size = internal_handle->io_handle->media_size;

#if defined( HAVE_LIBSMRAW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the media size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_media_size(
     libsmraw_handle_t *handle,
     size64_t media_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_media_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->read_values_initialized != 0 )
	 || ( internal_handle->write_values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media size cannot be changed.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	internal_handle->io_handle->media_size = media_size;

	return( 1 );
}

/* Retrieves the bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_bytes_per_sector(
     libsmraw_handle_t *handle,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_bytes_per_sector";
	const char *identifier                      = "bytes_per_sector";
	size_t identifier_size                      = 16;
	uint64_t value_64bit                        = 0;
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( bytes_per_sector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bytes per sector.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBSMRAW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*bytes_per_sector = 0;

/* TODO refactor into media_values_get_64bit_value function ? */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->media_values,
	          (uint8_t *) identifier,
	          identifier_size,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from media values table.",
		 function,
		 identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_64bit(
		     value,
		     0,
		     &value_64bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to a 64-bit value.",
			 function );

			goto on_error;
		}
		if( value_64bit > (uint64_t) UINT32_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: 64-bit bytes per sector value out of bounds.",
			 function );

			goto on_error;
		}
		*bytes_per_sector = (uint32_t) value_64bit;
	}
#if defined( HAVE_LIBSMRAW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBSMRAW_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_bytes_per_sector(
     libsmraw_handle_t *handle,
     uint32_t bytes_per_sector,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_bytes_per_sector";
	const char *identifier                      = "bytes_per_sector";
	size_t identifier_size                      = 16;
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( ( internal_handle->read_values_initialized != 0 )
	 || ( internal_handle->write_values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media values cannot be changed.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
/* TODO refactor into media_values_set_64bit_value function ? */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->media_values,
	          (uint8_t *) identifier,
	          identifier_size,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from media values table.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_64BIT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     (uint8_t *) identifier,
		     identifier_size,
		     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->media_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in media values table.",
			 function,
			 identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_64bit(
	     value,
	     0,
	     (uint64_t) bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value from a 64-bit value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the media type
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_media_type(
     libsmraw_handle_t *handle,
     uint8_t *media_type,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	uint8_t *value_data                         = NULL;
	static char *function                       = "libsmraw_handle_get_media_type";
	const char *identifier                      = "media_type";
	size_t identifier_size                      = 10;
	size_t value_data_size                      = 0;
	int encoding                                = 0;
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( media_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media type.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	*media_type = LIBSMRAW_MEDIA_TYPE_UNKNOWN;

	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->media_values,
	          (uint8_t *) identifier,
	          identifier_size,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from media values table.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_data(
		     value,
		     &value_data,
		     &value_data_size,
		     &encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			return( -1 );
		}
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value data.",
			 function );

			return( -1 );
		}
		if( value_data_size == 6 )
		{
			if( narrow_string_compare(
			     "fixed",
			     value_data,
			     5 ) == 0 )
			{
				*media_type = LIBSMRAW_MEDIA_TYPE_FIXED;
			}
		}
		else if( value_data_size == 7 )
		{
			if( narrow_string_compare(
			     "memory",
			     value_data,
			     6 ) == 0 )
			{
				*media_type = LIBSMRAW_MEDIA_TYPE_MEMORY;
			}
		}
		else if( value_data_size == 8 )
		{
			if( narrow_string_compare(
			     "optical",
			     value_data,
			     7 ) == 0 )
			{
				*media_type = LIBSMRAW_MEDIA_TYPE_OPTICAL;
			}
		}
		else if( value_data_size == 10 )
		{
			if( narrow_string_compare(
			     "removable",
			     value_data,
			     9 ) == 0 )
			{
				*media_type = LIBSMRAW_MEDIA_TYPE_REMOVABLE;
			}
		}
	}
	return( 1 );
}

/* Sets the media type
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_media_type(
     libsmraw_handle_t *handle,
     uint8_t media_type,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_media_type";
	const char *identifier                      = "media_type";
	const char *value_string                    = NULL;
	size_t identifier_size                      = 10;
	size_t value_string_length                  = 0;
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( ( internal_handle->read_values_initialized != 0 )
	 || ( internal_handle->write_values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media values cannot be changed.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	switch( media_type )
	{
		case LIBSMRAW_MEDIA_TYPE_FIXED:
			value_string        = "fixed";
			value_string_length = 5;

			break;

		case LIBSMRAW_MEDIA_TYPE_MEMORY:
			value_string        = "memory";
			value_string_length = 6;

			break;

		case LIBSMRAW_MEDIA_TYPE_OPTICAL:
			value_string        = "optical";
			value_string_length = 7;

			break;

		case LIBSMRAW_MEDIA_TYPE_REMOVABLE:
			value_string        = "removable";
			value_string_length = 9;

			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported media type: 0x%02x.",
			 function,
			 media_type );

			return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->media_values,
	          (uint8_t *) identifier,
	          identifier_size,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from media values table.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     (uint8_t *) identifier,
		     identifier_size,
		     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->media_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in media values table.",
			 function,
			 identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_utf8_string(
	     value,
	     0,
	     (uint8_t *) value_string,
	     value_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value from an UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the media flags
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_media_flags(
     libsmraw_handle_t *handle,
     uint8_t *media_flags,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	uint8_t *value_data                         = NULL;
	static char *function                       = "libsmraw_handle_get_media_flags";
	const char *identifier                      = "media_flags";
	size_t identifier_size                      = 11;
	size_t value_data_size                      = 0;
	int encoding                                = 0;
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( media_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media flags.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->media_values,
	          (uint8_t *) identifier,
	          identifier_size,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from media values table.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_data(
		     value,
		     &value_data,
		     &value_data_size,
		     &encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			return( -1 );
		}
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value data.",
			 function );

			return( -1 );
		}
		if( value_data_size == 8 )
		{
			if( narrow_string_compare(
			     "logical",
			     value_data,
			     7 ) == 0 )
			{
				*media_flags &= ~( LIBSMRAW_MEDIA_FLAG_PHYSICAL );
			}
		}
		else if( value_data_size == 9 )
		{
			if( narrow_string_compare(
			     "physical",
			     value_data,
			     8 ) == 0 )
			{
				*media_flags |= LIBSMRAW_MEDIA_FLAG_PHYSICAL;
			}
		}
	}
	return( 1 );
}

/* Sets the media flags
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_media_flags(
     libsmraw_handle_t *handle,
     uint8_t media_flags,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_media_flags";
	const char *identifier                      = "media_flags";
	const char *value_string                    = NULL;
	size_t identifier_size                      = 11;
	size_t value_string_length                  = 0;
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( ( internal_handle->read_values_initialized != 0 )
	 || ( internal_handle->write_values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media size cannot be changed.",
		 function );

		return( -1 );
	}
	if( ( media_flags & ~( LIBSMRAW_MEDIA_FLAG_PHYSICAL ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported media flags: 0x%02x.",
		 function,
		 media_flags );

		return( -1 );
	}
/* TODO add thread-safety support */
	if( ( media_flags & LIBSMRAW_MEDIA_FLAG_PHYSICAL ) != 0 )
	{
		value_string        = "physical";
		value_string_length = 8;
	}
	else
	{
		value_string        = "logical";
		value_string_length = 7;
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->media_values,
	          (uint8_t *) identifier,
	          identifier_size,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from media values table.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     (uint8_t *) identifier,
		     identifier_size,
		     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->media_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in media values table.",
			 function,
			 identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_utf8_string(
	     value,
	     0,
	     (uint8_t *) value_string,
	     value_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value from an UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of information values
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_number_of_information_values(
     libsmraw_handle_t *handle,
     int *number_of_information_values,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_number_of_information_values";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	if( libfvalue_table_get_number_of_values(
	     internal_handle->information_values,
	     number_of_information_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of information values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of a specific information value identifier
 * The identifier size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_information_value_identifier_size(
     libsmraw_handle_t *handle,
     int information_value_index,
     size_t *identifier_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	uint8_t *value_identifier                   = NULL;
	static char *function                       = "libsmraw_handle_get_information_value_identifier_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	if( libfvalue_table_get_value_by_index(
	     internal_handle->information_values,
	     information_value_index,
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from information values table.",
		 function,
		 information_value_index );

		return( -1 );
	}
	if( libfvalue_value_get_identifier(
	     value,
	     &value_identifier,
	     identifier_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value identifier: %d.",
		 function,
		 information_value_index );

		return( -1 );
	}
	if( value_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value identifier: %d.",
		 function,
		 information_value_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific information value identifier
 * The identifier size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_information_value_identifier(
     libsmraw_handle_t *handle,
     int information_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	uint8_t *value_identifier                   = NULL;
	static char *function                       = "libsmraw_handle_get_information_value_identifier";
	size_t value_identifier_size                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	if( libfvalue_table_get_value_by_index(
	     internal_handle->information_values,
	     information_value_index,
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from information values table.",
		 function,
		 information_value_index );

		return( -1 );
	}
	if( libfvalue_value_get_identifier(
	     value,
	     &value_identifier,
	     &value_identifier_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value identifier: %d.",
		 function,
		 information_value_index );

		return( -1 );
	}
	if( value_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value identifier: %d.",
		 function,
		 information_value_index );

		return( -1 );
	}
	if( identifier_size < value_identifier_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: identifier too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     identifier,
	     value_identifier,
	     value_identifier_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of an UTF-8 encoded information value for the specific identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf8_information_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf8_information_value_size";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->information_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from information values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_utf8_string_size(
		     value,
		     0,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %s UTF-8 string size.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves an UTF-8 encoded information value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf8_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf8_information_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->information_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from information values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_utf8_string(
		     value,
		     0,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value: %s to an UTF-8 string.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Sets an UTF-8 encoded information value for the specific identifier
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_utf8_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_utf8_information_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->read_values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: information values cannot be changed.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->information_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from information values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     identifier,
		     identifier_length + 1,
		     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->information_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in information values table.",
			 function,
			 (char *) identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_utf8_string(
	     value,
	     0,
	     utf8_string,
	     utf8_string_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value: %s from an UTF-8 string.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of an UTF-16 encoded information value for the specific identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf16_information_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf16_information_value_size";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->information_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from information values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_utf16_string_size(
		     value,
		     0,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %s UTF-16 string size.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves an UTF-16 encoded information value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf16_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf16_information_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->information_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from information values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_utf16_string(
		     value,
		     0,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value: %s to an UTF-16 string.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Sets an UTF-16 encoded information value for the specific identifier
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_utf16_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_utf16_information_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->read_values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: information values cannot be changed.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->information_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from information values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     identifier,
		     identifier_length + 1,
		     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->information_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in information values table.",
			 function,
			 (char *) identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_utf16_string(
	     value,
	     0,
	     utf16_string,
	     utf16_string_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value: %s from an UTF-16 string.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of integrity hash values
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_number_of_integrity_hash_values(
     libsmraw_handle_t *handle,
     int *number_of_integrity_hash_values,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_number_of_integrity_hash_values";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	if( libfvalue_table_get_number_of_values(
	     internal_handle->integrity_hash_values,
	     number_of_integrity_hash_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of integrity hash values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of a specific integrity hash value identifier
 * The identifier size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_integrity_hash_value_identifier_size(
     libsmraw_handle_t *handle,
     int integrity_hash_value_index,
     size_t *identifier_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	uint8_t *value_identifier                   = NULL;
	static char *function                       = "libsmraw_handle_get_integrity_hash_value_identifier_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	if( libfvalue_table_get_value_by_index(
	     internal_handle->integrity_hash_values,
	     integrity_hash_value_index,
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from integrity hash values table.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	if( libfvalue_value_get_identifier(
	     value,
	     &value_identifier,
	     identifier_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value identifier: %d.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	if( value_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value identifier: %d.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific integrity hash value identifier
 * The identifier size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_integrity_hash_value_identifier(
     libsmraw_handle_t *handle,
     int integrity_hash_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	uint8_t *value_identifier                   = NULL;
	static char *function                       = "libsmraw_handle_get_integrity_hash_value_identifier";
	size_t value_identifier_size                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	if( libfvalue_table_get_value_by_index(
	     internal_handle->integrity_hash_values,
	     integrity_hash_value_index,
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from integrity hash values table.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	if( libfvalue_value_get_identifier(
	     value,
	     &value_identifier,
	     &value_identifier_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value identifier: %d.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	if( value_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value identifier: %d.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	if( identifier_size < value_identifier_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: identifier too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     identifier,
	     value_identifier,
	     value_identifier_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of an UTF-8 encoded integrity hash value for the specific identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf8_integrity_hash_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf8_integrity_hash_value_size";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->integrity_hash_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from integrity hash values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_utf8_string_size(
		     value,
		     0,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %s UTF-8 string size.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves an UTF-8 encoded integrity hash value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf8_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf8_integrity_hash_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->integrity_hash_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from integrity hash values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_utf8_string(
		     value,
		     0,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value: %s to an UTF-8 string.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Sets an UTF-8 encoded integrity hash value for the specific identifier
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_utf8_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_utf8_integrity_hash_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->read_values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: integrity hash values cannot be changed.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->integrity_hash_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from integrity hash values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     identifier,
		     identifier_length + 1,
		     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->integrity_hash_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in integrity hash values table.",
			 function,
			 (char *) identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_utf8_string(
	     value,
	     0,
	     utf8_string,
	     utf8_string_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value: %s from an UTF-8 string.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of an UTF-16 encoded integrity hash value for the specific identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf16_integrity_hash_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf16_integrity_hash_value_size";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->integrity_hash_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from integrity hash values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_utf16_string_size(
		     value,
		     0,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %s UTF-16 string size.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves an UTF-16 encoded integrity hash value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf16_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf16_integrity_hash_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->integrity_hash_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from integrity hash values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_utf16_string(
		     value,
		     0,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value: %s to an UTF-16 string.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Sets an UTF-16 encoded integrity hash value for the specific identifier
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_utf16_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_utf16_integrity_hash_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->read_values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: integrity hash values cannot be changed.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
/* TODO add thread-safety support */
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->integrity_hash_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from integrity hash values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     identifier,
		     identifier_length + 1,
		     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->integrity_hash_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in integrity hash values table.",
			 function,
			 (char *) identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_utf16_string(
	     value,
	     0,
	     utf16_string,
	     utf16_string_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value: %s from an UTF-16 string.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	return( 1 );
}

