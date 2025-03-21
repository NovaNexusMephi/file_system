file(GLOB APP_HEADER_FILES "include/*.h")
file(GLOB APP_SOURCE_FILES "source/*.cpp")

add_library(app STATIC ${APP_HEADER_FILES} ${APP_SOURCE_FILES})

target_link_libraries(app PUBLIC config)
target_include_directories(app PUBLIC include)