file(GLOB LIB_HEADER_FILES "include/*.h")

add_library(lib INTERFACE ${LIB_HEADER_FILES})

target_include_directories(lib INTERFACE include)