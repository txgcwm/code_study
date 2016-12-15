set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/transmit/rtmp/test)

include_directories(
	${TOPDIR}/basics/base/include
	${TOPDIR}/transmit/rtmp/src
	${TOPDIR}/opensource/prebuild/include/rtmpdump/
)

link_directories(
	${TOPDIR}/opensource/prebuild/libs/${OPENSOURCE_LIB_DIR}
)


#################################
add_executable(TestRtmpRecv TestRtmpRecv.cpp)
target_link_libraries(TestRtmpRecv rtmpsend rtmp ssl crypto z base pthread)


#################################
add_executable(TestLiveRtmpStream TestLiveRtmpStream.cpp)
target_link_libraries(TestLiveRtmpStream rtmpsend rtmp ssl crypto z base pthread)