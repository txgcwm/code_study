CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

set(TOPDIR ${CMAKE_CURRENT_SOURCE_DIR})

add_compile_options("--std=c++11")

set(OUTDIR ${TOPDIR}/cmake_build/out/)
set(BUILDDIR ${TOPDIR}/cmake_build/build)

set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/gtest-cmake-example/)

include_directories(
	${TOPDIR}/3rdparty/install/include/
)

link_directories(
	${TOPDIR}/3rdparty/install/lib/
)


#################################
add_executable(test_configure Configure.cpp ConfigureTest.cpp main.cpp)
target_link_libraries(test_configure gtest pthread)
