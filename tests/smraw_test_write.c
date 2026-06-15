/*
 * Storage media (SM) RAW library write testing program
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

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "smraw_test_getopt.h"
#include "smraw_test_libcerror.h"
#include "smraw_test_libsmraw.h"

#define SMRAW_TEST_BUFFER_SIZE	4096

/* Copies a string of a decimal value to a 64-bit value
 * Returns 1 if successful or -1 on error
 */
int smraw_test_system_string_decimal_copy_to_64_bit(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	static char *function              = "smraw_test_system_string_decimal_copy_to_64_bit";
	size_t string_index                = 0;
	system_character_t character_value = 0;
	uint8_t maximum_string_index       = 20;
	int8_t sign                        = 1;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_64bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 64-bit.",
		 function );

		return( -1 );
	}
	*value_64bit = 0;

	if( string[ string_index ] == (system_character_t) '-' )
	{
		string_index++;
		maximum_string_index++;

		sign = -1;
	}
	else if( string[ string_index ] == (system_character_t) '+' )
	{
		string_index++;
		maximum_string_index++;
	}
	while( string_index < string_size )
	{
		if( string[ string_index ] == 0 )
		{
			break;
		}
		if( string_index > (size_t) maximum_string_index )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
			 "%s: string too large.",
			 function );

			return( -1 );
		}
		*value_64bit *= 10;

		if( ( string[ string_index ] >= (system_character_t) '0' )
		 && ( string[ string_index ] <= (system_character_t) '9' ) )
		{
			character_value = (system_character_t) ( string[ string_index ] - (system_character_t) '0' );
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: %" PRIc_SYSTEM " at index: %d.",
			 function,
			 string[ string_index ],
			 string_index );

			return( -1 );
		}
		*value_64bit += character_value;

		string_index++;
	}
	if( sign == -1 )
	{
		*value_64bit *= (uint64_t) -1;
	}
	return( 1 );
}

int smraw_test_write(
     const system_character_t *filename,
     size_t media_size,
     size_t maximum_segment_size,
     libcerror_error_t **error )
{
	libsmraw_handle_t *handle = NULL;
	uint8_t *buffer           = NULL;
	static char *function     = "smraw_test_write";
	size_t write_size         = 0;
	ssize_t write_count       = 0;
	int result                = 1;
	int sector_iterator       = 0;

	if( libsmraw_handle_initialize(
	     &handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_handle_open_wide(
	     handle,
	     (wchar_t * const *) &filename,
	     1,
	     LIBSMRAW_OPEN_WRITE,
	     error ) != 1 )
#else
	if( libsmraw_handle_open(
	     handle,
	     (char * const *) &filename,
	     1,
	     LIBSMRAW_OPEN_WRITE,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle.",
		 function );

		goto on_error;
	}
	if( media_size > 0 )
	{
		if( libsmraw_handle_set_media_size(
		     handle,
		     media_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable set media size.",
			 function );

			goto on_error;
		}
	}
	if( libsmraw_handle_set_maximum_segment_size(
	     handle,
	     maximum_segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable set maximum segment size.",
		 function );

		goto on_error;
	}
	buffer = (uint8_t *) memory_allocate(
	                      SMRAW_TEST_BUFFER_SIZE );

	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable create buffer.",
		 function );

		goto on_error;
	}
	write_size = 512;

	for( sector_iterator = 0;
	     sector_iterator < 26;
	     sector_iterator++ )
	{
		if( memory_set(
		     buffer,
		     (int) 'A' + sector_iterator,
		     write_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable set value in buffer.",
			 function );

			goto on_error;
		}
		write_count = libsmraw_handle_write_buffer(
		               handle,
		               buffer,
		               write_size,
		               error );

		if( write_count != (ssize_t) write_size )
		{
			if( write_count != (ssize_t) media_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable write buffer of size: %" PRIzd ".",
				 function,
				 write_size );

				goto on_error;
			}
		}
		if( media_size > 0 )
		{
			media_size -= write_count;
		}
	}
	write_size = 3751;

	for( sector_iterator = 0;
	     sector_iterator < 26;
	     sector_iterator++ )
	{
		if( memory_set(
		     buffer,
		     (int) 'a' + sector_iterator,
		     write_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable set value in buffer.",
			 function );

			goto on_error;
		}
		write_count = libsmraw_handle_write_buffer(
		               handle,
		               buffer,
		               write_size,
		               error );

		if( write_count != (ssize_t) write_size )
		{
			if( write_count != (ssize_t) media_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable write buffer of size: %" PRIzd ".",
				 function,
				 write_size );

				goto on_error;
			}
		}
		if( media_size > 0 )
		{
			media_size -= write_count;
		}
	}
	memory_free(
	 buffer );

	buffer = NULL;

	/* Clean up
	 */
	if( libsmraw_handle_close(
	     handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close handle.",
		 function );

		goto on_error;
	}
	if( libsmraw_handle_free(
	     &handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free handle.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( buffer != NULL )
	{
		memory_free(
		 buffer );
	}
	if( handle != NULL )
	{
		libsmraw_handle_close(
		 handle,
		 NULL );
		libsmraw_handle_free(
		 &handle,
		 NULL );
	}
	return( -1 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libsmraw_error_t *error                         = NULL;
	system_character_t *option_media_size           = NULL;
	system_character_t *option_maximum_segment_size = NULL;
	system_integer_t option                         = 0;
	size64_t maximum_segment_size                   = 0;
	size64_t media_size                             = 0;
	size_t string_length                            = 0;

	while( ( option = smraw_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "B:S:" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );

			case (system_integer_t) 'B':
				option_media_size = optarg;

				break;

			case (system_integer_t) 'S':
				option_maximum_segment_size = optarg;

				break;
		}
	}
	if( ( optind == argc )
	 || ( argv[ optind ] == NULL ) )
	{
		fprintf(
		 stderr,
		 "Missing storage media image filename.\n" );

		return( EXIT_FAILURE );
	}
	if( option_maximum_segment_size != NULL )
	{
		string_length = system_string_length(
				 option_maximum_segment_size );

		if( smraw_test_system_string_decimal_copy_to_64_bit(
		     option_maximum_segment_size,
		     string_length + 1,
		     &maximum_segment_size,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unsupported maximum segment size.\n" );

			goto on_error;
		}
	}
	if( option_media_size != NULL )
	{
		string_length = system_string_length(
				 option_media_size );

		if( smraw_test_system_string_decimal_copy_to_64_bit(
		     option_media_size,
		     string_length + 1,
		     &media_size,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unsupported media size.\n" );

			goto on_error;
		}
	}
	if( smraw_test_write(
	     argv[ optind ],
	     media_size,
	     maximum_segment_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );
		libsmraw_error_free(
		 &error );
	}
	return( EXIT_FAILURE );
}

