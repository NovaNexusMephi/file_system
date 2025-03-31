file(GLOB CONFIG_LIB_HEADER_FILES "include/*.h")
file(GLOB CONFIG_LIB_SOURCE_FILES "source/*.cpp")

add_library(config STATIC ${CONFIG_LIB_HEADER_FILES} ${CONFIG_LIB_SOURCE_FILES})

target_include_directories(config PUBLIC include)