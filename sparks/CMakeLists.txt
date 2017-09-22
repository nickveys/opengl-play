project(sparks)

add_executable(sparks src/sparks.c)
target_link_libraries(sparks ${GLUT_LIBRARY} ${OPENGL_LIBRARY})
