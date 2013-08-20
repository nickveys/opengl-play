project(sierpinski)

add_executable(sierpinski src/sierpinski.c)
target_link_libraries(sierpinski ${GLUT_LIBRARY} ${OPENGL_LIBRARY})
