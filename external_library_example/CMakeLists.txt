set(SQEXTLIBEXM_SRC main.cpp
                    vector.cpp
                    )

add_library(sq_external_library_example SHARED ${SQEXTLIBEXM_SRC})

target_link_libraries(sq_external_library_example squirrel::squirrel)
target_link_libraries(sq_external_library_example squirrel::sqstdlib)

