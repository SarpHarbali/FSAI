add_library(fs_ai STATIC fs-ai_api.c)
target_include_directories(fs_ai
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE usr/include/)