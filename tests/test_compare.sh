#!/bin/bash
#
# Storage media (SM) RAW library seek and read compare testing script
#
# Copyright (c) 2010-2013, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

INPUT="input";
TMP="tmp";

CMP="cmp";
LS="ls";
TR="tr";
SED="sed";
SORT="sort";
UNIQ="uniq";
WC="wc";

COMPARE_SINGLE="compare_single.raw";
COMPARE_MULTI="compare_multi.raw";

COMPARE_SINGLE="usb256_single.raw";
COMPARE_MULTI="usb256_multi.raw";

SMRAW_TEST_COMPARE="smraw_test_compare";

test_compare()
{ 
	echo "Testing seek and read compare of input:" $*;

	./${SMRAW_TEST_COMPARE} $*;

	RESULT=$?;

	echo "";

	return ${RESULT};
}

if ! test -x ${SMRAW_TEST_COMPARE};
then
	echo "Missing executable: ${SMRAW_TEST_COMPARE}";

	exit ${EXIT_FAILURE};
fi

if ! test -d ${INPUT};
then
	echo "No input directory found, to test read create input directory and place test files in directory.";
	echo "Use unique filename bases per set of RAW image file(s)."

	exit ${EXIT_IGNORE};
fi

if ! test -f ${INPUT}/${COMPARE_SINGLE};
then
	echo "No single segment file RAW image found in input directory (${COMPARE_SINGLE}),";
	echo "to test read place test files in directory. Use unique filename bases per set of RAW image file(s)."

	exit ${EXIT_IGNORE};
fi

RESULT=`${LS} ${INPUT}/${COMPARE_MULTI}* | ${TR} ' ' '\n' | ${SED} 's/[.][^.]*$//' | ${SORT} | ${UNIQ} | ${WC} -l`;

if test ${RESULT} -eq 0;
then
	echo "No single segment file RAW image found in input directory (${COMPARE_MULTI}*),";
	echo "to test read place test files in directory. Use unique filename bases per set of RAW image file(s)."

	exit ${EXIT_IGNORE};
fi

if ! test_compare ${INPUT}/${COMPARE_SINGLE} ${INPUT}/${COMPARE_MULTI};
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};

