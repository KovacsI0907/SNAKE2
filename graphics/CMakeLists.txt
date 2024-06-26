cmake_minimum_required(VERSION 3.10)
project(snakegraphics)

set(CMAKE_CXX_STANDARD 17)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIBRARY_DIR ${CMAKE_SOURCE_DIR}/libs)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(snakegraphics ${SOURCES})

find_package(OpenGL REQUIRED)

include_directories(${SRC_DIR})

if(WIN32)
	add_definitions(-DGLEW_STATIC -DFREEGLUT_STATIC)

	include_directories(${LIBRARY_DIR}/glew/include)
	include_directories(${LIBRARY_DIR}/freeglut/include)
	add_library(glew_static STATIC IMPORTED)
	add_library(freeglut STATIC IMPORTED)
	set_target_properties(glew_static PROPERTIES IMPORTED_LOCATION ${LIBRARY_DIR}/glew/libglew32d.lib)
	set_target_properties(freeglut PROPERTIES IMPORTED_LOCATION ${LIBRARY_DIR}/freeglut/freeglut_staticd.lib)
	
	target_include_directories(snakegraphics PRIVATE ${SRC_DIR})
	target_include_directories(snakegraphics PUBLIC ${OPENGL_INCLUDE_DIR})
	
	target_link_libraries(snakegraphics 
		${OPENGL_LIBRARIES}
		glew_static
		freeglut
	)
elseif(UNIX)
	link_directories(${GTKMM_LIBRARY_DIRS})
	include_directories(${GTKMM_INCLUDE_DIRS})
	
	target_link_libraries(snakegraphics GL glut GLU GLEW)
else()
	message(FATAL_ERROR "Unsupported platform. This project supports only Windows and Unix-based systems.")
endif()
