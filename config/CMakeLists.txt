find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig++)

file(GLOB CONFIG_HEADER_FILES "include/config/*.h")
file(GLOB CONFIG_SOURCE_FILES "source/*.cpp")

add_library(config STATIC ${CONFIG_HEADER_FILES} ${CONFIG_SOURCE_FILES})

target_link_libraries(config PUBLIC ${LIBCONFIG_LIBRARIES})
target_include_directories(config PUBLIC
    include
    ${LIBCONFIG_INCLUDE_DIRS}
)