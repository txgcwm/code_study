set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/test)

include_directories(
    ${TOPDIR}/include
    ${TOPDIR}/src
)

link_directories(
)


#################################
add_executable(TestRtspServer TestRtspServer.cpp)
target_link_libraries(TestRtspServer rtsp pthread)


