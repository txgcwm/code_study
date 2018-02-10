set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/test)

include_directories(
	${TOPDIR}/tinyxml
)

link_directories(
)


#################################
add_executable(test test.cpp)
target_link_libraries(test tinyxml)