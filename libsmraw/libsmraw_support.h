/*
 * libsmraw support functions
 *
 * Copyright (c) 2008-2010, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBSMRAW_SUPPORT_H )
#define _LIBSMRAW_SUPPORT_H

#include <common.h>

#include <stdio.h>

#include "libsmraw_error.h"
#include "libsmraw_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBSMRAW_EXTERN const char *libsmraw_get_version(
                            void );

LIBSMRAW_EXTERN int libsmraw_detect_type(
                    const char *filename,
                    int *type,
                    liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

