cmake_minimum_required(VERSION 3.0.0)

enable_testing()

#project(wordle-solver)

if(EMSCRIPTEN)
	SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/web-install CACHE PATH ADH FORCE)
else()
	SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/console-install CACHE PATH ADH FORCE)
endif()

add_subdirectory(lib)

if(EMSCRIPTEN)
	add_subdirectory(html)
else()
	add_subdirectory(console)
endif()


