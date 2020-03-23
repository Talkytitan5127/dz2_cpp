cmake_minimum_required(VERSION 3.15)
project(dz2 C)

set(CMAKE_C_STANDARD 11)

add_library(${PROJECT_NAME}_lib STATIC "static_library/static_lib.c")
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${PROJECT_SOURCE_DIR}/static_library)

file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)