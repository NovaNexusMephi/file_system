find_package(yaml-cpp REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIRS})

file(GLOB CONFIG_HEADER_FILES "include/*.h")
file(GLOB CONFIG_SOURCE_FILES "src/*.cpp")
add_library(config STATIC ${CONFIG_HEADER_FILES} ${CONFIG_SOURCE_FILES})
target_link_libraries(config PRIVATE yaml-cpp)
target_include_directories(config PUBLIC include)