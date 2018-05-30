include_directories(
	${TOPDIR}/include
	${TOPDIR}/src
)

file(GLOB_RECURSE HEADERFILES "../include/*")
aux_source_directory(. DIR_SRCS)
add_library(rtsp ${DIR_SRCS} ${HEADERFILES})