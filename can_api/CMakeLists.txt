cmake_minimum_required(VERSION 3.31)
project(msm_can_api)
add_executable(can_dashboard main.c)

target_link_libraries(can_dashboard fs_ai)
add_subdirectory(FS-AI_API)