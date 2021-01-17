/*
 * Library function of libsmraw .net managed wrapper
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
#pragma managed( push, off )
#include <common.h>

#include <libsmraw.h>

#pragma managed( pop )

#include <vcclr.h>

#include "smraw.net.h"

#using <mscorlib.dll>

using namespace System;

namespace SMRaw {

System::String^ SMRaw::GetVersion( void )
{
	return( gcnew System::String(
	               libsmraw_get_version() ) );
}

} // namespace SMRaw

