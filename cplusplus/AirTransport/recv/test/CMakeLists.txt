set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/recv/test)

include_directories(
	${TOPDIR}/recv/src
)

link_directories(
)


#################################
add_executable(TestAirData TestAirData.cpp)
target_link_libraries(TestAirData recv)


#################################
add_executable(TestPatternMatch TestPatternMatch.cpp)
target_link_libraries(TestPatternMatch recv)