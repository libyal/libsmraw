#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libsmraw/.libs/libsmraw.1.dylib && test -f ./pysmraw/.libs/pysmraw.so;
then
	install_name_tool -change /usr/local/lib/libsmraw.1.dylib ${PWD}/libsmraw/.libs/libsmraw.1.dylib ./pysmraw/.libs/pysmraw.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

