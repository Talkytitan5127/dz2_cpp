set(CMAKE_C_STANDARD 11)

file(GLOB tests "${PROJECT_SOURCE_DIR}/static_test/*.cpp")
list(REMOVE_ITEM tests "${PROJECT_SOURCE_DIR}/static_test/main.cpp")

set(ENV{TEST_FILE} "${PROJECT_SOURCE_DIR}/testfile.txt")

foreach(file ${tests})
    set(name)
    get_filename_component(name ${file} NAME_WE)
    add_executable("${name}_tests"
            ${sources}
            ${file}
            "${PROJECT_SOURCE_DIR}/static_test/main.cpp")
    target_link_libraries("${name}_tests" gtest_main)
    target_link_libraries("${name}_tests" ${PROJECT_NAME}_lib)
    add_test(NAME ${name} COMMAND "${name}_tests")
endforeach()