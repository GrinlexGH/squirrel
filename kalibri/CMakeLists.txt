set(KALIBRI_SRC kbimport.cpp)

if(NOT DISABLE_DYNAMIC)
  add_library(kalibri SHARED ${KALIBRI_SRC})
  add_library(squirrel::kalibri ALIAS kalibri)
  set_property(TARGET kalibri PROPERTY EXPORT_NAME kalibri)
  set_property(TARGET kalibri PROPERTY OUTPUT_NAME kalibri)
  target_link_libraries(kalibri squirrel::squirrel squirrel::sqstdlib)
  if(NOT SQ_DISABLE_INSTALLER)
    install(TARGETS kalibri EXPORT squirrel
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Libraries
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Libraries NAMELINK_SKIP
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Libraries
      )
    install(TARGETS kalibri EXPORT squirrel
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development NAMELINK_ONLY
      )
  endif()
  get_target_property(SQUIRREL_SOURCE_DIR squirrel SOURCE_DIR)
  target_include_directories(kalibri PUBLIC
    "$<BUILD_INTERFACE:${SQUIRREL_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )
endif()

if(NOT DISABLE_STATIC)
  add_library(kalibri_static STATIC ${KALIBRI_SRC})
  add_library(squirrel::kalibri_static ALIAS kalibri_static)
  set_property(TARGET kalibri_static PROPERTY EXPORT_NAME kalibri_static)
  target_link_libraries(kalibri_static squirrel::squirrel_static squirrel::sqstdlib_static)
  if(NOT SQ_DISABLE_INSTALLER)
    install(TARGETS kalibri_static EXPORT squirrel ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)
  endif()
  get_target_property(SQUIRREL_SOURCE_DIR squirrel SOURCE_DIR)
  target_include_directories(kalibri_static PUBLIC
    "$<BUILD_INTERFACE:${SQUIRREL_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )
endif()

if(LONG_OUTPUT_NAMES)
  if(NOT DISABLE_STATIC)
    set_target_properties(kalibri_static PROPERTIES OUTPUT_NAME kalibri_static)
  endif()
endif()
