/*
 * List type functions
 *
 * Copyright (C) 2008-2010, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBSMRAW_LIST_TYPE_H )
#define _LIBSMRAW_LIST_TYPE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBSMRAW_LIST_COMPARE_DEFINITIONS
{
	LIBSMRAW_LIST_COMPARE_LESS,
	LIBSMRAW_LIST_COMPARE_EQUAL,
	LIBSMRAW_LIST_COMPARE_GREATER
};

typedef struct libsmraw_list_element libsmraw_list_element_t;

struct libsmraw_list_element
{
	/* The previous element
	 */
	libsmraw_list_element_t *previous;

	/* The next element
	 */
	libsmraw_list_element_t *next;

	/* The list element value
	 */
	intptr_t *value;
};

typedef struct libsmraw_list libsmraw_list_t;

struct libsmraw_list
{
	/* The amount of elements
	 */
	int amount_of_elements;

	/* The first list element
	 */
	libsmraw_list_element_t *first;

	/* The last list element
	 */
	libsmraw_list_element_t *last;
};

int libsmraw_list_element_initialize(
     libsmraw_list_element_t **list_element,
     liberror_error_t **error );

int libsmraw_list_element_free(
     libsmraw_list_element_t **list_element,
     int (*value_free_function)( intptr_t *value, liberror_error_t **error ),
     liberror_error_t **error );

int libsmraw_list_initialize(
     libsmraw_list_t **list,
     liberror_error_t **error );

int libsmraw_list_free(
     libsmraw_list_t **list,
     int (*value_free_function)( intptr_t *value, liberror_error_t **error ),
     liberror_error_t **error );

int libsmraw_list_empty(
     libsmraw_list_t *list,
     int (*value_free_function)( intptr_t *value, liberror_error_t **error ),
     liberror_error_t **error );

int libsmraw_list_clone(
     libsmraw_list_t **destination,
     libsmraw_list_t *source,
     int (*value_clone_function)( intptr_t **destination, intptr_t *source, liberror_error_t **error ),
     liberror_error_t **error );

int libsmraw_list_get_amount_of_elements(
     libsmraw_list_t *list,
     int *amount_of_elements,
     liberror_error_t **error );

int libsmraw_list_get_element(
     libsmraw_list_t *list,
     int element_index,
     libsmraw_list_element_t **element,
     liberror_error_t **error );

int libsmraw_list_get_value(
     libsmraw_list_t *list,
     int element_index,
     intptr_t **value,
     liberror_error_t **error );

int libsmraw_list_prepend_element(
     libsmraw_list_t *list,
     libsmraw_list_element_t *element,
     liberror_error_t **error );

int libsmraw_list_prepend_value(
     libsmraw_list_t *list,
     intptr_t *value,
     liberror_error_t **error );

int libsmraw_list_append_element(
     libsmraw_list_t *list,
     libsmraw_list_element_t *element,
     liberror_error_t **error );

int libsmraw_list_append_value(
     libsmraw_list_t *list,
     intptr_t *value,
     liberror_error_t **error );

int libsmraw_list_insert_element(
     libsmraw_list_t *list,
     libsmraw_list_element_t *element,
     int (*value_compare_function)( intptr_t *first, intptr_t *second, liberror_error_t **error ),
     liberror_error_t **error );

int libsmraw_list_insert_value(
     libsmraw_list_t *list,
     intptr_t *value,
     int (*value_compare_function)( intptr_t *first, intptr_t *second, liberror_error_t **error ),
     liberror_error_t **error );

int libsmraw_list_remove_element(
     libsmraw_list_t *list,
     libsmraw_list_element_t *element,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

