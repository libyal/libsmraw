/*
 * Python bindings for libsmraw (pysmraw)
 *
 * Copyright (C) 2010-2018, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYSMRAW_H )
#define _PYSMRAW_H

#include <common.h>
#include <types.h>

#include "pysmraw_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

PyObject *pysmraw_get_version(
           PyObject *self,
           PyObject *arguments );

PyObject *pysmraw_glob(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_pysmraw(
                void );
#else
PyMODINIT_FUNC initpysmraw(
                void );
#endif

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYSMRAW_H ) */

