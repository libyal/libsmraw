/*
 * Storage media (SM) RAW library read compare testing program
 *
 * Copyright (c) 2010-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <libcstring.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "digest_hash.h"
#include "smraw_test_libhmac.h"
#include "smraw_test_libsmraw.h"

#define SMRAW_TEST_READ_BUFFER_SIZE	4096

/* Tests libsmraw_handle_seek_offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_seek_offset(
     libsmraw_handle_t *handle,
     off64_t input_offset,
     int input_whence,
     off64_t output_offset )
{
	libsmraw_error_t *error = NULL;
	off64_t result_offset   = 0;
	int result              = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	result_offset = libsmraw_handle_seek_offset(
	                 handle,
	                 input_offset,
	                 input_whence,
	                 &error );

	if( result_offset == -1 )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
	}
	if( result_offset == output_offset )
	{
		result = 1;
	}
	return( result );
}

/* Tests libsmraw_seek_offset and libsmraw_handle_read_buffer and calculates
 * the MD5 integrity hash of the data read
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_read_hash(
     libsmraw_handle_t *handle,
     libhmac_md5_context_t *md5_context,
     off64_t input_offset,
     int input_whence,
     size64_t input_size,
     off64_t output_offset,
     size64_t output_size )
{
	uint8_t buffer[ SMRAW_TEST_READ_BUFFER_SIZE ];

	libsmraw_error_t *error = NULL;
	off64_t result_offset   = 0;
	size64_t remaining_size = 0;
	size64_t result_size    = 0;
	size_t read_size        = 0;
	ssize_t read_count      = 0;
	int result              = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	if( md5_context == NULL )
	{
		return( -1 );
	}
	result_offset = libsmraw_handle_seek_offset(
	                 handle,
	                 input_offset,
	                 input_whence,
	                 &error );

	if( result_offset == -1 )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );

		libsmraw_error_free(
		 &error );
	}
	if( result_offset != output_offset )
	{
		fprintf(
		 stderr,
		 "Unexpected result offset: %" PRIi64 "\n",
		 result_offset );
	}
	else if( result_offset == -1 )
	{
		result = 1;
	}
	else if( result_offset >= 0 )
	{
		remaining_size = input_size;

		while( remaining_size > 0 )
		{
			read_size = SMRAW_TEST_READ_BUFFER_SIZE;

			if( remaining_size < (size64_t) read_size )
			{
				read_size = (size_t) remaining_size;
			}
			read_count = libsmraw_handle_read_buffer(
				      handle,
				      buffer,
				      read_size,
				      &error );

			if( read_count < 0 )
			{
				libsmraw_error_backtrace_fprint(
				 error,
				 stderr );

				libsmraw_error_free(
				 &error );

				break;
			}
			remaining_size -= (size64_t) read_count;
			result_size    += (size64_t) read_count;

			if( libhmac_md5_update(
			     md5_context,
			     buffer,
			     read_count,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to update MD5 context.\n" );

				libsmraw_error_backtrace_fprint(
				 error,
				 stderr );

				libsmraw_error_free(
				 &error );

				return( -1 );
			}
			if( read_count != (ssize_t) read_size )
			{
				break;
			}
		}
		if( output_size == result_size )
		{
			result = 1;
		}
		else
		{
			fprintf(
			 stderr,
			 "Unexpected read count: %" PRIu64 "\n",
			 result_size );
		}
	}
	else
	{
		fprintf(
		 stderr,
		 "Unexpected result offset: %" PRIi64 "\n",
		 result_offset );
	}
	return( result );
}

/* Case 0: test if MD5 hashes of both files match
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_compare_case0(
     libsmraw_handle_t *handle_single,
     libsmraw_handle_t *handle_multi,
      size64_t media_size )
{
	libcstring_system_character_t md5_hash_string[ 33 ];

	uint8_t md5_hash_multi[ LIBHMAC_MD5_HASH_SIZE ];
	uint8_t md5_hash_single[ LIBHMAC_MD5_HASH_SIZE ];

	libhmac_md5_context_t *md5_context_multi  = NULL;
	libhmac_md5_context_t *md5_context_single = NULL;
	libsmraw_error_t *error                   = NULL;
	static char *function                     = "smraw_test_compare_case0";
	int result                                = 0;

	if( libhmac_md5_initialize(
	     &md5_context_single,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize MD5 context.\n" );

		goto on_error;
	}
	if( libhmac_md5_initialize(
	     &md5_context_multi,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize MD5 context.\n" );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Testing compare of single and multiple segment file RAW image of size: %" PRIu64 "\t",
	 media_size );

	result = smraw_test_read_hash(
	          handle_single,
	          md5_context_single,
	          0,
	          SEEK_SET,
	          media_size,
	          0,
	          media_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer from single segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = smraw_test_read_hash(
		  handle_multi,
		  md5_context_multi,
	          0,
	          SEEK_SET,
	          media_size,
	          0,
	          media_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer for multiple segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libhmac_md5_finalize(
	     md5_context_single,
	     md5_hash_single,
	     LIBHMAC_MD5_HASH_SIZE,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to finalize MD5 context.\n" );

		goto on_error;
	}
	if( libhmac_md5_finalize(
	     md5_context_multi,
	     md5_hash_multi,
	     LIBHMAC_MD5_HASH_SIZE,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to finalize MD5 context.\n" );

		goto on_error;
	}
	if( memory_compare(
	     md5_hash_single,
	     md5_hash_multi,
	     LIBHMAC_MD5_HASH_SIZE ) != 0 )
	{
		fprintf(
		 stderr,
		 "Mismatch in MD5 hashes.\n" );

		result = 0;
	}
	if( digest_hash_copy_to_string(
	     md5_hash_single,
	     LIBHMAC_MD5_HASH_SIZE,
	     md5_hash_string,
	     33,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create MD5 hash string.\n" );

		goto on_error;
	}
	if( libhmac_md5_free(
	     &md5_context_single,
	     &error ) != 1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MD5 context.",
		 function );

		goto on_error;
	}
	if( libhmac_md5_free(
	     &md5_context_multi,
	     &error ) != 1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MD5 context.",
		 function );

		goto on_error;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	fprintf(
	 stdout,
	 "MD5: %" PRIs_LIBCSTRING_SYSTEM "\n",
	 md5_hash_string );

	return( result );

on_error:
	if( md5_context_multi != NULL )
	{
		libhmac_md5_free(
		 &md5_context_multi,
		 NULL );
	}
	if( md5_context_single != NULL )
	{
		libhmac_md5_free(
		 &md5_context_single,
		 NULL );
	}
	if( error != NULL )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );
		libsmraw_error_free(
		 &error );
	}
	return( -1 );
}

/* Case 1: Test SEEK_SET and read
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_compare_case1(
     libsmraw_handle_t *handle_single,
     libsmraw_handle_t *handle_multi,
      size64_t media_size )
{
	uint8_t md5_hash_multi[ LIBHMAC_MD5_HASH_SIZE ];
	uint8_t md5_hash_single[ LIBHMAC_MD5_HASH_SIZE ];

	libhmac_md5_context_t *md5_context_multi  = NULL;
	libhmac_md5_context_t *md5_context_single = NULL;
	libsmraw_error_t *error                   = NULL;
	static char *function                     = "smraw_test_compare_case1";
	off64_t read_offset                       = 0;
	size64_t read_size                        = 0;
	int result                                = 0;

	fprintf(
	 stdout,
	 "Testing compare of single and multiple segment file RAW image using SEEK_SET and read\t" );

	while( read_offset < (off64_t) media_size )
	{
		if( libhmac_md5_initialize(
		     &md5_context_single,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to initialize MD5 context.\n" );

			goto on_error;
		}
		if( libhmac_md5_initialize(
		     &md5_context_multi,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to initialize MD5 context.\n" );

			goto on_error;
		}
		read_size = 16 * 1024;

		if( ( (size64_t) read_offset + read_size ) > media_size )
		{
			read_size = media_size - (size64_t) read_offset;
		}
		result = smraw_test_read_hash(
			  handle_single,
			  md5_context_single,
		          read_offset,
		          SEEK_SET,
		          read_size,
		          read_offset,
		          read_size );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to read buffer from single segment file RAW image.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = smraw_test_read_hash(
			  handle_multi,
			  md5_context_multi,
		          read_offset,
		          SEEK_SET,
		          read_size,
		          read_offset,
		          read_size );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to read buffer for multiple segment file RAW image.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libhmac_md5_finalize(
		     md5_context_single,
		     md5_hash_single,
		     LIBHMAC_MD5_HASH_SIZE,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to finalize MD5 context.\n" );

			goto on_error;
		}
		if( libhmac_md5_finalize(
		     md5_context_multi,
		     md5_hash_multi,
		     LIBHMAC_MD5_HASH_SIZE,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to finalize MD5 context.\n" );

			goto on_error;
		}
		if( libhmac_md5_free(
		     &md5_context_single,
		     &error ) != 1 )
		{
			liberror_error_set(
			 &error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MD5 context.",
			 function );

			goto on_error;
		}
		if( libhmac_md5_free(
		     &md5_context_multi,
		     &error ) != 1 )
		{
			liberror_error_set(
			 &error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MD5 context.",
			 function );

			goto on_error;
		}
		if( memory_compare(
		     md5_hash_single,
		     md5_hash_multi,
		     LIBHMAC_MD5_HASH_SIZE ) != 0 )
		{
			fprintf(
			 stderr,
			 "Mismatch in MD5 hashes.\n" );

			result = 0;

			break;
		}
		read_offset += 17 * 1024;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	return( result );

on_error:
	if( md5_context_multi != NULL )
	{
		libhmac_md5_free(
		 &md5_context_multi,
		 NULL );
	}
	if( md5_context_single != NULL )
	{
		libhmac_md5_free(
		 &md5_context_single,
		 NULL );
	}
	if( error != NULL )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );
		libsmraw_error_free(
		 &error );
	}
	return( -1 );
}

/* Case 2: Test SEEK_CUR and read
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_compare_case2(
     libsmraw_handle_t *handle_single,
     libsmraw_handle_t *handle_multi,
      size64_t media_size )
{
	uint8_t md5_hash_multi[ LIBHMAC_MD5_HASH_SIZE ];
	uint8_t md5_hash_single[ LIBHMAC_MD5_HASH_SIZE ];

	libhmac_md5_context_t *md5_context_multi  = NULL;
	libhmac_md5_context_t *md5_context_single = NULL;
	libsmraw_error_t *error                   = NULL;
	static char *function                     = "smraw_test_compare_case2";
	off64_t read_offset                       = 0;
	size64_t read_size                        = 0;
	int result                                = 0;

	fprintf(
	 stdout,
	 "Testing compare of single and multiple segment file RAW image using SEEK_CUR and read\t" );

	result = smraw_test_seek_offset(
	          handle_single,
	          0,
	          SEEK_SET,
	          0 );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to seek offset: 0 of single segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = smraw_test_seek_offset(
	          handle_multi,
	          0,
	          SEEK_SET,
	          0 );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to seek offset: 0 of multiple segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	read_offset = 0;

	while( read_offset < (off64_t) media_size )
	{
		if( libhmac_md5_initialize(
		     &md5_context_single,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to initialize MD5 context.\n" );

			goto on_error;
		}
		if( libhmac_md5_initialize(
		     &md5_context_multi,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to initialize MD5 context.\n" );

			goto on_error;
		}
		read_size    = 16 * 1024;
		read_offset += 17 * 1024;

		if( ( (size64_t) read_offset + read_size ) > media_size )
		{
			read_size = media_size - (size64_t) read_offset;
		}
		result = smraw_test_read_hash(
			  handle_single,
			  md5_context_single,
		          17 * 1024,
		          SEEK_CUR,
		          read_size,
		          read_offset,
		          read_size );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to read buffer from single segment file RAW image.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = smraw_test_read_hash(
			  handle_multi,
			  md5_context_multi,
		          17 * 1024,
		          SEEK_CUR,
		          read_size,
		          read_offset,
		          read_size );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to read buffer for multiple segment file RAW image.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libhmac_md5_finalize(
		     md5_context_single,
		     md5_hash_single,
		     LIBHMAC_MD5_HASH_SIZE,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to finalize MD5 context.\n" );

			goto on_error;
		}
		if( libhmac_md5_finalize(
		     md5_context_multi,
		     md5_hash_multi,
		     LIBHMAC_MD5_HASH_SIZE,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to finalize MD5 context.\n" );

			goto on_error;
		}
		if( libhmac_md5_free(
		     &md5_context_single,
		     &error ) != 1 )
		{
			liberror_error_set(
			 &error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MD5 context.",
			 function );

			goto on_error;
		}
		if( libhmac_md5_free(
		     &md5_context_multi,
		     &error ) != 1 )
		{
			liberror_error_set(
			 &error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MD5 context.",
			 function );

			goto on_error;
		}
		if( memory_compare(
		     md5_hash_single,
		     md5_hash_multi,
		     LIBHMAC_MD5_HASH_SIZE ) != 0 )
		{
			fprintf(
			 stderr,
			 "Mismatch in MD5 hashes.\n" );

			result = 0;

			break;
		}
		read_offset += read_size;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	return( result );

on_error:
	if( md5_context_multi != NULL )
	{
		libhmac_md5_free(
		 &md5_context_multi,
		 NULL );
	}
	if( md5_context_single != NULL )
	{
		libhmac_md5_free(
		 &md5_context_single,
		 NULL );
	}
	if( error != NULL )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );
		libsmraw_error_free(
		 &error );
	}
	return( -1 );
}

/* Case 3: Test SEEK_END and read
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_compare_case3(
     libsmraw_handle_t *handle_single,
     libsmraw_handle_t *handle_multi,
      size64_t media_size )
{
	uint8_t md5_hash_multi[ LIBHMAC_MD5_HASH_SIZE ];
	uint8_t md5_hash_single[ LIBHMAC_MD5_HASH_SIZE ];

	libhmac_md5_context_t *md5_context_multi  = NULL;
	libhmac_md5_context_t *md5_context_single = NULL;
	libsmraw_error_t *error                   = NULL;
	static char *function                     = "smraw_test_compare_case3";
	off64_t read_offset                       = 0;
	size64_t read_size                        = 0;
	int result                                = 0;

	fprintf(
	 stdout,
	 "Testing compare of single and multiple segment file RAW image using SEEK_END and read\t" );

	result = smraw_test_seek_offset(
	          handle_single,
	          0,
	          SEEK_SET,
	          0 );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset of single segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = smraw_test_seek_offset(
	          handle_multi,
	          0,
	          SEEK_SET,
	          0 );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset of multiple segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	read_offset = 9 * 1024;

	while( read_offset < (off64_t) media_size )
	{
		if( libhmac_md5_initialize(
		     &md5_context_single,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to initialize MD5 context.\n" );

			goto on_error;
		}
		if( libhmac_md5_initialize(
		     &md5_context_multi,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to initialize MD5 context.\n" );

			goto on_error;
		}
		read_size = 16 * 1024;

		if( (size64_t) read_offset < read_size )
		{
			read_size = (size64_t) read_offset;
		}
		result = smraw_test_read_hash(
			  handle_single,
			  md5_context_single,
		          -1 * read_offset,
		          SEEK_END,
		          read_size,
		          media_size - read_offset,
		          read_size );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to read buffer from single segment file RAW image.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = smraw_test_read_hash(
			  handle_multi,
			  md5_context_multi,
		          -1 * read_offset,
		          SEEK_END,
		          read_size,
		          media_size - read_offset,
		          read_size );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to read buffer for multiple segment file RAW image.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libhmac_md5_finalize(
		     md5_context_single,
		     md5_hash_single,
		     LIBHMAC_MD5_HASH_SIZE,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to finalize MD5 context.\n" );

			goto on_error;
		}
		if( libhmac_md5_finalize(
		     md5_context_multi,
		     md5_hash_multi,
		     LIBHMAC_MD5_HASH_SIZE,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to finalize MD5 context.\n" );

			goto on_error;
		}
		if( libhmac_md5_free(
		     &md5_context_single,
		     &error ) != 1 )
		{
			liberror_error_set(
			 &error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MD5 context.",
			 function );

			goto on_error;
		}
		if( libhmac_md5_free(
		     &md5_context_multi,
		     &error ) != 1 )
		{
			liberror_error_set(
			 &error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MD5 context.",
			 function );

			goto on_error;
		}
		if( memory_compare(
		     md5_hash_single,
		     md5_hash_multi,
		     LIBHMAC_MD5_HASH_SIZE ) != 0 )
		{
			fprintf(
			 stderr,
			 "Mismatch in MD5 hashes.\n" );

			result = 0;

			break;
		}
		read_offset += 17 * 1024;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	return( result );

on_error:
	if( md5_context_multi != NULL )
	{
		libhmac_md5_free(
		 &md5_context_multi,
		 NULL );
	}
	if( md5_context_single != NULL )
	{
		libhmac_md5_free(
		 &md5_context_single,
		 NULL );
	}
	if( error != NULL )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );
		libsmraw_error_free(
		 &error );
	}
	return( -1 );
}

/* Case 4a: Test SEEK_SET, SEEK_CUR and read
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_compare_case4a(
     libsmraw_handle_t *handle_single,
     libsmraw_handle_t *handle_multi,
      size64_t media_size )
{
	uint8_t md5_hash_multi[ LIBHMAC_MD5_HASH_SIZE ];
	uint8_t md5_hash_single[ LIBHMAC_MD5_HASH_SIZE ];

	libhmac_md5_context_t *md5_context_multi  = NULL;
	libhmac_md5_context_t *md5_context_single = NULL;
	libsmraw_error_t *error                   = NULL;
	static char *function                     = "smraw_test_compare_case4a";
	off64_t read_offset                       = 0;
	size64_t read_size                        = 0;
	int result                                = 0;

	fprintf(
	 stdout,
	 "Testing compare of single and multiple segment file RAW image using SEEK_SET, SEEK_CUR and read\t" );

	if( libhmac_md5_initialize(
	     &md5_context_single,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize MD5 context.\n" );

		goto on_error;
	}
	if( libhmac_md5_initialize(
	     &md5_context_multi,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize MD5 context.\n" );

		goto on_error;
	}
	read_offset = (off64_t) media_size / 2;
	read_size   = media_size / 8;

	result = smraw_test_read_hash(
		  handle_single,
		  md5_context_single,
		  read_offset,
		  SEEK_SET,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer from single segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = smraw_test_read_hash(
		  handle_multi,
		  md5_context_multi,
		  read_offset,
		  SEEK_SET,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer for multiple segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	read_offset += ( media_size / 8 ) + ( media_size / 8 );
	read_size    = media_size / 7;

	result = smraw_test_read_hash(
		  handle_single,
		  md5_context_single,
		  (off64_t) media_size / 8,
		  SEEK_CUR,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer from single segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = smraw_test_read_hash(
		  handle_multi,
		  md5_context_multi,
		  (off64_t) media_size / 8,
		  SEEK_CUR,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer for multiple segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libhmac_md5_finalize(
	     md5_context_single,
	     md5_hash_single,
	     LIBHMAC_MD5_HASH_SIZE,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to finalize MD5 context.\n" );

		goto on_error;
	}
	if( libhmac_md5_finalize(
	     md5_context_multi,
	     md5_hash_multi,
	     LIBHMAC_MD5_HASH_SIZE,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to finalize MD5 context.\n" );

		goto on_error;
	}
	if( memory_compare(
	     md5_hash_single,
	     md5_hash_multi,
	     LIBHMAC_MD5_HASH_SIZE ) != 0 )
	{
		fprintf(
		 stderr,
		 "Mismatch in MD5 hashes.\n" );

		result = 0;
	}
	if( libhmac_md5_free(
	     &md5_context_single,
	     &error ) != 1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MD5 context.",
		 function );

		goto on_error;
	}
	if( libhmac_md5_free(
	     &md5_context_multi,
	     &error ) != 1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MD5 context.",
		 function );

		goto on_error;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	return( result );

on_error:
	if( md5_context_multi != NULL )
	{
		libhmac_md5_free(
		 &md5_context_multi,
		 NULL );
	}
	if( md5_context_single != NULL )
	{
		libhmac_md5_free(
		 &md5_context_single,
		 NULL );
	}
	if( error != NULL )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );
		libsmraw_error_free(
		 &error );
	}
	return( -1 );
}

/* Case 4b: Test SEEK_SET, SEEK_CUR and read
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_compare_case4b(
     libsmraw_handle_t *handle_single,
     libsmraw_handle_t *handle_multi,
      size64_t media_size )
{
	uint8_t md5_hash_multi[ LIBHMAC_MD5_HASH_SIZE ];
	uint8_t md5_hash_single[ LIBHMAC_MD5_HASH_SIZE ];

	libhmac_md5_context_t *md5_context_multi  = NULL;
	libhmac_md5_context_t *md5_context_single = NULL;
	libsmraw_error_t *error                   = NULL;
	static char *function                     = "smraw_test_compare_case4b";
	off64_t read_offset                       = 0;
	size64_t read_size                        = 0;
	int result                                = 0;

	fprintf(
	 stdout,
	 "Testing compare of single and multiple segment file RAW image using SEEK_SET, SEEK_CUR and read\t" );

	if( libhmac_md5_initialize(
	     &md5_context_single,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize MD5 context.\n" );

		goto on_error;
	}
	if( libhmac_md5_initialize(
	     &md5_context_multi,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize MD5 context.\n" );

		goto on_error;
	}
	read_offset = (off64_t) media_size / 2;
	read_size   = media_size / 8;

	result = smraw_test_read_hash(
		  handle_single,
		  md5_context_single,
		  read_offset,
		  SEEK_SET,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer from single segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = smraw_test_read_hash(
		  handle_multi,
		  md5_context_multi,
		  read_offset,
		  SEEK_SET,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer for multiple segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	read_offset += ( media_size / 8 ) - ( media_size / 8 );
	read_size    = media_size / 7;

	result = smraw_test_read_hash(
		  handle_single,
		  md5_context_single,
		  -1 * ( (off64_t) media_size / 8 ),
		  SEEK_CUR,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer from single segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = smraw_test_read_hash(
		  handle_multi,
		  md5_context_multi,
		  -1 * ( (off64_t) media_size / 8 ),
		  SEEK_CUR,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer for multiple segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libhmac_md5_finalize(
	     md5_context_single,
	     md5_hash_single,
	     LIBHMAC_MD5_HASH_SIZE,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to finalize MD5 context.\n" );

		goto on_error;
	}
	if( libhmac_md5_finalize(
	     md5_context_multi,
	     md5_hash_multi,
	     LIBHMAC_MD5_HASH_SIZE,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to finalize MD5 context.\n" );

		goto on_error;
	}
	if( memory_compare(
	     md5_hash_single,
	     md5_hash_multi,
	     LIBHMAC_MD5_HASH_SIZE ) != 0 )
	{
		fprintf(
		 stderr,
		 "Mismatch in MD5 hashes.\n" );

		result = 0;
	}
	if( libhmac_md5_free(
	     &md5_context_single,
	     &error ) != 1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MD5 context.",
		 function );

		goto on_error;
	}
	if( libhmac_md5_free(
	     &md5_context_multi,
	     &error ) != 1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MD5 context.",
		 function );

		goto on_error;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	return( result );

on_error:
	if( md5_context_multi != NULL )
	{
		libhmac_md5_free(
		 &md5_context_multi,
		 NULL );
	}
	if( md5_context_single != NULL )
	{
		libhmac_md5_free(
		 &md5_context_single,
		 NULL );
	}
	if( error != NULL )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );
		libsmraw_error_free(
		 &error );
	}
	return( -1 );
}

/* Case 5a: Test SEEK_END, SEEK_CUR and read
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_compare_case5a(
     libsmraw_handle_t *handle_single,
     libsmraw_handle_t *handle_multi,
      size64_t media_size )
{
	uint8_t md5_hash_multi[ LIBHMAC_MD5_HASH_SIZE ];
	uint8_t md5_hash_single[ LIBHMAC_MD5_HASH_SIZE ];

	libhmac_md5_context_t *md5_context_multi  = NULL;
	libhmac_md5_context_t *md5_context_single = NULL;
	libsmraw_error_t *error                   = NULL;
	static char *function                     = "smraw_test_compare_case5a";
	off64_t read_offset                       = 0;
	size64_t read_size                        = 0;
	int result                                = 0;

	fprintf(
	 stdout,
	 "Testing compare of single and multiple segment file RAW image using SEEK_END, SEEK_CUR and read\t" );

	if( libhmac_md5_initialize(
	     &md5_context_single,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize MD5 context.\n" );

		goto on_error;
	}
	if( libhmac_md5_initialize(
	     &md5_context_multi,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize MD5 context.\n" );

		goto on_error;
	}
	read_offset = (off64_t) media_size / 2;
	read_size   = media_size / 8;

	result = smraw_test_read_hash(
		  handle_single,
		  md5_context_single,
		  -1 * read_offset,
		  SEEK_END,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer from single segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = smraw_test_read_hash(
		  handle_multi,
		  md5_context_multi,
		  -1 * read_offset,
		  SEEK_END,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer for multiple segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	read_offset += ( media_size / 8 ) + ( media_size / 8 );
	read_size    = media_size / 7;

	result = smraw_test_read_hash(
		  handle_single,
		  md5_context_single,
		  (off64_t) media_size / 8,
		  SEEK_CUR,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer from single segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = smraw_test_read_hash(
		  handle_multi,
		  md5_context_multi,
		  (off64_t) media_size / 8,
		  SEEK_CUR,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer for multiple segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libhmac_md5_finalize(
	     md5_context_single,
	     md5_hash_single,
	     LIBHMAC_MD5_HASH_SIZE,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to finalize MD5 context.\n" );

		goto on_error;
	}
	if( libhmac_md5_finalize(
	     md5_context_multi,
	     md5_hash_multi,
	     LIBHMAC_MD5_HASH_SIZE,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to finalize MD5 context.\n" );

		goto on_error;
	}
	if( memory_compare(
	     md5_hash_single,
	     md5_hash_multi,
	     LIBHMAC_MD5_HASH_SIZE ) != 0 )
	{
		fprintf(
		 stderr,
		 "Mismatch in MD5 hashes.\n" );

		result = 0;
	}
	if( libhmac_md5_free(
	     &md5_context_single,
	     &error ) != 1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MD5 context.",
		 function );

		goto on_error;
	}
	if( libhmac_md5_free(
	     &md5_context_multi,
	     &error ) != 1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MD5 context.",
		 function );

		goto on_error;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	return( result );

on_error:
	if( md5_context_multi != NULL )
	{
		libhmac_md5_free(
		 &md5_context_multi,
		 NULL );
	}
	if( md5_context_single != NULL )
	{
		libhmac_md5_free(
		 &md5_context_single,
		 NULL );
	}
	if( error != NULL )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );
		libsmraw_error_free(
		 &error );
	}
	return( -1 );
}

/* Case 5b: Test SEEK_END, SEEK_CUR and read
 * Returns 1 if successful, 0 if not or -1 on error
 */
int smraw_test_compare_case5b(
     libsmraw_handle_t *handle_single,
     libsmraw_handle_t *handle_multi,
      size64_t media_size )
{
	uint8_t md5_hash_multi[ LIBHMAC_MD5_HASH_SIZE ];
	uint8_t md5_hash_single[ LIBHMAC_MD5_HASH_SIZE ];

	libhmac_md5_context_t *md5_context_multi  = NULL;
	libhmac_md5_context_t *md5_context_single = NULL;
	libsmraw_error_t *error                   = NULL;
	static char *function                     = "smraw_test_compare_case5b";
	off64_t read_offset                       = 0;
	size64_t read_size                        = 0;
	int result                                = 0;

	fprintf(
	 stdout,
	 "Testing compare of single and multiple segment file RAW image using SEEK_END, SEEK_CUR and read\t" );

	if( libhmac_md5_initialize(
	     &md5_context_single,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize MD5 context.\n" );

		goto on_error;
	}
	if( libhmac_md5_initialize(
	     &md5_context_multi,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize MD5 context.\n" );

		goto on_error;
	}
	read_offset = (off64_t) media_size / 2;
	read_size   = media_size / 8;

	result = smraw_test_read_hash(
		  handle_single,
		  md5_context_single,
		  -1 * read_offset,
		  SEEK_END,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer from single segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = smraw_test_read_hash(
		  handle_multi,
		  md5_context_multi,
		  -1 * read_offset,
		  SEEK_END,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer for multiple segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	read_offset += ( media_size / 8 ) - ( media_size / 8 );
	read_size    = media_size / 7;

	result = smraw_test_read_hash(
		  handle_single,
		  md5_context_single,
		  -1 * ( (off64_t) media_size / 8 ),
		  SEEK_CUR,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer from single segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = smraw_test_read_hash(
		  handle_multi,
		  md5_context_multi,
		  -1 * ( (off64_t) media_size / 8 ),
		  SEEK_CUR,
		  read_size,
		  read_offset,
		  read_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer for multiple segment file RAW image.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libhmac_md5_finalize(
	     md5_context_single,
	     md5_hash_single,
	     LIBHMAC_MD5_HASH_SIZE,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to finalize MD5 context.\n" );

		goto on_error;
	}
	if( libhmac_md5_finalize(
	     md5_context_multi,
	     md5_hash_multi,
	     LIBHMAC_MD5_HASH_SIZE,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to finalize MD5 context.\n" );

		goto on_error;
	}
	if( memory_compare(
	     md5_hash_single,
	     md5_hash_multi,
	     LIBHMAC_MD5_HASH_SIZE ) != 0 )
	{
		fprintf(
		 stderr,
		 "Mismatch in MD5 hashes.\n" );

		result = 0;
	}
	if( libhmac_md5_free(
	     &md5_context_single,
	     &error ) != 1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MD5 context.",
		 function );

		goto on_error;
	}
	if( libhmac_md5_free(
	     &md5_context_multi,
	     &error ) != 1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MD5 context.",
		 function );

		goto on_error;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	return( result );

on_error:
	if( md5_context_multi != NULL )
	{
		libhmac_md5_free(
		 &md5_context_multi,
		 NULL );
	}
	if( md5_context_single != NULL )
	{
		libhmac_md5_free(
		 &md5_context_single,
		 NULL );
	}
	if( error != NULL )
	{
		libsmraw_error_backtrace_fprint(
		 error,
		 stderr );
		libsmraw_error_free(
		 &error );
	}
	return( -1 );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	wchar_t **filenames_multi        = NULL;
#else
	char **filenames_multi           = NULL;
#endif

	libsmraw_error_t *error          = NULL;
	libsmraw_handle_t *handle_multi  = NULL;
	libsmraw_handle_t *handle_single = NULL;
	size64_t media_size_multi        = 0;
	size64_t media_size_single       = 0;
	int number_of_filenames_multi    = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing single filename.\n" );

		return( EXIT_FAILURE );
	}
	if( argc < 3 )
	{
		fprintf(
		 stderr,
		 "Missing basename of mutiple files.\n" );

		return( EXIT_FAILURE );
	}
	/* Initialization single
	 */
	if( libsmraw_handle_initialize(
	     &handle_single,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create handle.\n" );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_handle_open_wide(
	     handle_single,
	     &( argv[ 1 ] ),
	     1,
	     LIBSMRAW_OPEN_READ,
	     &error ) != 1 )
#else
	if( libsmraw_handle_open(
	     handle_single,
	     &( argv[ 1 ] ),
	     1,
	     LIBSMRAW_OPEN_READ,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open file(s).\n" );

		goto on_error;
	}
	if( libsmraw_handle_get_media_size(
	     handle_single,
	     &media_size_single,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve media size.\n" );

		goto on_error;
	}
	if( media_size_single > (size64_t) INT64_MAX )
	{
		fprintf(
		 stderr,
		 "Media size exceeds maximum.\n" );

		goto on_error;
	}
	/* Initialization multi
	 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_glob_wide(
	     argv[ 2 ],
	     libcstring_wide_string_length(
	      argv[ 2 ] ),
	     &filenames_multi,
	     &number_of_filenames_multi,
	     &error ) != 1 )
#else
	if( libsmraw_glob(
	     argv[ 2 ],
	     libcstring_narrow_string_length(
	      argv[ 2 ] ),
	     &filenames_multi,
	     &number_of_filenames_multi,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to glob filenames.\n" );

		goto on_error;
	}
	if( libsmraw_handle_initialize(
	     &handle_multi,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create handle.\n" );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		libsmraw_glob_wide_free(
		 filenames_multi,
		 number_of_filenames_multi,
		 NULL );
#else
		libsmraw_glob_free(
		 filenames_multi,
		 number_of_filenames_multi,
		 NULL );
#endif
		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_handle_open_wide(
	     handle_multi,
	     filenames_multi,
	     number_of_filenames_multi,
	     LIBSMRAW_OPEN_READ,
	     &error ) != 1 )
#else
	if( libsmraw_handle_open(
	     handle_multi,
	     filenames_multi,
	     number_of_filenames_multi,
	     LIBSMRAW_OPEN_READ,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open file(s).\n" );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		libsmraw_glob_wide_free(
		 filenames_multi,
		 number_of_filenames_multi,
		 NULL );
#else
		libsmraw_glob_free(
		 filenames_multi,
		 number_of_filenames_multi,
		 NULL );
#endif
		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_glob_wide_free(
	     filenames_multi,
	     number_of_filenames_multi,
	     &error ) != 1 )
#else
	if( libsmraw_glob_free(
	     filenames_multi,
	     number_of_filenames_multi,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

		goto on_error;
	}
	if( libsmraw_handle_get_media_size(
	     handle_multi,
	     &media_size_multi,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve media size.\n" );

		goto on_error;
	}
	if( media_size_multi > (size64_t) INT64_MAX )
	{
		fprintf(
		 stderr,
		 "Media size exceeds maximum.\n" );

		goto on_error;
	}
	if( media_size_single != media_size_multi )
	{
		fprintf(
		 stderr,
		 "Mismatch in media size.\n" );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Media size: %" PRIu64 " bytes\n",
	 media_size_single );

	if( smraw_test_compare_case0(
	     handle_single,
	     handle_multi,
	     media_size_single ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test case: 0.\n" );

		goto on_error;
	}
	if( smraw_test_compare_case1(
	     handle_single,
	     handle_multi,
	     media_size_single ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test case: 1.\n" );

		goto on_error;
	}
	if( smraw_test_compare_case2(
	     handle_single,
	     handle_multi,
	     media_size_single ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test case: 2.\n" );

		goto on_error;
	}
	if( smraw_test_compare_case3(
	     handle_single,
	     handle_multi,
	     media_size_single ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test case: 3.\n" );

		goto on_error;
	}
	if( smraw_test_compare_case4a(
	     handle_single,
	     handle_multi,
	     media_size_single ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test case: 4a.\n" );

		goto on_error;
	}
	if( smraw_test_compare_case4b(
	     handle_single,
	     handle_multi,
	     media_size_single ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test case: 4b.\n" );

		goto on_error;
	}
	if( smraw_test_compare_case5a(
	     handle_single,
	     handle_multi,
	     media_size_single ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test case: 5a.\n" );

		goto on_error;
	}
	if( smraw_test_compare_case5b(
	     handle_single,
	     handle_multi,
	     media_size_single ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test case: 5b.\n" );

		goto on_error;
	}
	/* Clean up multi
	 */
	if( libsmraw_handle_close(
	     handle_multi,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file(s).\n" );

		goto on_error;
	}
	if( libsmraw_handle_free(
	     &handle_multi,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free handle.\n" );

		goto on_error;
	}
	/* Clean up single
	 */
	if( libsmraw_handle_close(
	     handle_single,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file(s).\n" );

		goto on_error;
	}
	if( libsmraw_handle_free(
	     &handle_single,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free handle.\n" );

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
	if( handle_multi != NULL )
	{
		libsmraw_handle_close(
		 handle_multi,
		 NULL );
		libsmraw_handle_free(
		 &handle_multi,
		 NULL );
	}
	if( handle_single != NULL )
	{
		libsmraw_handle_close(
		 handle_single,
		 NULL );
		libsmraw_handle_free(
		 &handle_single,
		 NULL );
	}
	return( EXIT_FAILURE );
}

