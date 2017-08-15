include_directories(
	${TOPDIR}/jsoncpp/include/
)

file(GLOB_RECURSE HEADERFILES "../include/*")
aux_source_directory(. DIR_SRCS)
add_library(jsoncpp ${DIR_SRCS} ${HEADERFILES})