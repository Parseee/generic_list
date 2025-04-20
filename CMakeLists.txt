cmake_minimum_required(VERSION 3.10)
project(My_list C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build options
add_compile_options(-Wall -Wextra)

# Source files
set(SOURCES
    src/List.h
    src/List.c
    src/List_verify.c
    src/List_node/List_node.c
    src/List_node/List_node.h)

# Create library
add_library(${PROJECT_NAME} ${SOURCES})
# target_link_libraries(${PROJECT_NAME} PRIVATE My_string)
# target_include_directories(My_list PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)