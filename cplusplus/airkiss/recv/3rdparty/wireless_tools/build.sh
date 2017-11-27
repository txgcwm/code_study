#!/bin/bash

CROSSPREFIX=
CC=${CROSSPREFIX}gcc
CXX=${CROSSPREFIX}g++
LD=${CROSSPREFIX}ld
AR=${CROSSPREFIX}ar
NM=${CROSSPREFIX}nm
RANLIB=${CROSSPREFIX}ranlib
STRIP=${CROSSPREFIX}strip

CURDIR=`pwd`
PACKAGE=wireless_tools.29

if [ ! -d ${PACKAGE} ]; then
	tar -zxvf ${PACKAGE}.tar.gz
fi

cd ${PACKAGE}/
export BUILD_STATIC=y
export PREFIX=${CURDIR}/../prebuild/wireless_tools
make -j8 CC=${CC} AR=${AR} || exit 1
make install
cd ..

rm -rf ${PACKAGE}
