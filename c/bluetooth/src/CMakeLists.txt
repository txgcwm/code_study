set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/src)

include_directories(
    ${TOPDIR}/3rdparty/prebuild/bluez/include
)

link_directories(
    ${TOPDIR}/3rdparty/prebuild/bluez/lib
)


#################################
add_executable(simplescan simplescan.c)
target_link_libraries(simplescan bluetooth-internal)