include_directories(
	${TOPDIR}/install/wolfssl/include/
)

file(GLOB_RECURSE HEADERFILES "../include/*")
aux_source_directory(. DIR_SRCS)
add_library(wfsslencry ${DIR_SRCS} ${HEADERFILES})
