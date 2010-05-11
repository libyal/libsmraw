#!/bin/bash
#
# Storage media (SM) RAW library glob testing script
#
# Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

TMP="tmp";

CMP="cmp";

SMRAW_TEST_GLOB="smraw_test_glob";

function test_glob
{ 
	BASENAME=$1;
	SCHEMA=$2;
	FILENAMES=$3;

	mkdir ${TMP};
	cd ${TMP};

	echo ${FILENAMES} > input;

	touch ${FILENAMES};

	../${SMRAW_TEST_GLOB} ${BASENAME} > output;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		if ! ${CMP} -s input output;
		then
			RESULT=${EXIT_FAILURE};
		fi
	fi

	cd ..;
	rm -rf ${TMP};

	echo -n "Testing glob: for basename: ${BASENAME} and schema: ${SCHEMA} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

if ! test -x ${SMRAW_TEST_GLOB};
then
	echo "Missing executable: ${SMRAW_TEST_GLOB}";

	exit ${EXIT_FAILURE};
fi

rm -rf ${TMP};

if ! test_glob "PREFIX" ".dd" "PREFIX.dd";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.dd" ".dd" "PREFIX.dd";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" ".raw" "PREFIX.raw";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.raw" ".raw" "PREFIX.raw";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" ".0" "PREFIX.0 PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.0" ".0" "PREFIX.0 PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" ".1" "PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.1" ".1" "PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" ".00" "PREFIX.00 PREFIX.01 PREFIX.02 PREFIX.03 PREFIX.04 PREFIX.05 PREFIX.06 PREFIX.07 PREFIX.08 PREFIX.09 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.00" ".00" "PREFIX.00 PREFIX.01 PREFIX.02 PREFIX.03 PREFIX.04 PREFIX.05 PREFIX.06 PREFIX.07 PREFIX.08 PREFIX.09 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" ".01" "PREFIX.01 PREFIX.02 PREFIX.03 PREFIX.04 PREFIX.05 PREFIX.06 PREFIX.07 PREFIX.08 PREFIX.09 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.01" ".01" "PREFIX.01 PREFIX.02 PREFIX.03 PREFIX.04 PREFIX.05 PREFIX.06 PREFIX.07 PREFIX.08 PREFIX.09 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" ".000" "PREFIX.000 PREFIX.001 PREFIX.002 PREFIX.003 PREFIX.004 PREFIX.005 PREFIX.006 PREFIX.007 PREFIX.008 PREFIX.009 PREFIX.010";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.000" ".000" "PREFIX.000 PREFIX.001 PREFIX.002 PREFIX.003 PREFIX.004 PREFIX.005 PREFIX.006 PREFIX.007 PREFIX.008 PREFIX.009 PREFIX.010";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" ".001" "PREFIX.001 PREFIX.002 PREFIX.003 PREFIX.004 PREFIX.005 PREFIX.006 PREFIX.007 PREFIX.008 PREFIX.009 PREFIX.010";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.001" ".001" "PREFIX.001 PREFIX.002 PREFIX.003 PREFIX.004 PREFIX.005 PREFIX.006 PREFIX.007 PREFIX.008 PREFIX.009 PREFIX.010";
then
	exit ${EXIT_FAILURE};
fi

if test_glob "PREFIX" "0" "PREFIX0 PREFIX1 PREFIX2 PREFIX3 PREFIX4 PREFIX5 PREFIX6 PREFIX7 PREFIX8 PREFIX9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX0" "0" "PREFIX0 PREFIX1 PREFIX2 PREFIX3 PREFIX4 PREFIX5 PREFIX6 PREFIX7 PREFIX8 PREFIX9";
then
	exit ${EXIT_FAILURE};
fi

if test_glob "PREFIX" "1" "PREFIX1 PREFIX2 PREFIX3 PREFIX4 PREFIX5 PREFIX6 PREFIX7 PREFIX8 PREFIX9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX1" "1" "PREFIX1 PREFIX2 PREFIX3 PREFIX4 PREFIX5 PREFIX6 PREFIX7 PREFIX8 PREFIX9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" "00" "PREFIX00 PREFIX01 PREFIX02 PREFIX03 PREFIX04 PREFIX05 PREFIX06 PREFIX07 PREFIX08 PREFIX09 PREFIX10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX00" "00" "PREFIX00 PREFIX01 PREFIX02 PREFIX03 PREFIX04 PREFIX05 PREFIX06 PREFIX07 PREFIX08 PREFIX09 PREFIX10";
then
	exit ${EXIT_FAILURE};
fi

if test_glob "PREFIX" "01" "PREFIX01 PREFIX02 PREFIX03 PREFIX04 PREFIX05 PREFIX06 PREFIX07 PREFIX08 PREFIX09 PREFIX10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX01" "01" "PREFIX01 PREFIX02 PREFIX03 PREFIX04 PREFIX05 PREFIX06 PREFIX07 PREFIX08 PREFIX09 PREFIX10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" "aa" "PREFIXaa PREFIXab PREFIXac PREFIXad PREFIXae PREFIXaf PREFIXag PREFIXah PREFIXai PREFIXaj";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIXaa" "aa" "PREFIXaa PREFIXab PREFIXac PREFIXad PREFIXae PREFIXaf PREFIXag PREFIXah PREFIXai PREFIXaj";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" "000" "PREFIX000 PREFIX001 PREFIX002 PREFIX003 PREFIX004 PREFIX005 PREFIX006 PREFIX007 PREFIX008 PREFIX009 PREFIX010";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX000" "000" "PREFIX000 PREFIX001 PREFIX002 PREFIX003 PREFIX004 PREFIX005 PREFIX006 PREFIX007 PREFIX008 PREFIX009 PREFIX010";
then
	exit ${EXIT_FAILURE};
fi

if test_glob "PREFIX" "001" "PREFIX001 PREFIX002 PREFIX003 PREFIX004 PREFIX005 PREFIX006 PREFIX007 PREFIX008 PREFIX009 PREFIX010";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX001" "001" "PREFIX001 PREFIX002 PREFIX003 PREFIX004 PREFIX005 PREFIX006 PREFIX007 PREFIX008 PREFIX009 PREFIX010";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" "aaa" "PREFIXaaa PREFIXaab PREFIXaac PREFIXaad PREFIXaae PREFIXaaf PREFIXaag PREFIXaah PREFIXaai PREFIXaaj";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIXaaa" "aaa" "PREFIXaaa PREFIXaab PREFIXaac PREFIXaad PREFIXaae PREFIXaaf PREFIXaag PREFIXaah PREFIXaai PREFIXaaj";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX001.asb" "001.asb" "PREFIX001.asb PREFIX002.asb PREFIX003.asb PREFIX004.asb PREFIX005.asb PREFIX006.asb PREFIX007.asb PREFIX008.asb PREFIX009.asb PREFIX010.asb";
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};

