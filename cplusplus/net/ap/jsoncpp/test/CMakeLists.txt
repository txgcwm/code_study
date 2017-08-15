set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/test)

include_directories(
	${TOPDIR}/jsoncpp/include/
)

link_directories(
	#${TOPDIR}/3rdparty/install/lib/
)


#################################
add_executable(TestJson TestJson.cpp)
target_link_libraries(TestJson jsoncpp)
