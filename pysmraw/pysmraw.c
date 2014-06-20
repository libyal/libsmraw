/*
 * Python bindings module for libsmraw (pysmraw)
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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#include "pysmraw.h"
#include "pysmraw_error.h"
#include "pysmraw_handle.h"
#include "pysmraw_libcerror.h"
#include "pysmraw_libcstring.h"
#include "pysmraw_libsmraw.h"
#include "pysmraw_python.h"
#include "pysmraw_unused.h"

/* The pysmraw module methods
 */
PyMethodDef pysmraw_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pysmraw_get_version,
	  METH_NOARGS,
	  "get_version() -> String\n"
	  "\n"
	  "Retrieves the version." },

	{ "glob",
	  (PyCFunction) pysmraw_glob,
	  METH_VARARGS | METH_KEYWORDS,
	  "glob(filename) -> Object\n"
	  "\n"
	  "Globs filenames according to several split RAW storage media image naming schemas." },

	{ "open",
	  (PyCFunction) pysmraw_handle_new_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filenames, mode='r') -> Object\n"
	  "\n"
          "Opens file(s) from a sequence (list) of all the segment filenames.\n"
          "Use pysmraw.glob() to determine the segment filenames from first (E01)." },

/* TODO: open file-like object using pool - list of file objects */

	/* Sentinel */
	{ NULL,
	  NULL,
	  0,
	  NULL}
};

/* Retrieves the pysmraw/libsmraw version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmraw_get_version(
           PyObject *self PYSMRAW_ATTRIBUTE_UNUSED,
           PyObject *arguments PYSMRAW_ATTRIBUTE_UNUSED )
{
	const char *errors           = NULL;
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	PYSMRAW_UNREFERENCED_PARAMETER( self )
	PYSMRAW_UNREFERENCED_PARAMETER( arguments )

	Py_BEGIN_ALLOW_THREADS

	version_string = libsmraw_get_version();

	Py_END_ALLOW_THREADS

	version_string_length = libcstring_narrow_string_length(
	                         version_string );

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_length,
	         errors ) );
}

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )

/* Globs filenames according to the Expert Witness Compression Format (SMRAW) segment file naming schema
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmraw_glob(
           PyObject *self PYSMRAW_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	wchar_t **filenames_wide         = NULL;
	char **filenames_narrow          = NULL;
	libcerror_error_t *error         = NULL;
	PyObject *exception_string       = NULL;
	PyObject *exception_traceback    = NULL;
	PyObject *exception_type         = NULL;
	PyObject *exception_value        = NULL;
	PyObject *filename_string_object = NULL;
	PyObject *list_object            = NULL;
	PyObject *string_object          = NULL;
	const wchar_t *filename_wide     = NULL;
	static char *function            = "pysmraw_glob";
	static char *keyword_list[]      = { "filename", NULL };
	const char *errors               = NULL;
	const char *filename_narrow      = NULL;
	char *error_string               = NULL;
	size_t filename_length           = 0;
	int filename_index               = 0;
	int number_of_filenames          = 0;
	int result                       = 0;

	PYSMRAW_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. We cannot use "u" here either since that does not allow us to pass non Unicode string objects and
	 * Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		PyErr_Fetch(
		 &exception_type,
		 &exception_value,
		 &exception_traceback );

		exception_string = PyObject_Repr(
		                    exception_value );

		error_string = PyString_AsString(
		                exception_string );

		if( error_string != NULL )
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type unicode with error: %s.",
			 function,
			 error_string );
		}
		else
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type unicode.",
			 function );
		}
		Py_DecRef(
		 exception_string );

		goto on_error;
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );

		filename_length = libcstring_wide_string_length(
		                   filename_wide );

		Py_BEGIN_ALLOW_THREADS

		result = libsmraw_glob_wide(
			  filename_wide,
			  filename_length,
			  LIBSMRAW_FORMAT_UNKNOWN,
			  &filenames_wide,
			  &number_of_filenames,
			  &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pysmraw_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to glob filenames.",
			 function );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		list_object = PyList_New(
			       (Py_ssize_t) number_of_filenames );

		for( filename_index = 0;
		     filename_index < number_of_filenames;
		     filename_index++ )
		{
			filename_length = libcstring_wide_string_length(
					   filenames_wide[ filename_index ] );

			filename_string_object = PyUnicode_FromWideChar(
						  filenames_wide[ filename_index ],
						  filename_length );

			if( filename_string_object == NULL )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to convert filename: %d into Unicode.",
				 function,
				 filename_index );

				goto on_error;
			}
			if( PyList_SetItem(
			     list_object,
			     (Py_ssize_t) filename_index,
			     filename_string_object ) != 0 )
			{
				PyErr_Format(
				 PyExc_MemoryError,
				 "%s: unable to set filename: %d in list.",
				 function,
				 filename_index );

				goto on_error;
			}
			filename_string_object = NULL;
		}
		Py_BEGIN_ALLOW_THREADS

		result = libsmraw_glob_wide_free(
			  filenames_wide,
			  number_of_filenames,
			  &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pysmraw_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free globbed filenames.",
			 function );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		return( list_object );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );

	if( result == -1 )
	{
		PyErr_Fetch(
		 &exception_type,
		 &exception_value,
		 &exception_traceback );

		exception_string = PyObject_Repr(
				    exception_value );

		error_string = PyString_AsString(
				exception_string );

		if( error_string != NULL )
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type string with error: %s.",
			 function,
			 error_string );
		}
		else
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type string.",
			 function );
		}
		Py_DecRef(
		 exception_string );

		goto on_error;
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		filename_narrow = PyString_AsString(
				   string_object );

		filename_length = libcstring_narrow_string_length(
		                   filename_narrow );

		Py_BEGIN_ALLOW_THREADS

		result = libsmraw_glob(
			  filename_narrow,
			  filename_length,
			  LIBSMRAW_FORMAT_UNKNOWN,
			  &filenames_narrow,
			  &number_of_filenames,
			  &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pysmraw_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to glob filenames.",
			 function );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		list_object = PyList_New(
			       (Py_ssize_t) number_of_filenames );

		for( filename_index = 0;
		     filename_index < number_of_filenames;
		     filename_index++ )
		{
			filename_length = libcstring_narrow_string_length(
					   filenames_narrow[ filename_index ] );

			filename_string_object = PyUnicode_Decode(
						  filenames_narrow[ filename_index ],
						  filename_length,
						  PyUnicode_GetDefaultEncoding(),
						  errors );

			if( filename_string_object == NULL )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to convert filename: %d into Unicode.",
				 function,
				 filename_index );

				goto on_error;
			}
			if( PyList_SetItem(
			     list_object,
			     (Py_ssize_t) filename_index,
			     filename_string_object ) != 0 )
			{
				PyErr_Format(
				 PyExc_MemoryError,
				 "%s: unable to set filename: %d in list.",
				 function,
				 filename_index );

				goto on_error;
			}
			filename_string_object = NULL;
		}
		Py_BEGIN_ALLOW_THREADS

		result = libsmraw_glob_free(
			  filenames_narrow,
			  number_of_filenames,
			  &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pysmraw_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free globbed filenames.",
			 function );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		return( list_object );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type",
	 function );

on_error:
	if( filename_string_object != NULL )
	{
		Py_DecRef(
		 filename_string_object );
	}
	if( list_object != NULL )
	{
		Py_DecRef(
		 list_object );
	}
	if( filenames_narrow != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		libsmraw_glob_free(
		 filenames_narrow,
		 number_of_filenames,
		 NULL );

		Py_END_ALLOW_THREADS
	}
	if( filenames_wide != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		libsmraw_glob_wide_free(
		 filenames_wide,
		 number_of_filenames,
		 NULL );

		Py_END_ALLOW_THREADS
	}
	return( NULL );
}

#else

/* Globs filenames according to the Expert Witness Compression Format (SMRAW) segment file naming schema
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmraw_glob(
           PyObject *self PYSMRAW_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	char **filenames                 = NULL;
	libcerror_error_t *error         = NULL;
	PyObject *filename_string_object = NULL;
	PyObject *list_object            = NULL;
	static char *function            = "pysmraw_glob";
	static char *keyword_list[]      = { "filename", NULL };
	const char *errors               = NULL;
	const char *filename             = NULL;
	size_t filename_length           = 0;
	int filename_index               = 0;
	int number_of_filenames          = 0;
	int result                       = 0;

	PYSMRAW_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * For systems that support UTF-8 this works for Unicode string objects as well.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|s",
	     keyword_list,
	     &filename ) == 0 )
	{
		return( NULL );
	}
	filename_length = libcstring_narrow_string_length(
	                   filename );

	Py_BEGIN_ALLOW_THREADS

	result = libsmraw_glob(
	          filename,
	          filename_length,
	          &filenames,
	          &number_of_filenames,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to glob filenames.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	list_object = PyList_New(
	               (Py_ssize_t) number_of_filenames );

	for( filename_index = 0;
	     filename_index < number_of_filenames;
	     filename_index++ )
	{
		filename_length = libcstring_narrow_string_length(
		                   filenames[ filename_index ] );

		/* Pass the string length to PyUnicode_DecodeUTF8
		 * otherwise it makes the end of string character is part
		 * of the string
		 */
		filename_string_object = PyUnicode_DecodeUTF8(
		                          filenames[ filename_index ],
		                          filename_length,
		                          errors );

		if( filename_string_object == NULL )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to convert UTF-8 filename: %d into Unicode.",
			 function,
			 filename_index );

			goto on_error;
		}
		if( PyList_SetItem(
		     list_object,
		     (Py_ssize_t) filename_index,
		     filename_string_object ) != 0 )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to set filename: %d in list.",
			 function,
			 filename_index );

			goto on_error;
		}
		filename_string_object = NULL;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libsmraw_glob_free(
	          filenames,
	          number_of_filenames,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free globbed filenames.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	return( list_object );

on_error:
	if( filename_string_object != NULL )
	{
		Py_DecRef(
		 filename_string_object );
	}
	if( list_object != NULL )
	{
		Py_DecRef(
		 list_object );
	}
	if( filenames != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		libsmraw_glob_free(
		 filenames,
		 number_of_filenames,
		 NULL );

		Py_END_ALLOW_THREADS
	}
	return( NULL );
}

#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

/* Declarations for DLL import/export
 */
#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

/* Initializes the pysmraw module
 */
PyMODINIT_FUNC initpysmraw(
                void )
{
	PyObject *module                 = NULL;
	PyTypeObject *handle_type_object = NULL;
	PyGILState_STATE gil_state       = 0;

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
	module = Py_InitModule3(
	          "pysmraw",
	          pysmraw_module_methods,
	          "Python libsmraw module (pysmraw)." );

	PyEval_InitThreads();

	gil_state = PyGILState_Ensure();

	/* Setup the handle type object
	 */
	pysmraw_handle_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pysmraw_handle_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject * ) &pysmraw_handle_type_object );

	handle_type_object = &pysmraw_handle_type_object;

	PyModule_AddObject(
	 module,
	 "handle",
	 (PyObject *) handle_type_object );

on_error:
	PyGILState_Release(
	 gil_state );
}

