/*
 * Legacy functions
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
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include "libsmraw_handle.h"
#include "libsmraw_legacy.h"
#include "libsmraw_metadata.h"
#include "libsmraw_types.h"

#if !defined( HAVE_LOCAL_LIBSMRAW )

/* Sets the maximum amount of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_set_maximum_amount_of_open_handles(
     libsmraw_handle_t *handle,
     int maximum_amount_of_open_handles,
     liberror_error_t **error )
{
	return( libsmraw_handle_set_maximum_number_of_open_handles(
	         handle,
	         maximum_amount_of_open_handles,
	         error ) );
}

/* Retrieves the amount of information values
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_amount_of_information_values(
     libsmraw_handle_t *handle,
     int *amount_of_information_values,
     liberror_error_t **error )
{
	return( libsmraw_handle_get_number_of_information_values(
	         handle,
	         amount_of_information_values,
	         error ) );
}

/* Retrieves the amount of integrity hash values
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_amount_of_integrity_hash_values(
     libsmraw_handle_t *handle,
     int *amount_of_integrity_hash_values,
     liberror_error_t **error )
{
	return( libsmraw_handle_get_number_of_integrity_hash_values(
	         handle,
	         amount_of_integrity_hash_values,
	         error ) );
}

#endif /* !defined( HAVE_LOCAL_LIBSMRAW ) */

