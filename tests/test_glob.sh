#!/usr/bin/env bash
# Library glob testing script
#
# Version: 20260606

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

seq()
{
	local VALUE=$1;
	local SEQUENCE="";

	while test ${VALUE} -le $2;
	do
		SEQUENCE="${SEQUENCE} ${VALUE}";

		VALUE=`expr ${VALUE} + 1`;
	done

	echo ${SEQUENCE};
}

test_generate_test_files()
{
	TMPDIR=$1;
	FILENAMES=$2;
	INPUT=$3;

	TEST_PATH="${TMPDIR}/${BASENAME}";
	TEST_FILENAMES=`echo ${FILENAMES} | sed "s?^?${TMPDIR}/?;s? ? ${TMPDIR}/?g"`;

	touch ${TEST_FILENAMES};

	OSTYPE_LOWER=`echo "$OSTYPE" | tr '[A-Z]' '[a-z]'`;
	case "${OSTYPE_LOWER}" in
		cygwin*|msys*)
			if test ${TESTS_USE_WINAPI} = "yes" || test "${MSYSTEM}" = "MINGW32" || test "${MSYSTEM}" = "MINGW64";
			then
				WINDOWS_FILENAMES="";

				for TEST_FILENAME in ${TEST_FILENAMES};
				do
					cygpath -w "${TEST_FILENAME}" >> ${INPUT};
				done

				TEST_PATH=`cygpath -w "${TEST_PATH}"`;
			else
				echo "${TEST_FILENAMES}" | tr ' ' '\n' > ${INPUT};
			fi
			;;

		linux-gnu*)
			if test -n "$WSL_DISTRO_NAME" || grep -qi "microsoft" /proc/version 2>/dev/null;
			then
				echo "WSL currently not supported";

				exit ${EXIT_IGNORE};
			fi
			if ! test -x ${OBJDUMP};
			then
				echo "Missing executable: ${OBJDUMP}";

				exit ${EXIT_IGNORE};
			fi
			if test ${TESTS_USE_WINAPI} = "yes" || ${OBJDUMP} -f "${TEST_EXECUTABLE}" 2>&1 | grep -q "pei-";
			then
				WINDOWS_FILENAMES="";

				for TEST_FILENAME in ${TEST_FILENAMES};
				do
					winepath -w "${TEST_FILENAME}" >> ${INPUT} 2>/dev/null;
				done

				TEST_PATH=`winepath -w "${TEST_PATH}" 2>/dev/null`;
			else
				echo "${TEST_FILENAMES}" | tr ' ' '\n' > ${INPUT};
			fi
			;;

		*)
			if test ${TESTS_USE_WINAPI} = "yes";
			then
				echo "WINAPI not supported on ${OSTYPE}";

				exit ${EXIT_IGNORE};
			else
				echo "${TEST_FILENAMES}" | tr ' ' '\n' > ${INPUT};
			fi
			;;
	esac
}

test_compare()
{
	INPUT=$1;
	OUTPUT=$2;

	OSTYPE_LOWER=`echo "$OSTYPE" | tr '[A-Z]' '[a-z]'`;
	case "${OSTYPE_LOWER}" in
		cygwin*|msys*)
			if test ${TESTS_USE_WINAPI} = "yes" || test "${MSYSTEM}" = "MINGW32" || test "${MSYSTEM}" = "MINGW64";
			then
		 		sed -i'~' 's/\r$//' ${OUTPUT};
			fi
			;;

		linux-gnu*)
			if test ${TESTS_USE_WINAPI} = "yes" || ${OBJDUMP} -f "${TEST_EXECUTABLE}" 2>&1 | grep -q "pei-";
			then
		 		sed -i'~' 's/\r$//' ${OUTPUT};
			fi
			;;

		*)
			;;
	esac
	if ! cmp -s ${INPUT} ${OUTPUT};
	then
		diff ${INPUT} ${OUTPUT};

		return ${EXIT_FAILURE};
	fi
	return ${EXIT_SUCCESS};
}

test_glob()
{
	local EXPECTED_RESULT=$1;
	local BASENAME=$2;
	local SCHEMA=$3;
	local FILENAMES=$4;

	local TEST_DESCRIPTION="Testing glob: for basename: ${BASENAME} and schema: ${SCHEMA} ";

	local TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	test_generate_test_files "${TMPDIR}" "${FILENAMES}" "${TMPDIR}/input";

	run_test_with_arguments "" "${TEST_EXECUTABLE}" "${TEST_PATH}" > "${TMPDIR}/output";
	local RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		test_compare "${TMPDIR}/input" "${TMPDIR}/output";
		RESULT=$?;
	fi

	rm -rf ${TMPDIR};

	echo -n "${TEST_DESCRIPTION} ";

	if test ${RESULT} -ne ${EXPECTED_RESULT};
	then
		echo " (FAIL)";

		return ${EXIT_FAILURE};
	fi
	echo " (PASS)";

	return ${EXIT_SUCCESS};
}

test_glob_sequence()
{
	local EXPECTED_RESULT=$1;
	local BASENAME=$2;
	local SCHEMA=$3;
	local FILENAME=$4;
	local LAST=$5;

	local SEQUENCE=`seq 1 ${LAST}`;
	local FILENAMES=`for NUMBER in ${SEQUENCE}; do echo -n "${FILENAME}.${NUMBER}of${LAST} "; echo $FILE; done`;

	local TEST_DESCRIPTION="Testing glob: for basename: ${BASENAME} and schema: ${SCHEMA} ";

	local TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	test_generate_test_files "${TMPDIR}" "${FILENAMES}" "${TMPDIR}/input";

	run_test_with_arguments "" "${TEST_EXECUTABLE}" "${TEST_PATH}" > "${TMPDIR}/output";
	local RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		test_compare "${TMPDIR}/input" "${TMPDIR}/output";
		RESULT=$?;
	fi
	rm -rf ${TMPDIR};

	echo -n "${TEST_DESCRIPTION} ";

	if test ${RESULT} -ne ${EXPECTED_RESULT};
	then
		echo " (FAIL)";

		return ${EXIT_FAILURE};
	fi
	echo " (PASS)";

	return ${EXIT_SUCCESS};
}

if ! test -z ${SKIP_LIBRARY_TESTS};
then
	exit ${EXIT_IGNORE};
fi

TEST_EXECUTABLE="./smraw_test_glob";

if ! test -x "${TEST_EXECUTABLE}";
then
	TEST_EXECUTABLE="./smraw_test_glob.exe";
fi

if ! test -x "${TEST_EXECUTABLE}";
then
	echo "Missing test executable: ${TEST_EXECUTABLE}";

	exit ${EXIT_FAILURE};
fi

TEST_DIRECTORY=`dirname $0`;

TEST_RUNNER="${TEST_DIRECTORY}/test_runner.sh";

if ! test -f "${TEST_RUNNER}";
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

source ${TEST_RUNNER};

if ! test_glob ${EXIT_SUCCESS} "PREFIX" ".dd" "PREFIX.dd";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.dd" ".dd" "PREFIX.dd";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" ".dmg" "PREFIX.dmg";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.dmg" ".dmg" "PREFIX.dmg";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" ".img" "PREFIX.img";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.img" ".img" "PREFIX.img";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" ".raw" "PREFIX.raw";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.raw" ".raw" "PREFIX.raw";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" ".0" "PREFIX.0 PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.0" ".0" "PREFIX.0 PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" ".1" "PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.1" ".1" "PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" ".0" "PREFIX.0 PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.0" ".0" "PREFIX.0 PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" ".1" "PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.1" ".1" "PREFIX.1 PREFIX.2 PREFIX.3 PREFIX.4 PREFIX.5 PREFIX.6 PREFIX.7 PREFIX.8 PREFIX.9 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" ".00" "PREFIX.00 PREFIX.01 PREFIX.02 PREFIX.03 PREFIX.04 PREFIX.05 PREFIX.06 PREFIX.07 PREFIX.08 PREFIX.09 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.00" ".00" "PREFIX.00 PREFIX.01 PREFIX.02 PREFIX.03 PREFIX.04 PREFIX.05 PREFIX.06 PREFIX.07 PREFIX.08 PREFIX.09 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" ".01" "PREFIX.01 PREFIX.02 PREFIX.03 PREFIX.04 PREFIX.05 PREFIX.06 PREFIX.07 PREFIX.08 PREFIX.09 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.01" ".01" "PREFIX.01 PREFIX.02 PREFIX.03 PREFIX.04 PREFIX.05 PREFIX.06 PREFIX.07 PREFIX.08 PREFIX.09 PREFIX.10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" ".000" "PREFIX.000 PREFIX.001 PREFIX.002 PREFIX.003 PREFIX.004 PREFIX.005 PREFIX.006 PREFIX.007 PREFIX.008 PREFIX.009 PREFIX.010";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.000" ".000" "PREFIX.000 PREFIX.001 PREFIX.002 PREFIX.003 PREFIX.004 PREFIX.005 PREFIX.006 PREFIX.007 PREFIX.008 PREFIX.009 PREFIX.010";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" ".001" "PREFIX.001 PREFIX.002 PREFIX.003 PREFIX.004 PREFIX.005 PREFIX.006 PREFIX.007 PREFIX.008 PREFIX.009 PREFIX.010";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.001" ".001" "PREFIX.001 PREFIX.002 PREFIX.003 PREFIX.004 PREFIX.005 PREFIX.006 PREFIX.007 PREFIX.008 PREFIX.009 PREFIX.010";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if ! test_glob ${EXIT_FAILURE} "PREFIX" "0" "PREFIX0 PREFIX1 PREFIX2 PREFIX3 PREFIX4 PREFIX5 PREFIX6 PREFIX7 PREFIX8 PREFIX9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX0" "0" "PREFIX0 PREFIX1 PREFIX2 PREFIX3 PREFIX4 PREFIX5 PREFIX6 PREFIX7 PREFIX8 PREFIX9";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if ! test_glob ${EXIT_FAILURE} "PREFIX" "1" "PREFIX1 PREFIX2 PREFIX3 PREFIX4 PREFIX5 PREFIX6 PREFIX7 PREFIX8 PREFIX9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX1" "1" "PREFIX1 PREFIX2 PREFIX3 PREFIX4 PREFIX5 PREFIX6 PREFIX7 PREFIX8 PREFIX9";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" "00" "PREFIX00 PREFIX01 PREFIX02 PREFIX03 PREFIX04 PREFIX05 PREFIX06 PREFIX07 PREFIX08 PREFIX09 PREFIX10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX00" "00" "PREFIX00 PREFIX01 PREFIX02 PREFIX03 PREFIX04 PREFIX05 PREFIX06 PREFIX07 PREFIX08 PREFIX09 PREFIX10";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if ! test_glob ${EXIT_FAILURE} "PREFIX" "01" "PREFIX01 PREFIX02 PREFIX03 PREFIX04 PREFIX05 PREFIX06 PREFIX07 PREFIX08 PREFIX09 PREFIX10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX01" "01" "PREFIX01 PREFIX02 PREFIX03 PREFIX04 PREFIX05 PREFIX06 PREFIX07 PREFIX08 PREFIX09 PREFIX10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" "aa" "PREFIXaa PREFIXab PREFIXac PREFIXad PREFIXae PREFIXaf PREFIXag PREFIXah PREFIXai PREFIXaj";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIXaa" "aa" "PREFIXaa PREFIXab PREFIXac PREFIXad PREFIXae PREFIXaf PREFIXag PREFIXah PREFIXai PREFIXaj";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" "000" "PREFIX000 PREFIX001 PREFIX002 PREFIX003 PREFIX004 PREFIX005 PREFIX006 PREFIX007 PREFIX008 PREFIX009 PREFIX010";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX000" "000" "PREFIX000 PREFIX001 PREFIX002 PREFIX003 PREFIX004 PREFIX005 PREFIX006 PREFIX007 PREFIX008 PREFIX009 PREFIX010";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if ! test_glob ${EXIT_FAILURE} "PREFIX" "001" "PREFIX001 PREFIX002 PREFIX003 PREFIX004 PREFIX005 PREFIX006 PREFIX007 PREFIX008 PREFIX009 PREFIX010";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX001" "001" "PREFIX001 PREFIX002 PREFIX003 PREFIX004 PREFIX005 PREFIX006 PREFIX007 PREFIX008 PREFIX009 PREFIX010";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX" "aaa" "PREFIXaaa PREFIXaab PREFIXaac PREFIXaad PREFIXaae PREFIXaaf PREFIXaag PREFIXaah PREFIXaai PREFIXaaj";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIXaaa" "aaa" "PREFIXaaa PREFIXaab PREFIXaac PREFIXaad PREFIXaae PREFIXaaf PREFIXaag PREFIXaah PREFIXaai PREFIXaaj";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX001.asb" "001.asb" "PREFIX001.asb PREFIX002.asb PREFIX003.asb PREFIX004.asb PREFIX005.asb PREFIX006.asb PREFIX007.asb PREFIX008.asb PREFIX009.asb PREFIX010.asb";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX-f001.vmdk" "001.vmdk" "PREFIX-f001.vmdk PREFIX-f002.vmdk PREFIX-f003.vmdk PREFIX-f004.vmdk PREFIX-f005.vmdk PREFIX-f006.vmdk PREFIX-f007.vmdk PREFIX-f008.vmdk PREFIX-f009.vmdk PREFIX-f010.vmdk";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if ! test_glob ${EXIT_FAILURE} "PREFIX" ".1of5" "PREFIX.1of5 PREFIX.2of5 PREFIX.3of5 PREFIX.4of5 PREFIX.5of5";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.1of5" ".1of5" "PREFIX.1of5 PREFIX.2of5 PREFIX.3of5 PREFIX.4of5 PREFIX.5of5";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if ! test_glob ${EXIT_FAILURE} "PREFIX.1of5" ".1of5" "PREFIX.1of5 PREFIX.2of5 PREFIX.3of5 PREFIX.4of5";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if ! test_glob ${EXIT_FAILURE} "PREFIX.1of5" ".1of5" "PREFIX.1of5 PREFIX.2of5 PREFIX.3of5 PREFIX.4of5 PREFIX.5of5 PREFIX.6of5";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if ! test_glob ${EXIT_FAILURE} "PREFIX" ".1of10" "PREFIX.1of10 PREFIX.2of10 PREFIX.3of10 PREFIX.4of10 PREFIX.5of10 PREFIX.6of10 PREFIX.7of10 PREFIX.8of10 PREFIX.9of10 PREFIX.10of10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.1of10" ".1of10" "PREFIX.1of10 PREFIX.2of10 PREFIX.3of10 PREFIX.4of10 PREFIX.5of10 PREFIX.6of10 PREFIX.7of10 PREFIX.8of10 PREFIX.9of10 PREFIX.10of10";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if ! test_glob ${EXIT_FAILURE} "PREFIX.1of10" ".1of10" "PREFIX.1of10 PREFIX.2of10 PREFIX.3of10 PREFIX.4of10 PREFIX.5of10 PREFIX.6of10 PREFIX.7of10 PREFIX.8of10 PREFIX.9of10";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if ! test_glob ${EXIT_FAILURE} "PREFIX.1of10" ".1of10" "PREFIX.1of10 PREFIX.2of10 PREFIX.3of10 PREFIX.4of10 PREFIX.5of10 PREFIX.6of10 PREFIX.7of10 PREFIX.8of10 PREFIX.9of10 PREFIX.10of10 PREFIX.11of10";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if ! test_glob ${EXIT_FAILURE} "PREFIX" ".01of10" "PREFIX.01of10 PREFIX.02of10 PREFIX.03of10 PREFIX.04of10 PREFIX.05of10 PREFIX.06of10 PREFIX.07of10 PREFIX.08of10 PREFIX.09of10 PREFIX.10of10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob ${EXIT_SUCCESS} "PREFIX.01of10" ".01of10" "PREFIX.01of10 PREFIX.02of10 PREFIX.03of10 PREFIX.04of10 PREFIX.05of10 PREFIX.06of10 PREFIX.07of10 PREFIX.08of10 PREFIX.09of10 PREFIX.10of10";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if ! test_glob ${EXIT_FAILURE} "PREFIX.01of10" ".01of10" "PREFIX.01of10 PREFIX.02of10 PREFIX.03of10 PREFIX.04of10 PREFIX.05of10 PREFIX.06of10 PREFIX.07of10 PREFIX.08of10 PREFIX.09of10";
then
	exit ${EXIT_FAILURE};
fi

# This test should fail
if ! test_glob ${EXIT_FAILURE} "PREFIX.01of10" ".01of10" "PREFIX.01of10 PREFIX.02of10 PREFIX.03of10 PREFIX.04of10 PREFIX.05of10 PREFIX.06of10 PREFIX.07of10 PREFIX.08of10 PREFIX.09of10 PREFIX.10of10 PREFIX.11of10";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob_sequence ${EXIT_SUCCESS} "PREFIX.1of192" ".1of192" "PREFIX" "192";
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};

