set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/test)

include_directories(
	${TOPDIR}/src
	${TOPDIR}/install/wolfssl/include/
)

link_directories(
	${TOPDIR}/install/wolfssl/lib/
)


#################################
add_executable(TestWsslEnDecrypt TestWsslEnDecrypt.cpp)
target_link_libraries(TestWsslEnDecrypt wfsslencry wolfssl pthread)

#################################
add_executable(TestAes TestAes.cpp)
target_link_libraries(TestAes wfsslencry wolfssl pthread)


#################################
add_executable(TestSslAes TestSslAes.cpp)
target_link_libraries(TestSslAes wfsslencry wolfssl ssl crypto pthread)