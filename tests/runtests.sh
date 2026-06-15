#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libsmraw/.libs/libsmraw.1.dylib && test -f ./pysmraw/.libs/pysmraw.so
then
	install_name_tool -change /usr/local/lib/libsmraw.1.dylib ${PWD}/libsmraw/.libs/libsmraw.1.dylib ./pysmraw/.libs/pysmraw.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

