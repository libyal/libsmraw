/*
 * libsmraw interface (API) functions
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
#include <types.h>
#include <wide_string.h>

#include <liberror.h>

#include "libsmraw_definitions.h"
#include "libsmraw_handle.h"
#include "libsmraw_metadata.h"
#include "libsmraw_types.h"

/* Retrieves the media size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_media_size(
     libsmraw_handle_t *handle,
     size64_t *media_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_media_size";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_media_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get media size function.",
		 function );

		return( -1 );
	}
	if( internal_handle->get_media_size(
	     internal_handle->io_handle,
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

/* Sets the media size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_media_size(
     libsmraw_handle_t *handle,
     size64_t media_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_set_media_size";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->set_media_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing set media size function.",
		 function );

		return( -1 );
	}
	if( internal_handle->set_media_size(
	     internal_handle->io_handle,
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
	return( 1 );
}

/* Retrieves the bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_bytes_per_sector(
     libsmraw_handle_t *handle,
     size_t *bytes_per_sector,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_bytes_per_sector";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_bytes_per_sector == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get bytes per sector function.",
		 function );

		return( -1 );
	}
	if( internal_handle->get_bytes_per_sector(
	     internal_handle->io_handle,
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

/* Sets the bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_bytes_per_sector(
     libsmraw_handle_t *handle,
     size_t bytes_per_sector,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_set_bytes_per_sector";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->set_bytes_per_sector == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing set bytes per sector function.",
		 function );

		return( -1 );
	}
	if( internal_handle->set_bytes_per_sector(
	     internal_handle->io_handle,
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
	return( 1 );
}

/* Retrieves the maximum segment size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_maximum_segment_size(
     libsmraw_handle_t *handle,
     size64_t *maximum_segment_size,
     int *maximum_possible,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_maximum_segment_size";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_maximum_segment_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get maximum segment size function.",
		 function );

		return( -1 );
	}
	if( internal_handle->get_maximum_segment_size(
	     internal_handle->io_handle,
	     maximum_segment_size,
	     maximum_possible,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve maximum segment size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the maximum segment size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_maximum_segment_size(
     libsmraw_handle_t *handle,
     size64_t maximum_segment_size,
     int maximum_possible,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_set_maximum_segment_size";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->set_maximum_segment_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing set maximum segment size function.",
		 function );

		return( -1 );
	}
	if( internal_handle->set_maximum_segment_size(
	     internal_handle->io_handle,
	     maximum_segment_size,
	     maximum_possible,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set maximum segment size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the block size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_block_size(
     libsmraw_handle_t *handle,
     size_t *block_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_block_size";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_block_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get block size function.",
		 function );

		return( -1 );
	}
	if( internal_handle->get_block_size(
	     internal_handle->io_handle,
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

/* Sets the block size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_block_size(
     libsmraw_handle_t *handle,
     size_t block_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_set_block_size";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->set_block_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing set block size function.",
		 function );

		return( -1 );
	}
	if( internal_handle->set_block_size(
	     internal_handle->io_handle,
	     block_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set block size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the compression values
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_compression_values(
     libsmraw_handle_t *handle,
     int *type,
     int *level,
     int *flags,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_compression_values";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_compression_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get compression values function.",
		 function );

		return( -1 );
	}
	if( internal_handle->get_compression_values(
	     internal_handle->io_handle,
	     type,
	     level,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve compression values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the compression values
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_compression_values(
     libsmraw_handle_t *handle,
     int type,
     int level,
     int flags,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_set_compression_values";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->set_compression_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing set compression values function.",
		 function );

		return( -1 );
	}
	if( internal_handle->set_compression_values(
	     internal_handle->io_handle,
	     type,
	     level,
	     flags,
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
	return( 1 );
}

/* Retrieves the media values
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_media_values(
     libsmraw_handle_t *handle,
     int *media_type,
     int *volume_type,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_media_values";
	int result                                 = 0;

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get media values function.",
		 function );

		return( -1 );
	}
	result = internal_handle->get_media_values(
	          internal_handle->io_handle,
	          media_type,
	          volume_type,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media values.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the media values
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_media_values(
     libsmraw_handle_t *handle,
     int media_type,
     int volume_type,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_set_media_values";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->set_media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing set media values function.",
		 function );

		return( -1 );
	}
	if( internal_handle->set_media_values(
	     internal_handle->io_handle,
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
	return( 1 );
}

/* Retrieves a filename size
 * The filename size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_filename_size(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_filename_size";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_filename_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get filename size function.",
		 function );

		return( -1 );
	}
	if( internal_handle->get_filename_size(
	     internal_handle->io_handle,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves an information value
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_filename(
     libsmraw_handle_t *handle,
     char *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_filename";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get filename function.",
		 function );

		return( -1 );
	}
	if( internal_handle->get_filename(
	     internal_handle->io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of information values
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_amount_of_information_values(
     libsmraw_handle_t *handle,
     int *amount_of_information_values,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_amount_of_information_values";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_amount_of_information_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get amount of information values function.",
		 function );

		return( -1 );
	}
	if( internal_handle->get_amount_of_information_values(
	     internal_handle->io_handle,
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
     libsmraw_handle_t *handle,
     int information_value_index,
     size_t *identifier_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_information_value_idenfier_size";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_information_value_identifier_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get information value identifier size function.",
		 function );

		return( -1 );
	}
	if( internal_handle->get_information_value_identifier_size(
	     internal_handle->io_handle,
	     information_value_index,
	     identifier_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve information value identifier size.",
		 function );

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
     libsmraw_handle_t *handle,
     int information_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_information_value_identifier";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_information_value_identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get information value identifier function.",
		 function );

		return( -1 );
	}
	if( internal_handle->get_information_value_identifier(
	     internal_handle->io_handle,
	     information_value_index,
	     identifier,
	     identifier_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve information value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves an information value size
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_information_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_information_value_size";
	int result                                 = 0;

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_information_value_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get information value size function.",
		 function );

		return( -1 );
	}
	result = internal_handle->get_information_value_size(
	          internal_handle->io_handle,
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
		 "%s: unable to retrieve information value size.",
		 function );

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
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_information_value";
	int result                                 = 0;

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_information_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get information value function.",
		 function );

		return( -1 );
	}
	result = internal_handle->get_information_value(
	          internal_handle->io_handle,
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
		 "%s: unable to retrieve information value.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets an information value
 * The strings are encoded in UTF-8
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_set_information_value";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->set_information_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing set information value function.",
		 function );

		return( -1 );
	}
	if( internal_handle->set_information_value(
	     internal_handle->io_handle,
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
		 "%s: unable to set information value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of integrity hash values
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_amount_of_integrity_hash_values(
     libsmraw_handle_t *handle,
     int *amount_of_integrity_hash_values,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_amount_of_integrity_hash_values";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_amount_of_integrity_hash_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get amount of integrity_hash values function.",
		 function );

		return( -1 );
	}
	if( internal_handle->get_amount_of_integrity_hash_values(
	     internal_handle->io_handle,
	     amount_of_integrity_hash_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of integrity_hash values.",
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
     libsmraw_handle_t *handle,
     int integrity_hash_value_index,
     size_t *identifier_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_integrity_hash_value_idenfier_size";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_integrity_hash_value_identifier_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get integrity hash value identifier size function.",
		 function );

		return( -1 );
	}
	if( internal_handle->get_integrity_hash_value_identifier_size(
	     internal_handle->io_handle,
	     integrity_hash_value_index,
	     identifier_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve integrity hash value identifier size.",
		 function );

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
     libsmraw_handle_t *handle,
     int integrity_hash_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_integrity_hash_value_identifier";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_integrity_hash_value_identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get integrity hash value identifier function.",
		 function );

		return( -1 );
	}
	if( internal_handle->get_integrity_hash_value_identifier(
	     internal_handle->io_handle,
	     integrity_hash_value_index,
	     identifier,
	     identifier_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve integrity hash value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves an integrity hash value
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_integrity_hash_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_integrity_hash_value_size";
	int result                                 = 0;

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_integrity_hash_value_size != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get integrity hash value size function.",
		 function );

		return( -1 );
	}
	result = internal_handle->get_integrity_hash_value_size(
	          internal_handle->io_handle,
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
		 "%s: unable to retrieve integrity hash value size.",
		 function );

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
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_integrity_hash_value";
	int result                                 = 0;

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_integrity_hash_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get integrity hash value function.",
		 function );

		return( -1 );
	}
	result = internal_handle->get_integrity_hash_value(
	          internal_handle->io_handle,
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
		 "%s: unable to retrieve integrity hash value.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets an integrity hash value
 * The strings are encoded in UTF-8
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_set_integrity_hash_value";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->set_integrity_hash_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing set integrity hash value function.",
		 function );

		return( -1 );
	}
	if( internal_handle->set_integrity_hash_value(
	     internal_handle->io_handle,
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
		 "%s: unable to set integrity hash value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of read errors
 * Returns 1 if successful, 0 if no read errors are present or -1 on error
 */
int libsmraw_handle_get_amount_of_read_errors(
     libsmraw_handle_t *handle,
     int *amount_of_errors,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_amount_of_read_errors";
	int result                                 = 0;

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_amount_of_read_errors == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get amount of read errors function.",
		 function );

		return( -1 );
	}
	result = internal_handle->get_amount_of_read_errors(
	          internal_handle->io_handle,
	          amount_of_errors,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of read errors.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the information of a read error
 * Returns 1 if successful, 0 if no read error could be found or -1 on error
 */
int libsmraw_handle_get_read_error(
     libsmraw_handle_t *handle,
     int index,
     off64_t *offset,
     size64_t *size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_read_error";
	int result                                 = 0;

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_read_error == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get read error function.",
		 function );

		return( -1 );
	}
	result = internal_handle->get_read_error(
	          internal_handle->io_handle,
	          index,
	          offset,
	          size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve read error.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Add a read error
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_add_read_error(
     libsmraw_handle_t *handle,
     off64_t offset,
     size64_t size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_add_read_error";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->add_read_error == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing add read error function.",
		 function );

		return( -1 );
	}
	if( internal_handle->add_read_error(
	     internal_handle->io_handle,
	     offset,
	     size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append read error.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of write errors
 * Returns 1 if successful, 0 if no write errors are present or -1 on error
 */
int libsmraw_handle_get_amount_of_write_errors(
     libsmraw_handle_t *handle,
     int *amount_of_errors,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_amount_of_write_errors";
	int result                                 = 0;

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_amount_of_write_errors == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get amount of write errors function.",
		 function );

		return( -1 );
	}
	result = internal_handle->get_amount_of_write_errors(
	          internal_handle->io_handle,
	          amount_of_errors,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of write errors.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the information of a write error
 * Returns 1 if successful, 0 if no write error could be found or -1 on error
 */
int libsmraw_handle_get_write_error(
     libsmraw_handle_t *handle,
     int index,
     off64_t *offset,
     size64_t *size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_write_error";
	int result                                 = 0;

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_write_error == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get write error function.",
		 function );

		return( -1 );
	}
	result = internal_handle->get_write_error(
	          internal_handle->io_handle,
	          index,
	          offset,
	          size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve read error.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Add a write error
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_add_write_error(
     libsmraw_handle_t *handle,
     off64_t offset,
     size64_t size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_add_write_error";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->add_write_error == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing add write error function.",
		 function );

		return( -1 );
	}
	if( internal_handle->add_write_error(
	     internal_handle->io_handle,
	     offset,
	     size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append write error.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of sessions
 * Returns 1 if successful, 0 if no sessions are present or -1 on error
 */
int libsmraw_handle_get_amount_of_sessions(
     libsmraw_handle_t *handle,
     int *amount_of_sessions,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_amount_of_sessions";
	int result                                 = 0;

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_amount_of_sessions == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get amount of sessions function.",
		 function );

		return( -1 );
	}
	result = internal_handle->get_amount_of_sessions(
	          internal_handle->io_handle,
	          amount_of_sessions,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of sessions.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the information of a session
 * Returns 1 if successful, 0 if no session could be found or -1 on error
 */
int libsmraw_handle_get_session(
     libsmraw_handle_t *handle,
     int index,
     off64_t *offset,
     size64_t *size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_session";
	int result                                 = 0;

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->get_session == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing get session function.",
		 function );

		return( -1 );
	}
	result = internal_handle->get_session(
	          internal_handle->io_handle,
	          index,
	          offset,
	          size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve read error.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Add a session
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_add_session(
     libsmraw_handle_t *handle,
     off64_t offset,
     size64_t size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_add_session";

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
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->add_session == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing add session function.",
		 function );

		return( -1 );
	}
	if( internal_handle->add_session(
	     internal_handle->io_handle,
	     offset,
	     size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append session.",
		 function );

		return( -1 );
	}
	return( 1 );
}

