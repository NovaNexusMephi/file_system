cmake_minimum_required(VERSION 3.22)
project(Tests)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

file(GLOB TESTS_SOURCE_FILES "source/*.cpp")

add_executable(Tests ${TESTS_SOURCE_FILES})

target_link_libraries(Tests gtest
                            gtest_main
                            monitor
                            config
                            ${LIBCONFIG_LIBRARIES}
)
