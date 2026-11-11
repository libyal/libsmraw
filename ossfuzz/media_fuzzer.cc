/*
 * OSS-Fuzz target for libsmraw media value type
 *
 * Copyright (C) 2010-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libsmraw.h"

#if !defined( LIBSMRAW_HAVE_BFIO )

/* Opens a set of storage media RAW files using a Basic File IO (bfio) pool
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_open_file_io_pool(
      libsmraw_handle_t *handle,
      libbfio_pool_t *file_io_pool,
      int access_flags,
      libsmraw_error_t **error );

#endif /* !defined( LIBSMRAW_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	char segment_filename[ 32 ];

	libbfio_handle_t *file_io_handle     = NULL;
	libbfio_pool_t *file_io_pool         = NULL;
	libsmraw_handle_t *handle            = NULL;
	size64_t media_size                 = 0;
	size64_t maximum_segment_size       = 0;
	size_t segment_filename_length      = 0;
	uint8_t media_flags                 = 0;
	uint8_t media_type                  = 0;
	uint32_t bytes_per_sector           = 0;
	uint8_t value_8bit                  = 0;
	uint32_t value_32bit                = 0;
	size_t filename_index               = 0;
	int entry_index                     = 0;

	if( size < 2 )
	{
		return( 0 );
	}
	media_size           = (size64_t) size;
	maximum_segment_size = (size64_t) ( size / 2 );

	if( size > 2 )
	{
		bytes_per_sector = (uint32_t) data[ 0 ] * 512;
		media_type       = data[ 1 ] % 5;
		media_flags      = data[ 2 ] & 1;
	}
	segment_filename_length = size - 1;

	if( segment_filename_length > 31 )
	{
		segment_filename_length = 31;
	}
	for( filename_index = 0;
	     filename_index < segment_filename_length;
	     filename_index++ )
	{
		segment_filename[ filename_index ] = (char ) data[ filename_index ];
	}
	segment_filename[ segment_filename_length ] = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     0,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libbfio_pool_append_handle(
	     file_io_pool,
	     &entry_index,
	     file_io_handle,
	     LIBBFIO_OPEN_WRITE,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	/* The file IO pool takes over management of the file IO handle
	 */
	file_io_handle = NULL;

	if( libsmraw_handle_initialize(
	     &handle,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libsmraw_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     LIBSMRAW_OPEN_WRITE,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_set_media_size(
	     handle,
	     media_size,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_get_media_size(
	     handle,
	     &media_size,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_set_bytes_per_sector(
	     handle,
	     bytes_per_sector,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_get_bytes_per_sector(
	     handle,
	     &value_32bit,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_set_media_type(
	     handle,
	     media_type,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_get_media_type(
	     handle,
	     &value_8bit,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_set_media_flags(
	     handle,
	     media_flags,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_get_media_flags(
	     handle,
	     &value_8bit,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_set_maximum_segment_size(
	     handle,
	     maximum_segment_size,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_get_maximum_segment_size(
	     handle,
	     &maximum_segment_size,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_set_segment_filename(
	     handle,
	     segment_filename,
	     segment_filename_length,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_get_segment_filename_size(
	     handle,
	     &segment_filename_length,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( ( segment_filename_length > 0 )
	 && ( segment_filename_length <= 32 ) )
	{
		if( libsmraw_handle_get_segment_filename(
		     handle,
		     segment_filename,
		     32,
		     NULL ) != 1 )
		{
			goto on_error_libsmraw;
		}
	}
	libsmraw_handle_close(
	 handle,
	 NULL );

 on_error_libsmraw:
	libsmraw_handle_free(
	 &handle,
	 NULL );

 on_error_libbfio:
	/* Note that on error the handle still has a reference to file_io_pool
	 * that will be closed. Therefore the file IO pool and handle need to
	 * be freed after closing or freeing the handle.
	 */
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

} /* extern "C" */
