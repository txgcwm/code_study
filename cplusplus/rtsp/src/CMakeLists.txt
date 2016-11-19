include_directories(
	${TOPDIR}/basics/base/include
	${TOPDIR}/transmit/rtsp/include
	${TOPDIR}/transmit/rtsp/srv
)

file(GLOB_RECURSE HEADERFILES "../include/*")
aux_source_directory(. DIR_SRCS)
add_library(rtspserver ${DIR_SRCS} ${HEADERFILES})