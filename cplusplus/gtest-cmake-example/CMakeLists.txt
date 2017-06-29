CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

set(TOPDIR ${CMAKE_CURRENT_SOURCE_DIR})

add_compile_options("--std=c++11")

set(OUTDIR ${TOPDIR}/cmake_build/out/)
set(BUILDDIR ${TOPDIR}/cmake_build/build)

add_subdirectory(${TOPDIR}/configure/ ${BUILDDIR}/configure/)