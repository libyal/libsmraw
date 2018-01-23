/*
 * Codepage definitions for libsmraw
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

#if !defined( _LIBSMRAW_CODEPAGE_H )
#define _LIBSMRAW_CODEPAGE_H

#include <libsmraw/types.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* The codepage definitions
 */
enum LIBSMRAW_CODEPAGES
{
	LIBSMRAW_CODEPAGE_ASCII				= 20127,

	LIBSMRAW_CODEPAGE_ISO_8859_1			= 28591,
	LIBSMRAW_CODEPAGE_ISO_8859_2			= 28592,
	LIBSMRAW_CODEPAGE_ISO_8859_3			= 28593,
	LIBSMRAW_CODEPAGE_ISO_8859_4			= 28594,
	LIBSMRAW_CODEPAGE_ISO_8859_5			= 28595,
	LIBSMRAW_CODEPAGE_ISO_8859_6			= 28596,
	LIBSMRAW_CODEPAGE_ISO_8859_7			= 28597,
	LIBSMRAW_CODEPAGE_ISO_8859_8			= 28598,
	LIBSMRAW_CODEPAGE_ISO_8859_9			= 28599,
	LIBSMRAW_CODEPAGE_ISO_8859_10			= 28600,
	LIBSMRAW_CODEPAGE_ISO_8859_11			= 28601,
	LIBSMRAW_CODEPAGE_ISO_8859_13			= 28603,
	LIBSMRAW_CODEPAGE_ISO_8859_14			= 28604,
	LIBSMRAW_CODEPAGE_ISO_8859_15			= 28605,
	LIBSMRAW_CODEPAGE_ISO_8859_16			= 28606,

	LIBSMRAW_CODEPAGE_KOI8_R			= 20866,
	LIBSMRAW_CODEPAGE_KOI8_U			= 21866,

	LIBSMRAW_CODEPAGE_WINDOWS_874			= 874,
	LIBSMRAW_CODEPAGE_WINDOWS_932			= 932,
	LIBSMRAW_CODEPAGE_WINDOWS_936			= 936,
	LIBSMRAW_CODEPAGE_WINDOWS_949			= 949,
	LIBSMRAW_CODEPAGE_WINDOWS_950			= 950,
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

#define LIBSMRAW_CODEPAGE_US_ASCII			LIBSMRAW_CODEPAGE_ASCII

#define LIBSMRAW_CODEPAGE_ISO_WESTERN_EUROPEAN		LIBSMRAW_CODEPAGE_ISO_8859_1
#define LIBSMRAW_CODEPAGE_ISO_CENTRAL_EUROPEAN		LIBSMRAW_CODEPAGE_ISO_8859_2
#define LIBSMRAW_CODEPAGE_ISO_SOUTH_EUROPEAN		LIBSMRAW_CODEPAGE_ISO_8859_3
#define LIBSMRAW_CODEPAGE_ISO_NORTH_EUROPEAN		LIBSMRAW_CODEPAGE_ISO_8859_4
#define LIBSMRAW_CODEPAGE_ISO_CYRILLIC			LIBSMRAW_CODEPAGE_ISO_8859_5
#define LIBSMRAW_CODEPAGE_ISO_ARABIC			LIBSMRAW_CODEPAGE_ISO_8859_6
#define LIBSMRAW_CODEPAGE_ISO_GREEK			LIBSMRAW_CODEPAGE_ISO_8859_7
#define LIBSMRAW_CODEPAGE_ISO_HEBREW			LIBSMRAW_CODEPAGE_ISO_8859_8
#define LIBSMRAW_CODEPAGE_ISO_TURKISH			LIBSMRAW_CODEPAGE_ISO_8859_9
#define LIBSMRAW_CODEPAGE_ISO_NORDIC			LIBSMRAW_CODEPAGE_ISO_8859_10
#define LIBSMRAW_CODEPAGE_ISO_THAI			LIBSMRAW_CODEPAGE_ISO_8859_11
#define LIBSMRAW_CODEPAGE_ISO_BALTIC			LIBSMRAW_CODEPAGE_ISO_8859_13
#define LIBSMRAW_CODEPAGE_ISO_CELTIC			LIBSMRAW_CODEPAGE_ISO_8859_14

#define LIBSMRAW_CODEPAGE_ISO_LATIN_1			LIBSMRAW_CODEPAGE_ISO_8859_1
#define LIBSMRAW_CODEPAGE_ISO_LATIN_2			LIBSMRAW_CODEPAGE_ISO_8859_2
#define LIBSMRAW_CODEPAGE_ISO_LATIN_3			LIBSMRAW_CODEPAGE_ISO_8859_3
#define LIBSMRAW_CODEPAGE_ISO_LATIN_4			LIBSMRAW_CODEPAGE_ISO_8859_4
#define LIBSMRAW_CODEPAGE_ISO_LATIN_5			LIBSMRAW_CODEPAGE_ISO_8859_9
#define LIBSMRAW_CODEPAGE_ISO_LATIN_6			LIBSMRAW_CODEPAGE_ISO_8859_10
#define LIBSMRAW_CODEPAGE_ISO_LATIN_7			LIBSMRAW_CODEPAGE_ISO_8859_13
#define LIBSMRAW_CODEPAGE_ISO_LATIN_8			LIBSMRAW_CODEPAGE_ISO_8859_14
#define LIBSMRAW_CODEPAGE_ISO_LATIN_9			LIBSMRAW_CODEPAGE_ISO_8859_15
#define LIBSMRAW_CODEPAGE_ISO_LATIN_10			LIBSMRAW_CODEPAGE_ISO_8859_16

#define LIBSMRAW_CODEPAGE_KOI8_RUSSIAN			LIBSMRAW_CODEPAGE_KOI8_R
#define LIBSMRAW_CODEPAGE_KOI8_UKRAINIAN		LIBSMRAW_CODEPAGE_KOI8_U

#define LIBSMRAW_CODEPAGE_WINDOWS_THAI			LIBSMRAW_CODEPAGE_WINDOWS_874
#define LIBSMRAW_CODEPAGE_WINDOWS_JAPANESE		LIBSMRAW_CODEPAGE_WINDOWS_932
#define LIBSMRAW_CODEPAGE_WINDOWS_CHINESE_SIMPLIFIED	LIBSMRAW_CODEPAGE_WINDOWS_936
#define LIBSMRAW_CODEPAGE_WINDOWS_KOREAN		LIBSMRAW_CODEPAGE_WINDOWS_949
#define LIBSMRAW_CODEPAGE_WINDOWS_CHINESE_TRADITIONAL	LIBSMRAW_CODEPAGE_WINDOWS_950
#define LIBSMRAW_CODEPAGE_WINDOWS_CENTRAL_EUROPEAN	LIBSMRAW_CODEPAGE_WINDOWS_1250
#define LIBSMRAW_CODEPAGE_WINDOWS_CYRILLIC		LIBSMRAW_CODEPAGE_WINDOWS_1251
#define LIBSMRAW_CODEPAGE_WINDOWS_WESTERN_EUROPEAN	LIBSMRAW_CODEPAGE_WINDOWS_1252
#define LIBSMRAW_CODEPAGE_WINDOWS_GREEK			LIBSMRAW_CODEPAGE_WINDOWS_1253
#define LIBSMRAW_CODEPAGE_WINDOWS_TURKISH		LIBSMRAW_CODEPAGE_WINDOWS_1254
#define LIBSMRAW_CODEPAGE_WINDOWS_HEBREW		LIBSMRAW_CODEPAGE_WINDOWS_1255
#define LIBSMRAW_CODEPAGE_WINDOWS_ARABIC		LIBSMRAW_CODEPAGE_WINDOWS_1256
#define LIBSMRAW_CODEPAGE_WINDOWS_BALTIC		LIBSMRAW_CODEPAGE_WINDOWS_1257
#define LIBSMRAW_CODEPAGE_WINDOWS_VIETNAMESE		LIBSMRAW_CODEPAGE_WINDOWS_1258

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBSMRAW_CODEPAGE_H ) */

