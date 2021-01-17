/*
 * Python bindings module for libsmraw (pysmraw)
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
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pysmraw.h"
#include "pysmraw_error.h"
#include "pysmraw_handle.h"
#include "pysmraw_libcerror.h"
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
	  (PyCFunction) pysmraw_open_new_handle,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filenames, mode='r') -> Object\n"
	  "\n"
	  "Opens file(s) from a sequence (list) of all the segment filenames.\n"
	  "Use pysmraw.glob() to determine the segment filenames from first." },

	{ "open_file_objects",
	  (PyCFunction) pysmraw_open_new_handle_with_file_objects,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_objects(file_objects, mode='r') -> Object\n"
	  "\n"
          "Opens file(s) from a sequence (list) of file-like objects." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
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

	version_string_length = narrow_string_length(
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

/* Globs filenames according to the storage media (SM) (split) RAW segment file naming schema
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmraw_glob(
           PyObject *self PYSMRAW_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	char **filenames_narrow          = NULL;
	libcerror_error_t *error         = NULL;
	PyObject *filename_string_object = NULL;
	PyObject *list_object            = NULL;
	PyObject *string_object          = NULL;
	static char *function            = "pysmraw_glob";
	static char *keyword_list[]      = { "filename", NULL };
	const char *errors               = NULL;
	const char *filename_narrow      = NULL;
	size_t filename_length           = 0;
	int filename_index               = 0;
	int number_of_filenames          = 0;
	int result                       = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wchar_t **filenames_wide         = NULL;
	const wchar_t *filename_wide     = NULL;
#else
	PyObject *utf8_string_object     = NULL;
#endif

	PYSMRAW_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
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
		pysmraw_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );

		filename_length = PyUnicode_GetSize(
		                   string_object );

		Py_BEGIN_ALLOW_THREADS

		result = libsmraw_glob_wide(
			  filename_wide,
			  filename_length,
			  &filenames_wide,
			  &number_of_filenames,
			  &error );

		Py_END_ALLOW_THREADS
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pysmraw_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   utf8_string_object );

		filename_length = PyBytes_Size(
				   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
				   utf8_string_object );

		filename_length = PyString_Size(
				   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libsmraw_glob(
			  filename_narrow,
			  filename_length,
			  &filenames_narrow,
			  &number_of_filenames,
			  &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result != 1 )
		{
			pysmraw_error_raise(
			 error,
			 PyExc_RuntimeError,
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			filename_length = wide_string_length(
					   filenames_wide[ filename_index ] );

			filename_string_object = PyUnicode_FromWideChar(
						  filenames_wide[ filename_index ],
						  filename_length );
#else
			filename_length = narrow_string_length(
			                   filenames_narrow[ filename_index ] );

			/* Pass the string length to PyUnicode_DecodeUTF8
			 * otherwise it makes the end of string character is part
			 * of the string
			 */
			filename_string_object = PyUnicode_DecodeUTF8(
			                          filenames_narrow[ filename_index ],
			                          filename_length,
			                          errors );
#endif
			if( filename_string_object == NULL )
			{
				PyErr_Format(
				 PyExc_RuntimeError,
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

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libsmraw_glob_wide_free(
			  filenames_wide,
			  number_of_filenames,
			  &error );
#else
		result = libsmraw_glob_free(
			  filenames_narrow,
			  number_of_filenames,
			  &error );
#endif
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

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pysmraw_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   string_object );

		filename_length = PyBytes_Size(
				   string_object );
#else
		filename_narrow = PyString_AsString(
				   string_object );

		filename_length = PyString_Size(
				   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libsmraw_glob(
			  filename_narrow,
			  filename_length,
			  &filenames_narrow,
			  &number_of_filenames,
			  &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pysmraw_error_raise(
			 error,
			 PyExc_RuntimeError,
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
			filename_length = narrow_string_length(
					   filenames_narrow[ filename_index ] );

			filename_string_object = PyUnicode_Decode(
						  filenames_narrow[ filename_index ],
						  filename_length,
						  PyUnicode_GetDefaultEncoding(),
						  errors );

			if( filename_string_object == NULL )
			{
				PyErr_Format(
				 PyExc_RuntimeError,
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( filenames_wide != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		libsmraw_glob_wide_free(
		 filenames_wide,
		 number_of_filenames,
		 NULL );

		Py_END_ALLOW_THREADS
	}
#endif
	return( NULL );
}

/* Creates a new handle object and opens it
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmraw_open_new_handle(
           PyObject *self PYSMRAW_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	pysmraw_handle_t *pysmraw_handle = NULL;
	static char *function            = "pysmraw_open_new_handle";

	PYSMRAW_UNREFERENCED_PARAMETER( self )

	/* PyObject_New does not invoke tp_init
	 */
	pysmraw_handle = PyObject_New(
	                  struct pysmraw_handle,
	                  &pysmraw_handle_type_object );

	if( pysmraw_handle == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
	if( pysmraw_handle_init(
	     pysmraw_handle ) != 0 )
	{
		goto on_error;
	}
	if( pysmraw_handle_open(
	     pysmraw_handle,
	     arguments,
	     keywords ) == NULL )
	{
		goto on_error;
	}
	return( (PyObject *) pysmraw_handle );

on_error:
	if( pysmraw_handle != NULL )
	{
		Py_DecRef(
		 (PyObject *) pysmraw_handle );
	}
	return( NULL );
}

/* Creates a new handle object and opens it using file-like objects
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmraw_open_new_handle_with_file_objects(
           PyObject *self PYSMRAW_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	pysmraw_handle_t *pysmraw_handle = NULL;
	static char *function            = "pysmraw_open_new_handle_with_file_objects";

	PYSMRAW_UNREFERENCED_PARAMETER( self )

	/* PyObject_New does not invoke tp_init
	 */
	pysmraw_handle = PyObject_New(
	                  struct pysmraw_handle,
	                  &pysmraw_handle_type_object );

	if( pysmraw_handle == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
	if( pysmraw_handle_init(
	     pysmraw_handle ) != 0 )
	{
		goto on_error;
	}
	if( pysmraw_handle_open_file_objects(
	     pysmraw_handle,
	     arguments,
	     keywords ) == NULL )
	{
		goto on_error;
	}
	return( (PyObject *) pysmraw_handle );

on_error:
	if( pysmraw_handle != NULL )
	{
		Py_DecRef(
		 (PyObject *) pysmraw_handle );
	}
	return( NULL );
}

#if PY_MAJOR_VERSION >= 3

/* The pysmraw module definition
 */
PyModuleDef pysmraw_module_definition = {
	PyModuleDef_HEAD_INIT,

	/* m_name */
	"pysmraw",
	/* m_doc */
	"Python libsmraw module (pysmraw).",
	/* m_size */
	-1,
	/* m_methods */
	pysmraw_module_methods,
	/* m_reload */
	NULL,
	/* m_traverse */
	NULL,
	/* m_clear */
	NULL,
	/* m_free */
	NULL,
};

#endif /* PY_MAJOR_VERSION >= 3 */

/* Initializes the pysmraw module
 */
#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_pysmraw(
                void )
#else
PyMODINIT_FUNC initpysmraw(
                void )
#endif
{
	PyObject *module           = NULL;
	PyGILState_STATE gil_state = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libsmraw_notify_set_stream(
	 stderr,
	 NULL );
	libsmraw_notify_set_verbose(
	 1 );
#endif

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
#if PY_MAJOR_VERSION >= 3
	module = PyModule_Create(
	          &pysmraw_module_definition );
#else
	module = Py_InitModule3(
	          "pysmraw",
	          pysmraw_module_methods,
	          "Python libsmraw module (pysmraw)." );
#endif
	if( module == NULL )
	{
#if PY_MAJOR_VERSION >= 3
		return( NULL );
#else
		return;
#endif
	}
#if PY_VERSION_HEX < 0x03070000
	PyEval_InitThreads();
#endif
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
	 (PyObject *) &pysmraw_handle_type_object );

	PyModule_AddObject(
	 module,
	 "handle",
	 (PyObject *) &pysmraw_handle_type_object );

	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( module );
#else
	return;
#endif

on_error:
	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( NULL );
#else
	return;
#endif
}

