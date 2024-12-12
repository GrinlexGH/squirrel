cmake_minimum_required(VERSION 3.12)
project(winsock2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/.output/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/.output/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/.output/$<CONFIG>")

add_library(winsock2 SHARED
    winsock.cpp
)

# Put here your installed squirrel library
set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/libs)

find_package(squirrel REQUIRED)

target_link_libraries(winsock2 PRIVATE
    squirrel::kalibri
    ws2_32
)

add_custom_command(
    TARGET winsock2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts
    $<TARGET_FILE_DIR:winsock2>/scripts
)
