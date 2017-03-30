set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/send/test)

include_directories(
	${TOPDIR}/send/src
)

link_directories(
)


#################################
add_executable(TestAirTransport TestAirTransport.cpp)
target_link_libraries(TestAirTransport send pthread)

