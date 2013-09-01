#!/bin/bash
#
# Library write testing script
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

test_write()
{ 
	rm -rf tmp;
	mkdir tmp;

	echo "Testing write";

	${TEST_RUNNER} ./${SMRAW_TEST_WRITE} tmp/;

	RESULT=$?;

	echo "";

	rm -rf tmp;

	return ${RESULT};
}

SMRAW_TEST_WRITE="smraw_test_write";

if ! test -x ${SMRAW_TEST_WRITE};
then
	echo "Missing executable: ${SMRAW_TEST_WRITE}";

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

if ! test_write
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};

