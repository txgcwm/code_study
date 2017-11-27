include_directories(
)

file(GLOB_RECURSE HEADERFILES "../include/*")
aux_source_directory(. DIR_SRCS)

add_library(airsend ${DIR_SRCS} ${HEADERFILES})