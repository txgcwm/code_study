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


#################################
add_executable(TestCombination TestCombination.cpp)
target_link_libraries(TestCombination)


#################################
add_executable(TestArray TestArray.cpp)
target_link_libraries(TestArray)


#################################
add_executable(TestSeq TestSeq.cpp)
target_link_libraries(TestSeq)