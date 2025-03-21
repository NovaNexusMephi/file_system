cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(
    FileSystemModel
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Coping of configuration files into build directory
set(CONFIGS_SOURCES ${CMAKE_SOURCE_DIR}/configs)
set(CONFIGS_DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CONFIGS_SOURCES} DESTINATION ${CONFIGS_DESTINATION})

find_package(fmt REQUIRED)

add_subdirectory(config)

# add_subdirectory(app)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} config  fmt::fmt config++)