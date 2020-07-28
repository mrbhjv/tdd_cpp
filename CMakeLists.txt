cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(run_tests 
        test/dummy_test.cpp
        )

target_link_libraries(run_tests 
    ${GTEST_LIBRARY}
    ${GTEST_MAIN_LIBRARY}
    )

add_test(
    NAME run_tests
    COMMAND run_tests
)