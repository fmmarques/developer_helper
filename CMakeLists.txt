cmake_minimum_required(VERSION 3.10)

project(MyLibrary)

# Add include directory
include_directories(include)

# Add source directory
add_library(MyLibrary STATIC src/mylibrary.cpp)

# Clone and/or checkout master of gtest
execute_process(
    COMMAND git clone https://github.com/google/googletest.git libs/gtest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE git_clone_result
)
if(NOT git_clone_result EQUAL 0)
    execute_process(
        COMMAND git -C libs/gtest checkout main
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE git_checkout_result
    )
    if(NOT git_checkout_result EQUAL 0 AND NOT EXISTS(libs/gtest/CMakeLists.txt))
        message(FATAL_ERROR "Failed to clone or checkout gtest")
    endif()
endif()


# Add Google Test
enable_testing()
#find_package(GTest REQUIRED)
add_subdirectory(libs/gtest)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test directory
file(GLOB TEST_SRCS test/*.cpp)
add_executable(MyLibraryTests ${TEST_SRCS})
target_link_libraries(MyLibraryTests MyLibrary gtest_main)
add_test(MyLibraryTests MyLibraryTests)

# Add dependencies
find_package(Poco REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)
target_link_libraries(MyLibrary Poco::Foundation Poco::Util Boost::system Boost::filesystem)
