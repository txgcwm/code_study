CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

set(TOPDIR ${CMAKE_CURRENT_SOURCE_DIR})

set(OUTDIR ${TOPDIR}/out/)
set(BUILDDIR ${TOPDIR}/build/)

add_subdirectory(${TOPDIR}/src ${BUILDDIR}/src)
add_subdirectory(${TOPDIR}/test ${BUILDDIR}/test)
