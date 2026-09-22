#!/bin/sh

TESTDIR="${ABS_TOP_SRCDIR}/t"
INPUT="${TESTDIR}/testsuite/input"

cp -f ${INPUT}/*.m .
cp -f ${INPUT}/*.am .
cp -f ${INPUT}/RESP.* .
cp -f ${INPUT}/*.sac .
cp -f ${INPUT}/*.SAC .
cp -f ${INPUT}/*.GSE .
cp -f ${INPUT}/*.xml .
cp -f ${INPUT}/2002.054* .
cp -f ${TESTDIR}/*.txt .
cp -f ${INPUT}/set_picks.sm .
cp -f ${INPUT}/test_fd.* .

exit 0

