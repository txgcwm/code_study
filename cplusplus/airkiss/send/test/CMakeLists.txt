set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/test)

include_directories(
    ${TOPDIR}/src/
)

link_directories(
)


#################################
add_executable(TestAirKiss TestAirKiss.cpp)
target_link_libraries(TestAirKiss airsend pthread)