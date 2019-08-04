#!/bin/sh

MSEED_V="3.0.7"
SACIO_V="1.0.2"
MSEED="libmseed-v${MSEED_V}"
SACIO="sacio-v${SACIO_V}"

if [ ! -e libmseed ]; then
    echo "Downloading ${MSEED}.tar.gz"
    wget -q -O ${MSEED}.tar.gz https://github.com/iris-edu/libmseed/archive/v${MSEED_V}.tar.gz
    tar zxf ${MSEED}.tar.gz
    mv libmseed-${MSEED_V} libmseed
fi
if [ ! -e sacio ]; then
    echo "Downloading ${SACIO}.tar.gz"
    wget -q -O ${SACIO}.tar.gz https://github.com/savage13/sacio/tarball/master
    mkdir sacio 
    tar zxf ${SACIO}.tar.gz -C sacio --strip-components 1
    if [ ! -e sacio ] ; then
        echo "Error making sacio directory"
        exit -1
    fi
    if [ ! -e sacio/configure ]; then
        echo "Error unpacking sacio distribution"
        exit -1
    fi
fi

if [ ! -e libmseed/libmseed.a ]; then
    echo "Compiling libmseed"
    cd libmseed
    make -s
    cd ..
fi

if [ ! -e sacio/libsacio_bsd.a ]; then
    echo "Compiling sacio"
    cd sacio
    ./configure && make -s && make -s test
    cd ..
fi

./configure LDFLAGS="-Llibmseed -Lsacio" && make && make test
