include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.13.0 
)

FetchContent_MakeAvailable(spdlog)

file(GLOB MONITOR_HEADER_FILES "include/*.h")
file(GLOB MONITOR_SOURCE_FILES "source/*.cpp")

add_library(monitor STATIC ${MONITOR_HEADER_FILES} ${MONITOR_SOURCE_FILES})
target_link_libraries(monitor PUBLIC config spdlog::spdlog_header_only)

target_include_directories(monitor PUBLIC include)