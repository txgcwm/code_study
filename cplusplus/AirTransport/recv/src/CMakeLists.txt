include_directories(
	
)

file(GLOB_RECURSE HEADERFILES "../include/*")
aux_source_directory(. DIR_SRCS)
add_library(recv ${DIR_SRCS} ${HEADERFILES})