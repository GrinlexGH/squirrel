cmake_minimum_required(VERSION 3.12)
project(examplelib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/.output/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/.output/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/.output/$<CONFIG>")

add_library(examplelib SHARED
    main.cpp
    vector.cpp
    vector.h
)

find_package(squirrel REQUIRED)

target_link_libraries(examplelib PRIVATE squirrel::kalibri)

add_custom_command(
    TARGET examplelib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts
    $<TARGET_FILE_DIR:examplelib>/scripts
)
