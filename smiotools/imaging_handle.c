/* 
 * Imaging handle
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

#include "digest_context.h"
#include "digest_hash.h"
#include "smiocommon.h"
#include "imaging_handle.h"
#include "md5.h"
#include "sha1.h"

#if !defined( USE_LIBSMIO_GET_HASH_VALUE_MD5 ) && !defined( USE_LIBEWF_GET_MD5_HASH )
#define USE_LIBSMIO_GET_HASH_VALUE_MD5
#endif

/* Initializes the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_initialize(
     imaging_handle_t **imaging_handle,
     uint8_t calculate_md5,
     uint8_t calculate_sha1,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_initialize";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( *imaging_handle == NULL )
	{
		*imaging_handle = (imaging_handle_t *) memory_allocate(
		                                        sizeof( imaging_handle_t ) );

		if( *imaging_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create imaging handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *imaging_handle,
		     0,
		     sizeof( imaging_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear imaging handle.",
			 function );

			memory_free(
			 *imaging_handle );

			*imaging_handle = NULL;

			return( -1 );
		}
		( *imaging_handle )->calculate_md5  = calculate_md5;
		( *imaging_handle )->calculate_sha1 = calculate_sha1;

		if( ( ( *imaging_handle )->calculate_md5 != 0 )
		 && ( md5_initialize(
		       &( ( *imaging_handle )->md5_context ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize MD5 context.",
			 function );

			memory_free(
			 *imaging_handle );

			*imaging_handle = NULL;

			return( -1 );
		}
		if( ( ( *imaging_handle )->calculate_sha1 != 0 )
		 && ( sha1_initialize(
		       &( ( *imaging_handle )->sha1_context ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize SHA1 context.",
			 function );

			memory_free(
			 *imaging_handle );

			*imaging_handle = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the imaging handle and its elements
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_free(
     imaging_handle_t **imaging_handle,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_free";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( *imaging_handle != NULL )
	{
		if( ( ( *imaging_handle )->output_handle != NULL )
		 && ( libsmio_handle_free(
		       &( ( *imaging_handle )->output_handle ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free output handle.",
			 function );
		}
		memory_free(
		 *imaging_handle );

		*imaging_handle = NULL;
	}
	return( 1 );
}

/* Signals the imaging handle to abort
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_signal_abort(
     imaging_handle_t *imaging_handle,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_signal_abort";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid imaging handle - output handle already set.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_signal_abort(
	     imaging_handle->output_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to signal output handle to abort.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the input of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_open_input(
     imaging_handle_t *imaging_handle,
     const libsystem_character_t *filename,
     liberror_error_t **error )
{
	libsystem_character_t *libsmio_filenames[ 1 ] = { NULL };
	static char *function                         = "imaging_handle_open_input";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->input_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid imaging handle - value already set.",
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
	if( libsmio_device_initialize(
	     &( imaging_handle->input_handle ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize device input handle.",
		 function );

		return( -1 );
	}
	libsmio_filenames[ 0 ] = (libsystem_character_t *) filename;

#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
	if( libsmio_handle_open_wide(
	     imaging_handle->input_handle,
	     (wchar_t * const *) libsmio_filenames,
	     1,
	     LIBSMIO_OPEN_READ,
	     error ) != 1 )
#else
	if( libsmio_handle_open(
	     imaging_handle->input_handle,
	     (char * const *) libsmio_filenames,
	     1,
	     LIBSMIO_OPEN_READ,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file(s).",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the output of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_open_output(
     imaging_handle_t *imaging_handle,
     const libsystem_character_t *filename,
     int output_storage_media_type,
     liberror_error_t **error )
{
	libsystem_character_t *filenames[ 1 ] = { NULL };
	static char *function                 = "imaging_handle_open_output";
	int result                            = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid imaging handle - output handle already set.",
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
	switch( output_storage_media_type )
	{
#if defined( LIBSMIO_AFF_SUPPORT )
		case LIBSMIO_TYPE_AFF:
			result = libsmio_aff_initialize(
			          &( imaging_handle->output_handle ),
			          error );

			break;
#endif
		case LIBSMIO_TYPE_EWF:
			result = libsmio_ewf_initialize(
			          &( imaging_handle->output_handle ),
			          error );
			break;

		case LIBSMIO_TYPE_RAW:
			result = libsmio_raw_initialize(
			          &( imaging_handle->output_handle ),
			          error );
			break;
	}
	if( ( result != 1 )
	 || ( imaging_handle->output_handle == NULL ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: unable to initialize output handle.",
		 function );

		return( -1 );
	}
	filenames[ 0 ] = (libsystem_character_t *) filename;

#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
	if( libsmio_handle_open_wide(
	     imaging_handle->output_handle,
	     filenames,
	     1,
	     LIBSMIO_OPEN_WRITE,
	     error ) != 1 )
#else
	if( libsmio_handle_open(
	     imaging_handle->output_handle,
	     filenames,
	     1,
	     LIBSMIO_OPEN_WRITE,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		return( 1 );
	}
	return( 1 );
}

/* Prepares a buffer after reading the input of the imaging handle
 * Returns the resulting buffer size or -1 on error
 */
ssize_t imaging_handle_prepare_read_buffer(
         imaging_handle_t *imaging_handle,
         libsmio_buffer_t *storage_media_buffer,
         liberror_error_t **error )
{
	static char *function = "imaging_handle_prepare_read_buffer";
	ssize_t process_count = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing input handle.",
		 function );

		return( -1 );
	}
	process_count = libsmio_handle_prepare_read_buffer(
	                 imaging_handle->input_handle,
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

/* Reads a buffer from the input of the imaging handle
 * Returns the amount of bytes written or -1 on error
 */
ssize_t imaging_handle_read_buffer(
         imaging_handle_t *imaging_handle,
         libsmio_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error )
{
	static char *function = "imaging_handle_read_buffer";
	ssize_t read_count    = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing input handle.",
		 function );

		return( -1 );
	}
	read_count = libsmio_handle_read_buffer(
                      imaging_handle->input_handle,
                      storage_media_buffer,
                      read_size,
	              error );

	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Prepares a buffer before writing the output of the imaging handle
 * Returns the resulting buffer size or -1 on error
 */
ssize_t imaging_handle_prepare_write_buffer(
         imaging_handle_t *imaging_handle,
         libsmio_buffer_t *storage_media_buffer,
         liberror_error_t **error )
{
	static char *function = "imaging_handle_prepare_write_buffer";
	ssize_t process_count = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
	process_count = libsmio_handle_prepare_write_buffer(
	                 imaging_handle->output_handle,
	                 storage_media_buffer,
	                 error );

	if( process_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to prepare storage media buffer before writing.",
		 function );

		return( -1 );
	}
	return( process_count );
}

/* Writes a buffer to the output of the imaging handle
 * Returns the amount of bytes written or -1 on error
 */
ssize_t imaging_handle_write_buffer(
         imaging_handle_t *imaging_handle,
         libsmio_buffer_t *storage_media_buffer,
         size_t write_size,
         liberror_error_t **error )
{
	static char *function = "imaging_handle_write_buffer";
	ssize_t write_count   = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
	if( write_size == 0 )
	{
		return( 0 );
	}
	write_count = libsmio_handle_write_buffer(
	               imaging_handle->output_handle,
	               storage_media_buffer,
	               write_size,
	               error );

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write storage media buffer.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Seeks the offset
 * Returns the resulting offset or -1 on error
 */
off64_t imaging_handle_seek_offset(
         imaging_handle_t *imaging_handle,
         off64_t offset,
         liberror_error_t **error )
{
	static char *function = "imaging_handle_seek_offset";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_seek_offset(
	     imaging_handle->input_handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	return( offset );
}

/* Swaps the byte order of byte pairs within a buffer of a certain size
 * Returns 1 if successful, -1 on error
 */
int imaging_handle_swap_byte_pairs(
     imaging_handle_t *imaging_handle,
     libsmio_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error )
{
	uint8_t *data         = NULL;
	static char *function = "imaging_handle_swap_byte_pairs";
	size_t data_size      = 0;
	size_t iterator       = 0;
	uint8_t byte          = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
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
		 "%s: invalid read size value out of range.",
		 function );

		return( -1 );
	}
	if( libsmio_buffer_get_data(
	     storage_media_buffer,
	     (void **) &data,
	     &data_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve buffer data.",
		 function );

		return( -1 );
	}
	/* If the last bit is set the value is odd
	 */
	if( ( read_size & 0x01 ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid read size value is odd.",
		 function );

		return( -1 );
	}
	if( read_size != data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: mismatch in read size and data size.",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < read_size; iterator += 2 )
	{
		byte                 = data[ iterator ];
		data[ iterator ]     = data[ iterator + 1 ];
		data[ iterator + 1 ] = byte;
	}
	return( 1 );
}

/* Updates the integrity hash(es)
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_update_integrity_hash(
     imaging_handle_t *imaging_handle,
     libsmio_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_update_integrity_hash";
	void *data            = NULL;
	size_t data_size      = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
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
		 "%s: unable to retrieve storage media buffer data.",
		 function );

		return( -1 );
	}
	if( imaging_handle->calculate_md5 != 0 )
	{
		/* TODO check for return value */
		md5_update(
		 &( imaging_handle->md5_context ),
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
	if( imaging_handle->calculate_sha1 != 0 )
	{
		/* TODO check for return value */
		sha1_update(
		 &( imaging_handle->sha1_context ),
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

/* Closes the imaging handle
 * Returns the 0 if succesful or -1 on error
 */
int imaging_handle_close(
     imaging_handle_t *imaging_handle,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_close";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_close(
	     imaging_handle->output_handle,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close output handle.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Retrieves the input media size
 * Returns the 1 if succesful or -1 on error
 */
int imaging_handle_get_input_media_size(
     imaging_handle_t *imaging_handle,
     size64_t *media_size,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_get_input_media_size";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_get_media_size(
	     imaging_handle->input_handle,
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
	return( 1 );
}

/* Retrieves the input bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
int imaging_handle_get_input_bytes_per_sector(
     imaging_handle_t *imaging_handle,
     size_t *bytes_per_sector,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_get_input_bytes_per_sector";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_get_bytes_per_sector(
	     imaging_handle->input_handle,
	     bytes_per_sector,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the block size
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_get_block_size(
     imaging_handle_t *imaging_handle,
     size_t *block_size,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_get_block_size";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
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
	if( libsmio_handle_get_block_size(
	     imaging_handle->output_handle,
	     block_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve block size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the output values of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_output_values(
     imaging_handle_t *imaging_handle,
     libsystem_character_t *case_number,
     libsystem_character_t *description,
     libsystem_character_t *evidence_number,
     libsystem_character_t *examiner_name,
     libsystem_character_t *notes,
     libsystem_character_t *acquiry_operating_system,
     libsystem_character_t *acquiry_software,
     libsystem_character_t *acquiry_software_version,
     uint32_t bytes_per_sector,
     size64_t media_size,
     int media_type,
     int volume_type,
     int compression_type,
     int compression_level,
     int compression_flags,
     size64_t maximum_segment_size,
     int maximum_possible_segment_size,
     uint32_t sectors_per_block,
     uint32_t sector_error_granularity,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_set_output_values";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
	/* Set case number
	 */
	if( imaging_handle_set_information_value(
	     imaging_handle,
	     "case_number",
	     11,
	     case_number,
	     libsystem_string_length(
	      case_number ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: case number.",
		 function );

		return( -1 );
	}
	/* Set description
	 */
	if( imaging_handle_set_information_value(
	     imaging_handle,
	     "description",
	     11,
	     description,
	     libsystem_string_length(
	      description ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: description.",
		 function );

		return( -1 );
	}
	/* Set evidence number
	 */
	if( imaging_handle_set_information_value(
	     imaging_handle,
	     "evidence_number",
	     15,
	     evidence_number,
	     libsystem_string_length(
	      evidence_number ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: evidence number.",
		 function );

		return( -1 );
	}
	/* Set examiner name
	 */
	if( imaging_handle_set_information_value(
	     imaging_handle,
	     "examiner_name",
	     13,
	     examiner_name,
	     libsystem_string_length(
	      examiner_name ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: examiner name.",
		 function );

		return( -1 );
	}
	/* Set notes
	 */
	if( imaging_handle_set_information_value(
	     imaging_handle,
	     "notes",
	     5,
	     notes,
	     libsystem_string_length(
	      notes ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: notes.",
		 function );

		return( -1 );
	}
	/* Password is not used within libsmio
	 */

	/* Acquiry date, system date and compression type will be generated automatically when set to NULL
	 */

	/* Set acquiry operating system, software and software version
	 */
	if( ( acquiry_operating_system != NULL )
	 && ( imaging_handle_set_information_value(
	       imaging_handle,
	       "acquiry_operating_system",
	       24,
	       acquiry_operating_system,
	       libsystem_string_length(
	        acquiry_operating_system ),
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: acquiry operating system.",
		 function );

		return( -1 );
	}
	if( imaging_handle_set_information_value(
	     imaging_handle,
	     "acquiry_software",
	     16,
	     acquiry_software,
	     libsystem_string_length(
	      acquiry_software ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: acquiry software.",
		 function );

		return( -1 );
	}
	if( imaging_handle_set_information_value(
	     imaging_handle,
	     "acquiry_software_version",
	     24,
	     acquiry_software_version,
	     libsystem_string_length(
	      acquiry_software_version ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: acquiry software version.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_set_bytes_per_sector(
	     imaging_handle->output_handle,
	     bytes_per_sector,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set bytes per sector.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_set_media_size(
	     imaging_handle->output_handle,
	     media_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media size.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_set_media_values(
	     imaging_handle->output_handle,
	     media_type,
	     volume_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media values.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_set_compression_values(
	     imaging_handle->output_handle,
	     compression_type,
	     compression_level,
	     compression_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set compression values.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_set_maximum_segment_size(
	     imaging_handle->output_handle,
	     maximum_segment_size,
	     maximum_possible_segment_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment file size.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_set_block_size(
	     imaging_handle->output_handle,
	     sectors_per_block * bytes_per_sector,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set block size in output handle.",
		 function );

		return( -1 );
	}
#ifdef TODO
	if( libsmio_handle_set_error_granularity(
	     imaging_handle->output_handle,
	     sector_error_granularity,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set error granularity in output handle.",
		 function );

		return( -1 );
	}
#endif
#ifdef TODO
	/* TODO for now just fake one session
	 */
	if( media_type == LIBSMIO_MEDIA_TYPE_OPTICAL )
	{
		if( media_size > (size_t) UINT32_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid media size value out of range to add session.",
			 function );

			return( -1 );
		}
		if( libsmio_handle_add_session(
		     imaging_handle->output_handle,
		     1,
		     (uint32_t) media_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable add session.",
			 function );

			return( -1 );
		}
	}
#endif
	imaging_handle->bytes_per_sector = bytes_per_sector;

	return( 1 );
}

/* Sets the output EWF format of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_output_ewf_format(
     imaging_handle_t *imaging_handle,
     uint8_t ewf_format,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_set_output_ewf_format";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
	if( libsmio_ewf_set_format(
	     imaging_handle->output_handle,
	     ewf_format,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set EWF format.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the header value in the output handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_information_value(
     imaging_handle_t *imaging_handle,
     char *information_value_identifier,
     size_t information_value_identifier_length,
     libsystem_character_t *information_value,
     size_t information_value_length,
     liberror_error_t **error )
{
	uint8_t *utf8_information_value    = NULL;
	static char *function         = "imaging_handle_set_information_value";
	size_t utf8_information_value_size = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
	if( libsystem_string_size_to_utf8_string(
	     information_value,
	     information_value_length + 1,
	     &utf8_information_value_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine UTF-8 header value size.",
		 function );

		return( -1 );
	}
	utf8_information_value = (uint8_t *) memory_allocate(
	                                 sizeof( uint8_t ) * utf8_information_value_size );

	if( utf8_information_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create UTF-8 header value.",
		 function );

		return( -1 );
	}
	if( libsystem_string_copy_to_utf8_string(
	     information_value,
	     information_value_length + 1,
	     utf8_information_value,
	     utf8_information_value_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-8 header value.",
		 function );

		memory_free(
		 utf8_information_value );

		return( -1 );
	}
	if( libsmio_handle_set_information_value(
	     imaging_handle->output_handle,
	     (uint8_t *) information_value_identifier,
	     information_value_identifier_length,
	     utf8_information_value,
	     utf8_information_value_size - 1,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: %s.",
		 function,
		 information_value_identifier );

		memory_free(
		 utf8_information_value );

		return( -1 );
	}
	memory_free(
	 utf8_information_value );

	return( 1 );
}

/* Sets the hash value in the output handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_integrity_hash_value(
     imaging_handle_t *imaging_handle,
     char *integrity_hash_value_identifier,
     size_t integrity_hash_value_identifier_length,
     libsystem_character_t *integrity_hash_value,
     size_t integrity_hash_value_length,
     liberror_error_t **error )
{
	uint8_t *utf8_integrity_hash_value    = NULL;
	static char *function       = "imaging_handle_set_integrity_hash_value";
	size_t utf8_integrity_hash_value_size = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
	if( libsystem_string_size_to_utf8_string(
	     integrity_hash_value,
	     integrity_hash_value_length + 1,
	     &utf8_integrity_hash_value_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine UTF-8 hash value size.",
		 function );

		return( -1 );
	}
	utf8_integrity_hash_value = (uint8_t *) memory_allocate(
	                               sizeof( uint8_t ) * utf8_integrity_hash_value_size );

	if( utf8_integrity_hash_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create UTF-8 hash value.",
		 function );

		return( -1 );
	}
	if( libsystem_string_copy_to_utf8_string(
	     integrity_hash_value,
	     integrity_hash_value_length + 1,
	     utf8_integrity_hash_value,
	     utf8_integrity_hash_value_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-8 hash value.",
		 function );

		memory_free(
		 utf8_integrity_hash_value );

		return( -1 );
	}
	if( libsmio_handle_set_integrity_hash_value(
	     imaging_handle->output_handle,
	     (uint8_t *) integrity_hash_value_identifier,
	     integrity_hash_value_identifier_length,
	     utf8_integrity_hash_value,
	     utf8_integrity_hash_value_size - 1,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set hash value: %s.",
		 function,
		 integrity_hash_value_identifier );

		memory_free(
		 utf8_integrity_hash_value );

		return( -1 );
	}
	memory_free(
	 utf8_integrity_hash_value );

	return( 1 );
}

/* Appends a read error to the output handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_add_read_error(
      imaging_handle_t *imaging_handle,
      off64_t start_offset,
      size_t amount_of_bytes,
      liberror_error_t **error )
{
	static char *function      = "imaging_handle_add_read_error";
	off64_t start_sector       = 0;
	uint32_t amount_of_sectors = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->bytes_per_sector == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid imaging handle - invalid bytes per sector value out of range.",
		 function );

		return( -1 );
	}
	start_sector      = start_offset / imaging_handle->bytes_per_sector;
	amount_of_sectors = (uint32_t) ( amount_of_bytes / imaging_handle->bytes_per_sector );

	if( libsmio_handle_add_write_error(
	     imaging_handle->output_handle,
	     start_sector,
	     amount_of_sectors,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to add write errror.",
		 function );

		return( -1 );
	}
	return( 1 );
}
 
/* Finalizes the imaging handle
 * Returns the amount of input bytes written or -1 on error
 */
ssize_t imaging_handle_finalize(
         imaging_handle_t *imaging_handle,
         libsystem_character_t *calculated_md5_hash_string,
         size_t calculated_md5_hash_string_size,
         libsystem_character_t *calculated_sha1_hash_string,
         size_t calculated_sha1_hash_string_size,
         liberror_error_t **error )
{
#if defined( USE_LIBSMIO_GET_MD5_HASH )
        digest_hash_t stored_md5_hash[ DIGEST_HASH_SIZE_MD5 ];
#endif

	digest_hash_t calculated_md5_hash[ DIGEST_HASH_SIZE_MD5 ];
	digest_hash_t calculated_sha1_hash[ DIGEST_HASH_SIZE_SHA1 ];

	static char *function            = "imaging_handle_finalize";
	size_t calculated_md5_hash_size  = DIGEST_HASH_SIZE_MD5;
	size_t calculated_sha1_hash_size = DIGEST_HASH_SIZE_SHA1;
	ssize_t write_count              = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->calculate_md5 != 0 )
	{
		/* Finalize the MD5 hash calculation
		 */
		if( md5_finalize(
		     &( imaging_handle->md5_context ),
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
		if( imaging_handle_set_integrity_hash_value(
		     imaging_handle,
		     "MD5",
		     3,
		     calculated_md5_hash_string,
		     calculated_md5_hash_string_size - 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: MD5.",
			 function );

			return( -1 );
		}
	}
	if( imaging_handle->calculate_sha1 != 0 )
	{
		/* Finalize the SHA1 hash calculation
		 */
		if( sha1_finalize(
		     &( imaging_handle->sha1_context ),
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
		if( imaging_handle_set_integrity_hash_value(
		     imaging_handle,
		     "SHA1",
		     5,
		     calculated_sha1_hash_string,
		     calculated_sha1_hash_string_size - 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: SHA1.",
			 function );

			return( -1 );
		}
	}
#ifdef TODO
	write_count = libsmio_handle_write_finalize(
	               imaging_handle->output_handle,
	               error );

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to finalize storage media file(s).",
		 function );

		return( -1 );
	}
#endif
	return( write_count );
}

/* Print the imaging errors to a stream
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_imaging_errors_fprint(
     imaging_handle_t *imaging_handle,
     FILE *stream,
     liberror_error_t **error )
{
	static char *function    = "imaging_handle_imaging_errors_fprint";
	off64_t start_offset     = 0;
	size64_t amount_of_bytes = 0;
	int amount_of_errors     = 0;
	int error_iterator       = 0;
	int result               = 1;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
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
	if( libsmio_handle_get_amount_of_write_errors(
	     imaging_handle->output_handle,
	     &amount_of_errors,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of write errors.",
		 function );

		return( -1 );
	}
	if( amount_of_errors > 0 )
	{
		fprintf(
		 stream,
		 "Read errors during write:\n" );
		fprintf(
		 stream,
		 "\ttotal amount: %d\n",
		 amount_of_errors );
		
		for( error_iterator = 0; error_iterator < amount_of_errors; error_iterator++ )
		{
			if( libsmio_handle_get_write_error(
			     imaging_handle->output_handle,
			     error_iterator,
			     &start_offset,
			     &amount_of_bytes,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the write error: %d.",
				 function,
				 error_iterator );

				start_offset    = 0;
				amount_of_bytes = 0;

				result = -1;
			}
			fprintf(
			 stream,
			 "\tat offset(s): %" PRIu64 " - %" PRIu64 " (size: %" PRIu64 " bytes)\n",
			 (uint64_t) start_offset,
			 (uint64_t) ( start_offset + amount_of_bytes ),
			 amount_of_bytes );
		}
		fprintf(
		 stream,
		 "\n" );
	}
	return( result );
}

