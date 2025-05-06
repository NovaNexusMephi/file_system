file(GLOB COMMANDS_LIB_HEADER_FILES "include/commands/*.hpp")
file(GLOB COMMANDS_LIB_SOURCE_FILES "source/*.cpp")

add_library(commands STATIC ${COMMANDS_LIB_HEADER_FILES} ${COMMANDS_LIB_SOURCE_FILES})
target_link_libraries(commands PRIVATE ${LIBCONFIG_LIBRARIES} lib filesystem)


target_include_directories(commands PUBLIC include)