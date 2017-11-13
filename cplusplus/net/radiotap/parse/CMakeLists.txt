set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/parse)

include_directories(
    ${TOPDIR}/radiotap-library
)

link_directories()


#################################
add_executable(ParseHeader ParseHeader.c)
target_link_libraries(ParseHeader radiotap)