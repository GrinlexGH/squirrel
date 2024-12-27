cmake_minimum_required(VERSION 3.12)
project(json LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/.output/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/.output/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/.output/$<CONFIG>")

add_library(json SHARED
    json.cpp
)

# Put here your installed squirrel library
set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/libs)

find_package(squirrel REQUIRED)

target_link_libraries(json PRIVATE
    squirrel::kalibri
)

# Important to specify that you are using kalibri to export your library
target_compile_definitions(json PRIVATE KALIBRI_EXPORT)

add_custom_command(
    TARGET json POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts
    $<TARGET_FILE_DIR:json>/scripts
)
