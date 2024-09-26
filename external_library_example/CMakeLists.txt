cmake_minimum_required(VERSION 3.12)
project(test LANGUAGES C CXX)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif ()

include(GNUInstallDirs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/libs)

find_package(squirrel CONFIG REQUIRED)

add_library(examplelib SHARED main.cpp
                        vector.cpp
                        vector.h
                        )

target_link_libraries(examplelib PRIVATE squirrel::squirrel)
target_link_libraries(examplelib PRIVATE squirrel::sqstdlib)
