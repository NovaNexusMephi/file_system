find_package(Doxygen)

if(Doxygen_FOUND AND BUILD_DOC)
    message(STATUS "Documentation generation is enabled")

    # Define paths for the CSS files
    set(DOXYGEN_AWESOME_CSS "${CMAKE_CURRENT_LIST_DIR}/doxygen-awesome.css")

    # Download CSS stylesheets if they don't exist
    message(STATUS "Checking for CSS stylesheets...")
    if(NOT EXISTS ${DOXYGEN_AWESOME_CSS})
        message(STATUS "Downloading doxygen-awesome.css...")
        file(DOWNLOAD https://raw.githubusercontent.com/jothepro/doxygen-awesome-css/v2.3.4/doxygen-awesome.css
            ${DOXYGEN_AWESOME_CSS}
            SHOW_PROGRESS
        )
    else()
        message(STATUS "doxygen-awesome.css already exists. Skipping download.")
    endif()

    message(STATUS "CSS stylesheets are ready.")

    # Add a custom target for generating documentation
    add_custom_target(DocsGeneration
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/doxygen.conf
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        BYPRODUCTS ${CMAKE_BINARY_DIR}/html/index.html
    )

    # Add a post-build message
    add_custom_command(TARGET DocsGeneration POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Documentation successfully generated. You can preview at: ${CMAKE_BINARY_DIR}/html/index.html"
    )
elseif(NOT BUILD_DOC)
    message(STATUS "Documentation build is disabled (BUILD_DOC=OFF)")
else()
    message(STATUS "Documentation build is disabled or Doxygen not found")
endif()
