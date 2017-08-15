include_directories(
	${TOPDIR}/jsoncpp/include/
	${TOPDIR}/include/
)

file(GLOB_RECURSE HEADERFILES "../include/*")
aux_source_directory(. DIR_SRCS)
add_library(ap ${DIR_SRCS} ${HEADERFILES})