cmake_minimum_required(VERSION 3.10)
project(snakegraphics)

set(CMAKE_CXX_STANDARD 17)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIBRARY_DIR ${CMAKE_SOURCE_DIR}/libs)

add_executable(snakegraphics ${SRC_DIR}/main.cpp)

find_package(OpenGL REQUIRED)

add_library(glew_static STATIC IMPORTED)
set_target_properties(glew_static PROPERTIES IMPORTED_LOCATION ${LIBRARY_DIR}/glew/libglew32d.lib)
include_directories(${LIBRARY_DIR}/glew/include)

add_library(freeglut_static STATIC IMPORTED)
set_target_properties(freeglut_static PROPERTIES IMPORTED_LOCATION ${LIBRARY_DIR}/freeglut/freeglut_staticd.lib)
include_directories(${LIBRARY_DIR}/freeglut/include)

target_include_directories(snakegraphics PRIVATE ${SRC_DIR})
target_include_directories(snakegraphics PUBLIC ${OPENGL_INCLUDE_DIR})

target_link_libraries(snakegraphics 
	${OPENGL_LIBRARIES}
	glew_static
	freeglut_static
)
