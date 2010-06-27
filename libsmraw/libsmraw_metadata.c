/*
 * Meta data functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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
#include <types.h>

#include <libcstring.h>
#include <liberror.h>
#include <libnotify.h>

#include "libsmraw_definitions.h"
#include "libsmraw_handle.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_string.h"
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
	static char *function                       = "libsmraw_handle_get_media_size";

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
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
	*media_size = internal_handle->media_size;

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
	static char *function                       = "libsmraw_handle_set_media_size";

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

	if( internal_handle->read_values_initialized != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media size cannot be changed.",
		 function );

		return( -1 );
	}
	internal_handle->media_size = media_size;

	return( 1 );
}

/* Retrieves the bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_bytes_per_sector(
     libsmraw_handle_t *handle,
     uint32_t *bytes_per_sector,
     liberror_error_t **error )
{
	libcstring_character_t value_string[ 11 ];

	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_bytes_per_sector";
	size_t value_string_size                    = 0;
	uint64_t value_64bit                        = 0;
	int result                                  = 0;

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	*bytes_per_sector = 0;

	result = libsmraw_values_table_get_value_size(
	          internal_handle->media_values_table,
	          _LIBCSTRING_STRING( "bytes_per_sector" ),
	          16,
	          &value_string_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media value size for identifier: media_type.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( value_string_size != 11 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported value string size: %" PRIzd ".",
			 function,
			 value_string_size );

			return( -1 );
		}
		if( libsmraw_values_table_get_value(
		     internal_handle->media_values_table,
		     _LIBCSTRING_STRING( "bytes_per_sector" ),
		     16,
		     value_string,
		     11,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media value for identifier: bytes_per_sector.",
			 function );

			return( -1 );
		}
		if( libsmraw_string_copy_to_64bit_hexadecimal(
		     value_string,
		     11,
		     &value_64bit,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy string to 64-bit value.",
			 function );

			return( -1 );
		}
		if( value_64bit > (uint64_t) UINT32_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: 64-bit bytes per sector value out of range.",
			 function );

			return( -1 );
		}
		*bytes_per_sector = (size_t) value_64bit;
	}
	return( 1 );
}

/* Sets the bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_bytes_per_sector(
     libsmraw_handle_t *handle,
     uint32_t bytes_per_sector,
     liberror_error_t **error )
{
	libcstring_character_t value_string[ 11 ];

	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_bytes_per_sector";
	int print_count                             = 0;

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

	if( ( internal_handle->read_values_initialized != 0 )
	 || ( internal_handle->write_values_initialized != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: bytes per sector cannot be changed.",
		 function );

		return( -1 );
	}
	print_count = libcstring_string_snprintf(
	               value_string,
	               11,
	               "0x%08" PRIx32 "",
	               bytes_per_sector );

	if( ( print_count < 0 )
	 || ( print_count > 11 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set bytes per sector value string.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_set_value(
	     internal_handle->media_values_table,
	     _LIBCSTRING_STRING( "bytes_per_sector" ),
	     16,
	     value_string,
	     11,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media value for identifier: bytes_per_sector.",
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
     liberror_error_t **error )
{
	libcstring_character_t value_string[ 10 ];

	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_media_type";
	size_t value_string_size                    = 0;
	int result                                  = 0;

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
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
	*media_type = LIBSMRAW_MEDIA_TYPE_UNKNOWN;

	result = libsmraw_values_table_get_value_size(
	          internal_handle->media_values_table,
	          _LIBCSTRING_STRING( "media_type" ),
	          10,
	          &value_string_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media value size for identifier: media_type.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libsmraw_values_table_get_value(
		     internal_handle->media_values_table,
		     _LIBCSTRING_STRING( "media_type" ),
		     10,
		     value_string,
		     10,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media value for identifier: media_type.",
			 function );

			return( -1 );
		}
		if( value_string_size == 6 )
		{
			if( libcstring_string_compare(
			     _LIBCSTRING_STRING( "fixed" ),
			     value_string,
			     5 ) == 0 )
			{
				*media_type = LIBSMRAW_MEDIA_TYPE_FIXED;
			}
		}
		else if( value_string_size == 7 )
		{
			if( libcstring_string_compare(
			     _LIBCSTRING_STRING( "memory" ),
			     value_string,
			     6 ) == 0 )
			{
				*media_type = LIBSMRAW_MEDIA_TYPE_MEMORY;
			}
		}
		else if( value_string_size == 8 )
		{
			if( libcstring_string_compare(
			     _LIBCSTRING_STRING( "optical" ),
			     value_string,
			     7 ) == 0 )
			{
				*media_type = LIBSMRAW_MEDIA_TYPE_OPTICAL;
			}
		}
		else if( value_string_size == 10 )
		{
			if( libcstring_string_compare(
			     _LIBCSTRING_STRING( "removable" ),
			     value_string,
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
     liberror_error_t **error )
{
	libcstring_character_t value_string[ 10 ];

	libcstring_character_t *result              = NULL;
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_media_type";
	size_t value_string_length                  = 0;

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

	if( internal_handle->read_values_initialized != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media size cannot be changed.",
		 function );

		return( -1 );
	}
	switch( media_type )
	{
		case LIBSMRAW_MEDIA_TYPE_FIXED:
			value_string_length = 5;

			result = libcstring_string_copy(
				  value_string,
				  _LIBCSTRING_STRING( "fixed" ),
				  value_string_length );
			break;

		case LIBSMRAW_MEDIA_TYPE_MEMORY:
			value_string_length = 6;

			result = libcstring_string_copy(
				  value_string,
				  _LIBCSTRING_STRING( "memory" ),
				  value_string_length );
			break;

		case LIBSMRAW_MEDIA_TYPE_OPTICAL:
			value_string_length = 7;

			result = libcstring_string_copy(
				  value_string,
				  _LIBCSTRING_STRING( "optical" ),
				  value_string_length );
			break;

		case LIBSMRAW_MEDIA_TYPE_REMOVABLE:
			value_string_length = 9;

			result = libcstring_string_copy(
				  value_string,
				  _LIBCSTRING_STRING( "removable" ),
				  value_string_length );
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
	value_string[ value_string_length ] = 0;

	if( result == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media type value string.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_set_value(
	     internal_handle->media_values_table,
	     _LIBCSTRING_STRING( "media_type" ),
	     10,
	     value_string,
	     value_string_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media value for identifier: media_type.",
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
     liberror_error_t **error )
{
	libcstring_character_t value_string[ 9 ];

	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_media_flags";
	size_t value_string_size                    = 0;
	int result                                  = 0;

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( media_flags == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media flags.",
		 function );

		return( -1 );
	}
	result = libsmraw_values_table_get_value_size(
	          internal_handle->media_values_table,
	          _LIBCSTRING_STRING( "media_flags" ),
	          11,
	          &value_string_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media value size for identifier: media_flags.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libsmraw_values_table_get_value(
		     internal_handle->media_values_table,
		     _LIBCSTRING_STRING( "media_flags" ),
		     11,
		     value_string,
		     9,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media value for identifier: media_flags.",
			 function );

			return( -1 );
		}
		if( value_string_size == 8 )
		{
			if( libcstring_string_compare(
			     _LIBCSTRING_STRING( "logical" ),
			     value_string,
			     7 ) == 0 )
			{
				*media_flags &= ~( LIBSMRAW_MEDIA_FLAG_PHYSICAL );
			}
		}
		else if( value_string_size == 9 )
		{
			if( libcstring_string_compare(
			     _LIBCSTRING_STRING( "physical" ),
			     value_string,
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
     liberror_error_t **error )
{
	libcstring_character_t value_string[ 9 ];

	libcstring_character_t *result              = NULL;
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_media_flags";
	size_t value_string_length                  = 0;

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

	if( internal_handle->read_values_initialized != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media size cannot be changed.",
		 function );

		return( -1 );
	}
	if( ( media_flags & ~( LIBSMRAW_MEDIA_FLAG_PHYSICAL ) ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported media flags: 0x%02x.",
		 function,
		 media_flags );

		return( -1 );
	}
	if( ( media_flags & LIBSMRAW_MEDIA_FLAG_PHYSICAL ) != 0 )
	{
		value_string_length = 8;

		result = libcstring_string_copy(
			  value_string,
			  _LIBCSTRING_STRING( "physical" ),
			  value_string_length );
	}
	else
	{
		value_string_length = 7;

		result = libcstring_string_copy(
			  value_string,
			  _LIBCSTRING_STRING( "logical" ),
			  value_string_length );
	}
	value_string[ value_string_length ] = 0;

	if( result == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media flags value string.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_set_value(
	     internal_handle->media_values_table,
	     _LIBCSTRING_STRING( "media_flags" ),
	     11,
	     value_string,
	     value_string_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media value for identifier: media_flags.",
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
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_number_of_information_values";

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->information_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing information values table.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_get_number_of_values(
	     internal_handle->information_values_table,
	     number_of_information_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of information values.",
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
	static char *function                       = "libsmraw_handle_get_information_value_identifier_size";

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->information_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing information values table.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_get_identifier_size(
	     internal_handle->information_values_table,
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
     libsmraw_handle_t *handle,
     int information_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_information_value_identifier";

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->information_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing information values table.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_get_identifier(
	     internal_handle->information_values_table,
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
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_information_value_size";
	int result                                  = 0;

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->information_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing information values table.",
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
	          internal_handle->information_values_table,
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
		 "%s: unable to retrieve information value size for identifier: %" PRIs_LIBCSTRING ".",
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
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_information_value";
	int result                                  = 0;

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->information_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing information values table.",
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
	          internal_handle->information_values_table,
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
		 "%s: unable to retrieve information value for identifier: %" PRIs_LIBCSTRING ".",
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
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_information_value";

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

	if( internal_handle->read_values_initialized != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media size cannot be changed.",
		 function );

		return( -1 );
	}
	if( internal_handle->information_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing information values table.",
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
	     internal_handle->information_values_table,
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
		 "%s: unable to set information value for identifier: %" PRIs_LIBCSTRING ".",
		 function,
		 identifier );

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
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_number_of_integrity_hash_values";

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->integrity_hash_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing integrity hash values table.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_get_number_of_values(
	     internal_handle->integrity_hash_values_table,
	     number_of_integrity_hash_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of integrity hash values.",
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
	static char *function                       = "libsmraw_handle_get_integrity_hash_value_identifier_size";

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->integrity_hash_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing integrity hash values table.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_get_identifier_size(
	     internal_handle->integrity_hash_values_table,
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
     libsmraw_handle_t *handle,
     int integrity_hash_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_integrity_hash_value_identifier";

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->integrity_hash_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing integrity hash values table.",
		 function );

		return( -1 );
	}
	if( libsmraw_values_table_get_identifier(
	     internal_handle->integrity_hash_values_table,
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
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_integrity_hash_value_size";
	int result                                  = 0;

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->integrity_hash_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing integrity hash values table.",
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
	          internal_handle->integrity_hash_values_table,
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
		 "%s: unable to retrieve integrity hash value size for identifier: %" PRIs_LIBCSTRING ".",
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
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_integrity_hash_value";
	int result                                  = 0;

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

	if( internal_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->integrity_hash_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing integrity hash values table.",
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
	          internal_handle->integrity_hash_values_table,
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
		 "%s: unable to retrieve integrity hash value for identifier: %" PRIs_LIBCSTRING ".",
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
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     liberror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_integrity_hash_value";

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

	if( internal_handle->read_values_initialized != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media size cannot be changed.",
		 function );

		return( -1 );
	}
	if( internal_handle->integrity_hash_values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing integrity hash values table.",
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
	     internal_handle->integrity_hash_values_table,
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
		 "%s: unable to set integrity hash value for identifier: %" PRIs_LIBCSTRING ".",
		 function,
		 identifier );

		return( -1 );
	}
	return( 1 );
}

