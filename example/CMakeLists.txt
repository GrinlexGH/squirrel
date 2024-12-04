cmake_minimum_required(VERSION 3.12)
project(examplelib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/.output/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/.output/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/.output/$<CONFIG>")

set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/libs)

add_library(examplelib SHARED
    main.cpp
    vector.cpp
    vector.h
)

find_package(squirrel REQUIRED)

target_link_libraries(examplelib PRIVATE squirrel::kalibri)
