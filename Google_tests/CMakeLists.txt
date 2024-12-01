# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
# 'test1.cpp test2.cpp' are source files with tests
add_executable(Google_Tests tests.cpp
        array_sequence.cpp
        data_structures_tests.cpp
        priority_queue_tests.cpp
        graph_structures_tests.cpp
)
target_link_libraries(Google_Tests gtest gtest_main)
