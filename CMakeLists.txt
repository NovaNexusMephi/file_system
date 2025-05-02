cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(
    FileSystemModel
    VERSION 0.0.1
    LANGUAGES CXX
)

include(FetchContent)

#FetchContent_Declare(
#  nlohmann_json
#  GIT_REPOSITORY https://github.com/nlohmann/json.git
#  GIT_TAG v3.11.3
#) 

#FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  json_schema_validator
  GIT_REPOSITORY https://github.com/pboettch/json-schema-validator.git
  GIT_TAG 2.3.0 # Use the correct tag format
)
FetchContent_MakeAvailable(json_schema_validator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_subdirectory(configs)

add_subdirectory(src/lib)
add_subdirectory(src/config)
<<<<<<< HEAD
add_subdirectory(src/monitor)
=======
add_subdirectory(src/filesystem)
add_subdirectory(src/monitor)
add_subdirectory(src/commands)
add_subdirectory(src/command_builder)

>>>>>>> origin/file_system_max
add_subdirectory(tests)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} config monitor)


add_dependencies(${PROJECT_NAME} CopyConfigFiles)