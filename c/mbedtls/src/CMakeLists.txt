set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/src)

include_directories(
    ${TOPDIR}/3rdparty/prebuild/mbedtls/include
    ${TOPDIR}/3rdparty/prebuild/json-c/include
    ${TOPDIR}/3rdparty/prebuild/libubox/include
    ${TOPDIR}/3rdparty/prebuild/ubus/include
)

link_directories(
    ${TOPDIR}/3rdparty/prebuild/mbedtls/lib
    ${TOPDIR}/3rdparty/prebuild/json-c/lib
    ${TOPDIR}/3rdparty/prebuild/libubox/lib
    ${TOPDIR}/3rdparty/prebuild/ubus/lib
)


#################################
add_executable(hmac-test hmac-test.c)
target_link_libraries(hmac-test mbedtls mbedcrypto mbedx509)

#################################
add_executable(json-c-test json-c-test.c)
target_link_libraries(json-c-test json-c)

#################################
add_executable(libubox-ulog-test libubox-ulog-test.c)
target_link_libraries(libubox-ulog-test ubox)

#################################
add_executable(libubox-md5-test libubox-md5-test.c)
target_link_libraries(libubox-md5-test ubox)

#################################
add_executable(libubox-base64-test libubox-base64-test.c)
target_link_libraries(libubox-base64-test ubox)

#################################
add_executable(libubox-list-test libubox-list-test.c)
target_link_libraries(libubox-list-test ubox)

#################################
add_executable(libubox-usock-server libubox-usock-server.c)
target_link_libraries(libubox-usock-server ubox)

#################################
add_executable(libubox-usock-client libubox-usock-client.c)
target_link_libraries(libubox-usock-client ubox)

#################################
add_executable(hello_ubus hello_ubus.c)
target_link_libraries(hello_ubus ubus ubox blobmsg_json json-c)