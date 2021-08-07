/*
 * Python object wrapper of libsmraw_handle_t
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
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pysmraw_error.h"
#include "pysmraw_file_objects_io_pool.h"
#include "pysmraw_handle.h"
#include "pysmraw_integer.h"
#include "pysmraw_libbfio.h"
#include "pysmraw_libcerror.h"
#include "pysmraw_libsmraw.h"
#include "pysmraw_metadata.h"
#include "pysmraw_python.h"
#include "pysmraw_unused.h"

#if !defined( LIBSMRAW_HAVE_BFIO )

LIBSMRAW_EXTERN \
int libsmraw_handle_open_file_io_pool(
     libsmraw_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libsmraw_error_t **error );

#endif /* !defined( LIBSMRAW_HAVE_BFIO ) */

PyMethodDef pysmraw_handle_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pysmraw_handle_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the handle to abort the current activity." },

	/* Functions to access the media data */

	{ "open",
	  (PyCFunction) pysmraw_handle_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filenames, mode='r') -> None\n"
	  "\n"
	  "Opens a handle from a sequence (list) of all the segment filenames.\n"
	  "Use pysmraw.glob() to determine the segment filenames from first." },

	{ "open_file_objects",
	  (PyCFunction) pysmraw_handle_open_file_objects,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_objects(file_objects, mode='r') -> None\n"
	  "\n"
	  "Opens a handle using a list of file-like objects." },

	{ "close",
	  (PyCFunction) pysmraw_handle_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a handle." },

	{ "read_buffer",
	  (PyCFunction) pysmraw_handle_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> String\n"
	  "\n"
	  "Reads a buffer of media data." },

	{ "read_buffer_at_offset",
	  (PyCFunction) pysmraw_handle_read_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer_at_offset(size, offset) -> String\n"
	  "\n"
	  "Reads a buffer of media data at a specific offset." },

	{ "write_buffer",
	  (PyCFunction) pysmraw_handle_write_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "write_buffer(buffer) -> None\n"
	  "\n"
	  "Writes a buffer of media data." },

	{ "write_buffer_at_offset",
	  (PyCFunction) pysmraw_handle_write_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "write_buffer_at_offset(buffer, offset) -> None\n"
	  "\n"
	  "Writes a buffer of media data at a specific offset." },

	{ "seek_offset",
	  (PyCFunction) pysmraw_handle_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the media data." },

	{ "get_offset",
	  (PyCFunction) pysmraw_handle_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Retrieves the current offset within the media data." },

	/* Some Pythonesque aliases */

	{ "read",
	  (PyCFunction) pysmraw_handle_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read(size) -> String\n"
	  "\n"
	  "Reads a buffer of media data." },

	{ "write",
	  (PyCFunction) pysmraw_handle_write_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "write(buffer, size) -> None\n"
	  "\n"
	  "Writes a buffer of media data." },

	{ "seek",
	  (PyCFunction) pysmraw_handle_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the media data." },

	{ "tell",
	  (PyCFunction) pysmraw_handle_get_offset,
	  METH_NOARGS,
	  "tell() -> Integer\n"
	  "\n"
	  "Retrieves the current offset within the media data." },

	/* Functions to access the metadata */

	{ "get_media_size",
	  (PyCFunction) pysmraw_handle_get_media_size,
	  METH_NOARGS,
	  "get_media_size() -> Integer\n"
	  "\n"
	  "Retrieves the size of the media data." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pysmraw_handle_object_get_set_definitions[] = {

	{ "media_size",
	  (getter) pysmraw_handle_get_media_size,
	  (setter) 0,
	  "The media size.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pysmraw_handle_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pysmraw.handle",
	/* tp_basicsize */
	sizeof( pysmraw_handle_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pysmraw_handle_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
	/* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pysmraw handle object (wraps libsmraw_handle_t)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pysmraw_handle_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pysmraw_handle_object_get_set_definitions,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pysmraw_handle_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Initializes a handle object
 * Returns 0 if successful or -1 on error
 */
int pysmraw_handle_init(
     pysmraw_handle_t *pysmraw_handle )
{
	static char *function    = "pysmraw_handle_init";
	libcerror_error_t *error = NULL;

	if( pysmraw_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	pysmraw_handle->handle       = NULL;
	pysmraw_handle->file_io_pool = NULL;

	if( libsmraw_handle_initialize(
	     &( pysmraw_handle->handle ),
	     &error ) != 1 )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize handle.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a handle object
 */
void pysmraw_handle_free(
      pysmraw_handle_t *pysmraw_handle )
{
	libcerror_error_t *error    = NULL;
	struct _typeobject *ob_type = NULL;
	static char *function       = "pysmraw_handle_free";
	int result                  = 0;

	if( pysmraw_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pysmraw_handle );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pysmraw_handle->file_io_pool != NULL )
	{
		if( pysmraw_handle_close(
		     pysmraw_handle,
		     NULL ) == NULL )
		{
			return;
		}
	}
	if( pysmraw_handle->handle != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libsmraw_handle_free(
		          &( pysmraw_handle->handle ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pysmraw_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free handle.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	ob_type->tp_free(
	 (PyObject*) pysmraw_handle );
}

/* Signals the handle to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmraw_handle_signal_abort(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments PYSMRAW_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pysmraw_handle_signal_abort";
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

	result = libsmraw_handle_signal_abort(
	          pysmraw_handle->handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to signal abort.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Open a handle
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmraw_handle_open(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords )
{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	PyObject *codepage_string_object = NULL;
	wchar_t **filenames              = NULL;
	wchar_t *filename_wide           = NULL;
	const char *errors               = NULL;
#else
	PyObject *utf8_string_object     = NULL;
	char **filenames                 = NULL;
#endif
	PyObject *sequence_object        = NULL;
	PyObject *string_object          = NULL;
	libcerror_error_t *error         = NULL;
	char *filename_narrow            = NULL;
	char *mode                       = NULL;
	static char *keyword_list[]      = { "filenames", "mode", NULL };
	static char *function            = "pysmraw_handle_open";
	Py_ssize_t sequence_size         = 0;
	size_t filename_length           = 0;
	int access_flags                 = 0;
	int filename_index               = 0;
	int is_unicode_string            = 0;
	int number_of_filenames          = 0;
	int result                       = 0;

	if( pysmraw_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	      keywords,
	      "O|s",
	      keyword_list,
	      &sequence_object,
	      &mode ) == 0 )
	{
		return( NULL );
	}
	if( PySequence_Check(
	     sequence_object ) == 0 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: argument: files must be a sequence object.",
		 function );

		return( NULL );
	}
	if( mode == NULL )
	{
		access_flags = LIBSMRAW_OPEN_READ;
	}
	else if( mode[ 0 ] == 'r' )
	{
		if( mode[ 1 ] == '+' )
		{
			if( ( mode[ 2 ] == 0 )
			 || ( ( mode[ 2 ] == 'b' )
			  &&  ( mode[ 3 ] == 0 ) ) )
			{
				access_flags = LIBSMRAW_OPEN_READ_WRITE;
			}
		}
		else if( ( mode[ 1 ] == 0 )
		      || ( ( mode[ 1 ] == 'b' )
		       &&  ( mode[ 2 ] == 0 ) ) )
		{
			access_flags = LIBSMRAW_OPEN_READ;
		}
	}
	else if( mode[ 0 ] == 'w' )
	{
		if( ( mode[ 1 ] == 0 )
		 || ( ( mode[ 1 ] == 'b' )
		  &&  ( mode[ 2 ] == 0 ) ) )
		{
			access_flags = LIBSMRAW_OPEN_WRITE;
		}
	}
	if( access_flags == 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	sequence_size = PySequence_Size(
	                 sequence_object );

	if( sequence_size > (Py_ssize_t) INT_MAX )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence size value exceeds maximum.",
		 function );

		goto on_error;
	}
	number_of_filenames = (int) sequence_size;

	if( ( number_of_filenames <= 0 )
	 || ( number_of_filenames > (int) UINT16_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid number of files.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	filenames = (wchar_t **) PyMem_Malloc(
	                          sizeof( wchar_t * ) * number_of_filenames );
#else
	filenames = (char **) PyMem_Malloc(
	                       sizeof( char * ) * number_of_filenames );
#endif
	if( filenames == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create filenames.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( memory_set(
	     filenames,
	     0,
	     sizeof( wchar_t * ) * number_of_filenames ) == NULL )
#else
	if( memory_set(
	     filenames,
	     0,
	     sizeof( char * ) * number_of_filenames ) == NULL )
#endif
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to clear filenames.",
		 function );

		PyMem_Free(
		 filenames );

		return( NULL );
	}
	for( filename_index = 0;
	     filename_index < number_of_filenames;
	     filename_index++ )
	{
		string_object = PySequence_GetItem(
		                 sequence_object,
		                 filename_index );

		PyErr_Clear();

		result = PyObject_IsInstance(
			  string_object,
			  (PyObject *) &PyUnicode_Type );

		if( result == -1 )
		{
			pysmraw_error_fetch_and_raise(
			 PyExc_ValueError,
			 "%s: unable to determine if the sequence object: %d is of type unicode.",
			 function,
			 filename_index );

			goto on_error;
		}
		else if( result != 0 )
		{
			is_unicode_string = 1;
		}
		else
		{
			is_unicode_string = 0;

			PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
			result = PyObject_IsInstance(
				  string_object,
				  (PyObject *) &PyBytes_Type );
#else
			result = PyObject_IsInstance(
				  string_object,
				  (PyObject *) &PyString_Type );
#endif /* PY_MAJOR_VERSION >= 3 */

			if( result == -1 )
			{
				pysmraw_error_fetch_and_raise(
				 PyExc_RuntimeError,
				 "%s: unable to determine if string object is of type string.",
				 function );

				goto on_error;
			}
			else if( result == 0 )
			{
				PyErr_Format(
				 PyExc_TypeError,
				 "%s: unsupported string object type",
				 function );

				goto on_error;
			}
		}
		if( is_unicode_string != 0 )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			filename_wide = (wchar_t *) PyUnicode_AsUnicode(
			                             string_object );

			filename_length = PyUnicode_GetSize(
			                   string_object );
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
#endif /* PY_MAJOR_VERSION >= 3 */
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */
		}
		else
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
#endif /* PY_MAJOR_VERSION >= 3 */

			codepage_string_object = PyUnicode_Decode(
						  filename_narrow,
						  filename_length,
						  PyUnicode_GetDefaultEncoding(),
						  errors );

			if( codepage_string_object == NULL )
			{
				PyErr_Format(
				 PyExc_RuntimeError,
				 "%s: unable to convert filename: %d into Unicode.",
				 function,
				 filename_index );

				goto on_error;
			}
			filename_wide = (wchar_t *) PyUnicode_AsUnicode(
			                             codepage_string_object );

			filename_length = PyUnicode_GetSize(
			                   codepage_string_object );
#else
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
#endif /* PY_MAJOR_VERSION >= 3 */
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( ( filename_wide == NULL )
		 || ( filename_length == 0 ) )
#else
		if( ( filename_narrow == NULL )
		 || ( filename_length == 0 ) )
#endif
		{
			PyErr_Format(
			 PyExc_RuntimeError,
			 "%s: missing filename: %d.",
			 function,
			 filename_index );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filenames[ filename_index ] = (wchar_t *) PyMem_Malloc(
		                                           sizeof( wchar_t ) * ( filename_length + 1 ) );
#else
		filenames[ filename_index ] = (char *) PyMem_Malloc(
		                                        sizeof( char ) * ( filename_length + 1 ) );
#endif
		if( filenames[ filename_index ] == NULL )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to create filename: %d.",
			 function,
			 filename_index );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( wide_string_copy(
		     filenames[ filename_index ],
		     filename_wide,
		     filename_length ) == NULL )
#else
		if( narrow_string_copy(
		     filenames[ filename_index ],
		     filename_narrow,
		     filename_length ) == NULL )
#endif
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to set filename: %d.",
			 function,
			 filename_index );

			goto on_error;
		}
		( filenames[ filename_index ] )[ filename_length ] = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( codepage_string_object != NULL )
		{
			Py_DecRef(
			 codepage_string_object );

			codepage_string_object = NULL;
		}
#else
		if( utf8_string_object != NULL )
		{
			Py_DecRef(
			 utf8_string_object );

			utf8_string_object = NULL;
		}
#endif
		/* The string object was reference by PySequence_GetItem
		 */
		Py_DecRef(
		 string_object );

		string_object = NULL;
	}
	Py_BEGIN_ALLOW_THREADS

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libsmraw_handle_open_wide(
	          pysmraw_handle->handle,
	          filenames,
	          number_of_filenames,
	          access_flags,
	          &error );
#else
	result = libsmraw_handle_open(
	          pysmraw_handle->handle,
	          filenames,
	          number_of_filenames,
	          access_flags,
	          &error );
#endif
	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	for( filename_index = 0;
	     filename_index < number_of_filenames;
	     filename_index++ )
	{
		PyMem_Free(
		 filenames[ filename_index ] );
	}
	PyMem_Free(
	 filenames );

	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( codepage_string_object != NULL )
	{
		Py_DecRef(
		 codepage_string_object );
	}
#else
	if( utf8_string_object != NULL )
	{
		Py_DecRef(
		 utf8_string_object );
	}
#endif
	if( string_object != NULL )
	{
		Py_DecRef(
		 string_object );
	}
	if( filenames != NULL )
	{
		for( ; filename_index > 0; filename_index-- )
		{
			PyMem_Free(
			 filenames[ filename_index - 1 ] );
		}
		PyMem_Free(
		 filenames );
	}
	return( NULL );
}

/* Opens a handle using a list of file-like objects
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmraw_handle_open_file_objects(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_objects      = NULL;
	libcerror_error_t *error    = NULL;
	char *mode                  = NULL;
	static char *keyword_list[] = { "file_object", "mode", NULL };
	static char *function       = "pysmraw_handle_open_file_objects";
	int result                  = 0;

	if( pysmraw_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &file_objects,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	if( pysmraw_handle->file_io_pool != NULL )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: invalid handle - file IO pool already set.",
		 function );

		goto on_error;
	}
	if( pysmraw_file_objects_pool_initialize(
	     &( pysmraw_handle->file_io_pool ),
	     file_objects,
	     LIBBFIO_OPEN_READ,
	     &error ) != 1 )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO pool.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libsmraw_handle_open_file_io_pool(
	          pysmraw_handle->handle,
	          pysmraw_handle->file_io_pool,
	          LIBSMRAW_OPEN_READ,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open file.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pysmraw_handle->file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &( pysmraw_handle->file_io_pool ),
		 NULL );
	}
	return( NULL );
}

/* Closes a handle
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmraw_handle_close(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments PYSMRAW_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pysmraw_handle_close";
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

	result = libsmraw_handle_close(
	          pysmraw_handle->handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to close handle.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( pysmraw_handle->file_io_pool != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_pool_free(
		          &( pysmraw_handle->file_io_pool ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pysmraw_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to free libbfio file IO pool.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Reads a buffer of media data
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pysmraw_handle_read_buffer(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	char *buffer                = NULL;
	static char *function       = "pysmraw_handle_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	ssize_t read_count          = 0;
	int64_t read_size           = 0;
	int result                  = 0;

	if( pysmraw_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &integer_object ) == 0 )
	{
		return( NULL );
	}
	if( integer_object == NULL )
	{
		result = 0;
	}
	else
	{
		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyLong_Type );

		if( result == -1 )
		{
			pysmraw_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type long.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION < 3
		else if( result == 0 )
		{
			PyErr_Clear();

			result = PyObject_IsInstance(
			          integer_object,
			          (PyObject *) &PyInt_Type );

			if( result == -1 )
			{
				pysmraw_error_fetch_and_raise(
				 PyExc_RuntimeError,
				 "%s: unable to determine if integer object is of type int.",
				 function );

				return( NULL );
			}
		}
#endif
	}
	if( result != 0 )
	{
		if( pysmraw_integer_signed_copy_to_64bit(
		     integer_object,
		     &read_size,
		     &error ) != 1 )
		{
			pysmraw_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else if( ( integer_object == NULL )
	      || ( integer_object == Py_None ) )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libsmraw_handle_get_media_size(
		          pysmraw_handle->handle,
		          (size64_t *) &read_size,
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
	}
	else
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (int64_t) INT_MAX )
	 || ( read_size > (int64_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libsmraw_handle_read_buffer(
	              pysmraw_handle->handle,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count == -1 )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Reads a buffer of media data at a specific offset
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pysmraw_handle_read_buffer_at_offset(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	char *buffer                = NULL;
	static char *function       = "pysmraw_handle_read_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	ssize_t read_count          = 0;
	off64_t read_offset         = 0;
	int64_t read_size           = 0;
	int result                  = 0;

	if( pysmraw_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "OL",
	     keyword_list,
	     &integer_object,
	     &read_offset ) == 0 )
	{
		return( NULL );
	}
	result = PyObject_IsInstance(
	          integer_object,
	          (PyObject *) &PyLong_Type );

	if( result == -1 )
	{
		pysmraw_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if integer object is of type long.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION < 3
	else if( result == 0 )
	{
		PyErr_Clear();

		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyInt_Type );

		if( result == -1 )
		{
			pysmraw_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type int.",
			 function );

			return( NULL );
		}
	}
#endif
	if( result != 0 )
	{
		if( pysmraw_integer_signed_copy_to_64bit(
		     integer_object,
		     &read_size,
		     &error ) != 1 )
		{
			pysmraw_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (int64_t) INT_MAX )
	 || ( read_size > (int64_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
	if( read_offset < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read offset value less than zero.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libsmraw_handle_read_buffer_at_offset(
	              pysmraw_handle->handle,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              (off64_t) read_offset,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count == -1 )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Writes a buffer of media data
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pysmraw_handle_write_buffer(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pysmraw_handle_write_buffer";
	static char *keyword_list[] = { "buffer", NULL };
	char *buffer                = NULL;
	Py_ssize_t buffer_size      = 0;
	ssize_t write_count         = 0;

	if( pysmraw_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	buffer = PyBytes_AsString(
	          string_object );

	buffer_size = PyBytes_Size(
	               string_object );
#else
	buffer = PyString_AsString(
	          string_object );

	buffer_size = PyString_Size(
	               string_object );
#endif
	if( ( buffer_size < 0 )
	 || ( buffer_size > (Py_ssize_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument buffer size value out of bounds.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	write_count = libsmraw_handle_write_buffer(
	               pysmraw_handle->handle,
	               (uint8_t *) buffer,
	               (size_t) buffer_size,
	               &error );

	Py_END_ALLOW_THREADS

	if( write_count != (ssize_t) buffer_size )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to write data.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Writes a buffer of media data at a specific offset
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pysmraw_handle_write_buffer_at_offset(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pysmraw_handle_write_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	char *buffer                = NULL;
	off64_t write_offset        = 0;
	Py_ssize_t buffer_size      = 0;
	ssize_t write_count         = 0;

	if( pysmraw_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|L",
	     keyword_list,
	     &string_object,
	     &write_offset ) == 0 )
	{
		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	buffer = PyBytes_AsString(
	          string_object );

	buffer_size = PyBytes_Size(
	               string_object );
#else
	buffer = PyString_AsString(
	          string_object );

	buffer_size = PyString_Size(
	               string_object );
#endif
	if( ( buffer_size < 0 )
	 || ( buffer_size > (Py_ssize_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument buffer size value out of bounds.",
		 function );

		return( NULL );
	}
	if( write_offset < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument write offset value less than zero.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	write_count = libsmraw_handle_write_buffer_at_offset(
	               pysmraw_handle->handle,
	               (uint8_t *) buffer,
	               (size_t) buffer_size,
	               write_offset,
	               &error );

	Py_END_ALLOW_THREADS

	if( write_count != (ssize_t) buffer_size )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to write data.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Seeks a certain offset in the media data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pysmraw_handle_seek_offset(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pysmraw_handle_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( pysmraw_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "L|i",
	     keyword_list,
	     &offset,
	     &whence ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	offset = libsmraw_handle_seek_offset(
	          pysmraw_handle->handle,
	          offset,
	          whence,
	          &error );

	Py_END_ALLOW_THREADS

 	if( offset == -1 )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to seek offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmraw_handle_get_offset(
           pysmraw_handle_t *pysmraw_handle,
           PyObject *arguments PYSMRAW_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pysmraw_handle_get_offset";
	off64_t current_offset   = 0;
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

	result = libsmraw_handle_get_offset(
	          pysmraw_handle->handle,
	          &current_offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pysmraw_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pysmraw_integer_signed_new_from_64bit(
	                  (int64_t) current_offset );

	return( integer_object );
}

