
file(GLOB CONFIG_LIB_HEADER_FILES "include/*.hpp")
file(GLOB CONFIG_LIB_SOURCE_FILES "source/*.cpp")

add_library(config STATIC ${CONFIG_LIB_HEADER_FILES} ${CONFIG_LIB_SOURCE_FILES})
target_link_libraries(config PRIVATE ${LIBCONFIG_LIBRARIES} lib)

target_include_directories(config PUBLIC include)