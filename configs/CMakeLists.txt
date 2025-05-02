set(CONFIG_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(CONFIG_DEST_DIR "${CMAKE_BINARY_DIR}/src/dialog/configs")

file(GLOB CONFIG_FILES "${CONFIG_SOURCE_DIR}/*.json")

set(OUTPUT_FILES "")
foreach(FILE ${CONFIG_FILES})
    get_filename_component(FILENAME ${FILE} NAME)
    list(APPEND OUTPUT_FILES "${CONFIG_DEST_DIR}/${FILENAME}")
endforeach()

add_custom_command(
    OUTPUT ${OUTPUT_FILES}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CONFIG_DEST_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CONFIG_SOURCE_DIR} ${CONFIG_DEST_DIR}
    COMMENT "Copying configuration files to build directory"
)

add_custom_target(CopyConfigFiles ALL DEPENDS ${OUTPUT_FILES})