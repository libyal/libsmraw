/*
 *  Metadata functions for the Python object definition of the libsmraw handle
 *
 * Copyright (C) 2010-2021, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "pysmraw_error.h"
#include "pysmraw_handle.h"
#include "pysmraw_integer.h"
#include "pysmraw_libcerror.h"
#include "pysmraw_libclocale.h"
#include "pysmraw_libsmraw.h"
#include "pysmraw_metadata.h"
#include "pysmraw_python.h"
#include "pysmraw_unused.h"

/* Retrieves the size of the media data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmraw_handle_get_media_size(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments PYSMRAW_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pysmraw_handle_get_media_size";
	size64_t media_size      = 0;
	int result               = 0;

	PYSMRAW_UNREFERENCED_PARAMETER( arguments )

	if( pysmraw_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libsmraw_handle_get_media_size(
	          pysmraw_handle->handle,
	          &media_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve media size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pysmraw_integer_unsigned_new_from_64bit(
	                  (uint64_t) media_size );

	return( integer_object );
}

