#!/usr/bin/env sh

PROG_NAME=ssltest
PASSWORD=bCCbPf61EoSFCwlnFNQ3A+jzxkaEuZptfKckWuewTTA=

test_compile()
{
    PREFIX=

    OPENSSL_CPPFLAGS=
    OPENSSL_CFLAGS=
    OPENSSL_LDFLAGS=-lcrypto

    WOLFSSL_CPPFLAGS=-D_USE_WOLFSSL
    WOLFSSL_CFLAGS=-I/srv/example/mine/code_study/cplusplus/aes/install/wolfssl/include
    WOLFSSL_LDFLAGS=/srv/example/mine/code_study/cplusplus/aes/install/wolfssl/lib/libwolfssl.a

    echo

    echo "compiling openssl version ..."
    ${PREFIX}g++ ${OPENSSL_CPPFLAGS} -o ${PROG_NAME}_openssl \
             ssltest.cpp \
             ${OPENSSL_CFLAGS} ${OPENSSL_LDFLAGS}

    echo "compiling wolfssl version ..."
    ${PREFIX}g++ ${WOLFSSL_CPPFLAGS} -o ${PROG_NAME}_wolfssl \
             ssltest.cpp \
             WolfsslEncryptDecrypt.cpp \
             ${WOLFSSL_CFLAGS} ${WOLFSSL_LDFLAGS}

    echo
}

test_test()
{
    ret=0

    repeat=$1
    enc_ver=$2
    dec_ver=$3

    repeat=$((repeat + 0))
    if [ ${repeat} -le 0 ]; then
        repeat=1
    fi

    if [ x"$enc_ver" = x"" ]; then
        enc_ver="openssl"
    fi
    if [ x"$dec_ver" = x"" ]; then
        dec_ver=${enc_ver}
    fi

    curr_cnt=0
    while [ $repeat -gt 0 ]; do
        echo
        curr_cnt=$((curr_cnt + 1))
        prt="[$curr_cnt] "
        echo "${prt} testing encrypt:${enc_ver} - decrypt:${dec_ver} ..."
        echo "${prt} generating test file ..."
        TMP_FILE=`mktemp /tmp/ssltest.XXXXXXXX`
        offset=`cat /dev/urandom | od -td2 | head -n 1 | sed -e 's/\s\+/ /g' | cut -d' ' -f2`
        #offset=$((`date +%N` % 1000 - 500))
        dd if=/dev/urandom of=${TMP_FILE} bs=$((5 * 1024 * 1024 + offset)) count=1
        echo "${prt} encryt:${enc_ver} test file ..."
        ./${PROG_NAME}_${enc_ver} encrypt ${PASSWORD} ${TMP_FILE} ${TMP_FILE}.enc > /dev/null
        echo "${prt} decryt:${dec_ver} test file ..."
        ./${PROG_NAME}_${dec_ver} decrypt ${PASSWORD} ${TMP_FILE}.enc ${TMP_FILE}.org > /dev/null
        echo "${prt} comparing the result ..."
        checksum=`md5sum ${TMP_FILE} | cut -d' ' -f1`
        checksum_org=`md5sum ${TMP_FILE}.org | cut -d' ' -f1`
        echo
        echo "${prt} ******************************************************************"
        echo "${prt} enc:${enc_ver} - dec:${dec_ver}"
        echo "${prt} $checksum vs $checksum_org"
        if [ x"$checksum" = x"$checksum_org" ]; then
            echo "${prt} ***   OK   ***"
        else
            echo "${prt} ***   Fail   ***"
            ret=1
        fi
        echo "${prt} ******************************************************************"
        echo

        rm ${TMP_FILE}
        rm ${TMP_FILE}.enc
        rm ${TMP_FILE}.org

        repeat=$((repeat - 1))
        if [ $ret -ne 0 ]; then
            break
        fi
    done

    return $ret
}

echo "-----------------------------------------------------------------"
test_compile
echo "-----------------------------------------------------------------"
test_test 5 openssl
echo "-----------------------------------------------------------------"
test_test 5 wolfssl

