cmake_minimum_required(VERSION 3.30 FATAL_ERROR)
project(
    FileSystemModel
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CONFIGS_SOURCES ${CMAKE_SOURCE_DIR}/configs)
set(CONFIGS_DESTINATION ${CMAKE_BINARY_DIR}/configs)

set(CONFIGS_SOURCES ${CMAKE_SOURCE_DIR}/configs)
set(CONFIGS_DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CONFIGS_SOURCES} DESTINATION ${CONFIGS_DESTINATION})


find_package(yaml-cpp REQUIRED)

add_subdirectory(config)

add_subdirectory(app)

add_executable(${PROJECT_NAME} main.cpp)
# FIXME: delete linking of config library there
target_link_libraries(${PROJECT_NAME} app config yaml-cpp::yaml-cpp)