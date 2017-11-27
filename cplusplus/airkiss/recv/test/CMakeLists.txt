set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/test)

include_directories(
    ${TOPDIR}/3rdparty/prebuild/wireless_tools/include/
    ${TOPDIR}/src/
)

link_directories(
    ${TOPDIR}/3rdparty/prebuild/wireless_tools/lib
)


#################################
add_executable(airkiss main.c)
target_link_libraries(airkiss aircapture iw rt m)