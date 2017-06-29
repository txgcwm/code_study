set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/gtest-cmake-example/environment)

include_directories(
	${TOPDIR}/3rdparty/install/include/
)

link_directories(
	${TOPDIR}/3rdparty/install/lib/
)


#################################
add_executable(test_environment Configure.cpp ConfigureTest.cpp main.cpp)
target_link_libraries(test_environment gtest pthread)