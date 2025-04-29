cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(
    FileSystemModel
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_subdirectory(configs)

add_subdirectory(src/lib)
add_subdirectory(src/config)
add_subdirectory(src/monitor)
add_subdirectory(tests)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} config monitor)


add_dependencies(${PROJECT_NAME} CopyConfigFiles)