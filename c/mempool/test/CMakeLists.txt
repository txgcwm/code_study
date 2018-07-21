set(EXECUTABLE_OUTPUT_PATH ${OUTDIR}/test)

include_directories(
    ${TOPDIR}/include
    ${TOPDIR}/src
)

link_directories(
)


#################################
add_executable(buddy_test buddy_test.c)
target_link_libraries(buddy_test mempool)


#################################
add_executable(pow_test pow_test.c)
target_link_libraries(pow_test mempool)


#################################
add_executable(mempool_test mempool_test.c)
target_link_libraries(mempool_test mempool)


#################################
add_executable(list_test list_test.c)
target_link_libraries(list_test mempool)


#################################
add_executable(data_test data_test.c)
target_link_libraries(data_test mempool)