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
#file(GLOB TEST_SRCS test/*.cpp)
set(TEST_FEAT_DESIGN   test/feature/design-pattern/stack-machine.cpp)
add_executable(feature-design-pattern ${TEST_FEAT_DESIGN})
target_link_libraries(feature-design-pattern dev gtest_main)


add_test(NAME dev_tests COMMAND feature-design-pattern)

# Add POCO
add_subdirectory(libs/poco)

# Add boost
add_subdirectory(libs/boost)

# Add dependencies
target_link_libraries(dev Poco::Foundation Poco::Util Boost::system Boost::filesystem)
