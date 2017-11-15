set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/src)

include_directories(
    ${TOPDIR}/3rdparty/prebuild/mbedtls/include
    ${TOPDIR}/3rdparty/prebuild/json-c/include
)

link_directories(
    ${TOPDIR}/3rdparty/prebuild/mbedtls/lib
    ${TOPDIR}/3rdparty/prebuild/json-c/lib
)


#################################
add_executable(hmac-test hmac-test.c)
target_link_libraries(hmac-test mbedtls mbedcrypto mbedx509)

#################################
add_executable(json-c-test json-c-test.c)
target_link_libraries(json-c-test json-c)