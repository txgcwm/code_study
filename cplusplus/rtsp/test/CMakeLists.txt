set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/transmit/rtsp/test)

include_directories(
	${TOPDIR}/basics/base/include
    ${TOPDIR}/transmit/rtsp/include
)

link_directories(
)


#################################
add_executable(TestRtspServer TestRtspServer.cpp)
target_link_libraries(TestRtspServer rtspserver base pthread)


