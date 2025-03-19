file(GLOB CONFIG_HEADER_FILES "include/config/*.h")
file(GLOB CONFIG_SOURCE_FILES "source/*.cpp")

add_library(config STATIC ${CONFIG_HEADER_FILES} ${CONFIG_SOURCE_FILES})
target_link_libraries(config PUBLIC yaml-cpp)
target_include_directories(config PUBLIC include)
