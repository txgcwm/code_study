#! /bin/bash



source param.mk

CURDIR=`pwd`

mkdir out

tar -xzvf ${PCAPFILE}

cd ${PCAPDIR}
./configure --enable-static --prefix=${CURDIR}/out
make
make install
cd ..

cp -rf out/include/* include/pcap
cp out/lib/libpcap.a lib/

rm -rf out/*


tar -xzvf ${LIBNETFILE}

cd ${LIBNETDIR}
./configure --prefix=${CURDIR}/out
make
make install
cd ..

cp -rf out/include/* include/libnet
cp out/lib/libnet.a lib/

rm -rf out/