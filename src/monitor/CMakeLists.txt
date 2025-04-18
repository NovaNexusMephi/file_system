file(GLOB MONITOR_HEADER_FILES "include/*.h")
file(GLOB MONITOR_SOURCE_FILES "source/*.cpp")

add_library(monitor STATIC ${MONITOR_HEADER_FILES} ${MONITOR_SOURCE_FILES})
target_link_libraries(monitor PRIVATE config)

target_include_directories(monitor PUBLIC include)