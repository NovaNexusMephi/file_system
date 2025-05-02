file(GLOB COMMAND_BUILDER_LIB_HEADER_FILES "include/command_builder/*.hpp")
file(GLOB COMMAND_BUILDER_LIB_SOURCE_FILES "source/*.cpp")

find_package(nlohmann_json_schema_validator REQUIRED)

add_library(command_builder STATIC ${COMMAND_BUILDER_LIB_HEADER_FILES} ${COMMAND_BUILDER_LIB_SOURCE_FILES})
target_link_libraries(command_builder PRIVATE 
                                    ${LIBCONFIG_LIBRARIES}
                                    lib
                                    filesystem
                                    commands
                                    nlohmann_json::nlohmann_json
                                    nlohmann_json_schema_validator)

target_include_directories(command_builder PUBLIC include)