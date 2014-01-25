#!/bin/sh

# pwd
# echo "SACAUX: $SACAUX"
# echo "SACBIN: $SACBIN"
# echo "SRCDIR: $TOP_SRCDIR"
# echo "BUILDDIR: $ABS_BUILDDIR"
#cd ${TOP_SRCDIR}/t/testsuite
TESTDIR="${ABS_TOP_SRCDIR}/t"
INPUT="${TESTDIR}/testsuite/input"
OUTPUT="${TESTDIR}/testsuite/output"
SAC_TO_TEST="${ABS_BUILDDIR}/${SACBIN}"
echo "Testing: ${SAC_TO_TEST}"
echo "        " `pwd`

# Checking for most recent version of python, >= 2.4
python="python"
echo "Checking for recent versions of python ...."
for v in 2.4 2.5 2.6 2.7; do
    echo "Looking for python version $v ..."
    python${v} -c 'True' 2>&1 > /dev/null
    RETVAL=$?
    if [ $RETVAL -eq 0 ]; then
	python=`which python${v}`
	echo "Found version $v at $python"
    fi
done
echo "python: $python"

#$python ../pytest.py -t -s $SAC_TO_TEST input/*
if [ ! -d testsuite ] ; then 
     mkdir testsuite
fi
cd testsuite 

cp -f ${INPUT}/*.m .
cp -f ${INPUT}/*.am .
cp -f ${INPUT}/RESP.* .
cp -f ${INPUT}/*.sac .
cp -f ${INPUT}/*.SAC .
cp -f ${INPUT}/*.GSE .
cp -f ${INPUT}/2002.054* .

TESTS=${INPUT}/*
if [ x"$SACTESTS" != x"" ]; then
    TESTS=${INPUT}/${SACTESTS}
fi

#
# This should be abstracted out for possible other tests of conditional compiliation
#
grep 'HAVE_LIBRPC_DISABLED 1' ../../inc/config.h
grep 'HAVE_LIBRPC_DISABLED 1' ../../inc/config.h > /dev/null
if [ $? -eq 0 ]; then
    echo "Removing xdr from Tests"
    NTESTS=""
    for t in $TESTS; do
        v=` echo $t | grep -v '/xdr$' `
        NTESTS="$NTESTS $v"
    done
    TESTS=$NTESTS
fi

DIE_OPTION=""
if [ x"$SAC_DIE_ON_ERROR" != x"" ]; then
   DIE_OPTION="--die-on-error"
fi

VALGRIND_OPTION=""
if [ x"$SACVALGRIND" != x"" ]; then
    VALGRIND_OPTION="--valgrind "
    echo "Removing help from Tests"
    NTESTS=""
    for t in $TESTS; do
        v=` echo $t | grep -v '/help$' `
        NTESTS="$NTESTS $v"
    done
    TESTS=$NTESTS
fi

$python ${TESTDIR}/pytest.py -t \
    -s $SAC_TO_TEST ${VALGRIND_OPTION} ${DIE_OPTION} \
    -o ${OUTPUT} ${TESTS}

