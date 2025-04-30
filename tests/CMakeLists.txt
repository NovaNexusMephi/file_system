cmake_minimum_required(VERSION 3.22)
project(Tests)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

file(GLOB TESTS_SOURCE_FILES "source/*.cpp")

foreach(test_source ${TESTS_SOURCE_FILES})
    get_filename_component(test_name ${test_source} NAME_WE)
    
    add_executable(${test_name} ${test_source})
    
    target_include_directories(${test_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src
        ${GTEST_INCLUDE_DIRS}
    )
    
    target_link_libraries(${test_name} PRIVATE
        GTest::gtest
        GTest::gtest_main
        monitor
        filesystem
        commands
    )
    
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach() 