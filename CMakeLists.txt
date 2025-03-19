cmake_minimum_required(VERSION 3.30 FATAL_ERROR)
project(
    FileSystemModel
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_subdirectory(src)
# add_subdirectory(tests)

add_executable(${PROJECT_NAME} main.cpp)