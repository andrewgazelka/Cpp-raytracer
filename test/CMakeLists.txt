project(basic_test)

message(STATUS "Loading test")

find_package(GTest REQUIRED)

enable_testing()
add_executable(basic_test
        file_reader_test.cpp
        )
target_link_libraries(basic_test PUBLIC GTest::gtest GTest::gtest_main)
#add_test( runUnitTests runUnitTests )
