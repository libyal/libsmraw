#!/bin/bash
# Library API write functions testing script
#
# Version: 20170802

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_FUNCTIONS="write";
OPTION_SETS="";

test_api_write_function()
{ 
	local TEST_FUNCTION=$1;
	shift 1;
	local ARGUMENTS=$@;

	local TEST_EXECUTABLE="./smraw_test_${TEST_FUNCTION}";

	if ! test -x "${TEST_EXECUTABLE}";
	then
		TEST_EXECUTABLE="./smraw_test_${TEST_FUNCTION}.exe";
	fi

	if ! test -x "${TEST_EXECUTABLE}";
	then
		echo "Missing test executable: ${TEST_EXECUTABLE}";

		return ${EXIT_FAILURE};
	fi
	TEST_DESCRIPTION="Testing write function: libsmraw_${TEST_FUNCTION}";

	local TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	run_test_with_arguments "${TEST_DESCRIPTION}" "${TEST_EXECUTABLE}" ${ARGUMENTS[@]} "${TMPDIR}";
	local RESULT=$?;

	rm -rf ${TMPDIR};

	return ${RESULT};
}

test_write()
{
	local TEST_FUNCTION=$1;

	test_api_write_function "${TEST_FUNCTION}";
	local RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		return ${RESULT};
	fi
	return ${RESULT};
}

if ! test -z ${SKIP_LIBRARY_TESTS};
then
	exit ${EXIT_IGNORE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -f "${TEST_RUNNER}";
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -f "${TEST_RUNNER}";
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

source ${TEST_RUNNER};

for TEST_FUNCTION in ${TEST_FUNCTIONS};
do
	test_write "${TEST_FUNCTION}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

exit ${RESULT};

