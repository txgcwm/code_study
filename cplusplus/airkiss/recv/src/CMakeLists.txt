include_directories(
	${TOPDIR}/3rdparty/prebuild/wireless_tools/include/
)

file(GLOB_RECURSE HEADERFILES "../include/*")
aux_source_directory(. DIR_SRCS)
aux_source_directory(capture/ DIR_SRCS)
aux_source_directory(capture/radiotap DIR_SRCS)

add_library(aircapture ${DIR_SRCS} ${HEADERFILES})