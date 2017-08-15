set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/test)

include_directories(
	${TOPDIR}/include/
)

link_directories(
	#${TOPDIR}/3rdparty/install/lib/
)


#################################
add_executable(TestAes TestAes.cpp)
target_link_libraries(TestAes ap)

#################################
add_executable(TestClient TestClient.cpp)
target_link_libraries(TestClient ap)

#################################
add_executable(TestServer TestServer.cpp)
target_link_libraries(TestServer ap)