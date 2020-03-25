set(CMAKE_C_STANDARD 11)

file(GLOB tests "${PROJECT_SOURCE_DIR}/dynamic_test/*.cpp")
list(REMOVE_ITEM tests "${PROJECT_SOURCE_DIR}/dynamic_test/main.cpp")

set(ENV{TEST_FILE} "${PROJECT_SOURCE_DIR}/testfile.txt")

foreach(file ${tests})
    set(name)
    get_filename_component(name ${file} NAME_WE)
    add_executable("${name}_dynamic"
            ${sources}
            ${file}
            "${PROJECT_SOURCE_DIR}/dynamic_test/main.cpp")
    target_link_libraries("${name}_dynamic" gtest_main)
    target_link_libraries("${name}_dynamic" ${PROJECT_NAME}_dlib)
    add_test(NAME ${name} COMMAND "dynamic")
endforeach()