/* 
 * Verification handle
 *
 * Copyright (C) 2007-2010, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <types.h>

#include <liberror.h>

/* If libtool DLL support is enabled set LIBSMIO_DLL_IMPORT
 * before including libsmio_extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBSMIO_DLL_IMPORT
#endif

#include <libsmio.h>

#include <libsystem.h>

#include "digest_context.h"
#include "digest_hash.h"
#include "md5.h"
#include "sha1.h"
#include "verification_handle.h"

#define VERIFICATION_HANDLE_VALUE_SIZE			64
#define VERIFICATION_HANDLE_VALUE_IDENTIFIER_SIZE	32

/* Initializes the verification handle
 * Returns 1 if successful or -1 on error
 */
int verification_handle_initialize(
     verification_handle_t **verification_handle,
     uint8_t calculate_md5,
     uint8_t calculate_sha1,
     liberror_error_t **error )
{
	static char *function = "verification_handle_initialize";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( *verification_handle == NULL )
	{
		*verification_handle = (verification_handle_t *) memory_allocate(
								  sizeof( verification_handle_t ) );

		if( *verification_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create verification handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *verification_handle,
		     0,
		     sizeof( verification_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear verification handle.",
			 function );

			memory_free(
			 *verification_handle );

			*verification_handle = NULL;

			return( -1 );
		}
		( *verification_handle )->calculate_md5  = calculate_md5;
		( *verification_handle )->calculate_sha1 = calculate_sha1;

		if( ( ( *verification_handle )->calculate_md5 != 0 )
		 && ( md5_initialize(
		       &( ( *verification_handle )->md5_context ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize MD5 context.",
			 function );

			memory_free(
			 *verification_handle );

			*verification_handle = NULL;

			return( -1 );
		}
		if( ( ( *verification_handle )->calculate_sha1 != 0 )
		 && ( sha1_initialize(
		       &( ( *verification_handle )->sha1_context ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize SHA1 context.",
			 function );

			memory_free(
			 *verification_handle );

			*verification_handle = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the verification handle and its elements
 * Returns 1 if successful or -1 on error
 */
int verification_handle_free(
     verification_handle_t **verification_handle,
     liberror_error_t **error )
{
	static char *function = "verification_handle_free";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( *verification_handle != NULL )
	{
		if( ( ( *verification_handle )->input_handle != NULL )
		 && ( libsmio_handle_free(
		       &( ( *verification_handle )->input_handle ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input handle.",
			 function );
		}
		memory_free(
		 *verification_handle );

		*verification_handle = NULL;
	}
	return( 1 );
}

/* Signals the verification handle to abort
 * Returns 1 if successful or -1 on error
 */
int verification_handle_signal_abort(
     verification_handle_t *verification_handle,
     liberror_error_t **error )
{
	static char *function = "verification_handle_signal_abort";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid verification handle - input handle already set.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_signal_abort(
	     verification_handle->input_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to signal input handle to abort.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the input of the verification handle
 * Returns 1 if successful or -1 on error
 */
int verification_handle_open_input(
     verification_handle_t *verification_handle,
     libsystem_character_t * const * filenames,
     int amount_of_filenames,
     liberror_error_t **error )
{
	libsystem_character_t **libsmio_filenames = NULL;
	static char *function                     = "verification_handle_open_input";
	size_t first_filename_length              = 0;
	int result                                = 1;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid verification handle - input handle already set.",
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
	if( amount_of_filenames <= 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid amount of filenames.",
		 function );

		return( -1 );
	}
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
	if( libsmio_detect_type_wide(
	     filenames[ 0 ],
	     &( verification_handle->input_storage_media_type ),
	     error ) != 1 )
#else
	if( libsmio_detect_type(
	     filenames[ 0 ],
	     &( verification_handle->input_storage_media_type ),
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine input storage media type.",
		 function );

		return( -1 );
	}
	switch( verification_handle->input_storage_media_type )
	{
#if defined( LIBSMIO_AFF_SUPPORT )
		case LIBSMIO_TYPE_AFF:
			result = libsmio_aff_initialize(
			          &( verification_handle->input_handle ),
			          error );

			break;
#endif
		case LIBSMIO_TYPE_EWF:
			result = libsmio_ewf_initialize(
			          &( verification_handle->input_handle ),
			          error );
			break;

		case LIBSMIO_TYPE_RAW:
			result = libsmio_raw_initialize(
			          &( verification_handle->input_handle ),
			          error );
			break;
	}
	if( ( result != 1 )
	 || ( verification_handle->input_handle == NULL ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: unable to initialize input handle.",
		 function );

		return( -1 );
	}
	if( amount_of_filenames == 1 )
	{
		first_filename_length = libsystem_string_length(
		                         filenames[ 0 ] );

#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
		if( libsmio_handle_glob_wide(
		     verification_handle->input_handle,
		     filenames[ 0 ],
		     first_filename_length,
		     &libsmio_filenames,
		     &amount_of_filenames,
		     error ) != 1 )
#else
		if( libsmio_handle_glob(
		     verification_handle->input_handle,
		     filenames[ 0 ],
		     first_filename_length,
		     &libsmio_filenames,
		     &amount_of_filenames,
		     error ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to resolve filename(s).",
			 function );

			return( -1 );
		}
		filenames = (libsystem_character_t * const *) libsmio_filenames;
	}
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
	if( libsmio_handle_open_wide(
	     verification_handle->input_handle,
	     filenames,
	     amount_of_filenames,
	     LIBSMIO_OPEN_READ,
	     error ) != 1 )
#else
	if( libsmio_handle_open(
	     verification_handle->input_handle,
	     filenames,
	     amount_of_filenames,
	     LIBSMIO_OPEN_READ,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open files.",
		 function );

		result = -1;
	}
	if( libsmio_filenames != NULL )
	{
		for( ; amount_of_filenames > 0; amount_of_filenames-- )
		{
			memory_free(
			 libsmio_filenames[ amount_of_filenames - 1 ] );
		}
		memory_free(
		 libsmio_filenames );
	}
	return( result );
}

/* Prepares a buffer after reading the input of the verification handle
 * Returns the resulting buffer size or -1 on error
 */
ssize_t verification_handle_prepare_read_buffer(
         verification_handle_t *verification_handle,
         libsmio_buffer_t *storage_media_buffer,
         liberror_error_t **error )
{
	static char *function = "verification_handle_prepare_read_buffer";
	ssize_t process_count = 0;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
	process_count = libsmio_handle_prepare_read_buffer(
	                 verification_handle->input_handle,
	                 storage_media_buffer,
	                 error );

	if( process_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to prepare storage media buffer after reading.",
		 function );

		return( -1 );
	}
	return( process_count );
}

/* Reads a buffer from the input of the verification handle
 * Returns the amount of bytes written or -1 on error
 */
ssize_t verification_handle_read_buffer(
         verification_handle_t *verification_handle,
         libsmio_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error )
{
	static char *function = "verification_handle_read_buffer";
	ssize_t read_count    = 0;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
	read_count = libsmio_handle_read_buffer(
                      verification_handle->input_handle,
                      storage_media_buffer,
                      read_size,
	              error );

	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read storage media buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Updates the integrity hash(es)
 * Returns 1 if successful or -1 on error
 */
int verification_handle_update_integrity_hash(
     verification_handle_t *verification_handle,
     libsmio_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error )
{
	static char *function = "verification_handle_update_integrity_hash";
	void *data            = NULL;
	size_t data_size      = 0;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( ( read_size == 0 )
	 || ( read_size > (size_t) SSIZE_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid size value out of range.",
		 function );

		return( -1 );
	}
	if( libsmio_buffer_get_data(
	     storage_media_buffer,
	     &data,
	     &data_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine storage media buffer data.",
		 function );

		return( -1 );
	}
	if( verification_handle->calculate_md5 != 0 )
	{
		/* TODO check for return value */
		md5_update(
		 &( verification_handle->md5_context ),
		 data,
		 read_size,
		 error );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update MD5 digest hash.",
			 function );

			return( -1 );
		}
	}
	if( verification_handle->calculate_sha1 != 0 )
	{
		/* TODO check for return value */
		sha1_update(
		 &( verification_handle->sha1_context ),
		 data,
		 read_size,
		 error );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update SHA1 digest hash.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Closes the verification handle
 * Returns the 0 if succesful or -1 on error
 */
int verification_handle_close(
     verification_handle_t *verification_handle,
     liberror_error_t **error )
{
	static char *function = "verification_handle_close";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_close(
	     verification_handle->input_handle,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input handle.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Retrieves several verification values
 * The block size is set to 0 if not available
 * Returns 1 if successful or -1 on error
 */
int verification_handle_get_values(
     verification_handle_t *verification_handle,
     size64_t *media_size,
     size_t *block_size,
     liberror_error_t **error )
{
	static char *function = "verification_handle_get_values";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
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
	if( libsmio_handle_get_media_size(
	     verification_handle->input_handle,
	     media_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_get_bytes_per_sector(
	     verification_handle->input_handle,
	     &( verification_handle->bytes_per_sector ),
	     NULL ) != 1 )
	{
		verification_handle->bytes_per_sector = 512;
	}
	if( libsmio_handle_get_block_size(
	     verification_handle->input_handle,
	     &( verification_handle->block_size ),
	     NULL ) != 1 )
	{
		verification_handle->block_size = 32768;
	}
	*block_size = verification_handle->block_size;

	return( 1 );
}

/* Retrieves the amount of read errors
 * Returns 1 if successful or -1 on error
 */
int verification_handle_get_amount_of_read_errors(
     verification_handle_t *verification_handle,
     int *amount_of_errors,
     liberror_error_t **error )
{
	static char *function = "verification_handle_get_amount_of_read_errors";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( amount_of_errors == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of errors.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_get_amount_of_read_errors(
	     verification_handle->input_handle,
	     amount_of_errors,
	     NULL ) == -1 )
	{
		*amount_of_errors = 0;
	}
	return( 1 );
}

/* Retrieves the hash value from the input handle
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int verification_handle_get_integrity_hash_value(
     verification_handle_t *verification_handle,
     char *integrity_hash_value_identifier,
     size_t integrity_hash_value_identifier_length,
     libsystem_character_t *integrity_hash_value,
     size_t integrity_hash_value_size,
     liberror_error_t **error )
{
	uint8_t utf8_integrity_hash_value[ VERIFICATION_HANDLE_VALUE_SIZE ];

	static char *function                       = "verification_handle_get_integrity_hash_value";
	size_t calculated_integrity_hash_value_size = 0;
	size_t utf8_integrity_hash_value_size       = VERIFICATION_HANDLE_VALUE_SIZE;
	int result                                  = 0;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
	result = libsmio_handle_get_integrity_hash_value(
	          verification_handle->input_handle,
	          (uint8_t *) integrity_hash_value_identifier,
	          integrity_hash_value_identifier_length,
	          utf8_integrity_hash_value,
	          utf8_integrity_hash_value_size,
	          NULL );

	if( result == -1 )
	{
		return( 0 );
	}
	else if( result == 1 )
	{
		/* Determine the hash value size
		 */
		utf8_integrity_hash_value_size = 1 + narrow_string_length(
		                                      (char *) utf8_integrity_hash_value );

		if( libsystem_string_size_from_utf8_string(
		     utf8_integrity_hash_value,
		     utf8_integrity_hash_value_size,
		     &calculated_integrity_hash_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to determine hash value size.",
			 function );

			return( -1 );
		}
		if( integrity_hash_value_size < calculated_integrity_hash_value_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: hash value too small.",
			 function );

			return( -1 );
		}
		if( libsystem_string_copy_from_utf8_string(
		     integrity_hash_value,
		     integrity_hash_value_size,
		     utf8_integrity_hash_value,
		     utf8_integrity_hash_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set hash value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Sets the input values of the verification handle
 * Returns 1 if successful or -1 on error
 */
int verification_handle_set_input_values(
     verification_handle_t *verification_handle,
     int wipe_block_on_error,
     liberror_error_t **error )
{
	static char *function = "verification_handle_set_input_values";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
#ifdef TODO
	if( libsmio_handle_set_read_wipe_block_on_error(
	     verification_handle->input_handle,
	     (uint8_t) wipe_block_on_error,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set wipe block on error in input handle.",
		 function );

		return( -1 );
	}
#endif
	verification_handle->wipe_block_on_error = wipe_block_on_error;

	return( 1 );
}

/* Finalizes the verification handle
 * Returns 1 if successful or -1 on error
 */
int verification_handle_finalize(
     verification_handle_t *verification_handle,
     libsystem_character_t *calculated_md5_hash_string,
     size_t calculated_md5_hash_string_size,
     libsystem_character_t *stored_md5_hash_string,
     size_t stored_md5_hash_string_size,
     int *stored_md5_hash_available,
     libsystem_character_t *calculated_sha1_hash_string,
     size_t calculated_sha1_hash_string_size,
     libsystem_character_t *stored_sha1_hash_string,
     size_t stored_sha1_hash_string_size,
     int *stored_sha1_hash_available,
     liberror_error_t **error )
{
	digest_hash_t calculated_md5_hash[ DIGEST_HASH_SIZE_MD5 ];
	digest_hash_t calculated_sha1_hash[ DIGEST_HASH_SIZE_SHA1 ];

	static char *function            = "verification_handle_finalize";
	size_t calculated_md5_hash_size  = DIGEST_HASH_SIZE_MD5;
	size_t calculated_sha1_hash_size = DIGEST_HASH_SIZE_SHA1;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->calculate_md5 != 0 )
	{
		/* Finalize the MD5 hash calculation
		 */
		if( md5_finalize(
		     &( verification_handle->md5_context ),
		     calculated_md5_hash,
		     &calculated_md5_hash_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize MD5 hash.",
			 function );

			return( -1 );
		}
		if( digest_hash_copy_to_string(
		     calculated_md5_hash,
		     calculated_md5_hash_size,
		     calculated_md5_hash_string,
		     calculated_md5_hash_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBSMIO_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set calculated MD5 hash string.",
			 function );

			return( -1 );
		}
		if( stored_md5_hash_available == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid stored MD5 hash available value.",
			 function );

			return( -1 );
		}
		*stored_md5_hash_available = verification_handle_get_integrity_hash_value(
		                              verification_handle,
		                              "MD5",
		                              3,
		                              stored_md5_hash_string,
		                              stored_md5_hash_string_size,
		                              error );

		if( *stored_md5_hash_available == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine stored MD5 hash string.",
			 function );

			return( -1 );
		}
	}
	if( verification_handle->calculate_sha1 != 0 )
	{
		/* Finalize the SHA1 hash calculation
		 */
		if( sha1_finalize(
		     &( verification_handle->sha1_context ),
		     calculated_sha1_hash,
		     &calculated_sha1_hash_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize SHA1 hash.",
			 function );

			return( -1 );
		}
		if( digest_hash_copy_to_string(
		     calculated_sha1_hash,
		     calculated_sha1_hash_size,
		     calculated_sha1_hash_string,
		     calculated_sha1_hash_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create calculated SHA1 hash string.",
			 function );

			return( -1 );
		}
		if( stored_sha1_hash_available == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid stored SHA1 hash available value.",
			 function );

			return( -1 );
		}
		*stored_sha1_hash_available = verification_handle_get_integrity_hash_value(
		                               verification_handle,
		                               "SHA1",
		                               4,
		                               stored_sha1_hash_string,
		                               stored_sha1_hash_string_size,
		                               error );

		if( *stored_sha1_hash_available == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine stored SHA1 hash string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Print the additional hash values to a stream
 * Returns 1 if successful or -1 on error
 */
int verification_handle_additional_integrity_hash_values_fprint(
     verification_handle_t *verification_handle,
     FILE *stream,
     liberror_error_t **error )
{
	char integrity_hash_value_identifier[ VERIFICATION_HANDLE_VALUE_IDENTIFIER_SIZE ];
	libsystem_character_t integrity_hash_value[ VERIFICATION_HANDLE_VALUE_SIZE ];

	static char *function                       = "verification_handle_additional_integrity_hash_values_fprint";
	size_t integrity_hash_value_identifier_size = VERIFICATION_HANDLE_VALUE_IDENTIFIER_SIZE;
	size_t integrity_hash_value_size            = VERIFICATION_HANDLE_VALUE_SIZE;
	uint8_t print_header                        = 1;
	int amount_of_values                        = 0;
	int integrity_hash_value_iterator           = 0;
	int result                                  = 1;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_get_amount_of_integrity_hash_values(
	     verification_handle->input_handle,
	     &amount_of_values,
	     NULL ) == -1 )
	{
		return( 1 );
	}
	for( integrity_hash_value_iterator = 0; integrity_hash_value_iterator < amount_of_values; integrity_hash_value_iterator++ )
	{
		if( libsmio_handle_get_integrity_hash_value_identifier_size(
		     verification_handle->input_handle,
		     integrity_hash_value_iterator,
		     &integrity_hash_value_identifier_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the integrity hash value identifier size for index: %" PRIu32 ".",
			 function,
			 integrity_hash_value_iterator );

			result = -1;

			continue;
		}
		if( integrity_hash_value_identifier_size > VERIFICATION_HANDLE_VALUE_IDENTIFIER_SIZE )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: integrity hash value identifier size value out of range for index: %d.",
			 function,
			 integrity_hash_value_iterator );

			result = -1;

			continue;
		}
		if( libsmio_handle_get_integrity_hash_value_identifier(
		     verification_handle->input_handle,
		     integrity_hash_value_iterator,
		     (uint8_t *) integrity_hash_value_identifier,
		     integrity_hash_value_identifier_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the integrity hash value identifier for index: %" PRIu32 ".",
			 function,
			 integrity_hash_value_iterator );

			result = -1;

			continue;
		}
		if( ( verification_handle->calculate_md5 != 0 )
		 && ( narrow_string_compare(
		       integrity_hash_value_identifier,
		       "MD5",
		       3 ) == 0 ) )
		{
			continue;
		}
		if( ( verification_handle->calculate_sha1 != 0 )
		 && ( narrow_string_compare(
		       integrity_hash_value_identifier,
		       "SHA1",
		       4 ) == 0 ) )
		{
			continue;
		}
		if( verification_handle_get_integrity_hash_value(
		     verification_handle,
		     integrity_hash_value_identifier,
		     integrity_hash_value_identifier_size - 1,
		     integrity_hash_value,
		     integrity_hash_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the hash value for identifier: %s.",
			 function,
			 integrity_hash_value_identifier );

			result = -1;
		}
		else
		{
			if( print_header != 0 )
			{
				fprintf(
				 stream,
				 "\nAdditional hash values:\n" );

				print_header = 0;
			}
			fprintf(
			 stream,
			 "%s:\t%" PRIs_LIBSYSTEM "\n",
			 integrity_hash_value_identifier,
			 integrity_hash_value );
		}
	}
	return( result );
}

/* Print the read errors to a stream
 * Returns 1 if successful or -1 on error
 */
int verification_handle_read_errors_fprint(
     verification_handle_t *verification_handle,
     FILE *stream,
     liberror_error_t **error )
{
	libsystem_character_t *filename      = NULL;
	libsystem_character_t *last_filename = NULL;
	static char *function                = "verification_handle_read_errors_fprint";
	off64_t start_offset                 = 0;
	off64_t end_offset                   = 0;
	size64_t amount_of_bytes             = 0;
	size_t filename_size                 = 0;
	size_t last_filename_size            = 0;
	int amount_of_errors                 = 0;
	int error_iterator                   = 0;
	int result                           = 1;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_get_amount_of_read_errors(
	     verification_handle->input_handle,
	     &amount_of_errors,
	     NULL ) == -1 )
	{
		return( 1 );
	}
	if( amount_of_errors > 0 )
	{
		fprintf( 
		stream,
		 "Validation errors:\n" );
		fprintf(
		 stream,
		 "\ttotal amount: %d\n",
		 amount_of_errors );

		for( error_iterator = 0; error_iterator < amount_of_errors; error_iterator++ )
		{
			if( libsmio_handle_get_read_error(
			     verification_handle->input_handle,
			     error_iterator,
			     &start_offset,
			     &amount_of_bytes,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the read error: %d.",
				 function,
				 error_iterator );

				start_offset    = 0;
				amount_of_bytes = 0;

				result = -1;

				continue;
			}
			end_offset = start_offset + amount_of_bytes;

			fprintf(
			 stream,
			 "\tat offset(s): %" PRIu64 " - %" PRIu64 " (size: %" PRIu64 " bytes)",
			 (uint64_t) start_offset,
			 (uint64_t) end_offset,
			 amount_of_bytes );

			fprintf(
			 stream,
			 " in segment file(s):" );

			while( start_offset < end_offset )
			{
				if( libsmio_handle_seek_offset(
				     verification_handle->input_handle,
				     start_offset,
				     SEEK_SET,
				     error ) == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_SEEK_FAILED,
					 "%s: unable to seek offset: %" PRIu64 ".",
					 function,
					 start_offset );

					if( last_filename != NULL )
					{
						memory_free(
						 last_filename );
					}
					return( -1 );
				}
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
				if( libsmio_handle_get_filename_size_wide(
				     verification_handle->input_handle,
				     &filename_size,
				     error ) != 1 )
#else
				if( libsmio_handle_get_filename_size(
				     verification_handle->input_handle,
				     &filename_size,
				     error ) != 1 )
#endif
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve filename size.",
					 function );

					if( last_filename != NULL )
					{
						memory_free(
						 last_filename );
					}
					return( -1 );
				}
				filename = (libsystem_character_t *) memory_allocate(
				                                      sizeof( libsystem_character_t ) * filename_size ); 


				if( filename == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_MEMORY,
					 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create filename.",
					 function );

					if( last_filename != NULL )
					{
						memory_free(
						 last_filename );
					}
					return( -1 );
				}
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
				if( libsmio_handle_get_filename_wide(
				     verification_handle->input_handle,
				     filename,
				     256,
				     error ) != 1 )
#else
				if( libsmio_handle_get_filename(
				     verification_handle->input_handle,
				     filename,
				     256,
				     error ) != 1 )
#endif
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve filename.",
					 function );

					if( last_filename != NULL )
					{
						memory_free(
						 last_filename );
					}
					memory_free(
					 filename );

					return( -1 );
				}
				if( last_filename == NULL )
				{
					fprintf(
					 stream,
					 " %s",
					 filename );

					last_filename      = filename;
					last_filename_size = filename_size;
				}
				else if( ( last_filename_size != filename_size )
				      || ( memory_compare(
				            last_filename,
				            filename,
				            filename_size ) != 0 ) )
				{
					fprintf(
					 stream,
					 ", %s",
					 filename );

					memory_free(
					 last_filename );

					last_filename      = filename;
					last_filename_size = filename_size;
				}
				start_offset += verification_handle->block_size;
			}
			memory_free(
			 last_filename );

			last_filename      = NULL;
			last_filename_size = 0;

			fprintf(
			 stream,
			 "\n" );
		}
		fprintf(
		 stream,
		 "\n" );
	}
	return( result );
}

