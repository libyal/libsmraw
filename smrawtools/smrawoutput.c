/*
 * Output functions
 *
 * Copyright (c) 2010-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_STRING_H ) || defined( WINAPI )
#include <string.h>
#endif

#if defined( HAVE_LOCAL_LIBBFIO )
#include <libbfio_definitions.h>
#elif defined( HAVE_LIBBFIO )
#include <libbfio.h>
#endif

#if defined( HAVE_LOCAL_LIBFVALUE )
#include <libfvalue_definitions.h>
#elif defined( HAVE_LIBFVALUE )
#include <libfvalue.h>
#endif

#include "smrawoutput.h"
#include "smrawtools_libcerror.h"
#include "smrawtools_libclocale.h"
#include "smrawtools_libcnotify.h"
#include "smrawtools_libcstring.h"
#include "smrawtools_libcsystem.h"
#include "smrawtools_libsmraw.h"
#include "smrawtools_libuna.h"

/* Prints the executable version information
 */
void smrawoutput_copyright_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf(
	 stream,
	 "Copyright (c) 2010-2012, Joachim Metz <%s>.\n"
	 "This is free software; see the source for copying conditions. There is NO\n"
	 "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n",
	 PACKAGE_BUGREPORT );
}

/* Prints the version information to a stream
 */
void smrawoutput_version_fprint(
      FILE *stream,
      const libcstring_system_character_t *program )
{
	if( stream == NULL )
	{
		return;
	}
	if( program == NULL )
	{
		return;
	}
	fprintf(
	 stream,
	 "%" PRIs_LIBCSTRING_SYSTEM " %s\n\n",
	 program,
	 LIBSMRAW_VERSION_STRING );
}

/* Prints the detailed version information to a stream
 */
void smrawoutput_version_detailed_fprint(
      FILE *stream,
      const libcstring_system_character_t *program )
{
	if( stream == NULL )
	{
		return;
	}
	if( program == NULL )
	{
		return;
	}
	fprintf(
	 stream,
	 "%" PRIs_LIBCSTRING_SYSTEM " %s (libsmraw %s",
	 program,
	 LIBSMRAW_VERSION_STRING,
	 LIBSMRAW_VERSION_STRING );

#if defined( HAVE_LIBUNA ) || defined( HAVE_LOCAL_LIBUNA )
	fprintf(
	 stream,
	 ", libuna %s",
	 LIBUNA_VERSION_STRING );
#endif

#if defined( HAVE_LIBBFIO ) || defined( HAVE_LOCAL_LIBBFIO )
	fprintf(
	 stream,
	 ", libbfio %s",
	 LIBBFIO_VERSION_STRING );
#endif

#if defined( HAVE_LIBFVALUE ) || defined( HAVE_LOCAL_LIBFVALUE )
	fprintf(
	 stream,
	 ", libfvalue %s",
	 LIBFVALUE_VERSION_STRING );
#endif

	fprintf(
	 stream,
	 ")\n\n" );
}

