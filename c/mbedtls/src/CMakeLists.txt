set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/src)

include_directories(
    ${TOPDIR}/3rdparty/prebuild/mbedtls/include
)

link_directories(
	${TOPDIR}/3rdparty/prebuild/mbedtls/lib
)


#################################
add_executable(hmac-test hmac-test.c)
target_link_libraries(hmac-test mbedtls mbedcrypto mbedx509)