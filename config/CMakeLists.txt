find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig++)

file(GLOB CONFIG_HEADER_FILES "include/*.h")
file(GLOB CONFIG_SOURCE_FILES "source/*.cpp")

add_library(config STATIC ${CONFIG_HEADER_FILES} ${CONFIG_SOURCE_FILES})

target_link_libraries(config PRIVATE ${LIBCONFIG_LIBRARIES})
target_include_directories(config PUBLIC include ${LIBCONFIG_LIBRARIES})
