cmake_minimum_required(VERSION 3.10)

project(dev)

# Add include directory
include_directories(include)

# Add source directory
add_library(dev STATIC src/dev.cpp)

# Add Google Test
enable_testing()
add_subdirectory(libs/googletest)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test directory
file(GLOB TEST_SRCS test/*.cpp)
add_executable(dev_tests ${TEST_SRCS})
target_link_libraries(dev_tests dev gtest_main)
add_test(dev_tests dev_tests)

# Add POCO
add_subdirectory(libs/poco)

# Add boost
add_subdirectory(libs/boost)

# Add dependencies
target_link_libraries(dev Poco::Foundation Poco::Util Boost::system Boost::filesystem)
