cmake_minimum_required(VERSION 2.6)
project(opengl-play)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${GLUT_INCLUDE_DIR})
if(APPLE)
include_directories(${OPENGL_INCLUDE_DIR}/Headers)
else()
include_directories(${OPENGL_INCLUDE_DIR})
endif()

set(CMAKE_C_FLAGS "-g -Wall -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS "-g -Wall -Wno-deprecated-declarations")

#add_subdirectory(bezier)
add_subdirectory(paint)
#add_subdirectory(scenegraph)
#add_subdirectory(scenegraph-camera)
#add_subdirectory(shapes)
add_subdirectory(shooting-gallery)
add_subdirectory(sierpinski)
add_subdirectory(sparks)
