/*
 * OSS-Fuzz target for libsmraw information value type
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
	char string[ 64 ];
	uint8_t utf8_string[ 512 ];
	uint16_t utf16_string[ 256 ];

	libbfio_handle_t *file_io_handle      = NULL;
	libbfio_pool_t *file_io_pool          = NULL;
	libsmraw_handle_t *handle             = NULL;
	size_t remaining_size                 = 0;
	size_t identifier_length              = 0;
	size_t identifier_2_length            = 0;
	size_t identifier_size                = 0;
	size_t value_length                   = 0;
	size_t value_2_length                 = 0;
	size_t utf8_string_size               = 0;
	size_t utf16_string_size              = 0;
	int entry_index                       = 0;
	int information_value_index           = 0;
	int number_of_information_values      = 0;

	if( size < 2 )
	{
		return( 0 );
	}

	/* Note that the set functions interpret the passed string length as the
	 * size excluding the end-of-string character and read one character beyond
	 * the passed length, therefore one byte beyond each string needs to be
	 * readable. One byte of the input data is reserved for that purpose.
	 */
	remaining_size  = size - 1;
	identifier_length = remaining_size;

	if( identifier_length > 16 )
	{
		identifier_length = 16;
	}
	remaining_size -= identifier_length;

	value_length = remaining_size;

	if( value_length > 480 )
	{
		value_length = 480;
	}
	remaining_size -= value_length;

	if( remaining_size > 1 )
	{
		identifier_2_length = remaining_size - 1;

		if( identifier_2_length > 16 )
		{
			identifier_2_length = 16;
		}
		value_2_length = remaining_size - identifier_2_length - 1;

		if( value_2_length > 240 )
		{
			value_2_length = 240;
		}
	}
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
	if( libsmraw_handle_set_utf8_information_value(
	     handle,
	     (uint8_t *) data,
	     identifier_length,
	     (uint8_t *) ( data + identifier_length ),
	     value_length,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	if( ( identifier_2_length > 0 )
	 && ( value_2_length > 0 ) )
	{
		size_t value_index = 0;

		for( value_index = 0;
		     value_index < value_2_length;
		     value_index++ )
		{
			utf16_string[ value_index ] = (uint16_t ) data[ identifier_length + value_length + identifier_2_length + value_index ];
		}
		utf16_string[ value_2_length ] = 0;

		if( libsmraw_handle_set_utf16_information_value(
		     handle,
		     (uint8_t *) ( data + identifier_length + value_length ),
		     identifier_2_length,
		     (uint16_t *) utf16_string,
		     value_2_length,
		     NULL ) != 1 )
		{
			goto on_error_libsmraw;
		}
	}
	if( libsmraw_handle_get_number_of_information_values(
	     handle,
	     &number_of_information_values,
	     NULL ) != 1 )
	{
		goto on_error_libsmraw;
	}
	for( information_value_index = 0;
	     information_value_index < number_of_information_values;
	     information_value_index++ )
	{
		identifier_size = 0;

		if( libsmraw_handle_get_information_value_identifier_size(
		     handle,
		     information_value_index,
		     &identifier_size,
		     NULL ) != 1 )
		{
			goto on_error_libsmraw;
		}
		if( ( identifier_size > 0 )
		 && ( identifier_size < 64 ) )
		{
			if( libsmraw_handle_get_information_value_identifier(
			     handle,
			     information_value_index,
			     (uint8_t *) string,
			     identifier_size,
			     NULL ) != 1 )
			{
				goto on_error_libsmraw;
			}
		}
	}
	if( libsmraw_handle_get_utf8_information_value_size(
	     handle,
	     (uint8_t *) data,
	     identifier_length,
	     &utf8_string_size,
	     NULL ) == -1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_get_utf8_information_value(
	     handle,
	     (uint8_t *) data,
	     identifier_length,
	     (uint8_t *) utf8_string,
	     512,
	     NULL ) == -1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_get_utf16_information_value_size(
	     handle,
	     (uint8_t *) data,
	     identifier_length,
	     &utf16_string_size,
	     NULL ) == -1 )
	{
		goto on_error_libsmraw;
	}
	if( libsmraw_handle_get_utf16_information_value(
	     handle,
	     (uint8_t *) data,
	     identifier_length,
	     (uint16_t *) utf16_string,
	     256,
	     NULL ) == -1 )
	{
		goto on_error_libsmraw;
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
