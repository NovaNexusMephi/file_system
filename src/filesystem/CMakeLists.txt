file(GLOB FILESYSTEM_LIB_HEADER_FILES "include/filesystem/*.hpp")
file(GLOB FILESYSTEM_LIB_SOURCE_FILES "source/*.cpp")

add_library(filesystem STATIC ${FILESYSTEM_LIB_HEADER_FILES} ${FILESYSTEM_LIB_SOURCE_FILES})
target_link_libraries(filesystem PRIVATE ${LIBCONFIG_LIBRARIES} lib)

target_include_directories(filesystem PUBLIC include)