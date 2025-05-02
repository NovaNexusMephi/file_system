add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} config monitor commands command_builder filesystem)


add_dependencies(${PROJECT_NAME} CopyConfigFiles)