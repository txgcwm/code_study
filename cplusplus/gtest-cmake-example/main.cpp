#include <gtest/gtest.h> 



int main(int argc, char** argv)
{ 
	testing::InitGoogleTest(&argc, argv); 

    // Runs all tests using Google Test. 
    return RUN_ALL_TESTS(); 
}

// https://www.ibm.com/developerworks/cn/linux/l-cn-cppunittest/
// ./cmake_build/out/gtest-cmake-example/test_configure --gtest_filter=ConfigureTest.getSize