/*
 * Codepage functions
 *
 * Copyright (C) 2010-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBSMRAW_INTERNAL_CODEPAGE_H )
#define _LIBSMRAW_INTERNAL_CODEPAGE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* Define HAVE_LOCAL_LIBSMRAW for local use of libsmraw
 * The definitions in <libsmraw/codepage.h> are copied here
 * for local use of libsmraw
 */
#if !defined( HAVE_LOCAL_LIBSMRAW )

#include <libsmraw/codepage.h>

#else

/* The codepage definitions
 */
enum LIBSMRAW_CODEPAGES
{
	LIBSMRAW_CODEPAGE_ASCII		= 20127,

	LIBSMRAW_CODEPAGE_ISO_8859_1	= 28591,
	LIBSMRAW_CODEPAGE_ISO_8859_2	= 28592,
	LIBSMRAW_CODEPAGE_ISO_8859_3	= 28593,
	LIBSMRAW_CODEPAGE_ISO_8859_4	= 28594,
	LIBSMRAW_CODEPAGE_ISO_8859_5	= 28595,
	LIBSMRAW_CODEPAGE_ISO_8859_6	= 28596,
	LIBSMRAW_CODEPAGE_ISO_8859_7	= 28597,
	LIBSMRAW_CODEPAGE_ISO_8859_8	= 28598,
	LIBSMRAW_CODEPAGE_ISO_8859_9	= 28599,
	LIBSMRAW_CODEPAGE_ISO_8859_10	= 28600,
	LIBSMRAW_CODEPAGE_ISO_8859_11	= 28601,
	LIBSMRAW_CODEPAGE_ISO_8859_13	= 28603,
	LIBSMRAW_CODEPAGE_ISO_8859_14	= 28604,
	LIBSMRAW_CODEPAGE_ISO_8859_15	= 28605,
	LIBSMRAW_CODEPAGE_ISO_8859_16	= 28606,

	LIBSMRAW_CODEPAGE_KOI8_R	= 20866,
	LIBSMRAW_CODEPAGE_KOI8_U	= 21866,

	LIBSMRAW_CODEPAGE_WINDOWS_874	= 874,
	LIBSMRAW_CODEPAGE_WINDOWS_932	= 932,
	LIBSMRAW_CODEPAGE_WINDOWS_936	= 936,
	LIBSMRAW_CODEPAGE_WINDOWS_949	= 949,
	LIBSMRAW_CODEPAGE_WINDOWS_950	= 950,
	LIBSMRAW_CODEPAGE_WINDOWS_1250	= 1250,
	LIBSMRAW_CODEPAGE_WINDOWS_1251	= 1251,
	LIBSMRAW_CODEPAGE_WINDOWS_1252	= 1252,
	LIBSMRAW_CODEPAGE_WINDOWS_1253	= 1253,
	LIBSMRAW_CODEPAGE_WINDOWS_1254	= 1254,
	LIBSMRAW_CODEPAGE_WINDOWS_1255	= 1255,
	LIBSMRAW_CODEPAGE_WINDOWS_1256	= 1256,
	LIBSMRAW_CODEPAGE_WINDOWS_1257	= 1257,
	LIBSMRAW_CODEPAGE_WINDOWS_1258	= 1258
};

#endif /* !defined( HAVE_LOCAL_LIBSMRAW ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBSMRAW_INTERNAL_CODEPAGE_H ) */

