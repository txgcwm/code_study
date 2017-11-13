include_directories(
)

add_definitions("-D_BSD_SOURCE -DRADIOTAP_SUPPORT_OVERRIDES")

file(GLOB_RECURSE HEADERFILES "../include/*")
aux_source_directory(. DIR_SRCS)
add_library(radiotap ${DIR_SRCS} ${HEADERFILES})