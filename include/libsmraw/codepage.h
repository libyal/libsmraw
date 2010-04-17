/*
 * Codepage definitions for libsmraw
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBSMRAW_CODEPAGE_H )
#define _LIBSMRAW_CODEPAGE_H

#include <libsmraw/types.h>

/* The codepages
 */
enum LIBSMRAW_CODEPAGES
{
	LIBSMRAW_CODEPAGE_ASCII				= 20127,
	LIBSMRAW_CODEPAGE_WINDOWS_1250			= 1250,
	LIBSMRAW_CODEPAGE_WINDOWS_1251			= 1251,
	LIBSMRAW_CODEPAGE_WINDOWS_1252			= 1252,
	LIBSMRAW_CODEPAGE_WINDOWS_1253			= 1253,
	LIBSMRAW_CODEPAGE_WINDOWS_1254			= 1254,
	LIBSMRAW_CODEPAGE_WINDOWS_1255			= 1255,
	LIBSMRAW_CODEPAGE_WINDOWS_1256			= 1256,
	LIBSMRAW_CODEPAGE_WINDOWS_1257			= 1257,
	LIBSMRAW_CODEPAGE_WINDOWS_1258			= 1258
};

#define LIBSMRAW_CODEPAGE_WINDOWS_CENTRAL_EUROPEAN	LIBSMRAW_CODEPAGE_WINDOWS_1250
#define LIBSMRAW_CODEPAGE_WINDOWS_CYRILLIC		LIBSMRAW_CODEPAGE_WINDOWS_1251
#define LIBSMRAW_CODEPAGE_WINDOWS_WESTERN_EUROPEAN	LIBSMRAW_CODEPAGE_WINDOWS_1252
#define LIBSMRAW_CODEPAGE_WINDOWS_GREEK			LIBSMRAW_CODEPAGE_WINDOWS_1253
#define LIBSMRAW_CODEPAGE_WINDOWS_TURKISH		LIBSMRAW_CODEPAGE_WINDOWS_1254
#define LIBSMRAW_CODEPAGE_WINDOWS_HEBREW		LIBSMRAW_CODEPAGE_WINDOWS_1255
#define LIBSMRAW_CODEPAGE_WINDOWS_ARABIC		LIBSMRAW_CODEPAGE_WINDOWS_1256
#define LIBSMRAW_CODEPAGE_WINDOWS_BALTIC		LIBSMRAW_CODEPAGE_WINDOWS_1257
#define LIBSMRAW_CODEPAGE_WINDOWS_VIETNAMESE		LIBSMRAW_CODEPAGE_WINDOWS_1258

#endif

