cmake_minimum_required(VERSION 3.20)
project(can_api)
add_executable(can_dashboard main.c)
add_executable(vcu vcu.c)
add_subdirectory(FS-AI_API)
target_link_libraries(can_dashboard PUBLIC fs_ai)
target_link_libraries(vcu PUBLIC fs_ai)
target_include_directories(can_dashboard PUBLIC ${PROJECT_SOURCE_DIR}/FS-AI_API)
target_include_directories(vcu PUBLIC ${PROJECT_SOURCE_DIR}/FS-AI_API)
