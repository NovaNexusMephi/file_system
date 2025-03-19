cmake_minimum_required(VERSION 3.22)
project(config)

find_package(yaml-cpp REQUIRED)

set(CONFIG_HEADER_FILES
    include/config/config.h
    include/config/fat_config.h
    include/config/info_config.h
)

set(CONFIG_SOURCE_FILES
    source/config.cpp
    source/fat_config.cpp
    source/info_config.cpp
)

add_library(config STATIC ${CONFIG_SOURCE_FILES})

if(TARGET yaml-cpp::yaml-cpp)
    target_link_libraries(config PUBLIC yaml-cpp::yaml-cpp)
else()
    target_link_libraries(config PUBLIC ${YAML_CPP_LIBRARIES})
    target_include_directories(config PUBLIC ${YAML_CPP_INCLUDE_DIRS})
endif()

target_include_directories(config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
