add_library(fs_ai STATIC fs-ai_api.c can.c)
target_link_libraries(fs_ai PRIVATE pthread m)
target_include_directories(fs_ai
    INTERFACE ${CMAKECURRENT_SOURCE_DIR}
    PRIVATE usr/include/)
