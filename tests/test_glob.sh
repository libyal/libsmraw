#!/bin/bash
#
# Library glob testing script
#
# Copyright (c) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
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

seq()
{
	VALUE=$1;
	SEQUENCE="";

	while [ ${VALUE} -le $2 ];
	do
		SEQUENCE="${SEQUENCE} ${VALUE}";

		VALUE=`expr ${VALUE} + 1`;
	done

	echo ${SEQUENCE};
}

test_glob()
{ 
	BASENAME=$1;
	SCHEMA=$2;
	FILENAMES=$3;

	FILENAMES=`echo ${FILENAMES} | sed 's?^?tmp/?' | sed 's? ? tmp/?g'`;

	rm -rf tmp;
	mkdir tmp;

	echo ${FILENAMES} > tmp/input;

	touch ${FILENAMES};

	${TEST_RUNNER} ./${SMRAW_TEST_GLOB} tmp/${BASENAME} > tmp/output;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		if ! cmp -s tmp/input tmp/output;
		then
			RESULT=${EXIT_FAILURE};
		fi
	fi

	rm -rf tmp;

	echo -n "Testing glob: for basename: ${BASENAME} and schema: ${SCHEMA} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

test_glob_sequence()
{ 
	BASENAME=$1;
	SCHEMA=$2;
	FILENAME=$3;
	LAST=$4;

	SEQUENCE=`seq 1 ${LAST}`;
	FILENAMES=`for NUMBER in ${SEQUENCE}; do echo -n "${FILENAME}.${NUMBER}of${LAST} "; echo $FILE; done`;

	FILENAMES=`echo ${FILENAMES} | sed 's?^?tmp/?' | sed 's? ? tmp/?g'`;

	rm -rf tmp;
	mkdir tmp;

	echo ${FILENAMES} > tmp/input;

	touch ${FILENAMES};

	${TEST_RUNNER} ./${SMRAW_TEST_GLOB} tmp/${BASENAME} > tmp/output;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		if ! cmp -s tmp/input tmp/output;
		then
			RESULT=${EXIT_FAILURE};
		fi
	fi

	rm -rf tmp;

	echo -n "Testing glob: for basename: ${BASENAME} and schema: ${SCHEMA} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

SMRAW_TEST_GLOB="smraw_test_glob";

if ! test -x ${SMRAW_TEST_GLOB};
then
	echo "Missing executable: ${SMRAW_TEST_GLOB}";

	exit ${EXIT_FAILURE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -x ${TEST_RUNNER};
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -x ${TEST_RUNNER};
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" ".dd" "PREFIX.dd";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.dd" ".dd" "PREFIX.dd";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" ".dmg" "PREFIX.dmg";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.dmg" ".dmg" "PREFIX.dmg";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" ".img" "PREFIX.img";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.img" ".img" "PREFIX.img";
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

if ! test_glob "PREFIX" ".0" "PREFIX.0 PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.0" ".0" "PREFIX.0 PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX" ".1" "PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.1" ".1" "PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9 PREFIX.10";
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

# This test should fail
if test_glob "PREFIX" "0" "PREFIX0 PREFIX1 PREFIX2 PREFIX3 PREFIX4 PREFIX5 PREFIX6 PREFIX7 PREFIX8 PREFIX9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX0" "0" "PREFIX0 PREFIX1 PREFIX2 PREFIX3 PREFIX4 PREFIX5 PREFIX6 PREFIX7 PREFIX8 PREFIX9";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
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

# This test should fail
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

# This test should fail
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

if ! test_glob "PREFIX-f001.vmdk" "001.vmdk" "PREFIX-f001.vmdk PREFIX-f002.vmdk PREFIX-f003.vmdk PREFIX-f004.vmdk PREFIX-f005.vmdk PREFIX-f006.vmdk PREFIX-f007.vmdk PREFIX-f008.vmdk PREFIX-f009.vmdk PREFIX-f010.vmdk";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if test_glob "PREFIX" ".1of5" "PREFIX.1of5 PREFIX.2of5 PREFIX.3of5 PREFIX.4of5 PREFIX.5of5";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.1of5" ".1of5" "PREFIX.1of5 PREFIX.2of5 PREFIX.3of5 PREFIX.4of5 PREFIX.5of5";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if test_glob "PREFIX.1of5" ".1of5" "PREFIX.1of5 PREFIX.2of5 PREFIX.3of5 PREFIX.4of5";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if test_glob "PREFIX.1of5" ".1of5" "PREFIX.1of5 PREFIX.2of5 PREFIX.3of5 PREFIX.4of5 PREFIX.5of5 PREFIX.6of5";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if test_glob "PREFIX" ".1of10" "PREFIX.1of10 PREFIX.2of10 PREFIX.3of10 PREFIX.4of10 PREFIX.5of10 PREFIX.6of10 PREFIX.7of10 PREFIX.8of10 PREFIX.9of10 PREFIX.10of10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.1of10" ".1of10" "PREFIX.1of10 PREFIX.2of10 PREFIX.3of10 PREFIX.4of10 PREFIX.5of10 PREFIX.6of10 PREFIX.7of10 PREFIX.8of10 PREFIX.9of10 PREFIX.10of10";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if test_glob "PREFIX.1of10" ".1of10" "PREFIX.1of10 PREFIX.2of10 PREFIX.3of10 PREFIX.4of10 PREFIX.5of10 PREFIX.6of10 PREFIX.7of10 PREFIX.8of10 PREFIX.9of10";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if test_glob "PREFIX.1of10" ".1of10" "PREFIX.1of10 PREFIX.2of10 PREFIX.3of10 PREFIX.4of10 PREFIX.5of10 PREFIX.6of10 PREFIX.7of10 PREFIX.8of10 PREFIX.9of10 PREFIX.10of10 PREFIX.11of10";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if test_glob "PREFIX" ".01of10" "PREFIX.01of10 PREFIX.02of10 PREFIX.03of10 PREFIX.04of10 PREFIX.05of10 PREFIX.06of10 PREFIX.07of10 PREFIX.08of10 PREFIX.09of10 PREFIX.10of10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.01of10" ".01of10" "PREFIX.01of10 PREFIX.02of10 PREFIX.03of10 PREFIX.04of10 PREFIX.05of10 PREFIX.06of10 PREFIX.07of10 PREFIX.08of10 PREFIX.09of10 PREFIX.10of10";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if test_glob "PREFIX.01of10" ".01of10" "PREFIX.01of10 PREFIX.02of10 PREFIX.03of10 PREFIX.04of10 PREFIX.05of10 PREFIX.06of10 PREFIX.07of10 PREFIX.08of10 PREFIX.09of10";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if test_glob "PREFIX.01of10" ".01of10" "PREFIX.01of10 PREFIX.02of10 PREFIX.03of10 PREFIX.04of10 PREFIX.05of10 PREFIX.06of10 PREFIX.07of10 PREFIX.08of10 PREFIX.09of10 PREFIX.10of10 PREFIX.11of10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob_sequence "PREFIX.1of192" ".1of192" "PREFIX" "192";
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};

