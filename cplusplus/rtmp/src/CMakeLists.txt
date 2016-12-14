include_directories(
	${TOPDIR}/basics/base/include
	${TOPDIR}/opensource/prebuild/include/rtmpdump/
)

file(GLOB_RECURSE HEADERFILES "../include/*")
aux_source_directory(. DIR_SRCS)
add_library(rtmpsend ${DIR_SRCS} ${HEADERFILES})