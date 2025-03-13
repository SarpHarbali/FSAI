cmake_minimum_required(VERSION 3.10)
project(MyCProject C)

# Set the C standard (e.g., C11)
set(CMAKE_C_STANDARD 11)

# Add an executable target. Replace 'mergsort.c' with your main source file.
add_executable(myprogram 
main.c
path_config.c
path_generator.c
track_generator.c
)
