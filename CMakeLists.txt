cmake_minimum_required(VERSION 3.1)
project(dz2)

##
### Test definitions ###
##

add_compile_options("-Wall" "-Werror")

configure_file(CMakeLists.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

#static library
add_library(${PROJECT_NAME}_lib STATIC "static_library/static_lib.c")
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${PROJECT_SOURCE_DIR}/static_library)

#shared library
add_library(${PROJECT_NAME}_dlib SHARED dynamic_library/dynamic_lib.c)
target_include_directories(${PROJECT_NAME}_dlib PUBLIC ${PROJECT_SOURCE_DIR}/dynamic_library)

set(ENV{TEST_FILE} "testfile.txt")

enable_testing()
add_subdirectory(dynamic_test)
add_subdirectory(static_test)

include_directories("${PROJECT_SOURCE_DIR}/include")

#sources
file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_dlib)