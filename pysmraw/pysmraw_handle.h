/*
 * Python object definition of the libsmraw handle
 *
 * Copyright (c) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYSMRAW_HANDLE_H )
#define _PYSMRAW_HANDLE_H

#include <common.h>
#include <types.h>

#include "pysmraw_libbfio.h"
#include "pysmraw_libcerror.h"
#include "pysmraw_libsmraw.h"
#include "pysmraw_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pysmraw_handle pysmraw_handle_t;

struct pysmraw_handle
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libsmraw handle
	 */
	libsmraw_handle_t *handle;

	/* The file IO pool
	 */
	libbfio_pool_t *file_io_pool;
};

extern PyMethodDef pysmraw_handle_object_methods[];
extern PyTypeObject pysmraw_handle_type_object;

PyObject *pysmraw_handle_new(
           void );

PyObject *pysmraw_handle_new_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmraw_handle_new_open_file_objects(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

int pysmraw_handle_init(
     pysmraw_handle_t *pysmraw_handle );

void pysmraw_handle_free(
      pysmraw_handle_t *pysmraw_handle );

PyObject *pysmraw_handle_signal_abort(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments );

PyObject *pysmraw_handle_open(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmraw_handle_open_file_objects(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmraw_handle_close(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments );

PyObject *pysmraw_handle_read_buffer(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmraw_handle_read_buffer_at_offset(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmraw_handle_write_buffer(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmraw_handle_write_buffer_at_offset(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmraw_handle_seek_offset(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmraw_handle_get_offset(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif
