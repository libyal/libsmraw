/*
 * Input/Output (IO) handle functions
 *
 * Copyright (c) 2010-2013, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libsmraw_io_handle.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_libcerror.h"
#include "libsmraw_libfdata.h"

/* Initialize an IO handle
 * Make sure the value io_handle is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libsmraw_io_handle_initialize(
     libsmraw_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libsmraw_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libsmraw_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libsmraw_io_handle_free(
     libsmraw_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_io_handle_free";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( 1 );
}

/* Reads segment data into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libsmraw_io_handle_read_segment_data(
         libsmraw_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int segment_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         libcerror_error_t **error )
{
	static char *function = "libsmraw_io_handle_read_segment_data";
	ssize_t read_count    = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
/* TODO refactor */
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              0,
	              segment_data,
	              segment_data_size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read segment data.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Writes segment data from a buffer
 * Returns the number of bytes written or -1 on error
 */
ssize_t libsmraw_io_handle_write_segment_data(
         libsmraw_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int segment_index,
         const uint8_t *segment_data,
         size_t segment_data_size,
         libcerror_error_t **error )
{
	static char *function = "libsmraw_io_handle_write_segment_data";
	ssize_t write_count    = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
/* TODO refactor */
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               0,
	               segment_data,
	               segment_data_size,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write segment data.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Seeks a certain segment offset
 * Returns the offset or -1 on error
 */
off64_t libsmraw_io_handle_seek_segment_offset(
         libsmraw_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int segment_index,
         off64_t segment_offset,
         int whence,
         libcerror_error_t **error )
{
	static char *function = "libsmraw_io_handle_seek_segment_offset";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
/* TODO refactor */
	segment_offset = libbfio_pool_seek_offset(
	                  file_io_pool,
	                  0,
	                  segment_offset,
	                  whence,
	                  error );

	if( segment_offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to seek segment offset.",
		 function );

		return( -1 );
	}
	return( segment_offset );
}

