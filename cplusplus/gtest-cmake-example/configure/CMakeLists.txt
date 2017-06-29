set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/gtest-cmake-example/configure)

include_directories(
	${TOPDIR}/3rdparty/install/include/
)

link_directories(
	${TOPDIR}/3rdparty/install/lib/
)


#################################
add_executable(test_configure Configure.cpp ConfigureTest.cpp main.cpp)
target_link_libraries(test_configure gtest pthread)