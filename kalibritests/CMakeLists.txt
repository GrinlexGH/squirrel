# Build dependencies
if(WIN32)
    set(BUILD_SCRIPT_CMD cmd.exe /c ${CMAKE_CURRENT_SOURCE_DIR}/libs/build_dependencies.cmd)
    set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/bin/windows)
else()
    set(BUILD_SCRIPT_CMD bash ${CMAKE_CURRENT_SOURCE_DIR}/libs/build_dependencies.sh)
    set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/bin/linux)
endif()

execute_process(
    COMMAND ${BUILD_SCRIPT_CMD}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/
)
##########

# Project setup
set(SOURCES
    Main.cpp
    ArrayBinding.cpp
    ClassBinding.cpp
    ClassInstances.cpp
    ClassProperties.cpp
    ConstBindings.cpp
    Fixture.h
    FuncInputArgumentType.cpp
    FunctionOverload.cpp
    FunctionParams.cpp
    ImportTest.cpp
    NullPointerReturn.cpp
    RunStackHandling.cpp
    ScriptLoading.cpp
    KalibriVM.cpp
    KalibriVM2.cpp
    SquirrelFunctions.cpp
    SuspendVM.cpp
    TableBinding.cpp
    UniqueObject.cpp
    Vector.cpp
    Vector.h
)

add_executable(kalibritests ${SOURCES})

foreach(FILE IN LISTS SOURCES)
    get_filename_component(SOURCE_PATH "${FILE}" PATH)
    string(REPLACE "/" "\\" SOURCE_PATH_GROUP "${SOURCE_PATH}")
    source_group("Source Files\\${SOURCE_PATH_GROUP}" FILES "${FILE}")
endforeach()

# set project as startup project
get_directory_property(CMKR_VS_STARTUP_PROJECT DIRECTORY ${PROJECT_SOURCE_DIR} DEFINITION VS_STARTUP_PROJECT)
if(NOT CMKR_VS_STARTUP_PROJECT)
    set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT kalibritests)
endif()

target_link_libraries(kalibritests PRIVATE GTest::gtest squirrel::kalibri)
##########

# Copy script folder
add_custom_command(
    TARGET kalibritests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts
    $<TARGET_FILE_DIR:kalibritests>/scripts
)

add_custom_command(
    TARGET kalibritests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts
    ${CMAKE_CURRENT_BINARY_DIR}/scripts
)
