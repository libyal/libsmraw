/* 
 * Info handle
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
#include <byte_stream.h>
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

#include "byte_size_string.h"
#include "digest_hash.h"
#include "guid.h"
#include "info_handle.h"

#define INFO_HANDLE_VALUE_SIZE			512
#define INFO_HANDLE_VALUE_IDENTIFIER_SIZE	64

/* Initializes the info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     liberror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle == NULL )
	{
		*info_handle = (info_handle_t *) memory_allocate(
		                                  sizeof( info_handle_t ) );

		if( *info_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create info handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *info_handle,
		     0,
		     sizeof( info_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear info handle.",
			 function );

			memory_free(
			 *info_handle );

			*info_handle = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the info handle and its elements
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     liberror_error_t **error )
{
	static char *function = "info_handle_free";

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( ( ( *info_handle )->input_handle != NULL )
		 && ( libsmio_handle_free(
		       &( ( *info_handle )->input_handle ),
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
		 *info_handle );

		*info_handle = NULL;
	}
	return( 1 );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     liberror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle - input handle already set.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_signal_abort(
	     info_handle->input_handle,
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

/* Opens the input of the info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_open_input(
     info_handle_t *info_handle,
     libsystem_character_t * const * filenames,
     int amount_of_filenames,
     liberror_error_t **error )
{
	libsystem_character_t **libsmio_filenames = NULL;
	static char *function                     = "info_handle_open_input";
	size_t first_filename_length              = 0;
	int result                                = 1;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle - input handle already set.",
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
	     &( info_handle->input_storage_media_type ),
	     error ) != 1 )
#else
	if( libsmio_detect_type(
	     filenames[ 0 ],
	     &( info_handle->input_storage_media_type ),
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
	switch( info_handle->input_storage_media_type )
	{
#if defined( LIBSMIO_AFF_SUPPORT )
		case LIBSMIO_TYPE_AFF:
			result = libsmio_aff_initialize(
			          &( info_handle->input_handle ),
			          error );

			break;
#endif
		case LIBSMIO_TYPE_EWF:
			result = libsmio_ewf_initialize(
			          &( info_handle->input_handle ),
			          error );
			break;

		case LIBSMIO_TYPE_RAW:
			result = libsmio_raw_initialize(
			          &( info_handle->input_handle ),
			          error );
			break;
	}
	if( ( result != 1 )
	 || ( info_handle->input_handle == NULL ) )
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
		     info_handle->input_handle,
		     filenames[ 0 ],
		     first_filename_length,
		     &libsmio_filenames,
		     &amount_of_filenames,
		     error ) != 1 )
#else
		if( libsmio_handle_glob(
		     info_handle->input_handle,
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
	     info_handle->input_handle,
	     filenames,
	     amount_of_filenames,
	     LIBSMIO_OPEN_READ,
	     error ) != 1 )
#else
	if( libsmio_handle_open(
	     info_handle->input_handle,
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
		 "%s: unable to open file(s).",
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

/* Closes the info handle
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close(
     info_handle_t *info_handle,
     liberror_error_t **error )
{
	static char *function = "info_handle_close";

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( libsmio_handle_close(
	     info_handle->input_handle,
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

/* Retrieves the information value from the input handle
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int info_handle_get_information_value(
     info_handle_t *info_handle,
     char *information_value_identifier,
     size_t information_value_identifier_length,
     libsystem_character_t *information_value,
     size_t information_value_size,
     liberror_error_t **error )
{
	uint8_t utf8_information_value[ INFO_HANDLE_VALUE_SIZE ];

	static char *function                    = "info_handle_get_information_value";
	size_t calculated_information_value_size = 0;
	size_t utf8_information_value_size       = INFO_HANDLE_VALUE_SIZE;
	int result                               = 0;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
	result = libsmio_handle_get_information_value(
	          info_handle->input_handle,
	          (uint8_t *) information_value_identifier,
	          information_value_identifier_length,
	          utf8_information_value,
	          utf8_information_value_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve information value: %s.",
		 function,
		 information_value_identifier );

		return( -1 );
	}
	else if( result == 1 )
	{
		/* Determine the information value size
		 */
		utf8_information_value_size = 1 + narrow_string_length(
		                                   (char *) utf8_information_value );

		if( libsystem_string_size_from_utf8_string(
		     utf8_information_value,
		     utf8_information_value_size,
		     &calculated_information_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to determine information value size.",
			 function );

			return( -1 );
		}
		if( information_value_size < calculated_information_value_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: information value too small.",
			 function );

			return( -1 );
		}
		if( libsystem_string_copy_from_utf8_string(
		     information_value,
		     information_value_size,
		     utf8_information_value,
		     utf8_information_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set information value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the hash value from the input handle
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int info_handle_get_integrity_hash_value(
     info_handle_t *info_handle,
     char *integrity_hash_value_identifier,
     size_t integrity_hash_value_identifier_length,
     libsystem_character_t *integrity_hash_value,
     size_t integrity_hash_value_size,
     liberror_error_t **error )
{
	uint8_t utf8_integrity_hash_value[ INFO_HANDLE_VALUE_SIZE ];

	static char *function                       = "info_handle_get_integrity_hash_value";
	size_t calculated_integrity_hash_value_size = 0;
	size_t utf8_integrity_hash_value_size       = INFO_HANDLE_VALUE_SIZE;
	int result                                  = 0;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
	result = libsmio_handle_get_integrity_hash_value(
	          info_handle->input_handle,
	          (uint8_t *) integrity_hash_value_identifier,
	          integrity_hash_value_identifier_length,
	          utf8_integrity_hash_value,
	          utf8_integrity_hash_value_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value: %s.",
		 function,
		 integrity_hash_value_identifier );

		return( -1 );
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

/* Print the information values to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_information_values_fprint(
     info_handle_t *info_handle,
     FILE *stream,
     liberror_error_t **error )
{
	char information_value_identifier[ INFO_HANDLE_VALUE_IDENTIFIER_SIZE ];
	libsystem_character_t information_value[ INFO_HANDLE_VALUE_SIZE ];

	static char *function                    = "info_handle_information_values_fprint";
	size_t information_value_identifier_size = INFO_HANDLE_VALUE_IDENTIFIER_SIZE;
	size_t information_value_size            = INFO_HANDLE_VALUE_SIZE;
	int amount_of_values                     = 0;
	int information_value_iterator           = 0;
	int information_value_result             = 0;
	int result                               = 1;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
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
	if( libsmio_handle_get_amount_of_information_values(
	     info_handle->input_handle,
	     &amount_of_values,
	     NULL ) != 1 )
	{
		return( 1 );
	}
	fprintf(
	 stream,
	 "Imaging information\n" );

	if( amount_of_values == 0 )
	{
		fprintf(
		 stream,
		 "\tNo information found in file.\n" );
	}
	else
	{
		/* Case number
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "case_number",
		                            11,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tCase number:\t\t%" PRIs_LIBSYSTEM "\n",
			 information_value );
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: case_number.",
			 function );

			result = -1;
		}
		/* Description
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "description",
		                            11,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tDescription:\t\t%" PRIs_LIBSYSTEM "\n",
			 information_value );
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: description.",
			 function );

			result = -1;
		}
		/* Examiner name
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "examiner_name",
		                            13,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tExaminer name:\t\t%" PRIs_LIBSYSTEM "\n",
			 information_value );
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: examiner_name.",
			 function );

			result = -1;
		}
		/* Evidence nubmer
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "evidence_number",
		                            15,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tEvidence number:\t%" PRIs_LIBSYSTEM "\n",
			 information_value );
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: evidence_number.",
			 function );

			result = -1;
		}
		/* Notes
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "notes",
		                            5,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tNotes:\t\t\t%" PRIs_LIBSYSTEM "\n",
			 information_value );
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: notes.",
			 function );

			result = -1;
		}
		/* Acquiry date
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "acquiry_date",
		                            12,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tAcquiry date:\t\t%" PRIs_LIBSYSTEM "\n",
			 information_value );
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: acquiry_date.",
			 function );

			result = -1;
		}
		/* System date
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "system_date",
		                            11,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tSystem date:\t\t%" PRIs_LIBSYSTEM "\n",
			 information_value );
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: system_date.",
			 function );

			result = -1;
		}
		/* Acquiry operating system
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "acquiry_operating_system",
		                            24,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tOperating system used:\t%" PRIs_LIBSYSTEM "\n",
			 information_value );
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: acquiry_operating_system.",
			 function );

			result = -1;
		}
		/* Acquiry software version
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "acquiry_software_version",
		                            24,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tSoftware version used:\t%" PRIs_LIBSYSTEM "\n",
			 information_value );
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: acquiry_software_version.",
			 function );

			result = -1;
		}
		/* Password
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "password",
		                            8,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tPassword:\t\t(hash: %" PRIs_LIBSYSTEM ")\n",
			 information_value );
		}
		else if( information_value_result == 0 )
		{
			fprintf(
			 stream,
			 "\tPassword:\t\tN/A\n" );
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: password.",
			 function );

			result = -1;
		}
		/* Compression type
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "compression_type",
		                            16,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tCompression type:\t" );

			if( libsystem_string_compare(
			     information_value,
			     _LIBSYSTEM_CHARACTER_T_STRING( "n" ),
			     1 ) == 0 )
			{
				fprintf(
				 stream,
				 "none\n" );
			}
			else if( libsystem_string_compare(
				  information_value,
				  _LIBSYSTEM_CHARACTER_T_STRING( "f" ),
				  1 ) == 0 )
			{
				fprintf(
				 stream,
				 "good (fast)\n" );
			}
			else if( libsystem_string_compare(
				  information_value,
				  _LIBSYSTEM_CHARACTER_T_STRING( "b" ),
				  1 ) == 0 )
			{
				fprintf(
				 stream,
				 "best\n" );
			}
			else
			{
				fprintf(
				 stream,
				 "unknown\n" );
			}
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: compression_type.",
			 function );

			result = -1;
		}
		/* Model
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "model",
		                            5,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tModel:\t\t\t%" PRIs_LIBSYSTEM "\n",
			 information_value );
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: model.",
			 function );

			result = -1;
		}
		/* Serial number
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "serial_number",
		                            13,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tSerial number:\t\t%" PRIs_LIBSYSTEM "\n",
			 information_value );
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: serial_number.",
			 function );

			result = -1;
		}
		/* Process identifier
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "process_identifier",
		                            18,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tProcess identifier:\t%" PRIs_LIBSYSTEM "\n",
			 information_value );
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: process_identifier.",
			 function );

			result = -1;
		}
		/* Unknown dc
		 * TODO figure out what this value represents and add get & set API functions to libsmio
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "unknown_dc",
		                            10,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tUnknown value dc:\t%" PRIs_LIBSYSTEM "\n",
			 information_value );
		}
		else if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: unknown_dc.",
			 function );

			result = -1;
		}
		/* Extents
		 */
		information_value_result = info_handle_get_information_value(
		                            info_handle,
		                            "extents",
		                            7,
		                            information_value,
		                            information_value_size,
		                            error );

		if( information_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: extents.",
			 function );

			result = -1;
		}
		else if( ( information_value_result == 1 )
		      && ( info_handle_information_value_extents_fprint(
		            information_value,
		            information_value_size - 1,
		            stream,
		            error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print information value: extents.",
			 function );

			result = -1;
		}
		/* Currently there are 16 default values
		 */
		if( amount_of_values > 16 )
		{
			fprintf(
			 stream,
			 "\n\tAdditional values:\n" );

			for( information_value_iterator = 16; information_value_iterator < amount_of_values; information_value_iterator++ )
			{
				if( libsmio_handle_get_information_value_identifier_size(
				     info_handle->input_handle,
				     information_value_iterator,
				     &information_value_identifier_size,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve the information value identifier size for index: %d.",
					 function,
					 information_value_iterator );

					result = -1;

					continue;
				}
				if( information_value_identifier_size > INFO_HANDLE_VALUE_IDENTIFIER_SIZE )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
					 "%s: information value identifier size value out of range for index: %d.",
					 function,
					 information_value_iterator );

					result = -1;

					continue;
				}
				if( libsmio_handle_get_information_value_identifier(
				     info_handle->input_handle,
				     information_value_iterator,
				     (uint8_t *) information_value_identifier,
				     information_value_identifier_size,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve the header identifier for index: %d.",
					 function,
					 information_value_iterator );

					result = -1;

					continue;
				}
				information_value_result = info_handle_get_information_value(
				                            info_handle,
				                            information_value_identifier,
				                            information_value_identifier_size - 1,
				                            information_value,
				                            information_value_size,
				                            error );

				if( information_value_result == 1 )
				{
					fprintf(
					 stream,
					 "\t%s: %" PRIs_LIBSYSTEM "\n",
					 information_value_identifier,
					 information_value );
				}
				else if( information_value_result == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve the information value for identifier: %s.",
					 function,
					 information_value_identifier );

					result = -1;
				}
			}
		}
	}
	fprintf(
	 stream,
	 "\n" );

	return( result );
}

/* Print the extents information values to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_information_value_extents_fprint(
     libsystem_character_t *information_value,
     size_t information_value_length,
     FILE *stream,
     liberror_error_t **error )
{
	libsystem_split_values_t *extents_elements = NULL;
	static char *function                      = "info_handle_information_value_extents_fprint";
	size_t extents_element_iterator            = 0;

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
	if( libsystem_split_values_parse_string(
	     &extents_elements,
	     information_value,
	     information_value_length,
	     (libsystem_character_t) ' ',
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split information value into elements.",
		 function );

		return( -1 );
	}
	if( ( extents_elements->amount_of_values % 4 ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported amount of extents elements in information value.",
		 function );

		libsystem_split_values_free(
		 &extents_elements,
		 NULL );

		return( -1 );
	}
	fprintf(
	 stream,
	 "\tExtents:\t\t%" PRIs_LIBSYSTEM "\n",
	 extents_elements->values[ 0 ] );

	for( extents_element_iterator = 1;
	     extents_element_iterator < extents_elements->amount_of_values;
	     extents_element_iterator += 4 )
	{
		fprintf(
		 stream,
		 "\t\t\t\t%" PRIs_LIBSYSTEM " %" PRIs_LIBSYSTEM " %" PRIs_LIBSYSTEM " %" PRIs_LIBSYSTEM "\n",
		 extents_elements->values[ extents_element_iterator ],
		 extents_elements->values[ extents_element_iterator + 1 ],
		 extents_elements->values[ extents_element_iterator + 2 ],
		 extents_elements->values[ extents_element_iterator + 3 ] );
	}
	if( libsystem_split_values_free(
	     &extents_elements,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split date time elements.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Print the media information to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_media_information_fprint(
     info_handle_t *info_handle,
     FILE *stream,
     liberror_error_t **error )
{
        libsystem_character_t media_size_string[ 16 ];
        libsystem_character_t guid_string[ GUID_STRING_SIZE ];
        uint8_t guid[ 16 ];

	char *ewf_format_string    = NULL;
	char *type_string          = NULL;
	static char *function      = "info_handle_media_information_fprint";
	size64_t media_size        = 0;
	size_t bytes_per_sector    = 0;
	uint32_t error_granularity = 0;
	int compression_flags      = 0;
	int compression_level      = 0;
	int compression_type       = 0;
	int ewf_format             = 0;
	int media_type             = 0;
	int result                 = 1;
	int volume_type            = 0;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
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
	fprintf(
	 stream,
	 "Storage media information\n" );

	switch( info_handle->input_storage_media_type )
	{
		case LIBSMIO_TYPE_AFF:
			type_string = "Advanced Forensic Format (AFF)";
			break;

		case LIBSMIO_TYPE_DEVICE:
			type_string = "device";
			break;

		case LIBSMIO_TYPE_EWF:
			type_string = "Expert Witness Compression Format (EWF)";
			break;

		case LIBSMIO_TYPE_RAW:
			type_string = "RAW";
			break;

		case LIBSMIO_TYPE_UNDEFINED:
		default:
			type_string = "unknown";
			break;

	}
	fprintf(
	 stdout,
	 "\tStorage media type:\t%s\n",
	 type_string );

	if( info_handle->input_storage_media_type == LIBSMIO_TYPE_EWF )
	{
		if( libsmio_ewf_get_format(
		     info_handle->input_handle,
		     &ewf_format,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EWF format.",
			 function );

			return( -1 );
		}
		else
		{
			switch( ewf_format )
			{
				case LIBSMIO_EWF_FORMAT_EWF:
					ewf_format_string = "original EWF";
					break;

				case LIBSMIO_EWF_FORMAT_SMART:
					ewf_format_string = "SMART";
					break;

				case LIBSMIO_EWF_FORMAT_FTK:
					ewf_format_string = "FTK Imager";
					break;

				case LIBSMIO_EWF_FORMAT_ENCASE1:
					ewf_format_string = "EnCase 1";
					break;

				case LIBSMIO_EWF_FORMAT_ENCASE2:
					ewf_format_string = "EnCase 2";
					break;

				case LIBSMIO_EWF_FORMAT_ENCASE3:
					ewf_format_string = "EnCase 3";
					break;

				case LIBSMIO_EWF_FORMAT_ENCASE4:
					ewf_format_string = "EnCase 4";
					break;

				case LIBSMIO_EWF_FORMAT_ENCASE5:
					ewf_format_string = "EnCase 5";
					break;

				case LIBSMIO_EWF_FORMAT_ENCASE6:
					ewf_format_string = "EnCase 6";
					break;

				case LIBSMIO_EWF_FORMAT_LINEN5:
					ewf_format_string = "linen 5";
					break;

				case LIBSMIO_EWF_FORMAT_LINEN6:
					ewf_format_string = "linen 6";
					break;

				case LIBSMIO_EWF_FORMAT_EWFX:
					ewf_format_string = "EWFX (extended EWF)";
					break;

				case LIBSMIO_EWF_FORMAT_UNDEFINED:
				default:
					ewf_format_string = "unknown";
					break;

			}
			fprintf(
			 stdout,
			 "\tEWF format:\t\t%s\n",
			 ewf_format_string );
		}
	}
	if( libsmio_handle_get_compression_values(
	     info_handle->input_handle,
	     &compression_type,
	     &compression_level,
	     &compression_flags,
	     error ) == 1 )
	{
		if( compression_level == LIBSMIO_COMPRESSION_TYPE_NONE )
		{
			fprintf(
			 stream,
			 "\tCompression type:\tnone\n" );
		}
		else if( compression_level == LIBSMIO_COMPRESSION_TYPE_ZLIB )
		{
			fprintf(
			 stream,
			 "\tCompression type:\tzlib\n" );
		}
		else
		{
			fprintf(
			 stream,
			 "\tCompression type:\tunknown\n" );
		}
		if( compression_level == LIBSMIO_COMPRESSION_LEVEL_NONE )
		{
			fprintf(
			 stream,
			 "\tCompression level:\tnone\n" );
		}
		else if( compression_level == LIBSMIO_COMPRESSION_LEVEL_FAST )
		{
			fprintf(
			 stream,
			 "\tCompression level:\tgood (fast)\n" );
		}
		else if( compression_level == LIBSMIO_COMPRESSION_LEVEL_BEST )
		{
			fprintf(
			 stream,
			 "\tCompression level:\tbest\n" );
		}
		else
		{
			fprintf(
			 stream,
			 "\tCompression type:\tunknown\n" );
		}
		/* TODO: compression flags */
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve compression values.",
		 function );

		result = -1;
	}
	if( info_handle->input_storage_media_type == LIBSMIO_TYPE_EWF )
	{
		if( ( ewf_format == LIBSMIO_EWF_FORMAT_ENCASE5 )
		 || ( ewf_format == LIBSMIO_EWF_FORMAT_ENCASE6 )
		 || ( ewf_format == LIBSMIO_EWF_FORMAT_LINEN5 )
		 || ( ewf_format == LIBSMIO_EWF_FORMAT_LINEN6 )
		 || ( ewf_format == LIBSMIO_EWF_FORMAT_EWFX ) )
		{
			if( libsmio_ewf_get_error_granularity(
			     info_handle->input_handle,
			     &error_granularity,
			     error ) == 1 )
			{
				fprintf(
				 stream,
				 "\tError granularity:\t%" PRIu32 "\n",
				 error_granularity );
			}
			else
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve error granularity.",
				 function );

				result = -1;
			}
			if( libsmio_ewf_get_guid(
			     info_handle->input_handle,
			     guid,
			     16,
			     error ) == 1 )
			{
				if( guid_to_string(
				     guid,
				     16,
				     _BYTE_STREAM_ENDIAN_LITTLE,
				     guid_string,
				     GUID_STRING_SIZE,
				     NULL ) == 1 )
				{
					fprintf(
					 stream,
					 "\tGUID:\t\t\t%" PRIs_LIBSYSTEM "\n",
					 guid_string );
				}
			}
			else
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve GUID.",
				 function );

				result = -1;
			}
		}
	}
	fprintf(
	 stream,
	 "\nMedia information\n" );

	if( libsmio_handle_get_media_values(
	     info_handle->input_handle,
	     &media_type,
	     &volume_type,
	     error ) == 1 )
	{
		if( media_type == LIBSMIO_MEDIA_TYPE_REMOVABLE )
		{
			fprintf(
			 stream,
			 "\tMedia type:\t\tremovable disk\n" );
		}
		else if( media_type == LIBSMIO_MEDIA_TYPE_FIXED )
		{
			fprintf(
			 stream,
			 "\tMedia type:\t\tfixed disk\n" );
		}
		else if( media_type == LIBSMIO_MEDIA_TYPE_OPTICAL )
		{
			fprintf(
			 stream,
			 "\tMedia type:\t\toptical disk (CD/DVD/BD)\n" );
		}
		else if( media_type == LIBSMIO_MEDIA_TYPE_MEMORY )
		{
			fprintf(
			 stream,
			 "\tMedia type:\t\tmemory (RAM)\n" );
		}
		else
		{
			fprintf(
			 stream,
			 "\tMedia type:\t\tunknown (0x%02" PRIx8 ")\n",
			 media_type );
		}
		if( volume_type == LIBSMIO_VOLUME_TYPE_LOGICAL )
		{
			fprintf(
			 stream,
			 "\tVolume type:\t\tlogical\n" );
		}
		else if( volume_type == LIBSMIO_VOLUME_TYPE_PHYSICAL )
		{
			fprintf(
			 stream,
			 "\tVolume type:\t\tphysical\n" );
		}
		else
		{
			fprintf(
			 stream, 
			"\tVolume type:\t\tunknown (0x%02" PRIx8 ")\n",
			 volume_type );
		}
	}
	if( libsmio_handle_get_bytes_per_sector(
	     info_handle->input_handle,
	     &bytes_per_sector,
	     error ) == 1 )
	{
		if( bytes_per_sector == 0 )
		{
			fprintf(
			 stream, 
			"\tBytes per sector:\tunknown (0)\n" );
		}
		else if( libsmio_handle_get_bytes_per_sector(
			  info_handle->input_handle,
			  &bytes_per_sector,
			  error ) == 1 )
		{
			fprintf(
			 stream,
			 "\tBytes per sector:\t%" PRIzd "\n",
			 bytes_per_sector );
		}
	}
	if( libsmio_handle_get_media_size(
	     info_handle->input_handle,
	     &media_size,
	     error ) == 1 )
	{
		result = byte_size_string_create(
			  media_size_string,
			  16,
			  media_size,
			  BYTE_SIZE_STRING_UNIT_MEBIBYTE,
			  NULL );

		if( result == 1 )
		{
			fprintf(
			 stream,
			 "\tMedia size:\t\t%" PRIs_LIBSYSTEM " (%" PRIu64 " bytes)\n",
			 media_size_string, media_size );
		}
		else
		{
			fprintf(
			 stream,
			 "\tMedia size:\t\t%" PRIu64 " bytes\n",
			 media_size );
		}
	}
	fprintf(
	 stream,
	 "\n" );

	return( result );
}

/* Print the integrity hash values to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_integrity_hash_values_fprint(
     info_handle_t *info_handle,
     FILE *stream,
     liberror_error_t **error )
{
	char integrity_hash_value_identifier[ INFO_HANDLE_VALUE_IDENTIFIER_SIZE ];
	libsystem_character_t integrity_hash_value[ INFO_HANDLE_VALUE_SIZE ];

	static char *function                       = "info_handle_integrity_hash_values_fprint";
	size_t integrity_hash_value_identifier_size = INFO_HANDLE_VALUE_IDENTIFIER_SIZE;
	size_t integrity_hash_value_size            = INFO_HANDLE_VALUE_SIZE;
	uint8_t print_header                        = 1;
	int amount_of_values                        = 0;
	int integrity_hash_value_iterator           = 0;
	int result                                  = 1;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
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
	     info_handle->input_handle,
	     &amount_of_values,
	     NULL ) != 1 )
	{
		return( 1 );
	}
	for( integrity_hash_value_iterator = 0; integrity_hash_value_iterator < amount_of_values; integrity_hash_value_iterator++ )
	{
		if( libsmio_handle_get_integrity_hash_value_identifier_size(
		     info_handle->input_handle,
		     integrity_hash_value_iterator,
		     &integrity_hash_value_identifier_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the integrity hash value identifier size for index: %d.",
			 function,
			 integrity_hash_value_iterator );

			result = -1;

			continue;
		}
		if( integrity_hash_value_identifier_size > INFO_HANDLE_VALUE_IDENTIFIER_SIZE )
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
		     info_handle->input_handle,
		     integrity_hash_value_iterator,
		     (uint8_t *) integrity_hash_value_identifier,
		     integrity_hash_value_identifier_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the integrity hash value identifier for index: %d.",
			 function,
			 integrity_hash_value_iterator );

			result = -1;

			continue;
		}
		integrity_hash_value_identifier_size = 1 + narrow_string_length(
		                                            integrity_hash_value_identifier );

		if( narrow_string_compare(
		     integrity_hash_value_identifier,
		     "MD5",
		     3 ) == 0 )
		{
			continue;
		}
		if( info_handle_get_integrity_hash_value(
		     info_handle,
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
			 "%s: unable to retrieve the integrity hash value for identifier: %s.",
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
				 "Digest hash information\n" );

				print_header = 0;
			}
			fprintf(
			 stream,
			 "\t%s:\t\t\t%" PRIs_LIBSYSTEM "\n",
			 integrity_hash_value_identifier,
			 integrity_hash_value );
		}
	}
	if( print_header == 0 )
	{
		fprintf(
		 stream,
		 "\n" );
	}
	return( result );
}

/* Print the imaging errors to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_imaging_errors_fprint(
     info_handle_t *info_handle,
     FILE *stream,
     liberror_error_t **error )
{
	static char *function    = "info_handle_imaging_errors_fprint";
	off64_t start_offset     = 0;
	size64_t amount_of_bytes = 0;
	int amount_of_errors     = 0;
	int error_iterator       = 0;
	int result               = 1;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
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
	     info_handle->input_handle,
	     &amount_of_errors,
	     NULL ) != 1 )
	{
		return( 1 );
	}
	if( amount_of_errors > 0 )
	{
		fprintf(
		 stream,
		 "Read errors during acquiry:\n" );
		fprintf(
		 stream,
		 "\ttotal amount: %d\n",
		 amount_of_errors );
		
		for( error_iterator = 0; error_iterator < amount_of_errors; error_iterator++ )
		{
			if( libsmio_handle_get_write_error(
			     info_handle->input_handle,
			     error_iterator,
			     &start_offset,
			     &amount_of_bytes,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the acquiry error: %d.",
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

/* Print the sessions to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_sessions_fprint(
     info_handle_t *info_handle,
     FILE *stream,
     liberror_error_t **error )
{
	static char *function    = "info_handle_sessions_fprint";
	off64_t start_offset     = 0;
	size64_t amount_of_bytes = 0;
	int amount_of_sessions   = 0;
	int session_iterator     = 0;
	int result               = 1;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
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
	if( libsmio_handle_get_amount_of_sessions(
	     info_handle->input_handle,
	     &amount_of_sessions,
	     NULL ) != 1 )
	{
		return( 1 );
	}
	if( amount_of_sessions > 0 )
	{
		fprintf(
		 stream,
		 "Sessions:\n" );
		fprintf(
		 stream,
		 "\ttotal amount: %d\n",
		 amount_of_sessions );

		for( session_iterator = 0; session_iterator < amount_of_sessions; session_iterator++ )
		{
			if( libsmio_handle_get_session(
			     info_handle->input_handle,
			     session_iterator,
			     &start_offset,
			     &amount_of_bytes,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the session: %d.",
				 function,
				 session_iterator );

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

